/*
 * siggen_cyd.ino
 * VFO / Signal Generator — CYD (ESP32-2432S028 / ILI9341) Main Firmware
 * TM-SG-001  Rev A
 *
 * Hardware: CYD (ESP32 + 2.8" 320×240 ILI9341 TFT + XPT2046 touch)
 * Synthesizers: AD9851 DDS (HF), Si5351 (HF/VHF), ADF4351 (VHF/UHF/µwave)
 * Output control: HMC1119 digital attenuator, switched π-attenuators via relays
 * Power monitor: AD8307 log detector
 * Reference: 25 MHz TCXO (Si5351/DDS), 10 MHz TCXO (ADF4351)
 *
 * CYD GPIO map:
 *   TFT:       SPI HSPI (CLK=14, MOSI=13, MISO=12, CS=15, DC=2, RST=N/C)
 *   Touch:     SPI VSPI (CLK=25, MOSI=32, MISO=39, CS=33, IRQ=36)  ← CYD default
 *   Synth SPI: HSPI with GPIO18/23 (remapped from TFT bus — use separate SPI)
 *
 *   *** CYD SPI NOTE: ***
 *   CYD uses HSPI for TFT (GPIO14/13/12/15) and VSPI for touch (GPIO25/32/39/33).
 *   The synthesizer SPI bus uses GPIO18(CLK)/23(MOSI) on the default VSPI pins.
 *   Touch is disabled or remapped.  In this design, SPI is bit-banged for synths
 *   using GPIO18/23 to avoid conflict.  Alternatively, use the VSPI instance
 *   (SPI class = VSPI) with different CS pins per device.
 *
 *   Synth SPI (VSPI): CLK=18, MOSI=23 (TFT uses HSPI; no conflict)
 *   AD9851 FQ_UD: GPIO5
 *   AD9851 RESET: GPIO4
 *   ADF4351 LE:   GPIO16
 *   HMC1119 CS:   GPIO17
 *
 *   I2C (Si5351 + PCF8574):  SDA=21, SCL=22
 *
 *   ADC inputs (ADC1 only):
 *     GPIO34: Audio input (AF_ADC)
 *     GPIO35: Battery voltage monitor
 *     GPIO36: AD8307 power monitor
 *
 *   Encoder: CLK=GPIO26, DT=GPIO27, SW=GPIO0 (boot button as menu select)
 *   CW Key:  GPIO32 (active LOW; internal pull-up)
 *
 * Required libraries (install via Arduino Library Manager):
 *   TFT_eSPI          — Bodmer (configure for ILI9341 + CYD pinout)
 *   Etherkit Si5351   — Jason Milldrum (NT7S)
 *   Wire              — built-in
 *   SPI               — built-in
 *   Preferences       — built-in (NVS settings storage)
 */

#include <SPI.h>
#include <Wire.h>
#include <TFT_eSPI.h>
#include <si5351.h>
#include <Preferences.h>

#include "dds_driver.h"
#include "adf4351_driver.h"
#include "sweep_modulation.h"

// ── TFT ──────────────────────────────────────────────────────────────────────
TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft);  // sprite for flicker-free updates

// ── Synthesizer objects ───────────────────────────────────────────────────────
DDS_AD9851  dds;
Si5351      si5351;
ADF4351     adf4351;

// ── Preferences (NVS) ────────────────────────────────────────────────────────
Preferences prefs;

// ── Encoder pins ─────────────────────────────────────────────────────────────
#define ENC_CLK   26
#define ENC_DT    27
#define ENC_SW     0   // Boot button

// ── Battery monitor ───────────────────────────────────────────────────────────
#define BATT_PIN  35
// Voltage divider: 47kΩ + 10kΩ → factor = (47+10)/10 = 5.7
#define BATT_DIV  5.7f

// ── Screen definitions ───────────────────────────────────────────────────────
enum Screen {
    SCR_MAIN,       // Main VFO / signal gen display
    SCR_SWEEP,      // Sweep mode (spectrum view)
    SCR_LEVEL,      // Output level control
    SCR_MOD,        // Modulation settings
    SCR_SETTINGS,   // Calibration, reference, band filter
    SCR_ABOUT
};

// ── Synthesizer source ───────────────────────────────────────────────────────
enum SynthSrc { SRC_DDS, SRC_SI5351, SRC_ADF4351 };

// ── Application state ────────────────────────────────────────────────────────
struct AppState {
    Screen    screen;
    SynthSrc  synth;
    uint32_t  freq_hz;        // Current output frequency
    uint32_t  freq_step;      // Tuning step (Hz)
    float     level_dBm;      // Target output level
    float     meas_dBm;       // AD8307 measured level
    ModMode   mod_mode;
    float     fm_dev_hz;
    float     am_depth;
    bool      rf_on;
    bool      locked;         // ADF4351 lock status
    // Sweep config (persistent across screen)
    SweepConfig  sweep;
    SweepResult  sweep_result;
    // Calibration
    float     cal_offset_dB;  // Added to meas_dBm for calibration
    // Battery
    float     batt_v;
    // Stepped attenuator (bitmask: 10|20|40 dB)
    uint8_t   att_db;
    float     fine_att_db;
    // Display
    bool      dirty;          // Screen needs redraw
} app;

// Frequency steps for encoder tuning (Hz)
const uint32_t STEP_TABLE[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000};
const int      STEP_COUNT   = 8;
int            step_idx     = 3;  // default 1 kHz steps

// ── Band table ───────────────────────────────────────────────────────────────
struct BandEntry {
    const char* name;
    uint32_t    f_lo;
    uint32_t    f_hi;
    uint32_t    f_default;
    SynthSrc    synth;
};

const BandEntry BANDS[] = {
    {"160M", 1800000,    2000000,   1900000,   SRC_DDS},
    {"80M",  3500000,    4000000,   3750000,   SRC_DDS},
    {"60M",  5330500,    5406400,   5368500,   SRC_DDS},
    {"40M",  7000000,    7300000,   7150000,   SRC_DDS},
    {"30M",  10100000,   10150000,  10125000,  SRC_DDS},
    {"20M",  14000000,   14350000,  14175000,  SRC_DDS},
    {"17M",  18068000,   18168000,  18118000,  SRC_DDS},
    {"15M",  21000000,   21450000,  21225000,  SRC_DDS},
    {"12M",  24890000,   24990000,  24940000,  SRC_DDS},
    {"10M",  28000000,   29700000,  28500000,  SRC_DDS},
    {"6M",   50000000,   54000000,  52000000,  SRC_SI5351},
    {"2M",   144000000,  148000000, 146000000, SRC_SI5351},
    {"1.25M",222000000,  225000000, 223500000, SRC_ADF4351},
    {"70cm", 420000000,  450000000, 432100000, SRC_ADF4351},
    {"33cm", 902000000,  928000000, 915000000, SRC_ADF4351},
    {"23cm", 1240000000, 1300000000,1296000000,SRC_ADF4351},
};
const int BAND_COUNT = sizeof(BANDS) / sizeof(BANDS[0]);
int       band_idx = 4;  // default 20M

// ── Encoder state machine ─────────────────────────────────────────────────────
volatile int  enc_delta = 0;
volatile bool enc_btn   = false;
int           enc_last_clk = HIGH;

void IRAM_ATTR encISR() {
    int clk = digitalRead(ENC_CLK);
    int dt  = digitalRead(ENC_DT);
    if (clk != enc_last_clk && clk == LOW)
        enc_delta += (dt != clk) ? 1 : -1;
    enc_last_clk = clk;
}

// ── Synthesizer abstraction ───────────────────────────────────────────────────
void setOutputFreq(uint32_t freq_hz) {
    selectLPF(freq_hz);  // Switch output filter
    switch (app.synth) {
    case SRC_DDS:
        dds.setFrequency((double)freq_hz);
        break;
    case SRC_SI5351:
        // Si5351 set_freq takes centi-Hz (Hz × 100)
        si5351.set_freq((uint64_t)freq_hz * 100ULL, SI5351_CLK0);
        break;
    case SRC_ADF4351:
        app.locked = adf4351.setFrequency(freq_hz);
        break;
    }
    app.freq_hz = freq_hz;
}

void rfEnable(bool en) {
    app.rf_on = en;
    if (!en) hmc1119SetAtten(31.75f);  // Full attenuation = off
    else     applyLevel();
}

void applyLevel() {
    // Stepped attenuation
    setAttenuatorStepped(app.att_db);
    // Fine attenuation
    hmc1119SetAtten(app.fine_att_db);
}

void selectSynth(SynthSrc src) {
    app.synth = src;
    // Power down inactive synthesizers
    dds.powerDown(src != SRC_DDS);
    adf4351.rfEnable(src == SRC_ADF4351);
    if (src != SRC_SI5351) {
        si5351.output_enable(SI5351_CLK0, 0);
    } else {
        si5351.output_enable(SI5351_CLK0, 1);
    }
}

// ── CYD DISPLAY — Drawing routines ───────────────────────────────────────────

// Colour palette
#define C_BG       TFT_BLACK
#define C_HEADER   0x1082   // dark blue-grey
#define C_FREQ     TFT_CYAN
#define C_LABEL    TFT_DARKGREY
#define C_VALUE    TFT_WHITE
#define C_GREEN    TFT_GREEN
#define C_YELLOW   TFT_YELLOW
#define C_RED      TFT_RED
#define C_ORANGE   0xFD20   // orange
#define C_TAB_ACT  0x02D5   // active tab blue
#define C_TAB_INACT 0x4208  // inactive tab grey

void drawTabBar() {
    const char* tabs[] = {"VFO","SWEEP","LEVEL","MOD","SET"};
    int tw = 64;
    for (int i = 0; i < 5; i++) {
        bool active = (i == (int)app.screen);
        tft.fillRect(i*tw, 0, tw-1, 20,
                     active ? C_TAB_ACT : C_TAB_INACT);
        tft.setTextColor(active ? TFT_WHITE : TFT_LIGHTGREY);
        tft.setTextDatum(MC_DATUM);
        tft.drawString(tabs[i], i*tw + tw/2, 10, 2);
    }
}

void drawMainScreen() {
    tft.fillRect(0, 21, 320, 219, C_BG);

    // Band and synth indicator
    tft.setTextColor(C_LABEL); tft.setTextDatum(TL_DATUM);
    tft.drawString(BANDS[band_idx].name, 4, 25, 2);
    const char* synth_str[] = {"DDS","Si5351","ADF4351"};
    tft.drawString(synth_str[app.synth], 80, 25, 2);

    // Lock indicator (ADF4351)
    if (app.synth == SRC_ADF4351) {
        tft.setTextColor(app.locked ? C_GREEN : C_RED);
        tft.drawString(app.locked ? "LOCK" : "UNLK", 180, 25, 2);
    }

    // RF ON/OFF
    tft.setTextColor(app.rf_on ? C_GREEN : C_RED);
    tft.fillRect(270, 22, 48, 18, app.rf_on ? 0x0200 : 0x4000);
    tft.drawString(app.rf_on ? " ON " : " OFF", 270, 25, 2);

    // Main frequency display (large)
    char freq_str[20];
    uint32_t f = app.freq_hz;
    if (f >= 1000000000UL)
        snprintf(freq_str, sizeof(freq_str), "%lu.%06lu GHz",
                 f/1000000000UL, (f%1000000000UL)/1000UL);
    else if (f >= 1000000UL)
        snprintf(freq_str, sizeof(freq_str), "%lu.%06lu MHz",
                 f/1000000UL, f%1000000UL);
    else
        snprintf(freq_str, sizeof(freq_str), "%lu.%03lu kHz",
                 f/1000UL, f%1000UL);

    tft.setTextColor(C_FREQ); tft.setTextDatum(MC_DATUM);
    tft.drawString(freq_str, 160, 80, 4);

    // Step indicator
    char step_str[16];
    uint32_t s = app.freq_step;
    if      (s >= 1000000) snprintf(step_str, 16, "Step: %lu MHz", s/1000000);
    else if (s >= 1000)    snprintf(step_str, 16, "Step: %lu kHz", s/1000);
    else                   snprintf(step_str, 16, "Step: %lu Hz", s);
    tft.setTextColor(C_LABEL); tft.setTextDatum(MC_DATUM);
    tft.drawString(step_str, 160, 110, 2);

    // Output level
    char lvl_str[24];
    snprintf(lvl_str, 24, "%.1f dBm", app.level_dBm);
    tft.setTextColor(C_VALUE);
    tft.drawString(lvl_str, 80, 140, 4);

    // Measured level (AD8307)
    snprintf(lvl_str, 24, "Meas: %.1f dBm", app.meas_dBm + app.cal_offset_dB);
    tft.setTextColor(C_YELLOW); tft.setTextDatum(TL_DATUM);
    tft.drawString(lvl_str, 10, 170, 2);

    // Modulation mode
    const char* mod_str[] = {"NONE","AM","FM","FM-PLL","CW"};
    tft.setTextColor(C_LABEL);
    tft.drawString("Mod:", 10, 195, 2);
    tft.setTextColor(C_VALUE);
    tft.drawString(mod_str[app.mod_mode], 60, 195, 2);

    // Battery
    char batt_str[16];
    snprintf(batt_str, 16, "%.2fV", app.batt_v);
    tft.setTextColor(app.batt_v > 3.5f ? C_GREEN : (app.batt_v > 3.2f ? C_YELLOW : C_RED));
    tft.drawString(batt_str, 245, 195, 2);
}

void drawSweepScreen() {
    tft.fillRect(0, 21, 320, 219, C_BG);

    SweepResult& r = app.sweep_result;
    if (r.n_pts == 0) {
        tft.setTextColor(C_LABEL); tft.setTextDatum(MC_DATUM);
        tft.drawString("No sweep data.  Press encoder.", 160, 120, 2);
        tft.setTextDatum(TL_DATUM);
        return;
    }

    // Plot area: x = 0..319, y = 40..219 (180 pixels height)
    const int PX = 10, PW = 300, PY = 45, PH = 160;
    const float DB_MIN = -90.0f, DB_MAX = 10.0f;

    tft.drawRect(PX, PY, PW, PH, TFT_DARKGREY);

    // Y-axis labels
    tft.setTextColor(C_LABEL); tft.setTextDatum(TR_DATUM);
    for (int db = -80; db <= 0; db += 20) {
        int y = PY + PH - (int)((db - DB_MIN) / (DB_MAX - DB_MIN) * PH);
        tft.drawFastHLine(PX, y, PW, 0x2104); // very dark gridline
        char s[8]; snprintf(s, 8, "%d", db);
        tft.drawString(s, PX-2, y-4, 1);
    }

    // Plot trace
    int prev_x = -1, prev_y = -1;
    for (uint16_t i = 0; i < r.n_pts; i++) {
        int x = PX + (int)((float)i / r.n_pts * PW);
        float db = r.pts[i].level_dBm;
        if (db < DB_MIN) db = DB_MIN;
        if (db > DB_MAX) db = DB_MAX;
        int y = PY + PH - (int)((db - DB_MIN) / (DB_MAX - DB_MIN) * PH);
        if (prev_x >= 0) tft.drawLine(prev_x, prev_y, x, y, C_GREEN);
        prev_x = x; prev_y = y;
    }

    // Annotations
    tft.setTextColor(C_YELLOW); tft.setTextDatum(TL_DATUM);
    char ann[48];
    snprintf(ann, 48, "Peak: %.1f dBm @ %.3f MHz",
             r.peak_dBm, r.peak_freq_hz / 1e6f);
    tft.drawString(ann, PX, 26, 1);

    float bw = (r.f_at_m3db_hi - r.f_at_m3db_lo) / 1000.0f;
    snprintf(ann, 48, "-3dB BW: %.1f kHz", bw);
    tft.drawString(ann, PX, 35, 1);
    tft.setTextDatum(TL_DATUM);
}

void drawLevelScreen() {
    tft.fillRect(0, 21, 320, 219, C_BG);

    tft.setTextColor(C_LABEL); tft.setTextDatum(TL_DATUM);
    tft.drawString("Output Level Control", 10, 28, 2);

    // Horizontal level bar
    float db = app.level_dBm;
    float frac = (db + 73.0f) / 86.0f;  // map -73..+13 to 0..1
    if (frac < 0) frac = 0; if (frac > 1) frac = 1;
    int barw = (int)(frac * 280);
    tft.fillRect(20, 55, 280, 20, 0x2104);
    tft.fillRect(20, 55, barw, 20, C_GREEN);
    char lvl_str[16];
    snprintf(lvl_str, 16, "%.1f dBm", db);
    tft.setTextColor(TFT_WHITE); tft.setTextDatum(MC_DATUM);
    tft.drawString(lvl_str, 160, 65, 4);

    // Stepped attenuator
    tft.setTextColor(C_LABEL); tft.setTextDatum(TL_DATUM);
    tft.drawString("Switched atten:", 10, 100, 2);
    char att_str[8]; snprintf(att_str, 8, "%d dB", app.att_db);
    tft.setTextColor(C_VALUE); tft.drawString(att_str, 160, 100, 2);

    // Fine attenuator
    tft.setTextColor(C_LABEL);
    tft.drawString("Fine atten:", 10, 125, 2);
    char fine_str[12]; snprintf(fine_str, 12, "%.2f dB", app.fine_att_db);
    tft.setTextColor(C_VALUE); tft.drawString(fine_str, 160, 125, 2);

    // Measured
    tft.setTextColor(C_YELLOW);
    tft.drawString("Measured:", 10, 150, 2);
    char meas_str[16]; snprintf(meas_str, 16, "%.1f dBm",
                                app.meas_dBm + app.cal_offset_dB);
    tft.setTextColor(C_VALUE); tft.drawString(meas_str, 160, 150, 2);

    // Amp bypass
    tft.setTextColor(C_LABEL); tft.drawString("Amplifier:", 10, 175, 2);
    bool amp_in = !(_relay_state & RELAY_AMPBYP);
    tft.setTextColor(amp_in ? C_GREEN : C_LABEL);
    tft.drawString(amp_in ? "IN" : "BYPASSED", 160, 175, 2);
    tft.setTextDatum(TL_DATUM);
}

void drawModScreen() {
    tft.fillRect(0, 21, 320, 219, C_BG);
    const char* modes[] = {"NONE","AM","FM-DDS","FM-PLL","CW"};
    tft.setTextColor(C_LABEL); tft.setTextDatum(TL_DATUM);
    tft.drawString("Modulation Mode:", 10, 28, 2);
    tft.setTextColor(C_FREQ); tft.drawString(modes[app.mod_mode], 200, 28, 2);

    tft.setTextColor(C_LABEL); tft.drawString("FM Dev:", 10, 70, 2);
    char dev_str[16]; snprintf(dev_str, 16, "%.0f Hz", app.fm_dev_hz);
    tft.setTextColor(C_VALUE); tft.drawString(dev_str, 160, 70, 2);

    tft.setTextColor(C_LABEL); tft.drawString("AM Depth:", 10, 100, 2);
    char am_str[8]; snprintf(am_str, 8, "%.0f%%", app.am_depth * 100.0f);
    tft.setTextColor(C_VALUE); tft.drawString(am_str, 160, 100, 2);

    tft.setTextColor(C_LABEL); tft.drawString("CW Rise:", 10, 130, 2);
    tft.setTextColor(C_VALUE); tft.drawString("5 ms", 160, 130, 2);

    tft.setTextColor(C_LABEL); tft.drawString("Key Input:", 10, 160, 2);
    bool key_down = (digitalRead(KEY_PIN) == LOW);
    tft.setTextColor(key_down ? C_RED : C_GREEN);
    tft.drawString(key_down ? "DOWN" : "UP", 160, 160, 2);
    tft.setTextDatum(TL_DATUM);
}

void drawSettingsScreen() {
    tft.fillRect(0, 21, 320, 219, C_BG);
    tft.setTextColor(C_LABEL); tft.setTextDatum(TL_DATUM);
    tft.drawString("Settings / Calibration", 10, 28, 2);

    tft.setTextColor(C_LABEL); tft.drawString("Cal offset:", 10, 60, 2);
    char cal_str[12]; snprintf(cal_str, 12, "%+.1f dB", app.cal_offset_dB);
    tft.setTextColor(C_VALUE); tft.drawString(cal_str, 160, 60, 2);

    tft.setTextColor(C_LABEL); tft.drawString("Reference:", 10, 90, 2);
    tft.setTextColor(C_VALUE); tft.drawString("25MHz TCXO + 10MHz", 160, 90, 2);

    tft.setTextColor(C_LABEL); tft.drawString("DDS SysCLK:", 10, 115, 2);
    tft.setTextColor(C_VALUE); tft.drawString("150 MHz", 160, 115, 2);

    // Sweep config display
    char sw_str[32];
    snprintf(sw_str, 32, "%lu – %lu MHz",
             app.sweep.f_start/1000000, app.sweep.f_stop/1000000);
    tft.setTextColor(C_LABEL); tft.drawString("Sweep range:", 10, 140, 2);
    tft.setTextColor(C_VALUE); tft.drawString(sw_str, 160, 140, 2);

    snprintf(sw_str, 32, "%u ms", app.sweep.dwell_ms);
    tft.setTextColor(C_LABEL); tft.drawString("Sweep dwell:", 10, 165, 2);
    tft.setTextColor(C_VALUE); tft.drawString(sw_str, 160, 165, 2);

    tft.setTextColor(C_LABEL); tft.drawString("Encoder:tap=band, hold=step", 10, 195, 1);
    tft.setTextDatum(TL_DATUM);
}

void redraw() {
    drawTabBar();
    switch (app.screen) {
    case SCR_MAIN:     drawMainScreen();     break;
    case SCR_SWEEP:    drawSweepScreen();    break;
    case SCR_LEVEL:    drawLevelScreen();    break;
    case SCR_MOD:      drawModScreen();      break;
    case SCR_SETTINGS: drawSettingsScreen(); break;
    default: break;
    }
    app.dirty = false;
}

// ── Touch handling ────────────────────────────────────────────────────────────
void handleTouch() {
    uint16_t tx, ty;
    if (!tft.getTouch(&tx, &ty)) return;

    // Tab bar: y < 20
    if (ty < 20) {
        int tab = tx / 64;
        if (tab < 5) {
            app.screen = (Screen)tab;
            app.dirty = true;
        }
        return;
    }

    // Screen-specific touch
    if (app.screen == SCR_MAIN) {
        // Toggle RF ON/OFF (right side)
        if (tx > 270 && ty < 45) { rfEnable(!app.rf_on); app.dirty = true; }
        // Frequency area: increment/decrement based on left/right half
        if (ty > 55 && ty < 105) {
            if (tx < 160) app.freq_hz -= app.freq_step;
            else          app.freq_hz += app.freq_step;
            setOutputFreq(app.freq_hz);
            app.dirty = true;
        }
        // Band touch: cycle bands (bottom bar)
        if (ty > 210) {
            band_idx = (band_idx + 1) % BAND_COUNT;
            app.freq_hz = BANDS[band_idx].f_default;
            selectSynth(BANDS[band_idx].synth);
            setOutputFreq(app.freq_hz);
            app.dirty = true;
        }
    } else if (app.screen == SCR_SWEEP) {
        // Center tap = run sweep
        if (ty > 100 && ty < 180) {
            app.sweep.stream_serial = true;
            runSweep(app.sweep, &app.sweep_result, setOutputFreq);
            app.dirty = true;
        }
    } else if (app.screen == SCR_LEVEL) {
        // Level bar: map tap X to dBm
        if (ty > 50 && ty < 80) {
            float frac = (tx - 20.0f) / 280.0f;
            app.level_dBm = -73.0f + frac * 86.0f;
            // Translate to attenuator settings
            // Simple: use fine attenuator for full range if att_db=0
            float fine = -app.level_dBm + 13.0f;  // invert: +13=0 fine, -73=86 fine
            if (fine > 31.75f) { app.att_db = 40; fine -= 40; }
            if (fine > 31.75f) { app.att_db += 20; fine -= 20; }
            if (fine > 31.75f) { app.att_db += 10; fine -= 10; }
            app.fine_att_db = fine < 0 ? 0 : fine;
            applyLevel();
            app.dirty = true;
        }
    } else if (app.screen == SCR_MOD) {
        // Mode cycle on top half touch
        if (ty < 100) {
            app.mod_mode = (ModMode)((app.mod_mode + 1) % 5);
            app.dirty = true;
        }
    }
}

// ── Encoder handling ──────────────────────────────────────────────────────────
void handleEncoder() {
    int delta = 0;
    noInterrupts(); delta = enc_delta; enc_delta = 0; interrupts();
    if (delta == 0) return;

    if (app.screen == SCR_MAIN) {
        // Tuning: adjust frequency by step × delta
        int32_t change = (int32_t)app.freq_step * delta;
        if (change < 0 && (uint32_t)(-change) > app.freq_hz)
            app.freq_hz = BANDS[band_idx].f_lo;
        else
            app.freq_hz += change;
        // Clamp to band
        if (app.freq_hz < BANDS[band_idx].f_lo) app.freq_hz = BANDS[band_idx].f_lo;
        if (app.freq_hz > BANDS[band_idx].f_hi) app.freq_hz = BANDS[band_idx].f_hi;
        setOutputFreq(app.freq_hz);
    } else if (app.screen == SCR_LEVEL) {
        app.fine_att_db -= delta * 0.25f;
        if (app.fine_att_db < 0) app.fine_att_db = 0;
        if (app.fine_att_db > 31.75f) app.fine_att_db = 31.75f;
        hmc1119SetAtten(app.fine_att_db);
        app.level_dBm = 13.0f - app.att_db - app.fine_att_db;
    } else if (app.screen == SCR_SETTINGS) {
        app.cal_offset_dB += delta * 0.1f;
    } else if (app.screen == SCR_MOD) {
        if (app.mod_mode == MOD_FM_DDS || app.mod_mode == MOD_FM_PLL)
            app.fm_dev_hz += delta * 100.0f;
        else if (app.mod_mode == MOD_AM)
            app.am_depth += delta * 0.05f;
        app.am_depth = constrain(app.am_depth, 0, 1.0f);
        app.fm_dev_hz = constrain(app.fm_dev_hz, 100, 75000);
    }
    app.dirty = true;
}

void handleEncoderButton() {
    if (!enc_btn) return;
    enc_btn = false;

    if (app.screen == SCR_MAIN) {
        // Button cycles tuning step
        step_idx = (step_idx + 1) % STEP_COUNT;
        app.freq_step = STEP_TABLE[step_idx];
    } else if (app.screen == SCR_SWEEP) {
        // Button triggers sweep
        app.sweep.stream_serial = true;
        runSweep(app.sweep, &app.sweep_result, setOutputFreq);
        // Restore to original frequency
        setOutputFreq(app.freq_hz);
    }
    app.dirty = true;
}

// ── CW key ISR ────────────────────────────────────────────────────────────────
volatile bool _key_down = false;
volatile bool _key_changed = false;

void IRAM_ATTR keyISR() {
    _key_down = (digitalRead(KEY_PIN) == LOW);
    _key_changed = true;
}

// ── Slow update tasks (500 ms interval) ─────────────────────────────────────
void slowUpdate() {
    // Read AD8307 (16 samples)
    app.meas_dBm = readPower_dBm(16);

    // Battery voltage
    int raw = analogRead(BATT_PIN);
    app.batt_v = raw * (3.3f / 4095.0f) * BATT_DIV;

    // ADF4351 lock check
    if (app.synth == SRC_ADF4351)
        app.locked = adf4351.isLocked();

    app.dirty = true;
}

// ── Save / Load settings ─────────────────────────────────────────────────────
void saveSettings() {
    prefs.putUInt("freq", app.freq_hz);
    prefs.putUChar("band", band_idx);
    prefs.putUChar("synth", app.synth);
    prefs.putUChar("step_idx", step_idx);
    prefs.putFloat("level", app.level_dBm);
    prefs.putUChar("att_db", app.att_db);
    prefs.putFloat("fine_att", app.fine_att_db);
    prefs.putFloat("cal_off", app.cal_offset_dB);
    prefs.putUChar("mod", app.mod_mode);
    prefs.putFloat("fm_dev", app.fm_dev_hz);
    prefs.putFloat("am_depth", app.am_depth);
}

void loadSettings() {
    app.freq_hz       = prefs.getUInt("freq",    14175000);
    band_idx          = prefs.getUChar("band",   6);
    app.synth         = (SynthSrc)prefs.getUChar("synth", SRC_DDS);
    step_idx          = prefs.getUChar("step_idx", 3);
    app.freq_step     = STEP_TABLE[step_idx];
    app.level_dBm     = prefs.getFloat("level",  -20.0f);
    app.att_db        = prefs.getUChar("att_db", 30);
    app.fine_att_db   = prefs.getFloat("fine_att", 3.0f);
    app.cal_offset_dB = prefs.getFloat("cal_off", 0.0f);
    app.mod_mode      = (ModMode)prefs.getUChar("mod", MOD_NONE);
    app.fm_dev_hz     = prefs.getFloat("fm_dev",  2500.0f);
    app.am_depth      = prefs.getFloat("am_depth", 0.5f);
}

// ── Setup ─────────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);

    // I2C
    Wire.begin(21, 22);

    // SPI (synthesizers use default VSPI: CLK=18, MOSI=23)
    SPI.begin(18, 19, 23, -1);  // CLK, MISO, MOSI, SS (SS managed per-device)

    // TFT (TFT_eSPI configured in User_Setup.h for CYD)
    tft.init();
    tft.setRotation(1);  // Landscape
    tft.fillScreen(C_BG);

    // Synthesizer init
    dds.begin();
    si5351.init(SI5351_CRYSTAL_LOAD_8PF, 25000000, 0);
    si5351.set_correction(0, SI5351_PLL_INPUT_XO);  // calibrate via settings
    si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA);
    si5351.output_enable(SI5351_CLK0, 0);  // start disabled
    adf4351.begin();

    // HMC1119
    pinMode(HMC1119_CS_PIN, OUTPUT);
    digitalWrite(HMC1119_CS_PIN, HIGH);
    hmc1119SetAtten(31.75f);  // full attenuation at startup

    // Encoder
    pinMode(ENC_CLK, INPUT_PULLUP);
    pinMode(ENC_DT,  INPUT_PULLUP);
    pinMode(ENC_SW,  INPUT_PULLUP);
    enc_last_clk = digitalRead(ENC_CLK);
    attachInterrupt(ENC_CLK, encISR, CHANGE);
    attachInterrupt(ENC_SW, [](){enc_btn=true;}, FALLING);

    // CW Key
    pinMode(KEY_PIN, INPUT_PULLUP);
    attachInterrupt(KEY_PIN, keyISR, CHANGE);

    // Settings
    prefs.begin("siggen", false);
    loadSettings();

    // Apply initial state
    app.screen  = SCR_MAIN;
    app.rf_on   = false;
    app.locked  = false;
    app.dirty   = true;

    // Default sweep range: 1–30 MHz
    app.sweep.f_start = 1000000;
    app.sweep.f_stop  = 30000000;
    app.sweep.f_step  = 0;  // auto
    app.sweep.dwell_ms = 10;
    app.sweep.stream_serial = false;

    selectSynth(BANDS[band_idx].synth);
    setOutputFreq(app.freq_hz);
    applyLevel();

    Serial.println(F("TM-SG-001 Signal Generator ready"));
    Serial.println(F("Type 'help' for commands"));
}

// ── Serial command handler ────────────────────────────────────────────────────
void handleSerial() {
    if (!Serial.available()) return;
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "help") {
        Serial.println(F("Commands:"));
        Serial.println(F("  freq <Hz>      — set output frequency"));
        Serial.println(F("  level <dBm>    — set output level"));
        Serial.println(F("  rf on|off      — enable/disable RF output"));
        Serial.println(F("  sweep <f1> <f2> <step> <dwell_ms>  — run sweep"));
        Serial.println(F("  cal <dB>       — set calibration offset"));
        Serial.println(F("  save           — save settings to NVS"));
        Serial.println(F("  status         — print current status"));
    } else if (cmd.startsWith("freq ")) {
        uint32_t f = (uint32_t)cmd.substring(5).toInt();
        if (f > 0) { app.freq_hz = f; setOutputFreq(f); app.dirty = true; }
    } else if (cmd.startsWith("level ")) {
        float db = cmd.substring(6).toFloat();
        app.level_dBm = db;
        // Simple mapping: all attenuation on fine+stepped
        float total_atten = 13.0f - db;
        app.att_db = 0;
        if (total_atten >= 40) { app.att_db += 40; total_atten -= 40; }
        if (total_atten >= 20) { app.att_db += 20; total_atten -= 20; }
        if (total_atten >= 10) { app.att_db += 10; total_atten -= 10; }
        app.fine_att_db = constrain(total_atten, 0, 31.75f);
        applyLevel(); app.dirty = true;
    } else if (cmd == "rf on")  { rfEnable(true);  app.dirty = true; }
      else if (cmd == "rf off") { rfEnable(false); app.dirty = true; }
      else if (cmd.startsWith("sweep ")) {
        uint32_t f1, f2, step; uint16_t dwell;
        sscanf(cmd.c_str() + 6, "%lu %lu %lu %hu", &f1, &f2, &step, &dwell);
        app.sweep.f_start = f1; app.sweep.f_stop = f2;
        app.sweep.f_step = step; app.sweep.dwell_ms = dwell;
        app.sweep.stream_serial = true;
        runSweep(app.sweep, &app.sweep_result, setOutputFreq);
        setOutputFreq(app.freq_hz);
        app.dirty = true;
    } else if (cmd.startsWith("cal ")) {
        app.cal_offset_dB = cmd.substring(4).toFloat();
        app.dirty = true;
    } else if (cmd == "save") {
        saveSettings();
        Serial.println(F("Settings saved."));
    } else if (cmd == "status") {
        Serial.printf("Freq: %lu Hz\n", app.freq_hz);
        Serial.printf("Level (target): %.1f dBm\n", app.level_dBm);
        Serial.printf("Level (measured): %.1f dBm\n", app.meas_dBm + app.cal_offset_dB);
        Serial.printf("Batt: %.2f V\n", app.batt_v);
        Serial.printf("RF: %s\n", app.rf_on ? "ON" : "OFF");
        if (app.synth == SRC_ADF4351)
            Serial.printf("PLL lock: %s\n", app.locked ? "YES" : "NO");
    }
}

// ── Main loop ─────────────────────────────────────────────────────────────────
static uint32_t t_slow = 0;
static uint32_t t_save = 0;

void loop() {
    // Encoder and touch
    handleEncoder();
    handleEncoderButton();
    handleTouch();

    // CW key state change
    if (_key_changed) {
        _key_changed = false;
        if (app.mod_mode == MOD_CW && app.rf_on)
            cwKey(_key_down, 5);
    }

    // AM modulation at audio rate (if active)
    if (app.mod_mode == MOD_AM && app.rf_on) {
        int sample = analogRead(AF_ADC_PIN);
        doAM(app.fine_att_db, sample, app.am_depth);
    }

    // FM modulation via DDS at audio rate
    if (app.mod_mode == MOD_FM_DDS && app.rf_on && app.synth == SRC_DDS) {
        int sample = analogRead(AF_ADC_PIN);
        int offset = (int)(((sample - 2048.0f) / 2048.0f) * app.fm_dev_hz);
        dds.setOffset(offset);
    }

    // Slow updates: power monitor, battery, lock detect
    if (millis() - t_slow >= 500) {
        t_slow = millis();
        slowUpdate();
    }

    // Autosave every 60 seconds
    if (millis() - t_save >= 60000) {
        t_save = millis();
        saveSettings();
    }

    // Redraw if dirty
    if (app.dirty) redraw();

    // Serial commands
    handleSerial();

    delay(5);  // yield to WiFi stack (not used here but good practice)
}
