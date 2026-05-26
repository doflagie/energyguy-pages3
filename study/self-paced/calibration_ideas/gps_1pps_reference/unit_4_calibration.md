# Unit 4 — Calibration Procedure and Calculations
**[TM-CAL-002 — Open Handout](../../../Calibration_Ideas/gps_1pps_reference/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Execute calibration procedure; compute error and determine pass/fail
**Estimated time:** 30 minutes

---

## Step 1: Read the TM

Open TM-CAL-002. Read **Chapter 5** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 — Calibration Procedure

> **CAUTION:** Do not use 1PPS for calibration until GPS lock is confirmed. Many modules continue to output 1PPS without lock; unlocked 1PPS can be off by milliseconds.

### 5-1. FREQUENCY COUNTER GATE CALIBRATION

1. Set frequency counter to 1 Hz external gate input or 1 s internal gate.
2. Apply 1PPS to the counter's external trigger or gate input.
3. Measure a 10 MHz oscillator with 10 s gate time. The count should be exactly 100,000,000 ± 1 count for a perfect oscillator.
4. Record: counts = fosc / f1PPS × gate time.
5. Error in ppm = (actual counts &minus; expected counts) / (expected counts) × 106.
6. Example: gate = 10 s, expected 100,000,000 counts, actual 100,000,210 counts → +2.1 ppm error.

### 5-2. OSCILLOSCOPE TIMEBASE CALIBRATION

1. Connect 1PPS to oscilloscope input. Set trigger to rising edge.
2. Set timebase to 200 ms/div (2 seconds full screen for a 10-div display).
3. The 1PPS pulse should appear at exactly 1.000000 s intervals.
4. Measure the displayed period using cursors or automated measurement.
5. Compare displayed period to 1.000000 s. Error >0.5% indicates timebase drift.
6. For 2–div cursor placement: set cursors at 0 ms and 1000 ms displayed. Count actual division spacing. Error = (measured &minus; 1000 ms) / 1000 ms × 100%.

---

## Appendix A — Formulas

```
Frequency error from 1PPS gate count
errorppm = (Nactual &minus; Nexpected) / Nexpected × 106
```

```
Expected counts (10 MHz oscillator, 10 s gate)
Nexpected = fosc × tgate = 10×106 × 10 = 100,000,000
```

```
1PPS accuracy in seconds per day
drifts/day = accuracyns × 10&minus;9 (this is per-pulse jitter, not accumulation)
```

---

## Key Formulas Summary

- errorppm = (Nactual &minus; Nexpected) / Nexpected × 106
- Nexpected = fosc × tgate = 10×106 × 10 = 100,000,000
- drifts/day = accuracyns × 10&minus;9 (this is per-pulse jitter, not accumulation)

---

## The Calibration Procedure

Chapter 5 specifies 12 calibration steps.

Calibration is a comparison: you apply a known reference value to the instrument under test and record what the instrument reads. The difference is the error. You then either:
- Adjust the instrument until the error is within the acceptance criterion, or
- Record the error as a correction factor to apply to future readings

An error criterion found in Chapter 5: **0.5%**. Confirm the exact criterion in the TM.

---

## Practice Problems

Work these before checking answers.

**P4-1.** The reference value is 10.000 V. Your instrument reads 10.043 V.
(a) What is the error in volts?
(b) What is the error as a percentage of the reference value? Show your work.

**P4-2.** Using the formula for % error: error% = (measured − reference) / reference × 100
Apply it to: reference = 100.0 kHz, measured = 99,985 Hz.
(a) Error in Hz.
(b) Error in %.
(c) Error in ppm.

**P4-3.** The acceptance criterion in the TM is ±1%. Your measurement gives an error of +0.8%. Does it pass? State your reasoning.

---

## Practice Problem Answers

**P4-1.**
(a) 10.043 − 10.000 = **+0.043 V**
(b) 0.043 / 10.000 × 100 = **+0.43%**

**P4-2.**
(a) 99,985 − 100,000 = **−15 Hz**
(b) −15 / 100,000 × 100 = **−0.015%**
(c) −15 / 100,000 × 1,000,000 = **−150 ppm**

**P4-3.** +0.8% is within ±1%. **PASS.** State: "error = +0.8%; criterion = ±1%; result = PASS." Always cite the TM section for the criterion.

---

## Self-Check Questions

**SC4-1.** How many steps does Chapter 5 specify for the calibration procedure?

**SC4-2.** What reference value(s) does Chapter 5 apply to the instrument under test?

**SC4-3.** State the calibration acceptance criterion from the TM. Cite the section.

**SC4-4.** Write the error formula from Appendix A. Include units.

**SC4-5.** If the instrument reads 2.3% high, is the error positive or negative? What does a positive error indicate?

---

## Answer Key

**SC4-1.** Count the numbered steps in Chapter 5. *(TM Ch. 5)*

**SC4-2.** See Chapter 5 for the reference values applied. These are the known-good inputs used to check the instrument. *(TM §5-1)*

**SC4-3.** See TM Chapter 5 or Chapter 7. Copy the criterion exactly with units and section number.

**SC4-4.** See Appendix A. Write it exactly as shown.

**SC4-5.** Positive (instrument reads higher than reference). A positive error means the instrument over-reads — it reports a value higher than the true value. *(TM App. A)*

---

## Checkpoint

Before proceeding, you must be able to:
- State the calibration acceptance criterion without looking
- Write the error formula from memory
- Work a % error and ppm calculation correctly

**→ Proceed to [Unit 5](unit_5_verification.md)**
