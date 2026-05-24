#pragma once
// ============================================================
// config.h — TM-ADF-002 Rev A
// Doppler VHF/UHF ADF Controller
// ============================================================

// --- Element switching (FST3253 / 74HC4052) -----------------
#define GPIO_SW_A0          16   // Element select bit 0
#define GPIO_SW_A1          17   // Element select bit 1
#define GPIO_SW_ENABLE      18   // Switch enable (active LOW)

// --- Reference output and audio input ----------------------
#define GPIO_REF_OUT        26   // 1 kHz reference (LEDC PWM)
#define GPIO_AUDIO_ADC      34   // ADC1_CH6 — FM discriminator audio
#define GPIO_RSSI_ADC       35   // ADC1_CH7 — received signal strength

// --- GPS (UART2) -------------------------------------------
#define GPIO_GPS_RX          4
#define GPIO_GPS_TX          5
#define GPIO_GPS_PPS        15
#define GPS_BAUD            9600
#define GPS_UART_NUM         2

// --- I2C (OLED + compass) ----------------------------------
#define GPIO_I2C_SDA        21
#define GPIO_I2C_SCL        22
#define OLED_I2C_ADDR       0x3C
#define COMPASS_I2C_ADDR    0x0D

// --- Status LED --------------------------------------------
#define GPIO_BT_LED          2

// ============================================================
// Doppler Parameters
// ============================================================
#define DOPPLER_SWITCH_HZ   1000     // Element switching rate (Hz)
#define DOPPLER_ADC_HZ      10000    // Audio ADC sample rate (Hz)
#define DOPPLER_BLOCK_SIZE  1024     // Samples per Goertzel block
#define DOPPLER_AVG_COUNT   8        // Averages per output bearing
#define DOPPLER_MIN_RSSI    200      // ADC counts min for valid bearing

// ============================================================
// Band Table
// ============================================================
struct VHFBandConfig {
    const char* name;
    uint32_t    freq_mhz;
    float       lambda_mm;
    float       array_diam_mm;   // 0.5λ diameter circle
    float       element_len_mm;  // λ/4 monopole length
    uint16_t    switch_hz;       // recommended switching rate
};

#define NUM_VHF_BANDS 5

static const VHFBandConfig VHF_BANDS[NUM_VHF_BANDS] = {
    { "2M",   146,  2055.0f,  1027.0f, 514.0f, 1000 },
    { "1.25M",222,  1351.0f,   675.0f, 338.0f, 1000 },
    { "70CM", 435,   690.0f,   345.0f, 172.0f, 1000 },
    { "33CM", 915,   328.0f,   164.0f,  82.0f, 2000 },
    { "20CM",1270,   236.0f,   118.0f,  59.0f, 2000 },
};

// ============================================================
// WiFi / Bluetooth
// ============================================================
#define WIFI_SSID       "RF-ADFVHF"
#define WIFI_PASS       ""
#define BT_DEVICE_NAME  "RF-ADFVHFinder"
#define HTTP_PORT        80
