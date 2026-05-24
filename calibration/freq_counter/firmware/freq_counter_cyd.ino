/*
 * freq_counter_cyd.ino
 * Portable Frequency Counter — CYD (ESP32-2432S028) Main Firmware
 * TM-FC-001 Rev A
 *
 * Hardware:
 *   Display:    ILI9341 2.8" TFT, 320×240, SPI (HSPI)
 *   Touch:      XPT2046, SPI (HSPI shared)
 *   Input:      GPIO34 (direct count / reciprocal, via 74LVC14A)
 *   Prescaler:  GPIO35 (MB506 ÷8/÷64 output)
 *   Prescaler mode select: GPIO0 (MC pin to MB506, HIGH=÷64, LOW=÷8)
 *   GPS:        UART2 GPIO16/17 (NEO-6M, 9600 baud)
 *   GPSDO:      GPIO26 (1 PPS), GPIO39 (10 MHz ref PCNT), GPIO25 (DAC → TCXO VC)
 *   Gate LED:   GPIO27
 *   WiFi/BT:    On-chip (ESP32)
 *   EEPROM:     Emulated (NVS via Preferences)
 *
 * Measurement modes:
 *   DIRECT     — PCNT gate; resolution = 1/gate_time; up to 40 MHz
 *   PRESCALER  — PCNT via MB506 ÷8 or ÷64; up to 3 GHz
 *   RECIPROCAL — esp_timer + PCNT; resolution = f²/(N × f_ref); HF/LF
 *   PERIOD     — same as reciprocal, displayed as period (µs/ms/s)
 *
 * Screens:
 *   SCREEN_MAIN      — large frequency display
 *   SCREEN_PERIOD    — period measurement display
 *   SCREEN_SETTINGS  — gate time, mode, N, attenuator
 *   SCREEN_GPSDO     — GPSDO status, lock, error, statistics
 *   SCREEN_NETWORK   — WiFi IP, BLE status, live stream toggle
 *   SCREEN_STATS     — min/max/mean/stddev over session
 */

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <driver/pcnt.h>
#include <driver/dac.h>
#include <esp_timer.h>
#include <Preferences.h>
#include <WiFi.h>
#include <WiFiServer.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "gpsdo.h"
#include "reciprocal_counter.h"

// ── CYD Pin Definitions ───────────────────────────────────────────────────────
#define TFT_BL            21
#define GATE_LED_PIN      27
#define PRESCALER_MC_PIN   0    // MB506 modulus control (HIGH=÷64, LOW=÷8)
#define DIRECT_INPUT_PIN  34
#define PRESCALER_IN_PIN  35

// ── Measurement Constants ─────────────────────────────────────────────────────
#define PCNT_DIRECT_UNIT   PCNT_UNIT_0
#define PCNT_H_LIMIT       30000
#define GATE_100MS         100000UL     // µs
#define GATE_1S            1000000UL
#define GATE_10S           10000000UL
#define GATE_100S          100000000UL
#define WIFI_SSID          "FreqCounter"
#define WIFI_PASS          "10MHz0001"
#define BLE_SERVICE_UUID   "0000FFF0-0000-1000-8000-00805F9B34FB"
#define BLE_CHAR_UUID      "0000FFF1-0000-1000-8000-00805F9B34FB"

// ── Enumerations ─────────────────────────────────────────────────────────────
typedef enum { MODE_DIRECT=0, MODE_PRESCALER, MODE_RECIPROCAL, MODE_PERIOD } MeasMode;
typedef enum {
    SCREEN_MAIN=0, SCREEN_PERIOD, SCREEN_SETTINGS,
    SCREEN_GPSDO, SCREEN_NETWORK, SCREEN_STATS, SCREEN_COUNT
} Screen;
typedef enum { GATE_100MS_IDX=0, GATE_1S_IDX, GATE_10S_IDX, GATE_100S_IDX } GateIdx;
typedef enum { ATT_0DB=0, ATT_10DB, ATT_20DB, ATT_30DB } AttIdx;
typedef enum { PRESCALE_1=1, PRESCALE_8=8, PRESCALE_64=64 } PrescaleFactor;

// ── Application State ─────────────────────────────────────────────────────────
struct AppState {
    // Measurement
    MeasMode    mode;
    GateIdx     gate_idx;
    uint32_t    gate_us;          // active gate time in µs
    PrescaleFactor prescale;
    AttIdx      att_idx;
    uint32_t    recip_n;          // N cycles for reciprocal mode
    bool        auto_n;           // auto-select N based on measured freq

    // Results
    double      frequency_hz;
    double      period_ns;
    char        freq_str[32];
    char        period_str[32];
    char        resolution_str[20];
    double      accuracy_ppb;
    bool        result_valid;
    bool        measuring;

    // PCNT direct count
    volatile int64_t pcnt_accum;
    volatile bool    gate_open;
    int64_t          gate_start_us;

    // Statistics
    double  stat_min;
    double  stat_max;
    double  stat_sum;
    double  stat_sum_sq;
    int32_t stat_count;

    // Display
    Screen  screen;
    Screen  prev_screen;
    bool    needs_redraw;
    bool    needs_update;

    // WiFi / BLE
    bool    wifi_ap_active;
    bool    ble_active;
    bool    stream_enabled;
    char    ip_str[20];

    // Hold
    bool    hold;
};

static AppState app = {};
static TFT_eSPI tft = TFT_eSPI();
static Preferences prefs;

// WiFi / BLE
static WiFiServer tcp_server(7373);
static WiFiClient tcp_client;
static BLEServer*          ble_server   = nullptr;
static BLECharacteristic*  ble_char     = nullptr;
static bool                ble_connected = false;

// ── PCNT Direct-Count ISR ─────────────────────────────────────────────────────
static void IRAM_ATTR direct_pcnt_isr(void* arg) {
    if (app.gate_open) {
        app.pcnt_accum += PCNT_H_LIMIT;
    }
}

// ── Gate Timer (esp_timer callback) ──────────────────────────────────────────
static void gate_timer_cb(void* arg) {
    if (!app.gate_open) return;
    app.gate_open = false;
    digitalWrite(GATE_LED_PIN, LOW);
}

static esp_timer_handle_t gate_timer = nullptr;

// ── Color Palette ─────────────────────────────────────────────────────────────
#define C_BG        TFT_BLACK
#define C_FG        TFT_WHITE
#define C_ACCENT    0x07FF    // cyan
#define C_GREEN     TFT_GREEN
#define C_YELLOW    TFT_YELLOW
#define C_RED       TFT_RED
#define C_ORANGE    0xFD20
#define C_BLUE      0x001F
#define C_DGRAY     0x39E7
#define C_LGRAY     0xC618
#define C_LOCKED    C_GREEN
#define C_LOCKING   C_YELLOW
#define C_HOLDOVER  C_ORANGE
#define C_NOGPS     C_LGRAY

// ── BLE callbacks ─────────────────────────────────────────────────────────────
class BLEServerCB : public BLEServerCallbacks {
    void onConnect(BLEServer*) override    { ble_connected = true;  }
    void onDisconnect(BLEServer*) override { ble_connected = false;
        BLEDevice::startAdvertising(); }
};

// ── PCNT direct-count setup ───────────────────────────────────────────────────
void direct_pcnt_init(uint8_t gpio) {
    pcnt_config_t cfg = {};
    cfg.pulse_gpio_num = gpio;
    cfg.ctrl_gpio_num  = PCNT_PIN_NOT_USED;
    cfg.lctrl_mode     = PCNT_MODE_KEEP;
    cfg.hctrl_mode     = PCNT_MODE_KEEP;
    cfg.pos_mode       = PCNT_COUNT_INC;
    cfg.neg_mode       = PCNT_COUNT_DIS;
    cfg.counter_h_lim  = PCNT_H_LIMIT;
    cfg.counter_l_lim  = -1;
    cfg.unit           = PCNT_DIRECT_UNIT;
    cfg.channel        = PCNT_CHANNEL_0;
    pcnt_unit_config(&cfg);
    pcnt_filter_disable(PCNT_DIRECT_UNIT);
    pcnt_event_enable(PCNT_DIRECT_UNIT, PCNT_EVT_H_LIM);
    pcnt_isr_register(direct_pcnt_isr, NULL, 0, NULL);
    pcnt_intr_enable(PCNT_DIRECT_UNIT);
    pcnt_counter_pause(PCNT_DIRECT_UNIT);
    pcnt_counter_clear(PCNT_DIRECT_UNIT);
}

// ── Start a direct-count measurement ─────────────────────────────────────────
void direct_count_start() {
    app.pcnt_accum = 0;
    pcnt_counter_pause(PCNT_DIRECT_UNIT);
    pcnt_counter_clear(PCNT_DIRECT_UNIT);

    app.gate_open     = true;
    app.measuring     = true;
    app.gate_start_us = esp_timer_get_time();

    digitalWrite(GATE_LED_PIN, HIGH);
    pcnt_counter_resume(PCNT_DIRECT_UNIT);

    esp_timer_start_once(gate_timer, app.gate_us);
}

// ── Finish direct-count measurement ──────────────────────────────────────────
// Call from main loop when gate_open goes false.
void direct_count_finish() {
    int16_t remainder = 0;
    pcnt_get_counter_value(PCNT_DIRECT_UNIT, &remainder);
    int64_t total = app.pcnt_accum + (int64_t)remainder;

    double elapsed_s = (double)app.gate_us * 1e-6 * gpsdo_correction();
    app.frequency_hz = (elapsed_s > 0.0) ? ((double)total / elapsed_s) : 0.0;
    app.frequency_hz *= (double)app.prescale;

    app.result_valid = (total > 0);
    app.measuring    = false;
}

// ── Reciprocal measurement wrapper ───────────────────────────────────────────
void reciprocal_start_meas() {
    if (app.auto_n) {
        uint32_t n = reciprocal_auto_n(app.frequency_hz > 0.0 ? app.frequency_hz : 1000.0);
        reciprocal_set_n(n);
        app.recip_n = n;
    } else {
        reciprocal_set_n(app.recip_n);
    }
    reciprocal_set_mode((app.mode == MODE_PERIOD) ? RC_MODE_PERIOD : RC_MODE_RECIPROCAL);
    app.measuring = true;
    reciprocal_start(gpsdo_correction());
    digitalWrite(GATE_LED_PIN, HIGH);
}

// ── Update statistics ─────────────────────────────────────────────────────────
void stats_update(double f) {
    if (f <= 0.0) return;
    app.stat_count++;
    app.stat_sum    += f;
    app.stat_sum_sq += f * f;
    if (f < app.stat_min || app.stat_count == 1) app.stat_min = f;
    if (f > app.stat_max || app.stat_count == 1) app.stat_max = f;
}

void stats_reset() {
    app.stat_min = app.stat_max = app.stat_sum = app.stat_sum_sq = 0.0;
    app.stat_count = 0;
}

double stats_mean() {
    return (app.stat_count > 0) ? (app.stat_sum / app.stat_count) : 0.0;
}

double stats_stddev() {
    if (app.stat_count < 2) return 0.0;
    double m = stats_mean();
    double var = (app.stat_sum_sq / app.stat_count) - (m * m);
    return (var > 0.0) ? sqrt(var) : 0.0;
}

// ── Format frequency string ───────────────────────────────────────────────────
void format_frequency(double hz, char* buf, size_t sz) {
    if (hz >= 1e9)       snprintf(buf, sz, "%.9f GHz", hz * 1e-9);
    else if (hz >= 1e6)  snprintf(buf, sz, "%.9f MHz", hz * 1e-6);
    else if (hz >= 1e3)  snprintf(buf, sz, "%.6f kHz", hz * 1e-3);
    else                 snprintf(buf, sz, "%.3f Hz", hz);
}

// ── WiFi AP initialization ────────────────────────────────────────────────────
void wifi_init() {
    WiFi.softAP(WIFI_SSID, WIFI_PASS);
    IPAddress ip = WiFi.softAPIP();
    snprintf(app.ip_str, sizeof(app.ip_str), "%d.%d.%d.%d",
             ip[0], ip[1], ip[2], ip[3]);
    tcp_server.begin();
    app.wifi_ap_active = true;
}

// ── BLE initialization ────────────────────────────────────────────────────────
void ble_init() {
    BLEDevice::init("FreqCounter");
    ble_server = BLEDevice::createServer();
    ble_server->setCallbacks(new BLEServerCB());
    BLEService* svc  = ble_server->createService(BLE_SERVICE_UUID);
    ble_char = svc->createCharacteristic(BLE_CHAR_UUID,
                   BLECharacteristic::PROPERTY_READ |
                   BLECharacteristic::PROPERTY_NOTIFY);
    ble_char->addDescriptor(new BLE2902());
    svc->start();
    BLEAdvertising* adv = BLEDevice::getAdvertising();
    adv->addServiceUUID(BLE_SERVICE_UUID);
    adv->setScanResponse(false);
    BLEDevice::startAdvertising();
    app.ble_active = true;
}

// ── Stream frequency to WiFi TCP client or BLE ───────────────────────────────
void stream_frequency() {
    if (!app.stream_enabled || !app.result_valid) return;
    char line[64];
    snprintf(line, sizeof(line), "%.3f,%s\n", app.frequency_hz, app.freq_str);

    if (app.wifi_ap_active) {
        if (!tcp_client || !tcp_client.connected()) {
            tcp_client = tcp_server.available();
        }
        if (tcp_client && tcp_client.connected()) {
            tcp_client.print(line);
        }
    }
    if (app.ble_active && ble_connected) {
        ble_char->setValue((uint8_t*)line, strlen(line));
        ble_char->notify();
    }
}

// ── EEPROM / Preferences ──────────────────────────────────────────────────────
void settings_load() {
    prefs.begin("fc001", true);
    app.mode      = (MeasMode)prefs.getUChar("mode",    MODE_DIRECT);
    app.gate_idx  = (GateIdx) prefs.getUChar("gate",    GATE_1S_IDX);
    app.att_idx   = (AttIdx)  prefs.getUChar("att",     ATT_0DB);
    app.recip_n   = prefs.getUInt ("recip_n", 1000);
    app.auto_n    = prefs.getBool ("auto_n",  true);
    prefs.end();

    const uint32_t gate_table[] = { GATE_100MS, GATE_1S, GATE_10S, GATE_100S };
    app.gate_us = gate_table[app.gate_idx];
}

void settings_save() {
    prefs.begin("fc001", false);
    prefs.putUChar("mode",   (uint8_t)app.mode);
    prefs.putUChar("gate",   (uint8_t)app.gate_idx);
    prefs.putUChar("att",    (uint8_t)app.att_idx);
    prefs.putUInt ("recip_n", app.recip_n);
    prefs.putBool ("auto_n",  app.auto_n);
    prefs.end();
}

// ── HTTP API (minimal text/plain) ─────────────────────────────────────────────
// Serves GET /freq — returns current frequency as text
void http_serve_client(WiFiClient& client) {
    String req = client.readStringUntil('\r');
    client.flush();
    if (req.indexOf("GET /freq") >= 0) {
        char body[128];
        snprintf(body, sizeof(body),
            "freq_hz=%.3f\nfreq_str=%s\nlocked=%d\naccuracy_ppb=%.1f\n",
            app.frequency_hz, app.freq_str,
            (int)gpsdo.locked, gpsdo_accuracy_ppb());
        client.printf("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n"
                       "Content-Length: %d\r\nConnection: close\r\n\r\n%s",
                       (int)strlen(body), body);
    } else {
        client.print("HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\n");
    }
    client.stop();
}

// ═══════════════════════════════════════════════════════════════════════════════
// DISPLAY FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════════

// ── Draw tab bar (bottom navigation) ─────────────────────────────────────────
void draw_tab_bar() {
    const char* labels[] = { "FREQ", "PERIOD", "SET", "GPS", "NET", "STAT" };
    int tab_w = 320 / SCREEN_COUNT;
    for (int i = 0; i < SCREEN_COUNT; i++) {
        uint16_t bg = (i == app.screen) ? C_ACCENT : C_DGRAY;
        uint16_t fg = (i == app.screen) ? TFT_BLACK : C_FG;
        tft.fillRect(i * tab_w, 220, tab_w - 1, 20, bg);
        tft.setTextColor(fg, bg);
        tft.setTextSize(1);
        tft.setCursor(i * tab_w + (tab_w - strlen(labels[i]) * 6) / 2, 226);
        tft.print(labels[i]);
    }
}

// ── SCREEN_MAIN — large frequency display ────────────────────────────────────
void draw_main_screen() {
    tft.fillRect(0, 0, 320, 218, C_BG);

    // Mode badge
    const char* mode_str[] = { "DIRECT", "PRESCALE", "RECIPROCAL", "PERIOD" };
    const char* gate_str[] = { "100ms", "1s", "10s", "100s" };
    const char* att_str[]  = { "0dB", "-10dB", "-20dB", "-30dB" };
    uint32_t prescale_val  = (uint32_t)app.prescale;

    tft.setTextColor(C_ACCENT, C_BG);
    tft.setTextSize(1);
    tft.setCursor(4, 4);
    tft.print(mode_str[app.mode]);
    if (app.mode == MODE_PRESCALER) {
        tft.printf(" ÷%u", prescale_val);
    }

    // Gate time / N
    tft.setCursor(150, 4);
    tft.setTextColor(C_LGRAY, C_BG);
    if (app.mode == MODE_DIRECT || app.mode == MODE_PRESCALER) {
        tft.printf("GATE: %s", gate_str[app.gate_idx]);
    } else {
        tft.printf("N: %u", app.recip_n);
    }

    // Attenuator
    tft.setCursor(260, 4);
    tft.setTextColor(C_YELLOW, C_BG);
    tft.print(att_str[app.att_idx]);

    // GPSDO status
    tft.setCursor(4, 16);
    uint16_t gps_col = C_NOGPS;
    if      (gpsdo.locked)     gps_col = C_LOCKED;
    else if (gpsdo.holdover)   gps_col = C_HOLDOVER;
    else if (gpsdo.gps_valid)  gps_col = C_LOCKING;
    tft.setTextColor(gps_col, C_BG);
    tft.printf("GPSDO: %s", gpsdo_status_str());

    // Big frequency display
    if (!app.result_valid) {
        tft.setTextColor(C_DGRAY, C_BG);
        tft.setTextSize(2);
        tft.setCursor(60, 80);
        tft.print(app.measuring ? "COUNTING..." : "-- Hz");
    } else {
        // Split freq string for large display
        // Try to fit on two lines: value + unit
        char val[24], unit[12];
        double f = app.frequency_hz;
        if (f >= 1e9) {
            snprintf(val,  sizeof(val),  "%.9f", f * 1e-9);
            strcpy(unit, "GHz");
        } else if (f >= 1e6) {
            snprintf(val,  sizeof(val),  "%.9f", f * 1e-6);
            strcpy(unit, "MHz");
        } else if (f >= 1e3) {
            snprintf(val,  sizeof(val),  "%.6f", f * 1e-3);
            strcpy(unit, "kHz");
        } else {
            snprintf(val,  sizeof(val),  "%.3f", f);
            strcpy(unit, "Hz");
        }

        tft.setTextColor(app.hold ? C_YELLOW : C_FG, C_BG);
        tft.setTextSize(2);
        // Trim leading zeros but keep one digit before decimal
        tft.setCursor(4, 55);
        tft.print(val);
        tft.setTextColor(C_ACCENT, C_BG);
        tft.printf(" %s", unit);

        // Resolution / accuracy line
        tft.setTextSize(1);
        tft.setTextColor(C_LGRAY, C_BG);
        tft.setCursor(4, 100);
        tft.printf("±%s  %.0f ppb", app.resolution_str, app.accuracy_ppb);

        // Period line
        tft.setCursor(4, 114);
        tft.setTextColor(C_LGRAY, C_BG);
        tft.printf("T = %s", app.period_str);
    }

    // HOLD indicator
    if (app.hold) {
        tft.fillRect(265, 55, 50, 18, C_YELLOW);
        tft.setTextColor(TFT_BLACK, C_YELLOW);
        tft.setTextSize(1);
        tft.setCursor(270, 60);
        tft.print("HOLD");
    }

    // Bottom info bar
    tft.drawFastHLine(0, 132, 320, C_DGRAY);
    tft.setTextSize(1);
    tft.setTextColor(C_LGRAY, C_BG);
    tft.setCursor(4, 137);
    if (app.stat_count > 0) {
        char mean_s[20];
        format_frequency(stats_mean(), mean_s, sizeof(mean_s));
        tft.printf("N=%ld  mean=%s", app.stat_count, mean_s);
    } else {
        tft.print("No measurements yet");
    }

    // Touch hints
    tft.setTextColor(C_DGRAY, C_BG);
    tft.setCursor(4, 152);
    tft.print("[HOLD] [RESET STATS] [START]");
}

// ── SCREEN_PERIOD — period measurement ───────────────────────────────────────
void draw_period_screen() {
    tft.fillRect(0, 0, 320, 218, C_BG);
    tft.setTextColor(C_ACCENT, C_BG);
    tft.setTextSize(1);
    tft.setCursor(4, 4);
    tft.print("PERIOD MEASUREMENT");

    tft.setCursor(4, 20);
    tft.setTextColor(C_LGRAY, C_BG);
    if (app.auto_n) {
        tft.printf("N=auto (%u cycles)", app.recip_n);
    } else {
        tft.printf("N=%u cycles", app.recip_n);
    }

    if (!app.result_valid) {
        tft.setTextColor(C_DGRAY, C_BG);
        tft.setTextSize(2);
        tft.setCursor(40, 80);
        tft.print(app.measuring ? "MEASURING..." : "-- s");
    } else {
        tft.setTextColor(C_FG, C_BG);
        tft.setTextSize(2);
        tft.setCursor(4, 55);
        tft.print(app.period_str);

        tft.setTextSize(1);
        tft.setTextColor(C_LGRAY, C_BG);
        tft.setCursor(4, 90);
        tft.printf("= %.6f µs", app.period_ns * 1e-3);
        tft.setCursor(4, 104);
        tft.printf("= %.3f ns", app.period_ns);
        tft.setCursor(4, 118);
        char fstr[32];
        format_frequency(app.frequency_hz, fstr, sizeof(fstr));
        tft.printf("f = %s", fstr);
        tft.setCursor(4, 132);
        tft.printf("±%s", app.resolution_str);
    }
}

// ── SCREEN_SETTINGS ───────────────────────────────────────────────────────────
static int settings_focus = 0;
void draw_settings_screen() {
    tft.fillRect(0, 0, 320, 218, C_BG);
    tft.setTextColor(C_ACCENT, C_BG);
    tft.setTextSize(1);
    tft.setCursor(4, 4);
    tft.print("SETTINGS");

    const char* modes[] = { "DIRECT", "PRESCALE", "RECIPROCAL", "PERIOD" };
    const char* gates[] = { "100 ms", "1 s", "10 s", "100 s" };
    const char* atts[]  = { "0 dB", "-10 dB", "-20 dB", "-30 dB" };
    const char* ns[]    = { "10", "100", "1000", "10000", "100000", "AUTO" };
    uint8_t n_idx_val   = app.auto_n ? 5 : (
        app.recip_n == 10     ? 0 :
        app.recip_n == 100    ? 1 :
        app.recip_n == 1000   ? 2 :
        app.recip_n == 10000  ? 3 : 4);

    // Helper: draw one setting row
    auto row = [&](int y, const char* label, const char* val, bool selected) {
        uint16_t bg = selected ? C_ACCENT : C_BG;
        uint16_t fg = selected ? TFT_BLACK : C_FG;
        tft.fillRect(0, y, 320, 18, bg);
        tft.setTextColor(selected ? TFT_BLACK : C_LGRAY, bg);
        tft.setCursor(4, y + 5);
        tft.print(label);
        tft.setTextColor(fg, bg);
        tft.setCursor(140, y + 5);
        tft.print(val);
    };

    row(24,  "Mode",      modes[app.mode],     settings_focus == 0);
    row(44,  "Gate Time", gates[app.gate_idx], settings_focus == 1);
    row(64,  "Attenuator",atts[app.att_idx],   settings_focus == 2);
    row(84,  "N cycles",  ns[n_idx_val],        settings_focus == 3);

    // Prescaler ratio (auto-set, info only)
    tft.setTextColor(C_DGRAY, C_BG);
    tft.setCursor(4, 108);
    tft.printf("Prescale ratio: ÷%u  (auto)", (uint32_t)app.prescale);

    // TCXO VC DAC value
    tft.setCursor(4, 122);
    tft.printf("TCXO DAC: %u / 255  (%.2fV)", gpsdo.dac_value,
               gpsdo.dac_value * 3.3f / 255.0f);

    // WiFi/BLE toggles
    tft.setTextColor(app.wifi_ap_active ? C_GREEN : C_DGRAY, C_BG);
    tft.setCursor(4, 140);
    tft.printf("WiFi AP: %s  %s",
               app.wifi_ap_active ? "ON " : "OFF", app.ip_str);
    tft.setTextColor(app.ble_active ? C_GREEN : C_DGRAY, C_BG);
    tft.setCursor(4, 154);
    tft.printf("BLE: %s  Stream: %s",
               app.ble_active    ? "ON " : "OFF",
               app.stream_enabled ? "ON " : "OFF");

    tft.setTextColor(C_DGRAY, C_BG);
    tft.setCursor(4, 172);
    tft.print("Touch row to change. +/- to adjust.");
}

// ── SCREEN_GPSDO ──────────────────────────────────────────────────────────────
void draw_gpsdo_screen() {
    tft.fillRect(0, 0, 320, 218, C_BG);
    tft.setTextColor(C_ACCENT, C_BG);
    tft.setTextSize(1);
    tft.setCursor(4, 4);
    tft.print("GPSDO STATUS");

    // Status badge
    uint16_t col;
    if      (gpsdo.locked)     col = C_LOCKED;
    else if (gpsdo.holdover)   col = C_HOLDOVER;
    else if (gpsdo.gps_valid)  col = C_LOCKING;
    else                       col = C_NOGPS;

    tft.fillRect(200, 2, 115, 16, col);
    tft.setTextColor(TFT_BLACK, col);
    tft.setCursor(204, 5);
    tft.print(gpsdo_status_str());

    tft.setTextColor(C_FG, C_BG);
    tft.setTextSize(1);

    auto lrow = [&](int y, const char* label, const char* val, uint16_t vc=C_FG) {
        tft.setTextColor(C_LGRAY, C_BG);
        tft.setCursor(4, y);
        tft.print(label);
        tft.setTextColor(vc, C_BG);
        tft.setCursor(140, y);
        tft.print(val);
    };

    char tbuf[32];

    snprintf(tbuf, sizeof(tbuf), "%.3f ppb", gpsdo.freq_error_ppm * 1000.0);
    lrow(26, "Freq error",   tbuf, fabsf((float)gpsdo.freq_error_ppm) < 0.001 ? C_GREEN : C_YELLOW);

    snprintf(tbuf, sizeof(tbuf), "%.1f ns", gpsdo.phase_error_ns);
    lrow(40, "Phase error",  tbuf, fabsf((float)gpsdo.phase_error_ns) < 200.0 ? C_GREEN : C_YELLOW);

    snprintf(tbuf, sizeof(tbuf), "%.1f ns", gpsdo.phase_accum_ns);
    lrow(54, "Phase accum",  tbuf);

    snprintf(tbuf, sizeof(tbuf), "%u / 255", gpsdo.dac_value);
    lrow(68, "DAC value",    tbuf);

    snprintf(tbuf, sizeof(tbuf), "%d / %d", gpsdo.lock_counter, GPSDO_LOCK_COUNT);
    lrow(82, "Lock count",   tbuf);

    snprintf(tbuf, sizeof(tbuf), "%.3f ppb", gpsdo_accuracy_ppb());
    lrow(96, "Accuracy est", tbuf, gpsdo.locked ? C_GREEN : C_YELLOW);

    lrow(110, "GPS time",    gpsdo.gps_time);
    snprintf(tbuf, sizeof(tbuf), "%d sats  %s",
             gpsdo.gps_satellites, gpsdo.gps_valid ? "VALID" : "NO FIX");
    lrow(124, "GPS",         tbuf, gpsdo.gps_valid ? C_GREEN : C_RED);

    if (gpsdo.error_count > 0) {
        snprintf(tbuf, sizeof(tbuf), "%.2f ppb",
                 gpsdo.error_sum_ppb / gpsdo.error_count);
        lrow(138, "Mean ppb error", tbuf);
        snprintf(tbuf, sizeof(tbuf), "%.2f / %.2f ppb",
                 gpsdo.error_min_ppb, gpsdo.error_max_ppb);
        lrow(152, "Min / Max ppb", tbuf);
    }

    if (gpsdo.holdover) {
        snprintf(tbuf, sizeof(tbuf), "%lu s", gpsdo.holdover_seconds);
        lrow(166, "Holdover time", tbuf, C_ORANGE);
    }
}

// ── SCREEN_NETWORK ────────────────────────────────────────────────────────────
void draw_network_screen() {
    tft.fillRect(0, 0, 320, 218, C_BG);
    tft.setTextColor(C_ACCENT, C_BG);
    tft.setTextSize(1);
    tft.setCursor(4, 4);
    tft.print("NETWORK / REMOTE");

    tft.setTextColor(C_FG, C_BG);
    tft.setCursor(4, 22);
    tft.printf("WiFi AP SSID: %s", WIFI_SSID);
    tft.setCursor(4, 36);
    tft.printf("Password:     %s", WIFI_PASS);
    tft.setCursor(4, 50);
    tft.setTextColor(app.wifi_ap_active ? C_GREEN : C_DGRAY, C_BG);
    tft.printf("IP Address:   %s", app.wifi_ap_active ? app.ip_str : "OFF");

    tft.setTextColor(C_LGRAY, C_BG);
    tft.setCursor(4, 68);
    tft.print("TCP port 7373 — raw freq stream (1 line/measurement)");
    tft.setCursor(4, 82);
    tft.print("HTTP GET http://<IP>/freq — text/plain key=value");

    tft.setTextColor(C_FG, C_BG);
    tft.setCursor(4, 100);
    tft.setTextColor(app.ble_active ? C_GREEN : C_DGRAY, C_BG);
    tft.printf("BLE: %s  Device: FreqCounter",
               app.ble_active ? (ble_connected ? "CONNECTED" : "ADVERTISING") : "OFF");
    tft.setTextColor(C_LGRAY, C_BG);
    tft.setCursor(4, 114);
    tft.printf("Service: %s", BLE_SERVICE_UUID + 4);
    tft.setCursor(4, 128);
    tft.printf("Char:    %s", BLE_CHAR_UUID + 4);
    tft.setCursor(4, 142);
    tft.print("Notify: freq_hz\\n  (UTF-8)");

    tft.setCursor(4, 160);
    tft.setTextColor(app.stream_enabled ? C_GREEN : C_DGRAY, C_BG);
    tft.printf("Stream: %s  [Touch to toggle]", app.stream_enabled ? "ENABLED" : "DISABLED");
}

// ── SCREEN_STATS ──────────────────────────────────────────────────────────────
void draw_stats_screen() {
    tft.fillRect(0, 0, 320, 218, C_BG);
    tft.setTextColor(C_ACCENT, C_BG);
    tft.setTextSize(1);
    tft.setCursor(4, 4);
    tft.printf("STATISTICS  N=%ld", app.stat_count);

    auto srow = [&](int y, const char* label, double val) {
        char vbuf[24];
        format_frequency(val, vbuf, sizeof(vbuf));
        tft.setTextColor(C_LGRAY, C_BG);
        tft.setCursor(4, y);
        tft.print(label);
        tft.setTextColor(C_FG, C_BG);
        tft.setCursor(100, y);
        tft.print(vbuf);
    };

    if (app.stat_count < 2) {
        tft.setTextColor(C_DGRAY, C_BG);
        tft.setCursor(60, 90);
        tft.print("Need ≥2 measurements");
    } else {
        srow(26,  "Min:",    app.stat_min);
        srow(44,  "Max:",    app.stat_max);
        srow(62,  "Mean:",   stats_mean());
        srow(80,  "StdDev:", stats_stddev());

        double spread = app.stat_max - app.stat_min;
        double ppm = (stats_mean() > 0.0) ? (spread / stats_mean() * 1e6) : 0.0;
        tft.setTextColor(C_LGRAY, C_BG);
        tft.setCursor(4, 100);
        tft.printf("Spread: %.3f ppm", ppm);

        tft.setCursor(4, 114);
        tft.setTextColor(C_LGRAY, C_BG);
        tft.printf("GPSDO accuracy: ±%.1f ppb", gpsdo_accuracy_ppb());
    }

    tft.setTextColor(C_DGRAY, C_BG);
    tft.setCursor(4, 178);
    tft.print("[RESET] — clear all statistics");
}

// ── Dispatch screen draw ──────────────────────────────────────────────────────
void draw_screen() {
    switch (app.screen) {
        case SCREEN_MAIN:     draw_main_screen();     break;
        case SCREEN_PERIOD:   draw_period_screen();   break;
        case SCREEN_SETTINGS: draw_settings_screen(); break;
        case SCREEN_GPSDO:    draw_gpsdo_screen();    break;
        case SCREEN_NETWORK:  draw_network_screen();  break;
        case SCREEN_STATS:    draw_stats_screen();    break;
        default: break;
    }
    draw_tab_bar();
}

// ═══════════════════════════════════════════════════════════════════════════════
// TOUCH HANDLING
// ═══════════════════════════════════════════════════════════════════════════════

static uint16_t last_tx = 0, last_ty = 0;
static uint32_t last_touch_ms = 0;
#define TOUCH_DEBOUNCE_MS  200

void handle_tab_touch(uint16_t tx, uint16_t ty) {
    if (ty >= 220 && ty < 240) {
        int tab = tx / (320 / SCREEN_COUNT);
        if (tab >= 0 && tab < SCREEN_COUNT && tab != (int)app.screen) {
            app.screen = (Screen)tab;
            app.needs_redraw = true;
        }
    }
}

void handle_main_touch(uint16_t tx, uint16_t ty) {
    if (ty < 20) return;
    if (ty >= 152 && ty < 172) {
        // Touch hints row
        if      (tx < 80)  { app.hold = !app.hold; }
        else if (tx < 200) { stats_reset(); }
        else               { /* manual start trigger (already auto) */ }
        app.needs_redraw = true;
    }
}

void handle_settings_touch(uint16_t tx, uint16_t ty) {
    if      (ty >= 24 && ty < 42)  settings_focus = 0;
    else if (ty >= 44 && ty < 62)  settings_focus = 1;
    else if (ty >= 64 && ty < 82)  settings_focus = 2;
    else if (ty >= 84 && ty < 102) settings_focus = 3;
    else if (ty >= 140 && ty < 158) {
        app.wifi_ap_active ? WiFi.softAPdisconnect(true) : wifi_init();
        app.wifi_ap_active = !app.wifi_ap_active;
    } else if (ty >= 154 && ty < 172) {
        app.stream_enabled = !app.stream_enabled;
    } else return;

    // Adjust focused setting: left half = decrement, right half = increment
    bool inc = (tx > 160);
    if (settings_focus == 0) {
        int m = (int)app.mode + (inc ? 1 : -1);
        app.mode = (MeasMode)constrain(m, 0, 3);
    } else if (settings_focus == 1) {
        int g = (int)app.gate_idx + (inc ? 1 : -1);
        app.gate_idx = (GateIdx)constrain(g, 0, 3);
        const uint32_t gt[] = { GATE_100MS, GATE_1S, GATE_10S, GATE_100S };
        app.gate_us = gt[app.gate_idx];
    } else if (settings_focus == 2) {
        int a = (int)app.att_idx + (inc ? 1 : -1);
        app.att_idx = (AttIdx)constrain(a, 0, 3);
    } else if (settings_focus == 3) {
        const uint32_t ns_table[] = { 10, 100, 1000, 10000, 100000, 0 };
        // find current index
        int ni = 5; // AUTO
        for (int i = 0; i < 5; i++) if (app.recip_n == ns_table[i]) { ni = i; break; }
        ni = constrain(ni + (inc ? 1 : -1), 0, 5);
        if (ni == 5) {
            app.auto_n  = true;
        } else {
            app.auto_n  = false;
            app.recip_n = ns_table[ni];
        }
    }
    settings_save();
    app.needs_redraw = true;
}

void handle_network_touch(uint16_t tx, uint16_t ty) {
    if (ty >= 160 && ty < 180) {
        app.stream_enabled = !app.stream_enabled;
        app.needs_redraw = true;
    }
}

void handle_stats_touch(uint16_t tx, uint16_t ty) {
    if (ty >= 178 && ty < 200) {
        stats_reset();
        app.needs_redraw = true;
    }
}

void handle_touch() {
    if (!tft.getTouch(&last_tx, &last_ty)) return;
    uint32_t now = millis();
    if (now - last_touch_ms < TOUCH_DEBOUNCE_MS) return;
    last_touch_ms = now;

    handle_tab_touch(last_tx, last_ty);
    if (last_ty >= 220) return;

    switch (app.screen) {
        case SCREEN_MAIN:     handle_main_touch(last_tx, last_ty);     break;
        case SCREEN_SETTINGS: handle_settings_touch(last_tx, last_ty); break;
        case SCREEN_NETWORK:  handle_network_touch(last_tx, last_ty);  break;
        case SCREEN_STATS:    handle_stats_touch(last_tx, last_ty);    break;
        default: break;
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
// MEASUREMENT ENGINE
// ═══════════════════════════════════════════════════════════════════════════════

static uint32_t last_meas_ms = 0;

void auto_select_prescale(double freq_hint) {
    if (app.mode != MODE_PRESCALER) {
        app.prescale = PRESCALE_1;
        return;
    }
    if (freq_hint > 600e6) {
        app.prescale = PRESCALE_64;
        digitalWrite(PRESCALER_MC_PIN, HIGH);
    } else {
        app.prescale = PRESCALE_8;
        digitalWrite(PRESCALER_MC_PIN, LOW);
    }
}

void measurement_start() {
    if (app.measuring || app.hold) return;
    auto_select_prescale(app.frequency_hz);

    uint8_t input_gpio = (app.mode == MODE_PRESCALER) ? PRESCALER_IN_PIN : DIRECT_INPUT_PIN;
    // Reconfigure PCNT if input changed
    static uint8_t last_gpio = 0;
    if (input_gpio != last_gpio) {
        direct_pcnt_init(input_gpio);
        last_gpio = input_gpio;
    }

    if (app.mode == MODE_DIRECT || app.mode == MODE_PRESCALER) {
        direct_count_start();
    } else {
        reciprocal_start_meas();
    }
}

void measurement_poll() {
    if (!app.measuring) return;

    if (app.mode == MODE_DIRECT || app.mode == MODE_PRESCALER) {
        if (!app.gate_open) {
            direct_count_finish();
            app.result_valid = true;
        } else {
            return;
        }
    } else {
        RecipResult rr;
        if (!reciprocal_poll(&rr)) return;
        digitalWrite(GATE_LED_PIN, LOW);
        if (rr.valid) {
            app.frequency_hz = rr.frequency_hz * (double)app.prescale;
            app.period_ns    = rr.period_ns;
            app.accuracy_ppb = rr.accuracy_ppb;
            reciprocal_format_resolution(rr.resolution_hz, app.resolution_str, sizeof(app.resolution_str));
            app.recip_n      = rr.cycles_measured;
            app.result_valid = true;
        } else if (rr.timeout) {
            app.result_valid = false;
        }
        app.measuring = false;
    }

    if (app.result_valid) {
        format_frequency(app.frequency_hz, app.freq_str, sizeof(app.freq_str));
        reciprocal_format_period(
            app.frequency_hz > 0.0 ? (1e9 / app.frequency_hz) : 0.0,
            app.period_str, sizeof(app.period_str));
        stats_update(app.frequency_hz);
        stream_frequency();
        app.needs_update = true;
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
// SETUP
// ═══════════════════════════════════════════════════════════════════════════════

void setup() {
    Serial.begin(115200);

    // GPIO init
    pinMode(GATE_LED_PIN,      OUTPUT);
    pinMode(PRESCALER_MC_PIN,  OUTPUT);
    pinMode(TFT_BL,            OUTPUT);
    digitalWrite(GATE_LED_PIN,     LOW);
    digitalWrite(PRESCALER_MC_PIN, LOW);   // default ÷8
    analogWrite(TFT_BL, 200);              // backlight 78%

    // TFT init
    tft.init();
    tft.setRotation(1);   // landscape
    tft.fillScreen(C_BG);
    tft.setTextColor(C_ACCENT, C_BG);
    tft.setTextSize(2);
    tft.setCursor(60, 100);
    tft.print("FC-001  Rev A");
    tft.setTextSize(1);
    tft.setTextColor(C_LGRAY, C_BG);
    tft.setCursor(80, 130);
    tft.print("Initializing...");

    // Load settings
    settings_load();

    // PCNT for direct count
    direct_pcnt_init(DIRECT_INPUT_PIN);

    // Reciprocal counter
    reciprocal_init();

    // GPSDO
    gpsdo_init();

    // Gate timer
    esp_timer_create_args_t targs = {};
    targs.callback = gate_timer_cb;
    targs.name     = "gate_timer";
    esp_timer_create(&targs, &gate_timer);

    // App state init
    memset(&app, 0, sizeof(app));
    settings_load();
    app.screen       = SCREEN_MAIN;
    app.needs_redraw = true;
    app.auto_n       = true;
    app.recip_n      = RC_N_DEFAULT;
    app.prescale     = PRESCALE_1;
    app.stat_min     = 1e18;
    app.stat_max     = -1e18;

    // WiFi AP + BLE
    wifi_init();
    ble_init();

    delay(1000);
    tft.fillScreen(C_BG);
    Serial.println("[FC001] Ready.");
}

// ═══════════════════════════════════════════════════════════════════════════════
// LOOP
// ═══════════════════════════════════════════════════════════════════════════════

void loop() {
    uint32_t now_ms = millis();

    // GPS and GPSDO
    gpsdo_parse_nmea();
    gpsdo_update();

    // Touch
    handle_touch();

    // Measurement cycle
    if (!app.measuring) {
        uint32_t interval_ms = (app.gate_us / 1000UL) + 50;
        if (now_ms - last_meas_ms >= interval_ms) {
            last_meas_ms = now_ms;
            measurement_start();
        }
    } else {
        measurement_poll();
    }

    // WiFi HTTP
    if (app.wifi_ap_active) {
        WiFiClient http_client = tcp_server.available();
        if (http_client) {
            String first = http_client.readStringUntil('\r');
            if (first.indexOf("HTTP") >= 0) {
                http_serve_client(http_client);
            } else if (app.stream_enabled) {
                tcp_client = http_client;
            }
        }
    }

    // Redraw
    if (app.needs_redraw) {
        draw_screen();
        app.needs_redraw = false;
        app.needs_update = false;
    } else if (app.needs_update && !app.measuring) {
        // Partial update: only refresh data area without full redraw
        // For simplicity, redraw full screen (fast enough at 40 MHz SPI)
        draw_screen();
        app.needs_update = false;
    }

    // Periodic GPSDO display refresh
    static uint32_t gpsdo_refresh_ms = 0;
    if (app.screen == SCREEN_GPSDO && now_ms - gpsdo_refresh_ms >= 1000) {
        gpsdo_refresh_ms = now_ms;
        draw_gpsdo_screen();
        draw_tab_bar();
    }
}
