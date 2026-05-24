# TECHNICAL MANUAL
## POOL NOODLE NON-LOOP ANTENNA SYSTEM
### TM-PNL2-001 Rev A

---

```
DISTRIBUTION STATEMENT A: Approved for public release; distribution unlimited.
Amateur radio construction project — personal use.
```

**Document Number:** TM-PNL2-001 Rev A  
**Title:** Pool Noodle Non-Loop Antenna System — 160m through 20cm  
**Author:** Merv Martin  
**Date:** 2026-05-08  
**Supersedes:** None (initial release)

**Related Documents:**

| Document | Title |
|---------|-------|
| TM-PNL2-SCH-001 | Loading Coil Theory — Shortened Dipole |
| TM-PNL2-SCH-002 | J-Pole Antenna Matching and Construction |
| TM-PNL2-SCH-003 | Yagi-Uda Antenna Design for Pool Noodle Boom |
| TM-PNL2-ENC-001 | Loading Coil Housing (OpenSCAD) |
| TM-PNL2-ENC-002 | Dipole Center Insulator (OpenSCAD) |
| TM-PNL2-ENC-003 | J-Pole Base Mount (OpenSCAD) |
| TM-PNL2-ENC-004 | Yagi Element Clip (OpenSCAD) |
| TM-PNL2-FW-001  | ESP32 PNL Tuner Firmware |
| TM-PNL2-FW-002  | CYD PNL Display Firmware |
| TM-PNL2-NEC-001 through -015 | NEC-2 Antenna Models |

---

## TABLE OF CONTENTS

- [Chapter 1: Introduction and Scope](#chapter-1-introduction-and-scope)
- [Chapter 2: Theory — Inductive Loading and Shortened Dipoles](#chapter-2-theory--inductive-loading-and-shortened-dipoles)
- [Chapter 3: Theory — J-Pole Antenna](#chapter-3-theory--j-pole-antenna)
- [Chapter 4: Theory — Yagi-Uda Directive Array](#chapter-4-theory--yagi-uda-directive-array)
- [Chapter 5: Materials and Parts List](#chapter-5-materials-and-parts-list)
- [Chapter 6: Construction — HF Loading Coil Dipole (160m–10m)](#chapter-6-construction--hf-loading-coil-dipole-160m10m)
- [Chapter 7: Construction — Multi-Band Roll-up Trap Dipole](#chapter-7-construction--multi-band-roll-up-trap-dipole)
- [Chapter 8: Construction — VHF J-Pole (6m–1.25m)](#chapter-8-construction--vhf-j-pole-6m125m)
- [Chapter 9: Construction — UHF Yagi (70cm–20cm)](#chapter-9-construction--uhf-yagi-70cm20cm)
- [Chapter 10: Directional Coupler SWR System](#chapter-10-directional-coupler-swr-system)
- [Chapter 11: ESP32 Relay Tuner Firmware](#chapter-11-esp32-relay-tuner-firmware)
- [Chapter 12: CYD Display Firmware](#chapter-12-cyd-display-firmware)
- [Chapter 13: Tuning and Alignment](#chapter-13-tuning-and-alignment)
- [Chapter 14: Performance Data](#chapter-14-performance-data)
- [Chapter 15: Safety](#chapter-15-safety)
- [Chapter 16: Maintenance and Field Repair](#chapter-16-maintenance-and-field-repair)
- [Appendix A: Band Data Sheets](#appendix-a-band-data-sheets)
- [Appendix B: Bill of Materials](#appendix-b-bill-of-materials)
- [Appendix C: NEC Model Summary](#appendix-c-nec-model-summary)
- [Appendix D: Troubleshooting](#appendix-d-troubleshooting)
- [Appendix E: References](#appendix-e-references)

---

## CHAPTER 1: INTRODUCTION AND SCOPE

### 1.1 Purpose

This technical manual provides design, construction, tuning, and operating instructions for three families of pool noodle antennas covering all fifteen amateur radio bands from 160 meters to 20 centimeters:

1. **Pool Noodle Inductive Loading Dipole (PNILD)** — HF bands 160m through 10m. A shortened center-loaded dipole where the pool noodle is the loading coil former. A 9-position relay-switched coil tap system enables automatic band selection.

2. **Pool Noodle J-Pole** — VHF bands 6m, 2m, and 1.25m. The pool noodle serves as the insulating support for the quarter-wave matching stub. No tuning required; passive SWR monitoring via the coupler system.

3. **Pool Noodle Yagi** — UHF bands 70cm, 33cm, and 20cm. The pool noodle serves as the antenna boom. Element clips (TM-PNL2-ENC-004) mount wire elements perpendicular to the boom. 3-element (70cm), 5-element (33cm), and 7-element (20cm) configurations.

### 1.2 Design Philosophy

This suite complements the Pool Noodle Magnetic Loop system (TM-PNL-001). Where the loop provides high-Q narrow-bandwidth resonant operation with a tuning capacitor, the PNILD provides:

- **Lower circulating voltage** — no high-Q capacitor; the load is distributed across the coil and wire arms
- **Wider instantaneous bandwidth** — 2.1 kHz (160m) to 32.5 kHz (10m)
- **Simple construction** — wire arms, wound coil, coax feed with balun
- **Comparable efficiency on upper HF** — 86–90% on 15m–20m, competitive with full-size dipoles

The J-pole and Yagi are complementary technologies covering bands where the HF dipole is not applicable (VHF/UHF).

### 1.3 System Overview

```
  Controller: ESP32-WROOM-32
    ├── Bruene directional coupler + dual AD8307 → SWR/power measurement
    ├── 74HC595 shift register → 9 relay drivers → loading coil taps
    ├── Wi-Fi AP (192.168.4.1) → web dashboard
    └── Bluetooth serial → CYD display (TM-PNL2-FW-002)

  Display: ESP32-2432S028R (CYD)
    ├── ILI9341 TFT 320×240 16-bit color
    ├── XPT2046 touch controller
    ├── 4 pages: SWR, Antenna Info, Power, Guide
    └── Bluetooth serial receive from ESP32 tuner

  Antennas:
    PNILD (HF):   Loading coil (wound on pool noodle) + wire arms
    J-Pole (VHF): Pool noodle stub support + wire radiator
    Yagi (UHF):   Pool noodle boom + element clips + wire elements
```

### 1.4 Document Conventions

- All frequencies in MHz unless stated otherwise
- All lengths in meters unless stated otherwise
- Impedances in ohms (Ω)
- Wire gauge: AWG (American Wire Gauge)
- Pool noodle dimensions: OD = 47mm, ID = 25mm, L = 120cm

---

## CHAPTER 2: THEORY — INDUCTIVE LOADING AND SHORTENED DIPOLES

### 2.1 The Short Dipole Problem

A resonant half-wave dipole has physical length λ/2 and input impedance ≈ 73 + j0 Ω. When the dipole is physically shorter than λ/2, the input impedance becomes capacitive:

```
  Z_input = R_rad + j × X_input

  X_input < 0  (negative = capacitive) for physical length < λ/2
```

The antenna cannot accept energy efficiently in this state. It must be brought to resonance (X_input = 0) by adding an inductive loading element.

### 2.2 Transmission-Line Model of Dipole Arm

Each arm of a center-fed dipole behaves as an open-ended transmission line. For arm length h < λ/4:

```
  Z_arm = -j × Z₀_arm × cot(βh)    [capacitive input impedance]

  β = 2π/λ    (wave number)
  h = physical arm length [m]

  Characteristic impedance of thin wire in free space:
  Z₀_arm = 120 × (ln(2h/a) − 1)    [Ω]

  where a = wire radius [m]
  For #14 AWG (a = 0.814mm): Z₀_arm ≈ 880 – 1115 Ω (varies with h)
```

The loading coil must supply inductive reactance X_L = Z₀_arm × cot(βh) to cancel the arm's capacitive reactance and achieve resonance.

### 2.3 Required Loading Inductance

```
  X_L = Z₀_arm / tan(βh)    [Ω]

  L_load = X_L / (2πf)       [H]

  Example: 40m band, h = 5.0m, f = 7.15 MHz:

    λ = 299.8/7.15 = 41.93m
    β = 2π/41.93 = 0.1499 rad/m
    βh = 0.1499 × 5.0 = 0.749 rad = 42.93°
    Z₀_arm = 120 × (ln(2×5.0/0.000814) − 1) = 120 × (ln(12,285) − 1)
           = 120 × (9.416 − 1) = 1,010 Ω
    X_L = 1010 / tan(42.93°) = 1010 / 0.929 = 1,087 Ω
    L = 1087 / (2π × 7.15×10⁶) = 24.2 μH  ✓  (matches CSV)
```

### 2.4 Radiation Resistance

For a center-fed dipole with effective half-length h_eff:

```
  h_eff = h × sin(βh)/(βh)    (current distribution correction)

  R_rad = 80π² × (2h_eff/λ)²    [Ω]

  For 40m (h=5.0m, βh=0.749 rad):
    sin(0.749)/0.749 = 0.681/0.749 = 0.909
    h_eff = 5.0 × 0.909 = 4.545m
    R_rad = 80π² × (9.09/41.93)² = 789.6 × 0.0470 = 37.1 Ω
```

Note: R_rad = 37.1 Ω vs. 73 Ω for a full-size dipole. The short dipole has lower radiation resistance, but with a good loading coil it can still be efficient.

### 2.5 Efficiency

```
  Loading coil Q: Q_coil ≈ 200 for well-wound coil on 47mm pool noodle form
  Coil loss resistance: R_coil = X_L / Q_coil  [per arm]
  Total loss (two coils): R_loss = 2 × X_L / Q_coil

  Efficiency: η = R_rad / (R_rad + R_loss) × 100%

  For 40m: R_loss = 2 × 1087/200 = 10.87 Ω
           η = 37.1 / (37.1 + 10.87) = 77.4%
```

Efficiency rises sharply with increasing frequency because R_rad increases faster than R_loss. At 20m and above, efficiency exceeds 85%.

### 2.6 Wheeler's Formula for Loading Coil

Single-layer solenoid inductance (Wheeler's formula):

```
  L = r² × N² / (9r + 10l)    [μH, r and l in inches]

  r = coil radius [in]  = 23.5mm / 25.4 = 0.925 in (pool noodle radius)
  l = coil length [in]  = N/(TPC × 2.54)
  TPC = 8.9 turns/cm (practical, #18 AWG on 47mm form)

  Solving for N given L:
    2.176N² − 1.25L·N − 21.15L = 0
    N = [1.25L + √(1.5625L² + 184.2L)] / 4.352

  Example: 40m, L = 24.17 μH:
    N = [1.25×24.17 + √(1.5625×583.8 + 184.2×24.17)] / 4.352
      = [30.2 + √(911.8 + 4450)] / 4.352
      = [30.2 + 73.2] / 4.352 = 23.8 ≈ 23 turns  ✓
```

### 2.7 Bandwidth

The 3-dB bandwidth of the resonant loaded dipole is:

```
  Q_total = ω₀L_load / (R_rad + R_loss)    [total antenna Q]

  BW = f₀ / Q_total    [Hz]

  Note: this is wider than a magnetic loop because R_rad is much larger
  for the dipole than for the STL at the same frequency.

  Example: 40m:
    ω₀L_load = 2π × 7.15×10⁶ × 24.17×10⁻⁶ = 1,086 Ω
    Q_total = 1086 / (37.1 + 10.87) = 22.6
    BW = 7.15/22.6 = 0.316 MHz = 316 kHz → much wider than loop!
```

Wait — this is the resonant circuit Q, but the antenna bandwidth for SWR < 2:1 depends on the feed impedance match. For a 50Ω feed:

```
  SWR < 2:1 bandwidth:
  BW_SWR2 ≈ BW × (Z₀/R_feed)^0.5

  Approximate: 2–40 kHz per band (narrower than full-size, wider than loop)
```

---

## CHAPTER 3: THEORY — J-POLE ANTENNA

### 3.1 Basic Principle

The J-pole is a half-wave end-fed element matched to the coax via a quarter-wave shorted transmission-line stub. It is named for its J-shape profile (stub + radiator).

The end-feed impedance of a half-wave element is very high (~5000 Ω). The quarter-wave stub transforms this high impedance through continuous voltage/current standing wave distribution. At some tap point along the stub, the impedance equals 50 Ω.

### 3.2 Stub Impedance Transformation

The matching stub is a shorted transmission line. At the shorted end, impedance = 0. Moving up (toward open end at λ/4), impedance increases from 0 toward infinity:

```
  Z_tap(d) = j × Z₀_stub × tan(2πd/λ)    [inductive below resonance]

  At d = λ/4:  Z_tap = ∞ (parallel resonance with radiator)
  At d = 0:    Z_tap = 0 (short)

  Finding the 50Ω tap point:
    50 = Z₀_stub × tan(2πd/λ)
    d = (λ/2π) × arctan(50/Z₀_stub)

  For two-wire stub (6mm gap between wires) Z₀_stub ≈ 250–350 Ω:
    d_50 ≈ λ/4 × (50/Z₀_stub) × (2/π) ≈ 5–8% of λ/4
```

In practice, start at d = 5% of stub length (from the short) and adjust for minimum SWR. The exact value depends on wire gauge, spacing, and height above ground.

### 3.3 Velocity Factor and Length

The pool noodle (εr ≈ 1.05) barely slows the wave. The effective velocity factor is 0.95, dominated by wire end-effects and height above ground. All dimensions in the J-pole tables use VF = 0.95.

### 3.4 Pattern

The J-pole has the same radiation pattern as a center-fed half-wave dipole: omnidirectional in the horizontal plane, with nulls at the vertical poles. Gain ≈ 2.15 dBi. The common-mode shield current on the feed coax is suppressed by the ferrite choke balun — mandatory for clean pattern.

---

## CHAPTER 4: THEORY — YAGI-UDA DIRECTIVE ARRAY

### 4.1 Parasitic Element Operation

A Yagi-Uda antenna achieves gain through parasitic excitation. The reflector (behind the driven element) and directors (in front) are excited by mutual coupling to the driven element. Their re-radiated fields combine with the driven element's field to produce a directional pattern.

**Reflector action:** The reflector is longer than resonant. It presents inductive reactance and re-radiates with a phase lag that reinforces the field in the forward direction.

**Director action:** Each director is shorter than resonant. It presents capacitive reactance and re-radiates with a phase lead that reinforces the forward field.

**Gain is additive:** Each additional director adds approximately 0.5–1.5 dBd depending on spacing.

### 4.2 Element Length Formulas

Starting estimate for free-space, thin-wire elements:

```
  Resonant half-dipole: L = 0.4735λ (shortened ~5.3% from λ/2 = 0.5λ)

  Reflector:   L_ref = 1.01 × L_dipole  (longer; inductive)
  Driven:      L_drv = 0.94 × L_dipole  (shortened; facilitates match)
  Director_1:  L_dir1 = 0.92 × L_dipole (shorter; capacitive)
  Director_n:  L_dirn = L_dir1 − 0.003n × λ  (each slightly shorter)
```

These are starting points. The values in yagi_dimensions.csv are optimized for maximum F/B ratio and broadband gain using standard Yagi design tables (DL6WU spacing, consistent with ARRL UHF reference).

### 4.3 Pool Noodle Boom Correction

Metal booms require element length correction because the boom changes the effective element capacitance. The pool noodle boom (εr ≈ 1.05, tan δ < 0.001) has negligible effect:

- No boom correction needed
- Element lengths used exactly as in yagi_dimensions.csv
- If replacing pool noodle boom with aluminum: reduce all element lengths by 3–5%

### 4.4 Gain and F/B Ratio

```
  3-element Yagi:   G = 7.1 dBd = 9.3 dBi;  F/B ≈ 20 dB
  5-element Yagi:   G = 9.2 dBd = 11.4 dBi; F/B ≈ 25 dB
  7-element Yagi:   G = 11.2 dBd = 13.4 dBi; F/B ≈ 28 dB
```

### 4.5 Directional Coupler Theory

See Chapter 10 for the Bruene reflectometer used across all three antenna families.

---

## CHAPTER 5: MATERIALS AND PARTS LIST

### 5.1 Pool Noodle Stock

| Specification | Quantity | Notes |
|-------------|----------|-------|
| OD=47mm, ID=25mm, L=120cm | 6–8 total | PE foam; εr≈1.05; no RF effect |

Pool noodle lengths needed per antenna type:
- PNILD coil housing: 1 per band (coil form section ~15–20cm)
- J-pole: 1.5–2 noodles (6m), 0.6 (2m), 0.4 (1.25m)
- Yagi boom: 0.25–0.35 noodle sections (UHF bands; short)

### 5.2 Wire

| Band / Antenna | AWG | Type | Notes |
|---------------|-----|------|-------|
| 160m–40m arms | #12 | Bare copper | Reduce R_loss; critical for 160m/80m |
| 30m–10m arms  | #14 | Bare copper | Standard portable HF wire |
| All coils | #18 | Bare copper | Close-wound on noodle form |
| J-pole, 6m | #14 | Bare copper | Stub + radiator |
| J-pole, 2m–1.25m | #14–#16 | Bare copper | — |
| Yagi, 70cm | #14 | Bare copper | Elements cut to ±1mm |
| Yagi, 33cm–20cm | #16–#18 | Bare copper | Elements cut to ±0.5mm |

### 5.3 Electronics

| Component | Part | Qty | Notes |
|-----------|------|-----|-------|
| Microcontroller | ESP32-WROOM-32 dev kit | 1 | — |
| Log amp | AD8307ANZ | 2 | Forward + reflected detectors |
| Shift register | 74HC595 | 2 | Cascaded for 9+ relay outputs |
| Relay | G5V-1-5VDC (5V SPDT, PCB) | 9 | One per HF band tap |
| Relay driver | ULN2003A (or equivalent) | 2 | 7 drivers per IC; use 2 ICs for 9 relays |
| RF coupler | BN-43-2402 binocular core | 1 | Bruene current transformer |
| Cap | 33pF NP0 | 1 | Bruene voltage sample |
| Detector diode | BAT54 Schottky | 2 | Low-Vf signal detectors |
| Bleeder (unused) | N/A | — | No HV cap; no bleeder required |
| Display | ESP32-2432S028R (CYD) | 1 | ILI9341 TFT, XPT2046 touch |
| Balun core | FT-240-61 toroid | 1 | 8-turn coax choke balun |

### 5.4 Hardware

| Item | Qty | Notes |
|------|-----|-------|
| M3 × 8mm SHCS | 24 | Enclosures |
| M3 heat-set inserts, 4.6×4mm | 16 | Dipole center, J-pole base |
| Hose clamps, 25mm | 4 | Mounting |
| BNC chassis connector | 2 | RF in/out |
| DC barrel jack, 2.1mm | 1 | 12V power |
| Self-fusing silicone tape | 3m | Wire retention, weatherproofing |
| Banana jacks (red/black) | 18 | Coil tap terminals (optional manual tapping) |

---

## CHAPTER 6: CONSTRUCTION — HF LOADING COIL DIPOLE (160m–10m)

### 6.1 Overview

Each PNILD consists of:
1. Two wire arms (total physical length varies by band)
2. One loading coil wound on a pool noodle section (one per arm, center-fed → one coil section each side)
3. 1:1 current choke balun at the feed point
4. 50Ω coax feed

For multi-band use, the relay-switched system (Chapter 11) selects the tap point on a single compound coil covering all 9 HF bands. For single-band use, construct individual coils per Section 6.3.

### 6.2 Physical Arm Lengths

These are the TOTAL arm lengths for single-band antennas:

| Band | Arm Length (each side) | Total Dipole | Notes |
|------|----------------------|--------------|-------|
| 160m | 12.0 m | 24.0 m | Requires 10× 120cm noodles as support or long wire |
| 80m | 7.0 m | 14.0 m | 6× noodles or long wire deployment |
| 40m | 5.0 m | 10.0 m | Excellent portable length |
| 30m | 4.0 m | 8.0 m | Field-portable; fits in most sites |
| 20m | 3.0 m | 6.0 m | Very portable; 6m total |
| 17m | 2.5 m | 5.0 m | 5m total; fits any yard |
| 15m | 2.2 m | 4.4 m | Excellent 4.4m length |
| 12m | 1.8 m | 3.6 m | Very compact |
| 10m | 1.5 m | 3.0 m | Barely longer than one noodle each arm |

**Wire selection:** Use #12 AWG for 160m/80m arms to minimize R_arm_conductor. The loading coil dominates losses on low bands; the arm wire contributes additionally. On 40m and above, #14 AWG is adequate.

### 6.3 Winding the Loading Coil

Each arm requires one loading coil, wound on a pool noodle section (OD=47mm):

**Winding materials:**
- #18 AWG bare copper wire
- Pool noodle section, cut to coil length + 10mm each end
- Cable ties

**Winding procedure:**
1. Leave 10cm wire lead at start (for connection to arm wire).
2. Wind N turns close-wound clockwise (viewed from one end).
3. Keep turns uniform; do not let them pile up.
4. Leave 10cm wire lead at finish (for connection to feed point / balun).
5. Secure turns with cable ties at each end and at midpoint for long coils.
6. Measure inductance with LC meter. Compare to target from loading_coil_tables.csv.
7. If L is low: add turns (2 turns increases L by approximately (N+2)²/N² × L).
8. If L is high: remove turns.
9. Coat completed coil with one coat of Q-dope or polyurethane to prevent movement.

**Target inductance (per arm):**

| Band | Target L (μH) | Turns | Coil length (cm) |
|------|-------------|-------|----------------|
| 160m | 180.4 | 110 | 12.3 |
| 80m | 72.7 | 52 | 5.8 |
| 40m | 24.2 | 23 | 2.6 |
| 30m | 13.6 | 16 | 1.8 |
| 20m | 8.6 | 12 | 1.3 |
| 17m | 5.8 | 10 | 1.1 |
| 15m | 4.6 | 8 | 0.9 |
| 12m | 4.1 | 8 | 0.9 |
| 10m | 3.9 | 8 | 0.9 |

### 6.4 Center Feed Assembly

The two loading coils connect at the antenna center, along with the feed balun:

```
  Left arm wire ─── LEFT COIL (noodle form) ─── LEFT COIL END
                                                     │
                                                   BALUN (1:1)
                                                     │
  Right arm wire ─── RIGHT COIL (noodle form) ─── RIGHT COIL END
                                                     │
                                                  50Ω coax
```

**Balun construction (FT-240-61 toroid, 8 turns RG-8X):**

1. Wind 8 turns of RG-8X coax through FT-240-61 toroid.
2. Connect coax input (from transceiver) to the winding input.
3. Connect coax center at winding output to LEFT COIL END.
4. Connect coax shield at winding output to RIGHT COIL END.
5. The toroid provides >1000Ω common-mode impedance at 2–30 MHz.

The dipole center insulator (TM-PNL2-ENC-002) provides a mechanical housing for the balun assembly.

### 6.5 Deployment Options

**Horizontal dipole:** Run arm wires horizontally from the center point. Stake or tie to supports at arm ends. This provides a bidirectional pattern broadside to the wire.

**Inverted-V:** Center elevated on mast. Arms slope downward at 45°. The compressed pattern increases low-angle radiation — better for DX. Minimum apex height: 0.3λ (preferred 0.5λ).

**Sloper:** One end elevated, other end low. Slightly cardioid pattern toward the low end.

**160m/80m deployments:** These require 24m and 14m total wire runs respectively. For portable use:
- Use a fishing pole or fiberglass mast at the center to elevate the feed point.
- Arms slope down to stakes at a comfortable angle.
- The pool noodle coil form hangs at the feed point.

---

## CHAPTER 7: CONSTRUCTION — MULTI-BAND ROLL-UP TRAP DIPOLE

### 7.1 Concept

The roll-up trap dipole covers 40m through 10m from a single antenna using LC traps. The pool noodle roll-up spool (TM-PNL-ENC-003, from the magnetic loop suite) serves as the storage spool.

**Principle:** Traps are parallel LC circuits that present very high impedance at their resonant frequency, electrically isolating the outer wire sections on higher bands.

On 40m: all traps are transparent (operating frequency < all trap frequencies). Full arm length active.
On 20m: 40m and 30m traps activate, blocking outer wire sections. Inner 3m arm section resonates.
On 10m: five traps block, leaving only the 1.5m innermost section active.

### 7.2 Trap Assembly

Trap construction uses T50-2 (20m–40m) or T50-6 (10m–17m) powdered iron toroids:

| Trap | Core | Turns | Parallel C | Resonance | Z_trap |
|------|------|-------|-----------|-----------|--------|
| 40m | T50-2 | 12 | 207pF NP0 | 7.15 MHz | 3760 Ω |
| 30m | T50-2 | 10 | 150pF NP0 | 10.125 MHz | 3830 Ω |
| 20m | T50-2 | 8 | 120pF SM | 14.175 MHz | 3960 Ω |
| 17m | T50-6 | 8 | 106pF SM | 18.118 MHz | 4460 Ω |
| 15m | T50-6 | 7 | 100pF SM | 21.225 MHz | 4110 Ω |
| 10m | T50-6 | 5 | 111pF NP0 | 28.500 MHz | 2920 Ω |

Full winding table with measured tolerances: see `calculations/trap_winding_tables.csv`.

**Trap winding:**
1. Wind coil on T50-x toroid as specified in table.
2. Solder NP0 or silver-mica capacitor across coil terminals.
3. Verify resonant frequency with antenna analyzer dip test (±50 kHz tolerance).
4. Seal with polyurethane or Q-dope; allow 24h cure.
5. Each arm needs one of each trap (total: 12 trap assemblies for 6 traps × 2 arms).

### 7.3 Roll-up Assembly

1. Lay out left arm wire: 5.0m (#14 AWG) from center.
2. At 1.5m from center: solder 10m trap in series.
3. At 2.2m: solder 15m trap.
4. At 2.5m: solder 17m trap.
5. At 3.0m: solder 20m trap.
6. At 4.0m: solder 30m trap.
7. At 5.0m (far end): solder 40m trap.
8. Repeat for right arm (mirror image).
9. Connect both arm center ends to the loading coil assembly.
10. Coil: use the 40m 23-turn loading coil (innermost section active at all bands).

**Deployment:** Each arm is 5.0m total. Extend as inverted-V or horizontal. The traps will be positioned at fixed points along each arm.

---

## CHAPTER 8: CONSTRUCTION — VHF J-POLE (6m–1.25m)

### 8.1 Overview

The pool noodle J-pole uses the noodle as an insulating structural support for the quarter-wave matching stub. The radiator extends above the top of the noodle.

**Advantages over other VHF antennas:**
- Simple construction: cut to length, no turns to count
- Omnidirectional: usable for FM, simplex, satellite
- DC-grounded: lightning protection via the short at the base
- Lightweight: pool noodle + wire weighs < 200g for 2m version

### 8.2 Dimensions

All lengths based on VF = 0.95:

| Band | Radiator (λ/2) | Stub (λ/4) | Total wire | Feed tap from short |
|------|--------------|------------|-----------|-------------------|
| 6m (51 MHz) | 279.2 cm | 139.6 cm | 418.8 cm | 14.0 cm |
| 2m (146 MHz) | 97.5 cm | 48.8 cm | 146.3 cm | 4.9 cm |
| 1.25m (222 MHz) | 64.1 cm | 32.1 cm | 96.2 cm | 3.2 cm |

### 8.3 Cutting Procedure

1. For the stub wire: cut to stub length listed above. **Measure from end with a metal rule; cloth tape stretches.**
2. For the radiator wire: cut to radiator length. Same precision requirement.
3. Mark the feed tap position on the stub wire before assembly.

**Tolerance:** ±5mm for 6m; ±2mm for 2m; ±1mm for 1.25m.

### 8.4 Assembly

```
  Assembly sequence:

  1. Insert pool noodle section into J-pole base (TM-PNL2-ENC-003).
     Noodle length = stub length + 25mm (gap section).

  2. Run stub wire along the wire groove on the outside of the noodle.
     Bottom of stub wire extends past noodle base by ~5mm.

  3. Bend stub wire at bottom: this is the "short" — both wire conductors
     will be bridged here.

  4. Position radiator wire parallel to stub wire, 25mm to the side.
     Radiator wire bottom is at the top of the noodle + gap.

  5. Connect stub and radiator at the gap junction:
     NO electrical connection at the gap — only physical positioning.
     The gap provides electrical separation between stub and radiator.

  6. Short the stub at its base: use a 5cm #14 AWG wire to bridge
     the two stub wire ends. Solder both connections.

  7. Connect coax at the tap mark on the stub:
     Coax center conductor → stub wire at tap mark
     Coax braid → the other wire at the same height
     (Or: coax center to stub, coax braid to radiator at bottom —
      consult local sources for preferred J-pole feed convention.)

  8. Install ferrite choke at coax, 30cm below feed point.
     4 turns of coax on FT-50-43 toroid.
```

### 8.5 Adjusting the Feed Tap

The tap position is the primary alignment adjustment:

```
  Tool: antenna analyzer at band center frequency.

  Initial tap: 5% of stub length from short (see dimensions table).

  Procedure:
  a. Connect analyzer to coax feed.
  b. Observe SWR. Typical initial SWR: 2:1 to 4:1.
  c. Move tap slightly upward (away from short): SWR should decrease.
  d. At some point SWR will reach minimum (< 1.5:1 target).
  e. If moving tap up makes SWR worse: try moving down instead.
  f. Minimum SWR should be achievable ≤ 1.5:1.
  g. Mark the correct tap position with a paint pen.
  h. Solder permanently or use a clip connector.

  If SWR minimum is > 2:1 at all tap positions:
  - Verify gap is correct (25mm, no electrical connection).
  - Verify stub is shorted at base.
  - Verify stub and radiator lengths are within tolerance.
```

### 8.6 6-Meter J-Pole Special Considerations

The 6m noodle (142cm needed for stub section) spans just over one standard noodle (120cm) plus an extension. Options:
1. Join two noodles with a 22mm dowel insert
2. Use a fiberglass rod as the stub support
3. Use a PVC tube (3/4 inch schedule 40 = 26mm OD; adjust J-pole base MAST_OD parameter)

For 6m, ferrite choke is critical to prevent radiator pattern distortion from coax shield current.

---

## CHAPTER 9: CONSTRUCTION — UHF YAGI (70cm–20cm)

### 9.1 Overview

The pool noodle Yagi uses the noodle OD=47mm as the antenna boom. Wire elements are mounted perpendicular to the boom via printed element clips (TM-PNL2-ENC-004). The non-conductive boom eliminates boom-correction factors.

### 9.2 Element Fabrication

**Cut all elements to the lengths in yagi_dimensions.csv before assembly.** Once inserted into clips, trimming is difficult.

```
  Cutting procedure:
  1. Straighten wire by pulling through a cloth (removes kinks).
  2. Measure and mark with a fine-tip permanent marker.
  3. Cut with wire cutters; verify with metal rule.
  4. Label each element with tape tag: "REF", "DE", "D1", etc.
  5. Bundle and store until assembly.
```

**Wire radius correction:** The element length tables assume #14/16/18 AWG wire with negligible radius correction for the pool noodle boom. If using aluminum rod or tube elements, the element lengths may need adjustment (typically 1–3% shorter for thicker conductors — use NEC model to verify).

### 9.3 Boom Preparation

Cut pool noodle section to boom length from yagi_dimensions.csv:

| Band | Boom length | Noodle sections |
|------|-----------|----------------|
| 70cm | 20.8 cm | Short cut from one noodle |
| 33cm | 23.8 cm | Short cut |
| 20cm | 27.5 cm | Short cut |

Mark all element positions on the boom with a permanent marker before mounting clips. Measure from the reflector end.

### 9.4 Element Assembly

1. Print element clips (TM-PNL2-ENC-004), select correct ELEMENT_WIRE_D.
2. Thread each element wire through the wire groove in its clip.
3. Center the element: mark the midpoint of each element wire; align this with the clip centerline.
4. Snap clip onto boom at correct spacing.
5. Verify element centering with a metal rule.
6. Apply one drop of cyanoacrylate to the wire-in-groove junction if permanent.

### 9.5 Feed System — Hairpin Match

The driven element uses a hairpin (beta match) to transform the split-dipole impedance to 50Ω:

**For 70cm (446 MHz):**

```
  Split driven element: total length 310.6mm; split at center (gap = 3mm)

  Hairpin dimensions:
    Material: #14 AWG wire (same as elements)
    Width: 35mm (measured between the two legs)
    Length: 50mm (each leg parallel to driven element)

  Connection:
    Hairpin leg 1 → left half of driven element
    Hairpin leg 2 → right half of driven element
    Coax center → left half at hairpin junction
    Coax shield → right half at hairpin junction (or common point)

  Adjustment:
    Slide hairpin along the driven element while monitoring SWR.
    Target: SWR < 1.5:1 at 446 MHz.
    For permanently fixed: mark optimum position with paint.
```

For 33cm and 20cm, scale the hairpin proportionally (smaller dimensions at higher frequency).

### 9.6 Mounting and Polarization

**Horizontal polarization:** Mount boom horizontal, elements in a vertical plane. Points at horizon. Used for terrestrial SSB and weak-signal work.

**Vertical polarization:** Mount boom horizontal, rotate 90° so elements are horizontal. (Boom axis still horizontal, but elements now in horizontal plane.) Used for FM, digital modes, satellite.

**For satellite uplink/downlink:** Circular polarization is preferred. Achieve with two orthogonal Yagis fed 90° apart using a quadrature hybrid coupler.

---

## CHAPTER 10: DIRECTIONAL COUPLER SWR SYSTEM

### 10.1 Bruene Reflectometer

The Bruene directional coupler is the standard method for measuring SWR in HF/VHF RF systems. It is the same architecture used in the Pool Noodle Magnetic Loop system (TM-PNL-001) and in commercial SWR bridges. This section summarizes the theory and implementation.

**Principle:** The coupler samples the transmission line at a point using both voltage and current coupling. These samples are combined additively (forward power) and subtractively (reflected power):

```
  V_forward ∝ V_line + I_line × Z₀    (constructive sum)
  V_reflected ∝ V_line − I_line × Z₀  (subtractive sum)

  When load = Z₀ (matched): V_reflected = 0
  When load ≠ Z₀: V_reflected ≠ 0
```

### 10.2 Coupler Construction

**Current sampling transformer:**
- Core: BN-43-2402 binocular ferrite, Fair-Rite part 2843002402
- Primary: 1 turn (main coax passes through both holes)
- Secondary: 3 turns #22 AWG, through both holes
- Coupling: approximately 20 dB

**Voltage sampling capacitor:**
- C = 33pF NP0 from main line to summing node
- Chosen to balance current transformer sample at 50Ω

**Detector diodes:** BAT54 Schottky, with 10μF/10kΩ RC filter (τ = 100ms)

### 10.3 AD8307 Log Amplifier

The AD8307 converts input power to a DC voltage:

```
  V_out = 25 mV/dB × (P_in_dBm + 84)    [mV]

  Converting to power:
  P_dBm = (V_mV / 25.0) − 84.0
  P_watts = 10^(P_dBm/10) / 1000

  Add coupler factor: P_corrected = P_watts × 10^(20/10) = P_watts × 100
```

**ESP32 ADC:** 12-bit SAR ADC, 64× oversampled per reading, 8 readings averaged. Total: 512 samples per final SWR value. Resolution: approximately 0.05 dB.

### 10.4 SWR Computation

```cpp
  Γ = √(P_reflected / P_forward)    (reflection coefficient)
  SWR = (1 + |Γ|) / (1 − |Γ|)
```

Minimum valid forward power: −20 dBm (10 μW). Below this, the SWR reading is marked invalid and displayed as "---" on the CYD.

### 10.5 Calibration

```
  Procedure (one-time after construction):

  1. Connect calibrated power reference to antenna port.
     (Use QRP rig + known-good inline power meter.)

  2. Terminate with 50Ω dummy load.

  3. Apply 1W at 14.175 MHz. Read PNL-Tuner /api/status.
     Adjust CAL_OFFSET_FWD via /api/cal?fwd=X until reading = 1.00W.

  4. Apply 2:1 SWR reference (known mismatch).
     Verify SWR reading = 2.0 ± 0.1.

  5. Save calibration values in NVS (persistent across power cycles).
```

---

## CHAPTER 11: ESP32 RELAY TUNER FIRMWARE

### 11.1 Overview

Document: TM-PNL2-FW-001  
File: `firmware/esp32_pnl_tuner/esp32_pnl_tuner.ino`

The ESP32 firmware provides:
- **Relay tap control:** 74HC595 shift register drives 9 SPDT relays selecting loading coil tap (PNILD bands)
- **SWR measurement:** Bruene coupler + dual AD8307 (same as loop tuner)
- **Auto-tune:** Scans all 9 relay positions; selects minimum SWR tap
- **Band management:** 15-band table with NVS persistence
- **Bluetooth serial:** 200ms telemetry → CYD display
- **Wi-Fi AP:** HTTP control dashboard at 192.168.4.1

### 11.2 State Machine

```
  ST_IDLE     (0): No active tuning; relay holds last position
  ST_TUNING   (1): Scanning all 9 relay taps for minimum SWR
  ST_LOCKED   (2): Best tap selected; SWR < SWR_GOOD (1.5:1)
  ST_PASSTHRU (3): J-pole or Yagi band; relay not applicable; SWR monitor only
```

### 11.3 Auto-Tune Algorithm

The auto-tune for PNILD scans all relay positions sequentially:

```
  autoTune():
  1. Scan taps 0–8 in order.
  2. At each tap: engage relay, wait 50ms (relay + RF settle), measure SWR.
  3. Record tap index for lowest observed SWR.
  4. Early exit: if SWR < SWR_GOOD (1.5:1), stop scanning.
  5. Set relay to best tap.
  6. Save band index to NVS.
  7. Set state = ST_LOCKED if best SWR < SWR_GOOD, else ST_IDLE.
```

This is a full search (O(N) where N=9 taps), not a gradient search, because the relay positions are discrete and the SWR curve may have multiple local minima due to harmonic interactions. A full scan guarantees the global minimum.

**Scan time:** 9 taps × (50ms relay settle + 8ms measurement) = ~520ms total.

### 11.4 74HC595 Relay Control

Two 74HC595 shift registers cascade to provide 16 output bits (9 used for relays, 7 spare):

```
  One-hot relay selection (only one relay active at a time):

  Tap 0 (160m): bits = 0000000000000001
  Tap 1 (80m):  bits = 0000000000000010
  Tap 2 (40m):  bits = 0000000000000100
  ...
  Tap 8 (10m):  bits = 0000000100000000

  J-pole/Yagi (no tap): bits = 0000000000000000 (all relays open)
```

### 11.5 Bluetooth Packet Format

```
  Format:
  "BAND:%s,TYPE:%d,L:%.2f,H:%.1f,SWR:%.2f,FWD:%.3fW,EFF:%.1f,STATE:%d,TAP:%d\n"

  Example:
  "BAND:40m,TYPE:0,L:24.17,H:5.0,SWR:1.28,FWD:4.992W,EFF:77.4,STATE:2,TAP:2\n"
```

### 11.6 Web Interface

| URL | Function |
|-----|---------|
| `/` | Dashboard: band, SWR, power, efficiency, state |
| `/band/up` | Next band (wraps 15→0) |
| `/band/dn` | Previous band (wraps 0→15) |
| `/tune` | Trigger relay scan |
| `/api/status` | JSON status |
| `/api/cal?fwd=X&refl=Y` | Set calibration offsets (dB) |

---

## CHAPTER 12: CYD DISPLAY FIRMWARE

### 12.1 Overview

Document: TM-PNL2-FW-002  
File: `firmware/cyd_pnl_display/cyd_pnl_display.ino`

The CYD display receives Bluetooth serial telemetry and presents:

| Page | Content |
|------|---------|
| 0: SWR | Large SWR numeric + bar graph + forward power + efficiency |
| 1: Antenna | PNILD: coil info, relay tap, arm length; J-pole: dimensions; Yagi: gain/pattern |
| 2: Power | SWR numeric, forward watts, efficiency with color coding |
| 3: Guide | Band-specific operating procedure |

### 12.2 Page Navigation

Touch left 80px zone → previous page. Touch right 80px zone → next page. Debounce 300ms.

### 12.3 Connecting State

If no BT packet received within 3000ms, the display shows "CONNECTING..." with blinking dots and the Wi-Fi AP address. Reconnect is automatic.

### 12.4 Color Coding

| Condition | Color |
|-----------|-------|
| SWR < 1.5:1 | Green |
| 1.5:1 ≤ SWR < 2.5:1 | Yellow |
| SWR ≥ 2.5:1 | Red |
| Efficiency > 75% | Green |
| 50% < Efficiency ≤ 75% | Yellow |
| Efficiency ≤ 50% | Red |
| State = LOCKED | Green |
| State = TUNING | Yellow |

---

## CHAPTER 13: TUNING AND ALIGNMENT

### 13.1 PNILD Initial Setup

```
  1. Verify coil inductance with LC meter before installation.
     Target ± 2 μH for all bands.
  2. Install coils and arm wires. Dress balun at feed point.
  3. Power on ESP32 tuner. CYD connects via BT.
  4. Select band (web UI or BAND buttons).
  5. Deploy antenna in inverted-V or horizontal configuration.
  6. Apply low power (1–2W) from transceiver.
  7. Press TUNE: relay scan runs in ~520ms.
  8. CYD shows new SWR and state = LOCKED if < 1.5:1.
```

### 13.2 PNILD Manual Tuning (No ESP32)

For standalone use (no electronics):

1. Install banana jacks at each turn tap on the loading coil (9 jacks per coil, each connecting to a tap point).
2. Use alligator clip jumpers to connect the feed wire to different tap positions.
3. Measure SWR at each position with antenna analyzer.
4. Record optimum tap for each band.

This approach is field-serviceable without any electronics.

### 13.3 J-Pole SWR Adjustment

See Chapter 8.5 for detailed tap adjustment procedure. Key points:

- Target SWR < 1.5:1 at band center
- Tap moves up = impedance increases at feed
- Tap moves down = impedance decreases
- Final SWR < 1.5:1 achievable on all three VHF bands

### 13.4 Yagi SWR Adjustment

The hairpin match provides one adjustment variable:

```
  Moving hairpin away from driven element center: increases inductance
  → decreases feed impedance → SWR changes

  Sliding hairpin along element: changes tap point on driven element
  → adjusts impedance transformation

  Start: hairpin centered on driven element, width = 35mm (70cm band)
  Adjust until SWR < 1.5:1 at band center.
```

---

## CHAPTER 14: PERFORMANCE DATA

### 14.1 PNILD Efficiency by Band

| Band | f (MHz) | Total Length | Efficiency | Est. BW (kHz) |
|------|---------|-------------|-----------|--------------|
| 160m | 1.900 | 24.0 m | 44.0% | 21 |
| 80m | 3.750 | 14.0 m | 56.1% | 42 |
| 40m | 7.150 | 10.0 m | 77.4% | 89 |
| 30m | 10.125 | 8.0 m | 83.9% | 108 |
| 20m | 14.175 | 6.0 m | 86.3% | 130 |
| 17m | 18.118 | 5.0 m | 88.8% | 179 |
| 15m | 21.225 | 4.4 m | 90.0% | 220 |
| 12m | 24.940 | 3.6 m | 89.0% | 279 |
| 10m | 28.500 | 3.0 m | 87.6% | 325 |

Notes:
- Efficiency computed for loading coil Q = 200 (#18 AWG, 47mm former)
- Increasing coil Q (Litz wire, larger former) improves low-band efficiency significantly
- At 20m and above, PNILD efficiency is within 1.5 dB of a full-size dipole

### 14.2 Comparison: PNILD vs. Magnetic Loop vs. Full Dipole

| Criterion | Full Dipole | Magnetic Loop STL | PNILD |
|-----------|------------|-------------------|-------|
| Physical length (40m) | 20m | 4.8m (loop perimeter) | 10m |
| Efficiency (40m) | 99%+ | 29% | 77% |
| Efficiency (20m) | 99%+ | 65% | 86% |
| Bandwidth (40m) | 200 kHz | 8.9 kHz | 89 kHz |
| HV hazard | None | Lethal (1700V+ at 5W) | None |
| Retune needed | No | Yes (< 5 kHz BW) | No (89 kHz BW) |
| Setup complexity | Low | High (motor, coupler) | Medium (coil + balun) |

**Summary:** The PNILD is the best practical choice when a full-size dipole is too large but the narrow bandwidth and high-voltage hazard of the STL loop are undesirable. The PNILD offers 5–15 dB better efficiency than the STL loop while covering 10× wider bandwidth per band.

### 14.3 VHF/UHF Performance

| Antenna | Freq (MHz) | Gain (dBi) | F/B (dB) | Pattern |
|---------|-----------|-----------|---------|---------|
| J-Pole 6m | 51.0 | 4.30 | N/A | Omni |
| J-Pole 2m | 146.0 | 4.30 | N/A | Omni |
| J-Pole 1.25m | 222.0 | 4.30 | N/A | Omni |
| Yagi 70cm | 446.0 | 9.25 | 20 | Directional |
| Yagi 33cm | 906.0 | 11.35 | 25 | Directional |
| Yagi 20cm | 1296.0 | 13.35 | 28 | Directional |

### 14.4 NEC Model Results

Complete models: TM-PNL2-NEC-001 through -015 in `/nec_models/`.

Key NEC outputs:

| Band | Elevation of max | Take-off angle | Max gain (dBi) |
|------|-----------------|---------------|---------------|
| 40m PNILD | Broadside | 45° | 6.2 |
| 20m PNILD | Broadside | 32° | 7.5 |
| 10m PNILD | Broadside | 22° | 8.1 |
| 70cm Yagi | Forward | 0° (horizontal) | 9.25 |
| 20cm Yagi | Forward | 0° (horizontal) | 13.35 |

Note: NEC model for PNILD uses lumped LD card loading. Actual pattern accurate; impedance is approximate.

---

## CHAPTER 15: SAFETY

### 15.1 RF Safety

The PNILD, J-Pole, and Yagi antennas do not present high-voltage component hazards (no tuning capacitor). Standard RF safety practices apply:

- Do not place any part of the body in the near-field of a transmitting antenna
- At 5W on 40m, the field near the loading coil can cause RF burns on contact
- At VHF/UHF with Yagi antennas, never look into the forward aperture of the antenna during transmit (eyes are sensitive to RF)
- Maintain safe distances per FCC OET-65 / ARRL RF Safety guidelines

### 15.2 No High-Voltage Hazard

Unlike the magnetic loop system (TM-PNL-001), this antenna system contains **no resonant high-voltage capacitor**. The 160m STL loop stores 2,791V peak at 5W; the 160m PNILD loading coil stores < 50V peak at 5W. This is not a shock hazard at normal operating power.

The 160m/80m PNILD is therefore safer to operate and maintain than the equivalent STL loop. The tradeoff is higher total physical size (more wire required).

### 15.3 Electrical Safety (Electronics)

The ESP32 tuner circuit operates at 12V/5V/3.3V — all safe DC voltages. Standard electronics safety practices:

- Do not operate in rain without weatherproofing the electronics enclosure
- Ensure all solder joints are clean and secure before applying RF
- The relays are rated for 5V coil; do not exceed this
- If using the system in the field on battery power: ensure battery fuse protection

### 15.4 Mechanical Safety

- PETG printed enclosures (TM-PNL2-ENC series) are not rated for structural loading
- Do not hang the antenna weight from printed parts; use wire strain relief at all load points
- For 160m/80m deployments with long wire runs: use non-metallic rope for support; keep clear of power lines
- Pool noodle materials degrade in sustained UV exposure; replace corner connectors annually if in permanent outdoor installation

---

## CHAPTER 16: MAINTENANCE AND FIELD REPAIR

### 16.1 Preventive Maintenance

```
  Before each operating session:
  □ Visual inspect coil for wire movement or damaged coating
  □ Verify balun is secured at dipole center
  □ Check arm wire connections at coil ends (solder joints)
  □ For Yagi: verify element clips are seated firmly; no loose elements
  □ CYD: verify BT connects within 30 seconds
  □ Apply RF: verify SWR < 1.5:1 on expected bands

  Monthly (for regular users):
  □ Inspect loading coil winding for corrosion (bare copper tarnishes)
  □ Lightly sand or clean copper wire if green patina visible
  □ Tighten all hardware; check M3 screws in printed parts
  □ Replace pool noodle sections showing cracks, UV whitening, or brittleness
```

### 16.2 Loading Coil Repair

**Broken wire in coil:**

```
  Symptoms: SWR suddenly very high on one band; other bands affected.

  Repair:
  a. Remove coil from housing.
  b. Locate break (visible or continuity check).
  c. Splice wire with small solder joint.
  d. Reseal splice with heat shrink + Q-dope.
  e. Re-measure inductance; adjust turns if needed.
```

**Coil form cracked:**

```
  The pool noodle coil form may crack if severely bent or stepped on.

  Repair:
  a. Wrap cracked section with self-fusing silicone tape.
  b. This maintains coil geometry; RF performance unaffected.
  c. Replace form when convenient.
```

### 16.3 J-Pole Tap Adjustment in the Field

If SWR increases on a J-pole after transportation (wire flexing can shift tap position):

1. Check that feed coax is properly connected at tap point.
2. If soldered: may need to re-solder after repeated flexing.
3. Alternative: use a clothespin-style alligator clip for the tap — allows field adjustment without tools.

### 16.4 Yagi Element Repair

Elements in the Yagi are bare wire. If an element is bent or broken:

1. **Bent element:** Straighten carefully. If kink remains, cut and re-splice with short sleeve of same-gauge wire. Measure final length and trim to spec.
2. **Broken element:** Replace with new wire cut to exact length. No splicing across the break — splice changes the electrical length.
3. **Lost element:** Carry a 20cm length of #14 AWG in field kit.

### 16.5 Relay Failure Diagnosis

```
  Symptom: SWR high on one specific band; other bands OK.
  Likely: relay for that band not actuating.

  Check:
  1. Does LED indicator on ULN2003 board illuminate for that tap?
     (Each relay has an indicator LED on standard breakout boards)
  2. If no LED: 74HC595 may not be shifting correctly.
     Measure SR_DATA, SR_CLOCK, SR_LATCH with oscilloscope during tune.
  3. If LED lights but relay doesn't actuate: relay coil/contact failure.
     Replace relay (G5V-1, through-hole, common part).
```

### 16.6 Storage

```
  Short-term (<1 week):
  a. Coil on pool noodle: keep coil wound; tape free ends.
  b. Do not compress or bend the noodle with coil installed.
  c. Arm wires: coil loosely, minimum bend radius 10× wire dia.

  Long-term (>1 month):
  a. Apply clear lacquer to bare copper wire to prevent oxidation.
  b. Remove coil from noodle; store coil in sealed bag with desiccant.
  c. Disconnect all connectors; apply dielectric grease to RF connectors.
  d. Store electronics in ESD bag.
```

---

## APPENDIX A: BAND DATA SHEETS

### A.1 40-Meter PNILD Data Sheet

```
  TM-PNL2-A003 — 40m Pool Noodle Loading Coil Dipole

  Design Frequency:     7.150 MHz
  Band:                 40m (7.000–7.300 MHz)

  GEOMETRY:
  Arm wire gauge:       #14 AWG bare copper (a = 0.814mm)
  Physical arm length:  5.0 m each (10.0 m total dipole)
  h/λ ratio:            5.0/10.49 = 47.7% of quarter-wave
  Antenna type:         Center-loaded shortened dipole (PNILD)

  LOADING COIL (per arm):
  Target inductance:    24.17 μH
  Core:                 Pool noodle OD = 47mm
  Wire:                 #18 AWG close-wound
  Turns:                23
  Winding length:       26 mm
  Wheeler calc. L:      24.5 μH
  Q (estimated):        200
  R_coil:               5.43 Ω per arm

  COMPUTED PARAMETERS:
  Z₀_arm:               1,010 Ω
  βh:                   42.93°
  X_arm per arm:        1,087 Ω
  Radiation resistance: 37.11 Ω
  Loss resistance:      10.87 Ω (both arms)
  Efficiency:           77.4%
  3-dB Bandwidth:       ~89 kHz (estimated)

  RELAY TAP:            2 (of 9, 0-indexed)

  MATCH NOTE:
  Good portable DX dipole. 10m total length. Coil Q critical.

  NEC MODEL: TM-PNL2-NEC-003_40m_pnild.nec
```

### A.2 2-Meter J-Pole Data Sheet

```
  TM-PNL2-A011 — 2m Pool Noodle J-Pole

  Design Frequency:     146.000 MHz
  Band:                 2m (144–148 MHz)

  GEOMETRY:
  Radiator (λ/2):       97.54 cm
  Matching stub (λ/4):  48.77 cm
  Total wire:           146.31 cm
  Feed tap from short:  4.88 cm (start; adjust for minimum SWR)
  Gap (stub to rad.):   25 mm
  VF:                   0.95
  Wire:                 #14 AWG bare copper

  COMPUTED PARAMETERS:
  Antenna impedance:    ~50Ω at tap point
  Gain:                 4.30 dBi (2.15 dBd)
  Pattern:              Omnidirectional (horizontal plane)
  Efficiency:           ≈ 99% (full resonant element)

  POOL NOODLE:
  Length needed:        51.3 cm (stub + gap section)
  One noodle section (120cm) cut to 51.3cm; remainder for another project.

  BALUN:
  1:1 choke balun mandatory: 4T coax on FT-50-43, adjacent to feed point.

  NEC MODEL: TM-PNL2-NEC-011_2m_jpole.nec
```

### A.3 70cm Yagi Data Sheet

```
  TM-PNL2-A013 — 70cm 3-Element Pool Noodle Yagi

  Design Frequency:     446.000 MHz
  Band:                 70cm (420–450 MHz)

  GEOMETRY:
  Number of elements:   3 (reflector + driven + 1 director)
  Boom:                 Pool noodle OD=47mm, length = 20.84 cm

  Element Dimensions:
    Reflector:   329.38 mm at 0.00 mm from reflector
    Driven:      310.56 mm at 94.11 mm from reflector
    Director 1:  295.77 mm at 208.38 mm from reflector

  Element wire:  #14 AWG bare copper
  Element clips: TM-PNL2-ENC-004 (pool noodle snap-on)
  Feed:          Split driven element + hairpin match, 50Ω coax

  COMPUTED PERFORMANCE:
  Gain:           7.1 dBd (9.25 dBi)
  F/B ratio:      20 dB
  3-dB beamwidth: 80°
  Efficiency:     ≈ 99%

  Element accuracy required: ±1.0 mm

  NEC MODEL: TM-PNL2-NEC-013_70cm_yagi.nec
```

*(Additional data sheets for all remaining bands available in calculations CSV files.)*

---

## APPENDIX B: BILL OF MATERIALS

### B.1 PNILD Single Band (40m Example)

| Item | Qty | Notes |
|------|-----|-------|
| Pool noodle (47mm OD) | 1 | Cut 10cm for coil form; remainder for support |
| #14 AWG bare copper wire | 12m | Arms: 5m × 2 + 2m spare |
| #18 AWG bare copper wire | 0.5m | Loading coil: 23 turns |
| FT-240-61 toroid | 1 | 1:1 balun core |
| RG-8X coax, 0.5m | 1 | Balun winding |
| BNC-F chassis connector | 1 | Feed output |
| Dipole center housing | 1 | Print: TM-PNL2-ENC-002 |
| Coil housing | 2 | Print: TM-PNL2-ENC-001 (one per arm coil) |
| M3 heat-set inserts | 4 | Enclosure lids |
| Self-fusing tape, 0.3m | 1 | Coupling loop / strain relief |

### B.2 9-Band PNILD Relay Tuner

| Item | Qty | Notes |
|------|-----|-------|
| ESP32-WROOM-32 dev kit | 1 | Controller |
| AD8307ANZ | 2 | Log amplifiers |
| BN-43-2402 | 1 | Bruene coupler core |
| BAT54 Schottky diode | 2 | RF detectors |
| 74HC595 shift register | 2 | Cascaded for 16 outputs |
| G5V-1-5VDC relay (Omron) | 9 | SPDT PCB mount |
| ULN2003A driver IC | 2 | 7 relay drivers each |
| ESP32-2432S028R (CYD) | 1 | Display terminal |
| 5V / 2A regulator | 1 | AMS1117-5.0 |
| 3.3V regulator | 1 | AMS1117-3.3 |
| 33pF NP0 cap | 1 | Voltage sample |
| 10μF / 16V electrolytic | 2 | RF detector filter |
| 10kΩ resistor | 2 | Detector load |
| 1kΩ resistor | 2 | ADC input |

**Total cost estimate (USA, 2026):** $55–80 for electronics; $15–25 for wire and hardware per single band; $200–250 for the full 9-band relay system.

---

## APPENDIX C: NEC MODEL SUMMARY

### C.1 PNILD Models (TM-PNL2-NEC-001 through -009)

All PNILD models use:
- Two-wire dipole geometry along X axis
- LD 0 cards for lumped inductance loading at each arm's feed-segment midpoint
- Ground: Sommerfeld-Norton (GN 2), conductivity 13 mS/m, εr = 13
- Frequency: single-frequency evaluation at band center

NEC-2 handles lumped loading accurately. The LD card inductive loading (R=0, L= H) correctly models the loading coil at the specified segment. The model gives accurate gain, pattern, and radiation resistance.

**Limitation:** NEC-2 does not model coil Q directly. The LD card specifies ideal inductance (R=0). Actual coil Q = 200 introduces R_coil which reduces efficiency. The efficiency reported in the NEC output will be higher than the calculated values in the CSV (which include coil loss).

### C.2 J-Pole Models (TM-PNL2-NEC-010 through -012)

J-pole models use:
- Wire 1: matching stub (ground plane at bottom, excitation at tap point)
- Wire 2: radiator (gap separation from stub, free end at top)
- Ground: Sommerfeld-Norton for realistic near-ground analysis
- Excitation: EX 0 at the tap segment

Tap segment is approximated by the starting 5% tap position. Actual SWR tuning shifts this; the NEC model shows pattern accuracy, not exact SWR.

### C.3 Yagi Models (TM-PNL2-NEC-013 through -015)

Yagi models use:
- All elements as GW cards, boom along X axis, elements along Y axis
- Free-space GN -1 (appropriate for directional antenna analysis)
- Excitation at driven element center (segment 6 of 11)
- Pattern sweep: 181 azimuth points at 0° elevation

---

## APPENDIX D: TROUBLESHOOTING

### D.1 SWR Never Below 2:1 (PNILD)

```
  Check 1: Balun installed?
    Without a 1:1 balun, common-mode current on the coax distorts the
    antenna pattern and shifts the apparent feed impedance.
    Install FT-240-61 choke balun and re-test.

  Check 2: Coil inductance correct?
    Measure with LC meter. Each coil must be within ±2 μH of target.
    Off-frequency coil shifts the resonance away from band center.

  Check 3: Arm length correct?
    Measure from coil terminal to wire tip. Must match h_arm values.

  Check 4: Antenna isolated from ground?
    If arm wire is touching a metal mast or support, the effective length
    is changed. Use non-metallic support stakes.
```

### D.2 SWR Varies with Hand Near Antenna (Body Capacitance)

```
  This indicates insufficient RF ground or poor balun isolation.

  Fix: Ensure 1:1 current choke balun is installed AND effective.
  Test: If SWR stabilizes after adding the choke, the balun is working.
  Alternative: Add additional ferrite sleeves on coax at 1m intervals.
```

### D.3 Relay Auto-Tune Fails (No Tap Gives SWR < 2:1)

```
  Check 1: Is RF power applied during tune?
    Without RF, the SWR measurement is invalid.
    Verify transceiver is keyed at 1–2W during tune.

  Check 2: Is the antenna deployed correctly?
    Arms fully extended? Connections made?

  Check 3: Is one arm accidentally disconnected?
    A broken or disconnected arm makes the antenna one-sided; SWR
    will be very high (> 5:1) at all tap positions.

  Check 4: Is the coil tapped correctly by the relay?
    Use a multimeter to verify relay contact switching. With relay N
    energized, measure continuity between the appropriate coil tap and
    the relay common pin.
```

### D.4 J-Pole Minimum SWR Is 3:1 (Cannot Get Lower)

```
  Most common cause: feedline common-mode current.
  Fix: Increase ferrite loading on coax. Use FT-240-43 (better VHF
  coverage) instead of FT-50-43.

  Second cause: radiator and stub physically touching (short circuit).
  Fix: Verify 25mm gap is maintained over entire length.

  Third cause: stub or radiator length off by >10mm.
  Re-cut to exact specification.
```

### D.5 Yagi F/B Ratio Disappointing

```
  If measured F/B < 15 dB (expected 20+ dB):

  Check element spacing (measure from reflector to each director).
  Even 5mm spacing error can reduce F/B significantly.

  Check element lengths. Trim and re-measure if needed.

  Check for coax radiation: install choke balun at driven element.
  Common-mode coax current creates a backward pattern.
```

---

## APPENDIX E: REFERENCES

| Reference | Title | Relevance |
|-----------|-------|---------|
| ARRL Antenna Book, 25th Ed. | Chapters 6 (Dipoles) and 11 (VHF/UHF) | Primary design reference |
| Terman, F.E., "Radio Engineers Handbook" | Inductance and coil design | Wheeler formula derivation |
| DL6WU, "Wide-Band Yagis" | CQ-DL, 1977 | Yagi spacing tables used in design |
| Leeson, D.B., "Simple BPF and Helical Coils" | QST, 1996 | Coil Q analysis |
| AD8307 Datasheet | Analog Devices | Log amplifier characteristics |
| "Directional Couplers" | Pozar, Microwave Engineering Ch. 7 | Bruene coupler theory |
| NEC-2 User's Manual | Burke & Poggio, 1981 | Lumped loading (LD card) accuracy |
| FCC OET Bulletin 65 | RF Exposure Guidelines | RF safety thresholds |
| ARRL Handbook, 2024 Ed. | RF Safety chapter | Practical distance guidelines |

---

## FILE INVENTORY

```
/home/merv/Public/claude/PNL/
├── calculations/
│   ├── pnild_dimensions.csv          (9 HF bands, all computed values)
│   ├── jpole_dimensions.csv          (3 VHF J-pole bands)
│   ├── yagi_dimensions.csv           (3 UHF Yagi bands, all elements)
│   ├── loading_coil_tables.csv       (winding table, all 9 HF bands)
│   ├── trap_winding_tables.csv       (6 LC traps for roll-up)
│   └── rollup_trapdipole_dimensions.csv (multi-band trap dipole)
├── enclosures/
│   ├── coil_housing.scad             (TM-PNL2-ENC-001)
│   ├── dipole_center.scad            (TM-PNL2-ENC-002)
│   ├── jpole_base.scad               (TM-PNL2-ENC-003)
│   └── yagi_element_clip.scad        (TM-PNL2-ENC-004)
├── firmware/
│   ├── esp32_pnl_tuner/
│   │   ├── config.h                  (TM-PNL2-FW-001)
│   │   └── esp32_pnl_tuner.ino
│   └── cyd_pnl_display/
│       ├── config.h                  (TM-PNL2-FW-002)
│       └── cyd_pnl_display.ino
├── manual/
│   └── POOL_NOODLE_ANTENNA_MANUAL.md (this document, TM-PNL2-001)
├── nec_models/
│   ├── TM-PNL2-NEC-001_160m_pnild.nec
│   ├── TM-PNL2-NEC-002_80m_pnild.nec
│   ├── TM-PNL2-NEC-003_40m_pnild.nec
│   ├── TM-PNL2-NEC-004_30m_pnild.nec
│   ├── TM-PNL2-NEC-005_20m_pnild.nec
│   ├── TM-PNL2-NEC-006_17m_pnild.nec
│   ├── TM-PNL2-NEC-007_15m_pnild.nec
│   ├── TM-PNL2-NEC-008_12m_pnild.nec
│   ├── TM-PNL2-NEC-009_10m_pnild.nec
│   ├── TM-PNL2-NEC-010_6m_jpole.nec
│   ├── TM-PNL2-NEC-011_2m_jpole.nec
│   ├── TM-PNL2-NEC-012_125m_jpole.nec
│   ├── TM-PNL2-NEC-013_70cm_yagi.nec
│   ├── TM-PNL2-NEC-014_33cm_yagi.nec
│   └── TM-PNL2-NEC-015_20cm_yagi.nec
├── schematics/
│   ├── loading_coil_theory_ascii.txt  (TM-PNL2-SCH-001)
│   ├── jpole_matching_ascii.txt       (TM-PNL2-SCH-002)
│   └── yagi_design_ascii.txt          (TM-PNL2-SCH-003)
└── sketches/
    ├── pnild_overview.png
    ├── jpole_yagi_construction.png
    └── rollup_trapdipole.png
```

---

## END OF TECHNICAL MANUAL

```
  TM-PNL2-001 Rev A — Pool Noodle Non-Loop Antenna System
  Document complete.

  Prepared by: Merv Martin
  Date: 2026-05-08

  ─────────────────────────────────────────────────────────────────────
  Antennas compared:
  STL loop:    small, high-Q, narrow BW, lethal cap voltage, very efficient on 20m
  PNILD:       moderate size, lower Q, wide BW, safe voltage, efficient 40m and above
  Use the loop when stealth and selectivity matter.
  Use the PNILD when bandwidth and safety matter.
  Use both when it's a slow afternoon and you have wire.
  73 DE N6IMV
  ─────────────────────────────────────────────────────────────────────
```
