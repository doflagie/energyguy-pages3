# Unit 1 — Theory of Operation
**[TM-CAL-016 — Open Handout](../../Calibration_Ideas/known_length_dipole_standard/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand how the reference standard generates or maintains its reference value; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-CAL-016. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. HALF-WAVE DIPOLE RESONANCE

```
Free-space half-wave dipole resonant frequency
fr (MHz) = 150 / Lm
```

```
With end effect (velocity factor ~0.95 for wire)
fr (MHz) = 142.5 / Lm or fr (MHz) = 468 / Lft
```

Where L is the *total* tip-to-tip length. At resonance, feedpoint impedance is approximately 70–73 Ω in free space; ground effects reduce this to 20–50 Ω near ground. Height above ground affects both resonant frequency (lowered slightly) and feedpoint impedance.

| Total Length (m) | Total Length (ft) | Resonant Freq (MHz) | Band |
|---|---|---|---|
| 20.07 | 65.8 | 7.1 | 40m CW |
| 10.03 | 32.9 | 14.2 | 20m SSB |
| 6.69 | 21.9 | 21.3 | 15m |
| 4.78 | 15.7 | 29.8 | 10m |
| 2.00 | 6.56 | 71.3 | 4m/VHF |

### 2-2. NANOVANA MEASUREMENT

The NanoVNA measures complex impedance Z = R + jX. At resonance: X = 0 (or near zero). The resonant frequency is observed as the zero-crossing of the reactance curve, or as the minimum of |S11|.

---

## Why Theory Matters for Calibration

You cannot calibrate what you do not understand. Theory tells you:
- What the instrument or standard is supposed to do — so you recognize when it is not doing it
- What the sources of error are — so you know which ones your calibration procedure addresses
- What the limits of the calibration are — so you know when to stop and call it good

If a calibration measurement produces a surprising result, theory is where you look first.

---

## Self-Check Questions

**SC1-1.** In one sentence, state the operating principle of the KNOWN-LENGTH DIPOLE STANDARD as described in Chapter 2.

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
- The operating principle of the KNOWN-LENGTH DIPOLE STANDARD
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_equipment.md)**
