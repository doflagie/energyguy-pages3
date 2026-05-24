/**
 * cyd_roller_display.ino — TM-ROLLER-CYD-001 Rev A
 * CYD (ILI9341 320×240) Display for Roller Inductor Controller
 *
 * Bluetooth Master: connects to "RF-RollerTuner" controller.
 * Displays inductance, position, motor state, band presets.
 * Touch controls: HOME, STEP+/−, BAND SELECT, SETL, SWEEP.
 *
 * Libraries: TFT_eSPI, XPT2046_Touchscreen, ArduinoJson, BluetoothSerial
 *
 * CYD pin assignments (factory):
 *   TFT: SPI, CS=15, DC=2, RST=-1, MOSI=13, SCK=14, MISO=12
 *   Touch: XPT2046, CS=33, IRQ=36
 *   TFT_BL: GPIO21 (PWM backlight)
 */

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>

// ─── Display Setup ────────────────────────────────────────────────────────────

TFT_eSPI     tft = TFT_eSPI();
TFT_eSprite  spr = TFT_eSprite(&tft);
BluetoothSerial btSerial;

#define TOUCH_CS    33
#define TOUCH_IRQ   36
#define TFT_BL      21

XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);

// ─── Color Palette ────────────────────────────────────────────────────────────

#define COL_BG          TFT_BLACK
#define COL_GREEN       TFT_GREEN
#define COL_CYAN        0x07FF    // Cyan
#define COL_ORANGE      0xFD20    // Orange
#define COL_RED         TFT_RED
#define COL_YELLOW      TFT_YELLOW
#define COL_WHITE       TFT_WHITE
#define COL_GRAY        0x8410    // Dark gray
#define COL_DKBLUE      0x0011    // Dark blue (tab background)
#define COL_LTBLUE      0x1BDF    // Light blue (active tab)

// ─── Display Modes ────────────────────────────────────────────────────────────

enum DisplayMode { MODE_LIVE = 0, MODE_BAND, MODE_SWEEP, MODE_SETTINGS, MODE_COUNT };
DisplayMode current_mode = MODE_LIVE;

// ─── Live Data ────────────────────────────────────────────────────────────────

struct RollerData {
    float    l_uh       = 0.0f;
    float    turns      = 0.0f;
    float    pos_mm     = 0.0f;
    long     steps      = 0;
    int      state      = 0;     // MotorState enum
    bool     homed      = false;
    bool     moving     = false;
    bool     fault      = false;
    char     fw[12]     = "---";
    uint32_t last_rx_ms = 0;
};

RollerData live;

// L history for sparkline (60 points)
static float l_history[60];
static int   l_hist_idx = 0;

// Sweep data
struct SweepPt { float l_cmd; float l_actual; };
static SweepPt sweep_pts[64];
static int sweep_n = 0;
static bool sweep_active = false;

// ─── Band Presets (mirrored from controller) ──────────────────────────────────

struct BandItem { const char* name; float l_uh; };
static const BandItem BANDS[] = {
    {"160m", 28.0f}, {"80m",  18.0f}, {"60m",  12.0f},
    {"40m",   8.0f}, {"30m",   5.5f}, {"20m",   3.5f},
    {"17m",   2.2f}, {"15m",   1.5f}, {"12m",   1.1f}, {"10m", 0.8f},
};
static const int N_BANDS = 10;
static int selected_band = 4;   // Default 40m

// ─── Touch Zones ─────────────────────────────────────────────────────────────

struct Zone { int x, y, w, h; const char* label; };

// Top tab bar
static const Zone TABS[] = {
    {  0,  0, 79, 28, "LIVE"},
    { 80,  0, 79, 28, "BAND"},
    {160,  0, 79, 28, "SWEEP"},
    {240,  0, 80, 28, "SETUP"},
};

// Bottom button bar
static const Zone BTNS[] = {
    {  0, 212, 79, 28, "HOME"},
    { 80, 212, 59, 28, "STEP-"},
    {140, 212, 59, 28, "STEP+"},
    {200, 212, 59, 28, "STOP"},
    {260, 212, 60, 28, "SETL"},
};

bool hitTest(const Zone& z, int tx, int ty) {
    return tx >= z.x && tx < z.x + z.w && ty >= z.y && ty < z.y + z.h;
}

// ─── BT Command ───────────────────────────────────────────────────────────────

void sendCmd(const char* cmd) {
    if (btSerial.hasClient()) {
        btSerial.println(cmd);
        Serial.printf("BT_CMD: %s\n", cmd);
    } else {
        Serial.printf("BT not connected, CMD: %s\n", cmd);
    }
}

// ─── JSON Parse ───────────────────────────────────────────────────────────────

void parseStatus(const String& json) {
    StaticJsonDocument<256> doc;
    if (deserializeJson(doc, json)) return;
    live.l_uh   = doc["l_uh"]   | 0.0f;
    live.turns  = doc["turns"]  | 0.0f;
    live.pos_mm = doc["pos_mm"] | 0.0f;
    live.steps  = doc["steps"]  | 0L;
    live.state  = doc["state"]  | 0;
    live.homed  = doc["homed"]  | false;
    live.moving = doc["moving"] | false;
    live.fault  = doc["fault"]  | false;
    const char* fw = doc["fw"];
    if (fw) strncpy(live.fw, fw, 11);
    live.last_rx_ms = millis();
    // Update history
    l_history[l_hist_idx] = live.l_uh;
    l_hist_idx = (l_hist_idx + 1) % 60;
}

// ─── Drawing Functions ────────────────────────────────────────────────────────

void drawTabBar() {
    for (int i = 0; i < 4; i++) {
        uint16_t col = (current_mode == i) ? COL_LTBLUE : COL_DKBLUE;
        spr.fillRect(TABS[i].x, TABS[i].y, TABS[i].w, TABS[i].h, col);
        spr.drawRect(TABS[i].x, TABS[i].y, TABS[i].w, TABS[i].h, COL_GRAY);
        spr.setTextColor(COL_WHITE);
        spr.setTextSize(1);
        spr.setCursor(TABS[i].x + 6, TABS[i].y + 10);
        spr.print(TABS[i].label);
    }
    // BT indicator
    uint16_t bt_col = btSerial.hasClient() ? COL_GREEN : COL_GRAY;
    spr.fillCircle(308, 14, 5, bt_col);
    spr.setTextColor(COL_WHITE);
    spr.setTextSize(1);
    spr.setCursor(295, 5);
    spr.print("BT");
}

void drawBottomBar() {
    for (int i = 0; i < 5; i++) {
        uint16_t col = COL_DKBLUE;
        if (i == 0 && !live.homed)      col = COL_ORANGE;
        if (i == 3 && live.moving)      col = COL_RED;
        spr.fillRect(BTNS[i].x, BTNS[i].y, BTNS[i].w, BTNS[i].h, col);
        spr.drawRect(BTNS[i].x, BTNS[i].y, BTNS[i].w, BTNS[i].h, COL_GRAY);
        spr.setTextColor(COL_WHITE);
        spr.setTextSize(1);
        spr.setCursor(BTNS[i].x + 4, BTNS[i].y + 10);
        spr.print(BTNS[i].label);
    }
}

void drawInductanceBar(float l_uh) {
    // Horizontal bar 0–34 µH
    int bar_x = 10, bar_y = 120, bar_w = 300, bar_h = 20;
    spr.drawRect(bar_x, bar_y, bar_w, bar_h, COL_GRAY);
    float pct = constrain(l_uh / 34.0f, 0.0f, 1.0f);
    int fill = (int)(pct * (bar_w - 2));
    // Gradient: green → yellow → orange
    uint16_t col = (pct < 0.5f) ? COL_GREEN :
                   (pct < 0.8f) ? COL_YELLOW : COL_ORANGE;
    spr.fillRect(bar_x + 1, bar_y + 1, fill, bar_h - 2, col);
    // Scale marks at 5, 10, 15, 20, 25, 30 µH
    for (float mark = 5.0f; mark <= 30.0f; mark += 5.0f) {
        int mx = bar_x + (int)(mark / 34.0f * (bar_w - 2));
        spr.drawFastVLine(mx, bar_y + bar_h, 4, COL_GRAY);
        spr.setTextColor(COL_GRAY);
        spr.setTextSize(1);
        spr.setCursor(mx - 4, bar_y + bar_h + 5);
        spr.printf("%.0f", mark);
    }
}

void drawSparkline(int x, int y, int w, int h) {
    // 60-point L history
    float l_min = 34.0f, l_max = 0.0f;
    for (int i = 0; i < 60; i++) {
        if (l_history[i] > l_max) l_max = l_history[i];
        if (l_history[i] < l_min) l_min = l_history[i];
    }
    float rng = l_max - l_min;
    if (rng < 0.1f) rng = 0.1f;
    spr.drawRect(x, y, w, h, COL_GRAY);
    for (int i = 1; i < 60; i++) {
        int idx0 = (l_hist_idx + i - 1) % 60;
        int idx1 = (l_hist_idx + i    ) % 60;
        float v0 = l_history[idx0], v1 = l_history[idx1];
        int y0 = y + h - 2 - (int)((v0 - l_min) / rng * (h - 4));
        int y1 = y + h - 2 - (int)((v1 - l_min) / rng * (h - 4));
        int x0 = x + 1 + (i - 1) * (w - 2) / 59;
        int x1 = x + 1 +  i      * (w - 2) / 59;
        spr.drawLine(x0, y0, x1, y1, COL_CYAN);
    }
}

void drawLiveMode() {
    spr.fillRect(0, 30, 320, 180, COL_BG);

    bool connected = (millis() - live.last_rx_ms < 3000);
    uint16_t data_col = connected ? COL_WHITE : COL_GRAY;

    // Large inductance readout
    spr.setTextColor(COL_GREEN);
    spr.setTextSize(4);
    spr.setCursor(10, 38);
    if (connected) spr.printf("%.2f", live.l_uh);
    else           spr.print("--.-");
    spr.setTextSize(2);
    spr.setTextColor(COL_CYAN);
    spr.setCursor(210, 45);
    spr.print("uH");

    // Turns and position
    spr.setTextSize(2);
    spr.setTextColor(data_col);
    spr.setCursor(10, 82);
    spr.printf("%.1f turns  %.1fmm", live.turns, live.pos_mm);

    // Motor state
    spr.setTextSize(1);
    const char* state_str[] = {"IDLE","MOVING","HOMING","FAULT","NOT HOMED"};
    const uint16_t state_col[] = {COL_GREEN, COL_YELLOW, COL_ORANGE, COL_RED, COL_ORANGE};
    int st = constrain(live.state, 0, 4);
    spr.setTextColor(state_col[st]);
    spr.setCursor(10, 102);
    spr.printf("Motor: %s  Steps: %ld", state_str[st], live.steps);

    // Inductance bar
    drawInductanceBar(live.l_uh);

    // Sparkline
    drawSparkline(10, 150, 140, 45);
    spr.setTextColor(COL_GRAY);
    spr.setTextSize(1);
    spr.setCursor(10, 196);
    spr.print("L history");

    // Fault indicator
    if (live.fault) {
        spr.fillRect(160, 150, 150, 40, COL_RED);
        spr.setTextColor(COL_WHITE);
        spr.setTextSize(2);
        spr.setCursor(170, 162);
        spr.print("FAULT!");
    }

    // Firmware version
    spr.setTextColor(COL_GRAY);
    spr.setTextSize(1);
    spr.setCursor(160, 196);
    spr.printf("fw:%s", live.fw);
}

void drawBandMode() {
    spr.fillRect(0, 30, 320, 180, COL_BG);
    spr.setTextColor(COL_WHITE);
    spr.setTextSize(1);
    spr.setCursor(10, 35);
    spr.print("BAND PRESETS — tap to select and go");

    // Band grid (5 per row × 2 rows)
    for (int i = 0; i < N_BANDS; i++) {
        int col = i % 5;
        int row = i / 5;
        int bx = 4 + col * 62;
        int by = 50 + row * 60;
        uint16_t bg = (i == selected_band) ? COL_LTBLUE : COL_DKBLUE;
        spr.fillRect(bx, by, 58, 50, bg);
        spr.drawRect(bx, by, 58, 50, COL_GRAY);
        spr.setTextColor(COL_WHITE);
        spr.setTextSize(2);
        spr.setCursor(bx + 6, by + 6);
        spr.print(BANDS[i].name);
        spr.setTextSize(1);
        spr.setTextColor(COL_CYAN);
        spr.setCursor(bx + 4, by + 30);
        spr.printf("%.1fuH", BANDS[i].l_uh);
    }

    // "GO" button
    spr.fillRect(230, 160, 80, 35, COL_GREEN);
    spr.drawRect(230, 160, 80, 35, COL_WHITE);
    spr.setTextColor(COL_BG);
    spr.setTextSize(2);
    spr.setCursor(247, 170);
    spr.print("GO");
}

void drawSweepMode() {
    spr.fillRect(0, 30, 320, 180, COL_BG);

    if (sweep_n < 2) {
        spr.setTextColor(COL_WHITE);
        spr.setTextSize(1);
        spr.setCursor(10, 80);
        spr.print("Send SWEEP command to start.");
        spr.setCursor(10, 96);
        spr.print("Example: SWEEP 1.0 30.0 0.5");
        // Sweep button
        spr.fillRect(100, 140, 120, 35, COL_CYAN);
        spr.setTextColor(COL_BG);
        spr.setTextSize(1);
        spr.setCursor(108, 150);
        spr.print("START SWEEP");
        return;
    }

    // XY chart of commanded vs. measured inductance
    int cx = 40, cy = 35, cw = 270, ch = 130;
    spr.drawRect(cx, cy, cw, ch, COL_GRAY);
    spr.setTextColor(COL_GRAY);
    spr.setTextSize(1);

    // Axis labels
    float l_min = sweep_pts[0].l_cmd;
    float l_max = sweep_pts[sweep_n - 1].l_cmd;
    for (int g = 0; g <= 5; g++) {
        float l  = l_min + g * (l_max - l_min) / 5.0f;
        int   gx = cx + (int)((l - l_min) / (l_max - l_min) * (cw - 2)) + 1;
        spr.drawFastVLine(gx, cy, ch, COL_GRAY);
        spr.setCursor(gx - 8, cy + ch + 2);
        spr.printf("%.0f", l);
    }
    spr.setCursor(cx + cw/2 - 20, cy + ch + 12);
    spr.print("L cmd (uH)");
    spr.setTextColor(COL_CYAN);
    spr.setCursor(0, cy + ch/2);
    spr.print("L\n act");

    // Plot points
    for (int i = 0; i < sweep_n; i++) {
        float lc = sweep_pts[i].l_cmd;
        float la = sweep_pts[i].l_actual;
        int px = cx + 1 + (int)((lc - l_min) / (l_max - l_min) * (cw - 2));
        int py = cy + ch - 2 - (int)((la - l_min) / (l_max - l_min) * (ch - 4));
        spr.fillCircle(px, py, 2, COL_CYAN);
    }

    // Status
    spr.setTextColor(COL_WHITE);
    spr.setTextSize(1);
    spr.setCursor(10, 180);
    spr.printf("%d points  %.1f-%.1f uH", sweep_n, l_min, l_max);
}

void drawSettingsMode() {
    spr.fillRect(0, 30, 320, 180, COL_BG);
    spr.setTextColor(COL_WHITE);
    spr.setTextSize(1);
    spr.setCursor(10, 38);
    spr.print("SETTINGS");

    // Device info
    spr.setTextColor(COL_CYAN);
    spr.setCursor(10, 56);
    spr.printf("Device fw: %s", live.fw);
    spr.setCursor(10, 70);
    spr.printf("Steps: %ld  Pos: %.2fmm", live.steps, live.pos_mm);
    spr.setCursor(10, 84);
    spr.printf("%.1f turns  L=%.3f uH", live.turns, live.l_uh);

    // Manual L entry
    spr.fillRect(10, 110, 140, 30, COL_DKBLUE);
    spr.drawRect(10, 110, 140, 30, COL_GRAY);
    spr.setTextColor(COL_WHITE);
    spr.setCursor(16, 120);
    spr.print("SETL (manual)");

    // Table dump
    spr.fillRect(10, 150, 140, 30, COL_DKBLUE);
    spr.drawRect(10, 150, 140, 30, COL_GRAY);
    spr.setTextColor(COL_WHITE);
    spr.setCursor(16, 160);
    spr.print("DUMP TABLE (serial)");

    // Rehome button
    spr.fillRect(170, 110, 140, 30, COL_ORANGE);
    spr.setTextColor(COL_BG);
    spr.setCursor(200, 120);
    spr.print("REHOME");
}

void drawFrame() {
    spr.fillSprite(COL_BG);
    drawTabBar();
    drawBottomBar();
    switch (current_mode) {
        case MODE_LIVE:     drawLiveMode();     break;
        case MODE_BAND:     drawBandMode();     break;
        case MODE_SWEEP:    drawSweepMode();    break;
        case MODE_SETTINGS: drawSettingsMode(); break;
        default: break;
    }
    spr.pushSprite(0, 0);
}

// ─── Touch Handling ───────────────────────────────────────────────────────────

void handleTouch(int tx, int ty) {
    // Tab bar
    for (int i = 0; i < 4; i++) {
        if (hitTest(TABS[i], tx, ty)) { current_mode = (DisplayMode)i; return; }
    }
    // Bottom buttons
    if (hitTest(BTNS[0], tx, ty)) { sendCmd("HOME");  return; }
    if (hitTest(BTNS[1], tx, ty)) { sendCmd("STEP -8000"); return; }
    if (hitTest(BTNS[2], tx, ty)) { sendCmd("STEP  8000"); return; }
    if (hitTest(BTNS[3], tx, ty)) { sendCmd("STOP");  return; }
    if (hitTest(BTNS[4], tx, ty)) {
        // SETL — go to selected band's inductance
        char cmd[32];
        snprintf(cmd, sizeof(cmd), "SETL %.2f", BANDS[selected_band].l_uh);
        sendCmd(cmd);
        return;
    }

    // Mode-specific touch
    if (current_mode == MODE_BAND) {
        for (int i = 0; i < N_BANDS; i++) {
            int col = i % 5, row = i / 5;
            int bx = 4 + col * 62, by = 50 + row * 60;
            if (tx >= bx && tx < bx + 58 && ty >= by && ty < by + 50) {
                selected_band = i;
                return;
            }
        }
        // GO button
        if (tx >= 230 && tx < 310 && ty >= 160 && ty < 195) {
            char cmd[32];
            snprintf(cmd, sizeof(cmd), "BAND %s", BANDS[selected_band].name);
            sendCmd(cmd);
        }
    }
    if (current_mode == MODE_SWEEP) {
        if (tx >= 100 && tx < 220 && ty >= 140 && ty < 175)
            sendCmd("SWEEP 1.0 30.0 0.5");
    }
    if (current_mode == MODE_SETTINGS) {
        if (tx >= 170 && tx < 310 && ty >= 110 && ty < 140) sendCmd("HOME");
        if (tx >= 10 && tx < 150 && ty >= 150 && ty < 180) sendCmd("TABLE");
    }
}

// ─── BT Receive ───────────────────────────────────────────────────────────────

String btBuf = "";
void checkBT() {
    while (btSerial.available()) {
        char c = (char)btSerial.read();
        if (c == '\n') {
            btBuf.trim();
            if (btBuf.startsWith("{")) {
                parseStatus(btBuf);
            } else if (btBuf.startsWith("SWEEP_PT,")) {
                float lc, la;
                if (sscanf(btBuf.c_str() + 9, "%f,%f", &lc, &la) == 2 && sweep_n < 64) {
                    sweep_pts[sweep_n++] = {lc, la};
                    sweep_active = true;
                }
            } else if (btBuf == "SWEEP_END") {
                sweep_active = false;
            }
            btBuf = "";
        } else if (btBuf.length() < 250) {
            btBuf += c;
        }
    }
}

// ─── setup() ─────────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(115200);
    Serial.println("CYD RollerTuner Display");

    // Backlight
    ledcSetup(0, 5000, 8);
    ledcAttachPin(TFT_BL, 0);
    ledcWrite(0, 220);   // ~86% brightness

    // TFT
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(COL_BG);

    // Sprite (full-screen double buffer)
    spr.createSprite(320, 240);

    // Touch
    SPI.begin();
    touch.begin();
    touch.setRotation(1);

    // BT Master
    btSerial.begin("CYD-RollerDisplay", true);  // true = master mode
    Serial.println("BT: connecting to RF-RollerTuner...");
    // Note: actual connection happens in loop()

    // Clear history
    memset(l_history, 0, sizeof(l_history));

    drawFrame();
}

// ─── loop() ──────────────────────────────────────────────────────────────────

uint32_t last_draw_ms  = 0;
uint32_t last_bt_check = 0;
bool     bt_connected  = false;

void loop() {
    // BT connect
    if (!bt_connected && millis() - last_bt_check > 3000) {
        last_bt_check = millis();
        if (btSerial.connect("RF-RollerTuner")) {
            bt_connected = true;
            Serial.println("BT: connected");
        }
    }
    if (bt_connected && !btSerial.hasClient()) {
        bt_connected = false;
        Serial.println("BT: disconnected");
    }

    checkBT();

    // Touch
    if (touch.tirqTouched() && touch.touched()) {
        TS_Point p = touch.getPoint();
        // Map raw touch coordinates to screen (calibration values)
        int tx = map(p.x, 200, 3700, 0, 320);
        int ty = map(p.y, 200, 3700, 0, 240);
        tx = constrain(tx, 0, 319);
        ty = constrain(ty, 0, 239);
        handleTouch(tx, ty);
        delay(50);  // Debounce touch
    }

    // Redraw at 10 Hz
    if (millis() - last_draw_ms >= 100) {
        last_draw_ms = millis();
        drawFrame();
    }
}
