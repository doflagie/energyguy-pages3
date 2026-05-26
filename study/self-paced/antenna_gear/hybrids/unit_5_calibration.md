# Unit 5 — Calibration and Verification
**[TM-GEAR-009 — Open Handout](../../../antenna_gear/hybrids/tm.html)**
**TM Chapters:** Chapter 6, Chapter 7
**ELOs:** Calibrate the RF HYBRID COUPLERS — QUADRATURE, WILKINSON, AND RAT-RACE; verify performance against acceptance criteria; complete calibration log
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-GEAR-009. Read **Chapter 6 — Calibration** and **Chapter 7 — Verification and Acceptance** completely.

Then come back here.

---

## Chapter 6 — Calibration Content

1. Branch-line hybrid: with NanoVNA, measure S11 at Port 1 (must be
 <&minus;20 dB return loss at f0) and S21 to ports 2
 and 3 (&minus;3 ± 0.5 dB each). Measure S31 vs. S21 phase
 (must be &minus;90 ± 5°). S41 must be <&minus;30 dB
 (isolation).
2. Wilkinson: S11 <&minus;20 dB; S21 = S31 = &minus;3 ± 0.5 dB;
 S23 (isolation) <&minus;20 dB at center frequency.

---

## Chapter 7 — Verification Content

1. Amplitude balance: |S21| &minus; |S31| <0.5 dB.
2. Phase accuracy: Δφ within ±5° of design value (90°
 or 0°) at center frequency.
3. Return loss (input match): <&minus;20 dB at center frequency.
4. Isolation: <&minus;20 dB between output ports.
5. Power handling (Wilkinson): verify 100Ω isolation resistor rating.
 At 100W input, the resistor dissipates up to 50W if ports 2 and 3
 are mismatched — use 5W resistor minimum.
6. Log: date, design type, center frequency, S21/S31 balance, phase error,
 return loss, isolation, operator.

---

## Calibration vs. Verification — Distinction

**Calibration** (Chapter 6): active adjustments to bring the component within specification.
You are permitted to adjust, tweak, and repeat until the criterion is met.

**Verification** (Chapter 7): independent confirmation that the calibration succeeded.
No adjustments are permitted during verification. If a verification check fails, you
return to Chapter 6, not to Chapter 7.

---

## Acceptance Criterion

A criterion found in Chapter 6: **20 dB**. Confirm the exact criterion in the TM.

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
