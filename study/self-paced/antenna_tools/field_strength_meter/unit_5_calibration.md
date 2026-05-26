# Unit 5 — Calibration and Verification
**[TM-TOOL-005 — Open Handout](../../../antenna_tools/field_strength_meter/tm.html)**
**TM Chapters:** Chapter 6, Chapter 7
**ELOs:** Calibrate the FIELD STRENGTH METER — CONSTRUCTION AND USE; verify performance against acceptance criteria; complete calibration log
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-TOOL-005. Read **Chapter 6 — Calibration** and **Chapter 7 — Verification and Acceptance** completely.

Then come back here.

---

## Chapter 6 — Calibration Content

6-1 Absolute Calibration (Active and Digital FSMs)

1. Place a calibrated signal generator (or TinySA in generator mode) at
 a measured distance r from the FSM probe.
2. Set generator output to a known level P (dBm) into a calibrated antenna
 with known gain G (dBi). Compute E-field at distance r:

E (V/m) = sqrt(30 × P_watts × G_linear) / r

1. Record FSM reading at this field level. This is the calibration reference
 point. Adjust offset constant in firmware so that the CYD display
 shows the calculated E-field value.
2. Repeat at &minus;10, &minus;20, &minus;30 dB relative levels using known
 attenuators. Verify linearity within ±1 dB across the range.

---

## Chapter 7 — Verification Content

1. Passive FSM: connect a 0 dBm signal (from TinySA or signal generator)
 at 7 MHz via 50Ω coax. Meter should deflect to a repeatable scale
 reading. Deflection should decrease by approximately half (6 dB) when
 a 6 dB attenuator is inserted.
2. Digital FSM: apply &minus;40 dBm, &minus;50 dBm, &minus;60 dBm.
 Verify CYD readings within ±2 dBm of expected values.
 (AD8307 typical accuracy: ±1 dB from &minus;74 to +10 dBm.)
3. Log: date, version (passive/active/digital), calibration signal source,
 calibration power level, measured offset, operator.

---

## Calibration vs. Verification — Distinction

**Calibration** (Chapter 6): active adjustments to bring the tool within specification.
You are permitted to adjust, tweak, and repeat until the criterion is met.

**Verification** (Chapter 7): independent confirmation that the calibration succeeded.
No adjustments are permitted during verification. If a verification check fails, you
return to Chapter 6, not to Chapter 7.

---

## Acceptance Criterion

A criterion found in Chapter 6: **30 dB**. Confirm the exact criterion in the TM.

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
