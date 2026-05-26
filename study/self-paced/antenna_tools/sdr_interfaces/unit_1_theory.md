# Unit 1 — Theory of Operation
**[TM-TOOL-009 — Open Handout](../../../antenna_tools/sdr_interfaces/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the SDR INTERFACES AND PRESELECTOR; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-TOOL-009. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 HF Upconverter

An SA612A double-balanced mixer converts HF input (0.1–30 MHz)
to IF (125.1–155 MHz) by mixing with a 125.000 MHz TCXO local
oscillator. The RTL-SDR is then tuned to 125–155 MHz to receive
HF signals without needing direct-sampling mode. An SPDT relay bypasses
the upconverter for 6m and above (where RTL-SDR tunes directly).
Optional SPF5189Z LNA (+19 dB, NF = 0.6 dB) precedes the mixer.

| Parameter | Value |
|---|---|
| Input frequency | 0.1–30 MHz |
| LO frequency | 125.000 MHz (±2.5 ppm TCXO) |
| IF output | 125.1–155 MHz |
| Conversion gain (passive) | &minus;2 dB |
| Conversion gain (with LNA) | +17 dB |
| Image frequency | >155 MHz (suppressed >45 dB by IF BPF) |
| LO leakage to RF port | 2-2 16-Band Switchable Preselector

Sixteen bandpass filters (one per amateur band, 160m through 23cm plus
one bypass) select with an SP4T×4 relay tree driven by a 4-to-16 decoder
(74HC154) from ESP32 GPIO[3:0]. Filter topology: 3-pole Chebyshev BPF
for 160m–10m; interdigital for 2m/1.25m; stripline/SAW for 70cm and
above. Rejection of out-of-band signals: ≥50 dB adjacent band,
≥60 dB for FM broadcast (88–108 MHz) when tuned to HF.

2-3 Bias-T

A bias-T injects DC power onto coax to supply a remote LNA or active
antenna. An RF choke (L, >10&micro;H SRF) passes DC while blocking RF;
a DC-blocking capacitor (C) passes RF while blocking DC. The HF version
(Version A) covers 0.1–50 MHz; the VHF/UHF version (Version B)
covers 10 MHz–3 GHz.

2-4 PE4302 Digital Step Attenuator

The PE4302 provides 0–31.5 dB attenuation in 0.5 dB steps
via 6-bit SPI control. Frequency range DC–3 GHz, IP3 = +40 dBm,
NF = attenuation setting + 0.5 dB. Combined with an SPF5189Z LNA
(+19 dB gain, NF = 0.6 dB), the combination provides adjustable
gain from &minus;31.5 dB (full atten) to +18.5 dB (no atten, LNA on).

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

**SC1-1.** In one sentence, state the operating principle of the SDR INTERFACES AND PRESELECTOR as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of measurement error or uncertainty?

**SC1-3.** What key specification(s) (accuracy, range, frequency coverage) does the TM state?

**SC1-4.** What does Chapter 2 say the SDR INTERFACES AND PRESELECTOR cannot do — what are its limitations?

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
- The operating principle of the SDR INTERFACES AND PRESELECTOR
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
