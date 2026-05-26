# Oscilloscope Voltage Calibration

## Overview

Voltage calibration ensures accurate amplitude measurements. We'll build precision voltage references using cheap ICs and measure scope accuracy at each voltage range.

**Key Idea:** Use $2 precision voltage reference ICs as "voltage rulers" to check scope accuracy.

---

## Building Precision Voltage References

### Parts List

| Qty | Part | Voltage | Accuracy | Cost | Source |
|-----|------|---------|----------|------|--------|
| 1 | LM4040-1.2 | 1.225V | ±0.1% | $1.50 | Mouser, Digikey |
| 1 | LM4040-2.5 | 2.500V | ±0.1% | $1.50 | Mouser, Digikey |
| 1 | LM4040-5.0 | 5.000V | ±0.1% | $2.00 | Mouser, Digikey |
| 2 | Resistor 10kΩ 0.1% | - | ±0.1% | $1.00 | Mouser, Digikey |
| 1 | 9V battery | ~9V | ±5% | $1.00 | Local store |
| 1 | Breadboard | - | - | $3.00 | Local store |

**Total:** ~$10-15

### Circuit Schematic

**LM4040 Basic Circuit:**
```
        9V Battery (+)
            │
            ├─────→ To other refs
            │
         10kΩ
            │
            ├────→ OUTPUT (1.225V, 2.5V, or 5V)
            │       │
        ┌───┴───┐   │
        │ LM4040│   ├───→ Scope probe
        │       │   │
        └───┬───┘   │
            │       │
           GND ─────┴───→ Scope ground
```

**Complete Reference Board (4 voltages):**
```
9V Battery
   │
   ├─ 10kΩ ─┬─ LM4040-1.2 ─→ 1.225V output ─→ Banana jack RED
   │        │    │
   ├─ 10kΩ ─┼─ LM4040-2.5 ─→ 2.500V output ─→ Banana jack YEL
   │        │    │
   ├─ 10kΩ ─┼─ LM4040-5.0 ─→ 5.000V output ─→ Banana jack GRN
   │        │    │
   │        │    │
   │    Precision voltage divider:
   │        │
   │      10kΩ (0.1%) ─┬─→ 10.00V output ─→ Banana jack BLU
   │                   │   (from 5V ref × 2)
   │                 10kΩ (0.1%)
   │                   │
   └──────────────── GND ─────────────────→ Banana jack BLK
```

### Building the Circuit

**Step 1: LM4040 Connections** (30 minutes)

1. **Identify LM4040 pins:**
   ```
   LM4040 (TO-92 package, looking at flat side):

   [Cathode] [Anode] [NC]
       1        2      3

   Pin 1 (Cathode) = Output voltage
   Pin 2 (Anode) = Ground
   Pin 3 (NC) = Not connected
   ```

2. **Breadboard layout:**
   ```
   Row 1: 9V+
   Row 2: 10kΩ → LM4040-1.2 cathode → 1.225V output
   Row 3: LM4040-1.2 anode → GND

   Row 5: 9V+
   Row 6: 10kΩ → LM4040-2.5 cathode → 2.500V output
   Row 7: LM4040-2.5 anode → GND

   Row 9: 9V+
   Row 10: 10kΩ → LM4040-5.0 cathode → 5.000V output
   Row 11: LM4040-5.0 anode → GND
   ```

3. **Connect outputs to binding posts** (for easy scope connection)

**Step 2: 10V Divider** (15 minutes)

**From 5V reference to 10V:**
```
Actually, easier method: Use two 5V refs in series!

9V+ ─ 10kΩ ─ LM4040-5.0 (REF1) ─┬─ 5V output
                                  │
             10kΩ ─ LM4040-5.0 (REF2) ─┬─ 10V output (REF1+REF2)
                                        │
                                       GND
```

**Or use precision divider:**
```
5V reference ─ 10kΩ (0.1%) ─┬─ 10V output
                             │
              10kΩ (0.1%) ──┴─ GND

Wait, this gives 2.5V, not 10V!

Correct circuit:
Need op-amp to create 10V from 5V, or use two refs in series.

Simplest: Two LM4040-5.0 in series = 10V
```

### Testing References with DMM

**Verify each reference:**

1. **1.225V reference:**
   ```
   DMM reading: 1.224V to 1.226V (±0.1%)
   ```

2. **2.500V reference:**
   ```
   DMM reading: 2.498V to 2.502V (±0.1%)
   ```

3. **5.000V reference:**
   ```
   DMM reading: 4.995V to 5.005V (±0.1%)
   ```

4. **10.00V (if built):**
   ```
   Two 5V in series: 9.99V to 10.01V (±0.1%)
   ```

**If readings are off:** Check wiring, polarity, battery voltage >7V

---

## Calibration Procedure

### Step 1: Prepare Oscilloscope

1. **Warm up:** 15-30 minutes powered on
2. **Probe:** 10:1 probe (or 1:1 for low voltages)
3. **Settings:**
   - Coupling: DC
   - Bandwidth: Full (not limited)
   - Trigger: Normal, not Auto

### Step 2: Measure Each Reference

**1.225V Reference:**

1. **Connect:**
   - Scope probe to 1.225V output
   - Ground clip to GND

2. **Scope settings:**
   - V/div: 500 mV/div (to see ~1.2V signal)
   - Timebase: Doesn't matter (DC signal)

3. **Read voltage:**
   - Use MEASURE → DC voltage
   - Or use cursor at trace level

4. **Record:**
   ```
   Reference: 1.225V
   Scope reads: _____ V
   Error: _____ V
   Error %: _____ %
   ```

**Repeat for all references:**
- 2.500V
- 5.000V
- 10.00V (if built)

**Test multiple V/div settings:**

For 5V reference, test:
- 1V/div (scope reading: ___)
- 2V/div (scope reading: ___)
- 5V/div (scope reading: ___)

**Each setting may have different error!**

### Step 3: Calculate Errors

**Example measurements:**
```
Reference: 5.000V
V/div Setting    Scope Reads    Error (V)    Error (%)
─────────────────────────────────────────────────────
1V/div           5.12V          +0.12        +2.4%
2V/div           5.08V          +0.08        +1.6%
5V/div           5.05V          +0.05        +1.0%

Conclusion: Error varies by V/div setting!
Scope is generally reading HIGH (+1 to +2.4%)
```

### Step 4: Document Corrections

**Create calibration table:**
```
DSO1013D CH1 Voltage Calibration

Reference    V/div    Reading    Error    Correction
─────────────────────────────────────────────────────
1.225V       500mV    1.24V      +1.2%    ×0.988
2.500V       1V       2.54V      +1.6%    ×0.984
5.000V       2V       5.08V      +1.6%    ×0.984
5.000V       5V       5.05V      +1.0%    ×0.990

Average error: +1.35%
Average correction factor: ×0.987

CONCLUSION: Scope reads 1.35% HIGH
Multiply displayed voltage by 0.987 for actual voltage
```

**Repeat for CH2!** (Each channel may differ)

---

## Probe Compensation

**Critical for AC measurements!**

### Using Built-in Cal Signal

**Most scopes have ~1 kHz square wave output:**

1. **Locate cal output:**
   - Usually labeled "CAL", "PROBE COMP", or "1kHz"
   - Typically 1 kHz, 5V peak-to-peak square wave

2. **Connect probe:**
   - Probe tip to CAL signal
   - Ground clip to GND

3. **Observe square wave:**
   ```
   Good (compensated):
   ┌──┐  ┌──┐
   │  │  │  │
   ┘  └──┘  └──

   Under-compensated:
   ┌──┐  ┌──┐
   ╱  │  ╱  │
   ┘  └──┘  └──
   (Rounded edges, looks like RC filter)

   Over-compensated:
   ┌─┐   ┌─┐
   │ ╲╱  │ ╲╱
   ┘  └──┘  └──
   (Overshoot, ringing)
   ```

4. **Adjust probe:**
   - Find small trimmer capacitor on probe body
   - Use plastic screwdriver
   - Adjust until square wave has flat top
   - Should look crisp, no overshoot

**Do this for EVERY probe, EVERY time you connect it!**

### Without Built-in Cal

**Build external square wave:**

**Simple 555 timer circuit:**
```
555 timer configured as astable
Frequency: ~1 kHz
Output: 5V square wave

       +5V
        │
     ┌──┴──┐
     │ 555 │
     │timer│
     └──┬──┘
        │
     Output → Scope
```

**Or use function generator** (if available)

---

## Verification

### Cross-Check with Known Voltages

**Battery voltage:**
```
Fresh AA battery: 1.5V - 1.65V
Measure with scope and DMM:
Should agree within ±3%
```

**USB 5V:**
```
USB port: 4.75V - 5.25V (spec)
Measure with scope
Compare to DMM
```

**Known signal:**
```
If you have signal generator:
Set to 1V RMS sine wave
Scope should show 2.83V peak-to-peak (1V × 2√2)
```

---

## Complete Example

### Calibrating DSO1013D Voltage

**Equipment:**
- DSO1013D oscilloscope
- LM4040-2.5 reference (built)
- DMM (for verification)

**Procedure:**

1. **Built 2.5V reference**
   - LM4040AIZ-2.5
   - 9V battery, 10kΩ resistor
   - Breadboard

2. **Verified with DMM:**
   ```
   DMM reading: 2.501V ✓
   (within ±0.1% spec)
   ```

3. **Connected to scope CH1:**
   - 10:1 probe
   - V/div: 1V (to see ~2.5V clearly)
   - Coupling: DC

4. **Measured on scope:**
   ```
   Scope reading: 2.58V
   ```

5. **Calculated error:**
   ```
   Reference: 2.501V (DMM-verified)
   Scope: 2.58V
   Error: +0.079V
   Error %: (0.079 / 2.501) × 100 = +3.2%
   ```

6. **Tested other V/div settings:**
   ```
   V/div    Scope Reading    Error %
   500mV    2.56V            +2.4%
   1V       2.58V            +3.2%
   2V       2.54V            +1.6%
   5V       2.52V            +0.8%

   Average error: +2.0%
   ```

7. **Created correction table:**
   ```
   CH1: Reads ~2% HIGH
   Correction: Multiply by 0.98

   Example:
   Scope shows 5.0V → Actual is 4.9V
   Scope shows 3.3V → Actual is 3.23V
   ```

8. **Labeled scope:**
   ```
   "CH1: +2.0% error
    CH2: (test separately)
    Multiply readings by 0.98"
   ```

**Result:** Know voltage accuracy to ±0.5%!

---

## Alternative: Zener Diode References

**If can't get LM4040:**

**Common zener voltages:**
- 3.3V zener diode (cheap, ~5% accuracy)
- 5.1V zener diode (cheap, ~5% accuracy)

**Circuit:**
```
9V ─ 1kΩ ─┬─ 5.1V zener ─ GND
           │
        Output (≈5.1V ±5%)
```

**Less accurate but usable for rough calibration**

---

## Summary

### Voltage Calibration Achieved

✓ Built precision references (±0.1%)
✓ Measured scope at multiple voltages
✓ Documented errors per V/div setting
✓ Created correction table
✓ Probe compensation verified

### Key Findings

1. **Scope errors typical:** ±2-5% uncalibrated
2. **Each V/div setting different:** Must test all
3. **CH1 and CH2 differ:** Calibrate separately
4. **Probe compensation critical:** Do every time!

### Accuracy Achieved

**Before calibration:** Unknown, possibly ±5%
**After calibration:** Known to ±0.5-1%

---

**Next:** [Complete example](examples/dso1013d_complete_calibration.md)

**Voltage calibration complete!**
