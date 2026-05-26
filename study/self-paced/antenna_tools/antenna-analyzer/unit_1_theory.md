# Unit 1 — Theory of Operation
**[TM-TOOL-002 — Open Handout](../../../antenna_tools/antenna-analyzer/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the ANTENNA ANALYZER — VECTOR IMPEDANCE MEASUREMENT; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-TOOL-002. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 AD8302 Gain/Phase Detector

The AD8302 accepts two RF signals on INPA and INPB (50Ω each) and
produces two DC outputs:

- **VMAG:** Proportional to |V_A / V_B| in dB (30 mV/dB,
 range &minus;30 to 0 dBm per input).
- **VPHS:** Proportional to phase difference (10 mV/°,
 0° to 180° range).

INPA receives the reference (forward-coupled) signal; INPB receives the
reflected signal. The ratio VREFL/VFWD and the phase shift Δφ
together yield the complex reflection coefficient Γ:

Γ = |V_refl / V_fwd| &angle; Δφ
Z_ant = Z0 × (1 + Γ) / (1 &minus; Γ) (Z0 = 50 Ω)

2-2 Directional Coupler
A multiband RF coupler (TM-TOOL-002-SCH-004) separates the forward and
reflected waves on the transmission line to the DUT (antenna). Coupling
factor is &minus;20 dB on the HF section and &minus;20 dB on the
VHF/UHF section. Directivity must be ≥20 dB across the band to keep
reflected signal isolation adequate for accurate Γ measurement.

2-3 Frequency Sources

**AD9851 DDS** (HF, 0.1–30 MHz): 125 MHz reference
clock, 32-bit frequency word, phase noise &minus;100 dBc/Hz at 1 kHz offset.
Controlled via 3-wire SPI from ESP32. Sweep rate: ~1000 points/second.

**ADF4351 PLL** (VHF/UHF, 30 MHz–1.3 GHz): Fractional-N
synthesizer, 32.768 MHz TCXO reference, phase noise &minus;90 dBc/Hz
at 10 kHz. Integer-boundary spurs require spur-avoidance in sweep
software when crossing integer multiples of the PFD reference frequency.

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

**SC1-1.** In one sentence, state the operating principle of the ANTENNA ANALYZER — VECTOR IMPEDANCE MEASUREMENT as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of measurement error or uncertainty?

**SC1-3.** What key specification(s) (accuracy, range, frequency coverage) does the TM state?

**SC1-4.** What does Chapter 2 say the ANTENNA ANALYZER — VECTOR IMPEDANCE MEASUREMENT cannot do — what are its limitations?

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
- The operating principle of the ANTENNA ANALYZER — VECTOR IMPEDANCE MEASUREMENT
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
