/*
 * psu_monitor_esp32.ino
 * TM-PWR-001 Rev A — Power Supply Monitor and Controller
 *
 * Hardware: ESP32-2432S028 (CYD) or ESP32-WROOM-32 + ILI9341 TFT
 * Sensors:  INA219 (I2C 0x40/0x41), INA226 (I2C 0x45), ADS1115 (I2C 0x48)
 *           JK BMS UART (GPIO16 RX, GPIO17 TX, 115200 baud)
 * Outputs:  4× relay GPIO (power sequencer), PWM fan control
 * Display:  2.8" 320×240 TFT via SPI (CYD built-in)
 * WiFi:     Web server for remote monitoring and configuration
 * NVS:      Energy accumulator (Wh), configuration, alarm thresholds
 */

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Adafruit_INA219.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <driver/ledc.h>

// ─── Pin Assignments ─────────────────────────────────────────────────────────
#define PIN_RELAY1      12    // Power sequencer relay 1 (PSU enable)
#define PIN_RELAY2      13    // Power sequencer relay 2 (radio audio)
#define PIN_RELAY3      14    // Power sequencer relay 3 (radio RF)
#define PIN_RELAY4      15    // Power sequencer relay 4 (amplifier)
#define PIN_FAN_PWM     26    // Fan speed control (PWM)
#define PIN_ALERT_INA   35    // INA226 ALERT interrupt (active LOW)
#define PIN_MPPT_PWM    25    // Optional: direct MPPT PWM output
#define PIN_BMS_RX      16    // JK BMS UART receive
#define PIN_BMS_TX      17    // JK BMS UART transmit (not used; BMS is one-way)
#define PIN_BUZZER      32    // Piezo buzzer for alarms

// CYD TFT backlight (CYD-specific — adjust for other boards)
#define PIN_BL          21    // Backlight PWM (some CYD variants use 27)

// ─── I2C Addresses ───────────────────────────────────────────────────────────
#define INA219_ADDR_IN  0x40  // Input / panel current sensor
#define INA219_ADDR_OUT 0x41  // Output current sensor
#define INA226_ADDR     0x45  // High-accuracy output (A0=VCC, A1=VCC)
#define ADS1115_ADDR    0x48  // Temperature ADC

// ─── Constants ───────────────────────────────────────────────────────────────
#define SHUNT_OHMS_IN   0.050f  // Shunt for INA219 input channel
#define SHUNT_OHMS_OUT  0.025f  // Shunt for INA219 output channel
#define NTC_R_REF       10000.0f
#define NTC_B           3950.0f
#define NTC_T0          298.15f
#define V_SUPPLY        3.3f

// Alarm thresholds (configurable via web)
#define V_OV_DEFAULT    15.0f   // Overvoltage trip (V)
#define V_UV_DEFAULT    11.5f   // Undervoltage trip (V)
#define I_OC_DEFAULT    22.0f   // Overcurrent trip (A)
#define T_OT_DEFAULT    65.0f   // Over-temperature trip (°C)
#define T_FAN_ON        35.0f   // Fan turn-on temperature (°C)
#define T_FAN_FULL      55.0f   // Fan full-speed temperature (°C)

// ─── Display Dimensions ──────────────────────────────────────────────────────
#define SCREEN_W        320
#define SCREEN_H        240

// ─── Global Objects ──────────────────────────────────────────────────────────
TFT_eSPI     tft;
TFT_eSprite  spr(&tft);
Adafruit_INA219 ina_in(INA219_ADDR_IN);
Adafruit_INA219 ina_out(INA219_ADDR_OUT);
WebServer    server(80);
Preferences  prefs;
HardwareSerial bmsSerial(1);  // UART1 for JK BMS

// ─── Measurement Data ────────────────────────────────────────────────────────
struct PowerData {
    float v_in, i_in, p_in;      // Input (panel or mains) measurements
    float v_out, i_out, p_out;   // Output measurements
    float v_bat;                  // Battery pack voltage
    float temp_pass;              // Pass element / MOSFET temperature (°C)
    float temp_bat;               // Battery temperature (°C)
    float efficiency;             // η = P_out / P_in × 100%
    float wh_in;                  // Wh in (from solar/mains) — accumulated
    float wh_out;                 // Wh out (to load) — accumulated
    unsigned long timestamp;      // millis() of last update
};

PowerData g_data = {};
float g_wh_in_accum  = 0;
float g_wh_out_accum = 0;
unsigned long g_last_accum_ms = 0;

// ─── BMS Data ─────────────────────────────────────────────────────────────
struct BMSData {
    float cell_v[4];              // Individual cell voltages (V)
    float pack_v;                 // Pack voltage (V)
    float pack_i;                 // Pack current (A); negative = discharge
    float soc;                    // State of charge (%)
    float temp1, temp2;           // BMS temperatures (°C)
    bool  ov_fault;               // Overvoltage fault flag
    bool  uv_fault;               // Undervoltage fault flag
    bool  oc_fault;               // Overcurrent fault flag
    bool  valid;                  // True if recent valid packet received
    unsigned long last_rx_ms;     // Timestamp of last valid packet
};

BMSData g_bms = {};
uint8_t g_bms_buf[256];
int     g_bms_len = 0;

// ─── Alarm State ──────────────────────────────────────────────────────────
struct AlarmConfig {
    float v_ov, v_uv, i_oc, t_ot;
    bool  relay_on_ov, relay_on_oc;  // True = de-energize relay on alarm
};

AlarmConfig g_alarms = { V_OV_DEFAULT, V_UV_DEFAULT, I_OC_DEFAULT, T_OT_DEFAULT, true, true };
bool g_alarm_active = false;
bool g_alarm_latched = false;  // Requires manual reset via web

// ─── Display Mode ─────────────────────────────────────────────────────────
enum DisplayMode { DM_MAIN, DM_BATTERY, DM_SOLAR, DM_ALARM, DM_CONFIG };
DisplayMode g_disp_mode = DM_MAIN;
unsigned long g_disp_rotate_ms = 0;
#define DISP_ROTATE_INTERVAL  8000  // Auto-rotate screens every 8 s

// ─── WiFi Config ──────────────────────────────────────────────────────────
char g_ssid[64]     = "FieldStation";
char g_password[64] = "hamradio";

// ─── Relay Sequencer State ────────────────────────────────────────────────
bool g_relays[4]         = { false, false, false, false };
unsigned long g_relay_on_time[4] = { 0, 200, 500, 1000 };  // Sequence delay (ms)
bool g_sequencer_active = false;

// ─────────────────────────────────────────────────────────────────────────────
// SETUP
// ─────────────────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    Wire.begin(21, 22);
    bmsSerial.begin(115200, SERIAL_8N1, PIN_BMS_RX, PIN_BMS_TX);

    // Relay outputs
    for (int i = 0; i < 4; i++) {
        int pins[] = { PIN_RELAY1, PIN_RELAY2, PIN_RELAY3, PIN_RELAY4 };
        pinMode(pins[i], OUTPUT);
        digitalWrite(pins[i], LOW);
    }

    // Fan PWM
    ledcSetup(0, 25000, 8);   // Channel 0, 25 kHz, 8-bit (fan; above audible range)
    ledcAttachPin(PIN_FAN_PWM, 0);
    ledcWrite(0, 0);           // Fan off initially

    // Alert interrupt
    pinMode(PIN_ALERT_INA, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_ALERT_INA), isr_ina_alert, FALLING);

    // Buzzer
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);

    // TFT backlight
    pinMode(PIN_BL, OUTPUT);
    digitalWrite(PIN_BL, HIGH);

    // TFT init
    tft.init();
    tft.setRotation(1);  // Landscape
    tft.fillScreen(TFT_BLACK);
    spr.createSprite(SCREEN_W, SCREEN_H);

    // Sensor init
    if (!ina_in.begin()) {
        Serial.println("INA219 (input) not found");
    }
    if (!ina_out.begin()) {
        Serial.println("INA219 (output) not found");
    }
    ina_in.setCalibration_16V_400mA();   // Adjust for actual shunt value
    ina_out.setCalibration_16V_400mA();  // Adjust for actual shunt value

    // Load configuration from NVS
    prefs.begin("psu", false);
    g_wh_in_accum  = prefs.getFloat("wh_in", 0.0f);
    g_wh_out_accum = prefs.getFloat("wh_out", 0.0f);
    g_alarms.v_ov  = prefs.getFloat("v_ov",  V_OV_DEFAULT);
    g_alarms.v_uv  = prefs.getFloat("v_uv",  V_UV_DEFAULT);
    g_alarms.i_oc  = prefs.getFloat("i_oc",  I_OC_DEFAULT);
    g_alarms.t_ot  = prefs.getFloat("t_ot",  T_OT_DEFAULT);
    strlcpy(g_ssid,     prefs.getString("ssid", "FieldStation").c_str(), 64);
    strlcpy(g_password, prefs.getString("pass", "hamradio").c_str(), 64);
    prefs.end();

    // WiFi
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP("PSU-Monitor", "hamradio");  // Always-on AP
    WiFi.begin(g_ssid, g_password);

    // Web server routes
    server.on("/",            HTTP_GET,  handle_root);
    server.on("/api/status",  HTTP_GET,  handle_api_status);
    server.on("/api/relay",   HTTP_POST, handle_api_relay);
    server.on("/api/reset_alarm", HTTP_POST, handle_api_reset_alarm);
    server.on("/api/reset_energy", HTTP_POST, handle_api_reset_energy);
    server.on("/config",      HTTP_GET,  handle_config_get);
    server.on("/config",      HTTP_POST, handle_config_post);
    server.begin();

    Serial.println("PSU Monitor ready");
    g_last_accum_ms = millis();
    draw_splash();
    delay(1500);
}

// ─────────────────────────────────────────────────────────────────────────────
// MAIN LOOP
// ─────────────────────────────────────────────────────────────────────────────
void loop() {
    static unsigned long t_sensor_ms = 0;
    static unsigned long t_display_ms = 0;
    static unsigned long t_nvs_ms = 0;

    server.handleClient();
    bms_receive();

    unsigned long now = millis();

    // Sensor poll: 500 ms
    if (now - t_sensor_ms >= 500) {
        t_sensor_ms = now;
        read_sensors();
        accumulate_energy();
        check_alarms();
        update_fan();
        update_relays();
    }

    // Display update: 250 ms
    if (now - t_display_ms >= 250) {
        t_display_ms = now;
        if (now - g_disp_rotate_ms >= DISP_ROTATE_INTERVAL) {
            g_disp_rotate_ms = now;
            g_disp_mode = (DisplayMode)((g_disp_mode + 1) % 3);  // Rotate main/battery/solar
        }
        draw_display();
    }

    // NVS save: every 60 s
    if (now - t_nvs_ms >= 60000) {
        t_nvs_ms = now;
        save_energy_nvs();
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// SENSOR READING
// ─────────────────────────────────────────────────────────────────────────────
void read_sensors() {
    // INA219 input channel
    g_data.v_in  = ina_in.getBusVoltage_V() + (ina_in.getShuntVoltage_mV() / 1000.0f);
    g_data.i_in  = ina_in.getCurrent_mA() / 1000.0f;
    g_data.p_in  = g_data.v_in * g_data.i_in;

    // INA219 output channel
    g_data.v_out  = ina_out.getBusVoltage_V() + (ina_out.getShuntVoltage_mV() / 1000.0f);
    g_data.i_out  = ina_out.getCurrent_mA() / 1000.0f;
    g_data.p_out  = g_data.v_out * g_data.i_out;

    // Efficiency
    if (g_data.p_in > 0.1f) {
        g_data.efficiency = (g_data.p_out / g_data.p_in) * 100.0f;
    } else {
        g_data.efficiency = 0;
    }

    // Temperature via ADS1115 channels 0 and 1
    g_data.temp_pass = read_ntc_ads1115(0);
    g_data.temp_bat  = read_ntc_ads1115(1);

    // Battery voltage from BMS or INA219 output
    if (g_bms.valid && (millis() - g_bms.last_rx_ms < 5000)) {
        g_data.v_bat = g_bms.pack_v;
    } else {
        g_data.v_bat = g_data.v_out;
    }

    g_data.timestamp = millis();
}

// Read NTC thermistor via ADS1115
float read_ntc_ads1115(uint8_t channel) {
    // Write config register: single-shot, channel selection, ±4.096V, 128 SPS
    Wire.beginTransmission(ADS1115_ADDR);
    Wire.write(0x01);  // Config register
    // MUX: channels 0=AIN0+GND(0x4000), 1=AIN1+GND(0x5000), etc.
    uint16_t mux = 0x4000 + (channel << 12);
    uint16_t config = mux | 0x0183;  // ±4.096V, single-shot, 128 SPS
    config |= 0x8000;                // OS bit = start conversion
    Wire.write((config >> 8) & 0xFF);
    Wire.write(config & 0xFF);
    Wire.endTransmission();
    delay(9);  // Wait for 128 SPS conversion (7.8 ms + margin)

    // Read conversion register
    Wire.beginTransmission(ADS1115_ADDR);
    Wire.write(0x00);  // Conversion register
    Wire.endTransmission();
    Wire.requestFrom(ADS1115_ADDR, (uint8_t)2);
    int16_t raw = ((int16_t)Wire.read() << 8) | Wire.read();
    float v_adc = (raw / 32768.0f) * 4.096f;
    if (v_adc < 0) v_adc = 0;

    return ntc_temp(v_adc);
}

float ntc_temp(float v_adc) {
    if (v_adc <= 0.01f || v_adc >= (V_SUPPLY - 0.01f)) return -99.0f;  // Sensor error
    float r_ntc = NTC_R_REF * v_adc / (V_SUPPLY - v_adc);
    float t_k   = NTC_B / (NTC_B / NTC_T0 + log(r_ntc / NTC_R_REF));
    return t_k - 273.15f;
}

// ─────────────────────────────────────────────────────────────────────────────
// JK BMS UART PARSER
// ─────────────────────────────────────────────────────────────────────────────
/*
 * JK BMS protocol: frames begin with 0x4E 0x57, followed by frame length,
 * terminal number, command word, and data. Checksum is XOR of all bytes.
 * Reference: github.com/jblance/jkbms (open-source decoder)
 *
 * Simplified: parse the most common frame (0x4E 0x57 ... cell voltages, pack V,
 * current, SOC, temperature).
 */
void bms_receive() {
    while (bmsSerial.available()) {
        uint8_t b = bmsSerial.read();
        if (g_bms_len == 0 && b != 0x4E) return;  // Wait for frame start
        if (g_bms_len == 1 && b != 0x57) { g_bms_len = 0; return; }
        if (g_bms_len < 256) g_bms_buf[g_bms_len++] = b;
        if (g_bms_len >= 4) {
            // Byte 2 and 3 are frame length (big-endian)
            uint16_t frame_len = ((uint16_t)g_bms_buf[2] << 8) | g_bms_buf[3];
            if (g_bms_len >= frame_len + 1) {
                bms_parse_frame(g_bms_buf, g_bms_len);
                g_bms_len = 0;
            }
        }
    }
}

void bms_parse_frame(uint8_t* buf, int len) {
    // Minimal JK BMS frame parser (cell info response, command 0x06)
    // Offset 11 onward: cell voltage records (2 bytes each, big-endian, mV)
    if (len < 20) return;

    // Check frame type: byte 10 should be 0x02 (cell info response) or 0x01
    // Full protocol parsing omitted for brevity; parse key fields by offset:
    // Offsets below valid for JK BMS 24S frame; adjust for your version.

    // Cell voltages: offset 11, 2 bytes each, N_cells determined from frame
    uint8_t n_cells = buf[10];  // Number of cells (typically 4 for 4S)
    if (n_cells > 4) n_cells = 4;
    for (uint8_t i = 0; i < n_cells && i < 4; i++) {
        int ofs = 11 + i * 2;
        if (ofs + 1 < len) {
            g_bms.cell_v[i] = ((uint16_t)buf[ofs] << 8 | buf[ofs + 1]) / 1000.0f;
        }
    }

    // Pack voltage: after cell voltages
    int v_ofs = 11 + n_cells * 2;
    if (v_ofs + 1 < len) {
        g_bms.pack_v = ((uint16_t)buf[v_ofs] << 8 | buf[v_ofs + 1]) / 100.0f;
    }

    // Current: next 2 bytes (signed, 10 mA/bit)
    if (v_ofs + 3 < len) {
        int16_t raw_i = (int16_t)((uint16_t)buf[v_ofs + 2] << 8 | buf[v_ofs + 3]);
        g_bms.pack_i = raw_i / 100.0f;  // Positive = charging
    }

    // SOC: next byte (%)
    if (v_ofs + 4 < len) {
        g_bms.soc = buf[v_ofs + 4];
    }

    // Temperature: next 2 bytes (0.1°C/bit, offset 2731 = 0°C)
    if (v_ofs + 7 < len) {
        g_bms.temp1 = (((uint16_t)buf[v_ofs + 5] << 8 | buf[v_ofs + 6]) - 2731) / 10.0f;
    }

    g_bms.valid       = true;
    g_bms.last_rx_ms  = millis();
}

// ─────────────────────────────────────────────────────────────────────────────
// ENERGY ACCUMULATION
// ─────────────────────────────────────────────────────────────────────────────
void accumulate_energy() {
    unsigned long now = millis();
    float dt_h = (now - g_last_accum_ms) / 3600000.0f;  // Delta time in hours
    g_last_accum_ms = now;

    if (g_data.p_in > 0) g_wh_in_accum  += g_data.p_in  * dt_h;
    if (g_data.p_out > 0) g_wh_out_accum += g_data.p_out * dt_h;

    g_data.wh_in  = g_wh_in_accum;
    g_data.wh_out = g_wh_out_accum;
}

void save_energy_nvs() {
    prefs.begin("psu", false);
    prefs.putFloat("wh_in",  g_wh_in_accum);
    prefs.putFloat("wh_out", g_wh_out_accum);
    prefs.end();
}

// ─────────────────────────────────────────────────────────────────────────────
// ALARM CHECKING
// ─────────────────────────────────────────────────────────────────────────────
void check_alarms() {
    bool alarm = false;

    if (g_data.v_out > g_alarms.v_ov)   alarm = true;  // Overvoltage
    if (g_data.v_out < g_alarms.v_uv && g_data.v_out > 1.0f) alarm = true;  // Undervoltage
    if (g_data.i_out > g_alarms.i_oc)   alarm = true;  // Overcurrent
    if (g_data.temp_pass > g_alarms.t_ot) alarm = true; // Over-temperature

    if (g_bms.valid) {
        if (g_bms.ov_fault || g_bms.uv_fault || g_bms.oc_fault) alarm = true;
    }

    if (alarm && !g_alarm_latched) {
        g_alarm_latched = true;
        g_alarm_active  = true;
        tone_alarm();
        if (g_alarms.relay_on_ov || g_alarms.relay_on_oc) {
            // De-energize all relays (failsafe — normally-closed contacts open load)
            for (int i = 0; i < 4; i++) {
                set_relay(i, false);
            }
        }
    }
    g_alarm_active = alarm;
}

void tone_alarm() {
    // Three short beeps
    for (int i = 0; i < 3; i++) {
        digitalWrite(PIN_BUZZER, HIGH);
        delay(100);
        digitalWrite(PIN_BUZZER, LOW);
        delay(100);
    }
}

// ISR for INA226 alert pin
void IRAM_ATTR isr_ina_alert() {
    // Minimal ISR: just set a flag; main loop handles the alarm
    g_alarm_active = true;
}

// ─────────────────────────────────────────────────────────────────────────────
// FAN CONTROL (thermal management)
// ─────────────────────────────────────────────────────────────────────────────
void update_fan() {
    float temp = max(g_data.temp_pass, g_data.temp_bat);
    uint8_t duty = 0;
    if (temp >= T_FAN_FULL) {
        duty = 255;
    } else if (temp >= T_FAN_ON) {
        duty = (uint8_t)(255.0f * (temp - T_FAN_ON) / (T_FAN_FULL - T_FAN_ON));
        if (duty < 80) duty = 80;  // Minimum duty to keep fan spinning
    }
    ledcWrite(0, duty);
}

// ─────────────────────────────────────────────────────────────────────────────
// RELAY SEQUENCER
// ─────────────────────────────────────────────────────────────────────────────
void set_relay(int idx, bool on) {
    int pins[] = { PIN_RELAY1, PIN_RELAY2, PIN_RELAY3, PIN_RELAY4 };
    if (idx < 0 || idx > 3) return;
    g_relays[idx] = on;
    digitalWrite(pins[idx], on ? HIGH : LOW);
}

void sequence_on() {
    g_sequencer_active = true;
    for (int i = 0; i < 4; i++) {
        delay(g_relay_on_time[i]);
        set_relay(i, true);
    }
    g_sequencer_active = false;
}

void sequence_off() {
    // Reverse order: amplifier first
    for (int i = 3; i >= 0; i--) {
        set_relay(i, false);
        delay(200);
    }
}

void update_relays() {
    // Nothing to do in main loop unless sequencer is running (handled in sequence_on/off)
}

// ─────────────────────────────────────────────────────────────────────────────
// DISPLAY
// ─────────────────────────────────────────────────────────────────────────────

// Color palette
#define C_BG        TFT_BLACK
#define C_TITLE     0x07FF    // Cyan
#define C_VALUE     TFT_WHITE
#define C_UNIT      0x8410    // Gray
#define C_GOOD      TFT_GREEN
#define C_WARN      TFT_YELLOW
#define C_ALARM     TFT_RED
#define C_GRID      0x2104    // Dark gray

void draw_display() {
    spr.fillSprite(C_BG);

    if (g_alarm_active && g_alarm_latched) {
        draw_alarm_screen();
    } else {
        switch (g_disp_mode) {
            case DM_MAIN:    draw_main_screen();    break;
            case DM_BATTERY: draw_battery_screen(); break;
            case DM_SOLAR:   draw_solar_screen();   break;
            default:         draw_main_screen();    break;
        }
    }

    spr.pushSprite(0, 0);
}

void draw_main_screen() {
    // Title bar
    spr.fillRect(0, 0, SCREEN_W, 24, C_TITLE);
    spr.setTextColor(TFT_BLACK);
    spr.setTextSize(2);
    spr.drawString("PSU MONITOR", 5, 4);

    // WiFi icon (top right)
    spr.setTextColor(WiFi.status() == WL_CONNECTED ? C_GOOD : C_WARN);
    spr.drawString("WiFi", 255, 4);

    // Large value display: V_out, I_out, P_out
    spr.setTextColor(C_UNIT);
    spr.setTextSize(1);
    spr.drawString("OUTPUT VOLTAGE", 5, 30);
    spr.drawString("OUTPUT CURRENT", 165, 30);

    // V_out
    spr.setTextColor(v_color(g_data.v_out));
    spr.setTextSize(3);
    char buf[16];
    snprintf(buf, sizeof(buf), "%5.2fV", g_data.v_out);
    spr.drawString(buf, 5, 42);

    // I_out
    snprintf(buf, sizeof(buf), "%5.2fA", g_data.i_out);
    spr.setTextColor(i_color(g_data.i_out));
    spr.drawString(buf, 165, 42);

    // P_out (large)
    spr.setTextColor(C_UNIT);
    spr.setTextSize(1);
    spr.drawString("OUTPUT POWER", 5, 82);
    spr.setTextColor(C_VALUE);
    spr.setTextSize(3);
    snprintf(buf, sizeof(buf), "%6.1fW", g_data.p_out);
    spr.drawString(buf, 5, 94);

    // Efficiency
    spr.setTextColor(C_UNIT);
    spr.setTextSize(1);
    spr.drawString("EFFICIENCY", 200, 82);
    spr.setTextColor(C_VALUE);
    spr.setTextSize(2);
    snprintf(buf, sizeof(buf), "%4.1f%%", g_data.efficiency);
    spr.drawString(buf, 200, 95);

    // Bottom row: temperatures + relay status
    spr.drawFastHLine(0, 140, SCREEN_W, C_GRID);
    spr.setTextSize(1);
    spr.setTextColor(C_UNIT);
    spr.drawString("PASS TEMP", 5, 148);
    spr.drawString("BAT TEMP", 100, 148);
    spr.drawString("ENERGY IN", 195, 148);

    spr.setTextSize(2);
    snprintf(buf, sizeof(buf), "%3.0fC", g_data.temp_pass);
    spr.setTextColor(t_color(g_data.temp_pass));
    spr.drawString(buf, 5, 160);

    snprintf(buf, sizeof(buf), "%3.0fC", g_data.temp_bat);
    spr.setTextColor(t_color(g_data.temp_bat));
    spr.drawString(buf, 100, 160);

    snprintf(buf, sizeof(buf), "%5.1fWh", g_data.wh_in);
    spr.setTextColor(C_VALUE);
    spr.drawString(buf, 195, 160);

    // Relay status dots
    spr.drawString("RELAYS:", 5, 195);
    const char* rnames[] = { "PSU", "AUD", "RF ", "AMP" };
    for (int i = 0; i < 4; i++) {
        spr.fillCircle(90 + i * 55, 201, 8, g_relays[i] ? C_GOOD : C_GRID);
        spr.setTextColor(C_VALUE);
        spr.drawString(rnames[i], 80 + i * 55, 215);
    }
}

void draw_battery_screen() {
    spr.fillRect(0, 0, SCREEN_W, 24, 0x0200);  // Dark green header
    spr.setTextColor(TFT_WHITE);
    spr.setTextSize(2);
    spr.drawString("LIFEPO4 BATTERY", 5, 4);

    char buf[32];

    if (!g_bms.valid || (millis() - g_bms.last_rx_ms > 5000)) {
        spr.setTextColor(C_WARN);
        spr.drawString("BMS not connected", 5, 50);
        spr.setTextColor(C_UNIT);
        spr.drawString("(Check UART wiring)", 5, 75);
        return;
    }

    // SOC bar
    spr.setTextColor(C_UNIT);
    spr.setTextSize(1);
    spr.drawString("STATE OF CHARGE", 5, 30);
    spr.setTextSize(3);
    spr.setTextColor(soc_color(g_bms.soc));
    snprintf(buf, sizeof(buf), "%3.0f%%", g_bms.soc);
    spr.drawString(buf, 5, 42);

    // SOC bar graph
    int bar_w = (int)(g_bms.soc * 2.5f);  // Max 250px at 100%
    spr.fillRect(5, 75, 250, 16, C_GRID);
    spr.fillRect(5, 75, bar_w, 16, soc_color(g_bms.soc));

    // Pack voltage
    spr.setTextSize(1);
    spr.setTextColor(C_UNIT);
    spr.drawString("PACK V", 5, 98);
    spr.drawString("CURRENT", 100, 98);
    spr.setTextSize(2);
    snprintf(buf, sizeof(buf), "%5.2fV", g_bms.pack_v);
    spr.setTextColor(C_VALUE);
    spr.drawString(buf, 5, 110);
    snprintf(buf, sizeof(buf), "%+5.2fA", g_bms.pack_i);
    spr.setTextColor(g_bms.pack_i >= 0 ? C_GOOD : C_WARN);
    spr.drawString(buf, 100, 110);

    // Cell voltages (4 cells)
    spr.drawFastHLine(0, 135, SCREEN_W, C_GRID);
    spr.setTextColor(C_UNIT);
    spr.setTextSize(1);
    spr.drawString("CELL VOLTAGES", 5, 142);

    for (int i = 0; i < 4; i++) {
        float cv = g_bms.cell_v[i];
        uint16_t cc = (cv > 3.55f) ? C_GOOD : (cv > 3.0f) ? C_VALUE : C_WARN;
        if (cv < 2.8f) cc = C_ALARM;
        spr.setTextColor(cc);
        spr.setTextSize(2);
        snprintf(buf, sizeof(buf), "C%d:%4.2fV", i + 1, cv);
        spr.drawString(buf, 5 + (i / 2) * 160, 155 + (i % 2) * 22);
    }

    // Temperature
    spr.drawFastHLine(0, 200, SCREEN_W, C_GRID);
    spr.setTextSize(1);
    spr.setTextColor(C_UNIT);
    spr.drawString("BMS T1:", 5, 208);
    spr.drawString("BMS T2:", 100, 208);
    spr.setTextColor(t_color(g_bms.temp1));
    snprintf(buf, sizeof(buf), "%3.0fC", g_bms.temp1);
    spr.setTextSize(2);
    spr.drawString(buf, 55, 205);
    spr.setTextColor(t_color(g_bms.temp2));
    snprintf(buf, sizeof(buf), "%3.0fC", g_bms.temp2);
    spr.drawString(buf, 150, 205);
}

void draw_solar_screen() {
    spr.fillRect(0, 0, SCREEN_W, 24, 0x6200);  // Dark orange
    spr.setTextColor(TFT_WHITE);
    spr.setTextSize(2);
    spr.drawString("SOLAR MPPT", 5, 4);

    char buf[32];

    spr.setTextSize(1);
    spr.setTextColor(C_UNIT);
    spr.drawString("PANEL VOLTAGE", 5, 30);
    spr.drawString("PANEL CURRENT", 165, 30);
    spr.setTextSize(3);
    spr.setTextColor(C_VALUE);
    snprintf(buf, sizeof(buf), "%5.1fV", g_data.v_in);
    spr.drawString(buf, 5, 42);
    snprintf(buf, sizeof(buf), "%4.2fA", g_data.i_in);
    spr.drawString(buf, 165, 42);

    spr.setTextSize(1);
    spr.setTextColor(C_UNIT);
    spr.drawString("PANEL POWER", 5, 82);
    spr.drawString("CHARGE POWER", 165, 82);
    spr.setTextSize(2);
    spr.setTextColor(C_GOOD);
    snprintf(buf, sizeof(buf), "%5.1fW", g_data.p_in);
    spr.drawString(buf, 5, 95);
    snprintf(buf, sizeof(buf), "%5.1fW", g_data.p_out);
    spr.drawString(buf, 165, 95);

    // Energy today
    spr.drawFastHLine(0, 120, SCREEN_W, C_GRID);
    spr.setTextSize(1);
    spr.setTextColor(C_UNIT);
    spr.drawString("ENERGY IN (session)", 5, 128);
    spr.drawString("ENERGY OUT (session)", 165, 128);
    spr.setTextSize(2);
    spr.setTextColor(C_VALUE);
    snprintf(buf, sizeof(buf), "%5.1fWh", g_data.wh_in);
    spr.drawString(buf, 5, 140);
    snprintf(buf, sizeof(buf), "%5.1fWh", g_data.wh_out);
    spr.drawString(buf, 165, 140);

    // Efficiency bar
    spr.setTextSize(1);
    spr.setTextColor(C_UNIT);
    spr.drawString("MPPT EFFICIENCY", 5, 168);
    int eff_w = (int)(g_data.efficiency * 2.5f);
    spr.fillRect(5, 180, 250, 14, C_GRID);
    spr.fillRect(5, 180, eff_w, 14, C_GOOD);
    spr.setTextColor(TFT_WHITE);
    spr.setTextSize(2);
    snprintf(buf, sizeof(buf), "%4.1f%%", g_data.efficiency);
    spr.drawString(buf, 255, 178);

    // Battery SOC
    spr.setTextSize(1);
    spr.setTextColor(C_UNIT);
    spr.drawString("BATTERY SOC:", 5, 205);
    spr.setTextSize(2);
    spr.setTextColor(g_bms.valid ? soc_color(g_bms.soc) : C_UNIT);
    snprintf(buf, sizeof(buf), g_bms.valid ? "%3.0f%%" : "---", g_bms.soc);
    spr.drawString(buf, 110, 202);
}

void draw_alarm_screen() {
    spr.fillScreen(C_ALARM);
    spr.setTextColor(TFT_WHITE);
    spr.setTextSize(3);
    spr.drawString("!! ALARM !!", 50, 30);

    spr.setTextSize(2);
    char buf[64];
    int y = 70;

    if (g_data.v_out > g_alarms.v_ov) {
        snprintf(buf, sizeof(buf), "OVERVOLT: %4.2fV", g_data.v_out);
        spr.drawString(buf, 5, y); y += 25;
    }
    if (g_data.v_out < g_alarms.v_uv && g_data.v_out > 1.0f) {
        snprintf(buf, sizeof(buf), "UNDERVOLT: %4.2fV", g_data.v_out);
        spr.drawString(buf, 5, y); y += 25;
    }
    if (g_data.i_out > g_alarms.i_oc) {
        snprintf(buf, sizeof(buf), "OVERCURRENT: %4.2fA", g_data.i_out);
        spr.drawString(buf, 5, y); y += 25;
    }
    if (g_data.temp_pass > g_alarms.t_ot) {
        snprintf(buf, sizeof(buf), "OVER-TEMP: %3.0fC", g_data.temp_pass);
        spr.drawString(buf, 5, y); y += 25;
    }
    if (g_bms.valid && (g_bms.ov_fault || g_bms.uv_fault || g_bms.oc_fault)) {
        spr.drawString("BMS FAULT (see app)", 5, y); y += 25;
    }

    spr.setTextSize(1);
    spr.drawString("Use web UI to reset alarm", 5, 215);
    char ip[20];
    snprintf(ip, sizeof(ip), "AP: 192.168.4.1", "");
    spr.drawString(ip, 180, 215);
}

void draw_splash() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_CYAN);
    tft.setTextSize(3);
    tft.drawString("PSU MONITOR", 20, 60);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.drawString("TM-PWR-001 Rev A", 30, 100);
    tft.setTextColor(TFT_DARKGREY);
    tft.setTextSize(1);
    tft.drawString("Solar / Linear / LiFePO4", 60, 135);
    tft.drawString("Initializing...", 110, 155);
}

// Color helpers
uint16_t v_color(float v) {
    if (v > g_alarms.v_ov || (v < g_alarms.v_uv && v > 1.0f)) return C_ALARM;
    if (v > 14.6f || v < 12.0f) return C_WARN;
    return C_GOOD;
}

uint16_t i_color(float i) {
    if (i > g_alarms.i_oc) return C_ALARM;
    if (i > g_alarms.i_oc * 0.8f) return C_WARN;
    return C_VALUE;
}

uint16_t t_color(float t) {
    if (t > g_alarms.t_ot) return C_ALARM;
    if (t > g_alarms.t_ot * 0.8f) return C_WARN;
    return C_GOOD;
}

uint16_t soc_color(float soc) {
    if (soc > 60) return C_GOOD;
    if (soc > 20) return C_WARN;
    return C_ALARM;
}

// ─────────────────────────────────────────────────────────────────────────────
// WEB SERVER
// ─────────────────────────────────────────────────────────────────────────────
void handle_root() {
    String html = R"html(<!DOCTYPE html>
<html><head><meta charset='utf-8'>
<meta name='viewport' content='width=device-width,initial-scale=1'>
<title>PSU Monitor</title>
<style>
body{font-family:monospace;background:#111;color:#eee;margin:0;padding:10px}
h1{color:#0ff;margin-bottom:5px}
.grid{display:grid;grid-template-columns:repeat(3,1fr);gap:10px;margin-bottom:15px}
.card{background:#222;border:1px solid #444;border-radius:6px;padding:10px}
.card h3{color:#0af;margin:0 0 6px}
.val{font-size:1.8em;font-weight:bold}
.unit{color:#888;font-size:0.9em}
.ok{color:#0f0}.warn{color:#ff0}.alarm{color:#f00}
.bar-bg{background:#333;height:16px;border-radius:3px;margin:4px 0}
.bar{height:16px;border-radius:3px;transition:width 0.5s}
button{background:#224;border:1px solid #48f;color:#eee;padding:8px 16px;
       border-radius:4px;cursor:pointer;margin:3px}
button:hover{background:#336}
input,select{background:#222;border:1px solid #555;color:#eee;padding:4px;border-radius:3px}
table{width:100%;border-collapse:collapse}
td,th{padding:4px 8px;border:1px solid #333;text-align:left}
th{color:#0af;background:#1a1a1a}
</style>
<script>
function refresh(){
  fetch('/api/status').then(r=>r.json()).then(d=>{
    document.getElementById('v_out').textContent=d.v_out.toFixed(2)+'V';
    document.getElementById('i_out').textContent=d.i_out.toFixed(2)+'A';
    document.getElementById('p_out').textContent=d.p_out.toFixed(1)+'W';
    document.getElementById('eff').textContent=d.eff.toFixed(1)+'%';
    document.getElementById('temp_pass').textContent=d.temp_pass.toFixed(0)+'°C';
    document.getElementById('temp_bat').textContent=d.temp_bat.toFixed(0)+'°C';
    document.getElementById('soc').textContent=d.soc.toFixed(0)+'%';
    document.getElementById('wh_in').textContent=d.wh_in.toFixed(1)+'Wh';
    document.getElementById('wh_out').textContent=d.wh_out.toFixed(1)+'Wh';
    document.getElementById('alarm').textContent=d.alarm?'ACTIVE':'OK';
    document.getElementById('alarm').className=d.alarm?'val alarm':'val ok';
    var soc_bar=document.getElementById('soc_bar');
    soc_bar.style.width=d.soc+'%';
    soc_bar.style.background=d.soc>60?'#0f0':d.soc>20?'#ff0':'#f00';
  });
}
setInterval(refresh, 1000);
window.onload=refresh;
function relayCmd(idx,on){
  fetch('/api/relay',{method:'POST',
    headers:{'Content-Type':'application/x-www-form-urlencoded'},
    body:'relay='+idx+'&state='+(on?1:0)});
}
function seqOn(){fetch('/api/relay',{method:'POST',
  headers:{'Content-Type':'application/x-www-form-urlencoded'},
  body:'relay=all&state=1'});}
function seqOff(){fetch('/api/relay',{method:'POST',
  headers:{'Content-Type':'application/x-www-form-urlencoded'},
  body:'relay=all&state=0'});}
function resetAlarm(){fetch('/api/reset_alarm',{method:'POST'});}
function resetEnergy(){if(confirm('Reset energy counters?'))
  fetch('/api/reset_energy',{method:'POST'});}
</script>
</head><body>
<h1>PSU Monitor — TM-PWR-001</h1>
<div class='grid'>
  <div class='card'><h3>Output</h3>
    <div><span class='unit'>Voltage: </span><span class='val' id='v_out'>--</span></div>
    <div><span class='unit'>Current: </span><span class='val' id='i_out'>--</span></div>
    <div><span class='unit'>Power:   </span><span class='val' id='p_out'>--</span></div>
  </div>
  <div class='card'><h3>Efficiency &amp; Temp</h3>
    <div><span class='unit'>Efficiency: </span><span class='val' id='eff'>--</span></div>
    <div><span class='unit'>Pass temp:  </span><span class='val' id='temp_pass'>--</span></div>
    <div><span class='unit'>Bat temp:   </span><span class='val' id='temp_bat'>--</span></div>
  </div>
  <div class='card'><h3>Battery SOC</h3>
    <div class='val' id='soc'>--</div>
    <div class='bar-bg'><div class='bar' id='soc_bar' style='width:0%'></div></div>
    <div><span class='unit'>Wh in:  </span><span id='wh_in'>--</span></div>
    <div><span class='unit'>Wh out: </span><span id='wh_out'>--</span></div>
  </div>
</div>
<div class='card' style='margin-bottom:10px'>
  <h3>Alarm Status: <span id='alarm' class='val ok'>OK</span></h3>
  <button onclick='resetAlarm()'>Reset Alarm</button>
  <button onclick='resetEnergy()'>Reset Energy</button>
</div>
<div class='card' style='margin-bottom:10px'>
  <h3>Power Sequencer</h3>
  <button onclick='seqOn()'>SEQUENCE ON</button>
  <button onclick='seqOff()'>SEQUENCE OFF</button>
  <br><br>
  <button onclick='relayCmd(0,1)'>PSU ON</button>
  <button onclick='relayCmd(0,0)'>PSU OFF</button>
  &nbsp;
  <button onclick='relayCmd(1,1)'>AUDIO ON</button>
  <button onclick='relayCmd(1,0)'>AUDIO OFF</button>
  &nbsp;
  <button onclick='relayCmd(2,1)'>RF ON</button>
  <button onclick='relayCmd(2,0)'>RF OFF</button>
  &nbsp;
  <button onclick='relayCmd(3,1)'>AMP ON</button>
  <button onclick='relayCmd(3,0)'>AMP OFF</button>
</div>
<p><a href='/config' style='color:#0af'>Configuration</a></p>
</body></html>)html";
    server.send(200, "text/html", html);
}

void handle_api_status() {
    StaticJsonDocument<512> doc;
    doc["v_out"]     = g_data.v_out;
    doc["i_out"]     = g_data.i_out;
    doc["p_out"]     = g_data.p_out;
    doc["v_in"]      = g_data.v_in;
    doc["i_in"]      = g_data.i_in;
    doc["p_in"]      = g_data.p_in;
    doc["eff"]       = g_data.efficiency;
    doc["temp_pass"] = g_data.temp_pass;
    doc["temp_bat"]  = g_data.temp_bat;
    doc["wh_in"]     = g_data.wh_in;
    doc["wh_out"]    = g_data.wh_out;
    doc["alarm"]     = g_alarm_active;
    doc["soc"]       = g_bms.valid ? g_bms.soc : -1.0f;
    doc["bms_ok"]    = g_bms.valid;
    for (int i = 0; i < 4; i++) doc["relay"][i] = g_relays[i];

    String out;
    serializeJson(doc, out);
    server.send(200, "application/json", out);
}

void handle_api_relay() {
    if (!server.hasArg("relay") || !server.hasArg("state")) {
        server.send(400, "text/plain", "Missing args");
        return;
    }
    String r = server.arg("relay");
    bool on = server.arg("state") == "1";

    if (r == "all") {
        if (on) sequence_on();
        else    sequence_off();
    } else {
        int idx = r.toInt();
        if (idx >= 0 && idx <= 3) set_relay(idx, on);
    }
    server.send(200, "text/plain", "OK");
}

void handle_api_reset_alarm() {
    g_alarm_latched = false;
    g_alarm_active  = false;
    server.send(200, "text/plain", "OK");
}

void handle_api_reset_energy() {
    g_wh_in_accum  = 0;
    g_wh_out_accum = 0;
    save_energy_nvs();
    server.send(200, "text/plain", "OK");
}

void handle_config_get() {
    char html[2048];
    snprintf(html, sizeof(html),
        "<!DOCTYPE html><html><head><title>PSU Config</title>"
        "<style>body{font-family:monospace;background:#111;color:#eee;padding:10px}"
        "input{background:#222;border:1px solid #555;color:#eee;padding:4px;width:150px}"
        "label{display:inline-block;width:180px;color:#0af}"
        "button{background:#224;border:1px solid #48f;color:#eee;padding:8px 16px;"
        "border-radius:4px;cursor:pointer;margin-top:12px}"
        "</style></head><body>"
        "<h2>PSU Configuration</h2>"
        "<form method='POST' action='/config'>"
        "<h3>WiFi</h3>"
        "<label>SSID:</label><input name='ssid' value='%s'><br><br>"
        "<label>Password:</label><input type='password' name='pass' value='%s'><br><br>"
        "<h3>Alarm Thresholds</h3>"
        "<label>Overvoltage (V):</label><input name='v_ov' value='%.1f'><br><br>"
        "<label>Undervoltage (V):</label><input name='v_uv' value='%.1f'><br><br>"
        "<label>Overcurrent (A):</label><input name='i_oc' value='%.1f'><br><br>"
        "<label>Over-temp (C):</label><input name='t_ot' value='%.0f'><br><br>"
        "<button type='submit'>Save</button>"
        "</form><p><a href='/' style='color:#0af'>Back</a></p>"
        "</body></html>",
        g_ssid, g_password,
        g_alarms.v_ov, g_alarms.v_uv, g_alarms.i_oc, g_alarms.t_ot);
    server.send(200, "text/html", html);
}

void handle_config_post() {
    if (server.hasArg("ssid"))    strlcpy(g_ssid,     server.arg("ssid").c_str(), 64);
    if (server.hasArg("pass"))    strlcpy(g_password, server.arg("pass").c_str(), 64);
    if (server.hasArg("v_ov"))    g_alarms.v_ov = server.arg("v_ov").toFloat();
    if (server.hasArg("v_uv"))    g_alarms.v_uv = server.arg("v_uv").toFloat();
    if (server.hasArg("i_oc"))    g_alarms.i_oc = server.arg("i_oc").toFloat();
    if (server.hasArg("t_ot"))    g_alarms.t_ot = server.arg("t_ot").toFloat();

    prefs.begin("psu", false);
    prefs.putString("ssid", g_ssid);
    prefs.putString("pass", g_password);
    prefs.putFloat("v_ov",  g_alarms.v_ov);
    prefs.putFloat("v_uv",  g_alarms.v_uv);
    prefs.putFloat("i_oc",  g_alarms.i_oc);
    prefs.putFloat("t_ot",  g_alarms.t_ot);
    prefs.end();

    WiFi.begin(g_ssid, g_password);

    server.sendHeader("Location", "/config");
    server.send(303);
}

// ─── TFT_eSPI User_Setup.h required for CYD ─────────────────────────────────
/*
  In User_Setup.h for ESP32-2432S028 (CYD):
    #define ILI9341_DRIVER
    #define TFT_MISO  12
    #define TFT_MOSI  13
    #define TFT_SCLK  14
    #define TFT_CS    15
    #define TFT_DC     2
    #define TFT_RST   -1
    #define TOUCH_CS   33
    #define TFT_BL     21
    #define TFT_BACKLIGHT_ON HIGH
    #define LOAD_GLCD
    #define LOAD_FONT2
    #define LOAD_FONT4
    #define SPI_FREQUENCY  27000000
    #define SPI_READ_FREQUENCY  20000000
    #define SPI_TOUCH_FREQUENCY  2500000
*/
