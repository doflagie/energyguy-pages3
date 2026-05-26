# Unit 1 — Theory of Operation
**[TM-TOOL-006 — Open Handout](../../../antenna_tools/noise_bridge/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the RF NOISE BRIDGE — IMPEDANCE MEASUREMENT; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-TOOL-006. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 Bridge Circuit

A standard 4-arm Wheatstone bridge topology adapted for RF. Two fixed
51Ω arms establish the bridge reference. One variable arm (R_cal + jX_cal)
is the calibrated reference. The unknown arm (Z_ant) is the antenna under test.
The null detector sees zero voltage when the bridge is balanced:

Z_ant = R_cal + jX_cal (at null)
A 1:1:1 trifilar-wound transformer (T1) on a BN-43-202 core drives the two
bridge legs from the noise source. Trifilar winding ensures 2-2 Noise Source

Two designs are provided:

- **NGZ (Zener):** 5.1V zener diode biased into avalanche
 breakdown produces wideband white noise. Output amplified by a MAR-6
 MMIC (+20 dB, DC–2 GHz) to approximately &minus;25 dBm
 into 50Ω.
- **NGT (Transistor):** Reverse-biased NPN transistor base-emitter
 junction noise; higher output level and more uniform spectral density
 than zener. Amplified by ERA-3SM MMIC.

2-3 Null Detection Modes

| Mode | Hardware | Sensitivity | Notes |
|---|---|---|---|
| A — External RX | Communications receiver/SDR | <&minus;60 dBm | Best; frequency-selective |
| B — Audio amplifier | ERA-3SM MMIC + headphones | &minus;50 dBm | Standalone; no RX needed |
| C — AD8307 | Log detector + CYD display | &minus;70 dBm | Digital null indication |

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

**SC1-1.** In one sentence, state the operating principle of the RF NOISE BRIDGE — IMPEDANCE MEASUREMENT as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of measurement error or uncertainty?

**SC1-3.** What key specification(s) (accuracy, range, frequency coverage) does the TM state?

**SC1-4.** What does Chapter 2 say the RF NOISE BRIDGE — IMPEDANCE MEASUREMENT cannot do — what are its limitations?

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
- The operating principle of the RF NOISE BRIDGE — IMPEDANCE MEASUREMENT
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
