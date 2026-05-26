# Unit 4 — Calibration and Tuning
**[TM-ANT-022 — Open Handout](../../antennas/end_fed_zepp/tm.html)**
**TM Chapters:** Chapter 5, Chapter 6, Appendix A
**ELOs:** Calibrate the END-FED ZEPP ANTENNA to resonance; tune SWR to within acceptance criteria; apply frequency calculations
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-ANT-022. Read **Chapter 5 — Calibration Procedure**, **Chapter 6 — Tuning and Adjustment**, and **Appendix A** completely.

Then come back here.

---

## Chapter 5 — Calibration Procedure

1. SOLT calibrate NanoVNA at end of feed line (shack end) with the antenna deployed at operating height.
2. Set sweep: target band ±15%.
3. Connect NanoVNA. Record SWR across band. Resonance appears as dip in SWR (may be very sharp for EFHW).
4. If SWR dip is at wrong frequency: trim wire end for higher frequency (too long), or add wire for lower frequency (too short). Change ~6 in for ~50 kHz shift at 40M.
5. Verify SWR on harmonics for multiband use (EFHW is harmonically resonant at 2f, 3f, ...).
6. Check common-mode current: touch coax braid — if RF is felt, add choke at feedpoint.

---

## Chapter 6 — Tuning and Adjustment

Trim wire for resonance. For multiband EFHW, the 49:1 unun presents high impedance at the design frequency; SWR on harmonics is naturally low if the wire is properly trimmed at the fundamental. If SWR is acceptable at fundamental but poor at harmonics, adjust ATU or add a series capacitor (50–100 pF) in the counterpoise for harmonic correction.

---

## Appendix A — Calculations and Formulas

```
Wire length for EFHW (feet)
L = 468 / fMHz
```

```
49:1 unun impedance transformation
Zwire = 49 × Zcoax = 49 × 50 = 2450 Ω
```

```
9:1 unun impedance transformation
Zwire = 9 × 50 = 450 Ω
```

---

## Key Formulas Summary

- `L = 468 / fMHz`
- `Zwire = 49 × Zcoax = 49 × 50 = 2450 Ω`
- `Zwire = 9 × 50 = 450 Ω`

---

## Calibration vs. Tuning — Distinction

**Calibration** (Chapter 5): initial setup measurements to establish current antenna state — resonant frequency, SWR at design frequency, impedance at feedpoint. No adjustments yet; you are characterizing what you built.

**Tuning** (Chapter 6): active adjustments based on calibration data. Trim the element, adjust the matching network, or change height/orientation until Chapter 7 acceptance criteria are met.

---

## Acceptance Criterion

Chapter 5 specifies a criterion: **15%**. Confirm the exact value in the TM.

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

**P4-1.** The design frequency of the END-FED ZEPP ANTENNA is stated in Chapter 1. Using the formula from Appendix A,
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
