# Unit 1 — Theory of Operation
**[TM-CAL-011 — Open Handout](../../../Calibration_Ideas/rc_time_constant_standard/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand how the reference standard generates or maintains its reference value; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-CAL-011. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. RC EXPONENTIAL RESPONSE

```
Time constant definition
&tau; = R × C
```

```
Voltage at time t (charging)
V(t) = Vsupply × (1 &minus; e&minus;t/&tau;)
```

```
Time to reach 63.2% of supply
t63% = &tau; = R × C
```

```
10–90% rise time
tr = 2.197 × &tau;
```

| R (Ω) | C | &tau; | f&minus;3dB | Notes |
|---|---|---|---|---|
| 1k | 100 nF | 100 μs | 1592 Hz | Scope timebase ref |
| 1k | 1 μF | 1 ms | 159.2 Hz | Easy to measure |
| 10k | 100 nF | 1 ms | 159.2 Hz | Low current |
| 100k | 10 nF | 1 ms | 159.2 Hz | Very low current |

### 2-2. COMPONENT STABILITY

Capacitor types for precision RC standards:

- C0G/NP0: TC = 0 ±30 ppm/°C. Best choice for precision RC standards.
- X7R: TC = ±15% over temperature. Not acceptable for standards.
- Polystyrene: TC ≈ &minus;120 ppm/°C. Very stable, good for audio range.
- Mica: TC ≈ ±50 ppm/°C. Good, but large and expensive.

> **NOTE:** Resistor types: use 0.1% metal film or better. See TM-CAL-008. Avoid wirewound in AC circuits due to inductance.

---

## Why Theory Matters for Calibration

You cannot calibrate what you do not understand. Theory tells you:
- What the instrument or standard is supposed to do — so you recognize when it is not doing it
- What the sources of error are — so you know which ones your calibration procedure addresses
- What the limits of the calibration are — so you know when to stop and call it good

If a calibration measurement produces a surprising result, theory is where you look first.

---

## Self-Check Questions

**SC1-1.** In one sentence, state the operating principle of the RC TIME-CONSTANT STANDARD as described in Chapter 2.

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
- The operating principle of the RC TIME-CONSTANT STANDARD
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_equipment.md)**
