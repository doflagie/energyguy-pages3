# Unit 5 — Calibration and Verification
**[TM-TOOL-006 — Open Handout](../../antenna_tools/noise_bridge/tm.html)**
**TM Chapters:** Chapter 6, Chapter 7
**ELOs:** Calibrate the RF NOISE BRIDGE — IMPEDANCE MEASUREMENT; verify performance against acceptance criteria; complete calibration log
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-TOOL-006. Read **Chapter 6 — Calibration** and **Chapter 7 — Verification and Acceptance** completely.

Then come back here.

---

## Chapter 6 — Calibration Content

1. Connect a precision 50Ω non-inductive resistor to ANT port. Set
 R_cal = 50Ω, X_cal = 0. Verify null at ≥20 dB below noise floor.
 If null is <20 dB: re-check bridge transformer balance.
2. Connect a known 100Ω resistor. Set R_cal = 100Ω, X_cal = 0.
 Verify null is achieved at that setting.
3. Connect a 47 pF NP0 capacitor. At 10 MHz:
 X_C = 1/(2π×10e6×47e-12) = &minus;338Ω.
 Set R_cal = 0Ω (resistor is pure reactance), X_cal to
 &minus;338Ω (capacitive). Verify null.
4. Record calibration date and reference components used in log.

---

## Chapter 7 — Verification Content

1. Measure a known 50Ω dummy load: R must read 50 ± 3Ω,
 X must read 0 ± 10Ω.
2. Measure a known antenna of known resonant frequency (verified by NanoVNA).
 At resonance, X should read 0 ± 15Ω; R should agree with NanoVNA
 feedpoint resistance within 10%.
3. Log: date, reference impedances measured, null depth achieved, measured
 vs. expected values, operator.

---

## Calibration vs. Verification — Distinction

**Calibration** (Chapter 6): active adjustments to bring the tool within specification.
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
