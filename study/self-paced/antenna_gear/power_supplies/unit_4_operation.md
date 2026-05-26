# Unit 4 — Operating Procedures and Calculations
**[TM-GEAR-013 — Open Handout](../../../antenna_gear/power_supplies/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Install and operate the POWER SUPPLIES — LINEAR, SWITCHING, AND FIELD PORTABLE correctly; interpret performance data; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-GEAR-013. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

1. Linear supply: allow 5 minutes warm-up before setting critical output
 voltage. Adjust the voltage trimmer to 13.8V (verified with DMM).
 Set current limit to 5A above the transceiver&rsquo;s peak draw.
2. SMPS: verify output voltage before connecting transceiver.
 Do not exceed rated current; SMPS enter fold-back current limiting and
 may drop voltage abruptly under severe overload.
3. LiFePO4 portable: check pack voltage before operation. Below 12.4V
 (SOC ~20%), plan for recharging soon. Do not discharge below 10.0V.

---

## Appendix A — Reference Formulas

For a linear supply, the voltage drop across series wiring:
  V_drop = I × R_wire
  For #14 AWG, R = 8.4 mΩ/ft (2.76 mΩ/m)
  At 30A over 3m of #14 AWG (6m round trip):
  V_drop = 30 × 0.00276 × 6 = 0.50V
  This means the bench at the end of 3m of #14 AWG gets 13.8 − 0.5 = 13.3V.
  Use #10 AWG for long runs or supply >20A.

---

## Key Formulas Summary

Key mathematical relationships from Appendix A:

- `V_drop = I × R_wire`
- `For #14 AWG, R = 8.4 mΩ/ft (2.76 mΩ/m)`
- `V_drop = 30 × 0.00276 × 6 = 0.50V`
- `This means the bench at the end of 3m of #14 AWG gets 13.8 − 0.5 = 13.3V.`

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
State the installation steps you would take to put the POWER SUPPLIES — LINEAR, SWITCHING, AND FIELD PORTABLE in service on a 40m (7.150 MHz) station.
List steps in order.

**P4-2.** From Chapter 5: what installation or setup detail produces the best RF performance
with the POWER SUPPLIES — LINEAR, SWITCHING, AND FIELD PORTABLE? What is the tradeoff if you omit or shortcut that step?

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
