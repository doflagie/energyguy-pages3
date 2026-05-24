// ============================================================
//  config.h — Pool Noodle Non-Loop Antenna Tuner
//  TM-PNL2-FW-001 Rev A
//  ESP32-WROOM-32
//  PNILD (HF, 160m–10m): relay-switched loading coil taps
//  J-Pole (VHF, 6m–1.25m): no tuning; SWR measurement only
//  Yagi (UHF, 70cm–20cm): no tuning; SWR measurement only
//  Bruene coupler + dual AD8307 for SWR/power
// ============================================================

#pragma once
#include <Arduino.h>

// ---- GPIO: SWR Measurement ----
#define PIN_FWD_ADC      35    // AD8307 #1 forward output (ADC1_CH7)
#define PIN_REFL_ADC     39    // AD8307 #2 reflected output (ADC1_CH3)

// ---- GPIO: Relay Control (74HC595 shift register) ----
// 9 relays select loading coil tap for 9 HF bands (160m–10m)
#define PIN_SR_DATA      25    // 74HC595 SER (data)
#define PIN_SR_CLOCK     26    // 74HC595 SRCLK (shift clock)
#define PIN_SR_LATCH     27    // 74HC595 RCLK (storage/latch clock)
#define PIN_SR_OE        14    // 74HC595 /OE (active LOW output enable)
#define N_RELAYS         9     // one per HF band tap

// ---- GPIO: Buttons ----
#define PIN_TUNE_BTN     0     // Auto-tune / manual SWR check (active LOW, boot pin)
#define PIN_BAND_UP      4     // Band select up
#define PIN_BAND_DN      5     // Band select down

// ---- GPIO: LEDs ----
#define PIN_LED_STATUS   2     // Blue status LED (onboard)
#define PIN_LED_SWR_OK   17    // Green: SWR < SWR_GOOD
#define PIN_LED_SWR_WARN 16    // Yellow: SWR_GOOD ≤ SWR < SWR_WARN
#define PIN_LED_SWR_HIGH 18    // Red: SWR ≥ SWR_WARN

// ---- AD8307 ----
#define AD8307_MV_PER_DB     25.0f
#define AD8307_INTERCEPT     84.0f
#define ADC_VREF_MV          3300.0f
#define ADC_BITS             4095.0f
#define ADC_OVERSAMPLE       64
#define SWR_AVERAGE_N        8
#define COUPLER_FWD_DB       20.0f
#define COUPLER_REFL_DB      20.0f
#define CAL_OFFSET_FWD       0.0f
#define CAL_OFFSET_REFL      0.0f
#define MIN_FWD_DBM          -20.0f

// ---- SWR Thresholds ----
#define SWR_GOOD         1.5f
#define SWR_WARN         2.5f
#define SWR_MAX_DISPLAY  10.0f

// ---- Wi-Fi ----
#define WIFI_SSID        "PNL-Tuner"
#define WIFI_PASS        "antenna73"
#define WIFI_CHANNEL     6

// ---- Bluetooth ----
#define BT_DEVICE_NAME   "PNL-Tuner"
#define BT_INTERVAL_MS   200

// ---- NVS ----
#define NVS_NAMESPACE    "pnl"
#define NVS_KEY_BAND     "band_idx"
#define NVS_KEY_CAL_FWD  "cal_fwd"
#define NVS_KEY_CAL_REFL "cal_refl"

// ---- HTTP ----
#define HTTP_PORT        80

// ---- Frame Rate ----
#define FPS_INTERVAL_MS  100

// ---- Antenna Type Enum ----
enum AntType : uint8_t {
    ANT_PNILD = 0,   // HF center-loaded shortened dipole
    ANT_JPOLE = 1,   // VHF J-pole (no tuning)
    ANT_YAGI  = 2    // UHF Yagi (no tuning)
};

// ---- Tuner State Enum ----
enum TunerState : uint8_t {
    ST_IDLE    = 0,
    ST_TUNING  = 1,   // relay scanning for best SWR
    ST_LOCKED  = 2,   // relay set, SWR < SWR_GOOD
    ST_PASSTHRU = 3   // J-pole / Yagi — SWR monitor only, no relay action
};

// ---- Band Table ----
struct BandEntry {
    const char* name;
    float       freq_mhz;
    AntType     ant_type;
    float       L_uH;          // loading coil inductance (PNILD only)
    float       h_arm_m;       // physical arm length (PNILD only)
    float       efficiency_pct;// computed efficiency
    float       bandwidth_khz; // -3dB bandwidth estimate
    uint8_t     relay_tap;     // relay index 0–8 (PNILD); 0xFF = not applicable
    const char* match_note;
};

#define NUM_BANDS 15

static const BandEntry BANDS[NUM_BANDS] = {
    // ---- HF PNILD Bands (160m–10m) ----
    {"160m",  1.900f,  ANT_PNILD, 180.35f, 12.0f, 44.0f,  2.1f,  0,
     "Low efficiency; use CW/FT8. Deploy radials. Check coil Q."},
    {"80m",   3.750f,  ANT_PNILD,  72.68f,  7.0f, 56.1f,  4.2f,  1,
     "Moderate efficiency. Inverted-V preferred. Radials help."},
    {"40m",   7.150f,  ANT_PNILD,  24.17f,  5.0f, 77.4f,  8.9f,  2,
     "Good portable DX dipole. 10m total length. Coil Q critical."},
    {"30m",  10.125f,  ANT_PNILD,  13.61f,  4.0f, 83.9f, 10.8f,  3,
     "Excellent efficiency at 30m. Narrow bandwidth — retune for ±5kHz."},
    {"20m",  14.175f,  ANT_PNILD,   8.61f,  3.0f, 86.3f, 13.0f,  4,
     "Very good portable antenna. 6m total. Good DX capability."},
    {"17m",  18.118f,  ANT_PNILD,   5.83f,  2.5f, 88.8f, 17.9f,  5,
     "High efficiency. 5m total length. Good for portable ops."},
    {"15m",  21.225f,  ANT_PNILD,   4.60f,  2.2f, 90.0f, 22.0f,  6,
     "Excellent efficiency. 4.4m total. Comparable to full dipole."},
    {"12m",  24.940f,  ANT_PNILD,   4.13f,  1.8f, 89.0f, 27.9f,  7,
     "Very high efficiency. Small antenna. Good propagation band."},
    {"10m",  28.500f,  ANT_PNILD,   3.87f,  1.5f, 87.6f, 32.5f,  8,
     "3m total length. Minimal loading coil. High efficiency."},
    // ---- VHF J-Pole Bands (6m–1.25m) ----
    {"6m",   51.000f,  ANT_JPOLE,   0.0f,   0.0f, 99.0f, 4000.f, 0xFF,
     "J-pole; λ/2 radiator + λ/4 stub. VF=0.95. Adjust tap for SWR<1.5."},
    {"2m",  146.000f,  ANT_JPOLE,   0.0f,   0.0f, 99.0f,10000.f, 0xFF,
     "J-pole; radiator=97.5cm, stub=48.8cm. Start tap at 4.9cm from short."},
    {"1.25m",222.000f, ANT_JPOLE,   0.0f,   0.0f, 99.0f,22200.f, 0xFF,
     "J-pole; radiator=64.1cm, stub=32.1cm. Start tap at 3.2cm from short."},
    // ---- UHF Yagi Bands (70cm–20cm) ----
    {"70cm", 446.000f, ANT_YAGI,    0.0f,   0.0f, 99.0f, 5000.f, 0xFF,
     "3-el Yagi; 7.1 dBd gain; 20dB F/B. Pool noodle boom. Hairpin match."},
    {"33cm", 906.000f, ANT_YAGI,    0.0f,   0.0f, 99.0f,10000.f, 0xFF,
     "5-el Yagi; 9.2 dBd gain; 25dB F/B. Element accuracy ±1mm critical."},
    {"20cm",1296.000f, ANT_YAGI,    0.0f,   0.0f, 99.0f,15000.f, 0xFF,
     "7-el Yagi; 11.2 dBd gain; 28dB F/B. Element accuracy ±0.5mm critical."},
};
