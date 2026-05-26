# Unit 4 — Operating Procedures and Calculations
**[TM-GEAR-006 — Open Handout](../../../antenna_gear/coil_winder/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Install and operate the MOTORIZED COIL WINDER — ESP32 CONTROLLER correctly; interpret performance data; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-GEAR-006. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

1. Home the traverse axis: press HOME button. Traverse moves to minus
 limit switch and sets position to zero.
2. Enter winding parameters via encoder: wire gauge (AWG), turns, coil
 length (or pitch), wind direction (CW or CCW looking at spindle).
3. Press START. The controller winds the specified turns, counting on
 the OLED display. Press PAUSE to stop at any point; press RESUME
 to continue.
4. After completion: the controller stops and displays DONE + final turn count.
 The completed program is logged to the SD card with date, wire gauge,
 turns, and any errors.
5. For multi-layer coils: increment the layer counter in the program.
 The traverse will reverse direction automatically for each layer.

---

## Appendix A — Reference Formulas

| AWG | OD bare (mm) | OD insulated (mm) | Max current (A) |
|---|---|---|---|
| 14 | 1.628 | 1.95 | 15 |
| 18 | 1.024 | 1.30 | 7 |
| 22 | 0.644 | 0.85 | 3 |
| 26 | 0.405 | 0.55 | 1 |
| 28 | 0.321 | 0.45 | 0.5 |
| 30 | 0.255 | 0.35 | 0.3 |

---

## Key Formulas Summary

Key mathematical relationships from Appendix A:

*(See Appendix A in the TM)*

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
State the installation steps you would take to put the MOTORIZED COIL WINDER — ESP32 CONTROLLER in service on a 40m (7.150 MHz) station.
List steps in order.

**P4-2.** From Chapter 5: what installation or setup detail produces the best RF performance
with the MOTORIZED COIL WINDER — ESP32 CONTROLLER? What is the tradeoff if you omit or shortcut that step?

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
