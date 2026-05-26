# Unit 1 — Theory of Operation
**[TM-GEAR-012 — Open Handout](../../antenna_gear/motorized_tuner/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the MOTORIZED ANTENNA TUNER — HF AND VHF/UHF; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-012. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 T-Network HF Tuner

The balanced T-network (input cap C1, series inductor L, output cap C2)
can match a wide range of impedances to 50Ω. Typical range:
5–2000Ω, with moderate losses at high transformation ratios.
L is a roller inductor (variable inductance by tapping along the coil);
C1 and C2 are motorized air variables.

2-2 VHF Stub Tuner

A short-circuit stub (adjustable-length transmission line) in shunt with
the antenna feedline cancels the reactive component of the antenna impedance.
The stub length is adjusted by a motorized sliding short-circuit piston.
A quarter-wave transformer section then converts the resistive remainder
to 50Ω. Effective for narrowband VHF/UHF matching.

2-3 SWR-Guided Auto-Tune Algorithm

The ESP32 reads the SWR bridge ADC outputs (V_fwd, V_ref) and minimizes
the SWR by a coordinate descent algorithm: adjust L in one direction;
if SWR decreases, continue; if it increases, reverse. Then adjust C1,
then C2. Iterate until SWR <1.5:1 or no further improvement.

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

**SC1-1.** In one sentence, state the operating principle of the MOTORIZED ANTENNA TUNER — HF AND VHF/UHF as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of loss or degradation in performance?

**SC1-3.** What key specification(s) (frequency range, power rating, insertion loss, impedance ratio) does the TM state?

**SC1-4.** What does Chapter 2 say the MOTORIZED ANTENNA TUNER — HF AND VHF/UHF cannot do — what are its limitations?

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
- The operating principle of the MOTORIZED ANTENNA TUNER — HF AND VHF/UHF
- The primary loss or degradation source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
