/*
 * gpsdo.h
 * GPS-Disciplined Oscillator for Frequency Counter
 * TM-FC-001 Rev A
 *
 * GPS 1 PPS input: GPIO26 (RISING edge interrupt)
 * TCXO/OCXO reference: counted by PCNT_UNIT_2 (GPIO25 as input via jumper)
 * Control DAC: GPIO25 (DAC1, 0–3.3V → TCXO VC pin)
 * GPS UART2: GPIO16 (RX), GPIO17 (TX), 9600 baud
 *
 * Algorithm:
 *   1. Count 10 MHz reference ticks between GPS 1 PPS edges.
 *   2. Compute frequency error vs expected 10,000,000 ticks/second.
 *   3. Apply PI control loop to DAC output.
 *   4. Converges to GPS UTC accuracy over ~10–60 minute timeframe.
 *
 * NOTE: GPIO25 is shared between DAC output (control) and a possible
 * reference input. In practice: route TCXO 10 MHz to a separate PCNT pin
 * (e.g., GPIO39) and keep GPIO25 as DAC output only. See wiring notes.
 */

#pragma once
#include <Arduino.h>
#include <driver/pcnt.h>

// ── Pin assignments ──────────────────────────────────────────────────────────
#define GPSDO_PPS_PIN     26    // GPS 1 PPS rising edge
#define GPSDO_DAC_PIN     25    // DAC output → TCXO VC
#define GPSDO_REF_PIN     39    // 10 MHz TCXO output monitored by PCNT_UNIT_2
#define GPSDO_GPS_RX      16    // GPS UART2 RX
#define GPSDO_GPS_TX      17    // GPS UART2 TX

// ── PCNT unit for reference counting ────────────────────────────────────────
#define REF_PCNT_UNIT     PCNT_UNIT_2
#define REF_NOMINAL_HZ    10000000UL   // 10 MHz reference

// ── PI loop parameters ───────────────────────────────────────────────────────
// Tune Kp and Ki for your specific oscillator's pull sensitivity.
// TCXO typical VC sensitivity: ~1 ppm/V = 10 Hz/V at 10 MHz.
// DAC range 0–3.3V → ~33 Hz pull range at 10 MHz.
#define GPSDO_KP          0.0002f    // proportional gain (per ns error → DAC delta)
#define GPSDO_KI          0.00002f   // integral gain
#define GPSDO_LOCK_NS     200.0f     // phase error threshold for "locked" (ns)
#define GPSDO_LOCK_COUNT  30         // consecutive seconds within threshold to declare lock
#define GPSDO_HOLDOVER_MAX 3600      // max holdover seconds before warning

// ── DAC range ────────────────────────────────────────────────────────────────
#define DAC_CENTER        128        // nominal center (maps to 1.65V)
#define DAC_MIN           10
#define DAC_MAX           245

// ── State structure ───────────────────────────────────────────────────────────
struct GPSDOState {
    // Measurement
    int64_t  ref_ticks;           // 10 MHz ticks since last PPS
    double   freq_error_hz;       // error in Hz vs 10 MHz nominal
    double   freq_error_ppm;
    double   phase_error_ns;      // phase error vs GPS PPS
    double   phase_accum_ns;      // accumulated phase for display

    // PI loop state
    double   integrator;
    uint8_t  dac_value;           // current DAC output 0–255
    int      lock_counter;        // consecutive seconds in lock band
    bool     locked;
    bool     holdover;
    uint32_t holdover_seconds;

    // GPS state (from NMEA parser in fsm_datalogger.h or dedicated parser here)
    bool     gps_valid;
    int      gps_satellites;
    char     gps_time[12];

    // Statistics
    double   error_min_ppb;
    double   error_max_ppb;
    double   error_sum_ppb;
    int      error_count;

    // OCXO warm-up (for Config B)
    float    oven_temp_c;
    bool     oven_ready;
};

static GPSDOState gpsdo = {};

// ── PCNT overflow for 10 MHz reference counting ──────────────────────────────
static volatile int64_t ref_overflow_accum = 0;
static volatile bool    pps_triggered      = false;
static volatile int64_t pps_ref_snapshot   = 0;   // ref count at PPS edge
static int64_t          ref_at_last_pps    = 0;

static void IRAM_ATTR ref_pcnt_isr(void* arg) {
    ref_overflow_accum += 30000;
}

static void IRAM_ATTR pps_isr(void* arg) {
    // Snapshot reference count at PPS edge
    int16_t cnt = 0;
    PCNT.cnt_unit[REF_PCNT_UNIT].cnt_val;   // raw register read (fast)
    pps_ref_snapshot = ref_overflow_accum + (int64_t)cnt;
    pps_triggered = true;
}

// ── Initialization ────────────────────────────────────────────────────────────
void gpsdo_init() {
    memset(&gpsdo, 0, sizeof(gpsdo));
    gpsdo.dac_value   = DAC_CENTER;
    gpsdo.integrator  = (double)DAC_CENTER;
    gpsdo.error_min_ppb = 9999.0;
    gpsdo.error_max_ppb = -9999.0;

    // Configure PCNT for 10 MHz reference counting
    pcnt_config_t ref_cfg = {};
    ref_cfg.pulse_gpio_num = GPSDO_REF_PIN;
    ref_cfg.ctrl_gpio_num  = PCNT_PIN_NOT_USED;
    ref_cfg.lctrl_mode     = PCNT_MODE_KEEP;
    ref_cfg.hctrl_mode     = PCNT_MODE_KEEP;
    ref_cfg.pos_mode       = PCNT_COUNT_INC;
    ref_cfg.neg_mode       = PCNT_COUNT_DIS;
    ref_cfg.counter_h_lim  = 30000;
    ref_cfg.counter_l_lim  = -1;
    ref_cfg.unit           = REF_PCNT_UNIT;
    ref_cfg.channel        = PCNT_CHANNEL_0;
    pcnt_unit_config(&ref_cfg);
    pcnt_filter_disable(REF_PCNT_UNIT);
    pcnt_event_enable(REF_PCNT_UNIT, PCNT_EVT_H_LIM);
    pcnt_isr_register(ref_pcnt_isr, NULL, 0, NULL);
    pcnt_intr_enable(REF_PCNT_UNIT);
    pcnt_counter_resume(REF_PCNT_UNIT);

    // Configure 1 PPS interrupt
    pinMode(GPSDO_PPS_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(GPSDO_PPS_PIN), pps_isr, RISING);

    // Initialize GPS UART
    Serial2.begin(9600, SERIAL_8N1, GPSDO_GPS_RX, GPSDO_GPS_TX);

    // Set DAC to center
    dacWrite(GPSDO_DAC_PIN, gpsdo.dac_value);
    Serial.println("[GPSDO] Initialized. Waiting for GPS 1PPS...");
}

// ── Get current 64-bit reference count ───────────────────────────────────────
int64_t gpsdo_ref_count() {
    int16_t cnt = 0;
    pcnt_get_counter_value(REF_PCNT_UNIT, &cnt);
    return ref_overflow_accum + (int64_t)cnt;
}

// ── Called from main loop; processes GPS 1 PPS ──────────────────────────────
void gpsdo_update() {
    if (!pps_triggered) return;
    pps_triggered = false;

    // How many reference ticks since last PPS?
    int64_t current_pps_count = pps_ref_snapshot;
    int64_t ticks_per_second  = current_pps_count - ref_at_last_pps;
    ref_at_last_pps            = current_pps_count;

    // Skip first sample (no valid delta yet)
    static bool first_sample = true;
    if (first_sample) { first_sample = false; return; }

    // Compute error
    double error_ticks = (double)ticks_per_second - (double)REF_NOMINAL_HZ;
    double error_hz    = error_ticks;                    // error in Hz
    double error_ppm   = error_hz / (REF_NOMINAL_HZ / 1e6);
    double error_ppb   = error_ppm * 1000.0;
    // Phase error: 1 tick = 100 ns at 10 MHz
    double phase_ns    = error_ticks * 100.0;

    gpsdo.ref_ticks      = ticks_per_second;
    gpsdo.freq_error_hz  = error_hz;
    gpsdo.freq_error_ppm = error_ppm;
    gpsdo.phase_error_ns = phase_ns;
    gpsdo.phase_accum_ns += phase_ns;

    // PI controller
    double prop  = GPSDO_KP * phase_ns;
    gpsdo.integrator += GPSDO_KI * phase_ns;
    gpsdo.integrator  = constrain(gpsdo.integrator, DAC_MIN, DAC_MAX);
    double control    = (double)DAC_CENTER + prop + gpsdo.integrator - DAC_CENTER;

    gpsdo.dac_value = (uint8_t)constrain((int)control, DAC_MIN, DAC_MAX);
    dacWrite(GPSDO_DAC_PIN, gpsdo.dac_value);

    // Lock detection
    if (fabsf((float)phase_ns) < GPSDO_LOCK_NS) {
        if (gpsdo.lock_counter < GPSDO_LOCK_COUNT) gpsdo.lock_counter++;
    } else {
        if (gpsdo.lock_counter > 0) gpsdo.lock_counter--;
    }
    gpsdo.locked = (gpsdo.lock_counter >= GPSDO_LOCK_COUNT);

    // Statistics
    gpsdo.error_sum_ppb += error_ppb;
    gpsdo.error_count++;
    if (error_ppb < gpsdo.error_min_ppb) gpsdo.error_min_ppb = error_ppb;
    if (error_ppb > gpsdo.error_max_ppb) gpsdo.error_max_ppb = error_ppb;

    // GPS valid status from gps_fix (from fsm_datalogger.h or equivalent)
    gpsdo.gps_valid = true;   // set false if GPS NMEA silent >5s in caller

    if (!gpsdo.gps_valid) {
        gpsdo.holdover = true;
        gpsdo.holdover_seconds++;
        // In holdover: freeze DAC, let integrator hold
    } else {
        gpsdo.holdover = false;
        gpsdo.holdover_seconds = 0;
    }
}

// ── Compute frequency correction factor from GPSDO ──────────────────────────
// Returns a multiplicative correction to apply to measured frequencies.
// When locked: returns a value close to 1.0 (within ±5 ppm).
// When unlocked: returns 1.0 (no correction applied).
double gpsdo_correction() {
    if (!gpsdo.locked || gpsdo.ref_ticks == 0) return 1.0;
    return (double)REF_NOMINAL_HZ / (double)gpsdo.ref_ticks;
}

// ── Simple NMEA parser for 1 PPS GPS data (standalone version) ──────────────
// Parses $GPRMC to extract time and validity.
// For full GPS parsing (lat/lon/date), see fsm_datalogger.h.
void gpsdo_parse_nmea() {
    while (Serial2.available()) {
        static char buf[96];
        static int  pos = 0;
        char c = (char)Serial2.read();
        if (c == '$') pos = 0;
        if (pos < (int)sizeof(buf) - 1) buf[pos++] = c;
        buf[pos] = '\0';
        if (c == '\n' && pos > 6) {
            if (strncmp(buf, "$GPRMC", 6) == 0) {
                // Extract validity (field 2)
                char* tok = strtok(buf, ",");
                int field = 0;
                while (tok) {
                    if (field == 1) strncpy(gpsdo.gps_time, tok, 10);
                    if (field == 2) gpsdo.gps_valid = (tok[0] == 'A');
                    if (field == 7) gpsdo.gps_satellites = atoi(tok);
                    tok = strtok(NULL, ",");
                    field++;
                }
            }
            pos = 0;
        }
    }
}

// ── Status string for display ─────────────────────────────────────────────────
const char* gpsdo_status_str() {
    if (gpsdo.holdover)   return "HOLDOVER";
    if (!gpsdo.gps_valid) return "GPS WAIT";
    if (gpsdo.locked)     return "LOCKED";
    int pct = (gpsdo.lock_counter * 100) / GPSDO_LOCK_COUNT;
    static char buf[16];
    snprintf(buf, sizeof(buf), "LOCKING %d%%", pct);
    return buf;
}

// ── Accuracy estimate (ppb) ───────────────────────────────────────────────────
double gpsdo_accuracy_ppb() {
    if (!gpsdo.locked)    return 500000.0;  // unlocked: ±0.5 ppm = 500 ppb
    if (gpsdo.holdover)   return 500.0;     // holdover: ±0.5 ppb/hr est.
    // Locked: limited by GPS 1 PPS jitter (~30–100 ns → ~3–10 ppb at 10 MHz)
    return fabsf((float)gpsdo.freq_error_ppm) * 1000.0 + 1.0;
}
