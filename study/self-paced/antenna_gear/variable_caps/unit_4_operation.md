# Unit 4 — Operating Procedures and Calculations
**[TM-GEAR-019 — Open Handout](../../../antenna_gear/variable_caps/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Install and operate the VARIABLE CAPACITORS — AIR, MOTORIZED, AND STACK-ON FLAT-PLATE correctly; interpret performance data; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-GEAR-019. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

5-1 Motorized Air-Variable

1. On power-up, the controller moves to the stored last position (from NVS).
2. Enter the desired capacitance on the OLED display using the rotary encoder.
 The motor drives to the corresponding position (from the calibration table).
3. For magnetic loop antenna tuning: the ESP32 receives SWR data from the
 SWR bridge and automatically sweeps capacitance until SWR minimum is found.

5-2 Stack-On Capacitor

Stack-on units are passive; simply connect the plate-A lead to one circuit
node and the plate-B lead to the other. Add or remove units to increase
or decrease capacitance in approximately 100–200 pF steps.

---

## Appendix A — Reference Formulas

C = ε0 × εr × A / d

Example: 50 × 75 mm LDPE sheet (0.05 mm thick):
  A = 0.050 × 0.075 = 0.00375 m²
  d = 0.05 mm = 5×10&minus;5 m
  εr (LDPE) = 2.3
  C = 8.854e-12 × 2.3 × 0.00375 / 5e-5
  C = 8.854e-12 × 172.5 = 1527 pF ≈ 1.5 nF per unit

For 100 pF per unit: reduce area or increase dielectric thickness.

---

## Key Formulas Summary

Key mathematical relationships from Appendix A:

- `C = ε0 × εr × A / d`
- `A = 0.050 × 0.075 = 0.00375 m²`
- `d = 0.05 mm = 5×10&minus;5 m`
- `εr (LDPE) = 2.3`
- `C = 8.854e-12 × 2.3 × 0.00375 / 5e-5`
- `C = 8.854e-12 × 172.5 = 1527 pF ≈ 1.5 nF per unit`

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
State the installation steps you would take to put the VARIABLE CAPACITORS — AIR, MOTORIZED, AND STACK-ON FLAT-PLATE in service on a 40m (7.150 MHz) station.
List steps in order.

**P4-2.** From Chapter 5: what installation or setup detail produces the best RF performance
with the VARIABLE CAPACITORS — AIR, MOTORIZED, AND STACK-ON FLAT-PLATE? What is the tradeoff if you omit or shortcut that step?

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
