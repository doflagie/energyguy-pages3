/**
 * esp32_coil_winder.ino — TM-CWND-ESP32-001 Rev A
 * Motorized Coil Winder Controller
 *
 * Hardware:
 *   ESP32-WROOM-32
 *   TMC2208 or A4988 × 2 (spindle + traverse stepper drivers)
 *   NEMA 23 (spindle) + NEMA 17 (traverse + T8 leadscrew)
 *   SSD1306 OLED 128×64
 *   Rotary encoder w/ push button (menu nav)
 *   Optical or Hall turn sensor
 *   Wire break optical sensor
 *   Foot pedal input
 *   Traverse home limit switch
 *   10kΩ speed pot (ADC)
 *   SD card (SPI)
 *   Bluetooth SPP → CYD display
 *
 * Libraries:
 *   AccelStepper, Adafruit_SSD1306, BluetoothSerial, ArduinoJson,
 *   Preferences, SD (built-in ESP32)
 */

#include <Arduino.h>
#include <AccelStepper.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include "config.h"
#include "winder.h"
#include "menu.h"

// ─── Hardware Objects ─────────────────────────────────────────────────────────

AccelStepper     spindle(AccelStepper::DRIVER, PIN_SPIN_STEP, PIN_SPIN_DIR);
AccelStepper     traverse(AccelStepper::DRIVER, PIN_TRAV_STEP, PIN_TRAV_DIR);
BluetoothSerial  btSerial;
Preferences      prefs;
Adafruit_SSD1306 oled(128, 64, &Wire, -1);
bool             oled_ok = false;

// ─── Application Objects ──────────────────────────────────────────────────────

WinderController winder;
PresetManager    presetMgr;
SDLogger         sdlog;
RotaryEncoder    encoder;
MenuSystem       menu;

// ─── ISR Flags ────────────────────────────────────────────────────────────────

volatile bool     rev_complete_flag = false;  // Set by turn sensor ISR
volatile uint32_t wire_break_low_since = 0;   // For wire break debounce

// ─── Timing ───────────────────────────────────────────────────────────────────

uint32_t last_bt_ms     = 0;
uint32_t last_oled_ms   = 0;
uint32_t last_speed_ms  = 0;
uint32_t last_btn_ms    = 0;
uint32_t motor_idle_since = 0;

// ─── BT Receive Buffer ────────────────────────────────────────────────────────

String btBuf = "", serialBuf = "";

// ─── Turn Sensor ISR ─────────────────────────────────────────────────────────

void IRAM_ATTR onTurnSensor() {
    winder.turns.pulse_count++;
    rev_complete_flag = true;
}

// ─── Wire Break Detection ─────────────────────────────────────────────────────

void checkWireBreak() {
    if (!winder.isRunning()) return;
    bool broken = (digitalRead(PIN_WIRE_BREAK) == LOW);
    if (broken) {
        if (wire_break_low_since == 0)
            wire_break_low_since = millis();
        else if (millis() - wire_break_low_since > WIRE_BREAK_DEBOUNCE_MS)
            winder.setFault(FAULT_WIRE_BREAK);
    } else {
        wire_break_low_since = 0;
    }
}

// ─── Foot Pedal / E-Stop ─────────────────────────────────────────────────────

void checkFootPedal() {
    static bool last_pedal = false;
    if (millis() - last_btn_ms < DEBOUNCE_MS) return;

    bool pedal = (digitalRead(PIN_FOOT_PEDAL) == LOW);
    if (pedal && !last_pedal) {
        last_btn_ms = millis();
        if (winder.isRunning())
            winder.pauseWinding();
        else if (winder.state == WINDER_PAUSED)
            winder.resumeWinding();
        else if (winder.state == WINDER_IDLE || winder.state == WINDER_DONE)
            winder.startWinding();
        Serial.printf("PEDAL: state=%d\n", (int)winder.state);
    }
    last_pedal = pedal;
}

// ─── Traverse Limit Switch ────────────────────────────────────────────────────

void checkTraverseLimit() {
    if (digitalRead(PIN_TRAV_LIMIT_MIN) == LOW) {
        if (winder.state == WINDER_HOMING) {
            winder.traverse.onHomeLimit();
            winder.state = WINDER_IDLE;
            Serial.println("TRAV: homed");
        } else if (winder.isRunning()) {
            // Unexpected limit during winding → fault
            winder.setFault(FAULT_TRAV_LIMIT);
            Serial.println("TRAV: unexpected limit — FAULT");
        }
    }
}

// ─── Speed Pot ────────────────────────────────────────────────────────────────

void updateSpeedFromPot() {
    if (millis() - last_speed_ms < 100) return;
    last_speed_ms = millis();
    int raw = analogRead(PIN_SPEED_POT);
    float rpm = MIN_RPM + ((float)raw / 4095.0f) * (MAX_RPM - MIN_RPM);
    winder.setSpeed(rpm);
}

// ─── LED ──────────────────────────────────────────────────────────────────────

void updateLED() {
    static uint32_t last_blink = 0;
    static bool     state = false;
    uint32_t period;
    switch (winder.state) {
        case WINDER_FAULT:   period = LED_FAULT_MS;   break;
        case WINDER_HOMING:  period = LED_HOMING_MS;  break;
        case WINDER_WINDING: period = LED_WINDING_MS; break;
        case WINDER_PAUSED:  period = LED_PAUSED_MS;  break;
        default:             period = LED_IDLE_MS;    break;
    }
    if (millis() - last_blink >= period) {
        last_blink = millis();
        state = !state;
        digitalWrite(PIN_STATUS_LED, state);
    }
}

// ─── BT Status Broadcast ──────────────────────────────────────────────────────

void broadcastStatus() {
    if (!btSerial.hasClient()) return;
    if (millis() - last_bt_ms < BT_STATUS_INTERVAL_MS) return;
    last_bt_ms = millis();

    StaticJsonDocument<384> doc;
    doc["turns"]     = (unsigned long)winder.turns.turns;
    doc["target"]    = (unsigned long)winder.program.target_turns;
    doc["rpm"]       = serialized(String(winder.turns.rpm, 1));
    doc["calc_uh"]   = serialized(String(winder.program.calc_uh, 2));
    doc["layer"]     = winder.traverse.layer + 1;
    doc["trav_mm"]   = serialized(String(winder.traverse.pos_mm, 1));
    doc["state"]     = (int)winder.state;
    doc["fault"]     = (int)winder.fault;
    doc["homed"]     = winder.traverse.homed;
    doc["awg"]       = winder.program.awg;
    doc["former_mm"] = serialized(String(winder.program.former_dia_mm, 1));
    doc["layers"]    = winder.program.layers;
    doc["fw"]        = FW_VERSION;

    String out;
    serializeJson(doc, out);
    btSerial.println(out);
}

// ─── Serial Command Handler ───────────────────────────────────────────────────

void handleCommand(const String& raw, bool from_bt) {
    String cmd = raw;
    cmd.trim();
    cmd.toUpperCase();
    String resp = "";

    if (cmd == "START")      { winder.startWinding(); resp = "OK: START"; }
    else if (cmd == "PAUSE") { winder.pauseWinding(); resp = "OK: PAUSE"; }
    else if (cmd == "RESUME"){ winder.resumeWinding();resp = "OK: RESUME";}
    else if (cmd == "STOP")  { winder.stopWinding();  resp = "OK: STOP";  }
    else if (cmd == "HOME")  { winder.startHoming();  resp = "OK: HOMING";}
    else if (cmd == "FAULT") { winder.clearFault();   resp = "OK: FAULT CLEARED"; }
    else if (cmd == "STATUS") {
        resp = String("TURNS=") + (unsigned long)winder.turns.turns
             + " TARGET=" + (unsigned long)winder.program.target_turns
             + " RPM=" + winder.turns.rpm
             + " L=" + winder.program.calc_uh + "uH"
             + " AWG=" + winder.program.awg
             + " DIA=" + winder.program.former_dia_mm + "mm"
             + " STATE=" + (int)winder.state;
    }
    else if (cmd.startsWith("TURNS ")) {
        winder.program.target_turns = cmd.substring(6).toInt();
        winder.program.target_uh = 0;
        winder.program.calculate();
        resp = "OK: turns=" + String((unsigned long)winder.program.target_turns);
    }
    else if (cmd.startsWith("AWG ")) {
        winder.program.awg = (uint8_t)cmd.substring(4).toInt();
        winder.program.calculate();
        resp = "OK: AWG=" + String(winder.program.awg);
    }
    else if (cmd.startsWith("DIA ")) {
        winder.program.former_dia_mm = cmd.substring(4).toFloat();
        winder.program.calculate();
        resp = "OK: former_dia=" + String(winder.program.former_dia_mm);
    }
    else if (cmd.startsWith("LEN ")) {
        winder.program.coil_len_mm = cmd.substring(4).toFloat();
        winder.program.calculate();
        resp = "OK: coil_len=" + String(winder.program.coil_len_mm);
    }
    else if (cmd.startsWith("L ")) {
        // Set inductance target, calculate turns
        float uh = cmd.substring(2).toFloat();
        winder.program.target_uh = uh;
        if (winder.program.former_dia_mm > 0 && winder.program.coil_len_mm > 0) {
            winder.program.target_turns = WinderMath::turnsForInductance(
                uh, winder.program.former_dia_mm, winder.program.coil_len_mm);
            winder.program.calculate();
            resp = "OK: L=" + String(uh) + "uH → turns=" + (unsigned long)winder.program.target_turns;
        } else {
            resp = "ERROR: set DIA and LEN first";
        }
    }
    else if (cmd.startsWith("RPM ")) {
        float rpm = cmd.substring(4).toFloat();
        winder.setSpeed(rpm);
        resp = "OK: RPM=" + String(rpm);
    }
    else if (cmd.startsWith("LAYERS ")) {
        winder.program.layers = (uint8_t)cmd.substring(7).toInt();
        resp = "OK: layers=" + String(winder.program.layers);
    }
    else if (cmd.startsWith("PITCH ")) {
        winder.program.pitch_factor = cmd.substring(6).toFloat();
        resp = "OK: pitch=" + String(winder.program.pitch_factor);
    }
    else if (cmd.startsWith("PRESET SAVE ")) {
        int slot = cmd.substring(12).toInt();
        presetMgr.save(slot, winder.program);
        resp = "OK: saved to slot " + String(slot);
    }
    else if (cmd.startsWith("PRESET LOAD ")) {
        int slot = cmd.substring(12).toInt();
        if (presetMgr.load(slot, winder.program))
            resp = "OK: loaded slot " + String(slot);
        else
            resp = "ERROR: slot empty";
    }
    else if (cmd == "CALC") {
        float L = WinderMath::calcInductance(winder.program);
        resp = "CALC: L=" + String(L, 2) + "uH"
             + " turns=" + (unsigned long)winder.program.target_turns
             + " wire=" + String(winder.program.total_wire_m, 2) + "m"
             + " R=" + String(winder.program.dc_resistance, 3) + "ohm";
    }
    else if (cmd == "WTABLE") {
        Serial.println("AWG  dia_mm  ins_mm  ohm/m   A_max");
        for (int i = 0; i < N_WIRE_SPECS; i++)
            Serial.printf("%2d   %.3f   %.3f   %.5f  %.1f\n",
                WIRE_TABLE[i].awg, WIRE_TABLE[i].dia_mm,
                WIRE_TABLE[i].dia_ins_mm, WIRE_TABLE[i].ohm_per_m,
                WIRE_TABLE[i].max_a);
        resp = "OK: wire table printed";
    }
    else if (cmd == "INFO") {
        resp = String("RF-CoilWinder fw=") + FW_VERSION
             + " built=" + FW_BUILD_DATE
             + " TRAV_STEPS_PER_MM=" + (int)TRAV_STEPS_PER_MM
             + " SPIN_STEPS_PER_ROT=" + (int)SPIN_STEPS_PER_ROT;
    }
    else if (cmd == "RESET") ESP.restart();
    else resp = "? (try: START STOP PAUSE RESUME HOME TURNS AWG DIA LEN L RPM LAYERS PITCH PRESET CALC WTABLE STATUS INFO RESET)";

    Serial.println(resp);
    if (from_bt && btSerial.hasClient()) btSerial.println(resp);
}

void checkSerial() {
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\n' || c == '\r') {
            if (serialBuf.length()) { handleCommand(serialBuf, false); serialBuf = ""; }
        } else serialBuf += c;
    }
}

void checkBT() {
    while (btSerial.available()) {
        char c = btSerial.read();
        if (c == '\n' || c == '\r') {
            if (btBuf.length()) { handleCommand(btBuf, true); btBuf = ""; }
        } else btBuf += c;
    }
}

// ─── Motor Management ─────────────────────────────────────────────────────────

void manageMotorHold() {
    if (winder.isRunning() || winder.state == WINDER_HOMING) {
        motor_idle_since = millis();
        spindle.enableOutputs();
        traverse.enableOutputs();
        return;
    }
    // Keep energized for MOTOR_HOLD_TIMEOUT_MS then release traverse
    // (Spindle may need to stay energized if chuck weight is significant)
    if (millis() - motor_idle_since > MOTOR_HOLD_TIMEOUT_MS) {
        traverse.disableOutputs();
        // Spindle: keep energized to prevent chuck rotation
    }
}

// ─── setup() ─────────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial.printf("\n\nRF-CoilWinder fw=%s built=%s\n", FW_VERSION, FW_BUILD_DATE);

    // GPIO
    pinMode(PIN_STATUS_LED,    OUTPUT);
    pinMode(PIN_TURN_SENSOR,   INPUT_PULLUP);
    pinMode(PIN_WIRE_BREAK,    INPUT_PULLUP);
    pinMode(PIN_FOOT_PEDAL,    INPUT_PULLUP);
    pinMode(PIN_TRAV_LIMIT_MIN,INPUT_PULLUP);
    analogSetAttenuation(ADC_11db);
    analogReadResolution(12);

    // Turn sensor interrupt
    attachInterrupt(digitalPinToInterrupt(PIN_TURN_SENSOR),
                    onTurnSensor, FALLING);

    // I2C + OLED
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    if (oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        oled_ok = true;
        oled.clearDisplay();
        oled.setTextSize(1); oled.setTextColor(SSD1306_WHITE);
        oled.setCursor(0,0); oled.println("RF-CoilWinder");
        oled.setCursor(0,12); oled.println(FW_VERSION);
        oled.display();
    }

    // Bluetooth
    btSerial.begin(BT_DEVICE_NAME);
    Serial.printf("BT: advertising as '%s'\n", BT_DEVICE_NAME);

    // NVS
    prefs.begin(NVS_NAMESPACE, false);
    presetMgr.begin(&prefs);

    // SD card
    sdlog.begin();

    // Encoder
    encoder.begin();

    // Steppers
    spindle.setEnablePin(PIN_SPIN_EN);
    spindle.setPinsInverted(false, false, true);
    spindle.setMaxSpeed(SPIN_MAX_SPEED);
    spindle.setAcceleration(SPIN_ACCEL);

    traverse.setEnablePin(PIN_TRAV_EN);
    traverse.setPinsInverted(false, false, true);
    traverse.setMaxSpeed(TRAV_MAX_SPEED);
    traverse.setAcceleration(TRAV_ACCEL);

    // Microstepping
    pinMode(PIN_SPIN_MS1, OUTPUT); digitalWrite(PIN_SPIN_MS1, HIGH);
    pinMode(PIN_SPIN_MS2, OUTPUT); digitalWrite(PIN_SPIN_MS2, HIGH);
    pinMode(PIN_TRAV_MS1, OUTPUT); digitalWrite(PIN_TRAV_MS1, HIGH);
    pinMode(PIN_TRAV_MS2, OUTPUT); digitalWrite(PIN_TRAV_MS2, HIGH);

    // Winder
    winder.begin(&spindle, &traverse);

    // Default program
    winder.program.awg           = 22;
    winder.program.former_dia_mm = 25.4f;  // 1 inch
    winder.program.coil_len_mm   = 25.4f;  // 1 inch
    winder.program.target_turns  = 0;
    winder.program.pitch_factor  = 1.0f;
    winder.program.layers        = 1;
    strncpy(winder.program.name, "DEFAULT", 24);

    // Menu
    menu.begin(oled_ok ? &oled : nullptr,
               &winder, &presetMgr, &sdlog, &encoder);

    Serial.printf("Traverse: %.0f steps/mm, max %.0f mm\n",
        TRAV_STEPS_PER_MM, TRAV_MAX_MM);
    Serial.printf("Spindle: %d steps/rev, max %.0f RPM\n",
        SPIN_STEPS_PER_ROT, SPS_TO_RPM(SPIN_MAX_SPEED));
    Serial.printf("Commands: START STOP PAUSE RESUME HOME TURNS AWG DIA LEN L RPM LAYERS PITCH PRESET CALC WTABLE STATUS INFO RESET\n");
    Serial.println(">>> Send HOME to home traverse before winding <<<");
}

// ─── loop() ──────────────────────────────────────────────────────────────────

void loop() {
    // Run stepper motors (must be called every loop)
    spindle.run();
    traverse.run();

    // Process completed revolution flag (from ISR)
    if (rev_complete_flag) {
        rev_complete_flag = false;
        if (winder.isRunning()) winder.onRevComplete();
    }

    // Safety checks
    checkWireBreak();
    checkTraverseLimit();
    checkFootPedal();

    // Speed control from pot
    updateSpeedFromPot();

    // OLED menu update
    if (millis() - last_oled_ms > OLED_UPDATE_MS) {
        last_oled_ms = millis();
        menu.update();
    }

    // BT / serial
    checkSerial();
    checkBT();
    broadcastStatus();

    // Motor hold management
    manageMotorHold();

    // LED
    updateLED();

    // Done: auto-save to SD
    static bool last_done = false;
    if (winder.isDone() && !last_done) {
        last_done = true;
        sdlog.logWinding(winder.program);
        Serial.printf("DONE: %.0f turns wound, L=%.2f µH\n",
            winder.turns.turns, winder.program.calc_uh);
    }
    if (!winder.isDone()) last_done = false;
}
