# MOTORIZED COIL WINDER SYSTEM
## Technical Manual TM-CWND-001 Rev A
### Precision Coil Winding Machine for HF Inductors, RF Chokes, and Transformers

---

**Document Control**

| Item | Value |
|------|-------|
| Document Number | TM-CWND-001 |
| Revision | A |
| Date | 2026-05-01 |
| Author | Merv Martin — KG6VJF |
| Classification | Amateur / Educational |
| Applicability | All TM-CWND-001 series hardware |

---

> **WARNING**
> This machine includes rotating machinery capable of entangling wire, clothing, hair,
> and fingers. Do not operate without clear area around rotating chuck.
> Keep fingers, tools, and loose clothing away from the chuck and wire path during operation.
> Emergency stop button must be accessible at all times during operation.

> **CAUTION**
> Fine magnet wire (AWG 24–30) breaks at very low force (less than 50 grams).
> Sudden wire breaks may release stored spring energy in tensioner. Eye protection
> is recommended when winding fine wire.

---

## TABLE OF CONTENTS

1. [Introduction](#1-introduction)
2. [Theory of Coil Winding](#2-theory-of-coil-winding)
3. [Machine Description](#3-machine-description)
4. [Mechanical Assembly](#4-mechanical-assembly)
5. [Electrical Assembly](#5-electrical-assembly)
6. [Controller Firmware Architecture](#6-controller-firmware-architecture)
7. [CYD Touchscreen Display](#7-cyd-touchscreen-display)
8. [Calibration Procedures](#8-calibration-procedures)
9. [Operating Procedures](#9-operating-procedures)
10. [Winding Tables](#10-winding-tables)
11. [Maintenance](#11-maintenance)
12. [Troubleshooting](#12-troubleshooting)

**Appendices**
- [A: Specifications](#appendix-a-specifications)
- [B: Bill of Materials](#appendix-b-bill-of-materials)
- [C: Wheeler Formula Reference](#appendix-c-wheeler-formula-reference)
- [D: Wire Reference Tables](#appendix-d-wire-reference-tables)

---

## 1. INTRODUCTION

### 1.1 Purpose

This manual provides complete technical description, assembly instructions, operating procedures, and maintenance guidance for the TM-CWND-001 Motorized Coil Winder System. The machine is designed for the amateur radio experimenter who regularly winds custom inductors, RF chokes, baluns, and power transformers.

### 1.2 Scope

This manual covers:

a. Coil winding theory — inductance calculation, wire selection, layer winding
b. Machine mechanical design — base, chuck, traverse, tensioner
c. Electrical design — ESP32 controller, stepper drivers, sensors
d. Firmware — turn counter, traverse synchronization, auto-wind mode
e. CYD touchscreen interface — remote display and control
f. Calibration and operating procedures
g. Winding tables for common HF, VHF, and power applications

### 1.3 Design Goals

| Parameter | Goal | Achieved |
|-----------|------|----------|
| Wire gauge range | AWG 10–30 | AWG 10–30 + magnet wire |
| Coil former diameter | 0.25"–4" | 6.35–102mm |
| Maximum coil length | 12" (300mm) | 310mm traverse travel |
| Turn accuracy | ±1 turn | ±0.5 turns (optical sensor) |
| Traverse accuracy | ±0.1mm | ±0.0625mm (1600 steps/mm) |
| Inductance calculation | Wheeler formula | Implemented, both single/multi-layer |
| Preset storage | Yes | 10 NVS presets + SD card log |
| Remote display | CYD | Bluetooth SPP, 4 display modes |

### 1.4 Related Documents

| Document | Title |
|----------|-------|
| TM-RLIND-001 | Motorized Roller Inductor Manual |
| TM-VCAP-001 | Variable Capacitor System Manual |
| TM-PWRMTR-001 | RF Power Meter System Manual |

---

## 2. THEORY OF COIL WINDING

### 2.1 Inductor Fundamentals

An inductor stores energy in a magnetic field. For a coil of N turns carrying current I, the stored energy is:

```
W = ½ × L × I²
```

The inductance L depends on the physical geometry of the winding.

### 2.2 Wheeler's Formula — Single Layer

For a single-layer air-core coil (the most common amateur radio construction):

```
L(µH) = (d² × N²) / (18d + 40l)
```

Where:
| Symbol | Quantity | Units |
|--------|----------|-------|
| L | Inductance | µH |
| d | Coil diameter | inches |
| N | Number of turns | dimensionless |
| l | Winding length | inches |

This formula was derived empirically by Harold Wheeler in 1928 and is accurate to within 1% for coils where d/l is between 0.1 and 10.

#### 2.2.1 Example Calculation

For an 80m loading coil (L = 10 µH, 2" diameter former, 2" winding length):

```
10 = (4 × N²) / (36 + 80)
N² = 10 × 116 / 4 = 290
N = 17 turns
```

#### 2.2.2 Inverted Form — Turns from Inductance

Solving Wheeler's formula for N:

```
N = √(L × (18d + 40l) / d²)
```

The firmware uses Newton-Raphson iteration to solve the full form more accurately.

### 2.3 Wheeler's Formula — Multi-Layer

For a multi-layer coil (bankwound or layer-wound):

```
L(µH) = (0.8 × a² × N²) / (6a + 9l + 10c)
```

Where:
| Symbol | Quantity | Units |
|--------|----------|-------|
| a | Mean radius of winding | inches |
| N | Total turns | dimensionless |
| l | Winding length (axial) | inches |
| c | Radial build (winding depth) | inches |

Mean radius:
```
a = (r_former + c/2)
```

Where r_former is the former radius and c is the total winding thickness (layers × wire diameter).

### 2.4 Q Factor

The quality factor Q represents the ratio of energy stored to energy dissipated per cycle:

```
Q = X_L / R_total = (2π × f × L) / R_total
```

For an air-core coil wound with copper wire, the dominant loss mechanism depends on frequency:

| Frequency range | Dominant loss |
|----------------|---------------|
| < 1 MHz | DC resistance of wire |
| 1–30 MHz | Skin effect (current concentrates on conductor surface) |
| > 30 MHz | Proximity effect, radiation |

#### 2.4.1 Skin Depth

At radio frequencies, current flows only in a thin skin layer on the conductor surface:

```
δ = 1 / √(π × f × µ₀ × σ)
```

For copper (σ = 5.8×10⁷ S/m) at 7 MHz: δ ≈ 25 µm

A solid conductor uses its cross-section efficiently only when its radius is comparable to or smaller than δ. For AWG 22 (0.644mm diameter, 0.322mm radius): at 7 MHz, radius >> δ, so most conductor cross-section carries no current and the effective RF resistance is much higher than DC resistance.

**Rule of thumb:** RF resistance at HF = DC resistance × √(f_MHz / 0.05)

For maximum Q at HF:
- Use silver-plated wire (silver has higher conductivity than copper near surface)
- Use Litz wire for frequencies below 1 MHz (bundled fine conductors)
- Keep winding pitch at 1.5–2× wire diameter for single-layer coils (reduces proximity effect)

#### 2.4.2 Practical Q Values

| Application | Typical Q | Wire | Note |
|-------------|-----------|------|------|
| HF loading coil (3.5–30 MHz) | 100–300 | AWG 18–22 Cu | Air core, 2–3" dia |
| HF tank coil | 200–400 | AWG 14–18 Cu | Large diameter, spaced |
| RF choke (50–200 MHz) | 30–80 | AWG 26–30 | Close wound, small |
| Toroidal (ferrite core) | 30–200 | AWG 22–28 | Core loss dominant |
| Power inductor (< 100kHz) | 20–80 | AWG 10–16 | DC resistance dominant |

### 2.5 Self-Resonant Frequency

Every wound coil has distributed capacitance between turns. At the self-resonant frequency, this capacitance resonates with the coil's inductance, making the coil appear as a very high impedance rather than an inductance:

```
f_SRF = 1 / (2π × √(L × C_distributed))
```

Typical distributed capacitance for a single-layer solenoid: 0.5–5 pF.

**Important:** The coil only behaves as an inductance at frequencies well below f_SRF. At f > f_SRF, the coil appears capacitive.

**Rule of thumb:** Operating frequency should be < f_SRF / 3 for well-behaved inductive behavior.

### 2.6 Winding Pitch and Spacing

**Close-wound:** Turns touching (pitch = 1.0 × wire diameter). Maximum inductance per unit length, but higher distributed capacitance, lower Q.

**Space-wound:** Turns separated by gap (pitch = 1.5–2.0 × wire diameter). Lower distributed capacitance, higher Q, lower inductance per unit length.

**Recommended pitch:**
- Inductors for resonant circuits (Q-critical): pitch = 1.5–2.0
- Loading coils (L-critical): pitch = 1.0–1.2
- RF chokes: pitch = 1.0 (maximum suppression)
- Power inductors: pitch = 1.0 (maximum energy storage)

The firmware `pitch_factor` parameter directly controls the traverse advance per turn: `traverse_advance = wire_dia × pitch_factor`.

### 2.7 Toroidal and Core-Based Inductors

The TM-CWND-001 is designed for air-core and ferrite-core coil winding. For ferrite and powdered iron toroids, the inductance is:

```
L(µH) = A_L × N²  (A_L in µH/100 turns² = nH/turn²)
```

Where A_L is the core's inductance per turn squared, obtained from the manufacturer's data for the specific core material and size.

For toroids: the machine can be adapted with a toroid winding shuttle, but this requires manual operation of the shuttle threading the wire through the core center. The machine assists by controlling wire tension and counting turns.

### 2.8 Transformer and Choke Design

#### 2.8.1 Transmission Line Choke (Balun/Unun)

A transmission line choke consists of coaxial cable or bifilar wire wound on a ferrite core. The inductance seen by common-mode current:

```
L_cm = A_L × N²
```

Required choking reactance at lowest operating frequency:
```
X_choke = 2π × f_min × L_cm ≥ 10 × Z_line  (rule of thumb)
```

For a 50Ω system at 1.8 MHz: X_choke ≥ 500 Ω → L_cm ≥ 44 µH.

#### 2.8.2 Bifilar and Trifilar Winding

For 1:1 current baluns, two wires are wound simultaneously side by side. The machine supports this by threading both wires through the guide eye simultaneously (use a larger eye, or split-eye with two holes).

### 2.9 Capacitance and Voltage Stress

Each turn in a multi-layer coil is at a different RF potential. Adjacent turns in the same layer have low potential difference, but turns in adjacent layers separated by one winding height may have high potential difference — especially in high-voltage tank circuits.

For a tank circuit at 1 kW, 7 MHz, with L = 10 µH in a series circuit:
```
V_peak = I_peak × X_L = √(2 × P × X_L) ≈ 838 V
```
Voltage per turn = 838 / N (approximately).
For N = 17 turns: ~49 V per turn.
Between adjacent layers (if winding reverses): up to 2 × 49 = 98 V.

**Implication:** Multi-layer tank coils wound in close layers may arc between layers at high power. Either use single-layer construction, or insert interlayer insulation (Kapton tape, PTFE sheet).

---

## 3. MACHINE DESCRIPTION

### 3.1 Mechanical Subsystems

The TM-CWND-001 consists of six mechanical subsystems:

1. **Machine base** — aluminum extrusion or printed PETG, 400mm × 180mm
2. **Spindle headstock** — chuck + bearing block + NEMA 23 motor drive
3. **Chuck mechanism** — 3-jaw T-slot sliding, 6–102mm range
4. **Traverse assembly** — LM12UU carriages on 12mm rods, T8 leadscrew
5. **Wire tensioner** — spring-loaded arm, adjustable, wire break sensor
6. **Control electronics** — panel-mount enclosure, front-panel display and controls

### 3.2 Spindle Drive

The spindle is driven by a NEMA 23 stepper motor through a direct or 1:1 coupling to the chuck shaft. No reduction is used on the spindle: the motor directly turns the chuck.

| Parameter | Value |
|-----------|-------|
| Motor | NEMA 23, 200 steps/rev, 2.5A |
| Microstepping | 1/16 (3200 pulses/rev) |
| Speed range | 0.5–150 RPM |
| Holding torque | 1.2 N·m (at rated current) |
| Chuck capacity | 6–102mm OD formers |

Speed range by wire gauge:

| Wire Gauge | Max RPM | Reason |
|-----------|---------|--------|
| AWG 10–14 | 20 RPM | High wire tension; motor torque limit |
| AWG 15–22 | 60 RPM | Medium tension; good for most work |
| AWG 23–28 | 120 RPM | Low tension; speed limited by traverse response |
| AWG 29–30 | 60 RPM | Very fine; slow to prevent breakage |

### 3.3 Traverse System

The wire guide carriage traverses the length of the coil former, driven by a NEMA 17 stepper motor and T8 leadscrew.

| Parameter | Value |
|-----------|-------|
| Motor | NEMA 17, 200 steps/rev, 1.7A |
| Microstepping | 1/16 (3200 steps/rev) |
| Leadscrew | T8 × 2mm pitch |
| Steps per mm | 1600 steps/mm |
| Linear resolution | 0.000625 mm/step |
| Maximum travel | 310mm |
| Homing | Limit switch at home (zero) position |

Traverse synchronization:
- One complete spindle revolution advances traverse by exactly one wire diameter × pitch factor
- Firmware calculates traverse steps per spindle revolution at program setup
- The turn sensor (one pulse per revolution) triggers traverse advance

### 3.4 Turn Counter

The turn counter uses an optical or Hall-effect sensor to count spindle revolutions.

**Optical method:** An IR LED/phototransistor pair straddles a flag on the spindle shaft. One flag = one pulse per revolution. The firmware ISR increments a counter on each falling edge.

**Hall effect method:** A small neodymium magnet (6×3mm) epoxied to the spindle collar, with an AH1806 Hall sensor. Triggers once per revolution.

Turn count accuracy: ±0.5 turns (one trigger at the end of the last revolution). If winding stops mid-revolution, the partial turn is not counted. Firmware halts at target turn count + accepts final partial-turn completion via final sensor pulse.

### 3.5 Wire Tensioner

The spring-loaded tensioner maintains consistent wire tension from the spool to the coil former.

| Parameter | Value |
|-----------|-------|
| Mechanism | Spring lever arm |
| Spring constant | ~0.5 N/mm |
| Tension range | 0.2–5 N (via preload adjustment) |
| Wire break detection | Optical or limit switch on arm |
| Supported gauges | AWG 10 (2.6mm dia) to AWG 30 (0.3mm dia) |

> **NOTE**
> Fine magnet wire (AWG 26–30) requires very low tension — typically 0.2–0.5 N.
> The tensioner spring must be lightly preloaded for fine wire.
> Heavy wire (AWG 10–14) requires 3–5 N to prevent sag and loose turns.
> Reset the tensioner spring preload when changing wire gauge significantly.

### 3.6 Chuck Design

The 3-jaw T-slot sliding chuck accommodates cylindrical coil formers from 6.35mm (1/4") to 102mm (4") diameter.

Jaw extension range: 0mm (minimum, grips 6mm via center insert) to 40mm (maximum, grips 102mm OD).

For irregular cross-sections (square forms, hex forms): use soft-jaw inserts (foam or rubber pad bonded to jaw face). For PVC pipe formers: use split-jaw collet style for larger diameters.

---

## 4. MECHANICAL ASSEMBLY

### 4.1 Parts List and Hardware

| Item | Part | Qty | Source |
|------|------|-----|--------|
| Base rail | 12mm smooth rod, 400mm | 2 | Various |
| Leadscrew | T8 × 2mm pitch, 400mm | 1 | AliExpress |
| T8 nut | Anti-backlash brass nut | 1 | AliExpress |
| Linear bearings | LM12UU | 4 | AliExpress |
| Bearings | 6001ZZ (12×28×8) × 2 | 2 | VXB or local |
| Bearing (T8 end) | F608ZZ flanged, 8mm bore | 2 | AliExpress |
| Spindle motor | NEMA 23, 1.8° step | 1 | StepperOnline |
| Traverse motor | NEMA 17, 1.8° step | 1 | StepperOnline |
| Spindle coupler | 14mm–14mm jaw coupler | 1 | AliExpress |
| T8 coupler | 5mm–8mm flexible coupler | 1 | AliExpress |
| Printed parts | All SCAD files | — | Self-fabricated |
| Fasteners | M3, M4, M5 assorted | — | Hardware store |
| Rubber feet | M4, bumper type | 8 | McMaster |

### 4.2 Base Assembly

**Procedure 4.2 — Assemble Machine Base**

1. Print `base_plate.scad` as Section A and Section B (set `print_section="A"` then `"B"`).
2. Print splice plate (`print_section="SPLICE"`).
3. Insert M5 alignment pins (5mm × 20mm) into Section A end holes.
4. Align Section B onto pins. The two sections should form a flat 396mm × 180mm surface.
5. Install splice plate over joint. Secure with 4× M5 × 20mm screws.
6. Install rubber feet in counterbore recesses (all 8 corners and midpoints).
7. Verify base is flat on workbench (no rocking). If rocking: adjust feet height.

### 4.3 Rail and Leadscrew Installation

**Procedure 4.3 — Install Linear Rails and Leadscrew**

1. Insert rail rods (12mm × 400mm) through rail support bosses from headstock end.
2. Rods must slide through all bosses freely. If tight: ream boss bore lightly with 12.3mm drill.
3. Secure rod at headstock end with shaft collar (M3 set screw).
4. Secure rod at tailstock end with shaft collar.
5. Install anti-backlash T8 nut in carriage body pocket. Secure with M3 × 8 screws.
6. Thread T8 leadscrew through carriage nut.
7. Install flanged bearing (F608ZZ) in headstock end bearing boss for leadscrew.
8. Couple leadscrew to traverse motor shaft with 5mm–8mm flexible coupler.
9. Verify carriage slides freely on rods through full travel. If binding: check rail parallelism.

### 4.4 Spindle Headstock Assembly

**Procedure 4.4 — Install Spindle Headstock**

1. Press 6001ZZ bearing (12mm bore, 28mm OD) into headstock boss bore.
2. Insert spindle shaft (14mm OD, 200mm long stainless steel) through bearing.
3. Install chuck backplate on spindle shaft. Align key flat. Tighten M5 set screw with Loctite 222.
4. Install chuck body on backplate. Secure with 3× M4 × 20mm bolts.
5. Install 3 jaws in T-slots. Set jaws to minimum position.
6. Couple spindle shaft to NEMA 23 motor shaft with 14mm jaw coupler.
7. Mount NEMA 23 motor to motor mount plate. Align shafts coaxially.
8. Verify chuck rotates freely. If binding: re-align motor mount.

### 4.5 Chuck Jaw Adjustment

**Procedure 4.5 — Set Chuck Jaws for Former**

1. Loosen all three jaw M5 grub screws.
2. Slide jaws outward to accommodate former diameter + 5mm clearance.
3. Insert coil former in chuck.
4. Slide jaws inward until all three contact former evenly.
5. Tighten grub screws in sequence (tighten each half-turn, cycle through all three).
6. Verify former is centered and seated parallel to spindle axis.
7. Rotate spindle by hand one full revolution. Verify no wobble (runout < 1mm at 100mm from chuck).

> **CAUTION**
> Overtightening grub screws on plastic formers will crush them. Tighten only until
> the former is held firmly without significant jaw marks on the former surface.
> For hard formers (aluminum, acrylic): standard torque. For soft formers (PVC, foam, cardboard): hand-tight only.

### 4.6 Wire Tensioner Installation

**Procedure 4.6 — Install Wire Tensioner**

1. Mount tensioner base to machine frame at spindle end (or on separate post).
2. Install tensioner arm on pivot bolt. Bushing or F624ZZ bearing.
3. Install spring between tensioner arm and base tower. Compress spring preload:
   - AWG 10–16 (heavy): 3–5 turns of preload
   - AWG 17–22 (medium): 1–2 turns
   - AWG 23–30 (fine): minimum preload (spring barely compressed)
4. Install wire break sensor:
   - Optical gate: mount in bracket over wire path. Verify wire blocks IR path when present.
   - Limit switch: mount at high arm position. Verify switch activates when arm swings up.
5. Install appropriate wire guide eye for wire gauge:
   - AWG 10–16: 3.2mm bore eye
   - AWG 17–30: 1.6mm bore eye
   - Magnet wire (very fine): 0.5mm bore ceramic (commercial eyelet)

### 4.7 Spool Setup

The wire spool mounts on a horizontal axle on the tensioner post. Standard spool axle: 10mm diameter.

1. Slide spool onto axle through open slot.
2. Install axle end cap to prevent spool falling off.
3. Ensure spool rotates freely. If tight: reduce friction with small piece of PTFE tape on axle.
4. Route wire over lower guide post, through tensioner arm eye, to carriage guide eye.
5. Wind leader wire 2–3 turns on coil former by hand before starting machine.

---

## 5. ELECTRICAL ASSEMBLY

### 5.1 Controller Wiring Sequence

**Procedure 5.1 — Controller Wiring**

Follow schematic in `coil_winder_controller_ascii.txt`. Wire in this sequence:

**Step 1 — Power supply wiring:**
1. Connect 24V PSU output to PCB J3 power terminals. Verify polarity.
2. Do NOT power on yet.

**Step 2 — Stepper driver configuration:**
1. Set TMC2208/A4988 microstepping to 1/16 (MS1=1, MS2=1, MS3=1 for A4988; or Vref for TMC2208).
2. For TMC2208: set Vref = 0.96V for NEMA 17 (1.2A); 1.27V for NEMA 23 (2.0A Irun).
   Vref formula (A4988): Vref = I × 8 × R_sense (R_sense = 0.1Ω typical).
3. Insert driver boards into PCB headers.

**Step 3 — Motor wiring:**
1. Connect NEMA 23 spindle motor to Driver U1 output (1A/1B = coil A, 2A/2B = coil B).
2. Connect NEMA 17 traverse motor to Driver U2 output.
3. Verify resistance: each motor coil should measure 0.5–5Ω.

**Step 4 — Sensor wiring:**
1. Connect turn sensor (optical or Hall): Vcc=3.3V, GND=GND, OUT→GPIO35.
2. Connect wire break sensor: Vcc=5V, GND=GND, OUT→GPIO13 (with 10kΩ pullup).
3. Connect traverse limit switch: COM→GND, NO→GPIO39 (with 10kΩ pullup).
4. Connect foot pedal: COM→GND, NO→GPIO15 (with 10kΩ pullup).
5. Connect speed pot: wiper→GPIO36, ends to 3.3V and GND.

**Step 5 — Display and encoder:**
1. Connect OLED SSD1306: VCC=3.3V, GND, SDA→GPIO21, SCL→GPIO22.
2. Install 4.7kΩ pullup resistors on SDA and SCL to 3.3V.
3. Connect rotary encoder: CLK→GPIO32, DT→GPIO33, SW→GPIO34, + →3.3V, GND→GND.
4. Install 10kΩ pullup on CLK, DT, SW to 3.3V.

**Step 6 — Emergency stop:**
1. Wire E-stop mushroom button between 3.3V and ESP32 EN pin.
   Press button → EN goes LOW → ESP32 resets → all motor enables deasserted → motors stop.
2. Alternatively: connect E-stop NO contact to GPIO15 (foot pedal input) in parallel with foot pedal (either can stop the machine).

### 5.2 Power-On Test

**Procedure 5.2 — Initial Power-On Test**

> **WARNING**
> Do not install the coil former or wire until this test is complete.
> Keep fingers away from the chuck.

1. Connect USB-serial to ESP32.
2. Apply 24V power.
3. Observe serial output: `RF-CoilWinder fw=X.X.X`
4. Send `STATUS` command. Verify response contains: `state=0` (IDLE).
5. Send `HOME`. Observe traverse carriage moving slowly toward home limit.
6. When limit switch triggers: carriage should stop. Verify: `TRAV: homed`.
7. Send `TURNS 5` then `AWG 22` then `DIA 25.4` then `LEN 25.4`.
8. Send `CALC`. Verify response: `L=XX.XX µH` (approximately 2–3 µH for this geometry).
9. Send `RPM 10`. Spindle should NOT move yet (no `START` sent).
10. Hold fingers well clear. Send `START`. Verify spindle rotates at slow speed.
11. Verify turn counter increments on OLED display.
12. Verify traverse advances slowly.
13. After 5 turns: spindle should stop. OLED should show `DONE`.
14. If any motor does not move: check enable pin state, driver Vref, wiring.

---

## 6. CONTROLLER FIRMWARE ARCHITECTURE

### 6.1 File Structure

```
esp32_coil_winder/
├── config.h    — Pin assignments, motor constants, limits
├── winder.h    — WinderMath, WireSpec, WindingProgram, TurnCounter, TraverseController, WinderController
├── menu.h      — MenuSystem, PresetManager, SDLogger, RotaryEncoder
└── esp32_coil_winder.ino — Main loop, ISRs, BT, serial, LED
```

### 6.2 Traverse Synchronization Algorithm

The traverse advance is triggered by the turn sensor ISR. The critical path is:

```
Turn sensor falling edge
    → ISR: pulse_count++, rev_complete_flag = true
    → Loop: detects rev_complete_flag
    → WinderController::onRevComplete()
        → TurnCounter::update()
        → TraverseController::advanceOneWire(wire_dia, pitch_factor)
            → traverse steps = round(wire_dia × pitch_factor × 1600 steps/mm)
            → stepper->move(steps)
        → Check layer end
        → Check turn target
```

This architecture means traverse advance is not interrupt-driven — it happens in the main loop on the next iteration after the ISR. At 150 RPM and typical loop time of <1ms, the delay is negligible.

### 6.3 Traverse Steps per Turn Calculation

```
traverse_mm_per_turn = wire_dia_mm × pitch_factor
traverse_steps       = traverse_mm_per_turn × TRAV_STEPS_PER_MM
                     = traverse_mm_per_turn × 1600
```

For AWG 22 (insulated dia = 0.70mm), pitch_factor = 1.0:
```
traverse_steps = 0.70 × 1600 = 1120 steps per turn
```

Resolution verification: 1/1600 mm = 0.625µm per step → sub-turn accuracy is more than adequate.

### 6.4 Layer Winding Logic

For multi-layer coils, the machine reverses traverse direction at each layer end:

```
Layer 0: start → end (forward)
Layer 1: end → start (reverse)
Layer 2: start → end (forward)
...
```

This "bifilar" traversal minimizes voltage stress between adjacent layers (each layer's start turn is adjacent to the previous layer's end turn). For high-voltage coils requiring specific interlayer orientation, use single-layer winding only or add interlayer insulation.

Layer transition: when traverse reaches end_mm position, `TraverseController::atLayerEnd()` returns true, `WinderController::onRevComplete()` increments layer count and sets new traverse limits.

### 6.5 Command Reference

| Command | Arguments | Function |
|---------|-----------|----------|
| START | — | Begin winding with current program |
| STOP | — | Halt immediately |
| PAUSE | — | Pause at current position |
| RESUME | — | Resume from pause |
| HOME | — | Home traverse to limit switch |
| TURNS | N | Set target turn count |
| L | µH | Set target inductance (calculates turns) |
| AWG | gauge | Set wire gauge (10–30) |
| DIA | mm | Set former outer diameter (mm) |
| LEN | mm | Set coil winding length (mm) |
| RPM | speed | Set spindle speed (RPM) |
| LAYERS | N | Set number of layers |
| PITCH | factor | Set pitch factor (1.0=close-wound) |
| CALC | — | Calculate inductance from current program |
| WTABLE | — | Print complete wire specification table |
| PRESET SAVE N | slot | Save current program to NVS slot N |
| PRESET LOAD N | slot | Load program from NVS slot N |
| STATUS | — | Full status report |
| INFO | — | Firmware version and machine constants |
| RESET | — | Restart firmware |
| FAULT | — | Clear fault state |

### 6.6 State Machine

```
          ┌────────────────────────────────────────────────────────┐
          │                                                        │
          ▼                                                        │
        IDLE ──[START]──► WINDING ──[target reached]──► DONE ───┐ │
          │                  │                                   │ │
          │             [PAUSE cmd]                              │ │
          │                  │                                   │ │
          │             PAUSED ──[RESUME]──► WINDING             │ │
          │                                                      │ │
        DONE ──[HOME or START]──► HOMING ──[limit switch]──► IDLE│ │
                                                                 │ │
        FAULT ◄──────────────────────────────────────────────────┘ │
          │                                                        │
          └──[FAULT cmd]──────────────────────────────────────────┘
```

---

## 7. CYD TOUCHSCREEN DISPLAY

### 7.1 Hardware Connection

The CYD (ESP32-2432S028) connects to the controller via Bluetooth SPP. The CYD is the BT master; it initiates connection to "RF-CoilWinder".

BT connection sequence at startup:
1. CYD displays boot splash
2. Calls `btMaster.connect("RF-CoilWinder")`
3. If connection fails within 30s: retry automatically

### 7.2 Display Modes

**WIND mode** — Primary operating display:
- Large turn counter (cyan, green on done, red on fault)
- Remaining/target turns
- Circular progress arc (270° sweep)
- Calculated inductance (updating in real time)
- RPM sparkline (60-point rolling history)
- Layer and traverse position
- Fault message overlay (if FAULT state)

**SETUP mode** — Program configuration:
- 7-parameter list: turns, target L, former dia, AWG, layers, pitch, coil length
- Tap row to select, use button bar increment/decrement
- SEND button pushes all parameters to controller
- CALC button triggers CALC command, result shown as status

**CALC mode** — Wheeler calculator (standalone, no BT required):
- Diameter, turns, length entry via buttons
- Real-time L calculation in display
- X_L at 14 MHz shown
- Formula displayed for reference

**LOG mode** — Winding history:
- Last 20 completed windings
- Shows: turns, inductance, AWG, former diameter
- CLEAR button erases history (local to CYD)

### 7.3 Touch Zone Layout

```
┌──────────────────────────────────────┐
│ BT:OK  RF-CoilWinder         WIND   │ ← Header (20px)
├──────────────────────────────────────┤
│                                      │
│        (mode content)                │ ← Work area (162px)
│                                      │
├──────────────────────────────────────┤
│  WIND  │ SETUP │  CALC  │  LOG      │ ← Tab bar (28px)
├──────────────────────────────────────┤
│ BTN0  │ BTN1  │  BTN2  │ BTN3 │BTN4│ ← Button bar (28px)
└──────────────────────────────────────┘
  0     64    128    192   256  320px
```

Button bar by mode:

| Mode | BTN0 | BTN1 | BTN2 | BTN3 | BTN4 |
|------|------|------|------|------|------|
| WIND | START | PAUSE/RESUME | STOP | HOME | CLEAR FAULT |
| SETUP | ▲ INC | ▼ DEC | SEND | CALC | BACK |
| CALC | N+ | N− | D+ | D− | CALC |
| LOG | CLEAR | — | — | — | BACK |

---

## 8. CALIBRATION PROCEDURES

### 8.1 Traverse Home Calibration

**Procedure 8.1 — Traverse Home**

The traverse home position (zero) is set by the limit switch at the minimum carriage position.

1. Jog traverse manually to visual minimum position.
2. Adjust limit switch position so it just triggers at this point.
3. Send: `HOME`
4. Carriage moves slowly to limit switch. When triggered, step counter zeros.
5. Verify: `STATUS` shows `homed=true`, `trav_mm=0.0`.

### 8.2 Turn Sensor Calibration

**Procedure 8.2 — Verify Turn Counter**

1. Mark coil former at one point (tape flag).
2. Send: `HOME`. Verify `homed=true`.
3. Set `TURNS 10 AWG 22 DIA 50 LEN 50`.
4. Set `RPM 5` (very slow for hand counting).
5. Send `START`.
6. Count revolutions visually as mark passes sensor.
7. Machine should stop at exactly 10 turns ± 1.
8. If turn count is consistently off: verify `TURN_PULSES_PER_REV` setting in config.h.

### 8.3 Wire Tension Calibration

**Procedure 8.3 — Set Wire Tension**

Equipment: Small postal or fishing scale (0–500g range)

1. Thread wire through tensioner guide eye.
2. Attach scale to wire end.
3. Pull wire steadily while reading scale.
4. Target tension by gauge:
   - AWG 10–14: 200–500g (2–5N)
   - AWG 15–22: 50–150g (0.5–1.5N)
   - AWG 23–28: 20–50g (0.2–0.5N)
   - AWG 29–30: 10–30g (0.1–0.3N)
5. Adjust tension spring preload until target tension is reached.

> **NOTE**
> Tension calibration is a judgment call. Wind a test coil and inspect for:
> - Even spacing (correct tension)
> - Loose turns (too little tension)
> - Wire stretched or color changed (too much tension — especially magnet wire)
> - Wire breaks (far too much tension)

### 8.4 Pitch Factor Verification

**Procedure 8.4 — Verify Traverse Pitch**

1. Load AWG 22 magnet wire.
2. Set: `AWG 22 DIA 25.4 LEN 25.4 TURNS 36 PITCH 1.0`.
3. The 36 turns at pitch 1.0 (wire dia = 0.70mm) should fill: 36 × 0.70 = 25.2mm ≈ 25.4mm.
4. Wind the coil. Remove from chuck. Measure winding length with calipers.
5. Expected: 25–26mm. If > 27mm: pitch factor in config.h is too large. If < 23mm: too small.
6. Adjust `PITCH` setting proportionally: `PITCH = PITCH_commanded × (measured_mm / expected_mm)`.

---

## 9. OPERATING PROCEDURES

### 9.1 Standard Single-Layer Winding Procedure

**Procedure 9.1 — Wind a Single-Layer Inductor**

1. Load coil former in chuck (Procedure 4.5).
2. Load wire spool on tensioner (Section 4.7).
3. Set tension for wire gauge (Procedure 8.3).
4. Route wire through eyes to former. Wind 2–3 anchor turns by hand. Tape wire end.
5. Set winding program:
   ```
   TURNS N          (or use L µH to auto-calculate)
   AWG gauge
   DIA mm           (coil former OD)
   LEN mm           (desired winding length)
   PITCH 1.0        (close wound) or 1.5 (spaced)
   LAYERS 1
   RPM 30           (adjust per wire gauge)
   ```
6. Send: `HOME` (if not already homed).
7. Verify carriage is at winding start position (adjust manually if needed).
8. Send: `START`. Machine begins winding.
9. Monitor OLED or CYD for turn count and status.
10. When winding completes: OLED shows `DONE`, motor stops.
11. Secure final wire end with tape or solder to terminal.
12. Measure inductance with LCR meter or NanoVNA. Compare to predicted value.
13. Record winding: `PRESET SAVE N` (where N = slot 0–9) or allow SD auto-log.

### 9.2 Multi-Layer Winding Procedure

**Procedure 9.2 — Wind a Multi-Layer Inductor**

Multi-layer winding is used for:
- Compact high-inductance coils (>100 µH in small size)
- RF chokes (maximizing inductance per unit volume)
- Power inductors (many turns required)

1. Follow Procedure 9.1 steps 1–5, but set:
   ```
   LAYERS N          (number of layers, typically 2–5)
   PITCH 1.0         (close wound for compact result)
   ```
2. Calculate winding dimensions: each layer adds `wire_dia` to the coil radius.
   After N layers: coil OD = former_OD + 2 × N × wire_dia.
3. The machine automatically reverses traverse direction at each layer end.
4. Between layers: the machine continues winding without pause.
5. After winding: measure inductance. Multi-layer coils are more sensitive to layer build accuracy.

> **CAUTION**
> For high-voltage applications (tank coils, matching networks), multi-layer winding
> creates high interlayer voltage gradients. Insert Kapton tape (25 µm) between
> layers if operating at > 500V RF. For > 1 kV RF: use single-layer only.

### 9.3 Inductance-Target Mode

The machine can calculate the required turn count from a target inductance value:

1. Set former diameter and winding length:
   ```
   DIA 38.1          (1.5 inch former)
   LEN 50.8          (2 inch length)
   AWG 20
   ```
2. Set inductance target:
   ```
   L 47              (47 µH target)
   ```
3. Firmware calculates turns using Wheeler formula inversion.
4. Response: `OK: L=47.00uH → turns=XXX`
5. Verify: `CALC` should show L ≈ 47 µH.
6. Wind normally.

### 9.4 Foot Pedal Operation (Semi-Automatic)

The foot pedal provides start/stop/pause control without touching the controller:

- Press once (machine idle): starts winding
- Press once (machine winding): pauses winding
- Press again (machine paused): resumes winding
- E-stop button: halts immediately (different from foot pedal — does not resume)

### 9.5 Variable Pitch (Space-Wound) Coils

For maximum Q in resonant circuits, space-wound coils are preferred:

1. Set `PITCH 1.5` (turns separated by 0.5 wire diameters)
2. The traverse advance per revolution = 1.5 × wire_dia
3. Winding length for N turns: L = N × wire_dia × pitch_factor
4. Always recalculate L after changing pitch (Wheeler formula uses actual winding length).

### 9.6 Toroidal Winding (Manual Mode)

For toroidal formers, the machine assists with tension and counting but cannot automate the shuttle threading. Use manual mode:

1. Set `RPM 0` (zero speed)
2. Set `TURNS N` (target count)
3. Thread wire manually through core center on each pass
4. Press foot pedal once per complete pass through the core
5. Turn counter increments on each pedal press (if wired as single pulse per trigger)
6. Alternatively: use the encoder to manually count turns via the menu

> **NOTE**
> Toroidal winding with the machine is a hybrid process. The machine provides
> accurate turn counting and tension management, but threading the shuttle is manual.
> For large runs of identical toroids, consider a commercial toroid winder.

### 9.7 Preset Management

Preset slots 0–9 stored in ESP32 NVS (non-volatile):

```
PRESET SAVE 0       ← Save current program to slot 0
PRESET LOAD 0       ← Load slot 0 and apply
```

Each preset stores: turns, target_uh, former_dia, coil_len, AWG, pitch, layers, name.

On the CYD: SETUP mode → SEND sends all current parameters to controller.
On the OLED: menu PRESETS → Save Preset / Load Preset.

---

## 10. WINDING TABLES

### 10.1 Common HF Inductors (Air Core, Single Layer)

All values calculated with Wheeler's formula. Verify with LCR meter after winding.

#### Table 10.1-A: AWG 22 Magnet Wire, 1-inch (25.4mm) OD PVC Former

| L target | Turns | Length (close) | Pitch 1.5 | Q@7MHz (est) |
|---------|-------|----------------|-----------|--------------|
| 0.5 µH | 7 | 5mm | 7mm | 180 |
| 1.0 µH | 10 | 7mm | 10mm | 200 |
| 2.0 µH | 14 | 10mm | 14mm | 230 |
| 5.0 µH | 22 | 15mm | 22mm | 280 |
| 10 µH | 31 | 22mm | 32mm | 320 |
| 20 µH | 44 | 31mm | 46mm | 350 |
| 50 µH | 70 | 49mm | 73mm | 380 |

#### Table 10.1-B: AWG 18 Tinned Copper, 2-inch (50.8mm) OD Former

| L target | Turns | Length (close) | Pitch 1.5 | Q@7MHz (est) |
|---------|-------|----------------|-----------|--------------|
| 1.0 µH | 5 | 6mm | 8mm | 220 |
| 2.5 µH | 7 | 8mm | 12mm | 280 |
| 5.0 µH | 10 | 11mm | 17mm | 350 |
| 10 µH | 14 | 16mm | 24mm | 420 |
| 25 µH | 23 | 26mm | 39mm | 480 |
| 50 µH | 32 | 37mm | 55mm | 500 |
| 100 µH | 46 | 53mm | 80mm | 490 |

#### Table 10.1-C: AWG 14 Silver-Plated, 3-inch (76.2mm) OD Former (HF Tank Coil)

| L target | Turns | Length (close) | Pitch 1.5 | Q@14MHz (est) |
|---------|-------|----------------|-----------|---------------|
| 0.5 µH | 3 | 5mm | 7mm | 250 |
| 1.0 µH | 4 | 7mm | 10mm | 310 |
| 2.5 µH | 6 | 10mm | 15mm | 400 |
| 5.0 µH | 9 | 15mm | 22mm | 480 |
| 10 µH | 12 | 21mm | 31mm | 550 |
| 20 µH | 17 | 30mm | 45mm | 580 |

### 10.2 RF Chokes

RF chokes use closely wound coils on small formers. Close-winding (pitch = 1.0) maximizes inductance per unit length.

#### Table 10.2: RF Choke Values (AWG 26, 0.5-inch / 12.7mm OD, Close-Wound)

| L target | Turns | Length | f_SRF (est) | Application |
|---------|-------|--------|-------------|-------------|
| 1 µH | 16 | 9mm | 200 MHz | VHF bypass |
| 5 µH | 36 | 19mm | 70 MHz | UHF→HF isolation |
| 10 µH | 51 | 27mm | 45 MHz | HF TRX shielding |
| 25 µH | 81 | 43mm | 25 MHz | 10–160m bypass |
| 47 µH | 111 | 59mm | 18 MHz | 160m bypass |
| 100 µH | 163 | 86mm | 12 MHz | LF/MF bypass |

> **NOTE**
> RF chokes must have f_SRF > highest operating frequency for effective bypassing.
> At f_SRF, the choke looks like a resistor (high Z) — useful for notch filtering.
> Above f_SRF, choke is capacitive — usually undesirable.

### 10.3 Balun and Choke Transformers

#### Table 10.3: 1:1 Current Balun Windings (Coaxial Cable on FT-240-43 Core)

For the machine, wind the coax around a toroidal form. Count passes as turns.

| Band coverage | Turns | Wire | Core | Common-mode Z @3.5MHz |
|--------------|-------|------|------|----------------------|
| 160m only | 14 | RG-316 | FT-240-43 | 800 Ω |
| 80m–10m | 10 | RG-316 | FT-240-43 | 500 Ω |
| 40m–10m | 8 | RG-316 | FT-240-43 | 250 Ω |
| HF wideband | 12 | 14 AWG bifilar | FT-240-31 | 1200 Ω |

#### Table 10.4: Transmission Line Transformer (1:4 Unun, AWG 18 Bifilar)

Bifilar winding on T200-2 (powdered iron, red/clear):

| Turns | L (bifilar) | Z transform | Band |
|-------|------------|-------------|------|
| 14 | ~8 µH | 1:4 (50→200Ω) | 40m–10m |
| 18 | ~12 µH | 1:4 | 80m–10m |
| 24 | ~21 µH | 1:4 | 160m–10m |

### 10.4 Power Inductors

For switching power supplies and audio output transformers.

#### Table 10.5: Switching Power Supply Inductors (AWG 16, Ferrite Core, Close-Wound)

Note: All values approximate for air core. Ferrite core multiplies inductance by µ_r (typically 50–10000 for ferrite).

| Air core L | Turns | AWG | Former OD | DC resistance |
|-----------|-------|-----|-----------|---------------|
| 10 µH | 14 | 16 | 25.4mm | 14 mΩ |
| 50 µH | 31 | 16 | 25.4mm | 31 mΩ |
| 100 µH | 44 | 16 | 38.1mm | 38 mΩ |
| 500 µH | 100 | 18 | 38.1mm | 160 mΩ |

---

## 11. MAINTENANCE

### 11.1 Scheduled Maintenance

| Interval | Task |
|----------|------|
| Every 8 hours of winding | Clean wire guide eyes with IPA |
| Weekly | Lubricate rail rods (light machine oil) |
| Monthly | Check belt or coupler condition; check chuck jaw grub screws |
| Quarterly | Check leadscrew lubrication; verify calibration |
| Annually | Replace wire guide eyes if worn; verify motor current settings |

### 11.2 Rail Lubrication

**Procedure 11.1 — Lubricate Linear Rails**

1. Stop machine. Move carriage to center of travel.
2. Apply 2–3 drops light machine oil (Mobil Vactra No. 2 or equivalent) to each rail rod.
3. Manually traverse carriage full length 5 times.
4. Wipe excess oil from rails (excess oil attracts metal filings).
5. If using printed PTFE bushings (no LM12UU): no oil needed — PTFE is self-lubricating. Wipe rods clean of oil.

### 11.3 Leadscrew Lubrication

The T8 leadscrew requires light lubrication in the anti-backlash nut:

1. Apply PTFE or lithium grease to leadscrew threads (not oil — oil migrates).
2. Traverse full length 5 times to distribute.
3. Check anti-backlash spring tension: nut should have zero lash but not excessive drag. Adjust spring nut if needed.

### 11.4 Wire Guide Eye Replacement

Wire guide eyes wear over time, especially with fine magnet wire. Inspect surface for grooves.

1. Inspect bore of guide eye: no grooves > 0.1mm depth allowed.
2. If worn: remove eye insert from carriage arm.
3. Print new `wire_eye_insert()` from `wire_guide_carriage.scad`. Print at 0.1mm layers for smooth bore.
4. Press-fit new eye into arm bore.
5. Alternatively: install commercial ceramic eyelet (available from textile supply dealers, McMaster-Carr).

### 11.5 Chuck Maintenance

1. Monthly: check jaw grub screw torque. Retighten to 1.5 N·m if loose.
2. Inspect T-slot jaw tracks for wear or debris. Clear with compressed air.
3. Apply light grease (lithium-based) to jaw T-slot surfaces.
4. Check spindle bearing for play: grasp chuck and apply radial force. No perceptible play allowed (< 0.1mm).
5. If bearing worn: disassemble headstock and press out 6001ZZ bearing.

### 11.6 Motor Current Verification

Over time, TMC2208/A4988 Vref trimpots can drift. Verify annually:

1. Measure Vref on driver board with voltmeter.
2. NEMA 17 (1.2A Irms): Vref = 0.96V
3. NEMA 23 (1.8A Irms): Vref = 1.44V
4. Adjust if drifted more than ±0.1V.

---

## 12. TROUBLESHOOTING

### 12.1 Motor Problems

| Symptom | Probable Cause | Action |
|---------|---------------|--------|
| Motor doesn't turn | Enable pin not asserted | Check firmware: `enableOutputs()` called? |
| Motor hums, doesn't turn | Overcurrent protection | Reduce Vref 10% |
| Motor turns one direction only | DIR pin stuck | Check GPIO19/GPIO26 wiring |
| Motor skips steps | Current too low | Increase Vref; reduce speed |
| Motor overheats | Current too high | Reduce Vref; add heatsink |
| Traverse doesn't home | Limit switch not wiring | Check GPIO39, pullup |
| Traverse drifts after homing | Anti-backlash nut loose | Tighten nut spring preload |

### 12.2 Turn Counter Problems

| Symptom | Probable Cause | Action |
|---------|---------------|--------|
| No counts | Sensor not aligned | Align optical sensor with flag/magnet |
| Count drifts high | Multiple pulses per rev | Check sensor gap: should be 2–3mm |
| Count stops after N turns | ISR overflow | Check 32-bit counter: no overflow at 10,000 turns |
| Count is always even | Hall sensor oscillates | Add 0.1µF bypass cap at sensor Vcc |
| Inconsistent at high RPM | ISR latency | Reduce spindle speed; check ISR timing |

### 12.3 Traverse Problems

| Symptom | Probable Cause | Action |
|---------|---------------|--------|
| Traverse doesn't advance | rev_complete_flag never set | Check turn sensor |
| Advance too much | pitch_factor too high | Recalibrate Procedure 8.4 |
| Advance too little | pitch_factor too low | Recalibrate Procedure 8.4 |
| Wire piles up at end | No layer reversal | Set `LAYERS 1` or verify layer logic |
| Traverse binding | Rail misalignment | Check rail parallelism; lubricate |
| Backlash in traverse | Anti-backlash spring worn | Replace nut spring or nut assembly |

### 12.4 Wire Break Faults

| Symptom | Probable Cause | Action |
|---------|---------------|--------|
| Immediate FAULT on start | Sensor wired backwards | Check wire break sensor polarity |
| No fault on actual wire break | Sensor too slow | Reduce `WIRE_BREAK_DEBOUNCE_MS` |
| Random faults | Vibration triggering sensor | Increase debounce; check sensor mount |
| Wire keeps breaking | Tension too high | Reduce spring preload (Procedure 8.3) |
| Wire breaks at guide eye | Eye worn | Replace guide eye (Procedure 11.4) |

### 12.5 Inductance Errors

| Symptom | Probable Cause | Action |
|---------|---------------|--------|
| Measured L < predicted | Fewer turns than counted | Verify turn sensor alignment |
| Measured L > predicted | Wire diameter larger than spec | Use AWG spec from wire supplier |
| L off by fixed percentage | Wrong wire diameter in config | Update wire table in winder.h |
| L off on long coils | Pitch factor error | Recalibrate Procedure 8.4 |
| L varies between units | Winding inconsistency | Verify tension, check for loose turns |

### 12.6 BT/CYD Connection Problems

| Symptom | Action |
|---------|--------|
| CYD shows BT:-- | Verify controller is advertising ("RF-CoilWinder" in BT scan) |
| CYD connects but no data | Check UART2 wiring; verify JSON broadcast |
| CYD data is stale | Check BT_STATUS_INTERVAL_MS in config.h (should be 500ms) |
| CYD shows "RF-CoilWinder" not found | Power cycle both devices; BT stack reset |

---

## APPENDIX A: SPECIFICATIONS

### A.1 Mechanical Specifications

| Parameter | Value | Units |
|-----------|-------|-------|
| Machine base dimensions | 400 × 180 × 22 | mm |
| Chuck capacity | 6.35–102 | mm OD |
| Maximum coil length | 310 | mm |
| Spindle motor | NEMA 23, 200 steps/rev, 2.5A | — |
| Traverse motor | NEMA 17, 200 steps/rev, 1.7A | — |
| Spindle microstepping | 1/16 (3200 steps/rev) | — |
| Traverse microstepping | 1/16 (3200 steps/rev) | — |
| Traverse resolution | 1/1600 mm (0.625 µm) | mm/step |
| Linear travel | 310 | mm |
| Wire gauge range | AWG 10–30 | — |
| Maximum spindle speed | 150 | RPM |

### A.2 Electrical Specifications

| Parameter | Value | Units |
|-----------|-------|-------|
| Supply voltage | 24 | VDC |
| Supply current (max) | 5 | A |
| Controller | ESP32-WROOM-32 | — |
| Stepper drivers | TMC2208 or A4988 × 2 | — |
| Turn sensor | Optical or Hall, 1 pulse/rev | — |
| Position sensor | Limit switch (traverse home) | — |
| Data log | SD card (FAT32) | — |
| Serial port | 115200 baud, USB | — |
| Bluetooth | SPP (Serial Port Profile) | — |
| Display (local) | SSD1306 OLED 128×64 | — |
| Display (remote) | CYD ESP32-2432S028 | — |
| Preset storage | 10 slots, NVS | — |

---

## APPENDIX B: BILL OF MATERIALS

### B.1 Mechanical

| Item | Specification | Qty | Source |
|------|---------------|-----|--------|
| Smooth rod | 12mm OD × 400mm | 2 | AliExpress, hardware |
| T8 leadscrew | 8mm × 400mm, 2mm pitch | 1 | AliExpress |
| Anti-backlash nut | T8 × 2, spring-loaded | 1 | AliExpress |
| LM12UU bearings | 12mm bore, 21mm OD | 4 | AliExpress or VXB |
| 6001ZZ bearing | 12mm × 28mm × 8mm | 2 | VXB or local |
| F608ZZ bearing | 8mm bore, flanged | 2 | AliExpress |
| Jaw coupler | 14mm × 14mm | 1 | AliExpress |
| Motor coupler | 5mm × 8mm flexible | 1 | AliExpress |
| Spindle shaft | 14mm OD × 200mm SS | 1 | McMaster or lathe |
| Rubber feet | M4 bumper, 10mm dia | 8 | McMaster |
| PETG filament | 1.75mm, 2 kg | 2 | Prusament or eSun |
| M3, M4, M5 hardware | Assorted | — | Hardware store |
| Rail end collars | 12mm shaft collar | 4 | AliExpress |

### B.2 Electronics

| Item | Part Number / Description | Qty | Source |
|------|---------------------------|-----|--------|
| ESP32 | ESP32-WROOM-32 DevKit V1 | 1 | AliExpress, Mouser |
| Stepper driver | TMC2208 StepStick | 2 | AliExpress |
| Spindle motor | NEMA 23, 2.5A, 1.8° | 1 | StepperOnline 23HS22-2804S |
| Traverse motor | NEMA 17, 1.7A, 1.8° | 1 | StepperOnline 17HS19-1684S |
| CYD | ESP32-2432S028 | 1 (opt) | AliExpress |
| OLED display | SSD1306 128×64 I2C | 1 | AliExpress |
| Rotary encoder | KY-040 or EC11 | 1 | AliExpress |
| Turn sensor | AH1806 Hall or ITR9606 optical | 1 | AliExpress |
| Wire break sensor | ITR9606 slotted optical | 1 | AliExpress |
| Limit switch | SS-5GL micro-switch | 1 | Omron or AliExpress |
| Foot pedal | SPDT NO pedal switch | 1 | AliExpress |
| Speed pot | 10kΩ panel mount B-taper | 1 | Bourns PTV09 |
| E-stop button | 22mm mushroom red | 1 | Schneider or AliExpress |
| PSU | 24V 5A (120W) | 1 | Mean Well LRS-100-24 |
| SD card module | SPI, 3.3V | 1 | AliExpress |
| PCB | Fabricate from design | 1 | JLCPCB |

### B.3 Small Parts

| Item | Specification | Qty |
|------|---------------|-----|
| Neodymium magnet | 6mm × 3mm disc | 2 |
| Tension spring | 10mm OD × 40mm, 0.5N/mm | 1 |
| Wire eye insert | Commercial ceramic, 1.5mm bore | 2 |
| Wire eye insert | Commercial ceramic, 3mm bore | 2 |
| Spool axle cap | Print from spool_axle() | 1 |

---

## APPENDIX C: WHEELER FORMULA REFERENCE

### C.1 Single-Layer Coil

```
L(µH) = (d² × N²) / (18d + 40l)     [d, l in inches]
```

Solve for N given L, d, l:
```
N = √(L × (18d + 40l) / d²)
```

Accuracy: ±1% for d/l between 0.1 and 10 (typical solenoid range).

### C.2 Multi-Layer Coil

```
L(µH) = (0.8 × a² × N²) / (6a + 9l + 10c)     [a, l, c in inches]
```

Where:
- a = mean radius = r_inner + c/2
- c = winding radial build = layers × wire_diameter

### C.3 Toroid

```
L(µH) = (A_L × N²) / 1000     [A_L in mH/1000T²]
```

Common core A_L values (check manufacturer datasheet):
- Fair-Rite 5943: 6 nH/T²
- Fair-Rite 5977: 80 nH/T²
- Amidon T200-2: 120 nH/T² (red/clear)
- Amidon T200-6: 173 nH/T² (yellow/white)

### C.4 Quick Sizing Chart

| L target | 1" dia, 1" long | 2" dia, 2" long | 3" dia, 3" long |
|---------|----------------|----------------|----------------|
| 1 µH | 10T | 5T | 3T |
| 5 µH | 22T | 10T | 7T |
| 10 µH | 32T | 15T | 10T |
| 50 µH | 71T | 33T | 22T |
| 100 µH | 100T | 47T | 31T |
| 500 µH | 225T | 105T | 70T |

---

## APPENDIX D: WIRE REFERENCE TABLES

### D.1 Bare Conductor Dimensions and Resistance

| AWG | Dia (mm) | Area (mm²) | Ω/m (DC, 20°C) | Max A (chassis) |
|-----|---------|-----------|----------------|-----------------|
| 10 | 2.588 | 5.261 | 0.00328 | 30.0 |
| 12 | 2.053 | 3.309 | 0.00521 | 20.0 |
| 14 | 1.628 | 2.081 | 0.00828 | 13.0 |
| 16 | 1.291 | 1.309 | 0.01316 | 8.0 |
| 18 | 1.024 | 0.823 | 0.02090 | 5.0 |
| 20 | 0.812 | 0.518 | 0.03320 | 3.0 |
| 22 | 0.644 | 0.326 | 0.05279 | 2.0 |
| 24 | 0.511 | 0.205 | 0.08394 | 1.2 |
| 26 | 0.405 | 0.129 | 0.13341 | 0.8 |
| 28 | 0.321 | 0.081 | 0.21200 | 0.5 |
| 30 | 0.255 | 0.051 | 0.33680 | 0.3 |

### D.2 Magnet Wire (Single Build) Insulated Diameter

| AWG | Bare dia (mm) | Single build OD (mm) | Heavy build OD (mm) |
|-----|--------------|---------------------|---------------------|
| 14 | 1.628 | 1.69 | 1.75 |
| 16 | 1.291 | 1.35 | 1.41 |
| 18 | 1.024 | 1.08 | 1.14 |
| 20 | 0.812 | 0.87 | 0.93 |
| 22 | 0.644 | 0.70 | 0.75 |
| 24 | 0.511 | 0.57 | 0.62 |
| 26 | 0.405 | 0.46 | 0.51 |
| 28 | 0.321 | 0.37 | 0.42 |
| 30 | 0.255 | 0.30 | 0.34 |

### D.3 Skin Effect — Effective Resistance vs Frequency

RF resistance multiplier = √(f_MHz / reference):

| AWG | DC R (Ω/m) | ×3.5MHz | ×7MHz | ×14MHz | ×21MHz | ×28MHz |
|-----|-----------|---------|-------|--------|--------|--------|
| 14 | 0.00828 | 0.041 | 0.058 | 0.082 | 0.101 | 0.117 |
| 18 | 0.02090 | 0.104 | 0.147 | 0.208 | 0.255 | 0.294 |
| 22 | 0.05279 | 0.263 | 0.372 | 0.526 | 0.644 | 0.743 |
| 26 | 0.13341 | 0.665 | 0.940 | 1.33 | 1.63 | 1.88 |

Values calculated as: R_ac = R_dc × √(f_MHz × 20)

### D.4 Maximum Turns at 100% Packing per Winding Length (per inch of length)

| AWG | Turns/inch (close-wound) | Notes |
|-----|------------------------|-------|
| 10 | 9.5 | AWG 10 = 2.65mm insulated |
| 12 | 12.0 | |
| 14 | 14.8 | |
| 16 | 18.5 | |
| 18 | 23.1 | |
| 20 | 28.7 | |
| 22 | 36.3 | |
| 24 | 44.2 | |
| 26 | 55.2 | |
| 28 | 68.7 | |
| 30 | 84.7 | |

---

*End of Document — TM-CWND-001 Rev A*

*For corrections or additions, contact: d.ecaudata@gmail.com*
