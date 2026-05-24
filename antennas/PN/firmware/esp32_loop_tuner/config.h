// ============================================================
//  config.h — Pool Noodle Magnetic Loop Tuner
//  TM-PNL-FW-001 Rev A
//  ESP32-WROOM-32 + Bruene coupler + AD8307 x2
//  Motorized air-variable tuning capacitor via 28BYJ-48
// ============================================================

#pragma once
#include <Arduino.h>

// ---- GPIO: SWR Measurement ----
#define PIN_FWD_ADC      35    // AD8307 #1 forward output (ADC1_CH7, input-only)
#define PIN_REFL_ADC     39    // AD8307 #2 reflected output (ADC1_CH3, input-only)

// ---- GPIO: Motor (28BYJ-48 + ULN2003) ----
#define PIN_MOTOR_IN1    25    // ULN2003 IN1 → ORG wire
#define PIN_MOTOR_IN2    26    // ULN2003 IN2 → YEL wire
#define PIN_MOTOR_IN3    27    // ULN2003 IN3 → GRN wire
#define PIN_MOTOR_IN4    14    // ULN2003 IN4 → BLK wire

// ---- GPIO: Capacitor Travel Limit Switches ----
#define PIN_CAP_MIN_SW   34    // Full-mesh stop (max C); input-only; 10kΩ pull-up; LOW=triggered
#define PIN_CAP_MAX_SW   36    // Min-mesh stop (min C);  input-only; 10kΩ pull-up; LOW=triggered

// ---- GPIO: Buttons ----
#define PIN_TUNE_BTN     0     // Auto-tune trigger (boot button, active LOW, pull-up)
#define PIN_BAND_UP      4     // Band select up, active LOW, pull-up
#define PIN_BAND_DN      5     // Band select down, active LOW, pull-up

// ---- GPIO: LEDs ----
#define PIN_LED_STATUS   2     // Status LED (blue onboard, active HIGH)
#define PIN_LED_SWR_OK   17    // Green: SWR < SWR_GOOD
#define PIN_LED_SWR_WARN 16    // Yellow: SWR_GOOD ≤ SWR < SWR_WARN
#define PIN_LED_SWR_HIGH 18    // Red: SWR ≥ SWR_WARN

// ---- Capacitor Motor Constants ----
// 28BYJ-48: 8192 half-steps/output rev
// External gear: 4:1 (10T motor gear → 40T cap gear)
// Total: 32768 half-steps per capacitor revolution
// Air-variable cap: ~10 revolutions full travel → 327,680 total steps
#define STEPS_PER_CAP_REV    32768UL
#define MAX_CAP_STEPS        327680UL
#define HOME_SPEED_DELAY     3          // ms/step during homing (slow, reliable)
#define TUNE_SPEED_DELAY     5          // ms/step during auto-tune (SWR must stabilize)
#define FAST_SPEED_DELAY     2          // ms/step during band-change slew
#define TUNE_STEP_SIZE       500        // initial bisection step (half-steps)
#define TUNE_STEP_MIN        125        // stop bisecting below this step size

// ---- AD8307 Log Amplifier / ADC ----
#define AD8307_MV_PER_DB     25.0f     // transfer function slope
#define AD8307_INTERCEPT     84.0f     // intercept in dBm
#define ADC_VREF_MV          3300.0f   // ESP32 ADC reference
#define ADC_BITS             4095.0f   // 12-bit ADC
#define ADC_OVERSAMPLE       64        // samples averaged per reading
#define SWR_AVERAGE_N        8         // readings averaged per SWR measurement
#define COUPLER_FWD_DB       20.0f     // Bruene coupler coupling factor (dB)
#define COUPLER_REFL_DB      20.0f
#define CAL_OFFSET_FWD       0.0f      // factory cal offset (dB); adjust after calibration
#define CAL_OFFSET_REFL      0.0f
#define MIN_FWD_DBM          -20.0f    // below this level, SWR reading invalid

// ---- SWR Display Thresholds ----
#define SWR_GOOD             1.5f
#define SWR_WARN             2.5f
#define SWR_MAX_DISPLAY      10.0f

// ---- Safety: High-Voltage Warning Zone ----
// Warn when cap is near minimum-mesh end (low C = high voltage)
#define CAP_HV_THRESHOLD_STEPS  15000UL  // steps from MIN switch; warn below this

// ---- WiFi Access Point ----
#define WIFI_SSID            "PNML-Tuner"
#define WIFI_PASS            "antenna73"
#define WIFI_CHANNEL         6

// ---- Bluetooth Serial ----
#define BT_DEVICE_NAME       "PNML-Tuner"
#define BT_INTERVAL_MS       200        // BT packet interval

// ---- NVS Persistent Storage ----
#define NVS_NAMESPACE        "pnml"
#define NVS_KEY_BAND         "band_idx"
#define NVS_KEY_CAP_POS      "cap_pos"
#define NVS_KEY_CAL_FWD      "cal_fwd"
#define NVS_KEY_CAL_REFL     "cal_refl"
#define NVS_KEY_HOMED        "homed"

// ---- HTTP ----
#define HTTP_PORT            80

// ---- Loop Type Enum ----
enum LoopType : uint8_t { LOOP_STL = 0, LOOP_QUAD = 1 };

// ---- Tuner State Enum ----
enum TunerState : uint8_t {
    ST_IDLE       = 0,
    ST_HOMING     = 1,
    ST_TUNING     = 2,
    ST_LOCKED     = 3,
    ST_QUAD_READY = 4
};

// ---- Band Table Entry ----
struct BandEntry {
    const char* name;
    float       freq_mhz;
    LoopType    loop_type;
    float       C_pF;              // target resonating capacitance (STL)
    float       L_uH;              // loop inductance
    float       Q_est;             // Q at resonance
    float       efficiency_pct;    // antenna efficiency
    float       bandwidth_khz;     // -3 dB bandwidth
    float       vcap_5w_peak;      // capacitor peak voltage at 5W (safety ref)
    long        default_cap_steps; // motor steps from home for this band
    const char* match_note;        // operating note shown on web/CYD
};

#define NUM_BANDS 15

static const BandEntry BANDS[NUM_BANDS] = {
    // ---- HF STL Bands (160m–10m) ----
    {"160m",  1.900f,  LOOP_STL,   41.7f, 168.11f,  891.f, 12.89f,  2.13f,  2791.f,  294912L,
     "DANGER: >2.8kV@5W. Bleeder 100kOhm mandatory. Vacuum cap required."},
    {"80m",   3.750f,  LOOP_STL,   26.4f,  68.28f,  904.f, 22.14f,  4.15f,  2379.f,  196608L,
     "CAUTION: >2.4kV@5W. Vacuum or high-V air-variable cap."},
    {"40m",   7.150f,  LOOP_STL,   21.0f,  23.55f,  807.f, 29.23f,  8.86f,  1738.f,  131072L,
     "CAUTION: >1.7kV@5W. Air-variable cap; do not touch cap during TX."},
    {"30m",  10.125f,  LOOP_STL,   12.9f,  19.13f,  936.f, 44.68f, 10.82f,  1775.f,   98304L,
     "High voltage present. Tune then stand back."},
    {"20m",  14.175f,  LOOP_STL,    7.4f,  16.96f, 1091.f, 65.65f, 12.99f,  1682.f,   73728L,
     "Tune cap for SWR minimum. BW = 13 kHz."},
    {"17m",  18.118f,  LOOP_STL,   20.8f,   3.70f,  606.f, 28.37f, 29.89f,   957.f,   57344L,
     "Tune cap for SWR minimum. BW = 30 kHz."},
    {"15m",  21.225f,  LOOP_STL,   17.7f,   3.18f,  643.f, 31.59f, 33.00f,   966.f,   45056L,
     "Tune cap for SWR minimum. BW = 33 kHz."},
    {"12m",  24.940f,  LOOP_STL,   15.3f,   2.66f,  681.f, 33.84f, 36.61f,   969.f,   34816L,
     "Tune cap for SWR minimum. BW = 37 kHz."},
    {"10m",  28.500f,  LOOP_STL,   14.5f,   2.16f,  708.f, 32.07f, 40.26f,   963.f,   28672L,
     "Tune cap for SWR minimum. BW = 40 kHz."},
    // ---- VHF/UHF Quad Bands (no capacitor required) ----
    {"6m",   51.000f,  LOOP_QUAD,   0.0f,    0.0f,  600.f, 98.00f, 85.0f,      0.f,       0L,
     "Full-wave quad. Side=1.397m. 2.5:1 UNUN or gamma match to 50Ohm."},
    {"2m",  146.000f,  LOOP_QUAD,   0.0f,    0.0f,  400.f, 97.00f,365.0f,      0.f,       0L,
     "Full-wave quad. Side=0.488m. 2.5:1 UNUN. Zin approx 125Ohm."},
    {"1.25m",222.000f, LOOP_QUAD,   0.0f,    0.0f,  300.f, 96.00f,740.0f,      0.f,       0L,
     "Full-wave quad. Side=0.321m. Gamma match preferred at 222 MHz."},
    {"70cm", 446.000f, LOOP_QUAD,   0.0f,    0.0f,  200.f, 95.00f,2230.0f,     0.f,       0L,
     "Full-wave quad. Side=0.160m. Direct feed + 2.5:1 balun."},
    {"33cm", 906.000f, LOOP_QUAD,   0.0f,    0.0f,  150.f, 92.00f,6040.0f,     0.f,       0L,
     "Rigid wire or PCB quad. Side=78.5mm. Verify element at UHF."},
    {"20cm",1296.000f, LOOP_QUAD,   0.0f,    0.0f,  100.f, 88.00f,12960.0f,    0.f,       0L,
     "SMD/PCB quad. Side=55mm. NEC model is approximate at 1.3 GHz."},
};
