/**
 * calibration.h — TM-PWR-ESP32-001 Rev A
 * RF Power Meter Calibration Data, Temperature Compensation, NVS Storage
 */

#pragma once
#include <Arduino.h>
#include <Preferences.h>
#include <math.h>

// ─── Calibration Store ────────────────────────────────────────────────────────

struct CalData {
    float fwd_zero_mv;      // Vfwd ADC at zero RF (diode offset)
    float ref_zero_mv;      // Vref ADC at zero RF
    float fwd_scale;        // Forward path scale factor (1.0 nominal)
    float ref_scale;        // Reflected path scale factor
    float freq_mhz;         // Frequency at last calibration
    bool  valid;            // Calibration performed flag

    CalData() : fwd_zero_mv(0), ref_zero_mv(0),
                fwd_scale(1.0f), ref_scale(1.0f),
                freq_mhz(0), valid(false) {}
};

// ─── UHF Dispersion Correction Table ──────────────────────────────────────────
// Measured coupling vs. frequency for coupled microstrip coupler
// Update after measuring your specific PCB on a VNA
struct DispEntry { float freq_mhz; float coupling_db; };

static const DispEntry UHF_DISP[] = {
    { 420.0f, -26.2f }, { 500.0f, -25.8f }, { 600.0f, -25.4f },
    { 700.0f, -25.1f }, { 800.0f, -25.0f }, { 860.0f, -25.0f },
    { 900.0f, -24.8f }, {1000.0f, -24.5f }, {1100.0f, -24.1f },
    {1200.0f, -23.8f }, {1296.0f, -23.5f },
};
static const int UHF_DISP_N = sizeof(UHF_DISP)/sizeof(DispEntry);


// ─── Calibration Class ────────────────────────────────────────────────────────

class PowerCalibration {
public:
    CalData cal;

    void load(Preferences& p) {
        cal.fwd_zero_mv = p.getFloat(NVS_KEY_FWD_ZERO, 0.0f);
        cal.ref_zero_mv = p.getFloat(NVS_KEY_REF_ZERO, 0.0f);
        cal.fwd_scale   = p.getFloat(NVS_KEY_FWD_SCALE, 1.0f);
        cal.ref_scale   = p.getFloat(NVS_KEY_REF_SCALE, 1.0f);
        cal.freq_mhz    = p.getFloat(NVS_KEY_FREQ_MHZ,  14.0f);
        cal.valid       = p.getBool (NVS_KEY_CAL_VALID,  false);
    }

    void save(Preferences& p) {
        p.putFloat(NVS_KEY_FWD_ZERO,  cal.fwd_zero_mv);
        p.putFloat(NVS_KEY_REF_ZERO,  cal.ref_zero_mv);
        p.putFloat(NVS_KEY_FWD_SCALE, cal.fwd_scale);
        p.putFloat(NVS_KEY_REF_SCALE, cal.ref_scale);
        p.putFloat(NVS_KEY_FREQ_MHZ,  cal.freq_mhz);
        p.putBool (NVS_KEY_CAL_VALID, true);
        cal.valid = true;
    }

    // Zero calibration — call with no RF applied
    void calibrateZero(float raw_fwd_mv, float raw_ref_mv) {
        cal.fwd_zero_mv = raw_fwd_mv;
        cal.ref_zero_mv = raw_ref_mv;
    }

    // Scale calibration — call with known reference power into 50Ω load
    // known_w: actual power applied (Watts), measured by reference standard
    void calibrateScale(float raw_fwd_mv, float known_w,
                        float raw_ref_mv = 0.0f) {
        float corrected = raw_fwd_mv - cal.fwd_zero_mv;
        if (corrected > 0.001f && known_w > 0.0f) {
            // Expected V_det for known power:
            // P_coupled = known_w × 10^(COUPLING_DB/10)
            // V_expected = sqrt(P_coupled × 50) × 1000 [mV]
            float p_coupled = known_w * powf(10.0f, COUPLING_DB / 10.0f);
            float v_expected = sqrtf(p_coupled * 50.0f) * 1000.0f;
            cal.fwd_scale = v_expected / corrected;
        }
        // Ref channel: with matched load, ref should be near zero
        float corr_ref = raw_ref_mv - cal.ref_zero_mv;
        if (fabsf(corr_ref) > 0.5f)
            cal.ref_scale = cal.fwd_scale;  // assume matched detectors
    }

    // Apply calibration to raw ADC reading (returns calibrated mV)
    float applyFwd(float raw_mv) const {
        float v = (raw_mv - cal.fwd_zero_mv) * cal.fwd_scale;
        return (v < 0) ? 0.0f : v;
    }
    float applyRef(float raw_mv) const {
        float v = (raw_mv - cal.ref_zero_mv) * cal.ref_scale;
        return (v < 0) ? 0.0f : v;
    }

    // Temperature correction for Schottky detector
    // temp_c: measured temperature from NTC thermistor
    // Returns: multiplicative correction factor (1.0 at cal temperature)
    static float tempCorrection(float temp_c, float cal_temp_c = 25.0f,
                                 float alpha = 0.023f) {
        return expf(alpha * (temp_c - cal_temp_c));
    }

    // NTC thermistor temperature from ADC millivolts
    // Voltage divider: 3.3V → R_bias (10kΩ) → NTC → GND
    static float ntcTempC(float v_mv, float vcc_mv = 3300.0f,
                           float r_bias = 10000.0f,
                           float r25 = 10000.0f, float beta = 3950.0f) {
        if (v_mv <= 0.0f || v_mv >= vcc_mv) return 25.0f;
        float r_ntc = r_bias * (v_mv / (vcc_mv - v_mv));
        float inv_t = (1.0f/298.15f) + (1.0f/beta) * logf(r_ntc / r25);
        return (1.0f / inv_t) - 273.15f;
    }

    // UHF: interpolated coupling correction (dB offset from nominal)
    static float uhfCouplingDB(float freq_mhz) {
        if (freq_mhz <= UHF_DISP[0].freq_mhz) return UHF_DISP[0].coupling_db;
        if (freq_mhz >= UHF_DISP[UHF_DISP_N-1].freq_mhz)
            return UHF_DISP[UHF_DISP_N-1].coupling_db;
        for (int i = 0; i < UHF_DISP_N-1; i++) {
            if (freq_mhz >= UHF_DISP[i].freq_mhz &&
                freq_mhz <= UHF_DISP[i+1].freq_mhz) {
                float t = (freq_mhz - UHF_DISP[i].freq_mhz) /
                          (UHF_DISP[i+1].freq_mhz - UHF_DISP[i].freq_mhz);
                return UHF_DISP[i].coupling_db
                     + t*(UHF_DISP[i+1].coupling_db - UHF_DISP[i].coupling_db);
            }
        }
        return COUPLING_DB;
    }
};


// ─── Power Computation ────────────────────────────────────────────────────────

struct PowerReading {
    float fwd_w;        // Forward power, Watts
    float ref_w;        // Reflected power, Watts
    float peak_w;       // Peak power (peak detector channel), Watts
    float swr;          // SWR (1.0 = perfect match)
    float rl_db;        // Return loss, dB (positive = good)
    float gamma;        // Reflection coefficient magnitude
    float efficiency;   // Power delivery efficiency (%)
    float vfwd_mv;      // Calibrated Vfwd (mV)
    float vref_mv;      // Calibrated Vref (mV)
    float vpeak_mv;     // Peak detector voltage (mV)
    float temp_c;       // Temperature
    uint32_t ts_ms;     // Timestamp millis()
    int  range;         // Active range index
    bool valid;         // True if meaningful signal present
};

// Convert detector millivolt → Watts
// v_mv     : calibrated detector output voltage (mV)
// coupling : coupler coupling factor (dB, negative)
// atten    : attenuator pad in path (dB, positive)
inline float mvToWatts(float v_mv, float coupling_db, float atten_db) {
    if (v_mv < 1.0f) return 0.0f;
    float v_v = v_mv / 1000.0f;
    float p_coupled = (v_v * v_v) / 50.0f;
    float c_lin = powf(10.0f, (-coupling_db) / 10.0f);
    float a_lin = powf(10.0f, (atten_db)     / 10.0f);
    return p_coupled * c_lin * a_lin;
}

// Compute SWR from forward and reflected power
inline float computeSWR(float p_fwd, float p_ref) {
    if (p_fwd < 1e-6f) return 0.0f;
    float ratio = p_ref / p_fwd;
    if (ratio >= 1.0f) return SWR_MAX;
    float gamma = sqrtf(ratio);
    float swr = (1.0f + gamma) / (1.0f - gamma);
    return (swr > SWR_MAX) ? SWR_MAX : swr;
}

// Rolling average buffer
template<int N>
class RollingAvg {
    float buf[N];
    int   idx = 0;
    int   count = 0;
public:
    void push(float v) {
        buf[idx] = v;
        idx = (idx + 1) % N;
        if (count < N) count++;
    }
    float avg() const {
        if (count == 0) return 0.0f;
        float s = 0;
        for (int i = 0; i < count; i++) s += buf[i];
        return s / count;
    }
    void reset() { idx = 0; count = 0; }
    int  size()  const { return count; }
};
