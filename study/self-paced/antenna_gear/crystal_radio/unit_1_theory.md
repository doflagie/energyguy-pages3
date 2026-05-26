# Unit 1 — Theory of Operation
**[TM-GEAR-008 — Open Handout](../../../antenna_gear/crystal_radio/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the CRYSTAL RADIO — AM BROADCAST AND SHORTWAVE; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-008. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 Tuned Circuit

A parallel LC tank circuit resonates at the desired station frequency.
At resonance, the circuit impedance is maximum (Z = L/(R×C), where R
is coil resistance); off-resonant signals see low impedance and are bypassed.
Selectivity is determined by Q = ωL/R; higher Q means sharper tuning
but also higher loss. For AM broadcast: L ≈ 250 &micro;H,
C = 15–365 pF (variable).

2-2 Detector Diode

The detector diode (galena crystal, 1N34A germanium, or BAT42 Schottky)
rectifies the AM carrier, demodulating the audio. A bypass capacitor
(100 pF to 1 &micro;F depending on audio frequency range)
filters the RF carrier, leaving the audio envelope. Germanium diodes
have lower forward voltage (~0.2V) than silicon; they work with weaker
signals. Galena (natural PbS crystal) historically achieved the highest
sensitivity when point-contacting the sweet spot on the crystal.

2-3 Antenna Coupling

The antenna couples RF energy into the tuned circuit. Over-coupling
loads the tank circuit, reducing Q and selectivity. Under-coupling
reduces signal level. Optimal coupling: the antenna resonant impedance
is tapped partway down the coil (inductive coupling) to match the low
antenna impedance (~100–300Ω) to the high tank circuit
impedance (~50–200 kΩ).

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

**SC1-1.** In one sentence, state the operating principle of the CRYSTAL RADIO — AM BROADCAST AND SHORTWAVE as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of loss or degradation in performance?

**SC1-3.** What key specification(s) (frequency range, power rating, insertion loss, impedance ratio) does the TM state?

**SC1-4.** What does Chapter 2 say the CRYSTAL RADIO — AM BROADCAST AND SHORTWAVE cannot do — what are its limitations?

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
- The operating principle of the CRYSTAL RADIO — AM BROADCAST AND SHORTWAVE
- The primary loss or degradation source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
