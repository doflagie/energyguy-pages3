# Modulation Analyzer Theory of Operation

## Calibration Philosophy

We use **absolute references** (GPS 1PPS, WWV/CHU, FM stations) and **known physics**
(speed of light, coax velocity factor, resonance) to validate the analyzer without relying on
any pre-calibrated lab equipment.

## Key Principles

1. **Frequency accuracy**: Check displayed frequency against GPS/WWV/CHU/FM carriers.
2. **Impedance accuracy**: Validate using a known load and known reactance from a stub or LC.
3. **Repeatability**: Measurements should be stable over time and temperature.

## Reference Sources

- **GPS 1PPS**: Atomic time tick (best accuracy).
- **WWV/CHU**: HF time standards (good accuracy).
- **FM broadcast**: Convenient local reference (moderate accuracy).
