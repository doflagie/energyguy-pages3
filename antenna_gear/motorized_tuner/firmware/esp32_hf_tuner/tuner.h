// =============================================================================
// tuner.h — Tuner State, Band Management, Network Control, Relay Control
// Project:  RF-HFTuner / Motorized Antenna Tuner Suite
// Device:   RF-HFTuner (ESP32-WROOM-32)
// Rev:      1.0  2026-05-01
// =============================================================================
#pragma once
#ifndef TUNER_H
#define TUNER_H

#include <Arduino.h>
#include <AccelStepper.h>
#include <Preferences.h>
#include "config.h"
#include "swr.h"

// =============================================================================
// BAND TABLE — All 11 HF + 6M bands
// =============================================================================

const BandData BAND_TABLE[NUM_BANDS] = {
    // name    freq_mhz  lambda_m  L_min  L_max   C_min  C_max   preferred_net  preset_L  preset_C
    { "160m",  1.85f,   162.2f,   5.0f,  30.0f,  50.0f, 500.0f, PI_NETWORK,    12.0f,    250.0f },
    { "80m",   3.60f,    83.3f,   2.0f,  20.0f,  25.0f, 300.0f, PI_NETWORK,     7.0f,    150.0f },
    { "60m",   5.35f,    56.1f,   1.0f,  15.0f,  15.0f, 200.0f, L_LOWPASS,      4.0f,     90.0f },
    { "40m",   7.15f,    41.9f,   0.5f,  10.0f,  10.0f, 150.0f, L_LOWPASS,      3.0f,     60.0f },
    { "30m",  10.12f,    29.6f,   0.3f,   7.0f,   8.0f, 100.0f, L_LOWPASS,      2.0f,     40.0f },
    { "20m",  14.20f,    21.1f,   0.2f,   5.0f,   5.0f,  75.0f, L_LOWPASS,      1.2f,     25.0f },
    { "17m",  18.10f,    16.6f,   0.1f,   4.0f,   4.0f,  60.0f, L_LOWPASS,      0.8f,     18.0f },
    { "15m",  21.20f,    14.2f,   0.1f,   3.0f,   3.0f,  50.0f, L_HIGHPASS,     0.6f,     14.0f },
    { "12m",  24.90f,    12.0f,   0.1f,   2.5f,   2.0f,  40.0f, L_HIGHPASS,     0.4f,     10.0f },
    { "10m",  28.50f,    10.5f,  0.05f,   2.0f,   2.0f,  35.0f, L_HIGHPASS,     0.3f,      8.0f },
    { "6m",   50.10f,     6.0f,  0.02f,   0.5f,   1.0f,  20.0f, L_HIGHPASS,     0.1f,      4.0f },
};

// =============================================================================
// NetworkController — Controls 3 relays for L/Pi/T topology selection
// =============================================================================
//
// Relay truth table:
//   L_LOWPASS  : K1=0, K2=0, K3=0  — Series L (source→load), Shunt C to GND
//   L_HIGHPASS : K1=1, K2=0, K3=0  — Shunt C (source), Series L to load
//   PI_NETWORK : K1=0, K2=1, K3=0  — C1 shunt, Series L, C2 shunt
//   T_NETWORK  : K1=0, K2=0, K3=1  — Series L1, Shunt C, Series L2
// =============================================================================

class NetworkController {
public:
    NetworkController();

    // Initialize relay GPIO pins (or PCF8574 if USE_PCF8574 defined)
    void begin();

    // Set network topology; switches relays with settle delay
    void setNetwork(NetworkType net);

    // Get current network topology
    NetworkType getNetwork();

    // Get network name string
    const char* getNetworkName(NetworkType net);
    const char* getNetworkName();  // Current network

    // Return relay states as bitmask (bit0=K1, bit1=K2, bit2=K3)
    uint8_t getRelayState();

private:
    NetworkType _current_net;
    bool        _k1, _k2, _k3;

    void _setRelays(bool k1, bool k2, bool k3);
    void _writeRelayPin(int relay_num, bool state);
};

// =============================================================================
// CapPositionManager — Variable Capacitor Position Control
//   (Simplified version; full version in TM-VCAP-001)
// =============================================================================
//
// Manages AccelStepper for the variable capacitor motor.
// Tracks position in pF (floating point), correlates to motor steps.
// Homing: drive to MIN limit switch, set position = CAP_MIN_PF.
// Backlash compensation: always approach target from same direction.
// =============================================================================

class CapPositionManager {
public:
    CapPositionManager();

    // Initialize with pointer to AccelStepper instance
    void begin(AccelStepper* stepper);

    // Move to target capacitance in pF; non-blocking
    // Returns true if move started; false if out of range
    bool moveToPF(float target_pf);

    // Home the capacitor: drive to MIN limit, set position = CAP_MIN_PF
    // Blocking until home complete or timeout
    bool homeToMin();

    // Returns current position in pF (interpolated from step count)
    float getCurrentPF();

    // Returns current step position
    long getCurrentSteps();

    // Returns true if motor is moving
    bool isMoving();

    // Returns true if homed
    bool isHomed();

    // Returns true if at min limit switch
    bool isAtMin();

    // Returns true if at max limit switch
    bool isAtMax();

    // Position error: difference between commanded and pot-read position (pF)
    // Requires optional pot ADC connection; returns 0.0 if no pot
    float positionError();

    // Run stepper (call from loop)
    void update();

    // Force-set position (use after homing or known position)
    void setPosition(float pf);

    // Emergency stop
    void stop();

    float current_pf;
    bool  homed;

private:
    AccelStepper* _stepper;
    long   _step_zero;        // Step count corresponding to CAP_MIN_PF
    float  _target_pf;
    bool   _moving;
    bool   _last_dir_positive; // Track last direction for backlash compensation

    // Convert pF to steps from zero
    long   _pfToSteps(float pf);

    // Convert steps from zero to pF
    float  _stepsToPF(long steps);

    // Read limit switches with debounce
    bool   _readLimitMin();
    bool   _readLimitMax();
};

// =============================================================================
// InductorPositionManager — Roller Inductor Position Control
//   (Simplified version; full version in TM-RLIND-001)
// =============================================================================
//
// Manages AccelStepper for the roller inductor motor.
// Tracks position in µH using Wheeler formula for coil inductance.
// Wheeler formula (single-layer, air core):
//   L(µH) = (r² × N²) / (9r + 10l)
//   where r = coil radius (inches), l = wound length (inches), N = turns used
// Contact position (mm from start) → N (turns used) → L (µH)
// =============================================================================

class InductorPositionManager {
public:
    InductorPositionManager();

    // Initialize with pointer to AccelStepper instance
    void begin(AccelStepper* stepper);

    // Move to target inductance in µH; non-blocking
    bool moveToUH(float target_uh);

    // Home the inductor: drive to MIN limit, set position = IND_MIN_UH
    bool homeToMin();

    // Returns current inductance in µH (from Wheeler formula)
    float getCurrentUH();

    // Returns current position in mm (from step count)
    float getCurrentMM();

    // Returns current step position
    long getCurrentSteps();

    // Returns true if motor is moving
    bool isMoving();

    // Returns true if homed
    bool isHomed();

    bool isAtMin();
    bool isAtMax();

    // Run stepper (call from loop)
    void update();

    // Force-set position
    void setPositionMM(float mm);

    void stop();

    float current_uh;
    bool  homed;

private:
    AccelStepper* _stepper;
    long   _step_zero;
    float  _target_uh;
    bool   _moving;
    bool   _last_dir_positive;

    // Wheeler formula: returns µH given number of active turns
    float  _wheelerFormula(float turns_active);

    // Convert contact position (mm) to active turns
    float  _mmToTurns(float mm);

    // Convert µH to contact position (mm) — inverse Wheeler
    // Solved numerically (Newton-Raphson)
    float  _uhToMM(float target_uh);

    long   _mmToSteps(float mm);
    float  _stepsToMM(long steps);

    bool   _readLimitMin();
    bool   _readLimitMax();
};

// =============================================================================
// TunerCommandParser — Text Command Interface (Serial / BT SPP / WiFi)
// =============================================================================
//
// Commands (case-insensitive):
//   TUNE [band]       — auto-tune for specified band (band name or 0-10 index)
//   HOME              — home both axes (CAP and IND)
//   SETL <µH>         — move inductor to specified µH
//   SETC <pF>         — move capacitor to specified pF
//   BAND <n|name>     — select band (sets L/C to presets if available)
//   NETWORK <n|name>  — set network type (LLOWPASS/LHIGHPASS/PI/T/AUTO)
//   SWR               — return current SWR reading as "SWR: x.xx"
//   STATUS            — return full JSON status
//   CAL               — trigger SWR zero-power calibration
//   CALPWR <watts>    — trigger SWR power calibration at known power
//   SAVE [band]       — save current L/C as preset for current (or named) band
//   LOAD [band]       — load preset for current (or named) band
//   INFO              — firmware version, device name, uptime
//   HELP              — list all commands
//   STOP              — stop all motion immediately
//   RESET             — restart firmware (soft reset)
//   SETPARAM <k> <v>  — set configuration parameter (VF_ACTUAL, etc.)
//   GETPARAM <k>      — get configuration parameter
// =============================================================================

class TunerCommandParser {
public:
    TunerCommandParser();

    // Link all sub-systems before use
    void begin(SWRMeter*             swr,
               AutoTuner*            at,
               CapPositionManager*   cap,
               InductorPositionManager* ind,
               NetworkController*    net,
               Preferences*          prefs);

    // Process a received command string; returns response string.
    // Input: complete line (newline stripped)
    // Returns: response string (may be multi-line JSON or single value)
    String process(const String& cmd);

private:
    SWRMeter*               _swr;
    AutoTuner*              _at;
    CapPositionManager*     _cap;
    InductorPositionManager* _ind;
    NetworkController*      _net;
    Preferences*            _prefs;

    int     _current_band;

    // Command handlers — each returns a response string
    String  _cmdTune(const String& arg);
    String  _cmdHome();
    String  _cmdSetL(const String& arg);
    String  _cmdSetC(const String& arg);
    String  _cmdBand(const String& arg);
    String  _cmdNetwork(const String& arg);
    String  _cmdSWR();
    String  _cmdStatus();
    String  _cmdCal();
    String  _cmdCalPwr(const String& arg);
    String  _cmdSave(const String& arg);
    String  _cmdLoad(const String& arg);
    String  _cmdInfo();
    String  _cmdHelp();
    String  _cmdStop();
    String  _cmdSetParam(const String& arg);
    String  _cmdGetParam(const String& arg);

    // Utilities
    int     _parseBand(const String& arg);
    NetworkType _parseNetwork(const String& arg);
    String  _statusJSON();
    String  _errorResp(const String& msg);
    String  _okResp(const String& msg);
};

// =============================================================================
// BandPresetStore — Per-band tuning preset storage in NVS
// =============================================================================
//
// Stores the best known L/C tuning point for each band.
// 11 band slots (BAND_160M through BAND_6M).
// Each slot: L_uh (float), C_pf (float), best_swr (float).
// Stored in NVS namespace "tuner_presets".
// =============================================================================

class BandPresetStore {
public:
    BandPresetStore();

    // Initialize with Preferences instance
    void begin(Preferences* prefs);

    // Save best tuning point for a band
    // band: band index (0–10), L/C in µH/pF, swr: achieved SWR
    bool save(int band, float L_uh, float C_pf, float swr);

    // Load preset for band; returns false if no preset stored
    bool load(int band, float* L_uh, float* C_pf, float* swr);

    // Clear preset for band
    void clear(int band);

    // Clear all presets
    void clearAll();

    // Returns true if a preset exists for band
    bool hasPreset(int band);

    // Returns best SWR stored for band (or 99.0 if none)
    float getBestSWR(int band);

private:
    Preferences* _prefs;

    // Key generation: "pL_NN", "pC_NN", "pS_NN" where NN = band index 00-10
    String _keyL(int band);
    String _keyC(int band);
    String _keyS(int band);
};

// =============================================================================
// END OF tuner.h
// =============================================================================

#endif // TUNER_H
