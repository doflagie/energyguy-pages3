/*
 * reciprocal_counter.h
 * High-Resolution Reciprocal / Period Frequency Counter
 * TM-FC-001 Rev A
 *
 * Method: Measure the exact time for N complete input cycles using
 * ESP32 hardware timer capture via the RMT peripheral or PCNT gate.
 *
 * Resolution: δf = f_input² / (N × f_reference)
 *   At 1 kHz, N=1000, f_ref=10 MHz: δf = (10³)² / (10³ × 10⁷) = 0.0001 Hz (0.1 mHz)
 *   At 14.175 MHz, N=10000, f_ref=10 MHz: δf ≈ 2 Hz
 *   At 100 kHz, N=1000: δf = 0.01 Hz
 *
 * Implementation:
 *   Uses ESP32 PCNT to count input cycles AND simultaneously uses esp_timer
 *   (80 MHz APB clock, 12.5 ns resolution) to measure elapsed time.
 *   Period = elapsed_timer_ticks / (timer_frequency × N_cycles)
 *   Frequency = N_cycles / period_seconds
 *
 * GPIO assignments:
 *   Input signal:   GPIO34 (PCNT_UNIT_0, via 74LVC14A)
 *   Reference gate: Uses esp_timer (APB clock, optionally corrected by GPSDO)
 *
 * Note on reference accuracy:
 *   The ESP32 APB clock (80 MHz) has ±25 ppm drift without correction.
 *   With TCXO reference counting (gpsdo.h), apply gpsdo_correction() factor.
 *   With GPSDO locked: accuracy approaches GPS UTC (<<1 ppb long-term).
 */

#pragma once
#include <Arduino.h>
#include <driver/pcnt.h>
#include <esp_timer.h>

// ── Configuration ─────────────────────────────────────────────────────────────
#define RC_INPUT_GPIO        34          // frequency input (from 74LVC14A)
#define RC_PCNT_UNIT         PCNT_UNIT_0 // reuses direct-count unit
#define RC_TIMER_HZ          80000000UL  // ESP32 APB clock (80 MHz)
#define RC_N_DEFAULT         1000        // default: measure 1000 cycles
#define RC_N_MIN             1
#define RC_N_MAX             1000000
#define RC_TIMEOUT_US        5000000UL   // 5 second timeout (handles <1 Hz)
#define RC_APB_PPM           25.0        // APB clock accuracy without correction

// ── Mode enumeration ──────────────────────────────────────────────────────────
typedef enum {
    RC_MODE_RECIPROCAL = 0,   // f = N / T  (standard reciprocal)
    RC_MODE_PERIOD,            // T = t / N  (period measurement, display in µs/ms)
    RC_MODE_SINGLE_PERIOD,     // T = time for 1 cycle (lowest freq, max period)
    RC_MODE_BURST,             // burst mode: measure M bursts of N cycles, average
} RecipMode;

// ── Result structure ─────────────────────────────────────────────────────────
struct RecipResult {
    double      frequency_hz;     // measured frequency in Hz
    double      period_us;        // measured period in microseconds
    double      period_ns;        // measured period in nanoseconds (for <1 MHz)
    uint32_t    cycles_measured;  // actual N cycles used
    double      elapsed_us;       // actual gate time in microseconds
    double      resolution_hz;    // theoretical resolution (f²/N·f_ref)
    double      accuracy_ppb;     // estimated accuracy (ppb)
    bool        valid;            // measurement complete and valid
    bool        timeout;          // timed out waiting for signal
    bool        overflow;         // input frequency too high for reciprocal mode
};

// ── State ─────────────────────────────────────────────────────────────────────
static RecipResult recip_result = {};
static volatile bool  rc_gate_open   = false;
static volatile bool  rc_complete    = false;
static volatile int64_t rc_start_us  = 0;
static volatile int64_t rc_stop_us   = 0;
static volatile int64_t rc_overflow_accum = 0;
static uint32_t rc_target_cycles    = RC_N_DEFAULT;
static RecipMode rc_mode            = RC_MODE_RECIPROCAL;

// ── PCNT overflow ISR (shared with direct count when in reciprocal mode) ──────
static void IRAM_ATTR rc_pcnt_isr(void* arg) {
    if (rc_gate_open) {
        rc_overflow_accum += 30000;
    }
}

// ── PCNT: read full 64-bit count including overflow ──────────────────────────
static int64_t rc_pcnt_read() {
    int16_t cnt = 0;
    pcnt_get_counter_value(RC_PCNT_UNIT, &cnt);
    return rc_overflow_accum + (int64_t)cnt;
}

// ── Initialization ────────────────────────────────────────────────────────────
void reciprocal_init() {
    pcnt_config_t cfg = {};
    cfg.pulse_gpio_num = RC_INPUT_GPIO;
    cfg.ctrl_gpio_num  = PCNT_PIN_NOT_USED;
    cfg.lctrl_mode     = PCNT_MODE_KEEP;
    cfg.hctrl_mode     = PCNT_MODE_KEEP;
    cfg.pos_mode       = PCNT_COUNT_INC;
    cfg.neg_mode       = PCNT_COUNT_DIS;
    cfg.counter_h_lim  = 30000;
    cfg.counter_l_lim  = -1;
    cfg.unit           = RC_PCNT_UNIT;
    cfg.channel        = PCNT_CHANNEL_0;
    pcnt_unit_config(&cfg);
    pcnt_filter_disable(RC_PCNT_UNIT);
    pcnt_event_enable(RC_PCNT_UNIT, PCNT_EVT_H_LIM);
    pcnt_isr_register(rc_pcnt_isr, NULL, 0, NULL);
    pcnt_intr_enable(RC_PCNT_UNIT);
    pcnt_counter_pause(RC_PCNT_UNIT);
    pcnt_counter_clear(RC_PCNT_UNIT);
}

// ── Set measurement parameters ────────────────────────────────────────────────
void reciprocal_set_n(uint32_t n) {
    rc_target_cycles = constrain(n, RC_N_MIN, RC_N_MAX);
}

void reciprocal_set_mode(RecipMode mode) {
    rc_mode = mode;
}

// ── Auto-select N based on expected frequency ─────────────────────────────────
// Chooses N so that gate time ≈ 1 second and resolution is maximized.
uint32_t reciprocal_auto_n(double freq_hz) {
    if (freq_hz <= 0.0) return 1;
    if (freq_hz < 1.0)       return 1;         // single period, < 1 Hz
    if (freq_hz < 10.0)      return 1;         // too slow for averaging
    if (freq_hz < 100.0)     return 10;
    if (freq_hz < 1000.0)    return 100;
    if (freq_hz < 10000.0)   return 1000;
    if (freq_hz < 100000.0)  return 10000;
    if (freq_hz < 1000000.0) return 100000;
    return 1000000;                             // 14 MHz × 1M = ~70 ms gate
}

// ── Synchronous measurement (blocking; use from task or separate thread) ──────
// Returns true if measurement completed, false on timeout.
// correction: GPSDO correction factor from gpsdo_correction() (1.0 if no GPSDO)
bool reciprocal_measure(RecipResult* result, double correction) {
    memset(result, 0, sizeof(RecipResult));

    uint32_t N = rc_target_cycles;
    if (rc_mode == RC_MODE_SINGLE_PERIOD) N = 1;

    // Reset PCNT and accumulators
    rc_overflow_accum = 0;
    rc_complete       = false;
    pcnt_counter_pause(RC_PCNT_UNIT);
    pcnt_counter_clear(RC_PCNT_UNIT);

    // Start counting
    rc_gate_open = true;
    int64_t t_start = esp_timer_get_time();   // microseconds
    pcnt_counter_resume(RC_PCNT_UNIT);

    // Wait for N cycles or timeout
    int64_t deadline = t_start + (int64_t)RC_TIMEOUT_US;
    while (true) {
        int64_t now = esp_timer_get_time();
        if (now > deadline) {
            // Timeout — read what we have
            pcnt_counter_pause(RC_PCNT_UNIT);
            rc_gate_open = false;
            int64_t t_stop = now;
            int64_t cycles = rc_pcnt_read();

            if (cycles < 2) {
                result->timeout = true;
                result->valid   = false;
                return false;
            }
            // Compute with partial count (less accurate but better than nothing)
            double elapsed_s = (double)(t_stop - t_start) * 1e-6 * correction;
            result->frequency_hz   = (double)cycles / elapsed_s;
            result->period_us      = elapsed_s * 1e6 / (double)cycles;
            result->period_ns      = result->period_us * 1000.0;
            result->cycles_measured = (uint32_t)cycles;
            result->elapsed_us     = (double)(t_stop - t_start);
            result->timeout        = true;
            result->valid          = (cycles >= 2);
            return result->valid;
        }

        // Check if we've counted enough cycles
        int64_t cnt = rc_pcnt_read();
        if (cnt >= (int64_t)N) {
            // Capture stop time as close to N-th cycle as possible
            int64_t t_stop = esp_timer_get_time();
            pcnt_counter_pause(RC_PCNT_UNIT);
            rc_gate_open = false;

            // Re-read for final value (might have incremented during pause delay)
            int64_t final_cnt = rc_pcnt_read();

            double elapsed_s = (double)(t_stop - t_start) * 1e-6 * correction;

            if (elapsed_s <= 0.0 || final_cnt <= 0) {
                result->valid = false;
                return false;
            }

            double freq = (double)final_cnt / elapsed_s;

            result->frequency_hz    = freq;
            result->period_us       = elapsed_s * 1e6 / (double)final_cnt;
            result->period_ns       = result->period_us * 1000.0;
            result->cycles_measured = (uint32_t)final_cnt;
            result->elapsed_us      = (double)(t_stop - t_start);
            result->valid           = true;
            result->timeout         = false;

            // Theoretical resolution (Hz)
            double f_ref = (double)RC_TIMER_HZ * correction;
            result->resolution_hz = (freq * freq) / ((double)final_cnt * f_ref);

            // Accuracy estimate (ppb)
            if (correction != 1.0) {
                result->accuracy_ppb = 1.0;   // GPSDO: <1 ppb
            } else {
                result->accuracy_ppb = RC_APB_PPM * 1000.0;  // APB: ~25,000 ppb
            }
            return true;
        }
        // Yield to avoid watchdog trigger on long measurements
        if ((esp_timer_get_time() - t_start) > 100000LL) {
            vTaskDelay(1);   // yield 1 tick (1 ms) on long measurements
        }
    }
}

// ── Non-blocking state machine version ────────────────────────────────────────
// Call reciprocal_start() once, then poll reciprocal_poll() until it returns true.
// Useful when main loop must stay responsive (touch, display updates).

typedef enum {
    RC_SM_IDLE = 0,
    RC_SM_COUNTING,
    RC_SM_DONE,
    RC_SM_TIMEOUT
} RecipSMState;

static RecipSMState rc_sm_state = RC_SM_IDLE;
static int64_t      rc_sm_start_us  = 0;
static double       rc_sm_correction = 1.0;

void reciprocal_start(double correction) {
    uint32_t N = rc_target_cycles;
    if (rc_mode == RC_MODE_SINGLE_PERIOD) N = 1;
    rc_target_cycles = N;

    rc_overflow_accum = 0;
    rc_sm_correction  = correction;

    pcnt_counter_pause(RC_PCNT_UNIT);
    pcnt_counter_clear(RC_PCNT_UNIT);

    rc_gate_open   = true;
    rc_sm_start_us = esp_timer_get_time();
    pcnt_counter_resume(RC_PCNT_UNIT);
    rc_sm_state    = RC_SM_COUNTING;
}

// Returns true when measurement is complete (result populated) or timed out.
bool reciprocal_poll(RecipResult* result) {
    if (rc_sm_state == RC_SM_IDLE) return false;
    if (rc_sm_state == RC_SM_DONE || rc_sm_state == RC_SM_TIMEOUT) {
        rc_sm_state = RC_SM_IDLE;
        return true;
    }

    int64_t now    = esp_timer_get_time();
    int64_t cnt    = rc_pcnt_read();
    int64_t target = (int64_t)rc_target_cycles;

    bool timed_out = (now - rc_sm_start_us) > (int64_t)RC_TIMEOUT_US;
    bool got_n     = (cnt >= target);

    if (got_n || timed_out) {
        int64_t t_stop = now;
        pcnt_counter_pause(RC_PCNT_UNIT);
        rc_gate_open = false;

        int64_t final_cnt = rc_pcnt_read();
        double elapsed_s  = (double)(t_stop - rc_sm_start_us) * 1e-6 * rc_sm_correction;

        memset(result, 0, sizeof(RecipResult));
        result->timeout = timed_out && !got_n;

        if (final_cnt >= 2 && elapsed_s > 0.0) {
            double freq = (double)final_cnt / elapsed_s;
            result->frequency_hz    = freq;
            result->period_us       = elapsed_s * 1e6 / (double)final_cnt;
            result->period_ns       = result->period_us * 1000.0;
            result->cycles_measured = (uint32_t)final_cnt;
            result->elapsed_us      = (double)(t_stop - rc_sm_start_us);
            result->valid           = true;

            double f_ref = (double)RC_TIMER_HZ * rc_sm_correction;
            result->resolution_hz = (freq * freq) / ((double)final_cnt * f_ref);
            result->accuracy_ppb  = (rc_sm_correction != 1.0) ? 1.0 : (RC_APB_PPM * 1000.0);
        } else {
            result->valid = false;
        }

        rc_sm_state = RC_SM_IDLE;
        return true;
    }
    return false;
}

bool reciprocal_is_running() {
    return (rc_sm_state == RC_SM_COUNTING);
}

// ── Period formatting helpers ─────────────────────────────────────────────────
// Format period into human-readable string with appropriate units.
void reciprocal_format_period(double period_ns, char* buf, size_t bufsz) {
    if (period_ns >= 1e9) {
        snprintf(buf, bufsz, "%.6f s", period_ns * 1e-9);
    } else if (period_ns >= 1e6) {
        snprintf(buf, bufsz, "%.6f ms", period_ns * 1e-6);
    } else if (period_ns >= 1e3) {
        snprintf(buf, bufsz, "%.6f µs", period_ns * 1e-3);
    } else {
        snprintf(buf, bufsz, "%.3f ns", period_ns);
    }
}

// ── Frequency formatting helper ───────────────────────────────────────────────
// Format frequency with appropriate prefix (Hz/kHz/MHz/GHz).
// prescale_factor: multiply raw frequency by this (1 for direct, 8 or 64 for prescaler)
void reciprocal_format_freq(double freq_hz, uint32_t prescale_factor, char* buf, size_t bufsz) {
    double f = freq_hz * (double)prescale_factor;
    if (f >= 1e9) {
        snprintf(buf, bufsz, "%.9f GHz", f * 1e-9);
    } else if (f >= 1e6) {
        snprintf(buf, bufsz, "%.9f MHz", f * 1e-6);
    } else if (f >= 1e3) {
        snprintf(buf, bufsz, "%.6f kHz", f * 1e-3);
    } else {
        snprintf(buf, bufsz, "%.3f Hz", f);
    }
}

// ── Resolution description string ────────────────────────────────────────────
void reciprocal_format_resolution(double res_hz, char* buf, size_t bufsz) {
    if (res_hz < 0.001) {
        snprintf(buf, bufsz, "%.4f mHz", res_hz * 1000.0);
    } else if (res_hz < 1.0) {
        snprintf(buf, bufsz, "%.4f Hz", res_hz);
    } else {
        snprintf(buf, bufsz, "%.2f Hz", res_hz);
    }
}
