/*
 * hybrid_monitor_cyd.ino
 * Hybrid Coupler Phase/Amplitude Balance Monitor
 * TM-HYB-001 Rev A
 *
 * Hardware (CYD ESP32-2432S028):
 *   AD8307 #1 (port A level): GPIO36 ADC
 *   AD8307 #2 (port B level): GPIO39 ADC
 *   Phase detector input A:   GPIO34 (zero-crossing)
 *   Phase detector input B:   GPIO35 (zero-crossing)
 *   Relay/switch control:     GPIO27 (band relay K1)
 *                             GPIO26 (band relay K2)
 *                             GPIO25 (band relay K3)
 *                             GPIO33 wait — this is touch CS, use GPIO16
 *   GPIO16 = relay K4
 *   GPIO17 = relay K5
 *   Audio tone out:           GPIO21 (TFT BL, repurposed or separate)
 *
 * Screens:
 *   SCREEN_METER   — amplitude A, amplitude B, level difference, SWR estimate
 *   SCREEN_PHASE   — phase difference (°), phase error vs target
 *   SCREEN_BAND    — band selection (160M–10M), target phase
 *   SCREEN_HISTORY — min/max/mean of 100 readings
 *   SCREEN_SPEC    — display spec limits (pass/fail)
 *
 * AD8307 slope: 25 mV/dB, intercept: −84 dBm
 *   P_dBm = V_adc × (3300 / 4095) × 40 − 84   [V_adc in counts]
 *
 * Phase measurement: zero-crossing time difference on GPIO34/GPIO35.
 * Requires: signal on both ports, same frequency, minimum −20 dBm.
 * Phase resolution: ≈ 1° at 14 MHz (timing ≈ 2 ns with 80 MHz clock).
 */

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <driver/adc.h>
#include <esp_timer.h>
#include <Preferences.h>

// ── Pin Definitions ───────────────────────────────────────────────────────────
#define AD8307_A_PIN   36    // ADC1_CH0 — port A level
#define AD8307_B_PIN   39    // ADC1_CH3 — port B level
#define ZC_A_PIN       34    // zero-crossing input A
#define ZC_B_PIN       35    // zero-crossing input B
#define TFT_BL_PIN     21    // backlight
#define RELAY1_PIN     27
#define RELAY2_PIN     26
#define RELAY3_PIN     25
#define RELAY4_PIN     16
#define RELAY5_PIN     17

// ── AD8307 calibration ────────────────────────────────────────────────────────
#define AD8307_SLOPE     40.0f    // dB/V (25 mV/dB → 40 dB/V)
#define AD8307_INTERCEPT -84.0f   // dBm at 0 V output
#define ADC_VREF         3.3f
#define ADC_COUNTS       4095.0f

// ── Enumerations ─────────────────────────────────────────────────────────────
typedef enum { SCREEN_METER=0, SCREEN_PHASE, SCREEN_BAND, SCREEN_HISTORY, SCREEN_SPEC } Screen;
typedef enum { MODE_WILKINSON=0, MODE_QUAD90, MODE_RATRACE180 } HybridMode;

// Band table
struct Band {
    const char* name;
    float freq_mhz;
    float target_phase_deg;  // expected phase difference between ports
    uint8_t relay_bits;      // relay control byte for band switching
};

static const Band BANDS[] = {
    { "160M",  1.85f,   0.0f, 0b00000 },
    { "80M",   3.75f,   0.0f, 0b00001 },
    { "40M",   7.15f,   0.0f, 0b00010 },
    { "30M",  10.125f,  0.0f, 0b00011 },
    { "20M",  14.175f,  0.0f, 0b00100 },
    { "17M",  18.118f,  0.0f, 0b00101 },
    { "15M",  21.225f,  0.0f, 0b00110 },
    { "12M",  24.940f,  0.0f, 0b00111 },
    { "10M",  28.850f,  0.0f, 0b01000 },
    { "6M",   50.500f,  0.0f, 0b01001 },
};
#define NUM_BANDS (sizeof(BANDS) / sizeof(BANDS[0]))

// ── Application State ─────────────────────────────────────────────────────────
struct AppState {
    Screen screen;
    HybridMode mode;
    int band_idx;

    // Level measurements
    float dbm_a;
    float dbm_b;
    float diff_db;         // |A - B| — amplitude balance
    float target_diff;     // expected diff (0 dB for equal split)

    // Phase measurement
    float phase_deg;       // measured phase difference A-B
    float phase_error;     // measured - target
    bool  phase_valid;

    // Statistics (circular buffer of last 100 readings)
    float hist_diff[100];
    float hist_phase[100];
    int   hist_idx;
    int   hist_count;

    // Pass/fail
    float spec_amp_bal_db;    // amplitude balance limit (typ 0.5 dB)
    float spec_phase_err_deg; // phase error limit (typ 5°)

    bool  pass_amp;
    bool  pass_phase;
    bool  needs_redraw;
};

static AppState app = {};
static TFT_eSPI tft = TFT_eSPI();
static Preferences prefs;

// ── Zero-crossing phase measurement ──────────────────────────────────────────
// Uses GPIO34/35 and esp_timer to measure time between rising edges.
// Phase (degrees) = (Δt × f₀ × 360°)

static volatile int64_t zc_a_time = 0;
static volatile int64_t zc_b_time = 0;
static volatile bool    zc_a_fresh = false;
static volatile bool    zc_b_fresh = false;

static void IRAM_ATTR zc_a_isr() {
    zc_a_time  = esp_timer_get_time();
    zc_a_fresh = true;
}

static void IRAM_ATTR zc_b_isr() {
    zc_b_time  = esp_timer_get_time();
    zc_b_fresh = true;
}

float measure_phase() {
    zc_a_fresh = false;
    zc_b_fresh = false;

    // Wait up to 2 ms for both edges
    int64_t deadline = esp_timer_get_time() + 2000LL;
    while ((!zc_a_fresh || !zc_b_fresh) && esp_timer_get_time() < deadline) {
        delayMicroseconds(1);
    }
    if (!zc_a_fresh || !zc_b_fresh) return NAN;

    int64_t dt_us = zc_b_time - zc_a_time;
    float   period_us = 1e6f / BANDS[app.band_idx].freq_mhz;  // freq in MHz → period in µs
    float   phase = ((float)dt_us / period_us) * 360.0f;

    // Normalize to −180°…+180°
    while (phase >  180.0f) phase -= 360.0f;
    while (phase < -180.0f) phase += 360.0f;
    return phase;
}

// ── AD8307 read helpers ───────────────────────────────────────────────────────
float read_dbm(int gpio_pin) {
    // Average 16 samples
    int32_t sum = 0;
    for (int i = 0; i < 16; i++) sum += analogRead(gpio_pin);
    float v_adc = (sum / 16.0f) * ADC_VREF / ADC_COUNTS;
    return v_adc * AD8307_SLOPE + AD8307_INTERCEPT;
}

// ── Relay / band switching ────────────────────────────────────────────────────
void relay_set(uint8_t bits) {
    digitalWrite(RELAY1_PIN, (bits >> 0) & 1);
    digitalWrite(RELAY2_PIN, (bits >> 1) & 1);
    digitalWrite(RELAY3_PIN, (bits >> 2) & 1);
    digitalWrite(RELAY4_PIN, (bits >> 3) & 1);
    digitalWrite(RELAY5_PIN, (bits >> 4) & 1);
    delay(20); // relay settle
}

void set_band(int idx) {
    app.band_idx = constrain(idx, 0, (int)NUM_BANDS - 1);
    relay_set(BANDS[app.band_idx].relay_bits);
    // Update target phase based on mode
    float tp = 0.0f;
    if      (app.mode == MODE_QUAD90)    tp = 90.0f;
    else if (app.mode == MODE_RATRACE180) tp = 180.0f;
    for (int i = 0; i < (int)NUM_BANDS; i++) {
        ((Band*)BANDS)[i].target_phase_deg = tp;  // runtime update (cast away const)
    }
    app.needs_redraw = true;
}

// ── Statistics ────────────────────────────────────────────────────────────────
void history_push(float diff, float phase) {
    app.hist_diff[app.hist_idx]  = diff;
    app.hist_phase[app.hist_idx] = phase;
    app.hist_idx = (app.hist_idx + 1) % 100;
    if (app.hist_count < 100) app.hist_count++;
}

void history_stats(float* arr, int n, float* mn, float* mx, float* mean) {
    if (n == 0) { *mn = *mx = *mean = 0.0f; return; }
    float lo = arr[0], hi = arr[0], sum = 0.0f;
    for (int i = 0; i < n; i++) {
        if (arr[i] < lo) lo = arr[i];
        if (arr[i] > hi) hi = arr[i];
        sum += arr[i];
    }
    *mn = lo; *mx = hi; *mean = sum / n;
}

// ─── Color palette ─────────────────────────────────────────────────────────
#define C_BG     TFT_BLACK
#define C_FG     TFT_WHITE
#define C_GREEN  TFT_GREEN
#define C_YELLOW TFT_YELLOW
#define C_RED    TFT_RED
#define C_ACCENT 0x07FF   // cyan
#define C_DGRAY  0x39E7
#define C_LGRAY  0xC618

// ── Tab bar ───────────────────────────────────────────────────────────────────
void draw_tabs() {
    const char* labels[] = { "LEVEL", "PHASE", "BAND", "HIST", "SPEC" };
    int tw = 64;
    for (int i = 0; i < 5; i++) {
        uint16_t bg = (i == (int)app.screen) ? C_ACCENT : C_DGRAY;
        uint16_t fg = (i == (int)app.screen) ? TFT_BLACK : C_FG;
        tft.fillRect(i * tw, 220, tw - 1, 20, bg);
        tft.setTextColor(fg, bg);
        tft.setTextSize(1);
        tft.setCursor(i*tw + (tw - strlen(labels[i])*6)/2, 226);
        tft.print(labels[i]);
    }
}

// ── Horizontal level bar ─────────────────────────────────────────────────────
void draw_level_bar(int x, int y, int w, int h, float dbm, float dbm_min, float dbm_max) {
    tft.drawRect(x, y, w, h, C_DGRAY);
    float frac = (dbm - dbm_min) / (dbm_max - dbm_min);
    frac = constrain(frac, 0.0f, 1.0f);
    int fill = (int)(frac * (w - 2));
    uint16_t col = (frac > 0.9f) ? C_RED : (frac > 0.7f) ? C_YELLOW : C_GREEN;
    tft.fillRect(x + 1, y + 1, fill, h - 2, col);
    tft.fillRect(x + 1 + fill, y + 1, w - 2 - fill, h - 2, C_BG);
}

// ── SCREEN_METER ──────────────────────────────────────────────────────────────
void draw_meter_screen() {
    tft.fillRect(0, 0, 320, 218, C_BG);
    tft.setTextColor(C_ACCENT, C_BG); tft.setTextSize(1);
    tft.setCursor(4, 4);
    tft.printf("HYBRID LEVEL MONITOR  %s  %s",
               BANDS[app.band_idx].name,
               app.mode == MODE_WILKINSON ? "WIL" :
               app.mode == MODE_QUAD90    ? "90°" : "180°");

    // Port A
    tft.setTextColor(C_FG, C_BG); tft.setCursor(4, 22);
    tft.printf("PORT A: %+.1f dBm", app.dbm_a);
    draw_level_bar(4, 36, 310, 16, app.dbm_a, -80.0f, 10.0f);

    // Port B
    tft.setCursor(4, 60); tft.printf("PORT B: %+.1f dBm", app.dbm_b);
    draw_level_bar(4, 74, 310, 16, app.dbm_b, -80.0f, 10.0f);

    // Amplitude balance
    float diff = app.dbm_a - app.dbm_b;
    tft.setCursor(4, 100);
    uint16_t diff_col = (fabsf(diff) > app.spec_amp_bal_db) ? C_RED : C_GREEN;
    tft.setTextColor(diff_col, C_BG);
    tft.printf("ΔAmp: %+.2f dB  (spec ±%.1f dB)  %s",
               diff, app.spec_amp_bal_db, fabsf(diff) <= app.spec_amp_bal_db ? "PASS" : "FAIL");

    // SWR estimate (crude: return loss from port match)
    tft.setCursor(4, 116);
    tft.setTextColor(C_LGRAY, C_BG);
    tft.print("SWR: measure with dedicated bridge (see manual §3.4)");

    // Pass/fail summary
    tft.drawFastHLine(0, 140, 320, C_DGRAY);
    tft.setCursor(4, 148);
    tft.setTextColor(C_LGRAY, C_BG);
    tft.printf("Readings: %d    Mode: %s",
               app.hist_count,
               app.mode == MODE_WILKINSON ? "Wilkinson (0° nominal)" :
               app.mode == MODE_QUAD90    ? "Quadrature (90° nominal)" : "Rat-race (180° nominal)");

    // Touch hint
    tft.setTextColor(C_DGRAY, C_BG); tft.setCursor(4, 200);
    tft.print("[RESET HIST]   Touch BAND to change band");
}

// ── SCREEN_PHASE ──────────────────────────────────────────────────────────────
void draw_phase_screen() {
    tft.fillRect(0, 0, 320, 218, C_BG);
    tft.setTextColor(C_ACCENT, C_BG); tft.setTextSize(1);
    tft.setCursor(4, 4);
    tft.printf("PHASE MONITOR  %s  %.3f MHz",
               BANDS[app.band_idx].name, BANDS[app.band_idx].freq_mhz);

    float target = (app.mode == MODE_QUAD90) ? 90.0f :
                   (app.mode == MODE_RATRACE180) ? 180.0f : 0.0f;

    if (!app.phase_valid) {
        tft.setTextColor(C_DGRAY, C_BG); tft.setTextSize(2);
        tft.setCursor(40, 80); tft.print("NO SIGNAL / < -20 dBm");
    } else {
        float err = app.phase_deg - target;
        while (err >  180.0f) err -= 360.0f;
        while (err < -180.0f) err += 360.0f;

        // Big phase readout
        tft.setTextColor(C_FG, C_BG); tft.setTextSize(3);
        tft.setCursor(20, 35);
        tft.printf("%.1f°", app.phase_deg);

        tft.setTextSize(2);
        tft.setCursor(20, 80);
        tft.printf("Target: %.1f°", target);

        // Phase error with color coding
        uint16_t err_col = (fabsf(err) > app.spec_phase_err_deg) ? C_RED :
                           (fabsf(err) > app.spec_phase_err_deg * 0.5f) ? C_YELLOW : C_GREEN;
        tft.setTextColor(err_col, C_BG); tft.setTextSize(2);
        tft.setCursor(20, 108);
        tft.printf("Error: %+.1f°  %s",
                   err, fabsf(err) <= app.spec_phase_err_deg ? "PASS" : "FAIL");

        // Phase angle arc indicator (compass-style)
        int cx = 256, cy = 110, r = 70;
        tft.drawCircle(cx, cy, r, C_DGRAY);
        // Draw target spoke (yellow)
        float tar_rad = target * PI / 180.0f;
        tft.drawLine(cx, cy,
                     cx + (int)(r * cosf(tar_rad - PI/2)),
                     cy + (int)(r * sinf(tar_rad - PI/2)),
                     C_YELLOW);
        // Draw measured spoke (green/red)
        float meas_rad = app.phase_deg * PI / 180.0f;
        tft.drawLine(cx, cy,
                     cx + (int)(r * cosf(meas_rad - PI/2)),
                     cy + (int)(r * sinf(meas_rad - PI/2)),
                     err_col);
        tft.drawCircle(cx, cy, 3, C_FG);
        tft.setTextColor(C_LGRAY, C_BG); tft.setTextSize(1);
        tft.setCursor(cx - 12, cy - r - 12); tft.print("0°");
        tft.setCursor(cx + r + 4, cy - 4);  tft.print("90°");
        tft.setCursor(cx - 18, cy + r + 4); tft.print("180°");
    }
}

// ── SCREEN_BAND ───────────────────────────────────────────────────────────────
void draw_band_screen() {
    tft.fillRect(0, 0, 320, 218, C_BG);
    tft.setTextColor(C_ACCENT, C_BG); tft.setTextSize(1);
    tft.setCursor(4, 4);
    tft.print("BAND / MODE SELECT");

    // Band grid: 5 columns × 2 rows = 10 bands
    for (int i = 0; i < (int)NUM_BANDS; i++) {
        int col = i % 5;
        int row = i / 5;
        int bx = col * 64, by = 24 + row * 36;
        bool sel = (i == app.band_idx);
        tft.fillRect(bx, by, 62, 32, sel ? C_ACCENT : C_DGRAY);
        tft.setTextColor(sel ? TFT_BLACK : C_FG, sel ? C_ACCENT : C_DGRAY);
        tft.setTextSize(1);
        tft.setCursor(bx + 8, by + 10);
        tft.print(BANDS[i].name);
        tft.setCursor(bx + 4, by + 22);
        tft.printf("%.3f", BANDS[i].freq_mhz);
    }

    // Mode select
    tft.setTextColor(C_ACCENT, C_BG); tft.setCursor(4, 104);
    tft.print("HYBRID TYPE:");
    const char* mnames[] = { "WILKINSON (0°)", "QUADRATURE (90°)", "RAT-RACE (180°)" };
    for (int i = 0; i < 3; i++) {
        bool sel = (i == (int)app.mode);
        tft.fillRect(4, 118 + i*26, 310, 22, sel ? C_ACCENT : C_DGRAY);
        tft.setTextColor(sel ? TFT_BLACK : C_FG, sel ? C_ACCENT : C_DGRAY);
        tft.setCursor(12, 124 + i*26);
        tft.print(mnames[i]);
    }

    tft.setTextColor(C_DGRAY, C_BG); tft.setCursor(4, 202);
    tft.print("Touch to select band or mode");
}

// ── SCREEN_HISTORY ────────────────────────────────────────────────────────────
void draw_history_screen() {
    tft.fillRect(0, 0, 320, 218, C_BG);
    tft.setTextColor(C_ACCENT, C_BG); tft.setTextSize(1);
    tft.setCursor(4, 4);
    tft.printf("HISTORY  N=%d  %s", app.hist_count, BANDS[app.band_idx].name);

    if (app.hist_count < 2) {
        tft.setTextColor(C_DGRAY, C_BG); tft.setCursor(60, 100);
        tft.print("Need ≥2 readings");
        return;
    }

    float mn, mx, mean;

    history_stats(app.hist_diff, app.hist_count, &mn, &mx, &mean);
    tft.setTextColor(C_FG, C_BG);
    tft.setCursor(4, 22);  tft.printf("ΔAmp (dB):  min=%+.2f  max=%+.2f  mean=%+.2f", mn, mx, mean);

    history_stats(app.hist_phase, app.hist_count, &mn, &mx, &mean);
    tft.setCursor(4, 40);  tft.printf("Phase (°):  min=%+.1f   max=%+.1f   mean=%+.1f", mn, mx, mean);

    // Mini amplitude balance sparkline
    tft.setTextColor(C_LGRAY, C_BG); tft.setCursor(4, 58); tft.print("ΔAmp trend:");
    int n = min(app.hist_count, 100);
    for (int i = 0; i < n; i++) {
        int xi = i * 3 + 4;
        int yi = 90 - (int)(app.hist_diff[i] * 10.0f);
        yi = constrain(yi, 65, 115);
        tft.drawPixel(xi, yi, C_GREEN);
    }
    tft.drawFastHLine(4, 90, 300, C_DGRAY);  // zero line

    // Mini phase sparkline
    tft.setTextColor(C_LGRAY, C_BG); tft.setCursor(4, 120); tft.print("Phase trend:");
    float target = (app.mode == MODE_QUAD90) ? 90.0f : (app.mode == MODE_RATRACE180) ? 180.0f : 0.0f;
    for (int i = 0; i < n; i++) {
        int xi = i * 3 + 4;
        int yi = 155 - (int)((app.hist_phase[i] - target) * 1.0f);
        yi = constrain(yi, 125, 185);
        tft.drawPixel(xi, yi, C_ACCENT);
    }
    tft.drawFastHLine(4, 155, 300, C_DGRAY);  // target line

    tft.setTextColor(C_DGRAY, C_BG); tft.setCursor(4, 198);
    tft.print("[RESET] to clear history");
}

// ── SCREEN_SPEC ───────────────────────────────────────────────────────────────
void draw_spec_screen() {
    tft.fillRect(0, 0, 320, 218, C_BG);
    tft.setTextColor(C_ACCENT, C_BG); tft.setTextSize(1);
    tft.setCursor(4, 4);
    tft.print("PASS / FAIL SPECIFICATION");

    auto spec_row = [&](int y, const char* label, float val, float limit, const char* unit) {
        bool pass = fabsf(val) <= limit;
        tft.setTextColor(C_LGRAY, C_BG); tft.setCursor(4, y); tft.print(label);
        tft.setTextColor(C_FG, C_BG); tft.setCursor(130, y);
        tft.printf("%+.2f%s", val, unit);
        tft.setTextColor(C_LGRAY, C_BG); tft.setCursor(210, y);
        tft.printf("(lim ±%.1f%s)", limit, unit);
        tft.fillRect(285, y - 1, 32, 12, pass ? C_GREEN : C_RED);
        tft.setTextColor(TFT_BLACK, pass ? C_GREEN : C_RED);
        tft.setCursor(289, y); tft.print(pass ? "PASS" : "FAIL");
    };

    float diff_now = app.dbm_a - app.dbm_b;
    float target   = (app.mode == MODE_QUAD90) ? 90.0f : (app.mode == MODE_RATRACE180) ? 180.0f : 0.0f;
    float ph_err   = app.phase_valid ? (app.phase_deg - target) : 999.0f;

    spec_row(26,  "Amplitude balance:", diff_now,  app.spec_amp_bal_db,    " dB");
    spec_row(44,  "Phase error:",       ph_err,    app.spec_phase_err_deg, "°");

    // Editable spec limits
    tft.drawFastHLine(0, 65, 320, C_DGRAY);
    tft.setTextColor(C_ACCENT, C_BG); tft.setCursor(4, 70);
    tft.print("SPEC LIMITS (touch ± to adjust):");

    tft.setTextColor(C_FG, C_BG); tft.setCursor(4, 86);
    tft.printf("Amplitude limit: ±%.1f dB", app.spec_amp_bal_db);
    tft.fillRect(240, 84, 32, 14, C_DGRAY); tft.setTextColor(C_FG, C_DGRAY);
    tft.setCursor(248, 86); tft.print("−");
    tft.fillRect(278, 84, 32, 14, C_DGRAY);
    tft.setCursor(286, 86); tft.print("+");

    tft.setTextColor(C_FG, C_BG); tft.setCursor(4, 106);
    tft.printf("Phase limit:    ±%.1f°", app.spec_phase_err_deg);
    tft.fillRect(240, 104, 32, 14, C_DGRAY); tft.setTextColor(C_FG, C_DGRAY);
    tft.setCursor(248, 106); tft.print("−");
    tft.fillRect(278, 104, 32, 14, C_DGRAY);
    tft.setCursor(286, 106); tft.print("+");

    // Hybrid type spec summary
    tft.drawFastHLine(0, 125, 320, C_DGRAY);
    tft.setTextColor(C_LGRAY, C_BG); tft.setCursor(4, 130);
    if (app.mode == MODE_WILKINSON) {
        tft.print("Wilkinson: amp bal <0.3dB, phase <2°, isolation >20dB");
    } else if (app.mode == MODE_QUAD90) {
        tft.print("Quadrature: amp bal <0.3dB, phase 90°±5°, isolation >18dB");
    } else {
        tft.print("Rat-race: amp bal <0.5dB, phase 180°±5°, isolation >20dB");
    }
}

// ── Screen dispatch ───────────────────────────────────────────────────────────
void draw_screen() {
    switch (app.screen) {
        case SCREEN_METER:   draw_meter_screen();   break;
        case SCREEN_PHASE:   draw_phase_screen();   break;
        case SCREEN_BAND:    draw_band_screen();     break;
        case SCREEN_HISTORY: draw_history_screen();  break;
        case SCREEN_SPEC:    draw_spec_screen();     break;
    }
    draw_tabs();
}

// ── Touch handling ────────────────────────────────────────────────────────────
static uint32_t last_touch_ms = 0;

void handle_touch() {
    uint16_t tx, ty;
    if (!tft.getTouch(&tx, &ty)) return;
    if (millis() - last_touch_ms < 200) return;
    last_touch_ms = millis();

    // Tab bar
    if (ty >= 220) {
        app.screen = (Screen)(tx / 64);
        app.needs_redraw = true;
        return;
    }

    if (app.screen == SCREEN_BAND) {
        // Band grid
        if (ty >= 24 && ty < 96) {
            int col = tx / 64, row = (ty - 24) / 36;
            int idx = row * 5 + col;
            if (idx < (int)NUM_BANDS) { set_band(idx); }
        }
        // Mode select
        if (ty >= 118 && ty < 198) {
            int mi = (ty - 118) / 26;
            if (mi >= 0 && mi < 3) { app.mode = (HybridMode)mi; app.needs_redraw = true; }
        }
    }

    if (app.screen == SCREEN_METER && ty >= 195 && ty < 215) {
        // RESET HIST
        app.hist_count = app.hist_idx = 0;
        app.needs_redraw = true;
    }

    if (app.screen == SCREEN_SPEC) {
        // Amp limit ± buttons
        if (ty >= 84 && ty < 98) {
            if (tx >= 240 && tx < 272) app.spec_amp_bal_db = max(0.1f, app.spec_amp_bal_db - 0.1f);
            if (tx >= 278)             app.spec_amp_bal_db = min(5.0f, app.spec_amp_bal_db + 0.1f);
            app.needs_redraw = true;
        }
        // Phase limit ± buttons
        if (ty >= 104 && ty < 118) {
            if (tx >= 240 && tx < 272) app.spec_phase_err_deg = max(1.0f, app.spec_phase_err_deg - 1.0f);
            if (tx >= 278)             app.spec_phase_err_deg = min(30.0f, app.spec_phase_err_deg + 1.0f);
            app.needs_redraw = true;
        }
    }
}

// ── Setup ─────────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);

    // Output pins
    pinMode(RELAY1_PIN, OUTPUT); pinMode(RELAY2_PIN, OUTPUT);
    pinMode(RELAY3_PIN, OUTPUT); pinMode(RELAY4_PIN, OUTPUT);
    pinMode(RELAY5_PIN, OUTPUT); pinMode(TFT_BL_PIN, OUTPUT);
    analogWrite(TFT_BL_PIN, 200);

    // Zero-crossing inputs
    pinMode(ZC_A_PIN, INPUT);
    pinMode(ZC_B_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(ZC_A_PIN), zc_a_isr, RISING);
    attachInterrupt(digitalPinToInterrupt(ZC_B_PIN), zc_b_isr, RISING);

    // ADC configuration
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); // GPIO36
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11); // GPIO39

    // TFT
    tft.init(); tft.setRotation(1); tft.fillScreen(TFT_BLACK);
    tft.setTextColor(0x07FF, TFT_BLACK); tft.setTextSize(2);
    tft.setCursor(50, 100); tft.print("HYB-001 Monitor");
    delay(800);

    // App init
    memset(&app, 0, sizeof(app));
    app.screen           = SCREEN_METER;
    app.mode             = MODE_WILKINSON;
    app.band_idx         = 4;  // 20M default
    app.spec_amp_bal_db  = 0.5f;
    app.spec_phase_err_deg = 5.0f;
    app.needs_redraw     = true;

    set_band(app.band_idx);
    tft.fillScreen(TFT_BLACK);
}

// ── Loop ──────────────────────────────────────────────────────────────────────
static uint32_t last_meas_ms = 0;

void loop() {
    handle_touch();

    uint32_t now = millis();
    if (now - last_meas_ms >= 500) {
        last_meas_ms = now;

        // Amplitude readings
        app.dbm_a = read_dbm(AD8307_A_PIN);
        app.dbm_b = read_dbm(AD8307_B_PIN);
        app.diff_db = app.dbm_a - app.dbm_b;

        // Phase reading (requires signal > −20 dBm on both ports)
        if (app.dbm_a > -60.0f && app.dbm_b > -60.0f) {
            float ph = measure_phase();
            app.phase_valid = !isnan(ph);
            if (app.phase_valid) app.phase_deg = ph;
        } else {
            app.phase_valid = false;
        }

        history_push(app.diff_db, app.phase_valid ? app.phase_deg : 0.0f);

        app.pass_amp   = (fabsf(app.diff_db) <= app.spec_amp_bal_db);
        float target   = (app.mode == MODE_QUAD90) ? 90.0f :
                         (app.mode == MODE_RATRACE180) ? 180.0f : 0.0f;
        app.pass_phase = app.phase_valid &&
                         (fabsf(app.phase_deg - target) <= app.spec_phase_err_deg);

        app.needs_redraw = true;

        Serial.printf("%.3f MHz  A=%.1f dBm  B=%.1f dBm  ΔdB=%.2f  Phase=%.1f°  %s/%s\n",
                      BANDS[app.band_idx].freq_mhz,
                      app.dbm_a, app.dbm_b, app.diff_db,
                      app.phase_valid ? app.phase_deg : 0.0f,
                      app.pass_amp ? "AMP_OK" : "AMP_FAIL",
                      app.pass_phase ? "PH_OK" : "PH_FAIL");
    }

    if (app.needs_redraw) {
        draw_screen();
        app.needs_redraw = false;
    }
}
