#pragma once

// ============================================================
// Screwdriver Antenna Tuner — ESP32 Configuration
// 28BYJ-48 stepper + AD8307 Bruene coupler + WiFi AP + BT
// ============================================================

// -----------------------------------------------------------
// Motor GPIO (28BYJ-48 via ULN2003)
// -----------------------------------------------------------
#define PIN_MOTOR_IN1   25
#define PIN_MOTOR_IN2   26
#define PIN_MOTOR_IN3   27
#define PIN_MOTOR_IN4   14
#define PIN_HOME_SW     34   // Home limit switch (LOW when triggered, pulled HIGH)
#define PIN_TOP_SW      36   // Top limit switch (input-only GPIO)

// -----------------------------------------------------------
// ADC Pins
// -----------------------------------------------------------
#define PIN_FWD_ADC     35
#define PIN_REFL_ADC    39

// -----------------------------------------------------------
// Buttons and LED
// -----------------------------------------------------------
#define PIN_BAND_UP     4
#define PIN_BAND_DN     5
#define PIN_TUNE_BTN    0    // Also triggers auto-tune cycle
#define PIN_LED_STATUS  2
#define PIN_LED_SWR_OK  17
#define PIN_LED_SWR_WARN 16
#define PIN_LED_SWR_HIGH 18

// -----------------------------------------------------------
// AD8307 Log Amp Constants
// -----------------------------------------------------------
#define AD8307_MV_PER_DB   25.0f   // Slope: 25 mV/dB
#define AD8307_INTERCEPT   84.0f   // Output at -84 dBm = 0 V
#define ADC_VREF_MV        3300.0f
#define ADC_BITS           4095.0f
#define ADC_OVERSAMPLE     64      // Averages for noise reduction

// -----------------------------------------------------------
// Directional Coupler
// -----------------------------------------------------------
#define COUPLER_FWD_DB     20.0f   // Forward port coupling factor dB
#define COUPLER_REFL_DB    20.0f   // Reflected port coupling factor dB
#define CAL_OFFSET_FWD     0.0f    // User calibration trim (saved to NVS)
#define CAL_OFFSET_REFL    0.0f

// -----------------------------------------------------------
// SWR Thresholds
// -----------------------------------------------------------
#define SWR_GOOD           1.5f
#define SWR_WARN           2.5f
#define SWR_MAX_DISPLAY   10.0f
#define MIN_FWD_DBM       -20.0f   // Ignore readings below this power

// -----------------------------------------------------------
// Motor Mechanics
//   28BYJ-48: 4096 full-steps = 1 rev (half-step mode: 8192)
//   1/4-20 rod: 20 TPI → 1.27 mm/rev
//   Coil pitch: 2.3 mm → steps/turn = (2.3/1.27) × 4096 = 7420 half-steps
//   Full coil 41 turns: 41 × 7420 = 304,220 half-steps
// -----------------------------------------------------------
#define STEPS_PER_COIL_TURN  7420    // half-steps per winding turn
#define MAX_STEPS           304220   // full coil extent (41.0 turns)
#define HOME_SPEED_DELAY      3      // ms per step during homing (slow)
#define FAST_SPEED_DELAY      2      // ms per step during band-change move
#define TUNE_SPEED_DELAY      5      // ms per step during fine-tune sweep
#define TUNE_STEP_SIZE        370    // half-steps per SWR sample (~0.05 T)
#define SWR_AVERAGE_N         8      // SWR samples to average per point

// -----------------------------------------------------------
// WiFi / Bluetooth
// -----------------------------------------------------------
#define WIFI_SSID        "SC-Tuner"
#define WIFI_PASS        "screw73"
#define BT_NAME          "SC-Tuner"
#define HTTP_PORT        80
#define BT_INTERVAL_MS   200         // BT telemetry packet interval

// -----------------------------------------------------------
// NVS Persistence Keys
// -----------------------------------------------------------
#define NVS_NAMESPACE    "screw"
#define NVS_KEY_BAND     "band"
#define NVS_KEY_POS      "pos"       // current band saved position
// Per-band positions stored as "pos_0" through "pos_14"
#define NVS_KEY_CAL_FWD  "cfwd"
#define NVS_KEY_CAL_REV  "crev"
#define NVS_KEY_HOMED    "homed"

// -----------------------------------------------------------
// Band Table Entry
// -----------------------------------------------------------
struct BandEntry {
    const char* name;
    float       freq_mhz;
    float       coil_L_uH;
    float       efficiency_pct;
    long        default_steps;   // pre-calculated home half-step position
    bool        needs_ext_coil;  // true for 160m
    const char* coil_note;
};

// -----------------------------------------------------------
// Band Table  (15 entries, index 0 = 160m)
// HF step positions from coil_design_tables.csv (half-steps)
// VHF/UHF bands: coil fully retracted (0 steps), direct feed
// -----------------------------------------------------------
#define NUM_BANDS 15
static const BandEntry BANDS[NUM_BANDS] = {
    // name     freq     L_uH   eff%   steps    ext    note
    {"160m",  1.900,  282.50,  0.13, 304220, true,  "Install 160m ext coil; main coil at max"},
    {"80m",   3.750,   72.16,  1.20, 304220, false, "Full coil; max position"},
    {"60m",   5.330,   35.33,  3.80, 176226, false, ""},
    {"40m",   7.150,   19.24,  9.50, 115306, false, ""},
    {"30m",  10.125,    9.09, 25.20,  71028, false, ""},
    {"20m",  14.175,    4.21, 52.00,  44592, false, ""},
    {"17m",  18.118,    2.21, 73.70,  30866, false, ""},
    {"15m",  21.225,    1.30, 85.50,  23002, false, ""},
    {"12m",  24.940,    0.680,93.30,  16174, false, ""},
    {"10m",  28.500,    0.238,97.90,   9346, false, ""},
    {"6m",   51.000,    0.0,  98.00,      0, false, "Remove coil; use short VHF whip 1.40m"},
    {"2m",  146.000,    0.0,  98.00,      0, false, "Use VHF stub 0.488m; no loading coil"},
    {"1.25m",222.000,   0.0,  98.00,      0, false, "Use 0.321m stub"},
    {"70cm", 446.000,   0.0,  98.00,      0, false, "Use 0.160m stub"},
    {"33cm", 906.000,   0.0,  98.00,      0, false, "Use stub antenna"},
};
