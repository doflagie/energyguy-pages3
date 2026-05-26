# Unit 1 — Theory of Operation
**[TM-CAL-017 — Open Handout](../../Calibration_Ideas/field_strength_reference/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand how the reference standard generates or maintains its reference value; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-CAL-017. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. FAR-FIELD ELECTRIC FIELD STRENGTH

```
Far-field electric field strength
E (V/m) = &radic;(30 × PW × G) / d
```

Where PW = transmitter power in watts, G = antenna gain (linear ratio, not dB), d = distance in meters from antenna.

```
Half-wave dipole gain (linear)
Gdipole = 1.64 (2.15 dBi)
```

```
Isotropic antenna gain
Gisotropic = 1.0 (0 dBi)
```

| Power (W) | Distance (m) | E (V/m) | E (dBμV/m) |
|---|---|---|---|
| 0.1 | 1 | 7.01 | 137 |
| 0.1 | 3 | 2.34 | 127 |
| 1.0 | 3 | 7.40 | 137 |
| 5.0 | 3 | 16.5 | 144 |
| 5.0 | 10 | 4.96 | 134 |

### 2-2. NEAR FIELD VS. FAR FIELD

The far-field approximation (above) applies only beyond the Rayleigh distance.

```
Far-field boundary (electrically short antenna)
dfar = λ / (2π) ≈ 0.159 λ
```

At HF (3–30 MHz), λ = 10–100 m, so far-field begins at 1.6–16 m. Tests at <3 m at HF may be in the near-field where the 1/d law does not hold.

---

## Why Theory Matters for Calibration

You cannot calibrate what you do not understand. Theory tells you:
- What the instrument or standard is supposed to do — so you recognize when it is not doing it
- What the sources of error are — so you know which ones your calibration procedure addresses
- What the limits of the calibration are — so you know when to stop and call it good

If a calibration measurement produces a surprising result, theory is where you look first.

---

## Self-Check Questions

**SC1-1.** In one sentence, state the operating principle of the FIELD STRENGTH REFERENCE as described in Chapter 2.

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
- The operating principle of the FIELD STRENGTH REFERENCE
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_equipment.md)**
