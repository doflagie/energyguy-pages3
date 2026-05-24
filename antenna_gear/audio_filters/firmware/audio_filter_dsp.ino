/*
 * audio_filter_dsp.ino
 * CW / SSB DSP Audio Filter — ESP32
 * TM-AF-001 Rev A
 *
 * Hardware:
 *   ESP32-WROOM-32 @ 240 MHz
 *   PCM5102A I2S DAC   — I2S0: BCK=GPIO26, WS=GPIO27, DATA=GPIO25
 *   INMP441 I2S mic    — I2S1: BCK=GPIO14, WS=GPIO15, DATA=GPIO12
 *   WM8978 codec (alt) — I2C: SDA=GPIO21, SCL=GPIO22
 *   Built-in ADC (alt) — GPIO34 (simple single-supply option)
 *   Rotary encoder     — CLK=GPIO32, DT=GPIO33, SW=GPIO4
 *   Status LED         — GPIO2 (470Ω series)
 *   OLED (SSD1306)     — I2C 0x3C
 *   Bypass relay       — GPIO5 → 2N3904 → relay coil
 *
 * Filter modes (selectable via encoder):
 *   0: SSB  300–3000 Hz (127-tap FIR)
 *   1: CW   700 Hz ±250 Hz (127-tap FIR)
 *   2: CW   500 Hz ±150 Hz (127-tap FIR)
 *   3: CW   400 Hz ±100 Hz (127-tap FIR)
 *   4: NOTCH 800 Hz (63-tap FIR)
 *   5: IIR CW biquad 700 Hz (low latency, 2 samples)
 *   6: IIR NOTCH 800 Hz (low latency, 2 samples)
 *   7: BYPASS (direct pass-through)
 *   8: LMS AUTO-NOTCH (adaptive, tracks interferer)
 *
 * Processing pipeline:
 *   ADC → block_in[32] → HP prefilter → main filter → AGC → DAC
 *
 * Latency:
 *   FIR:   block_size/Fs + (N-1)/(2×Fs) = 4 ms + 7.9 ms = ~12 ms (127-tap)
 *   IIR:   block_size/Fs + 2/Fs = 4 ms + 0.25 ms = ~4 ms
 *   bypass: 4 ms (block buffering only)
 */

#include <Arduino.h>
#include <driver/i2s.h>
#include <Wire.h>
#include <Preferences.h>
#include "fir_coefficients.h"

// ── Optional display ──────────────────────────────────────────────────────────
#ifdef USE_OLED
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 oled(128, 64, &Wire, -1);
#endif

// ── I/O config ────────────────────────────────────────────────────────────────
#define I2S_DAC_BCK   26
#define I2S_DAC_WS    27
#define I2S_DAC_DATA  25
#define I2S_ADC_BCK   14
#define I2S_ADC_WS    15
#define I2S_ADC_DATA  12

#define ADC_BUILTIN_PIN   34    // fallback: built-in ADC input
#define ENC_CLK           32
#define ENC_DT            33
#define ENC_SW             4
#define LED_PIN            2
#define BYPASS_RELAY_PIN   5

#define SAMPLE_RATE    8000
#define BLOCK_SIZE       32     // samples per processing block
#define I2S_BUF_LEN      64    // I2S DMA buffer (2× BLOCK_SIZE)

// ── State ─────────────────────────────────────────────────────────────────────
static FilterMode  g_mode        = FILT_CW_700;
static bool        g_bypass      = false;
static float       g_agc_gain    = 1.0f;
static float       g_agc_target  = 0.5f;    // target RMS (0.5 = half full-scale)
static float       g_lms_mu      = 0.005f;  // LMS step size

// FIR delay line (longest filter = 127 taps)
static float fir_delay[FIR_LEN_SSB] = {0};
static int   fir_idx = 0;

// IIR biquad state variables (two sections for cascade)
static float iir_s1[2] = {0, 0};
static float iir_s2[2] = {0, 0};

// LMS delay line
static float lms_delay[LMS_NOTCH_LEN] = {0};
static int   lms_idx = 0;

// Encoder state
static volatile int  enc_count   = 0;
static volatile bool enc_pressed = false;
static int           enc_last    = 0;
static unsigned long enc_press_ms = 0;

// Preferences
static Preferences prefs;

// ── I2S setup ─────────────────────────────────────────────────────────────────

void i2s_dac_init() {
    i2s_config_t cfg = {
        .mode                 = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate          = SAMPLE_RATE,
        .bits_per_sample      = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format       = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags     = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count        = 4,
        .dma_buf_len          = I2S_BUF_LEN,
        .use_apll             = true,
        .tx_desc_auto_clear   = true,
    };
    i2s_pin_config_t pins = {
        .bck_io_num   = I2S_DAC_BCK,
        .ws_io_num    = I2S_DAC_WS,
        .data_out_num = I2S_DAC_DATA,
        .data_in_num  = I2S_PIN_NO_CHANGE,
    };
    i2s_driver_install(I2S_NUM_0, &cfg, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pins);
    i2s_set_clk(I2S_NUM_0, SAMPLE_RATE, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_STEREO);
}

void i2s_adc_init() {
    i2s_config_t cfg = {
        .mode                 = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate          = SAMPLE_RATE,
        .bits_per_sample      = I2S_BITS_PER_SAMPLE_32BIT,  // INMP441 outputs 24-bit in 32-bit frame
        .channel_format       = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags     = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count        = 4,
        .dma_buf_len          = I2S_BUF_LEN,
        .use_apll             = true,
    };
    i2s_pin_config_t pins = {
        .bck_io_num   = I2S_ADC_BCK,
        .ws_io_num    = I2S_ADC_WS,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num  = I2S_ADC_DATA,
    };
    i2s_driver_install(I2S_NUM_1, &cfg, 0, NULL);
    i2s_set_pin(I2S_NUM_1, &pins);
}

// ── FIR filter (direct-form, symmetric optimization) ─────────────────────────

float fir_apply(float x, const float* h, int N) {
    fir_delay[fir_idx] = x;
    float y = 0.0f;
    int k = fir_idx;
    for (int i = 0; i < N; i++) {
        y += pgm_read_float(&h[i]) * fir_delay[k];
        k = (k == 0) ? N - 1 : k - 1;
    }
    fir_idx = (fir_idx + 1) % N;
    return y;
}

// ── IIR biquad (Direct Form II transposed) ───────────────────────────────────

float iir_biquad(float x, const float* b, const float* a, float* s) {
    float y = b[0] * x + s[0];
    s[0] = b[1] * x - a[0] * y + s[1];
    s[1] = b[2] * x - a[1] * y;
    return y;
}

// ── LMS adaptive notch ────────────────────────────────────────────────────────
// Tracks and cancels a narrowband interferer.
// x = input sample; returns filtered output with interferer removed.

float lms_autonotch(float x) {
    lms_delay[lms_idx] = x;

    // Reference = delayed version of input (delay = 1 sample for notch)
    int ref_idx = (lms_idx == 0) ? LMS_NOTCH_LEN - 1 : lms_idx - 1;
    float d = lms_delay[ref_idx];

    // Compute filter output (estimated interference)
    float y_hat = 0.0f;
    int k = lms_idx;
    for (int i = 0; i < LMS_NOTCH_LEN; i++) {
        y_hat += lms_w[i] * lms_delay[k];
        k = (k == 0) ? LMS_NOTCH_LEN - 1 : k - 1;
    }

    float e = x - y_hat;    // error = desired signal (after removing interferer)

    // Weight update
    k = lms_idx;
    for (int i = 0; i < LMS_NOTCH_LEN; i++) {
        lms_w[i] += 2.0f * g_lms_mu * e * lms_delay[k];
        k = (k == 0) ? LMS_NOTCH_LEN - 1 : k - 1;
    }

    lms_idx = (lms_idx + 1) % LMS_NOTCH_LEN;
    return e;
}

// ── HP prefilter (DC block and 300 Hz rolloff, IIR first-order) ───────────────

static float hp_x_prev = 0.0f, hp_y_prev = 0.0f;
float hp_prefilter(float x) {
    // Simple first-order highpass: y[n] = 0.97×y[n-1] + x[n] − x[n-1]
    // f_-3dB ≈ (1-0.97)×Fs/(2π) ≈ 38 Hz — removes DC and very low hum
    float y = 0.97f * hp_y_prev + x - hp_x_prev;
    hp_x_prev = x;
    hp_y_prev = y;
    return y;
}

// ── AGC ───────────────────────────────────────────────────────────────────────

float agc_process(float x) {
    float level = fabsf(x);
    // Slow attack, fast release
    if (level > g_agc_gain * g_agc_target)
        g_agc_gain *= 0.999f;   // slow attack: ~8 samples to halve
    else
        g_agc_gain = g_agc_gain * 0.9998f + g_agc_target / (level + 1e-6f) * 0.0002f;

    // Limit gain range
    if (g_agc_gain < 0.1f) g_agc_gain = 0.1f;
    if (g_agc_gain > 100.0f) g_agc_gain = 100.0f;

    return x * g_agc_gain;
}

// ── Core processing ───────────────────────────────────────────────────────────

float process_sample(float x) {
    if (g_bypass) return x;

    x = hp_prefilter(x);

    float y;
    switch (g_mode) {
        case FILT_SSB:
            y = fir_apply(x, FIR_SSB, FIR_LEN_SSB);
            break;
        case FILT_CW_700:
            y = fir_apply(x, FIR_CW_700, FIR_LEN_CW);
            break;
        case FILT_CW_500:
            y = fir_apply(x, FIR_CW_500, FIR_LEN_CW);
            break;
        case FILT_CW_400:
            y = fir_apply(x, FIR_CW_400, FIR_LEN_CW);
            break;
        case FILT_NOTCH:
            y = fir_apply(x, FIR_NOTCH_800, FIR_LEN_NOTCH);
            break;
        case FILT_IIR_CW:
            y = iir_biquad(x, IIR_CW_700_B, IIR_CW_700_A, iir_s1);
            y *= 36.3f;    // restore passband gain (Q=10 → gain × 2Q = ×20 in dB)
            break;
        case FILT_IIR_NOTCH:
            y = iir_biquad(x, IIR_NOTCH_800_B, IIR_NOTCH_800_A, iir_s2);
            break;
        case FILT_LMS_NOTCH:
            y = lms_autonotch(x);
            break;
        default:
            y = x;
            break;
    }

    return agc_process(y);
}

// ── Audio task (runs on Core 1) ───────────────────────────────────────────────

static int16_t in_buf[BLOCK_SIZE];
static int16_t out_buf[BLOCK_SIZE * 2];  // stereo (L+R same)

void audio_task(void* param) {
    size_t bytes_read, bytes_written;

    while (true) {
        // Read block from I2S ADC (INMP441, 32-bit words)
        int32_t raw_buf[BLOCK_SIZE];
        i2s_read(I2S_NUM_1, raw_buf, sizeof(raw_buf), &bytes_read, portMAX_DELAY);

        for (int i = 0; i < BLOCK_SIZE; i++) {
            // INMP441: data in upper 24 bits of 32-bit frame
            float x = (float)(raw_buf[i] >> 11) / 32768.0f;   // normalize to ±1.0
            float y = process_sample(x);
            // Clip and convert back to 16-bit
            if (y >  1.0f) y =  1.0f;
            if (y < -1.0f) y = -1.0f;
            int16_t s = (int16_t)(y * 32767.0f);
            out_buf[2 * i]     = s;    // Left
            out_buf[2 * i + 1] = s;    // Right (same as L for mono)
        }

        i2s_write(I2S_NUM_0, out_buf, sizeof(out_buf), &bytes_written, portMAX_DELAY);
    }
}

// Alternate: built-in ADC task (lower quality but no I2S ADC module needed)
void audio_task_builtin_adc(void* param) {
    size_t bytes_written;
    unsigned long next_sample = micros();
    const unsigned long period_us = 1000000UL / SAMPLE_RATE;    // 125 µs at 8kHz

    while (true) {
        unsigned long now = micros();
        if (now >= next_sample) {
            next_sample += period_us;

            int raw = analogRead(ADC_BUILTIN_PIN);       // 0–4095
            float x = (raw - 2048) / 2048.0f;           // normalize ±1.0
            float y = process_sample(x);

            if (y >  1.0f) y =  1.0f;
            if (y < -1.0f) y = -1.0f;
            int16_t s = (int16_t)(y * 32767.0f);
            int16_t stereo[2] = { s, s };
            i2s_write(I2S_NUM_0, stereo, 4, &bytes_written, 0);
        }
        taskYIELD();
    }
}

// ── Encoder ISR ──────────────────────────────────────────────────────────────

void IRAM_ATTR enc_isr() {
    bool clk = digitalRead(ENC_CLK);
    bool dt  = digitalRead(ENC_DT);
    if (clk != dt) enc_count++;
    else           enc_count--;
}

void IRAM_ATTR enc_sw_isr() {
    enc_pressed = true;
    enc_press_ms = millis();
}

// ── Bypass relay control ──────────────────────────────────────────────────────

void set_bypass(bool bypass) {
    g_bypass = bypass;
    digitalWrite(BYPASS_RELAY_PIN, bypass ? LOW : HIGH);   // relay active = filter
    memset(fir_delay, 0, sizeof(fir_delay));
    memset(iir_s1, 0, sizeof(iir_s1));
    memset(iir_s2, 0, sizeof(iir_s2));
    memset(lms_w, 0, sizeof(lms_w));
    fir_idx = 0; lms_idx = 0;
    hp_x_prev = 0; hp_y_prev = 0;
    g_agc_gain = 1.0f;
}

// ── Display update ────────────────────────────────────────────────────────────

void update_display() {
#ifdef USE_OLED
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0, 0);
    oled.println(F("CW/SSB Audio Filter"));
    oled.drawLine(0, 9, 127, 9, SSD1306_WHITE);

    oled.setCursor(0, 12);
    oled.print(F("Mode: "));
    oled.println(FILT_NAMES[g_mode]);

    oled.setCursor(0, 22);
    oled.print(F("AGC:  x"));
    oled.print(g_agc_gain, 1);

    oled.setCursor(0, 32);
    if (g_bypass)
        oled.println(F("** BYPASS **"));
    else
        oled.println(F("Filter: ACTIVE"));

    // Simple signal level bar
    int level_bar = (int)(g_agc_target * g_agc_gain * 100);
    if (level_bar > 80) level_bar = 80;
    oled.setCursor(0, 44);
    oled.print(F("Lvl: |"));
    for (int i = 0; i < level_bar / 8; i++) oled.print('#');
    oled.println('|');

    oled.setCursor(0, 54);
    oled.print(F("LMS mu: "));
    oled.println(g_lms_mu, 4);

    oled.display();
#endif
}

// ── Settings persistence ──────────────────────────────────────────────────────

void save_settings() {
    prefs.begin("af", false);
    prefs.putUInt("mode",  (uint8_t)g_mode);
    prefs.putBool("bypass", g_bypass);
    prefs.putFloat("mu", g_lms_mu);
    prefs.end();
}

void load_settings() {
    prefs.begin("af", true);
    g_mode    = (FilterMode)prefs.getUInt("mode",   FILT_CW_700);
    g_bypass  = prefs.getBool("bypass", false);
    g_lms_mu  = prefs.getFloat("mu",    0.005f);
    prefs.end();
    if (g_mode >= FILT_MODE_COUNT) g_mode = FILT_CW_700;
}

// ── Serial command interpreter ────────────────────────────────────────────────

void handle_serial() {
    if (!Serial.available()) return;
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toLowerCase();

    if (cmd.startsWith("mode ")) {
        int m = cmd.substring(5).toInt();
        if (m >= 0 && m < FILT_MODE_COUNT) {
            g_mode = (FilterMode)m;
            Serial.printf("Mode: %s\n", FILT_NAMES[g_mode]);
        }
    } else if (cmd == "bypass on") {
        set_bypass(true);
        Serial.println("Bypass: ON");
    } else if (cmd == "bypass off") {
        set_bypass(false);
        Serial.println("Bypass: OFF");
    } else if (cmd.startsWith("mu ")) {
        float mu = cmd.substring(3).toFloat();
        if (mu > 0 && mu < 0.1f) g_lms_mu = mu;
        Serial.printf("LMS mu: %.5f\n", g_lms_mu);
    } else if (cmd == "status") {
        Serial.printf("Mode:   %s\n", FILT_NAMES[g_mode]);
        Serial.printf("Bypass: %s\n", g_bypass ? "ON" : "OFF");
        Serial.printf("AGC:    x%.2f\n", g_agc_gain);
        Serial.printf("LMS mu: %.5f\n", g_lms_mu);
        Serial.printf("Fs:     %d Hz\n", SAMPLE_RATE);
    } else if (cmd == "save") {
        save_settings();
        Serial.println("Saved.");
    } else if (cmd == "help") {
        Serial.println("Commands:");
        Serial.println("  mode N      — set filter mode 0-8");
        Serial.println("  bypass on/off");
        Serial.println("  mu X        — set LMS step size (0.001-0.05)");
        Serial.println("  status");
        Serial.println("  save");
        Serial.println("Modes:");
        for (int i = 0; i < FILT_MODE_COUNT; i++)
            Serial.printf("  %d: %s\n", i, FILT_NAMES[i]);
    }
}

// ── setup ─────────────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(115200);
    Serial.println(F("TM-AF-001 CW/SSB Audio Filter Rev A"));

    pinMode(LED_PIN,          OUTPUT);
    pinMode(BYPASS_RELAY_PIN, OUTPUT);
    pinMode(ENC_CLK,   INPUT_PULLUP);
    pinMode(ENC_DT,    INPUT_PULLUP);
    pinMode(ENC_SW,    INPUT_PULLUP);

    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BYPASS_RELAY_PIN, LOW);   // default: bypass (fail-safe)

    load_settings();
    memset(lms_w, 0, sizeof(lms_w));
    memset(fir_delay, 0, sizeof(fir_delay));

    i2s_dac_init();
    i2s_adc_init();

    Wire.begin(21, 22);

#ifdef USE_OLED
    if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 not found"));
    }
    oled.clearDisplay();
    oled.display();
#endif

    attachInterrupt(digitalPinToInterrupt(ENC_CLK), enc_isr,    CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENC_SW),  enc_sw_isr, FALLING);

    // Start audio processing on core 1
    xTaskCreatePinnedToCore(
        audio_task,
        "AudioTask",
        4096,
        NULL,
        24,           // high priority
        NULL,
        1             // core 1
    );

    // Apply loaded settings
    set_bypass(g_bypass);
    if (!g_bypass) digitalWrite(BYPASS_RELAY_PIN, HIGH);

    Serial.println(F("Ready. Type 'help' for commands."));
    update_display();
}

// ── loop ──────────────────────────────────────────────────────────────────────

static unsigned long last_save_ms   = 0;
static unsigned long last_disp_ms   = 0;
static int           last_enc_count = 0;

void loop() {
    unsigned long now = millis();

    // Handle encoder rotation → cycle filter mode
    int delta = enc_count - last_enc_count;
    if (delta != 0) {
        last_enc_count = enc_count;
        int new_mode = (int)g_mode + (delta > 0 ? 1 : -1);
        if (new_mode < 0) new_mode = FILT_MODE_COUNT - 1;
        if (new_mode >= FILT_MODE_COUNT) new_mode = 0;
        g_mode = (FilterMode)new_mode;
        // Reset filter state on mode change
        memset(fir_delay, 0, sizeof(fir_delay));
        memset(iir_s1, 0, sizeof(iir_s1));
        memset(iir_s2, 0, sizeof(iir_s2));
        fir_idx = 0;
        Serial.printf("Mode: %s\n", FILT_NAMES[g_mode]);
        update_display();
    }

    // Handle encoder push → toggle bypass
    if (enc_pressed && (now - enc_press_ms) > 20) {    // 20 ms debounce
        enc_pressed = false;
        set_bypass(!g_bypass);
        Serial.printf("Bypass: %s\n", g_bypass ? "ON" : "OFF");
        update_display();
    }

    // Display refresh every 250 ms
    if (now - last_disp_ms > 250) {
        last_disp_ms = now;
        update_display();
    }

    // Autosave every 5 minutes
    if (now - last_save_ms > 300000UL) {
        last_save_ms = now;
        save_settings();
    }

    handle_serial();

    // LED heartbeat
    digitalWrite(LED_PIN, (now / 500) & 1);

    delay(5);
}

/*
 * FILTER ALIGNMENT PROCEDURE
 * ──────────────────────────
 * 1. Connect audio signal generator to input jack (or use VFO/signal generator
 *    TM-SG-001).
 * 2. Connect oscilloscope or audio analyzer to output jack.
 * 3. Set mode to FILT_SSB (mode 0). Inject −10 dBu at 1000 Hz.
 *    Output should be approximately −10 dBu ± 2 dB (AGC will stabilize).
 * 4. Sweep input from 100 Hz to 4000 Hz. Record output level at:
 *    100, 200, 300, 500, 1000, 2000, 3000, 4000 Hz.
 *    Verify −3 dB at 300 Hz and 3000 Hz ± 50 Hz.
 * 5. Set mode to FILT_CW_700 (mode 1). Inject at 700 Hz.
 *    Sweep ±500 Hz. Verify −3 dB at 450 Hz and 950 Hz.
 * 6. Test LMS AUTO-NOTCH (mode 8):
 *    a. Inject desired audio (speech or CW from radio) plus a 800 Hz sine tone.
 *    b. After 1–2 seconds, notch depth should reach 30–40 dB.
 *    c. Vary tone to 600 Hz; verify notch tracks within ~0.5 seconds.
 * 7. Test bypass: press encoder. Audio should pass unfiltered.
 *    Verify no amplitude or phase change in bypass vs signal generator source.
 *
 * CALIBRATION NOTES
 * ─────────────────
 * AGC target: g_agc_target = 0.5 (output ≈ −6 dBFS RMS for nominal input)
 * Adjust g_agc_target if output level too high/low for headphones.
 * LMS step size (g_lms_mu):
 *   Larger = faster convergence, more residual noise.
 *   0.005 is nominal; for weak signals increase to 0.01.
 *   For very strong carriers reduce to 0.001.
 */
