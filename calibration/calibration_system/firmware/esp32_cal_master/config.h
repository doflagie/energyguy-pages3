/**
 * config.h — TM-CAL-FW-001
 * ESP32 Calibration Master — Configuration
 */

#pragma once

// ── GPIO Assignments ──────────────────────────────────────────────────────────
// GPS module (NEO-M8N)
#define GPIO_GPS_TX       16    // ESP32 UART2 RX ← GPS TX (NMEA)
#define GPIO_GPS_RX       17    // ESP32 UART2 TX → GPS RX (config commands)
#define GPIO_GPS_PPS      34    // 1PPS input (interrupt, rising edge)

// Si5351A (I2C shared bus)
#define GPIO_I2C_SDA      21
#define GPIO_I2C_SCL      22

// Frequency counter input
#define GPIO_PCNT_INPUT   32    // direct count input (1 Hz – 40 MHz)
#define GPIO_PCNT_DIV10   33    // ÷10 prescaler output (up to 400 MHz)
#define GPIO_RANGE_SEL    25    // HIGH = direct; LOW = ÷10 prescaler

// AD8307 RSSI
#define GPIO_RSSI_ADC     36    // ADC1_CH0 (VP); no pullup; input only

// LEDs
#define GPIO_LED_GPS       2    // green: GPS fix status
#define GPIO_LED_LOCK      4    // blue:  discipline lock status

// ── Si5351A Configuration ─────────────────────────────────────────────────────
#define SI5351_XTAL_FREQ  25000000UL    // 25 MHz reference crystal/TCXO
#define SI5351_CLK0_FREQ  10000000UL    // 10 MHz reference output
#define SI5351_CLK1_FREQ   1000000UL    // 1 MHz reference output
#define SI5351_CLK0_MA    SI5351_DRIVE_8MA
#define SI5351_CLK1_MA    SI5351_DRIVE_8MA
#define SI5351_CLK2_MA    SI5351_DRIVE_8MA

// ── GPS Discipline Loop ───────────────────────────────────────────────────────
#define GATE_SECONDS      10            // PPS intervals per measurement window
#define EXPECTED_COUNTS   100000000L    // 10 MHz × 10 s
#define LOCK_PPB_THRESH   1.0f          // ppb threshold for LOCK status
#define LOCK_CONSEC_WIN   3             // consecutive windows within threshold
#define DISCIPLINE_KP     0.5f          // proportional gain (0.1–1.0)
#define MAX_CORRECTION   50000L         // max correction ±500 ppb in 0.01ppb units

// ── Frequency Counter ─────────────────────────────────────────────────────────
#define COUNTER_GATE_SEC  1             // default gate: 1 second
#define COUNTER_GATE_10S  10            // extended gate: 10 seconds (0.1 Hz res)

// ── AD8307 Power Meter ────────────────────────────────────────────────────────
#define RSSI_SLOPE_MV_DB  25.0f         // mV per dB (AD8307 typical)
#define RSSI_INTERCEPT    -84.0f        // dBm at 0V output
#define RSSI_AVG_SAMPLES  32            // ADC averaging for stability
#define RSSI_VREF_MV     3300.0f        // ESP32 ADC reference voltage (mV)
#define RSSI_ADC_BITS     4095.0f       // 12-bit ADC full scale
#define CAL_OFFSET_KEY    "cal_offset"  // NVS Preferences key for offset

// ── Programmable Output Presets ───────────────────────────────────────────────
struct FreqPreset {
    const char *label;
    uint64_t   freq_cHz;   // frequency in centiherts (Hz × 100) for si5351 lib
};

static const FreqPreset FREQ_PRESETS[] = {
    { "100kHz",    10000000ULL  },
    { "1MHz",     100000000ULL  },
    { "10MHz",   1000000000ULL  },
    { "14.1MHz", 1410000000ULL  },
    { "25MHz",   2500000000ULL  },
    { "50MHz",   5000000000ULL  },
    { "100MHz", 10000000000ULL  },
    { "144MHz", 14400000000ULL  },
};
#define NUM_FREQ_PRESETS  (sizeof(FREQ_PRESETS) / sizeof(FREQ_PRESETS[0]))

// ── WiFi AP ───────────────────────────────────────────────────────────────────
#define WIFI_SSID         "CAL-Station"
#define WIFI_PASS         "gpsdo2024"
#define WIFI_IP           "192.168.4.1"

// ── Bluetooth ─────────────────────────────────────────────────────────────────
#define BT_DEVICE_NAME    "CAL-Master"

// ── Calibration Log ──────────────────────────────────────────────────────────
#define CAL_LOG_ENTRIES   50            // rolling log in PSRAM/RAM
#define CAL_LOG_KEY       "callog"

// ── Serial Debug ──────────────────────────────────────────────────────────────
#define SERIAL_BAUD       115200
#define GPS_UART_BAUD     9600          // NEO-M8N default; firmware upgrades to 115200
