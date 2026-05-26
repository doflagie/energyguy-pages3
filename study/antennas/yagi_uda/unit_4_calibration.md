# Unit 4 — Calibration and Tuning
**[TM-ANT-073 — Open Handout](../../antennas/yagi_uda/tm.html)**
**TM Chapters:** Chapter 5, Chapter 6, Appendix A
**ELOs:** Calibrate the YAGI-UDA DIRECTIONAL ANTENNA to resonance; tune SWR to within acceptance criteria; apply frequency calculations
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-ANT-073. Read **Chapter 5 — Calibration Procedure**, **Chapter 6 — Tuning and Adjustment**, and **Appendix A** completely.

Then come back here.

---

## Chapter 5 — Calibration Procedure

1. SOLT calibrate NanoVNA at feed point of antenna (on bench, 3 ft above ground for initial check).
2. Set sweep: center frequency ±10%.
3. Adjust gamma match: slide shorting bar toward center to increase impedance; away from center to decrease. Adjust trimmer capacitor to null reactance.
4. Target: SWR <1.5:1, R = 45–55 Ω, X = &minus;5 to +5 Ω at design frequency.
5. Once matched, verify SWR across full target band. Record: SWR at center, SWR at band edges, F/B ratio (compare field-strength meter readings front vs. back).
6. At final installed height (away from nearby objects), re-verify SWR — ground proximity affects impedance.

---

## Chapter 6 — Tuning and Adjustment

Gamma match adjustment: Moving the shorting bar changes the equivalent inductance in the match network; moving the trimmer changes reactance compensation. Typical procedure: set trimmer to midrange; slide shorting bar for minimum SWR; fine-adjust trimmer for minimum SWR; iterate. Director lengths can be adjusted ±2% to trade off gain vs. F/B ratio per NEC2 simulation.

---

## Appendix A — Calculations and Formulas

```
Wavelength in free space (m)
λ = 300 / fMHz
```

```
Gain vs. boom length (estimate)
G(dBd) ≈ 10 log10(2.5 × Lboom/λ) for L>0.3λ
```

```
Boom correction factor for element length (metallic boom)
ΔL = &minus;0.0085 × dboom × delem / λ² (approximate)
```

---

## Key Formulas Summary

- `λ = 300 / fMHz`
- `ΔL = &minus;0.0085 × dboom × delem / λ² (approximate)`

---

## Calibration vs. Tuning — Distinction

**Calibration** (Chapter 5): initial setup measurements to establish current antenna state — resonant frequency, SWR at design frequency, impedance at feedpoint. No adjustments yet; you are characterizing what you built.

**Tuning** (Chapter 6): active adjustments based on calibration data. Trim the element, adjust the matching network, or change height/orientation until Chapter 7 acceptance criteria are met.

---

## Acceptance Criterion

Chapter 5 specifies a criterion: **SWR <1.5:1**. Confirm the exact value in the TM.

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

**P4-1.** The design frequency of the YAGI-UDA DIRECTIONAL ANTENNA is stated in Chapter 1. Using the formula from Appendix A,
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
