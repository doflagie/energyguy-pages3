# TECHNICAL MANUAL
## GPS-DISCIPLINED FIELD CALIBRATION SYSTEM
### TM-CAL-001 Rev A

---

**Equipment:** GPS-Disciplined Oscillator, Frequency Counter, Signal/Level Reference, SOLT Kit  
**Application:** Field calibration of NanoVNA, TinySA, RTL-SDR, and RF test instruments  
**Classification:** UNCLASSIFIED — Amateur Radio / Field Use  
**Prepared for:** Field Calibration of Amateur Radio Test Equipment

---

## TABLE OF CONTENTS

1. [Introduction and System Overview](#1-introduction-and-system-overview)
2. [Theory of Operation — GPS Discipline](#2-theory-of-operation--gps-discipline)
3. [Theory of Operation — Frequency Measurement](#3-theory-of-operation--frequency-measurement)
4. [Theory of Operation — Amplitude Calibration](#4-theory-of-operation--amplitude-calibration)
5. [System Hardware Description](#5-system-hardware-description)
6. [Construction and Assembly](#6-construction-and-assembly)
7. [Initial Setup and Power-On](#7-initial-setup-and-power-on)
8. [GPS Lock Procedure](#8-gps-lock-procedure)
9. [NanoVNA Calibration Procedures](#9-nanovna-calibration-procedures)
10. [TinySA Calibration Procedures](#10-tinysaa-calibration-procedures)
11. [RTL-SDR Frequency Correction](#11-rtl-sdr-frequency-correction)
12. [SDR Interfaces Preselector Calibration](#12-sdr-interfaces-preselector-calibration)
13. [Frequency Counter Operation](#13-frequency-counter-operation)
14. [Calibration Record Keeping](#14-calibration-record-keeping)
15. [Web Interface Operation](#15-web-interface-operation)
16. [CYD Display Operation](#16-cyd-display-operation)
17. [Maintenance and Troubleshooting](#17-maintenance-and-troubleshooting)
18. [Appendix A: Calibration Quick Reference Card](#appendix-a-calibration-quick-reference-card)
19. [Appendix B: Calibration Log Template](#appendix-b-calibration-log-template)
20. [Appendix C: Frequency Error Table](#appendix-c-frequency-error-table)
21. [Appendix D: Parts List and Sources](#appendix-d-parts-list-and-sources)

---

## 1. INTRODUCTION AND SYSTEM OVERVIEW

### 1.1 Purpose

This manual describes the construction, operation, and maintenance of the GPS-Disciplined Field Calibration System. The system provides GPS-referenced frequency and amplitude standards for calibrating RF test instruments used in portable and field operations including antenna analysis, spectrum monitoring, and SDR reception.

The system addresses a common problem in amateur field operations: instruments drift with temperature and age, introducing measurement errors that invalidate antenna comparisons, filter alignments, and signal level assessments. Without a reference, a NanoVNA reading of "−23 dBm" at 14.225 MHz has unknown accuracy; with this calibration system, the accuracy is traceable to GPS atomic standards.

### 1.2 System Architecture

The calibration system consists of five functional elements:

```
┌─────────────────────────────────────────────────────────────────────────┐
│                     GPS CALIBRATION STATION                              │
│                                                                          │
│  ┌──────────────┐    ┌──────────────┐    ┌──────────────────────────┐  │
│  │ NEO-M8N GPS  │    │   Si5351A    │    │     ESP32 Controller     │  │
│  │              │    │ Clock Synth  │    │                          │  │
│  │ 1PPS output  │───►│ GPS-discipl. │◄──►│ Discipline loop          │  │
│  │ NMEA data    │───►│ 10 MHz CLK0  │    │ Frequency counter PCNT   │  │
│  │              │    │  1 MHz CLK1  │    │ Web server 192.168.4.1   │  │
│  └──────────────┘    │ prog  CLK2   │    │ BT → CYD display         │  │
│                      └──────┬───────┘    │ NVS save/restore         │  │
│                             │            └──────────────────────────┘  │
│                    ┌────────▼───────┐                                   │
│                    │  74AC04 Buffers│                                   │
│                    └────────┬───────┘                                   │
│                             │                                           │
│        SMA: 10M REF, 1M REF, PROG OUT, CAL LVL (−20 dBm)             │
│        BNC: FREQ IN (counter)   SMA: GPS ANT   USB: power/prog         │
└─────────────────────────────────────────────────────────────────────────┘

           +
┌────────────────────────────────────────────────────────────────────────┐
│                         SOLT KIT CASE                                   │
│  SHORT, OPEN, LOAD, THRU standards  +  6 attenuator pads  +  cables   │
└────────────────────────────────────────────────────────────────────────┘
```

### 1.3 Performance Summary

| Parameter | Specification | Notes |
|-----------|--------------|-------|
| Freq. reference accuracy | <1 ppb (GPS locked) | After 5 min lock |
| Freq. counter resolution | 1 Hz (1s gate) / 0.1 Hz (10s) | Direct mode |
| Freq. counter range | 1 Hz – 400 MHz | Direct + ÷10 |
| Signal source freq. range | 8 kHz – 160 MHz | Si5351 CLK2 |
| Level reference accuracy | ±1 dB (absolute) | AD8307 calibrated |
| Level reference range | −60 to 0 dBm | With attenuator pads |
| Lock acquisition time | 2–5 minutes | Open sky, >6 SVs |
| GPS receiver | u-blox NEO-M8N | Multi-constellation |
| 1PPS accuracy | ±10 ns RMS | Open sky |

### 1.4 Related Documents

- TM-CAL-SCH-001: GPSDO Schematic
- TM-CAL-SCH-002: Frequency Counter Schematic
- TM-CAL-SCH-003: Signal Reference Schematic
- TM-CAL-SCH-004: Power Reference Schematic
- TM-CAL-SCH-005: System Distribution and Integration
- TM-SDR-001: SDR Interfaces and Preselector (TM-RFI companion)
- TM-RFI-001: RFI Mitigation Manual

---

## 2. THEORY OF OPERATION — GPS DISCIPLINE

### 2.1 GPS Timing Accuracy

The Global Positioning System (GPS) is fundamentally a precision timing system. Each GPS satellite broadcasts timing signals derived from onboard cesium and rubidium atomic frequency standards, synchronized to Universal Coordinated Time (UTC) by the USAF 2nd Space Operations Squadron.

The GPS receiver (NEO-M8N) extracts timing from multiple satellite signals and outputs a 1 Pulse Per Second (1PPS) signal synchronized to UTC. Accuracy of the 1PPS signal:

- **Single-satellite solution:** ±100 ns typical
- **Multi-satellite, 3D fix (>6 SVs):** ±10–20 ns RMS
- **Over 100-second average:** approaches GPS system accuracy (~1 ns)

For frequency calibration purposes, what matters is the **frequency accuracy** of the 1PPS signal, not its absolute phase alignment to UTC. The frequency accuracy is:

```
f_accuracy = 1 / T_gate × timing_error
```

For a 10-second gate with ±10 ns timing error:
```
f_accuracy = 1 / 10 × 10e-9 = 1e-9 = 1 ppb
```

This is more than adequate for calibrating amateur instruments, which typically have initial accuracies of ±5 to ±50 ppm.

### 2.2 Phase-Locked Loop Discipline

The Si5351A clock synthesizer generates its output frequencies from an internal 25 MHz crystal reference. Left undisciplined, this crystal has ±5 ppm accuracy (50 Hz at 10 MHz) and drifts with temperature and age.

The GPS discipline loop corrects the Si5351A output by measuring its frequency against the GPS 1PPS gate and applying corrections to the Si5351's internal calibration register:

```
Discipline Loop:
                    GPS 1PPS
                       │
               ┌───────▼────────────┐
               │  ESP32 Gate Timer  │
               │  Count Si5351 CLK0 │
               │  for N seconds     │
               └───────────┬────────┘
                           │ count
               ┌───────────▼────────┐
               │  Error Calculator  │
               │  err = count - N×f │
               └───────────┬────────┘
                           │ ppb error
               ┌───────────▼────────┐
               │  PI Controller     │
               │  corr += err × Kp  │
               └───────────┬────────┘
                           │ correction value
               ┌───────────▼────────┐
               │  Si5351 set_corr() │
               │  (0.01 ppb units)  │
               └────────────────────┘
```

**Discipline loop parameters:**
- Gate time: 10 seconds (100 million counts at 10 MHz)
- Resolution: 1 count = 0.1 ppb error
- Proportional gain Kp = 0.5 (prevents overshoot)
- Lock threshold: |error| < 1.0 ppb for 3 consecutive 10-second windows

### 2.3 Frequency Error and Correction Math

If the Si5351 CLK0 runs at 10,000,000 Hz exactly, and the GPS gate is exactly 10 seconds, the counter should read exactly 100,000,000. Any deviation represents error:

```
error_counts = measured_count − 100,000,000

error_ppb = error_counts × 10 / GATE_SECONDS
          = error_counts × 10 / 10
          = error_counts (in ppb, with 10s gate)

(One count high in 100 million = 10 ppb × 1/10 gates = 1 ppb)
```

The Si5351 correction register accepts values in units of 0.01 ppb. To apply 1 ppb correction, write register value 100:

```cpp
si5351.set_correction(correction_reg, SI5351_PLL_INPUT_XO);
// where correction_reg is in 0.01 ppb units
```

### 2.4 Time to Lock

Lock acquisition depends on:

1. **GPS fix time** (Time To First Fix, TTFF): 30–90 seconds with cold start; <10 s with warm start (backup battery maintaining almanac).
2. **Discipline convergence:** Typically 5–10 correction iterations = 50–100 seconds.
3. **Total to LOCK LED solid:** 2–5 minutes typical.

For optimal results, allow 10 minutes of warm-up before commencing calibration. In field conditions with partial sky view, allow 15 minutes.

---

## 3. THEORY OF OPERATION — FREQUENCY MEASUREMENT

### 3.1 Reciprocal Counting Method

The frequency counter uses the simplest and most accurate method: direct pulse counting over a GPS-gated time window.

```
Principle:
  f_measured = N_counts / T_gate

where:
  N_counts = pulses counted from DUT oscillator
  T_gate   = gate time (exactly N × 1 PPS interval)
```

The gate time is defined by GPS 1PPS intervals, making it as accurate as GPS. There is no oscillator in the counter that needs calibration — accuracy is determined entirely by:

1. The GPS 1PPS accuracy (±10 ns RMS → 1 ppb at 10s gate)
2. The counter quantization error (±1 count → ±1 Hz at 1s gate)

**Resolution vs. gate time:**
| Gate | Resolution | Example at 10 MHz |
|------|-----------|-------------------|
| 1 s  | 1 Hz      | 10.000,001 MHz    |
| 10 s | 0.1 Hz    | 10.000,000.1 MHz  |

### 3.2 Frequency Ranges and Prescaler

The ESP32 PCNT (Pulse Counter) peripheral is rated to approximately 40 MHz maximum count rate. For higher frequencies, a prescaler divides the input:

```
Direct (÷1):   1 Hz – 40 MHz    GPIO32 ← input directly
÷10 mode:      10 Hz – 400 MHz  GPIO32 ← 74HC390 ÷10 output
                                (multiply result × 10)
```

74HC390 is a dual decade counter IC that provides ÷10 (two stages: ÷2 then ÷5). Maximum clock frequency of 74HC390: 60 MHz at 3.3V, 100 MHz at 5V. For signals above 100 MHz, use 74AC390 (100+ MHz rated) or purchase an external prescaler module.

### 3.3 ESP32 PCNT Peripheral

The ESP32 contains a dedicated Pulse Counter (PCNT) peripheral with hardware overflow detection. This allows continuous counting without CPU interrupts for each pulse:

- 16-bit counter (range ±32767)
- Threshold interrupt at ±32767 (overflow)
- Overflow ISR increments 32-bit software accumulator
- Total count = (overflow_count × 32768) + current_pcnt_value
- Maximum accumulation: 2^48 total (with 32-bit overflow count)

At 40 MHz for 10 seconds: 400 million counts — well within 32-bit range (4.29 billion).

---

## 4. THEORY OF OPERATION — AMPLITUDE CALIBRATION

### 4.1 RF Power Measurement with AD8307

The AD8307 is a logarithmic amplifier/detector with a 92 dB dynamic range and output voltage proportional to input power in dBm:

```
Transfer function:
  V_out (mV) = Slope × (P_in_dBm − Intercept_dBm) + V_offset
  V_out (mV) = 25 × (P_in_dBm + 84)
  P_in_dBm   = (V_out_mV / 25) − 84
```

**Key AD8307 characteristics:**
| Parameter | Value |
|-----------|-------|
| Slope | 25 mV/dB |
| Intercept | −84 dBm (0V output) |
| Dynamic range | −75 to +17 dBm |
| Frequency response | DC to 500 MHz |
| Temperature coefficient | ±0.01 dB/°C (slope) |

### 4.2 Calibrated Level Traceability

The power level calibration chain:

```
GPS (frequency reference, 0.01 ppb)
        ↓
Si5351 CLK2 (known frequency, GPS-disciplined)
        ↓
74AC04 buffer output (known impedance: 50Ω + 33Ω series)
        ↓
AD8307 measures actual output level (corrected to NanoVNA measurement)
        ↓
Fixed precision attenuator (0.1% resistors, ±0.2 dB)
        ↓
Calibrated level reference (±0.5 dB absolute, ±0.2 dB relative)
```

The absolute level accuracy (±1 dB) is limited by:
- Si5351 output drive variation with frequency: ±2 dB
- 74AC04 output swing variation: ±1 dB
- AD8307 calibration offset uncertainty: ±0.5 dB

For ±0.5 dB absolute accuracy: calibrate the AD8307 zero-point by measuring the "CAL LVL" output with a NanoVNA CH0 S11 measurement (NanoVNA is calibrated via SOLT, giving ≈ ±0.3 dB S21 accuracy). Record the offset in NVS.

### 4.3 Precision Attenuator Design

Resistive PI attenuators provide accurate, temperature-stable attenuation because:

1. Attenuation ratio depends only on resistor ratios (ratiometric)
2. Temperature coefficients cancel (all same material)
3. No reactive components (valid from DC to >1 GHz)
4. Exact values: calculable from Z0 and dB specification

**PI attenuator formulas:**
```
Given: Z0 = 50Ω, attenuation L_dB
K = 10^(L_dB / 20)    (voltage ratio)

R_series = Z0 × (K² − 1) / (2K)
R_shunt  = Z0 × (K + 1) / (K − 1)
```

With 0.1% tolerance resistors:
- Calculated values within ±0.2 dB of design value
- Insertion loss measurement accuracy ±0.1 dB with calibrated VNA

---

## 5. SYSTEM HARDWARE DESCRIPTION

### 5.1 GPS Module — u-blox NEO-M8N

The NEO-M8N is a multi-constellation GPS/GLONASS/Galileo/BeiDou receiver module.

**Key specifications:**
- Tracking: GPS L1, GLONASS L1, Galileo E1, BeiDou B1 (72 channels)
- Time pulse: configurable; default 1 PPS aligned to UTC rising edge
- Time pulse accuracy: ±10 ns RMS (open sky, >6 SVs tracked)
- Supply: 3.3V, 30 mA acquisition, 15 mA tracking
- Interface: UART (9600 baud default, configurable to 115200)
- Backup battery: CR1220 maintains almanac/ephemeris (10-30 second TTFF with warm start)

The module includes an active patch antenna or can connect to an external active GPS antenna via SMA coax. For field operations, place the antenna with clear sky view in all directions above 10° elevation.

### 5.2 Clock Synthesizer — Si5351A

The Si5351A is a programmable clock synthesizer with three independent outputs, each configurable from 8 kHz to 160 MHz.

**Internal architecture:**
```
25 MHz XTAL/TCXO
       ↓
  [VCO: PLL A]  ← Phase comparator + loop filter
       ↓        (multiply to 600–900 MHz)
  [Output Divider A] → CLK0 (10 MHz reference)
  [Output Divider B] → CLK1 (1 MHz reference)

  [VCO: PLL B]
       ↓
  [Output Divider C] → CLK2 (programmable)
```

Each output is independently divisible. Phase noise at 10 MHz:
−90 dBc/Hz at 100 Hz offset; −110 dBc/Hz at 1 kHz offset.

The correction register (`XTAL_CL` / frequency calibration) adjusts the effective reference frequency seen by the PLL. Resolution: 0.01 ppb per LSB of the 32-bit signed correction register.

### 5.3 Output Buffer — 74AC04

The 74AC04 hex inverter buffers Si5351 CMOS outputs for driving 50Ω SMA connectors. The 74AC logic family is required (not 74HC) because:

- 74AC04 maximum propagation delay: 5 ns → usable to 160 MHz
- 74HC04 maximum propagation delay: 15 ns → limited to ~40 MHz
- 74ACT04 accepts 5V CMOS input while running on 3.3V supply

Output level: ~3.3 Vpp CMOS → into 50Ω via 33Ω series resistor → ~0 dBm effective. This is suitable for driving test equipment inputs without damage.

### 5.4 Power Detector — AD8307

The AD8307 logarithmic amplifier provides real-time monitoring of the PROG OUT signal level. The ESP32 ADC reads the AD8307 output every 500 ms and displays the result in dBm on the web interface and CYD display.

The AD8307 input is AC-coupled via 100 nF capacitor and terminated with 52.3Ω to GND (approximate 50Ω match; AD8307 input impedance ~1 kΩ shunts the termination to give ~52Ω × 1000/1052 ≈ 49Ω effective).

---

## 6. CONSTRUCTION AND ASSEMBLY

### 6.1 PCB Construction Options

**Option A: Perfboard/Veroboard construction**
- Mount components on 0.1" pitch perfboard
- Wire-wrap or solder-bridge connections
- Adequate for all components in this design
- Estimated assembly time: 4–6 hours

**Option B: Custom PCB**
- Use KiCad to draw schematic from TM-CAL-SCH-001 through 004
- Export Gerbers; order from JLCPCB, PCBWay, or OSHPark
- 2-layer, 1.6mm FR4, 1 oz copper
- Estimated cost: $15–25 for 5 PCBs shipped

**Option C: Module assembly (fastest)**
- Purchase Si5351A breakout module (Adafruit #2045 or clone)
- Purchase NEO-M8N module (AliExpress, with ceramic patch antenna)
- Mount ESP32 devboard
- Point-to-point wire between modules
- Estimated assembly time: 2 hours

### 6.2 Critical Construction Notes

**RF signal paths:**
- Keep SMA connector traces short (<15 mm from connector pin to first component)
- Si5351 CLK outputs to buffers: use 33Ω series resistor within 5 mm of Si5351 pin
- 74AC04 outputs to SMA connectors: use 33Ω series resistor within 5 mm of IC pin
- Ground plane under all RF traces: use copper tape if building on perfboard
- AD8307 input path: AC-couple with 100 nF NP0 (not X5R/X7R — these degrade at RF)

**Power supply decoupling:**
- 100 nF NP0 ceramic capacitor within 3 mm of every VCC pin
- Additional 10 µF tantalum on 3.3V rail (within 20 mm of ESP32 VCC)
- Separate decoupling for Si5351 and 74AC04 (don't share a single cap)

**GPS module:**
- NEO-M8N is sensitive to EMI from ESP32 switching regulators
- Place NEO-M8N at least 30 mm from ESP32 WROOM module
- Shield GPS module with thin copper tape (leave antenna area unshielded)
- TIMEPULSE output: 4.7 kΩ pull-up to 3.3V required; rise time >1V/µs needed for clean interrupt

### 6.3 Enclosure Assembly

1. Print enclosure halves in PETG (ABS also acceptable; avoid PLA — softens at field temperatures).
2. Apply copper adhesive tape to all interior surfaces (floor, walls, lid interior).
3. Connect copper tape panels together at corners with additional strips.
4. Install SMA panel connectors: tighten to 0.5 N·m (finger-tight + quarter turn with wrench).
5. Install BNC panel connector: tighten to 1.0 N·m.
6. Mount PCB on standoffs with M3 × 6mm screws.
7. Internal wiring: use RG-178 coax for all RF connections inside box; 50Ω impedance throughout.
8. Connect copper tape ground to SMA connector shells with short copper tape strip.
9. Close lid with M3 × 10mm screws (4 corners).

### 6.4 SOLT Standards Construction

#### 6.4.1 SHORT Standard

**Materials:** 1× SMA(M) connector (PCB edge launch or crimp type)

**Procedure:**
1. Strip center conductor of SMA(M). Do not attach to any cable.
2. Solder a small bridge of 28 AWG tinned copper wire from center pin to outer barrel.
3. Solder bridge should be as short as physically possible (< 2 mm) to minimize added inductance.
4. Verify with multimeter: continuity between center and outer, <0.1 Ω.
5. Mark with RED paint dot or red nail polish on connector body.

**Electrical characteristics:**
- S11 magnitude: 0 dB (total reflection, |Γ| = 1)
- S11 phase: 180° at reference plane (short circuit)
- Residual inductance: ~0.1 nH (negligible below 1 GHz)

#### 6.4.2 OPEN Standard

**Materials:** 1× SMA(M) connector

**Procedure:**
1. Leave center pin completely unconnected (open circuit).
2. Protect center pin from contamination: apply small dab of RTV silicone to form dome over pin. Allow to cure 24 hours. RTV is RF-transparent and prevents pin oxidation.
3. Mark with GREEN paint dot.

**Electrical characteristics:**
- S11 magnitude: 0 dB (total reflection, |Γ| = 1)
- S11 phase: 0° at reference plane (open circuit)
- Fringe capacitance: ~0.2 pF (equivalent length 3 mm at 10 GHz; negligible below 1 GHz)

#### 6.4.3 LOAD Standard (50Ω)

**Materials:** 1× SMA(M) connector, 2× 100Ω SMD 0402 resistors (0.1% tolerance)

**Procedure:**
1. Solder two 100Ω resistors in parallel between center pin and outer shell.
2. Use 0402 (1005 metric) resistors — smaller size reduces inductance.
3. Orient resistors symmetrically (180° apart around center pin if space allows).
4. Resistance: measure with precision ohmmeter between center and shell. Accept: 49.0–51.0 Ω.
5. Mark with BLUE paint dot.

**Electrical characteristics:**
- S11 magnitude: theoretically 0 (perfect match)
- Return loss: >34 dB at DC–300 MHz (limited by resistor parasitic inductance ~0.5 nH)
- Typical measured return loss: >40 dB at 100 MHz, >30 dB at 500 MHz

#### 6.4.4 THRU Standard

Use an SMA(M)-SMA(M) barrel adapter (purchased). Insertion loss: <0.5 dB at 3 GHz. No construction required. Mark with no dot (natural).

---

## 7. INITIAL SETUP AND POWER-ON

### 7.1 Pre-Power Checks

Before applying power for the first time:

- [ ] Verify 3.3V regulator output with multimeter (no load): 3.25–3.35V
- [ ] Verify no shorts between 3.3V and GND: resistance >10 kΩ (unpowered)
- [ ] Verify Si5351 I2C connection: SDA/SCL pulled up (measure ~3.3V at GPIO21, GPIO22)
- [ ] Verify GPS module VCC: 3.3V rail properly connected
- [ ] Verify all SMA connectors are not shorted internally (center to shell continuity >1 MΩ)

### 7.2 Firmware Installation

Install the Arduino IDE libraries before compiling:

1. **etherkit/si5351arduino** — Si5351 control: Library Manager → search "Si5351"
2. **TinyGPSPlus** — GPS NMEA parsing: Library Manager → search "TinyGPS++"
3. **ESP32 board package** — Tools → Board → Boards Manager → search "ESP32" → install Espressif Systems

Compile and upload `esp32_cal_master.ino`:
- Board: ESP32 Dev Module
- CPU Speed: 240 MHz
- Flash Size: 4MB (32Mb)
- Partition Scheme: Default 4MB with spiffs
- Upload speed: 921600

After upload, open Serial Monitor (115200 baud). Expected output:
```
[CAL] GPS Calibration Station starting
[CAL] Si5351 init OK
[CAL] WiFi AP: CAL-Station  BT: CAL-Master
```

### 7.3 Initial Si5351 Verification

After upload, with Serial Monitor open:

1. Measure Si5351 CLK0 output on "10M REF" SMA with frequency counter or oscilloscope.
2. Expected: 10 MHz ± 5 ppm (before GPS discipline).
3. Measure CLK1 "1M REF": 1 MHz ± 5 ppm.
4. If output is missing: check I2C connections; verify Si5351 address (default 0x60).
5. If frequency is far off (>50 ppm): XTAL may not be oscillating; check load capacitors.

---

## 8. GPS LOCK PROCEDURE

### 8.1 Initial Lock Acquisition

1. **Connect GPS antenna:** Attach active GPS patch antenna to SMA "GPS ANT" connector. Route coax to location with clear sky view.
2. **Power on:** Apply 5V to barrel jack or USB. Serial Monitor shows GPS sentences within 60 seconds.
3. **Observe GPS LED (green):**
   - Blinking rapidly (2 Hz): searching for satellites
   - Blinking slowly (1 Hz): satellites found, computing fix
   - Solid: 3D position fix, 1PPS active
4. **Observe LOCK LED (blue):**
   - Off: GPS not yet active or no fix
   - Blinking (1 Hz): discipline loop running, converging
   - Solid: locked, |error| < 1 ppb

**Expected timeline:**
```
T+0:   Power on
T+30s: GPS LED blinking, first NMEA sentences received
T+60s: GPS LED solid (3D fix, 1PPS active)
T+70s: LOCK LED starts blinking (discipline loop starts)
T+3min: LOCK LED solid (converged, <1 ppb error)
```

### 8.2 Verifying Lock

When the LOCK LED is solid, verify via web interface or BT:

**Web interface (192.168.4.1):**
- Connect phone/laptop to WiFi "CAL-Station" (password: gpsdo2024)
- Open browser to 192.168.4.1
- STATUS card shows: `LOCK: LOCKED`, `ERROR (ppb): <1.0`

**BT serial terminal:**
- Connect to "CAL-Master"
- Send: `STATUS`
- Response: `GPS:1,SATS:8,TIME:14:23:01,LOCK:1,ERR:0.43,CORR:2345,...`

### 8.3 Partial-Sky Operation

In field conditions (trees, buildings blocking sky), GPS may acquire fewer than 6 satellites, degrading 1PPS accuracy:

| Satellites | 1PPS Accuracy | Freq. Accuracy (10s gate) |
|-----------|--------------|--------------------------|
| ≥8 | ±10 ns | <1 ppb |
| 5–7 | ±20 ns | ~2 ppb |
| 3–4 | ±50 ns | ~5 ppb |
| <3 | No fix | N/A |

For calibration requiring better than 5 ppm accuracy, maintain at least 5 SVs in lock.

---

## 9. NANOVNA CALIBRATION PROCEDURES

### 9.1 Frequency Calibration

The NanoVNA's internal reference oscillator determines frequency accuracy. Most NanoVNA units are within ±10 ppm from factory; temperature drift can add ±5 ppm.

**Procedure:**

1. **Wait for GPS lock** (LOCK LED solid, >10 minutes warm-up recommended).
2. **Set up NanoVNA:** Power on, allow 5 minutes warm-up.
3. **Connect:** GPSDO "10M REF" → SMA cable → NanoVNA CH0 input.
4. **Configure NanoVNA sweep:** Center 10.000 MHz, Span 1 kHz.
5. **Read displayed frequency at marker:** The marker should show exactly 10.000 000 MHz.
6. **Calculate error:**
   ```
   f_error_Hz = f_displayed_MHz × 1e6 − 10,000,000
   ppm_error   = f_error_Hz / 10
   ```
7. **Document:** Record ppm error in calibration log. NanoVNA has no user frequency correction; the error is a known offset to apply mentally or in post-processing.
8. **Typical result:** NanoVNA within ±10 ppm of GPSDO reference = PASS.

**Acceptance criteria:** |f_error| < 5 ppm at 10 MHz. If >10 ppm, the NanoVNA crystal may need temperature stabilization or replacement.

### 9.2 OSL Calibration (S11 Amplitude and Phase)

OSL (Open-Short-Load) calibration corrects the NanoVNA's port 1 response for connector losses, impedance mismatch, and frequency-dependent errors.

**Preparation:**
- Verify GPS lock and frequency accuracy (section 9.1) before OSL.
- Allow NanoVNA 10 minutes warm-up.
- Use calibration standards from SOLT kit case.

**Procedure:**

1. Set NanoVNA sweep to desired frequency range (e.g., 1–50 MHz for HF).
2. On NanoVNA: **CAL → RESET** (clears previous calibration).
3. **SHORT calibration:**
   - Connect SHORT standard (red dot) to CH0.
   - Press **CAL → OPEN** — WAIT, this step is OPEN not SHORT.
   
   *NOTE: NanoVNA calibration order is typically OPEN → SHORT → LOAD → THRU.*
   
   - Press **CAL → OPEN**: Connect OPEN standard (green dot) → confirm.
   - Press **CAL → SHORT**: Connect SHORT standard (red dot) → confirm.
   - Press **CAL → LOAD**: Connect LOAD standard (blue dot) → confirm.
4. **THRU calibration** (for S21):
   - Connect SMA cable between CH0 and CH1.
   - Press **CAL → THRU** → confirm.
5. Press **CAL → DONE**.
6. Press **CAL → APPLY** (saves calibration).

**Verification:**
- With SHORT standard on CH0: S11 should be 0.0 dB, phase ≈ 180°.
- With OPEN standard on CH0: S11 should be 0.0 dB, phase ≈ 0°.
- With LOAD standard on CH0: S11 should be < −30 dB across sweep range.
- With THRU between CH0 and CH1: S21 should be 0.0 ± 0.5 dB.

**Amplitude verification using CAL LVL:**
- After OSL cal, connect GPSDO "CAL LVL" output (−20 dBm) to NanoVNA CH0.
- Set NanoVNA to show CH0 power (some NanoVNA firmware versions support this).
- Or measure using NanoVNA as signal generator: inject −20 dBm from GPSDO, measure on spectrum analyzer (TinySA) or SDR. Expected: −20 ± 1.5 dB.

### 9.3 Calibration Save and Recall

NanoVNA stores up to 5 calibration slots. After completing calibration:

1. **SAVE → Slot 1** (or desired slot): Saves this calibration for future sessions.
2. Label the slot in calibration log with date and frequency range.
3. Before any measurement session: **RECALL → Slot X** to restore saved calibration.

**Calibration validity window:**
- Temperature change ≤10°C from calibration temperature: valid
- Time since calibration: valid up to 24 hours (field conditions)
- Battery change or power cycle: may shift calibration slightly; re-verify SHORT/OPEN/LOAD

---

## 10. TINYSAA CALIBRATION PROCEDURES

### 10.1 Frequency Calibration

The TinySA internal clock determines frequency display accuracy. Calibration corrects the displayed frequency to match GPS-reference truth.

**Required:** TinySA v1 or TinySA Ultra with Expert Config access.

**Procedure:**

1. **Connect GPSDO:** PROG OUT → [10 dB attenuator pad] → TinySA INPUT.
   (Attenuator protects TinySA from overload at near-0 dBm signal level.)
2. **Set GPSDO frequency:** Web UI or BT command: `FREQ 10000000` (10 MHz).
3. **Set TinySA:** Center 10 MHz, Span 100 kHz, RBW 1 kHz.
4. **Read peak:** Note exact frequency displayed at peak marker (e.g., 10,000,412 Hz).
5. **Calculate ppm error:**
   ```
   ppm_error = (f_displayed − 10,000,000) / 10
              = (10,000,412 − 10,000,000) / 10
              = 41.2 ppm
   ```
6. **Apply correction:** TinySA → **CONFIG → EXPERT CONFIG → FREQ CORRECTION** → enter **−41** ppm.
   (Round to nearest integer; TinySA accepts integer ppm values.)
7. **Verify:** Repeat measurement; displayed frequency should now be within ±1 ppm of 10 MHz.

**Repeat at a second frequency** (e.g., 100 MHz) to verify linearity:
- `FREQ 100000000` on GPSDO; use 20 dB pad.
- Verify TinySA shows 100.000 ± 0.1 MHz.

### 10.2 Level Calibration

**Procedure:**

1. **Verify frequency calibration** (10.1) before level calibration.
2. **Connect:** GPSDO "CAL LVL" (−20 dBm reference, PROG OUT through on-board 20 dB pad) → TinySA INPUT.
3. **Set GPSDO:** `FREQ 10000000` (or desired calibration frequency).
4. **Read TinySA peak level:** should display −20.0 ± 1.0 dBm.
5. **Apply input correction:** TinySA → **CONFIG → CALIBRATION → INPUT CORRECTION** → adjust offset.
   - If reads −19.2 dBm, enter −0.8 dB correction (reduces displayed value by 0.8 dB).
6. **Verify:** Re-measure; should now read −20.0 ± 0.5 dBm.

**Multi-point level calibration:**
For best accuracy across TinySA's dynamic range, calibrate at −20, −40, −60 dBm using GPSDO PROG OUT through different attenuator pads. Document each correction. Average if corrections differ by <2 dB; use frequency-dependent correction if they vary more than 2 dB.

---

## 11. RTL-SDR FREQUENCY CORRECTION

### 11.1 RTL-SDR Frequency Error Sources

RTL-SDR dongles (RTL2832U + R820T2/E4000) use a 28.8 MHz crystal reference. Crystal accuracy: ±25 to ±100 ppm without TCXO. With TCXO upgrade: ±1 ppm.

Frequency error in RTL-SDR manifests as:
- Carrier displayed at wrong frequency in GQRX/SDR++
- WSPR spots decoded at ±25 Hz typical (negligible for WSPR)
- FT8/FT4: ±100 ppm at 14 MHz = ±1.4 kHz display error (problematic)

### 11.2 PPM Measurement Procedure

1. **Connect GPSDO:** PROG OUT → [30 dB attenuator] → RTL-SDR antenna input.
   (30 dB pad prevents overload; reduces ~0 dBm to ~−30 dBm, within RTL-SDR range.)
2. **Open GQRX:** Set hardware frequency to 10,000,000 Hz. Set RF gain to 20 dB.
3. **Set GPSDO:** `FREQ 10000000` (10 MHz GPS-locked signal).
4. **Observe carrier in GQRX:** The carrier should appear at 10.000 000 MHz ± the RTL-SDR error.
5. **Measure offset:** Use GQRX frequency click-and-drag marker to center on carrier.
   Example: carrier at 9,999,672 Hz → error = −328 Hz → −32.8 ppm at 10 MHz.
6. **Apply correction:**
   - GQRX: Configure → Input Devices → Device settings: **freq_corr_ppm = 33** (round to integer).
   - SDR++: Source → RTL-SDR → PPM Offset: **33**.
   - RTL_TCP: Command line `-p 33`.
7. **Verify:** Re-run; carrier should appear within ±5 Hz of 10,000,000 Hz.
8. **Record in calibration log:** Date, device serial, ppm correction applied.

**Repeat at higher frequency** (e.g., 144 MHz) using GPSDO `FREQ 144000000` (no pad needed at this level). Verify correction holds across frequency range.

### 11.3 TCXO-Equipped RTL-SDR

RTL-SDR Blog v3 units include a TCXO (±1 ppm). Measure and apply residual correction:

1. Measured error is typically ±5 ppm or less.
2. Apply exact correction in software as above.
3. TCXO drift is <0.5 ppm over 0–50°C; recalibrate annually or after cold exposure.

---

## 12. SDR INTERFACES PRESELECTOR CALIBRATION

### 12.1 Insertion Loss Verification

The preselector bandpass filters (from TM-SDR-001) introduce insertion loss that affects system noise figure and sensitivity. Verify filter performance after assembly using NanoVNA + GPSDO.

**Equipment:** NanoVNA (calibrated per section 9), GPSDO (locked), SMA cables.

**Procedure per filter:**
1. Configure NanoVNA sweep to cover ±20% of filter center frequency (e.g., for 40m: 5–10 MHz).
2. Connect NanoVNA CH0 → filter IN → NanoVNA CH1.
3. Measure S21 (insertion loss) at center frequency. Accept: < −3 dB.
4. Measure −3 dB bandwidth. Compare to design specification (Appendix A of TM-SDR-001).
5. Measure S11 (return loss) at center frequency. Accept: > −10 dB (VSWR < 2:1).
6. Record pass/fail in calibration log.

### 12.2 Frequency Counter Verification of Attenuator Setting

The PE4302 digital step attenuator has inherent frequency-dependent insertion loss. Verify:

1. Set attenuator to 0 dB. Measure RSSI with AD8307 using GPSDO signal source at each band center frequency.
2. Set attenuator to 20 dB. Measure RSSI. Verify ΔRSSI = 20 ± 1 dB.
3. If delta differs by >1 dB, the PE4302 calibration offset per frequency can be stored in ESP32 NVS.

### 12.3 SDR Noise Figure Measurement

After preselector calibration, estimate system noise figure using the Y-factor method:

1. Connect GPSDO "PROG OUT" through known attenuators to preselector input.
2. Set GPSDO to desired band center frequency.
3. Increase attenuation until carrier disappears into noise floor in SDR software.
4. The minimum detectable signal level equals the system MDS.
5. MDS = −174 + NF + 10×log10(BW)
6. Solve for NF: NF = MDS + 174 − 10×log10(BW)

---

## 13. FREQUENCY COUNTER OPERATION

### 13.1 Direct Measurement (1 Hz – 40 MHz)

1. Connect signal to be measured to BNC "FREQ IN" connector.
2. Ensure RANGE switch is in DIRECT (÷1) position.
3. Select gate time: 1s (standard) or 10s (high resolution).
4. Initiate measurement: web interface → COUNTER → START COUNT, or BT command: `COUNT`.
5. Wait for gate to complete (1 or 10 seconds).
6. Read displayed frequency.

**Signal level requirements:**
- Minimum: 50 mVpp sine wave or 1 Vpp square wave
- Maximum: 10 Vpp (limited by TVS clamp; do NOT exceed 30V peak)
- Impedance: 1 MΩ input (high-impedance probe style)

For 50Ω source output: add SMA-BNC adapter and connect directly. Source impedance of 50Ω and counter input impedance of 1 MΩ || 20 pF loads the source negligibly below 10 MHz.

### 13.2 Prescaled Measurement (40 MHz – 400 MHz)

1. Connect signal to BNC "FREQ IN".
2. Set RANGE switch to DIV10 position.
3. Initiate measurement.
4. Firmware automatically multiplies count × 10 and displays correct frequency.

**Note:** With 74HC390 prescaler, the counter measures accurately from 10 MHz to 400 MHz in this mode. Below 10 MHz in DIV10 mode, the 74HC390 count is still valid but resolution is reduced (1 count = 10 Hz at 1s gate).

### 13.3 Calibrating the NanoVNA Oscillator with the Counter

To directly measure the NanoVNA's internal reference oscillator (if accessible via test point):

1. Open NanoVNA (void warranty) or use oscillator output if available on connector.
2. Connect NanoVNA oscillator test point to BNC FREQ IN via probe or coax.
3. Measure: most NanoVNA units use 26 MHz crystal × PLL = various harmonic frequencies.
4. Compare to GPS-gated counter reading.

**Non-invasive method:** Use NanoVNA as signal source (set to transmission mode), measure its output frequency with counter.

---

## 14. CALIBRATION RECORD KEEPING

### 14.1 Calibration Log Requirements

Every calibration event should be logged with sufficient detail to:
- Reproduce the measurement
- Identify instrument drift trends over time
- Support decisions about instrument serviceability

Minimum log entry:
- Date and UTC time (from GPS display)
- Instrument identified (model, serial number if known)
- Measurement type (frequency, level, return loss)
- Expected value (from GPSDO reference)
- Measured value (instrument display)
- Error (measured − expected)
- Units
- Environmental conditions (temperature, GPS SVs)
- Operator (your callsign)

### 14.2 Web Interface Log Export

The esp32_cal_master firmware maintains a 50-entry rolling calibration log in RAM. Export procedure:

1. Connect to web interface at 192.168.4.1.
2. CALIBRATION LOG section → click **EXPORT CSV**.
3. Browser downloads `callog.csv`.
4. Open in spreadsheet; add to permanent calibration record.

### 14.3 Calibration Interval Recommendations

| Instrument | Recommended Cal Interval | Trigger Events |
|-----------|--------------------------|----------------|
| NanoVNA (no TCXO) | Before each field session | Temperature change >15°C |
| NanoVNA (with TCXO) | Every 30 days | Power cycle after storage |
| TinySA v1 | Before each session | Any |
| RTL-SDR (no TCXO) | Every session | Temperature change |
| RTL-SDR (with TCXO) | Monthly | Temperature extremes |
| Preselector filters | Annually | Physical shock |

---

## 15. WEB INTERFACE OPERATION

### 15.1 Connecting

1. Apply power to calibration station.
2. WiFi network "CAL-Station" appears within 10 seconds.
3. Connect device (phone, laptop) to "CAL-Station" (password: gpsdo2024).
4. Open browser to **192.168.4.1**.

### 15.2 GPS Status Card

Displays real-time:
- **GPS STATUS:** FIX (green) or NO FIX (red)
- **SATELLITES:** number of SVs tracked
- **UTC TIME:** GPS-derived UTC time (HH:MM:SS)
- **DISCIPLINE:** LOCKED (green, <1 ppb) or DISCIPLINING (orange, converging)
- **ERROR (ppb):** Current frequency error measurement
- **CORRECTION:** Si5351 correction register value (accumulated)

### 15.3 Signal Source Control

- **PRESET dropdown:** Select from 8 preset frequencies (100 kHz to 144 MHz)
- **CUSTOM FREQ field:** Enter arbitrary frequency in Hz; press SET
- **RSSI:** Live AD8307 reading of PROG OUT level in dBm
- Frequency changes take effect immediately; Si5351 updates CLK2

### 15.4 Frequency Counter

- **RANGE buttons:** DIRECT (≤40 MHz) or ÷10 (≤400 MHz)
- **GATE buttons:** 1s (1 Hz resolution) or 10s (0.1 Hz resolution)
- **START COUNT button:** Initiates measurement; display updates after gate closes
- **MEASURED FREQ:** Updated after each completed gate

### 15.5 Calibration Log

- Rolling 50-entry log visible in web interface
- **CLEAR:** Erases log from RAM
- **EXPORT CSV:** Downloads log as CSV file

---

## 16. CYD DISPLAY OPERATION

The CYD (Cheap Yellow Display, ESP32-2432S028) runs `cyd_cal_display.ino` and connects via Bluetooth to the calibration station.

### 16.1 Pages

**STATUS page** (default):
- Header row: GPS lock status, BT connection indicator
- GPS status: FIX/SEARCH, satellite count, UTC time
- Discipline lock: LOCKED (green) or DISCIPLINING (orange), ppb error value
- Signal source: current CLK2 frequency in large text
- RSSI: current level in dBm, 60-point sparkline history

**COUNTER page:**
- Large display of most recent counter measurement
- GPS reference status
- Touch instructions

**CAL GUIDE page:**
- Quick reference for NanoVNA, TinySA, RTL-SDR calibration steps
- Non-scrolling; fits on single screen

### 16.2 Touch Zones

| Touch Area | STATUS Page | COUNTER Page |
|-----------|-------------|--------------|
| Left half | Cycle to next freq preset | (reserved) |
| Right half | Start 1s count | Start 1s count |
| Bottom ⅓ left | Switch to STATUS tab | Same |
| Bottom ⅓ center | Switch to COUNTER tab | Same |
| Bottom ⅓ right | Switch to CAL GUIDE tab | Same |

### 16.3 BT Connection Management

The CYD automatically connects to "CAL-Master" on startup and reconnects every 5 seconds if connection drops. Data updates every 2 seconds when connected.

---

## 17. MAINTENANCE AND TROUBLESHOOTING

### 17.1 GPS Will Not Fix

**Symptom:** GPS LED blinks rapidly continuously; no fix after 5 minutes.

**Causes and remedies:**
1. **Antenna not connected:** Verify SMA cable from GPS ANT to antenna. Check continuity.
2. **Antenna blocked:** Move to clear sky location. Trees attenuate GPS signals.
3. **Active antenna not powered:** NEO-M8N VANT pin must supply 3.3V to active patch antenna. Check 10Ω + 100nF bias circuit.
4. **NEO-M8N not receiving:** Check UART: Serial Monitor should show `$GPGSV,` sentences. If blank, check RX/TX pin connections (RX of one device connects to TX of other).
5. **Almanac expired:** Without backup battery, full cold-start TTFF can take 12+ minutes. Allow extended time on first power-on outdoors.

### 17.2 LOCK LED Does Not Light After GPS Fix

**Symptom:** GPS LED solid (fix acquired), LOCK LED never goes solid after 10 minutes.

**Causes and remedies:**
1. **PCNT not counting Si5351 CLK0:** Verify CLK0 is connected to GPIO32. Measure CLK0 with oscilloscope at test point.
2. **Si5351 CLK0 not enabled:** Check firmware initialization; `output_enable(SI5351_CLK0, 1)` must be called.
3. **Frequency error too large to converge:** Initial XTAL error >500 ppm would require many iterations. Add `Serial.println(g_freq_error_ppb)` debug output to check values.
4. **GPS 1PPS not triggering interrupt:** Measure 1PPS at GPIO34 pin. Pull-up to 3.3V required. Should see 100ms pulses at 1 Hz.

### 17.3 Si5351 I2C Not Responding

**Symptom:** Serial Monitor shows `[CAL] Si5351 init FAILED`.

**Remedies:**
1. Check I2C pull-ups: measure ~3.3V at SDA (GPIO21) and SCL (GPIO22) with Si5351 connected.
2. Verify I2C address: default is 0x60; if ADDR pin tied to VDD, use 0x61. Update in config.h or library init call.
3. Scope I2C traffic: 100 kHz clock should appear on SCL during init.

### 17.4 Frequency Counter Reading Zero or Erratic

**Causes and remedies:**
1. **No 1PPS gate:** Verify GPS fix; counter requires 1PPS to gate.
2. **Signal too weak:** Input requires >50 mVpp. Check signal source; add signal preamplifier if needed.
3. **Signal too fast for direct mode:** Switch to DIV10 mode for f > 40 MHz.
4. **PCNT configuration not applied:** After range switch, ESP32 reconfigures PCNT input GPIO. Check `startCounter()` code path.
5. **Overflow ISR not registering:** Verify `pcnt_intr_enable()` called in setup.

### 17.5 AD8307 Reads −84 dBm (Zero Output)

**Causes:**
1. No input signal connected: normal behavior with open input.
2. AD8307 not powered: check 5V supply to VCC pin (or 3.3V if using reduced supply).
3. ENB pin not tied high: must be at VCC to enable IC.
4. Output filter capacitor too large: 100 nF may need to be reduced to 10 nF for faster settling.

### 17.6 CYD Shows "NO DATA FROM CAL-MASTER"

**Causes:**
1. BT not connected: CYD auto-reconnects every 5 seconds; wait 10 seconds.
2. CAL-Master BT not enabled: verify `BT.begin(BT_DEVICE_NAME)` called in esp32_cal_master setup.
3. BT pairing conflict: delete "CAL-Master" from CYD Bluetooth settings; power cycle both devices.

---

## APPENDIX A: CALIBRATION QUICK REFERENCE CARD

*Print, laminate, and store in SOLT kit case.*

```
┌─────────────────────────────────────────────────────────────────────┐
│                   GPS CAL STATION — QUICK REFERENCE                  │
│                                                                      │
│  STARTUP SEQUENCE:                                                   │
│  1. Power on station. GPS LED blinks.                               │
│  2. Wait for GPS LED solid (3D fix, ~60s outdoors).                 │
│  3. Wait for LOCK LED solid (discipline locked, ~3 min).            │
│  4. Wait additional 10 min before calibrating.                      │
│                                                                      │
│  CONNECTIONS:           NANOVNA:                                     │
│  10M REF → SMA          OPEN → CAL OPEN                            │
│  1M REF  → SMA          SHORT → CAL SHORT                          │
│  PROG OUT → SMA         LOAD → CAL LOAD                            │
│  CAL LVL → SMA(−20dBm)  THRU → CAL THRU → DONE                   │
│  FREQ IN → BNC                                                       │
│                         TINYSAA:                                     │
│  STANDARDS:             PROG OUT + 10dB → input                     │
│  RED   = SHORT          Config>Expert>Freq Correction (ppm)         │
│  GREEN = OPEN           CAL LVL → input → check −20 dBm            │
│  BLUE  = LOAD                                                        │
│  NONE  = THRU           RTL-SDR:                                    │
│                         PROG OUT + 30dB → SDR                       │
│  ATTENUATOR PADS:       Measure offset at 10 MHz                    │
│  3,6,10,20,30,40 dB     GQRX: Config>PPM Offset                     │
│                                                                      │
│  WiFi: CAL-Station / gpsdo2024    IP: 192.168.4.1                  │
│  BT: CAL-Master                                                      │
└─────────────────────────────────────────────────────────────────────┘
```

---

## APPENDIX B: CALIBRATION LOG TEMPLATE

```
INSTRUMENT CALIBRATION RECORD

Date: ____________  UTC Time: ____________  GPS SVs: ____  Temp: ____°C
Station: ________________  Operator Callsign: ________________

GPS Lock Status:  □ LOCKED (<1 ppb)   □ DISCIPLINING   □ NOT LOCKED

──────────────────────────────────────────────────────────────────────
INSTRUMENT:     ____________________________________________
Model/SN:       ____________________________________________
Last Cal Date:  ____________

FREQUENCY CALIBRATION:
  Reference freq (GPSDO): __________________ Hz
  Displayed freq:          __________________ Hz
  Error (Hz):              __________________
  Error (ppm):             __________________  (accept: < 5 ppm)
  Correction applied:      __________________  (ppm value entered)
  Result:  □ PASS (<5 ppm)   □ CONDITIONAL (5–20 ppm)   □ FAIL (>20 ppm)

AMPLITUDE CALIBRATION:
  Reference level (GPSDO CAL LVL): −20.0 dBm
  Displayed level:                 __________ dBm
  Error (dB):                      __________  (accept: < 1.5 dB)
  Correction applied:              __________ dB
  Result:  □ PASS (<1.5 dB)   □ CONDITIONAL (1.5–3 dB)   □ FAIL (>3 dB)

NanoVNA SOLT CAL VERIFICATION (if performed):
  SHORT S11 magnitude:  __________ dB  (accept: >−0.1 dB)
  OPEN  S11 magnitude:  __________ dB  (accept: >−0.1 dB)
  LOAD  S11 magnitude:  __________ dB  (accept: <−30 dB)
  THRU  S21 magnitude:  __________ dB  (accept: <−0.5 dB loss)
  Result:  □ PASS   □ FAIL (repeat SOLT cal)

NOTES:
________________________________________________________________
________________________________________________________________

Next Cal Due: ______________________  Cal Interval: ______________
──────────────────────────────────────────────────────────────────────
```

---

## APPENDIX C: FREQUENCY ERROR TABLE

Reference for computing ppm error at common calibration frequencies:

| GPS Ref Freq | 1 ppm error = | 5 ppm = | 10 ppm = | 25 ppm = |
|-------------|--------------|---------|---------|---------|
| 1.000 MHz | 1 Hz | 5 Hz | 10 Hz | 25 Hz |
| 3.500 MHz | 3.5 Hz | 17.5 Hz | 35 Hz | 87.5 Hz |
| 7.000 MHz | 7 Hz | 35 Hz | 70 Hz | 175 Hz |
| 10.000 MHz | 10 Hz | 50 Hz | 100 Hz | 250 Hz |
| 14.000 MHz | 14 Hz | 70 Hz | 140 Hz | 350 Hz |
| 21.000 MHz | 21 Hz | 105 Hz | 210 Hz | 525 Hz |
| 28.000 MHz | 28 Hz | 140 Hz | 280 Hz | 700 Hz |
| 50.000 MHz | 50 Hz | 250 Hz | 500 Hz | 1,250 Hz |
| 100.000 MHz | 100 Hz | 500 Hz | 1,000 Hz | 2,500 Hz |
| 144.000 MHz | 144 Hz | 720 Hz | 1,440 Hz | 3,600 Hz |

**Quick rule:** ppm = (frequency error in Hz) / (reference frequency in MHz)

---

## APPENDIX D: PARTS LIST AND SOURCES

| Qty | Part | Value / Model | Source | ~Unit Cost |
|-----|------|---------------|--------|-----------|
| 1 | GPS Module | u-blox NEO-M8N with antenna | AliExpress | $18 |
| 1 | Clock Synth | Si5351A-B-GT (SOP-20) or breakout | DigiKey / Adafruit | $2–$6 |
| 1 | Buffer IC | 74AC04 (SOIC-14 or DIP) | DigiKey | $0.50 |
| 1 | Microcontroller | ESP32-WROOM-32 devboard | AliExpress | $5 |
| 1 | LDO Regulator | AMS1117-3.3 | DigiKey | $0.30 |
| 1 | Log Amplifier | AD8307AN (DIP-8) | DigiKey | $6 |
| 1 | Prescaler | 74HC390 or 74AC390 (DIP-16) | DigiKey | $0.80 |
| 1 | Schmitt Trigger | 74ACT14 (SOIC-14) | DigiKey | $0.60 |
| 1 | TVS Diode | SMAJ5.0CA | DigiKey | $0.30 |
| 1 | TCXO (opt.) | 25 MHz, ±1 ppm, CMOS | DigiKey / Mouser | $4 |
| 4 | SMA(F) Panel | Edge launch or panel mount | Amazon | $1 ea |
| 1 | BNC(F) Panel | Panel mount | Amazon | $1.50 |
| 1 | SMA(M) conn. | PCB edge type (×6 for SOLT kit) | Amazon (10-pk) | $0.50 ea |
| 6 | SMD Attenuator | SMA-SMA inline 3/6/10/20/30/40 dB | Mini-Circuits or Amazon | $2–4 ea |
| 3 | SMA cable | 150mm RG-316 SMA-SMA | Amazon | $3 ea |
| 2 | BNC-SMA adapter | BNC(M)-SMA(F) | Amazon | $2 ea |
| 2 | 100Ω 0402 (0.1%) | SOLT load resistors | DigiKey | $0.15 ea |
| 10 | SMD resistors | 0.1% 0402 for attenuators | DigiKey | $0.15 ea |
| 1 | CR1220 + holder | GPS almanac backup | Amazon | $0.80 |
| 1 | OLED (option) | SSD1306 128×64 I2C | Amazon | $3 |
| 1 | CYD display | ESP32-2432S028 | AliExpress | $12 |
| 1 | Enclosure | Printed PETG (gpsdo_enclosure.scad) | (print) | $1.50 filament |
| 1 | SOLT case | Printed PETG (solt_kit_case.scad) | (print) | $2 filament |
| 1 | Copper tape | 25mm × 10m, conductive adhesive | Amazon | $6 |
| — | Misc | capacitors, resistors, wire, connectors | — | $5 |
| | | | **TOTAL** | **~$120–150** |

**Key suppliers:**
- **DigiKey** (digikey.com): Precision components, ICs, connectors
- **Mouser** (mouser.com): Alternative to DigiKey; similar inventory
- **AliExpress**: GPS modules, ESP32 boards, SMA cables (10–14 day shipping)
- **Amazon**: Adapters, cables, copper tape (2-day shipping)
- **Mini-Circuits** (minicircuits.com): Precision attenuator pads (higher cost, better specs)

---

*TM-CAL-001 Rev A — GPS-Disciplined Field Calibration System*  
*For amateur radio and field RF measurement use.*  
*All calibration values traceable to GPS atomic frequency standard.*
