// ============================================================================
// TM-ANT-SW-001  Rev A — Motorized Antenna Switch
// ESP32 / CYD Firmware — 28BYJ-48 Stepper + Hall Sensor Position Feedback
// 4-position or 6-position rotary switch
// WiFi/Web UI + CYD touch + push buttons + serial
// ============================================================================

#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <TFT_eSPI.h>

// ── Stepper motor pins (28BYJ-48 via ULN2003A) ────────────────────────────
#define STEP_IN1    26
#define STEP_IN2    27
#define STEP_IN3    14
#define STEP_IN4    12

// ── Hall effect sensor pins (active LOW = magnet present = at this position)
#define HALL_1       4   // position 1 sensor
#define HALL_2       5   // position 2 sensor
#define HALL_3      18   // position 3 sensor
#define HALL_4      19   // position 4 sensor

// ── Status LEDs ────────────────────────────────────────────────────────────
#define LED_1       25
#define LED_2       33
#define LED_3       32
#define LED_4       13

// ── Push buttons (active LOW, internal pull-up) ────────────────────────────
#define BTN_1        4   // NOTE: may conflict with HALL_1 — use different GPIO
#define BTN_FWD     16   // advance one position forward
#define BTN_REV     17   // advance one position backward

// ── TX inhibit ─────────────────────────────────────────────────────────────
#define PIN_TX_INHIBIT  34
#define TX_INHIBIT_HIGH  1

// ── CYD backlight ──────────────────────────────────────────────────────────
#define PIN_BL      21

// ── Switch configuration ───────────────────────────────────────────────────
#define NUM_POSITIONS    4   // change to 6 for SP6T switch
#define STEPS_PER_REV  512   // 28BYJ-48 full steps per output shaft revolution
#define DEGREES_PER_POS (360 / NUM_POSITIONS)   // 90° for 4-pos, 60° for 6-pos
#define STEPS_PER_POS  (STEPS_PER_REV * DEGREES_PER_POS / 360)  // 128 or 85

// Step timing
#define STEP_DELAY_US    4000   // microseconds between steps (4ms = ~250 steps/sec)
#define SETTLE_MS          80   // wait after reaching position before de-energizing

// ── Colors ─────────────────────────────────────────────────────────────────
#define C_BG      TFT_BLACK
#define C_TITLE   TFT_CYAN
#define C_ACTIVE  TFT_GREEN
#define C_IDLE    0x4A69
#define C_MOVING  TFT_YELLOW
#define C_ERROR   TFT_RED
#define C_WHITE   TFT_WHITE

// ── Stepper half-step sequence (8 steps per electrical cycle) ──────────────
static const uint8_t STEP_SEQ[8][4] = {
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 1},
    {1, 0, 0, 1},
};

// ── Global state ───────────────────────────────────────────────────────────
enum MotorState { IDLE, MOVING_FWD, MOVING_REV, HOMING, ERROR_STATE };

static uint8_t    g_position    = 1;      // current position (1-based)
static uint8_t    g_target      = 1;      // target position
static MotorState g_motor_state = IDLE;
static int        g_step_idx    = 0;      // current step in sequence
static int        g_steps_remaining = 0; // steps left to reach target
static uint32_t   g_last_step_us = 0;    // timestamp of last step
static bool       g_tx_active   = false;
static bool       g_wifi_ok     = false;
static uint8_t    g_pending     = 0;
static bool       g_homed       = false;
static char       g_ant_names[NUM_POSITIONS][16];

const char ANT_NAME_DEFAULT[NUM_POSITIONS][16] = {
    "ANT 1", "ANT 2", "ANT 3", "ANT 4"
};

static const int HALL_PINS[4] = {HALL_1, HALL_2, HALL_3, HALL_4};
static const int LED_PINS[4]  = {LED_1,  LED_2,  LED_3,  LED_4};
static const int STEP_PINS[4] = {STEP_IN1, STEP_IN2, STEP_IN3, STEP_IN4};

Preferences prefs;
WebServer   server(80);
TFT_eSPI    tft = TFT_eSPI();

// ── Stepper motor control ──────────────────────────────────────────────────
void step_apply(int idx) {
    for (int i = 0; i < 4; i++)
        digitalWrite(STEP_PINS[i], STEP_SEQ[idx][i]);
}

void step_off() {
    for (int i = 0; i < 4; i++)
        digitalWrite(STEP_PINS[i], LOW);
}

void step_forward_one() {
    g_step_idx = (g_step_idx + 1) % 8;
    step_apply(g_step_idx);
}

void step_backward_one() {
    g_step_idx = (g_step_idx + 7) % 8;   // +7 mod 8 = -1 mod 8
    step_apply(g_step_idx);
}

// ── Hall sensor reading ────────────────────────────────────────────────────
int read_position_from_hall() {
    // Returns 1-based position (1–4), or 0 if between positions / error
    int found = -1;
    int count = 0;
    for (int i = 0; i < NUM_POSITIONS && i < 4; i++) {
        if (digitalRead(HALL_PINS[i]) == LOW) {  // LOW = magnet present
            found = i + 1;
            count++;
        }
    }
    if (count == 1) return found;   // exactly one sensor active = valid position
    return 0;                        // 0 = transitioning or fault
}

void update_leds() {
    for (int i = 0; i < NUM_POSITIONS && i < 4; i++)
        digitalWrite(LED_PINS[i], (g_position == i + 1 && g_motor_state == IDLE) ? HIGH : LOW);

    // Blink target LED while moving
    if (g_motor_state == MOVING_FWD || g_motor_state == MOVING_REV) {
        static uint32_t blink_t = 0;
        static bool blink_state = false;
        if (millis() - blink_t > 200) {
            blink_state = !blink_state;
            blink_t = millis();
            if (g_target >= 1 && g_target <= NUM_POSITIONS)
                digitalWrite(LED_PINS[g_target - 1], blink_state ? HIGH : LOW);
        }
    }
}

// ── Homing sequence ────────────────────────────────────────────────────────
// On power-up: read Hall sensors. If at a valid position, done.
// If not at a valid position: step slowly until a sensor activates.
bool home_sequence() {
    Serial.println("[HOME] Starting homing sequence...");
    tft.fillScreen(C_BG);
    tft.setTextColor(C_MOVING, C_BG);
    tft.setTextSize(2);
    tft.setCursor(20, 100);
    tft.print("HOMING...");

    int hall_pos = read_position_from_hall();
    if (hall_pos > 0) {
        g_position = hall_pos;
        g_homed    = true;
        Serial.printf("[HOME] Already at position %d\n", hall_pos);
        return true;
    }

    // Sweep forward up to one full revolution to find a Hall position
    for (int s = 0; s < STEPS_PER_REV + 50; s++) {
        step_forward_one();
        delayMicroseconds(STEP_DELAY_US);
        hall_pos = read_position_from_hall();
        if (hall_pos > 0) {
            g_position = hall_pos;
            g_homed    = true;
            step_off();
            Serial.printf("[HOME] Found position %d after %d steps\n", hall_pos, s);
            return true;
        }
    }
    step_off();
    Serial.println("[HOME] FAILED — no Hall sensor found");
    g_motor_state = ERROR_STATE;
    return false;
}

// ── Motor task — called from loop() ───────────────────────────────────────
// Non-blocking: takes one step per call if step delay has elapsed.
void motor_run() {
    if (g_motor_state == IDLE || g_motor_state == ERROR_STATE) return;

    if ((micros() - g_last_step_us) < STEP_DELAY_US) return;
    g_last_step_us = micros();

    if (g_steps_remaining > 0) {
        if (g_motor_state == MOVING_FWD) step_forward_one();
        else                             step_backward_one();
        g_steps_remaining--;

        // Check Hall sensors at every step for early stopping
        int hall_pos = read_position_from_hall();
        if (hall_pos == (int)g_target && g_steps_remaining <= STEPS_PER_POS / 4) {
            // Close to target and Hall confirms position — stop early
            g_steps_remaining = 0;
        }
    }

    if (g_steps_remaining == 0) {
        // Verify final position via Hall
        delay(SETTLE_MS);
        int hall_pos = read_position_from_hall();
        if (hall_pos == (int)g_target) {
            g_position    = g_target;
            g_motor_state = IDLE;
            step_off();
            prefs.putUChar("pos", g_position);
            Serial.printf("[SW] Position %d confirmed by Hall sensor\n", g_position);
        } else if (hall_pos > 0) {
            // Reached a position but wrong one — unexpected; accept it
            g_position    = hall_pos;
            g_motor_state = IDLE;
            step_off();
            Serial.printf("[SW] WARN: landed at %d (expected %d)\n", hall_pos, g_target);
        } else {
            // Between positions; advance a few more steps
            g_steps_remaining = STEPS_PER_POS / 8;
            Serial.println("[SW] WARN: Hall not found after motion; continuing...");
        }
        draw_main_screen();
    }
}

// ── Move to position ───────────────────────────────────────────────────────
void move_to(uint8_t target) {
    if (target < 1 || target > NUM_POSITIONS) return;
    if (target == g_position && g_motor_state == IDLE) return;

    // Calculate shortest path (CW vs CCW)
    int cur = g_position - 1;
    int tgt = target - 1;
    int fwd_steps = ((tgt - cur + NUM_POSITIONS) % NUM_POSITIONS) * STEPS_PER_POS;
    int rev_steps = ((cur - tgt + NUM_POSITIONS) % NUM_POSITIONS) * STEPS_PER_POS;

    g_target = target;
    if (fwd_steps <= rev_steps) {
        g_motor_state    = MOVING_FWD;
        g_steps_remaining = fwd_steps;
    } else {
        g_motor_state    = MOVING_REV;
        g_steps_remaining = rev_steps;
    }
    Serial.printf("[SW] Moving %s from %d to %d (%d steps)\n",
        g_motor_state == MOVING_FWD ? "FWD" : "REV",
        g_position, target, g_steps_remaining);
    draw_main_screen();
}

void request_move(uint8_t target) {
    if (g_tx_active) {
        g_pending = target;
        Serial.printf("[SW] TX active — move to %d deferred\n", target);
        return;
    }
    move_to(target);
    g_pending = 0;
}

// ── TX inhibit ─────────────────────────────────────────────────────────────
void check_tx_inhibit() {
    bool tx = (digitalRead(PIN_TX_INHIBIT) == (TX_INHIBIT_HIGH ? HIGH : LOW));
    if (tx != g_tx_active) {
        g_tx_active = tx;
        if (!tx && g_pending > 0) {
            Serial.printf("[SW] TX cleared — executing pending move to %d\n", g_pending);
            move_to(g_pending);
            g_pending = 0;
        }
        draw_main_screen();
    }
}

// ── Display ────────────────────────────────────────────────────────────────
void draw_main_screen() {
    tft.fillScreen(C_BG);
    tft.setTextColor(C_TITLE, C_BG);
    tft.setTextSize(2);
    tft.setCursor(10, 8);
    tft.print("MOTOR SWITCH");

    // Motor state indicator
    const char* state_str = "IDLE";
    uint16_t    state_col  = C_ACTIVE;
    if (g_motor_state == MOVING_FWD)   { state_str = "FWD >>"; state_col = C_MOVING; }
    if (g_motor_state == MOVING_REV)   { state_str = "<< REV"; state_col = C_MOVING; }
    if (g_motor_state == HOMING)       { state_str = "HOMING"; state_col = C_MOVING; }
    if (g_motor_state == ERROR_STATE)  { state_str = "ERROR";  state_col = C_ERROR;  }

    tft.setTextSize(1);
    tft.setTextColor(state_col, C_BG);
    tft.setCursor(200, 12);
    tft.print(state_str);

    for (int i = 0; i < NUM_POSITIONS; i++) {
        int x = 10 + (i % 2) * 155;
        int y = 40 + (i / 2) * 80;
        bool at_pos = (g_position == i + 1 && g_motor_state == IDLE);
        bool is_tgt = (g_target  == i + 1 && g_motor_state != IDLE);

        uint16_t bg = at_pos ? C_ACTIVE : (is_tgt ? C_MOVING : C_IDLE);
        tft.fillRoundRect(x, y, 140, 65, 8, bg);
        tft.drawRoundRect(x, y, 140, 65, 8, C_WHITE);

        tft.setTextColor(at_pos ? C_BG : C_WHITE, bg);
        tft.setTextSize(2);
        tft.setCursor(x + 10, y + 12);
        tft.printf("POS %d", i + 1);

        tft.setTextSize(1);
        tft.setCursor(x + 10, y + 36);
        tft.print(g_ant_names[i]);

        tft.setCursor(x + 10, y + 50);
        if (at_pos)  tft.print("HERE");
        if (is_tgt)  tft.print("TARGET");
    }

    tft.setTextSize(1);
    tft.setTextColor(C_WHITE, C_BG);
    tft.setCursor(10, 220);
    tft.printf("Homed:%s  TX:%s  IP:%s",
        g_homed ? "Y" : "N",
        g_tx_active ? "TX" : "--",
        g_wifi_ok ? WiFi.localIP().toString().c_str() : "---");
}

// ── Web server ─────────────────────────────────────────────────────────────
void web_root() {
    String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width'>"
        "<style>body{background:#111;color:#eee;font-family:monospace;padding:20px}"
        "h1{color:#0cf}.btn{display:inline-block;padding:20px 30px;margin:8px;"
        "border:2px solid #555;border-radius:8px;text-decoration:none;font-size:1.3em;"
        "background:#222;color:#eee}.active{background:#060;border-color:#0c0}"
        ".moving{background:#660;border-color:#aa0}.tx{color:#f44;font-weight:bold}"
        "</style></head><body><h1>&#127917; Motor Switch</h1>";

    String state = "IDLE";
    if (g_motor_state == MOVING_FWD) state = "MOVING FWD";
    if (g_motor_state == MOVING_REV) state = "MOVING REV";
    if (g_motor_state == ERROR_STATE) state = "<span style='color:#f44'>ERROR</span>";
    html += "<p>State: " + state + " | Pos: " + String(g_position) +
            " | Homed: " + (g_homed ? "Yes" : "No") + "</p>";

    if (g_tx_active) html += "<p class='tx'>TX ACTIVE — switch deferred</p>";

    for (int i = 0; i < NUM_POSITIONS; i++) {
        html += "<a href='/move?pos=" + String(i+1) + "' class='btn";
        if (g_position == i+1) html += " active";
        if (g_target  == i+1 && g_motor_state != IDLE) html += " moving";
        html += "'>" + String(g_ant_names[i]) + "</a>";
    }
    html += "<p><a href='/home' style='color:#f80'>Re-Home</a> | "
            "<a href='/api/status' style='color:#0af'>JSON</a></p></body></html>";
    server.send(200, "text/html", html);
}

void web_move() {
    if (server.hasArg("pos")) {
        uint8_t p = server.arg("pos").toInt();
        if (p >= 1 && p <= NUM_POSITIONS) {
            request_move(p);
            server.sendHeader("Location", "/");
            server.send(302);
            return;
        }
    }
    server.send(400, "text/plain", "Bad request");
}

void web_home() {
    Serial.println("[WEB] Re-home requested");
    if (g_motor_state == IDLE) {
        g_homed = false;
        home_sequence();
    }
    server.sendHeader("Location", "/");
    server.send(302);
}

void web_status() {
    String j = "{\"position\":" + String(g_position) +
               ",\"target\":" + String(g_target) +
               ",\"state\":\"" + String(g_motor_state == IDLE ? "idle" :
                    g_motor_state == MOVING_FWD ? "fwd" : "rev") + "\"" +
               ",\"homed\":" + (g_homed ? "true" : "false") +
               ",\"tx\":" + (g_tx_active ? "true" : "false") + "}";
    server.send(200, "application/json", j);
}

void setup_webserver() {
    server.on("/",           HTTP_GET, web_root);
    server.on("/move",       HTTP_GET, web_move);
    server.on("/home",       HTTP_GET, web_home);
    server.on("/api/status", HTTP_GET, web_status);
    server.begin();
}

// ── Serial commands ────────────────────────────────────────────────────────
void handle_serial() {
    if (!Serial.available()) return;
    String cmd = Serial.readStringUntil('\n');
    cmd.trim(); cmd.toUpperCase();

    if (cmd.startsWith("MOVE ") || cmd.startsWith("POS ")) {
        int p = cmd.substring(5).toInt();
        if (p >= 1 && p <= NUM_POSITIONS) {
            request_move(p);
            Serial.printf("OK moving to %d\n", p);
        } else {
            Serial.println("ERR: position out of range");
        }
    } else if (cmd == "HOME") {
        g_homed = false;
        home_sequence();
        set_antenna_leds();
        draw_main_screen();
    } else if (cmd == "STATUS") {
        Serial.printf("Pos:%d Target:%d State:%s Homed:%s TX:%s\n",
            g_position, g_target,
            g_motor_state==IDLE?"IDLE":g_motor_state==MOVING_FWD?"FWD":"REV",
            g_homed?"Y":"N", g_tx_active?"TX":"idle");
    } else if (cmd == "HELP") {
        Serial.println("Commands: MOVE <1-4>  POS <1-4>  HOME  STATUS  HELP");
    }
}

void set_antenna_leds() {
    for (int i = 0; i < NUM_POSITIONS && i < 4; i++)
        digitalWrite(LED_PINS[i], (g_position == i+1) ? HIGH : LOW);
}

// ── Setup ──────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    Serial.println("\n[BOOT] TM-ANT-SW-001 Motor Switch v1.0");

    // Stepper pins
    for (int i = 0; i < 4; i++) {
        pinMode(STEP_PINS[i], OUTPUT);
        digitalWrite(STEP_PINS[i], LOW);
    }
    // Hall sensors
    for (int i = 0; i < 4; i++)
        pinMode(HALL_PINS[i], INPUT_PULLUP);
    // LEDs
    for (int i = 0; i < 4; i++) {
        pinMode(LED_PINS[i], OUTPUT);
        digitalWrite(LED_PINS[i], LOW);
    }
    pinMode(BTN_FWD, INPUT_PULLUP);
    pinMode(BTN_REV, INPUT_PULLUP);
    pinMode(PIN_TX_INHIBIT, INPUT);
    pinMode(PIN_BL, OUTPUT);
    digitalWrite(PIN_BL, HIGH);

    // TFT
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(C_BG);

    // NVS
    prefs.begin("mot-sw", false);
    for (int i = 0; i < NUM_POSITIONS; i++) {
        String def = ANT_NAME_DEFAULT[i];
        String saved = prefs.getString(("ant" + String(i)).c_str(), def);
        saved.toCharArray(g_ant_names[i], 16);
    }
    g_target   = prefs.getUChar("pos", 1);

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

    // Home and move to last position
    if (home_sequence()) {
        if (g_target != g_position) move_to(g_target);
    }

    draw_main_screen();
    Serial.println("[BOOT] Ready.");
}

// ── Loop ───────────────────────────────────────────────────────────────────
void loop() {
    motor_run();
    check_tx_inhibit();
    update_leds();
    handle_serial();
    if (g_wifi_ok) server.handleClient();

    // Forward button
    static bool btn_fwd_last = true;
    bool btn_fwd = digitalRead(BTN_FWD);
    if (!btn_fwd && btn_fwd_last) {
        uint8_t nxt = (g_position % NUM_POSITIONS) + 1;
        request_move(nxt);
    }
    btn_fwd_last = btn_fwd;

    // Reverse button
    static bool btn_rev_last = true;
    bool btn_rev = digitalRead(BTN_REV);
    if (!btn_rev && btn_rev_last) {
        uint8_t prv = (g_position - 2 + NUM_POSITIONS) % NUM_POSITIONS + 1;
        request_move(prv);
    }
    btn_rev_last = btn_rev;

    delay(5);
}
