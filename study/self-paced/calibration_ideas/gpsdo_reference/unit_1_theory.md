# Unit 1 — Theory of Operation
**[TM-CAL-003 — Open Handout](../../../Calibration_Ideas/gpsdo_reference/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand how the reference standard generates or maintains its reference value; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-CAL-003. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. GPSDO PRINCIPLE

A GPSDO phase-locks a voltage-controlled crystal oscillator (VCXO) or temperature-compensated crystal oscillator (TCXO) to the GPS 1PPS signal using a phase-locked loop (PLL) or proportional-integral (PI) control algorithm.

Long-term frequency accuracy follows GPS (≈10&minus;12 relative). Short-term stability (seconds) follows the free-running crystal oscillator, which is better than GPS for short intervals due to GPS noise.

### 2-2. IMPLEMENTATION: NEO-M8N + Si5351

The Si5351A clock generator synthesizes multiple output frequencies from an onboard 25 MHz crystal. Output frequencies are set by integer and fractional division registers over I2C. The ESP32 microcontroller reads GPS NMEA data, monitors 1PPS, and adjusts Si5351 PLL registers to phase-lock the output.

| Parameter | Value | Notes |
|---|---|---|
| Frequency range | 1 Hz – 160 MHz | Si5351A spec |
| Output frequency (typical) | 10.000000 MHz | Adjustable |
| Long-term accuracy | <1×10&minus;10 | GPS-limited |
| Short-term stability (1 s) | ≈10&minus;9 | Crystal-limited |
| Output level | ~3 dBm into 50 Ω | Si5351 CLK0 |
| Lock time | 5–15 minutes | After GPS position fix |

---

## Why Theory Matters for Calibration

You cannot calibrate what you do not understand. Theory tells you:
- What the instrument or standard is supposed to do — so you recognize when it is not doing it
- What the sources of error are — so you know which ones your calibration procedure addresses
- What the limits of the calibration are — so you know when to stop and call it good

If a calibration measurement produces a surprising result, theory is where you look first.

---

## Self-Check Questions

**SC1-1.** In one sentence, state the operating principle of the GPS-DISCIPLINED OSCILLATOR (GPSDO) as described in Chapter 2.

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
- The operating principle of the GPS-DISCIPLINED OSCILLATOR (GPSDO)
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_equipment.md)**
