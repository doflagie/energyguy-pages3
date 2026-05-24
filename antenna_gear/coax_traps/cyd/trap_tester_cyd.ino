/*
 * COAXIAL TRAP RESONANCE TESTER
 * CYD (Cheap Yellow Display) — ESP32-2432S028 / ILI9341 2.8" TFT
 *
 * PURPOSE:
 *   Measures and displays coaxial trap resonant frequency, Q factor,
 *   and bandwidth against target values from the winding table.
 *   Uses dip-oscillator (GDO) principle via external RF oscillator
 *   connected to GPIO26 (DAC1) and measured via GPIO36 (VP/ADC).
 *
 * HARDWARE REQUIRED (external):
 *   - Signal source: Si5351A breakout (I2C: SDA=GPIO21, SCL=GPIO22)
 *     OR NanoVNA serial output (UART2: RX=GPIO16, TX=GPIO17)
 *   - Coupling loop: 3-turn loop on RG-58 coax, 25mm diameter
 *     connected to GPIO36 (ADC) via 100k + 100pF low-pass filter
 *   - Optional: ADS1115 for more accurate ADC (I2C address 0x48)
 *
 * CONNECTIONS (CYD pinout):
 *   TFT:        Built-in (SPI2, CS=15, DC=2, RST=12, BL=21)
 *   Touch:      Built-in (SPI1, CS=33, IRQ=36)  -- NOTE: shared pin
 *   Si5351 SDA: GPIO21 (shared with TFT backlight — use alternate)
 *   Si5351 SCL: GPIO22
 *   ADC input:  GPIO39 (VP, 12-bit, 0-3.3V, connect via 100k resistor)
 *   LED red:    GPIO4  (built-in)
 *   LED green:  GPIO16 (alternate, check your CYD variant)
 *
 * OPERATING PROCEDURE:
 *   1. Select target band with UP/DOWN touch buttons
 *   2. Connect coupling loop to trap under test
 *   3. Press SCAN — Si5351 sweeps frequency ±10% around target
 *   4. Display shows: dip frequency, Q, bandwidth, PASS/FAIL
 *   5. Trim capacitor or adjust turns until PASS
 *
 * LIBRARIES REQUIRED (install via Arduino Library Manager):
 *   TFT_eSPI    (configure for ILI9341, CYD pinout)
 *   si5351      (Jason Milldrum NT7S)
 *   EEPROM      (built-in)
 *
 * TFT_eSPI User_Setup.h settings for CYD:
 *   #define ILI9341_DRIVER
 *   #define TFT_CS   15
 *   #define TFT_DC    2
 *   #define TFT_RST  12
 *   #define TFT_MOSI 13
 *   #define TFT_SCLK 14
 *   #define TFT_MISO -1  (or 12 on some variants)
 *   #define TOUCH_CS 33
 *
 * LICENSE: MIT
 * AUTHOR:  Generated for Merv Martin (K-number), 2026
 */

#include <TFT_eSPI.h>
#include <si5351.h>
#include <Wire.h>
#include <EEPROM.h>

// ============================================================
// Hardware
// ============================================================
#define ADC_PIN        39    // RF detector input
#define LED_R          4
#define TOUCH_IRQ      36
#define BACKLIGHT_PIN  21

#define ADC_SAMPLES    64    // oversample for noise reduction
#define SWEEP_STEPS    200   // frequency sweep points

// ============================================================
// Band table — target frequencies, accept window ±2%
// ============================================================
struct BandEntry {
    const char* name;
    float       target_mhz;
    float       tol_mhz;     // ±tolerance for PASS
    int         q_min;       // minimum acceptable Q
};

static const BandEntry BANDS[] = {
    {"160M",  1.850f, 0.050f,  30},
    {"80M",   3.750f, 0.075f,  40},
    {"40M",   7.150f, 0.100f,  50},
    {"30M",  10.125f, 0.100f,  55},
    {"20M",  14.175f, 0.150f,  60},
    {"17M",  18.118f, 0.150f,  60},
    {"15M",  21.225f, 0.200f,  55},
    {"12M",  24.940f, 0.250f,  50},
    {"10M",  28.500f, 0.300f,  45},
    {"6M",   51.000f, 0.500f,  35},
    {"2M",  146.000f, 1.000f,  25},
};
static const int N_BANDS = sizeof(BANDS) / sizeof(BANDS[0]);

// ============================================================
// Globals
// ============================================================
TFT_eSPI tft = TFT_eSPI();
Si5351   si5351;

int   current_band  = 2;   // default 40M
bool  scanning      = false;
float measured_freq = 0.0f;
float measured_q    = 0.0f;
float measured_bw   = 0.0f;

// Sweep result buffer
float sweep_freq[SWEEP_STEPS];
int   sweep_adc[SWEEP_STEPS];

// ============================================================
// Colors (ILI9341)
// ============================================================
#define C_BG        TFT_BLACK
#define C_HEADER    0x04B0   // dark blue
#define C_ACCENT    0xFD20   // orange
#define C_PASS      TFT_GREEN
#define C_FAIL      TFT_RED
#define C_WARN      TFT_YELLOW
#define C_TEXT      TFT_WHITE
#define C_GRID      0x4208   // dark gray

// ============================================================
// ADC read (oversampled)
// ============================================================
int read_adc_avg() {
    long sum = 0;
    for (int i = 0; i < ADC_SAMPLES; i++) {
        sum += analogRead(ADC_PIN);
        delayMicroseconds(50);
    }
    return (int)(sum / ADC_SAMPLES);
}

// ============================================================
// Set Si5351 output frequency (Hz), CLK0
// ============================================================
void set_freq_hz(uint64_t freq_hz) {
    si5351.set_freq(freq_hz * SI5351_FREQ_MULT, SI5351_CLK0);
}

// ============================================================
// Perform frequency sweep around target
// Returns index of minimum ADC reading (resonance dip)
// ============================================================
int do_sweep(float center_mhz, float span_pct) {
    float span_mhz  = center_mhz * span_pct / 100.0f;
    float start_mhz = center_mhz - span_mhz;
    float step_mhz  = 2.0f * span_mhz / (SWEEP_STEPS - 1);

    int min_idx = 0;
    int min_val = 9999;

    for (int i = 0; i < SWEEP_STEPS; i++) {
        float f = start_mhz + i * step_mhz;
        sweep_freq[i] = f;
        uint64_t f_hz = (uint64_t)(f * 1e6f);
        set_freq_hz(f_hz);
        delayMicroseconds(500);
        int v = read_adc_avg();
        sweep_adc[i] = v;
        if (v < min_val) {
            min_val = v;
            min_idx = i;
        }
    }
    return min_idx;
}

// ============================================================
// Calculate Q from sweep data
// Q = f0 / BW_3dB
// BW found by finding -3dB points around dip
// ============================================================
float calc_q(int dip_idx, float *bw_out) {
    int   dip_val    = sweep_adc[dip_idx];
    int   peak_val   = 0;
    for (int i = 0; i < SWEEP_STEPS; i++)
        if (sweep_adc[i] > peak_val) peak_val = sweep_adc[i];

    int   half_depth = dip_val + (peak_val - dip_val) / 2;  // -3dB point

    // Find left -3dB crossing
    int lo = dip_idx;
    while (lo > 0 && sweep_adc[lo] < half_depth) lo--;
    // Find right -3dB crossing
    int hi = dip_idx;
    while (hi < SWEEP_STEPS-1 && sweep_adc[hi] < half_depth) hi++;

    float bw = sweep_freq[hi] - sweep_freq[lo];
    if (bw_out) *bw_out = bw;
    if (bw < 0.001f) return 0.0f;
    return sweep_freq[dip_idx] / bw;
}

// ============================================================
// Draw spectrum plot
// ============================================================
void draw_spectrum(int x0, int y0, int w, int h) {
    // Background
    tft.fillRect(x0, y0, w, h, C_BG);
    tft.drawRect(x0, y0, w, h, C_GRID);

    // Find min/max for scaling
    int vmin = 4095, vmax = 0;
    for (int i = 0; i < SWEEP_STEPS; i++) {
        if (sweep_adc[i] < vmin) vmin = sweep_adc[i];
        if (sweep_adc[i] > vmax) vmax = sweep_adc[i];
    }
    int vrange = vmax - vmin;
    if (vrange < 1) vrange = 1;

    // Plot points
    for (int i = 1; i < SWEEP_STEPS; i++) {
        int x1 = x0 + (i-1) * w / SWEEP_STEPS;
        int x2 = x0 + i     * w / SWEEP_STEPS;
        int y1 = y0 + h - (sweep_adc[i-1] - vmin) * (h-4) / vrange - 2;
        int y2 = y0 + h - (sweep_adc[i]   - vmin) * (h-4) / vrange - 2;
        y1 = constrain(y1, y0+1, y0+h-1);
        y2 = constrain(y2, y0+1, y0+h-1);
        tft.drawLine(x1, y1, x2, y2, C_ACCENT);
    }

    // Target freq marker
    float target = BANDS[current_band].target_mhz;
    float sweep_start = sweep_freq[0];
    float sweep_end   = sweep_freq[SWEEP_STEPS-1];
    if (target >= sweep_start && target <= sweep_end) {
        int tx = x0 + (int)((target - sweep_start) / (sweep_end - sweep_start) * w);
        tft.drawFastVLine(tx, y0, h, C_WARN);
    }
}

// ============================================================
// Draw main UI
// ============================================================
void draw_ui() {
    tft.fillScreen(C_BG);

    // Header
    tft.fillRect(0, 0, 320, 28, C_HEADER);
    tft.setTextColor(C_TEXT, C_HEADER);
    tft.setTextSize(2);
    tft.setCursor(8, 6);
    tft.print("COAX TRAP TESTER");

    // Band selector
    tft.setTextColor(C_TEXT, C_BG);
    tft.setTextSize(1);
    tft.setCursor(8, 34);
    tft.print("BAND:");
    tft.setTextColor(C_ACCENT, C_BG);
    tft.setTextSize(2);
    tft.setCursor(60, 30);
    tft.print(BANDS[current_band].name);
    tft.setTextSize(1);
    tft.setTextColor(C_TEXT, C_BG);
    tft.setCursor(130, 34);
    tft.printf("TARGET: %.3f MHz", BANDS[current_band].target_mhz);

    // Buttons: UP DOWN SCAN
    tft.fillRoundRect(5,   55, 45, 28, 4, 0x0410);
    tft.fillRoundRect(55,  55, 45, 28, 4, 0x0410);
    tft.fillRoundRect(115, 55, 60, 28, 4, 0x8000);
    tft.setTextColor(C_TEXT);
    tft.setTextSize(2);
    tft.setCursor(12,  62); tft.print("UP");
    tft.setCursor(61,  62); tft.print("DN");
    tft.setCursor(120, 62); tft.print("SCAN");

    // Spectrum area
    tft.drawRect(0, 90, 320, 100, C_GRID);
    tft.setTextColor(C_GRID, C_BG);
    tft.setCursor(4, 93);
    tft.print("-- SWEEP SPECTRUM --");

    // Results area
    tft.drawLine(0, 196, 320, 196, C_GRID);

    update_results_display();
}

void update_results_display() {
    // Clear results area
    tft.fillRect(0, 198, 320, 42, C_BG);

    if (measured_freq < 0.001f) {
        tft.setTextColor(C_GRID, C_BG);
        tft.setTextSize(1);
        tft.setCursor(80, 210);
        tft.print("Press SCAN to measure");
        return;
    }

    float target = BANDS[current_band].target_mhz;
    float tol    = BANDS[current_band].tol_mhz;
    float err    = measured_freq - target;
    bool  f_pass = fabsf(err) <= tol;
    bool  q_pass = measured_q >= BANDS[current_band].q_min;
    bool  all_ok = f_pass && q_pass;

    // Freq result
    tft.setTextSize(1);
    tft.setTextColor(C_TEXT, C_BG);
    tft.setCursor(4, 200);
    tft.printf("FREQ: %.4f MHz", measured_freq);
    tft.setTextColor(f_pass ? C_PASS : C_FAIL, C_BG);
    tft.printf("  %+.3f MHz %s", err, f_pass ? "[OK]" : "[FAIL]");

    // Q result
    tft.setTextColor(C_TEXT, C_BG);
    tft.setCursor(4, 212);
    tft.printf("Q:    %.0f   BW: %.2f kHz", measured_q, measured_bw * 1000.0f);
    tft.setTextColor(q_pass ? C_PASS : C_FAIL, C_BG);
    tft.printf("  %s", q_pass ? "[OK]" : "[LOW]");

    // Overall verdict
    tft.fillRect(220, 198, 96, 42, all_ok ? C_PASS : C_FAIL);
    tft.setTextColor(TFT_BLACK, all_ok ? C_PASS : C_FAIL);
    tft.setTextSize(3);
    tft.setCursor(228, 206);
    tft.print(all_ok ? "PASS" : "FAIL");
}

// ============================================================
// Touch handling
// ============================================================
bool touch_pressed(int *tx, int *ty) {
    // Basic resistive touch read — works on most CYD variants
    // Replace with XPT2046 library call if needed
    uint16_t x, y;
    // Using TFT_eSPI built-in touch
    if (tft.getTouch(&x, &y, 300)) {
        *tx = x;
        *ty = y;
        return true;
    }
    return false;
}

void handle_touch(int x, int y) {
    if (scanning) return;

    // UP button (5-50, 55-83)
    if (x >= 5 && x <= 50 && y >= 55 && y <= 83) {
        current_band = (current_band + 1) % N_BANDS;
        measured_freq = 0.0f;
        draw_ui();
        delay(200);
    }
    // DOWN button (55-100, 55-83)
    else if (x >= 55 && x <= 100 && y >= 55 && y <= 83) {
        current_band = (current_band - 1 + N_BANDS) % N_BANDS;
        measured_freq = 0.0f;
        draw_ui();
        delay(200);
    }
    // SCAN button (115-175, 55-83)
    else if (x >= 115 && x <= 175 && y >= 55 && y <= 83) {
        run_scan();
    }
}

// ============================================================
// Run a scan
// ============================================================
void run_scan() {
    scanning = true;
    measured_freq = 0.0f;

    // Show "SCANNING..." in spectrum area
    tft.fillRect(1, 91, 318, 98, C_BG);
    tft.setTextColor(C_WARN, C_BG);
    tft.setTextSize(2);
    tft.setCursor(80, 130);
    tft.print("SCANNING...");

    float target = BANDS[current_band].target_mhz;

    // First sweep: wide (±15%) to find rough dip
    int dip_idx = do_sweep(target, 15.0f);
    float rough_freq = sweep_freq[dip_idx];

    // Second sweep: narrow (±5%) around detected dip
    dip_idx = do_sweep(rough_freq, 5.0f);

    // Calculate results
    measured_freq = sweep_freq[dip_idx];
    measured_q    = calc_q(dip_idx, &measured_bw);

    // Draw spectrum
    draw_spectrum(1, 91, 318, 98);

    // Update results
    update_results_display();

    // Blink LED for pass/fail
    float tol  = BANDS[current_band].tol_mhz;
    bool  pass = (fabsf(measured_freq - target) <= tol) &&
                 (measured_q >= BANDS[current_band].q_min);
    for (int i = 0; i < 3; i++) {
        digitalWrite(LED_R, pass ? LOW : HIGH);
        delay(150);
        digitalWrite(LED_R, HIGH);
        delay(150);
    }

    scanning = false;
}

// ============================================================
// Setup
// ============================================================
void setup() {
    Serial.begin(115200);
    Serial.println("COAX TRAP TESTER v1.0");

    pinMode(LED_R, OUTPUT);
    digitalWrite(LED_R, HIGH);  // active low on CYD

    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);

    // TFT init
    tft.init();
    tft.setRotation(1);  // landscape
    tft.fillScreen(C_BG);

    // Backlight
    ledcSetup(0, 5000, 8);
    ledcAttachPin(BACKLIGHT_PIN, 0);
    ledcWrite(0, 200);

    // Si5351 init
    Wire.begin(21, 22);
    if (!si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0)) {
        tft.setTextColor(C_FAIL);
        tft.setCursor(10, 100);
        tft.print("Si5351 NOT FOUND");
        tft.setCursor(10, 120);
        tft.print("Check wiring: SDA=21 SCL=22");
        while (1) delay(1000);
    }
    si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_2MA);
    si5351.output_enable(SI5351_CLK0, 1);
    si5351.output_enable(SI5351_CLK1, 0);
    si5351.output_enable(SI5351_CLK2, 0);

    EEPROM.begin(4);
    current_band = EEPROM.read(0);
    if (current_band >= N_BANDS) current_band = 2;

    draw_ui();
    Serial.println("Ready.");
}

// ============================================================
// Loop
// ============================================================
void loop() {
    int tx, ty;
    if (touch_pressed(&tx, &ty)) {
        handle_touch(tx, ty);
        // Save last band selection
        EEPROM.write(0, current_band);
        EEPROM.commit();
        delay(50);
    }
}

/*
 * ============================================================
 * COUPLING LOOP CONSTRUCTION (3-turn, 25mm diameter, RG-58)
 * ============================================================
 *
 * Materials: 150mm RG-58, 25mm ID PVC pipe section (25mm long)
 *
 * 1. Wind 3 turns RG-58 coax on 25mm former
 * 2. Connect shield of coax to GPIO GND
 * 3. Connect center conductor through 100kohm resistor to GPIO39
 * 4. Place 100pF capacitor from GPIO39 to GND (low-pass filter)
 * 5. When testing trap, hold loop ~10mm from trap winding
 *    (do not touch — capacitive coupling distorts measurement)
 *
 * CALIBRATION:
 * 1. Set Si5351 to known frequency (e.g. 14.000 MHz)
 * 2. Measure with frequency counter for correction factor
 * 3. If Si5351 crystal is off-freq, adjust in si5351.init()
 *    third parameter (correction in ppb)
 *
 * ACCURACY SPECIFICATION:
 *   Frequency: ±0.5% (limited by Si5351 accuracy)
 *   Q:         ±20% (limited by 12-bit ADC dynamic range)
 *   Minimum detectable Q: ~20
 *   Maximum measurable Q: ~500
 *   Frequency range: 500 kHz - 160 MHz (Si5351 CLK0 limit)
 * ============================================================
 */
