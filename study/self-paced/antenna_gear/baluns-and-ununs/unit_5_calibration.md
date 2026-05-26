# Unit 5 — Calibration and Verification
**[TM-GEAR-003 — Open Handout](../../../antenna_gear/baluns-and-ununs/tm.html)**
**TM Chapters:** Chapter 6, Chapter 7
**ELOs:** Calibrate the BALUNS AND UN-UNS — CURRENT AND VOLTAGE TYPES; verify performance against acceptance criteria; complete calibration log
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-GEAR-003. Read **Chapter 6 — Calibration** and **Chapter 7 — Verification and Acceptance** completely.

Then come back here.

---

## Chapter 6 — Calibration Content

1. Measure impedance transformation ratio: connect a known resistive load
 (e.g., 200Ω for a 4:1 balun) to the balanced port; measure
 impedance at the unbalanced port with a NanoVNA. Target: 50Ω ±10%.
2. Measure common-mode impedance (choke action): connect NanoVNA port 1
 to coax shield at input; port 2 to coax shield at output.
 Z_CM should exceed 2000Ω across 1.8–30 MHz for type #31 core.
3. Measure insertion loss: S21 through the balun into a matched load.
 Target: <0.3 dB at all HF frequencies.

---

## Chapter 7 — Verification Content

1. Impedance ratio correct to within ±10% at center frequency.
2. Common-mode impedance ≥1000Ω at all specified frequencies.
3. Insertion loss <0.3 dB at 1.8–30 MHz.
4. SWR <1.2:1 at all test frequencies with correct load.
5. Power test: key a 100W transmitter into a dummy load through the balun
 for 30 seconds. No smoke, no temperature rise exceeding 40°C above ambient.
6. Log: date, design type, core material, turns, ratio test result, CM impedance, operator.

---

## Calibration vs. Verification — Distinction

**Calibration** (Chapter 6): active adjustments to bring the component within specification.
You are permitted to adjust, tweak, and repeat until the criterion is met.

**Verification** (Chapter 7): independent confirmation that the calibration succeeded.
No adjustments are permitted during verification. If a verification check fails, you
return to Chapter 6, not to Chapter 7.

---

## Acceptance Criterion

A criterion found in Chapter 6: **10%**. Confirm the exact criterion in the TM.

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
