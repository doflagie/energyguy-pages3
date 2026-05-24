/*
 * DUMMY LOAD POWER MONITOR & SWR DISPLAY
 * CYD (ESP32-2432S028) — ILI9341 2.8" TFT + Resistive Touch
 *
 * DISPLAYS:
 *   - Forward power (watts, peak and average)
 *   - Reflected power (watts)
 *   - SWR (calculated)
 *   - Reflection coefficient (rho)
 *   - Oil/heatsink temperature (NTC thermistor)
 *   - Duty cycle (active %)
 *   - Cumulative watt-seconds (energy deposited)
 *   - Over-temperature alarm and auto-cutoff relay
 *
 * HARDWARE REQUIRED:
 *   - CYD (ESP32-2432S028)
 *   - Directional coupler (see schematics/sch_dir_coupler.txt)
 *     Forward detector output -> GPIO39
 *     Reflected detector output -> GPIO36 (or GPIO34)
 *   - NTC thermistor 10k @ 25C, B=3950
 *     In voltage divider: 3.3V -- 10k fixed -- GPIO35 -- NTC -- GND
 *   - Fan control: GPIO26 (DAC or PWM)
 *   - Relay (over-temp cutoff): GPIO27
 *
 * COUPLER CALIBRATION:
 *   Set CALIB_K_FWD and CALIB_K_REF below using known power source.
 *   Default values assume 20 dB coupling factor.
 *
 * TFT_eSPI USER SETUP (User_Setup.h):
 *   #define ILI9341_DRIVER
 *   #define TFT_CS 15, TFT_DC 2, TFT_RST 12
 *   #define TFT_MOSI 13, TFT_SCLK 14
 *   #define TOUCH_CS 33
 *
 * LICENSE: MIT  AUTHOR: Generated for Merv Martin, 2026
 */

#include <TFT_eSPI.h>
#include <EEPROM.h>
#include <math.h>

// ============================================================
// Pin assignments (CYD)
// ============================================================
#define PIN_FWD_ADC    39    // Forward power detector (GPIO39 = VP)
#define PIN_REF_ADC    36    // Reflected power detector (GPIO36 = VN)
#define PIN_TEMP_ADC   35    // NTC thermistor divider
#define PIN_FAN_PWM    26    // Fan speed (0-255 PWM or DAC)
#define PIN_RELAY      27    // Over-temp relay (HIGH = energized/on)
#define PIN_LED_R       4    // CYD red LED (active LOW)
#define PIN_LED_G      16    // CYD green LED (check your variant)

// ============================================================
// Calibration constants
// Adjust after measuring with known power source
// ============================================================
// K = power_watts / (V_detector * V_detector)
// For 20 dB coupler + BAT54 detector, typical K ~ 200-400
// Measure: apply 10W, read V_fwd ADC, K = 10.0 / (V_fwd * V_fwd)
float CALIB_K_FWD    = 250.0f;   // forward power factor (W/V^2)
float CALIB_K_REF    = 250.0f;   // reflected power factor (W/V^2)
float CALIB_K_TEMP   = 10000.0f; // NTC resistance at 25C
float CALIB_B_TEMP   = 3950.0f;  // NTC B parameter

// ============================================================
// Thresholds
// ============================================================
#define TEMP_WARN_C     80.0f    // yellow warning
#define TEMP_ALARM_C    95.0f    // red alarm + relay cutoff
#define TEMP_HYSTERESIS  5.0f    // reset below alarm - hysteresis
#define SWR_WARN        2.0f
#define SWR_ALARM       3.0f

// ============================================================
// Fan control thresholds (C)
// ============================================================
#define FAN_ON_TEMP     45.0f
#define FAN_FULL_TEMP   70.0f

// ============================================================
// Averaging and display
// ============================================================
#define ADC_OVERSAMPLE   64
#define PEAK_HOLD_MS   3000    // peak hold duration
#define AVG_WINDOW_MS  1000    // averaging window
#define DISPLAY_UPDATE_MS  100 // display refresh rate

// ============================================================
// EEPROM layout
// ============================================================
#define EE_K_FWD      0    // float, 4 bytes
#define EE_K_REF      4
#define EE_CAL_FLAG   8    // 0xAB if calibrated

// ============================================================
// Colors
// ============================================================
#define C_BG        TFT_BLACK
#define C_HEADER    0x0350   // dark navy blue
#define C_ACCENT    0xFD20   // orange
#define C_PASS      TFT_GREEN
#define C_WARN      TFT_YELLOW
#define C_FAIL      TFT_RED
#define C_TEXT      TFT_WHITE
#define C_GRID      0x4208
#define C_DARK      0x2104
#define C_FWDBAR    0x07E0   // bright green
#define C_REFBAR    0xF800   // bright red
#define C_SWRBAR    0x001F   // bright blue

TFT_eSPI tft = TFT_eSPI();

// ============================================================
// State variables
// ============================================================
float fwd_power_w    = 0.0f;
float ref_power_w    = 0.0f;
float swr            = 1.0f;
float rho            = 0.0f;
float temp_c         = 25.0f;
float peak_fwd_w     = 0.0f;
float avg_fwd_w      = 0.0f;
float duty_pct       = 0.0f;
double watt_seconds  = 0.0;   // cumulative energy

bool  temp_alarm     = false;
bool  relay_tripped  = false;
uint8_t screen_mode  = 0;     // 0=main, 1=cal, 2=energy, 3=setup

uint32_t last_display = 0;
uint32_t last_update  = 0;
uint32_t peak_time    = 0;
uint32_t tx_start_ms  = 0;
uint32_t tx_total_ms  = 0;
uint32_t session_ms   = 0;

// Rolling average buffer
#define AVG_SAMPLES 20
float avg_buf[AVG_SAMPLES] = {0};
int   avg_idx = 0;

// ============================================================
// ADC helper — oversample for noise rejection
// ============================================================
float read_adc_v(int pin) {
    long sum = 0;
    for (int i = 0; i < ADC_OVERSAMPLE; i++) {
        sum += analogRead(pin);
        delayMicroseconds(30);
    }
    // ESP32 ADC: 12-bit, 3.3V full scale at ADC_11db attenuation
    return (float)sum / ADC_OVERSAMPLE * 3.3f / 4095.0f;
}

// ============================================================
// Temperature from NTC (Steinhart-Hart / B-parameter method)
// ============================================================
float read_temperature() {
    float v = read_adc_v(PIN_TEMP_ADC);
    if (v < 0.05f || v > 3.25f) return -99.0f;  // open/short
    // Voltage divider: 3.3V -- 10k -- ADC -- NTC -- GND
    float r_ntc = 10000.0f * v / (3.3f - v);
    // B-parameter equation
    float t_k = 1.0f / (1.0f/(25.0f + 273.15f) + log(r_ntc/CALIB_K_TEMP)/CALIB_B_TEMP);
    return t_k - 273.15f;
}

// ============================================================
// Power measurement
// ============================================================
void measure_power() {
    float v_fwd = read_adc_v(PIN_FWD_ADC);
    float v_ref = read_adc_v(PIN_REF_ADC);

    // Schottky forward voltage offset (~0.2V for BAT54)
    v_fwd = max(0.0f, v_fwd - 0.20f);
    v_ref = max(0.0f, v_ref - 0.20f);

    fwd_power_w = CALIB_K_FWD * v_fwd * v_fwd;
    ref_power_w = CALIB_K_REF * v_ref * v_ref;

    // SWR calculation
    if (fwd_power_w > 0.1f) {
        rho = sqrtf(ref_power_w / fwd_power_w);
        rho = constrain(rho, 0.0f, 0.999f);
        swr = (1.0f + rho) / (1.0f - rho);
        swr = constrain(swr, 1.0f, 99.0f);
    } else {
        rho = 0.0f;
        swr = 1.0f;
    }

    // Rolling average
    avg_buf[avg_idx] = fwd_power_w;
    avg_idx = (avg_idx + 1) % AVG_SAMPLES;
    float sum = 0;
    for (int i = 0; i < AVG_SAMPLES; i++) sum += avg_buf[i];
    avg_fwd_w = sum / AVG_SAMPLES;

    // Peak hold
    if (fwd_power_w > peak_fwd_w) {
        peak_fwd_w = fwd_power_w;
        peak_time = millis();
    } else if (millis() - peak_time > PEAK_HOLD_MS) {
        peak_fwd_w = fwd_power_w;
    }

    // Duty cycle tracking
    uint32_t now = millis();
    session_ms = now;
    if (fwd_power_w > 1.0f) {
        if (tx_start_ms == 0) tx_start_ms = now;
        tx_total_ms += (now - last_update);
        watt_seconds += fwd_power_w * (now - last_update) / 1000.0;
    } else {
        tx_start_ms = 0;
    }
    if (session_ms > 0)
        duty_pct = (float)tx_total_ms / session_ms * 100.0f;

    last_update = now;
}

// ============================================================
// Fan control (temperature-based PWM)
// ============================================================
void control_fan() {
    if (temp_c < FAN_ON_TEMP) {
        ledcWrite(1, 0);   // fan off
    } else if (temp_c > FAN_FULL_TEMP) {
        ledcWrite(1, 255); // full speed
    } else {
        uint8_t spd = (uint8_t)((temp_c - FAN_ON_TEMP) /
                      (FAN_FULL_TEMP - FAN_ON_TEMP) * 255.0f);
        ledcWrite(1, spd);
    }
}

// ============================================================
// Over-temp protection
// ============================================================
void check_overtemp() {
    if (temp_c >= TEMP_ALARM_C && !relay_tripped) {
        relay_tripped = true;
        digitalWrite(PIN_RELAY, LOW);   // de-energize (open circuit)
        digitalWrite(PIN_LED_R, LOW);   // active LOW: RED on
    }
    if (relay_tripped && temp_c < (TEMP_ALARM_C - TEMP_HYSTERESIS)) {
        relay_tripped = false;
        digitalWrite(PIN_RELAY, HIGH);  // re-energize (closed)
        digitalWrite(PIN_LED_R, HIGH);  // LED off
    }
    temp_alarm = (temp_c >= TEMP_WARN_C);
}

// ============================================================
// Bargraph helper
// ============================================================
void draw_bargraph(int x, int y, int w, int h,
                   float val, float max_val,
                   uint16_t bar_color, const char* label,
                   const char* unit_str) {
    float pct = constrain(val / max_val, 0.0f, 1.0f);
    int bar_w = (int)(pct * (w - 2));

    // Background
    tft.fillRect(x, y, w, h, C_DARK);
    tft.drawRect(x, y, w, h, C_GRID);

    // Bar fill
    if (bar_w > 0)
        tft.fillRect(x + 1, y + 1, bar_w, h - 2, bar_color);

    // Label
    tft.setTextColor(C_TEXT, C_DARK);
    tft.setTextSize(1);
    tft.setCursor(x + 3, y + 2);
    tft.print(label);

    // Value (right justified)
    char buf[20];
    if (val >= 1000.0f)
        snprintf(buf, sizeof(buf), "%.1fkW", val/1000.0f);
    else
        snprintf(buf, sizeof(buf), "%.1f%s", val, unit_str);
    tft.setCursor(x + w - strlen(buf)*6 - 3, y + 2);
    tft.print(buf);
}

// ============================================================
// Main display screen
// ============================================================
void draw_main_screen() {
    // Header
    tft.fillRect(0, 0, 320, 22, C_HEADER);
    tft.setTextColor(C_TEXT, C_HEADER);
    tft.setTextSize(2);
    tft.setCursor(6, 4);
    tft.print("RF POWER MONITOR");
    if (relay_tripped) {
        tft.setTextColor(C_FAIL, C_HEADER);
        tft.setCursor(230, 4);
        tft.print("TRIP");
    }

    // Forward power (large)
    tft.setTextColor(C_FWDBAR, C_BG);
    tft.setTextSize(3);
    tft.setCursor(6, 27);
    char fbuf[16];
    if (fwd_power_w >= 1000.0f)
        snprintf(fbuf, sizeof(fbuf), "%.2fkW", fwd_power_w/1000.0f);
    else
        snprintf(fbuf, sizeof(fbuf), "%.1fW", fwd_power_w);
    tft.print(fbuf);
    tft.setTextSize(1);
    tft.setTextColor(C_GRID, C_BG);
    tft.setCursor(6, 58);
    tft.printf("AVG: %.1fW  PEAK: %.1fW", avg_fwd_w, peak_fwd_w);

    // Bargraphs
    int bg_y = 70;
    draw_bargraph(4, bg_y,      152, 16, fwd_power_w, 1500.0f, C_FWDBAR, "FWD", "W");
    draw_bargraph(164, bg_y,    152, 16, ref_power_w, 150.0f,  C_REFBAR, "REF", "W");
    bg_y += 20;
    draw_bargraph(4, bg_y,      152, 16, swr - 1.0f,  9.0f,    C_SWRBAR, "SWR", "");
    draw_bargraph(164, bg_y,    152, 16, temp_c, 120.0f,
                  temp_alarm ? C_FAIL : C_WARN, "TEMP", "C");

    // SWR digital readout
    uint16_t swr_col = swr < SWR_WARN ? C_PASS : swr < SWR_ALARM ? C_WARN : C_FAIL;
    tft.fillRect(4, bg_y + 20, 158, 36, C_DARK);
    tft.drawRect(4, bg_y + 20, 158, 36, swr_col);
    tft.setTextColor(swr_col, C_DARK);
    tft.setTextSize(3);
    tft.setCursor(10, bg_y + 27);
    char swrbuf[10];
    snprintf(swrbuf, sizeof(swrbuf), "%.2f:1", swr);
    tft.print(swrbuf);
    tft.setTextSize(1);
    tft.setTextColor(C_TEXT, C_DARK);
    tft.setCursor(10, bg_y + 22);
    tft.print("SWR");

    // Return loss
    float rl_db = -20.0f * log10f(max(rho, 1e-6f));
    tft.setTextColor(C_TEXT, C_DARK);
    tft.setCursor(100, bg_y + 22);
    char rlbuf[12];
    snprintf(rlbuf, sizeof(rlbuf), "RL:%.1fdB", rl_db);
    tft.print(rlbuf);

    // Temperature
    tft.fillRect(164, bg_y + 20, 152, 36, C_DARK);
    tft.drawRect(164, bg_y + 20, 152, 36,
                 temp_c > TEMP_WARN_C ? C_FAIL : C_GRID);
    tft.setTextSize(3);
    tft.setTextColor(temp_alarm ? C_FAIL : C_TEXT, C_DARK);
    tft.setCursor(170, bg_y + 27);
    char tbuf[10];
    snprintf(tbuf, sizeof(tbuf), "%.1fC", temp_c);
    tft.print(tbuf);
    tft.setTextSize(1);
    tft.setTextColor(C_TEXT, C_DARK);
    tft.setCursor(170, bg_y + 22);
    tft.print("OIL TEMP");

    // Bottom stats bar
    tft.fillRect(0, 210, 320, 30, 0x0820);
    tft.setTextSize(1);
    tft.setTextColor(C_TEXT, 0x0820);
    tft.setCursor(4, 215);
    tft.printf("DUTY: %.0f%%", duty_pct);
    tft.setCursor(80, 215);
    char wsbuf[16];
    double ws = watt_seconds;
    if (ws >= 3600.0) snprintf(wsbuf, sizeof(wsbuf), "%.2fWh", ws/3600.0);
    else snprintf(wsbuf, sizeof(wsbuf), "%.0fWs", ws);
    tft.printf("ENERGY: %s", wsbuf);
    tft.setCursor(220, 215);
    tft.printf("TX: %.0fs", (float)tx_total_ms/1000.0f);

    // Mode switch hint
    tft.setTextColor(C_GRID, 0x0820);
    tft.setCursor(4, 227);
    tft.print("TAP:SCREEN  HOLD:CAL");
}

// ============================================================
// Energy / duty cycle detail screen
// ============================================================
void draw_energy_screen() {
    tft.fillScreen(C_BG);
    tft.fillRect(0, 0, 320, 22, C_HEADER);
    tft.setTextColor(C_TEXT, C_HEADER);
    tft.setTextSize(2);
    tft.setCursor(6, 4);
    tft.print("ENERGY & DUTY CYCLE");

    tft.setTextSize(2);
    tft.setTextColor(C_ACCENT, C_BG);
    tft.setCursor(6, 30);
    tft.printf("Session TX: %.0f sec", (float)tx_total_ms/1000.0f);
    tft.setCursor(6, 52);
    tft.printf("Duty cycle: %.1f%%", duty_pct);
    tft.setCursor(6, 74);
    double wh = watt_seconds / 3600.0;
    tft.printf("Energy: %.3f Wh", wh);
    tft.setCursor(6, 96);
    tft.printf("Avg power: %.1f W", avg_fwd_w);
    tft.setCursor(6, 118);
    tft.printf("Peak power: %.1f W", peak_fwd_w);

    // Derating curve reminder
    tft.setTextSize(1);
    tft.setTextColor(C_GRID, C_BG);
    tft.setCursor(6, 150);
    tft.print("DERATING: see MANUAL_DUMMY_LOADS.TXT");
    tft.setCursor(6, 162);
    tft.print("100W load: 100% duty continuous");
    tft.setCursor(6, 174);
    tft.print("500W oil: 100% duty (good airflow)");
    tft.setCursor(6, 186);
    tft.print("1kW oil: 30% duty max without cooling");

    // Reset button
    tft.fillRoundRect(200, 195, 110, 30, 4, 0x8000);
    tft.setTextColor(C_TEXT);
    tft.setTextSize(2);
    tft.setCursor(212, 202);
    tft.print("RESET");
}

// ============================================================
// Touch handling
// ============================================================
uint32_t touch_down_time = 0;

void handle_touch(int tx, int ty) {
    uint32_t held = millis() - touch_down_time;

    if (screen_mode == 0) {
        // Long press (>1s) = enter calibration
        if (held > 1000) {
            screen_mode = 1;
            draw_cal_screen();
            return;
        }
        // Short tap = cycle screens
        screen_mode = (screen_mode + 1) % 3;
    } else if (screen_mode == 2) {
        // Reset energy on button tap
        if (tx > 200 && tx < 310 && ty > 195 && ty < 225) {
            tx_total_ms  = 0;
            tx_start_ms  = 0;
            watt_seconds = 0.0;
            duty_pct     = 0.0f;
            session_ms   = 0;
            peak_fwd_w   = 0.0f;
        } else {
            screen_mode = 0;
        }
    } else {
        screen_mode = 0;
    }
    tft.fillScreen(C_BG);
}

void draw_cal_screen() {
    tft.fillScreen(C_BG);
    tft.fillRect(0, 0, 320, 22, 0x8000);
    tft.setTextColor(C_TEXT, 0x8000);
    tft.setTextSize(2);
    tft.setCursor(6, 4);
    tft.print("CALIBRATION MODE");
    tft.setTextSize(1);
    tft.setTextColor(C_TEXT, C_BG);
    tft.setCursor(6, 28);
    tft.print("Connect known power source to load.");
    tft.setCursor(6, 40);
    tft.print("Apply exactly 10.0W. Read V_fwd below.");
    tft.setCursor(6, 52);
    tft.print("K = 10.0 / (V_fwd * V_fwd)");
    tft.setCursor(6, 80);
    char buf[40];
    float v = read_adc_v(PIN_FWD_ADC);
    snprintf(buf, sizeof(buf), "V_fwd = %.4f V", v);
    tft.setTextColor(C_ACCENT, C_BG);
    tft.setTextSize(2);
    tft.print(buf);
    tft.setTextSize(1);
    tft.setTextColor(C_TEXT, C_BG);
    tft.setCursor(6, 110);
    snprintf(buf, sizeof(buf), "K_calc = %.1f", v > 0.01f ? 10.0f/(v*v) : 0.0f);
    tft.print(buf);
    tft.setCursor(6, 125);
    tft.print("Tap APPLY to save, anywhere else to exit.");
    tft.fillRoundRect(100, 180, 120, 36, 4, C_PASS);
    tft.setTextColor(TFT_BLACK, C_PASS);
    tft.setTextSize(2);
    tft.setCursor(113, 192);
    tft.print("APPLY");
}

// ============================================================
// Setup
// ============================================================
void setup() {
    Serial.begin(115200);
    Serial.println("RF Power Monitor v1.0");

    pinMode(PIN_LED_R, OUTPUT); digitalWrite(PIN_LED_R, HIGH);
    pinMode(PIN_LED_G, OUTPUT); digitalWrite(PIN_LED_G, HIGH);
    pinMode(PIN_RELAY, OUTPUT); digitalWrite(PIN_RELAY, HIGH); // relay on (normal)

    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);

    // Fan PWM
    ledcSetup(1, 25000, 8);
    ledcAttachPin(PIN_FAN_PWM, 1);
    ledcWrite(1, 0);

    // TFT
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(C_BG);

    // Backlight
    ledcSetup(0, 5000, 8);
    ledcAttachPin(21, 0);
    ledcWrite(0, 200);

    // Load calibration from EEPROM
    EEPROM.begin(16);
    if (EEPROM.read(EE_CAL_FLAG) == 0xAB) {
        EEPROM.get(EE_K_FWD, CALIB_K_FWD);
        EEPROM.get(EE_K_REF, CALIB_K_REF);
        Serial.printf("Cal loaded: K_fwd=%.1f K_ref=%.1f\n",
                      CALIB_K_FWD, CALIB_K_REF);
    }

    draw_main_screen();
}

// ============================================================
// Main loop
// ============================================================
void loop() {
    uint32_t now = millis();

    // Measure
    measure_power();
    temp_c = read_temperature();
    if (temp_c < -50.0f) temp_c = 25.0f; // sensor fault fallback

    // Thermal management
    control_fan();
    check_overtemp();

    // Display update
    if (now - last_display > DISPLAY_UPDATE_MS) {
        last_display = now;
        if (screen_mode == 0)       draw_main_screen();
        else if (screen_mode == 2)  draw_energy_screen();
    }

    // Touch
    uint16_t tx, ty;
    if (tft.getTouch(&tx, &ty, 300)) {
        if (touch_down_time == 0) touch_down_time = now;
        // Debounce
    } else {
        if (touch_down_time > 0) {
            handle_touch(tx, ty);
            touch_down_time = 0;
        }
    }

    // Serial debug output (every 2s)
    static uint32_t last_serial = 0;
    if (now - last_serial > 2000) {
        last_serial = now;
        Serial.printf("Fwd=%.1fW Ref=%.1fW SWR=%.2f Temp=%.1fC Duty=%.0f%%\n",
                      fwd_power_w, ref_power_w, swr, temp_c, duty_pct);
    }
}
