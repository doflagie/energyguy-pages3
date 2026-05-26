# Unit 4 — Operating Procedures and Calculations
**[TM-GEAR-003 — Open Handout](../../antenna_gear/baluns-and-ununs/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Install and operate the BALUNS AND UN-UNS — CURRENT AND VOLTAGE TYPES correctly; interpret performance data; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-GEAR-003. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

1. Mount the balun at the antenna feedpoint — not at the radio end.
 Feedpoint mounting minimizes the length of balanced feedline and
 the associated common-mode current problem.
2. Weatherproof outdoor installations: coat all solder joints and exposed
 windings with two coats of polyurethane or Plasti-Dip. Wrap the
 core with self-amalgamating tape before mounting outdoors.
3. Check SWR after installation. If SWR is higher than expected, the most
 common causes are: wrong impedance ratio for the antenna type, poor solder
 joint on the balun output terminals, or water ingress.

---

## Appendix A — Reference Formulas

| Material | Best frequency range | Application |
|---|---|---|
| #31 ferrite | 1–300 MHz | Choke baluns (1:1); excellent CMC for all HF |
| #43 ferrite | 1–100 MHz | TLT baluns (4:1, 9:1); HF transformation |
| #61 ferrite | 10–200 MHz | 6M and VHF TLTs |
| #67 ferrite | 50–500 MHz | VHF/UHF choke baluns and TLTs |
| Air core | 50–1300 MHz | VHF/UHF choke (coax coil, no saturation risk) |

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
State the installation steps you would take to put the BALUNS AND UN-UNS — CURRENT AND VOLTAGE TYPES in service on a 40m (7.150 MHz) station.
List steps in order.

**P4-2.** From Chapter 5: what installation or setup detail produces the best RF performance
with the BALUNS AND UN-UNS — CURRENT AND VOLTAGE TYPES? What is the tradeoff if you omit or shortcut that step?

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
