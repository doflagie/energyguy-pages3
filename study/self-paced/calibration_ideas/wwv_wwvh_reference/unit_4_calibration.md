# Unit 4 — Calibration Procedure and Calculations
**[TM-CAL-001 — Open Handout](../../../Calibration_Ideas/wwv_wwvh_reference/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Execute calibration procedure; compute error and determine pass/fail
**Estimated time:** 30 minutes

---

## Step 1: Read the TM

Open TM-CAL-001. Read **Chapter 5** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 — Calibration Procedure

> **CAUTION:** If calibrating the frequency counter's internal oscillator (TCXO/OCXO), refer to the counter manufacturer's manual. Some counters require internal adjustment access; incorrect adjustment can degrade accuracy.

### 5-1. DIRECT CARRIER METHOD (PREFERRED)

This method measures the received carrier directly with the frequency counter. It requires a receiver with a BFO output or accessible IF/detector output.

1. Tune receiver to 10.000 MHz (best daytime signal) or 5.000 MHz (night).
2. Switch receiver to CW or USB mode. Zero-beat the carrier: the audio beat note approaches zero Hz as receiver VFO aligns with the carrier.
3. Measure the receiver VFO frequency at zero-beat. This equals 10.000000 MHz (within ±5 Hz for consumer receivers, ±100 Hz for older equipment).
4. If the counter shows 10.000100 MHz at zero-beat, the counter's reference is high by 10 Hz at 10 MHz = 1 ppm high.
5. Apply correction: freqtrue = freqdisplayed × (1 &minus; errorppm/106).
6. Repeat on 5.000 MHz and 15.000 MHz to verify consistency.
7. Record result in calibration log.

### 5-2. AUDIO BEAT NOTE METHOD

If direct carrier access is not available, beat the carrier against a known oscillator and count the beat note frequency.

1. Connect a 10 MHz signal generator (or GPSDO output) to one input of a signal mixer or the RF input of a second receiver.
2. Receive WWV on the first receiver.
3. Mix the two signals; the difference frequency (beat note) is the error between your oscillator and WWV.
4. Count the beat note with the frequency counter. Any non-zero reading is the error of your signal generator at 10 MHz.
5. Error in ppm = beatHz / 10 (at 10 MHz reference).

---

## Appendix A — Formulas

### A-1. FREQUENCY ERROR

```
Parts-per-million error
errorppm = (fmeasured &minus; fnominal) / fnominal × 106
```

```
Correction factor
ftrue = fdisplayed × (1 &minus; errorppm / 106)
```

```
Hz error from ppm (10 MHz)
errorHz = errorppm × 10 (at 10 MHz)
```

### A-2. PROPAGATION DELAY

```
One-way propagation delay (ionospheric, approximate)
delay ≈ 3.3 ms per 1000 km
```

> **NOTE:** Propagation delay affects *phase* and *timing* measurements only. For frequency calibration, the Doppler shift from a quasi-static ionosphere averages to zero over 30 seconds or more. Time-of-day synchronization requires propagation delay correction; frequency calibration does not.

---

## Key Formulas Summary

- errorppm = (fmeasured &minus; fnominal) / fnominal × 106
- ftrue = fdisplayed × (1 &minus; errorppm / 106)
- errorHz = errorppm × 10 (at 10 MHz)

---

## The Calibration Procedure

Chapter 5 specifies 12 calibration steps.

Calibration is a comparison: you apply a known reference value to the instrument under test and record what the instrument reads. The difference is the error. You then either:
- Adjust the instrument until the error is within the acceptance criterion, or
- Record the error as a correction factor to apply to future readings

An error criterion found in Chapter 5: **1 ppm**. Confirm the exact criterion in the TM.

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
