/*
 * dds_driver.h
 * AD9851 DDS Signal Generator Driver
 * TM-SG-001 Rev A
 *
 * AD9851: 32-bit phase accumulator, 180 MHz max system clock (with 6× multiplier).
 * Serial mode: 40 bits total (32-bit FTW + 8 control bits), MSB first.
 *
 * GPIO assignments (shared SPI bus):
 *   CLK  → GPIO18 (shared with ADF4351, HMC1119)
 *   DATA → GPIO23 (MOSI, shared)
 *   FQ_UD (CS) → GPIO5
 *   RESET      → GPIO4
 */

#pragma once
#include <Arduino.h>
#include <SPI.h>

// AD9851 control
#define DDS_CLK_PIN    18
#define DDS_DATA_PIN   23
#define DDS_FQUD_PIN    5
#define DDS_RESET_PIN   4

// System clock: 25 MHz TCXO × 6 = 150 MHz
// Adjust if TCXO is different or 6× multiplier not used.
#define DDS_SYSCLK_HZ  150000000ULL

// 6× multiplier enable bit in control byte
#define DDS_6X_MULT    0x01

class DDS_AD9851 {
public:
    void begin() {
        pinMode(DDS_FQUD_PIN, OUTPUT);
        pinMode(DDS_RESET_PIN, OUTPUT);
        digitalWrite(DDS_FQUD_PIN, LOW);
        digitalWrite(DDS_RESET_PIN, LOW);

        // Serial mode: pull W_CLK high, FQ_UD high, W_CLK low, FQ_UD low
        // This sequence selects serial mode on reset.
        reset();
    }

    void reset() {
        digitalWrite(DDS_RESET_PIN, HIGH);
        delayMicroseconds(5);
        digitalWrite(DDS_RESET_PIN, LOW);
        delayMicroseconds(5);
        // Pulse W_CLK (CLK) and FQ_UD to enter serial mode
        // AD9851 datasheet: 3 rising edges on W_CLK before first FQ_UD → serial mode
        SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
        for (int i = 0; i < 3; i++) {
            digitalWrite(DDS_CLK_PIN, HIGH); delayMicroseconds(1);
            digitalWrite(DDS_CLK_PIN, LOW);  delayMicroseconds(1);
        }
        digitalWrite(DDS_FQUD_PIN, HIGH); delayMicroseconds(1);
        digitalWrite(DDS_FQUD_PIN, LOW);
        SPI.endTransaction();
    }

    // Set output frequency in Hz.
    // Uses 6× reference multiplier (SYSCLK = TCXO × 6 = 150 MHz).
    void setFrequency(double freq_hz) {
        if (freq_hz < 0) freq_hz = 0;
        if (freq_hz > 70000000.0) freq_hz = 70000000.0;

        // FTW = f_out × 2^32 / f_sysclk
        uint32_t ftw = (uint32_t)((freq_hz * 4294967296.0) / (double)DDS_SYSCLK_HZ);
        _freq_hz = freq_hz;
        _ftw = ftw;
        _writeFTW(ftw, DDS_6X_MULT);
    }

    // Set frequency via pre-calculated FTW (for fast FM modulation updates).
    void setFTW(uint32_t ftw) {
        _ftw = ftw;
        _freq_hz = (double)ftw * (double)DDS_SYSCLK_HZ / 4294967296.0;
        _writeFTW(ftw, DDS_6X_MULT);
    }

    // Apply a frequency offset (signed Hz) relative to current frequency.
    // For FM modulation: call at audio sample rate.
    inline void setOffset(int32_t offset_hz) {
        int32_t delta = (int32_t)((double)offset_hz * 4294967296.0 / (double)DDS_SYSCLK_HZ);
        _writeFTW(_ftw + delta, DDS_6X_MULT);
    }

    void powerDown(bool pd) {
        _writeFTW(_ftw, pd ? 0x04 | DDS_6X_MULT : DDS_6X_MULT);
    }

    double getFrequency() const { return _freq_hz; }
    uint32_t getFTW() const { return _ftw; }

    // Calculate FTW for a given frequency (utility, no side effects).
    static uint32_t calcFTW(double freq_hz) {
        return (uint32_t)((freq_hz * 4294967296.0) / (double)DDS_SYSCLK_HZ);
    }

private:
    double   _freq_hz = 0;
    uint32_t _ftw = 0;

    void _writeFTW(uint32_t ftw, uint8_t ctrl) {
        // 40-bit word: W39..W8 = FTW (MSB first), W7..W0 = control
        SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
        digitalWrite(DDS_FQUD_PIN, LOW);
        SPI.transfer((ftw >> 24) & 0xFF);
        SPI.transfer((ftw >> 16) & 0xFF);
        SPI.transfer((ftw >>  8) & 0xFF);
        SPI.transfer( ftw        & 0xFF);
        SPI.transfer(ctrl);
        // Latch: FQ_UD rising edge
        delayMicroseconds(1);
        digitalWrite(DDS_FQUD_PIN, HIGH);
        delayMicroseconds(1);
        digitalWrite(DDS_FQUD_PIN, LOW);
        SPI.endTransaction();
    }
};
