# Unit 4 — Operating Procedures and Calculations
**[TM-GEAR-008 — Open Handout](../../antenna_gear/crystal_radio/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Install and operate the CRYSTAL RADIO — AM BROADCAST AND SHORTWAVE correctly; interpret performance data; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-GEAR-008. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

1. Connect a long outdoor wire antenna (30 m or more) and a good ground
 (cold water pipe, driven rod, or extensive counterpoise).
2. Put on high-impedance crystal earphones (or low-impedance via transformer).
 Rotate the tuning capacitor slowly from maximum capacitance to minimum.
 Stations appear as increases in audio level at their resonant settings.
3. Adjust antenna coupling tap for best audio level without degrading
 selectivity. Experiment: more coupling gives more volume but reduces
 the ability to separate adjacent stations.
4. For galena detector: use the cat&rsquo;s whisker to probe different spots
 on the crystal surface. The sensitive spot is small — a light touch
 is more effective than pressure.

---

## Appendix A — Reference Formulas

f0 = 1 / (2π × sqrt(L × C))

AM broadcast coverage:
  At C_max = 365 pF, f0 = 1/(2π×sqrt(250e-6 × 365e-12)) = 527 kHz
  At C_min = 10 pF,  f0 = 1/(2π×sqrt(250e-6 × 10e-12))  = 3.18 MHz
  (Reduce L to 50 μH for higher frequency coverage)

---

## Key Formulas Summary

Key mathematical relationships from Appendix A:

- `f0 = 1 / (2π × sqrt(L × C))`
- `At C_max = 365 pF, f0 = 1/(2π×sqrt(250e-6 × 365e-12)) = 527 kHz`
- `At C_min = 10 pF,  f0 = 1/(2π×sqrt(250e-6 × 10e-12))  = 3.18 MHz`

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
State the installation steps you would take to put the CRYSTAL RADIO — AM BROADCAST AND SHORTWAVE in service on a 40m (7.150 MHz) station.
List steps in order.

**P4-2.** From Chapter 5: what installation or setup detail produces the best RF performance
with the CRYSTAL RADIO — AM BROADCAST AND SHORTWAVE? What is the tradeoff if you omit or shortcut that step?

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
