# Unit 5 — Calibration and Verification
**[TM-TOOL-010 — Open Handout](../../../antenna_tools/swr_meter/tm.html)**
**TM Chapters:** Chapter 6, Chapter 7
**ELOs:** Calibrate the SWR METER — CONSTRUCTION AND USE; verify performance against acceptance criteria; complete calibration log
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-TOOL-010. Read **Chapter 6 — Calibration** and **Chapter 7 — Verification and Acceptance** completely.

Then come back here.

---

## Chapter 6 — Calibration Content

1. Connect a calibrated 50Ω dummy load to OUT. Apply known RF power
 (e.g., 10W at 14.175 MHz from a calibrated transmitter).
2. Adjust FWD calibration trimmer until FWD meter reads exactly full scale
 for the selected range (e.g., at the 10W mark on the 10W range).
3. Verify REF meter reads zero (or minimum). Adjust REF null trimmer if
 deflection >2% of FSD.
4. Apply a known 100Ω load (SWR 2.0:1). Verify SWR reads
 2.0 ± 0.2.
5. Repeat calibration procedure for VHF section at 145 MHz.

---

## Chapter 7 — Verification Content

1. SWR accuracy: measured SWR must be within ±0.1 SWR units
 for SWR <3:1; within ±10% for SWR 3:1–10:1.
2. Power accuracy: forward power reading within ±5% of actual power
 (verified with calibrated power reference).
3. Directivity check: with 50Ω load on OUT, REF deflection must be
 <2% of FWD deflection at full power (indicates ≥34 dB directivity).
4. Insertion loss: <0.1 dB at 1.8–30 MHz; <0.2 dB
 at 50–150 MHz; <0.5 dB at 150–450 MHz.
5. Log: date, calibration load used, forward calibration power, SWR accuracy
 check results, insertion loss, operator.

---

## Calibration vs. Verification — Distinction

**Calibration** (Chapter 6): active adjustments to bring the tool within specification.
You are permitted to adjust, tweak, and repeat until the criterion is met.

**Verification** (Chapter 7): independent confirmation that the calibration succeeded.
No adjustments are permitted during verification. If a verification check fails, you
return to Chapter 6, not to Chapter 7.

---

## Acceptance Criterion

A criterion found in Chapter 6: **2%**. Confirm the exact criterion in the TM.

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
