// =============================================================================
// esp32_whip_tuner.ino
// 5.6m Telescopic Whip Tuner — ESP32-WROOM-32
// Bruene coupler + dual AD8307 → SWR/power measurement
// Web UI on WiFi AP 192.168.4.1 | BT stream to CYD display
// =============================================================================

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <BluetoothSerial.h>
#include <Preferences.h>
#include <math.h>
#include "config.h"

// --- Globals -----------------------------------------------------------------
WebServer    server(HTTP_PORT);
BluetoothSerial BT;
Preferences  prefs;

int   g_band_idx  = 0;
float g_cal_fwd   = CAL_OFFSET_FWD;
float g_cal_refl  = CAL_OFFSET_REFL;

struct Measurement {
    float fwd_dbm;
    float refl_dbm;
    float rho;
    float swr;
    float return_loss_db;
    float fwd_watts;
    bool  valid;
};

Measurement g_meas = {0};

// --- Timing state ------------------------------------------------------------
static unsigned long last_meas_ms   = 0;
static unsigned long last_bt_ms     = 0;
static unsigned long last_status_ms = 0;
static bool          status_led_state = false;

// =============================================================================
// readADC_mV — 64x oversample, return millivolts
// =============================================================================
float readADC_mV(int pin) {
    uint32_t acc = 0;
    for (int i = 0; i < ADC_OVERSAMPLE_N; i++) {
        acc += analogRead(pin);
    }
    float avg = (float)acc / (float)ADC_OVERSAMPLE_N;
    return (avg / ADC_FULL_SCALE) * ADC_VREF_MV;
}

// =============================================================================
// ad8307_to_dbm — convert AD8307 output voltage to power in dBm
// V_out = slope * (P_dBm - intercept)  →  P_dBm = V_out/slope + intercept
// Add coupler coupling factor and calibration trim offset.
// =============================================================================
float ad8307_to_dbm(float v_mv, float coupler_db, float cal_offset) {
    float p_dbm = (v_mv / AD8307_SLOPE_MV_DB) + AD8307_INTERCEPT_DBM;
    return p_dbm + coupler_db + cal_offset;
}

// =============================================================================
// measureSWR — average ADC_AVERAGE_N readings, compute reflection coefficient,
//              SWR, return loss, forward power in watts.
// =============================================================================
Measurement measureSWR() {
    Measurement m;
    m.valid = false;

    float fwd_acc  = 0.0f;
    float refl_acc = 0.0f;

    for (int i = 0; i < ADC_AVERAGE_N; i++) {
        float v_fwd  = readADC_mV(PIN_FWD_ADC);
        float v_refl = readADC_mV(PIN_REFL_ADC);
        fwd_acc  += ad8307_to_dbm(v_fwd,  COUPLER_FWD_DB,  g_cal_fwd);
        refl_acc += ad8307_to_dbm(v_refl, COUPLER_REFL_DB, g_cal_refl);
    }

    m.fwd_dbm  = fwd_acc  / (float)ADC_AVERAGE_N;
    m.refl_dbm = refl_acc / (float)ADC_AVERAGE_N;

    // Require minimum forward power for a meaningful SWR reading
    if (m.fwd_dbm < MIN_FWD_DBM) {
        m.swr            = 0.0f;
        m.rho            = 0.0f;
        m.return_loss_db = 0.0f;
        m.fwd_watts      = 0.0f;
        return m;
    }

    // Reflection coefficient magnitude
    float db_diff = m.refl_dbm - m.fwd_dbm;
    m.rho = powf(10.0f, db_diff / 20.0f);
    if (m.rho >= 1.0f) m.rho = 0.999f;  // Prevent divide-by-zero / crazy SWR

    m.swr            = (1.0f + m.rho) / (1.0f - m.rho);
    m.return_loss_db = -20.0f * log10f(m.rho);   // positive dB value
    m.fwd_watts      = powf(10.0f, (m.fwd_dbm - 30.0f) / 10.0f); // dBm → W

    // Clamp SWR to display range
    if (m.swr > SWR_MAX) m.swr = SWR_MAX;

    m.valid = true;
    return m;
}

// =============================================================================
// updateLEDs — drive status LEDs based on SWR; blink status LED when no RF
// =============================================================================
void updateLEDs(const Measurement& m) {
    if (!m.valid) {
        // No RF: blink status LED, all SWR LEDs off
        unsigned long now = millis();
        if (now - last_status_ms >= 500) {
            last_status_ms = now;
            status_led_state = !status_led_state;
            digitalWrite(PIN_LED_STATUS, status_led_state ? HIGH : LOW);
        }
        digitalWrite(PIN_LED_SWR_OK,   LOW);
        digitalWrite(PIN_LED_SWR_WARN, LOW);
        digitalWrite(PIN_LED_SWR_HIGH, LOW);
        return;
    }

    // Steady status LED when RF present
    digitalWrite(PIN_LED_STATUS, HIGH);

    if (m.swr <= SWR_GOOD) {
        digitalWrite(PIN_LED_SWR_OK,   HIGH);
        digitalWrite(PIN_LED_SWR_WARN, LOW);
        digitalWrite(PIN_LED_SWR_HIGH, LOW);
    } else if (m.swr <= SWR_WARN) {
        digitalWrite(PIN_LED_SWR_OK,   LOW);
        digitalWrite(PIN_LED_SWR_WARN, HIGH);
        digitalWrite(PIN_LED_SWR_HIGH, LOW);
    } else {
        digitalWrite(PIN_LED_SWR_OK,   LOW);
        digitalWrite(PIN_LED_SWR_WARN, LOW);
        digitalWrite(PIN_LED_SWR_HIGH, HIGH);
    }
}

// =============================================================================
// sendBTPacket — send CSV telemetry packet to CYD display
// =============================================================================
void sendBTPacket() {
    if (!BT.connected()) return;

    const BandEntry& b = BANDS[g_band_idx];
    char buf[128];
    snprintf(buf, sizeof(buf),
        "BAND:%s,EXT:%.3f,COIL:%.1f,SWR:%.2f,FWD:%.3fW,REFL:%.1f,RL:%.1f,VALID:%d\n",
        b.name,
        b.extension_m,
        b.coil_L_uH,
        g_meas.valid ? g_meas.swr  : 0.0f,
        g_meas.valid ? g_meas.fwd_watts : 0.0f,
        g_meas.valid ? g_meas.refl_dbm  : -99.0f,
        g_meas.valid ? g_meas.return_loss_db : 0.0f,
        g_meas.valid ? 1 : 0
    );
    BT.print(buf);
}

// =============================================================================
// checkButtons — debounced band up/down; save selection to NVS
// =============================================================================
static unsigned long last_btn_ms = 0;

void checkButtons() {
    unsigned long now = millis();
    if (now - last_btn_ms < 300) return;  // Debounce interval

    if (digitalRead(PIN_BAND_UP) == LOW) {
        last_btn_ms = now;
        g_band_idx = (g_band_idx + 1) % NUM_BANDS;
        prefs.begin(NVS_NAMESPACE, false);
        prefs.putInt(NVS_KEY_BAND, g_band_idx);
        prefs.end();
    } else if (digitalRead(PIN_BAND_DN) == LOW) {
        last_btn_ms = now;
        g_band_idx = (g_band_idx - 1 + NUM_BANDS) % NUM_BANDS;
        prefs.begin(NVS_NAMESPACE, false);
        prefs.putInt(NVS_KEY_BAND, g_band_idx);
        prefs.end();
    }
}

// =============================================================================
// tuning_hint — returns HTML instruction string based on coil_type
// =============================================================================
String tuning_hint(const BandEntry& b) {
    switch (b.coil_type) {
        case 1:
            return String("Install ") + b.match_note +
                   "; extend whip fully (5.6m); adjust L-network trimmer for minimum SWR";
        case 2:
            return String("No loading coil; retract whip to ") +
                   String(b.extension_m, 2) + "m; add 300pF series cap at base; trim cap for minimum SWR";
        case 0:
        default:
            return String("No loading coil; retract whip to ") +
                   String(b.extension_m, 2) + "m; use direct feed; adjust gamma cap if needed";
    }
}

// =============================================================================
// SWR color for HTML
// =============================================================================
const char* swr_color(float swr) {
    if (!g_meas.valid)    return "#888888";
    if (swr <= SWR_GOOD)  return "#00ff00";
    if (swr <= SWR_WARN)  return "#ffff00";
    return "#ff4444";
}

// =============================================================================
// handleRoot — full dark-theme HTML web UI
// =============================================================================
void handleRoot() {
    const BandEntry& b = BANDS[g_band_idx];

    char swr_buf[16];
    if (g_meas.valid) {
        snprintf(swr_buf, sizeof(swr_buf), "%.2f", g_meas.swr);
    } else {
        snprintf(swr_buf, sizeof(swr_buf), "---");
    }

    String page = F(
        "<!DOCTYPE html><html><head>"
        "<meta charset='utf-8'>"
        "<meta http-equiv='refresh' content='3'>"
        "<meta name='viewport' content='width=device-width,initial-scale=1'>"
        "<title>WHP Tuner</title>"
        "<style>"
        "body{background:#000;color:#00ff00;font-family:monospace;margin:0;padding:8px;}"
        "h1,h2{color:#ffff00;margin:4px 0;}"
        ".big{font-size:2.8em;font-weight:bold;}"
        ".swr-ok{color:#00ff00;} .swr-warn{color:#ffff00;} .swr-bad{color:#ff4444;}"
        ".box{border:1px solid #333;padding:8px;margin:8px 0;}"
        ".label{color:#7bef7b;font-size:0.85em;}"
        ".note{color:#aaaaaa;font-size:0.85em;}"
        "button{background:#222;color:#00ff00;border:1px solid #00ff00;"
        "padding:8px 18px;margin:4px;font-family:monospace;font-size:1em;cursor:pointer;}"
        "button:hover{background:#003300;}"
        "table{border-collapse:collapse;width:100%;}"
        "td{padding:4px 8px;}"
        "</style></head><body>"
    );

    page += "<h1>5.6m WHIP TUNER</h1>";

    // --- Band & Frequency ---
    page += "<div class='box'>";
    page += "<span class='label'>BAND: </span>";
    page += "<span style='color:#ffff00;font-size:1.6em;font-weight:bold;'>";
    page += b.name;
    page += "</span>";
    page += "&nbsp;&nbsp;<span class='label'>FREQ: </span>";
    page += "<span style='color:#00ffff;font-size:1.2em;'>";
    page += String(b.freq_mhz, 3);
    page += " MHz</span></div>";

    // --- SWR ---
    page += "<div class='box'>";
    page += "<span class='label'>SWR&nbsp;&nbsp;&nbsp;</span>";
    page += "<span class='big' style='color:";
    page += swr_color(g_meas.swr);
    page += ";'>";
    page += swr_buf;
    page += "</span><br>";

    if (g_meas.valid) {
        page += "<table><tr>";
        page += "<td><span class='label'>FWD POWER</span><br>";
        page += "<span style='color:#00ffff;'>";
        char fwdbuf[32];
        if (g_meas.fwd_watts >= 1.0f)
            snprintf(fwdbuf, sizeof(fwdbuf), "%.2f W", g_meas.fwd_watts);
        else
            snprintf(fwdbuf, sizeof(fwdbuf), "%.0f mW", g_meas.fwd_watts * 1000.0f);
        page += fwdbuf;
        page += "</span></td>";
        page += "<td><span class='label'>RETURN LOSS</span><br>";
        page += "<span style='color:#00ffff;'>";
        page += String(g_meas.return_loss_db, 1);
        page += " dB</span></td>";
        page += "<td><span class='label'>REFL</span><br>";
        page += "<span style='color:#888;'>";
        page += String(g_meas.refl_dbm, 1);
        page += " dBm</span></td>";
        page += "</tr></table>";
    } else {
        page += "<span style='color:#888;'>NO RF — TRANSMIT BRIEFLY TO MEASURE</span>";
    }
    page += "</div>";

    // --- Setup Guide ---
    page += "<div class='box'><h2>SETUP GUIDE</h2>";
    page += "<table>";
    page += "<tr><td class='label'>EXTENSION:</td><td style='color:#ffff00;font-size:1.3em;'>";
    page += String(b.extension_m, 2);
    page += " m</td></tr>";
    page += "<tr><td class='label'>QW LENGTH:</td><td style='color:#aaaaaa;'>";
    page += String(b.qw_m, 2);
    page += " m</td></tr>";

    if (b.coil_type == 1) {
        page += "<tr><td class='label'>LOADING COIL:</td><td style='color:#ff8800;'>REQUIRED</td></tr>";
        page += "<tr><td class='label'>INDUCTANCE:</td><td style='color:#00ffff;'>";
        page += String(b.coil_L_uH, 3);
        page += " &micro;H</td></tr>";
        page += "<tr><td class='label'>CORE/WIND:</td><td style='color:#00ff88;'>";
        page += b.match_note;
        page += "</td></tr>";
    } else if (b.coil_type == 2) {
        page += "<tr><td class='label'>LOADING COIL:</td><td style='color:#00ff00;'>NONE — SERIES CAP</td></tr>";
    } else {
        page += "<tr><td class='label'>LOADING COIL:</td><td style='color:#00ff00;'>NONE — DIRECT FEED</td></tr>";
    }

    page += "</table>";
    page += "<p class='note'>TUNING: ";
    page += tuning_hint(b);
    page += "</p></div>";

    // --- Buttons ---
    page += "<div class='box'>";
    page += "<a href='/band_up'><button>BAND &#9650;</button></a>";
    page += "<a href='/band_dn'><button>BAND &#9660;</button></a>";
    page += "<a href='/'><button>REFRESH</button></a>";
    page += "</div>";

    // --- All bands quick-ref ---
    page += "<div class='box'><h2>BAND TABLE</h2><table>";
    page += "<tr><td class='label'>BAND</td><td class='label'>MHz</td>"
            "<td class='label'>EXT(m)</td><td class='label'>COIL</td></tr>";
    for (int i = 0; i < NUM_BANDS; i++) {
        const char* hl = (i == g_band_idx) ? " style='background:#002200;color:#ffff00;'" : "";
        page += "<tr";
        page += hl;
        page += "><td>";
        page += BANDS[i].name;
        page += "</td><td>";
        page += String(BANDS[i].freq_mhz, 3);
        page += "</td><td>";
        page += String(BANDS[i].extension_m, 3);
        page += "</td><td>";
        if (BANDS[i].coil_type == 1) {
            page += String(BANDS[i].coil_L_uH, 2);
            page += " uH";
        } else if (BANDS[i].coil_type == 2) {
            page += "300pF cap";
        } else {
            page += "—";
        }
        page += "</td></tr>";
    }
    page += "</table></div>";

    page += "</body></html>";

    server.send(200, "text/html", page);
}

// =============================================================================
// handleBandUp / handleBandDn
// =============================================================================
void handleBandUp() {
    g_band_idx = (g_band_idx + 1) % NUM_BANDS;
    prefs.begin(NVS_NAMESPACE, false);
    prefs.putInt(NVS_KEY_BAND, g_band_idx);
    prefs.end();
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
}

void handleBandDn() {
    g_band_idx = (g_band_idx - 1 + NUM_BANDS) % NUM_BANDS;
    prefs.begin(NVS_NAMESPACE, false);
    prefs.putInt(NVS_KEY_BAND, g_band_idx);
    prefs.end();
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
}

// =============================================================================
// handleApiStatus — JSON telemetry endpoint
// =============================================================================
void handleApiStatus() {
    const BandEntry& b = BANDS[g_band_idx];
    char json[512];
    snprintf(json, sizeof(json),
        "{"
        "\"band\":\"%s\","
        "\"freq\":%.3f,"
        "\"qw\":%.3f,"
        "\"extension\":%.3f,"
        "\"coil_type\":%d,"
        "\"coil_L\":%.3f,"
        "\"swr\":%.3f,"
        "\"fwd_w\":%.4f,"
        "\"fwd_dbm\":%.2f,"
        "\"refl_dbm\":%.2f,"
        "\"rl_db\":%.2f,"
        "\"valid\":%s"
        "}",
        b.name,
        b.freq_mhz,
        b.qw_m,
        b.extension_m,
        (int)b.coil_type,
        b.coil_L_uH,
        g_meas.valid ? g_meas.swr : 0.0f,
        g_meas.valid ? g_meas.fwd_watts : 0.0f,
        g_meas.fwd_dbm,
        g_meas.refl_dbm,
        g_meas.valid ? g_meas.return_loss_db : 0.0f,
        g_meas.valid ? "true" : "false"
    );
    server.send(200, "application/json", json);
}

// =============================================================================
// handleApiCal — GET: return cal offsets JSON; POST: set and save offsets
// =============================================================================
void handleApiCal() {
    if (server.method() == HTTP_POST) {
        if (server.hasArg("cal_fwd")) {
            g_cal_fwd = server.arg("cal_fwd").toFloat();
        }
        if (server.hasArg("cal_refl")) {
            g_cal_refl = server.arg("cal_refl").toFloat();
        }
        prefs.begin(NVS_NAMESPACE, false);
        prefs.putFloat(NVS_KEY_CALFWD, g_cal_fwd);
        prefs.putFloat(NVS_KEY_CALRFL, g_cal_refl);
        prefs.end();
    }

    char json[128];
    snprintf(json, sizeof(json),
        "{\"cal_fwd\":%.3f,\"cal_refl\":%.3f}",
        g_cal_fwd, g_cal_refl
    );
    server.send(200, "application/json", json);
}

// =============================================================================
// setup
// =============================================================================
void setup() {
    Serial.begin(115200);
    Serial.println("\n=== ESP32 Whip Tuner ===");

    // GPIO: LEDs
    pinMode(PIN_LED_STATUS,   OUTPUT);
    pinMode(PIN_LED_SWR_OK,   OUTPUT);
    pinMode(PIN_LED_SWR_WARN, OUTPUT);
    pinMode(PIN_LED_SWR_HIGH, OUTPUT);
    digitalWrite(PIN_LED_STATUS,   LOW);
    digitalWrite(PIN_LED_SWR_OK,   LOW);
    digitalWrite(PIN_LED_SWR_WARN, LOW);
    digitalWrite(PIN_LED_SWR_HIGH, LOW);

    // GPIO: Buttons (active-low, internal pull-up)
    pinMode(PIN_BAND_UP,  INPUT_PULLUP);
    pinMode(PIN_BAND_DN,  INPUT_PULLUP);
    pinMode(PIN_TUNE_BTN, INPUT_PULLUP);

    // ADC: 12-bit, 11dB attenuation (0–3.3V range)
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);

    // NVS: restore saved band and cal offsets
    prefs.begin(NVS_NAMESPACE, true);  // read-only open
    g_band_idx = prefs.getInt(NVS_KEY_BAND,   0);
    g_cal_fwd  = prefs.getFloat(NVS_KEY_CALFWD, CAL_OFFSET_FWD);
    g_cal_refl = prefs.getFloat(NVS_KEY_CALRFL, CAL_OFFSET_REFL);
    prefs.end();

    // Bounds check band index
    if (g_band_idx < 0 || g_band_idx >= NUM_BANDS) g_band_idx = 0;
    Serial.printf("Restored band: %s  cal_fwd: %.2f  cal_refl: %.2f\n",
                  BANDS[g_band_idx].name, g_cal_fwd, g_cal_refl);

    // WiFi AP
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_SSID, WIFI_PASS, WIFI_CHANNEL);
    Serial.printf("WiFi AP: %s  IP: %s\n", WIFI_SSID,
                  WiFi.softAPIP().toString().c_str());

    // HTTP routes
    server.on("/",          HTTP_GET,  handleRoot);
    server.on("/band_up",   HTTP_GET,  handleBandUp);
    server.on("/band_dn",   HTTP_GET,  handleBandDn);
    server.on("/api/status",HTTP_GET,  handleApiStatus);
    server.on("/api/cal",   HTTP_GET,  handleApiCal);
    server.on("/api/cal",   HTTP_POST, handleApiCal);
    server.begin();
    Serial.printf("HTTP server started on port %d\n", HTTP_PORT);

    // Bluetooth Serial
    BT.begin(BT_DEVICE_NAME);
    Serial.printf("BT device: %s\n", BT_DEVICE_NAME);

    // LED heartbeat: brief flash on all LEDs to confirm startup
    for (int i = 0; i < 3; i++) {
        digitalWrite(PIN_LED_STATUS,   HIGH);
        digitalWrite(PIN_LED_SWR_OK,   HIGH);
        digitalWrite(PIN_LED_SWR_WARN, HIGH);
        digitalWrite(PIN_LED_SWR_HIGH, HIGH);
        delay(120);
        digitalWrite(PIN_LED_STATUS,   LOW);
        digitalWrite(PIN_LED_SWR_OK,   LOW);
        digitalWrite(PIN_LED_SWR_WARN, LOW);
        digitalWrite(PIN_LED_SWR_HIGH, LOW);
        delay(120);
    }

    Serial.println("Setup complete.");
}

// =============================================================================
// loop
// =============================================================================
void loop() {
    server.handleClient();
    checkButtons();

    unsigned long now = millis();

    // Measure every 100ms
    if (now - last_meas_ms >= 100) {
        last_meas_ms = now;
        g_meas = measureSWR();
        updateLEDs(g_meas);
    }

    // BT stream
    if (now - last_bt_ms >= BT_INTERVAL_MS) {
        last_bt_ms = now;
        sendBTPacket();
    }
}
