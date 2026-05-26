# Unit 4 — Calibration and Tuning
**[TM-ANT-024 — Open Handout](../../antennas/fan_dipole/tm.html)**
**TM Chapters:** Chapter 5, Chapter 6, Appendix A
**ELOs:** Calibrate the FAN DIPOLE ANTENNA to resonance; tune SWR to within acceptance criteria; apply frequency calculations
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-ANT-024. Read **Chapter 5 — Calibration Procedure**, **Chapter 6 — Tuning and Adjustment**, and **Appendix A** completely.

Then come back here.

---

## Chapter 5 — Calibration Procedure

### 5-1. NANOVNA SWR AND IMPEDANCE SWEEP

> **NOTE:** The NEC2 model file for this antenna is included in the antenna directory. Run it with
xnec2c, 4nec2, or any NEC2-compatible engine to generate polar plots, impedance data,
and gain figures. The NanoVNA measurements in Chapter 5 should be compared against
NEC2 predictions — deviations >3 dB or >20% impedance indicate a construction error.

1. Perform SOLT calibration on NanoVNA using the SOL (Short-Open-Load) kit at the antenna end of the feed line.
2. Set NanoVNA sweep range to cover ±10% of target center frequency (example: 40M → 6.5–7.8 MHz).
3. Connect NanoVNA to feedpoint. Navigate to CH0 S11 display. Select Smith Chart and SWR graphs.
4. Record: frequency of minimum SWR (fres), SWR at fres, SWR at band edges, R + jX at fres.
5. Resonance is confirmed when X ≈ 0 and R ≈ 73 Ω (simple dipole) or per design (matched system).
6. Compare measured fres to design frequency. If fres is too high, the element is short — lengthen each arm 1–2 in. If fres is too low, the element is long — trim each arm 1 in. Repeat until fres is within ±0.5% of design frequency.

---

## Chapter 6 — Tuning and Adjustment

### 6-1. RESONANCE ADJUSTMENT

Trim or extend element arms symmetrically to shift resonance. Each 1-inch change in total length shifts resonance by approximately fMHz/468 × 12 kHz for a 40M dipole. Adjust in 2-inch increments and re-measure SWR between adjustments. Finalize element length when SWR at design frequency is <1.5:1 or as specified.

### 6-2. IMPEDANCE MATCHING

If impedance at resonance differs from 50 Ω, adjust element height (increases ground effect), add a matching network (L-network, λ/4 transformer, or series capacitor), or use a 4:1 balun for designs with higher feed-point impedance such as folded dipoles (50 Ω).

---

## Appendix A — Calculations and Formulas

```
Half-wave dipole element length (each arm, feet)
Larm = 234 / fMHz
```

```
Velocity factor correction (for insulated wire)
Ladj = Larm × Vf (Vf ≈ 0.95 for #14 AWG PVC)
```

```
Radiation resistance (half-wave dipole, free space)
Rr = 73.1 Ω
```

```
Gain over isotropic (free space)
G = 2.14 dBi
```

```
Height for DX angle (degrees elevation for max gain)
θ ≈ arcsin(1 / (2h/λ)) for h > 0.25λ
```

---

## Key Formulas Summary

- `Larm = 234 / fMHz`
- `Ladj = Larm × Vf (Vf ≈ 0.95 for #14 AWG PVC)`
- `Rr = 73.1 Ω`
- `G = 2.14 dBi`

---

## Calibration vs. Tuning — Distinction

**Calibration** (Chapter 5): initial setup measurements to establish current antenna state — resonant frequency, SWR at design frequency, impedance at feedpoint. No adjustments yet; you are characterizing what you built.

**Tuning** (Chapter 6): active adjustments based on calibration data. Trim the element, adjust the matching network, or change height/orientation until Chapter 7 acceptance criteria are met.

---

## Acceptance Criterion

Chapter 5 specifies a criterion: **3 dB**. Confirm the exact value in the TM.

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

**P4-1.** The design frequency of the FAN DIPOLE ANTENNA is stated in Chapter 1. Using the formula from Appendix A,
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
