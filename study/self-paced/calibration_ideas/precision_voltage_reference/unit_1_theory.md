# Unit 1 — Theory of Operation
**[TM-CAL-006 — Open Handout](../../../Calibration_Ideas/precision_voltage_reference/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand how the reference standard generates or maintains its reference value; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-CAL-006. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. BAND-GAP REFERENCE PRINCIPLE

Band-gap voltage references exploit the temperature-stable forward voltage of silicon junctions. A PTAT (proportional-to-absolute-temperature) current is summed with a complementary CTAT current; when properly ratioed, the sum equals the silicon band-gap extrapolated voltage of approximately 1.205 V. This voltage is independent of temperature to first order.

Precision reference ICs (LM4040, LT1021, LM399, LTZ1000) implement this principle with additional trimming and, in high-precision devices, temperature-controlled substrates.

| IC | Voltage | Initial Acc. | Temp. Coeff. | Notes |
|---|---|---|---|---|
| LM4040A-5.0 | 5.000 V | ±0.1% | 100 ppm/°C | Shunt reg, low cost |
| LM4040B-5.0 | 5.000 V | ±0.2% | 100 ppm/°C | Economy grade |
| LT1021-5 | 5.000 V | ±0.05% | 5 ppm/°C | Excellent, SOT-23 |
| LM399 | 6.95 V | ±0.3% | 0.3 ppm/°C | Heated substrate, best TC |
| LTZ1000 | 7.0 V | ±0.05% | 0.05 ppm/°C | Ultra-precision, requires heater |

---

## Why Theory Matters for Calibration

You cannot calibrate what you do not understand. Theory tells you:
- What the instrument or standard is supposed to do — so you recognize when it is not doing it
- What the sources of error are — so you know which ones your calibration procedure addresses
- What the limits of the calibration are — so you know when to stop and call it good

If a calibration measurement produces a surprising result, theory is where you look first.

---

## Self-Check Questions

**SC1-1.** In one sentence, state the operating principle of the PRECISION VOLTAGE REFERENCE as described in Chapter 2.

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
- The operating principle of the PRECISION VOLTAGE REFERENCE
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_equipment.md)**
