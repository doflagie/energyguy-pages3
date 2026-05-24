/**
 * position.h — TM-VCAP-ESP32-001 Rev A
 * Variable Capacitor Position Sensing and Capacitance Calculation
 * Supports: rotary potentiometer (absolute) + step counter (relative)
 */

#pragma once
#include <Arduino.h>
#include <AccelStepper.h>
#include <Preferences.h>
#include "config.h"

// ─── ADC Helper ───────────────────────────────────────────────────────────────

// Oversampled ADC read (reduces noise on capacitor position pot)
inline int adcRead(uint8_t pin, int samples = ADC_OVERSAMPLE) {
    long sum = 0;
    for (int i = 0; i < samples; i++) sum += analogRead(pin);
    return (int)(sum / samples);
}

inline float adcToMV(int counts) {
    return (float)counts * ADC_VREF_MV / ADC_COUNTS;
}


// ─── Potentiometer Position Sensor ────────────────────────────────────────────

class PotSensor {
public:
    int   cal_min = POT_COUNT_MIN;  // ADC count at 0° (load from NVS after cal)
    int   cal_max = POT_COUNT_MAX;  // ADC count at 180°
    bool  calibrated = false;

    void begin() {
        pinMode(PIN_POT_ADC, INPUT);
    }

    // Raw ADC read
    int rawCount() const { return adcRead(PIN_POT_ADC); }

    // Angle in degrees (0 = min capacitance, 180 = max capacitance)
    float angleDeg() const {
        int raw = adcRead(PIN_POT_ADC);
        if (cal_max == cal_min) return 0.0f;
        float norm = (float)(raw - cal_min) / (float)(cal_max - cal_min);
        norm = constrain(norm, 0.0f, 1.0f);
        return norm * CAP_ROTATION_DEG;
    }

    // Calibrate minimum position (call at min capacitance, plates fully unmeshed)
    void calMin() {
        cal_min = adcRead(PIN_POT_ADC, 64);
        Serial.printf("POT CAL_MIN: count=%d (%.0fmV)\n",
            cal_min, adcToMV(cal_min));
    }

    // Calibrate maximum position (call at max capacitance, plates fully meshed)
    void calMax() {
        cal_max = adcRead(PIN_POT_ADC, 64);
        calibrated = true;
        Serial.printf("POT CAL_MAX: count=%d (%.0fmV)\n",
            cal_max, adcToMV(cal_max));
    }

    void save(Preferences& p) {
        p.putInt(NVS_KEY_CAL_MIN, cal_min);
        p.putInt(NVS_KEY_CAL_MAX, cal_max);
        p.putBool(NVS_KEY_CALIBRATED, calibrated);
    }

    void load(Preferences& p) {
        cal_min    = p.getInt(NVS_KEY_CAL_MIN, POT_COUNT_MIN);
        cal_max    = p.getInt(NVS_KEY_CAL_MAX, POT_COUNT_MAX);
        calibrated = p.getBool(NVS_KEY_CALIBRATED, false);
    }
};


// ─── Limit Switch Monitor ─────────────────────────────────────────────────────

class CapLimitSwitches {
public:
    volatile bool min_tripped = false;
    volatile bool max_tripped = false;

    void begin() {
        pinMode(PIN_LIM_MIN, INPUT_PULLUP);
        pinMode(PIN_LIM_MAX, INPUT_PULLUP);
        attachInterruptArg(digitalPinToInterrupt(PIN_LIM_MIN), _isr_min, this, FALLING);
        attachInterruptArg(digitalPinToInterrupt(PIN_LIM_MAX), _isr_max, this, FALLING);
    }

    bool minActive() const { return digitalRead(PIN_LIM_MIN) == LOW; }
    bool maxActive() const { return digitalRead(PIN_LIM_MAX) == LOW; }
    void clearTrips()      { min_tripped = false; max_tripped = false; }

private:
    static void IRAM_ATTR _isr_min(void* arg) { ((CapLimitSwitches*)arg)->min_tripped = true; }
    static void IRAM_ATTR _isr_max(void* arg) { ((CapLimitSwitches*)arg)->max_tripped = true; }
};


// ─── Capacitance Calculator ───────────────────────────────────────────────────

class CapCalc {
public:
    // Physical parameters (from config.h, overrideable)
    float c_min_pf   = CAP_C_MIN_PF;
    float c_max_pf   = CAP_C_MAX_PF;
    float rot_deg    = CAP_ROTATION_DEG;

    // C(angle) — linear approximation for semicircular plates
    // For actual semi-circular vanes: C(θ) = C_min + (C_max - C_min) × (θ / rot_max)
    // This is accurate to within ±2% for typical designs.
    float angleToC(float angle_deg) const {
        float norm = constrain(angle_deg / rot_deg, 0.0f, 1.0f);
        return c_min_pf + (c_max_pf - c_min_pf) * norm;
    }

    // Invert: given target capacitance, compute required angle
    float cToAngle(float c_pf) const {
        c_pf = constrain(c_pf, c_min_pf, c_max_pf);
        float norm = (c_pf - c_min_pf) / (c_max_pf - c_min_pf);
        return norm * rot_deg;
    }

    // Convert angle to stepper steps from home
    long angleToSteps(float angle_deg) const {
        return (long)(angle_deg / DEG_PER_STEP);
    }

    // Convert steps to angle
    float stepsToAngle(long steps) const {
        return (float)steps * DEG_PER_STEP;
    }

    // Convert capacitance directly to steps
    long cToSteps(float c_pf) const {
        return angleToSteps(cToAngle(c_pf));
    }

    // Convert steps to capacitance
    float stepsToC(long steps) const {
        return angleToC(stepsToAngle(steps));
    }

    // Inductive reactance of capacitor at frequency f (for display)
    // X_C = 1 / (2π × f × C)
    float reactanceOhm(float c_pf, float freq_mhz) const {
        if (c_pf < 0.1f || freq_mhz < 0.1f) return 1e9f;
        return 1.0f / (2.0f * M_PI * freq_mhz * 1e6f * c_pf * 1e-12f);
    }

    // Verify plate design meets capacitance spec (Wheeler/parallel-plate formula)
    // C_calc = ε₀ × A_plate × (N-1) / d
    // Returns calculated C_max in pF
    static float calcPlateCapPF(float plate_dia_mm, int n_plates, float gap_mm) {
        const float eps0 = 8.854e-12f;  // F/m
        float r_m   = (plate_dia_mm / 2.0f) / 1000.0f;  // radius in meters
        float A_m2  = M_PI * r_m * r_m * 0.5f;  // semicircular (180° rotation)
        float d_m   = gap_mm / 1000.0f;
        float C_F   = eps0 * A_m2 * (float)(n_plates - 1) / d_m;
        return C_F * 1e12f;  // Convert to pF
    }

    // Voltage rating from plate spacing (air breakdown 3kV/mm)
    // Safety factor 2× (operating voltage = Vbreakdown / 2)
    static float voltageRatingKV(float gap_mm) {
        return (gap_mm * 3.0f) / 2.0f;   // kV, with 2× safety margin
    }

    // Peak RF voltage across capacitor at power P into reactance X_C
    static float peakVoltsRF(float power_w, float xc_ohm) {
        return sqrtf(2.0f * power_w * xc_ohm);
    }
};


// ─── Capacitor Position Manager ───────────────────────────────────────────────

class CapPositionManager {
public:
    AccelStepper*    stepper;
    PotSensor        pot;
    CapLimitSwitches lim;
    CapCalc          calc;

    CapMotorState state    = CAP_UNCAL;
    long          pos_steps = 0;
    bool          fault    = false;
    bool          homed    = false;

    void begin(AccelStepper* stp, Preferences& prefs) {
        stepper = stp;
        pot.begin();
        lim.begin();

        pinMode(PIN_ENABLE, OUTPUT);
        pinMode(PIN_FAULT,  INPUT_PULLUP);
        digitalWrite(PIN_ENABLE, HIGH);  // Disabled until calibrated

        stepper->setMaxSpeed(MOTOR_SPEED_FAST);
        stepper->setAcceleration(MOTOR_ACCEL);

        pot.load(prefs);
        pos_steps = prefs.getLong(NVS_KEY_STEPS, 0);
        if (pot.calibrated) {
            state = CAP_IDLE;
            homed = true;
            stepper->setCurrentPosition(pos_steps);
        }
        Serial.printf("CapPos: cal=%s pos=%ld steps C=%.1f pF\n",
            pot.calibrated ? "OK" : "NEEDED",
            pos_steps, calc.stepsToC(pos_steps));
    }

    void enableMotor()  { digitalWrite(PIN_ENABLE, LOW);  }
    void disableMotor() { digitalWrite(PIN_ENABLE, HIGH); }
    bool isFault()      { return digitalRead(PIN_FAULT) == LOW; }

    // Move to absolute step position
    void moveTo(long target_steps) {
        if (!homed) { Serial.println("ERR: not calibrated"); return; }
        target_steps = constrain(target_steps, STEPS_MIN, STEPS_MAX);
        enableMotor();
        stepper->setMaxSpeed(MOTOR_SPEED_FAST);
        stepper->setAcceleration(MOTOR_ACCEL);
        stepper->moveTo(target_steps);
        state = CAP_MOVING;
    }

    void moveToC(float c_pf)        { moveTo(calc.cToSteps(c_pf)); }
    void moveRelative(long delta)   { moveTo(pos_steps + delta); }

    // Move to minimum capacitance (fully unmeshed)
    void startHoming() {
        enableMotor();
        state = CAP_HOMING;
        stepper->setMaxSpeed(MOTOR_SPEED_HOME);
        stepper->setAcceleration(MOTOR_ACCEL);
        stepper->setSpeed(-MOTOR_SPEED_HOME);  // Negative = toward min
        Serial.println("HOMING: moving to min capacitance...");
    }

    void updateHoming() {
        if (state != CAP_HOMING) return;
        stepper->runSpeed();

        // Check limit switch or pot at minimum
        if (lim.minActive()) {
            stepper->stop();
            stepper->setCurrentPosition(0);
            pos_steps = 0;
            homed = true;
            state = CAP_IDLE;
            lim.clearTrips();
            Serial.println("HOMED: at min capacitance, step=0");
        }
        if (isFault()) {
            stepper->stop();
            state = CAP_FAULT;
            fault = true;
        }
    }

    // Calibrate using potentiometer (no limit switch required)
    void calibrateFromPot() {
        // Sync step counter to pot reading
        float angle = pot.angleDeg();
        long  cal_steps = calc.angleToSteps(angle);
        stepper->setCurrentPosition(cal_steps);
        pos_steps = cal_steps;
        homed = true;
        state = CAP_IDLE;
        Serial.printf("CAL FROM POT: angle=%.1f° steps=%ld C=%.1f pF\n",
            angle, pos_steps, calc.stepsToC(pos_steps));
    }

    void update() {
        if (state == CAP_HOMING) { updateHoming(); return; }
        if (state != CAP_MOVING) return;

        if (isFault()) {
            stepper->stop();
            state = CAP_FAULT;
            fault = true;
            disableMotor();
            Serial.println("FAULT: DRV8825");
            return;
        }
        // Limit switch protection
        if (lim.minActive() && stepper->distanceToGo() < 0) {
            stepper->stop(); stepper->setCurrentPosition(0);
            pos_steps = 0; state = CAP_IDLE;
            return;
        }
        if (lim.maxActive() && stepper->distanceToGo() > 0) {
            stepper->stop(); state = CAP_IDLE;
            return;
        }

        stepper->run();
        pos_steps = stepper->currentPosition();

        if (stepper->distanceToGo() == 0) {
            state = CAP_IDLE;
            Serial.printf("ARRIVED: C=%.1f pF (%.1f°, %ld steps)\n",
                currentC(), currentAngle(), pos_steps);
        }
    }

    float currentC()     const { return calc.stepsToC(pos_steps); }
    float currentAngle() const { return calc.stepsToAngle(pos_steps); }
    float potAngle()     const { return pot.angleDeg(); }
    float potC()         const { return calc.angleToC(pot.angleDeg()); }
    bool  isMoving()     const { return state == CAP_MOVING || state == CAP_HOMING; }

    // Pot-based position check (verify step counter matches pot)
    float positionError() const {
        return fabsf(currentAngle() - potAngle());
    }

    void savePosition(Preferences& p) {
        p.putLong(NVS_KEY_STEPS, pos_steps);
        pot.save(p);
    }
};
