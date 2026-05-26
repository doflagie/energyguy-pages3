# Unit 4 — Calibration and Tuning
**[TM-ANT-045 — Open Handout](../../antennas/magnetic_loop_antennas/tm.html)**
**TM Chapters:** Chapter 5, Chapter 6, Appendix A
**ELOs:** Calibrate the MAGNETIC LOOP ANTENNA to resonance; tune SWR to within acceptance criteria; apply frequency calculations
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-ANT-045. Read **Chapter 5 — Calibration Procedure**, **Chapter 6 — Tuning and Adjustment**, and **Appendix A** completely.

Then come back here.

---

## Chapter 5 — Calibration Procedure

> **WARNING:** HIGH VOLTAGE HAZARD: At 100 W output power the tuning capacitor of a magnetic loop antenna
develops 4,000–12,000 V across its terminals, depending on loop Q and operating frequency.
Use ONLY vacuum variable capacitors or high-voltage butterfly (split-stator) capacitors rated
for at least 5 kV RMS. Do NOT use DIY stacked aluminum-plate capacitors, rolled-foil capacitors,
or short lengths of coaxial cable as capacitors — these will arc and fail, causing equipment damage
and potential personal injury. Allow the capacitor to fully discharge before touching any part of
the loop structure after transmitting. Keep hands and metal objects away from the capacitor gap
during tuning.

1. Begin with transmitter output ≤5 W for initial tuning. Do not apply full power until resonance is confirmed.
2. SOLT calibrate NanoVNA. Connect to coupling loop feedpoint.
3. Set NanoVNA sweep: target frequency ±10%.
4. Slowly rotate capacitor while watching SWR display. Resonance appears as a sharp dip (narrow bandwidth — sweep may miss it at first).
5. Narrow sweep span to ±2% once resonance found. Verify minimum SWR occurs at exact target frequency.
6. Record: fres, SWRmin, 3 dB bandwidth (rotate cap slowly to find SWR = 1.4× minimum SWR points).
7. Calculate Q: Q = fres / BW3dB. High Q (>200) indicates good construction quality.
8. Verify capacitor voltage is within rating before applying full power: VC = &radic;(2PQ²Rr).

---

## Chapter 6 — Tuning and Adjustment

Magnetic loops require tuning for every frequency change due to high Q and narrow bandwidth. Tune by rotating the variable capacitor until minimum SWR at the desired frequency. A good indicator of resonance is maximum receiver noise (on receive) or minimum reflected power (on transmit at low power). Fine-tune on the air by adjusting for maximum signal strength on a known station or beacon.

> **CAUTION:** Always tune at ≤5 W first. Verify SWR

---

## Appendix A — Calculations and Formulas

```
Loop inductance L (μH, circular loop)
L = 0.002D[ln(4D/d) &minus; 2] (D, d in cm)
```

```
Resonant capacitance
C = 1/(4π²f²L) (μF when L in μH, f in MHz)
```

```
Bandwidth
BW = f0/Q
```

```
Efficiency
η = Rr/(Rr + Rloss)
```

```
Capacitor peak voltage
Vpeak = &radic;(2×P) × Q/&radic;(Rr)
```

---

## Key Formulas Summary

- `L = 0.002D[ln(4D/d) &minus; 2] (D, d in cm)`
- `C = 1/(4π²f²L) (μF when L in μH, f in MHz)`
- `BW = f0/Q`
- `η = Rr/(Rr + Rloss)`
- `Vpeak = &radic;(2×P) × Q/&radic;(Rr)`

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

**P4-1.** The design frequency of the MAGNETIC LOOP ANTENNA is stated in Chapter 1. Using the formula from Appendix A,
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
