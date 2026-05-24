// cyd_rfi_display.ino — TM-RFI-FW-002 Rev A
// CYD (ESP32-2432S028R) RFI Noise Floor Display
// ILI9341 320×240, XPT2046 touch, TFT_eSPI
// BT master: connects to "RF-NoiseMonitor"
// Shows: per-band noise floor bars, alert indicators, history trend

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <BluetoothSerial.h>

// ── CYD hardware ──────────────────────────────────────────────────────────
#define CYD_LED_R  4
#define CYD_LED_G  16
#define CYD_LED_B  17

TFT_eSPI    tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);
BluetoothSerial BT;

// ── Colors ────────────────────────────────────────────────────────────────
#define C_BG      0x0841   // very dark blue-gray
#define C_PANEL   0x0C62
#define C_CYAN    0x07FF
#define C_GREEN   0x07E0
#define C_YELLOW  0xFFE0
#define C_RED     0xF800
#define C_ORANGE  0xFD20
#define C_GRAY    0x7BEF
#define C_WHITE   0xFFFF

// ── Band data ─────────────────────────────────────────────────────────────
#define NUM_BANDS  11
static const char *BAND_NAMES[NUM_BANDS] = {
    "160M","80M","40M","30M","20M","17M","15M","12M","10M","6M","2M"
};

// ── State ─────────────────────────────────────────────────────────────────
static float  g_dBm[NUM_BANDS];
static bool   g_alert[NUM_BANDS];
static bool   g_connected = false;
static float  g_thresh = -90.0f;
static uint32_t g_scan_count = 0;
static unsigned long g_last_rx_ms = 0;
static unsigned long g_connect_ms = 0;

// ── History for mini sparklines ───────────────────────────────────────────
#define HIST_LEN 40
static float g_hist[NUM_BANDS][HIST_LEN];
static uint8_t g_hist_idx = 0;

// ── Parse BT packet from monitor ─────────────────────────────────────────
// Format: "SCAN:123,160M:-95.3,80M:-88.1,...\n"
void parseBTPacket(const String &s) {
    auto getVal = [&](const char *key) -> float {
        int ki = s.indexOf(key);
        if (ki < 0) return -120.0f;
        int vs = ki + strlen(key) + 1;
        int ve = s.indexOf(',', vs);
        if (ve < 0) ve = s.length();
        return s.substring(vs, ve).toFloat();
    };

    int scan_ki = s.indexOf("SCAN:");
    if (scan_ki >= 0) {
        g_scan_count = s.substring(scan_ki + 5, s.indexOf(',', scan_ki)).toInt();
    }

    for (int i = 0; i < NUM_BANDS; i++) {
        g_dBm[i] = getVal(BAND_NAMES[i]);
        g_alert[i] = (g_dBm[i] > g_thresh);
        g_hist[i][g_hist_idx] = g_dBm[i];
    }
    g_hist_idx = (g_hist_idx + 1) % HIST_LEN;
    g_connected = true;
    g_last_rx_ms = millis();
}

// ── Color for noise level ─────────────────────────────────────────────────
uint16_t noiseColor(float dBm) {
    if (dBm > -60) return C_RED;
    if (dBm > -80) return C_ORANGE;
    if (dBm > -100) return C_YELLOW;
    return C_GREEN;
}

// ── Draw main noise floor display ─────────────────────────────────────────
void drawNoiseBars() {
    spr.fillSprite(C_BG);

    // Title bar
    spr.fillRect(0, 0, 320, 20, C_PANEL);
    spr.setTextColor(C_CYAN, C_PANEL);
    spr.setTextSize(2);
    spr.setCursor(4, 2);
    spr.print("RF Noise Monitor");
    uint16_t dot_col = g_connected ? C_GREEN : C_RED;
    spr.fillCircle(308, 10, 5, dot_col);

    // Scan counter
    spr.setTextColor(C_GRAY, C_PANEL);
    spr.setTextSize(1);
    spr.setCursor(200, 5);
    spr.printf("Scan:%lu", (unsigned long)g_scan_count);

    // Band bars — 2 columns × 6 rows + 1 remaining
    const int BAR_W = 140;
    const int BAR_H = 18;
    const int COLS  = 2;
    const int ROW_H = 22;
    const int START_Y = 24;

    for (int i = 0; i < NUM_BANDS; i++) {
        int col = i % COLS;
        int row = i / COLS;
        int x = col * 160 + 4;
        int y = START_Y + row * ROW_H;

        // Background
        uint16_t bg = g_alert[i] ? 0x3000 : C_PANEL;
        spr.fillRect(x, y, 154, BAR_H, bg);
        spr.drawRect(x, y, 154, BAR_H, g_alert[i] ? C_RED : 0x1040);

        // Band name
        spr.setTextColor(g_alert[i] ? C_RED : C_CYAN, bg);
        spr.setTextSize(1);
        spr.setCursor(x + 2, y + 5);
        spr.print(BAND_NAMES[i]);

        // Value
        spr.setTextColor(noiseColor(g_dBm[i]), bg);
        spr.setCursor(x + 35, y + 5);
        spr.printf("%5.1f", g_dBm[i]);
        spr.setTextColor(C_GRAY, bg);
        spr.print("dBm");

        // Bar: map −120→−40 dBm to 0→90px
        float norm = (g_dBm[i] + 120.0f) / 80.0f;
        int bw = (int)(norm * 60.0f);
        bw = constrain(bw, 0, 60);
        spr.fillRect(x + 90, y + 4, bw, BAR_H - 8, noiseColor(g_dBm[i]));

        // Mini sparkline (40 points → 40px wide, 8px tall)
        int sx = x + 110;
        int sy = y + 2;
        for (int j = 0; j < HIST_LEN - 1; j++) {
            int idx0 = (g_hist_idx + j) % HIST_LEN;
            int idx1 = (g_hist_idx + j + 1) % HIST_LEN;
            float v0 = constrain((g_hist[i][idx0] + 120.0f) / 80.0f, 0.0f, 1.0f);
            float v1 = constrain((g_hist[i][idx1] + 120.0f) / 80.0f, 0.0f, 1.0f);
            int y0 = sy + 8 - (int)(v0 * 8);
            int y1 = sy + 8 - (int)(v1 * 8);
            int x0 = sx + j;
            int x1 = sx + j + 1;
            spr.drawLine(x0, y0, x1, y1, noiseColor(g_hist[i][idx0]));
        }
    }

    // Status bar
    spr.fillRect(0, 218, 320, 22, C_PANEL);
    spr.setTextColor(g_connected ? C_GREEN : C_RED, C_PANEL);
    spr.setTextSize(1);
    spr.setCursor(4, 224);
    if (g_connected) {
        spr.printf("BT:OK  Threshold:%.0fdBm  IP:192.168.4.1", g_thresh);
    } else {
        spr.print("BT: Searching for RF-NoiseMonitor...");
    }

    // Alert row (if any alerts)
    bool any_alert = false;
    for (int i = 0; i < NUM_BANDS; i++) if (g_alert[i]) { any_alert = true; break; }
    if (any_alert) {
        spr.fillRect(0, 200, 320, 18, 0x3000);
        spr.setTextColor(C_RED, 0x3000);
        spr.setTextSize(1);
        spr.setCursor(4, 205);
        spr.print("ALERT: ");
        for (int i = 0; i < NUM_BANDS; i++) {
            if (g_alert[i]) { spr.print(BAND_NAMES[i]); spr.print(" "); }
        }
    }

    spr.pushSprite(0, 0);
}

// ── Touch: threshold up/down ──────────────────────────────────────────────
void handleTouch(int tx, int ty) {
    // Bottom status bar: tap left half = decrease threshold, right half = increase
    if (ty >= 218) {
        if (tx < 160) {
            g_thresh -= 5.0f;
            if (g_thresh < -130.0f) g_thresh = -130.0f;
        } else {
            g_thresh += 5.0f;
            if (g_thresh > -20.0f) g_thresh = -20.0f;
        }
        // Recompute alerts
        for (int i = 0; i < NUM_BANDS; i++)
            g_alert[i] = (g_dBm[i] > g_thresh);
    }
}

// ── setup() ───────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);

    pinMode(CYD_LED_R, OUTPUT); pinMode(CYD_LED_G, OUTPUT); pinMode(CYD_LED_B, OUTPUT);
    digitalWrite(CYD_LED_R, HIGH); digitalWrite(CYD_LED_G, HIGH); digitalWrite(CYD_LED_B, LOW);

    // Init history
    for (int i = 0; i < NUM_BANDS; i++)
        for (int j = 0; j < HIST_LEN; j++)
            g_hist[i][j] = -120.0f;
    for (int i = 0; i < NUM_BANDS; i++) {
        g_dBm[i] = -120.0f;
        g_alert[i] = false;
    }

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(C_BG);
    spr.createSprite(320, 240);
    spr.setTextDatum(TL_DATUM);

    tft.setTextColor(C_CYAN, C_BG);
    tft.setTextSize(2);
    tft.setCursor(30, 100);
    tft.print("RFI Noise Monitor");
    tft.setTextSize(1);
    tft.setCursor(70, 130);
    tft.print("TM-RFI-FW-002 Rev A");
    tft.setCursor(50, 148);
    tft.print("Connecting to monitor...");

    BT.begin("CYD-RFIDisplay", true);
    BT.connect("RF-NoiseMonitor");
    g_connect_ms = millis();

    delay(1500);
    digitalWrite(CYD_LED_B, HIGH);
}

// ── loop() ────────────────────────────────────────────────────────────────
void loop() {
    // BT reconnect
    if (!BT.connected() && millis() - g_connect_ms > 5000) {
        g_connect_ms = millis();
        BT.connect("RF-NoiseMonitor");
        g_connected = false;
        static bool blink_state = false;
        blink_state = !blink_state;
        digitalWrite(CYD_LED_B, blink_state ? LOW : HIGH);
    } else if (BT.connected()) {
        g_connected = true;
        digitalWrite(CYD_LED_B, HIGH);
        digitalWrite(CYD_LED_G, LOW);
    }

    // BT RX
    static String bt_buf;
    while (BT.available()) {
        char c = BT.read();
        if (c == '\n' || c == '\r') {
            if (bt_buf.length() > 0) { parseBTPacket(bt_buf); bt_buf = ""; }
        } else bt_buf += c;
    }

    // Stale check
    if (millis() - g_last_rx_ms > 8000 && g_connected) {
        g_connected = false;
        digitalWrite(CYD_LED_G, HIGH);
    }

    // Touch
    uint16_t tx, ty;
    static unsigned long touch_ms = 0;
    if (tft.getTouch(&tx, &ty, 300) && millis() - touch_ms > 200) {
        touch_ms = millis();
        handleTouch(tx, ty);
    }

    // Draw at 10 FPS
    static unsigned long last_draw = 0;
    if (millis() - last_draw >= 100) {
        last_draw = millis();
        drawNoiseBars();
    }
}
