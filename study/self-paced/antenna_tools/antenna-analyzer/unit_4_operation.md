# Unit 4 — Operating Procedures and Calculations
**[TM-TOOL-002 — Open Handout](../../../antenna_tools/antenna-analyzer/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Execute operating procedure; interpret readings; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-TOOL-002. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

5-1 SWR Sweep

1. Connect antenna to DUT port (SMA). Select band (HF or VHF/UHF).
2. Enter start/stop frequency and number of sweep points (101 or 201).
3. Press SWEEP. The display plots SWR vs. frequency in real time.
4. Identify the resonance (SWR minimum). Press MARKER; the instrument
 displays f_res, SWR, R, X at the marker frequency.

5-2 Smith Chart Display

1. Select SMITH mode. The sweep traces the impedance locus on the Smith chart.
2. Clockwise rotation with increasing frequency = capacitive reactance dominant
 (antenna too short). Counter-clockwise = inductive (antenna too long).
3. At resonance, the locus crosses the real axis; R at crossing = feedpoint
 resistance. Ideal dipole: 72Ω (free space), lower over ground.

---

## Appendix A — Reference Formulas

SWR = (1 + |Γ|) / (1 &minus; |Γ|)
|Γ| = (SWR &minus; 1) / (SWR + 1)
Return loss (dB) = &minus;20 log10(|Γ|)
Z_ant = 50 × (1 + Γ) / (1 &minus; Γ)    [complex arithmetic]

---

## Key Formulas Summary

Key mathematical relationships from Appendix A:

- `SWR = (1 + |Γ|) / (1 &minus; |Γ|)`
- `|Γ| = (SWR &minus; 1) / (SWR + 1)`
- `Return loss (dB) = &minus;20 log10(|Γ|)`
- `Z_ant = 50 × (1 + Γ) / (1 &minus; Γ)    [complex arithmetic]`

---

## Operating Notes

Chapter 5 specifies 7 operating steps.

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
using the ANTENNA ANALYZER — VECTOR IMPEDANCE MEASUREMENT. List steps in order.

**P4-2.** From Chapter 5: what measurement setting or configuration produces the most accurate
result with the ANTENNA ANALYZER — VECTOR IMPEDANCE MEASUREMENT? What is the tradeoff if you use a less accurate setting?

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
