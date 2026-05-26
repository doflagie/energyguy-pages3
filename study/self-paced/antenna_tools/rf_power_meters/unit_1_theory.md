# Unit 1 — Theory of Operation
**[TM-TOOL-008 — Open Handout](../../../antenna_tools/rf_power_meters/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the RF POWER METERS — TYPES AND OPERATION; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-TOOL-008. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 Bird Model 43 Slug Type

The Bird 43 is an inline directional wattmeter using interchangeable
plug-in elements (slugs). Each slug contains a toroid directional coupler
wound for a specific frequency range and power range. The slug's flat face
orientation determines forward vs. reflected reading: flat toward the meter
face = forward power; flat away = reflected. Output is a DC voltage
proportional to forward (or reflected) power, driving an analog panel meter.

Slug PCB dimensions: 25.0 mm OD × 25.4 mm length. Retention:
O-ring at body midpoint. Contact: 2 mm banana pin. The flatted-D
cross-section prevents 90° orientation errors.

2-2 Inline Directional Meter

Uses a toroid coupler (1-turn primary = center conductor; N-turn secondary)
plus Schottky diode detectors (1N5711) for forward and reflected channels.
An ADS1115 16-bit I2C ADC digitizes both channels; ESP32 computes forward
power, reflected power, and SWR in firmware. CYD display shows real-time
readings. Typical frequency range 1.8–600 MHz with appropriate
coupler design.

2-3 Terminating Wattmeter

A terminating wattmeter absorbs all transmitted power in an internal
50Ω load (dry or oil-cooled) and measures the absorbed power via
a coupler before the load. Because there is no transmitted power beyond
the instrument, no antenna is needed during testing. Used for final
amplifier output power verification and transmitter tuning.

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

**SC1-1.** In one sentence, state the operating principle of the RF POWER METERS — TYPES AND OPERATION as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of measurement error or uncertainty?

**SC1-3.** What key specification(s) (accuracy, range, frequency coverage) does the TM state?

**SC1-4.** What does Chapter 2 say the RF POWER METERS — TYPES AND OPERATION cannot do — what are its limitations?

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
- The operating principle of the RF POWER METERS — TYPES AND OPERATION
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
