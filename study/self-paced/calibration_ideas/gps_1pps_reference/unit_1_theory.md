# Unit 1 — Theory of Operation
**[TM-CAL-002 — Open Handout](../../../Calibration_Ideas/gps_1pps_reference/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand how the reference standard generates or maintains its reference value; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-CAL-002. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. GPS TIMING PRINCIPLE

GPS satellites carry cesium and rubidium atomic clocks synchronized to the GPS master control segment, which in turn is steered to UTC(USNO). The GPS receiver continuously solves for the time offset between the receiver clock and GPS time. Once a navigation solution is obtained (position locked), the 1PPS output is synchronized to UTC(GPS) within the stated accuracy.

Typical 1PPS accuracy specifications:

| Module | 1PPS Accuracy (RMS) | 1PPS Accuracy (Peak) |
|---|---|---|
| u-blox NEO-M8N (standard) | ±30 ns RMS | ±60 ns peak |
| u-blox NEO-M8N (timing mode) | ±20 ns RMS | ±30 ns peak |
| Generic SiRF IV module | ±100 ns RMS | ±300 ns peak |
| Generic NEO-6M | ±50 ns RMS | ±100 ns peak |

### 2-2. 1PPS SIGNAL CHARACTERISTICS

The 1PPS pulse is a TTL-level (3.3 V or 5 V, depending on module) positive pulse, typically 100 ms wide, with the *rising edge* coinciding with the UTC second boundary. The pulse width is configurable on most modules.

> **NOTE:** The 1PPS output is valid only after the receiver achieves a 3D position lock (typically 60–120 seconds outdoors, longer with indoor antenna). Always verify lock status before using 1PPS for calibration.

---

## Why Theory Matters for Calibration

You cannot calibrate what you do not understand. Theory tells you:
- What the instrument or standard is supposed to do — so you recognize when it is not doing it
- What the sources of error are — so you know which ones your calibration procedure addresses
- What the limits of the calibration are — so you know when to stop and call it good

If a calibration measurement produces a surprising result, theory is where you look first.

---

## Self-Check Questions

**SC1-1.** In one sentence, state the operating principle of the GPS 1PPS TIMING REFERENCE as described in Chapter 2.

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
- The operating principle of the GPS 1PPS TIMING REFERENCE
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_equipment.md)**
