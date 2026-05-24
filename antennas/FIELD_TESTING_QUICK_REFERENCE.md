# Field Testing Quick Reference Card
## nanoVNA + tinySA — Portable Antenna Analysis

**Print this page and laminate for field use**

---

## NANOVNA — 5-MINUTE STARTUP

```
1. POWER ON → 2 min warm-up
2. CALIBRATION:
   • Short standard: S11 ≈ 0° magnitude 1.0
   • Open standard: S11 ≈ 180° magnitude 1.0
   • 50Ω load: S11 ≈ 0° magnitude 0.0
   • If load > 0.05 magnitude: Recalibrate
3. CONNECT ANTENNA → coax to feedpoint
4. SCAN BAND (±20% around center frequency)
5. MARK RESONANCE (zero-reactance point)
6. RECORD: freq_res, SWR@res, Z@res, SWR@band_center
```

---

## QUICK READS

| Measurement | Method | Expected | Tolerance |
|---|---|---|---|
| **Resonance** | Zero-reactance dip on Smith chart | Design freq | ±1% |
| **SWR** | Direct readout or (1+Γ)/(1-Γ) | < 1.5:1 | < 2.0:1 acceptable |
| **Impedance (R)** | Real part at resonance | 50Ω | 40–60Ω typical |
| **Q** | f_center / BW at −3dB | Varies by type | ±20% design |
| **Resonance dip** | Sharpest point on S11 magnitude curve | Band center | ±2% if marginal |

---

## SWR CONVERSION CHART

| SWR | Return Loss (dB) | Reflected Power | Status |
|---|---|---|---|
| 1.0 | ∞ | 0% | Perfect |
| 1.1 | −20 | 0.3% | Excellent |
| 1.5 | −14 | 4% | ✓ Good |
| 2.0 | −10 | 11% | ⚠ Marginal |
| 3.0 | −6 | 25% | ✗ Poor |
| 5.0 | −3 | 56% | ✗ Very poor |

---

## TINYSA — SPECTRUM SCAN

```
1. POWER ON (USB or battery)
2. SET FREQUENCY SPAN:
   • Start: [band low MHz]
   • Stop: [band high MHz]
3. SET REFERENCE LEVEL:
   • Quiet area: −60 dBm
   • Typical: −50 dBm
   • Urban: −30 dBm
4. ENABLE PEAK HOLD
5. TRANSMIT test signal (CW, low power 1–5W)
6. RECORD: peak_freq, peak_dBm, BW@−3dB
7. ROTATE ANTENNA (360°) → record max/min dBm
```

---

## BANDWIDTH & Q CALCULATION

```
Q = f_center / (f_high − f_low)

Example (40M compact dipole):
  f_center: 7.150 MHz
  f_−3dB_low: 7.075 MHz
  f_−3dB_high: 7.225 MHz
  BW: 0.150 MHz
  Q: 7.150 / 0.150 = 47.7 ≈ 48
```

**Expected Q by antenna type:**
- Full-size dipole: Q ≈ 50–100
- Loaded dipole: Q ≈ 15–30
- Magnetic loop: Q ≈ 100–300
- Yagi: Q ≈ 40–100

---

## PROBLEM DIAGNOSIS TREE

### High SWR (> 2.0)

```
☐ Check cable: TDR scan → look for discontinuity
☐ Check connection: DC resistance < 0.5Ω (center-to-shield)
☐ Check length: Measure arm length vs. design ±5%
☐ Check traps: Secondary resonance offset? Adjust trimmer
☐ Check ground: Nearby metal objects < 10m shift resonance
```

### Frequency Offset (> ±2% from design)

```
Too LOW:  → Loading coil too many turns → Remove turns
          → Antenna too long → Trim wire
Too HIGH: → Loading coil too few turns → Add turns
          → Antenna too short → Extend wire
```

### Broad Resonance Peak (Q much lower than expected)

```
☐ High ground loss (wet soil, salt water)
☐ Cable moisture (TDR check)
☐ Poor solder joints (visual inspect feedpoint)
☐ High ambient RF (near radio station)
```

---

## TRAP TUNING PROCEDURE

```
GOAL: Minimize SWR at design frequency

1. Locate secondary resonance peak (Smith chart)
2. Check if peak is above/below design frequency
3. Adjust trimmer capacitor:
   CW = lower frequency (increase capacitance)
   CCW = raise frequency (decrease capacitance)
4. Make 0.5-turn adjustment
5. Rescan nanoVNA
6. Repeat until secondary peak merges with primary
```

---

## IMPEDANCE TARGETS

| Antenna Type | Feedpoint Z | SWR Target |
|---|---|---|
| Dipole (center-fed) | 50–75Ω | < 1.3 |
| Monopole (radiated) | 35–40Ω | < 1.5 |
| Yagi (reflector spaced) | 40–60Ω | < 1.2 |
| Loop (mag coupling) | 25–100Ω | < 2.0 |
| End-fed | 200–600Ω | Needs tuner |
| End-fed HALF-wave | 2000–5000Ω | Needs 9:1 unun |

*Use antenna tuner if feedpoint Z >> 50Ω*

---

## FIELD LOG — MINIMUM DATA

For each test, record:

```
ANTENNA: ___________________    DATE: ___________
LOCATION: _________________    OPERATOR: ________

CONFIGURATION:
  Height AGL: _____ m    Arm length: _____ m
  Feedline: _____ ft @ _____ (type)

MEASUREMENTS:
  nanoVNA:
    Res freq: _____ MHz    SWR@res: _____
    Impedance R/jX: _____ / _____ Ω
    
  tinySA:
    Peak: _____ MHz        Power: _____ dBm
    BW@−3dB: _____ kHz     Q: _____
    
  Azimuth test (if applicable):
    0°: _____ dBm  90°: _____ dBm
    180°: _____ dBm  270°: _____ dBm

STATUS: ☐ PASS ☐ MARGINAL ☐ FAIL
NOTES: _____________________________
```

---

## TORQUE SPEC

| Connector | Torque | Note |
|---|---|---|
| SMA female | 1.2 N·m (10 in·lbf) | Use calibrated wrench |
| N female | 1.7 N·m | Tighter than SMA |
| PL-259 | 1.5–2.0 N·m | Hand-tight + 1/4 turn |

**Over-tightening damages gold plating and thread.**

---

## CALIBRATION KIT — WHAT TO CARRY

- [ ] nanoVNA + power cable (USB-C)
- [ ] Short standard (shorted connector)
- [ ] Open standard (included in kit)
- [ ] 50Ω load (precision terminator)
- [ ] Test coax cable(s) with length marked
- [ ] SMA-to-[antenna connector] adapter(s)
- [ ] Torque wrench (SMA 1.2 N·m)
- [ ] Calipers (measure antenna dimensions)
- [ ] tinySA + power (USB-C battery pack)
- [ ] Small antenna probe (whip/dipole)
- [ ] Notebook + pen for field log
- [ ] Camera (document antenna configuration)

---

## COMMON MEASUREMENT ERRORS

| Error | Cause | Fix |
|---|---|---|
| Erratic SWR | Loose SMA connector | Torque to 1.2 N·m |
| Impedance rotated on Smith chart | Cable VF wrong | Check cable type (0.66 or 0.85) |
| Broad dip | High loss environment | TDR scan; inspect for moisture |
| No spectrum peak | Transmitter off | Verify tx active; check frequency |
| Resonance shifts during session | Temperature change | Allow 30 min thermal stabilization |

---

## REFERENCE: FORMULAS

```
SWR = (1 + |Γ|) / (1 − |Γ|)

Return Loss (dB) = −20 × log₁₀(|Γ|)

Resonance: 1/(2π√LC)

Q = f₀ / Δf(−3dB)

Arm length (half-wave dipole, full size):
L_half = 71.3 / f_MHz (meters)
```

---

## PASS/FAIL QUICK CHECK

✓ **PASS** if:
- [ ] Resonance within ±1% of design
- [ ] SWR < design target at resonance
- [ ] Impedance R within 40–60Ω (for 50Ω designs)
- [ ] Q measured within ±20% of design
- [ ] Radiation pattern matches antenna type

⚠ **MARGINAL** if:
- [ ] Resonance within ±2% of design
- [ ] SWR 1.5–2.0 (slightly high)
- [ ] Impedance R within 35–70Ω
- [ ] Needs minor tuning (trap adjustment, trim wire)

✗ **FAIL** if:
- [ ] Resonance > ±3% from design
- [ ] SWR > 2.5 at design frequency
- [ ] Impedance R outside 25–100Ω
- [ ] Radiation pattern asymmetric/unexpected
- [ ] Requires rework (Section IV.2 of main guide)

---

## CONTACTS & RESOURCES

- **nanoVNA GitHub**: github.com/ttrftech/NanoVNA
- **nanoVNA Saver software**: github.com/NanoVNASaver/nanovna-saver
- **tinySA project**: github.com/DiSlord/NanoVNA-D
- **Field Testing guide**: See `FIELD_TESTING_WITH_INSTRUMENTS.md` in antenna suite

---

**Laminate this card and bring to field!**
