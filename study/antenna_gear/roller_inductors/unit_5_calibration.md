# Unit 5 — Calibration and Verification
**[TM-GEAR-017 — Open Handout](../../antenna_gear/roller_inductors/tm.html)**
**TM Chapters:** Chapter 6, Chapter 7
**ELOs:** Calibrate the ROLLER INDUCTORS — MOTORIZED VARIABLE INDUCTANCE; verify performance against acceptance criteria; complete calibration log
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-GEAR-017. Read **Chapter 6 — Calibration** and **Chapter 7 — Verification and Acceptance** completely.

Then come back here.

---

## Chapter 6 — Calibration Content

1. Set the roller to each of 10 equally-spaced positions (0, 10, 20, ..., 100%
 of full travel). Measure inductance at each position with an LCR meter
 at 100 kHz. Record the (step_count, L_uH) pairs as the calibration table.
2. Store the calibration table in ESP32 NVS. The firmware uses linear
 interpolation between table points for positions between calibration points.
3. Calibration must be repeated if the motor coupler or limit switch position
 is changed.

---

## Chapter 7 — Verification Content

1. Position repeatability: command 10 &micro;H 10 times from random positions.
 Measured inductance must be within ±0.5 &micro;H of target each time.
2. Limit switch test: command travel beyond the end-of-travel limit. Motor
 must stop at the limit switch; verify no lost steps after the stop.
3. Home repeatability: home 5 times. After each home, command 10 &micro;H;
 measure inductance. Must be within ±0.2 &micro;H each run.
4. Log: date, inductor model, calibration table (10-point), home repeatability,
 position repeatability at 10 &micro;H and 20 &micro;H, operator.

---

## Calibration vs. Verification — Distinction

**Calibration** (Chapter 6): active adjustments to bring the component within specification.
You are permitted to adjust, tweak, and repeat until the criterion is met.

**Verification** (Chapter 7): independent confirmation that the calibration succeeded.
No adjustments are permitted during verification. If a verification check fails, you
return to Chapter 6, not to Chapter 7.

---

## Acceptance Criterion

A criterion found in Chapter 6: **100%**. Confirm the exact criterion in the TM.

Write the exact criterion from the TM here before you start the lab:
_________________________________________________

---

## Lab Checklist

Work through each item in sequence. Do not skip steps.

- [ ] All required equipment gathered and ready
- [ ] Equipment warmed up per TM requirements
- [ ] All Chapter 4 setup steps verified complete
- [ ] Chapter 6 calibration performed (all steps)
- [ ] Calibration criterion met (recorded below)
- [ ] Chapter 7 verification performed (all steps, no adjustments)
- [ ] All verification criteria passed
- [ ] Calibration log entry completed

---

## Calibration Log Entry

- [ ] Date
- [ ] Equipment used
- [ ] Measurements taken
- [ ] Results vs. criteria
- [ ] Operator name

Record your results:

| Item | Measured Value | Acceptance Criterion | Pass/Fail |
|------|---------------|---------------------|-----------|
| (from Ch 7) | | | |
| (from Ch 7) | | | |
| (from Ch 7) | | | |

---

## Self-Check Questions

**SC5-1.** What is the acceptance criterion for this calibration (from Chapter 7)?

**SC5-2.** During verification, one reading fails. What is the correct action per the TM?

**SC5-3.** List the items required in a calibration log entry per Chapter 7.

**SC5-4.** What is the difference between calibration and verification in this context?

**SC5-5.** If the component passes all Chapter 7 checks, what does that tell you about its current state?

---

## Answer Key

**SC5-1.** See Chapter 7. The criterion is stated as a specific value with units.

**SC5-2.** Stop verification. Return to Chapter 6 and re-calibrate. Diagnose the cause before re-adjusting.

**SC5-3.** See Chapter 7, last numbered step. The log items are listed there.

**SC5-4.** Calibration allows adjustments; verification does not. Verification is independent confirmation.

**SC5-5.** It tells you the component is performing within its specified parameters at this time. It does not tell you how long it will remain in calibration — periodic re-verification is required.

---

## Checkpoint

You are ready for the final assessment when you can:
- [ ] State the acceptance criterion from memory
- [ ] State the difference between calibration and verification
- [ ] State the required log items from memory

**→ Proceed to [Final Assessment](final_assessment.md)**
