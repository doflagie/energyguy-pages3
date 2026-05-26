# Unit 4 — Operating Procedures and Calculations
**[TM-GEAR-005 — Open Handout](../../../antenna_gear/coax_traps/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Install and operate the COAXIAL CABLE TRAPS — MULTIBAND ANTENNA ISOLATION correctly; interpret performance data; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-GEAR-005. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

1. Measure the trap resonant frequency using the dip method: couple an
 RF signal source (TinySA or signal generator) to the trap through a
 1-turn coupling loop; sweep the frequency while monitoring the S11 dip
 on the NanoVNA. The resonant frequency is the dip minimum.
2. If the measured frequency is too low: remove one turn (the trap is too
 long). If too high: add a turn or increase the coil pitch (spread turns
 slightly to reduce capacitance).
3. Target: f_trap within ±100 kHz of the desired band center frequency.
4. After tuning, secure turns with a bead of weatherproof silicone or
 UV-resistant cable tie at each turn.

---

## Appendix A — Reference Formulas

40M trap, RG-8X on 76 mm former, 7 turns:

Coil radius r = 76mm / 2 = 38mm = 1.496 inches
Coil length l ≈ 7 × 6.15mm (wire OD) = 43mm = 1.693 inches
L = r² × N² / (9r + 10l) = 1.496² × 49 / (9 × 1.496 + 10 × 1.693)
  = 2.238 × 49 / (13.464 + 16.93) = 109.7 / 30.39 = 3.61 μH

Length of 7 turns on 76mm former: 7 × π × 76mm / 1000 = 1.672 m RG-8X
C = 1.672 m × 78.7 pF/m = 131.6 pF

f0 = 1 / (2π × sqrt(3.61e-6 × 131.6e-12))
   = 1 / (2π × sqrt(4.75e-16))
   = 1 / (2π × 2.179e-8)
   = 7.30 MHz   (close to 40M; adjust turns to fine-tune)

---

## Key Formulas Summary

Key mathematical relationships from Appendix A:

- `Coil radius r = 76mm / 2 = 38mm = 1.496 inches`
- `Coil length l ≈ 7 × 6.15mm (wire OD) = 43mm = 1.693 inches`
- `L = r² × N² / (9r + 10l) = 1.496² × 49 / (9 × 1.496 + 10 × 1.693)`
- `= 2.238 × 49 / (13.464 + 16.93) = 109.7 / 30.39 = 3.61 μH`
- `Length of 7 turns on 76mm former: 7 × π × 76mm / 1000 = 1.672 m RG-8X`
- `C = 1.672 m × 78.7 pF/m = 131.6 pF`

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
State the installation steps you would take to put the COAXIAL CABLE TRAPS — MULTIBAND ANTENNA ISOLATION in service on a 40m (7.150 MHz) station.
List steps in order.

**P4-2.** From Chapter 5: what installation or setup detail produces the best RF performance
with the COAXIAL CABLE TRAPS — MULTIBAND ANTENNA ISOLATION? What is the tradeoff if you omit or shortcut that step?

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
