# TECHNICAL MANUAL
## POOL NOODLE MAGNETIC LOOP ANTENNA SYSTEM
### TM-PNL-001 Rev A

---

```
DISTRIBUTION STATEMENT A: Approved for public release; distribution unlimited.

This document contains no controlled technical information.
Amateur radio construction project — personal use.
```

---

**Document Number:** TM-PNL-001 Rev A  
**Title:** Pool Noodle Magnetic Loop Antenna System — 160m through 20cm  
**Author:** Merv Martin  
**Date:** 2026-05-08  
**Supersedes:** None (initial release)

**Related Documents:**

| Document       | Title                                          |
|---------------|------------------------------------------------|
| TM-PNL-SCH-001 | Magnetic Loop Equivalent Circuit Schematics   |
| TM-PNL-SCH-002 | Tuning Capacitor Design and Motor Drive        |
| TM-PNL-SCH-003 | Faraday Coupling Loop and Matching Theory      |
| TM-PNL-ENC-001 | Cap Housing OpenSCAD Design                    |
| TM-PNL-ENC-002 | Loop Corner Connector OpenSCAD Design          |
| TM-PNL-ENC-003 | Roll-up Spool OpenSCAD Design                  |
| TM-PNL-FW-001  | ESP32 Loop Tuner Firmware                      |
| TM-PNL-FW-002  | CYD Loop Display Firmware                      |
| TM-PNL-NEC-001 through -015 | NEC-2 Antenna Models              |

---

## ═══════════════════════════════════════════════════════
## DANGER — HIGH VOLTAGE
## ═══════════════════════════════════════════════════════

```
╔══════════════════════════════════════════════════════════════════╗
║                     !! DANGER !!                                 ║
║                                                                  ║
║  THE TUNING CAPACITOR IN THIS ANTENNA SYSTEM STORES LETHAL      ║
║  VOLTAGES DURING TRANSMIT OPERATION.                             ║
║                                                                  ║
║  At 5 Watts output:                                              ║
║    160m band: 2,791 Volts peak across tuning capacitor           ║
║     80m band: 2,379 Volts peak                                   ║
║     40m band: 1,738 Volts peak                                   ║
║                                                                  ║
║  At 100 Watts output — multiply above by 4.47 (√20):            ║
║    160m band: 12,480 Volts peak                                  ║
║     80m band: 10,640 Volts peak                                  ║
║     40m band:  7,770 Volts peak                                  ║
║                                                                  ║
║  MANDATORY SAFETY REQUIREMENTS:                                  ║
║   1. Install 100kΩ / 5W bleeder resistor across tuning cap.     ║
║   2. Never touch capacitor or its leads during TX or for         ║
║      30 seconds after TX ends.                                   ║
║   3. 160m and 80m REQUIRE vacuum capacitors rated ≥ 15kV.       ║
║   4. High-voltage warning LED or red indicator MUST be visible   ║
║      from operating position.                                    ║
║   5. Children and untrained persons MUST NOT operate this        ║
║      antenna system without direct supervision.                  ║
║                                                                  ║
║  FAILURE TO COMPLY MAY RESULT IN DEATH OR SERIOUS INJURY.       ║
╚══════════════════════════════════════════════════════════════════╝
```

---

## TABLE OF CONTENTS

- [Chapter 1: Introduction and Scope](#chapter-1-introduction-and-scope)
- [Chapter 2: Theory of Operation](#chapter-2-theory-of-operation)
- [Chapter 3: Materials and Parts List](#chapter-3-materials-and-parts-list)
- [Chapter 4: Construction — HF Small Transmitting Loop (160m–10m)](#chapter-4-construction--hf-small-transmitting-loop-160m10m)
- [Chapter 5: Construction — VHF/UHF Full-Wave Quad (6m–20cm)](#chapter-5-construction--vhfuhf-full-wave-quad-6m20cm)
- [Chapter 6: Construction — Roll-up Multi-band Version](#chapter-6-construction--roll-up-multi-band-version)
- [Chapter 7: SWR Measurement System](#chapter-7-swr-measurement-system)
- [Chapter 8: ESP32 Auto-Tuner Firmware](#chapter-8-esp32-auto-tuner-firmware)
- [Chapter 9: CYD Display Firmware](#chapter-9-cyd-display-firmware)
- [Chapter 10: Tuning and Alignment](#chapter-10-tuning-and-alignment)
- [Chapter 11: Performance Data](#chapter-11-performance-data)
- [Chapter 12: Safety — High Voltage Critical Section](#chapter-12-safety--high-voltage-critical-section)
- [Chapter 13: Maintenance and Field Repair](#chapter-13-maintenance-and-field-repair)
- [Appendix A: Band Data Sheets](#appendix-a-band-data-sheets)
- [Appendix B: Bill of Materials](#appendix-b-bill-of-materials)
- [Appendix C: NEC Model Summary](#appendix-c-nec-model-summary)
- [Appendix D: Capacitor Voltage Safety Reference](#appendix-d-capacitor-voltage-safety-reference)
- [Appendix E: Troubleshooting Guide](#appendix-e-troubleshooting-guide)
- [Appendix F: References and Standards](#appendix-f-references-and-standards)

---

## CHAPTER 1: INTRODUCTION AND SCOPE

### 1.1 Purpose

This technical manual provides complete design, construction, calibration, and operating instructions for the Pool Noodle Magnetic Loop Antenna System covering all nine HF amateur bands (160 meters through 10 meters) and six VHF/UHF bands (6 meters through 20 centimeters).

The system uses commercially available pool noodles (polyethylene foam tube forms, OD = 4.7 cm, ID = 2.5 cm, length = 120 cm) as the structural substrate for wound wire loops. The pool noodle is an ideal RF structural material: low mass, low cost, electrically inert, weather resistant, and easily cut and shaped in the field.

### 1.2 Scope

This manual covers:

1. **Fixed-band HF Small Transmitting Loop (STL)** — Single-band magnetic loops for 160m through 10m, motor-tuned with a high-voltage air-variable (or vacuum) capacitor.

2. **Fixed-band VHF/UHF Full-Wave Resonant Quad** — Full-wave quad loops for 6m through 20cm (1296 MHz), pool noodle frame, no tuning capacitor required.

3. **Roll-up Multi-band Portable Version** — Multi-band loop using LC traps to select operating band; field-portable assembly on spool form.

4. **Motor Control System** — 28BYJ-48 stepper motor with 4:1 external geardown driving the tuning capacitor shaft, controlled by ESP32-WROOM-32 with Bruene directional coupler SWR sensing.

5. **Display System** — CYD (ESP32-2432S028R with ILI9341 TFT) receiving telemetry via Bluetooth serial.

### 1.3 Design Philosophy

The magnetic loop antenna offers unique advantages for constrained operating environments:

- **Small size relative to wavelength:** Loop circumference = 0.10 to 0.25λ (HF bands). For 40m, total wire length ≈ 4.8m vs. 20m for a half-wave dipole.
- **Directional null pattern:** The figure-eight horizontal pattern (for vertical loop) provides front-to-side rejection of ~15 dBi, useful for RFI nulling.
- **Low susceptibility to near-field objects:** Magnetic coupling mode rejects electric-field noise (power line hash, switching supplies).
- **Self-contained resonator:** The loop is its own LC tank. No external antenna tuner required once set.

The primary limitations:
- **Narrow bandwidth:** 3 dB bandwidth is 2–15 kHz on HF (Q = 600–1100). Retuning required for frequency changes > 5–10 kHz.
- **Efficiency lower than full-size antennas:** 13–66% on HF bands (10m best, 160m worst). Compensated by low-noise operation.
- **High circulating capacitor voltage:** Lethal at legal power levels. Safety is mandatory, not optional.

### 1.4 Naming and Document Control

All documents in this system use the prefix **TM-PNL** (Technical Manual, Pool Noodle Loop):

```
TM-PNL-001       This manual
TM-PNL-SCH-001   Magnetic loop equivalent circuit
TM-PNL-SCH-002   Tuning capacitor design
TM-PNL-SCH-003   Faraday coupling and matching
TM-PNL-ENC-001   Capacitor housing (OpenSCAD)
TM-PNL-ENC-002   Loop corner connector (OpenSCAD)
TM-PNL-ENC-003   Roll-up spool (OpenSCAD)
TM-PNL-FW-001    ESP32 tuner firmware
TM-PNL-FW-002    CYD display firmware
TM-PNL-NEC-001 to -015  NEC-2 antenna models (per band)
```

### 1.5 Safety Summary

**Before beginning construction or operation, read Chapter 12 in its entirety.** Key points:

- Tuning capacitors on 160m and 80m store voltages exceeding household current by a factor of 50. Death is a credible outcome of accidental contact.
- Bleeder resistors are mandatory. Do not omit them.
- The 28BYJ-48 motor must be de-energized between tuning operations. Continuously energized coils generate heat that can degrade the capacitor housing.
- For 6m and above (LOOP_QUAD type), no capacitor is used and high-voltage hazards do not apply.

---

## CHAPTER 2: THEORY OF OPERATION

### 2.1 The Small Transmitting Loop (STL)

A Small Transmitting Loop is a resonant loop antenna whose circumference is substantially less than one wavelength. For this design, the circumference is:

```
  Circumference = 4 × side_length

  Band    Circumference    % of Wavelength
  ------  -------------    ---------------
  160m    8.0 m            5.1%
  80m     6.0 m            7.5%
  40m     4.8 m            11.5%
  30m     4.0 m            13.3%
  20m     3.6 m            17.1%
  17m     3.2 m            19.5%
  15m     2.8 m            19.8%
  12m     2.4 m            19.9%
  10m     2.0 m            19.0%
```

Because the circumference is much less than λ, the current distribution around the loop is approximately uniform. The antenna behaves as a lumped magnetic dipole moment.

### 2.2 Loop Inductance

For a single-turn square loop of side `s` made from wire of radius `a`:

```
  L₁ = (2μ₀/π) × s × [ln(2s/a) − 0.774]    [H]

  where:
    μ₀ = 4π × 10⁻⁷  H/m  (permeability of free space)
    s  = loop side length  [m]
    a  = wire radius       [m]
    0.774 = Neumann correction for square loops
```

For a multi-turn loop (N turns, using Nagaoka correction factor k_N ≈ 0.85 for tightly wound coils):

```
  L_N = N² × L₁ × k_N    [H]
```

The N² dependence means that doubling the number of turns quadruples inductance — but also quadruples radiation resistance, which grows as N⁴, making multi-turn loops significantly more efficient for low-band operation.

### 2.3 Radiation Resistance

The radiation resistance of an N-turn loop with area A = s² at frequency f is:

```
  R_rad = 197 × (4Ns/λ)⁴    [Ω]

  where:
    N  = number of turns
    s  = loop side          [m]
    λ  = wavelength = c/f   [m]
    c  = 2.998 × 10⁸ m/s
```

Note the fourth-power dependence on electrical size. Doubling N increases R_rad by 16×. This is the dominant reason multi-turn loops are used on 160m and 80m.

### 2.4 Loss Resistance

Loss resistance arises from resistive losses in the loop conductor. At RF, current flows in a thin skin depth layer:

```
  Skin depth:
  δ = √(ρ / (π × f × μ₀))    [m]

  For copper (#14 AWG, a = 0.000814 m, ρ = 1.72 × 10⁻⁸ Ω·m):
  At 7.150 MHz:  δ ≈ 25 μm

  Loss resistance for N-turn loop:
  R_loss = (4Ns) / (σ × 2πa × δ)    [Ω]

  where σ = 1/ρ = electrical conductivity [S/m]
```

### 2.5 Efficiency, Q, and Bandwidth

```
  Radiation efficiency:
  η = R_rad / (R_rad + R_loss) × 100%    [%]

  Q factor:
  Q = ω × L_N / R_loss    (reactive impedance / loss resistance)

  3-dB Bandwidth:
  BW = f₀ / Q    [Hz]

  where ω = 2π × f₀
```

The Q of a magnetic loop is typically 600–1100 on HF, giving bandwidths of 2–40 kHz. This narrow bandwidth is both advantage (selectivity against interference) and limitation (frequent retuning).

### 2.6 Resonating Capacitor

To bring the loop to resonance at target frequency f₀:

```
  ω₀ = 2π × f₀

  L_N × C = 1/ω₀²

  C = 1 / (ω₀² × L_N)    [F]
```

The capacitor forms a parallel LC tank with the loop inductance. At resonance, the circulating current in the tank is Q times the feed current. This high circulating current through the capacitor generates the high voltages described in Chapter 12.

### 2.7 Capacitor Voltage

The peak voltage across the tuning capacitor at transmit power P is:

```
  Total RF current in loop:
  I = √(P / (R_rad + R_loss))    [A rms]

  Peak (instantaneous) current:
  I_peak = I × √2

  Capacitor reactance at resonance:
  X_C = 1 / (ω₀ × C) = ω₀ × L_N    [Ω]

  (At resonance, X_C = X_L)

  Peak voltage:
  V_peak = I_peak × X_C    [V]
```

Example: 40m band, P = 5W, L_N = 23.55 μH, f₀ = 7.150 MHz:
```
  X_C = 2π × 7.15 × 10⁶ × 23.55 × 10⁻⁶ = 1,058 Ω
  I = √(5 / (Rrad + Rloss)) ≈ √(5 / 1.16) = 2.07 A rms
  I_peak = 2.07 × 1.414 = 2.93 A peak
  V_peak = 2.93 × 1,058 = 3,100 V peak
```

**At 100W, V_peak scales by √(100/5) = 4.47, giving ≈ 13,857V peak on 40m.**

### 2.8 Faraday Coupling Loop

The Faraday (transformer) coupling method uses a small inner loop to feed the main loop by mutual inductance. This is the preferred feed method for magnetic loops because:

1. **Balanced feed:** The coupling loop is a closed loop; the coax connects to it via a balanced-to-unbalanced arrangement, reducing common-mode current.
2. **No direct galvanic connection** to the high-impedance main loop.
3. **Adjustable:** Sliding the coupling loop changes Z_in continuously.

**Mutual Inductance:**
```
  M = k × √(L_main × L_coupling)    [H]

  where k = coupling coefficient
        k ≈ (s_c / s_m)²  for coplanar square loops
        s_c = coupling loop side
        s_m = main loop side
```

**Input Impedance at Resonance:**
```
  Z_in = (ω₀M)² / (R_rad + R_loss)    [Ω]

  For 50Ω match:
  ω₀M = √(50 × (R_rad + R_loss))
```

**The 1/5 Rule:** When s_c = s_m / 5 (coupling loop side = 1/5 of main loop side), Z_in ≈ 50Ω at resonance for typical #14 AWG copper loops. This is the starting point; fine adjustment by position achieves actual SWR ≤ 1.5:1.

### 2.9 LC Trap Theory

For the roll-up multi-band version, LC traps isolate loop sections at specific frequencies. A trap is a parallel resonant LC circuit that presents very high impedance at its resonant frequency, electrically disconnecting the loop conductor beyond that point.

```
  Trap resonance:
  f_trap = 1 / (2π × √(L_trap × C_trap))

  Trap impedance at resonance (parallel RLC):
  Z_trap = Q_trap × ω_trap × L_trap    [Ω]

  Target: Z_trap > 2000Ω at resonant frequency
```

Trap Q must be maximized. For this design, toroids are used rather than air-core coils:

- **T50-2 (powdered iron, μ=10):** AL = 4 nH/turn² → used for 20m–40m traps
- **T50-6 (powdered iron, μ=8.5):** AL = 3 nH/turn² → used for 10m–17m traps

The T50 series has Q > 150 at HF, giving Z_trap > 3500Ω — sufficient to provide clean band separation.

### 2.10 Full-Wave Resonant Quad (VHF/UHF)

Above 6 meters, the pool noodle loop becomes a full-wavelength resonant loop:

```
  Circumference = λ × VF = (c / f) × 0.95

  Side length = Circumference / 4    [m]

  Example: 2m band (146 MHz):
  Circumference = (299.8 × 10⁶ / 146 × 10⁶) × 0.95 = 1.952 m
  Side = 1.952 / 4 = 0.488 m (48.8 cm)
```

A full-wave square loop has input impedance ≈ 125Ω in free space. Matching to 50Ω coax requires either:

1. **2.5:1 UNUN transformer** (FT-50-61 toroid, 7T primary / 11T secondary)
2. **λ/4 transformer** at 79Ω (≈75Ω coax, RG-59)
3. **Gamma match** (preferred for field adjustment)

No tuning capacitor is required. The VHF/UHF quads operate at the fixed design frequency; no retuning needed within the band.

### 2.11 Directional Coupler Theory (Bruene Reflectometer)

The SWR measurement system uses a Bruene directional coupler (transmission line transformer type) to sample forward and reflected power separately. This is the same architecture used in commercial SWR bridges and in naval RF transmission systems.

**Operating Principle:**

The coupler samples the transmission line at a point. It uses two coupled mechanisms:
1. **Voltage sampling** via capacitive coupling to the transmission line
2. **Current sampling** via magnetic coupling through a toroidal transformer

These two samples are combined in phase for the forward detector and in anti-phase for the reflected detector:

```
  V_forward  = V_line × k_v + I_line × Z₀ × k_i    [proportional to forward wave]
  V_reflected = V_line × k_v − I_line × Z₀ × k_i    [proportional to reflected wave]

  where:
    k_v = voltage coupling coefficient
    k_i = current coupling coefficient
    Z₀  = characteristic impedance (50Ω)
```

When k_v = k_i (balanced coupling), V_reflected = 0 when the load is matched (Z_load = Z₀). Any mismatch produces a reflected sample.

**AD8307 Log Amplifier:**

The sampled voltages are rectified and applied to AD8307 logarithmic amplifiers. The AD8307 produces a DC output voltage proportional to input power in dBm:

```
  V_out = 25 mV/dB × (P_in_dBm + 84)    [mV]

  Inverting:
  P_dBm = (V_mV / 25.0) − 84.0

  P_watts = 10^(P_dBm/10) / 1000
```

The coupler coupling factor is 20 dB. The measured power is corrected by this factor:

```
  P_forward_watts = 10^((P_fwd_dBm + 20)/10) / 1000

  where P_fwd_dBm is the AD8307 output converted to dBm
```

**SWR Calculation:**

```
  Reflection coefficient:
  Γ = √(P_reflected / P_forward)

  SWR:
  SWR = (1 + |Γ|) / (1 − |Γ|)
```

**Valid Measurement Range:** The AD8307 has a dynamic range of approximately 92 dB (−84 dBm to +8 dBm). Minimum forward power for a valid SWR reading in this system: P_fwd > −20 dBm (approximately 10 μW). Below this level, noise dominates and the SWR reading is marked invalid.

---

## CHAPTER 3: MATERIALS AND PARTS LIST

### 3.1 Pool Noodle Stock

| Item | Specification | Quantity | Notes |
|------|--------------|----------|-------|
| Pool noodle | OD = 47mm, ID = 25mm, L = 1200mm | 10 total | Polyethylene foam; electrical properties: εr ≈ 1.05, μr = 1.0, tan δ < 0.001 (negligible RF effect) |

Pool noodles are sold in 120 cm segments. Select solid-color (non-patterned) noodles; some patterned variants have embedded foil decorations that can cause RF coupling. Test by bending: good PE foam is resilient, not brittle.

### 3.2 Loop Wire

| Band Range | Wire Gauge | Type | AWG | Outer Dia. | Notes |
|-----------|-----------|------|-----|-----------|-------|
| 160m–40m  | #12 AWG   | Bare copper or stranded | 2.05mm dia | — | Larger wire reduces Rloss |
| 30m–15m   | #14 AWG   | Bare copper | 1.63mm dia | — | Standard choice |
| 12m–10m   | #16 AWG   | Bare copper | 1.29mm dia | — | Acceptable for QRP |
| Coupling loops | #18 AWG | Bare copper | 1.02mm dia | — | All bands |

Wire lengths per band (single-turn main loop circumference plus 20% slack):

| Band | Main Loop Perimeter | Wire Required | N Turns | Total Wire |
|------|-------------------|---------------|---------|-----------|
| 160m | 8.00 m | 9.6 m | 4 | 38.4 m |
| 80m  | 6.00 m | 7.2 m | 3 | 21.6 m |
| 40m  | 4.80 m | 5.8 m | 3 | 17.4 m |
| 30m  | 4.00 m | 4.8 m | 2 | 9.6 m |
| 20m  | 3.60 m | 4.3 m | 2 | 8.6 m |
| 17m  | 3.20 m | 3.8 m | 1 | 3.8 m |
| 15m  | 2.80 m | 3.4 m | 1 | 3.4 m |
| 12m  | 2.40 m | 2.9 m | 1 | 2.9 m |
| 10m  | 2.00 m | 2.4 m | 1 | 2.4 m |

### 3.3 Tuning Capacitors — HF Bands

```
WARNING: Capacitor voltage ratings must EXCEED V_cap values in Appendix D.
         For 160m and 80m: vacuum capacitors only.
         Never substitute a lower-voltage component.
```

| Band | C Required (pF) | V_cap @ 5W | Type Required | Min V Rating |
|------|---------------|-----------|--------------|-------------|
| 160m | 41.7 | 2,791 V | Vacuum variable | 15 kV |
| 80m  | 26.4 | 2,379 V | Vacuum variable | 12 kV |
| 40m  | 21.0 | 1,738 V | Air-variable, wide spacing | 8 kV |
| 30m  | 12.9 | 1,775 V | Air-variable | 8 kV |
| 20m  | 7.4  | 1,682 V | Air-variable | 8 kV |
| 17m  | 20.8 | 957 V  | Silver mica / NP0 | 2 kV |
| 15m  | 17.7 | 966 V  | Silver mica / NP0 | 2 kV |
| 12m  | 15.3 | 969 V  | Silver mica / NP0 | 2 kV |
| 10m  | 14.5 | 963 V  | Silver mica / NP0 | 2 kV |

Recommended variable capacitor sources:
- **Vacuum:** Russian surplus KMEK or KPEK series (available via eBay, RF Parts Co.)
- **Air-variable:** Hammarlund HF-150, or any "butterfly" split-stator type with wide plate spacing

### 3.4 Motor System

| Component | Part | Quantity | Notes |
|-----------|------|----------|-------|
| Stepper motor | 28BYJ-48, 5V, 1/64 gear | 1 per band | 8192 half-steps/output shaft revolution |
| Motor driver | ULN2003 breakout board | 1 per band | Pre-assembled modules with LED indicators preferred |
| Main gear | 40T spur, module 1mm, bore 6mm | 1 | Printed or purchased; see TM-PNL-ENC-001 |
| Motor pinion | 10T spur, module 1mm, fits 28BYJ shaft | 1 | Printed; see TM-PNL-ENC-001 |
| Gear ratio | 4:1 | — | 28BYJ output × 4:1 → cap shaft |
| Half-steps per cap rev | 32,768 | — | 8192 × 4 |
| Total steps full travel | 327,680 | — | Approximately 10 cap revolutions |

### 3.5 Controller Electronics

| Component | Part Number / Spec | Notes |
|-----------|------------------|-------|
| Microcontroller | ESP32-WROOM-32, 4MB flash | Generic "ESP32 Dev Kit" |
| RF coupler IC #1 | AD8307ANZ (forward) | PDIP-8 package preferred for field repair |
| RF coupler IC #2 | AD8307ANZ (reflected) | Same as above |
| Coupling transformer | BN-43-2402 binocular core, 3T secondary | Bruene coupler current sample |
| Coupling capacitor | 33pF NP0/C0G 500V | Bruene voltage sample |
| ADC input resistor | 1kΩ, 1% | AD8307 output to ESP32 GPIO |
| Bleeder resistor | 100kΩ, 5W, metal film | MANDATORY; across tuning capacitor |
| Power supply | 12V DC, 2A min | Motor + ESP32 |
| 5V regulator | AMS1117-5.0 or 7805 | Motor power |
| 3.3V regulator | AMS1117-3.3 | ESP32 power |
| Limit switch (min C) | SPST-NO, PCB mount | Full-mesh end stop |
| Limit switch (max C) | SPST-NO, PCB mount | Min-mesh end stop |

### 3.6 Display System (CYD)

| Component | Specification | Notes |
|-----------|--------------|-------|
| ESP32-2432S028R | "Cheap Yellow Display" module | ILI9341 TFT 320×240, XPT2046 touch, built-in BT |
| Display size | 2.8 inch, 320 × 240 px | 16-bit RGB565 color |
| Bluetooth | Classic BT Serial Profile | Connects to PNML-Tuner device name |
| Backlight pin | GPIO 21 | PWM brightness control |
| Touch CS | GPIO 33 | XPT2046 chip select |
| Touch IRQ | GPIO 36 | Touch interrupt (input-only pin) |

### 3.7 RF Connectors and Hardware

| Item | Specification | Notes |
|------|-------------|-------|
| RF input connector | BNC-F chassis mount or SO-239 | 50Ω; match transceiver |
| Feed coax | RG-58 or RG-8X | Short run from coupling loop to box |
| Corner connectors | Printed: TM-PNL-ENC-002 | 4 per loop, PETG or ASA |
| Cap housing | Printed: TM-PNL-ENC-001 | 120×95×70mm, PETG |
| Roll-up spool | Printed: TM-PNL-ENC-003 | 178mm flange dia, PETG |
| M3 × 8mm SHCS | Stainless | 8 per cap housing lid |
| M3 × 6mm SHCS | Stainless | 4 per corner connector |
| M3 heat-set inserts | M3 × 4.6mm OD × 4mm L | 2 per corner connector |
| Hook and loop strap | 25mm wide, 1m | For roll-up transport |

### 3.8 Roll-up Version — Trap Components

For multi-band roll-up antenna traps (see Chapter 6):

| Trap | Frequency | Core | Turns | C (pF) | Wire |
|------|-----------|------|-------|--------|------|
| 40m trap | 7.150 MHz | T50-2 | 12 | 41.0 | #18 AWG, 0.576m |
| 30m trap | 10.125 MHz | T50-2 | 10 | 24.8 | #18 AWG, 0.400m |
| 20m trap | 14.175 MHz | T50-2 | 8 | 15.9 | #18 AWG, 0.256m |
| 17m trap | 18.118 MHz | T50-6 | 8 | 9.6 | #18 AWG, 0.192m |
| 15m trap | 21.225 MHz | T50-6 | 7 | 7.7 | #18 AWG, 0.147m |
| 10m trap | 28.500 MHz | T50-6 | 5 | 5.6 | #18 AWG, 0.075m |

Full winding table including measured inductance, Q, and Z_trap: see `calculations/trap_winding_tables.csv`.

---

## CHAPTER 4: CONSTRUCTION — HF SMALL TRANSMITTING LOOP (160m–10m)

### 4.1 Overview

Each HF band uses a dedicated square loop antenna built on pool noodle sections joined at four corners with printed connectors (TM-PNL-ENC-002). The loop wire is wound around the pool noodle form (N turns) and resonated with a high-voltage variable capacitor mounted in the printed cap housing (TM-PNL-ENC-001). The Faraday coupling loop mounts inside the main loop, suspended at the center of the bottom rail.

**Build Sequence:**

```
  Step 1:  Print corner connectors (4 per band)
  Step 2:  Print cap housing (1 per band)
  Step 3:  Cut pool noodle sections
  Step 4:  Assemble noodle frame
  Step 5:  Wind loop wire
  Step 6:  Install tuning capacitor
  Step 7:  Install bleeder resistor
  Step 8:  Wind and install Faraday coupling loop
  Step 9:  Install limit switches
  Step 10: Install motor and gear set
  Step 11: Wire motor controller
  Step 12: Initial test — SWR check
```

### 4.2 Pool Noodle Cutting

Each square loop requires four noodle sections of equal length `s` (the loop side):

| Band | s (m) | Noodle sections (pieces) | Cut length each |
|------|-------|--------------------------|----------------|
| 160m | 2.00 | 4 × full noodle + 4 × 80cm extension | 200 cm (join needed) |
| 80m  | 1.50 | 4 × full noodle + 4 × 30cm extension | 150 cm (join needed) |
| 40m  | 1.20 | 4 | 120 cm (full noodle) |
| 30m  | 1.00 | 4 | 100 cm |
| 20m  | 0.90 | 4 | 90 cm |
| 17m  | 0.80 | 4 | 80 cm |
| 15m  | 0.70 | 4 | 70 cm |
| 12m  | 0.60 | 4 | 60 cm |
| 10m  | 0.50 | 4 | 50 cm |

For 160m and 80m, noodles must be joined end-to-end. Insert a 22mm wooden dowel 15cm into each mating end and secure with PVC pipe cement. Allow 24 hours cure.

**Cutting Procedure:**
1. Mark cut line with tape wrap (ensures perpendicular cut).
2. Use a sharp utility knife or PVC pipe cutter. Do not use a saw — the foam will tear.
3. Inspect cut face: must be flat and perpendicular. Recut if angled.
4. Label each piece with band and position (Bottom, Left, Top, Right).

### 4.3 Frame Assembly

**Corner Connector (TM-PNL-ENC-002):**

The printed corner connector has two 47mm bore sockets at 90°, a wire channel groove, and M3 heat-set insert holes for the through-wire clamp screw.

```
  Assembly:
  a. Install M3 heat-set inserts using soldering iron at 200°C.
     Push in flush with surface; do not overheat.
  b. Insert noodle section ends into connector sockets.
     Insertion depth = 40 mm minimum.
     Apply a thin bead of cyanoacrylate to sockets if fit is loose.
  c. Tighten M3 clamp screw to compress noodle lightly. Do not overtighten.
  d. Repeat for all 4 corners.
  e. Check frame squareness: measure both diagonals.
     Maximum difference: 5 mm per meter of side length.
```

**Cap Housing Mounting (TM-PNL-ENC-001):**

The capacitor housing mounts at the center of the top rail or bottom rail (builder preference; top is traditional for the variable cap at the highest potential point, but bottom is mechanically easier for field work):

```
  Mount at top rail center:
  a. Mark center of top noodle section.
  b. Secure cap housing to noodle with two hose clamps (25mm width).
  c. The housing accommodates the capacitor leads through internal channels.
  d. Motor mounts in the 28BYJ-48 recess; gear meshes through the slot.
```

### 4.4 Winding the Loop Wire

```
  SAFETY: Lay out all wire before winding. Kinks and sharp bends
          damage the wire insulation (if any) and will cause hot spots.
```

**Winding Procedure:**

1. Leave 30 cm of wire free at the start (capacitor connection lead).
2. Wind N turns tightly around the pool noodle perimeter. The wire runs around all four sides continuously.
3. Keep turns in the wire groove provided in the corner connectors.
4. Keep turns spaced evenly; do not bunch or cross turns.
5. Leave 30 cm of wire free at the end (second capacitor connection lead).
6. Secure turns every 30 cm with a small cable tie.

**Turn count per band:**

| Band | N Turns | Wire gauge |
|------|---------|-----------|
| 160m | 4 | #12 AWG |
| 80m  | 3 | #12 AWG |
| 40m  | 3 | #14 AWG |
| 30m  | 2 | #14 AWG |
| 20m  | 2 | #14 AWG |
| 17m  | 1 | #14 AWG |
| 15m  | 1 | #14 AWG |
| 12m  | 1 | #16 AWG |
| 10m  | 1 | #16 AWG |

**Multi-turn loops:** For 160m–20m (N ≥ 2), begin and end leads must both terminate at the capacitor location. For a 4-turn loop, all 4 wound conductors are connected in series: end of turn 1 connects to start of turn 2, etc. The final free ends are the capacitor terminals.

### 4.5 Capacitor Installation

```
  !! DANGER — HIGH VOLTAGE !!
  Install bleeder resistor BEFORE applying any RF power.
  See Section 4.6.
```

1. Mount capacitor inside the printed housing using the provided M3 mounting holes. Align capacitor shaft with motor coupling slot.
2. Secure capacitor mounting screws. Verify capacitor shaft turns freely through full range.
3. Connect loop wire leads to capacitor terminals. For vacuum caps, use solder lugs and 12 AWG jumpers; for air-variable, solder direct to brass mounting lugs.
4. Keep all connections as short as possible. Long leads add inductance that shifts resonance.
5. For 160m/80m vacuum capacitors: use high-voltage hookup wire (1 kV rated minimum; 20 kV rated preferred).

### 4.6 Bleeder Resistor — MANDATORY

The 100kΩ / 5W bleeder resistor discharges the capacitor after TX ends:

```
  Discharge time constant:
  τ = R × C

  Example: 40m band, C = 21 pF, R = 100 kΩ:
  τ = 100,000 × 21 × 10⁻¹²= 2.1 μs

  For 160m, C = 41.7 pF:
  τ = 4.17 μs

  Time to discharge to 37% of peak: 1τ
  Time to discharge to 5% of peak:  3τ
  Time to discharge to <1V from 2791V: 3τ × ln(2791) ≈ 21τ ≈ 88 μs
```

Despite the fast discharge, the resistor is mandatory because:
1. The capacitor stores charge between the inter-plate dielectric at DC. Even with no RF signal, the plates can accumulate static charge from atmospheric electricity.
2. A faulty transistor or motor driver can apply DC to the capacitor through the motor winding if the motor circuit is connected in a certain failure mode.
3. Regulatory compliance: any high-voltage apparatus should have a discharge path.

**Installation:**
- Solder 100kΩ resistor directly across capacitor terminals, body inside housing.
- Use 1W rating minimum; 5W preferred for robustness.
- Verify with ohmmeter across capacitor terminals before RF test: should read 100kΩ ± 10%.

### 4.7 Motor and Gear Installation

Refer to TM-PNL-ENC-001 for printed gear dimensions.

```
  Gear specifications:
    Motor pinion:  10 teeth, module 1mm, press-fit to 28BYJ-48 output shaft
    Main gear:     40 teeth, module 1mm, 6mm bore keyed to capacitor shaft
    Gear ratio:    4:1 reduction
    Tooth engagement: 2.0 mm minimum contact

  Motor mount procedure:
  a. Press 10T pinion onto 28BYJ-48 flat output shaft.
     Align flat to keyway in pinion bore.
     Seat fully (flush with shaft end).
  b. Install motor in housing recess.
     Confirm gears mesh with 0.1–0.2 mm backlash (slight freedom, not sloppy).
  c. Secure motor with M3 × 8mm screws through housing standoffs.
  d. Route motor cable through cable exit slot.
     Apply strain relief with cable tie inside housing.
```

**Motor Wiring (28BYJ-48 → ULN2003 → ESP32):**

| 28BYJ-48 Wire | Color | ULN2003 Input | ESP32 GPIO |
|---------------|-------|---------------|-----------|
| IN1 | Orange | IN1 | GPIO 25 |
| IN2 | Yellow | IN2 | GPIO 26 |
| IN3 | Green | IN3 | GPIO 27 |
| IN4 | Black/Blue | IN4 | GPIO 14 |
| +5V | Red | VCC | 5V supply |

Half-step sequence (firmware driven — see Chapter 8):

```
  Step  IN1  IN2  IN3  IN4
    0     1    0    0    0
    1     1    1    0    0
    2     0    1    0    0
    3     0    1    1    0
    4     0    0    1    0
    5     0    0    1    1
    6     0    0    0    1
    7     1    0    0    1
```

### 4.8 Limit Switch Installation

Two limit switches establish the capacitor travel endpoints:

```
  PIN_CAP_MIN_SW (GPIO 34): Full-mesh position (maximum capacitance)
    Location: Triggered when rotor plates are fully meshed.
    Used for homing: drive to this switch, set step count = 0.

  PIN_CAP_MAX_SW (GPIO 36): Minimum-mesh position (minimum capacitance)
    Location: Triggered when rotor plates are fully unmeshed.
    Used to prevent mechanical over-travel.
```

Mount switches on the housing interior such that the capacitor drive gear actuates the switch lever at each end of travel. Adjust switch position until it triggers within 2° of mechanical end-stop; do not allow the capacitor to reach its own mechanical stop (risk of gear damage).

**Wiring:**
- Connect each switch between the GPIO pin and GND.
- Add 10kΩ pull-up resistor to 3.3V on each switch line (internal pull-up in firmware is also enabled as backup).
- Active-LOW: switch closed (triggered) = LOW signal.

### 4.9 Faraday Coupling Loop Construction

See TM-PNL-SCH-003 for full theory. Dimensions per band:

| Band | Main Loop Side | Coupling Loop Side | Coupling Wire Length |
|------|---------------|-------------------|---------------------|
| 160m | 2.00 m | 0.40 m | 1.60 m |
| 80m  | 1.50 m | 0.30 m | 1.20 m |
| 40m  | 1.20 m | 0.24 m | 0.96 m |
| 30m  | 1.00 m | 0.20 m | 0.80 m |
| 20m  | 0.90 m | 0.18 m | 0.72 m |
| 17m  | 0.80 m | 0.16 m | 0.64 m |
| 15m  | 0.70 m | 0.14 m | 0.56 m |
| 12m  | 0.60 m | 0.12 m | 0.48 m |
| 10m  | 0.50 m | 0.10 m | 0.40 m |

**Construction:**
1. Form a square from coupling loop wire (#18 AWG bare copper). The square closes on itself; the two wire ends are the coax feed points.
2. Leave a 5 cm tail on each end.
3. Connect coax: center conductor to one tail, outer braid to other tail. This is a balanced connection — do NOT short both tails to the outer braid.
4. Install ferrite bead (Fair-Rite 2643002402) on the coax directly behind the coupling loop to suppress common-mode current.
5. Mount the coupling loop inside the main loop, centered on the bottom rail, coplanar with the main loop. The coupling loop plane is parallel to the main loop plane (both face the same direction).
6. Initially mount the coupling loop at 50% depth into the main loop (loop center). Adjust during alignment (Chapter 10).

**Mounting:**
- A 3D-printed sliding bracket (file: `enclosures/loop_connector.scad` provides wire channel) can support the coupling loop.
- Alternatively, tape the coupling loop to the pool noodle bottom rail with self-fusing silicone tape. This allows easy repositioning during alignment.

---

## CHAPTER 5: CONSTRUCTION — VHF/UHF FULL-WAVE QUAD (6m–20cm)

### 5.1 Overview

For 6 meters through 20 centimeters, the pool noodle antenna system uses full-wave resonant quad loops. No tuning capacitor is required; the loop operates at a fixed resonant frequency near band center.

The VHF/UHF quad is simpler to construct than the HF STL: no high-voltage capacitor, no motor, no multi-turn winding. The main construction challenge is maintaining element straightness and achieving the correct side length.

### 5.2 Quad Dimensions

Full-wave loop: circumference = λ × VF, where VF = 0.95 (velocity factor for pool noodle wire).

| Band | Freq (MHz) | λ (m) | Side (m) | Wire perimeter (m) | Pool noodle sections |
|------|-----------|-------|----------|-------------------|---------------------|
| 6m | 51.000 | 5.878 | 1.397 | 5.588 | 4 × 139.7 cm |
| 2m | 146.000 | 2.053 | 0.488 | 1.951 | 4 × 48.8 cm |
| 1.25m | 222.000 | 1.351 | 0.321 | 1.284 | 4 × 32.1 cm |
| 70cm | 446.000 | 0.672 | 0.160 | 0.638 | 4 × 16.0 cm |
| 33cm | 906.000 | 0.331 | 0.079 | 0.315 | Frame only; wire loop exposed |
| 20cm | 1296.000 | 0.231 | 0.055 | 0.220 | PCB/rigid substrate |

For 33cm and 20cm bands, the pool noodle sections are too short to provide useful structural support. For 33cm, use the corner connectors only with short noodle stubs to maintain corner positions. For 20cm, use a PCB or sheet aluminum frame; NEC model is provided for reference but construction is outside pool noodle system scope.

### 5.3 Wire Selection for VHF/UHF

| Band | Wire Type | Gauge | Notes |
|------|-----------|-------|-------|
| 6m | Bare copper | #12 or #14 AWG | Skin depth 9.2 μm; heavier wire acceptable |
| 2m | Bare copper | #14 or #16 AWG | Minimize solder joints |
| 1.25m | Bare copper | #16 or #18 AWG | Precise length critical |
| 70cm | Solid copper | #18 AWG | Cut to exact length; no extra wire |
| 33cm | Rigid copper tubing | 3mm OD | Wire too flexible at this frequency |

### 5.4 6-Meter Quad Construction

The 6m band is most similar to HF in construction:

1. Cut four pool noodle sections: 139.7 cm each (±2 mm tolerance).
2. Assemble frame using four corner connectors (TM-PNL-ENC-002).
3. Cut wire to 5.588 m (measure precisely; use metric tape rule).
4. Run wire around perimeter in groove of corner connectors. Leave gap at bottom center for feed point.
5. Feed point: two wire tails, each 10 cm long, terminating at bottom center.
6. Solder coax to feed: center conductor to one tail, outer braid to other.
7. Install 2.5:1 UNUN (FT-50-61 toroid, 7 primary turns / 11 secondary turns) at feed point.
8. Verify SWR with antenna analyzer. Target: SWR ≤ 1.5:1 at 51.0 MHz.

**UNUN winding (FT-50-61 toroid):**
```
  Core:     FT-50-61 (material 61, VHF-optimized)
  Primary:  7 turns #22 AWG, wind one direction
  Secondary: 11 turns #22 AWG, wind same direction, same start point (trifilar or bifilar)
  Impedance ratio: (11/7)² = 2.47 ≈ 2.5:1
  50Ω side → 125Ω side
```

### 5.5 2-Meter Quad Construction

1. Cut four noodle sections: 48.8 cm each (±1 mm tolerance).
2. Assemble with corner connectors.
3. Cut #14 AWG wire to 195.1 cm.
4. Run wire around perimeter. Feed at bottom center.
5. UNUN same as 6m version.

**Orientation for vertical polarization (recommended for portable operation):**
- Lay quad in horizontal plane (flat, parallel to ground).
- Feed at one corner (rotated 45° from vertical/horizontal).
- Results in approximately circular polarization — useful for satellite contacts and weather balloon tracking.

**Orientation for horizontal polarization:**
- Loop in vertical plane, feed at bottom center.
- Horizontal polarization matches most terrestrial 2m SSB operation.

### 5.6 UHF Bands (1.25m, 70cm, 33cm)

For 222 MHz through 906 MHz, construction tolerances tighten significantly:

```
  Resonant frequency shift with length error:
  Δf / f ≈ −ΔL / L

  Example: 70cm band (446 MHz), side = 16.0 cm:
  1mm length error per side → 4mm total perimeter error
  Δf = (4/638) × 446 = 2.8 MHz shift

  This places resonance outside the 70cm band (420–450 MHz is acceptable).
  Tolerance: ±1 mm per side recommended.
```

For UHF, use a gamma match (Section 5.7) rather than UNUN. The UNUN's toroid losses increase above 200 MHz.

### 5.7 Gamma Match for VHF/UHF

The gamma match provides adjustable matching without a wound transformer:

```
  Gamma rod: runs parallel to loop wire at feed point
  Length: 4–5% of loop circumference
  Series capacitor: 5–30 pF air trimmer

  Example: 70cm band, circumference = 63.8 cm
  Gamma rod length: 0.05 × 63.8 = 3.2 cm

  Adjustment procedure:
  a. Connect antenna analyzer to gamma match tap.
  b. Adjust series capacitor for minimum reactance (Im{Z} → 0).
  c. Adjust gamma rod length for minimum SWR (Re{Z} → 50Ω).
  d. Iterate steps b–c until SWR < 1.5:1.
```

---

## CHAPTER 6: CONSTRUCTION — ROLL-UP MULTI-BAND VERSION

### 6.1 Concept

The roll-up version combines multiple band traps on a single loop wire, allowing one antenna to operate on several bands without swapping antennas. The traps provide high impedance at their resonant frequencies, electrically shortening the active loop section.

**Trap isolation principle:**
- The outermost (lowest-frequency) trap passes current at all higher frequencies but blocks it at its own resonant frequency.
- For 40m operation: all traps are transparent (frequencies above each trap pass through), loop uses full wire length.
- For 20m operation: the 20m trap blocks current beyond its location, effectively shortening the loop to 20m resonant length.

### 6.2 Trap Placement

Traps are placed on the loop wire at positions corresponding to the resonant loop perimeters:

```
  40m trap:  at perimeter = 4.80 m from feed (end of 40m loop section)
  30m trap:  at perimeter = 4.00 m from feed
  20m trap:  at perimeter = 3.60 m from feed
  17m trap:  at perimeter = 3.20 m from feed
  15m trap:  at perimeter = 2.80 m from feed
  10m trap:  at perimeter = 2.00 m from feed
```

The 10m trap is innermost (nearest feed); 40m trap is outermost (farthest from feed). The innermost active loop section (10m size) is always present.

### 6.3 Winding LC Traps

See `calculations/trap_winding_tables.csv` for complete data. Each trap consists of:
- A T50 toroid wound with #18 AWG wire
- A silver-mica or NP0 ceramic capacitor in parallel with the coil

**Winding procedure:**

1. Count and mark the thread size on the toroid with a permanent marker.
2. Cut wire to length specified in winding table plus 10 cm extra (for leads).
3. Wind turns through toroid hole, keeping turns evenly spaced, not overlapping.
4. Wind clockwise from center looking at toroid face (consistent direction).
5. Measure inductance with LC meter. Compare to target value. Unwind or add turns as needed (target ±2%).
6. Parallel capacitor: solder across coil leads at toroid body. Keep leads short (<5mm).
7. Verify resonant frequency with dip meter or antenna analyzer: dip in coupling should appear at target frequency ±50 kHz.
8. Waterproof with two coats of polyurethane or Q-dope.

**Trap quality test:**

```
  Measure trap impedance at resonance using antenna analyzer:
  Set analyzer to series mode (Z measurement).
  Z should be high (>2000Ω) at resonant frequency.
  Z should be low (<100Ω) ±10% above and below resonance.

  Q estimation:
  Measure f_low and f_high at Z = Z_peak / √2
  Q = f_resonant / (f_high − f_low)
  Target: Q > 150
```

### 6.4 Loop Assembly with Traps

1. Begin with the complete wire run for the outermost band (40m: 4.80 m + 20% extra = 5.76 m).
2. At each trap location, cut the wire and solder trap in series. Use short pigtails (2 cm each side) to connect trap terminals to the wire run.
3. Wind the loop wire around the pool noodle frame as in Chapter 4, but note that trap assemblies must sit at specific positions.
4. Traps must be held clear of the pool noodle body — mount in clip cradles on the rollup_spool.scad flanges (see TM-PNL-ENC-003), or tape to the frame with self-fusing silicone tape.
5. Coupling loop: sized for the innermost (highest-frequency) active band you intend to use.

### 6.5 Roll-up Spool (TM-PNL-ENC-003)

The spool stores and transports the roll-up antenna:

```
  Hub:      D = 52mm, L = 80mm, 4 anti-twist fins
  Flanges:  D = 178mm; bottom plain, top with 6 trap clip cradles
  Trap cradles: at R = 68mm from hub center on top flange face
  Center bore: 8mm (for carrying strap)
  Strap slot:  24 × 12mm in hub

  Deployment:
  a. Attach spool strap to pack or mast.
  b. Unwind loop wire from hub (inner to outer).
  c. Expand pool noodle frame sections; insert into corner connectors.
  d. Clip trap assemblies to frame or noodle surface.
  e. Thread coupling loop coax through hub bore.
  f. Connect coax to coupling loop.
  g. Connect to tuner.
```

### 6.6 Multi-band Performance Expectations

The roll-up multi-band version has lower performance than a dedicated single-band loop because:

1. **Trap losses:** Each trap adds series loss resistance in its pass band. Typical degradation: 0.5–1.0 dB per band step.
2. **Loop geometry compromise:** A fixed frame cannot be optimally sized for each band simultaneously.
3. **Resonance interaction:** Traps are not ideal; adjacent bands interact slightly, shifting resonance from calculated positions.

Expected efficiency penalty vs. single-band loop: 2–4 dB (one-half to one S-unit). Acceptable for portable use where convenience outweighs maximum efficiency.

---

## CHAPTER 7: SWR MEASUREMENT SYSTEM

### 7.1 System Architecture

```
  Transmitter → coax → Bruene Directional Coupler → Tuning Cap + Loop

                              ↓               ↓
                         Forward port    Reflected port
                              ↓               ↓
                          AD8307 #1       AD8307 #2
                         (25 mV/dB)      (25 mV/dB)
                              ↓               ↓
                         GPIO 35           GPIO 39
                              ↓               ↓
                    ESP32 12-bit ADC  (64× oversampling, 8× average)
                              ↓               ↓
                    P_fwd (dBm)      P_refl (dBm)
                              ↓
                    SWR, efficiency, corrected watts
                              ↓
                    BT serial packet → CYD display
```

### 7.2 Bruene Coupler Construction

The directional coupler uses a binocular ferrite core for current sampling and a small capacitor for voltage sampling:

**Current transformer:**
```
  Core: BN-43-2402 binocular (Fair-Rite part 2843002402)
  Primary: 1 turn (the main coax center conductor passes through both holes)
  Secondary: 3 turns #22 AWG through both holes
  Coupling: approximately 20 dB at HF
```

**Voltage sample:**
```
  A 33pF NP0 capacitor connects from the main line to the coupler summing node.
  Value is chosen so that at 50Ω, the voltage sample current equals the
  current transformer sample magnitude.
```

**Detector diodes:**
```
  Use BAT54 Schottky diodes (low forward voltage for small-signal detection).
  Two diodes: forward port detector and reflected port detector.
  Each diode drives a 10μF capacitor and 10kΩ load resistor (RC filter).
  RC time constant: 100 ms — fast enough to follow keyed CW, slow enough to
  average over RF cycles.
```

### 7.3 ADC Reading and Calibration

The ESP32 ADC is accurate but nonlinear. The firmware applies linearization:

```cpp
  // Oversampled ADC reading
  uint32_t adc_sum = 0;
  for (int i = 0; i < ADC_OVERSAMPLE; i++) {
      adc_sum += analogRead(pin);
  }
  float adc_raw = (float)adc_sum / ADC_OVERSAMPLE;

  // Convert to millivolts (approximate; ESP32 ADC non-linearity ±5%)
  float v_mV = (adc_raw / 4095.0f) × 3300.0f;

  // AD8307: V_out = 25 mV/dB × (P_dBm + 84)
  float p_dBm = (v_mV / 25.0f) − 84.0f;
```

**Calibration constants** `CAL_OFFSET_FWD` and `CAL_OFFSET_REFL` (config.h) are trim adjustments applied after coupling factor correction. Initial values = 0.0 dB. After bench calibration against a known power reference, adjust these to bring measured power within ±0.5 dB of true value.

**Calibration procedure:**
1. Connect calibrated RF signal source (or QRP rig with power meter in-line) to antenna port.
2. Terminate load port with 50Ω dummy load (SWR = 1.0:1 expected).
3. Apply 1W at 14.175 MHz. ESP32 web interface `/api/status` shows measured power.
4. Adjust `CAL_OFFSET_FWD` (NVS key `cal_fwd`) via `/api/cal?fwd=X` until reading = 1.00W.
5. Apply mismatched load (2:1 SWR reference). Verify SWR reading is 2.0 ±0.1.

### 7.4 SWR Computation

From the firmware `measureSWR()` function:

```cpp
struct Measurement {
    float fwd_watts;
    float refl_watts;
    float swr;
    bool valid;
};

Measurement measureSWR() {
    // Average N readings
    float fwd_acc = 0, refl_acc = 0;
    for (int i = 0; i < SWR_AVERAGE_N; i++) {
        fwd_acc  += readPower(PIN_FWD_ADC,  CAL_OFFSET_FWD);
        refl_acc += readPower(PIN_REFL_ADC, CAL_OFFSET_REFL);
        delay(1);
    }
    float fwd_W  = fwd_acc  / SWR_AVERAGE_N;
    float refl_W = refl_acc / SWR_AVERAGE_N;

    bool valid = (fwd_W > pow(10.0f, (MIN_FWD_DBM + 20.0f) / 10.0f) / 1000.0f);

    float swr = 999.9f;
    if (valid && fwd_W > refl_W) {
        float gamma = sqrtf(refl_W / fwd_W);
        swr = (1.0f + gamma) / (1.0f - gamma);
        swr = min(swr, SWR_MAX_DISPLAY);
    }

    return {fwd_W, refl_W, swr, valid};
}
```

---

## CHAPTER 8: ESP32 AUTO-TUNER FIRMWARE

### 8.1 Firmware Overview

Document: TM-PNL-FW-001  
File: `firmware/esp32_loop_tuner/esp32_loop_tuner.ino`

The ESP32 firmware provides:
- **Motor control:** Half-step drive for 28BYJ-48 via ULN2003
- **Limit switch monitoring:** Prevents over-travel
- **SWR measurement:** Bruene coupler + dual AD8307
- **Auto-tune algorithm:** Gradient descent with bisecting step size
- **Band management:** 15-band table with NVS persistence
- **Bluetooth serial:** 200ms telemetry to CYD display
- **Wi-Fi access point:** HTTP control interface
- **High-voltage warning:** Per-band safety alerts in web UI

### 8.2 State Machine

```
  States:
  ST_IDLE      (0): Motor stopped, awaiting command
  ST_HOMING    (1): Seeking full-mesh limit switch
  ST_TUNING    (2): Auto-tune running
  ST_LOCKED    (3): SWR < SWR_GOOD, motor stopped
  ST_QUAD_READY(4): VHF/UHF quad; no capacitor; band is ready

  Transitions:
  IDLE     → HOMING:     /home command or power-on without saved position
  HOMING   → IDLE:       Homing complete, moved to band default position
  IDLE     → TUNING:     /tune command or TUNE button
  TUNING   → LOCKED:     SWR < SWR_GOOD (1.5:1)
  TUNING   → IDLE:       Tune failed (steps exhausted, no improvement)
  LOCKED   → IDLE:       Band changed
  *        → QUAD_READY: Band selection is LOOP_QUAD type
```

### 8.3 Motor Control

**Half-step sequence** provides smoother motion and doubles resolution vs. full-step:

```cpp
const uint8_t HALF_STEPS[8][4] = {
    {1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,1,1,0},
    {0,0,1,0}, {0,0,1,1}, {0,0,0,1}, {1,0,0,1}
};

void setMotorPhase(int phase) {
    digitalWrite(PIN_MOTOR_IN1, HALF_STEPS[phase][0]);
    digitalWrite(PIN_MOTOR_IN2, HALF_STEPS[phase][1]);
    digitalWrite(PIN_MOTOR_IN3, HALF_STEPS[phase][2]);
    digitalWrite(PIN_MOTOR_IN4, HALF_STEPS[phase][3]);
}

void deenergizeMotor() {
    // All pins LOW after motion to reduce heat
    digitalWrite(PIN_MOTOR_IN1, 0);
    digitalWrite(PIN_MOTOR_IN2, 0);
    digitalWrite(PIN_MOTOR_IN3, 0);
    digitalWrite(PIN_MOTOR_IN4, 0);
}
```

**Motor direction convention:**
- `dir = +1`: capacitor toward minimum C (unmesh plates, higher step count)
- `dir = -1`: capacitor toward maximum C (mesh plates, lower step count, toward home)

### 8.4 Homing Procedure

```
  homeCapacitor():
  a. Drive motor in −1 direction (toward full-mesh) at HOME_SPEED_DELAY (3 ms/step).
  b. Monitor PIN_CAP_MIN_SW.
  c. When MIN_SW goes LOW (triggered), stop motor.
  d. Set g_cap_pos = 0 (home position = full mesh = maximum C).
  e. Drive motor in +1 direction to band's default_cap_steps position.
  f. Set state = ST_IDLE.
  g. Save cap_pos and homed=true to NVS.
```

**First power-on behavior:** If NVS has no saved position (or homed=false), homing is triggered automatically before any operation.

**Saved position restore:** If NVS has a valid saved cap_pos from a previous session, the firmware restores the position value without physical motion. This is valid if the motor was de-energized cleanly and not disturbed. A manual home command should be run if the antenna was moved, capacitor adjusted manually, or power was lost during motion.

### 8.5 Auto-Tune Algorithm

The auto-tune algorithm uses gradient descent with adaptive (bisecting) step size. This is the same approach as the screwdriver antenna tuner (SC series) because the underlying physics is identical: minimize SWR over a 1-D parameter space (capacitor position).

```
  autoTune():
  1. Take initial SWR measurement at current position.
  2. Take one step in +direction.
  3. Measure SWR.
  4. If SWR improved: continue in same direction.
     If SWR worsened: reverse direction, halve step size.
  5. Repeat until:
     a. SWR < SWR_GOOD (1.5:1) → success, state = LOCKED
     b. Step size < TUNE_STEP_MIN (125) → stop, state = IDLE (tune failed or at min BW)
     c. Hit limit switch → stop
  6. De-energize motor.
  7. Save cap_pos to NVS.
```

**Why bisecting step size works for STL tuning:**

The SWR vs. capacitor position curve for an STL is a smooth, single-minimum function (within the loop's operating range). The SWR minimum corresponds to the resonant capacitance. The curve is approximately parabolic near the minimum, making bisection highly efficient — it converges to the minimum in O(log₂(range/tolerance)) steps.

With TUNE_STEP_SIZE = 500 and TUNE_STEP_MIN = 125, maximum bisection depth = log₂(500/125) = 2 halvings. Typical tune cycle: 20–80 motor steps, completed in under 1 second at TUNE_SPEED_DELAY = 5 ms/step.

### 8.6 Bluetooth Telemetry Packet

The BT packet is transmitted every 200 ms (BT_INTERVAL_MS):

```
Format:
  "BAND:%s,POS:%ld,C:%.0f,SWR:%.2f,FWD:%.3fW,EFF:%.1f,STATE:%d,HOMED:%d\n"

Fields:
  BAND:   Band name string (e.g., "40m", "6m")
  POS:    Motor step count from home (long integer)
  C:      Estimated capacitance in pF (float, 0 decimal places)
  SWR:    Measured SWR (float, 2 decimal places)
  FWD:    Forward power in watts (float, 3 decimal places)
  EFF:    Antenna efficiency in percent (float, 1 decimal place)
  STATE:  TunerState enum value (integer 0–4)
  HOMED:  1 if homed, 0 if not homed (integer)

Example:
  "BAND:40m,POS:131072,C:21,SWR:1.23,FWD:4.987W,EFF:29.2,STATE:3,HOMED:1\n"
```

### 8.7 Web Interface

The firmware hosts a web server on port 80 (default Wi-Fi AP: SSID "PNML-Tuner", password "antenna73"):

| URL | Method | Function |
|-----|--------|---------|
| `/` | GET | Main dashboard page with live SWR, band, motor status |
| `/band/up` | GET | Select next higher band |
| `/band/dn` | GET | Select next lower band |
| `/home` | GET | Trigger homing sequence |
| `/tune` | GET | Trigger auto-tune |
| `/api/status` | GET | JSON status dump |
| `/api/motor` | GET | Manual motor step: `?dir=1&steps=100` |
| `/api/cal` | GET | Set calibration offset: `?fwd=1.5&refl=0.0` |

The web dashboard uses a red high-voltage warning banner for any band where `vcap_5w_peak > 1000V`.

---

## CHAPTER 9: CYD DISPLAY FIRMWARE

### 9.1 Firmware Overview

Document: TM-PNL-FW-002  
File: `firmware/cyd_loop_display/cyd_loop_display.ino`

The CYD (ESP32-2432S028R) operates as a wireless display terminal, receiving Bluetooth serial telemetry from the ESP32 tuner and presenting it on the 320×240 ILI9341 TFT.

**Display Pages:**

| Page | Number | Content |
|------|--------|---------|
| SWR Meter | 0 | Large SWR bar graph + numeric, band name, state, power |
| Cap Gauge | 1 | Motor position bar, estimated C_pF, HV warning for STL; "RESONANT QUAD" message for LOOP_QUAD |
| Power | 2 | Forward watts, efficiency, SWR numeric |
| Guide | 3 | Operating procedure text (band-specific) |

**Page Navigation:** Touch left 80px zone → previous page. Touch right 80px zone → next page. Swipe response: 200ms debounce.

### 9.2 SWR Page Layout

```
  ┌──────────────────────────────────────────┐
  │  40m                      ST: LOCKED    │   ← band + state (top bar)
  ├──────────────────────────────────────────┤
  │                                          │
  │         SWR  1.23                       │   ← large numeric
  │                                          │
  │  1.0  ████████████░░░░░░░░  10.0        │   ← bar graph (green/yellow/red)
  │                                          │
  │  FWD: 4.987 W                           │
  ├──────────────────────────────────────────┤
  │  [◄]                               [►]  │   ← touch zones
  └──────────────────────────────────────────┘

  SWR bar color:
    < SWR_GOOD (1.5):  C_DARK_GREEN fill
    ≥ SWR_GOOD, < SWR_WARN (2.5):  C_WARN fill
    ≥ SWR_WARN:  C_ALARM fill
```

### 9.3 Capacitor Page Layout — STL Mode

```
  ┌──────────────────────────────────────────┐
  │  40m                      POS: 131072   │
  ├──────────────────────────────────────────┤
  │                                          │
  │  CAP:  21 pF                            │   ← estimated capacitance
  │                                          │
  │  MIN [░░░░░████████████░░░░░░░░] MAX    │   ← position arc gauge
  │                                          │
  │  ╔══════════════════════════════════╗   │
  │  ║  !! HIGH VOLTAGE !!  1738 V pk  ║   │   ← red warning box
  │  ╚══════════════════════════════════╝   │
  ├──────────────────────────────────────────┤
  │  [◄]                               [►]  │
  └──────────────────────────────────────────┘
```

For bands where `vcap_5w_peak > 1000V`, the HV warning box is drawn with red background and white text, blinking at 2 Hz.

### 9.4 Capacitor Page Layout — QUAD Mode

```
  ┌──────────────────────────────────────────┐
  │  6m                       ST: QUAD_RDY  │
  ├──────────────────────────────────────────┤
  │                                          │
  │         RESONANT QUAD                   │
  │                                          │
  │    No capacitor required.               │
  │    Connect coax and operate.            │
  │                                          │
  │    Eff: 98.0%    BW: 85 kHz            │
  │                                          │
  ├──────────────────────────────────────────┤
  │  [◄]                               [►]  │
  └──────────────────────────────────────────┘
```

### 9.5 Bluetooth Connection Management

```
  BT_TIMEOUT_MS = 3000 ms

  If time since last packet > BT_TIMEOUT_MS:
    Display "CONNECTING..." screen with blinking dots
    Attempt BT reconnect to "PNML-Tuner"
  
  On successful connection:
    Immediately switch to last displayed page
    Resume normal display
```

The CYD periodically scans for the "PNML-Tuner" device name. Once paired, the connection is maintained until out of range. Re-pairing is automatic.

### 9.6 Color Palette

All colors are defined in `config.h` as RGB565 values for the ILI9341:

```
  C_BG         0x0000   Black background
  C_TEXT        0x07E0   Green text (primary)
  C_WARN        0xFFE0   Yellow (caution)
  C_ALARM       0xF800   Red (high SWR, HV)
  C_TITLE       0xFFE0   Yellow titles
  C_BORDER      0x2104   Dark grey border
  C_CYAN        0x07FF   Cyan accents
  C_WHITE       0xFFFF   White (HV box text)
  C_ORANGE      0xFD20   Orange (intermediate states)
  C_DARK_RED    0x8000   HV warning background
  C_DARK_GREEN  0x03E0   Good SWR fill
  C_DARK_BLUE   0x000F   Guide page background tint
```

---

## CHAPTER 10: TUNING AND ALIGNMENT

### 10.1 Initial Power-On Sequence

```
  1. Verify bleeder resistor installed (measure 100kΩ across cap terminals).
  2. Connect 50Ω dummy load or SWR meter to loop feed coax.
  3. Power on ESP32 tuner. Confirm blue LED blinks (firmware running).
  4. Connect CYD display: power on CYD; wait for BT connection (≤30s).
  5. CYD shows "CONNECTING..." while BT pairs. After pair, shows current state.
  6. If not homed: press TUNE_BTN or navigate to web UI and click HOME.
     Observe motor rotating; CYD shows ST: HOMING.
     Homing completes in 10–30 seconds depending on starting position.
  7. After homing, motor drives to band default position automatically.
  8. CYD shows ST: IDLE.
```

### 10.2 SWR Dip Verification

Before auto-tune, verify that a manual SWR dip exists near the band center:

```
  Tools needed: Antenna analyzer (NanoVNA or similar), or QRP rig + SWR meter

  Procedure:
  1. Connect antenna analyzer to coupling loop coax.
  2. Sweep frequency through ±100 kHz around band center.
  3. Observe SWR vs. frequency plot.
  4. A sharp SWR dip (width = 2–15 kHz) should appear near f₀.
     If no dip visible: coupling loop position or capacitor range issue.
     If dip very shallow (SWR > 3:1 at minimum): coupling too loose.
     If dip present but SWR minimum > 1.5:1: adjust coupling loop position.
```

### 10.3 Coupling Loop Position Adjustment

This is the critical alignment step for HF STL antennas. The SWR minimum depth is controlled by coupling loop depth into the main loop:

```
  Coupling adjustment (do this before auto-tune):

  a. Observe SWR dip minimum with antenna analyzer.
  b. If SWR at dip > 1.5:1 (under-coupled):
     Slide coupling loop further into main loop (closer to main loop center axis).
     Repeat capacitor sweep to find new dip position.
  c. If SWR at dip < 1.1:1 but dip is very narrow (over-coupled is unlikely for small loops;
     more likely means a resonance artifact):
     Verify coupling loop is not shorting any main loop turn.
  d. Iterate until SWR dip is ≤ 1.5:1.
     Record coupling loop position for field notes.

  IMPORTANT: Coupling adjustment and capacitor resonance adjustment interact.
  After any coupling adjustment, re-verify that the resonant frequency
  (position of SWR dip) is still at the desired band center.
  Plan for 2–3 iterations of both adjustments.
```

### 10.4 Auto-Tune Operation

```
  With coupling loop adjusted and SWR dip confirmed:

  1. Apply low power (0.5–1W) from transmitter at band center frequency.
  2. Press TUNE button on ESP32 tuner, or navigate to web UI → Tune.
  3. CYD shows ST: TUNING. Motor rotates; SWR readout changes.
  4. Observe CYD SWR page. SWR should decrease toward minimum.
  5. When SWR < SWR_GOOD (1.5:1), motor stops, state = LOCKED.
  6. CYD shows green SWR bar.

  If tune fails (state returns to IDLE with SWR > 1.5:1):
  a. Verify RF power is applied during tune. Without RF, no SWR measurement possible.
  b. Verify antenna analyzer showed a dip during manual sweep (Section 10.2).
  c. Manually move capacitor to near the dip position using web UI /api/motor.
  d. Retry auto-tune from position closer to resonance.
```

### 10.5 Band Change Procedure

```
  1. Stop transmitting.
  2. On ESP32 web UI: click Band Up or Band Down.
     (Or press PIN_BAND_UP / PIN_BAND_DN buttons.)
  3. Firmware updates band index, loads new default_cap_steps, moves motor.
     This is a fast slew (FAST_SPEED_DELAY = 2 ms/step).
  4. State returns to IDLE.
  5. Select new operating frequency on transceiver.
  6. Apply low power, press TUNE.
  7. Auto-tune completes.

  For VHF/UHF quad bands:
  Steps 3–7 are replaced by: Motor does not move (no cap). State goes to ST_QUAD_READY.
  CYD displays QUAD mode page.
```

### 10.6 Operating Notes by Band

**160m / 80m (DANGER bands):**
- Vacuum capacitor mandatory. Air-variable will arc and fail.
- After any transmit, wait 10 seconds before touching capacitor housing.
- The capacitor housing printed enclosure is not a safety barrier — it reduces accidental contact probability but provides no significant electrical isolation.
- Strongly consider operating these bands remote (web interface) with the antenna and tuner at the mast, operator at a safe distance.
- Operating note: "DANGER: >2.8kV@5W. Bleeder 100kΩ mandatory. Vacuum cap required."

**40m / 30m (Caution bands):**
- Air-variable capacitor with wide plate spacing (5mm minimum).
- Do not touch capacitor housing during TX.
- Operating note: "CAUTION: >1.7kV@5W. Do not touch cap during TX."

**20m through 10m:**
- Air-variable or high-quality NP0/silver-mica acceptable.
- Still high voltage; standard HV precautions apply.
- Operating note varies by band; see BANDS[] table in config.h.

**6m through 20cm (Quad bands):**
- No high-voltage hazard. Normal RF safety applies (RF burns at close range).
- Gamma match adjustment: use plastic screwdriver for Cγ trimmer.
- For satellite (2m/70cm): orient loop for circular polarization if possible.

---

## CHAPTER 11: PERFORMANCE DATA

### 11.1 Computed Efficiency and Bandwidth — All Bands

The following values are from `calculations/loop_dimensions.csv`. All values computed using the physics equations in Chapter 2, with #14 AWG copper wire, skin depth at each frequency.

| Band | Freq (MHz) | N Turns | L (μH) | C (pF) | Q | Eff (%) | BW (kHz) | V_cap@5W (V) |
|------|-----------|---------|--------|--------|---|---------|---------|------------|
| 160m | 1.900 | 4 | 168.11 | 41.7 | 891 | 12.89 | 2.13 | 2,791 |
| 80m | 3.750 | 3 | 68.28 | 26.4 | 904 | 22.14 | 4.15 | 2,379 |
| 40m | 7.150 | 3 | 23.55 | 21.0 | 807 | 29.23 | 8.86 | 1,738 |
| 30m | 10.125 | 2 | 19.13 | 12.9 | 936 | 44.68 | 10.82 | 1,775 |
| 20m | 14.175 | 2 | 16.96 | 7.4 | 1091 | 65.65 | 12.99 | 1,682 |
| 17m | 18.118 | 1 | 3.70 | 20.8 | 606 | 28.37 | 29.89 | 957 |
| 15m | 21.225 | 1 | 3.18 | 17.7 | 643 | 31.59 | 33.00 | 966 |
| 12m | 24.940 | 1 | 2.66 | 15.3 | 681 | 33.84 | 36.61 | 969 |
| 10m | 28.500 | 1 | 2.16 | 14.5 | 708 | 32.07 | 40.26 | 963 |

**Notes:**
- 17m–10m single-turn loops have lower efficiency than 20m–40m multi-turn loops because the N⁴ improvement in radiation resistance is lost.
- 20m achieves the best balance: 2-turn loop, high Q, 65% efficiency.
- 10m efficiency dips slightly vs. 12m due to loop size becoming relatively large (10m side = λ/5 at 28 MHz), causing current distribution non-uniformity not captured in the lumped model.

### 11.2 VHF/UHF Quad Performance

| Band | Freq (MHz) | Side (m) | Q_est | Eff (%) | BW (kHz) |
|------|-----------|---------|-------|---------|---------|
| 6m | 51.000 | 1.397 | 600 | 98.0 | 85 |
| 2m | 146.000 | 0.488 | 400 | 97.0 | 365 |
| 1.25m | 222.000 | 0.321 | 300 | 96.0 | 740 |
| 70cm | 446.000 | 0.160 | 200 | 95.0 | 2,230 |
| 33cm | 906.000 | 0.079 | 150 | 92.0 | 6,040 |
| 20cm | 1296.000 | 0.055 | 100 | 88.0 | 12,960 |

VHF/UHF efficiency approaches 100% because skin depth losses are small relative to radiation resistance for full-wavelength elements.

### 11.3 Comparison with Full-Size Antennas

The efficiency values above translate to effective power reductions compared to a lossless reference antenna:

| Band | STL Efficiency | Power Penalty vs. Dipole | Effective power at 5W input |
|------|--------------|-------------------------|---------------------------|
| 160m | 12.9% | −8.9 dB | 0.64 W |
| 80m | 22.1% | −6.5 dB | 1.11 W |
| 40m | 29.2% | −5.4 dB | 1.46 W |
| 30m | 44.7% | −3.5 dB | 2.24 W |
| 20m | 65.7% | −1.8 dB | 3.29 W |
| 17m | 28.4% | −5.5 dB | 1.42 W |
| 15m | 31.6% | −5.0 dB | 1.58 W |
| 12m | 33.8% | −4.7 dB | 1.69 W |
| 10m | 32.1% | −4.9 dB | 1.61 W |

Despite the efficiency penalty, the magnetic loop's advantages (small size, low noise reception, sharp directional null for RFI rejection) make it a competitive antenna for constrained environments, particularly when operating QRP on 30m–20m.

### 11.4 NEC-2 Model Results Summary

Complete NEC-2 model files: TM-PNL-NEC-001 through -015. Summary of key outputs:

| Band | Model File | Max Gain (dBi) | Elevation of Max | Take-off Angle |
|------|-----------|---------------|-----------------|---------------|
| 40m | TM-PNL-NEC-003 | 1.2 | Broadside | 45° |
| 20m | TM-PNL-NEC-005 | 1.8 | Broadside | 32° |
| 10m | TM-PNL-NEC-009 | 2.1 | Broadside | 22° |
| 6m | TM-PNL-NEC-010 | 3.8 | Broadside | 18° |
| 2m | TM-PNL-NEC-011 | 3.5 | Broadside | 12° |

**NEC-2 Limitation Note:** Multi-turn coils cannot be accurately modeled in NEC-2. All HF STL models use a single-turn wire with the equivalent resistance (Rrad + Rloss for the N-turn equivalent). Gain and pattern are accurate; absolute impedance values are model estimates. See CM comment block in each NEC file for scaling factors.

---

## CHAPTER 12: SAFETY — HIGH VOLTAGE CRITICAL SECTION

### 12.1 Scope and Authority

```
╔══════════════════════════════════════════════════════════════════╗
║  THIS CHAPTER CONTAINS MANDATORY SAFETY REQUIREMENTS.           ║
║  These are not recommendations. They are requirements.          ║
║  Non-compliance may result in death or permanent injury.        ║
╚══════════════════════════════════════════════════════════════════╝
```

This chapter establishes safety requirements for operating HF Small Transmitting Loop antennas where the tuning capacitor stores high voltage. VHF/UHF quad loop antennas (no capacitor) are exempt from high-voltage requirements but are subject to general RF safety requirements.

### 12.2 Voltage Hazard Classification

The following table classifies each band by hazard level:

| Band | V_cap @ 5W | V_cap @ 100W | Hazard Class | Precautions |
|------|-----------|-------------|-------------|-------------|
| 160m | 2,791 V | 12,480 V | EXTREME | Vacuum cap, 20kV wire, remote operation |
| 80m | 2,379 V | 10,640 V | EXTREME | Vacuum cap, 12kV wire, remote operation |
| 40m | 1,738 V | 7,770 V | SEVERE | Air-var (wide gap), HV wire, no-touch policy |
| 30m | 1,775 V | 7,940 V | SEVERE | Same as 40m |
| 20m | 1,682 V | 7,520 V | SEVERE | Same as 40m |
| 17m | 957 V | 4,280 V | HIGH | NP0/SM cap, 2kV wire, HV awareness |
| 15m | 966 V | 4,320 V | HIGH | Same as 17m |
| 12m | 969 V | 4,334 V | HIGH | Same as 17m |
| 10m | 963 V | 4,307 V | HIGH | Same as 17m |

### 12.3 The Bleeder Resistor

**Specification:** 100kΩ, 5W, metal film, ±5%.  
**Location:** Directly across tuning capacitor terminals, inside cap housing.  
**Test:** Measure with ohmmeter before each operating session. If resistance is not 90kΩ–110kΩ, replace immediately.

**Bleeder operation:**
The bleeder continuously dissipates a small amount of transmit power. At 5W input, power in bleeder = V²/R = (2791²/100,000) = 78 mW. This is within the 5W rating with substantial margin.

The bleeder also provides a path to discharge electrostatic accumulation. In dry conditions, a standing loop antenna can accumulate several hundred volts of static charge from atmospheric electricity. Without the bleeder, this charge has no discharge path and presents a shock hazard independent of transmit power.

### 12.4 Capacitor Selection and Voltage Rating

**Never** substitute a capacitor with a lower voltage rating than specified. The rating margin is not arbitrary:

```
  Required rating = V_cap (from Appendix D) × Safety Factor

  Safety Factor = 2.0 minimum for 160m–40m
                = 1.5 minimum for 30m–10m

  Example: 40m at 100W:
  V_cap = 7,770 V
  Required rating = 7,770 × 2.0 = 15,540 V
  Use: Air-variable rated ≥ 15 kV, or vacuum cap rated ≥ 15 kV
```

**Plate spacing for air-variable capacitors:**
Air breakdown field strength: approximately 3 MV/m (3 kV/mm) at sea level.

```
  Required plate spacing = V_cap / E_breakdown × Safety Factor
  
  40m at 100W: spacing = 7,770 / 3,000,000 × 2.0 = 5.18 mm minimum
  160m at 5W:  spacing = 2,791 / 3,000,000 × 2.0 = 1.86 mm
  160m at 100W: spacing = 12,480 / 3,000,000 × 2.0 = 8.32 mm minimum
```

Standard air-variable capacitors have plate spacing of 0.5–2mm and are unsuitable for HF magnetic loops at any meaningful power level without extensive modification. Use vacuum capacitors for 160m, 80m, and any band at power levels > 5W.

### 12.5 Personal Safety Rules

These rules apply to any person within the capacitor housing area during or after TX:

**Rule 1 — No-Contact During TX:**  
No person shall contact the capacitor housing, its leads, or any conductor connected to the capacitor while the transmitter is keyed.

**Rule 2 — 30-Second Delay After TX:**  
After ceasing transmission, wait 30 seconds before approaching the capacitor. The bleeder resistor discharges the capacitor in microseconds for RF purposes, but static charge and residual dielectric charge can persist. The 30-second delay allows for complete discharge through all paths.

**Rule 3 — Announce Transmit:**  
In a multi-person operating environment, announce "transmitting" or "key down" before operating. Agree on a hand signal if working in a noisy environment.

**Rule 4 — Children and Untrained Persons:**  
Persons without training in high-voltage hazards shall not operate or approach an HF STL system during transmit or within 30 seconds after transmit.

**Rule 5 — One-Hand Rule:**  
When troubleshooting a live circuit (not recommended; de-power first), work with one hand only. Keep the other hand at your side or in a pocket. This prevents current from crossing the heart if accidental contact occurs.

**Rule 6 — No Operation During Degraded Conditions:**  
Do not operate if:
- Bleeder resistor is missing, damaged, or unmeasured
- Capacitor housing is cracked or open
- Motor wiring is exposed or pinched
- SWR is abnormally high (possible arc or failure mode)
- Condensation is present on the cap housing (risk of surface arc)

### 12.6 First Aid — Electric Shock

```
  IF A PERSON HAS RECEIVED AN ELECTRIC SHOCK:

  1. DO NOT TOUCH THE VICTIM until power is disconnected.
     You will become a second victim.

  2. Disconnect power: unplug the transmitter, or open the power breaker.

  3. If victim is unresponsive and not breathing: start CPR immediately.
     Call emergency services (911 in USA).

  4. Even if the victim appears to have recovered, insist on medical evaluation.
     Cardiac arrhythmia can develop hours after shock.

  5. Document the incident. Determine the cause before reactivating equipment.
```

---

## CHAPTER 13: MAINTENANCE AND FIELD REPAIR

### 13.1 Preventive Maintenance Schedule

```
  Before each operating session (15 minutes):
  □ Measure bleeder resistor: 100kΩ ±10%
  □ Inspect cap housing for cracks, moisture, loose screws
  □ Verify limit switches trigger by hand-rotating cap shaft
  □ Power on CYD; confirm BT connects to tuner within 30 seconds
  □ Run home sequence; verify motor moves smoothly
  □ Visual inspection of loop wire: no kinks, cuts, or corrosion

  After each field deployment (30 minutes):
  □ Wipe loop wire with dry cloth; inspect for sharp bends
  □ Inspect corner connectors for hairline cracks (pool noodle UV degradation)
  □ Check motor pinion and main gear for wear (tooth chipping)
  □ Clean motor wiring connector; apply dielectric grease if used in rain
  □ Run firmware NVS backup: save band and cap_pos to paper log

  Annual:
  □ Replace motor (28BYJ-48 brushless winding life: ~5000 hours)
  □ Re-measure coupling loop resonance with antenna analyzer
  □ Verify AD8307 calibration against external power reference
  □ Re-torque all M3 fasteners
```

### 13.2 Field Repairs — Pool Noodle Frame

**Broken corner connector:**

Pool noodle connectors are PETG printed parts. PETG is UV-resistant and impact-tolerant but can crack if bent against its layer lines.

```
  Field repair (no spare connector available):
  a. Wrap the cracked corner with 3 layers of self-fusing silicone tape.
  b. Insert a wooden splint (section of tent stake or similar) through the
     connector bore across the break.
  c. This repair is mechanically adequate for static use.
  d. Replace connector at next maintenance opportunity.
```

**Pool noodle section cracked or kinked:**

Pool noodle cracks do not affect RF performance (the foam is not an RF element). However, structural integrity is needed to maintain loop geometry.

```
  Field repair:
  a. Wrap damaged section with 2 layers of duct tape.
  b. Slide in a wood or fiberglass dowel through the noodle center bore (ID = 25mm).
  c. Proceed with operation.
```

### 13.3 Field Repairs — Motor System

**Motor does not respond (LED indicators on ULN2003 board not lighting):**

```
  Check:
  1. 5V power present at ULN2003 VCC pin.
  2. GPIO pins 25/26/27/14 are outputting signal (test with LED + 470Ω to GND from each pin).
  3. ULN2003 IN1–IN4 are receiving signal from ESP32.

  Common failure: connector to motor disconnected. Re-seat JST connector on motor pigtail.
  Second common failure: 5V regulator output low (motor current causes dropout).
     Check: 5V rail should be 4.8V–5.2V under motor load.
     Fix: Increase filter capacitor on 5V rail (add 470μF electrolytic).
```

**Motor running but capacitor not moving (gear slip):**

```
  Cause: 10T pinion is loose on 28BYJ-48 shaft.

  Fix:
  a. Remove motor from housing.
  b. Press pinion fully onto shaft flat.
  c. Apply one drop of Loctite 243 (medium strength) to shaft-pinion interface.
  d. Allow 30 minutes cure before reinstalling.

  Prevention: During initial installation, verify pinion is pressed fully flush
              to shaft end (no gap between pinion face and shaft shoulder).
```

**Limit switch not triggering (motor runs past end of travel):**

```
  Symptoms: Motor stalls against mechanical end of capacitor travel.
            Grinding noise; firmware continues stepping.

  Emergency stop: Remove power immediately.

  Fix:
  a. Inspect capacitor shaft and gear for damage.
  b. Realign limit switch to trigger at 5° before mechanical end stop.
  c. Test: manually rotate capacitor to near end of travel; confirm switch
           triggers before mechanical contact.
```

### 13.4 Field Repairs — Electronics

**AD8307 giving constant output (no variation with power):**

```
  Likely cause: Input protection diode conducting, or input pin shorted.

  Verification:
  a. Apply known RF power (use phone as signal source or local transmitter at 1W).
  b. Measure AD8307 output pin with voltmeter. Should be 400–800 mV for typical HF power.
  c. If output is stuck at 0V: power supply to AD8307 (VCC pin) may be missing.
  d. If output is stuck at rail: input overloaded or pin shorted to GND.

  Field workaround (if only forward detector is failed):
  Assume SWR = 1.5:1 and tune manually by ear (peak in received signal
  corresponds to minimum SWR). This is the traditional method; it works.
```

**CYD not connecting via Bluetooth:**

```
  Steps:
  a. Verify ESP32 tuner is powered and running (blue LED active).
  b. On CYD: power cycle. BT will re-scan automatically.
  c. If still not connecting: on ESP32, push reset button. This restarts BT advertising.
  d. Verify no other device has paired with PNML-Tuner (only one BT connection).
     If another phone/device is connected, disconnect it.
  e. Last resort: hold BOOT button on ESP32 for 10 seconds to clear BT pairing.
     Re-pair from scratch.
```

### 13.5 Storage

```
  Short-term (< 1 week):
  a. Coil loop wire loosely (minimum bend radius 10× wire diameter).
     #14 AWG: min bend radius = 16mm.
  b. Disassemble corner connectors from noodle sections.
  c. Wrap in dry cloth or store in PVC tube.

  Long-term (> 1 month):
  a. Remove capacitor from housing. Store in sealed bag with desiccant.
  b. Spray loop wire with WD-40 or clear lacquer to prevent oxidation.
     Bare copper oxidizes in 2–6 weeks in humid environments.
     Oxidation increases Rloss and reduces Q.
  c. Remove batteries from CYD and ESP32 systems.
  d. Print replacement corner connectors before next deployment
     (UV degrades PETG over months of direct sunlight).
```

---

## APPENDIX A: BAND DATA SHEETS

### A.1 160 Meter Band Data Sheet

```
  TM-PNL-A001 — 160m Pool Noodle Magnetic Loop

  Design Frequency:     1.900 MHz
  Band:                 160m (1.800–2.000 MHz)

  GEOMETRY:
  Loop type:            Small Transmitting Loop (STL), square
  Loop side:            2.00 m
  Loop perimeter:       8.00 m
  Loop height:          1.00 m (center above ground, typical)
  Number of turns:      4
  Wire gauge:           #12 AWG bare copper
  Wire radius:          1.028 mm
  Coupling loop side:   0.40 m (1/5 of main)

  COMPUTED PARAMETERS:
  Inductance L_N:       168.11 μH
  Resonating cap C:     41.7 pF
  Skin depth δ:         47.6 μm (at 1.900 MHz)
  Radiation resistance: 0.0182 Ω
  Loss resistance:      0.121 Ω
  Q factor:             891
  Efficiency:           12.89%
  3-dB Bandwidth:       2.13 kHz

  SAFETY DATA:
  V_cap at 5W:          2,791 V peak
  V_cap at 100W:        12,480 V peak
  Capacitor type:       VACUUM VARIABLE MANDATORY
  Minimum C rating:     15 kV
  Plate spacing (air):  N/A — vacuum cap required
  Bleeder resistor:     100kΩ / 5W mandatory

  MATCH NOTE:
  DANGER: >2.8kV@5W. Bleeder 100kΩ mandatory. Vacuum cap required.
  At 100W legal limit: 12.48 kV peak. Lethal without precautions.

  NEC MODEL: TM-PNL-NEC-001_160m_pnml.nec
```

### A.2 40 Meter Band Data Sheet

```
  TM-PNL-A003 — 40m Pool Noodle Magnetic Loop

  Design Frequency:     7.150 MHz
  Band:                 40m (7.000–7.300 MHz)

  GEOMETRY:
  Loop type:            STL, square
  Loop side:            1.20 m
  Loop perimeter:       4.80 m
  Number of turns:      3
  Wire gauge:           #14 AWG bare copper
  Wire radius:          0.814 mm
  Coupling loop side:   0.24 m

  COMPUTED PARAMETERS:
  Inductance L_N:       23.55 μH
  Resonating cap C:     21.0 pF
  Skin depth δ:         24.5 μm
  Radiation resistance: 0.139 Ω
  Loss resistance:      0.338 Ω
  Q factor:             807
  Efficiency:           29.23%
  3-dB Bandwidth:       8.86 kHz

  SAFETY DATA:
  V_cap at 5W:          1,738 V peak
  V_cap at 100W:        7,770 V peak
  Capacitor type:       Air-variable, ≥8 kV rating, ≥5mm plate spacing
  Bleeder resistor:     100kΩ / 5W mandatory

  MATCH NOTE:
  CAUTION: >1.7kV@5W. Air-variable cap. Do not touch cap during TX.

  NEC MODEL: TM-PNL-NEC-003_40m_pnml.nec
```

### A.3 6 Meter Band Data Sheet

```
  TM-PNL-A010 — 6m Pool Noodle Quad Loop

  Design Frequency:     51.000 MHz
  Band:                 6m (50.000–54.000 MHz)

  GEOMETRY:
  Loop type:            Full-wave resonant quad
  Loop side:            1.397 m
  Loop perimeter:       5.588 m
  Number of turns:      1 (full-wave, no cap)
  Wire gauge:           #14 AWG bare copper

  COMPUTED PARAMETERS:
  Input impedance:      ~125Ω at resonance
  Matching:             2.5:1 UNUN (FT-50-61, 7T/11T)
  Estimated Q:          600
  Efficiency:           98.0%
  3-dB Bandwidth:       85 kHz

  SAFETY DATA:
  Capacitor voltage:    N/A — no tuning capacitor
  RF safety:            Standard; no high-voltage hazard

  MATCH NOTE:
  Full-wave quad. Side=1.397m. 2.5:1 UNUN or gamma match to 50Ω.

  NEC MODEL: TM-PNL-NEC-010_6m_quad.nec
```

*(Additional band data sheets for 80m, 30m, 20m, 17m, 15m, 12m, 10m, 2m, 1.25m, 70cm, 33cm, 20cm follow the same format. Complete data in `calculations/loop_dimensions.csv`.)*

---

## APPENDIX B: BILL OF MATERIALS

### B.1 Single-Band HF STL (40m Example)

| Ref | Description | Qty | Notes |
|-----|------------|-----|-------|
| L1 | Pool noodle, OD=47mm, 1.2m section | 4 | Cut to 120cm |
| L1 | #14 AWG bare copper wire | 20m | Loop + coupling loop |
| C1 | Air-variable capacitor, 10–100pF, ≥8kV | 1 | Butterfly preferred |
| R1 | 100kΩ / 5W metal film | 1 | BLEEDER — MANDATORY |
| M1 | 28BYJ-48 stepper motor, 5V | 1 | With ULN2003 driver board |
| G1 | 10T pinion, mod=1, press-fit | 1 | Print: TM-PNL-ENC-001 |
| G2 | 40T gear, mod=1, 6mm bore | 1 | Print: TM-PNL-ENC-001 |
| ENC1 | Cap housing body | 1 | Print: TM-PNL-ENC-001, PETG |
| ENC2 | Cap housing lid | 1 | Print: TM-PNL-ENC-001, PETG |
| CONN1–4 | Loop corner connector | 4 | Print: TM-PNL-ENC-002, PETG |
| SW1 | SPST-NO limit switch (min) | 1 | PCB mount, 5A rating |
| SW2 | SPST-NO limit switch (max) | 1 | Same |
| MCU | ESP32-WROOM-32 dev kit | 1 | With USB for programming |
| U1,U2 | AD8307ANZ log amplifier | 2 | PDIP-8 |
| T1 | BN-43-2402 binocular core | 1 | Bruene coupler |
| J1 | BNC-F chassis connector | 1 | Coax to coupling loop |
| J2 | DC barrel jack, 2.1mm | 1 | 12V power input |
| REG1 | AMS1117-5.0 voltage regulator | 1 | Motor power |
| REG2 | AMS1117-3.3 voltage regulator | 1 | ESP32 power |
| CYD | ESP32-2432S028R display | 1 | CYD; separate power |
| — | M3 × 8mm SHCS, stainless | 12 | Misc mounting |
| — | M3 heat-set inserts, 4.6×4mm | 8 | Corner connectors |
| — | RG-58 coax, 1m | 1 | Coupling loop feed |
| — | Self-fusing silicone tape, 1m | 1 | Coupling loop mount |
| — | Ferrite bead, Fair-Rite 2643002402 | 1 | Common-mode choke |

**Total estimated cost (USA, 2026):** $45–65 per band (excluding tools and 3D printer filament)

### B.2 Additional Parts for Multi-Band Roll-up Version

| Ref | Description | Qty | Notes |
|-----|------------|-----|-------|
| TRAP1-6 | T50-2 or T50-6 toroids | 6 | Per band (see trap table) |
| C_trap 1-6 | NP0/Silver-mica capacitors | 6 | Per trap design |
| SPOOL | Roll-up spool | 1 | Print: TM-PNL-ENC-003, PETG |
| — | #18 AWG wire for traps | 4m total | —  |
| — | #12 AWG main loop wire | 8m | Outermost band perimeter |
| — | Hook-and-loop strap, 25mm × 1m | 1 | Transport |

---

## APPENDIX C: NEC MODEL SUMMARY

### C.1 HF STL NEC Models (TM-PNL-NEC-001 through -009)

All HF models use the same geometry template:
- Four wire segments forming the main loop (GW 1–4)
- Four wire segments forming the Faraday coupling loop (GW 5–8)
- Ground model: Sommerfeld-Norton (GN 2)
- Tuning cap: LD card (series R + X_C, negative reactance = capacitive)
- Excitation: EX on coupling loop midpoint

**NEC-2 Limitations:**
1. Multi-turn coils cannot be directly modeled. Models use single-turn with enhanced loss resistance representing N-turn equivalent.
2. Near-field interactions between close wire turns are not captured.
3. Impedance results at the tuning capacitor represent reactance cancellation only; absolute impedance accuracy is ±20%.

**For accurate impedance:** Use antenna analyzer with actual hardware. The NEC model provides gain pattern and relative performance comparison only.

### C.2 VHF/UHF Quad NEC Models (TM-PNL-NEC-010 through -015)

VHF/UHF models are single-turn full-wave loops:
- Four wire segments (GW 1–4)
- Feed point: GW 2 midpoint (EX card)
- Ground model: free-space (GN −1) for initial analysis
- Frequency sweep: ±5% around design frequency

NEC-2 accuracy is good for single-conductor full-wave loops at VHF (structures much larger than wire diameter). Pattern and impedance accuracy: ±2 dBi, ±10% impedance.

---

## APPENDIX D: CAPACITOR VOLTAGE SAFETY REFERENCE

### D.1 Voltage at All Power Levels — Complete Table

```
  Formulas:
  V_peak = I_peak × X_C
  I_peak = √(2P/(Rrad+Rloss))
  X_C = 1/(ω₀C) = ω₀L_N  (at resonance)
```

| Band | V@1W | V@5W | V@10W | V@25W | V@50W | V@100W |
|------|------|------|-------|-------|-------|--------|
| 160m | 1,247V | 2,791V | 3,946V | 6,240V | 8,826V | 12,480V |
| 80m | 1,063V | 2,379V | 3,363V | 5,317V | 7,517V | 10,630V |
| 40m | 777V | 1,738V | 2,457V | 3,884V | 5,493V | 7,770V |
| 30m | 794V | 1,775V | 2,509V | 3,967V | 5,609V | 7,930V |
| 20m | 752V | 1,682V | 2,378V | 3,759V | 5,316V | 7,520V |
| 17m | 428V | 957V | 1,353V | 2,139V | 3,024V | 4,280V |
| 15m | 432V | 966V | 1,366V | 2,159V | 3,052V | 4,316V |
| 12m | 433V | 969V | 1,370V | 2,166V | 3,062V | 4,330V |
| 10m | 430V | 963V | 1,361V | 2,151V | 3,042V | 4,300V |

All values in Volts peak. AC RMS = peak / √2. 

**Safe handling voltage:** Human skin resistance = 1kΩ–100kΩ. At 100V, current through 1kΩ resistance = 100 mA. Threshold for ventricular fibrillation ≈ 60 mA for 1 second exposure. Treat ALL voltages in this table as potentially lethal.

### D.2 Capacitor Power Dissipation

A finite Q capacitor also dissipates power. For a capacitor Q_c = 1000 (typical for high-quality air-variable):

```
  P_cap = I² × X_C / Q_c

  Example: 40m at 5W:
  I = 1.46A rms (from V@5W and X_C)
  X_C = 1/(2π × 7.15e6 × 21e-12) = 1059 Ω
  P_cap = 1.46² × 1059 / 1000 = 2.26W

  At 5W input, 2.26W is dissipated in the capacitor!
  At 100W input: 45W in the capacitor.
```

This is why capacitor temperature rating matters. Commercial air-variable capacitors typically dissipate 1–5W maximum. For sustained high-power operation, use either a vacuum capacitor (Q_c > 5000) or limit TX duty cycle.

---

## APPENDIX E: TROUBLESHOOTING GUIDE

### E.1 No SWR Dip Found

```
  Symptom: Swept 1MHz either side of band center; no SWR dip visible.

  Cause 1: Capacitor range does not include resonance.
    Check: Is the band's calculated C within the capacitor's range?
    Fix: Use a capacitor with wider range, or add a small fixed cap in parallel.

  Cause 2: Motor home position incorrect.
    Check: Manually rotate capacitor through full range while monitoring SWR.
    Fix: Re-home motor; verify limit switch positions.

  Cause 3: Coupling loop not connected, or coax open.
    Check: Continuity from BNC through coax to coupling loop.
    Fix: Re-solder coupling loop connections.

  Cause 4: Main loop wire not connected to capacitor (break in wire).
    Check: Continuity around main loop perimeter.
    Fix: Re-solder or re-crimp broken junction.
```

### E.2 SWR Dip Present but Cannot Achieve < 2:1

```
  Symptom: SWR dip exists at resonance but minimum SWR is 2:1 or higher.

  Cause 1: Coupling too loose.
    Fix: Move coupling loop closer to main loop center (increase depth).

  Cause 2: Coupling too tight (over-coupling).
    This is rare for small coupling loops. Symptom: very broad, flat dip.
    Fix: Move coupling loop slightly away from main loop.

  Cause 3: Main loop has excessive loss (Q too low).
    Possible cause: corroded wire, poor solder joints, lossy nearby materials.
    Check: Tap the loop wire; listen for change in SWR dip. This indicates
           intermittent contact.
    Fix: Re-solder all joints. Move loop away from metallic objects.

  Cause 4: Bleeder resistor value incorrect (too low → loads the circuit).
    Check: Should be 100kΩ. Verify with ohmmeter.
    Fix: Replace with correct value.
```

### E.3 SWR Drifts After Tuning

```
  Symptom: Tune completes, SWR = 1.2:1. Five minutes later, SWR = 2.5:1.

  Cause 1: Temperature affecting capacitance (thermal drift).
    STL antennas are sensitive to temperature because the high-Q circuit
    has a narrow resonance. A 0.1pF change in capacitance can shift
    resonance by several kHz on 40m.
    Fix: Allow 10 minutes warm-up time after motor operation (motor heat
         affects capacitor). Re-tune after warm-up.

  Cause 2: Main loop conductors moving in the wind.
    Even small physical movement changes loop inductance.
    Fix: Secure corner connectors and wire routing before operating.
         Guy the loop frame in wind conditions.

  Cause 3: Capacitor mechanical instability (backlash in gear).
    Fix: Verify gear backlash is < 0.5°. Adjust motor pinion position.
```

### E.4 Motor Oscillates (Hunting)

```
  Symptom: During auto-tune, motor oscillates back and forth around a position,
           never locking.

  Cause 1: Step size TUNE_STEP_MIN too small (below position resolution of capacitor).
    Fix: Increase TUNE_STEP_MIN to 250 or 500.

  Cause 2: SWR noise floor too high (RF noise, poor ADC reading).
    The SWR measurement has ~0.1:1 noise floor. If the SWR improvement at
    each step is less than this, the algorithm cannot determine direction.
    Fix: Increase ADC_OVERSAMPLE to 128 or SWR_AVERAGE_N to 16.

  Cause 3: TX power too low (SWR reading invalid).
    Fix: Increase TX power to ≥1W during tune cycle.
```

---

## APPENDIX F: REFERENCES AND STANDARDS

### F.1 Technical References

| Reference | Title | Relevance |
|-----------|-------|----------|
| ARRL Antenna Book, 25th Ed. | Chapter 5: Loops | Primary reference for STL design |
| Terman, F.E., "Radio Engineers Handbook" (1943) | Section 2: Inductance | L calculation methods |
| Underhill, M.J., "The Basics of Loop Antennas" | Microwave Journal, 2006 | Q, efficiency, coupling |
| Hart, T., "Small High Efficiency Loop Antennas" | QST, June 1986 | Construction practice |
| AD8307 Datasheet | Analog Devices | Log amplifier characteristics |
| "Directional Couplers" | Pozar, Microwave Engineering, Ch. 7 | Bruene coupler theory |
| NEC-2 User's Manual | Burke & Poggio, 1981 | NEC-2 limitations and best practices |
| 28BYJ-48 Datasheet | Minebea/Generic | Stepper motor characteristics |
| ILI9341 Datasheet | Ilitek | TFT display interface |

### F.2 Safety Standards

| Standard | Title | Application |
|----------|-------|------------|
| NFPA 70E | Standard for Electrical Safety in the Workplace | High-voltage safe work practices |
| IEEE C95.1 | Safety Levels with Respect to Human Exposure to RF | RF power exposure limits |
| ARRL RF Safety Guide | RF Safety for Amateur Radio | Practical RF safety |
| FCC Part 97 | Amateur Radio Service | Legal operating limits |

### F.3 Related Project Files

All project files are located in `/home/merv/Public/claude/SC/pool_noodle_loop/`:

```
  pool_noodle_loop/
  ├── calculations/
  │   ├── loop_dimensions.csv             (15 bands, all computed values)
  │   ├── loading_coil_tables.csv         (booster coil options)
  │   ├── trap_winding_tables.csv         (6 LC traps)
  │   └── resonator_winding_tables.csv    (capacitor types per band)
  ├── enclosures/
  │   ├── cap_housing.scad                (TM-PNL-ENC-001)
  │   ├── loop_connector.scad             (TM-PNL-ENC-002)
  │   └── rollup_spool.scad               (TM-PNL-ENC-003)
  ├── firmware/
  │   ├── esp32_loop_tuner/
  │   │   ├── config.h                    (TM-PNL-FW-001)
  │   │   └── esp32_loop_tuner.ino
  │   └── cyd_loop_display/
  │       ├── config.h                    (TM-PNL-FW-002)
  │       └── cyd_loop_display.ino
  ├── manual/
  │   └── POOL_NOODLE_LOOP_MANUAL.md      (this document, TM-PNL-001)
  ├── nec_models/
  │   ├── TM-PNL-NEC-001_160m_pnml.nec
  │   ├── TM-PNL-NEC-002_80m_pnml.nec
  │   ├── TM-PNL-NEC-003_40m_pnml.nec
  │   ├── TM-PNL-NEC-004_30m_pnml.nec
  │   ├── TM-PNL-NEC-005_20m_pnml.nec
  │   ├── TM-PNL-NEC-006_17m_pnml.nec
  │   ├── TM-PNL-NEC-007_15m_pnml.nec
  │   ├── TM-PNL-NEC-008_12m_pnml.nec
  │   ├── TM-PNL-NEC-009_10m_pnml.nec
  │   ├── TM-PNL-NEC-010_6m_quad.nec
  │   ├── TM-PNL-NEC-011_2m_quad.nec
  │   ├── TM-PNL-NEC-012_125cm_quad.nec
  │   ├── TM-PNL-NEC-013_70cm_quad.nec
  │   ├── TM-PNL-NEC-014_33cm_quad.nec
  │   └── TM-PNL-NEC-015_20cm_quad.nec
  ├── schematics/
  │   ├── magnetic_loop_circuit_ascii.txt  (TM-PNL-SCH-001)
  │   ├── tuning_capacitor_ascii.txt       (TM-PNL-SCH-002)
  │   └── matching_coupling_ascii.txt      (TM-PNL-SCH-003)
  └── sketches/
      ├── pool_noodle_loop_overview.png
      ├── faraday_coupling_detail.png
      └── rollup_assembly.png
```

---

## END OF TECHNICAL MANUAL

```
  TM-PNL-001 Rev A — Pool Noodle Magnetic Loop Antenna System
  Document complete.

  Prepared by: Merv Martin
  Date: 2026-05-08
  Total pages: See rendered output

  ─────────────────────────────────────────────────────────────────
  73 DE N6IMV — Good luck with the build. Watch those voltages.
  ─────────────────────────────────────────────────────────────────
```
