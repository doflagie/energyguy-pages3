# Unit 5 — Calibration and Verification
**[TM-GEAR-004 — Open Handout](../../../antenna_gear/bms/tm.html)**
**TM Chapters:** Chapter 6, Chapter 7
**ELOs:** Calibrate the BATTERY MANAGEMENT SYSTEM (BMS) — LiFePO4 AND LI-ION; verify performance against acceptance criteria; complete calibration log
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-GEAR-004. Read **Chapter 6 — Calibration** and **Chapter 7 — Verification and Acceptance** completely.

Then come back here.

---

## Chapter 6 — Calibration Content

1. Calibrate cell voltage offsets: measure each cell with a precision DMM
 (Fluke 15B+). Compare to BQ76920 I2C readings. Enter offset corrections
 in firmware NVS (ESP32 Preferences library).
2. Calibrate current sense: apply a known load (resistive, measured with
 the calibrated DMM). Adjust firmware current-sense gain constant until
 displayed current matches measured.
3. Calibrate SOC: fully charge pack to 14.4V. Reset Coulomb counter to
 100%. Discharge completely to 10.0V. Record Ah removed. Compare to
 cell rated capacity. Adjust C_nominal in firmware.

---

## Chapter 7 — Verification Content

1. All cell voltages within 20 mV of each other at rest (balanced).
2. OV protection: charge one cell to 3.70V (using external bench supply on
 sense pins). Verify BQ76920 opens CHG FET within 2 seconds.
3. UV protection: discharge to 9.8V. Verify DSG FET opens within 1 second.
4. Overcurrent: apply a momentary 60 A load for 100 ms. Verify
 DSG FET opens (BQ76920 OCD detection).
5. Log: date, cell batch numbers, initial balance spread (mV), OV trip voltage,
 UV trip voltage, OCD trip current, measured capacity (Ah), operator.

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
