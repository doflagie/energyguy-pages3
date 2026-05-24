# Inductance Meter Calculations

## Quarter-Wave Coax Stub

Use a shorted coax stub to create a known resonance:

```
L = (c / (4 * f)) * VF
```

Where:
- `L` = stub length (meters)
- `c` = 299,792,458 m/s
- `f` = frequency (Hz)
- `VF` = velocity factor of coax (e.g., 0.66 solid PE, 0.78 foam)

### Example

Target frequency: 14.200 MHz

```
L = (299,792,458 / (4 * 14,200,000)) * 0.66
L = 3.49 m
```

Cut slightly long, then trim while watching the analyzer until resonance hits target.

## 50 ohm Load (Homebrew)

Parallel resistor network:

```
R_total = 1 / (1/R1 + 1/R2 + ... + 1/Rn)
```

### Example

Four 200 ohm resistors in parallel:

```
R_total = 1 / (4/200) = 50 ohms
```

Use 1% or 0.1% resistors if possible.
