# Unit 4 — Operating Procedures and Calculations
**[TM-TOOL-001 — Open Handout](../../../antenna_tools/adf_systems/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Execute operating procedure; interpret readings; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-TOOL-001. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

5-1 Watson-Watt Bearing Measurement

1. Tune receiver to target signal. Confirm adequate SNR (>20 dB for
 ±5° accuracy).
2. Enable 2-channel sampling. Record V_NS and V_EW amplitude values.
3. Compute bearing: `brg = atan2(V_NS, V_EW)`. Apply declination
 correction.
4. Confirm sense: enable sense antenna and verify the 180° correct quadrant.
5. Average 10 readings; discard outliers more than 15° from median.

5-2 Butler Matrix Bearing Estimate

1. Read RSSI on all four beam ports simultaneously (or in rapid sequence
 <10 ms total).
2. Identify the two highest-RSSI ports (adjacent beams straddle the signal).
3. Interpolate: bearing = beam_angle_1 + 45° × (RSSI_1 / (RSSI_1 + RSSI_2)).
4. Apply platform heading offset if mounted on a moving vehicle.

---

## Appendix A — Reference Formulas

| Parameter | Watson-Watt | Doppler | Butler 4-el |
|---|---|---|---|
| Accuracy (typical) | ±5° | ±2° | ±3° |
| Min SNR required | 20 dB | 15 dB | 10 dB |
| Multipath sensitivity | High | Medium | Low |
| Moving platform | Poor | Good | Good |
| Hardware complexity | Low | Medium | High |

---

## Key Formulas Summary

Key mathematical relationships from Appendix A:

*(See Appendix A in the TM)*

---

## Operating Notes

Chapter 5 specifies 9 operating steps.

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
using the ADF SYSTEMS — ANTENNA DIRECTION FINDING. List steps in order.

**P4-2.** From Chapter 5: what measurement setting or configuration produces the most accurate
result with the ADF SYSTEMS — ANTENNA DIRECTION FINDING? What is the tradeoff if you use a less accurate setting?

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
