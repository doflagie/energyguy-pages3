/*
 * sweep_modulation.h
 * Frequency Sweep Engine and Modulation Control
 * TM-SG-001 Rev A
 *
 * Sweep: steps synthesizer from f_start to f_stop, samples AD8307 at each
 * step, streams CSV data over USB serial and buffers for CYD display.
 *
 * Modulation: AM via HMC1119 attenuation, FM via DDS FTW offset,
 * CW with shaped keying via HMC1119 ramp.
 *
 * Requires: dds_driver.h, adf4351_driver.h, Si5351 library (Etherkit)
 * References synthesizer objects declared in main sketch.
 */

#pragma once
#include <Arduino.h>

// AD8307 power monitor
#define AD8307_ADC_PIN  36       // GPIO36 ADC1_CH0
#define AD8307_SLOPE    24.0f    // mV/dB (calibrate empirically)
#define AD8307_INTERCEPT (-84.0f) // dBm at 0V output

// Relay control (via PCF8574 I2C GPIO expander at 0x20)
// PCF8574 P0–P7 drive relay coils via 2N7002 MOSFETs
#define RELAY_10DB   0x01  // K1
#define RELAY_20DB   0x02  // K2
#define RELAY_40DB   0x04  // K3
#define RELAY_AMPBYP 0x08  // K_byp
#define RELAY_LPF1   0x10  // f_c=2.5 MHz
#define RELAY_LPF2   0x20  // f_c=8.5 MHz
#define RELAY_LPF3   0x40  // f_c=35 MHz
#define RELAY_LPF4   0x80  // f_c=160 MHz

// HMC1119 digital attenuator (SPI)
#define HMC1119_CS_PIN  17       // GPIO17 chip select

// Audio input ADC
#define AF_ADC_PIN      34       // GPIO34 ADC1_CH6 (audio input)
#define AF_SAMPLE_HZ    8000     // Audio sample rate

// CW key input
#define KEY_PIN         32       // GPIO32 (active LOW = key down)

// ── Power Monitor ────────────────────────────────────────────────────────────

// Read output level in dBm from AD8307.  Average n samples.
float readPower_dBm(int n_avg = 16) {
    long sum = 0;
    for (int i = 0; i < n_avg; i++) {
        sum += analogRead(AD8307_ADC_PIN);
        delayMicroseconds(50);
    }
    float adc_v = (sum / (float)n_avg) * (3.3f / 4095.0f);
    return (adc_v * 1000.0f / AD8307_SLOPE) + AD8307_INTERCEPT;
}

// ── HMC1119 Digital Attenuator ───────────────────────────────────────────────

// Set attenuation 0.00 to 31.75 dB in 0.25 dB steps.
// 7-bit control word: bit 6 = 16 dB, 5 = 8, 4 = 4, 3 = 2, 2 = 1, 1 = 0.5, 0 = 0.25 dB
void hmc1119SetAtten(float atten_dB) {
    if (atten_dB < 0) atten_dB = 0;
    if (atten_dB > 31.75f) atten_dB = 31.75f;
    uint8_t code = (uint8_t)(atten_dB / 0.25f) & 0x7F;
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
    digitalWrite(HMC1119_CS_PIN, LOW);
    SPI.transfer(code);
    delayMicroseconds(1);
    digitalWrite(HMC1119_CS_PIN, HIGH);
    SPI.endTransaction();
}

// ── PCF8574 Relay Control ────────────────────────────────────────────────────

#include <Wire.h>
#define PCF8574_ADDR 0x20

static uint8_t _relay_state = 0;

void relayWrite(uint8_t mask, bool active) {
    if (active) _relay_state |= mask;
    else        _relay_state &= ~mask;
    Wire.beginTransmission(PCF8574_ADDR);
    Wire.write(_relay_state);
    Wire.endTransmission();
}

// Set switched attenuator by total dB value (0, 10, 20, 30, 40, 50, 60, 70 dB).
void setAttenuatorStepped(uint8_t db) {
    relayWrite(RELAY_10DB, db & 10);
    relayWrite(RELAY_20DB, db & 20);
    relayWrite(RELAY_40DB, db & 40);
}

// Select output LPF based on frequency
void selectLPF(uint32_t freq_hz) {
    // Clear all LPF relays, then enable appropriate one
    _relay_state &= ~(RELAY_LPF1|RELAY_LPF2|RELAY_LPF3|RELAY_LPF4);
    if      (freq_hz <= 2500000UL)   _relay_state |= RELAY_LPF1;
    else if (freq_hz <= 8500000UL)   _relay_state |= RELAY_LPF2;
    else if (freq_hz <= 35000000UL)  _relay_state |= RELAY_LPF3;
    else                             _relay_state |= RELAY_LPF4;
    Wire.beginTransmission(PCF8574_ADDR);
    Wire.write(_relay_state);
    Wire.endTransmission();
}

// ── Sweep Engine ─────────────────────────────────────────────────────────────

#define SWEEP_MAX_POINTS 512

struct SweepPoint {
    uint32_t freq_hz;
    float    level_dBm;
};

struct SweepConfig {
    uint32_t f_start;       // Hz
    uint32_t f_stop;        // Hz
    uint32_t f_step;        // Hz (0 = auto: 500 evenly-spaced points)
    uint16_t dwell_ms;      // ms per step
    bool     log_scale;     // true = logarithmic frequency steps
    bool     stream_serial; // true = output CSV over Serial
    bool     ping_pong;     // true = forward then reverse
};

struct SweepResult {
    SweepPoint pts[SWEEP_MAX_POINTS];
    uint16_t   n_pts;
    float      f_at_m3db_lo;   // lower −3 dB frequency
    float      f_at_m3db_hi;   // upper −3 dB frequency
    float      peak_dBm;
    uint32_t   peak_freq_hz;
};

// Forward declaration: the main sketch provides this callback to set frequency.
// Signature: void setOutputFreq(uint32_t freq_hz);
typedef void (*SetFreqCallback)(uint32_t);

// Execute a sweep.  Calls set_freq_cb for each step, reads AD8307.
// Stores results in *result.  Returns number of points taken.
uint16_t runSweep(const SweepConfig& cfg, SweepResult* result,
                  SetFreqCallback set_freq_cb) {
    memset(result, 0, sizeof(SweepResult));

    uint32_t f_range = cfg.f_stop - cfg.f_start;
    uint32_t step = cfg.f_step;
    if (step == 0 || (f_range / step) > SWEEP_MAX_POINTS)
        step = f_range / SWEEP_MAX_POINTS;
    if (step < 1) step = 1;

    uint16_t n = 0;
    float    peak = -200.0f;
    uint32_t f = cfg.f_start;

    if (cfg.stream_serial)
        Serial.println(F("freq_hz,level_dBm"));

    while (f <= cfg.f_stop && n < SWEEP_MAX_POINTS) {
        set_freq_cb(f);
        selectLPF(f);
        delay(cfg.dwell_ms);

        float lvl = readPower_dBm(32);
        result->pts[n] = {f, lvl};
        if (lvl > peak) { peak = lvl; result->peak_freq_hz = f; }

        if (cfg.stream_serial) {
            Serial.print(f); Serial.print(',');
            Serial.println(lvl, 2);
        }

        if (cfg.log_scale && f > 0)
            f = (uint32_t)((double)f * pow((double)cfg.f_stop / cfg.f_start,
                           1.0 / ((f_range / step) + 1)));
        else
            f += step;
        n++;
    }

    result->n_pts    = n;
    result->peak_dBm = peak;

    // Find −3 dB points
    float thresh = peak - 3.0f;
    result->f_at_m3db_lo = result->pts[0].freq_hz;
    result->f_at_m3db_hi = result->pts[n-1].freq_hz;
    for (uint16_t i = 0; i < n; i++) {
        if (result->pts[i].level_dBm >= thresh) {
            result->f_at_m3db_lo = result->pts[i].freq_hz;
            break;
        }
    }
    for (int i = n-1; i >= 0; i--) {
        if (result->pts[i].level_dBm >= thresh) {
            result->f_at_m3db_hi = result->pts[i].freq_hz;
            break;
        }
    }

    // Ping-pong: reverse sweep
    if (cfg.ping_pong) {
        for (int i = n-2; i >= 0 && n < SWEEP_MAX_POINTS; i--) {
            set_freq_cb(result->pts[i].freq_hz);
            selectLPF(result->pts[i].freq_hz);
            delay(cfg.dwell_ms);
            result->pts[n++] = {result->pts[i].freq_hz, readPower_dBm(32)};
        }
    }

    return result->n_pts;
}

// ── Modulation ───────────────────────────────────────────────────────────────

enum ModMode { MOD_NONE, MOD_AM, MOD_FM_DDS, MOD_FM_PLL, MOD_CW };

struct ModConfig {
    ModMode  mode;
    float    fm_deviation_hz; // FM: peak deviation in Hz
    float    am_depth;        // AM: 0.0–1.0 (100%)
    uint16_t cw_rise_ms;      // CW: shaped rise time (ms)
    uint16_t cw_sidetone_hz;  // CW: sidetone frequency
};

// AM modulation: update HMC1119 at audio rate.
// atten_base: static attenuation for carrier level.
// audio_sample: signed 12-bit ADC reading (0–4095, bias at 2048).
// am_depth: 0.0–1.0.
void doAM(float atten_base, int audio_sample, float am_depth) {
    float normalized = (audio_sample - 2048.0f) / 2048.0f;  // −1 to +1
    // Envelope = 1 + m·sin(ωt); AM attenuation decreases when envelope peaks
    float envelope = 1.0f + am_depth * normalized;
    if (envelope < 0.01f) envelope = 0.01f;
    // Level change in dB: atten = atten_base − 20·log10(envelope)
    float atten = atten_base - 20.0f * log10f(envelope);
    if (atten < 0) atten = 0;
    if (atten > 31.75f) atten = 31.75f;
    hmc1119SetAtten(atten);
}

// CW shaped keying: ramp attenuation over rise_ms.
// direction: true = key down (ramp ON), false = key up (ramp OFF).
// dds: pointer to DDS object for phase reset on key-down.
void cwKey(bool key_down, uint16_t rise_ms) {
    const int steps = 20;
    int delay_step = rise_ms / steps;
    if (key_down) {
        // Ramp from full attenuation (70 dB) to 0 dB
        for (int i = steps; i >= 0; i--) {
            hmc1119SetAtten(31.75f * i / steps);
            delay(delay_step);
        }
    } else {
        // Ramp from 0 dB to full attenuation
        for (int i = 0; i <= steps; i++) {
            hmc1119SetAtten(31.75f * i / steps);
            delay(delay_step);
        }
        hmc1119SetAtten(31.75f);  // ensure fully off
    }
}
