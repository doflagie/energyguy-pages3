# Nanovna Vector Network Analyzer Calibration Overview

## Gear Profile

- Example models: NanoVNA V2/V2 Plus, NanoVNA H4, NanoVNA-H
- Frequency range: 50 kHz to 3 GHz (varies by model)
- Connector type: SMA
- Typical use: Measure antenna impedance/SWR, filters, and RF networks

## Purpose

This guide describes a **homebrew, no-calibration-needed** method to calibrate a NanoVNA vector network analyzer using
public reference signals and first-principles checks. It borrows techniques from:

- `../gps_calibration.md`
- `../fm_broadcast_calibration.md`
- `../radio_standard_calibration.md`
- `../verification_procedures.md`

## What You Will Build

- A simple reference load (homebrew 50 ohm)
- A quarter-wave coax stub test fixture
- Optional GPS 1PPS counter (if you want tighter frequency checks)

## Expected Accuracy

- Frequency axis: 0.1 to 1 ppm (with GPS or time standards)
- SWR/impedance: limited by resistor tolerance (typically 1-2%)
