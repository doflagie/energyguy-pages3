# BALUN AND UNUN SYSTEM
## TECHNICAL MANUAL

**Document Number:** TM-BAL-001 Rev A  
**Equipment:** Baluns (1:1, 4:1, 6:1, 9:1) and Ununs (4:1, 9:1, 16:1, 49:1, 64:1)  
**Frequency Coverage:** 1.8 MHz – 1.3 GHz (160M through 23cm)  
**Classification:** UNCLASSIFIED — Amateur Radio / Field Use  
**Date:** 2026-05-24  
**Supersedes:** None (initial issue)

---

```
CHAPTER/SECTION                                                    PAGE
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CHAPTER 1   INTRODUCTION — IMPEDANCE TRANSFORMATION THEORY  . . .  1-1
CHAPTER 2   FERRITE CORE MATERIALS AND SELECTION  . . . . . . . .  2-1
CHAPTER 3   WINDING TECHNIQUES AND CONSTRUCTION . . . . . . . . .  3-1
CHAPTER 4   CURRENT BALUN vs VOLTAGE BALUN DESIGN  . . . . . . .  4-1
CHAPTER 5   COMMON-MODE CHOKE THEORY AND DESIGN . . . . . . . . .  5-1
CHAPTER 6   UNUN THEORY AND APPLICATIONS  . . . . . . . . . . . .  6-1
CHAPTER 7   BAND-SPECIFIC DESIGN GUIDE  . . . . . . . . . . . . .  7-1
CHAPTER 8   FIELD CONSTRUCTION AND WEATHERPROOFING  . . . . . . .  8-1
CHAPTER 9   TEST AND VERIFICATION PROCEDURES  . . . . . . . . . .  9-1
CHAPTER 10  TROUBLESHOOTING  . . . . . . . . . . . . . . . . . . 10-1
APPENDIX A  WINDING TABLES — BALUNS . . . . . . . . . . . . . . .  A-1
APPENDIX B  WINDING TABLES — UNUNS  . . . . . . . . . . . . . . .  B-1
APPENDIX C  FERRITE CORE SPECIFICATIONS . . . . . . . . . . . . .  C-1
APPENDIX D  PARTS LIST AND SOURCING . . . . . . . . . . . . . . .  D-1
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

## RECORD OF CHANGES

| Change No. | Rev | Date | Description | By |
|:---:|:---:|:---:|---|:---:|
| 1 | A | 2026-05-24 | Initial issue — all chapters | M. Martin |

---

## CHAPTER 1 — INTRODUCTION: IMPEDANCE TRANSFORMATION THEORY

### 1.1  Scope

This manual covers the design, construction, installation, and testing of
baluns and ununs for portable and fixed amateur radio antenna systems operating
on 160 meters (1.8 MHz) through the 23-centimeter band (1296 MHz).  Balun
designs covered: 1:1 common-mode choke, 4:1 Guanella current balun, 6:1
trifilar, 9:1 trifilar.  Unun designs covered: 4:1, 9:1, 16:1, 49:1, 64:1.

### 1.2  Impedance Transformation — Why It Is Required

Standard RF transmission systems operate at 50 ohms.  Antenna feedpoint
impedances vary widely:

| Antenna Type | Feedpoint Impedance |
|---|---|
| Resonant half-wave dipole | 50–73 Ω |
| Folded dipole | 200–300 Ω |
| Off-center fed dipole | 150–300 Ω |
| Random wire / longwire | 50–600 Ω (variable) |
| End-fed half-wave (EFHW) | 1000–5000 Ω |
| High-Q loop | 10–50 Ω or 1000+ Ω |

Direct connection of a mismatched impedance causes: elevated SWR, reduced
power transfer efficiency, and receiver noise figure degradation.

### 1.3  Impedance Ratio and Turns Ratio

The impedance transformation ratio equals the square of the turns ratio:

```
Z_ratio = (N_secondary / N_primary)²
```

Example — 4:1 transformation (200Ω antenna → 50Ω feedline):
```
N_ratio = √4 = 2
Turns: 3 primary, 6 secondary (2:1 ratio)
Z_out = 200 / 4 = 50 Ω
```

Voltage and current relationships for a lossless transformer:
```
V_out / V_in = N_primary / N_secondary
I_out / I_in = N_secondary / N_primary
P_in = P_out   (minus resistive losses)
```

### 1.4  Balun vs Unun — Definitions

**BALUN (Balanced to Unbalanced):**
- Input: balanced — two conductors carrying equal and opposite signals
- Output: unbalanced — single conductor plus shield
- Requires common-mode suppression (ferrite choke action)
- Applications: center-fed dipoles, folded dipoles, balanced feedline systems

**UNUN (Unbalanced to Unbalanced):**
- Input: unbalanced — single conductor plus ground reference
- Output: unbalanced — coaxial feedline
- Provides impedance transformation only; no common-mode suppression
- Applications: random wires, longwires, end-fed antennas

```
CRITICAL DISTINCTION: Baluns suppress common-mode current through ferrite
impedance.  Ununs provide impedance transformation only.  Using a unun where
a balun is needed will not suppress feedline radiation.
```

### 1.5  Transmission-Line Transformer (Guanella) Principle

A transmission-line transformer uses ferrite-loaded coaxial segments rather
than simple wire windings.  Each segment acts as a transmission line with
characteristic impedance Z_c.

Characteristic impedance of RG-58 wound on ferrite:
```
Z_c ≈ 100 Ω
```

Guanella 4:1 balun — series-parallel configuration:
```
Two TL segments in series on primary (antenna): 100 + 100 = 200 Ω input
Two TL segments in parallel on secondary (feedline): 100 ∥ 100 = 50 Ω output
Impedance ratio: 200 / 50 = 4:1
```

Advantages over simple transformer: superior common-mode suppression,
tighter phase coherence, and excellent bandwidth (±50% frequency range).

### 1.6  Insertion Loss

Balun insertion loss depends on ferrite material loss, wire resistance, and
turns count.  Typical values at HF:

| Device | Insertion Loss |
|---|---|
| 1:1 choke | 0.08–0.15 dB |
| 4:1 Guanella | 0.15–0.25 dB |
| 9:1 trifilar | 0.30–0.50 dB |
| 4:1 voltage unun | 0.08–0.12 dB |
| 49:1 EFHW unun | 0.20–0.40 dB |

Power dissipated as heat at 500 W input with 0.2 dB insertion loss:
```
P_loss = 500 × (1 – 10^(–0.2/10)) = 500 × 0.0046 ≈ 2.3 W
```

---

## CHAPTER 2 — FERRITE CORE MATERIALS AND SELECTION

### 2.1  Ferrite Basics

Ferrites are ceramic materials with high magnetic permeability (µ).  They
increase inductance per turn without requiring large physical dimensions.

Key parameter: **permeability (µ)**
- High µ (850): more inductance per turn; suited to low frequencies
- Low µ (125): less inductance per turn; suited to high frequencies
- Ferrite loss increases significantly above 100 MHz

### 2.2  Fair-Rite Material Mixes

**Mix 31 (µ = 3000):**
- Extremely lossy at RF — use for EMI suppression on power leads only
- Not suitable for impedance transformation baluns or ununs

**Mix 43 (µ = 850) — RECOMMENDED FOR HF:**
- Balanced permeability and low loss; frequency range 1–100 MHz
- Insertion loss: 0.1–0.2 dB at 80M, rising to 0.3 dB at 10M
- Core sizes: FT-240-43 (AL = 1100), FT-140-43 (AL = 520)

**Mix 61 (µ = 125) — RECOMMENDED FOR VHF:**
- Low permeability, minimal loss; frequency range 10–1000 MHz
- Insertion loss: < 0.1 dB across VHF/UHF
- Core sizes: FT-240-61 (AL = 500), FT-140-61 (AL = 240)

**Mix 75 (µ = 50):**
- Extreme VHF/UHF (100 MHz–3 GHz); specialized use only

### 2.3  AL Parameter (Inductance Index)

```
L (µH) = AL × N² / 10,000

FT-240-43, 10 turns: L = 1100 × 100 / 10000 = 11 µH
FT-240-43, 12 turns: L = 1100 × 144 / 10000 = 15.84 µH
FT-240-61,  5 turns: L = 500  × 25  / 10000 = 1.25 µH
```

### 2.4  Core Saturation

Ferrite saturates when flux density exceeds the core's critical value.
Indicators: inductance drop, rapid temperature rise, unpredictable SWR.

Prevention: select core appropriate for frequency and power; add turns on
FT-240 cores for high power; use stacked cores for 160M/80M operation.

### 2.5  Core Specifications

| Core | OD (mm) | Mix | AL (µH/100T) | Max Power (W) |
|---|---|---|---|---|
| FT-240-43 | 61 | 43 | 1100 | 700 |
| FT-140-43 | 35.6 | 43 | 520 | 350 |
| FT-240-61 | 61 | 61 | 500 | 600 |
| FT-140-61 | 35.6 | 61 | 240 | 200 |

Temperature coefficient: µ decreases ~0.3% per °C.  Effect is negligible
when Z_CM >> 250 Ω (design margin accommodates drift).

---

## CHAPTER 3 — WINDING TECHNIQUES AND CONSTRUCTION

### 3.1  Wire Selection

- Gauge: AWG #12–#20; larger gauge = fewer turns needed, easier winding
- Insulation: polyurethane (red) or polyimide (yellow) enamel
- Strip enamel: sand lightly with 400-grit, tin with rosin-core solder

### 3.2  Bifilar Winding

Two wires wound simultaneously through the core.

Procedure:
1. Cut two equal-length wire segments.
2. Twist together loosely (uniform helix, not tight).
3. Wind twisted pair through the core bore simultaneously.
4. Count as bifilar turns (each pass counts as one bifilar turn).

Use: Guanella 4:1 current balun; 1:1 CM choke bifilar variant.

### 3.3  Trifilar Winding

Three wires wound simultaneously; required for 6:1 and 9:1 designs.

Procedure:
1. Cut three equal wire lengths.
2. Keep parallel — do NOT pre-twist.
3. Feed all three through core bore together.
4. Wrap around core in synchronized pattern.
5. Separate and connect per device schematic.

```
WARNING
-------
Keep wires parallel and untwisted.  Crossing wires during trifilar winding
causes interleaved short circuits that destroy the impedance transformation
ratio.  Verify continuity between all windings (>10 MΩ primary to secondary)
before installation.
```

### 3.4  Coaxial Winding

Thread coaxial cable (RG-58, RG-174) through the core bore.  Preferred for
common-mode chokes because it maintains 50 Ω characteristic impedance
throughout and avoids enamel insulation damage.

Bend radius constraint: do not kink coax.  Minimum bend radius of RG-58 is
25 mm; RG-174 is 12 mm.

### 3.5  Interleaving

Alternating primary and secondary turns around the core reduces leakage
inductance and stray capacitance, improving high-frequency bandwidth.

Non-interleaved (poor): all primary turns together, then all secondary.
Interleaved (preferred): primary–secondary–primary–secondary pattern.

### 3.6  Practical Winding Procedure

```
CAUTION
-------
Do not allow wire enamel to contact sharp core edges.  Abrasion of enamel
insulation causes primary-to-secondary shorts.  Inspect core edges; smooth
with fine sandpaper if sharp burrs are present before winding.
```

Materials required: FT-240-43 core, #14 enameled copper wire, small vise,
400-grit sandpaper, soldering iron, rosin-core solder.

1.  Estimate wire length: 30 inches per turn (rule of thumb for FT-240).
2.  Mark starting point on core with tape.
3.  Wind primary coil — tight, even spacing.  Mark every 2–3 turns.
4.  Wind secondary coil — continue on same core.
5.  Strip 8 mm of enamel from each wire end.
6.  Tin all wire ends with rosin solder.
7.  Solder to connector lugs per device schematic.
8.  Verify isolation: ohmmeter, primary to secondary ≥ 10 MΩ.

---

## CHAPTER 4 — CURRENT BALUN vs VOLTAGE BALUN DESIGN

### 4.1  Current Balun (Guanella)

Operates on the transmission-line transformer principle.  Two bifilar-wound
TL segments are connected in series on the balanced (antenna) side and in
parallel on the unbalanced (feedline) side.

Common-mode suppression mechanism:
- Differential signal (opposite polarity on two legs): passes through with
  minimal loss through the TL impedance
- Common-mode signal (same polarity on both legs): blocked by series ferrite
  core impedance Z_CM = 2πfL

Advantages: excellent CM suppression (>20 dB typical), tight phase matching,
wide bandwidth.  Disadvantages: complex bifilar winding, larger cores required
at low frequencies.

### 4.2  Voltage Balun (Ruthroff)

Operates as a simple flux-coupled transformer.  Voltage is divided by turns
ratio; no series TL impedance in the CM path.

CM suppression is limited to the primary winding inductance only (typically
5–10 dB).  Insertion loss is lower than Guanella (simpler magnetic path).

### 4.3  Selection Criteria

Use current balun (Guanella) when:
- Antenna is balanced (dipole, folded dipole, balanced feedline)
- CM suppression is critical
- Multiband operation is required

Use voltage balun (Ruthroff) when:
- Simple impedance transformation only (no CM current expected)
- Antenna is unbalanced (random wire, longwire)
- A separate 1:1 CM choke will be added downstream

### 4.4  Hybrid Approach (Recommended for Field Portable Use)

Voltage unun (impedance transformation, 0.08 dB) + separate 1:1 CM choke
on feedline coax (CM suppression, 0.15 dB).  Total insertion loss ≈ 0.25 dB.
This combination simplifies winding while providing effective performance.

---

## CHAPTER 5 — COMMON-MODE CHOKE THEORY AND DESIGN

### 5.1  Differential Mode vs Common Mode

**Differential mode (signal, desired):** current flows on center conductor,
returns on INSIDE of outer conductor.  Fields cancel; no radiation.

**Common mode (unwanted):** current flows on OUTSIDE of outer conductor.
Driven by antenna feedpoint imbalance, ground loops, or coupling from nearby
RF.  Effects: feedline radiation, RFI in shack, pattern distortion.

### 5.2  Choking Impedance Requirements

Target: Z_CM ≥ 250 Ω at the operating frequency.  This provides >14 dB of
CM suppression relative to the 50 Ω system impedance.

Design equation:
```
Z_CM = 2π × f × L
L (µH) = AL × N² / 10,000
```

Example — 160M (1.8 MHz), FT-240-43, 14 turns:
```
L = 1100 × 196 / 10,000 = 21.56 µH
Z_CM = 2π × 1.8×10⁶ × 21.56×10⁻⁶ = 245 Ω  (meets minimum)
At 3.75 MHz (80M):  Z_CM = 514 Ω  (excellent)
At 7.15 MHz (40M):  Z_CM = 967 Ω  (excellent)
```

### 5.3  Self-Resonant Frequency (SRF)

Parasitic inter-turn capacitance creates a parallel resonance.  Above SRF,
impedance drops (capacitive behavior).

Typical SRF values:
| Core / Turns | SRF |
|---|---|
| FT-240-43, 14 turns | 40–50 MHz |
| FT-140-43, 8 turns | 60–80 MHz |
| FT-240-61, 5 turns | 150–200 MHz |

Design rule: keep operating frequency at least 2× below SRF.

### 5.4  CM Impedance Measurement

Using NanoVNA:
1.  Leave Port 2 open circuit.
2.  Connect Port 1 to choke input.
3.  Measure S11 reflection coefficient.
4.  Convert: Z_CM = 50 × (1+S11) / (1–S11)
5.  Verify Z_CM ≥ 250 Ω at operating frequency.

---

## CHAPTER 6 — UNUN THEORY AND APPLICATIONS

### 6.1  Unun Definition

An unun transforms impedance between two unbalanced ports.  There is no
balanced signal present; therefore no common-mode suppression is required
or provided.

### 6.2  Impedance Transformation

```
Z_out = Z_in / N²     where N = N_secondary / N_primary

4:1 unun:   3 primary, 6 secondary  → 200 Ω → 50 Ω
9:1 unun:   4 primary, 12 secondary → 450 Ω → 50 Ω
49:1 unun:  3 primary, 21 secondary → 2450 Ω → 50 Ω
```

### 6.3  Random Wire Antenna Applications

Random wire (non-resonant) feedpoint impedance varies widely with frequency:
50–100 Ω near resonance; 100–600 Ω typical; occasionally >1000 Ω.  A 4:1
unun provides coarse matching; an antenna tuner handles residual variation.

### 6.4  End-Fed Half-Wave (EFHW) Antenna

The EFHW is fed at the end of a half-wave element.  End impedance is very
high (theoretically infinite at exact resonance; practically 2000–5000 Ω).
A 49:1 unun (3:21 turns, FT-240-43 core) transforms this to the 50 Ω range.

Standard design: 135-foot wire (resonant on 40M, harmonically useful on
80M–10M) + 49:1 unun + 10–30 foot counterpoise + small ATU.

```
NOTE: The floating (unbalanced) secondary of the 49:1 unun means the EFHW
counterpoise can be short (λ/20 ≈ 1.5m on 40M) without loss of matching.
This is a key advantage of EFHW over other end-fed designs.
```

---

## CHAPTER 7 — BAND-SPECIFIC DESIGN GUIDE

### 7.1  160M (1.8 MHz)

| Parameter | Value |
|---|---|
| Recommended device | 1:1 CM choke or 9:1 unun |
| Core | FT-240-43 × 2 stacked |
| Turns (1:1 choke) | 14 |
| Turns (9:1 unun) | 4:12 |
| Wire gauge | AWG #14 |
| Insertion loss | 0.35–0.50 dB |
| Z_CM (1:1 choke) | ~280 Ω (marginal, acceptable) |

Stacked cores required at 160M to provide adequate inductance headroom.

### 7.2  80M (3.75 MHz)

| Parameter | Value |
|---|---|
| Recommended | 1:1 CM choke, 4:1 Guanella, or 9:1 unun |
| Core | FT-240-43 single |
| Insertion loss | 0.20–0.30 dB |
| Z_CM (1:1 choke) | ~375 Ω (excellent) |

Sweet spot for ferrite performance.

### 7.3  40M (7.15 MHz)

| Parameter | Value |
|---|---|
| Recommended | 4:1 Guanella, 6:1 trifilar, or 9:1 unun |
| Core | FT-240-43 or FT-140-43 |
| Insertion loss | 0.15–0.25 dB |
| Z_CM | >500 Ω (excellent) |

### 7.4  30M–20M (10–14 MHz)

| Parameter | Value |
|---|---|
| Recommended | 4:1 Guanella or 9:1 unun |
| Core | FT-140-43 adequate |
| Insertion loss | 0.15–0.20 dB |
| Z_CM | 500–1000 Ω (very good) |

### 7.5  17M–10M (18–28 MHz)

Ferrite marginally usable; air-core coiled-coax choke preferred above 10M.

| Parameter | Value |
|---|---|
| Recommended | 4:1 Guanella (ferrite) or air-core coax choke |
| Insertion loss (ferrite) | 0.12–0.18 dB |
| Insertion loss (air-core) | 0.03 dB |

### 7.6  6M (51 MHz) through 23cm (1296 MHz)

Air-core coiled-coax choke only.  Ferrite loss is too high above 100 MHz.

| Band | Recommended Form | Turns | Insertion Loss |
|---|---|---|---|
| 6M | RG-174 on 89mm PVC | 5 | 0.05 dB |
| 2M | RG-58 on 89mm PVC | 8 | 0.08 dB |
| 70cm | RG-58 on 76mm PVC | 6 | 0.05 dB |
| 33cm | RG-58 on 64mm PVC | 5 | 0.03 dB |
| 23cm | RG-58 on 51mm PVC | 3 | 0.02 dB |

---

## CHAPTER 8 — FIELD CONSTRUCTION AND WEATHERPROOFING

### 8.1  Enclosure Material Selection

| Material | UV Resistance | Thermal | Cost | Recommended Use |
|---|---|---|---|---|
| PETG | Excellent | Good | Low | Portable field use |
| ABS | Poor (paint required) | Excellent | Low | Permanent/structural |
| Aluminum | Excellent (anodize) | Best | High | High-power, permanent |

Print settings: PETG — 240°C nozzle, 80°C bed.

### 8.2  Weatherproofing Levels

**Level 1 — Quick field seal:**
Silicone sealant around connector holes; PVC tape wrap.  Cost: <$5.
Durability: 6–12 months.

**Level 2 — Portable/field operation:**
O-ring gasket on enclosure lid; 1.6mm drain hole in bottom to prevent water
accumulation.  Cost: $15–25.  Durability: 2–3 years.

**Level 3 — Semi-permanent installation:**
Aluminum enclosure; O-ring; two-part polyurethane paint; stainless hardware.
Cost: $40–60.  Durability: 5+ years.

### 8.3  Potting Compound Selection

Silicone rubber: recommended — flexible (resists thermal cracking), moderate
thermal conductivity.  Epoxy: not recommended for baluns — poor thermal
conductivity traps heat from ferrite losses.

### 8.4  Heat Management

```
CAUTION
-------
All balun/unun cores dissipate heat during transmission.  Expected
temperature rise at 500 W input with 0.2 dB insertion loss is approximately
20°C above ambient.  Do not exceed 100°C core temperature — ferrite
permeability degrades above this point.  If enclosure is uncomfortably hot
to the touch, reduce operating power or improve ventilation.
```

### 8.5  Pre-Installation Checklist

```
[ ] All solder connections complete and cooled
[ ] No bare conductors — insulation intact throughout
[ ] Primary to secondary continuity: OPEN (≥ 10 MΩ)
[ ] Center to shield (antenna side) continuity: OPEN
[ ] Visual inspection: no cracked core, no enamel damage
[ ] Preliminary SWR test: impedance transformation verified
```

---

## CHAPTER 9 — TEST AND VERIFICATION PROCEDURES

### 9.1  Test Equipment Required

- NanoVNA (50 kHz–3 GHz; $50–150) — required
- Short, open, and 50 Ω load calibration standards
- Dummy loads: 50 Ω, 200 Ω (for 4:1 ratio verification)

### 9.2  NanoVNA SOLT Calibration

```
CAUTION
-------
Calibration is mandatory.  Uncalibrated NanoVNA measurements are not
suitable for balun/unun verification.  Perform SOLT calibration at the
cable ends that will be connected to the device under test.
```

SOLT procedure:
1.  SHORT — short center to braid; measure Port 1, Port 2.
2.  OPEN — leave port open (no connector); measure each port.
3.  LOAD — connect 50 Ω standard resistor; measure each port.
4.  THRU — connect Port 1 directly to Port 2; record baseline loss.

After calibration: connect known 50 Ω load; verify S11 = 0 dB, Z = 50 Ω.

### 9.3  Insertion Loss Measurement (S21)

1.  Port 1: balun input (50 Ω).
2.  Port 2: balun output into 50 Ω termination.
3.  Sweep frequency range.
4.  Record S21 in dB.

Accept/reject criteria:
- S21 > –0.5 dB: PASS (good design)
- S21 –0.5 to –1.0 dB: MARGINAL (investigate)
- S21 < –1.0 dB: FAIL (rewind or replace core)

### 9.4  Impedance Ratio Verification

1.  Connect design-impedance load on antenna port (e.g., 200 Ω for 4:1).
2.  Measure S11 at feedline port.
3.  Convert: Z_in = 50 × (1+S11) / (1–S11)
4.  Expected: Z_in = design input impedance ±10%.

### 9.5  Common-Mode Impedance Measurement

1.  Leave Port 2 open circuit.
2.  Connect Port 1 to choke input.
3.  Measure S11.
4.  Convert: Z_CM = 50 × (1+S11) / (1–S11)
5.  Verify Z_CM ≥ 250 Ω at operating frequency.

### 9.6  Inductance Verification (Indirect)

At a single frequency:
```
L (µH) = Z_measured / (2π × f)

Example: Z_measured = 375 Ω at 3.75 MHz
L = 375 / (2π × 3.75×10⁶) = 15.9 µH
Expected (FT-240-43, 12 turns): L = 1100 × 144 / 10000 = 15.84 µH
Agreement: < 1% error — PASS
```

If measured L differs > 10% from design: wrong turn count, damaged core,
or primary-to-secondary short circuit.

### 9.7  Quick Field Test Procedure (5 Minutes)

1.  Calibrate NanoVNA (SOLT).
2.  Measure S11 at operating frequency: verify Z matches design.
3.  Measure S21 at operating frequency: verify > –0.5 dB.
4.  If both pass: balun functional.

---

## CHAPTER 10 — TROUBLESHOOTING

### 10.1  Symptom Index

| Symptom | Paragraph |
|---|---|
| High SWR (> 3:1) on antenna | 10.2 |
| Balun heating during transmission | 10.3 |
| Intermittent connection or frequency-dependent SWR | 10.4 |
| Radiation pattern distorted | 10.5 |
| RF in shack (microphone burns, audio rectification) | 10.6 |

### 10.2  High SWR (> 3:1)

| Possible Cause | Diagnostic Test | Corrective Action |
|---|---|---|
| Wrong turns ratio | Count turns; compare to design table | Rewind |
| Ferrite core damaged or saturated | Measure L with NanoVNA; if L < design ÷ 2 | Replace core |
| Feedline impedance mismatch | Check antenna resonance | Use antenna tuner |
| Primary-to-secondary short | Ohmmeter: should be > 10 MΩ | Rewind |
| Connector corrosion or cold solder joint | Inspect connectors visually | Resolder; apply corrosion preventative |

### 10.3  Balun Heating During Transmission (Core > 50°C)

| Possible Cause | Diagnostic Test | Corrective Action |
|---|---|---|
| High common-mode current | Measure Z_CM with NanoVNA; if < 250 Ω | Add CM choke in series |
| Core saturation (insufficient turns) | Measure L vs power; if L drops at high power | Add turns or use FT-240 core |
| Ferrite loss excessive | Measure S21; if > –0.3 dB | Reduce power or switch to air-core design |
| Antenna shorted | Ohmmeter on antenna — should be > 5 Ω | Inspect antenna for contact with structure |

### 10.4  Intermittent SWR

| Possible Cause | Diagnostic Test | Corrective Action |
|---|---|---|
| Approaching self-resonant frequency (SRF) | Sweep impedance; look for peak at SRF | Reduce turns; use Mix 61 core; air-core above 30 MHz |
| Connector intermittent | Visual inspection; flex test | Resolder; apply thread-locker |

### 10.5  Radiation Pattern Distorted

Cause: common-mode current on feedline.  Z_CM < 250 Ω.
Action: add CM choke at feedpoint; move feedline away from metal structures.

### 10.6  RF in Shack

Cause: common-mode current returning to shack on feedline outer conductor.
Action: install 1:1 CM choke at shack entry point; verify antenna balance.

---

## APPENDIX A — WINDING TABLES: BALUNS

Detailed winding tables are in:

```
baluns/winding_tables/balun_1to1_winding_table.csv
baluns/winding_tables/balun_4to1_winding_table.csv
baluns/winding_tables/balun_6to1_winding_table.csv
baluns/winding_tables/balun_9to1_winding_table.csv
```

### A.1  1:1 Current Choke (Coaxial Winding)

| Band | Core | Turns | Wire | Z_CM (Ω) | Max Power (W) |
|---|---|---|---|---|---|
| 160M | FT-240-43 × 2 | 14 | RG-174 | 245 | 700 |
| 80M | FT-240-43 | 12 | RG-174 | 375 | 700 |
| 40M | FT-240-43 | 10 | RG-174 | 500 | 700 |
| 20M | FT-140-43 | 8 | RG-174 | 600 | 350 |
| 6M | FT-240-61 | 6 | RG-174 | 800 | 500 |

### A.2  4:1 Guanella Current Balun

| Band | Core | Turns/Leg | Wire | Insertion Loss | Max Power (W) |
|---|---|---|---|---|---|
| 160M–80M | FT-240-43 × 2 | 10 | #14 bifilar | 0.20 dB | 500 |
| 40M–10M | FT-240-43 | 8 | #14 bifilar | 0.18 dB | 700 |
| 6M | FT-240-61 | 6 | #16 bifilar | 0.12 dB | 400 |

---

## APPENDIX B — WINDING TABLES: UNUNS

Detailed winding tables are in:

```
ununs/winding_tables/unun_4to1_winding_table.csv
ununs/winding_tables/unun_9to1_winding_table.csv
ununs/winding_tables/unun_49to1_winding_table.csv
ununs/winding_tables/unun_16to1_winding_table.csv
ununs/winding_tables/unun_64to1_winding_table.csv
```

### B.1  49:1 EFHW Unun (Most Common Field Design)

| Core | Primary Turns | Secondary Turns | Wire | Max Power (W) |
|---|---|---|---|---|
| FT-240-43 | 3 | 21 | #16 bifilar | 700 |
| FT-140-43 | 3 | 21 | #18 bifilar | 350 |

Counterpoise: 1.5–2.5 m at 45°, or λ/20 for operating band.

---

## APPENDIX C — FERRITE CORE SPECIFICATIONS

See Section 2.5 for tabulated specifications.  Supplier: Fair-Rite Products
(www.fair-rite.com); Mouser; Amidon Associates.

Core identification: outer diameter (inches) × 100, followed by dash and mix
number.  FT-240-43 = 2.40 inch OD, Mix 43.

---

## APPENDIX D — PARTS LIST AND SOURCING

| Component | Specification | Source |
|---|---|---|
| FT-240-43 | 2.4" OD Mix 43 toroid | Amidon, Mouser, eBay |
| FT-140-43 | 1.4" OD Mix 43 toroid | Amidon, Mouser |
| FT-240-61 | 2.4" OD Mix 61 toroid | Amidon, Mouser |
| Enameled wire | AWG #14, polyurethane | Amidon, Amazon |
| RG-174 coax | 50 Ω, 2.8 mm OD | Mouser, eBay |
| SO-239 chassis | UHF female, 50 Ω | Amazon, RF Parts |
| PETG filament | 1.75 mm, UV-resistant | Amazon, Hatchbox |

End of TM-BAL-001 Rev A.
