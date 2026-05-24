/*
 * field_strength_cyd.ino
 * Field Strength Meter — CYD (ESP32-2432S028) Firmware
 * TM-FSM-001 Rev A
 *
 * Hardware:
 *   AD8307 log detector output → GPIO34 (ADC1_CH6)
 *   Battery voltage monitor    → GPIO35 (ADC1_CH7) via 330k/56k divider
 *   Second probe ADC           → GPIO36 (ADC1_CH0)
 *   Audio tone PWM             → GPIO25
 *   Audio mute                 → GPIO26 (LOW = muted)
 *   Status LED                 → GPIO27
 *   SD card: CS=5 SCK=18 MISO=19 MOSI=23 (VSPI)
 *   GPS:     RX=16 TX=17 (UART2, 9600 baud)
 *   TFT:     built-in CYD (HSPI GPIO 13/14/15/2/12)
 *   Touch:   XPT2046 GPIO33
 *
 * Libraries: TFT_eSPI (Bodmer), XPT2046_Touchscreen, SD (built-in)
 * Board: ESP32 Dev Module, 4MB Flash, partition: Default 4MB
 *
 * User_Setup.h (TFT_eSPI):
 *   #define ILI9341_DRIVER
 *   #define TFT_CS 15  TFT_DC 2  TFT_RST 12  TFT_MOSI 13  TFT_SCLK 14
 *   #define TOUCH_CS 33
 */

#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <EEPROM.h>
#include "fsm_datalogger.h"

// ── Pin assignments ──────────────────────────────────────────────────────────
#define ADC_RF      34    // AD8307 VOUT
#define ADC_BATT    35    // battery monitor
#define ADC_PROBE2  36    // secondary probe
#define AUDIO_PIN   25    // PWM tone output
#define MUTE_PIN    26    // audio mute (LOW = muted)
#define STAT_LED    27    // status LED

// ── TFT / Touch ──────────────────────────────────────────────────────────────
TFT_eSPI tft = TFT_eSPI();
XPT2046_Touchscreen touch(33);

// ── Audio ────────────────────────────────────────────────────────────────────
#define AUDIO_LEDC_CH  0
#define AUDIO_FREQ_MIN 200
#define AUDIO_FREQ_MAX 3500

// ── EEPROM layout ────────────────────────────────────────────────────────────
#define EEPROM_SIZE     64
#define EE_MAGIC        0    // 1 byte: 0xA5 = valid
#define EE_ADC_OFFSET   1    // 4 bytes (float): ADC zero offset
#define EE_ADC_SCALE    5    // 4 bytes (float): ADC scale
#define EE_AF_OFFSET    9    // 4 bytes (float): antenna factor user correction
#define EE_AUDIO_MODE   13   // 1 byte: 0=proportional, 1=pulse, 2=off
#define EE_LOG_INTERVAL 14   // 1 byte: 0=manual, 1=5s, 2=10s, 3=30s, 4=60s
#define EE_BRIGHTNESS   15   // 1 byte: 0–100 %
#define EE_TOUCH_CAL    16   // 20 bytes: XPT2046 cal (5 × int32)

// ── Band table ───────────────────────────────────────────────────────────────
struct Band {
    const char* name;
    float freq_MHz;
    float af_whip;    // dB/m, antenna factor for 50cm whip
    float af_dipole;  // dB/m, λ/2 dipole
    float af_loop;    // dB/m, 10cm shielded loop
};

#define NUM_BANDS 15
static const Band BANDS[NUM_BANDS] = {
    {"160M",  1.850f,  32.0f, -30.6f, 56.0f},
    {"80M",   3.750f,  26.0f, -24.6f, 50.0f},
    {"40M",   7.150f,  20.0f, -18.6f, 44.0f},
    {"30M",  10.125f,  17.0f, -14.9f, 41.0f},
    {"20M",  14.175f,  14.0f, -12.5f, 38.0f},
    {"17M",  18.118f,  12.0f, -10.0f, 36.0f},
    {"15M",  21.225f,  10.0f,  -8.5f, 34.0f},
    {"12M",  24.940f,   9.0f,  -7.0f, 33.0f},
    {"10M",  28.500f,   8.0f,  -6.5f, 32.0f},
    {"6M",   50.000f,   6.0f,  -3.0f, 30.0f},
    {"2M",  144.000f,   3.0f,   7.2f, 27.0f},
    {"1.25M",222.000f,  2.0f,  11.5f, 25.0f},
    {"70cm", 432.000f,  1.0f,  17.2f, 23.0f},
    {"33cm", 902.000f,  0.0f,  22.8f, 21.0f},
    {"20cm",1296.000f, -1.0f,  26.2f, 19.0f},
};

// ── Probe types ──────────────────────────────────────────────────────────────
enum ProbeType { PROBE_WHIP, PROBE_DIPOLE, PROBE_LOOP };
static const char* PROBE_NAMES[] = {"WHIP", "DIPOLE", "LOOP"};

// ── Application state ────────────────────────────────────────────────────────
struct AppState {
    int      band_idx;
    int      atten_dB;        // 0, 10, 20, 30
    ProbeType probe;
    float    dBm;
    float    dBm_hold;
    bool     hold_active;
    float    s_units;
    float    E_Vm;            // field strength V/m
    float    E_dBuVm;         // dBμV/m
    float    V_rms_uV;        // probe terminal voltage μV
    float    batt_volts;
    int      screen;          // 0=smeter, 1=calibrated, 2=direction, 3=logger, 4=band, 5=settings
    bool     logging_auto;
    int      log_interval_s;  // 0=manual
    int      audio_mode;      // 0=proportional, 1=pulse, 2=off
    float    adc_offset;
    float    adc_scale;
    float    af_user_dB;      // user calibration offset on top of table AF
    int      brightness;
    // Direction finder
    float    df_dBm_max;
    float    df_dBm_min;
    float    df_bearing_max;
    float    df_bearing_current;
};

static AppState st;

// ── Color palette ─────────────────────────────────────────────────────────────
#define COL_BG      TFT_BLACK
#define COL_HEADER  0x2945    // dark blue-grey header
#define COL_TEXT    TFT_WHITE
#define COL_GREEN   0x07E0
#define COL_YELLOW  0xFFE0
#define COL_RED     0xF800
#define COL_ORANGE  0xFD00
#define COL_DIM     0x4208
#define COL_BUTTON  0x2965
#define COL_BTN_HL  0x5E4B    // highlighted button
#define COL_BARS    0x07FF    // cyan for S-meter bars

// ── Helpers ───────────────────────────────────────────────────────────────────

// Multi-sample ADC read with averaging
float read_adc_volts(int pin, int samples = 32) {
    long sum = 0;
    for (int i = 0; i < samples; i++) sum += analogRead(pin);
    float counts = (float)sum / samples;
    return counts * 3.3f / 4095.0f;
}

// AD8307: V_out → dBm
float ad8307_to_dBm(float v_adc) {
    // V = slope × (Pin_dBm − intercept_dBm)
    // slope = 25 mV/dB = 0.025 V/dB
    // intercept ≈ −84 dBm (nominal; calibrated via adc_offset/scale)
    float v_cal = (v_adc - st.adc_offset) * st.adc_scale;
    return v_cal / 0.025f - 84.0f;
}

// dBm → S-units (float)
float dBm_to_sunits(float dBm) {
    // S1 = −121 dBm; each S-unit = 6 dB
    return (dBm + 121.0f) / 6.0f;
}

// dBm → field strength V/m (requires AF in dB/m)
float dBm_to_Evm(float dBm, float af_dBm) {
    // dBm to dBuV at 50Ω: dBuV = dBm + 107
    float dBuV = dBm + 107.0f;
    // dBuVm = dBuV + AF(dB/m)
    float dBuVm = dBuV + af_dBm + st.af_user_dB;
    // V/m = 10^(dBuVm/20) × 10^-6
    return powf(10.0f, dBuVm / 20.0f) * 1e-6f;
}

// dBm → probe voltage (μV at 50Ω)
float dBm_to_uV(float dBm) {
    float v_rms = sqrtf(powf(10.0f, dBm / 10.0f) / 1000.0f * 50.0f);
    return v_rms * 1e6f;
}

// Battery voltage from ADC
float read_battery() {
    float v_adc = read_adc_volts(ADC_BATT, 16);
    return v_adc * (386.0f / 56.0f);   // 330k + 56k divider
}

// Current AF value for selected band and probe
float current_AF() {
    const Band& b = BANDS[st.band_idx];
    switch (st.probe) {
        case PROBE_DIPOLE: return b.af_dipole;
        case PROBE_LOOP:   return b.af_loop;
        default:           return b.af_whip;
    }
}

// ── S-meter bar color ─────────────────────────────────────────────────────────
uint16_t smeter_color(int seg, int total_segs) {
    // Green for S1–S6, yellow S7–S9, red S9+
    float frac = (float)seg / total_segs;
    if (frac < 0.5f) return COL_GREEN;
    if (frac < 0.75f) return COL_YELLOW;
    return COL_RED;
}

// ── Draw bottom touch buttons (shared across screens) ─────────────────────────
struct Button { int x, y, w, h; const char* label; };

static void draw_button(const Button& b, bool highlighted = false) {
    uint16_t bg = highlighted ? COL_BTN_HL : COL_BUTTON;
    tft.fillRoundRect(b.x, b.y, b.w, b.h, 4, bg);
    tft.drawRoundRect(b.x, b.y, b.w, b.h, 4, COL_DIM);
    tft.setTextColor(COL_TEXT, bg);
    tft.setTextSize(1);
    tft.setCursor(b.x + b.w/2 - strlen(b.label)*3, b.y + b.h/2 - 4);
    tft.print(b.label);
}

static bool button_hit(const Button& b, int tx, int ty) {
    return tx >= b.x && tx < b.x + b.w && ty >= b.y && ty < b.y + b.h;
}

// ── SCREEN 0: S-METER MAIN ────────────────────────────────────────────────────
void draw_smeter_screen() {
    tft.fillScreen(COL_BG);

    // Header bar
    tft.fillRect(0, 0, 320, 22, COL_HEADER);
    tft.setTextColor(COL_TEXT, COL_HEADER);
    tft.setTextSize(1);
    tft.setCursor(4, 7);
    tft.printf("FSM  [%s %.3fMHz]  [-%ddB]",
               BANDS[st.band_idx].name,
               BANDS[st.band_idx].freq_MHz,
               st.atten_dB);

    // Battery indicator (top right)
    int batt_pct = (int)((st.batt_volts - 14.0f) / 4.0f * 100.0f);
    batt_pct = constrain(batt_pct, 0, 100);
    tft.setCursor(260, 7);
    tft.printf("B:%d%%", batt_pct);

    // --- S-meter bar --------------------------------------------------------
    int bar_x = 10, bar_y = 28, bar_w = 300, bar_h = 22;
    tft.drawRect(bar_x - 1, bar_y - 1, bar_w + 2, bar_h + 2, COL_DIM);

    int total_segs = 30;
    float s_normalized = constrain(st.s_units, 0.0f, 9.0f + 30.0f/6.0f); // S9+30 max
    int lit_segs = (int)(s_normalized / (9.0f + 30.0f/6.0f) * total_segs);
    lit_segs = constrain(lit_segs, 0, total_segs);

    int seg_w = bar_w / total_segs;
    for (int i = 0; i < total_segs; i++) {
        uint16_t col = (i < lit_segs) ? smeter_color(i, total_segs) : 0x1082;
        tft.fillRect(bar_x + i * seg_w + 1, bar_y + 1, seg_w - 1, bar_h - 2, col);
    }

    // S-meter labels below bar
    tft.setTextColor(COL_DIM, COL_BG);
    tft.setTextSize(1);
    for (int s = 1; s <= 9; s++) {
        int seg = (int)((float)s / (9.0f + 30.0f/6.0f) * total_segs);
        int px = bar_x + seg * seg_w;
        tft.drawFastVLine(px, bar_y + bar_h, 4, COL_DIM);
        tft.setCursor(px - 3, bar_y + bar_h + 5);
        tft.printf("%d", s);
    }
    // +10, +20, +30 marks
    for (int over = 10; over <= 30; over += 10) {
        float sval = 9.0f + over / 6.0f;
        int seg = (int)(sval / (9.0f + 30.0f/6.0f) * total_segs);
        int px = bar_x + seg * seg_w;
        tft.drawFastVLine(px, bar_y + bar_h, 4, COL_DIM);
        tft.setCursor(px - 6, bar_y + bar_h + 5);
        tft.printf("+%d", over);
    }

    // --- Large numeric readout -----------------------------------------------
    int ny = 70;
    tft.setTextSize(2);
    tft.setTextColor(COL_TEXT, COL_BG);

    // S-unit reading
    float s_disp = constrain(st.s_units, 0.0f, 9.0f);
    float s_over = st.dBm + 73.0f;
    char s_str[24];
    if (s_over > 0)
        snprintf(s_str, sizeof(s_str), "S9+%.0fdB", s_over);
    else
        snprintf(s_str, sizeof(s_str), "S%.1f", s_disp);
    tft.setCursor(10, ny);
    tft.setTextColor(s_over > 0 ? COL_RED : (st.s_units > 6 ? COL_YELLOW : COL_GREEN), COL_BG);
    tft.print(s_str);

    // dBm
    tft.setTextColor(COL_TEXT, COL_BG);
    tft.setCursor(160, ny);
    tft.printf("%.1f dBm", st.hold_active ? st.dBm_hold : st.dBm);

    // HOLD indicator
    if (st.hold_active) {
        tft.setTextColor(COL_YELLOW, COL_BG);
        tft.setTextSize(1);
        tft.setCursor(285, ny);
        tft.print("HOLD");
    }

    // Field strength
    tft.setTextSize(1);
    tft.setTextColor(COL_TEXT, COL_BG);
    ny += 22;
    if (st.E_Vm < 0.001f)
        tft.setCursor(10, ny), tft.printf("E = %.2f mV/m", st.E_Vm * 1000.0f);
    else if (st.E_Vm < 1.0f)
        tft.setCursor(10, ny), tft.printf("E = %.4f V/m  (%.1f dBuV/m)", st.E_Vm, st.E_dBuVm);
    else
        tft.setCursor(10, ny), tft.printf("E = %.3f V/m  (%.1f dBuV/m)", st.E_Vm, st.E_dBuVm);

    // Probe voltage
    ny += 14;
    tft.setCursor(10, ny);
    if (st.V_rms_uV < 1000.0f)
        tft.printf("V = %.2f uV  Probe: %s", st.V_rms_uV, PROBE_NAMES[(int)st.probe]);
    else
        tft.printf("V = %.3f mV  Probe: %s", st.V_rms_uV / 1000.0f, PROBE_NAMES[(int)st.probe]);

    // GPS / log status line
    ny += 14;
    tft.setCursor(10, ny);
    if (logger_gps_ok()) {
        const GPSFix& gps = logger_gps();
        tft.setTextColor(COL_GREEN, COL_BG);
        tft.printf("GPS: %.4f N  %.4f E  %s UTC", gps.lat, gps.lon, gps.utc_time);
    } else {
        tft.setTextColor(COL_YELLOW, COL_BG);
        tft.print("GPS: acquiring...  SD:");
        tft.setTextColor(logger_sd_ok() ? COL_GREEN : COL_RED, COL_BG);
        tft.print(logger_sd_ok() ? "OK" : "NONE");
        tft.setTextColor(COL_TEXT, COL_BG);
        tft.printf("  #%d", logger_count());
    }

    // --- Bottom touch buttons -----------------------------------------------
    static const Button btns[] = {
        {  0, 210, 52, 30, "BND-"},
        { 53, 210, 52, 30, "BND+"},
        {106, 210, 52, 30, "HOLD"},
        {159, 210, 52, 30, "LOG"},
        {212, 210, 52, 30, "DIR"},
        {265, 210, 52, 30, "SET"},
    };
    for (auto& b : btns) draw_button(b, false);

    // Auto-log indicator
    if (st.logging_auto) {
        tft.setTextColor(COL_GREEN, COL_BUTTON);
        tft.setCursor(163, 218);
        tft.print("LOG*");
    }
}

// ── SCREEN 1: CALIBRATED ─────────────────────────────────────────────────────
void draw_calibrated_screen() {
    tft.fillScreen(COL_BG);
    tft.fillRect(0, 0, 320, 22, COL_HEADER);
    tft.setTextColor(COL_TEXT, COL_HEADER);
    tft.setTextSize(1);
    tft.setCursor(4, 7);
    tft.printf("CALIBRATED  %s  %.3f MHz  %s",
               BANDS[st.band_idx].name, BANDS[st.band_idx].freq_MHz,
               PROBE_NAMES[(int)st.probe]);

    tft.setTextColor(COL_TEXT, COL_BG);
    int y = 30;

    // Large dBm
    tft.setTextSize(3);
    tft.setCursor(10, y);
    tft.printf("%.1f", st.hold_active ? st.dBm_hold : st.dBm);
    tft.setTextSize(2);
    tft.setCursor(140, y + 8);
    tft.print("dBm");

    y += 40;
    tft.setTextSize(2);
    // E field
    tft.setCursor(10, y);
    if (st.E_Vm < 0.001f)
        tft.printf("%.3f mV/m", st.E_Vm * 1000.0f);
    else
        tft.printf("%.4f V/m", st.E_Vm);

    y += 24;
    tft.setCursor(10, y);
    tft.printf("%.1f dBuV/m", st.E_dBuVm);

    y += 24;
    tft.setTextSize(1);
    tft.setTextColor(COL_DIM, COL_BG);
    tft.setCursor(10, y);
    tft.printf("AF = %.1f dB/m   User offset = %+.1f dB",
               current_AF(), st.af_user_dB);

    y += 18;
    tft.setTextColor(COL_TEXT, COL_BG);
    tft.setTextSize(2);

    // Probe voltage
    tft.setCursor(10, y);
    if (st.V_rms_uV < 1000.0f)
        tft.printf("%.3f uV rms", st.V_rms_uV);
    else
        tft.printf("%.4f mV rms", st.V_rms_uV / 1000.0f);

    y += 24;
    // S-units
    float s_disp = constrain(st.s_units, 0.0f, 9.0f);
    float s_over = st.dBm + 73.0f;
    tft.setCursor(10, y);
    if (s_over > 0)
        tft.printf("S9 +%.0f dB", s_over);
    else
        tft.printf("S %.1f", s_disp);

    // Back button
    static const Button back = {0, 210, 80, 30, "BACK"};
    draw_button(back);
}

// ── SCREEN 2: DIRECTION FINDER ────────────────────────────────────────────────
void draw_direction_screen() {
    tft.fillScreen(COL_BG);
    tft.fillRect(0, 0, 320, 22, COL_HEADER);
    tft.setTextColor(COL_TEXT, COL_HEADER);
    tft.setTextSize(1);
    tft.setCursor(4, 7);
    tft.print("DIRECTION FINDER — Rotate loop for null/peak");

    // Compass circle
    int cx = 160, cy = 130, cr = 90;
    tft.drawCircle(cx, cy, cr, COL_DIM);
    tft.drawCircle(cx, cy, cr + 1, COL_DIM);

    // Cardinal labels
    tft.setTextColor(COL_DIM, COL_BG);
    tft.setTextSize(1);
    tft.setCursor(cx - 3, cy - cr - 10); tft.print("N");
    tft.setCursor(cx - 3, cy + cr + 2);  tft.print("S");
    tft.setCursor(cx + cr + 2, cy - 4);  tft.print("E");
    tft.setCursor(cx - cr - 8, cy - 4);  tft.print("W");

    // Current bearing line (green)
    float bearing_rad = st.df_bearing_current * M_PI / 180.0f;
    int lx = cx + (int)(cr * 0.85f * sinf(bearing_rad));
    int ly = cy - (int)(cr * 0.85f * cosf(bearing_rad));
    tft.drawLine(cx, cy, lx, ly, COL_GREEN);

    // Max bearing line (red)
    float max_rad = st.df_bearing_max * M_PI / 180.0f;
    int mx = cx + (int)(cr * 0.7f * sinf(max_rad));
    int my = cy - (int)(cr * 0.7f * cosf(max_rad));
    tft.drawLine(cx, cy, mx, my, COL_RED);

    // Readout
    tft.setTextColor(COL_TEXT, COL_BG);
    tft.setTextSize(1);
    tft.setCursor(4, 32);
    tft.printf("Current: %.1f dBm", st.dBm);
    tft.setCursor(4, 46);
    tft.printf("Peak:    %.1f dBm @ %.0f deg", st.df_dBm_max, st.df_bearing_max);
    tft.setCursor(4, 60);
    tft.printf("Null:    %.1f dBm", st.df_dBm_min);

    // Buttons
    static const Button btns[] = {
        {  0, 210, 80, 30, "RESET"},
        { 85, 210, 80, 30, "BACK"},
        {170, 210, 70, 30, "BEAR+"},
        {245, 210, 70, 30, "BEAR-"},
    };
    for (auto& b : btns) draw_button(b);
}

// ── SCREEN 3: DATA LOGGER ─────────────────────────────────────────────────────
void draw_logger_screen() {
    tft.fillScreen(COL_BG);
    tft.fillRect(0, 0, 320, 22, COL_HEADER);
    tft.setTextColor(COL_TEXT, COL_HEADER);
    tft.setTextSize(1);
    tft.setCursor(4, 7);
    tft.printf("DATA LOGGER  %s  #%d entries",
               logger_sd_ok() ? "SD:OK" : "SD:--", logger_count());

    // GPS status
    tft.setTextColor(logger_gps_ok() ? COL_GREEN : COL_YELLOW, COL_BG);
    tft.setCursor(4, 26);
    if (logger_gps_ok()) {
        const GPSFix& gps = logger_gps();
        tft.printf("GPS: %s  %s  Sats:%d  Alt:%.0fm",
                   gps.utc_time, gps.utc_date, gps.satellites, gps.alt_m);
        tft.setTextColor(COL_TEXT, COL_BG);
        tft.setCursor(4, 38);
        tft.printf("Pos: %.5f N  %.5f E", gps.lat, gps.lon);
    } else {
        tft.print("GPS: Acquiring... (allow 90-180s cold start)");
    }

    // File info
    tft.setTextColor(COL_DIM, COL_BG);
    tft.setCursor(4, 52);
    tft.printf("File: %s", logger_filename());

    // Last log entries
    static char log_lines[5][96];
    int n = logger_read_last(log_lines, 5, 96);
    tft.setTextColor(COL_TEXT, COL_BG);
    for (int i = 0; i < n; i++) {
        tft.setCursor(4, 66 + i * 14);
        tft.printf("%.60s", log_lines[i]);
    }

    static const Button btns[] = {
        {  0, 210, 80, 30, st.logging_auto ? "AUTO*" : "AUTO"},
        { 85, 210, 80, 30, "LOG NOW"},
        {170, 210, 70, 30, "CLEAR"},
        {245, 210, 70, 30, "BACK"},
    };
    for (auto& b : btns) draw_button(b, (b.x == 0 && st.logging_auto));
}

// ── SCREEN 4: BAND SELECT ─────────────────────────────────────────────────────
void draw_band_screen() {
    tft.fillScreen(COL_BG);
    tft.fillRect(0, 0, 320, 22, COL_HEADER);
    tft.setTextColor(COL_TEXT, COL_HEADER);
    tft.setTextSize(1);
    tft.setCursor(4, 7);
    tft.print("SELECT BAND");

    // Grid: 3 columns × 5 rows
    for (int i = 0; i < NUM_BANDS; i++) {
        int col = i % 3;
        int row = i / 3;
        int bx = 4 + col * 105;
        int by = 28 + row * 36;
        bool sel = (i == st.band_idx);
        tft.fillRoundRect(bx, by, 100, 30, 4, sel ? COL_BTN_HL : COL_BUTTON);
        tft.drawRoundRect(bx, by, 100, 30, 4, sel ? COL_TEXT : COL_DIM);
        tft.setTextColor(COL_TEXT, sel ? COL_BTN_HL : COL_BUTTON);
        tft.setTextSize(1);
        tft.setCursor(bx + 4, by + 6);
        tft.printf("%s", BANDS[i].name);
        tft.setCursor(bx + 4, by + 16);
        tft.printf("%.3f MHz", BANDS[i].freq_MHz);
    }

    static const Button back = {0, 210, 80, 30, "BACK"};
    draw_button(back);
}

// ── SCREEN 5: SETTINGS ───────────────────────────────────────────────────────
void draw_settings_screen() {
    tft.fillScreen(COL_BG);
    tft.fillRect(0, 0, 320, 22, COL_HEADER);
    tft.setTextColor(COL_TEXT, COL_HEADER);
    tft.setTextSize(1);
    tft.setCursor(4, 7);
    tft.print("SETTINGS");

    tft.setTextColor(COL_TEXT, COL_BG);
    int y = 28;

    tft.setCursor(4, y); y += 16;
    tft.printf("Attenuation: %d dB", st.atten_dB);

    tft.setCursor(4, y); y += 16;
    static const char* audio_labels[] = {"Proportional", "Pulse", "Off"};
    tft.printf("Audio: %s", audio_labels[st.audio_mode]);

    tft.setCursor(4, y); y += 16;
    tft.printf("AF correction: %+.1f dB  Probe: %s",
               st.af_user_dB, PROBE_NAMES[(int)st.probe]);

    tft.setCursor(4, y); y += 16;
    static const char* int_labels[] = {"Manual", "5s", "10s", "30s", "60s"};
    tft.printf("Log interval: %s", int_labels[st.log_interval_s]);

    tft.setCursor(4, y); y += 16;
    tft.printf("Brightness: %d%%", st.brightness);

    tft.setCursor(4, y); y += 16;
    tft.printf("ADC offset: %.4fV  scale: %.4f", st.adc_offset, st.adc_scale);

    tft.setCursor(4, y); y += 16;
    tft.printf("SD: %s  GPS: %s  Log#: %d",
               logger_sd_ok() ? "OK" : "NONE",
               logger_gps_ok() ? "FIX" : "WAIT",
               logger_count());

    static const Button btns[] = {
        {  0, 180, 78, 28, "ATT+"},
        { 80, 180, 78, 28, "ATT-"},
        {160, 180, 78, 28, "AUDIO"},
        {240, 180, 78, 28, "PROBE"},
        {  0, 210, 78, 28, "AF+"},
        { 80, 210, 78, 28, "AF-"},
        {160, 210, 78, 28, "SAVE"},
        {240, 210, 78, 28, "BACK"},
    };
    for (auto& b : btns) draw_button(b);
}

// ── Touch handler ──────────────────────────────────────────────────────────────
void handle_touch() {
    if (!touch.tirqTouched() || !touch.touched()) return;
    TS_Point p = touch.getPoint();

    // Map raw touch to screen coordinates (calibrate if needed)
    int tx = map(p.x, 200, 3800, 0, 320);
    int ty = map(p.y, 200, 3800, 0, 240);
    tx = constrain(tx, 0, 319);
    ty = constrain(ty, 0, 239);

    delay(80);  // debounce

    if (st.screen == 0) {
        // S-meter buttons
        if (tx < 53 && ty > 205)  { st.band_idx = max(0, st.band_idx - 1); return; }
        if (tx < 106 && ty > 205) { st.band_idx = min(NUM_BANDS-1, st.band_idx + 1); return; }
        if (tx < 160 && ty > 205) { // HOLD
            st.hold_active = !st.hold_active;
            if (st.hold_active) st.dBm_hold = st.dBm;
            return;
        }
        if (tx < 212 && ty > 205) { // LOG
            FSMReading r;
            r.dBm = st.dBm; r.V_rms_uV = st.V_rms_uV;
            r.E_Vm = st.E_Vm; r.s_units = st.s_units;
            r.band_name = BANDS[st.band_idx].name;
            r.freq_MHz = BANDS[st.band_idx].freq_MHz;
            r.atten_dB = st.atten_dB; r.notes = "";
            logger_write(r);
            return;
        }
        if (tx < 265 && ty > 205) { st.screen = 2; tft.fillScreen(COL_BG); return; } // DIR
        if (ty > 205)              { st.screen = 5; tft.fillScreen(COL_BG); return; } // SET
        // Tap upper area → calibrated screen
        if (ty > 22 && ty < 180)   { st.screen = 1; tft.fillScreen(COL_BG); }
    }
    else if (st.screen == 1) {
        st.screen = 0; tft.fillScreen(COL_BG);
    }
    else if (st.screen == 2) {
        if (tx < 80 && ty > 205) {
            st.df_dBm_max = -999.0f; st.df_dBm_min = 0.0f;
            st.df_bearing_max = 0.0f; return;
        }
        if (tx < 165 && ty > 205) { st.screen = 0; tft.fillScreen(COL_BG); return; }
        if (tx < 245 && ty > 205) { st.df_bearing_current = fmodf(st.df_bearing_current + 5.0f, 360.0f); return; }
        if (ty > 205) { st.df_bearing_current = fmodf(st.df_bearing_current - 5.0f + 360.0f, 360.0f); }
    }
    else if (st.screen == 3) {
        if (tx < 80 && ty > 205) { st.logging_auto = !st.logging_auto; return; }
        if (tx < 165 && ty > 205) {
            FSMReading r;
            r.dBm = st.dBm; r.V_rms_uV = st.V_rms_uV; r.E_Vm = st.E_Vm;
            r.s_units = st.s_units; r.band_name = BANDS[st.band_idx].name;
            r.freq_MHz = BANDS[st.band_idx].freq_MHz;
            r.atten_dB = st.atten_dB; r.notes = "manual";
            logger_write(r);
            return;
        }
        if (ty > 205) { st.screen = 0; tft.fillScreen(COL_BG); }
    }
    else if (st.screen == 4) {
        // Band grid
        if (ty > 205) { st.screen = 0; tft.fillScreen(COL_BG); return; }
        for (int i = 0; i < NUM_BANDS; i++) {
            int col = i % 3, row = i / 3;
            int bx = 4 + col * 105, by = 28 + row * 36;
            if (tx >= bx && tx < bx+100 && ty >= by && ty < by+30) {
                st.band_idx = i;
                st.screen = 0;
                tft.fillScreen(COL_BG);
                return;
            }
        }
    }
    else if (st.screen == 5) {
        if (tx < 80 && ty > 175 && ty < 210) {
            st.atten_dB = min(30, st.atten_dB + 10); return;
        }
        if (tx < 160 && ty > 175 && ty < 210) {
            st.atten_dB = max(0, st.atten_dB - 10); return;
        }
        if (tx < 240 && ty > 175 && ty < 210) {
            st.audio_mode = (st.audio_mode + 1) % 3; return;
        }
        if (ty > 175 && ty < 210) {
            st.probe = (ProbeType)((int)(st.probe + 1) % 3); return;
        }
        if (tx < 80 && ty > 205)  { st.af_user_dB += 1.0f; return; }
        if (tx < 160 && ty > 205) { st.af_user_dB -= 1.0f; return; }
        if (tx < 240 && ty > 205) {
            // Save to EEPROM
            EEPROM.writeByte(EE_MAGIC, 0xA5);
            EEPROM.writeFloat(EE_ADC_OFFSET, st.adc_offset);
            EEPROM.writeFloat(EE_ADC_SCALE,  st.adc_scale);
            EEPROM.writeFloat(EE_AF_OFFSET,  st.af_user_dB);
            EEPROM.writeByte(EE_AUDIO_MODE,  st.audio_mode);
            EEPROM.writeByte(EE_LOG_INTERVAL,st.log_interval_s);
            EEPROM.writeByte(EE_BRIGHTNESS,  st.brightness);
            EEPROM.commit();
            return;
        }
        if (ty > 205) { st.screen = 0; tft.fillScreen(COL_BG); }
    }
}

// ── Audio output ──────────────────────────────────────────────────────────────
static unsigned long pulse_last_ms = 0;
static bool pulse_on = false;

void update_audio() {
    if (st.audio_mode == 2) {
        ledcWriteTone(AUDIO_LEDC_CH, 0);
        return;
    }
    float dbm = st.hold_active ? st.dBm_hold : st.dBm;
    if (dbm < -100.0f) { ledcWriteTone(AUDIO_LEDC_CH, 0); return; }

    if (st.audio_mode == 0) {
        // Proportional: map dBm to tone frequency
        int freq = (int)map((long)(dbm * 10), -1000, 100,
                            AUDIO_FREQ_MIN, AUDIO_FREQ_MAX);
        freq = constrain(freq, AUDIO_FREQ_MIN, AUDIO_FREQ_MAX);
        ledcWriteTone(AUDIO_LEDC_CH, freq);
    } else {
        // Pulse mode: rate = 10^((dBm+100)/20) pulses/sec, 50ms beep
        float rate = powf(10.0f, (dbm + 100.0f) / 20.0f);
        rate = constrain(rate, 0.1f, 30.0f);
        unsigned long interval_ms = (unsigned long)(1000.0f / rate);
        unsigned long now = millis();
        if (!pulse_on && (now - pulse_last_ms > interval_ms)) {
            ledcWriteTone(AUDIO_LEDC_CH, 1000);
            pulse_on = true;
            pulse_last_ms = now;
        } else if (pulse_on && (now - pulse_last_ms > 50)) {
            ledcWriteTone(AUDIO_LEDC_CH, 0);
            pulse_on = false;
        }
    }
}

// ── EEPROM load/defaults ──────────────────────────────────────────────────────
void load_eeprom() {
    EEPROM.begin(EEPROM_SIZE);
    if (EEPROM.readByte(EE_MAGIC) == 0xA5) {
        st.adc_offset       = EEPROM.readFloat(EE_ADC_OFFSET);
        st.adc_scale        = EEPROM.readFloat(EE_ADC_SCALE);
        st.af_user_dB       = EEPROM.readFloat(EE_AF_OFFSET);
        st.audio_mode       = EEPROM.readByte(EE_AUDIO_MODE);
        st.log_interval_s   = EEPROM.readByte(EE_LOG_INTERVAL);
        st.brightness       = EEPROM.readByte(EE_BRIGHTNESS);
    } else {
        st.adc_offset     = 0.0f;
        st.adc_scale      = 1.0f;
        st.af_user_dB     = 0.0f;
        st.audio_mode     = 0;
        st.log_interval_s = 0;
        st.brightness     = 80;
    }
}

// ── setup() ──────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    Serial.println("FSM CYD boot v1.0");

    // Initialize state
    memset(&st, 0, sizeof(st));
    st.band_idx    = 2;    // 40M default
    st.atten_dB    = 0;
    st.probe       = PROBE_WHIP;
    st.screen      = 0;
    st.df_dBm_max  = -999.0f;
    st.df_dBm_min  = 0.0f;
    load_eeprom();

    // ADC
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);

    // Audio
    pinMode(MUTE_PIN, OUTPUT);
    digitalWrite(MUTE_PIN, HIGH);
    ledcSetup(AUDIO_LEDC_CH, 1000, 8);
    ledcAttachPin(AUDIO_PIN, AUDIO_LEDC_CH);

    // Status LED
    pinMode(STAT_LED, OUTPUT);
    digitalWrite(STAT_LED, LOW);

    // TFT
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(COL_BG);
    ledcSetup(1, 5000, 8);
    ledcAttachPin(21, 1);
    ledcWrite(1, (int)(st.brightness * 255.0f / 100.0f));

    // Touch
    touch.begin();
    touch.setRotation(1);

    // GPS + SD
    logger_gps_init();
    if (!logger_sd_init()) {
        Serial.println("SD init failed");
    }

    // Splash
    tft.setTextColor(COL_TEXT, COL_BG);
    tft.setTextSize(2);
    tft.setCursor(40, 80);
    tft.print("FIELD STRENGTH METER");
    tft.setTextSize(1);
    tft.setCursor(80, 110);
    tft.printf("TM-FSM-001 Rev A   Band: %s", BANDS[st.band_idx].name);
    tft.setCursor(80, 126);
    tft.printf("SD: %s   GPS: acquiring",
               logger_sd_ok() ? "OK" : "not found");
    delay(1500);
    tft.fillScreen(COL_BG);
}

// ── loop() ───────────────────────────────────────────────────────────────────
static unsigned long last_measure_ms  = 0;
static unsigned long last_display_ms  = 0;
static unsigned long last_autolog_ms  = 0;
static unsigned long last_debug_ms    = 0;
static unsigned long last_screen_ms   = 0;

void loop() {
    unsigned long now = millis();

    // GPS poll (non-blocking, every loop iteration)
    gps_poll();
    logger_update_filename();
    led_blink_update();

    // RF measurement: 50 ms update rate
    if (now - last_measure_ms > 50) {
        last_measure_ms = now;

        float v_adc = read_adc_volts(ADC_RF, 64);
        st.dBm = ad8307_to_dBm(v_adc) - (float)st.atten_dB;

        float af = current_AF();
        st.s_units    = dBm_to_sunits(st.dBm);
        st.E_Vm       = dBm_to_Evm(st.dBm, af);
        st.E_dBuVm    = 20.0f * log10f(st.E_Vm * 1e6f);
        st.V_rms_uV   = dBm_to_uV(st.dBm);
        st.batt_volts = read_battery();

        // Direction finder tracking
        if (st.dBm > st.df_dBm_max) {
            st.df_dBm_max = st.dBm;
            st.df_bearing_max = st.df_bearing_current;
        }
        if (st.dBm < st.df_dBm_min) st.df_dBm_min = st.dBm;
    }

    // Display update: 200 ms
    if (now - last_display_ms > 200) {
        last_display_ms = now;
        switch (st.screen) {
            case 0: draw_smeter_screen();     break;
            case 1: draw_calibrated_screen(); break;
            case 2: draw_direction_screen();  break;
            case 3: draw_logger_screen();     break;
            case 4: draw_band_screen();       break;
            case 5: draw_settings_screen();   break;
        }
    }

    // Auto-logging
    if (st.logging_auto && st.log_interval_s > 0) {
        static const int intervals_s[] = {0, 5, 10, 30, 60};
        unsigned long interval_ms = intervals_s[st.log_interval_s] * 1000UL;
        if (now - last_autolog_ms > interval_ms) {
            last_autolog_ms = now;
            FSMReading r;
            r.dBm = st.dBm; r.V_rms_uV = st.V_rms_uV; r.E_Vm = st.E_Vm;
            r.s_units = st.s_units; r.band_name = BANDS[st.band_idx].name;
            r.freq_MHz = BANDS[st.band_idx].freq_MHz;
            r.atten_dB = st.atten_dB; r.notes = "auto";
            logger_write(r);
        }
    }

    // Audio
    update_audio();

    // Touch
    handle_touch();

    // GPS lock LED indication
    if (now - last_screen_ms > 1000) {
        last_screen_ms = now;
        led_blink(logger_gps_ok() ? 2 : 1);
    }

    // Serial debug every 2s
    if (now - last_debug_ms > 2000) {
        last_debug_ms = now;
        Serial.printf("[FSM] %s %.3fMHz  atten=-%ddB  dBm=%.1f  S=%.1f  "
                      "E=%.4fV/m  GPS:%s  SD:%s  log#%d\n",
                      BANDS[st.band_idx].name,
                      BANDS[st.band_idx].freq_MHz,
                      st.atten_dB,
                      st.dBm,
                      st.s_units,
                      st.E_Vm,
                      logger_gps_ok() ? "FIX" : "WAIT",
                      logger_sd_ok()  ? "OK"  : "NONE",
                      logger_count());
    }
}
