# Unit 1 — Theory of Operation
**[TM-TOOL-004 — Open Handout](../../../antenna_tools/dummy_loads/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the RF DUMMY LOADS — CONSTRUCTION AND USE; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-TOOL-004. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 Resistor Network Fundamentals

Non-inductive resistors in parallel sets combine power ratings and
maintain the 50Ω impedance. For N identical resistors each of value
R in parallel: Z = R / N. Use:

- 100W load: 4 × 200Ω 25W resistors in parallel = 50Ω, 100W
- 500W load: 4 × 200Ω 125W in parallel = 50Ω, 500W
- 1 kW load: 8 × 400Ω 125W in parallel = 50Ω, 1000W

2-2 Parasitic Inductance and Frequency Limit

Each resistor lead has approximately 5–10 nH of lead inductance.
At 150 MHz: X_L = 2π × 150e6 × 8e-9 = 7.5Ω, causing SWR to rise
above 1.5:1. Minimizing lead length to ≤10 mm each side and using
star wiring (all high ends bonded to a single center stud) reduces effective
inductance to 2-3 Oil Cooling

Transformer oil (mineral oil) provides thermal conductivity of
0.135 W/m·K, approximately 10× better than still air. Resistors
submerged in oil can dissipate their full rated power continuously at
25°C ambient. Seal the container; oil expands approximately 7% from
20°C to 100°C.

---

## Why Theory Matters

You cannot use a measurement tool correctly without understanding how it works. Theory tells you:
- What the tool measures and how it converts the quantity to a readable output
- What the sources of error are — so you can recognize and minimize them
- What the valid operating range is — so you stay within its specifications
- How to interpret results that don't match expectations

If a measurement looks wrong, theory is where you look first.

---

## Self-Check Questions

**SC1-1.** In one sentence, state the operating principle of the RF DUMMY LOADS — CONSTRUCTION AND USE as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of measurement error or uncertainty?

**SC1-3.** What key specification(s) (accuracy, range, frequency coverage) does the TM state?

**SC1-4.** What does Chapter 2 say the RF DUMMY LOADS — CONSTRUCTION AND USE cannot do — what are its limitations?

**SC1-5.** List two formulas or relationships from Chapter 2 that govern the tool's operation.

---

## Answer Key

**SC1-1.** See TM §2-1. Compare your sentence to the first substantive paragraph of Chapter 2.

**SC1-2.** See Chapter 2. Look for language about error sources, accuracy limits, parasitic effects, or frequency dependence.

**SC1-3.** See Chapter 2. Look for numbers with units: %, ppm, Hz, Ω, dB, W.

**SC1-4.** See Chapter 2 and Chapter 1. Limitations are often stated as frequency range, power limits, or accuracy bounds.

**SC1-5.** See Chapter 2. Equations or proportionality statements are the relationships that govern the tool.

---

## Checkpoint

Before proceeding, state without looking:
- The operating principle of the RF DUMMY LOADS — CONSTRUCTION AND USE
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
