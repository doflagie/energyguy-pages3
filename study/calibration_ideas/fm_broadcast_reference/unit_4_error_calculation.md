# Unit 4 — Computing Frequency Error in Hz and ppm
**[TM-CAL-004 — Open Handout](../../Calibration_Ideas/fm_broadcast_reference/tm.html)**
**ELOs:** 2.1, 2.2, 2.3, 2.4  
**Estimated time:** 30 minutes  
**TM sections to read first:** Section 5-1 (steps 3–6), Appendix A, Appendix B

---

## Step 1: Read the TM

Open TM-CAL-004. Read:
- Section 5-1, steps 3–6 (the calculation steps)
- Appendix A: Calculations and Formulas
- Appendix B: Example Results

Then come back here.

---

## What This Unit Covers

You have a measured frequency from the TinySA and an assigned frequency from the FCC database. This unit shows you how to turn those two numbers into an actionable correction value.

There are two calculations: error in Hz (absolute), and error in ppm (relative). You need both.

---

## The Two Formulas

### Formula 1 — Error in Hz

```
error_Hz = f_measured − f_assigned
```

This is simple subtraction. The sign matters:
- **Positive result** → TinySA read a higher frequency than the station was actually assigned → your oscillator is running **high**
- **Negative result** → TinySA read a lower frequency → your oscillator is running **low**

### Formula 2 — Error in ppm

```
error_ppm = error_Hz / f_assigned × 1,000,000
```

ppm (parts per million) expresses the error *relative* to the carrier frequency. This is the standard way to express oscillator error because it scales with frequency — a 10 ppm error at 10 MHz is 100 Hz, but the same 10 ppm error at 100 MHz is 1,000 Hz.

The ppm value is what you enter into the TinySA calibration menu. It is also how you compare your instrument against a specification.

---

## Worked Example 1 — From Appendix A

- Assigned: 98,100,000 Hz (98.100 MHz)
- Measured: 98,102,400 Hz

**Step 1:** error_Hz = 98,102,400 − 98,100,000 = **+2,400 Hz**

**Step 2:** error_ppm = 2,400 / 98,100,000 × 1,000,000 = **+24.5 ppm**

**Interpretation:** The TinySA read 2,400 Hz too high. The internal reference oscillator is running fast (high). The ppm correction to apply is −24.5 ppm.

---

## Worked Example 2 — From Appendix B

Three stations were measured. Their errors:

| Station | Assigned (MHz) | Measured (MHz) | error_Hz | error_ppm |
|---|---|---|---|---|
| KXYZ | 98.100 | 98.1002 | +200 Hz | +2.0 ppm |
| KABC | 103.500 | 103.5011 | +1,100 Hz | +10.6 ppm |
| KDEF | 107.700 | 107.7007 | +700 Hz | +6.5 ppm |

All three errors are positive — the TinySA is reading high on every station. The errors are not identical because each station has its own slight deviation from its assigned frequency, and because the TinySA's measurement has some noise.

**Averaging:** (2.0 + 10.6 + 6.5) / 3 = **6.4 ppm** average correction.

Averaging across stations reduces the effect of any one station's individual frequency offset and measurement noise. This is why TM-CAL-004 requires at least 2–3 stations, not just one.

---

## Verify Your Understanding — Work These Problems

Work each problem before checking the answer. Show your arithmetic.

---

**Problem 4-1.** Station KDEF is assigned 107.700 MHz. Your TinySA measures 107,700,700 Hz.

(a) Compute error_Hz.  
(b) Compute error_ppm.  
(c) Is the TinySA oscillator running high or low?

*Answer:*  
(a) 107,700,700 − 107,700,000 = **+700 Hz**  
(b) 700 / 107,700,000 × 10⁶ = **+6.5 ppm**  
(c) **High** (positive error)  
*(Matches Appendix B, TM-CAL-004)*

---

**Problem 4-2.** Station WXYZ is assigned 91.500 MHz. Your TinySA measures 91,499,100 Hz.

(a) Compute error_Hz.  
(b) Compute error_ppm.  
(c) Is the TinySA oscillator running high or low?

*Answer:*  
(a) 91,499,100 − 91,500,000 = **−900 Hz**  
(b) −900 / 91,500,000 × 10⁶ = **−9.8 ppm**  
(c) **Low** (negative error — TinySA is reading frequencies lower than actual)

---

**Problem 4-3.** You measure three stations and get these errors: +8.2 ppm, +6.9 ppm, +7.5 ppm. What average correction should you apply to the TinySA?

*Answer:*  
(8.2 + 6.9 + 7.5) / 3 = 22.6 / 3 = **+7.5 ppm**  
Apply **−7.5 ppm** correction (negative, to bring the high-running oscillator down).

---

**Problem 4-4.** After applying a −7.5 ppm correction, you re-measure a station assigned 103.900 MHz. You would expect the residual error to be less than what value in Hz?

*Answer:*  
Acceptance criterion is <2 kHz. So residual error < **2,000 Hz** on any station. *(TM Ch. 7)*

---

## Self-Check Questions

**SC4-1.** Write the formula for error in Hz.

**SC4-2.** Write the formula for error in ppm.

**SC4-3.** A measured frequency is 450 Hz *below* the assigned frequency. Is the oscillator running high or low?

**SC4-4.** Why do you measure 2–3 stations and average the errors rather than correcting from a single measurement?

---

## Answer Key

**SC4-1.** error_Hz = f_measured − f_assigned *(TM App. A)*

**SC4-2.** error_ppm = error_Hz / f_assigned × 1,000,000 *(TM App. A)*

**SC4-3.** Low — measured is below assigned, so the oscillator is reading frequencies lower than actual. *(TM §5-1, step 6)*

**SC4-4.** Each station has its own slight individual offset from assigned frequency, and each measurement has noise. Averaging reduces the influence of both. *(TM §5-1, step 7)*

---

## Checkpoint

Before proceeding to Unit 5, you must be able to:
- Write both formulas from memory
- Correctly determine high vs. low from the sign of the result
- Work a complete problem (Hz and ppm) without looking at the formulas

If you missed any of problems 4-1 through 4-4, work through them again before continuing.

**→ Proceed to [Unit 5](unit_5_calibration_verification.md)**
