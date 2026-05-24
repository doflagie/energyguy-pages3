// ============================================================================
// TM-ANT-SW-001  Rev A — Relay-Based Remote Antenna Switch
// ESP32 / CYD (ESP32-2432S028) Firmware
// 4-port SP4T relay switch: WiFi/Web + local CYD touchscreen + push buttons
// ULN2003A relay driver  |  Position LEDs  |  TX-inhibit on SEND line
// ============================================================================

#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <TFT_eSPI.h>       // CYD display driver (configure User_Setup.h for CYD)

// ── Pin definitions ────────────────────────────────────────────────────────
// Relay outputs (via ULN2003A; HIGH = relay energized = antenna selected)
#define PIN_RELAY_1     26
#define PIN_RELAY_2     27
#define PIN_RELAY_3     14
#define PIN_RELAY_4     12

// Status LEDs (active HIGH)
#define PIN_LED_1       25
#define PIN_LED_2       33
#define PIN_LED_3       32
#define PIN_LED_4       13

// Push buttons (active LOW with internal pull-up)
#define PIN_BTN_1        4
#define PIN_BTN_2        5
#define PIN_BTN_3       18
#define PIN_BTN_4       19

// TX inhibit (SEND line from transceiver; HIGH = transmitting on most Yaesu)
#define PIN_TX_INHIBIT  34   // input only

// CYD backlight (active HIGH for CYD 2.8")
#define PIN_BL          21

// Power supply / hardware fault indicator
#define PIN_FAULT_LED   15   // optional; tie to GND if unused

// ── Constants ──────────────────────────────────────────────────────────────
#define NUM_PORTS        4
#define NO_PORT          0   // value meaning no antenna selected
#define TX_INHIBIT_HIGH  1   // set to 0 if your radio's SEND line is active-LOW

// Antenna name strings (user-editable via web or serial)
const char ANT_NAME_DEFAULT[NUM_PORTS][16] = {
    "ANT 1", "ANT 2", "ANT 3", "ANT 4"
};

// ── Color palette (TFT_eSPI 16-bit 565 format) ─────────────────────────────
#define C_BG        TFT_BLACK
#define C_TITLE     TFT_CYAN
#define C_ACTIVE    TFT_GREEN
#define C_INACTIVE  0x4A69   // dark grey
#define C_TX        TFT_RED
#define C_WHITE     TFT_WHITE
#define C_YELLOW    TFT_YELLOW

// ── Global state ───────────────────────────────────────────────────────────
static uint8_t     g_selected    = 1;   // current antenna port (1–4)
static uint8_t     g_pending     = 0;   // pending switch (while TX active)
static bool        g_tx_active   = false;
static bool        g_wifi_ok     = false;
static char        g_ant_names[NUM_PORTS][16];
static uint32_t    g_switch_count[NUM_PORTS] = {0};
static uint32_t    g_total_switches          = 0;

// ── Hardware objects ────────────────────────────────────────────────────────
Preferences  prefs;
WebServer    server(80);
TFT_eSPI     tft = TFT_eSPI();

// ── Relay and LED control ───────────────────────────────────────────────────
static const int RELAY_PINS[NUM_PORTS] = {PIN_RELAY_1, PIN_RELAY_2, PIN_RELAY_3, PIN_RELAY_4};
static const int LED_PINS[NUM_PORTS]   = {PIN_LED_1,   PIN_LED_2,   PIN_LED_3,   PIN_LED_4};
static const int BTN_PINS[NUM_PORTS]   = {PIN_BTN_1,   PIN_BTN_2,   PIN_BTN_3,   PIN_BTN_4};

void set_antenna(uint8_t port) {
    // port: 1-based (1–4).  0 = all off.
    if (port > NUM_PORTS) port = 0;

    // Interlock: de-energize all, then energize selected.
    // Small make-before-break window can be added here if needed.
    for (int i = 0; i < NUM_PORTS; i++) {
        digitalWrite(RELAY_PINS[i], LOW);
        digitalWrite(LED_PINS[i],   LOW);
    }
    if (port > 0) {
        digitalWrite(RELAY_PINS[port - 1], HIGH);
        digitalWrite(LED_PINS[port - 1],   HIGH);
        g_switch_count[port - 1]++;
    }
    g_total_switches++;
    g_selected = port;
    prefs.putUChar("selected", port);
    draw_main_screen();
}

bool tx_active() {
#if TX_INHIBIT_HIGH
    return digitalRead(PIN_TX_INHIBIT) == HIGH;
#else
    return digitalRead(PIN_TX_INHIBIT) == LOW;
#endif
}

void request_switch(uint8_t port) {
    if (tx_active()) {
        g_pending = port;   // defer switch until TX ends
        Serial.printf("[SW] TX active — switch to port %d deferred\n", port);
    } else {
        set_antenna(port);
        g_pending = 0;
        Serial.printf("[SW] Switched to port %d (%s)\n", port, g_ant_names[port-1]);
    }
}

// ── Display ────────────────────────────────────────────────────────────────
void draw_main_screen() {
    tft.fillScreen(C_BG);
    tft.setTextColor(C_TITLE, C_BG);
    tft.setTextSize(2);
    tft.setCursor(10, 8);
    tft.print("ANT SWITCH");

    if (g_tx_active) {
        tft.setTextColor(C_TX, C_BG);
        tft.setCursor(180, 8);
        tft.print("TX");
    }

    // WiFi status
    tft.setTextSize(1);
    tft.setTextColor(g_wifi_ok ? C_ACTIVE : TFT_ORANGE, C_BG);
    tft.setCursor(220, 12);
    tft.print(g_wifi_ok ? "WiFi OK" : "No WiFi");

    // 4 antenna buttons
    for (int i = 0; i < NUM_PORTS; i++) {
        int x = 10 + (i % 2) * 155;
        int y = 40 + (i / 2) * 80;
        bool active = (g_selected == i + 1);
        bool pending = (g_pending == i + 1);

        uint16_t bg = active ? C_ACTIVE : (pending ? C_YELLOW : C_INACTIVE);
        tft.fillRoundRect(x, y, 140, 65, 8, bg);
        tft.drawRoundRect(x, y, 140, 65, 8, C_WHITE);

        tft.setTextColor(active ? C_BG : C_WHITE, bg);
        tft.setTextSize(2);
        tft.setCursor(x + 10, y + 12);
        tft.printf("ANT %d", i + 1);

        tft.setTextSize(1);
        tft.setCursor(x + 10, y + 36);
        tft.print(g_ant_names[i]);

        if (active) {
            tft.setTextColor(C_BG, bg);
            tft.setCursor(x + 10, y + 50);
            tft.print("ACTIVE");
        }
        if (pending) {
            tft.setTextColor(C_BG, bg);
            tft.setCursor(x + 10, y + 50);
            tft.print("PENDING");
        }
    }

    // Status bar at bottom
    tft.setTextSize(1);
    tft.setTextColor(C_WHITE, C_BG);
    tft.setCursor(10, 220);
    tft.printf("Switches: %lu  | IP: %s", g_total_switches,
               g_wifi_ok ? WiFi.localIP().toString().c_str() : "---");
}

// ── Web server handlers ────────────────────────────────────────────────────
void web_root() {
    String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width'>"
        "<style>"
        "body{background:#111;color:#eee;font-family:monospace;padding:20px}"
        "h1{color:#0cf}"
        ".btn{display:inline-block;padding:20px 30px;margin:8px;border:2px solid #555;"
        "border-radius:8px;text-decoration:none;font-size:1.4em;cursor:pointer;"
        "background:#222;color:#eee}"
        ".btn:hover{background:#333}"
        ".active{background:#060;border-color:#0c0;color:#0f0}"
        ".pending{background:#660;border-color:#aa0}"
        ".tx{color:#f44;font-weight:bold}"
        "</style></head><body>"
        "<h1>&#128225; Antenna Switch</h1>";

    if (g_tx_active) html += "<p class='tx'>&#128308; TX ACTIVE — switching deferred</p>";

    html += "<div>";
    for (int i = 0; i < NUM_PORTS; i++) {
        bool act = (g_selected == i + 1);
        bool pend = (g_pending == i + 1);
        html += "<a href='/set?port=" + String(i+1) + "' class='btn";
        if (act)  html += " active";
        if (pend) html += " pending";
        html += "'>" + String(g_ant_names[i]);
        if (act)  html += "<br><small>&#9679; ACTIVE</small>";
        if (pend) html += "<br><small>PENDING</small>";
        html += "</a>";
    }
    html += "</div>";
    html += "<p style='margin-top:20px;font-size:0.9em'>Total switches: " + String(g_total_switches) + "</p>";
    html += "<p><a href='/api/status' style='color:#0af'>JSON Status</a> | "
            "<a href='/config' style='color:#0af'>Config</a></p>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}

void web_set() {
    if (server.hasArg("port")) {
        uint8_t p = server.arg("port").toInt();
        if (p >= 1 && p <= NUM_PORTS) {
            request_switch(p);
            server.sendHeader("Location", "/");
            server.send(302, "text/plain", "Redirecting...");
            return;
        }
    }
    server.send(400, "text/plain", "Bad request: port must be 1-4");
}

void web_api_status() {
    StaticJsonDocument<512> doc;
    doc["selected"]   = g_selected;
    doc["pending"]    = g_pending;
    doc["tx_active"]  = g_tx_active;
    doc["wifi_rssi"]  = WiFi.RSSI();
    doc["switches_total"] = g_total_switches;
    JsonArray names = doc.createNestedArray("antenna_names");
    JsonArray counts = doc.createNestedArray("switch_counts");
    for (int i = 0; i < NUM_PORTS; i++) {
        names.add(g_ant_names[i]);
        counts.add(g_switch_count[i]);
    }
    String out;
    serializeJson(doc, out);
    server.send(200, "application/json", out);
}

void web_config() {
    if (server.method() == HTTP_POST) {
        for (int i = 0; i < NUM_PORTS; i++) {
            String key = "name" + String(i + 1);
            if (server.hasArg(key)) {
                String v = server.arg(key);
                v = v.substring(0, 15);
                v.toCharArray(g_ant_names[i], 16);
                prefs.putString(("ant" + String(i)).c_str(), String(g_ant_names[i]));
            }
        }
        draw_main_screen();
        server.sendHeader("Location", "/");
        server.send(302);
        return;
    }
    String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width'>"
        "<style>body{background:#111;color:#eee;font-family:monospace;padding:20px}"
        "input{background:#222;color:#eee;border:1px solid #555;padding:6px}"
        "button{padding:10px 20px;background:#006;color:#eee;border:none;cursor:pointer}"
        "</style></head><body><h2>Configure Antenna Names</h2>"
        "<form method='POST' action='/config'>";
    for (int i = 0; i < NUM_PORTS; i++) {
        html += "<label>ANT " + String(i+1) + ": <input name='name" + String(i+1) +
                "' value='" + String(g_ant_names[i]) + "' maxlength='15'></label><br><br>";
    }
    html += "<button type='submit'>Save</button> <a href='/'>Cancel</a></form></body></html>";
    server.send(200, "text/html", html);
}

void setup_webserver() {
    server.on("/",          HTTP_GET,  web_root);
    server.on("/set",       HTTP_GET,  web_set);
    server.on("/api/status",HTTP_GET,  web_api_status);
    server.on("/config",    HTTP_GET,  web_config);
    server.on("/config",    HTTP_POST, web_config);
    server.begin();
}

// ── Button debounce ────────────────────────────────────────────────────────
static uint32_t btn_last_time[NUM_PORTS] = {0};
static bool     btn_last_state[NUM_PORTS] = {true};  // true = not pressed

void check_buttons() {
    for (int i = 0; i < NUM_PORTS; i++) {
        bool state = digitalRead(BTN_PINS[i]);  // LOW = pressed (pull-up)
        if (!state && btn_last_state[i] &&
            (millis() - btn_last_time[i]) > 50) {
            btn_last_time[i] = millis();
            Serial.printf("[BTN] Button %d pressed\n", i + 1);
            request_switch(i + 1);
        }
        btn_last_state[i] = state;
    }
}

// ── TX inhibit monitor ─────────────────────────────────────────────────────
void check_tx_inhibit() {
    bool tx = tx_active();
    if (tx != g_tx_active) {
        g_tx_active = tx;
        Serial.printf("[TX] TX %s\n", tx ? "ACTIVE" : "ended");
        if (!tx && g_pending > 0) {
            Serial.printf("[SW] TX cleared — executing pending switch to port %d\n", g_pending);
            set_antenna(g_pending);
            g_pending = 0;
        }
        draw_main_screen();
    }
}

// ── Serial command interface ───────────────────────────────────────────────
void handle_serial() {
    if (!Serial.available()) return;
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();

    if (cmd.startsWith("ANT ") || cmd.startsWith("PORT ")) {
        int p = cmd.substring(4).toInt();
        if (p >= 1 && p <= NUM_PORTS) {
            request_switch(p);
            Serial.printf("OK port %d\n", p);
        } else {
            Serial.println("ERR: port out of range");
        }
    } else if (cmd == "STATUS") {
        Serial.printf("Selected: %d  Pending: %d  TX: %s  Switches: %lu\n",
                       g_selected, g_pending, g_tx_active ? "ACTIVE" : "idle",
                       g_total_switches);
        for (int i = 0; i < NUM_PORTS; i++)
            Serial.printf("  ANT%d: %s  count=%lu\n", i+1, g_ant_names[i], g_switch_count[i]);
    } else if (cmd == "OFF") {
        set_antenna(0);
        Serial.println("OK all ports off");
    } else if (cmd == "HELP") {
        Serial.println("Commands: ANT <1-4>  STATUS  OFF  HELP");
    } else if (cmd.length() > 0) {
        Serial.println("ERR: unknown command. Type HELP");
    }
}

// ── Setup ──────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    Serial.println("\n[BOOT] TM-ANT-SW-001 Relay Switch v1.0");

    // GPIO init
    for (int i = 0; i < NUM_PORTS; i++) {
        pinMode(RELAY_PINS[i], OUTPUT);
        pinMode(LED_PINS[i],   OUTPUT);
        pinMode(BTN_PINS[i],   INPUT_PULLUP);
        digitalWrite(RELAY_PINS[i], LOW);
        digitalWrite(LED_PINS[i],   LOW);
    }
    pinMode(PIN_TX_INHIBIT, INPUT);
    pinMode(PIN_BL, OUTPUT);
    digitalWrite(PIN_BL, HIGH);

    // CYD / TFT init
    tft.init();
    tft.setRotation(1);   // landscape for CYD 2.8"
    tft.fillScreen(C_BG);
    tft.setTextColor(C_TITLE, C_BG);
    tft.setTextSize(2);
    tft.setCursor(40, 100);
    tft.print("ANT SWITCH");
    tft.setTextSize(1);
    tft.setCursor(60, 130);
    tft.setTextColor(C_WHITE, C_BG);
    tft.print("Initializing...");

    // Load persistent settings
    prefs.begin("ant-sw", false);
    g_selected = prefs.getUChar("selected", 1);
    for (int i = 0; i < NUM_PORTS; i++) {
        String def = ANT_NAME_DEFAULT[i];
        String saved = prefs.getString(("ant" + String(i)).c_str(), def);
        saved.toCharArray(g_ant_names[i], 16);
        g_switch_count[i] = prefs.getULong(("cnt" + String(i)).c_str(), 0);
        g_total_switches += g_switch_count[i];
    }

    // WiFi
    String ssid = prefs.getString("ssid", "");
    String pass = prefs.getString("pass", "");
    if (ssid.length() > 0) {
        Serial.printf("[WiFi] Connecting to %s...\n", ssid.c_str());
        WiFi.begin(ssid.c_str(), pass.c_str());
        uint32_t t = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - t < 8000) {
            delay(250);
        }
        if (WiFi.status() == WL_CONNECTED) {
            g_wifi_ok = true;
            Serial.printf("[WiFi] Connected: %s\n", WiFi.localIP().toString().c_str());
            setup_webserver();
        } else {
            Serial.println("[WiFi] Failed — continuing without WiFi");
        }
    } else {
        Serial.println("[WiFi] No SSID configured. Use serial: WIFI ssid pass");
    }

    // Restore last selected antenna
    if (!tx_active()) {
        set_antenna(g_selected);
    } else {
        draw_main_screen();
    }

    Serial.println("[BOOT] Ready. Type HELP for commands.");
}

// ── Loop ───────────────────────────────────────────────────────────────────
void loop() {
    check_buttons();
    check_tx_inhibit();
    handle_serial();
    if (g_wifi_ok) server.handleClient();
    delay(10);
}
