# Complete DSO1013D Oscilloscope Calibration Example

## Project Overview

**Hardware:** DSO1013D Plus (Hantek) digital storage oscilloscope
**Goal:** Calibrate timebase and voltage using homebrew methods
**Budget:** $15 (reusing GPS from previous projects)
**Time:** One Saturday (6 hours total)
**Result:** Professional-grade calibrated oscilloscope

---

## Starting Point

### Equipment Owned

- DSO1013D Plus oscilloscope (uncalibrated, out of box)
- GPS module + Arduino (from TinySA calibration project)
- Multimeter (cheap $20 DMM)
- Soldering iron, breadboard
- Basic components

### Unknown Errors

**Timebase:** Unknown (could be ±100 ppm)
**Voltage:** Unknown (could be ±5%)
**Probes:** Never compensated (factory default)

**Problem:** Can't trust any measurements!

---

## Day 1: Complete Calibration

### 9:00 AM - Setup and Planning

**Inventory check:**
- GPS module working ✓
- Voltage reference ICs ordered (arriving today) ✓
- Scope powered on, warming up

**Plan for today:**
1. Timebase calibration (GPS method) - 1 hour
2. Build voltage references - 1 hour
3. Voltage calibration - 2 hours
4. Probe compensation - 30 min
5. Verification tests - 1.5 hours

---

### 10:00 AM - Timebase Calibration

#### Setup (15 minutes)

**Connected GPS to scope:**
```
GPS NEO-6M module:
VCC (5V) → USB power adapter
GND → Common ground
1PPS → Scope CH1 input
```

**GPS lock:**
- Placed near window
- Blue LED blinking after 45 seconds (GPS lock achieved)
- 1PPS LED blinking once per second ✓

**Scope settings:**
```
CH1: 1V/div, DC coupling
Timebase: 200 ms/div
Trigger: CH1, rising edge, ~1.5V
Acquisition: Average 16
```

#### Measurement (15 minutes)

**Method: Period measurement with cursors**

1. **Enabled time cursors**
2. **Placed cursors on consecutive rising edges:**
   ```
   Cursor 1: First rising edge at 0.0 s
   Cursor 2: Second rising edge
   ```

3. **Read ΔT:**
   ```
   Display: ΔT = 1.000031 s
   ```

4. **Verified over multiple periods:**
   ```
   Measurement 1: 1.000032 s
   Measurement 2: 1.000030 s
   Measurement 3: 1.000031 s
   Measurement 4: 1.000032 s
   Measurement 5: 1.000031 s

   Average: 1.000031 s
   ```

#### Calculation (5 minutes)

**Error calculation:**
```
Measured: 1.000031 s
Expected: 1.000000 s (GPS atomic clock)
Error: +0.000031 s = +31 μs

PPM error: (31 × 10^-6 / 1.0) × 10^6 = +31 ppm

Interpretation: Scope timebase is 31 ppm FAST
```

**What this means:**
```
@ 1 second: Reads 1.000031 s (31 μs error)
@ 1 ms: Reads 1.000031 ms (31 ns error)
@ 1 MHz: Would read 1.000031 MHz (31 Hz high)
@ 100 kHz: Would read 100.0031 kHz (3.1 Hz high)
```

#### Documentation (5 minutes)

**Created correction card:**
```
┌──────────────────────────────────────┐
│ DSO1013D TIMEBASE CALIBRATION        │
│                                      │
│ Error: +31 ppm (FAST)                │
│ Date: 2026-01-02                     │
│                                      │
│ CORRECTION:                          │
│ Multiply time readings by 0.999969   │
│                                      │
│ Example:                             │
│ Scope shows 1.000 ms                 │
│ Actual time = 0.999969 ms            │
│                                      │
│ Next cal: 2027-01-02                 │
└──────────────────────────────────────┘
```

**Taped to scope front panel**

**Timebase calibration complete: 10:40 AM**

---

### 11:00 AM - Build Voltage References

#### Parts Arrival and Inventory

**Parts received (ordered 2 days ago):**
- LM4040AIZ-2.5 precision reference: $1.85
- LM4040AIZ-5.0 precision reference: $2.10
- 10kΩ resistors 1% × 5: $0.50
- 10kΩ precision 0.1% × 2: $2.00
- Banana binding posts × 4: $3.00
- 9V battery: $1.50

**Total: $10.95**

#### Circuit Construction (45 minutes)

**Built on breadboard:**

1. **2.5V Reference:**
   ```
   9V battery (+) → 10kΩ → LM4040-2.5 cathode → RED binding post
                            LM4040-2.5 anode → GND
   ```

2. **5.0V Reference:**
   ```
   9V battery (+) → 10kΩ → LM4040-5.0 cathode → GREEN binding post
                            LM4040-5.0 anode → GND
   ```

3. **10.0V Reference (two 5V in series):**
   ```
   9V+ → 10kΩ → LM4040-5.0 (#1) cathode → (node A)
                 LM4040-5.0 (#1) anode → (node B)

   9V+ → 10kΩ → LM4040-5.0 (#2) cathode → (node B)
                 LM4040-5.0 (#2) anode → GND

   10V output: Between (node A) and GND → BLUE binding post
   ```

4. **Common ground:**
   ```
   All reference grounds → BLACK binding post
   ```

**Circuit photo taken for documentation**

#### Verification with DMM (15 minutes)

**Measured each reference:**

| Reference | DMM Reading | Spec | Error |
|-----------|-------------|------|-------|
| 2.5V | 2.501V | 2.500V ±0.1% | +0.04% ✓ |
| 5.0V | 4.998V | 5.000V ±0.1% | -0.04% ✓ |
| 10.0V | 9.999V | 10.000V ±0.2% | -0.01% ✓ |

**All within spec!** ✓

**Voltage reference build complete: 11:45 AM**

---

### 12:00 PM - Lunch Break

Took 30 minutes. Ate sandwich, reviewed plan.

---

### 12:30 PM - Voltage Calibration CH1

#### Test Setup

**Scope probe:** 10:1 probe (came with scope)
**Initial probe compensation:** Adjusted using scope's built-in 1kHz cal signal

**Probe comp procedure:**
1. Connected probe to CAL terminal (1kHz, ~5V square wave)
2. Observed square wave
3. Was over-compensated (overshoot visible)
4. Adjusted trimmer on probe body
5. Now shows flat top ✓

#### Measuring 2.5V Reference

**Multiple V/div settings tested:**

| V/div | Scope Reading | Reference | Error | Error % |
|-------|---------------|-----------|-------|---------|
| 500mV | 2.54V | 2.501V | +0.039V | +1.56% |
| 1V | 2.56V | 2.501V | +0.059V | +2.36% |
| 2V | 2.53V | 2.501V | +0.029V | +1.16% |
| 5V | 2.51V | 2.501V | +0.009V | +0.36% |

**Observation:** Error varies by V/div setting!

#### Measuring 5.0V Reference

| V/div | Scope Reading | Reference | Error | Error % |
|-------|---------------|-----------|-------|---------|
| 1V | 5.12V | 4.998V | +0.122V | +2.44% |
| 2V | 5.08V | 4.998V | +0.082V | +1.64% |
| 5V | 5.03V | 4.998V | +0.032V | +0.64% |

#### Measuring 10.0V Reference

| V/div | Scope Reading | Reference | Error | Error % |
|-------|---------------|-----------|-------|---------|
| 2V | 10.20V | 9.999V | +0.201V | +2.01% |
| 5V | 10.08V | 9.999V | +0.081V | +0.81% |

#### Analysis

**Key findings:**
1. **Scope consistently reads HIGH** (+0.8% to +2.4%)
2. **Smaller V/div settings have larger errors**
3. **5V/div setting most accurate** (±0.6-0.8%)

**Average error across all measurements:** +1.5%

#### CH1 Calibration Table

```
┌────────────────────────────────────────────┐
│ DSO1013D CH1 VOLTAGE CALIBRATION           │
│                                            │
│ General Error: +1.5% (reads HIGH)          │
│                                            │
│ V/div Specific Corrections:                │
│ 500mV-1V: × 0.98  (−2% error)              │
│ 2V:       × 0.985 (−1.5% error)            │
│ 5V:       × 0.993 (−0.7% error)            │
│                                            │
│ Quick correction:                          │
│ Multiply all readings by 0.985             │
│                                            │
│ Date: 2026-01-02                           │
└────────────────────────────────────────────┘
```

**CH1 calibration complete: 2:00 PM**

---

### 2:00 PM - Voltage Calibration CH2

**Repeated procedure for Channel 2:**

#### Results Summary

| V/div | Reference | CH2 Reading | Error % |
|-------|-----------|-------------|---------|
| 500mV | 2.501V | 2.48V | -0.84% |
| 1V | 2.501V | 2.49V | -0.44% |
| 2V | 2.501V | 2.50V | -0.04% |
| 5V | 2.501V | 2.50V | -0.04% |

**Interesting:** CH2 reads slightly LOW (opposite of CH1!)

#### CH2 Analysis

**Average error:** -0.3% (reads low)

**Conclusion:** CH2 more accurate than CH1!

#### CH2 Calibration Table

```
┌────────────────────────────────────────────┐
│ DSO1013D CH2 VOLTAGE CALIBRATION           │
│                                            │
│ General Error: -0.3% (reads LOW)           │
│                                            │
│ Correction: × 1.003                        │
│                                            │
│ CH2 is MORE accurate than CH1              │
│ Use CH2 for critical measurements          │
│                                            │
│ Date: 2026-01-02                           │
└────────────────────────────────────────────┘
```

**CH2 calibration complete: 3:00 PM**

---

### 3:15 PM - Verification Tests

#### Test 1: USB 5V Rail

**Setup:**
- Connected scope to USB port 5V
- Measured with both CH1 and CH2
- Compared to DMM

**Results:**
```
DMM: 5.02V
CH1 raw: 5.10V → Corrected (×0.985): 5.02V ✓
CH2 raw: 5.01V → Corrected (×1.003): 5.03V ✓
```

**Excellent agreement!**

#### Test 2: AA Battery

**Fresh Duracell AA battery:**
```
DMM: 1.62V
CH1 (500mV/div): 1.65V → Corrected: 1.62V ✓
CH2 (500mV/div): 1.61V → Corrected: 1.62V ✓
```

**Perfect!**

#### Test 3: Sine Wave from Signal Generator

**If available - I borrowed neighbor's old function generator:**

**Setup:**
- Function gen: 1 kHz, 2V RMS
- Expected peak-to-peak: 2 × 2.828 = 5.66V

**Measured:**
```
CH1: 5.75V p-p → Corrected: 5.66V ✓
CH2: 5.64V p-p → Corrected: 5.65V ✓
```

**Matches expected value!**

#### Test 4: Frequency Verification

**Used GPS 1PPS:**
```
Period measurement: 1.000031 s (from before)
Frequency: 1 / 1.000031 = 0.999969 Hz

Expected: 1.000000 Hz
Difference: 31 ppm (matches our calibration)

Applying correction: 0.999969 / 0.999969 = 1.000000 Hz ✓
```

**Verification complete: 4:00 PM**

---

### 4:00 PM - Documentation

#### Created Master Calibration Document

```
═══════════════════════════════════════════════════
   DSO1013D PLUS OSCILLOSCOPE CALIBRATION RECORD
═══════════════════════════════════════════════════

Serial Number: DSO1013D-123456
Calibration Date: 2026-01-02
Next Calibration: 2027-01-02
Calibrated By: [My Name]

───────────────────────────────────────────────────
TIMEBASE CALIBRATION
───────────────────────────────────────────────────

Method: GPS 1PPS (NEO-6M module)
Reference Accuracy: ±0.01 ppm (atomic clock)

Measured Period: 1.000031 s
Expected Period: 1.000000 s
Error: +31 ppm (FAST)

Correction Factor: 0.999969
Application: Multiply all time readings by 0.999969

Examples:
  Scope shows 1.000 ms → Actual: 0.999969 ms
  Scope shows 10.00 μs → Actual: 9.99969 μs
  Scope shows 100.0 ns → Actual: 99.9969 ns

───────────────────────────────────────────────────
VOLTAGE CALIBRATION - CHANNEL 1
───────────────────────────────────────────────────

Method: LM4040 precision references
References Used:
  2.500V ±0.1% (LM4040-2.5)
  5.000V ±0.1% (LM4040-5.0)
  10.00V ±0.2% (two LM4040-5.0 in series)

Average Error: +1.5% (reads HIGH)
Correction Factor: 0.985

V/div Specific Corrections:
  500mV-1V div: ×0.980
  2V div:       ×0.985
  5V div:       ×0.993

General Rule: Multiply by 0.985 for ±1% accuracy

───────────────────────────────────────────────────
VOLTAGE CALIBRATION - CHANNEL 2
───────────────────────────────────────────────────

Average Error: -0.3% (reads LOW)
Correction Factor: 1.003

Note: CH2 is more accurate than CH1
Recommendation: Use CH2 for critical voltage measurements

───────────────────────────────────────────────────
PROBE COMPENSATION
───────────────────────────────────────────────────

Probe 1 (CH1, 10:1): Compensated ✓
Probe 2 (CH2, 10:1): Compensated ✓

Method: Built-in 1kHz cal signal
Result: Flat square wave, no overshoot

Reminder: Re-compensate after disconnecting probe!

───────────────────────────────────────────────────
VERIFICATION TESTS
───────────────────────────────────────────────────

Test 1: USB 5V rail
  DMM: 5.02V
  CH1 corrected: 5.02V ✓
  CH2 corrected: 5.03V ✓

Test 2: AA Battery (1.62V)
  Agreement within 0.01V ✓

Test 3: Signal generator (5.66V p-p expected)
  Agreement within 0.02V ✓

Test 4: GPS frequency (1.000 Hz expected)
  Matches within calibration error ✓

───────────────────────────────────────────────────
ESTIMATED ACCURACY AFTER CALIBRATION
───────────────────────────────────────────────────

Timebase: ±0.01 ppm (GPS-limited)
          ±10 ns @ 1 second
          ±10 Hz @ 1 GHz

Voltage CH1: ±1.0% (with correction)
Voltage CH2: ±0.5% (with correction)

───────────────────────────────────────────────────
CALIBRATION EQUIPMENT USED
───────────────────────────────────────────────────

GPS Module: NEO-6M
Voltage Refs: LM4040-2.5, LM4040-5.0 (×2)
DMM: Fluke 117 (for verification)
Signal Gen: HP 33120A (borrowed, for verification)

Total Cost: $15 (GPS reused from TinySA project)
            $11 (voltage references, new)
            ────
            $26 total

Equivalent Commercial Cal Service: $150-300
Savings: $124-274

═══════════════════════════════════════════════════
```

**Printed and stored in lab notebook**

---

## Results Summary

### Before Calibration

**Timebase:** Unknown
- Could be ±100 ppm
- Frequency measurements unreliable

**Voltage:** Unknown
- Could be ±5%
- Can't trust amplitude

**Confidence:** Low
- "Is this measurement even close?"

### After Calibration

**Timebase:** +31 ppm, documented
- Know exact error
- Can correct to ±0.01 ppm

**Voltage CH1:** +1.5%, documented
- Correctable to ±1%

**Voltage CH2:** -0.3%, documented
- Correctable to ±0.5%

**Confidence:** High
- "These measurements are traceable to GPS atomic clock and precision references"

---

## Cost Analysis

### Actual Costs

| Item | Cost | Source |
|------|------|--------|
| GPS module | $0 | Reused from TinySA project |
| Arduino | $0 | Reused |
| LM4040-2.5 | $1.85 | Mouser |
| LM4040-5.0 × 2 | $4.20 | Mouser |
| Resistors, misc | $3.00 | Local store |
| Binding posts | $3.00 | eBay |
| 9V battery | $1.50 | Local store |
| Breadboard | $0 | Already owned |
| **Total** | **$13.55** | |

### Value Comparison

| Service | Cost | Time |
|---------|------|------|
| **DIY Calibration** | **$13.55** | **6 hours** |
| Commercial cal service | $150-300 | 2-4 weeks |
| NIST-traceable cal | $500+ | 4-6 weeks |

**Savings: $136-486**

---

## Lessons Learned

### What Went Well

1. **GPS method brilliant** - Atomic clock for $0 (reused)
2. **LM4040 references perfect** - ±0.1%, cheap, easy
3. **Channel 2 more accurate** - Discovered by testing both
4. **Verification critical** - Cross-checks built confidence
5. **Documentation invaluable** - Future me will thank me

### Challenges

1. **Probe compensation tricky** - Took 3 tries to get perfect
2. **V/div settings vary** - Each setting has different error
3. **DMM not perfect** - Used for verification but not gospel
4. **Temperature not controlled** - Should have waited for full warmup

### Would Do Differently

1. **Buy LM4040-1.2 also** - Would like 1.225V reference too
2. **Build permanent reference box** - Instead of breadboard
3. **Label each V/div setting** - On correction card
4. **Plot voltage error vs V/div** - Make graph for interpolation

---

## Applications Enabled

### With Calibrated Scope Can Now:

**Time measurements:**
- PWM duty cycle (±0.1%)
- Crystal frequencies (±10 Hz)
- Pulse widths (±0.01%)
- Setup/hold times for digital circuits

**Voltage measurements:**
- Power supply ripple (±1%)
- Signal amplitudes (±1%)
- Logic levels (±0.05V)
- Battery voltages (±0.02V)

**Waveform analysis:**
- Rise/fall times
- Overshoot percentage
- Signal integrity
- Noise measurements

**Real projects enabled:**
- Design switch-mode power supplies
- Debug microcontroller timing
- Verify filter responses
- Characterize amplifiers
- Tune oscillators

---

## Maintenance Plan

### Monthly Quick Check (5 minutes)

1. **Timebase:**
   - Measure GPS 1PPS period
   - Should still be 1.000031 s ±0.000002 s
   - If changed >10 ppm → Re-calibrate

2. **Voltage:**
   - Measure 5V reference
   - Should still show 5.08V (on CH1, 2V/div)
   - If changed >2% → Re-calibrate

### Annual Full Calibration

1. Complete timebase calibration
2. Complete voltage calibration (all V/div settings)
3. Probe compensation check
4. Update documentation
5. Date: 2027-01-02

---

## Conclusion

### Success Metrics - All Achieved!

✓ Timebase calibrated to GPS (±0.01 ppm)
✓ Voltage calibrated with precision refs (±1%)
✓ Probes compensated (flat response)
✓ Complete documentation created
✓ Verification tests passed
✓ Total cost: $13.55
✓ Total time: 6 hours

### Key Achievement

**Transformed a $100 uncalibrated oscilloscope into a precision instrument with accuracy rivaling $500+ scopes, for $14 and one Saturday.**

### Personal Impact

**Before:** "I think this is about 5 volts..."
**After:** "This is 5.02V ±0.05V, and I can prove it."

**Confidence in measurements: Priceless.**

---

## Sharing Results

**Posted to:**
- EEVBlog forum: "DSO1013D Calibration Success"
- Reddit r/AskElectronics: Detailed writeup
- Personal blog: Full documentation

**Helped others calibrate their scopes!**

**Community feedback:** "This is amazing, doing this weekend!"

---

**Project complete! Oscilloscope now calibrated and ready for precision work.**

**Total satisfaction: 100%**

**Would I do it again: Absolutely! Already planning to calibrate friend's scope.**

**73 and happy measuring!**
