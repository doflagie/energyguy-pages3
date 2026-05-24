# TECHNICAL MANUAL — TM-EF-001 Rev A
# END-FED QUARTER-WAVE (EFQW) ANTENNA SYSTEM
# 160 Meters Through 20 Centimeters

---

**DISTRIBUTION:** Unrestricted  
**AUTHORITY:** Project documentation  
**EFFECTIVE DATE:** 2026-05-06  
**SUPERSEDES:** None (initial issue)

---

## RECORD OF CHANGES

| Change | Date | Description | By |
|--------|------|-------------|-----|
| Rev A  | 2026-05-06 | Initial issue | MM |

---

## TABLE OF CONTENTS

- Chapter 1: Introduction and Theory
- Chapter 2: Antenna Physics and Design Equations
- Chapter 3: Feedpoint Matching Networks
- Chapter 4: LC Trap Theory and Construction
- Chapter 5: Directional Coupler Theory
- Chapter 6: SWR Measurement and Interpretation
- Chapter 7: Construction — HF Fixed Antennas
- Chapter 8: Construction — Roll-Up Trapped Multiband Antenna
- Chapter 9: Construction — VHF/UHF Elements
- Chapter 10: NEC Modeling and Simulation
- Chapter 11: Electronic Instrumentation (ESP32 + CYD)
- Chapter 12: Calibration and Field Setup
- Chapter 13: Troubleshooting
- Chapter 14: Parts Reference
- Appendix A: Winding Tables — Traps
- Appendix B: Winding Tables — Matching Inductors
- Appendix C: NEC Card Reference
- Appendix D: Quick Reference Card

---

## CHAPTER 1: INTRODUCTION AND THEORY

### 1.1 Purpose

This manual provides complete design, construction, and operational data for a family of end-fed quarter-wave (EFQW) antennas covering all fifteen amateur radio bands from 160 meters through 20 centimeters. It includes matching network theory, LC trap design, directional coupler theory, numerical electromagnetic model (NEC) files, and embedded controller firmware.

### 1.2 System Overview

An EFQW antenna consists of:

1. **Radiating element** — wire or rigid rod, electrical quarter-wavelength at the operating frequency
2. **Counterpoise** — one or more radials providing the return current path
3. **Feedpoint matching network** — transforms ~35 Ω feed impedance to 50 Ω coaxial line
4. **Choke balun** — prevents RF from flowing on the coaxial shield outer surface
5. **SWR monitor** — directional coupler, diode detectors or AD8307 log amplifiers, ESP32 microcontroller
6. **CYD display** — 320×240 ILI9341 touchscreen for real-time SWR, power, and tuning guide

For the roll-up multiband version, LC traps are inserted at calculated intervals along the radiating element. Each trap resonates at a specific frequency and presents a high impedance that electrically shortens the antenna for that band.

### 1.3 Scope

| Band | Freq (MHz) | QW Length | Version |
|------|-----------|-----------|---------|
| 160m | 1.900 | 38.29 m | Fixed wire |
| 80m  | 3.750 | 19.40 m | Fixed wire |
| 60m  | 5.330 | 13.67 m | Fixed wire |
| 40m  | 7.150 | 10.18 m | Fixed wire / Roll-up base |
| 30m  | 10.125 | 7.19 m | Fixed wire |
| 20m  | 14.175 | 5.13 m | Roll-up (top section) |
| 17m  | 18.118 | 4.01 m | Roll-up |
| 15m  | 21.225 | 3.43 m | Roll-up |
| 12m  | 24.940 | 2.92 m | Roll-up |
| 10m  | 28.500 | 2.55 m | Roll-up tip |
| 6m   | 51.000 | 1.40 m | Rigid element |
| 2m   | 146.000 | 0.488 m | Rigid rod |
| 1.25m| 222.000 | 0.321 m | Rigid rod |
| 70cm | 446.000 | 0.160 m | Rigid tube |
| 33cm | 906.000 | 0.0785 m | Machined |

---

## CHAPTER 2: ANTENNA PHYSICS AND DESIGN EQUATIONS

### 2.1 Quarter-Wave Radiator

The fundamental relationship between frequency and quarter-wave element length is:

```
λ/4 = c / (4f)

where:
  c = 2.998 × 10⁸ m/s (speed of light)
  f = frequency in Hz

Simplified for practical use:
  QW_bare (m) = 75 / f_MHz

With velocity factor VF:
  QW_physical (m) = 75 × VF / f_MHz

Velocity factors:
  #14–18 AWG insulated wire:  VF = 0.97
  3mm–6mm aluminum rod:       VF = 0.95
  Copper tubing:               VF = 0.95
```

**Example (40m, 7.150 MHz):**
```
QW_bare     = 75 / 7.150  = 10.49 m
QW_physical = 10.49 × 0.97 = 10.18 m
```

### 2.2 Feed Impedance

A quarter-wave monopole over a perfect ground plane exhibits a feedpoint impedance of approximately 36 Ω (half the 72 Ω dipole value). Over real ground with limited radials:

| Ground Condition | Typical Z_feed |
|-----------------|----------------|
| Perfect (NEC GN 1) | 36 Ω + j0 |
| Real ground, 4 radials | 40–50 Ω + j5–j15 |
| Real ground, 2 radials | 50–70 Ω + j10–j25 |
| Elevated, 4 sloped radials | 38–45 Ω + j5 |

For design purposes, use Z_feed = 35 Ω for L-network calculations. Fine-tuning is accomplished with the variable capacitor.

### 2.3 Counterpoise Design

The counterpoise (ground radials) carries the return current and establishes the antenna's reference plane. Guidelines:

- **Minimum:** 2 radials, each λ/4 at operating frequency
- **Practical:** 4 radials for HF; 4 rigid bent radials for VHF/UHF
- **For roll-up portable:** 2 wire radials deployed on ground surface
- **Radial length:** Same as radiating element (λ/4), or 5% shorter
- **VHF/UHF:** 4 radials bent downward at 45° form a cone ground plane; radial length = 0.95 × λ/4

### 2.4 Radiation Pattern

A quarter-wave monopole over a ground plane produces a hemispherical pattern with the main lobe at low elevation angles (15–25°) — excellent for long-distance HF communication. Theoretical gain relative to isotropic:

- Over perfect ground: +5.2 dBi (same as half-wave dipole)
- Over real ground: +2 to +4 dBi depending on soil conductivity

For VHF/UHF, the 4-radial ground plane provides a pattern tilted slightly upward (~15°), useful for both local and elevated path work.

---

## CHAPTER 3: FEEDPOINT MATCHING NETWORKS

### 3.1 L-Network Theory (HF Bands)

The L-network is a two-element matching network consisting of a series reactance and a shunt reactance. The low-pass configuration (series inductor, shunt capacitor) is preferred because it attenuates harmonics.

**Design equations:**

```
Q_design = √(Z_source / Z_load − 1)
         = √(50 / 35 − 1)
         = √(0.4286)
         = 0.655

Series arm (inductive):
  Xs = Q × Z_load
     = 0.655 × 35
     = 22.9 Ω

Shunt arm (capacitive):
  Xp = Z_source / Q
     = 50 / 0.655
     = 76.3 Ω
```

**Values per band:**

| Band | f (MHz) | Ls (µH) | Cp (pF) |
|------|---------|---------|---------|
| 160m | 1.900 | 1.920 | 1,098 |
| 80m  | 3.750 | 0.973 | 556 |
| 60m  | 5.330 | 0.684 | 391 |
| 40m  | 7.150 | 0.510 | 291 |
| 30m  | 10.125 | 0.360 | 205 |
| 20m  | 14.175 | 0.257 | 147 |
| 17m  | 18.118 | 0.201 | 115 |
| 15m  | 21.225 | 0.172 | 98 |
| 12m  | 24.940 | 0.146 | 83 |
| 10m  | 28.500 | 0.128 | 73 |

Variable trimmer covers ±30% range; fixed NP0 provides the base value. Total C = C_fixed + C_variable.

### 3.2 Gamma Match (VHF Bands: 6m, 2m, 1.25m)

The gamma match is a practical feeding technique for vertical monopoles at VHF frequencies. A short parallel rod (the gamma rod) taps a portion of the element voltage; a series capacitor tunes out the residual reactance.

**Design dimensions:**

| Band | f (MHz) | Element (m) | Gamma rod (m) | Rod spacing | C_gamma |
|------|---------|-------------|---------------|-------------|---------|
| 6m   | 51.0    | 1.40        | 0.210         | 15 mm       | 0–30 pF |
| 2m   | 146.0   | 0.488       | 0.073         | 12 mm       | 0–20 pF |
| 1.25m| 222.0   | 0.321       | 0.048         | 10 mm       | 0–12 pF |

**Adjustment procedure:**
1. Set rod length to tabulated value; fix spacing.
2. Adjust C_gamma for minimum SWR (start at mid-range).
3. Slide rod outward (longer) if SWR minimum is above center frequency.
4. Slide rod inward (shorter) if SWR minimum is below center frequency.
5. Iterate C_gamma and rod position until SWR < 1.5.

### 3.3 Direct Feed (UHF Bands: 70cm, 33cm)

At 70 cm and above, the feedpoint impedance of a well-constructed monopole with four λ/4 radials bent at 45° is typically 48–52 Ω — close enough to 50 Ω for direct coaxial connection without a matching network. A sleeve balun is still required.

### 3.4 Choke Balun

The choke balun prevents common-mode RF current from flowing on the outside of the coaxial shield. Without it, the coaxial feed cable becomes part of the antenna system, distorting the pattern and introducing RF into the station.

**HF construction:**
- Wind 8–10 turns of RG-174 or RG-58 on an FT-240-31 toroid core (Mix 31, 73 material)
- Place at feedpoint, in series with coaxial feed
- Common-mode choking impedance: >1,000 Ω from 1.8 to 30 MHz

**VHF construction (sleeve balun):**
- Machine or form a λ/4 copper or aluminum tube
- Tube slides over coaxial outer jacket; open at bottom, soldered to braid at top
- Length = 0.95 × (c/4f) (uses 0.95 VF for tube approximation)

---

## CHAPTER 4: LC TRAP THEORY AND CONSTRUCTION

### 4.1 Parallel Resonance Theory

A parallel LC circuit resonates at:

```
f₀ = 1 / (2π√(L×C))

Rearranged for design:
  C = 1 / ((2πf₀)² × L)
  L = 1 / ((2πf₀)² × C)
```

At resonance, the impedance of a parallel LC circuit reaches its maximum value:

```
Z_trap = Q² × R_coil  (practical)
       ≈ Q × √(L/C)   (approximate, valid when Q >> 1)

where:
  R_coil = series resistance of the inductor winding
  Q      = quality factor = ω₀L / R_coil
```

For toroid inductors at HF: Q = 150–260 (depending on core material and frequency). This gives:

```
Z_trap = 5,000–20,000 Ω

Choking ratio = Z_trap / Z_wire = 5,000–20,000 / 50 = 100:1 to 400:1
```

This ratio is sufficient to "open" the antenna wire at the trap frequency. The wire beyond the trap (toward the tip) is electrically disconnected at that frequency.

### 4.2 Behavior Off-Resonance

The trap is a reactive element when operating off its resonant frequency:

```
Below f₀: Inductor dominates → trap is inductive
           Adds electrical length → antenna resonates LOWER than physical length

Above f₀: Capacitor dominates → trap is capacitive
           Subtracts electrical length → antenna resonates HIGHER
```

This property is used to make the roll-up antenna multiband: each trap electrically isolates the wire beyond it at the trap's resonant frequency, while being nearly transparent to frequencies well below resonance.

### 4.3 Core Selection

| Core | Material | AL (nH/t²) | Freq range | Application |
|------|----------|------------|-----------|-------------|
| T68-2 | Mix 2 (red) | 57 | DC–30 MHz | 160m, 80m, 60m traps |
| T50-2 | Mix 2 (red) | 49 | DC–30 MHz | 40m, 30m traps |
| T50-6 | Mix 6 (yellow) | 40 | 10–50 MHz | 20m, 17m, 15m, 12m, 10m |
| T37-10 | Mix 10 (black) | 32 | 40–150 MHz | Not used in this design |
| Air core | N/A | per dia/turns | >50 MHz | 6m, 2m traps |

### 4.4 Winding Calculation

```
L (µH) = AL × N² / 1,000

Turns required:
  N = √(L_required_µH × 1,000 / AL)

Example: 40m trap, T50-2, target L = 1.1 µH
  N = √(1.1 × 1000 / 49) = √22.4 = 4.73 → 5 turns (round up)
  Actual L = 49 × 25 / 1000 = 1.225 µH
  Required C = 1 / ((2π × 7.15e6)² × 1.225e-6) = 203 pF → 220 pF NP0
```

### 4.5 Capacitor Selection

Use NP0 (C0G) class capacitors only. These have near-zero temperature coefficient and maintain value to better than ±30 ppm/°C. Silver mica capacitors are equivalent or superior but larger.

**Voltage rating:** An amateur radio transmitter at 100 W into 50 Ω produces approximately 100 V peak on the feed line. Traps, especially those at resonance with moderate SWR, can see 2–5× higher voltage. Use capacitors rated at minimum 500 V for HF traps; 100 V acceptable for VHF.

**Combining capacitors:** When the required capacitance falls between standard values, combine two capacitors in series (reduces value, increases voltage rating) or in parallel (increases value). See Appendix A for exact values per band.

### 4.6 Roll-Up Antenna Trap Placement

The roll-up antenna uses five traps for coverage of 10m through 40m from a single element. Trap order from feedpoint: highest frequency to lowest. This ensures that when operating on a lower band, the higher-band traps are below their resonant frequency and add only a small inductive reactance to the element, slightly loading it but maintaining resonance.

```
Feedpoint                                                    Tip
    │      T10          T12          T15          T20          │
    ├──────[trap]────[trap]────[trap]────[trap]─────────────┤
    │       │            │            │            │              │
   [L-net] 2.55m       2.92m       3.43m       5.13m        10.18m
```

**Active wire on each band:**

| Band | Active section | Traps passed |
|------|---------------|--------------|
| 10m  | FP → 2.55m (T10 blocks) | None |
| 12m  | FP → 2.92m (T12 blocks) | T10 inductive |
| 15m  | FP → 3.43m (T15 blocks) | T10, T12 inductive |
| 20m  | FP → 5.13m (T20 blocks) | T10, T12, T15 inductive |
| 40m  | FP → 10.18m (full wire) | All traps transparent |

### 4.7 Trap Construction Procedure

1. Select core type from Appendix A for target band.
2. Calculate turns from Table A-1. Wind tightly with #22–#26 enameled magnet wire.
3. Trim turns ±1 to achieve target inductance (measure with LC meter or NanoVNA).
4. Solder the capacitor in parallel with the winding (one cap lead to each end of the winding).
5. Verify resonant frequency with antenna analyzer: insert trap in series; sweep for impedance peak.
6. Target: resonance within ±50 kHz of design frequency.
7. Install in 3D-printed housing (trap_housing.scad). Thread antenna wire through grommet holes; solder to winding ends.
8. Snap lid; mark band designation on top of housing.

---

## CHAPTER 5: DIRECTIONAL COUPLER THEORY

### 5.1 Traveling Wave Decomposition

A transmission line carrying a mismatched load supports two traveling waves simultaneously:

```
Incident (forward) wave:  V⁺, traveling from source to load
Reflected wave:            V⁻, traveling from load to source

Total voltage at any point: V = V⁺ + V⁻
Total current at any point: I = (V⁺ − V⁻) / Z₀

Solving:
  V⁺ = (V + I×Z₀) / 2    (forward voltage wave amplitude)
  V⁻ = (V − I×Z₀) / 2    (reflected voltage wave amplitude)
```

A directional coupler separates V⁺ from V⁻ by combining magnetic coupling (proportional to I) with electric coupling (proportional to V) in correct phase.

### 5.2 Bruene Coupler Design

The Bruene coupler uses a transformer to couple the line current, combined with a resistive voltage divider to sample the line voltage. These two signals are summed to give V⁺ or subtracted to give V⁻.

```
                PRIMARY: 1 turn (single pass-through)
                ┌──────────────────────────────────┐
  IN ───────── ═══════[FT-37-43 toroid, 10T sec]══ ─── ANT
                └──────────────────────────────────┘
                                 │
                SECONDARY: 10 turns — drives two detector networks:

  FORWARD DETECTOR:
  Sec.end1 ──[R1: 51Ω]──┬──[D1: 1N5711]──┬── Vfwd DC
                         │                │
                        [R2: 51Ω]        [C1: 100nF]
                         │                │
  Sec.end2 ──────────────┘               GND

  REFLECTED DETECTOR (D2 reversed):
  Sec.end1 ──[R3: 51Ω]──┬──[D2: 1N5711]──┬── Vrefl DC
                         │   (reversed)    │
                        [R4: 51Ω]        [C2: 100nF]
  Sec.end2 ──────────────┘               GND
```

The secondary winding couples a voltage proportional to line current. The termination resistors (R1–R4, 51 Ω matched to secondary characteristic impedance) ensure that signal combination is directional. An optional voltage sampling network (R5: 1 kΩ, R6: 51 Ω from the antenna side to GND) enhances directivity by adding the electric field component.

**Coupling factor (10T secondary on FT-37-43):**
- Turns ratio: N_s/N_p = 10/1 = 10
- Induced secondary voltage ≈ line current × ωM
- Coupling ≈ 20 dB below line signal (approximate; calibrate empirically)

### 5.3 AD8307 Logarithmic Detector Implementation

For calibrated power measurement, the Bruene coupler's RF output ports feed AD8307 logarithmic amplifier ICs instead of simple diode detectors.

```
Vfwd_RF  ──[C: 100nF]──[AD8307 #1]── Vout_fwd  (DC, 25 mV/dB)
Vrefl_RF ──[C: 100nF]──[AD8307 #2]── Vout_refl (DC, 25 mV/dB)
```

**AD8307 output equation:**
```
V_out_mV = 25 × (P_in_dBm + 84)

Solving for power:
  P_dBm = (V_out_mV / 25) − 84

Dynamic range: −73 to +17 dBm (90 dB)

After coupler correction:
  P_fwd_dBm_actual  = P_fwd_dBm_detected  + Coupling_factor_dB
  P_refl_dBm_actual = P_refl_dBm_detected + Coupling_factor_dB
```

The AD8307 requires a 2.7–5.5 V supply. At 3.3 V (ESP32), output swing is limited to ~0–2.0 V, covering about −73 to +13 dBm input. Power in watts:

```
P_watts = 10^((P_dBm − 30) / 10)
```

### 5.4 SWR and Reflection Coefficient Calculation

```
ρ (reflection coefficient, voltage) = |V⁻| / |V⁺|
                                     = √(P_refl / P_fwd)
                                     = 10^((P_refl_dBm − P_fwd_dBm) / 20)

SWR = (1 + ρ) / (1 − ρ)

Return loss (dB) = −20 × log₁₀(ρ)
                 = P_fwd_dBm − P_refl_dBm  (difference in dB)

Mismatch loss (dB) = −10 × log₁₀(1 − ρ²)
```

**Quick reference:**

| SWR | ρ | Return Loss | P_refl/P_fwd |
|-----|---|-------------|-------------|
| 1.0:1 | 0.000 | ∞ dB | 0% |
| 1.1:1 | 0.048 | 26.4 dB | 0.23% |
| 1.5:1 | 0.200 | 14.0 dB | 4.0% |
| 2.0:1 | 0.333 | 9.5 dB | 11.1% |
| 3.0:1 | 0.500 | 6.0 dB | 25.0% |
| 5.0:1 | 0.667 | 3.5 dB | 44.4% |
| 10.0:1 | 0.818 | 1.7 dB | 66.9% |

### 5.5 Directivity

Directivity is the ability of the coupler to discriminate forward from reflected power. It is defined as:

```
Directivity (dB) = Forward port coupling (dBc) − Reverse port coupling (dBc)

Coupler type                  Directivity     Frequency range
Simple diode bridge           20–25 dB        DC–150 MHz
Bruene (toroid + R network)   30–35 dB        1.8–30 MHz
Dual AD8307 + Bruene          35–40 dB        1–500 MHz
Commercial (Mini-Circuits)    40+ dB          per model
```

For field tuning (finding minimum SWR rather than measuring absolute power), 20 dB directivity is adequate. For calibrated power logging, use the AD8307 implementation targeting 35 dB directivity.

---

## CHAPTER 6: SWR MEASUREMENT AND INTERPRETATION

### 6.1 Interpreting SWR

SWR is the ratio of maximum to minimum voltage along a transmission line. It is determined entirely by the load impedance and the characteristic impedance of the feed line.

For field tuning, the goal is to minimize SWR, not to achieve a specific absolute value. A properly constructed and tuned EFQW should achieve SWR < 1.5:1 across the entire operating bandwidth of each band.

### 6.2 SWR Across the Band

A quarter-wave antenna with L-network matching can achieve SWR < 1.5 across a bandwidth that depends on the Q of the matching network. Approximate 1.5:1 bandwidth:

```
BW (MHz) ≈ f₀ / (2Q)

For Q = 0.655 (our design): BW ≈ 0.76 × f₀

This gives very wide bandwidth. The practical bandwidth is limited by the
antenna element's natural impedance variation across the band.

For 40m at 7.150 MHz:
  Expected 1.5:1 SWR bandwidth: ≈ 200 kHz using variable cap trim
```

### 6.3 Effect of Trap Q on Bandwidth

Each LC trap introduces some insertion loss and affects the impedance bandwidth of the affected band:

```
Insertion loss = 20 × log₁₀(Q / √(Q² + 1)) ≈ 0 dB for Q >> 1

At Q = 200:  Insertion loss = 0.022 dB  (negligible)
At Q = 50:   Insertion loss = 0.09 dB   (still acceptable)
```

### 6.4 Common SWR Problems and Causes

| Symptom | Likely Cause |
|---------|-------------|
| SWR > 3:1 on all bands | Choke balun missing or defective |
| SWR high on one band only | Trap mistuned; check with analyzer |
| SWR won't null below 2:1 | Element length error >5%; measure and correct |
| SWR varies with antenna orientation | Common-mode current; improve balun |
| SWR good at feedpoint, high at rig | Coax loss; check connectors |
| Minimum SWR shifts with transmitter power | Trap core saturating; use higher-power core |

---

## CHAPTER 7: CONSTRUCTION — HF FIXED ANTENNAS

### 7.1 Materials List (per antenna)

| Item | Specification | Quantity |
|------|--------------|---------|
| Antenna wire | #14–18 AWG stranded, THHN or PTFE | Per length table |
| Toroid core | Per Appendix B | 1 |
| Enameled wire | #22 AWG | 1 m |
| NP0 capacitor | Per Appendix B | 1–2 |
| Variable trimmer | 0–500 pF, Jackson Brothers or equiv | 1 |
| SMA connector (F) | Panel mount | 1 |
| Binding post | 4mm banana, nylon | 1 |
| Feedpoint box | 3D printed (feedpoint_box.scad) | 1 |
| FT-240-31 toroid | For choke balun | 1 |
| RG-174 coax | 0.6 m (choke winding) | 1 |
| Hardware | M3 screws, standoffs | Assorted |

### 7.2 Feedpoint Box Assembly

1. Print feedpoint_box.scad in PETG (0.4 mm nozzle, 3 perimeters, 40% infill).
2. Line interior walls with adhesive copper tape for RF shielding; solder tape seams.
3. Install SMA panel mount (front) and binding post (rear).
4. Wind choke balun: 8 turns RG-174 through FT-240-31 core; fit inside box or mount externally on coax.
5. Wind series inductor toroid per Appendix B; mount on standoff.
6. Install fixed NP0 cap and variable trimmer on board or direct-mounted; connect in parallel.
7. Wire: coax center → choke input → L1 series → junction → binding post + C_fixed + C_trim → ground/coax shield.
8. Close lid with M3 screws.

### 7.3 Element Deployment

**Vertical mounting (recommended):**
- Insert 10–12m fiberglass mast or painter's pole as support
- Attach feedpoint box at base
- Route antenna wire vertically up mast; secure with cable ties
- Deploy 2–4 radials horizontally on ground or at 45° downward if elevated

**Sloping (field expedient):**
- Attach feedpoint to support at 3–5m height
- Run wire at 30–45° angle toward available anchor point
- Trim wire if resonant frequency is too low (wire too long)
- Add wire if too high

### 7.4 Wire Length Adjustment

NEC models are calculated for the exact quarter-wave. In practice, field conditions require minor adjustment:

```
Wire too long (resonance below target): Trim 5–10 cm at tip; recheck SWR.
Wire too short (resonance above target): Add a wire extension; twist-connect at tip.
Target: SWR minimum at operating frequency.
```

---

## CHAPTER 8: CONSTRUCTION — ROLL-UP TRAPPED MULTIBAND ANTENNA

### 8.1 Roll-Up Antenna Design Summary

The roll-up antenna covers 40m through 10m (5 bands) from a single 10.18 m wire with four inline LC traps. The feedpoint box provides the L-network matching, which is trimmed once per band segment.

Total packed dimensions:
- Wire spool: 160 mm diameter × 55 mm width
- Feedpoint box: 70 × 50 × 38 mm
- 5 trap housings: 30 × 22 × 24 mm each
- Total kit mass: approximately 350 g

### 8.2 Trap Assembly Order

Traps are installed in the wire **starting from the feedpoint end**, with the highest-frequency trap nearest the feedpoint:

| Position | Trap | Distance from feedpoint | Core | Turns | C |
|----------|------|------------------------|------|-------|---|
| 1 | 10m (28.5 MHz) | 2.55 m | T50-6 | 3T | 82 pF |
| 2 | 12m (24.9 MHz) | 2.92 m | T50-6 | 3–4T | 100 pF |
| 3 | 15m (21.2 MHz) | 3.43 m | T50-6 | 4T | 120 pF |
| 4 | 20m (14.175 MHz) | 5.13 m | T50-6 | 5T | 120+6.8 pF |
| End | — | 10.18 m | — | — | — |

**Wire preparation:**
1. Cut a single wire to 10.18 m total length.
2. Mark positions at 2.55, 2.92, 3.43, and 5.13 m from the feedpoint end.
3. At each mark, cut the wire; install the trap housing; solder wire ends to winding ends inside housing; close housing.
4. After all four traps are installed, total wire will measure 10.18 m from feedpoint end to tip (the trap housings add small lengths; allow 2–3 cm per trap in the initial wire length).

### 8.3 Verification Procedure

After assembly, verify each trap's resonant frequency before winding onto the spool:

1. Using an antenna analyzer (NanoVNA), connect across each trap with the wire ends open.
2. Sweep frequency; locate impedance peak (|Z| maximum).
3. Compare to target frequency (see table above).
4. Adjust turns or capacitor if >±100 kHz off target.

### 8.4 Field Deployment

1. **Identify operating band** using band selector button or CYD display.
2. **Deploy spool:** Unwind wire from spool, letting trap housings deploy freely.
3. **Raise antenna:** Attach feedpoint box to support at 3–5 m height; run wire vertically.
4. **Deploy radials:** Uncoil two counterpoise wires (each ≈ length of active section for band); lay on ground or hold elevated.
5. **Connect coax:** Attach 50 Ω coaxial feed to SMA connector on feedpoint box.
6. **Power on tuner:** Connect ESP32 SWR monitor; observe CYD display.
7. **Trim for minimum SWR:** Adjust variable capacitor using trimmer screwdriver; rotate slowly while monitoring SWR bar.
8. **Confirm SWR < 1.5:1** before transmitting at full power.

---

## CHAPTER 9: CONSTRUCTION — VHF/UHF ELEMENTS

### 9.1 6m and 2m (Gamma Match)

**Element materials:**
- 6m: 6 mm aluminum or copper rod, 1.40 m
- 2m: 3 mm aluminum rod, 0.488 m
- Gamma rod: same material, 15% of element length
- Ground plane: 4 rigid radials same material, bent 45° downward

**Construction:**
1. Machine or cut element to specified length.
2. Drill and tap base for SO-239 (UHF) or N-type connector.
3. Cut gamma rod; form a bracket to hold at 12–15 mm spacing.
4. Add compression-type Teflon trimmer (C_gamma) between rod tip and coax center.
5. Attach four radials at base; bend to 45°; solder or bolt to connector shell.
6. Adjust per Section 3.2.

### 9.2 70cm and 33cm (Direct Feed)

**Construction:**
- 70cm: 6 mm aluminum tube, 0.160 m, N-type connector
- 33cm: PCB trace or machined element, SMA connector
- Sleeve balun fabricated from copper tube; length per Section 3.4

**NEC-2 accuracy note:** NEC-2 becomes inaccurate above approximately 300 MHz due to segment length constraints. The 33 cm and 20 cm models in `efqw_vhf_uhf.nec` are provided for reference only. Verify experimentally with an antenna analyzer and trim element length as needed.

### 9.3 33cm and 20cm Band Notes

These bands (902 MHz and 1.27 GHz) are used primarily for:
- ATV (amateur television)
- Weak-signal EME (Earth-Moon-Earth)
- Digital modes (Winlink, APRS via satellite)

Direct coaxial connection is standard. The element length is critical to within ±1 mm. Use SMA or N-type connectors with PTFE dielectric; avoid BNC at these frequencies.

---

## CHAPTER 10: NEC MODELING AND SIMULATION

### 10.1 NEC-2 Overview

The Numerical Electromagnetics Code (NEC-2) is a method-of-moments solver for wire antenna problems. It computes current distribution on wire structures and calculates far-field radiation patterns, feedpoint impedance, and gain.

**Key NEC-2 cards used in this project:**

| Card | Function |
|------|----------|
| CM | Comment |
| CE | Comment end |
| GW | Wire segment definition |
| GE | Geometry end |
| GN | Ground specification |
| EX | Excitation (feed source) |
| LD | Load (trap, coil, capacitor) |
| FR | Frequency sweep |
| RP | Radiation pattern request |
| EN | End |

### 10.2 GW Card Format

```
GW  tag  segs  x1  y1  z1  x2  y2  z2  radius

tag:    wire tag number (integer)
segs:   number of segments (use ≥10 per half-wavelength; odd numbers preferred for center-feed)
x1,y1,z1: start point in meters
x2,y2,z2: end point in meters
radius:  wire radius in meters
```

### 10.3 LD Card — Trap Modeling

NEC-2 does not natively support parallel LC traps. Instead, LDTYP=0 (series RLC) approximates the trap behavior near its resonant frequency:

```
LD  LDTYP  LDTAG  LDMIN  LDMAX  ZL_real  ZL_imag

LDTYP=0: series R, L, C load
ZL_real: resistance in ohms
ZL_imag: reactance in ohms (positive=inductive, negative=capacitive)

For parallel resonant trap (series approximation):
  At resonance: Z ≈ Q²R (purely resistive, high)
  Model as series R = Q²×R_coil with zero reactance

This approximation is valid only near resonance. For precise
multiband modeling, use EZNEC or NEC-4 with parallel LC loads.
```

### 10.4 GN Card — Ground Specification

```
GN  IPERF  NRADL  BLANK  BLANK  EPSE  SIGE

IPERF=1: Perfect ground (theoretical, best case)
IPERF=2: Real ground, finite conductivity
IPERF=-1: Free space (no ground)

Typical real ground values:
  Average soil:   EPSE=13, SIGE=0.005
  Good soil:      EPSE=20, SIGE=0.030
  Sea water:      EPSE=80, SIGE=5.000
  Poor/rocky:     EPSE=7,  SIGE=0.001
```

### 10.5 Interpreting NEC Output

**Feedpoint impedance:** Look for the "ANTENNA INPUT PARAMETERS" section in the output. The complex impedance Z = R + jX. The imaginary part (X) should be near zero at resonance.

**SWR calculation from NEC:**
```
ρ = |Z − Z₀| / |Z + Z₀|   where Z₀ = 50 Ω
SWR = (1 + ρ) / (1 − ρ)
```

**Gain figures:** NEC reports gain in dBi (relative to isotropic). Gain in dBd (relative to dipole) = dBi − 2.15 dB.

### 10.6 Recommended NEC Software

| Software | Platform | Notes |
|----------|----------|-------|
| 4nec2 | Windows | GUI, free, NEC-2 engine |
| EZNEC | Windows | Commercial, excellent GUI |
| nec2c | Linux | Command line, open source |
| Xnec2c | Linux | GUI for nec2c |
| OpenEMS | Cross-platform | Full-wave FEM, for UHF+ |

Running the provided .nec files: `nec2c -i efqw_40m.nec -o efqw_40m.out`

---

## CHAPTER 11: ELECTRONIC INSTRUMENTATION

### 11.1 ESP32 SWR Monitor

The ESP32 SWR monitor reads two AD8307 logarithmic detectors connected to the forward and reflected ports of the directional coupler.

**Key firmware functions (esp32_antenna_tuner.ino):**

| Function | Description |
|----------|-------------|
| `readADC_mV()` | 64× oversampled ADC read, returns millivolts |
| `ad8307_to_dbm()` | Converts AD8307 voltage to dBm with coupler correction |
| `measureSWR()` | Averages 8 readings, computes ρ, SWR, return loss, watts |
| `updateLEDs()` | Sets green/yellow/red LEDs per SWR thresholds |
| `sendBTPacket()` | Streams CSV-format packet to CYD display over BT |
| `handleRoot()` | Serves HTML SWR display page at 192.168.4.1 |
| `handleApiStatus()` | REST JSON status endpoint |
| `handleApiCal()` | GET/POST calibration offset in dB |

**NVS persistence:** Band index and calibration offsets are saved to non-volatile storage and restored on power-up.

### 11.2 CYD Display

The CYD (Cheap Yellow Display, ESP32-2432S028) receives data from the ESP32 tuner via Bluetooth Serial and displays it on a 320×240 ILI9341 TFT touchscreen.

**Pages:**

| Page | Content |
|------|---------|
| SWR | Large SWR reading, colored SWR bar (green/yellow/red), band/frequency, return loss |
| POWER | Forward power (W and dBm), reflected dBm, reflection coefficient |
| GUIDE | Band-appropriate tuning instructions (HF L-net, VHF gamma, UHF direct) |

**Touch:** Tap bottom tab bar to switch pages. No other touch interaction required.

**Auto-reconnect:** CYD attempts BT reconnect every 5 seconds if connection is lost. BT device name: "EFQW-Tuner".

### 11.3 Web Interface

Connect any WiFi device (phone, laptop) to SSID "EFQW-Tuner" (password: antenna73). Navigate to `http://192.168.4.1` for the HTML SWR display. The page auto-refreshes when you click REFRESH or the band buttons.

**API endpoints:**

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/` | GET | HTML SWR display |
| `/band/up` | GET | Increment band index |
| `/band/dn` | GET | Decrement band index |
| `/api/status` | GET | JSON status |
| `/api/cal` | GET/POST | Calibration offsets |

---

## CHAPTER 12: CALIBRATION AND FIELD SETUP

### 12.1 AD8307 Calibration

The AD8307 output is factory-specified at 25 mV/dB slope with −84 dBm intercept. In practice, slope varies ±5% and intercept shifts with supply voltage. For accurate power readings:

1. Connect a known RF source (signal generator or GPSDO calibration system) to the coupler input.
2. Set source power to a known value (e.g., −10 dBm).
3. Read AD8307 forward output voltage.
4. Calculate expected voltage: V_expected = 25 × (−10 + 84) = 1,850 mV (after coupler correction).
5. Calculate actual power: P_actual = (V_measured / 25) − 84 + Coupler_dB.
6. Set cal_fwd offset = known_power − calculated_power via POST to /api/cal.

### 12.2 Coupler Calibration

The Bruene coupler coupling factor depends on the toroid and secondary winding. Measure empirically:

1. Connect 50 Ω dummy load to antenna port.
2. Apply known power (e.g., 1.00 W = +30.0 dBm) at RF input.
3. Read V_fwd_mV from AD8307 #1.
4. P_detected = (V_fwd_mV / 25) − 84.
5. Coupler_factor_dB = +30.0 − P_detected.
6. Update COUPLER_FWD_DB in config.h.

### 12.3 Initial Field Setup Checklist

```
□ Feedpoint box components installed and wired
□ Choke balun wound and tested (check common-mode impedance ≥1 kΩ)
□ Traps verified at resonant frequency (±100 kHz)
□ Wire lengths measured and cut per Appendix A
□ Radials cut to matching length per band
□ ESP32 programmed; WiFi AP confirmed at 192.168.4.1
□ CYD paired to EFQW-Tuner BT
□ Dummy load SWR check: SWR = 1.0:1 with 50 Ω load connected
□ Antenna deployed vertical, 3–5 m height if possible
□ Radials deployed, all connections tight
□ Trim C2 for SWR minimum; confirm < 1.5:1 on operating band
□ Log SWR, band, time, location in field notebook
```

---

## CHAPTER 13: TROUBLESHOOTING

### 13.1 No SWR Reading

**Symptom:** CYD shows "NO RF" or SWR = 0.

1. Verify transmitter is in TX mode (carrier or CW tone).
2. Verify RF cable connections at coupler input.
3. Check AD8307 supply voltage: should be 2.7–5.5 V (use 3.3 V from ESP32).
4. Check AD8307 INLO pin: must be grounded for differential input.
5. Measure AD8307 output pin with multimeter while transmitting. Should be 0.5–2.0 V.
6. Check ESP32 ADC pins (GPIO35, GPIO39). These are input-only on ESP32.

### 13.2 SWR Shows 1.0:1 with Antenna Connected

**Symptom:** SWR appears perfect regardless of antenna condition.

This typically means the reflected detector is not working. Check:
1. D2 orientation (reversed vs. D1).
2. R3, R4 values (should be 51 Ω, not 0 Ω or open).
3. C2 capacitor value and connection.
4. AD8307 #2 wiring and supply.

### 13.3 High SWR on All Bands

1. **Check choke balun:** Remove balun; if SWR changes significantly, balun is the problem (or it was working and is now failing).
2. **Check coax:** Substitute a short known-good coaxial jumper.
3. **Check ground connection:** RF ground (counterpoise) must be connected at feedpoint box ground terminal.
4. **Measure element length:** Long wire = resonance below target = high SWR near band center.

### 13.4 SWR High on One Band Only

1. **Trap mistuned:** Verify that band's trap resonates at correct frequency.
2. **Wire section length error:** Measure wire from feedpoint to that band's trap position.
3. **Trap housing cracked or damaged:** Inspect; reseal with RTV if contaminated.
4. **Variable capacitor range:** Check that C_fixed + C_variable includes the required value for that band.

### 13.5 CYD Not Connecting

1. Verify ESP32 BT is running (check serial monitor: "BT: EFQW-Tuner").
2. Delete paired device from CYD; allow it to re-scan.
3. ESP32 and CYD must be within approximately 10 m with no obstructions.
4. Verify CYD firmware was built with `BluetoothSerial` library included.

---

## CHAPTER 14: PARTS REFERENCE

### 14.1 Critical Components

| Component | Description | Source |
|-----------|-------------|--------|
| AD8307 | Log amp, SOT-23 or DIP-8 | Analog Devices; Mouser/Digi-Key |
| 1N5711 | Schottky detector diode | Mouser; equivalent: BAT43 |
| FT-37-43 | Toroid core, Mix 43, 0.37" | Amidon; Fair-Rite |
| FT-240-31 | Toroid core, Mix 31, 2.40" | Amidon (for choke balun) |
| T50-6 | Toroid core, Mix 6 (yellow), 0.50" | Amidon |
| T50-2 | Toroid core, Mix 2 (red), 0.50" | Amidon |
| T68-2 | Toroid core, Mix 2 (red), 0.68" | Amidon |
| NP0 capacitors | 500V rating for HF traps | Mouser; Vishay VJ series |
| Variable trimmer | 0–500 pF, screwdriver adjust | Murata TZC3 series |
| ESP32-WROOM-32 | MCU module | DigiKey; Mouser |
| CYD (ESP32-2432S028) | Display board | AliExpress "Cheap Yellow Display" |

### 14.2 Wire Specifications

| Band | Wire | AWG | Min. rated W |
|------|------|-----|-------------|
| 160m–80m | Stranded copper, THHN | 14 | 500 W |
| 60m–20m | Stranded copper, THHN | 16 | 300 W |
| 17m–10m (roll-up) | Stranded, flexible | 18 | 150 W |
| 6m | Solid/stranded | 16–18 | 100 W |
| 2m–1.25m | 3mm Al rod | — | 50 W |
| 70cm–33cm | 6mm Al tube or rod | — | 25 W |

---

## APPENDIX A: WINDING TABLES — TRAPS

See `/home/merv/Public/claude/EF/calculations/trap_winding_tables.csv` for complete data.

Summary of key values:

| Band | f₀ (MHz) | Core | N (turns) | C (pF) | Z_trap (Ω) | Q | Position (m) |
|------|---------|------|----------|-------|-----------|---|-------------|
| 160m | 1.900 | T68-2 | 16 | 470 | 4,800 | 180 | (fixed ant) |
| 80m  | 3.750 | T68-2 | 12 | 180 | 5,600 | 195 | (fixed ant) |
| 60m  | 5.330 | T50-2 | 9  | 200 | 5,900 | 200 | (fixed ant) |
| 40m  | 7.150 | T50-2 | 7  | 220 | 7,400 | 230 | (fixed ant) |
| 30m  | 10.125 | T50-2 | 5 | 220 | 7,800 | 235 | (fixed ant) |
| 20m  | 14.175 | T50-6 | 5 | 120+6.8 | 9,100 | 250 | 5.13 |
| 17m  | 18.118 | T50-6 | 4 | 120 | 9,600 | 255 | 4.01 |
| 15m  | 21.225 | T50-6 | 4 | 100 | 9,800 | 255 | 3.43 |
| 12m  | 24.940 | T50-6 | 3 | 120 | 10,000 | 258 | 2.92 |
| 10m  | 28.500 | T50-6 | 3 | 82 | 10,200 | 260 | 2.55 |

---

## APPENDIX B: WINDING TABLES — MATCHING INDUCTORS

See `/home/merv/Public/claude/EF/calculations/resonator_winding_tables.csv` for complete data.

| Band | f (MHz) | Core | N | L (µH) | C_fixed (pF) | C_var range |
|------|---------|------|---|--------|-------------|------------|
| 160m | 1.900 | T68-2 | 58T | 1.920 | 820+220 | 0–100 |
| 80m  | 3.750 | T68-2 | 42T | 0.973 | 470 | 0–100 |
| 40m  | 7.150 | T50-2 | 23T | 0.510 | 220+68 | 0–50 |
| 20m  | 14.175 | T50-6 | 11T | 0.257 | 120+27 | 0–30 |
| 10m  | 28.500 | T50-6 | 6T  | 0.128 | 56+18 | 0–20 |

VHF/UHF uses gamma match (Chapter 3.2) or direct feed (Chapter 3.3); no matching inductor required.

---

## APPENDIX C: NEC CARD REFERENCE

```
CM  Comment line
CE  End of comments
GW  tag segs x1 y1 z1 x2 y2 z2 rad   Wire segment
GE  0/1      End geometry (1=ground plane symmetry)
GN  type ...  Ground (1=perfect, 2=real, -1=free space)
EX  0 tag seg 0 Vr Vi    Voltage source excitation
LD  0 tag s1 s2 R L C    Series RLC load
FR  0 nf 0 0 f_start f_step   Frequency sweep
RP  0 nth nph 1000 th0 ph0 dth dph   Radiation pattern
EN               End of file
```

---

## APPENDIX D: QUICK REFERENCE CARD

```
┌───────────────────────────────────────────────────────────┐
│           EFQW QUICK REFERENCE                            │
│                                                           │
│  QW Length (m) = 75 × VF / f_MHz                        │
│  VF = 0.97 (wire)  0.95 (rod/tube)                      │
│                                                           │
│  SWR = (1+ρ)/(1−ρ)   ρ = √(P_refl/P_fwd)               │
│  Return Loss = −20 log₁₀(ρ)  [dB]                       │
│                                                           │
│  TRAP RESONANCE: f₀ = 1/(2π√LC)                         │
│  TURNS: N = √(L_µH × 1000 / AL)                         │
│                                                           │
│  SWR ≤ 1.5 → GREEN   (excellent)                        │
│  SWR 1.5–2.5 → YELLOW (acceptable)                      │
│  SWR > 2.5 → RED    (retune)                             │
│                                                           │
│  TUNING HF: Adjust C2 (trimmer) for SWR minimum         │
│  TUNING VHF: Adjust gamma cap + rod position             │
│  TUNING UHF: Trim element length by 2mm steps            │
│                                                           │
│  WiFi: EFQW-Tuner  PW: antenna73  IP: 192.168.4.1       │
│  BT:   EFQW-Tuner                                        │
└───────────────────────────────────────────────────────────┘
```

---

*END OF TM-EF-001 Rev A*

*Prepared by: Merv Martin, W6— / TM-EF-001*  
*All dimensions in meters unless otherwise noted.*  
*Verify all component values with current sources before ordering.*
