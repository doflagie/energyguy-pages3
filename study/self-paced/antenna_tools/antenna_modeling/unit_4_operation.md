# Unit 4 — Operating Procedures and Calculations
**[TM-TOOL-003 — Open Handout](../../../antenna_tools/antenna_modeling/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Execute operating procedure; interpret readings; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-TOOL-003. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

5-1 Running the Engine

from nec_runner import NECRunner, NECEngine
runner = NECRunner(engine=NECEngine.NEC2)
result = runner.run(model)
if not result.success:
 print(result.stderr)
The runner writes a temporary .nec file, invokes nec2c as a subprocess,
captures stdout/stderr, and returns a NECResult object. Timeout default:
60 seconds (adjustable for large sweeps).

5-2 Parsing Output
from nec_parser import NECOutputParser
parser = NECOutputParser()
points = parser.parse(result.output_file)
# points: list of {freq_mhz, gain_dbi, z_real, z_imag, swr, theta, phi}

---

## Appendix A — Reference Formulas

| Error | Cause | Fix |
|---|---|---|
| SEGMENT FAILURE | Wire too short for segment count | Reduce min_segs or increase length |
| Impedance = 0+j0 | Source on wrong tag/segment | Verify EX card tag matches GW tag |
| Gain >30 dBi | Wire below ground (Z 2 × radius |

---

## Key Formulas Summary

Key mathematical relationships from Appendix A:

- `| Impedance = 0+j0 | Source on wrong tag/segment | Verify EX card tag matches GW tag |`

---

## Operating Notes

Work through all steps in Chapter 5 in sequence.

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
using the NEC ANTENNA MODELING SYSTEM. List steps in order.

**P4-2.** From Chapter 5: what measurement setting or configuration produces the most accurate
result with the NEC ANTENNA MODELING SYSTEM? What is the tradeoff if you use a less accurate setting?

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
