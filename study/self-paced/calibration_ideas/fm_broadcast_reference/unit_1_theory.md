# Unit 1 — Theory of Operation
**[TM-CAL-004 — Open Handout](../../../Calibration_Ideas/fm_broadcast_reference/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand how the reference standard generates or maintains its reference value; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-CAL-004. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. FM BROADCAST FREQUENCY STANDARDS

FCC Rules 73.1545 require FM stations to maintain carrier frequency within ±2,000 Hz of assigned channel. Channels are assigned in the range 88.1 to 107.9 MHz at 200 kHz spacing. In practice, modern station automation with GPS-disciplined frequency synthesis typically achieves ±100–500 Hz.

The carrier is accessible without decoding: a spectrum analyzer or antenna analyzer with tracking generator can measure the carrier center frequency directly.

| Station Type | Typical Accuracy | FCC Limit |
|---|---|---|
| Major station with GPS synth | ±100 Hz | ±2 kHz |
| Typical commercial station | ±500 Hz | ±2 kHz |
| Low-power FM (LPFM) | ±2 kHz | ±3 kHz |
| Translator station | ±1 kHz | ±2 kHz |

### 2-2. TRACEABILITY

FM stations certified under FCC 73.1545 are required to maintain compliance. For the purpose of this standard, the FM carrier provides a VHF reference accurate to approximately ±2 kHz / 100 MHz ≈ ±20 ppm worst case, ±5 ppm typical. This is adequate for VHF receiver alignment and spectrum analyzer calibration checks.

---

## Why Theory Matters for Calibration

You cannot calibrate what you do not understand. Theory tells you:
- What the instrument or standard is supposed to do — so you recognize when it is not doing it
- What the sources of error are — so you know which ones your calibration procedure addresses
- What the limits of the calibration are — so you know when to stop and call it good

If a calibration measurement produces a surprising result, theory is where you look first.

---

## Self-Check Questions

**SC1-1.** In one sentence, state the operating principle of the FM BROADCAST CARRIER REFERENCE as described in Chapter 2.

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
- The operating principle of the FM BROADCAST CARRIER REFERENCE
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_equipment.md)**
