// =============================================================================
// cyd_whip_display.ino
// CYD (ESP32-2432S028) Display for 5.6m Telescopic Whip Tuner
// Receives BT telemetry from WHP-Tuner ESP32 — TFT_eSPI + sprite rendering
// 4 pages: SWR | SETUP | POWER | GUIDE
// =============================================================================

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <BluetoothSerial.h>

// =============================================================================
// Display & Color Constants
// =============================================================================
#define SCREEN_W  320
#define SCREEN_H  240
#define TAB_H      28
#define TITLE_H    24
#define CONTENT_Y  (TITLE_H + 2)
#define CONTENT_H  (SCREEN_H - TITLE_H - TAB_H - 4)

// 16-bit RGB565 colors
#define COL_BG      0x0000  // Black
#define COL_TITLE   0xFFE0  // Yellow
#define COL_LABEL   0x7BEF  // Mid grey-green
#define COL_VALUE   0x07FF  // Cyan
#define COL_OK      0x07E0  // Green
#define COL_WARN    0xFFE0  // Yellow
#define COL_BAD     0xF800  // Red
#define COL_BORDER  0x4208  // Dark grey
#define COL_WHITE   0xFFFF  // White
#define COL_ORANGE  0xFD20  // Orange
#define COL_DGREY   0x2104  // Very dark grey (tab inactive bg)

// =============================================================================
// Pages
// =============================================================================
enum Page { PAGE_SWR = 0, PAGE_SETUP, PAGE_POWER, PAGE_GUIDE, PAGE_COUNT };

// =============================================================================
// Tuner Data Struct — matches CSV packet from esp32_whip_tuner
// =============================================================================
struct TunerData {
    char  band[8];
    float freq_mhz;
    float extension_m;
    float coil_L_uH;
    int   coil_type;
    float swr;
    float fwd_w;
    float refl_dbm;
    float rl_db;
    bool  valid;
    bool  connected;
    char  match_note[64];
};

// =============================================================================
// Globals
// =============================================================================
TFT_eSPI    tft;
TFT_eSprite spr(&tft);
BluetoothSerial BT;

TunerData   g_data = {};
Page        g_page = PAGE_SWR;

static char     bt_buf[256];
static int      bt_pos = 0;
static unsigned long last_reconnect_ms = 0;
static unsigned long last_render_ms    = 0;
static unsigned long last_packet_ms    = 0;

// Touch calibration (CYD default orientation 1 — landscape)
// Adjust if touchscreen is miscalibrated on your specific unit
#define TOUCH_MIN_X   340
#define TOUCH_MAX_X  3900
#define TOUCH_MIN_Y   260
#define TOUCH_MAX_Y  3800

// =============================================================================
// maintainBT — attempt reconnect to "WHP-Tuner" every 5s if disconnected
// =============================================================================
void maintainBT() {
    if (BT.connected()) {
        g_data.connected = true;
        return;
    }
    g_data.connected = false;

    // Mark data stale if we haven't received a packet in > 3s
    if (millis() - last_packet_ms > 3000) {
        g_data.valid = false;
    }

    unsigned long now = millis();
    if (now - last_reconnect_ms < 5000) return;
    last_reconnect_ms = now;

    Serial.println("[BT] Attempting reconnect to WHP-Tuner...");
    BT.connect("WHP-Tuner");  // Non-blocking attempt; success noted next loop
}

// =============================================================================
// parseBTLine — parse CSV packet:
//   BAND:%s,EXT:%.3f,COIL:%.1f,SWR:%.2f,FWD:%.3fW,REFL:%.1f,RL:%.1f,VALID:%d
// =============================================================================
void parseBTLine(const char* line) {
    // Make a mutable copy
    char buf[256];
    strncpy(buf, line, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    // Helper lambda — find value string after "KEY:" in token
    auto extractStr = [](char* token, const char* key, char* out, int outlen) -> bool {
        const char* p = strstr(token, key);
        if (!p) return false;
        p += strlen(key);
        strncpy(out, p, outlen - 1);
        out[outlen - 1] = '\0';
        return true;
    };
    auto extractFloat = [](const char* token, const char* key, float* out) -> bool {
        const char* p = strstr(token, key);
        if (!p) return false;
        p += strlen(key);
        // Strip any trailing unit letters
        char tmp[32];
        strncpy(tmp, p, sizeof(tmp) - 1);
        tmp[sizeof(tmp) - 1] = '\0';
        // Remove 'W' suffix for FWD field if present
        for (int i = 0; tmp[i]; i++) {
            if (tmp[i] == 'W' || tmp[i] == '\r' || tmp[i] == '\n') { tmp[i] = '\0'; break; }
        }
        *out = atof(tmp);
        return true;
    };
    auto extractInt = [](const char* token, const char* key, int* out) -> bool {
        const char* p = strstr(token, key);
        if (!p) return false;
        p += strlen(key);
        *out = atoi(p);
        return true;
    };

    // Tokenise by comma
    char* tok = strtok(buf, ",");
    while (tok) {
        if (strstr(tok, "BAND:")) {
            char tmp[8] = {};
            extractStr(tok, "BAND:", tmp, sizeof(tmp));
            strncpy(g_data.band, tmp, sizeof(g_data.band) - 1);
        } else if (strstr(tok, "EXT:")) {
            extractFloat(tok, "EXT:", &g_data.extension_m);
        } else if (strstr(tok, "COIL:")) {
            extractFloat(tok, "COIL:", &g_data.coil_L_uH);
            // Derive coil_type from coil_L_uH value
            if (g_data.coil_L_uH > 0.0f)
                g_data.coil_type = 1;
            // (series cap case inferred from band table on tuner side; display shows coil_L==0 as no coil)
        } else if (strstr(tok, "SWR:")) {
            extractFloat(tok, "SWR:", &g_data.swr);
        } else if (strstr(tok, "FWD:")) {
            extractFloat(tok, "FWD:", &g_data.fwd_w);
        } else if (strstr(tok, "REFL:")) {
            extractFloat(tok, "REFL:", &g_data.refl_dbm);
        } else if (strstr(tok, "RL:")) {
            extractFloat(tok, "RL:", &g_data.rl_db);
        } else if (strstr(tok, "VALID:")) {
            int v = 0;
            extractInt(tok, "VALID:", &v);
            g_data.valid = (v != 0);
        }
        tok = strtok(nullptr, ",");
    }

    last_packet_ms = millis();
}

// =============================================================================
// readBT — accumulate serial characters, parse on '\n'
// =============================================================================
void readBT() {
    while (BT.available()) {
        char c = (char)BT.read();
        if (c == '\n') {
            bt_buf[bt_pos] = '\0';
            if (bt_pos > 5) {
                parseBTLine(bt_buf);
            }
            bt_pos = 0;
        } else if (c != '\r') {
            if (bt_pos < (int)sizeof(bt_buf) - 1) {
                bt_buf[bt_pos++] = c;
            }
        }
    }
}

// =============================================================================
// checkTouch — bottom tab bar: 4 equal zones across 320px
// =============================================================================
void checkTouch() {
    uint16_t x, y;
    if (!tft.getTouch(&x, &y)) return;

    // Only respond to touches in the tab bar zone
    if (y < (SCREEN_H - TAB_H)) return;

    int zone = x / (SCREEN_W / PAGE_COUNT);
    if (zone >= PAGE_COUNT) zone = PAGE_COUNT - 1;
    Page new_page = (Page)zone;
    if (new_page != g_page) {
        g_page = new_page;
        // Force immediate redraw
        last_render_ms = 0;
    }
}

// =============================================================================
// drawTabBar — 4 tabs at bottom of screen
// =============================================================================
void drawTabBar(Page active) {
    const char* labels[PAGE_COUNT] = { "SWR", "SETUP", "POWER", "GUIDE" };
    int tab_w = SCREEN_W / PAGE_COUNT;
    int y = SCREEN_H - TAB_H;

    for (int i = 0; i < PAGE_COUNT; i++) {
        int x = i * tab_w;
        uint16_t bg = (i == (int)active) ? COL_BORDER : COL_DGREY;
        uint16_t fg = (i == (int)active) ? COL_TITLE  : COL_LABEL;
        spr.fillRect(x, y, tab_w - 1, TAB_H, bg);
        spr.drawRect(x, y, tab_w - 1, TAB_H, COL_BORDER);
        spr.setTextColor(fg, bg);
        spr.setTextDatum(MC_DATUM);
        spr.setTextSize(1);
        spr.drawString(labels[i], x + tab_w / 2, y + TAB_H / 2);
    }
}

// =============================================================================
// drawTitleBar — standard title bar with connection indicator
// =============================================================================
void drawTitleBar(const char* title) {
    spr.fillRect(0, 0, SCREEN_W, TITLE_H, 0x0010);  // Very dark blue
    spr.setTextColor(COL_TITLE, 0x0010);
    spr.setTextDatum(ML_DATUM);
    spr.setTextSize(1);
    spr.drawString(title, 6, TITLE_H / 2);

    // Connection indicator dot (right side)
    uint16_t dot_col = g_data.connected ? COL_OK : COL_BAD;
    int cx = SCREEN_W - 10;
    int cy = TITLE_H / 2;
    spr.fillCircle(cx, cy, 5, dot_col);

    // Band chip right of dot
    spr.setTextColor(COL_VALUE, 0x0010);
    spr.setTextDatum(MR_DATUM);
    spr.drawString(g_data.band[0] ? g_data.band : "---", SCREEN_W - 18, TITLE_H / 2);
}

// =============================================================================
// drawSWRBar — horizontal colored bar with tick marks
// swr range 1.0 – 10.0 mapped to 0–w pixels
// =============================================================================
void drawSWRBar(int x, int y, int w, int h, float swr) {
    // Background
    spr.fillRect(x, y, w, h, 0x1082);
    spr.drawRect(x, y, w, h, COL_BORDER);

    // Determine fill width
    float swr_clamped = swr;
    if (swr_clamped < 1.0f) swr_clamped = 1.0f;
    if (swr_clamped > 10.0f) swr_clamped = 10.0f;
    float frac = (swr_clamped - 1.0f) / 9.0f;
    int fill_w = (int)(frac * (float)(w - 2));

    // Color gradient: green → yellow → red
    uint16_t bar_col;
    if (frac < 0.056f)      bar_col = COL_OK;    // ≤1.5
    else if (frac < 0.167f) bar_col = COL_WARN;  // ≤2.5
    else                    bar_col = COL_BAD;

    if (fill_w > 0 && g_data.valid) {
        spr.fillRect(x + 1, y + 1, fill_w, h - 2, bar_col);
    }

    // Tick marks at 1.5, 2, 3, 5, 10
    float ticks[] = {1.5f, 2.0f, 3.0f, 5.0f, 10.0f};
    const char* tick_labels[] = {"1.5", "2", "3", "5", "10"};
    for (int i = 0; i < 5; i++) {
        float tf = (ticks[i] - 1.0f) / 9.0f;
        int tx = x + 1 + (int)(tf * (float)(w - 2));
        spr.drawFastVLine(tx, y, h, COL_WHITE);
        spr.setTextColor(COL_LABEL, COL_BG);
        spr.setTextDatum(TC_DATUM);
        spr.setTextSize(1);
        spr.drawString(tick_labels[i], tx, y + h + 2);
    }
}

// =============================================================================
// swr_color — returns appropriate color for SWR value
// =============================================================================
uint16_t swr_color(float swr) {
    if (!g_data.valid)       return COL_LABEL;
    if (swr <= 1.5f)         return COL_OK;
    if (swr <= 2.5f)         return COL_WARN;
    return COL_BAD;
}

// =============================================================================
// drawPageSWR — main SWR monitor page
// =============================================================================
void drawPageSWR() {
    drawTitleBar("WHIP SWR MONITOR");

    int cy = CONTENT_Y + 2;

    // Band + Frequency
    spr.setTextDatum(ML_DATUM);
    spr.setTextColor(COL_TITLE, COL_BG);
    spr.setTextSize(2);
    spr.drawString(g_data.band[0] ? g_data.band : "---", 6, cy + 10);
    spr.setTextColor(COL_VALUE, COL_BG);
    spr.setTextSize(1);
    char freq_buf[24];
    snprintf(freq_buf, sizeof(freq_buf), "%.3f MHz", g_data.freq_mhz);
    spr.drawString(freq_buf, 70, cy + 10);
    cy += 24;

    if (!g_data.valid) {
        spr.setTextColor(COL_LABEL, COL_BG);
        spr.setTextDatum(MC_DATUM);
        spr.setTextSize(1);
        spr.drawString("NO RF", SCREEN_W / 2, cy + 14);
        spr.drawString("TRANSMIT BRIEFLY TO MEASURE", SCREEN_W / 2, cy + 28);
    } else {
        // SWR large value
        char swr_buf[12];
        snprintf(swr_buf, sizeof(swr_buf), "%.2f", g_data.swr);
        spr.setTextColor(swr_color(g_data.swr), COL_BG);
        spr.setTextDatum(MC_DATUM);
        spr.setTextSize(4);
        spr.drawString(swr_buf, SCREEN_W / 2, cy + 22);
        spr.setTextColor(COL_LABEL, COL_BG);
        spr.setTextSize(1);
        spr.drawString("SWR", SCREEN_W / 2, cy + 44);
        cy += 52;

        // SWR bar
        drawSWRBar(4, cy, SCREEN_W - 8, 14, g_data.swr);
        cy += 32;

        // Return loss
        char rl_buf[24];
        snprintf(rl_buf, sizeof(rl_buf), "RL: %.1f dB", g_data.rl_db);
        spr.setTextColor(COL_VALUE, COL_BG);
        spr.setTextDatum(MC_DATUM);
        spr.setTextSize(1);
        spr.drawString(rl_buf, SCREEN_W / 2, cy + 6);
    }

    drawTabBar(PAGE_SWR);
}

// =============================================================================
// drawPageSetup — whip setup guide page
// =============================================================================
void drawPageSetup() {
    drawTitleBar("WHIP SETUP GUIDE");

    int y = CONTENT_Y + 4;
    const int lx = 8;
    const int vx = 140;

    // Extension label + value large
    spr.setTextColor(COL_LABEL, COL_BG);
    spr.setTextDatum(ML_DATUM);
    spr.setTextSize(1);
    spr.drawString("EXTENSION:", lx, y + 8);
    spr.setTextColor(COL_TITLE, COL_BG);
    spr.setTextSize(2);
    char ext_buf[16];
    snprintf(ext_buf, sizeof(ext_buf), "%.3f m", g_data.extension_m);
    spr.drawString(ext_buf, vx, y + 6);
    y += 22;

    // Visual bar: 0–5.6m scale, full width
    {
        int bx = lx, by = y, bw = SCREEN_W - 2 * lx, bh = 10;
        spr.fillRect(bx, by, bw, bh, 0x1082);
        spr.drawRect(bx, by, bw, bh, COL_BORDER);
        float frac = g_data.extension_m / 5.6f;
        if (frac > 1.0f) frac = 1.0f;
        if (frac < 0.0f) frac = 0.0f;
        int fw = (int)(frac * (float)(bw - 2));
        if (fw > 0) spr.fillRect(bx + 1, by + 1, fw, bh - 2, COL_VALUE);
        // Tick at 5.6m end
        spr.setTextColor(COL_LABEL, COL_BG);
        spr.setTextDatum(TR_DATUM);
        spr.setTextSize(1);
        spr.drawString("5.6m", bx + bw, by + bh + 2);
        spr.setTextDatum(TL_DATUM);
        spr.drawString("0", bx, by + bh + 2);
    }
    y += 22;

    // Loading coil section
    spr.setTextColor(COL_LABEL, COL_BG);
    spr.setTextDatum(ML_DATUM);
    spr.setTextSize(1);
    spr.drawString("LOADING COIL:", lx, y + 8);
    y += 16;

    if (g_data.coil_type == 1 && g_data.coil_L_uH > 0.0f) {
        spr.setTextColor(COL_ORANGE, COL_BG);
        spr.setTextSize(1);
        spr.drawString("INSTALL LOADING COIL", lx, y + 6);
        y += 14;
        char coil_buf[32];
        snprintf(coil_buf, sizeof(coil_buf), "L = %.3f uH", g_data.coil_L_uH);
        spr.setTextColor(COL_VALUE, COL_BG);
        spr.drawString(coil_buf, lx, y + 6);
        y += 14;
        // match_note if available
        if (g_data.match_note[0]) {
            spr.setTextColor(COL_OK, COL_BG);
            // Wrap long notes manually at ~40 chars
            char note_buf[64];
            strncpy(note_buf, g_data.match_note, sizeof(note_buf) - 1);
            note_buf[sizeof(note_buf) - 1] = '\0';
            if (strlen(note_buf) > 38) note_buf[38] = '\0';
            spr.drawString(note_buf, lx, y + 6);
            y += 14;
        }
    } else if (g_data.coil_L_uH == 0.0f && g_data.extension_m < 5.6f - 0.05f) {
        // extension < 5.6 and no coil: check if it's 20m series-cap band
        // Use heuristic: 20m band has extension 5.13m
        if (g_data.extension_m > 5.0f) {
            spr.setTextColor(COL_WARN, COL_BG);
            spr.setTextSize(1);
            spr.drawString("ADD 300pF SERIES CAP AT BASE", lx, y + 6);
        } else {
            spr.setTextColor(COL_OK, COL_BG);
            spr.setTextSize(1);
            spr.drawString("NO COIL NEEDED", lx, y + 6);
            y += 14;
            spr.setTextColor(COL_LABEL, COL_BG);
            spr.drawString("DIRECT FEED", lx, y + 6);
        }
    } else {
        spr.setTextColor(COL_OK, COL_BG);
        spr.setTextSize(1);
        spr.drawString("NO COIL NEEDED — DIRECT FEED", lx, y + 6);
    }
    y += 20;

    // Tuning one-liner
    spr.drawFastHLine(lx, y, SCREEN_W - 2 * lx, COL_BORDER);
    y += 4;
    spr.setTextColor(COL_LABEL, COL_BG);
    spr.setTextSize(1);
    if (g_data.coil_type == 1) {
        spr.drawString("Extend fully; trim L-network for min SWR", lx, y + 6);
    } else if (g_data.extension_m > 5.0f) {
        spr.drawString("Retract to mark; trim 300pF cap for min SWR", lx, y + 6);
    } else {
        spr.drawString("Retract to mark; direct feed or gamma cap", lx, y + 6);
    }

    drawTabBar(PAGE_SETUP);
}

// =============================================================================
// drawPagePower — detailed power page
// =============================================================================
void drawPagePower() {
    drawTitleBar("RF POWER");

    int y = CONTENT_Y + 8;
    const int lx = 12;

    spr.setTextDatum(ML_DATUM);
    spr.setTextSize(1);

    if (!g_data.valid) {
        spr.setTextColor(COL_LABEL, COL_BG);
        spr.setTextDatum(MC_DATUM);
        spr.drawString("NO RF DETECTED", SCREEN_W / 2, SCREEN_H / 2 - 20);
        drawTabBar(PAGE_POWER);
        return;
    }

    // Forward power — large display
    spr.setTextColor(COL_LABEL, COL_BG);
    spr.setTextDatum(ML_DATUM);
    spr.drawString("FORWARD POWER", lx, y);
    y += 12;

    char fwd_w_buf[24], fwd_dbm_buf[24];
    if (g_data.fwd_w >= 1.0f)
        snprintf(fwd_w_buf, sizeof(fwd_w_buf), "%.2f W", g_data.fwd_w);
    else
        snprintf(fwd_w_buf, sizeof(fwd_w_buf), "%.0f mW", g_data.fwd_w * 1000.0f);
    float fwd_dbm = 10.0f * log10f(g_data.fwd_w * 1000.0f);  // W → mW → dBm
    snprintf(fwd_dbm_buf, sizeof(fwd_dbm_buf), "(%.1f dBm)", fwd_dbm);

    spr.setTextColor(COL_OK, COL_BG);
    spr.setTextSize(3);
    spr.drawString(fwd_w_buf, lx, y);
    spr.setTextColor(COL_LABEL, COL_BG);
    spr.setTextSize(1);
    spr.drawString(fwd_dbm_buf, lx, y + 28);
    y += 44;

    spr.drawFastHLine(lx, y, SCREEN_W - 2 * lx, COL_BORDER);
    y += 6;

    // Reflected power
    spr.setTextColor(COL_LABEL, COL_BG);
    spr.setTextDatum(ML_DATUM);
    spr.setTextSize(1);
    spr.drawString("REFLECTED:", lx, y);
    spr.setTextColor(COL_BAD, COL_BG);
    char refl_buf[24];
    snprintf(refl_buf, sizeof(refl_buf), "%.1f dBm", g_data.refl_dbm);
    spr.drawString(refl_buf, lx + 90, y);
    y += 16;

    // Return loss
    spr.setTextColor(COL_LABEL, COL_BG);
    spr.drawString("RETURN LOSS:", lx, y);
    spr.setTextColor(COL_VALUE, COL_BG);
    char rl_buf[24];
    snprintf(rl_buf, sizeof(rl_buf), "%.1f dB", g_data.rl_db);
    spr.drawString(rl_buf, lx + 90, y);
    y += 16;

    // Rho (reflection coefficient) — derive from SWR
    float rho = (g_data.swr - 1.0f) / (g_data.swr + 1.0f);
    spr.setTextColor(COL_LABEL, COL_BG);
    spr.drawString("RHO:", lx, y);
    spr.setTextColor(COL_VALUE, COL_BG);
    char rho_buf[16];
    snprintf(rho_buf, sizeof(rho_buf), "%.4f", rho);
    spr.drawString(rho_buf, lx + 90, y);
    y += 16;

    // SWR row
    spr.setTextColor(COL_LABEL, COL_BG);
    spr.drawString("SWR:", lx, y);
    spr.setTextColor(swr_color(g_data.swr), COL_BG);
    spr.setTextSize(2);
    char swr_buf[12];
    snprintf(swr_buf, sizeof(swr_buf), "%.2f", g_data.swr);
    spr.drawString(swr_buf, lx + 60, y - 4);

    drawTabBar(PAGE_POWER);
}

// =============================================================================
// drawPageGuide — 4-line band-appropriate tuning procedure
// =============================================================================
void drawPageGuide() {
    drawTitleBar("TUNING GUIDE");

    int y = CONTENT_Y + 6;
    const int lx = 6;
    const int lh = 18;

    spr.setTextDatum(ML_DATUM);
    spr.setTextColor(COL_TITLE, COL_BG);
    spr.setTextSize(1);

    char line1[48], line2[48], line3[48], line4[48];

    if (g_data.coil_type == 1) {
        // Loading coil bands (160m–30m)
        snprintf(line1, sizeof(line1), "1. Install loading coil at whip base");
        snprintf(line2, sizeof(line2), "   L = %.3f uH", g_data.coil_L_uH);
        snprintf(line3, sizeof(line3), "2. Extend whip fully to 5.60m");
        snprintf(line4, sizeof(line4), "3. Adjust L-network trimmer: min SWR");
    } else if (g_data.extension_m > 5.0f) {
        // 20m — series cap
        snprintf(line1, sizeof(line1), "1. Remove loading coil if fitted");
        snprintf(line2, sizeof(line2), "2. Retract whip to %.2fm", g_data.extension_m);
        snprintf(line3, sizeof(line3), "3. Install 300pF series cap at base");
        snprintf(line4, sizeof(line4), "4. Trim capacitor for minimum SWR");
    } else if (g_data.extension_m > 1.0f) {
        // HF/VHF retract bands
        snprintf(line1, sizeof(line1), "1. Remove loading coil if fitted");
        snprintf(line2, sizeof(line2), "2. Retract whip to %.3fm", g_data.extension_m);
        snprintf(line3, sizeof(line3), "3. Direct feed — no components needed");
        snprintf(line4, sizeof(line4), "4. Trim gamma cap if SWR > 1.5");
    } else {
        // VHF/UHF short sections
        snprintf(line1, sizeof(line1), "1. Collapse whip to minimum");
        snprintf(line2, sizeof(line2), "2. Extend tip sections: %.3fm", g_data.extension_m);
        snprintf(line3, sizeof(line3), "3. Direct coax feed at base");
        snprintf(line4, sizeof(line4), "4. Check SWR with brief TX burst");
    }

    spr.drawString(line1, lx, y);           y += lh;
    spr.setTextColor(COL_VALUE, COL_BG);
    spr.drawString(line2, lx, y);           y += lh;
    spr.setTextColor(COL_TITLE, COL_BG);
    spr.drawString(line3, lx, y);           y += lh;
    spr.setTextColor(COL_LABEL, COL_BG);
    spr.drawString(line4, lx, y);           y += lh + 4;

    spr.drawFastHLine(lx, y, SCREEN_W - 2 * lx, COL_BORDER);
    y += 6;

    // Current band context
    spr.setTextColor(COL_LABEL, COL_BG);
    char ctx[48];
    snprintf(ctx, sizeof(ctx), "Band: %s   %.3f MHz   ext=%.3fm",
             g_data.band[0] ? g_data.band : "---",
             g_data.freq_mhz,
             g_data.extension_m);
    spr.drawString(ctx, lx, y);

    drawTabBar(PAGE_GUIDE);
}

// =============================================================================
// setup
// =============================================================================
void setup() {
    Serial.begin(115200);
    Serial.println("\n=== CYD Whip Display ===");

    // TFT init
    tft.init();
    tft.setRotation(1);  // Landscape
    tft.fillScreen(COL_BG);
    tft.setTextColor(COL_TITLE, COL_BG);
    tft.setTextSize(1);

    // Touch calibration (raw → pixel)
    uint16_t calData[5] = {TOUCH_MIN_X, TOUCH_MAX_X, TOUCH_MIN_Y, TOUCH_MAX_Y, 1};
    tft.setTouch(calData);

    // Sprite (full-screen double-buffer)
    spr.createSprite(SCREEN_W, SCREEN_H);
    spr.setColorDepth(16);

    // Splash screen
    tft.fillScreen(0x0010);
    tft.setTextColor(COL_TITLE, 0x0010);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(2);
    tft.drawString("WHIP TUNER", SCREEN_W / 2, SCREEN_H / 2 - 20);
    tft.setTextColor(COL_VALUE, 0x0010);
    tft.setTextSize(1);
    tft.drawString("5.6m Telescopic + Bruene Bridge", SCREEN_W / 2, SCREEN_H / 2);
    tft.drawString("Connecting to WHP-Tuner...", SCREEN_W / 2, SCREEN_H / 2 + 16);
    delay(1200);

    // BT: master mode, connect to tuner
    BT.begin("CYD-WHP", true);  // true = master mode
    Serial.println("[BT] Connecting to WHP-Tuner...");
    bool ok = BT.connect("WHP-Tuner");
    if (ok) {
        g_data.connected = true;
        Serial.println("[BT] Connected.");
        tft.fillScreen(COL_BG);
        tft.setTextColor(COL_OK, COL_BG);
        tft.setTextDatum(MC_DATUM);
        tft.drawString("BT CONNECTED", SCREEN_W / 2, SCREEN_H / 2);
        delay(500);
    } else {
        Serial.println("[BT] Not connected — will retry in loop.");
        tft.fillScreen(COL_BG);
        tft.setTextColor(COL_BAD, COL_BG);
        tft.setTextDatum(MC_DATUM);
        tft.drawString("BT NOT FOUND", SCREEN_W / 2, SCREEN_H / 2);
        tft.setTextColor(COL_LABEL, COL_BG);
        tft.drawString("Will retry...", SCREEN_W / 2, SCREEN_H / 2 + 16);
        delay(800);
    }

    // Initialise data struct defaults
    strncpy(g_data.band, "---", sizeof(g_data.band));
    g_data.valid     = false;
    g_data.freq_mhz  = 0.0f;
    g_data.extension_m = 0.0f;
    g_data.coil_L_uH   = 0.0f;
    g_data.coil_type   = 0;
    g_data.swr         = 0.0f;
    g_data.fwd_w       = 0.0f;
    g_data.refl_dbm    = -99.0f;
    g_data.rl_db       = 0.0f;
    g_data.match_note[0] = '\0';

    last_packet_ms   = millis();
    last_reconnect_ms = millis();

    Serial.println("Setup complete.");
}

// =============================================================================
// loop — 10 FPS via sprite.pushSprite
// =============================================================================
void loop() {
    maintainBT();
    readBT();
    checkTouch();

    unsigned long now = millis();
    if (now - last_render_ms < 100) return;  // 10 FPS cap
    last_render_ms = now;

    // Clear sprite
    spr.fillSprite(COL_BG);

    // Render active page
    switch (g_page) {
        case PAGE_SWR:   drawPageSWR();   break;
        case PAGE_SETUP: drawPageSetup(); break;
        case PAGE_POWER: drawPagePower(); break;
        case PAGE_GUIDE: drawPageGuide(); break;
        default:         drawPageSWR();   break;
    }

    // Push sprite to display
    spr.pushSprite(0, 0);
}
