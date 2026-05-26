# Unit 4 — Calibration Procedure and Calculations
**[TM-CAL-003 — Open Handout](../../../Calibration_Ideas/gpsdo_reference/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Execute calibration procedure; compute error and determine pass/fail
**Estimated time:** 30 minutes

---

## Step 1: Read the TM

Open TM-CAL-003. Read **Chapter 5** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 — Calibration Procedure

### 5-1. INITIAL FREQUENCY CALIBRATION

1. Connect GPSDO 10 MHz output to frequency counter via 50 Ω BNC cable.
2. Allow GPSDO to lock completely (GPS fix acquired + 15 min warm-up).
3. Measure output frequency. Should read 10.000000 ±0.010 MHz within 5 minutes of lock.
4. If frequency is off by more than 1 Hz, check Si5351 crystal frequency setting in firmware. The on-board crystal may be 25.000 or 26.000 MHz; verify and update firmware constant.
5. Monitor frequency for 30 minutes. Deviation from 10.000000 should be 0.1 Hz over 24 hours indicates PLL loop filter or crystal aging issue.
4. Compare to WWV at 10 MHz using zero-beat method (see TM-CAL-001). Agreement within 1 Hz confirms NIST traceability.

---

## Appendix A — Formulas

```
Si5351A output frequency
fout = fxtal × PLL_mult / (output_divider)
```

```
PPM error from integer count
errorppm = (fmeasured &minus; 10,000,000) / 10
```

```
Allan deviation (GPSDO estimate)
ADEV(&tau; = 1 s) ≈ 1×10&minus;9 (crystal limited)
```

---

## Key Formulas Summary

- fout = fxtal × PLL_mult / (output_divider)
- errorppm = (fmeasured &minus; 10,000,000) / 10
- ADEV(&tau; = 1 s) ≈ 1×10&minus;9 (crystal limited)

---

## The Calibration Procedure

Chapter 5 specifies 6 calibration steps.

Calibration is a comparison: you apply a known reference value to the instrument under test and record what the instrument reads. The difference is the error. You then either:
- Adjust the instrument until the error is within the acceptance criterion, or
- Record the error as a correction factor to apply to future readings

An error criterion found in Chapter 5: **50 Ω**. Confirm the exact criterion in the TM.

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
