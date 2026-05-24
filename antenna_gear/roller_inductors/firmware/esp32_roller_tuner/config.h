/**
 * config.h — TM-ROLLER-ESP32-001 Rev A
 * Roller Inductor Motorized Controller Configuration
 * ESP32 + DRV8825 stepper driver + limit switches + encoder
 */

#pragma once

// ─── Firmware ─────────────────────────────────────────────────────────────────
#define FW_VERSION          "1.0.0"
#define DEVICE_NAME         "RF-RollerTuner"
#define FW_BUILD_DATE       __DATE__

// ─── Coil Physical Parameters ─────────────────────────────────────────────────
// Wheeler's formula: L(µH) = (d² × N²) / (18d + 40l)
// d = coil diameter (inches), l = winding length (inches), N = turns

#define COIL_DIAMETER_IN    2.5f        // Coil form OD, inches (63.5mm)
#define COIL_TURNS_TOTAL    30          // Total turns wound on coil
#define COIL_PITCH_MM       2.54f       // Turn pitch (mm) = 10 TPI = 2.54mm/turn
#define COIL_WIND_START_MM  40.0f       // Distance from home end to first turn
#define COIL_WIND_LENGTH_MM 76.2f       // Total winding length (30 × 2.54mm)
#define COIL_WIRE_AWG       12          // Wire gauge

// ─── Motion System ────────────────────────────────────────────────────────────
#define LEADSCREW_PITCH_MM  2.0f        // Leadscrew pitch (mm per revolution)
#define GEAR_RATIO          10.0f       // Worm gear reduction ratio
#define STEPS_PER_REV       200         // Stepper motor full steps per revolution
#define MICROSTEP_DIV       16          // Microstepping divisor (1/16)

// Derived: steps per mm of carriage travel
// Steps/mm = (STEPS_PER_REV × MICROSTEP_DIV × GEAR_RATIO) / LEADSCREW_PITCH_MM
// = (200 × 16 × 10) / 2.0 = 16000 steps/mm
#define STEPS_PER_MM        ((float)(STEPS_PER_REV * MICROSTEP_DIV * GEAR_RATIO) \
                              / LEADSCREW_PITCH_MM)

// Total travel (80mm working range over 76.2mm winding + 4mm margins)
#define TRAVEL_MAX_MM       82.0f
#define TRAVEL_MAX_STEPS    ((long)(TRAVEL_MAX_MM * STEPS_PER_MM))

// ─── Motor Speeds ─────────────────────────────────────────────────────────────
#define MOTOR_SPEED_FAST    2000        // Steps/second (coarse positioning)
#define MOTOR_SPEED_SLOW    500         // Steps/second (fine positioning)
#define MOTOR_SPEED_HOME    400         // Steps/second (homing)
#define MOTOR_ACCEL         800         // Steps/second² (acceleration)
#define MOTOR_HOLD_TIMEOUT_MS 5000     // Time before motor hold current reduced

// ─── GPIO Pin Assignments ─────────────────────────────────────────────────────
#define PIN_STEP            25
#define PIN_DIR             26
#define PIN_ENABLE          27          // Active LOW
#define PIN_FAULT           33          // DRV8825 fault (active LOW, open drain)

#define PIN_ENC_A           32          // Rotary encoder phase A
#define PIN_ENC_B           36          // Rotary encoder phase B
#define PIN_ENC_SW          39          // Rotary encoder push button

#define PIN_LIM_HOME        34          // Home limit switch (active LOW)
#define PIN_LIM_MAX         35          // Max limit switch (active LOW)

#define PIN_BTN_HOME        4           // Manual home button
#define PIN_BTN_STEP_P      5           // Step+ button (coarser control)
#define PIN_BTN_STEP_M      16          // Step− button
#define PIN_BTN_AUTO        17          // Auto-tune button

#define PIN_STATUS_LED      2           // Onboard LED
#define PIN_I2C_SDA         21
#define PIN_I2C_SCL         22

// ─── Encoder ──────────────────────────────────────────────────────────────────
#define ENCODER_PPR         100         // Pulses per revolution (leadscrew encoder)
// Encoder pulses per mm = PPR / leadscrew_pitch
#define ENCODER_PULSES_PER_MM  50       // 100 PPR / 2.0mm pitch

// ─── Inductance Limits ────────────────────────────────────────────────────────
#define L_MIN_UH            0.5f        // Minimum inductance (µH) at 1-2 turns
#define L_MAX_UH            34.0f       // Maximum inductance (µH) at 30 turns

// ─── Button Debounce ──────────────────────────────────────────────────────────
#define DEBOUNCE_MS         50
#define LIM_DEBOUNCE_MS     5           // Faster debounce for limit switches

// ─── Serial / BT ──────────────────────────────────────────────────────────────
#define SERIAL_BAUD         115200
#define BT_STATUS_INTERVAL_MS  500      // Broadcast status every 500ms
#define BT_DEVICE_NAME      "RF-RollerTuner"

// ─── OLED (optional, I2C) ─────────────────────────────────────────────────────
#define OLED_ADDR           0x3C        // SSD1306 I2C address
#define OLED_UPDATE_MS      200

// ─── NVS (Non-Volatile Storage) ───────────────────────────────────────────────
#define NVS_NAMESPACE       "rollerind"
#define NVS_KEY_POSITION    "pos_steps"
#define NVS_KEY_HOMED       "homed"
#define NVS_KEY_COIL_D      "coil_d"
#define NVS_KEY_COIL_N      "coil_n"
#define NVS_KEY_COIL_PITCH  "pitch_mm"

// ─── LED Blink Rates ──────────────────────────────────────────────────────────
#define LED_IDLE_MS         2000        // Slow blink: idle/homed
#define LED_MOVING_MS       100         // Fast blink: motor moving
#define LED_ERROR_MS        50          // Very fast: fault condition
#define LED_HOMING_MS       250         // Medium: homing in progress

// ─── Watchdog ─────────────────────────────────────────────────────────────────
#define WDT_TIMEOUT_S       30

// ─── Status Codes ─────────────────────────────────────────────────────────────
enum MotorState {
    MOTOR_IDLE = 0,
    MOTOR_MOVING,
    MOTOR_HOMING,
    MOTOR_FAULT,
    MOTOR_NOT_HOMED
};

enum HomeState {
    HOME_UNKNOWN = 0,
    HOME_COMPLETE,
    HOME_FAILED
};
