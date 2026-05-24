# ROLLER INDUCTOR SYSTEM
## TECHNICAL MANUAL

**Document Number:** TM-ROLLER-001 Rev A  
**Equipment:** Motorized Roller Inductor for HF Antenna Tuners  
**Frequency Coverage:** 1.8 – 30 MHz (HF Amateur Bands)  
**Classification:** UNCLASSIFIED  
**Date:** 2026-05-01  

---

```
CHAPTER/SECTION                                                    PAGE
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CHAPTER 1   INTRODUCTION AND GENERAL INFORMATION . . . . . . . . .  1-1
CHAPTER 2   INDUCTOR THEORY . . . . . . . . . . . . . . . . . . .  2-1
CHAPTER 3   ROLLER CONTACT DESIGN . . . . . . . . . . . . . . . .  3-1
CHAPTER 4   EQUIPMENT DESCRIPTION . . . . . . . . . . . . . . . .  4-1
CHAPTER 5   CONSTRUCTION TECHNIQUES . . . . . . . . . . . . . . .  5-1
CHAPTER 6   INSTALLATION . . . . . . . . . . . . . . . . . . . .  6-1
CHAPTER 7   OPERATION . . . . . . . . . . . . . . . . . . . . . .  7-1
CHAPTER 8   CALIBRATION PROCEDURES . . . . . . . . . . . . . . .  8-1
CHAPTER 9   FIRMWARE REFERENCE . . . . . . . . . . . . . . . . .  9-1
CHAPTER 10  MAINTENANCE . . . . . . . . . . . . . . . . . . . . . 10-1
CHAPTER 11  TROUBLESHOOTING . . . . . . . . . . . . . . . . . . . 11-1
APPENDIX A  SPECIFICATIONS . . . . . . . . . . . . . . . . . . .   A-1
APPENDIX B  INDUCTANCE TABLE . . . . . . . . . . . . . . . . . .   B-1
APPENDIX C  BILL OF MATERIALS . . . . . . . . . . . . . . . . . .  C-1
APPENDIX D  BAND PRESET REFERENCE . . . . . . . . . . . . . . . .  D-1
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

---

## LIST OF WARNINGS, CAUTIONS, AND NOTES

```
WARNING — HIGH VOLTAGE ON COIL DURING TRANSMIT . . . . . . . . .  1-3
WARNING — MOTOR MOVEMENT HAZARD . . . . . . . . . . . . . . . . .  1-4
CAUTION  — POWER DOWN BEFORE MANUAL ROLLER ADJUSTMENT . . . . . .  6-2
CAUTION  — HOME SEQUENCE BEFORE FIRST USE . . . . . . . . . . . .  7-2
CAUTION  — COIL MUST BE STATIONARY DURING TRANSMIT . . . . . . .  7-3
CAUTION  — ROLLER CONTACT CLEANLINESS . . . . . . . . . . . . . .  3-4
NOTE     — WHEELER FORMULA ACCURACY . . . . . . . . . . . . . . .  2-5
NOTE     — TEMPERATURE AND INDUCTANCE STABILITY . . . . . . . . .  2-9
NOTE     — WORM GEAR SELF-LOCKING PROPERTY . . . . . . . . . . .  4-6
```

---

# CHAPTER 1 — INTRODUCTION AND GENERAL INFORMATION

## 1.1 SCOPE

### 1.1.1 Purpose

1.1.1.1  This manual provides complete information for construction, installation, calibration, operation, and maintenance of the Roller Inductor System, designation TM-ROLLER-001. The system is a continuously variable inductance coil used as the series element in HF antenna tuning networks (L-network, T-network, or pi-network) covering 1.8 to 30 MHz.

1.1.1.2  The inductor is adjustable from approximately 0.5 to 34 µH by moving a silver-plated roller contact along the coil winding. Inductance is set either:

a. **Manually**, by rotating the reduction drive handwheel.

b. **Motorized**, via an ESP32 controller with NEMA 17 stepper motor, DRV8825 driver, and 10:1 worm gear reduction, controllable by front panel buttons, Bluetooth SPP, USB serial, or CYD (ILI9341) touchscreen display.

### 1.1.2 Applicability

1.1.2.1  This inductor is designed for use in:

- HF antenna tuners (manual and automatic)
- Transmatch networks for resonating non-resonant wire antennas
- Impedance matching between transmitter and feedline
- Loading coils for short vertical antennas
- Field portable operation in the 1.8–30 MHz range
- QRP (< 5 W) through legal limit (1 kW PEP SSB) operation

### 1.1.3 Design Basis

1.1.3.1  The roller inductor design follows established practice for high-power, high-Q variable inductors used in professional and amateur station equipment. Key design principles:

- **Minimum contact resistance** — silver-plated roller and wire surface, spring-loaded contact
- **Maximum Q factor** — large conductor diameter (AWG 12), air-core construction, low-loss former material
- **High voltage standoff** — ceramic or fiberglass former, turn spacing ≥ 1× wire diameter, >5 kVRMS rating
- **Repeatability** — worm gear reduction, step counting, limit switch homing, NVS position memory

---

## 1.2 PERFORMANCE SUMMARY

```
Table 1-1. System Performance Summary
═══════════════════════════════════════════════════════════════════════
Parameter                          Value           Condition
═══════════════════════════════════════════════════════════════════════
Inductance range                   0.5 – 34 µH     Continuous
Minimum step (manual, 20:1 gear)   0.005 µH        At 1 µH setting
Minimum step (motorized, 1/16 µs)  0.0001 µH       At full res.
Frequency range                    1.8 – 30 MHz    HF amateur bands
Current rating (continuous)        10 A RMS        In free air, 25°C
Current rating (peak)              20 A peak       SSB envelope peak
Voltage rating                     > 5 kVRMS       Air gap 1.5×wire OD
Power rating (continuous)          500 W           10A × 50Ω match
Power rating (peak)                1500 W          20A peak × 50Ω
Q factor (7 MHz, full inductance)  ≥ 300           Clean contacts
Q factor (14 MHz, half inductance) ≥ 250           Clean contacts
Insertion loss (7 MHz, 25 µH)      < 0.1 dB        Q ≥ 300 equivalent
Coil former OD                     63.5 mm (2.5")
Winding turns                      30
Wire gauge                         AWG 12 silver-plated copper
Winding pitch                      2.54 mm (10 TPI)
Winding length                     76.2 mm (3.0")
Former material                    PETG or glazed ceramic
Motor (motorized variant)          NEMA 17, 1.8°/step, 200 steps/rev
Drive reduction                    10:1 worm gear
Leadscrew pitch                    2.0 mm
Position resolution (motorized)    0.000125 mm/step (1/16 µstep)
Homing method                      Mechanical limit switch
Position memory                    NVS (survives power cycle)
Control interface                  BT SPP, USB Serial, CYD touch
Supply (motorized)                 12V DC, 2A
═══════════════════════════════════════════════════════════════════════
```

---

## 1.3 SAFETY PRECAUTIONS

### 1.3.1 RF Voltage Hazard

```
╔══════════════════════════════════════════════════════════════════╗
║                            WARNING                               ║
║                                                                  ║
║  DURING TRANSMIT OPERATION, THE COIL WINDING MAY BE AT HIGH     ║
║  RF VOLTAGE RELATIVE TO CHASSIS GROUND, PARTICULARLY WHEN       ║
║  USED IN A SERIES-LOADED ANTENNA TUNER WITH A REACTIVE LOAD.    ║
║  AT 1 kW INTO A REACTIVE ANTENNA, VOLTAGES EXCEEDING 5 kV      ║
║  CAN APPEAR ACROSS THE COIL. DO NOT CONTACT THE COIL OR         ║
║  ASSOCIATED WIRING WHILE RF IS APPLIED.                         ║
╚══════════════════════════════════════════════════════════════════╝
```

### 1.3.2 Moving Machinery

```
╔══════════════════════════════════════════════════════════════════╗
║                            WARNING                               ║
║                                                                  ║
║  THE MOTORIZED DRIVE SYSTEM CAN PINCH FINGERS BETWEEN THE       ║
║  ROLLER CARRIAGE AND COIL FORMER. KEEP FINGERS CLEAR OF THE     ║
║  CARRIAGE TRAVEL PATH WHEN THE MOTOR IS ENERGIZED. DISABLE      ║
║  THE MOTOR (SEND STOP COMMAND OR DISCONNECT POWER) BEFORE       ║
║  PERFORMING ADJUSTMENTS OR MAINTENANCE.                         ║
╚══════════════════════════════════════════════════════════════════╝
```

### 1.3.3 Transmit Interlock

1.3.3.1  The inductor **must not be moved while the transmitter is operating**. Moving the roller contact under RF power will:
- Cause arcing at the contact point, destroying both the roller and the wire
- Create a momentary short circuit as the roller traverses the space between turns
- Produce a tuning transient that may damage the transmitter final amplifier

An interlock relay (PTT-controlled, normally-open relay in the motor enable line) is strongly recommended in high-power installations.

---

## 1.4 NOMENCLATURE AND ABBREVIATIONS

```
Table 1-2. Abbreviations
─────────────────────────────────────────────────────────────────
Abbreviation    Definition
─────────────────────────────────────────────────────────────────
AWG             American Wire Gauge
BT              Bluetooth
CYD             Cheap Yellow Display (ILI9341 320×240 TFT)
ESR             Equivalent Series Resistance
GPIO            General Purpose Input/Output
NEMA            National Electrical Manufacturers Association
NVS             Non-Volatile Storage
PPR             Pulses Per Revolution (encoder)
Q               Quality factor (ratio of reactance to resistance)
SWR             Standing Wave Ratio
TPI             Turns Per Inch (winding density)
µH              Microhenry (10⁻⁶ Henry)
WDT             Watchdog Timer
Z₀              Characteristic impedance (50 Ω)
─────────────────────────────────────────────────────────────────
```

---

# CHAPTER 2 — INDUCTOR THEORY

## 2.1 INDUCTANCE FUNDAMENTALS

### 2.1.1 Physical Basis

2.1.1.1  An inductor stores energy in a magnetic field surrounding a current-carrying conductor. When current changes through the coil, the changing magnetic flux induces a voltage opposing the change (Lenz's law):

```
  V  =  L × (dI/dt)          [Volts, where L = inductance in Henries]
```

2.1.1.2  The inductance of a coil is determined by its geometry: diameter, number of turns, and winding length. All other factors being equal, inductance increases as:
- Coil diameter increases (as d²)
- Number of turns increases (as N²)
- Coil length decreases (for a fixed number of turns)

### 2.1.2 Wheeler's Formula

2.1.2.1  For a single-layer solenoid coil in air, Wheeler's formula (1928 revision) gives inductance accurate to within ±1% for practical coil proportions:

```
         d² × N²
L  =   ─────────────     [µH]
         18d + 40l

where:
  d = coil outer diameter (inches)
  N = number of turns (the active turns between roller and fixed terminal)
  l = winding length (inches) = N × pitch (in/turn)
  L = inductance in microhenries
```

```
╔══════════════════════════════════════════════════════════════════╗
║                             NOTE                                 ║
║                                                                  ║
║  WHEELER'S FORMULA ASSUMES: (1) WIRE DIAMETER MUCH LESS THAN    ║
║  COIL DIAMETER; (2) TURNS CLOSELY WOUND (PITCH ≈ WIRE DIA);    ║
║  (3) AIR CORE (NO MAGNETIC MATERIAL). ACCURACY IS ±1% FOR      ║
║  COILS WITH l/d RATIO BETWEEN 0.4 AND 3.0. OUTSIDE THIS RANGE  ║
║  ERROR INCREASES. MEASURE WITH INDUCTANCE METER FOR PRECISE     ║
║  CALIBRATION VALUES.                                            ║
╚══════════════════════════════════════════════════════════════════╝
```

### 2.1.3 Worked Examples — This Coil

2.1.3.1  For the TM-ROLLER-001 coil (d = 2.5", pitch = 2.54 mm = 0.100 in/turn):

```
Table 2-1. Inductance vs. Active Turns (Wheeler's Formula)
───────────────────────────────────────────────────────────────────
Active    Winding    l/d       L calc    Primary HF
Turns     Length(in) ratio     (µH)      band use
───────────────────────────────────────────────────────────────────
 2        0.20       0.080      0.15     (below HF use)
 5        0.50       0.200      0.92     10m edge
 8        0.80       0.320      2.25     10/12m
10        1.00       0.400      3.44     17/15m
13        1.30       0.520      5.63     20m
16        1.60       0.640      8.30     40m edge
19        1.90       0.760     11.45     40m low
22        2.20       0.880     15.10     60m
25        2.50       1.000     19.20     80m edge
27        2.70       1.080     21.90     80m
28        2.80       1.120     23.40     80m
29        2.90       1.160     25.00     80m
30        3.00       1.200     26.50     80/160m
───────────────────────────────────────────────────────────────────
Note: Measured values will vary ±5% from these computed values.
Calibrate against inductance meter for precision use.
───────────────────────────────────────────────────────────────────
```

2.1.3.2  The firmware uses these calculations via `WheelerCalc::calcL(N)` in `position.h`. The Newton-Raphson solver in `turnsForL()` inverts the formula to find required turns for a target inductance.

---

## 2.2 Q FACTOR

### 2.2.1 Definition

2.2.2.1  The quality factor Q is the ratio of reactance to total series resistance:

```
        X_L         2π × f × L
Q  =  ───────  =  ────────────────
        R_total        R_total
```

where R_total includes:
- DC winding resistance R_dc
- Skin effect resistance R_skin (increases as √f)
- Contact resistance R_contact (roller + terminal connections)
- Dielectric loss R_diel (in former material)

2.2.2.2  High Q is desirable because:
- Higher Q → lower insertion loss in the tuning network
- Lower circulating current for a given power level
- Sharper resonance response
- Cooler operation at high power

### 2.2.2 Skin Effect

2.2.2.1  At RF frequencies, current flows in a thin skin near the conductor surface. Skin depth δ:

```
       1
δ  =  ────────────          [meters]
       √(π × f × µ × σ)

For copper at 7 MHz:
  δ = 1 / √(π × 7×10⁶ × 4π×10⁻⁷ × 5.8×10⁷) = 25 µm
```

2.2.2.2  For AWG 12 copper wire (diameter = 2.053 mm), the effective cross-section at 7 MHz:

```
Circumference = π × 2.053mm = 6.449mm
Effective area = 6.449mm × 0.025mm = 0.161 mm²
R_skin = ρ / A_eff = 17.2 nΩ·m / (0.161×10⁻⁶ m²) = 107 µΩ/m at 7 MHz
```

2.2.2.3  Silver plating improves skin effect resistance because silver (σ = 6.3×10⁷ S/m) has slightly better conductivity than copper (σ = 5.8×10⁷ S/m). The improvement in Q is approximately 4%.

### 2.2.3 Contact Resistance

2.2.3.1  Contact resistance between the roller and coil wire is the dominant Q-limiting factor in a roller inductor. Contact resistance R_c is:

```
  R_c  =  ρ_contact / A_contact

where A_contact is determined by Hertzian contact mechanics:

  A_contact  =  π × (3 × F × R_eff / (4 × E_eff))^(2/3)

  F = normal contact force (N)
  R_eff = harmonic mean of roller and wire radii
  E_eff = reduced elastic modulus
```

2.2.3.2  For a 0.5 N contact force, 4mm roller radius, 1mm wire radius, in silver:
- Contact area ≈ 0.03 mm²
- Contact resistance ≈ 1.5 mΩ

2.2.3.3  This contact resistance appears in series with the coil for all power flowing from the roller to the terminal. At 10 A RMS, power dissipated in the contact:

```
P_contact  =  I² × R_c  =  10² × 0.0015  =  0.15 W
```

This is acceptable. At 20 A peak (SSB), instantaneous dissipation = 0.6 W — also acceptable.

### 2.2.4 Q Optimization Checklist

2.2.4.1  To maximize Q:

```
Table 2-2. Q Optimization Measures
──────────────────────────────────────────────────────────────────────
Measure                              Effect on Q      Implementation
──────────────────────────────────────────────────────────────────────
Silver-plate roller and wire         +4–8%            Silver-plate or solid silver
Maintain clean contact surface       +20–50%          Clean weekly per Ch. 10
Increase contact force (0.5→1N)      +5%              Adjust spring preload
Use AWG 10 instead of AWG 12         +12%             Larger wire (heavier)
Increase coil diameter (2.5→3 in)    +15%             Larger former
Use non-conductive former material   +2–5%            Ceramic vs. PLA
Wind turns with spacing (1.5× pitch) +8%              Reduces inter-turn C
──────────────────────────────────────────────────────────────────────
```

---

## 2.3 INDUCTIVE REACTANCE AND TUNING NETWORKS

### 2.3.1 Inductive Reactance

2.3.1.1  The inductive reactance at frequency f:

```
X_L  =  2π × f × L  =  ω × L     [Ohms]
```

2.3.1.2  At key HF frequencies, reactance for this coil range:

```
Table 2-3. Inductive Reactance at HF Band Centers
───────────────────────────────────────────────────────────────────
Frequency    Band   L = 5 µH    L = 15 µH   L = 30 µH
───────────────────────────────────────────────────────────────────
1.8 MHz      160m    57 Ω        170 Ω        339 Ω
3.5 MHz       80m   110 Ω        330 Ω        660 Ω
7.0 MHz       40m   220 Ω        660 Ω       1320 Ω
10.1 MHz      30m   318 Ω        950 Ω       1900 Ω
14.2 MHz      20m   447 Ω       1340 Ω       2680 Ω
21.2 MHz      15m   666 Ω       2000 Ω       4000 Ω
28.5 MHz      10m   895 Ω       2680 Ω       5370 Ω
───────────────────────────────────────────────────────────────────
```

### 2.3.2 Use in L-Network Tuner

2.3.2.1  In a series-L, shunt-C L-network (low-pass for loads below 50 Ω):

```
Transmitter (50Ω)              Antenna (Z_A)
     │                               │
    [L] ─────────────────────────── ─┤
     │              │                │
    GND            [C]              GND
                    │
                   GND
```

Required series inductance for impedance transformation:

```
Q_net  =  √(R_high/R_low - 1)
X_L    =  Q_net × R_low
L      =  X_L / (2π × f)
```

Example: Transform Z_A = 200 Ω down to 50 Ω at 7 MHz:

```
Q_net = √(200/50 − 1) = √3 = 1.732
X_L   = 1.732 × 50 = 86.6 Ω
L     = 86.6 / (2π × 7×10⁶) = 1.97 µH
```

2.3.2.2  For a T-network or pi-network, both the series and shunt inductors/capacitors are calculated by the firmware's band preset algorithm (see Appendix D). Band presets represent typical values for a center-fed half-wave dipole on each band using a standard tuner circuit.

### 2.3.3 Self-Resonance

2.3.3.1  A real coil has distributed capacitance between turns (interwinding capacitance C_w). At some frequency, the coil self-resonates:

```
f_SRF  =  1 / (2π × √(L × C_w))
```

2.3.3.2  Above the SRF, the coil appears capacitive, not inductive. For this coil at full inductance (26.5 µH):
- Estimated C_w ≈ 5–10 pF (distributed)
- Estimated SRF ≈ 14–20 MHz at 30 turns

This means the coil should not be used at full turns on 20m or higher. In practice, fewer turns are needed for higher bands anyway (see Table 2-1).

```
╔══════════════════════════════════════════════════════════════════╗
║                             NOTE                                 ║
║                                                                  ║
║  TEMPERATURE AFFECTS INDUCTANCE THROUGH THERMAL EXPANSION       ║
║  OF THE FORMER AND WIRE. FOR PETG FORMER (CTE ≈ 60 ppm/°C),    ║
║  A 30°C TEMPERATURE RISE CAUSES APPROXIMATELY 0.18% CHANGE IN  ║
║  INDUCTANCE. THIS IS NEGLIGIBLE FOR ANTENNA TUNING. CERAMIC     ║
║  FORMERS (CTE ≈ 6 ppm/°C) ARE SUPERIOR FOR TEMPERATURE         ║
║  STABILITY IN PRECISION APPLICATIONS.                           ║
╚══════════════════════════════════════════════════════════════════╝
```

---

# CHAPTER 3 — ROLLER CONTACT DESIGN

## 3.1 ROLLER CONTACT MECHANICS

### 3.1.1 Contact Types

3.1.1.1  Three roller contact designs are used in practice for variable inductors:

**Type 1 — Ball Bearing Roller (this design):**
- Grooved or knurled silver-plated ball bearing rides in the space between wire turns
- Makes contact on the wire surface between grooves
- Pros: low friction, self-cleaning, consistent normal force
- Cons: small contact area, requires precise alignment with turns

**Type 2 — Knife Edge (wiper):**
- Thin phosphor-bronze or silver strip runs along wire surface
- Scrapes clean on each move
- Pros: very low contact resistance, simple
- Cons: higher friction, wears wire surface over time

**Type 3 — Roller Wheel (this design, alternate):**
- Precision-turned brass roller, silver-plated, on ball bearing shaft
- Contacts wire surface with crown profile
- Pros: large contact area, even wear, high power
- Cons: more complex bearing assembly

3.1.1.2  This design uses a **crown-profile brass roller on a ball bearing shaft**, silver-plated. The crown ensures the contact point is always at the roller centerline regardless of small carriage misalignment.

### 3.1.2 Contact Force Requirements

3.1.2.1  Minimum contact force for reliable low-resistance electrical contact is determined by:

```
F_min  =  H_v × A_contact_required

For silver-on-silver, contact hardness H_v ≈ 350 MPa
For R_c ≤ 5 mΩ, A_required ≈ 0.05 mm²
F_min  =  350 × 10⁶ × 0.05 × 10⁻⁶  =  17.5 N  ← too high for ball bearing

In practice, multiple asperity contacts (Holm's theory):
F_min ≈ 0.3 – 1.0 N for silver contacts at normal RF surface currents.
```

3.1.2.2  Design spring preload = **0.5 ± 0.1 N** (50 gf). This is set by the compression spring in the roller carriage pocket (see Section 4.3.1).

3.1.2.3  Excessive contact force:
- Increases carriage drag (more motor torque required)
- Wears wire surface and roller plating faster
- Increases carriage alignment forces
- Limit: 2.0 N maximum

### 3.1.3 Silver Plating

3.1.3.1  Silver plating provides:
- Lowest resistance of common plating materials
- Self-lubricating property (silver oxide is a conductor)
- Prevents copper oxidation on wire surface

```
Table 3-1. Contact Plating Comparison
──────────────────────────────────────────────────────────────────
Plating         Resistivity     Contact R     Oxidation
──────────────────────────────────────────────────────────────────
Silver          16 nΩ·m         Lowest        Tarnishes (Ag₂S)
                                              (Ag₂S is conductive)
Gold            22 nΩ·m         Low           None
Nickel          69 nΩ·m         Moderate      Minimal
Tin             115 nΩ·m        High          Tin whisker risk
Bare copper     17 nΩ·m         Low (new)     Cu₂O high R
──────────────────────────────────────────────────────────────────
```

3.1.3.2  Silver plating methods for the roller:
- **Commercial:** Purchase silver-plated brass roller bearing (preferred)
- **Electrolytic:** Plate brass roller in silver cyanide bath (hazardous)
- **Conductive paint:** Brush-apply silver-filled epoxy paint (MG Chemicals 842AR)
  - Resistivity 0.01 Ω/square — acceptable for this application
  - Apply thin coat, cure 24h, polish smooth with 600-grit
  - Reapply annually or when contact resistance increases

```
╔══════════════════════════════════════════════════════════════════╗
║                            CAUTION                               ║
║                                                                  ║
║  ROLLER CONTACT SURFACE MUST BE FREE OF OIL, OXIDE, DUST, AND  ║
║  FOREIGN MATERIAL. ANY CONTAMINATION INCREASES CONTACT          ║
║  RESISTANCE, CAUSING HEATING AND POTENTIAL ARC DAMAGE AT HIGH   ║
║  POWER. CLEAN CONTACT SURFACES BEFORE EACH HIGH-POWER SESSION.  ║
╚══════════════════════════════════════════════════════════════════╝
```

### 3.1.4 Wire Surface Condition

3.1.4.1  The coil wire surface in the contact zone must be kept clean. Silver-plated wire naturally tarnishes to silver sulfide (Ag₂S) over time. Ag₂S has resistivity approximately 10⁻⁵ Ω·cm — still conductive, but far more resistive than clean silver.

3.1.4.2  To slow tarnishing:
- Store indoors away from sulfur sources (rubber, off-gassing plastics, outdoor air)
- Apply Stabilant-22 contact treatment to roller surface (not wire surface)
- Cover coil when not in use

3.1.4.3  Wire surface cleaning: See Chapter 10, Maintenance.

---

## 3.2 WINDING DESIGN

### 3.2.1 Wire Selection

3.2.1.1  Wire specifications:

```
Table 3-2. Coil Wire Specifications
──────────────────────────────────────────────────────────────────────
Parameter           Specification         Why
──────────────────────────────────────────────────────────────────────
Gauge               AWG 12                Current rating 20A peak
Bare diameter       2.053 mm              Sets minimum winding pitch
Plating             Silver over copper    Lowest contact resistance
Insulation          None or clear enamel  Roller must contact bare metal
Temper              Half-hard             Holds winding shape vs. soft
Stranding           Solid                 Litz not needed ≤30 MHz
──────────────────────────────────────────────────────────────────────
```

3.2.1.2  AWG 12 solid silver-plated copper wire is available from Belden (9369), Remington Industries, or surplus electronics suppliers. Total wire required for 30 turns on a 2.5" diameter former:

```
Length per turn = π × d = π × 63.5mm = 199.5mm
Total wire = 30 × 199.5mm = 5.99m ≈ 6m (order 8m to allow terminals)
```

### 3.2.2 Winding Density

3.2.2.1  The winding pitch (center-to-center turn spacing) must be:

```
Pitch_min = wire_diameter + clearance
          = 2.053mm + 0.2mm = 2.253mm  (contact clearance for roller)

This design uses pitch = 2.54mm (10 TPI) — provides 0.49mm
clearance between adjacent turns.
```

3.2.2.2  The clearance between adjacent turns determines the voltage rating. For a sinusoidal voltage across N turns, the inter-turn voltage:

```
V_inter_turn  =  V_total / N

At 5 kV across 30 turns: V_inter_turn = 167 Vrms per turn

Breakdown voltage of air at 1 atm: ~3 kV/mm
For 0.49mm gap: V_breakdown ≈ 0.49 × 3000 = 1470 V

5 kVRMS across all 30 turns: max inter-turn V = 167 V ← safely below 1470 V
Safety factor: 1470 / 167 = 8.8 ✓
```

### 3.2.3 Turns Direction Convention

3.2.3.1  Wind the coil in a **left-hand helix** viewed from the bottom (home end). This convention matches the firmware's assumption: increasing roller position from home = increasing active turns = increasing inductance.

3.2.3.2  The fixed terminal is at the **top end** of the coil (max end, away from home limit switch). The roller terminal carries the variable tap. The bottom end of the winding is a dummy terminal (not connected) — the winding exits through the bottom flange wire hole but is not electrically used unless a minimum-L stop is needed.

---

# CHAPTER 4 — EQUIPMENT DESCRIPTION

## 4.1 COIL FORMER ASSEMBLY

### 4.1.1 Former Description

4.1.1.1  The coil former (P/N RL-FORMER-001) is a hollow cylindrical tube:

```
Table 4-1. Former Dimensions
────────────────────────────────────────
Dimension             Value
────────────────────────────────────────
Outer diameter        63.5 mm (2.500 in)
Inner diameter        57.5 mm
Wall thickness        3.0 mm
Total length          188 mm
(incl. top/bottom flanges 4mm each)
Active winding length 76.2 mm (3.0 in)
Winding start from    40 mm from
bottom flange         bottom face
────────────────────────────────────────
```

4.1.1.2  **Helical groove:** A 2.54mm-pitch helical groove is molded/printed into the outer surface. This groove guides the wire during winding and establishes precise turn spacing. Groove depth = 1.0 mm (wire sits 50% in groove, contacts roller at outer surface).

4.1.1.3  **Material options:**
- **PETG** (printed): Low cost, adequate for < 500W. Dielectric constant εr ≈ 3.6 may slightly increase inter-turn capacitance vs. air.
- **Glazed alumina ceramic** (machined/commercial): Best Q, zero moisture absorption, εr ≈ 8 but uniform surface. Use for > 500W or high-Q requirements.
- **G10/FR4 fiberglass tube** (off-the-shelf): Good compromise. Available in 2.5" OD from McMaster-Carr (9535K25 equiv). Cut to length and groove with lathe.

4.1.1.4  **Flanges:** Top and bottom flanges (4mm thick, 73.5mm OD) carry the guide rod mounting holes and wire exit holes. Four M3 mounting holes on each flange attach to the main chassis frame.

### 4.1.2 Guide Rod System

4.1.2.1  Two 6mm diameter ground steel or stainless rods run parallel to the former axis, offset 42.5 mm from the former centerline (10 mm outside the former OD). Rods press-fit into the flange holes.

4.1.2.2  Rod length = 188mm (same as former total length). Rods extend 5mm beyond each flange into the chassis end plates for alignment.

4.1.2.3  Rod surface finish: ground to h6 tolerance (0/-0.016mm). Carriage linear bearings (IGUS RJUM-01-06) provide 0.025mm radial clearance.

---

## 4.2 ROLLER CARRIAGE ASSEMBLY

### 4.2.1 Carriage Body

4.2.1.1  The roller carriage (P/N RL-CARRIAGE-001) slides along both guide rods. Its principal features:

- Two linear bearing bores (rod_spacing = 85mm center-to-center)
- Central leadscrew nut pocket (M5 hex anti-backlash nut)
- Spring-loaded roller arm pivoting from carriage body
- Wiper terminal boss with M3 screw, connected to external RF terminal via flexible wire

4.2.1.2  **Anti-backlash nut:** A spring-loaded split nut or POM anti-backlash nut eliminates leadscrew backlash. This is critical for position repeatability: without anti-backlash, the carriage may jump 0.5mm when direction reverses, causing ±0.25 turn position error.

### 4.2.2 Roller Arm

4.2.2.1  The pivoting roller arm carries the roller bearing at its lower end. A compression spring between the carriage body and the arm top provides constant-force contact:

```
Spring specs:
  OD:           5mm
  Wire dia:     0.5mm
  Free length:  15mm
  Rate:         ~0.5 N/mm (estimate)
  Pre-load:     5mm compression = 2.5N ← too high!

Use: OD=5mm, free length=15mm, rate=0.1 N/mm stainless music wire spring
  Pre-load 5mm × 0.1 N/mm = 0.5N ✓
```

### 4.2.3 Position Indicator

4.2.3.1  A V-shaped pointer on the carriage front face aligns with a graduated scale on the former. Scale markings every 5 turns allow visual position readout (±1 turn accuracy without electronic counting).

4.2.3.2  The scale is printed on a 10mm wide strip of aluminum adhesive label and applied to the former surface adjacent to the roller travel path. Markings: "5", "10", "15", "20", "25", "30" at corresponding turn positions.

---

## 4.3 DRIVE MECHANISM

### 4.3.1 Worm Gear Reduction

4.3.1.1  The 10:1 worm gear reduction provides:

```
Reduction ratio:    10:1
Worm lead:          2.0mm (single-start)
Worm wheel teeth:   10
Output rotation per input revolution: 1/10 turn
Leadscrew pitch:    2.0mm/revolution
Carriage travel per motor revolution: 2.0mm / 10 = 0.2mm
```

4.3.1.2  The worm gear is **self-locking** when the lead angle is less than the friction angle (approximately 4–5° for steel worm):

```
Lead angle = arctan(lead / (π × pitch_dia))
           = arctan(2.0mm / (π × 20mm))
           = arctan(0.0318) = 1.82°

Friction angle (lubricated steel) ≈ 5–8°
Lead angle (1.82°) < Friction angle → SELF-LOCKING ✓
```

```
╔══════════════════════════════════════════════════════════════════╗
║                             NOTE                                 ║
║                                                                  ║
║  THE SELF-LOCKING PROPERTY OF THE WORM GEAR MEANS THAT          ║
║  MOTOR POWER CAN BE REMOVED AFTER POSITIONING AND THE COIL      ║
║  ROLLER WILL REMAIN AT THE SET POSITION INDEFINITELY, WITHOUT   ║
║  CREEP, EVEN UNDER VIBRATION OR RF FORCES. THIS IS WHY THE      ║
║  FIRMWARE DE-ENERGIZES THE MOTOR AFTER 5 SECONDS OF IDLE.       ║
╚══════════════════════════════════════════════════════════════════╝
```

4.3.1.3  Commercial worm gear sets recommended:
- SDP/SI L1Z6-LGF10-200 (10:1, brass wheel, steel worm)
- Tamiya 70167 Worm Gear Box (suitable for low-power prototype)
- McMaster-Carr 2822T11 + 2822T51 (2mm pitch worm/wheel, 10:1)

### 4.3.2 Leadscrew

4.3.2.1  An M5 leadscrew (5mm OD, 0.8mm pitch) or a dedicated 5mm×2mm-pitch precision leadscrew rod. This design uses 2mm pitch for faster carriage travel speed at a given motor RPM.

4.3.2.2  Leadscrew material: stainless steel (304 or 316), length 100mm. Ends turned to 5mm diameter for bearings (625ZZ at each end).

---

## 4.4 STEPPER MOTOR AND DRIVER

### 4.4.1 NEMA 17 Stepper

4.4.1.1  Stepper motor specifications:

```
Table 4-2. NEMA 17 Stepper Motor Specifications
──────────────────────────────────────────────────────────────
Parameter             Specification
──────────────────────────────────────────────────────────────
Frame size            NEMA 17 (42.3mm × 42.3mm face)
Step angle            1.8° (200 full steps/revolution)
Rated current         1.0 – 1.7A per phase (set 1.0A)
Holding torque        ≥ 40 N·cm at 1.0A
Rotor inertia         ≤ 68 g·cm²
Winding resistance    ~2 Ω/phase
Inductance            ~3 mH/phase
Shaft                 5mm diameter × 24mm
──────────────────────────────────────────────────────────────
```

4.4.1.2  Required motor torque calculation:

```
Carriage friction force: F_friction ≈ 2N (both bearings at 0.5N contact force × coefficient)
Leadscrew efficiency: η ≈ 0.45 (typical for 2mm pitch steel worm gear)
Torque required at motor output: T = F × pitch / (2π × η × gear_ratio)
  = 2N × 0.002m / (2π × 0.45 × 10)
  = 0.000141 N·m = 0.141 N·cm

NEMA 17 holding torque: 40 N·cm >> 0.141 N·cm → Margin: 284:1 ✓
Motor is massively oversized for this load, which ensures reliable operation.
```

### 4.4.2 DRV8825 Driver

4.4.2.1  The DRV8825 stepper driver provides:
- 1/16 microstepping (3200 steps/revolution)
- Decay mode: mixed (default), fast, slow (adjustable)
- Current sensing via external 0.1 Ω resistor
- Thermal shutdown protection
- Fault output (active LOW, open drain)

4.4.2.2  Current setting: Adjust VREF potentiometer on DRV8825 module:

```
I_max = VREF / 0.5Ω    (DRV8825 with 0.1Ω sense resistors, 5× gain)

For I_max = 1.0A: VREF = 0.5V
For I_max = 1.5A: VREF = 0.75V

Measure VREF at the pot wiper with no motor attached.
Start at 0.5V for initial testing.
```

---

# CHAPTER 5 — CONSTRUCTION TECHNIQUES

## 5.1 PRINTED FORMER CONSTRUCTION

### 5.1.1 Printing the Former

**Procedure 5-1: Coil Former Printing**

1. Load `coil_former.scad` in OpenSCAD. Verify dimensions via console output.
2. Export to STL (F6 → Export as STL).
3. Slice settings:
   - Material: PETG (dries at 75°C / 8h before printing)
   - Nozzle: 0.4mm
   - Layer: 0.15mm (important for groove accuracy)
   - Infill: 40% gyroid
   - Perimeters: 3
   - Print temp: 240°C / Bed: 80°C
   - Speed: 40mm/s for walls, 60mm/s fill
4. Orient vertical (Z-axis along former axis). This prints the helix with all layer lines perpendicular to the groove, maximizing groove dimensional accuracy.
5. Print time: approximately 8–12 hours at these settings.
6. Post-process:
   a. Remove any stringing in groove with fine wire brush or dental pick
   b. Sand outer surface lightly with 320-grit to smooth groove entry faces
   c. Check groove pitch with calipers: should be 2.54 ± 0.10mm

### 5.1.2 Ceramic Former Alternative

5.1.2.1  A ceramic or fiberglass tube former is mechanically superior to printed PETG. Suitable materials:

- **Glazed alumina tube** (Coors Tek, 99.5% Al₂O₃), OD 63.5mm, wall 3mm: machine groove on lathe using diamond wheel, pitch 2.54mm, depth 1.0mm
- **G10 fiberglass tube** (McMaster-Carr 9535K25): OD 63.5mm, cut groove with rotary tool and 1/8" ball-end carbide bit

5.1.2.2  Ceramic former advantages:
- Zero moisture absorption
- CTE ~6 ppm/°C (5× better than PETG)
- Higher heat tolerance (1500°C vs. PETG 80°C deflection temp)
- No outgassing in hermetically sealed enclosure

---

## 5.2 COIL WINDING

**Procedure 5-2: Coil Winding**

1. Obtain AWG 12 solid silver-plated copper wire, ~8m.
2. Thread wire through bottom flange exit hole, leaving 150mm terminal lead inside flange.
3. Secure end with terminal lug (M3 ring lug, silver-plated) in bottom terminal hole.
4. Beginning at the winding start mark (40mm from bottom), lay first turn in groove.
5. **Important:** Maintain constant tension (use 1kg hanging weight on wire spool).
6. Wind 30 turns, following each groove in sequence. Do not skip grooves.
7. At last (30th) turn, route wire through top flange exit hole, leaving 150mm terminal lead.
8. Secure with terminal lug at top terminal hole.
9. Inspect: all turns should lie flat in grooves with approximately equal spacing.
10. **Optional (recommended):** Apply a thin coat of clear polyurethane or clear epoxy to the wire where it contacts the grooves to stabilize turns against vibration. Keep the outer wire surface (roller contact zone) free of coating.
11. Allow to cure 24h before installing roller.

```
╔══════════════════════════════════════════════════════════════════╗
║                            CAUTION                               ║
║                                                                  ║
║  DO NOT APPLY SOLDER FLUX, PETROLEUM LUBRICANT, OR ANY          ║
║  DIELECTRIC MATERIAL TO THE OUTER SURFACE OF THE WIRE. THE      ║
║  ROLLER CONTACT REQUIRES DIRECT METAL-TO-METAL CONTACT.         ║
║  EVEN THIN OIL FILMS INCREASE CONTACT RESISTANCE BY 10× OR      ║
║  MORE AT RF FREQUENCIES.                                        ║
╚══════════════════════════════════════════════════════════════════╝
```

---

## 5.3 ROLLER AND CARRIAGE ASSEMBLY

**Procedure 5-3: Carriage Assembly**

1. Press M5 anti-backlash nut into carriage leadscrew pocket. Secure with 2 drops of CA adhesive (not epoxy — needs future serviceability).
2. Press IGUS RJUM-01-06 linear bearings into rod bores (hand pressure sufficient; bearings are designed for press-fit into printed bores).
3. Install roller arm: insert M3×20 stainless pivot pin through arm and carriage body slots.
4. Install compression spring in spring pocket above roller arm.
5. Verify roller arm motion: arm should swing freely, spring should maintain contact pressure.
6. Install roller on arm: thread M3×20 stainless shaft through fork hole, roller bearing, and opposing fork hole. Secure with M3 Nyloc nut. Roller must spin freely on shaft.
7. **Check contact alignment:** With carriage installed on guide rods and leadscrew, roller center should contact wire at the coil OD (±0.5mm vertical, ±0.3mm lateral). Adjust arm geometry if necessary.
8. Attach flexible wiper wire (AWG 16 stranded, bare copper, 200mm long) from wiper boss to main RF terminal block with M3 screws.

---

## 5.4 MOTOR AND GEAR ASSEMBLY

**Procedure 5-4: Motor and Drive Assembly**

1. Install 608ZZ bearings in worm shaft seats (press fit, use bearing press or bench vise with flat plate).
2. Install worm shaft through bearings. Verify smooth rotation.
3. Install worm gear on leadscrew. Torque M3 set screw to 0.3 N·m. Lock with thread-lock.
4. Assemble housing halves. Torque M3 screws to 0.5 N·m. Verify worm mesh: should feel smooth with no binding. Light backlash is acceptable.
5. Apply one drop of light machine oil (ISO VG 32) to worm/gear mesh.
6. Mount motor bracket to housing. Torque M3 screws.
7. Install jaw coupler halves on motor shaft and worm input shaft. Insert elastomer spider.
8. Align shafts to within 0.1mm offset: use dial indicator on each half while rotating.
9. Tighten set screws and apply thread-lock.
10. Thread leadscrew through carriage nut and seat both ends in 625ZZ bearings.

---

# CHAPTER 6 — INSTALLATION

## 6.1 MOUNTING

### 6.1.1 Chassis Mount

6.1.1.1  The roller inductor assembly requires mounting that provides:
- Rigid support (coil weight ≈ 300g, motor weight ≈ 250g)
- RF isolation from chassis ground if the coil is not at ground potential
- Vibration damping for portable field use

6.1.1.2  Mount the assembly on four M3 standoffs (min. 10mm height), using the flange mounting holes. If coil is ground-referenced at one end, aluminum or steel chassis is acceptable. If the full coil is at RF potential (both ends floating), use insulating nylon standoffs.

### 6.1.2 RF Terminal Connections

6.1.2.1  Two RF terminals are provided:

- **Terminal 1 — Fixed end:** Top terminal lug. Connect to the fixed (non-variable) RF circuit node. This terminal carries the full coil inductance when the roller is at minimum position (home).
- **Terminal 2 — Wiper:** Flexible wire from roller carriage to external lug. Connect to the variable tap RF node.

6.1.2.2  Use silver-plated ring lugs (M3 stud, 10mm ring ID) at all RF connection points. Star washers are prohibited (they increase contact resistance). Use flat washers under lug, flat washer over lug, Nyloc nut — torque to 0.4 N·m.

```
╔══════════════════════════════════════════════════════════════════╗
║                            CAUTION                               ║
║                                                                  ║
║  POWER DOWN THE TRANSMITTER AND DISCONNECT DC POWER TO THE      ║
║  MOTOR CONTROLLER BEFORE MAKING OR BREAKING RF CONNECTIONS.     ║
║  DO NOT WORK ON RF CONNECTIONS WHILE RF IS APPLIED.            ║
╚══════════════════════════════════════════════════════════════════╝
```

---

# CHAPTER 7 — OPERATION

## 7.1 INITIAL STARTUP

**Procedure 7-1: Initial Startup and Homing**

1. Verify motor cable connected to DRV8825.
2. Verify limit switch cables connected (home and max switches).
3. Apply 12V DC power to controller.
4. Observe status LED: **fast blink** (motor not homed).
5. Connect Bluetooth terminal or USB serial (115200 baud) to "RF-RollerTuner".
6. Verify startup message: `RF-RollerTuner fw=1.0.0 built=...`

```
╔══════════════════════════════════════════════════════════════════╗
║                            CAUTION                               ║
║                                                                  ║
║  PERFORM THE HOMING SEQUENCE BEFORE ANY MOVEMENT COMMAND.       ║
║  MOVING WITHOUT HOMING WILL USE AN UNKNOWN STARTING POSITION,   ║
║  POTENTIALLY DRIVING THE CARRIAGE INTO THE END STOP AND         ║
║  STALLING THE MOTOR.                                            ║
╚══════════════════════════════════════════════════════════════════╝
```

7. Send command: `HOME`
8. Observe: carriage moves slowly toward home limit switch.
9. When limit switch trips, carriage stops. LED changes to **slow blink** (idle/homed).
10. Verify: `HOMING: complete, position=0`
11. Verify: `STATUS` returns `"homed":true`

```
╔══════════════════════════════════════════════════════════════════╗
║                            CAUTION                               ║
║                                                                  ║
║  DO NOT APPLY RF POWER WHILE THE MOTOR IS MOVING. THE FIRMWARE  ║
║  PROVIDES NO AUTOMATIC TRANSMIT INHIBIT. IF A PTT INTERLOCK IS  ║
║  NOT WIRED, THE OPERATOR MUST ENSURE THE TRANSMITTER IS         ║
║  KEYED ONLY WHEN THE MOTOR IS IDLE (LED = SLOW BLINK).         ║
╚══════════════════════════════════════════════════════════════════╝
```

---

## 7.2 TUNING BY INDUCTANCE VALUE

**Procedure 7-2: Setting Inductance**

1. Ensure homing is complete.
2. Determine required inductance (from tuner calculator, SWR analyzer, or band preset table).
3. Send command: `SETL <value>` (example: `SETL 8.0` for 8.0 µH)
4. Motor moves to calculated position.
5. Verify: serial output shows `ARRIVED: pos=NNNNN steps, L=8.00 µH`
6. Apply low-power test carrier to antenna tuner.
7. Peak for minimum SWR by trimming inductance:
   - Increase inductance: `STEP 8000` (+0.5 turn approximately)
   - Decrease inductance: `STEP -8000`
8. When SWR < 1.5, apply full power.

---

## 7.3 BAND PRESETS

**Procedure 7-3: Using Band Presets**

1. Send command: `BAND 40m` (example for 40m band)
2. Motor moves to preset position for that band.
3. Fine-tune with `STEP` commands as needed.
4. Save the tuned position: `SAVE 40m_dipole`
5. Recall later: `RECALL 40m_dipole`

7.3.1  Available band names: 160m, 80m, 60m, 40m, 30m, 20m, 17m, 15m, 12m, 10m.

7.3.2  If operating frequency is known, use: `FREQ 14.2` — firmware selects the nearest band preset.

---

## 7.4 CYD TOUCHSCREEN OPERATION

7.4.1  Power on CYD display separately (USB-C). CYD automatically connects to "RF-RollerTuner" via Bluetooth.

7.4.2  **LIVE tab:** Shows current inductance (large readout), active turns, position in mm, motor state, inductance bar graph, and 60-point L history sparkline.

7.4.3  **BAND tab:** Grid of 10 band presets. Tap band name to highlight. Tap **GO** to move inductor to that band's preset inductance.

7.4.4  **SWEEP tab:** After a sweep completes (initiated by `SWEEP` command), shows XY chart of commanded vs. actual inductance across the sweep range.

7.4.5  **Bottom bar buttons:**
- **HOME** — triggers homing sequence
- **STEP−** / **STEP+** — moves carriage by 0.5 turn
- **STOP** — immediate motor stop
- **SETL** — moves to selected band preset inductance

---

# CHAPTER 8 — CALIBRATION PROCEDURES

## 8.1 CALIBRATION OVERVIEW

### 8.1.1 What Requires Calibration

8.1.1.1  The roller inductor system has two calibration needs:

1. **Position calibration:** Verify that the step count accurately represents physical carriage position (mm from home). Drift can occur due to missed steps (motor stall), which is detected by comparison with encoder count.

2. **Inductance calibration:** Verify that the Wheeler formula result matches measured inductance. Due to wire diameter variations, former dimensional tolerances, and winding accuracy, actual inductance may differ from calculated by ±5–10%.

### 8.1.2 Equipment Required

```
Table 8-1. Calibration Equipment
──────────────────────────────────────────────────────────────────────
Equipment               Specification              Typical
──────────────────────────────────────────────────────────────────────
Inductance meter        1% accuracy, 10kHz/100kHz  AADE L/C meter II
Calipers                0.02mm resolution          Digital calipers
USB serial terminal     115200 baud                putty, minicom
Bluetooth terminal      SPP capable                phone app
──────────────────────────────────────────────────────────────────────
```

---

## 8.2 POSITION CALIBRATION

**Procedure 8-1: Position Calibration (Step Count Verification)**

1. Home the inductor: `HOME`, wait for completion.
2. Measure physical carriage position with calipers (from home end stop to roller center): should read approximately 3.0mm (home clearance).
3. Send: `GOTO 40000` (moves 2.5mm at 16,000 steps/mm).
4. Measure physical position. Expected: 3.0mm + 2.5mm = 5.5mm.
5. Calculate error: `error_mm = measured − expected`
6. If error > 0.1mm: check for missed steps (stalled motor) or incorrect `STEPS_PER_MM` constant.
7. If steps/mm is incorrect: measure actual pitch (Procedure 8-2) and update `config.h`.

**Procedure 8-2: Leadscrew Pitch Verification**

1. Mark carriage position with marker pen on guide rod.
2. Send: `GOSTEP 32000` (should move exactly 2.0mm at 16,000 steps/mm).
3. Measure displacement with calipers.
4. Calculate actual STEPS_PER_MM:
   ```
   STEPS_PER_MM_actual = 32000 / measured_mm
   ```
5. Update `config.h` if differs from 16000 by more than 1%.

---

## 8.3 INDUCTANCE CALIBRATION

**Procedure 8-3: Wheeler Formula Verification**

1. Home the inductor.
2. Send: `TABLE` — firmware prints inductance lookup table (turns vs. µH calculated).
3. For each 5-turn increment, physically measure inductance:
   a. Send: `SETL X.X` (matching the 5-turn calculated value)
   b. Connect inductance meter to roller wiper terminal and fixed terminal.
   c. Measure inductance at 100 kHz.
   d. Record: `turns, L_calc, L_measured`
4. If L_measured differs from L_calc by more than 5%: determine correction factor.

8.3.1  If a systematic offset exists (measured always reads higher or lower by a constant factor), the coil diameter constant `COIL_DIAMETER_IN` in `config.h` can be adjusted. A 5% high measurement corresponds to a ~2.5% larger effective diameter than nominal.

8.3.2  Point-by-point corrections can be loaded as a lookup table if accuracy better than ±2% is required. See `tuning.h` for the `BandPreset` structure which can be augmented with per-band calibration offsets.

---

# CHAPTER 9 — FIRMWARE REFERENCE

## 9.1 MODULE SUMMARY

```
Table 9-1. Firmware Module Reference
═══════════════════════════════════════════════════════════════════════
File                      Class/Function         Description
═══════════════════════════════════════════════════════════════════════
config.h                  Constants              All configurable values
                          COIL_DIAMETER_IN       Former OD in inches
                          COIL_TURNS_TOTAL       Total wound turns
                          COIL_PITCH_MM          Turn pitch (mm)
                          STEPS_PER_MM           Motion resolution
                          TRAVEL_MAX_MM          Carriage travel limit

position.h                QuadratureEncoder      ISR-driven encoder
                          LimitSwitches          Home/max ISR handler
                          WheelerCalc            Inductance computation
                            calcL(turns)         Wheeler formula → µH
                            turnsForL(uh)        Invert formula (N-R)
                            turnsToMM(turns)     Turns → mm position
                            mmToTurns(mm)        mm → fractional turns
                            lToSteps(uh)         L → stepper steps
                          PositionManager        Motion control
                            startHoming()        Begin homing sequence
                            moveTo(steps)        Absolute step move
                            moveToL(uh)          Move to inductance
                            moveRelative(delta)  Relative step move
                            update()             Call every loop iter.
                            currentL()           Current inductance µH
                            currentTurns()       Current active turns

tuning.h                  BandPreset[]           10-band preset table
                          AutoTuner              Preset management
                            goToBandPreset(f)    Tune by freq
                            goToNamedBand(name)  Tune by band name
                            savePosition(label)  Store current pos
                            recallByLabel(name)  Restore saved pos
                            startSweep(a,b,s)    L sweep command
                            updateSweep()        Advance sweep state
                            printLookupTable()   Dump L table to serial
                          CommandParser          BT/serial command handling
                            process(cmd)         Execute command string

esp32_power_meter.ino     setup()                Hardware initialization
                          loop()                 Main execution loop
                          broadcastStatus()      BT JSON status output
                          updateOLED()           OLED display refresh
                          checkButtons()         Front panel input
═══════════════════════════════════════════════════════════════════════
```

## 9.2 COMMAND REFERENCE

```
Table 9-2. Complete Command Reference
══════════════════════════════════════════════════════════════════════════════
Command          Arguments        Response         Function
══════════════════════════════════════════════════════════════════════════════
HOME             (none)           OK/ERR           Start homing sequence
STATUS           (none)           JSON object      Full status readback
SETL <uh>        float µH         OK               Move to inductance value
GOTO <uh>        float µH         OK               Alias for SETL
GOSTEP <steps>   long integer     OK               Move to absolute step pos
STEP <n>         long ±integer    OK               Relative move (±steps)
BAND <name>      e.g. "40m"       OK/ERR           Move to band preset
FREQ <mhz>       float MHz        OK               Nearest band preset
SAVE <label>     string ≤15 chars OK/ERR           Save current position
RECALL <label>   string           OK/ERR           Recall saved position
SWEEP <s> <e> <t> floats µH      OK               Inductance sweep
TABLE            (none)           CSV table        Print L lookup table
PRESETS          (none)           text list        Print all band presets
LIST             (none)           text list        Print saved positions
STOP             (none)           OK               Immediate motor stop
INFO             (none)           text string      Firmware/coil info
RESET            (none)           (reboots)        Software reset
══════════════════════════════════════════════════════════════════════════════
```

## 9.3 STATUS JSON FORMAT

9.3.1  The STATUS command and automatic 500ms BT broadcasts return:

```json
{
  "l_uh":   "8.302",      // Current inductance (µH)
  "turns":  "16.00",      // Active turns (fractional)
  "pos_mm": "80.64",      // Carriage position (mm from home)
  "steps":  1289872,      // Absolute step count
  "state":  0,            // 0=idle, 1=moving, 2=homing, 3=fault, 4=not homed
  "homed":  true,         // Homing complete
  "moving": false,        // Motor currently running
  "fault":  false,        // DRV8825 fault asserted
  "fw":     "1.0.0"       // Firmware version
}
```

---

# CHAPTER 10 — MAINTENANCE

## 10.1 SCHEDULED MAINTENANCE

```
Table 10-1. Maintenance Schedule
──────────────────────────────────────────────────────────────────────────────
Interval        Item                     Procedure
──────────────────────────────────────────────────────────────────────────────
Before each     Roller surface           Wipe with clean dry cloth;
high-power      cleanliness              no solvents on wire surface
session

Monthly         Contact resistance       Measure R between roller terminal
                check                    and fixed terminal with DVM;
                                         should be < 0.1 Ω at any position

Monthly         Guide rod lubrication    Apply thin film of PTFE dry lube
                                         to both rods; wipe off excess

Monthly         Worm gear inspection     Inspect for wear debris; clean
                                         and re-oil if discolored

Quarterly       Coil wire inspection     Check for abrasion marks at
                                         roller contact zone; look for
                                         copper bright spots through plating

Quarterly       Bearing inspection       Rotate carriage by hand; should
                                         travel full length smoothly

Annually or     Silver replating         Replate roller if contact
as needed       roller                   resistance > 5 mΩ

Annually        Leadscrew cleaning       Remove leadscrew; clean threads;
                                         re-lubricate with Mobiltemp SHC 100
                                         or equivalent synthetic grease
──────────────────────────────────────────────────────────────────────────────
```

---

## 10.2 CONTACT CLEANING PROCEDURE

**Procedure 10-1: Coil Wire Surface Cleaning**

1. Disconnect all RF connections.
2. Power down motor controller.
3. Loosen roller contact pressure spring by backing off one turn on preload adjustment.
4. Move carriage manually to one end.
5. Using clean lint-free cloth (optical cleaning tissue), apply **isopropyl alcohol 99%** to cloth.
6. Wipe along wire surface in direction of coil axis. Do not wipe circumferentially (this risks shorting turns).
7. Repeat with dry cloth to remove residue.
8. For stubborn tarnish (black or brown discoloration): use 3M Scotch-Brite 7447 (non-scratch, gray) with isopropyl alcohol. Very light pressure.
9. Restore spring preload.
10. Verify contact resistance: DVM across roller terminal and fixed terminal, carriage at midpoint. Should read ≤ 0.05 Ω.

**Procedure 10-2: Roller Surface Cleaning**

1. Remove roller from arm (slide M3 shaft out).
2. Inspect: look for scoring, pitting, or plating loss.
3. Clean: IPA on cloth, rub roller surface until no further color transfer.
4. If plating is worn through to brass (bright yellow visible): replate with MG Chemicals 842AR silver paint.
5. Reinstall and verify contact resistance per Procedure 10-1, step 10.

---

## 10.3 BEARING MAINTENANCE

**Procedure 10-3: Linear Bearing Replacement (IGUS RJUM-01-06)**

1. IGUS polymer bearings are maintenance-free and self-lubricating (embedded PTFE).
2. Replace when carriage has measurable radial play (> 0.1 mm): press old bearing out with appropriate mandrel; press new bearing in flush with bore face.
3. Do not lubricate IGUS bearings — this causes contamination of the embedded lubricant.

---

# CHAPTER 11 — TROUBLESHOOTING

## 11.1 DIAGNOSTIC APPROACH

11.1.1  Use the STATUS command and serial monitor as primary diagnostic tools. Check physical condition before replacing components.

```
Table 11-1. Troubleshooting Guide
═══════════════════════════════════════════════════════════════════════════════════
Symptom                    Probable Cause           Corrective Action
═══════════════════════════════════════════════════════════════════════════════════
Motor does not move        Motor not enabled         Check PIN_ENABLE = LOW
                           DRV8825 fault             Read FAULT pin; check
                                                     VREF setting (<0.75V)
                           No 12V supply             Check J1 power supply
                           Motor cable               Check 4-wire motor cable
                                                     color code

Motor moves but no         Coupler slipping          Retighten set screws
carriage movement          Leadscrew nut stripped    Inspect nut; replace
                           Carriage binding          Check rod straightness

Homing fails (carriage     Home switch not wiring    Check GPIO34 and switch
moves max direction)       Default state wrong       Verify switch NO vs NC
                           Inverted DIR              Swap motor cable A+ and A-

Homing overshoots          Speed too high            Reduce MOTOR_SPEED_HOME
                           Debounce delay            Check LIM_DEBOUNCE_MS

Position drifts after      Missed steps (motor stall) Reduce speed or increase
repeated moves             Backlash in leadscrew      current; install anti-
                                                      backlash nut

Inductance reading          Wrong COIL_DIAMETER_IN    Re-measure coil and update
consistently off by         in config.h               config.h
fixed percent

Contact resistance high    Dirty roller/wire          Clean per Procedure 10-1
(> 0.1 Ω at midpoint)      Worn silver plating       Replate roller
                           Spring preload too low     Increase spring preload
                           Roller misaligned          Adjust arm geometry

High contact heating at    Excessive contact R        Check and clean contact
high power (smell,         Wrong wire gauge           Verify AWG 12 not lighter
discoloration)             Loose terminal connection  Retorque all terminal nuts

SWR rises after coil       Coil wire arced            Inspect wire at roller
operates at high power     (turned under RF!)         zone for arc pits; rewind
                                                      if severe

BT not connecting          Wrong device name          Verify BT_DEVICE_NAME
                           Already paired to other    Clear BT pairing on phone
                           Low signal                 Move within 5m of device

Steps/mm wrong             Incorrect gear ratio       Re-measure and verify
                           in config.h               GEAR_RATIO constant
                           Worm gear slipping         Check set screws; verify
                                                      gear is keyed to shaft

OLED not displaying        Wrong I2C address          Check OLED_ADDR (0x3C or 0x3D)
                           SDA/SCL swapped            Verify GPIO21=SDA, 22=SCL

DRV8825 FAULT output       Over-temperature           Improve driver ventilation
asserted                   Overcurrent                Reduce VREF below 0.5V
                           Short circuit              Check motor cable for short
═══════════════════════════════════════════════════════════════════════════════════
```

---

# APPENDIX A — SPECIFICATIONS

```
Table A-1. Complete Specifications
═══════════════════════════════════════════════════════════════════════════
PARAMETER                                    SPECIFICATION
═══════════════════════════════════════════════════════════════════════════
ELECTRICAL
  Inductance range (calculated)              0.5 – 34 µH
  Inductance range (usable, > Q=100)         1.0 – 30 µH
  Minimum step (motorized)                   0.003 µH @ L=1 µH
  Current rating, continuous                 10 A RMS
  Current rating, peak (SSB envelope)        20 A (< 10ms)
  Voltage rating                             > 5 kVRMS
  Power rating, continuous                   500 W @ 50Ω match
  Power rating, PEP                          1500 W PEP
  Q factor @ 7 MHz, 20 µH                    ≥ 300 (clean contacts)
  Q factor @ 14 MHz, 8 µH                    ≥ 250 (clean contacts)
  Insertion loss @ 7 MHz (in-circuit)        < 0.1 dB
  Self-resonant frequency (full inductance)  ≥ 15 MHz (estimated)
  DC resistance (full winding)               < 30 mΩ (AWG 12, 6m)
  Contact resistance                         < 5 mΩ (clean silver)

MECHANICAL — COIL ASSEMBLY
  Former OD                                  63.5 mm (2.500 in)
  Former ID                                  57.5 mm
  Former material                            PETG or ceramic
  Total length (with flanges)                188 mm
  Winding turns                              30
  Wire gauge                                 AWG 12
  Wire type                                  Solid silver-plated copper
  Winding pitch                              2.54 mm (10 TPI)
  Active winding length                      76.2 mm
  Guide rod diameter                         6 mm
  Guide rod material                         Ground stainless (304)

MECHANICAL — DRIVE
  Gear ratio (worm)                          10:1
  Worm gear self-locking                     Yes (lead angle < friction angle)
  Leadscrew pitch                            2.0 mm/rev
  Carriage travel range                      80 mm
  Stepper motor                              NEMA 17, 200 steps/rev, 1.0A

CONTROLLER
  Microcontroller                            ESP32-WROOM-32
  Stepper driver                             DRV8825, 1/16 microstep
  Position resolution                        0.000125 mm/step
  Homing method                              Mechanical limit switch
  Position memory                            NVS (non-volatile)
  Control interface                          BT SPP + USB 115200 baud
  Display (optional)                         CYD ILI9341 320×240 TFT
  Power supply                               12V DC, 2A

ENVIRONMENTAL
  Operating temperature                      −10 to +55°C (PETG former)
  Storage temperature                        −20 to +70°C
  Humidity                                   0–95% non-condensing
  Weatherproofing                            IP44 (with enclosure)
═══════════════════════════════════════════════════════════════════════════
```

---

# APPENDIX B — INDUCTANCE TABLE

```
Table B-1. Wheeler's Formula Inductance vs. Turns
(d = 2.500 in, pitch = 2.54 mm = 0.100 in/turn)
═══════════════════════════════════════════════════════════════════════════
Turns    l (in)   L calc   X_L(3.5) X_L(7.0) X_L(14)  X_L(21)  X_L(28)
         (length) (µH)     (Ω @80m)  (Ω @40m)  (Ω @20m) (Ω @15m) (Ω @10m)
═══════════════════════════════════════════════════════════════════════════
 2       0.20      0.15      3.3       6.6      13.2     19.8     26.4
 4       0.40      0.53     11.7      23.4      46.7     70.1     93.4
 5       0.50      0.82     18.0      36.0      72.0    108.0    144.0
 6       0.60      1.17     25.7      51.4     102.8    154.2    205.6
 7       0.70      1.58     34.7      69.4     138.7    208.1    277.5
 8       0.80      2.04     44.8      89.6     179.2    268.8    358.4
 9       0.90      2.56     56.3     112.5     225.0    337.5    450.0
10       1.00      3.12     68.5     137.0     274.0    411.0    548.0
11       1.10      3.73     82.0     163.9     327.8    491.7    655.6
12       1.20      4.39     96.6     193.2     386.4    579.6    772.8
13       1.30      5.09    112.0     223.9     447.8    671.7    895.6
14       1.40      5.83    128.1     256.2     512.4    768.6   1024.8
15       1.50      6.61    145.4     290.7     581.5    872.2   1163.0
16       1.60      7.43    163.4     326.7     653.5    980.2   1307.0
17       1.70      8.28    182.1     364.1     728.2   1092.3   1456.5
18       1.80      9.17    201.7     403.4     806.9   1210.3   1613.7
19       1.90     10.09    221.8     443.7     887.3   1331.0   1774.6
20       2.00     11.04    242.7     485.5     970.9   1456.4   1941.8
21       2.10     12.02    264.4     528.9    1057.8   1586.6   2115.5
22       2.20     13.03    286.7     573.4    1146.8   1720.1   2293.5
23       2.30     14.06    309.3     618.5    1237.1   1855.6   2474.1
24       2.40     15.11    332.4     664.8    1329.7   1994.5   2659.4
25       2.50     16.19    356.2     712.3    1424.7   2137.0   2849.4
26       2.60     17.28    380.2     760.5    1521.0   2281.5   3042.0
27       2.70     18.40    404.8     809.6    1619.2   2428.9   3238.5
28       2.80     19.53    429.7     859.4    1718.7   2578.1   3437.5
29       2.90     20.68    454.8     909.7    1819.4   2729.1   3638.7
30       3.00     21.84    480.4     960.7    1921.4   2882.2   3843.0
═══════════════════════════════════════════════════════════════════════════
Note: X_L values are reactive impedance in Ω at each band center frequency.
All values computed from Wheeler's formula. Measure actual coil for precision.
═══════════════════════════════════════════════════════════════════════════
```

---

# APPENDIX C — BILL OF MATERIALS

```
Table C-1. Mechanical Components
══════════════════════════════════════════════════════════════════════════════
Item   Qty  Description                      Supplier        Part/Note
══════════════════════════════════════════════════════════════════════════════
COIL ASSEMBLY
F1     1    Former (PETG printed or G10 tube)  Print/McMaster  coil_former.scad
W1     1    AWG 12 silver-plated copper wire   Belden 9369     8m
LUG1   2    Ring lug, M3, silver-plated        Panduit LCA3-5   
SCAD1  2    Flange standoff M3×10mm            McMaster 92320A120
ROD1   2    Ground steel rod, 6mm×200mm        McMaster 1265K98

CARRIAGE ASSEMBLY
C1     1    Carriage body (PETG printed)       Print           roller_carriage.scad
A1     1    Roller arm (PETG printed)          Print           roller_carriage.scad
BRG1   2    Linear bearing IGUS RJUM-01-06     IGUS            6mm bore
BRG2   1    Roller bearing (silver-plated)     McMaster        5912K713 or equiv
NUT1   1    Anti-backlash nut M5               Amazon          POM split nut
PIN1   1    Pivot pin M3×20mm stainless        McMaster
SPR1   1    Compression spring 5mm OD          McMaster        9657K245 (0.1N/mm)
SCR1   2    M3×20 stainless SHCS               McMaster

DRIVE ASSEMBLY
GBX1   1    Worm gear set 10:1, 2mm pitch      SDP/SI          L1Z6 or equiv
LS1    1    Leadscrew M5×2mm pitch, 100mm      McMaster        98940A556
BRG3   2    625ZZ bearing (5mm bore, 16mm OD)  Amazon
BRG4   2    608ZZ bearing (8mm bore, 22mm OD)  Amazon
CPL1   1    Jaw coupler 20mm/5mm/5mm           Amazon          20mm L-050 equiv

HARDWARE
SC_M3  24   M3 SHCS various lengths            McMaster
NUT_M3 12   M3 Nyloc nut                       McMaster
WAS_M3 20   M3 flat washer                     McMaster
SC_M5  4    M5×20 SHCS (leadscrew retention)   McMaster
══════════════════════════════════════════════════════════════════════════════

Table C-2. Electronic Components (Motorized Controller)
══════════════════════════════════════════════════════════════════════════════
Item   Qty  Description                      Part Number       Value/Note
══════════════════════════════════════════════════════════════════════════════
U1     1    ESP32-WROOM-32 module            Espressif         38-pin dev board
U2     1    DRV8825 stepper driver module    Pololu 2133       or clone
U3     1    LM7805 +5V regulator             LM7805CT          TO-220
D1     1    Rectifier diode                  1N4007            Polarity protect
M1     1    NEMA 17 stepper motor            17HS4401          200 step, 1.7A
SW1    1    Limit switch home (SPDT)         Omron SS-10GL2    or equiv
SW2    1    Limit switch max (SPDT)          Omron SS-10GL2
ENC1   1    Incremental encoder 100PPR       Bourns EMS22A     Optional
J1     1    DC barrel jack 2.1mm             CUI PJ-002A
J2     1    4-pin Molex connector (motor)    Molex 22-01-3047
J3     1    2-pin screw terminal (RF)        Phoenix 1803277
C1     1    Electrolytic cap 470µF/25V       Panasonic ECA     Bulk filter
C2     1    Electrolytic cap 10µF/16V        (decoupling LDO)
C3     1    Film cap 100nF                   (decoupling LDO)
C4     1    Electrolytic cap 100µF/25V       DRV8825 VMOT
R1     1    Trim pot 1kΩ                     Bourns 3386P      VREF adjust
R2,R3  2    Resistor 10kΩ 1% 0805           (limit sw pullup)
LED1   1    Status LED green 0805            
R_LED  1    Resistor 330Ω 0805              (LED current)
OLED   1    SSD1306 OLED 128×64 I2C         0.96" module      Optional
CYD    1    ILI9341 320×240 TFT touchscreen  "CYD" board       Optional separate
══════════════════════════════════════════════════════════════════════════════
```

---

# APPENDIX D — BAND PRESET REFERENCE

```
Table D-1. Band Preset Inductance Values
(Center-fed dipole at resonance, 50Ω source, series-L L-network)
══════════════════════════════════════════════════════════════════════════════
Band    Freq    L preset  Turns  Steps     X_L @ freq   Notes
        (MHz)   (µH)      (calc) (approx)  (Ω)
══════════════════════════════════════════════════════════════════════════════
160m    1.90    28.0      28.7   —         334 Ω        Near full coil
80m     3.70    18.0      25.0   —         419 Ω
60m     5.30    12.0      22.0   —         399 Ω
40m     7.10     8.0      19.0   —         357 Ω
30m    10.10     5.5      16.5   —         349 Ω
20m    14.20     3.5      13.3   —         313 Ω
17m    18.10     2.2      10.8   —         250 Ω
15m    21.20     1.5       9.1   —         200 Ω
12m    24.90     1.1       8.0   —         172 Ω
10m    28.50     0.8       6.9   —         143 Ω
══════════════════════════════════════════════════════════════════════════════
NOTE: These presets are STARTING POINTS for a typical dipole or random wire.
Actual required inductance depends on antenna impedance at your operating
frequency. Fine-tune for minimum SWR using the STEP command.

RECALL SAVED POSITIONS: After tuning each band once, use SAVE <label> to
store the exact position. Future QSOs on that band/frequency: use RECALL.
══════════════════════════════════════════════════════════════════════════════
```

---

```
════════════════════════════════════════════════════════════════════════
END OF TM-ROLLER-001 Rev A
ROLLER INDUCTOR SYSTEM TECHNICAL MANUAL

Document Number:  TM-ROLLER-001
Revision:         A
Date:             2026-05-01

All firmware source files:
  /home/merv/Public/claude/ant_gear/roller_inductors/firmware/

OpenSCAD enclosures:
  /home/merv/Public/claude/ant_gear/roller_inductors/enclosures/

ASCII schematics:
  /home/merv/Public/claude/ant_gear/roller_inductors/schematics/
════════════════════════════════════════════════════════════════════════
```
