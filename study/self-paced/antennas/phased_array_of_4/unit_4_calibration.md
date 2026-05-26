# Unit 4 — Calibration and Tuning
**[TM-ANT-053 — Open Handout](../../../antennas/phased_array_of_4/tm.html)**
**TM Chapters:** Chapter 5, Chapter 6, Appendix A
**ELOs:** Calibrate the PHASED ARRAY — 4-ELEMENT to resonance; tune SWR to within acceptance criteria; apply frequency calculations
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-ANT-053. Read **Chapter 5 — Calibration Procedure**, **Chapter 6 — Tuning and Adjustment**, and **Appendix A** completely.

Then come back here.

---

## Chapter 5 — Calibration Procedure

> **NOTE:** The NEC2 model file for this antenna is included in the antenna directory. Run it with
xnec2c, 4nec2, or any NEC2-compatible engine to generate polar plots, impedance data,
and gain figures. The NanoVNA measurements in Chapter 5 should be compared against
NEC2 predictions — deviations >3 dB or >20% impedance indicate a construction error.

1. SOLT calibrate NanoVNA at feedpoint (antenna connector).
2. Set sweep range: ±10% of design center frequency.
3. Connect NanoVNA to antenna. Record SWR minimum and frequency, R + jX at resonance.
4. Compare measured impedance to NEC2 predicted values. Investigate discrepancies >20%.
5. Verify SWR ≤1.5:1 at operating frequency. If not, adjust matching per Chapter 6.
6. For gain verification: compare received signal strength (using TinySA as receiver) from a known beacon vs. reference antenna.

---

## Chapter 6 — Tuning and Adjustment

Element lengths can be adjusted by ±2% of design length to shift resonance and optimize impedance. For matching network adjustment: follow gamma-match, delta-match, or transformer-tap procedure per the specific implementation. Refer to NEC2 model for sensitivity analysis — the model can be re-run at ±2% element length to determine which element most affects resonance and F/B ratio.

---

## Appendix A — Calculations and Formulas

```
Wavelength in free space
λ (m) = 300 / fMHz
```

```
Effective gain formula (over isotropic)
GdBi = 10 log10(4π × Aeff / λ²) where Aeff = effective aperture (m²)
```

```
Special parameters
Square 1λ spacing, switchable patterns (broadside, endfire, cardioid); 7–9 dBi on selected heading; system uses PIN diode switching
```

---

## Key Formulas Summary

- `λ (m) = 300 / fMHz`
- `GdBi = 10 log10(4π × Aeff / λ²) where Aeff = effective aperture (m²)`

---

## Calibration vs. Tuning — Distinction

**Calibration** (Chapter 5): initial setup measurements to establish current antenna state — resonant frequency, SWR at design frequency, impedance at feedpoint. No adjustments yet; you are characterizing what you built.

**Tuning** (Chapter 6): active adjustments based on calibration data. Trim the element, adjust the matching network, or change height/orientation until Chapter 7 acceptance criteria are met.

---

## Acceptance Criterion

Chapter 5 specifies a criterion: **SWR ≤1.5:1**. Confirm the exact value in the TM.

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

**P4-1.** The design frequency of the PHASED ARRAY — 4-ELEMENT is stated in Chapter 1. Using the formula from Appendix A,
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
