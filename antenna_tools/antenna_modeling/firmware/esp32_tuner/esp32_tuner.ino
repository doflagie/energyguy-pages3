/**
 * esp32_tuner.ino — TM-TUNER-001 Rev A
 * ESP32 Automatic Antenna Tuner with NEC Model-Assisted Prediction
 *
 * Hardware:
 *   ESP32-WROOM-32
 *   L-network: 74HC595 shift register → relay matrix (8 inductance taps)
 *   Capacitor: A4988-driven stepper motor, 200 steps/rotation
 *   SWR sensor: directional coupler (-30dB) + 1N5711 detectors on ADC pins
 *   Bypass relay: direct RF path when already matched
 *
 * Libraries required (Arduino IDE):
 *   ESP32 board package >= 3.0  (espressif/arduino-esp32)
 *   ArduinoJson >= 7.0          (bblanchon/ArduinoJson)
 *   ArduinoOTA                  (included in ESP32 package)
 *
 * WiFi + BT Serial for control. Queries NEC web API for initial tuning
 * prediction, then fine-tunes by measuring SWR.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <BluetoothSerial.h>
#include <Preferences.h>
#include <ArduinoOTA.h>
#include <esp_task_wdt.h>
#include "config.h"
#include "tuner_protocol.h"

// ─── Globals ──────────────────────────────────────────────────────────────────
BluetoothSerial BT;
Preferences     prefs;

volatile bool   homeDetected    = false;
int             capCurrentSteps = 0;
int             currentLTap     = 0;
float           currentFreqMHz  = 14.225f;
char            currentAntType[32] = DEFAULT_ANT_TYPE;
bool            bypassEngaged   = false;
String          btCmdBuffer;

// ADC calibration offsets (set by CALZERO command)
int             adcFwdZero = 0;
int             adcRefZero = 0;

// ─── ISR ──────────────────────────────────────────────────────────────────────
void IRAM_ATTR onHomeDetected() {
    homeDetected = true;
}

// ─── Setup ────────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial.println("\n=== TM-TUNER-001 NEC Auto-Tuner v" FW_VERSION " ===");

    initPins();
    initPreferences();
    initWiFi();
    initBluetooth();
    initOTA();
    initWatchdog();

    Serial.println("POST: Running self-test...");
    selfTest();
    Serial.println("POST: PASS");
    Serial.printf("Antenna: %s  Freq: %.3f MHz\n", currentAntType, currentFreqMHz);
}

// ─── Loop ─────────────────────────────────────────────────────────────────────
void loop() {
    esp_task_wdt_reset();
    ArduinoOTA.handle();

    static uint32_t lastPollMs = 0;
    if (millis() - lastPollMs >= POLL_INTERVAL_MS) {
        lastPollMs = millis();
        reportStatus();
    }

    handleBTCommands();
    handleSerialCommands();
}

// ─── Pin Initialization ───────────────────────────────────────────────────────
void initPins() {
    // Shift register
    pinMode(SR_CLK_PIN,   OUTPUT);
    pinMode(SR_DATA_PIN,  OUTPUT);
    pinMode(SR_LATCH_PIN, OUTPUT);
    setLTap(0);

    // Capacitor stepper
    pinMode(CAP_STEP_PIN, OUTPUT);
    pinMode(CAP_DIR_PIN,  OUTPUT);
    digitalWrite(CAP_DIR_PIN, LOW);

    // Hall sensor home
    pinMode(CAP_HOME_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(CAP_HOME_PIN), onHomeDetected, FALLING);

    // SWR ADC
    pinMode(SWR_FWD_PIN, INPUT);
    pinMode(SWR_REF_PIN, INPUT);
    analogSetAttenuation(ADC_11db);

    // Bypass relay
    pinMode(BYPASS_RELAY_PIN, OUTPUT);
    setBypass(false);
}

// ─── Preferences ──────────────────────────────────────────────────────────────
void initPreferences() {
    prefs.begin(NVS_NAMESPACE, false);
    adcFwdZero = prefs.getInt("calFwdZero", 0);
    adcRefZero = prefs.getInt("calRefZero", 0);
    strlcpy(currentAntType,
            prefs.getString("antType", DEFAULT_ANT_TYPE).c_str(),
            sizeof(currentAntType));
}

// ─── WiFi ─────────────────────────────────────────────────────────────────────
void initWiFi() {
    Serial.printf("WiFi: connecting to %s...\n", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    uint32_t t0 = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - t0 < WIFI_TIMEOUT_MS) {
        delay(250);
        Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("\nWiFi: connected, IP=%s\n", WiFi.localIP().toString().c_str());
    } else {
        Serial.println("\nWiFi: FAILED (continuing without API)");
    }
}

// ─── Bluetooth ────────────────────────────────────────────────────────────────
void initBluetooth() {
    if (!BT.begin(BT_DEVICE_NAME)) {
        Serial.println("BT: init failed");
    } else {
        Serial.printf("BT: ready, name='%s'\n", BT_DEVICE_NAME);
    }
}

// ─── OTA ──────────────────────────────────────────────────────────────────────
void initOTA() {
    ArduinoOTA.setPassword(OTA_PASSWORD);
    ArduinoOTA.setHostname(DEVICE_NAME);
    ArduinoOTA.onStart([]() { Serial.println("OTA: start"); });
    ArduinoOTA.onEnd([]()   { Serial.println("OTA: complete"); });
    ArduinoOTA.onError([](ota_error_t err) {
        Serial.printf("OTA error[%u]\n", err); });
    ArduinoOTA.begin();
}

// ─── Watchdog ─────────────────────────────────────────────────────────────────
void initWatchdog() {
    esp_task_wdt_init(WDT_TIMEOUT_S, true);
    esp_task_wdt_add(NULL);
}

// ─── Self Test ────────────────────────────────────────────────────────────────
void selfTest() {
    // Verify ADC reads sane (should be near zero with no RF)
    int fwd = readADCAvg(SWR_FWD_PIN);
    int ref = readADCAvg(SWR_REF_PIN);
    Serial.printf("ADC: fwd=%d ref=%d (baseline)\n", fwd, ref);

    // Test shift register: cycle all L taps
    for (int i = 0; i < L_TAPS; i++) { setLTap(i); delay(10); }
    setLTap(0);

    // Home stepper
    Serial.println("Homing capacitor stepper...");
    homeCapacitor();
}

// ─── L-Tap Selector (74HC595) ────────────────────────────────────────────────
void setLTap(int tap) {
    tap = constrain(tap, 0, L_TAPS - 1);
    uint8_t data = (1 << tap);
    digitalWrite(SR_LATCH_PIN, LOW);
    shiftOut(SR_DATA_PIN, SR_CLK_PIN, MSBFIRST, data);
    digitalWrite(SR_LATCH_PIN, HIGH);
    currentLTap = tap;
    delayMicroseconds(100);
}

// ─── Capacitor Stepper Control ────────────────────────────────────────────────
void stepCap(int steps) {
    if (steps == 0) return;
    bool dir = (steps > 0);
    int  abs_steps = abs(steps);

    digitalWrite(CAP_DIR_PIN, dir ? HIGH : LOW);
    delayMicroseconds(10);

    for (int i = 0; i < abs_steps; i++) {
        // Check limits
        int target = capCurrentSteps + (dir ? 1 : -1);
        if (target < 0 || target > CAP_MAX_STEPS) break;

        digitalWrite(CAP_STEP_PIN, HIGH);
        delayMicroseconds(STEP_DELAY_US / 2);
        digitalWrite(CAP_STEP_PIN, LOW);
        delayMicroseconds(STEP_DELAY_US / 2);
        capCurrentSteps += (dir ? 1 : -1);
    }
}

void setCapSteps(int target) {
    target = constrain(target, 0, CAP_MAX_STEPS);
    stepCap(target - capCurrentSteps);
}

bool homeCapacitor() {
    homeDetected = false;
    // Back off if already at home
    if (digitalRead(CAP_HOME_PIN) == LOW) {
        stepCap(20);
        delay(200);
    }
    // Move toward home
    digitalWrite(CAP_DIR_PIN, LOW);
    uint32_t t0 = millis();
    while (!homeDetected && millis() - t0 < HOME_TIMEOUT_MS) {
        esp_task_wdt_reset();
        digitalWrite(CAP_STEP_PIN, HIGH); delayMicroseconds(STEP_DELAY_US / 2);
        digitalWrite(CAP_STEP_PIN, LOW);  delayMicroseconds(STEP_DELAY_US / 2);
    }
    if (homeDetected) {
        capCurrentSteps = 0;
        Serial.println("Cap: HOME OK");
        return true;
    }
    Serial.println("Cap: HOME TIMEOUT — hardware check required");
    return false;
}

// ─── Bypass Relay ─────────────────────────────────────────────────────────────
void setBypass(bool engage) {
    digitalWrite(BYPASS_RELAY_PIN, engage ? HIGH : LOW);
    bypassEngaged = engage;
}

// ─── SWR Measurement ──────────────────────────────────────────────────────────
int readADCAvg(int pin) {
    long sum = 0;
    for (int i = 0; i < ADC_SAMPLES; i++) {
        sum += analogRead(pin);
        delayMicroseconds(10);
    }
    return (int)(sum / ADC_SAMPLES);
}

float adcToVolts(int raw) {
    return ((float)raw / 4095.0f) * ADC_REF_MV / 1000.0f / V_DIVIDER_RATIO;
}

float voltsToPower_mW(float v_det) {
    // Detector: Schottky diode + 51Ω load + coupling factor
    // At low level: V_out ≈ (V_rf / coupling_linear) * detector_factor
    // coupling_linear = 10^(COUPLING_DB/20)
    float coupling_lin = pow(10.0f, COUPLING_DB / 20.0f);
    if (v_det <= 0.0f || coupling_lin <= 0.0f) return 0.0f;
    // V_rf at detector input = v_det / coupling_lin
    // P = V^2 / R, R = 50Ω
    float v_rf = v_det / coupling_lin;
    return (v_rf * v_rf / 50.0f) * 1000.0f;  // milliwatts
}

float measureSWR() {
    int   fwd_raw  = readADCAvg(SWR_FWD_PIN) - adcFwdZero;
    int   ref_raw  = readADCAvg(SWR_REF_PIN) - adcRefZero;
    if (fwd_raw <= 0) return 99.9f;  // No forward power

    float v_fwd    = adcToVolts(max(fwd_raw, 0));
    float v_ref    = adcToVolts(max(ref_raw, 0));
    float p_fwd    = voltsToPower_mW(v_fwd);
    float p_ref    = voltsToPower_mW(v_ref);

    if (p_fwd < 0.001f) return 99.9f;
    float gamma    = sqrt(p_ref / p_fwd);
    if (gamma >= 1.0f) return 99.9f;
    return (1.0f + gamma) / (1.0f - gamma);
}

// ─── API: Get Predicted Tune ──────────────────────────────────────────────────
bool getAPIPrediction(float freq_mhz, TuneResponse& resp) {
    if (WiFi.status() != WL_CONNECTED) return false;

    TuneRequest req;
    req.freq_mhz = freq_mhz;
    strlcpy(req.antenna_type, currentAntType, sizeof(req.antenna_type));
    req.z_r = 50.0f; req.z_x = 0.0f;

    String url = String("http://") + API_HOST + ":" + API_PORT + "/tune/predict";
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    http.setTimeout(API_TIMEOUT_MS);

    int code = http.POST(req.toJSON());
    if (code == 200) {
        resp.fromJSON(http.getString());
        Serial.printf("API: L=%d C=%d SWR_pred=%.2f conf=%.2f\n",
                      resp.l_tap, resp.c_steps, resp.predicted_swr, resp.confidence);
        http.end();
        return resp.valid;
    }
    Serial.printf("API: HTTP %d\n", code);
    http.end();
    return false;
}

// ─── Tuning Algorithm ────────────────────────────────────────────────────────
void tuneToFreq(float freq_mhz) {
    Serial.printf("TUNE: %.3f MHz (%s)\n", freq_mhz, currentAntType);
    currentFreqMHz = freq_mhz;

    // Step 1: Check if bypass is sufficient
    setBypass(false);
    delay(SETTLE_DELAY_MS);
    float swr_direct = measureSWR();
    if (swr_direct <= SWR_BYPASS_THRESH) {
        setBypass(true);
        Serial.printf("TUNE: bypass engaged (SWR=%.2f)\n", swr_direct);
        return;
    }

    // Step 2: Check NVS map
    int   map_idx  = freqToMapIndex(freq_mhz);
    TuneMapEntry entry = loadMapEntry(map_idx);
    int   start_l  = 0;
    int   start_c  = CAP_MAX_STEPS / 2;

    if (entry.freq_khz > 0) {
        // Use stored map
        start_l = entry.l_tap;
        start_c = entry.getCapSteps();
        Serial.printf("NVS: L=%d C=%d\n", start_l, start_c);
    } else {
        // Step 3: Query API
        TuneResponse api_resp;
        if (getAPIPrediction(freq_mhz, api_resp)) {
            start_l = api_resp.l_tap;
            start_c = api_resp.c_steps;
        }
    }

    // Apply initial settings
    setLTap(start_l);
    setCapSteps(start_c);
    delay(SETTLE_DELAY_MS);
    float swr = measureSWR();
    Serial.printf("TUNE: initial L=%d C=%d SWR=%.2f\n", start_l, start_c, swr);

    // Step 4: Fine-tune — sweep L and C around starting point
    float best_swr = swr;
    int   best_l   = start_l;
    int   best_c   = start_c;

    for (int dl = -FINE_TUNE_L_RANGE; dl <= FINE_TUNE_L_RANGE; dl++) {
        int l = constrain(start_l + dl, 0, L_TAPS - 1);
        setLTap(l);
        for (int dc = -FINE_TUNE_C_RANGE; dc <= FINE_TUNE_C_RANGE; dc += 2) {
            int c = constrain(start_c + dc, 0, CAP_MAX_STEPS);
            setCapSteps(c);
            delay(SETTLE_DELAY_MS / 4);
            esp_task_wdt_reset();
            float s = measureSWR();
            if (s < best_swr) {
                best_swr = s;
                best_l   = l;
                best_c   = c;
            }
            if (s < SWR_GOOD_THRESH) goto tune_done;
        }
    }

tune_done:
    setLTap(best_l);
    setCapSteps(best_c);
    Serial.printf("TUNE: done L=%d C=%d SWR=%.2f\n", best_l, best_c, best_swr);

    // Save to NVS map
    entry.freq_khz = (uint16_t)(freq_mhz * 1000);
    entry.l_tap    = best_l;
    entry.setCapSteps(best_c);
    saveMapEntry(map_idx, entry);

    // Engage bypass if SWR is excellent
    if (best_swr <= SWR_BYPASS_THRESH) {
        setBypass(true);
    }
}

// ─── NVS Tune Map ────────────────────────────────────────────────────────────
int freqToMapIndex(float freq_mhz) {
    int freq_khz = (int)(freq_mhz * 1000 + 0.5f);
    return constrain((freq_khz - MAP_FREQ_LOW_KHZ) / MAP_FREQ_STEP_KHZ,
                     0, MAP_ENTRIES - 1);
}

TuneMapEntry loadMapEntry(int idx) {
    char key[12];
    sprintf(key, "tm%04d", idx);
    TuneMapEntry e;
    memset(&e, 0, sizeof(e));
    prefs.getBytes(key, &e, sizeof(e));
    return e;
}

void saveMapEntry(int idx, const TuneMapEntry& e) {
    char key[12];
    sprintf(key, "tm%04d", idx);
    prefs.putBytes(key, &e, sizeof(e));
}

// ─── Status Report ────────────────────────────────────────────────────────────
void reportStatus() {
    float swr = measureSWR();
    TunerStatus st;
    st.swr       = swr;
    st.fwd_v     = adcToVolts(readADCAvg(SWR_FWD_PIN));
    st.ref_v     = adcToVolts(readADCAvg(SWR_REF_PIN));
    st.l_tap     = currentLTap;
    st.c_steps   = capCurrentSteps;
    st.freq_mhz  = currentFreqMHz;
    st.bypassed  = bypassEngaged;
    st.timestamp_ms = millis();

    if      (bypassEngaged)           strlcpy(st.status_str, "BYPASS",  sizeof(st.status_str));
    else if (swr < SWR_GOOD_THRESH)   strlcpy(st.status_str, "TUNED",   sizeof(st.status_str));
    else if (swr > SWR_MAX_SAFE)      strlcpy(st.status_str, "FAULT",   sizeof(st.status_str));
    else                              strlcpy(st.status_str, "RUNNING", sizeof(st.status_str));

    String json = st.toJSON();
    if (BT.hasClient()) BT.println(json);
    Serial.println(json);
}

// ─── Band Sweep ───────────────────────────────────────────────────────────────
void sweepBand(float f_start, float f_stop, float f_step) {
    Serial.printf("SWEEP: %.2f–%.2f MHz step=%.2f\n", f_start, f_stop, f_step);
    int steps = (int)((f_stop - f_start) / f_step) + 1;
    for (int i = 0; i < steps; i++) {
        float f = f_start + i * f_step;
        if (f > f_stop) f = f_stop;
        tuneToFreq(f);
        Serial.printf("SWEEP: %.3f MHz done (L=%d C=%d SWR=%.2f)\n",
                      f, currentLTap, capCurrentSteps, measureSWR());
        esp_task_wdt_reset();
    }
    Serial.println("SWEEP: complete");
}

// ─── Map Dump ─────────────────────────────────────────────────────────────────
void dumpMap() {
    String csv = "freq_khz,l_tap,c_steps\n";
    for (int i = 0; i < MAP_ENTRIES; i++) {
        TuneMapEntry e = loadMapEntry(i);
        if (e.freq_khz > 0) {
            csv += String(e.freq_khz) + "," + String(e.l_tap) + "," +
                   String(e.getCapSteps()) + "\n";
        }
    }
    if (BT.hasClient()) BT.print(csv);
    Serial.print(csv);
}

// ─── Command Parsing ──────────────────────────────────────────────────────────
void processCommand(const String& cmd) {
    String c = cmd;
    c.trim();
    c.toUpperCase();

    if (c.startsWith("TUNE ")) {
        float f = c.substring(5).toFloat();
        if (f > 1.0f && f < 50.0f) tuneToFreq(f);
    }
    else if (c.startsWith("SWEEP ")) {
        float f1 = 0, f2 = 0, fs = 0.1f;
        sscanf(c.c_str(), "SWEEP %f %f %f", &f1, &f2, &fs);
        if (f1 > 0 && f2 > f1) sweepBand(f1, f2, fs);
    }
    else if (c == "STOREBAND") {
        int idx = freqToMapIndex(currentFreqMHz);
        TuneMapEntry e;
        e.freq_khz = (uint16_t)(currentFreqMHz * 1000);
        e.l_tap    = currentLTap;
        e.setCapSteps(capCurrentSteps);
        saveMapEntry(idx, e);
        Serial.printf("STOREBAND: saved %.3f MHz L=%d C=%d\n",
                      currentFreqMHz, currentLTap, capCurrentSteps);
    }
    else if (c == "STATUS") {
        reportStatus();
    }
    else if (c == "HOME") {
        homeCapacitor();
    }
    else if (c == "BYPASS ON")  { setBypass(true);  Serial.println("Bypass: ON"); }
    else if (c == "BYPASS OFF") { setBypass(false); Serial.println("Bypass: OFF"); }
    else if (c == "GETMAP") {
        dumpMap();
    }
    else if (c.startsWith("SETANT ")) {
        String ant = c.substring(7);
        ant.toLowerCase();
        strlcpy(currentAntType, ant.c_str(), sizeof(currentAntType));
        prefs.putString("antType", currentAntType);
        Serial.printf("ANT: set to '%s'\n", currentAntType);
    }
    else if (c == "CALZERO") {
        adcFwdZero = readADCAvg(SWR_FWD_PIN);
        adcRefZero = readADCAvg(SWR_REF_PIN);
        prefs.putInt("calFwdZero", adcFwdZero);
        prefs.putInt("calRefZero", adcRefZero);
        Serial.printf("CAL: zero set fwd=%d ref=%d\n", adcFwdZero, adcRefZero);
    }
    else if (c == "RESET") {
        prefs.clear();
        Serial.println("NVS: cleared. Rebooting...");
        delay(500);
        ESP.restart();
    }
    else {
        Serial.printf("Unknown command: %s\n", cmd.c_str());
    }
}

void handleBTCommands() {
    while (BT.available()) {
        char ch = BT.read();
        if (ch == '\n' || ch == '\r') {
            if (btCmdBuffer.length() > 0) {
                processCommand(btCmdBuffer);
                btCmdBuffer = "";
            }
        } else {
            btCmdBuffer += ch;
        }
    }
}

void handleSerialCommands() {
    static String serBuf;
    while (Serial.available()) {
        char ch = Serial.read();
        if (ch == '\n' || ch == '\r') {
            if (serBuf.length() > 0) {
                processCommand(serBuf);
                serBuf = "";
            }
        } else {
            serBuf += ch;
        }
    }
}
