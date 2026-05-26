# Unit 1 — Theory of Operation
**[TM-TOOL-007 — Open Handout](../../../antenna_tools/rf_power_meter/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the COMPACT PORTABLE RF POWER METER; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-TOOL-007. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 Directional Coupler Principle

A toroid transformer (T1) with 1 primary turn (the center conductor of
the transmission line) and N secondary turns couples a fraction of the
forward and reflected waves. The coupling factor C (dB) = &minus;20 log10(N)
for a toroid coupler. Typical values:

| Power range | N2 turns | Coupling | Detector input (at full scale) |
|---|---|---|---|
| 1000W | 32 | &minus;30 dB | 1.0W = 0 dBm |
| 100W | 10 | &minus;20 dB | 100W → 1 mW = 0 dBm |
| 10W | 10 | &minus;20 dB | 10W → 0.1 mW |
| 1W | 3 | &minus;10 dB | 1W → 0.1W |

2-2 Schottky Diode Detector

1N5711 Schottky diodes detect the coupled RF. Forward voltage ~0.25V
at low signal levels (square-law region: V_out &prop; P_in) and ~0.4V at
high levels (linear region: V_out &prop; V_in). The ADS1115 16-bit ADC
samples V_fwd and V_ref; firmware converts to power using a
calibration table stored in NVS.

2-3 SWR Computation
|Γ| = sqrt(P_ref / P_fwd)
SWR = (1 + |Γ|) / (1 &minus; |Γ|)

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

**SC1-1.** In one sentence, state the operating principle of the COMPACT PORTABLE RF POWER METER as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of measurement error or uncertainty?

**SC1-3.** What key specification(s) (accuracy, range, frequency coverage) does the TM state?

**SC1-4.** What does Chapter 2 say the COMPACT PORTABLE RF POWER METER cannot do — what are its limitations?

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
- The operating principle of the COMPACT PORTABLE RF POWER METER
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
