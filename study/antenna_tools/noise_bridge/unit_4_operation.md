# Unit 4 — Operating Procedures and Calculations
**[TM-TOOL-006 — Open Handout](../../antenna_tools/noise_bridge/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Execute operating procedure; interpret readings; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-TOOL-006. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

5-1 Measuring Antenna Impedance

1. Connect antenna to ANT BNC. Connect communications receiver (tuned to
 a clear frequency in the antenna's operating range) to DET BNC.
2. Enable noise source (power switch ON). Receiver S-meter should show
 S3–S7 noise level. If no noise: check battery, MMIC bias.
3. Set R_cal to 50Ω (center scale) and X_cal to zero (cap at minimum,
 inductor at minimum).
4. Adjust R_cal for minimum S-meter reading. Note: the null may be sharp;
 sweep slowly.
5. If null is imperfect at all R_cal values, the antenna has significant
 reactance. Adjust X_cal (capacitive for a high-SWR dipole that is
 too long; inductive for one that is too short) until the null is
 satisfactorily deep (≥10 dB below noise floor).
6. Read R and X from the calibration dials. Reported impedance:
 Z_ant = R_cal + jX_cal at the measurement frequency.

---

## Appendix A — Reference Formulas

| Component | 1 MHz | 7 MHz | 14 MHz | 28 MHz |
|---|---|---|---|---|
| 47 pF cap | &minus;3386Ω | &minus;484Ω | &minus;242Ω | &minus;121Ω |
| 100 pF cap | &minus;1592Ω | &minus;227Ω | &minus;114Ω | &minus;57Ω |
| 1 &micro;H inductor | +6.3Ω | +44Ω | +88Ω | +176Ω |
| 10 &micro;H inductor | +63Ω | +440Ω | +879Ω | +1759Ω |

---

## Key Formulas Summary

Key mathematical relationships from Appendix A:

*(See Appendix A in the TM)*

---

## Operating Notes

Chapter 5 specifies 6 operating steps.

Measurement discipline:
- Always warm up the tool and reference sources before taking readings
- Multiple readings improve accuracy — use 3–5 readings and average them
- Record every reading with frequency, power level, and settings
- A result that seems wrong almost always means a setup error, not a failed tool

---

## Practice Problems

Work these before reading the answer key below.

**P4-1.** Using the operating procedure from Chapter 5 and the formulas from Appendix A:
State the measurement steps you would take to characterize a 40m (7.150 MHz) dipole antenna
using the RF NOISE BRIDGE — IMPEDANCE MEASUREMENT. List steps in order.

**P4-2.** From Chapter 5: what measurement setting or configuration produces the most accurate
result with the RF NOISE BRIDGE — IMPEDANCE MEASUREMENT? What is the tradeoff if you use a less accurate setting?

**P4-3.** Chapter 5 specifies an operating procedure for a specific measurement scenario.
State the first three steps of that procedure from memory.

**P4-4.** Appendix A gives a formula for computing a result from measured values.
Pick one formula and compute a worked example using made-up but realistic values.
Show all work.

---

## Answer Key — Practice Problems

**P4-1.** Compare your list to Chapter 5. Steps should include: select range/mode → connect antenna → apply signal or enable source → adjust for null/maximum → read and record → compute result.

**P4-2.** See Chapter 5. The most accurate setting is usually the narrowest bandwidth or highest resolution, at the cost of longer measurement time or reduced range.

**P4-3.** See Chapter 5, steps 1–3. Copy exactly then close the TM and state from memory.

**P4-4.** See Appendix A for the formula. Your arithmetic is correct if your result has the right units and is physically plausible.

---

## Checkpoint

Before proceeding:
- [ ] You can state the operating procedure from memory (at least the first 5 steps)
- [ ] You can compute the derived quantity from Chapter 5 / Appendix A without looking
- [ ] You understand what "an unexpected result" tells you about setup

**→ Proceed to [Unit 5](unit_5_calibration.md)**
