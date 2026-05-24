# SWR AND POWER METER SET
## TECHNICAL MANUAL

**Document Number:** TM-SWR-001 Rev A  
**Equipment:** SWR and Forward Power Meter, Portable, Dual-Section  
**Frequency Coverage:** 1.8–1300 MHz (160M through 23cm)  
**Power Range:** 5 W – 200 W HF; 5 W – 100 W VHF/UHF  
**Classification:** UNCLASSIFIED — Amateur Radio / Field Use  
**Date:** 2026-05-24  
**Supersedes:** SWR_METER_MANUAL.txt Rev — (initial formatted release)

---

```
CHAPTER/SECTION                                                    PAGE
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CHAPTER 1   GENERAL INFORMATION . . . . . . . . . . . . . . . . .  1-1
CHAPTER 2   THEORY OF OPERATION . . . . . . . . . . . . . . . . .  2-1
CHAPTER 3   EQUIPMENT DESCRIPTION  . . . . . . . . . . . . . . .  3-1
CHAPTER 4   COMPONENT SELECTION AND PROCUREMENT . . . . . . . . .  4-1
CHAPTER 5   HF COUPLER CONSTRUCTION  . . . . . . . . . . . . . .  5-1
CHAPTER 6   VHF/UHF COUPLER CONSTRUCTION . . . . . . . . . . . .  6-1
CHAPTER 7   DETECTOR AND METER CIRCUITS  . . . . . . . . . . . .  7-1
CHAPTER 8   MECHANICAL CONSTRUCTION . . . . . . . . . . . . . . .  8-1
CHAPTER 9   FINAL ASSEMBLY  . . . . . . . . . . . . . . . . . . .  9-1
CHAPTER 10  CALIBRATION PROCEDURES . . . . . . . . . . . . . . . 10-1
CHAPTER 11  OPERATING PROCEDURES  . . . . . . . . . . . . . . . 11-1
CHAPTER 12  MAINTENANCE . . . . . . . . . . . . . . . . . . . . 12-1
CHAPTER 13  TROUBLESHOOTING  . . . . . . . . . . . . . . . . . . 13-1
APPENDIX A  SWR CALCULATION TABLES . . . . . . . . . . . . . . .  A-1
APPENDIX B  SPECIFICATIONS  . . . . . . . . . . . . . . . . . . .  B-1
APPENDIX C  BILL OF MATERIALS  . . . . . . . . . . . . . . . . .  C-1
APPENDIX D  CALIBRATION WORKSHEET  . . . . . . . . . . . . . . .  D-1
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

## RECORD OF CHANGES

| Change No. | Rev | Date | Description | By |
|:---:|:---:|:---:|---|:---:|
| 1 | A | 2026-05-24 | Initial formatted release | M. Martin |

---

## CHAPTER 1 — GENERAL INFORMATION

### 1.1  Scope

This manual provides design, construction, calibration, operating, and
maintenance instructions for the TM-SWR-001 Portable SWR and Forward Power
Meter.  The instrument uses a dual-section design: a tandem-match directional
coupler for HF (1.8–30 MHz) and a microstrip directional coupler for
VHF/UHF (50–1300 MHz).  Operation is passive (no battery required for
basic measurement).

### 1.2  Purpose

The SWR meter measures the impedance match between a transmitter and its
antenna system by comparing forward and reflected RF power.  It is used to:

- Verify antenna resonance and tuning
- Adjust transmatch (antenna tuner) settings
- Monitor transmitter output power
- Diagnose feedline and connector faults
- Protect the transmitter from high-SWR damage

```
NOTE: SWR measures match at the meter's measurement port, not at the
antenna itself.  A dummy load shows SWR 1.0:1 but radiates nothing.
Feedline loss also reduces observed SWR.  A low reading does not
guarantee the antenna is efficient — only that the feedline presents
a matched load to the transmitter.
```

### 1.3  Equipment Identification

| Item | Value |
|---|---|
| Equipment designation | SWR-1 |
| HF coupler type | Tandem-match transmission-line |
| VHF/UHF coupler type | Microstrip PCB directional coupler |
| Detector type | Schottky diode (HP2835 or 1N5711) |
| Display | 2 × 100 µA analog panel meter |
| Physical dimensions | 180 × 120 × 60 mm |
| Weight | ~400 g |
| Power source | Passive (none required) |

---

## CHAPTER 2 — THEORY OF OPERATION

### 2.1  Standing Wave Ratio — Definition

SWR is the ratio of the maximum to the minimum voltage magnitude on a
transmission line:
```
SWR = V_max / V_min
```

Expressed in terms of reflection coefficient ρ:
```
SWR = (1 + ρ) / (1 – ρ)

where ρ = |Γ| = √(P_reflected / P_forward)
```

SWR versus power delivered:

| SWR | Power Delivered (%) | Return Loss (dB) |
|---|---|---|
| 1.0:1 | 100 | ∞ |
| 1.5:1 | 96 | 14 |
| 2.0:1 | 89 | 9.5 |
| 3.0:1 | 75 | 6 |
| 5.0:1 | 56 | 3.5 |
| 10.0:1 | 33 | 1.7 |

### 2.2  Directional Coupler Principle

A directional coupler samples a fixed fraction of the RF power traveling
in each direction while leaving the main signal path essentially undisturbed.

**Coupling factor:**
```
Coupling (dB) = 10 × log₁₀(P_main / P_sampled)

Example: −30 dB coupling at 100 W input:
  P_sampled = 100 × 10^(30/10) = 0.1 W
  P_to_antenna = 99.9 W
```

**Directivity:**
Directivity measures the coupler's ability to separate forward from reflected
signals.  It determines the maximum SWR that can be measured accurately.

```
Directivity (dB) = 10 × log₁₀(P_desired / P_undesired)

Directivity (dB)   Maximum accurate SWR
20 dB              10:1
25 dB              20:1
30 dB              50:1
```

This design achieves > 25 dB directivity on HF and > 20 dB on VHF/UHF.

### 2.3  HF Coupler — Tandem-Match Design

The tandem-match is a transmission-line coupler using two lengths of coaxial
cable wound as transformers inside ferrite cores.  One section detects forward
power; the other detects reflected power.  Directivity is achieved through
the phase relationship between the coupled signals.

Operating range: 1.8–30 MHz.  Coupling: −30 dB.  Insertion loss: < 0.08 dB.

### 2.4  VHF/UHF Coupler — Microstrip Design

A microstrip coupler uses two parallel PCB traces separated by a controlled
gap.  The electromagnetic coupling between the traces samples forward and
reflected power.  The trace spacing, width, and length are calculated for
50 Ω operation at the design frequency.

Operating range: 50–1300 MHz.  Coupling: −30 dB.  Insertion loss: < 0.15 dB.

### 2.5  Detector Circuit

Schottky diodes (HP2835 or 1N5711) convert the sampled RF to DC voltage.
The DC voltage is proportional to the peak RF voltage (envelope detection).

Power calculation from detector output:
```
P (W) = V_det² / (R_det × k²)

where k = coupling factor (linear), R_det = detector load resistance.
```

### 2.6  SWR Calculation

The SWR meter derives SWR from the forward and reflected meter readings:
```
ρ = V_ref / V_fwd   (ratio of meter deflections)
SWR = (1 + ρ) / (1 – ρ)
```

See Appendix A for precomputed SWR vs. meter reading tables.

---

## CHAPTER 3 — EQUIPMENT DESCRIPTION

### 3.1  HF Section (160M–10M)

```
Parameter               Value
──────────────────────────────────────────────────────────────────
Frequency range         1.8–30 MHz
Coupler type            Tandem-match, wound on FT-50-43 toroids
Coupling factor         −30 dB
Directivity             > 25 dB
Insertion loss          < 0.08 dB
Power rating            200 W continuous; 400 W PEP
Input/output            SO-239 (UHF female)
```

### 3.2  VHF/UHF Section (6M–23cm)

```
Parameter               Value
──────────────────────────────────────────────────────────────────
Frequency range         50–1300 MHz
Coupler type            Microstrip PCB directional coupler
Coupling factor         −30 dB
Directivity             > 20 dB
Insertion loss          < 0.15 dB
Power rating            100 W continuous; 200 W PEP
Input/output            SO-239 (6M/2M); SMA (70cm and above)
```

### 3.3  Meter and Display

Two 100 µA full-scale panel meters, 50–60 mm face diameter.
- Meter 1: Forward power (calibrated in watts)
- Meter 2: Reflected power (calibrated in watts; SWR scale on overlay)

Power ranges (switched): 5 W, 20 W, 200 W full scale.

### 3.4  Front Panel Controls

- HF/VHF toggle switch: selects which coupler is active
- Range rotary switch: 5 W / 20 W / 200 W full scale
- Optional FWD/REF switch (single-meter variant): toggles between meters

---

## CHAPTER 4 — COMPONENT SELECTION AND PROCUREMENT

### 4.1  Panel Meters

Required: 100 µA full-scale deflection; 50–60 mm face; smooth jeweled movement.

Quality indicators: smooth pointer motion; reliable zero return; metal case;
manufactured by Simpson, Triplett, or Weston.  Reject meters with sticky or
erratic movement.

Sources: eBay (search "100µA panel meter"); Mouser Electronics; surplus
electronics dealers.

### 4.2  Schottky Diodes

Use only Schottky-type diodes for lowest forward voltage and best HF response.

- HP2835 (preferred): low forward voltage 0.15–0.25 V; excellent linearity
- 1N5711: equivalent performance; more readily available
- BAT46: acceptable substitute

```
CAUTION
-------
Do not substitute silicon signal diodes (1N4148, 1N914).  Silicon
diodes have forward voltage ≈ 0.6 V, which causes a "dead zone" at
low power levels making the meter non-functional below 20–30 W.
Schottky diodes conduct at 0.15–0.25 V, allowing accurate measurement
down to 1–5 W.
```

Diode matching: For best SWR accuracy, select matched pairs with forward
voltage within 5 mV of each other at 1 mA test current.

### 4.3  HF Coupler Coax and Cores

- Cores: FT-50-43 toroids (two per coupler section)
- Coax: RG-174 (2.8 mm OD) for winding; fits FT-50 bore
- Each tandem-match winding: 10 turns RG-174 through core

### 4.4  VHF/UHF Coupler PCB

Fabricate coupler section on PTFE/glass laminate (Rogers 4003C or equivalent)
for stable dielectric constant across frequency.  FR-4 is acceptable for
6M/2M; inadequate for 70cm and above.

Track width, gap, and length are specified in the PCB layout files in
`mechanical/` subdirectory.

---

## CHAPTER 5 — HF COUPLER CONSTRUCTION

### 5.1  Tandem-Match Coupler Design

The tandem-match coupler uses two identical FT-50-43 toroid sections.
Section 1 detects forward power; Section 2 detects reflected power.

Each section: 10 turns of RG-174 coaxial cable wound through a FT-50-43
toroid, with the inner conductor and shield connected as a transmission line.

### 5.2  Winding Procedure

```
CAUTION
-------
Maintain coax bend radius ≥ 6 mm when winding through FT-50 toroids.
Kinking RG-174 at the core bore damages the center conductor and produces
intermittent contact.  Use a toothpick or thin wire guide to ease coax
through the bore.
```

1.  Cut 40 cm of RG-174.  Form first pass through FT-50-43 bore.
2.  Wind 10 turns through bore, keeping coax flat against the core face.
3.  Trim and prepare both ends: strip 10 mm outer jacket; strip 5 mm
    inner insulation; tin center conductor and braid separately.
4.  Repeat for second FT-50-43 core (Section 2).
5.  Connect Sections 1 and 2 per schematic in `schematics/` subdirectory.

### 5.3  HF Coupler Testing

Connect to NanoVNA.  Sweep 1.8–30 MHz with termination on main port.
Verify:
- Insertion loss S21 < 0.1 dB across band
- Coupling flatness ±1 dB across band
- Directivity > 20 dB (verify by reversing input/output)

---

## CHAPTER 6 — VHF/UHF COUPLER CONSTRUCTION

### 6.1  PCB Fabrication

The VHF/UHF section is etched on PTFE/glass substrate.  Coupler dimensions
are in the PCB layout file `mechanical/vhf_uhf_coupler.scad`.  For 50–1300 MHz
coverage, minimum trace tolerance is ±0.1 mm.

Commercial PCB fabrication (OSHPark, JLCPCB) is recommended over homebrew
etching for this section due to the precision required.

### 6.2  Component Assembly

Mount Schottky diodes on the coupler output pads using proper RF technique:
short leads, no flux residue, decoupling capacitors per schematic.

### 6.3  VHF/UHF Coupler Testing

Connect to NanoVNA.  Sweep 50–1300 MHz.  Verify:
- Insertion loss S21 < 0.2 dB across band
- Coupling variation < ±2 dB
- No resonances (sharp S21 dips) in band

---

## CHAPTER 7 — DETECTOR AND METER CIRCUITS

### 7.1  Schottky Diode Detector

Each detector circuit: one Schottky diode, one load resistor (1–10 kΩ),
one bypass capacitor (100 pF).  Output DC voltage:
```
V_DC ≈ V_peak – V_forward ≈ V_peak – 0.2 V (for Schottky)
```

The detector output drives the panel meter movement through a scaling
resistor network.

### 7.2  Meter Scaling Resistor

The series resistor R_s sets the meter full-scale deflection at the selected
power level:
```
R_s = V_DC_at_full_scale / I_FSD

Example: 5 W range, 50 Ω, V_peak = √(2 × 5 × 50) = 22.4 V
V_DC ≈ 22.4 – 0.2 = 22.2 V
R_s = 22.2 V / 100 µA = 222 kΩ (use 220 kΩ + 2 kΩ trimmer for calibration)
```

### 7.3  Range Switching

A three-position rotary switch selects resistance networks for 5 W, 20 W,
and 200 W full-scale power ranges.  Resistor values for each range:

| Range | R_s (approx) |
|---|---|
| 5 W | 220 kΩ |
| 20 W | 100 kΩ |
| 200 W | 33 kΩ |

Exact values determined by calibration (see Chapter 10).

---

## CHAPTER 8 — MECHANICAL CONSTRUCTION

### 8.1  Printed Enclosure

Print main body and front panel from `mechanical/` OpenSCAD files.
Material: PETG, 0.2 mm layer height, 25% infill, 3 perimeter walls.
Estimated print time: 8–10 hours total.

### 8.2  Meter Installation

```
WARNING
-------
Do not overtighten meter mounting hardware.  Meter face ring is plastic
and cracks under excessive torque.  Finger-tight plus 1/4 turn.  Use
nylon washers between panel and meter bezel.
```

Mark center of each meter cutout with compass.  Use step drill or chassis
punch for clean circular cutout.  File to fit if needed.

### 8.3  Front Panel

Apply laser-printed label to front panel before final assembly.  Laminate
label, then apply to printed panel with contact adhesive.  Trim flush after
adhesive sets.

---

## CHAPTER 9 — FINAL ASSEMBLY

### 9.1  Assembly Sequence

1.  Mount HF and VHF/UHF coupler assemblies inside enclosure, secured
    with M3 machine screws.
2.  Mount SO-239 connectors on rear panel; torque to 20 N·cm.
3.  Mount range switch and HF/VHF toggle on front panel.
4.  Install panel meters.
5.  Wire RF path: transceiver SO-239 → HF coupler → antenna SO-239.
    Keep all RF path conductors under 50 mm.
6.  Wire detector outputs through range switch to panel meters.
7.  Connect all chassis ground points with AWG #20 braid to single
    ground stud.
8.  Verify no RF path conductors contact enclosure walls.

### 9.2  Pre-Power Check

Before first RF application:
```
[ ] All solder joints inspected — no cold joints or bridges
[ ] RF path continuity: transceiver SO-239 center → antenna SO-239 center
    (resistance < 1 Ω)
[ ] RF path to chassis: should be OPEN (> 10 MΩ)
[ ] Range switch positions all functional (meter deflects when pressed
    with DC from 9V battery through 100 kΩ on each range)
[ ] Coupler input/output correctly oriented (not reversed)
```

---

## CHAPTER 10 — CALIBRATION PROCEDURES

### 10.1  Equipment Required

```
WARNING
-------
Calibration requires a known RF power source.  Do not use a transmitter
directly without a precise inline power reference.  Incorrect calibration
produces inaccurate SWR and power readings that may lead to antenna
damage or transmitter failure.
```

- Calibrated RF power source (TinySA Ultra at known output level, or
  transceiver with known power output verified against a commercial meter)
- Dummy load: 50 Ω, rated for power level used
- Trimmer screwdriver; digital multimeter

### 10.2  Forward Power Calibration

1.  Connect power source → SWR meter input → 50 Ω dummy load.
2.  Select 200 W range.
3.  Apply 100 W RF.
4.  Adjust R_s trimmer (200 W range) for full-scale deflection on forward meter.
5.  Apply 50 W RF; verify meter reads half-scale (50%).  Deviation < ±10%.
6.  Repeat for 20 W and 5 W ranges at appropriate test power levels.

### 10.3  Reflected Power Zero Verification

With 50 Ω dummy load on output (SWR 1.0:1):
1.  Apply 100 W RF.
2.  Reflected power meter must read < 5% full scale.
3.  If reflected reading > 5% with dummy load, coupler directivity is
    inadequate or couplers are reversed.  Inspect construction.

### 10.4  SWR Accuracy Verification

Create a known SWR using a mismatch load:

| Mismatch Load | Expected SWR |
|---|---|
| 100 Ω | 2.0:1 |
| 150 Ω | 3.0:1 |
| 25 Ω | 2.0:1 |

Apply known mismatch; apply RF; compute SWR from meter readings using
Appendix A tables.  Measured SWR must be within ±10% of expected.

### 10.5  Creating Meter Scales

After calibration, create scale overlays for each range:
- Forward scale: mark 10%, 25%, 50%, 75%, 100% in watts
- SWR overlay: mark 1.0, 1.5, 2.0, 3.0, 5.0 on reflected power scale

Print scales on laser printer, laminate, cut to meter face diameter.
Fasten behind meter lens with double-sided tape.

---

## CHAPTER 11 — OPERATING PROCEDURES

### 11.1  Standard Measurement Setup

1.  Connect transmitter to TRANS connector; antenna (or dummy load)
    to ANT connector.
2.  Select HF or VHF/UHF per operating frequency.
3.  Select power range: start at highest range (200 W); reduce if
    meter does not reach at least 30% deflection.
4.  Key transmitter at low power (5–10 W) before applying full power.
5.  Read forward power on Meter 1; read reflected power on Meter 2.
6.  Compute SWR using Appendix A tables, or read directly from SWR overlay.

### 11.2  Antenna Tuning Procedure

1.  Connect transmatch between transmitter and antenna.
2.  Connect SWR meter between transmitter output and transmatch input.
3.  Set transmitter to minimum power (5 W or CW tune mode).
4.  Adjust transmatch controls for minimum reflected power reading.
5.  When reflected reading is < 10% of forward reading (SWR < 2:1),
    increase power to operating level and verify readings.

### 11.3  Power Monitoring

For continuous power monitoring during operation, keep range switch set
to the range above the expected transmit power level.  The forward meter
provides a continuous indication of transmitter output.

### 11.4  Interpreting Readings

- Forward meter 0% with transmitter keyed: RF path open circuit or
  transmitter not producing output.
- Reflected equals forward: antenna disconnected or open circuit.
- SWR high on all bands: meter incorrectly oriented (input/output reversed).
- SWR high on specific band: antenna not resonant or tuner required.

---

## CHAPTER 12 — MAINTENANCE

### 12.1  Periodic Inspection (Annual)

```
[ ] Inspect all solder joints for corrosion
[ ] Clean SO-239 center pins with DeoxIT D5; verify center pin spring
[ ] Inspect HF coupler toroid winding for cracked coax insulation
[ ] Verify meter pointer returns to zero (mechanical zero adjustment)
[ ] Check range switch contacts for arcing or oxidation
[ ] Clean front panel with isopropyl alcohol (isopropyl only — no acetone)
```

### 12.2  Recalibration

Recalibrate after any component replacement or if readings appear erratic.
Follow Chapter 10 procedures.  Calibration interval: annually or after
any high-power (> 150 W) transmission event.

### 12.3  Storage

Store with internal desiccant packet.  Keep connectors capped.  Avoid
storage above 50°C (deforms PETG enclosure).

---

## CHAPTER 13 — TROUBLESHOOTING

### 13.1  Symptom Index

| Symptom | Paragraph |
|---|---|
| No meter deflection with transmitter keyed | 13.2 |
| Incorrect power readings (high or low) | 13.3 |
| Poor directivity (reflected reads high with dummy load) | 13.4 |
| Erratic or fluctuating meter behavior | 13.5 |
| Frequency-dependent errors (one band OK, others off) | 13.6 |

### 13.2  No Meter Deflection

| Possible Cause | Test | Action |
|---|---|---|
| RF path not connected | Continuity check: input to output | Re-solder or reconnect |
| Diode failed open | Measure diode forward voltage | Replace diode |
| Range switch contact failed | Inject DC; check each switch position | Clean or replace switch |
| Panel meter movement failed | Apply DC directly to meter terminals | Replace meter |
| Coupler winding continuity failure | Resistance check on coupler coils | Rewind coupler |

### 13.3  Incorrect Power Readings

Cause: calibration trimmer out of range, or wrong-value scaling resistor.
Action: recalibrate per Chapter 10.  Verify diode type is Schottky (not silicon).

### 13.4  High Reflected Reading with 50 Ω Dummy Load

```
CAUTION
-------
High reflected reading into a known 50 Ω load indicates directivity failure.
Operating without correcting this will produce inaccurate SWR readings,
potentially allowing undetected antenna faults that could damage the
transmitter.
```

Possible causes: coupler sections connected in wrong orientation (input
and output reversed); damaged toroid winding; wrong core material (must
be Mix 43 for HF).  Inspect coupler wiring per schematic.

### 13.5  Erratic Meter Behavior

Cause: cold solder joint, oxidized range switch contact, or loose
SO-239 center pin.  Clean switch contacts with DeoxIT; resolder
suspect joints; verify SO-239 center pin seating.

### 13.6  Frequency-Dependent Errors

HF only (6M–23cm reads correctly): HF coupler construction issue; check
coil turns and coax connections.  VHF/UHF only: PCB coupler issue;
inspect microstrip dimensions and diode placement.

---

## APPENDIX A — SWR CALCULATION TABLES

```
ρ = V_ref / V_fwd   (ratio of reflected to forward meter reading)
SWR = (1 + ρ) / (1 – ρ)

ρ       SWR         Return Loss (dB)
──────────────────────────────────────
0.00    1.00:1      ∞
0.05    1.11:1      26
0.10    1.22:1      20
0.15    1.35:1      16.5
0.20    1.50:1      14
0.25    1.67:1      12
0.30    1.86:1      10.5
0.35    2.08:1      9.1
0.40    2.33:1      7.9
0.45    2.64:1      6.9
0.50    3.00:1      6.0
0.60    4.00:1      4.4
0.70    5.67:1      3.1
0.80    9.00:1      1.9
0.90    19.0:1      0.9
```

---

## APPENDIX B — SPECIFICATIONS

| Parameter | Value |
|---|---|
| HF frequency coverage | 1.8–30 MHz |
| VHF/UHF frequency coverage | 50–1300 MHz |
| HF coupling factor | −30 dB |
| VHF/UHF coupling factor | −30 dB |
| HF directivity | > 25 dB |
| VHF/UHF directivity | > 20 dB |
| HF insertion loss | < 0.08 dB |
| VHF/UHF insertion loss | < 0.15 dB |
| HF power rating | 200 W CW; 400 W PEP |
| VHF/UHF power rating | 100 W CW; 200 W PEP |
| Power ranges | 5 W / 20 W / 200 W full scale |
| SWR accuracy | ±5% at SWR 1.5:1 through 3.0:1 |
| Power accuracy | ±10% with calibration |
| Minimum measurable power | ~1 W |
| Input/output connectors | SO-239 HF/6M/2M; SMA 70cm and above |
| Dimensions | 180 × 120 × 60 mm |
| Weight | ~400 g |

---

## APPENDIX C — BILL OF MATERIALS

| Qty | Description | Specification |
|---|---|---|
| 2 | Panel meter | 100 µA FSD, 50–60 mm face |
| 4 | Schottky diode | HP2835 or 1N5711 (matched pairs) |
| 2 | FT-50-43 toroid | Mix 43 HF coupler cores |
| 1 m | RG-174 coax | 50 Ω, 2.8 mm OD (coupler winding) |
| 2 | SO-239 chassis | UHF female, 50 Ω, panel mount |
| 1 | Rotary switch | 3-position; range selection |
| 1 | Toggle switch | HF/VHF selection |
| 1 | VHF/UHF coupler PCB | PTFE/glass substrate |
| 1 | PETG filament | 1.75 mm, UV-resistant |
| AR | Resistors | ±1% metal film; see Chapter 7 |
| AR | Trimmer potentiometers | 500 kΩ cermet; one per power range |

---

## APPENDIX D — CALIBRATION WORKSHEET

```
Date: _______________  Operator: _______________

Reference power source: _______________  Last calibration: _______________

FORWARD POWER CALIBRATION
Range       Applied Power (W)   Meter Reading (%)   Trimmer Setting
────────    ─────────────────   ─────────────────   ───────────────
200 W       100 W               ______%             ______
200 W        50 W               ______%             (should be ~50%)
 20 W        10 W               ______%             ______
  5 W         2.5 W             ______%             ______

REFLECTED POWER (DIRECTIVITY) CHECK — 50 Ω DUMMY LOAD
Power applied: 100 W
Reflected reading: ______%    (must be < 5%)
Result: PASS / FAIL

SWR ACCURACY CHECK
Load (Ω)   Expected SWR   Measured SWR   Error (%)
────────   ────────────   ────────────   ─────────
100 Ω      2.0:1          ______         ______
150 Ω      3.0:1          ______         ______
 25 Ω      2.0:1          ______         ______

Overall calibration result: PASS / FAIL
```

End of TM-SWR-001 Rev A.
