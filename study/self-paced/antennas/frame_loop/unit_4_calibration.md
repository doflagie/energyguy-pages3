# Unit 4 — Calibration and Tuning
**[TM-ANT-030 — Open Handout](../../../antennas/frame_loop/tm.html)**
**TM Chapters:** Chapter 5, Chapter 6, Appendix A
**ELOs:** Calibrate the FRAME LOOP RECEIVING/TRANSMITTING ANTENNA to resonance; tune SWR to within acceptance criteria; apply frequency calculations
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-ANT-030. Read **Chapter 5 — Calibration Procedure**, **Chapter 6 — Tuning and Adjustment**, and **Appendix A** completely.

Then come back here.

---

## Chapter 5 — Calibration Procedure

1. Connect receiving antenna to receiver. Set receiver to target band (40M or 160M). Use CW or AM mode with narrow filter to minimize noise bandwidth.
2. Listen for a strong reference station on a known bearing. Note S-meter reading.
3. Rotate antenna (K9AY/frame loop) or note signal versus antenna direction. Record signal level in each direction.
4. Adjust termination resistance: vary RT by ±20% and note effect on F/B ratio. Optimize for maximum ratio, not maximum signal level.
5. Use TinySA as signal tracer: inject a −40 dBm signal at antenna terminal; verify output at receiver connector is ≥−80 dBm (insertion loss ≤40 dB is acceptable for receive-only).
6. Record: insertion loss, F/B at 40M, 80M, 160M; SNR vs. reference antenna on noise floor test (no signals, 40M, 2100 UTC).

---

## Chapter 6 — Tuning and Adjustment

Adjust termination resistance for maximum F/B ratio using the on-air method: rotate array toward a known noise source (AC power line, broadcast station in null direction). Vary the termination resistance in small steps while monitoring noise level. Minimum noise in the null direction corresponds to optimal termination resistance. Fine-tune the feed transformer coupling for minimum insertion loss while maintaining impedance match.

---

## Appendix A — Calculations and Formulas

```
Beverage wave velocity (m/s)
v = c / &radic;(1 + (60σ/f)) where σ = ground conductivity (S/m), f in Hz
```

```
Front-to-back ratio (Beverage)
F/B (dB) ≈ 20 log10[1 + e2πL/λ]
```

```
Noise figure of antenna (approximate, HF)
NFant = 10 log10(Tsky/290) (always negative for external-noise-dominated HF)
```

---

## Key Formulas Summary

- `v = c / &radic;(1 + (60σ/f)) where σ = ground conductivity (S/m), f in Hz`
- `NFant = 10 log10(Tsky/290) (always negative for external-noise-dominated HF)`

---

## Calibration vs. Tuning — Distinction

**Calibration** (Chapter 5): initial setup measurements to establish current antenna state — resonant frequency, SWR at design frequency, impedance at feedpoint. No adjustments yet; you are characterizing what you built.

**Tuning** (Chapter 6): active adjustments based on calibration data. Trim the element, adjust the matching network, or change height/orientation until Chapter 7 acceptance criteria are met.

---

## Acceptance Criterion

Chapter 5 specifies a criterion: **40 dB**. Confirm the exact value in the TM.

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

**P4-1.** The design frequency of the FRAME LOOP RECEIVING/TRANSMITTING ANTENNA is stated in Chapter 1. Using the formula from Appendix A,
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
