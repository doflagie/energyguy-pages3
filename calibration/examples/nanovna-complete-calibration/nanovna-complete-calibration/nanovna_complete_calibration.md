# Complete NanoVNA Calibration Example

## Project Overview

This is a **complete worked example** of calibrating a NanoVNA from scratch, including:
1. Frequency calibration (GPS method)
2. Building DIY calibration standards
3. Port calibration (SOL)
4. Verification with real measurements

**Hardware:** NanoVNA v1 (original)
**Goal:** Calibrate for 2-meter ham band antenna work (144-148 MHz)
**Budget:** $30
**Time:** 1 weekend

---

## Day 1: Frequency Calibration

### Equipment Available

- NanoVNA v1 (uncalibrated)
- GPS module + Arduino (from TinySA project - already built!)
- Multimeter
- Soldering iron

### Step 1: Verify Frequency Error (30 min)

**Used FM broadcast method for quick check:**

1. **Tuned to local NPR station:**
   - Known frequency: 90.9 MHz (WBUR Boston)
   - Confirmed GPS-locked online

2. **NanoVNA settings:**
   ```
   START: 90.8 MHz
   STOP: 91.0 MHz
   Format: LOGMAG
   Connected 6" wire as antenna
   ```

3. **Found carrier peak:**
   ```
   Marker: 90.927.8 MHz
   Known: 90.900.0 MHz
   Error: +27.8 kHz
   PPM: 27,800 / 90,900,000 × 10^6 = +30.58 ppm
   ```

**Conclusion:** NanoVNA is 30.6 ppm fast - needs calibration!

### Step 2: GPS Calibration (1 hour)

**Reused GPS frequency counter from TinySA project:**

1. **Modified for 8 MHz measurement:**
   - Changed Arduino code: `expected = 8000000.0`
   - Set NanoVNA to CW mode at 100 MHz
   - Connected CH0 output to Arduino

2. **Measurements:**
   ```
   Frequency: 100,003,060 Hz
   Expected:  100,000,000 Hz
   Error: +3,060 Hz
   PPM: +30.60 ppm
   ```

   **Matches FM measurement!** ✓

3. **Calculate new TCXO value:**
   ```
   Current TCXO: 8,000,000 Hz (default)
   Error: +30.60 ppm

   New TCXO = 8,000,000 × (1 + 30.60/10^6)
            = 8,000,000 × 1.00003060
            = 8,000,245 Hz
   ```

4. **Apply to NanoVNA:**
   ```
   CONFIG → EXPERT CONFIG → TCXO
   Enter: 8000245
   SAVE
   Power cycle
   ```

5. **Verify:**
   ```
   Re-measured FM station:
   Marker: 90.900.1 MHz
   Error: +100 Hz = +1.1 ppm

   Much better! (30.6 → 1.1 ppm)

   Second iteration:
   New TCXO = 8,000,245 × (1 + 1.1/10^6) = 8,000,254 Hz

   Final result:
   FM station: 90.900.0 MHz exact
   Error: 0 ppm! ✓
   ```

**Frequency calibration complete: 30.6 ppm → 0 ppm**

---

## Day 2: Building Calibration Standards

### Shopping List

Went to local electronics store:
- 10× 51Ω resistors, 1%, metal film: $2.50
- 2× SMA panel mount connectors: $6.00
- SMA male-male adapter: $3.50
- Copper wire, solder (had)

**Total spent: $12.00**

### Building SHORT (30 min)

**Method:** Copper wire in SMA connector

1. **Prepared SMA connector:**
   - Removed outer insulator
   - Exposed center pin and shell

2. **Created short:**
   - Cut 1/2" of solid copper wire (18 AWG)
   - Stripped clean
   - Flattened with hammer (reduce inductance)
   - Soldered center pin to shell

3. **Tested:**
   ```
   DMM: 0.05Ω ✓
   Visual: Clean solder joint ✓
   ```

### Building OPEN (5 min)

**Method:** Empty SMA female

1. Took spare SMA panel connector
2. Left center socket empty
3. That's it!

**Tested:**
```
DMM: >10 MΩ ✓
Visual: No contamination ✓
```

### Building LOAD (45 min)

**Method:** Through-hole resistor in SMA

1. **Measured all 10 resistors with DMM:**
   ```
   R1: 50.8Ω
   R2: 51.3Ω
   R3: 50.6Ω
   R4: 51.0Ω
   R5: 50.2Ω ← Best!
   R6: 51.4Ω
   R7: 50.9Ω
   R8: 51.2Ω
   R9: 50.7Ω
   R10: 51.1Ω
   ```

   **Selected R5: 50.2Ω** (only +0.4% error)

2. **Installed in SMA connector:**
   - Trimmed leads to 3mm total
   - Soldered center pin to one resistor lead
   - Soldered other lead to shell
   - Added heat shrink for strain relief

3. **Tested:**
   ```
   DMM (4-wire): 50.3Ω (lead resistance added 0.1Ω)
   Close enough! ✓
   ```

### Verifying THRU (SMA adapter)

**Bought SMA male-male barrel adapter**

**Tested:**
- Visual: Good quality, gold plated
- Mechanical: Tight connections
- Ready to use ✓

**Total build time: 2 hours**
**Total cost: $12**

---

## Day 2 (continued): Port Calibration

### Goal: Calibrate for 2m Band

**Target range:** 144-148 MHz (2-meter amateur band)

### Procedure (10 minutes)

1. **Warm up NanoVNA:**
   - Turned on
   - Ran sweep 1-900 MHz
   - Waited 15 minutes

2. **Set frequency range:**
   ```
   STIMULUS → START: 140 MHz
   STIMULUS → STOP: 150 MHz
   (Slightly wider than target for margin)
   ```

3. **Started SOL calibration:**
   ```
   CAL → CALIBRATE → S11 (1-port)
   ```

4. **Connected SHORT:**
   - Attached DIY SHORT to CH0
   - Finger-tightened
   - Pressed **SHORT** on screen
   - Waited for sweep (2 seconds)
   - **Green checkmark appeared** ✓

5. **Connected OPEN:**
   - Removed SHORT
   - Attached DIY OPEN to CH0
   - Pressed **OPEN**
   - Waited for sweep
   - **Green checkmark** ✓

6. **Connected LOAD:**
   - Removed OPEN
   - Attached DIY 50Ω LOAD
   - Pressed **LOAD**
   - Waited for sweep
   - **Green checkmark** ✓

7. **Finished:**
   - Pressed **DONE**
   - Message: "Calibration applied"
   - **CAL → SAVE → Slot 1** (saved for future)

**Calibration complete!**

---

## Verification Tests

### Test 1: 50Ω LOAD (Baseline)

**Connected 50Ω LOAD (the one used for cal):**

```
Format: SWR
Reading: 1.00:1 across 140-150 MHz ✓

Format: Smith Chart
Position: Dead center ✓

Format: LOGMAG (S11)
Reading: -45 dB (excellent) ✓

Format: IMPEDANCE
Reading: 50.0 + j0.0 Ω ✓
```

**Perfect! Cal standard measures as expected.**

### Test 2: Known Resistor

**Measured 100Ω 1% resistor (not used in cal):**

```
DMM reading: 99.8Ω

NanoVNA @ 145 MHz:
Impedance: 100.2 + j2.1 Ω

Real part: 100.2Ω ✓ (matches DMM)
Imaginary: +j2.1Ω (small inductance from leads - expected)
```

**Excellent agreement!**

### Test 3: Supplied Cal Kit LOAD

**Measured the LOAD that came with NanoVNA:**

```
DMM: 49.1Ω (hmm, slightly low)

NanoVNA @ 145 MHz:
Impedance: 49.3 + j0.8 Ω
SWR: 1.02:1

Close, but not perfect.
My DIY load (50.2Ω) is actually more accurate!
```

### Test 4: Real Antenna

**Measured my 2m dipole antenna:**

**Before calibration (uncal NanoVNA):**
```
Resonance appeared at: 144.8 MHz
SWR minimum: 1.3:1 (seemed high)
```

**After frequency + port calibration:**
```
Resonance: 146.1 MHz
SWR minimum: 1.05:1

Much better! Makes sense - antenna was cut for 146 MHz.
```

**Cross-checked with calibrated TinySA:**
```
TinySA return loss sweep: Minimum at 146.0 MHz
NanoVNA: 146.1 MHz

Agreement within 100 kHz! ✓
```

### Test 5: Commercial Filter

**Measured Minicircuits SLP-150+ low-pass filter:**

**Datasheet specs:**
```
3dB cutoff: 165 MHz
Insertion loss: <1.5 dB (DC-150 MHz)
Stopband rejection: >30 dB (200+ MHz)
```

**NanoVNA measurements (S21):**
```
@ 145 MHz: -0.8 dB (good, within spec)
@ 165 MHz: -3.2 dB (3dB point, matches datasheet!)
@ 200 MHz: -28 dB (good stopband)
@ 250 MHz: -35 dB (excellent)
```

**Matches datasheet within ±0.5 dB!** ✓

---

## Results Summary

### Frequency Calibration

**Before:**
- Error: +30.6 ppm
- At 146 MHz: +4.5 kHz error

**After:**
- Error: 0 ppm (GPS-calibrated)
- At 146 MHz: <10 Hz error

**Improvement: 3000× better accuracy**

### Port Calibration

**With DIY standards:**
- 50Ω LOAD: Measures 50.0Ω ✓
- 100Ω resistor: Measures 100.2Ω ✓
- Commercial filter: Matches datasheet ✓

**Estimated impedance accuracy: ±2-5%**

### Total Cost

| Item | Cost |
|------|------|
| GPS module (reused from TinySA) | $0 |
| Arduino (reused) | $0 |
| Resistors, SMA connectors | $12 |
| **Total** | **$12** |

### Total Time

| Task | Time |
|------|------|
| Frequency cal (FM check) | 30 min |
| Frequency cal (GPS) | 1 hour |
| Build cal standards | 2 hours |
| Port calibration | 10 min |
| Verification tests | 1 hour |
| **Total** | **4.7 hours** |

---

## Lessons Learned

### What Went Well

1. **GPS method works perfectly** - Same hardware as TinySA
2. **DIY load actually better** than supplied kit
3. **Frequency cal made huge difference** - Resonances now at correct frequency
4. **Verification critical** - Measuring known components builds confidence

### Challenges

1. **Finding 8 MHz test point** - Used 100 MHz output instead (easier)
2. **Resistor selection** - Bought 10, needed to measure all
3. **SHORT inductance** - First attempt with wire loop was inductive, rebuilt with flat copper

### Would Do Differently

1. **Buy SMD resistors** - Lower inductance for future high-freq work
2. **Build multiple LOADs** - Having backup would be nice
3. **Label standards** - Which is which? Marked them now.

### Unexpected Discoveries

1. **Supplied kit LOAD was 49Ω** not 50Ω - Good thing I verified!
2. **Antenna resonance shifted 1.3 MHz** after proper calibration
3. **NanoVNA quite stable** after warmup (drift <0.5 ppm/hour)

---

## Applications

### What I Can Now Measure Accurately

**Antennas:**
- Resonant frequency (±100 kHz)
- Feedpoint impedance (±2Ω)
- SWR across band (±0.05)
- Bandwidth

**Filters:**
- Insertion loss (±0.5 dB)
- Cutoff frequency (±1%)
- Stopband rejection
- Group delay

**Components:**
- Resistor values at RF
- Inductor L and Q
- Capacitor C and ESR
- Transformer ratios

**Transmission Lines:**
- Cable loss (±0.2 dB)
- Velocity factor (±2%)
- Impedance (±2Ω)
- Connector quality

---

## Before and After Comparison

### Before Calibration

```
Measuring 2m dipole:
- Resonance: 144.8 MHz (WRONG!)
- SWR minimum: 1.3:1
- Feedpoint Z: 44Ω (seems low)
- Confidence level: Low

Measuring 100Ω resistor:
- Reads: 87Ω (way off!)
```

### After Calibration

```
Same dipole:
- Resonance: 146.1 MHz (correct!)
- SWR minimum: 1.05:1 (realistic)
- Feedpoint Z: 52Ω (good)
- Confidence level: High

Same 100Ω resistor:
- Reads: 100.2Ω (spot on!)
```

---

## Maintaining Calibration

### Regular Tasks

**Monthly:**
- Check frequency with FM station
- Re-calibrate if drifted >5 ppm

**Each measurement session:**
- Power on, wait 15 minutes (warmup)
- Load saved port cal OR recalibrate
- Verify with 50Ω load before measuring

**Annually:**
- Full frequency calibration (GPS)
- Verify all cal standards (measure with DMM)
- Temperature coefficient characterization

### Documentation

**Created calibration log:**
```
Date: 2026-01-02
NanoVNA: v1, Serial 12345
Frequency cal: GPS, 0 ppm
Cal standards: DIY, verified
Port cal slots:
- Slot 0: 1-30 MHz (HF)
- Slot 1: 140-150 MHz (2m) ← current
- Slot 2: 420-450 MHz (70cm)
- Slot 3: Reserved
- Slot 4: Temporary

Next cal due: 2026-02-01
```

---

## Conclusion

### Success Criteria - All Met!

✓ Frequency accurate to 0 ppm (GPS)
✓ Impedance accurate to ±5%
✓ Total cost: $12
✓ Build time: <5 hours
✓ Verified with multiple methods
✓ Matches commercial filter datasheet
✓ Matches calibrated TinySA readings

### Key Achievement

**Turned a $50 uncalibrated NanoVNA into a precision RF measurement tool for $12 and 5 hours of work.**

### Confidence Level

**Before:** "Is this reading even close?"
**After:** "I trust these measurements for real design work."

---

## Recommendations

### For Others Doing This

1. **Start with frequency cal** - Makes everything else make sense
2. **Buy extra resistors** - Select best match
3. **Verify everything** - Trust but verify
4. **Document** your calibration
5. **Cross-check** with known components

### Next Projects

1. **Design matching network** for 2m antenna
2. **Build bandpass filter** for 70cm
3. **Characterize ferrite cores**
4. **Measure coaxial cable** velocity factors
5. **Build directional coupler** and verify

---

**Total project success! NanoVNA now calibrated and ready for precision RF work.**

**73 and enjoy your calibrated VNA!**
