# Unit 1 — Theory of Operation
**[TM-GEAR-017 — Open Handout](../../../antenna_gear/roller_inductors/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the ROLLER INDUCTORS — MOTORIZED VARIABLE INDUCTANCE; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-017. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 Variable Inductance

A roller inductor consists of a large single-layer air-core coil wound
on a threaded ceramic or PTFE former. A sliding contact (the &ldquo;roller&rdquo;)
driven along the former by a threaded rod taps a variable number of turns.
Inductance is approximately proportional to turns squared (Wheeler&rsquo;s formula)
for a fixed coil geometry:

L = r² × N² / (9r + 10l) (L in &micro;H, r and l in inches)
For a fixed coil with N_total turns:
 L(n) ≈ L_max × (n / N_total)²
A 25 &micro;H roller inductor with 40 turns has L = 0 at n=0, rising
to 25 &micro;H at n=40. The relationship is not exactly quadratic
due to coil end effects, so a calibration table maps encoder position to
actual inductance (measured with an LCR meter).

2-2 Stepper Motor Positioning

A NEMA 17 stepper with 200 steps/rev, 1/16 microstepping = 3200 steps/rev.
The lead screw pitch (typically 1.25 mm/rev for M8) gives a linear
resolution of 1.25 / 3200 = 0.391 &micro;m per step — far more than
needed. Position is tracked by step count from the home position (home limit
switch). On power-up, the controller homes before accepting position commands.

---

## Why Theory Matters

You cannot build or use RF gear correctly without understanding how it works. Theory tells you:
- What the component does and how it produces that effect
- What the sources of loss, distortion, or error are — so you can recognize and minimize them
- What the valid operating range is — frequency, power, impedance — so you stay within specifications
- How to interpret results or system behavior that doesn't match expectations

If a component doesn't perform as expected, theory is where you look first.

---

## Self-Check Questions

**SC1-1.** In one sentence, state the operating principle of the ROLLER INDUCTORS — MOTORIZED VARIABLE INDUCTANCE as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of loss or degradation in performance?

**SC1-3.** What key specification(s) (frequency range, power rating, insertion loss, impedance ratio) does the TM state?

**SC1-4.** What does Chapter 2 say the ROLLER INDUCTORS — MOTORIZED VARIABLE INDUCTANCE cannot do — what are its limitations?

**SC1-5.** List two formulas or relationships from Chapter 2 that govern the component's behavior.

---

## Answer Key

**SC1-1.** See TM §2-1. Compare your sentence to the first substantive paragraph of Chapter 2.

**SC1-2.** See Chapter 2. Look for language about loss mechanisms, parasitic effects, frequency limits, or power constraints.

**SC1-3.** See Chapter 2. Look for numbers with units: %, dB, Hz, Ω, W, V.

**SC1-4.** See Chapter 2 and Chapter 1. Limitations are often stated as frequency range, power handling, or impedance range.

**SC1-5.** See Chapter 2. Equations or proportionality statements are the relationships that govern the component.

---

## Checkpoint

Before proceeding, state without looking:
- The operating principle of the ROLLER INDUCTORS — MOTORIZED VARIABLE INDUCTANCE
- The primary loss or degradation source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
