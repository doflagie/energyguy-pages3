// =============================================================================
// swr.h — SWR Measurement and Auto-Tuning
// Project:  RF-HFTuner / Motorized Antenna Tuner Suite
// Device:   RF-HFTuner (ESP32-WROOM-32)
// Rev:      1.0  2026-05-01
// =============================================================================
#pragma once
#ifndef SWR_H
#define SWR_H

#include <Arduino.h>
#include <math.h>
#include "config.h"

// Forward declarations (classes defined in tuner.h)
class CapPositionManager;
class InductorPositionManager;

// =============================================================================
// TuningResult — Output of one complete auto-tune run
// =============================================================================

struct TuningResult {
    float       swr;          // Best SWR achieved
    float       L_uh;         // Inductor setting at best SWR (µH)
    float       C_pf;         // Capacitor setting at best SWR (pF)
    int         iterations;   // Total iterations taken
    NetworkType network;      // Network topology used
    int         band;         // Band index (BAND_xxx)
    uint32_t    timestamp;    // millis() at completion
    bool        success;      // True if SWR_TARGET was reached
    float       return_loss;  // Return loss at best match (dB)
    float       gamma;        // Reflection coefficient magnitude at best match
};

// =============================================================================
// SWRMeter — Directional power and SWR measurement
// =============================================================================
//
// Reads two ADC channels (forward and reflected voltage from SWR bridge),
// applies calibration, temperature compensation, and computes SWR, return
// loss, and reflection coefficient.
//
// ADC averaging: 16 samples per reading, highest and lowest 2 rejected
// (trimmed mean), remaining 12 averaged. Reduces ADC noise and RF ripple.
//
// Temperature compensation: applies correction for BAT46 Schottky diode
// Vf drift (-2mV/°C). Correction referenced to 25°C cal point.
//
// Power calibration: slope and offset stored in NVS, applied per channel.
// Set by CALPWR command with a known input power level.
// =============================================================================

class SWRMeter {
public:
    SWRMeter();

    // Initialize ADC pins and load calibration from NVS
    // pin_fwd: GPIO for forward voltage (ADC1 channel required)
    // pin_rev: GPIO for reflected voltage (ADC1 channel required)
    void begin(int pin_fwd, int pin_rev);

    // --- Primary measurement methods ---

    // Returns calibrated forward power in watts
    // Minimum return: SWR_MIN_DETECTABLE_W; returns 0.0 if no RF detected
    float readForward();

    // Returns calibrated reflected power in watts
    float readReverse();

    // Returns SWR: range 1.0 (perfect) to 99.0 (open/short)
    // Returns 99.0 if forward power below noise floor
    float readSWR();

    // Returns return loss in dB (positive value)
    // RL = -20 × log10(Γ)
    // Returns 0.0 if no RF; returns 40.0 if Γ < 0.01
    float readReturnLoss();

    // Returns reflection coefficient magnitude Γ = V_rev / V_fwd
    // Range 0.0 (perfect match) to 1.0 (open/short)
    float readGamma();

    // Returns raw ADC voltage for forward channel (calibrated, mV)
    float readVfwdMV();

    // Returns raw ADC voltage for reflected channel (calibrated, mV)
    float readVrevMV();

    // --- Calibration ---

    // Zero-power calibration: measure ADC offsets with no RF present.
    // Stores cal_offset_fwd and cal_offset_rev. Call with TX off.
    bool calibrate();

    // Set power calibration for known input power level.
    // apply_power_w: actual RF power at TX (e.g. 50.0 watts)
    // Computes and stores cal_slope_fwd, cal_slope_rev.
    bool calibratePower(float apply_power_w);

    // Load calibration constants from NVS
    void loadCalibration();

    // Save calibration constants to NVS
    void saveCalibration();

    // Reset calibration to defaults (unity gain, zero offset)
    void resetCalibration();

    // --- Temperature compensation ---

    // Set current temperature for Vf compensation
    // Reads from NTC thermistor or I2C sensor; call periodically
    void setTemperature(float celsius);

    // Get current temperature reading (°C)
    float getTemperature();

    // --- Status ---

    // Returns true if forward power is above noise floor
    bool hasRF();

    // Returns true if SWR is above fault threshold
    bool isFault();

    // Get last computed SWR without new measurement
    float getLastSWR();

    // Get last computed forward power (W)
    float getLastForward();

    // --- Public calibration constants (also accessible for NVS storage) ---
    float cal_offset_fwd;   // Zero offset, forward channel (V)
    float cal_offset_rev;   // Zero offset, reflected channel (V)
    float cal_slope_fwd;    // Gain correction, forward channel
    float cal_slope_rev;    // Gain correction, reflected channel

private:
    int   _pin_fwd;
    int   _pin_rev;

    float _v_fwd_raw;       // Last raw forward voltage (V, before calibration)
    float _v_rev_raw;       // Last raw reflected voltage (V)
    float _v_fwd_cal;       // Last calibrated forward voltage (V)
    float _v_rev_cal;       // Last calibrated reflected voltage (V)
    float _last_swr;
    float _last_fwd_w;
    float _last_rev_w;
    float _temperature_c;   // Current temperature for compensation
    bool  _calibrated;

    // Internal: read ADC with averaging and outlier rejection
    // Returns voltage in mV, calibrated via analogReadMilliVolts()
    float _readADCmV(int pin);

    // Internal: convert ADC voltage to RF power (watts)
    // v_adc_mv: ADC voltage in mV after divider
    // returns_slope, offset: channel-specific calibration
    float _adcToWatts(float v_adc_mv, float slope, float offset);

    // Internal: apply temperature compensation to Vf
    // temp_c: current temperature; returns correction voltage (V)
    float _tempCorrection(float temp_c);

    // Internal: apply ESP32 ADC nonlinearity correction polynomial
    float _adcPolyCorrect(float v_mv);

    // Trim-mean averaging: reject 2 highest and 2 lowest of 16 samples
    float _trimMean(uint32_t* samples, int count);
};

// =============================================================================
// AutoTuner — State-machine based automatic L/C tuning
// =============================================================================
//
// Algorithm:
//   COARSE SCAN:
//     1. Divide L range into AT_COARSE_L_STEPS positions
//     2. For each L position, sweep C across range in AT_COARSE_C_STEPS
//     3. Record SWR at each (L, C) grid point
//     4. Find grid point with lowest SWR → coarse best (L0, C0)
//
//   FINE SCAN (gradient descent around coarse best):
//     5. Starting from (L0, C0), compute numerical gradient:
//        - Measure SWR at (L0+dL, C0), (L0-dL, C0), (L0, C0+dC), (L0, C0-dC)
//        - Gradient: ∂SWR/∂L, ∂SWR/∂C
//     6. Step in negative gradient direction (descend toward minimum)
//     7. Reduce step size as SWR approaches target
//     8. Converge when SWR < AT_SWR_TARGET or iterations exhausted
//
//   CONVERGENCE CRITERIA:
//     - SWR < SWR_TARGET (1.5): success
//     - SWR < SWR_GOOD (1.2): excellent, stop early
//     - Iterations > AT_MAX_ITERATIONS: fault
//     - SWR > SWR_FAULT_THRESHOLD for > 10 steps: fault
// =============================================================================

// Auto-tuner state machine states
typedef enum {
    AT_IDLE      = 0,  // Not running
    AT_SETUP     = 1,  // Initializing scan parameters
    AT_COARSE_L  = 2,  // Sweeping L positions
    AT_COARSE_C  = 3,  // Sweeping C for current L
    AT_FINE_L    = 4,  // Fine-tuning L (gradient descent)
    AT_FINE_C    = 5,  // Fine-tuning C (gradient descent)
    AT_CONVERGE  = 6,  // Checking convergence
    AT_DONE      = 7,  // Tuning complete
    AT_FAULT     = 8,  // Fault (timeout, no RF, SWR too high)
} AutoTunerState;

class AutoTuner {
public:
    AutoTuner();

    // Link to hardware managers; call before startAutoTune()
    // swr_meter:    pointer to initialized SWRMeter
    // cap_manager:  pointer to initialized CapPositionManager
    // ind_manager:  pointer to initialized InductorPositionManager
    void begin(SWRMeter* swr_meter,
               CapPositionManager* cap_manager,
               InductorPositionManager* ind_manager);

    // Start automatic tuning for specified band
    // band: band index (BAND_160M through BAND_6M)
    // net:  network topology to use (NET_AUTO for firmware selection)
    // Returns true if started successfully
    bool startAutoTune(int band, NetworkType net = NET_AUTO);

    // State machine update — call from main loop every cycle
    // Moves motors, reads SWR, advances state.
    // Returns true if still running; false if done or fault.
    bool update();

    // Stop tuning immediately (leaves motors at current position)
    void stop();

    // Reset to IDLE state
    void reset();

    // --- Results ---

    // Returns best SWR achieved (valid after AT_DONE or AT_FAULT)
    float getBestSWR();

    // Returns inductor value at best SWR (µH)
    float getBestL();

    // Returns capacitor value at best SWR (pF)
    float getBestC();

    // Returns number of iterations taken
    int getIterations();

    // Returns current state
    AutoTunerState getState();

    // Returns true while tuning is in progress
    bool isTuning();

    // Returns true if last tune succeeded (SWR < target)
    bool isSuccess();

    // Returns full TuningResult from last completed tune
    TuningResult getResult();

    // Returns human-readable state string
    const char* getStateString();

private:
    SWRMeter*              _swr;
    CapPositionManager*    _cap;
    InductorPositionManager* _ind;

    AutoTunerState  _state;
    int             _band;
    NetworkType     _network;
    int             _iterations;
    float           _best_swr;
    float           _best_L;
    float           _best_C;
    TuningResult    _result;

    // Coarse scan state
    int     _coarse_L_idx;
    int     _coarse_C_idx;
    float   _coarse_L_step;
    float   _coarse_C_step;
    float   _coarse_L_start;
    float   _coarse_C_start;
    float   _grid_swr[AT_COARSE_L_STEPS][AT_COARSE_C_STEPS + 1];

    // Fine scan state
    float   _fine_L;
    float   _fine_C;
    float   _fine_L_step;
    float   _fine_C_step;
    float   _grad_L;
    float   _grad_C;

    uint32_t _start_time;
    uint32_t _last_move_time;

    // Internal helpers
    float  _measureSWR();           // Settle and measure SWR
    bool   _moveToLC(float L, float C); // Move both axes to L(µH), C(pF)
    void   _clampLC(float& L, float& C); // Clamp to band limits
    void   _initCoarseScan();
    bool   _stepCoarseScan();
    void   _initFineScan();
    bool   _stepFineScan();
    void   _computeGradient();
    NetworkType _selectNetwork(int band);
};

// =============================================================================
// Impedance Matching Math
// =============================================================================
//
// All network calculations return component values for a target match.
// Input impedance: complex (R_load + jX_load), source: R_source (real, 50Ω)
// Frequency: f_hz in Hz.
//
// Reference: ARRL Handbook, Chapter 9 (Impedance Matching Networks)
//            Terman "Radio Engineers' Handbook"
// =============================================================================

struct LNetworkResult {
    float L_uh;   // Series inductance (µH)
    float C_pf;   // Shunt capacitance (pF)
    bool  valid;  // True if solution exists for this impedance
};

struct PiNetworkResult {
    float C1_pf;  // Input shunt capacitance (pF)
    float L_uh;   // Series inductance (µH)
    float C2_pf;  // Output shunt capacitance (pF)
    bool  valid;
};

struct TNetworkResult {
    float L1_uh;  // Input series inductance (µH)
    float C_pf;   // Shunt capacitance (pF)
    float L2_uh;  // Output series inductance (µH)
    bool  valid;
};

// L-network: low-pass (series L from source, shunt C to ground)
// Works when R_load > R_source (step-up transformation)
// Derivation:
//   Q = sqrt(R_load/R_source - 1)
//   X_L = Q × R_source  →  L = X_L / (2π×f)
//   X_C = R_load / Q    →  C = 1 / (2π×f×X_C)
//   Adjusted for load reactance X_load using complex arithmetic
LNetworkResult calcLNetworkLowPass(float R_source, float R_load,
                                   float X_load, float freq_hz);

// L-network: high-pass (shunt C from source, series L to load)
// Works when R_load < R_source (step-down transformation)
// Derivation:
//   Q = sqrt(R_source/R_load - 1)
//   X_C = R_source / Q  →  C = 1 / (2π×f×X_C)
//   X_L = Q × R_load    →  L = X_L / (2π×f)
LNetworkResult calcLNetworkHighPass(float R_source, float R_load,
                                    float X_load, float freq_hz);

// Pi-network: both shunt caps, series inductor
// Q_target: desired network Q (typically 5-15 for HF)
// Derivation:
//   For Pi: Q = sqrt((R_high/R_low) × (Q²+1) - 1)
//   X_C1 = R_source / Q_loaded
//   X_C2 = R_load / Q_loaded
//   X_L  = R_source × Q / (Q²+1) × (Q² + (R_source/X_L)²) ...
//   (Full derivation: ARRL Handbook eq. 9.41–9.44)
PiNetworkResult calcPiNetwork(float R_source, float R_load,
                               float X_load, float freq_hz, float Q_target);

// T-network: two series inductors, shunt capacitor
// Good for matching low-impedance antennas (Z < 50Ω)
// Derivation:
//   Dual of Pi network; swap L↔C and R↔G
//   Q = sqrt((R_source / R_node) - 1) where R_node = virtual series resistance
//   X_L1 = Q × R_source
//   X_L2 = Q × R_load
//   X_C  = R_node × (Q1 + Q2) / (Q1² + 1)
//   (Full derivation: Terman "Radio Engineers' Handbook" p.219)
TNetworkResult calcTNetwork(float R_source, float R_load,
                             float X_load, float freq_hz, float Q_target);

// Utility: estimate load impedance from SWR + line length
// (Simple model; use NanoVNA for accurate measurement)
// Returns R_load + jX_load estimate
// swr: measured SWR, line_len_m: feedline length (m), freq_hz: frequency
void estimateLoadImpedance(float swr, float line_len_m, float freq_hz,
                           float* R_load, float* X_load);

// Utility: compute reactance from L and C at frequency
float inductiveReactance(float L_uh, float freq_hz);   // XL = 2π×f×L
float capacitiveReactance(float C_pf, float freq_hz);  // XC = 1/(2π×f×C)

// Utility: compute component values from reactance at frequency
float reactanceToL(float X_ohm, float freq_hz);  // L(µH) = X / (2π×f) × 1e6
float reactanceToC(float X_ohm, float freq_hz);  // C(pF) = 1/(2π×f×X) × 1e12

// =============================================================================
// END OF swr.h
// =============================================================================

#endif // SWR_H
