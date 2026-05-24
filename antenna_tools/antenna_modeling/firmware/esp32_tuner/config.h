/**
 * config.h — TM-TUNER-001 Rev A
 * ESP32 Automatic Antenna Tuner Configuration
 * Modify WIFI_SSID, WIFI_PASSWORD, and API_HOST before flashing.
 */

#pragma once

// ─── Firmware ─────────────────────────────────────────────────────────────────
#define FW_VERSION       "1.0.0"
#define DEVICE_NAME      "NEC-AutoTuner"
#define OTA_PASSWORD     "antenna2026"

// ─── Network ──────────────────────────────────────────────────────────────────
#define WIFI_SSID        "YOUR_SSID"
#define WIFI_PASSWORD    "YOUR_PASSWORD"
#define API_HOST         "192.168.1.100"
#define API_PORT         8000
#define API_TIMEOUT_MS   10000

// ─── Serial ───────────────────────────────────────────────────────────────────
#define SERIAL_BAUD      115200
#define BT_DEVICE_NAME   "AutoTuner"

// ─── Shift Register (74HC595) — L-tap selector ───────────────────────────────
#define SR_CLK_PIN       18
#define SR_DATA_PIN      23
#define SR_LATCH_PIN     5
#define L_TAPS           8       // Number of inductance taps

// ─── Capacitor Stepper ────────────────────────────────────────────────────────
#define CAP_STEP_PIN     26
#define CAP_DIR_PIN      27
#define CAP_HOME_PIN     34      // Hall sensor (INPUT_PULLUP, active LOW)
#define CAP_STEPS_FULL   200     // Steps for full rotation
#define CAP_MAX_STEPS    195     // Mechanical limit (just under full rotation)
#define STEP_DELAY_US    800     // Microseconds between steps (speed)
#define ACCEL_STEPS      20      // Steps to accelerate/decelerate

// ─── SWR Detector ADC ────────────────────────────────────────────────────────
#define SWR_FWD_PIN      32      // ADC1_CH4 — forward detector (0–3.3V)
#define SWR_REF_PIN      33      // ADC1_CH5 — reflected detector (0–3.3V)
#define ADC_SAMPLES      64      // Oversample for noise reduction
#define ADC_REF_MV       3300    // ADC reference millivolts
#define COUPLING_DB      (-30)   // Directional coupler coupling factor (dB)
// Detector sensitivity: 1N5711 Schottky, ~0.5 mV/µW at low level
// Voltage divider ratio on detector output (adjusts for input range)
#define V_DIVIDER_RATIO  0.33f   // e.g., 10kΩ/(10kΩ+20kΩ) = 0.33

// ─── Bypass Relay ────────────────────────────────────────────────────────────
#define BYPASS_RELAY_PIN 25      // HIGH = bypass engaged (direct connect)
#define SWR_BYPASS_THRESH 1.15f  // Engage bypass if SWR < this

// ─── Fine-Tuning ──────────────────────────────────────────────────────────────
#define FINE_TUNE_L_RANGE  2     // ±2 taps during fine search
#define FINE_TUNE_C_RANGE  10    // ±10 steps during fine search
#define SWR_GOOD_THRESH   1.5f   // SWR threshold for "good match"
#define SWR_MAX_SAFE      5.0f   // Maximum SWR before halting TX

// ─── NVS Tune Map ────────────────────────────────────────────────────────────
#define NVS_NAMESPACE    "tuner"
#define MAP_FREQ_STEP_KHZ 100    // Frequency resolution of stored map
#define MAP_FREQ_LOW_KHZ  1800   // 160m low edge
#define MAP_FREQ_HIGH_KHZ 29700  // 10m high edge
#define MAP_ENTRIES       ((MAP_FREQ_HIGH_KHZ - MAP_FREQ_LOW_KHZ) / MAP_FREQ_STEP_KHZ + 1)

// ─── Timing ───────────────────────────────────────────────────────────────────
#define POLL_INTERVAL_MS  500    // SWR readout update interval
#define SETTLE_DELAY_MS   100    // Wait after relay/stepper change before reading SWR
#define HOME_TIMEOUT_MS   5000   // Timeout for homing sequence
#define WIFI_TIMEOUT_MS   15000  // WiFi connection timeout
#define WDT_TIMEOUT_S     30     // Watchdog timer

// ─── Antenna Type (for API prediction) ───────────────────────────────────────
// Override via "SETANT <type>" BT command
#define DEFAULT_ANT_TYPE "dipole"
