/**
 * config.h — TM-PWR-ESP32-001 Rev A
 * ESP32 RF Power Meter Configuration
 * Supports inline directional meter and terminating meter variants.
 * Select METER_TYPE and BAND before compiling.
 */

#pragma once

// ─── Meter Type — uncomment ONE ───────────────────────────────────────────────
#define METER_TYPE_INLINE      // Inline directional: FWD + REF, SWR computed
// #define METER_TYPE_TERMINATING // Terminating: forward power only, dummy load

// ─── Band — uncomment ONE ─────────────────────────────────────────────────────
#define BAND_HF                // 1.8–30 MHz, FT-114-43 toroid
// #define BAND_VHF            // 50–148 MHz, BN-43-202 binocular
// #define BAND_UHF            // 420–1300 MHz, coupled microstrip

// ─── Firmware ─────────────────────────────────────────────────────────────────
#define FW_VERSION          "1.0.0"
#define DEVICE_NAME         "RF-PowerMeter"
#define FW_BUILD_DATE       __DATE__

// ─── I2C ──────────────────────────────────────────────────────────────────────
#define I2C_SDA_PIN         21
#define I2C_SCL_PIN         22
#define ADS1115_ADDR        0x48
#define OLED_ADDR           0x3C   // SSD1306 fallback if no CYD

// ─── ADS1115 Channel Assignments ──────────────────────────────────────────────
#define ADC_FWD_CH          0   // A0 — average forward detector voltage
#define ADC_REF_CH          1   // A1 — average reflected detector voltage
#define ADC_PEAK_CH         2   // A2 — peak detector (BAT54 + 1µF hold)
#define ADC_TEMP_CH         3   // A3 — NTC thermistor (temp compensation)
#define ADC_PGA             GAIN_TWOTHIRDS   // ±6.144V range
#define ADC_RATE            ADS1115_DR_860SPS
#define ADC_OVERSAMPLE      16   // Average this many samples per reading

// ─── Relay / Range Pins ────────────────────────────────────────────────────────
#define RELAY_1_PIN         25   // Range relay 1 (e.g. 20dB pad)
#define RELAY_2_PIN         26   // Range relay 2 (e.g. +10dB pad)
#define RELAY_SETTLE_MS     15   // Relay switching settle time

// ─── Peak Reset ───────────────────────────────────────────────────────────────
#define PEAK_RESET_PIN      27   // GPIO → 2N7000 → C_pk drain to GND
#define PEAK_RESET_PULSE_MS 20   // Duration of peak-reset pulse

// ─── Button Pins ──────────────────────────────────────────────────────────────
#define BTN_PEAK_PIN        34   // Peak hold reset / toggle
#define BTN_AVG_PIN         35   // Averaging window cycle
#define BTN_RANGE_PIN       36   // Manual range override
#define BTN_LOG_PIN         39   // Toggle SD logging
#define BTN_DEBOUNCE_MS     50

// ─── DS18B20 Load Temperature (terminating meter) ─────────────────────────────
#define DS18B20_PIN         4    // OneWire bus GPIO
#define TEMP_ALERT_C        70.0f
#define TEMP_SHUTDOWN_C     85.0f

// ─── SD Card ──────────────────────────────────────────────────────────────────
#define SD_CS_PIN           5
#define SD_SCK_PIN          18
#define SD_MOSI_PIN         23
#define SD_MISO_PIN         19
#define SD_LOG_INTERVAL_MS  1000
#define SD_FILENAME_PREFIX  "/pwrmtr_"
#define SD_MAX_FILE_SIZE    (1024*1024*10)  // 10 MB per log file

// ─── Bluetooth ────────────────────────────────────────────────────────────────
#define BT_DEVICE_NAME      "RF-PowerMeter"
#define BT_LOG_INTERVAL_MS  500
#define BT_TX_PIN           17
#define BT_RX_PIN           16

// ─── USB Serial ───────────────────────────────────────────────────────────────
#define SERIAL_BAUD         115200
#define USB_LOG_INTERVAL_MS 200

// ─── Status LED ───────────────────────────────────────────────────────────────
#define LED_PIN             2
#define LED_NORMAL_MS       1000
#define LED_WARN_MS         250
#define LED_FAULT_MS        100

// ─── Coupler Parameters ───────────────────────────────────────────────────────
#ifdef BAND_HF
  #define COUPLING_DB       (-30.0f)
  #define FREQ_LOW_MHZ      1.8f
  #define FREQ_HIGH_MHZ     30.0f
  #define BAND_STR          "HF"
#endif
#ifdef BAND_VHF
  #define COUPLING_DB       (-26.0f)
  #define FREQ_LOW_MHZ      50.0f
  #define FREQ_HIGH_MHZ     148.0f
  #define BAND_STR          "VHF"
#endif
#ifdef BAND_UHF
  #define COUPLING_DB       (-25.0f)
  #define FREQ_LOW_MHZ      420.0f
  #define FREQ_HIGH_MHZ     1300.0f
  #define BAND_STR          "UHF"
#endif

// ─── Power Ranges (4 ranges: 1-10, 10-100, 100-1000, 1k-3kW) ────────────────
#define NUM_RANGES          4
static const float RANGE_MAX_W[]    = {10.0f, 100.0f, 1000.0f, 3000.0f};
static const float RANGE_ATTEN_DB[] = {0.0f, 20.0f, 30.0f, 35.0f};
static const char* RANGE_LABEL[]    = {"1-10W", "10-100W", "0.1-1kW", "1-3kW"};

// ─── Power Thresholds ────────────────────────────────────────────────────────
#define SWR_GOOD            1.5f
#define SWR_WARN            2.5f
#define SWR_MAX             10.0f
#define POWER_MIN_W         0.01f  // Below this: display "---"
#define VDET_MIN_MV         20.0f  // Min detector mV for valid reading
#define VDET_OVERLOAD_MV    4800.0f // ADC near-full-scale: uprange

// ─── Averaging ────────────────────────────────────────────────────────────────
#define AVG_WINDOW_FAST_MS  100
#define AVG_WINDOW_MED_MS   500
#define AVG_WINDOW_SLOW_MS  2000
#define AVG_DEFAULT         AVG_WINDOW_MED_MS

// ─── NVS ──────────────────────────────────────────────────────────────────────
#define NVS_NAMESPACE       "pwrmeter"
#define NVS_KEY_FWD_ZERO    "fwd_z"
#define NVS_KEY_REF_ZERO    "ref_z"
#define NVS_KEY_FWD_SCALE   "fwd_s"
#define NVS_KEY_REF_SCALE   "ref_s"
#define NVS_KEY_FREQ_MHZ    "freq"
#define NVS_KEY_CAL_VALID   "cal_ok"

// ─── Watchdog ─────────────────────────────────────────────────────────────────
#define WDT_TIMEOUT_S       30

// ─── Display Update Rates ─────────────────────────────────────────────────────
#define OLED_UPDATE_MS      200
#define CYD_UPDATE_MS       100    // CYD can handle 10 Hz
