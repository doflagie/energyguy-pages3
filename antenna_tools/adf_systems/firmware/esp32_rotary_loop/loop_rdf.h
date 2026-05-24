#pragma once
// ============================================================
// loop_rdf.h — TM-ADF-005 Rev A
// Motorized Loop RDF — non-blocking stepper + scan engine
// Header-only implementation
// ============================================================

#include <Arduino.h>
#include <math.h>
#include "config.h"

enum LoopMode {
    LOOP_IDLE     = 0,
    LOOP_HOMING   = 1,
    LOOP_SCANNING = 2,
    LOOP_TRACKING = 3,
    LOOP_MANUAL   = 4,
    LOOP_FINE     = 5,
};

struct LoopRDFResult {
    float    bearing_mag;
    float    bearing_true;
    float    null_depth_db;
    float    null_width_deg;
    float    confidence;
    bool     ambiguity_resolved;
    uint32_t timestamp_ms;
};

class LoopRDF {
public:
    LoopRDF(uint8_t step_pin, uint8_t dir_pin, uint8_t en_pin,
            uint8_t pos_adc, uint8_t sig_adc)
        : _step(step_pin), _dir(dir_pin), _en(en_pin),
          _pos_adc(pos_adc), _sig_adc(sig_adc),
          _step_count(0), _position_deg(0.0f), _target_deg(0.0f),
          _mode(LOOP_IDLE), _moving(false), _scan_idx(0),
          _scan_complete(false), _result_ready(false),
          _step_period_us(1250), _last_step_us(0),
          _dir_sign(1), _sig_cal_slope(SIG_CAL_SLOPE),
          _sig_cal_offset(SIG_CAL_OFFSET), _declination(0.0f) {
        memset(_scan_map, 0, sizeof(_scan_map));
        memset(&_result, 0, sizeof(_result));
    }

    void begin() {
        pinMode(_step, OUTPUT); digitalWrite(_step, LOW);
        pinMode(_dir,  OUTPUT); digitalWrite(_dir,  LOW);
        pinMode(_en,   OUTPUT); digitalWrite(_en,   HIGH);  // disabled at start
    }

    void home() {
        _mode     = LOOP_HOMING;
        _moving   = true;
        _dir_sign = -1;
        _setSpeed(HOME_SPEED_SPS);
        _enableMotor(true);
        // Will stop when _step_count reaches 0 (home position)
        // For actual hardware: use limit switch or hard stop + step counting
        // Here: drive to step_count=0 from current position
        _target_deg = 0.0f;
    }

    void moveTo(float deg) {
        while (deg <   0.0f) deg += 360.0f;
        while (deg >= 360.0f) deg -= 360.0f;
        _target_deg = deg;
        float diff  = deg - _position_deg;
        if (diff >  180.0f) diff -= 360.0f;
        if (diff < -180.0f) diff += 360.0f;
        if (fabsf(diff) < DEG_PER_STEP) return;
        _dir_sign = (diff > 0) ? 1 : -1;
        _setSpeed(MAX_STEP_SPS);
        _moving = true;
        _mode   = LOOP_MANUAL;
        _enableMotor(true);
    }

    void moveBy(float deg) {
        moveTo(_position_deg + deg);
    }

    float getPosition() const { return _position_deg; }

    void startFullScan() {
        _scan_idx      = 0;
        _scan_complete = false;
        _result_ready  = false;
        memset(_scan_map, 0, sizeof(_scan_map));
        _mode = LOOP_SCANNING;
        moveTo(0.0f);  // start from 0
    }

    void startNullScan(float approx_bearing) {
        float start = approx_bearing - NULL_WINDOW_DEG / 2.0f;
        moveTo(start);
        _mode = LOOP_FINE;
    }

    void stop() {
        _moving = false;
        _mode   = LOOP_IDLE;
        _enableMotor(false);
    }

    void setDeclinationCorrection(float d) { _declination = d; }

    // Call in main loop — non-blocking stepper tick
    void update() {
        uint32_t now_us = micros();

        // Step if it's time and we're moving
        if (_moving && (now_us - _last_step_us >= _step_period_us)) {
            _last_step_us = now_us;
            _doStep();
        }

        // State machine
        switch (_mode) {
        case LOOP_SCANNING:
            _updateScan();
            break;
        case LOOP_FINE:
            _updateFine();
            break;
        case LOOP_HOMING:
            if (!_moving) {
                _step_count   = 0;
                _position_deg = 0.0f;
                _mode         = LOOP_IDLE;
            }
            break;
        case LOOP_MANUAL:
            // Stop when target reached
            if (_atTarget()) {
                _moving = false;
                _mode   = LOOP_IDLE;
                _enableMotor(false);
            }
            break;
        default: break;
        }
    }

    LoopRDFResult getResult()  const { return _result; }
    bool isResultReady()       const { return _result_ready; }
    void clearResult()               { _result_ready = false; }

    float readSignalLevel() {
        uint32_t sum = 0;
        for (int i = 0; i < 8; i++) sum += analogRead(_sig_adc);
        float adc = (float)(sum / 8);
        return _sig_cal_offset + _sig_cal_slope * adc;
    }

    void calibrateSignalLevel(float known_dbm, uint16_t adc_reading) {
        _sig_cal_offset = known_dbm - _sig_cal_slope * (float)adc_reading;
    }

    void enableSense(bool en) {
        pinMode(GPIO_SENSE_RELAY, OUTPUT);
        digitalWrite(GPIO_SENSE_RELAY, en ? HIGH : LOW);
    }

    void setSensePhase(uint8_t phase_0_or_180) {
        pinMode(GPIO_SENSE_PHASE, OUTPUT);
        digitalWrite(GPIO_SENSE_PHASE, phase_0_or_180 ? HIGH : LOW);
    }

    float resolveAmbiguity(float null_bearing) {
        // Test: null_bearing vs null_bearing+180 by checking cardioid pattern
        // Enable sense with 0° phase: peak should be at (null_bearing + 180)
        // If signal is stronger at null_bearing end, sense is reinforcing from that side
        enableSense(true);
        setSensePhase(0);
        moveTo(null_bearing);
        delay(200);
        float s1 = readSignalLevel();
        moveTo(null_bearing + 180.0f);
        delay(200);
        float s2 = readSignalLevel();
        enableSense(false);
        // Transmitter is in the direction where cardioid peaks (stronger signal)
        return (s2 > s1) ? null_bearing : _wrap(null_bearing + 180.0f);
    }

    // Scan data access
    float getScanValue(uint16_t deg_idx) const {
        if (deg_idx > 359) return 0.0f;
        return _scan_map[deg_idx];
    }

    float getNullBearing() {
        if (!_scan_complete) return -1.0f;
        return _findNullInScan();
    }

    float getPeakBearing() {
        float peak_val = -200.0f; uint16_t peak_idx = 0;
        for (uint16_t i = 0; i < 360; i++) {
            if (_scan_map[i] > peak_val) { peak_val = _scan_map[i]; peak_idx = i; }
        }
        return (float)peak_idx;
    }

    void autoTune() {
        if (_mode != LOOP_IDLE) return;
        float best_v = 0.0f; float best_sig = -200.0f;
        for (uint8_t step = 0; step < 32; step++) {
            float v = (float)step * 3.3f / 32.0f;
            dacWrite(GPIO_TUNE_DAC, (uint8_t)(v / 3.3f * 255.0f));
            delay(20);
            float s = readSignalLevel();
            if (s > best_sig) { best_sig = s; best_v = v; }
        }
        dacWrite(GPIO_TUNE_DAC, (uint8_t)(best_v / 3.3f * 255.0f));
    }

    void setTuneVoltage(float v) {
        uint8_t dac_val = (uint8_t)constrain(v / 3.3f * 255.0f, 0.0f, 255.0f);
        dacWrite(GPIO_TUNE_DAC, dac_val);
    }

private:
    uint8_t  _step, _dir, _en, _pos_adc, _sig_adc;
    int32_t  _step_count;
    float    _position_deg, _target_deg;
    LoopMode _mode;
    bool     _moving;
    uint16_t _scan_idx;
    bool     _scan_complete, _result_ready;
    uint32_t _step_period_us, _last_step_us;
    int8_t   _dir_sign;
    float    _scan_map[360];   // signal level at each degree
    LoopRDFResult _result;
    float    _sig_cal_slope, _sig_cal_offset, _declination;

    // Scan state
    float    _scan_pos   = 0.0f;
    float    _fine_start = 0.0f;
    bool     _at_scan_position = false;
    uint32_t _dwell_start = 0;

    void _doStep() {
        digitalWrite(_dir, _dir_sign > 0 ? HIGH : LOW);
        digitalWrite(_step, HIGH);
        delayMicroseconds(2);
        digitalWrite(_step, LOW);
        _step_count += _dir_sign;
        _position_deg = (float)(_step_count % (int32_t)STEPS_PER_LOOP_REV)
                        * DEG_PER_STEP;
        if (_position_deg < 0) _position_deg += 360.0f;
    }

    void _setSpeed(uint32_t sps) {
        _step_period_us = (sps > 0) ? 1000000UL / sps : 10000UL;
    }

    void _enableMotor(bool en) {
        digitalWrite(_en, en ? LOW : HIGH);
    }

    bool _atTarget() {
        float diff = fabsf(_position_deg - _target_deg);
        if (diff > 180.0f) diff = 360.0f - diff;
        return diff < DEG_PER_STEP * 2;
    }

    void _updateScan() {
        // State: move to _scan_pos, dwell, record, advance
        if (!_at_scan_position) {
            if (_atTarget()) {
                _at_scan_position = true;
                _dwell_start = millis();
            }
            return;
        }
        // Dwell
        if (millis() - _dwell_start < SCAN_DWELL_MS) return;
        // Record signal level at this position
        uint16_t idx = (uint16_t)(fmod(_scan_pos + 360.0f, 360.0f));
        if (idx > 359) idx = 359;
        _scan_map[idx] = readSignalLevel();
        // Advance
        _scan_pos += SCAN_STEP_DEG;
        if (_scan_pos >= 360.0f) {
            // Scan complete
            _scan_complete = true;
            _mode = LOOP_IDLE;
            _moving = false;
            _enableMotor(false);
            _buildResult();
        } else {
            _at_scan_position = false;
            moveTo(_scan_pos);
        }
    }

    void _updateFine() {
        // Fine scan: SCAN_STEP_DEG/4 steps within NULL_WINDOW_DEG
        static float fine_pos = 0;
        static bool fine_init = false;
        if (!fine_init) {
            fine_pos  = _fine_start;
            fine_init = true;
            moveTo(fine_pos);
        }
        if (!_at_scan_position) {
            if (_atTarget()) { _at_scan_position = true; _dwell_start = millis(); }
            return;
        }
        if (millis() - _dwell_start < 100) return;
        uint16_t idx = (uint16_t)fmod(fine_pos + 360.0f, 360.0f);
        if (idx > 359) idx = 359;
        _scan_map[idx] = readSignalLevel();
        fine_pos += FINE_STEP_DEG;
        if (fine_pos >= _fine_start + NULL_WINDOW_DEG) {
            fine_init = false;
            _scan_complete = true;
            _mode = LOOP_IDLE;
            _moving = false;
            _enableMotor(false);
            _buildResult();
        } else {
            _at_scan_position = false;
            moveTo(fine_pos);
        }
    }

    void _buildResult() {
        float null_brg = _findNullInScan();
        float peak_brg = getPeakBearing();
        float null_val = _scan_map[(uint16_t)null_brg];
        float peak_val = _scan_map[(uint16_t)peak_brg];
        float depth = peak_val - null_val;

        _result.bearing_mag  = null_brg;
        _result.bearing_true = _wrap(null_brg + _declination);
        _result.null_depth_db = depth;
        _result.null_width_deg = _measureNullWidth(null_brg, null_val + 10.0f);
        _result.confidence   = fminf(depth / 30.0f, 1.0f);
        _result.timestamp_ms = millis();
        _result_ready = true;
    }

    float _findNullInScan() {
        float min_val = 1e9f; uint16_t min_idx = 0;
        for (uint16_t i = 0; i < 360; i++) {
            if (_scan_map[i] != 0.0f && _scan_map[i] < min_val) {
                min_val = _scan_map[i]; min_idx = i;
            }
        }
        // Gaussian sub-degree interpolation
        uint16_t L = (min_idx == 0)   ? 359 : min_idx - 1;
        uint16_t R = (min_idx == 359) ?   0 : min_idx + 1;
        float vl = _scan_map[L], vc = _scan_map[min_idx], vr = _scan_map[R];
        float denom = vl - 2*vc + vr;
        float offset = (fabsf(denom) > 0.001f) ? 0.5f*(vl-vr)/denom : 0.0f;
        return _wrap((float)min_idx + offset);
    }

    float _measureNullWidth(float null_deg, float threshold_dbm) {
        float w = 0.0f;
        for (float d = -90.0f; d <= 90.0f; d += 1.0f) {
            uint16_t idx = (uint16_t)_wrap(null_deg + d);
            if (_scan_map[idx] < threshold_dbm) w += 1.0f;
        }
        return w;
    }

    static float _wrap(float a) {
        while (a <   0.0f) a += 360.0f;
        while (a >= 360.0f) a -= 360.0f;
        return a;
    }
};
