// cyd_sdr_display.ino — TM-SDR-FW-002 Rev A
// CYD (ESP32-2432S028R) SDR Controller Display
// ILI9341 320×240, XPT2046 touch, TFT_eSPI library
// Bluetooth master: connects to "RF-SDRCtrl"
// Shows: band grid, attenuator, LNA/bypass/bias-T controls, RSSI meter

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <BluetoothSerial.h>

// ── CYD hardware ──────────────────────────────────────────────────────────
#define CYD_LED_R  4
#define CYD_LED_G  16
#define CYD_LED_B  17
#define CYD_TOUCH_CS  33
#define CYD_TOUCH_IRQ 36

TFT_eSPI    tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);
BluetoothSerial BT;

// ── Colors ────────────────────────────────────────────────────────────────
#define C_BG        0x1820   // dark navy
#define C_PANEL     0x0C20   // darker panel
#define C_ACCENT    0x07FF   // cyan
#define C_GREEN     0x07E0
#define C_RED       0xF800
#define C_YELLOW    0xFFE0
#define C_GRAY      0x7BEF
#define C_WHITE     0xFFFF
#define C_ORANGE    0xFD20
#define C_ACTIVE    0x2A3F   // active band highlight

// ── Band data ─────────────────────────────────────────────────────────────
struct BandEntry { const char *name; uint32_t center; };
static const BandEntry BANDS[] = {
    {"160M", 1900000}, {"80M",  3750000}, {"40M",  7150000}, {"30M", 10125000},
    {"20M", 14175000}, {"17M", 18118000}, {"15M", 21225000}, {"12M", 24940000},
    {"10M", 28850000}, {"6M",  52000000}, {"2M",  146000000},{"1.25",223500000},
    {"70cm",435000000}, {"33cm",915000000},{"23cm",1270000000},{"WIDE",0}
};
#define NUM_BANDS 16

// ── State from controller ─────────────────────────────────────────────────
struct SDRState {
    char    band[8]  = "40M";
    int8_t  att      = 0;
    bool    lna      = false;
    bool    bypass   = false;
    bool    biastee  = false;
    bool    alc      = false;
    float   rssi     = -99.0f;
    bool    connected = false;
};
SDRState g_state;

// ── Display modes ─────────────────────────────────────────────────────────
enum DisplayMode { MODE_MAIN, MODE_BANDS };
DisplayMode g_mode = MODE_MAIN;

// ── BT connection ─────────────────────────────────────────────────────────
static String  g_bt_buf;
static unsigned long g_last_rx_ms = 0;
static unsigned long g_connect_ms = 0;
static bool    g_bt_connecting   = false;

// ── RSSI history for bar chart ────────────────────────────────────────────
#define RSSI_HIST 80
static float g_rssi_hist[RSSI_HIST] = {};
static uint8_t g_rssi_idx = 0;

// ─── Parse BT status packet ───────────────────────────────────────────────
// Format: BAND:40M,FREQ:7150000,ATT:0,LNA:1,BYP:0,BT:0,RSSI:-45.2
void parseBTPacket(const String &s) {
    auto getField = [&](const char *key) -> String {
        int ki = s.indexOf(key);
        if (ki < 0) return "";
        int vs = ki + strlen(key) + 1;  // skip ':'
        int ve = s.indexOf(',', vs);
        if (ve < 0) ve = s.length();
        return s.substring(vs, ve);
    };
    String b = getField("BAND");
    if (b.length() > 0) b.toCharArray(g_state.band, sizeof(g_state.band));
    String att = getField("ATT");
    if (att.length() > 0) g_state.att = att.toInt();
    String lna = getField("LNA");
    if (lna.length() > 0) g_state.lna = (lna.toInt() == 1);
    String byp = getField("BYP");
    if (byp.length() > 0) g_state.bypass = (byp.toInt() == 1);
    String bt = getField("BT");
    if (bt.length() > 0) g_state.biastee = (bt.toInt() == 1);
    String rssi = getField("RSSI");
    if (rssi.length() > 0) g_state.rssi = rssi.toFloat();
    g_state.connected = true;
    g_last_rx_ms = millis();
    // Update history
    g_rssi_hist[g_rssi_idx] = g_state.rssi;
    g_rssi_idx = (g_rssi_idx + 1) % RSSI_HIST;
}

// ─── Send command to controller ───────────────────────────────────────────
void sendCmd(const char *cmd) {
    if (BT.connected()) {
        BT.println(cmd);
    }
}

// ─── Draw main view ───────────────────────────────────────────────────────
void drawMain() {
    spr.fillSprite(C_BG);

    // Title bar
    spr.fillRect(0, 0, 320, 22, C_PANEL);
    spr.setTextColor(C_ACCENT, C_PANEL);
    spr.setTextSize(2);
    spr.setCursor(4, 3);
    spr.print("SDR Controller");
    // Connection dot
    uint16_t dot_col = g_state.connected ? C_GREEN : C_RED;
    spr.fillCircle(308, 11, 6, dot_col);

    // Band display large
    spr.setTextColor(C_WHITE, C_BG);
    spr.setTextSize(3);
    spr.setCursor(8, 30);
    spr.print(g_state.band);

    // Frequency
    spr.setTextColor(C_GRAY, C_BG);
    spr.setTextSize(1);
    spr.setCursor(100, 34);
    for (int i = 0; i < NUM_BANDS; i++) {
        if (strcmp(BANDS[i].name, g_state.band) == 0 && BANDS[i].center > 0) {
            uint32_t f = BANDS[i].center;
            if (f < 30000000UL)
                spr.printf("%lu.%03lu MHz", f / 1000000UL, (f % 1000000UL) / 1000UL);
            else if (f < 1000000000UL)
                spr.printf("%lu.%03lu MHz", f / 1000000UL, (f % 1000000UL) / 1000UL);
            else
                spr.printf("%.3f GHz", f / 1e9f);
            break;
        }
    }

    // RSSI meter
    spr.setTextColor(C_GRAY, C_BG);
    spr.setTextSize(1);
    spr.setCursor(8, 60);
    spr.print("RSSI");
    // Bar: map −100→0 dBm to 0→280 px
    int rssi_px = (int)((g_state.rssi + 100.0f) * 280.0f / 100.0f);
    rssi_px = constrain(rssi_px, 0, 280);
    uint16_t rssi_col = (g_state.rssi > -20) ? C_RED :
                        (g_state.rssi > -40) ? C_YELLOW : C_GREEN;
    spr.drawRect(36, 58, 282, 14, C_GRAY);
    spr.fillRect(37, 59, rssi_px, 12, rssi_col);
    spr.setTextColor(C_WHITE, C_BG);
    spr.setCursor(260, 60);
    spr.printf("%.0fdBm", g_state.rssi);

    // RSSI history sparkline
    spr.drawRect(8, 76, 304, 30, C_PANEL);
    for (int i = 0; i < RSSI_HIST - 1; i++) {
        int idx0 = (g_rssi_idx + i) % RSSI_HIST;
        int idx1 = (g_rssi_idx + i + 1) % RSSI_HIST;
        int y0 = 105 - (int)((g_rssi_hist[idx0] + 100.0f) * 28.0f / 80.0f);
        int y1 = 105 - (int)((g_rssi_hist[idx1] + 100.0f) * 28.0f / 80.0f);
        y0 = constrain(y0, 77, 105);
        y1 = constrain(y1, 77, 105);
        int x0 = 8 + i * 304 / RSSI_HIST;
        int x1 = 8 + (i+1) * 304 / RSSI_HIST;
        spr.drawLine(x0, y0, x1, y1, C_ACCENT);
    }

    // Controls row
    int y = 112;
    spr.fillRect(0, y, 320, 22, C_PANEL);
    // ATT
    spr.setTextColor(C_GRAY, C_PANEL);
    spr.setCursor(4, y + 4);
    spr.setTextSize(1);
    spr.print("ATT:");
    spr.setTextColor(C_WHITE, C_PANEL);
    spr.setTextSize(2);
    spr.setCursor(36, y + 2);
    spr.printf("%2ddB", g_state.att);
    // Indicators
    auto drawIndicator = [&](int x, int yy, const char *lbl, bool on) {
        spr.fillRoundRect(x, yy, 48, 16, 3, on ? C_GREEN : C_PANEL);
        spr.setTextColor(on ? C_BG : C_GRAY, on ? C_GREEN : C_PANEL);
        spr.setTextSize(1);
        spr.setCursor(x + 4, yy + 4);
        spr.print(lbl);
    };
    drawIndicator(100, y + 3, "LNA", g_state.lna);
    drawIndicator(152, y + 3, "BYP", g_state.bypass);
    drawIndicator(204, y + 3, "BT", g_state.biastee);
    drawIndicator(256, y + 3, "ALC", g_state.alc);

    // Control buttons
    y = 138;
    // ATT − and + buttons
    spr.fillRoundRect(4,  y, 48, 32, 4, C_PANEL);
    spr.fillRoundRect(56, y, 48, 32, 4, C_PANEL);
    spr.setTextColor(C_ACCENT, C_PANEL);
    spr.setTextSize(3);
    spr.setCursor(14, y + 4); spr.print("-");
    spr.setCursor(63, y + 4); spr.print("+");
    spr.setTextColor(C_GRAY, C_BG);
    spr.setTextSize(1);
    spr.setCursor(12, y + 24); spr.print("ATT-");
    spr.setCursor(62, y + 24); spr.print("ATT+");

    // LNA toggle
    spr.fillRoundRect(112, y, 48, 32, 4, g_state.lna ? C_GREEN : C_PANEL);
    spr.setTextColor(g_state.lna ? C_BG : C_GRAY, g_state.lna ? C_GREEN : C_PANEL);
    spr.setTextSize(1);
    spr.setCursor(120, y + 10); spr.print("LNA");

    // Bypass toggle
    spr.fillRoundRect(164, y, 48, 32, 4, g_state.bypass ? C_YELLOW : C_PANEL);
    spr.setTextColor(g_state.bypass ? C_BG : C_GRAY, g_state.bypass ? C_YELLOW : C_PANEL);
    spr.setCursor(170, y + 10); spr.print("BYPASS");

    // BiasT toggle
    spr.fillRoundRect(216, y, 48, 32, 4, g_state.biastee ? C_ORANGE : C_PANEL);
    spr.setTextColor(g_state.biastee ? C_BG : C_GRAY, g_state.biastee ? C_ORANGE : C_PANEL);
    spr.setCursor(222, y + 10); spr.print("BIAS-T");

    // Bands button
    spr.fillRoundRect(268, y, 48, 32, 4, C_ACCENT);
    spr.setTextColor(C_BG, C_ACCENT);
    spr.setCursor(274, y + 10); spr.print("BANDS");

    // ALC toggle
    y = 174;
    spr.fillRoundRect(4, y, 60, 20, 3, g_state.alc ? C_ACCENT : C_PANEL);
    spr.setTextColor(g_state.alc ? C_BG : C_GRAY, g_state.alc ? C_ACCENT : C_PANEL);
    spr.setTextSize(1);
    spr.setCursor(10, y + 6); spr.print("ALC AUTO");

    // Status bar
    spr.fillRect(0, 218, 320, 22, C_PANEL);
    spr.setTextColor(C_GRAY, C_PANEL);
    spr.setTextSize(1);
    spr.setCursor(4, 224);
    if (g_state.connected) {
        spr.printf("BT:OK  IP:192.168.4.1  %s", FW_VERSION_SHORT);
    } else {
        spr.print("BT: Connecting to RF-SDRCtrl...");
    }

    spr.pushSprite(0, 0);
}

#define FW_VERSION_SHORT "TM-SDR-FW-002"

// ─── Draw band grid ───────────────────────────────────────────────────────
void drawBandGrid() {
    spr.fillSprite(C_BG);
    spr.fillRect(0, 0, 320, 20, C_PANEL);
    spr.setTextColor(C_ACCENT, C_PANEL);
    spr.setTextSize(2);
    spr.setCursor(4, 2);
    spr.print("Select Band");

    for (int i = 0; i < NUM_BANDS; i++) {
        int col = i % 4;
        int row = i / 4;
        int x = col * 80;
        int y = 22 + row * 50;
        bool active = (strcmp(BANDS[i].name, g_state.band) == 0);
        spr.fillRoundRect(x+2, y+2, 76, 44, 4, active ? C_ACTIVE : C_PANEL);
        spr.drawRoundRect(x+2, y+2, 76, 44, 4, active ? C_ACCENT : C_GRAY);
        spr.setTextColor(active ? C_ACCENT : C_WHITE, active ? C_ACTIVE : C_PANEL);
        spr.setTextSize(2);
        spr.setCursor(x + 8, y + 8);
        spr.print(BANDS[i].name);
        if (BANDS[i].center > 0) {
            spr.setTextColor(C_GRAY, active ? C_ACTIVE : C_PANEL);
            spr.setTextSize(1);
            spr.setCursor(x + 6, y + 28);
            uint32_t f = BANDS[i].center;
            if (f < 1000000000UL)
                spr.printf("%.3fMHz", f / 1e6f);
            else
                spr.printf("%.3fGHz", f / 1e9f);
        }
    }
    spr.pushSprite(0, 0);
}

// ─── Touch handling ───────────────────────────────────────────────────────
void handleTouch(int tx, int ty) {
    if (g_mode == MODE_MAIN) {
        int y_btn = 138;
        // ATT−
        if (tx >= 4 && tx <= 52 && ty >= y_btn && ty <= y_btn+32) {
            int8_t na = constrain(g_state.att - 3, 0, 31);
            char cmd[20]; snprintf(cmd, sizeof(cmd), "ATT %d", (int)na);
            sendCmd(cmd);
        }
        // ATT+
        else if (tx >= 56 && tx <= 104 && ty >= y_btn && ty <= y_btn+32) {
            int8_t na = constrain(g_state.att + 3, 0, 31);
            char cmd[20]; snprintf(cmd, sizeof(cmd), "ATT %d", (int)na);
            sendCmd(cmd);
        }
        // LNA
        else if (tx >= 112 && tx <= 160 && ty >= y_btn && ty <= y_btn+32) {
            sendCmd(g_state.lna ? "LNA OFF" : "LNA ON");
        }
        // Bypass
        else if (tx >= 164 && tx <= 212 && ty >= y_btn && ty <= y_btn+32) {
            sendCmd(g_state.bypass ? "BYP OFF" : "BYP ON");
        }
        // Bias-T
        else if (tx >= 216 && tx <= 264 && ty >= y_btn && ty <= y_btn+32) {
            sendCmd(g_state.biastee ? "BT OFF" : "BT ON");
        }
        // Bands
        else if (tx >= 268 && tx <= 316 && ty >= y_btn && ty <= y_btn+32) {
            g_mode = MODE_BANDS;
        }
        // ALC
        else if (tx >= 4 && tx <= 64 && ty >= 174 && ty <= 194) {
            sendCmd(g_state.alc ? "ALC OFF" : "ALC ON");
        }
    }
    else if (g_mode == MODE_BANDS) {
        // Back via header area
        if (ty < 22) { g_mode = MODE_MAIN; return; }
        int col = tx / 80;
        int row = (ty - 22) / 50;
        int idx = row * 4 + col;
        if (idx >= 0 && idx < NUM_BANDS) {
            char cmd[20]; snprintf(cmd, sizeof(cmd), "BAND %s", BANDS[idx].name);
            sendCmd(cmd);
            g_mode = MODE_MAIN;
        }
    }
}

// ─── Touch read ───────────────────────────────────────────────────────────
static uint16_t g_touch_x = 0, g_touch_y = 0;
static bool g_touched = false;
static unsigned long g_touch_ms = 0;

void pollTouch() {
    // XPT2046 SPI touch read (simplified, no interrupt)
    // Uses TFT_eSPI built-in touch support
    if (tft.getTouch(&g_touch_x, &g_touch_y, 300)) {
        if (millis() - g_touch_ms > 200) {  // 200 ms debounce
            g_touch_ms = millis();
            handleTouch(g_touch_x, g_touch_y);
        }
    }
}

// ─── setup() ──────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);

    // CYD LEDs
    pinMode(CYD_LED_R, OUTPUT); pinMode(CYD_LED_G, OUTPUT); pinMode(CYD_LED_B, OUTPUT);
    digitalWrite(CYD_LED_R, HIGH); digitalWrite(CYD_LED_G, HIGH); digitalWrite(CYD_LED_B, LOW);

    // TFT
    tft.init();
    tft.setRotation(1);   // landscape
    tft.fillScreen(C_BG);

    // Sprite
    spr.createSprite(320, 240);
    spr.setTextDatum(TL_DATUM);

    // Startup screen
    tft.setTextColor(C_ACCENT, C_BG);
    tft.setTextSize(2);
    tft.setCursor(40, 100);
    tft.print("SDR Controller");
    tft.setTextSize(1);
    tft.setCursor(80, 125);
    tft.print("TM-SDR-FW-002 Rev A");

    // Bluetooth master
    BT.begin("CYD-SDRDisplay", true);  // true = master mode
    delay(500);

    BT.connect("RF-SDRCtrl");
    g_bt_connecting = true;
    g_connect_ms = millis();

    tft.setCursor(80, 145);
    tft.print("Connecting BT...");

    delay(2000);
    digitalWrite(CYD_LED_B, HIGH);
}

// ─── loop() ───────────────────────────────────────────────────────────────
void loop() {
    // BT reconnect if needed
    if (!BT.connected() && millis() - g_connect_ms > 5000) {
        g_connect_ms = millis();
        BT.connect("RF-SDRCtrl");
        g_state.connected = false;
        // LED: blue blink = trying to connect
        static bool blink = false;
        blink = !blink;
        digitalWrite(CYD_LED_B, blink ? LOW : HIGH);
    } else if (BT.connected()) {
        g_state.connected = true;
        digitalWrite(CYD_LED_B, HIGH);  // BT LED off when connected
        digitalWrite(CYD_LED_G, LOW);   // green = connected
    }

    // BT RX
    while (BT.available()) {
        char c = BT.read();
        if (c == '\n' || c == '\r') {
            if (g_bt_buf.length() > 0) {
                parseBTPacket(g_bt_buf);
                g_bt_buf = "";
            }
        } else {
            g_bt_buf += c;
        }
    }

    // Check stale connection
    if (millis() - g_last_rx_ms > 5000 && g_state.connected) {
        g_state.connected = false;
        digitalWrite(CYD_LED_G, HIGH);
    }

    // Touch
    pollTouch();

    // Draw
    static unsigned long last_draw = 0;
    if (millis() - last_draw >= 100) {  // 10 FPS
        last_draw = millis();
        if (g_mode == MODE_MAIN)      drawMain();
        else if (g_mode == MODE_BANDS) drawBandGrid();
    }
}
