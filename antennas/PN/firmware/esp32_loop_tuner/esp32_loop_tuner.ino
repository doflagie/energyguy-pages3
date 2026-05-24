// ============================================================
//  esp32_loop_tuner.ino — Pool Noodle Magnetic Loop Tuner
//  TM-PNL-FW-001 Rev A
//  ESP32-WROOM-32, Bruene coupler + AD8307 x2
//  Motorized air-variable capacitor (28BYJ-48 + ULN2003, 4:1 gear)
//  WiFi AP + web UI + Bluetooth serial to CYD display
// ============================================================

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <BluetoothSerial.h>
#include <Preferences.h>
#include "config.h"

// ---- Globals ----
WebServer     server(HTTP_PORT);
BluetoothSerial BT;
Preferences   prefs;

int           g_band_idx  = 0;
long          g_cap_pos   = 0;     // half-steps from home (full-mesh = 0)
bool          g_homed     = false;
TunerState    g_state     = ST_IDLE;
float         g_cal_fwd   = CAL_OFFSET_FWD;
float         g_cal_refl  = CAL_OFFSET_REFL;
int           g_step_phase = 0;
volatile bool g_do_home   = false;
volatile bool g_do_tune   = false;

struct Measurement {
    float fwd_dbm, refl_dbm, rho, swr, return_loss_db, fwd_watts;
    bool  valid;
};
Measurement g_meas = {};

// Half-step sequence for 28BYJ-48
const uint8_t HALF_STEPS[8][4] = {
    {1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,1,1,0},
    {0,0,1,0}, {0,0,1,1}, {0,0,0,1}, {1,0,0,1}
};

// ---- ADC / SWR Measurement ----
float readADC_mV(int pin) {
    long sum = 0;
    for (int i = 0; i < ADC_OVERSAMPLE; i++) sum += analogRead(pin);
    float avg = (float)sum / ADC_OVERSAMPLE;
    return (avg / ADC_BITS) * ADC_VREF_MV;
}

float ad8307_to_dbm(float v_mv, float coupler_db, float cal) {
    float p_coupled = (v_mv / AD8307_MV_PER_DB) - AD8307_INTERCEPT;
    return p_coupled + coupler_db + cal;
}

Measurement measureSWR() {
    float sum_fwd = 0, sum_refl = 0;
    for (int i = 0; i < SWR_AVERAGE_N; i++) {
        sum_fwd  += readADC_mV(PIN_FWD_ADC);
        sum_refl += readADC_mV(PIN_REFL_ADC);
        delay(5);
    }
    Measurement m;
    m.fwd_dbm  = ad8307_to_dbm(sum_fwd  / SWR_AVERAGE_N, COUPLER_FWD_DB,  g_cal_fwd);
    m.refl_dbm = ad8307_to_dbm(sum_refl / SWR_AVERAGE_N, COUPLER_REFL_DB, g_cal_refl);

    if (m.fwd_dbm < MIN_FWD_DBM) {
        m.valid = false; m.swr = 0; m.rho = 0;
        m.return_loss_db = 0; m.fwd_watts = 0;
        return m;
    }
    m.valid = true;
    float delta_db = m.refl_dbm - m.fwd_dbm;
    if (delta_db > 0) delta_db = 0;
    m.rho = pow(10.0f, delta_db / 20.0f);
    m.rho = constrain(m.rho, 0.0f, 0.999f);
    m.swr = (1.0f + m.rho) / (1.0f - m.rho);
    m.swr = constrain(m.swr, 1.0f, SWR_MAX_DISPLAY);
    m.return_loss_db = -20.0f * log10f(m.rho);
    m.fwd_watts = pow(10.0f, (m.fwd_dbm - 30.0f) / 10.0f);
    return m;
}

// ---- Motor Control ----
void setMotorPhase(int phase) {
    digitalWrite(PIN_MOTOR_IN1, HALF_STEPS[phase][0]);
    digitalWrite(PIN_MOTOR_IN2, HALF_STEPS[phase][1]);
    digitalWrite(PIN_MOTOR_IN3, HALF_STEPS[phase][2]);
    digitalWrite(PIN_MOTOR_IN4, HALF_STEPS[phase][3]);
}

void deenergizeMotor() {
    digitalWrite(PIN_MOTOR_IN1, 0);
    digitalWrite(PIN_MOTOR_IN2, 0);
    digitalWrite(PIN_MOTOR_IN3, 0);
    digitalWrite(PIN_MOTOR_IN4, 0);
}

// dir: +1 = increase cap position (toward full mesh / max C)
//      -1 = decrease cap position (toward min mesh / min C)
void stepMotor(int dir, long steps, int delay_ms) {
    for (long i = 0; i < steps; i++) {
        // Check limits
        if (dir == -1 && digitalRead(PIN_CAP_MIN_SW) == LOW) break;
        if (dir == +1 && digitalRead(PIN_CAP_MAX_SW) == LOW) break;
        if (g_cap_pos <= 0 && dir == -1) break;
        if (g_cap_pos >= (long)MAX_CAP_STEPS && dir == +1) break;

        g_step_phase = (g_step_phase + (dir > 0 ? 1 : 7)) & 7;
        setMotorPhase(g_step_phase);
        g_cap_pos += dir;
        delay(delay_ms);
    }
    deenergizeMotor();
}

void homeCapacitor() {
    g_state = ST_HOMING;
    // Move toward full-mesh (dir = -1 = decrease position = toward home switch)
    // Home switch at full-mesh = maximum capacitance = position 0
    long safety_limit = (long)MAX_CAP_STEPS + 10000L;
    long steps_taken = 0;
    while (digitalRead(PIN_CAP_MIN_SW) != LOW && steps_taken < safety_limit) {
        g_step_phase = (g_step_phase + 7) & 7;  // reverse = toward home
        setMotorPhase(g_step_phase);
        g_cap_pos--;
        steps_taken++;
        delay(HOME_SPEED_DELAY);
    }
    deenergizeMotor();
    g_cap_pos = 0;
    g_homed = true;
    prefs.putBool(NVS_KEY_HOMED, true);
    prefs.putLong(NVS_KEY_CAP_POS, 0);

    // Move to band default position
    const BandEntry& b = BANDS[g_band_idx];
    if (b.loop_type == LOOP_STL) {
        stepMotor(+1, b.default_cap_steps, FAST_SPEED_DELAY);
    }
    g_state = (BANDS[g_band_idx].loop_type == LOOP_QUAD) ? ST_QUAD_READY : ST_IDLE;
}

void autoTune() {
    if (BANDS[g_band_idx].loop_type == LOOP_QUAD) {
        g_state = ST_QUAD_READY;
        return;
    }
    if (!g_homed) homeCapacitor();

    g_state = ST_TUNING;
    g_meas = measureSWR();
    if (!g_meas.valid) { g_state = ST_IDLE; return; }

    float swr_best = g_meas.swr;
    long  step_size = TUNE_STEP_SIZE;
    int   dir = +1;

    while (step_size >= TUNE_STEP_MIN) {
        if (swr_best <= SWR_GOOD) break;

        stepMotor(dir, step_size, TUNE_SPEED_DELAY);
        g_meas = measureSWR();
        float swr_new = g_meas.valid ? g_meas.swr : SWR_MAX_DISPLAY;

        if (swr_new < swr_best) {
            swr_best = swr_new;
        } else {
            dir = -dir;
            step_size /= 2;
        }
    }

    prefs.putLong(NVS_KEY_CAP_POS, g_cap_pos);
    g_state = ST_LOCKED;
}

// ---- LEDs ----
void updateLEDs(const Measurement& m) {
    if (!m.valid) {
        digitalWrite(PIN_LED_SWR_OK, LOW);
        digitalWrite(PIN_LED_SWR_WARN, LOW);
        digitalWrite(PIN_LED_SWR_HIGH, LOW);
        return;
    }
    digitalWrite(PIN_LED_SWR_OK,   m.swr < SWR_GOOD);
    digitalWrite(PIN_LED_SWR_WARN, m.swr >= SWR_GOOD && m.swr < SWR_WARN);
    digitalWrite(PIN_LED_SWR_HIGH, m.swr >= SWR_WARN);
}

// ---- Bluetooth ----
// Estimate C from position (linear interpolation: pos=0→C_max, pos=MAX→C_min)
float estimateCapPF() {
    const BandEntry& b = BANDS[g_band_idx];
    if (b.loop_type == LOOP_QUAD) return 0;
    float C_max = b.C_pF * 3.0f;
    float C_min = b.C_pF * 0.1f;
    float ratio = (float)g_cap_pos / (float)MAX_CAP_STEPS;
    return C_max - ratio * (C_max - C_min);
}

void sendBTPacket() {
    if (!BT.connected()) return;
    const BandEntry& b = BANDS[g_band_idx];
    char buf[200];
    snprintf(buf, sizeof(buf),
        "BAND:%s,POS:%ld,C:%.0f,SWR:%.2f,FWD:%.3fW,EFF:%.1f,STATE:%d,HOMED:%d\n",
        b.name, g_cap_pos, estimateCapPF(),
        g_meas.valid ? g_meas.swr : 0.0f,
        g_meas.valid ? g_meas.fwd_watts : 0.0f,
        b.efficiency_pct,
        (int)g_state, (int)g_homed
    );
    BT.print(buf);
}

// ---- Buttons ----
void checkButtons() {
    static uint32_t last_up = 0, last_dn = 0, last_tune = 0;
    uint32_t now = millis();

    if (digitalRead(PIN_BAND_UP) == LOW && now - last_up > 300) {
        last_up = now;
        g_band_idx = (g_band_idx + 1) % NUM_BANDS;
        prefs.putInt(NVS_KEY_BAND, g_band_idx);
        g_state = (BANDS[g_band_idx].loop_type == LOOP_QUAD) ? ST_QUAD_READY : ST_IDLE;
        if (g_homed && BANDS[g_band_idx].loop_type == LOOP_STL)
            stepMotor(g_cap_pos > BANDS[g_band_idx].default_cap_steps ? -1 : +1,
                      abs(g_cap_pos - BANDS[g_band_idx].default_cap_steps), FAST_SPEED_DELAY);
    }
    if (digitalRead(PIN_BAND_DN) == LOW && now - last_dn > 300) {
        last_dn = now;
        g_band_idx = (g_band_idx - 1 + NUM_BANDS) % NUM_BANDS;
        prefs.putInt(NVS_KEY_BAND, g_band_idx);
        g_state = (BANDS[g_band_idx].loop_type == LOOP_QUAD) ? ST_QUAD_READY : ST_IDLE;
        if (g_homed && BANDS[g_band_idx].loop_type == LOOP_STL)
            stepMotor(g_cap_pos > BANDS[g_band_idx].default_cap_steps ? -1 : +1,
                      abs(g_cap_pos - BANDS[g_band_idx].default_cap_steps), FAST_SPEED_DELAY);
    }
    if (digitalRead(PIN_TUNE_BTN) == LOW && now - last_tune > 1000) {
        last_tune = now;
        if (g_state == ST_IDLE || g_state == ST_LOCKED || g_state == ST_QUAD_READY)
            g_do_tune = true;
    }
}

// ---- Web Handlers ----
const char* stateName() {
    switch (g_state) {
        case ST_IDLE:       return "IDLE";
        case ST_HOMING:     return "HOMING";
        case ST_TUNING:     return "TUNING";
        case ST_LOCKED:     return "LOCKED";
        case ST_QUAD_READY: return "QUAD RDY";
        default:            return "UNKNOWN";
    }
}

void handleRoot() {
    const BandEntry& b = BANDS[g_band_idx];
    bool is_quad = (b.loop_type == LOOP_QUAD);
    bool hv_warn = (!is_quad && b.vcap_5w_peak > 1000.0f);
    float C_est = estimateCapPF();

    char swr_class[16];
    strcpy(swr_class, g_meas.swr < SWR_GOOD ? "swr-ok" : g_meas.swr < SWR_WARN ? "swr-warn" : "swr-hi");

    String page = "<!DOCTYPE html><html><head>"
        "<meta charset='utf-8'><meta name='viewport' content='width=device-width'>"
        "<title>PNML Tuner</title>"
        "<style>body{font-family:monospace;background:#111;color:#0f0;padding:16px}"
        "h1{color:#ff0}table{border-collapse:collapse;width:100%}"
        "td,th{border:1px solid #444;padding:6px 12px}"
        ".swr-ok{color:#0f0}.swr-warn{color:#ff0}.swr-hi{color:#f00}"
        ".btn{background:#333;color:#0f0;border:1px solid #0f0;padding:8px 16px;cursor:pointer;margin:4px}"
        ".hv{background:#600;color:#fff;padding:12px;margin:8px 0;font-size:1.1em;border:2px solid #f00}"
        ".quad{background:#003;color:#0ff;padding:8px;margin:8px 0}"
        "</style></head><body><h1>PNML TUNER</h1><table>";

    char row[256];
    snprintf(row, sizeof(row), "<tr><th>Band</th><td>%s (%.3f MHz)</td></tr>", b.name, b.freq_mhz);
    page += row;
    snprintf(row, sizeof(row), "<tr><th>Loop Type</th><td>%s</td></tr>", is_quad ? "RESONANT QUAD" : "SMALL TX LOOP");
    page += row;
    if (g_meas.valid) {
        snprintf(row, sizeof(row), "<tr><th>SWR</th><td class='%s'>%.2f : 1</td></tr>", swr_class, g_meas.swr);
        page += row;
        snprintf(row, sizeof(row), "<tr><th>Fwd Power</th><td>%.3f W (%.1f dBm)</td></tr>", g_meas.fwd_watts, g_meas.fwd_dbm);
        page += row;
        snprintf(row, sizeof(row), "<tr><th>Return Loss</th><td>%.1f dB</td></tr>", g_meas.return_loss_db);
        page += row;
    } else {
        page += "<tr><th>RF Status</th><td style='color:#ff0'>NO RF — transmit briefly to measure</td></tr>";
    }
    if (!is_quad) {
        snprintf(row, sizeof(row), "<tr><th>Cap Position</th><td>%ld steps (%.1f%%)</td></tr>",
                 g_cap_pos, g_cap_pos * 100.0f / MAX_CAP_STEPS);
        page += row;
        snprintf(row, sizeof(row), "<tr><th>Est. C</th><td>%.1f pF</td></tr>", C_est);
        page += row;
        snprintf(row, sizeof(row), "<tr><th>Q / Efficiency</th><td>%.0f / %.1f%%</td></tr>",
                 b.Q_est, b.efficiency_pct);
        page += row;
        snprintf(row, sizeof(row), "<tr><th>Bandwidth</th><td>%.1f kHz</td></tr>", b.bandwidth_khz);
        page += row;
    } else {
        snprintf(row, sizeof(row), "<tr><th>Efficiency</th><td>%.1f%%</td></tr>", b.efficiency_pct);
        page += row;
        snprintf(row, sizeof(row), "<tr><th>Bandwidth</th><td>%.0f kHz</td></tr>", b.bandwidth_khz);
        page += row;
    }
    snprintf(row, sizeof(row), "<tr><th>State</th><td>%s | Homed: %s</td></tr>",
             stateName(), g_homed ? "YES" : "NO");
    page += row;
    page += "</table>";

    if (hv_warn) {
        snprintf(row, sizeof(row),
            "<div class='hv'>!! HIGH VOLTAGE !! Cap voltage: %.0f V pk @ 5W — %.0f V pk @ 100W<br>"
            "DISCHARGE before touching cap. Bleeder resistor MUST be installed.</div>",
            b.vcap_5w_peak, b.vcap_5w_peak * 4.47f);
        page += row;
    }
    if (is_quad) {
        page += "<div class='quad'>RESONANT QUAD LOOP — No capacitor required.<br>";
        page += b.match_note;
        page += "</div>";
    } else {
        page += "<p style='color:#aaa'>";
        page += b.match_note;
        page += "</p>";
    }

    page += "<br>"
        "<button class='btn' onclick=\"location='/band/up'\">BAND ▲</button>"
        "<button class='btn' onclick=\"location='/band/dn'\">BAND ▼</button>"
        "<button class='btn' onclick=\"location='/home'\">HOME CAP</button>"
        "<button class='btn' onclick=\"location='/tune'\">AUTO TUNE</button>"
        "<button class='btn' onclick=\"location='/'\">REFRESH</button>"
        "</body></html>";

    server.send(200, "text/html", page);
}

void handleBandUp() {
    g_band_idx = (g_band_idx + 1) % NUM_BANDS;
    prefs.putInt(NVS_KEY_BAND, g_band_idx);
    g_state = (BANDS[g_band_idx].loop_type == LOOP_QUAD) ? ST_QUAD_READY : ST_IDLE;
    server.sendHeader("Location", "/"); server.send(302, "text/plain", "");
}

void handleBandDn() {
    g_band_idx = (g_band_idx - 1 + NUM_BANDS) % NUM_BANDS;
    prefs.putInt(NVS_KEY_BAND, g_band_idx);
    g_state = (BANDS[g_band_idx].loop_type == LOOP_QUAD) ? ST_QUAD_READY : ST_IDLE;
    server.sendHeader("Location", "/"); server.send(302, "text/plain", "");
}

void handleHome() {
    g_do_home = true;
    server.sendHeader("Location", "/"); server.send(302, "text/plain", "");
}

void handleTune() {
    g_do_tune = true;
    server.sendHeader("Location", "/"); server.send(302, "text/plain", "");
}

void handleApiStatus() {
    const BandEntry& b = BANDS[g_band_idx];
    char buf[512];
    snprintf(buf, sizeof(buf),
        "{\"band\":\"%s\",\"freq\":%.3f,\"loop_type\":%d,"
        "\"swr\":%.2f,\"fwd_w\":%.4f,\"fwd_dbm\":%.1f,"
        "\"refl_dbm\":%.1f,\"rl_db\":%.1f,"
        "\"cap_pos\":%ld,\"C_pF\":%.1f,"
        "\"Q\":%.0f,\"eff_pct\":%.1f,\"bw_khz\":%.2f,"
        "\"state\":%d,\"homed\":%s,\"valid\":%s}",
        b.name, b.freq_mhz, (int)b.loop_type,
        g_meas.valid ? g_meas.swr : 0.0f,
        g_meas.valid ? g_meas.fwd_watts : 0.0f,
        g_meas.valid ? g_meas.fwd_dbm : 0.0f,
        g_meas.valid ? g_meas.refl_dbm : -99.0f,
        g_meas.valid ? g_meas.return_loss_db : 0.0f,
        g_cap_pos, estimateCapPF(),
        b.Q_est, b.efficiency_pct, b.bandwidth_khz,
        (int)g_state, g_homed ? "true" : "false",
        g_meas.valid ? "true" : "false"
    );
    server.send(200, "application/json", buf);
}

void handleApiMotor() {
    if (server.method() == HTTP_POST && server.hasArg("plain")) {
        String body = server.arg("plain");
        int si = body.indexOf("\"steps\":");
        if (si >= 0) {
            long steps = body.substring(si + 8).toInt();
            int dir = (steps > 0) ? +1 : -1;
            stepMotor(dir, abs(steps), TUNE_SPEED_DELAY);
            prefs.putLong(NVS_KEY_CAP_POS, g_cap_pos);
        }
        server.send(200, "application/json", "{\"status\":\"ok\"}");
    } else {
        char buf[64];
        snprintf(buf, sizeof(buf), "{\"pos\":%ld,\"homed\":%s}", g_cap_pos, g_homed ? "true":"false");
        server.send(200, "application/json", buf);
    }
}

void handleApiCal() {
    if (server.method() == HTTP_POST && server.hasArg("plain")) {
        String body = server.arg("plain");
        int fi = body.indexOf("\"fwd\":");
        int ri = body.indexOf("\"refl\":");
        if (fi >= 0) g_cal_fwd  = body.substring(fi + 6).toFloat();
        if (ri >= 0) g_cal_refl = body.substring(ri + 7).toFloat();
        prefs.putFloat(NVS_KEY_CAL_FWD, g_cal_fwd);
        prefs.putFloat(NVS_KEY_CAL_REFL, g_cal_refl);
        server.send(200, "application/json", "{\"status\":\"ok\"}");
    } else {
        char buf[80];
        snprintf(buf, sizeof(buf), "{\"cal_fwd\":%.2f,\"cal_refl\":%.2f}", g_cal_fwd, g_cal_refl);
        server.send(200, "application/json", buf);
    }
}

// ---- Setup ----
void setup() {
    Serial.begin(115200);

    // GPIO setup
    pinMode(PIN_LED_STATUS,   OUTPUT);
    pinMode(PIN_LED_SWR_OK,   OUTPUT);
    pinMode(PIN_LED_SWR_WARN, OUTPUT);
    pinMode(PIN_LED_SWR_HIGH, OUTPUT);
    pinMode(PIN_MOTOR_IN1,    OUTPUT);
    pinMode(PIN_MOTOR_IN2,    OUTPUT);
    pinMode(PIN_MOTOR_IN3,    OUTPUT);
    pinMode(PIN_MOTOR_IN4,    OUTPUT);
    pinMode(PIN_TUNE_BTN,     INPUT_PULLUP);
    pinMode(PIN_BAND_UP,      INPUT_PULLUP);
    pinMode(PIN_BAND_DN,      INPUT_PULLUP);
    pinMode(PIN_CAP_MIN_SW,   INPUT);   // external pull-up
    pinMode(PIN_CAP_MAX_SW,   INPUT);   // external pull-up

    analogSetAttenuation(ADC_11db);
    analogReadResolution(12);

    // NVS
    prefs.begin(NVS_NAMESPACE, false);
    g_band_idx = prefs.getInt(NVS_KEY_BAND, 0);
    g_cap_pos  = prefs.getLong(NVS_KEY_CAP_POS, 0);
    g_homed    = prefs.getBool(NVS_KEY_HOMED, false);
    g_cal_fwd  = prefs.getFloat(NVS_KEY_CAL_FWD,  CAL_OFFSET_FWD);
    g_cal_refl = prefs.getFloat(NVS_KEY_CAL_REFL, CAL_OFFSET_REFL);

    if (BANDS[g_band_idx].loop_type == LOOP_QUAD) g_state = ST_QUAD_READY;

    // WiFi AP
    WiFi.softAP(WIFI_SSID, WIFI_PASS, WIFI_CHANNEL);
    Serial.printf("WiFi AP: %s  IP: %s\n", WIFI_SSID, WiFi.softAPIP().toString().c_str());

    // Web routes
    server.on("/",            handleRoot);
    server.on("/band/up",     handleBandUp);
    server.on("/band/dn",     handleBandDn);
    server.on("/home",        handleHome);
    server.on("/tune",        handleTune);
    server.on("/api/status",  handleApiStatus);
    server.on("/api/motor",   handleApiMotor);
    server.on("/api/cal",     handleApiCal);
    server.begin();

    // Bluetooth
    BT.begin(BT_DEVICE_NAME);
    Serial.printf("BT: %s\n", BT_DEVICE_NAME);

    digitalWrite(PIN_LED_STATUS, HIGH);
    Serial.println("PNML Tuner ready.");
    Serial.printf("Band: %s | Cap pos: %ld | Homed: %s\n",
                  BANDS[g_band_idx].name, g_cap_pos, g_homed ? "yes" : "no");
}

// ---- Loop ----
void loop() {
    server.handleClient();
    checkButtons();

    // Handle deferred actions from web handlers
    if (g_do_home) { g_do_home = false; homeCapacitor(); }
    if (g_do_tune) { g_do_tune = false; autoTune(); }

    static uint32_t last_meas = 0, last_bt = 0;
    static bool     led_blink = false;
    uint32_t now = millis();

    if (now - last_meas >= 100) {
        last_meas = now;
        g_meas = measureSWR();
        updateLEDs(g_meas);
        if (!g_meas.valid) {
            led_blink = !led_blink;
            digitalWrite(PIN_LED_STATUS, led_blink);
        } else {
            digitalWrite(PIN_LED_STATUS, HIGH);
        }
        // Serial diagnostic
        Serial.printf("PNML|BAND:%s|POS:%ld|C:%.1fpF|SWR:%.2f|FWD:%.3fW|STATE:%s\n",
                      BANDS[g_band_idx].name, g_cap_pos, estimateCapPF(),
                      g_meas.valid ? g_meas.swr : 0.0f,
                      g_meas.valid ? g_meas.fwd_watts : 0.0f,
                      stateName());
    }

    if (now - last_bt >= BT_INTERVAL_MS) {
        last_bt = now;
        sendBTPacket();
    }
}
