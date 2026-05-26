# Unit 4 — Calibration and Tuning
**[TM-ANT-041 — Open Handout](../../../antennas/kmr_400_magnetic_loop/tm.html)**
**TM Chapters:** Chapter 5, Chapter 6, Appendix A
**ELOs:** Calibrate the KMR-400 COMPACT MAGNETIC LOOP ANTENNA to resonance; tune SWR to within acceptance criteria; apply frequency calculations
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-ANT-041. Read **Chapter 5 — Calibration Procedure**, **Chapter 6 — Tuning and Adjustment**, and **Appendix A** completely.

Then come back here.

---

## Chapter 5 — Calibration Procedure

1. SOLT calibrate NanoVNA at antenna end of feed line (or at feedpoint balun output).
2. Set sweep: fundamental resonance ±20%.
3. Verify fres where X = 0. Record R and SWR.
4. Expected: R = 100–150 Ω at resonance before matching. After 2:1 balun: R = 50–75 Ω at 50 Ω coax.
5. Trim loop perimeter in 6-inch increments to shift resonance to design frequency. Do not cut — fold wire back on itself until correct length found, then cut cleanly.
6. Check harmonic resonances at 2f0, 3f0; verify SWR acceptable for multiband use.

---

## Chapter 6 — Tuning and Adjustment

Trim loop perimeter to shift resonance. Each 1-ft change in total perimeter shifts resonance approximately 10–20 kHz at 40M. An ATU at the feed line allows operation across a 2:1 frequency range from a single cut length. For delta loops, adjusting the feed point position (base corner vs. apex) changes the feed impedance but not the resonant frequency.

---

## Appendix A — Calculations and Formulas

```
Full-wave loop perimeter (feet)
P = 1005 / fMHz
```

```
Radiation resistance (full-wave loop, free space)
Rr ≈ 100–150 Ω (shape-dependent)
```

```
Gain over dipole at same height
ΔG ≈ 1.5–2 dB
```

---

## Key Formulas Summary

- `P = 1005 / fMHz`

---

## Calibration vs. Tuning — Distinction

**Calibration** (Chapter 5): initial setup measurements to establish current antenna state — resonant frequency, SWR at design frequency, impedance at feedpoint. No adjustments yet; you are characterizing what you built.

**Tuning** (Chapter 6): active adjustments based on calibration data. Trim the element, adjust the matching network, or change height/orientation until Chapter 7 acceptance criteria are met.

---

## Acceptance Criterion

Chapter 5 specifies a criterion: **2:1**. Confirm the exact value in the TM.

Write the exact criterion from the TM here before you start:
_________________________________________________

---

## Lab Checklist

- [ ] Antenna assembled per Chapter 4 and installed at operating height
- [ ] NanoVNA or SWR bridge connected per Chapter 5
- [ ] Chapter 5 calibration measurements taken and recorded
- [ ] Resonant frequency identified (minimum SWR point)
- [ ] SWR at design frequency recorded
- [ ] Chapter 6 tuning performed if needed
- [ ] Acceptance criterion met

---

## Calibration Log

| Parameter | Measured | Specification | Pass/Fail |
|-----------|----------|---------------|-----------|
| Resonant frequency (MHz) | | (design freq ± tolerance) | |
| SWR at design frequency | | (from Ch. 5) | |
| Feedpoint impedance (Ω) | | (from Ch. 2) | |

---

## Practice Problems

**P4-1.** The design frequency of the KMR-400 COMPACT MAGNETIC LOOP ANTENNA is stated in Chapter 1. Using the formula from Appendix A,
compute the theoretical element length for that frequency. Show all work.

**P4-2.** Your NanoVNA shows minimum SWR at a frequency 3% above the design frequency.
Which direction do you adjust — lengthen or shorten? By what percentage?

**P4-3.** SWR at resonance is 1.8:1, but the acceptance criterion is SWR ≤ 2.0:1.
Does the antenna pass? What does SWR 1.8:1 mean in terms of reflected power?

**P4-4.** Compute reflected power percentage for SWR = 2.0:1.
Formula: reflected power (%) = ((SWR − 1)/(SWR + 1))² × 100

---

## Answer Key — Practice Problems

**P4-1.** See Appendix A. For a dipole: L(ft) = 468/f(MHz). For a quarter-wave vertical: L(ft) = 234/f(MHz). Apply the formula specific to this antenna type.

**P4-2.** Resonance is above design frequency → antenna is electrically short → **lengthen** the element. Adjustment magnitude: approx. 3% longer.

**P4-3.** SWR 1.8:1 passes the 2.0:1 criterion. Reflected power: ((1.8−1)/(1.8+1))² × 100 = (0.8/2.8)² × 100 = 8.2% reflected.

**P4-4.** SWR 2.0:1: ((2−1)/(2+1))² × 100 = (1/3)² × 100 = 11.1% reflected. Most of the power still gets through.

---

## Checkpoint

Before proceeding:
- [ ] Calibration measurements taken and logged
- [ ] Antenna tuned to within the acceptance criterion
- [ ] You can compute element length adjustment from frequency error

**→ Proceed to [Unit 5](unit_5_verification.md)**
