/**
 * esp32_cap_tuner.ino — TM-VCAP-ESP32-001 Rev A
 * Motorized Variable Capacitor Controller
 *
 * Hardware:
 *   ESP32-WROOM-32
 *   DRV8825 stepper driver (1/16 microstepping)
 *   NEMA 17 → 10:1 belt/gear reduction → cap shaft
 *   10kΩ rotary pot on cap shaft (position feedback)
 *   2× limit switches (min/max rotation)
 *   SSD1306 OLED 128×64 (optional)
 *   Bluetooth SPP → CYD display or phone terminal
 *
 * Libraries:
 *   AccelStepper, Adafruit_SSD1306, BluetoothSerial, ArduinoJson, Preferences
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

// ─── Globals ──────────────────────────────────────────────────────────────────

AccelStepper       stepper(AccelStepper::DRIVER, PIN_STEP, PIN_DIR);
BluetoothSerial    btSerial;
Preferences        prefs;
CapPositionManager capPos;
CapAutoTuner       autoTuner;
CapCommandParser   cmdParser;
Adafruit_SSD1306   oled(128, 64, &Wire, -1);
bool               oled_ok = false;

// ─── Timing ───────────────────────────────────────────────────────────────────

uint32_t last_bt_ms      = 0;
uint32_t last_oled_ms    = 0;
uint32_t last_btn_ms     = 0;
uint32_t last_poscheck   = 0;
uint32_t motor_idle_since = 0;
bool     motor_held       = false;

// ─── Button State ─────────────────────────────────────────────────────────────

struct Btn {
    uint8_t  pin;
    bool     last;
    uint32_t last_ms;
    bool pressed() {
        bool cur = digitalRead(pin) == LOW;
        if (cur != last && millis() - last_ms > DEBOUNCE_MS) {
            last_ms = millis();
            last = cur;
            return cur;
        }
        return false;
    }
};

Btn btnHome    = { PIN_BTN_HOME,    false, 0 };
Btn btnCPlus   = { PIN_BTN_C_PLUS,  false, 0 };
Btn btnCMinus  = { PIN_BTN_C_MINUS, false, 0 };
Btn btnPreset  = { PIN_BTN_PRESET,  false, 0 };

static int preset_idx = 0;

// ─── Command Buffers ──────────────────────────────────────────────────────────

String serialBuf = "", btBuf = "";

// ─── LED ──────────────────────────────────────────────────────────────────────

void updateLED() {
    static uint32_t last_blink = 0;
    static bool     state = false;
    uint32_t period;
    switch (capPos.state) {
        case CAP_FAULT:   period = LED_ERROR_MS;   break;
        case CAP_HOMING:  period = LED_HOMING_MS;  break;
        case CAP_MOVING:  period = LED_MOVING_MS;  break;
        case CAP_UNCAL:   period = LED_UNCAL_MS;   break;
        default:          period = LED_IDLE_MS;    break;
    }
    if (millis() - last_blink >= period) {
        last_blink = millis();
        state = !state;
        digitalWrite(PIN_STATUS_LED, state);
    }
}

// ─── Position Verification ────────────────────────────────────────────────────

void checkPositionError() {
    if (!capPos.homed || capPos.isMoving()) return;
    if (millis() - last_poscheck < 2000) return;
    last_poscheck = millis();

    float err = capPos.positionError();
    if (err > 5.0f) {  // More than 5° discrepancy
        Serial.printf("WARN: position error %.1f° (stepper vs pot)\n", err);
        Serial.println("  Possible missed steps. Suggest CALPOT to re-sync.");
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

    const char* state_str[] = {"IDLE","MOVING","HOMING","FAULT","UNCAL"};

    oled.setCursor(0, 0);
    if (capPos.homed) oled.printf("C: %.1f pF", capPos.currentC());
    else              oled.print("C: --- (uncal)");

    oled.setCursor(0, 12);
    oled.printf("Angle: %.1f deg", capPos.currentAngle());

    oled.setCursor(0, 24);
    oled.printf("Pot: %.1f pF / %.1f deg",
        capPos.potC(), capPos.potAngle());

    oled.setCursor(0, 36);
    oled.printf("State: %s", state_str[(int)capPos.state]);

    oled.setCursor(0, 48);
    oled.printf("BT:%s FW:%s",
        btSerial.hasClient() ? "CON" : "---", FW_VERSION);

    oled.display();
}

// ─── BT Status Broadcast ─────────────────────────────────────────────────────

void broadcastStatus() {
    if (!btSerial.hasClient()) return;
    if (millis() - last_bt_ms < BT_STATUS_INTERVAL_MS) return;
    last_bt_ms = millis();

    StaticJsonDocument<256> doc;
    doc["c_pf"]      = serialized(String(capPos.currentC(), 1));
    doc["angle"]     = serialized(String(capPos.currentAngle(), 2));
    doc["pot_c"]     = serialized(String(capPos.potC(), 1));
    doc["pot_ang"]   = serialized(String(capPos.potAngle(), 1));
    doc["steps"]     = (long)capPos.pos_steps;
    doc["state"]     = (int)capPos.state;
    doc["homed"]     = capPos.homed;
    doc["moving"]    = capPos.isMoving();
    doc["fault"]     = capPos.fault;
    doc["pos_err"]   = serialized(String(capPos.positionError(), 2));
    doc["fw"]        = FW_VERSION;

    String out;
    serializeJson(doc, out);
    btSerial.println(out);
}

// ─── Motor Hold Management ────────────────────────────────────────────────────

void manageMotorHold() {
    if (capPos.isMoving()) {
        motor_idle_since = millis();
        motor_held = false;
        capPos.enableMotor();
        return;
    }
    if (!motor_held && millis() - motor_idle_since > MOTOR_HOLD_TIMEOUT_MS) {
        // Variable capacitor shaft has NO self-locking mechanism (unlike worm gear)
        // ONLY de-energize if using a worm gear (self-locking).
        // With belt drive: motor must stay energized to hold position!
        // Check GEAR_TYPE and decide:
#ifdef CAP_GEAR_SELF_LOCKING
        capPos.disableMotor();
        motor_held = true;
        Serial.println("MOTOR: de-energized (self-locking gear)");
#else
        // Keep motor energized — belt drive not self-locking
        // Reduce current instead (not implemented in DRV8825 without separate pin)
        // For now: keep full hold current
#endif
    }
}

// ─── Command Handling ─────────────────────────────────────────────────────────

void handleCommand(const String& cmd, bool from_bt) {
    String resp = cmdParser.process(cmd);
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

// ─── Button Handling ─────────────────────────────────────────────────────────

void checkButtons() {
    if (millis() - last_btn_ms < 10) return;
    last_btn_ms = millis();

    if (btnHome.pressed()) {
        Serial.println("BTN: HOME");
        capPos.startHoming();
    }
    if (btnCPlus.pressed()) {
        // Step one small increment (about 1 pF at midrange)
        long delta = (long)(1.0f / DEG_PER_STEP);  // ~1 degree = ~1.3pF at midrange
        capPos.moveRelative(delta);
        Serial.printf("BTN: C+ (%.1f pF)\n", capPos.currentC());
    }
    if (btnCMinus.pressed()) {
        long delta = -(long)(1.0f / DEG_PER_STEP);
        capPos.moveRelative(delta);
        Serial.printf("BTN: C- (%.1f pF)\n", capPos.currentC());
    }
    if (btnPreset.pressed()) {
        preset_idx = (preset_idx + 1) % N_CAP_PRESETS;
        const char* name = CAP_BAND_PRESETS[preset_idx].name;
        autoTuner.goToNamedBand(name);
        Serial.printf("BTN: PRESET -> %s\n", name);
    }
}

// ─── setup() ─────────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial.printf("\n\nRF-CapTuner fw=%s built=%s\n", FW_VERSION, FW_BUILD_DATE);

    // GPIO
    pinMode(PIN_STATUS_LED,  OUTPUT);
    pinMode(PIN_BTN_HOME,    INPUT_PULLUP);
    pinMode(PIN_BTN_C_PLUS,  INPUT_PULLUP);
    pinMode(PIN_BTN_C_MINUS, INPUT_PULLUP);
    pinMode(PIN_BTN_PRESET,  INPUT_PULLUP);

    // ADC attenuation for 0–3.3V range
    analogSetAttenuation(ADC_11db);
    analogReadResolution(12);

    // I2C + OLED
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    if (oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        oled_ok = true;
        oled.clearDisplay();
        oled.setTextSize(1);
        oled.setTextColor(SSD1306_WHITE);
        oled.setCursor(0,0); oled.println("RF-CapTuner");
        oled.setCursor(0,12); oled.println(FW_VERSION);
        oled.display();
    }

    // Bluetooth
    btSerial.begin(BT_DEVICE_NAME);
    Serial.printf("BT: advertising as '%s'\n", BT_DEVICE_NAME);

    // NVS
    prefs.begin(NVS_NAMESPACE, false);

    // Stepper + capacitor position
    stepper.setEnablePin(PIN_ENABLE);
    stepper.setPinsInverted(false, false, true);
    capPos.begin(&stepper, prefs);

    // Auto-tuner + parser
    autoTuner.begin(&capPos);
    cmdParser.begin(&capPos, &autoTuner);

    // Print startup info
    Serial.printf("Cap: %.0f-%.0f pF, %.0f deg rotation\n",
        CAP_C_MIN_PF, CAP_C_MAX_PF, CAP_ROTATION_DEG);
    Serial.printf("Plate calc: N=%d d=%.1fmm → C_max=%.0f pF\n",
        CAP_PLATE_N, CAP_PLATE_GAP_MM,
        CapCalc::calcPlateCapPF(CAP_PLATE_DIA_MM, CAP_PLATE_N, CAP_PLATE_GAP_MM));
    Serial.printf("V rating: %.1f kV (%.1f mm gap)\n",
        CapCalc::voltageRatingKV(CAP_PLATE_GAP_MM), CAP_PLATE_GAP_MM);
    Serial.printf("Pot: cal=%s (raw=%d)\n",
        capPos.pot.calibrated ? "OK" : "NEEDED", capPos.pot.rawCount());
    Serial.printf("Commands: HOME CALMIN CALMAX CALPOT SETC BAND FREQ SWEEP TABLE VTABLE STATUS INFO\n");

    if (!capPos.homed)
        Serial.println(">>> Send CALMIN (at min C) then CALMAX (at max C) to calibrate pot <<<");
}

// ─── loop() ──────────────────────────────────────────────────────────────────

void loop() {
    capPos.update();
    autoTuner.updateSweep();
    manageMotorHold();
    checkSerial();
    checkBT();
    checkButtons();
    broadcastStatus();
    updateOLED();
    updateLED();
    checkPositionError();

    // Periodic NVS save
    static uint32_t last_save = 0;
    if (!capPos.isMoving() && millis() - last_save > 10000) {
        last_save = millis();
        capPos.savePosition(prefs);
    }
}
