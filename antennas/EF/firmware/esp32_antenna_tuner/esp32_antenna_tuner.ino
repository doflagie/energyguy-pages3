// ============================================================
//  esp32_antenna_tuner.ino — EFQW SWR Monitor & Band Guide
//  TM-EF-FW-001 Rev A
//  Reads directional coupler (AD8307), computes SWR/power,
//  serves web UI, streams BT to CYD display.
//  Hardware: ESP32-WROOM-32, AD8307 x2, Bruene coupler
// ============================================================

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <BluetoothSerial.h>
#include <Preferences.h>
#include "config.h"

// ============================================================
// Globals
// ============================================================
WebServer       server(HTTP_PORT);
BluetoothSerial BT;
Preferences     prefs;

int    g_band_idx   = 0;
float  g_cal_fwd    = CAL_OFFSET_FWD;
float  g_cal_refl   = CAL_OFFSET_REFL;

struct Measurement {
    float fwd_dbm;
    float refl_dbm;
    float rho;
    float swr;
    float return_loss_db;
    float fwd_watts;
    bool  valid;      // false if forward power too low
};

Measurement g_meas = {};

// ============================================================
// ADC reading — oversampled, converted to millivolts
// ============================================================
float readADC_mV(int pin) {
    long sum = 0;
    for (int i = 0; i < ADC_OVERSAMPLE; i++) {
        sum += analogRead(pin);
    }
    float avg = (float)sum / ADC_OVERSAMPLE;
    return (avg / ADC_BITS) * ADC_VREF_MV;
}

// ============================================================
// Convert AD8307 output voltage to power in dBm
// ============================================================
float ad8307_to_dbm(float v_mv, float coupler_db, float cal_offset) {
    float p_coupled = (v_mv / AD8307_MV_PER_DB) - AD8307_INTERCEPT;
    return p_coupled + coupler_db + cal_offset;
}

// ============================================================
// Measure SWR from both AD8307 channels
// ============================================================
Measurement measureSWR() {
    // Average N readings
    float sum_fwd_mv = 0, sum_refl_mv = 0;
    for (int i = 0; i < SWR_AVERAGE_N; i++) {
        sum_fwd_mv  += readADC_mV(PIN_FWD_ADC);
        sum_refl_mv += readADC_mV(PIN_REFL_ADC);
        delay(5);
    }
    float fwd_mv  = sum_fwd_mv  / SWR_AVERAGE_N;
    float refl_mv = sum_refl_mv / SWR_AVERAGE_N;

    Measurement m;
    m.fwd_dbm  = ad8307_to_dbm(fwd_mv,  COUPLER_FWD_DB,  g_cal_fwd);
    m.refl_dbm = ad8307_to_dbm(refl_mv, COUPLER_REFL_DB, g_cal_refl);

    if (m.fwd_dbm < MIN_FWD_DBM) {
        m.valid = false;
        m.swr   = 0;
        m.rho   = 0;
        m.return_loss_db = 0;
        m.fwd_watts = 0;
        return m;
    }

    m.valid = true;

    // ρ from log power ratio
    float delta_db = m.refl_dbm - m.fwd_dbm;
    // clamp: reflected can't exceed forward in practice
    if (delta_db > 0) delta_db = 0;
    m.rho = pow(10.0f, delta_db / 20.0f);  // voltage ratio
    m.rho = constrain(m.rho, 0.0f, 0.999f);

    m.swr = (1.0f + m.rho) / (1.0f - m.rho);
    m.swr = constrain(m.swr, 1.0f, SWR_MAX_DISPLAY);

    m.return_loss_db = -20.0f * log10f(m.rho);

    // Forward power in watts
    m.fwd_watts = pow(10.0f, (m.fwd_dbm - 30.0f) / 10.0f);

    return m;
}

// ============================================================
// LED indicator update
// ============================================================
void updateLEDs(const Measurement& m) {
    if (!m.valid) {
        digitalWrite(PIN_LED_STATUS,   HIGH);   // blink waiting
        digitalWrite(PIN_LED_SWR_OK,   LOW);
        digitalWrite(PIN_LED_SWR_WARN, LOW);
        digitalWrite(PIN_LED_SWR_HIGH, LOW);
        return;
    }
    digitalWrite(PIN_LED_SWR_OK,   m.swr <  SWR_GOOD);
    digitalWrite(PIN_LED_SWR_WARN, m.swr >= SWR_GOOD  && m.swr < SWR_WARN);
    digitalWrite(PIN_LED_SWR_HIGH, m.swr >= SWR_WARN);
}

// ============================================================
// Bluetooth packet to CYD display
// Format: "BAND:40m,SWR:1.23,FWD:4.75W,REFL:-18.2dBm,RL:14.1dB,VALID:1\n"
// ============================================================
void sendBTPacket() {
    if (!BT.connected()) return;

    char buf[160];
    const BandEntry& b = BANDS[g_band_idx];
    snprintf(buf, sizeof(buf),
        "BAND:%s,FREQ:%.3f,SWR:%.2f,FWD:%.3fW,REFL:%.1f,RL:%.1f,VALID:%d\n",
        b.name,
        b.freq_mhz,
        g_meas.valid ? g_meas.swr   : 0.0f,
        g_meas.valid ? g_meas.fwd_watts : 0.0f,
        g_meas.valid ? g_meas.refl_dbm  : -99.0f,
        g_meas.valid ? g_meas.return_loss_db : 0.0f,
        g_meas.valid ? 1 : 0
    );
    BT.print(buf);
}

// ============================================================
// Web handlers
// ============================================================
void handleRoot() {
    const BandEntry& b = BANDS[g_band_idx];
    char page[4096];
    snprintf(page, sizeof(page),
        "<!DOCTYPE html><html><head>"
        "<meta charset='utf-8'><meta name='viewport' content='width=device-width'>"
        "<title>EFQW Tuner</title>"
        "<style>body{font-family:monospace;background:#111;color:#0f0;padding:16px}"
        "h1{color:#ff0}table{border-collapse:collapse;width:100%%}"
        "td,th{border:1px solid #444;padding:6px 12px}"
        ".swr-ok{color:#0f0}.swr-warn{color:#ff0}.swr-high{color:#f00}"
        ".btn{background:#333;color:#0f0;border:1px solid #0f0;padding:8px 16px;"
        "cursor:pointer;margin:4px}"
        "</style></head><body>"
        "<h1>EFQW SWR MONITOR</h1>"
        "<table>"
        "<tr><th>Band</th><td>%s (%.3f MHz)</td></tr>"
        "<tr><th>QW Length</th><td>%.3f m</td></tr>"
        "<tr><th>SWR</th><td class='%s'>%.2f : 1</td></tr>"
        "<tr><th>Fwd Power</th><td>%.3f W (%.1f dBm)</td></tr>"
        "<tr><th>Refl Power</th><td>%.1f dBm</td></tr>"
        "<tr><th>Return Loss</th><td>%.1f dB</td></tr>"
        "<tr><th>Status</th><td>%s</td></tr>"
        "</table>"
        "<br>"
        "<button class='btn' onclick=\"location='/band/up'\">BAND ▲</button>"
        "<button class='btn' onclick=\"location='/band/dn'\">BAND ▼</button>"
        "<button class='btn' onclick=\"location='/'\">REFRESH</button>"
        "<hr><p>%s</p>"
        "</body></html>",
        b.name, b.freq_mhz,
        b.qw_m,
        (g_meas.swr < SWR_GOOD) ? "swr-ok" :
            (g_meas.swr < SWR_WARN)  ? "swr-warn" : "swr-high",
        g_meas.valid ? g_meas.swr : 0.0f,
        g_meas.valid ? g_meas.fwd_watts : 0.0f,
        g_meas.valid ? g_meas.fwd_dbm  : 0.0f,
        g_meas.valid ? g_meas.refl_dbm : -99.0f,
        g_meas.valid ? g_meas.return_loss_db : 0.0f,
        g_meas.valid ? "RF PRESENT" : "NO RF (transmit briefly to tune)",
        matchingNote(b.match_type)
    );
    server.send(200, "text/html", page);
}

const char* matchingNote(uint8_t type) {
    switch (type) {
        case 0: return "HF L-network: adjust trimmer for minimum SWR. "
                       "Rotate clockwise → more C → lower resonant freq.";
        case 1: return "VHF gamma match: slide gamma rod and adjust trimmer. "
                       "Goal: SWR &lt; 1.5";
        case 2: return "UHF direct feed: verify element length and connector. "
                       "Trim element tip by 2mm increments.";
        default: return "";
    }
}

void handleBandUp() {
    g_band_idx = (g_band_idx + 1) % NUM_BANDS;
    prefs.putInt(NVS_KEY_BAND, g_band_idx);
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
}

void handleBandDn() {
    g_band_idx = (g_band_idx - 1 + NUM_BANDS) % NUM_BANDS;
    prefs.putInt(NVS_KEY_BAND, g_band_idx);
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
}

void handleApiStatus() {
    char buf[512];
    const BandEntry& b = BANDS[g_band_idx];
    snprintf(buf, sizeof(buf),
        "{\"band\":\"%s\",\"freq\":%.3f,\"qw\":%.3f,"
        "\"swr\":%.2f,\"fwd_w\":%.4f,\"fwd_dbm\":%.1f,"
        "\"refl_dbm\":%.1f,\"rl_db\":%.1f,\"valid\":%s}",
        b.name, b.freq_mhz, b.qw_m,
        g_meas.valid ? g_meas.swr  : 0.0f,
        g_meas.valid ? g_meas.fwd_watts : 0.0f,
        g_meas.valid ? g_meas.fwd_dbm   : 0.0f,
        g_meas.valid ? g_meas.refl_dbm  : -99.0f,
        g_meas.valid ? g_meas.return_loss_db : 0.0f,
        g_meas.valid ? "true" : "false"
    );
    server.send(200, "application/json", buf);
}

void handleApiCal() {
    // POST: {"fwd": 0.5, "refl": -0.3}  — offset trim in dB
    if (server.method() == HTTP_POST && server.hasArg("plain")) {
        String body = server.arg("plain");
        // Simple parse — no JSON library dependency
        int fi = body.indexOf("\"fwd\":");
        int ri = body.indexOf("\"refl\":");
        if (fi >= 0) g_cal_fwd  = body.substring(fi+6).toFloat();
        if (ri >= 0) g_cal_refl = body.substring(ri+7).toFloat();
        prefs.putFloat(NVS_KEY_CAL_FWD,  g_cal_fwd);
        prefs.putFloat(NVS_KEY_CAL_REFL, g_cal_refl);
        server.send(200, "application/json", "{\"status\":\"ok\"}");
    } else {
        char buf[128];
        snprintf(buf, sizeof(buf),
            "{\"cal_fwd\":%.2f,\"cal_refl\":%.2f}", g_cal_fwd, g_cal_refl);
        server.send(200, "application/json", buf);
    }
}

// ============================================================
// Button handling
// ============================================================
void checkButtons() {
    static uint32_t last_up = 0, last_dn = 0;
    uint32_t now = millis();

    if (digitalRead(PIN_BAND_UP) == LOW && now - last_up > 300) {
        last_up = now;
        g_band_idx = (g_band_idx + 1) % NUM_BANDS;
        prefs.putInt(NVS_KEY_BAND, g_band_idx);
    }
    if (digitalRead(PIN_BAND_DN) == LOW && now - last_dn > 300) {
        last_dn = now;
        g_band_idx = (g_band_idx - 1 + NUM_BANDS) % NUM_BANDS;
        prefs.putInt(NVS_KEY_BAND, g_band_idx);
    }
}

// ============================================================
// Setup
// ============================================================
void setup() {
    Serial.begin(115200);

    // GPIO
    pinMode(PIN_LED_STATUS,   OUTPUT);
    pinMode(PIN_LED_SWR_OK,   OUTPUT);
    pinMode(PIN_LED_SWR_WARN, OUTPUT);
    pinMode(PIN_LED_SWR_HIGH, OUTPUT);
    pinMode(PIN_TUNE_BTN,     INPUT_PULLUP);
    pinMode(PIN_BAND_UP,      INPUT_PULLUP);
    pinMode(PIN_BAND_DN,      INPUT_PULLUP);

    // ADC: set attenuation for 0–3.3V range
    analogSetAttenuation(ADC_11db);
    analogReadResolution(12);

    // NVS
    prefs.begin(NVS_NAMESPACE, false);
    g_band_idx = prefs.getInt(NVS_KEY_BAND,  0);
    g_cal_fwd  = prefs.getFloat(NVS_KEY_CAL_FWD,  CAL_OFFSET_FWD);
    g_cal_refl = prefs.getFloat(NVS_KEY_CAL_REFL, CAL_OFFSET_REFL);

    // WiFi AP
    WiFi.softAP(WIFI_SSID, WIFI_PASS, WIFI_CHANNEL);
    Serial.printf("WiFi AP: %s  IP: %s\n", WIFI_SSID, WiFi.softAPIP().toString().c_str());

    // Web routes
    server.on("/",          handleRoot);
    server.on("/band/up",   handleBandUp);
    server.on("/band/dn",   handleBandDn);
    server.on("/api/status", handleApiStatus);
    server.on("/api/cal",   handleApiCal);
    server.begin();

    // Bluetooth
    BT.begin(BT_DEVICE_NAME);
    Serial.printf("BT: %s\n", BT_DEVICE_NAME);

    digitalWrite(PIN_LED_STATUS, HIGH);
    Serial.println("EFQW Tuner ready.");
}

// ============================================================
// Loop
// ============================================================
void loop() {
    server.handleClient();
    checkButtons();

    static uint32_t last_meas = 0;
    static uint32_t last_bt   = 0;
    static bool     led_state = false;
    uint32_t now = millis();

    // Measure every 100 ms
    if (now - last_meas >= 100) {
        last_meas = now;
        g_meas = measureSWR();
        updateLEDs(g_meas);

        // Heartbeat blink on status LED when no RF
        if (!g_meas.valid) {
            led_state = !led_state;
            digitalWrite(PIN_LED_STATUS, led_state);
        } else {
            digitalWrite(PIN_LED_STATUS, HIGH);
        }
    }

    // BT stream every BT_INTERVAL_MS
    if (now - last_bt >= BT_INTERVAL_MS) {
        last_bt = now;
        sendBTPacket();
    }
}
