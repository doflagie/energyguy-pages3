# Field Strength Meter Calibration Overview

## Gear Profile

- Example models: Basic field strength meter, RF probe + DVM
- Frequency range: HF/VHF/UHF depending on probe and detector
- Connector type: Probe lead or BNC/SMA adapter
- Typical use: Relative signal strength comparisons and pattern checks

## Purpose

This guide describes a **homebrew, no-calibration-needed** method to calibrate a field strength meter using
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
