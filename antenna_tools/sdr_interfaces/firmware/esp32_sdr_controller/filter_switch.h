#pragma once
// filter_switch.h — TM-SDR-FW-001 Rev A
// FilterSwitch class: preselector, attenuator, LNA, and relay control

#include <Arduino.h>
#include <SPI.h>
#include "config.h"

class FilterSwitch {
public:
    // ── Initialization ───────────────────────────────────────────────
    void begin() {
        // GPIO init
        pinMode(GPIO_BPF_A0,   OUTPUT);
        pinMode(GPIO_BPF_A1,   OUTPUT);
        pinMode(GPIO_BPF_A2,   OUTPUT);
        pinMode(GPIO_BPF_A3,   OUTPUT);
        pinMode(GPIO_BPF_EN,   OUTPUT);
        pinMode(GPIO_ATT_LE,   OUTPUT);
        pinMode(GPIO_LNA,      OUTPUT);
        pinMode(GPIO_BYPASS,   OUTPUT);
        pinMode(GPIO_BIASTEE,  OUTPUT);
        pinMode(GPIO_BPFSEL,   OUTPUT);

        // All relays off, all outputs low, BPF disabled
        digitalWrite(GPIO_BPF_EN,   LOW);
        digitalWrite(GPIO_ATT_LE,   LOW);
        digitalWrite(GPIO_LNA,      LOW);
        digitalWrite(GPIO_BYPASS,   LOW);
        digitalWrite(GPIO_BIASTEE,  LOW);
        digitalWrite(GPIO_BPFSEL,   LOW);

        // ADC configuration
        analogSetAttenuation(ADC_11db);   // 0–3.3V range
        analogReadResolution(12);

        // SPI init (VSPI, 1 MHz, mode 0, LSB first)
        SPI.begin(GPIO_ATT_CLK, -1, GPIO_ATT_DATA, -1);

        // Set 0 dB attenuation, disable LNA as safe startup state
        _att_db     = 0;
        _lna_en     = false;
        _bypass_en  = false;
        _biastee_en = false;
        _bpf_code   = 15;      // bypass filter on startup
        _alc_en     = false;
        _band_idx   = 2;       // default 40M

        _setAttenuator(0);
        _setBPF(15);           // bypass filter
    }

    // ── Band selection ────────────────────────────────────────────────
    void selectBand(uint8_t idx) {
        if (idx >= NUM_BANDS) return;
        _band_idx = idx;
        const BandConfig &b = BANDS[idx];

        // Configure preselector
        _setBPF(b.bpf_code);
        digitalWrite(GPIO_BPFSEL, (b.bpf_code < 15) ? HIGH : LOW);

        // Configure upconverter bypass
        // bypass = LOW = upconverter active (relay deenergized = upconv path)
        // bypass = HIGH = direct path (relay energized = skip upconv)
        bool direct = !b.use_upconv;
        setBypass(direct);

        // Set default attenuation and LNA for this band
        _setAttenuator(b.default_att);
        setLNA(b.lna_default);
    }

    void selectBandByName(const char *name) {
        for (uint8_t i = 0; i < NUM_BANDS; i++) {
            if (strcasecmp(BANDS[i].name, name) == 0) {
                selectBand(i);
                return;
            }
        }
    }

    uint8_t getBandIndex() const { return _band_idx; }
    const BandConfig &getBand() const { return BANDS[_band_idx]; }

    // ── Attenuation ───────────────────────────────────────────────────
    // att_db: 0–31 dB in 1 dB steps (internally uses 0.5 dB PE4302 steps)
    void setAttenuation(int8_t att_db) {
        att_db = constrain(att_db, ATT_MIN_DB, ATT_MAX_DB);
        _att_db = att_db;
        _setAttenuator(att_db);
    }

    void stepAttenuation(int8_t delta_db) {
        setAttenuation(_att_db + delta_db);
    }

    int8_t getAttenuation() const { return _att_db; }

    // ── LNA ───────────────────────────────────────────────────────────
    void setLNA(bool enable) {
        _lna_en = enable;
        digitalWrite(GPIO_LNA, enable ? HIGH : LOW);
    }

    bool getLNA() const { return _lna_en; }

    // ── Bypass (upconverter) ──────────────────────────────────────────
    void setBypass(bool bypass) {
        _bypass_en = bypass;
        digitalWrite(GPIO_BYPASS, bypass ? HIGH : LOW);
    }

    bool getBypass() const { return _bypass_en; }

    // ── Bias-T ────────────────────────────────────────────────────────
    void setBiasT(bool enable) {
        _biastee_en = enable;
        digitalWrite(GPIO_BIASTEE, enable ? HIGH : LOW);
    }

    bool getBiasT() const { return _biastee_en; }

    // ── RSSI readback ────────────────────────────────────────────────
    // Returns instantaneous RSSI in dBm from AD8307
    float readRSSI_dBm() {
        // Average 8 samples for stability
        uint32_t sum = 0;
        for (int i = 0; i < 8; i++) {
            sum += analogRead(GPIO_RSSI);
            delayMicroseconds(200);
        }
        float counts = sum / 8.0f;
        float v_adc = counts * (3.3f / 4095.0f);
        return (v_adc / AD8307_SLOPE_V_PER_DB) + AD8307_INTERCEPT_DBM;
    }

    // ── ALC (automatic level control) ────────────────────────────────
    void setALC(bool enable) { _alc_en = enable; }
    bool getALC() const { return _alc_en; }

    // Call periodically (every ALC_POLL_MS) when ALC is enabled
    void updateALC() {
        if (!_alc_en) return;
        float rssi = readRSSI_dBm();
        _last_rssi = rssi;

        if (rssi > ALC_UPPER_DBM + ALC_HYSTERESIS_DB) {
            // Overload: increase attenuation by 3 dB
            if (_att_db < ATT_MAX_DB) {
                setAttenuation(_att_db + 3);
            } else if (_lna_en) {
                // Max attenuation and still overloaded: disable LNA
                setLNA(false);
            }
        } else if (rssi < ALC_LOWER_DBM - ALC_HYSTERESIS_DB) {
            // Underload: decrease attenuation by 3 dB
            if (_att_db > ATT_MIN_DB) {
                setAttenuation(_att_db - 3);
            } else if (!_lna_en) {
                // Zero attenuation and still weak: enable LNA
                setLNA(true);
            }
        }
    }

    float getLastRSSI() const { return _last_rssi; }

    // ── Status string ────────────────────────────────────────────────
    // Fills buf with JSON status packet
    void getStatusJSON(char *buf, size_t len) {
        const BandConfig &b = BANDS[_band_idx];
        snprintf(buf, len,
            "{\"band\":\"%s\",\"freq_center\":%lu,\"att\":%d,"
            "\"lna\":%s,\"bypass\":%s,\"biastee\":%s,"
            "\"alc\":%s,\"rssi\":%.1f,\"bpf_code\":%d}",
            b.name, b.freq_center, _att_db,
            _lna_en     ? "true" : "false",
            _bypass_en  ? "true" : "false",
            _biastee_en ? "true" : "false",
            _alc_en     ? "true" : "false",
            _last_rssi, _bpf_code
        );
    }

    // Single-line BT status
    void getStatusBT(char *buf, size_t len) {
        const BandConfig &b = BANDS[_band_idx];
        snprintf(buf, len,
            "BAND:%s,FREQ:%lu,ATT:%d,LNA:%d,BYP:%d,BT:%d,RSSI:%.1f\n",
            b.name, b.freq_center, _att_db,
            (int)_lna_en, (int)_bypass_en, (int)_biastee_en, _last_rssi
        );
    }

private:
    uint8_t _band_idx   = 2;
    uint8_t _bpf_code   = 15;
    int8_t  _att_db     = 0;
    bool    _lna_en     = false;
    bool    _bypass_en  = false;
    bool    _biastee_en = false;
    bool    _alc_en     = false;
    float   _last_rssi  = -99.0f;

    // ── Low-level: set 74HC154 address ───────────────────────────────
    void _setBPF(uint8_t code) {
        _bpf_code = code & 0x0F;
        // Disable decoder while changing address (prevents glitch)
        digitalWrite(GPIO_BPF_EN, LOW);
        delayMicroseconds(10);
        digitalWrite(GPIO_BPF_A0, (_bpf_code >> 0) & 1);
        digitalWrite(GPIO_BPF_A1, (_bpf_code >> 1) & 1);
        digitalWrite(GPIO_BPF_A2, (_bpf_code >> 2) & 1);
        digitalWrite(GPIO_BPF_A3, (_bpf_code >> 3) & 1);
        delayMicroseconds(10);
        // Re-enable only if code < 15 (15 = bypass, no relay needed)
        if (_bpf_code < 15) {
            digitalWrite(GPIO_BPF_EN, HIGH);
        }
    }

    // ── Low-level: set PE4302 attenuation ────────────────────────────
    // att_db: 0–31 dB (internally converted to 0.5 dB step count)
    void _setAttenuator(int8_t att_db) {
        // PE4302: 6-bit word, LSB first; bit weight = 0.5,1,2,4,8,16 dB
        uint8_t steps = (uint8_t)(att_db * 2) & 0x3F;  // 0.5 dB units, 6 bits
        SPI.beginTransaction(SPISettings(1000000, LSBFIRST, SPI_MODE0));
        SPI.transfer(steps);
        SPI.endTransaction();
        // Latch the new attenuation value
        digitalWrite(GPIO_ATT_LE, HIGH);
        delayMicroseconds(2);
        digitalWrite(GPIO_ATT_LE, LOW);
    }
};
