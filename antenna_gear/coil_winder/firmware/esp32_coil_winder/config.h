#pragma once
/**
 * config.h — TM-CWND-ESP32-001 Rev A
 * Coil Winder Controller Configuration
 */

// ─── Firmware Identity ────────────────────────────────────────────────────────
#define FW_VERSION      "1.0.0"
#define FW_BUILD_DATE   __DATE__
#define DEVICE_NAME     "RF-CoilWinder"
#define NVS_NAMESPACE   "coil_winder"

// ─── Spindle Stepper ──────────────────────────────────────────────────────────
#define PIN_SPIN_STEP       18
#define PIN_SPIN_DIR        19
#define PIN_SPIN_EN         5
#define PIN_SPIN_MS1        23
#define PIN_SPIN_MS2        22

#define SPIN_STEPS_PER_REV  200       // NEMA 23 full steps
#define SPIN_MICROSTEP      16        // 1/16 microstepping
#define SPIN_STEPS_PER_ROT  (SPIN_STEPS_PER_REV * SPIN_MICROSTEP)  // 3200
#define SPIN_MAX_SPEED      8000.0f   // steps/sec (~150 RPM at 1/16)
#define SPIN_MIN_SPEED      100.0f    // steps/sec (~1.9 RPM)
#define SPIN_ACCEL          1200.0f   // steps/sec²

// RPM ↔ steps/sec: speed_sps = RPM × SPIN_STEPS_PER_ROT / 60
#define RPM_TO_SPS(rpm)     ((rpm) * SPIN_STEPS_PER_ROT / 60.0f)
#define SPS_TO_RPM(sps)     ((sps) * 60.0f / SPIN_STEPS_PER_ROT)

// ─── Traverse Stepper ─────────────────────────────────────────────────────────
#define PIN_TRAV_STEP       25
#define PIN_TRAV_DIR        26
#define PIN_TRAV_EN         27
#define PIN_TRAV_MS1        14
#define PIN_TRAV_MS2        12

#define TRAV_STEPS_PER_REV  200       // NEMA 17 full steps
#define TRAV_MICROSTEP      16        // 1/16 microstepping
#define TRAV_LEADSCREW_MM   2.0f      // T8 leadscrew pitch (mm/rev)
// Steps per mm of linear travel:
#define TRAV_STEPS_PER_MM   ((TRAV_STEPS_PER_REV * TRAV_MICROSTEP) / TRAV_LEADSCREW_MM)
// = (200 × 16) / 2.0 = 1600 steps/mm
#define TRAV_MAX_MM         310.0f    // Maximum traverse travel (12" + margin)
#define TRAV_MAX_SPEED      5000.0f   // steps/sec
#define TRAV_ACCEL          2000.0f   // steps/sec²
#define TRAV_HOMING_SPEED   300.0f    // slow for homing

// ─── Traverse — Wire Pitch Calculation ───────────────────────────────────────
// For a given wire diameter, spindle must advance traverse by exactly
// one wire diameter per revolution (or per N revolutions for spaced winding).
// Traverse steps per spindle revolution:
//   trav_steps = wire_dia_mm × TRAV_STEPS_PER_MM × pitch_factor
//
// pitch_factor: 1.0 = close-wound, >1.0 = spaced winding

// ─── I/O Pins ─────────────────────────────────────────────────────────────────
#define PIN_TURN_SENSOR     35    // Optical/Hall sensor, input-only, interrupt
#define PIN_WIRE_BREAK      13    // Wire break optical gate, active LOW
#define PIN_FOOT_PEDAL      15    // Foot pedal, active LOW
#define PIN_TRAV_LIMIT_MIN  39    // Traverse home limit, input-only, active LOW
#define PIN_SPEED_POT       36    // ADC: speed potentiometer
#define PIN_ENC_A           32    // Rotary encoder A
#define PIN_ENC_B           33    // Rotary encoder B
#define PIN_ENC_BTN         34    // Encoder button, input-only
#define PIN_STATUS_LED      2
#define PIN_I2C_SDA         21
#define PIN_I2C_SCL         22
// SD card (HSPI bus)
#define PIN_SD_CS           4
#define PIN_SD_CLK          14    // Shared with TRAV_MS1 — resolve in hardware
#define PIN_SD_MOSI         13
#define PIN_SD_MISO         12

// ─── Display ──────────────────────────────────────────────────────────────────
#define OLED_ADDR       0x3C
#define OLED_UPDATE_MS  150

// ─── Bluetooth ────────────────────────────────────────────────────────────────
#define BT_DEVICE_NAME     "RF-CoilWinder"
#define BT_STATUS_INTERVAL_MS  500

// ─── Turn Counter ─────────────────────────────────────────────────────────────
// Number of magnets/flags on spindle shaft (1 = one pulse per rev)
#define TURN_PULSES_PER_REV   1
// Fractional turn tracking: multiply counts by this to get fractional turns
#define TURN_FRAC_SCALE       (1.0f / TURN_PULSES_PER_REV)

// ─── Motor Hold ───────────────────────────────────────────────────────────────
// Spindle and traverse both use non-self-locking drives
// Motors must remain energized to hold position
// De-energize after this delay when truly idle (manual override)
#define MOTOR_HOLD_TIMEOUT_MS  10000
// For auto-wind: never de-energize during active wind sequence

// ─── Speed and Winding Limits ─────────────────────────────────────────────────
#define MIN_RPM         0.5f     // Minimum spindle RPM (hand-wind mode)
#define MAX_RPM         150.0f   // Maximum spindle RPM (fine wire)
#define DEFAULT_RPM     30.0f    // Default speed
// Heavy wire (AWG 10–14): max ~20 RPM
// Medium wire (AWG 15–22): max ~60 RPM
// Fine wire (AWG 23–30): max ~120 RPM
#define RPM_HEAVY       20.0f
#define RPM_MEDIUM      60.0f
#define RPM_FINE        120.0f

// ─── Wire Break Detection ─────────────────────────────────────────────────────
#define WIRE_BREAK_DEBOUNCE_MS  200   // Sustained LOW before fault declared

// ─── Safety ───────────────────────────────────────────────────────────────────
#define MAX_TURNS       10000     // Software limit — prompt if exceeded
#define MAX_LAYERS      20        // Software layer limit

// ─── Serial ───────────────────────────────────────────────────────────────────
#define SERIAL_BAUD     115200

// ─── Debounce ─────────────────────────────────────────────────────────────────
#define DEBOUNCE_MS     25

// ─── LED blink periods ────────────────────────────────────────────────────────
#define LED_IDLE_MS     2000
#define LED_WINDING_MS  500
#define LED_HOMING_MS   200
#define LED_FAULT_MS    100
#define LED_PAUSED_MS   800
