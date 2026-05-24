// ============================================================
//  esp32_pnl_tuner.ino — Pool Noodle Non-Loop Antenna Tuner
//  TM-PNL2-FW-001 Rev A
//  ESP32-WROOM-32
//
//  Controls 9-relay tap-selected loading coil for HF PNILD.
//  SWR measurement via Bruene coupler + dual AD8307.
//  Band table covers PNILD (160m-10m), J-pole (6m-1.25m),
//  Yagi (70cm-20cm). J-pole/Yagi: SWR monitor only.
//  BT serial telemetry → CYD display (TM-PNL2-FW-002).
//  Web interface via Wi-Fi AP.
// ============================================================

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "BluetoothSerial.h"
#include <Preferences.h>
#include "config.h"

// ---- Globals ----
BluetoothSerial BT;
WebServer       server(HTTP_PORT);
Preferences     prefs;

volatile int        g_band_idx  = 4;   // default: 20m
volatile TunerState g_state     = ST_IDLE;
volatile bool       g_do_tune   = false;

struct Measurement {
    float fwd_watts;
    float refl_watts;
    float swr;
    bool  valid;
};

// ============================================================
// SWR Measurement
// ============================================================
float readPower(int pin, float cal_offset_db) {
    uint32_t acc = 0;
    for (int i = 0; i < ADC_OVERSAMPLE; i++)
        acc += analogRead(pin);
    float v_mV  = (float)acc / ADC_OVERSAMPLE / ADC_BITS * ADC_VREF_MV;
    float p_dBm = (v_mV / AD8307_MV_PER_DB) - AD8307_INTERCEPT + cal_offset_db;
    // Correct for coupler factor
    p_dBm += COUPLER_FWD_DB;
    return powf(10.0f, p_dBm / 10.0f) / 1000.0f;  // watts
}

Measurement measureSWR() {
    float fwd_acc = 0.0f, refl_acc = 0.0f;
    for (int i = 0; i < SWR_AVERAGE_N; i++) {
        fwd_acc  += readPower(PIN_FWD_ADC,  CAL_OFFSET_FWD);
        refl_acc += readPower(PIN_REFL_ADC, CAL_OFFSET_REFL);
        delay(1);
    }
    float fwd_W  = fwd_acc  / SWR_AVERAGE_N;
    float refl_W = refl_acc / SWR_AVERAGE_N;

    // Valid only if forward power above noise floor
    float min_fwd_W = powf(10.0f, (MIN_FWD_DBM + COUPLER_FWD_DB) / 10.0f) / 1000.0f;
    bool  valid = (fwd_W > min_fwd_W);

    float swr = SWR_MAX_DISPLAY;
    if (valid && fwd_W > refl_W) {
        float gamma = sqrtf(refl_W / fwd_W);
        if (gamma < 0.98f)
            swr = fminf((1.0f + gamma) / (1.0f - gamma), SWR_MAX_DISPLAY);
    }
    return { fwd_W, refl_W, swr, valid };
}

// ============================================================
// Relay (74HC595) Control
// ============================================================
void setRelayTap(uint8_t tap_idx) {
    // tap_idx 0–8: one-hot bit pattern into shift register
    // tap 0xFF: all relays off (J-pole / Yagi bands)
    uint16_t bits = 0;
    if (tap_idx < N_RELAYS)
        bits = (1u << tap_idx);

    digitalWrite(PIN_SR_OE, HIGH);   // disable outputs during update
    digitalWrite(PIN_SR_LATCH, LOW);
    // Shift in MSB first (relay 8 → relay 0)
    for (int i = 15; i >= 0; i--) {
        digitalWrite(PIN_SR_DATA,  (bits >> i) & 1);
        digitalWrite(PIN_SR_CLOCK, HIGH);
        delayMicroseconds(2);
        digitalWrite(PIN_SR_CLOCK, LOW);
        delayMicroseconds(2);
    }
    digitalWrite(PIN_SR_LATCH, HIGH);
    delayMicroseconds(2);
    digitalWrite(PIN_SR_LATCH, LOW);
    digitalWrite(PIN_SR_OE, LOW);    // re-enable outputs
}

void applyBandRelay() {
    const BandEntry& b = BANDS[g_band_idx];
    setRelayTap(b.relay_tap);
}

// ============================================================
// Auto-Tune (PNILD only — scan relay taps for best SWR)
// ============================================================
void autoTune() {
    const BandEntry& band = BANDS[g_band_idx];
    if (band.ant_type != ANT_PNILD) {
        g_state = ST_PASSTHRU;
        return;
    }

    g_state = ST_TUNING;
    digitalWrite(PIN_LED_STATUS, HIGH);

    float best_swr  = 999.9f;
    int   best_tap  = band.relay_tap;  // start from band's nominal tap

    // Scan all 9 taps and find minimum SWR
    for (int tap = 0; tap < N_RELAYS; tap++) {
        setRelayTap(tap);
        delay(50);  // relay settle + RF settle time
        Measurement m = measureSWR();
        if (m.valid && m.swr < best_swr) {
            best_swr = m.swr;
            best_tap = tap;
        }
        if (m.valid && m.swr < SWR_GOOD) break;  // good enough; stop
    }

    // Set best tap
    setRelayTap(best_tap);
    g_state = (best_swr < SWR_GOOD) ? ST_LOCKED : ST_IDLE;
    digitalWrite(PIN_LED_STATUS, LOW);
    g_do_tune = false;

    prefs.begin(NVS_NAMESPACE, false);
    prefs.putInt(NVS_KEY_BAND, g_band_idx);
    prefs.end();
}

// ============================================================
// Band Change
// ============================================================
void setBand(int idx) {
    if (idx < 0 || idx >= NUM_BANDS) return;
    g_band_idx = idx;
    g_state    = (BANDS[idx].ant_type == ANT_PNILD) ? ST_IDLE : ST_PASSTHRU;
    applyBandRelay();
    updateSWRLEDs(measureSWR().swr);
}

void updateSWRLEDs(float swr) {
    bool ok   = (swr < SWR_GOOD);
    bool warn = (!ok && swr < SWR_WARN);
    bool high = (swr >= SWR_WARN);
    digitalWrite(PIN_LED_SWR_OK,   ok   ? HIGH : LOW);
    digitalWrite(PIN_LED_SWR_WARN, warn ? HIGH : LOW);
    digitalWrite(PIN_LED_SWR_HIGH, high ? HIGH : LOW);
}

// ============================================================
// Web Interface
// ============================================================
String buildDashboard() {
    const BandEntry& b = BANDS[g_band_idx];
    Measurement m = measureSWR();
    String swrColor = (m.swr < SWR_GOOD) ? "#00ff00" : (m.swr < SWR_WARN) ? "#FFE000" : "#ff2200";
    String stateStr[] = {"IDLE","TUNING","LOCKED","PASSTHRU"};

    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>"
        "<meta http-equiv='refresh' content='2'>"
        "<title>PNL Tuner</title>"
        "<style>body{background:#0a0a0a;color:#00ff00;font-family:monospace;padding:20px;}"
        "h1{color:#FFE000;} table{border-collapse:collapse;width:100%;}"
        "td,th{border:1px solid #333;padding:8px;} th{background:#1a1a1a;color:#FFE000;}"
        ".btn{background:#1a3a1a;color:#00ff00;border:1px solid #00ff00;padding:8px 18px;"
        "font-size:1em;cursor:pointer;margin:4px;border-radius:4px;}"
        ".btn:hover{background:#2a5a2a;}"
        ".warn{background:#3a2200;color:#FFE000;padding:6px;border:1px solid #FFE000;margin:8px 0;}"
        "</style></head><body>"
        "<h1>PNL Antenna Tuner</h1>"
        "<table><tr><th>Band</th><th>Ant Type</th><th>SWR</th><th>Fwd (W)</th><th>State</th></tr>"
        "<tr><td><b>" + String(b.name) + "</b></td>"
        "<td>" + String(b.ant_type == ANT_PNILD ? "PNILD" : b.ant_type == ANT_JPOLE ? "J-Pole" : "Yagi") + "</td>"
        "<td style='color:" + swrColor + "'><b>" + (m.valid ? String(m.swr, 2) : "---") + "</b></td>"
        "<td>" + String(m.fwd_watts, 3) + "</td>"
        "<td>" + stateStr[g_state] + "</td>"
        "</tr></table>";

    // PNILD info block
    if (b.ant_type == ANT_PNILD) {
        html += "<table style='margin-top:10px'>"
            "<tr><th>L_load (μH)</th><th>Arm (m)</th><th>Efficiency</th><th>BW (kHz)</th></tr>"
            "<tr><td>" + String(b.L_uH, 2) + "</td>"
            "<td>" + String(b.h_arm_m, 1) + " × 2 = " + String(b.h_arm_m*2, 1) + "m total</td>"
            "<td>" + String(b.efficiency_pct, 1) + "%</td>"
            "<td>" + String(b.bandwidth_khz, 1) + "</td>"
            "</tr></table>";
    }

    html += "<p style='color:#888'>" + String(b.match_note) + "</p>";

    html += "<div style='margin:12px 0'>"
        "<a href='/band/dn'><button class='btn'>◀ Band Down</button></a>"
        "<a href='/tune'><button class='btn'>▶ TUNE</button></a>"
        "<a href='/band/up'><button class='btn'>Band Up ▶</button></a>"
        "</div>";

    html += "</body></html>";
    return html;
}

void handleRoot()    { server.send(200, "text/html", buildDashboard()); }
void handleBandUp()  { setBand(g_band_idx + 1); server.sendHeader("Location", "/"); server.send(303); }
void handleBandDn()  { setBand(g_band_idx - 1); server.sendHeader("Location", "/"); server.send(303); }
void handleTune()    { g_do_tune = true; server.sendHeader("Location", "/"); server.send(303); }
void handleStatus()  {
    const BandEntry& b = BANDS[g_band_idx];
    Measurement m = measureSWR();
    String j = "{\"band\":\"" + String(b.name) + "\",\"ant_type\":" + String(b.ant_type) +
               ",\"swr\":" + String(m.swr, 2) + ",\"fwd_w\":" + String(m.fwd_watts, 3) +
               ",\"refl_w\":" + String(m.refl_watts, 3) + ",\"valid\":" + String(m.valid ? 1:0) +
               ",\"state\":" + String(g_state) + ",\"relay_tap\":" + String(b.relay_tap) +
               ",\"L_uH\":" + String(b.L_uH, 2) + ",\"h_arm\":" + String(b.h_arm_m, 1) +
               ",\"eff\":" + String(b.efficiency_pct, 1) + "}";
    server.send(200, "application/json", j);
}
void handleCal() {
    if (server.hasArg("fwd"))  { float v = server.arg("fwd").toFloat(); prefs.begin(NVS_NAMESPACE, false); prefs.putFloat(NVS_KEY_CAL_FWD, v); prefs.end(); }
    if (server.hasArg("refl")) { float v = server.arg("refl").toFloat(); prefs.begin(NVS_NAMESPACE, false); prefs.putFloat(NVS_KEY_CAL_REFL, v); prefs.end(); }
    server.send(200, "text/plain", "OK");
}

// ============================================================
// BT Telemetry
// ============================================================
void sendBTPacket() {
    static unsigned long last_bt = 0;
    if (millis() - last_bt < BT_INTERVAL_MS) return;
    last_bt = millis();

    const BandEntry& b = BANDS[g_band_idx];
    Measurement m = measureSWR();
    char buf[160];
    snprintf(buf, sizeof(buf),
        "BAND:%s,TYPE:%d,L:%.2f,H:%.1f,SWR:%.2f,FWD:%.3fW,EFF:%.1f,STATE:%d,TAP:%d\n",
        b.name, (int)b.ant_type, b.L_uH, b.h_arm_m,
        m.swr, m.fwd_watts, b.efficiency_pct, (int)g_state, (int)b.relay_tap);
    BT.print(buf);
}

// ============================================================
// Setup & Loop
// ============================================================
void setup() {
    Serial.begin(115200);

    // GPIO
    pinMode(PIN_FWD_ADC,  INPUT);
    pinMode(PIN_REFL_ADC, INPUT);
    pinMode(PIN_SR_DATA,  OUTPUT);
    pinMode(PIN_SR_CLOCK, OUTPUT);
    pinMode(PIN_SR_LATCH, OUTPUT);
    pinMode(PIN_SR_OE,    OUTPUT);
    pinMode(PIN_TUNE_BTN, INPUT_PULLUP);
    pinMode(PIN_BAND_UP,  INPUT_PULLUP);
    pinMode(PIN_BAND_DN,  INPUT_PULLUP);
    pinMode(PIN_LED_STATUS,  OUTPUT);
    pinMode(PIN_LED_SWR_OK,  OUTPUT);
    pinMode(PIN_LED_SWR_WARN,OUTPUT);
    pinMode(PIN_LED_SWR_HIGH,OUTPUT);

    digitalWrite(PIN_SR_OE, LOW);

    // Restore band from NVS
    prefs.begin(NVS_NAMESPACE, true);
    g_band_idx = prefs.getInt(NVS_KEY_BAND, 4);
    prefs.end();
    if (g_band_idx < 0 || g_band_idx >= NUM_BANDS) g_band_idx = 4;

    applyBandRelay();
    g_state = (BANDS[g_band_idx].ant_type == ANT_PNILD) ? ST_IDLE : ST_PASSTHRU;

    // Wi-Fi AP
    WiFi.softAP(WIFI_SSID, WIFI_PASS, WIFI_CHANNEL);
    server.on("/",          handleRoot);
    server.on("/band/up",   handleBandUp);
    server.on("/band/dn",   handleBandDn);
    server.on("/tune",      handleTune);
    server.on("/api/status",handleStatus);
    server.on("/api/cal",   handleCal);
    server.begin();

    // Bluetooth
    BT.begin(BT_DEVICE_NAME);

    Serial.println("[PNL-Tuner] Ready. Band: " + String(BANDS[g_band_idx].name));
}

void loop() {
    server.handleClient();
    sendBTPacket();

    // Button handling (debounced)
    static unsigned long last_btn = 0;
    if (millis() - last_btn > 200) {
        if (!digitalRead(PIN_TUNE_BTN)) { g_do_tune = true; last_btn = millis(); }
        if (!digitalRead(PIN_BAND_UP))  { setBand(g_band_idx + 1); last_btn = millis(); }
        if (!digitalRead(PIN_BAND_DN))  { setBand(g_band_idx - 1); last_btn = millis(); }
    }

    // Deferred tune
    if (g_do_tune) autoTune();

    // LED update
    static unsigned long last_led = 0;
    if (millis() - last_led > FPS_INTERVAL_MS) {
        Measurement m = measureSWR();
        updateSWRLEDs(m.swr);
        last_led = millis();
    }
}
