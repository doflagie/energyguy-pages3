#pragma once

// =============================================================================
// config.h — ESP32 Whip Tuner (5.6m Telescopic)
// Bruene directional coupler + dual AD8307 log amps
// =============================================================================

// --- ADC GPIO Pins -----------------------------------------------------------
#define PIN_FWD_ADC   35    // AD8307 forward detector output
#define PIN_REFL_ADC  39    // AD8307 reflected detector output

// --- Button GPIO Pins --------------------------------------------------------
#define PIN_BAND_UP   4
#define PIN_BAND_DN   5
#define PIN_TUNE_BTN  0

// --- LED GPIO Pins -----------------------------------------------------------
#define PIN_LED_STATUS   2   // Heartbeat / no-RF blink
#define PIN_LED_SWR_OK   17  // Green: SWR ≤ 1.5
#define PIN_LED_SWR_WARN 16  // Yellow: SWR 1.5–2.5
#define PIN_LED_SWR_HIGH 18  // Red: SWR > 2.5

// --- AD8307 Log Amp Constants ------------------------------------------------
// Output: V_out = slope_mv_db * (P_dBm - intercept_dbm)
#define AD8307_SLOPE_MV_DB   25.0f   // mV per dB
#define AD8307_INTERCEPT_DBM (-84.0f) // dBm at 0V output
#define ADC_VREF_MV          3300.0f  // ADC reference voltage in mV
#define ADC_FULL_SCALE       4095.0f  // 12-bit ADC
#define ADC_OVERSAMPLE_N     64       // Oversampling for noise reduction

// --- Directional Coupler Constants -------------------------------------------
#define COUPLER_FWD_DB    20.0f   // Forward port coupling factor (dB, positive)
#define COUPLER_REFL_DB   20.0f   // Reflected port coupling factor (dB, positive)

// --- Calibration Offsets (saved to NVS) --------------------------------------
#define CAL_OFFSET_FWD    0.0f   // dB trim for forward path
#define CAL_OFFSET_REFL   0.0f   // dB trim for reflected path

// --- SWR Thresholds ----------------------------------------------------------
#define SWR_GOOD      1.5f    // Below this → green LED
#define SWR_WARN      2.5f    // Below this → yellow LED; above → red
#define SWR_MAX       10.0f   // Clamp display at this value
#define MIN_FWD_DBM  (-20.0f) // Minimum detectable forward power for valid reading

// --- WiFi Access Point -------------------------------------------------------
#define WIFI_SSID     "WHP-Tuner"
#define WIFI_PASS     "whip5p6m"
#define WIFI_CHANNEL  6

// --- Bluetooth ---------------------------------------------------------------
#define BT_DEVICE_NAME "WHP-Tuner"

// --- NVS Storage -------------------------------------------------------------
#define NVS_NAMESPACE  "whp"
#define NVS_KEY_BAND   "band"
#define NVS_KEY_CALFWD "cal_fwd"
#define NVS_KEY_CALRFL "cal_refl"

// --- Timing & HTTP -----------------------------------------------------------
#define HTTP_PORT       80
#define BT_INTERVAL_MS  200   // BT packet interval
#define ADC_AVERAGE_N   8     // Readings to average for one measurement

// =============================================================================
// Band Table
// =============================================================================
// coil_type: 0 = no coil (direct feed, retract whip)
//            1 = base loading coil (extend fully to 5.6m)
//            2 = series capacitor trim (retract whip + 300pF cap at base)

struct BandEntry {
    const char* name;          // Band designator, e.g. "40m"
    float       freq_mhz;      // Nominal band center frequency
    float       qw_m;          // True quarter-wave length (m)
    float       extension_m;   // Whip extension needed (m)
    uint8_t     coil_type;     // 0=none, 1=loading coil, 2=series cap
    float       coil_L_uH;    // Loading coil inductance (µH); 0 if not needed
    const char* match_note;    // Human-readable setup note
};

static const BandEntry BANDS[15] = {
    {"160m",   1.900f,  38.29f, 5.60f,  1, 144.6f,  "T200-2 31T #22AWG; extend fully"},
    {"80m",    3.750f,  19.40f, 5.60f,  1,  35.5f,  "T130-2 18T #20AWG; extend fully"},
    {"60m",    5.330f,  13.65f, 5.60f,  1,  16.3f,  "T130-2 12T #20AWG; extend fully"},
    {"40m",    7.150f,  10.18f, 5.60f,  1,   7.85f, "T106-2 8T #18AWG; extend fully"},
    {"30m",   10.125f,   7.19f, 5.60f,  1,   2.477f,"T50-2 7T #18AWG; extend fully"},
    {"20m",   14.175f,   5.13f, 5.13f,  2,   0.0f,  "No coil; retract to 5.13m; add 300pF series cap"},
    {"17m",   18.118f,   4.01f, 4.01f,  0,   0.0f,  "No coil; retract to 4.01m"},
    {"15m",   21.225f,   3.43f, 3.43f,  0,   0.0f,  "No coil; retract to 3.43m"},
    {"12m",   24.940f,   2.92f, 2.92f,  0,   0.0f,  "No coil; retract to 2.92m"},
    {"10m",   28.500f,   2.55f, 2.55f,  0,   0.0f,  "No coil; retract to 2.55m"},
    {"6m",    51.000f,   1.40f, 1.40f,  0,   0.0f,  "No coil; retract to 1.40m; 2 sections"},
    {"2m",   146.000f,   0.488f,0.488f, 0,   0.0f,  "No coil; retract to 0.488m; 1 section"},
    {"1.25m",222.000f,   0.321f,0.321f, 0,   0.0f,  "No coil; retract to 0.321m; mark on section 1"},
    {"70cm", 446.000f,   0.160f,0.160f, 0,   0.0f,  "No coil; retract to 0.160m; tip section only"},
    {"33cm", 906.000f,   0.079f,0.079f, 0,   0.0f,  "Use stub antenna; 79mm element"},
};

#define NUM_BANDS 15
