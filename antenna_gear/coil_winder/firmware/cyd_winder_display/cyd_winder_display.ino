/**
 * cyd_winder_display.ino — TM-CWND-CYD-001 Rev A
 * CYD (Cheap Yellow Display) Touchscreen Interface
 * for Motorized Coil Winder Controller
 *
 * Hardware: ESP32-2432S028 (ILI9341 320×240, XPT2046 touch)
 * BT SPP master → connects to "RF-CoilWinder"
 *
 * Modes:
 *   [WIND]   — Live winding status, turn counter, RPM, layer
 *   [SETUP]  — Program parameters (turns, AWG, former dia, layers)
 *   [CALC]   — Wheeler calculator, Q factor, wire length
 *   [LOG]    — Last 20 winding results, inductance history
 */

#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>

// ─── Pins ─────────────────────────────────────────────────────────────────────
#define XPT_CS   33
#define XPT_IRQ  36
#define RGB_R    4
#define RGB_G    16
#define RGB_B    17

// ─── Screen geometry ──────────────────────────────────────────────────────────
#define SCR_W   320
#define SCR_H   240
#define TAB_H   28
#define BTN_H   28
#define HDR_H   20
#define WORK_H  (SCR_H - HDR_H - TAB_H - BTN_H)
#define WORK_Y  HDR_H

// ─── Colors ───────────────────────────────────────────────────────────────────
#define COL_BG       TFT_BLACK
#define COL_HEADER   0x1082
#define COL_TAB_A    0x0438
#define COL_TAB_I    0x2945
#define COL_TEXT     TFT_WHITE
#define COL_DIM      0x8410
#define COL_GREEN    TFT_GREEN
#define COL_YELLOW   TFT_YELLOW
#define COL_ORANGE   0xFD20
#define COL_RED      TFT_RED
#define COL_CYAN     TFT_CYAN
#define COL_PROGRESS 0x07FF   // Cyan bar

// ─── Mode ─────────────────────────────────────────────────────────────────────
enum Mode { MODE_WIND, MODE_SETUP, MODE_CALC, MODE_LOG, MODE_COUNT };
Mode     mode = MODE_WIND;
bool     dirty = true;

// ─── Hardware ─────────────────────────────────────────────────────────────────
TFT_eSPI          tft;
XPT2046_Touchscreen touch(XPT_CS, XPT_IRQ);
BluetoothSerial    btMaster;

// ─── Winder Status ────────────────────────────────────────────────────────────
struct WinderStatus {
    unsigned long turns    = 0;
    unsigned long target   = 0;
    float  rpm             = 0;
    float  calc_uh         = 0;
    int    layer           = 1;
    float  trav_mm         = 0;
    int    state           = 0;   // 0=IDLE,1=WIND,2=PAUSE,3=HOME,4=FAULT,5=DONE
    int    fault           = 0;
    bool   homed           = false;
    uint8_t awg            = 22;
    float  former_mm       = 25.4f;
    int    layers          = 1;
    char   fw[16]          = "?.?.?";
    uint32_t age_ms        = 0;
} ws;

// ─── Log buffer (last 20 completed windings) ──────────────────────────────────
struct LogEntry { unsigned long turns; float uh; uint8_t awg; float dia_mm; };
LogEntry log_buf[20];
int      log_n = 0;

// ─── Sparkline (60 RPM samples) ───────────────────────────────────────────────
float rpm_hist[60];
int   rpm_head = 0;

// ─── BT ───────────────────────────────────────────────────────────────────────
bool   btConnected = false;
String btBuf = "";
uint32_t last_rx = 0;

// ─── Setup params (local edit state) ─────────────────────────────────────────
struct SetupParam {
    const char* label;
    float*      valptr;
    float       step;
    float       min_v;
    float       max_v;
    const char* unit;
};

float sp_turns   = 0;
float sp_uh      = 0;
float sp_dia     = 25.4f;
float sp_awg     = 22;
float sp_layers  = 1;
float sp_pitch   = 1.0f;
float sp_len     = 25.4f;

SetupParam params[] = {
    {"Turns",    &sp_turns,  1,    0,    10000, ""},
    {"Target L", &sp_uh,     0.1f, 0,    5000,  "uH"},
    {"Former D", &sp_dia,    0.5f, 6.35f,101.6f,"mm"},
    {"AWG",      &sp_awg,    1,    10,   30,    ""},
    {"Layers",   &sp_layers, 1,    1,    20,    ""},
    {"Pitch",    &sp_pitch,  0.1f, 1.0f, 5.0f,  "×"},
    {"Coil L",   &sp_len,    1,    5,    310,   "mm"},
};
static const int N_PARAMS = sizeof(params)/sizeof(params[0]);
int  param_sel = 0;

// ─── Calc mode ────────────────────────────────────────────────────────────────
float calc_d_in   = 1.0f;
float calc_n      = 20;
float calc_l_in   = 1.0f;
float calc_result = 0;

float wheelerSingle(float d, float N, float l) {
    if (l < 0.001f || d < 0.001f) return 0;
    return (d * d * N * N) / (18.0f * d + 40.0f * l);
}

// ─── Parse JSON ───────────────────────────────────────────────────────────────
void parseStatus(const String& json) {
    StaticJsonDocument<384> doc;
    if (deserializeJson(doc, json) != DeserializationError::Ok) return;
    ws.turns    = doc["turns"]     | 0UL;
    ws.target   = doc["target"]    | 0UL;
    ws.rpm      = doc["rpm"]       | 0.0f;
    ws.calc_uh  = doc["calc_uh"]   | 0.0f;
    ws.layer    = doc["layer"]     | 1;
    ws.trav_mm  = doc["trav_mm"]   | 0.0f;
    ws.state    = doc["state"]     | 0;
    ws.fault    = doc["fault"]     | 0;
    ws.homed    = doc["homed"]     | false;
    ws.awg      = doc["awg"]       | 22;
    ws.former_mm= doc["former_mm"] | 25.4f;
    ws.layers   = doc["layers"]    | 1;
    const char* fw = doc["fw"];
    if (fw) strncpy(ws.fw, fw, 15);
    ws.age_ms = 0;
    last_rx   = millis();

    rpm_hist[rpm_head++ % 60] = ws.rpm;

    if (ws.state == 5) {  // DONE
        if (log_n < 20) {
            log_buf[log_n++] = {ws.turns, ws.calc_uh, ws.awg, ws.former_mm};
        }
    }
}

void sendCmd(const String& c) {
    if (btMaster.hasClient()) btMaster.println(c);
    Serial.println("BT>> " + c);
}

// ─── Header ───────────────────────────────────────────────────────────────────
void drawHeader() {
    tft.fillRect(0, 0, SCR_W, HDR_H, COL_HEADER);
    tft.setTextSize(1);
    tft.setTextColor(btConnected ? COL_GREEN : COL_RED, COL_HEADER);
    tft.setCursor(4, 6);
    tft.print(btConnected ? "BT:OK" : "BT:--");

    tft.setTextColor(COL_CYAN, COL_HEADER);
    tft.setCursor(90, 6);
    tft.print("RF-CoilWinder");

    const char* state_str[] = {"IDLE","WIND","PAUSE","HOME","FAULT","DONE"};
    int s = constrain(ws.state, 0, 5);
    uint16_t sc = (s == 4) ? COL_RED : (s == 1) ? COL_GREEN : COL_YELLOW;
    tft.setTextColor(sc, COL_HEADER);
    tft.setCursor(240, 6);
    tft.print(state_str[s]);
}

// ─── Tab bar ──────────────────────────────────────────────────────────────────
const char* tab_names[] = {"WIND","SETUP","CALC","LOG"};
void drawTabBar() {
    int y = SCR_H - TAB_H - BTN_H;
    int tw = SCR_W / MODE_COUNT;
    for (int i = 0; i < MODE_COUNT; i++) {
        uint16_t bg = (i == (int)mode) ? COL_TAB_A : COL_TAB_I;
        tft.fillRect(i * tw, y, tw - 1, TAB_H, bg);
        tft.setTextColor(COL_TEXT, bg);
        tft.setTextSize(1);
        int tx = i * tw + (tw - strlen(tab_names[i]) * 6) / 2;
        tft.setCursor(tx, y + 9);
        tft.print(tab_names[i]);
    }
}

// ─── Button bar ───────────────────────────────────────────────────────────────
struct BtnDef { const char* lbl; uint16_t col; };
BtnDef wind_btns[]  = {{"START",COL_GREEN},{"PAUSE",COL_YELLOW},{"STOP",COL_RED},{"HOME",COL_ORANGE},{"CLEAR",COL_DIM}};
BtnDef setup_btns[] = {{"▲INC", COL_CYAN}, {"▼DEC", COL_CYAN}, {"SEND",COL_GREEN},{"CALC",COL_YELLOW},{"BACK",COL_DIM}};
BtnDef calc_btns[]  = {{"N+",   COL_CYAN}, {"N-",   COL_CYAN}, {"D+",  COL_ORANGE},{"D-",COL_ORANGE},{"CALC",COL_GREEN}};
BtnDef log_btns[]   = {{"CLEAR",COL_RED},  {"",     COL_DIM},  {"",    COL_DIM},   {"",  COL_DIM},  {"BACK",COL_DIM}};

void drawButtonBar(BtnDef* btns, int n) {
    int y = SCR_H - BTN_H;
    int bw = SCR_W / n;
    for (int i = 0; i < n; i++) {
        if (!btns[i].lbl[0]) continue;
        tft.fillRect(i * bw, y, bw - 1, BTN_H, btns[i].col);
        tft.setTextColor(TFT_BLACK, btns[i].col);
        tft.setTextSize(1);
        int tx = i * bw + (bw - strlen(btns[i].lbl) * 6) / 2;
        tft.setCursor(tx, y + 9);
        tft.print(btns[i].lbl);
    }
}

// ─── Progress arc ─────────────────────────────────────────────────────────────
void drawTurnArc(int cx, int cy, int r, float frac) {
    frac = constrain(frac, 0, 1);
    int a_end = (int)(frac * 270);
    for (int a = 0; a <= 270; a += 2) {
        float rad = (a - 135) * M_PI / 180.0f;
        int x = cx + (int)(r * cosf(rad));
        int y = cy + (int)(r * sinf(rad));
        uint16_t c = (a <= a_end) ? COL_PROGRESS : COL_DIM;
        tft.drawPixel(x, y, c);
        tft.drawPixel(x+1, y, c);
    }
}

// ─── WIND mode ────────────────────────────────────────────────────────────────
void drawWindMode() {
    int y = WORK_Y + 4;
    char buf[32];

    // Large turn display
    tft.setTextSize(3);
    uint16_t c = (ws.state == 4) ? COL_RED : (ws.state == 5) ? COL_GREEN : COL_CYAN;
    tft.setTextColor(c, COL_BG);
    snprintf(buf, sizeof(buf), "%6lu", ws.turns);
    tft.setCursor(10, y + 2);
    tft.print(buf);

    // Target and progress
    tft.setTextSize(1);
    tft.setTextColor(COL_DIM, COL_BG);
    tft.setCursor(10, y + 30);
    snprintf(buf, sizeof(buf), "of %lu turns", ws.target);
    tft.print(buf);

    // Progress arc (right side)
    float frac = (ws.target > 0) ? (float)ws.turns / ws.target : 0;
    int cx = 260, cy = y + 48, r = 44;
    tft.fillCircle(cx, cy, r + 3, COL_BG);
    drawTurnArc(cx, cy, r, frac);
    tft.setTextSize(2);
    tft.setTextColor(COL_TEXT, COL_BG);
    snprintf(buf, sizeof(buf), "%2d%%", (int)(frac * 100));
    tft.setCursor(cx - 14, cy - 8);
    tft.print(buf);

    // Inductance
    tft.setTextSize(2);
    tft.setTextColor(COL_YELLOW, COL_BG);
    snprintf(buf, sizeof(buf), "%.2fuH", ws.calc_uh);
    tft.setCursor(10, y + 48);
    tft.print(buf);

    // Info row
    tft.setTextSize(1);
    tft.setTextColor(COL_DIM, COL_BG);
    tft.setCursor(4, y + 80);
    snprintf(buf, sizeof(buf), "RPM:%.0f  AWG%d  D:%.0fmm",
             ws.rpm, ws.awg, ws.former_mm);
    tft.print(buf);

    tft.setCursor(4, y + 92);
    snprintf(buf, sizeof(buf), "Layer:%d/%d  Trav:%.1fmm",
             ws.layer, ws.layers, ws.trav_mm);
    tft.print(buf);

    // RPM sparkline
    int sx = 4, sy = y + 108, sw = SCR_W - 8, sh = 28;
    tft.drawRect(sx, sy, sw, sh, COL_DIM);
    float rmax = 1.0f;
    for (int i = 0; i < 60; i++) rmax = max(rmax, rpm_hist[i]);
    for (int i = 1; i < 60; i++) {
        int i0 = (rpm_head - 60 + i - 1 + 60) % 60;
        int i1 = (rpm_head - 60 + i + 60) % 60;
        int x0 = sx + (i-1) * sw / 59;
        int x1 = sx + i * sw / 59;
        int y0 = sy + sh - 1 - (int)(rpm_hist[i0] / rmax * (sh - 2));
        int y1 = sy + sh - 1 - (int)(rpm_hist[i1] / rmax * (sh - 2));
        tft.drawLine(x0, y0, x1, y1, COL_CYAN);
    }

    // Fault overlay
    if (ws.state == 4) {
        const char* flt[] = {"","WIRE BREAK","OVERRUN","TRAV LIMIT","E-STOP"};
        tft.setTextColor(COL_RED, COL_BG);
        tft.setTextSize(2);
        tft.setCursor(20, y + 140);
        tft.print("FAULT: ");
        if (ws.fault >= 1 && ws.fault <= 4) tft.print(flt[ws.fault]);
    }

    drawButtonBar(wind_btns, 5);
}

// ─── SETUP mode ───────────────────────────────────────────────────────────────
void drawSetupMode() {
    int y = WORK_Y + 4;
    tft.setTextSize(1);

    for (int i = 0; i < N_PARAMS; i++) {
        uint16_t bg = (i == param_sel) ? COL_TAB_A : COL_BG;
        uint16_t fg = (i == param_sel) ? COL_TEXT : COL_DIM;
        if (i == param_sel) tft.fillRect(0, y + i * 18, SCR_W, 17, bg);
        tft.setTextColor(fg, bg);
        char buf[32];
        snprintf(buf, sizeof(buf), "%-10s: %6.1f %s",
            params[i].label, *params[i].valptr, params[i].unit);
        tft.setCursor(4, y + i * 18 + 4);
        tft.print(buf);
    }

    drawButtonBar(setup_btns, 5);
}

// ─── CALC mode ────────────────────────────────────────────────────────────────
void drawCalcMode() {
    int y = WORK_Y + 4;
    calc_result = wheelerSingle(calc_d_in, calc_n, calc_l_in);
    char buf[48];
    tft.setTextSize(1);
    tft.setTextColor(COL_DIM, COL_BG);

    tft.setCursor(4, y);
    snprintf(buf, sizeof(buf), "Wheeler Single-Layer Calculator");
    tft.print(buf);

    tft.setCursor(4, y + 14);
    snprintf(buf, sizeof(buf), "Diameter D: %.2f in (%.1f mm)",
        calc_d_in, calc_d_in * 25.4f);
    tft.setTextColor(COL_TEXT, COL_BG);
    tft.print(buf);

    tft.setCursor(4, y + 26);
    snprintf(buf, sizeof(buf), "Turns N:    %.0f", calc_n);
    tft.print(buf);

    tft.setCursor(4, y + 38);
    snprintf(buf, sizeof(buf), "Length L:   %.2f in (%.1f mm)",
        calc_l_in, calc_l_in * 25.4f);
    tft.print(buf);

    // Result
    tft.fillRect(0, y + 55, SCR_W, 40, 0x0010);
    tft.setTextSize(2);
    tft.setTextColor(COL_YELLOW, 0x0010);
    snprintf(buf, sizeof(buf), "L = %.3f uH", calc_result);
    tft.setCursor(10, y + 63);
    tft.print(buf);

    tft.setTextSize(1);
    tft.setTextColor(COL_CYAN, 0x0010);
    float xL_14 = 2.0f * M_PI * 14.0e6f * calc_result * 1e-6f;
    snprintf(buf, sizeof(buf), "Xl@14MHz = %.0f ohm", xL_14);
    tft.setCursor(10, y + 83);
    tft.print(buf);

    // Formula reminder
    tft.setTextSize(1);
    tft.setTextColor(COL_DIM, COL_BG);
    tft.setCursor(4, y + 105);
    tft.print("L = d^2*N^2 / (18d + 40l)  [inches, uH]");

    drawButtonBar(calc_btns, 5);
}

// ─── LOG mode ─────────────────────────────────────────────────────────────────
void drawLogMode() {
    int y = WORK_Y + 4;
    tft.setTextSize(1);
    if (log_n == 0) {
        tft.setTextColor(COL_DIM, COL_BG);
        tft.setCursor(40, y + 60);
        tft.print("No completed windings yet");
    } else {
        tft.setTextColor(COL_DIM, COL_BG);
        tft.setCursor(4, y);
        tft.print("#   Turns    L(uH)  AWG  D(mm)");
        for (int i = 0; i < min(log_n, 8); i++) {
            char buf[40];
            tft.setTextColor(COL_TEXT, COL_BG);
            snprintf(buf, sizeof(buf), "%-2d  %-7lu  %-6.2f  %-3d  %.1f",
                i + 1, log_buf[i].turns, log_buf[i].uh,
                log_buf[i].awg, log_buf[i].dia_mm);
            tft.setCursor(4, y + 12 + i * 13);
            tft.print(buf);
        }
    }
    drawButtonBar(log_btns, 5);
}

// ─── Touch ────────────────────────────────────────────────────────────────────
uint32_t last_touch = 0;
void handleTouch(int tx, int ty) {
    if (millis() - last_touch < 280) return;
    last_touch = millis();

    int tab_y  = SCR_H - TAB_H - BTN_H;
    int btn_y  = SCR_H - BTN_H;

    if (ty >= tab_y && ty < tab_y + TAB_H) {
        int tw = SCR_W / MODE_COUNT;
        int m  = tx / tw;
        if (m >= 0 && m < MODE_COUNT) { mode = (Mode)m; dirty = true; }
        return;
    }

    if (ty >= btn_y) {
        int bw = SCR_W / 5;
        int b  = tx / bw;
        handleButton(b);
        return;
    }

    // Setup mode: tap row to select
    if (mode == MODE_SETUP) {
        int row = (ty - WORK_Y - 4) / 18;
        if (row >= 0 && row < N_PARAMS) { param_sel = row; dirty = true; }
    }
}

void handleButton(int b) {
    char buf[32];
    switch (mode) {
        case MODE_WIND:
            switch (b) {
                case 0: sendCmd("START"); break;
                case 1: sendCmd(ws.state == 2 ? "RESUME" : "PAUSE"); break;
                case 2: sendCmd("STOP"); break;
                case 3: sendCmd("HOME"); break;
                case 4: sendCmd("FAULT"); break;
            }
            break;

        case MODE_SETUP:
            switch (b) {
                case 0: *params[param_sel].valptr = min(*params[param_sel].valptr + params[param_sel].step, params[param_sel].max_v); dirty = true; break;
                case 1: *params[param_sel].valptr = max(*params[param_sel].valptr - params[param_sel].step, params[param_sel].min_v); dirty = true; break;
                case 2:
                    // Send all params to controller
                    snprintf(buf, sizeof(buf), "TURNS %d", (int)sp_turns); sendCmd(buf);
                    snprintf(buf, sizeof(buf), "AWG %d",   (int)sp_awg);   sendCmd(buf);
                    snprintf(buf, sizeof(buf), "DIA %.1f", sp_dia);        sendCmd(buf);
                    snprintf(buf, sizeof(buf), "LEN %.1f", sp_len);        sendCmd(buf);
                    snprintf(buf, sizeof(buf), "LAYERS %d",(int)sp_layers);sendCmd(buf);
                    snprintf(buf, sizeof(buf), "PITCH %.2f",sp_pitch);     sendCmd(buf);
                    if (sp_uh > 0) { snprintf(buf, sizeof(buf), "L %.2f", sp_uh); sendCmd(buf); }
                    break;
                case 3: sendCmd("CALC"); break;
                case 4: mode = MODE_WIND; dirty = true; break;
            }
            break;

        case MODE_CALC:
            switch (b) {
                case 0: calc_n  = min(calc_n + 1, 1000.0f); dirty = true; break;
                case 1: calc_n  = max(calc_n - 1, 1.0f);    dirty = true; break;
                case 2: calc_d_in = min(calc_d_in + 0.125f, 8.0f); dirty = true; break;
                case 3: calc_d_in = max(calc_d_in - 0.125f, 0.125f); dirty = true; break;
                case 4: dirty = true; break;
            }
            break;

        case MODE_LOG:
            if (b == 0) { log_n = 0; dirty = true; }
            if (b == 4) { mode = MODE_WIND; dirty = true; }
            break;

        default: break;
    }
}

// ─── RGB LED ──────────────────────────────────────────────────────────────────
void updateLED() {
    static uint32_t last_blink = 0;
    static bool     st = false;
    int period = btConnected ? (ws.state == 1 ? 300 : ws.state == 4 ? 80 : 2000) : 150;
    if (millis() - last_blink > (uint32_t)period) {
        last_blink = millis(); st = !st;
        digitalWrite(RGB_R, (ws.state == 4 && st) ? LOW : HIGH);
        digitalWrite(RGB_G, (ws.state == 1 && st && btConnected) ? LOW : HIGH);
        digitalWrite(RGB_B, (!btConnected && st) ? LOW : HIGH);
    }
}

// ─── Redraw ───────────────────────────────────────────────────────────────────
uint32_t last_redraw = 0;
void redraw() {
    tft.fillScreen(COL_BG);
    drawHeader();
    drawTabBar();
    switch (mode) {
        case MODE_WIND:  drawWindMode();  break;
        case MODE_SETUP: drawSetupMode(); break;
        case MODE_CALC:  drawCalcMode();  break;
        case MODE_LOG:   drawLogMode();   break;
        default: break;
    }
    dirty = false;
}

// ─── Partial update (WIND mode live data) ────────────────────────────────────
void updateWindPartial() {
    if (mode != MODE_WIND) return;
    int y = WORK_Y + 4;
    char buf[32];

    tft.setTextSize(3);
    uint16_t c = (ws.state == 4) ? COL_RED : (ws.state == 5) ? COL_GREEN : COL_CYAN;
    tft.setTextColor(c, COL_BG);
    snprintf(buf, sizeof(buf), "%6lu", ws.turns);
    tft.setCursor(10, y + 2);
    tft.print(buf);

    tft.setTextSize(2);
    tft.setTextColor(COL_YELLOW, COL_BG);
    snprintf(buf, sizeof(buf), "%.2fuH", ws.calc_uh);
    tft.setCursor(10, y + 48);
    tft.print(buf);

    // Redraw arc
    float frac = (ws.target > 0) ? (float)ws.turns / ws.target : 0;
    int cx = 260, cy = y + 48, r = 44;
    tft.fillCircle(cx, cy, r + 3, COL_BG);
    drawTurnArc(cx, cy, r, frac);
    tft.setTextSize(2);
    tft.setTextColor(COL_TEXT, COL_BG);
    snprintf(buf, sizeof(buf), "%2d%%", (int)(frac * 100));
    tft.setCursor(cx - 14, cy - 8);
    tft.print(buf);

    tft.setTextSize(1);
    tft.fillRect(0, y + 78, SCR_W, 22, COL_BG);
    tft.setTextColor(COL_DIM, COL_BG);
    tft.setCursor(4, y + 80);
    snprintf(buf, sizeof(buf), "RPM:%.0f  AWG%d  D:%.0fmm", ws.rpm, ws.awg, ws.former_mm);
    tft.print(buf);
    tft.setCursor(4, y + 92);
    snprintf(buf, sizeof(buf), "Layer:%d/%d  Trav:%.1fmm", ws.layer, ws.layers, ws.trav_mm);
    tft.print(buf);

    drawHeader();
}

// ─── setup() ─────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    pinMode(RGB_R, OUTPUT); digitalWrite(RGB_R, HIGH);
    pinMode(RGB_G, OUTPUT); digitalWrite(RGB_G, HIGH);
    pinMode(RGB_B, OUTPUT); digitalWrite(RGB_B, HIGH);

    SPI.begin();
    touch.begin(); touch.setRotation(1);
    tft.init(); tft.setRotation(1); tft.fillScreen(COL_BG);

    tft.setTextColor(COL_CYAN); tft.setTextSize(2);
    tft.setCursor(50, 80); tft.println("RF-CoilWinder");
    tft.setTextSize(1); tft.setTextColor(COL_DIM);
    tft.setCursor(80, 108); tft.println("TM-CWND-CYD-001");
    delay(1200);

    btMaster.begin("CYD-WindDisplay", true);
    btMaster.connect("RF-CoilWinder");

    memset(rpm_hist, 0, sizeof(rpm_hist));
    dirty = true;
}

// ─── loop() ──────────────────────────────────────────────────────────────────
void loop() {
    bool was_connected = btConnected;
    btConnected = btMaster.hasClient();
    if (btConnected && !was_connected) { dirty = true; }
    if (!btConnected && was_connected) {
        dirty = true;
        delay(2000);
        btMaster.connect("RF-CoilWinder");
    }

    while (btMaster.available()) {
        char c = btMaster.read();
        if (c == '\n' || c == '\r') {
            if (btBuf.length()) {
                parseStatus(btBuf); btBuf = "";
                if (mode == MODE_WIND && !dirty) updateWindPartial();
                else dirty = true;
            }
        } else btBuf += c;
    }

    ws.age_ms = millis() - last_rx;

    if (dirty || millis() - last_redraw > 5000) {
        redraw(); last_redraw = millis();
    }

    if (touch.tirqTouched() && touch.touched()) {
        TS_Point p = touch.getPoint();
        handleTouch(map(p.x, 200, 3900, 0, SCR_W),
                    map(p.y, 200, 3900, 0, SCR_H));
    }

    updateLED();
}
