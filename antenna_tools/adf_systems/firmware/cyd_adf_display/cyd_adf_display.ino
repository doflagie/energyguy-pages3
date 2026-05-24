// ============================================================
// cyd_adf_display.ino — TM-ADF-008 Rev A
// CYD (ESP32-2432S028R) Touchscreen ADF Display
// 320×240 ILI9341 + XPT2046 touch + BT master
//
// Connects via Bluetooth SPP to:
//   RF-ADFHFinder (Watson-Watt HF)
//   RF-ADFVHFinder (Doppler VHF)
//   RF-LoopRDF     (Rotary loop)
// Displays: compass rose with bearing needle, signal strength,
//           GPS coordinates, band info, mode selection
//
// Required libraries:
//   TFT_eSPI (configured for ILI9341, 320x240)
//   XPT2046_Touchscreen
//   BluetoothSerial (ESP32 built-in)
// ============================================================

#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <BluetoothSerial.h>
#include <math.h>

// ─── CYD Hardware ───────────────────────────────────────────
#define TFT_BL      21    // Backlight
#define TOUCH_CS    33
#define TOUCH_IRQ   36
TFT_eSPI         tft;
TFT_eSprite      spr = TFT_eSprite(&tft);
XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);
BluetoothSerial  SerialBT;

// ─── Colors ─────────────────────────────────────────────────
#define COL_BG      TFT_BLACK
#define COL_RING    0x07E0        // green
#define COL_RING2   0x03E0        // dark green
#define COL_NEEDLE  TFT_RED
#define COL_NORTH   0xF800        // red
#define COL_TEXT    TFT_WHITE
#define COL_DIM     0x4208        // gray
#define COL_GOOD    0x07E0        // green
#define COL_WARN    0xFFA0        // orange
#define COL_BAD     TFT_RED
#define COL_CARD    0x1082        // very dark gray for cards

// ─── Display Modes ──────────────────────────────────────────
enum DisplayMode { MODE_COMPASS, MODE_MAP, MODE_BANDS, MODE_SETTINGS };
static DisplayMode g_mode = MODE_COMPASS;

// ─── ADF Data State ─────────────────────────────────────────
struct ADFData {
    float    bearing_mag    = -1.0f;
    float    bearing_true   = -1.0f;
    float    snr_db         = 0.0f;
    float    confidence     = 0.0f;
    float    rssi_dbm       = -120.0f;
    float    gps_lat        = 0.0f;
    float    gps_lon        = 0.0f;
    uint8_t  gps_sats       = 0;
    char     band[8]        = "---";
    char     mode[12]       = "---";
    bool     valid          = false;
    uint32_t last_update_ms = 0;
};

static ADFData  g_adf;
static bool     g_bt_connected = false;
static char     g_bt_target[24] = "RF-ADFHFinder";  // default target

// ─── BT data parsing ────────────────────────────────────────
static char bt_buf[256];
static uint8_t bt_pos = 0;

void parseADFPacket(const char* pkt) {
    // Format: BRG:045.2,TRUE:046.1,SNR:18.3,CNF:0.87,BAND:40M,LAT:37.3456,LON:-120.456,SAT:8
    // or VHF: BRG:135.4,RSSI:-78,CNF:0.91,BAND:2M
    // or loop: POS:247.3,NULL:127.1,DEPTH:-24.1,CNF:0.88,BAND:40M
    auto getField = [&](const char* key) -> float {
        const char* p = strstr(pkt, key);
        if (!p) return 0.0f;
        p += strlen(key);
        return atof(p);
    };
    auto getStrField = [&](const char* key, char* out, size_t olen) {
        const char* p = strstr(pkt, key);
        if (!p) return;
        p += strlen(key);
        size_t i = 0;
        while (*p && *p != ',' && i < olen-1) out[i++] = *p++;
        out[i] = '\0';
    };

    // Check packet type
    if (strstr(pkt, "NULL:")) {
        // Rotary loop packet
        float null_brg = getField("NULL:");
        if (null_brg > 0.0f || strstr(pkt, "NULL:0")) {
            g_adf.bearing_mag  = null_brg;
            g_adf.bearing_true = null_brg;
            g_adf.snr_db       = -getField("DEPTH:");  // depth as SNR proxy
            g_adf.confidence   = getField("CNF:");
        }
    } else {
        g_adf.bearing_mag  = getField("BRG:");
        g_adf.bearing_true = getField("TRUE:");
        g_adf.snr_db       = strstr(pkt,"SNR:") ? getField("SNR:") : 0.0f;
        g_adf.confidence   = getField("CNF:");
        g_adf.rssi_dbm     = strstr(pkt,"RSSI:") ? getField("RSSI:") : -100.0f;
        g_adf.gps_lat      = getField("LAT:");
        g_adf.gps_lon      = getField("LON:");
        g_adf.gps_sats     = (uint8_t)getField("SAT:");
    }

    getStrField("BAND:", g_adf.band, sizeof(g_adf.band));
    getStrField("MODE:", g_adf.mode, sizeof(g_adf.mode));

    g_adf.valid          = (g_adf.bearing_mag >= 0.0f && g_adf.confidence > 0.1f);
    g_adf.last_update_ms = millis();
}

void parseBTStream() {
    while (SerialBT.available()) {
        char c = SerialBT.read();
        if (c == '\n') {
            bt_buf[bt_pos] = '\0';
            if (bt_pos > 2) parseADFPacket(bt_buf);
            bt_pos = 0;
        } else if (bt_pos < 255) {
            bt_buf[bt_pos++] = c;
        }
    }
}

// ─── Compass Rose drawing ───────────────────────────────────
// Draws into a sprite then pushes to TFT

void drawCompassRose(int cx, int cy, int r, float bearing_deg, float conf) {
    // Draw rings
    spr.drawCircle(cx, cy, r,     COL_RING);
    spr.drawCircle(cx, cy, r*2/3, COL_RING2);
    spr.drawCircle(cx, cy, r/3,   COL_RING2);

    // Cardinal tick marks and labels
    const char* cards[] = {"N","NE","E","SE","S","SW","W","NW"};
    for (int i = 0; i < 8; i++) {
        float a = i * 45.0f * (float)M_PI / 180.0f;
        float sa = sinf(a), ca = -cosf(a);
        int x1 = cx + (int)((r-4)*sa),  y1 = cy + (int)((r-4)*ca);
        int x2 = cx + (int)((r+3)*sa),  y2 = cy + (int)((r+3)*ca);
        spr.drawLine(x1, y1, x2, y2, (i == 0) ? COL_NORTH : COL_RING);
        if (i % 2 == 0) {
            int tx = cx + (int)((r-15)*sa) - 4;
            int ty = cy + (int)((r-15)*ca) - 4;
            spr.setTextColor((i == 0) ? COL_NORTH : COL_TEXT);
            spr.setTextSize(1);
            spr.setCursor(tx, ty);
            spr.print(cards[i]);
        }
    }

    // 5-degree tick marks
    for (int i = 0; i < 72; i++) {
        float a = i * 5.0f * (float)M_PI / 180.0f;
        float sa = sinf(a), ca = -cosf(a);
        int ticklen = (i % 9 == 0) ? 8 : (i % 3 == 0 ? 5 : 3);
        int x1 = cx + (int)((r-ticklen)*sa), y1 = cy + (int)((r-ticklen)*ca);
        int x2 = cx + (int)(r*sa),            y2 = cy + (int)(r*ca);
        spr.drawLine(x1, y1, x2, y2, COL_RING2);
    }

    // Bearing needle
    if (bearing_deg >= 0.0f) {
        float a = bearing_deg * (float)M_PI / 180.0f;
        float sa = sinf(a), ca = -cosf(a);

        // Confidence ring (green = high, red = low)
        uint32_t conf_col = (conf > 0.7f) ? COL_GOOD : (conf > 0.3f ? COL_WARN : COL_BAD);
        spr.drawCircle(cx, cy, r*2/3, conf_col);

        // Needle (thick, red)
        int nx = cx + (int)((r-10)*sa);
        int ny = cy + (int)((r-10)*ca);
        for (int w = -2; w <= 2; w++) {
            spr.drawLine(cx+w, cy, nx+w, ny, COL_NEEDLE);
            spr.drawLine(cx, cy+w, nx, ny+w, COL_NEEDLE);
        }
        // Tail (opposite direction, shorter, dim)
        int tx = cx - (int)((r/4)*sa);
        int ty = cy - (int)((r/4)*ca);
        spr.drawLine(cx, cy, tx, ty, COL_DIM);

        // Bearing text in center
        char bstr[12];
        snprintf(bstr, sizeof(bstr), "%05.1f\xb0", bearing_deg);
        spr.setTextColor(COL_TEXT);
        spr.setTextSize(2);
        spr.setCursor(cx - 30, cy - 10);
        spr.print(bstr);
    } else {
        // No signal indicator
        spr.setTextColor(COL_DIM);
        spr.setTextSize(1);
        spr.setCursor(cx - 16, cy - 4);
        spr.print("NO SIG");
    }

    // Center dot
    spr.fillCircle(cx, cy, 4, COL_NEEDLE);
}

// ─── Signal strength bar ────────────────────────────────────
void drawSignalBar(int x, int y, int w, int h, float conf, float snr) {
    spr.drawRect(x, y, w, h, COL_RING);
    int fill = (int)(conf * (float)(w-2));
    if (fill > 0) {
        uint32_t col = (conf > 0.7f) ? COL_GOOD : (conf > 0.3f ? COL_WARN : COL_BAD);
        spr.fillRect(x+1, y+1, fill, h-2, col);
    }
    spr.setTextColor(COL_TEXT); spr.setTextSize(1);
    spr.setCursor(x + w + 4, y + 2);
    spr.printf("%.0f%%", conf * 100.0f);
}

// ─── MODE: COMPASS ──────────────────────────────────────────
void drawCompassMode() {
    spr.fillSprite(COL_BG);

    // Status bar top
    spr.setTextColor(COL_DIM); spr.setTextSize(1);
    spr.setCursor(2, 2);
    bool stale = (millis() - g_adf.last_update_ms > 3000);
    spr.printf("BT:%s  BAND:%s",
               g_bt_connected ? (stale ? "STALE" : "OK") : "---",
               g_adf.band);

    // Compass rose: centered at (160, 130), radius 100
    drawCompassRose(160, 130, 100, g_adf.bearing_mag, g_adf.confidence);

    // Left panel
    // True bearing
    spr.setTextColor(COL_TEXT); spr.setTextSize(1);
    spr.setCursor(2, 20);
    spr.printf("MAG%05.1f", g_adf.bearing_mag >= 0 ? g_adf.bearing_mag : 0);
    spr.setCursor(2, 32);
    spr.printf("TRU%05.1f", g_adf.bearing_true >= 0 ? g_adf.bearing_true : 0);

    // SNR
    spr.setCursor(2, 48);
    spr.printf("SNR%4.0fdB", g_adf.snr_db);

    // Signal bar
    drawSignalBar(2, 62, 60, 10, g_adf.confidence, g_adf.snr_db);

    // GPS
    spr.setCursor(2, 82);
    if (g_adf.gps_sats > 0) {
        spr.setTextColor(COL_GOOD);
        spr.printf("GPS:%d sat", g_adf.gps_sats);
        spr.setCursor(2, 94);
        spr.setTextColor(COL_DIM);
        spr.printf("%.4f", g_adf.gps_lat);
        spr.setCursor(2, 106);
        spr.printf("%.4f", g_adf.gps_lon);
    } else {
        spr.setTextColor(COL_BAD);
        spr.print("GPS:NO FIX");
    }

    // RSSI
    spr.setTextColor(COL_DIM); spr.setTextSize(1);
    spr.setCursor(2, 120);
    spr.printf("RSSI%.0f", g_adf.rssi_dbm);

    // Mode buttons at bottom
    const char* mode_names[] = {"CMPS","MAP","BAND","SET"};
    DisplayMode modes[]       = {MODE_COMPASS, MODE_MAP, MODE_BANDS, MODE_SETTINGS};
    for (int i = 0; i < 4; i++) {
        int bx = i * 80;
        uint32_t fg = (g_mode == modes[i]) ? COL_BG    : COL_TEXT;
        uint32_t bg = (g_mode == modes[i]) ? COL_RING  : COL_CARD;
        spr.fillRect(bx, 222, 79, 17, bg);
        spr.drawRect(bx, 222, 79, 17, COL_RING);
        spr.setTextColor(fg); spr.setTextSize(1);
        spr.setCursor(bx + 22, 228);
        spr.print(mode_names[i]);
    }

    spr.pushSprite(0, 0);
}

// ─── MODE: BANDS ────────────────────────────────────────────
void drawBandsMode() {
    spr.fillSprite(COL_BG);
    spr.setTextColor(COL_RING); spr.setTextSize(2);
    spr.setCursor(80, 4); spr.print("ADF BANDS");

    const char* hf_bands[]  = {"160M","80M","60M","40M","30M","20M","17M","15M","12M","10M","6M"};
    const char* vhf_bands[] = {"2M","1.25M","70CM","33CM","20CM"};

    spr.setTextSize(1); spr.setTextColor(COL_DIM);
    spr.setCursor(4, 28); spr.print("HF BANDS:");
    for (int i = 0; i < 11; i++) {
        int bx = (i % 4) * 78 + 4;
        int by = 38 + (i / 4) * 22;
        bool active = (strcmp(hf_bands[i], g_adf.band) == 0);
        spr.fillRect(bx, by, 74, 18, active ? COL_RING : COL_CARD);
        spr.drawRect(bx, by, 74, 18, COL_RING2);
        spr.setTextColor(active ? COL_BG : COL_TEXT);
        spr.setCursor(bx+14, by+5);
        spr.print(hf_bands[i]);
    }

    spr.setTextColor(COL_DIM); spr.setCursor(4, 120);
    spr.print("VHF/UHF:");
    for (int i = 0; i < 5; i++) {
        int bx = i * 62 + 4;
        int by = 130;
        bool active = (strcmp(vhf_bands[i], g_adf.band) == 0);
        spr.fillRect(bx, by, 58, 18, active ? COL_RING : COL_CARD);
        spr.drawRect(bx, by, 58, 18, COL_RING2);
        spr.setTextColor(active ? COL_BG : COL_TEXT);
        spr.setCursor(bx+4, by+5);
        spr.print(vhf_bands[i]);
    }

    spr.setTextColor(COL_DIM); spr.setTextSize(1);
    spr.setCursor(4, 168);
    spr.print("Touch band on controller to switch.");

    // Mode bar
    const char* mode_names[] = {"CMPS","MAP","BAND","SET"};
    for (int i = 0; i < 4; i++) {
        int bx = i * 80;
        spr.fillRect(bx, 222, 79, 17, (g_mode == MODE_BANDS && i==2) ? COL_RING : COL_CARD);
        spr.drawRect(bx, 222, 79, 17, COL_RING);
        spr.setTextColor((g_mode == MODE_BANDS && i==2) ? COL_BG : COL_TEXT);
        spr.setCursor(bx+22, 228);
        spr.print(mode_names[i]);
    }

    spr.pushSprite(0, 0);
}

// ─── Touch handling ─────────────────────────────────────────
void handleTouch() {
    if (!touch.tirqTouched() || !touch.touched()) return;
    TS_Point p = touch.getPoint();
    // Map raw coordinates to screen (calibrate for your CYD)
    int sx = map(p.x, 200, 3900, 0, 320);
    int sy = map(p.y, 200, 3900, 0, 240);

    // Mode bar (y > 222)
    if (sy > 222) {
        int tab = sx / 80;
        switch (tab) {
        case 0: g_mode = MODE_COMPASS;  break;
        case 1: g_mode = MODE_MAP;      break;
        case 2: g_mode = MODE_BANDS;    break;
        case 3: g_mode = MODE_SETTINGS; break;
        }
    }
    delay(80);  // debounce
}

// ─── BT reconnect logic ─────────────────────────────────────
static uint32_t g_last_connect_try = 0;

void maintainBT() {
    if (g_bt_connected) return;
    uint32_t now = millis();
    if (now - g_last_connect_try < 5000) return;
    g_last_connect_try = now;
    g_bt_connected = SerialBT.connect(g_bt_target);
}

// ─── setup() ────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);

    // Backlight
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    // TFT init
    tft.init();
    tft.setRotation(1);  // landscape
    tft.fillScreen(COL_BG);

    // Splash
    tft.setTextColor(0x07E0); tft.setTextSize(3);
    tft.setCursor(50, 80);   tft.print("ADF DISPLAY");
    tft.setTextSize(1); tft.setTextColor(TFT_WHITE);
    tft.setCursor(60, 130);  tft.print("TM-ADF-008 Rev A");
    tft.setCursor(80, 150);  tft.print("Connecting BT...");
    delay(1000);

    // Sprite (full screen buffer)
    spr.createSprite(320, 240);
    spr.setColorDepth(16);

    // Touch
    touch.begin();
    touch.setRotation(1);

    // Bluetooth
    SerialBT.begin("CYD-ADFDisplay", true);  // master mode
    g_last_connect_try = 0;  // try immediately
}

// ─── loop() ─────────────────────────────────────────────────
void loop() {
    maintainBT();
    parseBTStream();
    handleTouch();

    static uint32_t last_draw = 0;
    if (millis() - last_draw >= 100) {  // 10 FPS
        last_draw = millis();
        switch (g_mode) {
        case MODE_COMPASS:  drawCompassMode(); break;
        case MODE_BANDS:    drawBandsMode();   break;
        default:            drawCompassMode(); break;
        }
    }
}
