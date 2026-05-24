#pragma once
// ============================================================
// config.h — TM-ADF-005 Rev A
// Motorized Rotary Loop RDF Controller
// ============================================================

// --- Stepper motor (DRV8825 or TMC2208) --------------------
#define GPIO_MOTOR_STEP     26
#define GPIO_MOTOR_DIR      27
#define GPIO_MOTOR_EN       14   // active LOW

// --- Position and signal sensors ---------------------------
#define GPIO_POS_ADC        36   // ADC1_CH0 — position potentiometer
#define GPIO_SIG_ADC        39   // ADC1_CH3 — signal level from receiver

// --- Sense antenna relay -----------------------------------
#define GPIO_SENSE_RELAY    15   // DPDT relay: loop-only vs. cardioid
#define GPIO_SENSE_PHASE    4    // Second relay: 0° vs 180° sense phase

// --- Tuning DAC (varactor for loop resonance) --------------
#define GPIO_TUNE_DAC       25   // DAC output 0–3.3V → varactor

// --- Band switching relays (3 bits = 8 bands) --------------
#define GPIO_BAND_R0        16
#define GPIO_BAND_R1        17
#define GPIO_BAND_R2        18

// --- I2C + GPS + BT ----------------------------------------
#define GPIO_I2C_SDA        21
#define GPIO_I2C_SCL        22
#define GPIO_GPS_RX         5
#define GPIO_GPS_TX         19
#define GPIO_BT_LED         2
#define GPS_BAUD            9600
#define GPS_UART_NUM        1    // UART1

// ============================================================
// Motor Parameters
// ============================================================
#define STEPS_PER_MOTOR_REV  3200    // 200 steps × 16 microstepping
#define GEAR_RATIO           3       // 20T → 60T belt reduction
#define STEPS_PER_LOOP_REV   (STEPS_PER_MOTOR_REV * GEAR_RATIO)  // 9600
#define DEG_PER_STEP         (360.0f / (float)STEPS_PER_LOOP_REV) // 0.0375°
#define MAX_STEP_SPS         800     // max steps/sec (limited by motor + mechanics)
#define ACCEL_SPS2           200     // acceleration steps/sec²
#define HOME_SPEED_SPS       200     // slow speed for homing

// ============================================================
// Scan Parameters
// ============================================================
#define SCAN_STEP_DEG        2.0f    // degrees per scan step
#define SCAN_DWELL_MS        150     // dwell at each step (ms)
#define FINE_STEP_DEG        0.5f    // fine scan resolution
#define NULL_WINDOW_DEG      30.0f   // search window for null refinement
#define MIN_NULL_DEPTH_DB    6.0f    // minimum null depth to report

// ============================================================
// Signal Level (AD8307 log amp calibration)
// ============================================================
#define SIG_CAL_SLOPE        (87.0f / 4095.0f)   // dB per ADC count (AD8307: 87dB / 4096)
#define SIG_CAL_OFFSET       (-75.0f)             // dBm at ADC=0

// ============================================================
// Band Table for Loop
// ============================================================
struct LoopBand {
    const char* name;
    uint32_t    freq_hz;
    float       tune_dac_v;   // varactor voltage for band center (calibrate!)
    uint8_t     relay_bits;   // band relay combination
};

#define NUM_LOOP_BANDS 8

static const LoopBand LOOP_BANDS[NUM_LOOP_BANDS] = {
    { "160M",  1850000UL, 0.3f, 0 },
    { " 80M",  3750000UL, 0.5f, 1 },
    { " 60M",  5350000UL, 0.8f, 2 },
    { " 40M",  7150000UL, 1.1f, 3 },
    { " 30M", 10125000UL, 1.5f, 4 },
    { " 20M", 14175000UL, 2.0f, 5 },
    { " 17M", 18118000UL, 2.5f, 6 },
    { " 15M", 21225000UL, 3.0f, 7 },
};

// ============================================================
// WiFi / BT
// ============================================================
#define WIFI_SSID  "RF-LoopRDF"
#define WIFI_PASS  ""
#define BT_NAME    "RF-LoopRDF"
#define HTTP_PORT  80
