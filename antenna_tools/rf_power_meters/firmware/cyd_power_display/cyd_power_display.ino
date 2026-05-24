/**
 * cyd_power_display.ino — TM-PWR-CYD-001 Rev A
 * CYD (Cheap Yellow Display) touchscreen firmware for RF Power Meter
 * ILI9341 320×240 TFT + XPT2046 touch + ESP32 WiFi
 *
 * Polls the ESP32 power meter via BT serial or direct I2C.
 * Four display modes: LIVE, PEAK, LOG, SWEEP.
 * Touch zones for mode switching, range control, peak reset, log toggle.
 *
 * Libraries:
 *   TFT_eSPI        (configured for CYD: ILI9341, SPI)
 *   XPT2046_Touchscreen
 *   ArduinoJson     >= 6.21.0
 *   BluetoothSerial (built-in ESP32)
 *
 * TFT_eSPI User_Setup.h for CYD:
 *   #define ILI9341_DRIVER
 *   #define TFT_MISO 12  #define TFT_MOSI 13  #define TFT_SCLK 14
 *   #define TFT_CS   15  #define TFT_DC    2  #define TFT_RST  -1
 *   #define TFT_BL   21  (backlight PWM)
 *   #define TOUCH_CS 33
 *   #define SPI_FREQUENCY 40000000
 */

#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <ArduinoJson.h>
#include <BluetoothSerial.h>

// ─── CYD Pin Definitions ─────────────────────────────────────────────────────
#define TFT_BL_PIN      21
#define TOUCH_CS_PIN    33
#define TOUCH_IRQ_PIN   36

// ─── Display Constants ────────────────────────────────────────────────────────
#define SCR_W     320
#define SCR_H     240

// Layout zones
#define TOP_BAR_H   28
#define BOT_BAR_H   44
#define CHART_TOP   TOP_BAR_H
#define CHART_H     (SCR_H - TOP_BAR_H - BOT_BAR_H)  // 168px
#define CHART_BOT   (CHART_TOP + CHART_H)

// Colors (ILI9341 RGB565)
#define COL_BG      0x0000   // Black
#define COL_GREEN   0x07E0   // Green — good SWR, low power
#define COL_YELLOW  0xFFE0   // Yellow — marginal
#define COL_RED     0xF800   // Red — high SWR, overload
#define COL_CYAN    0x07FF   // Cyan — accent, labels
#define COL_ORANGE  0xFD20   // Orange — peak indicators
#define COL_WHITE   0xFFFF
#define COL_GRAY    0x7BEF
#define COL_DARKGRAY 0x39E7
#define COL_PANEL   0x2104   // Dark panel background

// SWR thresholds for color coding
#define SWR_GOOD    1.5f
#define SWR_WARN    2.5f
#define SWR_MAX_DISP 10.0f

// ─── Display Modes ────────────────────────────────────────────────────────────
enum DispMode { MODE_LIVE=0, MODE_PEAK, MODE_LOG, MODE_SWEEP, MODE_COUNT };

// ─── State ────────────────────────────────────────────────────────────────────
TFT_eSPI          tft;
TFT_eSprite       spr(&tft);
XPT2046_Touchscreen touch(TOUCH_CS_PIN, TOUCH_IRQ_PIN);
BluetoothSerial   btSerial;

DispMode   curMode    = MODE_LIVE;
uint32_t   lastTouch  = 0;
uint32_t   lastPoll   = 0;
uint32_t   lastDraw   = 0;
bool       btConn     = false;

// Power data from meter
struct MeterData {
    float fwd_w    = 0;
    float ref_w    = 0;
    float peak_w   = 0;
    float swr      = 1.0f;
    float rl_db    = 0;
    float gamma    = 0;
    float freq_mhz = 0;
    float temp_c   = 25;
    int   range    = 0;
    bool  valid    = false;
    uint32_t ts_ms = 0;
    char band[8]   = "HF";
    char range_label[12] = "1-10W";
    bool sd_log    = false;
};

MeterData live;
MeterData peak;

// Sweep data
struct SweepPt { float freq; float swr; float fwd_w; };
SweepPt  sweepPts[200];
int      sweepN    = 0;
float    sweepMaxW = 0;

// Bargraph history (live SWR, 200 samples for trend line)
float  swrHistory[200];
int    swrHistN    = 0;
float  fwdHistory[200];

// Log stats
uint32_t logCount  = 0;
char     logFile[32] = "";

// ─── Color from SWR ──────────────────────────────────────────────────────────
uint16_t swrColor(float swr) {
    if (swr <= SWR_GOOD)  return COL_GREEN;
    if (swr <= SWR_WARN)  return COL_YELLOW;
    return COL_RED;
}

uint16_t powerColor(float w, float max_w) {
    float ratio = w / max(max_w, 0.001f);
    if (ratio < 0.3f) return COL_GREEN;
    if (ratio < 0.7f) return COL_YELLOW;
    return COL_RED;
}

// ─── Draw: Top Status Bar ─────────────────────────────────────────────────────
void drawTopBar() {
    spr.fillRect(0, 0, SCR_W, TOP_BAR_H, COL_PANEL);
    spr.setTextColor(COL_CYAN, COL_PANEL);
    spr.setTextSize(1);

    // Mode tabs
    const char* tabs[] = {"LIVE", "PEAK", "LOG", "SWEEP"};
    for (int i = 0; i < MODE_COUNT; i++) {
        uint16_t bg = (i == curMode) ? COL_CYAN : COL_DARKGRAY;
        uint16_t fg = (i == curMode) ? COL_BG : COL_GRAY;
        spr.fillRect(i * 55, 0, 54, TOP_BAR_H - 2, bg);
        spr.setTextColor(fg, bg);
        spr.drawString(tabs[i], i * 55 + 8, 8, 2);
    }

    // BT status (right side)
    spr.setTextColor(btConn ? COL_GREEN : COL_GRAY, COL_PANEL);
    spr.drawString(btConn ? "BT" : "--", 290, 8, 2);

    // Band label
    spr.setTextColor(COL_ORANGE, COL_PANEL);
    spr.drawString(live.band, 235, 8, 2);
}

// ─── Draw: Bottom Button Bar ──────────────────────────────────────────────────
void drawBottomBar() {
    spr.fillRect(0, CHART_BOT, SCR_W, BOT_BAR_H, COL_PANEL);
    spr.drawFastHLine(0, CHART_BOT, SCR_W, COL_GRAY);

    // Buttons: [RANGE] [PEAK RST] [LOG] [MODE]
    struct BtnDef { int x; int w; const char* label; uint16_t col; };
    BtnDef btns[] = {
        {  0, 70, live.range_label, COL_CYAN   },
        { 75, 70, "PEAK RST",       COL_ORANGE },
        {150, 70, live.sd_log ? "LOG ON" : "LOG OFF", live.sd_log ? COL_GREEN : COL_GRAY},
        {225, 95, "AUTO/MAN",       COL_CYAN   },
    };
    for (auto& b : btns) {
        spr.fillRect(b.x + 2, CHART_BOT + 4, b.w - 4, BOT_BAR_H - 8, COL_DARKGRAY);
        spr.drawRect(b.x + 2, CHART_BOT + 4, b.w - 4, BOT_BAR_H - 8, b.col);
        spr.setTextColor(b.col, COL_DARKGRAY);
        spr.drawString(b.label, b.x + 6, CHART_BOT + 12, 1);
    }
}

// ─── Draw: LIVE Mode ──────────────────────────────────────────────────────────
void drawLiveMode() {
    int cy = CHART_TOP;
    int ch = CHART_H;
    spr.fillRect(0, cy, SCR_W, ch, COL_BG);

    uint16_t swrCol = swrColor(live.swr);

    // Large SWR display (left half)
    spr.setTextColor(swrCol, COL_BG);
    spr.drawString("SWR", 10, cy + 10, 2);
    char buf[16];
    if (live.valid)
        snprintf(buf, sizeof(buf), "%.2f", live.swr);
    else
        strcpy(buf, "---");
    spr.setTextColor(swrCol, COL_BG);
    spr.drawString(buf, 10, cy + 28, 4);  // large font

    // Forward power (right of SWR)
    spr.setTextColor(COL_CYAN, COL_BG);
    spr.drawString("FWD", 170, cy + 10, 2);
    if (live.valid) {
        if (live.fwd_w >= 100)     snprintf(buf, sizeof(buf), "%.0fW", live.fwd_w);
        else if (live.fwd_w >= 10) snprintf(buf, sizeof(buf), "%.1fW", live.fwd_w);
        else                        snprintf(buf, sizeof(buf), "%.2fW", live.fwd_w);
    } else {
        strcpy(buf, "---");
    }
    spr.setTextColor(powerColor(live.fwd_w, 100.0f), COL_BG);
    spr.drawString(buf, 170, cy + 28, 4);

    // SWR bargraph
    int bar_y = cy + 80;
    int bar_x = 10;
    int bar_w = SCR_W - 20;
    int bar_h = 20;
    spr.drawRect(bar_x, bar_y, bar_w, bar_h, COL_GRAY);
    if (live.valid && live.swr > 1.0f) {
        float ratio = min((live.swr - 1.0f) / (SWR_MAX_DISP - 1.0f), 1.0f);
        int fill_w = (int)(ratio * (bar_w - 2));
        // Color gradient: green → yellow → red
        for (int bx = 0; bx < fill_w; bx++) {
            float pos = (float)bx / (bar_w - 2);
            uint16_t c = (pos < 0.3f) ? COL_GREEN : (pos < 0.6f ? COL_YELLOW : COL_RED);
            spr.drawFastVLine(bar_x + 1 + bx, bar_y + 1, bar_h - 2, c);
        }
        // SWR scale marks
        for (float s : {1.5f, 2.0f, 3.0f, 5.0f}) {
            int mx = bar_x + (int)((s - 1.0f) / (SWR_MAX_DISP - 1.0f) * (bar_w - 2));
            spr.drawFastVLine(mx, bar_y + bar_h, 6, COL_GRAY);
            snprintf(buf, sizeof(buf), "%.0f", s);
            spr.setTextColor(COL_GRAY, COL_BG);
            spr.drawString(buf, mx - 3, bar_y + bar_h + 7, 1);
        }
    }

    // Reflected power + Return loss (lower section)
    spr.setTextColor(COL_GRAY, COL_BG);
    spr.drawString("REF:", 10, cy + 120, 2);
    if (live.valid) snprintf(buf, sizeof(buf), "%.2f W", live.ref_w);
    else            strcpy(buf, "---");
    spr.setTextColor(COL_ORANGE, COL_BG);
    spr.drawString(buf, 70, cy + 120, 2);

    spr.setTextColor(COL_GRAY, COL_BG);
    spr.drawString("RL:", 180, cy + 120, 2);
    if (live.valid) snprintf(buf, sizeof(buf), "%.1f dB", live.rl_db);
    else            strcpy(buf, "---");
    spr.setTextColor(COL_CYAN, COL_BG);
    spr.drawString(buf, 215, cy + 120, 2);

    // Freq + Temp footer
    spr.setTextColor(COL_DARKGRAY, COL_BG);
    if (live.freq_mhz > 0)
        snprintf(buf, sizeof(buf), "%.3f MHz", live.freq_mhz);
    else
        strcpy(buf, "-- MHz");
    spr.drawString(buf, 10, cy + 142, 1);
    snprintf(buf, sizeof(buf), "%.0f°C", live.temp_c);
    spr.drawString(buf, 200, cy + 142, 1);

    // SWR trend (tiny history graph, right side)
    if (swrHistN >= 2) {
        int gx = SCR_W - 65;
        int gy = cy + 28;
        int gw = 60;
        int gh = 50;
        spr.drawRect(gx, gy, gw, gh, COL_DARKGRAY);
        for (int i = 1; i < min(swrHistN, gw); i++) {
            int idx0 = max(0, swrHistN - gw + i - 1);
            int idx1 = max(0, swrHistN - gw + i);
            float s0 = min(swrHistory[idx0], SWR_MAX_DISP);
            float s1 = min(swrHistory[idx1], SWR_MAX_DISP);
            int y0 = gy + gh - 1 - (int)((s0 - 1.0f) / (SWR_MAX_DISP - 1.0f) * (gh - 2));
            int y1 = gy + gh - 1 - (int)((s1 - 1.0f) / (SWR_MAX_DISP - 1.0f) * (gh - 2));
            y0 = constrain(y0, gy + 1, gy + gh - 1);
            y1 = constrain(y1, gy + 1, gy + gh - 1);
            spr.drawLine(gx + i - 1, y0, gx + i, y1, swrColor(s1));
        }
    }
}

// ─── Draw: PEAK Mode ──────────────────────────────────────────────────────────
void drawPeakMode() {
    int cy = CHART_TOP;
    spr.fillRect(0, cy, SCR_W, CHART_H, COL_BG);

    spr.setTextColor(COL_ORANGE, COL_BG);
    spr.drawString("PEAK HOLD", 10, cy + 8, 2);

    char buf[24];
    if (peak.valid) {
        // Peak FWD
        spr.setTextColor(COL_ORANGE, COL_BG);
        spr.drawString("PEAK FWD:", 10, cy + 30, 2);
        snprintf(buf, sizeof(buf), "%.2fW", peak.fwd_w);
        spr.setTextColor(COL_WHITE, COL_BG);
        spr.drawString(buf, 140, cy + 30, 4);

        // Peak SWR
        spr.setTextColor(COL_ORANGE, COL_BG);
        spr.drawString("PEAK SWR:", 10, cy + 80, 2);
        snprintf(buf, sizeof(buf), "%.2f", peak.swr);
        spr.setTextColor(swrColor(peak.swr), COL_BG);
        spr.drawString(buf, 140, cy + 80, 4);

        // Age
        uint32_t age_s = (millis() - peak.ts_ms) / 1000;
        snprintf(buf, sizeof(buf), "Captured %lus ago", (unsigned long)age_s);
        spr.setTextColor(COL_GRAY, COL_BG);
        spr.drawString(buf, 10, cy + 130, 2);
    } else {
        spr.setTextColor(COL_GRAY, COL_BG);
        spr.drawString("No peak captured", 10, cy + 60, 2);
        spr.drawString("Transmit to capture peak.", 10, cy + 80, 1);
    }

    // Live values for comparison
    spr.drawFastHLine(0, cy + 148, SCR_W, COL_DARKGRAY);
    spr.setTextColor(COL_CYAN, COL_BG);
    spr.drawString("LIVE:", 10, cy + 152, 1);
    if (live.valid) {
        snprintf(buf, sizeof(buf), "FWD %.2fW  SWR %.2f", live.fwd_w, live.swr);
        spr.drawString(buf, 50, cy + 152, 1);
    }
}

// ─── Draw: LOG Mode ───────────────────────────────────────────────────────────
void drawLogMode() {
    int cy = CHART_TOP;
    spr.fillRect(0, cy, SCR_W, CHART_H, COL_BG);

    spr.setTextColor(COL_CYAN, COL_BG);
    spr.drawString("DATA LOGGING", 10, cy + 8, 2);

    char buf[48];
    spr.setTextColor(COL_WHITE, COL_BG);
    snprintf(buf, sizeof(buf), "SD: %s", live.sd_log ? "LOGGING" : "STOPPED");
    spr.setTextColor(live.sd_log ? COL_GREEN : COL_GRAY, COL_BG);
    spr.drawString(buf, 10, cy + 35, 2);

    spr.setTextColor(COL_GRAY, COL_BG);
    snprintf(buf, sizeof(buf), "Records: %lu", (unsigned long)logCount);
    spr.drawString(buf, 10, cy + 60, 2);

    spr.setTextColor(COL_DARKGRAY, COL_BG);
    spr.drawString("File:", 10, cy + 85, 1);
    spr.drawString(logFile[0] ? logFile : "(none)", 50, cy + 85, 1);

    // BT streaming status
    spr.setTextColor(btConn ? COL_GREEN : COL_GRAY, COL_BG);
    snprintf(buf, sizeof(buf), "BT: %s", btConn ? "CONNECTED" : "DISCONNECTED");
    spr.drawString(buf, 10, cy + 110, 2);

    // Instructions
    spr.setTextColor(COL_DARKGRAY, COL_BG);
    spr.drawString("Hold LOG button 2s to toggle", 10, cy + 140, 1);
    spr.drawString("BT: connect to 'RF-PowerMeter'", 10, cy + 152, 1);
}

// ─── Draw: SWEEP Mode ─────────────────────────────────────────────────────────
void drawSweepMode() {
    int cy = CHART_TOP;
    int ch = CHART_H;
    spr.fillRect(0, cy, SCR_W, ch, COL_BG);

    if (sweepN < 2) {
        spr.setTextColor(COL_GRAY, COL_BG);
        spr.drawString("SWEEP MODE", 10, cy + 8, 2);
        spr.drawString("Send: SWEEP 1.8 30 0.1", 10, cy + 40, 1);
        spr.drawString("(via BT or USB serial)", 10, cy + 55, 1);
        return;
    }

    // Find freq range and SWR range
    float fMin = sweepPts[0].freq, fMax = sweepPts[sweepN-1].freq;
    float sMax = 1.0f;
    for (int i = 0; i < sweepN; i++) sMax = max(sMax, sweepPts[i].swr);
    sMax = min(sMax * 1.1f, SWR_MAX_DISP);

    // Grid
    int gx = 30, gy = cy + 10, gw = SCR_W - 40, gh = ch - 30;
    spr.drawRect(gx, gy, gw, gh, COL_GRAY);
    // Y axis: SWR 1 at bottom, sMax at top
    for (float sv : {1.5f, 2.0f, 3.0f, 5.0f}) {
        if (sv > sMax) break;
        int py = gy + gh - 1 - (int)((sv - 1.0f)/(sMax - 1.0f) * (gh - 2));
        spr.drawFastHLine(gx, py, gw, COL_DARKGRAY);
        char lbl[8]; snprintf(lbl, sizeof(lbl), "%.0f", sv);
        spr.setTextColor(COL_GRAY, COL_BG);
        spr.drawString(lbl, gx - 22, py - 4, 1);
    }
    // X axis freq ticks
    spr.setTextColor(COL_GRAY, COL_BG);
    spr.drawString(String(fMin, 0) + "M", gx, gy + gh + 4, 1);
    spr.drawString(String(fMax, 0) + "M", gx + gw - 30, gy + gh + 4, 1);

    // Plot SWR trace
    for (int i = 1; i < sweepN; i++) {
        float x0 = gx + (sweepPts[i-1].freq - fMin)/(fMax - fMin) * (gw - 2);
        float x1 = gx + (sweepPts[i  ].freq - fMin)/(fMax - fMin) * (gw - 2);
        float s0 = min(sweepPts[i-1].swr, sMax);
        float s1 = min(sweepPts[i  ].swr, sMax);
        int y0 = gy + gh - 1 - (int)((s0 - 1.0f)/(sMax - 1.0f) * (gh - 2));
        int y1 = gy + gh - 1 - (int)((s1 - 1.0f)/(sMax - 1.0f) * (gh - 2));
        y0 = constrain(y0, gy, gy + gh);
        y1 = constrain(y1, gy, gy + gh);
        spr.drawLine((int)x0, y0, (int)x1, y1, swrColor(s1));
    }
    // Label
    char buf[24];
    snprintf(buf, sizeof(buf), "%d pts  SWRmax %.2f", sweepN, sMax / 1.1f);
    spr.setTextColor(COL_CYAN, COL_BG);
    spr.drawString(buf, gx, cy + 2, 1);
}

// ─── Touch Handling ───────────────────────────────────────────────────────────
void handleTouch() {
    if (!touch.tirqTouched() || !touch.touched()) return;
    if (millis() - lastTouch < 150) return;
    lastTouch = millis();

    TS_Point p = touch.getPoint();
    // Map raw touch coords to screen (calibration values for typical CYD)
    int tx = map(p.x, 200, 3800, 0, SCR_W);
    int ty = map(p.y, 200, 3800, 0, SCR_H);

    // Top bar: mode tabs
    if (ty < TOP_BAR_H) {
        int tab = tx / 55;
        if (tab < MODE_COUNT) {
            curMode = (DispMode)tab;
            return;
        }
    }

    // Bottom bar: button zone
    if (ty > CHART_BOT) {
        if (tx < 70) {
            // RANGE button — cycle range
            btSerial.println("RANGE " + String(((live.range) % 4) + 1));
        } else if (tx < 145) {
            // PEAK RST
            btSerial.println("PEAKRST");
            peak = {};
        } else if (tx < 220) {
            // LOG toggle
            btSerial.println(live.sd_log ? "LOGOFF" : "LOGON");
        } else {
            // AUTO/MAN range
            btSerial.println("RANGE AUTO");
        }
        return;
    }

    // Chart area touches (mode-specific)
    if (curMode == MODE_LIVE) {
        // Tap left = switch display type (future expansion)
    }
}

// ─── Parse Incoming BT/JSON ───────────────────────────────────────────────────
void parseStatus(const String& json) {
    StaticJsonDocument<512> doc;
    DeserializationError err = deserializeJson(doc, json);
    if (err) return;

    live.fwd_w    = doc["fwd_w"]    | 0.0f;
    live.ref_w    = doc["ref_w"]    | 0.0f;
    live.peak_w   = doc["peak_w"]   | 0.0f;
    live.swr      = doc["swr"]      | 1.0f;
    live.rl_db    = doc["rl_db"]    | 0.0f;
    live.gamma    = doc["gamma"]    | 0.0f;
    live.freq_mhz = doc["freq_mhz"]| 0.0f;
    live.temp_c   = doc["temp_c"]   | 25.0f;
    live.sd_log   = doc["sd_log"]   | false;
    live.ts_ms    = millis();
    live.valid    = (live.fwd_w > 0.01f);
    strlcpy(live.band, doc["band"] | "HF", sizeof(live.band));
    strlcpy(live.range_label, doc["range"] | "?", sizeof(live.range_label));

    // Update peak
    if (live.valid && live.fwd_w > peak.fwd_w) peak = live;

    // Push to history
    if (swrHistN < 200) {
        swrHistory[swrHistN]  = live.swr;
        fwdHistory[swrHistN]  = live.fwd_w;
        swrHistN++;
    } else {
        memmove(swrHistory, swrHistory + 1, 199 * sizeof(float));
        memmove(fwdHistory, fwdHistory + 1, 199 * sizeof(float));
        swrHistory[199] = live.swr;
        fwdHistory[199] = live.fwd_w;
    }
}

void parseSweepLine(const String& line) {
    // CSV: freq_mhz,fwd_w,ref_w,swr
    if (line == "SWEEP_END") { return; }
    if (line.startsWith("freq")) return;  // header
    float f, fw, rw, sw;
    if (sscanf(line.c_str(), "%f,%f,%f,%f", &f, &fw, &rw, &sw) == 4) {
        if (sweepN < 200) {
            sweepPts[sweepN++] = {f, sw, fw};
        }
    }
}

// ─── Setup ────────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    Serial.println("CYD Power Display FW 1.0.0");

    // Display
    tft.init();
    tft.setRotation(1);  // Landscape
    tft.fillScreen(COL_BG);

    // Backlight
    ledcSetup(0, 5000, 8);
    ledcAttachPin(TFT_BL_PIN, 0);
    ledcWrite(0, 200);  // ~78% brightness

    // Sprite (double-buffer, full screen)
    spr.createSprite(SCR_W, SCR_H);
    spr.setTextDatum(TL_DATUM);

    // Touch
    touch.begin();
    touch.setRotation(1);

    // Bluetooth — connect to power meter
    btSerial.begin("CYD-PowerDisplay", true);  // master mode
    Serial.println("BT: scanning for RF-PowerMeter...");
    btConn = btSerial.connect("RF-PowerMeter");
    Serial.printf("BT: %s\n", btConn ? "connected" : "not found (retry in loop)");

    // Splash
    spr.fillSprite(COL_BG);
    spr.setTextColor(COL_CYAN, COL_BG);
    spr.drawString("RF POWER METER", 40, 80, 4);
    spr.setTextColor(COL_GRAY, COL_BG);
    spr.drawString("CYD Display  FW 1.0.0", 60, 130, 2);
    spr.drawString("TM-PWR-CYD-001 Rev A", 65, 155, 1);
    spr.pushSprite(0, 0);
    delay(2000);
}

// ─── Loop ─────────────────────────────────────────────────────────────────────
void loop() {
    // BT reconnect
    if (!btConn && millis() - lastPoll > 5000) {
        lastPoll = millis();
        btConn = btSerial.connect("RF-PowerMeter");
    }

    // Poll meter via BT
    if (btConn) {
        if (millis() - lastPoll > 500) {
            lastPoll = millis();
            btSerial.println("STATUS");
        }
        while (btSerial.available()) {
            String line = btSerial.readStringUntil('\n');
            line.trim();
            if (line.startsWith("{")) {
                parseStatus(line);
            } else if (line.startsWith("freq_mhz") ||
                       line.contains(",") ||
                       line == "SWEEP_END") {
                parseSweepLine(line);
                if (curMode != MODE_SWEEP) curMode = MODE_SWEEP;
            }
        }
    }

    // Touch
    handleTouch();

    // Redraw at display update rate
    if (millis() - lastDraw >= 100) {
        lastDraw = millis();
        spr.fillSprite(COL_BG);

        drawTopBar();
        drawBottomBar();

        switch (curMode) {
            case MODE_LIVE:  drawLiveMode();  break;
            case MODE_PEAK:  drawPeakMode();  break;
            case MODE_LOG:   drawLogMode();   break;
            case MODE_SWEEP: drawSweepMode(); break;
        }

        spr.pushSprite(0, 0);
    }

    delay(10);
}
