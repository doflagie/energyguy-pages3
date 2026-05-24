// ============================================================
// Screwdriver Tuner — CYD Display (ESP32-2432S028, 320×240)
// TFT_eSPI ILI9341 + BluetoothSerial
// Receives telemetry from SC-Tuner ESP32 over BT Serial
// ============================================================

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <BluetoothSerial.h>
#include <string.h>
#include <stdlib.h>

// -----------------------------------------------------------
// Display
// -----------------------------------------------------------
TFT_eSPI    tft  = TFT_eSPI();
TFT_eSprite spr  = TFT_eSprite(&tft);
BluetoothSerial SerialBT;

// -----------------------------------------------------------
// Touch (XPT2046 on CYD via SPI)
// -----------------------------------------------------------
#define TOUCH_CS    33
#define TOUCH_IRQ   36
#define SCREEN_W   320
#define SCREEN_H   240
#define TAB_H       36
#define CONTENT_H  (SCREEN_H - TAB_H)

// -----------------------------------------------------------
// Colors (RGB565)
// -----------------------------------------------------------
#define COL_BG        0x0841   // very dark navy
#define COL_CARD      0x18E3   // dark grey-blue
#define COL_BORDER    0x3186   // medium grey
#define COL_WHITE     0xFFFF
#define COL_GREY      0x8C51
#define COL_CYAN      0x07FF
#define COL_GREEN     0x07E0
#define COL_GREEN_DIM 0x03E0
#define COL_YELLOW    0xFFE0
#define COL_RED       0xF800
#define COL_ORANGE    0xFD20
#define COL_BLUE      0x001F
#define COL_BLUE_MED  0x2B5F
#define COL_TAB_ACT   0x2A7F   // active tab highlight
#define COL_TAB_INACT 0x1082   // inactive tab

// -----------------------------------------------------------
// Page definitions
// -----------------------------------------------------------
enum Page { PAGE_SWR = 0, PAGE_MOTOR, PAGE_SETUP, PAGE_POWER, PAGE_GUIDE };
#define NUM_PAGES 5
const char* TAB_LABELS[NUM_PAGES] = { "SWR", "MOTOR", "SETUP", "POWER", "GUIDE" };

Page g_page = PAGE_SWR;

// -----------------------------------------------------------
// Tuner data from BT packet
// -----------------------------------------------------------
struct TunerData {
    char  band[8];
    long  pos_steps;
    float turns;
    float L_uH;
    float eff_pct;
    float swr;
    float fwd_w;
    float refl_dbm;
    int   tune_state;  // maps to TuneState enum on ESP32
    bool  homed;
    bool  connected;
    bool  valid;
};

TunerData g_data = {"---", 0, 0.0f, 0.0f, 0.0f, 9.99f, 0.0f, -99.0f, 0, false, false, false};

// -----------------------------------------------------------
// BT management
// -----------------------------------------------------------
#define BT_TARGET      "SC-Tuner"
#define BT_RECONNECT_MS 5000
unsigned long g_bt_last_try   = 0;
bool          g_bt_connected  = false;

char   g_bt_buf[256];
int    g_bt_buf_len = 0;

// -----------------------------------------------------------
// Rendering timing
// -----------------------------------------------------------
#define FRAME_MS  100    // 10 FPS
unsigned long g_last_frame = 0;

// -----------------------------------------------------------
// Touch state
// -----------------------------------------------------------
unsigned long g_last_touch = 0;
#define TOUCH_DEBOUNCE_MS 300

// -----------------------------------------------------------
// Forward declarations
// -----------------------------------------------------------
void drawTabBar(Page current);
void drawPageSWR();
void drawPageMotor();
void drawPageSetup();
void drawPagePower();
void drawPageGuide();
void maintainBT();
void readBT();
bool parseBTPacket(const char* line, TunerData& d);
uint16_t swrColor(float swr);
const char* tuneStateLabel(int state);
void drawCard(int x, int y, int w, int h, uint16_t fill = COL_CARD);
void drawLabelValue(int x, int y, const char* label, const char* value,
                    uint16_t col_val = COL_WHITE, uint8_t txt_size = 1);
void drawProgressBar(int x, int y, int w, int h,
                     float frac, uint16_t fill, uint16_t bg = COL_BORDER);
void checkTouch();

// -----------------------------------------------------------
// Utility: SWR color
// -----------------------------------------------------------
uint16_t swrColor(float swr) {
    if (swr <= 1.5f) return COL_GREEN;
    if (swr <= 2.5f) return COL_YELLOW;
    return COL_RED;
}

// -----------------------------------------------------------
// Utility: tune state label
// -----------------------------------------------------------
const char* tuneStateLabel(int state) {
    switch (state) {
        case 0: return "IDLE";
        case 1: return "HOMING...";
        case 2: return "MOVING...";
        case 3: return "SWEEPING...";
        case 4: return "LOCKED";
    }
    return "?";
}

// -----------------------------------------------------------
// Draw a filled rounded-ish rectangle card
// -----------------------------------------------------------
void drawCard(int x, int y, int w, int h, uint16_t fill) {
    spr.fillRoundRect(x, y, w, h, 4, fill);
    spr.drawRoundRect(x, y, w, h, 4, COL_BORDER);
}

// -----------------------------------------------------------
// Draw label + value row in sprite
// -----------------------------------------------------------
void drawLabelValue(int x, int y, const char* label, const char* value,
                    uint16_t col_val, uint8_t txt_size) {
    spr.setTextColor(COL_GREY, COL_CARD);
    spr.setTextSize(1);
    spr.setCursor(x, y);
    spr.print(label);

    spr.setTextColor(col_val, COL_CARD);
    spr.setTextSize(txt_size);
    int val_x = x + 90;
    spr.setCursor(val_x, y - (txt_size > 1 ? (txt_size - 1) * 4 : 0));
    spr.print(value);
}

// -----------------------------------------------------------
// Draw a horizontal progress bar
// -----------------------------------------------------------
void drawProgressBar(int x, int y, int w, int h, float frac, uint16_t fill, uint16_t bg) {
    frac = constrain(frac, 0.0f, 1.0f);
    spr.fillRoundRect(x, y, w, h, 2, bg);
    int filled = (int)(frac * w);
    if (filled > 2) {
        spr.fillRoundRect(x, y, filled, h, 2, fill);
    }
    // Tick marks at 15%, 37%, 51%, 73%, 100% (1.5/2.5/3/5/10 SWR normalized)
    uint16_t tick_cols[] = {COL_GREEN, COL_YELLOW, COL_YELLOW, COL_ORANGE, COL_RED};
    float tick_fracs[] = {0.063f, 0.167f, 0.222f, 0.444f, 1.0f};  // log-ish scale
    for (int t = 0; t < 5; t++) {
        int tx = x + (int)(tick_fracs[t] * w);
        spr.drawFastVLine(tx, y, h, tick_cols[t]);
    }
}

// -----------------------------------------------------------
// Draw tab bar at bottom
// -----------------------------------------------------------
void drawTabBar(Page current) {
    int tab_w = SCREEN_W / NUM_PAGES;
    int y0 = CONTENT_H;
    spr.fillRect(0, y0, SCREEN_W, TAB_H, COL_TAB_INACT);
    spr.drawFastHLine(0, y0, SCREEN_W, COL_BORDER);

    for (int i = 0; i < NUM_PAGES; i++) {
        int x0 = i * tab_w;
        if (i == (int)current) {
            spr.fillRect(x0 + 1, y0 + 1, tab_w - 2, TAB_H - 2, COL_TAB_ACT);
            spr.drawFastVLine(x0, y0, TAB_H, COL_CYAN);
            spr.drawFastVLine(x0 + tab_w - 1, y0, TAB_H, COL_CYAN);
        }
        spr.setTextColor((i == (int)current) ? COL_CYAN : COL_GREY);
        spr.setTextSize(1);
        int tw = strlen(TAB_LABELS[i]) * 6;
        spr.setCursor(x0 + (tab_w - tw) / 2, y0 + (TAB_H - 8) / 2);
        spr.print(TAB_LABELS[i]);
    }
}

// -----------------------------------------------------------
// Draw title bar common to all pages
// -----------------------------------------------------------
void drawTitleBar(const char* title) {
    spr.fillRect(0, 0, SCREEN_W, 24, COL_BLUE_MED);
    spr.setTextColor(COL_CYAN, COL_BLUE_MED);
    spr.setTextSize(1);
    spr.setCursor(6, 8);
    spr.print(title);

    // BT connection dot (top right)
    uint16_t dot_col = g_bt_connected ? COL_GREEN : COL_RED;
    spr.fillCircle(SCREEN_W - 10, 12, 5, dot_col);
    spr.setTextColor(COL_GREY, COL_BLUE_MED);
    spr.setCursor(SCREEN_W - 24, 8);
    spr.print(g_bt_connected ? "BT" : "  ");
}

// -----------------------------------------------------------
// PAGE_SWR
// -----------------------------------------------------------
void drawPageSWR() {
    spr.fillSprite(COL_BG);
    drawTitleBar("SCREWDRIVER TUNER");

    int y = 28;

    // Band + frequency
    drawCard(2, y, SCREEN_W - 4, 32);
    spr.setTextColor(COL_WHITE, COL_CARD);
    spr.setTextSize(2);
    spr.setCursor(8, y + 6);
    spr.print(g_data.band);
    char freq_buf[24];
    snprintf(freq_buf, sizeof(freq_buf), "%.3f MHz", 0.0f);  // freq not in packet
    spr.setTextColor(COL_GREY, COL_CARD);
    spr.setTextSize(1);
    spr.setCursor(80, y + 12);
    // Just show band name large
    y += 36;

    // SWR large value
    uint16_t scol = swrColor(g_data.swr);
    drawCard(2, y, 150, 60, COL_CARD);
    spr.setTextColor(COL_GREY, COL_CARD);
    spr.setTextSize(1);
    spr.setCursor(8, y + 4);
    spr.print("SWR");
    char swr_buf[16];
    if (g_data.valid) {
        snprintf(swr_buf, sizeof(swr_buf), "%.2f", g_data.swr);
    } else {
        strcpy(swr_buf, "---");
    }
    spr.setTextColor(scol, COL_CARD);
    spr.setTextSize(4);
    spr.setCursor(8, y + 18);
    spr.print(swr_buf);

    // Return loss  (RL = -20*log10(rho) dB, but we'll show as SWR quality)
    // Show efficiency instead on right side
    drawCard(156, y, SCREEN_W - 158, 60, COL_CARD);
    spr.setTextColor(COL_GREY, COL_CARD);
    spr.setTextSize(1);
    spr.setCursor(162, y + 4);
    spr.print("EFF%");
    char eff_buf[16];
    snprintf(eff_buf, sizeof(eff_buf), "%.1f", g_data.eff_pct);
    spr.setTextColor(COL_CYAN, COL_CARD);
    spr.setTextSize(2);
    spr.setCursor(162, y + 18);
    spr.print(eff_buf);
    spr.setTextSize(1);
    spr.setCursor(162, y + 38);
    spr.print("percent");
    y += 64;

    // SWR bar
    drawCard(2, y, SCREEN_W - 4, 22, COL_CARD);
    float swr_frac = 0.0f;
    if (g_data.swr > 1.0f) {
        // Map SWR 1.0–10.0 logarithmically
        swr_frac = log10f(constrain(g_data.swr, 1.0f, 10.0f)) / log10f(10.0f);
    }
    drawProgressBar(6, y + 3, SCREEN_W - 12, 16, swr_frac, scol);
    // Tick labels
    const char* tick_labels[] = {"1.5", "2", "3", "5", "10"};
    float tick_swrs[] = {1.5f, 2.0f, 3.0f, 5.0f, 10.0f};
    spr.setTextColor(COL_GREY, COL_CARD);
    spr.setTextSize(1);
    for (int t = 0; t < 5; t++) {
        float frac = log10f(tick_swrs[t]) / log10f(10.0f);
        int tx = 6 + (int)(frac * (SCREEN_W - 12)) - 3;
        spr.setCursor(tx, y + 4);
    }
    y += 26;

    // Tune state
    drawCard(2, y, SCREEN_W - 4, 24, COL_CARD);
    spr.setTextColor(COL_GREY, COL_CARD);
    spr.setTextSize(1);
    spr.setCursor(8, y + 4);
    spr.print("STATE: ");
    uint16_t state_col = COL_WHITE;
    if (g_data.tune_state == 4) state_col = COL_GREEN;       // LOCKED
    if (g_data.tune_state == 3) state_col = COL_YELLOW;      // SWEEPING
    if (g_data.tune_state == 1 || g_data.tune_state == 2) state_col = COL_CYAN;
    spr.setTextColor(state_col, COL_CARD);
    spr.print(tuneStateLabel(g_data.tune_state));
    if (g_data.tune_state == 4) spr.print(" \x07");  // checkmark approx
    y += 28;

    // Homed indicator
    spr.setTextColor(g_data.homed ? COL_GREEN : COL_RED, COL_BG);
    spr.setTextSize(1);
    spr.setCursor(8, y);
    spr.print(g_data.homed ? "HOMED" : "NOT HOMED");

    // Touch hint
    spr.setTextColor(COL_GREY, COL_BG);
    spr.setCursor(SCREEN_W / 2 + 10, y);
    spr.print(">> TAP RIGHT = TUNE");

    drawTabBar(g_page);
    spr.pushSprite(0, 0);
}

// -----------------------------------------------------------
// PAGE_MOTOR
// -----------------------------------------------------------
void drawPageMotor() {
    spr.fillSprite(COL_BG);
    drawTitleBar("MOTOR POSITION");

    int y = 28;

    // Large turns display
    drawCard(2, y, SCREEN_W - 4, 52, COL_CARD);
    char turns_buf[24];
    snprintf(turns_buf, sizeof(turns_buf), "%.2f TURNS", g_data.turns);
    spr.setTextColor(COL_GREY, COL_CARD);
    spr.setTextSize(1);
    spr.setCursor(8, y + 4);
    spr.print("WIPER POSITION");
    spr.setTextColor(COL_CYAN, COL_CARD);
    spr.setTextSize(3);
    spr.setCursor(8, y + 18);
    spr.print(turns_buf);
    y += 56;

    // Progress bar 0–41 turns
    drawCard(2, y, SCREEN_W - 4, 26, COL_CARD);
    float frac = g_data.turns / 41.0f;
    drawProgressBar(6, y + 4, SCREEN_W - 12, 18, frac, COL_CYAN, COL_BORDER);
    spr.setTextColor(COL_GREY, COL_CARD);
    spr.setTextSize(1);
    spr.setCursor(6, y + 6);
    spr.print("0");
    spr.setCursor(SCREEN_W - 24, y + 6);
    spr.print("41T");
    y += 30;

    // Inductance
    drawCard(2, y, SCREEN_W - 4, 28, COL_CARD);
    char l_buf[24];
    snprintf(l_buf, sizeof(l_buf), "%.3f uH", g_data.L_uH);
    drawLabelValue(8, y + 8, "INDUCTANCE", l_buf, COL_YELLOW, 2);
    y += 32;

    // Step count
    drawCard(2, y, SCREEN_W - 4, 22, COL_CARD);
    char step_buf[40];
    snprintf(step_buf, sizeof(step_buf), "%ld / 304220", g_data.pos_steps);
    drawLabelValue(8, y + 7, "STEP", step_buf, COL_WHITE, 1);
    y += 26;

    // State + homed
    drawCard(2, y, SCREEN_W / 2 - 4, 24, COL_CARD);
    drawLabelValue(8, y + 7, "STATE", tuneStateLabel(g_data.tune_state), COL_CYAN, 1);

    drawCard(SCREEN_W / 2, y, SCREEN_W / 2 - 2, 24, COL_CARD);
    spr.setTextColor(COL_GREY, COL_CARD);
    spr.setTextSize(1);
    spr.setCursor(SCREEN_W / 2 + 8, y + 4);
    spr.print("HOMED");
    uint16_t homed_col = g_data.homed ? COL_GREEN : COL_RED;
    spr.fillCircle(SCREEN_W - 18, y + 12, 6, homed_col);

    drawTabBar(g_page);
    spr.pushSprite(0, 0);
}

// -----------------------------------------------------------
// PAGE_SETUP
// -----------------------------------------------------------
void drawPageSetup() {
    spr.fillSprite(COL_BG);
    drawTitleBar("SETUP GUIDE");

    int y = 28;
    spr.setTextSize(1);

    // VHF/UHF bands: no loading coil
    bool is_vhf = (g_data.turns < 0.1f &&
                   strcmp(g_data.band, "10m") != 0 &&
                   strcmp(g_data.band, "12m") != 0 &&
                   strcmp(g_data.band, "15m") != 0 &&
                   strcmp(g_data.band, "17m") != 0 &&
                   strcmp(g_data.band, "20m") != 0 &&
                   strcmp(g_data.band, "30m") != 0 &&
                   strcmp(g_data.band, "40m") != 0 &&
                   strcmp(g_data.band, "60m") != 0 &&
                   strcmp(g_data.band, "80m") != 0 &&
                   strcmp(g_data.band, "160m") != 0);

    bool is_160 = (strcmp(g_data.band, "160m") == 0);

    // Band display
    drawCard(2, y, SCREEN_W - 4, 28);
    spr.setTextColor(COL_WHITE, COL_CARD);
    spr.setTextSize(2);
    spr.setCursor(8, y + 6);
    spr.print(g_data.band);
    spr.setTextSize(1);
    spr.setTextColor(COL_GREY, COL_CARD);
    char eff_buf[20];
    snprintf(eff_buf, sizeof(eff_buf), "%.1f%% eff", g_data.eff_pct);
    spr.setCursor(60, y + 12);
    spr.print(eff_buf);
    y += 32;

    if (is_160) {
        // Warn about ext coil
        drawCard(2, y, SCREEN_W - 4, 36, 0x4000);  // dark red
        spr.setTextColor(COL_YELLOW, 0x4000);
        spr.setTextSize(1);
        spr.setCursor(8, y + 4);
        spr.print("! INSTALL 160m EXT COIL");
        spr.setCursor(8, y + 16);
        spr.print("  Main coil at MAX (41 turns)");
        spr.setCursor(8, y + 28);
        spr.print("  Eff: 0.13% — lossy, QRP only");
        y += 40;

    } else if (is_vhf) {
        drawCard(2, y, SCREEN_W - 4, 58, COL_CARD);
        spr.setTextColor(COL_YELLOW, COL_CARD);
        spr.setTextSize(1);
        spr.setCursor(8, y + 4);
        spr.print("REMOVE LOADING COIL");
        spr.setCursor(8, y + 16);
        spr.print("Direct feed — no coil needed");
        spr.setTextColor(COL_CYAN, COL_CARD);
        spr.setCursor(8, y + 28);
        // Band-specific element length hints
        if      (strcmp(g_data.band, "6m")    == 0) spr.print("Element: 1.40m QW whip");
        else if (strcmp(g_data.band, "2m")    == 0) spr.print("Element: 0.488m QW stub");
        else if (strcmp(g_data.band, "1.25m") == 0) spr.print("Element: 0.321m stub");
        else if (strcmp(g_data.band, "70cm")  == 0) spr.print("Element: 0.160m stub");
        else if (strcmp(g_data.band, "33cm")  == 0) spr.print("Element: stub antenna");
        else spr.print("See band stub chart");
        spr.setTextColor(COL_GREY, COL_CARD);
        spr.setCursor(8, y + 40);
        spr.print("Motor: retract coil to 0 steps");
        y += 62;

    } else {
        // HF band: coil info
        drawCard(2, y, SCREEN_W - 4, 72, COL_CARD);
        spr.setTextColor(COL_WHITE, COL_CARD);
        spr.setTextSize(1);
        spr.setCursor(8, y + 4);
        spr.print("HF LOADING COIL IN USE");

        char buf[48];
        snprintf(buf, sizeof(buf), "Target: %.2f turns (%.3f uH)",
            g_data.turns, g_data.L_uH);
        spr.setTextColor(COL_CYAN, COL_CARD);
        spr.setCursor(8, y + 18);
        spr.print(buf);

        spr.setTextColor(COL_GREY, COL_CARD);
        spr.setCursor(8, y + 30);
        spr.print("Step to default pos, then auto-tune");
        spr.setCursor(8, y + 42);
        spr.print("Min SWR search by gradient sweep");
        spr.setCursor(8, y + 54);

        snprintf(buf, sizeof(buf), "Expected SWR after tune: ~1.2");
        spr.print(buf);
        y += 76;
    }

    // Efficiency bar
    drawCard(2, y, SCREEN_W - 4, 28, COL_CARD);
    spr.setTextColor(COL_GREY, COL_CARD);
    spr.setTextSize(1);
    spr.setCursor(8, y + 4);
    spr.print("EFFICIENCY");
    float eff_frac = g_data.eff_pct / 100.0f;
    uint16_t eff_col = (g_data.eff_pct > 70.0f) ? COL_GREEN :
                       (g_data.eff_pct > 20.0f) ? COL_YELLOW : COL_RED;
    drawProgressBar(8, y + 14, SCREEN_W - 16, 10, eff_frac, eff_col);

    drawTabBar(g_page);
    spr.pushSprite(0, 0);
}

// -----------------------------------------------------------
// PAGE_POWER
// -----------------------------------------------------------
void drawPagePower() {
    spr.fillSprite(COL_BG);
    drawTitleBar("RF POWER");

    int y = 28;

    // Forward power
    drawCard(2, y, SCREEN_W - 4, 38, COL_CARD);
    char buf[40];
    float fwd_dbm = 10.0f * log10f(g_data.fwd_w * 1000.0f + 1e-9f) + 30.0f;
    // Approximate from watts
    snprintf(buf, sizeof(buf), "%.3f W", g_data.fwd_w);
    drawLabelValue(8, y + 6, "FORWARD", buf, COL_GREEN, 2);
    snprintf(buf, sizeof(buf), "%.1f dBm", fwd_dbm);
    spr.setTextColor(COL_GREY, COL_CARD);
    spr.setTextSize(1);
    spr.setCursor(8, y + 28);
    spr.print(buf);
    y += 42;

    // Reflected power
    drawCard(2, y, SCREEN_W - 4, 30, COL_CARD);
    snprintf(buf, sizeof(buf), "%.1f dBm", g_data.refl_dbm);
    drawLabelValue(8, y + 8, "REFLECTED", buf, COL_RED, 1);
    y += 34;

    // Reflection coefficient
    drawCard(2, y, SCREEN_W - 4, 30, COL_CARD);
    float rho = powf(10.0f, (g_data.refl_dbm - fwd_dbm) / 20.0f);
    rho = constrain(rho, 0.0f, 1.0f);
    snprintf(buf, sizeof(buf), "%.3f", rho);
    drawLabelValue(8, y + 8, "RHO (rho)", buf, COL_YELLOW, 1);
    y += 34;

    // Radiated power = fwd × efficiency
    drawCard(2, y, SCREEN_W - 4, 38, COL_CARD);
    float p_rad = g_data.fwd_w * (g_data.eff_pct / 100.0f);
    snprintf(buf, sizeof(buf), "%.4f W", p_rad);
    drawLabelValue(8, y + 6, "RADIATED", buf, COL_CYAN, 2);
    spr.setTextColor(COL_GREY, COL_CARD);
    spr.setTextSize(1);
    snprintf(buf, sizeof(buf), "(%.1f%% of %.3fW fwd)", g_data.eff_pct, g_data.fwd_w);
    spr.setCursor(8, y + 28);
    spr.print(buf);
    y += 42;

    // SWR
    drawCard(2, y, SCREEN_W - 4, 22, COL_CARD);
    snprintf(buf, sizeof(buf), "%.2f", g_data.swr);
    drawLabelValue(8, y + 6, "SWR", buf, swrColor(g_data.swr), 1);

    drawTabBar(g_page);
    spr.pushSprite(0, 0);
}

// -----------------------------------------------------------
// PAGE_GUIDE
// -----------------------------------------------------------
void drawPageGuide() {
    spr.fillSprite(COL_BG);
    drawTitleBar("TUNING PROCEDURE");

    int y = 30;
    spr.setTextSize(1);

    bool is_vhf = (g_data.turns < 0.1f &&
                   strcmp(g_data.band, "10m") != 0 &&
                   strcmp(g_data.band, "12m") != 0 &&
                   strcmp(g_data.band, "15m") != 0 &&
                   strcmp(g_data.band, "17m") != 0 &&
                   strcmp(g_data.band, "20m") != 0 &&
                   strcmp(g_data.band, "30m") != 0 &&
                   strcmp(g_data.band, "40m") != 0 &&
                   strcmp(g_data.band, "60m") != 0 &&
                   strcmp(g_data.band, "80m") != 0 &&
                   strcmp(g_data.band, "160m") != 0);

    // Context label
    drawCard(2, y, SCREEN_W - 4, 22, COL_BLUE_MED);
    spr.setTextColor(COL_CYAN, COL_BLUE_MED);
    spr.setCursor(8, y + 7);
    spr.print(is_vhf ? "VHF/UHF BAND — Direct feed mode"
                     : "HF BAND — Loading coil mode");
    y += 26;

    // Procedure steps
    struct Step { uint16_t col; const char* text; };

    if (is_vhf) {
        Step steps[] = {
            {COL_YELLOW, "1. Remove loading coil from antenna"},
            {COL_WHITE,  "2. Attach correct length element"},
            {COL_CYAN,   "3. Check element length (SETUP tab)"},
            {COL_WHITE,  "4. Verify coil wiper at step 0"},
            {COL_GREEN,  "5. Transmit; check SWR on SWR tab"},
            {COL_GREY,   "6. Trim element if SWR > 1.5"},
        };
        for (int i = 0; i < 6; i++) {
            spr.setTextColor(steps[i].col, COL_BG);
            spr.setCursor(8, y);
            spr.print(steps[i].text);
            y += 16;
        }
    } else if (strcmp(g_data.band, "160m") == 0) {
        Step steps[] = {
            {COL_YELLOW, "1. Install 160m extension coil"},
            {COL_WHITE,  "2. HOME motor (full coil = 41T)"},
            {COL_CYAN,   "3. Key up at low power (< 5W)"},
            {COL_GREEN,  "4. Press AUTO-TUNE or tap SWR page"},
            {COL_WHITE,  "5. Wait for LOCKED — increase power"},
            {COL_GREY,   "Eff 0.13%: very lossy — QRP only"},
        };
        for (int i = 0; i < 6; i++) {
            spr.setTextColor(steps[i].col, COL_BG);
            spr.setCursor(8, y);
            spr.print(steps[i].text);
            y += 16;
        }
    } else {
        Step steps[] = {
            {COL_WHITE,  "1. Select band (BAND+/- buttons)"},
            {COL_CYAN,   "2. Motor moves to default position"},
            {COL_WHITE,  "3. Key up at low power (< 5W)"},
            {COL_GREEN,  "4. AUTO-TUNE sweeps for min SWR"},
            {COL_YELLOW, "5. STATE changes: SWEEPING->LOCKED"},
            {COL_WHITE,  "6. Increase to full power when LOCKED"},
        };
        for (int i = 0; i < 6; i++) {
            spr.setTextColor(steps[i].col, COL_BG);
            spr.setCursor(8, y);
            spr.print(steps[i].text);
            y += 16;
        }
        y += 2;
        spr.setTextColor(COL_GREY, COL_BG);
        spr.setCursor(8, y);
        spr.print("Positions saved per-band to NVS");
    }

    drawTabBar(g_page);
    spr.pushSprite(0, 0);
}

// -----------------------------------------------------------
// Parse BT packet
// "BAND:%s,POS:%ld,TURNS:%.2f,L:%.3f,EFF:%.1f,SWR:%.2f,FWD:%.3fW,REFL:%.1f,STATE:%d,HOMED:%d\n"
// -----------------------------------------------------------
bool parseBTPacket(const char* line, TunerData& d) {
    // Parse each field by keyword
    const char* p;

    p = strstr(line, "BAND:");
    if (!p) return false;
    strncpy(d.band, p + 5, 7);
    d.band[7] = '\0';
    // Trim at comma
    char* comma = strchr(d.band, ',');
    if (comma) *comma = '\0';

    p = strstr(line, "POS:");
    if (p) d.pos_steps = atol(p + 4);

    p = strstr(line, "TURNS:");
    if (p) d.turns = atof(p + 6);

    p = strstr(line, "L:");
    if (p) d.L_uH = atof(p + 2);

    p = strstr(line, "EFF:");
    if (p) d.eff_pct = atof(p + 4);

    p = strstr(line, "SWR:");
    if (p) d.swr = atof(p + 4);

    p = strstr(line, "FWD:");
    if (p) d.fwd_w = atof(p + 4);
    // Strip trailing 'W' — atof stops at 'W', so fine

    p = strstr(line, "REFL:");
    if (p) d.refl_dbm = atof(p + 5);

    p = strstr(line, "STATE:");
    if (p) d.tune_state = atoi(p + 6);

    p = strstr(line, "HOMED:");
    if (p) d.homed = (atoi(p + 6) != 0);

    d.valid = true;
    return true;
}

// -----------------------------------------------------------
// maintainBT — reconnect to SC-Tuner every 5 s if lost
// -----------------------------------------------------------
void maintainBT() {
    if (SerialBT.connected()) {
        g_bt_connected = true;
        g_data.connected = true;
        return;
    }
    g_bt_connected = false;
    g_data.connected = false;

    unsigned long now = millis();
    if (now - g_bt_last_try < BT_RECONNECT_MS) return;
    g_bt_last_try = now;

    Serial.println("BT: attempting connect to SC-Tuner...");
    // connect() is blocking up to ~5s; wrap in separate task would be better,
    // but for simplicity call with short timeout via available API
    // TFT will freeze briefly during connect — acceptable for this app
    SerialBT.connect(BT_TARGET);
}

#define BT_RECONNECT_MS 5000

// -----------------------------------------------------------
// readBT — accumulate characters, parse on newline
// -----------------------------------------------------------
void readBT() {
    while (SerialBT.available()) {
        char c = (char)SerialBT.read();
        if (c == '\n' || c == '\r') {
            if (g_bt_buf_len > 5) {
                g_bt_buf[g_bt_buf_len] = '\0';
                TunerData tmp = g_data;
                if (parseBTPacket(g_bt_buf, tmp)) {
                    tmp.connected = true;
                    g_data = tmp;
                }
            }
            g_bt_buf_len = 0;
        } else {
            if (g_bt_buf_len < (int)(sizeof(g_bt_buf) - 1)) {
                g_bt_buf[g_bt_buf_len++] = c;
            }
        }
    }
}

// -----------------------------------------------------------
// checkTouch — read XPT2046, map to tab or SWR action
// -----------------------------------------------------------
void checkTouch() {
    unsigned long now = millis();
    if (now - g_last_touch < TOUCH_DEBOUNCE_MS) return;

    // CYD XPT2046: read via SPI (basic polling)
    digitalWrite(TOUCH_CS, LOW);
    // Check IRQ pin — low means touched
    if (digitalRead(TOUCH_IRQ) != LOW) {
        digitalWrite(TOUCH_CS, HIGH);
        return;
    }

    // Read X, Y raw via SPI manually
    // Command bytes for XPT2046: 0xD0 = Y, 0x90 = X
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    // Read X
    SPI.transfer(0x90);
    uint16_t raw_x = SPI.transfer16(0) >> 3;
    // Read Y
    SPI.transfer(0xD0);
    uint16_t raw_y = SPI.transfer16(0) >> 3;
    SPI.endTransaction();
    digitalWrite(TOUCH_CS, HIGH);

    // Map raw (0–4095) to screen coordinates
    // Calibration constants typical for CYD (adjust per unit)
    int tx = map(raw_x, 300, 3800, 0, SCREEN_W);
    int ty = map(raw_y, 300, 3800, SCREEN_H, 0);
    tx = constrain(tx, 0, SCREEN_W - 1);
    ty = constrain(ty, 0, SCREEN_H - 1);

    g_last_touch = now;

    // Tab bar at bottom CONTENT_H..SCREEN_H
    if (ty >= CONTENT_H) {
        int tab_w = SCREEN_W / NUM_PAGES;
        int tab_idx = tx / tab_w;
        if (tab_idx >= 0 && tab_idx < NUM_PAGES) {
            g_page = (Page)tab_idx;
        }
        return;
    }

    // SWR page: tap right half = auto-tune request
    // (CYD can't send BT commands directly in this build, but we can note intent)
    if (g_page == PAGE_SWR && tx > SCREEN_W / 2) {
        // Visual flash — actual tune triggered on ESP32 side via BT command
        // or physically pressing TUNE button; mark with brief indicator
        // For future: send "CMD:TUNE\n" over BT
        if (SerialBT.connected()) {
            SerialBT.print("CMD:TUNE\n");
        }
    }
}

// -----------------------------------------------------------
// setup
// -----------------------------------------------------------
void setup() {
    Serial.begin(115200);
    Serial.println("CYD Screwdriver Display v1.0");

    // TFT
    tft.init();
    tft.setRotation(1);  // Landscape
    tft.fillScreen(COL_BG);

    // Sprite: full screen double buffer
    spr.createSprite(SCREEN_W, SCREEN_H);
    spr.setSwapBytes(true);

    // Touch CS + IRQ
    pinMode(TOUCH_CS,  OUTPUT);
    pinMode(TOUCH_IRQ, INPUT);
    digitalWrite(TOUCH_CS, HIGH);

    // SPI for touch (share TFT SPI bus, different CS)
    // TFT_eSPI already inits SPI; we reuse it

    // Splash screen
    spr.fillSprite(COL_BG);
    spr.setTextColor(COL_CYAN, COL_BG);
    spr.setTextSize(2);
    spr.setCursor(30, 80);
    spr.print("SCREWDRIVER TUNER");
    spr.setTextSize(1);
    spr.setTextColor(COL_GREY, COL_BG);
    spr.setCursor(60, 110);
    spr.print("Connecting to SC-Tuner...");
    spr.pushSprite(0, 0);

    // BT
    SerialBT.begin("CYD-Display", true);  // master mode
    delay(500);

    // Attempt initial connect
    g_bt_last_try = millis() - BT_RECONNECT_MS;  // force immediate attempt
    maintainBT();

    Serial.println("setup() done");
}

// -----------------------------------------------------------
// loop
// -----------------------------------------------------------
void loop() {
    unsigned long now = millis();

    // BT maintenance and receive
    maintainBT();
    readBT();

    // Touch
    checkTouch();

    // Render at ~10 FPS
    if (now - g_last_frame >= FRAME_MS) {
        g_last_frame = now;

        switch (g_page) {
            case PAGE_SWR:   drawPageSWR();   break;
            case PAGE_MOTOR: drawPageMotor(); break;
            case PAGE_SETUP: drawPageSetup(); break;
            case PAGE_POWER: drawPagePower(); break;
            case PAGE_GUIDE: drawPageGuide(); break;
        }
    }
}
