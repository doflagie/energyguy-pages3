/**
 * cyd_cap_display.ino — TM-VCAP-CYD-001 Rev A
 * CYD (Cheap Yellow Display) Touchscreen Interface
 * for Motorized Variable Capacitor Controller
 *
 * Hardware:
 *   ESP32-2432S028 (CYD): ILI9341 320×240 TFT, XPT2046 touch, RGB LED
 *   Bluetooth SPP master → connects to "RF-CapTuner" (esp32_cap_tuner)
 *
 * Display modes (tab bar across bottom):
 *   [LIVE]  — Large capacitance readout, angle arc, pot/stepper comparison
 *   [BAND]  — 10-band preset grid, tap to load
 *   [SWEEP] — XY chart: capacitance sweep trace
 *   [INFO]  — Plate geometry, voltage ratings, firmware version
 *
 * Libraries:
 *   TFT_eSPI (User_Setup: CYD preset), XPT2046_Touchscreen, BluetoothSerial, ArduinoJson
 */

#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>

// ─── Pin Definitions (CYD) ────────────────────────────────────────────────────

#define XPT_CS   33
#define XPT_IRQ  36
#define RGB_R    4
#define RGB_G    16
#define RGB_B    17
#define CYD_LDR  34

// ─── Display Geometry ─────────────────────────────────────────────────────────

#define SCR_W    320
#define SCR_H    240
#define TAB_H    30    // Tab bar height (bottom)
#define BTN_H    30    // Button bar height (very bottom)
#define HEADER_H 20    // Top header bar

#define PLOT_X   10
#define PLOT_Y   (HEADER_H + 4)
#define PLOT_W   (SCR_W - 20)
#define PLOT_H   (SCR_H - HEADER_H - TAB_H - BTN_H - 8)

// ─── Colors ───────────────────────────────────────────────────────────────────

#define COL_BG       TFT_BLACK
#define COL_HEADER   0x1082      // Dark blue-grey
#define COL_TAB_ACT  0x02B5      // Active tab: teal
#define COL_TAB_IDLE 0x2945      // Inactive tab
#define COL_TEXT     TFT_WHITE
#define COL_DIM      0x8410      // Dim grey
#define COL_GREEN    TFT_GREEN
#define COL_YELLOW   TFT_YELLOW
#define COL_ORANGE   0xFD20      // Orange
#define COL_RED      TFT_RED
#define COL_CYAN     TFT_CYAN
#define COL_MAGENTA  TFT_MAGENTA
#define COL_VOLT_OK  0x07E0      // Bright green — safe voltage
#define COL_VOLT_WARN 0xFD20     // Orange — caution
#define COL_VOLT_DANGER TFT_RED  // Red — exceeds rating

// ─── Capacitor color ramp (low → high C) ──────────────────────────────────────

uint16_t capColor(float c_pf, float c_min, float c_max) {
    float frac = constrain((c_pf - c_min) / (c_max - c_min), 0.0f, 1.0f);
    // Blue (low C) → Cyan → Green → Yellow → Red (high C)
    if (frac < 0.25f) return TFT_BLUE;
    if (frac < 0.50f) return TFT_CYAN;
    if (frac < 0.75f) return COL_GREEN;
    if (frac < 0.90f) return COL_YELLOW;
    return COL_RED;
}

// ─── Globals ──────────────────────────────────────────────────────────────────

TFT_eSPI          tft;
XPT2046_Touchscreen touch(XPT_CS, XPT_IRQ);
BluetoothSerial   btMaster;

enum DisplayMode { MODE_LIVE, MODE_BAND, MODE_SWEEP, MODE_INFO, MODE_COUNT };
DisplayMode       mode       = MODE_LIVE;
bool              btConnected = false;
bool              dirty       = true;     // Full redraw needed

// ─── Received Status ──────────────────────────────────────────────────────────

struct CapStatus {
    float  c_pf       = 0;
    float  angle      = 0;
    float  pot_c      = 0;
    float  pot_ang    = 0;
    long   steps      = 0;
    int    state      = 0;  // 0=IDLE,1=MOVING,2=HOMING,3=FAULT,4=UNCAL
    bool   homed      = false;
    bool   moving     = false;
    bool   fault      = false;
    float  pos_err    = 0;
    char   fw[16]     = "?.?.?";
    uint32_t age_ms   = 0;   // ms since last update
} cap;

// Sweep buffer
#define SWEEP_PTS 200
float sweep_buf[SWEEP_PTS];
int   sweep_n = 0;
bool  sweep_active = false;

// Sparkline (30s of C history at ~2Hz = 60 points)
#define SPARK_N 60
float spark_buf[SPARK_N];
int   spark_head = 0;

// BT receive buffer
String btBuf = "";
uint32_t last_rx = 0;

// ─── BT Connection ────────────────────────────────────────────────────────────

void connectBT() {
    tft.fillRect(0, HEADER_H + 40, SCR_W, 30, COL_BG);
    tft.setTextColor(COL_YELLOW, COL_BG);
    tft.setTextSize(1);
    tft.setCursor(60, HEADER_H + 48);
    tft.print("Connecting to RF-CapTuner...");
    btMaster.connect("RF-CapTuner");
}

// ─── Parse JSON Status ────────────────────────────────────────────────────────

void parseStatus(const String& json) {
    StaticJsonDocument<256> doc;
    if (deserializeJson(doc, json) != DeserializationError::Ok) return;

    cap.c_pf    = doc["c_pf"]   | 0.0f;
    cap.angle   = doc["angle"]  | 0.0f;
    cap.pot_c   = doc["pot_c"]  | 0.0f;
    cap.pot_ang = doc["pot_ang"]| 0.0f;
    cap.steps   = doc["steps"]  | 0L;
    cap.state   = doc["state"]  | 0;
    cap.homed   = doc["homed"]  | false;
    cap.moving  = doc["moving"] | false;
    cap.fault   = doc["fault"]  | false;
    cap.pos_err = doc["pos_err"]| 0.0f;
    const char* fw = doc["fw"];
    if (fw) strncpy(cap.fw, fw, 15);
    cap.age_ms  = 0;
    last_rx = millis();

    // Push to sparkline
    spark_buf[spark_head % SPARK_N] = cap.c_pf;
    spark_head++;

    // Collect sweep
    if (sweep_active && sweep_n < SWEEP_PTS)
        sweep_buf[sweep_n++] = cap.c_pf;
}

void sendCommand(const String& cmd) {
    if (btMaster.hasClient()) btMaster.println(cmd);
}

// ─── Header Bar ───────────────────────────────────────────────────────────────

void drawHeader() {
    tft.fillRect(0, 0, SCR_W, HEADER_H, COL_HEADER);
    tft.setTextColor(COL_TEXT, COL_HEADER);
    tft.setTextSize(1);

    // Left: BT status
    tft.setCursor(4, 6);
    if (btConnected) {
        tft.setTextColor(COL_GREEN, COL_HEADER);
        tft.print("BT:OK");
    } else {
        tft.setTextColor(COL_RED, COL_HEADER);
        tft.print("BT:--");
    }

    // Center: title
    tft.setTextColor(COL_CYAN, COL_HEADER);
    tft.setCursor(100, 6);
    tft.print("RF-CapTuner");

    // Right: state
    const char* state_str[] = {"IDLE","MOVE","HOME","FAULT","UNCAL"};
    tft.setCursor(230, 6);
    if (cap.fault) {
        tft.setTextColor(COL_RED, COL_HEADER);
        tft.print("FAULT");
    } else if (!cap.homed) {
        tft.setTextColor(COL_YELLOW, COL_HEADER);
        tft.print("UNCAL");
    } else {
        tft.setTextColor(COL_GREEN, COL_HEADER);
        tft.print(state_str[constrain(cap.state, 0, 4)]);
    }
}

// ─── Tab Bar ──────────────────────────────────────────────────────────────────

const char* tab_labels[] = { "LIVE", "BAND", "SWEEP", "INFO" };

void drawTabBar() {
    int y = SCR_H - TAB_H - BTN_H;
    int tab_w = SCR_W / MODE_COUNT;
    for (int i = 0; i < MODE_COUNT; i++) {
        uint16_t bg = (i == (int)mode) ? COL_TAB_ACT : COL_TAB_IDLE;
        tft.fillRect(i * tab_w, y, tab_w - 1, TAB_H, bg);
        tft.setTextColor(COL_TEXT, bg);
        tft.setTextSize(1);
        int tx = i * tab_w + (tab_w - strlen(tab_labels[i]) * 6) / 2;
        tft.setCursor(tx, y + 10);
        tft.print(tab_labels[i]);
    }
}

// ─── Button Bar ───────────────────────────────────────────────────────────────

struct Btn {
    const char* label;
    uint16_t    color;
};

Btn live_btns[] = { {"HOME", COL_ORANGE}, {"C-", COL_CYAN}, {"C+", COL_CYAN},
                    {"STOP", COL_RED},    {"CAL", COL_YELLOW} };
Btn band_btns[] = { {"HOME", COL_ORANGE}, {"<<", COL_CYAN}, {">>", COL_CYAN},
                    {"LOAD", COL_GREEN},  {"BACK", COL_DIM} };
Btn sweep_btns[]= { {"HOME", COL_ORANGE}, {"SWSTART", COL_GREEN}, {"SWSTOP", COL_RED},
                    {"CLEAR", COL_DIM},   {"BACK", COL_DIM} };
Btn info_btns[] = { {"HOME", COL_ORANGE}, {"RESET", COL_RED}, {"STATUS", COL_CYAN},
                    {"CALPOT", COL_YELLOW},{"BACK", COL_DIM} };

void drawButtonBar(Btn* btns, int n) {
    int y = SCR_H - BTN_H;
    int bw = SCR_W / n;
    for (int i = 0; i < n; i++) {
        tft.fillRect(i * bw, y, bw - 1, BTN_H, btns[i].color);
        tft.setTextColor(TFT_BLACK, btns[i].color);
        tft.setTextSize(1);
        int tx = i * bw + (bw - strlen(btns[i].label) * 6) / 2;
        tft.setCursor(tx, y + 10);
        tft.print(btns[i].label);
    }
}

// ─── Voltage Warning Color ────────────────────────────────────────────────────

// Estimate peak RF voltage: V = sqrt(2 * P * Xc)
// Xc = 1 / (2π × f × C)
// Reference 100W at 14 MHz with C in pF
float estimatePeakV(float c_pf, float freq_mhz, float power_w) {
    if (c_pf < 1.0f) return 0;
    float xc = 1.0f / (2.0f * M_PI * freq_mhz * 1e6f * c_pf * 1e-12f);
    return sqrtf(2.0f * power_w * xc);
}

uint16_t voltColor(float v_peak, float v_rating_kv) {
    float ratio = v_peak / (v_rating_kv * 1000.0f);
    if (ratio < 0.5f) return COL_VOLT_OK;
    if (ratio < 0.8f) return COL_VOLT_WARN;
    return COL_VOLT_DANGER;
}

// ─── LIVE mode ────────────────────────────────────────────────────────────────

void drawCapArc(float c_pf, float c_min, float c_max) {
    // Draw semicircular arc representing rotation (0° = top, 180° = clockwise)
    int cx = SCR_W / 2;
    int cy = HEADER_H + 85;
    int r  = 55;

    // Background arc (full 180°)
    for (int a = 0; a <= 180; a += 2) {
        float rad = (a - 90) * M_PI / 180.0f;
        int x = cx + (int)(r * cosf(rad));
        int y = cy + (int)(r * sinf(rad));
        tft.drawPixel(x, y, COL_DIM);
        tft.drawPixel(x+1, y, COL_DIM);
    }

    // Active arc
    float frac = constrain((c_pf - c_min) / (c_max - c_min), 0, 1);
    int   a_end = (int)(frac * 180);
    uint16_t col = capColor(c_pf, c_min, c_max);
    for (int a = 0; a <= a_end; a++) {
        float rad = (a - 90) * M_PI / 180.0f;
        int x = cx + (int)(r * cosf(rad));
        int y = cy + (int)(r * sinf(rad));
        tft.drawPixel(x, y, col);
        tft.drawPixel(x+1, y, col);
    }

    // Needle
    float rad_n = (a_end - 90) * M_PI / 180.0f;
    int nx = cx + (int)((r - 8) * cosf(rad_n));
    int ny = cy + (int)((r - 8) * sinf(rad_n));
    tft.drawLine(cx, cy, nx, ny, COL_TEXT);

    // Min/Max labels
    tft.setTextColor(COL_DIM, COL_BG);
    tft.setTextSize(1);
    tft.setCursor(cx - r - 4, cy + 2);
    tft.print("MIN");
    tft.setCursor(cx + r - 10, cy + 2);
    tft.print("MAX");
}

void drawSparkline(int x, int y, int w, int h) {
    // Find min/max in buffer
    float mn = 1e9f, mx = -1e9f;
    int count = min(spark_head, SPARK_N);
    for (int i = 0; i < count; i++) {
        mn = min(mn, spark_buf[i]);
        mx = max(mx, spark_buf[i]);
    }
    if (mx - mn < 1.0f) mx = mn + 1.0f;

    tft.drawRect(x, y, w, h, COL_DIM);

    for (int i = 1; i < count; i++) {
        int idx0 = (spark_head - count + i - 1 + SPARK_N) % SPARK_N;
        int idx1 = (spark_head - count + i + SPARK_N) % SPARK_N;
        int x0 = x + (i - 1) * w / (SPARK_N - 1);
        int x1 = x + i * w / (SPARK_N - 1);
        int y0 = y + h - 1 - (int)((spark_buf[idx0] - mn) / (mx - mn) * (h - 2));
        int y1 = y + h - 1 - (int)((spark_buf[idx1] - mn) / (mx - mn) * (h - 2));
        tft.drawLine(x0, y0, x1, y1, COL_CYAN);
    }
}

void drawLiveMode() {
    int y = HEADER_H + 4;

    // Large capacitance value
    tft.setTextSize(3);
    uint16_t col = capColor(cap.c_pf, 10, 250);
    tft.setTextColor(col, COL_BG);
    char buf[24];
    snprintf(buf, sizeof(buf), "%6.1f pF", cap.c_pf);
    tft.setCursor(30, y + 4);
    tft.print(buf);

    // Position error indicator (red if >5°)
    tft.setTextSize(1);
    if (cap.pos_err > 5.0f) {
        tft.setTextColor(COL_RED, COL_BG);
        tft.setCursor(220, y + 10);
        snprintf(buf, sizeof(buf), "ERR:%.1f", cap.pos_err);
        tft.print(buf);
    }

    // Rotation arc
    drawCapArc(cap.c_pf, 10, 250);

    // Pot vs stepper comparison
    int py = y + 100;
    tft.setTextSize(1);
    tft.setTextColor(COL_DIM, COL_BG);
    tft.setCursor(4, py);
    tft.print("Stepper:");
    tft.setTextColor(COL_TEXT, COL_BG);
    snprintf(buf, sizeof(buf), "%.1f pF / %.1f deg", cap.c_pf, cap.angle);
    tft.print(buf);

    tft.setCursor(4, py + 12);
    tft.setTextColor(COL_DIM, COL_BG);
    tft.print("Pot:    ");
    tft.setTextColor(COL_CYAN, COL_BG);
    snprintf(buf, sizeof(buf), "%.1f pF / %.1f deg", cap.pot_c, cap.pot_ang);
    tft.print(buf);

    // Sparkline
    drawSparkline(4, py + 26, SCR_W - 8, 30);

    // Voltage warning (estimate at 14 MHz, 100W reference)
    float vpeak = estimatePeakV(cap.c_pf, 14.0f, 100.0f);
    uint16_t vc = voltColor(vpeak, 5.0f);
    tft.setCursor(4, py + 60);
    tft.setTextColor(vc, COL_BG);
    snprintf(buf, sizeof(buf), "Xc@14MHz: %.0f ohm  Vpk@100W: %.0f V",
             1.0f / (2.0f * M_PI * 14e6f * cap.c_pf * 1e-12f), vpeak);
    tft.print(buf);

    drawButtonBar(live_btns, 5);
}

// ─── BAND mode ────────────────────────────────────────────────────────────────

// 16 bands: HF (160m-10m), VHF (6m-1.25m), UHF (70cm-20cm)
static const char* band_names[] = {
    "160m","80m","60m","40m","30m","20m","17m","15m",
    "12m","10m","6m","2m","1.25m","70cm","33cm","20cm"
};
static const float band_c[] = {
    220, 160, 100, 70, 50, 32, 24, 18,
    14, 11, 18, 8.5f, 5.5f, 4.0f, 2.5f, 1.5f
};
static const float band_freq[] = {
    1.9f, 3.7f, 5.3f, 7.1f, 10.1f, 14.2f, 18.1f, 21.2f,
    24.9f, 28.5f, 51.0f, 146.0f, 222.0f, 446.0f, 906.0f, 1296.0f
};
#define N_BANDS_CYD 16
static int  band_sel = 0;

// Cap design type per band (for voltage rating lookup)
// 0=QRP/HF100, 1=HF100, 2=TX, 3=VHF, 4=UHF
static const float band_v_rating_kv[] = {
    1.5f,1.5f,1.5f,1.5f,1.5f,1.5f,1.5f,1.5f,
    1.5f,1.5f,0.5f,0.5f,0.5f,0.2f,0.2f,0.2f
};

void drawBandMode() {
    int y = HEADER_H + 4;
    int cols = 4, rows = 4;
    int bw = SCR_W / cols;
    int bh = 28;

    tft.setTextSize(1);
    for (int i = 0; i < N_BANDS_CYD; i++) {
        int col = i % cols, row = i / cols;
        int bx = col * bw, by = y + row * (bh + 1);
        // Color: green=HF, cyan=VHF, magenta=UHF; brighter=selected
        uint16_t bg;
        if (i == band_sel)      bg = COL_TAB_ACT;
        else if (i < 10)        bg = COL_HEADER;
        else if (i < 13)        bg = 0x0245;   // dark teal = VHF
        else                    bg = 0x3003;   // dark purple = UHF
        tft.fillRect(bx, by, bw - 1, bh, bg);
        tft.setTextColor(COL_TEXT, bg);
        tft.setCursor(bx + 2, by + 3);
        tft.print(band_names[i]);
        tft.setCursor(bx + 2, by + 14);
        char buf[12];
        snprintf(buf, sizeof(buf), "%.1fpF", band_c[i]);
        tft.setTextColor(i < 10 ? COL_CYAN : (i < 13 ? COL_GREEN : COL_MAGENTA), bg);
        tft.print(buf);
    }

    // Selected band detail below grid
    int dy = y + rows * (bh + 1) + 4;
    tft.setTextColor(COL_TEXT, COL_BG);
    char buf[48];
    snprintf(buf, sizeof(buf), "%s  %.1fMHz  C=%.1fpF  Vmax=%.0fkV",
        band_names[band_sel], band_freq[band_sel], band_c[band_sel],
        band_v_rating_kv[band_sel]);
    tft.setCursor(2, dy);
    tft.setTextSize(1);
    tft.print(buf);

    drawButtonBar(band_btns, 5);
}

// ─── SWEEP mode ───────────────────────────────────────────────────────────────

void drawSweepMode() {
    int px = PLOT_X, py = PLOT_Y;
    int pw = PLOT_W, ph = PLOT_H - 20;

    tft.drawRect(px, py, pw, ph, COL_DIM);

    if (sweep_n < 2) {
        tft.setTextColor(COL_DIM, COL_BG);
        tft.setTextSize(1);
        tft.setCursor(px + 40, py + ph / 2 - 4);
        tft.print("Press SWSTART to begin sweep");
        drawButtonBar(sweep_btns, 5);
        return;
    }

    // Find range
    float mn = 1e9f, mx = -1e9f;
    for (int i = 0; i < sweep_n; i++) {
        mn = min(mn, sweep_buf[i]);
        mx = max(mx, sweep_buf[i]);
    }
    if (mx - mn < 1.0f) mx = mn + 1.0f;

    // Plot
    for (int i = 1; i < sweep_n; i++) {
        int x0 = px + (i - 1) * pw / (sweep_n - 1);
        int x1 = px + i * pw / (sweep_n - 1);
        int y0 = py + ph - 1 - (int)((sweep_buf[i-1] - mn) / (mx - mn) * (ph - 2));
        int y1 = py + ph - 1 - (int)((sweep_buf[i]   - mn) / (mx - mn) * (ph - 2));
        uint16_t col = capColor(sweep_buf[i], mn, mx);
        tft.drawLine(x0, y0, x1, y1, col);
    }

    // Axis labels
    tft.setTextSize(1);
    tft.setTextColor(COL_DIM, COL_BG);
    char buf[16];
    snprintf(buf, sizeof(buf), "%.0fpF", mx);
    tft.setCursor(px + 2, py + 2);
    tft.print(buf);
    snprintf(buf, sizeof(buf), "%.0fpF", mn);
    tft.setCursor(px + 2, py + ph - 10);
    tft.print(buf);

    snprintf(buf, sizeof(buf), "n=%d", sweep_n);
    tft.setCursor(px + pw - 30, py + 2);
    tft.print(buf);

    drawButtonBar(sweep_btns, 5);
}

// ─── INFO mode ────────────────────────────────────────────────────────────────

void drawInfoMode() {
    int y = HEADER_H + 4;
    tft.setTextSize(1);
    tft.setTextColor(COL_TEXT, COL_BG);

    char buf[48];
    int ly = y;
    int lh = 13;

    auto line = [&](const char* label, const char* val, uint16_t col) {
        tft.setTextColor(COL_DIM, COL_BG);
        tft.setCursor(4, ly);
        tft.print(label);
        tft.setTextColor(col, COL_BG);
        tft.print(val);
        ly += lh;
    };

    snprintf(buf, sizeof(buf), "%.1f pF", cap.c_pf);   line("Capacitance: ", buf, COL_CYAN);
    snprintf(buf, sizeof(buf), "%.1f deg", cap.angle);  line("Angle:       ", buf, COL_TEXT);
    snprintf(buf, sizeof(buf), "%.1f pF", cap.pot_c);   line("Pot reading: ", buf, COL_CYAN);
    snprintf(buf, sizeof(buf), "%.2f deg", cap.pos_err);
    line("Pos error:   ", buf, cap.pos_err > 5.0f ? COL_RED : COL_GREEN);
    snprintf(buf, sizeof(buf), "%ld", cap.steps);       line("Steps:       ", buf, COL_DIM);
    snprintf(buf, sizeof(buf), "%s", cap.homed ? "YES" : "NO");
    line("Homed:       ", buf, cap.homed ? COL_GREEN : COL_YELLOW);

    ly += 4;
    // Voltage ratings table header
    tft.setTextColor(COL_YELLOW, COL_BG);
    tft.setCursor(4, ly); tft.print("--- Voltage Ratings (current C) ---"); ly += lh;

    struct { const char* band; float freq_mhz; float v_kv; } bands[] = {
        {"160m", 1.9f, 1.5f}, {"80m", 3.6f, 1.5f}, {"40m", 7.1f, 1.5f},
        {"20m", 14.2f, 1.5f}, {"10m", 28.5f, 1.5f}, {"6m", 51.0f, 0.5f}
    };
    for (auto& b : bands) {
        float xc = 1.0f / (2.0f * M_PI * b.freq_mhz * 1e6f * max(cap.c_pf, 1.0f) * 1e-12f);
        float vp = sqrtf(2.0f * 100.0f * xc);
        uint16_t vc = voltColor(vp, b.v_kv);
        snprintf(buf, sizeof(buf), "%-5s Xc=%5.0f  Vpk@100W=%5.0fV", b.band, xc, vp);
        tft.setTextColor(vc, COL_BG);
        tft.setCursor(4, ly);
        tft.print(buf);
        ly += lh;
    }

    tft.setTextColor(COL_DIM, COL_BG);
    tft.setCursor(4, ly);
    snprintf(buf, sizeof(buf), "FW: %s", cap.fw);
    tft.print(buf);

    drawButtonBar(info_btns, 5);
}

// ─── Touch Handling ───────────────────────────────────────────────────────────

uint32_t last_touch = 0;
#define TOUCH_DEBOUNCE 300

void handleTouch(int tx, int ty) {
    if (millis() - last_touch < TOUCH_DEBOUNCE) return;
    last_touch = millis();

    // Tab bar touch (second strip from bottom)
    int tab_y = SCR_H - TAB_H - BTN_H;
    if (ty >= tab_y && ty < tab_y + TAB_H) {
        int tab_w = SCR_W / MODE_COUNT;
        int new_mode = tx / tab_w;
        if (new_mode >= 0 && new_mode < MODE_COUNT) {
            mode = (DisplayMode)new_mode;
            dirty = true;
        }
        return;
    }

    // Button bar touch (bottom strip)
    if (ty >= SCR_H - BTN_H) {
        int n_btns = 5;
        int bw = SCR_W / n_btns;
        int btn = tx / bw;
        handleButtonPress(btn);
        return;
    }

    // Mode-specific touch (BAND grid)
    if (mode == MODE_BAND) {
        int y = HEADER_H + 4;
        int bw = SCR_W / 5, bh = 36;
        int col = tx / bw;
        int row = (ty - y) / (bh + 2);
        if (row >= 0 && row < 2 && col >= 0 && col < 5) {
            int idx = row * 5 + col;
            if (idx < 10) { band_sel = idx; dirty = true; }
        }
    }
}

void handleButtonPress(int btn) {
    char buf[32];
    switch (mode) {
        case MODE_LIVE:
            switch (btn) {
                case 0: sendCommand("HOME"); break;
                case 1: sendCommand("GOSTEP -1000"); break;
                case 2: sendCommand("GOSTEP 1000"); break;
                case 3: sendCommand("STOP"); break;
                case 4: sendCommand("CALPOT"); break;
            }
            break;

        case MODE_BAND:
            switch (btn) {
                case 0: sendCommand("HOME"); break;
                case 1: band_sel = max(0, band_sel - 1); dirty = true; break;
                case 2: band_sel = min(N_BANDS_CYD - 1, band_sel + 1); dirty = true; break;
                case 3:
                    snprintf(buf, sizeof(buf), "BAND %s", band_names[band_sel]);
                    sendCommand(buf); break;
                case 4: mode = MODE_LIVE; dirty = true; break;
            }
            break;

        case MODE_SWEEP:
            switch (btn) {
                case 0: sendCommand("HOME"); break;
                case 1:
                    sweep_n = 0; sweep_active = true;
                    sendCommand("SWEEP"); dirty = true; break;
                case 2:
                    sweep_active = false;
                    sendCommand("STOP"); break;
                case 3: sweep_n = 0; dirty = true; break;
                case 4: mode = MODE_LIVE; dirty = true; break;
            }
            break;

        case MODE_INFO:
            switch (btn) {
                case 0: sendCommand("HOME"); break;
                case 1: sendCommand("RESET"); break;
                case 2: sendCommand("STATUS"); break;
                case 3: sendCommand("CALPOT"); break;
                case 4: mode = MODE_LIVE; dirty = true; break;
            }
            break;

        default: break;
    }
}

// ─── RGB LED ──────────────────────────────────────────────────────────────────

void updateLED() {
    static uint32_t last_blink = 0;
    static bool     state = false;
    int period;
    if (!btConnected)       period = 200;
    else if (cap.fault)     period = 100;
    else if (cap.moving)    period = 250;
    else if (!cap.homed)    period = 500;
    else                    period = 2000;

    if (millis() - last_blink > (uint32_t)period) {
        last_blink = millis();
        state = !state;
        digitalWrite(RGB_R, state && (cap.fault || !btConnected) ? LOW : HIGH);
        digitalWrite(RGB_G, state && btConnected && !cap.fault ? LOW : HIGH);
        digitalWrite(RGB_B, state && !cap.homed ? LOW : HIGH);
    }
}

// ─── Redraw ───────────────────────────────────────────────────────────────────

void redraw() {
    tft.fillScreen(COL_BG);
    drawHeader();
    drawTabBar();

    switch (mode) {
        case MODE_LIVE:  drawLiveMode();  break;
        case MODE_BAND:  drawBandMode();  break;
        case MODE_SWEEP: drawSweepMode(); break;
        case MODE_INFO:  drawInfoMode();  break;
        default: break;
    }
    dirty = false;
}

// ─── Partial Updates (LIVE mode only — avoid full redraw every 500ms) ─────────

void updateLivePartial() {
    if (mode != MODE_LIVE) return;

    // Redraw just the numeric fields, not the full screen
    int y = HEADER_H + 4;
    char buf[32];

    tft.setTextSize(3);
    uint16_t col = capColor(cap.c_pf, 10, 250);
    tft.setTextColor(col, COL_BG);
    snprintf(buf, sizeof(buf), "%6.1f pF", cap.c_pf);
    tft.setCursor(30, y + 4);
    tft.print(buf);

    // Re-draw arc (clears old with black fill first)
    int cx = SCR_W / 2, cy = HEADER_H + 85, r = 60;
    tft.fillCircle(cx, cy, r + 2, COL_BG);
    drawCapArc(cap.c_pf, 10, 250);

    // Pot comparison
    int py = y + 100;
    tft.setTextSize(1);
    tft.fillRect(0, py, SCR_W, 70, COL_BG);

    tft.setTextColor(COL_DIM, COL_BG);
    tft.setCursor(4, py);
    tft.print("Stepper:");
    tft.setTextColor(COL_TEXT, COL_BG);
    snprintf(buf, sizeof(buf), "%.1f pF / %.1f deg", cap.c_pf, cap.angle);
    tft.print(buf);

    tft.setCursor(4, py + 12);
    tft.setTextColor(COL_DIM, COL_BG);
    tft.print("Pot:    ");
    tft.setTextColor(COL_CYAN, COL_BG);
    snprintf(buf, sizeof(buf), "%.1f pF / %.1f deg", cap.pot_c, cap.pot_ang);
    tft.print(buf);

    drawSparkline(4, py + 26, SCR_W - 8, 30);

    // Voltage warning line
    float vpeak = estimatePeakV(cap.c_pf, 14.0f, 100.0f);
    uint16_t vc = voltColor(vpeak, 5.0f);
    tft.setCursor(4, py + 60);
    tft.setTextColor(vc, COL_BG);
    snprintf(buf, sizeof(buf), "Xc@14MHz: %.0f ohm  Vpk@100W: %.0f V",
             1.0f / (2.0f * M_PI * 14e6f * cap.c_pf * 1e-12f), vpeak);
    tft.fillRect(0, py + 60, SCR_W, 12, COL_BG);
    tft.print(buf);

    // Update header state indicator only
    drawHeader();
}

// ─── setup() ─────────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(115200);
    Serial.println("CYD Cap Display boot");

    // RGB LED (active LOW)
    pinMode(RGB_R, OUTPUT); digitalWrite(RGB_R, HIGH);
    pinMode(RGB_G, OUTPUT); digitalWrite(RGB_G, HIGH);
    pinMode(RGB_B, OUTPUT); digitalWrite(RGB_B, HIGH);

    // Touch
    SPI.begin();
    touch.begin();
    touch.setRotation(1);

    // TFT
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(COL_BG);

    // Boot splash
    tft.setTextColor(COL_CYAN, COL_BG);
    tft.setTextSize(2);
    tft.setCursor(60, 80);
    tft.println("RF-CapTuner");
    tft.setTextSize(1);
    tft.setTextColor(COL_DIM, COL_BG);
    tft.setCursor(80, 108);
    tft.println("TM-VCAP-CYD-001");

    delay(1200);

    // BT
    btMaster.begin("CYD-CapDisplay", true);  // true = master
    connectBT();

    memset(spark_buf, 0, sizeof(spark_buf));
    memset(sweep_buf, 0, sizeof(sweep_buf));

    dirty = true;
}

// ─── loop() ──────────────────────────────────────────────────────────────────

uint32_t last_redraw = 0;

void loop() {
    bool was_connected = btConnected;
    btConnected = btMaster.hasClient();

    if (btConnected && !was_connected) {
        dirty = true;
        Serial.println("BT connected");
    }
    if (!btConnected && was_connected) {
        dirty = true;
        Serial.println("BT disconnected — retrying");
        delay(2000);
        connectBT();
    }

    // Receive BT data
    while (btMaster.available()) {
        char c = btMaster.read();
        if (c == '\n' || c == '\r') {
            if (btBuf.length()) {
                parseStatus(btBuf);
                btBuf = "";
                // Partial update in LIVE mode (smooth, no flicker)
                if (mode == MODE_LIVE && !dirty)
                    updateLivePartial();
                // Force full redraw in other modes periodically
                else if (mode != MODE_LIVE)
                    dirty = true;
            }
        } else btBuf += c;
    }

    // Stale data warning
    cap.age_ms = millis() - last_rx;

    // Full redraw
    if (dirty || millis() - last_redraw > 5000) {
        redraw();
        last_redraw = millis();
    }

    // Touch
    if (touch.tirqTouched() && touch.touched()) {
        TS_Point p = touch.getPoint();
        // Map XPT2046 raw → screen pixels (calibrate if needed)
        int tx = map(p.x, 200, 3900, 0, SCR_W);
        int ty = map(p.y, 200, 3900, 0, SCR_H);
        handleTouch(tx, ty);
    }

    updateLED();
}
