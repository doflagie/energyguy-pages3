# Unit 1 — Theory of Operation
**[TM-TOOL-010 — Open Handout](../../../antenna_tools/swr_meter/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the SWR METER — CONSTRUCTION AND USE; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-TOOL-010. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 Tandem-Match Directional Coupler (HF)

Two toroids wound on BN-43-202 or FT-50-43 cores provide &minus;30 dB
coupling. The forward arm measures V_fwd (proportional to forward wave
voltage); the reflected arm measures V_ref (proportional to reflected wave).
Termination resistors (51Ω 1%) on each secondary provide the
directionality; directivity is typically >35 dB across 1.8–30 MHz.

2-2 Diode Detector Circuits

1N5711 Schottky diodes detect the coupled RF. In the square-law region
(V_in 100 mV): V_dc &prop; V_rf. The meter driver compensates
for detector law by adjusting the meter scale or applying a lookup table
in firmware.

2-3 SWR Calculation

SWR = (V_fwd + V_ref) / (V_fwd &minus; V_ref) [analog cross-needle meter]
SWR = (1 + sqrt(P_ref/P_fwd)) / (1 &minus; sqrt(P_ref/P_fwd)) [digital meter]
2-4 Range Switching
Resistive dividers on the detector outputs set the effective full-scale
power range. Switching from 100W to 10W range adds 10× attenuation
before the panel meter, maintaining accuracy across the power range. Six
ranges: 5W, 20W, 50W, 200W, 500W, 1500W (switchable by front-panel rotary).

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

**SC1-1.** In one sentence, state the operating principle of the SWR METER — CONSTRUCTION AND USE as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of measurement error or uncertainty?

**SC1-3.** What key specification(s) (accuracy, range, frequency coverage) does the TM state?

**SC1-4.** What does Chapter 2 say the SWR METER — CONSTRUCTION AND USE cannot do — what are its limitations?

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
- The operating principle of the SWR METER — CONSTRUCTION AND USE
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
