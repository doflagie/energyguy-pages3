# Unit 4 — Calibration Procedure and Calculations
**[TM-CAL-015 — Open Handout](../../Calibration_Ideas/attenuator_pad_standards/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Execute calibration procedure; compute error and determine pass/fail
**Estimated time:** 30 minutes

---

## Step 1: Read the TM

Open TM-CAL-015. Read **Chapter 5** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 — Calibration Procedure

1. Connect pad to TinySA: source on input, TinySA on output.
2. Set TinySA to 0–200 MHz, measure insertion loss.
3. Verify attenuation = Ameasured at multiple frequencies: 1, 10, 30, 100, 200 MHz.
4. Attenuation should be flat within ±0.2 dB from DC to 100 MHz.
5. Measure input reflection: |S11| should be <&minus;20 dB (SWR <1.22) if resistor values are correct.
6. Record actual attenuation vs. designed value. Correction = Adesigned &minus; Ameasured.

---

## Appendix A — Formulas

```
Attenuation voltage ratio
k = 10AdB/20
```

```
Pi shunt resistors
Rshunt = Z0(k+1)/(k&minus;1)
```

```
Pi series resistor
Rseries = Z0 × 2k / (k2&minus;1)
```

```
Total power at input for measuring attenuation (dB from V ratio)
AdB = 20 × log10(Vin/Vout)
```

---

## Key Formulas Summary

- k = 10AdB/20
- Rshunt = Z0(k+1)/(k&minus;1)
- Rseries = Z0 × 2k / (k2&minus;1)
- AdB = 20 × log10(Vin/Vout)

---

## The Calibration Procedure

Chapter 5 specifies 6 calibration steps.

Calibration is a comparison: you apply a known reference value to the instrument under test and record what the instrument reads. The difference is the error. You then either:
- Adjust the instrument until the error is within the acceptance criterion, or
- Record the error as a correction factor to apply to future readings

Locate the acceptance criterion in Chapter 5 or Chapter 7.

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
