/**
 * position.h — TM-ROLLER-ESP32-001 Rev A
 * Roller Inductor Position Sensing, Inductance Calculation, AccelStepper Wrapper
 */

#pragma once
#include <Arduino.h>
#include <AccelStepper.h>
#include <Preferences.h>
#include "config.h"

// ─── Encoder (ISR-driven) ─────────────────────────────────────────────────────

class QuadratureEncoder {
public:
    volatile long count = 0;

    void begin() {
        pinMode(PIN_ENC_A, INPUT_PULLUP);
        pinMode(PIN_ENC_B, INPUT_PULLUP);
        attachInterruptArg(digitalPinToInterrupt(PIN_ENC_A), _isr_a, this, CHANGE);
        attachInterruptArg(digitalPinToInterrupt(PIN_ENC_B), _isr_b, this, CHANGE);
    }

    long getCount() const { return count; }
    void reset() { count = 0; }

    // Convert encoder pulses to mm of travel
    float toMM() const { return (float)count / ENCODER_PULSES_PER_MM; }

private:
    static void IRAM_ATTR _isr_a(void* arg) {
        QuadratureEncoder* enc = (QuadratureEncoder*)arg;
        bool a = digitalRead(PIN_ENC_A);
        bool b = digitalRead(PIN_ENC_B);
        enc->count += (a == b) ? +1 : -1;
    }
    static void IRAM_ATTR _isr_b(void* arg) {
        QuadratureEncoder* enc = (QuadratureEncoder*)arg;
        bool a = digitalRead(PIN_ENC_A);
        bool b = digitalRead(PIN_ENC_B);
        enc->count += (a != b) ? +1 : -1;
    }
};


// ─── Limit Switch State ────────────────────────────────────────────────────────

class LimitSwitches {
public:
    bool home_tripped = false;
    bool max_tripped  = false;

    void begin() {
        pinMode(PIN_LIM_HOME, INPUT_PULLUP);
        pinMode(PIN_LIM_MAX,  INPUT_PULLUP);
        attachInterruptArg(digitalPinToInterrupt(PIN_LIM_HOME), _isr_home, this, FALLING);
        attachInterruptArg(digitalPinToInterrupt(PIN_LIM_MAX),  _isr_max,  this, FALLING);
    }

    bool homeActive() const { return digitalRead(PIN_LIM_HOME) == LOW; }
    bool maxActive()  const { return digitalRead(PIN_LIM_MAX)  == LOW; }

    void clearTrips() { home_tripped = false; max_tripped = false; }

private:
    static void IRAM_ATTR _isr_home(void* arg) {
        ((LimitSwitches*)arg)->home_tripped = true;
    }
    static void IRAM_ATTR _isr_max(void* arg) {
        ((LimitSwitches*)arg)->max_tripped = true;
    }
};


// ─── Wheeler Inductance Calculator ────────────────────────────────────────────

class WheelerCalc {
public:
    float coil_d_in;    // Coil outer diameter, inches
    float pitch_mm;     // Turn pitch (mm)
    int   total_turns;  // Total turns wound

    WheelerCalc() :
        coil_d_in(COIL_DIAMETER_IN),
        pitch_mm(COIL_PITCH_MM),
        total_turns(COIL_TURNS_TOTAL) {}

    // Calculate inductance for a given number of active turns
    // active_turns: number of turns between roller and fixed terminal
    // Returns: inductance in µH
    float calcL(float active_turns) const {
        if (active_turns < 0.5f) return 0.0f;
        float d = coil_d_in;
        float n = active_turns;
        float l_in = (n * pitch_mm) / 25.4f;   // winding length in inches
        if (l_in < 0.001f) return 0.0f;
        return (d * d * n * n) / (18.0f * d + 40.0f * l_in);
    }

    // Given a target inductance, solve for required turns (Newton-Raphson)
    // Returns fractional turns (for sub-turn positioning)
    float turnsForL(float target_uh) const {
        if (target_uh <= 0.0f) return 0.0f;
        if (target_uh >= calcL(total_turns)) return (float)total_turns;
        float n = (float)total_turns / 2.0f;   // initial guess
        for (int i = 0; i < 50; i++) {
            float f  = calcL(n) - target_uh;
            float df = dLdN(n);
            if (fabsf(df) < 1e-6f) break;
            float n_new = n - f / df;
            if (n_new < 0.1f) n_new = 0.1f;
            if (n_new > total_turns) n_new = (float)total_turns;
            if (fabsf(n_new - n) < 0.001f) { n = n_new; break; }
            n = n_new;
        }
        return n;
    }

    // Convert active turns to roller carriage position (mm from home)
    float turnsToMM(float turns) const {
        return COIL_WIND_START_MM + turns * pitch_mm;
    }

    // Convert carriage position to active turns
    float mmToTurns(float pos_mm) const {
        float active_mm = pos_mm - COIL_WIND_START_MM;
        return (active_mm > 0.0f) ? (active_mm / pitch_mm) : 0.0f;
    }

    // Convert mm to steps
    long mmToSteps(float mm) const {
        return (long)(mm * STEPS_PER_MM);
    }

    // Convert steps to mm
    float stepsToMM(long steps) const {
        return (float)steps / STEPS_PER_MM;
    }

    // Convert steps to inductance
    float stepsToL(long steps) const {
        float mm = stepsToMM(steps);
        float turns = mmToTurns(mm);
        return calcL(turns);
    }

    // Convert target inductance to stepper position
    long lToSteps(float target_uh) const {
        float turns = turnsForL(target_uh);
        float mm    = turnsToMM(turns);
        return mmToSteps(mm);
    }

private:
    // Numerical derivative dL/dN
    float dLdN(float n) const {
        float h = 0.01f;
        return (calcL(n + h) - calcL(n - h)) / (2.0f * h);
    }
};


// ─── Position Manager ─────────────────────────────────────────────────────────

class PositionManager {
public:
    AccelStepper*    stepper;
    QuadratureEncoder enc;
    LimitSwitches    lim;
    WheelerCalc      calc;

    MotorState state     = MOTOR_NOT_HOMED;
    HomeState  home_state = HOME_UNKNOWN;
    long       position_steps = 0;   // Current stepper position (steps from home)
    bool       fault     = false;

    void begin(AccelStepper* stp) {
        stepper = stp;
        enc.begin();
        lim.begin();

        pinMode(PIN_ENABLE, OUTPUT);
        pinMode(PIN_FAULT,  INPUT_PULLUP);
        digitalWrite(PIN_ENABLE, HIGH);   // Motor disabled until homed

        stepper->setMaxSpeed(MOTOR_SPEED_FAST);
        stepper->setAcceleration(MOTOR_ACCEL);
    }

    void enableMotor()  { digitalWrite(PIN_ENABLE, LOW);  }
    void disableMotor() { digitalWrite(PIN_ENABLE, HIGH); }

    bool isFault() const { return digitalRead(PIN_FAULT) == LOW; }

    // HOME sequence: move toward home limit switch at slow speed
    // Returns true when complete
    bool startHoming() {
        if (state == MOTOR_HOMING) return false;
        enableMotor();
        state = MOTOR_HOMING;
        home_state = HOME_UNKNOWN;
        stepper->setSpeed(-MOTOR_SPEED_HOME);   // Negative = toward home
        stepper->setMaxSpeed(MOTOR_SPEED_HOME);
        Serial.println("HOMING: moving to home limit...");
        return true;
    }

    // Call from main loop during homing
    void updateHoming() {
        if (state != MOTOR_HOMING) return;
        stepper->runSpeed();
        if (lim.homeActive()) {
            stepper->stop();
            stepper->setCurrentPosition(0);
            position_steps = 0;
            enc.reset();
            state = MOTOR_IDLE;
            home_state = HOME_COMPLETE;
            lim.clearTrips();
            Serial.println("HOMING: complete, position=0");
        }
        if (isFault()) {
            stepper->stop();
            state = MOTOR_FAULT;
            fault = true;
            Serial.println("HOMING: DRV8825 fault!");
        }
    }

    // Move to absolute step position
    void moveTo(long target_steps) {
        if (home_state != HOME_COMPLETE) {
            Serial.println("ERR: not homed");
            return;
        }
        if (target_steps < 0) target_steps = 0;
        if (target_steps > TRAVEL_MAX_STEPS) target_steps = TRAVEL_MAX_STEPS;

        enableMotor();
        stepper->setMaxSpeed(MOTOR_SPEED_FAST);
        stepper->setAcceleration(MOTOR_ACCEL);
        stepper->moveTo(target_steps);
        state = MOTOR_MOVING;
        Serial.printf("MOVE: target=%ld steps (%.2f mm, %.2f µH)\n",
            target_steps,
            calc.stepsToMM(target_steps),
            calc.stepsToL(target_steps));
    }

    // Move to inductance value
    void moveToL(float target_uh) {
        long steps = calc.lToSteps(target_uh);
        moveTo(steps);
    }

    // Move by relative steps
    void moveRelative(long delta_steps) {
        long target = position_steps + delta_steps;
        moveTo(target);
    }

    // Call every loop iteration
    void update() {
        if (state == MOTOR_HOMING) { updateHoming(); return; }
        if (state != MOTOR_MOVING) return;

        if (isFault()) {
            stepper->stop();
            state = MOTOR_FAULT;
            fault = true;
            disableMotor();
            Serial.println("FAULT: DRV8825 over-temp or overcurrent");
            return;
        }

        if (lim.homeActive() && stepper->distanceToGo() < 0) {
            stepper->stop();
            stepper->setCurrentPosition(0);
            position_steps = 0;
            state = MOTOR_IDLE;
            Serial.println("LIMIT: home switch hit during move");
            return;
        }
        if (lim.maxActive() && stepper->distanceToGo() > 0) {
            stepper->stop();
            state = MOTOR_IDLE;
            Serial.println("LIMIT: max switch hit during move");
            return;
        }

        stepper->run();
        position_steps = stepper->currentPosition();

        if (stepper->distanceToGo() == 0) {
            state = MOTOR_IDLE;
            Serial.printf("ARRIVED: pos=%ld steps, L=%.2f µH\n",
                position_steps, calc.stepsToL(position_steps));
        }
    }

    float currentL()      const { return calc.stepsToL(position_steps); }
    float currentMM()     const { return calc.stepsToMM(position_steps); }
    float currentTurns()  const { return calc.mmToTurns(currentMM()); }
    bool  isMoving()      const { return state == MOTOR_MOVING || state == MOTOR_HOMING; }
    bool  isHomed()       const { return home_state == HOME_COMPLETE; }

    void savePosition(Preferences& prefs) {
        prefs.putLong(NVS_KEY_POSITION, position_steps);
        prefs.putBool(NVS_KEY_HOMED, home_state == HOME_COMPLETE);
    }

    void loadPosition(Preferences& prefs) {
        position_steps = prefs.getLong(NVS_KEY_POSITION, 0);
        bool was_homed = prefs.getBool(NVS_KEY_HOMED, false);
        if (was_homed) {
            stepper->setCurrentPosition(position_steps);
            home_state = HOME_COMPLETE;
            state = MOTOR_IDLE;
        }
    }
};
