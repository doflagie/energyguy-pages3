/**
 * config.h — TM-VCAP-ESP32-001 Rev A
 * Variable Capacitor Motorized Controller Configuration
 * ESP32 + DRV8825 + Rotary Pot + Limit Switches
 */

#pragma once

// ─── Firmware ─────────────────────────────────────────────────────────────────
#define FW_VERSION          "1.0.0"
#define DEVICE_NAME         "RF-CapTuner"
#define FW_BUILD_DATE       __DATE__

// ─── Capacitor Type — select ONE ──────────────────────────────────────────────
#define CAP_TYPE_AIR_VARIABLE     // Standard parallel-plate air variable
// #define CAP_TYPE_BUTTERFLY     // Split-stator butterfly (balanced tuner)
// #define CAP_TYPE_VACUUM        // Vacuum variable (linear drive, not rotary)

// ─── Capacitor Physical Parameters ───────────────────────────────────────────
// Capacitance formula: C = ε₀ × A_eff(θ) × (N_plates - 1) / d
// For air variable: C(θ) = C_min + (C_max - C_min) × sin²(θ/2) for semicircular plates
// Approximated as linear: C(θ) ≈ C_min + (C_max - C_min) × (θ / ROTATION_MAX_DEG)

#define CAP_C_MIN_PF        10.0f       // Minimum capacitance (pF), plates fully unmeshed
#define CAP_C_MAX_PF        250.0f      // Maximum capacitance (pF), plates fully meshed
#define CAP_ROTATION_DEG    180.0f      // Full travel rotation (degrees)
#define CAP_PLATE_N         41          // Total plates (20 rotor + 21 stator)
#define CAP_PLATE_DIA_MM    50.0f       // Plate diameter (mm)
#define CAP_PLATE_GAP_MM    2.0f        // Plate spacing (mm)
#define CAP_VOLTAGE_KV      5.0f        // Voltage rating (kVpeak)

// For butterfly (split-stator): each section C_max = half total
#ifdef CAP_TYPE_BUTTERFLY
  #define CAP_C_MAX_PF      125.0f      // Per section (total = 250pF balanced)
  #define CAP_SECTIONS      2
#endif

// For vacuum variable: linear shaft drive, not rotary
#ifdef CAP_TYPE_VACUUM
  #define CAP_C_MIN_PF      10.0f
  #define CAP_C_MAX_PF      1000.0f
  #define CAP_TRAVEL_MM     50.0f       // Linear travel for full range
#endif

// ─── Motion System ────────────────────────────────────────────────────────────
#define GEAR_RATIO          10.0f       // Reduction gear ratio (motor:output)
#define STEPS_PER_REV       200         // Stepper motor full steps/revolution
#define MICROSTEP_DIV       16          // 1/16 microstepping

// Degrees per step at capacitor shaft:
// DEG_PER_STEP = 360 / (STEPS_PER_REV × MICROSTEP_DIV × GEAR_RATIO)
//              = 360 / (200 × 16 × 10) = 0.001125 °/step
#define DEG_PER_STEP        (360.0f / (STEPS_PER_REV * MICROSTEP_DIV * GEAR_RATIO))

// Total steps for full 180° rotation:
// STEPS_FULL_ROTATION = 180 / DEG_PER_STEP = 160,000 steps
#define STEPS_FULL_ROT      ((long)(CAP_ROTATION_DEG / DEG_PER_STEP))

// Soft travel limits (leave 2° margin each end)
#define STEPS_MIN           ((long)(2.0f / DEG_PER_STEP))
#define STEPS_MAX           ((long)((CAP_ROTATION_DEG - 2.0f) / DEG_PER_STEP))

// ─── Motor Speeds ─────────────────────────────────────────────────────────────
#define MOTOR_SPEED_FAST    3000        // Steps/second (coarse move)
#define MOTOR_SPEED_SLOW    800         // Steps/second (fine tune)
#define MOTOR_SPEED_HOME    600         // Steps/second (homing)
#define MOTOR_ACCEL         1200        // Steps/second² (ramp)
#define MOTOR_HOLD_TIMEOUT_MS 3000     // De-energize after 3s idle (capacitor holds position)

// ─── GPIO Pins ────────────────────────────────────────────────────────────────
#define PIN_STEP            25
#define PIN_DIR             26
#define PIN_ENABLE          27          // Active LOW
#define PIN_FAULT           33          // DRV8825 fault (active LOW, open-drain)

#define PIN_POT_ADC         32          // Position potentiometer wiper (ADC1_CH4)
#define PIN_SWR_ADC         36          // SWR feedback (ADC1_CH0) — optional
#define PIN_POWER_ADC       39          // Forward power (ADC1_CH3) — optional

#define PIN_LIM_MIN         34          // Minimum capacitance limit (active LOW)
#define PIN_LIM_MAX         35          // Maximum capacitance limit (active LOW)

#define PIN_BTN_HOME        4           // Manual recalibration button
#define PIN_BTN_C_PLUS      5           // Capacitance step increase
#define PIN_BTN_C_MINUS     16          // Capacitance step decrease
#define PIN_BTN_PRESET      17          // Cycle band presets

#define PIN_STATUS_LED      2
#define PIN_I2C_SDA         21
#define PIN_I2C_SCL         22

// ─── ADC Calibration ──────────────────────────────────────────────────────────
#define ADC_VREF_MV         3300        // ADC reference voltage (mV)
#define ADC_BITS            12          // ESP32 ADC resolution
#define ADC_COUNTS          4096        // 2^12
#define ADC_OVERSAMPLE      16          // Average samples to reduce noise

// Pot calibration: measured counts at min and max rotation
// Calibrate with Procedure 8-1, then update these values
#define POT_COUNT_MIN       150         // ADC count at 0° (min capacitance)
#define POT_COUNT_MAX       3900        // ADC count at 180° (max capacitance)

// ─── Debounce ─────────────────────────────────────────────────────────────────
#define DEBOUNCE_MS         50
#define LIM_DEBOUNCE_MS     5

// ─── Serial / Bluetooth ───────────────────────────────────────────────────────
#define SERIAL_BAUD         115200
#define BT_DEVICE_NAME      "RF-CapTuner"
#define BT_STATUS_INTERVAL_MS  500

// ─── OLED ─────────────────────────────────────────────────────────────────────
#define OLED_ADDR           0x3C
#define OLED_UPDATE_MS      200

// ─── NVS Keys ─────────────────────────────────────────────────────────────────
#define NVS_NAMESPACE       "vcap"
#define NVS_KEY_STEPS       "steps"
#define NVS_KEY_CAL_MIN     "cal_min"
#define NVS_KEY_CAL_MAX     "cal_max"
#define NVS_KEY_CALIBRATED  "cal_ok"

// ─── LED Blink Codes ──────────────────────────────────────────────────────────
#define LED_IDLE_MS         2000
#define LED_MOVING_MS       100
#define LED_HOMING_MS       250
#define LED_ERROR_MS        50
#define LED_UNCAL_MS        500         // Uncalibrated: medium blink

// ─── Status Codes ─────────────────────────────────────────────────────────────
enum CapMotorState {
    CAP_IDLE       = 0,
    CAP_MOVING     = 1,
    CAP_HOMING     = 2,
    CAP_FAULT      = 3,
    CAP_UNCAL      = 4    // Pot not calibrated
};

// ─── Capacitor Design Type ────────────────────────────────────────────────────
// Five designs covering QRP/HF/TX/VHF/UHF  (TM-VCAP-CAL-001)
// Set ACTIVE_CAP_DESIGN to match installed hardware before flashing.
// C_min/C_max in config override these specs when CAP_TYPE_AIR_VARIABLE is set.
enum CapDesignType : uint8_t {
    CAP_DESIGN_QRP   = 0,  // VC-QRP:   8–345 pF,  200V pk, PETG,  r=20mm d=0.5mm, 16R+17S
    CAP_DESIGN_HF100 = 1,  // VC-HF100: 10–225 pF, 1500V pk, Delrin, r=30mm d=1.5mm, 14R+15S
    CAP_DESIGN_TX    = 2,  // VC-TX:    5–84 pF,  4000V pk, Al frame, r=40mm d=4.0mm, 8R+9S
    CAP_DESIGN_VHF   = 3,  // VC-VHF:   2–47 pF,   500V pk, PETG,  r=15mm d=1.0mm, 8R+9S
    CAP_DESIGN_UHF   = 4,  // VC-UHF:  0.5–12 pF,  200V pk, PETG,  r=8mm  d=0.5mm, 4R+5S
};
#define ACTIVE_CAP_DESIGN  CAP_DESIGN_HF100

struct CapDesignSpec {
    CapDesignType design;
    float         c_min_pf;
    float         c_max_pf;
    float         v_working_kv;   // peak working voltage (kV)
    const char*   name;
};

static const CapDesignSpec CAP_DESIGNS[] = {
    { CAP_DESIGN_QRP,    8.0f, 345.0f, 0.200f, "VC-QRP"   },
    { CAP_DESIGN_HF100, 10.0f, 225.0f, 1.500f, "VC-HF100" },
    { CAP_DESIGN_TX,     5.0f,  84.0f, 4.000f, "VC-TX"    },
    { CAP_DESIGN_VHF,    2.0f,  47.0f, 0.500f, "VC-VHF"   },
    { CAP_DESIGN_UHF,    0.5f,  12.0f, 0.200f, "VC-UHF"   },
};
#define N_CAP_DESIGNS 5

// Active design voltage rating (used in voltage warning display)
#define ACTIVE_CAP_V_KV  (CAP_DESIGNS[ACTIVE_CAP_DESIGN].v_working_kv)

// ─── Drive Mode ───────────────────────────────────────────────────────────────
// Rotary mode: angle → C mapping (standard air variable, gear-driven).
// Linear mode: insertion depth → C mapping (PVC pipe plunger, vacuum variable).
#define CAP_DRIVE_MODE_ROTARY
// #define CAP_DRIVE_MODE_LINEAR

#ifdef CAP_DRIVE_MODE_LINEAR
  // PVC Pipe Sliding Capacitor (TM-VCAP-CAL-005, TM-VCAP-SCH-004)
  // Uncomment one design; comment the others.
  #define PIPE_DESIGN         "PIPE-HF"
  #define PIPE_C_PER_MM       17.7f     // pF/mm — PIPE-HF (1/2" PVC, Kapton 0.13mm)
  #define PIPE_C_MIN_PF        3.0f     // Stray C at x=0 (no insertion)
  #define PIPE_MAX_DEPTH_MM  150.0f     // Maximum insertion depth (mm)
  // #define PIPE_C_PER_MM    4.9f      // PIPE-HF-HV (PTFE 8-layer, 0.61mm)
  // #define PIPE_MAX_DEPTH_MM 200.0f
  // #define PIPE_C_PER_MM    22.9f     // PIPE-VHF  (3/8" PVC, Kapton 0.065mm)
  // #define PIPE_MAX_DEPTH_MM  20.0f
  // #define PIPE_C_PER_MM    21.7f     // PIPE-QRP  (flexible vinyl, OHP 0.10mm)
  // #define PIPE_MAX_DEPTH_MM  50.0f

  // Lead-screw: M6×1.0mm, NEMA14, 1/8 µstep (1600 µsteps/rev) → 0.000625 mm/µstep
  // ΔC per µstep = MM_PER_MICROSTEP × PIPE_C_PER_MM ≈ 0.011 pF/µstep (PIPE-HF)
  #define MM_PER_MICROSTEP    0.000625f

  #define NVS_KEY_PIPE_MM     "pipe_mm"   // Current position (mm) in NVS
  #define NVS_KEY_PIPE_K      "pipe_k"    // Calibrated K pF/mm (SETC PIPE_K <val>)
#endif

// ─── Stack-On Modular Units (CSM series, TM-VCAP-CAL-006) ────────────────────
// Stack-on units supply coarse C via manually added/removed foil+polymer slabs.
// Firmware tracks CSM_BASE_PF as an offset to fine-tuner C; set with SETCSM <pF>.
#define CSM_ENABLE
#define CSM_BASE_PF_DEFAULT  0.0f
#define NVS_KEY_CSM_BASE    "csm_base"

static const float CSM_UNIT_C[]  = { 248.0f, 159.0f, 100.0f, 100.0f, 54.0f, 31.0f, 18.0f, 10.0f };
static const char* CSM_UNIT_ID[] = { "A2",   "B2",   "C2",   "D",    "E",   "F",   "G",   "VHF" };
#define N_CSM_UNITS 8
