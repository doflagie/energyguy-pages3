# TECHNICAL MANUAL
## RF HYBRID COUPLERS AND POWER DIVIDERS
### Model TM-HYB-001 &mdash; Revision A
### All Amateur Radio Bands, 160M &ndash; 23cm

---

## LIST OF EFFECTIVE PAGES

| Chapter/Section | Topic |
|---|---|
| Chapter 1 | General Information |
| Chapter 2 | Technical Description |
| Chapter 3 | Operating Instructions |
| Chapter 4 | Phase and Amplitude Balance Measurement |
| Chapter 5 | Construction Procedures |
| Chapter 6 | EZNEC/NEC Phased Array Modeling |
| Chapter 7 | Maintenance |
| Chapter 8 | Troubleshooting |
| Chapter 9 | Parts List |
| Appendix A | Complete Component Value Tables |
| Appendix B | Transmission Line Length Tables |
| Appendix C | NEC Model Quick Reference |
| Appendix D | Weatherproofing and Field Installation |
| Appendix E | Glossary |

---

## CHAPTER 1 — GENERAL INFORMATION

### 1.1 Scope

This manual provides design, construction, operating, and test instructions for
the TM-HYB-001 family of RF hybrid couplers and power dividers.  Designs cover
all amateur radio bands from 160 meters through 23 centimeters (1.85 MHz
through 1296 MHz) and are intended for phased array feed networks, circular
polarization feeds, antenna switching, and power combining/splitting
applications.

The following designs are documented:

| Designator | Type | Coverage |
|---|---|---|
| TM-HYB-001A | 2-way Wilkinson power divider | 160M–23cm |
| TM-HYB-001B | 4-way Wilkinson power divider | 40M–23cm |
| TM-HYB-001C | 90° quadrature branch-line hybrid | 160M–23cm |
| TM-HYB-001D | 180° rat-race (ring) hybrid | HF/VHF/UHF |
| TM-HYB-001E | Transmission line transformers (TLT) | 1.5–50 MHz |
| TM-HYB-001F | Lumped-element HF hybrids | 160M–6M |

### 1.2 Specifications Summary

| Parameter | Wilkinson (A/B) | Quadrature 90° (C) | Rat-Race 180° (D) |
|---|---|---|---|
| Insertion loss | 3.0 dB (2-way) | 3.0 dB | 3.0 dB |
| Return loss | >20 dB | >18 dB | >18 dB |
| Isolation | >20 dB | >18 dB | >20 dB |
| Phase difference | 0° (in-phase) | 90° ± 5° | 0°/180° ± 5° |
| Amplitude balance | <0.3 dB | <0.3 dB | <0.5 dB |
| Bandwidth (−1 dB) | 1.5:1 | 1.4:1 | 1.8:1 |
| Max power (HF) | 100 W (lumped) | 50 W (lumped) | 50 W |
| Connectors | BNC or SMA | BNC or SMA | SMA |
| Enclosure | Weatherproof ASA | Weatherproof ASA | Weatherproof ASA |

### 1.3 Definitions

**Hybrid coupler:** A 4-port network that divides power between two output ports
with a controlled phase difference.

**Power divider:** A 3-port network that divides power between two or more output
ports with controlled amplitude ratio and phase relationship.

**Isolation:** The attenuation between two output ports of a divider.  High
isolation means a mismatch at one port does not affect the other.

**Phase balance:** The difference in electrical phase between two output ports.
For a quadrature hybrid, the target is 90°.  For a Wilkinson, the target is 0°.

**Amplitude balance:** The difference in signal level (dB) between two output
ports.  Target: 0 dB (equal split).

**Return loss:** 20 × log₁₀(|Γ|) in dB.  A well-matched port shows >20 dB
return loss (|Γ| < 0.1, SWR < 1.22:1).

---

## CHAPTER 2 — TECHNICAL DESCRIPTION

### 2.1 Wilkinson Power Divider Theory

The Wilkinson power divider (1960) achieves simultaneous port matching and
output isolation through two λ/4 transmission line sections and a bridging
resistor.

**Design equations (2-way, equal split):**

```
Quarter-wave arm impedance: Z_λ/4 = Z0 × √2 = 70.71 Ω   (for Z0 = 50 Ω)
Isolation resistor:          R_iso = 2 × Z0 = 100 Ω
Split ratio:                  −3.01 dB (equal)
```

**S-matrix (ideal, at design frequency):**

```
       [  0      -j/√2   -j/√2 ]
[S] =  [-j/√2    0        0   ]
       [-j/√2    0        0   ]
```

Port 1 is the input; Ports 2 and 3 are the equal-amplitude, in-phase outputs.
The isolation resistor (100 Ω between ports 2 and 3) only dissipates power when
ports 2 and 3 are presented with unequal loads.  Under matched conditions, it
carries no current.

**Bandwidth:** The Wilkinson's 3 dB bandwidth is approximately 70% to 150% of
the design frequency (2.1:1 ratio) for <1 dB amplitude ripple.  Multi-section
designs extend this to 4:1 or greater.

**Lumped-element substitute for λ/4 section** (low-pass T-network):

```
               L/2            L/2
IN ────┤▬▬▬├──────┬──────┤▬▬▬├──── OUT
                 [C]
                  │
                 GND

L = Z_arm / (2π × f₀)       [Henrys, Z in Ohms, f in Hz]
C = 1 / (2π × f₀ × Z_arm)   [Farads]
```

This single-section approximation gives ±5° phase error and ±0.5 dB amplitude
error over approximately ±40% of center frequency.

### 2.2 Quadrature (90°) Hybrid — Branch-Line Coupler

The branch-line coupler consists of four λ/4 sections arranged in a square,
with ports at each corner.

**Port definitions:**

| Port | Function | Phase at design f |
|---|---|---|
| 1 | Input | 0° (reference) |
| 2 | Through | −90° |
| 3 | Coupled | −180° (or −90° relative to P2) |
| 4 | Isolated | No output |

**Standard convention:** Phase of Port 3 lags Port 2 by 90°.

**Design equations:**

```
Shunt arm (vertical) impedance:  Z_sh = Z0 = 50 Ω
Series arm (horizontal) impedance: Z_sr = Z0 / √2 = 35.35 Ω
All arms: λ/4 at design frequency
```

**S-matrix (ideal):**

```
       [  0    -j/√2   -1/√2   0   ]
[S] =  [-j/√2   0       0    -1/√2 ]
       [-1/√2   0       0     j/√2 ]
       [  0    -1/√2   j/√2    0   ]
```

The 90° phase difference between Port 2 (Through) and Port 3 (Coupled) makes
this network ideal for:

- **Circular polarization feeds:** Connect Port 2 to one linear element and
  Port 3 to the orthogonal element.  Equal amplitudes with 90° phase difference
  produce circular polarization.
- **Phased array endfire elements:** Two λ/4-spaced elements with 90° phase
  shift produce a cardioid (unidirectional) pattern with ≈5 dB F/B ratio.
- **I/Q mixing:** The 90° channel is the I path; the 0° channel is the Q path.

### 2.3 Rat-Race (180°) Hybrid

The rat-race (ring) coupler consists of a single transmission line ring of
3λ/2 circumference, with four ports connected at λ/4 intervals.

**Key dimensions:**

```
Ring characteristic impedance: Z_ring = Z0 × √2 = 70.71 Ω
Total ring length:              3λ/2
Port spacing:  P1→P3 = λ/4,  P3→P2 = λ/4,  P2→P4 = λ/4,  P4→P1 = 3λ/4
```

**Operation:**

| Drive Port | Output P3 | Output P4 | Isolated |
|---|---|---|---|
| P1 (Σ) | −3 dB, 0° | −3 dB, 0° | P2 (Δ) |
| P2 (Δ) | −3 dB, 0° | −3 dB, 180° | P1 (Σ) |

**Phased array application (2-element):**

- Drive **P1 (Σ):** both antennas in-phase → broadside radiation
- Drive **P2 (Δ):** antennas 180° out of phase → endfire radiation (null at
  broadside, two endfire lobes)
- Switch between P1 and P2 for switchable pattern diversity

**Monopulse DF (direction finding):**

- Connect two antennas to P3 and P4
- P1 (Σ): maximum signal in boresight direction → tracking
- P2 (Δ): null at boresight, proportional to angular error → steering

### 2.4 Transmission Line Transformers (TLT)

TLTs exploit the transmission-line mode (as opposed to transformer flux) for
bandwidth.  The transmission line passes differential-mode signals unchanged
while the core suppresses common-mode (choking action).

**Key principle:** The core does not transform voltage or current.  It provides
high impedance to common-mode currents, forcing all current to flow as
differential mode through the line.  This gives decade-wide bandwidth.

**Guanella balun (1:1):** A single coaxial or bifilar pair wound on a ferrite
core.  Converts unbalanced (coax) to balanced (dipole/balanced line) feed.
Common-mode impedance: >500 Ω across 1–30 MHz with 6 turns on BN-43-202.

**Guanella 4:1 balun:** Two 1:1 sections in series at balanced (200 Ω) end,
in parallel at unbalanced (50 Ω) end.  Voltage across each line = V/2;
current doubles; impedance ratio = 4:1.

**Ruthroff 4:1 unun:** Voltage-addition type.  Input voltage appears at both
primary and secondary ends; output = 2V, output impedance = Z_in/4.  Narrower
bandwidth than Guanella (limited by propagation delay through line).

### 2.5 Circular Polarization Feed

For a phased array or single antenna requiring circular polarization:

```
             ┌─────────────────────────────────────┐
    RF in ───┤  90° Hybrid Coupler (Branch-Line)   │
             │   Port 1 → Input                     │
             │   Port 2 → Element A (0°)             ├──→ Element A (horizontal)
             │   Port 3 → Element B (−90°)           ├──→ Element B (vertical)
             │   Port 4 → 50Ω load (isolated)       │
             └─────────────────────────────────────┘

RHCP (Right-Hand Circular):  Element A=0°,  Element B=−90°
LHCP (Left-Hand Circular):   Element A=0°,  Element B=+90°  (swap P2/P3)
```

**Axial ratio** (quality of circular polarization):

```
AR (dB) = 20 × log₁₀ [ tan(45° + ε_phase/2) ]    (ε_phase = phase error in degrees)
AR (dB) ≈ 20 × log₁₀ [ (1 + δ_amp) / (1 − δ_amp) ]   (δ_amp = amplitude error ratio)
```

For AR < 1.5 dB: phase error < 5°, amplitude error < 0.5 dB.
For AR < 3 dB: phase error < 10°, amplitude error < 1 dB.

---

## CHAPTER 3 — OPERATING INSTRUCTIONS

### 3.1 Initial Setup and Connections

> **WARNING:** Do not connect any port to a transmitter without first verifying
> the hybrid is terminated in 50 Ω at all ports.  An open or short at any port
> will appear as a partial mismatch at the input and may damage the transmitter
> or the hybrid's resistors.

> **CAUTION:** Never exceed the power rating of the isolation resistors.
> At HF (100 W input), use 2 W resistors.  At VHF/UHF, 0.5 W SMD resistors
> are adequate for up to 25 W.

**Initial setup procedure:**

1. Terminate all output ports in matched 50 Ω loads (SMA or BNC terminations).
2. With an RF source (QRP transmitter at low power, or signal generator), apply
   a signal to Port 1 (input).
3. Measure signal level at each output port with a power meter or calibrated
   receiver.
4. Verify amplitude balance: outputs should be equal within 0.5 dB.
5. Proceed to Chapter 4 for phase verification.

### 3.2 Wilkinson Divider — Operation

**Power dividing (1 to 2):**
- Port 1: RF input
- Ports 2 and 3: equal-power outputs; load both with 50 Ω antennas or stages
- Each output receives −3 dB (half the input power)

**Power combining (2 to 1):**
- Ports 2 and 3: input signals (must be coherent — same frequency and phase)
- Port 1: combined output (−3 dB per input port)
- The isolation resistor absorbs any power difference between inputs

> **NOTE:** For power combining, the two input signals must be coherent
> (in-phase, within ±5°).  If signals are out of phase, the isolation
> resistor absorbs the mismatch power.  At 180° phase difference, all
> power is absorbed by the resistor and no output appears at Port 1.

### 3.3 Quadrature Hybrid — Operation

**Circular polarization feed:**
1. Connect Port 1 to the RF source (transceiver or power amplifier).
2. Connect Port 2 (0° output) to one linear polarization element.
3. Connect Port 3 (−90° output) to the orthogonal element.
4. Connect Port 4 to a 50 Ω termination.
5. Verify axial ratio per Chapter 4 procedure.

**Switchable polarization:**
- RHCP: Port 2 → Horizontal element, Port 3 → Vertical element
- LHCP: Port 3 → Horizontal element, Port 2 → Vertical element
- Use a DPDT relay or manual switch to swap P2/P3 connections

**2-element endfire phased array:**
1. Connect Port 1 to RF source.
2. Port 2 (0°): Element 1 (rear element for +X endfire).
3. Port 3 (−90°): Element 2 (front element, λ/4 spacing toward +X).
4. Port 4: 50 Ω load.
5. Endfire direction: toward Element 2 (+X axis).

### 3.4 Rat-Race Hybrid — Operation

**2-element phased array (switchable broadside/endfire):**
1. Antennas connected to Ports 3 and 4 (the two driven ports).
2. Drive Port 1 (Σ): Both antennas in-phase → broadside radiation.
3. Drive Port 2 (Δ): Antennas 180° out-of-phase → endfire radiation.
4. Terminate unused port (Σ or Δ) in 50 Ω when driving the other.

**Direction-finding (monopulse):**
1. Antennas to Ports 3 and 4.
2. Port 1 (Σ) → receiver channel A: sum signal.
3. Port 2 (Δ) → receiver channel B: difference signal.
4. Bearing: signal is maximum on channel A when beam is on target.
5. Null on channel B confirms boresight.

### 3.5 Band Switching (HF Lumped-Element Version)

The band-switched lumped-element Wilkinson has dual rotary switches (SW1, SW2)
on the lid for selecting the inductor and capacitor values.

**Procedure:**
1. Identify operating band.
2. Set SW1 (inductor switch) to the position labeled for that band.
3. Set SW2 (capacitor switch) to the matching position.

> **NOTE:** Both switches must be on the same position (same band) at all
> times.  Operating with mismatched switch positions will result in
> incorrect phase and amplitude, potentially damaging connected equipment.

4. Verify match with antenna analyzer before applying transmit power.

---

## CHAPTER 4 — PHASE AND AMPLITUDE BALANCE MEASUREMENT

### 4.1 Overview

Phase and amplitude balance measurement verifies that the hybrid coupler is
performing to specification.  Two methods are described:

- **Method A** — Using the CYD monitor (firmware/hybrid_monitor_cyd.ino)
- **Method B** — Using general-purpose instruments (VNA, oscilloscope,
  spectrum analyzer)

### 4.2 Method A — CYD Hybrid Monitor

The CYD firmware measures amplitude balance using two AD8307 log detectors
and phase balance using zero-crossing time measurement.

**Required hardware:**
- CYD (ESP32-2432S028) with hybrid_monitor_cyd.ino firmware loaded
- Two AD8307 log detector modules (one per output port)
- Signal source at band center frequency, −20 to 0 dBm

**Connection:**

```
  Signal source ──── Port 1 (input) of hybrid under test

  Port 2 output ──── AD8307 module #1 ──── CYD GPIO36 (ADC1)
  Port 3 output ──── AD8307 module #2 ──── CYD GPIO39 (ADC1)

  Port 2 output ──── (after resistive divider) ──── CYD GPIO34 (zero-crossing)
  Port 3 output ──── (after resistive divider) ──── CYD GPIO35 (zero-crossing)

  Isolated port (Port 4) ──── 50 Ω termination
```

**Procedure:**

1. Flash hybrid_monitor_cyd.ino to CYD.
2. Connect hardware as shown above.
3. Apply power.  CYD startup screen shows "HYB-001 Monitor".
4. Touch **BAND** tab.  Select band under test and hybrid type (WILKINSON /
   QUADRATURE 90° / RAT-RACE 180°).
5. Touch **LEVEL** tab.  Verify both AD8307 readings appear.  Amplitude
   balance (ΔAmp) is displayed in dB.
6. Touch **PHASE** tab.  Verify phase angle is displayed.  Phase error vs.
   target is shown in green (PASS) or red (FAIL).
7. Record readings.  Touch **HIST** tab to view statistics after multiple
   measurements.
8. Touch **SPEC** tab to configure pass/fail limits and view GO/NO-GO result.

**Expected readings (at design frequency):**

| Mode | ΔAmp (dB) | Phase |
|---|---|---|
| Wilkinson | < 0.3 | 0° ± 2° |
| Quadrature | < 0.3 | −90° ± 5° |
| Rat-Race | < 0.5 | 0°/180° ± 5° |

### 4.3 Method B — VNA or Network Analyzer

A 2-port VNA (such as NanoVNA or miniVNA Tiny) can measure phase and amplitude
at each port relative to Port 1.

**Procedure (2-port VNA, S21 measurement):**

1. Calibrate VNA (SOL at Port 1, Open at Port 2).
2. Connect VNA Port 1 to hybrid input (Port 1 of DUT).
3. Terminate hybrid's isolated port (Port 4 for quadrature hybrid) in 50 Ω.
4. Connect VNA Port 2 to hybrid Port 2 (through output).
5. Sweep frequency through design band.
6. Record: |S21| (amplitude) and ∠S21 (phase) at design frequency.
7. Move VNA Port 2 to hybrid Port 3 (coupled output).
8. Record: |S21| and ∠S21.
9. Amplitude balance = |S21 at P2| − |S21 at P3| in dB.
10. Phase balance = ∠S21 at P2 − ∠S21 at P3 in degrees.

**Expected S21 values:**

| Network | |S21| at P2 | |S21| at P3 | ∠S21 at P2 | ∠S21 at P3 | Phase diff |
|---|---|---|---|---|---|---|
| Wilkinson 2-way | −3.0 dB | −3.0 dB | −90° | −90° | 0° |
| Quadrature hybrid | −3.0 dB | −3.0 dB | −90° | −180° | −90° |
| Rat-race (Σ port) | −3.0 dB | −3.0 dB | ~−90° | ~−90° | ~0° |
| Rat-race (Δ port) | −3.0 dB | −3.0 dB | ~−90° | ~−270° | ~−180° |

> **NOTE:** Absolute phase values depend on measurement cable lengths and
> calibration.  Only the **relative** phase difference between ports is
> meaningful in these measurements.

### 4.4 Method C — Oscilloscope (Two-Channel)

For frequencies below 50 MHz, a dual-channel oscilloscope can directly
display the phase difference.

**Procedure:**

1. Apply a 1 Vpeak sine wave at band center frequency to hybrid Port 1.
2. Connect oscilloscope Channel 1 to Port 2; Channel 2 to Port 3.
3. Terminate all unused ports in 50 Ω.
4. Set oscilloscope to display both channels at same scale.
5. Verify amplitude balance: channels should be equal amplitude.
6. Use oscilloscope cursors to measure time difference between zero crossings
   of Channel 1 and Channel 2.
7. Phase difference = (Δt / T) × 360° where T = 1/f₀.

**Example (40M, 7.150 MHz, 90° hybrid):**
```
  T = 1 / 7.150 MHz = 139.9 ns
  Expected Δt (90°): 139.9 / 4 = 35.0 ns
  Acceptable range: 31.4–38.6 ns (±5°)
```

### 4.5 Return Loss (Port Match) Measurement

> **IMPORTANT:** Return loss must be measured with all other ports terminated.
> An unterminated port causes reflections that make the measurement invalid.

**Using NanoVNA (S11 measurement):**

1. Calibrate NanoVNA with SOLT (Short-Open-Load-Through).
2. Terminate hybrid Ports 2, 3 (and 4 if applicable) in 50 Ω loads.
3. Connect NanoVNA CH0 to hybrid Port 1.
4. Measure S11 (return loss) across frequency band.
5. Acceptance criterion: S11 < −20 dB at design frequency.

**Using directional coupler bridge:**
See Chapter 3 of TM-FC-001 (frequency counter manual) for SWR bridge
construction.  Place bridge between source and hybrid Port 1.  Null indicates
perfect match (SWR = 1.0:1).

### 4.6 Isolation Measurement

**Procedure:**

1. Connect signal source to Port 2.
2. Terminate Port 1 and Port 4 (if applicable) in 50 Ω.
3. Measure power at Port 3 (the isolated port).
4. Isolation (dB) = P_in(dBm) − P_at_P3(dBm).
5. Acceptance criterion: Isolation > 20 dB.

**Typical results:**
- At design frequency: 25–35 dB (excellent)
- At ±20% of design frequency: 15–20 dB (adequate)
- At 2× design frequency: 8–12 dB (significant leakage — use LP filter)

### 4.7 Phase and Amplitude Balance Record Sheet

Copy this table for field measurements:

```
TM-HYB-001 BALANCE MEASUREMENT LOG

Date: ____________  Operator: ____________  Band: ____________
Hybrid type: [ ] Wilkinson  [ ] Quadrature  [ ] Rat-race
Frequency: ____________ MHz  Signal level at Port 1: ________ dBm

AMPLITUDE BALANCE:
  Port 2 level: ________ dBm
  Port 3 level: ________ dBm
  ΔAmp (P2−P3): ________ dB   Spec: ±0.3 dB   [ ] PASS  [ ] FAIL

PHASE BALANCE:
  Phase at Port 2: ________ °
  Phase at Port 3: ________ °
  Phase difference: ________ °   Target: ________ °
  Phase error: ________ °        Spec: ±5°       [ ] PASS  [ ] FAIL

RETURN LOSS:
  Port 1 S11: ________ dB   Spec: <−20 dB   [ ] PASS  [ ] FAIL
  Port 2 S11: ________ dB                   [ ] PASS  [ ] FAIL
  Port 3 S11: ________ dB                   [ ] PASS  [ ] FAIL

ISOLATION:
  Port 2 → Port 3: ________ dB   Spec: >20 dB   [ ] PASS  [ ] FAIL

REMARKS: _______________________________________________
```

---

## CHAPTER 5 — CONSTRUCTION PROCEDURES

### 5.1 General Construction Guidelines

**Component quality:**
- Use NP0/C0G capacitors for all RF applications.  X7R and Y5V dielectrics
  have value shifts with temperature and voltage that degrade performance.
- Use ±5% capacitors as minimum tolerance.  ±1% for VHF/UHF.
- Wind all inductors on cores of correct permeability for the band.
  Measure finished inductors with an LC meter before installation.
- Use E96 resistors (1%) for isolation resistors.  0.5 W for up to 25 W
  RF power; 2 W for up to 100 W.

**PCB construction:**
- HF (through 30 MHz): Perfboard or Manhattan-style construction is adequate.
  Keep leads as short as possible.  Star-ground at circuit center.
- VHF (50–200 MHz): Use double-sided FR-4.  Continuous ground plane on bottom.
  Short component leads (< 5 mm).
- UHF (200 MHz+): Microstrip PCB construction required.  Use 50 Ω trace width
  on FR-4 (h=1.6 mm): w ≈ 2.9 mm for 50 Ω.  Controlled impedance board
  recommended for 23 cm and above.

### 5.2 Toroid Winding Procedure (HF Inductors)

1. Measure core OD.  Use appropriate wire gauge (see parts list).
2. Thread wire through core hole.  First pass through counts as one turn.
3. Wind evenly distributed around circumference (not bunched).
4. Leave 30 mm leads at each end.
5. Measure inductance with LC meter after winding.
6. Adjust turns as needed:
   - Too high by X%: Remove √X% of turns (inductance ∝ N²).
   - Too low by X%: Add √X% of turns.
7. Secure winding with one drop of nail varnish or varnish at two points.
8. Trim leads to 10 mm.

**Target tolerance: ±3% of design value.**

### 5.3 Coaxial λ/4 Section Construction (VHF/UHF)

1. Calculate physical length:
   ```
   length_mm = (c × VF / (4 × f)) × 1000
   ```
   Where VF = velocity factor (0.66 for RG-58/RG-59; 0.70 for PTFE; 1.0 for
   air-spaced line).

2. Cut coax **longer** than calculated by 5 mm.

3. Install SMA or BNC connectors on both ends.

4. Trim in steps of 1–2 mm, measuring electrical length with VNA (S11 phase
   response) until the section is exactly 90° (λ/4) at design frequency.

5. Seal both ends with heat-shrink tubing after final trim.

> **CAUTION:** Cutting is irreversible.  Always start long and trim toward
> the target length.

### 5.4 Microstrip Hybrid Construction (UHF)

1. Print PCB with controlled impedance (specify to fab if ordering).
   Alternatively, etch at home: use photoresist or toner transfer.

2. Trace widths:
   - 50 Ω on FR-4 (h=1.6 mm, εr=4.4): w = 2.9 mm
   - 35.35 Ω on FR-4: w = 5.2 mm
   - 70.71 Ω on FR-4: w = 0.9 mm

3. Verify trace widths with Saturn PCB Toolkit or AppCAD before ordering.

4. Solder SMA edge-launch connectors at each port.  Keep solder joints
   small; excess solder creates capacitive discontinuities at UHF.

5. Mount isolation resistors (for Wilkinson) as close as possible to the
   output junction pads.  For >500 MHz, use 0402 SMD MELF resistors.

### 5.5 Weatherproofing Procedure

For outdoor (mast-head) installation, all hybrid enclosures must be
weatherproofed before deployment.

1. Apply conformal coating (Humiseal 1B31 or equivalent) to all PCB
   surfaces after testing is complete and verified.  Apply 2 coats.

2. Allow 24 hours cure time at room temperature.

3. Install completed PCB in ASA enclosure (openscad/enclosure_hybrid_hf.scad
   or enclosure_hybrid_vhf.scad).

4. Install O-ring in lid groove.  O-ring should protrude 0.3 mm above
   groove face when uncompressed.

5. Torque lid screws uniformly.  M3 screws: 0.5 N·m (5 in-lbf).
   M4 screws: 1.0 N·m (9 in-lbf).  Do not overtighten (cracks ASA).

6. Apply self-amalgamating (self-fusing) tape over all SMA/BNC connector
   bodies and coaxial cable entry points.  At least 2 layers.

7. Apply silicone sealant around cable gland entry.

8. Label box with: band, hybrid type, date of construction, and port ID.

---

## CHAPTER 6 — EZNEC/NEC PHASED ARRAY MODELING

### 6.1 Overview of NEC Model Files

Three NEC model files are provided in the `nec_models/` directory:

| File | Description |
|---|---|
| `phased_2el_endfire_20m.nec` | 2-element endfire, 20M, 90° hybrid feed |
| `phased_4el_square_20m.nec` | 4-element square array, sequential phasing |
| `crossed_dipole_circ_pol_2m.nec` | Crossed dipoles, circular polarization, 2M |

### 6.2 Running NEC Models

**With 4nec2 (Windows, free):**

1. Open 4nec2 application.
2. File → Open → select .nec file.
3. Run → Calculate.
4. View → Far Field Plot for radiation patterns.
5. View → Currents for element current distribution.
6. View → Impedance at feed points.

**With EZNEC (Windows, paid — EZNEC Pro/2 or EZNEC+):**

1. Open EZNEC.
2. File → Open NEC file (some syntax differences may require conversion).
3. Run Frequency Sweep to see pattern vs. frequency.
4. Patterns → 3D Pattern for full visualization.

**With PyNEC (Python, cross-platform):**

```python
import PyNEC
c = PyNEC.nec_context()
c.geometry_complete(0)    # GE
c.gn_card(2, 0, 0, 0, 13.0, 0.005, 0, 0)  # GN
# Load GW, EX, FR, RP cards from .nec file
# See PyNEC documentation for card mapping
```

**With xnec2c (Linux):**

```bash
xnec2c phased_2el_endfire_20m.nec
```

### 6.3 Interpreting NEC Output

**Gain:** Shown in dBi (isotropic) or dBd (over dipole; dBd = dBi − 2.15 dB).
The pattern shows gain vs. azimuth and elevation angle.

**F/B ratio:** Maximum forward gain minus rearward gain at 180° from the main
lobe.  Read at the same elevation angle.

**Feed impedance:** NEC reports complex impedance Z = R + jX at each feed point.
For a phased array with mutual coupling, impedance may differ significantly from
a free-space single element.  A matching network may be required.

**Current forcing:** A quadrature hybrid or Wilkinson divider enforces equal
currents regardless of element self-impedance changes from mutual coupling.
This is the key practical advantage of these networks over simple L-C phasing:
the hybrid's port isolation prevents mutual coupling from affecting the feed
currents.

### 6.4 Modifying NEC Models for Other Bands

To adapt the 20M model to another band, scale all dimensions by f₂₀/f_target:

```
scale_factor = 14.175 / f_target_MHz

New element length = 10.470 × scale_factor   (meters)
New element spacing = 5.292 × scale_factor   (meters)
New height = 10.0 × scale_factor             (meters, optional)
```

For the 40M version (7.150 MHz): scale_factor = 14.175/7.15 = 1.982
- Element length: 10.470 × 1.982 = 20.75 m (half-wave)
- Element spacing: 5.292 × 1.982 = 10.49 m (λ/4)

### 6.5 4-Element Square Array — Phasing Network

The `phased_4el_square_20m.nec` model uses sequential phasing:

```
Element 1:   0°  → direct feed (reference)
Element 2: −90°  → one all-pass section
Element 3: −180° → two all-pass sections  (or reverse-polarity feed)
Element 4: −270° → three all-pass sections (or +90° directly)
```

All-pass 90° section for 14.175 MHz (from §2.4 of sch_lumped_element_hf.txt):

| Component | Value | Part |
|---|---|---|
| L | 561 nH | T-50-6, 9t, #22 AWG |
| C | 225 pF | NP0, C0G |

Connect in series: source → L → node → C to GND → 50 Ω load.
Phase shift: −90° at 14.175 MHz, ±10° from 8–25 MHz.

---

## CHAPTER 7 — MAINTENANCE

### 7.1 Routine Maintenance Schedule

| Interval | Action |
|---|---|
| After each field use | Inspect connectors for corrosion; dry any moisture |
| Monthly | Check SMA/BNC connector tightness (hand-tight + ¼ turn) |
| Quarterly | Re-verify amplitude and phase balance (Chapter 4) |
| Annually | Inspect O-ring; replace if compressed, cracked, or missing |
| As needed | Re-conformal-coat if PCB coating shows wear or discoloration |

### 7.2 Connector Maintenance

**SMA connectors:**
- Inspect center pin for straightness.  A bent center pin causes VSWR degradation.
- Apply a small amount of Dow Corning 111 compound or PTFE lubricant to threads
  before assembly.  This prevents galling and ensures waterproof seal.
- Torque: hand-tight + ¼ turn with wrench.  Do not use pliers directly on the
  connector hex.

**BNC connectors:**
- Inspect bayonet pins for wear.  A worn bayonet allows connector to rotate,
  causing intermittent contact.
- Replace BNC if bayonet is loose.

### 7.3 O-Ring Replacement

1. Remove lid screws.  Carefully pry lid from box body.
2. Remove old O-ring.  Clean groove with isopropyl alcohol.
3. Measure O-ring groove dimensions: width and depth.
4. Order replacement O-ring: 2 mm cross-section, continuous loop matching groove
   perimeter.  Use EPDM or silicone material (UV resistant).
5. Apply thin coat of silicone grease to new O-ring.
6. Seat O-ring in groove without twisting.
7. Reinstall lid; check O-ring protrusion before final tightening.

---

## CHAPTER 8 — TROUBLESHOOTING

### 8.1 High Insertion Loss (>3.5 dB)

| Possible Cause | Check | Remedy |
|---|---|---|
| Resistive connector | Inspect SMA/BNC | Clean or replace |
| Wrong core material | Check core markings | Verify Mix-43 vs. Mix-61 |
| Inductor value wrong | Measure with LC meter | Rewind to spec |
| Loss in capacitor | Verify NP0 type | Replace with C0G/NP0 |
| Cold solder joint | Visual inspection + probe | Re-solder |
| PCB trace too narrow | Measure trace width | Route wider trace |

### 8.2 Poor Amplitude Balance (>0.5 dB)

| Possible Cause | Check | Remedy |
|---|---|---|
| Mismatched L values | Measure both arms | Rewind mismatched inductor |
| Mismatched C values | Measure both arms | Replace mismatched capacitor |
| Ground loop | Inspect board layout | Add ground wire; redesign |
| Band switch contact | Measure switch R | Replace or clean rotary switch |

### 8.3 Phase Error (>5°)

| Possible Cause | Check | Remedy |
|---|---|---|
| L or C value off | Measure components | Rewind/replace |
| Unequal cable lengths | Measure cable electrical length | Add/trim cable length |
| Wrong coax VF used | Verify VF | Cut new section with correct VF |
| Frequency off-center | Verify signal source | Set exact center frequency |
| Ground inductance | Check ground path length | Use direct short traces |

### 8.4 Poor Isolation (<15 dB)

| Possible Cause | Check | Remedy |
|---|---|---|
| Wrong isolation R value | Measure R_iso | Replace with 100 Ω ±1% |
| R_iso open | Measure continuity | Replace resistor |
| External coupling | Check layout | Shield sections; increase separation |
| Frequency far from design | Confirm design frequency | Redesign for correct band |

### 8.5 Hybrid Works at Test, Fails Outdoors

| Possible Cause | Check | Remedy |
|---|---|---|
| Moisture ingress | Inspect O-ring seal | Replace O-ring; re-seal |
| Connector corrosion | Inspect visually | Clean with DeOxIt; replace |
| Thermal expansion | Check at operating temperature | Use wider tolerance C/L |
| Vibration loosening | Check all connections | Apply Loctite 222 to threads |

---

## CHAPTER 9 — PARTS LIST

### 9.1 2-Way Wilkinson Divider — HF (One Unit)

| Ref | Qty | Value | Part Description |
|---|---|---|---|
| L1, L2 | 2 | Per band table | Series inductors (L/2 each) |
| C1, C2 | 2 | Per band table | Shunt capacitors, NP0/C0G |
| R1 | 1 | 100 Ω, 1 W | Isolation resistor |
| J1 | 1 | BNC or SMA | Input connector |
| J2, J3 | 2 | BNC or SMA | Output connectors |

### 9.2 4-Way Wilkinson Divider — HF (One Unit)

| Ref | Qty | Value | Part Description |
|---|---|---|---|
| L1–L6 | 6 | Per band table | Series inductors |
| C1–C3 | 3 | Per band table | Shunt capacitors |
| R1–R3 | 3 | 100 Ω, 1 W | Isolation resistors |
| J1 | 1 | BNC or SMA | Input |
| J2–J5 | 4 | BNC or SMA | Outputs |

### 9.3 Quadrature Hybrid — HF (One Unit)

| Ref | Qty | Value | Part Description |
|---|---|---|---|
| L_sh1–4 | 4 | Per band table | 50 Ω arm inductors |
| L_sr1–4 | 4 | Per band table | 35.35 Ω arm inductors |
| C_sh1–4 | 4 | Per band table | 50 Ω arm capacitors |
| C_sr1–4 | 4 | Per band table | 35.35 Ω arm capacitors |
| R_term | 1 | 50 Ω, 1 W | Port 4 termination |
| J1–J4 | 4 | BNC or SMA | Four ports |

### 9.4 Rat-Race Coupler — VHF/UHF (One Unit)

| Ref | Qty | Value | Part Description |
|---|---|---|---|
| TL1 | 1 | RG-59, 3λ/2 length | 70.71 Ω ring coaxial cable |
| J1–J4 | 4 | SMA (edge or chassis) | Four ports |
| R_term | 1 | 50 Ω, 0.5 W SMD | Spare port termination |
| PCB | 1 | 50 × 50 mm | VHF hybrid PCB (4nec2 layout) |

### 9.5 TLT Balun — HF (Guanella 1:1, One Unit)

| Ref | Qty | Value | Part Description |
|---|---|---|---|
| T1 | 1 | BN-43-202 | Binocular core, Mix 43 |
| Wire | 1 | 30 cm, #22 AWG | Teflon-coated magnet wire |
| J1 | 1 | SO-239 | Unbalanced (coax) port |
| J2, J3 | 2 | Binding post | Balanced port terminals |

### 9.6 CYD Monitor — Hybrid Test System

| Ref | Qty | Value | Part Description |
|---|---|---|---|
| U1 | 1 | ESP32-2432S028 | CYD touchscreen module |
| U2, U3 | 2 | AD8307 module | Log detector, −75 to +17 dBm |
| R1, R2 | 2 | 1 kΩ | Zero-crossing input voltage divider |
| R3, R4 | 2 | 1 kΩ | Second divider resistor (to GND) |
| K1–K5 | 5 | 5V relay | Band switching relays |
| J1–J6 | 6 | SMA or BNC | Hybrid port connections |

### 9.7 Enclosure Components

| Ref | Qty | Value | Part Description |
|---|---|---|---|
| — | 1 | ASA filament 500g | Enclosure body, lid, parts |
| O-ring | 1 | 2 mm × 280 mm perim. | Silicone or EPDM (HF box) |
| M3 ins. | 8 | Ruthex MK3-4 | Heat-set inserts (VHF box) |
| M4 ins. | 8 | Ruthex MK4-4 | Heat-set inserts (HF box) |
| M3 screws | 8 | M3 × 8 SHCS | Lid fasteners (VHF) |
| M4 screws | 8 | M4 × 10 SHCS | Lid fasteners (HF) |

---

## APPENDIX A — COMPLETE COMPONENT VALUE TABLES

### A.1 Wilkinson 2-Way — All Bands

For 50 Ω system; Z_arm = 70.71 Ω; T-network (L/2, C, L/2); R_iso = 100 Ω.
L = 70.71/(2πf), C = 1/(2πf × 70.71)

| Band | f₀ (MHz) | L total (µH) | L/2 (µH) | C (pF) | Core |
|---|---|---|---|---|---|
| 160M | 1.850 | 6.08 | 3.04 | 1216 | T-94-2, 22t |
| 80M | 3.750 | 3.00 | 1.50 | 601 | T-94-2, 16t |
| 40M | 7.150 | 1.574 | 0.787 | 315 | T-68-6, 11t |
| 30M | 10.125 | 1.111 | 0.556 | 222 | T-50-6, 10t |
| 20M | 14.175 | 0.793 | 0.397 | 159 | T-50-6, 8t |
| 17M | 18.118 | 0.621 | 0.311 | 124 | T-50-6, 7t |
| 15M | 21.225 | 0.530 | 0.265 | 106 | T-50-6, 7t |
| 12M | 24.940 | 0.451 | 0.226 | 90 | T-37-6, 8t |
| 10M | 28.850 | 0.390 | 0.195 | 78 | T-37-6, 7t |
| 6M | 50.500 | 0.223 | 0.111 | 44.7 | T-37-6, 5t |

### A.2 Quadrature Hybrid — All Bands

L_sh = 50/(2πf), C_sh = 1/(2πf×50), L_sr = 35.35/(2πf), C_sr = 1/(2πf×35.35)

| Band | f₀ (MHz) | L_sh (µH) | C_sh (pF) | L_sr (µH) | C_sr (pF) |
|---|---|---|---|---|---|
| 160M | 1.850 | 4.30 | 1719 | 3.04 | 2432 |
| 80M | 3.750 | 2.12 | 849 | 1.50 | 1201 |
| 40M | 7.150 | 1.113 | 446 | 0.787 | 631 |
| 30M | 10.125 | 0.785 | 315 | 0.555 | 446 |
| 20M | 14.175 | 0.561 | 225 | 0.397 | 318 |
| 17M | 18.118 | 0.439 | 176 | 0.310 | 249 |
| 15M | 21.225 | 0.375 | 150 | 0.265 | 213 |
| 12M | 24.940 | 0.319 | 128 | 0.226 | 181 |
| 10M | 28.850 | 0.276 | 110 | 0.195 | 156 |
| 6M | 50.500 | 0.158 | 63 | 0.111 | 89 |

---

## APPENDIX B — TRANSMISSION LINE LENGTH TABLES

### B.1 λ/4 Coaxial Sections (50 Ω, RG-58, VF=0.66)

Length (mm) = 49,500 / f_MHz

| Band | f₀ (MHz) | λ/4 physical (mm) |
|---|---|---|
| 6M | 50.500 | 980 |
| 2M | 146.000 | 339 |
| 1.25M | 223.500 | 221 |
| 70cm | 435.000 | 114 |
| 33cm | 915.000 | 54 |
| 23cm | 1296.000 | 38 |

### B.2 Rat-Race Ring (3λ/2, RG-59, VF=0.66)

Total length = 3 × (λ/4 × 4) × VF / 4 = 3 × (c×VF)/(4f) × ... = 3×49500/f_MHz mm

| Band | f₀ (MHz) | Total ring (mm) | λ/4 section (mm) |
|---|---|---|---|
| 6M | 50.500 | 2940 | 490 |
| 2M | 146 | 1017 | 170 |
| 1.25M | 223.5 | 665 | 111 |
| 70cm | 435 | 341 | 57 |
| 33cm | 915 | 162 | 27 |
| 23cm | 1296 | 115 | 19 |

---

## APPENDIX C — NEC MODEL QUICK REFERENCE

### C.1 NEC Card Summary for Phased Arrays

```
GW  wire geometry (tag, segs, x1,y1,z1, x2,y2,z2, radius)
GE  geometry end (0=free space, 1=ground present)
GN  ground parameters (type, -, -, -, εr, σ_Sm, -, -)
EX  excitation (0=voltage, tag, seg, 0, re(V), im(V))
FR  frequency (0=single, 0, 0, 0, f_MHz, 0)
RP  radiation pattern (0=gain, θ_pts, φ_pts, mode, θ0, φ0, Δθ, Δφ)
EN  end of model
```

### C.2 Phase Encoding for EX Card

| Phase (degrees) | Re(V) | Im(V) |
|---|---|---|
| 0° | 1.0 | 0.0 |
| −90° (lag) | 0.0 | −1.0 |
| −180° | −1.0 | 0.0 |
| −270° (=+90°) | 0.0 | 1.0 |
| +45° | 0.707 | 0.707 |
| −45° | 0.707 | −0.707 |

---

## APPENDIX D — WEATHERPROOFING AND FIELD INSTALLATION

### D.1 Outdoor Installation Checklist

```
[ ] Conformal coat applied (2 coats); cure complete
[ ] O-ring installed; groove clean and grease applied
[ ] Lid torqued to spec (M3: 0.5 N·m; M4: 1.0 N·m)
[ ] SMA/BNC connectors hand-tight + 1/4 turn
[ ] Self-amalgamating tape over connector bodies (min 2 layers)
[ ] Silicone applied at cable gland entry
[ ] Drain hole provided (bottom of enclosure, 3 mm dia)
[ ] Label: band, type, date, port ID, operator callsign
[ ] Mast clamp secure; lock-washer on clamp bolt
[ ] Drip cap installed if in direct rain exposure
[ ] Test: apply 100 mL water; no ingress after 5 minutes
```

### D.2 Cable Entry and Weatherproofing

For each SMA or BNC connector passing through the enclosure wall:

1. Use chassis-mount SMA with PTFE gasket (Amphenol 901-9703-RFX or similar).
2. Apply Loctite 569 thread sealant to panel thread before nut.
3. After final positioning, apply bead of silicone (Dow 738 or equivalent)
   around connector body on exterior face.
4. Apply self-amalgamating tape starting below connector body, overlapping
   onto the coaxial cable for at least 50 mm.

---

## APPENDIX E — GLOSSARY

**Amplitude balance:** Difference in insertion loss between two output ports
of a divider or hybrid.  Units: dB.  Target: 0 dB (equal power split).

**Axial ratio:** Figure of merit for circular polarization quality.  0 dB = perfect
circular; 3 dB = 2:1 ellipse; >6 dB ≈ nearly linear.  Measured in dBic.

**Branch-line coupler:** See *quadrature hybrid.*

**Cardioid:** Heart-shaped antenna radiation pattern; maximum in one direction,
null at 180° opposite.  Typical of λ/4-spaced dipoles fed with 90° phase shift.

**Current forcing:** A feed method that enforces equal current magnitudes at
array elements regardless of impedance changes due to mutual coupling.
Achieved by using a hybrid coupler with high output-port isolation.

**Endfire:** Radiation from an array in the direction of the element axis
(the direction along which elements are physically aligned and spaced).

**Isolation:** Attenuation between two output ports.  High isolation (>20 dB)
means loading one port does not affect the other.

**Monopulse:** A direction-finding technique using a sum (Σ) and difference (Δ)
output from a rat-race coupler.  Maximum sum signal + null difference = boresight.

**Phased array:** Multiple antenna elements fed with controlled phase and
amplitude to produce a steerable or shaped radiation pattern.

**Rat-race coupler:** A 4-port ring hybrid providing sum and difference ports.
Also called a ring coupler or hybrid ring.

**Return loss:** Ratio of incident to reflected power at a port: 20·log₁₀(1/|Γ|) dB.
A well-matched port shows >20 dB return loss.

**S-parameters:** Scattering parameters; used to describe RF network behavior.
S11 = input return loss.  S21 = forward insertion gain/loss.

**TLT:** Transmission line transformer.  Uses coax or twisted-pair on a ferrite
core; provides wideband operation because signal travels in TEM mode.

**Wilkinson divider:** A 3-port power divider providing equal-power in-phase
outputs with resistive port isolation.  First described by E. Wilkinson in 1960.

---

*End of TM-HYB-001 Rev A*

---
*Documentation standard: TM-HYB-001 follows Navy technical manual format.*
*For corrections or additions, file at project issue tracker.*
