# Unit 1 — Theory of Operation
**[TM-CAL-014 — Open Handout](../../Calibration_Ideas/dummy_load_power_standard/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand how the reference standard generates or maintains its reference value; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-CAL-014. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. RESISTIVE LOAD PRINCIPLE

Power delivered to a resistive load equals P = V2/R = I2R. For a 50 Ω load driven by an RF source matched to 50 Ω:

```
Power from RMS voltage
P (W) = VRMS2 / 50
```

```
RMS voltage for given power
VRMS = &radic;(P × 50)
```

| Power (W) | VRMS (V) | Vpeak (V) | IRMS (A) |
|---|---|---|---|
| 0.1 | 2.24 | 3.16 | 0.045 |
| 1.0 | 7.07 | 10.0 | 0.141 |
| 5.0 | 15.8 | 22.4 | 0.316 |
| 10.0 | 22.4 | 31.6 | 0.447 |
| 100.0 | 70.7 | 100.0 | 1.41 |

---

## Why Theory Matters for Calibration

You cannot calibrate what you do not understand. Theory tells you:
- What the instrument or standard is supposed to do — so you recognize when it is not doing it
- What the sources of error are — so you know which ones your calibration procedure addresses
- What the limits of the calibration are — so you know when to stop and call it good

If a calibration measurement produces a surprising result, theory is where you look first.

---

## Self-Check Questions

**SC1-1.** In one sentence, state the operating principle of the 50-OHM DUMMY LOAD POWER STANDARD as described in Chapter 2.

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
- The operating principle of the 50-OHM DUMMY LOAD POWER STANDARD
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_equipment.md)**
