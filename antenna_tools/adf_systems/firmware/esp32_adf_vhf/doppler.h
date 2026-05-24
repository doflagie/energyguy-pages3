#pragma once
// ============================================================
// doppler.h — TM-ADF-002 Rev A
// Doppler RDF algorithm — Goertzel phase extraction
// Header-only implementation
// ============================================================

#include <Arduino.h>
#include <math.h>

struct DopplerResult {
    float    bearing_mag;        // 0–360° magnetic
    float    bearing_true;       // After declination
    float    confidence;         // 0.0–1.0
    float    rssi_dbm;           // Received signal estimate
    float    phase_error_deg;    // Phase noise estimate
    bool     valid;
    uint32_t timestamp_ms;
};

class DopplerRDF {
public:
    DopplerRDF()
        : _sw_a0(0), _sw_a1(0), _sw_en(0), _switch_hz(1000),
          _declination(0.0f), _array_north(0.0f), _cal_offset(0.0f),
          _hist_head(0), _hist_count(0), _smoothed(-1.0f),
          _cal_count(0) {}

    void begin(uint8_t sw_a0, uint8_t sw_a1, uint8_t sw_en, uint32_t switch_hz = 1000) {
        _sw_a0 = sw_a0; _sw_a1 = sw_a1; _sw_en = sw_en;
        _switch_hz = switch_hz;
        pinMode(_sw_a0, OUTPUT);
        pinMode(_sw_a1, OUTPUT);
        pinMode(_sw_en, OUTPUT);
        digitalWrite(_sw_en, LOW);  // enable switch
    }

    void setDeclinationCorrection(float decl_deg)  { _declination = decl_deg; }
    void setArrayNorthOffset(float offset_deg)     { _array_north = offset_deg; }
    float getCalibrationOffset() const             { return _cal_offset; }

    // Select antenna element (0–3 = N, E, S, W)
    void selectElement(uint8_t el) {
        digitalWrite(_sw_a0, el & 0x01);
        digitalWrite(_sw_a1, (el >> 1) & 0x01);
    }

    // Goertzel algorithm — extracts magnitude and phase of target_hz
    // from buffer buf[] of length n sampled at sample_rate Hz
    void goertzel(const int16_t* buf, size_t n, float sample_rate,
                  float target_hz, float& magnitude, float& phase_deg) {
        float   omega   = 2.0f * (float)M_PI * target_hz / sample_rate;
        float   coeff   = 2.0f * cosf(omega);
        float   s0 = 0.0f, s1 = 0.0f, s2 = 0.0f;

        for (size_t i = 0; i < n; i++) {
            s0 = (float)buf[i] + coeff * s1 - s2;
            s2 = s1;
            s1 = s0;
        }

        float real = s1 - s2 * cosf(omega);
        float imag = s2 * sinf(omega);
        magnitude  = sqrtf(real * real + imag * imag) / ((float)n * 0.5f);
        phase_deg  = atan2f(imag, real) * 180.0f / (float)M_PI;
        if (phase_deg < 0.0f) phase_deg += 360.0f;
    }

    // IQ phase extraction (alternative to Goertzel for real-time use)
    float extractPhase_IQ(const int16_t* buf, size_t n, float sample_rate, float ref_hz) {
        float I = 0.0f, Q = 0.0f;
        float omega = 2.0f * (float)M_PI * ref_hz / sample_rate;
        for (size_t i = 0; i < n; i++) {
            float t = (float)i * omega;
            I += (float)buf[i] * cosf(t);
            Q += (float)buf[i] * sinf(t);
        }
        float phase = atan2f(Q, I) * 180.0f / (float)M_PI;
        if (phase < 0.0f) phase += 360.0f;
        return phase;
    }

    // Convert phase angle to bearing
    float phaseToBearing(float phase_deg, float north_offset = 0.0f) {
        float b = phase_deg + north_offset + _cal_offset;
        b += _array_north;
        while (b <   0.0f) b += 360.0f;
        while (b >= 360.0f) b -= 360.0f;
        return b;
    }

    // Full bearing computation from audio buffer
    DopplerResult compute(const int16_t* audio_buf, size_t n, float sample_rate) {
        DopplerResult r;
        r.timestamp_ms = millis();
        r.valid        = false;

        // Check signal level first (RMS of audio)
        float rms = 0.0f;
        for (size_t i = 0; i < n; i++) rms += (float)audio_buf[i] * (float)audio_buf[i];
        rms = sqrtf(rms / (float)n);

        if (rms < DOPPLER_MIN_RSSI) {
            r.confidence = 0.0f;
            r.rssi_dbm   = -120.0f;
            return r;
        }

        // Goertzel at switch frequency
        float mag, phase;
        goertzel(audio_buf, n, sample_rate, (float)_switch_hz, mag, phase);

        // Bearing from phase
        float bearing = phaseToBearing(phase, 0.0f);

        // Apply declination
        r.bearing_mag  = bearing;
        r.bearing_true = bearing + _declination;
        while (r.bearing_true <   0.0f) r.bearing_true += 360.0f;
        while (r.bearing_true >= 360.0f) r.bearing_true -= 360.0f;

        // Confidence from magnitude relative to total RMS
        r.confidence = fminf(mag / (rms * 0.1f + 1.0f), 1.0f);
        r.rssi_dbm   = 20.0f * log10f(rms / 2048.0f + 1e-6f) - 10.0f;

        // Phase noise estimate (std dev from multiple sub-blocks)
        r.phase_error_deg = _estimatePhaseNoise(audio_buf, n, sample_rate);

        r.valid = (r.confidence > 0.2f);

        // Update smoothed bearing
        if (_smoothed < 0.0f) {
            _smoothed = bearing;
        } else {
            float diff = bearing - _smoothed;
            if (diff >  180.0f) diff -= 360.0f;
            if (diff < -180.0f) diff += 360.0f;
            _smoothed += 0.3f * diff;
            while (_smoothed <   0.0f) _smoothed += 360.0f;
            while (_smoothed >= 360.0f) _smoothed -= 360.0f;
        }

        _bearing_history[_hist_head] = bearing;
        _hist_head = (_hist_head + 1) % 32;
        if (_hist_count < 32) _hist_count++;

        return r;
    }

    float getSmoothedBearing() const { return _smoothed; }

    void recordCalibrationPoint(float known_bearing, float measured_phase) {
        if (_cal_count < 8) {
            _cal_pts[_cal_count].bearing = known_bearing;
            _cal_pts[_cal_count].phase   = measured_phase;
            _cal_count++;
        }
    }

    void computeCalibration() {
        if (_cal_count < 2) return;
        // Average the offset between known bearing and measured phase
        float sum = 0.0f;
        for (uint8_t i = 0; i < _cal_count; i++) {
            float d = _cal_pts[i].bearing - _cal_pts[i].phase;
            while (d >  180.0f) d -= 360.0f;
            while (d < -180.0f) d += 360.0f;
            sum += d;
        }
        _cal_offset = sum / (float)_cal_count;
    }

private:
    uint8_t  _sw_a0, _sw_a1, _sw_en;
    uint32_t _switch_hz;
    float    _declination, _array_north, _cal_offset;
    float    _bearing_history[32];
    uint8_t  _hist_head, _hist_count;
    float    _smoothed;

    struct CalPoint { float bearing; float phase; };
    CalPoint _cal_pts[8];
    uint8_t  _cal_count;

    // Estimate phase noise by splitting block into 4 sub-blocks
    float _estimatePhaseNoise(const int16_t* buf, size_t n, float fs) {
        if (n < 256) return 10.0f;
        size_t sub = n / 4;
        float phases[4];
        for (int k = 0; k < 4; k++) {
            float mag, ph;
            goertzel(buf + k*sub, sub, fs, (float)_switch_hz, mag, ph);
            phases[k] = ph;
        }
        float mean = 0;
        for (int k = 0; k < 4; k++) mean += phases[k];
        mean /= 4.0f;
        float var = 0;
        for (int k = 0; k < 4; k++) {
            float d = phases[k] - mean;
            if (d >  180.0f) d -= 360.0f;
            if (d < -180.0f) d += 360.0f;
            var += d * d;
        }
        return sqrtf(var / 3.0f);
    }
};
