# Unit 4 — Operating Procedures and Calculations
**[TM-GEAR-001 — Open Handout](../../../antenna_gear/ant_switches/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Install and operate the ANTENNA SWITCHES — MANUAL, MOTORIZED, AND AUTO-BAND correctly; interpret performance data; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-GEAR-001. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

1. Manual switch: rotate selector to desired antenna port before transmitting.
 Never rotate the switch while transmitting — arcing at the contacts can
 weld them.
2. Auto-band switch: confirm CI-V address matches the transceiver's configured
 address (default 0x56 for IC-7300, 0x70 for IC-705). Monitor the CYD
 display to verify the switch follows band changes.
3. Remote switch: send band command via RS-485 or Wi-Fi API. Verify LED
 indicator (or CYD display) confirms correct port selected before transmitting.
4. Never transmit into an open switch port (no antenna connected) — reflected
 power will be 100%, potentially damaging the transmitter PA.

---

## Appendix A — Reference Formulas

| Band | Freq range (MHz) | Default port | Notes |
|---|---|---|---|
| 160M | 1.800–2.000 | 1 | Inverted-L or T-antenna |
| 80M | 3.500–4.000 | 2 | Dipole or vertical |
| 40M | 7.000–7.300 | 2 | Shared with 80M |
| 20M–10M | 14.000–29.700 | 3 | Yagi or beam |
| 6M–2M | 50–148 | 4 | VHF verticals |
| 70cm+ | 420–1300 | 5 | UHF verticals |

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
State the installation steps you would take to put the ANTENNA SWITCHES — MANUAL, MOTORIZED, AND AUTO-BAND in service on a 40m (7.150 MHz) station.
List steps in order.

**P4-2.** From Chapter 5: what installation or setup detail produces the best RF performance
with the ANTENNA SWITCHES — MANUAL, MOTORIZED, AND AUTO-BAND? What is the tradeoff if you omit or shortcut that step?

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
