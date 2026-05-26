# Unit 1 — Theory of Operation
**[TM-TOOL-005 — Open Handout](../../antenna_tools/field_strength_meter/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the FIELD STRENGTH METER — CONSTRUCTION AND USE; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-TOOL-005. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 Passive Detector

An RF signal intercepted by the probe antenna develops a voltage across
a germanium point-contact diode (1N34A). The diode rectifies the RF to
DC; a 10&micro;F capacitor integrates (smooths) the rectified DC; a
100&micro;A FSD panel meter displays the result. Sensitivity is limited by
the diode forward voltage (~0.2V for germanium vs. ~0.6V for silicon);
minimum detectable field from a 50 cm whip at 7 MHz is approximately
0.5 mV/m.

2-2 Active Version

A common-base RF amplifier (BF199 or 2N3904) precedes the detector
diode, providing 20–30 dB of gain before detection. An op-amp
(LM386 or similar) drives a meter with logarithmic compression for a
dB-proportional scale. Sensitivity improves to approximately 5 &micro;V/m.

2-3 Digital CYD FSM

An AD8307 logarithmic amplifier (0.1–500 MHz, &minus;74 to +17 dBm,
25 mV/dB) replaces the detector and meter. Its output drives an ESP32
ADC; the CYD display shows field strength in dBm (relative to 50Ω)
and logs readings with GPS timestamp. Absolute calibration ties the ADC
reading to a known field (calibrated reference transmitter at known distance).

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

**SC1-1.** In one sentence, state the operating principle of the FIELD STRENGTH METER — CONSTRUCTION AND USE as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of measurement error or uncertainty?

**SC1-3.** What key specification(s) (accuracy, range, frequency coverage) does the TM state?

**SC1-4.** What does Chapter 2 say the FIELD STRENGTH METER — CONSTRUCTION AND USE cannot do — what are its limitations?

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
- The operating principle of the FIELD STRENGTH METER — CONSTRUCTION AND USE
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
