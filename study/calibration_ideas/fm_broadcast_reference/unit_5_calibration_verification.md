# Unit 5 — Applying Correction, Verifying, and Logging
**[TM-CAL-004 — Open Handout](../../Calibration_Ideas/fm_broadcast_reference/tm.html)**
**ELOs:** 1.7, 1.8, 1.9, 1.10, 1.11  
**Estimated time:** 45 minutes (includes hands-on lab)  
**TM sections to read first:** Chapter 5 (full), Chapter 6, Chapter 7

---

## Step 1: Read the TM

Open TM-CAL-004. Read:
- Chapter 5: Calibration Procedure (all steps and the Caution)
- Chapter 6: Tuning and Adjustment
- Chapter 7: Verification (all steps)

Then come back here.

---

## What This Unit Covers

The previous units gave you the knowledge. This unit is the procedure. You will:
1. Execute the full calibration
2. Apply the correction to the TinySA
3. Verify the calibration against 3 stations
4. Record the results in a calibration log

**You need your TinySA, antenna, and the 3 station assignments you looked up in Unit 2.**

---

## Before You Start: The Multipath Caution

TM-CAL-004 §5-1 contains an important caution that applies before you take a single measurement:

> Multipath reflections can shift the apparent carrier frequency by up to several hundred Hz in urban environments. Measure from a location with clear line-of-sight to the transmitter if possible.

Multipath means your antenna is receiving the direct signal plus reflections off buildings or terrain. The reflected signals arrive at slightly different times and can shift the apparent carrier frequency at your location. In open terrain this is negligible; in a downtown building it can be hundreds of Hz.

**Practical action:** If your three measurements give wildly inconsistent ppm errors (e.g., +5, +22, −3 ppm), suspect multipath before you suspect a bad calibration. Move to a window with line-of-sight toward the transmitter, or step outside.

---

## Part A: Execute the Calibration

### Collect Your Measurements

With TinySA connected to antenna, work through each of your 3 stations using the procedure from Chapter 4 (your Unit 3 skills):

For each station:
1. Find carrier (wide sweep: 10 MHz span, RBW ≤3 kHz)
2. Zoom in (500 kHz span, RBW 1 kHz)
3. MARKER PEAK → record measured frequency to nearest 100 Hz
4. Record alongside the FCC-assigned frequency

### Calculate the Corrections

For each station, compute (Unit 4 formulas):
- error_Hz = f_measured − f_assigned
- error_ppm = error_Hz / f_assigned × 1,000,000

Average the three ppm values. This is your **correction value**.

---

## Part B: Apply the Correction (Chapter 6)

The TinySA has a reference oscillator calibration function in the system menu. The exact path varies by firmware version — consult your TinySA documentation. Look for: **System → Reference Calibration** or similar.

Enter your correction value in ppm or Hz as required by the menu. The sign convention matters:
- If your oscillator was running **high** (+ppm), you enter a **negative** correction to bring it down.
- If it was running **low** (−ppm), you enter a **positive** correction to bring it up.

Some TinySA firmware expresses this as a direct offset (enter the measured error and the firmware applies the opposite correction automatically). Read your instrument manual to confirm the sign convention before entering a value.

---

## Part C: Verify (Chapter 7)

After applying the correction, re-measure all three stations. The acceptance criterion is:

> **Residual error < 2 kHz on every station.**

2 kHz = 2,000 Hz. If your residual is 1,800 Hz: pass. If it is 2,100 Hz: fail — re-examine your correction calculation and re-apply.

### If One Station Shows > 3 kHz After Correction

TM-CAL-004 Chapter 7, step 4 is specific:

> Verify that station assignment in FCC database is current (stations occasionally shift channels).

Do not assume your calibration failed. FM stations do occasionally change assigned frequencies (channel moves, power changes). If one station is an outlier, look it up in the FCC database again. If the database confirms the assigned frequency you used, then the station may simply be poorly maintained — discard it and use a different reference station.

---

## Part D: The Calibration Log

Every calibration requires a log entry. TM-CAL-004 Chapter 7, step 5 specifies exactly what to record:

1. **Date**
2. **Station call signs**
3. **Assigned frequencies** (from FCC database)
4. **Measured frequencies** (from TinySA after correction)
5. **Errors** (in Hz and ppm)

Use this format (fill in your own values):

```
CALIBRATION LOG — TinySA Ultra Reference Oscillator
Procedure: TM-CAL-004
Date: ____________
Instrument S/N or ID: ____________

Pre-correction measurements:
  Station    Assigned (MHz)   Measured (MHz)   Error (Hz)   Error (ppm)
  ________   ______________   ______________   __________   __________
  ________   ______________   ______________   __________   __________
  ________   ______________   ______________   __________   __________
  Average error: _______ ppm   Correction applied: _______ ppm

Post-correction (verification):
  Station    Assigned (MHz)   Measured (MHz)   Residual (Hz)   Pass/Fail
  ________   ______________   ______________   _____________   ________
  ________   ______________   ______________   _____________   ________
  ________   ______________   ______________   _____________   ________

Result: PASS / FAIL
Operator: ____________
```

Complete this log entry before you consider the calibration finished.

---

## Self-Check Questions

**SC5-1.** What is the acceptance criterion for residual error after calibration?

**SC5-2.** How many stations must pass verification?

**SC5-3.** After calibration, station KABC shows a residual error of 3.8 kHz. What is the correct first action per TM-CAL-004?

**SC5-4.** What environmental condition can shift measured carrier frequency by several hundred Hz, and how do you mitigate it?

**SC5-5.** List the five required entries in a calibration log.

---

## Answer Key

**SC5-1.** Residual error < 2 kHz (2,000 Hz). *(TM Ch. 7, steps 2–3)*

**SC5-2.** All three stations must pass. *(TM Ch. 7, step 2)*

**SC5-3.** Verify the station's channel assignment in the FCC database is current — stations occasionally shift channels. *(TM Ch. 7, step 4)*

**SC5-4.** Multipath reflections (urban environments). Mitigation: measure from a location with clear line-of-sight to the transmitter. *(TM §5-1, Caution)*

**SC5-5.** Date; station call signs; assigned frequencies; measured frequencies; errors. *(TM Ch. 7, step 5)*

---

## Checkpoint

You have completed the hands-on procedure if:
- [ ] Three stations measured pre-correction
- [ ] Correction calculated and applied to TinySA
- [ ] Three stations re-measured post-correction
- [ ] All residuals < 2,000 Hz
- [ ] Calibration log entry completed with all 5 required fields

If all boxes are checked, you have achieved **TLO-1**.

**→ Proceed to [Final Assessment](final_assessment.md)**
