# TECHNICAL MANUAL TM-HFTN-001 REV A
## MOTORIZED HF/VHF/UHF ANTENNA TUNING SYSTEM
### COMPREHENSIVE OPERATOR AND MAINTENANCE MANUAL

---

```
DOCUMENT NUMBER:   TM-HFTN-001 Rev A
SYSTEM:            Motorized Antenna Tuning System
COVERAGE:          160M through 20CM (1.8 MHz – 1500 MHz)
CLASSIFICATION:    UNCLASSIFIED
DATE:              2026-05-02
SUPERSEDES:        None
PREPARED BY:       TM Engineering
```

---

## TABLE OF CONTENTS

- Chapter 1 — System Description and Theory of Operation
- Chapter 2 — Specifications
- Chapter 3 — Controls and Indicators
- Chapter 4 — Installation
- Chapter 5 — Operating Procedures
- Chapter 6 — Calibration Procedures
- Chapter 7 — Antenna NEC Models
- Chapter 8 — Firmware Architecture
- Chapter 9 — Winding Tables
- Chapter 10 — Troubleshooting
- Chapter 11 — Preventive Maintenance
- Chapter 12 — Parts List and Procurement
- Appendix A — Schematic Diagrams
- Appendix B — Band-by-Band Operating Notes
- Appendix C — Wheeler Formula Reference
- Appendix D — Firmware Command Quick Reference

---

## CHAPTER 1 — SYSTEM DESCRIPTION AND THEORY OF OPERATION

### 1.1 System Overview

The Motorized HF/VHF/UHF Antenna Tuning System is a two-subsystem impedance matching system designed to interface any amateur or experimental RF antenna to a 50-ohm transceiver over the frequency range 1.8 MHz through 1500 MHz. The system is controlled by an ESP32-WROOM-32 microcontroller operating as a WiFi access point and Bluetooth SPP server. A CYD (ESP32-2432S028R) touchscreen display provides the primary local operator interface. An automatic tuning algorithm measures SWR via a Bruene-type bridge and drives two stepper motors to minimize reflected power.

**Subsystem 1 — HF Tuner (1.8–54 MHz)**

The HF tuner employs a motorized variable air capacitor (10–500 pF) and a motorized roller inductor (0.05–80 µH) arranged in switchable L, Pi, and T network topologies via SPDT relays. The roller contact selects the effective inductance by tapping the winding at the desired turn. The variable capacitor provides continuous tuning between minimum and maximum plates engagement. Motor position is tracked by step count from a known home position. The network achieves impedance matching for antenna loads from approximately 10 to 500 ohms.

**Subsystem 2 — VHF/UHF Stub Tuner (144–1500 MHz)**

The VHF/UHF stub tuner uses an air-dielectric coaxial sliding short to form a resonant stub that cancels antenna reactance on the 2-meter, 1.25-meter, 70-centimeter, 33-centimeter, and 20-centimeter bands. A third NEMA 17 stepper motor drives the sliding short via an M8×1.25 leadscrew, achieving sub-millimeter position resolution. The stub length is set to approximately λ/4 for the selected band and fine-tuned under SWR feedback.

**System Block Diagram**

```
                          MOTORIZED ANTENNA TUNING SYSTEM
                         ┌─────────────────────────────────────────────────────┐
                         │                                                     │
  ANTENNA ───────────────┤──► [BRUENE SWR BRIDGE] ──────────────────────────┐ │
                         │         │ Vfwd Vref                               │ │
                         │         ▼                                         │ │
                         │    [ADC Ch0, Ch1]                                 │ │
                         │         │                                         │ │
                         │         ▼                                         │ │
                         │   [ESP32-WROOM-32] ◄──► [CYD TOUCHSCREEN via BT] │ │
                         │    240 MHz dual core                              │ │
                         │    WiFi AP 192.168.4.1 ◄──► [Web Browser]        │ │
                         │         │                                         │ │
                         │    ┌────┴────────────────────────────┐            │ │
                         │    │                                 │            │ │
                         │    ▼                                 ▼            │ │
                         │ [TMC2208 A]                     [TMC2208 B]       │ │
                         │ Motor Driver                    Motor Driver      │ │
                         │    │                                 │            │ │
                         │    ▼                                 ▼            │ │
                         │ [NEMA 17]                         [NEMA 17]       │ │
                         │ Roller Inductor                   Var. Cap        │ │
                         │    │                                 │            │ │
                         │    └──────────────┬──────────────────┘            │ │
                         │                  ▼                                │ │
                         │       [L/Pi/T NETWORK]  ◄─────────────────────────┘ │
                         │          Relay-switched topology                  │
                         │                  │                                │
  TRANSCEIVER ───────────┤──────────────────┘                                │
                         └─────────────────────────────────────────────────────┘
```

**Controller Summary**

The ESP32-WROOM-32 performs all real-time functions: ADC sampling of forward and reflected voltages, SWR calculation, stepper motor step generation via TMC2208 STEP/DIR interface, relay control for network topology switching, and hosting the HTTP web server. The auto-tune state machine runs as a FreeRTOS task at lower priority than the SWR sampling task, ensuring continuous SWR monitoring does not block tuning motor movement.

The CYD touchscreen communicates with the ESP32 via Bluetooth SPP (Serial Port Profile). The CYD receives status packets (SWR, L position, C position, band, fault code) at 10 Hz and transmits operator commands (TUNE, BAND, SETL, SETC, STOP). No I2C or SPI connection between the two units is required.

---

### 1.2 Impedance Matching Theory

**Transmission Line Theory**

When a transmission line of characteristic impedance Z₀ (typically 50 Ω) is terminated in a load Z_L that differs from Z₀, a portion of the incident wave is reflected back toward the source. The fraction of reflected voltage is characterized by the reflection coefficient Γ (Gamma):

```
        Z_L - Z_0
Γ  =  ───────────
        Z_L + Z_0
```

Γ is in general a complex number. Its magnitude |Γ| ranges from 0 (perfect match) to 1 (total reflection, open or short circuit). The Standing Wave Ratio (SWR) is derived from |Γ|:

```
        1 + |Γ|
SWR = ───────────
        1 - |Γ|
```

The SWR is a real number ≥ 1. SWR of 1:1 indicates a perfect match. SWR of infinity indicates total reflection (open or short).

**Power Loss from Mismatch**

The reflected power fraction is:

```
P_reflected = P_forward × |Γ|²
```

Practical consequence examples:

| SWR   | |Γ|   | P_reflected | Mismatch Loss (dB) |
|-------|--------|-------------|---------------------|
| 1.0:1 | 0.000  | 0%          | 0.00 dB             |
| 1.5:1 | 0.200  | 4%          | 0.18 dB             |
| 2.0:1 | 0.333  | 11%         | 0.51 dB             |
| 3.0:1 | 0.500  | 25%         | 1.25 dB             |
| 5.0:1 | 0.667  | 44%         | 2.55 dB             |
| 10:1  | 0.818  | 67%         | 4.81 dB             |

**Example: SWR 3:1**
- |Γ| = (3 - 1)/(3 + 1) = 0.5
- P_reflected = 0.5² = 0.25 → 25% of forward power reflected
- Mismatch loss = -10 log₁₀(1 - 0.25) = 1.25 dB

**Matching Network Purpose**

An impedance matching network is a reactive two-port circuit that presents Z₀ = 50 Ω at its input port while simultaneously presenting Z_antenna at its output port. Because the network contains only reactive elements (capacitors and inductors in the ideal case), it stores and re-releases energy rather than dissipating it. The practical loss of a well-constructed LC network at HF is typically 0.1 to 0.5 dB, which is far less than the mismatch loss it eliminates.

---

### 1.3 Network Topologies

**L-Network — Two Configurations**

The L-network consists of one series reactive element and one shunt reactive element. There are two configurations depending on whether the antenna impedance is higher or lower than 50 Ω.

*Type 1: High-impedance load (Z_antenna > 50 Ω) — shunt element at output*

```
      ──[Xs]──┬── (to antenna)
              │
             [Xp]
              │
             GND
```

The shunt element (Xp) at the output reduces the impedance seen by the series element (Xs) at the input.

*Type 2: Low-impedance load (Z_antenna < 50 Ω) — shunt element at input*

```
      ──┬──[Xs]── (to antenna)
        │
       [Xp]
        │
       GND
```

The shunt element at the input raises the impedance presented to the source.

**L-Network Design Equations**

The network Q is determined by the impedance transformation ratio:

```
Q = √(R_H / R_L − 1)
```

Where R_H is the higher of the two resistances (load or source) and R_L is the lower.

The reactive element values are:

```
X_shunt = R_H / Q
X_series = Q × R_L ± X_antenna_reactance
```

The sign of the ± term is chosen to cancel any reactive component in the antenna impedance. If the antenna has inductive reactance (+jX), use a series capacitive reactance (−jX_c) to cancel it, and vice versa.

*L-Network Design Example:*

Antenna Z = 120 + j30 Ω at 14.2 MHz, source = 50 Ω:

- R_H = 120, R_L = 50
- Q = √(120/50 − 1) = √1.4 = 1.183
- X_shunt = 120 / 1.183 = 101.4 Ω (capacitive, since shunt element at output)
- X_series = 1.183 × 50 = 59.2 Ω (inductive) + (−j30 cancellation) = net 29.2 Ω inductive
- C_shunt = 1/(2π × 14.2 × 10⁶ × 101.4) = 111 pF
- L_series = 29.2 / (2π × 14.2 × 10⁶) = 0.327 µH

**Pi-Network**

The Pi-network (π-network) uses two shunt capacitors and one series inductor. It is used when both the input and output impedances are defined (for instance, when matching a transmitter output network to an antenna feedline). The Pi offers higher Q than an L-network and better harmonic suppression, making it useful in transmitter output stages.

Pi design from loaded Q:

Given source R_in, load R_load, and desired loaded Q:

```
X_C1 = R_in / Q
X_C2 = R_load / √((Q² + 1)(R_in/R_load) − 1)
X_L  = (Q × R_in + R_load² / (X_C2)) / (Q² + 1 + (R_load/X_C2)²)   [approximate]
```

In practice, Pi-network values are read from design charts for a given frequency and impedance transformation ratio. For the motorized tuner, the Pi topology is implemented by adding a second shunt capacitor (C2) via relay.

**T-Network**

The T-network uses two series inductors and one shunt capacitor. It handles high-impedance antenna loads well and provides good harmonic suppression on the low-impedance side. The T is particularly useful for matching very high impedance antennas (end-fed wires, windom antennas) where impedances of several hundred to several thousand ohms may be seen.

```
   ──[L1]──┬──[L2]──
           │
          [C]
           │
          GND
```

**Network Type Summary**

| Network | Best Use Case                     | Pros                              | Cons                            |
|---------|-----------------------------------|-----------------------------------|---------------------------------|
| L       | General HF matching, wide range   | Simple, low loss, wide range      | Q fixed by impedance ratio      |
| Pi      | Transmitter output, defined Z     | Adjustable Q, good harmonic supp. | Requires 3 components           |
| T       | High-Z antennas, end-fed wires    | Wide matching range, high Z       | Higher circulating currents     |
| L (rev) | Low-Z antennas (short verticals)  | Simple, effective for Z < 50Ω    | Fixed Q                         |

---

### 1.4 Inductor Theory

**Basic Inductance Formula**

The inductance of a coil is related to its physical dimensions and core material by:

```
        µ₀ × µᵣ × N² × A
L  =  ─────────────────────    [Henry]
                l
```

Where:
- µ₀ = 4π × 10⁻⁷ H/m (permeability of free space)
- µᵣ = relative permeability of core (1.0 for air)
- N = number of turns
- A = cross-sectional area of core (m²)
- l = length of winding (m)

**Wheeler Single-Layer Approximation (English units)**

For a single-layer air-core coil with diameter d and length l (both in inches), and N turns:

```
         d² × N²
L(µH) = ──────────────
          18d + 40l
```

This formula is accurate to within 1% for l ≥ 0.4d. For very short coils (l < 0.4d) a correction factor applies.

**Wheeler Multi-Layer Approximation**

For a multi-layer coil with mean winding radius a, length l, and radial winding thickness c (all in inches):

```
          0.8 × a² × N²
L(µH) = ──────────────────────
          6a + 9l + 10c
```

**Q Factor**

The quality factor Q of an inductor is the ratio of energy stored to energy dissipated per cycle:

```
        ωL      2πfL
Q  =  ──────  = ──────
       R_ac      R_ac
```

Where R_ac is the AC resistance of the winding, which includes skin effect and proximity effect losses.

**Skin Effect**

At high frequencies, current flows only in a thin layer at the conductor surface. The skin depth δ is:

```
         1
δ  =  ──────────────
       √(π × f × µ × σ)
```

For copper: δ ≈ 66.5 / √(f_Hz) mm, or equivalently δ ≈ 0.0665 / √(f_MHz) mm.

The AC resistance relative to DC resistance is approximately:

```
R_ac ≈ R_dc × √(f_MHz)    [approximate, valid for conductor diameter >> δ]
```

This means Q degrades at higher frequencies unless larger-diameter wire is used.

**Self-Resonant Frequency**

Every inductor has distributed interwinding capacitance (C_stray). The coil self-resonates at:

```
             1
f_SRF  =  ──────────────
           2π √(L × C_stray)
```

Above f_SRF, the component behaves as a capacitor rather than an inductor. For reliable operation, use the coil only below 0.5 × f_SRF. The roller inductor winding design accounts for this by using a single-layer coil with wide pitch, minimizing C_stray.

**Typical Q Values for HF Air-Core Inductors**

| Band   | Freq (MHz) | L (µH) | Q (estimated) | Notes                    |
|--------|------------|--------|----------------|--------------------------|
| 160m   | 1.85       | 20–40  | 200–300        | Low frequency, high Q    |
| 80m    | 3.75       | 10–20  | 250–350        | Best overall Q range     |
| 60m    | 5.35       | 8–16   | 280–380        |                          |
| 40m    | 7.15       | 4–10   | 300–400        | Near peak Q frequency    |
| 30m    | 10.12      | 2–6    | 320–420        | Good Q                   |
| 20m    | 14.2       | 1–4    | 300–400        | Wire size critical       |
| 17m    | 18.1       | 0.5–2  | 250–350        | Skin effect increasing   |
| 15m    | 21.2       | 0.5–1.5 | 220–320      |                          |
| 12m    | 24.9       | 0.3–1  | 200–280        |                          |
| 10m    | 28.5       | 0.1–0.8 | 180–250      | Proximity effect matters |
| 6m     | 50.0       | 0.05–0.5 | 150–200     | Use larger wire gauge    |

Note: Q values are for well-wound air-core coils using AWG 18–20 wire. Poorly wound coils, or coils with close-wound enamel wire on plastic formers without proper dielectric clearance, will have lower Q.

---

### 1.5 Capacitor Theory

**Parallel Plate Capacitance**

For a capacitor with N conductive plates (N−1 pairs) of area A separated by distance d:

```
          ε₀ × (N − 1) × A
C  =  ─────────────────────
                d
```

Where ε₀ = 8.854 × 10⁻¹² F/m. For an air-dielectric capacitor, the dielectric constant εᵣ = 1.00 (no multiplication factor needed).

**Variable Air Capacitor Rotation**

For a variable capacitor with rotation angle θ (0° = minimum, 180° = maximum engagement):

```
C(θ) = C_min + (C_max − C_min) × (θ / 180°)
```

In the motorized system, θ is determined by step count from the home position.

**Voltage Rating**

The peak voltage across a capacitor at RF power P and reactance X_C is:

```
V_pk = √(2 × P × X_C)  =  √(2P / (2πfC))
```

The voltage rating of the air capacitor must exceed V_pk at all operating conditions. The maximum working voltage chart below assumes 150W forward power. Plate spacing of ≥ 1.0 mm is required for safety margin at these voltages.

**Maximum Working Voltage vs. Capacitance and Band (150W, 50Ω Source)**

| Band | Freq (MHz) | C = 10 pF | C = 50 pF | C = 100 pF | C = 500 pF |
|------|------------|-----------|-----------|------------|------------|
| 160m | 1.85       | 3120 V    | 1396 V    | 987 V      | 441 V      |
| 80m  | 3.75       | 2190 V    | 979 V     | 693 V      | 310 V      |
| 40m  | 7.15       | 1585 V    | 709 V     | 501 V      | 224 V      |
| 20m  | 14.2       | 1124 V    | 503 V     | 355 V      | 159 V      |
| 10m  | 28.5       | 793 V     | 354 V     | 251 V      | 112 V      |
| 6m   | 50.0       | 599 V     | 268 V     | 190 V      | 85 V       |

Note: High voltages at low capacitance on lower bands. The 10 pF minimum setting at 160m can see over 3 kV with 150W input. Ensure adequate plate spacing and never reduce capacitance while transmitting at high power.

**Capacitive Reactance**

```
         1
X_C  =  ──────────
          2πfC
```

Where f is in Hz and C is in Farads. For practical use with f in MHz and C in pF:

```
          10⁶
X_C  =  ──────────    [Ohms, f in MHz, C in pF]
          2πfC
```

---

### 1.6 SWR Bridge Theory

**Bruene Bridge Design**

The Bruene directional coupler samples a small fraction of the RF voltage and current flowing through a series sampling line. The core component is a toroidal transformer wound on a ferrite core (FT-50-43 material, µ ≈ 850 at HF), with a 20-turn secondary coupling winding. A through-conductor (one turn primary) passes through the toroid center.

The directional coupler produces two output voltages:
- V_fwd: proportional to V_forward traveling wave
- V_ref: proportional to V_reflected traveling wave

**Directional Coupler Principle**

```
                    FT-50-43 Toroid
                    20-turn coupling
                         ────
  TX ───────────────────┤  ├───────────────────── ANT
                    └── │  │ ──┘
                        ────
                         │  │
                    ┌────┘  └────┐
                   [R_term]  [R_term]
                    │              │
                   [D1 BAT46]   [D2 BAT46]
                    │              │
                  [GND]         [GND]
                    │              │
                  [C_filter]   [C_filter]
                    │              │
                  ADC_FWD       ADC_REF
```

**Forward and Reflected Voltage Calculation**

```
V_fwd = (V_through + V_coupled) / 2
V_ref = (V_through − V_coupled) / 2
```

The bridge detects these voltages using BAT46 Schottky diodes. The BAT46 has a forward voltage of approximately 0.2 V (vs. 0.6 V for silicon), making it suitable for detecting low-level RF signals. The detected DC voltage is proportional to the peak RF voltage at each port.

**ADC Scaling**

The ESP32 ADC is a 12-bit converter with 0–3.3V input range. Calibration maps the ADC counts to actual power:

- Full scale (4095 counts) corresponds to 3.3 V at ADC input
- At 100W into 50 Ω, V_fwd = √(2 × P × Z) = √(2 × 100 × 50) = 100 V peak
- After the coupling transformer (−20 dB = 0.01 ratio) and detector diode drop: ~0.8 V at ADC
- A trim resistor divider in the bridge sets the ADC voltage to approximately 3.0 V at 200W (full scale)
- Calibration at a known power level corrects for component tolerances

**Directivity**

Directivity is the ratio of the forward signal level to the leakage of forward signal into the reflected port (in dB). The Bruene bridge achieves > 30 dB directivity across the HF range, meaning the reflected port reads forward signal at −30 dB below its indicated level — negligible for SWR measurements.

**Insertion Loss**

The Bruene bridge introduces < 0.1 dB insertion loss across the HF range. This is because the coupling toroid is in parallel with the through-line, not in series with it, and only a small fraction (1%) of the voltage is diverted to the detector.

---

### 1.7 Auto-Tune Algorithm

**Overview**

The auto-tune algorithm finds the LC combination that minimizes SWR. It uses a two-phase approach: a coarse scan to identify the approximate best region, followed by a fine-tune gradient descent from the best coarse position. This approach is faster than a brute-force scan of the full LC space while avoiding false minima.

**Auto-Tune Flowchart**

```
                    ┌──────────────────┐
                    │   TUNE COMMAND   │
                    └────────┬─────────┘
                             │
                    ┌────────▼─────────┐
                    │  MEASURE BASELINE│
                    │       SWR        │
                    └────────┬─────────┘
                             │
                    ┌────────▼─────────┐
                    │  SWR < 1.5:1 ?   │
                    └────────┬─────────┘
                          Yes│  No
                    ┌────────┘  └────────────────────────┐
                    │                                    │
          ┌─────────▼────────┐               ┌───────────▼──────────┐
          │ ALREADY MATCHED  │               │    COARSE SCAN       │
          │   No action      │               │  Sweep L full range  │
          └──────────────────┘               │  Binary search C     │
                                             │  Record best (L,C)   │
                                             └───────────┬──────────┘
                                                         │
                                             ┌───────────▼──────────┐
                                             │  MOVE TO BEST (L,C)  │
                                             └───────────┬──────────┘
                                                         │
                                             ┌───────────▼──────────┐
                                             │     FINE TUNE        │
                                             │  Gradient descent    │
                                             │  Adjust L, then C    │
                                             │  Repeat until min    │
                                             └───────────┬──────────┘
                                                         │
                                          ┌──────────────┴──────────────┐
                                          │                             │
                               ┌──────────▼─────────┐       ┌──────────▼────────┐
                               │   SWR < 1.3:1      │       │  > 50 iterations  │
                               │   or converged?    │       │  OR               │
                               └──────────┬─────────┘       │  SWR still > 10:1 │
                                          │Yes               └──────────┬────────┘
                               ┌──────────▼─────────┐                  │
                               │   MATCHED           │       ┌──────────▼────────┐
                               │   Save preset       │       │     FAULT F01     │
                               │   if improved       │       │   Check antenna   │
                               └─────────────────────┘       └───────────────────┘
```

**Algorithm Steps (Detailed)**

1. Measure baseline SWR by sampling V_fwd and V_ref for 50 ms (moving average).
2. If SWR < 1.5:1, set state AT_MATCHED, return without moving motors.
3. If band preset exists and has not been tried yet, move to preset position and re-measure SWR. If SWR < 1.5:1, accept preset and go to step 13.
4. Coarse scan: divide L range into 20 equal steps. At each L position, perform a binary search of C range to find the C value that minimizes SWR. Record SWR at each (L, C_best) pair.
5. From the coarse scan results, identify the L step with minimum SWR. Record this as (L_best, C_best).
6. Move to (L_best, C_best).
7. Fine-tune phase: adjust L by ±2 steps in each direction. Keep the direction that reduces SWR. Continue stepping L in that direction until SWR begins increasing.
8. At the new L position, repeat binary C search over ±20% of C range.
9. Repeat steps 7–8 until neither L nor C adjustment reduces SWR, or 50 iterations are reached.
10. If SWR < 1.3:1, declare AT_MATCHED. Sound single tone on buzzer (if fitted). Set status LED green.
11. If SWR between 1.3 and 1.5:1, declare AT_MATCHED (acceptable match). No tone. Set status LED green.
12. If SWR > 1.5:1 after 50 iterations, declare AT_FAULT F01. Set status LED red.
13. If improvement from stored preset > 0.2 SWR units, save new (L, C) as updated band preset.
14. Return to AT_IDLE state.

**Convergence Note**

The binary search at each L position requires approximately log₂(200) ≈ 8 C steps to find the minimum. With 20 L steps × 8 C steps × 50 ms measurement time per step, the coarse scan takes approximately 8 seconds worst case. In practice, early termination when SWR < 1.5:1 is found makes typical tune time 3–5 seconds.

---

### 1.8 VHF/UHF Stub Tuner Theory

**Coaxial Stub Theory**

A coaxial transmission line section terminated in a short circuit presents a purely reactive impedance that varies with electrical length. A short-circuit stub of electrical length θ presents input impedance:

```
Z_stub = jZ₀ tan(βl)
```

Where:
- Z₀ = characteristic impedance of stub coax (typically 50 Ω)
- β = 2π/λ (phase constant)
- l = physical length of stub

At electrical length θ = 90° (λ/4), the input impedance is theoretically infinite (parallel resonance). Connected in shunt at the antenna feedpoint, a λ/4 short-circuit stub presents a high impedance at its design frequency, passing the desired signal, while simultaneously presenting a low impedance (short circuit) at 2f₀ (where θ = 180°), acting as a harmonic trap.

**Quarter-Wave Length Calculation**

For air dielectric (velocity factor VF = 1.00):

```
         75
l  =  ──────    [meters, f in MHz]
          f
```

For PTFE-filled coax (VF = 0.66):

```
         75 × 0.66         49.5
l  =  ─────────────  =  ──────────    [meters]
             f                f
```

For foam polyethylene coax (VF = 0.84):

```
         75 × 0.84         63
l  =  ─────────────  =  ──────────    [meters]
             f               f
```

**Velocity Factor Values**

| Coax Type          | Velocity Factor | Dielectric   |
|--------------------|-----------------|--------------|
| Air (sliding stub) | 1.00            | Air          |
| Heliax (foam)      | 0.88            | Foam PE      |
| RG-8, RG-213       | 0.66–0.80       | Solid PE     |
| RG-58              | 0.66            | Solid PE     |
| PTFE coax          | 0.66–0.70       | PTFE         |
| LMR-400            | 0.85            | Foam PE      |

**Stub Lengths for VHF/UHF Bands (Air Dielectric, VF = 1.00)**

| Band    | Center Freq (MHz) | λ/4 (mm) | λ/8 (mm) | Notes                  |
|---------|------------------|----------|----------|------------------------|
| 2m      | 146              | 514 mm   | 257 mm   | Longest stub           |
| 1.25m   | 222              | 338 mm   | 169 mm   |                        |
| 70cm    | 435              | 172 mm   | 86 mm    |                        |
| 33cm    | 915              | 82 mm    | 41 mm    | SMA connectors preferable |
| 20cm    | 1270             | 59 mm    | 30 mm    | Critical tolerances    |

**Motor Position Resolution**

The stub tuner uses a NEMA 17 stepper motor (200 full steps per revolution, 1/16 microstepping = 3200 microsteps/rev) driving an M8×1.25 leadscrew (1.25 mm pitch per revolution):

```
Position resolution = 1.25 mm / 3200 microsteps = 0.000391 mm/microstep
```

At 144 MHz, λ/4 = 514 mm. Position resolution as fraction of wavelength:

```
0.000391 mm / 2060 mm (λ at 144 MHz) = 1.9 × 10⁻⁷ wavelengths = 0.000069°
```

This is electrically negligible. Even at 1300 MHz, the position resolution of 0.000391 mm corresponds to 0.00019° electrical angle — far below any practical significance.

---

## CHAPTER 2 — SPECIFICATIONS

### 2.1 HF Tuner Specifications

| Parameter                  | Value                                | Notes                                    |
|----------------------------|--------------------------------------|------------------------------------------|
| Frequency range            | 1.8–54 MHz                           | 160m through 6m                          |
| Input impedance            | 50 Ω nominal                         | Transceiver port                         |
| Power rating (continuous)  | 150W PEP                             | CW and digital modes                     |
| Power rating (SSB peak)    | 200W PEP                             | SSB duty cycle assumed                   |
| Matched impedance range    | 10–500 Ω                             | Antenna port, purely resistive           |
| Matched impedance with X   | 10–500 Ω ± 500 Ω reactance           | Reactive component can be large          |
| Output SWR after tune      | < 1.5:1 guaranteed; < 1.3:1 typical |                                          |
| Inductance range           | 0.05–80 µH                           | Roller inductor, 180 mm winding          |
| Capacitance range          | 10–500 pF                            | Variable air capacitor                   |
| Network topologies         | L (2 configs), Pi, T                 | Via SPDT relay switching                 |
| Topology switching time    | < 20 ms                              | Relay actuation time                     |
| Tuning speed (typical)     | < 8 seconds                          | From previous band preset                |
| Tuning speed (worst case)  | < 15 seconds                         | Full coarse scan required                |
| Motor type                 | NEMA 17 stepper, 42 × 42 × 40 mm    | 1.5 A rated, 200 steps/rev               |
| Motor supply voltage       | 12V DC                               | Both motors                              |
| Motor current (max)        | 1.4 A (inductor), 1.0 A (capacitor) | Set by TMC2208 Vref                      |
| Motor driver               | TMC2208 StepStick                    | 1/16 microstepping default               |
| Controller                 | ESP32-WROOM-32                       | 240 MHz dual core Xtensa LX6             |
| Flash memory               | 4 MB                                 | OTA firmware update supported            |
| PSRAM                      | None required                        |                                          |
| Band presets               | 11 bands + 50 user presets           | Stored in NVS (non-volatile)             |
| WiFi standard              | 802.11 b/g/n, 2.4 GHz                | AP mode, no external router needed       |
| WiFi SSID                  | RF-HFTuner (configurable)            | No password by default                   |
| WiFi IP address            | 192.168.4.1                          | Fixed in AP mode                         |
| HTTP port                  | 80                                   | Any web browser                          |
| Bluetooth version          | 4.2 Classic SPP                      | CYD display connection                   |
| BT device name             | RF-HFTuner (configurable)            |                                          |
| Power input                | 12V DC, 2.1 mm barrel jack           | Center positive                          |
| Power consumption          | 2A idle, 3A during tuning            | Both motors active                       |
| Logic voltage              | 3.3V (ESP32)                         | AMS1117-3.3 regulator                    |
| Intermediate rail          | 5V                                   | LM7805 for relay coils and OLED          |
| Enclosure dimensions       | 180 × 140 × 90 mm                    | 3D printed PLA or PETG                   |
| Enclosure rating           | IP54                                 | Splash-resistant gasket seal             |
| Weight (assembled)         | ~800 g                               | Including motors and capacitor           |
| Operating temperature      | −20°C to +60°C                       | Motor drivers limit upper end            |
| Storage temperature        | −40°C to +85°C                       |                                          |
| Connectors (RF)            | N-type female × 2                    | Antenna and TX ports                     |
| Connectors (motor)         | 4-pin JST-XH × 2                     | Motor A and Motor B                      |

---

### 2.2 VHF/UHF Stub Tuner Specifications

| Parameter                  | Value                                | Notes                                    |
|----------------------------|--------------------------------------|------------------------------------------|
| Frequency bands            | 144–148, 219–225, 420–450,           |                                          |
|                            | 902–928, 1240–1300 MHz               |                                          |
| Stub type                  | Air-dielectric sliding short         | Brass tube inner conductor               |
| Stub length range          | 0–155 mm                             | Motorized, leadscrew driven              |
| Position resolution        | 0.0004 mm (0.4 µm)                   | 1/16 microstepping, M8×1.25 leadscrew    |
| Insertion loss at 144 MHz  | < 0.1 dB                             |                                          |
| Insertion loss at 435 MHz  | < 0.2 dB                             |                                          |
| Insertion loss at 1296 MHz | < 0.5 dB                             | Connector quality dominant               |
| RF connectors              | N-type female × 2                    | Both ends                                |
| Motor type                 | NEMA 17 stepper                      | 42 × 42 × 40 mm                          |
| Motor supply voltage       | 12V DC                               |                                          |
| Motor current              | 0.5 A                                | Stub has low mechanical load             |
| Enclosure dimensions       | 180 × 60 × 60 mm                     | Aluminum extrusion preferred             |
| Stub coax diameter         | 12.7 mm (0.5 inch) OD                | Custom air-dielectric construction       |
| Operating temperature      | −20°C to +60°C                       |                                          |

---

### 2.3 SWR Bridge Specifications

| Parameter                  | Value                                | Notes                                    |
|----------------------------|--------------------------------------|------------------------------------------|
| Frequency range (useful)   | 1.8–200 MHz                          | Toroid response limits upper end         |
| Toroid core                | FT-50-43                             | Amidon or equivalent                     |
| Coupling winding           | 20 turns, AWG 30 enamel              | Over through-conductor                   |
| Coupling ratio             | −20 dB (1:100 power)                 |                                          |
| Directivity                | > 30 dB across HF range              |                                          |
| Detector type              | BAT46 Schottky diode                 | 2 diodes (forward and reflected)         |
| Detector forward voltage   | 0.2 V typical                        | At 1 mA detector current                 |
| ADC                        | ESP32 internal, 12-bit               | 0–3.3V, 4096 counts                      |
| ADC sample rate            | 10 samples/second (averaged)         | 50 ms moving average                     |
| Power measurement range    | 0–200W forward                       | At TX port into 50 Ω                     |
| Power resolution           | ~0.05W                               | ADC limited                              |
| SWR display resolution     | 0.1:1                                |                                          |
| SWR accuracy               | ±0.2:1 at SWR < 3:1                  | After calibration                        |
| Insertion loss              | < 0.1 dB                             | Across HF range                          |
| Maximum input power        | 200W                                 | Determined by coupling resistor rating   |

---

## CHAPTER 3 — CONTROLS AND INDICATORS

### 3.1 Front Panel Description

```
╔═══════════════════════════════════════════════════════════════════════╗
║  RF HF/VHF ANTENNA TUNER                              TM-HFTN-001    ║
║                                                                       ║
║  [SWR]  [STATUS]   ┌──────────────────────────┐  [ENCODER]  [SPEED]  ║
║   LED     LED      │  SWR: 1.45:1             │    /───\     ( )     ║
║   (●)     (●)      │  L:  15.3 µH   (042%)    │   │     │           ║
║                    │  C: 245 pF     (048%)    │    \───/             ║
║                    │  BAND: 40m  7.150 MHz    │   PRESS=TUNE        ║
║                    └──────────────────────────┘                      ║
║                                                                       ║
║   [ANT ●──]                                    [── TX ●]             ║
║   N-female                                     N-female              ║
╚═══════════════════════════════════════════════════════════════════════╝
```

**SWR LED (left)**
- Illuminates with brightness proportional to SWR
- Bright = high SWR, dim = low SWR
- LED off = SWR < 1.1:1 (essentially perfect match)

**STATUS LED (second from left)**
- Green (steady): matched, SWR < 1.5:1
- Orange (flashing 2 Hz): tuning in progress
- Red (steady): fault condition (see Chapter 10)
- Blue (brief flash on power-up): system initializing

**OLED Display (128×64 pixels, SSD1306)**
- Line 1: SWR reading with one decimal place (e.g., "SWR: 1.45:1")
- Line 2: Inductor position in µH and percent of range (e.g., "L: 15.3 µH (042%)")
- Line 3: Capacitor position in pF and percent (e.g., "C: 245 pF (048%)")
- Line 4: Band name and frequency (e.g., "BAND: 40m 7.150 MHz")
- During tuning: line 4 alternates with "TUNING..." message
- During fault: all lines replaced by fault code and description

**Rotary Encoder**
- Rotate clockwise: increment current selection (band, L, or C)
- Rotate counterclockwise: decrement current selection
- Short press (< 0.5 sec): initiate automatic tuning (same as pressing TUNE button on web interface)
- Long press (> 1.5 sec): enter menu mode (MANUAL / SAVE / BAND SELECT / CONFIG)
- Double press: toggle between L and C control in manual mode

**Speed Potentiometer**
- Controls motor step rate during manual tuning
- Fully counterclockwise: slowest (fine adjustment, ~10 steps/sec)
- Fully clockwise: fastest (coarse adjustment, ~800 steps/sec)
- No effect during auto-tune (auto-tune uses fixed speed from config)

**ANT Connector (N-type female, left)**
- Connect to antenna or antenna feedline
- Maximum input power: 200W PEP
- Do not transmit without antenna or dummy load connected

**TX Connector (N-type female, right)**
- Connect to transceiver RF output
- Maximum input power from transceiver: 200W PEP
- 50 Ω nominal impedance

---

### 3.2 Rear Panel Description

```
╔═══════════════════════════════════════════════════════════════════════╗
║                                                                       ║
║  [MOTOR A]      [MOTOR B]      [USB]         [12V DC]                ║
║  JST-XH 4-pin  JST-XH 4-pin  Mini-USB       2.1mm Barrel            ║
║  Roller Ind.   Var. Cap.      ESP32 Prog.    (+) Center              ║
║                                                                       ║
╚═══════════════════════════════════════════════════════════════════════╝
```

**MOTOR A (Roller Inductor)**
- 4-wire stepper motor connection
- Wire color code: Black=A+, Green=A−, Red=B+, Blue=B−
- Maximum cable length: 1 meter (longer cables require series ferrite beads)
- JST-XH 2.54 mm pitch, 4-position

**MOTOR B (Variable Capacitor)**
- Same electrical specification as Motor A
- Lower current motor (1.0 A vs. 1.4 A for Motor A)
- Same cable length limitation

**USB Port (Mini-USB)**
- ESP32 programming and debug serial console
- 115200 baud, 8N1
- Powers ESP32 logic when 12V not applied (motors not operational)
- Use for firmware upload only; do not leave USB connected during RF operation (ground loop risk)

**12V DC Input (2.1 mm barrel jack)**
- Center positive, outer ring negative
- 12V regulated DC, 3A minimum recommended
- Absolute maximum: 15V
- Absolute minimum: 10V (below this, motors stall under load)
- Inline 3A automotive fuse recommended between supply and tuner

---

### 3.3 CYD Display Modes

The CYD (ESP32-2432S028R) touchscreen provides a 320×240 color touchscreen interface. Communication is via Bluetooth SPP. The display has four primary operating modes, selectable by tapping the mode button at the top of the screen.

**Mode 1: TUNE Mode (default)**

The TUNE mode is the primary operating display. It shows:

- Analog SWR meter: full-width semicircular gauge, green zone 1–2:1, yellow zone 2–3:1, red zone above 3:1. Needle animates in real time.
- L position bar: horizontal bar showing inductor position (0–100%), numerical µH value displayed.
- C position bar: horizontal bar showing capacitor position (0–100%), numerical pF value displayed.
- Band indicator: large font band name and frequency.
- TUNE button: large green button at bottom. Tap to initiate auto-tune. Button turns orange with "TUNING..." text during active tune. Returns to green on match or red on fault.
- STOP button: appears during tuning to halt auto-tune immediately.

**Mode 2: BANDS Mode**

A grid of 11 band buttons, one per amateur HF band plus 60m:

| 160m | 80m | 60m |
|------|-----|-----|
| 40m  | 30m | 20m |
| 17m  | 15m | 12m |
| 10m  | 6m  |     |

Each button shows the stored SWR for that band preset (or "---" if no preset saved). Tapping a band button selects that band and moves motors to the stored preset position. If no preset exists, the TUNE button is highlighted prompting the operator to tune.

**Mode 3: MATCH Mode**

Displays a schematic diagram of the currently active network topology (L, Pi, or T), with the current L and C values annotated on the diagram. Shows computed source and load impedance. Useful for verifying the network is behaving as expected and for manual network calculations.

**Mode 4: WIFI Mode**

Displays WiFi status information:
- SSID: RF-HFTuner (or configured name)
- IP address: 192.168.4.1
- Connected clients: number of devices currently connected to the ESP32 AP
- Signal strength: not applicable in AP mode (shows "AP mode — clients shown")
- BT status: paired device name, connection state, packet count

---

### 3.4 Web Interface

The ESP32 HTTP server serves a single-page application on port 80. The interface is compatible with any modern web browser (Chrome, Firefox, Safari, Edge) on any device (phone, tablet, laptop). No app installation required.

**Main Page (/)**

The main page displays:
- SWR reading: large font, updates every 500 ms via WebSocket
- Forward power reading in watts
- TUNE button: sends POST /api/tune when pressed
- STOP button: sends POST /api/stop
- Manual L slider: range 0–100%, sends POST /api/setL in real time
- Manual C slider: range 0–100%, sends POST /api/setC in real time
- Network topology selector: L-Hi, L-Lo, Pi, T (radio buttons, sends POST /api/topology)
- Band indicator showing current band

**Bands Page (/bands)**

Identical layout to the CYD BANDS mode. 11 band buttons with stored SWR values. Click to select band and recall preset.

**Config Page (/config)**

Provides access to tuner operating parameters:
- Auto-tune SWR threshold: the target SWR for considering a match achieved (default 1.5:1)
- Coarse scan steps: number of L steps in coarse scan (default 20, range 5–50)
- Motor speed (auto-tune): steps per second during auto-tune (default 400)
- Motor speed (manual): steps per second during manual operation (default 200)
- WiFi SSID: configurable AP name
- WiFi password: optional AP password (blank = open)
- BT device name: configurable Bluetooth name

Changes take effect immediately and are saved to NVS.

**Calibrate Page (/calibrate)**

Step-by-step calibration wizard:
- SWR bridge calibration (see Chapter 6.1)
- Inductor position calibration (see Chapter 6.2)
- Capacitor position calibration (see Chapter 6.3)
- ADC zero calibration (no RF, reads noise floor)
- Save calibration data

**Status Page (/api/status)**

Returns a JSON object with the full system state. Example response:

```json
{
  "swr": 1.45,
  "fwd_power_w": 98.3,
  "ref_power_w": 2.1,
  "l_pos_pct": 42,
  "l_val_uh": 15.3,
  "c_pos_pct": 48,
  "c_val_pf": 245,
  "band": "40m",
  "freq_mhz": 7.150,
  "topology": "L-Hi",
  "tuning": false,
  "fault_code": 0,
  "fault_msg": "OK",
  "uptime_sec": 3600
}
```

This endpoint is polled by the main page WebSocket handler and can also be accessed by external automation scripts.

---

## CHAPTER 4 — INSTALLATION

### 4.1 Mounting Options

The tuner enclosure accepts four mounting configurations. Select based on the installation environment.

**Table-Top**

The base plate has four M5 tapped holes for rubber bumper feet. Feet are included. Place on any stable, flat surface with adequate ventilation (25 mm minimum clearance on all sides). Do not place on top of the transceiver — the motor vibration can interfere with the transceiver's cooling fans and the RF cables must be short on the transceiver side.

**Rack Mount**

A 1U (44.45 mm) rack mounting panel is available. The panel accepts two tuner units side by side in a standard 19-inch rack. Four M6 rack screws (not supplied) secure the panel to the rack. Verify rack depth ≥ 250 mm to accommodate motor and connector clearances at the rear.

**DIN Rail**

The `enclosure.scad` file includes a `din_rail_clip()` module that prints clips for 35 mm standard DIN rail. Print two clips per unit. Clips engage the upper and lower DIN rail flanges and hold the enclosure firmly. This mount is suitable for equipment cabinets and small control panels.

**Field Portable**

Two M4 tapped mounting points on each side panel accept webbing strap attachments. Thread 25 mm nylon webbing through these points to form a carrying strap. When field-mounted (lashed to a mast base or equipment case), verify the IP54 seal is intact — the gasket is a 3 mm closed-cell foam strip compressed by the lid screws. Check the gasket before each field deployment.

---

### 4.2 RF Connections

**Antenna Port (ANT)**

The ANT port accepts the feedline from the antenna. This may be coaxial cable terminated in an N-to-PL-259 adapter, direct N-type coax, or open-wire feedline via a 4:1 balun (see operating notes in Appendix B for open-wire feedline recommendations). The tuner matches impedances from 10 to 500 ohms at this port. Maximum power into the antenna port in any mismatch condition: 200W.

**Transceiver Port (TX)**

The TX port connects to the transceiver RF output. Use the shortest practical coax run to minimize feedline loss between transceiver and tuner. The tuner presents 50 Ω at this port after matching; the transceiver sees a low-SWR load regardless of antenna impedance.

**Maximum Coax Length**

There is no maximum coax length between the tuner and the antenna. The tuner matches at its input; the loss of the coax beyond the tuner is a fixed loss at the operating frequency, not a mismatch loss. However, minimize feedline loss by using low-loss coax (LMR-400 or equivalent) especially on 10m and 6m.

**PL-259 Adapters**

Use silver-plated N-to-UHF (N-to-PL-259) adapters where the transceiver has a PL-259 (SO-239) output. Verify the adapter is tight and the center pin makes solid contact. Loose PL-259 connections are a common source of intermittent high-SWR readings.

---

### 4.3 Power Connections

**Supply Requirements**

The tuner requires 12V DC regulated. Supply must maintain voltage above 10V under the full motor load (3A peak during dual-motor tuning transitions). Switching supplies generally meet this requirement; linear wall-wart supplies often sag under the motor current peaks and are not recommended.

**Polarity**

Center pin is positive (+12V), outer barrel is negative (ground). Reverse polarity will not damage the unit if a protection diode is present on the PCB (verify in the schematic, Appendix A.2). However, do not rely on polarity protection — verify the supply polarity before connecting.

**Fusing**

Install a 3A inline blade fuse on the positive 12V supply wire, within 300 mm of the power supply output terminals. A blown fuse indicates excessive current draw — investigate before replacing. Common causes: motor jam, TMC2208 driver fault, or short circuit in wiring.

**Power Supply Recommendation**

A 12V 5A switching supply (Meanwell LRS-60-12 or equivalent) provides adequate current margin for dual-motor operation and leaves headroom for future accessories. Do not use a car battery charger as a bench supply — output voltage varies and can spike above 15V.

---

### 4.4 Motor Cable Routing

**Maximum Cable Length**

Motor cables longer than 1 meter are susceptible to induced RF voltage from the nearby antenna feedline. RF pickup on motor cables can cause erratic stepping behavior. Keep motor cables ≤ 1 meter.

**Routing**

Route motor cables perpendicular to RF cables where they must cross. Do not bundle motor cables alongside RF feedlines. Use cable ties to secure motor cables away from the SWR bridge assembly.

**RFI Mitigation**

If erratic motor behavior is observed during transmission, slip a Fair-Rite 31-material snap-on ferrite choke (two turns) over each motor cable near the motor connector. This typically resolves the issue. Part: Fair-Rite 0431480801 or equivalent.

---

### 4.5 WiFi Setup

On power-up, the ESP32 creates a WiFi access point automatically. No configuration is required.

1. The WiFi AP SSID is `RF-HFTuner` (or the configured name from the Config page).
2. The AP has no WPA password by default (open network).
3. Connect any WiFi device to the `RF-HFTuner` network.
4. Open a web browser and navigate to `http://192.168.4.1`
5. The main tuner control page loads immediately.
6. No internet connection is required. The web interface is served entirely from the ESP32 flash memory.

To set a WiFi password or change the SSID, navigate to the Config page, make changes, and save. The ESP32 restarts the AP with the new settings.

---

### 4.6 Bluetooth Setup

**CYD Display Pairing**

The CYD display runs firmware from the `cyd_tuner_display/` project. On first power-up with both devices present:

1. Power on the main tuner unit (ESP32 starts Bluetooth SPP advertising as `RF-HFTuner`).
2. Power on the CYD display.
3. The CYD firmware scans for the `RF-HFTuner` Bluetooth device and initiates pairing automatically.
4. Pairing completes within 10 seconds. The CYD status bar shows "BT: Connected".
5. On subsequent power-ups, both devices remember the pairing and reconnect automatically within 5 seconds.

**If Pairing Fails**

If the CYD shows "BT: Disconnected" for more than 30 seconds:

1. Power cycle both the main tuner unit and the CYD display.
2. If the second attempt fails, navigate to the CYD settings menu (long-press the title bar) → BT Settings → Forget Device → Rescan.
3. Verify the main tuner ESP32 is advertising (status LED should be flashing blue during BT advertisement period on startup).
4. If the main unit is connected to a different BT device, the pairing may be blocked. Reset BT from the web Config page.

**Bluetooth SPP Range**

Bluetooth SPP (Classic, not BLE) range is typically 10 meters line-of-sight. Walls and equipment reduce range. For field use at 10+ meter separation, consider a wired USB serial extension if Bluetooth is unreliable.

---

## CHAPTER 5 — OPERATING PROCEDURES

### 5.1 Initial Power-Up

Perform this procedure on first use and after any maintenance that involved disconnecting internal wiring.

1. Verify antenna is connected to ANT port (or dummy load for bench testing). Never power up with RF ports open-circuit — the SWR bridge expects a terminated port.
2. Verify transceiver is connected to TX port.
3. Verify 12V DC power supply is correct polarity (center positive) and is off.
4. Connect 12V DC supply cable to the barrel jack on the rear panel.
5. Turn on the 12V power supply.
6. Observe STATUS LED: should flash green three times within 2 seconds.
7. OLED displays: "RF HF TUNER" (line 1), "INITIALIZING" (line 2).
8. Motor homing sequence begins automatically. The roller inductor motor drives toward minimum inductance (minimum step count from limit switch) and the capacitor motor drives toward minimum capacitance. This takes approximately 5 seconds. Do not interrupt power during homing.
9. OLED displays the operating screen with current band, SWR reading, L and C positions.
10. STATUS LED illuminates green (if SWR < 1.5:1) or orange (if SWR > 1.5:1, indicating a tune is recommended).
11. Connect any WiFi device to RF-HFTuner network and verify web interface is accessible at 192.168.4.1.
12. System is ready for operation.

---

### 5.2 Automatic Tuning

Automatic tuning is the normal operating mode. The operator initiates a tune; the firmware handles all motor movement and SWR optimization without further input.

1. Select the desired operating band on the transceiver.
2. On the tuner, select the matching band using the rotary encoder (rotate to scroll through bands) or via the web interface Bands page.
3. If a band preset exists from a previous tune, the tuner moves to the stored position automatically. The OLED shows the stored SWR for that preset.
4. Key the transceiver at reduced power — 10 watts or less is recommended for the initial tune. Higher power during tuning causes excessive SWR transients and can stress the transceiver output stage.
5. Press the TUNE button (rotary encoder short press, or TUNE button on web interface, or TUNE on CYD touchscreen).
6. STATUS LED changes to orange, flashing at 2 Hz.
7. OLED line 4 shows "TUNING..." alternating with current SWR.
8. The auto-tune algorithm executes (see Section 1.7 for algorithm detail).
9. When SWR < 1.5:1, STATUS LED turns green (steady) and a single tone sounds on the buzzer (if fitted).
10. Release the PTT on the transceiver.
11. Increase power to normal operating level.
12. Verify SWR remains low at operating power. If SWR increases significantly at full power (antenna impedance varies with power due to ground conductivity changes), press TUNE again at operating power for a fine adjustment.
13. The tuned position is automatically saved as the band preset for this band.

> **CAUTION:** Do not operate the tuner at full power while tuning is in progress. The variable capacitor and roller inductor experience maximum voltage and current stresses during a mismatch condition. Tuning at 10W limits maximum component stress to safe levels.

---

### 5.3 Manual Tuning

Manual tuning allows the operator to directly control inductor and capacitor positions. This is useful for:
- Verifying the auto-tune result is the true minimum SWR
- Making small adjustments when the antenna impedance drifts (e.g., due to wind loading or precipitation)
- Educational experimentation

1. Enter MANUAL mode: long-press the rotary encoder, then select MANUAL from the menu.
2. OLED line 4 shows "MANUAL: [L]" indicating the encoder controls L.
3. Rotate the encoder clockwise to increase inductance, counterclockwise to decrease.
4. Double-press encoder to toggle between L control and C control.
5. Monitor SWR on OLED display (or CYD or web interface).
6. Minimize SWR by adjusting L first (coarse), then C (fine), then iterate.
7. To save the position: long-press encoder → SAVE → rotate to confirm → short-press to execute.
8. To return to automatic mode: long-press encoder → AUTO.

---

### 5.4 Band Changing Procedure

When changing operating frequency to a new amateur band:

1. Change band on the transceiver.
2. On the tuner, rotate the encoder to select the new band name, or tap the new band button on the CYD BANDS page, or click the band on the web interface Bands page.
3. The tuner motors move to the stored preset position for that band.
4. OLED shows the stored SWR for that band preset.
5. If the stored SWR was good (< 1.5:1), the match is likely still adequate — no tune required. Verify SWR with a brief low-power transmission.
6. If no preset exists, the OLED shows "---" for SWR and the STATUS LED is orange.
7. Press TUNE at 10W to find a new match for this band.
8. The new match position is automatically saved as the band preset.
9. SWR readings may be slightly different from stored values if antenna conditions have changed (precipitation, temperature change, antenna movement). Re-tune as needed.

---

### 5.5 VHF/UHF Stub Tuner Operation

The VHF/UHF stub tuner is a separate physical unit. It is connected in series with the antenna feedline on the appropriate VHF or UHF band antenna.

1. Connect the VHF/UHF antenna to the ANT port of the stub tuner unit.
2. Connect the transceiver (or VHF/UHF rig) to the TX port.
3. Apply 12V power to the stub tuner.
4. Connect to the stub tuner web interface (separate ESP32, SSID: RF-StubTuner, IP: 192.168.4.1) or use the shared CYD display (the CYD firmware supports both tuner units via Bluetooth device selection).
5. Select the operating band from the web interface.
6. Press TUNE. The stub motor drives to the calculated λ/4 position for the selected band.
7. Press TUNE again (or transmit at low power to enable SWR measurement) to initiate SWR-based fine tuning.
8. Fine adjustment: the stub position is adjusted in small increments while monitoring SWR. The algorithm minimizes SWR by comparing positions ±2 mm from the current location and stepping toward minimum.
9. Save position as band preset when satisfied.

---

### 5.6 Operating Limits

> **WARNING: Maximum Power**
> The maximum continuous power input is 150W PEP (CW and digital modes). SSB peak power may reach 200W PEP. Exceeding 200W will overvoltage the variable capacitor at minimum capacitance settings on lower bands and will overheat the roller inductor contact. Do not exceed these limits.

> **CAUTION: Tune at Reduced Power**
> Always initiate auto-tune with ≤ 10W transmitter output. High-SWR conditions during tuning cause elevated voltages on capacitor vanes. The auto-tune algorithm is designed to be conducted at reduced power.

> **WARNING: High SWR Protection**
> If SWR exceeds 10:1 at any point, the tuner sets FAULT F01, stops all motor movement, and illuminates the STATUS LED red. This prevents transmission into a likely open or short circuit (disconnected antenna). Clear the fault by resolving the antenna connection issue, then press RESET on the web Config page.

> **CAUTION: Motor Overtemperature**
> The TMC2208 stepper drivers include thermal shutdown at approximately 150°C junction temperature. If the motor drivers reach this temperature during extended operation in a warm environment, they shut down and FAULT F05 is set. Allow the drivers to cool for 10 minutes before resuming operation. Verify Vref settings are correct (see Section 11.4) — excess Vref causes excessive motor heating.

> **CAUTION: Antenna Connection Required**
> Never operate the tuner without an antenna or dummy load connected to the ANT port. The Bruene bridge requires a terminated port to function correctly. With an open port, the SWR bridge may read erratic values and the auto-tune algorithm may make incorrect decisions.

---

## CHAPTER 6 — CALIBRATION PROCEDURES

### 6.1 SWR Bridge Calibration

**Equipment Required**
- 50 Ω dummy load (Bird, Palstar, or equivalent), rated ≥ 150W
- Transceiver with power output adjustable to 10W and 50W
- Optional: calibrated directional power meter (e.g., Bird 43 with slug)

**Procedure**

1. Connect 50 Ω dummy load to the ANT port.
2. Connect transceiver to TX port.
3. Apply 12V power. Connect to the web interface.
4. Navigate to Calibrate page → SWR Bridge Calibration.
5. Transmit at exactly 10W (use the transceiver's power output control; verify with external meter if available).
6. Note the forward ADC count shown on the calibrate page (raw ADC value, 0–4095).
7. Enter 10 in the "Forward Power (W)" field and click Set Forward Cal Point.
8. Increase power to 50W. Verify the forward power reading updates proportionally (should read ~50W). This is a linearity check — if the reading is significantly nonlinear, the ADC may be saturating or the detector diode is not in its linear region.
9. Stop transmitting.
10. For reflected calibration: disconnect the dummy load from the ANT port. Connect a 200 Ω resistor (3W or more) in its place.
11. Transmit at 10W. The expected SWR is (200/50) = 4.0:1. The tuner should display approximately 4.0:1 on the OLED.
12. If the displayed SWR differs from 4.0:1 by more than ±0.3, adjust the VREF offset in the calibrate page until the reading matches the calculated SWR.
13. Reconnect the 50 Ω dummy load. Verify SWR reads 1.0:1.
14. Click Save Calibration.

---

### 6.2 Inductor Position Calibration

**Equipment Required**
- LCR meter (e.g., DE-5000) or NanoVNA capable of inductance measurement
- 1 kHz reference signal source (most LCR meters provide this)

**Procedure**

1. Connect to the web interface. Navigate to Calibrate page → Inductor Position.
2. Click HOME. The inductor motor drives to the minimum inductance position (limit switch contact). Observe OLED; L position should show 0.00 µH.
3. If OLED shows a nonzero value at the home position, click Set Zero at Current Position.
4. Using the web interface L slider, move the inductor to maximum position (100%).
5. Disconnect motor power (click Motor Off on calibrate page) to prevent the motor from moving during measurement.
6. Connect LCR meter probes to the roller inductor terminals (accessible at the motor coupling end of the inductor assembly via test points TP1 and TP2).
7. Measure inductance at 1 kHz.
8. Record measured L_max value (expected approximately 78–82 µH for correct winding).
9. Enter measured L_max in the "L Max (µH)" field on the calibrate page.
10. Click Calculate and Save. The firmware recalculates the µH-per-step conversion factor.
11. Re-enable motor power (click Motor On).
12. Move L to 50% using the slider. Read the OLED µH value. Measure inductance with LCR meter. Verify the displayed value is within ±10% of the measured value.

---

### 6.3 Capacitor Position Calibration

**Equipment Required**
- NanoVNA (for capacitance measurement via reactance at a known frequency)
- Or LCR meter with capacitance range covering 10–500 pF

**Procedure**

1. Navigate to Calibrate page → Capacitor Position.
2. Click HOME. Capacitor drives to minimum capacitance (vanes disengaged). OLED shows C = 0 pF (minimum).
3. With NanoVNA: measure reactance at the capacitor terminals at 10 MHz. Compute C = 1/(2π × f × |X_C|). Record as C_min.
4. Set C_min in calibrate page.
5. Move capacitor to maximum (100% slider).
6. Measure capacitance at maximum engagement. Record as C_max (expected ~470–510 pF).
7. Set C_max in calibrate page.
8. Click Save.
9. Verify midpoint (50%): display should show approximately (C_min + C_max) / 2. Measure actual capacitance and verify within ±15%.

---

### 6.4 Band Preset Calibration

After calibrating the SWR bridge and position sensors, verify each band preset against an external reference SWR meter.

For each band in use:

1. Connect a representative antenna for that band (or a resistive load close to the expected antenna impedance).
2. Select the band on the tuner.
3. Press TUNE at 10W. Allow the tuner to find a match.
4. After tuning, transmit at operating power and read the SWR on an external directional power meter in the feedline between transceiver and tuner TX port.
5. Compare the external SWR reading to the tuner's displayed SWR.
6. If the difference exceeds ±0.2:1, the SWR bridge calibration should be repeated (Section 6.1) with particular attention to the reflected calibration step.
7. Band presets with accurate SWR readings can be relied upon for subsequent operations.

---

### 6.5 NanoVNA Integration

The NanoVNA provides swept frequency measurement capability not available from the SWR bridge alone. Use the NanoVNA to characterize the tuner's insertion loss and to verify the matching network topology.

**Insertion Loss Test (Tuner Bypass / Matched)**

1. Connect NanoVNA port 1 to the TX port of the tuner.
2. Connect NanoVNA port 2 to the ANT port of the tuner.
3. Connect a 50 Ω termination to the NanoVNA's reference port (or use the through calibration).
4. Perform SOLT calibration at the cable ends.
5. Tune the tuner to match a 50 Ω load (SWR = 1.0:1 condition — connect 50 Ω dummy to antenna port).
6. Sweep 1.8–54 MHz. Observe S21 (insertion loss).
7. Expected result: S21 better than −0.5 dB across all HF bands (< 0.5 dB insertion loss).
8. Check for unexpected dips in S21 that would indicate spurious resonances in the tuner network. These should not be present when the tuner is properly tuned.

**Smith Chart Verification**

1. With the tuner set for a specific antenna match, view the S11 Smith chart at the TX port.
2. The impedance at the TX port should appear near the center of the Smith chart (50 Ω ± tolerance) across the band for which the tuner is matched.
3. At frequencies far from the tuned band, the impedance will deviate from center — this is expected.
4. If the S11 trace spirals outward from center within the tuned band, the tuner is introducing excessive reactance and the motor positions may need recalibration.

---

## CHAPTER 7 — ANTENNA NEC MODELS

### 7.1 NEC-2 Model File Usage

NEC-2 (Numerical Electromagnetics Code) antenna models are provided in the `nec_models/` directory within the motorized_tuner project. These models characterize the impedance of typical antennas used with this tuner, providing the R + jX values at each operating frequency. These impedance values drive the matching network design decisions for each band.

**Supported NEC Simulation Programs**

| Program      | Platform       | Download                             |
|--------------|----------------|--------------------------------------|
| 4NEC2        | Windows        | www.qsl.net/4nec2                    |
| MMANA-GAL    | Windows        | www.qsl.net/mmana                    |
| xnec2c       | Linux/macOS    | sudo apt install xnec2c              |
| nec2++       | Linux (CLI)    | sudo apt install necpp               |
| cocoaNEC     | macOS          | cocoaNEC.software                    |

**Installing xnec2c on Debian/Ubuntu:**

```
sudo apt install xnec2c
xnec2c nec_models/dipole_40m.nec
```

---

### 7.2 Available Models

| Model File                     | Antenna Type              | Frequency    | Notes                        |
|-------------------------------|---------------------------|--------------|------------------------------|
| dipole_160m.nec               | Half-wave dipole          | 1.8–2.0 MHz  | 15m height, real ground      |
| dipole_80m.nec                | Half-wave dipole          | 3.5–4.0 MHz  | 12m height, real ground      |
| dipole_40m.nec                | Half-wave dipole          | 7.0–7.3 MHz  | 12m height, real ground      |
| vertical_40m_radials.nec      | λ/4 vertical              | 7.0–7.3 MHz  | 8 radials, real ground       |
| loaded_vertical_hf.nec        | 5m base-loaded whip       | 7.15 MHz     | 15 µH loading coil           |
| end_fed_halfwave_80m.nec      | EFHW 80m                  | 3.5 MHz      | 49:1 transformer model       |
| yagi_2m_7el.nec               | 7-element Yagi            | 144.2 MHz    | Free space, 12 dBd gain      |
| ground_plane_vhf.nec          | 5/8 wave vertical         | 146 MHz      | 3 radials, elevated          |
| magnetic_loop_40m.nec         | Small transmitting loop   | 7.15 MHz     | 1m diameter copper           |
| fan_dipole_multiband.nec      | Fan dipole 40/20/10m      | 7/14/28 MHz  | Three resonant elements      |

---

### 7.3 Interpreting NEC Results for Tuner Design

**Key NEC Output Parameters**

The critical output from a NEC model for tuner design is the **feed-point impedance** at the operating frequency:

```
Z_antenna = R + jX  (Ohms)
```

Where R is the radiation resistance plus loss resistance, and X is the net reactance (positive = inductive, negative = capacitive).

**Decision Chart: Selecting Network Topology**

Given the NEC impedance result:

| Condition                         | Recommended Network             | Comments                           |
|-----------------------------------|---------------------------------|------------------------------------|
| R ≈ 50, X small (< ±20 Ω)        | None (no tuner needed)          | Antenna nearly resonant            |
| R < 50, X small                   | L-network, shunt at input       | Short vertical, loaded antenna     |
| R > 50, X small                   | L-network, shunt at output      | Long dipole, trapped vertical      |
| R < 50, X large positive          | T-network                       | Inductive load, short antenna      |
| R > 50, X large                   | Pi-network                      | High-Z end-fed antenna             |
| R very high (> 300), X large      | T-network with series cap       | End-fed at current minimum         |

**Worked Calculation: Loaded Vertical at 7.15 MHz**

NEC model output for `loaded_vertical_hf.nec`:

```
Frequency: 7.150 MHz
Z_antenna = 15 + j40 Ω
```

Design the matching network:

1. R_L = 15 Ω (antenna), R_H = 50 Ω (system impedance)
2. Select L-network with shunt element at input (since Z_antenna < 50 Ω):

```
Q = √(R_H / R_L − 1) = √(50/15 − 1) = √(2.333) = 1.527
```

3. Shunt element (capacitor at input, since shunt-to-ground must present capacitive reactance to cancel inductive tendency):

```
X_shunt = R_H / Q = 50 / 1.527 = 32.7 Ω  (capacitive)
```

4. Series element (inductor in series line):

```
X_series = Q × R_L = 1.527 × 15 = 22.9 Ω (inductive)
X_series_net = 22.9 − j40 (must cancel antenna's +j40 reactance)
X_series_net = 22.9 − 40 = −17.1 Ω  → net capacitive
```

Wait — the series reactance required is net capacitive (−17.1 Ω) to cancel the inductive antenna reactance. In this case, the series element should be a capacitor:

```
C_series = 1/(2π × 7.15 × 10⁶ × 17.1) = 1303 pF
```

5. Shunt capacitor at input:

```
C_shunt = 1/(2π × 7.15 × 10⁶ × 32.7) = 681 pF
```

The tuner sets C to approximately 681 pF and L to near minimum (just enough to present the correct series inductive reactance needed to reach the series capacitor equivalent). In practice, the auto-tune algorithm finds the exact position experimentally, with the NEC model result serving as the starting estimate for the preset table.

---

### 7.4 Running NEC Models on Debian Linux

**Command-Line (nec2++ / necpp)**

```bash
sudo apt install necpp
nec2++ -i nec_models/dipole_40m.nec -o dipole_40m.out
```

Parse the output file for "IMPEDANCE DATA" section to find R + jX at each frequency.

**Graphical (xnec2c)**

```bash
sudo apt install xnec2c
xnec2c nec_models/dipole_40m.nec
```

Within xnec2c:
- View impedance plot: Calculations menu → Impedance
- View VSWR: Calculations menu → VSWR
- Change frequency: use frequency sweep controls at top of main window
- Export data: File → Save → Numerics data (saves CSV of impedance vs. frequency)

**Modifying Models**

The .nec files are plain text. Edit the `FR` (frequency) card to sweep the desired frequency range. Edit the `GH` or `GW` (wire geometry) cards to change antenna dimensions. Refer to the NEC-2 User Manual (available from Lawrence Livermore National Laboratory) for card format documentation.

---

## CHAPTER 8 — FIRMWARE ARCHITECTURE

### 8.1 ESP32 Firmware Structure

The ESP32 firmware is an Arduino IDE project located in `firmware/esp32_hf_tuner/`:

```
esp32_hf_tuner/
├── esp32_hf_tuner.ino      Main sketch: setup(), loop(), task registration
├── config.h                Pin definitions, band table, tuner parameters, constants
├── swr.h                   SWRMeter class (ADC sampling, SWR calculation)
│                           AutoTuner class (state machine, coarse scan, fine tune)
└── tuner.h                 BandManager class (band table, preset storage/recall)
                            MotorController class (step generation, homing)
                            NetworkTopology class (relay control, topology switching)
```

**Task Structure (FreeRTOS)**

| Task Name        | Core | Priority | Stack  | Period    | Function                              |
|------------------|------|----------|--------|-----------|---------------------------------------|
| SWRSampleTask    | 0    | 5 (high) | 4096   | 100 ms    | ADC sampling, SWR calculation         |
| AutoTuneTask     | 1    | 3        | 8192   | Event     | Auto-tune state machine               |
| WiFiServerTask   | 0    | 2        | 8192   | Event     | HTTP request handling                 |
| BTCommTask       | 1    | 3        | 4096   | Event     | Bluetooth SPP tx/rx                   |
| OLEDUpdateTask   | 1    | 1 (low)  | 4096   | 200 ms    | OLED display refresh                  |
| MotorTask        | 1    | 4        | 4096   | 1 ms      | Step pulse generation                 |

Note: The SWRSampleTask runs at high priority on core 0 and is never blocked by the auto-tune algorithm. This ensures SWR monitoring continues even when the auto-tune algorithm is running multi-second scans.

---

### 8.2 AutoTuner State Machine

```
                                 ┌───────────────────────────────────────────┐
                                 │              AT_IDLE                      │
                                 │  Motors stationary, SWR monitoring active │
                                 └───────────────┬───────────────────────────┘
                                                 │ TUNE command received
                                 ┌───────────────▼───────────────────────────┐
                                 │          AT_PRESET_CHECK                  │
                                 │  Try stored preset if available           │
                                 └───────────────┬───────────────────────────┘
                                         SWR OK  │  SWR not OK or no preset
                             ┌───────────────────┤
                             │                   │
               ┌─────────────▼────────┐  ┌───────▼──────────────────────────────┐
               │    AT_MATCHED        │  │         AT_COARSE_SCAN               │
               │  Preset accepted     │  │  Sweep L (20 steps)                  │
               │  Save if new best    │  │  Binary search C at each L           │
               └──────────────────────┘  │  Record best (L,C) pair              │
                                         └───────────────┬──────────────────────┘
                                                         │ Scan complete
                                         ┌───────────────▼──────────────────────┐
                                         │           AT_MOVE_BEST               │
                                         │  Move motors to best coarse position  │
                                         └───────────────┬──────────────────────┘
                                                         │
                                         ┌───────────────▼──────────────────────┐
                                         │           AT_FINE_TUNE               │
                                         │  Gradient descent: adjust L, then C  │
                                         │  Repeat until convergence            │
                                         └───────────┬─────────────┬────────────┘
                                                     │             │
                                          SWR < 1.5  │             │  > 50 iters
                                                     │             │  or SWR > 10
                             ┌───────────────────────▼──┐  ┌───────▼──────────────┐
                             │       AT_MATCHED          │  │    AT_FAULT          │
                             │  Status LED green         │  │  Status LED red      │
                             │  Save band preset         │  │  Display fault code  │
                             │  Return to AT_IDLE        │  │  Await RESET command │
                             └───────────────────────────┘  └──────────────────────┘
                                                     ▲
                                    SWR degrades     │ (periodic recheck)
                             AT_MATCHED ─────────────┘
```

Note: From AT_MATCHED, the SWRSampleTask continues to monitor SWR. If SWR exceeds 2.5:1 (indicating the antenna has changed, e.g., wind shifting a portable antenna), the state reverts to AT_IDLE and the operator is prompted (STATUS LED turns orange) to initiate a new tune.

---

### 8.3 BT SPP Command Reference

The Bluetooth SPP interface accepts ASCII text commands terminated by CR (carriage return, \r) or CRLF (\r\n). Responses are terminated by CRLF. All commands are case-insensitive.

| Command        | Arguments           | Response                           | Description                                  |
|----------------|--------------------|------------------------------------|----------------------------------------------|
| TUNE           | (none)             | OK or ERR:reason                   | Initiate auto-tune                           |
| STOP           | (none)             | OK                                 | Halt auto-tune or motor movement             |
| HOME           | A\|B\|ALL          | OK or ERR:reason                   | Home motor A, B, or both                    |
| BAND           | n (0–10)           | OK:band_name or ERR:range          | Select band by index                         |
| BAND?          | (none)             | band_name                          | Query current band                           |
| SETL           | pct (0–100)        | OK:uh_value or ERR                 | Set inductor to % of range                  |
| SETL           | uh:value           | OK:uh_value or ERR                 | Set inductor to µH value                    |
| SETC           | pct (0–100)        | OK:pf_value or ERR                 | Set capacitor to % of range                 |
| SETC           | pf:value           | OK:pf_value or ERR                 | Set capacitor to pF value                   |
| SWR?           | (none)             | swr_value                          | Query current SWR reading                   |
| FWD?           | (none)             | watts_value                        | Query forward power in watts                |
| REF?           | (none)             | watts_value                        | Query reflected power in watts              |
| STATUS         | (none)             | JSON status string                 | Full system status (same as /api/status)    |
| PRESET SAVE    | band (0–10)        | OK or ERR                          | Save current (L,C) as band preset           |
| PRESET LOAD    | band (0–10)        | OK:l,c or ERR:no_preset            | Load band preset, move motors               |
| PRESET LIST    | (none)             | Multi-line: band,l,c,swr per band  | List all stored presets                     |
| PRESET CLEAR   | band (0–10)        | OK                                 | Clear preset for specified band             |
| CAL            | fwd:watts          | OK                                 | Set SWR bridge forward calibration at watts |
| RESET          | (none)             | OK                                 | Reset fault state, clear fault code        |
| REBOOT         | (none)             | OK (then disconnect)               | Soft reboot ESP32                           |
| WIFI           | on\|off            | OK                                 | Enable/disable WiFi AP                     |
| WIFI?          | (none)             | ssid,ip,clients                    | Query WiFi status                           |
| BTNAME         | new_name           | OK (takes effect after reboot)     | Set Bluetooth device name                  |
| TOPO           | L-HI\|L-LO\|PI\|T | OK or ERR                          | Set network topology                        |
| TOPO?          | (none)             | topology_name                      | Query current topology                      |
| VER?           | (none)             | firmware_version_string            | Query firmware version                      |
| HELP           | (none)             | Command list summary               | Print command reference                     |

**Example Session:**

```
> SWR?
2.34
> BAND 3
OK:40m
> PRESET LOAD 3
OK:42,48
> SWR?
1.45
> TUNE
OK
> SWR?
1.21
> PRESET SAVE 3
OK
```

---

### 8.4 WiFi HTTP API

The ESP32 HTTP server provides both the browser-accessible HTML interface and a JSON REST API for automation. All API endpoints return Content-Type: application/json.

| Endpoint               | Method | Parameters              | Response Body                     | Description                           |
|------------------------|--------|-------------------------|-----------------------------------|---------------------------------------|
| /api/status            | GET    | none                    | JSON full status                  | Complete system state                 |
| /api/tune              | POST   | none                    | {"result":"ok"}                   | Initiate auto-tune                    |
| /api/stop              | POST   | none                    | {"result":"ok"}                   | Stop tuning or motor movement         |
| /api/home              | POST   | motor=A\|B\|all         | {"result":"ok"}                   | Home specified motor(s)               |
| /api/setL              | POST   | pos=0–100 or uh=value   | {"l_pct":N,"l_uh":N}             | Set inductor position                 |
| /api/setC              | POST   | pos=0–100 or pf=value   | {"c_pct":N,"c_pf":N}             | Set capacitor position                |
| /api/band              | POST   | n=0–10                  | {"band":"40m"}                    | Select band                           |
| /api/topology          | POST   | topo=L-HI\|L-LO\|PI\|T | {"topo":"L-HI"}                   | Set matching network topology         |
| /api/presets           | GET    | none                    | JSON array of all presets         | Retrieve all band presets             |
| /api/preset/save       | POST   | band=0–10               | {"result":"ok"}                   | Save current position as preset       |
| /api/preset/load       | POST   | band=0–10               | {"result":"ok","l":N,"c":N}      | Load and apply preset                 |
| /api/preset/clear      | POST   | band=0–10               | {"result":"ok"}                   | Clear stored preset                   |
| /api/cal/swr           | POST   | power=watts             | {"result":"ok"}                   | SWR bridge calibration point          |
| /api/cal/l_max         | POST   | uh=value                | {"result":"ok"}                   | Set inductor max µH                   |
| /api/cal/c_max         | POST   | pf=value                | {"result":"ok"}                   | Set capacitor max pF                  |
| /api/reset             | POST   | none                    | {"result":"ok"}                   | Reset fault state                     |
| /api/config            | GET    | none                    | JSON config object                | Retrieve all config parameters        |
| /api/config            | POST   | JSON body               | {"result":"ok"}                   | Update config parameters              |
| /ws                    | WS     | WebSocket upgrade       | JSON status stream at 2 Hz        | Real-time status WebSocket            |

**WebSocket Status Packet (sent every 500 ms):**

```json
{
  "swr": 1.45,
  "fwd_w": 98.3,
  "ref_w": 2.1,
  "l_pct": 42,
  "l_uh": 15.3,
  "c_pct": 48,
  "c_pf": 245,
  "band": "40m",
  "tuning": false,
  "fault": 0
}
```

---

### 8.5 NVS Preset Storage

The ESP32 Non-Volatile Storage (NVS) system uses a flash partition to store key-value pairs that survive power cycles and firmware updates (NVS is preserved across firmware updates unless the NVS partition is explicitly erased).

**NVS Namespace:** `hf_tuner`

**Band Preset Keys (11 bands, index 0–10):**

| Key          | Type  | Description                              |
|--------------|-------|------------------------------------------|
| band0_l      | I32   | Inductor step count for band 0 (160m)   |
| band0_c      | I32   | Capacitor step count for band 0 (160m)  |
| band0_swr    | FLOAT | Best SWR achieved for band 0 preset     |
| band0_topo   | U8    | Network topology for band 0             |
| ...          | ...   | Repeated for band1 through band10       |

**User Preset Keys (50 user presets):**

| Key      | Type  | Description                          |
|----------|-------|--------------------------------------|
| u0_l     | I32   | Inductor step count, user preset 0   |
| u0_c     | I32   | Capacitor step count, user preset 0  |
| u0_freq  | FLOAT | Frequency for user preset 0 (MHz)    |
| ...      | ...   | Repeated for u1 through u49          |

**Calibration Keys:**

| Key              | Type  | Description                               |
|------------------|-------|-------------------------------------------|
| swr_fwd_scale    | FLOAT | Forward ADC counts-per-watt calibration   |
| swr_ref_scale    | FLOAT | Reflected ADC counts-per-watt calibration |
| swr_fwd_offset   | I32   | Forward ADC zero offset                   |
| swr_ref_offset   | I32   | Reflected ADC zero offset                 |
| l_max_uh         | FLOAT | Measured inductor maximum in µH            |
| l_steps_full     | I32   | Total steps for inductor full range       |
| c_max_pf         | FLOAT | Measured capacitor maximum in pF           |
| c_steps_full     | I32   | Total steps for capacitor full range      |

**Configuration Keys:**

| Key              | Type   | Description                              | Default    |
|------------------|--------|------------------------------------------|------------|
| tune_threshold   | FLOAT  | Auto-tune target SWR                     | 1.5        |
| coarse_steps     | U8     | Number of L steps in coarse scan         | 20         |
| motor_speed_auto | U16    | Steps/sec during auto-tune               | 400        |
| motor_speed_man  | U16    | Steps/sec during manual operation        | 200        |
| wifi_ssid        | STRING | WiFi AP SSID                             | RF-HFTuner |
| wifi_pass        | STRING | WiFi AP password (empty = open)          | (empty)    |
| bt_name          | STRING | Bluetooth device name                    | RF-HFTuner |

---

### 8.6 Firmware Update

**Prerequisites**

- Arduino IDE 2.x installed
- ESP32 Arduino board support package installed via Arduino IDE Boards Manager (search: "esp32 by Espressif Systems")
- Required libraries installed via Library Manager:
  - ESPAsyncWebServer
  - AsyncTCP
  - ArduinoJSON 6.x
  - ESP32 BT SPP (included in Espressif ESP32 board package)
  - SSD1306 (Adafruit SSD1306 + Adafruit GFX Library)

**Firmware Upload Procedure**

1. Connect USB cable to the ESP32 USB port on the rear panel of the tuner.
2. Do not apply 12V power during firmware upload (USB 5V powers the ESP32 logic sufficiently).
3. Open Arduino IDE 2.x.
4. Open `firmware/esp32_hf_tuner/esp32_hf_tuner.ino`.
5. Select Board: Tools → Board → ESP32 Arduino → ESP32 Dev Module.
6. Select Upload Speed: Tools → Upload Speed → 921600.
7. Select Port: Tools → Port → /dev/ttyUSB0 (Linux) or COMx (Windows). The port appears when the USB cable is connected.
8. If the port is not detected on Linux: `sudo usermod -aG dialout $USER` and log out/in.
9. Click Upload (right-arrow button in Arduino IDE toolbar).
10. Monitor the upload progress in the console at the bottom of the IDE.
11. On successful upload: "Done uploading" message appears.
12. Open Serial Monitor: Tools → Serial Monitor. Set baud rate to 115200.
13. Apply 12V power. Boot messages should appear in the serial monitor within 2 seconds.
14. Verify the firmware version string in the boot messages matches the expected version.
15. Calibration data in NVS is preserved across firmware updates. If the NVS key format has changed in the new firmware version, run the calibration procedure (Chapter 6) after updating.

**OTA (Over-The-Air) Firmware Update**

The firmware supports OTA update via the Arduino OTA library. To use:

1. Connect to the tuner WiFi AP (RF-HFTuner).
2. In Arduino IDE, select Port → Network Ports → RF-HFTuner at 192.168.4.1.
3. Upload as normal. The IDE sends the firmware via WiFi.
4. OTA update does not require physical access to the USB port.

---

## CHAPTER 9 — WINDING TABLES

### 9.1 Using the Winding Tables

Complete winding tables are provided in two formats:

- `winding_tables/winding_tables.ods` — OpenDocument Spreadsheet, opens in LibreOffice Calc
- `winding_tables/winding_tables.csv` — Plain CSV, opens in any editor or spreadsheet program

**Regenerating Tables from Source**

The tables are generated by the Python script `generate_winding_tables.py`:

```bash
pip install odfpy
python3 generate_winding_tables.py --verbose
```

This regenerates both the .ods and .csv files from the embedded Wheeler formula calculations. The `--verbose` flag prints each calculation to the console for verification.

---

### 9.2 Roller Inductor Design Notes

The roller inductor is a single continuous winding on a cylindrical former. A spring-loaded roller contact rides along the wire surface, tapping the winding at a selectable point. The number of turns between the bottom terminal and the roller contact determines the effective inductance.

**Design Parameters for 0.05–80 µH Range**

| Parameter          | Value                           | Notes                               |
|--------------------|---------------------------------|-------------------------------------|
| Former material    | PVC pipe, schedule 40           | 76 mm (3 inch) OD preferred         |
| Former length      | 200 mm (with end caps)          | 180 mm winding area                 |
| Wire gauge         | AWG 20 (0.812 mm diameter)      | Bare tinned copper                  |
| Winding type       | Close-wound (turns touching)    | Maximizes L per unit length         |
| Turns per mm       | ~1.2 turns/mm (AWG 20 close)   |                                     |
| Total turns        | ~216 turns over 180 mm          | Approximate; measure and adjust     |
| L_min (1 turn)     | ~0.05 µH                        | At minimum roller engagement        |
| L_max (all turns)  | ~80 µH                          | At maximum roller engagement        |
| Self-resonance     | > 10 MHz for L_min setting      | Acceptable for HF use               |

**Roller Contact Specifications**

- Material: phosphor bronze or beryllium copper for spring temper and conductivity
- Contact force: 1.5–2.0 N at roller contact point (prevents arcing under high current)
- Contact geometry: rounded tip (0.5 mm radius) to prevent wire cutting
- Roller guide: PTFE or Delrin bushing on leadscrew nut for smooth travel
- Contact lubrication: none (dry contact to prevent contamination)

**Wire Selection**

Use bare tinned copper wire (not enamel coated) for the roller contact surface. Enamel-coated wire introduces resistance at the roller contact point and causes erratic inductance readings. Tinned copper also resists corrosion at the contact point. Purchase from antenna wire suppliers or use un-coated hookup wire stripped of insulation.

---

### 9.3 Single-Layer Winding Table

Representative values from the full table (full table in winding_tables.csv). All calculations use the Wheeler single-layer formula: L(µH) = d²N² / (18d + 40l), d and l in inches.

| Band | Former (mm) | Target L (µH) | AWG | Turns | Length (mm) | Q est. |
|------|-------------|---------------|-----|-------|-------------|--------|
| 160m | 76          | 40.0          | 18  | 95    | 95          | 280    |
| 160m | 50          | 40.0          | 20  | 140   | 140         | 220    |
| 80m  | 76          | 20.0          | 18  | 66    | 66          | 320    |
| 80m  | 50          | 20.0          | 20  | 98    | 98          | 250    |
| 60m  | 50          | 12.0          | 20  | 76    | 76          | 270    |
| 40m  | 50          | 8.0           | 20  | 62    | 62          | 290    |
| 40m  | 38          | 8.0           | 22  | 80    | 72          | 260    |
| 30m  | 50          | 5.0           | 20  | 49    | 49          | 300    |
| 20m  | 38          | 3.0           | 22  | 50    | 45          | 290    |
| 20m  | 25          | 3.0           | 24  | 72    | 58          | 250    |
| 17m  | 38          | 2.0           | 22  | 41    | 37          | 285    |
| 15m  | 25          | 1.5           | 24  | 57    | 46          | 260    |
| 12m  | 25          | 1.0           | 24  | 46    | 37          | 240    |
| 10m  | 19          | 0.6           | 26  | 46    | 35          | 210    |
| 10m  | 25          | 0.6           | 24  | 36    | 29          | 230    |
| 6m   | 19          | 0.3           | 26  | 32    | 24          | 190    |
| 6m   | 12.7        | 0.3           | 28  | 44    | 30          | 170    |
| 6m   | 25          | 0.1           | 24  | 19    | 15          | 200    |
| 2m   | 12.7        | 0.1           | 28  | 29    | 20          | 160    |
| VHF  | 9.5         | 0.05          | 30  | 18    | 12          | 140    |

Note: Q estimates are for well-wound, self-supporting coils on dielectric formers. Q will be lower if the former has high dielectric loss (PVC has moderate loss; PTFE/Teflon former improves Q by 15–20% at the expense of cost and machinability).

---

### 9.4 Winding Procedure

Perform the following procedure using the motorized coil winder (see TM-CWND-001 for coil winder operation manual).

1. Cut the former to required length plus 20 mm for end caps. Deburr cut ends.
2. Drill axial holes in end caps to pass the leadscrew or mounting shaft.
3. Mark the winding start and end points with a felt-tip pen (lines parallel to the former axis).
4. Chuck the former in the coil winder spindle. Verify the former runs true (< 0.5 mm runout at winding area).
5. Load the correct AWG wire onto the winder tensioner spool. Set tensioner drag to the value specified in the wire tension table (typically 0.3–0.5 N for AWG 18–22).
6. Thread the wire through the tensioner arm and out to the starting point on the former. Secure the wire end with a small strip of polyimide tape folded back on itself (not covering the contact surface).
7. Enter coil parameters in the coil winder controller:
   - AWG: enter wire gauge
   - Former diameter: enter OD in mm
   - Target turns: enter from winding table
   - Pitch multiplier: 1.00 (close-wound)
8. Press START. Monitor the winding as it progresses; watch for crossed turns or wire jumping. Stop immediately if either occurs.
9. At target turn count, press STOP. The winder holds position.
10. Secure the wire end with polyimide tape. Apply a small drop of thin cyanoacrylate (CA) adhesive to each wire end terminal where it exits the former, to prevent the wire from unraveling.
11. Remove the former from the winder.
12. Measure inductance with an LCR meter at 1 kHz and at the operating frequency.
13. Compare measured inductance to the Wheeler prediction. Acceptable range: within ±10%.
14. If measured inductance differs from predicted by more than 10%: recount turns by visual inspection. Rewind if count is incorrect.
15. Record actual measured inductance on the winding label (permanent marker on end cap, or a small adhesive label).

> **NOTE:** Wheeler formula accuracy is typically ±3% for close-wound single-layer coils when former diameter and length are measured accurately. Larger errors usually indicate a turn-counting error during winding or incorrect former diameter entry.

---

## CHAPTER 10 — TROUBLESHOOTING

### 10.1 Troubleshooting Matrix

| Symptom                                  | Probable Cause                              | Corrective Action                                               |
|------------------------------------------|---------------------------------------------|------------------------------------------------------------------|
| SWR will not decrease below 3:1          | Antenna impedance outside tuner range       | Check antenna; Z may be < 10Ω or > 500Ω; check for open/short  |
| SWR will not decrease below 3:1          | Wrong network topology for this antenna     | Switch to T-network for high-Z loads; retry auto-tune           |
| SWR will not decrease below 3:1          | Capacitor or inductor at limit              | Check if L or C reads at 0% or 100% after tune; extend range   |
| SWR reads 10:1 or higher constantly      | Antenna disconnected or open coax           | Inspect ANT port connector and feedline continuity              |
| SWR reads 10:1 or higher constantly      | Short circuit at ANT port                   | Inspect coax center/shield continuity with ohmmeter            |
| SWR reads 10:1 or higher constantly      | SWR bridge component failure                | Check BAT46 diodes; replace if open or shorted                  |
| Motor runs but position does not change  | Coupler set screw loose                     | Tighten M3 set screw on flexible coupler with 1.5 mm hex key   |
| Motor runs but position does not change  | Coupler broken (spider damaged)             | Replace spider insert (6-jaw silicone coupler)                  |
| Motor runs but position does not change  | Leadscrew nut loose or stripped             | Inspect T8 anti-backlash nut; replace if stripped               |
| Auto-tune oscillates between two positions | L and C interaction: two local minima     | Increase coarse scan resolution (Config page: coarse steps 30) |
| Auto-tune oscillates between two positions | SWR measurement noise               | Check antenna for RF current arcing or intermittent connection  |
| Tuner matches but SWR increases after minutes | Thermal drift in roller contact    | Clean roller contact surface (Section 11.3); verify contact force |
| Tuner matches but SWR increases after minutes | Antenna impedance shift (wind)     | Re-tune; may need to re-tune at intervals in high wind          |
| No WiFi AP visible                       | ESP32 WiFi not initialized                  | Check serial console for WiFi error; reboot ESP32               |
| No WiFi AP visible                       | WiFi disabled in config                     | Connect via BT; send WIFI ON command                            |
| No WiFi AP visible                       | Conflicting SSID on another device          | Unlikely; try connecting with manual SSID entry                 |
| CYD display shows "BT: Disconnected"     | BT pairing lost                             | Power cycle both devices; re-pair (Section 4.6)                |
| CYD display shows "BT: Disconnected"     | Distance too large                          | Move CYD within 10 meters of tuner                              |
| OLED blank after power-up                | I2C address mismatch (0x3C vs 0x3D)        | Check SSD1306 I2C address jumper; update config.h               |
| OLED blank after power-up                | 5V rail absent (LM7805 failed)              | Measure 5V at OLED VCC pin; replace LM7805 if absent           |
| OLED blank after power-up                | Loose SDA/SCL connection                    | Re-seat OLED I2C connector                                      |
| Motor makes grinding noise               | Motor coupling misaligned                   | Re-align motor shaft and leadscrew; both must be coaxial       |
| Motor makes grinding noise               | Leadscrew debris                            | Clean leadscrew with isopropyl alcohol; re-lubricate lightly   |
| Motor makes grinding noise               | Stepper skipping steps (too fast or too heavy load) | Reduce motor speed in config; increase Vref (if below spec) |
| Band preset incorrect (saves wrong pos.) | Homing failure on previous power-up         | Run HOME ALL from web interface; re-tune band; save new preset |
| Band preset incorrect (saves wrong pos.) | NVS data corrupt                            | Erase NVS (Config page → Factory Reset); recalibrate          |
| SWR bridge reads forward power = 0       | Transmitter not keyed during measurement    | Verify TX is keying; check PTT connection                       |
| SWR bridge reads forward power = 0       | Coupling toroid winding open                | Measure toroid secondary; 20-turn winding should read ~50 Ω    |
| SWR bridge reads forward power = 0       | BAT46 diode open circuit                    | Replace forward detector diode (D1)                             |
| Inductor sparking/arcing at high power   | Roller contact contaminated or high resistance | Clean contact surface; verify 1.5–2.0 N contact force        |
| Inductor sparking/arcing at high power   | Power too high during tuning                | Reduce to 10W before tuning; section 5.2                       |
| Inductor sparking/arcing at high power   | Gap in winding (unwound area)               | Inspect winding surface; rewound damaged section or replace    |

---

### 10.2 Fault Codes

All fault codes are displayed on the OLED (line 1) and STATUS LED (red, steady). The web interface also shows the fault code and description on the main page header. Clear faults with the RESET command (BT SPP), the Reset button on the web Config page, or by pressing the rotary encoder for 5 seconds.

| Fault Code | OLED Display          | Meaning                                    | Recovery Action                                              |
|------------|-----------------------|--------------------------------------------|--------------------------------------------------------------|
| F01        | FAULT: HIGH SWR       | SWR > 10:1 after completing full scan      | Check antenna connection; inspect feedline and connector     |
| F02        | FAULT: MOTOR A        | Inductor motor stall or TMC2208 overcurrent| Reduce Vref if > 0.8V; check coupler alignment; inspect load |
| F03        | FAULT: MOTOR B        | Capacitor motor stall or obstruction       | Inspect mechanical stops; verify leadscrew free travel       |
| F04        | FAULT: NO SWR         | SWR bridge ADC reads zero (or < noise floor) with RF applied | Check D1 BAT46; check toroid winding; check ADC pin GP34/GP35 |
| F05        | FAULT: TEMP           | TMC2208 driver thermal shutdown activated  | Power off; cool 10 min; reduce ambient temp or motor current |
| F06        | FAULT: HOME FAIL      | Motor did not reach limit switch within expected steps | Check limit switch wiring; inspect LED/phototransistor pair; verify motor can move to limit |
| F07        | FAULT: CAL            | NVS calibration data CRC fail or keys absent | Run full calibration procedure (Chapter 6); save new data  |

---

### 10.3 Performance Testing

Perform this test procedure after any repair, firmware update, or calibration change to verify the tuner is operating within specification.

1. Connect a 50 Ω dummy load (Bird, Palstar, or home-built resistive load) to the ANT port. Connect transceiver to TX port.
2. Power on tuner. Wait for initialization and homing to complete.
3. On the web interface main page, observe the SWR reading. It should read 1.0:1 (±0.1) with a matched 50 Ω load and no RF. Some residual noise is normal (ADC reads near zero → SWR calculation may show 1.0–1.1:1).
4. Transmit at 10W. SWR should read 1.0:1 (±0.1). Forward power should read approximately 10W.
5. Disconnect the 50 Ω dummy load. Connect a 200 Ω resistor (5W or more rated) to the ANT port.
6. Press TUNE. The auto-tune should find a match (SWR < 1.5:1) within 15 seconds.
7. After match, press TUNE again (re-tune from scratch: press RESET first to clear preset). Time the second tune: should be < 8 seconds for the same antenna impedance.
8. Connect an open circuit (no load) to the ANT port. The SWR should read above 10:1 and FAULT F01 should be set.
9. Press RESET to clear the fault.
10. Connect a short circuit (coax center shorted to shield). FAULT F01 should be set again.
11. Press RESET. Reconnect the 50 Ω dummy load. SWR returns to 1.0:1. All fault codes cleared.
12. Verify all 11 band presets can be recalled without error. Check that motor positions change when switching bands.
13. The tuner passes performance testing if all above steps yield the expected results.

---

## CHAPTER 11 — PREVENTIVE MAINTENANCE

### 11.1 Maintenance Schedule

| Interval            | Task                                                    | Procedure Reference   |
|---------------------|---------------------------------------------------------|-----------------------|
| After each field use | Visual inspection: connectors, cables, enclosure seal | Section 11.2, 4.1     |
| After each field use | Functional test: SWR = 1.0:1 with dummy load          | Section 10.3          |
| Monthly             | Clean capacitor vanes                                   | Section 11.2          |
| Monthly             | Clean and inspect roller inductor contact               | Section 11.3          |
| Monthly             | Check motor coupler set screws                          | Section 11.4          |
| Quarterly           | Verify SWR bridge calibration                          | Section 6.1           |
| Quarterly           | Verify motor homing accuracy (test preset recall)      | Section 5.4           |
| Annually            | Replace motor driver TMC2208 thermal pads              | Section 11.4          |
| Annually            | Inspect capacitor vane spacing uniformity              | Section 11.2          |
| Annually            | Full performance test                                   | Section 10.3          |
| As needed           | Firmware update                                         | Section 8.6           |

---

### 11.2 Capacitor Vane Cleaning

Airborne dust, salt (coastal environments), and oxidation can reduce capacitor Q and cause arcing between vanes at high power.

1. Power off the tuner. Disconnect all RF cables. Disconnect 12V power.
2. Remove lid (4× M3 screws, Torx T10 or Phillips head).
3. Using compressed dry air (canned air or small air compressor with moisture trap), blow out dust from the vane stack. Hold can upright to prevent propellant from depositing on vanes.
4. Inspect vanes for corrosion (green-grey patches on aluminum vanes). Minor oxidation is not harmful; heavy corrosion or pitting should be addressed.
5. If vanes have deposits: moisten a lint-free cloth with 91% or better isopropyl alcohol. Wipe vane edges gently. Do not use water (corrosion risk), petroleum products (flammable and insulating), or abrasive materials (scratch vane surfaces).
6. After cleaning, verify there is no contact between rotor and stator vanes through the full rotation range. Manually rotate the capacitor shaft slowly through 180° and listen for any scraping.
7. If vane contact is present: do not operate. The capacitor requires realignment or replacement of bent vanes.
8. Replace lid. Verify all M3 screws are tight (gasket must be compressed for IP54 seal).

---

### 11.3 Roller Inductor Maintenance

The roller inductor contact is the component most susceptible to degradation under field conditions.

1. Power off the tuner. Disconnect all RF cables.
2. Remove the inductor access panel (if fitted) or open the enclosure lid.
3. Inspect the winding surface along the full roller travel path. Look for:
   - Dark oxidation bands where the roller frequently contacts (indicate high contact resistance)
   - Bright wear tracks (normal and acceptable)
   - Physical damage (bent or broken turns)
4. If oxidation bands are present: cut a strip of 600-grit wet/dry abrasive cloth approximately 10 mm × 100 mm. Lay the strip abrasive-side-up on the wire surface. Drive the roller back and forth over the strip approximately 10 passes to polish the contact area. Remove all abrasive residue with isopropyl alcohol on a lint-free cloth.
5. Verify spring tension on the roller contact assembly. Press the roller into the winding surface with a small spring scale or tension gauge. The reading should be 1.5–2.0 N. If below 1.0 N, the spring must be replaced or adjusted (bend the spring leaf slightly toward the contact).
6. Inspect the roller contact tip. If it has developed a sharp edge or groove from wire contact, lightly polish with 1000-grit paper to restore the rounded tip geometry.
7. Wipe the entire winding surface and roller with a dry lint-free cloth to remove all residue.

---

### 11.4 Motor Driver Inspection

The TMC2208 stepper drivers are the active electronic components with the highest thermal dissipation in the system. Annual inspection prevents premature failure.

1. Power off tuner. Disconnect 12V supply.
2. Open enclosure lid.
3. Locate the two TMC2208 StepStick modules on the main PCB.
4. Verify the aluminum heat sinks are firmly bonded to the TMC2208 package. Press gently; the heat sink should not lift or rotate. If loose, clean both surfaces with isopropyl alcohol and apply new thermal adhesive tape (Fischer Elektronik WA-T 1 or equivalent, 0.2 mm thickness).
5. Verify Vref potentiometer settings using a calibrated DC voltmeter:
   - Motor A (roller inductor): Vref = 0.70–0.75 V (sets motor current to ~1.4 A)
   - Motor B (variable capacitor): Vref = 0.48–0.52 V (sets motor current to ~1.0 A)
   - Measure Vref at the center pin of the trimmer potentiometer on each StepStick module, with 12V power applied and no RF transmission in progress.
   - TMC2208 I_peak = Vref × 1.41 / (0.11 × R_sense). For R_sense = 0.11 Ω (standard StepStick): I_peak = Vref / 0.1554.
   - If Vref is outside specification, adjust the trimmer with a small flat-blade screwdriver while monitoring with the voltmeter.
6. Inspect all stepper motor cable JST-XH connectors. The connector latch should click firmly. Pull lightly on each cable — it should not separate without pressing the latch. Loose connectors cause intermittent stepping faults (Fault F02 or F03).
7. Inspect flexible coupler spider inserts for cracks or deformation. The silicone spider should be pliable, not hardened. Replace if cracked or distorted.
8. With motors manually operated at low speed (web interface slider at 10%), listen for unusual sounds. Smooth stepping is normal; grinding, squeaking, or hesitation indicates a mechanical issue.

---

## CHAPTER 12 — PARTS LIST AND PROCUREMENT

### 12.1 Electronic Components

| Item                             | Qty | Description                                  | Source                           |
|----------------------------------|-----|----------------------------------------------|----------------------------------|
| ESP32-WROOM-32 DevKit            | 1   | Espressif ESP32, 38-pin breakout             | Mouser, DigiKey, Amazon          |
| TMC2208 StepStick module         | 2   | Trinamic stepper driver, 2A peak, 1/16 step  | Amazon, AliExpress, Mouser       |
| FT-50-43 toroid                  | 1   | Ferrite toroid, #43 material, 12.7 mm OD     | Amidon, Mouser, Palomar Engineers |
| BAT46 Schottky diode             | 4   | 100V, 150 mA, 0.2V Vf, DO-35 glass          | Mouser, DigiKey                  |
| LM358N op-amp                    | 2   | Dual op-amp, DIP-8                           | Mouser, DigiKey, Texas Instruments |
| ULN2003A relay driver IC         | 1   | 7-channel Darlington array, DIP-16          | Mouser, DigiKey                  |
| 5V SPDT relay, PC mount          | 3   | 5V coil, 10A contacts, 2-Form-C or SPDT     | Amazon, Mouser                   |
| AMS1117-3.3 LDO regulator        | 1   | 3.3V, 1A, SOT-223                           | Mouser, DigiKey                  |
| LM7805 LDO regulator             | 1   | 5V, 1A, TO-220                              | Mouser, DigiKey                  |
| SSD1306 OLED display, 128×64     | 1   | I2C interface, 0.96 inch                    | Amazon, AliExpress                |
| CYD (ESP32-2432S028R)            | 1   | ESP32 with 2.8" ILI9341 touch display       | AliExpress, Cheap Yellow Display Project |
| Rotary encoder with switch       | 1   | 20 detent, momentary push, 6 mm shaft       | Bourns PEC11 or equivalent, Mouser |
| 47 Ω resistor, 1W                | 2   | Termination for Bruene bridge ports         | Mouser, any supplier              |
| 100 Ω resistor, 1/4W             | 4   | Detector load resistors                     | Mouser, any supplier              |
| 100 nF ceramic capacitor         | 10  | Bypass capacitors, 50V, 0805 or DIP         | Mouser, DigiKey                  |
| 10 µF electrolytic capacitor     | 5   | Bulk bypass, 25V, radial                    | Mouser, DigiKey                  |
| 3 mm LED, green                  | 1   | Status LED                                  | Any supplier                     |
| 3 mm LED, red                    | 1   | SWR indicator                               | Any supplier                     |
| PCB, custom (see schematic)      | 1   | Main controller board                       | JLCPCB, PCBWay, OSHPark          |
| 12V active buzzer, PC mount      | 1   | Optional; tune-complete tone                | Any supplier                     |

---

### 12.2 Mechanical Components

| Item                             | Qty | Specification                                | Source                           |
|----------------------------------|-----|----------------------------------------------|----------------------------------|
| NEMA 17 stepper motor            | 2   | 42 × 42 × 40 mm body, 1.5 A, 200 steps/rev  | Amazon, Stepperonline, StockDrive |
| Flexible shaft coupler 5mm–8mm   | 2   | Aluminum spider type, 25 mm OD              | Amazon, StockDrive                |
| Silicone spider insert (spare)   | 4   | Replacement for coupler above               | Amazon (sold with couplers)      |
| Linear bearing LM8UU             | 4   | 8 mm bore, 15 mm OD, 24 mm length           | Amazon, AliExpress                |
| 8 mm linear rod, 200 mm          | 2   | Hardened, ground; guide rods for carriage   | Amazon, RobotDigg                 |
| T8 leadscrew 250 mm + nut        | 1   | 8 mm OD, 2 mm pitch (T8×2), anti-backlash  | Amazon, AliExpress                |
| Variable air capacitor 10–500 pF | 1   | Two-gang or single-gang, 1 kV working       | Fair Radio Sales, eBay surplus    |
| Roller inductor, 0–80 µH         | 1   | Build per Section 9.4 or surplus military   | Collins surplus, Fair Radio       |
| N-type chassis connector female  | 4   | Panel mount (ANT, TX, plus 2 spare)         | Mouser, DigiKey, Amphenol         |
| 2.1 mm barrel jack, panel mount  | 1   | 12V DC power input                          | Mouser, DigiKey                   |
| Mini-USB type B, panel mount     | 1   | Programming port                            | Amazon, Mouser                    |
| JST-XH 4-pin connector pair      | 4   | Motor cable connectors, 2.54 mm pitch       | Amazon (sold in assortment kits)  |
| 3D printed enclosure             | 1   | Print from enclosure.scad (PLA or PETG)     | Prusa/Bambu printer, filament     |
| 3D printed enclosure lid         | 1   | Print from enclosure.scad                  | Same as above                     |
| Closed-cell foam gasket strip    | 1m  | 3 × 3 mm, for IP54 lid seal                | Amazon, local hardware            |
| Heat-set insert M3               | 20  | M3 × 4 mm, for enclosure threaded bosses   | Amazon, Prusa shop                |

---

### 12.3 Hardware

All fasteners are metric stainless steel (A2-70 or A4-80 for corrosion resistance in field use).

| Item                                 | Qty | Notes                                           |
|--------------------------------------|-----|-------------------------------------------------|
| M3 × 8 socket head cap screw (SHCS)  | 20  | Enclosure lid, PCB, motor mount                 |
| M3 × 6 SHCS                         | 12  | Inductor end cap, capacitor bracket             |
| M3 × 4 SHCS                         | 8   | OLED mount, relay board                         |
| M4 × 12 SHCS                        | 8   | NEMA 17 motor mount                             |
| M4 × 20 SHCS                        | 4   | Capacitor bracket to enclosure floor            |
| M2.5 × 6 SHCS                       | 8   | ESP32 DevKit PCB mount (M2.5 nylon standoffs)   |
| M3 flat washer                       | 20  | Under all M3 SHCS heads                         |
| M3 hex nut                          | 10  | Spare / nut plate applications                  |
| M3 nylon standoff 10 mm FF           | 8   | PCB mounting, insulating                        |
| M2.5 nylon standoff 6 mm FF          | 4   | ESP32 board mounting                            |
| Cable tie 3 × 100 mm                 | 50  | Cable management                                |
| Ferrite snap-on 31-material          | 4   | Motor cable RFI suppression (if needed)         |
| Heat-shrink tubing assortment       | 1   | Wire insulation and connector covers            |

---

## APPENDIX A — SCHEMATIC DIAGRAMS

### A.1 Bruene SWR Bridge

Refer to: `schematics/swr_bridge_ascii.txt`

**Circuit Description**

The Bruene bridge uses a single FT-50-43 toroid core with a 20-turn secondary coupling winding wound over a single-turn primary (the through-line conductor, which passes through the toroid center hole once). The secondary winding is center-tapped to chassis ground via matched 47 Ω resistors, which provide the characteristic impedance reference for the directional coupler.

Two BAT46 Schottky diodes (D1 for forward, D2 for reflected) detect the rectified signal from each port of the center-tapped secondary. Each detector feeds an RC low-pass filter (100 Ω series, 100 nF to ground) that averages the RF envelope to a DC level proportional to the RMS RF voltage. The DC output is buffered by one section of the LM358N dual op-amp before feeding the ESP32 ADC inputs (GPIO 34 and GPIO 35, input-only pins selected to prevent inadvertent output drive damage).

The entire bridge assembly is mounted in a shielded sub-enclosure of copper-clad FR4, with separate RF ground plane sections for the forward and reflected detector channels to maintain > 30 dB directivity.

---

### A.2 HF Tuner Controller

Refer to: `schematics/hf_tuner_controller_ascii.txt`

**Circuit Description**

The HF tuner controller board integrates the following functions:

- **Power regulation**: 12V input through a 2A inline fuse to the 12V bus for motors and relay coils. The LM7805 (with 2200 µF input and 470 µF output bulk capacitors) provides a clean 5V rail for relays and the OLED display. The AMS1117-3.3 (with 10 µF input and 10 µF output) provides 3.3V for the ESP32 and bridge op-amps.

- **ESP32 connections**: The ESP32 DevKit is mounted on four M2.5 nylon standoffs above the controller PCB. GPIO pins are connected via right-angle 0.1-inch headers to the PCB through-holes. Key connections: GPIO 34 (ADC forward), GPIO 35 (ADC reflected), GPIO 26/27/14/12 (Motor A step/dir/enable, limit switch), GPIO 25/33/32/4 (Motor B step/dir/enable, limit switch), GPIO 21/22 (I2C SDA/SCL for OLED), GPIO 2 (status LED via 220 Ω), GPIO 15 (SWR LED via 100 Ω), GPIO 19/18/5 (relay control via ULN2003A).

- **TMC2208 stepper drivers**: Two StepStick modules plug into 0.1-inch pin headers. The STEP and DIR signals from the ESP32 drive the motors. The EN (enable) signal is normally LOW (enabled). The UART configuration line on each TMC2208 allows the ESP32 to configure internal registers (current, microstepping, StealthChop) via UART.

- **Relay topology switching**: Three SPDT relays switch network topology elements into and out of the signal path. The ULN2003A provides relay coil drive current (the ESP32 GPIO cannot source sufficient current for relay coils directly). Relay contacts are rated 10A/250V; the RF current at 200W/50Ω is 2A peak, well within relay contact rating.

---

### A.3 VHF Stub Tuner Controller

Refer to: `schematics/vhf_stub_tuner_ascii.txt`

**Circuit Description**

The VHF/UHF stub tuner controller is a simplified version of the HF controller, using only one TMC2208 driver and one NEMA 17 motor. The stub position is controlled by a 200 mm leadscrew (M8×1.25 pitch, higher precision than the T8 leadscrew) for fine control of the stub length.

Two N-type female connectors are mounted in a machined aluminum enclosure with a central channel for the air-dielectric coaxial stub. The inner conductor of the stub is a 10 mm OD brass rod; the outer conductor is a 22 mm ID aluminum tube. The sliding short is a machined brass plug that shorts inner to outer conductor at the desired position. The motor drives the sliding short via the leadscrew and a brass nut threaded into the short body.

The ESP32 in this unit also hosts a WiFi AP and BT SPP server, using the same firmware base with a stub-tuner configuration flag set in `config.h`. When connected to the shared CYD display, the CYD firmware presents a VHF/UHF band selection interface automatically.

---

## APPENDIX B — BAND-BY-BAND OPERATING NOTES

### 160 Meters (1.8–2.0 MHz)

The 160-meter band presents significant tuning challenges due to the large inductance required (20–40 µH typical) for short antennas. A full-size λ/2 dipole at 1.85 MHz is 81 meters long; most installations use a shortened vertical or loaded dipole. A 10-meter loaded vertical at 160m may present an impedance of 10 + j500 Ω — very low resistance with enormous inductive reactance. The T-network topology (two series inductors, one shunt capacitor) is recommended for loads below 25 Ω on 160m. Expect the roller inductor to be at or near maximum (80 µH) and the capacitor at maximum engagement. The high Q network on 160m means the tuning is sharp — a small motor position error produces large SWR change. Allow the auto-tune to complete fully rather than attempting manual tuning on this band.

### 80 Meters (3.5–4.0 MHz)

The 80-meter band is broad (500 kHz) and a single preset may not serve the entire band from resonant dipole SWR to acceptable SWR across the full 3.5–4.0 MHz span. Consider saving separate presets for 3.5, 3.75, and 4.0 MHz if operating across the full 80m band. A resonant 80m dipole (40 meters half-length) presents near 50 Ω at resonance; non-resonant antennas may present 10–300 Ω. L-network topology works well for most 80m antennas. Inductor setting typically 8–20 µH, capacitor variable across a wide range.

### 60 Meters (5.3–5.5 MHz)

The 60-meter band (US allocation: 5.3305, 5.3465, 5.3570, 5.3715, 5.4035 MHz) consists of five discrete channels. Antennas tuned for 40m or 80m require matching on 60m. An existing 40m dipole at 60m is 1.6 × too long and presents a high impedance (several hundred ohms). The Pi-network or T-network handles this well. Impedance variation between the five 60m channels is small enough that a single preset serves all channels.

### 40 Meters (7.0–7.3 MHz)

The 40-meter band is a workhorse band for HF communications, 300 kHz wide. A resonant dipole or vertical presents near 50 Ω; a multiband doublet or NVIS antenna may present 50–200 Ω. The L-network handles the full range of typical 40m antenna impedances efficiently. Typical settings: L 4–10 µH, C 50–250 pF depending on antenna. Band is wide enough to merit separate presets for 7.0 MHz (CW) and 7.2 MHz (SSB) if SWR changes significantly across the band.

### 30 Meters (10.1–10.15 MHz)

The 30-meter WARC band is narrow (50 kHz). A single preset serves the entire band. Antenna impedances are similar to 40m but with about 30% less inductance required. This band is often excellent for propagation on multiband antennas. Typical settings: L 2–6 µH, C 50–200 pF.

### 20 Meters (14.0–14.35 MHz)

Twenty meters is the premier DX band. Resonant antennas present near 50 Ω; loaded or shortened antennas may be significantly capacitive. The L-network is efficient at 20m. Note the reduced inductance required compared to lower bands (typically 1–4 µH). At 20m, the capacitor voltage rating is less critical (lower reactance, lower voltage for same power). Typical settings: L 1–3 µH, C 100–400 pF. Auto-tune is fast on 20m due to reduced L range to search.

### 17 Meters (18.068–18.168 MHz)

The 17-meter WARC band (100 kHz wide). Similar to 20m in behavior. A single preset serves the band well. Many multiband antennas have good inherent match on 17m if designed with 20m and 15m in mind (trapped or fan dipole). Inductance in the range 0.5–2 µH, capacitance 50–200 pF.

### 15 Meters (21.0–21.45 MHz)

Fifteen meters is excellent for long-distance propagation during solar cycle peaks. A resonant 15m dipole presents near 50 Ω; a compromise or multiband antenna may need light matching. The tuner handles 15m easily; motor travel distances are small. Typical settings: L 0.5–1.5 µH, C 50–150 pF. If using a trapped or fan dipole designed for 15m, the tuner may need minimal action (< 1 second tune time).

### 12 Meters (24.89–24.99 MHz)

The 12-meter WARC band. Similar to 10m in antenna and propagation behavior, but slightly longer element lengths. Tuner settings comparable to 10m but with slightly larger inductance. Narrow band (100 kHz); one preset covers it.

### 10 Meters (28.0–29.7 MHz)

The 10-meter band is 1.7 MHz wide — the widest HF amateur band. Multiple presets may be needed for 28.0, 28.5, and 29.0 MHz when using non-resonant antennas. At 10m, the required inductance is low (0.1–0.8 µH), and the roller inductor must be at near-minimum engagement. Capacitor position at medium engagement. Note that on 10m, the roller contact touches the first few turns of the winding; verify these turns are in good condition during maintenance.

### 6 Meters (50–54 MHz)

The 6-meter magic band requires careful design. The tuner operates at the upper edge of its HF range here. The roller inductor at minimum setting (0.05 µH) may still present excessive reactance if the inductance cannot be reduced to the required value. If the tuner does not achieve SWR < 1.5:1 on 6m, the inductance range may need to be extended downward by reducing the number of turns in the minimum contact position. A resonant 6m antenna is strongly preferred — the tuner should need minimal correction. Typical settings: L 0.05–0.5 µH, C 10–50 pF.

### 2 Meters (144–148 MHz)

The VHF/UHF stub tuner is used for 2-meter operation. The HF tuner is not suitable above 54 MHz (inductor self-resonance, capacitor lead inductance, and relay contact parasitics dominate). For 2m, connect the VHF stub tuner in the feedline. Set the stub to 144 MHz center frequency (λ/4 = 514 mm, air dielectric). A well-designed 2m antenna (Yagi, collinear, J-pole) at resonance needs no stub tuning; the stub is useful for marginal antennas or when operating at the band edges.

### 1.25 Meters (219–225 MHz)

The 1.25-meter band (222 MHz center) is used primarily in North America. Stub tuner λ/4 length at 222 MHz: 338 mm. Antenna designs for this band are similar to 2m but physically smaller. The stub tuner is effective for correcting minor impedance deviations from ideal.

### 70 Centimeters (420–450 MHz)

The 70-centimeter amateur band sees heavy use for FM repeaters, ATV, satellite, and weak-signal SSB/CW. Stub tuner λ/4 at 435 MHz: 172 mm. At this frequency, connector quality becomes important for insertion loss. Use clean N-type connectors; do not use PL-259 adapters on the stub tuner at 70cm (excessive loss). A quarter-wave stub at 70cm is relatively short and easy to implement precisely. Position resolution of 0.4 µm gives electrical angle resolution of less than 0.001° — mechanical position is not the limiting factor on tuning accuracy at this band.

### 33 Centimeters (902–928 MHz)

The 33-centimeter band is used for spread-spectrum, ATV, and experimental modes. The 902–928 MHz ISM overlap means interference considerations apply. Stub tuner λ/4 at 915 MHz: 82 mm. At these frequencies, stub tuner construction quality is critical — solder joints must be clean, and the stub connector interface must be mechanically rigid. Any mechanical movement of the stub connector under vibration will detune the match.

### 20 Centimeters (1240–1300 MHz)

The 23-centimeter band (commonly called 20cm in context of this tuner system) requires SMA or N-type connectors with low parasitic inductance. The stub tuner λ/4 at 1270 MHz: 59 mm. Tolerances are tight — a 1 mm position error represents 0.6° electrical, which causes a 0.1:1 SWR change for a marginal antenna match. The auto-tune algorithm's fine-step resolution (0.4 µm) is more than adequate; the limiting factor is the stub's own connector and construction tolerances. Ensure the sliding short contact resistance is < 10 mΩ; higher contact resistance causes measurable insertion loss at 1.3 GHz.

---

## APPENDIX C — WHEELER FORMULA REFERENCE

### C.1 Wheeler Single-Layer Formula

The Wheeler 1928 single-layer inductance formula is the standard approximation for air-core coils. It is accurate to within 1% for length-to-diameter ratios of 0.4 to infinity (elongated coils). For pancake coils (l < 0.4d), a separate formula applies (not commonly needed for HF inductors).

**Formula (English units — inches):**

```
         d² × N²
L  =  ─────────────    [µH]
        18d + 40l
```

Where:
- d = former diameter in inches
- l = winding length in inches
- N = number of turns

**Formula (SI units — meters, for reference):**

```
         µ₀ × π × (d/2)² × N²
L  =  ──────────────────────────    [Henry]
                l
```

This is the ideal solenoid formula (infinite length). Wheeler corrects for finite length with a form factor.

---

### C.2 Wheeler Multi-Layer Formula

For coils wound in multiple layers (used when winding length must be minimized):

**Formula (English units — inches):**

```
          0.8 × a² × N²
L  =  ────────────────────────    [µH]
          6a + 9l + 10c
```

Where:
- a = mean radius of winding in inches (= (d_inner + d_outer) / 4)
- l = length of winding in inches
- c = radial depth of winding in inches (= (d_outer − d_inner) / 2)
- N = total number of turns

Multi-layer winding is typically used for compact designs where L is large but must fit in a small space. The roller inductor in this tuner uses single-layer winding for low contact resistance and ease of roller tracking.

---

### C.3 Worked Examples

**Example 1: 40m inductor, 10 µH, 50 mm former**

Given: d = 50 mm = 1.969 inch, L = 10 µH, assume l = d (square coil, l = d = 1.969 inch)

Solve for N:

```
10 = N² × (1.969)² / (18 × 1.969 + 40 × 1.969)
10 = N² × 3.878 / (35.44 + 78.76)
10 = N² × 3.878 / 114.2
N² = 10 × 114.2 / 3.878 = 294.6
N = 17.2 turns
```

For a close-wound coil with AWG 20 wire (0.812 mm OD), 17.2 turns over 1.969 inches = 50 mm:
Turns per mm = 17.2 / 50 = 0.344 t/mm, but AWG 20 is 0.812 mm diameter, so close-wound pitch = 1/0.812 = 1.232 t/mm.

With 1.232 t/mm × 50 mm = 61.6 turns possible in 50 mm — more than enough. Actual winding would be shorter than assumed:

Revised l = 17.2 / 1.232 = 14.0 mm = 0.551 inch.

Re-check with Wheeler:

```
L = (1.969)² × (17.2)² / (18 × 1.969 + 40 × 0.551)
  = 3.878 × 295.8 / (35.44 + 22.04)
  = 1146.9 / 57.48
  = 19.96 µH  ← does not match target
```

Iteration required. Target 10 µH with shorter winding length means fewer turns are needed. Reduce N and recalculate. In practice, the generate_winding_tables.py script performs this iteration automatically.

**Example 2: 160m inductor, 40 µH, 76 mm former**

From the winding table: 95 turns on 76 mm former, 95 mm winding length.

Verify with Wheeler (d = 76 mm = 2.992 inch, l = 95 mm = 3.740 inch, N = 95):

```
L = (2.992)² × (95)² / (18 × 2.992 + 40 × 3.740)
  = 8.952 × 9025 / (53.86 + 149.6)
  = 80,792 / 203.46
  = 39.7 µH  ✓ (target 40 µH, within 0.75%)
```

---

### C.4 Inductance for 1-Inch (25.4 mm) Former at Various Turn Counts

Winding pitch: 1 turn per 0.9 mm (AWG 22 close-wound). Length l = N × 0.9 mm = N × 0.035 inch.

| N    | l (inch) | L (µH) | Notes                |
|------|----------|--------|----------------------|
| 5    | 0.178    | 0.076  | 6m typical           |
| 10   | 0.354    | 0.282  | 10m typical          |
| 15   | 0.531    | 0.598  | 15m typical          |
| 20   | 0.709    | 1.016  | 17m typical          |
| 25   | 0.886    | 1.531  | 20m typical          |
| 30   | 1.063    | 2.142  | 17–20m               |
| 40   | 1.417    | 3.632  | 40m typical          |
| 50   | 1.772    | 5.479  | 40m higher L         |
| 60   | 2.126    | 7.682  | 30m                  |
| 80   | 2.835    | 13.10  | 80m                  |
| 100  | 3.543    | 19.77  | 80m/160m             |
| 120  | 4.252    | 27.70  | 160m                 |
| 150  | 5.315    | 41.85  | 160m high L          |
| 200  | 7.087    | 71.06  | 160m maximum         |

Note: These values are for 1-inch (25.4 mm) former only. Scale by (d_actual / 1 inch)² × correction factor for other former diameters.

---

### C.5 Skin Effect Correction Table for HF Bands

The AC resistance of a conductor increases with frequency due to skin effect. The correction factor below is the ratio R_ac / R_dc for solid-conductor wire gauges commonly used in HF inductors.

| Freq (MHz) | δ copper (mm) | AWG 18 R_ac/R_dc | AWG 20 R_ac/R_dc | AWG 22 R_ac/R_dc |
|------------|----------------|------------------|------------------|------------------|
| 1.85       | 0.049          | 1.46             | 1.28             | 1.18             |
| 3.75       | 0.034          | 1.99             | 1.62             | 1.38             |
| 7.15       | 0.025          | 2.74             | 2.12             | 1.73             |
| 10.12      | 0.021          | 3.26             | 2.47             | 1.98             |
| 14.2       | 0.018          | 3.86             | 2.90             | 2.30             |
| 18.1       | 0.016          | 4.35             | 3.27             | 2.57             |
| 21.2       | 0.014          | 4.71             | 3.54             | 2.78             |
| 28.5       | 0.012          | 5.46             | 4.10             | 3.21             |
| 50.0       | 0.0094         | 7.24             | 5.44             | 4.25             |

Note: For Litz wire (multiple individually insulated strands twisted together), the skin effect correction factor approaches 1.0 at HF frequencies if the strand diameter is less than 2δ. Litz wire is beneficial for high-Q VHF inductors but is uncommon in the roller inductor due to high contact resistance at the roller surface.

---

### C.6 Self-Resonance Frequency Estimation

The self-resonant frequency (SRF) of a coil is determined by the distributed interwinding capacitance (C_stray). Approximately:

```
            1
f_SRF ≈ ───────────────────────────────────────
          2π √(L_total × C_stray_total)
```

Empirical rule of thumb for close-wound single-layer coils:

```
f_SRF(MHz) ≈ 25.33 / (N × d_inch)    [approximate]
```

Where N = total turns and d_inch = former diameter in inches. This is approximate to within a factor of 2; actual SRF depends on wire pitch, lead lengths, and mounting.

**Safe operating frequency:** < f_SRF / 2

For the 80 µH roller inductor at full engagement (95 turns on 3-inch former):

```
f_SRF ≈ 25.33 / (95 × 3.0) = 0.089 MHz = 89 kHz
```

At partial engagement (e.g., 10 turns, L ≈ 1 µH):

```
f_SRF ≈ 25.33 / (10 × 3.0) = 0.84 MHz = 840 kHz
```

The roller inductor is operated over its full range on the 1.85 MHz band (minimum L for SRF compatibility). At lower L settings (higher bands), the SRF is higher and there is no concern. On 160m, use only the inductance needed — do not engage more turns than required, as that unnecessarily reduces SRF toward the operating frequency.

---

## APPENDIX D — FIRMWARE COMMAND QUICK REFERENCE

**Bluetooth SPP / Serial Console Command Reference Card**

All commands are ASCII text, terminated with CR or CRLF. Responses from the tuner are terminated with CRLF.

| Command          | Syntax                        | Example                | Response                | Description                              |
|------------------|-------------------------------|------------------------|-------------------------|------------------------------------------|
| TUNE             | TUNE                          | TUNE                   | OK or ERR:reason        | Initiate automatic SWR minimization      |
| STOP             | STOP                          | STOP                   | OK                      | Halt tuning or motor movement immediately |
| HOME             | HOME {A\|B\|ALL}              | HOME ALL               | OK or ERR:reason        | Drive motor(s) to home position (limit)  |
| BAND             | BAND {n}                      | BAND 3                 | OK:40m or ERR:range     | Select band by index (0=160m, 10=6m)     |
| BAND?            | BAND?                         | BAND?                  | 40m                     | Query currently selected band            |
| SETL             | SETL {pct} or SETL uh:{val}   | SETL 42 / SETL uh:15.3 | OK:15.3 or ERR         | Set inductor by percent or µH value      |
| SETC             | SETC {pct} or SETC pf:{val}   | SETC 48 / SETC pf:245  | OK:245 or ERR          | Set capacitor by percent or pF value     |
| SWR?             | SWR?                          | SWR?                   | 1.45                    | Query current SWR reading (one decimal)  |
| FWD?             | FWD?                          | FWD?                   | 98.3                    | Query forward power in watts             |
| REF?             | REF?                          | REF?                   | 2.1                     | Query reflected power in watts           |
| STATUS           | STATUS                        | STATUS                 | JSON object             | Full system status as JSON string        |
| TOPO             | TOPO {L-HI\|L-LO\|PI\|T}     | TOPO L-HI              | OK or ERR               | Select matching network topology         |
| TOPO?            | TOPO?                         | TOPO?                  | L-HI                    | Query current network topology           |
| PRESET SAVE      | PRESET SAVE {n}               | PRESET SAVE 3          | OK or ERR               | Save current (L,C) as band n preset      |
| PRESET LOAD      | PRESET LOAD {n}               | PRESET LOAD 3          | OK:42,48 or ERR:no_pre  | Load band n preset and move motors       |
| PRESET LIST      | PRESET LIST                   | PRESET LIST            | Multi-line list         | List all 11 band presets                 |
| PRESET CLEAR     | PRESET CLEAR {n}              | PRESET CLEAR 3         | OK                      | Erase preset for band n                  |
| CAL              | CAL fwd:{watts}               | CAL fwd:10             | OK                      | Calibrate forward SWR bridge at watts    |
| CAL L            | CAL l_max:{uh}                | CAL l_max:79.5         | OK                      | Set measured L maximum value             |
| CAL C            | CAL c_max:{pf}                | CAL c_max:492          | OK                      | Set measured C maximum value             |
| RESET            | RESET                         | RESET                  | OK                      | Clear fault code, resume normal ops      |
| REBOOT           | REBOOT                        | REBOOT                 | OK (disconnects)        | Software reboot ESP32                    |
| WIFI             | WIFI {on\|off}                | WIFI on                | OK                      | Enable or disable WiFi AP                |
| WIFI?            | WIFI?                         | WIFI?                  | RF-HFTuner,192.168.4.1,2 | Query SSID, IP, and client count       |
| BTNAME           | BTNAME {name}                 | BTNAME MyTuner         | OK (reboot to apply)    | Set Bluetooth device name               |
| VER?             | VER?                          | VER?                   | TM-HFTN-001 Rev A 1.0.3 | Query firmware version string           |
| HELP             | HELP                          | HELP                   | Command summary list    | Print abbreviated command reference     |

**Band Index Reference**

| Index | Band  | Frequency Range  |
|-------|-------|-----------------|
| 0     | 160m  | 1.800–2.000 MHz  |
| 1     | 80m   | 3.500–4.000 MHz  |
| 2     | 60m   | 5.330–5.405 MHz  |
| 3     | 40m   | 7.000–7.300 MHz  |
| 4     | 30m   | 10.10–10.15 MHz  |
| 5     | 20m   | 14.00–14.35 MHz  |
| 6     | 17m   | 18.07–18.17 MHz  |
| 7     | 15m   | 21.00–21.45 MHz  |
| 8     | 12m   | 24.89–24.99 MHz  |
| 9     | 10m   | 28.00–29.70 MHz  |
| 10    | 6m    | 50.00–54.00 MHz  |

---

*END OF TECHNICAL MANUAL TM-HFTN-001 Rev A*

*MOTORIZED HF/VHF/UHF ANTENNA TUNING SYSTEM*

*CLASSIFICATION: UNCLASSIFIED*

*Document prepared by TM Engineering — 2026-05-02*
