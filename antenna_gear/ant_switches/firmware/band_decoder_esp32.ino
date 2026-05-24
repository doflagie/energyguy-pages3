// ============================================================================
// TM-ANT-SW-001  Rev A — Automatic Band-Data Antenna Switch
// ESP32 / CYD Firmware — Band decoder + automatic antenna selection
// Inputs: Yaesu BCD | Icom CI-V | CAT serial | Band voltage ADC
// Output: 4-port relay switch control (integrates with relay_switch_esp32)
// ============================================================================

#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <TFT_eSPI.h>

// ── Band data input pins ───────────────────────────────────────────────────
// Yaesu BCD (logic inputs after 3.3V level shift from 5V radio outputs)
#define BCD_B1       4    // Band data bit 1 (LSB)
#define BCD_B2       5    // Band data bit 2
#define BCD_B4      18    // Band data bit 4
#define BCD_B8      19    // Band data bit 8 (MSB)
#define PIN_SEND    34    // TX inhibit (SEND line) — input only, no pull-up

// Band voltage input (ADC — for radios using DC voltage band coding)
#define BAND_VOLT_ADC  35   // 0–3.3V input (after divider from radio output)

// CI-V serial (Icom)
#define CIV_RX      16    // UART2 RX — CI-V data from radio
#define CIV_TX      17    // UART2 TX — not needed if listen-only

// ── Relay outputs (to relay driver — same as relay_switch_esp32) ───────────
#define PIN_RELAY_1     26
#define PIN_RELAY_2     27
#define PIN_RELAY_3     14
#define PIN_RELAY_4     12

// ── Status LEDs ────────────────────────────────────────────────────────────
#define LED_1       25
#define LED_2       33
#define LED_3       32
#define LED_4       13

// ── CYD backlight ──────────────────────────────────────────────────────────
#define PIN_BL      21

// ── Band definitions ───────────────────────────────────────────────────────
#define NUM_BANDS    12
#define NUM_PORTS     4

typedef struct {
    const char* name;
    uint32_t    freq_low_khz;
    uint32_t    freq_high_khz;
    uint8_t     yaesu_bcd;       // BCD code (B8×8 + B4×4 + B2×2 + B1×1)
    uint8_t     default_port;    // antenna port (1-based)
} BandEntry;

static const BandEntry BANDS[NUM_BANDS] = {
    // name     low_kHz  high_kHz  BCD  default_port
    { "160M",    1800,    2000,     1,   1 },
    { "80M",     3500,    4000,     2,   1 },
    { "40M",     7000,    7300,     4,   2 },
    { "30M",    10100,   10150,     5,   2 },
    { "20M",    14000,   14350,     6,   3 },
    { "17M",    18068,   18168,     7,   3 },
    { "15M",    21000,   21450,     8,   3 },
    { "12M",    24890,   24990,     9,   4 },
    { "10M",    28000,   29700,    10,   4 },
    { "6M",     50000,   54000,    11,   4 },
    { "2M",    144000,  148000,    12,   1 },   // remap in user config
    { "70cm",  420000,  450000,     0,   1 },   // not in Yaesu BCD
};

// Band-to-antenna mapping (user configurable; stored in NVS)
static uint8_t g_band_map[NUM_BANDS];   // g_band_map[band_idx] = port (1-4)

// ── Input source selection ─────────────────────────────────────────────────
enum InputSource { SRC_BCD, SRC_CIV, SRC_VOLTAGE, SRC_MANUAL };
static InputSource g_input_source = SRC_BCD;

// ── State ──────────────────────────────────────────────────────────────────
static int      g_current_band  = -1;   // index into BANDS[], -1 = unknown
static uint8_t  g_selected_port =  1;
static bool     g_auto_mode     = true; // false = manual override active
static bool     g_tx_active     = false;
static uint8_t  g_pending_port  = 0;
static bool     g_wifi_ok       = false;
static char     g_ant_names[NUM_PORTS][16];

const char ANT_NAME_DEFAULT[NUM_PORTS][16] = {
    "ANT 1", "ANT 2", "ANT 3", "ANT 4"
};

static const int RELAY_PINS[NUM_PORTS] = {PIN_RELAY_1, PIN_RELAY_2, PIN_RELAY_3, PIN_RELAY_4};
static const int LED_PINS_A[NUM_PORTS] = {LED_1, LED_2, LED_3, LED_4};

Preferences prefs;
WebServer   server(80);
TFT_eSPI    tft = TFT_eSPI();

// ── Relay control ──────────────────────────────────────────────────────────
void apply_port(uint8_t port) {
    if (port < 1 || port > NUM_PORTS) port = 1;
    for (int i = 0; i < NUM_PORTS; i++) {
        bool on = (i == port - 1);
        digitalWrite(RELAY_PINS[i], on ? HIGH : LOW);
        digitalWrite(LED_PINS_A[i], on ? HIGH : LOW);
    }
    g_selected_port = port;
    prefs.putUChar("port", port);
}

bool is_tx_active() {
    return digitalRead(PIN_SEND) == HIGH;   // HIGH = transmitting (most Yaesu)
}

void switch_to_port(uint8_t port) {
    if (is_tx_active()) {
        if (port != g_selected_port) {
            g_pending_port = port;
            Serial.printf("[SW] TX active — port %d queued\n", port);
        }
        return;
    }
    if (port != g_selected_port) {
        Serial.printf("[SW] Switching to port %d\n", port);
        apply_port(port);
        g_pending_port = 0;
        draw_main_screen();
    }
}

// ── BCD band decode ────────────────────────────────────────────────────────
int decode_bcd() {
    uint8_t b1 = digitalRead(BCD_B1) ? 1 : 0;
    uint8_t b2 = digitalRead(BCD_B2) ? 2 : 0;
    uint8_t b4 = digitalRead(BCD_B4) ? 4 : 0;
    uint8_t b8 = digitalRead(BCD_B8) ? 8 : 0;
    uint8_t bcd = b8 | b4 | b2 | b1;
    for (int i = 0; i < NUM_BANDS; i++)
        if (BANDS[i].yaesu_bcd == bcd) return i;
    return -1;  // unknown BCD code
}

// ── CI-V band decode ───────────────────────────────────────────────────────
// Minimal CI-V listener: reads frequency packets, maps to band index.
// Format: FE FE [addr] [ctrl] 03 [5-byte BCD freq] FD
static uint8_t  civ_buf[32];
static uint8_t  civ_len = 0;
static uint32_t civ_last_khz = 0;

int freq_khz_to_band(uint32_t freq_khz) {
    for (int i = 0; i < NUM_BANDS; i++)
        if (freq_khz >= BANDS[i].freq_low_khz && freq_khz <= BANDS[i].freq_high_khz)
            return i;
    return -1;
}

uint32_t parse_civ_freq(uint8_t *buf, int len) {
    // Icom CI-V frequency bytes: 5 bytes BCD, Hz per digit
    // byte 0 = Hz × 1, byte 1 = Hz × 100, etc. (little-endian BCD)
    if (len < 5) return 0;
    uint32_t hz = 0;
    uint32_t mult = 1;
    for (int i = 0; i < 5; i++) {
        hz += (buf[i] & 0x0F) * mult;          mult *= 10;
        hz += ((buf[i] >> 4) & 0x0F) * mult;   mult *= 10;
    }
    return hz / 1000;  // return kHz
}

void civ_process_byte(uint8_t b) {
    static bool in_packet = false;
    if (b == 0xFE) {
        civ_len = 0;
        in_packet = true;
    }
    if (!in_packet) return;
    if (civ_len < 31) civ_buf[civ_len++] = b;
    if (b == 0xFD && civ_len > 8) {
        // Look for freq read response: FE FE [unk] [us] 03 [5 freq bytes] FD
        for (int i = 0; i < civ_len - 7; i++) {
            if (civ_buf[i] == 0xFE && civ_buf[i+1] == 0xFE &&
                civ_buf[i+4] == 0x03) {
                civ_last_khz = parse_civ_freq(&civ_buf[i+5], 5);
                break;
            }
        }
        in_packet = false;
        civ_len = 0;
    }
}

int decode_civ() {
    while (Serial2.available()) {
        civ_process_byte(Serial2.read());
    }
    if (civ_last_khz > 0) return freq_khz_to_band(civ_last_khz);
    return -1;
}

// ── Band voltage decode ────────────────────────────────────────────────────
// Different radios use different voltage-per-band tables.
// Example: Kenwood TS-50, TS-570 band voltage (approx):
static const float BAND_VOLTAGES[NUM_BANDS] = {
    0.0f,   // 160M
    0.33f,  // 80M
    0.67f,  // 60M  — extend/remove as needed
    1.0f,   // 40M
    1.33f,  // 30M
    1.67f,  // 20M
    2.0f,   // 17M
    2.33f,  // 15M
    2.67f,  // 12M
    3.0f,   // 10M
    3.33f,  // 6M
    3.67f,  // 2M
};

int decode_band_voltage() {
    int raw = analogRead(BAND_VOLT_ADC);
    float v = raw * 3.3f / 4095.0f;
    int best = -1;
    float best_diff = 0.3f;  // ±0.3V tolerance
    for (int i = 0; i < NUM_BANDS; i++) {
        float diff = fabsf(v - BAND_VOLTAGES[i]);
        if (diff < best_diff) { best_diff = diff; best = i; }
    }
    return best;
}

// ── Band detection loop ────────────────────────────────────────────────────
static uint32_t last_band_check = 0;
#define BAND_CHECK_MS  250   // check for band change every 250ms

void update_band() {
    if (millis() - last_band_check < BAND_CHECK_MS) return;
    last_band_check = millis();

    if (!g_auto_mode) return;

    int new_band = -1;
    switch (g_input_source) {
        case SRC_BCD:     new_band = decode_bcd();          break;
        case SRC_CIV:     new_band = decode_civ();          break;
        case SRC_VOLTAGE: new_band = decode_band_voltage(); break;
        default: break;
    }

    if (new_band >= 0 && new_band != g_current_band) {
        g_current_band = new_band;
        uint8_t target_port = g_band_map[new_band];
        Serial.printf("[BAND] %s detected → port %d (%s)\n",
            BANDS[new_band].name, target_port, g_ant_names[target_port - 1]);
        switch_to_port(target_port);
        draw_main_screen();
    }
}

// ── TX inhibit loop ────────────────────────────────────────────────────────
void check_tx() {
    bool tx = is_tx_active();
    if (tx != g_tx_active) {
        g_tx_active = tx;
        if (!tx && g_pending_port > 0) {
            Serial.printf("[SW] TX cleared — switching to port %d\n", g_pending_port);
            apply_port(g_pending_port);
            g_pending_port = 0;
        }
        draw_main_screen();
    }
}

// ── Display ────────────────────────────────────────────────────────────────
void draw_main_screen() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 8);
    tft.print("AUTO ANT SW");

    // Band display
    tft.setTextSize(2);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setCursor(10, 34);
    if (g_current_band >= 0) {
        tft.print(BANDS[g_current_band].name);
        if (civ_last_khz > 0) {
            tft.setTextSize(1);
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
            tft.setCursor(80, 38);
            tft.printf("  %lu kHz", civ_last_khz);
        }
    } else {
        tft.print("---");
    }

    // Mode indicator
    tft.setTextSize(1);
    tft.setTextColor(g_auto_mode ? TFT_GREEN : TFT_ORANGE, TFT_BLACK);
    tft.setCursor(200, 8);
    tft.print(g_auto_mode ? "AUTO" : "MANUAL");
    if (g_tx_active) {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.setCursor(200, 20);
        tft.print("TX");
    }

    // Port buttons
    for (int i = 0; i < NUM_PORTS; i++) {
        int x = 10 + (i % 2) * 155;
        int y = 70 + (i / 2) * 80;
        bool active = (g_selected_port == i + 1);

        // Determine which bands route to this port
        String band_list = "";
        for (int b = 0; b < NUM_BANDS; b++)
            if (g_band_map[b] == i + 1) {
                if (band_list.length()) band_list += " ";
                band_list += BANDS[b].name;
            }

        uint16_t bg = active ? TFT_GREEN : 0x4A69;
        tft.fillRoundRect(x, y, 140, 65, 8, bg);
        tft.drawRoundRect(x, y, 140, 65, 8, TFT_WHITE);
        tft.setTextColor(active ? TFT_BLACK : TFT_WHITE, bg);
        tft.setTextSize(2);
        tft.setCursor(x + 10, y + 8);
        tft.print(g_ant_names[i]);
        tft.setTextSize(1);
        tft.setCursor(x + 6, y + 36);
        // Truncate band list to fit
        if (band_list.length() > 20) band_list = band_list.substring(0, 20) + "..";
        tft.print(band_list);
        if (active) {
            tft.setCursor(x + 6, y + 52);
            tft.print("ACTIVE");
        }
    }

    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(10, 220);
    const char* src_name[] = {"BCD", "CI-V", "Volt", "Manual"};
    tft.printf("Src:%s  IP:%s", src_name[g_input_source],
        g_wifi_ok ? WiFi.localIP().toString().c_str() : "---");
}

// ── Web server ─────────────────────────────────────────────────────────────
void web_root() {
    String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width'>"
        "<style>body{background:#111;color:#eee;font-family:monospace;padding:20px}"
        "h1{color:#0cf}table{border-collapse:collapse;width:100%}"
        "th,td{padding:8px 12px;border:1px solid #444;text-align:left}"
        "th{background:#222}.active-row{background:#040}"
        ".btn{padding:8px 16px;background:#006;color:#eee;border:none;cursor:pointer;margin:4px}"
        "</style></head><body><h1>&#128225; Auto Antenna Switch</h1>";

    html += "<p>Band: <b>" + String(g_current_band >= 0 ? BANDS[g_current_band].name : "---") +
            "</b>  Active port: <b>" + String(g_selected_port) + " - " + g_ant_names[g_selected_port-1] +
            "</b>  Mode: <b>" + (g_auto_mode ? "AUTO" : "MANUAL") + "</b>";
    if (g_tx_active) html += " <span style='color:#f44'>TX ACTIVE</span>";
    html += "</p>";

    // Band map table
    html += "<h3>Band &#8594; Antenna Mapping</h3><table><tr><th>Band</th><th>Antenna</th><th>Select</th></tr>";
    for (int b = 0; b < NUM_BANDS; b++) {
        bool cur = (b == g_current_band);
        html += "<tr" + String(cur ? " class='active-row'" : "") + "><td><b>" +
                BANDS[b].name + "</b></td><td>" + g_ant_names[g_band_map[b]-1] +
                " (port " + String(g_band_map[b]) + ")</td><td>";
        for (int p = 1; p <= NUM_PORTS; p++) {
            html += "<a href='/setmap?band=" + String(b) + "&port=" + String(p) + "'>";
            html += "<button class='btn'" + String(g_band_map[b]==p?" style='background:#060'":"") +
                    ">" + g_ant_names[p-1] + "</button></a>";
        }
        html += "</td></tr>";
    }
    html += "</table>";

    // Manual port select
    html += "<h3>Manual Override</h3>";
    for (int p = 1; p <= NUM_PORTS; p++)
        html += "<a href='/mansel?port=" + String(p) + "'><button class='btn'" +
                String(g_selected_port==p?" style='background:#060'":"") +
                ">" + g_ant_names[p-1] + "</button></a>";
    html += "<a href='/automode'><button class='btn'>Auto Mode</button></a>";
    html += "<p><a href='/api/status' style='color:#0af'>JSON</a></p></body></html>";
    server.send(200, "text/html", html);
}

void web_set_map() {
    int b = server.hasArg("band") ? server.arg("band").toInt() : -1;
    int p = server.hasArg("port") ? server.arg("port").toInt() : -1;
    if (b >= 0 && b < NUM_BANDS && p >= 1 && p <= NUM_PORTS) {
        g_band_map[b] = p;
        prefs.putUChar(("bm" + String(b)).c_str(), p);
        Serial.printf("[MAP] %s → port %d\n", BANDS[b].name, p);
    }
    server.sendHeader("Location", "/");
    server.send(302);
}

void web_manual_select() {
    int p = server.hasArg("port") ? server.arg("port").toInt() : -1;
    if (p >= 1 && p <= NUM_PORTS) {
        g_auto_mode = false;
        switch_to_port(p);
    }
    server.sendHeader("Location", "/");
    server.send(302);
}

void web_auto_mode() {
    g_auto_mode = true;
    server.sendHeader("Location", "/");
    server.send(302);
}

void web_api_status() {
    String j = "{\"band\":\"" + String(g_current_band >= 0 ? BANDS[g_current_band].name : "") + "\","
               "\"port\":" + g_selected_port + ","
               "\"auto\":" + (g_auto_mode ? "true" : "false") + ","
               "\"tx\":" + (g_tx_active ? "true" : "false") + "}";
    server.send(200, "application/json", j);
}

void setup_webserver() {
    server.on("/",           HTTP_GET, web_root);
    server.on("/setmap",     HTTP_GET, web_set_map);
    server.on("/mansel",     HTTP_GET, web_manual_select);
    server.on("/automode",   HTTP_GET, web_auto_mode);
    server.on("/api/status", HTTP_GET, web_api_status);
    server.begin();
}

// ── Serial commands ────────────────────────────────────────────────────────
void handle_serial() {
    if (!Serial.available()) return;
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd.startsWith("ANT ") || cmd.startsWith("PORT ")) {
        int p = cmd.substring(4).toInt();
        if (p >= 1 && p <= NUM_PORTS) {
            g_auto_mode = false;
            switch_to_port(p);
            Serial.printf("OK manual port %d\n", p);
        }
    } else if (cmd == "AUTO") {
        g_auto_mode = true;
        Serial.println("OK auto mode");
    } else if (cmd.startsWith("SRC ")) {
        String src = cmd.substring(4);
        src.toUpperCase();
        if (src == "BCD")  { g_input_source = SRC_BCD;     Serial.println("OK source: BCD"); }
        if (src == "CIV")  { g_input_source = SRC_CIV;     Serial.println("OK source: CI-V"); }
        if (src == "VOLT") { g_input_source = SRC_VOLTAGE;  Serial.println("OK source: voltage"); }
    } else if (cmd.startsWith("MAP ")) {
        // MAP 40M 2  → assign 40M to port 2
        int space = cmd.indexOf(' ', 4);
        if (space > 4) {
            String band_name = cmd.substring(4, space);
            int port = cmd.substring(space+1).toInt();
            band_name.toUpperCase();
            for (int b = 0; b < NUM_BANDS; b++) {
                if (String(BANDS[b].name) == band_name && port >= 1 && port <= NUM_PORTS) {
                    g_band_map[b] = port;
                    prefs.putUChar(("bm" + String(b)).c_str(), port);
                    Serial.printf("OK %s → port %d\n", BANDS[b].name, port);
                }
            }
        }
    } else if (cmd == "STATUS") {
        Serial.printf("Band:%s  Port:%d  Auto:%s  TX:%s  Src:%s\n",
            g_current_band >= 0 ? BANDS[g_current_band].name : "---",
            g_selected_port,
            g_auto_mode ? "Y" : "N",
            g_tx_active ? "Y" : "N",
            g_input_source == SRC_BCD ? "BCD" : g_input_source == SRC_CIV ? "CI-V" : "Volt");
    } else if (cmd == "HELP") {
        Serial.println("Commands: ANT <1-4>  AUTO  SRC BCD|CIV|VOLT  MAP <band> <port>  STATUS  HELP");
    }
}

// ── Setup ──────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    Serial.println("\n[BOOT] TM-ANT-SW-001 Band Decoder v1.0");

    // BCD inputs (3.3V after level shift)
    pinMode(BCD_B1, INPUT);
    pinMode(BCD_B2, INPUT);
    pinMode(BCD_B4, INPUT);
    pinMode(BCD_B8, INPUT);
    pinMode(PIN_SEND, INPUT);

    // Relay outputs
    for (int i = 0; i < NUM_PORTS; i++) {
        pinMode(RELAY_PINS[i], OUTPUT);
        pinMode(LED_PINS_A[i], OUTPUT);
        digitalWrite(RELAY_PINS[i], LOW);
        digitalWrite(LED_PINS_A[i], LOW);
    }
    pinMode(PIN_BL, OUTPUT);
    digitalWrite(PIN_BL, HIGH);

    // CI-V UART
    Serial2.begin(1200, SERIAL_8N1, CIV_RX, CIV_TX);

    // TFT
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    // NVS
    prefs.begin("bd-sw", false);

    for (int i = 0; i < NUM_PORTS; i++) {
        String def = ANT_NAME_DEFAULT[i];
        prefs.getString(("ant" + String(i)).c_str(), def).toCharArray(g_ant_names[i], 16);
    }
    for (int b = 0; b < NUM_BANDS; b++) {
        uint8_t saved = prefs.getUChar(("bm" + String(b)).c_str(), BANDS[b].default_port);
        g_band_map[b] = (saved >= 1 && saved <= NUM_PORTS) ? saved : BANDS[b].default_port;
    }
    g_selected_port = prefs.getUChar("port", 1);
    apply_port(g_selected_port);

    // WiFi
    String ssid = prefs.getString("ssid", "");
    String pass = prefs.getString("pass", "");
    if (ssid.length() > 0) {
        WiFi.begin(ssid.c_str(), pass.c_str());
        uint32_t t = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - t < 8000) delay(250);
        if (WiFi.status() == WL_CONNECTED) {
            g_wifi_ok = true;
            Serial.printf("[WiFi] %s\n", WiFi.localIP().toString().c_str());
            setup_webserver();
        }
    }

    draw_main_screen();
    Serial.println("[BOOT] Ready. Type HELP for commands.");
    Serial.println("[BOOT] Configure WiFi: in serial type WIFI <ssid> <pass>  (then reboot)");
}

// ── Loop ───────────────────────────────────────────────────────────────────
void loop() {
    update_band();
    check_tx();
    handle_serial();
    if (g_wifi_ok) server.handleClient();
    delay(10);
}
