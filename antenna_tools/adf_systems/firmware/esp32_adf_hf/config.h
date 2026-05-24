#pragma once

// ============================================================
// config.h — TM-ADF-001 Rev A
// Watson-Watt HF ADF Controller
// ESP32-WROOM-32 pin assignments and constants
// ============================================================

// --- ADC Inputs (all ADC1, usable with WiFi active) --------
#define GPIO_NS_ADC         34   // ADC1_CH6 — N-S pair signal level
#define GPIO_EW_ADC         35   // ADC1_CH7 — E-W pair signal level
#define GPIO_SENSE_ADC      32   // ADC1_CH4 — Sense/omni antenna level

// --- RF Switch Control (SP4T, PE4259 / HMC253) -------------
#define GPIO_RF_SW_A        16   // Switch control bit 0
#define GPIO_RF_SW_B        17   // Switch control bit 1
#define GPIO_RF_SW_EN       18   // Switch enable, active LOW

// --- GPS (UART2) -------------------------------------------
#define GPIO_GPS_RX         4    // UART2 RX ← NEO-8M TX
#define GPIO_GPS_TX         5    // UART2 TX → NEO-8M RX
#define GPIO_GPS_PPS        15   // 1PPS input, rising edge
#define GPS_BAUD            9600
#define GPS_UART_NUM        2

// --- I2C (OLED + QMC5883L compass + MPU6050) ---------------
#define GPIO_I2C_SDA        21
#define GPIO_I2C_SCL        22
#define OLED_I2C_ADDR       0x3C
#define COMPASS_I2C_ADDR    0x0D  // QMC5883L
#define IMU_I2C_ADDR        0x68  // MPU6050

// --- Status LEDs -------------------------------------------
#define GPIO_BT_LED         2
#define GPIO_SIG_LED        13   // Signal-present indicator

// --- DAC audio output (optional audio monitor) -------------
#define GPIO_AUDIO_DAC      25   // DAC1

// --- Rotary loop motor (optional, loop mode) ---------------
#define GPIO_LOOP_STEP      26
#define GPIO_LOOP_DIR       27
#define GPIO_LOOP_EN        14   // active LOW

// ============================================================
// ADC Configuration
// ============================================================
#define ADC_SAMPLE_HZ       10000   // 10 kHz sample rate
#define ADC_BLOCK_SIZE      512     // samples per measurement block
#define ADC_ATTENUATION     ADC_ATTEN_DB_11  // 0–3.3V range
#define ADC_VREF_MV         3300
#define ADC_BITS            12
#define ADC_MAX_COUNT       4095

// ============================================================
// Watson-Watt Algorithm Parameters
// ============================================================
#define WW_BEARING_AVG      8       // readings to average per output
#define WW_MIN_SIGNAL       100     // ADC counts, below = no signal
#define WW_SNR_THRESHOLD    6.0f    // dB, minimum SNR for valid bearing
#define WW_SMOOTH_ALPHA     0.3f    // EMA smoothing factor (0=heavy, 1=none)
#define WW_HISTORY_SIZE     32      // circular bearing history buffer

// ============================================================
// RF Switch Selection Codes
// ============================================================
#define RF_SW_NS            0x00    // A=0, B=0 → N-S pair on ADC
#define RF_SW_EW            0x01    // A=1, B=0 → E-W pair on ADC
#define RF_SW_SENSE         0x02    // A=0, B=1 → Sense antenna on ADC
#define RF_SW_OFF           0x03    // A=1, B=1 → disconnected

// ============================================================
// Band Table
// ============================================================
struct BandConfig {
    const char* name;       // display name, e.g. "40M"
    uint32_t    freq_hz;    // center frequency in Hz
    float       lambda_m;   // wavelength in meters
    float       spacing_m;  // element spacing (lambda/4)
    float       delay_m;    // 90-deg delay coax length (RG-58, VF=0.66)
};

#define NUM_HF_BANDS 11

static const BandConfig HF_BANDS[NUM_HF_BANDS] = {
    { "160M", 1850000UL,  162.2f, 40.5f, 26.7f },
    { " 80M", 3750000UL,   80.0f, 20.0f, 13.2f },
    { " 60M", 5350000UL,   56.1f, 14.0f,  9.2f },
    { " 40M", 7150000UL,   41.9f, 10.5f,  6.9f },
    { " 30M",10125000UL,   29.6f,  7.4f,  4.9f },
    { " 20M",14175000UL,   21.2f,  5.3f,  3.5f },
    { " 17M",18118000UL,   16.6f,  4.1f,  2.7f },
    { " 15M",21225000UL,   14.1f,  3.5f,  2.3f },
    { " 12M",24940000UL,   12.0f,  3.0f,  2.0f },
    { " 10M",28850000UL,   10.4f,  2.6f,  1.7f },
    { "  6M",52000000UL,    5.8f,  1.4f,  0.9f },
};

// ============================================================
// WiFi / Bluetooth
// ============================================================
#define WIFI_SSID       "RF-ADFHF"
#define WIFI_PASS       ""
#define WIFI_IP         "192.168.4.1"
#define HTTP_PORT       80

#define BT_DEVICE_NAME  "RF-ADFHFinder"
#define BT_BAUD         115200

// ============================================================
// OLED Display
// ============================================================
#define OLED_WIDTH      128
#define OLED_HEIGHT     64
#define OLED_RESET      -1

// ============================================================
// NVS Key Names
// ============================================================
#define NVS_NAMESPACE   "adf_hf"
#define NVS_NS_GAIN     "ns_gain"     // float, N-S gain correction
#define NVS_EW_GAIN     "ew_gain"     // float, E-W gain correction
#define NVS_PHASE_CORR  "phase_corr"  // float, degrees
#define NVS_BAND        "band_idx"    // uint8_t, last band selected
#define NVS_DECL        "declination" // float, magnetic declination
#define NVS_HI_X        "hard_iron_x" // int16_t, compass calibration
#define NVS_HI_Y        "hard_iron_y"

// ============================================================
// Timing
// ============================================================
#define BEARING_UPDATE_MS   500   // bearing measurement interval
#define OLED_UPDATE_MS      250   // display refresh rate
#define BT_SEND_MS          1000  // Bluetooth telemetry interval
#define GPS_UPDATE_MS       100   // GPS NMEA parse interval
