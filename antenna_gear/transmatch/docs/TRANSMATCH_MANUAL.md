# TRANSMATCH ANTENNA TUNING UNIT
## TECHNICAL MANUAL

**Document Number:** TM-ATU-001 Rev A  
**Equipment:** Transmatch Antenna Tuning Unit, Portable, HF/VHF/UHF  
**Frequency Coverage:** 1.8 MHz – 1300 MHz (160M through 23cm)  
**Power Rating:** 100 W continuous HF; 50 W continuous VHF/UHF  
**Classification:** UNCLASSIFIED — Amateur Radio / Field Use  
**Date:** 2026-05-24  
**Supersedes:** None (initial issue)

---

```
CHAPTER/SECTION                                                    PAGE
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CHAPTER 1   GENERAL INFORMATION . . . . . . . . . . . . . . . . .  1-1
CHAPTER 2   THEORY OF OPERATION . . . . . . . . . . . . . . . . .  2-1
CHAPTER 3   EQUIPMENT DESCRIPTION  . . . . . . . . . . . . . . .  3-1
CHAPTER 4   COMPONENT SELECTION AND PROCUREMENT . . . . . . . . .  4-1
CHAPTER 5   CONSTRUCTION PROCEDURES  . . . . . . . . . . . . . .  5-1
CHAPTER 6   INSTALLATION  . . . . . . . . . . . . . . . . . . . .  6-1
CHAPTER 7   OPERATING PROCEDURES  . . . . . . . . . . . . . . . .  7-1
CHAPTER 8   CALIBRATION . . . . . . . . . . . . . . . . . . . . .  8-1
CHAPTER 9   MAINTENANCE . . . . . . . . . . . . . . . . . . . . .  9-1
CHAPTER 10  TROUBLESHOOTING  . . . . . . . . . . . . . . . . . . 10-1
APPENDIX A  SPECIFICATIONS  . . . . . . . . . . . . . . . . . . .  A-1
APPENDIX B  BILL OF MATERIALS  . . . . . . . . . . . . . . . . .  B-1
APPENDIX C  CIRCUIT SCHEMATICS  . . . . . . . . . . . . . . . . .  C-1
APPENDIX D  GLOSSARY  . . . . . . . . . . . . . . . . . . . . . .  D-1
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

## RECORD OF CHANGES

| Change No. | Rev | Date | Description | By |
|:---:|:---:|:---:|---|:---:|
| 1 | A | 2026-05-24 | Initial issue — all chapters | M. Martin |

---

## CHAPTER 1 — GENERAL INFORMATION

### 1.1  Scope

This manual provides design, construction, operating, and maintenance
instructions for the TM-ATU-001 Portable Transmatch Antenna Tuning Unit.
The ATU covers all amateur radio bands from 160 meters through 20 centimeters
using a hybrid topology: T-network for HF and L-network for VHF/UHF.

### 1.2  Purpose

A transmatch transforms antenna feedpoint impedance to the 50 Ω impedance
required by modern transceivers.  This allows full-power operation with
non-resonant antennas, random wire antennas, or antennas that are
slightly off resonance due to environmental conditions.

```
NOTE: A transmatch does not improve antenna radiation efficiency.  It
presents a 50 Ω match to the transmitter, eliminating SWR-induced power
reduction, but any loss within the antenna system remains unchanged.
```

### 1.3  Applicable Documents

| Document | Title |
|---|---|
| TM-BAL-001 | Balun and Unun System Technical Manual |
| TM-CMC-001 | Common-Mode Choke Technical Manual |
| TM-SWR-001 | SWR Meter Technical Manual |

### 1.4  Equipment Identification

| Item | Value |
|---|---|
| Equipment designation | ATU-1 |
| HF network topology | Balanced T-network (series-shunt-series) |
| VHF/UHF topology | L-network with switched elements |
| Input impedance | 50 Ω |
| Matching range | 10–1000 Ω (HF); 25–200 Ω (VHF/UHF) |
| Physical dimensions | 200 × 150 × 80 mm |
| Weight | ~800 g |

---

## CHAPTER 2 — THEORY OF OPERATION

### 2.1  Why Impedance Matching Is Required

Modern transceivers are designed for 50 Ω loads.  When antenna impedance
differs from 50 Ω:
- Reflected power travels back toward the transmitter
- SWR protection circuitry reduces output power
- Transmission efficiency decreases
- In extreme cases (SWR > 10:1), transmitter finals may be damaged

The transmatch absorbs the impedance mismatch by presenting 50 Ω to the
transmitter while driving the antenna at its actual impedance.

### 2.2  T-Network Topology (HF Bands)

The T-network uses three reactive elements:

```
  Transceiver      C1         L1         C2       Antenna
    (50 Ω)      (Input)    (Series)   (Output)
       │            │          │          │           │
       ├────────────┴──────────┴──────────┴───────────┤
       │                                               │
      GND                                            GND

C1: Input variable capacitor — matches transceiver to inductor
L1: Roller inductor or switched coils — provides reactance range
C2: Output variable capacitor — matches inductor to antenna
```

All three elements are adjusted simultaneously for minimum SWR.  The
T-network can match impedances from 10 Ω to 1000 Ω across HF.

T-network advantages: wide impedance matching range; works with both
high and low impedances; adjustable Q factor (affects bandwidth and
efficiency).

### 2.3  L-Network Topology (VHF/UHF Bands)

The L-network uses two reactive elements:

```
  Transceiver       L1      Antenna
    (50 Ω)       (Series)
       │             │           │
       ├─────────────┴───────────┤
       │        ┌─ C1 ─┐        │
       │        │       │        │
      GND      GND     GND      GND
```

Fewer components reduce insertion loss at VHF/UHF.  Switched fixed values
are used for each band; the L-network matches SWR up to approximately 3:1 input.

### 2.4  Q Factor and Efficiency

The T-network Q is set by the ratio of inductor reactance to the matched
load impedance.  High Q: narrow bandwidth, higher losses in reactive
components.  Low Q: wider bandwidth, lower losses.

For portable QRP field use, set Q ≤ 10 for efficiency.

---

## CHAPTER 3 — EQUIPMENT DESCRIPTION

### 3.1  HF Section (160M–10M)

```
Component               Specification
──────────────────────────────────────────────────────────────────
C1 (input capacitor)    15–500 pF variable, 500 V minimum rating
L1 (inductor)           5–30 µH, roller or switched-coil
C2 (output capacitor)   15–500 pF variable, 500 V minimum rating
Connectors              SO-239 input and output
Power rating            100 W continuous, 200 W PEP
Insertion loss          < 0.5 dB typical
```

### 3.2  VHF/UHF Section (6M–23cm)

```
Component               Specification
──────────────────────────────────────────────────────────────────
L1 (series inductor)    Switched; band-specific values
C1 (shunt capacitor)    Switched; band-specific values
Connectors              SO-239 (6M, 2M); SMA (70cm and above)
Power rating            50 W continuous, 100 W PEP
Insertion loss          < 0.3 dB typical
```

### 3.3  Enclosure

3D-printed PETG; 200 × 150 × 80 mm.  OpenSCAD source files in
`mechanical/` subdirectory.  Minimum wall thickness: 2.5 mm.

---

## CHAPTER 4 — COMPONENT SELECTION AND PROCUREMENT

### 4.1  Variable Capacitors

Critical component — quality directly affects insertion loss and maximum SWR
matching range.  Do not substitute ceramic or mica dielectric capacitors;
use air-dielectric only.

```
WARNING
-------
For 100 W CW operation, variable capacitor voltage rating must be
≥ 500 V DC (1000 V preferred).  At 100 W into 50 Ω, peak voltage is
100 V RMS × √2 = 141 V peak.  High-impedance antenna ports may see
several times this voltage.  Insufficient voltage rating causes arc-over,
capacitor damage, and possible fire.
```

Specifications required:
- Capacitance range: 15–500 pF (or dual-gang 2 × 250 pF)
- Voltage rating: 500 V minimum; 1000 V recommended; 2500 V for high-SWR loads
- Dielectric: air (not ceramic or mica for transmitting use)
- Plate material: brass or silver-plated brass

Sources: Fair Radio Sales, Ocean State Electronics (surplus, $15–40); Surplus
Sales of Nebraska, Antique Electronic Supply (new, $40–100); Cardwell,
Hammarlund, Johnson vintage (eBay, $50–150).

### 4.2  Roller Inductor

- Inductance range: 5–30 µH (covers all HF bands)
- Current rating: ≥ 10 A continuous
- Wire: AWG #14 silver-plated copper minimum

Pre-made: MFJ, Palstar, Barker & Williamson ($80–200); vintage Hammarlund,
Johnson ($50–150 used).  DIY option: wind from 6 mm copper tubing on
threaded fiberglass rod (total cost $15–30; detailed procedure in Section 5.4).

Alternative: switched inductors (separate fixed coils selected by rotary
switch).  Less flexible but easier to build.

### 4.3  Connectors

- Input/output HF: SO-239 (UHF female); silver-plated preferred
- VHF/UHF above 70cm: SMA female; 50 Ω rated

---

## CHAPTER 5 — CONSTRUCTION PROCEDURES

### 5.1  Safety Precautions

```
WARNING
-------
RF voltages during transmission are hazardous.  At 100 W into 50 Ω, RMS
voltage is 70.7 V; peak voltage is 100 V.  At high SWR conditions or
antenna impedances far from 50 Ω, voltages across the variable capacitors
may exceed 1000 V peak.  NEVER touch any internal component while
transmitting.  De-energize the transmitter and wait 10 seconds before
opening the enclosure.
```

### 5.2  Printed Enclosure Parts

Print order:
1. `enclosure_main.scad` — main body (longest print; start first)
2. `front_panel.scad` — control cutouts and labels
3. `inductor_mount.scad` — roller inductor mounting bracket
4. `capacitor_mount.scad` — variable capacitor mounting hardware

Print settings: PETG; 0.2 mm layer height; 25% infill; 3 perimeter walls.
Total print time: approximately 15–20 hours.

### 5.3  HF T-Network Assembly

1.  Mount variable capacitors C1 and C2 in enclosure front panel cutouts.
    Secure with flat washers and locknuts; do not overtighten.
2.  Mount roller inductor L1 on inductor bracket.
3.  Wire C1 input terminal to SO-239 input center pin.
4.  Wire C1 output terminal to L1 input terminal.
5.  Wire L1 output terminal to C2 input terminal.
6.  Wire C2 output terminal to SO-239 output center pin.
7.  Connect all chassis points (capacitor frames, inductor frame,
    SO-239 shields) to single ground point via AWG #14 braid.
8.  Keep all wiring short; maximum lead length 50 mm.

### 5.4  DIY Roller Inductor Construction

Materials: 13 mm OD copper tubing, fiberglass threaded rod 12 mm dia,
AWG #14 copper braid, two brass end plates.

Wind copper tubing in a helix on the threaded rod; secure ends to brass
plates.  Roller contact: silver-plated roller bearing riding along the outer
surface of the coil.  Inductance per turn varies by winding pitch; verify
with NanoVNA at completion.

### 5.5  VHF/UHF L-Network Assembly

Install switched inductors and capacitors on auxiliary PCB.  Band selection
via rotary switch on front panel.  Component values are in `schematics/`
subdirectory.

---

## CHAPTER 6 — INSTALLATION

### 6.1  Location

Place transmatch between transceiver output connector and antenna feedline.
Keep RF path leads as short as possible (< 150 mm each side).

### 6.2  Grounding

Connect transmatch chassis ground to station ground bus via shortest
practical path.  Do not rely on the coaxial cable shield as the sole ground
path; add a dedicated bond wire if using long feedlines.

### 6.3  Balun Placement

When used with balanced antennas (dipoles, doublets), install a 4:1 or 1:1
balun at the transmatch output, between the ATU and the antenna feedpoint.
See TM-BAL-001 for balun design and selection.

---

## CHAPTER 7 — OPERATING PROCEDURES

### 7.1  Initial Setup

1.  Set transceiver power to minimum test level (5 W).
2.  Connect transmatch between transceiver output and antenna.
3.  Set C1 and C2 to midrange; set L1 roller to approximate band position
    (see band preset reference chart posted inside enclosure lid).
4.  Do not transmit until SWR meter is installed in line.

### 7.2  Tuning Procedure

```
CAUTION
-------
Tune at reduced power (5–10 W) until minimum SWR is achieved.  Transmitting
at full power into a mistuned transmatch may cause voltage arc-over in the
capacitors or overheating in the inductor.  Only increase to operating power
after SWR < 2:1 is confirmed.
```

1.  Key transmitter (CW or tune mode) at 5–10 W.
2.  Adjust C1 and C2 alternately while observing SWR meter.
3.  Adjust L1 (roller position or switched tap) when C adjustment alone
    cannot reduce SWR below 2:1.
4.  Iterate: after each L1 change, re-optimize C1 and C2.
5.  When SWR < 1.5:1, increase power to operating level and verify SWR
    does not change.
6.  Record L1 tap/position and C1/C2 scale readings for this antenna and
    band (for quick recall on future activations).

### 7.3  Band Preset Chart

Mark a card with C1, L1, and C2 settings for each band-antenna combination.
Attach inside enclosure lid.  Pre-tuning time in the field is typically
5–30 seconds with a known preset.

### 7.4  VHF/UHF Tuning

Select the appropriate band switch position.  Adjust L1 and C1 for minimum
SWR.  VHF/UHF tuning range is narrower; the input SWR of the antenna must
be < 3:1 for the L-network to achieve a satisfactory match.

---

## CHAPTER 8 — CALIBRATION

### 8.1  Initial Calibration

After construction, verify with NanoVNA before first RF application:
1.  Measure insertion loss (S21) at minimum loss setting: expect < 0.5 dB HF.
2.  Verify capacitance range at both extremes with NanoVNA impedance mode.
3.  Verify inductance range across roller travel.
4.  No calibration adjustments available; replace components out of specification.

---

## CHAPTER 9 — MAINTENANCE

### 9.1  Periodic Inspection (Annual or After Field Deployment)

```
[ ] Inspect all solder joints — look for cold joints and corrosion
[ ] Clean variable capacitor plates with dry compressed air
[ ] Check roller inductor contact for arcing marks (pitting)
[ ] Verify all SO-239 connectors are tight; check center pin alignment
[ ] Check enclosure screws for tightness
[ ] Verify all ground connections are secure
```

### 9.2  Roller Inductor Maintenance

Clean roller contact surface with 600-grit crocus cloth.  Silver-plate the
roller with conductive silver paint if plating is worn.  Check spring tension
of roller mechanism; replace spring if contact pressure is inadequate.

### 9.3  Variable Capacitor Maintenance

Clean capacitor plates annually with dry compressed air.  Do not use liquid
cleaners.  Inspect for bent plates (touching plates cause shorts during
rotation).  Apply a small amount of conductive grease to shaft bearings.

### 9.4  Storage

Store with variable capacitors at midrange position (reduces bearing fatigue).
Place silica gel desiccant packet inside enclosure.  Keep enclosure closed.

---

## CHAPTER 10 — TROUBLESHOOTING

### 10.1  Symptom Index

| Symptom | Paragraph |
|---|---|
| Cannot achieve SWR < 2:1 on any band | 10.2 |
| SWR increases at high power | 10.3 |
| Arcing sound during transmit | 10.4 |
| High insertion loss (> 1 dB) | 10.5 |
| Roller inductor intermittent | 10.6 |

### 10.2  Cannot Achieve SWR < 2:1

| Possible Cause | Test | Action |
|---|---|---|
| Antenna impedance outside network range | Measure antenna Z with NanoVNA | Verify antenna, adjust length |
| Capacitor range too narrow | Measure C_min and C_max | Replace with wider-range capacitors |
| Inductor range insufficient | Measure L_max | Add turns or replace with larger inductor |
| Wrong network topology for impedance | Check theory in Chapter 2 | Consider adding transformer |

### 10.3  SWR Increases at High Power

Cause: component nonlinearity or partial arc-over.  Check capacitor voltage
rating.  Inspect inductor roller contact for pitting.  Increase SWR check
power in 10 W steps; note where SWR begins to change.

### 10.4  Arcing Sound During Transmit

```
WARNING
-------
Disconnect from transmitter immediately.  Arc-over indicates capacitor plate
gap exceeded.  Continued operation with arc-over present may cause fire.
Inspect capacitor plates; clean or replace.  Reduce operating power or
change to a higher-voltage-rated capacitor.
```

### 10.5  High Insertion Loss (> 1 dB)

Causes: roller inductor contact pitting; capacitor plate oxidation;
cold solder joint in RF path.  Trace with NanoVNA S21 while manipulating
controls to identify location.

### 10.6  Roller Inductor Intermittent

Clean roller contact and coil surface with crocus cloth.  Verify spring
pressure.  Check for stripped turns on coil (physical inspection).

---

## APPENDIX A — SPECIFICATIONS

| Parameter | Value |
|---|---|
| HF frequency coverage | 1.8–30 MHz (160M–10M) |
| VHF/UHF frequency coverage | 50–1300 MHz (6M–23cm) |
| Input impedance | 50 Ω |
| HF matching range | 10–1000 Ω to 50 Ω |
| VHF/UHF matching range | 25–200 Ω to 50 Ω |
| HF power rating | 100 W continuous; 200 W PEP |
| VHF/UHF power rating | 50 W continuous; 100 W PEP |
| HF insertion loss | < 0.5 dB typical |
| SWR at match | < 1.5:1 typical |
| C1, C2 range | 15–500 pF |
| L1 range | 5–30 µH |
| Voltage rating C1, C2 | 500 V minimum |
| Enclosure dimensions | 200 × 150 × 80 mm |
| Connectors | SO-239 (HF/6M/2M); SMA (70cm and above) |
| Weight | ~800 g |

---

## APPENDIX B — BILL OF MATERIALS

| Qty | Description | Specification |
|---|---|---|
| 2 | Variable capacitor | 15–500 pF, 500 V, air dielectric |
| 1 | Roller inductor | 5–30 µH, 10 A, AWG #14 |
| 2 | SO-239 chassis mount | UHF female, 50 Ω |
| 1 | PETG filament (1.75 mm) | UV-resistant, 1 kg spool |
| 1 | Front panel label sheet | Laser-printed, laminated |
| AR | AWG #14 hookup wire | Stranded, silver-plated |
| AR | AWG #14 ground braid | Copper, tinned |
| 1 | Rotary knob (C1) | 40 mm dia, pointer type |
| 1 | Rotary knob (C2) | 40 mm dia, pointer type |
| 1 | Vernier reduction drive | 6:1 or 10:1 for inductor |

---

## APPENDIX C — CIRCUIT SCHEMATICS

See `schematics/TRANSMATCH_SCHEMATICS.txt` for full ASCII circuit diagrams:
- HF T-network (160M–10M)
- VHF/UHF L-network (6M–23cm)
- Integral SWR indicator (optional)

---

## APPENDIX D — GLOSSARY

**ATU (Antenna Tuning Unit):** Device that transforms antenna feedpoint
impedance to 50 Ω for the transceiver.

**Insertion loss:** Power lost in the matching network, expressed in dB.

**Q factor:** Ratio of reactive power stored to resistive power dissipated;
determines bandwidth and efficiency of matching network.

**SWR (Standing Wave Ratio):** Ratio of maximum to minimum voltage on a
transmission line; 1.0:1 is perfect match.

**Transmatch:** Synonym for ATU; emphasizes the network's role in
transforming (matching) the transmission-line impedance.

End of TM-ATU-001 Rev A.
