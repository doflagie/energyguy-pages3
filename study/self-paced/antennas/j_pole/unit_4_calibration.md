# Unit 4 — Calibration and Tuning
**[TM-ANT-039 — Open Handout](../../../antennas/j_pole/tm.html)**
**TM Chapters:** Chapter 5, Chapter 6, Appendix A
**ELOs:** Calibrate the J-POLE ANTENNA to resonance; tune SWR to within acceptance criteria; apply frequency calculations
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-ANT-039. Read **Chapter 5 — Calibration Procedure**, **Chapter 6 — Tuning and Adjustment**, and **Appendix A** completely.

Then come back here.

---

## Chapter 5 — Calibration Procedure

### 5-1. NANOVNA IMPEDANCE SWEEP

> **NOTE:** The NEC2 model file for this antenna is included in the antenna directory. Run it with
xnec2c, 4nec2, or any NEC2-compatible engine to generate polar plots, impedance data,
and gain figures. The NanoVNA measurements in Chapter 5 should be compared against
NEC2 predictions — deviations >3 dB or >20% impedance indicate a construction error.

1. SOLT calibrate NanoVNA at the end of the feed line (antenna side).
2. Set sweep: center frequency = design band center; span = ±20% of center.
3. Connect CH0 port to antenna feedpoint. Enable Smith Chart and SWR displays.
4. Record: fres (X = 0), SWR at fres, R at fres, SWR bandwidth (2:1 SWR points).
5. Resonance target: X = 0 ±5 Ω, R = 36–52 Ω (ground losses shift R above 36.6 Ω).
6. If fres too high: element is short, extend by 1–2 in. If fres too low: element too long, trim 1 in. Repeat.

---

## Chapter 6 — Tuning and Adjustment

### 6-1. ELEMENT LENGTH ADJUSTMENT

Trim or extend the vertical element to set resonance. Each 1-inch change shifts fres by approximately 20–30 kHz at 40M. Adjust in 2-inch increments. If using a loading coil, adjust coil tap position to shift resonance; moving tap toward the feed end increases inductance and lowers resonance.

### 6-2. RADIAL SYSTEM OPTIMIZATION

Measure R at resonance before and after adding radials. A decrease in R indicates reduced ground loss (desired). Continue adding radials until further additions change R by <1 Ω. The point of diminishing returns is typically 16–32 radials for buried systems, 4–8 for elevated resonant systems.

---

## Appendix A — Calculations and Formulas

```
Quarter-wave element height (feet)
L = 234 / fMHz
```

```
Radiation resistance (quarter-wave over perfect ground)
Rr = 36.6 Ω
```

```
Efficiency (η)
η = Rr / (Rr + Rg + Rcoil)
```

```
Loading coil Q (air-wound)
QL = XL / Rcoil where XL = 2πf·L
```

---

## Key Formulas Summary

- `L = 234 / fMHz`
- `Rr = 36.6 Ω`
- `η = Rr / (Rr + Rg + Rcoil)`
- `QL = XL / Rcoil where XL = 2πf·L`

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

**P4-1.** The design frequency of the J-POLE ANTENNA is stated in Chapter 1. Using the formula from Appendix A,
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
