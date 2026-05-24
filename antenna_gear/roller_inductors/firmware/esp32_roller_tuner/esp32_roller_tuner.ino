/**
 * esp32_roller_tuner.ino — TM-ROLLER-ESP32-001 Rev A
 * Motorized Roller Inductor Controller
 *
 * Hardware:
 *   ESP32-WROOM-32
 *   DRV8825 stepper driver (1/16 microstepping)
 *   NEMA 17 stepper motor → 10:1 worm gear → M5×2mm leadscrew
 *   2× limit switches (home / max)
 *   Quadrature encoder (100 PPR, optional)
 *   SSD1306 OLED 128×64 (optional, I2C)
 *   Bluetooth SPP → CYD display or serial terminal
 *
 * Libraries required:
 *   AccelStepper   (Mike McCauley)
 *   Adafruit_SSD1306 + Adafruit_GFX
 *   BluetoothSerial (ESP32 built-in)
 *   ArduinoJson
 *   Preferences (ESP32 built-in)
 */

#include <Arduino.h>
#include <AccelStepper.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "config.h"
#include "position.h"
#include "tuning.h"

// ─── Global Objects ───────────────────────────────────────────────────────────

AccelStepper     stepper(AccelStepper::DRIVER, PIN_STEP, PIN_DIR);
BluetoothSerial  btSerial;
Preferences      prefs;
PositionManager  posMgr;
AutoTuner        autoTuner;
CommandParser    cmdParser;

Adafruit_SSD1306 oled(128, 64, &Wire, -1);
bool oled_ok = false;

// ─── Timing ───────────────────────────────────────────────────────────────────
uint32_t last_bt_ms       = 0;
uint32_t last_oled_ms     = 0;
uint32_t last_btn_ms      = 0;
uint32_t motor_idle_since = 0;
bool     motor_hold       = false;

// ─── Button State ─────────────────────────────────────────────────────────────
struct Button {
    uint8_t  pin;
    bool     last_state;
    uint32_t last_change_ms;
    bool     pressed() {
        bool cur = (digitalRead(pin) == LOW);
        if (cur != last_state && millis() - last_change_ms > DEBOUNCE_MS) {
            last_change_ms = millis();
            last_state = cur;
            if (cur) return true;
        }
        return false;
    }
};

Button btnHome  = { PIN_BTN_HOME,   false, 0 };
Button btnStepP = { PIN_BTN_STEP_P, false, 0 };
Button btnStepM = { PIN_BTN_STEP_M, false, 0 };
Button btnAuto  = { PIN_BTN_AUTO,   false, 0 };

// ─── BT / Serial Command Buffer ───────────────────────────────────────────────
String serialBuf = "";
String btBuf     = "";

// ─── LED Control ──────────────────────────────────────────────────────────────
void updateLED() {
    static uint32_t last_blink = 0;
    static bool     led_state  = false;
    uint32_t period;

    if      (posMgr.fault)                         period = LED_ERROR_MS;
    else if (posMgr.state == MOTOR_HOMING)          period = LED_HOMING_MS;
    else if (posMgr.state == MOTOR_MOVING)          period = LED_MOVING_MS;
    else if (posMgr.home_state == HOME_COMPLETE)    period = LED_IDLE_MS;
    else                                             period = LED_HOMING_MS; // not homed

    if (millis() - last_blink >= period) {
        last_blink = millis();
        led_state = !led_state;
        digitalWrite(PIN_STATUS_LED, led_state);
    }
}

// ─── OLED Display ─────────────────────────────────────────────────────────────
void updateOLED() {
    if (!oled_ok) return;
    if (millis() - last_oled_ms < OLED_UPDATE_MS) return;
    last_oled_ms = millis();

    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);

    // Line 1: Inductance
    oled.setCursor(0, 0);
    if (posMgr.isHomed()) {
        oled.printf("L: %.2f uH", posMgr.currentL());
    } else {
        oled.print("L: --- (not homed)");
    }

    // Line 2: Position
    oled.setCursor(0, 12);
    oled.printf("%.1f turns / %.1fmm",
        posMgr.currentTurns(), posMgr.currentMM());

    // Line 3: Steps
    oled.setCursor(0, 24);
    oled.printf("Step: %ld", posMgr.position_steps);

    // Line 4: Motor state
    oled.setCursor(0, 36);
    const char* states[] = {"IDLE","MOVING","HOMING","FAULT","NO HOME"};
    oled.printf("State: %s", states[(int)posMgr.state]);

    // Line 5: BT indicator
    oled.setCursor(0, 48);
    oled.printf("BT:%s FW:%s",
        btSerial.hasClient() ? "CON" : "---",
        FW_VERSION);

    oled.display();
}

// ─── Bluetooth Status Broadcast ───────────────────────────────────────────────
void broadcastStatus() {
    if (!btSerial.hasClient()) return;
    if (millis() - last_bt_ms < BT_STATUS_INTERVAL_MS) return;
    last_bt_ms = millis();

    StaticJsonDocument<256> doc;
    doc["l_uh"]     = serialized(String(posMgr.currentL(), 3));
    doc["turns"]    = serialized(String(posMgr.currentTurns(), 2));
    doc["pos_mm"]   = serialized(String(posMgr.currentMM(), 2));
    doc["steps"]    = (long)posMgr.position_steps;
    doc["state"]    = (int)posMgr.state;
    doc["homed"]    = posMgr.isHomed();
    doc["moving"]   = posMgr.isMoving();
    doc["fault"]    = posMgr.fault;
    doc["fw"]       = FW_VERSION;

    String out;
    serializeJson(doc, out);
    btSerial.println(out);
}

// ─── Motor Hold Current Reduction ─────────────────────────────────────────────
void manageMotorHold() {
    if (posMgr.isMoving()) {
        motor_idle_since = millis();
        motor_hold = false;
        posMgr.enableMotor();
        return;
    }
    if (!motor_hold && millis() - motor_idle_since > MOTOR_HOLD_TIMEOUT_MS) {
        // Worm gear is self-locking — safe to de-energize completely
        posMgr.disableMotor();
        motor_hold = true;
        Serial.println("MOTOR: hold current off (worm gear self-locks)");
    }
}

// ─── Command Dispatch ─────────────────────────────────────────────────────────
void handleCommand(const String& cmd, bool from_bt) {
    String resp = cmdParser.process(cmd);
    Serial.println(resp);
    if (from_bt && btSerial.hasClient())
        btSerial.println(resp);
}

void checkSerial() {
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\n' || c == '\r') {
            if (serialBuf.length() > 0) {
                handleCommand(serialBuf, false);
                serialBuf = "";
            }
        } else {
            serialBuf += c;
        }
    }
}

void checkBT() {
    while (btSerial.available()) {
        char c = btSerial.read();
        if (c == '\n' || c == '\r') {
            if (btBuf.length() > 0) {
                handleCommand(btBuf, true);
                btBuf = "";
            }
        } else {
            btBuf += c;
        }
    }
}

// ─── Button Handling ──────────────────────────────────────────────────────────
void checkButtons() {
    if (millis() - last_btn_ms < 10) return;
    last_btn_ms = millis();

    if (btnHome.pressed()) {
        Serial.println("BTN: HOME");
        posMgr.startHoming();
    }
    if (btnStepP.pressed()) {
        // Step forward by one half-turn (coil pitch / 2)
        long delta = (long)(COIL_PITCH_MM / 2 * STEPS_PER_MM);
        posMgr.moveRelative(delta);
        Serial.printf("BTN: STEP+ (%ld steps)\n", delta);
    }
    if (btnStepM.pressed()) {
        long delta = -(long)(COIL_PITCH_MM / 2 * STEPS_PER_MM);
        posMgr.moveRelative(delta);
        Serial.printf("BTN: STEP- (%ld steps)\n", delta);
    }
    if (btnAuto.pressed()) {
        // Auto-tune: start inductance sweep and print table
        Serial.println("BTN: AUTO - printing band presets");
        autoTuner.printBandPresets();
    }
}

// ─── setup() ──────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial.printf("\n\nRF-RollerTuner fw=%s built=%s\n", FW_VERSION, FW_BUILD_DATE);

    // GPIO setup
    pinMode(PIN_STATUS_LED, OUTPUT);
    pinMode(PIN_BTN_HOME,   INPUT_PULLUP);
    pinMode(PIN_BTN_STEP_P, INPUT_PULLUP);
    pinMode(PIN_BTN_STEP_M, INPUT_PULLUP);
    pinMode(PIN_BTN_AUTO,   INPUT_PULLUP);

    // I2C and OLED
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    if (oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        oled_ok = true;
        oled.clearDisplay();
        oled.setTextSize(1);
        oled.setTextColor(SSD1306_WHITE);
        oled.setCursor(0, 0);
        oled.println("RF-RollerTuner");
        oled.println(FW_VERSION);
        oled.display();
        Serial.println("OLED: OK");
    } else {
        Serial.println("OLED: not found");
    }

    // Bluetooth
    btSerial.begin(BT_DEVICE_NAME);
    Serial.printf("BT: advertising as '%s'\n", BT_DEVICE_NAME);

    // NVS
    prefs.begin(NVS_NAMESPACE, false);

    // Position manager (AccelStepper)
    stepper.setEnablePin(PIN_ENABLE);
    stepper.setPinsInverted(false, false, true);  // Enable pin inverted (active LOW)
    posMgr.begin(&stepper);
    posMgr.loadPosition(prefs);

    // Auto-tuner and command parser
    autoTuner.begin(&posMgr);
    cmdParser.begin(&posMgr, &autoTuner);

    // Print startup info
    Serial.printf("Coil: d=%.2f\" N=%d pitch=%.2fmm\n",
        COIL_DIAMETER_IN, COIL_TURNS_TOTAL, COIL_PITCH_MM);
    Serial.printf("L range: %.1f - %.1f uH\n", L_MIN_UH, L_MAX_UH);
    Serial.printf("Steps/mm: %.0f  Travel: %.0fmm max\n",
        STEPS_PER_MM, TRAVEL_MAX_MM);
    Serial.printf("Position: %ld steps (%.2f uH) [%s]\n",
        posMgr.position_steps,
        posMgr.currentL(),
        posMgr.isHomed() ? "homed" : "NOT HOMED — send HOME command");

    Serial.println("Commands: HOME STATUS SETL GOTO STEP BAND FREQ SAVE RECALL SWEEP TABLE PRESETS LIST STOP INFO RESET");
}

// ─── loop() ───────────────────────────────────────────────────────────────────
void loop() {
    // Motor update (must call every loop)
    posMgr.update();

    // Sweep advance
    autoTuner.updateSweep();

    // Motor hold current management
    manageMotorHold();

    // Input handling
    checkSerial();
    checkBT();
    checkButtons();

    // Outputs
    broadcastStatus();
    updateOLED();
    updateLED();

    // Periodic NVS position save (every 10s while idle)
    static uint32_t last_save = 0;
    if (!posMgr.isMoving() && millis() - last_save > 10000) {
        last_save = millis();
        posMgr.savePosition(prefs);
    }

    // Watchdog would be fed here if using esp_task_wdt
    // esp_task_wdt_reset();
}
