// ============================================================
// Screwdriver Antenna Tuner — ESP32 Main Sketch
// 28BYJ-48 stepper, AD8307 Bruene coupler, WiFi AP, BT Serial
// ============================================================

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <BluetoothSerial.h>
#include <Preferences.h>
#include "config.h"

// -----------------------------------------------------------
// Enumerations
// -----------------------------------------------------------
enum MotorDir   { DIR_UP, DIR_DOWN };
enum TuneState  { TUNE_IDLE, TUNE_HOMING, TUNE_MOVING, TUNE_SWEEPING, TUNE_LOCKED };

// -----------------------------------------------------------
// Half-step sequence for 28BYJ-48 (IN1 IN2 IN3 IN4)
// -----------------------------------------------------------
const uint8_t HALF_STEPS[8][4] = {
    {1,0,0,0},
    {1,1,0,0},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,1,0},
    {0,0,1,1},
    {0,0,0,1},
    {1,0,0,1}
};

// -----------------------------------------------------------
// Measurement result
// -----------------------------------------------------------
struct Measurement {
    float fwd_dbm;
    float refl_dbm;
    float rho;
    float swr;
    float fwd_watts;
    bool  valid;
};

// -----------------------------------------------------------
// Global State
// -----------------------------------------------------------
long       g_motor_pos   = 0;
int        g_step_phase  = 0;
bool       g_homed       = false;
bool       g_motor_moving= false;
TuneState  g_tune_state  = TUNE_IDLE;
int        g_band_idx    = 5;      // default 20m
Measurement g_last_meas  = {0, 0, 0, 9.99f, 0, false};

long       g_band_positions[NUM_BANDS]; // per-band learned positions
float      g_cal_fwd     = CAL_OFFSET_FWD;
float      g_cal_refl    = CAL_OFFSET_REFL;

unsigned long g_bt_last_tx  = 0;
unsigned long g_swr_last_ms = 0;

WebServer      server(HTTP_PORT);
BluetoothSerial SerialBT;
Preferences    prefs;

// -----------------------------------------------------------
// Forward declarations
// -----------------------------------------------------------
Measurement measureSWR();
void        updateLEDs(Measurement m, TuneState state);
void        sendBTPacket();
void        homeMotor();
float       autoTune();
void        handleRoot();
void        handleHome();
void        handleTune();
void        handleBandUp();
void        handleBandDn();
void        handleApiStatus();
void        handleApiMotor();
void        handleApiCal();
float       computeL_uH(long steps);
String      tuneStateStr(TuneState s);

// -----------------------------------------------------------
// 1. stepMotor — advance one half-step in given direction
// -----------------------------------------------------------
void stepMotor(MotorDir dir, int delay_ms) {
    // Check limit switches before moving
    if (dir == DIR_DOWN && digitalRead(PIN_HOME_SW) == LOW) {
        g_motor_pos = 0;
        return;
    }
    if (dir == DIR_UP && g_motor_pos >= MAX_STEPS) {
        return;
    }
    if (dir == DIR_UP && digitalRead(PIN_TOP_SW) == LOW) {
        return;
    }

    // Advance phase
    if (dir == DIR_UP) {
        g_step_phase = (g_step_phase + 1) & 7;
        g_motor_pos++;
    } else {
        g_step_phase = (g_step_phase + 7) & 7;  // subtract 1 mod 8
        if (g_motor_pos > 0) g_motor_pos--;
    }

    // Apply coil pattern
    digitalWrite(PIN_MOTOR_IN1, HALF_STEPS[g_step_phase][0]);
    digitalWrite(PIN_MOTOR_IN2, HALF_STEPS[g_step_phase][1]);
    digitalWrite(PIN_MOTOR_IN3, HALF_STEPS[g_step_phase][2]);
    digitalWrite(PIN_MOTOR_IN4, HALF_STEPS[g_step_phase][3]);

    delay(delay_ms);
}

// -----------------------------------------------------------
// 2. moveToPosition — move to absolute half-step target
// -----------------------------------------------------------
void moveToPosition(long target_steps, int speed_ms) {
    target_steps = constrain(target_steps, 0, MAX_STEPS);
    g_motor_moving = true;
    g_tune_state   = TUNE_MOVING;

    while (g_motor_pos != target_steps) {
        // Check limit switches
        if (g_motor_pos <= 0 && target_steps < g_motor_pos) break;
        if (digitalRead(PIN_HOME_SW) == LOW && target_steps < g_motor_pos) {
            g_motor_pos = 0;
            break;
        }
        if (digitalRead(PIN_TOP_SW) == LOW && target_steps > g_motor_pos) {
            break;
        }

        MotorDir dir = (target_steps > g_motor_pos) ? DIR_UP : DIR_DOWN;
        stepMotor(dir, speed_ms);
    }

    // De-energise coil to reduce heat
    digitalWrite(PIN_MOTOR_IN1, LOW);
    digitalWrite(PIN_MOTOR_IN2, LOW);
    digitalWrite(PIN_MOTOR_IN3, LOW);
    digitalWrite(PIN_MOTOR_IN4, LOW);

    g_motor_moving = false;
    g_tune_state   = TUNE_IDLE;
}

// -----------------------------------------------------------
// 3. homeMotor — seek home (DOWN) until limit switch trips
// -----------------------------------------------------------
void homeMotor() {
    g_tune_state = TUNE_HOMING;
    Serial.println("Homing...");

    // Move down until home switch triggers
    for (int safety = 0; safety < (MAX_STEPS + 5000); safety++) {
        if (digitalRead(PIN_HOME_SW) == LOW) {
            break;
        }
        stepMotor(DIR_DOWN, HOME_SPEED_DELAY);

        // Blink status LED during homing
        if (safety % 100 == 0) {
            digitalWrite(PIN_LED_STATUS, !digitalRead(PIN_LED_STATUS));
        }
    }

    g_motor_pos = 0;
    g_step_phase = 0;
    g_homed      = true;
    g_tune_state = TUNE_IDLE;

    // De-energise
    digitalWrite(PIN_MOTOR_IN1, LOW);
    digitalWrite(PIN_MOTOR_IN2, LOW);
    digitalWrite(PIN_MOTOR_IN3, LOW);
    digitalWrite(PIN_MOTOR_IN4, LOW);

    // Three quick blinks: homed OK
    for (int i = 0; i < 3; i++) {
        digitalWrite(PIN_LED_STATUS, HIGH);
        delay(120);
        digitalWrite(PIN_LED_STATUS, LOW);
        delay(120);
    }

    Serial.println("Homed. pos=0");

    prefs.begin(NVS_NAMESPACE, false);
    prefs.putBool(NVS_KEY_HOMED, true);
    prefs.end();
}

// -----------------------------------------------------------
// 4. readADC_mV — 64x oversampled ADC reading in millivolts
// -----------------------------------------------------------
float readADC_mV(int pin) {
    uint32_t sum = 0;
    for (int i = 0; i < ADC_OVERSAMPLE; i++) {
        sum += analogRead(pin);
    }
    float avg = (float)sum / ADC_OVERSAMPLE;
    return (avg / ADC_BITS) * ADC_VREF_MV;
}

// -----------------------------------------------------------
// 5. ad8307_to_dbm — convert AD8307 output voltage to dBm
//    V_out(mV) = slope_mV/dB × (P_in_dBm − intercept)
//    → P_dBm = V_out / slope + intercept
// -----------------------------------------------------------
float ad8307_to_dbm(float v_mv, float coupler_db, float cal_offset) {
    float dbm_at_detector = (v_mv / AD8307_MV_PER_DB) - AD8307_INTERCEPT;
    // Add coupling factor back to get power at antenna port
    return dbm_at_detector + coupler_db + cal_offset;
}

// -----------------------------------------------------------
// 6. measureSWR — sample AD8307 pair, compute SWR
// -----------------------------------------------------------
Measurement measureSWR() {
    Measurement m = {0, 0, 0, 9.99f, 0, false};

    float fwd_sum  = 0.0f;
    float refl_sum = 0.0f;
    for (int i = 0; i < SWR_AVERAGE_N; i++) {
        fwd_sum  += readADC_mV(PIN_FWD_ADC);
        refl_sum += readADC_mV(PIN_REFL_ADC);
        delay(2);
    }
    float fwd_mv  = fwd_sum  / SWR_AVERAGE_N;
    float refl_mv = refl_sum / SWR_AVERAGE_N;

    m.fwd_dbm  = ad8307_to_dbm(fwd_mv,  COUPLER_FWD_DB,  g_cal_fwd);
    m.refl_dbm = ad8307_to_dbm(refl_mv, COUPLER_REFL_DB, g_cal_refl);

    if (m.fwd_dbm < MIN_FWD_DBM) {
        m.valid = false;
        return m;
    }

    // rho = 10^((Prefl-Pfwd)/20)
    float db_diff = m.refl_dbm - m.fwd_dbm;
    m.rho = powf(10.0f, db_diff / 20.0f);
    if (m.rho > 1.0f) m.rho = 1.0f;
    if (m.rho < 0.0f) m.rho = 0.0f;

    // SWR = (1+rho)/(1-rho)
    if (m.rho < 0.9999f) {
        m.swr = (1.0f + m.rho) / (1.0f - m.rho);
    } else {
        m.swr = SWR_MAX_DISPLAY;
    }
    if (m.swr > SWR_MAX_DISPLAY) m.swr = SWR_MAX_DISPLAY;

    // Forward power
    m.fwd_watts = powf(10.0f, (m.fwd_dbm - 30.0f) / 10.0f);
    m.valid = true;
    return m;
}

// -----------------------------------------------------------
// 7. autoTune — gradient/bisection sweep for minimum SWR
// -----------------------------------------------------------
float autoTune() {
    if (!g_homed) {
        Serial.println("autoTune: not homed, skipping");
        return 9.99f;
    }

    g_tune_state = TUNE_SWEEPING;
    Serial.println("autoTune: start");

    Measurement m = measureSWR();
    if (!m.valid) {
        Serial.println("autoTune: no forward power");
        g_tune_state = TUNE_IDLE;
        return 9.99f;
    }

    float best_swr = m.swr;
    long  best_pos = g_motor_pos;
    int   step_size = TUNE_STEP_SIZE;
    int   direction = 1;           // +1 = UP, -1 = DOWN
    int   reversals = 0;
    const int MAX_REVERSALS = 8;

    while (step_size >= 185 && best_swr > SWR_GOOD && reversals < MAX_REVERSALS) {
        long next_pos = g_motor_pos + (long)(direction * step_size);
        next_pos = constrain(next_pos, 0L, (long)MAX_STEPS);

        moveToPosition(next_pos, TUNE_SPEED_DELAY);

        Measurement mn = measureSWR();
        if (!mn.valid) break;

        if (mn.swr < best_swr) {
            best_swr = mn.swr;
            best_pos = g_motor_pos;
            Serial.printf("  better: pos=%ld swr=%.2f\n", best_pos, best_swr);
        } else {
            // Worse — reverse and halve step size
            direction = -direction;
            step_size /= 2;
            reversals++;
            Serial.printf("  reversal %d: step_size=%d\n", reversals, step_size);
        }

        // Stop if can't move further in this direction
        if ((direction > 0 && g_motor_pos >= MAX_STEPS) ||
            (direction < 0 && g_motor_pos <= 0)) {
            direction = -direction;
            step_size /= 2;
            reversals++;
        }

        // Yield WiFi/BT
        yield();
    }

    // Move to best found position
    moveToPosition(best_pos, FAST_SPEED_DELAY);

    Serial.printf("autoTune done: pos=%ld swr=%.2f\n", best_pos, best_swr);

    // Save per-band position
    g_band_positions[g_band_idx] = best_pos;
    char key[12];
    snprintf(key, sizeof(key), "pos_%d", g_band_idx);
    prefs.begin(NVS_NAMESPACE, false);
    prefs.putLong(key, best_pos);
    prefs.end();

    g_tune_state = (best_swr <= SWR_GOOD) ? TUNE_LOCKED : TUNE_IDLE;
    return best_swr;
}

// -----------------------------------------------------------
// 8. updateLEDs — SWR status LEDs + fast blink during activity
// -----------------------------------------------------------
void updateLEDs(Measurement m, TuneState state) {
    // Status LED: fast blink when busy
    if (state == TUNE_HOMING || state == TUNE_SWEEPING || state == TUNE_MOVING) {
        bool blink = (millis() / 150) & 1;
        digitalWrite(PIN_LED_STATUS, blink);
    } else {
        digitalWrite(PIN_LED_STATUS, m.valid ? HIGH : LOW);
    }

    if (!m.valid) {
        digitalWrite(PIN_LED_SWR_OK,   LOW);
        digitalWrite(PIN_LED_SWR_WARN, LOW);
        digitalWrite(PIN_LED_SWR_HIGH, LOW);
        return;
    }

    bool ok   = (m.swr <= SWR_GOOD);
    bool warn = (!ok && m.swr <= SWR_WARN);
    bool high = (!ok && !warn);

    // During active tuning: blink the relevant LED
    if (state == TUNE_SWEEPING) {
        bool blink = (millis() / 200) & 1;
        digitalWrite(PIN_LED_SWR_OK,   ok   ? blink : LOW);
        digitalWrite(PIN_LED_SWR_WARN, warn ? blink : LOW);
        digitalWrite(PIN_LED_SWR_HIGH, high ? blink : LOW);
    } else {
        digitalWrite(PIN_LED_SWR_OK,   ok   ? HIGH : LOW);
        digitalWrite(PIN_LED_SWR_WARN, warn ? HIGH : LOW);
        digitalWrite(PIN_LED_SWR_HIGH, high ? HIGH : LOW);
    }
}

// -----------------------------------------------------------
// 9. computeL_uH — Wheeler short-coil inductance from step pos
//    Scaled linearly from known band calibration points.
//    At step=0 → ~0 uH; at MAX_STEPS → ~72.16 uH (80m value)
// -----------------------------------------------------------
float computeL_uH(long steps) {
    if (steps <= 0) return 0.0f;
    // Simple linear interpolation across the calibrated table
    // Walk from highest step entry downward to bracket
    for (int i = 0; i < NUM_BANDS - 1; i++) {
        long s_lo = BANDS[i + 1].default_steps;
        long s_hi = BANDS[i].default_steps;
        float l_lo = BANDS[i + 1].coil_L_uH;
        float l_hi = BANDS[i].coil_L_uH;
        if (s_lo == 0 && s_hi == 0) continue;
        if (steps >= s_lo && steps <= s_hi && s_hi > s_lo) {
            float frac = (float)(steps - s_lo) / (float)(s_hi - s_lo);
            return l_lo + frac * (l_hi - l_lo);
        }
    }
    // Below lowest HF entry
    if (steps < BANDS[9].default_steps) {
        // Linear from 0 to 10m entry
        float frac = (float)steps / (float)BANDS[9].default_steps;
        return frac * BANDS[9].coil_L_uH;
    }
    return BANDS[1].coil_L_uH;  // Max = 80m value
}

// -----------------------------------------------------------
// 10. sendBTPacket — format telemetry for CYD display
// -----------------------------------------------------------
void sendBTPacket() {
    if (!SerialBT.connected()) return;
    float turns = (float)g_motor_pos / STEPS_PER_COIL_TURN;
    float L_uH  = computeL_uH(g_motor_pos);
    float eff   = BANDS[g_band_idx].efficiency_pct;
    char buf[160];
    snprintf(buf, sizeof(buf),
        "BAND:%s,POS:%ld,TURNS:%.2f,L:%.3f,EFF:%.1f,SWR:%.2f,FWD:%.3fW,REFL:%.1f,STATE:%d,HOMED:%d\n",
        BANDS[g_band_idx].name,
        g_motor_pos,
        turns,
        L_uH,
        eff,
        g_last_meas.swr,
        g_last_meas.fwd_watts,
        g_last_meas.refl_dbm,
        (int)g_tune_state,
        (int)g_homed
    );
    SerialBT.print(buf);
}

// -----------------------------------------------------------
// 11. checkButtons — debounced band/tune button handler
// -----------------------------------------------------------
void checkButtons() {
    static unsigned long last_up   = 0;
    static unsigned long last_dn   = 0;
    static unsigned long last_tune = 0;
    const unsigned long DEBOUNCE_MS = 250;
    unsigned long now = millis();

    if (digitalRead(PIN_BAND_UP) == LOW && (now - last_up) > DEBOUNCE_MS) {
        last_up = now;
        if (g_band_idx > 0) {
            g_band_idx--;
            Serial.printf("Band UP → %s\n", BANDS[g_band_idx].name);
            g_tune_state = TUNE_MOVING;
            moveToPosition(g_band_positions[g_band_idx], FAST_SPEED_DELAY);
            autoTune();
        }
    }

    if (digitalRead(PIN_BAND_DN) == LOW && (now - last_dn) > DEBOUNCE_MS) {
        last_dn = now;
        if (g_band_idx < NUM_BANDS - 1) {
            g_band_idx++;
            Serial.printf("Band DN → %s\n", BANDS[g_band_idx].name);
            g_tune_state = TUNE_MOVING;
            moveToPosition(g_band_positions[g_band_idx], FAST_SPEED_DELAY);
            autoTune();
        }
    }

    if (digitalRead(PIN_TUNE_BTN) == LOW && (now - last_tune) > DEBOUNCE_MS) {
        last_tune = now;
        Serial.println("TUNE button pressed");
        autoTune();
    }
}

// -----------------------------------------------------------
// Helper: tune state to string
// -----------------------------------------------------------
String tuneStateStr(TuneState s) {
    switch (s) {
        case TUNE_IDLE:     return "IDLE";
        case TUNE_HOMING:   return "HOMING";
        case TUNE_MOVING:   return "MOVING";
        case TUNE_SWEEPING: return "SWEEPING";
        case TUNE_LOCKED:   return "LOCKED";
    }
    return "UNKNOWN";
}

// -----------------------------------------------------------
// Helper: SWR color for HTML
// -----------------------------------------------------------
String swrColor(float swr) {
    if (swr <= SWR_GOOD) return "#00ff88";
    if (swr <= SWR_WARN) return "#ffcc00";
    return "#ff4444";
}

// -----------------------------------------------------------
// 12. handleRoot — full dark-theme HTML web UI
// -----------------------------------------------------------
void handleRoot() {
    float turns = (float)g_motor_pos / STEPS_PER_COIL_TURN;
    float L_uH  = computeL_uH(g_motor_pos);
    String swr_col = swrColor(g_last_meas.swr);
    String state_str = tuneStateStr(g_tune_state);
    const BandEntry& band = BANDS[g_band_idx];

    String html = F("<!DOCTYPE html><html><head>");
    html += F("<meta charset='utf-8'>");
    html += F("<meta name='viewport' content='width=device-width,initial-scale=1'>");
    html += F("<meta http-equiv='refresh' content='2'>");
    html += F("<title>SC Tuner</title>");
    html += F("<style>"
              "body{background:#111;color:#ddd;font-family:monospace;margin:0;padding:10px;}"
              "h1{color:#88ccff;margin:0 0 8px 0;}"
              ".card{background:#1e1e1e;border:1px solid #333;border-radius:8px;padding:12px;margin:8px 0;}"
              ".big{font-size:3em;font-weight:bold;}"
              ".med{font-size:1.5em;}"
              ".label{color:#888;font-size:0.85em;}"
              ".row{display:flex;justify-content:space-between;margin:4px 0;}"
              "button{background:#2a4a7f;color:#fff;border:none;border-radius:6px;"
              "padding:10px 18px;margin:4px;font-size:1em;cursor:pointer;}"
              "button:hover{background:#3a6abf;}"
              ".btn-home{background:#5a2a2a;}"
              ".btn-tune{background:#2a5a2a;}"
              ".warn{color:#ffcc00;} .ok{color:#00ff88;} .bad{color:#ff4444;}"
              ".bar-bg{background:#333;border-radius:4px;height:18px;margin:6px 0;}"
              ".bar-fill{height:18px;border-radius:4px;transition:width 0.5s;}"
              "</style></head><body>");

    html += F("<h1>&#128246; Screwdriver Tuner</h1>");

    // Band / frequency card
    html += "<div class='card'>";
    html += "<div class='row'><span class='label'>BAND</span><span class='med'>";
    html += String(band.name) + " — " + String(band.freq_mhz, 3) + " MHz</span></div>";
    html += "<div class='row'><span class='label'>EFFICIENCY</span><span>";
    html += String(band.efficiency_pct, 1) + "%</span></div>";
    html += "</div>";

    // SWR card
    html += "<div class='card'>";
    html += "<div class='label'>SWR</div>";
    html += "<div class='big' style='color:" + swr_col + "'>";
    if (g_last_meas.valid) {
        char swr_buf[16];
        snprintf(swr_buf, sizeof(swr_buf), "%.2f", g_last_meas.swr);
        html += String(swr_buf);
    } else {
        html += "---";
    }
    html += "</div>";
    if (g_last_meas.valid) {
        html += "<div class='row'><span class='label'>Fwd</span><span>";
        html += String(g_last_meas.fwd_dbm, 1) + " dBm / ";
        html += String(g_last_meas.fwd_watts * 1000.0f, 1) + " mW</span></div>";
        html += "<div class='row'><span class='label'>Refl</span><span>";
        html += String(g_last_meas.refl_dbm, 1) + " dBm</span></div>";
        html += "<div class='row'><span class='label'>&#961;</span><span>";
        html += String(g_last_meas.rho, 3) + "</span></div>";
    }
    html += "<div class='row'><span class='label'>STATE</span><span>" + state_str + "</span></div>";
    html += "</div>";

    // Motor position card
    html += "<div class='card'>";
    html += "<div class='label'>MOTOR POSITION</div>";
    html += "<div class='row'><span>Turns</span><span class='med'>" + String(turns, 2) + " / 41.00</span></div>";
    html += "<div class='row'><span>Steps</span><span>" + String(g_motor_pos) + " / " + String(MAX_STEPS) + "</span></div>";
    html += "<div class='row'><span>Inductance</span><span>" + String(L_uH, 3) + " &#956;H</span></div>";
    html += "<div class='row'><span>Homed</span><span>";
    html += g_homed ? "<span class='ok'>YES</span>" : "<span class='bad'>NO</span>";
    html += "</span></div>";

    // Progress bar
    int pct = (int)(((float)g_motor_pos / MAX_STEPS) * 100.0f);
    html += "<div class='bar-bg'><div class='bar-fill' style='width:" + String(pct) + "%;background:" + swr_col + ";'></div></div>";
    html += "</div>";

    // Setup info card (coil notes or VHF info)
    if (strlen(band.coil_note) > 0 || band.needs_ext_coil || band.default_steps == 0) {
        html += "<div class='card'>";
        html += "<div class='label'>SETUP INFO</div>";
        if (band.needs_ext_coil) {
            html += "<div class='warn'>&#9888; INSTALL 160m EXTENSION COIL</div>";
        }
        if (strlen(band.coil_note) > 0) {
            html += "<div>" + String(band.coil_note) + "</div>";
        }
        if (band.default_steps == 0 && !band.needs_ext_coil) {
            html += "<div class='warn'>VHF/UHF BAND — Remove loading coil for direct feed</div>";
        }
        html += "</div>";
    }

    // Control buttons
    html += "<div class='card'>";
    html += "<a href='/band_up'><button>&#9650; BAND</button></a>";
    html += "<a href='/band_dn'><button>&#9660; BAND</button></a>";
    html += "<a href='/tune'><button class='btn-tune'>&#128269; AUTO-TUNE</button></a>";
    html += "<a href='/home_motor'><button class='btn-home'>&#8962; HOME</button></a>";
    html += "<a href='/'><button>&#8635; REFRESH</button></a>";
    html += "</div>";

    html += "</body></html>";
    server.send(200, "text/html", html);
}

// -----------------------------------------------------------
// 13. handleHome — trigger homing then redirect
// -----------------------------------------------------------
void handleHome() {
    homeMotor();
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
}

// -----------------------------------------------------------
// 14. handleTune — trigger auto-tune then redirect
// -----------------------------------------------------------
void handleTune() {
    autoTune();
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
}

// -----------------------------------------------------------
// 15. handleBandUp / handleBandDn
// -----------------------------------------------------------
void handleBandUp() {
    if (g_band_idx > 0) {
        g_band_idx--;
        moveToPosition(g_band_positions[g_band_idx], FAST_SPEED_DELAY);
        autoTune();
    }
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
}

void handleBandDn() {
    if (g_band_idx < NUM_BANDS - 1) {
        g_band_idx++;
        moveToPosition(g_band_positions[g_band_idx], FAST_SPEED_DELAY);
        autoTune();
    }
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
}

// -----------------------------------------------------------
// 16. handleApiStatus — JSON status response
// -----------------------------------------------------------
void handleApiStatus() {
    float turns = (float)g_motor_pos / STEPS_PER_COIL_TURN;
    float L_uH  = computeL_uH(g_motor_pos);
    char json[512];
    snprintf(json, sizeof(json),
        "{"
        "\"band\":\"%s\","
        "\"freq_mhz\":%.3f,"
        "\"pos_steps\":%ld,"
        "\"turns\":%.3f,"
        "\"L_uH\":%.3f,"
        "\"efficiency_pct\":%.1f,"
        "\"swr\":%.2f,"
        "\"fwd_dbm\":%.1f,"
        "\"fwd_watts\":%.4f,"
        "\"refl_dbm\":%.1f,"
        "\"rho\":%.3f,"
        "\"valid\":%s,"
        "\"homed\":%s,"
        "\"tune_state\":\"%s\","
        "\"needs_ext_coil\":%s"
        "}",
        BANDS[g_band_idx].name,
        BANDS[g_band_idx].freq_mhz,
        g_motor_pos,
        turns,
        L_uH,
        BANDS[g_band_idx].efficiency_pct,
        g_last_meas.swr,
        g_last_meas.fwd_dbm,
        g_last_meas.fwd_watts,
        g_last_meas.refl_dbm,
        g_last_meas.rho,
        g_last_meas.valid ? "true" : "false",
        g_homed           ? "true" : "false",
        tuneStateStr(g_tune_state).c_str(),
        BANDS[g_band_idx].needs_ext_coil ? "true" : "false"
    );
    server.send(200, "application/json", json);
}

// -----------------------------------------------------------
// 17. handleApiMotor — GET position / POST {"steps": N}
// -----------------------------------------------------------
void handleApiMotor() {
    if (server.method() == HTTP_POST) {
        String body = server.arg("plain");
        // Simple JSON parse: find "steps": N
        int idx = body.indexOf("\"steps\"");
        if (idx < 0) idx = body.indexOf("steps");
        if (idx >= 0) {
            int colon = body.indexOf(':', idx);
            if (colon >= 0) {
                long target = body.substring(colon + 1).toInt();
                target = constrain(target, 0L, (long)MAX_STEPS);
                moveToPosition(target, FAST_SPEED_DELAY);
            }
        }
    }
    char json[80];
    snprintf(json, sizeof(json),
        "{\"pos_steps\":%ld,\"turns\":%.3f}",
        g_motor_pos,
        (float)g_motor_pos / STEPS_PER_COIL_TURN
    );
    server.send(200, "application/json", json);
}

// -----------------------------------------------------------
// 18. handleApiCal — GET/POST calibration offsets
// -----------------------------------------------------------
void handleApiCal() {
    if (server.method() == HTTP_POST) {
        if (server.hasArg("fwd")) {
            g_cal_fwd = server.arg("fwd").toFloat();
        }
        if (server.hasArg("refl")) {
            g_cal_refl = server.arg("refl").toFloat();
        }
        prefs.begin(NVS_NAMESPACE, false);
        // Store as int × 10 to avoid float NVS issues
        prefs.putInt(NVS_KEY_CAL_FWD, (int)(g_cal_fwd * 10.0f));
        prefs.putInt(NVS_KEY_CAL_REV, (int)(g_cal_refl * 10.0f));
        prefs.end();
    }
    char json[80];
    snprintf(json, sizeof(json),
        "{\"cal_fwd\":%.1f,\"cal_refl\":%.1f}",
        g_cal_fwd, g_cal_refl
    );
    server.send(200, "application/json", json);
}

// -----------------------------------------------------------
// 19. setup
// -----------------------------------------------------------
void setup() {
    Serial.begin(115200);
    Serial.println("\nScrewdriver Tuner v1.0");

    // GPIO init
    pinMode(PIN_MOTOR_IN1,    OUTPUT);
    pinMode(PIN_MOTOR_IN2,    OUTPUT);
    pinMode(PIN_MOTOR_IN3,    OUTPUT);
    pinMode(PIN_MOTOR_IN4,    OUTPUT);
    pinMode(PIN_LED_STATUS,   OUTPUT);
    pinMode(PIN_LED_SWR_OK,   OUTPUT);
    pinMode(PIN_LED_SWR_WARN, OUTPUT);
    pinMode(PIN_LED_SWR_HIGH, OUTPUT);

    pinMode(PIN_HOME_SW,   INPUT);     // External pull-up; LOW = triggered
    pinMode(PIN_TOP_SW,    INPUT);     // Input-only GPIO, no pull on 36
    pinMode(PIN_BAND_UP,   INPUT_PULLUP);
    pinMode(PIN_BAND_DN,   INPUT_PULLUP);
    pinMode(PIN_TUNE_BTN,  INPUT_PULLUP);

    // ADC attenuation: 11 dB for 0–3.3 V range
    analogSetAttenuation(ADC_11db);

    // De-energise motor
    digitalWrite(PIN_MOTOR_IN1, LOW);
    digitalWrite(PIN_MOTOR_IN2, LOW);
    digitalWrite(PIN_MOTOR_IN3, LOW);
    digitalWrite(PIN_MOTOR_IN4, LOW);
    digitalWrite(PIN_LED_STATUS,   LOW);
    digitalWrite(PIN_LED_SWR_OK,   LOW);
    digitalWrite(PIN_LED_SWR_WARN, LOW);
    digitalWrite(PIN_LED_SWR_HIGH, LOW);

    // NVS restore
    prefs.begin(NVS_NAMESPACE, true);  // read-only first pass
    g_band_idx  = prefs.getInt(NVS_KEY_BAND, 5);  // default 20m
    g_homed     = prefs.getBool(NVS_KEY_HOMED, false);
    int cfwd_i  = prefs.getInt(NVS_KEY_CAL_FWD, 0);
    int crev_i  = prefs.getInt(NVS_KEY_CAL_REV, 0);
    g_cal_fwd   = (float)cfwd_i  / 10.0f;
    g_cal_refl  = (float)crev_i  / 10.0f;

    // Load per-band learned positions; fallback to defaults
    for (int i = 0; i < NUM_BANDS; i++) {
        char key[12];
        snprintf(key, sizeof(key), "pos_%d", i);
        g_band_positions[i] = prefs.getLong(key, BANDS[i].default_steps);
    }
    prefs.end();

    Serial.printf("Restored: band=%d (%s), homed=%d, cal_fwd=%.1f, cal_refl=%.1f\n",
        g_band_idx, BANDS[g_band_idx].name, g_homed, g_cal_fwd, g_cal_refl);

    // WiFi Access Point
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_SSID, WIFI_PASS);
    Serial.printf("AP: %s  IP: %s\n", WIFI_SSID,
        WiFi.softAPIP().toString().c_str());

    // HTTP routes
    server.on("/",           HTTP_GET,  handleRoot);
    server.on("/home_motor", HTTP_GET,  handleHome);
    server.on("/tune",       HTTP_GET,  handleTune);
    server.on("/band_up",    HTTP_GET,  handleBandUp);
    server.on("/band_dn",    HTTP_GET,  handleBandDn);
    server.on("/api/status", HTTP_GET,  handleApiStatus);
    server.on("/api/motor",  handleApiMotor);
    server.on("/api/cal",    handleApiCal);
    server.begin();
    Serial.println("HTTP server started");

    // Bluetooth Serial
    SerialBT.begin(BT_NAME);
    Serial.printf("BT: %s\n", BT_NAME);

    // Home motor on first boot or if not previously homed
    if (!g_homed || digitalRead(PIN_HOME_SW) == LOW) {
        Serial.println("First boot or home switch triggered — homing now");
        homeMotor();
        // After homing, move to last known band position
        if (g_band_positions[g_band_idx] > 0) {
            moveToPosition(g_band_positions[g_band_idx], FAST_SPEED_DELAY);
        }
    } else {
        // Trust NVS position (power-cycle recovery)
        // Can't know absolute position after power loss, so force re-home
        Serial.println("Previously homed — rehoming for position certainty");
        homeMotor();
        moveToPosition(g_band_positions[g_band_idx], FAST_SPEED_DELAY);
    }

    // Save current band to NVS
    prefs.begin(NVS_NAMESPACE, false);
    prefs.putInt(NVS_KEY_BAND, g_band_idx);
    prefs.end();

    Serial.println("setup() complete");
}

// -----------------------------------------------------------
// 20. loop
// -----------------------------------------------------------
void loop() {
    unsigned long now = millis();

    // Handle HTTP clients
    server.handleClient();

    // Button polling
    checkButtons();

    // SWR measurement every 200 ms (skip if motor is moving)
    if ((now - g_swr_last_ms) >= 200 && !g_motor_moving && g_tune_state == TUNE_IDLE) {
        g_swr_last_ms = now;
        g_last_meas = measureSWR();
    }

    // BT telemetry packet
    if ((now - g_bt_last_tx) >= BT_INTERVAL_MS) {
        g_bt_last_tx = now;
        sendBTPacket();
    }

    // LED update
    updateLEDs(g_last_meas, g_tune_state);

    yield();
}
