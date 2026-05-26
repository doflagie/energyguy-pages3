# Unit 5 — Calibration and Verification
**[TM-GEAR-012 — Open Handout](../../antenna_gear/motorized_tuner/tm.html)**
**TM Chapters:** Chapter 6, Chapter 7
**ELOs:** Calibrate the MOTORIZED ANTENNA TUNER — HF AND VHF/UHF; verify performance against acceptance criteria; complete calibration log
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-GEAR-012. Read **Chapter 6 — Calibration** and **Chapter 7 — Verification and Acceptance** completely.

Then come back here.

---

## Chapter 6 — Calibration Content

1. SWR bridge calibration: connect a calibrated 50Ω load. Adjust
 the SWR bridge null trimmer until reflected reading = 0. Adjust forward
 calibration trimmer until forward reading = applied power.
2. Position sensor calibration: with each component at minimum position,
 record the ADC reading. With each at maximum, record the ADC reading.
 Enter these bounds in firmware for linear interpolation to real units
 (pF or &micro;H).

---

## Chapter 7 — Verification Content

1. Tune into a 50Ω reference load: SWR must read <1.05:1 (verifies
 SWR bridge calibration).
2. Tune into a 200Ω non-reactive load: auto-tune must achieve
 SWR <1.5:1 within 60 seconds on any HF band.
3. Band memory: select 40M, tune to best SWR, change to 20M and tune, then
 return to 40M — the tuner must return to the 40M setting within 3 turns.
4. Motor limit switch test: command full travel in each direction. All three
 motors must stop at limit switches without stalling.
5. Log: date, SWR bridge calibration check, position sensor ranges, auto-tune
 test result (band, load, SWR achieved, time), operator.

---

## Calibration vs. Verification — Distinction

**Calibration** (Chapter 6): active adjustments to bring the component within specification.
You are permitted to adjust, tweak, and repeat until the criterion is met.

**Verification** (Chapter 7): independent confirmation that the calibration succeeded.
No adjustments are permitted during verification. If a verification check fails, you
return to Chapter 6, not to Chapter 7.

---

## Acceptance Criterion

A criterion found in Chapter 6: **50Ω**. Confirm the exact criterion in the TM.

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
