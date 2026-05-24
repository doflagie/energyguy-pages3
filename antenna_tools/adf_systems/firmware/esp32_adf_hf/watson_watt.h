#pragma once
// ============================================================
// watson_watt.h — TM-ADF-001 Rev A
// Watson-Watt / Adcock direction finding algorithm
// Header-only implementation
// ============================================================

#include <Arduino.h>
#include <math.h>

struct WatsonWattResult {
    float   bearing_mag;          // Magnetic bearing, 0–360°
    float   bearing_true;         // True bearing after declination
    float   confidence;           // 0.0–1.0
    float   signal_ns;            // RMS signal N-S pair (ADC counts)
    float   signal_ew;            // RMS signal E-W pair
    float   signal_sense;         // Sense antenna RMS
    float   snr_db;               // Estimated SNR in dB
    bool    ambiguity_resolved;   // true = sense antenna used
    uint32_t timestamp_ms;
};

class WatsonWatt {
public:
    WatsonWatt()
        : _declination(0.0f), _array_offset(0.0f), _min_signal(100),
          _ns_gain(1.0f), _ew_gain(1.0f), _phase_corr(0.0f),
          _history_head(0), _history_count(0), _smoothed(-1.0f) {}

    void setMagneticDeclination(float decl_deg)   { _declination = decl_deg; }
    void setArrayOrientation(float north_offset)   { _array_offset = north_offset; }
    void setMinSignalLevel(uint16_t min_adc)       { _min_signal = min_adc; }

    void calibrateBalance(float ns_gain, float ew_gain) {
        _ns_gain = ns_gain;
        _ew_gain = ew_gain;
    }
    void calibratePhase(float phase_deg) {
        _phase_corr = phase_deg * (float)M_PI / 180.0f;
    }

    // Main bearing computation
    // ns_samples, ew_samples, sense_samples: arrays of ADC readings (signed relative to midpoint)
    // count: number of samples in each array
    WatsonWattResult compute(const int16_t* ns_buf, const int16_t* ew_buf,
                              const int16_t* sense_buf, size_t count) {
        WatsonWattResult r;
        r.timestamp_ms = millis();
        r.ambiguity_resolved = false;

        // Compute RMS of each channel
        r.signal_ns    = _rms(ns_buf, count) * _ns_gain;
        r.signal_ew    = _rms(ew_buf, count) * _ew_gain;
        r.signal_sense = _rms(sense_buf, count);

        // Check minimum signal
        float total_sig = sqrtf(r.signal_ns * r.signal_ns + r.signal_ew * r.signal_ew);
        if (total_sig < (float)_min_signal) {
            r.bearing_mag  = -1.0f;  // invalid
            r.bearing_true = -1.0f;
            r.confidence   = 0.0f;
            r.snr_db       = 0.0f;
            return r;
        }

        // Estimate noise floor from minimum of the two channels
        float noise_est = 50.0f;  // ADC noise floor estimate (counts)
        r.snr_db = 20.0f * log10f(total_sig / noise_est);

        // Watson-Watt bearing: atan2(NS, EW)
        // Sign of NS and EW from sign of instantaneous sample at peak (use first sample as proxy)
        // For proper 4-quadrant: use signed amplitude (keep sign information)
        float signed_ns = (ns_buf[0] >= 0 ? r.signal_ns : -r.signal_ns);
        float signed_ew = (ew_buf[0] >= 0 ? r.signal_ew : -r.signal_ew);

        float bearing_rad = atan2f(signed_ns, signed_ew);

        // Convert to degrees, normalize 0–360
        float bearing_deg = bearing_rad * 180.0f / (float)M_PI;
        bearing_deg = _wrap360(bearing_deg);

        // Apply array orientation offset and phase correction
        bearing_deg += _array_offset + (_phase_corr * 180.0f / (float)M_PI);
        bearing_deg = _wrap360(bearing_deg);

        // Ambiguity resolution via sense antenna
        // Test: does adding sense reinforce signal_ns or signal_ew?
        if (r.signal_sense > (float)_min_signal * 0.5f) {
            float bearing_with_sense = resolveAmbiguity(bearing_deg,
                                                         r.signal_ns, r.signal_sense, 0.0f);
            bearing_deg = bearing_with_sense;
            r.ambiguity_resolved = true;
        }

        r.bearing_mag  = bearing_deg;
        r.bearing_true = _wrap360(bearing_deg + _declination);
        r.confidence   = fminf(r.snr_db / 20.0f, 1.0f);
        if (r.confidence < 0.0f) r.confidence = 0.0f;

        // Update history and smoothed bearing
        _history[_history_head] = r.bearing_mag;
        _history_head = (_history_head + 1) % WW_HISTORY_SIZE;
        if (_history_count < WW_HISTORY_SIZE) _history_count++;

        if (_smoothed < 0.0f) {
            _smoothed = r.bearing_mag;
        } else {
            // Circular EMA: handle 0°/360° wrap
            float diff = r.bearing_mag - _smoothed;
            if (diff >  180.0f) diff -= 360.0f;
            if (diff < -180.0f) diff += 360.0f;
            _smoothed = _wrap360(_smoothed + WW_SMOOTH_ALPHA * diff);
        }

        return r;
    }

    float resolveAmbiguity(float raw_bearing, float sig_rms,
                           float sense_rms, float sense_phase_deg) {
        // Two candidate bearings: raw and raw+180
        float b1 = raw_bearing;
        float b2 = _wrap360(raw_bearing + 180.0f);

        // For b1: sense adds constructively if bearing matches signal direction
        // For b2: sense subtracts
        // Simple heuristic: the bearing where sense reinforces gives higher total
        float strength1 = sig_rms + sense_rms * cosf((b1 - raw_bearing) * (float)M_PI / 180.0f);
        float strength2 = sig_rms + sense_rms * cosf((b2 - raw_bearing) * (float)M_PI / 180.0f);

        return (strength1 >= strength2) ? b1 : b2;
    }

    void resetHistory() {
        _history_head = 0;
        _history_count = 0;
        _smoothed = -1.0f;
    }

    float getSmoothedBearing() const { return _smoothed; }

    float getBearingStdDev() const {
        if (_history_count < 2) return 0.0f;
        float mean = _circMean(_history, _history_count);
        float sum_sq = 0.0f;
        for (uint8_t i = 0; i < _history_count; i++) {
            float d = _history[i] - mean;
            if (d >  180.0f) d -= 360.0f;
            if (d < -180.0f) d += 360.0f;
            sum_sq += d * d;
        }
        return sqrtf(sum_sq / (float)(_history_count - 1));
    }

    uint8_t getHistoryCount() const { return _history_count; }

private:
    float   _declination;
    float   _array_offset;
    uint16_t _min_signal;
    float   _ns_gain;
    float   _ew_gain;
    float   _phase_corr;

    static const uint8_t WW_HISTORY_SIZE = 32;
    static constexpr float WW_SMOOTH_ALPHA = 0.3f;

    float   _history[32];
    uint8_t _history_head;
    uint8_t _history_count;
    float   _smoothed;

    static float _rms(const int16_t* buf, size_t n) {
        if (n == 0) return 0.0f;
        float sum = 0.0f;
        for (size_t i = 0; i < n; i++) {
            float v = (float)buf[i];
            sum += v * v;
        }
        return sqrtf(sum / (float)n);
    }

    static float _wrap360(float a) {
        while (a <   0.0f) a += 360.0f;
        while (a >= 360.0f) a -= 360.0f;
        return a;
    }

    static float _circMean(const float* a, uint8_t n) {
        float sx = 0.0f, sy = 0.0f;
        for (uint8_t i = 0; i < n; i++) {
            sx += cosf(a[i] * (float)M_PI / 180.0f);
            sy += sinf(a[i] * (float)M_PI / 180.0f);
        }
        float m = atan2f(sy, sx) * 180.0f / (float)M_PI;
        return _wrap360(m);
    }
};
