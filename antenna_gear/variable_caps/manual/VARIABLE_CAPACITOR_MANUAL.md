# VARIABLE CAPACITOR SYSTEM
## Technical Manual TM-VCAP-001 Rev A
### Motorized Air Variable Capacitor for HF Antenna Tuning Systems

---

**Document Control**

| Item | Value |
|------|-------|
| Document Number | TM-VCAP-001 |
| Revision | A |
| Date | 2026-05-01 |
| Author | Merv Martin — KG6VJF |
| Classification | Amateur / Educational |
| Applicability | All TM-VCAP-001 series hardware |

---

> **WARNING**
> This equipment operates at RF potentials of hundreds to thousands of volts during transmit.
> At 1 kW into a reactive load, the capacitor shaft and plates may be at 3,000–10,000 V RF.
> De-energize the transmitter before touching any internal component.
> Failure to observe this warning may result in RF burn injury or cardiac event.

---

## TABLE OF CONTENTS

1. [Introduction](#1-introduction)
2. [Theory of Operation](#2-theory-of-operation)
3. [Variable Capacitor Design](#3-variable-capacitor-design)
4. [Butterfly and Split-Stator Configurations](#4-butterfly-and-split-stator-configurations)
5. [Vacuum Variable Capacitors](#5-vacuum-variable-capacitors)
6. [Drive and Position Sensing System](#6-drive-and-position-sensing-system)
7. [High-Voltage Isolation](#7-high-voltage-isolation)
8. [Construction and Assembly](#8-construction-and-assembly)
9. [Controller Firmware Architecture](#9-controller-firmware-architecture)
10. [CYD Touchscreen Display](#10-cyd-touchscreen-display)
11. [Calibration Procedures](#11-calibration-procedures)
12. [Operating Procedures](#12-operating-procedures)
13. [Maintenance](#13-maintenance)
14. [Troubleshooting](#14-troubleshooting)

**Appendices**
- [A: Specifications](#appendix-a-specifications)
- [B: Bill of Materials](#appendix-b-bill-of-materials)
- [C: Capacitor Design Worksheet](#appendix-c-capacitor-design-worksheet)
- [D: Band Capacitance Reference](#appendix-d-band-capacitance-reference)

---

## 1. INTRODUCTION

### 1.1 Purpose

This manual provides technical description, construction guidance, calibration procedures, and operating instructions for the TM-VCAP-001 Motorized Variable Capacitor System. The system is intended for use as the tuning element in HF antenna matching networks covering 1.8–30 MHz.

### 1.2 Scope

This manual covers:

a. Parallel-plate air variable capacitors, 10–500 pF range
b. Butterfly (split-stator) configuration for balanced tuners
c. Vacuum variable capacitors for >1 kW applications
d. 10:1 reduction drive with motorized control
e. ESP32 stepper motor controller firmware
f. CYD touchscreen display interface
g. High-voltage isolation requirements and techniques

### 1.3 Related Documents

| Document | Title |
|----------|-------|
| TM-RLIND-001 | Motorized Roller Inductor Manual |
| TM-PWRMTR-001 | RF Power Meter System Manual |
| TM-VCAP-ESP32-001 | Controller Firmware Release Notes |
| TM-VCAP-CYD-001 | CYD Display Firmware Release Notes |

### 1.4 System Overview

The TM-VCAP-001 system consists of:

1. **Variable capacitor assembly** — parallel plate air variable, 10–500 pF, 2–5 kV rating
2. **Reduction drive** — GT2 belt, 10:1 ratio, NEMA 17 stepper input
3. **HV insulating coupler** — 50 mm Delrin, 5 kV isolation
4. **Position sensor** — 10 kΩ rotary potentiometer on capacitor shaft
5. **Controller** — ESP32-WROOM-32, DRV8825 driver, 1/16 microstepping
6. **Display** — ESP32-2432S028 CYD touchscreen (optional remote display)
7. **Enclosure** — IP54 weatherproof PETG, 195×135×88 mm

---

## 2. THEORY OF OPERATION

### 2.1 Parallel Plate Capacitance

The capacitance of a parallel plate capacitor is given by:

```
C = ε₀ × εᵣ × A × (N − 1) / d
```

Where:

| Symbol | Quantity | Units |
|--------|----------|-------|
| C | Capacitance | farads |
| ε₀ | Permittivity of free space | 8.854 × 10⁻¹² F/m |
| εᵣ | Relative permittivity of dielectric | dimensionless (1.0 for air) |
| A | Area of one plate | m² |
| N | Total number of plates (rotor + stator) | dimensionless |
| d | Gap between adjacent plates | meters |

For circular plates of diameter D:

```
A = π × (D/2)²
```

The (N − 1) factor accounts for the fact that N plates form (N − 1) capacitive gaps.

#### 2.1.1 Design Example

For a typical HF tuner capacitor:
- N = 41 plates (21 rotor + 20 stator)
- D = 50 mm (plate diameter)
- d = 2.0 mm (plate gap)
- εᵣ = 1.0 (air dielectric)

```
A = π × (0.025)² = 1.963 × 10⁻³ m²
C_max = (8.854×10⁻¹²) × 1.0 × (1.963×10⁻³) × (41−1) / (2.0×10⁻³)
C_max = 347 pF
```

> **NOTE**
> This is the theoretical maximum with full plate overlap. Actual maximum capacitance
> is typically 70–80% of theoretical due to plate edge effects and rotor end-gap.

### 2.2 Angular Variation of Capacitance

For semicircular vanes (the standard construction for air variables), the capacitance varies linearly with rotation angle:

```
C(θ) = C_min + (C_max − C_min) × (θ / 180°)
```

Where θ is the rotor angle from minimum engagement (0°) to maximum engagement (180°).

This linear relationship is the fundamental reason semicircular vanes are used — it allows simple linear interpolation for firmware position control.

> **NOTE**
> Non-semicircular vane shapes produce non-linear C vs. angle curves. Logarithmic-taper
> vanes are used in some transmitter applications for uniform reactance change per degree.
> The TM-VCAP-001 design uses standard semicircular vanes.

### 2.3 Reactance and Impedance

Capacitive reactance at frequency f:

```
X_C = 1 / (2π × f × C)
```

At minimum capacitance (C_min), reactance is maximum — the capacitor presents the highest impedance. At maximum capacitance (C_max), reactance is minimum.

#### 2.3.1 Reactance at HF Bands (typical values)

| Band | Freq (MHz) | X_C at 10 pF (Ω) | X_C at 100 pF (Ω) | X_C at 250 pF (Ω) |
|------|-----------|-------------------|--------------------|--------------------|
| 160m | 1.9 | 8,378 | 838 | 335 |
| 80m | 3.6 | 4,420 | 442 | 177 |
| 40m | 7.1 | 2,242 | 224 | 90 |
| 30m | 10.1 | 1,575 | 158 | 63 |
| 20m | 14.2 | 1,121 | 112 | 45 |
| 17m | 18.1 | 879 | 88 | 35 |
| 15m | 21.2 | 751 | 75 | 30 |
| 12m | 24.9 | 640 | 64 | 26 |
| 10m | 28.5 | 559 | 56 | 22 |

### 2.4 Self-Resonance

A variable capacitor has stray inductance from its plates, frame, and connections. Self-resonant frequency:

```
f_SRF = 1 / (2π × √(L_stray × C_max))
```

Typical stray inductance for a multi-plate air variable: 5–20 nH. For C_max = 250 pF and L_stray = 10 nH:

```
f_SRF = 1 / (2π × √(10×10⁻⁹ × 250×10⁻¹²))
f_SRF ≈ 318 MHz
```

This is well above the HF range, so self-resonance is not a concern for frequencies below 30 MHz.

### 2.5 Current Rating

The current through the capacitor at frequency f when connected in a tuner:

```
I = V_RF / X_C = V_RF × 2π × f × C
```

For a 50 Ω system at 1 kW:

```
V_rms = √(P × R) = √(1000 × 50) = 224 V_rms
I_rms (at X_C = 50 Ω) = 224 / 50 = 4.5 A_rms
I_peak = 4.5 × √2 = 6.4 A_peak
```

The primary current path is through the wiping contact between rotor and stator. Contact resistance must be minimized.

### 2.6 Voltage Rating

#### 2.6.1 Air Dielectric Breakdown

Air breakdown field at sea level: approximately 3 kV/mm at 50 Hz. At radio frequencies, breakdown threshold is somewhat lower due to corona initiation.

Conservative RF operating voltage rating:

```
V_operating = d_mm × 2.0 kV/mm (50% safety factor)
```

For d = 2.0 mm gap: V_operating = 4.0 kV peak

#### 2.6.2 Peak RF Voltage Calculation

The peak voltage appearing across the capacitor depends on its reactance and the RF current:

```
V_peak = I_peak × X_C = I_peak / (2π × f × C)
```

Alternatively, from power and reactance:

```
V_peak = √(2 × P × X_C)
```

#### 2.6.3 Voltage Rating Verification Procedure

For each intended operating condition, verify:

```
V_peak < V_operating_rating (with ≥2× safety margin preferred)
```

| Power (W) | Freq (MHz) | C (pF) | X_C (Ω) | V_peak (V) | Rating (kV) | Status |
|-----------|-----------|--------|---------|-----------|------------|--------|
| 100 | 14.2 | 250 | 45 | 95 | 4.0 | SAFE |
| 100 | 14.2 | 10 | 1,121 | 474 | 4.0 | SAFE |
| 1000 | 14.2 | 10 | 1,121 | 1,498 | 4.0 | SAFE |
| 1000 | 7.1 | 10 | 2,242 | 2,117 | 4.0 | CAUTION |
| 1000 | 3.6 | 10 | 4,420 | 2,974 | 4.0 | WARNING |

> **WARNING**
> At 1 kW on 80m or 160m with a small capacitor value, voltage may approach or exceed
> the capacitor's rating. Use the VTABLE command to verify voltage ratings before
> transmitting at high power. Exceeding the voltage rating causes visible arcing and
> immediate plate damage.

---

## 3. VARIABLE CAPACITOR DESIGN

### 3.1 Plate Geometry

#### 3.1.1 Rotor and Stator Configuration

A standard variable capacitor consists of:

- **Stator plates** — fixed to the frame, connected to one RF terminal
- **Rotor plates** — mounted on the shaft (interleaved with stator), connected to the other RF terminal via the shaft bearing (wiping contact)
- **Vane shape** — typically semicircular (D-shaped) for linear C vs. angle

The rotor plates are offset from stator plates by half a pitch so that when they intermesh, there is equal gap on both sides of each rotor plate.

#### 3.1.2 Plate Thickness and Spacing

| Parameter | Low Power (<100W) | Medium Power (100–500W) | High Power (>500W) |
|-----------|------------------|------------------------|--------------------|
| Plate material | Aluminum 6061 | Aluminum 6061 | Aluminum 6061 or brass |
| Plate thickness | 0.4–0.8 mm | 0.8–1.2 mm | 1.2–2.0 mm |
| Plate gap | 1.0–1.5 mm | 1.5–2.5 mm | 2.5–5.0 mm |
| Frame | Aluminum | Aluminum | Heavy aluminum |
| Shaft bearing | Bronze or ball | Stainless ball | Heavy stainless ball |

#### 3.1.3 Plate Count Selection

More plates increase maximum capacitance but increase physical size. Typical selections:

| Application | N (plates) | C_max (pF, 50mm dia, 2mm gap) |
|-------------|-----------|-------------------------------|
| QRP tuner | 11 | 87 pF |
| 100W tuner | 21 | 174 pF |
| 1 kW tuner | 41 | 347 pF |
| Transmatch | 61 | 520 pF |

### 3.2 Frame Construction

#### 3.2.1 Commercial vs. Homebrew Frames

**Commercial surplus capacitors** (strongly recommended for most builders):

- Surplus military capacitors (Cardwell, Johnson, Hammarlund) are readily available
- Known voltage and current ratings
- Pre-aligned plates, precision bearings
- Available on eBay, surplus dealers, hamfests, $5–$50

**Homebrew construction** is appropriate when:

- Specific capacitance range or form factor is required
- Commercial units are unavailable
- Educational exercise

#### 3.2.2 Frame Material

- Aluminum alloy 6061-T6 for frame and plates (nonmagnetic, low RF loss)
- Stainless steel shaft (nonmagnetic, corrosion resistant)
- Bronze or stainless ball bearings at shaft support points
- Nylon standoffs or PTFE washers where RF isolation from chassis is required

#### 3.2.3 Plate Alignment

Plate alignment is critical. Misaligned plates reduce capacitance, increase corona onset voltage, and may cause mechanical contact between rotor and stator.

- Stator spacing set by threaded spacer rods
- Rotor plate spacing set by shaft-mounted spacers
- Total stack deflection must be < 0.1 mm under maximum torque

### 3.3 Wiper Contact Design

The rotor connection to the RF circuit is made through a wiping contact bearing on the rotor shaft. This is the highest-loss element in the capacitor at high current.

#### 3.3.1 Contact Materials

| Material | Contact Resistance | Corrosion Resistance | Current Rating |
|----------|-------------------|---------------------|----------------|
| Phosphor bronze | 1–5 mΩ | Good | Medium |
| Beryllium copper | 0.5–2 mΩ | Excellent | High |
| Silver-plated Be-Cu | 0.1–0.5 mΩ | Excellent | High |
| Gold-plated | 0.1–0.3 mΩ | Excellent | Low-medium |

For >500W operation: silver-plated beryllium copper spring contact.

#### 3.3.2 Contact Force

Contact force must be sufficient to maintain low resistance without causing excessive shaft torque or wear.

- Minimum spring force: 2–5 N for RF power to 1 kW
- Too little force → intermittent contact, arcing at high current
- Too much force → excessive bearing wear, increased drive torque

#### 3.3.3 Contact Cleaning

Silver contacts tarnish in air, forming silver sulfide (Ag₂S). Although Ag₂S has higher resistivity than silver, contact pressure usually maintains adequate conductivity in normal operation.

Clean contacts with:
- Caig DeoxIT Gold G5 contact cleaner (preferred)
- Isopropyl alcohol (90%+ purity)
- Do NOT use abrasive cleaners — they remove silver plating

---

## 4. BUTTERFLY AND SPLIT-STATOR CONFIGURATIONS

### 4.1 Balanced Tuner Requirements

In balanced (antenna tuner) and ladder-line applications, neither conductor should be at RF ground. A conventional single-section variable capacitor grounds one terminal through the frame and chassis, unbalancing the circuit.

A **split-stator** capacitor has:
- Rotor (shaft): at RF ground or isolated
- Stator section A: connected to balanced line terminal 1
- Stator section B: connected to balanced line terminal 2

Both stator sections are electrically isolated from the frame.

### 4.2 Butterfly Capacitor Construction

The **butterfly** or **double-section** variable capacitor provides equal capacitance on both sides of the rotor:

```
                Section A    Section B
Stator A  ─────┤            ├───── Stator B
               │  ← Rotor → │
Shaft/GND ─────┘            └───── (or float)
```

Both sections track simultaneously as the shaft rotates. For a balanced tuner, connect the two stator sections to the balanced transmission line. The rotor/shaft may be at circuit ground or left floating with a shaft-isolating coupler.

### 4.3 Differential Capacitance

In some tuner designs, the two sections are wired in series or with opposite phase to provide differential capacitance — as one section increases, the other decreases. This extends the useful capacitance range and improves symmetry.

Differential connection:
```
Line 1 → Stator A → [series path] → Stator B → Line 2
```

Mid-rotation (90°): both sections at C_max/2. At full rotation: maximum asymmetry.

### 4.4 Motor Drive for Split-Stator

The split-stator capacitor uses the same drive system as the single-section capacitor. The shaft rotation produces equal and simultaneous change in both sections.

> **CAUTION**
> In a split-stator capacitor driven by a motor, the shaft may be at RF potential on both
> sides if the rotor is connected to neither ground nor any circuit node. The insulating shaft
> coupler requirement (Section 7) still applies unless the rotor is intentionally grounded.

---

## 5. VACUUM VARIABLE CAPACITORS

### 5.1 Principles

Vacuum variable capacitors replace the air dielectric with vacuum (< 10⁻⁶ torr), which provides:

- Dielectric strength: ~30 kV/mm (vs. ~3 kV/mm for air)
- No corona, no arcing at operating voltages
- No atmospheric contamination
- Lower RF loss (no gas molecules to ionize)

Vacuum variables are used for:
- Power levels >1 kW where air variables are marginal
- High-VSWR environments where voltage swing is unpredictable
- Compact high-voltage tuners

### 5.2 Construction

A vacuum variable consists of two coaxial metal cylinders (usually copper or Kovar) sealed in a glass or ceramic envelope with the interior evacuated. Capacitance is varied by a bellows mechanism that changes the depth of engagement of the inner cylinder.

Typical vacuum variables:
- Range: 7–500 pF (various types)
- Voltage rating: 5–30 kV
- Current rating: 25–50 A continuous
- Adjustment travel: 12–25 mm linear (bellows stroke)

### 5.3 Drive Adaptation for Vacuum Variables

Vacuum variables require **linear actuation** (not rotary), and their adjustment force is higher than air variables due to the bellows spring force.

Adaptation for TM-VCAP-001 motorized drive:

1. Replace rotary reduction drive with leadscrew linear actuator
2. Use NEMA 17 stepper + 8mm T8 leadscrew (2mm pitch)
3. Position sensing: replace rotary pot with linear pot or optical encoder strip
4. Modify firmware: steps → mm travel (not degrees)
5. Insulating coupler: Delrin leadscrew nut block, minimum 25 mm length between metal parts

> **WARNING**
> Never attempt to disassemble a vacuum variable capacitor or drill into the glass/ceramic
> envelope. Loss of vacuum renders the device worthless and may cause implosion with ceramic
> shards. Vacuum variables damaged by overvoltage (internal arcing) must be replaced, not repaired.

### 5.4 Handling and Mounting

- Handle vacuum variables by the metal end fittings only, never the glass/ceramic body
- Mount vertically if possible (bellows at bottom) to prevent condensation in the bellows groove
- Insulate the lower (fixed) terminal from chassis if it is at RF potential
- Connect RF with short, heavy-gauge leads to minimize stray inductance
- Maximum lead length from vacuum variable to circuit node: 25 mm

---

## 6. DRIVE AND POSITION SENSING SYSTEM

### 6.1 Reduction Drive — Belt System

The TM-VCAP-001 uses a GT2 belt reduction drive for the following reasons:

- **No backlash** (zero-backlash with proper tension)
- **Low noise** compared to gear drives
- **Simple construction** — two pulleys and a belt
- **Easy ratio change** — swap pulleys for different ratio

Standard ratio: 10:1
- Motor pulley: 20T GT2 (13 mm OD)
- Output pulley: 200T GT2 (127 mm OD) — OR two stages: 20T → 60T (3:1) + 20T → 60T (3:1) = 9:1

> **NOTE**
> A 200T single pulley (127 mm OD) barely fits in the enclosure. The two-stage approach
> (two belt runs, two intermediate shafts) is mechanically complex. The recommended approach
> is a commercial worm gear set for exactly 10:1 with no backlash and a smaller footprint.
> Commercial source: SDP/SI P/N L1Z6-LGF10-200, McMaster 6236K48.

#### 6.1.1 Belt Drive vs. Worm Gear Comparison

| Property | GT2 Belt | Worm Gear |
|----------|----------|-----------|
| Backlash | ~0 (with tension) | ~0.5–2° at output |
| Self-locking | NO | YES (if lead angle < friction angle) |
| Motor hold required | YES | NO |
| Efficiency | 95% | 50–70% |
| Construction | Simple | Moderate |
| Noise | Low | Low |
| Cost | Low | Moderate |

> **CAUTION**
> Unlike a worm gear drive, the GT2 belt is NOT self-locking. If motor power is removed,
> the capacitor shaft will drift under its own return spring (if present). The firmware
> #ifdef CAP_GEAR_SELF_LOCKING guard determines whether the motor current is removed
> after movement. With belt drive: this flag must NOT be defined — the motor must remain
> energized at hold current to maintain position.

### 6.2 Angular Resolution

With NEMA 17 (200 steps/rev), 1/16 microstepping, and 10:1 reduction:

```
Steps per revolution at cap shaft = 200 × 16 × 10 = 32,000
Degrees per step = 360 / 32,000 = 0.01125°/step
Steps for full 180° travel = 180 / 0.01125° = 16,000 steps
```

This provides approximately 0.01° angular resolution at the capacitor shaft, which corresponds to a capacitance resolution of:

```
ΔC per step = (C_max − C_min) / (steps for 180°)
           = (250 − 10) / 16,000 = 0.015 pF/step
```

This resolution is more than adequate for all HF antenna tuning applications.

### 6.3 Position Sensing — Potentiometer

The TM-VCAP-001 uses a 10 kΩ rotary potentiometer coupled to the capacitor shaft (NOT to the motor shaft, and NOT to the intermediate drive) for absolute position sensing.

#### 6.3.1 Pot Coupling

The pot shaft is coupled to the capacitor shaft via a flexible coupler (see `shaft_coupler.scad`, `pot_coupler()` module). This coupler:
- Compensates for slight misalignment
- Must be fabricated from insulating material (Delrin or PETG)
- Must NOT be from metal if the capacitor shaft is at RF potential

#### 6.3.2 ADC Reading

ESP32 ADC (12-bit, 0–3.3V):
- Pot wiper voltage: V_wiper = V_supply × (θ / 360°)
- Raw count: 0–4095 (12-bit)
- Calibration maps raw_min (at C_min) and raw_max (at C_max) to angular position

Angle from calibrated pot:

```
θ = (raw − raw_min) / (raw_max − raw_min) × 180°
```

Capacitance from angle:

```
C(θ) = C_min + (C_max − C_min) × (θ / 180°)
```

#### 6.3.3 Dual Position Verification

The firmware maintains two independent position estimates:
1. **Stepper position** — accumulated step count, converted to angle and C
2. **Pot position** — ADC reading, converted to angle and C

If these differ by more than 5°, the firmware issues a warning. This catches:
- Missed steps (motor overloaded or speed too high)
- Belt slip (broken or loose belt)
- Pot coupling failure
- Firmware position corruption after power cycle

Use `CALPOT` command to re-synchronize the step counter to the pot reading.

### 6.4 Limit Switches

Two optical or mechanical limit switches detect the mechanical travel limits:

- **MIN switch** — triggered at full minimum capacitance (0° plate engagement)
- **MAX switch** — triggered at full maximum capacitance (180° plate engagement)

Switch operation:
- During homing: motor drives toward MIN switch at reduced speed (HOMING_SPEED)
- When MIN switch triggers: stop, zero step counter, set state = IDLE
- During normal operation: if MAX switch triggers unexpectedly, set FAULT state

> **CAUTION**
> Do not rely on limit switches as the primary mechanical stop. If a limit switch fails,
> the motor will continue to drive the plates into the mechanical stop, potentially bending
> plates or stripping the drive. Set software position limits in firmware 5–10 steps inside
> the hardware limits.

---

## 7. HIGH-VOLTAGE ISOLATION

### 7.1 The RF Voltage Problem

During transmit, the capacitor plates are at RF potential. In many matching network topologies, neither end of the capacitor is at DC or RF ground. The capacitor shaft, being connected to the rotor plates, is at RF potential.

If the motor shaft (grounded through the driver circuit to the controller PCB) is mechanically coupled to the capacitor shaft without isolation, RF current will flow through the coupling, the motor shaft, the motor windings, and the driver circuit. This can:
- Destroy the DRV8825 stepper driver
- Damage the ESP32
- Create dangerous RF voltage on the controller housing
- Create common-mode RF interference in the control wiring

### 7.2 Insulating Shaft Coupler Requirements

The insulating coupler (see `shaft_coupler.scad`) must provide:

**Minimum electrical isolation:** > 10 MΩ (measured with 500V ohmmeter between motor and cap shaft ends)

**Minimum dielectric withstand:** Must not arc over at operating RF voltage plus safety margin.

Required coupler length:

```
L_min = V_operating_kV / E_material_kV_per_mm
```

For Delrin (POM-C): E = 20 kV/mm

| Voltage Rating | Minimum Length | TM-VCAP-001 Length | Margin |
|---------------|----------------|-------------------|--------|
| 2 kV | 0.1 mm | 50 mm | 500× |
| 5 kV | 0.25 mm | 50 mm | 200× |
| 10 kV | 0.5 mm | 50 mm | 100× |

The 50 mm coupler length provides extreme margin for the bulk dielectric. The limiting factor in practice is **surface creepage path length**, not bulk breakdown.

### 7.3 Surface Creepage

Surface creepage refers to the path along the surface of the insulator. Moisture, salt, and contamination dramatically reduce surface breakdown voltage. The insulating coupler incorporates anti-creepage grooves (ring grooves in the center section) which extend the creepage path length.

Creepage path length for TM-VCAP-001 coupler:
- Straight path: 10 mm (gap between metal hubs)
- With 2 grooves (3 mm deep): 10 + 4 × 3 = 22 mm creepage path

IEC 60664-1 creepage requirements for Pollution Degree 2 (condensation possible), 5 kV:
- Minimum creepage: approximately 14 mm for CTI > 400

The TM-VCAP-001 coupler meets this requirement.

### 7.4 Material Selection

> **WARNING**
> Carbon-filled, graphite-filled, or other conductive filaments must NOT be used for
> the insulating coupler. These materials conduct RF current at high frequencies even
> if they appear resistive at DC. Use only Delrin (POM-C), PETG, or ABS (in order
> of preference). PLA is not acceptable due to hygroscopic properties.

| Material | Dielectric Strength | Moisture Absorption | Recommendation |
|----------|--------------------|--------------------|----------------|
| Delrin POM-C | 20 kV/mm | 0.2% | FIRST CHOICE |
| PETG | 15–20 kV/mm | 0.1% | ACCEPTABLE |
| ABS | 14–16 kV/mm | 0.3% | ACCEPTABLE |
| Nylon PA12 | 12 kV/mm | 0.25% | MARGINAL |
| PLA | 10–12 kV/mm | 0.5–1.5% | NOT ACCEPTABLE |
| Nylon PA6 | 12 kV/mm | 1.5–3.5% | NOT ACCEPTABLE |

### 7.5 Potentiometer Isolation

The potentiometer coupled to the capacitor shaft is also at RF potential. The pot body must be isolated from the chassis, or an optical encoder used instead.

Options in order of preference:

1. **Optical encoder** (best): No electrical path between capacitor shaft and controller PCB. Use optical encoder module with slotted disc on capacitor shaft. Zero RF current path through encoder.

2. **Isolated pot mounting**: Mount pot body on insulating standoffs (Delrin or PTFE), use insulating flexible shaft coupler (see `shaft_coupler.scad`). Install RF bypass capacitor (1 nF, 2 kV rating) on pot wiper wire at the PCB end to decouple RF from ADC input.

3. **RF choke on pot wiring**: Add ferrite bead (Fair-Rite 2661023801) on each pot lead at pot body end. Provides ~500 Ω RF impedance. May not be sufficient for >2 kV applications.

---

## 8. CONSTRUCTION AND ASSEMBLY

### 8.1 Tools and Materials Required

| Item | Specification |
|------|---------------|
| 3D printer | 200 × 200 × 200 mm minimum build volume |
| Filament | PETG (enclosure, brackets); Delrin rod or PETG (insulating coupler) |
| Drill and bits | 3.0, 3.3, 4.3, 5.0, 6.5 mm |
| Tap set | M3, M4 |
| Ohmmeter | Capable of 10 MΩ measurement range |
| Soldering iron | Temperature controlled |
| Thread-locking compound | Loctite 222 (low strength) for set screws |

### 8.2 Printed Parts Inventory

| File | Part | Material | Qty |
|------|------|----------|-----|
| `cap_enclosure.scad` → box_body() | Main enclosure body | PETG | 1 |
| `cap_enclosure.scad` → box_lid() | Enclosure lid | PETG | 1 |
| `cap_mounting_bracket.scad` | Capacitor mounting bracket | PETG | 1 |
| `reduction_drive.scad` → gt2_pulley() | Output drive pulley | PETG | 1 |
| `reduction_drive.scad` → motor_plate() | Motor mounting plate | PETG | 1 |
| `reduction_drive.scad` → housing_cover() | Belt drive cover | PETG | 1 |
| `reduction_drive.scad` → shaft_bearing_block() | Shaft bearing support | PETG | 1 |
| `shaft_coupler.scad` → insulating_coupler() | HV insulating coupler | Delrin or PETG | 1 |
| `shaft_coupler.scad` → pot_coupler() | Pot coupling | PETG | 1 |
| `shaft_coupler.scad` → pot_bracket() | Pot body mount | PETG | 1 |
| `shaft_coupler.scad` → shaft_collar() | Shaft retention collar | PETG | 1 |

### 8.3 Mechanical Assembly Sequence

#### 8.3.1 Capacitor Mounting

**Procedure 8.3.1 — Install Capacitor in Bracket**

1. Insert M4 nylon vibration isolation grommets into base plate counterbores. Press fit from below.
2. Position capacitor frame on side wall ledges of `cap_mounting_bracket`.
3. Verify shaft centerline height matches bracket shaft collar height.
4. Install M4 × 8 screws through bracket side walls into capacitor frame.

   > **CAUTION**
   > If the capacitor frame is at RF potential (floating, not grounded to chassis),
   > use M4 nylon insulating washers and nylon screws at ALL mounting points.
   > Metal fasteners from bracket to frame create a direct RF path to chassis.

5. Check that the capacitor shaft rotates freely through full 180° travel.
6. Install anti-rotation tab against frame side. Tighten M3 set screw finger-tight.
7. Mount bracket to enclosure floor with M4 × 12 screws through grommets.

#### 8.3.2 Drive Assembly

**Procedure 8.3.2 — Install Belt Drive**

1. Press output pulley (200T or 60T GT2, printed) onto capacitor shaft. Align with set screw.
2. Apply Loctite 222 to set screw threads. Tighten M3 set screw firmly (2 N·m).
3. Install shaft bearing block (`shaft_bearing_block`) to enclosure floor with M3 × 8 screws.
   Press-fit 624ZZ bearing (4 mm bore, 13 mm OD) into block.
4. Install motor plate (`motor_plate`) to enclosure. Thread four M3 × 8 screws into tensioner slots. Do not fully tighten yet.
5. Install NEMA 17 motor on motor plate. Fasten with M3 × 8 screws.
6. Install 20T GT2 pulley on motor shaft. Tighten set screw.
7. Install GT2 6 mm belt over both pulleys.
8. Shift motor back in tensioner slots until belt deflection (midspan) = 2–3 mm with 1N finger push.
9. Tighten motor plate screws.
10. Install belt drive housing cover.

#### 8.3.3 Insulating Coupler Installation

**Procedure 8.3.3 — Install HV Insulating Coupler**

> **WARNING**
> The insulating coupler is safety-critical. Verify all steps before applying RF power.

1. File or sand the motor shaft key flat if not already present. Verify the flat engages the coupler set screw alignment.
2. Press the motor shaft into the bottom hub of the insulating coupler. Push in until the shaft end is flush with the internal stop.
3. Tighten the motor-side M3 set screw to 1.5 N·m. Apply Loctite 222.
4. File or verify the capacitor shaft key flat.
5. Insert the capacitor shaft into the top hub of the coupler. Push in until 2 mm of shaft protrudes beyond the coupler body.
6. Tighten the cap-side M3 set screw to 1.5 N·m. Apply Loctite 222.

**Verification — Electrical Isolation Test:**

7. Using an ohmmeter set to the 10 MΩ range: measure resistance from motor shaft (accessible at motor body) to capacitor shaft (accessible beyond coupler).
8. Required reading: > 10 MΩ.
9. If reading is < 10 MΩ: inspect for metal chips, metal filings in coupler bore, metallic contamination of coupler surface, or incorrect material (conductive filament).

   > **WARNING**
   > DO NOT apply RF power if isolation test fails. Identify and correct the cause.

#### 8.3.4 Potentiometer Installation

**Procedure 8.3.4 — Install Position Potentiometer**

1. Mount pot bracket (`pot_bracket`) to capacitor frame rear using M3 × 8 screws through nylon standoffs.
2. Insert pot body through bracket hole. Secure with panel nut (wrench to 1 N·m, do not overtighten).
3. Slide pot coupler (`pot_coupler`) onto capacitor shaft. Push to within 5 mm of pot bracket face.
4. Align pot shaft with coupler top bore. Slide coupler onto pot shaft.
5. Tighten both set screws.
6. Rotate capacitor shaft through full travel. Verify pot shaft rotates freely without binding.
7. If the capacitor shaft is at RF potential, verify pot body is mounted with nylon standoffs and that no metal contact exists between pot body and chassis or bracket.

#### 8.3.5 Controller PCB and Wiring

**Procedure 8.3.5 — Install Controller PCB**

1. Install M3 × 5 mm brass heat-set inserts in PCB standoffs (if threaded inserts used) OR install M3 × 6 press-fit inserts.
2. Place controller PCB on standoffs. Secure with M3 × 6 screws.
3. Connect stepper motor cable (4-conductor) to DRV8825 output terminals.
4. Connect MIN limit switch to PIN_LIM_MIN and GND. (Normally open, closes at minimum position.)
5. Connect MAX limit switch to PIN_LIM_MAX and GND.
6. Connect pot wiper to PIN_POT_WIPER (ADC input). Connect pot ends to 3.3V and GND.
7. Install 1 nF 2 kV capacitor from pot wiper wire to GND, as close to PCB as possible.
8. Connect OLED SDA/SCL to PIN_I2C_SDA/SCL.
9. Connect LED to PIN_STATUS_LED with 330 Ω series resistor.
10. Connect 12V DC power to PCB power input.

#### 8.3.6 RF Connector Installation

1. Install SO-239 or N-type connectors in enclosure wall holes (left wall, per `cap_enclosure.scad`).
2. Use silver-plated solder lugs or copper strap for RF connections — avoid long wire runs.
3. Maximum lead length from RF connector to capacitor terminal: 50 mm.
4. Use RG-316 or silver-plated coax for leads inside enclosure.

### 8.4 Weatherproofing

**Procedure 8.4 — Weatherproof the Enclosure**

1. Measure the gasket groove path length around the lid mating surface.
2. Cut 3 mm EPDM O-cord (McMaster 5233K72 or equivalent) to this length plus 2%.
3. Join cord ends with cyanoacrylate (Loctite 495 or equivalent). Allow 5 minutes cure.
4. Press cord into gasket groove. It should sit slightly proud of the groove.
5. Install PG9 cable gland (rear wall) for motor/pot cable bundle. Torque to 5 N·m.
6. Install sealed barrel jack (DC input, rear wall).
7. Install Teflon seal rings on RF connectors.
8. Place 5 g silica gel packet inside enclosure before closing lid.
9. Apply RTV silicone (Permatex 81730 or equivalent) to all interior wire feedthroughs through glands.
10. Install lid with 6× M4 × 12 stainless screws. Torque evenly in crossing pattern to 2.5 N·m.

---

## 9. CONTROLLER FIRMWARE ARCHITECTURE

### 9.1 File Structure

```
esp32_cap_tuner/
├── config.h          — Hardware constants, pin definitions, capacitor parameters
├── position.h        — PotSensor, CapCalc, CapPositionManager classes
├── tuning.h          — Band presets, CapAutoTuner, CapCommandParser
└── esp32_cap_tuner.ino — Main setup() and loop(), BT, OLED, LED, buttons
```

### 9.2 config.h — Key Constants

```cpp
// Capacitor parameters
#define CAP_C_MIN_PF        10.0f     // Minimum capacitance (full mesh out)
#define CAP_C_MAX_PF        250.0f    // Maximum capacitance (full mesh in)
#define CAP_ROTATION_DEG    180.0f    // Total shaft rotation

// Motor and drive
#define STEPS_PER_REV       200       // NEMA 17 steps/revolution
#define MICROSTEP_DIV       16        // DRV8825 1/16 microstepping
#define GEAR_RATIO          10        // Drive reduction ratio

// Derived
#define DEG_PER_STEP        (360.0f / (STEPS_PER_REV * MICROSTEP_DIV * GEAR_RATIO))
// = 360 / 32000 = 0.01125°/step
#define STEPS_FULL_ROT      ((long)(CAP_ROTATION_DEG / DEG_PER_STEP))
// = 180 / 0.01125 = 16000 steps

// Plate geometry (for calculated C_max verification)
#define CAP_PLATE_N         41
#define CAP_PLATE_DIA_MM    50.0f
#define CAP_PLATE_GAP_MM    2.0f
#define CAP_VOLTAGE_KV      5.0f      // Operating voltage rating
```

### 9.3 CapCalc Class (position.h)

```cpp
class CapCalc {
public:
    // Convert shaft angle to capacitance (linear interpolation)
    static float angleToC(float deg, float c_min, float c_max, float rot_deg) {
        return c_min + (c_max - c_min) * (deg / rot_deg);
    }

    // Convert capacitance to shaft angle
    static float cToAngle(float c_pf, float c_min, float c_max, float rot_deg) {
        return (c_pf - c_min) / (c_max - c_min) * rot_deg;
    }

    // Convert capacitance to motor steps
    static long cToSteps(float c_pf, float c_min, float c_max, float rot_deg) {
        return (long)(cToAngle(c_pf, c_min, c_max, rot_deg) / DEG_PER_STEP);
    }

    // Capacitive reactance in ohms
    static float reactanceOhm(float freq_hz, float c_pf) {
        return 1.0f / (2.0f * M_PI * freq_hz * c_pf * 1e-12f);
    }

    // Parallel plate theoretical C from geometry
    static float calcPlateCapPF(float dia_mm, int n_plates, float gap_mm) {
        float eps0 = 8.854e-12f;
        float r = (dia_mm / 2.0f) / 1000.0f;
        float area = M_PI * r * r;
        float gap_m = gap_mm / 1000.0f;
        return eps0 * area * (n_plates - 1) / gap_m * 1e12f;
    }

    // Voltage rating from gap
    static float voltageRatingKV(float gap_mm) {
        return gap_mm * 2.0f;  // 2 kV/mm operating (50% of 3 kV/mm breakdown)
    }

    // Peak RF voltage at given power and reactance
    static float peakVoltsRF(float power_w, float xc_ohm) {
        return sqrtf(2.0f * power_w * xc_ohm);
    }
};
```

### 9.4 Calibration Workflow

The potentiometer calibration sequence is mandatory before the capacitor can be used:

```
Step 1: Send CALMIN
        → Manually set capacitor to minimum C (full unmesh)
        → Press Enter to confirm
        → Firmware records pot raw_min and zeros step counter

Step 2: Send CALMAX
        → Manually set capacitor to maximum C (full mesh)
        → Press Enter to confirm
        → Firmware records pot raw_max

Step 3: Send CALPOT
        → Firmware reads current pot position
        → Synchronizes step counter to pot-derived angle
        → Sets homed = true
```

After calibration, `STATUS` will show both pot and stepper positions in agreement.

### 9.5 Command Reference

| Command | Arguments | Function |
|---------|-----------|----------|
| HOME | — | Move to minimum C (homing sequence) |
| CALMIN | — | Record pot value at minimum C |
| CALMAX | — | Record pot value at maximum C |
| CALPOT | — | Sync stepper to pot (re-calibrate after slip) |
| SETC | pF | Move to specified capacitance |
| GOTO | steps | Move to absolute step position |
| GOSTEP | ±steps | Move relative number of steps |
| BAND | name | Move to band preset (e.g., BAND 20m) |
| FREQ | MHz | Calculate and move to series-resonant C for frequency |
| SWEEP | — | Sweep from min to max C, report |
| TABLE | — | Print C vs. angle table |
| VTABLE | — | Print peak voltage vs. band table |
| STATUS | — | Full JSON status dump |
| INFO | — | Firmware version, hardware constants |
| STOP | — | Halt motor immediately |
| RESET | — | Restart firmware |

### 9.6 State Machine

```
    ┌─────────────────────────────────────────────────────┐
    │                                                     │
    ▼                                                     │
  UNCAL ──[CALMIN+CALMAX]──► IDLE ──[HOME cmd]──► HOMING ─┤
    ▲                          │                          │
    │                    [SETC/BAND/GOTO]                 │
    │                          │                          │
    │                       MOVING ──[done]──► IDLE       │
    │                          │                          │
    │                     [limit fail]                    │
    │                          │                          │
    └──────────────────── FAULT ◄─────────────────────────┘
```

States 0–4: IDLE, MOVING, HOMING, FAULT, UNCAL

---

## 10. CYD TOUCHSCREEN DISPLAY

### 10.1 Hardware

The CYD (ESP32-2432S028) display connects to the controller via Bluetooth SPP. The CYD acts as the Bluetooth master and initiates connection to "RF-CapTuner".

| CYD Component | Function |
|---------------|----------|
| ILI9341 320×240 TFT | Main display |
| XPT2046 touch controller | Touch input |
| ESP32 Bluetooth | BT SPP master |
| RGB LED | Status indicator |

### 10.2 Display Modes

**LIVE mode** — Normal operating display:
- Large capacitance readout (3× text, color-coded by C range)
- Arc indicator showing rotor rotation position (0–180°)
- Pot vs. stepper comparison (two data lines)
- Sparkline — 60-point C history (last 30 seconds)
- Reactance and estimated peak voltage line (changes color at warning thresholds)

**BAND mode** — Band preset grid:
- 10-band grid (160m–10m)
- Tap band to highlight, tap LOAD to send BAND command to controller
- Shows typical series capacitance for each band

**SWEEP mode** — Capacitance sweep chart:
- XY plot: time (X) vs. capacitance (Y)
- Collects up to 200 points during a SWEEP command
- Color-codes plot points by C value (blue → cyan → green → yellow → red)

**INFO mode** — Technical data:
- Current C, angle, pot reading, position error, step count
- Voltage ratings table: X_C and V_peak at 100W for all HF bands, color-coded by safety margin
- Firmware version

### 10.3 Voltage Warning Color Coding

The CYD display uses three colors for voltage safety indication:

| Color | Condition |
|-------|-----------|
| Green | V_peak < 50% of voltage rating |
| Orange | V_peak 50–80% of voltage rating — caution |
| Red | V_peak > 80% of voltage rating — WARNING |

This warning is shown in LIVE mode (bottom line) and INFO mode (table).

> **NOTE**
> The voltage estimate in the display uses 100W as a reference power. For actual power levels,
> the estimate scales as √P. At 1 kW, multiply displayed V_peak by √10 ≈ 3.16.

### 10.4 Touch Zones

| Zone | Area | Function |
|------|------|----------|
| Tab bar | Bottom 30 px strip, full width | Switch display mode |
| Button bar | Bottom 30 px (very bottom) | Mode-dependent commands |
| Band grid | BAND mode main area | Select band preset |

Button bar assignments:

| Button | LIVE | BAND | SWEEP | INFO |
|--------|------|------|-------|------|
| 0 (leftmost) | HOME | HOME | HOME | HOME |
| 1 | C- (fine) | ◄◄ | SWEEP START | RESET |
| 2 | C+ (fine) | ►► | SWEEP STOP | STATUS |
| 3 | STOP | LOAD | CLEAR | CALPOT |
| 4 (rightmost) | CAL | BACK | BACK | BACK |

---

## 11. CALIBRATION PROCEDURES

### 11.1 Initial Calibration

**Procedure 11.1 — Initial Potentiometer Calibration**

This procedure must be performed before first use and after any mechanical changes to the drive system.

Required equipment:
- PC with serial terminal (115200 baud) or BT SPP terminal
- Ohmmeter (optional, for isolation verification)

**Step 1** — Verify isolation (first time only):
1. With power OFF, measure resistance between motor shaft and capacitor shaft.
2. Verify > 10 MΩ. If not, see Section 7.4.

**Step 2** — Power on:
1. Apply 12V DC to controller.
2. Connect to controller via serial or BT.
3. Observe startup message: `RF-CapTuner fw=X.X.X`
4. Observe prompt: `>>> Send CALMIN (at min C) then CALMAX (at max C) to calibrate pot <<<`

**Step 3** — Calibrate minimum position:
1. Manually rotate the capacitor shaft to minimum capacitance (plates fully unmeshed, rotor perpendicular to stator).
2. Send: `CALMIN`
3. Observe response: `CAL: min set, raw=XXXX`

**Step 4** — Calibrate maximum position:
1. Manually rotate the capacitor shaft to maximum capacitance (plates fully meshed).
2. Send: `CALMAX`
3. Observe response: `CAL: max set, raw=XXXX  C range: 10.0-250.0 pF  rotation: 180 deg`

**Step 5** — Sync stepper to pot:
1. Send: `CALPOT`
2. Observe response: `CALPOT: steps synced to pot angle XX.X deg`
3. Observe: state changes from UNCAL to IDLE

**Step 6** — Verify:
1. Send: `STATUS`
2. Verify pot_c and c_pf agree within 5 pF.
3. If they disagree significantly, repeat from Step 2.

**Step 7** — Test homing:
1. Send: `HOME`
2. Observe motor movement, state = HOMING.
3. Observe stop when MIN limit switch triggers or minimum steps reached.
4. Observe state = IDLE.
5. Send: `STATUS` — verify c_pf ≈ CAP_C_MIN_PF.

### 11.2 Periodic Verification

Verify calibration annually or after any of the following events:
- Mechanical disassembly and reassembly
- Belt replacement or retensioning
- Power failure during movement
- FAULT state occurrence

**Procedure 11.2 — Calibration Verification**

1. Send: `HOME` — move to minimum position.
2. Send: `STATUS` — record c_pf and pot_c. Verify both ≈ C_min (±5%).
3. Send: `SETC 125` — move to approximately midrange.
4. Send: `STATUS` — verify c_pf ≈ 125 pF (±10 pF), pot_c agrees within 5 pF.
5. Send: `SETC 250` — move to maximum.
6. Send: `STATUS` — verify c_pf ≈ C_max (±10 pF).
7. If any step fails, perform full calibration (Procedure 11.1).

### 11.3 Wheeler Formula Verification (Calculated vs. Measured)

The firmware reports calculated C_max from plate geometry at startup. Verify against measured value:

1. Connect NanoVNA or impedance analyzer to capacitor terminals.
2. Set capacitor to full mesh (maximum).
3. Measure capacitance at 1 MHz.
4. Compare measured to firmware-calculated value (startup message).
5. If measured value differs by > 20%, update `CAP_C_MAX_PF` in config.h.

> **NOTE**
> Measured capacitance at full mesh typically reads 70–85% of the theoretical plate calculation
> due to edge effects and the end gap (space at each end of the plate stack).
> The `CAP_C_MAX_PF` value should be set to the measured value, not the calculated plate value.

---

## 12. OPERATING PROCEDURES

### 12.1 Normal Operating Procedure

**Procedure 12.1 — Normal Tuning Operation**

Prerequisites:
- Calibration complete (Procedure 11.1 or 11.2)
- Transmitter connected, power OFF
- Dummy load or known antenna connected

1. Power on controller.
2. Send: `HOME` or press HOME button — wait for IDLE state.
3. Select band using BAND command or CYD BAND mode.
4. Enable low-power transmit (10W).
5. Observe SWR (via external meter or power meter system).
6. Trim capacitance using `SETC` command or CYD C+/C- buttons until SWR is minimized.
7. Save position: `SAVE bandname`
8. Increase to full power and verify SWR.

> **CAUTION**
> Do not adjust the capacitor while transmitting at high power. Although the motor drive
> is isolated from the RF circuit, motor movement during high-power TX may cause arcing
> at the wiper contact. Always reduce to 10W or less before adjusting.

### 12.2 Voltage Safety Check Procedure

**Procedure 12.2 — Pre-Transmit Voltage Safety Verification**

Before transmitting at power levels > 100W, or on 160m/80m at any power level:

1. Send: `VTABLE`
2. Read the voltage table output.
3. For the operating band and power level, calculate expected V_peak:
   ```
   V_peak(actual) = V_peak(100W from table) × √(P_actual / 100)
   ```
4. Verify V_peak(actual) < 80% of CAP_VOLTAGE_KV × 1000.

**Example:** Operating on 40m at 500W, C = 80 pF.
- VTABLE shows V_peak at 100W = 316 V for 80 pF at 7.1 MHz.
- At 500W: V_peak = 316 × √(500/100) = 316 × 2.24 = 708 V.
- Cap rating = 5 kV, 80% = 4,000 V.
- 708 V << 4,000 V → SAFE.

### 12.3 Emergency Stop

If arcing is heard (crackling or buzzing in received audio, visible spark inside enclosure):

1. Immediately reduce transmit power to zero.
2. Send: `STOP` command or press STOP button on CYD.
3. Do not transmit again until the cause is identified.
4. Inspect capacitor for carbon tracks, burned plates, or plate contact.

---

## 13. MAINTENANCE

### 13.1 Scheduled Maintenance

| Interval | Task |
|----------|------|
| Monthly | Inspect wiper contact, clean if tarnished (Procedure 13.1) |
| Quarterly | Check belt tension, adjust if needed (Procedure 13.2) |
| Annually | Replace desiccant pack, inspect O-cord, verify isolation (Procedure 13.3) |
| As needed | Clean and lubricate shaft bearings (Procedure 13.4) |

### 13.2 Wiper Contact Cleaning

**Procedure 13.1 — Clean Wiper Contact**

1. Remove RF power. De-energize controller.
2. Remove enclosure lid.
3. Apply 2 drops Caig DeoxIT Gold G5 to wiper contact area.
4. Rotate capacitor shaft through full travel 10 times.
5. Remove excess contact cleaner with cotton swab.
6. Allow 15 minutes to dry.
7. Reinstall lid.

### 13.3 Belt Tension Check

**Procedure 13.2 — Belt Tension Adjustment**

1. Remove enclosure lid.
2. With motor de-energized, press belt midspan with 1 N finger force.
3. Belt should deflect 2–3 mm. If deflection > 5 mm: belt is loose.
4. Loosen four motor plate screws.
5. Push motor away from output pulley until correct tension is achieved.
6. Retighten motor plate screws.
7. Check that belt does not rub on housing cover.

### 13.4 Annual Maintenance

**Procedure 13.3 — Annual Overhaul**

1. Remove lid. Remove and weigh old desiccant pack.
2. If pack weighs > 110% of original weight: discard, install new 5 g silica gel pack.
3. Inspect O-cord gasket for cracks, compression set, or hardening. Replace if cord no longer round in cross-section.
4. Check all M4 lid screws for looseness. Retorque to 2.5 N·m.
5. Re-verify electrical isolation: motor shaft to capacitor shaft > 10 MΩ.
6. Verify calibration (Procedure 11.2).
7. Inspect all RF connections for corrosion. Recoat with NoOx-ID A Special grease.

### 13.5 Bearing Lubrication

**Procedure 13.4 — Shaft Bearing Lubrication**

Interval: every 2 years, or if motor audibly strains.

1. Access the capacitor shaft bearing (main rotor bearing of the air variable).
2. Apply 1 drop synthetic bearing oil (Nye Lubricants Fluorocarbon FG series or equivalent RF-safe oil) to bearing races.
3. Apply 1 drop to pot shaft bearing.
4. Work oil in by rotating shaft through full travel.
5. Wipe excess.

> **CAUTION**
> Do not use petroleum-based lubricants, WD-40, or petroleum grease near the capacitor
> plates. Contamination of the air gap dielectric will reduce voltage rating and cause
> premature arcing. Use only fluorocarbon or synthetic ester lubricants.

---

## 14. TROUBLESHOOTING

### 14.1 Diagnostic Flowchart

```
Problem reported
     │
     ├─ No movement ──────────────────────────────────────────────┐
     │                                                            │
     ├─ Incorrect position (pos_err > 5°) ────────────────────────┤
     │                                                            │
     ├─ FAULT state ───────────────────────────────────────────────┤
     │                                                            │
     ├─ Arcing / RF burn ──────────────────────────────────────────┤
     │                                                            │
     ├─ No BT connection ─────────────────────────────────────────┤
     │                                                            │
     └─ UNCAL state ───────────────────────────────────────────────┘
```

### 14.2 No Motor Movement

| Symptom | Probable Cause | Corrective Action |
|---------|---------------|-------------------|
| No movement, no error | FAULT state, cleared but not homed | Send HOME |
| No movement, FAULT | Limit switch stuck closed | Inspect limit switch wiring |
| Motor moves but cap doesn't | Drive coupler set screw loose | Retighten, apply Loctite |
| Motor hums but doesn't turn | Motor current too low (DRV8825 Vref) | Adjust Vref: 0.5–0.8V |
| Motor stutters | Microstepping set incorrect | Verify MS1/MS2/MS3 → 1/1/1 for 1/16 |

### 14.3 Position Error (pos_err > 5°)

| Symptom | Probable Cause | Corrective Action |
|---------|---------------|-------------------|
| pos_err increases after each move | Missed steps — speed too high | Reduce MAX_SPEED in config.h |
| pos_err appears at power-on | Homing needed, pot offset | Send HOME, then CALPOT |
| pos_err large, pot_c reads 0 or max | Pot wiper disconnected or pot damaged | Check pot wiring; replace pot |
| pos_err random, no pattern | Electrical noise on pot signal | Add 10 nF cap from pot wiper to GND at PCB |
| pos_err consistent, same direction | Calibration error | Re-run CALMIN / CALMAX |

### 14.4 FAULT State

The controller enters FAULT state when:
- MAX limit switch triggered during movement
- Step counter exceeds software limits
- Pot reading goes out of range (pot disconnected or failed)

**Procedure 14.1 — Clear FAULT State**

1. Send: `STATUS` — read fault cause from output.
2. Identify and correct physical cause (limit switch, mechanical obstruction).
3. Send: `RESET` — firmware restart.
4. Send: `HOME` — re-home to known position.
5. If FAULT immediately recurs, send: `TABLE` — verify step counts correspond to expected C range.

### 14.5 Arcing Inside Enclosure

> **WARNING**
> Do not attempt to clear arcing by increasing transmitter drive. Reduce power immediately.

| Symptom | Probable Cause | Corrective Action |
|---------|---------------|-------------------|
| Continuous arc, burning smell | Plates shorted (mechanical contact) | Inspect plates, re-align |
| Intermittent crackle | Contamination on plates (dust, oil film) | Clean plates with IPA, dry thoroughly |
| Arc at min C only | Plate end gap too small | Adjust end-stop to prevent full unmesh |
| Arc at max C only | Plate gap too small, plates bent | Inspect plate spacing, re-space if needed |
| Arc on wiper contact | Insufficient contact pressure | Adjust wiper spring preload |

### 14.6 No BT Connection (CYD)

1. Verify controller is powered and advertising ("BT: advertising as 'RF-CapTuner'" in serial).
2. Verify CYD is within 10 m of controller.
3. On CYD: if "Connecting..." persists > 30 s, cycle CYD power.
4. Check that only one BT client is connected (two simultaneous BT SPP clients not supported).
5. If controller serial terminal is connected via BT, disconnect before CYD attempts connection.

### 14.7 UNCAL State After Power Cycle

The controller saves calibration data (raw_min, raw_max) in NVS (non-volatile storage). After power cycle, these values are restored.

If UNCAL appears after power cycle:
1. Send: `STATUS` — check if `pot.calibrated = false`.
2. Check NVS namespace in config.h (`NVS_NAMESPACE = "cap_tuner"`).
3. If NVS is erased: re-run full calibration (Procedure 11.1).
4. If problem recurs after each power cycle: NVS partition may be corrupted — reflash firmware with `nvs_erase` sketch first.

---

## APPENDIX A: SPECIFICATIONS

### A.1 Electrical Specifications

| Parameter | Value | Units |
|-----------|-------|-------|
| Capacitance range | 10–250 (standard) or 5–500 (large) | pF |
| Capacitance accuracy | ±5% of set value (post-calibration) | — |
| Capacitance resolution | 0.015 pF/step | pF |
| Frequency range | 1.8–30 | MHz |
| Operating voltage (standard) | 5 kV peak | V |
| Operating voltage (HV option) | 10 kV peak | V |
| Continuous current | 10 A RMS | A |
| Peak current | 25 A | A |
| RF connectors | 2× SO-239 or N-type | — |
| Motor supply | 12 V DC, 1.5 A max | V/A |
| Control supply (ESP32) | 3.3 V regulated from 12V | V |

### A.2 Mechanical Specifications

| Parameter | Value | Units |
|-----------|-------|-------|
| Capacitor shaft diameter | 6.35 (1/4 inch standard) | mm |
| Drive reduction ratio | 10:1 | — |
| Steps per degree (cap shaft) | 88.9 | steps/° |
| Angular resolution | 0.01125 | °/step |
| Travel | 180 | degrees |
| Homing speed | 100 (steps/sec at motor) | — |
| Maximum slew speed | 2000 (steps/sec at motor) | — |
| Enclosure dimensions | 195 × 135 × 88 + 12 (lid) | mm |
| Enclosure rating | IP54 (with EPDM gasket) | — |
| Print material (enclosure) | PETG | — |
| Print material (coupler) | Delrin (POM-C) or PETG | — |
| Insulating coupler length | 50 | mm |
| Insulating coupler isolation | > 10 MΩ @ 500 V DC | — |

### A.3 Controller Specifications

| Parameter | Value |
|-----------|-------|
| Controller | ESP32-WROOM-32 |
| Stepper driver | DRV8825, 1/16 microstepping |
| Position sensor | 10 kΩ rotary pot, 12-bit ADC |
| Limit switches | 2× optical or micro-switch |
| Display | SSD1306 OLED 128×64 (local) |
| Remote display | CYD ESP32-2432S028 (Bluetooth) |
| Communication | Bluetooth SPP, USB serial |
| Data persistence | ESP32 NVS (non-volatile) |
| Firmware | TM-VCAP-ESP32-001 Rev A |

---

## APPENDIX B: BILL OF MATERIALS

### B.1 Electronics

| Item | Part Number / Description | Qty | Source |
|------|---------------------------|-----|--------|
| ESP32 | ESP32-WROOM-32 DevKit | 1 | AliExpress, Mouser |
| Stepper driver | DRV8825 module | 1 | AliExpress |
| Stepper motor | NEMA 17, 1.7A, 4000 g·cm | 1 | StepperOnline 17HS19-2004S1 |
| OLED | SSD1306 128×64 I2C | 1 | AliExpress |
| CYD display | ESP32-2432S028 | 1 (optional) | AliExpress |
| Rotary pot | 10 kΩ panel pot, 6 mm shaft | 1 | Bourns PTV09A-4030F-B103 |
| Limit switches | SPST NO micro-switch | 2 | Omron SS-5GL |
| Capacitors | 1 nF 2 kV (pot bypass) | 2 | Vishay BC Components |
| Status LED | 3 mm green LED | 1 | any |
| LED resistor | 330 Ω 1/4W | 1 | any |
| Decoupling caps | 100 nF MLCC, 10 µF electrolytic | several | any |
| PCB | Fabricate from KiCad files | 1 | JLCPCB, PCBWay |
| DC barrel jack | 5.5/2.1 mm, sealed | 1 | Amphenol PPPK-02P |
| Cable gland | PG9 nylon, EPDM insert | 2 | McMaster 69915K310 |

### B.2 Mechanical

| Item | Specification | Qty | Source |
|------|---------------|-----|--------|
| Variable capacitor | Surplus air variable, 250 pF, 5 kV | 1 | eBay / surplus |
| GT2 belt | 6 mm × 500 mm loop | 1 | AliExpress |
| GT2 pulley (motor) | 20T, 5 mm bore | 1 | AliExpress |
| GT2 pulley (output) | 200T or 60T, 6.35 mm bore | 1 | Print or AliExpress |
| 624ZZ bearing | 4 mm ID, 13 mm OD, 5 mm | 2 | VXB or AliExpress |
| EPDM O-cord | 3 mm diameter | 1 m | McMaster 5233K72 |
| Silica gel | 5 g packet | 1 | any |
| Insulating coupler | Print in Delrin/PETG | 1 | Self-fabricated |
| M3 screws | Stainless, 6–12 mm | assorted | any |
| M4 screws | Stainless, 8–16 mm | assorted | any |
| M4 nylon grommets | McMaster 90131A151 | 4 | McMaster |
| M3 set screws | M3 × 5 mm socket | 6 | any |
| RF connectors | SO-239 panel mount | 2 | Amphenol 83-1R |

---

## APPENDIX C: CAPACITOR DESIGN WORKSHEET

### C.1 Plate Geometry Calculator

```
Input parameters:
  Plate diameter:    D = _____ mm
  Plate gap:         d = _____ mm
  Number of plates:  N = _____

Derived:
  Plate area:      A = π × (D/2000)²  = _____ m²
  Plate gaps:      N-1               = _____
  C_max (calc):    ε₀ × A × (N-1) / (d/1000) × 1e12  = _____ pF

  Voltage rating:  V = d × 2 kV/mm = _____ kV

  Current rating: (estimate)
    At P = 1000 W, f = 7 MHz, C = 10 pF:
    X_C = 1 / (2π × 7e6 × 10e-12) = 2274 Ω
    V_peak = √(2 × 1000 × 2274) = 2132 V
    I_peak = V_peak / X_C = 0.94 A
    I_rms  = I_peak / √2 = 0.66 A
```

### C.2 Angular Resolution Calculator

```
Input parameters:
  Steps per revolution:   STEPS_PER_REV = 200
  Microstepping divisor:  MICROSTEP_DIV = 16
  Gear reduction ratio:   GEAR_RATIO    = 10

Derived:
  Steps per revolution at output: 200 × 16 × 10 = 32,000
  Degrees per step:               360 / 32,000 = 0.01125°/step
  Steps for 180°:                 16,000 steps
  ΔC per step (250 pF range):     240 / 16,000 = 0.015 pF/step
```

---

## APPENDIX D: BAND CAPACITANCE REFERENCE

### D.1 Typical Tuner Capacitance Values

The following capacitance values are starting points. Exact values depend on antenna impedance and tuner topology. Use `BAND` command to load presets; trim with `SETC` or C+/C- for minimum SWR.

| Band | Freq (MHz) | C_series typical (pF) | C_shunt typical (pF) | Voltage risk |
|------|-----------|----------------------|---------------------|-------------|
| 160m | 1.8–2.0 | 150–250 | 200–400 | HIGH (high Xc at low C) |
| 80m | 3.5–4.0 | 100–200 | 150–300 | MODERATE |
| 60m | 5.3–5.4 | 80–150 | 100–200 | MODERATE |
| 40m | 7.0–7.3 | 60–120 | 80–150 | LOW-MODERATE |
| 30m | 10.1–10.15 | 45–90 | 60–120 | LOW |
| 20m | 14.0–14.35 | 35–75 | 45–100 | LOW |
| 17m | 18.068–18.168 | 28–60 | 35–80 | LOW |
| 15m | 21.0–21.45 | 22–50 | 28–65 | LOW |
| 12m | 24.89–24.99 | 18–40 | 22–55 | LOW |
| 10m | 28.0–29.7 | 15–35 | 18–45 | LOW |

### D.2 Reactance at Band Edges (pF reference values)

X_C in ohms for three representative capacitances:

| Band | Freq | 20 pF | 80 pF | 200 pF |
|------|------|-------|-------|--------|
| 160m | 1.9 MHz | 4,188 | 1,047 | 419 |
| 80m | 3.6 MHz | 2,210 | 553 | 221 |
| 40m | 7.1 MHz | 1,121 | 280 | 112 |
| 30m | 10.1 MHz | 788 | 197 | 79 |
| 20m | 14.2 MHz | 561 | 140 | 56 |
| 17m | 18.1 MHz | 440 | 110 | 44 |
| 15m | 21.2 MHz | 375 | 94 | 38 |
| 12m | 24.9 MHz | 320 | 80 | 32 |
| 10m | 28.5 MHz | 280 | 70 | 28 |

---

## APPENDIX E: VHF AND UHF CAPACITOR DESIGNS

### E.1 Overview

The TM-VCAP system is extended by two additional variable capacitor designs optimized for VHF (50–225 MHz) and UHF (430–1296 MHz) operation. These designs differ fundamentally from HF capacitors in four respects:

1. **Smaller plate area** — lower capacitance range (2–47 pF and 0.5–12 pF respectively)
2. **Critical lead length** — stray inductance from wiring dominates above 100 MHz
3. **Self-resonant frequency** — SRF must be well above the operating frequency
4. **Mechanical accuracy** — plate tolerances of ±0.1 mm critical at UHF

### E.2 VC-VHF Design Parameters

| Parameter | Value |
|-----------|-------|
| Capacitance range | 2–47 pF |
| Plate radius | 15 mm |
| Plate spacing | 1.0 mm |
| Number of rotor plates | 8 |
| Number of stator plates | 9 |
| Total capacitive gaps | 15 |
| Working voltage | 500 V peak |
| Breakdown voltage | 3.0 kV (6× safety) |
| Q at 146 MHz | ≈400 (brass plates, short leads) |
| Self-resonant frequency | ≈519 MHz |
| Frame dimensions | 40×40×35 mm (PETG) |
| Shaft diameter | 4 mm brass |

**Construction notes:**
- Use PTFE stator rod bushings; no PETG in the RF path
- Minimize all wiring: semi-rigid coax stubs directly soldered to terminal pads
- SRF at 519 MHz leaves only 3.5× margin at 146 MHz — adequate if leads < 5 mm
- Mount on PCB with minimal trace inductance; avoid through-hole to top-side paths

### E.3 VC-UHF Design Parameters

| Parameter | Value |
|-----------|-------|
| Capacitance range | 0.5–12 pF |
| Plate radius | 8 mm |
| Plate spacing | 0.5 mm |
| Number of rotor plates | 4 |
| Number of stator plates | 5 |
| Total capacitive gaps | 7 |
| Working voltage | 200 V peak |
| Breakdown voltage | 1.5 kV (7.5× safety) |
| Q at 432 MHz | ≈200 (brass, PCB mount) |
| Self-resonant frequency | ≈1453 MHz |
| Frame dimensions | 25×25×20 mm (PETG) |
| Shaft diameter | 3 mm brass |

> **CRITICAL — UHF LEAD LENGTH**
> The SRF of the VC-UHF is 1453 MHz, only 12% above 1296 MHz (23cm band).
> Any parasitic inductance from leads lowers the effective SRF further.
> Lead inductance: approximately 1 nH/mm.
> At 1296 MHz, X_L of 3 mm lead = 2π × 1296 MHz × 3 nH = 24 Ω.
> This is comparable to X_C of the capacitor and will detune the matching network.
> **Stator-to-circuit connections must be ≤ 3 mm total trace length.**
> Direct PCB soldering is mandatory. No clip-lead, turret, or through-hole mounting.

### E.4 VHF/UHF Impedance Matching Theory

At VHF and UHF frequencies, the variable capacitor is used in:

**Stub matching networks** — the capacitor tunes the susceptance of a short-circuited or open-circuited stub:

```
B_stub(f, l) = (1/Z₀) × tan(2πfl/v_p)  [siemens, short-circuit stub]
```

Tune capacitor in parallel to cancel the reactive part of the antenna impedance:

```
B_cap = 2πfC = -Im(Y_ant)  ← condition for resonance
C_required = -Im(Y_ant) / (2πf)
```

**L-network at VHF (series C, shunt L):**

```
Q_net = √(R_high/R_low − 1)
X_C_series = R_high / Q_net
X_L_shunt  = R_low × Q_net
C = 1 / (2π × f × X_C_series)
```

Example: Match 200 Ω to 50 Ω at 146 MHz:
```
Q = √(200/50 − 1) = √3 = 1.73
X_C = 200/1.73 = 115.6 Ω → C = 1/(2π×146e6×115.6) = 9.4 pF (VC-VHF range)
```

### E.5 Calibration at VHF/UHF

Standard pot-based position calibration applies to VC-VHF and VC-UHF (see Section 11). However, due to the small capacitance values, NanoVNA verification is especially important:

**VHF calibration (NanoVNA):**
1. Connect NanoVNA Port 1 to capacitor; terminate Port 2 in 50 Ω
2. Set span: 50–600 MHz
3. Note the null in S11 at each calibration angle — corresponds to capacitor in parallel resonance with a known inductance

**UHF calibration:**
- Use NanoVNA in S11 mode, 800 MHz–2 GHz sweep
- At each capacitor position, the series resonance with a 5 nH reference inductor gives C = 1/(4π²f_res²×L)
- Measure at 5 angles and fit a line: C = C_min + (C_max − C_min)×θ/180°
- Enter calibrated C_max via `SETC` command: `SETC CALMAX <measured_pF>`

### E.6 Band Preset Table — VHF/UHF

| Band | Freq (MHz) | Cap Design | C_nominal (pF) | C_search range (pF) | V_rating |
|------|-----------|-----------|----------------|---------------------|----------|
| 6m | 51.0 | VC-VHF | 18.0 | 8–35 | 500 V pk |
| 2m | 146.0 | VC-VHF | 8.5 | 3–20 | 500 V pk |
| 1.25m | 222.0 | VC-VHF | 5.5 | 2–12 | 500 V pk |
| 70cm | 446.0 | VC-UHF | 4.0 | 1–10 | 200 V pk |
| 33cm | 906.0 | VC-UHF | 2.5 | 0.5–6 | 200 V pk |
| 20cm | 1296.0 | VC-UHF | 1.5 | 0.5–4 | 200 V pk |

### E.7 Summary — Five-Design Comparison

| Design | C range | V_work | Power | Primary use | SRF | Frame |
|--------|---------|--------|-------|-------------|-----|-------|
| VC-QRP | 8–345 pF | 200 V | 10 W | QRP tuner, receiver | 156 MHz | PETG 50mm |
| VC-HF100 | 10–225 pF | 1500 V | 100 W | HF portable tuner | 150 MHz | PETG/Delrin 80mm |
| VC-TX | 5–84 pF | 4000 V | 500 W | HF pi-net tank | 173 MHz | Aluminum 120mm |
| VC-VHF | 2–47 pF | 500 V | 50 W | VHF matching | 519 MHz | PETG 40mm |
| VC-UHF | 0.5–12 pF | 200 V | 10 W | UHF matching, PCB | 1453 MHz | PETG 25mm |

---

## Appendix F: PVC Pipe Sliding Variable Capacitor

**TM-VCAP-SCH-004 / TM-VCAP-CAL-005 companion text**

### F.1 Operating Principle

The PVC pipe sliding capacitor forms a coaxial capacitor: two concentric cylindrical conductors separated by a thin polymer dielectric. Sliding the inner pipe deeper into the outer sleeve increases the conductor overlap length and thus the capacitance.

**Thin-dielectric cylindrical approximation:**

When the dielectric thickness *d* is much smaller than the inner conductor radius *a* (d/a < 0.02), the curved geometry "unrolls" to a flat-plate equivalent:

```
C(x) = ε₀ × εᵣ × (2πa) / d × x    [F, x in meters]
```

Where:
- `2πa` = circumference of inner conductor
- `d` = dielectric thickness
- `x` = overlap (insertion) depth

In practical units:

```
K [pF/mm] = ε₀ × εᵣ × 2π × a_m / d_m × 0.001
C(x) = C_min + K × x     [pF, x in mm]
```

`C_min` is the stray capacitance at zero insertion (typically 2–5 pF).

**Validation — PIPE-HF:**
- a = 10.77 mm, d = 0.130 mm, εᵣ = 3.40 (Kapton)
- K = 8.854×10⁻¹² × 3.40 × 2π × 0.01077 / 1.30×10⁻⁴ × 0.001 ≈ **15.7 pF/mm**
- Table value 17.7 pF/mm accounts for copper foil OD increase; use calibrated value

**Exact coaxial formula (large air gap):**

For air-dielectric designs without polymer fill:

```
C(x) = 2π × ε₀ × x / ln(b/a)
```

Where b = outer conductor inner radius. This gives lower C/mm for the same geometry; polymer dielectric improves this by a factor of εᵣ.

### F.2 Design Variants

| Design ID | Inner pipe | Dielectric | K (pF/mm) | Max depth | C_max (pF) | V_work |
|-----------|-----------|-----------|-----------|-----------|-----------|--------|
| PIPE-HF | 1/2" Sch 40 PVC | 2× Kapton 0.065mm | 17.7 | 150 mm | 2658 | 5200 V |
| PIPE-HF-HV | 1/2" Sch 40 PVC | PTFE tape 8× | 4.9 | 200 mm | 980 | 7333 V |
| PIPE-VHF | 3/8" nominal PVC | Kapton single layer | 22.9 | 20 mm | 458 | 2600 V |
| PIPE-QRP | 1/2" Sch 40 PVC | OHP transparency | 21.7 | 50 mm | 1085 | 1500 V |
| PIPE-UHF | 1/4" PVC tubing | Kapton single layer | 17.3 | 8 mm | 139 | 2600 V |

**PIPE-HF** is the primary design for HF portable field use. PIPE-HF-HV handles higher-power applications (200 W). PIPE-QRP requires no 3D printing — the flexible vinyl sleeve grips the pipe without a machined bore.

### F.3 Materials and Parts List

**Inner pipe (PIPE-HF):**
- 1/2" Schedule 40 PVC pipe (OD = 21.34 mm), cut to max_depth + 60 mm
- 0.10 mm copper foil tape with conductive adhesive (6 mm or wider roll)
- Kapton (polyimide) tape, 0.065 mm thickness, 8–10 mm wide
- 18 AWG stranded hookup wire (T₁ terminal lead), 400 mm minimum
- Thumb knob (3D-print TM-VCAP-ENC-006 thumb_knob) or PETG rod turned to suit

**Outer sleeve (PIPE-HF):**
- 3D-printed PETG sleeve (TM-VCAP-ENC-006 outer_sleeve), ID = 22.0 mm
  - OR: 1" garden soaker hose/vinyl tubing, ID ≈ 20 mm (relaxed), stretches to grip
- 0.10 mm copper foil tape for inner bore surface
- End caps (3D-print TM-VCAP-ENC-006 endcap_fixed, endcap_guide)
- M3 × 8 mm screws × 4 (chassis mounting)

**Motorized version only:**
- NEMA14 stepper motor, 200 steps/rev
- M6 × 1.0 mm threaded rod, length = max_depth + 50 mm
- M6 nut × 2 (one in inner pipe nut adapter, one as anti-backlash jam nut)
- 3D-print nut adapter (TM-VCAP-ENC-006 nut_adapter_inner)
- 3D-print motor mount plate (TM-VCAP-ENC-006 motor_mount_plate)
- DRV8825 driver; see TM-VCAP-SCH-002 for wiring

### F.4 Construction — Inner Pipe

**Step 1 — Clean the pipe surface.**
Wipe the outer surface of the PVC pipe with IPA. Remove all mold release agents, ink markings, and grease. The foil tape adhesive will not bond to contaminated surfaces.

**Step 2 — Apply copper foil tape.**
Using 0.10 mm copper foil tape with conductive adhesive, wrap helically along the pipe from the wire-end to the knob-end:
- Overlap each turn by 50% of tape width to ensure conductivity between turns
- Length of copper coverage = max_depth + 30 mm
- Leave the knob end 15 mm bare (no foil — this is the grip area)
- At the wire end, leave a 25 mm tab extending beyond the pipe end (this is the T₁ terminal tab)

Burnish the foil flat with a fingernail tool or credit card edge. Eliminate air bubbles at seams. Verify electrical continuity end-to-end with an ohmmeter: resistance should be < 0.5 Ω along the full copper length.

**Step 3 — Apply polymer dielectric.**
*For Kapton tape (preferred for PIPE-HF):* Wrap a single layer helically with 2 mm overlap per turn. Maintain uniform tension — overtightening stretches Kapton. For double-layer (PIPE-HF-HV), apply a second pass in the same direction. No wrinkles or folds: wrinkles create voltage hotspots that will arc first.

*For OHP transparency film (PIPE-QRP):* Cut a strip, width = π × pipe_OD + 5 mm. Wrap one revolution around the pipe. Secure at start and end with one small piece of Scotch tape. Check: no trapped air bubbles in center of plate overlap area.

*For PTFE plumber's tape (PIPE-HF-HV):* Wrap 8 tight turns, stretching tape slightly as you go. Total thickness ≈ 0.61 mm. PTFE tape's low εᵣ (2.1) reduces C/mm but greatly increases voltage rating.

**Step 4 — Quality check.**
Before inserting the inner pipe into the sleeve, check dielectric integrity: press a metal tool firmly against the outer surface of the dielectric over the copper foil area. Ohmmeter reading between the tool and the T₁ terminal foil tab must be **> 10 MΩ**. If it reads < 1 MΩ, there is a pinhole or fold — find and repair it. RF at 100 W will immediately punch through a compromised dielectric.

**Step 5 — Guide strips (optional, improves sliding feel).**
Apply three 3 mm × 50 mm strips of PTFE plumber's tape to the dielectric-covered pipe, positioned at 0°, 120°, and 240° around the circumference. These act as low-friction glide pads against the 3D-printed sleeve bore.

### F.5 Construction — Outer Sleeve

**Option A — 3D-Printed PETG Sleeve (standard):**
1. Print TM-VCAP-ENC-006 `outer_sleeve()` in PETG at 40% infill. Bore ID = 22.0 mm.
2. Sand the bore with 220-grit wrapped around a 21 mm dowel. Bore must be smooth.
3. Before inserting inner pipe, apply copper foil tape to the inside bore:
   - Cut one strip = π × 22 mm wide × sleeve_length long
   - Roll around a smaller rod, slide into bore, press against wall, burnish
   - The tape must lay flat — creases in the outer conductor cause C non-linearity
4. Check: ohmmeter continuity along full length of sleeve bore foil < 1 Ω.

**Option B — Flexible Vinyl Sleeve (no 3D printing):**
1. Use 1" garden soaker hose or soft vinyl tubing. Relaxed ID ≈ 20 mm (stretches to 21.3 mm when inner pipe is inserted).
2. Turn inside-out. Apply copper foil tape to the section that will be active: length = max_depth mm. Burnish.
3. Turn right-side-out. Trim ends.
4. Advantage: the tubing self-centers and grips the inner pipe without clearance gap. Consistent intimate contact. No printed tolerance issues.
5. Disadvantage: more friction; best for hand-tuned QRP use.

**Option C — Flat Sheet Variant:**
Instead of round pipe, use a flat PVC sheet (60 × max_depth+30 mm) as the inner plate. Apply copper foil to both faces with polymer sheet between. Slide into a PVC channel (slotted guide from flat PVC). This uses the same flat-plate formula C = ε₀εᵣWx/d and may be easier to fabricate at large scale.

### F.6 Electrical Connections

**T₁ (inner pipe / moving conductor):**
Solder an 18 AWG flexible stranded wire to the copper foil tab at the far end of the inner pipe. Wire must be at minimum 400 mm long — long enough to remain slack throughout the full 150 mm insertion range. Use coiled wire (stretch telephone cord) for compact storage.

**CRITICAL:** As the inner pipe moves in and out, this wire curls and uncoils. If the wire is under tension at full insertion, it will pull the inner pipe out of position, causing calibration drift. Plan slack carefully. Route the wire inside the sleeve body through a slot in the end cap.

**T₂ (outer sleeve / fixed conductor):**
Solder wire to the outer conductor foil through the access slot in the fixed end cap (TM-VCAP-ENC-006). Alternatively, extend the foil tape as a tab past the sleeve end and solder there.

**HV terminal isolation:**
At working voltages > 500 V (any transmitter use), keep T₁ and T₂ leads physically separated:

| Voltage | Minimum air gap between leads |
|---------|-------------------------------|
| 500 V | 5 mm |
| 1500 V | 15 mm |
| 5000 V | 50 mm |
| 7500 V (PIPE-HF-HV) | 75 mm |

Insulate the thumb knob from the inner pipe with a PTFE sleeve if operating above 200 V.

### F.7 Firmware Integration

The ESP32 firmware (TM-VCAP-FW-001) supports the pipe cap in LINEAR drive mode. Enable it in `config.h`:

```cpp
// Uncomment the linear drive mode and comment out rotary:
// #define CAP_DRIVE_MODE_ROTARY
#define CAP_DRIVE_MODE_LINEAR

// Set pipe design parameters:
#define PIPE_C_PER_MM     17.7f   // pF/mm (calibrated value from SETC PIPE_K)
#define PIPE_C_MIN_PF      3.0f   // stray C at zero insertion
#define PIPE_MAX_DEPTH_MM 150.0f  // maximum travel
#define MM_PER_MICROSTEP  0.000625f  // M6×1mm, 1/8 µstep
```

**Serial commands (same parser as rotary mode):**
- `HOME` — drive to limit switch at x = 0 mm (minimum insertion)
- `GOTO <pF>` — calculate required depth x = (C_target − C_min) / K, drive there
- `STEP +<n>` or `STEP -<n>` — advance or retract n microsteps
- `SETC PIPE_K <value>` — store calibrated K (pF/mm) in NVS, replaces #define
- `STATUS` — prints current position in mm and pF
- `BAND <name>` — move to band preset using pipe cap target C

**Position sensing:** The lead-screw motor counts microsteps from the hardware home limit switch. No separate encoder is needed at HF. If missed steps are a concern (motor overloaded), add a rotary potentiometer on the lead screw shaft and use it for coarse verification.

### F.8 Calibration Procedure

1. **Set zero.** Issue `HOME` command. The motor drives to the minimum limit switch. This is x = 0 mm, C = C_min. Note the position marker alignment on the inner pipe scale.

2. **Insert to 10 mm.** Issue `GOTO 180` (C_target = C_min + K×10 ≈ 3 + 177 = 180 pF). Wait for motor to stop. Measure with LCR meter at 1 kHz.

3. **Compare to expected.** Expected ≈ C_min + K × 10 mm. If measured value differs by more than 10%:
   - Low reading: air gaps in dielectric wrap — re-wrap more tightly, re-test
   - High reading: tube deformation reducing gap d — inspect pipe for ovality or dents

4. **Update K.** Calculate calibrated K = (C_measured − C_min) / 10. Issue: `SETC PIPE_K <K>`. This value is stored in NVS and used for all subsequent GOTO and BAND commands.

5. **Verify linearity.** Drive to 50 mm, 100 mm, 150 mm. Measure C at each. Plot C vs. x. Should be linear ± 5%. Non-linearity > 10% indicates:
   - Gradual buildup of wrinkles in dielectric (re-wrap)
   - Sleeve bore not uniform diameter (replace sleeve)

6. **Mark scale.** At each verified position, mark the inner pipe with a Sharpie: both mm value and pF value. Reference: TM-VCAP-SCH-004 Section 3 position scale table.

### F.9 Hybrid Operation

The pipe cap is designed to work in series with the stack-on modular units (Appendix G). The stack-on units provide coarse C (large steps per band change); the pipe cap provides fine continuous tuning within the band.

Firmware: issue `SETCSM <pF>` to set the base capacitance from installed stack-on units. Subsequent `GOTO` and `BAND` commands target the residual C (fine-tune portion) on the pipe cap. Example:

```
SETCSM 54        ← installed CSM-E (54 pF)
BAND 20m         ← firmware targets total 60 pF → pipe cap goes to ~6 pF
SETCSM 0         ← removed all stack-on units
BAND 20m         ← pipe cap goes to full 60 pF
```

---

## Appendix G: Stack-On Modular Flat-Plate Capacitor Units (CSM Series)

**TM-VCAP-CAL-006 / TM-VCAP-SCH-005 companion text**

### G.1 Concept and Applications

Stack-on capacitor units (CSM = Compact Stack-on Module) are small flat-plate capacitors made from copper foil tape, overhead transparency film, and a PVC or foam-core backing. They connect in parallel by aligning their foil tabs — adding units increases total capacitance in discrete steps.

**Why stack-on units?**

A variable capacitor covers a limited range. The PIPE-HF covers 0–2658 pF continuously, but at HF (3–30 MHz) typical L-network capacitor values range from 18 pF (10 m) to 250 pF (160 m) — a 14:1 range. Covering this range with the pipe cap alone requires 150 mm of travel, and fine control at one end is mechanically coarser at the other.

By pre-setting a base C with stack-on units (coarse), the pipe cap only needs to cover ±25–50 pF for fine tuning on any given band. This gives:
- Faster band changes (snap on/off a unit vs. large motor travel)
- Better fine-tuning resolution (small pipe travel = precise control)
- Redundancy — if the motor or electronics fail, you can manually adjust stack-on units and tune by ear

**Typical field workflow:**
1. Look up recommended stack for desired band in the band selection table (Section G.6)
2. Snap the correct unit(s) onto the velcro panel
3. Set `SETCSM <C_base>` in firmware (or set manually in analog tuners)
4. Use pipe cap or rotary trim cap for final SWR dip

### G.2 Flat-Plate Capacitance Theory

A flat-plate capacitor with two parallel conductors separated by a dielectric:

```
C = ε₀ × εᵣ × A / d
```

Where:
- A = overlap area in m² (plate_W × plate_L)
- d = dielectric thickness in m
- εᵣ = relative permittivity of dielectric

**For overhead transparency film (OHP):**
- εᵣ = 3.20, d = 0.100 mm
- C = 8.854×10⁻¹² × 3.20 × A_mm² × 10⁻⁶ / 0.0001
- C [pF] = **0.2833 × A_mm²**

This is the fundamental sizing equation. Each mm² of plate area = 0.2833 pF. A 16×12 mm plate gives A = 192 mm² → C = 54 pF.

**Multiple dielectric layers (series capacitors):**

Stacking N identical dielectric sheets halves C with each additional layer:
- 1 layer (0.1 mm): C = 0.2833 pF/mm²
- 2 layers (0.2 mm): C = 0.1417 pF/mm²
- 3 layers (0.3 mm): C = 0.0944 pF/mm²

Two-layer construction doubles the voltage rating (3000 V vs. 1500 V) at the cost of halved capacitance — appropriate for 500 W transmitters.

**Voltage rating per layer:**
- E_breakdown (OHP) ≈ 45 MV/m
- V_breakdown per layer = 45 × 10⁶ × 0.0001 = 4500 V
- V_working (3× safety factor) = **1500 V per layer**
- Two layers: V_working = 3000 V

At 100 W into a Q = 5 tuner tank, V_peak across the capacitor ≈ 500 V. A single OHP layer (1500 V rating) provides 3× margin. Adequate for 100 W. For 500 W with Q = 10, V_peak ≈ 3000 V — use two-layer construction.

### G.3 CSM Unit Specifications

| Unit | Plate W×L (mm) | Area (mm²) | Layers | C (pF) | V_work | Mass | Fastening | Primary band |
|------|---------------|-----------|--------|--------|--------|------|-----------|-------------|
| CSM-A2 | 50 × 35 | 1750 | 2 | 248 | 3000 V | 9 g | 25mm velcro | 160 m |
| CSM-B2 | 40 × 28 | 1120 | 2 | 159 | 3000 V | 6 g | 20mm velcro | 80 m |
| CSM-C2 | 32 × 22 | 704 | 2 | 100 | 3000 V | 4 g | 20mm velcro | 40 m |
| CSM-D | 22 × 16 | 352 | 1 | 100 | 1500 V | 2 g | 12mm velcro | 30 m |
| CSM-E | 16 × 12 | 192 | 1 | 54 | 1500 V | 1 g | 12mm velcro | 20 m |
| CSM-F | 12 × 9 | 108 | 1 | 31 | 1500 V | 0.5 g | Rubber band | 15 m |
| CSM-G | 9 × 7 | 63 | 1 | 18 | 1500 V | 0.3 g | Rubber band | 10 m |
| CSM-VHF | 7 × 5 | 35 | 1 | 10 | 1500 V | 0.2 g | Rubber band | VHF |

**Notes on single-layer (CSM-D through CSM-VHF):**
These smaller units use single OHP layers (1500 V). At HF (7 MHz and up), 100 W transmitters develop maximum ~500 V peak across the tank capacitor at Q = 5. The 1500 V rating gives 3× headroom, which is adequate for field use. However, do not use single-layer units in a 500 W station without verifying that Q × V_peak < 1500 V.

### G.4 Fabrication Procedure

**Materials per unit:**
- PVC backing strip: cut from flat PVC sheet (0.5–1.0 mm thick), OR use FR4 PCB laminate (copper-free), foam-core board, or ABS sheet. Dimensions: plate_W × plate_L.
- Copper foil tape (0.10 mm, conductive adhesive): two pieces per unit, each plate_W × (plate_L + 8 mm) for tab extension.
- OHP transparency film (0.10 mm): one or two sheets, plate_W × plate_L.
- Self-adhesive velcro (hook side on tray, loop side on adjacent unit or chassis), or rubber bands.
- 20 AWG insulated wire, 50 mm leads.

**Construction sequence:**

1. **Backing.** Cut PVC or FR4 backing to plate_W × plate_L. Sand edges smooth.

2. **Bottom conductor.** Apply copper foil tape to the bottom face of the backing. Extend 5 mm past the front (left) edge for the bottom terminal tab. Burnish completely flat.

3. **Dielectric.** Place OHP sheet on top of the bottom foil. The sheet covers only the plate area (plate_W × plate_L) — it must NOT extend to the tab area. This ensures the tab is bare copper for soldering.
   - For 2-layer construction: stack a second OHP sheet on top of the first.

4. **Top conductor.** Apply copper foil tape to the top face of the OHP sheet. Extend 5 mm past the front edge for the top terminal tab, but offset this tab 3 mm down from the plate edge (see below). This physical offset prevents the top and bottom tabs from accidentally shorting when units are aligned in a stack.

5. **Tab geometry.** Looking at the front edge of the unit:
   ```
   ┌─ TOP TAB ─┐  ← 5mm copper extension, at TOP of front edge
   ─▓▓▓▓▓▓▓▓▓▓
     ─────────────▓▓▓▓▓▓▓▓▓▓▓  ← BOTTOM TAB, 3mm lower (no accidental short)
   └─ BOT TAB ─┘
   ```

6. **Wire leads.** Solder a 50 mm 20 AWG lead to each tab. Use flux and low heat — copper foil tape adhesive softens above 80°C. Tin the wire and the tab separately before joining.

7. **Velcro.** Apply 12–25 mm self-adhesive velcro hook strip to the long face (back) of the backing. The matching loop strip goes on the adjacent unit or on the tuner chassis panel.

8. **Label.** Write unit ID, C value (pF), and band with Sharpie on the backing, or apply Dymo label. Format: "CSM-E / 54 pF / 20m"

9. **Tray (optional).** For organized storage and robust tab protection, print TM-VCAP-ENC-007 `stackon_cap_unit.scad` tray (set `UNIT_ID` to match your unit). The tray includes terminal posts for strain relief, compression ridge for foil contact, and velcro slot on the back face.

### G.5 Stacking and Parallel Connection

CSM units stack in **parallel**: all top-foil tabs connect to one terminal (T₁); all bottom-foil tabs connect to the other (T₂). Total capacitance = sum of all installed unit capacitances.

**Mechanical fastening:**
- **Velcro method (CSM-A2 through CSM-E):** Apply hook velcro to each unit's backing. Matching loop velcro on the adjacent unit back and on the tuner chassis panel. Units can be added or removed in seconds; position is maintained when peeled apart. Stack vertically (foil tabs all at the same end).
- **Rubber band method (CSM-F, CSM-G, CSM-VHF):** Wrap two rubber bands around the stack in perpendicular directions (one lengthwise, one widthwise). Use an alligator clip to grab the combined tab stack. Fast, simple, works for bench use.

**Electrical contact:**

The copper foil tabs are the electrical bus. When units are stacked with tabs aligned:
- All top tabs in contact = top conductor bus
- All bottom tabs in contact = bottom conductor bus

For reliable contact in field conditions, add a **spring contact strip**: a 0.15 mm phosphor-bronze strip (3 mm wide), bent into a slight arc, crimped onto the tab stack with a small binder clip. This ensures low-resistance contact (< 0.1 Ω) regardless of unit count or velcro tension variation.

**Verify total C.** After assembling the desired stack, measure with an LCR meter. The total should be within ±15% of the sum of individual unit values. Larger deviations indicate poor contact between tabs — check alignment, add spring strip.

### G.6 Band Selection Guide

The following table gives recommended starting stacks for each HF and VHF/UHF band, using a typical L-network tuner configuration (series C to antenna, shunt L to ground). Fine-tune with the pipe cap (Appendix F) or a small rotary trimmer in series.

| Band | Freq (MHz) | Target C (pF) | Recommended stack | Stack C (pF) | V_work |
|------|-----------|--------------|-------------------|-------------|--------|
| 160 m | 1.8 | 200–250 | 1× CSM-A2 | 248 | 3000 V |
| 80 m | 3.5 | 130–160 | 1× CSM-B2 | 159 | 3000 V |
| 40 m | 7.0 | 80–120 | 1× CSM-C2 | 100 | 3000 V |
| 30 m | 10.1 | 55–85 | 1× CSM-C2 **or** 1× CSM-D + 1× CSM-E | 100 or 154 | 3000 V |
| 20 m | 14.0 | 30–60 | 1× CSM-E | 54 | 1500 V |
| 17 m | 18.1 | 22–40 | 1× CSM-F + 1× CSM-G | 49 | 1500 V |
| 15 m | 21.0 | 18–35 | 2× CSM-F | 62 | 1500 V |
| 12 m | 24.9 | 12–22 | 1× CSM-F + 1× CSM-G | 49 | 1500 V |
| 10 m | 28.0 | 10–18 | 1× CSM-G | 18 | 1500 V |
| 6 m | 50.0 | 8–20 | 2× CSM-VHF | 20 | 1500 V |
| 2 m | 146.0 | 4–10 | 1× CSM-VHF | 10 | 1500 V |
| VHF trim | — | 1–5 | 1× CSM-VHF — partial insertion | variable | 1500 V |

**NOTE:** These are starting points, not exact settings. Antenna impedance varies with height, ground conductivity, nearby conductors, and frequency within the band. Always fine-tune with the pipe cap or a trimmer cap after installing the stack-on base.

**Finding the right stack in the field without a table:**

Recall: C is roughly proportional to 1/f² for a fixed-length resonant antenna. If you know the correct C for one band:
- Halving the frequency (doubling wavelength) requires ~4× the capacitance
- Doubling the frequency requires ~¼ the capacitance

Example: 20 m at 60 pF → 10 m at ~15 pF → 40 m at ~240 pF. This mental arithmetic gets you to within one unit of the right stack.

### G.7 Hybrid System — Stack-On + Pipe Cap

This is the primary intended operating mode for field HF operation.

```
Stack-on units          Pipe cap        Antenna feed
T₁ ─────┤├──────────────┬──────── ||slide|| ───────── to load
          (coarse C)    │          (fine C)
                         └── RF ground via tuner shunt path
```

**Operating procedure:**

1. **Select band preset.** Look up the band in the selection table above. Install the recommended stack-on unit(s) on the velcro panel. Announce the base C to the firmware: `SETCSM <C_base_pF>`.

2. **Zero the pipe cap.** Issue `HOME` command. Motor drives to x = 0.

3. **Request band tune.** Issue `BAND 20m` (or the desired band). Firmware calculates required C_fine = C_band_nominal − C_base, converts to mm depth, drives the pipe cap.

4. **Monitor SWR.** Check SWR meter. If SWR > 1.5:1, issue `STEP +100` or `STEP -100` to nudge the pipe cap in 0.0625 mm increments. Most antennas reach 1.2:1 within ±20 steps.

5. **Lock and save.** Issue `SAVE` to store the current pipe position in NVS. On next startup, the firmware will return to this position when `BAND 20m` is issued.

**Fine-tuning across a wide band:**

For a band like 80 m (3.5–4.0 MHz), the required C changes by ~15–20 pF across the band. The pipe cap covers this continuously. Use `STEP` commands to track SWR as you QSY. The pipe cap travels approximately 1 mm (18 steps of MOTOR_SPEED_SLOW) for a 17.7 pF change — less than a quarter turn of the thumb knob.

### G.8 Voltage and Power Ratings

At 100 W into a 50 Ω resistive load:
- V_rms = √(P × R) = √(100 × 50) = 70.7 V
- V_peak = 70.7 × √2 = 100 V

At 100 W into an HF tuner tank (typical Q = 3–8):
- V_C_peak = Q × V_peak = 5 × 100 = **500 V peak** (Q = 5 case)

Single-layer OHP (V_work = 1500 V) provides **3× margin** over this figure. Adequate for 100 W at any Q ≤ 15.

| Power | Q_tank | V_C_peak | Required V_work | CSM layer count |
|-------|--------|---------|-----------------|----------------|
| 10 W | 5 | 158 V | ≥ 200 V | Single layer OK |
| 100 W | 5 | 500 V | ≥ 600 V | Single layer OK |
| 100 W | 10 | 1000 V | ≥ 1200 V | Single layer (marginal) |
| 500 W | 5 | 1118 V | ≥ 1500 V | 2-layer required |
| 500 W | 10 | 2236 V | ≥ 2700 V | 2-layer required |

**CAUTION:** High-Q tuner with mismatched antenna can produce unexpected high voltage. When tuning across a mismatch (SWR > 3:1 presented to tuner), the tank voltage can be much higher than steady-state calculations suggest. When initially tuning into an unknown or damaged antenna, use reduced power (< 10 W) until an SWR minimum is found.

**Visual inspection:**
Before each field session, inspect CSM units:
- Hold each unit to the light and check the OHP dielectric for carbonization (black or brown spots)
- Check tab solder joints for corrosion or cold solder
- Check foil tape edges for lifting or delamination
- Replace any unit showing carbonization — it has experienced voltage breakdown and its rating is compromised

### G.9 Storage and Field Organization

**Pill organizer method:** A weekly pill organizer (7 days × 4 compartments = 28 slots) holds the complete CSM set twice over. Organize by descending capacitance (A2 first compartment, VHF last). A second organizer holds backup units.

**Small Pelican case:** A Pelican 1010 micro case (4.9 × 3.3 × 1.7 inches) holds the 8-unit set in a 4×2 grid using foam-cut slots sized to each unit's dimensions.

**Labeling:** Dymo label on each unit: unit ID, C in pF, primary band. Example: `A2 / 248pF / 160m`. If using Sharpie directly on the PVC backing, overcoat with clear fingernail polish to prevent smearing.

**Field replacement:** If a unit is lost or damaged, fabricate a replacement from materials found in most hardware stores: OHP transparency from an office supply store, copper foil tape from a hardware store (plumbing section uses it for slug barriers), and PVC trim board or foam-core board. Construction time per unit: approximately 15 minutes. Capacitance tolerance from hand-built units is typically ±10–15% — measure and re-label.

---

*End of Document — TM-VCAP-001 Rev A (revised: VHF/UHF, PVC pipe cap, stack-on units)*

*For corrections or additions, contact: d.ecaudata@gmail.com*
