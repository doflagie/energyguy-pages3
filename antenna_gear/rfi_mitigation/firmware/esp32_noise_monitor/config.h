#pragma once
// config.h — TM-RFI-FW-001 Rev A
// ESP32 RF Noise Floor Monitor: configuration constants

#include <Arduino.h>

// ── GPIO assignments ──────────────────────────────────────────────────────
#define GPIO_RSSI       34   // ADC1_CH6 ← AD8307 VOUT (input-only pin)
#define GPIO_STATUS_LED  2   // scanning activity LED
#define GPIO_ALERT_LED   4   // noise threshold alert LED
#define GPIO_SD_CS       5   // SD card chip select
// SPI: GPIO18=CLK, GPIO19=MISO, GPIO23=MOSI (SD card)
// I2C: GPIO21=SDA, GPIO22=SCL (Si5351)

// ── AD8307 calibration ────────────────────────────────────────────────────
#define AD8307_SLOPE     0.025f   // V/dB
#define AD8307_INTERCEPT (-84.0f) // dBm (output = 0V when input = −84 dBm)
// Calibration offset (measured vs. calibrated signal source):
#define AD8307_CAL_OFFSET_DB  0.0f  // adjust if measurements drift from reference

// ── Si5351 I2C ────────────────────────────────────────────────────────────
#define SI5351_I2C_ADDR  0x60
#define SI5351_REF_XTAL  25000000UL   // 25 MHz reference crystal

// ── IF frequency ─────────────────────────────────────────────────────────
#define IF_FREQ_HZ       10700000UL   // 10.7 MHz

// ── Band scan table ───────────────────────────────────────────────────────
#define NUM_SCAN_BANDS   11

struct ScanBand {
    const char   *name;
    uint32_t      center_hz;        // RF center frequency to measure
    float         threshold_dBm;    // noise alert threshold
    bool          use_upconv;       // true = LO = center + IF; false = LO below center
};

// LO = center_hz + IF_FREQ_HZ (high-side injection)
// f_IF = f_LO - f_RF → f_RF = f_LO - f_IF = center_hz (correct)
static const ScanBand SCAN_BANDS[NUM_SCAN_BANDS] = {
    // name    center_hz      threshold  upconv
    { "160M",  1900000UL,    -100.0f,   false },
    { "80M",   3750000UL,    -100.0f,   false },
    { "40M",   7150000UL,    -100.0f,   false },
    { "30M",  10125000UL,    -100.0f,   false },
    { "20M",  14175000UL,    -100.0f,   false },
    { "17M",  18118000UL,    -100.0f,   false },
    { "15M",  21225000UL,    -100.0f,   false },
    { "12M",  24940000UL,    -100.0f,   false },
    { "10M",  28850000UL,    -100.0f,   false },
    { "6M",   52000000UL,    -100.0f,   false },
    { "2M",  146000000UL,    -100.0f,   false },
};

// ── Scan parameters ───────────────────────────────────────────────────────
#define SCAN_DWELL_MS     2000   // time on each band before reading RSSI
#define RSSI_SAMPLES        16   // ADC averages per reading
#define HISTORY_DEPTH       96   // data points per band in memory (96 × dwell = ~3.2 min)

// ── WiFi AP ───────────────────────────────────────────────────────────────
#define WIFI_SSID         "RFI-Monitor"
#define WIFI_PASS         "rfimonitor"
#define WIFI_IP           "192.168.4.1"

// ── Bluetooth ─────────────────────────────────────────────────────────────
#define BT_NAME           "RF-NoiseMonitor"

// ── SD card logging ───────────────────────────────────────────────────────
#define LOG_FILENAME      "/noise_log.csv"
#define LOG_INTERVAL_MIN  5   // minutes between SD writes

// ── Firmware version ──────────────────────────────────────────────────────
#define FW_VERSION        "TM-RFI-FW-001 Rev A"
#define FW_DATE           "2026-05-02"
