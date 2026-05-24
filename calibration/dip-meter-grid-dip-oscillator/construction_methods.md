# Dip Meter Grid Dip Oscillator Construction Methods

## Build a 50 ohm Load

- Use four 200 ohm, 1% resistors in parallel.
- Solder directly inside a BNC/SMA connector shell if possible.
- Keep leads short to reduce inductance.

## Build a Quarter-Wave Stub

- Choose coax with known velocity factor.
- Cut to calculated length (see Calculations).
- Short the far end (center to shield).
- Label the stub with its target frequency and VF.

## Optional GPS 1PPS Interface

- GPS module with 1PPS output.
- LED + resistor for lock indication.
- Optional ESP32/CYD counter (see optional code example).
