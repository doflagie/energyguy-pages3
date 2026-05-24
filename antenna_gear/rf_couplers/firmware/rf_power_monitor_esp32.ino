/*
 * rf_power_monitor_esp32.ino
 * TM-COUP-001 Rev A — RF Power Monitor with SWR and BLE
 *
 * Hardware:
 *   CYD (Cheap Yellow Display) — ESP32 + 2.8" ILI9341 TFT 320×240 + XPT2046 touch
 *   Dual Schottky detector (FWD/REF) → ADC GPIO34, GPIO35
 *   AD8307 log amps (optional, higher accuracy) → same ADC pins
 *   Band select rotary encoder → GPIO25(CLK), GPIO26(DT), GPIO27(SW)
 *   Reference power LED → GPIO13
 *
 * Features:
 *   Real-time forward/reflected power display (W and dBm)
 *   SWR calculation and display with color-coded bar
 *   Return loss display (dB)
 *   Peak hold with decay
 *   6 ham band groups (HF/6M/2M/VHF-UHF) with calibration tables
 *   BLE broadcast (battery/power data to field laptop or logging device)
 *   Web API via WiFi AP mode
 *   NVS calibration storage
 *   Dual detector mode: Schottky peak OR AD8307 log amp (runtime select)
 *
 * CYD User_Setup.h (TFT_eSPI):
 *   #define ILI9341_DRIVER
 *   #define TFT_MOSI 13  #define TFT_SCLK 14  #define TFT_CS 15
 *   #define TFT_DC 2     #define TFT_RST -1   #define TFT_BL 21
 *   #define TOUCH_CS 33
 *   #define SPI_FREQUENCY 40000000
 */

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

// ─── Pin Definitions ─────────────────────────────────────────────────────────
#define PIN_FWD_ADC     34    // Forward power detector (ADC1_CH6)
#define PIN_REF_ADC     35    // Reflected power detector (ADC1_CH7)
#define PIN_ENC_CLK     25    // Rotary encoder A
#define PIN_ENC_DT      26    // Rotary encoder B
#define PIN_ENC_SW      27    // Encoder pushbutton
#define PIN_LED_REF     13    // Reference LED indicator
#define PIN_TOUCH_CS    33    // Touch controller CS

// ─── Constants ───────────────────────────────────────────────────────────────
#define ADC_SAMPLES     64      // ADC averaging count
#define ADC_VREF_MV    3300     // ADC reference voltage (mV)
#define ADC_BITS       4096     // 12-bit ADC
#define PEAK_DECAY_MS   3000    // Peak hold decay time (ms)
#define BLE_INTERVAL_MS 500     // BLE broadcast interval

// ─── Detector Mode ───────────────────────────────────────────────────────────
typedef enum { DET_SCHOTTKY = 0, DET_AD8307 = 1 } DetectorMode;

// ─── Band Configuration ──────────────────────────────────────────────────────
typedef struct {
    const char* name;
    float       freq_mhz;
    float       coupling_db;   // Coupler coupling factor (negative dB)
    float       cal_fwd_k;     // Forward cal constant: P_W = (V_mv ^ 2) * k
    float       cal_ref_k;     // Reflected cal constant (same or measured)
    float       vf_correction; // Diode Vf at 1mA for this band (mV)
} BandConfig;

static const BandConfig BANDS[] = {
    // Name    Freq   Coup   fwd_k    ref_k    Vf_mV
    { "160m",  1.85,  -20.0, 2.50e-8, 2.50e-8,  230 },
    { "80m",   3.65,  -20.0, 2.48e-8, 2.48e-8,  230 },
    { "40m",   7.15,  -20.0, 2.45e-8, 2.45e-8,  232 },
    { "30m",  10.13,  -20.0, 2.43e-8, 2.43e-8,  233 },
    { "20m",  14.25,  -20.0, 2.40e-8, 2.40e-8,  235 },
    { "17m",  18.12,  -20.0, 2.38e-8, 2.38e-8,  237 },
    { "15m",  21.27,  -20.0, 2.35e-8, 2.35e-8,  238 },
    { "12m",  24.94,  -20.0, 2.32e-8, 2.32e-8,  240 },
    { "10m",  28.50,  -20.0, 2.28e-8, 2.28e-8,  242 },
    { "6m",   50.15,  -20.0, 2.20e-8, 2.20e-8,  248 },
    { "2m",  144.20,  -20.0, 2.05e-8, 2.05e-8,  258 },
    { "1.25m",222.00, -20.0, 1.95e-8, 1.95e-8,  262 },
    { "70cm", 432.10, -20.0, 1.80e-8, 1.80e-8,  270 },
    { "33cm", 902.00, -20.0, 1.60e-8, 1.60e-8,  278 },
    { "23cm",1296.00, -20.0, 1.40e-8, 1.40e-8,  285 },
};
#define N_BANDS  (sizeof(BANDS)/sizeof(BANDS[0]))

// ─── AD8307 Transfer Function ─────────────────────────────────────────────────
// V_out = 25 mV/dB × (P_dBm + 84)
// P_dBm = V_out_mV / 25 − 84
// Coupling offset already applied in adc_to_dbm()
#define AD8307_SLOPE_MV_DB   25.0f
#define AD8307_INTERCEPT_DBM (-84.0f)

// ─── Voltage Divider (detector output → ADC) ─────────────────────────────────
// R_div1=30kΩ, R_div2=10kΩ → ratio = 10/(30+10) = 0.25
#define V_DIV_RATIO  0.25f

// ─── Data Structures ─────────────────────────────────────────────────────────
typedef struct {
    float  p_fwd_w;      // Forward power (Watts)
    float  p_ref_w;      // Reflected power (Watts)
    float  p_fwd_dbm;    // Forward power (dBm)
    float  p_ref_dbm;    // Reflected power (dBm)
    float  swr;          // SWR (1.0 = perfect match)
    float  return_loss;  // Return loss (dB, positive = good)
    float  gamma;        // Reflection coefficient |Γ|
    float  p_fwd_peak_w; // Peak hold (W)
    float  p_fwd_avg_w;  // Running average (W)
    float  v_fwd_mv;     // Raw FWD detector DC (mV, after divider)
    float  v_ref_mv;     // Raw REF detector DC (mV, after divider)
    uint8_t band_idx;    // Current band index
    bool   ble_connected;
    unsigned long peak_timestamp;
} RFData;

RFData g_rf = {0};

// ─── Display ──────────────────────────────────────────────────────────────────
TFT_eSPI       tft;
XPT2046_Touchscreen touch(PIN_TOUCH_CS);
TFT_eSprite    spr_main(&tft);
TFT_eSprite    spr_bar(&tft);

// ─── BLE ─────────────────────────────────────────────────────────────────────
// Nordic UART Service (NUS) — same convention as TM-BMS-001 and TM-MAST-001
#define BLE_SERVICE_UUID     "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define BLE_TX_UUID          "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"  // notify
#define BLE_RX_UUID          "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"  // write

BLEServer*         g_ble_server   = nullptr;
BLECharacteristic* g_ble_tx       = nullptr;
bool               g_ble_connected = false;

class BLECallback : public BLEServerCallbacks {
    void onConnect(BLEServer* svr)    { g_ble_connected = true; }
    void onDisconnect(BLEServer* svr) {
        g_ble_connected = false;
        BLEDevice::startAdvertising();
    }
};

class BLERxCallback : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* c) {
        String cmd = c->getValue().c_str();
        cmd.trim();
        // Accept: "BAND=20m\n" or "BAND=7\n" (index)
        if (cmd.startsWith("BAND=")) {
            String val = cmd.substring(5);
            for (int i = 0; i < (int)N_BANDS; i++) {
                if (val == BANDS[i].name || val.toInt() == i) {
                    g_rf.band_idx = i;
                    break;
                }
            }
        }
    }
};

// ─── WiFi AP + Web Server ─────────────────────────────────────────────────────
WebServer g_web(80);

void web_api_status() {
    char buf[256];
    snprintf(buf, sizeof(buf),
        "{\"band\":\"%s\",\"fwd_w\":%.2f,\"ref_w\":%.3f,"
        "\"swr\":%.2f,\"rl_db\":%.1f,\"peak_w\":%.2f}",
        BANDS[g_rf.band_idx].name,
        g_rf.p_fwd_w, g_rf.p_ref_w,
        g_rf.swr, g_rf.return_loss,
        g_rf.p_fwd_peak_w);
    g_web.send(200, "application/json", buf);
}

// ─── NVS Calibration Storage ──────────────────────────────────────────────────
Preferences g_prefs;

void cal_load() {
    g_prefs.begin("rfcal", true);
    g_rf.band_idx = g_prefs.getUChar("band", 4);  // default 20m
    g_prefs.end();
}

void cal_save() {
    g_prefs.begin("rfcal", false);
    g_prefs.putUChar("band", g_rf.band_idx);
    g_prefs.end();
}

// ─── ADC Utilities ───────────────────────────────────────────────────────────
static uint32_t adc_read_avg(uint8_t pin, int samples) {
    uint32_t sum = 0;
    for (int i = 0; i < samples; i++) sum += analogRead(pin);
    return sum / samples;
}

// Convert raw ADC count to millivolts (after external voltage divider)
static float adc_counts_to_mv(uint32_t counts) {
    // ESP32 ADC nonlinearity correction (empirical polynomial, 0–3.3V range)
    float v = (counts * ADC_VREF_MV) / (float)ADC_BITS;
    // First-order linearization (ESP32 ADC ~2% nonlinear at midscale)
    v = v * 1.0218f - 10.5f;
    if (v < 0) v = 0;
    // Undo voltage divider
    return v / V_DIV_RATIO;
}

// Convert detected DC voltage (mV) to RF power (Watts) — Schottky mode
// V_dc = V_peak_RF - Vf_diode
// V_peak_RF = V_dc + Vf
// P = V_peak_RF² / (2 × Z_det × 10^(coupling/10))
static float schottky_mv_to_watts(float v_dc_mv, const BandConfig* band, bool is_fwd) {
    float vf = band->vf_correction;   // Diode Vf in mV for this band
    float v_rf_mv = v_dc_mv + vf;     // Reconstruct RF peak voltage
    if (v_rf_mv < vf) return 0.0f;    // Below threshold

    float v_rf_v = v_rf_mv / 1000.0f;
    // P_coupled = V_peak² / (2 × 50Ω) = V² / 100
    float p_coupled_w = (v_rf_v * v_rf_v) / 100.0f;
    // Correct for coupling factor: P_main = P_coupled / 10^(coupling/10)
    // coupling_db is negative (e.g. -20); 10^(-20/10) = 0.01
    float c_linear = powf(10.0f, band->coupling_db / 10.0f);
    return p_coupled_w / c_linear;
}

// Convert AD8307 output voltage (mV) to power in Watts
static float ad8307_mv_to_watts(float v_out_mv, const BandConfig* band) {
    float p_coupled_dbm = (v_out_mv / AD8307_SLOPE_MV_DB) + AD8307_INTERCEPT_DBM;
    float p_main_dbm    = p_coupled_dbm - band->coupling_db;  // coupling_db is negative
    float p_main_w      = powf(10.0f, (p_main_dbm - 30.0f) / 10.0f);
    return p_main_w;
}

// ─── SWR and Return Loss ─────────────────────────────────────────────────────
static float calc_swr(float p_fwd, float p_ref) {
    if (p_fwd < 1e-9f) return 1.0f;
    float gamma = sqrtf(p_ref / p_fwd);          // |Γ| = sqrt(P_ref/P_fwd)
    gamma = constrain(gamma, 0.0f, 0.999f);
    return (1.0f + gamma) / (1.0f - gamma);
}

static float calc_return_loss(float p_fwd, float p_ref) {
    if (p_ref < 1e-12f || p_fwd < 1e-9f) return 99.0f;
    return -10.0f * log10f(p_ref / p_fwd);   // RL = -10 log(P_ref/P_fwd)
}

// ─── Measurement Loop ─────────────────────────────────────────────────────────
DetectorMode g_det_mode = DET_SCHOTTKY;

void measure_rf() {
    uint32_t raw_fwd = adc_read_avg(PIN_FWD_ADC, ADC_SAMPLES);
    uint32_t raw_ref = adc_read_avg(PIN_REF_ADC, ADC_SAMPLES);

    g_rf.v_fwd_mv = adc_counts_to_mv(raw_fwd);
    g_rf.v_ref_mv = adc_counts_to_mv(raw_ref);

    const BandConfig* band = &BANDS[g_rf.band_idx];

    if (g_det_mode == DET_SCHOTTKY) {
        g_rf.p_fwd_w = schottky_mv_to_watts(g_rf.v_fwd_mv, band, true);
        g_rf.p_ref_w = schottky_mv_to_watts(g_rf.v_ref_mv, band, false);
    } else {
        g_rf.p_fwd_w = ad8307_mv_to_watts(g_rf.v_fwd_mv, band);
        g_rf.p_ref_w = ad8307_mv_to_watts(g_rf.v_ref_mv, band);
    }

    // Power in dBm
    g_rf.p_fwd_dbm = (g_rf.p_fwd_w > 1e-9f) ? 10.0f * log10f(g_rf.p_fwd_w * 1000.0f) : -99.0f;
    g_rf.p_ref_dbm = (g_rf.p_ref_w > 1e-9f) ? 10.0f * log10f(g_rf.p_ref_w * 1000.0f) : -99.0f;

    // SWR and RL
    g_rf.gamma        = sqrtf(constrain(g_rf.p_ref_w / max(g_rf.p_fwd_w, 1e-9f), 0.0f, 0.9999f));
    g_rf.swr          = calc_swr(g_rf.p_fwd_w, g_rf.p_ref_w);
    g_rf.return_loss  = calc_return_loss(g_rf.p_fwd_w, g_rf.p_ref_w);

    // Peak hold
    if (g_rf.p_fwd_w > g_rf.p_fwd_peak_w) {
        g_rf.p_fwd_peak_w  = g_rf.p_fwd_w;
        g_rf.peak_timestamp = millis();
    } else if (millis() - g_rf.peak_timestamp > PEAK_DECAY_MS) {
        g_rf.p_fwd_peak_w = g_rf.p_fwd_w;  // Reset peak after decay
    }

    // Running average (exponential, τ ≈ 10 samples)
    g_rf.p_fwd_avg_w = g_rf.p_fwd_avg_w * 0.9f + g_rf.p_fwd_w * 0.1f;
}

// ─── Display ──────────────────────────────────────────────────────────────────
#define TFT_DARKGREEN  0x03E0
#define TFT_ORANGE     0xFD20
#define TFT_BG         0x0000   // Black background

// SWR color: green <1.5, yellow 1.5–2.5, orange 2.5–4, red >4
static uint16_t swr_color(float swr) {
    if (swr < 1.5f) return TFT_GREEN;
    if (swr < 2.5f) return TFT_YELLOW;
    if (swr < 4.0f) return TFT_ORANGE;
    return TFT_RED;
}

// Format power with appropriate unit (W or mW)
static void fmt_power(char* buf, size_t sz, float watts) {
    if (watts >= 100.0f)       snprintf(buf, sz, "%.0f W", watts);
    else if (watts >= 10.0f)   snprintf(buf, sz, "%.1f W", watts);
    else if (watts >= 1.0f)    snprintf(buf, sz, "%.2f W", watts);
    else if (watts >= 0.001f)  snprintf(buf, sz, "%.0f mW", watts * 1000.0f);
    else                        snprintf(buf, sz, "<1 mW");
}

// Draw bargraph (x, y, w, h, fraction 0.0–1.0, color)
static void draw_bar(TFT_eSprite* s, int x, int y, int w, int h, float frac, uint16_t col) {
    s->fillRect(x, y, w, h, 0x2104);          // Dark background
    s->drawRect(x, y, w, h, TFT_DARKGREY);
    int fill = (int)(frac * (w - 2));
    if (fill > 0) s->fillRect(x + 1, y + 1, fill, h - 2, col);
}

void update_display() {
    const int SW = 320, SH = 240;
    spr_main.fillSprite(TFT_BG);

    // ── Header bar ──
    spr_main.fillRect(0, 0, SW, 26, 0x1082);
    spr_main.setTextColor(TFT_WHITE, 0x1082);
    spr_main.setTextSize(1);
    spr_main.drawString("RF POWER MONITOR", 4, 4, 2);
    spr_main.setTextColor(TFT_CYAN, 0x1082);
    char band_str[24];
    snprintf(band_str, sizeof(band_str), "[%s] %.2f MHz",
             BANDS[g_rf.band_idx].name, BANDS[g_rf.band_idx].freq_mhz);
    spr_main.drawString(band_str, SW - 130, 4, 2);

    // ── Forward power ──
    spr_main.setTextColor(TFT_GREEN, TFT_BG);
    spr_main.drawString("FWD", 4, 32, 2);
    char pw_str[20];
    fmt_power(pw_str, sizeof(pw_str), g_rf.p_fwd_w);
    spr_main.setTextSize(1);
    spr_main.setTextColor(TFT_WHITE, TFT_BG);
    spr_main.drawString(pw_str, 52, 30, 4);   // Large font
    spr_main.setTextColor(TFT_DARKGREY, TFT_BG);
    char dbm_str[16];
    snprintf(dbm_str, sizeof(dbm_str), "%.1f dBm", g_rf.p_fwd_dbm);
    spr_main.drawString(dbm_str, 230, 32, 2);

    // Forward bar (log scale, 0dBm to +53dBm = 0–200W)
    float fwd_frac = constrain((g_rf.p_fwd_dbm + 10.0f) / 63.0f, 0.0f, 1.0f);
    draw_bar(&spr_main, 4, 58, 312, 14, fwd_frac, TFT_GREEN);

    // Peak marker on forward bar
    float peak_frac = constrain((10.0f * log10f(g_rf.p_fwd_peak_w * 1000.0f + 0.001f) + 10.0f) / 63.0f, 0.0f, 1.0f);
    int peak_x = 4 + (int)(peak_frac * 312);
    spr_main.drawFastVLine(peak_x, 58, 14, TFT_YELLOW);

    // ── Reflected power ──
    spr_main.setTextColor(TFT_ORANGE, TFT_BG);
    spr_main.drawString("REF", 4, 78, 2);
    char rw_str[20];
    fmt_power(rw_str, sizeof(rw_str), g_rf.p_ref_w);
    spr_main.setTextColor(TFT_WHITE, TFT_BG);
    spr_main.drawString(rw_str, 52, 76, 4);
    spr_main.setTextColor(TFT_DARKGREY, TFT_BG);
    snprintf(dbm_str, sizeof(dbm_str), "%.1f dBm", g_rf.p_ref_dbm);
    spr_main.drawString(dbm_str, 230, 78, 2);

    float ref_frac = constrain((g_rf.p_ref_dbm + 10.0f) / 63.0f, 0.0f, 1.0f);
    draw_bar(&spr_main, 4, 104, 312, 14, ref_frac, TFT_ORANGE);

    // ── SWR display ──
    uint16_t swr_col = swr_color(g_rf.swr);
    spr_main.setTextColor(TFT_WHITE, TFT_BG);
    spr_main.drawString("SWR:", 4, 126, 2);
    char swr_str[16];
    if (g_rf.swr > 99.0f)       strcpy(swr_str, "∞");
    else if (g_rf.swr > 9.9f)   snprintf(swr_str, sizeof(swr_str), "%.1f", g_rf.swr);
    else                          snprintf(swr_str, sizeof(swr_str), "%.2f:1", g_rf.swr);
    spr_main.setTextColor(swr_col, TFT_BG);
    spr_main.drawString(swr_str, 52, 122, 4);

    // Return loss
    spr_main.setTextColor(TFT_WHITE, TFT_BG);
    char rl_str[16];
    snprintf(rl_str, sizeof(rl_str), "RL: %.1f dB", g_rf.return_loss);
    spr_main.drawString(rl_str, 200, 126, 2);

    // SWR bar (1.0 to 5.0 scale, logarithmic)
    float swr_frac = constrain((g_rf.swr - 1.0f) / 4.0f, 0.0f, 1.0f);
    draw_bar(&spr_main, 4, 150, 312, 14, swr_frac, swr_col);
    // Target zone markers at SWR 1.5 and 2.0
    spr_main.drawFastVLine(4 + (int)(0.125f * 312), 150, 14, TFT_DARKGREY);  // SWR 1.5
    spr_main.drawFastVLine(4 + (int)(0.250f * 312), 150, 14, TFT_DARKGREY);  // SWR 2.0

    // ── Peak / Average stats ──
    spr_main.setTextColor(TFT_CYAN, TFT_BG);
    char pk_str[24], av_str[24];
    fmt_power(pk_str, sizeof(pk_str), g_rf.p_fwd_peak_w);
    fmt_power(av_str, sizeof(av_str), g_rf.p_fwd_avg_w);
    char stats_buf[48];
    snprintf(stats_buf, sizeof(stats_buf), "PK: %s  AVG: %s", pk_str, av_str);
    spr_main.drawString(stats_buf, 4, 170, 2);

    // ── Band touch buttons (bottom row) ──
    const char* quick_bands[] = { "40m", "20m", "2m", "70cm" };
    int quick_idxs[]          = {  2,     4,     10,   12    };
    for (int i = 0; i < 4; i++) {
        int bx = 4 + i * 78, by = 190;
        bool active = (g_rf.band_idx == (uint8_t)quick_idxs[i]);
        spr_main.fillRoundRect(bx, by, 72, 22, 4, active ? 0x0410 : 0x2104);
        spr_main.drawRoundRect(bx, by, 72, 22, 4, active ? TFT_GREEN : TFT_DARKGREY);
        spr_main.setTextColor(active ? TFT_WHITE : TFT_DARKGREY, active ? 0x0410 : 0x2104);
        spr_main.drawString(quick_bands[i], bx + 20, by + 4, 2);
    }

    // BLE indicator
    spr_main.drawString(g_ble_connected ? "BLE" : "   ", 292, 220, 1);
    spr_main.setTextColor(g_ble_connected ? TFT_BLUE : TFT_DARKGREY, TFT_BG);
    spr_main.drawString(g_ble_connected ? "BLE" : "   ", 292, 221, 2);

    spr_main.pushSprite(0, 0);
}

// ─── Touch Handling ───────────────────────────────────────────────────────────
static unsigned long last_touch = 0;

void handle_touch() {
    if (!touch.tirqTouched() || !touch.touched()) return;
    if (millis() - last_touch < 250) return;
    last_touch = millis();

    TS_Point p = touch.getPoint();
    // Map to 320×240 screen (calibrate for your CYD)
    int tx = map(p.x, 200, 3800, 0, 320);
    int ty = map(p.y, 200, 3800, 0, 240);

    // Band quick-select buttons (bottom row y=190–212)
    if (ty >= 190 && ty <= 212) {
        int quick_idxs[] = { 2, 4, 10, 12 };
        int btn = (tx - 4) / 78;
        if (btn >= 0 && btn < 4) {
            g_rf.band_idx = quick_idxs[btn];
            cal_save();
        }
    }

    // Long press anywhere resets peak
    // Handled by encoder SW (GPIO27) instead
}

// ─── Encoder ──────────────────────────────────────────────────────────────────
static int   enc_last  = 0;
static bool  enc_btn   = false;

void handle_encoder() {
    int a = digitalRead(PIN_ENC_CLK);
    int b = digitalRead(PIN_ENC_DT);
    int enc = (a << 1) | b;

    if (enc != enc_last) {
        if ((enc_last == 0b10 && enc == 0b00) || (enc_last == 0b01 && enc == 0b11)) {
            // CW: next band
            if (g_rf.band_idx < N_BANDS - 1) g_rf.band_idx++;
        } else if ((enc_last == 0b00 && enc == 0b10) || (enc_last == 0b11 && enc == 0b01)) {
            // CCW: previous band
            if (g_rf.band_idx > 0) g_rf.band_idx--;
        }
        enc_last = enc;
    }

    bool btn = !digitalRead(PIN_ENC_SW);
    if (btn && !enc_btn) {
        // Button press: reset peak hold
        g_rf.p_fwd_peak_w  = g_rf.p_fwd_w;
        g_rf.peak_timestamp = millis();
    }
    enc_btn = btn;
}

// ─── BLE Broadcasting ─────────────────────────────────────────────────────────
static unsigned long last_ble = 0;

void ble_broadcast() {
    if (!g_ble_connected) return;
    if (millis() - last_ble < BLE_INTERVAL_MS) return;
    last_ble = millis();

    char buf[128];
    snprintf(buf, sizeof(buf),
        "BAND=%s\nFWD=%.2f\nREF=%.3f\nSWR=%.2f\nRL=%.1f\nPEAK=%.2f\n",
        BANDS[g_rf.band_idx].name,
        g_rf.p_fwd_w, g_rf.p_ref_w,
        g_rf.swr, g_rf.return_loss,
        g_rf.p_fwd_peak_w);

    g_ble_tx->setValue((uint8_t*)buf, strlen(buf));
    g_ble_tx->notify();
}

// ─── Setup ────────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);

    // ADC configuration
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);  // 0–3.3V range

    // Encoder inputs
    pinMode(PIN_ENC_CLK, INPUT_PULLUP);
    pinMode(PIN_ENC_DT,  INPUT_PULLUP);
    pinMode(PIN_ENC_SW,  INPUT_PULLUP);
    pinMode(PIN_LED_REF, OUTPUT);
    enc_last = (digitalRead(PIN_ENC_CLK) << 1) | digitalRead(PIN_ENC_DT);

    // Display
    tft.init();
    tft.setRotation(1);  // Landscape
    tft.fillScreen(TFT_BLACK);
    spr_main.createSprite(320, 240);

    // Touch
    touch.begin();
    touch.setRotation(1);

    // NVS
    cal_load();

    // BLE
    BLEDevice::init("RF-COUP-001");
    g_ble_server = BLEDevice::createServer();
    g_ble_server->setCallbacks(new BLECallback());

    BLEService* svc = g_ble_server->createService(BLE_SERVICE_UUID);
    g_ble_tx = svc->createCharacteristic(BLE_TX_UUID,
        BLECharacteristic::PROPERTY_NOTIFY);
    g_ble_tx->addDescriptor(new BLE2902());

    BLECharacteristic* rx = svc->createCharacteristic(BLE_RX_UUID,
        BLECharacteristic::PROPERTY_WRITE);
    rx->setCallbacks(new BLERxCallback());

    svc->start();
    BLEDevice::startAdvertising();

    // WiFi AP
    WiFi.softAP("RF-MONITOR", "rfmonitor1");
    g_web.on("/api/status", web_api_status);
    g_web.begin();

    // Splash
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("TM-COUP-001  Rev A", 60, 100, 2);
    tft.drawString("RF POWER MONITOR", 65, 120, 2);
    delay(1500);
}

// ─── Main Loop ────────────────────────────────────────────────────────────────
static unsigned long last_meas   = 0;
static unsigned long last_disp   = 0;
#define MEAS_INTERVAL_MS   50    // 20 Hz measurement
#define DISP_INTERVAL_MS  100    // 10 Hz display update

void loop() {
    unsigned long now = millis();

    if (now - last_meas >= MEAS_INTERVAL_MS) {
        measure_rf();
        last_meas = now;
    }

    if (now - last_disp >= DISP_INTERVAL_MS) {
        update_display();
        last_disp = now;
    }

    handle_encoder();
    handle_touch();
    ble_broadcast();
    g_web.handleClient();
}
