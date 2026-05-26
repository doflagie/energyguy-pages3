# Unit 5 — Calibration and Verification
**[TM-GEAR-019 — Open Handout](../../../antenna_gear/variable_caps/tm.html)**
**TM Chapters:** Chapter 6, Chapter 7
**ELOs:** Calibrate the VARIABLE CAPACITORS — AIR, MOTORIZED, AND STACK-ON FLAT-PLATE; verify performance against acceptance criteria; complete calibration log
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-GEAR-019. Read **Chapter 6 — Calibration** and **Chapter 7 — Verification and Acceptance** completely.

Then come back here.

---

## Chapter 6 — Calibration Content

1. Motorized cap: move the capacitor to 10 equally-spaced pot positions.
 Measure capacitance at each position with an LCR meter or NanoVNA (use
 the impedance measurement mode at a known frequency). Store the
 (ADC_value, pF) table in ESP32 NVS.
2. Stack-on caps: measure each unit individually with an LCR meter at
 100 kHz. Label each unit with its measured capacitance. Build
 a combination table: n units in parallel = n × C_unit.

---

## Chapter 7 — Verification Content

1. Motorized cap positioning: command each of 5 target capacitances;
 measure actual capacitance; error must be <5% of target.
2. Limit switch test: command travel past the end limits; controller must
 stop at the limit switches without mechanical damage to the capacitor.
3. Stack-on units: each unit must measure within 20% of its labeled value
 (LCR meter verification; flat-plate caps have significant tolerance).
4. HV rating: apply the rated voltage across the capacitor at maximum
 capacitance setting and hold for 10 seconds. No arc-over or increase
 in leakage current.
5. Log: date, cap type, calibration table (10 points for motorized),
 stack-on unit values, positioning accuracy, operator.

---

## Calibration vs. Verification — Distinction

**Calibration** (Chapter 6): active adjustments to bring the component within specification.
You are permitted to adjust, tweak, and repeat until the criterion is met.

**Verification** (Chapter 7): independent confirmation that the calibration succeeded.
No adjustments are permitted during verification. If a verification check fails, you
return to Chapter 6, not to Chapter 7.

---

## Acceptance Criterion

A criterion found in Chapter 6: **100 kHz**. Confirm the exact criterion in the TM.

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
