# Unit 5 — Calibration and Verification
**[TM-GEAR-013 — Open Handout](../../../antenna_gear/power_supplies/tm.html)**
**TM Chapters:** Chapter 6, Chapter 7
**ELOs:** Calibrate the POWER SUPPLIES — LINEAR, SWITCHING, AND FIELD PORTABLE; verify performance against acceptance criteria; complete calibration log
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-GEAR-013. Read **Chapter 6 — Calibration** and **Chapter 7 — Verification and Acceptance** completely.

Then come back here.

---

## Chapter 6 — Calibration Content

1. Calibrate voltage: set output with a precision voltmeter (4.5 digits
 or better). Target: 13.800 ± 0.050V.
2. Calibrate current meter: apply a known resistive load and measure load
 current with an external shunt and DMM. Adjust current meter shunt
 gain in firmware until displayed value matches.
3. Calibrate over-current trip: increase load until the trip circuit
 activates. Trip current must be within 0.5A of the set value.

---

## Chapter 7 — Verification Content

1. Output voltage: 13.8 ± 0.1V at no load and full rated load.
2. Ripple: <10 mV peak-to-peak (linear); <100 mV pk-pk (SMPS)
 measured with oscilloscope at full load.
3. Current limit: trips within 0.5A of set value on all supplies.
4. SMPS EMI: with transceiver in receive mode, key the supply at full load.
 Noise floor must not increase by more than 1 S-unit at any HF frequency.
5. Log: date, supply type, measured voltage at no load and full load,
 ripple measurement, current limit trip point, operator.

---

## Calibration vs. Verification — Distinction

**Calibration** (Chapter 6): active adjustments to bring the component within specification.
You are permitted to adjust, tweak, and repeat until the criterion is met.

**Verification** (Chapter 7): independent confirmation that the calibration succeeded.
No adjustments are permitted during verification. If a verification check fails, you
return to Chapter 6, not to Chapter 7.

---

## Acceptance Criterion

Locate the acceptance criterion in Chapter 6 or Chapter 7.

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
