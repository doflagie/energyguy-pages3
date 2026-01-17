# NanoVNA Frequency Calibration

## Overview

The NanoVNA's frequency accuracy depends on its internal crystal oscillator. Like the TinySA, this crystal needs calibration for accurate measurements.

**Good news:** The same methods used for TinySA calibration work for NanoVNA!

### Quick Reference

**For detailed calibration methods, see:**
- **[gps_calibration.md](gps_calibration.md)** - GPS 1PPS method (0.01 ppm)
- **[radio_standard_calibration.md](radio_standard_calibration.md)** - WWV/CHU (0.1-1 ppm)
- **[fm_broadcast_calibration.md](fm_broadcast_calibration.md)** - FM stations (1-10 ppm)

This document covers NanoVNA-specific aspects and quick calibration procedures.

---

## NanoVNA Frequency Architecture

### Crystal Oscillator

**NanoVNA v1 (Original):**
- Crystal: 8 MHz
- PLL multiplication: up to 900 MHz
- Typical uncalibrated error: ±20-50 ppm

**NanoVNA-H/H4:**
- Crystal: 8 MHz or TCXO
- Better stability: ±5-20 ppm uncalibrated
- Higher frequency range (up to 4.4 GHz)

### Why Calibration Matters

**Example at 146 MHz (2m ham band):**
```
Uncalibrated (+30 ppm):
Display shows: 146.000 MHz
Actually measuring: 146.004 MHz
Error: 4.4 kHz

After GPS calibration (+0.01 ppm):
Display shows: 146.000 MHz
Actually measuring: 146.0000015 MHz
Error: 1.5 Hz
```

**Impact on measurements:**
- Antenna appears resonant at wrong frequency
- Filter peaks shifted
- Crystal filter tuning incorrect
- SAW filter characterization wrong

---

## Quick Calibration Methods

### Method 1: FM Broadcast (Easiest, 15 minutes)

**Equipment:**
- NanoVNA
- Known FM station frequency

**Procedure:**

1. **Find strong local FM station**
   - Example: 100.1 MHz
   - Verify online: [radio-locator.com](http://radio-locator.com)

2. **Configure NanoVNA:**
   - Start: 100.0 MHz
   - Stop: 100.2 MHz
   - Format: LOGMAG
   - Connect short wire antenna to CH0

3. **Find carrier peak:**
   - Look for strong spike
   - Place marker on peak
   - Read frequency

4. **Calculate error:**
   ```
   Known frequency: 100.100 MHz = 100,100,000 Hz
   NanoVNA reads: 100,100,520 Hz
   Error: +520 Hz
   PPM: 520 / 100,100,000 × 10^6 = +5.19 ppm
   ```

5. **Apply correction in NanoVNA:**
   - Press **CONFIG**
   - Select **EXPERT CONFIG**
   - Find **TCXO** or **XTAL**
   - Current: 8000000 Hz
   - Calculate new value:
     ```
     New TCXO = 8000000 × (1 + ppm/10^6)
     New TCXO = 8000000 × (1 + 5.19/10^6)
     New TCXO = 8000000 × 1.00000519
     New TCXO = 8000041.52 Hz
     Round to: 8000042 Hz
     ```
   - Enter: **8000042**
   - Press **SAVE**

6. **Verify:**
   - Re-measure FM station
   - Should now read 100.100 MHz exactly

**Accuracy:** ±1-5 ppm (good enough for most uses)

---

### Method 2: GPS 1PPS (Best, 2-4 hours first time)

**Equipment:**
- GPS module (NEO-6M/7M/8M) - $10-20
- Arduino Nano - $10
- Breadboard and wires - $5

**Procedure:**

**If you already built GPS counter for TinySA:**
- Same hardware works!
- Just connect to NanoVNA instead

**New build:**
- Follow complete instructions in [gps_calibration.md](gps_calibration.md)
- Adapt for NanoVNA's 8 MHz crystal

**Key difference:**
- TinySA: measure 30 MHz
- NanoVNA: measure 8 MHz

**Modified frequency counter:**
```cpp
// Same Arduino code, but:
double expected = 8000000.0;  // 8 MHz for NanoVNA

// Then in output:
double error_ppm = (freq - expected) / expected * 1e6;
```

**Accessing NanoVNA's 8 MHz:**

**Option A: Test point (requires opening case)**
- Open NanoVNA case (4 screws)
- Locate 8 MHz crystal
- Find test point or solder to crystal pin
- Connect to Arduino D5 via 0.1µF cap

**Option B: Use NanoVNA's output**
- Set NanoVNA to CW mode at 8 MHz
- Connect CH0 to Arduino
- Not ideal (goes through synthesizer)

**Option C: Indirect measurement**
- Set NanoVNA to 100 MHz output
- Measure with GPS counter
- Calculate back to 8 MHz error

**Recommended:** Option C (easiest, no disassembly)

**Example:**
```
Set NanoVNA to CW mode, 100.000 MHz output
Connect CH0 to GPS frequency counter
Counter reads: 100,000,520 Hz
Error: +520 Hz at 100 MHz
PPM: 520 / 100,000,000 × 10^6 = +5.20 ppm

This same error applies to 8 MHz crystal:
8 MHz error = 8,000,000 × 5.20/10^6 = 41.6 Hz
Actual 8 MHz = 8,000,041.6 Hz

Enter in TCXO setting: 8000042 Hz
```

**Accuracy:** ±0.01 ppm

---

### Method 3: WWV/CHU Time Standard (Good, 30-60 min)

**Equipment:**
- Shortwave receiver or RTL-SDR
- NanoVNA
- Computer

**Best frequency:** CHU 7.850 MHz or WWV 5 MHz

**Procedure:**

1. **Receive CHU 7.850 MHz** with SDR

2. **Set NanoVNA to measure CHU:**
   - Start: 7.800 MHz
   - Stop: 7.900 MHz
   - Connect antenna to CH0
   - Find CHU carrier

3. **Place marker on carrier**

4. **Read frequency:**
   ```
   Known: 7,850,000 Hz
   NanoVNA: 7,850,123 Hz
   Error: +123 Hz
   PPM: 123 / 7,850,000 × 10^6 = +15.67 ppm
   ```

5. **Calculate new TCXO value:**
   ```
   New = 8,000,000 × (1 + 15.67/10^6)
   New = 8,000,125 Hz
   ```

6. **Apply in CONFIG → EXPERT CONFIG → TCXO**

**Accuracy:** ±0.1-1 ppm (depends on propagation)

---

## NanoVNA-Specific Considerations

### Firmware Differences

**Different firmwares have different calibration methods:**

**edy555 original firmware:**
- CONFIG → EXPERT CONFIG → TCXO
- Enter frequency in Hz
- Example: 8000042 for +5.25 ppm

**NanoVNA-App / DiSlord firmware:**
- CONFIG → CALIBRATION → PPM CORRECTION
- Enter PPM directly
- Example: -5.25 for +5.25 ppm error (note sign)

**Check your firmware:**
- Press **CONFIG**, look at menu options
- Different menus = different firmware
- Consult firmware documentation

### Temperature Effects

**NanoVNA gets warm during use:**
- First 10-15 minutes: frequency drifts
- After warmup: stabilizes
- **Always calibrate after warmup!**

**Procedure:**
1. Turn on NanoVNA
2. Run a sweep (any frequency)
3. Wait 15 minutes
4. Now calibrate frequency
5. Now calibrate ports (SOL/SOLT)

### Verification

**Quick check:**

1. **Set NanoVNA to receive FM station**
   - Known frequency: 100.1 MHz
   - Should show carrier at exactly 100.100 MHz

2. **Measure crystal filter**
   - Known center frequency: 10.695 MHz (IF filter)
   - Should measure at 10.695 MHz ± 0.001 MHz

3. **Cross-check with TinySA**
   - If you have calibrated TinySA
   - Both should agree on frequency

---

## Calibration Storage

### Saving Calibration

**Most firmwares:**
- Frequency correction saved in flash
- Survives power cycle
- Persists through firmware update (usually)

**To verify calibration was saved:**
1. Note TCXO value after calibration
2. Power cycle NanoVNA
3. Check CONFIG → EXPERT CONFIG → TCXO
4. Should show same value

**If calibration doesn't save:**
- Firmware bug (update firmware)
- Flash memory issue
- Battery backup needed (some models)

### Multiple NanoVNAs

**If you have multiple NanoVNAs:**
- Each needs individual calibration
- Crystal tolerance varies ±50 ppm unit-to-unit
- Label each unit with its calibration date
- Keep log of each unit's PPM error

---

## Advanced: Measuring 8 MHz Directly

### Building 8 MHz Tap

**If you want best accuracy:**

1. **Open NanoVNA case** (warranty void!)

2. **Locate 8 MHz crystal** (usually near center)

3. **Find crystal pin or test point**
   - May be labeled "XTAL", "8M", or "REF"
   - Or solder thin wire to crystal package

4. **Add buffer (optional but recommended):**
   ```
   8MHz signal → 74HC04 inverter → Output to counter
                 (prevents loading crystal)
   ```

5. **Connect to GPS frequency counter**

6. **Measure directly:**
   ```
   Expected: 8,000,000 Hz
   Measured: 8,000,123 Hz
   Error: +123 Hz
   PPM: 123 / 8,000,000 × 10^6 = +15.375 ppm

   New TCXO: 8,000,123 Hz (enter directly)
   ```

**Advantage:** Most direct, most accurate
**Disadvantage:** Requires disassembly, potential for damage

---

## Temperature Compensation

### Characterizing Temperature Coefficient

**Same procedure as TinySA:**

1. **Cool NanoVNA** (refrigerator, 30 min, to ~5°C)
2. **Measure frequency error** with GPS or FM
3. **Warm to room temp** (natural, 45 min, to ~22°C)
4. **Measure at intervals**
5. **Heat gently** (hair dryer, to ~40°C)
6. **Plot error vs. temperature**

**Example data:**

| Temp (°C) | Error (ppm) | TCXO Hz |
|-----------|-------------|---------|
| 5 | +3.2 | 8000026 |
| 15 | +4.8 | 8000038 |
| 25 | +5.1 | 8000041 |
| 35 | +5.9 | 8000047 |
| 45 | +7.2 | 8000058 |

**Temperature coefficient:**
```
TC = Δppm / ΔT = (7.2 - 3.2) / (45 - 5) = +0.1 ppm/°C
```

**Compensation:**
- Calibrate at typical operating temperature (after warmup)
- Re-calibrate if ambient changes significantly
- Some firmwares support temperature compensation (needs sensor)

---

## Troubleshooting

### Frequency Won't Save

**Problem:** Enter new TCXO, but reverts after power cycle

**Solutions:**
1. Check firmware version (old versions had bugs)
2. Update to latest firmware
3. Some models need explicit SAVE command
4. Battery backup missing (some variants)

### Calibration Makes It Worse

**Problem:** After calibration, frequencies more wrong

**Cause:** Sign error

**Fix:**
- If entered +5 ppm, try -5 ppm
- Different firmwares use different sign conventions
- Check if you need to invert sign

### Frequency Drifts

**Problem:** Calibration good initially, then drifts

**Causes:**
1. Temperature change (wait for warmup)
2. Aging crystal (re-calibrate monthly)
3. Poor quality crystal (upgrade unit)

**Solution:**
- Always warm up before measurements
- Recalibrate periodically
- Monitor temperature

### Can't Access Crystal

**Problem:** No test point, can't open case

**Solution:**
- Use indirect method (measure NanoVNA's output at known frequency)
- Set to CW mode, pick a frequency
- Measure with calibrated receiver or TinySA
- Calculate error

---

## Real-World Example

### Calibrating NanoVNA v1 with FM Broadcast

**Equipment:**
- NanoVNA v1
- 6-inch wire (antenna)

**Station:**
- WBUR 90.9 MHz (Boston NPR, GPS-locked)

**Procedure:**

**Step 1: Measure FM station (before calibration)**
```
NanoVNA settings:
- Start: 90.8 MHz
- Stop: 91.0 MHz
- Format: LOGMAG

Marker on peak: 90.902.3 MHz
Known frequency: 90.900.0 MHz
Error: +2.3 kHz = +2300 Hz
PPM: 2300 / 90,900,000 × 10^6 = +25.30 ppm
```

**Step 2: Calculate new TCXO**
```
Current TCXO: 8,000,000 Hz (default)
Error: +25.30 ppm

New TCXO = 8,000,000 × (1 + 25.30/10^6)
         = 8,000,000 × 1.00002530
         = 8,000,202.4 Hz
Round to: 8,000,202 Hz
```

**Step 3: Apply**
```
CONFIG → EXPERT CONFIG → TCXO
Enter: 8000202
SAVE
```

**Step 4: Verify**
```
Re-measure 90.9 MHz station:
Marker: 90.900.1 MHz
Error: +100 Hz = +1.1 ppm

Much better! (25 ppm → 1 ppm)

For even better accuracy:
Iterate: New TCXO = 8,000,202 × (1 + 1.1/10^6) = 8,000,211 Hz

After second iteration:
Marker: 90.900.0 MHz
Error: 0 Hz! Perfect.
```

**Time:** 15 minutes
**Result:** 25 ppm → 0 ppm error

---

## Summary

### Frequency Calibration Achieved

✓ GPS method: ±0.01 ppm (best)
✓ WWV/CHU: ±0.1-1 ppm (good)
✓ FM broadcast: ±1-5 ppm (quick and easy)

### Key Points

1. **Same methods as TinySA** - GPS, WWV, FM all work
2. **Measure 8 MHz** directly or indirectly
3. **Calculate PPM error** from any known frequency
4. **Enter new TCXO value** in CONFIG
5. **Verify** with multiple sources
6. **Recalibrate** after temperature changes or annually

### Next Step

**Now that frequency is calibrated, proceed to port calibration:**
→ [nanovna_cal_standards.md](nanovna_cal_standards.md)

---

**Frequency calibration complete! Your NanoVNA now knows what frequency it's really measuring.**
