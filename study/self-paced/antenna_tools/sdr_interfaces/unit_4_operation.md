# Unit 4 — Operating Procedures and Calculations
**[TM-TOOL-009 — Open Handout](../../../antenna_tools/sdr_interfaces/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Execute operating procedure; interpret readings; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-TOOL-009. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

5-1 HF Reception with Upconverter

1. Enable bypass relay for frequencies ≥60 MHz (GPIO or front-panel
 switch). For 0.1–30 MHz: use upconverter path.
2. In SDR software: tune to 125 + f_HF (MHz). E.g., 40m = 7.15 MHz
 → tune SDR to 132.15 MHz.
3. Enable preselector for the band in use to reduce out-of-band
 interference. FM broadcast (88–108 MHz) causes severe IMD
 in RTL-SDR at HF; preselector eliminates this.

5-2 Adjusting Gain

1. Start with maximum attenuation (31.5 dB) on PE4302. Reduce in
 6 dB steps until the noise floor drops and desired signals become
 visible.
2. Enable SPF5189Z LNA only for weak-signal work (<&minus;100 dBm);
 for strong-signal environments (near broadcast transmitters), keep
 LNA off to avoid IMD.

---

## Appendix A — Reference Formulas

| Software | Setting | Value for 125 MHz LO |
|---|---|---|
| SDR# | Shift frequency | +125000000 Hz |
| GQRX | LNB LO (Tools → Offset) | 125.000 MHz |
| GNU Radio | Frequency subtract block | 125e6 Hz offset in flowgraph |
| CubicSDR | Not supported natively; use IF math | — |

---

## Key Formulas Summary

Key mathematical relationships from Appendix A:

*(See Appendix A in the TM)*

---

## Operating Notes

Chapter 5 specifies 5 operating steps.

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
using the SDR INTERFACES AND PRESELECTOR. List steps in order.

**P4-2.** From Chapter 5: what measurement setting or configuration produces the most accurate
result with the SDR INTERFACES AND PRESELECTOR? What is the tradeoff if you use a less accurate setting?

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
