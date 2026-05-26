# Unit 5 — Calibration and Verification
**[TM-TOOL-007 — Open Handout](../../antenna_tools/rf_power_meter/tm.html)**
**TM Chapters:** Chapter 6, Chapter 7
**ELOs:** Calibrate the COMPACT PORTABLE RF POWER METER; verify performance against acceptance criteria; complete calibration log
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-TOOL-007. Read **Chapter 6 — Calibration** and **Chapter 7 — Verification and Acceptance** completely.

Then come back here.

---

## Chapter 6 — Calibration Content

1. Connect a calibrated RF power source (TinySA in generator mode, or
 signal generator with known output) to the IN port. Load OUT with
 a calibrated 50Ω dummy load.
2. Apply &minus;10 dBm, 0 dBm, +10 dBm, +20 dBm
 at 14 MHz (HF calibration). Record ADC voltage vs. known power.
3. Fit a calibration curve (linear or polynomial) to the V_det vs. P_in
 data. Store coefficients in ESP32 NVS. Accuracy after calibration:
 ±5% over the calibrated range.
4. Repeat calibration procedure at 144 MHz for the VHF/UHF section.

---

## Chapter 7 — Verification Content

1. Apply known 5W at 14.175 MHz. Display must show 5.0 ± 0.3W.
2. Apply known 5W at 145.0 MHz. Display must show 5.0 ± 0.3W.
3. Short OUT connector (SWR = ∞). Verify CYD shows SWR ≥99:1
 and forward/reflected power are approximately equal.
4. Insertion loss verification: compare power reading with coupler in-line
 vs. direct connection. Difference must be <0.2 dB.
5. Log: date, calibration source, power levels checked, measured vs. expected
 values, operator.

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
