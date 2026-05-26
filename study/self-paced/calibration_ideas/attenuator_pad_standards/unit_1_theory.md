# Unit 1 — Theory of Operation
**[TM-CAL-015 — Open Handout](../../../Calibration_Ideas/attenuator_pad_standards/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand how the reference standard generates or maintains its reference value; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-CAL-015. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. PAD TOPOLOGIES

A matched attenuator pad maintains input and output impedance equal to Z0 (typically 50 Ω) at all attenuation settings. This ensures that the pad does not disturb the impedance environment of connected equipment.

```
Attenuation ratio
k = 10AdB/20
```

#### Pi Pad (50 Ω)

```
Shunt resistors R1 = R3
R1 = Z0 × (k+1) / (k&minus;1)
```

```
Series resistor R2
R2 = Z0 × 2k / (k2&minus;1)
```

#### T Pad (50 Ω)

```
Series resistors R1 = R3
R1 = Z0 × (k&minus;1) / (k+1)
```

```
Shunt resistor R2
R2 = Z0 × 2k / (k2&minus;1)
```

| AdB | k | R1 = R3 (Ω) | R2 (Ω) | Nearest 1% std |
|---|---|---|---|---|
| 3 | 1.412 | 292.4 | 17.6 | 294 / 17.8 |
| 6 | 1.995 | 150.5 | 37.4 | 150 / 37.4 |
| 10 | 3.162 | 96.2 | 71.2 | 97.6 / 71.5 |
| 20 | 10.00 | 61.1 | 247.5 | 61.9 / 249 |

---

## Why Theory Matters for Calibration

You cannot calibrate what you do not understand. Theory tells you:
- What the instrument or standard is supposed to do — so you recognize when it is not doing it
- What the sources of error are — so you know which ones your calibration procedure addresses
- What the limits of the calibration are — so you know when to stop and call it good

If a calibration measurement produces a surprising result, theory is where you look first.

---

## Self-Check Questions

**SC1-1.** In one sentence, state the operating principle of the FIXED ATTENUATOR PAD STANDARDS as described in Chapter 2.

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
- The operating principle of the FIXED ATTENUATOR PAD STANDARDS
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_equipment.md)**
