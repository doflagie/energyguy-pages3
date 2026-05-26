# Unit 4 — Operating Procedures and Calculations
**[TM-TOOL-008 — Open Handout](../../antenna_tools/rf_power_meters/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Execute operating procedure; interpret readings; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-TOOL-008. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

5-1 Bird 43 Use

1. Insert correct slug for operating frequency and expected power level.
 Orient flat face for forward or reflected measurement.
2. Connect IN to transmitter, OUT to antenna (or dummy load). Key
 transmitter at target power. Read panel meter. For reflected power:
 rotate slug 180° (flat face away from meter).
3. Compute SWR from P_fwd and P_ref readings.

5-2 Inline Digital Meter

1. Insert meter inline between transmitter and antenna (any orientation;
 no slug rotation needed).
2. CYD displays P_fwd (W), P_ref (W), SWR simultaneously. Log readings
 to SD card if installed.

---

## Appendix A — Reference Formulas

| Bird part | Power | Frequency | Homebrew N2 |
|---|---|---|---|
| 5H | 5W | 25–60 MHz | 6 turns, #67 core |
| 50H | 50W | 25–60 MHz | 10 turns, #67 core |
| 100D | 100W | 400–1000 MHz | Microstrip PCB element |
| 250B | 250W | 2–30 MHz | 10 turns, #43 core |

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
using the RF POWER METERS — TYPES AND OPERATION. List steps in order.

**P4-2.** From Chapter 5: what measurement setting or configuration produces the most accurate
result with the RF POWER METERS — TYPES AND OPERATION? What is the tradeoff if you use a less accurate setting?

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
