// =============================================================================
// cyd_tuner_display.ino — CYD (Cheap Yellow Display) for RF-HFTuner
// Project:  RF-HFTuner / Motorized Antenna Tuner Suite
// Device:   ESP32-2432S028 (CYD) with 2.8" TFT + XPT2046 touchscreen
// Rev:      1.0  2026-05-01
//
// Connects to "RF-HFTuner" via Bluetooth SPP.
// Displays: SWR bargraph, L/C readout, band grid, network diagram, status.
//
// Libraries required:
//   TFT_eSPI         >= 2.5.34   (Bodmer) — configure for ILI9341, CYD pins
//   XPT2046_Touchscreen >= 1.4.0 (Paul Stoffregen)
//   BluetoothSerial  (ESP32 built-in)
//   ArduinoJson      >= 6.21.0
//
// TFT_eSPI User_Setup.h settings for CYD (ESP32-2432S028):
//   #define ILI9341_DRIVER
//   #define TFT_MOSI 13
//   #define TFT_SCLK 14
//   #define TFT_CS   15
//   #define TFT_DC    2
//   #define TFT_RST  -1
//   #define TFT_BL   21   (backlight, optional)
//   #define SPI_FREQUENCY  40000000
//   #define SPI_READ_FREQUENCY 20000000
//   #define SPI_TOUCH_FREQUENCY 2500000
//   #define TOUCH_CS 33
// =============================================================================

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <math.h>

// =============================================================================
// HARDWARE PINS (CYD ESP32-2432S028)
// =============================================================================

#define CYD_TOUCH_CS   33
#define CYD_TOUCH_IRQ  36    // T_IRQ, optional
#define CYD_LED_R       4
#define CYD_LED_G      16
#define CYD_LED_B      17
#define CYD_BACKLIGHT  21

// =============================================================================
// DISPLAY CONSTANTS
// =============================================================================

#define SCREEN_W       320
#define SCREEN_H       240
#define TAB_H           30
#define TAB_COUNT        4
#define BTN_BAR_H       36
#define CONTENT_Y      (TAB_H)
#define CONTENT_H      (SCREEN_H - TAB_H - BTN_BAR_H)

// Color palette (RGB565)
#define C_BG           0x1082   // Dark navy #101010
#define C_CARD         0x0841   // Darker card #082028
#define C_BORDER       0x07BF   // Cyan border #007878
#define C_ACCENT       0x0676   // Cyan accent #00D4FF
#define C_TEXT         0xFFFF   // White
#define C_DIM          0x8410   // Gray
#define C_GREEN        0x07E0   // Pure green
#define C_GREEN_SWR    0x04F2   // SWR-good green #009090
#define C_YELLOW       0xFFE0   // Yellow
#define C_RED          0xF800   // Red
#define C_ORANGE       0xFC00   // Orange
#define C_BAND_160     0x780F   // Dark red
#define C_BAND_80      0x7800   // Red
#define C_BAND_40      0xFC00   // Orange
#define C_BAND_20      0xFFE0   // Yellow
#define C_BAND_10      0x07E0   // Green
#define C_BAND_6       0x001F   // Blue
#define C_TAB_ACTIVE   0x0676
#define C_TAB_INACTIVE 0x2945
#define C_BTN_TUNE     0x0676
#define C_BTN_HOME     0xFFE0
#define C_BTN_STOP     0xF800
#define C_BTN_SAVE     0x07E0
#define C_BTN_BAND     0x2945

// =============================================================================
// DISPLAY MODES (tabs)
// =============================================================================

typedef enum {
    MODE_TUNE   = 0,   // Live SWR bargraph + L/C/band readout
    MODE_BANDS  = 1,   // 11-band grid, tap to tune
    MODE_MATCH  = 2,   // Impedance/network topology diagram
    MODE_WIFI   = 3,   // WiFi IP, device info
} DisplayMode;

// =============================================================================
// GLOBAL OBJECTS
// =============================================================================

TFT_eSPI       tft = TFT_eSPI();
TFT_eSprite    spr_swr = TFT_eSprite(&tft);     // SWR meter sprite (partial update)
TFT_eSprite    spr_lc  = TFT_eSprite(&tft);     // L/C values sprite
XPT2046_Touchscreen touch(CYD_TOUCH_CS, CYD_TOUCH_IRQ);
BluetoothSerial SerialBT;

// =============================================================================
// STATE
// =============================================================================

DisplayMode currentMode = MODE_TUNE;
DisplayMode lastMode    = (DisplayMode)-1;

// Data received from RF-HFTuner
struct TunerState {
    float       swr         = 99.0f;
    float       fwd_w       = 0.0f;
    float       rev_w       = 0.0f;
    float       return_loss = 0.0f;
    float       gamma       = 1.0f;
    float       L_uh        = 0.0f;
    float       C_pf        = 0.0f;
    int         band_index  = 3;      // default 40m
    char        band_name[8]= "40m";
    char        network[16] = "L_LOWPASS";
    char        state[16]   = "IDLE";
    bool        is_tuning   = false;
    bool        homed       = false;
    uint32_t    last_update = 0;
    char        wifi_ip[20] = "192.168.4.1";
} ts;

// Band name list
const char* BAND_NAMES[11] = {
    "160m","80m","60m","40m","30m","20m","17m","15m","12m","10m","6m"
};

// Band colors (RGB565)
const uint16_t BAND_COLORS[11] = {
    0x780F, 0x7800, 0xFBE0, 0xFC00, 0xFDE0,
    0xFFE0, 0xC7E0, 0x07E0, 0x07FF, 0x001F, 0x781F
};

// BT state
bool    btConnected  = false;
String  btBuffer     = "";
uint32_t lastBTSend  = 0;
uint32_t lastSWRDraw = 0;
uint32_t lastFullRedraw = 0;

// Touch state
bool    touchActive  = false;
int     touchX = 0, touchY = 0;
uint32_t lastTouch   = 0;

// Animation (tuning arc rotation)
int     animAngle    = 0;
uint32_t lastAnim    = 0;

// Button pressed tracking
int     pressedBtn   = -1;

// =============================================================================
// SETUP
// =============================================================================

void setup() {
    Serial.begin(115200);
    Serial.println("CYD Tuner Display v1.0");

    // --- RGB LED ---
    pinMode(CYD_LED_R, OUTPUT);
    pinMode(CYD_LED_G, OUTPUT);
    pinMode(CYD_LED_B, OUTPUT);
    setLED(0, 0, 255);  // Blue = initializing

    // --- Backlight ---
    pinMode(CYD_BACKLIGHT, OUTPUT);
    digitalWrite(CYD_BACKLIGHT, HIGH);

    // --- TFT ---
    tft.init();
    tft.setRotation(1);  // Landscape
    tft.fillScreen(C_BG);
    tft.setTextColor(C_TEXT, C_BG);
    tft.setTextSize(2);
    tft.setCursor(60, 100);
    tft.println("RF-HFTuner");
    tft.setTextSize(1);
    tft.setCursor(80, 125);
    tft.println("CYD Display v1.0");
    tft.setCursor(70, 145);
    tft.println("Connecting BT...");

    // --- Create sprites ---
    spr_swr.createSprite(300, 70);  // SWR meter area
    spr_lc.createSprite(300, 30);   // L/C values bar

    // --- Touch ---
    touch.begin();
    touch.setRotation(1);

    // --- Bluetooth ---
    SerialBT.begin("CYD-Tuner", true);  // true = master mode
    Serial.println("Attempting BT connection to RF-HFTuner...");
    tft.fillScreen(C_BG);

    // Try to connect (non-blocking loop with timeout)
    uint32_t t0 = millis();
    while (!SerialBT.connected() && millis() - t0 < 8000) {
        SerialBT.connect("RF-HFTuner");
        delay(500);
        tft.setCursor(60, 115);
        tft.setTextSize(1);
        tft.setTextColor(C_ACCENT, C_BG);
        tft.printf("BT connecting... %lus", (millis()-t0)/1000);
    }
    btConnected = SerialBT.connected();

    if (btConnected) {
        setLED(0, 255, 0);  // Green = connected
        Serial.println("BT connected.");
        SerialBT.println("STATUS");  // Request initial status
    } else {
        setLED(255, 165, 0);  // Orange = not connected
        Serial.println("BT connection failed; will retry.");
    }

    // --- Draw initial UI ---
    drawFullUI();
}

// =============================================================================
// LOOP
// =============================================================================

void loop() {
    uint32_t now = millis();

    // --- BT reconnect ---
    if (!btConnected && now - lastBTSend > 5000) {
        SerialBT.connect("RF-HFTuner");
        lastBTSend = now;
        if (SerialBT.connected()) {
            btConnected = true;
            setLED(0, 255, 0);
            SerialBT.println("STATUS");
        }
    }
    if (btConnected && !SerialBT.connected()) {
        btConnected = false;
        setLED(255, 0, 0);
        Serial.println("BT disconnected.");
    }

    // --- Receive BT data ---
    while (SerialBT.available()) {
        char c = (char)SerialBT.read();
        if (c == '\n') {
            processResponse(btBuffer);
            btBuffer = "";
        } else if (c != '\r') {
            btBuffer += c;
        }
    }

    // --- Request status periodically ---
    if (btConnected && now - lastBTSend >= 500) {
        SerialBT.println("STATUS");
        lastBTSend = now;
    }

    // --- Touch ---
    handleTouch();

    // --- SWR partial update (every 500ms) ---
    if (currentMode == MODE_TUNE && now - lastSWRDraw >= 500) {
        lastSWRDraw = now;
        drawSWRPartial();
    }

    // --- Tuning animation ---
    if (ts.is_tuning && now - lastAnim >= 40) {
        lastAnim = now;
        animAngle = (animAngle + 10) % 360;
        if (currentMode == MODE_TUNE) drawTuningAnim();
    }

    // --- Full redraw on mode change ---
    if (currentMode != lastMode) {
        lastMode = currentMode;
        drawFullUI();
    }

    // --- RGB LED update per SWR ---
    updateLED();

    delay(10);
}

// =============================================================================
// BT RESPONSE PROCESSING
// =============================================================================

void processResponse(const String& line) {
    if (line.length() == 0) return;
    // Try to parse as JSON STATUS response
    if (line.startsWith("{")) {
        StaticJsonDocument<512> doc;
        DeserializationError err = deserializeJson(doc, line);
        if (!err) {
            ts.swr         = doc["swr"]         | 99.0f;
            ts.fwd_w       = doc["fwd_w"]        | 0.0f;
            ts.rev_w       = doc["rev_w"]        | 0.0f;
            ts.return_loss = doc["return_loss"]  | 0.0f;
            ts.gamma       = doc["gamma"]        | 1.0f;
            ts.L_uh        = doc["L_uh"]         | 0.0f;
            ts.C_pf        = doc["C_pf"]         | 0.0f;
            ts.band_index  = doc["band_index"]   | 3;
            ts.is_tuning   = doc["is_tuning"]    | false;
            ts.homed       = doc["homed"]        | false;
            strlcpy(ts.band_name, doc["band_name"] | "??",  sizeof(ts.band_name));
            strlcpy(ts.network,   doc["network"]   | "??",  sizeof(ts.network));
            strlcpy(ts.state,     doc["state"]     | "??",  sizeof(ts.state));
            ts.last_update = millis();
        }
    }
}

// =============================================================================
// TOUCH HANDLING
// =============================================================================

void handleTouch() {
    if (!touch.tirqTouched() && !touch.touched()) return;
    if (!touch.touched()) return;

    TS_Point p = touch.getPoint();
    // Map raw to screen (calibration values typical for CYD; adjust per unit)
    int x = map(p.x, 200, 3900, 0, SCREEN_W);
    int y = map(p.y, 240, 3800, 0, SCREEN_H);
    x = constrain(x, 0, SCREEN_W - 1);
    y = constrain(y, 0, SCREEN_H - 1);

    uint32_t now = millis();
    if (now - lastTouch < 200) return;  // debounce
    lastTouch = now;

    touchX = x; touchY = y;

    // --- Tab bar (top) ---
    if (y < TAB_H) {
        int tabW = SCREEN_W / TAB_COUNT;
        int tab = x / tabW;
        if (tab >= 0 && tab < TAB_COUNT) {
            currentMode = (DisplayMode)tab;
        }
        return;
    }

    // --- Button bar (bottom) ---
    if (y >= SCREEN_H - BTN_BAR_H) {
        handleButtonBar(x, y);
        return;
    }

    // --- Content area ---
    if (currentMode == MODE_BANDS) {
        handleBandGridTouch(x, y);
    }
}

void handleButtonBar(int x, int y) {
    // 5 buttons: TUNE, HOME, STOP, SAVE, BAND+
    int btnW = SCREEN_W / 5;
    int btn  = x / btnW;

    switch (btn) {
        case 0: // TUNE
            if (btConnected) {
                char cmd[32];
                snprintf(cmd, sizeof(cmd), "TUNE %d", ts.band_index);
                SerialBT.println(cmd);
            }
            break;
        case 1: // HOME
            if (btConnected) SerialBT.println("HOME");
            break;
        case 2: // STOP
            if (btConnected) SerialBT.println("STOP");
            break;
        case 3: // SAVE
            if (btConnected) {
                char cmd[32];
                snprintf(cmd, sizeof(cmd), "SAVE %d", ts.band_index);
                SerialBT.println(cmd);
            }
            break;
        case 4: // BAND+
            {
                int nextBand = (ts.band_index + 1) % 11;
                if (btConnected) {
                    char cmd[32];
                    snprintf(cmd, sizeof(cmd), "BAND %d", nextBand);
                    SerialBT.println(cmd);
                }
            }
            break;
    }
    // Visual flash feedback on pressed button
    pressedBtn = btn;
    drawButtonBar();
    delay(80);
    pressedBtn = -1;
    drawButtonBar();
}

void handleBandGridTouch(int x, int y) {
    // 11-band grid: 4 columns × 3 rows (last row has 3 bands)
    int cols = 4;
    int cellW = SCREEN_W / cols;
    int cellH = (CONTENT_H - 10) / 3;
    int row = (y - CONTENT_Y) / cellH;
    int col = x / cellW;
    int band = row * cols + col;
    if (band >= 0 && band < 11) {
        // Select band and request auto-tune
        char cmd[32];
        snprintf(cmd, sizeof(cmd), "BAND %d", band);
        if (btConnected) SerialBT.println(cmd);
        // Flash band cell
        int bx = col * cellW + 2;
        int by = CONTENT_Y + row * cellH + 2;
        tft.fillRect(bx, by, cellW-4, cellH-4, C_ACCENT);
        delay(80);
        drawBandGrid();
    }
}

// =============================================================================
// FULL UI DRAW
// =============================================================================

void drawFullUI() {
    tft.fillScreen(C_BG);
    drawTabBar();
    drawButtonBar();
    switch (currentMode) {
        case MODE_TUNE:  drawTuneMode();  break;
        case MODE_BANDS: drawBandsMode(); break;
        case MODE_MATCH: drawMatchMode(); break;
        case MODE_WIFI:  drawWifiMode();  break;
    }
}

// =============================================================================
// TAB BAR
// =============================================================================

void drawTabBar() {
    const char* tabs[4] = { "TUNE", "BANDS", "MATCH", "WiFi" };
    int tabW = SCREEN_W / TAB_COUNT;
    for (int i = 0; i < TAB_COUNT; i++) {
        uint16_t bg = (i == (int)currentMode) ? C_TAB_ACTIVE : C_TAB_INACTIVE;
        uint16_t fg = (i == (int)currentMode) ? TFT_BLACK : C_TEXT;
        tft.fillRect(i * tabW, 0, tabW - 1, TAB_H - 2, bg);
        tft.drawRect(i * tabW, 0, tabW - 1, TAB_H - 2, C_BORDER);
        tft.setTextColor(fg, bg);
        tft.setTextSize(1);
        int tx = i * tabW + (tabW - strlen(tabs[i]) * 6) / 2;
        tft.setCursor(tx, (TAB_H - 8) / 2);
        tft.print(tabs[i]);
    }
}

// =============================================================================
// BUTTON BAR
// =============================================================================

void drawButtonBar() {
    const char* btnLabels[5] = { "TUNE", "HOME", "STOP", "SAVE", "BND+" };
    const uint16_t btnColors[5] = { C_BTN_TUNE, C_BTN_HOME, C_BTN_STOP, C_BTN_SAVE, C_BTN_BAND };
    int btnW = SCREEN_W / 5;
    int y0 = SCREEN_H - BTN_BAR_H;
    for (int i = 0; i < 5; i++) {
        uint16_t bg = (i == pressedBtn) ? TFT_WHITE : btnColors[i];
        uint16_t fg = (i == pressedBtn) ? TFT_BLACK : TFT_BLACK;
        tft.fillRect(i * btnW, y0, btnW - 1, BTN_BAR_H - 1, bg);
        tft.drawRect(i * btnW, y0, btnW - 1, BTN_BAR_H - 1, C_BORDER);
        tft.setTextColor(fg, bg);
        tft.setTextSize(1);
        int tx = i * btnW + (btnW - strlen(btnLabels[i]) * 6) / 2;
        tft.setCursor(tx, y0 + (BTN_BAR_H - 8) / 2);
        tft.print(btnLabels[i]);
    }
}

// =============================================================================
// MODE: TUNE — SWR gauge, bargraph, L/C readout
// =============================================================================

void drawTuneMode() {
    int y = CONTENT_Y + 4;

    // Band + state header
    tft.fillRect(0, y, SCREEN_W, 14, C_CARD);
    tft.setTextColor(C_ACCENT, C_CARD);
    tft.setTextSize(1);
    tft.setCursor(4, y + 3);
    tft.printf("Band: %-5s  Net: %-12s  %s",
               ts.band_name, ts.network, ts.state);
    y += 16;

    // Analog SWR gauge
    drawSWRGauge(160, y + 42, 58, ts.swr);
    y += 90;

    // SWR bargraph
    drawSWRBarGraph(4, y, SCREEN_W - 8, 18, ts.swr);
    y += 24;

    // L/C/power row
    tft.fillRect(0, y, SCREEN_W, 20, C_CARD);
    tft.setTextColor(C_TEXT, C_CARD);
    tft.setTextSize(1);
    tft.setCursor(4, y + 6);
    tft.printf("L: %6.2fuH  C: %5.0fpF  Fwd:%5.1fW  Rev:%4.1fW",
               ts.L_uh, ts.C_pf, ts.fwd_w, ts.rev_w);
    y += 22;

    // Return loss / gamma
    tft.fillRect(0, y, SCREEN_W, 14, C_CARD);
    tft.setTextColor(C_DIM, C_CARD);
    tft.setTextSize(1);
    tft.setCursor(4, y + 3);
    tft.printf("RL: %5.1f dB   Gamma: %.3f   Rfl: %.1f%%",
               ts.return_loss, ts.gamma, ts.gamma * ts.gamma * 100.0f);
}

// Partial update: redraw only the SWR section without full screen wipe
void drawSWRPartial() {
    int y = CONTENT_Y + 4 + 16;
    drawSWRGauge(160, y + 42, 58, ts.swr);
    drawSWRBarGraph(4, y + 90, SCREEN_W - 8, 18, ts.swr);

    // Update L/C line in sprite
    spr_lc.fillSprite(C_CARD);
    spr_lc.setTextColor(C_TEXT, C_CARD);
    spr_lc.setTextSize(1);
    spr_lc.setCursor(0, 6);
    spr_lc.printf("L:%6.2fuH C:%5.0fpF Fwd:%5.1fW Rev:%4.1fW",
                  ts.L_uh, ts.C_pf, ts.fwd_w, ts.rev_w);
    spr_lc.pushSprite(0, CONTENT_Y + 4 + 16 + 90 + 24);
}

// Analog SWR meter gauge (arc + needle)
// cx, cy: center; r: radius; swr: value
void drawSWRGauge(int cx, int cy, int r, float swr) {
    // Background arc: 180 degrees, from 180° to 0° (left to right)
    // Draw graduated arc segments
    for (int a = 0; a <= 180; a += 2) {
        float rad = (a + 180) * DEG_TO_RAD;
        int x0 = cx + (r - 8) * cos(rad);
        int y0 = cy + (r - 8) * sin(rad);
        int x1 = cx + r * cos(rad);
        int y1 = cy + r * sin(rad);
        // Color gradient: a=0 (left=SWR 3+, red) → a=90 (top=SWR 2, yellow) → a=180 (right=SWR 1, green)
        uint16_t col;
        if (a < 60)      col = C_RED;
        else if (a < 120) col = C_YELLOW;
        else             col = C_GREEN;
        tft.drawLine(x0, y0, x1, y1, col);
    }

    // SWR scale markers
    // SWR 1.0 = right (0°), 1.5 = 60° from right, 2.0 = 90°, 3.0 = 150°, ∞ = 180° (left)
    auto swrToAngle = [](float s) -> float {
        // Map SWR 1.0 → 180 deg, SWR ∞ → 0 deg (arc position)
        // Use: angle = 180 - 180*(s-1)/(s+1) simplified
        float g = (s - 1.0f) / (s + 1.0f); // reflection coefficient
        return 180.0f - 180.0f * g;         // angle in arc (0=left=SWR∞, 180=right=SWR1)
    };

    // Tick marks
    float swrTicks[] = {1.0f, 1.2f, 1.5f, 2.0f, 2.5f, 3.0f};
    const char* tickLabels[] = {"1.0","","1.5","2","","3"};
    for (int i = 0; i < 6; i++) {
        float ang = swrToAngle(swrTicks[i]);
        float rad = (ang + 180) * DEG_TO_RAD;
        int x0 = cx + (r - 4) * cos(rad);
        int y0 = cy + (r - 4) * sin(rad);
        int x1 = cx + (r + 4) * cos(rad);
        int y1 = cy + (r + 4) * sin(rad);
        tft.drawLine(x0, y0, x1, y1, C_TEXT);
        if (strlen(tickLabels[i]) > 0) {
            int lx = cx + (r + 8) * cos(rad) - 6;
            int ly = cy + (r + 8) * sin(rad) - 4;
            tft.setTextSize(1);
            tft.setTextColor(C_DIM, C_BG);
            tft.setCursor(lx, ly);
            tft.print(tickLabels[i]);
        }
    }

    // Needle
    float disp_swr = min(swr, 10.0f);
    float needleAng = swrToAngle(disp_swr);
    float needleRad = (needleAng + 180) * DEG_TO_RAD;
    int nx = cx + (r - 10) * cos(needleRad);
    int ny = cy + (r - 10) * sin(needleRad);
    uint16_t needleCol = (swr <= 1.5f) ? C_GREEN : (swr <= 2.5f) ? C_YELLOW : C_RED;
    tft.drawLine(cx, cy, nx, ny, needleCol);
    tft.drawLine(cx+1, cy, nx, ny, needleCol);

    // Center hub
    tft.fillCircle(cx, cy, 4, C_DIM);

    // SWR value text below gauge
    tft.setTextSize(2);
    char buf[16];
    if (swr >= 99.0f) strcpy(buf, "SWR:--.-");
    else              snprintf(buf, sizeof(buf), "SWR:%.2f", swr);
    uint16_t col = (swr <= 1.5f) ? C_GREEN : (swr <= 2.5f) ? C_YELLOW : C_RED;
    tft.setTextColor(col, C_BG);
    int tx = cx - (strlen(buf) * 12) / 2;
    tft.setCursor(tx, cy + 8);
    tft.print(buf);
    tft.setTextSize(1);
}

// Horizontal SWR bargraph
// SWR 1.0 = full bar (green), 1.5 = 70%, 2.5 = 40%, 3.0 = 0%
void drawSWRBarGraph(int x, int y, int w, int h, float swr) {
    tft.drawRect(x, y, w, h, C_BORDER);
    float fill = constrain((3.0f - min(swr, 3.0f)) / 2.0f, 0.0f, 1.0f);
    int   filled_w = (int)(fill * (w - 2));
    uint16_t col = (swr <= 1.5f) ? C_GREEN : (swr <= 2.5f) ? C_YELLOW : C_RED;
    tft.fillRect(x + 1, y + 1, filled_w, h - 2, col);
    tft.fillRect(x + 1 + filled_w, y + 1, w - 2 - filled_w, h - 2, C_BG);
    // 1.5 marker
    int m15 = x + 1 + (int)(((3.0f - 1.5f) / 2.0f) * (w - 2));
    tft.drawLine(m15, y, m15, y + h, TFT_WHITE);
    // 1.0 marker (right edge)
    int m10 = x + w - 2;
    tft.drawLine(m10, y, m10, y + h, TFT_WHITE);
}

// Tuning animation: rotating arc in bottom-right corner
void drawTuningAnim() {
    if (!ts.is_tuning) return;
    int cx = SCREEN_W - 20, cy = SCREEN_H - BTN_BAR_H - 20;
    int r = 12;
    tft.fillCircle(cx, cy, r + 2, C_BG);
    for (int a = 0; a < 270; a += 10) {
        int da = (a + animAngle) % 360;
        float rad = da * DEG_TO_RAD;
        uint8_t br = (uint8_t)(255 * a / 270);
        // Fade color from dim to bright cyan
        uint16_t col = tft.color565(0, br >> 1, br);
        tft.drawPixel(cx + r * cos(rad), cy + r * sin(rad), col);
    }
}

// =============================================================================
// MODE: BANDS — 11-band grid
// =============================================================================

void drawBandsMode() {
    tft.setCursor(4, CONTENT_Y + 4);
    tft.setTextColor(C_ACCENT, C_BG);
    tft.setTextSize(1);
    tft.print("Tap band to select and load preset");
    drawBandGrid();
}

void drawBandGrid() {
    int cols   = 4;
    int startY = CONTENT_Y + 16;
    int cellW  = SCREEN_W / cols;
    int availH = CONTENT_H - 16;
    int rows   = 3;
    int cellH  = availH / rows;

    for (int i = 0; i < 11; i++) {
        int row = i / cols;
        int col = i % cols;
        int bx  = col * cellW + 2;
        int by  = startY + row * cellH + 2;
        int bw  = cellW - 4;
        int bh  = cellH - 4;
        bool isActive = (i == ts.band_index);
        uint16_t bg = isActive ? C_ACCENT : BAND_COLORS[i];
        uint16_t fg = isActive ? TFT_BLACK : TFT_WHITE;
        tft.fillRoundRect(bx, by, bw, bh, 4, bg);
        tft.drawRoundRect(bx, by, bw, bh, 4, isActive ? TFT_WHITE : C_BORDER);
        tft.setTextColor(fg, bg);
        tft.setTextSize(1);
        tft.setCursor(bx + (bw - strlen(BAND_NAMES[i]) * 6) / 2, by + (bh - 8) / 2 - 4);
        tft.print(BAND_NAMES[i]);
        // Show SWR if active band
        if (isActive) {
            char swr_str[10];
            if (ts.swr >= 99.0f) strcpy(swr_str, "---");
            else snprintf(swr_str, sizeof(swr_str), "%.2f", ts.swr);
            tft.setCursor(bx + (bw - strlen(swr_str) * 6) / 2, by + (bh / 2) + 2);
            tft.print(swr_str);
        }
    }
}

// =============================================================================
// MODE: MATCH — Impedance and network topology diagram
// =============================================================================

void drawMatchMode() {
    int y = CONTENT_Y + 6;

    // Header
    tft.setTextColor(C_ACCENT, C_BG);
    tft.setTextSize(1);
    tft.setCursor(4, y);
    tft.printf("Network: %-15s  Band: %s", ts.network, ts.band_name);
    y += 16;

    // L/C values
    tft.setTextColor(C_TEXT, C_BG);
    tft.setCursor(4, y);
    tft.printf("L = %.3f uH    C = %.1f pF", ts.L_uh, ts.C_pf);
    y += 14;
    tft.setTextColor(C_DIM, C_BG);
    tft.setCursor(4, y);
    tft.printf("RL = %.1f dB    Gamma = %.3f", ts.return_loss, ts.gamma);
    y += 18;

    // Network topology ASCII-style diagram using drawLine
    drawNetworkDiagram(4, y, SCREEN_W - 8, 80, ts.network);
    y += 84;

    // Impedance note
    tft.setTextColor(C_DIM, C_BG);
    tft.setTextSize(1);
    tft.setCursor(4, y);
    tft.print("Tap TUNE for auto-match. Use BANDS to change band.");
}

void drawNetworkDiagram(int x, int y, int w, int h, const char* net) {
    tft.fillRect(x, y, w, h, C_CARD);
    tft.drawRect(x, y, w, h, C_BORDER);

    // Schematic drawing uses tft.drawLine (ASCII-style on screen)
    int midY = y + h / 2;
    int lx = x + 10, rx = x + w - 10;
    int mx = x + w / 2;

    // TX label left
    tft.setTextColor(C_ACCENT, C_CARD);
    tft.setCursor(x + 2, midY - 4);
    tft.print("TX");
    int tx_end = x + 18;

    // ANT label right
    tft.setCursor(x + w - 22, midY - 4);
    tft.print("ANT");
    int ant_start = x + w - 24;

    if (strcmp(net, "L_LOWPASS") == 0) {
        // Series L then shunt C
        // Line: TX --- [LLL] --- node --- [C down] + --- ANT
        tft.drawLine(tx_end, midY, tx_end + 20, midY, C_TEXT);
        drawInductorSymbol(tx_end + 20, midY, 40);  // L symbol
        int node_x = tx_end + 64;
        tft.drawLine(node_x, midY, ant_start, midY, C_TEXT);
        drawCapSymbol(node_x, midY, 20);  // C to GND
        tft.setTextColor(C_GREEN, C_CARD); tft.setCursor(tx_end + 30, midY - 14); tft.print("L");
        tft.setTextColor(C_YELLOW, C_CARD); tft.setCursor(node_x + 4, midY + 22); tft.print("C");

    } else if (strcmp(net, "L_HIGHPASS") == 0) {
        // Shunt C then series L
        int node_x = tx_end + 20;
        tft.drawLine(tx_end, midY, node_x, midY, C_TEXT);
        drawCapSymbol(node_x, midY, 20);
        tft.drawLine(node_x, midY, node_x + 20, midY, C_TEXT);
        drawInductorSymbol(node_x + 20, midY, 40);
        tft.drawLine(node_x + 64, midY, ant_start, midY, C_TEXT);
        tft.setTextColor(C_YELLOW, C_CARD); tft.setCursor(node_x + 4, midY + 22); tft.print("C");
        tft.setTextColor(C_GREEN, C_CARD); tft.setCursor(node_x + 30, midY - 14); tft.print("L");

    } else if (strcmp(net, "PI_NETWORK") == 0) {
        // C1 shunt, series L, C2 shunt
        int n1 = tx_end + 18, n2 = ant_start - 18;
        tft.drawLine(tx_end, midY, n1, midY, C_TEXT);
        drawCapSymbol(n1, midY, 20);
        tft.drawLine(n1, midY, n1 + 16, midY, C_TEXT);
        drawInductorSymbol(n1 + 16, midY, 50);
        int after_l = n1 + 70;
        tft.drawLine(after_l, midY, n2, midY, C_TEXT);
        drawCapSymbol(n2, midY, 20);
        tft.drawLine(n2, midY, ant_start, midY, C_TEXT);
        tft.setTextColor(C_YELLOW, C_CARD); tft.setCursor(n1 + 2, midY + 22); tft.print("C1");
        tft.setTextColor(C_GREEN,  C_CARD); tft.setCursor(n1 + 24, midY - 14); tft.print("L");
        tft.setTextColor(C_YELLOW, C_CARD); tft.setCursor(n2 + 2, midY + 22); tft.print("C2");

    } else if (strcmp(net, "T_NETWORK") == 0) {
        // L1 series, C shunt, L2 series
        int n1 = tx_end + 16, n2 = ant_start - 60;
        tft.drawLine(tx_end, midY, n1, midY, C_TEXT);
        drawInductorSymbol(n1, midY, 36);
        int after_l1 = n1 + 38;
        tft.drawLine(after_l1, midY, n2, midY, C_TEXT);
        drawCapSymbol(n2, midY, 20);
        tft.drawLine(n2, midY, n2 + 16, midY, C_TEXT);
        drawInductorSymbol(n2 + 16, midY, 36);
        tft.drawLine(n2 + 54, midY, ant_start, midY, C_TEXT);
        tft.setTextColor(C_GREEN,  C_CARD); tft.setCursor(n1 + 6, midY - 14); tft.print("L1");
        tft.setTextColor(C_YELLOW, C_CARD); tft.setCursor(n2 + 2, midY + 22); tft.print("C");
        tft.setTextColor(C_GREEN,  C_CARD); tft.setCursor(n2 + 22, midY - 14); tft.print("L2");
    }

    // Ground symbols at bottom of shunt components
    drawGndSymbol(x + w / 2, y + h - 6);
}

// Draw inductor symbol (coil arcs) at (x, midY), length len px
void drawInductorSymbol(int x, int midY, int len) {
    int bumps = 4;
    int bumpW = len / bumps;
    int bumpR = bumpW / 2;
    for (int i = 0; i < bumps; i++) {
        int cx = x + i * bumpW + bumpR;
        // Draw semicircle arc using line segments
        for (int a = 0; a < 180; a += 15) {
            float r1 = a * DEG_TO_RAD;
            float r2 = (a + 15) * DEG_TO_RAD;
            tft.drawLine(
                cx + bumpR * cos(r1), midY - bumpR * sin(r1),
                cx + bumpR * cos(r2), midY - bumpR * sin(r2),
                C_GREEN
            );
        }
    }
    // Baseline
    tft.drawLine(x, midY, x + len, midY, C_TEXT);
}

// Draw capacitor symbol (two parallel plates) at (nodeX, midY), plates below
void drawCapSymbol(int nodeX, int midY, int stemLen) {
    tft.drawLine(nodeX, midY, nodeX, midY + stemLen, C_TEXT);    // stem down
    tft.drawLine(nodeX - 10, midY + stemLen, nodeX + 10, midY + stemLen, C_YELLOW); // plate 1
    tft.drawLine(nodeX - 10, midY + stemLen + 6, nodeX + 10, midY + stemLen + 6, C_YELLOW); // plate 2
    tft.drawLine(nodeX, midY + stemLen + 6, nodeX, midY + stemLen + 16, C_TEXT); // stem to GND
    // GND line
    tft.drawLine(nodeX - 8, midY + stemLen + 16, nodeX + 8, midY + stemLen + 16, C_TEXT);
}

// Draw ground symbol at (x, y)
void drawGndSymbol(int x, int y) {
    tft.drawLine(x - 10, y, x + 10, y, C_DIM);
    tft.drawLine(x - 6, y + 3, x + 6, y + 3, C_DIM);
    tft.drawLine(x - 2, y + 6, x + 2, y + 6, C_DIM);
}

// =============================================================================
// MODE: WIFI — IP address, device info
// =============================================================================

void drawWifiMode() {
    int y = CONTENT_Y + 8;
    tft.setTextColor(C_ACCENT, C_BG);
    tft.setTextSize(2);
    tft.setCursor(20, y);
    tft.print("RF-HFTuner WiFi");
    y += 24;

    tft.setTextSize(1);
    tft.setTextColor(C_TEXT, C_BG);
    tft.setCursor(10, y);
    tft.printf("SSID:     RF-HFTuner");
    y += 14;
    tft.setCursor(10, y);
    tft.printf("Password: hamradio");
    y += 14;
    tft.setCursor(10, y);
    tft.printf("IP Addr:  %s", ts.wifi_ip);
    y += 14;
    tft.setCursor(10, y);
    tft.printf("URL:      http://%s/", ts.wifi_ip);
    y += 20;

    tft.setTextColor(C_DIM, C_BG);
    tft.setCursor(10, y);
    tft.print("BT Device: RF-HFTuner");
    y += 14;
    tft.setCursor(10, y);
    tft.print("BT Status: ");
    tft.setTextColor(btConnected ? C_GREEN : C_RED, C_BG);
    tft.print(btConnected ? "Connected" : "Disconnected");
    y += 14;

    tft.setTextColor(C_DIM, C_BG);
    tft.setCursor(10, y);
    tft.printf("Tuner: %s   Homed: %s",
               ts.state, ts.homed ? "Yes" : "No");
    y += 16;

    // QR code hint
    tft.drawRect(10, y, 60, 60, C_BORDER);
    tft.setTextColor(C_DIM, C_BG);
    tft.setCursor(15, y + 26);
    tft.print(" [QR code]");
    tft.setCursor(15, y + 36);
    tft.print("  browser");
    tft.setTextColor(C_DIM, C_BG);
    tft.setCursor(80, y + 4);
    tft.print("Connect phone/PC");
    tft.setCursor(80, y + 16);
    tft.print("to RF-HFTuner WiFi");
    tft.setCursor(80, y + 28);
    tft.print("then open browser:");
    tft.setCursor(80, y + 40);
    tft.setTextColor(C_ACCENT, C_BG);
    tft.printf("http://%s", ts.wifi_ip);
}

// =============================================================================
// RGB LED CONTROL
// =============================================================================

// CYD RGB LED is common-anode: LOW = ON, HIGH = OFF
void setLED(uint8_t r, uint8_t g, uint8_t b) {
    analogWrite(CYD_LED_R, 255 - r);
    analogWrite(CYD_LED_G, 255 - g);
    analogWrite(CYD_LED_B, 255 - b);
}

void updateLED() {
    if (!btConnected) {
        setLED(255, 165, 0);  // Orange: BT disconnected
    } else if (ts.is_tuning) {
        // Flash white during tuning
        setLED((millis() / 100) % 2 ? 255 : 0,
               (millis() / 100) % 2 ? 255 : 0,
               (millis() / 100) % 2 ? 255 : 0);
    } else if (ts.swr <= 1.5f && ts.swr >= 1.0f) {
        setLED(0, 255, 0);    // Green: SWR < 1.5
    } else if (ts.swr <= 2.5f) {
        setLED(255, 200, 0);  // Yellow: SWR 1.5–2.5
    } else {
        setLED(255, 0, 0);    // Red: SWR > 2.5 or no RF
    }
}

// =============================================================================
// END OF cyd_tuner_display.ino
// =============================================================================
