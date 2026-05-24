/*
 * NOISE BRIDGE FIRMWARE — CYD (ESP32-2432S028)
 * RF Impedance Measurement, 1-60 MHz
 *
 * DISPLAYS:
 *   - R (0-200 ohm) from calibration pot VR1
 *   - X (-200 to +200 ohm) from calibration pot VR3 + polarity switch
 *   - |Z|, phase angle theta
 *   - SWR (calculated from R and X)
 *   - Return loss (dB)
 *   - Equivalent L or C at selected frequency
 *   - Bridge null depth (AD8307 log detector)
 *   - Smith chart with plotted point and trail
 *
 * SCREENS:
 *   0 = Main readout (R, X, Z, SWR, null meter)
 *   1 = Smith chart (200x200 pixel chart with plotted point)
 *   2 = Frequency / band select (touch)
 *   3 = Calibration / zero procedure
 *   4 = History plot (last 32 R+X readings vs time)
 *
 * HARDWARE:
 *   GPIO39: R_cal pot (0-3.3V = 0-200 ohm)
 *   GPIO36: X_cal pot (0-3.3V = 0-200 ohm magnitude)
 *   GPIO34: AD8307 VOUT (null depth, log detector)
 *   GPIO35: Battery voltage (via 1/3 voltage divider)
 *   GPIO25: X polarity (HIGH = inductive = negative X)
 *   GPIO26: Noise source enable (DAC2, HIGH = enabled)
 *   GPIO4:  LED red (active LOW)
 *   CYD built-in: TFT SPI, touch
 *
 * LIBRARIES:
 *   TFT_eSPI (configure for CYD/ILI9341)
 *   EEPROM
 *
 * LICENSE: MIT   AUTHOR: Generated for Merv Martin, 2026
 */

#include <TFT_eSPI.h>
#include <EEPROM.h>
#include <math.h>
#include "smith_chart.h"

// ============================================================
// Pin assignments
// ============================================================
#define PIN_R_ADC      39    // R calibration pot
#define PIN_X_ADC      36    // X calibration pot (magnitude)
#define PIN_NULL_ADC   34    // AD8307 null depth
#define PIN_BATT_ADC   35    // Battery monitor
#define PIN_X_POL      25    // X polarity (HIGH = inductive)
#define PIN_NOISE_EN   26    // Noise source enable (DAC)
#define PIN_LED_R       4    // Red LED (active LOW)
#define PIN_BACKLIGHT  21    // TFT backlight

// ============================================================
// Calibration constants
// ============================================================
#define R_MAX_OHM      200.0f
#define X_MAX_OHM      200.0f
#define Z0             50.0f

// AD8307: slope 25 mV/dB, intercept ~-84 dBm (5V supply)
#define AD8307_SLOPE   0.025f   // V/dB
#define AD8307_INTERCEPT -84.0f // dBm at 0V output

// EEPROM layout
#define EE_CAL_FLAG    0    // 0xBE if calibrated
#define EE_R_OFFSET    1    // float: R pot zero offset (4 bytes)
#define EE_X_OFFSET    5    // float: X pot zero offset
#define EE_R_SCALE     9    // float: R full-scale (default 200.0)
#define EE_X_SCALE    13    // float: X full-scale

// ============================================================
// Band table
// ============================================================
struct Band {
    const char* name;
    float f_mhz;
};

const Band BANDS[] = {
    {"160M",  1.850f},
    {"80M",   3.750f},
    {"40M",   7.150f},
    {"30M",  10.125f},
    {"20M",  14.175f},
    {"17M",  18.118f},
    {"15M",  21.225f},
    {"12M",  24.940f},
    {"10M",  28.500f},
    {"6M",   50.000f},
};
const int N_BANDS = sizeof(BANDS) / sizeof(BANDS[0]);

// ============================================================
// Colors
// ============================================================
#define C_BG        TFT_BLACK
#define C_HEADER    0x0350
#define C_ACCENT    0xFD20   // orange
#define C_POS       TFT_GREEN
#define C_NEG       0x001F   // blue (inductive)
#define C_WARN      TFT_YELLOW
#define C_FAIL      TFT_RED
#define C_TEXT      TFT_WHITE
#define C_GRID      0x4208
#define C_DIM       0x2104
#define C_SMITH_BG  0x0820

TFT_eSPI tft = TFT_eSPI();

// Smith chart (centered, radius 90px, to leave room for readout)
SmithChart sc(tft, 160, 155, 90);

// ============================================================
// State
// ============================================================
float  R_ohm    = 50.0f;
float  X_ohm    = 0.0f;
float  Z_mag    = 50.0f;
float  theta_deg = 0.0f;
float  swr      = 1.0f;
float  rl_db    = 99.0f;
float  null_dbm = -60.0f;
float  batt_v   = 18.0f;
bool   x_inductive = false;
int    band_idx = 4;   // default 20M

uint8_t screen_mode = 0;
uint32_t last_update = 0;
uint32_t last_display = 0;

// Calibration offsets (loaded from EEPROM)
float r_offset = 0.0f;
float x_offset = 0.0f;
float r_scale  = R_MAX_OHM;
float x_scale  = X_MAX_OHM;

// ADC oversample count
#define ADC_OS 32

// History buffer for trend display
#define HIST_LEN 64
float hist_R[HIST_LEN] = {0};
float hist_X[HIST_LEN] = {0};
int   hist_idx = 0;

// ============================================================
// ADC oversampled read
// ============================================================
float readADCv(int pin) {
    long s = 0;
    for (int i = 0; i < ADC_OS; i++) {
        s += analogRead(pin);
        delayMicroseconds(40);
    }
    return (float)s / ADC_OS * 3.3f / 4095.0f;
}

// ============================================================
// Measure and update all quantities
// ============================================================
void doMeasure() {
    // Read pots
    float v_r = readADCv(PIN_R_ADC);
    float v_x = readADCv(PIN_X_ADC);
    float v_n = readADCv(PIN_NULL_ADC);
    float v_b = readADCv(PIN_BATT_ADC);

    // Apply calibration
    R_ohm = max(0.0f, (v_r / 3.3f) * r_scale - r_offset);
    float x_mag = max(0.0f, (v_x / 3.3f) * x_scale - x_offset);

    x_inductive = (digitalRead(PIN_X_POL) == HIGH);
    X_ohm = x_inductive ? -x_mag : x_mag;

    // Derived impedance values
    Z_mag = sqrtf(R_ohm * R_ohm + X_ohm * X_ohm);
    theta_deg = atan2f(X_ohm, R_ohm) * 180.0f / M_PI;

    // SWR and return loss
    swr   = sc.computeSWR(R_ohm, X_ohm);
    rl_db = sc.computeReturnLoss(R_ohm, X_ohm);

    // Null depth from AD8307
    null_dbm = (v_n / AD8307_SLOPE) + AD8307_INTERCEPT;

    // Battery (3:1 divider: 3.3V ADC full scale = 9.9V at pin, x2 for 18V bus)
    batt_v = v_b * 3.0f * 2.0f;  // adjust multiplier to match actual divider

    // History
    hist_R[hist_idx] = R_ohm;
    hist_X[hist_idx] = X_ohm;
    hist_idx = (hist_idx + 1) % HIST_LEN;
}

// ============================================================
// Compute L or C from X and frequency
// ============================================================
void computeLC(float X, float f_mhz, float* L_nH, float* C_pF) {
    float f = f_mhz * 1e6f;
    *L_nH = 0.0f;
    *C_pF = 0.0f;
    if (X < -0.1f) {
        // Inductive
        *L_nH = (-X) / (2.0f * M_PI * f) * 1e9f;
    } else if (X > 0.1f) {
        // Capacitive
        *C_pF = 1.0f / (2.0f * M_PI * f * X) * 1e12f;
    }
}

// ============================================================
// Draw the null depth bargraph
// ============================================================
void drawNullBar(int x, int y, int w, int h, float dbm) {
    // dbm range: -80 (deep null) to -20 (off null)
    float pct = (dbm - (-80.0f)) / 60.0f;
    pct = constrain(pct, 0.0f, 1.0f);
    int bw = (int)(pct * (w - 2));

    tft.fillRect(x, y, w, h, C_DIM);
    tft.drawRect(x, y, w, h, C_GRID);

    // Color: green when near null (< -55 dBm), yellow, red
    uint16_t bc = dbm < -55.0f ? C_POS : dbm < -40.0f ? C_WARN : C_FAIL;
    if (bw > 0) tft.fillRect(x+1, y+1, bw, h-2, bc);

    tft.setTextSize(1);
    tft.setTextColor(C_TEXT, C_DIM);
    tft.setCursor(x+3, y+2);
    tft.print("NULL");
    char buf[12];
    snprintf(buf, sizeof(buf), "%.0fdBm", dbm);
    tft.setCursor(x + w - strlen(buf)*6 - 3, y+2);
    tft.print(buf);
}

// ============================================================
// SCREEN 0: Main readout
// ============================================================
void drawMainScreen() {
    // Header
    tft.fillRect(0, 0, 320, 22, C_HEADER);
    tft.setTextColor(C_TEXT, C_HEADER);
    tft.setTextSize(2);
    tft.setCursor(6, 4);
    tft.printf("NOISE BRIDGE  %s %.3fMHz",
               BANDS[band_idx].name, BANDS[band_idx].f_mhz);

    // Battery indicator
    tft.setTextSize(1);
    tft.setTextColor(batt_v > 14.0f ? C_POS : batt_v > 10.0f ? C_WARN : C_FAIL,
                     C_HEADER);
    tft.setCursor(270, 7);
    char bbuf[8];
    snprintf(bbuf, sizeof(bbuf), "%.1fV", batt_v);
    tft.print(bbuf);

    // --- R value ---
    tft.setTextSize(1);
    tft.setTextColor(C_GRID, C_BG);
    tft.setCursor(6, 28);
    tft.print("R (ohm):");
    tft.setTextSize(3);
    tft.setTextColor(C_TEXT, C_BG);
    tft.setCursor(6, 36);
    char rbuf[10];
    snprintf(rbuf, sizeof(rbuf), "%.1f", R_ohm);
    tft.printf("%-7s", rbuf);

    // --- X value ---
    tft.setTextSize(1);
    tft.setTextColor(C_GRID, C_BG);
    tft.setCursor(164, 28);
    tft.print("X (ohm):");
    tft.setTextSize(3);
    tft.setTextColor(x_inductive ? C_NEG : C_WARN, C_BG);
    tft.setCursor(164, 36);
    char xbuf[12];
    snprintf(xbuf, sizeof(xbuf), "%+.1f", X_ohm);
    tft.printf("%-7s", xbuf);

    // Small label for polarity
    tft.setTextSize(1);
    tft.setTextColor(x_inductive ? C_NEG : C_WARN, C_BG);
    tft.setCursor(264, 26);
    tft.print(x_inductive ? "(IND)" : "(CAP)");

    // --- |Z| and theta ---
    tft.setTextSize(1);
    tft.setTextColor(C_GRID, C_BG);
    tft.setCursor(6, 62);
    tft.print("|Z|:");
    tft.setTextColor(C_ACCENT, C_BG);
    tft.setCursor(36, 62);
    char zbuf[10];
    snprintf(zbuf, sizeof(zbuf), "%.1fR", Z_mag);
    tft.print(zbuf);

    tft.setTextColor(C_GRID, C_BG);
    tft.setCursor(110, 62);
    tft.print("Theta:");
    tft.setTextColor(C_ACCENT, C_BG);
    tft.setCursor(152, 62);
    char tbuf[10];
    snprintf(tbuf, sizeof(tbuf), "%+.1f deg", theta_deg);
    tft.print(tbuf);

    // --- SWR ---
    uint16_t swr_col = swr < 1.5f ? C_POS : swr < 3.0f ? C_WARN : C_FAIL;
    tft.fillRect(4, 74, 140, 28, C_DIM);
    tft.drawRect(4, 74, 140, 28, swr_col);
    tft.setTextSize(2);
    tft.setTextColor(swr_col, C_DIM);
    tft.setCursor(10, 80);
    char swrbuf[10];
    snprintf(swrbuf, sizeof(swrbuf), "SWR%.2f:1", swr);
    tft.print(swrbuf);

    // --- Return loss ---
    tft.fillRect(150, 74, 166, 28, C_DIM);
    tft.drawRect(150, 74, 166, 28, C_GRID);
    tft.setTextSize(2);
    tft.setTextColor(C_TEXT, C_DIM);
    tft.setCursor(156, 80);
    char rlbuf[14];
    snprintf(rlbuf, sizeof(rlbuf), "RL %5.1fdB", rl_db);
    tft.print(rlbuf);

    // --- L/C at selected frequency ---
    float L_nH = 0, C_pF = 0;
    computeLC(X_ohm, BANDS[band_idx].f_mhz, &L_nH, &C_pF);
    tft.setTextSize(1);
    tft.setTextColor(C_GRID, C_BG);
    tft.setCursor(6, 108);
    if (L_nH > 0) {
        if (L_nH >= 1000.0f)
            tft.printf("L = %.3f uH  (inductive)", L_nH/1000.0f);
        else
            tft.printf("L = %.1f nH  (inductive)", L_nH);
    } else if (C_pF > 0) {
        if (C_pF >= 1000.0f)
            tft.printf("C = %.3f nF  (capacitive)", C_pF/1000.0f);
        else
            tft.printf("C = %.1f pF  (capacitive)", C_pF);
    } else {
        tft.printf("X = 0  (resonant at %.3f MHz)", BANDS[band_idx].f_mhz);
    }

    // --- Null depth bargraph ---
    drawNullBar(4, 120, 312, 16, null_dbm);

    // --- Touch button hints ---
    tft.fillRect(0, 220, 320, 20, 0x0820);
    tft.setTextSize(1);
    tft.setTextColor(C_GRID, 0x0820);
    tft.setCursor(4, 227);
    tft.print("[SMITH] [BAND-] [BAND+] [CAL] [HOLD]");

    // Draw touch buttons
    struct { const char* lbl; int x; uint16_t col; } btns[] = {
        {"SMITH", 10, 0x0410},
        {"BND-",  75, 0x0410},
        {"BND+", 125, 0x0410},
        {"CAL",  180, 0x4000},
        {"HOLD", 240, 0x0420},
    };
    for (auto& b : btns) {
        tft.fillRoundRect(b.x, 140, 55, 22, 3, b.col);
        tft.setTextColor(C_TEXT, b.col);
        tft.setTextSize(1);
        tft.setCursor(b.x + 5, 148);
        tft.print(b.lbl);
    }
}

// ============================================================
// SCREEN 1: Smith Chart
// ============================================================
bool smith_drawn = false;

void drawSmithScreen() {
    if (!smith_drawn) {
        tft.fillScreen(C_SMITH_BG);

        // Header
        tft.fillRect(0, 0, 320, 20, C_HEADER);
        tft.setTextColor(C_TEXT, C_HEADER);
        tft.setTextSize(1);
        tft.setCursor(6, 6);
        tft.printf("SMITH CHART  Z0=50ohm  %s %.3fMHz",
                   BANDS[band_idx].name, BANDS[band_idx].f_mhz);

        // SWR circles overlay
        sc._cx = 155; sc._cy = 145; sc._r = 110;
        sc.drawChart();
        sc.drawSWRCircle(1.5f, 0x4208);
        sc.drawSWRCircle(2.0f, 0x4208);
        sc.drawSWRCircle(3.0f, 0x4208);

        // Annotations
        tft.setTextSize(1);
        tft.setTextColor(C_DIM, C_SMITH_BG);
        tft.setCursor(sc._cx - sc._r - 15, sc._cy - 3); tft.print("S");  // short
        tft.setCursor(sc._cx + sc._r + 3,  sc._cy - 3); tft.print("O");  // open
        tft.setCursor(sc._cx - 3, sc._cy - sc._r - 10); tft.print("+jX");
        tft.setCursor(sc._cx - 3, sc._cy + sc._r + 3);  tft.print("-jX");

        // Bottom readout strip
        tft.drawFastHLine(0, 250, 320, C_GRID);

        smith_drawn = true;
    }

    // Plot current point
    sc.plotPoint(R_ohm, X_ohm);

    // Bottom strip update
    tft.fillRect(0, 252, 320, 20, C_SMITH_BG);
    tft.setTextSize(1);
    tft.setTextColor(C_TEXT, C_SMITH_BG);
    tft.setCursor(4, 260);
    tft.printf("R=%.1f X=%+.1f |Z|=%.1f SWR=%.2f:1 RL=%.1fdB",
               R_ohm, X_ohm, Z_mag, swr, rl_db);

    // Touch hint
    tft.setCursor(4, 272);
    tft.setTextColor(C_GRID, C_SMITH_BG);
    tft.print("[TAP=MAIN]  [HOLD=CLEAR]");
}

// ============================================================
// SCREEN 2: Band select (touch)
// ============================================================
void drawBandScreen() {
    tft.fillScreen(C_BG);
    tft.fillRect(0, 0, 320, 22, C_HEADER);
    tft.setTextColor(C_TEXT, C_HEADER);
    tft.setTextSize(2);
    tft.setCursor(6, 4);
    tft.print("SELECT BAND");

    int cols = 2, rows = 5;
    int bw = 145, bh = 38;
    for (int i = 0; i < N_BANDS; i++) {
        int col = i % cols;
        int row = i / cols;
        int bx = col * (bw + 5) + 5;
        int by = row * (bh + 4) + 28;
        uint16_t bc = (i == band_idx) ? C_POS : C_DIM;
        tft.fillRoundRect(bx, by, bw, bh, 4, bc);
        tft.setTextColor(C_TEXT, bc);
        tft.setTextSize(2);
        tft.setCursor(bx + 10, by + 10);
        tft.printf("%s %.3fMHz", BANDS[i].name, BANDS[i].f_mhz);
    }
}

// ============================================================
// SCREEN 3: Calibration procedure
// ============================================================
void drawCalScreen() {
    tft.fillScreen(C_BG);
    tft.fillRect(0, 0, 320, 22, 0x8000);
    tft.setTextColor(C_TEXT, 0x8000);
    tft.setTextSize(2);
    tft.setCursor(6, 4);
    tft.print("CALIBRATION");

    tft.setTextSize(1);
    tft.setTextColor(C_TEXT, C_BG);
    int y = 28;
    auto line = [&](const char* s, uint16_t col=C_TEXT) {
        tft.setTextColor(col, C_BG);
        tft.setCursor(6, y);
        tft.print(s);
        y += 14;
    };

    line("ZERO PROCEDURE:", C_ACCENT);
    line("1. Connect 50-ohm load to ANT port.");
    line("2. Set R pot fully CCW (R=0 end).");
    line("3. Set X pot to center (X=0).");
    line("4. Enable noise source.");
    line("5. Tune for null (should be deep with 50-ohm load).");
    line("6. If R reads non-zero with 50R load, press ZERO R below.");
    y += 5;
    line("FULL-SCALE CHECK:", C_ACCENT);
    line("1. Connect 200-ohm resistor to ANT port.");
    line("2. Tune R pot for null. R should read ~200.");
    line("3. If off: press SCALE R below.");

    tft.fillRoundRect(10, 185, 90, 28, 4, 0x8000);
    tft.setTextColor(C_TEXT); tft.setTextSize(2);
    tft.setCursor(18, 193); tft.print("ZERO R");

    tft.fillRoundRect(115, 185, 90, 28, 4, 0x4000);
    tft.setCursor(120, 193); tft.print("SCALE R");

    tft.fillRoundRect(220, 185, 90, 28, 4, 0x0820);
    tft.setCursor(228, 193); tft.print("DONE");

    // Show current raw readings
    tft.setTextSize(1);
    tft.setTextColor(C_GRID, C_BG);
    tft.setCursor(6, 220);
    tft.printf("Raw R_V=%.3f  X_V=%.3f  Null=%.3f",
               readADCv(PIN_R_ADC),
               readADCv(PIN_X_ADC),
               readADCv(PIN_NULL_ADC));
}

// ============================================================
// Touch handler
// ============================================================
uint32_t touch_t0 = 0;

void handleTouch(int tx, int ty) {
    uint32_t held = millis() - touch_t0;

    if (screen_mode == 0) {
        // Smith button (10-65, 140-162)
        if (tx >= 10 && tx <= 65 && ty >= 140 && ty <= 162) {
            screen_mode = 1; smith_drawn = false;
            tft.fillScreen(C_SMITH_BG);
        }
        // Band- button
        else if (tx >= 75 && tx <= 130 && ty >= 140) {
            band_idx = max(0, band_idx - 1);
            tft.fillScreen(C_BG);
        }
        // Band+
        else if (tx >= 125 && tx <= 180 && ty >= 140) {
            band_idx = min(N_BANDS - 1, band_idx + 1);
            tft.fillScreen(C_BG);
        }
        // Cal
        else if (tx >= 180 && tx <= 235 && ty >= 140) {
            screen_mode = 3;
            drawCalScreen();
        }
    }
    else if (screen_mode == 1) {
        if (held > 1000) {
            sc.clearPoints();
        } else {
            screen_mode = 0;
            tft.fillScreen(C_BG);
        }
    }
    else if (screen_mode == 2) {
        // Band selection grid
        int cols = 2, bw = 145, bh = 38;
        for (int i = 0; i < N_BANDS; i++) {
            int bx = (i % cols) * (bw + 5) + 5;
            int by = (i / cols) * (bh + 4) + 28;
            if (tx >= bx && tx <= bx+bw && ty >= by && ty <= by+bh) {
                band_idx = i;
                break;
            }
        }
        screen_mode = 0;
        tft.fillScreen(C_BG);
    }
    else if (screen_mode == 3) {
        // ZERO R button
        if (tx >= 10 && tx <= 100 && ty >= 185 && ty <= 213) {
            r_offset = readADCv(PIN_R_ADC) / 3.3f * r_scale;
            EEPROM.put(EE_R_OFFSET, r_offset);
            EEPROM.put(EE_CAL_FLAG, (uint8_t)0xBE);
            EEPROM.commit();
        }
        // DONE
        else if (tx >= 220) {
            screen_mode = 0;
            tft.fillScreen(C_BG);
        }
        drawCalScreen();
    }
}

// ============================================================
// Setup
// ============================================================
void setup() {
    Serial.begin(115200);
    Serial.println("Noise Bridge CYD v1.0");

    pinMode(PIN_LED_R,  OUTPUT); digitalWrite(PIN_LED_R, HIGH);
    pinMode(PIN_X_POL,  INPUT_PULLDOWN);
    pinMode(PIN_NOISE_EN, OUTPUT); digitalWrite(PIN_NOISE_EN, HIGH);

    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);

    // TFT init
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(C_BG);

    // Backlight
    ledcSetup(0, 5000, 8);
    ledcAttachPin(PIN_BACKLIGHT, 0);
    ledcWrite(0, 200);

    // EEPROM calibration
    EEPROM.begin(32);
    if (EEPROM.read(EE_CAL_FLAG) == 0xBE) {
        EEPROM.get(EE_R_OFFSET, r_offset);
        EEPROM.get(EE_X_OFFSET, x_offset);
        EEPROM.get(EE_R_SCALE,  r_scale);
        EEPROM.get(EE_X_SCALE,  x_scale);
        Serial.printf("Cal: R_off=%.2f X_off=%.2f R_sc=%.1f\n",
                      r_offset, x_offset, r_scale);
    }

    // Splash screen
    tft.setTextColor(C_ACCENT, C_BG);
    tft.setTextSize(3);
    tft.setCursor(20, 80);
    tft.print("RF NOISE");
    tft.setCursor(20, 120);
    tft.print("BRIDGE");
    tft.setTextSize(1);
    tft.setTextColor(C_GRID, C_BG);
    tft.setCursor(20, 165);
    tft.print("TM-NB-001  1-60 MHz  50 ohm ref");
    tft.setCursor(20, 180);
    tft.print("Wheatstone bridge + AD8307 null detector");
    delay(2500);
    tft.fillScreen(C_BG);
}

// ============================================================
// Loop
// ============================================================
void loop() {
    uint32_t now = millis();

    // Measure every 100ms
    if (now - last_update > 100) {
        doMeasure();
        last_update = now;
    }

    // Display update
    if (now - last_display > 200) {
        last_display = now;
        if (screen_mode == 0) drawMainScreen();
        else if (screen_mode == 1) drawSmithScreen();
    }

    // Touch
    uint16_t tx, ty;
    if (tft.getTouch(&tx, &ty, 300)) {
        if (touch_t0 == 0) touch_t0 = now;
    } else {
        if (touch_t0 > 0) {
            handleTouch(tx, ty);
            touch_t0 = 0;
        }
    }

    // Serial debug
    static uint32_t last_ser = 0;
    if (now - last_ser > 2000) {
        last_ser = now;
        Serial.printf("R=%.1f X=%+.1f |Z|=%.1f SWR=%.2f null=%.0fdBm band=%s\n",
                      R_ohm, X_ohm, Z_mag, swr, null_dbm,
                      BANDS[band_idx].name);
    }
}
