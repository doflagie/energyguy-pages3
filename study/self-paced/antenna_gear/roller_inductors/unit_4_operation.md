# Unit 4 — Operating Procedures and Calculations
**[TM-GEAR-017 — Open Handout](../../../antenna_gear/roller_inductors/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Install and operate the ROLLER INDUCTORS — MOTORIZED VARIABLE INDUCTANCE correctly; interpret performance data; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-GEAR-017. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

5-1 Homing Sequence

1. On power-up, the controller automatically homes: moves toward the home
 limit switch at 100 steps/sec until the switch opens; then backs off
 at 50 steps/sec until the switch closes again. This position is zero.
2. After homing, move to the last-used position (stored in NVS).

5-2 Setting Inductance

1. Enter the desired inductance in &micro;H on the CYD or OLED display.
 The ESP32 looks up the corresponding step count in the calibration table
 and commands the motor.
2. For SWR-guided auto-tuning: the tuner controller (TM-GEAR-012) sends
 the target inductance via I2C or UART to the roller inductor controller.

---

## Appendix A — Reference Formulas

| Model | Max L (&micro;H) | Turns | Current rating (A) |
|---|---|---|---|
| Cardwell 154-33 | 33 | 45 | 15 |
| B&W 850 | 25 | 40 | 20 |
| Barker Williamson 851 | 50 | 56 | 20 |
| Homebrew (see TM-GEAR-006) | Variable | Variable | Depends on wire gauge |

---

## Key Formulas Summary

Key mathematical relationships from Appendix A:

*(See Appendix A in the TM)*

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
State the installation steps you would take to put the ROLLER INDUCTORS — MOTORIZED VARIABLE INDUCTANCE in service on a 40m (7.150 MHz) station.
List steps in order.

**P4-2.** From Chapter 5: what installation or setup detail produces the best RF performance
with the ROLLER INDUCTORS — MOTORIZED VARIABLE INDUCTANCE? What is the tradeoff if you omit or shortcut that step?

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
