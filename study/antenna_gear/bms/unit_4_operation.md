# Unit 4 — Operating Procedures and Calculations
**[TM-GEAR-004 — Open Handout](../../antenna_gear/bms/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Install and operate the BATTERY MANAGEMENT SYSTEM (BMS) — LiFePO4 AND LI-ION correctly; interpret performance data; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-GEAR-004. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

1. First charge: connect a 14.6V constant-voltage charger (LiFePO4 profile,
 CC/CV). Monitor all four cell voltages on the CYD display during the
 first charge cycle. If any cell exceeds 3.65V before the others reach
 3.45V, the pack is unbalanced — stop and balance manually.
2. Normal operation: the CYD displays pack voltage, current, SOC (from
 Coulomb counting), and individual cell voltages. SOC warning at 20%;
 shutdown at 10% (10.0V).
3. Temperature limits: if any cell or FET temperature exceeds 55°C during
 discharge, reduce load. Charging is inhibited below 0°C by firmware.

---

## Appendix A — Reference Formulas

| SOC (%) | Cell voltage (V) | 4S pack voltage (V) |
|---|---|---|
| 100% | 3.60 | 14.40 |
| 80% | 3.35 | 13.40 |
| 50% | 3.25 | 13.00 |
| 20% | 3.10 | 12.40 |
| 0% | 2.50 | 10.00 |

Note: LiFePO4 voltage vs. SOC is very flat from 20%–80%. Coulomb
counting is more accurate than voltage for SOC estimation in this range.

---

## Key Formulas Summary

Key mathematical relationships from Appendix A:

*(See Appendix A in the TM)*

---

## Operating Notes

Chapter 5 specifies 3 operating steps.

Installation and operating discipline:
- Always verify polarity and orientation before making connections — RF transformers and baluns are phase-sensitive
- Route feedlines away from parallel conductors — parallel runs create mutual coupling that degrades isolation
- Ground all exposed metalwork at a single chassis point — multiple grounds create loops
- Record settings, frequencies, and power levels for every test — you need baseline data for comparisons

---

## Practice Problems

Work these before reading the answer key below.

**P4-1.** Using the operating procedure from Chapter 5 and the formulas from Appendix A:
State the installation steps you would take to put the BATTERY MANAGEMENT SYSTEM (BMS) — LiFePO4 AND LI-ION in service on a 40m (7.150 MHz) station.
List steps in order.

**P4-2.** From Chapter 5: what installation or setup detail produces the best RF performance
with the BATTERY MANAGEMENT SYSTEM (BMS) — LiFePO4 AND LI-ION? What is the tradeoff if you omit or shortcut that step?

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
