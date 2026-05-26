# Unit 1 — Theory of Operation
**[TM-GEAR-019 — Open Handout](../../antenna_gear/variable_caps/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the VARIABLE CAPACITORS — AIR, MOTORIZED, AND STACK-ON FLAT-PLATE; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-019. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 Air-Variable Capacitor

A parallel-plate capacitor with interleaved rotor and stator plates (no
dielectric between the plates). Capacitance is proportional to the overlap
area between rotor and stator plates: C = ε0 × A / d,
where A is the overlap area and d is the plate spacing (typically 1.5–3 mm
for HF/VHF use). As the rotor turns, the overlap area changes continuously
from zero (minimum capacitance, plates disengaged) to maximum (full overlap).

2-2 Voltage Breakdown

Air breakdown occurs at approximately 30 kV/cm (3 MV/m). For a
3 mm plate spacing: V_breakdown = 3000V × 0.3 cm = 900V peak.
At 100W into 50Ω: V_peak = &radic;(2 × 100 × 50) = 100V peak —
well within rating. In a transmatch at high transformation ratio, voltages
across the capacitors can reach 1–3 kV. Use the capacitor voltage
rating appropriate for the transmatch power and impedance.

2-3 Homebrew Stack-On Flat-Plate Capacitor

Copper foil plates separated by a thin polymer dielectric (polyimide, LDPE,
or polypropylene film). Stacking multiple units in parallel increases total
capacitance. Stacking in series reduces total capacitance and increases
voltage rating. Formula:

C = ε0 × εr × A / d    [Farads]
  ε0 = 8.854×10&minus;12 F/m
  εr = relative permittivity (LDPE: 2.3; polypropylene: 2.2; polyimide: 3.5)
  A = plate area (m²); d = dielectric thickness (m)

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

**SC1-1.** In one sentence, state the operating principle of the VARIABLE CAPACITORS — AIR, MOTORIZED, AND STACK-ON FLAT-PLATE as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of loss or degradation in performance?

**SC1-3.** What key specification(s) (frequency range, power rating, insertion loss, impedance ratio) does the TM state?

**SC1-4.** What does Chapter 2 say the VARIABLE CAPACITORS — AIR, MOTORIZED, AND STACK-ON FLAT-PLATE cannot do — what are its limitations?

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
- The operating principle of the VARIABLE CAPACITORS — AIR, MOTORIZED, AND STACK-ON FLAT-PLATE
- The primary loss or degradation source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
