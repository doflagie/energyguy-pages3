# TECHNICAL MANUAL — 5.6 METER TELESCOPIC WHIP ANTENNA SYSTEM
## TECHNICAL DATA SHEET

**Document Number:** TM-MANUAL-001 Rev A
**Equipment:** TECHNICAL MANUAL — 5.6 METER TELESCOPIC WHIP ANTENNA SYSTEM
**Classification:** UNCLASSIFIED — Amateur Radio / Field Use
**Date:** 2026-05-24
**Supersedes:** None (initial issue)

---

## RECORD OF CHANGES

| Change No. | Rev | Date | Description | By |
|:---:|:---:|:---:|---|:---:|
| 1 | A | 2026-05-24 | Initial formatted release | M. Martin |

---

## Multiband HF/VHF/UHF Operation, 160 Meters Through 20 Centimeters

**Document Control:** TM-WHP-001 Rev A
**Prepared:** 2026-05-06
**Author:** M. Martin
**Classification:** UNCLASSIFIED
**Status:** Released

---

## TABLE OF CONTENTS

- [Chapter 1: Introduction and System Description](#chapter-1-introduction-and-system-description)
- [Chapter 2: Antenna Physics — Electrically Short Monopole](#chapter-2-antenna-physics--electrically-short-monopole)
- [Chapter 3: Loading Coil Design and Construction](#chapter-3-loading-coil-design-and-construction)
- [Chapter 4: Telescoping Whip Extension Guide](#chapter-4-telescoping-whip-extension-guide)
- [Chapter 5: Directional Coupler Theory and SWR Measurement](#chapter-5-directional-coupler-theory-and-swr-measurement)
- [Chapter 6: LC Traps — Roll-Up Wire Version](#chapter-6-lc-traps--roll-up-wire-version)
- [Chapter 7: NEC Modeling and Simulation](#chapter-7-nec-modeling-and-simulation)
- [Chapter 8: Electronic Instrumentation](#chapter-8-electronic-instrumentation)
- [Chapter 9: Construction — Base Unit and Matching Network](#chapter-9-construction--base-unit-and-matching-network)
- [Chapter 10: Field Setup Checklist and Operating Procedures](#chapter-10-field-setup-checklist-and-operating-procedures)
- [Chapter 11: Troubleshooting](#chapter-11-troubleshooting)
- [Chapter 12: Parts Reference](#chapter-12-parts-reference)
- [Appendix A: Complete Loading Coil Winding Table](#appendix-a-complete-loading-coil-winding-table)
- [Appendix B: Complete Trap Winding Table](#appendix-b-complete-trap-winding-table)
- [Appendix C: Whip Extension Quick Reference](#appendix-c-whip-extension-quick-reference)
- [Appendix D: NEC Quick Reference Card](#appendix-d-nec-quick-reference-card)
- [Appendix E: Quick Reference Field Card](#appendix-e-quick-reference-field-card)

---

## SAFETY NOTICES

> **WARNING**
> DO NOT erect or operate this antenna during electrical storms or when lightning is in the area. Disconnect and collapse the whip during such conditions. A 5.6-meter vertical antenna constitutes a significant lightning attraction hazard.

> **WARNING**
> DO NOT operate this antenna within falling distance of overhead power lines. The 5.6-meter fully extended whip can contact distribution voltages if the antenna falls or is carried without regard to clearance. Death or serious injury will result from contact with energized conductors.

> **WARNING**
> DO NOT exceed the power ratings of the loading coil and L-network components. The 160-meter coil (T200-2, 31 turns, #22 AWG) is rated for approximately 50 watts continuous into a matched load. Exceeding this limit may cause coil overheating and fire.

> **CAUTION**
> The tip section (Section 7) of the extended whip is capable of inflicting serious eye or facial injury. Fit the rubber tip cap when transporting or storing the collapsed whip. Remove the tip cap before transmitting.

> **CAUTION**
> Verify that all sections are fully locked before applying RF power. A partially seated section creates a high-resistance joint that will arc and pit the mating surfaces. Inspect all locking collars before each use.

> **NOTE**
> This manual references NEC-2 simulation files TM-WHP-NEC-001 through TM-WHP-NEC-012. These files are located in the nec_models/ directory of this project. Results are confirmed by calculation; field verification is the operator's responsibility.

---

# CHAPTER 1: INTRODUCTION AND SYSTEM DESCRIPTION

## 1.1 Purpose and Scope

This manual provides technical documentation for the 5.6-meter telescopic whip antenna system. The system is designed for portable HF/VHF/UHF operation from 1.8 MHz (160 meters) through 1.3 GHz (23 centimeters). The primary application is field propagation research, QRP operation, and mobile station deployment.

The manual covers system theory, construction, operation, calibration, and troubleshooting. It is intended for technically qualified operators and builders who require complete engineering data rather than simplified operating instructions.

This document does not cover transceiver operation, frequency coordination, or licensing requirements. The operator is responsible for compliance with applicable regulations.

## 1.2 System Components

The system consists of the following major assemblies:

### 1.2.1 5.6-Meter Telescoping Whip

- **Configuration:** 7 sections, each nominally 0.80 meters
- **Extended length:** 5.60 meters total
- **Collapsed length:** approximately 0.36 meters
- **Section diameters (base to tip):** 20 mm / 17 mm / 14 mm / 12 mm / 10 mm / 7 mm / 4 mm
- **Average conductor radius for NEC models:** 6 mm (0.006 m)
- **Material:** Stainless steel or aluminum alloy tubing
- **Base mount:** 3/8-24 threaded stud, standard CB/amateur mobile fitting
- **Velocity factor:** 0.97 (metallic rod)
- **Section locking:** Friction collars; twist-to-lock mechanism

### 1.2.2 Modular Base Loading Coil Set

Five plug-in loading coil modules provide inductive reactance to resonate the electrically short whip on lower HF bands. Each module presents a series inductance at the base of the whip. Modules are identified by color-coded housings.

| Module | Band | Core | Inductance | Color |
|--------|------|------|-----------|-------|
| 1 | 160 m | T200-2 | 144.6 µH | Red |
| 2 | 80 m | T130-2 | 35.5 µH | Orange |
| 3 | 60 m | T130-2 | 16.3 µH | Yellow |
| 4 | 40 m | T106-2 | 7.85 µH | Green |
| 5 | 30 m | T50-2 | 2.477 µH | Blue |

### 1.2.3 L-Network Base Unit with NMO Coil Socket

The base unit contains:

- An NMO-style bayonet socket accepting the plug-in loading coil modules
- A low-pass L-network with variable shunt capacitor (20 to 500 pF air-variable or switched)
- An SO-239 coaxial input connector
- Four banana jacks for counterpoise/radial wire attachment
- A ground stud (M6 bolt) for bonding to vehicle or support structure
- Internal wiring to the SWR bridge board

### 1.2.4 SWR Monitoring Bridge

The SWR bridge is a Bruene-type directional coupler based on the following components:

- FT-37-43 toroid, 1-turn primary (pass-through), 10-turn secondary, #28 AWG enameled wire
- Dual AD8307 logarithmic detector ICs, one for forward power and one for reflected power
- 51-ohm termination resistors for both coupler ports
- 100 nF AC-coupling capacitors at each AD8307 input
- 100 nF filter capacitors at each AD8307 output
- SMA connectors on both through-line ports

The coupler is rated for 1 to 150 MHz at power levels from -73 dBm to +47 dBm (50 watts into 50 ohms). Directivity is 35 to 40 dB when using the AD8307 configuration.

### 1.2.5 ESP32-WROOM-32 Controller

The tuner controller is an ESP32-WROOM-32 module providing:

- Dual 12-bit ADC channels (GPIO 35 and GPIO 39) for the AD8307 outputs
- WiFi access point: SSID "WHP-Tuner," password "whip5p6m," channel 6, IP 192.168.4.1
- Bluetooth Serial (Classic) device named "WHP-Tuner," streaming telemetry at 200 ms intervals
- HTTP server on port 80 with web UI and REST API endpoints
- NVS (non-volatile storage) for band selection and calibration offsets
- Band-up and band-down push buttons (GPIO 4, GPIO 5, active-low)
- Three SWR status LEDs: green (SWR ≤ 1.5), yellow (1.5 < SWR ≤ 2.5), red (SWR > 2.5)
- 64× ADC oversampling and 8-reading averaging for noise immunity

### 1.2.6 CYD Touchscreen Display (ESP32-2432S028)

The Cheap Yellow Display (CYD) provides a 320 × 240 pixel ILI9341 TFT touchscreen interface with:

- Four display pages selectable by touch tab bar: SWR, SETUP, POWER, GUIDE
- Bluetooth Serial master connecting to "WHP-Tuner" with automatic reconnect every 5 seconds
- Full-screen double-buffered sprite rendering at 10 FPS
- SWR bargraph with color-coded fill (green/yellow/red)
- Extension length visualization with 0–5.6 m proportional bar
- Loading coil identification and tuning guidance by band

### 1.2.7 Roll-Up Wire Version with Inline LC Traps

An alternative to the telescoping metallic whip, the roll-up wire version uses a 5.6-meter length of flexible wire with six LC trap assemblies soldered inline. Each trap presents high impedance (effectively an open circuit) at its design frequency, electrically shortening the antenna for operation above the trap frequency. The wire version is lighter and more packable than the telescoping version but does not offer VHF/UHF capability without modification.

## 1.3 Operational Concept

The 5.6-meter whip operates as a quarter-wave monopole on bands where the physical length equals or exceeds the quarter-wave dimension. For bands above 20 meters (17 m, 15 m, 12 m, 10 m), the operator retracts the whip to the appropriate quarter-wave length and feeds it directly with no loading components required.

On bands below 20 meters (30 m, 40 m, 60 m, 80 m, 160 m), the 5.6-meter whip is electrically short relative to the quarter-wave length. It presents a large capacitive reactance at the feedpoint. A base loading coil resonates out this capacitive reactance, converting the whip into an acceptable radiator. An L-network provides impedance transformation from the low radiation resistance to the 50-ohm system impedance.

On the 20-meter band the whip is approximately 9 percent too long for resonance. A 300 pF series NP0 capacitor at the base cancels the resulting small inductive reactance, or the operator may retract Section 7 slightly to approximately 5.13 meters.

On VHF bands the operator retracts the whip further to short lengths. At 2 meters (146 MHz) only Section 1 is used, extended to approximately 488 mm. At 70 centimeters only the stub portion of Section 1 to 160 mm is used.

## 1.4 Performance Summary Table

The following table summarizes system performance across all 15 operating bands. Efficiency values reflect the loading coil loss model only; ground losses depend on the counterpoise system deployed.

| Band | Freq (MHz) | Mode | Extension (m) | Loading | Rrad (Ω) | Coil Eff (%) | Notes |
|------|-----------|------|--------------|---------|----------|-------------|-------|
| 160 m | 1.900 | Base loaded | 5.60 | 144.6 µH coil | 0.124 | 1.8 | Ground loss dominant; 8–16 radials required |
| 80 m | 3.750 | Base loaded | 5.60 | 35.5 µH coil | 0.484 | 14.0 | 4–8 elevated radials; marginal efficiency |
| 60 m | 5.330 | Base loaded | 5.60 | 16.3 µH coil | 0.977 | 33.9 | WARC band; portable QSO capability |
| 40 m | 7.150 | Base loaded | 5.60 | 7.85 µH coil | 1.758 | 60.7 | Practical field antenna; 4 radials adequate |
| 30 m | 10.125 | Base loaded | 5.60 | 2.477 µH coil | 3.525 | 87.7 | Near-resonant; high efficiency; WARC band |
| 20 m | 14.175 | Near-QW | 5.13 or 5.60 | 300 pF cap | 6.913 | ~97 | Series cap trim; retract Sec7 or add cap |
| 17 m | 18.118 | QW resonant | 4.01 | None | 5.787 | ~98 | Retract to 6 sections; no components needed |
| 15 m | 21.225 | QW resonant | 3.43 | None | 5.807 | ~98 | Retract to 5 sections |
| 12 m | 24.940 | QW resonant | 2.92 | None | 5.825 | ~98 | Retract to 4 sections |
| 10 m | 28.500 | QW resonant | 2.55 | None | 5.788 | ~98 | Retract to 4 sections |
| 6 m | 51.000 | QW resonant | 1.40 | None | ~36 | ~99 | 2 sections; ground plane required |
| 2 m | 146.000 | QW monopole | 0.488 | None | ~36 | ~99 | Sec 1 only; coaxial flange ground plane |
| 1.25 m | 222.000 | QW stub | 0.321 | None | ~36 | ~99 | Mark on Sec 1; strict ground plane |
| 70 cm | 446.000 | QW stub | 0.160 | None | ~36 | ~99 | Tip stub; use dedicated antenna if available |
| 33 cm | 906.000 | Stub | 0.079 | Dedicated fixture | ~36 | ~95 | Beyond practical whip range; SMA stub preferred |

---

# CHAPTER 2: ANTENNA PHYSICS — ELECTRICALLY SHORT MONOPOLE

## 2.1 Quarter-Wave Monopole Fundamentals

A quarter-wave monopole over a perfect ground plane is the fundamental reference antenna for this system. At resonance, the feedpoint impedance is purely resistive at approximately 36.5 ohms. The radiation pattern is omnidirectional in azimuth with maximum radiation at low elevation angles.

The physical quarter-wave length including velocity factor is:

```
h_QW = (c / (4 × f)) × VF

where:
  c   = 2.998 × 10⁸ m/s (speed of light)
  f   = frequency in Hz
  VF  = velocity factor (0.97 for metallic rod)
```

Example for 7.150 MHz:

```
h_QW = (2.998e8 / (4 × 7.150e6)) × 0.97
     = 10.484 × 0.97
     = 10.17 m
```

The feedpoint impedance of a quarter-wave monopole over a perfect ground plane is:

```
Zin = Rrad + jX ≈ 36.5 + j0 Ω (at resonance)
```

Over real ground with finite conductivity and a practical radial system, the feedpoint resistance increases due to ground losses. The effective feedpoint resistance becomes:

```
Zfeed = Rrad + Rgnd

where Rgnd = ground loss resistance
            ≈ 5 to 20 Ω depending on radial system quality
```

## 2.2 Electrically Short Monopole

When the physical height h is less than the quarter-wavelength, the monopole is electrically short. Its feedpoint impedance becomes:

```
Zin = Rrad + jXant

where Xant is large and capacitive (negative)
```

**Capacitive Reactance of Short Monopole**

The feed reactance of an electrically short monopole is approximated by:

```
Xant ≈ -Zc × cot(βh)

where:
  Zc   = characteristic impedance of the monopole (Ω)
  β    = 2πf/c = wave number (rad/m)
  h    = physical height (m)
  βh   = electrical length (radians)
```

The characteristic impedance of a tapered monopole is computed using an average radius. For the 5.6-meter whip with average radius a ≈ 6 mm (0.006 m):

```
Zc = 60 × [ln(2h/a) - 1]
   = 60 × [ln(2 × 5.6 / 0.006) - 1]
   = 60 × [ln(1867) - 1]
   = 60 × [7.533 - 1]
   = 60 × 6.533
   = 392 Ω
```

**Electrical Length and Reactance by Band**

| Band | f (MHz) | βh (°) | Xant (Ω) | Rrad (Ω) | Xant/Rrad |
|------|---------|--------|---------|---------|----------|
| 160 m | 1.900 | 12.8° | −1730 | 0.124 | 13,952 |
| 80 m | 3.750 | 25.2° | −833 | 0.484 | 1,722 |
| 60 m | 5.330 | 35.8° | −543 | 0.977 | 556 |
| 40 m | 7.150 | 48.0° | −352 | 1.758 | 200 |
| 30 m | 10.125 | 68.0° | −158 | 3.525 | 45 |
| 20 m | 14.175 | 95.2° | +36 | 6.913 | 5.2 (ind.) |

> **NOTE**
> The 20-meter value shows a small positive (inductive) reactance because the 5.6-meter whip is slightly longer than a quarter-wave at 14.175 MHz. The quarter-wave at this frequency with VF = 0.97 is 5.13 meters. The +36 ohm residual is cancelled by a 300 pF series capacitor.

**Radiation Resistance of Electrically Short Monopole**

For a base-loaded monopole with triangular current distribution:

```
Rrad ≈ 10π² × (h/λ)²

where h/λ = electrical height as a fraction of wavelength
```

| Band | h (m) | λ (m) | h/λ | Rrad (Ω) |
|------|-------|-------|-----|---------|
| 160 m | 5.60 | 157.89 | 0.0355 | 0.124 |
| 80 m | 5.60 | 80.00 | 0.0700 | 0.484 |
| 60 m | 5.60 | 56.28 | 0.0995 | 0.977 |
| 40 m | 5.60 | 41.96 | 0.1335 | 1.758 |
| 30 m | 5.60 | 29.63 | 0.1890 | 3.525 |
| 20 m | 5.13 | 21.16 | 0.2424 | 6.913 |

## 2.3 Loading Coil Resonance

A series inductive reactance at the base of the whip cancels the capacitive reactance, placing the antenna at resonance. The required reactance is:

```
XL = |Xant|
L_required = |Xant| / (2πf)
```

The loading coil has a series resistance determined by its Q factor:

```
Rcoil = XL / Q_coil
```

For powdered-iron toroid cores at HF with proper winding technique, Q is typically 250 to 320 for this system.

**System Efficiency**

Efficiency at the feedpoint, considering only coil loss and radiation resistance:

```
η = Rrad / (Rrad + Rcoil) × 100%
```

**Efficiency Table — All HF Bands**

| Band | f (MHz) | L_req (µH) | Xant (Ω) | Rrad (Ω) | Q_coil | Rcoil (Ω) | η (%) |
|------|---------|-----------|---------|---------|--------|----------|-------|
| 160 m | 1.900 | 144.6 | 1730 | 0.124 | 250 | 6.92 | 1.8 |
| 80 m | 3.750 | 35.5 | 833 | 0.484 | 280 | 2.98 | 14.0 |
| 60 m | 5.330 | 16.3 | 543 | 0.977 | 285 | 1.91 | 33.9 |
| 40 m | 7.150 | 7.85 | 352 | 1.758 | 310 | 1.14 | 60.7 |
| 30 m | 10.125 | 2.477 | 158 | 3.525 | 320 | 0.494 | 87.7 |

> **NOTE**
> Efficiency values above reflect coil loss only. Ground resistance (Rgnd) adds additional loss. With a 4-radial counterpoise and average soil, add approximately 5 to 10 ohms of ground resistance to the denominator. On 160 meters this renders the effective efficiency below 1.5 percent under typical field conditions. A large elevated radial system is essential for useful 160-meter performance.

## 2.4 Effect of Ground and Counterpoise

The return current for a monopole antenna flows through the ground or a counterpoise system. Ground losses are a significant fraction of total system losses, particularly on the lower bands where radiation resistance is small.

**Ground Resistance vs. Radial System**

| Radial Configuration | Rgnd Estimate (Ω) | 40 m Eff with Rgnd |
|---------------------|------------------|--------------------|
| No radials (direct earth spike) | 15–30 | 5–10% |
| 2 radials, λ/8 length | 10–15 | 10–15% |
| 4 radials, λ/4 length (on ground) | 5–10 | 12–26% |
| 4 elevated radials, λ/4 length | 3–6 | 22–37% |
| 8 radials, λ/4 length (on ground) | 3–5 | 26–37% |
| 16 radials, λ/4 length | 2–3 | 33–47% |
| 32 radials, λ/4 length (NEC standard) | 1–2 | 47–64% |

**Minimum radial length** equals the active radiator extension length (5.6 meters on the lower bands, shorter on higher bands).

**Effect of soil conductivity** on radiation resistance is small relative to the effect on ground resistance. Average soil (σ = 0.005 S/m, εr = 13) is modeled in all NEC files in this system.

## 2.5 Whip Extended vs. Retracted Operation

When the whip is retracted to the exact quarter-wave length for a given band (17 m, 15 m, 12 m, 10 m, 6 m, 2 m), it operates as a resonant quarter-wave monopole. In this condition:

- Feedpoint impedance approaches Rrad ≈ 36 ohms (purely resistive)
- No loading coil is required
- An L-network or gamma match provides 50-ohm matching if desired
- Bandwidth is significantly wider than the base-loaded case because system Q is lower

A small residual reactance of approximately −18 to −31 ohms is common even at the marked extension points due to the discrete section steps. This residual is corrected by extending the tip section a few millimeters or by adding a small series capacitor (330 to 470 pF). The SWR will be under 1.5:1 at the band edges in all cases.

---

# CHAPTER 3: LOADING COIL DESIGN AND CONSTRUCTION

## 3.1 Design Equations

**Required inductance:**

```
L_required = |Xant| / (2πf)  [µH when Xant in Ω and f in MHz multiplied appropriately]
           = |Xant| / (2π × f_MHz × 10⁶) × 10⁶  µH
           = |Xant| / (6.2832 × f_MHz)  µH
```

**Wheeler's formula for single-layer air-core solenoid (metric):**

```
L = (r² × N²) / (22.86r + 25.4l)  µH

where:
  r = coil radius in centimeters
  l = winding length in centimeters
  N = number of turns
```

**Toroid inductance (powdered iron):**

```
L = AL × N² / 1000  µH

where:
  AL = inductance per turn squared in nH/T²
  N  = number of turns
```

## 3.2 Core Selection

Powdered-iron toroid cores are used throughout this system. They provide:

- Higher Q than ferrite at HF frequencies (reduced coil loss)
- Stable permeability vs. temperature and RF drive level
- Compact size relative to required inductance

| Core | Mix | AL (nH/T²) | Freq Range | OD (mm) | Application in This System |
|------|-----|-----------|-----------|---------|--------------------------|
| T200-2 | Mix 2 | 120 | 2–30 MHz | 50.8 | 160 m loading coil (large inductance, high Q) |
| T130-2 | Mix 2 | 110 | 2–30 MHz | 33.0 | 80 m and 60 m loading coils |
| T106-2 | Mix 2 | 135 | 2–30 MHz | 26.9 | 40 m loading coil |
| T50-2 | Mix 2 | 49 | 10–50 MHz | 12.7 | 30 m loading coil (small, few turns) |
| T50-6 | Mix 6 | 40 | 30–200 MHz | 12.7 | LC trap coils (roll-up version) |
| FT-240-31 | Mix 31 | — | 1–300 MHz | 61.0 | Common-mode choke balun (8T RG-174) |
| FT-37-43 | Mix 43 | — | 1–300 MHz | 9.5 | Bruene directional coupler (1T/10T) |

> **NOTE**
> Mix 2 powdered-iron cores (RED/CLEAR color code) maintain Q of 200 to 350 across the HF spectrum. Mix 43 ferrite is appropriate for wide-band transformers but is unsuitable for loading coils due to lower Q.

## 3.3 Coil Construction Details

**Full Winding Table:**

| Band | Core | N (turns) | Wire AWG | L actual (µH) | Xant cancelled (Ω) | Q est | Rcoil (Ω) | Winding Notes |
|------|------|----------|---------|--------------|-------------------|-------|----------|--------------|
| 160 m | T200-2 (AL=120) | 31 | #22 AWG | 115.4 | 1730 | 250 | 6.92 | Close-wound single layer; 31T fits core circumference |
| 80 m | T130-2 (AL=110) | 18 | #20 AWG | 35.6 | 833 | 280 | 2.98 | Single layer, 25–30 mm winding length |
| 60 m | T130-2 (AL=110) | 12 | #20 AWG | 15.8 | 543 | 285 | 1.91 | Single layer; 16–18 mm winding length |
| 40 m | T106-2 (AL=135) | 8 | #18 AWG | 8.64 | 352 | 310 | 1.14 | Single layer; 15–20 mm winding length |
| 30 m | T50-2 (AL=49) | 7 | #18 AWG | 2.40 | 158 | 320 | 0.494 | 5–6 turns visible; compact; low-loss |

> **NOTE**
> Toroid AL values are nominal. Measure inductance with an LC meter or NanoVNA after winding. Trim by adding or removing one turn as needed. A 10 percent error in inductance changes resonant frequency by approximately 5 percent, which is within the L-network trimming range.

**Wire Preparation:**

1. Cut wire to length with 50 mm spare at each end.
2. Wind evenly spaced turns in a single layer covering approximately 60 to 70 percent of the core circumference.
3. For enameled magnet wire, scrape insulation from both ends for 15 mm using fine-grit sandpaper or a sharp blade.
4. Pre-tin both scraped ends immediately after removing insulation.
5. Do not use excessive solder heat on the tinned ends; this may re-wick enamel inward and cause poor connections.
6. Secure winding ends with a spot of cyanoacrylate adhesive to prevent unraveling.

**Construction Procedure:**

1. Select core per winding table above.
2. Thread enameled wire through core, counting turns carefully. One pass through the core hole counts as one turn.
3. Wind in a single layer. Do not allow turns to overlap.
4. Keep winding length to the minimum required; a longer winding reduces distributed capacitance and improves Q.
5. After winding is complete, measure inductance on an LC meter at 1 kHz. Compare to required value.
6. If inductance is low by more than 10 percent, add one turn and measure again.
7. If inductance is high by more than 10 percent, remove one turn and measure again.
8. Verify coil resonates with the antenna at the design frequency using the SWR bridge.

## 3.4 Impedance Matching — L-Network Step-Up

After insertion of the loading coil, the antenna feedpoint presents a low resistance equal to Rrad + Rcoil. This low impedance must be transformed to 50 ohms for the coaxial feedline. A low-pass L-network provides this transformation.

**L-Network Design Equations:**

```
Q_LN = sqrt(Z_out / Z_in - 1)    where Z_out = 50 Ω, Z_in = Zfeed = Rrad + Rcoil

Xs = Q_LN × Z_in                  (series arm reactance, ohms)
Xp = Z_out / Q_LN                 (shunt arm reactance, ohms)

For a low-pass topology (series L toward antenna, shunt C to ground):
  L_series = Xs / (2πf)            (series inductance, henries)
  C_shunt  = 1 / (2πf × Xp)       (shunt capacitance, farads)
```

**L-Network Values by Band:**

| Band | Zfeed (Ω) | Q_LN | Xs (Ω) | Xp (Ω) | L_series (µH) | C_shunt (pF) |
|------|----------|------|--------|--------|-------------|-------------|
| 160 m | 7.04 | 2.47 | 17.4 | 20.2 | 1.46 | 4,137 |
| 80 m | 3.46 | 3.67 | 12.7 | 13.6 | 0.54 | 3,114 |
| 60 m | 2.88 | 4.04 | 11.7 | 12.4 | 0.35 | 2,414 |
| 40 m | 2.90 | 4.03 | 11.7 | 12.4 | 0.26 | 1,796 |
| 30 m | 4.02 | 3.38 | 13.6 | 14.8 | 0.21 | 1,063 |
| 20 m | ~36 | 0.655 | 22.9 | 76.4 | 0.26 | 147 |

> **NOTE**
> The series inductance (L_series) on the lower bands is small compared to the loading coil inductance. In practice, the loading coil provides both the resonating function and the series arm of the L-network. The operator adjusts only the shunt capacitor (C_shunt) for minimum SWR. A single variable capacitor covering 20 to 5,000 pF (air-variable or switched fixed capacitors) covers all five loading-coil bands.

> **CAUTION**
> The C_shunt values on 160 meters (4,137 pF) and 80 meters (3,114 pF) are large. Verify that the variable capacitor or switched bank can achieve these values without significant series resistance. A low-Q capacitor here degrades system efficiency.

## 3.5 Tapped Coil Alternative (Autotransformer)

The loading coil may be tapped to provide impedance matching without a separate L-network. The coil functions as an autotransformer:

```
N_tap / N_total = sqrt(Z_coax / Z_ant)

where Z_ant = Rrad + Rcoil at the top of the coil
```

**Tap Position Analysis:**

| Band | N_total | Z_ant (Ω) | N_tap (ideal) | Tap Fraction | Practical? |
|------|---------|----------|-------------|-------------|-----------|
| 160 m | 31 | 7.04 | 83 | 2.7× (> coil) | No — use L-network |
| 80 m | 18 | 3.46 | 53 | 2.9× (> coil) | No — use L-network |
| 60 m | 12 | 2.88 | 41 | 3.4× (> coil) | No — use L-network |
| 40 m | 8 | 2.90 | 14 | 1.7× (> coil) | No — use L-network |
| 30 m | 7 | 4.02 | 9 | 1.3× (> coil) | Marginal |

The analysis shows that for all bands in this system, the required autotransformer tap exceeds the total number of coil turns. This is a consequence of the very low radiation resistance. The L-network approach using a separate shunt capacitor is therefore the recommended matching method for all bands.

> **NOTE**
> The tapped coil approach is practical for mobile loading coils where the coil has many more turns (e.g., 60 to 150 turns on a large-diameter form). In this system's compact plug-in module design, the L-network is more efficient and flexible.

---

# CHAPTER 4: TELESCOPING WHIP EXTENSION GUIDE

## 4.1 Section Configuration

The 7-section telescoping whip has the following physical dimensions:

| Section | Designation | Nominal Length (m) | Outer Diameter (mm) | Cumulative Length (m) |
|---------|------------|-------------------|--------------------|-----------------------|
| 1 | Base section | 0.800 | 20 | 0.800 |
| 2 | Second section | 0.800 | 17 | 1.600 |
| 3 | Third section | 0.800 | 14 | 2.400 |
| 4 | Fourth section | 0.800 | 12 | 3.200 |
| 5 | Fifth section | 0.800 | 10 | 4.000 |
| 6 | Sixth section | 0.800 | 7 | 4.800 |
| 7 | Tip section | 0.800 | 4 | 5.600 |

**Collapsed length:** approximately 360 mm (all sections retracted into Section 1).

All measurements of extension position are taken from the mounting/feed point at the base of Section 1.

> **WARNING**
> Do not operate the antenna near overhead power lines. The fully extended 5.6-meter whip presents a lethal hazard if it contacts or arcs to distribution voltage conductors.

## 4.2 Extension Per Band

**Complete Band Extension Table:**

| Band | f (MHz) | λ/4 QW (m) | Extension (m) | Secs Extended | Loading Coil | Counterpoise | Trim Method |
|------|---------|-----------|--------------|--------------|-------------|-------------|------------|
| 160 m | 1.900 | 38.29 | 5.60 | 7 of 7 | Module 1 (144.6 µH) | 4 × 5.6 m radials | L-net C_shunt to min SWR |
| 80 m | 3.750 | 19.40 | 5.60 | 7 of 7 | Module 2 (35.5 µH) | 4 × 5.6 m radials | L-net C_shunt to min SWR |
| 60 m | 5.330 | 13.65 | 5.60 | 7 of 7 | Module 3 (16.3 µH) | 4 × 5.6 m radials | L-net C_shunt to min SWR |
| 40 m | 7.150 | 10.18 | 5.60 | 7 of 7 | Module 4 (7.85 µH) | 4 × 4.0 m radials | L-net C_shunt to min SWR |
| 30 m | 10.125 | 7.19 | 5.60 | 7 of 7 | Module 5 (2.477 µH) | 4 × 2.8 m radials | L-net C_shunt to min SWR |
| 20 m | 14.175 | 5.13 | 5.13 or 5.60 | 7 of 7 | None — 300 pF NP0 cap | 2–4 × 2.5 m radials | Retract Sec7 or trim cap |
| 17 m | 18.118 | 4.01 | 4.01 | 6 of 7 | None | 2–4 × 2.0 m radials | Trim Sec 5 tip ±20 mm |
| 15 m | 21.225 | 3.43 | 3.43 | 5 of 7 | None | 2–4 × 1.7 m radials | Trim Sec 5 to mark |
| 12 m | 24.940 | 2.92 | 2.92 | 4 of 7 | None | 2–4 × 1.5 m radials | Trim Sec 4 to mark |
| 10 m | 28.500 | 2.55 | 2.55 | 4 of 7 | None | 2–4 × 1.3 m radials | Trim Sec 4 to mark |
| 6 m | 51.000 | 1.40 | 1.40 | 2 of 7 | None | 3 × λ/4 rigid radials | Trim Sec 2 to mark |
| 2 m | 146.000 | 0.488 | 0.488 | 1 of 7 | None | 4 × 120 mm radials | Sec 1 to 2m mark |
| 1.25 m | 222.000 | 0.321 | 0.321 | 1 of 7 | None | 4 × 80 mm radials | Mark on Sec 1 |
| 70 cm | 446.000 | 0.160 | 0.160 | 1 of 7 | None | 4 × 40 mm radials | Stub to mark |
| 33 cm | 906.000 | 0.079 | 0.079 | Tip only | None | Coaxial fixture | SMA stub preferred |

## 4.3 Marking the Whip

Paint ring marks are applied to the sections to identify extension positions for each band. All positions are measured from the mounting/feed point at the base of Section 1.

**Mark Position Table:**

| Section | Position from Base (m) | Band | Color | Label |
|---------|----------------------|------|-------|-------|
| Sec 1 | 0.160 | 70 cm | White ring | "70" |
| Sec 1 | 0.321 | 1.25 m | Yellow ring | "125" |
| Sec 1 | 0.488 | 2 m | Green ring | "2M" |
| Sec 1 | 0.800 | Section tip reference | Silver ring | "S1" |
| Sec 2 | 1.400 | 6 m | Orange ring | "6M" |
| Sec 2 | 1.600 | Section tip reference | Silver ring | "S2" |
| Sec 3 | 2.400 | Section tip reference | Silver ring | "S3" |
| Sec 4 | 2.550 | 10 m | Red ring | "10" |
| Sec 4 | 2.920 | 12 m | Red-orange ring | "12" |
| Sec 4 | 3.200 | Section tip reference | Silver ring | "S4" |
| Sec 5 | 3.430 | 15 m | Blue ring | "15" |
| Sec 5 | 4.000 | 17 m | Blue ring | "17" (= Sec 5 tip) |
| Sec 6 | 4.800 | Section tip reference | Silver ring | "S6" |
| Sec 7 | 5.050 | 20 m trim | Dashed white ring | "20" |
| Sec 7 | 5.600 | Full extension reference | Silver ring | "MAX" |

**Mark Application Procedure:**

1. Clean the section surface thoroughly with isopropyl alcohol. Allow to dry.
2. Apply a strip of painter's tape at the measured position to serve as a guide edge.
3. Apply two coats of enamel paint or nail varnish in the appropriate color. Allow each coat to dry for 30 minutes.
4. Remove the painter's tape while the final coat is still slightly tacky (cleaner edge).
5. Allow marks to cure for 24 hours.
6. Apply one coat of clear nail varnish over all marks as a protective topcoat.
7. Add alphanumeric labels using a fine-point paint pen or engraving tool.

Mark position tolerance shall be within ±5 mm of the specified measurement. This corresponds to a frequency error of less than 0.5 percent on all bands within the whip's range.

> **CAUTION**
> Do not engrave marks on Sections 5, 6, or 7. The thin walls of the upper sections are susceptible to stress cracking when engraved. Use paint marks only on the upper sections.

## 4.4 Counterpoise Deployment

**HF bands (160 m through 30 m):** Deploy four radial wires, each equal in length to the active whip extension (5.60 meters), spread at 90-degree intervals around the base unit. Lay wires on the ground surface or peg slightly elevated. The counterpoise connects to the radial bus on the base unit rear panel.

**20-meter and HF above 20 m (17 m through 10 m):** Two to four radials, each equal to the whip extension length for that band, spread at 90-degree intervals. Elevated radials (0.5 to 1.0 meter above ground) outperform ground-laid radials and require fewer wires.

**VHF (6 m and 2 m):** Four rigid radials at 45-degree downward angle from the base coaxial connector. Radial length equals the quarter-wave at the operating frequency: 350 mm for 6 meters, 120 mm for 2 meters. These may be soldered or screwed directly to the SO-239 flange.

> **NOTE**
> On 160 meters, a minimum of eight elevated radials is recommended to achieve useful efficiency. A 4-radial system on 160 meters will typically have 15 to 20 ohms of ground resistance, reducing effective radiated power by 6 to 10 dB compared to a well-implemented ground system.

## 4.5 Base Unit Connection Procedure

1. Select the loading coil module for the desired band (or no module for 20 m and above).
2. If installing a loading coil: align the module pins with the NMO socket holes; insert and twist clockwise to lock. Verify seating by gentle upward pull.
3. Connect radial/counterpoise wires to the banana jacks on the base unit rear panel.
4. Connect the coaxial feedline from the transceiver to the SO-239 input connector.
5. Mount the whip in the NMO socket of the loading coil module (for loaded bands) or directly in the base unit NMO socket (for no-coil bands).
6. Extend the whip to the appropriate length per the extension table.
7. Lock all sections at the required position.
8. Apply a brief test transmission (1 to 5 watts, 5 seconds) and observe the SWR on the CYD display or web interface.
9. Adjust the L-network shunt capacitor for minimum SWR if using a loading coil band.

---

# CHAPTER 5: DIRECTIONAL COUPLER THEORY AND SWR MEASUREMENT

## 5.1 Traveling Wave Decomposition on Transmission Lines

Any signal on a transmission line can be decomposed into a forward-traveling wave (from source to load) and a reflected wave (from load toward source). The amplitude and phase of the reflected wave depend on the load impedance.

If the load impedance matches the characteristic impedance of the line (50 ohms in this system), no reflection occurs. If the load impedance differs from 50 ohms, a fraction of the forward power is reflected.

The reflection coefficient rho (ρ) describes the fraction of the forward voltage amplitude that is reflected:

```
ρ = (ZL - Z0) / (ZL + Z0)

where:
  ZL = load impedance (complex, ohms)
  Z0 = characteristic impedance (50 ohms)
  ρ  = reflection coefficient (complex, magnitude ≤ 1.0)
```

The magnitude |ρ| ranges from 0 (perfect match) to 1.0 (total reflection, open or short circuit).

## 5.2 Bruene Coupler Design

The Bruene directional coupler used in this system is assembled on the SWR bridge board. It consists of:

**Core:** FT-37-43 (Mix 43, outer diameter 9.5 mm, µi = 850)
**Primary:** 1 turn — the coaxial center conductor passes once through the core hole
**Secondary:** 10 turns of #28 AWG enameled wire, close-wound in a single layer
**Coupling factor:** 20 dB (1:10 turns ratio; power ratio 1:100)

The secondary winding samples the current on the transmission line. A 51-ohm resistor across the secondary samples the line voltage in quadrature. The vector sum at the two detector ports gives forward and reflected signals with 30 to 35 dB directivity.

**Component List:**

| Designator | Value/Type | Function |
|-----------|-----------|---------|
| T1 | FT-37-43, 1T/10T | Directional coupler core |
| R1 | 51 Ω, 1/4 W metal film | Forward detector load |
| R2 | 51 Ω, 1/4 W metal film | Quadrature voltage termination |
| R3 | 51 Ω, 1/4 W metal film | Reflected detector load |
| D1 | 1N5711 Schottky | Forward envelope detector |
| D2 | 1N5711 Schottky (reversed) | Reflected envelope detector |
| C1 | 100 nF ceramic | Forward detector filter |
| C2 | 100 nF ceramic | Reflected detector filter |

> **NOTE**
> The simple diode detector version (D1, D2) is temperature sensitive and nonlinear at low power levels. The AD8307 version described in section 5.3 is preferred for accurate measurement across the full power range.

## 5.3 AD8307 Logarithmic Detector

The AD8307 is a monolithic logarithmic amplifier with a dynamic range of 92 dB (approximately −73 dBm to +17 dBm) and bandwidth to 500 MHz. It operates from 3.3 volts and requires no external calibration under normal conditions.

**Transfer function:**

```
V_out = Slope × (P_in_dBm − Intercept)
      = 25 mV/dB × (P_in_dBm − (−84 dBm))
      = 25 mV/dB × (P_in_dBm + 84)

Solving for power:
  P_dBm = (V_out_mV / 25) − 84
```

**Conversion from ADC counts to dBm:**

```
ADC reference: 3300 mV, 12-bit (0 to 4095 counts)
1 ADC count = 3300 / 4095 = 0.8058 mV

V_mV = ADC_count × 0.8058

P_dBm = (V_mV / 25) − 84 + K_coupler + K_cal

where:
  K_coupler = 20.0 dB (coupling factor, positive)
  K_cal     = individual calibration trim (saved in NVS)
```

**Coupling factor correction:** The Bruene coupler couples 20 dB less power to the secondary than flows in the through-line. The AD8307 measures the coupled power. Adding 20 dB to the AD8307 reading gives the true line power.

**Dynamic range in this application:** The system measures from −73 + 20 = −53 dBm to +17 + 20 = +37 dBm line power (approximately 0.5 µW to 5 W). For higher power, the operator does not exceed the coupler's continuous rating (approximately 50 watts at HF).

## 5.4 SWR Calculation

```
Step 1 — Reflection coefficient magnitude:
  ρ = 10^((P_refl_dBm − P_fwd_dBm) / 20)
  (equivalently: ρ = sqrt(P_refl_W / P_fwd_W))

Step 2 — SWR:
  SWR = (1 + ρ) / (1 − ρ)

Step 3 — Return loss:
  RL_dB = P_fwd_dBm − P_refl_dBm    (positive value; higher is better)
        = −20 × log10(ρ)

Step 4 — Mismatch loss (power lost to reflection):
  ML_dB = −10 × log10(1 − ρ²)
```

## 5.5 Efficiency Loss from SWR

Mismatch loss is the fraction of forward power that is reflected and does not reach the antenna. This is separate from and in addition to coil and ground losses.

```
ML_dB = −10 × log10(1 − ρ²)

Examples:
  SWR 1.5:1 → ρ = 0.200 → ML = −10 × log10(1 − 0.040) = 0.18 dB (4% power lost)
  SWR 2.0:1 → ρ = 0.333 → ML = −10 × log10(1 − 0.111) = 0.51 dB (11% power lost)
  SWR 3.0:1 → ρ = 0.500 → ML = −10 × log10(1 − 0.250) = 1.25 dB (25% power lost)
```

## 5.6 SWR Quick Reference Table

| SWR | ρ | Return Loss (dB) | P_refl/P_fwd | Mismatch Loss (dB) | Practical Assessment |
|-----|---|-----------------|--------------|--------------------|---------------------|
| 1.0:1 | 0.000 | ∞ | 0.000 | 0.00 | Perfect match |
| 1.1:1 | 0.048 | 26.4 | 0.0023 | 0.01 | Negligible loss |
| 1.2:1 | 0.091 | 20.8 | 0.0083 | 0.04 | Excellent |
| 1.5:1 | 0.200 | 13.9 | 0.0400 | 0.18 | Tuner target |
| 1.8:1 | 0.286 | 10.9 | 0.0816 | 0.37 | Acceptable |
| 2.0:1 | 0.333 | 9.5 | 0.1111 | 0.51 | Acceptable |
| 2.5:1 | 0.429 | 7.3 | 0.1837 | 0.88 | Marginal |
| 3.0:1 | 0.500 | 6.0 | 0.2500 | 1.25 | Needs adjustment |
| 4.0:1 | 0.600 | 4.4 | 0.3600 | 1.94 | Poor |
| 5.0:1 | 0.667 | 3.5 | 0.4444 | 2.55 | Very poor |
| 7.0:1 | 0.750 | 2.5 | 0.5625 | 3.58 | Unacceptable |
| 10.0:1 | 0.818 | 1.7 | 0.6694 | 4.81 | Unacceptable |
| ∞:1 | 1.000 | 0.0 | 1.000 | ∞ | Open or short circuit |

## 5.7 Coupler Directivity Reference Table

Directivity is the isolation between forward and reflected ports when the through-line is terminated in a perfect 50-ohm load. Higher directivity gives more accurate SWR readings near 1.0:1.

| Coupler Type | Directivity (dB) | SWR Accuracy at 1.5:1 | Notes |
|-------------|-----------------|----------------------|-------|
| Simple resistive bridge | 20–25 | ±0.10 | Low cost; temperature-sensitive diodes |
| Bruene + FT-37-43, diode det. | 30–35 | ±0.03 | Good HF performance 1.8–30 MHz |
| Bruene + FT-37-43, AD8307 det. | 35–40 | ±0.01 | Best dynamic range; this system's implementation |
| Tandem match, dual-core | 40–45 | ±0.003 | Requires precision component matching |
| λ/4 coupled-line PCB | 20–30 | ±0.05 | VHF/UHF preferred; impractical at HF |

> **NOTE**
> The AD8307 eliminates detector diode nonlinearity as an error source and extends the dynamic range compared to the diode detector version. The 35 to 40 dB directivity of the AD8307-equipped Bruene coupler is sufficient for SWR accuracy to within ±0.01 at the 1.5:1 target.

---

# CHAPTER 6: LC TRAPS — ROLL-UP WIRE VERSION

## 6.1 Roll-Up Wire Design Concept

The roll-up wire version substitutes a 5.6-meter length of flexible stranded wire for the rigid telescoping whip. Six LC trap assemblies are soldered inline at specific positions along the wire. Each trap is a parallel resonant LC circuit that presents very high impedance (effectively open circuit) at its design frequency.

At frequencies above a trap's resonant frequency, the section of wire beyond the trap is electrically disconnected. This shortens the effective antenna length. Traps are placed at positions corresponding to the quarter-wave length for bands 17 m, 15 m, 12 m, 10 m, 6 m, and 2 m.

**Trap positions from the base (feed end) of the wire:**

| Trap | Target Band | Position (m) | QW for band (m) |
|------|------------|-------------|----------------|
| 1 (innermost) | 2 m (146 MHz) | 0.488 | 0.488 |
| 2 | 6 m (51 MHz) | 1.40 | 1.40 |
| 3 | 10 m (28.5 MHz) | 2.55 | 2.55 |
| 4 | 12 m (24.94 MHz) | 2.92 | 2.92 |
| 5 | 15 m (21.225 MHz) | 3.43 | 3.43 |
| 6 (outermost) | 17 m (18.118 MHz) | 4.01 | 4.01 |

For 160 m through 30 m, the loading coil modules plug into the base unit exactly as with the telescoping whip version. The 5.6-meter wire acts as the radiator. For 20 m, all traps appear inductive below their resonance frequencies; the minor residual reactance is cancelled by a small series capacitor at the base.

## 6.2 Trap Theory

A parallel LC circuit resonant at frequency f₀ presents maximum impedance at that frequency:

```
Z_trap = Q × sqrt(L/C)   (at resonance)

where:
  Q = unloaded Q of the trap coil
  L = trap inductance (H)
  C = trap capacitance (F)
```

Below the trap's resonant frequency, the trap appears inductive (net inductive reactance). This adds a small amount of loading to the antenna but does not disconnect it. Above the trap's resonant frequency, the trap appears capacitive, which also does not electrically disconnect the wire beyond it.

At frequencies near the trap resonance, the trap impedance is maximum and the wire beyond the trap is effectively isolated. The isolation bandwidth (frequencies within which Z_trap > 5,000 ohms) is approximately:

```
BW_isolation ≈ f₀ / Q
```

For Q = 250 and f₀ = 18.118 MHz: BW ≈ 72 kHz. This is adequate for the narrow 17-meter band (100 kHz wide).

## 6.3 Trap Construction Table

All traps for the roll-up wire version:

| Trap | Band | f₀ (MHz) | Core | AL (nH/T²) | N | L (µH) | C (pF) | C Type | f₀_calc (MHz) | Z_trap (Ω) | Q | IL (dB) | Position (m) | Housing OD (mm) |
|------|------|---------|------|-----------|---|--------|--------|--------|-------------|-----------|---|---------|-------------|----------------|
| T1 | 2 m | 146.000 | Air-core 4 mm dia. | — | 2 | 0.018 | 68 | NP0 SMD 0805, 50 V | 145.2 | 5,000 | 350 | 0.06 | 0.488 | 10 |
| T2 | 6 m | 51.000 | Air-core 5 mm dia. | — | 3 | 0.060 | 160 | NP0, 100 V | 51.4 | 8,000 | 300 | 0.04 | 1.40 | 12 |
| T3 | 10 m | 28.500 | T50-6 | 40 | 3 | 0.360 | 86.7 | NP0 (82+4.7 pF), 500 V | 28.49 | 16,100 | 250 | 0.02 | 2.55 | 19 |
| T4 | 12 m | 24.940 | T50-6 | 40 | 3 | 0.360 | 113.3 | NP0 (110+3.3 pF), 500 V | 24.91 | 14,095 | 250 | 0.02 | 2.92 | 19 |
| T5 | 15 m | 21.225 | T50-6 | 40 | 4 | 0.640 | 88.8 | NP0 (82+6.8 pF), 500 V | 21.14 | 17,000 | 250 | 0.03 | 3.43 | 19 |
| T6 | 17 m | 18.118 | T50-6 | 40 | 3 | 0.360 | 220 | NP0, 500 V | 17.88 | 10,113 | 250 | 0.02 | 4.01 | 19 |

**Inductance calculations for T50-6 traps:**

```
T50-6: AL = 40 nH/T²

T3, T4, T6 (3 turns): L = 40 × 9 / 1000 = 0.360 µH
T5 (4 turns): L = 40 × 16 / 1000 = 0.640 µH
```

**Resonant frequency verification:**

```
f₀ = 1 / (2π × sqrt(L × C))

T3: f₀ = 1 / (2π × sqrt(0.360e-6 × 86.7e-12)) = 28.49 MHz ✓
T5: f₀ = 1 / (2π × sqrt(0.640e-6 × 88.8e-12)) = 21.14 MHz ✓
T6: f₀ = 1 / (2π × sqrt(0.360e-6 × 220e-12)) = 17.88 MHz ✓
```

## 6.4 Trap Installation Procedure

1. Cut the antenna wire to exactly 5.60 meters plus 100 mm spare at each end.
2. Mark trap positions from the feed (base) end at the distances specified in the table.
3. At each mark, cut the wire, leaving 30 mm of bare wire on each cut end.
4. Tin both wire ends with rosin-core solder.
5. Solder the trap assembly in series: one wire end to one coil terminal, the other wire end to the remaining coil terminal. The capacitor shunts the coil terminals.
6. Verify the trap resonant frequency with a NanoVNA in the transmission mode: connect port 1 to one wire end and port 2 to the other wire end; the transmission minimum occurs at the trap resonant frequency.
7. House the trap assembly in heat-shrink tubing (for traps T3 through T6) or a small ABS enclosure (for T1 and T2, which are fragile SMD assemblies).
8. Allow sufficient mechanical flexibility at each trap location to prevent wire fatigue.

> **CAUTION**
> Do not apply excessive heat when soldering the SMD capacitors in traps T1 and T2. Use a 20-watt iron with a fine tip; limit solder contact time to 3 seconds. NP0 SMD capacitors are susceptible to thermal cracking if overheated.

## 6.5 Verification with NanoVNA

After complete assembly of the roll-up wire, verify each trap using the NanoVNA:

**Procedure:**

1. Connect one NanoVNA port (CH0) to the antenna feed end.
2. Use a short 50-ohm jumper to terminate the antenna tip.
3. Perform an S11 sweep from 1 MHz to 200 MHz.
4. At each trap's design frequency, a sharp impedance anomaly will be visible as the trap presents high impedance in series with the antenna.
5. Alternatively, verify each individual trap before installation using the NanoVNA in two-port (S21) mode as described in step 6 of the installation procedure.

Acceptable trap performance: resonant frequency within ±2 percent of design frequency; trap impedance (Z_trap) not less than 3,000 ohms at f₀.

---

# CHAPTER 7: NEC MODELING AND SIMULATION

## 7.1 NEC-2 Card Summary

NEC-2 input files consist of a sequence of cards (records). Each card begins with a 2-character mnemonic.

| Card | Mnemonic | Function | Key Parameters |
|------|---------|---------|----------------|
| Comment | CM | Human-readable comment | Free text |
| Comment End | CE | End of comment block | None |
| Geometry Wire | GW | Define a wire segment | Tag, N_segs, X1 Y1 Z1 X2 Y2 Z2 Radius |
| Geometry End | GE | End geometry block | I1 (1 = enable near-ground effects) |
| Ground | GN | Define ground parameters | Type, N_rad, ..., Dielectric constant, Conductivity |
| Loading | LD | Add impedance load to segment | Type, Tag, Seg1, Seg2, R, X (or L, C) |
| Excitation | EX | Voltage/current source | Type, Tag, Seg, ..., Vmag, Vphase |
| Frequency | FR | Set frequency sweep | Linear/log, N_steps, ..., f_start, f_step |
| Radiation Pattern | RP | Request radiation pattern | Mode, N_theta, N_phi, options, theta0, phi0, dtheta, dphi |
| End | EN | End of input file | None |

## 7.2 LD Card for Loading Coil Modeling

The loading coil is modeled as a series R + jX lumped impedance placed at the base of the antenna wire (segment 1 of GW 1).

**LD card format (Type 0, series R+jX):**

```
LD  LDTYP  LDTAG  LDTAGF  LDTAGT  ZLR  ZLI
    0      1      1       1       R    X

where:
  LDTYP = 0  (series R+jX, no frequency dependence)
  LDTAG = 1  (tag number of wire to load)
  LDTAGF = 1 (first segment of loading)
  LDTAGT = 1 (last segment of loading)
  ZLR    = series resistance (ohms) = XL/Q
  ZLI    = series reactance (ohms) = 2πfL (positive = inductive)
```

**Example — 40-meter loading coil (L = 7.85 µH, Q = 310, f = 7.15 MHz):**

```
XL = 2π × 7.15e6 × 7.85e-6 = 352.5 Ω
R  = 352.5 / 310 = 1.137 Ω

LD  0  1  1  1  1.14  352.5
```

> **NOTE**
> The LD Type 0 model does not account for the frequency dependence of core permeability or winding capacitance. For accurate results across a wide frequency sweep, use LD Type 4 (series R, L, C) or separate frequency-specific models. The NEC files in this system use single-frequency models for each band to maintain accuracy.

## 7.3 Ground Card

```
GN  IPERF  NRADL  ...  EPSR  SIG

For real average soil (used in all system NEC files):
GN  2  0  0  0  13  0.005

where:
  IPERF = 2  (Sommerfeld-Norton real ground)
  NRADL = 0  (no buried radials — ground plane is modeled as GW elements)
  EPSR  = 13 (relative dielectric constant, average soil)
  SIG   = 0.005 (conductivity, S/m, average soil)
```

For sensitivity checks:

| Soil Type | εr | σ (S/m) | GN Card |
|-----------|-----|---------|---------|
| Perfect ground | — | — | GN 1 |
| Very good (salt marsh) | 80 | 5.000 | GN 2 0 0 0 80 5.000 |
| Good (clay) | 20 | 0.010 | GN 2 0 0 0 20 0.010 |
| Average (this system) | 13 | 0.005 | GN 2 0 0 0 13 0.005 |
| Poor (sandy) | 10 | 0.002 | GN 2 0 0 0 10 0.002 |
| Very poor (rocky) | 5 | 0.001 | GN 2 0 0 0 5 0.001 |

## 7.4 NEC Model Files

All NEC files reside in the `nec_models/` directory of this project.

| File | TM Number | Band | Description |
|------|----------|------|-------------|
| whip_base_loaded_160m.nec | TM-WHP-NEC-001 | 160 m | Full extension, L=144.6 µH, 4 radials |
| whip_base_loaded_80m.nec | TM-WHP-NEC-002 | 80 m | Full extension, L=35.5 µH, 4 radials |
| whip_base_loaded_40m.nec | TM-WHP-NEC-003 | 40 m | Full extension, L=7.85 µH, 4 radials |
| whip_base_loaded_30m.nec | TM-WHP-NEC-004 | 30 m | Full extension, L=2.477 µH, 4 radials |
| whip_resonant_20m.nec | TM-WHP-NEC-005 | 20 m | Trimmed to 5.13 m, near-QW, no coil |
| whip_retracted_17m.nec | TM-WHP-NEC-006 | 17 m | Retracted to 4.01 m, QW resonant |
| whip_retracted_15m_12m_10m.nec | TM-WHP-NEC-007 | 15/12/10 m | Multi-band retracted QW models |
| whip_retracted_6m.nec | TM-WHP-NEC-008 | 6 m | Retracted to 1.40 m |
| whip_retracted_2m.nec | TM-WHP-NEC-009 | 2 m | Retracted to 0.488 m |
| whip_retracted_vhf_uhf.nec | TM-WHP-NEC-010 | 1.25m/70cm | Short stub models |
| whip_trapped_rollup_hf.nec | TM-WHP-NEC-011 | Roll-up | Wire with inline traps, 10 m active |
| whip_complete_multiband.nec | TM-WHP-NEC-012 | All | Complete system reference, 40 m active |

## 7.5 Running the Models

**Command line (nec2c):**

```bash
nec2c -i whip_base_loaded_40m.nec -o whip_base_loaded_40m.out
```

**Graphical (xnec2c on Linux, 4nec2 on Windows):**

```bash
xnec2c whip_base_loaded_40m.nec
```

**Batch sweep all models:**

```bash
for f in nec_models/*.nec; do
    out="${f%.nec}.out"
    nec2c -i "$f" -o "$out"
    echo "Processed: $f"
done
```

## 7.6 Interpreting Results

**Feedpoint Impedance:** Reported in the NEC output as "ANTENNA INPUT PARAMETERS." The format is:

```
IMPEDANCE = Rreal + j Rimag  (ohms)
```

Compare the real part to Rrad + Rcoil from the loading coil table. The imaginary part should be near zero at the design frequency.

**SWR to 50 Ω:** Computed from the feedpoint impedance:

```
ρ = |Z - 50| / |Z + 50|
SWR = (1 + ρ) / (1 - ρ)
```

**Gain (dBi):** Reported in the far-field pattern output. Expect approximately −2 to −4 dBi on the heavily loaded bands (160 m, 80 m) and 0 to −1 dBi on the lightly loaded or near-resonant bands (30 m, 20 m).

**Radiation Pattern:** The RP 0 card with option 1000 outputs gain as a function of elevation and azimuth angle. The antenna is omnidirectional in azimuth. Key parameter is the takeoff angle (angle of maximum radiation above the horizon) which ranges from 25 to 45 degrees depending on ground quality.

**Expected 40-Meter Results (TM-WHP-NEC-012):**

- Feedpoint Z without coil: approximately −j352 Ω (capacitive short vertical)
- With L = 7.85 µH coil at resonance: approximately 36 + j0 Ω
- Gain: approximately −2 dBi over average soil with 4 radials
- Takeoff angle: 25 to 30 degrees
- 2:1 SWR bandwidth: approximately 50 to 80 kHz with good coil and 4 radials

---

# CHAPTER 8: ELECTRONIC INSTRUMENTATION

## 8.1 ESP32 SWR Monitor (esp32_whip_tuner.ino)

**Core Functions:**

| Function | Description |
|---------|-------------|
| `readADC_mV(pin)` | 64× oversampled ADC read; returns millivolts |
| `ad8307_to_dbm(v_mv, coupler_db, cal_offset)` | Converts AD8307 output to dBm; applies coupling factor and cal trim |
| `measureSWR()` | Averages 8 readings; computes ρ, SWR, return loss, forward power in watts |
| `updateLEDs(m)` | Drives green/yellow/red LED indicators based on SWR thresholds |
| `sendBTPacket()` | Sends comma-delimited telemetry string to CYD via Bluetooth Serial |
| `checkButtons()` | Debounced band up/down; saves band selection to NVS |
| `tuning_hint(b)` | Returns HTML setup instructions appropriate to current band |
| `handleRoot()` | Serves full web UI with auto-refresh at 3-second intervals |
| `handleApiStatus()` | REST GET: returns JSON telemetry packet |
| `handleApiCal()` | REST GET/POST: returns or sets calibration offsets |

**REST API Endpoints:**

| Endpoint | Method | Description | Response |
|---------|--------|-------------|---------|
| `/` | GET | Full HTML web UI | HTML page with auto-refresh |
| `/band_up` | GET | Increment band index, redirect to `/` | 302 redirect |
| `/band_dn` | GET | Decrement band index, redirect to `/` | 302 redirect |
| `/api/status` | GET | JSON telemetry | `{"band":"40m","freq":7.150,"qw":10.18,"extension":5.600,"coil_type":1,"coil_L":7.850,"swr":1.25,"fwd_w":5.0000,"fwd_dbm":37.0,"refl_dbm":18.1,"rl_db":18.9,"valid":true}` |
| `/api/cal` | GET | JSON calibration offsets | `{"cal_fwd":0.000,"cal_refl":0.000}` |
| `/api/cal` | POST | Set calibration offsets | `cal_fwd=X.X&cal_refl=X.X` POST body |

**Bluetooth Telemetry Packet Format:**

```
BAND:%s,EXT:%.3f,COIL:%.1f,SWR:%.2f,FWD:%.3fW,REFL:%.1f,RL:%.1f,VALID:%d\n

Example:
BAND:40m,EXT:5.600,COIL:7.9,SWR:1.24,FWD:5.000W,REFL:18.2,RL:19.1,VALID:1
```

Packets are transmitted at 200 ms intervals when a CYD client is connected.

**GPIO Pin Assignment:**

| GPIO | Direction | Function |
|------|----------|---------|
| 35 | Input (ADC1_CH7) | AD8307 forward power output |
| 39 | Input (ADC1_CH3) | AD8307 reflected power output |
| 4 | Input (pull-up) | Band Up button (active-low) |
| 5 | Input (pull-up) | Band Down button (active-low) |
| 0 | Input (pull-up) | Tune button (active-low) |
| 2 | Output | Status/heartbeat LED |
| 17 | Output | SWR OK LED (green) |
| 16 | Output | SWR Warning LED (yellow) |
| 18 | Output | SWR High LED (red) |

## 8.2 CYD Display (cyd_whip_display.ino)

The CYD display is an ESP32-2432S028 module with a 320×240 ILI9341 TFT touchscreen. It operates as a Bluetooth Serial master, connecting to the "WHP-Tuner" device at startup and auto-reconnecting every 5 seconds if the link is lost.

**Display Pages:**

| Page | Tab Label | Content |
|------|----------|---------|
| 0 | SWR | Large SWR value with color coding; bargraph 1.0–10.0 with tick marks; return loss |
| 1 | SETUP | Extension length with proportional bar; loading coil identification; inline tuning guide |
| 2 | POWER | Forward power (W or mW); reflected power (dBm); return loss; reflection coefficient ρ; SWR |
| 3 | GUIDE | 4-line step-by-step tuning procedure appropriate to current band/coil type |

**Display Refresh Rate:** 10 FPS (frame period 100 ms), double-buffered sprite to prevent tearing.

**Touch Navigation:** The bottom 28 pixels of the screen contain a 4-zone touch bar. Touching the appropriate zone switches to that page immediately.

**Connection Indicator:** A colored dot in the upper right corner of the title bar indicates Bluetooth connection status: green when connected, red when disconnected or data stale.

**Auto-Reconnect:** The CYD attempts to reconnect to "WHP-Tuner" every 5 seconds when the Bluetooth link is absent. Displayed data is marked stale (VALID=0) if no packet is received for more than 3 seconds.

## 8.3 WiFi Access Point

| Parameter | Value |
|----------|-------|
| SSID | WHP-Tuner |
| Password | whip5p6m |
| Channel | 6 |
| IP Address | 192.168.4.1 |
| HTTP Port | 80 |
| Auto-refresh | 3 seconds (embedded meta-refresh in HTML) |
| BT Device Name | WHP-Tuner |
| BT Packet Interval | 200 ms |

The web UI provides a dark-theme responsive display suitable for operation in direct sunlight using a smartphone or tablet. Band selection is available via browser buttons at `/band_up` and `/band_dn`.

## 8.4 AD8307 Calibration Procedure

Calibration compensates for manufacturing variation in the AD8307 slope and intercept, component tolerances in the coupler, and any DC offset in the ESP32 ADC.

**Equipment Required:**
- Signal generator capable of −10 to +10 dBm output, accurate to ±0.5 dB
- 50-ohm termination (precision, BNC female)
- Laptop or smartphone with browser, connected to WHP-Tuner WiFi

**Procedure — Forward Channel Calibration:**

1. Connect signal generator to the SWR bridge RF input (SMA connector, through-line input).
2. Terminate the SWR bridge RF output (through-line output) in the 50-ohm termination.
3. Set signal generator to 7.15 MHz, 0 dBm output.
4. Open browser to `http://192.168.4.1/api/status`.
5. Record the `fwd_dbm` value. Compare to 0 dBm + 20 dB (coupler factor) = 20.0 dBm expected.
6. Compute calibration offset: `cal_fwd = 20.0 - fwd_dbm_reported`.
7. POST calibration offset: `curl -X POST http://192.168.4.1/api/cal -d "cal_fwd=X.X"` (substitute computed value).
8. Verify: refresh `/api/status` and confirm `fwd_dbm` reads 20.0 ± 0.3 dBm.

**Procedure — Reflected Channel Calibration:**

1. With signal generator at 7.15 MHz, 0 dBm, replace the 50-ohm termination with an OPEN circuit.
2. The reflected power should equal the forward power (all power reflected by open circuit).
3. Record `refl_dbm` value. Compare to expected 20.0 dBm.
4. Compute: `cal_refl = 20.0 - refl_dbm_reported`.
5. POST: `curl -X POST http://192.168.4.1/api/cal -d "cal_refl=X.X"`.
6. Verify: SWR should read approximately ∞:1 (or clamp to 10:1 per firmware).

> **NOTE**
> Calibration values are stored in ESP32 NVS (non-volatile storage) and survive power cycling. Repeat calibration whenever the SWR bridge is opened, components are replaced, or system temperature changes significantly (more than 20°C from calibration temperature).

---

# CHAPTER 9: CONSTRUCTION — BASE UNIT AND MATCHING NETWORK

## 9.1 Materials List

**Enclosure:**

| Item | Specification | Qty |
|------|-------------|-----|
| Base unit enclosure | OpenSCAD model: whip_base_unit.scad | 1 |
| Loading coil housing | OpenSCAD model: loading_coil_housing.scad (×5) | 5 |
| Roll-up wire spool | OpenSCAD model: rollup_whip_spool.scad | 1 |
| PLA or PETG filament | 1.75 mm, black recommended | As required |

**Hardware:**

| Item | Specification | Qty |
|------|-------------|-----|
| SO-239 chassis connector | UHF female, silver-plated | 1 |
| SMA connector (board mount) | Right-angle, 50 Ω | 2 |
| Banana jack (red) | 4 mm, panel mount | 2 |
| Banana jack (black) | 4 mm, panel mount | 2 |
| M6 stainless bolt + nut | 25 mm length, ground stud | 1 set |
| NMO coil socket | Brass, 3/8-24 thread | 2 |
| Variable capacitor | 20–500 pF air-variable or switched | 1 |
| PCB terminal blocks | 2-position, 3.5 mm pitch | 2 |
| M3 screws | 10 mm length, stainless | 12 |

**Electrical:**

| Item | Specification | Qty |
|------|-------------|-----|
| ESP32-WROOM-32 module | 38-pin development board | 1 |
| CYD display | ESP32-2432S028 | 1 |
| AD8307 | SOIC-8, 500 MHz log amp | 2 |
| FT-37-43 | 9.5 mm mix-43 toroid | 1 |
| FT-240-31 | 61 mm mix-31 toroid | 1 |
| 1N5711 Schottky diode | DO-35 or SOD-323 | 2 |
| 51 Ω, 1/4 W | Metal film | 4 |
| 100 nF ceramic | 50 V, 0805 or lead | 6 |
| LED green | 3 mm or 5 mm | 1 |
| LED yellow | 3 mm or 5 mm | 1 |
| LED red | 3 mm or 5 mm | 1 |
| 330 Ω resistor | 1/4 W, LED current limiting | 3 |
| Push button | Momentary SPST, PCB mount | 3 |
| 3.3 V regulator | AMS1117-3.3 or equivalent | 1 |
| Decoupling capacitors | 10 µF + 100 nF per rail | As required |
| RG-174 coax | 300 mm length for choke balun | 1 |

## 9.2 Base Unit Assembly

Print the base unit enclosure from `whip_base_unit.scad` using PLA or PETG. The enclosure accommodates the SWR bridge PCB, ESP32 module, L-network variable capacitor, SO-239 connector, and rear-panel banana jacks.

**Assembly Sequence:**

1. Print enclosure and loading coil housing shells.
2. Install SO-239 into the rear panel cutout; secure with four M3 screws.
3. Install banana jacks (2 red, 2 black) in the rear panel radial bus holes.
4. Mount the SWR bridge PCB on 4-mm standoffs inside the enclosure.
5. Mount the ESP32 development board adjacent to the SWR bridge on standoffs.
6. Install the variable capacitor (C_shunt) in the front panel with a shaft coupling to the panel knob.
7. Wire the variable capacitor between the SWR bridge through-line output and the NMO coil socket center conductor.
8. Wire the NMO coil socket to the variable capacitor (series connection to antenna).
9. Wire the choke balun between the SO-239 inner conductor and the SWR bridge input.
10. Connect the SWR bridge ground and the banana jacks to the SO-239 outer conductor.
11. Route GPIO 35 and GPIO 39 wires from the AD8307 outputs to the ESP32 ADC pins.
12. Install front-panel LEDs and buttons; connect to ESP32 GPIOs per the table in Chapter 8.
13. Apply 5 V to the ESP32 USB port and verify that the startup LED sequence (three flashes) occurs.

## 9.3 L-Network Wiring Per Band

The L-network in this system uses a single shared variable capacitor (C_shunt) that is adjusted for each band. The series arm is provided by the plug-in loading coil module for loading-coil bands. For no-coil bands, the whip is fed directly without a series L.

**Signal path with loading coil:**

```
SO-239 → Choke Balun → SWR Bridge → C_shunt (shunt to GND) → Loading Coil Module → Whip
                                                              ↑
                                                     L-network series arm
                                                     (provided by loading coil)
```

**Signal path without loading coil (17 m and above):**

```
SO-239 → Choke Balun → SWR Bridge → Direct connection → Whip
```

On 20 meters, a 300 pF NP0 capacitor is installed in series between the SWR bridge output and the whip base in place of the loading coil module.

## 9.4 Choke Balun

An RF common-mode choke prevents the coaxial feedline from radiating and maintains pattern symmetry. Install the choke balun between the transceiver and the base unit.

**Construction:**

- Core: FT-240-31 (Mix 31, outer diameter 61 mm)
- Winding: 8 turns of RG-174 coax, evenly spaced around core circumference
- Connection: coax center conductor and shield pass through as a continuous coaxial path

**Common-mode impedance provided:**

| Frequency (MHz) | Common-mode impedance (Ω) |
|----------------|--------------------------|
| 1.8 | ~1,800 |
| 3.5 | ~2,600 |
| 7.0 | ~2,100 |
| 14.0 | ~1,400 |
| 21.0 | ~900 |
| 28.0 | ~600 |

> **NOTE**
> For operation on 6 meters and above, substitute an FT-240-43 core or use an air-wound choke (6 turns, 25 mm diameter mandrel) for improved VHF performance.

---

# CHAPTER 10: FIELD SETUP CHECKLIST AND OPERATING PROCEDURES

## 10.1 Pre-Deployment Checklist

Before applying RF power to the antenna system, verify the following:

1. [ ] Whip sections inspected for corrosion, damaged locking collars, and mechanical looseness
2. [ ] Tip cap removed from Section 7 and stored in parts bag
3. [ ] Loading coil module selected for target band and locked into NMO socket
4. [ ] Whip installed in loading coil module socket and mechanically secure
5. [ ] All whip sections extended and locked at the correct position for the target band
6. [ ] Counterpoise wires appropriate length for target band; connected to banana jacks
7. [ ] Counterpoise wires spread at approximately 90-degree intervals and lying flat
8. [ ] Coaxial feedline connected to SO-239 and routed away from counterpoise
9. [ ] Choke balun installed at base unit SO-239 connection
10. [ ] ESP32 powered on; Bluetooth LED heartbeat observed or web interface accessible
11. [ ] CYD display powered on; band display matches intended operating band
12. [ ] No overhead power lines within falling distance of the antenna

## 10.2 Band Change Procedure

1. Reduce transmitter to STANDBY or zero power output before changing bands.
2. Press the Band Up or Band Down button on the base unit, or use the browser web interface at 192.168.4.1, to select the new band. Verify correct band displayed on CYD.
3. Unlock the whip by releasing the locking collar on the topmost extended section.
4. Retract or extend sections to match the extension length shown on the CYD SETUP page.
5. Align with the appropriate color-coded band mark; lock the section at that position.
6. If changing between loading-coil bands, swap loading coil modules (step 7 below).
7. If changing between a loading-coil band and a no-coil band, install or remove the loading coil module.
8. Deploy counterpoise wires of the correct length for the new band.
9. Apply a brief (3 second) low-power (1 to 5 watt) transmission and observe SWR on CYD.
10. Adjust C_shunt variable capacitor for minimum SWR reading (loading-coil bands only).
11. Verify SWR is below 1.5:1 before increasing to full power.

## 10.3 Loading Coil Swap Procedure

1. Reduce transmitter power to zero.
2. Grasp the loading coil module body firmly; do not apply torque to the coil winding.
3. Twist the module counterclockwise (looking up from the base) until it releases from the NMO socket.
4. Remove the module and store in the labeled compartment of the field kit.
5. Retrieve the replacement module for the new band.
6. Align the module pins with the NMO socket guide slots.
7. Insert module and twist clockwise until it seats firmly (a positive click or definite mechanical stop should be felt).
8. Grasp the module body and apply a firm upward pull. The module shall not move. If it does, re-seat and re-lock.
9. Verify module identification label matches the intended band.

> **CAUTION**
> Never apply RF power with a loading coil module that is not fully seated and locked. An arc at the module-to-socket interface will cause rapid pitting and corrosion of the brass contact surfaces.

## 10.4 SWR Adjustment Procedure

**For loading-coil bands (160 m through 30 m):**

1. Select the correct loading coil module and install per section 10.3.
2. Extend the whip fully to 5.60 meters; lock all sections.
3. Deploy counterpoise wires as specified in the extension table.
4. Apply a brief low-power transmission (1–5 W, 3 seconds).
5. Observe SWR on CYD Page 0 (SWR). Rotate the C_shunt variable capacitor slowly while observing the display.
6. Find the capacitor position that produces the lowest SWR reading.
7. If the minimum SWR is greater than 1.5:1 after full capacitor travel:
   a. Verify the loading coil module is seated correctly.
   b. Verify counterpoise wires are deployed and connected.
   c. Check for a loose section in the whip.
   d. Verify the loading coil module is the correct one for the operating band.
8. Note the capacitor position for the current band. Mark the dial or shaft for future reference.

**For no-coil bands (17 m through 10 m):**

1. Retract whip to the correct band mark.
2. Lock the topmost section at the mark position.
3. Deploy counterpoise wires of the correct length.
4. Apply a brief low-power transmission.
5. Observe SWR. On these bands the whip should be near-resonant without adjustment.
6. If SWR exceeds 1.5:1, extend or retract the tip section by 5 to 10 mm and re-test.

**For 20 meters:**

1. Install the 300 pF NP0 series capacitor at the base unit in place of the loading coil module. Alternatively, retract Section 7 to the 20-meter mark (5.05 m from base).
2. Apply a brief low-power transmission.
3. Adjust the series capacitor value if available, or trim the whip length as needed.

## 10.5 Safety Considerations

> **WARNING**
> Keep the antenna and all counterpoise wires out of contact with personnel during transmission. The antenna base and counterpoise wires carry RF voltage that can cause RF burns at HF power levels above 10 watts.

> **WARNING**
> The 5.6-meter fully extended whip is susceptible to mechanical failure in wind speeds above 30 km/h. Reduce extension to 4.0 meters (6 sections) or less in wind. Guy the base unit to a tent stake or vehicle body in all portable operations.

> **CAUTION**
> Do not touch the loading coil housing during transmission. At 100 watts, coil temperatures on the lower-efficiency bands (160 m, 80 m) can reach 60 to 80°C. Allow the coil to cool before handling after a transmission.

> **NOTE**
> The whip tip at 5.6 meters can accumulate static charge in dry conditions or near weather fronts. Install a high-value bleeder resistor (10 MΩ) between the antenna feed point and ground to discharge static safely. The ESP32 ADC inputs are particularly vulnerable to static discharge damage if the resistor is omitted.

---

# CHAPTER 11: TROUBLESHOOTING

The following table provides diagnostic guidance for common system faults. Perform troubleshooting at reduced power (1 to 5 watts) unless otherwise directed.

| Symptom | Probable Cause | Corrective Action |
|---------|---------------|------------------|
| High SWR on all bands | Coax connector failure or open | Inspect all coax connectors; verify continuity of center conductor and braid with ohmmeter |
| High SWR on all bands | Choke balun open | Bypass choke balun temporarily; if SWR improves, inspect balun winding for open turn |
| High SWR on all bands | Whip section not locked | Verify all extended sections are seated and locked; look for loose section (slight misalignment) |
| SWR reads 1.0:1 regardless of band or whip position | Coupler directional ports shorted | Inspect D1 and D2 Schottky diodes; a shorted diode will make forward and reflected appear equal |
| SWR reads 1.0:1 regardless of band or whip position | AD8307 power supply failure | Verify 3.3 V present at both AD8307 VPOS pins with voltmeter |
| SWR varies with transmitter power level | Diode detector nonlinearity | Upgrade to AD8307 configuration; diode detectors are nonlinear below 1 watt |
| SWR varies with transmitter power level | Feedline radiation (no balun) | Install or verify choke balun; without balun, operator body proximity changes feedpoint Z |
| No reading on CYD display | Bluetooth not connected | Verify "WHP-Tuner" appears in Bluetooth scan; ESP32 may have rebooted — power cycle base unit |
| No reading on CYD display | CYD display frozen | Power cycle CYD; if persistent, reflash firmware |
| Loading coil gets hot after 30 seconds of transmission | Excessive coil loss | Verify correct coil module is installed for band; verify counterpoise wires are deployed (high SWR forces large circulating current in coil) |
| Loading coil gets hot after 30 seconds of transmission | Power level too high for module rating | Reduce power to 50 watts or less; the compact T50-2 and T106-2 modules are not rated for 100 W continuous |
| SWR minimum shifts with frequency during transmit | Loading coil self-heating | Allow coil to cool between transmissions; reduce duty cycle; verify coil housing has adequate ventilation |
| SWR minimum shifts with frequency during transmit | Loose coil module contact | Remove and re-seat loading coil module; inspect contact pins for corrosion or deformation |
| Poor efficiency — signal reports consistently 10+ dB below comparable stations | Ground loss too high | Deploy more radials; use elevated radials; verify all 4 radials are actually connected |
| Poor efficiency on 160 m only | Radiation resistance too low | Expected; 1.8% coil efficiency on 160 m is inherent. Maximize radials (8 to 16 minimum); maximize coil Q; accept limitations of the electrically short antenna |
| Crackle or intermittent signal | Loose section joint | Collapse all sections; re-extend one at a time; twist each section firmly as it is extended; look for pitted contacts |
| Crackle or intermittent signal | Corroded NMO socket | Clean socket contacts with contact cleaner; lightly burnish with crocus cloth; apply conductive grease |
| VHF bands will not match; SWR remains above 2:1 | Counterpoise too long for VHF | Remove HF radial wires before operating on 6 m and above; HF counterpoise wires resonate at HF frequencies and distort VHF pattern |
| VHF bands will not match | Section tip not at correct mark | VHF is sensitive to ±5 mm error; verify position carefully at 2 m and above |
| Web interface not accessible | ESP32 WiFi AP not running | Connect directly to "WHP-Tuner" network; verify IP 192.168.4.1 is pingable; check ESP32 power |
| Web interface not accessible | Smartphone connected to wrong AP | Verify phone is connected to "WHP-Tuner" WiFi (password: whip5p6m) and not another network |
| ADC reads maximum (4095) on both channels with no RF | Static charge or electrostatic discharge | Verify 10 MΩ bleed resistor installed; check if either AD8307 is damaged by ESD |

---

# CHAPTER 12: PARTS REFERENCE

## 12.1 Loading Coil Materials

| Band | Core | N (turns) | Wire AWG | Core OD (mm) | Expected L (µH) | Winding Length (mm) |
|------|------|----------|---------|-------------|----------------|---------------------|
| 160 m | T200-2, AL=120 | 31 | #22 AWG | 50.8 | 115.4 | 45 |
| 80 m | T130-2, AL=110 | 18 | #20 AWG | 33.0 | 35.6 | 30 |
| 60 m | T130-2, AL=110 | 12 | #20 AWG | 33.0 | 15.8 | 25 |
| 40 m | T106-2, AL=135 | 8 | #18 AWG | 26.9 | 8.64 | 20 |
| 30 m | T50-2, AL=49 | 7 | #18 AWG | 12.7 | 2.40 | 15 |

## 12.2 LC Trap Materials

| Trap | Band | Core | N | L (µH) | C (pF) | C Type | f₀ (MHz) | Wire |
|------|------|------|---|--------|--------|--------|---------|------|
| T1 | 2 m | Air 4 mm | 2 | 0.018 | 68 | NP0 SMD 0805, 50 V | 145.2 | #28 AWG |
| T2 | 6 m | Air 5 mm | 3 | 0.060 | 160 | NP0 leaded, 100 V | 51.4 | #26 AWG |
| T3 | 10 m | T50-6 | 3 | 0.360 | 86.7 | NP0 (82+4.7), 500 V | 28.49 | #24 AWG |
| T4 | 12 m | T50-6 | 3 | 0.360 | 113.3 | NP0 (110+3.3), 500 V | 24.91 | #24 AWG |
| T5 | 15 m | T50-6 | 4 | 0.640 | 88.8 | NP0 (82+6.8), 500 V | 21.14 | #24 AWG |
| T6 | 17 m | T50-6 | 3 | 0.360 | 220 | NP0 leaded, 500 V | 17.88 | #24 AWG |

## 12.3 Electronic Components

| Component | Value/Part Number | Quantity | Function |
|----------|-----------------|---------|---------|
| AD8307AR | SOIC-8, Analog Devices | 2 | Logarithmic power detector, 500 MHz, 92 dB range |
| FT-37-43 | Mix 43, 9.5 mm OD toroid | 1 | Bruene coupler core |
| FT-240-31 | Mix 31, 61 mm OD toroid | 1 | Common-mode choke balun core |
| 1N5711 | Schottky diode, DO-35 | 2 | Envelope detector (alternate to AD8307) |
| ESP32-WROOM-32 | 38-pin dev board | 1 | WiFi/BT controller, ADC, web server |
| ESP32-2432S028 | CYD touchscreen module | 1 | 320×240 TFT display with touch |
| 51 Ω 1/4 W | Metal film, 1% | 4 | Coupler termination and detector loads |
| 100 nF ceramic | 50 V, C0G/NP0 | 6 | AC coupling and output filter |
| 10 MΩ 1/4 W | Carbon film | 1 | Antenna static bleed resistor |
| 330 Ω 1/4 W | Carbon film | 3 | LED current limiting |
| 3 mm LED green | 520 nm | 1 | SWR OK indicator |
| 3 mm LED yellow | 590 nm | 1 | SWR warning indicator |
| 3 mm LED red | 625 nm | 1 | SWR high indicator |
| Momentary SPST | PCB mount, 6 mm | 3 | Band up, band down, tune |

## 12.4 Hardware and Mechanical

| Item | Specification | Source |
|------|-------------|--------|
| 5.6 m telescoping whip | 7-section, 3/8-24 base, stainless or aluminum | Amateur radio mobile suppliers |
| NMO coil socket | Brass, 3/8-24 thread, center contact | CB/mobile antenna suppliers |
| SO-239 chassis connector | UHF female, silver-plated | Electronics distributors |
| 4 mm banana jacks | Panel mount, insulated, red and black | Electronics distributors |
| Variable capacitor | Air-variable, 20–500 pF, shaft-mount | Surplus electronics suppliers |
| M3 × 10 mm stainless screws | Pan-head Phillips | Hardware |
| M6 × 25 mm stainless bolt and nut | Ground stud | Hardware |
| RG-174 coaxial cable | 50 Ω, 300 mm | Electronics distributors |
| Heat-shrink tubing | 19 mm diameter (for trap housings) | Electronics distributors |
| Nail varnish / enamel paint | Assorted colors (see section 4.3) | Consumer supply |
| Counterpoise wire | 22 AWG stranded, 4 × 6 m lengths | Electronics distributors |

---

# APPENDIX A: COMPLETE LOADING COIL WINDING TABLE

The following table presents the complete design data for all five base loading coil modules. Data is from calculations file `loading_coil_tables.csv`.

| Band | f (MHz) | L_req (µH) | Xant (Ω) | Rrad (Ω) | Rcoil (Ω) | Eff (%) | Core | N | AWG | OD (mm) | Wind (mm) | Q | Zfeed (Ω) | Xs (Ω) | Xp (Ω) | C_shunt (pF) |
|------|---------|-----------|---------|---------|---------|--------|------|---|-----|---------|----------|---|----------|-------|-------|-------------|
| 160 m | 1.900 | 144.6 | 1730 | 0.124 | 6.919 | 1.8 | T200-2 | 31 | #22 | 50.8 | 45 | 250 | 7.04 | 17.4 | 20.2 | 4,137 |
| 80 m | 3.750 | 35.5 | 833 | 0.484 | 2.975 | 14.0 | T130-2 | 18 | #20 | 33.0 | 30 | 280 | 3.46 | 12.7 | 13.6 | 3,114 |
| 60 m | 5.330 | 16.3 | 543 | 0.977 | 1.906 | 33.9 | T130-2 | 12 | #20 | 33.0 | 25 | 285 | 2.88 | 11.7 | 12.4 | 2,414 |
| 40 m | 7.150 | 7.85 | 352 | 1.758 | 1.137 | 60.7 | T106-2 | 8 | #18 | 26.9 | 20 | 310 | 2.90 | 11.7 | 12.4 | 1,796 |
| 30 m | 10.125 | 2.477 | 158 | 3.525 | 0.494 | 87.7 | T50-2 | 7 | #18 | 12.7 | 15 | 320 | 4.02 | 13.6 | 14.8 | 1,063 |

**Notes by Band:**

**160 m:** The T200-2 core with 31 turns provides the required 144.6 µH. Efficiency is inherently very low (1.8%) due to the extreme electrical shortness of the antenna. Eight to sixteen elevated radials are essential for any useful performance. The C_shunt requirement of 4,137 pF is large; use a switched capacitor bank rather than a single air-variable unit.

**80 m:** Efficiency of 14% is marginal but usable for portable operation with a good counterpoise. The T130-2 core with 18 turns is a straightforward winding. Every additional radial wire on 80 meters provides measurable improvement in received signal reports.

**60 m:** The 60-meter WARC band at 5.330 MHz benefits from the moderate efficiency of 33.9%. An elevated counterpoise with 4 radials gives adequate QSO capability for field use.

**40 m:** At 60.7% coil efficiency, 40 meters is the primary practical field operating band. The T106-2 core with 8 turns of #18 AWG is compact and rugged. Two elevated radials are sufficient for acceptable performance.

**30 m:** The 30-meter band provides the best efficiency (87.7%) of the five loading-coil bands. The small T50-2 core with 7 turns of #18 AWG makes an extremely compact module. A single elevated radial is adequate for WARC band operation.

---

# APPENDIX B: COMPLETE TRAP WINDING TABLE

The following table presents the complete design data for all six inline LC trap assemblies used in the roll-up wire version. Data is from calculations file `trap_winding_tables.csv`.

| Trap | Band | f₀ (MHz) | Core | AL | N | L (µH) | C (pF) | C Type | V_rating | f₀_calc (MHz) | Z_trap (Ω) | Q | IL (dB) | Pos (m) | Hsg OD (mm) | Hsg L (mm) |
|------|------|---------|------|---|---|--------|--------|--------|---------|-------------|----------|---|---------|---------|------------|-----------|
| T1 | 2 m | 146.000 | Air 4 mm dia. | — | 2 | 0.018 | 68 | NP0 SMD 0805 | 50 V | 145.2 | 5,000 | 350 | 0.06 | 0.488 | 10 | 14 |
| T2 | 6 m | 51.000 | Air 5 mm dia. | — | 3 | 0.060 | 160 | NP0 | 100 V | 51.4 | 8,000 | 300 | 0.04 | 1.40 | 12 | 18 |
| T3 | 10 m | 28.500 | T50-6 | 40 | 3 | 0.360 | 86.7 | NP0 (82+4.7 pF) | 500 V | 28.49 | 16,100 | 250 | 0.02 | 2.55 | 19 | 22 |
| T4 | 12 m | 24.940 | T50-6 | 40 | 3 | 0.360 | 113.3 | NP0 (110+3.3 pF) | 500 V | 24.91 | 14,095 | 250 | 0.02 | 2.92 | 19 | 22 |
| T5 | 15 m | 21.225 | T50-6 | 40 | 4 | 0.640 | 88.8 | NP0 (82+6.8 pF) | 500 V | 21.14 | 17,000 | 250 | 0.03 | 3.43 | 19 | 25 |
| T6 | 17 m | 18.118 | T50-6 | 40 | 3 | 0.360 | 220 | NP0 | 500 V | 17.88 | 10,113 | 250 | 0.02 | 4.01 | 19 | 22 |

**Construction Notes by Trap:**

**T1 (2 m):** Air-core, 2 turns on a 4 mm mandrel (pencil or drill bit shank). The SMD 0805 NP0 capacitor is soldered directly to the coil ends. Housing is heat-shrink over the assembly. The Q of 350 for an air-core VHF trap gives very low insertion loss of 0.06 dB.

**T2 (6 m):** Air-core, 3 turns on a 5 mm mandrel. Leaded 160 pF NP0 capacitor. The housing is heat-shrink or a small slip of adhesive-lined heat-shrink. Position at 1.40 m corresponds exactly to the quarter-wave at 51 MHz.

**T3–T6 (10 m, 12 m, 15 m, 17 m):** All use T50-6 (Mix 6) cores with enameled wire. Mix 6 (Yellow/Clear) provides high Q above 30 MHz. The capacitor values are achieved by combining NP0 leaded capacitors in parallel. Verify the combined capacitance with an LCR meter before assembly.

---

# APPENDIX C: WHIP EXTENSION QUICK REFERENCE

The following table is designed for printing on a laminated pocket card (90 × 55 mm) and affixing inside the lid of the base unit field kit.

```
┌──────────────────────────────────────────────────────────────────┐
│           5.6m TELESCOPIC WHIP — QUICK REFERENCE                  │
│                        TM-WHP-001 Rev A                           │
├───────┬──────────┬────────────┬─────────────────┬────────────────┤
│ Band  │ Secs Ext │ Ext (m)    │ Loading Coil    │ Counterpoise   │
├───────┼──────────┼────────────┼─────────────────┼────────────────┤
│ 160 m │ 7 of 7   │ 5.60       │ Module 1        │ 4 × 5.6 m      │
│       │          │            │ (RED) 144.6 µH  │                │
├───────┼──────────┼────────────┼─────────────────┼────────────────┤
│ 80 m  │ 7 of 7   │ 5.60       │ Module 2        │ 4 × 5.6 m      │
│       │          │            │ (ORG) 35.5 µH   │                │
├───────┼──────────┼────────────┼─────────────────┼────────────────┤
│ 60 m  │ 7 of 7   │ 5.60       │ Module 3        │ 4 × 5.6 m      │
│       │          │            │ (YEL) 16.3 µH   │                │
├───────┼──────────┼────────────┼─────────────────┼────────────────┤
│ 40 m  │ 7 of 7   │ 5.60       │ Module 4        │ 4 × 4.0 m      │
│       │          │            │ (GRN) 7.85 µH   │                │
├───────┼──────────┼────────────┼─────────────────┼────────────────┤
│ 30 m  │ 7 of 7   │ 5.60       │ Module 5        │ 4 × 2.8 m      │
│       │          │            │ (BLU) 2.477 µH  │                │
├───────┼──────────┼────────────┼─────────────────┼────────────────┤
│ 20 m  │ 7 of 7   │ 5.13 retrt │ 300 pF cap      │ 4 × 2.5 m      │
│       │          │ or 5.60 +  │ or retract S7   │                │
│       │          │ cap        │ to WHITE mark   │                │
├───────┼──────────┼────────────┼─────────────────┼────────────────┤
│ 17 m  │ 6 of 7   │ 4.01       │ None            │ 4 × 2.0 m      │
├───────┼──────────┼────────────┼─────────────────┼────────────────┤
│ 15 m  │ 5 of 7   │ 3.43       │ None            │ 4 × 1.7 m      │
├───────┼──────────┼────────────┼─────────────────┼────────────────┤
│ 12 m  │ 4 of 7   │ 2.92       │ None            │ 4 × 1.5 m      │
├───────┼──────────┼────────────┼─────────────────┼────────────────┤
│ 10 m  │ 4 of 7   │ 2.55       │ None            │ 4 × 1.3 m      │
├───────┼──────────┼────────────┼─────────────────┼────────────────┤
│ 6 m   │ 2 of 7   │ 1.40       │ None            │ 4 × 0.35 m     │
│       │          │            │ ORANGE mark     │ rigid radials  │
├───────┼──────────┼────────────┼─────────────────┼────────────────┤
│ 2 m   │ 1 of 7   │ 0.488      │ None            │ 4 × 0.12 m     │
│       │          │            │ GREEN mark      │ rigid radials  │
├───────┼──────────┼────────────┼─────────────────┼────────────────┤
│ 70cm  │ 1 of 7   │ 0.160      │ None            │ 4 × 40 mm      │
│       │          │            │ WHITE mark      │ rigid radials  │
└───────┴──────────┴────────────┴─────────────────┴────────────────┘
```

---

# APPENDIX D: NEC QUICK REFERENCE CARD

| Card | Format | Example |
|------|--------|---------|
| CM | CM \<text\> | `CM Loading coil model 40m` |
| CE | CE | `CE` |
| GW | GW tag nseg x1 y1 z1 x2 y2 z2 rad | `GW 1 40 0 0 0.005 0 0 5.605 0.006` |
| GE | GE i1 | `GE 1` (1 = near-ground effects enabled) |
| GN | GN iperf nradl x1 y1 x2 y2 epsr sig | `GN 2 0 0 0 13 0.005` |
| LD | LD ldtyp ldtag ldtagf ldtagt zlr zli | `LD 0 1 1 1 1.14 352.5` |
| EX | EX extype tag seg ... vmag vphase | `EX 0 1 1 0 1 0` |
| FR | FR ifrq nfrq ... fmhz delfrq | `FR 0 7 0 0 7.000 0.050` |
| RP | RP mode ntheta nphi options theta0 phi0 dtheta dphi | `RP 0 37 73 1000 0 0 5 5` |
| EN | EN | `EN` |

**GW Geometry:** All coordinates in meters. Radius is conductor radius (not diameter) in meters.

**GN Type codes:** 0 = perfect ground, 1 = perfect (GE 1 near-field), 2 = Sommerfeld-Norton real ground.

**LD Type codes:** 0 = series R+jX (ohms); 4 = series R, L (H), C (F); 5 = shunt R+jX; 6 = wire loss.

**EX Type codes:** 0 = voltage source (V, degrees); 5 = current source; 6 = plane wave.

**RP Options field 1000:** Requests major lobe analysis output in addition to pattern data.

**Frequency note:** FR card specifies frequency in MHz. GW coordinates in meters. LD reactance in ohms at the specified frequency.

---

# APPENDIX E: QUICK REFERENCE FIELD CARD

The following information is the essential field operating guide. Print on waterproof paper (9 × 12 cm), laminate, and attach to the base unit carrying handle.

```
╔══════════════════════════════════════════════════════════════════╗
║         TM-WHP-001 — 5.6m TELESCOPIC WHIP FIELD CARD            ║
╠══════════════════════════════════════════════════════════════════╣
║  WiFi: WHP-Tuner  PW: whip5p6m  IP: 192.168.4.1                 ║
║  BT:   WHP-Tuner (auto-connect from CYD display)                 ║
╠══════════════════════════════════════════════════════════════════╣
║  SWR TARGET: ≤ 1.5:1      LED GREEN = OK / YELLOW = marginal     ║
╠══════════════════════════════════════════════════════════════════╣
║  BAND    EXT(m)  COIL MODULE   RADIALS          TUNE             ║
║  ──────  ──────  ───────────   ──────────────   ──────────────── ║
║  160m    5.60    RED (1)       4 × 5.6m         C_shunt var cap  ║
║  80m     5.60    ORANGE (2)    4 × 5.6m         C_shunt var cap  ║
║  60m     5.60    YELLOW (3)    4 × 5.6m         C_shunt var cap  ║
║  40m     5.60    GREEN (4)     4 × 4.0m         C_shunt var cap  ║
║  30m     5.60    BLUE (5)      4 × 2.8m         C_shunt var cap  ║
║  20m     5.13    300pF cap     4 × 2.5m         Trim cap/length  ║
║  17m     4.01    NONE          4 × 2.0m         Direct feed      ║
║  15m     3.43    NONE          4 × 1.7m         Direct feed      ║
║  12m     2.92    NONE          4 × 1.5m         Direct feed      ║
║  10m     2.55    NONE          4 × 1.3m         Direct feed      ║
║  6m      1.40    NONE          4 rigid × 350mm  Direct feed      ║
║  2m      0.488   NONE          4 rigid × 120mm  Direct feed      ║
╠══════════════════════════════════════════════════════════════════╣
║  COIL SWAP: reduce TX → twist CCW to remove → twist CW to lock   ║
║  SAFETY: verify coil locked before TX; no power lines in range   ║
║  WIND >30 km/h: retract to 4.0m max; guy base unit              ║
╠══════════════════════════════════════════════════════════════════╣
║  TROUBLESHOOTING                                                  ║
║  High SWR all bands: check coax connector, loose whip section    ║
║  SWR reads 1.0 always: check AD8307 power, diode D1/D2           ║
║  Coil hot: verify correct module; deploy counterpoise            ║
║  No CYD data: power cycle ESP32; check BT scan for WHP-Tuner     ║
╚══════════════════════════════════════════════════════════════════╝
```

---

## DOCUMENT CONTROL

| Revision | Date | Author | Changes |
|---------|------|--------|---------|
| Rev A | 2026-05-06 | M. Martin | Initial release |

**Cross-Reference Documents:**

| Document | Title |
|---------|-------|
| TM-WHP-SCH-001 Rev A | Base Loading Coil and L-Network Matching |
| TM-WHP-SCH-002 Rev A | Telescoping Whip Extension Guide — Band Selection |
| TM-WHP-SCH-003 Rev A | SWR Bridge and Directional Coupler |
| TM-WHP-NEC-001 through TM-WHP-NEC-012 | NEC-2 Antenna Models |
| loading_coil_tables.csv | Loading Coil Design Data |
| trap_winding_tables.csv | LC Trap Design Data |
| whip_dimensions.csv | Whip Section Dimensions and Extension Data |
| whip_base_unit.scad | Base Unit Enclosure (OpenSCAD) |
| loading_coil_housing.scad | Loading Coil Module Housing (OpenSCAD) |
| rollup_whip_spool.scad | Roll-Up Wire Storage Spool (OpenSCAD) |
| esp32_whip_tuner.ino | ESP32 Tuner Firmware |
| cyd_whip_display.ino | CYD Display Firmware |

---

*End of TM-WHP-001 Rev A — 5.6 Meter Telescopic Whip Antenna System*
