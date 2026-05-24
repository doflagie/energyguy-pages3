/*
 * bms_monitor_esp32.ino
 * TM-BMS-001 Rev A — Battery Management System Monitor
 *
 * Hardware: ESP32-2432S028 (CYD) or ESP32-WROOM-32 + ILI9341
 * BMS IC:   TI BQ76920 via I2C (0x08)
 * Sensors:  INA226 × 2 (charge + discharge current), ADS1115 (NTC × 4)
 * BLE:      GATT server, custom BMS service, real-time pack telemetry
 * Features:
 *   - Per-cell voltage monitoring (BQ76920)
 *   - Coulomb counting for SOC estimation
 *   - Extended Kalman Filter (EKF) SOC refinement via OCV correlation
 *   - Passive cell balancing control
 *   - OV/UV/OT fault detection and FET control
 *   - LVD control relay output
 *   - MPPT controller charge setpoint via UART
 *   - Charge/discharge efficiency tracking (Wh, Ah)
 *   - CYD display: cell voltages, SOC bar, temperature, fault status
 *   - BLE GATT: BMS service with notify characteristics
 *   - Web server (AP mode) for configuration and data download
 */

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <math.h>

// ─── Pin Assignments ─────────────────────────────────────────────────────────
#define PIN_SDA         21    // I2C SDA (BQ76920, INA226, ADS1115)
#define PIN_SCL         22    // I2C SCL
#define PIN_BMS_ALERT   35    // BQ76920 ALERT (active LOW interrupt)
#define PIN_LVD_RELAY   32    // LVD relay output (HIGH = connected)
#define PIN_MPPT_TX     17    // UART TX to MPPT controller
#define PIN_MPPT_RX     16    // UART RX from MPPT controller
#define PIN_BALANCE_1   25    // Optional external balance FET cell 1
#define PIN_BALANCE_2   26    // Balance cell 2
#define PIN_BALANCE_3   27    // Balance cell 3
#define PIN_BALANCE_4   14    // Balance cell 4
#define PIN_BUZZER      33    // Piezo buzzer

// ─── I2C Addresses ───────────────────────────────────────────────────────────
#define BQ76920_ADDR    0x08
#define INA226_CHG      0x40  // Charge current sensor (A0=GND, A1=GND)
#define INA226_LOAD     0x41  // Load/discharge current sensor (A0=VCC, A1=GND)
#define ADS1115_ADDR    0x48  // NTC temperature ADC

// ─── BQ76920 Register Map ────────────────────────────────────────────────────
#define BQ_SYS_STAT     0x00
#define BQ_CELLBAL1     0x01
#define BQ_SYS_CTRL1    0x04
#define BQ_SYS_CTRL2    0x05
#define BQ_PROTECT1     0x06
#define BQ_PROTECT2     0x07
#define BQ_PROTECT3     0x08
#define BQ_OV_TRIP      0x09
#define BQ_UV_TRIP      0x0A
#define BQ_ADCGAIN1     0x50
#define BQ_ADCGAIN2     0x59
#define BQ_ADCOFFSET    0x51
#define BQ_VC1_HI       0x0C  // Cell 1 voltage high byte (cells 1–4 at 0x0C–0x13)
#define BQ_CC_HI        0x32  // Coulomb counter high byte

// ─── INA226 Register Map ─────────────────────────────────────────────────────
#define INA226_CONFIG   0x00
#define INA226_SHUNT    0x01
#define INA226_BUS      0x02
#define INA226_POWER    0x03
#define INA226_CURRENT  0x04
#define INA226_CAL      0x05
#define INA226_MASKEN   0x06
#define INA226_LIMIT    0x07

// ─── Pack Configuration ───────────────────────────────────────────────────────
#define N_CELLS         4
#define CHEMISTRY       0     // 0=LiFePO4, 1=LiIon
#define PACK_CAPACITY_AH 50.0f
#define SHUNT_CHG_OHM   0.002f   // 2 mΩ charge shunt
#define SHUNT_LOAD_OHM  0.002f   // 2 mΩ load shunt
#define LVD_THRESHOLD   11.5f    // LVD disconnect voltage
#define LVD_RECONNECT   12.5f    // LVD reconnect voltage

// ─── Chemistry-Specific Parameters ───────────────────────────────────────────
// LiFePO4 OCV → SOC lookup table (4S pack, 20 points)
// Interpolated by ekf_ocv_to_soc()
static const float LIFEPO4_OCV[20] = {
    10.00f, 10.50f, 11.00f, 11.50f, 12.00f, 12.50f, 12.80f,
    13.00f, 13.10f, 13.15f, 13.20f, 13.25f, 13.30f, 13.35f,
    13.40f, 13.50f, 13.70f, 13.90f, 14.10f, 14.40f
};
static const float LIFEPO4_SOC[20] = {
    0.0f,  2.0f,  4.0f,  7.0f, 10.0f, 15.0f, 20.0f,
   30.0f, 40.0f, 50.0f, 55.0f, 60.0f, 65.0f, 70.0f,
   75.0f, 80.0f, 85.0f, 90.0f, 95.0f, 100.0f
};

// Li-Ion OCV → SOC lookup (4S pack)
static const float LIION_OCV[20] = {
    12.0f, 12.4f, 12.8f, 13.2f, 13.6f, 13.8f, 14.0f,
    14.2f, 14.4f, 14.6f, 14.8f, 15.0f, 15.2f, 15.4f,
    15.6f, 15.8f, 16.0f, 16.2f, 16.4f, 16.8f
};
static const float LIION_SOC[20] = {
    0.0f,  3.0f,  8.0f, 14.0f, 20.0f, 28.0f, 36.0f,
   44.0f, 52.0f, 60.0f, 67.0f, 74.0f, 80.0f, 86.0f,
   90.0f, 93.0f, 96.0f, 98.0f, 99.0f, 100.0f
};

// ─── Display ─────────────────────────────────────────────────────────────────
TFT_eSPI  tft;
TFT_eSprite spr(&tft);
#define SCREEN_W  320
#define SCREEN_H  240

// ─── BLE UUIDs ────────────────────────────────────────────────────────────────
// Custom 128-bit UUIDs for BMS GATT service
#define BLE_BMS_SERVICE_UUID    "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define BLE_CELLV_CHAR_UUID     "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define BLE_SOC_CHAR_UUID       "beb5483e-36e1-4688-b7f5-ea07361b26a9"
#define BLE_CURRENT_CHAR_UUID   "beb5483e-36e1-4688-b7f5-ea07361b26aa"
#define BLE_TEMP_CHAR_UUID      "beb5483e-36e1-4688-b7f5-ea07361b26ab"
#define BLE_STATUS_CHAR_UUID    "beb5483e-36e1-4688-b7f5-ea07361b26ac"
#define BLE_EFFICIENCY_CHAR_UUID "beb5483e-36e1-4688-b7f5-ea07361b26ad"

BLEServer*         g_ble_server   = nullptr;
BLECharacteristic* g_char_cellv   = nullptr;
BLECharacteristic* g_char_soc     = nullptr;
BLECharacteristic* g_char_current = nullptr;
BLECharacteristic* g_char_temp    = nullptr;
BLECharacteristic* g_char_status  = nullptr;
BLECharacteristic* g_char_eff     = nullptr;
bool               g_ble_connected = false;

// ─── BMS State ───────────────────────────────────────────────────────────────
struct BMSState {
    // Cell measurements
    float cell_v[N_CELLS];    // Cell voltages (V)
    float pack_v;             // Sum of cell voltages (V)
    float i_charge;           // Charging current (A, positive = charging)
    float i_load;             // Load current (A, positive = discharge)
    float i_net;              // Net current (positive = charging)

    // Temperature
    float temp_cell;          // Cell temperature (°C)
    float temp_fet;           // FET/heatsink temperature (°C)

    // SOC estimation
    float soc_coulomb;        // Coulomb counting SOC (%)
    float soc_ekf;            // EKF refined SOC (%)
    float soc_display;        // Displayed SOC (%)
    float ah_remaining;       // Estimated Ah remaining
    float minutes_remaining;  // Estimated runtime at current load (minutes)

    // Energy accounting
    float ah_in, wh_in;       // Charge energy accumulated this session
    float ah_out, wh_out;     // Discharge energy accumulated this session
    float coulomb_eff;        // Coulombic efficiency (%)
    float energy_eff;         // Round-trip energy efficiency (%)

    // Fault flags
    bool fault_ov;            // Any cell overvoltage
    bool fault_uv;            // Any cell undervoltage
    bool fault_oc;            // Overcurrent (charge or discharge)
    bool fault_ot;            // Over-temperature
    bool fault_comm;          // BMS IC communication failure

    // FET state
    bool chg_enabled;         // Charge FET enabled
    bool dsg_enabled;         // Discharge FET enabled
    bool lvd_connected;       // LVD relay state

    // BQ76920 raw status
    uint8_t bq_stat;          // SYS_STAT register
};

BMSState g_bms = {};

// ─── EKF State ───────────────────────────────────────────────────────────────
struct EKF {
    float x;      // State estimate: SOC (0.0 – 1.0)
    float P;      // Estimation error covariance
    float Q;      // Process noise covariance
    float R;      // Measurement noise covariance
    bool  initialized;
};

EKF g_ekf = { 0.5f, 0.1f, 1e-5f, 0.01f, false };

// ─── BQ76920 Calibration ─────────────────────────────────────────────────────
float g_adc_gain   = 365.0f;  // µV/LSB; read from BQ76920 registers at startup
float g_adc_offset = 0.0f;    // mV; read from BQ76920 register 0x51

// ─── Global Accumulators ─────────────────────────────────────────────────────
unsigned long g_last_update_ms  = 0;
unsigned long g_last_nvs_save   = 0;
Preferences   g_prefs;

// ─── Web Server ──────────────────────────────────────────────────────────────
WebServer g_server(80);
HardwareSerial mpptSerial(1);  // UART1 for MPPT controller

// ─────────────────────────────────────────────────────────────────────────────
// SETUP
// ─────────────────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    Wire.begin(PIN_SDA, PIN_SCL);
    mpptSerial.begin(9600, SERIAL_8N1, PIN_MPPT_RX, PIN_MPPT_TX);

    // Output pins
    pinMode(PIN_LVD_RELAY, OUTPUT);
    digitalWrite(PIN_LVD_RELAY, HIGH);  // Start with load connected

    for (int i = 0; i < 4; i++) {
        int pins[] = { PIN_BALANCE_1, PIN_BALANCE_2, PIN_BALANCE_3, PIN_BALANCE_4 };
        pinMode(pins[i], OUTPUT);
        digitalWrite(pins[i], LOW);
    }
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_BMS_ALERT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_BMS_ALERT), isr_bq_alert, FALLING);

    // TFT
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    spr.createSprite(SCREEN_W, SCREEN_H);

    // Load NVS state
    g_prefs.begin("bms", false);
    g_bms.soc_coulomb = g_prefs.getFloat("soc", 50.0f);
    g_bms.soc_ekf     = g_bms.soc_coulomb;
    g_bms.ah_in       = g_prefs.getFloat("ah_in", 0.0f);
    g_bms.wh_in       = g_prefs.getFloat("wh_in", 0.0f);
    g_bms.ah_out      = g_prefs.getFloat("ah_out", 0.0f);
    g_bms.wh_out      = g_prefs.getFloat("wh_out", 0.0f);
    g_prefs.end();

    // Initialize BQ76920
    if (!bq_init()) {
        Serial.println("BQ76920 init failed");
        g_bms.fault_comm = true;
    }

    // Initialize INA226 × 2
    ina226_init(INA226_CHG,  SHUNT_CHG_OHM);
    ina226_init(INA226_LOAD, SHUNT_LOAD_OHM);

    // BLE
    ble_init();

    // WiFi AP
    WiFi.mode(WIFI_AP);
    WiFi.softAP("BMS-Monitor", "hamradio");
    web_init();

    g_last_update_ms = millis();
    Serial.println("BMS Monitor ready");
    draw_splash();
    delay(1500);
}

// ─────────────────────────────────────────────────────────────────────────────
// MAIN LOOP
// ─────────────────────────────────────────────────────────────────────────────
void loop() {
    static unsigned long t_fast = 0;    // 250 ms
    static unsigned long t_slow = 0;    // 2000 ms (EKF, balancing)
    static unsigned long t_disp = 0;    // 200 ms display refresh
    static uint8_t disp_page = 0;
    unsigned long now = millis();

    g_server.handleClient();

    // Fast update: current measurement and LVD (250 ms)
    if (now - t_fast >= 250) {
        t_fast = now;
        update_current();
        update_coulomb_counting();
        check_lvd();
        ble_notify_fast();
    }

    // Slow update: cell voltages, temperature, EKF, balancing (2 s)
    if (now - t_slow >= 2000) {
        t_slow = now;
        update_cell_voltages();
        update_temperature();
        ekf_update();
        update_balancing();
        check_faults();
        update_mppt_setpoint();
        update_runtime_estimate();
        ble_notify_slow();
    }

    // Display refresh (200 ms)
    if (now - t_disp >= 200) {
        t_disp = now;
        // Auto-rotate pages every 10 s
        if ((now / 10000) % 4 != disp_page) {
            disp_page = (now / 10000) % 4;
        }
        draw_page(disp_page);
    }

    // NVS save every 60 s
    if (now - g_last_nvs_save >= 60000) {
        g_last_nvs_save = now;
        save_state_nvs();
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// BQ76920 DRIVER
// ─────────────────────────────────────────────────────────────────────────────
bool bq_init() {
    // Check device responds
    Wire.beginTransmission(BQ76920_ADDR);
    if (Wire.endTransmission() != 0) return false;

    // Read ADC calibration from factory-programmed OTP registers
    uint8_t gain1 = bq_read(BQ_ADCGAIN1);
    uint8_t gain2 = bq_read(BQ_ADCGAIN2);
    // ADCGAIN (µV/LSB) = 365 + (gain1[7:5] << 1) | (gain2[2:1])
    g_adc_gain = 365.0f + (float)(((gain1 & 0xE0) >> 4) | ((gain2 & 0x06) >> 1));
    int8_t offset_raw = (int8_t)bq_read(BQ_ADCOFFSET);
    g_adc_offset = (float)offset_raw;  // mV

    // Enable ADC and coulomb counter
    bq_write(BQ_SYS_CTRL1, 0x18);  // ADC_EN=1, TEMP_SEL=1 (use TS pin), LOAD_PRES=0
    bq_write(BQ_SYS_CTRL2, 0x43);  // CHG_ON=1, DSG_ON=1, CC_EN=1

    // Configure protection thresholds (LiFePO4)
    if (CHEMISTRY == 0) {  // LiFePO4
        bq_write(BQ_OV_TRIP,  0x20);  // 3.663V OV trip
        bq_write(BQ_UV_TRIP,  0x22);  // 2.834V UV trip
        bq_write(BQ_PROTECT1, 0x0C);  // SCD: 100 mV, 70 µs delay
        bq_write(BQ_PROTECT2, 0x07);  // OCD: 50 mV, 20 ms delay
        bq_write(BQ_PROTECT3, 0xA0);  // OV_DELAY: 2s, UV_DELAY: 4s
    } else {               // Li-Ion
        bq_write(BQ_OV_TRIP,  0x27);  // 4.091V OV trip (conservative for cycle life)
        bq_write(BQ_UV_TRIP,  0x26);  // 3.024V UV trip
        bq_write(BQ_PROTECT1, 0x14);  // SCD: 200 mV, 70 µs
        bq_write(BQ_PROTECT2, 0x07);  // OCD: 50 mV, 20 ms
        bq_write(BQ_PROTECT3, 0xA0);
    }

    // Clear any initial fault flags
    bq_write(BQ_SYS_STAT, 0xFF);
    return true;
}

uint8_t bq_read(uint8_t reg) {
    Wire.beginTransmission(BQ76920_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(BQ76920_ADDR, (uint8_t)1);
    return Wire.available() ? Wire.read() : 0;
}

void bq_write(uint8_t reg, uint8_t val) {
    Wire.beginTransmission(BQ76920_ADDR);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
}

void update_cell_voltages() {
    g_bms.pack_v = 0;
    for (int i = 0; i < N_CELLS; i++) {
        uint8_t hi = bq_read(BQ_VC1_HI + i * 2);
        uint8_t lo = bq_read(BQ_VC1_HI + i * 2 + 1);
        int16_t raw = ((int16_t)(hi & 0x3F) << 8) | lo;
        g_bms.cell_v[i] = (raw * g_adc_gain / 1000.0f + g_adc_offset) / 1000.0f;
        // Convert: raw × µV/LSB / 1000 = mV + offset_mV → / 1000 = V
        if (g_bms.cell_v[i] < 0) g_bms.cell_v[i] = 0;
        g_bms.pack_v += g_bms.cell_v[i];
    }
}

void bq_set_fets(bool chg, bool dsg) {
    g_bms.chg_enabled = chg;
    g_bms.dsg_enabled = dsg;
    uint8_t ctrl = bq_read(BQ_SYS_CTRL2);
    ctrl = (ctrl & 0xFC) | (chg ? 0x02 : 0x00) | (dsg ? 0x01 : 0x00);
    bq_write(BQ_SYS_CTRL2, ctrl);
}

void IRAM_ATTR isr_bq_alert() {
    // BQ76920 fault detected — minimal ISR, handle in main loop
    // (set flag to check SYS_STAT register)
}

// ─────────────────────────────────────────────────────────────────────────────
// INA226 DRIVER
// ─────────────────────────────────────────────────────────────────────────────
void ina226_write(uint8_t addr, uint8_t reg, uint16_t val) {
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write((val >> 8) & 0xFF);
    Wire.write(val & 0xFF);
    Wire.endTransmission();
}

int16_t ina226_read16(uint8_t addr, uint8_t reg) {
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(addr, (uint8_t)2);
    return (int16_t)((Wire.read() << 8) | Wire.read());
}

void ina226_init(uint8_t addr, float r_shunt) {
    // Configuration: 16 sample average, 1.1 ms conversion, continuous both
    ina226_write(addr, INA226_CONFIG, 0x4FFB);
    // Calibration: Cal = 0.00512 / (I_LSB × R_shunt)
    // Choose I_LSB = 1 mA/bit (can measure up to 32.768 A)
    float i_lsb = 0.001f;  // 1 mA/bit
    uint16_t cal = (uint16_t)(0.00512f / (i_lsb * r_shunt));
    ina226_write(addr, INA226_CAL, cal);
}

float ina226_current(uint8_t addr) {
    int16_t raw = ina226_read16(addr, INA226_CURRENT);
    return raw * 0.001f;  // 1 mA/bit
}

float ina226_voltage(uint8_t addr) {
    uint16_t raw = (uint16_t)ina226_read16(addr, INA226_BUS);
    return raw * 0.00125f;  // 1.25 mV/bit
}

void update_current() {
    g_bms.i_charge = ina226_current(INA226_CHG);
    g_bms.i_load   = ina226_current(INA226_LOAD);
    g_bms.i_net    = g_bms.i_charge - g_bms.i_load;
    if (g_bms.i_charge < 0) g_bms.i_charge = 0;
    if (g_bms.i_load < 0)   g_bms.i_load   = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// TEMPERATURE (ADS1115 NTC)
// ─────────────────────────────────────────────────────────────────────────────
float ads1115_read_voltage(uint8_t ch) {
    Wire.beginTransmission(ADS1115_ADDR);
    Wire.write(0x01);
    uint16_t cfg = 0x8000 | ((0x4 | ch) << 12) | 0x0183;
    Wire.write((cfg >> 8) & 0xFF);
    Wire.write(cfg & 0xFF);
    Wire.endTransmission();
    delay(10);
    Wire.beginTransmission(ADS1115_ADDR);
    Wire.write(0x00);
    Wire.endTransmission(false);
    Wire.requestFrom(ADS1115_ADDR, (uint8_t)2);
    int16_t raw = ((int16_t)Wire.read() << 8) | Wire.read();
    return constrain(raw * 4.096f / 32768.0f, 0.0f, 4.0f);
}

float ntc_celsius(float v_adc, float r_ref=10000.0f, float b=3950.0f) {
    if (v_adc < 0.05f || v_adc > 3.25f) return -99.0f;
    float r = r_ref * v_adc / (3.3f - v_adc);
    float t_k = b / (b / 298.15f + logf(r / r_ref));
    return t_k - 273.15f;
}

void update_temperature() {
    g_bms.temp_cell = ntc_celsius(ads1115_read_voltage(0));
    g_bms.temp_fet  = ntc_celsius(ads1115_read_voltage(1));
}

// ─────────────────────────────────────────────────────────────────────────────
// COULOMB COUNTING
// ─────────────────────────────────────────────────────────────────────────────
void update_coulomb_counting() {
    unsigned long now = millis();
    float dt_h = (now - g_last_update_ms) / 3600000.0f;
    g_last_update_ms = now;

    float i_net = g_bms.i_charge - g_bms.i_load;
    float dah   = i_net * dt_h;  // Positive = charging

    // Update SOC: SOC += dAh / Capacity (accounting for Coulombic efficiency)
    float eta_c = (i_net > 0) ? 0.97f : 1.0f;  // 97% charge efficiency for LiFePO4
    g_bms.soc_coulomb += (dah * eta_c / PACK_CAPACITY_AH) * 100.0f;
    g_bms.soc_coulomb  = constrain(g_bms.soc_coulomb, 0.0f, 100.0f);

    // Energy accounting
    if (i_net > 0) {
        g_bms.ah_in += dah;
        g_bms.wh_in += dah * g_bms.pack_v;
    } else if (i_net < 0) {
        g_bms.ah_out += (-dah);
        g_bms.wh_out += (-dah) * g_bms.pack_v;
    }

    // Efficiency calculation
    if (g_bms.ah_in > 0.5f) {
        g_bms.coulomb_eff = (g_bms.ah_out / g_bms.ah_in) * 100.0f;
    }
    if (g_bms.wh_in > 0.5f) {
        g_bms.energy_eff = (g_bms.wh_out / g_bms.wh_in) * 100.0f;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// EXTENDED KALMAN FILTER — SOC ESTIMATION
// ─────────────────────────────────────────────────────────────────────────────
/*
 * State: x = SOC (0.0 to 1.0)
 * Process model: x(k+1) = x(k) + eta_c × I_net × dt / Q_nom
 * Measurement model: OCV = f(x)  (nonlinear OCV-SOC curve)
 * Linearization: H = df/dx (derivative of OCV with respect to SOC)
 *
 * EKF is only valid during rest (no current flow).
 * During current flow: coulomb counting is used; EKF is in prediction-only mode.
 * At rest (|I| < 0.1A for > 30 s): OCV measurement is available → EKF update.
 *
 * This provides the best of both methods:
 *   - Coulomb counting: accurate during transients and operation
 *   - OCV correlation:  corrects accumulated drift during rest periods
 */

float ekf_ocv_to_soc(float ocv) {
    const float* tbl_ocv = (CHEMISTRY == 0) ? LIFEPO4_OCV : LIION_OCV;
    const float* tbl_soc = (CHEMISTRY == 0) ? LIFEPO4_SOC : LIION_SOC;
    if (ocv <= tbl_ocv[0])  return tbl_soc[0];
    if (ocv >= tbl_ocv[19]) return tbl_soc[19];
    for (int i = 0; i < 19; i++) {
        if (ocv >= tbl_ocv[i] && ocv < tbl_ocv[i+1]) {
            float t = (ocv - tbl_ocv[i]) / (tbl_ocv[i+1] - tbl_ocv[i]);
            return tbl_soc[i] + t * (tbl_soc[i+1] - tbl_soc[i]);
        }
    }
    return 50.0f;
}

// Derivative of OCV-SOC curve at x (for EKF Jacobian H)
float ekf_docv_dsoc(float soc_pct) {
    const float* tbl_ocv = (CHEMISTRY == 0) ? LIFEPO4_OCV : LIION_OCV;
    const float* tbl_soc = (CHEMISTRY == 0) ? LIFEPO4_SOC : LIION_SOC;
    float soc_frac = soc_pct;
    for (int i = 0; i < 19; i++) {
        if (soc_frac >= tbl_soc[i] && soc_frac < tbl_soc[i+1]) {
            float dv = tbl_ocv[i+1] - tbl_ocv[i];
            float ds = tbl_soc[i+1] - tbl_soc[i];
            return (ds > 0.01f) ? (dv / ds) : 0.001f;
        }
    }
    return 0.001f;
}

static unsigned long g_rest_start = 0;
static bool          g_in_rest    = false;

void ekf_update() {
    static unsigned long g_last_ekf_ms = 0;
    unsigned long now = millis();
    float dt_h = (now - g_last_ekf_ms) / 3600000.0f;
    g_last_ekf_ms = now;

    if (!g_ekf.initialized) {
        // Initialize EKF from OCV if pack is at rest
        if (fabsf(g_bms.i_net) < 0.1f) {
            float soc_ocv = ekf_ocv_to_soc(g_bms.pack_v);
            g_ekf.x = soc_ocv / 100.0f;
            g_ekf.P = 0.05f;  // Moderate initial uncertainty
            g_ekf.initialized = true;
            g_bms.soc_coulomb = soc_ocv;
        }
        return;
    }

    // ── EKF Prediction Step ──
    float i_net_A = g_bms.i_charge - g_bms.i_load;
    float eta_c   = (i_net_A > 0) ? 0.97f : 1.0f;
    float dx       = eta_c * i_net_A * dt_h / PACK_CAPACITY_AH;

    float x_pred = constrain(g_ekf.x + dx, 0.0f, 1.0f);
    float P_pred = g_ekf.P + g_ekf.Q;

    // ── EKF Update Step (only during rest) ──
    bool currently_resting = (fabsf(i_net_A) < 0.1f);
    if (currently_resting) {
        if (!g_in_rest) { g_in_rest = true; g_rest_start = now; }
        // Wait 60 seconds at rest before trusting OCV
        if (now - g_rest_start >= 60000) {
            float ocv     = g_bms.pack_v;  // Pack OCV at rest = pack voltage
            float soc_meas = ekf_ocv_to_soc(ocv) / 100.0f;  // Measurement (0..1)
            float H        = ekf_docv_dsoc(x_pred * 100.0f); // Jacobian (dOCV/dSOC)
            float y_pred   = x_pred;                          // Predicted SOC in same units
            float S        = H * H * P_pred + g_ekf.R;       // Innovation covariance
            float K        = P_pred * H / S;                  // Kalman gain

            // Update: EKF measurement is SOC inferred from OCV, not OCV itself
            float innovation = soc_meas - x_pred;
            g_ekf.x = constrain(x_pred + K * innovation, 0.0f, 1.0f);
            g_ekf.P = (1.0f - K * H) * P_pred;

            // Reseed coulomb counter from EKF estimate
            g_bms.soc_coulomb = g_ekf.x * 100.0f;
        }
    } else {
        g_in_rest = false;
        g_ekf.x = x_pred;
        g_ekf.P = P_pred;
    }

    g_bms.soc_ekf     = g_ekf.x * 100.0f;
    g_bms.soc_display = g_bms.soc_ekf;

    // Ah remaining estimate
    g_bms.ah_remaining = PACK_CAPACITY_AH * g_bms.soc_display / 100.0f;
}

// ─────────────────────────────────────────────────────────────────────────────
// CELL BALANCING CONTROL
// ─────────────────────────────────────────────────────────────────────────────
void update_balancing() {
    // Only balance when SOC > 85% and cells are not too hot
    if (g_bms.soc_display < 85.0f || g_bms.temp_cell > 45.0f) {
        bq_write(BQ_CELLBAL1, 0x00);  // Disable all balance FETs
        return;
    }

    float v_min = g_bms.cell_v[0];
    for (int i = 1; i < N_CELLS; i++) {
        if (g_bms.cell_v[i] < v_min) v_min = g_bms.cell_v[i];
    }

    uint8_t bal_mask = 0;
    for (int i = 0; i < N_CELLS; i++) {
        if ((g_bms.cell_v[i] - v_min) > 0.010f) {  // > 10 mV above minimum
            bal_mask |= (1 << i);
        }
    }
    bq_write(BQ_CELLBAL1, bal_mask);
}

// ─────────────────────────────────────────────────────────────────────────────
// FAULT CHECKING AND FET CONTROL
// ─────────────────────────────────────────────────────────────────────────────
void check_faults() {
    g_bms.bq_stat = bq_read(BQ_SYS_STAT);

    g_bms.fault_ov = (g_bms.bq_stat & 0x08);  // OV bit
    g_bms.fault_uv = (g_bms.bq_stat & 0x04);  // UV bit
    g_bms.fault_oc = (g_bms.bq_stat & 0x02) | (g_bms.bq_stat & 0x01);  // OCD|SCD
    g_bms.fault_ot = (g_bms.temp_cell > 55.0f);

    // Temperature-based FET control (firmware level, supplements BQ76920 hardware)
    bool chg = true, dsg = true;

    if (g_bms.fault_ov) chg = false;   // OV: stop charging
    if (g_bms.fault_uv) dsg = false;   // UV: stop discharging
    if (g_bms.fault_oc) { chg = false; dsg = false; }  // OC: stop both

    if (g_bms.temp_cell < 0.0f)   chg = false;  // Cold: no charging
    if (g_bms.temp_cell < -20.0f) dsg = false;  // Very cold: no discharge
    if (g_bms.temp_cell > 55.0f)  { chg = false; dsg = false; }  // Overtemp

    if (chg != g_bms.chg_enabled || dsg != g_bms.dsg_enabled) {
        bq_set_fets(chg, dsg);
        if (!chg || !dsg) beep_alarm();
    }

    // Clear non-latching faults after action taken
    if (g_bms.bq_stat & 0xF8) {
        bq_write(BQ_SYS_STAT, g_bms.bq_stat & 0xF8);  // Write 1 to clear
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// LOW-VOLTAGE DISCONNECT
// ─────────────────────────────────────────────────────────────────────────────
void check_lvd() {
    static bool lvd_tripped = false;

    if (!lvd_tripped && g_bms.pack_v < LVD_THRESHOLD) {
        lvd_tripped = true;
        g_bms.lvd_connected = false;
        digitalWrite(PIN_LVD_RELAY, LOW);   // Open relay, disconnect load
        bq_set_fets(false, false);
        beep_alarm();
        Serial.println("LVD TRIP");
    }
    // Reconnect only when charger restores voltage AND SOC > 20%
    if (lvd_tripped && g_bms.pack_v > LVD_RECONNECT && g_bms.soc_display > 20.0f) {
        lvd_tripped = false;
        g_bms.lvd_connected = true;
        digitalWrite(PIN_LVD_RELAY, HIGH);
        bq_set_fets(true, true);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// MPPT CONTROLLER INTERFACE
// ─────────────────────────────────────────────────────────────────────────────
void update_mppt_setpoint() {
    char cmd[64];
    float v_set = 0, i_set = 0;

    // Determine charge stage
    if (!g_bms.chg_enabled || g_bms.temp_cell < 0.0f) {
        // Charging prohibited
        snprintf(cmd, sizeof(cmd), "VSET=0.0 ISET=0.0\n");
    } else if (g_bms.soc_display < 80.0f) {
        // Bulk: constant current charge
        float v_adj = ((CHEMISTRY == 0) ? 14.4f : 16.8f)
                      - (g_bms.temp_cell - 25.0f) * 0.012f;
        snprintf(cmd, sizeof(cmd), "VSET=%.2f ISET=%.1f\n", v_adj, 25.0f);
    } else if (g_bms.i_charge > PACK_CAPACITY_AH / 20.0f) {
        // Absorption: constant voltage, tapering current
        float v_adj = ((CHEMISTRY == 0) ? 14.4f : 16.8f)
                      - (g_bms.temp_cell - 25.0f) * 0.012f;
        snprintf(cmd, sizeof(cmd), "VSET=%.2f ISET=%.1f\n", v_adj, 10.0f);
    } else {
        // Float
        snprintf(cmd, sizeof(cmd), "VSET=%.2f ISET=5.0\n",
                 (CHEMISTRY == 0) ? 13.5f : 15.5f);
    }

    mpptSerial.print(cmd);
}

// ─────────────────────────────────────────────────────────────────────────────
// RUNTIME ESTIMATE
// ─────────────────────────────────────────────────────────────────────────────
void update_runtime_estimate() {
    // Smooth load current with simple IIR filter
    static float i_avg = 0.0f;
    i_avg = 0.9f * i_avg + 0.1f * g_bms.i_load;

    if (i_avg > 0.1f) {
        float ah_remain = PACK_CAPACITY_AH * g_bms.soc_display / 100.0f;
        g_bms.minutes_remaining = (ah_remain / i_avg) * 60.0f;
    } else {
        g_bms.minutes_remaining = 9999.0f;  // Unknown (no load)
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// BLE GATT SERVER
// ─────────────────────────────────────────────────────────────────────────────
class BLECallback : public BLEServerCallbacks {
    void onConnect(BLEServer* svr)    { g_ble_connected = true; }
    void onDisconnect(BLEServer* svr) {
        g_ble_connected = false;
        BLEDevice::startAdvertising();  // Restart advertising on disconnect
    }
};

void ble_init() {
    BLEDevice::init("BMS-Monitor");
    g_ble_server = BLEDevice::createServer();
    g_ble_server->setCallbacks(new BLECallback());

    BLEService* svc = g_ble_server->createService(BLE_BMS_SERVICE_UUID);

    // Cell voltages characteristic (4× float32 = 16 bytes)
    g_char_cellv = svc->createCharacteristic(BLE_CELLV_CHAR_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    g_char_cellv->addDescriptor(new BLE2902());

    // SOC characteristic (float32 = 4 bytes)
    g_char_soc = svc->createCharacteristic(BLE_SOC_CHAR_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    g_char_soc->addDescriptor(new BLE2902());

    // Current characteristic (2× float32: charge + load)
    g_char_current = svc->createCharacteristic(BLE_CURRENT_CHAR_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    g_char_current->addDescriptor(new BLE2902());

    // Temperature (2× float32: cell + FET)
    g_char_temp = svc->createCharacteristic(BLE_TEMP_CHAR_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    g_char_temp->addDescriptor(new BLE2902());

    // Status flags (1 byte: bit 0=OV, 1=UV, 2=OC, 3=OT, 4=CHG_EN, 5=DSG_EN, 6=LVD)
    g_char_status = svc->createCharacteristic(BLE_STATUS_CHAR_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    g_char_status->addDescriptor(new BLE2902());

    // Efficiency (2× float32: coulombic %, energy %)
    g_char_eff = svc->createCharacteristic(BLE_EFFICIENCY_CHAR_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    g_char_eff->addDescriptor(new BLE2902());

    svc->start();

    BLEAdvertising* adv = BLEDevice::getAdvertising();
    adv->addServiceUUID(BLE_BMS_SERVICE_UUID);
    adv->setScanResponse(true);
    adv->setMinPreferred(0x06);
    BLEDevice::startAdvertising();
}

void ble_notify_fast() {
    if (!g_ble_connected) return;

    // Encode 2× float32 as raw bytes for current characteristic
    float curr_buf[2] = { g_bms.i_charge, g_bms.i_load };
    g_char_current->setValue((uint8_t*)curr_buf, sizeof(curr_buf));
    g_char_current->notify();
}

void ble_notify_slow() {
    if (!g_ble_connected) return;

    // Cell voltages (4× float32, little-endian)
    g_char_cellv->setValue((uint8_t*)g_bms.cell_v, sizeof(float) * N_CELLS);
    g_char_cellv->notify();

    // SOC
    g_char_soc->setValue((uint8_t*)&g_bms.soc_display, sizeof(float));
    g_char_soc->notify();

    // Temperature
    float temp_buf[2] = { g_bms.temp_cell, g_bms.temp_fet };
    g_char_temp->setValue((uint8_t*)temp_buf, sizeof(temp_buf));
    g_char_temp->notify();

    // Status byte
    uint8_t status = 0;
    if (g_bms.fault_ov)      status |= 0x01;
    if (g_bms.fault_uv)      status |= 0x02;
    if (g_bms.fault_oc)      status |= 0x04;
    if (g_bms.fault_ot)      status |= 0x08;
    if (g_bms.chg_enabled)   status |= 0x10;
    if (g_bms.dsg_enabled)   status |= 0x20;
    if (g_bms.lvd_connected) status |= 0x40;
    g_char_status->setValue(&status, 1);
    g_char_status->notify();

    // Efficiency
    float eff_buf[2] = { g_bms.coulomb_eff, g_bms.energy_eff };
    g_char_eff->setValue((uint8_t*)eff_buf, sizeof(eff_buf));
    g_char_eff->notify();
}

// ─────────────────────────────────────────────────────────────────────────────
// CYD DISPLAY
// ─────────────────────────────────────────────────────────────────────────────
#define C_BG     TFT_BLACK
#define C_TITLE  0x07FF
#define C_VAL    TFT_WHITE
#define C_GOOD   TFT_GREEN
#define C_WARN   TFT_YELLOW
#define C_ALARM  TFT_RED
#define C_GRID   0x2104
#define C_BLUE   0x001F

void draw_page(uint8_t page) {
    spr.fillSprite(C_BG);
    bool any_fault = g_bms.fault_ov || g_bms.fault_uv || g_bms.fault_oc || g_bms.fault_ot;
    if (any_fault) { draw_fault_page(); spr.pushSprite(0,0); return; }
    switch (page % 4) {
        case 0: draw_cell_page();  break;
        case 1: draw_soc_page();   break;
        case 2: draw_power_page(); break;
        case 3: draw_eff_page();   break;
    }
    spr.pushSprite(0, 0);
}

void draw_cell_page() {
    spr.fillRect(0, 0, SCREEN_W, 24, C_TITLE);
    spr.setTextColor(TFT_BLACK); spr.setTextSize(2);
    spr.drawString("CELL VOLTAGES", 5, 4);
    spr.setTextColor(g_ble_connected ? C_GOOD : C_GRID);
    spr.drawString("BLE", 280, 4);

    // Four cell bars
    for (int i = 0; i < N_CELLS; i++) {
        float v = g_bms.cell_v[i];
        uint16_t cc = (v > 3.55f) ? C_GOOD : (v > 2.9f) ? C_VAL : (v > 2.7f) ? C_WARN : C_ALARM;

        int y = 32 + i * 48;
        spr.setTextColor(C_GRID); spr.setTextSize(1);
        char lbl[8]; snprintf(lbl, sizeof(lbl), "C%d", i+1);
        spr.drawString(lbl, 5, y);

        // Voltage bar (width 0–240 px = 0–4.2V)
        float v_max_chem = (CHEMISTRY == 0) ? 3.65f : 4.2f;
        float v_min_chem = (CHEMISTRY == 0) ? 2.5f  : 3.0f;
        int bar_w = (int)((v - v_min_chem) / (v_max_chem - v_min_chem) * 240.0f);
        bar_w = constrain(bar_w, 0, 240);
        spr.fillRect(25, y + 2, 240, 14, C_GRID);
        spr.fillRect(25, y + 2, bar_w, 14, cc);

        // Voltage value
        char vstr[12]; snprintf(vstr, sizeof(vstr), "%.3fV", v);
        spr.setTextColor(cc); spr.setTextSize(2);
        spr.drawString(vstr, 270, y);
    }

    // Pack voltage at bottom
    spr.drawFastHLine(0, 228, SCREEN_W, C_GRID);
    char pstr[24]; snprintf(pstr, sizeof(pstr), "Pack: %.2fV", g_bms.pack_v);
    spr.setTextColor(C_VAL); spr.setTextSize(1);
    spr.drawString(pstr, 5, 231);
}

void draw_soc_page() {
    spr.fillRect(0, 0, SCREEN_W, 24, 0x0200);
    spr.setTextColor(TFT_WHITE); spr.setTextSize(2);
    spr.drawString("STATE OF CHARGE", 5, 4);

    // Large SOC readout
    uint16_t soc_color = (g_bms.soc_display > 40) ? C_GOOD
                       : (g_bms.soc_display > 15) ? C_WARN : C_ALARM;
    char socstr[12]; snprintf(socstr, sizeof(socstr), "%.0f%%", g_bms.soc_display);
    spr.setTextColor(soc_color); spr.setTextSize(6);
    spr.drawString(socstr, 50, 40);

    // SOC bar
    int bar_w = (int)(g_bms.soc_display * 3.1f);  // 0–310 px
    spr.fillRect(5, 110, 310, 24, C_GRID);
    spr.fillRect(5, 110, bar_w, 24, soc_color);

    // Remaining time and Ah
    spr.setTextSize(1); spr.setTextColor(C_GRID);
    spr.drawString("REMAINING", 5, 144);
    spr.drawString("AH LEFT", 100, 144);
    spr.drawString("COULOMB EKF", 200, 144);

    spr.setTextSize(2); spr.setTextColor(C_VAL);
    char tstr[16];
    if (g_bms.minutes_remaining > 999.0f) snprintf(tstr, sizeof(tstr), "--:--");
    else snprintf(tstr, sizeof(tstr), "%3.0fmin", g_bms.minutes_remaining);
    spr.drawString(tstr, 5, 155);

    char ahstr[12]; snprintf(ahstr, sizeof(ahstr), "%.1fAh", g_bms.ah_remaining);
    spr.drawString(ahstr, 100, 155);

    // EKF vs coulomb comparison
    spr.setTextSize(1); spr.setTextColor(C_GRID);
    spr.drawString("EKF", 200, 155);
    spr.setTextSize(2); spr.setTextColor(C_BLUE);
    char ekfstr[12]; snprintf(ekfstr, sizeof(ekfstr), "%.0f%%", g_bms.soc_ekf);
    spr.drawString(ekfstr, 220, 155);

    // Temperature
    spr.drawFastHLine(0, 185, SCREEN_W, C_GRID);
    spr.setTextSize(1); spr.setTextColor(C_GRID);
    spr.drawString("CELL TEMP", 5, 192); spr.drawString("FET TEMP", 130, 192);
    spr.setTextSize(2);
    char tcel[12]; snprintf(tcel, sizeof(tcel), "%.0fC", g_bms.temp_cell);
    char tfet[12]; snprintf(tfet, sizeof(tfet), "%.0fC", g_bms.temp_fet);
    uint16_t tc = (g_bms.temp_cell > 50) ? C_ALARM : (g_bms.temp_cell > 35) ? C_WARN : C_GOOD;
    spr.setTextColor(tc); spr.drawString(tcel, 5, 204);
    spr.setTextColor(C_VAL); spr.drawString(tfet, 130, 204);
}

void draw_power_page() {
    spr.fillRect(0, 0, SCREEN_W, 24, 0x6200);
    spr.setTextColor(TFT_WHITE); spr.setTextSize(2);
    spr.drawString("POWER FLOW", 5, 4);

    spr.setTextSize(1); spr.setTextColor(C_GRID);
    spr.drawString("CHARGE CURRENT", 5, 30); spr.drawString("LOAD CURRENT", 175, 30);
    spr.setTextSize(3); spr.setTextColor(C_GOOD);
    char ic[12]; snprintf(ic, sizeof(ic), "%5.2fA", g_bms.i_charge);
    char il[12]; snprintf(il, sizeof(il), "%5.2fA", g_bms.i_load);
    spr.drawString(ic, 5, 42);
    spr.setTextColor(C_WARN); spr.drawString(il, 175, 42);

    spr.setTextSize(1); spr.setTextColor(C_GRID);
    spr.drawString("CHARGE POWER", 5, 82); spr.drawString("LOAD POWER", 175, 82);
    spr.setTextSize(2); spr.setTextColor(C_VAL);
    char pc[12]; snprintf(pc, sizeof(pc), "%5.1fW", g_bms.i_charge * g_bms.pack_v);
    char pl[12]; snprintf(pl, sizeof(pl), "%5.1fW", g_bms.i_load * g_bms.pack_v);
    spr.drawString(pc, 5, 94);
    spr.drawString(pl, 175, 94);

    // FET state indicators
    spr.drawFastHLine(0, 120, SCREEN_W, C_GRID);
    spr.setTextSize(1); spr.setTextColor(C_GRID);
    spr.drawString("CHG FET", 5, 128); spr.drawString("DSG FET", 90, 128);
    spr.drawString("LVD", 175, 128);
    spr.fillCircle(20,  142, 8, g_bms.chg_enabled ? C_GOOD : C_ALARM);
    spr.fillCircle(105, 142, 8, g_bms.dsg_enabled ? C_GOOD : C_ALARM);
    spr.fillCircle(182, 142, 8, g_bms.lvd_connected ? C_GOOD : C_ALARM);

    // Wh accumulators
    spr.drawFastHLine(0, 165, SCREEN_W, C_GRID);
    spr.setTextSize(1); spr.setTextColor(C_GRID);
    spr.drawString("Wh IN (session)", 5, 172); spr.drawString("Wh OUT", 175, 172);
    spr.setTextSize(2); spr.setTextColor(C_VAL);
    char win[14]; snprintf(win, sizeof(win), "%.1fWh", g_bms.wh_in);
    char wout[14]; snprintf(wout, sizeof(wout), "%.1fWh", g_bms.wh_out);
    spr.drawString(win, 5, 184);
    spr.drawString(wout, 175, 184);
}

void draw_eff_page() {
    spr.fillRect(0, 0, SCREEN_W, 24, 0x4010);
    spr.setTextColor(TFT_WHITE); spr.setTextSize(2);
    spr.drawString("EFFICIENCY", 5, 4);

    spr.setTextSize(1); spr.setTextColor(C_GRID);
    spr.drawString("COULOMBIC EFF.", 5, 32);
    spr.drawString("(Ah_out / Ah_in × 100%)", 5, 44);
    spr.setTextSize(3);
    uint16_t ec = (g_bms.coulomb_eff > 98) ? C_GOOD : (g_bms.coulomb_eff > 95) ? C_WARN : C_ALARM;
    spr.setTextColor(ec);
    char ce[12]; snprintf(ce, sizeof(ce), "%.1f%%", g_bms.coulomb_eff);
    spr.drawString(ce, 5, 56);

    spr.setTextSize(1); spr.setTextColor(C_GRID);
    spr.drawString("ENERGY EFF. (round-trip)", 5, 95);
    spr.drawString("(Wh_out / Wh_in × 100%)", 5, 107);
    spr.setTextSize(3);
    uint16_t ee = (g_bms.energy_eff > 93) ? C_GOOD : (g_bms.energy_eff > 88) ? C_WARN : C_ALARM;
    spr.setTextColor(ee);
    char estr[12]; snprintf(estr, sizeof(estr), "%.1f%%", g_bms.energy_eff);
    spr.drawString(estr, 5, 118);

    spr.drawFastHLine(0, 158, SCREEN_W, C_GRID);
    spr.setTextSize(1); spr.setTextColor(C_GRID);
    spr.drawString("Ah in/out:", 5, 165); spr.drawString("Wh in/out:", 175, 165);
    spr.setTextSize(2); spr.setTextColor(C_VAL);
    char ahs[24]; snprintf(ahs, sizeof(ahs), "%.1f/%.1f", g_bms.ah_in, g_bms.ah_out);
    char whs[24]; snprintf(whs, sizeof(whs), "%.0f/%.0f", g_bms.wh_in, g_bms.wh_out);
    spr.drawString(ahs, 5, 178);
    spr.drawString(whs, 175, 178);

    spr.setTextSize(1); spr.setTextColor(C_GRID);
    spr.drawString("BLE characteristics: cell_v, soc, I, temp, status, eff", 5, 215);
}

void draw_fault_page() {
    spr.fillScreen(C_ALARM);
    spr.setTextColor(TFT_WHITE); spr.setTextSize(3);
    spr.drawString("FAULT", 110, 20);
    spr.setTextSize(2); int y = 65;
    if (g_bms.fault_ov) { spr.drawString("OVERVOLTAGE", 5, y); y += 28; }
    if (g_bms.fault_uv) { spr.drawString("UNDERVOLTAGE", 5, y); y += 28; }
    if (g_bms.fault_oc) { spr.drawString("OVERCURRENT", 5, y); y += 28; }
    if (g_bms.fault_ot) { spr.drawString("OVER-TEMP", 5, y); y += 28; }
    spr.setTextSize(1);
    spr.drawString("BQ SYS_STAT:", 5, 200);
    char stat[12]; snprintf(stat, sizeof(stat), "0x%02X", g_bms.bq_stat);
    spr.drawString(stat, 110, 200);
    spr.drawString("CHG/DSG FETs OPEN — Clear fault source", 5, 215);
}

void draw_splash() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_CYAN); tft.setTextSize(3);
    tft.drawString("BMS MONITOR", 30, 50);
    tft.setTextColor(TFT_WHITE); tft.setTextSize(2);
    tft.drawString("TM-BMS-001 Rev A", 25, 95);
    tft.setTextColor(TFT_DARKGREY); tft.setTextSize(1);
    tft.drawString("EKF SOC  |  BQ76920  |  BLE GATT", 30, 130);
}

// ─────────────────────────────────────────────────────────────────────────────
// WEB SERVER
// ─────────────────────────────────────────────────────────────────────────────
void web_init() {
    g_server.on("/",           HTTP_GET,  web_root);
    g_server.on("/api/status", HTTP_GET,  web_api_status);
    g_server.on("/api/reset",  HTTP_POST, web_api_reset);
    g_server.begin();
}

void web_root() {
    g_server.send(200, "text/html",
        "<!DOCTYPE html><html><head><title>BMS Monitor</title>"
        "<meta http-equiv='refresh' content='3'>"
        "<style>body{font-family:monospace;background:#111;color:#eee;padding:10px}"
        "h2{color:#0ff}.ok{color:#0f0}.warn{color:#ff0}.err{color:#f00}"
        "table{border-collapse:collapse;width:100%}td,th{border:1px solid #444;padding:4px}"
        "th{color:#0af;background:#1a1a1a}</style></head><body>"
        "<h2>BMS Monitor — TM-BMS-001</h2>"
        "<p>See /api/status for JSON data.</p>"
        "<form action='/api/reset' method='POST'>"
        "<button>Reset Energy Counters</button></form>"
        "</body></html>");
}

void web_api_status() {
    StaticJsonDocument<1024> doc;
    JsonArray cells = doc.createNestedArray("cells");
    for (int i = 0; i < N_CELLS; i++) cells.add(g_bms.cell_v[i]);
    doc["pack_v"]        = g_bms.pack_v;
    doc["soc"]           = g_bms.soc_display;
    doc["soc_coulomb"]   = g_bms.soc_coulomb;
    doc["soc_ekf"]       = g_bms.soc_ekf;
    doc["i_charge"]      = g_bms.i_charge;
    doc["i_load"]        = g_bms.i_load;
    doc["temp_cell"]     = g_bms.temp_cell;
    doc["temp_fet"]      = g_bms.temp_fet;
    doc["ah_in"]         = g_bms.ah_in;
    doc["wh_in"]         = g_bms.wh_in;
    doc["ah_out"]        = g_bms.ah_out;
    doc["wh_out"]        = g_bms.wh_out;
    doc["coulomb_eff"]   = g_bms.coulomb_eff;
    doc["energy_eff"]    = g_bms.energy_eff;
    doc["fault_ov"]      = g_bms.fault_ov;
    doc["fault_uv"]      = g_bms.fault_uv;
    doc["fault_oc"]      = g_bms.fault_oc;
    doc["fault_ot"]      = g_bms.fault_ot;
    doc["chg_en"]        = g_bms.chg_enabled;
    doc["dsg_en"]        = g_bms.dsg_enabled;
    doc["lvd"]           = g_bms.lvd_connected;
    doc["ble_connected"] = g_ble_connected;
    doc["minutes_remain"]= g_bms.minutes_remaining;
    String out; serializeJson(doc, out);
    g_server.send(200, "application/json", out);
}

void web_api_reset() {
    g_bms.ah_in = g_bms.wh_in = g_bms.ah_out = g_bms.wh_out = 0;
    g_bms.coulomb_eff = g_bms.energy_eff = 0;
    save_state_nvs();
    g_server.sendHeader("Location", "/"); g_server.send(303);
}

// ─────────────────────────────────────────────────────────────────────────────
// NVS PERSISTENCE
// ─────────────────────────────────────────────────────────────────────────────
void save_state_nvs() {
    g_prefs.begin("bms", false);
    g_prefs.putFloat("soc",    g_bms.soc_display);
    g_prefs.putFloat("ah_in",  g_bms.ah_in);
    g_prefs.putFloat("wh_in",  g_bms.wh_in);
    g_prefs.putFloat("ah_out", g_bms.ah_out);
    g_prefs.putFloat("wh_out", g_bms.wh_out);
    g_prefs.end();
}

// ─────────────────────────────────────────────────────────────────────────────
// UTILITIES
// ─────────────────────────────────────────────────────────────────────────────
void beep_alarm() {
    for (int i = 0; i < 3; i++) {
        digitalWrite(PIN_BUZZER, HIGH); delay(80);
        digitalWrite(PIN_BUZZER, LOW);  delay(80);
    }
}

/*
 * TFT_eSPI User_Setup.h for CYD (ESP32-2432S028):
 * Same as psu_monitor_esp32.ino — see TM-PWR-001 firmware file.
 *
 * Arduino Libraries required:
 *   TFT_eSPI (Bodmer)
 *   ArduinoJson (Benoit Blanchon, v6)
 *   ESP32 BLE Arduino (Neil Kolban)
 *
 * Compile: ESP32 Arduino Core 2.x; board = "ESP32 Dev Module" or CYD custom
 */
