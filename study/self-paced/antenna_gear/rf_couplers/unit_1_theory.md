# Unit 1 — Theory of Operation
**[TM-GEAR-015 — Open Handout](../../../antenna_gear/rf_couplers/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the RF DIRECTIONAL COUPLERS — CONSTRUCTION AND CALIBRATION; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-015. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 Bruene Toroidal Coupler

A ferrite toroid transformer (T1) samples the current in the transmission
line (1-turn primary = center conductor). A separate voltage sampling
network monitors the line voltage. Combining current and voltage in the
correct phase relationship produces the forward and reflected wave voltages
separately. Directivity is the ratio of the response to the desired
direction vs. the undesired: ≥40 dB with high-precision resistors.

2-2 Transmission-Line Coupler

Two parallel transmission lines run close together for λ/4 at
the design frequency. Electromagnetic coupling transfers a small fraction
of power to the secondary line. Coupling factor C (&minus;dB) depends on
line spacing and length: C = &minus;20 log10(k) where k is the voltage
coupling coefficient. Typical: &minus;20 to &minus;30 dB. Used in
the SWR meter and RF power meter designs.

2-3 Resistive Bridge Coupler

A 4-resistor Wheatstone bridge measures the complex reflection coefficient
directly. One arm is the antenna under test; the opposite arm is the
reference (50Ω). At balance (antenna = 50Ω), the bridge output
is zero. Imbalance is proportional to (Z_ant &minus; 50) / (Z_ant + 50) = Γ.
Limited to low power (<1W); resistors must be non-inductive and matched.

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

**SC1-1.** In one sentence, state the operating principle of the RF DIRECTIONAL COUPLERS — CONSTRUCTION AND CALIBRATION as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of loss or degradation in performance?

**SC1-3.** What key specification(s) (frequency range, power rating, insertion loss, impedance ratio) does the TM state?

**SC1-4.** What does Chapter 2 say the RF DIRECTIONAL COUPLERS — CONSTRUCTION AND CALIBRATION cannot do — what are its limitations?

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
- The operating principle of the RF DIRECTIONAL COUPLERS — CONSTRUCTION AND CALIBRATION
- The primary loss or degradation source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
