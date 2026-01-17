# NanoVNA Port Calibration Procedures

## Overview

Port calibration corrects for NanoVNA's internal imperfections and allows accurate impedance measurements. This must be done **before each measurement session**.

---

## Quick Start: SOL Calibration (1-Port)

**Use for:** Antenna tuning, impedance measurements, SWR

**Time:** 2 minutes

### Step-by-Step Procedure

1. **Set frequency range:**
   - Press **STIMULUS**
   - Set START: (your min frequency, e.g., 1 MHz)
   - Set STOP: (your max frequency, e.g., 300 MHz)
   - **Important:** Only calibrate the range you'll measure!

2. **Start calibration:**
   - Press **CAL**
   - Select **CALIBRATE**
   - Choose **S11** (1-port)

3. **Connect SHORT:**
   - Attach SHORT standard to CH0
   - Press **SHORT** on screen
   - Wait for sweep to complete

4. **Connect OPEN:**
   - Remove SHORT
   - Attach OPEN standard to CH0
   - Press **OPEN** on screen
   - Wait for sweep

5. **Connect LOAD:**
   - Remove OPEN
   - Attach 50Ω LOAD to CH0
   - Press **LOAD** on screen
   - Wait for sweep

6. **Done:**
   - Press **DONE** or **APPLY**
   - Cal is now active
   - Save to slot: **CAL → SAVE → Select slot 0-4**

### Verification

**Connect 50Ω LOAD:**
- Should show SWR 1.00:1
- Impedance should show 50Ω +j0

**If not perfect:**
- Re-calibrate
- Check standards quality
- Ensure tight connections

---

## SOLT Calibration (2-Port)

**Use for:** Filters, amplifiers, attenuators, cables

**Time:** 3 minutes

### Procedure

1. **Set frequency range** (both ports)

2. **Start 2-port cal:**
   - **CAL → CALIBRATE → S11+S21+S22+S12** (full 2-port)

3. **CH0 (PORT1) Standards:**
   - Connect SHORT to CH0 → Press **SHORT**
   - Connect OPEN to CH0 → Press **OPEN**
   - Connect LOAD to CH0 → Press **LOAD**

4. **Connect THRU:**
   - Connect CH0 to CH1 with THRU adapter
   - Press **THRU**

5. **CH1 (PORT2) Standards:**
   - Disconnect THRU
   - Connect SHORT to CH1 → Press **SHORT**(continue)
   - Connect OPEN to CH1 → Press **OPEN**
   - Connect LOAD to CH1 → Press **LOAD**

6. **Done:**
   - Press **APPLY**
   - Save calibration

### What You Can Measure

- **S11:** Input return loss / impedance
- **S21:** Forward transmission / gain / insertion loss
- **S12:** Reverse transmission (isolation)
- **S22:** Output return loss

---

## Calibration Tips

### Frequency Range

**Critical rule:** Calibrate EXACTLY the range you'll measure

**Wrong:**
- Calibrate 1-900 MHz
- Measure at 1.5 GHz ✗ (cal invalid!)

**Right:**
- Will measure 144-148 MHz (2m ham band)
- Calibrate 140-150 MHz ✓

### Temperature

**Let NanoVNA warm up:**
1. Turn on
2. Run any sweep
3. Wait 10-15 minutes
4. NOW calibrate

**Why:** Crystal frequency drifts while warming up

### Connector Care

**Before calibrating:**
- Clean all connectors (IPA alcohol)
- Tighten finger-tight (don't overtighten!)
- No cross-threading
- Inspect for damage

### Saving Calibrations

**NanoVNA has multiple cal slots:**
- Slot 0: 1-30 MHz (HF)
- Slot 1: 140-150 MHz (2m)
- Slot 2: 420-450 MHz (70cm)
- Slot 3: 50-900 MHz (wideband)
- Slot 4: Temporary/test

**Load saved cal:**
- **CAL → RECALL → Select slot**

---

## Troubleshooting

### Calibration Doesn't Save

**Fix:**
- After **DONE**, must press **CAL → SAVE → slot**
- Some firmwares auto-save
- Check firmware documentation

### SWR Still Wrong After Cal

**Possible causes:**
1. LOAD not 50Ω → Measure with DMM
2. Forgot to press DONE → Redo calibration
3. Wrong frequency range → Recal correct range
4. Cable moved → Recalibrate

### Smith Chart Doesn't Center

**On 50Ω LOAD:**
- Should be dead center
- If offset → LOAD is not 50Ω
- If rotating → frequency drift

---

## Advanced: Enhanced Response Calibration

**For filter measurements requiring high dynamic range:**

**ERR (Enhanced Response) Cal:**
- Measures NanoVNA's directivity
- Improves return loss measurements
- Gives better accuracy for high VSWR

**Procedure:**
- Similar to SOL
- Additional REFLECT standard (usually same as SHORT)

**When to use:**
- Measuring very high or very low impedances
- Need >40 dB dynamic range
- Critical filter work

---

## Summary

✓ **SOL** for 1-port (antennas, impedances)
✓ **SOLT** for 2-port (filters, amplifiers)
✓ Calibrate before each session
✓ Calibrate exact frequency range
✓ Warm up first
✓ Save to slots
✓ Verify with known good component

---

**Next:** [nanovna_verification.md](nanovna_verification.md) - Testing calibration quality
