# TECHNICAL MANUAL — PORTABLE BATTERY MANAGEMENT SYSTEM

**Document Number:** TM-BMS-001  
**Revision:** A  
**Date:** 2026  
**Classification:** UNCLASSIFIED — AMATEUR RADIO TECHNICAL REFERENCE  
**Applicability:** Field Station Battery Systems, LiFePO4 and Li-Ion Packs

---

## TABLE OF CONTENTS

1. [Introduction and Scope](#1-introduction-and-scope)
2. [Battery Chemistry Reference](#2-battery-chemistry-reference)
3. [BMS Architecture and Protection Functions](#3-bms-architecture-and-protection-functions)
4. [BQ76920 Primary Protection IC](#4-bq76920-primary-protection-ic)
5. [Discrete Backup Protection Circuits](#5-discrete-backup-protection-circuits)
6. [Cell Balancing Theory and Implementation](#6-cell-balancing-theory-and-implementation)
7. [SOC Estimation: Coulomb Counting and Kalman Filter](#7-soc-estimation-coulomb-counting-and-kalman-filter)
8. [Low-Voltage Disconnect](#8-low-voltage-disconnect)
9. [MPPT Solar Charge Integration](#9-mppt-solar-charge-integration)
10. [ESP32 BLE Monitor System](#10-esp32-ble-monitor-system)
11. [Pack Assembly and Cell Wiring](#11-pack-assembly-and-cell-wiring)
12. [Installation and Commissioning](#12-installation-and-commissioning)
13. [Test Procedures](#13-test-procedures)
14. [Fault Isolation and Troubleshooting](#14-fault-isolation-and-troubleshooting)
15. [Preventive Maintenance](#15-preventive-maintenance)

**Appendix A:** Complete Parts Lists  
**Appendix B:** BQ76920 Register Reference  
**Appendix C:** Kalman Filter Mathematics  
**Appendix D:** BLE GATT Service Definition  
**Appendix E:** File Index

---

## 1. INTRODUCTION AND SCOPE

### 1.1 Purpose

This manual covers design, construction, commissioning, and maintenance of battery management systems for portable amateur radio field operation. Systems described include:

- **LiFePO4 4S BMS** using Texas Instruments BQ76920 — the preferred chemistry for field radio due to inherent safety and cycle life
- **Li-Ion 3S/4S BMS** using BQ76930 or Seiko S-8254 — higher energy density for weight-critical deployments
- **Discrete backup protection** — hardware-level overvoltage/undervoltage failsafe independent of all ICs
- **Active and passive cell balancing** — equalizing cell voltages to maximize pack capacity
- **Extended Kalman Filter SOC** — accurate state-of-charge estimation combining coulomb counting and open-circuit voltage correlation
- **Low-Voltage Disconnect (LVD)** — protecting cells from deep discharge during unattended operation
- **Solar MPPT integration** — field recharging with temperature-compensated charge profiles
- **ESP32/CYD monitor** — Bluetooth LE telemetry, real-time display, web interface

### 1.2 System Layered Protection Philosophy

The BMS uses a four-layer protection architecture. Failures cascade through layers; no single point of failure causes pack damage.

```
LAYER 1: BMS IC (BQ76920/BQ76930)
  Hardware OV/UV/OC/SC per-cell; coulomb counter; cell balancing
  Response time: 70 µs (SC) to 4 s (UV)
  Controllable via I2C from ESP32

LAYER 2: Discrete Comparator Backup
  TL431 + SCR latch (OV); LM393 (UV, OC); independent power
  Response time: < 100 ms
  No dependency on BMS IC or firmware

LAYER 3: ESP32 Firmware
  Temperature management; charge control; LVD; MPPT interface
  Response time: 250 ms – 2 s (poll rate)
  Implements charging algorithm, balancing, efficiency tracking

LAYER 4: Hardware Fuses and PTC
  ANL fuse (main), PTC resettable (balance wires)
  Non-resettable; last resort
```

### 1.3 Safety Precautions

> **WARNING:** Lithium cells at any charge level can deliver thousands of amperes into a short circuit. The resulting arc is instantaneous and can cause fires, severe burns, or explosion of the cell. NEVER short battery terminals. ALWAYS install the BMS before making any cell connections.

> **WARNING:** LiFePO4 is significantly safer than NMC/LCO chemistry, but overcharged or damaged LiFePO4 cells can vent toxic gases (HF, PF5, CO). Work in ventilated spaces. Do not seal a damaged cell in an airtight enclosure.

> **CAUTION:** Charging below 0°C causes irreversible lithium plating on the anode. A plated cell looks fully charged but has reduced capacity and a raised short-circuit risk. Never charge any lithium cell below 0°C.

> **CAUTION:** Cells must not be over-discharged (below 2.5V for LiFePO4, below 3.0V for Li-Ion). Over-discharge causes copper dissolution into the electrolyte, permanently reducing capacity and increasing internal resistance.

---

## 2. BATTERY CHEMISTRY REFERENCE

### 2.1 LiFePO4 (Lithium Iron Phosphate)

LiFePO4 is the preferred chemistry for field ham radio operation. The olivine crystal structure of the iron phosphate cathode does not release oxygen under stress, preventing thermal runaway — the dangerous exothermic reaction that causes Li-Ion fires.

**Cell Parameters:**

| Parameter | Value | Notes |
|-----------|-------|-------|
| Nominal voltage | 3.20 V | Used for capacity calculations |
| Full charge (OCV) | 3.55–3.65 V | Measure after 1 hr rest |
| Charge cutoff | 3.60 V | BMS OV trip; charger setpoint |
| Discharge cutoff | 2.50 V | BMS UV trip; never go below |
| Self-discharge | < 3%/month | Excellent shelf life |
| Cycle life | 2000–5000 | At 80% depth of discharge |
| Operating temperature (charge) | 0°C to +45°C | Charging below 0°C = plating |
| Operating temperature (discharge) | −20°C to +60°C | Reduced capacity at extremes |

**4S Pack (4 cells in series):**
- Nominal: **12.8V**
- Full charge: **14.4V** (3.6V/cell × 4)
- Discharge cutoff: **10.0V** (2.5V/cell × 4)

**The LiFePO4 Discharge Curve Problem:**  
The voltage changes very little between 20% and 80% SOC (approximately 13.0V to 13.2V for a 4S pack). This flat curve makes voltage-based SOC estimation unreliable during operation. The Kalman filter SOC estimation system (Chapter 7) addresses this limitation.

### 2.2 Li-Ion NMC (Nickel Manganese Cobalt)

NMC offers higher energy density (~150–200 Wh/kg vs. ~100–120 Wh/kg for LiFePO4) at the cost of reduced safety margin. The layered oxide structure of the NMC cathode releases oxygen when overheated or overcharged, enabling thermal runaway.

**Cell Parameters:**

| Parameter | Value | Notes |
|-----------|-------|-------|
| Nominal voltage | 3.60 V | |
| Full charge voltage | 4.20 V | NEVER exceed; triggers thermal runaway |
| Conservative charge | 4.10 V | Extends cycle life significantly |
| Discharge cutoff | 3.00 V | Capacity degradation accelerates below 2.8V |
| Cycle life | 300–500 (at 4.2V) | 1500+ at 4.1V/cell |

**Pack Configurations:**
- 3S: 10.8V nominal, 12.6V full, 9.0V cutoff — for QRP gear
- 4S: 14.4V nominal, 16.8V full, 12.0V cutoff — for standard 12V radios

**Important:** A 4S Li-Ion pack at 16.8V full charge exceeds the 13.8V nominal radio supply. Use a voltage regulator, or limit charge voltage to 4.1V/cell (16.4V), which is still above 13.8V but within most radio input tolerances (verify equipment specifications).

### 2.3 Cycle Life and Degradation

Cycle life depends primarily on:

1. **Charge voltage:** Lower V_max = exponentially longer life. LiFePO4 at 3.45V/cell (80% capacity) lasts 10,000+ cycles; at 3.65V/cell it degrades faster.
2. **Depth of discharge:** Shallower DoD extends cycle life. Cycling 20%–80% SOC (60% DoD) vs. 0%–100% (100% DoD) roughly triples cycle count.
3. **Temperature:** Every 10°C above 25°C approximately halves cycle life (Arrhenius aging).
4. **Charge rate:** Higher C-rate during charging generates more heat and SEI layer growth. Charge at ≤ 0.5C for longest life.

**Field use recommendation:**  
Keep LiFePO4 packs between 20%–90% SOC for day-to-day operation. Only charge to 100% when maximum runtime is needed for a specific activation. Store at 50% SOC if not used for > 2 weeks.

---

## 3. BMS ARCHITECTURE AND PROTECTION FUNCTIONS

### 3.1 Required Protection Functions

A BMS must protect against six fault conditions:

| Fault | Abbreviation | Effect if unprotected | Response |
|-------|-------------|----------------------|----------|
| Cell overvoltage | OV | Electrolyte decomposition, gas generation, cell failure | Open CHG FET |
| Cell undervoltage | UV | Copper dissolution, permanent capacity loss | Open DSG FET |
| Overcurrent discharge | OCD | Heating, separator damage, possible fire | Open DSG FET |
| Short circuit discharge | SCD | Catastrophic current spike, cell explosion | Open DSG FET (< 100 µs) |
| Over-temperature | OT | Accelerated aging, thermal runaway risk | Open CHG+DSG FET |
| Cold charging | LT (low temp) | Lithium plating, reduced capacity, safety risk | Open CHG FET |

### 3.2 FET Topology

The BMS switches the **negative** terminal of the pack (B−) through two MOSFETs in series:

```
Pack B− ──── DSG FET ──── CHG FET ──── P− (output)

DSG FET: controls discharge path. Body diode faces toward CHG FET.
CHG FET: controls charge path. Body diode faces toward B−.

Both ON:  Normal operation (charge and discharge)
CHG OFF:  Discharge only; body diode of CHG FET blocks reverse (charge) current
DSG OFF:  Charge only; body diode of DSG FET blocks forward (discharge) current
Both OFF: Pack isolated
```

**Why the negative path?** The gate drive for an N-channel MOSFET in the negative path is referenced to pack negative, which is also the circuit ground. This avoids the need for a floating bootstrap supply. P-channel MOSFETs in the positive path are an alternative but have higher Rds_on for equivalent die size.

### 3.3 Common-Port vs. Separate-Port BMS

**Common Port:** Single output connector (P−). Charge and discharge share the same terminal. The BMS determines mode from current direction.
- Simpler wiring; one output connector
- Limitation: cannot charge and discharge simultaneously through separate BMS ports

**Separate Port:** C− for charging (charger connects here); P− for discharge (load connects here). CHG and DSG FETs operate independently.
- More complex wiring
- Allows simultaneous solar charging and radio operation without current direction ambiguity
- Preferred for field station use

---

## 4. BQ76920 PRIMARY PROTECTION IC

### 4.1 Overview

The Texas Instruments BQ76920 is a purpose-built 3–5 cell monitor and protector IC. It provides all required protection functions in a single TSSOP-24 package.

**Key capabilities:**
- 14-bit ADC, 1.22 mV voltage resolution, ±1% accuracy
- Internal coulomb counter (16-bit, 250 µA LSB)
- Per-cell passive balance FET outputs (external resistors)
- I2C interface for host communication (ESP32)
- ALERT interrupt output (open-drain, fires on any fault)
- Factory-calibrated OTP registers (ADCGAIN, ADCOFFSET — read at startup)

### 4.2 Register Configuration for LiFePO4 4S

The BQ76920 thresholds are set via registers. Key registers:

**OV_TRIP (0x09):** Sets overvoltage trip voltage.  
Formula: V_OV = (OV_TRIP + 1) × 50.6 mV + 2.0 V  
For 3.65V: OV_TRIP = (3.65 − 2.0) / 0.0506 − 1 = 31.6 → write 0x20 (gives 3.663V, slightly above 3.65V; conservative)

**UV_TRIP (0x0A):** Sets undervoltage trip voltage.  
Formula: V_UV = (UV_TRIP + 1) × 50.6 mV + 1.0 V  
For 2.80V: UV_TRIP = (2.80 − 1.0) / 0.0506 − 1 = 34.6 → write 0x22 (gives 2.834V)

**PROTECT1 (0x06):** Short circuit protection.  
SCD_THRESH = 0b011 → 100 mV on sense resistor  
With 2 mΩ shunt: I_SCD = 100 mV / 0.002 Ω = 50A  
SCD_DELAY = 0b00 → 70 µs

**PROTECT2 (0x07):** Overcurrent protection.  
OCD_THRESH = 0b011 → 50 mV on sense resistor  
With 2 mΩ shunt: I_OCD = 50 mV / 0.002 Ω = 25A  
OCD_DELAY = 0b01 → 20 ms

### 4.3 Startup Sequence

The ESP32 must perform the following sequence on power-up:

1. Read ADCGAIN1 (0x50) and ADCGAIN2 (0x59) → compute g_adc_gain in µV/LSB
2. Read ADCOFFSET (0x51) → store as g_adc_offset in mV
3. Write SYS_CTRL1 → enable ADC and select thermistor pin
4. Write SYS_CTRL2 → enable CHG and DSG FETs; enable coulomb counter
5. Write OV_TRIP, UV_TRIP, PROTECT1, PROTECT2, PROTECT3 → set thresholds
6. Write SYS_STAT (0x00) → clear any startup fault flags (write 0xFF)
7. Begin reading cell voltages and CC register

**If ADCGAIN registers are not read and applied**, the cell voltage calculations will be inaccurate. The factory calibration typically places gain at 365–380 µV/LSB; not reading the registers and assuming 365 µV/LSB can introduce up to 40 mV per-cell error.

### 4.4 Cell Voltage Calculation

```
V_cell(n) = raw_adc × (ADCGAIN / 1,000,000) + ADCOFFSET / 1000

Where:
  raw_adc = 14-bit ADC reading from BQ_VC(n)_HI + BQ_VC(n)_LO
  ADCGAIN = calibration value in µV/LSB (read from OTP, typically 365–380)
  ADCOFFSET = calibration offset in mV (read from OTP, signed byte)
```

### 4.5 Coulomb Counter

The BQ76920 integrates current continuously. The CC register (0x32/0x33) is a signed 16-bit value that increments/decrements based on current direction.

```
I_actual = CC_register × (ADCGAIN / 4) / (R_sense × 1,000,000)
```

The CC register saturates at ±32767 and does not roll over automatically — firmware must read and reset it periodically. Recommended: read every 250 ms; accumulate in a software float accumulator. Reset CC by clearing the CC_EN bit and re-enabling.

---

## 5. DISCRETE BACKUP PROTECTION CIRCUITS

### 5.1 Overvoltage Backup (TL431 + SCR Latch)

The TL431 is an adjustable shunt reference with a built-in comparator. When the REF pin voltage exceeds 2.495V (internal reference), the device conducts. This is used to fire an SCR (silicon controlled rectifier) that latches the charge FET gate low.

**Operating principle:**
1. V_cell rises above trip threshold → TL431 REF pin exceeds 2.495V
2. TL431 cathode current flows → drives NPN transistor base
3. NPN collector drives SCR gate → SCR fires and latches
4. SCR holds CHG FET gate at battery negative → FET stays OFF
5. Once SCR fires, it remains conducting until anode current falls below holding current (~5 mA)
6. Recovery: remove charger (interrupts anode current) → SCR unlatches

**Trip voltage setting:**  
V_trip = 2.495 × (R_A + R_B) / R_B  
For V_trip = 3.65V (LiFePO4): R_A / R_B = (3.65/2.495) − 1 = 0.463  
Using R_B = 220 kΩ: R_A = 102 kΩ → use 100 kΩ (gives 3.630V, slightly conservative)

**Why use a latching SCR?** A simple comparator with relay would reset automatically when V_cell drops slightly below the trip threshold. This could allow repeated charge/discharge cycling around the overvoltage threshold, damaging cells. The SCR latch requires deliberate operator intervention (remove charger) to reset.

### 5.2 Undervoltage Backup (LM393)

The LM393 dual comparator monitors pack voltage and opens the discharge FET when voltage falls below the UV threshold. Unlike the OV circuit, UV recovery is automatic — when a charger is connected and voltage rises, the LM393 output changes state and allows the FET to conduct.

**Hysteresis is essential.** Without hysteresis, the comparator oscillates rapidly around the threshold as the FET switching changes the load current, which changes the battery terminal voltage. A 50–100 mV hysteresis band prevents this:
- Disconnect at 11.0V (4S LiFePO4 UV threshold)
- Reconnect at 11.1V (hysteresis restores 100 mV above disconnect)

### 5.3 Short Circuit Detection (Fast Comparator Path)

The short circuit path uses the same LM393 but with no RC delay on the output. Response time is limited by comparator propagation delay (~1 µs) and MOSFET turn-off time (~100 ns). Total SCD response: < 2 µs.

At 50A through 2 mΩ shunt: V_sense = 100 mV. At 100A (dead short): V_sense = 200 mV. Set SCD threshold to 150 mV (75A) to provide margin above maximum normal peak current.

### 5.4 Backup Circuit Independence

The backup circuits must operate independently of:
- BMS IC power supply (BQ76920 derives power from the top cell)
- ESP32 (may be unresponsive during firmware crash)
- I2C communication (bus failure should not disable protection)

**Power supply for backup comparators:** Run LM393 and TL431 circuits from a separate voltage reference derived from a middle cell tap (e.g., VC2, approximately 6.4V for 2S reference). Use a 5V zener regulator. This supply remains valid even if the BQ76920 REG pin supply fails.

---

## 6. CELL BALANCING THEORY AND IMPLEMENTATION

### 6.1 Why Cells Go Out of Balance

All cells in a series string are manufactured with slight differences in:
- **Capacity (Ah):** ±3–5% variation even within the same bin
- **Internal resistance (mΩ):** Higher-resistance cells heat more, age faster
- **Self-discharge rate:** Varies cell to cell; some discharge slightly faster

Over hundreds of cycles, these small differences accumulate. Without balancing, the cell with lowest capacity reaches UV cutoff first during discharge (limiting pack capacity), and the cell with highest capacity reaches OV first during charging (limiting charge input). The effective capacity is constrained by the weakest cell in both directions.

### 6.2 Top-of-Charge Balancing Algorithm

The firmware implements top-balance: cells are equalized only when the pack is near full charge (SOC > 85%). This strategy:
- Ensures all cells reach 100% SOC together
- Uses the absorption phase (when charge current is tapering) for balancing
- Avoids unnecessary resistor heating during normal discharge cycles

```
Balance decision at each 2-second tick:
  if (SOC < 85%) → disable all balance FETs (no balancing below 85%)
  if (cell temperature > 45°C) → disable all balance FETs (thermal protection)
  find V_min = minimum cell voltage in pack
  for each cell:
    if (V_cell > V_min + 10 mV) → enable balance FET for this cell
    else → disable balance FET
  when all cells within 5 mV → disable all balance FETs (balanced)
```

### 6.3 Passive Balance: Resistor Selection

The balance current flows through R_BAL when the BQ76920 balance FET for that cell is enabled:

```
I_bal = V_cell / R_BAL (approximately; FET Rds_on is small)
```

**Trade-off:**
- Low R_BAL (e.g., 10 Ω): 360 mA at 3.6V; fast balancing (hours); 1.3W dissipation
- High R_BAL (e.g., 47 Ω): 76 mA at 3.6V; slow balancing (days); 0.3W dissipation

For field use where overnight charging is available: 10 Ω is recommended. The 1.3W per resistor is manageable with a ventilated bracket; balance completes in 1–5 hours for typical imbalances encountered in field operations.

**Balance current flows through the CELL being balanced**, not across the pack. The bottom terminal of the balance resistor connects to the cell's bottom junction (the previous cell's positive terminal). Energy is wasted as heat in the balance resistor — not transferred to lower-SOC cells. This is the fundamental limitation of passive balancing.

### 6.4 Active Balancing (JK BMS Module)

Active balancing transfers energy from high-SOC cells to low-SOC cells using an inductor-based converter. The JK BMS active balancer operates at approximately 88% efficiency.

For a 50 Ah pack with 100 mV imbalance:
- ΔAh = 100 mV × 50 Ah / 3.2 V = 1.5 Ah
- At 1A balance current (JK BMS): t = 1.5 Ah / 1A = 1.5 hours
- Energy saved vs. passive: 1.5 Ah × 3.2 V × (1 − 0.88) = 0.58 Wh

The 0.58 Wh savings is small, but over thousands of cycles this accumulates, and the cells are maintained in better condition by avoiding the heat of passive dissipation.

**Active balancing is most beneficial when:**
- Pack is > 50 Ah (imbalance represents significant capacity loss)
- Cells are used at high C-rates generating larger voltage divergence
- Pack temperature is high (passive heat is unwanted)

---

## 7. SOC ESTIMATION: COULOMB COUNTING AND KALMAN FILTER

### 7.1 Why SOC is Difficult

The State of Charge of a battery is not directly measurable. The two observable quantities — terminal voltage and current — provide indirect information:

- **Voltage:** Reliable indicator only at rest (OCV = Open Circuit Voltage). Under load, the terminal voltage drops due to IR losses and polarization. A LiFePO4 cell under 10A load may read 0.3V lower than its OCV.
- **Current integration (coulomb counting):** Accurate over short time periods but accumulates error from measurement offset and sensor noise over hours. Also requires knowing the initial SOC.

No single method is perfect. The EKF combines both.

### 7.2 Coulomb Counting

Coulomb counting integrates current over time to track accumulated charge:

```
SOC(t) = SOC(t₀) + [∫ η_c × I(t) dt] / Q_nominal × 100%

Where:
  η_c  = Coulombic efficiency (0.97 for LiFePO4 during charging; 1.0 during discharge)
  I(t) = net current (positive = charging, negative = discharging)
  Q    = rated capacity in Ah
```

**Coulombic efficiency:** Not all charge input emerges as discharge output. The difference (3% for LiFePO4) is lost to side reactions (SEI layer growth, electrolyte decomposition). If η_c is not applied, the SOC estimate will drift upward over many cycles.

**Accuracy limitation:** A current sensor with 10 mA offset error at 1A average current introduces 1% SOC error per hour. Over a 10-hour activation, this accumulates to 10% error — significant. INA226 at 16-bit resolution has < 1 mA offset typical, limiting accumulated error to < 0.5% per hour.

### 7.3 Extended Kalman Filter

The EKF is a recursive estimator that optimally combines two sources of information:

1. **Prediction:** What we expect SOC to be based on the dynamic model (coulomb counting)
2. **Measurement:** What we can observe (OCV mapped to SOC via the OCV-SOC curve)

The "Extended" in EKF refers to the linearization of the nonlinear OCV-SOC relationship at each time step.

**EKF State Equations:**

```
State:        x = SOC (fraction, 0.0 to 1.0)

Prediction step (each time increment Δt):
  x̂(k|k-1) = x̂(k-1|k-1) + η_c × I_net × Δt / Q_nom
  P(k|k-1)  = P(k-1|k-1) + Q_noise

Update step (only when battery is at rest and OCV is valid):
  H         = dV_OCV/dSOC  (derivative of OCV curve at current SOC estimate)
  y         = SOC_from_OCV  (OCV measured → mapped to SOC via lookup table)
  S         = H² × P(k|k-1) + R_noise  (innovation covariance)
  K         = P(k|k-1) × H / S         (Kalman gain)
  x̂(k|k)   = x̂(k|k-1) + K × (y - x̂(k|k-1))
  P(k|k)    = (1 - K × H) × P(k|k-1)

Notation:
  Q_noise = process noise covariance (uncertainty in current measurement; ~1×10⁻⁵)
  R_noise = measurement noise covariance (uncertainty in OCV-SOC mapping; ~0.01)
  K       = Kalman gain (how much to trust measurement vs. prediction)
```

**OCV Rest Requirement:**  
The OCV measurement is only valid when current is < 100 mA and the pack has rested for at least 60 seconds. The EKF firmware watches for this condition and gates the update step accordingly.

**Practical Effect:**  
After a day of field use (charge → operate → charge), coulomb counting may have drifted ±5% SOC. When the operator stops for lunch and the pack rests, the EKF corrects the estimate using OCV. The display SOC snaps to a more accurate value. This self-correction is the primary advantage of EKF over pure coulomb counting.

### 7.4 OCV-SOC Tables

The OCV-SOC relationship is chemistry-specific and must be measured or obtained from cell manufacturer datasheets. The tables in the firmware were derived from published LiFePO4 and NMC OCV data at 20°C. At other temperatures:

- Warm cells (> 30°C): OCV shifts slightly higher for a given SOC
- Cold cells (< 10°C): OCV shifts lower; the flat LiFePO4 region becomes less flat
- Temperature correction: add 2 mV/°C to OCV measurement before table lookup (approximate)

For best accuracy: calibrate OCV-SOC table against your specific cell type by doing controlled discharge measurements at known SOC.

---

## 8. LOW-VOLTAGE DISCONNECT

### 8.1 Purpose and Behavior

The LVD circuit protects battery cells from deep discharge during unattended operation — overnight APRS, weather stations, remote sensors.

**Design parameters (4S LiFePO4):**
- **Disconnect voltage:** 11.5V pack (2.875V/cell — well above 2.5V UV cutoff but providing warning margin)
- **Reconnect voltage:** 12.5V pack (requires charger to restore voltage before load reconnects)
- **Hysteresis:** 1.0V — prevents load cycling around threshold

**Rationale for 11.5V disconnect (not 10.0V UV):**  
If discharge is allowed to continue to 10.0V (BMS UV trip), the pack is at near-zero SOC. Reconditioning from near-zero frequently accelerates aging. The LVD at 11.5V conserves ~10% SOC, leaves cells in a healthier state, and provides a visible "pack nearly empty" indication to the operator before complete shutdown.

### 8.2 Latching LVD

A non-latching LVD would reconnect immediately when a charger raises the battery voltage. This is usually correct. However, if the load itself (a large capacitor bank) caused a momentary voltage dip, a non-latching LVD would disconnect and reconnect repeatedly.

The firmware implements a latching LVD: once the LVD trips, the load is not reconnected until:
1. Pack voltage rises above 12.5V (charger connected and charging), AND
2. SOC estimate exceeds 20%

This prevents re-energization on a momentary recovery — the pack must genuinely be partially recharged before the load returns.

### 8.3 LVD vs. BMS UV Protection

These are different protections:

| Feature | BMS UV (BQ76920) | LVD (Firmware + Hardware) |
|---------|-----------------|--------------------------|
| Monitors | Per-cell voltage | Pack terminal voltage |
| Trip point | 2.5V/cell (10.0V pack) | 11.5V pack |
| Recovery | Automatic (cell V rises) | Requires charger + SOC > 20% |
| Response | 2–4 s (BQ76920 delay) | < 100 ms |
| Purpose | Final safety cutoff | Conservation / capacity protection |

Both must be present. The LVD is a conservation measure; the BMS UV is a safety measure.

---

## 9. MPPT SOLAR CHARGE INTEGRATION

### 9.1 Charge Setpoint Communication

The BMS ESP32 communicates charge setpoints to the MPPT controller ESP32 via a dedicated UART (9600 baud, ASCII protocol):

```
"VSET=14.40 ISET=25.0\n"   → Bulk charge: 14.4V limit, 25A limit
"VSET=14.40 ISET=10.0\n"   → Absorption: 14.4V, taper to 10A
"VSET=13.50 ISET=5.0\n"    → Float: 13.5V, minimal trickle
"VSET=0.0 ISET=0.0\n"      → Charge inhibit (temperature, OV fault)
```

The MPPT controller parses these commands and updates its PID setpoints accordingly. The BMS monitors I_charge via INA226; if the charge current exceeds ISET by > 10%, the BMS sends a reduced ISET command to the MPPT.

### 9.2 Temperature-Adjusted Charge Voltage

LiFePO4 charge voltage must be reduced at elevated cell temperature to prevent overvoltage stress on warmed cells (internal resistance drops; terminal voltage rises faster than expected):

```
V_adj = V_nominal - (T_cell - 25°C) × 0.003 V/°C × N_cells
      = 14.4V - (T - 25) × 0.012

At T = 40°C: V_adj = 14.4 - 0.18 = 14.22V
At T = 10°C: V_adj = 14.4 + 0.18 = 14.58V (slightly higher charge voltage acceptable in cold)
At T =  0°C: Charge inhibited entirely
```

The BMS reads NTC temperature every 2 seconds and sends an updated VSET command whenever the temperature changes by more than 2°C.

### 9.3 Multi-Source Charge Arbitration

When both solar and a mains charger are simultaneously connected:
- Both sources connect through separate Schottky diodes (MBR3045, 30A, 0.3V drop)
- The source with higher terminal voltage provides more current (natural ORing)
- BMS monitors total I_charge; if > I_max, reduces MPPT current limit
- Mains charger current cannot be reduced (no control interface); MPPT backs off

**Priority logic:**
1. If V_solar > V_mains + 0.1V: solar provides all current (mains Schottky reverse-biased)
2. If V_mains > V_solar: mains provides current; solar adds what it can
3. Combined current limited by BMS I_max command to MPPT

---

## 10. ESP32 BLE MONITOR SYSTEM

### 10.1 Bluetooth LE GATT Service

The ESP32 implements a BLE GATT server with a custom BMS service. Any BLE-capable device (phone, tablet, another ESP32) can connect and receive real-time battery data.

**BMS Service UUID:** `4fafc201-1fb5-459e-8fcc-c5c9c331914b`

**Characteristics:**

| Characteristic | UUID (suffix) | Data | Update Rate |
|---------------|--------------|------|-------------|
| Cell Voltages | ...26a8 | 4× float32 (V) | 2 s |
| SOC | ...26a9 | float32 (%) | 2 s |
| Current | ...26aa | 2× float32 (I_chg, I_load, A) | 250 ms |
| Temperature | ...26ab | 2× float32 (cell, FET, °C) | 2 s |
| Status Flags | ...26ac | uint8 (bit field) | 2 s |
| Efficiency | ...26ad | 2× float32 (coulombic %, energy %) | 2 s |

**Status flag byte:**
```
Bit 0: OV fault active
Bit 1: UV fault active
Bit 2: OC/SC fault active
Bit 3: Over-temperature active
Bit 4: CHG FET enabled
Bit 5: DSG FET enabled
Bit 6: LVD relay connected
Bit 7: Spare
```

### 10.2 BLE Client Application

Any BLE scanner application (nRF Connect, BLE Scanner, or custom app) can read the characteristics. The data encoding uses native float32 little-endian format.

**Python example (laptop/Raspberry Pi BLE client):**
```python
from bleak import BleakClient
import struct

BMS_SERVICE = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
CELLV_UUID  = "beb5483e-36e1-4688-b7f5-ea07361b26a8"
SOC_UUID    = "beb5483e-36e1-4688-b7f5-ea07361b26a9"

async def read_bms(address):
    async with BleakClient(address) as client:
        data = await client.read_gatt_char(CELLV_UUID)
        cells = struct.unpack('<ffff', data)
        print(f"Cells: {[f'{v:.3f}V' for v in cells]}")
        soc = struct.unpack('<f', await client.read_gatt_char(SOC_UUID))[0]
        print(f"SOC: {soc:.1f}%")
```

### 10.3 CYD Display Pages

The display rotates through four pages automatically (every 10 seconds):

1. **Cell Voltages:** Individual cell voltage bars + values; pack total at bottom
2. **SOC:** Large percentage readout; SOC bar; remaining time; EKF vs. coulomb comparison; temperature
3. **Power Flow:** Charge and load current; power; FET status indicators; Wh accumulators
4. **Efficiency:** Coulombic efficiency %; energy efficiency %; session Ah/Wh totals; BLE UUID reference

Any fault overrides all pages with a red alarm screen showing the active fault(s) and BQ76920 SYS_STAT register value.

### 10.4 Web Interface

The ESP32 hosts a web server on WiFi AP mode (SSID: `BMS-Monitor`, password: `hamradio`):
- Connect to `192.168.4.1` in any browser
- `/` — Summary HTML page (auto-refreshes every 3 s)
- `/api/status` — JSON telemetry (all measurements, faults, efficiency data)
- `/api/reset` — Reset session energy counters

---

## 11. PACK ASSEMBLY AND CELL WIRING

### 11.1 Cell Matching and Selection

**Never mix cells of different models, manufacturers, or ages in one series string.**  
Within a pack, all cells should have:
- Same model number (same chemistry lot)
- Capacity within 5% of each other (verify by individual discharge test before assembly)
- Internal resistance within 10% (measure with dedicated impedance meter at 1 kHz)
- Same initial SOC (pre-balance to same voltage before series connection)

**Pre-balance cells before first connection:**  
Individually charge each cell to the same voltage (e.g., 3.30V for LiFePO4) before connecting in series. If cells are connected at different voltages, large equalization currents flow between cells through the balance wires, potentially damaging the BMS balance circuit or cells.

### 11.2 LiFePO4 Prismatic Cell Assembly

**Cell orientation:** All terminals on same side. Positive terminal of Cell 1 faces away from negative terminal of Cell 2 (alternating orientation reduces wiring complexity for bus bars).

**Bus bar installation:**
1. Clean cell terminal surfaces with isopropyl alcohol
2. Apply anti-oxidant compound (NoAlox) to aluminum terminals
3. Torque M6 bolts to 4–5 N·m (35–44 in-lb) — do NOT over-torque
4. Verify 0V between adjacent cells (no accidental short) before final assembly
5. Install terminal covers (insulating caps) on all exposed bolt heads immediately

**Cell compression:**  
Apply 10–15 psi perpendicular to the large face of each cell:
- Two 3mm aluminum plates (or printed template + aluminum)
- Four M8 threaded rods through drilled holes in plates
- Tighten M8 nuts until plates are snug but cells are not visibly distorted
- Verify all four M8 fasteners are at equal tension (torque to 12 N·m each)

### 11.3 18650 Pack Assembly

**Spot welding vs. soldering:**  
Spot welding with 0.15mm × 8mm pure nickel strip is the correct method. Soldering directly to 18650 cell terminals is possible but risks thermal damage to the cell separator and anode. If spot welder is unavailable: use pre-welded tabbed cells, or use high-quality spring-contact cell holders (limited to < 5A).

**Parallel connections first:** Build each parallel group (e.g., 2P or 4P) before connecting in series. Connecting parallel cells at equal voltage first avoids large equalization currents.

**Balancing nickel strips:** Route nickel strips so all parallel cells in a group have equal resistance to the series connection point. Asymmetric resistance leads to unequal current sharing during charging and discharging.

### 11.4 Balance Tap Wiring (Critical)

The balance tap wires carry only milliamperes but must be connected correctly. An error in balance wiring is the most common BMS failure cause.

**Wire from BMS connector to cell junctions (4S):**
- B− (BMS pin): Cell 1 negative terminal
- VC1 (BMS pin): Junction between Cell 1 positive and Cell 2 negative
- VC2 (BMS pin): Junction between Cell 2 positive and Cell 3 negative
- VC3 (BMS pin): Junction between Cell 3 positive and Cell 4 negative
- B+ (BMS pin): Cell 4 positive terminal

**Use color-coded wires:** Black (B−), Red-1 (VC1), Red-2 (VC2), Red-3 (VC3), Red-4 (B+). Apply colored heat shrink at both ends.

**Measurement verification:** Before final BMS connection, measure voltage at each tap point:
- B− to B−: 0V (reference)
- B− to VC1: V_cell1 (should be ~3.2–3.6V)
- B− to VC2: V_cell1 + V_cell2 (should be ~6.4–7.2V)
- B− to VC3: V_cell1 + V_cell2 + V_cell3 (should be ~9.6–10.8V)
- B− to B+: Full pack voltage (should be ~12.8–14.4V)

Incorrect wiring at this step can immediately destroy the BMS IC.

---

## 12. INSTALLATION AND COMMISSIONING

### 12.1 Pre-Commissioning Checklist

Before powering the BMS system for the first time:

- [ ] All cells measured individually; voltages within 100 mV of each other (or pre-balanced)
- [ ] Balance tap wires verified with voltmeter (see Section 11.4)
- [ ] BMS sense resistor terminals correct (SRP on load side, SRN on cell side)
- [ ] MOSFET gate pull-down resistors installed (prevents floating gate at startup)
- [ ] Output fuse or Anderson Powerpole connection verified for polarity
- [ ] No tools, wires, or metallic debris near cell terminals
- [ ] Temperature sensor attached to cell body (not in air gap)
- [ ] ESP32 programmed and I2C scan confirms BQ76920 at address 0x08

### 12.2 First Power-Up Sequence

1. Connect balance tap wires to BMS (one at a time, starting from B−)
2. Verify BMS LED or ESP32 boot indication
3. Check that CHG and DSG FETs are enabled (firmware default)
4. Measure V_out at Anderson Powerpole terminal: should equal pack voltage
5. Connect load (resistor bank, not valuable radio equipment)
6. Verify load current reads correctly on CYD display and BLE
7. Connect charger; verify charge current reads correctly

### 12.3 Initial Calibration

**SOC initialization:**  
On first power-up, the EKF initializes from OCV if the pack has been at rest. If the pack was just assembled, let it rest at least 30 minutes before checking SOC. The firmware reads pack voltage; if current is < 100 mA, it uses the OCV-SOC table to initialize SOC.

**Manual SOC override** (web interface if OCV initialization is uncertain):
```
POST http://192.168.4.1/api/setsoc
Body: soc=50.0
```

**Current calibration verification:**  
Apply a known load (e.g., 10 Ω resistor: I = 12.8V / 10 = 1.28A). Verify INA226 reads 1.28A ± 0.05A. If not, check shunt resistance and INA226 calibration register value.

---

## 13. TEST PROCEDURES

### 13.1 BMS Protection Test — Overvoltage

**Procedure:** Apply variable bench supply to cell tap VC1 (Cell 1 +). Increase voltage to above 3.65V while monitoring BQ76920 SYS_STAT via I2C.

**Expected result:** BQ76920 OV flag sets (bit 3 in SYS_STAT = 0x00); CHG FET disabled within configured delay (2 s for OV_DELAY = 0b10); ALERT pin pulses LOW; display shows OV fault.

**Acceptance:** OV protection fires at ≤ 3.70V on affected cell. CHG FET confirmed disabled by voltmeter at output (charge current blocked).

### 13.2 BMS Protection Test — Undervoltage

**Procedure:** Drain one cell with a resistive load on its balance tap until V_cell < 2.80V.

**Expected result:** BQ76920 UV flag sets; DSG FET disabled within 4 s; display shows UV fault.

**Recovery test:** Apply charger. UV fault should clear when cell V rises above UV_TRIP threshold.

### 13.3 BMS Protection Test — Short Circuit

**Caution:** Short circuit test must be performed with a controlled resistance (0.01–0.1 Ω), not a dead short. Place a 0.05 Ω test resistor temporarily across P+ / P−.

**Procedure:** Monitor drain current with oscilloscope current probe. Connect test resistor. Record time to FET turn-off.

**Expected result:** BQ76920 SCD fires within 70–200 µs; FET turn-off within 100 µs of SCD.

### 13.4 LVD Test

**Procedure:** Reduce pack voltage artificially by connecting a resistive load until V_pack < 11.5V.

**Expected result:** LVD relay opens; display shows "LVD TRIP"; load disconnects. Pack voltage stabilizes (no load). Apply charger; verify load does not reconnect until V_pack > 12.5V and SOC > 20%.

### 13.5 EKF SOC Accuracy Test

**Procedure:**
1. Fully charge pack; set SOC = 100% via web interface.
2. Discharge at constant 5A load for exactly 10 Ah (stop when coulomb counter reads 50%).
3. Let pack rest for 2 hours.
4. Record EKF SOC estimate.
5. Compare to expected: 100% − (10 Ah / Q_rated × 100%) = expected SOC.

**Acceptance:** EKF SOC within ±5% of expected after rest period. Coulomb counting alone within ±8% acceptable. If EKF drifts > 10% from expected, check ADCGAIN calibration and INA226 offset.

### 13.6 Charge Efficiency Measurement

**Procedure:**
1. Reset energy counters (web interface: `/api/reset`).
2. Charge pack from 50% to 100% at constant 5A rate. Record Ah_in and Wh_in from display.
3. Discharge pack from 100% to 50% at constant 5A rate. Record Ah_out and Wh_out.
4. Calculate: η_Q = Ah_out / Ah_in; η_E = Wh_out / Wh_in.

**Expected results (LiFePO4):**
- Coulombic efficiency η_Q: 97–99%
- Round-trip energy efficiency η_E: 92–96%

Values below 90% for η_Q indicate:
- Current sensor error (recheck INA226 calibration)
- High self-discharge (check for balance resistors left enabled)
- Cell internal resistance too high (aging or damaged cell)

---

## 14. FAULT ISOLATION AND TROUBLESHOOTING

### 14.1 Troubleshooting Philosophy — The Electron's Path

Trace the electron from cell negative, through the BMS FET stack, to the load. At each node, verify expected voltage and current. Where voltage or current is wrong, the fault is nearby.

For BMS faults: isolate whether the fault is in the sensing path (wrong reading triggers false protection) or in the switching path (true fault exists but is not being cleared).

### 14.2 Fault Table

| Symptom | Probable Cause | Diagnostic |
|---------|---------------|------------|
| No output; no fault flags | BMS FETs off at startup; SYS_CTRL2 not written | Read BQ_SYS_CTRL2; write 0x43 to re-enable |
| OV fault; all cells normal | Noise on VC pin; missing bypass cap | Add 0.1µF from affected VC pin to V− |
| UV fault; cells all > 3.0V | Wrong UV_TRIP register; ADCOFFSET error | Re-read ADCGAIN/ADCOFFSET; recalculate |
| OV fault fires at wrong voltage | Incorrect ADCGAIN calibration | Measure actual cell V; compare to BQ76920 reading; correct calibration |
| FETs won't re-enable after fault | SYS_STAT flag not cleared; fault condition still present | Check SYS_STAT register; fix fault; write 0xFF to SYS_STAT |
| Balance resistors burning up | Balance FETs left on continuously | Check CELLBAL1 register; verify firmware disables after balance |
| SOC drifts > 10% per hour | INA226 offset error; wrong shunt resistance programmed | Disconnect load; measure I at zero current; should be < 5 mA |
| EKF never converges | Pack never rests long enough (60 s) | Ensure load is off for > 60 s; check rest detection threshold |
| BLE not connecting | BLE advertising stopped after client disconnect | Power cycle ESP32; verify BLEDevice::startAdvertising() in onDisconnect |
| MPPT not responding to setpoints | UART wiring; baud rate mismatch; ESP32 not sending | Check UART wire (TX→RX, RX←TX); verify mpptSerial.begin(9600) |
| LVD trips at wrong voltage | Divider resistors incorrect; comparator reference | Measure V at LM393 (+) and (−) pins with known V_pack |
| Charge efficiency < 90% | Balance resistor left enabled during discharge | Disable balance FETs when not in top-balance; check bal_mask |

### 14.3 I2C Communication Debug

Use the ESP32 serial port (115200 baud) for diagnostic output. Add temporary code:

```cpp
// I2C device scan
for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
        Serial.printf("Found device at 0x%02X\n", addr);
    }
}
// Expected: 0x08 (BQ76920), 0x40 (INA226), 0x41 (INA226), 0x48 (ADS1115)
```

If BQ76920 (0x08) is not found:
- Verify 3.3V on BQ76920 REG pin (requires > 1.5V between cell taps)
- Check 10Ω resistor from VC4 (top cell) to REG pin
- Check V− pin (should be tied to CELL1−, not a floating ground)
- Verify 4.7 kΩ I2C pull-ups to 3.3V on both SDA and SCL

---

## 15. PREVENTIVE MAINTENANCE

### 15.1 Quarterly Inspection

**Cell inspection:**
- Visual: look for swollen cells (pouch/prismatic) or protruding top buttons (18650)
- Swelling indicates gas generation — cell is failing; isolate and dispose properly
- Check bus bar connections for oxidation; clean and re-torque if needed

**BMS inspection:**
- Check balance resistors for discoloration or burned smell (overheating)
- Verify terminal screws are tight (heat cycling loosens terminals)
- Clean ventilation openings; remove any insect nests or debris

**Software:**
- Review charge/discharge efficiency trend; declining η_E indicates aging
- Check balance log: cells that require > 2 hours to balance at 90% SOC are diverging
- Reset session energy counters; start fresh efficiency baseline

### 15.2 Annual Capacity Test

A full capacity test provides the most accurate assessment of pack health:

1. Charge to 100% SOC using standard CC/CV profile
2. Rest 1 hour
3. Discharge at C/5 rate (10A for 50 Ah pack) to UV cutoff
4. Record discharged Ah from INA226 accumulator
5. Compare to rated capacity: Health % = Measured Ah / Rated Ah × 100%

**Action thresholds:**
- > 90%: Pack is healthy; continue operation
- 80–90%: Monitor quarterly; plan replacement
- < 80%: Replace pack; capacity loss is significant for field use
- Any cell with V_max < 3.4V at full charge: isolate that cell; replace pack

### 15.3 Storage Preparation

For storage > 2 weeks:
1. Charge to 50% SOC (approximately 13.1V for 4S LiFePO4)
2. Disconnect all loads including ESP32 (prevents slow self-discharge through BMS quiescent current)
3. Store in cool, dry location (15–25°C ideal; avoid > 35°C or freezing)
4. Check voltage monthly; recharge to 50% if voltage drops below 12.5V (4S LiFePO4)
5. Do NOT store Li-Ion cells fully charged — store at 40–60% SOC

---

## APPENDIX A: COMPLETE PARTS LISTS

See individual schematic files for complete per-circuit parts lists:
- `sch_lifepo4_4s_bms.txt` — BQ76920 primary BMS, FET stage
- `sch_liion_bms.txt` — BQ76930 and S-8254 Li-Ion BMS
- `sch_cell_balancing.txt` — Passive and active balance circuits
- `sch_protection_lvd.txt` — TL431/LM393 backup protection, LVD, pre-charge
- `sch_mppt_integration.txt` — Source ORing diodes, UART interface, INA226

---

## APPENDIX B: BQ76920 REGISTER REFERENCE

| Register | Address | Description | Default |
|---------|---------|-------------|---------|
| SYS_STAT | 0x00 | Status flags (OV/UV/SCD/OCD/OVRD/XREADY/WAKE) | 0x00 |
| CELLBAL1 | 0x01 | Balance FET enable (bits 0–4) | 0x00 |
| SYS_CTRL1 | 0x04 | ADC_EN, TEMP_SEL, SHUT_A, SHUT_B | 0x00 |
| SYS_CTRL2 | 0x05 | CHG_ON, DSG_ON, DELAY_DIS, CC_EN, CC_ONESHOT | 0x00 |
| PROTECT1 | 0x06 | SCD_THRESH[2:0], SCD_DELAY[1:0] | — |
| PROTECT2 | 0x07 | OCD_THRESH[3:0], OCD_DELAY[2:0] | — |
| PROTECT3 | 0x08 | OV_DELAY[1:0], UV_DELAY[1:0] | — |
| OV_TRIP | 0x09 | Overvoltage threshold | — |
| UV_TRIP | 0x0A | Undervoltage threshold | — |
| VC1_HI/LO | 0x0C/0D | Cell 1 voltage (14-bit) | — |
| VC2_HI/LO | 0x0E/0F | Cell 2 voltage | — |
| VC3_HI/LO | 0x10/11 | Cell 3 voltage | — |
| VC4_HI/LO | 0x12/13 | Cell 4 voltage | — |
| ADCGAIN1 | 0x50 | ADC gain calibration bits[7:5] | OTP |
| ADCOFFSET | 0x51 | ADC offset (signed, mV) | OTP |
| ADCGAIN2 | 0x59 | ADC gain calibration bits[2:1] | OTP |
| CC_HI/LO | 0x32/33 | Coulomb counter (signed 16-bit) | — |

---

## APPENDIX C: KALMAN FILTER MATHEMATICS

**Full derivation for the scalar EKF (single state):**

```
State transition: x(k) = x(k-1) + u(k) + w(k)
  where u(k) = η_c × I_net × Δt / Q_nom  (control input: coulomb count increment)
        w(k) ~ N(0, Q)                     (process noise)

Observation model: y(k) = h(x(k)) + v(k)
  where h(x) = SOC_from_OCV(x)            (nonlinear OCV-SOC mapping)
        v(k) ~ N(0, R)                     (measurement noise)

Prediction:
  x̂⁻(k) = x̂(k-1) + u(k)
  P⁻(k)  = P(k-1) + Q

Update (at rest periods only):
  H(k)   = ∂h/∂x |at x̂⁻(k)    (Jacobian: slope of OCV-SOC table)
  S(k)   = H(k)² P⁻(k) + R
  K(k)   = P⁻(k) H(k) / S(k)
  x̂(k)  = x̂⁻(k) + K(k)[y(k) - x̂⁻(k)]
  P(k)   = [1 - K(k) H(k)] P⁻(k)

Noise covariances (empirically tuned):
  Q = 1 × 10⁻⁵   (small: current sensor is accurate)
  R = 0.01        (larger: OCV-SOC table has uncertainty near flat region)
```

---

## APPENDIX D: BLE GATT SERVICE DEFINITION

```
Service: BMS Monitor Service
UUID: 4fafc201-1fb5-459e-8fcc-c5c9c331914b

Characteristics:
  Cell Voltages
    UUID: beb5483e-36e1-4688-b7f5-ea07361b26a8
    Properties: READ | NOTIFY
    Format: 4× IEEE-754 float32 LE (bytes 0–3: cell1, 4–7: cell2, 8–11: cell3, 12–15: cell4)
    Unit: Volts

  State of Charge (EKF)
    UUID: beb5483e-36e1-4688-b7f5-ea07361b26a9
    Properties: READ | NOTIFY
    Format: float32 LE
    Unit: Percent (0.0 – 100.0)

  Current (Charge / Load)
    UUID: beb5483e-36e1-4688-b7f5-ea07361b26aa
    Properties: READ | NOTIFY
    Format: 2× float32 LE (bytes 0–3: I_charge, 4–7: I_load)
    Unit: Amperes

  Temperature (Cell / FET)
    UUID: beb5483e-36e1-4688-b7f5-ea07361b26ab
    Properties: READ | NOTIFY
    Format: 2× float32 LE (bytes 0–3: T_cell, 4–7: T_FET)
    Unit: Celsius

  Status Flags
    UUID: beb5483e-36e1-4688-b7f5-ea07361b26ac
    Properties: READ | NOTIFY
    Format: uint8
    Bits: 0=OV, 1=UV, 2=OC, 3=OT, 4=CHG_EN, 5=DSG_EN, 6=LVD, 7=spare

  Efficiency
    UUID: beb5483e-36e1-4688-b7f5-ea07361b26ad
    Properties: READ | NOTIFY
    Format: 2× float32 LE (bytes 0–3: coulombic_eff, 4–7: energy_eff)
    Unit: Percent
```

---

## APPENDIX E: FILE INDEX

```
/ant_gear/bms/
├── schematics/
│   ├── sch_lifepo4_4s_bms.txt      BQ76920 4S LiFePO4 BMS, FET stage, calibration
│   ├── sch_liion_bms.txt           BQ76930, S-8254, 3S/4S Li-Ion, cell matching
│   ├── sch_cell_balancing.txt      Passive (R+FET), active (JK BMS), algorithm
│   ├── sch_protection_lvd.txt      TL431 OV, LM393 UV/OC, SCD, LVD, pre-charge
│   └── sch_mppt_integration.txt    Solar integration, charge arbitration, efficiency
├── firmware/
│   └── bms_monitor_esp32.ino       CYD BMS monitor: BQ76920 driver, EKF SOC,
│                                   BLE GATT server, LVD, MPPT UART, web UI
├── openscad/
│   ├── enclosure_bms.scad          Controller housing, 18650 holders, prismatic frame
│   └── mounting_brackets_bms.scad  Panel mount, cell tray, mast clamp, DIN clip,
│                                   shunt block, NTC clip, terminal covers, handle
└── MANUAL_BMS.md                   This document (TM-BMS-001 Rev A)
```

---

*TM-BMS-001 Rev A — End of Document*  
*Portable Battery Management System — Field Reference for Amateur Radio Operation*
