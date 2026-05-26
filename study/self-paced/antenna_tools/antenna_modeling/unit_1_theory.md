# Unit 1 — Theory of Operation
**[TM-TOOL-003 — Open Handout](../../../antenna_tools/antenna_modeling/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the NEC ANTENNA MODELING SYSTEM; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-TOOL-003. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 NEC Card Format Summary

NEC input is a plain-text deck of two-character card mnemonics, one per line:

| Card | Purpose | Key fields |
|---|---|---|
| CM | Comment | Free text |
| CE | End comments | (no fields) |
| GW | Wire segment | tag, segs, x1 y1 z1, x2 y2 z2, radius |
| GS | Scale geometry | scale factor (e.g., 0.3048 = feet to meters) |
| GE | End geometry | 0=free space, 1=ground present |
| GN | Ground parameters | type, εr, σ (good earth: 13, 0.005) |
| EX | Excitation (source) | tag, segment, V_real, V_imag |
| FR | Frequency | start MHz, step MHz, N steps |
| RP | Radiation pattern | theta/phi start, step, N points, mode |
| EN | End of input | (last card) |

2-2 Segment Count Guidelines

NEC accuracy depends on adequate segmentation. Rule: 10–20 segments
per wavelength, odd count preferred for symmetric elements with a center source.
Minimum 3 segments per wire. The system function:

def recommended_segments(length_m, freq_mhz, min_segs=3):
    wavelength = 299.792458e6 / (freq_mhz * 1e6)
    segs = max(min_segs, int(length_m / wavelength * 10))
    return segs if segs % 2 == 1 else segs + 1

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

**SC1-1.** In one sentence, state the operating principle of the NEC ANTENNA MODELING SYSTEM as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of measurement error or uncertainty?

**SC1-3.** What key specification(s) (accuracy, range, frequency coverage) does the TM state?

**SC1-4.** What does Chapter 2 say the NEC ANTENNA MODELING SYSTEM cannot do — what are its limitations?

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
- The operating principle of the NEC ANTENNA MODELING SYSTEM
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
