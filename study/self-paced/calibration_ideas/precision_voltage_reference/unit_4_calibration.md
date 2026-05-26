# Unit 4 — Calibration Procedure and Calculations
**[TM-CAL-006 — Open Handout](../../../Calibration_Ideas/precision_voltage_reference/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Execute calibration procedure; compute error and determine pass/fail
**Estimated time:** 30 minutes

---

## Step 1: Read the TM

Open TM-CAL-006. Read **Chapter 5** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 — Calibration Procedure

### 5-1. OUTPUT VOLTAGE MEASUREMENT

1. Allow reference to warm up for 15 minutes.
2. Measure output with the best available DMM (Fluke 15B+ or equivalent).
3. Record: Vmeas, ambient temperature, supply voltage.
4. Compare to nominal 5.000 V: error% = (Vmeas &minus; 5.000) / 5.000 × 100%.
5. For LT1021BCZ-5: accept ±0.05% (±2.5 mV). Trim with external resistor if needed.
6. For LM4040A: accept ±0.1% (±5.0 mV). Not adjustable; select units.

### 5-2. USING THE REFERENCE TO CALIBRATE A DMM

1. Connect reference output to DMM V input.
2. DMM should read within its specification of the reference voltage.
3. If DMM reads 4.985 V and reference is known 5.000 V, DMM error = &minus;3.0%.
4. Apply correction: actual voltage = DMM reading × (Vref / Vdmm).
5. Verify at multiple voltage settings using a precision resistor divider to derive additional reference points.

---

## Appendix A — Formulas

```
Voltage error in ppm
errorppm = (Vmeas &minus; Vnom) / Vnom × 106
```

```
Temperature error (LT1021, 5 ppm/°C)
ΔV = Vnom × TC × ΔT = 5.000 × 5×10&minus;6 × ΔT
```

```
Example: 10°C temperature change with LT1021
ΔV = 5.000 × 5×10&minus;6 × 10 = 0.00025 V = 0.25 mV
```

---

## Key Formulas Summary

- errorppm = (Vmeas &minus; Vnom) / Vnom × 106
- ΔV = Vnom × TC × ΔT = 5.000 × 5×10&minus;6 × ΔT
- ΔV = 5.000 × 5×10&minus;6 × 10 = 0.00025 V = 0.25 mV

---

## The Calibration Procedure

Chapter 5 specifies 11 calibration steps.

Calibration is a comparison: you apply a known reference value to the instrument under test and record what the instrument reads. The difference is the error. You then either:
- Adjust the instrument until the error is within the acceptance criterion, or
- Record the error as a correction factor to apply to future readings

An error criterion found in Chapter 5: **100%**. Confirm the exact criterion in the TM.

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
