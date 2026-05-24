/**
 * cyd_cal_display.ino — TM-CAL-FW-002
 * CYD (Cheap Yellow Display) — Calibration Station Display
 *
 * BT slave connects to "CAL-Master"
 * Displays: GPS status, lock indicator, frequency error, signal source freq,
 *           RSSI level, frequency counter, calibration quick-reference
 *
 * Touch zones:
 *   Left half: frequency preset cycling
 *   Right half: counter start / gate toggle
 *   Bottom bar: page select (STATUS / COUNTER / CAL GUIDE)
 */

#include <TFT_eSPI.h>
#include <SPI.h>
#include "BluetoothSerial.h"
#include <XPT2046_Touchscreen.h>

// CYD pin definitions (ESP32-2432S028 "Cheap Yellow Display")
#define TFT_BL      21
#define TOUCH_CS     5
#define TOUCH_IRQ   36

TFT_eSPI      tft;
TFT_eSprite   spr = TFT_eSprite(&tft);
BluetoothSerial BT;
XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);

// ── Display Layout ────────────────────────────────────────────────────────────
#define SCR_W   320
#define SCR_H   240
#define TAB_H    30

// ── Data from BT ─────────────────────────────────────────────────────────────
struct CalData {
    bool  gps_fix;
    int   sats;
    char  time_str[10];
    bool  locked;
    float err_ppb;
    long  correction;
    float prog_freq_hz;
    float rssi_dBm;
    float cnt_freq_hz;
} g_data;

bool     g_connected = false;
uint32_t g_last_rx   = 0;

// ── RSSI sparkline ────────────────────────────────────────────────────────────
#define SPARK_LEN 60
float    g_rssi_hist[SPARK_LEN] = {0};
uint8_t  g_spark_idx = 0;

// ── Pages ─────────────────────────────────────────────────────────────────────
#define PAGE_STATUS  0
#define PAGE_COUNTER 1
#define PAGE_GUIDE   2
uint8_t  g_page = PAGE_STATUS;

// ── Preset cycling ────────────────────────────────────────────────────────────
const uint32_t PRESETS[]  = {100000,1000000,10000000,14100000,25000000,50000000,100000000,144000000};
const char*    PRESET_LBL[] = {"100k","1M","10M","14.1M","25M","50M","100M","144M"};
uint8_t        g_preset_idx = 2;  // 10 MHz default

// ── Colors ────────────────────────────────────────────────────────────────────
#define C_BG        0x1082   // dark gray
#define C_PANEL     0x2945   // medium gray
#define C_HEADER    0x4BF5   // light blue
#define C_GREEN     0x07E0
#define C_RED       0xF800
#define C_YELLOW    0xFFE0
#define C_CYAN      0x07FF
#define C_WHITE     0xFFFF
#define C_LOCKED    0x07E0   // green
#define C_UNLOCKED  0xFC00   // orange

// ── BT Parsing ────────────────────────────────────────────────────────────────
String g_bt_buf;

void parseBT(const String &pkt) {
    // Packet: "GPS:1,SATS:8,TIME:12:34:56,LOCK:1,ERR:0.23,CORR:1234,CLK2:10000000,RSSI:-42.1,CNT:14225000\n"
    auto fval = [&](const char *key) -> float {
        int i = pkt.indexOf(key);
        if (i < 0) return 0.0f;
        return pkt.substring(i + strlen(key)).toFloat();
    };
    auto ival = [&](const char *key) -> int {
        int i = pkt.indexOf(key);
        if (i < 0) return 0;
        return pkt.substring(i + strlen(key)).toInt();
    };
    auto sval = [&](const char *key, char *out, int maxlen) {
        int i = pkt.indexOf(key);
        if (i < 0) return;
        int s = i + strlen(key);
        int e = pkt.indexOf(',', s);
        if (e < 0) e = pkt.length();
        String v = pkt.substring(s, e);
        strncpy(out, v.c_str(), maxlen-1);
    };

    g_data.gps_fix      = (bool)ival("GPS:");
    g_data.sats         = ival("SATS:");
    sval("TIME:", g_data.time_str, sizeof(g_data.time_str));
    g_data.locked       = (bool)ival("LOCK:");
    g_data.err_ppb      = fval("ERR:");
    g_data.correction   = (long)fval("CORR:");
    g_data.prog_freq_hz = fval("CLK2:");
    g_data.rssi_dBm     = fval("RSSI:");
    g_data.cnt_freq_hz  = fval("CNT:");

    g_rssi_hist[g_spark_idx] = g_data.rssi_dBm;
    g_spark_idx = (g_spark_idx + 1) % SPARK_LEN;
    g_last_rx = millis();
}

// ── Draw helpers ──────────────────────────────────────────────────────────────
String fmtFreq(float hz) {
    char buf[24];
    if      (hz >= 1e6) snprintf(buf, sizeof(buf), "%.6f MHz", hz / 1e6);
    else if (hz >= 1e3) snprintf(buf, sizeof(buf), "%.3f kHz", hz / 1e3);
    else                snprintf(buf, sizeof(buf), "%.0f Hz",   hz);
    return String(buf);
}

void drawTabBar() {
    uint16_t tab_colors[3] = { C_PANEL, C_PANEL, C_PANEL };
    tab_colors[g_page] = C_HEADER;
    const char *labels[3] = { "STATUS", "COUNTER", "CAL GUIDE" };
    for (int i = 0; i < 3; i++) {
        int x = i * (SCR_W / 3);
        spr.fillRect(x, SCR_H - TAB_H, SCR_W/3 - 1, TAB_H, tab_colors[i]);
        spr.setTextColor(C_WHITE, tab_colors[i]);
        spr.setTextSize(1);
        spr.drawString(labels[i], x + SCR_W/6 - strlen(labels[i])*3, SCR_H - TAB_H + 10);
    }
}

void drawStatusPage() {
    int y = 4;
    // Header row
    spr.fillRect(0, y, SCR_W, 22, C_PANEL);
    spr.setTextColor(C_HEADER, C_PANEL);
    spr.setTextSize(1);
    spr.drawString("GPS CALIBRATION STATION", 4, y+6);
    spr.setTextColor(g_connected ? C_GREEN : C_RED, C_PANEL);
    spr.drawString(g_connected ? "BT OK" : "NO BT", SCR_W-50, y+6);
    y += 26;

    // GPS status row
    spr.fillRect(0, y, SCR_W/2-2, 40, C_PANEL);
    spr.setTextColor(C_CYAN, C_PANEL);
    spr.drawString("GPS", 6, y+2);
    uint16_t gps_col = g_data.gps_fix ? C_GREEN : C_RED;
    spr.setTextColor(gps_col, C_PANEL);
    spr.setTextSize(2);
    spr.drawString(g_data.gps_fix ? "FIX" : "SEARCH", 6, y+14);
    spr.setTextSize(1);
    spr.setTextColor(C_WHITE, C_PANEL);
    char sats_str[10]; snprintf(sats_str, sizeof(sats_str), "%d SVs", g_data.sats);
    spr.drawString(sats_str, 80, y+16);

    // UTC time
    spr.fillRect(SCR_W/2+1, y, SCR_W/2-1, 40, C_PANEL);
    spr.setTextColor(C_CYAN, C_PANEL);
    spr.drawString("UTC", SCR_W/2+4, y+2);
    spr.setTextColor(C_WHITE, C_PANEL);
    spr.setTextSize(2);
    spr.drawString(g_data.time_str, SCR_W/2+4, y+14);
    spr.setTextSize(1);
    y += 44;

    // Lock status row
    spr.fillRect(0, y, SCR_W, 34, C_PANEL);
    spr.setTextColor(C_CYAN, C_PANEL);
    spr.drawString("DISCIPLINE LOCK", 4, y+2);
    uint16_t lock_col = g_data.locked ? C_LOCKED : C_UNLOCKED;
    spr.setTextColor(lock_col, C_PANEL);
    spr.setTextSize(2);
    spr.drawString(g_data.locked ? "LOCKED" : "DISCIPLINING", 4, y+12);
    spr.setTextSize(1);
    char err_str[30]; snprintf(err_str, sizeof(err_str), "%.3f ppb  corr=%ld", g_data.err_ppb, g_data.correction);
    spr.setTextColor(C_WHITE, C_PANEL);
    spr.drawString(err_str, 4+130, y+18);
    y += 38;

    // Signal source row
    spr.fillRect(0, y, SCR_W, 32, C_PANEL);
    spr.setTextColor(C_CYAN, C_PANEL);
    spr.drawString("CLK2 PROG OUT", 4, y+2);
    spr.setTextColor(C_YELLOW, C_PANEL);
    spr.setTextSize(2);
    spr.drawString(fmtFreq(g_data.prog_freq_hz).c_str(), 4, y+12);
    spr.setTextSize(1);
    // Touch hint
    spr.setTextColor(C_PANEL+0x2000, C_PANEL);
    spr.drawString("< TAP TO CYCLE PRESET >", 200, y+20);
    y += 36;

    // RSSI row + sparkline
    spr.fillRect(0, y, SCR_W, 50, C_PANEL);
    spr.setTextColor(C_CYAN, C_PANEL);
    spr.drawString("RSSI (PROG OUT)", 4, y+2);
    char rssi_str[16]; snprintf(rssi_str, sizeof(rssi_str), "%.1f dBm", g_data.rssi_dBm);
    spr.setTextColor(C_WHITE, C_PANEL);
    spr.setTextSize(2);
    spr.drawString(rssi_str, 4, y+14);
    spr.setTextSize(1);
    // Sparkline: 60 samples, 190 px wide, y range -80 to 0 dBm
    int sx = 130, sy = y+4, sw = 186, sh = 42;
    spr.drawRect(sx, sy, sw, sh, C_PANEL+0x3000);
    for (int i = 1; i < SPARK_LEN; i++) {
        int i0 = (g_spark_idx + i - 1) % SPARK_LEN;
        int i1 = (g_spark_idx + i)     % SPARK_LEN;
        float v0 = constrain(g_rssi_hist[i0], -90.0f, 0.0f);
        float v1 = constrain(g_rssi_hist[i1], -90.0f, 0.0f);
        int x0 = sx + (i-1) * sw / SPARK_LEN;
        int x1 = sx + i     * sw / SPARK_LEN;
        int y0 = sy + sh - (int)((v0 + 90.0f) / 90.0f * sh);
        int y1 = sy + sh - (int)((v1 + 90.0f) / 90.0f * sh);
        spr.drawLine(x0, y0, x1, y1, C_CYAN);
    }
}

void drawCounterPage() {
    int y = 4;
    spr.fillRect(0, y, SCR_W, 22, C_PANEL);
    spr.setTextColor(C_HEADER, C_PANEL);
    spr.drawString("FREQUENCY COUNTER", 4, y+6);
    y += 28;

    spr.fillRect(0, y, SCR_W, 60, C_PANEL);
    spr.setTextColor(C_CYAN, C_PANEL);
    spr.drawString("MEASURED FREQUENCY", 4, y+2);
    spr.setTextColor(C_YELLOW, C_PANEL);
    spr.setTextSize(3);
    spr.drawString(fmtFreq(g_data.cnt_freq_hz).c_str(), 4, y+14);
    spr.setTextSize(1);
    y += 68;

    // GPS reference
    spr.fillRect(0, y, SCR_W, 30, C_PANEL);
    spr.setTextColor(C_CYAN, C_PANEL);
    spr.drawString("GPS REF:", 4, y+8);
    spr.setTextColor(g_data.locked ? C_GREEN : C_YELLOW, C_PANEL);
    spr.drawString(g_data.locked ? "LOCKED <1 ppb" : "DISCIPLINING...", 70, y+8);
    y += 34;

    // Instructions
    spr.fillRect(0, y, SCR_W, 70, C_PANEL);
    spr.setTextColor(C_WHITE, C_PANEL);
    spr.drawString("Connect DUT to BNC FREQ IN", 4, y+4);
    spr.drawString("Tap RIGHT: START 1s count", 4, y+18);
    spr.drawString("Tap LEFT:  Toggle ÷10 mode", 4, y+32);
    spr.setTextColor(C_YELLOW, C_PANEL);
    spr.drawString("For <40 MHz: direct", 4, y+48);
    spr.drawString("For 40-400 MHz: ÷10 mode", 4, y+58);
}

void drawGuidePage() {
    int y = 4;
    spr.fillRect(0, y, SCR_W, 22, C_PANEL);
    spr.setTextColor(C_HEADER, C_PANEL);
    spr.drawString("CALIBRATION QUICK GUIDE", 4, y+6);
    y += 26;

    const char *lines[] = {
        "NANOVNA FREQ CAL:",
        "  10M REF -> CH0 | verify display",
        "NANOVNA S-PARAM CAL:",
        "  SHORT/OPEN/LOAD -> CH0 | THRU",
        "TINYSAA FREQ CAL:",
        "  PROG OUT 10M -> INPUT",
        "  Menu>Config>Expert>Correction",
        "TINYSAA LEVEL CAL:",
        "  PROG OUT+20dB pad -> INPUT",
        "  Verify -20dBm displayed",
        "RTL-SDR PPM CAL:",
        "  PROG OUT 10M + 30dB -> SDR",
        "  Measure offset in GQRX (ppm)",
        "LOCK BEFORE ALL CAL:",
        "  LOCK LED solid blue required"
    };
    spr.setTextColor(C_WHITE, C_BG);
    for (int i = 0; i < 15 && y < SCR_H - TAB_H - 10; i++) {
        if (lines[i][0] != ' ')
            spr.setTextColor(C_CYAN, C_BG);
        else
            spr.setTextColor(C_WHITE, C_BG);
        spr.drawString(lines[i], 4, y);
        y += 13;
    }
}

// ── Touch Handling ────────────────────────────────────────────────────────────
void handleTouch(int tx, int ty) {
    // Bottom tab bar
    if (ty > SCR_H - TAB_H) {
        if      (tx < SCR_W/3)       g_page = PAGE_STATUS;
        else if (tx < 2 * SCR_W/3)   g_page = PAGE_COUNTER;
        else                          g_page = PAGE_GUIDE;
        return;
    }

    if (g_page == PAGE_STATUS) {
        // Left half: cycle preset
        if (tx < SCR_W / 2) {
            g_preset_idx = (g_preset_idx + 1) % 8;
            // Send BT command to change frequency
            char cmd[24];
            snprintf(cmd, sizeof(cmd), "FREQ %lu\n", PRESETS[g_preset_idx]);
            if (BT.connected()) BT.print(cmd);
        } else {
            // Right half: start counter
            if (BT.connected()) BT.println("COUNT");
        }
    } else if (g_page == PAGE_COUNTER) {
        if (tx < SCR_W / 2) {
            // Toggle ÷10 — send via BT (simplified; operator can set on web UI)
        } else {
            if (BT.connected()) BT.println("COUNT");
        }
    }
}

// ── Setup / Loop ──────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(C_BG);

    spr.createSprite(SCR_W, SCR_H);
    spr.setTextFont(1);

    touch.begin();
    touch.setRotation(1);

    BT.begin("CYD-CalDisplay", true);
    BT.connect("CAL-Master");
    tft.drawString("Connecting to CAL-Master...", 10, 110);
}

void loop() {
    // BT receive
    while (BT.available()) {
        char c = BT.read();
        if (c == '\n') {
            g_bt_buf.trim();
            if (g_bt_buf.length() > 0) parseBT(g_bt_buf);
            g_bt_buf = "";
        } else {
            g_bt_buf += c;
        }
    }
    g_connected = BT.connected();

    // Touch
    if (touch.tirqTouched() && touch.touched()) {
        TS_Point p = touch.getPoint();
        int tx = map(p.x, 200, 3800, 0, SCR_W);
        int ty = map(p.y, 200, 3800, 0, SCR_H);
        handleTouch(tx, ty);
        delay(200);
    }

    // Reconnect BT if disconnected
    static uint32_t t_reconnect = 0;
    if (!g_connected && millis() - t_reconnect > 5000) {
        t_reconnect = millis();
        BT.connect("CAL-Master");
    }

    // Render frame at ~10 FPS
    static uint32_t t_last_draw = 0;
    if (millis() - t_last_draw < 100) return;
    t_last_draw = millis();

    spr.fillSprite(C_BG);

    // Stale data indicator
    if (millis() - g_last_rx > 5000) {
        spr.setTextColor(C_RED, C_BG);
        spr.drawString("NO DATA FROM CAL-MASTER", 60, SCR_H/2 - 10);
    } else {
        switch (g_page) {
            case PAGE_STATUS:  drawStatusPage();  break;
            case PAGE_COUNTER: drawCounterPage(); break;
            case PAGE_GUIDE:   drawGuidePage();   break;
        }
    }

    drawTabBar();
    spr.pushSprite(0, 0);
}
