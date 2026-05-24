/**
 * cyd_nec_viewer.ino — TM-CYD-001 Rev A
 * CYD (Cheap Yellow Display) NEC Antenna Modeling Results Viewer
 *
 * Hardware: ESP32 + ILI9341 2.8" TFT 320×240 + XPT2046 touchscreen
 * CYD standard pinout:
 *   TFT: CS=15 DC=2 RST=-1 MOSI=13 SCLK=14 MISO=12  BL=21
 *   Touch: CS=33 (via TFT_eSPI Touch driver)
 *   LED: RED=4 GREEN=16 BLUE=17
 *   SD: CS=5
 *
 * Libraries (Arduino IDE):
 *   TFT_eSPI >= 2.5.43  (bodmer/TFT_eSPI)  — configure User_Setup.h for CYD
 *   ArduinoJson >= 7.0  (bblanchon/ArduinoJson)
 *   WiFi, HTTPClient    (ESP32 core)
 *
 * TFT_eSPI User_Setup.h key settings for CYD:
 *   #define ILI9341_DRIVER
 *   #define TFT_CS   15
 *   #define TFT_DC    2
 *   #define TFT_MOSI 13
 *   #define TFT_SCLK 14
 *   #define TFT_MISO 12
 *   #define TOUCH_CS 33
 *   #define SPI_FREQUENCY  40000000
 *   #define SPI_TOUCH_FREQUENCY  2500000
 *
 * Display modes (bottom row buttons):
 *   [SWEEP]   SWR + Gain vs frequency charts
 *   [PATTERN] 2D polar radiation pattern (E-plane or H-plane)
 *   [OPT]     GA/PSO optimization convergence chart
 *   [TUNE]    Live tuner SWR bargraph + control
 */

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <math.h>

// ─── WiFi Config (edit before flashing) ──────────────────────────────────────
#define WIFI_SSID        "YOUR_SSID"
#define WIFI_PASSWORD    "YOUR_PASSWORD"
#define API_HOST         "192.168.1.100"
#define API_PORT         8000
#define POLL_INTERVAL_MS 2000
#define TUNE_POLL_MS     500

// ─── CYD Pin Definitions ──────────────────────────────────────────────────────
#define TFT_BL           21
#define LED_RED           4
#define LED_GREEN        16
#define LED_BLUE         17

// ─── Display Geometry ─────────────────────────────────────────────────────────
#define SCRN_W   320
#define SCRN_H   240
#define TOPBAR_H  20
#define BOTBAR_H  40
#define CHART_Y   TOPBAR_H
#define CHART_H   (SCRN_H - TOPBAR_H - BOTBAR_H)   // 180px
#define CHART_X    0
#define CHART_W    SCRN_W                            // 320px

// ─── Colours (RGB565) ────────────────────────────────────────────────────────
#define COL_BG       TFT_BLACK
#define COL_TEXT     0xD69A    // Light gray
#define COL_ACCENT   0x07FF    // Cyan
#define COL_GREEN    TFT_GREEN
#define COL_YELLOW   TFT_YELLOW
#define COL_RED      TFT_RED
#define COL_ORANGE   0xFD20    // Orange
#define COL_GRID     0x2104    // Dark gray
#define COL_BTN      0x2945    // Button background
#define COL_BTN_ACT  0x07FF    // Active button

// ─── Display Modes ────────────────────────────────────────────────────────────
enum DisplayMode { MODE_SWEEP = 0, MODE_PATTERN, MODE_OPT, MODE_TUNE, MODE_COUNT };
DisplayMode currentMode = MODE_SWEEP;

// ─── TFT + Sprite ─────────────────────────────────────────────────────────────
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);  // Double-buffer sprite for flicker-free charts

// ─── Touch State ──────────────────────────────────────────────────────────────
struct TouchCalib { int xMin=300, xMax=3800, yMin=300, yMax=3800; } calData;
bool touchActive = false;
uint32_t lastTouchMs = 0;
#define TOUCH_DEBOUNCE_MS 150

// ─── WiFi State ───────────────────────────────────────────────────────────────
bool wifiOk = false;
String currentJobId = "";
uint32_t lastPollMs = 0;

// ─── Data Storage ─────────────────────────────────────────────────────────────
#define MAX_FREQ_POINTS 200
struct FreqPoint {
    float freq_mhz;
    float swr_50;
    float gain_dBi;
    float r_ohm, x_ohm;
};
FreqPoint freqData[MAX_FREQ_POINTS];
int nFreqPoints = 0;

#define MAX_PATTERN_POINTS 1440
struct PatternPt { float theta, phi, gain; };
PatternPt patternData[MAX_PATTERN_POINTS];
int nPatternPts = 0;

#define MAX_CONV_POINTS 300
float convData[MAX_CONV_POINTS];
int nConvPoints = 0;

// ─── Tuner State ──────────────────────────────────────────────────────────────
float tuneSWR = 0;
int tuneLTap = 0, tuneCSteps = 0;
float tuneFreqMHz = 14.225f;
bool tuneBypassed = false;
char tuneStatus[16] = "---";
uint32_t lastTunePollMs = 0;

// Pattern display plane (E or H)
bool showEplane = true;
float tunerFreqInput = 14.225f;

// Brightness (0-255)
uint8_t brightness = 200;

// ─── Setup ────────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    Serial.println("TM-CYD-001 NEC Viewer v1.0");

    // LED off
    pinMode(LED_RED,   OUTPUT); digitalWrite(LED_RED,   HIGH);
    pinMode(LED_GREEN, OUTPUT); digitalWrite(LED_GREEN, HIGH);
    pinMode(LED_BLUE,  OUTPUT); digitalWrite(LED_BLUE,  HIGH);

    // Backlight
    pinMode(TFT_BL, OUTPUT);
    analogWrite(TFT_BL, brightness);

    // TFT init
    tft.init();
    tft.setRotation(1);   // Landscape, USB on right
    tft.fillScreen(COL_BG);
    tft.setTextColor(COL_ACCENT, COL_BG);
    tft.setTextSize(2);
    tft.drawString("TM-CYD-001", 60, 90);
    tft.setTextSize(1);
    tft.drawString("NEC Antenna Viewer", 80, 115);
    tft.drawString("Connecting...", 100, 140);

    // Sprite for chart area
    spr.createSprite(CHART_W, CHART_H);
    spr.setColorDepth(16);

    initWiFi();
    drawLayout();
}

// ─── Loop ─────────────────────────────────────────────────────────────────────
void loop() {
    handleTouch();
    pollData();
    static uint32_t lastRedraw = 0;
    if (millis() - lastRedraw > 500) {
        lastRedraw = millis();
        drawCurrentMode();
    }
}

// ─── WiFi Init ────────────────────────────────────────────────────────────────
void initWiFi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    uint32_t t0 = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - t0 < 12000) {
        delay(250);
    }
    wifiOk = (WiFi.status() == WL_CONNECTED);
    Serial.printf("WiFi: %s\n", wifiOk ? WiFi.localIP().toString().c_str() : "FAILED");
    digitalWrite(LED_GREEN, wifiOk ? LOW : HIGH);
}

// ─── HTTP Helper ──────────────────────────────────────────────────────────────
String httpGet(const String& path) {
    if (!wifiOk) return "";
    HTTPClient http;
    String url = String("http://") + API_HOST + ":" + API_PORT + path;
    http.begin(url);
    http.setTimeout(5000);
    int code = http.GET();
    String body = (code == 200) ? http.getString() : "";
    http.end();
    return body;
}

// ─── Data Polling ────────────────────────────────────────────────────────────
void pollData() {
    uint32_t now = millis();

    if (currentMode == MODE_TUNE && now - lastTunePollMs > TUNE_POLL_MS) {
        lastTunePollMs = now;
        pollTunerStatus();
        return;
    }

    if (now - lastPollMs < POLL_INTERVAL_MS) return;
    lastPollMs = now;

    if (currentJobId.length() == 0) return;

    if (currentMode == MODE_SWEEP) {
        String json = httpGet("/model/results/" + currentJobId);
        if (json.length() > 0) parseFreqResults(json);
    } else if (currentMode == MODE_PATTERN) {
        String json = httpGet("/model/results/" + currentJobId +
                              "/pattern3d/" + String(tuneFreqMHz, 3));
        if (json.length() > 0) parsePattern(json);
    } else if (currentMode == MODE_OPT) {
        String json = httpGet("/model/optimize/results/" + currentJobId);
        if (json.length() > 0) parseOptResult(json);
    }
}

void pollTunerStatus() {
    String json = httpGet("/tuner/status");
    if (json.length() == 0) return;
    DynamicJsonDocument doc(512);
    if (deserializeJson(doc, json)) return;
    tuneSWR      = doc["swr"]       | 0.0f;
    tuneLTap     = doc["l_tap"]     | 0;
    tuneCSteps   = doc["c_steps"]   | 0;
    tuneFreqMHz  = doc["freq_mhz"]  | 14.225f;
    tuneBypassed = doc["bypassed"]  | false;
    strlcpy(tuneStatus, doc["status"] | "---", sizeof(tuneStatus));
}

// ─── JSON Parsers ─────────────────────────────────────────────────────────────
void parseFreqResults(const String& json) {
    DynamicJsonDocument doc(32768);
    if (deserializeJson(doc, json)) return;
    JsonArray arr = doc["freq_points"];
    nFreqPoints = 0;
    for (JsonObject pt : arr) {
        if (nFreqPoints >= MAX_FREQ_POINTS) break;
        freqData[nFreqPoints++] = {
            pt["freq_mhz"]  | 0.0f,
            pt["swr_50"]    | 0.0f,
            pt["gain_dBi"]  | 0.0f,
            pt["r_ohm"]     | 0.0f,
            pt["x_ohm"]     | 0.0f,
        };
    }
}

void parsePattern(const String& json) {
    DynamicJsonDocument doc(65536);
    if (deserializeJson(doc, json)) return;
    JsonArray arr = doc["points"];
    nPatternPts = 0;
    for (JsonObject pt : arr) {
        if (nPatternPts >= MAX_PATTERN_POINTS) break;
        patternData[nPatternPts++] = {
            pt["t"] | pt["theta"] | 0.0f,
            pt["p"] | pt["phi"]   | 0.0f,
            pt["g"] | pt["gain_dBi"] | 0.0f,
        };
    }
}

void parseOptResult(const String& json) {
    DynamicJsonDocument doc(8192);
    if (deserializeJson(doc, json)) return;
    JsonArray arr = doc["convergence"];
    nConvPoints = 0;
    for (float v : arr) {
        if (nConvPoints >= MAX_CONV_POINTS) break;
        convData[nConvPoints++] = v;
    }
}

// ─── Layout ───────────────────────────────────────────────────────────────────
void drawLayout() {
    tft.fillScreen(COL_BG);
    drawTopBar();
    drawBottomButtons();
}

void drawTopBar() {
    tft.fillRect(0, 0, SCRN_W, TOPBAR_H, 0x1082);  // Dark blue-gray
    tft.setTextColor(COL_ACCENT, 0x1082);
    tft.setTextSize(1);
    const char* modeNames[] = {"SWEEP","PATTERN","OPTIMIZE","TUNE"};
    tft.drawString(String("TM-CYD-001  [") + modeNames[currentMode] + "]", 4, 6);
    // WiFi indicator
    tft.setTextColor(wifiOk ? COL_GREEN : COL_RED, 0x1082);
    tft.drawString(wifiOk ? "WiFi" : "NoWF", 270, 6);
}

void drawBottomButtons() {
    const char* labels[] = {"SWEEP", "PATTERN", "OPT", "TUNE"};
    int bw = SCRN_W / 4;
    for (int i = 0; i < 4; i++) {
        uint16_t bg = (i == (int)currentMode) ? COL_BTN_ACT : COL_BTN;
        uint16_t fg = (i == (int)currentMode) ? TFT_BLACK    : COL_TEXT;
        tft.fillRect(i * bw, SCRN_H - BOTBAR_H, bw - 2, BOTBAR_H - 1, bg);
        tft.setTextColor(fg, bg);
        tft.setTextSize(1);
        int lx = i * bw + (bw - strlen(labels[i]) * 6) / 2;
        tft.drawString(labels[i], lx, SCRN_H - BOTBAR_H + 14);
    }
}

// ─── Mode Drawing ─────────────────────────────────────────────────────────────
void drawCurrentMode() {
    drawTopBar();
    switch (currentMode) {
        case MODE_SWEEP:   drawSweepMode();   break;
        case MODE_PATTERN: drawPatternMode(); break;
        case MODE_OPT:     drawOptMode();     break;
        case MODE_TUNE:    drawTuneMode();    break;
    }
}

// ── MODE 1: Sweep — SWR + Gain charts ────────────────────────────────────────
void drawSweepMode() {
    spr.fillSprite(COL_BG);
    if (nFreqPoints < 2) {
        spr.setTextColor(COL_TEXT, COL_BG);
        spr.drawString("No data. Submit job via API.", 20, CHART_H/2 - 10);
        spr.drawString("GET /model/run  to start.", 20, CHART_H/2 + 4);
        spr.pushSprite(0, CHART_Y);
        return;
    }

    int halfH = CHART_H / 2;

    // --- Upper chart: SWR ---
    drawChartGrid(spr, 0, 0, CHART_W, halfH - 2, 1.0f, 5.0f, "SWR");
    float fMin = freqData[0].freq_mhz, fMax = freqData[nFreqPoints-1].freq_mhz;
    float fRange = max(fMax - fMin, 0.001f);

    for (int i = 1; i < nFreqPoints; i++) {
        float x0 = map_f(freqData[i-1].freq_mhz, fMin, fMax, 0, CHART_W-1);
        float x1 = map_f(freqData[i].freq_mhz,   fMin, fMax, 0, CHART_W-1);
        float y0 = map_f(freqData[i-1].swr_50,   1.0f, 5.0f, halfH-4, 2);
        float y1 = map_f(freqData[i].swr_50,      1.0f, 5.0f, halfH-4, 2);
        // Color: green < 2, yellow 2-3, red > 3
        uint16_t c = (freqData[i].swr_50 < 2.0f) ? COL_GREEN :
                     (freqData[i].swr_50 < 3.0f) ? COL_YELLOW : COL_RED;
        spr.drawLine(x0, y0, x1, y1, c);
    }
    // 2:1 SWR reference line
    int y2 = map_f(2.0f, 1.0f, 5.0f, halfH-4, 2);
    spr.drawFastHLine(0, y2, CHART_W, COL_ORANGE);

    // --- Lower chart: Gain ---
    float gMin = -10, gMax = 15;
    for (int i = 0; i < nFreqPoints; i++) {
        if (freqData[i].gain_dBi < gMin) gMin = freqData[i].gain_dBi - 1;
        if (freqData[i].gain_dBi > gMax) gMax = freqData[i].gain_dBi + 1;
    }
    drawChartGrid(spr, 0, halfH+2, CHART_W, halfH-4, gMin, gMax, "Gain dBi");
    for (int i = 1; i < nFreqPoints; i++) {
        float x0 = map_f(freqData[i-1].freq_mhz, fMin, fMax, 0, CHART_W-1);
        float x1 = map_f(freqData[i].freq_mhz,   fMin, fMax, 0, CHART_W-1);
        float y0 = map_f(freqData[i-1].gain_dBi, gMin, gMax, halfH+halfH-6, halfH+2);
        float y1 = map_f(freqData[i].gain_dBi,   gMin, gMax, halfH+halfH-6, halfH+2);
        spr.drawLine(x0, y0, x1, y1, COL_ORANGE);
    }

    // Freq labels
    spr.setTextColor(COL_TEXT, COL_BG);
    spr.setTextSize(1);
    spr.drawString(String(fMin,3)+" MHz", 2, CHART_H-14);
    String fMidStr = String((fMin+fMax)/2, 3)+" MHz";
    spr.drawString(fMidStr, CHART_W/2 - fMidStr.length()*3, CHART_H-14);
    spr.drawString(String(fMax,3)+" MHz", CHART_W-60, CHART_H-14);

    spr.pushSprite(0, CHART_Y);
}

// ── MODE 2: 2D Polar Pattern ─────────────────────────────────────────────────
void drawPatternMode() {
    spr.fillSprite(COL_BG);
    if (nPatternPts < 10) {
        spr.setTextColor(COL_TEXT, COL_BG);
        spr.drawString("No pattern data.", 80, CHART_H/2);
        spr.drawString(showEplane ? "E-plane" : "H-plane", 120, CHART_H/2 + 16);
        spr.pushSprite(0, CHART_Y);
        return;
    }

    int cx = CHART_W / 2;
    int cy = CHART_H / 2;
    int r_max = min(cx, cy) - 20;

    // Find max gain
    float gMax = -99, gMin = 99;
    for (int i = 0; i < nPatternPts; i++) {
        if (patternData[i].gain > gMax) gMax = patternData[i].gain;
        if (patternData[i].gain < gMin) gMin = patternData[i].gain;
    }

    // Draw polar grid circles at -3, -10, -20 dB from max
    for (int db : {0, -3, -10, -20}) {
        float norm = (gMax + db - gMin) / max(gMax - gMin, 0.01f);
        norm = constrain(norm, 0.05f, 1.0f);
        int r = (int)(norm * r_max);
        spr.drawCircle(cx, cy, r, COL_GRID);
        spr.setTextColor(COL_GRID, COL_BG);
        spr.drawString(String(db)+"dB", cx + r + 2, cy - 6);
    }
    // Azimuth lines
    for (int deg = 0; deg < 360; deg += 45) {
        float rad = deg * DEG_TO_RAD;
        spr.drawLine(cx, cy,
                     cx + (int)(r_max * sin(rad)),
                     cy - (int)(r_max * cos(rad)), COL_GRID);
    }
    // Cardinal labels
    spr.setTextColor(COL_ACCENT, COL_BG);
    spr.drawString("N", cx-4, cy-r_max-14);
    spr.drawString("S", cx-4, cy+r_max+2);
    spr.drawString("E", cx+r_max+2, cy-6);
    spr.drawString("W", cx-r_max-14, cy-6);

    // Plot pattern (E-plane: vary theta at phi=0°/180°; H-plane: vary phi at theta=90°)
    int prevX = -1, prevY = -1;
    float fixedAngle = showEplane ? 0.0f : 90.0f;

    for (int i = 0; i < nPatternPts; i++) {
        PatternPt& p = patternData[i];
        float angle, sweep;
        if (showEplane) {
            if (abs(p.phi) > 5 && abs(p.phi - 180) > 5) continue;
            sweep = p.theta;
            angle = (p.phi < 90) ? sweep : 180 + sweep;
        } else {
            if (abs(p.theta - 90) > 5) continue;
            sweep = p.phi;
            angle = sweep;
        }

        float norm = (p.gain - gMin) / max(gMax - gMin, 0.01f);
        norm = constrain(norm * 0.95f + 0.05f, 0.05f, 1.0f);
        int r = (int)(norm * r_max);
        float rad = angle * DEG_TO_RAD;
        int px = cx + (int)(r * sin(rad));
        int py = cy - (int)(r * cos(rad));

        uint16_t col = (p.gain > gMax - 3) ? COL_GREEN :
                       (p.gain > gMax - 10) ? COL_YELLOW : COL_TEXT;
        if (prevX >= 0) spr.drawLine(prevX, prevY, px, py, col);
        prevX = px; prevY = py;
    }

    // Info
    spr.setTextColor(COL_ACCENT, COL_BG);
    spr.setTextSize(1);
    spr.drawString(showEplane ? "E-PLANE" : "H-PLANE", 4, 4);
    spr.drawString("Tap to toggle", 200, 4);
    spr.drawString("Max: " + String(gMax,1) + " dBi", 4, CHART_H - 14);

    spr.pushSprite(0, CHART_Y);
}

// ── MODE 3: Optimization Convergence ─────────────────────────────────────────
void drawOptMode() {
    spr.fillSprite(COL_BG);
    if (nConvPoints < 2) {
        spr.setTextColor(COL_TEXT, COL_BG);
        spr.drawString("No optimization data.", 60, CHART_H/2);
        spr.drawString("POST /model/optimize to start", 30, CHART_H/2+16);
        spr.pushSprite(0, CHART_Y);
        return;
    }

    float cMin = convData[0], cMax = convData[0];
    for (int i = 1; i < nConvPoints; i++) {
        if (convData[i] < cMin) cMin = convData[i];
        if (convData[i] > cMax) cMax = convData[i];
    }
    float cRange = max(cMax - cMin, 0.001f);

    drawChartGrid(spr, 0, 0, CHART_W, CHART_H-20, cMin, cMax, "Fitness");

    for (int i = 1; i < nConvPoints; i++) {
        int x0 = map(i-1, 0, nConvPoints-1, 0, CHART_W-1);
        int x1 = map(i,   0, nConvPoints-1, 0, CHART_W-1);
        int y0 = map_f(convData[i-1], cMin, cMax, CHART_H-22, 4);
        int y1 = map_f(convData[i],   cMin, cMax, CHART_H-22, 4);
        spr.drawLine(x0, y0, x1, y1, COL_GREEN);
    }

    spr.setTextColor(COL_ACCENT, COL_BG);
    spr.drawString("Best: " + String(cMax, 3), 4, 4);
    spr.drawString(String(nConvPoints) + " iterations", 200, 4);
    spr.pushSprite(0, CHART_Y);
}

// ── MODE 4: Live Tuner Status ─────────────────────────────────────────────────
void drawTuneMode() {
    spr.fillSprite(COL_BG);

    // Big SWR bargraph (top half)
    float swr_display = constrain(tuneSWR, 1.0f, 5.0f);
    int barW = (int)((swr_display - 1.0f) / 4.0f * (CHART_W - 40));
    uint16_t barCol = (tuneSWR < 1.5f) ? COL_GREEN :
                      (tuneSWR < 3.0f) ? COL_YELLOW : COL_RED;

    spr.drawRect(20, 10, CHART_W-40, 30, COL_TEXT);
    spr.fillRect(21, 11, barW, 28, barCol);
    spr.setTextColor(TFT_WHITE, COL_BG);
    spr.setTextSize(2);
    spr.drawString("SWR " + String(tuneSWR, 2), 90, 48);
    spr.setTextSize(1);
    spr.drawString("1.0", 20, 42); spr.drawString("5.0", CHART_W-36, 42);

    // Status and settings
    spr.setTextColor(COL_ACCENT, COL_BG);
    spr.drawString("STATUS: " + String(tuneStatus), 10, 72);
    spr.drawString("Freq:  " + String(tuneFreqMHz, 3) + " MHz", 10, 86);
    spr.drawString("L-tap: " + String(tuneLTap) + "/7", 10, 100);
    spr.drawString("C-pos: " + String(tuneCSteps) + "/200", 10, 114);
    spr.drawString(tuneBypassed ? "BYPASS: ENGAGED" : "BYPASS: off", 10, 128);

    // Tune button
    uint16_t btnC = COL_BTN;
    spr.fillRoundRect(CHART_W-110, 70, 100, 36, 6, btnC);
    spr.setTextColor(COL_ACCENT, btnC);
    spr.setTextSize(1);
    spr.drawString("TUNE NOW", CHART_W-92, 84);

    // Freq +/- buttons
    spr.fillRect(CHART_W-110, 115, 45, 28, COL_BTN);
    spr.fillRect(CHART_W-60,  115, 45, 28, COL_BTN);
    spr.setTextColor(COL_TEXT, COL_BTN);
    spr.setTextSize(2);
    spr.drawString("-", CHART_W-93, 120);
    spr.drawString("+", CHART_W-47, 120);

    spr.pushSprite(0, CHART_Y);
}

// ─── Chart Grid Helper ────────────────────────────────────────────────────────
void drawChartGrid(TFT_eSprite& sp, int x0, int y0, int w, int h,
                   float vMin, float vMax, const char* label) {
    sp.drawRect(x0, y0, w, h, COL_GRID);
    // 4 horizontal grid lines
    for (int i = 1; i <= 3; i++) {
        int y = y0 + (h * i / 4);
        sp.drawFastHLine(x0, y, w, COL_GRID);
    }
    sp.setTextColor(COL_GRID, COL_BG);
    sp.setTextSize(1);
    sp.drawString(label, x0+2, y0+2);
    sp.drawString(String(vMax,1), x0+2, y0+12);
    sp.drawString(String(vMin,1), x0+2, y0+h-12);
}

// ─── Touch Handling ───────────────────────────────────────────────────────────
void handleTouch() {
    uint16_t tx, ty;
    bool pressed = tft.getTouch(&tx, &ty);
    uint32_t now = millis();

    if (pressed && !touchActive && (now - lastTouchMs > TOUCH_DEBOUNCE_MS)) {
        touchActive = true;
        lastTouchMs = now;
        onTap(tx, ty);
    } else if (!pressed) {
        touchActive = false;
    }
}

void onTap(int tx, int ty) {
    // Bottom button row
    if (ty >= SCRN_H - BOTBAR_H) {
        int btn = tx / (SCRN_W / 4);
        btn = constrain(btn, 0, MODE_COUNT - 1);
        if (btn != (int)currentMode) {
            currentMode = (DisplayMode)btn;
            drawLayout();
            drawCurrentMode();
        }
        return;
    }

    // Mode-specific taps
    if (currentMode == MODE_PATTERN) {
        // Toggle E/H plane
        showEplane = !showEplane;
        drawCurrentMode();
    }
    else if (currentMode == MODE_TUNE) {
        // TUNE NOW button area
        if (tx > SCRN_W-115 && tx < SCRN_W-10 && ty > CHART_Y+70 && ty < CHART_Y+110) {
            sendTuneCommand(tuneFreqMHz);
        }
        // Freq decrement
        if (tx > SCRN_W-115 && tx < SCRN_W-67 && ty > CHART_Y+115 && ty < CHART_Y+145) {
            tuneFreqMHz = max(1.8f, tuneFreqMHz - 0.025f);
        }
        // Freq increment
        if (tx > SCRN_W-63 && tx < SCRN_W-15 && ty > CHART_Y+115 && ty < CHART_Y+145) {
            tuneFreqMHz = min(30.0f, tuneFreqMHz + 0.025f);
        }
    }
    // Long-press top bar → brightness
    if (ty < TOPBAR_H) {
        brightness = (brightness < 100) ? 200 : 80;
        analogWrite(TFT_BL, brightness);
    }
}

// ─── Send Tune Command to Tuner API ──────────────────────────────────────────
void sendTuneCommand(float freq_mhz) {
    if (!wifiOk) return;
    HTTPClient http;
    String url = String("http://") + API_HOST + ":" + API_PORT + "/tuner/tune";
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    String body = "{\"freq_mhz\":" + String(freq_mhz, 3) + "}";
    int code = http.POST(body);
    Serial.printf("TuneCmd: %.3f MHz → HTTP %d\n", freq_mhz, code);
    http.end();
}

// ─── Map float to pixel ───────────────────────────────────────────────────────
int map_f(float v, float inMin, float inMax, int outMin, int outMax) {
    if (inMax == inMin) return (outMin + outMax) / 2;
    float norm = (v - inMin) / (inMax - inMin);
    return outMin + (int)(norm * (outMax - outMin));
}
