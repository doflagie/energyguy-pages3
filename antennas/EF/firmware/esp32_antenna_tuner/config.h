// ============================================================
//  config.h — EFQW Antenna Tuner / SWR Monitor
//  TM-EF-FW-001 Rev A
//  ESP32-WROOM-32 with Bruene directional coupler + AD8307
// ============================================================

#pragma once

// --- GPIO assignments ---
#define PIN_FWD_ADC       35    // AD8307 forward power output (ADC1_CH7)
#define PIN_REFL_ADC      39    // AD8307 reflected power output (ADC1_CH3, input-only)
#define PIN_TUNE_BTN      0     // Boot/tune button (active low, internal pull-up)
#define PIN_BAND_UP       4     // Band select up
#define PIN_BAND_DN       5     // Band select down
#define PIN_LED_STATUS    2     // Onboard LED (active high)
#define PIN_LED_SWR_OK    17    // Green LED: SWR < 1.5 (active high)
#define PIN_LED_SWR_WARN  16    // Yellow LED: SWR 1.5–2.5
#define PIN_LED_SWR_HIGH  18    // Red LED: SWR > 2.5

// --- AD8307 calibration ---
// V_out = 25 mV/dB; intercept at −84 dBm
// P_dBm = (V_mV / 25.0) − 84.0
// ADC: 12-bit, 0–3.3V reference → V_mV = (adc_raw / 4095.0) * 3300.0
#define AD8307_MV_PER_DB    25.0f
#define AD8307_INTERCEPT    84.0f   // dBm (subtract to get P_dBm)
#define ADC_VREF_MV         3300.0f
#define ADC_BITS            4095.0f
#define ADC_OVERSAMPLE      64      // hardware oversampling count per reading

// Coupling factor of directional coupler (dB, positive)
// Bruene with 10T secondary on FT-37-43: ~20 dB coupling
// Adjust based on actual coupler calibration
#define COUPLER_FWD_DB      20.0f
#define COUPLER_REFL_DB     20.0f

// Power correction (dBm): apply after coupler correction
// Set to 0.0 initially; calibrate against known reference
#define CAL_OFFSET_FWD      0.0f
#define CAL_OFFSET_REFL     0.0f

// --- SWR thresholds ---
#define SWR_GOOD            1.5f
#define SWR_WARN            2.5f
#define SWR_MAX_DISPLAY     10.0f

// --- ADC averaging ---
#define SWR_AVERAGE_N       8       // readings to average for display stability

// --- WiFi Access Point ---
#define WIFI_SSID           "EFQW-Tuner"
#define WIFI_PASS           "antenna73"
#define WIFI_CHANNEL        6
#define WIFI_IP             "192.168.4.1"

// --- Bluetooth Serial ---
#define BT_DEVICE_NAME      "EFQW-Tuner"

// --- Band table (15 bands) ---
// Columns: name, center_freq_MHz, qw_length_m, matching (0=L-net, 1=gamma, 2=direct)
struct BandEntry {
    const char* name;
    float       freq_mhz;
    float       qw_m;
    uint8_t     match_type;
};

#define NUM_BANDS 15
static const BandEntry BANDS[NUM_BANDS] = {
    {"160m",  1.900f,  38.29f,  0},
    {"80m",   3.750f,  19.40f,  0},
    {"60m",   5.330f,  13.67f,  0},
    {"40m",   7.150f,  10.18f,  0},
    {"30m",  10.125f,   7.19f,  0},
    {"20m",  14.175f,   5.13f,  0},
    {"17m",  18.118f,   4.01f,  0},
    {"15m",  21.225f,   3.43f,  0},
    {"12m",  24.940f,   2.92f,  0},
    {"10m",  28.500f,   2.55f,  0},
    {"6m",   51.000f,   1.40f,  1},
    {"2m",  146.000f,   0.488f, 1},
    {"1.25m",222.000f,  0.321f, 1},
    {"70cm", 446.000f,  0.160f, 2},
    {"33cm", 906.000f,  0.0785f,2},
};

// --- Web server port ---
#define HTTP_PORT           80

// --- BT packet interval (ms) ---
#define BT_INTERVAL_MS      200

// --- NVS namespace ---
#define NVS_NAMESPACE       "efqw"
#define NVS_KEY_CAL_FWD     "cal_fwd"
#define NVS_KEY_CAL_REFL    "cal_refl"
#define NVS_KEY_BAND        "band_idx"

// --- Low-power RF threshold (dBm); below this, SWR reading unreliable ---
#define MIN_FWD_DBM        -20.0f
