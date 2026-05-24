// =============================================================================
// config.h — HF Tuner Firmware Configuration
// Project:  RF-HFTuner / Motorized Antenna Tuner Suite
// Device:   RF-HFTuner (ESP32-WROOM-32)
// Rev:      1.0  2026-05-01
// =============================================================================
#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// =============================================================================
// FIRMWARE VERSION AND DEVICE IDENTITY
// =============================================================================

#define FW_VERSION_MAJOR     1
#define FW_VERSION_MINOR     0
#define FW_VERSION_PATCH     0
#define FW_VERSION_STRING    "1.0.0"
#define DEVICE_NAME          "RF-HFTuner"
#define DEVICE_MODEL         "HF-L-T-PI-001"
#define DEVICE_DESCRIPTION   "Motorized HF L/T/Pi Antenna Tuner Controller"
#define BUILD_DATE           __DATE__
#define BUILD_TIME           __TIME__

// =============================================================================
// GPIO PIN DEFINITIONS — CAPACITOR STEPPER (TMC2208 #1)
// =============================================================================

#define PIN_CAP_STEP         18   // Step pulse output
#define PIN_CAP_DIR          19   // Direction output (HIGH=increasing pF)
#define PIN_CAP_EN           5    // Enable (LOW=active, HIGH=disabled/coast)
#define PIN_CAP_UART         17   // UART to TMC2208 PDN_UART for config

// =============================================================================
// GPIO PIN DEFINITIONS — INDUCTOR STEPPER (TMC2208 #2)
// =============================================================================

#define PIN_IND_STEP         25   // Step pulse output
#define PIN_IND_DIR          26   // Direction output (HIGH=increasing µH)
#define PIN_IND_EN           27   // Enable (LOW=active, HIGH=disabled/coast)
#define PIN_IND_UART         16   // UART to TMC2208 PDN_UART for config

// =============================================================================
// GPIO PIN DEFINITIONS — LIMIT SWITCHES (all input-only, external pull-ups)
// =============================================================================

#define PIN_CAP_MIN          36   // Capacitor at minimum position (VP pin)
#define PIN_CAP_MAX          39   // Capacitor at maximum position (VN pin)
#define PIN_IND_MIN          34   // Inductor at minimum position
#define PIN_IND_MAX          35   // Inductor at maximum position
// Note: GPIO 34,35,36,39 are INPUT-ONLY. External 10kΩ pull-up to 3.3V required.
// Switch wires GPIO to GND when actuated; firmware reads LOW = limit hit.

// =============================================================================
// GPIO PIN DEFINITIONS — SWR BRIDGE ADC
// =============================================================================

#define PIN_VFWD_ADC         32   // Forward power voltage (ADC1_CH4)
#define PIN_VREV_ADC         33   // Reflected power voltage (ADC1_CH5)

// =============================================================================
// GPIO PIN DEFINITIONS — BAND SELECTOR (4-BIT BINARY)
// =============================================================================

#define PIN_BAND_BIT0        12   // LSB
#define PIN_BAND_BIT1        13
#define PIN_BAND_BIT2        14
#define PIN_BAND_BIT3        15   // MSB

// =============================================================================
// GPIO PIN DEFINITIONS — RELAY NETWORK (via PCF8574 I2C expander, preferred)
// =============================================================================

// If using PCF8574 I2C expander (preferred):
#define PCF8574_ADDR         0x20  // I2C address (A0=A1=A2=GND)
#define PCF8574_RELAY_K1     0     // PCF P0 → ULN2003 → K1 (topology bit 0)
#define PCF8574_RELAY_K2     1     // PCF P1 → ULN2003 → K2 (topology bit 1)
#define PCF8574_RELAY_K3     2     // PCF P2 → ULN2003 → K3 (topology bit 2)

// If using direct GPIO for relays (alternative, check I2C pin conflict):
#define PIN_RELAY_K1         4    // L/Pi/T topology relay 1
#define PIN_RELAY_K2         2    // L/Pi/T topology relay 2 (strapping pin!)
#define PIN_RELAY_K3         15   // L/Pi/T topology relay 3

// =============================================================================
// GPIO PIN DEFINITIONS — I2C OLED (SSD1306, 0.96", 128x64)
// =============================================================================

#define PIN_I2C_SDA          21   // I2C SDA (also RELAY_K1 if not using PCF8574)
#define PIN_I2C_SCL          22   // I2C SCL (also RELAY_K2 if not using PCF8574)
#define OLED_I2C_ADDR        0x3C // SSD1306 address (ADDR pin to GND)
#define OLED_WIDTH           128
#define OLED_HEIGHT          64

// =============================================================================
// GPIO PIN DEFINITIONS — STATUS LED
// =============================================================================

#define PIN_LED_STATUS       2    // Onboard LED (strapping pin, 10kΩ pull-up)

// =============================================================================
// MOTOR CONSTANTS — NEMA 17
// =============================================================================

#define MOTOR_STEPS_PER_REV      200    // Full steps per revolution (1.8°/step)
#define MOTOR_MICROSTEPS         16     // Microstepping divisor (1/16)
#define MOTOR_USTEPS_PER_REV     (MOTOR_STEPS_PER_REV * MOTOR_MICROSTEPS)  // 3200

// T8 leadscrew (8mm pitch, 4-start, 2mm per rev effective):
#define MOTOR_LEADSCREW_MM_PER_REV   2.0f   // mm of linear travel per revolution

// =============================================================================
// CAPACITOR MOTOR CONSTANTS (TM-VCAP-001 compatible)
// =============================================================================

// Physical: 360° rotation of capacitor rotor = full C swing
// Gearing: 1:1 direct drive (no gearbox)
#define CAP_STEPS_PER_DEGREE     (MOTOR_USTEPS_PER_REV / 360.0f)  // 8.89 steps/deg
#define CAP_MIN_PF               10.0f   // Minimum capacitance (pF), plates fully open
#define CAP_MAX_PF               500.0f  // Maximum capacitance (pF), plates fully meshed
#define CAP_VOLTAGE_RATING_V     5000.0f // Working voltage (5kV DC, or 3.5kV peak RF)
#define CAP_TOTAL_TRAVEL_DEG     180.0f  // Total angular travel of rotor (degrees)
#define CAP_STEPS_TOTAL          (CAP_STEPS_PER_DEGREE * CAP_TOTAL_TRAVEL_DEG)
// pF per step (linear approximation; actual capacitance may be non-linear vs. angle):
#define CAP_PF_PER_STEP          ((CAP_MAX_PF - CAP_MIN_PF) / CAP_STEPS_TOTAL)
#define CAP_BACKLASH_STEPS       10      // Backlash compensation (steps)
#define CAP_HOME_SPEED_US        1000    // Step period for homing (µs)
#define CAP_RUN_SPEED_US         500     // Step period for normal moves (µs)
#define CAP_ACCEL_STEPS          50      // Acceleration ramp length (steps)

// =============================================================================
// INDUCTOR MOTOR CONSTANTS (TM-RLIND-001 compatible)
// =============================================================================

// Physical: roller inductor, leadscrew drives contact along coil
#define IND_STEPS_PER_MM         (MOTOR_USTEPS_PER_REV / MOTOR_LEADSCREW_MM_PER_REV)
// = 3200 / 2.0 = 1600 steps/mm
#define IND_MIN_UH               0.0f    // Minimum inductance (µH), contact at start
#define IND_MAX_UH               30.0f   // Maximum inductance (µH), contact at end
#define IND_TOTAL_TRAVEL_MM      50.0f   // Total mechanical travel (mm)
#define IND_STEPS_TOTAL          (IND_STEPS_PER_MM * IND_TOTAL_TRAVEL_MM)
#define IND_UH_PER_STEP          ((IND_MAX_UH - IND_MIN_UH) / IND_STEPS_TOTAL)
#define IND_BACKLASH_STEPS       5       // Backlash compensation (steps)
#define IND_HOME_SPEED_US        1500    // Step period for homing (µs)
#define IND_RUN_SPEED_US         600     // Step period for normal moves (µs)
#define IND_ACCEL_STEPS          30      // Acceleration ramp length (steps)

// Wheeler formula constants for single-layer air-core coil (roller inductor):
// L(µH) = (r² × N²) / (9r + 10l)  where r=radius(in), l=length(in), N=turns
// For roller inductor: N varies with contact position, r and l are fixed.
// Coil geometry (fill in actual measured values):
#define IND_COIL_RADIUS_IN       0.75f   // Coil radius in inches (example: 0.75")
#define IND_COIL_LENGTH_IN       2.0f    // Total wound length in inches
#define IND_COIL_TOTAL_TURNS     50.0f   // Total turns on coil
// Turns per mm of travel: (total turns) / (total wound length in mm)
#define IND_TURNS_PER_MM         (IND_COIL_TOTAL_TURNS / (IND_COIL_LENGTH_IN * 25.4f))

// =============================================================================
// SWR BRIDGE CALIBRATION CONSTANTS
// =============================================================================

#define SWR_ADC_REF_VOLTAGE      3.3f    // ADC reference voltage (V)
#define SWR_ADC_RESOLUTION       4095    // 12-bit ADC max count
#define SWR_VFWD_DIVIDER_RATIO   0.1515f // 10k/(56k+10k) voltage divider
#define SWR_VREV_DIVIDER_RATIO   0.1515f // Same divider on REV channel
#define SWR_COUPLER_FACTOR       10.0f   // -20dB coupler: V_line = V_coupled × 10
#define SWR_DIODE_VF_25C         0.35f   // BAT46 forward voltage at 25°C (V)
#define SWR_DIODE_TEMPCO         -0.002f // Temperature coefficient (V/°C)
#define SWR_MIN_DETECTABLE_W     1.0f    // Minimum detectable power (watts)
#define SWR_ADC_NOISE_FLOOR_MV   5.0f    // ADC noise floor (mV)
#define SWR_FAULT_THRESHOLD      3.0f    // SWR fault threshold; stop tuning
#define SWR_TARGET               1.5f    // Auto-tune target SWR
#define SWR_SAMPLES              16      // ADC samples to average per reading
#define SWR_CAL_OFFSET_FWD       0.0f    // Forward channel zero-power offset (V)
#define SWR_CAL_OFFSET_REV       0.0f    // Reflected channel zero-power offset (V)
#define SWR_CAL_SLOPE_FWD        1.0f    // Forward channel gain correction
#define SWR_CAL_SLOPE_REV        1.0f    // Reflected channel gain correction

// ADC nonlinearity correction polynomial (ESP32 ADC):
// V_corrected = V_raw + (A2×V² + A1×V + A0)  (empirical, calibrate per unit)
#define SWR_ADC_POLY_A0          0.0f
#define SWR_ADC_POLY_A1          0.0f
#define SWR_ADC_POLY_A2          0.0f

// =============================================================================
// BAND DEFINITIONS
// =============================================================================

// Network topology enum
typedef enum {
    L_LOWPASS  = 0,  // L-network: series L, shunt C (low-pass, Z_source < Z_load)
    L_HIGHPASS = 1,  // L-network: shunt C, series L (high-pass, Z_source > Z_load)
    PI_NETWORK = 2,  // Pi-network: C1, series L, C2 (good for pi-match, T>50Ω)
    T_NETWORK  = 3,  // T-network: L1, shunt C, L2 (good for low-Z antennas)
    NET_AUTO   = 4,  // Firmware selects topology based on impedance mismatch
} NetworkType;

// Band data structure
typedef struct {
    const char* name;           // Band name string
    float       freq_mhz;       // Nominal center frequency (MHz)
    float       wavelength_m;   // Free-space wavelength (m)
    float       L_min_uh;       // Minimum inductance for this band (µH)
    float       L_max_uh;       // Maximum inductance for this band (µH)
    float       C_min_pf;       // Minimum capacitance for this band (pF)
    float       C_max_pf;       // Maximum capacitance for this band (pF)
    NetworkType preferred_net;  // Preferred network topology
    float       preset_L_uh;    // Last saved tuning preset L (µH)
    float       preset_C_pf;    // Last saved tuning preset C (pF)
} BandData;

// Number of supported bands
#define NUM_BANDS  11

// Band index definitions
#define BAND_160M   0
#define BAND_80M    1
#define BAND_60M    2
#define BAND_40M    3
#define BAND_30M    4
#define BAND_20M    5
#define BAND_17M    6
#define BAND_15M    7
#define BAND_12M    8
#define BAND_10M    9
#define BAND_6M    10

// Band table (defined in tuner.h as const BandData BAND_TABLE[NUM_BANDS])
// Frequency, L range, C range, preferred topology per band:
//
//  Band   Freq    L_min   L_max   C_min   C_max   Network
//  160m   1.85    5.0     30.0    50      500     PI_NETWORK
//  80m    3.60    2.0     20.0    25      300     PI_NETWORK
//  60m    5.35    1.0     15.0    15      200     L_LOWPASS
//  40m    7.15    0.5     10.0    10      150     L_LOWPASS
//  30m    10.12   0.3      7.0     8      100     L_LOWPASS
//  20m    14.20   0.2      5.0     5       75     L_LOWPASS
//  17m    18.10   0.1      4.0     4       60     L_LOWPASS
//  15m    21.20   0.1      3.0     3       50     L_HIGHPASS
//  12m    24.90   0.1      2.5     2       40     L_HIGHPASS
//  10m    28.50   0.05     2.0     2       35     L_HIGHPASS
//   6m    50.10   0.02     0.5     1       20     L_HIGHPASS

// =============================================================================
// AUTO-TUNE PARAMETERS
// =============================================================================

#define AT_MAX_ITERATIONS       100     // Maximum auto-tune iterations before fault
#define AT_SWR_TARGET           1.5f    // Stop when SWR reaches this value
#define AT_SWR_GOOD             1.2f    // "Excellent" threshold
#define AT_COARSE_L_STEPS       5       // Coarse L sweep: step this many L positions
#define AT_COARSE_C_STEPS       10      // Coarse C sweep steps per L position
#define AT_FINE_L_STEP_UH       0.1f    // Fine L step size (µH)
#define AT_FINE_C_STEP_PF       2.0f    // Fine C step size (pF)
#define AT_SETTLE_MS            50      // Settle time after motor move (ms)
#define AT_SWR_MEASURE_MS       100     // Time to average SWR reading (ms)
#define AT_GRADIENT_ALPHA       0.5f    // Gradient descent step scale factor
#define AT_MIN_POWER_W          5.0f    // Minimum TX power required for auto-tune
#define AT_RETRY_COUNT          3       // Retry count if SWR target not reached

// =============================================================================
// BLUETOOTH CONFIGURATION
// =============================================================================

#define BT_DEVICE_NAME          "RF-HFTuner"
#define BT_PIN                  "1234"  // SPP pairing PIN

// =============================================================================
// WIFI CONFIGURATION
// =============================================================================

#define WIFI_SSID               "RF-HFTuner"
#define WIFI_PASSWORD           "hamradio"      // Change for deployment
#define WIFI_CHANNEL            6
#define WIFI_MAX_CLIENTS        4
#define WIFI_AP_IP              "192.168.4.1"
#define WIFI_AP_GATEWAY         "192.168.4.1"
#define WIFI_AP_SUBNET          "255.255.255.0"
#define HTTP_PORT               80

// =============================================================================
// NVS (NON-VOLATILE STORAGE) NAMESPACES
// =============================================================================

#define NVS_NAMESPACE_PRESETS   "tuner_presets"  // Band preset L/C values
#define NVS_NAMESPACE_CAL       "swr_cal"         // SWR calibration data
#define NVS_NAMESPACE_CONFIG    "tuner_config"    // General configuration
#define NVS_KEY_LAST_BAND       "last_band"
#define NVS_KEY_LAST_L          "last_L"
#define NVS_KEY_LAST_C          "last_C"
#define NVS_KEY_CAL_OFFSET_FWD  "cal_off_f"
#define NVS_KEY_CAL_OFFSET_REV  "cal_off_r"
#define NVS_KEY_CAL_SLOPE_FWD   "cal_slp_f"
#define NVS_KEY_CAL_SLOPE_REV   "cal_slp_r"
#define NVS_KEY_VF_ACTUAL       "vf_actual"

// NVS preset keys per band (append band index 0-10):
// "pre_L_0" through "pre_L_10" — L preset per band
// "pre_C_0" through "pre_C_10" — C preset per band
// "pre_S_0" through "pre_S_10" — best SWR at preset per band

// =============================================================================
// TIMING CONSTANTS
// =============================================================================

#define LOOP_PERIOD_MS          10      // Main loop target period (ms)
#define SWR_UPDATE_MS           500     // SWR display update interval (ms)
#define OLED_UPDATE_MS          250     // OLED refresh interval (ms)
#define STATUS_BROADCAST_MS     1000    // WiFi JSON broadcast interval (ms)
#define BT_READ_TIMEOUT_MS      100     // BT command read timeout (ms)
#define MOTOR_IDLE_TIMEOUT_MS   2000    // Disable motors after idle (ms)
#define HOME_TIMEOUT_MS         30000   // Maximum time to complete homing (ms)
#define TUNE_TIMEOUT_MS         120000  // Maximum auto-tune time (ms)
#define DEBOUNCE_MS             5       // Limit switch debounce time (ms)
#define RELAY_SETTLE_MS         20      // Relay settling time after switching (ms)

// =============================================================================
// LED BLINK CODES
// =============================================================================

#define LED_IDLE_ON_MS          1000    // Idle: slow blink
#define LED_IDLE_OFF_MS         1000
#define LED_TUNING_ON_MS        100     // Tuning: fast blink
#define LED_TUNING_OFF_MS       100
#define LED_FAULT_ON_MS         100     // Fault: rapid double-blink
#define LED_FAULT_OFF_MS        100
#define LED_FAULT_PAUSE_MS      600
#define LED_HOMING_ON_MS        250     // Homing: medium blink
#define LED_HOMING_OFF_MS       250

// =============================================================================
// FIRMWARE STATE MACHINE STATES
// =============================================================================

typedef enum {
    STATE_INIT      = 0,
    STATE_HOMING    = 1,
    STATE_IDLE      = 2,
    STATE_TUNING    = 3,
    STATE_MOVING    = 4,
    STATE_CALIBRATE = 5,
    STATE_FAULT     = 6,
    STATE_ESTOP     = 7,
} FirmwareState;

// =============================================================================
// SAFETY LIMITS
// =============================================================================

#define SAFETY_MAX_SWR_FAULT    4.0f    // SWR above this → FAULT state
#define SAFETY_MAX_POWER_W      500.0f  // Maximum input power this unit can measure
#define SAFETY_CAP_MAX_STEPS    (int)(CAP_STEPS_TOTAL + 100)  // Hard limit on steps
#define SAFETY_IND_MAX_STEPS    (int)(IND_STEPS_TOTAL + 100)  // Hard limit on steps

// =============================================================================
// END OF config.h
// =============================================================================

#endif // CONFIG_H
