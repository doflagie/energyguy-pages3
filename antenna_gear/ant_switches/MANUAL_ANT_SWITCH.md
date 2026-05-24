# TECHNICAL MANUAL TM-ANT-SW-001
## Coaxial Antenna Switches for Amateur Radio Stations
### Revision A — April 2026

---

```
DISTRIBUTION STATEMENT A: Approved for unrestricted distribution.

This manual covers design, construction, installation, operation, and
maintenance of coaxial antenna switching systems for amateur radio stations
operating on 160 meters through 23 centimeters.

All designs comply with FCC Part 97 amateur radio technical standards.
High-power designs (1 kW+) must comply with applicable local electrical codes.
```

---

## TABLE OF CONTENTS

| Chapter | Title |
|---------|-------|
| 1 | Introduction and Safety |
| 2 | RF Switching Theory |
| 3 | Manual Rotary Switch Designs (SP2T, SP4T, SP6T) |
| 4 | Relay-Based Remote Switch |
| 5 | Motorized Rotary Switch |
| 6 | Automatic Band-Data Switch |
| 7 | High-Power Switch (1 kW / 2 kW PEP) |
| 8 | ESP32/CYD Firmware Guide |
| 9 | Band-by-Band Quick Reference |
| 10 | Installation Procedures |
| 11 | Testing and Alignment |
| 12 | Troubleshooting |
| Appendix A | Parts List Master |
| Appendix B | Band Data Interface Pinouts |
| Appendix C | RF Performance Calculations |
| Appendix D | Schematic and Firmware File Index |

---

## CHAPTER 1 — INTRODUCTION AND SAFETY

### 1.1 Purpose

This manual provides complete engineering documentation for antenna switching systems used in amateur radio stations. A switching system routes the transceiver output to one of several antennas, or routes multiple receivers to a single antenna.

Four levels of design are covered, ordered by complexity:

1. **Manual rotary switch** — purely mechanical, no electronics, highest reliability
2. **Relay remote switch** — ESP32-controlled electromechanical relays, WiFi/web interface
3. **Motorized rotary switch** — stepper motor drives a mechanical rotary switch remotely
4. **Automatic band-data switch** — reads band data from transceiver, selects antenna automatically

The high-power (1 kW / 2 kW PEP) design is a variant of options 2 and 3 using appropriately rated components.

### 1.2 Safety

---

> **WARNING — HIGH VOLTAGE**
> At 1500W into 50Ω with SWR 3:1, peak RF voltage at the switch can reach
> 775V. This voltage is LETHAL. Do not handle antenna connections while
> transmitting. De-energize the transmitter before connecting or disconnecting
> any coaxial cable.

---

> **WARNING — TX DURING SWITCHING**
> Never switch antenna ports while the transmitter is active. Switching under
> load creates an open-circuit momentarily, causing high reflected power that
> can damage the transmitter's final amplifier stage. All firmware designs
> include a TX-inhibit input for this reason — wire it.

---

> **WARNING — GROUND ALL UNUSED PORTS**
> Unconnected antenna ports accumulate static charge. This charge discharges
> suddenly through the receiver front-end (the lowest-impedance path) when
> sufficient voltage builds. Install static drain resistors (47 kΩ to chassis)
> on all unused ports.

---

### 1.3 Document Organization

Chapters 3–7 correspond to the five schematic files. Each chapter expands the schematic with theory, construction procedure, and performance specifications. Chapter 8 covers the three firmware files as a unified ESP32/CYD programming guide.

---

## CHAPTER 2 — RF SWITCHING THEORY

### 2.1 Insertion Loss

Insertion loss (IL) is the signal power lost when passing through the switch in the selected state. It includes:

**Contact resistance loss:**
```
IL_contact (dB) = 20 × log₁₀(1 + R_contact / Z₀)

For R_contact = 5 mΩ: IL = 0.0004 dB (negligible)
For R_contact = 500 mΩ (corroded): IL = 0.04 dB (noticeable)
```

**Shunt capacitance loss** (at junction where conductors meet):
```
IL_shunt (dB) ≈ 10 × log₁₀(1 + (πfC × Z₀/2)²)

For C_stray = 3 pF at 144 MHz:
  IL = 10 × log₁₀(1 + (π × 144×10⁶ × 3×10⁻¹² × 25)²)
     = 0.03 dB (acceptable)
```

**Combined specifications:**

| Band | Target IL | Achievable with good design |
|------|-----------|----------------------------|
| 1.8–30 MHz | < 0.05 dB | 0.01–0.03 dB typical |
| 50 MHz | < 0.10 dB | 0.03–0.07 dB |
| 144 MHz | < 0.15 dB | 0.05–0.12 dB |
| 432 MHz | < 0.20 dB | 0.10–0.18 dB |
| 1296 MHz | < 0.30 dB | 0.20–0.35 dB |

### 2.2 Port Isolation

Isolation is the attenuation between any two ports when neither is selected (both isolated), or between the selected port and an unselected port.

Isolation is limited by:
- Open contact capacitance C_open (typically 0.1–0.5 pF for mechanical contacts)
- Stray electromagnetic coupling through the enclosure
- Common ground path impedance

```
Isolation from C_open:
  I (dB) ≈ 20 × log₁₀(1 / (2π × f × C_open × Z₀/2))

For C_open = 0.5 pF:
  At 14 MHz:   I = 88 dB (excellent)
  At 144 MHz:  I = 68 dB (good)
  At 432 MHz:  I = 55 dB (adequate)
  At 1296 MHz: I = 45 dB (marginal)
```

**Improving isolation:**
- Metal dividers between port cavities: +20–40 dB at VHF/UHF
- 50Ω terminations on unused ports: +6 dB minimum
- Silver-plated contacts: minimizes C_open
- Keep enclosure cavities small (limits free-space coupling)

### 2.3 VSWR

VSWR degradation from a switch is caused by the junction geometry presenting an impedance discontinuity. At HF frequencies, the switch body dimensions are negligibly small compared to wavelength and VSWR is determined entirely by connector quality and center conductor continuity.

At VHF/UHF, maintain 50Ω center conductor geometry through the switch body. The center conductor diameter for 50Ω with PTFE dielectric (ε_r = 2.05) in a given outer conductor ID can be calculated, but for practical switch designs use the connector manufacturer's geometry and minimize the internal conductor length.

### 2.4 Power Handling

Peak RF voltage on a transmission line:

```
Matched load (SWR = 1:1):
  V_peak = √(2 × P × Z₀)    [V, W, Ω]

Under standing waves (SWR > 1:1):
  V_max = V_peak × √SWR      [at the voltage maximum node]
  I_max = I_peak × √SWR      [at the current maximum node]
```

At **1000W CW**: V_peak = 316V, I_peak = 6.32A (matched)
At **2000W PEP, SWR 3:1**: V_max = 775V, I_max = 15.5A

All switch contacts, connectors, and wiring must be rated for these values with appropriate safety margin.

### 2.5 Static Charge Accumulation and Discharge

An antenna not connected to a DC ground path accumulates static charge from precipitation, wind-blown debris, and corona. The accumulated charge can reach several kilovolts before the switch contact presents a low-impedance path during the next transmit sequence.

The sudden static discharge through the receiver front-end is a primary cause of receiver damage in unprotected stations. This failure mode is distinct from lightning damage and occurs in calm weather.

**Prevention:** Install a 47 kΩ to 100 kΩ resistor from each antenna port center conductor to chassis ground. This resistor:
- Drains static at the rate: I = V_static / R_drain (slow, continuous)
- Does not affect RF signal (R_drain >> Z₀ = 50Ω)
- Provides defined DC reference for all antenna inputs

---

## CHAPTER 3 — MANUAL ROTARY SWITCH DESIGNS

### 3.1 SP2T (2-Position)

The simplest switch: two antenna ports, one common. Used for:
- A/B antenna comparison at same band and time
- Switching between HF and VHF antennas on a shared feedline run
- Two-radio operation sharing one antenna (with TX lockout between radios)

**Construction notes:**
- SP2T can be built with a single DPDT toggle switch (use one pole for RF, other for accessory or shorting the unselected port)
- Ceramic wafer switch (Alpha SR2912F or Grayhill 56D set to 2 positions) is preferred for RF quality

### 3.2 SP4T (4-Position)

The standard station antenna switch. Four antenna ports cover most multi-band stations:
- Port 1: 160M/80M dipole
- Port 2: 40M/30M wire
- Port 3: 20M/17M/15M beam or wire
- Port 4: 10M/6M vertical or beam

**Panel layout:** Rotary knob center, four SO-239 on top face (N/E/S/W), common SO-239 on front face. Position markings embossed on lid.

**Contact arrangement:** 4-position, 30° detents (or 90° if 4-position with stops). The mechanical detent is critical — the switch must not rest between positions. A switch in an intermediate position shorts two antennas together.

### 3.3 SP6T (6-Position)

Six antenna ports for large multi-band or multi-directional arrays. With a 6-port switch and a rotatable beam:
- Port 1: 160M vertical
- Port 2: 80M dipole (or horizontal loop)
- Port 3: Beverage receive antenna
- Port 4: 40M wire
- Port 5: Tribander beam (20/15/10M)
- Port 6: 6M/VHF yagi

**60° port spacing** on a 6-position stator; rotary travel: 300° (5 × 60°) with end stops.

### 3.4 Contact Care and Maintenance

Silver-plated contacts oxidize over time. The oxide film increases contact resistance.

**Annual maintenance procedure:**
1. Rotate switch through all positions 10–20 times. The wiping action self-cleans.
2. If resistance is elevated (measured with ohmmeter): apply DeoxIT D5 contact cleaner to the wiper area; rotate through all positions.
3. Do not use abrasives on silver-plated contacts.
4. After cleaning, apply a thin film of DeoxIT Gold G5 to stabilize the surface.
5. Re-measure contact resistance. Target: < 10 mΩ.

### 3.5 High-Power Manual Switch

For 1.5 kW CW or 2 kW PEP:

- Use ceramic-body wafer switch (not phenolic)
- Silver or silver-plated contacts (minimum 500V isolation when open)
- Contact wiping force ≥ 200g (prevents micro-arcing from vibration)
- SO-239 connectors: adequate for 1.5 kW CW; N-type recommended for 2 kW continuous

Calculate sparkover margin:
```
At 2000W PEP, SWR 3:1:
  V_max = √(2 × 2000 × 50) × √3 = 775V
  
Contact isolation must exceed V_max × 2 = 1550V minimum.
Most ceramic rotary switches specify 500V isolation — marginal.
Verify with manufacturer or use a commercial high-power switch.
```

---

## CHAPTER 4 — RELAY-BASED REMOTE SWITCH

### 4.1 Relay Selection

The most important design decision is the relay type. Three categories:

**Category 1 — Purpose-built coaxial relays (best RF performance):**
- Tohtsu CX-110D (SO-239, 12V, 1 kW HF), CX-800 (2 kW HF)
- Dow-Key 40x series (commercial quality, 5 kW+)
- Defined insertion loss, VSWR, and isolation specifications
- Cost: $15–$80 each

**Category 2 — Quality SPDT signal relay on 50Ω PCB (budget):**
- Omron G2RL-1A4-E-TV8 (12V SPDT), Panasonic TQ2-12V
- Adequate IL/isolation at HF; degrades above 150 MHz
- Requires careful PCB layout (50Ω microstrip, ground plane, metal shields)
- Cost: $1–$5 each

**Category 3 — Latching relay (power-saving):**
- Omron G2RK-L (latching version of G2RL)
- Zero standby current after switching (pulse only to change state)
- Requires H-bridge or push-pull coil driver circuit
- Cost: $8–$15 each

### 4.2 Circuit Topology

**One relay per port (preferred for ≤8 ports):**
- Each relay switches one antenna path
- Only one relay energized at a time
- Signal passes through exactly one relay contact
- Minimum insertion loss path

**Binary tree (fewer relays, higher IL):**
- N ports require N-1 relays
- Signal passes through log₂(N) relay contacts
- Insertion loss accumulates: 4-port = 2 contacts in path
- Use only when relay count is the binding constraint

### 4.3 Relay Driver Circuit

The ULN2003A is the preferred relay driver for this design:

```
ESP32 GPIO (3.3V) → ULN2003A input → Darlington output → relay coil → +12V

ULN2003A internal structure per channel:
  - Input: 1kΩ resistor + Darlington NPN pair (hFE ≈ 1000)
  - Output: 500mA sink, 50V max
  - Built-in suppression diode (no external flyback diode needed)
  - 7 channels per IC ($0.40 each)
```

For higher current (Tohtsu CX-800 at 320 mA, or vacuum relays at 500+ mA):
```
Use IRLZ44N MOSFET per relay:
  ESP32 GPIO → 10kΩ → IRLZ44N Gate
  Relay coil → IRLZ44N Drain (to supply via flyback 1N5819)
  IRLZ44N Source → GND
  
R_DS(on) = 22 mΩ at V_GS = 5V → negligible power loss
No heatsink needed for relay drive currents.
```

### 4.4 TX Inhibit Wiring

Wire the transceiver's SEND line (TX active output) to ESP32 GPIO34 (input-only pin, no internal pull-up — add external 10 kΩ pull-down).

**Yaesu SEND line:** Active HIGH during transmit. Verify with DMM: should read 5V during transmit, 0V on receive.

**If no SEND line available:** Build an RF power detector:
- 20 dB directional coupler tap from COMMON port
- Schottky diode rectifier (BAT42, BAT54)
- RC filter (100 kΩ + 0.1 µF) into ESP32 ADC pin
- Threshold in firmware: > 200 mV ADC reading = TX active

**Firmware logic:**
```c
if (tx_active()) {
    queue_pending_switch(port);   // defer
    return;
}
set_antenna(port);                // execute immediately
```

The pending switch executes the moment the TX active flag clears.

### 4.5 RF Layout Guidelines

For the budget relay design (PCB implementation):

1. Route COMMON center conductor as 50Ω microstrip (W ≈ 3.0 mm on 1.6 mm FR-4)
2. Each relay connects via a short stub (< 5 mm) from the main trace to the relay contact
3. Relay contact to antenna port: another short 50Ω trace stub
4. Solid copper pour under all RF traces (ground plane)
5. Via stitching around the RF area perimeter (every 5 mm)
6. Metal shield or divider between relay positions (tack-solder a copper strip between relay bays)
7. Separate analog/digital ground reference only at one star point (power supply decoupling)

### 4.6 WiFi Configuration

WiFi credentials are stored in NVS (non-volatile storage via Preferences library). Configure via serial terminal at 115200 baud:

```
Serial command syntax:
  ANT 1     — select antenna 1 (manual)
  ANT 2     — select antenna 2
  ANT 3     — select antenna 3
  ANT 4     — select antenna 4
  STATUS    — print current status
  HELP      — command list
```

To set WiFi credentials, add these lines to setup() temporarily then remove:
```c
prefs.putString("ssid", "your_ssid");
prefs.putString("pass", "your_password");
```

Once connected, access the web interface at the IP shown on the CYD display.

---

## CHAPTER 5 — MOTORIZED ROTARY SWITCH

### 5.1 Design Philosophy

The motorized design preserves the RF performance of the mechanical rotary switch while adding remote control capability. The RF path is identical to the manual switch; only the knob is replaced by a motor drive.

Advantages over relay-based switch:
- RF performance identical to proven mechanical switch contacts
- No RF-path relays (no compromise from relay inductance or contact geometry)
- Higher power handling (mechanical contacts, not relay contacts)
- Easy retrofit of an existing manual switch

### 5.2 28BYJ-48 Stepper Motor

The 28BYJ-48 is a 5-wire unipolar stepper motor with an internal 1:64 reduction gearbox.

**Specifications:**
- Output shaft: 5.625°/step × 64 (gearbox) = 0.0879°/full step
- Full rotation: 512 full steps (or 1024 half-steps)
- Holding torque: 34 mN·m (after gearbox)
- Coil voltage: 5V DC, 4 phases
- Coil current: ~65 mA per phase; 2 phases active simultaneously = 130 mA

**For a 4-position switch (90° between positions):**
```
Steps per position change = 512 × 90/360 = 128 full steps
At 5 ms per step: 128 × 5 ms = 640 ms per position change
Half-step mode (1024 steps/rev): 256 steps, same time = smoother motion
```

**For a 6-position switch (60° between positions):**
```
Steps per position = 512 × 60/360 = 85.3 → use 85 steps
Position error accumulation: 0.3 steps per position = 0.03° — negligible if Hall sensors confirm position
```

### 5.3 Hall Sensor Absolute Position

Four A3144 Hall sensors (or OH090U) at the four detent positions provide absolute position confirmation independent of step counting.

**Installation:**
- Epoxy a 3 mm × 1.5 mm neodymium disc magnet to the switch shaft (or to a disc attached to the shaft)
- Mount four Hall sensors at 0°, 90°, 180°, 270° positions on the enclosure wall adjacent to the shaft
- Air gap: 1–2 mm between magnet face and sensor face
- A3144: VCC = 3.3V, open-drain output. Install 10 kΩ pull-up to 3.3V on each output.
- Output LOW = magnet present = at this position

**Absolute position decoding:**
```
Read all four sensors simultaneously.
Exactly one LOW → at that position.
All HIGH → in-transit between positions (valid during motor movement).
Two or more LOW → fault (magnets too close, or sensor placement error).
```

### 5.4 Shortest-Path Rotation

The firmware calculates whether to rotate clockwise (forward) or counterclockwise (reverse) to reach the target position in minimum steps:

```c
int fwd_steps = ((target - current + NUM_POSITIONS) % NUM_POSITIONS) * STEPS_PER_POS;
int rev_steps = ((current - target + NUM_POSITIONS) % NUM_POSITIONS) * STEPS_PER_POS;

if (fwd_steps <= rev_steps) { rotate_forward(fwd_steps); }
else                         { rotate_reverse(rev_steps); }
```

For a 4-position switch: maximum rotation is 2 positions = 180° (256 steps). Minimum is always taken.

### 5.5 Power Management

**De-energize coils after positioning:**
The 28BYJ-48 consumes 260 mA when running. The mechanical detent holds position without power. After confirming the position via Hall sensor, de-energize all coils.

```c
// After confirm:
step_off();   // all coil pins LOW
```

**PWM holding (optional for high-torque applications):**
If the switch requires continuous holding power (no detent, or stiff detent spring):
```c
// 50% duty cycle PWM on active coils
for (int i = 0; i < 4; i++)
    ledcWrite(i, STEP_SEQ[step_idx][i] * 128);  // 128/255 ≈ 50%
```

---

## CHAPTER 6 — AUTOMATIC BAND-DATA SWITCH

### 6.1 Band Data Interface Overview

Modern HF transceivers provide band information to external accessories. The band data decoder reads this information and automatically selects the pre-configured antenna for the current band.

**Source selection (configurable at runtime):**

| Source | Method | Best for |
|--------|--------|---------|
| Yaesu BCD | 4-wire logic | Yaesu FT-series, most common |
| Icom CI-V | Single-wire serial | Icom IC-7300, IC-7610, etc. |
| Band voltage | ADC (0–3.3V) | Older Kenwood, generic |
| Manual | Web/serial only | Radios without band output |

### 6.2 Yaesu BCD Interface

The Yaesu band data output is 5V TTL logic. The ESP32 GPIO is 3.3V tolerant on inputs but **5V is above the maximum spec**. Use a voltage divider:

```
Radio BCD pin (5V logic) ──[3.3kΩ]──┬── ESP32 GPIO
                                      │
                                   [6.8kΩ]
                                      │
                                     GND

Output voltage at ESP32: 5V × 6800/(3300+6800) = 3.37V ≈ 3.3V (marginal but safe)

Alternative (cleaner): CMOS 74LVC125 level shifter
  OE tied LOW; inputs 0–5V; outputs 0–3.3V.
```

**BCD verification procedure:**
1. Connect DMM to each BCD pin (B1, B2, B4, B8) with respect to radio GND.
2. Tune to 40M (7.0 MHz). Expected BCD = 4 (0100): B4 = HIGH, others LOW.
3. Verify each pin matches the decode table in sch_auto_band_switch.txt.
4. Some radios label B1 as MSB rather than LSB — swap if band decode is wrong.

### 6.3 Icom CI-V Interface

CI-V is a single-wire open-collector bidirectional bus at 1200 baud (adjustable).

**Minimal connection for listen-only:**
```
Radio CI-V jack (3.5mm tip) ──[10kΩ pull-up to 3.3V]──[1kΩ series]── ESP32 GPIO16 (UART2 RX)
```

The firmware parses frequency-update packets from the radio and maps frequency to band using the table in the source. No polling is required; the radio sends updates whenever frequency changes.

**CI-V baud rate:** Most Icom radios default to 9600 baud, selectable in radio menu. The firmware opens Serial2 at 1200 baud (default); change `Serial2.begin(1200,...)` to `Serial2.begin(9600,...)` if needed.

### 6.4 Band-to-Antenna Mapping Configuration

The mapping is stored in NVS and configurable via:

**Web interface:** Navigate to the switch IP address. The main page shows a table of all bands with buttons to assign each band to any antenna port.

**Serial command:**
```
MAP 20M 3      — assign 20M band to antenna port 3
MAP 40M 2      — assign 40M to port 2
STATUS         — show current mapping
```

**Default mapping (4-port switch):**
```
160M, 80M    → Port 1 (low-band antenna)
40M, 30M     → Port 2 (mid-band)
20M, 17M, 15M → Port 3 (beam or wire)
12M, 10M, 6M  → Port 4 (high-band)
```

### 6.5 TX Inhibit with Band Data

When the radio indicates a band change while transmitting (e.g., on a split-band transceiver where VFO-B is on a different band from VFO-A):

The firmware detects the SEND line and queues any band-triggered switch for execution when TX ends. A 500 ms settling time after TX ends before executing the switch prevents switching during the tail of an over-relayed signal.

---

## CHAPTER 7 — HIGH-POWER SWITCH (1 kW / 2 kW PEP)

### 7.1 Design Approach

High-power switching demands different component selection while the overall topology remains identical to the relay switch. The two primary choices are:

**Coaxial relay approach (Tohtsu CX-800 or equivalent):**
- Self-contained RF path in a factory-built coaxial relay
- Verified IL, VSWR, isolation specifications
- Simple installation; no PCB layout concerns
- Cost: $30–$80 per relay

**Vacuum relay approach (Jennings RJ1A, Gigavac GH24):**
- Glass-sealed contacts in vacuum
- Handles 10 kV peak voltage, 30A continuous
- DC to 200 MHz RF rating
- Used in high-power amplifiers and commercial RF switches
- Cost: $50–$150 per relay; last essentially forever if not mistreated

### 7.2 Current and Voltage Calculations

For all high-power switch designs, calculate the worst-case values at the design stage:

```
Design target: 2000W PEP, worst-case SWR 3:1

V_max = √(2 × P_PEP × Z₀) × √(SWR)
      = √(2 × 2000 × 50) × √3
      = 447 × 1.73 = 775V peak

I_max = V_max / Z₀ = 775 / 50 = 15.5A peak
I_RMS  = √(P / Z₀) = √(2000/50) = 6.32A (at PEP, matched load)

Contact voltage isolation required: > 775V × 2 (safety factor) = 1550V minimum
Contact current rating: > 6.32A RMS continuous (for CW)
```

The CX-800 is rated for 2000W at HF. The Jennings RJ1A handles far more. Both meet these requirements.

### 7.3 Connector Selection for High Power

**SO-239 (UHF-type) connectors:**
- Adequate for 1.5 kW CW at HF
- Silver-plated center pin carries 4.47A RMS at 1 kW/50Ω without thermal issue
- Not recommended for > 1.5 kW continuous or VHF applications above 200 MHz

**N-type connectors:**
- Recommended for 2 kW+
- Center pin diameter: 3.05 mm — larger current capacity than SO-239
- Specified to 1.5 kW at 1 GHz (some types)
- IP67 rated: mandatory for outdoor high-power applications

**For 2 kW CW continuous:**
- N-type mandatory
- Silver-plated interfaces
- Torque connectors to specification (20–30 in-lb for N-type)

### 7.4 Isolation at High Power

At high power, inadequate isolation allows transmitter power to couple into unselected antenna ports. This is not just a signal purity issue — at 1 kW, even −40 dB isolation = 100 mW at the unselected port. This is enough to damage a connected preamplifier or low-noise receiver.

**Maintain > 60 dB isolation at all operating frequencies** by:
- Metal dividers between port cavities
- Coaxial relay design (self-shielded coaxial structure)
- Terminated unselected ports with 50Ω loads

### 7.5 Thermal Considerations

For relay coil power dissipation in continuous operation:

| Relay | Coil power | 4 relays (1 active) | Enclosure ΔT |
|-------|-----------|--------------------|----|
| CX-800 (12V, 320mA) | 3.84W | 3.84W | ~40°C (Hammond 1590B2) |
| Omron G2RL (12V, 400mA) | 4.8W | 4.8W | ~50°C |

Add ventilation if ambient temperature exceeds 30°C, or if multiple relays remain energized.

---

## CHAPTER 8 — ESP32/CYD FIRMWARE GUIDE

### 8.1 CYD Hardware Reference

The CYD (Cheap Yellow Display, ESP32-2432S028) is an ESP32-WROOM-32 module with integrated:
- 2.8" ILI9341 TFT LCD (320×240), resistive touchscreen
- MicroSD slot
- RGB LED
- USB-C connector (CH340 USB-UART)

**TFT SPI bus (HSPI):**
```
CLK  → GPIO14
MOSI → GPIO13
MISO → GPIO12
CS   → GPIO15
DC   → GPIO2
BL   → GPIO21 (backlight, active HIGH)
```

**Touch SPI (separate):**
```
CLK  → GPIO25
MOSI → GPIO32
MISO → GPIO39
CS   → GPIO33
IRQ  → GPIO36
```

**TFT_eSPI library configuration (`User_Setup.h`):**
```cpp
#define ILI9341_DRIVER
#define TFT_CLK  14
#define TFT_MOSI 13
#define TFT_MISO 12
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST  -1
#define TOUCH_CS 33
#define SPI_FREQUENCY       40000000
#define SPI_TOUCH_FREQUENCY  2500000
```

**GPIO conflicts:** GPIO12 and GPIO14 are used by HSPI and may conflict with relay outputs or Hall sensors if the CYD's TFT SPI is also active. Reassign relay/sensor GPIO when using the full CYD — see the pin assignment tables in each firmware file.

### 8.2 Library Dependencies

Install via Arduino IDE Library Manager:

| Library | Version | Purpose |
|---------|---------|---------|
| TFT_eSPI | ≥ 2.5.0 | CYD display driver |
| ArduinoJson | ≥ 6.21 | JSON for web API |
| WiFi | built-in | ESP32 WiFi |
| WebServer | built-in | HTTP server |
| Preferences | built-in | NVS storage |

### 8.3 WiFi Setup Procedure

1. Open serial terminal (115200 baud, LF line endings).
2. Edit firmware `setup()` to temporarily include:
   ```c
   prefs.putString("ssid", "YOUR_NETWORK_NAME");
   prefs.putString("pass", "YOUR_PASSWORD");
   ```
3. Flash firmware; once connected, IP will appear on CYD and serial.
4. Remove the putString lines; reflash. Credentials persist in NVS.

**Access point mode (no router):** If no WiFi is configured, the firmware can be modified to start in AP mode. Add to setup():
```c
WiFi.softAP("AntSwitch", "password123");
```
Clients connect to "AntSwitch" and access `http://192.168.4.1`.

### 8.4 Firmware Customization

**Changing number of ports (relay_switch_esp32.ino):**
```c
#define NUM_PORTS  8   // change from 4 to 8; add relay/LED/button GPIO assignments
```
Add GPIO entries to RELAY_PINS[], LED_PINS[], BTN_PINS[] arrays.

**Custom antenna names via web interface:**
Navigate to `http://<ip>/config` — form accepts names up to 15 characters. Saved to NVS immediately.

**MQTT integration (add to relay firmware):**
```c
#include <PubSubClient.h>
client.subscribe("antenna/set");   // subscribe to set command topic
client.publish("antenna/status", String(g_selected).c_str()); // publish state
```
This allows integration with Home Assistant or other automation platforms.

### 8.5 Serial Command Reference (All Firmware)

**relay_switch_esp32.ino:**
```
ANT <1-4>    Select antenna port
PORT <1-4>   Same as ANT
OFF          De-energize all relays
STATUS       Show current state
HELP         Command list
```

**motorized_switch_esp32.ino:**
```
MOVE <1-4>   Move to position
POS <1-4>    Same as MOVE
HOME         Execute homing sequence
STATUS       Position, state, homed flag
HELP         Command list
```

**band_decoder_esp32.ino:**
```
ANT <1-4>    Manual port select (exits auto mode)
AUTO         Return to automatic band tracking
SRC BCD      Set band data source to Yaesu BCD
SRC CIV      Set band data source to Icom CI-V
SRC VOLT     Set band data source to band voltage
MAP <band> <port>   Assign band to port (e.g., "MAP 20M 3")
STATUS       Band, port, mode, source
HELP         Command list
```

---

## CHAPTER 9 — BAND-BY-BAND QUICK REFERENCE

### 9.1 HF Bands

| Band | Freq | Relay Type | V_s Risk | Connector | Notes |
|------|------|-----------|---------|-----------|-------|
| 160M | 1.8 MHz | G2RL/CX-110D | Low (at 100W) | SO-239 | Long wire common |
| 80M | 3.5 MHz | G2RL/CX-110D | Low | SO-239 | |
| 40M | 7.0 MHz | G2RL/CX-110D | Low | SO-239 | |
| 30M | 10.1 MHz | G2RL/CX-110D | Low | SO-239 | WARC, 200W limit |
| 20M | 14.0 MHz | G2RL/CX-110D | Low | SO-239 | |
| 17M | 18.1 MHz | G2RL/CX-110D | Low | SO-239 | WARC |
| 15M | 21.0 MHz | G2RL/CX-110D | Low | SO-239 | |
| 12M | 24.9 MHz | G2RL/CX-110D | Low | SO-239 or N | WARC |
| 10M | 28.0 MHz | CX-110D | Low | N-type | |
| 6M | 50.0 MHz | CX-110D | Low | N-type | Use N-type for outdoor |

Note: "Low V_s risk" = peak RF voltage well below relay coil dropout voltage.  All values assume ≤ 100W with SWR ≤ 2:1.  For 1.5 kW, use CX-800 or vacuum relay throughout.

### 9.2 VHF/UHF Bands

| Band | Freq | Relay Type | IL (typ) | Isolation | Connector |
|------|------|-----------|---------|---------|-----------|
| 2M | 144 MHz | CX-110D or coaxial | < 0.1 dB | > 60 dB | N-type |
| 1.25M | 222 MHz | CX-110D | < 0.15 dB | > 55 dB | N-type |
| 70cm | 432 MHz | CX-110D | < 0.15 dB | > 50 dB | N-type |
| 33cm | 902 MHz | Specialized RF relay | < 0.3 dB | > 45 dB | N-type or SMA |
| 23cm | 1296 MHz | Specialized RF relay | < 0.5 dB | > 40 dB | SMA or N-type |

For 33cm and 23cm, standard relays are inadequate. Use purpose-built microwave switches (Dow-Key, RF-Lambda, or equivalent). Alternatively, use a manual SP2T switch for these bands — it is more reliable than an under-rated relay at 1296 MHz.

### 9.3 Antenna-to-Band Assignments

A typical 4-port switch assignment for a general HF/VHF station:

| Port | Antenna | Bands |
|------|---------|-------|
| 1 | 80M/160M doublet (or inverted L) | 160M, 80M |
| 2 | 40M dipole or wire | 40M, 30M |
| 3 | Tribander or trapped vertical | 20M, 17M, 15M, 12M, 10M |
| 4 | 6M/2M vertical or yagi | 6M, 2M |

Modify this table via the web interface or `MAP` serial command.

---

## CHAPTER 10 — INSTALLATION PROCEDURES

### 10.1 Indoor / Shack Installation (Relay/Motorized Switch)

**Location:** As close to the cable entry panel as possible. Minimize the length of coax between the entry panel arrestors and the switch.

**Power:** 12V DC at 1A minimum. Use a regulated linear supply (or good-quality switching supply with < 20 mV ripple). Poor power supply regulation causes relay chatter and ESP32 resets.

**Connections:**
1. Connect COMMON port to transceiver with the shortest practical coax run.
2. Connect each antenna port to its antenna feedline.
3. Install 47 kΩ static drain resistors on any unused ports before first use.
4. Connect SEND/TX-inhibit line if available on transceiver.
5. Apply power. Verify LED for expected starting port.

**Grounding:** Bond the switch chassis to the station RF ground bus with 1/2" copper strap, shortest practical path. The chassis must be at station ground potential to maintain port isolation specifications.

### 10.2 Outdoor / Mast Installation (Weatherproof)

**Enclosure selection:**
- 3D-printed ASA: IP54 with gasket — adequate for protected outdoor locations (under eaves, inside antenna mast enclosure)
- Die-cast aluminum (Hammond 1590 series): IP65 or better — suitable for direct weather exposure
- For tower installations: IP67 minimum (condensation, driving rain, ice)

**Cable entry:** Use weathertight glands (PG-type or NPT-thread). Apply self-amalgamating tape over all connector flanges and glands after final assembly. Apply a downward drip loop on all coax runs entering the enclosure.

**Mounting:**
- Mast clamp bracket (from mounting_brackets_sw.scad): U-bolt to round mast
- Tower leg clamp: wrap bracket for square tower legs
- Wall mount: lag-bolt to exterior wall studs through wall mount bracket

**Vent/drain:** The enclosure drain hole (4 mm) points downward after mounting. Verify orientation before final tightening.

**Power and control wiring:** Outdoor control wiring runs inside weatherproof conduit or uses direct-burial rated cable. Install lightning protection on all control lines entering the building (see TM-LSP-001).

### 10.3 Band Data Connection (Auto Switch)

**Yaesu BCD:**
1. Identify BAND DATA connector on transceiver (consult radio manual).
2. Build or purchase a 6-pin DIN cable.
3. Connect B1–B4 to switch through the voltage divider (3.3kΩ/6.8kΩ) network.
4. Connect SEND to switch TX-inhibit input.
5. Connect GND.
6. Test: tune to each band; verify the switch selects the correct antenna.

**Icom CI-V:**
1. CI-V is a single wire (+pull-up). Connect radio CI-V DATA to ESP32 GPIO16 via 10kΩ pull-up to 3.3V and 1kΩ series resistor.
2. Set radio CI-V baud rate: Menu → CI-V → Baud Rate → match to firmware Serial2.begin() setting.
3. The firmware listens passively; no commands are sent to the radio.

---

## CHAPTER 11 — TESTING AND ALIGNMENT

### 11.1 Continuity Test (Pre-RF)

Before applying RF, verify signal continuity through each switch position:

1. Set DMM to resistance mode.
2. Touch probes to COMMON center pin and Antenna 1 center pin.
3. Advance switch to position 1.
4. Expected: < 0.1 Ω (direct contact resistance).
5. Advance to position 2: expected > 1 MΩ (open circuit for unselected port).
6. Repeat for all port combinations.
7. Any reading near zero in an unselected position = short circuit (wiring fault, failed relay).

### 11.2 Insertion Loss Measurement (NanoVNA)

1. Calibrate NanoVNA in 2-port mode at the connectors that will attach to the switch (SOLT calibration).
2. Connect COMMON port to Port 1; selected Antenna port to Port 2.
3. Set frequency span from 1.8 MHz to max operating frequency.
4. Measure S21 (forward transmission gain, which will show as a small negative number = loss in dB).
5. Verify |S21| < specification at all frequencies.
6. Repeat for each port position.

**Expected results:**

| Position | Freq Range | Expected S21 | Pass/Fail |
|----------|-----------|-------------|-----------|
| ANT 1 | 1.8–30 MHz | > −0.1 dB | |
| ANT 1 | 50–144 MHz | > −0.2 dB | |
| ANT 2 | same | same | |
| (all ports) | | | |

### 11.3 Isolation Measurement

1. Connect NanoVNA Port 1 to the COMMON port.
2. Advance switch to position 1 (ANT 1 selected).
3. Connect NanoVNA Port 2 to ANT 2 (an unselected port).
4. Measure S21 = forward transmission from COMMON to unselected ANT 2.
5. Verify S21 < −60 dB at HF, < −50 dB at VHF.
6. A poor result (< −40 dB) indicates inadequate shielding or relay contact capacitance.

### 11.4 Power Test (Relay Switch)

After bench tests pass, apply RF power to verify no unexpected heating or relay dropout:

1. Connect antenna to ANT 1. Connect transmitter to COMMON.
2. Apply 25W CW for 60 seconds. Check all relay bodies for unusual heat.
3. Apply 100W CW for 30 seconds. Verify relay remains energized (measure coil voltage; should hold at 12V ± 10%).
4. If the relay drops out under high RF: the RF field is inducing current in the coil wiring, causing dropout. Add RF bypass capacitors (1 nF) on coil leads to chassis.

### 11.5 Relay Contact Check After Use

Measure contact resistance annually:
1. Apply 12V to energize relay.
2. Ohmmeter between COMMON center pin and selected ANT center pin.
3. Expected: < 10 mΩ.
4. If > 50 mΩ: relay contact oxidation. Cycle relay 20–50 times (apply/remove coil voltage). Re-measure.
5. If still > 50 mΩ: replace relay.

---

## CHAPTER 12 — TROUBLESHOOTING

### 12.1 Switch Does Not Select Antenna

| Symptom | Likely Cause | Solution |
|---------|-------------|---------|
| No relay click when button pressed | ULN2003A output not reaching relay | Check 12V supply; measure ULN output with DMM |
| Relay clicks but no RF continuity | Relay contact failure (normally open stuck open) | Replace relay |
| Position selected but wrong antenna routes | Relay wiring error (COM/NO/NC swapped) | Trace wiring; swap NO and NC connections |
| Switch reverts after seconds | Relay coil thermal dropout | Check coil supply voltage under load |

### 12.2 High Insertion Loss After Installation

| Symptom | Cause | Solution |
|---------|-------|---------|
| IL high on all bands, all ports | Oxidized relay contacts | Clean contacts; cycle relay 50× |
| IL high above 50 MHz | Relay IL spec exceeded (wrong relay type) | Replace with coaxial relay (CX-110D) |
| IL high at one port only | Partial contact (spring tension low) | Adjust contact pressure or replace relay |
| IL fine with ohmmeter but bad with NanoVNA | Impedance mismatch in relay PCB layout | Verify 50Ω trace geometry; check ground plane |

### 12.3 Poor Isolation

| Symptom | Cause | Solution |
|---------|-------|---------|
| Isolation < 40 dB at HF | Missing static drain resistors; common ground coupling | Add 47kΩ per unselected port; check RF ground |
| Isolation degrades above 30 MHz | Open contact capacitance too high | Use lower-capacitance relay or coaxial relay |
| Isolation < 30 dB at any frequency | Internal enclosure coupling | Add metal dividers between relay bays |

### 12.4 ESP32 / Firmware Issues

| Symptom | Cause | Solution |
|---------|-------|---------|
| ESP32 resets under RF | RF pickup on power supply leads | Add ferrite bead + 100 nF on each supply lead to ESP32 |
| Web interface unreachable | WiFi not configured, or SSID changed | Check serial console for IP; reconfigure credentials |
| CYD display blank | Backlight not enabled | Verify GPIO21 HIGH; check TFT_eSPI User_Setup.h |
| Relay fires without command | RF rectification on GPIO | Add 100 pF to GND on each ULN2003A input |

### 12.5 Motorized Switch Faults

| Symptom | Cause | Solution |
|---------|-------|---------|
| Motor runs but switch doesn't move | Shaft coupler slipping | Tighten set screws; add Loctite 243 |
| Motor stalls before reaching position | Insufficient torque | Check 5V supply; try 6V (within motor spec); reduce step rate |
| Hall sensor not detected | Wrong GPIO, magnet misaligned | Verify magnet-to-sensor gap ≤ 2mm; rotate magnet orientation |
| Position drifts over many cycles | Step loss under load | Enable Hall confirmation on every move; re-home if drift detected |

---

## APPENDIX A — PARTS LIST MASTER

### Manual Rotary Switch

| Item | Qty | Description | Source |
|------|-----|-------------|--------|
| SW-SP4 | 1 | SP4T rotary switch, ceramic, silver contacts | Grayhill 56D, Alpha SR2912 |
| SW-SP6 | 1 | SP6T rotary switch, ceramic | Grayhill 56D |
| SO239 | 5–7 | SO-239 silver panel mount | Amphenol UG-58A/U |
| N-F | 5–7 | N-type female panel, IP67 | Amphenol 82-836 |
| KNOB | 1 | Pointer knob, 6mm shaft | Davies Molding 1100 |
| R_STAT | 6 | 47kΩ 1/4W metal film (static drain) | Mouser |
| ENCL | 1 | 130×120×45mm die-cast Al | Hammond 1590C |
| GSKT | per conn | Silver RF washer, per SO-239 flange | |

### Relay Remote Switch

| Item | Qty | Description | Source |
|------|-----|-------------|--------|
| K1–K4 | 4 | Omron G2RL-1A4-E-TV8 (12V SPDT) | Mouser |
| K_VHF | 4 | Tohtsu CX-110D (VHF coaxial relay) | DX Engineering |
| K_HP | 4 | Tohtsu CX-800 (2kW HF relay) | DX Engineering |
| ULN | 1 | ULN2003A driver IC | Mouser |
| ESP32 | 1 | ESP32-WROOM-32 / DevKit | Mouser |
| CYD | 1 | ESP32-2432S028 2.8" TFT | AliExpress |
| REG12 | 1 | LM7812 or LM2596 | Mouser |
| REG33 | 1 | AMS1117-3.3 | Mouser |
| R_LED | 8 | 470Ω 1/4W LED resistors | Mouser |
| LED | 5 | 5mm diffused LED, assorted colors | Mouser |
| BTN | 5 | 6mm tactile switch | Mouser |
| CAP | 8 | 100µF 25V + 0.1µF ceramic | Mouser |
| J_12V | 1 | 2.1mm barrel jack | Mouser |

### Motorized Switch (additional parts)

| Item | Qty | Description | Source |
|------|-----|-------------|--------|
| MOTOR | 1 | 28BYJ-48 stepper + ULN2003A module | AliExpress |
| HALL | 4 | A3144 Hall sensor TO-92 | Mouser |
| MAGNET | 4 | 3×1.5mm neodymium disc | AliExpress |
| COUPLE | 1 | 3D printed shaft coupler 5→6mm | Print from SCAD |
| BUCK | 1 | LM2596-5 to 5V (motor supply) | Mouser |

### Auto Band Switch (additional parts)

| Item | Qty | Description | Source |
|------|-----|-------------|--------|
| DIN6 | 1 | 6-pin DIN socket (Yaesu band data) | Mouser |
| J_CIV | 1 | 3.5mm mono jack (Icom CI-V) | Mouser |
| R_DIV | 8 | 3.3kΩ + 6.8kΩ per BCD pin | Mouser |
| D_CLAMP | 4 | BAT42 Schottky clamp diode | Mouser |

---

## APPENDIX B — BAND DATA INTERFACE PINOUTS

### Yaesu 6-pin DIN (Band Data Connector)

```
Pin 1: BAND DATA A (B1, LSB)     — logic output
Pin 2: BAND DATA B (B2)           — logic output
Pin 3: BAND DATA C (B4)           — logic output
Pin 4: BAND DATA D (B8, MSB)      — logic output (FT-2000, FTDX series)
Pin 5: SEND (TX active)           — output; HIGH = transmitting
Pin 6: GND
Pin 7: +5V (accessory supply)    — 200 mA max
Pin 8: TX GND
```

DIN connector type: 8-pin round DIN (DIN 45326 / IEC 60130-9), or
6-pin DIN on older models. Verify with specific radio service manual.

### Icom CI-V (Remote Jack)

```
3.5mm stereo jack:
  Tip:    CI-V data (single-wire, open-collector, bidirectional)
  Ring:   Not used (or GND on some models)
  Sleeve: GND

Electrical: 5V pull-up internal to radio; open-collector bus.
Protocol:   Asynchronous serial, 8N1, 1200 or 9600 baud.
```

### Generic Band Voltage Output

Varies by manufacturer and model. Typical range: 0V (lowest band) to 3.3V or 5V (highest band) in equal steps. Measure with DMM on each band and compile a voltage table. Enter values in firmware `BAND_VOLTAGES[]` array.

---

## APPENDIX C — RF PERFORMANCE CALCULATIONS

### C.1 Insertion Loss Budget for Relay Switch

```
Total IL = IL_connector_pair + IL_relay_contact + IL_PCB_trace

IL_connector (SO-239, silver, 14 MHz): < 0.01 dB per pair
IL_relay contact (G2RL at 14 MHz):       < 0.05 dB
IL_PCB trace (50Ω, 50mm length at 14 MHz): < 0.001 dB
─────────────────────────────────────────────────────
Total at 14 MHz:                          < 0.07 dB ✓

At 144 MHz:
IL_connector:     < 0.02 dB
IL_relay (G2RL):  < 0.20 dB (degrades at VHF; use CX-110D instead)
IL_relay (CX-110D): < 0.10 dB
IL_PCB trace:       0.01 dB
─────────────────────────────────────────
Total with G2RL:    < 0.23 dB (marginal)
Total with CX-110D: < 0.13 dB ✓
```

### C.2 Peak Voltage vs. Transmit Power

| Power (W) | SWR | V_peak (V) | I_peak (A) |
|-----------|-----|-----------|-----------|
| 5 | 1:1 | 22.4 | 0.45 |
| 100 | 1:1 | 100 | 2.0 |
| 100 | 3:1 | 173 | 3.46 |
| 500 | 1:1 | 224 | 4.47 |
| 500 | 3:1 | 387 | 7.75 |
| 1000 | 1:1 | 316 | 6.32 |
| 1000 | 3:1 | 548 | 10.95 |
| 1500 | 1:1 | 387 | 7.75 |
| 1500 | 3:1 | 670 | 13.4 |
| 2000 PEP | 1:1 | 447 | 8.94 |
| 2000 PEP | 3:1 | 775 | 15.5 |

---

## APPENDIX D — FILE INDEX

### Schematic Files
`/home/merv/Public/claude/ant_gear/ant_switches/schematics/`

| File | Content |
|------|---------|
| `sch_manual_rotary_switch.txt` | SP2T/SP4T/SP6T theory; contact ratings; isolation analysis; static drain; parts list |
| `sch_relay_remote_switch.txt` | Relay selection; binary-tree vs. one-relay-per-port topology; ULN2003A driver; power budget; 8-port extension |
| `sch_motorized_switch.txt` | 28BYJ-48 stepper drive; half-step sequence; Hall sensor wiring; motor protection; shaft coupling |
| `sch_auto_band_switch.txt` | Yaesu BCD decode table; Icom CI-V framing; band voltage table; TX inhibit; band-to-antenna mapping |
| `sch_high_power_switch.txt` | 1kW/2kW power analysis; CX-800 coaxial relay; Jennings vacuum relay; isolation improvement techniques |

### Firmware Files
`/home/merv/Public/claude/ant_gear/ant_switches/firmware/`

| File | Function |
|------|---------|
| `relay_switch_esp32.ino` | 4-port relay switch: CYD display, web UI, push buttons, TX inhibit, serial commands, NVS persistence |
| `motorized_switch_esp32.ino` | Stepper motor control: homing, Hall sensor decoding, shortest-path routing, non-blocking step loop |
| `band_decoder_esp32.ino` | Auto band switching: BCD/CI-V/voltage decode, band-to-antenna map, TX inhibit, web config |

### OpenSCAD Files
`/home/merv/Public/claude/ant_gear/ant_switches/openscad/`

| File | Modules |
|------|---------|
| `enclosure_ant_switch.scad` | SP4T body+lid, SP6T body, relay switch body+lid, motor switch body+lid, panel inserts (SO-239/N-type), shaft coupler, motor bracket |
| `mounting_brackets_sw.scad` | Mast clamp, wall mount, tower leg clamp, desk bracket, 1U rack panel, cable dress bar, label strip, knob, LED bezel, remote panel faceplate |

---

```
END OF MANUAL

TM-ANT-SW-001  Rev A
Coaxial Antenna Switches for Amateur Radio Stations
Classification: Unclassified / Amateur Radio Educational Material

Author: Station Documentation System
Date: April 2026
```
