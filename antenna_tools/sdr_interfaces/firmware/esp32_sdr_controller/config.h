#pragma once
// config.h — TM-SDR-FW-001 Rev A
// ESP32 SDR Controller: GPIO assignments, band table, system constants

#include <Arduino.h>

// ── GPIO assignments ───────────────────────────────────────────────────────
// Preselector BPF decoder (74HC154)
#define GPIO_BPF_A0     25
#define GPIO_BPF_A1     26
#define GPIO_BPF_A2     27
#define GPIO_BPF_A3     14
#define GPIO_BPF_EN     33   // HIGH = enable decoder

// Attenuator SPI (PE4302, VSPI)
#define GPIO_ATT_CLK    18   // VSPI CLK
#define GPIO_ATT_DATA   23   // VSPI MOSI
#define GPIO_ATT_LE      5   // Latch Enable (active-high pulse to commit)

// Relay controls
#define GPIO_LNA        2    // HIGH = LNA relay engaged
#define GPIO_BYPASS     4    // HIGH = upconverter bypassed (direct to SDR)
#define GPIO_BIASTEE   16    // HIGH = bias-T DC enabled
#define GPIO_BPFSEL    17    // HIGH = preselector engaged; LOW = wideband

// ADC input
#define GPIO_RSSI      34    // AD8307 output (ADC1_CH6, input-only pin)

// I2C (optional OLED)
#define GPIO_SDA       21
#define GPIO_SCL       22

// ── Band definitions ───────────────────────────────────────────────────────
#define NUM_BANDS      16

struct BandConfig {
    const char *name;       // e.g. "160M"
    uint32_t   freq_lo;     // lower edge, Hz
    uint32_t   freq_hi;     // upper edge, Hz
    uint32_t   freq_center; // center, Hz
    uint8_t    bpf_code;    // 0–15: 74HC154 address; 15 = bypass
    bool       use_upconv;  // true = 125 MHz LO upconverter required
    int8_t     default_att; // dB, suggested initial attenuation
    bool       lna_default; // true = enable LNA by default for this band
};

static const BandConfig BANDS[NUM_BANDS] = {
    // name    freq_lo       freq_hi      center       code  upconv  att  lna
    { "160M",  1800000UL,   2000000UL,  1900000UL,    0,    false,   0, true  },
    { "80M",   3500000UL,   4000000UL,  3750000UL,    1,    false,   0, true  },
    { "40M",   7000000UL,   7300000UL,  7150000UL,    2,    false,   0, true  },
    { "30M",  10100000UL,  10150000UL, 10125000UL,    3,    false,   0, true  },
    { "20M",  14000000UL,  14350000UL, 14175000UL,    4,    false,   0, true  },
    { "17M",  18068000UL,  18168000UL, 18118000UL,    5,    true,    0, true  },
    { "15M",  21000000UL,  21450000UL, 21225000UL,    6,    true,    0, true  },
    { "12M",  24890000UL,  24990000UL, 24940000UL,    7,    true,    0, true  },
    { "10M",  28000000UL,  29700000UL, 28850000UL,    8,    true,    0, true  },
    { "6M",   50000000UL,  54000000UL, 52000000UL,    9,    false,   0, true  },
    { "2M",  144000000UL, 148000000UL,146000000UL,   10,    false,   0, true  },
    { "1.25M",222000000UL, 225000000UL,223500000UL,  11,    false,   0, true  },
    { "70cm", 420000000UL, 450000000UL,435000000UL,  12,    false,   0, true  },
    { "33cm", 902000000UL, 928000000UL,915000000UL,  13,    false,  10, false },
    { "23cm",1240000000UL,1300000000UL,1270000000UL, 14,    false,  10, false },
    { "WIDE",         0UL,          0UL,        0UL, 15,    false,   0, false }, // bypass
};

// ── PE4302 attenuator ─────────────────────────────────────────────────────
#define ATT_MIN_DB   0
#define ATT_MAX_DB   31
#define ATT_STEP_DB  1      // round to nearest 1 dB in UI; HW supports 0.5 dB

// ── ALC (automatic level control) ────────────────────────────────────────
#define ALC_TARGET_DBM    (-30)   // target SDR input level
#define ALC_UPPER_DBM     (-15)   // increase attenuation threshold
#define ALC_LOWER_DBM     (-50)   // decrease attenuation threshold
#define ALC_HYSTERESIS_DB  5
#define ALC_POLL_MS       100

// ── AD8307 RSSI calibration ───────────────────────────────────────────────
// RSSI_dBm = (V_adc / 0.025) - 84.0
// V_adc = analogRead(GPIO_RSSI) × (3.3 / 4095.0)
#define AD8307_SLOPE_V_PER_DB  0.025f
#define AD8307_INTERCEPT_DBM  (-84.0f)

// ── WiFi ──────────────────────────────────────────────────────────────────
#define WIFI_SSID    "SDR-Controller"
#define WIFI_PASS    "sdrcontrol"
#define WIFI_IP      "192.168.4.1"

// ── Bluetooth ─────────────────────────────────────────────────────────────
#define BT_NAME      "RF-SDRCtrl"

// ── OLED ──────────────────────────────────────────────────────────────────
#define OLED_ADDR    0x3C
#define OLED_WIDTH   128
#define OLED_HEIGHT   32

// ── Firmware version ──────────────────────────────────────────────────────
#define FW_VERSION   "TM-SDR-FW-001 Rev A"
#define FW_DATE      "2026-05-02"
