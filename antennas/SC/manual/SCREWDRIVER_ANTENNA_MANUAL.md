# TECHNICAL MANUAL — MOTORIZED VARIABLE LOADING COIL (SCREWDRIVER) ANTENNA SYSTEM
## Continuously Variable HF/VHF Antenna, 160 Meters Through 20 Centimeters

---

**Document Control Number:** TM-SC-001  
**Revision:** A  
**Date:** 2026-05-08  
**Classification:** UNCLASSIFIED — UNLIMITED DISTRIBUTION  
**Issuing Authority:** Merv Martin, WB6MRM

---

## RECORD OF CHANGES

| Change No. | Revision | Date       | Description                        | Entered By  |
|:----------:|:--------:|:----------:|------------------------------------|:-----------:|
| 1          | A        | 2026-05-08 | Initial issue — all chapters       | M. Martin   |

---

## TABLE OF CONTENTS

- [Chapter 1: Introduction and Operational Concept](#chapter-1)
- [Chapter 2: Electrically Short Monopole Theory](#chapter-2)
- [Chapter 3: Coil Design and Winding](#chapter-3)
- [Chapter 4: Mechanical Assembly](#chapter-4)
- [Chapter 5: Motor Control Electronics](#chapter-5)
- [Chapter 6: Directional Coupler Theory and SWR Measurement](#chapter-6)
- [Chapter 7: Roll-Up Wire Version](#chapter-7)
- [Chapter 8: NEC Modeling](#chapter-8)
- [Chapter 9: Firmware Reference](#chapter-9)
- [Chapter 10: Calibration](#chapter-10)
- [Chapter 11: Field Setup and Operating Procedures](#chapter-11)
- [Chapter 12: Troubleshooting](#chapter-12)
- [Chapter 13: Parts and Materials Reference](#chapter-13)
- [Appendix A: Complete Coil Winding Table](#appendix-a)
- [Appendix B: Position vs. Frequency Table](#appendix-b)
- [Appendix C: Half-Step Sequence and Timing Table](#appendix-c)
- [Appendix D: NEC Card Reference](#appendix-d)
- [Appendix E: Quick Reference Field Card](#appendix-e)
- [Appendix F: Wiring Diagram (ASCII)](#appendix-f)

---

---

<a name="chapter-1"></a>
## CHAPTER 1: INTRODUCTION AND OPERATIONAL CONCEPT

### 1.1 Purpose

This manual describes the design, construction, calibration, and operation of the Motorized Variable Loading Coil (Screwdriver) Antenna System, designated SC-001. The system provides continuous antenna tuning across the HF spectrum from 160 meters (1.8 MHz) through 10 meters (30 MHz) using a single fixed 2.4-meter stainless steel whip element with a motor-driven variable loading coil at the base.

The operating principle is straightforward: a 2.4-meter whip is electrically short on all HF bands below 10 meters. At 10 meters, the whip is approximately a quarter-wave resonant element and requires no loading. On all lower bands, the whip presents a large capacitive reactance at its feed point that must be cancelled by an equal and opposite inductive reactance to achieve resonance. A series base loading coil of the correct inductance accomplishes this. By varying the effective inductance of this coil — achieved by moving a wiper contact up or down the turns of a helical coil — the antenna can be tuned to resonance on any HF frequency.

The 28BYJ-48 stepper motor drives a 1/4-20 threaded rod that moves the wiper contact carriage to any of 7,420 half-step positions along the coil. An ESP32 microcontroller manages motor position, reads forward and reflected power from an AD8307-based directional coupler, computes SWR, and runs an auto-tune algorithm that converges on the minimum-SWR coil position automatically. A CYD (Cheap Yellow Display) touchscreen provides local operator interface. The system can also be controlled via REST API over WiFi and via Bluetooth from a remote CYD panel.

### 1.2 History and Nomenclature

The "screwdriver antenna" name derives from early implementations built using a Black & Decker Model 9071 cordless screwdriver as the motorized drive mechanism. The screwdriver's planetary gear motor provided adequate torque and speed for wiper travel and the entire assembly was compact enough for mobile installation. The first documented design was published by Don Johnson, W6AAQ, in 1991 in QST magazine. Johnson's design established the essential elements still used today: a tall helical loading coil, motor-driven wiper contact, and remote tuning control.

Commercial variants that followed the W6AAQ design include the Hi-Q Antennas series (designed by Bill Hohnstein, K0HA), the Tarheel Antennas series, and the Scorpion Antennas series. All use the same operating principle — variable inductive loading at the base of an electrically short monopole — differing mainly in coil dimensions, motor type, and mounting hardware.

This system replaces the original screwdriver motor with a 28BYJ-48 stepper motor for precise step counting and position memory, and adds the ESP32-based controller with SWR sensing for automated tuning. The mechanical and electrical loading coil principles are identical to the original W6AAQ design.

### 1.3 System Components

| Component | Description | Quantity |
|-----------|-------------|:--------:|
| Whip element | 2.4 m × 8 mm diameter stainless steel, 5/8-24 thread base | 1 |
| Coil housing | 75 mm diameter PVC form, 95 mm winding length | 1 |
| Main loading coil | 41 turns #14 AWG tinned copper, close-wound, 2.3 mm/turn pitch | 1 |
| Extension coil | T200-2 toroid, 90 turns #18 AWG, 210 µH (160m only) | 1 |
| Stepper motor | 28BYJ-48, 5V unipolar, 64:1 internal gear ratio | 1 |
| Motor driver | ULN2003 Darlington array board | 1 |
| Drive rod | 1/4-20 stainless threaded rod, 150 mm | 1 |
| Jaw coupler | 5 mm to 1/4-20 flexible jaw coupler | 1 |
| Wiper carriage | 3D-printed PETG, captive M5 nut, phosphor bronze contact arm | 1 |
| Wiper contact | Phosphor bronze leaf spring, silver-paste contact tip | 1 |
| Controller | ESP32-WROOM-32 development board | 1 |
| SWR bridge | Bruene-type directional coupler, FT-37-43, 10T secondary | 1 |
| Log amplifier | AD8307 power detector module (×2, fwd and refl) | 2 |
| Display | CYD (ESP32-2432S028, 2.8" 320×240 TFT, capacitive touch) | 1 |
| Limit switches | Momentary microswitch, 5V pull-up (×2, home and top) | 2 |
| Base mount | SO-239 flange mount, 4-radial connector block | 1 |
| Housing | 3D-printed PETG upper and lower housing, UV-resistant | 2 |

### 1.4 System Limitations

| Limitation | Value | Notes |
|------------|-------|-------|
| Frequency coverage, HF | 1.8 – 30.0 MHz | Continuous, all amateur bands |
| Frequency coverage, VHF | 144 MHz, 222 MHz, 432 MHz | Requires coil removal; direct element |
| Minimum achievable SWR, 10m–17m | < 1.3 : 1 | High radiation resistance; good match |
| Minimum achievable SWR, 20m–40m | < 1.8 : 1 | Moderate radiation resistance; tap compromise |
| Minimum achievable SWR, 80m–160m | < 2.5 : 1 | Low radiation resistance; efficiency-limited |
| Maximum power, 10m–17m | 100 W continuous | Limited by wiper contact rating |
| Maximum power, 20m–40m | 50 W continuous | Limited by coil heating |
| Maximum power, 80m | 25 W continuous | Coil Q heating at high Vc |
| Maximum power, 160m | 5 W continuous | Extension coil thermal limit |
| Position resolution | 1 half-step = 0.049 mm wiper travel | 7,420 positions total |
| Homing accuracy | ±1 step of home position | Magnetic reed home switch |
| Operating temperature | −10°C to +55°C | Motor and PETG limits |
| Wind load | Maximum 50 km/h for portable mast | Reduce whip length in high wind |

### 1.5 Performance Overview — All Bands

| Band | Freq (MHz) | Efficiency (%) | L_coil (µH) | Active Turns | Exp. SWR Range | Notes |
|------|:----------:|:--------------:|:-----------:|:------------:|:--------------:|-------|
| 160m | 1.85       | 0.8            | 210 (ext)   | 41+ext       | 2.0–3.5        | Extension coil required |
| 80m  | 3.75       | 2.8            | 49.2        | 39           | 1.8–2.5        | Marginal efficiency |
| 60m  | 5.33       | 5.6            | 24.1        | 34           | 1.5–2.0        | Good for NVIS |
| 40m  | 7.15       | 9.5            | 13.1        | 27           | 1.5–2.0        | Practical portable |
| 30m  | 10.125     | 17.2           | 6.4         | 22           | 1.3–1.8        | CW only band |
| 20m  | 14.175     | 32.4           | 3.1         | 17           | 1.2–1.6        | Primary DX band |
| 17m  | 18.118     | 51.7           | 1.8         | 13           | 1.1–1.5        | Excellent efficiency |
| 15m  | 21.225     | 65.1           | 1.1         | 10           | 1.1–1.4        | High efficiency |
| 12m  | 24.94      | 78.3           | 0.55        | 7            | 1.1–1.3        | Near resonant length |
| 10m  | 28.5       | 91.2           | 0.12        | 3            | 1.0–1.2        | Near-QW resonant |

---

---

<a name="chapter-2"></a>
## CHAPTER 2: ELECTRICALLY SHORT MONOPOLE THEORY

> **NOTE:** This chapter provides the mathematical foundation for all design choices in this manual. Operators who understand the theory will make better decisions about operating practices, power limits, and ground system requirements. Read this chapter carefully.

### 2.1 Radiation Resistance vs. Antenna Height

#### 2.1.1 The Short Monopole Model

A monopole antenna above a perfect ground plane is the image-theory equivalent of a dipole of twice the height. For a short monopole (height h much less than λ/4), the radiation resistance is:

```
Rrad = 10π²(h/λ)²     [Ohms]
```

This formula assumes a linear current distribution from maximum at the base to zero at the tip, which is a reasonable approximation for a loaded monopole shorter than λ/8. The (h/λ)² dependence is critical: radiation resistance drops as the square of the electrical shortening ratio.

For reference, a true quarter-wave monopole has Rrad ≈ 36.5 Ω (half the 73 Ω dipole value).

#### 2.1.2 Radiation Resistance Table — 2.4m Whip

The following table gives calculated Rrad for the 2.4m whip on all HF amateur bands. The ratio h/λ uses h = 2.4 m.

| Band | Freq (MHz) | λ (m)  | h/λ    | (h/λ)² | Rrad (Ω) | QW Rrad (Ω) | Rrad Ratio |
|------|:----------:|:------:|:------:|:-------:|:--------:|:-----------:|:----------:|
| 160m | 1.85       | 162.16 | 0.0148 | 0.000219| 0.0216   | 36.5        | 1:1689     |
| 80m  | 3.75       | 79.98  | 0.0300 | 0.000900| 0.0888   | 36.5        | 1:411      |
| 60m  | 5.33       | 56.28  | 0.0426 | 0.00182 | 0.180    | 36.5        | 1:203      |
| 40m  | 7.15       | 41.94  | 0.0572 | 0.00327 | 0.323    | 36.5        | 1:113      |
| 30m  | 10.125     | 29.60  | 0.0811 | 0.00658 | 0.650    | 36.5        | 1:56       |
| 20m  | 14.175     | 21.15  | 0.1135 | 0.01289 | 1.273    | 36.5        | 1:29       |
| 17m  | 18.118     | 16.55  | 0.1450 | 0.02103 | 2.076    | 36.5        | 1:18       |
| 15m  | 21.225     | 14.13  | 0.1699 | 0.02887 | 2.851    | 36.5        | 1:13       |
| 12m  | 24.94      | 12.03  | 0.1996 | 0.03984 | 3.934    | 36.5        | 1:9.3      |
| 10m  | 28.5       | 10.52  | 0.2282 | 0.05208 | 5.145    | 36.5        | 1:7.1      |

> **NOTE:** The Rrad for 160m (0.022 Ω) is smaller than the DC resistance of a 2.4m length of #14 AWG wire (approximately 0.016 Ω). This is the fundamental reason why 160m operation from a short mobile antenna is so difficult — the antenna radiates almost nothing compared to what it dissipates.

### 2.2 Reactive Feed Impedance of the Short Monopole

#### 2.2.1 Characteristic Impedance of the Whip

The characteristic impedance (surge impedance) of the whip element, treated as an open-circuited transmission line stub, is:

```
Zc = 60 × [ln(2h/a) - 1]     [Ohms]
```

Where:
- h = physical length = 2.4 m
- a = conductor radius = 0.004 m (8 mm diameter whip)

```
Zc = 60 × [ln(2 × 2.4 / 0.004) - 1]
   = 60 × [ln(1200) - 1]
   = 60 × [7.090 - 1]
   = 60 × 6.090
   = 365 Ω
```

> **NOTE:** Some references use 324 Ω for a 2.4m whip with 8mm conductor. The discrepancy arises from whether the formula accounts for a tapered element or cylindrical approximation. This manual uses Zc = 365 Ω for a uniform 8mm diameter cylindrical whip as the more physically accurate value.

#### 2.2.2 Feed Reactance Formula

The antenna feed reactance (the reactance seen looking into the base of the open-circuited whip) is:

```
Xant = -Zc × cot(βh)     [Ohms, capacitive when βh < 90°]
```

Where:
- β = 2πf/c = wave number (radians/meter)
- βh = electrical length in radians = 2πfh/c
- cot(βh) = cos(βh)/sin(βh)

When βh < π/2 (i.e., shorter than quarter-wave), cot(βh) is positive, making Xant negative (capacitive).

#### 2.2.3 Feed Reactance Table — 2.4m Whip, All HF Bands

| Band | Freq (MHz) | βh (rad) | βh (deg) | cot(βh) | Xant (Ω)  | Character  |
|------|:----------:|:--------:|:--------:|:-------:|:---------:|:----------:|
| 160m | 1.85       | 0.0930   | 5.33°    | 10.71   | −3910     | Capacitive |
| 80m  | 3.75       | 0.1885   | 10.80°   | 5.24    | −1912     | Capacitive |
| 60m  | 5.33       | 0.2678   | 15.34°   | 3.64    | −1329     | Capacitive |
| 40m  | 7.15       | 0.3592   | 20.59°   | 2.65    | −968      | Capacitive |
| 30m  | 10.125     | 0.5089   | 29.17°   | 1.797   | −656      | Capacitive |
| 20m  | 14.175     | 0.7126   | 40.83°   | 1.159   | −423      | Capacitive |
| 17m  | 18.118     | 0.9107   | 52.17°   | 0.773   | −282      | Capacitive |
| 15m  | 21.225     | 1.0669   | 61.13°   | 0.554   | −202      | Capacitive |
| 12m  | 24.94      | 1.2533   | 71.81°   | 0.323   | −118      | Capacitive |
| 10m  | 28.5       | 1.4326   | 82.08°   | 0.141   | −51.5     | Capacitive |

> **NOTE:** At 10 meters, the whip is nearly resonant (βh = 82°). The residual −51.5 Ω reactance can be cancelled by only 3 turns of the loading coil or by adjusting the physical length slightly. This is why 10m operation is so efficient — very little loading is required and coil losses are minimal.

### 2.3 Loading Coil Design

#### 2.3.1 Resonance Condition

To achieve resonance, the loading coil must present an equal and opposite reactance to cancel the antenna's capacitive feed reactance:

```
XL = |Xant|     [Ohms]
```

The required inductance at each frequency is:

```
L = XL / (2πf) = |Xant| / (2πf)     [Henries]
```

#### 2.3.2 Required Inductance Table

| Band | Freq (MHz) | |Xant| (Ω) | Required L (µH) |
|------|:----------:|:-----------:|:---------------:|
| 160m | 1.85       | 3910        | 336.5           |
| 80m  | 3.75       | 1912        | 81.2            |
| 60m  | 5.33       | 1329        | 39.7            |
| 40m  | 7.15       | 968         | 21.5            |
| 30m  | 10.125     | 656         | 10.3            |
| 20m  | 14.175     | 423         | 4.76            |
| 17m  | 18.118     | 282         | 2.48            |
| 15m  | 21.225     | 202         | 1.52            |
| 12m  | 24.94      | 118         | 0.753           |
| 10m  | 28.5       | 51.5        | 0.288           |

> **NOTE:** The 160m requirement (336.5 µH) far exceeds what the main coil can provide (approximately 49 µH at full 41 turns). This is why a separate 160m extension coil (T200-2 toroid, ~210 µH) is wired in series with the main coil when 160m operation is desired. Together they provide approximately 259 µH, still short of ideal, which accounts for the compromised efficiency on 160m.

#### 2.3.3 Wheeler Formula for Single-Layer Solenoid

The inductance of a single-layer air-core solenoid is calculated using the Wheeler (1928) formula, accurate to within 1% for coils where length-to-radius ratio is between 0.4 and 8:

```
L(µH) = r²N² / (22.86r + 25.4l)

Where:
  r = coil radius in centimeters
  l = coil winding length in centimeters
  N = number of turns
```

For the main coil parameters:
- r = 3.75 cm (75 mm diameter form, 7.5 mm wall assumed negligible for winding radius)
- #14 AWG wire: 1.628 mm diameter
- Pitch with close winding: 1.628 mm + 0.2 mm spacing = ~1.828 mm ≈ 1.85 mm/turn

> **NOTE:** In practice, #14 AWG close-wound on a 75mm form achieves approximately 2.3 mm/turn when accounting for the wire diameter plus the natural separation that occurs during winding. This manual uses 2.3 mm/turn as the measured value.

For the 41-turn main coil with 2.3 mm/turn pitch:
- Winding length l = 41 × 0.23 cm = 9.43 cm

```
L(N) = (3.75)² × N² / (22.86 × 3.75 + 25.4 × (N × 0.23))
     = 14.0625 × N² / (85.73 + 5.842 × N)     [µH]
```

This is the working formula for the SC-001 main coil. For N = 41 turns:

```
L(41) = 14.0625 × 1681 / (85.73 + 5.842 × 41)
      = 23639 / (85.73 + 239.52)
      = 23639 / 325.25
      = 72.7 µH
```

> **NOTE:** This is the total inductance of the fully wound 41-turn main coil. In practice, the effective inductance used ranges from near zero (10m, wiper at ~3 turns) to 72.7 µH (80m, wiper at full 41 turns). The 80m resonant inductance calculated in Section 2.3.2 (81.2 µH) requires slightly more than 41 turns at the calculated pitch, confirming that 80m operation is at the upper limit of the main coil range. For reliable 80m performance, the coil is wound at a slightly tighter pitch than nominal, or the 160m extension coil is left in circuit and the main coil wiper is backed off.

#### 2.3.4 Coil Q vs. Frequency

The Q (quality factor) of the loading coil is the ratio of reactive power stored to resistive power dissipated. For #14 AWG close-wound single-layer air coil:

```
Q ≈ (ωL) / Rcoil_ac
```

The AC resistance increases with frequency due to skin effect:

```
Rskin = Rdc × sqrt(f / f_corner)     for f >> f_corner
```

For #14 AWG copper (Rdc = 8.286 mΩ/m):
- f_corner (skin depth = wire radius) ≈ 34 kHz
- At HF frequencies, Rskin >> Rdc

Measured Q values for close-wound #14 AWG single-layer solenoids in the HF range:

| Band | Freq (MHz) | Typical Q | Notes |
|------|:----------:|:---------:|-------|
| 160m | 1.85       | 180–220   | Distributed capacitance begins to limit Q |
| 80m  | 3.75       | 220–260   | Good Q; close-wound acceptable |
| 40m  | 7.15       | 260–300   | Moderate Q |
| 20m  | 14.175     | 320–360   | High Q; few turns active |
| 15m  | 21.225     | 360–400   | Near-peak Q for this wire gauge |
| 10m  | 28.5       | 350–380   | Slight decrease due to self-resonance proximity |

This manual uses conservative Q values: Q = 200 at 160m, Q = 250 at 80m, Q = 280 at 40m, Q = 340 at 20m, Q = 380 at 15m, Q = 360 at 10m.

#### 2.3.5 Loading Coil Resistance

The effective resistance of the loading coil (the power-dissipating component) is:

```
Rcoil = XL / Q = |Xant| / Q     [Ohms]
```

| Band | Freq (MHz) | |Xant| (Ω) | Q   | Rcoil (Ω) |
|------|:----------:|:-----------:|:---:|:---------:|
| 160m | 1.85       | 3910        | 200 | 19.6      |
| 80m  | 3.75       | 1912        | 250 | 7.65      |
| 60m  | 5.33       | 1329        | 265 | 5.02      |
| 40m  | 7.15       | 968         | 280 | 3.46      |
| 30m  | 10.125     | 656         | 305 | 2.15      |
| 20m  | 14.175     | 423         | 340 | 1.24      |
| 17m  | 18.118     | 282         | 365 | 0.772     |
| 15m  | 21.225     | 202         | 380 | 0.532     |
| 12m  | 24.94      | 118         | 375 | 0.315     |
| 10m  | 28.5       | 51.5        | 360 | 0.143     |

### 2.4 System Efficiency

#### 2.4.1 Efficiency Formula

The radiation efficiency of the antenna system is the fraction of power delivered to the antenna feed point that is actually radiated. With a series loading coil, the two dominant loss resistances are Rrad and Rcoil (ground system losses are addressed separately in Section 2.6):

```
η = Rrad / (Rrad + Rcoil) × 100%     [percent]
```

> **WARNING:** Ground system resistance Rgnd is not included in this formula. Poor ground systems add additional series resistance that further reduces efficiency. For the purposes of this table, an ideal ground plane is assumed. Real-world efficiency will be lower.

#### 2.4.2 System Efficiency Table — All HF Bands

| Band | Freq (MHz) | Rrad (Ω) | Rcoil (Ω) | η (%) | dB Loss | Power radiated at 100W input |
|------|:----------:|:--------:|:---------:|:-----:|:-------:|:----------------------------:|
| 160m | 1.85       | 0.0216   | 19.6      | 0.11  | −29.6   | 0.11 W                       |
| 80m  | 3.75       | 0.0888   | 7.65      | 1.15  | −19.4   | 1.15 W                       |
| 60m  | 5.33       | 0.180    | 5.02      | 3.46  | −14.6   | 3.46 W                       |
| 40m  | 7.15       | 0.323    | 3.46      | 8.55  | −10.7   | 8.55 W                       |
| 30m  | 10.125     | 0.650    | 2.15      | 23.2  | −6.35   | 23.2 W                       |
| 20m  | 14.175     | 1.273    | 1.24      | 50.6  | −2.96   | 50.6 W                       |
| 17m  | 18.118     | 2.076    | 0.772     | 72.9  | −1.37   | 72.9 W                       |
| 15m  | 21.225     | 2.851    | 0.532     | 84.3  | −0.74   | 84.3 W                       |
| 12m  | 24.94      | 3.934    | 0.315     | 92.6  | −0.33   | 92.6 W                       |
| 10m  | 28.5       | 5.145    | 0.143     | 97.3  | −0.12   | 97.3 W                       |

> **NOTE:** The transition from poor to acceptable efficiency occurs in the 17m–20m range, which is why these bands are the primary DX operating bands for mobile screwdriver antennas. The antenna performs essentially as well as a full-size quarter-wave vertical on 15m, 12m, and 10m.

#### 2.4.3 QRP Implications

At 5 watts input power:

| Band | 5W input → Radiated | Effective ERP vs. QW vertical |
|------|:-------------------:|:------------------------------:|
| 160m | 5.5 mW              | −42.6 dB (ineffective)         |
| 80m  | 57 mW               | −19.4 dB (marginal)            |
| 40m  | 0.43 W              | −10.7 dB (usable, short range) |
| 20m  | 2.53 W              | −3.0 dB (practical)            |
| 17m  | 3.65 W              | −1.4 dB (very good)            |
| 10m  | 4.87 W              | −0.1 dB (equivalent to QW)     |

> **CAUTION:** Do not attempt 160m or 80m QRP DX contacts with this antenna from a portable installation. The combination of low efficiency and inadequate ground system makes contacts unreliable except in rare band conditions. Use full power (within power limits) on these bands.

#### 2.4.4 Comparison to Alternative Antenna Types

| Antenna Type | 40m Efficiency | 20m Efficiency | Portability | Notes |
|--------------|:--------------:|:--------------:|:-----------:|-------|
| SC-001 Screwdriver (2.4m) | 8.6% | 50.6% | Excellent | This system |
| 7m vertical (QW 40m) | ~85% | ~75% | Good | Separate 20m element needed |
| EFHW 40m | ~95% | ~92% | Good | Long wire, needs tuner |
| Hamstick (1.8m) | ~5% | ~40% | Excellent | Single band each |
| Full-size dipole 40m | ~98% | ~95% | Poor | 20m span required |
| Magnetic loop (1m) | 60–70% | 70–80% | Fair | Narrow bandwidth, fragile |

### 2.5 Impedance Matching — Autotransformer Tap

#### 2.5.1 The Impedance Problem

At resonance, the antenna feed impedance is purely resistive at:

```
Zfeed = Rrad + Rcoil + Rgnd     [Ohms]
```

This value ranges from approximately 0.1 Ω (160m) to 5.3 Ω (10m). Standard coaxial transmission line and transmitter output impedance is 50 Ω. A direct connection would result in severe SWR.

#### 2.5.2 Autotransformer Tap Solution

The loading coil itself serves as the primary of an autotransformer. By connecting the coax center conductor to a tap N_tap turns up from the bottom of the coil (instead of at the coil base), the impedance is transformed:

```
Ztransformed = Zfeed × (N_total / N_tap)²     [Ohms]
```

The ideal tap for a 50 Ω match is:

```
N_tap = N_total × sqrt(Zfeed / 50)     [turns]
```

#### 2.5.3 Tap Position Analysis

The challenge is that Zfeed varies by a factor of 50:1 across the HF bands, while N_tap is a fixed physical connection. A standard design compromise uses N_tap = 4 turns:

| Band | Zfeed (Ω) | Ideal N_tap | Actual N_tap | Transformed Z (Ω) | SWR to 50Ω |
|------|:---------:|:-----------:|:------------:|:------------------:|:----------:|
| 160m | 19.6      | 41×√(19.6/50)=25.7 | 4   | 19.6×(41/4)²=2056  | 41.1:1 → requires tuner |
| 80m  | 7.74      | 41×√(7.74/50)=16.1 | 4   | 7.74×(41/4)²=813   | 16.3:1 → requires tuner |
| 40m  | 3.78      | 31×√(3.78/50)=8.5  | 4   | 3.78×(31/4)²=227   | 4.5:1 → marginal |
| 20m  | 2.51      | 20×√(2.51/50)=4.5  | 4   | 2.51×(20/4)²=157   | 3.1:1 → fine-tune coil |
| 17m  | 2.85      | 15×√(2.85/50)=3.6  | 4   | 2.85×(15/4)²=40.2  | 1.2:1 → good |
| 10m  | 5.29      | 4×√(5.29/50)=1.3   | 3   | 5.29×(4/3)²=9.4    | 5.3:1 → direct feed better |

> **NOTE:** The tap analysis reveals why screwdriver antennas are typically operated with a short-range automatic antenna tuner (ATU) in the coax feed line. The tap provides a useful starting point for matching, but the wide variation in Zfeed means the transceiver's internal ATU (typically capable of matching up to 3:1) handles the residual mismatch. For 160m and 80m, an external ATU with wider range is required. The auto-tune algorithm in Chapter 5 moves the wiper to minimize SWR at the tap connection, which naturally optimizes the transformed impedance for each band.

#### 2.5.4 Wiper Position vs. SWR

In practice, the auto-tune algorithm finds minimum SWR by sweeping the wiper position. The minimum SWR at the tap point is achieved when the combination of coil inductance and its tap ratio produces the closest match to 50 Ω. This position is not necessarily the exact resonant position of the antenna — it is the position where the transformed antenna impedance most closely equals 50 Ω. The difference in wiper positions between "exact resonance" and "minimum SWR" is typically less than 2 turns, and the auto-tune algorithm finds the SWR minimum regardless.

### 2.6 Ground System Requirements

#### 2.6.1 The Counterpoise

The performance of any vertical monopole depends critically on the return current path — the ground system. For a portable installation, this is provided by radials. Each radial is a wire that extends horizontally from the antenna base and provides the return RF current path.

The minimum practical ground system for this antenna is:
- **Minimum:** 2 radials, each at least λ/4 at the lowest operating frequency
- **Recommended portable:** 4 radials, 5m each (reasonable compromise for all HF bands)
- **Best performance:** 4 radials cut to λ/4 at primary operating frequency

#### 2.6.2 Ground Resistance Effect

Ground system resistance Rgnd adds directly to the denominator of the efficiency equation:

```
η_actual = Rrad / (Rrad + Rcoil + Rgnd) × 100%
```

For a 2-radial ground system on 40m, Rgnd ≈ 5–10 Ω is typical. With Rgnd = 7 Ω:

```
η_40m_actual = 0.323 / (0.323 + 3.46 + 7.0) = 0.323 / 10.78 = 3.0%
```

This reduces the already modest 40m efficiency (8.6% ideal) by a further factor of 2.8.

#### 2.6.3 Effect on Resonant Frequency

A poor ground system also shifts the resonant frequency downward (because the missing counterpoise effectively lengthens the electrical antenna). The auto-tune algorithm compensates for this automatically, but it means that band positions stored in NVS with one ground configuration may not be valid with a different ground system.

> **CAUTION:** Always re-run auto-tune when changing the ground system (different radials, different operating location, or mounting on a vehicle vs. portable mast). Stored band positions are site-specific.

---

---

<a name="chapter-3"></a>
## CHAPTER 3: COIL DESIGN AND WINDING

### 3.1 Main Coil Specifications

| Parameter | Value |
|-----------|-------|
| Form diameter (OD) | 75 mm |
| Form material | Schedule 40 PVC, 3 mm wall |
| Effective winding radius (r) | 3.75 cm (measured to wire centerline) |
| Wire | #14 AWG tinned copper, 1.628 mm diameter |
| Winding style | Close wound, single layer |
| Pitch | 2.3 mm/turn (measured; wire dia + insulation clearance) |
| Total turns | 41 |
| Total winding length | 94.3 mm (41 × 2.3 mm) |
| Total inductance (41T) | 72.7 µH (Wheeler formula) |
| Total inductance (41T, measured) | 68–74 µH (±5% typical) |
| Form length (total, including endcaps) | 140 mm |
| Winding area clear zone each end | 20 mm (for endcap and wiper clearance) |
| Maximum operating current | 5A RMS (limited by wiper contact, not wire) |
| Wire resistance, full 41T | 0.11 Ω DC |
| Self-resonant frequency (SRF) | > 60 MHz (measured; avoid operation above 0.5 × SRF) |

### 3.2 Winder Construction Procedure

> **NOTE:** Use a lathe or drill-press as a winding mandrel for best results. A hand winder is acceptable but requires more care to maintain consistent pitch.

**Required materials:** Schedule 40 PVC pipe 75mm OD (inside diameter 68mm), #14 AWG tinned copper wire (10m), fine-grit sandpaper (400 grit), epoxy or clear varnish, masking tape, end caps (3D-printed or turned PVC).

**Procedure:**

1. Cut PVC form to 140 mm length. Deburr both ends. Sand the outer surface lightly with 400-grit paper to improve epoxy adhesion.

2. Mark a line along the length of the form with a permanent marker. This line will be the reference for the wiper contact track groove.

3. Score a 2mm-wide groove along the marked line, full length of the winding area (94.3 mm), using a rotary tool with a small routing bit. This groove receives the silver-paint contact track.

4. Apply a thin bead of Hobby-Coat clear varnish or diluted polyurethane along the groove. Allow to dry 2 hours.

5. Mount the form in the lathe chuck or drill chuck. Verify it runs true within 0.5mm. If greater runout exists, shim with tape.

6. Cut 10 meters of #14 AWG tinned copper wire. Straighten by pulling through a cloth under light tension while rotating the spool.

7. Clamp the wire start 20mm from one end of the form (the "bottom" end). Begin winding in the direction that places the wire groove track on the operator side (facilitating later wiper installation).

8. Wind the coil at a steady tension, maintaining 2.3 mm pitch. Use a pitch gauge or cardboard template (2.3mm teeth cut with a hobby knife) against the previous turn as a guide. Wind all 41 turns without stopping.

9. Secure the wire end with a clamp. Apply one coat of clear varnish over the entire winding. Allow 1 hour cure time.

10. Apply a second coat of clear varnish. Allow overnight cure. The varnish protects the winding from moisture and fixes the turns against vibration.

11. Verify the groove track is clear of varnish (scrape gently if needed). Apply conductive silver paint (MG Chemicals 842AR or equivalent) in the groove along the full winding length. Allow to cure per silver paint instructions (typically 2–4 hours at room temperature).

12. Install end caps. The bottom end cap carries the base terminal (1/4-20 stud) and the coax tap connector (4T from bottom). The top end cap carries the whip connector (5/8-24 female thread). Thread tap wire through a small hole in the bottom endcap before cementing.

> **CAUTION:** Ensure the 4-turn tap wire exits cleanly at the bottom endcap without disturbing turns 1–4. The tap connection point is the most mechanically stressed part of the coil assembly.

### 3.3 Wiper Contact Design

The wiper contact is the key mechanical element of the screwdriver antenna. It must:
- Make reliable electrical contact with the silver-paint track as the carriage moves
- Maintain consistent contact pressure across the full travel range
- Have low contact resistance (< 0.01 Ω) to avoid adding loss
- Withstand RF current without arcing at the rated power levels

**Contact material:** Phosphor bronze leaf spring, 0.3 mm thickness, 6 mm width, 25 mm free length. This material provides the combination of spring constant, hardness, and electrical conductivity required.

**Contact tip:** The contact tip is loaded with silver-loaded epoxy (MG Chemicals 8331) applied as a 1mm-diameter dome. The tip is polished smooth after curing. This provides a hard, highly conductive sliding contact surface.

**Spring pressure:** The contact arm is designed for 0.8–1.2 N contact force against the coil track. Lower pressure increases contact resistance; higher pressure accelerates track wear. Verify contact force with a small postal scale during assembly.

**Carriage:** The wiper carriage is a 3D-printed PETG part with:
- M5 brass nut captured for engagement with the 1/4-20 threaded rod
- Dovetail slot for phosphor bronze arm retention and tension adjustment
- PTFE guide pads on two sides to reduce sliding friction

### 3.4 Verification Procedure

After construction, verify the coil inductance at 10 positions from 0 to 41 turns:

| Check Point | Turns Active | Calc L (µH) | Measured L (µH) | Tolerance |
|:-----------:|:------------:|:-----------:|:---------------:|:---------:|
| 1           | 5            | 2.89        | __________      | ±15%      |
| 2           | 10           | 9.30        | __________      | ±12%      |
| 3           | 15           | 17.8        | __________      | ±10%      |
| 4           | 20           | 27.1        | __________      | ±8%       |
| 5           | 25           | 36.6        | __________      | ±7%       |
| 6           | 28           | 42.7        | __________      | ±7%       |
| 7           | 31           | 48.3        | __________      | ±6%       |
| 8           | 35           | 56.4        | __________      | ±6%       |
| 9           | 38           | 62.8        | __________      | ±6%       |
| 10          | 41           | 72.7        | __________      | ±5%       |

> **NOTE:** Measure with an LCR meter at 100 kHz. Measurements at the operating frequency are preferable but require an antenna analyzer. If measured values fall outside tolerance, check the winding pitch — a consistent error (all values high or low) indicates the actual pitch differs from 2.3 mm/turn. Correct the Wheeler formula's l term accordingly and update the firmware band position table.

### 3.5 Extension Coil for 160m

The 160m extension coil provides approximately 210 µH of additional inductance, connected in series with the base of the main coil when 160m operation is desired.

| Parameter | Value |
|-----------|-------|
| Core | T200-2 (Type 2 powdered iron, 51mm OD × 31mm ID × 14mm) |
| Turns | 90T #18 AWG single-layer, close-wound |
| Calculated inductance | 207 µH (AL = 27 nH/N² for T200-2) |
| Measured inductance (typical) | 200–215 µH |
| SRF | > 5 MHz (measured) |
| Maximum operating power | 5W continuous (thermal limit of T200-2 at 160m current levels) |
| Mounting | SO-239 plug → extension coil → SO-239 socket (inline, 100mm pigtail) |

> **CAUTION:** Do not operate the 160m extension coil at power levels exceeding 5W continuous. The T200-2 core saturates at high flux densities and the wire gauge limits current. At 5W into the antenna system feed point and 160m efficiency of 0.11%, the antenna feed current is approximately 0.3A RMS, which is within the #18 AWG rating.

---

---

<a name="chapter-4"></a>
## CHAPTER 4: MECHANICAL ASSEMBLY

### 4.1 Threaded Rod Drive Train

The wiper carriage is driven by a 1/4-20 UNC stainless steel threaded rod, 150mm length. The 1/4-20 thread has:
- Pitch: 20 threads per inch = 1.27 mm/revolution
- Carriage travel per revolution: 1.27 mm
- Wiper travel per half-step: (1.27 mm/rev) / (2048 half-steps/rev × motor_ratio) — see Section 4.2

The rod is supported at the motor end by the motor shaft coupler and at the far end by a 3D-printed bearing block with a press-fit 1/4" nylon bushing. The bushing prevents rod deflection under wiper contact side-load while allowing free rotation.

| Parameter | Value |
|-----------|-------|
| Rod material | 316 stainless steel (corrosion resistant) |
| Rod length | 150 mm (between motor coupler and bearing block) |
| Thread | 1/4-20 UNC (unified coarse, 20 threads/inch) |
| Travel pitch | 1.270 mm per revolution |
| Total carriage travel | 94.3 mm (full coil winding length) |
| Rod rotations for full travel | 94.3 / 1.270 = 74.25 rotations |

### 4.2 Motor Specifications — 28BYJ-48

The 28BYJ-48 is a 5V unipolar stepper motor with internal planetary gear reduction, widely available and well-characterized.

| Parameter | Value |
|-----------|-------|
| Motor model | 28BYJ-48 |
| Operating voltage | 5V DC |
| Phase configuration | 4-phase unipolar |
| Coil resistance per phase | 50 Ω (nominal) |
| Current per phase (active) | 100 mA |
| Total current (2 phases active, half-step) | 200 mA |
| Internal gear ratio | 63.68395:1 (commonly rounded to 64:1) |
| Step angle (full step, no gear) | 5.625°/step = 64 full steps/rev before gear |
| Step angle (full step, after gear) | 5.625°/63.68 = 0.0883°/step |
| Full steps per output revolution | 64 × 63.68 = 4075.7 ≈ 4076 |
| Half steps per output revolution | 8153 |
| Motor steps for full coil travel (74.25 rev) | 74.25 × 8153 = 605,358 half-steps |

> **NOTE:** The commonly quoted "2048 steps/revolution" applies to a specific subset of 28BYJ-48 variants where the internal gear ratio produces exactly 2048 steps/rev in half-step mode. The more accurate figure is 8153 half-steps/revolution (output shaft). This manual uses 8153 for calculations. Verify your specific motor by counting steps per revolution and measuring actual wiper travel.

**Revised travel calculation using 8153 half-steps/rev:**
- Total half-steps for 94.3 mm travel: 94.3 mm / (1.270 mm/rev × 1 rev/8153 half-steps) = 605,200 half-steps
- This is the maximum range that would be required if the full coil were traversed

> **NOTE:** The firmware limits wiper travel to the winding length (home switch to top switch). In practice, the full 41-turn coil traversal requires approximately 605,000 half-steps. However, since most band positions are accessed in the lower 30 turns (80m to 10m), the majority of operation occurs within the first 440,000 half-steps from home.

**Position resolution:**
- Travel per half-step: 1.270 mm / 8153 = 0.1558 µm — effectively continuous
- Inductance change per half-step (at 27 turns, 40m): approximately 0.0005 µH/half-step
- This provides extremely fine tuning resolution; the bottleneck is SWR measurement SNR, not position resolution

### 4.3 Wiper Bracket and Carriage

The wiper bracket is a sliding carriage that engages the threaded rod and carries the phosphor bronze contact arm.

| Feature | Specification |
|---------|---------------|
| Carriage material | PETG (UV-stable grade) |
| M5 nut capture | Hex nut, captured with 0.1 mm interference fit |
| Rod engagement | Full-width captive nut, M5 thread to 1/4-20 adapter |
| Anti-rotation guide | Two 4mm PTFE pad bearing surfaces on coil form |
| Contact arm mount | Dovetail slot with M2 set screw tension adjust |
| Contact arm material | Phosphor bronze 0.3 mm × 6 mm × 25 mm |
| Contact force range | 0.8 N (min) to 1.5 N (max) |
| Carriage mass | 8 g (printed + hardware) |

### 4.4 Housing Assembly

The housing consists of two 3D-printed PETG components:

**Lower housing (motor_base.scad):**
- Contains motor mount (4-bolt M3 pattern, 28mm spacing)
- Houses jaw coupler in a protected well
- Has home limit switch pocket (recessed, M2 mount screws)
- Lower coil form socket (press-fit, 75mm ID)
- Cable management channels for motor wires and limit switch wires
- Base thread boss: 1/4-20 × 30mm for mast mounting

**Upper housing (coil_form.scad):**
- Coil form carrier (75mm OD coil snaps in)
- Top limit switch pocket
- Whip base connector boss: 5/8-24 thread, brass insert
- Three cable management clips for RF lead routing
- Weather seal groove for O-ring (10mm cord)

### 4.5 3D-Printed Parts List

| File Name | Part | Material | Nozzle | Layer | Infill | Perimeters | Print Time |
|-----------|------|:--------:|:------:|:-----:|:------:|:----------:|:----------:|
| motor_base.scad | Lower housing | PETG | 0.4 mm | 0.2 mm | 40% | 3 | 3.5 hr |
| coil_form.scad | Upper housing / coil carrier | PETG | 0.4 mm | 0.2 mm | 40% | 3 | 2.8 hr |
| wiper_carriage.scad | Wiper carriage | PETG | 0.4 mm | 0.15 mm | 50% | 4 | 1.2 hr |
| wiper_arm.scad | Contact arm bracket | PETG | 0.4 mm | 0.15 mm | 60% | 4 | 0.4 hr |
| bearing_block.scad | Rod end bearing block | PETG | 0.4 mm | 0.2 mm | 50% | 3 | 0.3 hr |
| mast_clamp.scad | Mast clamping plate | PETG | 0.4 mm | 0.2 mm | 50% | 4 | 0.5 hr |
| base_plate.scad | Radial connector plate | PETG | 0.4 mm | 0.2 mm | 40% | 3 | 1.0 hr |

> **NOTE:** Print PETG at 235°C nozzle, 80°C bed, 30 mm/s for structural parts, 50 mm/s for non-structural. Use at least 3 perimeters on all parts to ensure wall strength. The wiper carriage and bearing block are loaded mechanically and should use 4 perimeters and 50% infill minimum.

---

---

<a name="chapter-5"></a>
## CHAPTER 5: MOTOR CONTROL ELECTRONICS

### 5.1 28BYJ-48 Half-Step Sequence Table

Half-stepping provides smoother motion and finer position resolution than full-stepping by activating phases in the sequence shown:

| Step | IN1 | IN2 | IN3 | IN4 | Phase State |
|:----:|:---:|:---:|:---:|:---:|-------------|
| 1    |  1  |  0  |  0  |  0  | A only |
| 2    |  1  |  1  |  0  |  0  | A+B |
| 3    |  0  |  1  |  0  |  0  | B only |
| 4    |  0  |  1  |  1  |  0  | B+C |
| 5    |  0  |  0  |  1  |  0  | C only |
| 6    |  0  |  0  |  1  |  1  | C+D |
| 7    |  0  |  0  |  0  |  1  | D only |
| 8    |  1  |  0  |  0  |  1  | D+A |

For clockwise wiper travel (increasing turns from home), sequence 1→8 repeats.
For counter-clockwise travel (decreasing turns, toward home), sequence 8→1 repeats.

> **NOTE:** After reaching a target position, all phases should be de-energized (all IN = 0). Leaving phases energized while stationary draws 200 mA continuously and heats the motor and driver without benefit. The hold torque of the 28BYJ-48 through its gear reduction is sufficient to hold position with phases de-energized.

### 5.2 ULN2003 Driver Wiring

The ULN2003A is a 7-channel Darlington transistor array that interfaces the 3.3V logic outputs of the ESP32 to the 5V motor coils.

| ULN2003 Pin | Connection | Notes |
|:-----------:|------------|-------|
| IN1         | ESP32 GPIO 25 | Motor phase A |
| IN2         | ESP32 GPIO 26 | Motor phase B |
| IN3         | ESP32 GPIO 27 | Motor phase C |
| IN4         | ESP32 GPIO 14 | Motor phase D |
| IN5–IN7     | GND          | Unused inputs tied low |
| OUT1        | Motor blue wire | Phase A |
| OUT2        | Motor pink wire | Phase B |
| OUT3        | Motor yellow wire | Phase C |
| OUT4        | Motor orange wire | Phase D |
| COM (flyback) | +5V supply  | Flyback diode bus |
| GND         | System GND   | |

> **CAUTION:** The COM pin of the ULN2003 is the flyback diode cathode bus. It must be connected to +5V (the motor supply), not to +3.3V or left floating. Omitting this connection will cause voltage spikes that can damage the ULN2003 and may damage the ESP32 GPIO.

### 5.3 Limit Switch Installation and Wiring

Two limit switches protect the mechanical travel:

**Home switch (bottom):** Located at the lowest wiper position (0 turns active). This is the reference position. The firmware homes by slowly driving toward this switch and stopping when it activates, then backing off 50 half-steps to clear the switch.

**Top switch (maximum travel):** Located at the top of the coil (41 turns, maximum inductance). This switch prevents over-travel that would jam the carriage against the end bearing.

| Switch | GPIO | Pull-up | Active State | Function |
|--------|:----:|:-------:|:------------:|----------|
| Home   | 34   | 10 kΩ to 3.3V | LOW (switch connects to GND) | Reference position |
| Top    | 35   | 10 kΩ to 3.3V | LOW (switch connects to GND) | Maximum travel limit |

> **NOTE:** GPIO 34 and 35 on the ESP32 are input-only pins. They do not have internal pull-up capability. External 10 kΩ pull-up resistors to 3.3V are required.

**Homing procedure detail:**
1. Drive motor toward home at slow speed (8 ms/half-step delay)
2. Monitor GPIO 34 for LOW
3. When HOME switch activates, stop motor immediately
4. Drive motor away from home (increasing turns direction) for 50 half-steps
5. Set position counter = 0
6. Set HOME_CONFIRMED flag

### 5.4 Step Rate vs. Motor Response

The step delay (time between sequential half-steps) controls motor speed and torque:

| Step Delay (ms) | Speed (steps/sec) | Wiper Speed (mm/sec) | Torque | Use |
|:---------------:|:-----------------:|:--------------------:|:------:|-----|
| 1               | 1000              | 0.156                | Low    | Not reliable; occasional stall |
| 2               | 500               | 0.078                | Good   | Fast travel, reliable |
| 4               | 250               | 0.039                | Full   | Normal band changes |
| 8               | 125               | 0.020                | Full   | Fine tune / auto-tune |
| 20              | 50                | 0.0078               | Full   | Homing (accurate, slow) |

> **NOTE:** The minimum reliable step delay for the 28BYJ-48 is 2 ms. Below this, the rotor cannot keep pace with the phase switching and steps are lost without indication. The firmware uses 2 ms for fast band-to-band travel, 4 ms for normal operation, and 20 ms for homing.

**Time estimates for full coil traverse (home to 41 turns, 605,000 half-steps):**
- At 2 ms/step: 1210 seconds (20 minutes) — unacceptable for operation
- At 4 ms/step: 2420 seconds — unacceptable

> **NOTE:** Full coil traversal is never required in operation. The maximum travel needed is from 10m position (~3 turns, step ~18,000) to 80m position (~39 turns, step ~236,000) — approximately 218,000 steps. At 2 ms/step, this takes 436 seconds. In practice, band changes between adjacent bands (e.g., 40m to 20m) require only 54,000 steps — approximately 108 seconds at 2 ms/step. The firmware uses 2 ms/step for fast travel to within 500 steps of target, then switches to 8 ms/step for the final approach.

### 5.5 Position Counting

Position is tracked as an absolute half-step count from the home position (0 = wiper at coil base, all inductance bypassed):

```
position = 0                          (home, 0 turns, L ≈ 0 µH)
position = turns × steps_per_turn
turns = position / steps_per_turn

steps_per_turn = 8153 half-steps/motor_rev × (1.270 mm/rev)^-1 × (2.3 mm/turn)
               = 8153 × (2.3 / 1.270)
               = 8153 × 1.811
               = 14,765 half-steps per turn
```

Wait — this calculation conflates motor and mechanical dimensions. Correcting:

```
Motor shaft turns per coil form revolution: 1 (direct coupling via jaw coupler)
Thread pitch: 1.270 mm/revolution
Winding pitch: 2.3 mm/turn (of coil wire)
Wiper travel per coil turn: 2.3 mm
Wiper travel per rod revolution: 1.270 mm
Coil turns per rod revolution: 1.270/2.3 = 0.552 coil turns/rod revolution
Rod revolutions per coil turn: 2.3/1.270 = 1.811 rod revolutions/coil turn
Half-steps per coil turn: 8153 × 1.811 = 14,765 half-steps/coil turn
```

So the relationship is:
```
coil_turns = step_position / 14765
L (µH) = 14.0625 × coil_turns² / (85.73 + 5.842 × coil_turns)
```

The firmware maintains `step_position` as a 32-bit integer and computes inductance on demand.

### 5.6 Auto-Tune Algorithm

The auto-tune algorithm finds the minimum-SWR wiper position by gradient descent with bisecting step size:

```
FUNCTION auto_tune():
    step_size = 1000   // coarse initial step
    direction = +1     // start by increasing inductance (down in frequency)
    last_swr = read_swr()
    convergence_count = 0

    WHILE step_size >= 4 AND swr > SWR_GOOD_THRESHOLD:
        move_motor(direction × step_size)
        new_swr = read_swr()

        IF new_swr < last_swr:
            last_swr = new_swr
            convergence_count = 0
        ELSE:
            direction = -direction          // reverse direction
            step_size = step_size / 2      // halve step size
            convergence_count += 1

        IF convergence_count > 3:
            BREAK                           // converged

    RETURN current_position, last_swr
```

**Convergence criteria:**
- `SWR_GOOD_THRESHOLD` = 1.5 (typical; configurable in firmware)
- Minimum step size = 4 half-steps (≈ 2.7 µm wiper travel — below practical significance)
- Maximum iterations = 50 (prevents infinite loop on pathological SWR curves)

**Typical convergence time:** 15–30 iterations for a cold start far from resonance; 3–8 iterations for a small correction from a previously stored band position.

### 5.7 Band Change Procedure

The firmware executes the following sequence on a band change command:

1. Look up stored step position for target band from NVS (non-volatile storage)
2. Compute delta from current position
3. Move motor to stored position at fast speed (2 ms/step), switching to slow speed (8 ms/step) within 500 steps of target
4. Settle for 200 ms (allow SWR reading to stabilize)
5. Read SWR
6. If SWR ≤ 2.5: store confirmed, update "last good position" log, signal READY
7. If SWR > 2.5: run auto_tune() algorithm
8. After auto_tune() completes: update NVS stored position for this band
9. Signal READY; transmit final SWR and position to CYD display

---

---

<a name="chapter-6"></a>
## CHAPTER 6: DIRECTIONAL COUPLER THEORY AND SWR MEASUREMENT

### 6.1 Traveling Wave Decomposition

Any signal on a transmission line can be decomposed into a forward-traveling wave (V⁺, traveling from source to load) and a reflected wave (V⁻, traveling from load to source). At any point on the line:

```
V_total = V⁺ + V⁻
I_total = (V⁺ - V⁻) / Z₀

Where Z₀ = characteristic impedance of the transmission line (50 Ω)
```

The voltage reflection coefficient at the load is:

```
Γ = V⁻/V⁺ = (ZL - Z₀) / (ZL + Z₀)

Where ZL = load impedance (the antenna system impedance)
```

The reflection coefficient magnitude |Γ| (also written ρ) is related to return loss and SWR:

```
Return Loss (RL) = -20 × log₁₀(|Γ|)     [dB, positive value]
SWR = (1 + |Γ|) / (1 - |Γ|)
|Γ| = (SWR - 1) / (SWR + 1)
```

A directional coupler separates V⁺ and V⁻ by sampling the voltage and current on the line and forming their sum and difference.

### 6.2 Bruene Coupler Schematic and Design

The Bruene (W. W. Bruene, 1959, QST) directional coupler is the standard for HF power measurement. It uses a current transformer (CT) wound on a toroid core to sample line current, and a voltage divider to sample line voltage, then combines these to extract forward and reflected power samples.

**Core selection:** FT-37-43 (Ferrite, Type 43 material, 9.5 mm OD)
- Type 43 ferrite is the standard for broadband RF transformers 1–30 MHz
- AL = 420 nH/N² for FT-37-43
- 10-turn secondary: L = 420 × 100 = 42,000 nH = 42 µH — adequate for 1.8 MHz minimum

**Current transformer winding:** 10T #28 AWG through the core, primary = 1T (the main coax line passes through the toroid hole once).

**Coupling factor:** -20 × log₁₀(1/10) = −20 dB (10:1 turns ratio)

**Termination:** A 51 Ω resistor terminates the secondary end of the transformer. The voltage developed across this resistor is proportional to the line current.

**Voltage sampling:** A resistive divider (1000 Ω + 51 Ω) samples the line voltage. The 51 Ω output impedance of the voltage divider is matched to the transformer output.

**Detector:** The forward and reflected signals are detected by either:
- Schottky diodes (1N5711): appropriate for < 1W levels; inexpensive
- AD8307 log-amp IC: preferred for its 90 dB dynamic range and calibrated output

**Coupling to AD8307:** Each output (forward, reflected) feeds one AD8307 RF input through a 10 dB attenuator pad (matched to the AD8307's 200 Ω input impedance).

### 6.3 AD8307 Log Amplifier Details

The AD8307 is a 500 MHz log amplifier with 90 dB dynamic range, providing a DC output voltage proportional to the input power in dBm.

**Transfer function:**
```
V_out (mV) = 25 × (P_in_dBm + 84)

Equivalently:
P_in_dBm = (V_out_mV / 25) - 84
```

**Specifications relevant to SWR measurement:**

| Parameter | Value |
|-----------|-------|
| Input impedance | 200 Ω (differential) |
| Dynamic range | −75 dBm to +17 dBm |
| Frequency range | DC to 500 MHz |
| Slope | 25 mV/dB |
| Intercept | −84 dBm |
| Output range | 0.25 V (−75 dBm) to 2.5 V (+17 dBm) |
| Supply voltage | 2.7 V to 5.5 V (use 3.3 V for ESP32 ADC compatibility) |
| Supply current | 7.5 mA |
| Temperature coefficient | 0.5 dB over 0–70°C |

**Coupling factor correction:** The AD8307 measures the power in the coupler's secondary port, not the power in the main transmission line. The relationship is:

```
P_line_dBm = P_AD8307_dBm + coupling_factor_dB + attenuator_dB
           = P_AD8307_dBm + 20 + 10     [for 10:1 CT and 10 dB pad]
           = P_AD8307_dBm + 30
```

**ADC reading to power conversion (ESP32, 12-bit ADC, 3.3V reference):**

```
ADC counts → voltage: V_out = ADC_counts × (3.3 / 4095) V
Voltage → dBm (AD8307): P_dBm = (V_out × 1000 / 25) - 84
Line power: P_line_dBm = P_dBm + 30
Linear power: P_watts = 10^((P_line_dBm - 30) / 10) / 1000
```

### 6.4 SWR Calculation

From the forward (Pfwd) and reflected (Prefl) power readings:

```
Reflection coefficient magnitude:
ρ = sqrt(Prefl / Pfwd)

Or equivalently from dBm readings:
ρ_dB = (Pfwd_dBm - Prefl_dBm) / 2     (ρ in dB)
ρ = 10^(-ρ_dB / 20)

SWR = (1 + ρ) / (1 - ρ)
Return Loss = -20 × log₁₀(ρ)     [dB]
```

**Firmware implementation:**
```cpp
float compute_swr(int adc_fwd, int adc_refl) {
    float v_fwd = adc_fwd * 3.3f / 4095.0f;
    float v_refl = adc_refl * 3.3f / 4095.0f;
    float p_fwd_dBm = (v_fwd * 1000.0f / 25.0f) - 84.0f;
    float p_refl_dBm = (v_refl * 1000.0f / 25.0f) - 84.0f;
    float rho_dB = (p_fwd_dBm - p_refl_dBm) / 2.0f;
    float rho = pow(10.0f, -rho_dB / 20.0f);
    if (rho >= 1.0f) return 99.0f;  // open or short
    return (1.0f + rho) / (1.0f - rho);
}
```

### 6.5 Interpreting SWR in Context of Efficiency

> **CAUTION:** The relationship between SWR reading and antenna performance is not straightforward for electrically short antennas. Read this section carefully to avoid misinterpreting instrument readings.

**High-efficiency bands (10m–17m):** On these bands, the antenna's radiation resistance is significant compared to loss resistances. The SWR reading directly reflects impedance match quality. A reading of SWR = 1.2 means the antenna is well-matched and efficient. Auto-tune to SWR ≤ 1.5 is a reliable indicator of good operation.

**Low-efficiency bands (40m–80m):** On these bands, the radiation resistance (0.09–0.32 Ω) is much smaller than the coil loss resistance (3.5–7.7 Ω). The SWR reading reflects matching of the combined impedance to 50 Ω, not just radiation efficiency. An apparent SWR of 1.8:1 on 40m means the combined feed resistance is reasonably matched to 50 Ω — but 91.5% of that matched power is being dissipated in coil resistance, not radiated.

**The critical misinterpretation:** A poorly tuned antenna (significantly off resonance) may show a higher SWR than the "matched" condition. This is correct behavior — the auto-tune reduces SWR by improving the match. However, a "good" SWR reading on 80m does NOT mean efficient radiation. It means the transmitter sees 50 Ω; most of what it's "seeing" is coil resistance.

**Practical implication:** Always use the maximum allowed power on 80m and below, within the coil thermal limits. Low SWR on these bands means the transmitted power is accepted efficiently — but only a small fraction is radiated.

### 6.6 SWR Quick Reference Table

| SWR | ρ (|Γ|) | Return Loss (dB) | % Power Reflected | % Power into Load | Mismatch Loss (dB) |
|:---:|:-------:|:----------------:|:-----------------:|:-----------------:|:------------------:|
| 1.0 | 0.000   | ∞                | 0.0%              | 100.0%            | 0.00               |
| 1.1 | 0.048   | 26.4             | 0.23%             | 99.8%             | 0.01               |
| 1.2 | 0.091   | 20.8             | 0.83%             | 99.2%             | 0.04               |
| 1.5 | 0.200   | 14.0             | 4.0%              | 96.0%             | 0.18               |
| 1.8 | 0.286   | 10.9             | 8.2%              | 91.8%             | 0.37               |
| 2.0 | 0.333   | 9.55             | 11.1%             | 88.9%             | 0.51               |
| 2.5 | 0.429   | 7.36             | 18.4%             | 81.6%             | 0.88               |
| 3.0 | 0.500   | 6.02             | 25.0%             | 75.0%             | 1.25               |
| 4.0 | 0.600   | 4.44             | 36.0%             | 64.0%             | 1.94               |
| 5.0 | 0.667   | 3.52             | 44.4%             | 55.6%             | 2.55               |
| 7.0 | 0.750   | 2.50             | 56.3%             | 43.8%             | 3.59               |
| 10.0| 0.818   | 1.74             | 66.9%             | 33.1%             | 4.81               |

---

---

<a name="chapter-7"></a>
## CHAPTER 7: ROLL-UP WIRE VERSION

### 7.1 Design Concept

The roll-up wire version substitutes a 2.4m length of flexible PVC-insulated wire for the rigid stainless steel whip. This variant is optimized for maximum portability — the entire antenna folds into a small pouch. The trade-offs are:
- No motorized tuning (fixed loading per band or manual swap of coil modules)
- Reduced efficiency due to non-vertical element posture (wire must be supported)
- Inclusion of resonant traps enables operation on 6m and 2m without a loading coil

Two inline LC traps are inserted at calculated positions along the wire to create resonant sections for VHF bands while allowing HF operation through the traps.

### 7.2 Trap Construction

A resonant trap consists of a parallel LC circuit that presents a very high impedance at its resonant frequency. Above the trap resonant frequency, the trap "isolates" the wire beyond it from the antenna, shortening the effective electrical length.

**Trap resonance formula:**
```
f_trap = 1 / (2π × sqrt(L × C))
```

**2m trap (144 MHz):** Positioned at 0.488m from the antenna tip. This makes the tip section a quarter-wave at 144 MHz (λ/4 at 144 MHz = 0.521 m; 0.488m accounts for velocity factor and wire capacitance).

| 2m Trap Parameter | Value |
|-------------------|-------|
| Resonant frequency | 144.0 MHz |
| Position from tip | 0.488 m |
| Inductance | 0.148 µH (6T #20AWG, 8mm form) |
| Capacitance | 8.2 pF (silver mica, 500V) |
| L (computed) | 0.148 µH |
| C (computed) | 1/(4π²×144²×10¹²×0.148×10⁻⁶) = 8.25 pF |
| Trap Q | ~120 |

**6m trap (50 MHz):** Positioned at 1.40m from the antenna tip. At 50 MHz, the section from the tip to the 6m trap resonates as a half-wave dipole element (λ/2 at 50 MHz = 3.0 m; the element above the trap is electrically extended by the trap reactance).

| 6m Trap Parameter | Value |
|-------------------|-------|
| Resonant frequency | 50.2 MHz |
| Position from tip | 1.40 m |
| Inductance | 1.22 µH (18T #22AWG, 12mm form) |
| Capacitance | 8.2 pF (silver mica, 500V) |
| L (computed) | 1.22 µH |
| C (computed) | 1/(4π²×50.2²×10¹²×1.22×10⁻⁶) = 8.27 pF |
| Trap Q | ~90 |

**HF operation with traps:** At HF frequencies (< 30 MHz), both traps present low reactance (below their resonant frequencies) and the wire passes through them with only minor loading. The HF effective length is approximately the full 2.4 m with a small inductive correction for the trap coil reactances.

### 7.3 Loading Coil Options for Roll-Up Version

Instead of the motorized coil system, the roll-up version uses plug-in coil modules — one per band or one per two adjacent bands. These are wound on T106-2 toroids for HF:

| Band | L Required (µH) | Core | Turns | Wire |
|------|:---------------:|------|:-----:|------|
| 80m  | 81.2            | T106-2 × 2 stacked | 52T | #18 AWG |
| 60m  | 39.7            | T106-2 | 45T | #18 AWG |
| 40m  | 21.5            | T106-2 | 33T | #18 AWG |
| 30m  | 10.3            | T80-2  | 28T | #20 AWG |
| 20m  | 4.76            | T80-2  | 19T | #20 AWG |
| 17m  | 2.48            | T50-2  | 18T | #22 AWG |
| 15m  | 1.52            | T50-2  | 14T | #22 AWG |
| 12m  | 0.753           | T50-2  | 10T | #22 AWG |
| 10m  | 0.288           | T37-2  | 7T  | #24 AWG |

### 7.4 Comparison Table

| Feature | Motorized (SC-001) | Roll-Up Wire |
|---------|:------------------:|:------------:|
| Mass | 680 g | 120 g |
| Packed size | 200 × 120 mm | 180 × 60 mm pouch |
| Tuning speed | 15–30 sec (auto) | 2–5 min (manual swap) |
| HF bands covered | 160m–10m | 80m–10m (coil set) |
| VHF coverage | With separate element | 6m and 2m (built-in traps) |
| Efficiency | Per Chapter 2 table | ~5% lower than rigid (element posture) |
| SWR after tune | < 1.5 typical | < 2.5 typical (fixed coil approximation) |
| Power rating | 100W (10m–17m), 50W (40m) | 25W (all bands, trap heating) |
| Setup complexity | Moderate (motor system) | Low (plug coil, attach wire) |
| Durability | Good (PETG housing) | High (no moving parts) |
| Field repair | Firmware or mechanical | Wind replacement coil |
| Best use case | Repeated operation, any band | Ultralight pack, specific bands |

### 7.5 Deployment Procedure — Roll-Up Version

1. Unroll the 2.4m wire element. Inspect traps for physical damage (cracked forms, broken solder joints).

2. Select loading coil module for primary operating band. Connect coil module to base connector using the SMA pigtail. Confirm coil is fully seated.

3. Support the wire element vertically using a telescoping pole, tree branch, or throw-line. The element does not need to be precisely vertical but should be within 30 degrees of vertical for best performance.

4. Lay out radials (minimum 2, recommended 4) on the ground from the antenna base. Connect to the base radial connector block.

5. Connect coax feed line to the base. Keep the feed coax perpendicular to the element for at least 1 meter to minimize common-mode current.

6. Transmit a brief carrier (1 second) while observing SWR on transceiver meter. If SWR > 3:1, swap to the coil module for the closest adjacent band and re-test.

7. For fine adjustment: slide the antenna wire's free end through a plastic cable tie that can be moved along the element to adjust electrical length by approximately 5–10 cm. This provides a crude tuning trim.

8. Log operating frequency, coil used, SWR, and notes. This log will identify which coil module is closest for each band at the operating location.

---

---

<a name="chapter-8"></a>
## CHAPTER 8: NEC MODELING

### 8.1 NEC Model Files

The following NEC2 model files are provided with this system:

| File Name | Band | Key Parameters |
|-----------|------|----------------|
| TM-SC-NEC-001.nec | 160m | f=1.85 MHz, L=210µH (ext coil), Rcoil=19.6Ω |
| TM-SC-NEC-002.nec | 80m  | f=3.75 MHz, L=72.7µH, Rcoil=7.65Ω |
| TM-SC-NEC-003.nec | 60m  | f=5.33 MHz, L=39.7µH, Rcoil=5.02Ω |
| TM-SC-NEC-004.nec | 40m  | f=7.15 MHz, L=21.5µH, Rcoil=3.46Ω |
| TM-SC-NEC-005.nec | 30m  | f=10.125 MHz, L=10.3µH, Rcoil=2.15Ω |
| TM-SC-NEC-006.nec | 20m  | f=14.175 MHz, L=4.76µH, Rcoil=1.24Ω |
| TM-SC-NEC-007.nec | 17m  | f=18.118 MHz, L=2.48µH, Rcoil=0.772Ω |
| TM-SC-NEC-008.nec | 15m  | f=21.225 MHz, L=1.52µH, Rcoil=0.532Ω |
| TM-SC-NEC-009.nec | 12m  | f=24.94 MHz, L=0.753µH, Rcoil=0.315Ω |
| TM-SC-NEC-010.nec | 10m  | f=28.5 MHz, L=0.288µH, Rcoil=0.143Ω |
| TM-SC-NEC-011.nec | 6m   | f=50.2 MHz, no loading coil, direct feed |
| TM-SC-NEC-012.nec | 2m   | f=144 MHz, 0.488m element, no coil |
| TM-SC-NEC-013.nec | 40m sweep | Frequency sweep 6.8–7.5 MHz, 10 steps |
| TM-SC-NEC-014.nec | 20m sweep | Frequency sweep 13.5–15.0 MHz, 10 steps |

### 8.2 Loading Coil Representation in NEC

NEC2 does not directly model an inductive loading coil with distributed parameters. The coil is represented using the LD (loading) card:

```
LD  5  seg#  0  0  R_ohm  L_henry  C_farad
```

**LD card parameters for 40m (example):**
```
LD  5  1  0  0  3.46  2.15E-5  0
```

This places a series R+jωL impedance on segment 1 (the base segment) of the antenna wire:
- R = 3.46 Ω (Rcoil at 40m)
- L = 21.5 µH = 2.15 × 10⁻⁵ H
- C = 0 (no series capacitance term)

**Limitation of NEC LD card:** The LD card models the loading as a lumped element on a single segment. In reality, the loading coil is a distributed element with self-capacitance, inter-winding capacitance, and a distributed current distribution along its length. For the frequencies covered by this antenna (below 30 MHz, where the coil is well below self-resonance), the lumped model is accurate within ±15% for impedance and ±0.5 dB for gain.

**Parallel RLC model (more accurate for high-frequency bands):** On 12m and 10m where the coil length is a significant fraction of the wavelength, a parallel LD card representation may be more accurate:

```
LD  4  1  0  0  R_parallel  L_parallel  C_parallel
```

Where the parallel component values are derived from the series values via:
```
Rs = (Xs/Q)            C_parallel = Q²/(Q²+1) × 1/(ω×Xs)    (for high Q)
R_parallel = Q² × Rs   L_parallel ≈ L_series
```

### 8.3 Key NEC Results Summary

The following table summarizes the NEC2 output for each band with the loading coil modeled as described in Section 8.2, over a perfect ground plane, with 4 radials modeled explicitly:

| Band | f (MHz) | Feed Z (Ω) | SWR to 50Ω | Gain (dBi) | Takeoff Angle (deg) | Notes |
|------|:-------:|:----------:|:----------:|:----------:|:-------------------:|-------|
| 160m | 1.85    | 0.08+j0    | 625:1 → needs ATU | −25.8 | 87° | Near-NVIS; ATU needed |
| 80m  | 3.75    | 0.35+j0    | 143:1 → ATU | −15.4 | 75° | NVIS angle |
| 40m  | 7.15    | 3.78+j0    | 13.2:1 → ATU | −7.2 | 44° | Useful low angle |
| 30m  | 10.125  | 8.4+j0     | 6.0:1 → ATU | −3.5 | 32° | 1500 km range |
| 20m  | 14.175  | 24.0+j0    | 2.1:1 | −0.6 | 24° | Good DX |
| 17m  | 18.118  | 35.3+j0    | 1.4:1 | +0.7 | 20° | Excellent |
| 15m  | 21.225  | 42.7+j0    | 1.17:1 | +1.3 | 17° | Excellent |
| 12m  | 24.94   | 47.8+j0    | 1.05:1 | +1.7 | 15° | Near-perfect |
| 10m  | 28.5    | 51.2+j0    | 1.02:1 | +2.1 | 13° | Quarter-wave equivalent |

> **NOTE:** Feed impedances shown are at the antenna base (at the coil). The tap transformer transforms these to approximately 50 Ω as discussed in Chapter 2.5. The gain figures are negative on the low bands due to low efficiency — the antenna pattern is close to a short monopole pattern but scaled down by the efficiency factor.

### 8.4 Running NEC2 Models

**Command-line execution:**
```bash
# Install nec2c
sudo apt install nec2c

# Run a model
nec2c -i TM-SC-NEC-004.nec -o TM-SC-NEC-004.out

# View key results (impedance and gain)
grep "ANTENNA INPUT PARAMETERS" TM-SC-NEC-004.out -A 5
grep "MAXIMUM GAIN" TM-SC-NEC-004.out
```

**GUI options:**
- **xnec2c:** GTK-based NEC2 GUI for Linux. Install: `sudo apt install xnec2c`. Provides real-time Smith chart, SWR plot, and 3D pattern display.
- **4nec2:** Windows-based; runs under Wine on Linux. More features than xnec2c for model editing.

**Typical output excerpt (TM-SC-NEC-004.nec, 40m):**
```
 ANTENNA INPUT PARAMETERS

  IMPEDANCE =  3.78 + J  0.00   OHMS
  ADMITTANCE =  0.265  + J 0.000   MHOS
  POWER =   9.47E-02  WATTS

 MAXIMUM GAIN = -7.23 DB   AT  44 DEGREES ELEVATION
```

### 8.5 Validation

NEC2 predictions should be compared to measured SWR and resonant frequency:

| Validation Check | Method | Acceptable Error |
|-----------------|--------|:----------------:|
| Resonant frequency | Sweep with NanoVNA; find SWR minimum | ±5% |
| Feed impedance magnitude | NanoVNA Smith chart at resonance | ±20% |
| Gain (efficiency) | Relative comparison to reference antenna | ±3 dB |
| Takeoff angle | WSPR-based propagation verification | ±15° |

The primary cause of discrepancy between NEC model and measurement is the ground system — NEC's perfect ground plane produces higher efficiency and different impedance than real portable ground systems. If measured SWR is higher than NEC prediction, the ground system resistance is the most likely cause.

---

---

<a name="chapter-9"></a>
## CHAPTER 9: FIRMWARE REFERENCE

### 9.1 ESP32 GPIO Assignments

| GPIO | Direction | Function | Notes |
|:----:|:---------:|----------|-------|
| 25   | Output    | Motor IN1 (Phase A) | Via ULN2003 |
| 26   | Output    | Motor IN2 (Phase B) | Via ULN2003 |
| 27   | Output    | Motor IN3 (Phase C) | Via ULN2003 |
| 14   | Output    | Motor IN4 (Phase D) | Via ULN2003 |
| 34   | Input     | Home limit switch | Input-only; ext. 10kΩ pullup |
| 35   | Input     | Top limit switch | Input-only; ext. 10kΩ pullup |
| 36   | ADC1_CH0  | AD8307 Forward power | 12-bit ADC, 0–3.3V |
| 39   | ADC1_CH3  | AD8307 Reflected power | 12-bit ADC, 0–3.3V |
| 2    | Output    | Status LED (green) | 220Ω series resistor |
| 4    | Output    | Tune active LED (amber) | 220Ω series resistor |
| 5    | Output    | SWR alarm LED (red) | 220Ω series resistor |
| 18   | SPI CLK   | SPI bus clock | Shared CYD if used |
| 19   | SPI MISO  | SPI data in | |
| 23   | SPI MOSI  | SPI data out | |
| 21   | I2C SDA   | I2C data (reserved) | Future sensor expansion |
| 22   | I2C SCL   | I2C clock (reserved) | Future sensor expansion |
| TX0  | UART TX   | Serial monitor / debug | 115200 baud |
| RX0  | UART RX   | Serial input | |

### 9.2 Band Table (NVS Stored Positions)

The following default positions are loaded on first boot (NVS empty). They are overwritten by auto-tune as each band is used.

| Band | Freq (MHz) | Default Step | Active Turns | L (µH) | Notes |
|------|:----------:|:------------:|:------------:|:------:|-------|
| 160m | 1.85       | 607000       | 41+ext       | 283    | Extension coil required |
| 80m  | 3.75       | 575000       | 39.0         | 68.9   | Near full coil |
| 60m  | 5.33       | 502000       | 34.0         | 52.1   | |
| 40m  | 7.15       | 398000       | 26.9         | 37.7   | |
| 30m  | 10.125     | 324000       | 21.9         | 25.3   | |
| 20m  | 14.175     | 251000       | 17.0         | 15.3   | |
| 17m  | 18.118     | 192000       | 13.0         | 9.10   | |
| 15m  | 21.225     | 148000       | 10.0         | 5.56   | |
| 12m  | 24.94      | 103000       | 7.0          | 2.77   | |
| 10m  | 28.5       | 44000        | 3.0          | 0.526  | |
| 6m   | 50.2       | 0            | 0            | 0      | Coil removed; direct element |
| 2m   | 144.0      | 0            | 0            | 0      | Coil removed; 0.488m element |
| 1.25m| 222.0      | 0            | 0            | 0      | Coil removed; VHF stub |
| 70cm | 432.0      | 0            | 0            | 0      | Coil removed; direct collinear |
| 23cm | 1296.0     | 0            | 0            | 0      | Coil removed; microwave element |

### 9.3 REST API Reference

The ESP32 hosts an HTTP server on port 80 when in WiFi Access Point mode (SSID: `SC-001-TUNER`, password: `screwdriver`).

| Method | Endpoint | Description | Example Response |
|--------|----------|-------------|-----------------|
| GET | `/` | Home page with status dashboard | HTML page |
| GET | `/api/status` | Current system status JSON | `{"band":"40m","pos":398000,"turns":26.9,"swr":1.42,"fwd_w":45.2,"state":"READY"}` |
| GET | `/api/band` | Current band selection | `{"band":"40m","freq":7.15}` |
| POST | `/api/band` | Set band | Body: `band=40m` → `{"ok":true,"band":"40m"}` |
| GET | `/api/position` | Current step position | `{"position":398000,"turns":26.9,"L_uH":37.7}` |
| POST | `/api/position` | Set absolute position | Body: `pos=398000` → `{"ok":true,"position":398000}` |
| POST | `/api/move` | Relative move (steps) | Body: `steps=500` → `{"ok":true,"new_pos":398500}` |
| POST | `/api/home` | Execute home sequence | `{"ok":true,"message":"homing"}` |
| POST | `/api/tune` | Start auto-tune | `{"ok":true,"message":"tuning started"}` |
| GET | `/api/swr` | Current SWR reading | `{"swr":1.42,"fwd_dBm":16.3,"refl_dBm":4.7,"rho":0.170}` |
| GET | `/api/config` | Read configuration | JSON of all config parameters |
| POST | `/api/config` | Update configuration | Body: key=value pairs |
| GET | `/api/bands` | All band positions | JSON array of all 15 band records |
| POST | `/api/bands/reset` | Reset all bands to default | `{"ok":true,"message":"bands reset"}` |
| GET | `/api/log` | Last 50 tune events | JSON array of log entries |

### 9.4 Bluetooth Packet Format

The ESP32 advertises as a Bluetooth Classic SPP (Serial Port Profile) device named `SC-001`. The CYD connects to this device and exchanges packets at 9600 baud.

**ESP32 → CYD status packet (transmitted every 500 ms):**
```
SC|BAND:40m|POS:398000|TURNS:26.9|L:37.7|SWR:1.42|FWD:45.2|STATE:READY|CS:7A\n
```

| Field | Description | Example |
|-------|-------------|---------|
| `SC` | Packet identifier | `SC` |
| `BAND` | Current band | `40m` |
| `POS` | Absolute step position | `398000` |
| `TURNS` | Computed active turns | `26.9` |
| `L` | Computed inductance (µH) | `37.7` |
| `SWR` | Current SWR reading | `1.42` |
| `FWD` | Forward power (Watts) | `45.2` |
| `STATE` | System state | `READY`, `HOMING`, `TUNING`, `MOVING`, `FAULT` |
| `CS` | Checksum (XOR of all chars, hex) | `7A` |

**CYD → ESP32 command packet:**
```
CMD|BAND:20m\n
CMD|TUNE\n
CMD|HOME\n
CMD|MOVE:+500\n
CMD|MOVE:-200\n
```

### 9.5 Auto-Tune Algorithm Detail

Refer to Section 5.6 for the high-level algorithm. Additional implementation details:

**SWR averaging:** Each SWR reading is the median of 5 consecutive ADC samples taken 10 ms apart. This rejects single-sample noise spikes.

**Transmit gating:** The auto-tune algorithm requires the transmitter to be active (carrier or CW tone). The firmware monitors forward power; if Pfwd < 0.1W, auto-tune is suspended and a `NO_TX_POWER` warning is issued.

**Thermal compensation:** After a long transmit session (> 5 minutes at > 50W on 40m–80m), the coil form temperature rises and the wire expansion slightly changes the effective inductance. The firmware detects a drift of more than 5 steps in the stored position after thermal settling and updates the NVS value automatically.

**Gradient descent convergence detail:**

```
Iteration 1: step_size=1000, move +1000, SWR improves → continue same direction
Iteration 2: step_size=1000, move +1000, SWR worsens → reverse, halve step_size
Iteration 3: step_size=500, move -500, SWR improves → continue
Iteration 4: step_size=500, move -500, SWR worsens → reverse, halve
...
Iteration N: step_size=4, move ±4, converged
```

Typical iterations to converge from within ±2000 steps of optimal: 12–18 iterations.

### 9.6 NVS Stored Values

Non-volatile storage (NVS) in the ESP32 flash preserves settings across power cycles:

| Key | Type | Description | Default |
|-----|------|-------------|---------|
| `band_160m_pos` | int32 | Step position for 160m | 607000 |
| `band_80m_pos` | int32 | Step position for 80m | 575000 |
| `band_60m_pos` | int32 | Step position for 60m | 502000 |
| `band_40m_pos` | int32 | Step position for 40m | 398000 |
| `band_30m_pos` | int32 | Step position for 30m | 324000 |
| `band_20m_pos` | int32 | Step position for 20m | 251000 |
| `band_17m_pos` | int32 | Step position for 17m | 192000 |
| `band_15m_pos` | int32 | Step position for 15m | 148000 |
| `band_12m_pos` | int32 | Step position for 12m | 103000 |
| `band_10m_pos` | int32 | Step position for 10m | 44000 |
| `cal_fwd_offset` | float | AD8307 fwd zero offset (dBm) | 0.0 |
| `cal_refl_offset` | float | AD8307 refl zero offset (dBm) | 0.0 |
| `cal_coup_factor` | float | Coupler correction factor (dB) | 30.0 |
| `swr_good_thresh` | float | Auto-tune stop threshold | 1.50 |
| `step_fast_delay` | int | Fast step delay (ms) | 2 |
| `step_slow_delay` | int | Slow step delay (ms) | 8 |
| `bt_enabled` | bool | Bluetooth enable | true |
| `wifi_enabled` | bool | WiFi AP enable | true |
| `home_confirmed` | bool | Has homed since power-on | false |

### 9.7 CYD Display Pages

The CYD display cycles through 5 operational pages, selectable by swiping or tapping the band indicator:

| Page | Name | Content |
|:----:|------|---------|
| 1 | Main Status | Band name (large), SWR (large), forward power bar, position/turns, STATE indicator |
| 2 | Band Select | Grid of 10 HF band buttons + VHF; tap to change band; current band highlighted |
| 3 | SWR Meter | Analog-style SWR meter graphic, forward power W, reflected power W, return loss dB |
| 4 | Motor Control | Manual jog buttons (+1, +10, +100, +1000 steps; same in reverse), Home button, Tune button |
| 5 | System Info | Current step position, computed turns, computed L_uH, WiFi IP, BT status, firmware version |

---

---

<a name="chapter-10"></a>
## CHAPTER 10: CALIBRATION

### 10.1 Motor Calibration — Steps Per Turn Verification

**Purpose:** Verify that the firmware's steps_per_turn constant (14,765) matches the actual mechanical system.

**Required equipment:** Dial indicator or ruler accurate to 0.1 mm; known reference measurement.

**Procedure:**

1. Home the system (REST API: POST /api/home or CYD Home button).

2. Record the starting position (should be 0 after homing).

3. Command the motor to position 14,765 (exactly one coil turn of wiper travel).

4. Measure the physical displacement of the wiper carriage. Expected: 2.30 mm.

5. If measured displacement differs by more than 0.2 mm from 2.30 mm, compute correction:
   ```
   corrected_steps_per_turn = 14765 × (2.30 / measured_mm)
   ```

6. Update `STEPS_PER_TURN` constant in firmware `config.h` and reflash.

7. Verify correction by repeating procedure. Accept when within ±0.1 mm.

### 10.2 AD8307 Zero-Offset Calibration

**Purpose:** Correct for the voltage offset at the AD8307 output when no RF is present. This offset causes a false SWR reading when transmitter power is very low.

**Required equipment:** Multimeter; no RF source needed.

**Procedure:**

1. Ensure transmitter is off and no RF is present on the feed line.

2. Read ADC values via REST API: GET /api/swr. Note `fwd_dBm` and `refl_dBm` values.

3. Ideal no-signal reading: both channels should read ≤ −70 dBm.

4. If either channel reads > −50 dBm with no RF, there is an offset:
   ```
   cal_fwd_offset = expected_no_signal_dBm - measured_fwd_dBm
   ```

5. Apply offset via REST API: POST /api/config with `cal_fwd_offset=<value>` and `cal_refl_offset=<value>`.

6. The firmware adds this offset to all subsequent dBm readings.

### 10.3 Coupler Calibration — Coupling Factor Verification

**Purpose:** Verify the actual coupling factor of the directional coupler matches the assumed 30 dB (20 dB transformer + 10 dB attenuator) used in power calculations.

**Required equipment:** HF signal source (50 Ω output), known power level; or NanoVNA in power mode; known 50 Ω dummy load.

**Procedure:**

1. Connect dummy load (50 Ω, known calibration standard) to the antenna port.

2. Apply known RF power (e.g., 10W = +40 dBm) at the transmitter port.

3. Read the forward power via GET /api/swr. Note `fwd_dBm`.

4. Expected AD8307 input power: +40 dBm − 30 dB = +10 dBm (AD8307 input)

5. Expected AD8307 output voltage: 25 × (10 + 84) = 2350 mV

6. If measured power at the API differs from 10W, compute actual coupling factor:
   ```
   actual_coupling = applied_power_dBm - measured_fwd_dBm
   ```

7. Update `cal_coup_factor` NVS value via REST API config endpoint.

8. Verify: reflected power with 50 Ω load should read < −40 dBm (return loss > 40 dB).

### 10.4 Frequency Calibration — Resonant Frequency Verification

**Purpose:** Confirm that the wiper positions stored in NVS produce the correct resonant frequency on each band.

**Required equipment:** NanoVNA with calibrated SOLT; connect to antenna feed point (coax tap).

**Procedure:**

1. Command motor to the stored 40m position: POST /api/band with `band=40m`.

2. Allow 10 seconds for position to settle.

3. Sweep NanoVNA from 6.8 to 7.5 MHz. Identify SWR minimum frequency.

4. Expected: SWR minimum at 7.15 ± 0.15 MHz.

5. If SWR minimum is displaced:
   - Too high (>7.3 MHz): need more inductance → increase stored position by 2000 steps and retest
   - Too low (<7.0 MHz): need less inductance → decrease stored position by 2000 steps and retest

6. Repeat for all HF bands. Record verified positions in the calibration log (Section 10.5).

7. Update all verified positions via REST API: POST /api/bands endpoint.

### 10.5 Calibration Log Template

```
SC-001 CALIBRATION LOG
Date: __________  Operator: __________  Location: __________
Ambient temperature: ____°C  Humidity: ____%

MOTOR CALIBRATION
  Steps per turn (calculated):  14765
  Measured displacement at 14765 steps: ________ mm
  Corrected steps per turn: ________
  Correction applied to firmware: YES / NO

AD8307 CALIBRATION
  No-signal FWD reading: ________ dBm   (target: < -50 dBm)
  No-signal REFL reading: ________ dBm  (target: < -50 dBm)
  FWD offset applied: ________ dB
  REFL offset applied: ________ dB

COUPLER CALIBRATION
  Applied test power: ________ W  (________ dBm)
  Measured forward power (API): ________ W  (________ dBm)
  Computed coupling factor: ________ dB  (target: 30 dB)
  Coupling correction applied: ________ dB

RESONANT FREQUENCY VERIFICATION
  Band | Target freq | Measured resonance | Pass/Fail | Adjusted position
  160m | 1.850 MHz   | ________           | _______   | ________
  80m  | 3.750 MHz   | ________           | _______   | ________
  60m  | 5.330 MHz   | ________           | _______   | ________
  40m  | 7.150 MHz   | ________           | _______   | ________
  30m  | 10.125 MHz  | ________           | _______   | ________
  20m  | 14.175 MHz  | ________           | _______   | ________
  17m  | 18.118 MHz  | ________           | _______   | ________
  15m  | 21.225 MHz  | ________           | _______   | ________
  12m  | 24.940 MHz  | ________           | _______   | ________
  10m  | 28.500 MHz  | ________           | _______   | ________

Technician signature: __________  Date: __________
```

---

---

<a name="chapter-11"></a>
## CHAPTER 11: FIELD SETUP AND OPERATING PROCEDURES

### 11.1 Pre-Deployment Checklist

Complete this checklist before each deployment:

| # | Item | Check |
|:--:|------|:-----:|
| 1 | Whip element: inspect for bends, corrosion at threads | ☐ |
| 2 | Whip base thread: 5/8-24 engagement minimum 8mm | ☐ |
| 3 | Coil assembly: wiper contact moves freely, no binding | ☐ |
| 4 | Wiper contact: silver paint track clean, not oxidized | ☐ |
| 5 | Home switch: verified functional (press manually, LED response) | ☐ |
| 6 | Top switch: verified functional | ☐ |
| 7 | Motor connections: 4-conductor cable fully seated at ULN2003 board | ☐ |
| 8 | ESP32: power LED on, no boot errors on serial monitor | ☐ |
| 9 | WiFi AP: `SC-001-TUNER` visible on scan | ☐ |
| 10 | AD8307 modules: both power LEDs on, output voltages 0.25–0.35V (no-signal) | ☐ |
| 11 | Coax tap solder joint: 4T tap wire secure, no cold joints | ☐ |
| 12 | Radials: 4 × wire radials unpacked and labeled | ☐ |
| 13 | Base mount: SO-239 clean, center pin not damaged | ☐ |
| 14 | Battery: charged (>3.8V per cell for LiPo), or 12V supply ready | ☐ |
| 15 | Extension coil (160m): if 160m operation planned, extension coil packed | ☐ |

### 11.2 Homing Procedure

The system must home successfully before any band operation:

1. Apply power to the ESP32 controller (+5V from USB or regulated supply).

2. Observe the green status LED. It will blink slowly (0.5 Hz) during homing.

3. The motor will drive the wiper toward the home position (decreasing turns direction). Homing speed is 20 ms/step.

4. When the home switch activates, the motor stops and reverses 50 steps.

5. The position counter is set to 0.

6. The green LED becomes solid ON — system is homed and ready.

7. If the green LED continues blinking after 60 seconds: motor stalled or home switch failed. See Chapter 12 troubleshooting.

> **CAUTION:** Never attempt to operate the antenna without completing a successful home sequence after power cycling. An unhomed system will report incorrect wiper positions and the auto-tune algorithm may drive the wiper against the mechanical stop.

### 11.3 Band Selection

**Via CYD display (Page 2 — Band Select):**
1. Tap desired band button. The display shows the band name highlighted.
2. Motor moves to stored position for selected band.
3. STATUS changes to READY when movement complete.

**Via Web interface (WiFi):**
1. Connect to `SC-001-TUNER` WiFi AP.
2. Open browser to 192.168.4.1.
3. Click the desired band button on the status dashboard.

**Via REST API:**
```bash
curl -X POST http://192.168.4.1/api/band -d "band=40m"
```

**Via CYD remote panel (Bluetooth):**
1. CYD must be paired with ESP32 (BT name `SC-001`).
2. Tap band on CYD band select page; CYD sends `CMD|BAND:40m` packet.

### 11.4 SWR Adjustment

**Automatic (preferred for initial tune or after location change):**
1. Ensure transmitter is keyed on a clear frequency (identify first per FCC regulations).
2. Tap TUNE on CYD or POST /api/tune via REST API.
3. Auto-tune takes 15–45 seconds. TUNING state indicated on display.
4. When complete, READY state shown with achieved SWR displayed.
5. If SWR > 1.5 after tune: run tune again; if still high after second attempt, see Chapter 12.

**Manual motor jog (for fine adjustment):**
1. Navigate to CYD Page 4 (Motor Control).
2. Use +/- step buttons to nudge wiper position.
3. Monitor SWR on Page 3 (SWR Meter).
4. Small steps (±1 count) for fine tuning; larger steps (±100 counts) for coarse.

### 11.5 Changing Loading Coil — HF to VHF

When operating on 6m, 2m, or UHF:

1. Set band to 6m via CYD or API to move wiper to home (position 0).

2. Disconnect coax feed from transceiver before removing coil.

3. Disconnect the coax from the antenna base SO-239.

4. Unscrew the main loading coil assembly from the antenna base (quarter-turn bayonet or threaded coupling, depending on installation).

5. Install the appropriate VHF element directly in the base:
   - 6m: 1.40m flexible wire or rigid rod, with 6m trap at 0.488m (optional 2m section)
   - 2m: 0.488m rigid element or 0.52m flexible wire

6. Reconnect coax feed.

7. Set band to 6m or 2m on controller. Verify SWR via API.

> **NOTE:** The ESP32 controller provides no benefit for VHF operation (no loading coil, no tuning required). The controller can remain powered to provide SWR monitoring via the directional coupler.

### 11.6 160m Extension Coil Installation

1. Disconnect the main loading coil assembly from the antenna base.

2. Connect the 160m extension coil (SO-239 plug/socket inline module) between the antenna base SO-239 and the main loading coil's bottom connector.

3. Reconnect the entire assembly to the antenna base.

4. Select 160m band on controller. Motor will move to 160m position.

5. Run auto-tune. With extension coil installed, the resonant position will be near the top of the main coil travel.

6. If SWR > 3.0 after auto-tune: verify extension coil is properly connected; check that extension coil inductance is in the 200–215 µH range with an LCR meter.

> **WARNING:** Maximum operating power with 160m extension coil is 5W continuous. The T200-2 toroid core will overheat at higher power levels. Monitor the extension coil temperature during operation; if it becomes too hot to hold (>55°C), reduce power or transmit duty cycle.

### 11.7 Portable Operation from Battery

| Component | Current Draw (active) | Current Draw (idle) |
|-----------|:---------------------:|:-------------------:|
| ESP32 (WiFi on) | 120 mA | 80 mA |
| ESP32 (BT on, WiFi off) | 90 mA | 60 mA |
| Motor (stepping, 2 phases) | 200 mA | 0 mA |
| Motor (1 phase stepping) | 100 mA | 0 mA |
| ULN2003 board | included above | 0 mA |
| AD8307 modules (×2) | 15 mA | 15 mA |
| CYD display (backlight on) | 220 mA | 80 mA (standby) |
| Status LEDs (×3) | 20 mA | 5 mA (one on) |
| Total (motor active) | ~575 mA | ~160 mA |
| Total (motor idle) | ~375 mA | ~160 mA |

**Battery sizing:**
- 3S LiPo, 2200 mAh: approximately 4.5 hours of mixed operation (20% motor active)
- 12V SLA, 7 Ah: approximately 18 hours of mixed operation
- USB power bank (20,000 mAh, 5V): approximately 25 hours at 5V nominal

> **NOTE:** The transmitter current is NOT included above; the antenna controller only draws power from its own supply, separate from the transceiver power supply.

### 11.8 Recording Tuning Data

Maintain a tuning log to build a site-specific calibration database:

| Date | Time | Band | Freq (MHz) | Step Pos | SWR | Fwd (W) | Ground | Notes |
|------|------|------|:----------:|:--------:|:---:|:-------:|--------|-------|
|      |      |      |            |          |     |         |        |       |

Record:
- Date and time (UTC preferred)
- Band and operating frequency
- Step position at minimum SWR
- Achieved SWR (from API or CYD)
- Forward power being used
- Ground system configuration (2 radials, 4 radials, mobile, etc.)
- Any notable observations (nearby conductors, antenna orientation, weather)

This log enables comparison of performance across locations and identification of systematic issues.

---

---

<a name="chapter-12"></a>
## CHAPTER 12: TROUBLESHOOTING

### 12.1 Troubleshooting Table

| Symptom | Likely Cause | Diagnostic Step | Remedy |
|---------|-------------|-----------------|--------|
| Motor won't home — motor runs but home switch never activates | Home switch GPIO 34 wiring open; switch NC/NO reversed | Measure GPIO 34 voltage with multimeter; manually press home switch — voltage should drop from 3.3V to 0V | Check wiring from switch to GPIO 34; verify 10 kΩ pullup to 3.3V; swap switch leads if NC/NO reversed |
| High SWR on all HF bands — SWR > 5:1 regardless of position | Coax tap connection broken or cold solder joint | Inspect 4T tap wire at bottom of coil; check continuity from tap to coax center conductor | Reflow solder joint on tap wire; verify 4T exit through endcap is not crushed |
| SWR reads 1.0:1 at all positions with no transmitter output | Reflected power detector path open circuit | Check D2 (1N5711 or AD8307 on reflected channel) orientation and solder joints; check C2 (coupling capacitor) | Replace D2; re-solder C2; verify AD8307 reflected channel is powered |
| Motor stalls mid-travel — motor hums but carriage doesn't move | Insufficient current from 5V supply; mechanical binding | Check 5V supply voltage at ULN2003 COM pin; manually rotate threaded rod for binding | Ensure 5V supply rated > 500 mA; check rod alignment; clean rod with IPA; lubricate with dry PTFE |
| Motor overshoots stored band position; repeated moves accumulate error | Step loss due to mechanical binding or too-fast step rate | Watch CYD position counter during move — if it reaches target but wiper is short: steps lost | Reduce fast speed delay from 2 ms to 3 ms; check for rod-carriage binding |
| Auto-tune doesn't converge — SWR oscillates, never settles | SWR changes during transmit due to coil heating | Allow 5-minute thermal soak at operating power before auto-tune | Reduce power to 10W, run auto-tune, then increase power; add thermal lag parameter |
| No Bluetooth connection from CYD | BT device name mismatch; BT not enabled; firmware version mismatch | On CYD, scan for BT devices — verify `SC-001` appears; check ESP32 serial log for BT init errors | Verify `BT_NAME` in config.h matches CYD BLE scan; confirm `bt_enabled` NVS value = true |
| Motor turns but wiper carriage doesn't move | Jaw coupler set screws loose; coupler slipping | Inspect jaw coupler while motor runs — if motor shaft turns without rod turning: coupler slipped | Tighten M3 set screws on jaw coupler; apply thread locker (Loctite 243) |
| SWR shifts after 30 minutes of operation | Thermal expansion of coil form changes effective inductance | After warm-up, re-run auto-tune and compare position to cold position; difference > 200 steps indicates thermal issue | Allow 15-minute warm-up before final tune at operating power; enable thermal compensation in firmware |
| 160m operation poor — SWR > 5:1 even at full coil | 160m extension coil not installed or connection failed | Check that extension coil inline module is present between base and main coil; verify inductance > 180 µH with LCR meter | Install T200-2 extension coil; check SO-239 connections on extension coil module |
| WiFi AP not visible in scan | ESP32 crash at boot; power supply brownout | Check serial monitor at 115200 baud for boot errors; measure 3.3V rail with multimeter | Supply must be > 400 mA at 5V; check USB cable quality; look for firmware flash error |
| VHF operation shows high SWR (>3:1) | Loading coil still connected in circuit | Verify main loading coil has been removed for VHF operation; verify wiper is at home (position 0) | Remove main coil assembly; install VHF direct element; see Section 11.5 |
| Coil gets hot during operation (>40°C to touch) | Transmitting at power level exceeding band rating | Measure coil temperature with IR thermometer; reduce power | Observe power limits: 160m=5W, 80m=25W, 40m=50W, 20m+=100W; reduce duty cycle |
| CYD shows wrong band or stale position data | BT packet parse error; token mismatch between firmware versions | Check serial output of CYD for packet parse errors; verify BAND: token present in packet | Update ESP32 and CYD firmware to same version; verify packet format matches Section 9.4 |
| Wiper arcing during high-power operation — audible click, SWR jumps | Contact pressure too low or track oxidized; insufficient contact area | Inspect contact tip under magnification; clean track with Deoxit D5 | Clean track and contact with Deoxit; adjust spring tension to increase contact force to 1.0–1.2 N; re-apply silver paint to track if worn |

---

---

<a name="chapter-13"></a>
## CHAPTER 13: PARTS AND MATERIALS REFERENCE

### 13.1 Main Coil Components

| Item | Specification | Quantity | Source |
|------|--------------|:--------:|--------|
| Coil form | PVC pipe, 75mm OD, Schedule 40, 140mm length | 1 | Home Depot / hardware store |
| Winding wire | #14 AWG tinned copper, stranded OK | 10 m | Mouser, Digi-Key, or wire supplier |
| End caps | 3D-printed PETG or turned PVC (from coil_form.scad) | 2 | Printed locally |
| 4T tap wire | #18 AWG stranded, 50mm length | 1 | Scrap from winding wire |
| Tap connector | SMA female PCB mount or screw terminal | 1 | Mouser |
| Whip thread insert | 5/8-24 female thread brass insert, 20mm length | 1 | RadioWorld, SOTABEAMS |
| Base stud | 1/4-20 stainless stud, 30mm length, for mast mount | 1 | Hardware store |
| Varnish | Clear polyurethane varnish or Hobby-Coat | 1 | Hardware or hobby store |
| Conductive track paint | MG Chemicals 842AR silver paint, 12ml tube | 1 | Mouser, DigiKey |
| Phosphor bronze strip | 0.3 mm × 6 mm × 300 mm strip | 1 | Online metal supplier |

### 13.2 Extension Coil (160m) Components

| Item | Specification | Quantity | Source |
|------|--------------|:--------:|--------|
| Core | T200-2 powdered iron toroid (Amidon) | 1 | Mouser, Amidon |
| Winding wire | #18 AWG enameled magnet wire | 8 m | Mouser |
| Core varnish | Single-component polyurethane, clear | 1 | Hardware store |
| Housing shell | 3D-printed PETG cylinder, 55mm OD, 30mm length | 1 | Printed locally |
| Connectors | SO-239 flange mount × 2 (inline module) | 2 | RF connector suppliers |
| Shield braid | RG-213 outer jacket (braid + jacket), 80mm | 1 | Scrap coax |

### 13.3 Mechanical Drive Train

| Item | Specification | Quantity | Source |
|------|--------------|:--------:|--------|
| Threaded rod | 1/4-20 stainless, 150mm | 1 | Hardware store |
| Jaw coupler, motor side | 5mm bore jaw coupler half | 1 | Amazon, AliExpress |
| Jaw coupler, rod side | 1/4-20 to 6.35mm bore adapter or custom | 1 | Machine shop / 3D print |
| Jaw coupler spider | Polyurethane elastomer insert, 98A hardness | 1 | With coupler set |
| Limit switch (home) | Momentary microswitch, NO+C+NC, panel mount | 1 | Digi-Key |
| Limit switch (top) | Momentary microswitch, NO+C+NC, panel mount | 1 | Digi-Key |
| Nylon bushing | 1/4" ID nylon bushing for end bearing block | 1 | Hardware store |
| M3 screws | M3×8 stainless (motor mount), M3×12 (housing) | 12 | Hardware store |
| M2 screws | M2×6 stainless (switch mounts) | 4 | Hardware store |
| PTFE sheet | 0.5mm PTFE sheet for carriage guide pads | 1 piece | Amazon |

### 13.4 Electronic Components

| Item | Part Number | Quantity | Source |
|------|-------------|:--------:|--------|
| Stepper motor | 28BYJ-48-5V | 1 | Amazon, AliExpress |
| Motor driver | ULN2003 driver board for 28BYJ-48 | 1 | Amazon, AliExpress |
| Microcontroller | ESP32-WROOM-32 development board (30-pin) | 1 | Digi-Key, Mouser, Amazon |
| Log amp, forward | AD8307 power detector module | 1 | AliExpress, Digi-Key |
| Log amp, reflected | AD8307 power detector module | 1 | AliExpress, Digi-Key |
| CYD display | ESP32-2432S028 (2.8" 320×240 TFT) | 1 | AliExpress |
| Coupler core | FT-37-43 ferrite toroid | 1 | Mouser, Amidon |
| Termination resistor | 51 Ω, 0.25W, 1% metal film | 1 | Digi-Key |
| Attenuator resistors | 10 dB pi pad to 200 Ω (see schematic) | 1 set | Digi-Key |
| Schottky diodes | 1N5711 (backup; preferred is AD8307) | 2 | Digi-Key |
| Decoupling capacitors | 100 nF ceramic, 0.1 µF, various | 10 | Digi-Key |
| Coupling capacitors | 1000 pF silver mica, 500V, ±1% | 2 | Digi-Key |
| Status LEDs | 5mm green, amber, red LEDs | 3 | Digi-Key |
| Current-limiting resistors | 220 Ω, 0.25W for LEDs | 3 | Digi-Key |
| Pullup resistors | 10 kΩ, 0.25W for limit switch GPIOs | 2 | Digi-Key |
| PCB | 100×80mm perf board or fabricated PCB | 1 | OSHPark or DigiKey |
| Barrel connector | 2.1mm DC barrel, panel mount | 1 | Digi-Key |
| 5V regulator | 7805 or LM2596 buck module | 1 | Digi-Key |
| Input capacitor | 1000 µF, 25V electrolytic | 1 | Digi-Key |

### 13.5 3D-Printed Parts

| File | Part | PETG Mass (est.) | Print Time |
|------|------|:----------------:|:----------:|
| motor_base.scad | Lower housing | 85 g | 3.5 hr |
| coil_form.scad | Upper housing | 65 g | 2.8 hr |
| wiper_carriage.scad | Wiper carriage | 12 g | 1.2 hr |
| wiper_arm.scad | Contact arm bracket | 4 g | 0.4 hr |
| bearing_block.scad | Rod end bearing | 6 g | 0.3 hr |
| mast_clamp.scad | Mast clamping plate | 18 g | 0.5 hr |
| base_plate.scad | Radial connector plate | 25 g | 1.0 hr |
| extension_coil_housing.scad | 160m coil housing | 22 g | 0.9 hr |
| **Total PETG** | | **237 g** | **10.6 hr** |

---

---

<a name="appendix-a"></a>
## APPENDIX A: COMPLETE COIL WINDING TABLE

This table gives calculated values for every 5-turn increment across the full 41-turn main coil range. Values are computed from the Wheeler formula and the resonance condition (Xant from feed reactance table).

Formula: L(N) = 14.0625 × N² / (85.73 + 5.842 × N) µH
Resonant frequency: f_res = |Xant| / (2πL) — solve iteratively for exact band match.

| Position | Turns (N) | L (µH) | Primary Band | f_res_approx (MHz) | Notes |
|:--------:|:---------:|:------:|:------------:|:------------------:|-------|
| 1        | 0         | 0.000  | —            | —                  | Home position |
| 2        | 1         | 0.153  | 10m+         | > 35 MHz           | Near VHF, no match |
| 3        | 2         | 0.395  | 10m          | 29.8 MHz           | 10m low edge |
| 4        | 3         | 0.726  | 10m          | 28.1 MHz           | 10m center |
| 5        | 4         | 1.136  | 10m          | 25.6 MHz           | 10m low; 12m edge |
| 6        | 5         | 1.618  | 12m          | 23.1 MHz           | 12m operation |
| 7        | 6         | 2.164  | 12m          | 21.8 MHz           | 12m/15m boundary |
| 8        | 7         | 2.766  | 15m          | 20.2 MHz           | 15m center |
| 9        | 8         | 3.417  | 15m          | 19.0 MHz           | 15m/17m boundary |
| 10       | 9         | 4.112  | 17m          | 17.8 MHz           | 17m center |
| 11       | 10        | 4.843  | 17m          | 16.6 MHz           | 17m/20m boundary |
| 12       | 11        | 5.606  | 20m          | 15.6 MHz           | 20m low edge |
| 13       | 12        | 6.394  | 20m          | 14.8 MHz           | 20m center |
| 14       | 13        | 7.203  | 20m          | 14.1 MHz           | 20m/17m overlap |
| 15       | 14        | 8.028  | 20m          | 13.4 MHz           | 20m high end |
| 16       | 15        | 8.866  | 20m/30m      | 12.7 MHz           | |
| 17       | 16        | 9.713  | 30m edge     | 12.2 MHz           | |
| 18       | 17        | 10.566 | 30m          | 11.7 MHz           | |
| 19       | 18        | 11.422 | 30m          | 11.1 MHz           | 30m center |
| 20       | 19        | 12.281 | 30m          | 10.7 MHz           | |
| 21       | 20        | 13.139 | 30m/40m      | 10.4 MHz           | |
| 22       | 21        | 13.996 | 40m edge     | 10.1 MHz           | |
| 23       | 22        | 14.849 | 40m          | 9.77 MHz           | |
| 24       | 23        | 15.697 | 40m          | 9.44 MHz           | |
| 25       | 24        | 16.540 | 40m          | 9.12 MHz           | |
| 26       | 25        | 17.376 | 40m          | 8.82 MHz           | |
| 27       | 26        | 18.205 | 40m          | 8.55 MHz           | |
| 28       | 27        | 19.025 | 40m center   | 8.23 MHz           | 40m SSB center |
| 29       | 28        | 19.837 | 40m          | 7.94 MHz           | |
| 30       | 29        | 20.639 | 40m          | 7.69 MHz           | |
| 31       | 30        | 21.431 | 40m          | 7.49 MHz           | |
| 32       | 31        | 22.214 | 40m          | 7.29 MHz           | |
| 33       | 32        | 22.986 | 40m          | 7.10 MHz           | 40m CW edge |
| 34       | 33        | 23.748 | 60m approach | 6.88 MHz           | |
| 35       | 34        | 24.499 | 60m          | 6.70 MHz           | 60m entry |
| 36       | 35        | 25.240 | 60m          | 6.47 MHz           | |
| 37       | 36        | 25.970 | 60m          | 6.28 MHz           | |
| 38       | 37        | 26.689 | 60m/80m      | 6.06 MHz           | 60m band |
| 39       | 38        | 27.397 | 80m edge     | 5.87 MHz           | |
| 40       | 39        | 28.094 | 80m          | 5.65 MHz           | 80m approach |
| 41       | 40        | 28.781 | 80m          | 5.43 MHz           | |
| 42       | 41 (full) | 29.456 | 80m          | 5.20 MHz           | Full coil (80m low edge) |

> **NOTE:** The table shows that the main coil alone only reaches 5.2 MHz at 41 turns, requiring 80m and 160m operation to rely on external extension coils or an outboard L network. For a main coil that covers 80m without extension, increase turns to 55+ and rebuild with tighter winding.

---

<a name="appendix-b"></a>
## APPENDIX B: POSITION VS. FREQUENCY TABLE

This table gives 25 representative entries from the `position_vs_frequency.csv` dataset showing the relationship between step position, coil turns, inductance, and estimated resonant frequency for the 2.4m whip.

| Row | Step Position | Coil Turns | L (µH)  | Est. Resonant Freq (MHz) | Band | SWR (typical) |
|:---:|:------------:|:----------:|:-------:|:------------------------:|------|:-------------:|
| 1   | 0            | 0.000      | 0.000   | > 35 MHz                 | VHF only | N/A |
| 2   | 14765        | 1.000      | 0.153   | 33.0 MHz                 | VHF/10m | — |
| 3   | 29530        | 2.000      | 0.395   | 29.5 MHz                 | 10m | 1.4 |
| 4   | 44295        | 3.000      | 0.726   | 28.2 MHz                 | 10m | 1.2 |
| 5   | 59060        | 4.000      | 1.136   | 25.5 MHz                 | 12m edge | 1.5 |
| 6   | 73825        | 5.000      | 1.618   | 22.8 MHz                 | 12m | 1.3 |
| 7   | 103155       | 6.986      | 2.773   | 19.8 MHz                 | 15m | 1.3 |
| 8   | 132685       | 8.982      | 4.115   | 17.7 MHz                 | 17m | 1.2 |
| 9   | 148000       | 10.02      | 4.860   | 16.6 MHz                 | 17m/20m | 1.4 |
| 10  | 162215       | 10.987     | 5.615   | 15.5 MHz                 | 20m edge | 1.5 |
| 11  | 192000       | 13.00      | 7.220   | 14.0 MHz                 | 20m | 1.3 |
| 12  | 221000       | 14.97      | 8.856   | 12.7 MHz                 | 20m/30m | 1.6 |
| 13  | 251000       | 17.00      | 10.567  | 11.7 MHz                 | 30m | 1.4 |
| 14  | 280000       | 18.96      | 12.248  | 10.7 MHz                 | 30m | 1.3 |
| 15  | 309000       | 20.93      | 13.930  | 10.1 MHz                 | 30m/40m | 1.4 |
| 16  | 324000       | 21.94      | 14.770  | 9.75 MHz                 | 40m edge | 1.6 |
| 17  | 368000       | 24.93      | 17.310  | 8.91 MHz                 | 40m | 1.4 |
| 18  | 398000       | 26.95      | 18.988  | 8.28 MHz                 | 40m center | 1.4 |
| 19  | 428000       | 28.98      | 20.626  | 7.72 MHz                 | 40m | 1.3 |
| 20  | 458000       | 31.01      | 22.221  | 7.29 MHz                 | 40m CW | 1.3 |
| 21  | 488000       | 33.05      | 23.772  | 6.87 MHz                 | 60m | 1.5 |
| 22  | 518000       | 35.08      | 25.276  | 6.44 MHz                 | 60m | 1.4 |
| 23  | 545000       | 36.91      | 26.618  | 6.15 MHz                 | 60m | 1.5 |
| 24  | 572000       | 38.74      | 27.930  | 5.91 MHz                 | 80m edge | 1.8 |
| 25  | 605000       | 40.98      | 29.402  | 5.21 MHz                 | 80m (main coil limit) | 2.0 |

---

<a name="appendix-c"></a>
## APPENDIX C: HALF-STEP SEQUENCE AND TIMING TABLE

### C.1 Half-Step Sequence (Complete 8-Step Cycle)

| Step | IN1 | IN2 | IN3 | IN4 | Phases Active | Approx. Torque (% max) |
|:----:|:---:|:---:|:---:|:---:|:-------------:|:----------------------:|
| 1    |  1  |  0  |  0  |  0  | A             | 70% |
| 2    |  1  |  1  |  0  |  0  | A + B         | 100% |
| 3    |  0  |  1  |  0  |  0  | B             | 70% |
| 4    |  0  |  1  |  1  |  0  | B + C         | 100% |
| 5    |  0  |  0  |  1  |  0  | C             | 70% |
| 6    |  0  |  0  |  1  |  1  | C + D         | 100% |
| 7    |  0  |  0  |  0  |  1  | D             | 70% |
| 8    |  1  |  0  |  0  |  1  | D + A         | 100% |

### C.2 Timing Constraints

| Parameter | Minimum | Recommended | Maximum | Notes |
|-----------|:-------:|:-----------:|:-------:|-------|
| Step delay (fast) | 2 ms | 3 ms | — | Below 2 ms: stall risk |
| Step delay (slow/homing) | 8 ms | 20 ms | — | Slower = more reliable homing |
| Phase-off delay after stop | 0 ms | 100 ms | — | Motor coasts to stop; de-energize after |
| Re-energize delay before move | 0 ms | 50 ms | — | Allow supply to stabilize |
| SWR settle time after move | 50 ms | 200 ms | — | Allow antenna to stabilize |

### C.3 Current Requirements

| Condition | Current (mA) | Duration | Supply Requirement |
|-----------|:------------:|:--------:|-------------------|
| Single phase active (half-step) | 100 | Continuous during step | 5V @ 100 mA |
| Two phases active (half-step) | 200 | Continuous during step | 5V @ 200 mA |
| All phases off (idle) | 0 | Continuous at rest | 0 mA from motor supply |
| Stall (both phases, no movement) | 200 | Transient (< 5 sec) | 5V @ 200 mA; will overheat if prolonged |
| Peak inrush (phase switch) | 300 | < 1 ms | Decoupled by 1000 µF cap on 5V rail |

> **CAUTION:** Stalling the motor (commanding movement against a mechanical stop with phases energized) will overheat the ULN2003 driver and the motor winding within 30 seconds. The top and home limit switches prevent this in normal operation. If the limit switch circuit fails, a stall condition will occur and the motor/driver will be damaged. The firmware includes a maximum-steps-from-home limit as a software backstop: no move command can exceed 650,000 steps from home.

---

<a name="appendix-d"></a>
## APPENDIX D: NEC CARD REFERENCE

The following reference covers the NEC2 card types used in the SC-001 model files.

### D.1 GW Card — Wire Geometry

```
GW  tag  segs  x1  y1  z1  x2  y2  z2  radius
```

| Field | Description |
|-------|-------------|
| tag | Segment group tag number (1–999) |
| segs | Number of segments (recommend 11 for HF, 21 for accuracy) |
| x1,y1,z1 | Start point coordinates (meters) |
| x2,y2,z2 | End point coordinates (meters) |
| radius | Wire radius (meters) |

**2.4m whip example (vertical, base at z=0):**
```
GW  1  11  0  0  0  0  0  2.4  0.004
```

**4 radials (horizontal at z=0, each 2.5m):**
```
GW  2  5   0  0  0  2.5   0   0  0.001
GW  3  5   0  0  0  -2.5  0   0  0.001
GW  4  5   0  0  0  0   2.5   0  0.001
GW  5  5   0  0  0  0   -2.5  0  0.001
```

### D.2 GE Card — Geometry End

```
GE  ground_flag
```
- ground_flag = 0: free space
- ground_flag = 1: perfect ground plane (use for initial models)
- ground_flag = -1: use Sommerfeld ground model (GN card must follow)

### D.3 GN Card — Ground Parameters

```
GN  type  nradials  0  0  epsr  sigma  ...
```
- type = 0: Sommerfeld
- type = 1: reflective ground (fast approximate)
- epsr: relative permittivity (typical dry ground: 13)
- sigma: conductivity in S/m (typical dry ground: 0.005)

### D.4 EX Card — Excitation

```
EX  0  tag  seg  0  1.0  0.0
```
- Type 0: voltage source
- tag and seg: identifies the source segment (use tag=1, seg=1 for base of whip)
- 1.0, 0.0: voltage magnitude and phase (1V, 0°)

### D.5 LD Card — Loading

```
LD  5  tag  seg  0  R  L  C
```
- Type 5: series R-L-C per unit length (use type 5 for lumped loading)
- R: resistance in Ω
- L: inductance in Henries
- C: capacitance in Farads (use 0 for R-L only)

**40m loading coil on segment 1 of whip:**
```
LD  5  1  1  0  3.46  2.15E-5  0
```

### D.6 FR Card — Frequency

```
FR  0  1  0  0  7.15  0
```
- Single frequency: 7.15 MHz

**Frequency sweep (6.8 to 7.5 MHz, 8 steps):**
```
FR  0  8  0  0  6.8  0.1
```

### D.7 RP Card — Radiation Pattern

```
RP  0  91  1  1000  0  0  1  360
```
- Elevation pattern: 0° to 90° in 1° steps, single azimuth
- Output mode 1000: major lobe pattern

### D.8 EN Card — End of Input

```
EN
```

### D.9 Complete 40m Model Example

```nec
CM TM-SC-NEC-004 — SC-001 40m model
CM 2.4m whip, base-loaded, 4 radials, perfect ground
CE
GW  1  11  0  0  0  0  0  2.4  0.004
GW  2  5   0  0  0  2.5   0   0  0.001
GW  3  5   0  0  0  -2.5  0   0  0.001
GW  4  5   0  0  0  0   2.5   0  0.001
GW  5  5   0  0  0  0   -2.5  0  0.001
GE  1
LD  5  1  1  0  3.46  2.15E-5  0
EX  0  1  1  0  1.0  0.0
FR  0  1  0  0  7.15  0
RP  0  91  1  1000  0  0  1  360
EN
```

---

<a name="appendix-e"></a>
## APPENDIX E: QUICK REFERENCE FIELD CARD

The following table is formatted as a field reference card. Print on laminated 4×6 card stock, fold to fit shirt pocket.

```
╔══════════════════════════════════════════════════════════════════════════════╗
║          SC-001 SCREWDRIVER ANTENNA — QUICK REFERENCE FIELD CARD           ║
║                    WB6MRM / TM-SC-001 Rev A / 2026-05-08                   ║
╠══════════╦════════════╦══════════╦═══════════╦════════════╦════════════════╣
║  Band    ║  Freq      ║  Steps   ║  Turns    ║  Effic.   ║  Notes         ║
║          ║  (MHz)     ║  (×1000) ║  Active   ║  (%)      ║                ║
╠══════════╬════════════╬══════════╬═══════════╬════════════╬════════════════╣
║  160m    ║  1.85      ║  607     ║  41+ext   ║  0.1      ║  Ext coil req. ║
║  80m     ║  3.75      ║  575     ║  39       ║  1.2      ║  ATU needed    ║
║  60m     ║  5.33      ║  502     ║  34       ║  3.5      ║  NVIS          ║
║  40m     ║  7.15      ║  398     ║  27       ║  8.6      ║  Practical     ║
║  30m     ║  10.125    ║  324     ║  22       ║  23       ║  CW only       ║
║  20m     ║  14.175    ║  251     ║  17       ║  51       ║  Primary DX    ║
║  17m     ║  18.118    ║  192     ║  13       ║  73       ║  Excellent     ║
║  15m     ║  21.225    ║  148     ║  10       ║  84       ║  Excellent     ║
║  12m     ║  24.94     ║  103     ║   7       ║  93       ║  Near-QW       ║
║  10m     ║  28.5      ║   44     ║   3       ║  97       ║  Full eff.     ║
╠══════════╩════════════╩══════════╩═══════════╩════════════╩════════════════╣
║  POWER LIMITS:  160m=5W  80m=25W  40m=50W  20m and above=100W             ║
╠══════════════════════════════════════════════════════════════════════════════╣
║  WiFi: SC-001-TUNER  PW: screwdriver  IP: 192.168.4.1                     ║
║  BT: SC-001  API: GET /api/status  POST /api/band  POST /api/tune          ║
╠══════════════════════════════════════════════════════════════════════════════╣
║  STARTUP: Power → Home (green LED solid) → Select band → Transmit → Tune  ║
╠══════════════════════════════════════════════════════════════════════════════╣
║  VHF (6m/2m): Remove coil → Install VHF element → Select band             ║
║  160m:        Install ext coil (series with base) → Select 160m → Tune    ║
╠══════════════════════════════════════════════════════════════════════════════╣
║  SWR ALARM: If SWR>3.0 after two auto-tune cycles → See Ch.12             ║
╚══════════════════════════════════════════════════════════════════════════════╝
```

---

<a name="appendix-f"></a>
## APPENDIX F: WIRING DIAGRAM (ASCII)

```
SC-001 SCREWDRIVER ANTENNA CONTROLLER — COMPLETE WIRING DIAGRAM
TM-SC-001 Rev A

                            +12V INPUT
                                │
                           ┌────┴────┐
                           │ 7805 or │
                           │ LM2596  │ ─── 1000µF/25V cap ─── GND
                           │ 5V REG  │
                           └────┬────┘
                                │+5V bus
         ┌──────────────────────┼──────────────────────────────────┐
         │                      │                                  │
    ┌────┴────────────────────┐ │                    ┌─────────────┴──────┐
    │   ULN2003 DRIVER BOARD  │ │                    │  ESP32-WROOM-32    │
    │                         │ │                    │  (30-pin devboard) │
    │  IN1 ──────────────────────────────────────── GPIO 25              │
    │  IN2 ──────────────────────────────────────── GPIO 26              │
    │  IN3 ──────────────────────────────────────── GPIO 27              │
    │  IN4 ──────────────────────────────────────── GPIO 14              │
    │  COM ──── +5V bus                             │                    │
    │  GND ──── GND                                 │  GPIO 34 ─── 10kΩ ─── 3.3V
    │                         │                     │       └── HOME SW ─── GND
    │  OUT1 ─┐                │                     │  GPIO 35 ─── 10kΩ ─── 3.3V
    │  OUT2 ─┤ 28BYJ-48       │                     │       └── TOP  SW ─── GND
    │  OUT3 ─┤ MOTOR          │                     │                    │
    │  OUT4 ─┘ (5-pin JST-XH) │                     │  GPIO 36 (ADC1_CH0)│
    └─────────────────────────┘                     │       └────────────────────►
                                                    │  GPIO 39 (ADC1_CH3)│  AD8307
                                                    │       └────────────────────►  MODULES
                                                    │                    │  (see below)
                                                    │  GPIO 2 ─── 220Ω ─── GREEN LED ─── GND
                                                    │  GPIO 4 ─── 220Ω ─── AMBER LED ─── GND
                                                    │  GPIO 5 ─── 220Ω ─── RED   LED ─── GND
                                                    │                    │
                                                    │  3.3V ─── AD8307 VCC (both modules)
                                                    │  GND  ─── AD8307 GND (both modules)
                                                    │                    │
                                                    │  TX0 ─── USB/Serial monitor
                                                    │  GND ─── GND bus   │
                                                    └────────────────────┘

─── SWR DIRECTIONAL COUPLER ────────────────────────────────────────────────────

  COAX FROM        ┌─────────────────────────────────────────────┐
  TRANSCEIVER ─────┤  50Ω MAIN LINE                              ├──── TO ANTENNA
  (PL-259)         │                                             │     BASE (SO-239)
                   │   FT-37-43                                  │
                   │   ┌──────┐    10T secondary                 │
                   │   │ CORE │───┬──────────────────────────┐  │
                   │   └──────┘   │                          │  │
                   │              │    51Ω term.              │  │
                   │              ├──┤    ├── GND             │  │
                   │              │                          │  │
                   │              ▼ FWD port                 │  │
                   │         10dB Pi-pad (to 200Ω)           │  │
                   │              │                          │  │
                   │              ▼                          │  │
                   │         AD8307 FWD (INP+)               │  │
                   │         GPIO 36 ◄── AD8307 VOUT         │  │
                   │                                         │  │
                   │              ▼ REFL port                │  │
                   │         10dB Pi-pad (to 200Ω)           │  │
                   │              │                          │  │
                   │              ▼                          │  │
                   │         AD8307 REFL (INP+)              │  │
                   │         GPIO 39 ◄── AD8307 VOUT         │  │
                   └─────────────────────────────────────────┘  │

─── POWER DISTRIBUTION ──────────────────────────────────────────────────────────

  +12V input ──── 1000µF/25V ──── 7805/LM2596 ──── +5V bus
                                                      │
                              ┌───────────────────────┤
                              │                       │
                         ULN2003 COM              3.3V regulator (on ESP32 board)
                         Motor 5V supply              │
                                                  ESP32 VCC
                                                  AD8307 VCC ×2
                                                  Limit switch pull-ups
                                                  LED current limiters

─── CYD DISPLAY WIRING (optional Bluetooth remote panel) ─────────────────────

  CYD ESP32-2432S028 runs separate firmware
  Communicates with SC-001 ESP32 via Bluetooth Classic SPP

  CYD powered from USB (5V) or separate 5V supply
  No wired connection to SC-001 controller required
  BT name: SC-001
  Protocol: 9600 baud SPP, packet format per Section 9.4

─── LIMIT SWITCH DETAIL ─────────────────────────────────────────────────────────

         3.3V
          │
         10kΩ
          │
  GPIO 34 ├─────────────────────────── C (common)
  (HOME)  │                            │
          │                        SW (NO)
          │                            │
         GND ────────────────────── NO contact

  (Same circuit for GPIO 35 / TOP switch)

─── COIL TAP WIRING ─────────────────────────────────────────────────────────────

  COAX CENTER ──────────────── 4T tap on loading coil (4 turns from base)
  COAX BRAID ───────────────── Base plate / ground / radial common
  Coil top ─────────────────── Whip element base stud
  Coil bottom ──────────────── Wiper contact return / chassis

  Wiper carriage ──── silver-paint coil track ──── main coil turns
       │
  Wiper signal ────── (not RF-connected; wiper moves along track,
                       varies active turns between tap point and whip)
```

---

**END OF DOCUMENT**

---

*TM-SC-001 Rev A — 2026-05-08*  
*Issued by: Merv Martin, WB6MRM*  
*Next revision: Rev B — scheduled after field test validation, Summer 2026*

---
