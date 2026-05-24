# RF POWER METER SYSTEM
## TECHNICAL MANUAL

**Document Number:** TM-PWR-001 Rev A  
**Equipment:** RF Power Meter System — Thruline, Inline Directional, and Terminating Types  
**Applicability:** HF (1.8–30 MHz), VHF (50–148 MHz), UHF (420–1300 MHz)  
**Classification:** UNCLASSIFIED  
**Date:** 2026-05-01  

---

```
CHAPTER/SECTION                                                    PAGE
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CHAPTER 1   INTRODUCTION AND GENERAL INFORMATION . . . . . . . . .  1-1
CHAPTER 2   THEORY OF OPERATION . . . . . . . . . . . . . . . . .  2-1
CHAPTER 3   DETECTOR CIRCUITS . . . . . . . . . . . . . . . . . .  3-1
CHAPTER 4   EQUIPMENT DESCRIPTION . . . . . . . . . . . . . . . .  4-1
CHAPTER 5   INSTALLATION . . . . . . . . . . . . . . . . . . . .  5-1
CHAPTER 6   OPERATION . . . . . . . . . . . . . . . . . . . . . .  6-1
CHAPTER 7   CALIBRATION . . . . . . . . . . . . . . . . . . . . .  7-1
CHAPTER 8   FIRMWARE REFERENCE . . . . . . . . . . . . . . . . .  8-1
CHAPTER 9   DATA LOGGING AND ANALYSIS . . . . . . . . . . . . . .  9-1
CHAPTER 10  TROUBLESHOOTING . . . . . . . . . . . . . . . . . . . 10-1
APPENDIX A  SPECIFICATIONS . . . . . . . . . . . . . . . . . . .   A-1
APPENDIX B  BILL OF MATERIALS . . . . . . . . . . . . . . . . . .  B-1
APPENDIX C  COUPLER DESIGN WORKSHEET . . . . . . . . . . . . . .   C-1
APPENDIX D  ELEMENT COLOR CODE AND SELECTION GUIDE . . . . . . . . D-1
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

---

## LIST OF WARNINGS, CAUTIONS, AND NOTES

```
WARNING — LETHAL RF EXPOSURE . . . . . . . . . . . . . . . . . .  1-3
WARNING — HIGH-VOLTAGE TRANSIENTS . . . . . . . . . . . . . . . .  1-4
WARNING — OVERLOAD DESTRUCTION . . . . . . . . . . . . . . . . .  1-4
CAUTION  — CONNECTORS BEFORE RF APPLICATION . . . . . . . . . . .  5-3
CAUTION  — SLUG INSERTION SEQUENCE . . . . . . . . . . . . . . .  6-2
CAUTION  — CALIBRATION FREQUENCY MATCH . . . . . . . . . . . . .  7-2
CAUTION  — SD CARD HOT-SWAP PROHIBITED . . . . . . . . . . . . .  9-2
NOTE     — DETECTOR SQUARE-LAW REGION . . . . . . . . . . . . . .  2-8
NOTE     — TOROID CORE MATERIAL SATURATION . . . . . . . . . . .  4-5
NOTE     — TEMPERATURE SETTLING TIME . . . . . . . . . . . . . .  7-3
```

---

# CHAPTER 1 — INTRODUCTION AND GENERAL INFORMATION

## 1.1 SCOPE

### 1.1.1 Purpose

1.1.1.1  This manual provides operation, calibration, maintenance, and theory of operation information for the RF Power Meter System, designation TM-PWR-001. The system comprises three instruments:

a. **Thruline Directional Wattmeter** (Bird-type) — Model TM-PWR-BIRD-001. A sampling-type directional wattmeter employing interchangeable plug-in elements for measurement of forward and reflected power in coaxial transmission lines. Frequency coverage 1.8 MHz to 1300 MHz with appropriate elements.

b. **Inline Directional Power Meter** — Model TM-PWR-ESP32-001. A fixed-element, auto-ranging directional power meter with ESP32 digital display, SD card data logging, and Bluetooth output. Frequency coverage HF, VHF, or UHF depending on installed coupler (compile-time selection).

c. **Terminating Power Meter** — Model TM-PWR-TERM-001. An absorbing-type power meter with integral dummy load, for use where the transmitter must see a resistive termination during measurement. Available with resistor array (≤100 W), oil-cooled (≤1 kW), or coaxial (UHF ≤50 W) load.

### 1.1.2 Applicability

1.1.2.1  Equipment described herein is applicable to:

- Amateur radio station transmitter adjustment and testing
- Antenna tuner adjustment and SWR optimization  
- Field propagation research and portable operation
- Transmission line fault diagnosis
- Dummy load power verification

1.1.2.2  All three instruments measure the same fundamental quantities (forward power, reflected power, SWR, return loss) by different physical means suited to different measurement scenarios.

### 1.1.3 Related Documents

```
Table 1-1. Related Technical References
─────────────────────────────────────────────────────────────────
Document                         Subject
─────────────────────────────────────────────────────────────────
TM-SWR-001                       SWR Meter System Manual
IPC-2221B                        Generic Standard on PCB Design
MIL-STD-461G                     Electromagnetic Compatibility
ARRL Handbook (current ed.)      RF fundamentals reference
Fair-Rite Catalog                Ferrite core specifications
Coilcraft AN-1                   Toroid winding guidelines
Hewlett-Packard AN 1089          Diode detector theory
─────────────────────────────────────────────────────────────────
```

---

## 1.2 EQUIPMENT SUMMARY

### 1.2.1 Functional Description

1.2.1.1  All three instruments are based on the **directional coupler** principle. A directional coupler is a four-port network that separates forward-traveling and reverse-traveling waves on a transmission line, making each available for independent measurement without disturbing the main transmission path.

1.2.1.2  Schottky diode detectors convert the coupled RF signal to a DC voltage proportional to RF power. This DC voltage is measured by a precision analog-to-digital converter (ADS1115, 16-bit I²C) or applied directly to a calibrated moving-coil meter movement.

1.2.1.3  The ESP32 microcontroller computes power in Watts, SWR, return loss, reflection coefficient, and transmission efficiency. Results are displayed on an OLED or CYD (ILI9341 320×240 TFT) display, logged to SD card in CSV format, and streamed via Bluetooth JSON and USB serial CSV simultaneously.

### 1.2.2 Performance Summary

```
Table 1-2. System Performance Summary
─────────────────────────────────────────────────────────────────────────────────
Parameter               HF (1.8–30 MHz)   VHF (50–148 MHz)  UHF (420–1300 MHz)
─────────────────────────────────────────────────────────────────────────────────
Power ranges (W)        1–10, 10–100,      1–10, 10–100,     1–10, 10–100,
                        100–1000, 1–3k     100–1000          100–1000
Frequency accuracy      ±0.5 MHz (manual)  ±0.5 MHz          ±1 MHz
SWR accuracy            ±0.1 (SWR ≤3)     ±0.15 (SWR ≤3)    ±0.2 (SWR ≤3)
Power accuracy          ±5% (calibrated)  ±7% (calibrated)  ±10% (calibrated)
Dynamic range           30 dB (Schottky)  30 dB             25 dB
Directivity             ≥30 dB            ≥26 dB            ≥22 dB
Insertion loss          ≤0.5 dB           ≤0.7 dB           ≤1.0 dB
VSWR (instrument)       ≤1.05             ≤1.05             ≤1.10
Coupling factor         −30 dB (toroid)   −26 dB (binocul.) −25 dB (microstrip)
Temperature range       −10°C to +55°C    −10°C to +50°C    0°C to +45°C
Power supply            USB-C 5V/1A       USB-C 5V/1A       USB-C 5V/1A
Display                 OLED or CYD       OLED or CYD       OLED or CYD
SD logging              Yes               Yes               Yes
Bluetooth               SPP (BT 2.0)      SPP (BT 2.0)      SPP (BT 2.0)
─────────────────────────────────────────────────────────────────────────────────
```

---

## 1.3 SAFETY PRECAUTIONS

### 1.3.1 RF Hazards

```
╔══════════════════════════════════════════════════════════════════╗
║                            WARNING                               ║
║                                                                  ║
║  RF ENERGY AT POWER LEVELS MEASURED BY THIS INSTRUMENT CAN      ║
║  CAUSE SEVERE BURNS, RF INTERFERENCE, AND EQUIPMENT DAMAGE.     ║
║  DO NOT CONTACT OR APPROACH EXPOSED RF CONDUCTORS, CONNECTOR    ║
║  CENTER PINS, OR ANTENNA ELEMENTS WHILE RF IS APPLIED. ENSURE   ║
║  ALL CONNECTIONS ARE COMPLETED BEFORE APPLYING RF POWER.        ║
╚══════════════════════════════════════════════════════════════════╝
```

```
╔══════════════════════════════════════════════════════════════════╗
║                            WARNING                               ║
║                                                                  ║
║  AT POWER LEVELS ABOVE 100 W, HIGH-VOLTAGE TRANSIENTS CAN       ║
║  APPEAR ON TRANSMISSION LINE COMPONENTS DURING SWITCHING.       ║
║  RELAY SWITCHING AND RANGE CHANGES SHALL NOT BE PERFORMED       ║
║  WHILE RF POWER IS APPLIED.                                      ║
╚══════════════════════════════════════════════════════════════════╝
```

```
╔══════════════════════════════════════════════════════════════════╗
║                            WARNING                               ║
║                                                                  ║
║  EXCEEDING THE RATED INPUT POWER FOR THE INSTALLED ELEMENT OR   ║
║  RANGE WILL DESTROY DETECTOR DIODES, OVERLOAD PROTECTION        ║
║  COMPONENTS, AND MAY DAMAGE THE INSTRUMENT PERMANENTLY. VERIFY  ║
║  THE POWER RANGE BEFORE APPLYING RF.                            ║
╚══════════════════════════════════════════════════════════════════╝
```

### 1.3.2 Handling Precautions

1.3.2.1  Observe electrostatic discharge (ESD) precautions when handling plug-in elements and PCBs. The Schottky detector diodes have a gate threshold of approximately 200 mV and may be damaged by static discharge without proper grounding.

1.3.2.2  The terminating meter dummy load becomes hot during high-power operation. Do not touch the load assembly or heatsink fins during or immediately after operation above 50 W.

1.3.2.3  Do not operate the instrument in the presence of flammable vapors or gases.

---

## 1.4 NOMENCLATURE

### 1.4.1 Abbreviations

```
Table 1-3. Abbreviations
─────────────────────────────────────────────────────
Abbreviation    Definition
─────────────────────────────────────────────────────
ADC             Analog-to-Digital Converter
BT              Bluetooth
C               Coupling factor (dB)
CSV             Comma-Separated Values
CYD             Cheap Yellow Display (ILI9341 TFT)
ESR             Equivalent Series Resistance
FWD             Forward (incident) power
Γ               Reflection coefficient (gamma)
GPIO            General Purpose Input/Output
I²C             Inter-Integrated Circuit (bus)
JSON            JavaScript Object Notation
LDO             Low-Dropout regulator
NTC             Negative Temperature Coefficient
NVS             Non-Volatile Storage
OLED            Organic Light-Emitting Diode
PCB             Printed Circuit Board
PGA             Programmable Gain Amplifier
REF             Reflected power
RL              Return Loss (dB)
RMS             Root Mean Square
SPI             Serial Peripheral Interface
SWR             Standing Wave Ratio
TVS             Transient Voltage Suppressor
USB             Universal Serial Bus
VCC             Supply voltage
VSWR            Voltage SWR (synonym for SWR in this text)
WDT             Watchdog Timer
Z₀              Characteristic impedance (50 Ω nominal)
─────────────────────────────────────────────────────
```

---

# CHAPTER 2 — THEORY OF OPERATION

## 2.1 TRANSMISSION LINE FUNDAMENTALS

### 2.1.1 Traveling Waves

2.1.1.1  In a coaxial transmission line, RF power propagates as electromagnetic waves traveling in both the forward (source to load) and reverse (load to source) directions simultaneously. When the load impedance equals the characteristic impedance Z₀ of the line, all forward power is absorbed and no reverse wave exists.

2.1.1.2  When a mismatch exists at the load, the forward wave is partially reflected. The ratio of reflected to forward voltage defines the **reflection coefficient** Γ (gamma):

```
         V_reflected
Γ  =   ─────────────
         V_forward

         Z_load − Z₀
Γ  =   ─────────────────
         Z_load + Z₀
```

2.1.1.3  Γ is in general a complex number with magnitude |Γ| ranging from 0 (perfect match) to 1.0 (total reflection). Practical measurements use the magnitude only.

2.1.1.4  Forward and reflected power are related to Γ by:

```
  P_reflected = |Γ|² × P_forward
```

### 2.1.2 SWR Definition

2.1.2.1  Standing wave ratio (SWR) is the ratio of maximum to minimum voltage magnitude on a transmission line. It is related to |Γ| by:

```
         1 + |Γ|
SWR  =  ─────────
         1 − |Γ|
```

2.1.2.2  Since P_ref / P_fwd = |Γ|², the SWR can be computed from measured power values:

```
         1 + √(P_ref / P_fwd)
SWR  =  ──────────────────────
         1 − √(P_ref / P_fwd)
```

2.1.2.3  SWR is dimensionless and is quoted as a ratio, e.g., SWR 1.5:1. The ":1" is commonly omitted. SWR = 1.0 is a perfect match. SWR = ∞ indicates total reflection (open or short circuit termination).

### 2.1.3 Return Loss

2.1.3.1  Return loss (RL) expresses the reflected power in dB below the forward power:

```
RL (dB)  =  −20 × log₁₀(|Γ|)
          =  −10 × log₁₀(P_ref / P_fwd)
```

2.1.3.2  Return loss is always expressed as a positive dB value. A higher return loss indicates a better match. Reference values:

```
Table 2-1. Return Loss vs. SWR vs. Reflection Coefficient
─────────────────────────────────────────────────────────
RL (dB)    SWR      |Γ|     P_ref/P_fwd (%)
─────────────────────────────────────────────────────────
∞ (∞)      1.00     0.000       0.00%        (perfect)
26.4       1.10     0.048       0.23%
20.8       1.20     0.091       0.83%
17.7       1.30     0.130       1.69%
14.0       1.50     0.200       4.00%
9.5        2.00     0.333      11.1%
7.0        2.50     0.429      18.4%
4.4        3.50     0.556      30.9%
0.0         ∞       1.000     100.0%        (total refl.)
─────────────────────────────────────────────────────────
```

### 2.1.4 Power and Efficiency

2.1.4.1  Net power delivered to the load equals forward power minus reflected power:

```
P_net  =  P_fwd − P_ref  =  P_fwd × (1 − |Γ|²)
```

2.1.4.2  Transmission efficiency is the percentage of forward power delivered:

```
Efficiency (%)  =  (P_net / P_fwd) × 100
               =  (1 − P_ref/P_fwd) × 100
```

---

## 2.2 DIRECTIONAL COUPLER THEORY

### 2.2.1 Four-Port Network

2.2.1.1  A directional coupler is a four-port network with the following ports:

```
           ┌───────────────────┐
  Port 1 ──┤  INPUT            ├── Port 2    THRULINE (main path)
  (source)  │                   │  (load)
           │                   │
  Port 3 ──┤  COUPLED (FWD)    ├── Port 4   ISOLATED (terminated)
  (det fwd) │                   │  (terminated 50Ω)
           └───────────────────┘
```

2.2.1.2  An ideal directional coupler routes a fraction of the forward-traveling wave to Port 3, while Port 4 sees none of the forward wave (infinite directivity). With reversed power flow (Port 2 as source), Port 4 receives the coupled signal and Port 3 is isolated.

2.2.1.3  In practice, finite directivity limits the isolation between forward and reflected ports. Directivity D is:

```
D (dB)  =  RL_coupled_to_isolated_fwd
         =  S43_dB − S41_dB
```

Typical values: toroid 35–40 dB, microstrip 20–25 dB.

### 2.2.2 Coupling Factor

2.2.2.1  The coupling factor C relates the coupled port output to the main line power:

```
C (dB)  =  20 × log₁₀(V_coupled / V_main)
         =  10 × log₁₀(P_coupled / P_main)
```

C is always a negative number (P_coupled < P_main). This instrument uses:

```
Table 2-2. Coupling Factors by Band and Coupler Type
─────────────────────────────────────────────────────────────────
Band    Coupler Type              Nominal C    Core/Substrate
─────────────────────────────────────────────────────────────────
HF      FT-114-43 toroid         −30 dB       Fair-Rite type 43
VHF     BN-43-202 binocular      −26 dB       Fair-Rite type 43
UHF     Coupled microstrip       −25 dB       FR4 εr = 4.4
─────────────────────────────────────────────────────────────────
```

2.2.2.2  For the toroid coupler, coupling is determined by the turns ratio:

```
N₁  =  primary turns (1T, the main line passes through once)
N₂  =  secondary turns (32T for HF, 20T for VHF, etc.)
C   =  −20 × log₁₀(N₁/N₂)  [dB]

Example: N₁=1, N₂=32 → C = −20 × log₁₀(1/32) = −30.1 dB ✓
```

### 2.2.3 Conversion from Coupled Voltage to Line Power

2.2.3.1  Given a calibrated coupled voltage V_coupled (from the detector, after calibration), the forward power on the main line is:

```
P_coupled  =  V_coupled² / (2 × Z₀_det)    [W, RMS]
             where Z₀_det = 50 Ω (detector termination)

P_main     =  P_coupled × 10^(−C/10)        [W]
             where C is coupling in dB (negative)

Equivalently:
P_main     =  P_coupled / 10^(C/10)
```

2.2.3.2  With attenuator pads (range selection):

```
P_main  =  P_coupled × 10^(−C/10) × 10^(A/10)

where A = attenuator value in dB (positive, adds to denominator coupling)
```

2.2.3.3  In firmware, this is implemented as `mvToWatts()` in `calibration.h`:

```cpp
inline float mvToWatts(float v_mv, float coupling_db, float atten_db) {
    float v_v      = v_mv / 1000.0f;
    float p_coupled = (v_v * v_v) / 50.0f;           // W at detector
    float c_lin    = powf(10.0f, (-coupling_db)/10.0f); // coupling factor
    float a_lin    = powf(10.0f, (atten_db)/10.0f);     // attenuator
    return p_coupled * c_lin * a_lin;                  // W on main line
}
```

---

## 2.3 HF COUPLER — TOROID DESIGN

### 2.3.1 Core Selection

2.3.1.1  The HF coupler uses a **Fair-Rite 5943000401** ferrite toroid (equivalent to Amidon FT-114-43), outer diameter 29 mm, inner diameter 19 mm, height 7.5 mm, mix type 43.

2.3.1.2  Mix type 43 (MnZn ferrite) provides:
- Relative permeability μᵣ ≈ 850 at 100 kHz
- Usable frequency range: 1 MHz to 40 MHz
- Low core loss at HF frequencies
- AL value ≈ 523 nH/turn² (inductance per turn squared)

2.3.1.3  With 32 turns on the secondary, secondary inductance is:

```
L_sec  =  AL × N²  =  523 nH × 32²  =  535 µH

At 1.8 MHz:  X_L  =  2π × 1.8×10⁶ × 535×10⁻⁶  =  6,060 Ω >> 50 Ω ✓
At 30 MHz:   X_L  =  2π × 30×10⁶ × 535×10⁻⁶  =  101,000 Ω >> 50 Ω ✓
```

The secondary impedance is much larger than the 50 Ω detector termination across the entire HF band, ensuring the coupler functions as a true current transformer.

### 2.3.2 Termination and Directivity

2.3.2.1  The secondary (32T winding) is terminated at one end with 51 Ω. Two Schottky detectors, one at each end of the secondary (with opposite polarity), extract the forward and reflected wave components.

2.3.2.2  Directivity is achieved by the 180° phase reversal. For a forward wave:
- V_fwd detector sees: V_transformer + V_coupled (same phase) → maximum
- V_ref detector sees: V_transformer − V_coupled (opposing) → minimum

For a reflected wave, the polarity reverses.

2.3.2.3  Practical directivity of a well-wound toroid coupler exceeds 35 dB across 1.8–30 MHz. Primary degradation mechanisms are stray capacitance (degrades above 10 MHz) and lead inductance.

---

## 2.4 VHF COUPLER — BINOCULAR CORE DESIGN

### 2.4.1 Core Selection

2.4.1.1  The VHF coupler uses a **Fair-Rite BN-43-202** binocular (double-aperture) ferrite core, 19.1 mm × 6.1 mm × 14.3 mm, mix type 43.

2.4.1.2  The binocular form factor permits:
- 2T primary (main line passes through both holes twice)
- 20T secondary in the opposing aperture
- Closer coupling between primary and secondary

2.4.1.3  At 50–148 MHz, toroid permeability begins to decrease, making the binocular form with fewer turns preferable. Coupling factor: −26 dB.

### 2.4.2 Directivity Trim

2.4.2.1  VHF couplers include directivity trim capacitors C_trim (10–47 pF) at the detector nodes. These compensate for stray capacitance that degrades directivity above 100 MHz.

2.4.2.2  Trim procedure: With transmitter applied and load well-matched (SWR ≤ 1.05), adjust C_trim to minimize the reflected detector output. Optimum setting provides maximum directivity at the operating frequency.

---

## 2.5 UHF COUPLER — COUPLED MICROSTRIP DESIGN

### 2.5.1 Stripline Geometry

2.5.1.1  At UHF (420–1300 MHz), ferrite cores have excessive loss. The UHF coupler uses a **coupled microstrip** transmission line section on FR4 PCB (εr = 4.4, h = 1.6 mm):

```
┌─────────────────────────────────────────────────────────────┐
│         W_main = 3.0mm                                      │
│   ┌──────────────────────────────────────────────────────┐  │
│   │  MAIN LINE  (L = 48.0 mm)                50Ω         │  │
│   └──────────────────────────────────────────────────────┘  │
│         gap S = 0.35 mm                                     │
│   ┌──────────────────────────────────────────────────────┐  │
│   │  COUPLED LINE (L = 48.0 mm)               50Ω        │  │
│   └──────────────────────────────────────────────────────┘  │
│         W_coupled = 2.4 mm                                  │
└─────────────────────────────────────────────────────────────┘
```

2.5.1.2  The coupled section length L = 48.0 mm is λ/4 at 860 MHz (the geometric center of the UHF coverage band):

```
λ/4  =  c / (4 × f × √εr_eff)
     =  3×10⁸ / (4 × 860×10⁶ × √2.90)
     ≈  51.6 mm in free space → 48 mm on FR4 substrate
```

2.5.1.3  Coupling factor varies with frequency across the UHF band. The dispersion table in `calibration.h` provides measured correction values:

```
Table 2-3. UHF Coupling Dispersion Correction
─────────────────────────────────────────────────────
Freq (MHz)    Coupling (dB)    Correction vs. −25 dB
─────────────────────────────────────────────────────
420           −26.2            −1.2 dB (less coupling)
500           −25.8            −0.8 dB
600           −25.4            −0.4 dB
700           −25.1            −0.1 dB
800           −25.0             0.0 dB (reference)
860           −25.0             0.0 dB
900           −24.8            +0.2 dB
1000          −24.5            +0.5 dB
1100          −24.1            +0.9 dB
1200          −23.8            +1.2 dB
1296          −23.5            +1.5 dB (more coupling)
─────────────────────────────────────────────────────
Note: Values are for this specific PCB geometry.
Measure on VNA after fabrication; update table if different.
─────────────────────────────────────────────────────
```

2.5.1.4  Via stitching (0.5 mm diameter, 1.27 mm pitch) along both coupler edges is mandatory to prevent parallel-plate mode resonances that degrade directivity above 500 MHz.

---

# CHAPTER 3 — DETECTOR CIRCUITS

## 3.1 SCHOTTKY DIODE DETECTORS

### 3.1.1 Operating Principle

3.1.1.1  A Schottky diode detector rectifies the coupled RF signal, producing a DC output voltage proportional to the RF input. The two operating regimes are:

**Square-law region** (V_RF < η × kT/q ≈ 25 mV at 300 K):

```
V_out  =  k × V_RF²   (V_out ∝ P_RF)
```

**Linear region** (V_RF >> η × kT/q):

```
V_out  =  V_RF − V_forward_drop   (V_out ∝ V_RF ∝ √P_RF)
```

```
╔══════════════════════════════════════════════════════════════════╗
║                             NOTE                                 ║
║                                                                  ║
║  THE SQUARE-LAW REGION GIVES OUTPUT DIRECTLY PROPORTIONAL TO    ║
║  POWER, WHICH IS DESIRABLE. FOR MAXIMUM ACCURACY, DETECTOR      ║
║  OPERATING POINT SHOULD BE IN THE SQUARE-LAW REGION (INPUT      ║
║  BELOW APPROXIMATELY 25 mV RMS AT THE DETECTOR).                ║
║                                                                  ║
║  AT FULL-SCALE RF INPUT, THE DETECTOR WILL OPERATE IN THE       ║
║  TRANSITION/LINEAR REGION. THE CALIBRATION PROCEDURE ACCOUNTS   ║
║  FOR THIS, BUT ACCURACY DEGRADES AT EXTREMES OF THE RANGE.      ║
╚══════════════════════════════════════════════════════════════════╝
```

### 3.1.2 Device Selection by Band

3.1.2.1  Detector diode selection is critical for frequency coverage:

```
Table 3-1. Schottky Detector Selection Guide
──────────────────────────────────────────────────────────────────────────────
Device       Band        f_max    Cj0     Package   Key Parameter
──────────────────────────────────────────────────────────────────────────────
1N5711       HF          500 MHz  2 pF    DO-35     Low V_f, general purpose
HSMS-2822    VHF         4 GHz    1.1 pF  SOT-23    Matched dual, ΔV_f<1mV
HSMS-2852    UHF         6 GHz    0.18pF  SOT-23    Ultra-low Cj, high freq
BAT54        Peak det.   300 MHz  10 pF   SOT-23    Peak hold, fast recovery
AD8361       RMS det.    2.5 GHz  —       SOIC-8    True RMS, 50 dB range
LTC5596      Log det.    40 GHz   —       SC-70     Log slope 35 mV/dB
──────────────────────────────────────────────────────────────────────────────
```

3.1.2.2  The HSMS-2822 and HSMS-2852 are **matched dual** packages. Using a matched pair for forward and reflected detectors minimizes tracking error between channels, which directly improves SWR accuracy at low mismatch levels.

### 3.1.3 Basic Detector Circuit

3.1.3.1  The standard detector circuit for HF and VHF:

```
  RF_IN ──┬── D1 (Schottky) ──┬── V_DET (DC out)
          │      (series)      │
         R_in                  C_filter (10 nF)
         51Ω                   │
          │                   R_load (10 kΩ)
          │                    │
         GND ─────────────────GND
```

3.1.3.2  Component values:
- R_in: 51 Ω, input termination (matched to secondary winding termination)
- C_filter: 10 nF (HF), 1 nF (VHF), 100 pF (UHF) — removes RF ripple
- R_load: 10 kΩ, provides DC return and defines time constant

3.1.3.3  Time constant τ = R_load × C_filter:
- HF: τ = 10 kΩ × 10 nF = 100 µs (adequate for 1 MHz, excessive for fast envelope)
- VHF: τ = 10 kΩ × 1 nF = 10 µs
- UHF: τ = 10 kΩ × 100 pF = 1 µs

---

## 3.2 TEMPERATURE COMPENSATION

### 3.2.1 Schottky Temperature Characteristic

3.2.1.1  The Schottky diode forward voltage V_f decreases approximately 2 mV/°C. This means detector output voltage increases as temperature falls, causing the meter to read high in cold conditions and low when hot.

3.2.1.2  The temperature sensitivity coefficient for a typical Schottky detector in the square-law regime is approximately α = 0.023/°C. The correction factor applied by the firmware is:

```
K_comp  =  exp(α × (T_measured − T_cal))

P_corrected  =  P_raw / K_comp
```

where T_cal is the temperature at which the scale calibration was performed (default 25°C).

### 3.2.2 NTC Thermistor Measurement

3.2.2.1  A 10 kΩ NTC thermistor (B3950) is mounted adjacent to the detector diodes and connected in a voltage divider:

```
  +3.3V
    │
   R_bias (10 kΩ, 1%)
    │
    ├──── ADC_TEMP (ADS1115 A3)
    │
   NTC (10 kΩ @ 25°C, B=3950)
    │
   GND
```

3.2.2.2  Temperature from the ADC voltage:

```
V_ntc  =  ADC reading (mV)
R_ntc  =  R_bias × V_ntc / (V_cc − V_ntc)
         =  10000 × V_ntc / (3300 − V_ntc)

1/T    =  1/T₀ + (1/B) × ln(R_ntc / R_25)
         where T₀ = 298.15 K, B = 3950, R_25 = 10000

T_°C   =  (1/T) − 273.15
```

3.2.2.3  In firmware, this is implemented in `calibration.h` as `PowerCalibration::ntcTempC()`.

### 3.2.3 Complementary Bridge Method (Hardware Alternative)

3.2.3.1  An alternative hardware approach uses two matched Schottky diodes in a bridge configuration where the temperature coefficients cancel. One diode is in the detector path; the second diode (same lot, same die) is a compensation reference:

```
  V_RF_in ── D_detect ──┬── V_det
                         │
                        D_comp (no RF, in thermal contact with D_detect)
                         │
                        GND
```

3.2.3.2  The HSMS-2822 and HSMS-2852 matched dual packages are ideally suited for this approach. Temperature error can be reduced to < 0.1 dB over a 50°C range when properly implemented.

---

## 3.3 PEAK DETECTOR CIRCUIT

### 3.3.1 Peak Hold Operation

3.3.1.1  The peak power detector uses a BAT54 fast Schottky diode charging a 1 µF hold capacitor:

```
  V_fwd (from coupler) ──── BAT54 ──┬── V_peak (to ADC A2)
                                     │
                                    C_pk (1 µF)     R_leak (1 MΩ)
                                     │                    │
                                    GND ───────────────── ┘

  GPIO_PEAK_RST ── 2N7000 ──┬── drain
                             └── source to GND
                                (discharges C_pk when GPIO goes HIGH)
```

3.3.1.2  Hold time constant τ = R_leak × C_pk = 1 MΩ × 1 µF = 1 second. The capacitor retains peak voltage for approximately 3–5 seconds before decaying to 63% of peak. For indefinite hold, GPIO_PEAK_RST is kept low (2N7000 off).

3.3.1.3  Manual peak reset: GPIO27 pulsed high for 20 ms, discharging C_pk through the 2N7000. Button BTN_PEAK (GPIO34) triggers this via firmware.

3.3.1.4  Automatic peak reset occurs when firmware changes range (to prevent false peaks from the previous range appearing on the new range scale).

---

## 3.4 OVERLOAD PROTECTION

### 3.4.1 Three-Stage Protection

3.4.1.1  Three overload protection stages are implemented in cascade:

**Stage 1 — Series Resistor:**
A 47 Ω series resistor limits current to the detector diode under overload conditions. Maximum diode current = (V_peak − V_f) / 47 Ω. At V_peak = 5 V, I_max ≈ 90 mA (safe for 1N5711).

**Stage 2 — Schottky Clamp:**
Back-to-back BAT54 diodes clamp V_det to approximately ±0.3 V (Schottky V_f). This limits detector input and protects against reverse bias breakdown.

**Stage 3 — TVS Clamp:**
SMAJ5.0A transient voltage suppressor (TVS) clamps the detector output node to 5.1 V standoff, 7.5 V clamp. Protects ADC input which has an absolute maximum of 6.144 V (ADS1115, ±VCC + 0.3 V).

3.4.1.2  The firmware monitors ADC readings; if Vdet > 4800 mV (VDET_OVERLOAD_MV), it commands range-up via relay switching.

---

## 3.5 AUTO-RANGING

### 3.5.1 Range System

3.5.1.1  Four power ranges are implemented using relay-switched Pi-pad attenuators:

```
Table 3-2. Range Attenuation and Relay States
──────────────────────────────────────────────────────────────────
Range   Max Power   Attenuation   Relay_1   Relay_2   RANGE_STR
──────────────────────────────────────────────────────────────────
0       10 W        0 dB          OFF       OFF       "1-10W"
1       100 W       20 dB         ON        OFF       "10-100W"
2       1000 W      30 dB         ON        ON        "0.1-1kW"
3       3000 W      35 dB         —         —         "1-3kW"
──────────────────────────────────────────────────────────────────
Note: Range 3 uses an additional 5 dB pad hard-switched with RELAY_2.
```

3.5.1.2  Relay settling time after switching: 15 ms (RELAY_SETTLE_MS). ADC readings taken during this window are discarded.

3.5.1.3  Auto-range logic (in `esp32_power_meter.ino`):
- If Vfwd_mv > VDET_OVERLOAD_MV (4800 mV): increment range (range-up)
- If Vfwd_mv < VDET_MIN_MV × 3 (60 mV) and range > 0: decrement range (range-down)
- Hysteresis: 3:1 ratio between up and down thresholds prevents hunting

---

# CHAPTER 4 — EQUIPMENT DESCRIPTION

## 4.1 THRULINE DIRECTIONAL WATTMETER (BIRD-TYPE)

### 4.1.1 Body Assembly

4.1.1.1  The thruline wattmeter body (TM-PWR-BIRD-001) is constructed in two halves (top and bottom) of 3D-printed PETG or machined aluminum. Principal dimensions:

```
Table 4-1. Thruline Body Dimensions
─────────────────────────────────────
Dimension               Value
─────────────────────────────────────
Overall length          155 mm
Outer diameter          50 mm
Main bore diameter      8 mm
Slug slot inner dia.    25.2 mm (slip fit for 25.0 mm slug)
O-ring groove dia.      25.0 mm
O-ring groove width     2.3 mm (Buna-N −113 O-ring)
Meter window            72 × 32 mm
RF connector            SO-239 both ends (5/8-24 thread)
Center conductor        N/A (slug provides)
─────────────────────────────────────
```

4.1.1.2  The slug slot is a transverse cylindrical bore through the body center, with a D-shaped orientation flat that mates with the D-flat on the plug-in element. This keying prevents insertion of an element in the wrong rotational position, and provides the 180° rotation for forward/reflected direction selection.

4.1.1.3  Alignment pins (3 mm diameter × 10 mm) and four M4 screws hold the top and bottom halves together. Body assembly sequence: install slug, position O-ring, join halves, torque M4 screws to 0.5 N·m.

### 4.1.2 Plug-in Element Design

4.1.2.1  The plug-in element is a self-contained directional coupler and detector module in a D-shaped cylindrical housing 25.0 mm × 25.4 mm:

```
Table 4-2. Plug-in Element Types
───────────────────────────────────────────────────────────────────────────────────
Element  Band   Freq (MHz)    Max W    Core      Turns   Coupling   Color
───────────────────────────────────────────────────────────────────────────────────
HF-10    HF     1.8–30        10 W     FT-114-43  32T    −30 dB    Green
HF-100   HF     1.8–30        100 W    FT-114-43  10T    −20 dB    Yellow
HF-1K    HF     1.8–30        1000 W   FT-114-43 100T    −40 dB    Orange
HF-3K    HF     1.8–30        3000 W   FT-114-43 178T    −45 dB    Red
VHF-10   VHF    50–148        10 W     BN-43-202   2T/20T −26 dB   Blue
VHF-100  VHF    50–148        100 W    BN-43-202   1T/10T −20 dB   Purple
UHF-10   UHF    420–1300      10 W     Microstrip   —     −25 dB   White
UHF-100  UHF    420–1300      100 W    Microstrip   —     −19 dB   Black
───────────────────────────────────────────────────────────────────────────────────
Note: UHF elements use a 22 × 18 mm Rogers RO4003C PCB wafer with capacitive coupler.
───────────────────────────────────────────────────────────────────────────────────
```

4.1.2.2  Element output is a 2 mm banana jack on the D-flat face. The body provides a spring-loaded banana pin contact that mates with this jack. A 200 µA moving-coil meter is connected between this banana contact and chassis ground.

4.1.2.3  The full-scale deflection resistor (R_FS) is calculated per element:

```
V_FS  =  √(P_FS × Z₀) × 10^(C/20)   [V at detector secondary]

Example (HF-10, P_FS=10W, Z₀=50Ω, C=−30dB):
V_FS  =  √(10 × 50) × 10^(−30/20)  =  22.4 × 0.0316  =  0.708 V

R_FS  =  V_FS / I_FS  =  0.708 / 200×10⁻⁶  =  3,540 Ω
```

### 4.1.3 Direction Selection

4.1.3.1  Rotating the element 180° about its long axis switches the measurement direction:

- **D-flat facing panel meter** = Forward power measurement
- **D-flat facing away from panel meter** = Reflected power measurement

4.1.3.2  The D-flat orientation key prevents ambiguous insertion. Only two defined rotational positions exist: 0° (forward) and 180° (reflected). No intermediate positions are possible.

```
╔══════════════════════════════════════════════════════════════════╗
║                            CAUTION                               ║
║                                                                  ║
║  REMOVE RF POWER BEFORE ROTATING THE ELEMENT. THE BANANA PIN    ║
║  CONTACT BREAKS THE DETECTOR CIRCUIT DURING ROTATION. DO NOT    ║
║  FORCE THE ELEMENT THROUGH INTERMEDIATE POSITIONS.              ║
╚══════════════════════════════════════════════════════════════════╝
```

---

## 4.2 INLINE DIRECTIONAL POWER METER

### 4.2.1 PCB Assembly

4.2.1.1  The inline power meter PCB (100 × 80 mm, 2-layer, HASL) contains:

- Directional coupler (band-specific: toroid/binocular/microstrip)
- Two Schottky detector stages (forward + reflected)
- Peak detector (BAT54 + 1 µF)
- Three-stage overload protection (R + Schottky clamp + TVS)
- NTC thermistor in voltage divider
- ADS1115 16-bit I²C ADC (4-channel)
- Two range relay Pi-pads (PCF8574 I²C expander or direct GPIO)
- ESP32-WROOM-32 module
- AMS1117-3.3 LDO regulator, USB-C power input
- MicroSD card socket (SPI)
- HC-05 Bluetooth SPP module (or ESP32 internal BT)

4.2.1.2  PCB layout constraints:
- RF main line: 2.0 mm trace width (50 Ω, FR4 h=1.6mm)
- Detector traces: 0.5 mm, short as possible
- Ground plane both layers; no splits under RF section
- Decoupling capacitors (100 nF) at each IC VCC pin, placed within 2 mm

### 4.2.2 Enclosure

4.2.2.1  The inline meter enclosure (130 × 90 × 50 mm, PETG or ABS) provides:

- SO-239 connectors, both side walls (or N-type for UHF)
- OLED window (28 × 16 mm) or CYD window (73 × 54 mm)
- Bargraph LED window (90 × 5 mm)
- 4 push-button holes (front face): PEAK_RST, AVG_CYCLE, RANGE, LOG
- SD card slot (rear)
- USB-C power input (rear)
- Rear vent slots for convection cooling

---

## 4.3 TERMINATING POWER METER

### 4.3.1 Load Assembly Types

4.3.1.1  Three load configurations are available:

**Type A — Resistor Array (≤100 W):**
- 8× 390 Ω, 25 W non-inductive cement resistors in parallel (net 48.75 Ω ≈ 50 Ω)
- Mounted on anodized aluminum heatsink, 0.6°C/W
- Bolted directly to enclosure floor (thermal compound interface)

**Type B — Oil-Cooled (≤1000 W):**
- 24× 1200 Ω, 20 W resistors, 8 parallel × 3 series = 50 Ω
- Submerged in transformer oil (Nynas Nytro 3000 or equivalent)
- Steel tank, 6 mm walls, 200 × 100 × 100 mm
- Natural convection cooling

**Type C — Coaxial (UHF ≤50 W):**
- High-power carbon film resistor in coaxial geometry
- Inner conductor: 10.3 mm diameter (50 Ω coaxial geometry)
- Compatible through 1.3 GHz; VSWR < 1.05 to 1 GHz

4.3.1.2  Thermal protection:
- DS18B20 OneWire temperature sensor mounted on load element
- Firmware alarm at 70°C (TEMP_ALERT_C): yellow LED + BT notification
- Firmware shutdown at 85°C (TEMP_SHUTDOWN_C): red LED, SD log entry, TX inhibit relay opens (if connected)

---

# CHAPTER 5 — INSTALLATION

## 5.1 PRE-INSTALLATION INSPECTION

### 5.1.1 Unpacking and Inspection

**Procedure 5-1: Pre-Installation Inspection**

1. Remove instrument from shipping container. Retain all packaging.
2. Inspect exterior for shipping damage. Document any damage before proceeding.
3. Verify all connectors are present and undamaged.
4. Check that SO-239/N-type connector center pins are straight and undamaged.
5. For thruline meter: verify plug-in elements are present per packing list.
6. Verify SD card is installed (if ordering included SD card).
7. Power-on test: apply USB-C 5V. Verify OLED/CYD display illuminates within 2 seconds.

```
╔══════════════════════════════════════════════════════════════════╗
║                            CAUTION                               ║
║                                                                  ║
║  VERIFY ALL RF CONNECTORS ARE FULLY TIGHTENED BEFORE APPLYING   ║
║  RF POWER. A LOOSE CONNECTOR UNDER HIGH-POWER RF WILL ARC,      ║
║  DESTROYING THE CONNECTOR AND POTENTIALLY THE INSTRUMENT.       ║
╚══════════════════════════════════════════════════════════════════╝
```

---

## 5.2 INSTALLATION IN A TRANSMISSION LINE

### 5.2.1 Inline Meter Installation

**Procedure 5-2: Inline Meter Installation**

1. Remove power from transmitter.
2. Disconnect coaxial cable at antenna tuner or antenna feedpoint.
3. Connect RF_IN port of meter to transmitter output with minimum-length jumper.
4. Connect RF_OUT port to antenna or tuner.
5. Tighten all PL-259/SO-239 connections finger-tight plus 1/4 turn with wrench.
6. Route USB-C power cable to power source (laptop USB, phone charger, or 5V supply).
7. Allow 2 minutes for instrument to reach thermal equilibrium before calibration check.

5.2.1.1  Maximum cable length from transmitter to meter RF_IN: not specified, but keep short to minimize insertion loss. Insertion loss of instrument is ≤0.5 dB (HF), ≤0.7 dB (VHF), ≤1.0 dB (UHF). This loss represents power dissipated in the coupler and attenuator pads and does not affect accuracy.

### 5.2.2 Thruline Meter Installation

**Procedure 5-3: Thruline Meter Installation**

1. Select the correct element for the frequency and maximum power expected (see Table 4-2).
2. Remove power from transmitter.
3. Insert element with D-flat facing AWAY from meter window (= forward power position).
4. Secure element with retaining O-ring seated in groove.
5. Connect SO-239 connectors to transmission line.
6. Tighten all connectors.
7. Apply minimal RF and verify upscale deflection. If deflection is reverse (below zero), element is in reflected position — rotate 180°.

---

# CHAPTER 6 — OPERATION

## 6.1 INITIAL STARTUP

**Procedure 6-1: Initial Startup — Inline/Terminating Meter**

1. Apply USB-C 5V power.
2. Observe startup sequence on OLED/CYD display:
   - Firmware version, device name, build date
   - ADC initialization message
   - SD card: "found" or "not found"
   - Bluetooth: "RF-PowerMeter ready"
   - Calibration: "CAL LOADED" or "NO CAL — use defaults"
3. Allow 2-minute warmup for thermal stabilization.
4. Set frequency (MHz) via Bluetooth command SETFREQ or front panel (if rotary encoder installed).
5. Verify display shows correct band (HF/VHF/UHF) in upper right corner.
6. Select display mode with touch tab or mode button.
7. Apply RF.

---

## 6.2 DISPLAY MODES

### 6.2.1 LIVE Mode

6.2.1.1  LIVE mode is the primary operating mode. The display shows:

```
┌─────────────────────────────────────────┐  CYD 320×240 display
│ [LIVE] [PEAK] [LOG] [SWP]  BT● HF      │  ← Top bar
│                                          │
│  FWD  47.3 W                            │  ← Large power readout
│                                          │
│  SWR  1.52    ████████░░░░░░░░░░░░░░    │  ← SWR value + bargraph
│               1.0        3.0      10.0   │  ← Bargraph scale
│    _____                                 │  ← SWR trend sparkline
│   /     \___                            │     (60-point history)
│                                          │
│  REF: 3.7 W    RL: 12.2 dB             │  ← Footer line
│                                          │
│ [RANGE] [PEAK RST] [LOG] [AUTO/MAN]    │  ← Bottom bar touch
└─────────────────────────────────────────┘
```

6.2.1.2  Bargraph color coding:
- Green: SWR 1.0–1.5 (good match, no action required)
- Yellow: SWR 1.5–2.5 (acceptable, monitor)
- Red: SWR 2.5–10.0 (poor match, corrective action required)

### 6.2.2 PEAK Mode

6.2.2.1  PEAK mode displays the highest power and SWR values observed since last reset:

```
┌─────────────────────────────────────────┐
│ [LIVE] [PEAK] [LOG] [SWP]  BT● HF      │
│                                          │
│  PEAK FWD: 98.4 W    Elapsed: 00:02:17  │
│  PEAK SWR: 2.30                         │
│                                          │
│  Live FWD: 47.1 W    Live SWR: 1.52    │
│                                          │
│  [         PRESS TO RESET PEAK        ] │
└─────────────────────────────────────────┘
```

### 6.2.3 LOG Mode

6.2.3.1  LOG mode shows data logging status:

```
┌─────────────────────────────────────────┐
│ [LIVE] [PEAK] [LOG] [SWP]  BT● HF      │
│                                          │
│  SD:  LOGGING ● /pwrmtr_00003.csv       │
│  Records: 1,847  File: 1.8 MB           │
│                                          │
│  BT:  CONNECTED  (RF-PowerMeter)        │
│  USB: STREAMING                         │
│                                          │
│  [START/STOP LOG]  [NEW FILE]           │
└─────────────────────────────────────────┘
```

### 6.2.4 SWEEP Mode

6.2.4.1  SWEEP mode displays frequency sweep results as an XY chart:

```
┌─────────────────────────────────────────┐
│ [LIVE] [PEAK] [LOG] [SWP]  BT● HF      │
│                                          │
│ SWR│                                    │
│ 3.0│         ╭─╮                       │
│ 2.5│       ╭─╯  ╰─╮                   │
│ 2.0│     ╭─╯       ╰╮                  │
│ 1.5│   ╭─╯           ╰────────────     │
│ 1.0│───╯                               │
│    └──────────────────────────────────  │
│    1.8      14.0       21.0       30.0  │
│                Frequency (MHz)          │
└─────────────────────────────────────────┘
```

6.2.4.2  Sweep is initiated by Bluetooth command: `SWEEP 1.8 30.0 0.1` (start, stop, step in MHz). The meter sweeps by prompting the operator to change frequency at each step.

---

## 6.3 FRONT PANEL CONTROLS

```
Table 6-1. Front Panel Button Functions
──────────────────────────────────────────────────────────────────────
Button        GPIO    Function
──────────────────────────────────────────────────────────────────────
BTN_PEAK      34      Short press: reset peak hold
                      Long press (>2s): toggle peak hold display
BTN_AVG       35      Cycle averaging window: FAST(100ms)→MED→SLOW→FAST
BTN_RANGE     36      Short press: manual range up
                      Long press: toggle auto/manual range
BTN_LOG       39      Toggle SD card logging on/off
──────────────────────────────────────────────────────────────────────
Note: All buttons have 50 ms debounce (BTN_DEBOUNCE_MS).
──────────────────────────────────────────────────────────────────────
```

---

## 6.4 BLUETOOTH COMMAND INTERFACE

### 6.4.1 Command Protocol

6.4.1.1  Connect to Bluetooth device "RF-PowerMeter" (pairing PIN: 1234 for HC-05, or no PIN for ESP32 BT SPP). Use any serial terminal at 9600 baud (HC-05) or BT SPP directly.

6.4.1.2  Commands are ASCII strings terminated by carriage return (CR, 0x0D) or newline (LF, 0x0A). Responses are ASCII terminated by CR+LF. All commands are case-insensitive.

### 6.4.2 Command Reference

```
Table 6-2. Bluetooth Command Reference
──────────────────────────────────────────────────────────────────────────────────
Command          Arguments        Function
──────────────────────────────────────────────────────────────────────────────────
STATUS           (none)           Request full JSON status packet
SETFREQ          <freq_mhz>       Set operating frequency (e.g., SETFREQ 14.2)
CALZERO          (none)           Perform zero calibration (no RF applied)
CALREF           <power_w>        Scale calibration with known reference power
PEAKRST          (none)           Reset peak hold registers
LOGON            (none)           Enable SD card logging
LOGOFF           (none)           Disable SD card logging
SWEEP            <f1> <f2> <fs>   Start frequency sweep (start/stop/step MHz)
RANGE            <0-3>            Force manual range (0=1-10W, 3=1-3kW)
RANGE AUTO       (none)           Return to automatic ranging
RESET            (none)           Software reset (watchdog-triggered)
INFO             (none)           Device info: firmware version, calibration state
──────────────────────────────────────────────────────────────────────────────────
```

### 6.4.3 JSON Status Packet

6.4.3.1  The STATUS command and automatic streaming return JSON:

```json
{
  "fwd_w":   "47.321",
  "ref_w":   "3.681",
  "peak_w":  "98.445",
  "swr":     "1.524",
  "rl_db":   "12.18",
  "freq_mhz": 14.2,
  "temp_c":  "23.4",
  "range":   "10-100W",
  "band":    "HF",
  "sd_log":  true,
  "log_n":   1847,
  "fw":      "1.0.0"
}
```

---

# CHAPTER 7 — CALIBRATION

## 7.1 CALIBRATION OVERVIEW

### 7.1.1 Calibration Requirements

7.1.1.1  Calibration establishes the relationship between detector output voltage and actual RF power on the main transmission line. Without calibration, the instrument uses default coupling constants from `config.h` which are accurate to approximately ±15% due to component tolerances.

7.1.1.2  Two calibration steps are required:

1. **Zero calibration** — Establishes the detector noise floor (diode offset voltage) with no RF applied.
2. **Scale calibration** — Establishes the forward detector scale factor with a known reference power applied.

7.1.1.3  Calibration data is stored in ESP32 Non-Volatile Storage (NVS), namespace "pwrmeter". Data persists across power cycles.

```
╔══════════════════════════════════════════════════════════════════╗
║                            CAUTION                               ║
║                                                                  ║
║  CALIBRATE AT OR NEAR THE OPERATING FREQUENCY. COUPLING FACTOR  ║
║  VARIES WITH FREQUENCY (ESPECIALLY UHF). A CALIBRATION         ║
║  PERFORMED AT 14 MHz IS NOT VALID FOR OPERATION AT 7 MHz        ║
║  WITHOUT RECHARACTERIZATION.                                    ║
╚══════════════════════════════════════════════════════════════════╝
```

### 7.1.2 Required Equipment

```
Table 7-1. Calibration Equipment Requirements
──────────────────────────────────────────────────────────────────────
Equipment                   Specification              Typical Device
──────────────────────────────────────────────────────────────────────
Reference power standard    Traceable ±2%, same band   NanoVNA (with
                                                        calibrated source)
RF signal source            Stable, settable power      QRP transceiver,
                                                        signal generator
50 Ω dummy load             VSWR < 1.05                HN-31 or equiv.
Bluetooth terminal          Any SPP-capable device      Phone + app
Coaxial cables              Minimum insertion loss       Quality RG-58
──────────────────────────────────────────────────────────────────────
```

---

## 7.2 ZERO CALIBRATION

**Procedure 7-1: Zero Calibration**

1. Ensure no RF is applied to the instrument. Terminate both RF ports with 50 Ω loads.
2. Apply USB-C power. Allow 2-minute warmup.
3. Connect Bluetooth terminal.
4. Send command: `CALZERO`
5. Firmware reads 16-sample average of all ADC channels (forward, reflected, peak).
6. Zero values are stored:
   - `fwd_zero_mv` = ADC A0 average (typically 5–25 mV; diode offset)
   - `ref_zero_mv` = ADC A1 average
7. Verify response: `CALZERO OK: fwd=12.3mV ref=8.7mV`
8. If zero values exceed 100 mV: inspect for RF ingress or detector damage.

```
╔══════════════════════════════════════════════════════════════════╗
║                             NOTE                                 ║
║                                                                  ║
║  ALLOW THE INSTRUMENT TO WARM UP FOR AT LEAST 2 MINUTES BEFORE  ║
║  PERFORMING ZERO CALIBRATION. SCHOTTKY DIODE OFFSET VOLTAGE     ║
║  CHANGES BY APPROXIMATELY 2 mV PER DEGREE C. CALIBRATING COLD   ║
║  WILL CAUSE HIGH-READING ERRORS AT OPERATING TEMPERATURE.       ║
╚══════════════════════════════════════════════════════════════════╝
```

---

## 7.3 SCALE CALIBRATION

**Procedure 7-2: Scale Calibration (Forward Channel)**

1. Complete zero calibration (Procedure 7-1) first.
2. Connect reference power standard (traceable power meter) in series: Source → Reference Standard → RF_IN port.
3. Connect a calibrated 50 Ω dummy load to RF_OUT.
4. Set source to operating frequency.
5. Apply known reference power. Note reference meter reading P_ref (Watts).
6. Send command: `CALREF <P_ref>` (example: `CALREF 10.0` for 10 W reference)
7. Firmware computes scale factor:
   ```
   V_expected = √(P_known × 10^(COUPLING_DB/10) × 50) × 1000  [mV]
   fwd_scale  = V_expected / (raw_fwd_mv − fwd_zero_mv)
   ```
8. Verify response: `CALREF OK: scale=1.024 cal_freq=14.200MHz`
9. Remove reference standard. Reconnect instrument to antenna system.

7.3.1  Calibration of the reflected channel uses the assumption of matched detectors (same lot, same die preferred). With a matched load on RF_OUT, the reflected detector should read near zero (plus zero offset). Firmware sets `ref_scale = fwd_scale` by default. For improved reflected accuracy, use a known reflection standard (open, short, or calibrated mismatch load).

---

## 7.4 ANALOG METER CALIBRATION (THRULINE BIRD-TYPE)

**Procedure 7-3: Analog Element Calibration**

1. Select element for the operating band and power range.
2. Apply reference power P_ref to the thruline body (RF in → dummy load).
3. Observe meter deflection d% (percentage of full scale).
4. Calculate actual full-scale power: P_FS_actual = P_ref / d%
5. Adjust full-scale trim potentiometer R_trim (if installed) until meter reads correctly.
6. Verify at two additional power levels (25% and 75% FS).

7.4.1  If no trim potentiometer is installed, the full-scale resistor R_FS can be adjusted:

```
R_FS_new = R_FS_old × (V_FS_actual / V_FS_nominal)

V_FS_actual = meter reads d% at P_ref
V_FS_nominal = √(P_FS × Z₀) × 10^(C/20)
```

---

## 7.5 UHF FREQUENCY-DEPENDENT CALIBRATION

7.5.1  The UHF coupler coupling factor varies with frequency by up to ±1.5 dB across the 420–1300 MHz band. For best accuracy, the dispersion table in `calibration.h` should be measured for the specific fabricated PCB using a VNA and updated.

**Procedure 7-4: UHF Dispersion Table Measurement**

1. Fabricate UHF coupler PCB.
2. Connect PCB to VNA: Port 1 → main line IN, Port 2 → main line OUT, measure S21.
3. Terminate coupled port with 50 Ω.
4. Sweep from 420 MHz to 1300 MHz.
5. Record coupling = S41 at each frequency in Table 2-3.
6. Update `UHF_DISP[]` array in `calibration.h` with measured values.
7. Recompile and flash firmware.

---

# CHAPTER 8 — FIRMWARE REFERENCE

## 8.1 ARCHITECTURE OVERVIEW

### 8.1.1 Firmware Modules

```
Table 8-1. Firmware Module Structure
──────────────────────────────────────────────────────────────────────
File                    Module          Function
──────────────────────────────────────────────────────────────────────
config.h                Configuration   Constants, pin assignments, band
                                        selection, NVS keys
calibration.h           Calibration     CalData struct, PowerCalibration
                                        class, PowerReading struct,
                                        mvToWatts(), computeSWR(),
                                        RollingAvg<N> template
data_logging.h          Logging         DataLogger class (SD + BT + USB),
                                        SweepLogger class
esp32_power_meter.ino   Main            setup(), loop(), ADC reading,
                                        display, command processing,
                                        WDT, DS18B20 (terminating)
──────────────────────────────────────────────────────────────────────
```

### 8.1.2 Main Loop Timing

8.1.2.1  The main loop executes the following tasks on each iteration:

```
Table 8-2. Main Loop Task Schedule
──────────────────────────────────────────────────────────────────────
Task                      Period          Notes
──────────────────────────────────────────────────────────────────────
ADC measurement           Every loop      ADS1115 × 16 oversample
Temperature compensation  Every loop      Applied to all power values
Auto-range check          Every loop      Relay switch if needed
Display update            100 ms (CYD)    CYD_UPDATE_MS
                          200 ms (OLED)   OLED_UPDATE_MS
SD logging                1000 ms         SD_LOG_INTERVAL_MS
Bluetooth streaming       500 ms          BT_LOG_INTERVAL_MS
USB serial CSV            200 ms          USB_LOG_INTERVAL_MS
DS18B20 temperature       2000 ms         Terminating meter only
Watchdog feed             Every loop      WDT_TIMEOUT_S = 30
Button scan               Every loop      50 ms debounce
BT command receive        Every loop      Non-blocking FIFO read
──────────────────────────────────────────────────────────────────────
```

---

## 8.2 MEASUREMENT PIPELINE

8.2.1  The measurement pipeline converts raw ADC counts to calibrated power values:

```
Step 1: ADC Read (×16 oversample)
  raw_fwd_counts → average → raw_fwd_mv
  raw_ref_counts → average → raw_ref_mv
  raw_peak_counts          → raw_peak_mv
  raw_temp_counts          → raw_temp_mv

Step 2: Temperature
  temp_c = ntcTempC(raw_temp_mv)
  k_comp = exp(alpha × (temp_c − 25.0))

Step 3: Calibration
  cal_fwd_mv  = applyFwd(raw_fwd_mv)   // subtract zero, scale
  cal_ref_mv  = applyRef(raw_ref_mv)
  cal_peak_mv = cal_fwd_mv applied to peak (peak uses fwd calibration)

Step 4: Temperature Correction
  cal_fwd_mv  /= k_comp
  cal_ref_mv  /= k_comp

Step 5: UHF Dispersion (UHF band only)
  coupling_db = uhfCouplingDB(freq_mhz)  // interpolated from table

Step 6: Power Computation
  fwd_w  = mvToWatts(cal_fwd_mv,  coupling_db, atten_db[range])
  ref_w  = mvToWatts(cal_ref_mv,  coupling_db, atten_db[range])
  peak_w = mvToWatts(cal_peak_mv, coupling_db, atten_db[range])

Step 7: Derived Quantities
  swr    = computeSWR(fwd_w, ref_w)
  rl_db  = −10 × log10(ref_w / fwd_w)
  gamma  = sqrt(ref_w / fwd_w)
  eff    = (1 − ref_w/fwd_w) × 100%

Step 8: Rolling Average
  fwdAvg.push(fwd_w)    // RollingAvg<50>
  refAvg.push(ref_w)
```

---

## 8.3 CONFIGURATION REFERENCE

### 8.3.1 Build-Time Configuration

8.3.1.1  The following `#define` values in `config.h` must be set before compiling:

**Meter Type (select one):**
```cpp
#define METER_TYPE_INLINE       // Inline directional: FWD + REF + SWR
// #define METER_TYPE_TERMINATING  // Terminating: forward power + dummy load temp
```

**Band (select one):**
```cpp
#define BAND_HF    // 1.8–30 MHz, FT-114-43 toroid, COUPLING_DB = −30
// #define BAND_VHF // 50–148 MHz, BN-43-202 binocular, COUPLING_DB = −26
// #define BAND_UHF // 420–1300 MHz, coupled microstrip, COUPLING_DB = −25
```

8.3.1.2  After changing `config.h`, rebuild and flash the firmware. Do not attempt to update band or meter type at runtime.

---

## 8.4 CYD DISPLAY FIRMWARE

### 8.4.1 CYD Hardware

8.4.1.1  The CYD (Cheap Yellow Display) is an ESP32 development board with:
- ILI9341 320×240 TFT display (SPI interface)
- XPT2046 resistive touchscreen
- USB-C power
- TFT_BL (GPIO21) PWM backlight control
- TOUCH_CS (GPIO33)

8.4.1.2  The CYD operates as a **Bluetooth master**, connecting to "RF-PowerMeter" (the inline/terminating meter's BT name) and displaying the received data. The CYD does not perform any RF measurements itself.

### 8.4.2 Touch Zone Layout

8.4.2.1  Touch zones for the CYD display:

```
Table 8-3. CYD Touch Zone Coordinates
──────────────────────────────────────────────────────────────────────
Zone Name        X range     Y range     Function
──────────────────────────────────────────────────────────────────────
Tab LIVE         0–79        0–29        Switch to LIVE mode
Tab PEAK         80–159      0–29        Switch to PEAK mode
Tab LOG          160–239     0–29        Switch to LOG mode
Tab SWEEP        240–319     0–29        Switch to SWEEP mode
BT Status        280–319     0–29        (display only, no action)
RANGE button     0–79        210–239     Cycle manual range
PEAK RST button  80–159      210–239     Reset peak hold
LOG toggle       160–239     210–239     Toggle SD logging
AUTO/MAN         240–319     210–239     Toggle auto-range
──────────────────────────────────────────────────────────────────────
```

---

# CHAPTER 9 — DATA LOGGING AND ANALYSIS

## 9.1 SD CARD LOGGING

### 9.1.1 File Format

9.1.1.1  SD card log files are written in CSV format with the following header:

```
timestamp_ms,freq_mhz,fwd_w,ref_w,peak_w,swr,rl_db,gamma,eff_pct,
vfwd_mv,vref_mv,vpeak_mv,temp_c,range,band
```

9.1.1.2  Example data row:

```
1234567,14.200,47.321,3.682,98.445,1.524,12.18,0.280,92.2,
142.3,39.7,295.1,23.4,1,HF
```

9.1.1.3  File naming: `/pwrmtr_NNNNN.csv` where NNNNN is a zero-padded 5-digit number. Files are numbered sequentially at each power-on to avoid overwriting previous sessions.

9.1.1.4  File rolling: When a log file reaches 10 MB (SD_MAX_FILE_SIZE), a new file is automatically opened and the record counter resets.

```
╔══════════════════════════════════════════════════════════════════╗
║                            CAUTION                               ║
║                                                                  ║
║  DO NOT REMOVE THE SD CARD WHILE LOGGING IS ACTIVE. DATA IN     ║
║  THE WRITE BUFFER WILL BE LOST AND THE FILE SYSTEM MAY BE       ║
║  CORRUPTED. USE BTN_LOG OR BLUETOOTH "LOGOFF" COMMAND TO        ║
║  STOP LOGGING BEFORE REMOVING THE SD CARD.                      ║
╚══════════════════════════════════════════════════════════════════╝
```

### 9.1.2 SD Card Requirements

9.1.2.1  SD card specifications:
- Format: FAT32
- Capacity: 2 GB minimum, 32 GB maximum (FAT32 limit)
- Speed class: Class 10 or UHS-I recommended
- Maximum file size: 10 MB per session (firmware limit; actual FAT32 limit is 4 GB)

**Procedure 9-1: SD Card Format**

1. Insert SD card in PC card reader.
2. Format as FAT32 (not exFAT; not NTFS).
3. Verify empty root directory.
4. Insert in instrument before power-on.
5. Verify startup message: "SD: logging to /pwrmtr_00000.csv"

---

## 9.2 USB SERIAL CSV STREAMING

9.2.1  The USB serial port streams CSV data at 115200 baud (SERIAL_BAUD). The CSV format is identical to the SD card format. This stream begins immediately at power-on and continues regardless of SD logging state.

9.2.2  To capture USB serial data:

```bash
# Linux/macOS:
screen /dev/ttyUSB0 115200
# or
minicom -D /dev/ttyUSB0 -b 115200 -C capture.csv

# Python (cross-platform):
python3 -m serial.tools.miniterm /dev/ttyUSB0 115200 | tee capture.csv
```

---

## 9.3 BLUETOOTH JSON STREAMING

9.3.1  When bt_streaming is enabled, the meter transmits compact JSON every BT_LOG_INTERVAL_MS (500 ms):

```json
{"f":14.200,"fw":47.321,"re":3.682,"pk":98.445,"sw":1.524}
```

Fields: `f`=freq_mhz, `fw`=fwd_w, `re`=ref_w, `pk`=peak_w, `sw`=swr.

9.3.2  The CYD display firmware subscribes to this stream for its live display. Any Bluetooth SPP client (phone app, Raspberry Pi script, laptop) can also receive this stream.

---

## 9.4 DATA ANALYSIS

### 9.4.1 Analysis with Python

9.4.1.1  Basic analysis of a log file:

```python
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('pwrmtr_00001.csv')
df['timestamp_s'] = df['timestamp_ms'] / 1000.0

# Forward power vs. time
plt.figure(figsize=(12, 4))
plt.plot(df['timestamp_s'], df['fwd_w'], label='Forward W')
plt.plot(df['timestamp_s'], df['ref_w'], label='Reflected W')
plt.xlabel('Time (s)')
plt.ylabel('Power (W)')
plt.legend()
plt.grid(True)
plt.title('RF Power Log')
plt.savefig('power_log.png')

# SWR histogram
plt.figure()
df['swr'].hist(bins=50)
plt.xlabel('SWR')
plt.ylabel('Count')
plt.title('SWR Distribution')
plt.savefig('swr_hist.png')

print(f"Max FWD: {df['fwd_w'].max():.1f} W")
print(f"Avg SWR: {df['swr'].mean():.3f}")
print(f"Max SWR: {df['swr'].max():.2f}")
print(f"Efficiency: {(1 - df['ref_w']/df['fwd_w']).mean()*100:.1f}%")
```

### 9.4.2 Frequency Sweep Analysis

9.4.2.1  Sweep data format (from SweepLogger):

```
freq_mhz,fwd_w,ref_w,swr
1.800,45.321,0.124,1.053
1.900,45.289,0.156,1.066
...
30.000,44.912,8.923,2.130
SWEEP_END
```

9.4.2.2  Plot SWR vs. frequency:

```python
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('sweep_01234.csv')
plt.figure(figsize=(10, 4))
plt.plot(df['freq_mhz'], df['swr'])
plt.axhline(1.5, color='orange', linestyle='--', label='SWR 1.5')
plt.axhline(2.0, color='red', linestyle='--', label='SWR 2.0')
plt.xlabel('Frequency (MHz)')
plt.ylabel('SWR')
plt.title('Antenna SWR vs. Frequency')
plt.legend()
plt.grid(True)
plt.ylim(1.0, 4.0)
plt.savefig('sweep_swr.png')
```

---

# CHAPTER 10 — TROUBLESHOOTING

## 10.1 DIAGNOSTIC APPROACH

10.1.1  Use the systematic fault isolation approach: establish symptoms, confirm expected behavior, isolate the fault to a module, then to a component. Use the Bluetooth STATUS command to read internal state without opening the instrument.

```
Table 10-1. Diagnostic Data from STATUS Command
──────────────────────────────────────────────────
Field       Normal Value    Fault Indication
──────────────────────────────────────────────────
fwd_w       > POWER_MIN_W   = 0 with RF applied
ref_w       < fwd_w         > fwd_w (impossible — diode fault)
swr         ≥ 1.0           > 10.0 constant
temp_c      10–55°C         < 0 or > 80°C
sd_log      true/false      (status only)
──────────────────────────────────────────────────
```

---

## 10.2 SYMPTOM TABLE

```
Table 10-2. Troubleshooting Guide
──────────────────────────────────────────────────────────────────────────────────
Symptom                    Probable Cause           Corrective Action
──────────────────────────────────────────────────────────────────────────────────
No display at power-on     No 5V power              Check USB-C cable/supply
                           Display not seated       Reseat display ribbon cable
                           Firmware not flashed     Reflash via Arduino IDE

Forward reads zero         Open detector diode      Check D1 continuity
with RF applied            Zero cal too high        Re-run CALZERO with no RF
                           Connector not mated      Check PL-259 connections
                           Wrong range selected     Set range to 0 (1-10W) for test

Reflected reads high       FWD/REF swapped          Swap connector polarity
with matched load          Bad detector D2          Replace HSMS-2822/1N5711
                           Coupler poor directivity Verify toroid termination R

SWR stuck at 1.00          Reflected ADC open       Check A1 wiring to ADS1115
                           Zero cal error           Re-run CALZERO

SWR reads infinite         Forward ADC open         Check A0 wiring to ADS1115
(div-by-zero)              Detector D1 open         Replace D1

Reading oscillates         RF on during cal         Remove RF before CALZERO
(hunting)                  Auto-range hunting       Check hysteresis thresholds
                           BT interference          Move BT antenna away from PCB

SD card not found          Wrong format             Reformat as FAT32
                           Card not inserted        Verify orientation
                           CS pin conflict          Check SD_CS_PIN = 5

Bluetooth not pairing      Wrong PIN (HC-05)        Use PIN 1234
                           Already connected        Disconnect other client
                           BT_DEVICE_NAME mismatch  Check CYD BT master name

High reading at cold       Cal at wrong temp        Re-calibrate at operating temp
Low reading when hot       Alpha too low            Adjust alpha in firmware

Temp reads 25°C constant   NTC open/shorted         Check NTC resistance
                           ADC A3 disconnected      Check wiring

Overload relay stuck       Range > 3 at low power   Cycle power, re-home range
                           Relay coil shorted       Measure relay coil resistance

Display too bright (CYD)   TFT_BL PWM not set       Adjust backlight PWM duty
                           (night operation)        Add BL_NIGHT_DUTY constant
──────────────────────────────────────────────────────────────────────────────────
```

---

## 10.3 TEST PROCEDURES

### 10.3.1 ADC Functionality Test

**Procedure 10-1: ADS1115 ADC Test**

1. Remove all RF. Terminate all RF ports with 50 Ω.
2. Connect Bluetooth terminal.
3. Send: `STATUS`
4. Note all four voltage readings in JSON response.
5. Expected values with no RF, calibrated:
   - `vfwd_mv` ≤ 25 mV (diode offset only)
   - `vref_mv` ≤ 25 mV
   - `vpeak_mv` ≤ 25 mV
   - `temp_c` = ambient temperature ±3°C
6. If any ADC value is stuck at 0 or 4096 counts (0 mV or 6144 mV): check I²C connection to ADS1115 (SDA=21, SCL=22), check I²C pullup resistors (4.7 kΩ to 3.3V).

### 10.3.2 Detector Diode Test

**Procedure 10-2: Detector Diode Continuity Test**

1. Power off instrument.
2. Disconnect coaxial cables.
3. Using DMM in diode mode:
   - Probe D1 (forward detector): expect 0.15–0.25 V forward, OL reverse (Schottky)
   - Probe D2 (reflected detector): same
   - Probe BAT54 peak detector: 0.20–0.30 V forward
4. If any diode reads OL in both directions: diode is open (replace)
5. If any diode reads short (< 0.05 V) in both directions: diode is shorted (replace)

### 10.3.3 Relay Switching Test

**Procedure 10-3: Range Relay Test**

1. Send: `RANGE 0` (should click relay to 0 dB position)
2. Send: `RANGE 1` (relay 1 should energize — audible click)
3. Send: `RANGE 2` (relay 2 should also energize)
4. Measure resistance between RF_IN and RF_OUT center pins:
   - Range 0: ~0.1 Ω (straight through)
   - Range 1: Pi-pad insertion loss path
   - Range 2: two Pi-pad sections
5. If relay does not click: check GPIO25, GPIO26 drive; check relay coil voltage (should see 3.3V–5V depending on driver)

---

# APPENDIX A — SPECIFICATIONS

```
Table A-1. Complete Instrument Specifications
══════════════════════════════════════════════════════════════════════════════
PARAMETER                           HF              VHF             UHF
══════════════════════════════════════════════════════════════════════════════
FREQUENCY COVERAGE
  Operating range                   1.8–30 MHz      50–148 MHz      420–1300 MHz
  Calibration accuracy              ±0.5 MHz        ±0.5 MHz        ±1 MHz

POWER MEASUREMENT
  Full-scale ranges                 10/100/1000/3000 W  same            10/100/1000 W
  Minimum readable power            10 mW           10 mW           10 mW
  Accuracy (calibrated, 25°C)       ±5%             ±7%             ±10%
  Accuracy (uncalibrated)           ±15%            ±15%            ±20%
  Resolution (ADC-limited)          0.1 mW          0.1 mW          0.1 mW
  Peak hold decay                   −63%/second     same            same

SWR MEASUREMENT
  SWR range                         1.0–10.0        1.0–10.0        1.0–10.0
  Accuracy (SWR ≤ 3.0)              ±0.10           ±0.15           ±0.20
  Accuracy (SWR > 3.0)              ±0.30           ±0.40           ±0.50
  SWR display resolution            0.01            0.01            0.01

COUPLER PERFORMANCE
  Coupling factor                   −30 dB ±1       −26 dB ±1       −25 dB ±1.5
  Directivity                       ≥35 dB          ≥26 dB          ≥22 dB
  Insertion loss                    ≤0.5 dB         ≤0.7 dB         ≤1.0 dB
  Input VSWR                        ≤1.05           ≤1.05           ≤1.10
  Impedance                         50 Ω            50 Ω            50 Ω

TEMPERATURE
  Compensation                      NTC + firmware  NTC + firmware  NTC + firmware
  Accuracy with compensation        ±0.5 dB         ±0.5 dB         ±0.7 dB
  Operating range                   −10 to +55°C    −10 to +50°C    0 to +45°C

DIGITAL INTERFACE
  Display                           OLED 128×64 or CYD 320×240
  ADC                               ADS1115 16-bit, ±6.144V, 860 SPS
  Microcontroller                   ESP32-WROOM-32
  Bluetooth                         SPP (BT 2.0), "RF-PowerMeter"
  USB serial                        115200 baud, CSV
  SD card                           FAT32, SPI, auto-roll at 10 MB
  Update rate                       10 Hz (CYD), 5 Hz (OLED)

POWER SUPPLY
  Input                             USB-C, 5V nominal, 4.5–5.5V range
  Current (operating)               350 mA typical, 500 mA peak
  Current (with CYD backlight)      600 mA typical

MECHANICAL
  HF/VHF enclosure                  130×90×50 mm PETG
  UHF enclosure                     100×75×38 mm PETG
  Terminating enclosure             180×130×65 mm with heatsink fins
  Weight (HF inline, no load)       approximately 320 g
  Connectors                        SO-239 (HF/VHF), N-type (UHF)
══════════════════════════════════════════════════════════════════════════════
```

---

# APPENDIX B — BILL OF MATERIALS

```
Table B-1. Inline Directional Meter BOM (HF Variant)
═══════════════════════════════════════════════════════════════════════════════
RefDes   Qty  Description                    Part Number         Value/Note
═══════════════════════════════════════════════════════════════════════════════
HF COUPLER
T1       1    Toroid core FT-114-43          Fair-Rite 5943000401 Mix 43
         1    Primary: 1T primary            Hookup wire, 14AWG
         1    Secondary: 32T enameled wire   28AWG magnet wire

DETECTORS
D1       1    Schottky detector, forward     1N5711 or BAV70    DO-35
D2       1    Schottky detector, reflected   1N5711 or BAV70    DO-35
D3       1    Peak detector                  BAT54              SOT-23
D4,D5    2    Overload clamp                 BAT54 (B-B)        SOT-23

PROTECTION
TVS1     1    TVS clamp                      SMAJ5.0A           DO-214AC

PASSIVE — RF SECTION
R1,R2    2    Coupler termination            51 Ω, 1%, 0805     51R
R3       1    Peak reset drain               1 MΩ, 0805         1M
R4       1    Forward detector load          10 kΩ, 0805        10K
R5       1    Reflected detector load        10 kΩ, 0805        10K
R6       1    Overload series R              47 Ω, 0805         47R
C1,C2    2    Detector bypass                10 nF, 50V, X7R    10nF
C3       1    Peak hold capacitor            1 µF, 25V, X5R     1uF
C4,C5    2    Coupler bypass                 100 pF, 0805       100p

PASSIVE — NTC
R7       1    NTC bias resistor              10 kΩ, 1%, 0805    10K
RT1      1    NTC thermistor                 10 kΩ, B3950       TH

RANGE SWITCHING
RLY1,2   2    Relay, SPDT, 5V coil          HF3FF-5-L2         5V
Q1,Q2    2    Relay driver                  2N7002             SOT-23
D6,D7    2    Flyback diode                 1N4148             SOT-23
Pi-pads  2    Pi-pad attenuator (20 dB each) See coupler design worksheet

PEAK RESET
Q3       1    MOSFET, N-ch, reset            2N7000             TO-92
R8       1    Gate resistor                  1 kΩ, 0805         1K

ADC
U1       1    16-bit I2C ADC                ADS1115IDGSR       MSOP-10
C6–C9    4    ADC supply bypass             100 nF, X7R        100n

MICROCONTROLLER
U2       1    ESP32 module                  ESP32-WROOM-32     38-pin
U3       1    LDO regulator 3.3V            AMS1117-3.3        SOT-223
C10      1    LDO input cap                 10 µF, 10V         10u
C11      1    LDO output cap                10 µF, 10V         10u
J1       1    USB-C power connector         GCT USB4135        
SW1–SW4  4    Tactile switch                EVQPUA02           SMD
LED1     1    Status LED                    Green, 0805        
R9       1    LED current limit             330 Ω, 0805        330R

DATA LOGGING
U4       1    MicroSD card socket           Molex 5034980892   
C12      1    SD supply bypass              100 nF             100n

DISPLAY
OLED1    1    OLED, 128×64, I2C (or CYD)    SSD1306            0.96"

CONNECTORS
J2,J3    2    SO-239 panel mount            Amphenol 82-5051   5/8-24

PCB
PCB1     1    PCB, 2-layer, 100×80mm, HASL  Per KiCad files    1.6mm FR4
═══════════════════════════════════════════════════════════════════════════════
```

```
Table B-2. Terminating Meter Additional Components (Type A ≤100 W)
──────────────────────────────────────────────────────────────────────────────
RefDes   Qty  Description                    Part Number         Value/Note
──────────────────────────────────────────────────────────────────────────────
R_load   8    Cement resistor, 390 Ω, 25W   Arcol HL25         390R, ±5%
HS1      1    Heatsink                       Wakefield 394K     0.6°C/W
TG1      1    Thermal compound               Shin-Etsu X-23     ≥6 W/m·K
U_DS18   1    Temperature sensor             DS18B20            TO-92 OneWire
──────────────────────────────────────────────────────────────────────────────
```

---

# APPENDIX C — COUPLER DESIGN WORKSHEET

```
═══════════════════════════════════════════════════════════════════════
DIRECTIONAL COUPLER DESIGN WORKSHEET
═══════════════════════════════════════════════════════════════════════

Project: ___________________  Date: ___________  Band: HF / VHF / UHF
Designer: _________________  Core: ____________________________

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SECTION 1: COUPLING REQUIREMENTS
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Max forward power (P_max):            _________ W
Full-scale meter current (I_FS):      _________ µA
System impedance (Z₀):                _________ Ω  (nominal 50)

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SECTION 2: TURNS RATIO (Toroid/Binocular)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Primary turns N₁:                     _________
Secondary turns N₂:                   _________
Coupling C = −20×log₁₀(N₁/N₂):       _________ dB

Verify: C should be −20 to −45 dB for typical meter elements.

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SECTION 3: DETECTOR VOLTAGE AT FULL SCALE
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
V_FS = √(P_max × Z₀) × 10^(C/20):

  √(P_max × Z₀) = √(___ × ___) = _________ V
  10^(C/20) = 10^(___/20)      = _________
  V_FS = ___________ × _________ = _________ V

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SECTION 4: METER SHUNT RESISTOR
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
R_FS = V_FS / I_FS = _________ / _________ = _________ Ω

Select standard value: _________ Ω
Actual V_FS with selected R: _________ V
Percent error: _________ %  (acceptable if < 5%)

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SECTION 5: SECONDARY INDUCTANCE CHECK
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Core AL value:                         _________ nH/T²
L_sec = AL × N₂²:                     _________ µH
X_L at f_min = 2π × f_min × L_sec:   _________ Ω
Requirement: X_L >> 51 Ω (10× minimum)
X_L / 51 Ω = _________   PASS / FAIL

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SECTION 6: UHF MICROSTRIP DIMENSIONS (if UHF)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Substrate: ____________  εr: _______  h: _______ mm
Center frequency f₀:               _________ MHz
λ/4 in substrate:
  c / (4 × f₀ × √εr_eff) = _________ mm
  εr_eff ≈ (εr+1)/2 + (εr−1)/2 × [1+12h/W]^(−0.5) ≈ _______
  L_coupler = _________ mm

Main line width W_main (50Ω):       _________ mm
Coupled line width W_cpld (50Ω):    _________ mm
Gap S (for desired C):              _________ mm

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SECTION 7: SIGN-OFF
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Design verified by: ___________________  Date: ___________
Prototype built and measured: YES / NO
Measured coupling at f₀: _______ dB  (target: _______ dB)
Measured directivity: _______ dB     (minimum: 20 dB)
═══════════════════════════════════════════════════════════════════════
```

---

# APPENDIX D — ELEMENT COLOR CODE AND SELECTION GUIDE

```
Table D-1. Plug-in Element Color Code (Thruline Meter)
══════════════════════════════════════════════════════════════════════════
Color    Element    Band    Freq (MHz)   Max W    Core     Coupling
══════════════════════════════════════════════════════════════════════════
Green    HF-10      HF      1.8–30       10       FT-114   −30 dB, 32T
Yellow   HF-100     HF      1.8–30       100      FT-114   −20 dB, 10T
Orange   HF-1K      HF      1.8–30       1000     FT-114   −40 dB, 100T
Red      HF-3K      HF      1.8–30       3000     FT-114   −45 dB, 178T
Blue     VHF-10     VHF     50–148       10       BN-43    −26 dB, 2T/20T
Purple   VHF-100    VHF     50–148       100      BN-43    −20 dB, 1T/10T
White    UHF-10     UHF     420–1300     10       PCB      −25 dB, cpld.
Black    UHF-100    UHF     420–1300     100      PCB      −19 dB, cpld.
══════════════════════════════════════════════════════════════════════════
```

**Element Selection Flowchart:**

```
START: What band are you measuring?
  │
  ├── HF (1.8–30 MHz) → What is expected peak power?
  │       ├── ≤ 10 W   → GREEN  (HF-10)
  │       ├── ≤ 100 W  → YELLOW (HF-100)
  │       ├── ≤ 1 kW   → ORANGE (HF-1K)
  │       └── ≤ 3 kW   → RED    (HF-3K)
  │
  ├── VHF (50–148 MHz) → What is expected peak power?
  │       ├── ≤ 10 W   → BLUE   (VHF-10)
  │       └── ≤ 100 W  → PURPLE (VHF-100)
  │
  └── UHF (420–1300 MHz) → What is expected peak power?
          ├── ≤ 10 W   → WHITE  (UHF-10)
          └── ≤ 100 W  → BLACK  (UHF-100)

RULE: Always use the lowest-range element that covers your power level.
      Never apply power exceeding element rating. Use next higher element
      if power is uncertain.
```

---

```
═══════════════════════════════════════════════════════════════════════
END OF TM-PWR-001 Rev A
RF POWER METER SYSTEM TECHNICAL MANUAL

Document Number:  TM-PWR-001
Revision:         A
Date:             2026-05-01

All firmware source files referenced in this manual are located at:
  /home/merv/Public/claude/ant_gear/rf_power_meters/firmware/

KiCad PCB files:
  /home/merv/Public/claude/ant_gear/rf_power_meters/

OpenSCAD enclosures:
  /home/merv/Public/claude/ant_gear/rf_power_meters/enclosures/

ASCII schematics:
  /home/merv/Public/claude/ant_gear/rf_power_meters/schematics/
═══════════════════════════════════════════════════════════════════════
```
