# Unit 4 — Operating Procedures and Calculations
**[TM-GEAR-011 — Open Handout](../../../antenna_gear/mast_tower/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Install and operate the MAST AND TOWER — PORTABLE AND FIXED INSTALLATIONS correctly; interpret performance data; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-GEAR-011. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

1. Never erect a mast within fall distance of power lines.
 Required clearance: mast height + 3 m.
2. For portable operation, lower the antenna before moving the mast.
 Telescoping sections must be collapsed before transport; partially
 extended masts have the lowest collapse load capacity.
3. Inspect all guy wire attachment points before each use. Replace any
 guy wire showing kinks, broken strands, or corrosion at terminations.
4. In winds exceeding 50 mph: lower the antenna or lower the mast
 entirely if the design survival wind speed is less than the forecast wind.

---

## Appendix A — Reference Formulas

10 m aluminum mast, 50 mm OD, at 70 mph (31.3 m/s):

q = 0.613 × 31.3² = 0.613 × 979 = 600 Pa
A = 0.050 m × 10 m = 0.50 m²
F = 600 × 1.0 × 0.50 = 300 N (67 lbf) on the mast alone

Add 2-element Yagi (projected area ~0.5 m²) at top:
F_antenna = 600 × 1.2 × 0.50 = 360 N
Total F = 300 + 360 = 660 N (148 lbf)
Overturning moment M = 660 × 5 m = 3300 N·m (mast + antenna CG)

Guy wire tension (3 guys, 45° angle, 6 m radius):
T = 3300 / (6 × 0.707 × 2/3) = 3300 / 2.83 = 1166 N per guy
Use 1/4" Phillystran (rated 2200 N) with 3:1 safety factor.

---

## Key Formulas Summary

Key mathematical relationships from Appendix A:

- `q = 0.613 × 31.3² = 0.613 × 979 = 600 Pa`
- `A = 0.050 m × 10 m = 0.50 m²`
- `F = 600 × 1.0 × 0.50 = 300 N (67 lbf) on the mast alone`
- `F_antenna = 600 × 1.2 × 0.50 = 360 N`
- `Total F = 300 + 360 = 660 N (148 lbf)`
- `Overturning moment M = 660 × 5 m = 3300 N·m (mast + antenna CG)`

---

## Operating Notes

Chapter 5 specifies 4 operating steps.

Installation and operating discipline:
- Always verify polarity and orientation before making connections — RF transformers and baluns are phase-sensitive
- Route feedlines away from parallel conductors — parallel runs create mutual coupling that degrades isolation
- Ground all exposed metalwork at a single chassis point — multiple grounds create loops
- Record settings, frequencies, and power levels for every test — you need baseline data for comparisons

---

## Practice Problems

Work these before reading the answer key below.

**P4-1.** Using the operating procedure from Chapter 5 and the formulas from Appendix A:
State the installation steps you would take to put the MAST AND TOWER — PORTABLE AND FIXED INSTALLATIONS in service on a 40m (7.150 MHz) station.
List steps in order.

**P4-2.** From Chapter 5: what installation or setup detail produces the best RF performance
with the MAST AND TOWER — PORTABLE AND FIXED INSTALLATIONS? What is the tradeoff if you omit or shortcut that step?

**P4-3.** Chapter 5 specifies an operating procedure for a specific use case.
State the first three steps of that procedure from memory.

**P4-4.** Appendix A gives a formula for computing a result from measured values.
Pick one formula and compute a worked example using made-up but realistic values.
Show all work.

---

## Answer Key — Practice Problems

**P4-1.** Compare your list to Chapter 5. Steps should include: select mounting location → connect to feedline/antenna → verify polarity/orientation → apply power or signal → verify operation → record baseline.

**P4-2.** See Chapter 5. The most important installation detail is usually physical orientation, lead length, or ground bonding — the tradeoff if omitted is degraded isolation, increased SWR, or common-mode current leakage.

**P4-3.** See Chapter 5, steps 1–3. Copy exactly then close the TM and state from memory.

**P4-4.** See Appendix A for the formula. Your arithmetic is correct if your result has the right units and is physically plausible.

---

## Checkpoint

Before proceeding:
- [ ] You can state the operating procedure from memory (at least the first 5 steps)
- [ ] You can compute the derived quantity from Chapter 5 / Appendix A without looking
- [ ] You understand what a degraded or unexpected result tells you about the installation

**→ Proceed to [Unit 5](unit_5_calibration.md)**
