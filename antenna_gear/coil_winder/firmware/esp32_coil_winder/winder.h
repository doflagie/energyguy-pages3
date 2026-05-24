#pragma once
/**
 * winder.h — TM-CWND-ESP32-001 Rev A
 * Winding mathematics, state machine, traverse control
 */

#include <AccelStepper.h>
#include "config.h"

// ─── Winder State ─────────────────────────────────────────────────────────────

enum WinderState {
    WINDER_IDLE    = 0,
    WINDER_WINDING = 1,
    WINDER_PAUSED  = 2,
    WINDER_HOMING  = 3,
    WINDER_FAULT   = 4,
    WINDER_DONE    = 5,
};

enum WinderFault {
    FAULT_NONE       = 0,
    FAULT_WIRE_BREAK = 1,
    FAULT_OVERRUN    = 2,
    FAULT_TRAV_LIMIT = 3,
    FAULT_ESTOP      = 4,
};

// ─── Wire Size Table ──────────────────────────────────────────────────────────

struct WireSpec {
    uint8_t  awg;
    float    dia_mm;      // Bare conductor diameter (nominal)
    float    dia_ins_mm;  // Insulated diameter (magnet wire single-build)
    float    ohm_per_m;   // DC resistance at 20°C
    float    max_a;       // Maximum current (chassis wiring)
};

// AWG 10–30 data (magnet wire single-build insulation adds ~0.05–0.08mm)
static const WireSpec WIRE_TABLE[] = {
//  AWG  bare_mm  ins_mm   Ω/m      A_max
    {10,  2.588,  2.65f,  0.00328f, 30.0f},
    {11,  2.305,  2.37f,  0.00413f, 25.0f},
    {12,  2.053,  2.11f,  0.00521f, 20.0f},
    {13,  1.828,  1.89f,  0.00657f, 17.0f},
    {14,  1.628,  1.69f,  0.00828f, 13.0f},
    {15,  1.450,  1.51f,  0.01044f, 11.0f},
    {16,  1.291,  1.35f,  0.01316f,  8.0f},
    {17,  1.150,  1.21f,  0.01659f,  6.5f},
    {18,  1.024,  1.08f,  0.02090f,  5.0f},
    {19,  0.912,  0.97f,  0.02634f,  4.0f},
    {20,  0.812,  0.87f,  0.03320f,  3.0f},
    {21,  0.723,  0.78f,  0.04186f,  2.5f},
    {22,  0.644,  0.70f,  0.05279f,  2.0f},
    {23,  0.573,  0.63f,  0.06657f,  1.6f},
    {24,  0.511,  0.57f,  0.08394f,  1.2f},
    {25,  0.455,  0.51f,  0.10583f,  1.0f},
    {26,  0.405,  0.46f,  0.13341f,  0.8f},
    {27,  0.361,  0.41f,  0.16819f,  0.7f},
    {28,  0.321,  0.37f,  0.21200f,  0.5f},
    {29,  0.286,  0.33f,  0.26735f,  0.4f},
    {30,  0.255,  0.30f,  0.33680f,  0.3f},
};
static const int N_WIRE_SPECS = sizeof(WIRE_TABLE) / sizeof(WIRE_TABLE[0]);

const WireSpec* getWireSpec(uint8_t awg) {
    for (int i = 0; i < N_WIRE_SPECS; i++)
        if (WIRE_TABLE[i].awg == awg) return &WIRE_TABLE[i];
    return &WIRE_TABLE[8];  // default AWG 18
}

// ─── Winding Program ──────────────────────────────────────────────────────────

struct WindingProgram {
    // Target geometry
    uint32_t target_turns;     // Total turns to wind
    float    target_uh;        // Target inductance (0 = turns-based, not L-based)
    float    former_dia_mm;    // Coil former outer diameter
    float    coil_len_mm;      // Desired winding length (0 = auto from turns+wire)
    uint8_t  awg;              // Wire gauge
    float    pitch_factor;     // 1.0 = close-wound, >1 = spaced
    uint8_t  layers;           // Number of layers (1 = single-layer)
    bool     reverse_on_layer; // Auto-reverse traverse at layer end
    char     name[24];         // Program name for NVS storage

    // Derived (calculated)
    float    wire_dia_mm;      // Insulated wire diameter
    float    turns_per_layer;  // For multi-layer winding
    float    total_wire_m;     // Estimated wire length
    float    dc_resistance;    // Estimated winding resistance
    float    calc_uh;          // Wheeler-calculated inductance

    void calculate() {
        const WireSpec* ws = getWireSpec(awg);
        wire_dia_mm = ws->dia_ins_mm * pitch_factor;

        if (coil_len_mm > 0 && target_turns == 0) {
            // Length-based: calculate turns from length
            turns_per_layer = coil_len_mm / wire_dia_mm;
        } else if (target_turns > 0) {
            // Turns-based: calculate minimum winding length
            turns_per_layer = (layers > 1) ?
                ceilf((float)target_turns / layers) : target_turns;
            if (coil_len_mm == 0)
                coil_len_mm = turns_per_layer * wire_dia_mm;
        }
        total_wire_m = (float)target_turns * M_PI * (former_dia_mm + wire_dia_mm) / 1000.0f;
        dc_resistance = total_wire_m * ws->ohm_per_m;
    }
};

// ─── Wheeler's Formula ────────────────────────────────────────────────────────

class WinderMath {
public:
    // Single-layer coil inductance (Wheeler's formula)
    // d = coil diameter (inches), N = turns, l = winding length (inches)
    // Returns inductance in µH
    static float wheelerSingleLayer(float d_in, float N, float l_in) {
        if (l_in < 0.001f || d_in < 0.001f || N < 0.5f) return 0;
        return (d_in * d_in * N * N) / (18.0f * d_in + 40.0f * l_in);
    }

    // Multi-layer coil inductance (Wheeler multi-layer formula)
    // a = mean radius (inches), l = length (inches), c = radial build (inches)
    // N = total turns
    static float wheelerMultiLayer(float a_in, float l_in, float c_in, float N) {
        if (l_in < 0.001f || a_in < 0.001f) return 0;
        return (0.8f * a_in * a_in * N * N) / (6.0f * a_in + 9.0f * l_in + 10.0f * c_in);
    }

    // Calculate inductance for a WindingProgram
    static float calcInductance(const WindingProgram& p) {
        float d_in = p.former_dia_mm / 25.4f;
        float l_in = p.coil_len_mm / 25.4f;
        float N    = (float)p.target_turns;

        if (p.layers == 1) {
            return wheelerSingleLayer(d_in, N, l_in);
        } else {
            float build_in = (p.layers * p.wire_dia_mm) / 25.4f;
            float a_in = (p.former_dia_mm / 2.0f + build_in / 2.0f) / 25.4f;
            return wheelerMultiLayer(a_in, l_in, build_in, N);
        }
    }

    // Solve for turns to achieve target inductance (Newton-Raphson)
    // single-layer only, close-wound
    static uint32_t turnsForInductance(float target_uh, float d_mm, float l_mm) {
        float d_in = d_mm / 25.4f;
        float l_in = l_mm / 25.4f;
        float N = sqrtf(target_uh * (18.0f * d_in + 40.0f * l_in) / (d_in * d_in));
        // One Newton-Raphson refinement:
        for (int i = 0; i < 10; i++) {
            float f  = wheelerSingleLayer(d_in, N, l_in) - target_uh;
            float fp = 2.0f * N * d_in * d_in / (18.0f * d_in + 40.0f * l_in);
            if (fabsf(fp) < 1e-9f) break;
            N -= f / fp;
            if (N < 0.5f) N = 0.5f;
        }
        return (uint32_t)ceilf(N);
    }

    // Turns from frequency and inductance (for resonant circuit):
    // X_L = 2π × f × L, Z = √(L/C)
    // Given f(MHz) and desired L(µH):
    static float requiredLforFreq(float f_mhz, float c_pf) {
        float w = 2.0f * M_PI * f_mhz * 1e6f;
        return 1.0f / (w * w * c_pf * 1e-12f) * 1e6f;  // µH
    }

    // Self-resonant frequency estimate
    static float selfResonantFreq(float L_uh, float C_stray_pf) {
        return 1.0f / (2.0f * M_PI * sqrtf(L_uh * 1e-6f * C_stray_pf * 1e-12f)) / 1e6f; // MHz
    }

    // Q factor estimate (reasonable for air-core RF coil)
    // f in MHz, L in µH, AWG
    static float qFactor(float f_mhz, float L_uh, uint8_t awg, float wire_len_m) {
        const WireSpec* ws = getWireSpec(awg);
        float Xl = 2.0f * M_PI * f_mhz * 1e6f * L_uh * 1e-6f;
        float skin_factor = sqrtf(f_mhz);  // approximate skin effect
        float Rdc = wire_len_m * ws->ohm_per_m;
        float Rac = Rdc * skin_factor;
        return (Rac > 0.001f) ? Xl / Rac : 999;
    }

    // Traverse advance per spindle revolution
    // Returns: traverse steps to advance for each complete spindle revolution
    static long traverseStepsPerRev(float wire_dia_mm, float pitch_factor) {
        float advance_mm = wire_dia_mm * pitch_factor;
        return (long)roundf(advance_mm * TRAV_STEPS_PER_MM);
    }
};

// ─── Turn Counter ─────────────────────────────────────────────────────────────

class TurnCounter {
public:
    volatile uint32_t pulse_count = 0;  // ISR increments this
    uint32_t          last_count  = 0;
    float             turns       = 0.0f;
    float             rpm         = 0.0f;
    uint32_t          last_rpm_ms = 0;

    void reset() {
        pulse_count = 0;
        last_count  = 0;
        turns       = 0.0f;
        rpm         = 0.0f;
    }

    void update() {
        turns = (float)pulse_count * TURN_FRAC_SCALE;

        // RPM estimation (over last 500ms)
        if (millis() - last_rpm_ms >= 500) {
            uint32_t delta = pulse_count - last_count;
            float dt_min = (millis() - last_rpm_ms) / 60000.0f;
            rpm = (delta / (float)TURN_PULSES_PER_REV) / dt_min;
            last_count  = pulse_count;
            last_rpm_ms = millis();
        }
    }

    bool targetReached(uint32_t target) const {
        return (uint32_t)(turns + 0.5f) >= target;
    }
};

// ─── Traverse Controller ──────────────────────────────────────────────────────

class TraverseController {
public:
    AccelStepper* stepper = nullptr;
    float         pos_mm  = 0;           // Current position in mm
    float         start_mm = 0;          // Layer start position
    float         end_mm  = 0;           // Layer end position
    int           layer   = 0;           // Current layer (0-indexed)
    bool          homed   = false;
    bool          reversing = false;

    void begin(AccelStepper* s) { stepper = s; }

    void setLayer(int layer_idx, float from_mm, float to_mm) {
        layer    = layer_idx;
        start_mm = from_mm;
        end_mm   = to_mm;
        pos_mm   = from_mm;
        reversing = (layer_idx % 2 != 0);  // Even layers go forward, odd reverse
        if (reversing) {
            pos_mm = end_mm;
            stepper->moveTo(mmToSteps(end_mm));
        } else {
            pos_mm = start_mm;
            stepper->moveTo(mmToSteps(start_mm));
        }
    }

    // Advance traverse by exactly one wire diameter × pitch_factor
    // Called once per spindle revolution
    void advanceOneWire(float wire_dia_mm, float pitch_factor) {
        long steps = WinderMath::traverseStepsPerRev(wire_dia_mm, pitch_factor);
        if (!reversing) {
            stepper->move(steps);
            pos_mm += wire_dia_mm * pitch_factor;
        } else {
            stepper->move(-steps);
            pos_mm -= wire_dia_mm * pitch_factor;
        }
    }

    bool atLayerEnd() const {
        if (!reversing) return pos_mm >= end_mm;
        else            return pos_mm <= start_mm;
    }

    void startHoming() {
        homed = false;
        stepper->moveTo(-(long)(TRAV_MAX_MM * TRAV_STEPS_PER_MM));
        stepper->setMaxSpeed(TRAV_HOMING_SPEED);
    }

    void onHomeLimit() {
        stepper->stop();
        stepper->setCurrentPosition(0);
        pos_mm = 0;
        homed = true;
        stepper->setMaxSpeed(TRAV_MAX_SPEED);
    }

private:
    long mmToSteps(float mm) const {
        return (long)(mm * TRAV_STEPS_PER_MM);
    }
};

// ─── Winder State Machine ─────────────────────────────────────────────────────

class WinderController {
public:
    WinderState    state   = WINDER_IDLE;
    WinderFault    fault   = FAULT_NONE;
    WindingProgram program;
    TurnCounter    turns;
    TraverseController traverse;

    uint32_t  layer_start_turn = 0;  // Turn count at start of current layer
    float     turns_this_layer = 0;
    uint32_t  last_rev_count   = 0;  // Tracks completed revolutions for traverse advance

    AccelStepper* spindle  = nullptr;
    AccelStepper* travStepper = nullptr;

    void begin(AccelStepper* spin, AccelStepper* trav) {
        spindle   = spin;
        travStepper = trav;
        traverse.begin(trav);
    }

    void startWinding() {
        if (state != WINDER_IDLE && state != WINDER_DONE) return;
        if (!traverse.homed) { startHoming(); return; }

        program.calculate();
        state = WINDER_WINDING;
        fault = FAULT_NONE;
        turns.reset();
        last_rev_count = 0;
        layer_start_turn = 0;
        traverse.setLayer(0, 0, program.coil_len_mm);

        float sps = RPM_TO_SPS(program.target_turns > 0 ?
            DEFAULT_RPM : DEFAULT_RPM);
        spindle->setMaxSpeed(sps);
        spindle->setAcceleration(SPIN_ACCEL);
        spindle->move((long)program.target_turns * SPIN_STEPS_PER_ROT * 2);
    }

    void pauseWinding() {
        if (state == WINDER_WINDING) {
            spindle->stop();
            travStepper->stop();
            state = WINDER_PAUSED;
        }
    }

    void resumeWinding() {
        if (state == WINDER_PAUSED) {
            state = WINDER_WINDING;
            long remaining = ((long)program.target_turns - (long)turns.turns)
                             * SPIN_STEPS_PER_ROT;
            spindle->move(remaining);
        }
    }

    void stopWinding() {
        spindle->stop();
        travStepper->stop();
        state = WINDER_IDLE;
    }

    void setFault(WinderFault f) {
        spindle->stop();
        travStepper->stop();
        fault = f;
        state = WINDER_FAULT;
    }

    void clearFault() {
        if (state == WINDER_FAULT) {
            fault = FAULT_NONE;
            state = WINDER_IDLE;
        }
    }

    void startHoming() {
        state = WINDER_HOMING;
        traverse.startHoming();
    }

    void setSpeed(float rpm) {
        float sps = RPM_TO_SPS(constrain(rpm, MIN_RPM, MAX_RPM));
        spindle->setMaxSpeed(sps);
    }

    // Call once per spindle revolution (from ISR context via flag)
    void onRevComplete() {
        if (state != WINDER_WINDING) return;
        turns.update();
        turns_this_layer = turns.turns - layer_start_turn;

        // Advance traverse
        traverse.advanceOneWire(program.wire_dia_mm, program.pitch_factor);

        // Check layer end
        if (program.layers > 1 && traverse.atLayerEnd()) {
            int next_layer = traverse.layer + 1;
            if (next_layer >= program.layers) {
                // Done with all layers
                finishWinding();
                return;
            }
            layer_start_turn = (uint32_t)turns.turns;
            float layer_thick = program.wire_dia_mm * next_layer;
            traverse.setLayer(next_layer,
                (next_layer % 2 == 0) ? 0 : program.coil_len_mm,
                (next_layer % 2 == 0) ? program.coil_len_mm : 0);
        }

        // Check turn target
        if (program.target_turns > 0 && turns.targetReached(program.target_turns)) {
            finishWinding();
        }
    }

    void finishWinding() {
        spindle->stop();
        travStepper->stop();
        state = WINDER_DONE;
        // Calculate actual inductance
        program.target_turns = (uint32_t)turns.turns;
        program.calc_uh = WinderMath::calcInductance(program);
    }

    bool isRunning() const { return state == WINDER_WINDING; }
    bool isFault()   const { return state == WINDER_FAULT; }
    bool isDone()    const { return state == WINDER_DONE; }
};
