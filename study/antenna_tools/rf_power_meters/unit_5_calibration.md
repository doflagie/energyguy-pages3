# Unit 5 — Calibration and Verification
**[TM-TOOL-008 — Open Handout](../../antenna_tools/rf_power_meters/tm.html)**
**TM Chapters:** Chapter 6, Chapter 7
**ELOs:** Calibrate the RF POWER METERS — TYPES AND OPERATION; verify performance against acceptance criteria; complete calibration log
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-TOOL-008. Read **Chapter 6 — Calibration** and **Chapter 7 — Verification and Acceptance** completely.

Then come back here.

---

## Chapter 6 — Calibration Content

1. Connect calibrated power source to IN; calibrated 50Ω load to OUT.
2. Apply 10W at 14.175 MHz. Panel meter (Bird 43) or CYD should read
 10 ± 1W. Adjust calibration pot R_cal (Bird 43 body) if needed.
 For digital inline meter: update calibration coefficient in NVS.
3. Verify at 50W and 100W. Response must be linear within ±5%.
4. Repeat for each slug / frequency range in use.

---

## Chapter 7 — Verification Content

1. Power measurement within ±5% of reference (calibrated signal source)
 at all test frequencies.
2. SWR reading within ±0.1 of calculated SWR for a known mismatch
 (e.g., 100Ω load gives SWR 2.0:1 into 50Ω system).
3. Insertion loss with slug installed: <0.1 dB measured by comparing
 received signal with and without meter inline.
4. Log: date, slug serial number (or homebrew tag), frequency, measured
 power vs. reference, SWR accuracy, operator.

---

## Calibration vs. Verification — Distinction

**Calibration** (Chapter 6): active adjustments to bring the tool within specification.
You are permitted to adjust, tweak, and repeat until the criterion is met.

**Verification** (Chapter 7): independent confirmation that the calibration succeeded.
No adjustments are permitted during verification. If a verification check fails, you
return to Chapter 6, not to Chapter 7.

---

## Acceptance Criterion

A criterion found in Chapter 6: **5%**. Confirm the exact criterion in the TM.

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

**SC5-5.** If the tool passes all Chapter 7 checks, what does that tell you about its current state?

---

## Answer Key

**SC5-1.** See Chapter 7. The criterion is stated as a specific value with units.

**SC5-2.** Stop verification. Return to Chapter 6 and re-calibrate. Diagnose the cause before re-adjusting.

**SC5-3.** See Chapter 7, last numbered step. The log items are listed there.

**SC5-4.** Calibration allows adjustments; verification does not. Verification is independent confirmation.

**SC5-5.** It tells you the tool is performing within its specified accuracy at this time. It does not tell you how long it will remain in calibration — periodic re-calibration is required.

---

## Checkpoint

You are ready for the final assessment when you can:
- [ ] State the acceptance criterion from memory
- [ ] State the difference between calibration and verification
- [ ] State the required log items from memory

**→ Proceed to [Final Assessment](final_assessment.md)**
