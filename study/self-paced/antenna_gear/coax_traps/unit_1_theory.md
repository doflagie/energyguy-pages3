# Unit 1 — Theory of Operation
**[TM-GEAR-005 — Open Handout](../../../antenna_gear/coax_traps/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the COAXIAL CABLE TRAPS — MULTIBAND ANTENNA ISOLATION; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-005. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 Resonant Circuit

The coil inductance follows Wheeler's formula for a short single-layer solenoid:

L (&micro;H) = r² × N² / (9r + 10l)
 r = coil radius (inches), N = turns, l = coil length (inches)
The distributed capacitance of coaxial cable is approximately:

| Coax type | Capacitance (pF/m) | OD (mm) |
|---|---|---|
| RG-58/U | 93.5 | 4.95 |
| RG-8X | 78.7 | 6.15 |
| RG-213/U | 96.8 | 10.29 |

Resonant frequency:

f0 = 1 / (2π × sqrt(L × C))
Q factor: typically 100–200 for coax traps. Higher Q means sharper
isolation (narrower stop band) and lower insertion loss in the pass band.

2-2 Effect on Antenna

Below the trap resonant frequency, the trap appears as a short inductor
(low impedance) — the full antenna length is active. At the trap resonant
frequency, high trap impedance isolates the antenna element beyond the trap.
Above the resonant frequency, the trap appears capacitive and can shorten
the resonant length of the inner portion of the antenna.

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

**SC1-1.** In one sentence, state the operating principle of the COAXIAL CABLE TRAPS — MULTIBAND ANTENNA ISOLATION as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of loss or degradation in performance?

**SC1-3.** What key specification(s) (frequency range, power rating, insertion loss, impedance ratio) does the TM state?

**SC1-4.** What does Chapter 2 say the COAXIAL CABLE TRAPS — MULTIBAND ANTENNA ISOLATION cannot do — what are its limitations?

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
- The operating principle of the COAXIAL CABLE TRAPS — MULTIBAND ANTENNA ISOLATION
- The primary loss or degradation source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
