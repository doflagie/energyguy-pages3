# TECHNICAL MANUAL
## TM-COUP-001 Rev A
### RF DIRECTIONAL COUPLERS AND POWER SAMPLERS
#### 160m through 23cm — HF Toroidal Bruene, Transmission-Line, Resistive Bridge, SWR Monitor

---

**DISTRIBUTION:** Unrestricted  
**CLASSIFICATION:** Unclassified  
**MANUAL NUMBER:** TM-COUP-001 Rev A  
**DATE:** 2026-04-26  
**AUTHORITY:** Field Engineering Reference — RF Power Monitoring Systems

---

## TABLE OF CONTENTS

1. [Introduction and System Overview](#chapter-1)
2. [Directional Coupler Theory](#chapter-2)
3. [HF Toroidal Bruene Coupler (1.8–30 MHz)](#chapter-3)
4. [Transmission-Line Coupler (50–1300 MHz)](#chapter-4)
5. [Resistive Bridge Coupler (DC–500 MHz)](#chapter-5)
6. [Detector Circuits and Power Measurement](#chapter-6)
7. [SWR Calculation Theory and Practice](#chapter-7)
8. [ESP32/CYD Monitor Firmware and Display](#chapter-8)
9. [Calibration Procedures](#chapter-9)
10. [PCB Layout and Construction Guidelines](#chapter-10)
11. [3D-Printed Enclosure Fabrication](#chapter-11)
12. [Frequency-Band Performance Tables](#chapter-12)
13. [Test and Verification Procedures](#chapter-13)
14. [Fault Isolation and Troubleshooting](#chapter-14)
15. [Safety: RF Exposure and High-Voltage Hazards](#chapter-15)

**Appendices:**  
A. [Quick Reference: Coupling, Insertion Loss, Directivity by Band](#appendix-a)  
B. [Toroid Core Selection Guide](#appendix-b)  
C. [Schottky Diode Selection and Matching](#appendix-c)  
D. [AD8307 Application Notes](#appendix-d)  
E. [BLE Data Protocol and Python Client](#appendix-e)

---

<a name="chapter-1"></a>
## CHAPTER 1 — INTRODUCTION AND SYSTEM OVERVIEW

### 1.1 Purpose

This manual provides complete design, construction, calibration, and troubleshooting procedures for the TM-COUP-001 series of portable RF directional couplers and power samplers. These instruments measure forward and reflected RF power simultaneously, enabling real-time SWR monitoring and power logging across all amateur radio HF/VHF/UHF bands without interrupting transmission.

### 1.2 System Configurations

**TM-COUP-001A — HF Toroidal Bruene Coupler (1.8–30 MHz)**  
Best directivity (>30 dB), lowest insertion loss (<0.05 dB), and excellent frequency flatness across all HF bands. Requires manually-wound toroid. SO-239 main line connectors; BNC coupled outputs.

**TM-COUP-001B — Transmission-Line Microstrip Coupler (50–1300 MHz)**  
PCB-etched coupled lines for VHF and UHF bands. Four band groups: 6m/2m/1.25m, 70cm/33cm, 23cm. SMA connectors throughout. Excellent phase tracking; directivity reduces above 500 MHz.

**TM-COUP-001C — Resistive Bridge Coupler (DC–500 MHz)**  
Wideband, transformer-free operation from DC to 500 MHz. Best for spectrum analyzer connection, pulse power measurement, and lab bench use. Higher insertion loss (0.4–0.5 dB) is acceptable trade-off for extreme bandwidth.

**TM-COUP-001D — Combined SWR Monitor (all bands)**  
All three coupler types feed into a common ESP32/CYD dual-channel detector. Displays forward power, reflected power, SWR, return loss, peak hold, and average power. BLE broadcast and WiFi API.

### 1.3 Design Objectives

| Parameter | HF (001A) | VHF/UHF (001B) | Wideband (001C) |
|-----------|-----------|----------------|-----------------|
| Frequency range | 1.8–30 MHz | 50–1300 MHz | DC–500 MHz |
| Coupling | −20, −30, −40 dB | −20 dB | −20 dB |
| Directivity | >30 dB | >25 dB | >20 dB |
| Insertion loss | <0.05 dB | <0.15 dB | <0.5 dB |
| Power handling | 100W CW | 50W CW | 100W (10s) |
| Impedance | 50Ω | 50Ω | 50Ω |
| Coupling flatness | ±0.5 dB | ±1.5 dB | ±2 dB |

### 1.4 Relationship to Other TM Manuals

- **TM-BMS-001:** Power source for field deployment (LiFePO4 battery with ESP32 BMS)
- **TM-PWR-001:** Power supplies; linear and switching PSU outputs feed transmitters
- **TM-MAST-001:** Antenna elevation; directional couplers typically installed between transceiver and feedline at the base of the antenna mast

---

<a name="chapter-2"></a>
## CHAPTER 2 — DIRECTIONAL COUPLER THEORY

### 2.1 Transmission Line Fundamentals

An RF transmission line supports two simultaneous traveling waves: a **forward wave** traveling from source to load, and a **reflected wave** traveling from the load back toward the source (whenever the load impedance deviates from the line's characteristic impedance Z₀).

The key parameters:

**Reflection coefficient Γ:**
```
Γ = (Z_L − Z₀) / (Z_L + Z₀)     [complex, magnitude 0 to 1]
```

**SWR (Voltage Standing Wave Ratio):**
```
SWR = (1 + |Γ|) / (1 − |Γ|)     [dimensionless, 1.0 to ∞]
```

**Return Loss (RL):**
```
RL = −20 log₁₀|Γ| = −10 log₁₀(P_ref/P_fwd)     [dB, positive = good]
```

**Mismatch Loss:**
```
ML = −10 log₁₀(1 − |Γ|²)     [dB]
```

A directional coupler separates the forward and reflected waves into two isolated output ports while passing the main signal through with minimal perturbation.

### 2.2 Coupler Parameters Defined

**Coupling Factor (C):**  
Ratio of coupled-port power to input power:
```
C [dB] = 10 log₁₀(P_coupled / P_input) = typically −20 to −40 dB
```
A −20 dB coupler delivers 1% of input power to the coupled port. The other 99% passes through to the load (minus insertion loss).

**Directivity (D):**  
Ratio of coupled-port response to isolated-port response:
```
D [dB] = 20 log₁₀(V_fwd / V_rev) with matched load (SWR=1)
```
High directivity means the coupler accurately distinguishes forward from reflected power. Directivity > 25 dB is required for SWR measurements accurate to ±0.1 SWR unit near SWR = 1.0.

**Why directivity matters for SWR accuracy:**

If directivity = 25 dB (V_rev / V_fwd = 0.056 leakage):  
- Actual SWR = 1.0 (matched), but coupler sees residual V_rev  
- Indicated reflection: |Γ_error| = 0.056  
- Indicated SWR: (1 + 0.056) / (1 − 0.056) = 1.12 (reads 1.12 when actually 1.0)

For accurate readings near SWR = 1.0, directivity > 30 dB is strongly preferred.

**Isolation (I):**
```
I [dB] = C − D = power ratio from input to isolated port
```
Isolation > 40 dB typical for quality couplers.

**Insertion Loss (IL):**  
Power lost from main line due to coupler presence:
```
IL [dB] = 10 log₁₀(P_through / P_input) = typically −0.05 to −0.5 dB
```

### 2.3 Four-Port Network Description

A directional coupler is a four-port network with an S-parameter matrix:

```
Port 1 (Input)    S-matrix:     Port 2 (Through)
Port 3 (Coupled)  ────────────  Port 4 (Isolated)
```

Ideal coupler S-parameters (lossless, −3 dB example for illustration):
```
     S11  S12   S13   S14      0   j/√2   1/√2   0
S =  S21  S22   S23   S24  =  j/√2  0     0    1/√2
     S31  S32   S33   S34    1/√2   0     0    j/√2
     S41  S42   S43   S44      0   1/√2  j/√2   0
```

For our −20 dB coupler: |S21| ≈ 1.0 (through), |S31| = 0.1 (coupled), S41 ≈ 0 (isolated).

### 2.4 Even and Odd Mode Analysis

The coupled-line coupler is analyzed by decomposing any excitation into even and odd modes:

**Even mode:** Both lines excited in phase. Fields between lines partially cancel; effective impedance Z_even > Z₀.

**Odd mode:** Lines excited 180° out of phase. Fields between lines reinforce; effective impedance Z_odd < Z₀.

```
Z_even × Z_odd = Z₀²    [geometric mean constraint]

Coupling: C = (Z_even − Z_odd) / (Z_even + Z_odd)
```

The coupling coefficient C is determined solely by the ratio of even to odd mode impedances, which is set by the physical gap between conductors. Larger gap → lower coupling (weaker coupling); smaller gap → higher coupling (tighter coupling).

### 2.5 Bruene Bridge Analysis

The Bruene (Sontheimer) bridge achieves directivity through simultaneous current and voltage sampling:

The transmission line supports:
```
V_line = V_fwd + V_rev     [voltage is sum of forward + reflected]
I_line = (V_fwd − V_rev) / Z₀  [current is difference / Z₀]
```

Therefore:
```
V_fwd = (V_line + I_line × Z₀) / 2
V_rev = (V_line − I_line × Z₀) / 2
```

The toroid T1 samples I_line (current transformer).  
The resistive divider samples V_line (voltage divider).  
Summing the two samples gives V_fwd; differencing gives V_rev.

**This is the key insight:** Forward and reflected waves are not detected by spatial separation (as in a physical stub coupler) but by algebraic combination of line voltage and current. This works at any frequency where the sampling network is electrically small relative to λ.

---

<a name="chapter-3"></a>
## CHAPTER 3 — HF TOROIDAL BRUENE COUPLER

### 3.1 Core Selection and Winding

The toroid core material determines the useful frequency range. Ferrite material (mix/type) governs complex permeability µ = µ' − jµ'':

| Material | Type | Frequency Range | AL (nH/T²) | Use |
|----------|------|-----------------|------------|-----|
| MnZn ferrite | Type 43 | 0.5–30 MHz | 523 (FT-50-43) | HF primary choice |
| MnZn ferrite | Type 77 | 0.1–10 MHz | 884 | 160m/80m emphasis |
| NiZn ferrite | Type 61 | 10–200 MHz | 173 (FT-50-61) | HF/VHF overlap |
| NiZn ferrite | Type 52 | 1–100 MHz | 250 (FT-50-52) | Wideband HF |

**Core size vs. power handling:**

| Core | OD (mm) | ID (mm) | Max V-µs product | Max power (single turn primary) |
|------|---------|---------|------------------|---------------------------------|
| FT-37-43 | 9.5 | 5.3 | 1,500 | 25W |
| FT-50-43 | 12.7 | 7.7 | 2,200 | 50W |
| FT-82-43 | 21.0 | 13.0 | 4,500 | 100W |
| FT-140-43 | 35.6 | 23.0 | 8,000 | 200W |
| FT-240-43 | 61.0 | 35.6 | 15,000 | 500W |

**Winding instructions (10-turn secondary for −20 dB):**

1. Cut 300mm of #24 AWG enameled magnet wire.
2. Wind 10 turns through the toroid core in the same rotational direction (no twist-back).
3. Spread turns evenly around ~270° of the core (leave 90° clear for the pass-through primary).
4. Pass the main transmission line conductor once through the core center (this is the 1-turn primary).
5. Strip and tin both secondary ends. Mark start with red marker.
6. Measure secondary inductance: L = AL × N² = 523 × 100 = 52,300 nH ≈ 52.3 µH (FT-50-43).
7. Verify: f_low = Z_L / (2π × L_sec) = 50 / (2π × 52.3e-6) = 152 kHz (well below 160m).

**Common winding errors:**
- Wind in both directions around core (capacitive coupling between half-turns; reduces directivity)
- Too few turns (poor low-frequency response; 160m coupling drops)
- Secondary too close to main line without shielding (stray capacitive coupling; reduces directivity at 28 MHz)

### 3.2 Balance Calibration

Maximum directivity requires that the voltage sample and current sample be equal in magnitude at the reference frequency (Z₀ = 50Ω).

**Null procedure:**
1. Connect 50Ω precision load (return loss > 40 dB) to output port.
2. Apply 5–10W CW to input at 14 MHz (20m reference).
3. Measure REF port DC voltage with DVM.
4. Adjust R_null trimmer (50Ω cermet, 10-turn) for minimum REF voltage.
5. Recheck at 7 MHz and 28 MHz; the null moves slightly with frequency.
6. Set null at most-used operating frequency.

**Typical achievable null depths:**

| Frequency | Typical directivity | Limiting factor |
|-----------|--------------------|-----------------|
| 1.8 MHz | 32–38 dB | Core flux; winding capacitance |
| 7 MHz | 35–45 dB | Diode matching |
| 14 MHz | 33–42 dB | Diode matching + winding C |
| 21 MHz | 28–36 dB | Winding parasitic C dominant |
| 28 MHz | 25–33 dB | Winding parasitic C dominant |

### 3.3 Construction Notes

**Main line conductor:**  
Use rigid 14 AWG bare copper wire or small-diameter coax center conductor for the pass-through. Keep the pass-through conductor centered in the toroid bore for repeatable coupling.

**Shielded secondary:**  
For best directivity above 14 MHz: pass the secondary through a short piece of coaxial cable (RG-174) before connecting to the summing network. The coax outer conductor is grounded; the center conductor is the secondary winding lead. This reduces capacitive coupling between secondary and main line.

**Ground plane:**  
Mount all components on a PCB with a full bottom-side ground plane. Keep all detector circuitry (diodes, capacitors, resistors) within 15mm of the toroid for minimum stray reactance.

---

<a name="chapter-4"></a>
## CHAPTER 4 — TRANSMISSION-LINE COUPLER

### 4.1 Coupled-Line Design Theory

Transmission-line couplers use the electromagnetic coupling between two parallel conductors in close proximity. The coupling is a function of:

1. **Physical gap** between conductor edges
2. **Conductor width** relative to substrate height
3. **Electrical length** of the coupled region (maximum coupling at λ/4)
4. **Substrate dielectric** (affects both coupling and λ/4 length)

For our −20 dB coupler on FR4 (εr = 4.4, h = 1.6mm):

```
Required: C = −20 dB → |C| = 0.100

Z_oe = Z₀ × √((1+C)/(1−C)) = 50 × √(1.1/0.9) = 50 × 1.106 = 55.3Ω
Z_oo = Z₀ × √((1−C)/(1+C)) = 50 × √(0.9/1.1) = 50 × 0.904 = 45.2Ω

Verify: Z_oe × Z_oo = 55.3 × 45.2 = 2500 = 50² ✓
```

**Microstrip line dimensions for Z_oe and Z_oo:**

Using Hammerstad-Jensen synthesis (software tools: AppCAD, Qucs, Saturn PCB):

| Impedance | Line width (w) | Gap (s) on FR4 1.6mm |
|-----------|---------------|----------------------|
| 50Ω (single) | 3.04mm | — |
| 55.3Ω (even) | 2.42mm | — |
| 45.2Ω (odd) | 3.82mm | — |
| Both (coupled) | ~2.7mm | ~1.0mm |

Note: The coupled-line dimensions differ from single-line dimensions because the coupling modifies the effective impedance. Use a coupled-line synthesis tool for final dimensions.

### 4.2 Quarter-Wave Lengths by Band

| Band | Frequency (MHz) | λ/4 in free space | λ/4 on FR4 (εr_eff ≈ 3.26) | Notes |
|------|----------------|-------------------|---------------------------|-------|
| 6m | 50 | 1500 mm | 832 mm | Too long for PCB; use lumped or hairpin |
| 2m | 144 | 521 mm | 289 mm | Use meander or multi-section |
| 1.25m | 222 | 338 mm | 187 mm | Use hairpin (fold in half) |
| 70cm | 432 | 174 mm | 96 mm | Fits on 100mm PCB with fold |
| 33cm | 902 | 83 mm | 46 mm | Compact; straightforward |
| 23cm | 1296 | 58 mm | 32 mm | Small; easy PCB layout |

**For 6m and 2m bands:** Use lumped LC equivalent circuits or coaxial construction (see Chapter 4.4). The λ/4 PCB sections are impractically long.

### 4.3 PCB Layout Guidelines for Low Directivity Error

**Critical layout rules for VHF/UHF couplers:**

1. **Terminate the isolated port locally.** The 50Ω termination resistor for Port 4 must be SMD (0402 or 0603), placed within 3mm of the port trace. Remote termination adds parasitic inductance and destroys directivity above 200 MHz.

2. **Match all four port line lengths.** Unequal trace lengths from coupler to connectors cause phase errors. Error of 1mm at 432 MHz = 2.3° of electrical length → ~0.3 dB directivity reduction.

3. **SMA connector transition.** The SMA pin soldered to the PCB introduces ~0.5 nH of parasitic inductance. Compensate with a 1pF capacitor from each SMA pin to ground, or taper the trace at the connector.

4. **Ground vias.** Place ground vias on 3mm grid throughout the ground plane. At 1296 MHz, a via grid spacing of λ/20 (≈15mm) is the maximum. Without adequate vias, the ground plane resonates.

5. **Solder mask clearance.** Remove solder mask (bare copper) over the coupled-line section. Solder mask εr (≈3.5) changes the effective εr of the coupled region and shifts coupling frequency.

### 4.4 Coaxial Construction for 50–300 MHz

For the 6m and 2m bands where PCB λ/4 sections are impractically large, a coaxial construction directional coupler is both practical and achieves excellent performance.

**Construction steps (50mm sleeve coupler):**

1. Cut a 60mm length of 10mm ID × 12mm OD brass tubing (outer conductor).
2. Cut a 60mm length of 4mm OD brass rod (inner conductor, Z₀ = 55Ω).
3. Cut a 30mm length of 1.5mm OD bare copper wire (coupling element).
4. Solder SMA connectors at each end of the brass assembly.
5. Position the coupling wire parallel to the inner conductor, held in a PTFE (Teflon) sleeve cut from 1/4" PTFE rod.
6. Solder the coupling wire to FWD and REF SMA ports.
7. Install the assembly in the coaxial sleeve housing (enclosure_vhf_coupler.scad: coax_sleeve_housing()).

**Coupling empirical calibration:**
Apply known power (1W CW at 144 MHz) and measure FWD port output. Adjust coupling wire length for desired −20 dB response.

---

<a name="chapter-5"></a>
## CHAPTER 5 — RESISTIVE BRIDGE COUPLER

### 5.1 Operating Principle

Unlike the toroidal or transmission-line coupler, the resistive bridge operates on purely lumped-element principles. It has no inherent frequency-dependent behavior (reactive elements are minimized) and therefore works from DC to the frequency where parasitic reactances of the resistors and circuit layout become significant.

The fundamental circuit is a Wheatstone bridge with the line under test replacing one bridge arm.

**Key advantage:** Flat response from DC to hundreds of MHz. A single instrument covers 160m through 70cm without band switching.

**Key disadvantage:** Insertion loss is fundamental to the design. Approximately 0.4–0.5 dB insertion loss is unavoidable in a practical 50Ω-to-50Ω resistive bridge. This is unacceptable for an amplifier stage output monitor but acceptable for an inline station monitor where 0.5 dB is tolerable.

### 5.2 Null Trimmer Adjustment

The resistive bridge has a null trimmer (R_null, 50Ω 10-turn cermet) that allows fine adjustment of directivity. Unlike the toroidal coupler where the null is set at one frequency and degrades at others, the resistive bridge null is frequency-independent (the resistor network has no frequency dependence at DC–50 MHz).

**Null procedure:**
1. Apply 1W CW to the input port (any frequency; resistive bridge is frequency-flat).
2. Measure V_ref at the reflected port with a sensitive DVM (1mV resolution).
3. Slowly adjust R_null until V_ref is minimized. Typical null: V_ref < 2mV for a 1W input.
4. Lock trimmer with a drop of thread-lock compound.

After adjustment, directivity is limited by:
- Resistor tolerance (1% resistors: ≈34 dB limit)
- Layout parasitics above 100 MHz
- Diode matching (see Appendix C)

---

<a name="chapter-6"></a>
## CHAPTER 6 — DETECTOR CIRCUITS AND POWER MEASUREMENT

### 6.1 Schottky Diode Peak Detector

The Schottky diode peak detector is the simplest and most common detector for amateur radio SWR meters. The diode rectifies the coupled RF voltage; the RC filter averages the peak:

```
V_dc = V_peak_RF − V_f_diode
```

Where V_f is the forward voltage drop of the Schottky diode (typ. 0.20–0.35V at the operating current).

**Frequency response of the peak detector:**

The detector's RC time constant must be short compared to the measurement update interval but long compared to the RF period:

```
1/f_RF << RC << 1/f_update
At 1.8 MHz: T_RF = 556 ns
RC filter: R_L × C = 2.2kΩ × 1nF = 2.2µs >> 556 ns ✓
Update rate: 50 Hz → T_update = 20ms >> 2.2µs ✓
```

This is satisfied for all amateur bands. However, at 1296 MHz (23cm), the RC must be reduced to allow the detector to follow fast RF envelopes:

```
At 1296 MHz: T_RF = 0.77 ns
RC = 2.2kΩ × 10pF = 22 ns >> 0.77 ns ✓
```

Use 10 pF detector capacitors for UHF bands; 100–1000 pF for HF.

### 6.2 AD8307 Logarithmic Amplifier

The AD8307 provides 92 dB of dynamic range in a single IC, ideal when accurate power measurement at both very low power (QRP, <1W) and full power (100W) is needed simultaneously.

**Transfer function:**
```
V_out = 0.025 × (P_dBm + 84)     [Volts; P_dBm = power into AD8307 in dBm]
```

**To find main line power from AD8307 output:**
```
P_main_dBm = (V_adc_mv / 25) − 84 + |coupling_dB|
P_main_W = 10^((P_main_dBm − 30) / 10)
```

**Example (−20 dB coupler, 50W transmitter):**
```
P_main = 50W = 47.0 dBm
P_coupled = 47.0 − 20 = 27.0 dBm
V_AD8307 = 0.025 × (27.0 + 84) = 0.025 × 111 = 2.775V
```

This is within the ESP32 ADC range (3.3V max) — no voltage divider needed for ≤50W with −20 dB coupler.

**For 100W (50 dBm):** V_AD8307 = 0.025 × (30 + 84) = 2.85V — still within range.  
**For 200W (53 dBm):** V_AD8307 = 0.025 × (33 + 84) = 2.925V — marginal; use −30 dB coupler for 200W+.

### 6.3 True RMS Detection for SSB

A CW-calibrated peak detector indicates higher power than actually exists when measuring SSB (single sideband) because SSB power varies with voice modulation.

**The problem:**  
A 100W PEP SSB signal has a **peak** power of 100W but an **average** power of 25–35W (depending on audio content). A peak detector reads 100W; an RMS detector reads 25–35W. Neither is "wrong" — they measure different things.

**For SSB, the correct reading is average power (RMS):**  
Use the AD8361 true-RMS detector (DC–2.5 GHz; 36 dB dynamic range) for accurate SSB power measurement. The ESP32 firmware has a runtime-selectable detector mode flag.

**Meter interpretation guidance (for CYD display):**

| Mode | Reading | Meaning |
|------|---------|---------|
| CW | Peak = PEP | Carrier power (same as average for CW) |
| FM | Peak = Average = PEP | Constant envelope modulation |
| SSB (voice) | Peak >> Average | PEP shown; average ≈ 25–35% of PEP |
| Digital (FT8) | Peak ≈ Average ≈ PEP | Constant envelope, full duty cycle |

---

<a name="chapter-7"></a>
## CHAPTER 7 — SWR CALCULATION THEORY AND PRACTICE

### 7.1 SWR from Power Measurements

Given forward power P_fwd and reflected power P_ref:

```
|Γ| = √(P_ref / P_fwd)      [voltage reflection coefficient magnitude]
SWR = (1 + |Γ|) / (1 − |Γ|)
Return Loss = −10 log₁₀(P_ref / P_fwd)  [dB]
Mismatch Loss = −10 log₁₀(1 − |Γ|²)    [dB]
```

### 7.2 SWR Accuracy vs. Directivity

A coupler with finite directivity D introduces a measurement error in the reflected port. The apparent reflection coefficient has an error bounded by:

```
|Γ_error| ≤ 10^(−D/20)
```

For D = 25 dB: |Γ_error| ≤ 0.056 → SWR error up to 1.12 on a matched load.  
For D = 30 dB: |Γ_error| ≤ 0.032 → SWR error up to 1.07 on a matched load.  
For D = 40 dB: |Γ_error| ≤ 0.010 → SWR error up to 1.02 on a matched load.

**Practical guideline:** For SWR measurements that need to be accurate to better than ±0.1 SWR unit near SWR = 1.5, require D > 28 dB. The HF toroidal coupler (D > 30 dB) meets this criterion. The VHF transmission-line coupler (D > 25 dB) is adequate for most field use.

### 7.3 SWR and System Effects

**Power delivered to antenna:**
```
P_antenna = P_fwd × (1 − |Γ|²)
```

For SWR = 2: |Γ| = 1/3; P_ant = P_fwd × (1 − 1/9) = 0.889 × P_fwd  
For SWR = 3: |Γ| = 1/2; P_ant = P_fwd × (1 − 1/4) = 0.750 × P_fwd  

**Feedline loss effect on SWR:**  
Loss in the feedline between the coupler and the antenna attenuates both the forward and reflected waves. The coupler measures SWR at the **coupler location** — not at the antenna. A lossy line makes the SWR appear lower than it really is at the antenna. Install the coupler as close to the antenna as possible for accurate antenna SWR measurement.

**Coupler measurement convention:**  
In this system, the coupler measures SWR at the coupler location. The convention is:
- Coupler IN port = transmitter side
- Coupler OUT port = antenna/feedline side
- FWD port = forward wave (transmitter → antenna)
- REF port = reflected wave (antenna → transmitter)

---

<a name="chapter-8"></a>
## CHAPTER 8 — ESP32/CYD MONITOR FIRMWARE AND DISPLAY

### 8.1 Hardware Configuration

```
ESP32 (CYD) GPIO Assignments:
  GPIO 34  — FWD detector ADC input (ADC1_CH6; input-only, no internal pull-up)
  GPIO 35  — REF detector ADC input (ADC1_CH7; input-only)
  GPIO 25  — Rotary encoder CLK
  GPIO 26  — Rotary encoder DT
  GPIO 27  — Rotary encoder switch (PUSH)
  GPIO 13  — Reference LED (CYD onboard LED; also reference output indicator)
  GPIO 33  — Touch controller CS (XPT2046)
  CYD TFT  — ILI9341 320×240 (built-in to CYD; SPI bus)

  External voltage divider on ADC inputs:
    R_top = 30kΩ, R_bot = 10kΩ → V_adc = V_detector × 0.25
    Maximum detector output: 3.3V / 0.25 = 13.2V
    Maximum coupler output at 100W: V_det ≈ 9.8V → V_adc = 2.45V ✓
```

### 8.2 ADC Nonlinearity Correction

The ESP32 built-in ADC has approximately ±2% nonlinearity, particularly at the midpoint of its range. The firmware applies a first-order correction:

```cpp
// Linearization (empirical; adjust slope/offset for your ESP32 unit)
v_corrected_mv = v_raw_mv * 1.0218f − 10.5f;
```

For calibration-grade accuracy, measure a series of known DC voltages (from a precision voltage reference or calibrated power level) and fit the correction coefficients for your specific ESP32.

### 8.3 Band Calibration Tables

Each band entry in the `BANDS[]` array contains:
- `coupling_db`: The measured coupling of the installed coupler at this frequency
- `cal_fwd_k`: Calibration constant k where P_fwd [W] = (V_detector_mV)² × k
- `vf_correction`: Schottky diode forward voltage at this band's frequency (mV)

To calibrate a new band:
1. Apply known power (measured on a reference wattmeter) at the target band.
2. Record V_fwd_mv from the CYD debug display.
3. Calculate: k = P_known_W / (V_fwd_mv)²
4. Update BANDS[] entry; re-flash firmware.

### 8.4 CYD Display Layout

```
┌─────────────────────────────────────────────────────────────────────┐
│  RF POWER MONITOR                            [20m]  14.25 MHz       │
│                                                                      │
│  FWD  ██████████████████████████░░░░░░    95.3 W    47.8 dBm       │
│       [▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓|░░░░░░░░░░]  peak mark     │
│                                                                      │
│  REF  ████░░░░░░░░░░░░░░░░░░░░░░░░░░░░     1.2 W    30.8 dBm       │
│       [▓▓▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░]               │
│                                                                      │
│  SWR  1.24:1     ▐█░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▌  RL: 20.2 dB │
│       [SWR=1.5] [SWR=2.0]  markers                                  │
│                                                                      │
│  PK: 97.1 W    AVG: 93.8 W                                         │
│                                                                      │
│  [40m]  [20m]  [2m]  [70cm]                                BLE     │
└─────────────────────────────────────────────────────────────────────┘

Color coding:
  SWR < 1.5:  Green
  SWR 1.5–2.5: Yellow
  SWR 2.5–4.0: Orange
  SWR > 4.0:   Red (also activates reference LED on GPIO 13)
```

### 8.5 BLE Data Protocol

Device name: `RF-COUP-001`  
Profile: Nordic UART Service (NUS) — same as TM-BMS-001 and TM-MAST-001

**TX (notify, 2 Hz):**
```
BAND=20m\nFWD=95.30\nREF=1.20\nSWR=1.24\nRL=20.2\nPEAK=97.1\n
```

**RX (write from client):**
```
BAND=20m\n    — select band by name
BAND=4\n      — select band by index
```

---

<a name="chapter-9"></a>
## CHAPTER 9 — CALIBRATION PROCEDURES

### 9.1 Required Equipment

- **Reference wattmeter:** Bird Model 43 or equivalent (±5% accuracy), OR calibrated attenuator + spectrum analyzer
- **Precision 50Ω termination:** RL > 40 dB at all test frequencies
- **CW signal source:** Transceiver or signal generator
- **Digital multimeter:** 1mV DC resolution
- **Optional:** VNA (NanoVNA, nanoVNA-H4) for coupling and directivity plots

### 9.2 Step 1 — Insertion Loss Verification

1. Connect reference wattmeter in-line (no coupler) between 5W source and 50Ω load. Record P_reference.
2. Install coupler in-line. Measure P_through with same wattmeter at output port.
3. IL = 10 × log₁₀(P_through / P_reference).
4. Accept: IL < 0.05 dB (toroidal), < 0.20 dB (transmission-line), < 0.60 dB (resistive).

### 9.3 Step 2 — Coupling Factor Measurement

1. Connect 50Ω precision load to output port.
2. Apply known power P_input (measure with reference wattmeter at input).
3. Measure FWD port DC voltage V_fwd.
4. Calculate V_RF_fwd ≈ V_fwd + V_f (add back diode V_f).
5. P_coupled = V_RF_fwd² / (2 × 50) = V² / 100.
6. Coupling = 10 × log₁₀(P_coupled / P_input) — compare to design value.

### 9.4 Step 3 — Directivity Measurement

1. Precision 50Ω load on output; known power on input.
2. Measure V_fwd and V_ref (DC voltage at each detector output).
3. Directivity = 20 × log₁₀(V_fwd / V_ref).
4. Accept: D > 30 dB (toroidal HF), > 25 dB (transmission-line VHF).

### 9.5 Step 4 — SWR Accuracy Verification

1. Install precision 50Ω load (known SWR = 1.0:1).
2. Apply power; verify CYD reads SWR ≤ 1.10:1.
3. Install known 2:1 load (e.g., 100Ω resistor in 50Ω system; SWR = 2.00:1).
4. Verify CYD reads SWR = 2.00 ± 0.10.
5. Install open circuit (SWR = ∞). Verify CYD reads SWR > 20:1 (effectively unlimited).

### 9.6 Calibration Log

```
COUPLER CALIBRATION RECORD — TM-COUP-001A
Serial/Build:  _______________  Date: _______________
Technician:    _______________  Ambient T: _____ °C

Insertion Loss:
  1.8 MHz: _____ dB  (limit: <0.10 dB)
  14 MHz:  _____ dB  (limit: <0.05 dB)
  28 MHz:  _____ dB  (limit: <0.08 dB)

Coupling Factor (at 50Ω load, 10W input):
  1.8 MHz: _____ dB  (nominal: −20.0 ±1.0 dB)
  14 MHz:  _____ dB
  28 MHz:  _____ dB

Directivity:
  1.8 MHz: _____ dB  (limit: >25 dB)
  7 MHz:   _____ dB  (limit: >28 dB)
  14 MHz:  _____ dB  (limit: >28 dB)
  21 MHz:  _____ dB  (limit: >25 dB)
  28 MHz:  _____ dB  (limit: >23 dB)

SWR Accuracy:
  At 1.0:1 (50Ω load): indicated SWR = _____  (limit: ≤1.10:1)
  At 2.0:1 (100Ω load): indicated SWR = _____ (limit: 1.90–2.10:1)
```

---

<a name="chapter-10"></a>
## CHAPTER 10 — PCB LAYOUT AND CONSTRUCTION GUIDELINES

### 10.1 HF Coupler PCB (≤30 MHz)

The HF coupler PCB is not frequency-critical — wavelengths at 28 MHz are 10+ meters, so microstrip geometry is irrelevant. Layout priorities:

1. **Full ground plane** on bottom layer, no interruptions
2. **Short, direct traces** from toroid secondary to summing resistors to diodes
3. **Star ground** for detector circuits; all grounds meet at single PCB point near SO-239 shell
4. **Separation:** Keep summing resistors and detector network at least 10mm from toroid
5. **Shield can** optional but improves directivity above 21 MHz

### 10.2 VHF/UHF PCB (50–1296 MHz)

Layout is critical. Follow these rules strictly:

1. **Controlled impedance lines**: All signal traces must be 50Ω microstrip (3.04mm wide on FR4 1.6mm with 1oz copper)
2. **Ground vias**: Every 3mm grid; tie top and bottom ground planes together continuously
3. **SMD only**: No through-hole components in the RF signal path (stray inductance)
4. **Coupled line geometry**: Maintain ±0.1mm dimensional accuracy; use DRC in KiCad
5. **Component placement**: All SMD RF components within 2mm of their circuit nodes
6. **No 90° trace bends**: Use 45° or curved bends to reduce reflections above 1 GHz
7. **Board outline**: PCB edge must have plated edge vias or be metal-enclosed for shielding

### 10.3 Soldering and Assembly

**HF coupler assembly sequence:**
1. Install SO-239 connectors first; align and solder
2. Install PCB standoff mounting holes (heat inserts via soldering iron)
3. Wind and install toroid T1
4. Solder summing resistors R1, R2, R_T
5. Match and install diode pair D1/D2 (measure Vf before installing)
6. Install detector load resistors and filter capacitors
7. Install null trimmer R_null last (requires calibration access)

**VHF/UHF PCB assembly sequence:**
1. Reflow paste and place SMD components; reflow oven preferred
2. Hand-solder SMA connectors to PCB edges (use stencil for alignment)
3. Install isolation resistor (51Ω, Port 4)
4. Verify ground via continuity with multimeter before RF testing

---

<a name="chapter-11"></a>
## CHAPTER 11 — 3D-PRINTED ENCLOSURE FABRICATION

### 11.1 Print Specifications

| Parameter | HF Enclosure | VHF/UHF Enclosure |
|-----------|-------------|-------------------|
| Material | PETG | PETG (≤500 MHz) or ABS (>500 MHz) |
| Layer height | 0.2mm | 0.15mm (finer detail at SMA) |
| Perimeters | 4 | 4 |
| Infill | 30% gyroid | 25% gyroid |
| Top/bottom | 5 layers | 5 layers |

**Why material matters above 500 MHz:**  
At 1296 MHz, the enclosure material is electrically close to the conductors. PETG (εr ≈ 3.0, tan_δ ≈ 0.008) introduces moderate dielectric loss. ABS (εr ≈ 2.5, tan_δ ≈ 0.005) is slightly better. Neither affects insertion loss measurably when the enclosure wall is ≥5mm from the RF line.

### 11.2 Connector Hole Accuracy

Connector holes require accurate dimensions for reliable RF ground connections:

| Connector | Bore | Tolerance | Notes |
|-----------|------|-----------|-------|
| SO-239 | 29.2mm | ±0.3mm | Snug fit; nuts on back |
| BNC chassis | 12.4mm | ±0.2mm | |
| SMA chassis | 6.5mm | ±0.15mm | Critical — loose = poor ground |
| N-type | 16.0mm | ±0.3mm | |

If your printer runs tight: add 0.2mm to bore diameters in the OpenSCAD file before slicing (set TOL = 0.4 instead of 0.3).

### 11.3 RF Grounding of Connectors

The connector shell must make direct metal-to-metal contact with the enclosure ground plane. Plastic enclosures do not conduct. Two methods:

**Method A — Foil liner (recommended):**  
Cut 0.1mm copper foil to line the interior of the printed enclosure. Solder to SO-239/SMA shells. Connect to PCB ground plane with short wire.

**Method B — Conductive paint:**  
Brush apply MG Chemicals 843 silver conductive paint to interior surfaces. Two coats; dry 24 hours. Connect to PCB ground at each corner.

**Method C — Accept plastic enclosure (lowest performance):**  
For the HF toroidal coupler at ≤30 MHz, the unshielded plastic enclosure introduces minimal degradation. Directivity may be reduced by 2–3 dB at 28 MHz due to stray coupling through the enclosure. Acceptable for field use.

---

<a name="chapter-12"></a>
## CHAPTER 12 — FREQUENCY-BAND PERFORMANCE TABLES

### 12.1 TM-COUP-001A — HF Toroidal Coupler (FT-50-43, 10T, −20 dB)

| Band | Freq (MHz) | Coupling | Directivity | IL | Notes |
|------|-----------|----------|-------------|-----|-------|
| 160m | 1.85 | −20.5 dB | >32 dB | 0.02 dB | Core flux at limit; adequate |
| 80m | 3.65 | −20.2 dB | >35 dB | 0.02 dB | Excellent |
| 40m | 7.10 | −20.1 dB | >37 dB | 0.02 dB | Best performance |
| 30m | 10.13 | −20.0 dB | >36 dB | 0.02 dB | Excellent |
| 20m | 14.25 | −20.0 dB | >34 dB | 0.02 dB | Reference band |
| 17m | 18.12 | −20.1 dB | >31 dB | 0.03 dB | Good |
| 15m | 21.27 | −20.2 dB | >30 dB | 0.03 dB | Good |
| 12m | 24.94 | −20.4 dB | >27 dB | 0.04 dB | Winding capacitance effect |
| 10m | 28.50 | −20.8 dB | >25 dB | 0.05 dB | Use FT-50-61 for better |

### 12.2 TM-COUP-001B — Transmission-Line Coupler (432 MHz design)

| Band | Freq (MHz) | Coupling | Directivity | IL | Notes |
|------|-----------|----------|-------------|-----|-------|
| 6m | 50 | −20 ±3 dB | >20 dB | <0.2 dB | Use coax construction |
| 2m | 144 | −20 ±2 dB | >25 dB | <0.15 dB | Hairpin or coax |
| 1.25m | 222 | −20 ±1.5 dB | >27 dB | <0.12 dB | PCB hairpin |
| 70cm | 432 | −20 ±0.5 dB | >28 dB | <0.10 dB | Optimum design point |
| 33cm | 902 | −20 ±1 dB | >24 dB | <0.12 dB | Good |
| 23cm | 1296 | −20 ±1 dB | >20 dB | <0.18 dB | Phase comp. needed |

### 12.3 TM-COUP-001C — Resistive Bridge (DC–500 MHz)

| Frequency | Coupling | Directivity | IL |
|-----------|----------|-------------|-----|
| DC–10 MHz | −20 ±0.1 dB | >30 dB | 0.43 dB |
| 10–100 MHz | −20 ±0.5 dB | >25 dB | 0.45 dB |
| 100–300 MHz | −20 ±1.5 dB | >20 dB | 0.50 dB |
| 300–500 MHz | −20 ±3 dB | >15 dB | 0.60 dB |

---

<a name="chapter-13"></a>
## CHAPTER 13 — TEST AND VERIFICATION PROCEDURES

### 13.1 Bench Verification (Pre-Field)

```
TEST 1 — DC CONTINUITY
  Measure with DVM: All SO-239/SMA shells to GND pad = 0Ω
  Main line IN to OUT (center pins): 0Ω (copper pass-through)
  FWD port to REF port: ∞Ω (isolated)
  RESULT: Pass / Fail: _______

TEST 2 — DIODE CHECK
  Set DVM to diode check.
  D1 forward: 0.20–0.35V (BAT41) / 0.20–0.28V (BAT85)
  D1 reverse: OL (overload; not conducting)
  D2: same range within 5mV of D1
  RESULT: Pass / Fail: _______

TEST 3 — CYD STARTUP
  Power on 5V USB. Display initializes to splash screen within 2 seconds.
  Band selection shows "20m" default.
  BLE device "RF-COUP-001" visible from BLE scanner app.
  RESULT: Pass / Fail: _______

TEST 4 — LOW-LEVEL RESPONSE
  Apply 100mW (20 dBm) CW to IN port; 50Ω load on OUT.
  Schottky detector (−20 dB coupler): V_fwd_det = 0.77V nominal.
  ADC after 4:1 divider: V_adc = 0.19V → ~240 ADC counts.
  CYD should display: FWD = 100 mW ±15%.
  RESULT: _______

TEST 5 — SWR READING (KNOWN LOADS)
  50Ω load: CYD SWR ≤ 1.10:1
  100Ω load: CYD SWR = 2.00 ± 0.15
  Open circuit: CYD SWR > 15:1
  RESULT: Pass / Fail: _______
```

### 13.2 Field Verification

Before each operating session:

```
□ Visual: No cracked solder joints at connectors; no loose hardware
□ Connector shells tight (SO-239 nut, SMA nut)
□ Cable connections secure at both ends of feedline
□ CYD displays correct band for operating frequency
□ Low-power test: 1W CW → FWD shows 1.0 ±0.2W
□ SWR with known-good antenna: reasonable reading (SWR < 2.5 on a tuned antenna)
```

---

<a name="chapter-14"></a>
## CHAPTER 14 — FAULT ISOLATION AND TROUBLESHOOTING

### 14.1 Troubleshooting Philosophy

When a directional coupler malfunctions, trace the signal path from the RF source through the coupler to each output port. Follow the electron path:

1. Does RF pass through the main line? (Check insertion loss)
2. Does the FWD port respond to RF? (Check coupling)
3. Does the REF port respond to high-SWR load? (Check directivity)
4. Does the detector convert RF to DC? (Check diode and filter)
5. Does the ADC read the DC? (Check wiring and firmware)

### 14.2 Fault Table

| Symptom | Probable Cause | Test | Repair |
|---------|---------------|------|--------|
| CYD shows FWD=0 with known RF applied | Diode D1 open; trace break; ADC fault | Measure V_fwd_det with DVM | Replace D1; reflow trace; check ADC pin |
| FWD reads high, REF reads 0 always | D2 open; REF path break | DVM on REF det | Replace D2 |
| SWR reads >10 on matched load | Directivity poor; diode mismatch; bridge null off | Adjust R_null; check diode Vf match | Null adjustment; replace diodes as matched pair |
| Insertion loss high (antenna VSWR correct, FWD low) | Core saturated (overpower); shorted capacitor on main line | Remove coupler; check with dummy load | Replace core; check bypass caps |
| CYD reads same power on all bands | Band calibration table not loaded; firmware default | Check BANDS[] array in firmware | Re-flash with correct band constants |
| FWD reading unstable / noisy | Poor ground connection at SO-239; cold solder | Wiggle connectors; DC continuity | Reflow connector grounds |
| Power reads correct but SWR wrong | Coupling factor different FWD vs. REF (detector mismatch) | Measure V_fwd / V_ref ratio at known SWR | Calibrate cal_fwd_k vs. cal_ref_k separately |
| CYD no BLE advertisement | BLEDevice::init() not called; BLE antenna poor | BLE scanner on phone | Check firmware init; CYD antenna position |
| Display correct but web API gives wrong band | band_idx not saved; WiFi STA/AP conflict | Open http://192.168.4.1/api/status | Check WiFi.softAP() call order |

---

<a name="chapter-15"></a>
## CHAPTER 15 — SAFETY: RF EXPOSURE AND HIGH-VOLTAGE HAZARDS

### 15.1 RF Voltage at Coupler Connectors

The coupler is placed **in series** with the transmitter output. The full RF voltage is present at the IN and OUT (THROUGH) connectors. At 100W into 50Ω:

```
V_peak = √(2 × P × Z₀) = √(2 × 100 × 50) = 100V_peak
V_RMS  = √(P × Z₀) = √5000 = 70.7V_RMS
```

**70V RMS RF is capable of causing RF burns and electrical shock.** Do not touch the center conductor of any connected connector during transmission. Disconnect all power before servicing.

**Coupled port voltages** (after −20 dB coupler at 100W):
```
V_coupled_peak = 100V / 10 = 10V_peak (DC after detector ≈ 9.8V)
```
Coupled port voltages are low enough to be generally safe, but still RF. Treat as hazardous.

### 15.2 RF Exposure (EMF)

The coupler is passive; it does not transmit. However, the main transmission line carries full transmitter power. Maintain FCC/ICNIRP RF exposure compliance for the transmitting antenna system. The coupler itself does not increase RF exposure.

### 15.3 Connector Torque

Over-torqued SMA connectors crack the plastic dielectric and permanently degrade VSWR performance. Use a torque wrench:

| Connector | Torque | Notes |
|-----------|--------|-------|
| SMA (standard) | 0.56 N·m (5 in-lb) | Do not exceed |
| N-type | 1.36 N·m (12 in-lb) | |
| SO-239 (PL-259 mating) | Hand-tight + 1/8 turn | |

### 15.4 Temperature Limits

- **Operating:** −20°C to +60°C (PETG enclosure; electronics)
- **Storage:** −40°C to +85°C
- **Core temperature at rated power (100W CW):** Core surface <70°C (ferrite OK to 120°C)
- **Detector diodes:** 125°C junction maximum; operating <80°C junction

If coupler feels warm after sustained CW operation: normal up to 40°C above ambient. If too hot to touch (>60°C surface): reduce power, inspect for short circuit at main line.

---

<a name="appendix-a"></a>
## APPENDIX A — QUICK REFERENCE: COUPLING, INSERTION LOSS, DIRECTIVITY

### A-1: Coupling Factor by Number of Turns (Toroidal, 50Ω)

| N (turns) | Coupling | Pwr at coupled port (100W in) |
|-----------|----------|-------------------------------|
| 10 | −20.0 dB | 1.00 W |
| 15 | −23.5 dB | 447 mW |
| 20 | −26.0 dB | 250 mW |
| 32 | −30.1 dB | 97 mW |
| 50 | −34.0 dB | 40 mW |
| 100 | −40.0 dB | 10 mW |

### A-2: SWR to Return Loss / Reflection Coefficient

| SWR | |Γ| | Return Loss | Mismatch Loss |
|-----|------|-------------|---------------|
| 1.0 | 0.000 | ∞ dB | 0.00 dB |
| 1.1 | 0.048 | 26.4 dB | 0.01 dB |
| 1.5 | 0.200 | 14.0 dB | 0.18 dB |
| 2.0 | 0.333 | 9.5 dB | 0.51 dB |
| 3.0 | 0.500 | 6.0 dB | 1.25 dB |
| 5.0 | 0.667 | 3.5 dB | 3.01 dB |
| 10.0 | 0.818 | 1.7 dB | 7.4 dB |
| ∞ | 1.000 | 0.0 dB | ∞ dB |

---

<a name="appendix-b"></a>
## APPENDIX B — TOROID CORE SELECTION GUIDE

### B-1: Core Material Selection by Frequency

| Mix | Name | f range | µ_i | Key property |
|-----|------|---------|-----|--------------|
| 43 | MnZn | 0.5–30 MHz | 850 | Best HF; high AL |
| 77 | MnZn | 0.1–10 MHz | 2000 | 160m/80m; very high AL |
| 61 | NiZn | 10–200 MHz | 125 | VHF; low loss |
| 52 | NiZn | 1–100 MHz | 250 | Wideband HF/VHF |
| 67 | NiZn | 50–500 MHz | 40 | VHF/UHF; very low loss |

### B-2: Core Size and Power Rating (1-turn primary pass-through)

| Core | OD | Window | Flux density (100W) | Max sustained power |
|------|-----|--------|---------------------|---------------------|
| FT-37 | 9.5mm | 5.3mm | High | 25W |
| FT-50 | 12.7mm | 7.7mm | Moderate | 50W |
| FT-82 | 21.0mm | 13.0mm | Low | 100W |
| FT-140 | 35.6mm | 23.0mm | Very low | 200W |
| FT-240 | 61.0mm | 35.6mm | Negligible | 500W |

**For 100W CW on 160m (1.8 MHz):** Use FT-82-43 minimum; FT-140-43 preferred.  
At 1.8 MHz the flux density in the core is highest (V × t / N × Ae); larger core needed.

---

<a name="appendix-c"></a>
## APPENDIX C — SCHOTTKY DIODE SELECTION AND MATCHING

### C-1: Diode Selection

| Part | Vf at 1mA | f_max | Vr | Package | Notes |
|------|-----------|-------|-----|---------|-------|
| BAT41 | 220–280 mV | 300 MHz | 100V | SOD-123 | Best HF; buy in quantity |
| BAT85 | 190–250 mV | 1 GHz | 30V | SOD-23 | Good VHF/UHF |
| 1N5711 | 350–400 mV | 500 MHz | 70V | DO-35 | Old standard; higher Vf |
| HSMS-2850 | 150–200 mV | 2 GHz | 15V | SOT-23 | Best UHF; lower Vf |
| SMS7621 | 200–250 mV | 3 GHz | 7V | SC-70 | 23cm band |

### C-2: Matching Procedure

1. Set DVM to diode test mode (applies ~1mA current, reads forward voltage).
2. Measure Vf of 10–20 diodes from the same batch.
3. Select a matched pair where Vf differs by < 5mV.
4. Record the matched Vf for firmware calibration (vf_correction parameter).

**Why matching matters:** Each mV of Vf mismatch causes ~0.3% error in the detector output voltage, translating to ~0.1 dB coupling error. For directivity > 30 dB, match Vf within 2mV at the measurement current.

---

<a name="appendix-d"></a>
## APPENDIX D — AD8307 APPLICATION NOTES

### D-1: Power Supply Decoupling

The AD8307 requires clean 5V. Place 100nF + 10nF ceramic in parallel, directly at VPS pin 7, with minimum trace length to ground.

### D-2: Input Impedance and Matching

The AD8307 input (pins 1 and 8) has ~1.1kΩ || 1.4pF impedance. For 50Ω source matching, add 52.3Ω from INP (pin 8) to INM (pin 1), or 52.3Ω to GND from INM (single-ended).

### D-3: Output Filtering

The AD8307 VOUT (pin 4) has an internal 12.5kΩ source impedance. Add 1nF from VOUT to GND to filter to 8 kHz bandwidth (adequate for SWR display; peak detector behavior at voice-modulation speeds).

### D-4: Calibration

The AD8307 intercept and slope vary ±2 dB part-to-part. For absolute accuracy, apply a known power level (e.g., 0 dBm from a calibrated signal generator) and measure VOUT. The slope is stable (±0.5 mV/dB); only the intercept needs calibration.

```
cal_offset_dBm = P_known_dBm − (V_out_measured_mV / 25 − 84)
```

Store cal_offset in NVS and apply to all power calculations.

---

<a name="appendix-e"></a>
## APPENDIX E — BLE DATA PROTOCOL AND PYTHON CLIENT

### E-1: Data Frame Format

The ESP32 broadcasts data as a Nordic UART Service notification every 500ms:

```
BAND=20m\nFWD=95.30\nREF=1.20\nSWR=1.24\nRL=20.2\nPEAK=97.1\n
```

Fields:
| Field | Unit | Range |
|-------|------|-------|
| BAND | string | 160m, 80m, 40m, 30m, 20m, 17m, 15m, 12m, 10m, 6m, 2m, 1.25m, 70cm, 33cm, 23cm |
| FWD | Watts | 0–200W |
| REF | Watts | 0–200W |
| SWR | dimensionless | 1.00–99.9 |
| RL | dB | 0–60 |
| PEAK | Watts | 0–200W |

### E-2: Python BLE Client

```python
import asyncio
from bleak import BleakClient, BleakScanner

DEVICE_NAME = "RF-COUP-001"
NUS_TX_UUID = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
NUS_RX_UUID = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

def parse_frame(data_str):
    fields = {}
    for line in data_str.strip().split('\n'):
        if '=' in line:
            k, v = line.split('=', 1)
            fields[k] = v
    return fields

async def main():
    devices = await BleakScanner.discover(timeout=5)
    target = next((d for d in devices if d.name == DEVICE_NAME), None)
    if not target:
        print("RF-COUP-001 not found")
        return

    async with BleakClient(target.address) as client:
        print(f"Connected to {DEVICE_NAME}")

        def on_rx(char, data):
            text = data.decode('utf-8', errors='ignore')
            fields = parse_frame(text)
            if 'FWD' in fields:
                print(f"[{fields.get('BAND','?')}] "
                      f"FWD={fields['FWD']}W  "
                      f"REF={fields.get('REF','?')}W  "
                      f"SWR={fields.get('SWR','?')}  "
                      f"RL={fields.get('RL','?')}dB")

        await client.start_notify(NUS_TX_UUID, on_rx)

        # Set band to 20m
        await client.write_gatt_char(NUS_RX_UUID, b"BAND=20m\n")

        print("Logging... Ctrl-C to stop")
        await asyncio.sleep(300)   # Log for 5 minutes

asyncio.run(main())
```

---

*End of TM-COUP-001 Rev A*

*Prepared by: Field Engineering Reference*  
*Related manuals: TM-BMS-001 (battery), TM-PWR-001 (power supplies), TM-MAST-001 (antenna masts), TM-ANT-001 (antenna suite)*
