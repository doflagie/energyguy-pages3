/*
 * adf4351_driver.h
 * ADF4351 Fractional-N PLL Synthesizer Driver
 * TM-SG-001 Rev A
 *
 * Covers 35 MHz – 4.4 GHz via internal VCO (2.2–4.4 GHz) + output dividers.
 * Six 32-bit registers (R0–R5); write R5 first, R0 last.
 * SPI: 32 bits per register, MSB first, latch on LE rising edge.
 *
 * GPIO assignments:
 *   CLK  → GPIO18 (shared SPI bus)
 *   DATA → GPIO23 (MOSI, shared)
 *   LE   → GPIO16 (ADF4351 latch enable)
 *   CE   → tied to 3.3V (always enabled)
 *   LD   → GPIO35 (lock detect input, ADC1 — read as digital HIGH=locked)
 *
 * Reference: 10 MHz TCXO on REFIN pin.
 */

#pragma once
#include <Arduino.h>
#include <SPI.h>

#define ADF4351_LE_PIN   16
#define ADF4351_LD_PIN   35
#define ADF4351_REF_HZ   10000000ULL   // 10 MHz TCXO

// Output divider values encoded as register bits [22:20] in R4
#define ADF4351_DIV_1    0
#define ADF4351_DIV_2    1
#define ADF4351_DIV_4    2
#define ADF4351_DIV_8    3
#define ADF4351_DIV_16   4
#define ADF4351_DIV_32   5
#define ADF4351_DIV_64   6

// Output power bits [4:3] in R4
#define ADF4351_PWR_M4DBM  0   // -4 dBm
#define ADF4351_PWR_M1DBM  1   // -1 dBm
#define ADF4351_PWR_P2DBM  2   // +2 dBm
#define ADF4351_PWR_P5DBM  3   // +5 dBm (default)

struct ADF4351Config {
    uint32_t freq_hz;      // Target output frequency
    uint8_t  output_pwr;   // ADF4351_PWR_* constant
    bool     rf_enabled;   // RF output enable
    bool     aux_enabled;  // Auxiliary output enable
    uint8_t  aux_pwr;      // Auxiliary output power
};

class ADF4351 {
public:
    void begin() {
        pinMode(ADF4351_LE_PIN, OUTPUT);
        pinMode(ADF4351_LD_PIN, INPUT);
        digitalWrite(ADF4351_LE_PIN, LOW);
        // Initialize with output off
        _initRegisters();
        _writeAllRegisters();
    }

    // Set output frequency.  Returns true if lock achieved within timeout_ms.
    bool setFrequency(uint32_t freq_hz, uint8_t output_pwr = ADF4351_PWR_P5DBM,
                      uint32_t timeout_ms = 50) {
        if (freq_hz < 35000000UL || freq_hz > 4400000000UL) return false;
        _cfg.freq_hz = freq_hz;
        _cfg.output_pwr = output_pwr;
        _cfg.rf_enabled = true;
        _calculateRegisters(freq_hz, output_pwr);
        _writeAllRegisters();
        uint32_t t0 = millis();
        while (millis() - t0 < timeout_ms) {
            if (isLocked()) return true;
            delay(1);
        }
        return false;
    }

    void rfEnable(bool en) {
        if (en) _r4 |= (1 << 2); else _r4 &= ~(1 << 2);
        _writeReg(4, _r4);
    }

    bool isLocked() {
        return digitalRead(ADF4351_LD_PIN) == HIGH;
    }

    // Read back current configuration
    uint32_t getFrequency() const { return _cfg.freq_hz; }
    uint32_t getR0() const { return _r0; }

    // Direct register write (advanced use)
    void writeRegister(uint8_t addr, uint32_t val) {
        uint32_t word = (val & 0xFFFFFFF8) | (addr & 0x07);
        _writeReg(addr, word);
    }

private:
    ADF4351Config _cfg = {};
    uint32_t _r0=0, _r1=0, _r2=0, _r3=0, _r4=0, _r5=0;

    void _initRegisters() {
        // R5: lock detect to MUXOUT
        _r5 = (0b01 << 22) | 5;   // LD_PIN_MODE = digital lock detect
        // R4: output divider, power, RF enable
        _r4 = (ADF4351_PWR_P5DBM << 3) | (1 << 2) | 4;
        // R3: clock divider
        _r3 = (150 << 3) | 3;     // CLK_DIVIDER = 150 for fast lock
        // R2: CP=5mA, R=1, PD polarity positive, MUXOUT=digital LD
        _r2 = (7 << 9) |   // CP current = 5 mA (bits 12:9)
              (1 << 7) |    // PD polarity = positive
              (0b110 << 4)| // MUXOUT = digital lock detect
              2;
        // R1: modulus = 1000, phase = 0
        _r1 = (0 << 15) | (1000 << 3) | 1;
        // R0: INT=100, FRAC=0 (will be recalculated)
        _r0 = (100 << 15) | (0 << 3) | 0;
    }

    void _calculateRegisters(uint32_t freq_hz, uint8_t pwr) {
        // Determine output divider
        uint8_t div_sel = ADF4351_DIV_1;
        uint32_t vco_freq = freq_hz;

        // VCO must be 2200–4400 MHz
        // Find smallest divider such that vco_freq = freq × divider is in [2200, 4400] MHz
        uint32_t dividers[] = {1, 2, 4, 8, 16, 32, 64};
        for (int d = 0; d < 7; d++) {
            uint64_t vco = (uint64_t)freq_hz * dividers[d];
            if (vco >= 2200000000UL && vco <= 4400000000UL) {
                div_sel = d;
                vco_freq = (uint32_t)vco;
                break;
            }
        }

        // Calculate INT and FRAC
        // VCO = f_PD × (INT + FRAC/MOD)
        // f_PD = ADF4351_REF_HZ / R_counter = 10 MHz
        const uint32_t f_pd = (uint32_t)ADF4351_REF_HZ;  // R=1
        const uint32_t mod  = 1000;  // 12-bit modulus
        uint32_t N_int  = vco_freq / f_pd;
        uint32_t N_frac = (uint32_t)(((uint64_t)(vco_freq - N_int * f_pd) * mod) / f_pd);

        // Clamp INT to valid range (23–65535)
        if (N_int < 23)  N_int = 23;
        if (N_int > 65535) N_int = 65535;

        // Build R0
        _r0 = ((uint32_t)(N_int & 0xFFFF) << 15) |
              ((uint32_t)(N_frac & 0x0FFF) << 3) | 0;

        // Build R1 with modulus
        _r1 = (0 << 15) | ((mod & 0x0FFF) << 3) | 1;

        // Build R4 with divider select and power
        _r4 = ((uint32_t)(div_sel & 0x07) << 20) |
              ((uint32_t)(pwr & 0x03) << 3) |
              (1 << 2) | 4;  // RF_OUTPUT_ENABLE
    }

    void _writeAllRegisters() {
        // Must write in order R5, R4, R3, R2, R1, R0
        _writeReg(5, _r5);
        _writeReg(4, _r4);
        _writeReg(3, _r3);
        _writeReg(2, _r2);
        _writeReg(1, _r1);
        _writeReg(0, _r0);
    }

    void _writeReg(uint8_t addr, uint32_t val) {
        uint32_t word = (val & ~0x07UL) | (addr & 0x07);
        SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
        digitalWrite(ADF4351_LE_PIN, LOW);
        SPI.transfer((word >> 24) & 0xFF);
        SPI.transfer((word >> 16) & 0xFF);
        SPI.transfer((word >>  8) & 0xFF);
        SPI.transfer( word        & 0xFF);
        delayMicroseconds(1);
        digitalWrite(ADF4351_LE_PIN, HIGH);
        delayMicroseconds(1);
        digitalWrite(ADF4351_LE_PIN, LOW);
        SPI.endTransaction();
    }
};
