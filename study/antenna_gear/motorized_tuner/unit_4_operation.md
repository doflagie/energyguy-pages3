# Unit 4 — Operating Procedures and Calculations
**[TM-GEAR-012 — Open Handout](../../antenna_gear/motorized_tuner/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Install and operate the MOTORIZED ANTENNA TUNER — HF AND VHF/UHF correctly; interpret performance data; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-GEAR-012. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

5-1 Manual Tuning

1. Select band on the CYD touchscreen. The tuner recalls the last
 successful setting for that band and moves the components to that position.
2. Key the transmitter at reduced power (5–10W for initial tune).
 Observe the SWR on the CYD display. Adjust C1, C2, and L buttons
 until SWR 5-2 Auto-Tune

1. Key the transmitter at 10W. Press AUTO on the CYD. The ESP32 runs
 the coordinate descent algorithm. Typical tuning time: 10–30 seconds.
2. The display shows SWR progress. When SWR <1.5:1, the algorithm stops
 and saves the band setting to NVS.
3. If auto-tune reaches minimum movement without achieving SWR <1.5:1,
 the antenna may be outside the tuner&rsquo;s matching range for this band.
 Check antenna connections and feedline continuity.

---

## Appendix A — Reference Formulas

T-network with C1_max = C2_max = 365 pF, L_max = 28 μH:
  At 14 MHz:  matchable impedance range = ~5Ω to ~2000Ω
  At 7 MHz:   matchable impedance range = ~10Ω to ~1000Ω
  At 3.5 MHz: matchable impedance range = ~20Ω to ~500Ω
  (Rough estimates; actual range depends on losses and Q)

---

## Key Formulas Summary

Key mathematical relationships from Appendix A:

- `T-network with C1_max = C2_max = 365 pF, L_max = 28 μH:`
- `At 14 MHz:  matchable impedance range = ~5Ω to ~2000Ω`
- `At 7 MHz:   matchable impedance range = ~10Ω to ~1000Ω`
- `At 3.5 MHz: matchable impedance range = ~20Ω to ~500Ω`

---

## Operating Notes

Chapter 5 specifies 5 operating steps.

Installation and operating discipline:
- Always verify polarity and orientation before making connections — RF transformers and baluns are phase-sensitive
- Route feedlines away from parallel conductors — parallel runs create mutual coupling that degrades isolation
- Ground all exposed metalwork at a single chassis point — multiple grounds create loops
- Record settings, frequencies, and power levels for every test — you need baseline data for comparisons

---

## Practice Problems

Work these before reading the answer key below.

**P4-1.** Using the operating procedure from Chapter 5 and the formulas from Appendix A:
State the installation steps you would take to put the MOTORIZED ANTENNA TUNER — HF AND VHF/UHF in service on a 40m (7.150 MHz) station.
List steps in order.

**P4-2.** From Chapter 5: what installation or setup detail produces the best RF performance
with the MOTORIZED ANTENNA TUNER — HF AND VHF/UHF? What is the tradeoff if you omit or shortcut that step?

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
