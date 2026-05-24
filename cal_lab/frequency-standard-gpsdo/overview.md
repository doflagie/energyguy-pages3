# Frequency Standard Gpsdo Calibration Overview

## Gear Profile

- Example models: Leo Bodnar GPSDO, Trimble Thunderbolt, BG7TBL GPSDO
- Frequency range: 10 MHz output (typical) + 1PPS time reference
- Connector type: BNC or SMA
- Typical use: Provide a stable frequency/time reference for counters and analyzers

## Purpose

This guide describes a **homebrew, no-calibration-needed** method to calibrate a frequency standard GPSDO using
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
