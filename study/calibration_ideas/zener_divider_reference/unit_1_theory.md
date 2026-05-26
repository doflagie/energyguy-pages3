# Unit 1 — Theory of Operation
**[TM-CAL-007 — Open Handout](../../Calibration_Ideas/zener_divider_reference/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand how the reference standard generates or maintains its reference value; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-CAL-007. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. TEMPERATURE-COMPENSATED ZENER

Zener diodes exhibit a negative temperature coefficient (TC) below approximately 5.6 V and a positive TC above 5.6 V. At ~5.6 V zener voltage, TC ≈ 0. The 1N829 and LM336 are designed to operate at this compensation point.

A forward-biased diode (TC ≈ &minus;2 mV/°C) in series with a 6.2 V zener (TC ≈ +2 mV/°C) achieves near-zero overall TC. The LM336 IC integrates this compensation internally.

| Device | Voltage | TC | Accuracy | Notes |
|---|---|---|---|---|
| LM336-2.5 | 2.5 V | <20 ppm/°C | ±1% | Adjustable with ext R |
| LM385-2.5 | 2.5 V | <50 ppm/°C | ±1% | Micropower |
| 1N829 | 6.2 V | <10 ppm/°C | ±0.5% | Hermetic zener |
| 1N5234 | 6.2 V | <100 ppm/°C | ±5% | General purpose |

### 2-2. RESISTOR DIVIDER

A precision resistor divider derives lower voltages from the master reference. Use 0.1% tolerance resistors for <0.2% division error.

---

## Why Theory Matters for Calibration

You cannot calibrate what you do not understand. Theory tells you:
- What the instrument or standard is supposed to do — so you recognize when it is not doing it
- What the sources of error are — so you know which ones your calibration procedure addresses
- What the limits of the calibration are — so you know when to stop and call it good

If a calibration measurement produces a surprising result, theory is where you look first.

---

## Self-Check Questions

**SC1-1.** In one sentence, state the operating principle of the ZENER + DIVIDER MULTI-VOLTAGE REFERENCE as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of measurement error or uncertainty?

**SC1-3.** What key specification(s) (accuracy, range, resolution) does the TM state for this standard?

**SC1-4.** What safety precaution does §1-3 specify? State it exactly.

**SC1-5.** What references does §1-2 cite? List at least two.

---

## Answer Key

**SC1-1.** See TM §2-1. Compare your sentence to the first substantive paragraph of Chapter 2.

**SC1-2.** See Chapter 2. Look for language about error sources, uncertainty, drift, or limiting factors.

**SC1-3.** See Chapter 2. Look for numbers with units: %, ppm, Hz, Ω, dB.

**SC1-4.** See TM §1-3. Copy the safety text exactly.

**SC1-5.** See TM §1-2. The references list is there.

---

## Checkpoint

Before proceeding, state without looking:
- The operating principle of the ZENER + DIVIDER MULTI-VOLTAGE REFERENCE
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_equipment.md)**
