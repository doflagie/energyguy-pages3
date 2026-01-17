# Oscilloscope Timebase Calibration

## Overview

The oscilloscope's timebase determines how accurately it measures time and frequency. This guide shows how to calibrate using GPS atomic clock accuracy.

**Key Insight:** Same GPS methods used for TinySA/NanoVNA work perfectly for oscilloscopes!

---

## Quick Method: GPS 1PPS

### If You Already Built GPS Setup

**From TinySA/NanoVNA calibration:**
- GPS module with 1PPS output: Ready ✓
- Arduino (optional): Not needed for scope!

**New procedure:**
1. Connect GPS 1PPS to oscilloscope input
2. Measure pulse characteristics
3. Calculate timebase error
4. Document or apply correction

**Time:** 15-30 minutes
**Cost:** $0 (reuse existing GPS)

---

## Step-by-Step Procedure

### Step 1: GPS Setup (if not already built)

**See:** [gps_calibration.md](gps_calibration.md) for detailed GPS module setup

**Quick version:**
1. GPS module (NEO-6M/7M/8M): $10-20
2. Power: 3.3V or 5V
3. Antenna: Included ceramic patch
4. Wait for lock: 30-120 seconds
5. 1PPS output: Blinks once per second

**1PPS signal characteristics:**
- Frequency: 1 Hz (period = 1.000000 seconds)
- Pulse width: Typically 100-200 ms
- Voltage: 3.3V or 5V TTL
- Accuracy: ±50 nanoseconds (±0.00005 ppm!)

### Step 2: Connect to Oscilloscope

**Connection:**
```
GPS Module          Oscilloscope
──────────────────────────────────
1PPS output    →    CH1 input
GND            →    GND (scope ground clip)
VCC (5V)       →    USB power or bench supply
```

**Scope settings:**
1. **Channel 1:**
   - Coupling: DC
   - V/div: 1V or 2V (to see ~3-5V signal clearly)
   - Position: Center

2. **Timebase:**
   - Time/div: 200 ms/div (to see ~1 second period)
   - Trigger: CH1, rising edge
   - Trigger level: ~1.5V (mid-level)

3. **Acquisition:**
   - Mode: Normal or Auto
   - Average: 16 or 32 (reduces jitter)

### Step 3: Measure Period

**Method A: Cursor Measurement**

1. **Enable cursors:**
   - Press CURSOR button
   - Select TIME cursors

2. **Place cursors on rising edges:**
   ```
   Cursor 1 → First rising edge
   Cursor 2 → Second rising edge (one pulse later)
   ```

3. **Read delta time (ΔT):**
   ```
   Display shows: ΔT = 1.0023 s (example - your scope will differ)
   Expected: 1.000000 s
   Error: +2.3 ms = +2300 ppm!
   ```

**Method B: Frequency Measurement**

1. **Enable frequency counter (if scope has one):**
   - Press MEASURE
   - Select Frequency
   - Source: CH1

2. **Read frequency:**
   ```
   Display shows: Freq = 0.9977 Hz (example)
   Expected: 1.0000 Hz
   Error: -0.0023 Hz = -2300 ppm
   ```

**Method C: Pulse Width**

1. **Measure pulse width:**
   - MEASURE → Width+ (positive pulse width)
   - Should be stable value

2. **Calculate period:**
   ```
   If pulse width = 100.0 ms
   And duty cycle is known (typically 10% for GPS 1PPS)
   Period = width / duty_cycle
   ```

   **Note:** Period measurement is better than width.

### Step 4: Calculate Timebase Error

**From period measurement:**
```
Measured period: T_measured (from scope)
Actual period: T_actual = 1.000000 s (GPS is atomic clock)

Error (seconds) = T_measured - T_actual
Error (ppm) = (Error / T_actual) × 10^6

Example:
T_measured = 1.000025 s
T_actual = 1.000000 s
Error = +0.000025 s = +25 μs
Error (ppm) = 0.000025 / 1.0 × 10^6 = +25 ppm

Interpretation: Scope timebase is 25 ppm FAST
```

**From frequency measurement:**
```
Measured frequency: F_measured
Actual frequency: F_actual = 1.000000 Hz

Error (ppm) = (F_actual - F_measured) / F_actual × 10^6

Example:
F_measured = 0.999975 Hz
F_actual = 1.000000 Hz
Error = (1.0 - 0.999975) / 1.0 × 10^6 = +25 ppm

Same result: Scope is 25 ppm fast
```

---

## Alternative: Mains Frequency

**If no GPS available:**

### Using 50/60 Hz Mains

**Warning:** This is less accurate but free!

**Mains frequency accuracy:**
- Short term (seconds): ±0.1 Hz
- Long term (hours): ±0.01 Hz
- Utility companies maintain accurate frequency
- Not as good as GPS, but usable

**Procedure:**

1. **Build isolation circuit (IMPORTANT - SAFETY!):**
   ```
   NEVER connect scope directly to mains!

   Instead: Use transformer
   Mains → Small transformer (12V or 9V output) → Scope input

   Or: Use phone charger
   Mains → USB charger → Monitor 5V ripple (120Hz in US)
   ```

2. **Measure frequency:**
   - Expected: 60.000 Hz (US) or 50.000 Hz (EU)
   - Scope reading: Compare

3. **Calculate error** (same as GPS method)

**Accuracy:** ±100 ppm (okay for rough check)

---

## Applying Corrections

### Method 1: Document the Error

**Simplest approach:**

1. **Measure timebase error:** (e.g., +25 ppm)
2. **Create correction table:**
   ```
   Timebase Error: +25 ppm
   Scope displays 1.000 s → Actual is 0.999975 s
   Scope displays 1.000 ms → Actual is 0.999975 ms
   Scope displays 1.000 μs → Actual is 0.999975 μs

   Correction factor: 0.999975
   ```

3. **Label scope:**
   ```
   Stick label on scope:
   "TIMEBASE: +25 ppm
    Multiply displayed time by 0.999975 for actual time"
   ```

### Method 2: Internal Calibration (if accessible)

**Some DSO1013D models have calibration menu:**

1. **Enter cal mode:**
   - Power off
   - Hold RUN/STOP while powering on
   - Or: UTILITY → CAL (hidden menu)

2. **Find timebase cal:**
   - Look for "TIMEBASE CAL" or "FREQUENCY CAL"

3. **Adjust:**
   - Usually a numerical entry
   - Enter PPM correction
   - Save

**Consult your specific firmware documentation!**

### Method 3: Mental Math

**For quick measurements:**
```
Error is +25 ppm

If scope shows 1.000 ms:
Actual = 1.000 × (1 - 25/10^6) = 0.999975 ms

Usually ignore for rough work
Apply correction for precision measurements
```

---

## Verification Methods

### Cross-Check 1: Known Frequency

**Use calibrated signal generator (if available):**
- Set to 1.000 MHz
- Measure on scope
- Should read 1.000 MHz ± your ppm error

**Or use calibrated TinySA:**
- Set TinySA to CW mode, 1 MHz
- Feed to scope
- Measure frequency

### Cross-Check 2: Crystal Oscillator

**Build simple crystal oscillator:**
```
Materials:
- 32.768 kHz watch crystal ($0.50)
- CD4060 divider IC ($0.50)
- Resistor, capacitors

Output: Exact 1 Hz from watch crystal
Accuracy: ±20 ppm (watch crystal)

Measure on scope, should match GPS within ±50 ppm
```

### Cross-Check 3: Multiple GPS Modules

**If you have two GPS modules:**
- Both produce 1 Hz
- Should agree to within ±0.1 ppm
- If scope shows difference → scope error

---

## Temperature Effects

### Characterizing Temperature Coefficient

**Same procedure as TinySA:**

1. **Cold soak:** Refrigerator, 30 min, ~5°C
2. **Measure period** at cold temperature
3. **Warm up** naturally to room temp (~22°C)
4. **Measure periodically**
5. **Heat** gently to ~40°C
6. **Plot error vs. temperature**

**Typical results:**
```
Temp (°C)   Period (s)   Error (ppm)
5           1.000035     +35
15          1.000028     +28
25          1.000025     +25
35          1.000030     +30
45          1.000038     +38
```

**Conclusion:** Scope timebase drifts with temperature. Calibrate after warmup!

---

## Advanced: 10 MHz Reference Input

**Some scopes have external reference input:**

**If your scope has 10 MHz REF IN:**
1. Build GPS-locked 10 MHz reference
2. Connect to REF IN
3. Scope locks to GPS
4. Continuous GPS accuracy!

**Building GPS-locked 10 MHz:**
- PLL locks 10 MHz VCXO to GPS 1PPS
- See advanced projects in [gps_calibration.md](gps_calibration.md)
- Cost: $50-100
- Result: Continuous atomic clock lock

---

## Real-World Example

### Calibrating DSO1013D

**Equipment:**
- DSO1013D oscilloscope
- NEO-6M GPS module (from TinySA project)
- 5V USB power supply

**Procedure:**

1. **Connected GPS 1PPS to CH1**

2. **Scope settings:**
   ```
   CH1: 1V/div, DC coupling
   Timebase: 200 ms/div
   Trigger: CH1, rising, 1.5V level
   Average: 16
   ```

3. **Enabled cursors, measured period:**
   ```
   ΔT = 1.000027 s
   ```

4. **Calculated error:**
   ```
   Error = 1.000027 - 1.000000 = +27 μs
   PPM = 27/10^6 = +27 ppm
   ```

5. **Interpretation:**
   - Scope timebase runs 27 ppm FAST
   - At 1 second: reads 1.000027 s (27 μs error)
   - At 1 ms: reads 1.000027 ms (27 ns error)
   - At 1 MHz: reads 1.000027 MHz (27 Hz error)

6. **Applied correction:**
   ```
   Correction factor: 1.000000 / 1.000027 = 0.999973
   Multiply all scope time readings by 0.999973
   ```

7. **Created label:**
   ```
   "TIMEBASE: +27 ppm FAST
    Correction: × 0.999973
    Cal date: 2026-01-02"
   ```

**Result:** Know scope accuracy to atomic clock standards!

---

## Summary

### What We Achieved

✓ Measured scope timebase error using GPS
✓ Accuracy limited by GPS: ±0.01 ppm
✓ Documented correction factor
✓ Can now make accurate time measurements

### Key Points

1. **GPS 1PPS is perfect reference** - atomic clock accuracy
2. **Scope measures period** - compare to exact 1.000000 s
3. **Calculate PPM error** - quantify timebase accuracy
4. **Apply correction** - mental math or label on scope
5. **Verify regularly** - monthly check, annual full cal

### Next Step

**Voltage calibration:** [oscilloscope_voltage_cal.md](oscilloscope_voltage_cal.md)

---

**Timebase now calibrated to GPS atomic clock accuracy!**
