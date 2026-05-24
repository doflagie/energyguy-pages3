// ============================================================
//  cyd_antenna_display.ino — EFQW Tuner CYD Display
//  TM-EF-FW-002 Rev A
//  Receives BT stream from EFQW-Tuner, displays SWR bar,
//  band info, power, return loss, and tuning guide.
//  Hardware: ESP32-2432S028 (CYD) — 320×240 ILI9341 + XPT2046
// ============================================================

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <BluetoothSerial.h>

// ============================================================
// Display setup
// ============================================================
TFT_eSPI     tft  = TFT_eSPI();
TFT_eSprite  spr  = TFT_eSprite(&tft);

BluetoothSerial BT;

// ============================================================
// Color palette
// ============================================================
#define COL_BG       0x0000   // black
#define COL_TITLE    0xFFE0   // yellow
#define COL_LABEL    0x7BEF   // light grey
#define COL_VALUE    0x07FF   // cyan
#define COL_OK       0x07E0   // green
#define COL_WARN     0xFFE0   // yellow
#define COL_BAD      0xF800   // red
#define COL_BORDER   0x4208   // dark grey
#define COL_BAR_BG   0x2104   // dark bar track
#define COL_WHITE    0xFFFF

// ============================================================
// Pages
// ============================================================
enum Page { PAGE_SWR, PAGE_POWER, PAGE_GUIDE, PAGE_COUNT };
Page g_page = PAGE_SWR;

// ============================================================
// Data from BT
// ============================================================
struct TunerData {
    char    band[8];
    float   freq_mhz;
    float   swr;
    float   fwd_w;
    float   refl_dbm;
    float   rl_db;
    bool    valid;
    bool    connected;
};
TunerData g_data = {"---", 0, 0, 0, -99, 0, false, false};

// ============================================================
// BT parsing
// "BAND:40m,FREQ:7.150,SWR:1.23,FWD:4.750W,REFL:-18.2,RL:14.1,VALID:1\n"
// ============================================================
String bt_buf = "";

void parseBTLine(const String& line) {
    auto getVal = [&](const char* key) -> String {
        int i = line.indexOf(key);
        if (i < 0) return "";
        i += strlen(key);
        int j = line.indexOf(',', i);
        return (j < 0) ? line.substring(i) : line.substring(i, j);
    };

    String band = getVal("BAND:");
    if (band.length() > 0 && band.length() < 8)
        strncpy(g_data.band, band.c_str(), sizeof(g_data.band)-1);

    String f = getVal("FREQ:");
    if (f.length()) g_data.freq_mhz = f.toFloat();

    String s = getVal("SWR:");
    if (s.length()) g_data.swr = s.toFloat();

    String fw = getVal("FWD:");
    if (fw.length()) g_data.fwd_w = fw.toFloat();

    String r = getVal("REFL:");
    if (r.length()) g_data.refl_dbm = r.toFloat();

    String rl = getVal("RL:");
    if (rl.length()) g_data.rl_db = rl.toFloat();

    g_data.valid     = getVal("VALID:").toInt() == 1;
    g_data.connected = true;
}

void readBT() {
    while (BT.available()) {
        char c = BT.read();
        if (c == '\n') {
            if (bt_buf.length() > 10) parseBTLine(bt_buf);
            bt_buf = "";
        } else {
            if (bt_buf.length() < 200) bt_buf += c;
        }
    }
}

// ============================================================
// Touch (XPT2046 via TFT_eSPI)
// ============================================================
uint16_t touch_x, touch_y;

bool checkTouch() {
    uint16_t tx, ty;
    if (tft.getTouch(&tx, &ty)) {
        touch_x = tx;
        touch_y = ty;
        return true;
    }
    return false;
}

// ============================================================
// SWR bar (horizontal)
// x, y: top-left of bar region
// w, h: width, height
// swr: value
// ============================================================
void drawSWRBar(int x, int y, int w, int h, float swr) {
    float frac = 0;
    if (swr > 1.0f) {
        // map SWR 1→10 to 0→1.0
        frac = constrain((swr - 1.0f) / 9.0f, 0, 1.0f);
    }
    int fill = (int)(frac * w);

    uint16_t col = COL_OK;
    if (swr >= 1.5f && swr < 2.5f) col = COL_WARN;
    if (swr >= 2.5f)                col = COL_BAD;

    spr.fillRect(x, y, w, h, COL_BAR_BG);
    spr.fillRect(x, y, fill, h, col);
    spr.drawRect(x-1, y-1, w+2, h+2, COL_BORDER);

    // Tick marks at SWR 1.5, 2.0, 3.0, 5.0
    int ticks[] = {1, 2, 3, 5, 10};
    for (int t : ticks) {
        float tf = (t - 1.0f) / 9.0f;
        int tx = x + (int)(tf * w);
        spr.drawFastVLine(tx, y - 3, h + 6, COL_LABEL);
        spr.setTextColor(COL_LABEL, COL_BG);
        spr.setTextSize(1);
        char tb[4]; snprintf(tb, sizeof(tb), "%d", t);
        spr.drawString(tb, tx - 3, y + h + 4);
    }
}

// ============================================================
// PAGE: SWR
// ============================================================
void drawPageSWR() {
    int W = 320, H = 240;
    spr.fillSprite(COL_BG);

    // Title bar
    spr.fillRect(0, 0, W, 24, 0x1082);
    spr.setTextColor(COL_TITLE, 0x1082);
    spr.setTextSize(2);
    spr.drawString("EFQW SWR MONITOR", 6, 4);

    // Connection indicator
    uint16_t conn_col = g_data.connected ? COL_OK : COL_BAD;
    spr.fillCircle(W-12, 12, 6, conn_col);

    // Band / freq
    spr.setTextColor(COL_LABEL, COL_BG);
    spr.setTextSize(1);
    spr.drawString("BAND", 8, 32);
    spr.setTextColor(COL_VALUE, COL_BG);
    spr.setTextSize(2);
    char bf[32];
    snprintf(bf, sizeof(bf), "%s  %.3f MHz", g_data.band, g_data.freq_mhz);
    spr.drawString(bf, 8, 44);

    // SWR large value
    spr.setTextColor(COL_LABEL, COL_BG);
    spr.setTextSize(1);
    spr.drawString("SWR", 8, 72);

    uint16_t swr_col = COL_OK;
    if (g_data.swr >= 1.5f && g_data.swr < 2.5f) swr_col = COL_WARN;
    if (g_data.swr >= 2.5f)                       swr_col = COL_BAD;

    spr.setTextColor(swr_col, COL_BG);
    spr.setTextSize(4);
    if (!g_data.valid) {
        spr.drawString("-- : 1", 8, 84);
    } else {
        char sb[16];
        snprintf(sb, sizeof(sb), "%.2f : 1", g_data.swr);
        spr.drawString(sb, 8, 84);
    }

    // SWR bar
    drawSWRBar(8, 138, W-16, 18, g_data.valid ? g_data.swr : 1.0f);

    // Return loss
    spr.setTextColor(COL_LABEL, COL_BG);
    spr.setTextSize(1);
    spr.drawString("RETURN LOSS", 8, 168);
    spr.setTextColor(COL_VALUE, COL_BG);
    spr.setTextSize(2);
    char rl[20];
    snprintf(rl, sizeof(rl), "%.1f dB", g_data.valid ? g_data.rl_db : 0.0f);
    spr.drawString(rl, 8, 180);

    // No-RF warning
    if (!g_data.valid) {
        spr.setTextColor(COL_WARN, COL_BG);
        spr.setTextSize(1);
        spr.drawString("TRANSMIT BRIEFLY TO TUNE", 8, 210);
    }

    // Bottom tab bar
    drawTabBar(0);
}

// ============================================================
// PAGE: Power
// ============================================================
void drawPagePower() {
    int W = 320, H = 240;
    spr.fillSprite(COL_BG);

    spr.fillRect(0, 0, W, 24, 0x1082);
    spr.setTextColor(COL_TITLE, 0x1082);
    spr.setTextSize(2);
    spr.drawString("RF POWER", 6, 4);
    spr.fillCircle(W-12, 12, 6, g_data.connected ? COL_OK : COL_BAD);

    // Forward power
    spr.setTextColor(COL_LABEL, COL_BG);
    spr.setTextSize(1);
    spr.drawString("FORWARD POWER", 8, 32);
    spr.setTextColor(COL_VALUE, COL_BG);
    spr.setTextSize(3);
    char fw[32];
    if (g_data.valid && g_data.fwd_w >= 1.0f)
        snprintf(fw, sizeof(fw), "%.2f W", g_data.fwd_w);
    else if (g_data.valid)
        snprintf(fw, sizeof(fw), "%.0f mW", g_data.fwd_w * 1000.0f);
    else
        snprintf(fw, sizeof(fw), "--- W");
    spr.drawString(fw, 8, 44);

    // dBm
    spr.setTextColor(COL_LABEL, COL_BG);
    spr.setTextSize(1);
    spr.drawString("FWD dBm", 8, 86);
    spr.setTextColor(COL_VALUE, COL_BG);
    spr.setTextSize(2);
    float fdbm = g_data.valid ? (10.0f*log10f(g_data.fwd_w) + 30.0f) : -99.0f;
    char fdbms[16]; snprintf(fdbms, sizeof(fdbms), "%.1f dBm", fdbm);
    spr.drawString(fdbms, 8, 98);

    // Reflected power
    spr.setTextColor(COL_LABEL, COL_BG);
    spr.setTextSize(1);
    spr.drawString("REFLECTED", 8, 128);
    spr.setTextColor(COL_WARN, COL_BG);
    spr.setTextSize(2);
    char rb[20]; snprintf(rb, sizeof(rb), "%.1f dBm", g_data.valid ? g_data.refl_dbm : -99.0f);
    spr.drawString(rb, 8, 140);

    // ρ
    float rho = (g_data.valid && g_data.swr > 1.0f) ?
                (g_data.swr - 1.0f) / (g_data.swr + 1.0f) : 0.0f;
    spr.setTextColor(COL_LABEL, COL_BG);
    spr.setTextSize(1);
    spr.drawString("REFL COEFF (rho)", 8, 168);
    spr.setTextColor(COL_VALUE, COL_BG);
    spr.setTextSize(2);
    char rhos[16]; snprintf(rhos, sizeof(rhos), "%.3f", rho);
    spr.drawString(rhos, 8, 180);

    drawTabBar(1);
}

// ============================================================
// PAGE: Tuning Guide
// ============================================================
void drawPageGuide() {
    int W = 320;
    spr.fillSprite(COL_BG);

    spr.fillRect(0, 0, W, 24, 0x1082);
    spr.setTextColor(COL_TITLE, 0x1082);
    spr.setTextSize(2);
    spr.drawString("TUNING GUIDE", 6, 4);

    spr.setTextColor(COL_VALUE, COL_BG);
    spr.setTextSize(1);
    int y = 32;

    // Instructions depend on band
    bool is_hf  = (g_data.freq_mhz <  54);
    bool is_vhf = (g_data.freq_mhz >= 54  && g_data.freq_mhz < 400);
    bool is_uhf = (g_data.freq_mhz >= 400);

    if (is_hf) {
        spr.drawString("HF L-NETWORK TUNING:", 8, y); y+=14;
        spr.setTextColor(COL_LABEL, COL_BG);
        spr.drawString("1. Set band with physical switch", 8, y); y+=12;
        spr.drawString("2. Transmit CW or low-duty tone", 8, y); y+=12;
        spr.drawString("3. Adjust trimmer C2 CW = more C", 8, y); y+=12;
        spr.drawString("   (lowers resonant freq)", 8, y); y+=12;
        spr.drawString("4. Seek minimum SWR reading", 8, y); y+=12;
        spr.drawString("5. Goal: SWR < 1.5 (green bar)", 8, y); y+=16;
        spr.setTextColor(COL_VALUE, COL_BG);
        spr.drawString("CHOKE BALUN: essential on HF!", 8, y); y+=12;
        spr.setTextColor(COL_LABEL, COL_BG);
        spr.drawString("8T RG-174 on FT-240-31 at feed", 8, y); y+=16;
        spr.setTextColor(COL_VALUE, COL_BG);
        char ql[48];
        snprintf(ql, sizeof(ql), "QW length: %.3f m  (%.1f ft)",
                 g_data.fwd_w, g_data.fwd_w*3.281f);  // fwd_w reused as qw placeholder
        spr.drawString("See band guide for wire length", 8, y);
    } else if (is_vhf) {
        spr.drawString("VHF GAMMA MATCH TUNING:", 8, y); y+=14;
        spr.setTextColor(COL_LABEL, COL_BG);
        spr.drawString("1. Set gamma rod = 15% element L", 8, y); y+=12;
        spr.drawString("2. Rod spacing ~15mm from element", 8, y); y+=12;
        spr.drawString("3. Adjust C_gamma (Teflon trimmer)", 8, y); y+=12;
        spr.drawString("4. Slide rod position for SWR dip", 8, y); y+=12;
        spr.drawString("5. Goal: SWR < 1.5", 8, y); y+=16;
        spr.setTextColor(COL_VALUE, COL_BG);
        spr.drawString("SLEEVE BALUN: lambda/4 tube over coax", 8, y);
    } else {
        spr.drawString("UHF DIRECT FEED TUNING:", 8, y); y+=14;
        spr.setTextColor(COL_LABEL, COL_BG);
        spr.drawString("1. Verify element length per table", 8, y); y+=12;
        spr.drawString("2. Trim tip by 2mm for SWR adjust", 8, y); y+=12;
        spr.drawString("3. Check connector solder quality", 8, y); y+=12;
        spr.drawString("4. Keep ground plane perpendicular", 8, y); y+=12;
        spr.drawString("5. NEC-2 accuracy limited at UHF", 8, y);
    }

    drawTabBar(2);
}

// ============================================================
// Bottom tab bar
// ============================================================
void drawTabBar(int active) {
    int W = 320, y = 218, h = 22;
    int tw = W / PAGE_COUNT;
    const char* labels[] = {"SWR", "POWER", "GUIDE"};
    for (int i = 0; i < PAGE_COUNT; i++) {
        uint16_t bg = (i == active) ? 0x3186 : 0x1082;
        uint16_t fg = (i == active) ? COL_WHITE : COL_LABEL;
        spr.fillRect(i*tw, y, tw, h, bg);
        spr.setTextColor(fg, bg);
        spr.setTextSize(1);
        spr.drawCentreString(labels[i], i*tw + tw/2, y+7, 1);
    }
}

// ============================================================
// Reconnect logic
// ============================================================
static uint32_t last_reconnect = 0;
static bool     bt_connecting  = false;

void maintainBT() {
    if (!g_data.connected && !bt_connecting) {
        uint32_t now = millis();
        if (now - last_reconnect > 5000) {
            last_reconnect = now;
            BT.connect("EFQW-Tuner");
            bt_connecting = true;
        }
    }
    if (bt_connecting && BT.connected()) {
        bt_connecting = false;
        g_data.connected = true;
    }
    // Detect disconnect
    if (g_data.connected && !BT.connected()) {
        g_data.connected = false;
        g_data.valid     = false;
    }
}

// ============================================================
// Setup
// ============================================================
void setup() {
    Serial.begin(115200);

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(COL_BG);
    spr.createSprite(320, 240);

    // Splash
    tft.setTextColor(COL_TITLE, COL_BG);
    tft.setTextSize(2);
    tft.drawCentreString("EFQW ANTENNA TUNER", 160, 80, 1);
    tft.setTextColor(COL_LABEL, COL_BG);
    tft.setTextSize(1);
    tft.drawCentreString("Connecting to EFQW-Tuner...", 160, 120, 1);
    delay(1000);

    BT.begin("CYD-EFTuner", true);  // master mode
    BT.connect("EFQW-Tuner");
}

// ============================================================
// Loop
// ============================================================
void loop() {
    maintainBT();
    readBT();

    // Touch: switch pages
    if (checkTouch()) {
        int tab_y = 218;
        if (touch_y > tab_y) {
            int tab = touch_x / (320 / PAGE_COUNT);
            if (tab >= 0 && tab < PAGE_COUNT) g_page = (Page)tab;
        }
        delay(150);
    }

    // Render at ~10 FPS
    static uint32_t last_draw = 0;
    if (millis() - last_draw >= 100) {
        last_draw = millis();

        switch (g_page) {
            case PAGE_SWR:   drawPageSWR();   break;
            case PAGE_POWER: drawPagePower(); break;
            case PAGE_GUIDE: drawPageGuide(); break;
            default: break;
        }

        spr.pushSprite(0, 0);
    }
}
