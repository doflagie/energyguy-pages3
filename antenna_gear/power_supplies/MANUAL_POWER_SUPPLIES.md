# TECHNICAL MANUAL — PORTABLE POWER SUPPLY SYSTEM

**Document Number:** TM-PWR-001  
**Revision:** A  
**Date:** 2026  
**Classification:** UNCLASSIFIED — AMATEUR RADIO TECHNICAL REFERENCE  
**Applicability:** Portable HF/VHF Field Station, QRP and Mobile Operation

---

## TABLE OF CONTENTS

1. [Introduction and Scope](#1-introduction-and-scope)
2. [Power Supply Theory](#2-power-supply-theory)
3. [Linear Regulated Power Supply](#3-linear-regulated-power-supply)
4. [Switching Mode Power Supply](#4-switching-mode-power-supply)
5. [LiFePO4 Battery Pack and BMS](#5-lifepo4-battery-pack-and-bms)
6. [Solar Charge Controller with MPPT](#6-solar-charge-controller-with-mppt)
7. [Metering, Protection, and Distribution](#7-metering-protection-and-distribution)
8. [RFI Suppression and Noise Filtering](#8-rfi-suppression-and-noise-filtering)
9. [Thermal Management](#9-thermal-management)
10. [Installation and Wiring](#10-installation-and-wiring)
11. [Test Procedures](#11-test-procedures)
12. [Fault Isolation and Troubleshooting](#12-fault-isolation-and-troubleshooting)
13. [Preventive Maintenance](#13-preventive-maintenance)

**Appendix A:** Complete Parts Lists  
**Appendix B:** Wiring Diagrams  
**Appendix C:** Calculation Reference  
**Appendix D:** File Index

---

## 1. INTRODUCTION AND SCOPE

### 1.1 Purpose

This manual covers design, construction, operation, testing, and maintenance of portable power supply systems for amateur radio field operation. Systems described include:

- **Linear regulated PSU** — 13.8V DC, 5A/10A/20A, low noise for sensitive receiver applications
- **Switching PSU (SMPS)** — 13.8V DC, 10A/20A/30A, high efficiency for battery-powered field use
- **LiFePO4 battery pack** — 12.8V nominal, 4S configuration, 20–100 Ah with BMS protection
- **Solar charge controller** — MPPT algorithm, ESP32 controlled, for field recharging
- **Metering and protection** — INA219/INA226 current sensing, CYD display, web interface

### 1.2 Design Philosophy

**Linear PSU:** Noise performance is paramount for receiver operation. A linear supply dissipates excess voltage across a series pass element; this limits efficiency but eliminates switching-frequency RF interference. Preferred when operating sensitive HF receivers within 10 meters of the power supply.

**SMPS:** Efficiency is paramount for portable battery operation. A 13.8V/20A station at 90% SMPS efficiency vs. 78% linear efficiency saves 29 watts — critical when drawing from a 50 Ah LiFePO4 pack. Proper output filtering brings SMPS noise below 10 mV p-p, acceptable for field SSB/CW operation.

**LiFePO4:** Safest lithium chemistry. Cannot enter thermal runaway. 2000+ cycle life. Flat discharge curve provides predictable runtime. Mandatory BMS for cell protection.

**Solar MPPT:** Maximum Power Point Tracking extracts 10–30% more energy from a solar panel than PWM control, particularly in cool weather or partial shading. Worthwhile for deployments exceeding one day.

### 1.3 Safety Precautions

> **WARNING:** AC mains voltage (120/240V AC) is present in linear PSU designs and in the SMPS input stage. Capacitors retain dangerous voltage for several minutes after power is removed. Always discharge bulk capacitors before servicing.

> **CAUTION:** LiFePO4 cells, although safer than other lithium chemistries, can still deliver hundreds of amperes into a short circuit. Never work on a battery pack without the BMS in circuit. Never short battery terminals.

> **CAUTION:** Solar panels produce voltage in daylight even when the load is disconnected. Cover the panel before working on the charge controller.

---

## 2. POWER SUPPLY THEORY

### 2.1 Linear vs. Switching — Fundamental Trade-off

**Linear supply:** A series pass transistor (or IC regulator) acts as a variable resistor. The control circuit adjusts transistor conductance to keep V_out constant as load or input varies. Power not delivered to the load is dissipated as heat in the pass element.

```
Efficiency: η = V_out / V_in (theoretical maximum)
Pass element dissipation: P_pass = (V_in - V_out) × I_out
```

For 13.8V output from 17V rectified supply at 10A: P_pass = 32W.  
This requires a substantial heatsink but produces no switching interference.

**Switching supply:** A power MOSFET switches the input voltage at high frequency (50–500 kHz). An LC filter averages the switched waveform to produce DC output. The duty cycle (on-time fraction) determines output voltage: V_out = D × V_in.

```
Efficiency: η = 88–93% (practical, synchronous buck)
Switching frequency harmonics appear on output unless filtered
```

The fundamental efficiency advantage: at 90% efficiency, only 15W is lost at 13.8V/10A vs. 32W in a linear supply — less than half the heat.

### 2.2 Buck Converter Operation

The synchronous buck converter steps voltage down:

```
High-side MOSFET ON:  Current flows through inductor to output; inductor stores energy.
High-side MOSFET OFF: Inductor's magnetic field collapses; freewheeling (low-side)
                       MOSFET or diode carries current through inductor to output.
```

The inductor averages the switching voltage to DC. The output capacitor suppresses ripple from the inductor current's sawtooth waveform.

**Key relationship:** `V_out = D × V_in` where D = duty cycle (0 to 1).  
For V_out = 13.8V from 24V: D = 13.8/24 = 0.575 (57.5% on-time).

**Inductor sizing:** L = (V_in - V_out) × D / (f_sw × ΔI_L)

With f_sw = 100 kHz and ΔI_L = 20% of I_out:  
L = (24 - 13.8) × 0.575 / (100,000 × 2A) = 29 µH → use 33 µH.

### 2.3 Regulation Feedback

Both linear and switching supplies use negative feedback to maintain constant output:

1. A resistor divider samples V_out.
2. An error amplifier compares the sample to a reference voltage.
3. The error signal adjusts the pass element (linear) or duty cycle (SMPS).
4. Output voltage stabilizes when error = 0.

**UC3843 SMPS feedback (13.8V output):**  
V_VFBK threshold = 2.5V (half of 5V V_ref).  
R_top = 12 kΩ, R_bot = 2.7 kΩ: V_out = 2.5 × (1 + 12/2.7) = 13.6V (adjust with 500Ω trim).

**LM338 linear feedback (13.8V output):**  
V_ref = 1.25V (between ADJ and OUT pins).  
R1 = 240 Ω, R2 = 2.4 kΩ: V_out = 1.25 × (1 + 2400/240) = 13.75V.

---

## 3. LINEAR REGULATED POWER SUPPLY

### 3.1 5A Version — LM338

The LM338 is a 5A adjustable regulator in a TO-3 or TO-220 package. It requires minimal support components and produces excellent noise performance.

**Circuit summary:**
- Transformer: 200 VA, 18V secondary
- Bridge rectifier: KBPC1510 (15A, 1000V)
- Filter capacitor: 22,000 µF / 50V
- Regulator: LM338 with R1=240Ω, R2=2.4kΩ
- Output filter: Π-type, 10 µH + 1000 µF

**Heatsink requirement:**  
P_pass = (V_in_max - V_out_min) × I_max = (24 - 13.8) × 5 = 51W.  
θ_heatsink ≤ (T_j_max - T_ambient - P × θ_jc) / P = (150 - 40 - 51) / 51 = 1.16 °C/W.  
Use 0.8 °C/W heatsink with thermal grease (Hammond HS151 or equivalent).

**Ripple rejection:**  
LM338 PSRR ≈ 65 dB at 120 Hz. Input ripple of 2V p-p → output ripple < 1 mV p-p.  
With output Π-filter: < 100 µV RMS broadband.

### 3.2 10A and 20A Versions — LM723 + Pass Transistors

The LM338 is limited to 5A. For 10A and 20A, the LM723 serves as the control/error amplifier driving external pass transistors.

**10A version:**
- LM723 → TIP35C (25A NPN TO-218) in Darlington configuration
- R_sense = 0.065 Ω for 10A current limit
- Emitter follower: V_out = V_base - V_be = (LM723 output) - 0.7V

**20A version:**
- LM723 → BD139 pre-driver → 3× 2N3055 in parallel
- Emitter resistors: 0.1 Ω each (prevents thermal runaway in parallel transistors)
- Emitter resistors force current sharing: ΔI = ΔV_be / R_emitter ≈ 60 mV / 0.1 Ω = 600 mA max imbalance
- 750 VA transformer required

**Why emitter resistors are mandatory:**  
NPN transistors have a positive temperature coefficient of current gain (hFE increases with temperature). Without emitter resistors, the transistor carrying the most current gets hotter, increasing its hFE, drawing more current — thermal runaway. The 0.1 Ω emitter resistors provide negative feedback: more current → more emitter voltage → less base-emitter voltage → less current.

### 3.3 Foldback Current Limiting

Simple current limiting holds current at I_limit during a short circuit, forcing the pass element to dissipate V_in × I_limit — potentially hundreds of watts.

**Foldback** reduces the current limit as output voltage drops:

```
Normal (V_out = 13.8V): I_limit = I_knee (design maximum)
Short circuit (V_out = 0V): I_limit = I_sc (fraction of I_knee)
```

Implementation using LM723 CL pins:
- R_fb1 = 3.3 kΩ from V_out to LM723 CL−
- R_fb2 = 820 Ω from LM723 CL− to GND
- As V_out falls, V_CL− falls, lowering the current limit

For I_knee = 10A, I_sc = 2A: pass element dissipates max 24 × 2 = 48W at dead short, vs. 240W without foldback.

**Recovery:** Remove the short circuit; V_out recovers automatically (foldback is not a latch).

---

## 4. SWITCHING MODE POWER SUPPLY

### 4.1 UC3843 Current-Mode PWM Controller

The UC3843 is an 8-pin current-mode PWM IC. It compares the inductor current (sensed via a small resistor) against a voltage error signal to set duty cycle on a cycle-by-cycle basis.

**Advantages of current-mode control:**
- Inherent peak current limiting (cycle-by-cycle)
- First-order output filter response (simpler compensation)
- Better load-step response than voltage-mode control

**Frequency setting:** R_T × C_T oscillator.  
For 100 kHz: R_T = 17.2 kΩ, C_T = 1 nF → f_sw = 1.72 / (R_T × C_T) = 100 kHz.

**Soft-start:** Connect 10 µF from COMP pin (1) to GND. On power-up, cap is uncharged → COMP low → duty cycle near zero → output ramps to set-point over ~10 ms, preventing inrush.

### 4.2 IR2110 Bootstrap Gate Driver

High-side MOSFETs require a gate drive voltage above V_drain (above the input supply). The IR2110 generates this from a bootstrap capacitor (100 nF, 50V) that charges to 15V via the internal supply during the low-side conduction period.

**Gate drive current:** 1.5A peak — essential. The MOSFET gate capacitance (1–10 nF for IRF3205) must charge/discharge in nanoseconds. Direct ESP32 GPIO drive (10 mA max) is completely inadequate.

**Dead time:** The IR2110 provides a short dead time between HO and LO transitions to prevent shoot-through (both MOSFETs conducting simultaneously, which shorts V_in to GND). If synchronous rectification is not used, the freewheeling diode body diode carries current during dead time.

### 4.3 20A and 30A Versions

**20A:** Parallel two IRF3205 MOSFETs for Q_H and Q_L. Use 22 Ω individual gate resistors (prevents oscillation between paralleled FETs due to parasitic inductance differences). Inductor becomes 15 µH; output capacitors doubled.

**30A (interleaved):** Two synchronous buck stages operating 180° out of phase. Ripple from each stage partially cancels at the output, reducing output filter requirements. Effective ripple frequency = 2 × f_sw. Requires two synchronized UC3843 controllers or a dedicated two-phase IC (LTC3722).

### 4.4 EMI Filtering

The SMPS generates conducted and radiated interference at the switching frequency and its harmonics. FCC Part 15B and ITU regulations (and good neighbor courtesy) require filtering.

**Input EMI filter:**
- Common-mode chokes (10 mH + 4.7 mH)
- X-capacitor (0.1 µF across L-N): suppresses differential mode
- Y-capacitors (2.2 nF L-to-chassis, N-to-chassis): suppresses common mode
- MOV (275V): transient protection

**Output filter:**
- Differential-mode LC filter: two stages, −51 dB at 100 kHz → reduces 50 mV ripple to < 2 mV
- Common-mode choke (10 mH, wound on same core): blocks RF on both supply leads
- Y-capacitors to chassis (4.7 nF): shunts residual CM noise

**NOTE:** The output CM choke and Y-caps are the most important elements for receiver noise. Install even when using a commercial SMPS — add them externally if the supply lacks them.

---

## 5. LIFEPO4 BATTERY PACK AND BMS

### 5.1 LiFePO4 Cell Chemistry

LiFePO4 (Lithium Iron Phosphate) is the preferred chemistry for field radio operation:

| Parameter | LiFePO4 | Li-Ion (NMC) | Lead-Acid |
|-----------|---------|--------------|-----------|
| Nominal cell voltage | 3.2V | 3.6V | 2.0V |
| Cycle life | 2000–5000 | 300–500 | 200–500 |
| Thermal runaway | No | Yes | No |
| Gravimetric energy density | 90–120 Wh/kg | 150–200 Wh/kg | 30–50 Wh/kg |
| Cost per Wh | Moderate | Moderate | Low |

**4S pack (4 cells in series):**
- Nominal: 4 × 3.2V = **12.8V**
- Full charge: 4 × 3.60V = **14.4V**
- Discharge cutoff: 4 × 2.50V = **10.0V**
- Optimal range (cycle-life preserving): 12.8V–13.4V (20–90% SOC)

### 5.2 SOC Estimation

The LiFePO4 discharge curve is exceptionally flat — from 20% to 80% SOC, pack voltage varies only 0.2–0.3V. This makes voltage-based SOC estimation unreliable during operation.

**Practical SOC methods:**
1. **OCV (Open Circuit Voltage):** Rest for ≥ 1 hour after charge or discharge; measure voltage with no load. Accurate but requires offline time.
2. **Coulomb counting:** Integrate current over time (Ah). Accurate if initial SOC is known; accumulates error. INA226 with 16-bit resolution supports this method.
3. **BMS SOC:** The JK BMS performs coulomb counting internally and reports SOC via Bluetooth and UART. Most practical for field use.

**SOC lookup table (4S, 20°C, after 1 hr rest):**

| SOC | Pack Voltage |
|-----|-------------|
| 100% | 14.2–14.4V (just after charge) |
| 90% | 13.4V |
| 80% | 13.2V |
| 60% | 13.1V |
| 40% | 13.0V |
| 20% | 12.8V |
| 10% | 12.5V |
| 0% | 10.0V (cutoff) |

### 5.3 BMS Functions and Selection

The BMS is non-optional. It protects against:

1. **Overvoltage (OV):** Disconnects charge if any cell exceeds 3.65V. Prevents electrolyte decomposition.
2. **Undervoltage (UV):** Disconnects load if any cell drops below 2.50V. Prevents irreversible capacity loss.
3. **Overcurrent Charge (OCC):** Limits charge rate. Set at 0.5C to 1C typically.
4. **Overcurrent Discharge (OCD):** Limits discharge. Set at 1C to 3C. Critical for short-circuit protection.
5. **Short Circuit (SC):** Ultra-fast cutoff (< 100 µs). Protects cells from catastrophic short.
6. **Temperature:** Inhibits charging below 0°C (lithium plating); shuts down above 60°C.
7. **Cell balancing:** Equalizes cell voltages during charging. Top-of-charge balancing is standard.

**Recommended BMS:**
- **Daly 4S 50A BMS** (~$15–25): Passive balancer. Common port (charge and discharge share P−). Suitable for 20–50 Ah packs where simplicity is preferred.
- **JK BMS 4S 100A** (~$30–50): Active inductor balancer (80–90% efficient). Separate charge/discharge ports. BLE app. UART to ESP32. Preferred for field use — Bluetooth monitoring eliminates need for wired status display.

### 5.4 Cell Compression

Prismatic LiFePO4 cells must be compressed at 10–15 psi during operation. This:
- Prevents cell swelling on charge cycles
- Maintains electrode contact area
- Increases cycle life by 20–30%

**Compression assembly:** Two 3mm aluminum plates + M8 threaded rods + nuts. Torque rod nuts to achieve 15 psi across cell face area.

For 105 × 207 mm cell face: Area = 217 cm² = 336 in². At 15 psi: total force = 5040 lbs. Two M8 rods per side carry 2520 lbs each — well within M8 tensile strength (≈ 4000 lbs for Grade 8.8).

### 5.5 JK BMS Integration with ESP32

```
JK BMS UART → ESP32 GPIO16 (RX)
Baud rate: 115200
Protocol: JK BMS request/response (documented at github.com/jblance/jkbms)
```

The ESP32 firmware reads cell voltages, pack voltage, current, SOC, temperatures, and fault flags. This data drives the CYD battery screen and web interface.

The JK BMS sends a "Cell Info" frame every 1–2 seconds. The ESP32 parser extracts:
- Cell voltages (2 bytes each, mV)
- Pack voltage (2 bytes, 10 mV/bit)
- Pack current (2 bytes signed, 10 mA/bit; positive = charging)
- SOC (1 byte, %)
- Temperature × 2 (2 bytes each, 0.1°C/bit, offset 2731 = 0°C)

---

## 6. SOLAR CHARGE CONTROLLER WITH MPPT

### 6.1 MPPT vs. PWM

**PWM (Pulse Width Modulation) charge control:**  
The panel voltage is pulled down to battery voltage. Panel operates at battery voltage (13.8V), not at its maximum power point (17.5V). Energy left in the panel above battery voltage is wasted.

**MPPT (Maximum Power Point Tracking):**  
A DC-DC buck converter steps the panel voltage down to charge voltage, while tracking the panel's maximum power point. The panel operates at its optimal voltage (Vmp), and the converter efficiently transfers that power to the battery.

**Efficiency gain of MPPT over PWM:**
- Summer (panel warm, Vmp close to battery): 5–10% gain
- Winter / cool conditions (Vmp higher): 20–30% gain
- Partial shading: 15–25% gain
- Average annual gain for typical ham installation: **12–20%**

For a 100W panel, 12% average gain = 12 additional watts continuously = significant on a multi-day deployment.

### 6.2 Perturb and Observe Algorithm

The P&O algorithm is the simplest effective MPPT implementation:

```
Every 100 ms:
  1. Measure V_panel and I_panel
  2. Compute P = V × I
  3. Compare P to P_previous
  4. If P increased: continue in same duty-cycle direction
     If P decreased: reverse duty-cycle direction
  5. Adjust D by ±0.005 (0.5% step)
  6. Clamp D to 0.05–0.95
  7. Write D to PWM; save P as P_previous
```

The algorithm oscillates ±0.5% around P_max in steady state. This residual oscillation wastes < 0.5% of P_max — acceptable for a field design.

**Why P&O over Incremental Conductance (IC)?**  
IC algorithm: finds P_max exactly (no oscillation) by computing dP/dV. Requires more accurate ADC and more complex firmware. For this application, P&O's simplicity wins; the 0.5% oscillation loss is negligible vs. other inefficiencies.

### 6.3 ESP32 MPPT Implementation

The ESP32 LEDC (LED Controller) peripheral generates the PWM signal:
```cpp
ledcSetup(0, 100000, 10);   // Channel 0, 100 kHz, 10-bit (0–1023)
ledcAttachPin(26, 0);
ledcWrite(0, duty_int);     // duty_int = D × 1023
```

Panel voltage is measured via resistor divider (57 kΩ / 3.3 kΩ) into ESP32 GPIO34 (ADC).  
Panel and battery current are measured via two INA219 sensors on the I2C bus.

**Important:** The ESP32 ADC is nonlinear near 0V and 3.3V. Constrain measurements to 10–90% of range (0.33V–2.97V = 6V–54V panel voltage) for best accuracy. The INA219 provides panel current with better accuracy than the ESP32 internal ADC.

### 6.4 MPPT Power Stage

The MPPT buck converter is identical to the SMPS converter but:
- Input voltage: 15–60V (solar panel, temperature-dependent Voc)
- UC3843 is replaced by ESP32 PWM + TC4420 gate driver
- No IR2110 required — the TC4420 provides 1.5A gate drive directly from single-ended PWM
- MOSFET: IRFZ44N (55V, 49A, 28 mΩ Rds_on) — adequate for 20A at 40V

**TC4420 gate driver:** The ESP32 GPIO output (3.3V logic, 10 mA max) cannot drive a MOSFET gate. The MOSFET gate capacitance (~3 nF for IRFZ44N) must charge to 10V in < 100 ns at 100 kHz. Required gate charge current: Q_g / t_rise ≈ 100 nC / 100 ns = 1A. The TC4420 provides this from its 3.3V input.

### 6.5 LiFePO4 Charge Stages

```
Stage 1 — BULK (CC, Constant Current):
  Charge at C/5 to C/2 of pack capacity.
  ESP32 PID controller: I_error = I_target - I_measured → adjust D.
  Continues until V_pack reaches 14.4V.

Stage 2 — ABSORPTION (CV, Constant Voltage):
  Hold V_pack = 14.4V (3.60V/cell).
  PID: V_error = V_target - V_measured → adjust D.
  I_charge decreases as cells approach full.
  Continues until I_charge < C/20.

Stage 3 — TERMINATION:
  I_charge drops below C/20 (2.5A for 50 Ah pack).
  Set D = 0. Charge complete.

Stage 4 — FLOAT:
  Maintain 13.5V (3.375V/cell).
  Tiny trickle current replaces self-discharge.
  Panel remains connected; controller runs at reduced duty cycle.
```

**Temperature compensation:**  
LiFePO4 charge voltage should decrease at high temperatures to prevent overvoltage stress, and increase slightly in cold weather.

```
V_charge(T) = V_charge_nominal - (T - 25°C) × 0.003 V/°C × N_cells
At T = 40°C: V_charge = 14.4 - (15 × 0.012) = 14.22V
At T = 10°C: V_charge = 14.4 + (15 × 0.012) = 14.58V
At T =  0°C: Charging prohibited (BMS will disconnect, but firmware should also stop)
```

---

## 7. METERING, PROTECTION, AND DISTRIBUTION

### 7.1 Current Sensing with INA219

The INA219 is a shunt-based I2C current/voltage sensor. It measures the voltage across a small series resistor (shunt) and divides by the resistance to give current. It simultaneously measures the bus voltage.

**Shunt selection:**  
The shunt must generate enough voltage for accurate measurement without excessive power loss.

```
R_shunt = V_shunt_max / I_max_expected
At 10A max: R = 320 mV / 10 A = 32 mΩ → use 25 mΩ (V_shunt_max = 250 mV at 10A)
Shunt power at 10A: P = I² × R = 100 × 0.025 = 2.5W → use 3W minimum
```

Use Kelvin (4-wire) construction for shunts above 1A: current terminals and sense terminals are separate connections. This eliminates contact resistance from the measurement path.

### 7.2 INA226 for High Accuracy

The INA226 provides 16-bit resolution (vs. INA219's 12-bit) and ±0.1% accuracy (vs. ±0.5%). It is preferred for the battery output where Wh energy integration requires precision, and for the display's voltage readout that operators use to gauge SOC.

**INA226 alert function:** Configure the ALERT pin to trigger on overcurrent, overvoltage, or undervoltage. Connect to an ESP32 interrupt pin. When the INA226 detects a fault, it fires the alert immediately — faster and more reliable than firmware polling.

### 7.3 Electronic Crowbar Overvoltage Protection

If the series pass transistor or PWM controller fails, V_out can rise to V_in (24V+), potentially destroying the connected radio.

The crowbar circuit:
1. LM431 adjustable reference monitors V_out.
2. When V_out exceeds the trip point (~15V), LM431 fires.
3. LM431 triggers an SCR gate.
4. SCR latches ON, short-circuits V_out.
5. The output fuse blows, disconnecting V_out.
6. Radio is protected; replace fuse and investigate root cause.

**LM431 trip voltage:** V_trip = 2.495 × (1 + R_top/R_bot)  
For 15.5V trip: R_top = 9.1 kΩ, R_bot = 1.8 kΩ.

**SCR selection:** MCR100-6 (0.8A, TO-92) — adequate for tripping the fuse.  
Fuse response: 10A slow-blow blows in < 100 ms at 3× rated current; SCR holds until fuse opens.

### 7.4 Anderson Powerpole Distribution

The Anderson Powerpole connector is the ARRL-standard DC connector for amateur radio. The Powerpole distribution center provides one high-current input and four independently fused outputs.

**Standard orientation:** When viewed from the mating face (cable side), RED contact is upper-right, BLACK contact is lower-left. Always use matched pairs. Verify polarity with a voltmeter before connecting equipment.

**Fuse sizing:** Each port fused at 1.25× the maximum continuous expected load.
- 100W HF transceiver: P = V × I → I = 200W / 12V = 16.7A → 20A fuse
- VHF/UHF transceiver at 50W: I ≈ 10A → 15A fuse
- Accessories (keyer, logging tablet): 5–10A → 10A or 15A fuse

**Wire size** (Anderson Powerpole standard): Use the correct wire AWG for the contact size.
- 15A contacts: 20–18 AWG
- 30A contacts: 16–12 AWG
- 45A contacts: 10 AWG

### 7.5 Power Sequencer

For station setups with multiple devices, power sequencing prevents audio pops and protects equipment from momentary voltage transients during connection.

**Recommended sequence (power ON):**
1. PSU / battery (no loads connected)
2. Metering controller (ESP32 boots, stabilizes in 500 ms)
3. Radio audio subsystem (+200 ms)
4. Radio RF subsystem (+500 ms)
5. Linear amplifier (+1000 ms)

**Recommended sequence (power OFF, reverse order):**
1. Amplifier
2. Radio RF
3. Radio audio
4. PSU

The ESP32 firmware implements this sequence via relay outputs. Normally-closed relay contacts provide failsafe: loss of ESP32 power opens all relays.

---

## 8. RFI SUPPRESSION AND NOISE FILTERING

### 8.1 Why PSU Noise Matters

A received signal at the antenna is typically −120 to −80 dBm (0.1–22 µV at 50 Ω). The noise floor of a good HF receiver is approximately −130 dBm. Any noise injected by the power supply into the receiver through the DC supply lead degrades sensitivity.

**RFI path analysis:**
1. SMPS generates switching noise at 100 kHz and harmonics
2. Noise couples to supply leads as differential mode (on the conductor) and common mode (on both conductors)
3. Supply leads act as antennas; radiated noise enters receiver directly or via antenna
4. Conducted noise on supply rail enters receiver through power supply input

**Goal:** Noise at receiver terminals < 0.1 µV (−127 dBm), which is below the noise floor.

### 8.2 Common-Mode vs. Differential-Mode Noise

**Differential mode (DM):** Noise appears between + and − conductors. Caused by switching ripple, inductor current ripple.  
**Filter:** Inductor in series with one conductor + capacitors from each conductor to GND.

**Common mode (CM):** Noise appears on both conductors in the same direction (both + and − relative to chassis/earth). Caused by capacitive coupling from switching node to chassis.  
**Filter:** Common-mode choke (wound on same toroid so DM currents cancel but CM currents add, and see the full inductance).

A complete supply output filter addresses both:
- DM stage: 10 µH series inductor + capacitor bank (100 µF + 10 µF + 0.1 µF)
- CM stage: 10 mH common-mode choke on both conductors
- Y-capacitors (4.7 nF, Y-rated safety type) from each conductor to chassis

### 8.3 Ferrite Beads on Output Leads

Even after the output filter, RF noise can couple back onto the supply leads via the connected loads. Install ferrite beads on the DC output cable, immediately behind the Powerpole connector:

- Fair-Rite 2643480002 (31 material): 195 Ω at 25 MHz per bead
- Stack 3 beads per conductor: 585 Ω at 25 MHz
- At 14 MHz (20M band): provides ~350 Ω impedance on noise path

**Field test:** Connect receiver to supply via Powerpole. Note S-meter reading with no antenna on a 40M or 20M frequency above 0.5 MHz from nearest occupied frequency. Switch between battery and SMPS — any S-meter increase indicates residual SMPS noise. Add CM choke until no change.

### 8.4 Snap-On Ferrite Cores (Retrofit Suppression)

For commercial power supplies that lack adequate output filtering, add external ferrite:
- Fair-Rite 0431177081 (31 material, split core): clips over existing cable
- Pass supply cable through core 3–4 times (multiple turns increase impedance)
- Place 6 inches from supply output

Multiple turns: impedance scales as N² (3 turns = 9× single turn).  
At 14 MHz, single pass ≈ 100 Ω; 3 turns = 900 Ω — effective noise suppression.

---

## 9. THERMAL MANAGEMENT

### 9.1 Pass Element Heatsinking

**Linear PSU** — all waste power dissipated in pass transistors:
```
P_pass = (V_in - V_out) × I_out
Worst case (light battery charging, V_in high): P = (28 - 13.8) × 20 = 284W
```

At 284W, the heatsink thermal resistance to ambient must be:
```
θ_heatsink ≤ (T_j_max - T_ambient - P × θ_jc) / P
For 2N3055 (θ_jc = 1.5°C/W), T_j_max = 150°C, T_ambient = 40°C:
θ_heatsink ≤ (150 - 40 - 284 × 1.5) / 284 = negative! → Must derate V_in or reduce I
```

**Lesson:** A linear supply should not be connected to a 28V source at 20A. Regulate V_in via a pre-regulator or restrict V_in. For field use, 24V bus → 13.8V output at 10A = 102W → feasible with large heatsink.

**SMPS** — much lower thermal dissipation:
```
P_loss = (1 - η) × P_out = 0.10 × (13.8 × 20) = 27.6W
MOSFET: P = I² × Rds = 400 × 0.008 = 3.2W each (with IRF3205)
Inductor: P = I² × DCR = 400 × 0.005 = 2W
Controller + driver: ~2W
Total: ~10W on heat-sensitive components → 60mm fan provides adequate cooling
```

### 9.2 Fan Control

The firmware implements proportional fan speed based on sensed temperature:
```
T < 35°C: Fan off
35°C ≤ T < 55°C: Fan speed = 30% + proportional to (T - 35°C) / 20°C
T ≥ 55°C: Fan 100%
```

Minimum duty cycle when fan is running: 30% (PWM = 77/255). Most fans stall below 20–25% duty cycle; 30% provides margin for starting under load.

**Fan selection:** 60mm × 25mm, 12V brushless, ball bearing (longer life than sleeve bearing in tilted orientation). Minimum 30 CFM at 0.1" WG static pressure.

### 9.3 LiFePO4 Battery Thermal Limits

| Temperature | Action |
|------------|--------|
| Below −20°C | No discharge — BMS disconnects load |
| −20°C to 0°C | Discharge at reduced rate (< 0.2C) |
| 0°C to 15°C | Charge prohibited — BMS inhibits. Discharge OK. |
| 15°C to 35°C | Normal operation — optimal range |
| 35°C to 45°C | Reduce charge rate; monitor closely |
| Above 45°C | BMS shuts down charging |
| Above 60°C | BMS disconnects all — emergency condition |

**Cold weather operation:** At 0°C, charging is prohibited. Warm the pack to ≥ 5°C before charging. Discharge is acceptable down to −20°C at reduced rate. A foam-insulated box retains battery heat from discharge self-warming; do not use sealed rigid enclosures that prevent venting.

---

## 10. INSTALLATION AND WIRING

### 10.1 Fusing Requirements

Every power source connection must be fused as close to the source as practical:

| Location | Fuse Type | Rating |
|---------|-----------|--------|
| Battery positive terminal | ANL bolt-down | 1.25× max load current |
| Solar controller input | DC-rated breaker | 1.5× max panel Isc |
| Distribution center feed | ANL | Sum of all output fuses |
| Each Powerpole output | Automotive blade | Per load calculation |
| AC input (linear PSU) | Slow-blow | 1.5× rated primary current |

**CRITICAL:** Use only DC-rated fuses and breakers for solar and battery circuits. AC breakers are not rated for DC interruption (DC arc does not have a natural zero-crossing). Incorrect fuse type may fail to interrupt a fault current.

### 10.2 Wire Sizing

From NEC Table 310.15(B)(16), plus field-experience margins:

| Current | Minimum AWG | Notes |
|---------|------------|-------|
| ≤ 15A | #14 AWG | QRP accessories |
| ≤ 20A | #12 AWG | Standard HF station (100W) |
| ≤ 30A | #10 AWG | High-power HF |
| ≤ 50A | #8 AWG | Battery bus, ANL fuse leads |
| ≤ 80A | #6 AWG | Battery positive terminal |
| > 80A | #4 AWG or larger | High-power amplifier |

**Voltage drop guidance:** Maximum 3% voltage drop from battery to radio at maximum current. V_drop = I × R_wire × length × 2 (round trip).

For 20A over 6 ft of #12 AWG: V_drop = 20 × 0.00193 × 12 = 0.46V (3.3%). Use #10 AWG.

### 10.3 Grounding and Bonding

**Single-point ground:** All equipment chassis connect to one point. From that point, a single heavy conductor runs to the station ground rod (and to battery negative).

**Supply output ground:** The Powerpole black (negative) conductor is the DC ground. Do NOT connect DC negative to AC neutral or to chassis at multiple points — ground loops cause noise.

**RF bonding:** DC ground and RF ground (station ground rod) must be bonded with flat copper strap (0.35 µH/m vs. 1.35 µH/m for round wire). Use 2" wide strap, as short as possible.

---

## 11. TEST PROCEDURES

### 11.1 Linear PSU Acceptance Test

**Required instruments:** DMM (4.5-digit preferred), oscilloscope (20 MHz BW), variable load (or resistor bank), thermometer.

| Test | Procedure | Acceptance Criteria |
|------|-----------|---------------------|
| Output voltage | Measure V_out at no load, 50% load, 100% load | 13.5–14.1V all conditions |
| Load regulation | ΔV_out from no-load to full load | < ±0.5% |
| Line regulation | Vary V_in ±10%; measure ΔV_out | < ±0.1% |
| Output ripple | Scope at full load, AC-coupled, 20 MHz BW | < 5 mV p-p |
| Current limit | Slowly increase load until current stops rising; measure I_limit | Within 10% of design value |
| Foldback | Verify current reduces as load resistance decreases below short | I_sc < 30% of I_knee |
| Thermal | Run at 80% full load for 30 minutes; measure heatsink temperature | < 70°C (heatsink surface) |
| Overvoltage | Temporarily increase feedback divider; verify crowbar trips | Crowbar fires before V_out > 15.5V |

### 11.2 SMPS Acceptance Test

| Test | Procedure | Acceptance Criteria |
|------|-----------|---------------------|
| Output voltage | As above | 13.5–14.1V |
| Switching frequency | Probe inductor node; measure frequency | 95–105 kHz |
| Output ripple | Scope full load, 20 MHz BW | < 10 mV p-p |
| Efficiency | η = (V_out × I_out) / (V_in × I_in) × 100% | > 85% at 50% load |
| EMI quick test | RX connected to supply, no antenna, S-meter check | No increase vs. battery |
| Soft-start | Monitor V_out at power-on; verify ramp | Reaches set-point in 10–50 ms; no overshoot > 200 mV |
| Current limit | Short output; measure I_sc | I_sc = 100–110% of rated |

### 11.3 LiFePO4 Battery Pack Test

| Test | Procedure | Acceptance Criteria |
|------|-----------|---------------------|
| Cell voltage balance | Measure each cell voltage before first charge | All cells within 0.05V |
| BMS protection | Attempt to charge below 0°C | BMS inhibits charging |
| OV protection | Apply 14.6V to pack; observe BMS | BMS disconnects at < 14.8V |
| Capacity | Discharge at C/5 rate from 100% to BMS cutoff; log Ah | > 90% of rated capacity |
| Balance quality | Charge full, then check cell voltages | All cells within 20 mV at top-of-charge |
| UART data | Connect ESP32; verify BMS data stream | All cell voltages, SOC, temps read correctly |

### 11.4 Solar Charge Controller Test

| Test | Procedure | Acceptance Criteria |
|------|-----------|---------------------|
| MPPT tracking | Use variable DC supply simulating panel; observe duty cycle changes | D tracks to maximize P_in |
| Charge stage transition | Verify CC → CV transition at 14.4V | V_bat held at 14.4V ± 0.1V in CV stage |
| Float voltage | Verify termination at I < C/20 and float at 13.5V | Float voltage 13.3–13.7V |
| Temperature compensation | Heat NTC; verify V_charge_target decreases | −12 mV/°C × 4 cells |
| Panel OC protection | Disconnect battery with panel connected; verify no damage | No over-voltage on circuit |

---

## 12. FAULT ISOLATION AND TROUBLESHOOTING

### 12.1 Troubleshooting Philosophy

Follow the electron. From the energy source to the load, identify where electron flow is blocked, misdirected, or impeded. Ask:

- Is voltage present at this point?
- Is the voltage the correct value?
- Is current flowing when it should be?
- Is current flowing when it should NOT be?

Divide the circuit into halves and test each half independently. Isolate: if the first half tests good, the fault is in the second half. Continue bisecting until the fault is localized to a single component.

### 12.2 Linear PSU Fault Table

| Symptom | Probable Cause | Isolation Procedure |
|---------|---------------|---------------------|
| No output voltage | Blown fuse; pass transistor open; LM723/LM338 failed | Check fuse; measure V_in; check regulator V_out pin |
| Output voltage too high | Feedback resistor open; R1 or R2 failed; pass transistor shorted B-C | Measure feedback divider; replace suspect resistor |
| Output voltage too low | Pass transistor low hFE; regulator overloaded; V_in too low | Reduce load; measure V_unregulated at filter caps |
| Output oscillating | Insufficient output capacitor; poor ground connection | Add capacitance; check all ground connections |
| Excessive ripple | Filter capacitor open or low capacitance; poor ESR | ESR-test filter capacitors; check PSRR of regulator |
| Overheating | Load too high; inadequate heatsink; V_in too high | Reduce load; check V_in; improve heatsink |
| Foldback doesn't recover | BMS disconnected load while foldback active | Remove overload; power-cycle supply |
| Crowbar fired | Regulator failed; V_out rose above trip point | Replace fuse; investigate why V_out went high |

### 12.3 SMPS Fault Table

| Symptom | Probable Cause | Isolation Procedure |
|---------|---------------|---------------------|
| No output, no switching | UC3843 not oscillating; V_CC too low; bootstrap not charging | Check UC3843 oscillator pin (pin 6) for triangle wave |
| Output 0V, switching present | High-side MOSFET failed open; inductor open; gate driver failed | Check Q_H gate waveform; check HO pin of IR2110 |
| Output = V_in | Low-side MOSFET failed short; Q_H and Q_L shorted | Both MOSFETs are suspected shorted; replace |
| Output low and variable | Duty cycle not regulation properly; current sense noisy | Check R_sense and sense circuit; check compensation network |
| High output ripple | Output capacitor ESR too high; capacitor failed | ESR-test output capacitors; check switching waveform at inductor |
| SMPS noise on received signal | Insufficient output filtering; CM choke missing or wrong core | Add CM choke; verify Y-capacitors installed |
| MOSFET getting hot | Excessive switching loss; gate drive too slow; Rds_on too high | Check gate waveform rise/fall time; verify gate driver supply voltage |

### 12.4 LiFePO4 Pack Fault Table

| Symptom | Probable Cause | Isolation Procedure |
|---------|---------------|---------------------|
| BMS cuts out under load | BMS OCD threshold too low; cell unbalanced (one cell UV) | Read cell voltages via BLE app during discharge |
| Charger does not start | BMS OCC; charging below 0°C; BMS UV latch | Warm pack; check charge voltage; reset BMS |
| Capacity below rated | Cell degradation; capacity test result | Full discharge test; replace if < 80% capacity |
| Cells not balancing | Balance wires not connected to BMS; balance tap resistance | Check balance tap connections; verify BMS balancing mode active |
| BMS overheating | Excessive continuous current; inadequate ventilation | Reduce load; improve BMS mounting (thermal contact to chassis) |

### 12.5 Solar Controller Fault Table

| Symptom | Probable Cause | Isolation Procedure |
|---------|---------------|---------------------|
| No charging with panel connected | Panel voltage too low for MPPT range; BMS blocking charge | Check panel voltage; check BMS temperature fault |
| Stuck in CC, never reaching CV | Battery significantly discharged; target too high | Measure battery voltage; check V_charge setpoint |
| Oscillating output | PID gains too high; MPPT step too large | Reduce D_step from 0.005 to 0.002; reduce PID gains |
| Panel power much lower than rated | Shading; panel temperature high; MPPT not converging | Measure I × V at terminals; compare to panel datasheet |
| MOSFET overheating | Continuous power too high; switching too fast for gate drive | Reduce panel current; verify TC4420 gate drive working |

---

## 13. PREVENTIVE MAINTENANCE

### 13.1 Annual Inspection

**Linear PSU:**
- Inspect electrolytic capacitors: bulging tops or leaking electrolyte indicate failure; replace.
- Check heatsink screws: retorque to 1.5 N·m (13 in-lb); heat cycling loosens fasteners.
- Verify thermal grease: inspect for drying/cracking; rework every 3–5 years.
- Test output voltage and ripple; compare to original acceptance test values.

**SMPS:**
- Inspect electrolytic capacitors (same as above).
- Clean fan blades and heatsink fins of dust (vacuum; do not blow dust into electronics).
- Check MOSFETs for thermal cycling cracks at solder joints (visual inspection, reflow if suspect).
- Test output ripple; increasing ripple indicates capacitor degradation.

**LiFePO4 Battery:**
- Capacity test: full discharge at C/5 rate; record Ah. Below 80% = cell aging.
- Check cell compression: verify threaded rod nuts are snug; re-torque if loose.
- Inspect bus bar connections for corrosion; clean with contact cleaner; re-torque M6 bolts to 5 N·m.
- Inspect BMS wiring for chafing or corrosion at balance taps.
- Test BMS protection: simulate low-cell condition (careful, use lab power supply to verify UV response).

### 13.2 Pre-Deployment Checklist

Before any field activation:

- [ ] Charge battery to 100%; verify SOC on BMS app
- [ ] Check all fuses intact and correct rating
- [ ] Verify Anderson Powerpole connections secure (tug test)
- [ ] Measure output voltage at distribution center (should read 12.8–14.4V depending on SOC)
- [ ] Confirm solar controller firmware operational if using solar
- [ ] Pack spare fuses: at least 2× each rating used
- [ ] Verify battery temperature within operating range (> 0°C if charging expected)

---

## APPENDIX A: COMPLETE PARTS LISTS

See individual schematic files for complete per-circuit parts lists:
- `sch_linear_psu.txt` — Linear PSU 5A/10A/20A
- `sch_switching_psu.txt` — SMPS 10A/20A/30A
- `sch_lifepo4_bms.txt` — LiFePO4 pack with BMS
- `sch_solar_charge_ctrl.txt` — MPPT solar controller
- `sch_metering_protection.txt` — Metering, protection, distribution

---

## APPENDIX B: WIRING DIAGRAMS

All wiring diagrams are contained in ASCII format in the schematic text files listed in Appendix A. Key diagrams:

- Complete MPPT system: `sch_solar_charge_ctrl.txt`, lines 153–162
- Buck converter power stage: `sch_switching_psu.txt`, lines 117–128
- LM723 + TIP35C 10A regulator: `sch_linear_psu.txt`, lines 162–172
- Daly BMS wiring: `sch_lifepo4_bms.txt`, lines 84–97
- Anderson Powerpole distribution: `sch_metering_protection.txt`, lines 200–215
- INA219 sensor wiring: `sch_metering_protection.txt`, lines 51–68

---

## APPENDIX C: CALCULATION REFERENCE

### Filter Capacitor
```
C = I_out / (2 × f_line × ΔV_ripple)
```

### Buck Inductor
```
L = (V_in - V_out) × D / (f_sw × ΔI_L)  where  D = V_out / V_in
```

### LM338 Output Voltage
```
V_out = 1.25 × (1 + R2/R1)
```

### LM723 Output Voltage
```
V_out = V_ref × (R_top + R_bot) / R_bot  where V_ref = 7.15V (LM723 internal)
```

### Heatsink Thermal Resistance
```
θ_heatsink = (T_j_max - T_ambient - P × θ_jc) / P
```

### Wire Voltage Drop
```
V_drop = I × R_wire_per_foot × length_one_way × 2
```

### MPPT Efficiency Gain Estimate
```
E_gain = (V_mpp / V_bat - 1) × η_converter × 100%
(approximate; actual gain varies with conditions)
```

### LiFePO4 Temperature Compensation
```
V_charge(T) = V_nominal - (T - 25) × 0.003 × N_cells
```

### NTC Temperature
```
T(K) = B / (B/T_0 + ln(R_ntc/R_0))    where T_0 = 298.15K, B = 3950 (typical)
```

---

## APPENDIX D: FILE INDEX

```
/ant_gear/power_supplies/
├── schematics/
│   ├── sch_linear_psu.txt           Linear PSU (LM338/LM723)
│   ├── sch_switching_psu.txt        SMPS (UC3843/IR2110/IRF3205)
│   ├── sch_lifepo4_bms.txt          LiFePO4 4S pack + BMS
│   ├── sch_solar_charge_ctrl.txt    MPPT solar controller (ESP32)
│   └── sch_metering_protection.txt  INA219/226, crowbar, Powerpole dist.
├── firmware/
│   └── psu_monitor_esp32.ino        CYD/ESP32 monitor, BMS UART, web UI
├── openscad/
│   ├── enclosure_psu.scad           PSU bodies, battery box, dist. panel
│   └── mounting_brackets_psu.scad   Rack ears, mast clamp, DIN clip
└── MANUAL_POWER_SUPPLIES.md        This document (TM-PWR-001 Rev A)
```

---

*TM-PWR-001 Rev A — End of Document*  
*Ham Radio Field Station Power Systems — Portable Design Reference*
