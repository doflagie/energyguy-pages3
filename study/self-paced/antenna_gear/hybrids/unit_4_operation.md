# Unit 4 — Operating Procedures and Calculations
**[TM-GEAR-009 — Open Handout](../../../antenna_gear/hybrids/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Install and operate the RF HYBRID COUPLERS — QUADRATURE, WILKINSON, AND RAT-RACE correctly; interpret performance data; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-GEAR-009. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

5-1 Circular Polarization Feed

1. Connect the transceiver to Port 1 (input) of the branch-line hybrid.
 Connect a horizontally polarized element to Port 2 (0°) and a
 vertically polarized element to Port 3 (&minus;90°).
2. Port 4 (isolated) is terminated in a 50Ω load — do not leave it
 open; it must be terminated for correct operation.
3. Result: the two antenna elements receive equal power with 90° phase
 offset, producing circular polarization.

---

## Appendix A — Reference Formulas

| Z0 (Ω) | Trace width (mm) | Application |
|---|---|---|
| 50 | 2.9 | Main port connections |
| 35.35 | 5.0 | Branch-line series arms (Z0/&radic;2) |
| 70.7 | 1.7 | Wilkinson arms, rat-race ring (Z0×&radic;2) |

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
State the installation steps you would take to put the RF HYBRID COUPLERS — QUADRATURE, WILKINSON, AND RAT-RACE in service on a 40m (7.150 MHz) station.
List steps in order.

**P4-2.** From Chapter 5: what installation or setup detail produces the best RF performance
with the RF HYBRID COUPLERS — QUADRATURE, WILKINSON, AND RAT-RACE? What is the tradeoff if you omit or shortcut that step?

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
