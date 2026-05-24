================================================================================

                   REMOTE ELEMENT SWITCHING SYSTEM
                        COMPREHENSIVE MANUAL
                    VHF/UHF YAG ANTENNA ARRAY CONTROL
                            Edition 1.0
                            April 2026

              DISTRIBUTION AUTHORIZED TO LICENSED RADIO OPERATORS
                     AND ENGINEERING RESEARCH PERSONNEL

================================================================================

                            TABLE OF CONTENTS

  SECTION 1.0   SCOPE AND APPLICABILITY
  SECTION 2.0   THEORY OF OPERATION
  SECTION 3.0   SYSTEM DESCRIPTION
  SECTION 4.0   EQUIPMENT AND MATERIALS
  SECTION 5.0   INSTALLATION AND ALIGNMENT
  SECTION 6.0   CALIBRATION AND INITIAL ALIGNMENT
  SECTION 7.0   OPERATION
  SECTION 8.0   TROUBLESHOOTING
  SECTION 9.0   MAINTENANCE
  SECTION 10.0  PARTS LIST AND PROCUREMENT
  APPENDIX A    NEC2 ANTENNA MODELS
  APPENDIX B    BEAM PATTERN CHARTS
  APPENDIX C    RF ISOLATION DESIGN TABLES
  APPENDIX D    CONNECTOR TORQUE AND SEALING SPECIFICATIONS

================================================================================

SECTION 1.0  SCOPE AND APPLICABILITY

1.1 PURPOSE

This manual provides complete technical documentation for the Remote Element
Switching System (RESS), a multi-band antenna control platform for VHF/UHF
amateur radio yagi arrays. The system enables remote parasitic element
switching via electronic relays (6M, 2M) or PIN diodes (1.25M through 20cm)
to dynamically reconfigure antenna beam patterns without mechanical rotation.

1.2 FREQUENCY BANDS SUPPORTED

    Band            Frequency Range    Wavelength    Switching Method
    ──────────────────────────────────────────────────────────────────
    6 Meters        50.0 - 50.3 MHz    5.98 m        Relay (G5V-2)
    2 Meters        144.0 - 148.0 MHz  2.08 m        Relay (G5V-2)
    1.25 Meters     222.0 - 225.0 MHz  1.35 m        PIN Diode (BAP51-02)
    70 Centimeters  430.0 - 450.0 MHz  0.69 m        PIN Diode (MA4P504)
    33 Centimeters  902.0 - 928.0 MHz  0.33 m        PIN Diode (SMP1320)
    20 Centimeters  1296.0-1300.0 MHz  0.23 m        PIN Diode (SMP1320)

1.3 SYSTEM CONFIGURATION OPTIONS

Configuration A: BASIC (Single Band, 3-Element)
    - Reflector + Driven + 1 Director (field-portable)
    - 1 Relay OR 1 PIN Diode per element
    - Simple serial or WiFi control
    - Cost: $50-100 depending on band

Configuration B: STANDARD (Single Band, 5-Element)
    - Reflector + Driven + 3 Directors (fixed installation)
    - 4 Relays OR 4 PIN Diodes
    - WiFi + BLE Mesh control
    - Touchscreen UI optional
    - Cost: $150-250

Configuration C: ADVANCED (Multi-Band Capable)
    - Complete 6-band system (6M through 20cm)
    - Array of switching elements for each band
    - BLE Mesh networked element nodes
    - Automated beam pattern selection by frequency
    - Cost: $1000-2000 (complete system)

1.4 INTENDED USERS

This manual is intended for:
    - Radio operators (Technician class and above)
    - Radio engineers and technicians
    - University engineering programs
    - Radio club technical committees
    - Commercial radio systems integrators

Familiarity with the following is assumed:
    - Basic electronics (voltage, current, impedance)
    - Antenna theory (resonance, impedance matching, radiation patterns)
    - Amateur radio operating practices
    - Embedded systems programming (for firmware customization)

1.5 SAFETY CONSIDERATIONS

WARNING: This system operates RF power levels that may exceed FCC/ICNIRP
exposure limits. Ensure proper RF safety measures:

    - Place antenna > 2 meters from operators during transmission
    - Do NOT operate near metallic objects or other antennas
    - Check antenna SWR before each operating session
    - Use appropriate RF shielding for control cables
    - Verify proper grounding of all RF equipment

DO NOT attempt to repair high-voltage circuits while powered. Always
disconnect power before service.

================================================================================

SECTION 2.0  THEORY OF OPERATION

2.1 PARASITIC ELEMENT COUPLING FUNDAMENTALS

In a yagi antenna array, parasitic elements (reflectors and directors)
interact with the driven element via electromagnetic coupling. The coupled
impedance depends on:

    1. Element length relative to resonance
    2. Element-to-element spacing along boom
    3. Whether element is isolated (open) or shorted to boom (grounded)

Reflector Mode: Element is shorted to boom conductor (low impedance short)
    - Effective electrical length: ~5% longer than resonance
    - Acts as reflecting surface, increases backward impedance
    - Reduces forward gain but increases F/B ratio
    - Effective when element = 0.494λ (longer than resonance)

Director Mode: Element is free/isolated (open circuit)
    - Effective electrical length: ~5% shorter than resonance
    - Acts as focusing element, increases forward impedance
    - Increases forward gain and narrows beamwidth
    - Effective when element = 0.449λ (shorter than resonance)

Switching Implementation:
    - Relay contact shorts element to boom (reflector mode)
    - Relay open circuit leaves element free (director mode)
    - PIN diode forward bias effectively shorts element (reflector)
    - PIN diode reverse bias isolates element (director mode)

2.2 PIN DIODE SWITCHING PHYSICS

PIN diodes operate as voltage-controlled RF switches by modulating the
depletion region width via applied DC bias voltage.

Forward Bias (Conducting State):
    - Applied voltage: +5V to +12V DC (typical 10-20 mA current)
    - Depletion region collapses, acts as low resistance (0.5-2 Ω)
    - Insertion loss: 0.3-0.8 dB (depends on device and frequency)
    - Element appears shorted to boom, acts as reflector
    - Switching time: typically < 30 nanoseconds

Reverse Bias (Blocking State):
    - Applied voltage: 0V or -5V DC (< 1 µA leakage current)
    - Depletion region expands, high impedance (> 10 kΩ)
    - Isolation: > 30 dB at operating frequency
    - Element appears open-circuited, acts as director
    - Switching time: slightly slower than forward (due to charge storage)

Device Selection by Band:
    - 222 MHz: BAP51-02 (0.5 pF, 1.8 Ω series resistance)
    - 432 MHz: MA4P504 (0.6 pF, 1.0 Ω series resistance)
    - 902 MHz: SMP1320 (0.63 pF, 1.0 Ω series resistance)
    - 1296 MHz: SMP1320 (same, lower parasitic inductance in microstrip)

Temperature Effects:
    - Impedance varies ~0.3%/°C for most PIN diodes
    - At UHF frequencies, minimize loop area of bias network (< 1 mm)
    - Microstrip implementation on PCB preferred for 1296 MHz (< 0.1 mm loops)

2.3 RELAY SWITCHING FOR HF/VHF

At VHF frequencies (144-222 MHz), small relay contact inductance is
acceptable. Contact leads exhibit ~0.5-2 nH inductance per pair.

Relay Contact Impedance at Operating Frequency:
    Z_contact = R_contact + j*ω*L_contact

At 50 MHz:   Z ≈ 100 mΩ + j*7.5 Ω (acceptable, < 0.1 dB loss)
At 144 MHz:  Z ≈ 100 mΩ + j*22 Ω (marginal, ~0.2 dB loss)
At 432 MHz:  Z ≈ 100 mΩ + j*65 Ω (UNACCEPTABLE, > 5 dB loss + reactance)

Relay Selection for Lower Bands:
    - Omron G5V-2: DPDT, 5V coil, 40 mA, gold-plated contacts
    - Contact bounce: ~1 ms (acceptable with 50 ms settling delay)
    - RF loss @ 50 MHz: < 0.15 dB
    - RF loss @ 144 MHz: < 0.25 dB
    - Cost: ~$2 per relay (qty 100)

Relay Driver Considerations:
    - 3V3 ESP32 GPIO output cannot directly drive 5V relay coil
    - Use ULN2803 Darlington array (8 channels, 500 mA per channel)
    - Alternative: Individual 2N2222 transistor per relay
    - Flyback diode (1N4007) across coil protects driver from voltage spikes

2.4 BEAM PATTERN CONTROL STRATEGIES

Five fundamental switching strategies exist for yagi beam modification:

Strategy 1: DIRECTOR IN/OUT
    Add/remove directors to control gain/beamwidth
    Effect: Gain varies 6.5 dBd (3-el) to 9.2 dBd (5-el)
    Beamwidth: Narrows from 45° to 25° as directors added

Strategy 2: REFLECTOR OPTIMIZATION
    Tune reflector impedance by varying length or spacing
    Effect: F/B ratio varies 8 dB to 22 dB
    Limited by element resonance; switching only achieves coarse control

Strategy 3: REVERSIBLE YAGI (Bi-directional Beam)
    Swap reflector/director roles via dual-element shorted stubs
    Effect: Can reverse beam 180° with single relay
    Cost: Extra elements (5-element → dual 3-element sides)
    Trade-off: Reduced gain in each direction (5 dBd vs 9 dBd)

Strategy 4: CARDIOID PATTERNS (8-point directional)
    Selectively ground different elements to create null patterns
    Effect: 8 cardioid directions (N, NE, E, SE, S, SW, W, NW)
    Gain: ~7 dBd, Null depth: 15-20 dB
    Elements: Each of 4 directors independently switchable

Strategy 5: PHASED ELEMENT CONTROL (Advanced)
    Vary phase relationship between elements via transmission line stubs
    Effect: Electronic beam steering (azimuth only, no mechanical rotation)
    Complexity: Requires variable-length delay lines (λ/4 stub at each element)
    This manual focuses on Strategies 1-4; phasing is addressed in Appendix C

2.5 RF ISOLATION OF CONTROL LINES

RF signal leakage through control lines can severely distort antenna
patterns, particularly at UHF frequencies where wavelengths are short.

Coupling Mechanisms:
    1. Capacitive coupling from RF element to bias line (>1 pF at UHF)
    2. Magnetic field coupling from element current to control loop
    3. Common-mode radiation on control cable shield
    4. Ground loop between boom ground and controller ground

Mitigation Techniques:
    a) Ferrite chokes on each control line (100 Ω @ 432 MHz minimum)
    b) Twisted-pair shielded cable for remote control runs
    c) Shield connected to boom ground ONLY at antenna (float at control end)
    d) Separate RF and logic ground planes on control PCB
    e) Minimize control line loop area (< 1 mm² at 1 GHz)
    f) Bypass capacitors (100 nF) at control entry point to boom

Expected Pattern Improvement:
    Without ferrite filtering: F/B degrades 5-10 dB when control energized
    With ferrite beads only: F/B degradation < 1 dB
    With ferrites + toroid + twisted pair: F/B degradation < 0.5 dB

2.6 SWR AND IMPEDANCE MATCHING

Remote element switching changes the driven element impedance, requiring
impedance matching adjustment. Typical changes:

Switching reflector ON/OFF:  ΔZ ≈ ±5 - 10 Ω (shifts real part)
Adding director:              ΔZ ≈ ±2 - 5 Ω (shifts reactive part)

Impedance matching must be broadband enough to accommodate these shifts:

Gamma Match: Tuned for baseline configuration
    - Capacitive tap compensates ~5-10 Ω reactance shift
    - SWR rise with switching: 1.0 → 1.3-1.5:1

Hairpin/Folded Dipole Match: Inherently broader
    - Handles impedance shifts with < 0.2 dB loss
    - SWR rise with switching: 1.0 → 1.1-1.2:1
    - Recommended for multi-pattern systems

50Ω Transmission Line Stub: Brute-force broadband
    - Minimize stub length (< λ/8) to minimize loss
    - SWR stays < 1.5:1 across switching patterns

Recommended Approach:
    1. Measure impedance change via NanoVNA in each pattern
    2. Design gamma match to center between min/max impedances
    3. If SWR swing > 1.5:1, use hairpin or stub match instead

================================================================================

SECTION 3.0  SYSTEM DESCRIPTION

3.1 BLOCK DIAGRAM: COMPLETE SYSTEM ARCHITECTURE

        ┌─────────────────────────────────────────────────┐
        │                  OPERATORS                       │
        │  (via CYD Touchscreen, WiFi App, or Serial CLI) │
        └────────────────────┬────────────────────────────┘
                             │
                    Commands: Pattern, Band
                             │
        ┌────────────────────▼────────────────────────────┐
        │  MAIN CONTROLLER (ESP32 + CYD 3.5" Display)     │
        │  - WiFi WebServer (192.168.4.1:80)              │
        │  - BLE Mesh Provisioner (AFM vendor opcodes)    │
        │  - Pattern storage (12 patterns per band)        │
        │  - SWR monitoring (AD8307 power detector)        │
        └────────┬──────────────────────────┬─────────────┘
                 │                          │
        BLE Mesh Opcode Broadcast    WiFi UDP/TCP
        AFM_OP_ELEMENT_SET           Remote monitoring
                 │                          │
        ┌────────▼──────────────────────────▼─────────────┐
        │  BLE MESH NETWORK (Private Group 0xFFFF)        │
        │  Provisioning: CYD Launcher → Element Nodes      │
        └────────┬───────────────────────────────────────┬─┘
                 │ AFM opcodes                           │
    ┌────────────▼──────────┐              ┌─────────────▼──────────┐
    │  ELEMENT NODE 0       │              │  ELEMENT NODE 1-3      │
    │  (Reflector)          │              │  (Directors 1-3)       │
    │  GPIO2 → Control Out  │              │  GPIO2 → Control Out   │
    │  ADC34 → RF Current   │              │  ADC34 → RF Current    │
    │  Status LED (GPIO4)   │              │  Status LED (GPIO4)    │
    └────────────┬──────────┘              └─────────────┬──────────┘
                 │                                      │
    ┌────────────▼──────────────────────────────────────▼────────────┐
    │  RF SWITCHING NETWORK (Element-by-Element)                     │
    │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐          │
    │  │ Relay/PIN #0 │  │ Relay/PIN #1 │  │ Relay/PIN #2 │  ...    │
    │  │ Controls:    │  │ Controls:    │  │ Controls:    │          │
    │  │ Reflector    │  │ Director 1   │  │ Director 2   │          │
    │  │ Element      │  │ Element      │  │ Element      │          │
    │  └──────────────┘  └──────────────┘  └──────────────┘          │
    └────────────┬──────────────────────────────────────┬─────────────┘
                 │                                      │
    ┌────────────▼──────────────────────────────────────▼────────────┐
    │  ANTENNA ARRAY (YAG, ELEMENT STATE VARIABLE)                   │
    │  Boom: ▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬ (center-fed at DE)                    │
    │  Ref:  ═════ (switchable reflector)                            │
    │        ───── DE (driven element, fixed)                        │
    │        ───── Dir1 (switchable, variable spacing)               │
    │        ─── Dir2 (switchable, variable spacing)                 │
    │        ──  Dir3 (switchable, variable spacing)                 │
    └────────────┬────────────────────────────────────────────────────┘
                 │
    ┌────────────▼────────────────────────────────────────────────────┐
    │  RF MEASUREMENT & FEEDBACK                                      │
    │  - Directional Coupler (fwd/ref power)                          │
    │  - RF Current Probe (ACS712, per element)                       │
    │  - Temperature Sensor (NTC thermistor)                          │
    │  - Data reported to main controller every 30s via AFM opcode    │
    └─────────────────────────────────────────────────────────────────┘


3.2 CONTROLLER SUBSYSTEM (Main ESP32 + CYD)

Hardware:
    - MCU: ESP32-WROOM-32 (dual-core, 160 MHz, 4 MB flash)
    - Display: CYD 2432S028 (480×320 TFT, 3.5", WiFi, BT)
    - Storage: 4 MB SPI flash (factory partitioned: 2 MB app, 2 MB NVS)
    - Power: 5V @ 2A USB-C (charging supported)
    - Connectivity: 802.11b/g/n WiFi, BLE 5.0 dual-mode

Software Stack:
    - Base: Arduino IDE 2.x + ESP32 board manager v2.0.14+
    - BLE Mesh: esp-idf-lib or Arduino's native BLE implementation
    - UI: Adafruit GFX + ST7789 display driver
    - Web: Arduino WebServer library
    - JSON: ArduinoJson (for REST API)
    - Storage: Preferences library (NVS wrapper)

Control Inputs (4):
    - WiFi REST API: POST /control?band=6m&pattern=baseline
    - BLE Mesh: From provisioned element nodes (bidirectional)
    - USB Serial: Text-based commands (p0-11, s, l, h)
    - Touchscreen: 12-button pattern matrix, 6-button band selector

Status Outputs:
    - CYD Display: Real-time beam pattern, SWR, power, element states
    - WiFi JSON API: {"band":"6m", "pattern":"baseline", "swr":1.2, ...}
    - Serial terminal: Formatted text status (115200 baud, 8N1)
    - Status LED: Blue = online, Red = error

3.3 ELEMENT NODE SUBSYSTEM (Per-Element ESP32)

Hardware (each node):
    - MCU: ESP32 module (cost-optimized, 80 MHz single-core variant OK)
    - GPIO outputs: Relay coil or PIN diode bias (5V/12V via external driver)
    - ADC inputs: RF current sensor (ACS712-5A), temperature (NTC)
    - Status LEDs: Blue (online), Red (error/over-current)
    - BOOT button: Physical reset for provisioning

Software Stack:
    - Base: Arduino IDE 2.x + ESP32 board manager
    - BLE Mesh: Full mesh node with vendor model support
    - No display; status via LEDs and RF current measurement
    - Heartbeat broadcast every 30 seconds

Control Inputs:
    - BLE Mesh AFM opcodes (from main controller broadcast)
    - Local override: BOOT button long-press (reset to OFF state)

Status Outputs:
    - BLE Mesh AFM_OP_ELEMENT_STATUS message (30s heartbeat)
    - Status LED: Blue slow = waiting, Blue fast = online, Red = error
    - RF current measured and logged (for diagnostics)

Provisioning:
    - CYD Launcher (main controller in provisioner mode) discovers node
    - Node UUID: 0x43 0x59 (ASCII "CY") + 6 bytes from BT MAC
    - AppKey added to node, subscription to group 0xFFFF configured
    - Node assigned element index (0-3) based on discovery order


3.4 RF SWITCHING NETWORK (Per-Band Implementation)

For 6M/2M Bands (Relay Switching):
    ┌─────────────────────────────────────────┐
    │ From Driven Element (50Ω coax)          │
    │        │                                │
    │     [C_iso 1µF]  ← Blocks DC to boom    │
    │        │                                │
    │        o─────┬───────────────► To antenna feed
    │        │     │                          │
    │     [K_relay]│ NC = reflector           │
    │        │     │ NO = director            │
    │        │     │                          │
    │     [GND]───┴────────────────► Boom GND │
    │                                         │
    │ Relay: Omron G5V-2 DPDT                │
    │ Driver: ULN2803 (8 relays shared)      │
    │ Power: 5V @ 200 mA total                │
    └─────────────────────────────────────────┘

For 70cm/33cm/20cm Bands (PIN Diode Switching):
    ┌─────────────────────────────────────────┐
    │ From Driven Element (50Ω coax)          │
    │        │                                │
    │     [C_iso 1µF]                         │
    │        │                                │
    │        o─────[RFC 47nH]─┬──────► To antenna
    │        │   (SMD 0402)    │               │
    │     [GND]         [PIN Diode D1]        │
    │                   MA4P504 SOT-23        │
    │                        │                │
    │                   [C_bypass 100pF]      │
    │                        │                │
    │                   [R_limit 1.5kΩ]      │
    │                        │                │
    │              [2N2222 Bias Driver]       │
    │                        │                │
    │              +12V or GND (from GPIO)    │
    │                                         │
    │ PIN Diode: MA4P504 (432 MHz example)   │
    │ Bias Voltage: +12V (forward), 0V (off) │
    │ Power: ~10-15 mA per element            │
    └─────────────────────────────────────────┘

3.5 SYSTEM CONFIGURATION PARAMETERS (Settable via Firmware)

    Band Enum:       0 = 6M, 1 = 2M, 2 = 1.25M, 3 = 70CM, 4 = 33CM, 5 = 20CM
    Pattern Enum:    0-11 (OMNI through F/B-MAX)
    Element State:   0 = OFF, 1 = REFLECTOR, 2 = DIRECTOR, 3 = BYPASS
    SWR Threshold:   Default 2.0:1 (for pattern validity check)
    Relay Settle:    Default 50 ms (contact bounce delay)
    Sensor Update:   Default 1 second (SWR/power measurement interval)
    Heartbeat:       Default 30 seconds (element node status broadcast)
    WiFi AP SSID:    "BeamController" (default, changeable)
    WiFi AP Password:"hamradio" (default, changeable)

================================================================================

SECTION 4.0  EQUIPMENT AND MATERIALS

4.1 BILL OF MATERIALS FOR BASIC 6M SYSTEM (3-ELEMENT, RELAY SWITCHING)

Item  Part Number           Description            Qty  Value    Cost
────────────────────────────────────────────────────────────────────────
 1    ESP32-WROOM-32         MCU module              1   4MB      $8
 2    Omron G5V-2-5          Relay DPDT 5V          1   40mA     $2
 3    ULN2803APC             Darlington array       1   8-ch     $0.40
 4    1N4007                 Flyback diode          1   1A/1kV   $0.05
 5    C315C105M5U            Capacitor              1   1µF/50V  $0.15
 6    100nF 0402             Capacitor              4   100nF    $0.08
 7    1000µF elect.          Capacitor              1   1000µF   $0.20
 8    DB9M connector         9-pin DConnector       1   -        $0.30
 9    1kΩ ±5% resistor       1/4W metal-film        4   1kΩ      $0.08
10    PCB FR-4               Single-sided, 2"x3"   1   50mil    $1.50
11    USB-to-Serial          FTDI FT232RL           1   5V       $3
12    Header pins            2.54mm 40-pin strip   1   -        $0.20

TOTAL COMPONENT COST (qty 1):                             ~$16-20


4.2 ADDITIONAL COMPONENTS FOR 5-ELEMENT ARRAY (RELAY VERSION)

Item  Description                                   Add'l Qty  Add'l Cost
────────────────────────────────────────────────────────────────────────
      Relays (additional)                               3      $6
      Relay socket/PCB pads                            3      $0.50
      Wire (22 AWG stranded)                          50 ft   $2
      Connector shrouds                                4      $0.20
      Solder, flux, thermal compound                   -      $2
      3D-printed enclosure (element box, optional)     1      $5 (materials)

TOTAL ADDITIONAL FOR 5-ELEMENT:                               ~$15-20


4.3 PIN DIODE SWITCHING (70cm EXAMPLE): COMPONENT ADDITIONS

Item  Part Number           Description            Qty  Value    Cost
────────────────────────────────────────────────────────────────────────
 1    MA4P504               PIN diode SOT-23        4   -        $2.20
 2    LQG15HS47NG00D        RFC choke 0402         4   47nH      $0.48
 3    Metal-film resistor   Bias limit resistor    4   1.5kΩ     $0.16
 4    NPO ceramic 0402      Bypass capacitor       4   100pF     $0.12
 5    2N2222 or BC337       Bias driver transistor 4   -         $0.32
 6    1N4148                Small signal diode     4   -         $0.16
 7    Microstrip PCB        RT/duroid 5880         1   2"x3"     $8

TOTAL COMPONENT COST FOR PIN DIODE SYSTEM:                 ~$12-15


4.4 TOOL AND EQUIPMENT LIST

    Soldering Equipment:
        - Soldering iron: 30-40W, 350-400°C tip
        - Solder: 60/40 or lead-free, 0.8mm diameter
        - Flux: Rosin-core (integral in solder) or paste
        - Desoldering braid or solder sucker (for rework)

    Measurement Instruments:
        - Multimeter: DVM 3.5 digit (impedance, continuity, voltage)
        - Network analyzer: NanoVNA-H (50-900 MHz, $30-50 on AliExpress)
        - RF power meter: Optional (AD8307-based homebrew, $20-30)
        - Oscilloscope: Optional (for relay bounce observation, 10-100 MHz)

    Assembly Tools:
        - Wire strippers: Automatic or manual
        - Diagonal cutters: Precision for SMD component leads
        - Tweezers: Precision stainless steel
        - Allen wrench set: For boom-mounted clamps
        - Torque wrench: 0-20 in-lb (for RF connector torque spec)

    Mechanical (3D Printing):
        - 3D printer: Minimum 200x200x200 mm build area (Ender 3/CR-10)
        - Filament: PETG or ASA (weather-resistant)
        - Nozzle: 0.4mm standard
        - Bed temperature: 80-90°C for PETG
        - Nozzle temperature: 240°C

    Miscellaneous:
        - Heat shrink tubing: Assorted sizes 2-10mm
        - Electrical tape: Standard 3M Scotch 33+
        - Cable ties: Reusable nylon, 6mm wide
        - Thread-lock compound: Removable (blue, 0.5mL)
        - Dielectric grease: For RF connector threads
        - Silicone caulk: Weatherproofing enclosure seams
        - Ferrite beads: 0402 SMD, 100Ω @ 100 MHz (qty 10)


4.5 VENDOR SOURCES FOR CRITICAL PARTS

Semiconductors (Relays, PIN Diodes, ICs):
    - Digi-Key: www.digikey.com (extensive inventory, 1-10 day shipping)
    - Mouser: www.mouser.com (similar stock, international)
    - AliExpress: www.aliexpress.com (bulk, 2-4 week lead time)
    - Newark: www.newark.com (enterprise tier, high cost)

Passive Components (Resistors, Capacitors):
    - Any major distributor above
    - Local electronics hobby shop (fast, but higher cost)

Connectors & Mechanical:
    - McMaster-Carr: www.mcmaster.com (vast selection, 1-day shipping)
    - Fastenal: www.fastenal.com (industrial hardware, local pickup)
    - Home Depot/Lowes: 3D printer filament, hardware fasteners

3D Printing Filament:
    - Prusament PETG: Prusa Research (Czech, EU-based)
    - Overture PETG: Amazon (US-based)
    - Generic PETG: AliExpress (bulk, variable quality)


4.6 ANTENNA HARDWARE (Not Included in Component List)

Boom & Element Material:
    - 6061-T6 Aluminum alloy (preferred for low RF loss)
    - Boom: Round tube 1-1.25" OD, wall 0.035" (standard 6M/2M)
    - Elements: Round rod 1/2" OD (6M), 1/4" OD (2M), 1/8" (UHF)
    - Alternative: Stainless steel (better corrosion resistance, higher loss)

Coax Feedline:
    - RG-8X or RG-213: 6M/2M (low loss, flexible)
    - LMR-240 or equivalent: VHF multi-band
    - LMR-600: Low loss, recommended for 50+ meter runs
    - Connectors: SO-239 (6M/2M), N-type (70cm+), SMA (UHF test)

Mounting Hardware:
    - U-bolts: Stainless steel, clamp to mast
    - Boom-to-mast clamp: Aluminum or stainless, torque 50-100 in-lb
    - Element clamps: Aluminum or nylon, prevent rotation

Weatherproofing:
    - Coax connectors: Coax Seal or self-amalgamating tape
    - Control cable seals: M20 strain relief gland
    - Sealant: Silicone RTV (non-acetic for RF components)
    - O-ring stock: NBR70, multiple sizes

================================================================================

SECTION 5.0  INSTALLATION AND ALIGNMENT

5.1 MECHANICAL ASSEMBLY: BOOM AND ELEMENTS

Step 1: Boom Preparation
    a) Measure boom to verify length per NEC2 model
    b) Mark element positions on boom (measure from center-fed driven element):
       - Reflector: 0.20λ behind DE
       - Director 1: 0.25λ ahead of DE
       - Director 2: 0.50λ ahead of DE
       - Director 3: 0.80λ ahead of DE
    c) Use digital calipers to verify spacing (tolerance ±5 mm)
    d) Drill mounting holes for element clamps (10mm diameter)

Step 2: Element Installation
    a) Cut elements to length per NEC2 model (±2 mm tolerance)
    b) Deburr cut ends with file or sandpaper
    c) Mount in clamps, center each element on boom
    d) Tighten clamp bolts uniformly (no more than hand-tight to avoid crushing)
    e) For driven element: position feed at center, mark coax entry point

Step 3: Boom Mounting to Mast
    a) U-bolt clamp assembly:
       - Install U-bolts 1/2" below driven element for balance
       - Tighten gradually in criss-cross pattern (avoid boom twist)
       - Torque: 50 in-lb minimum (verify with torque wrench)
    b) Check level with spirit level (boom should be horizontal ±1°)
    c) Tie off boom with guy-lines if mast > 30 feet

5.2 ELECTRICAL ASSEMBLY: RELAY/PIN DIODE SWITCHING NETWORK

Step 4: Main Control PCB Assembly
    a) Populate through-hole components (relays, DIP IC, connectors) first
    b) For SMD relay driver circuits:
       - Use fine-tip soldering iron (0.8mm tip)
       - Apply minimum solder (just enough for joint)
       - Use rosin flux paste for consistent wetting
    c) Verify no shorts between adjacent pins (use ohmmeter)
    d) Inspect all solder joints under magnification (30x loupe)

Step 5: Relay/PIN Diode Module Installation (Per Element)
    a) For relays: Mount relay socket on PCB, insert relay
    b) For PIN diodes:
       - Solder RFC choke to PCB trace (< 2 mm lead length)
       - Solder PIN diode (SOT-23 orientation per datasheet)
       - Solder bypass capacitor directly at PIN cathode (< 1 mm)
       - Apply conformal coating (optional, protects from salt spray)
    c) Verify impedance match with NanoVNA before field deployment

Step 6: Control Cable Routing
    a) Use shielded twisted-pair (CAT5e or similar) for > 1 meter runs
    b) Route separately from RF coax (minimum 10 cm separation)
    c) At antenna mast: Ferrite beads on each twisted pair
    d) Shield connection: Connect to boom GND only at antenna end
    e) Do NOT connect shield at controller end (floating, prevents ground loop)

5.3 COAX FEEDLINE INSTALLATION

Step 7: Driven Element Feed Point
    a) At driven element center: Install SO-239 female connector
    b) Coax routing: Zip-tie to boom every 12 inches
    c) Strain relief: Cinch-clamp at mast base
    d) Solder or crimp connections (solder preferred for low loss)

Step 8: Coax Connector Weatherproofing
    a) Wrap connector threads with PTFE thread-seal tape (2-3 wraps)
    b) Apply dielectric grease to threads (prevents corrosion)
    c) Install protective cap over SO-239 when not in use
    d) For remote installations: Use Coax Seal self-amalgamating tape
       - Wrap around SO-239 shell (3 overlapping layers)
       - Provides superior UV and salt-fog protection

Step 9: Coax Termination at Shack
    a) Install N-type or SO-239 bulkhead connector on feedthrough
    b) Attach length of coax to transceiver with appropriate connector type
    c) Keep coax run as short as possible (loss ∝ length)
    d) Use low-loss coax for > 50 foot runs (see Section 4.5)

5.4 WEATHER-SEALED ENCLOSURE INSTALLATION

Step 10: Element Junction Box (At Boom)
    a) Install relay/PIN diode PCB in 3D-printed IP66 enclosure
    b) SO-239 connector: Pointing toward antenna feed
    c) N-type connector: Pointing toward boom RF return
    d) DB9 control port: Rear, pointing downward (drainage)
    e) Secure enclosure to boom with stainless steel U-bolts
    f) Route control cable through M20 cable gland (rear)

Step 11: Gasket/Sealing
    a) Install O-ring (NBR70, 3.2 mm OD × 2.4 mm ID) in groove
    b) Apply thin layer of silicone grease to O-ring
    c) Close lid with snap tabs, apply moderate downward pressure (2 kg)
    d) Lid should require deliberate motion to open (good seal indicat)

Step 12: Grounding
    a) Boom GND connection: Short, thick wire to main enclosure
    b) Control cable shield: Soldered to boom GND at antenna
    c) Shack grounding: Separate ground run to transceiver chassis
       (Prevents RF coupling into control lines)

5.5 MAIN CONTROLLER INSTALLATION

Step 13: Mast-Top or Ground Mount
    a) Select mounting location:
       - Mast-top: Minimizes control cable run, maximizes integration
       - Ground-mount: Easier access, reduces wind loading on mast
    b) Install 3D-printed housing on post (10-foot ABS Schedule 40 pipe works)
    c) WiFi antenna: Mount on top of enclosure, oriented vertically
    d) Control cable: Route in conduit (PVC or UV-resistant spiral)
    e) Power: USB-C via weatherproof connector or Anderson pole (12V conversion)

Step 14: WiFi Access Point Configuration
    a) Power on controller (USB-C or 12V PSU)
    b) Scan for WiFi SSID "BeamController" (default)
    c) Connect with password "hamradio"
    d) Web interface available at http://192.168.4.1
    e) Change SSID/password to suit your installation

================================================================================

SECTION 6.0  CALIBRATION AND INITIAL ALIGNMENT

6.1 NEC2 ANTENNA MODEL BASELINE MEASUREMENT

Step 1: Measure Impedance in Each Pattern
    a) Equipment needed: NanoVNA-H or similar (50-900 MHz capable)
    b) Procedure:
       1. Connect NanoVNA to driven element via short coax (< 1 meter)
       2. Select frequency of interest (50.1 MHz for 6M, etc.)
       3. Open S11 (reflection) measurement mode
       4. Normalize to 50Ω using calibration ("CAL" button → "SOLT")
       5. Apply each beam pattern (via serial command or WiFi)
       6. Record S11 magnitude and phase for each pattern
       7. Document: Frequency, S11 (dB), Return Loss (dB), VSWR, Impedance (Z=Rx+jXx)

Step 2: Expected Results
    Pattern          Freq    Return Loss  VSWR    Impedance (approx)
    ────────────────────────────────────────────────────────────────
    Baseline 5-el    50.0    15+ dB       < 1.5   45-55Ω + j0-5Ω
    Compact 3-el     50.0    12+ dB       < 1.8   40-60Ω + j5-10Ω
    Reverse          50.0    10+ dB       < 2.0   30-70Ω + j10-15Ω

    If VSWR > 2.5 in any pattern, antenna may need tuning. Proceed to step 3.

Step 3: Impedance Matching Adjustment (If Needed)
    a) If RL < 12 dB in baseline pattern:
       1. Measure driven element resonance (should be ~5% lower than NEC2)
       2. Shorten driven element by 1-2 cm
       3. Re-measure S11
       4. Repeat until RL > 15 dB
    b) If reactance |X| > 10Ω, check matching network:
       1. Verify gamma-match capacitor value (should tune XC = -X)
       2. Adjust cap trim/slug if present
       3. Or use hairpin match for broader bandwidth

Step 4: Document Baseline
    a) Save screenshots of NanoVNA displays for each pattern
    b) Create baseline table (include in operations manual)
    c) Compare to NEC2 predictions in Appendix B

6.2 RF ISOLATION VERIFICATION

Step 5: Pattern Distortion Check (Field)
    a) Equipment: NanoVNA or spectrum analyzer (for F/B measurement)
    b) Procedure:
       1. Set antenna to baseline pattern (all directors active)
       2. Measure forward radiation pattern (azimuth, 0-180°)
       3. Observe F/B ratio and main lobe shape
       4. Energize all control relays (apply bias to all elements)
       5. Measure pattern again
       6. Compare: F/B should degrade < 1-2 dB (less if ferrites installed)
    c) If F/B degrades > 3 dB:
       1. Add ferrite bead to problematic control line
       2. Re-measure
       3. Repeat until degradation < 1 dB

6.3 ELEMENT STATE VERIFICATION (With RF Sensor)

Step 6: RF Current Probe Calibration
    a) Equipment: ACS712-5A current sensor (if installed)
    b) DC calibration (bench test):
       1. Apply 5V to sensor input through 1.5kΩ resistor
       2. Read voltage output (should be 2.5V ± 0.1V @ 0A)
       3. Adjust zero-offset in firmware if > 0.1V error
    c) RF calibration (mast-top):
       1. Transmit at known power level (e.g., 10W on 2M)
       2. Measure RF current at element feed (should be 0.5-2A RMS for 10W)
       3. Verify element state (switching ON/OFF should double current)
       4. Create lookup table in firmware (Power_W vs Current_A per band)

6.4 SWR MONITORING CALIBRATION

Step 7: AD8307 Power Detector Calibration (If Installed)
    a) Equipment: Signal generator (or transmitter), 50Ω load
    b) Procedure:
       1. Set signal generator to band center frequency (50.1 MHz for 6M)
       2. Connect generator to antenna input via 50Ω load
       3. Measure power with external power meter
       4. Read ADC voltage from ESP32 (via serial or web API)
       5. Apply polynomial fit: P(dBm) = a0 + a1*V + a2*V²
       6. Store coefficients in firmware

    Expected behavior:
       0 dBm (1 W) → 2.4V output
       +10 dBm (10 W) → 2.65V output
       +20 dBm (100 W) → 2.9V output
       -10 dBm (100 mW) → 2.15V output

Step 8: SWR Calculation Verification
    a) SWR = (1 + Γ) / (1 - Γ) where Γ = √(Pref/Pfwd)
    b) Example calculation:
       1. Transmit 100W (forward power = +20 dBm)
       2. Measure reflected power = -5 dBm (0.316 W)
       3. Γ = √(0.316/100) = 0.056 (≈-25 dB)
       4. SWR = (1 + 0.056)/(1 - 0.056) = 1.12:1
    c) If calculated SWR disagrees with NanoVNA measurement:
       1. Check forward/reflected power calibration
       2. Verify coupling factor of directional coupler (-20 to -30 dB typical)
       3. Recalibrate coefficients in firmware

6.5 BEAM PATTERN VERIFICATION (Optional, Advanced)

Step 9: Field Measurement
    a) Equipment: Portable signal generator + 10 dBi reference antenna OR
                  Satellite doppler shift measurement
    b) Procedure:
       1. Set antenna to known pattern (e.g., baseline 5-element)
       2. Measure field strength in 36 directions (every 10°)
       3. Plot polar diagram
       4. Compare to NEC2 model in Appendix B
       5. Expected agreement: ±2 dBi in main lobe

    Alternative (Doppler):
       1. Monitor satellite pass (LEO like ISS or amateur satellite)
       2. Measure signal strength variation as satellite transits
       3. Peak strength indicates main lobe direction
       4. Null position indicates F/B ratio

================================================================================

SECTION 7.0  OPERATION

7.1 CONTROL INTERFACES

Three methods available to select beam patterns:

METHOD 1: TOUCHSCREEN UI (CYD 480x320 Display)
────────────────────────────────────────────────
    Button Layout (12x6 grid):

        [6M]   [2M]  [1.25M] [70CM]  [33CM]  [20CM]  ← Band Select

        [OMNI]        [BASE] [CMPCT]  ← Patterns
        [CARDI-N]     [REV]  [ENDFIRE]
        [CARDI-E]     [MAX-G][MAX-FB]
        [CARDI-W]
        [STATUS]      [RESET]

    Display Shows (Real-Time Updates):
        - Current band (large text, top)
        - Current pattern (large text, center)
        - SWR: X.XX:1 (green if < 1.5, yellow if 1.5-2.0, red if > 2.0)
        - Forward power: XXXX.X dBm
        - Reflected power: XXXX.X dBm
        - Element states: [Refl][Dir1][Dir2][Dir3] (ON/OFF indicators)
        - Controller status: ONLINE (blue), OFFLINE (red), ERROR (red flashing)

    Procedure:
        1. Select desired band (6M, 2M, etc.) → display shows available patterns
        2. Select pattern (BASELINE, COMPACT, REVERSE, etc.)
        3. Pattern loads immediately (50 ms relay settling delay)
        4. Monitor SWR on display (should stay < 1.5:1 across patterns)
        5. To switch bands: Press new band button → pattern list updates

METHOD 2: WIFI REST API (Via Laptop or Mobile)
──────────────────────────────────────────────────
    Endpoint: http://192.168.4.1/control (POST request)
    Format: application/x-www-form-urlencoded

    Example 1: Set pattern
        POST /control?band=2m&pattern=baseline
        Response: 200 OK "Pattern applied"

    Example 2: Direct element control
        POST /element?idx=0&state=2
        Parameters: idx=[0-3], state=[0=OFF, 1=REFLECTOR, 2=DIRECTOR]
        Response: 200 OK "Element updated"

    Status query:
        GET /api
        Response: JSON
            {
              "band": "2m",
              "pattern": "baseline",
              "swr": 1.2,
              "power_fwd_dbm": 35.2,
              "power_ref_dbm": 5.1,
              "elements": [2, 2, 2, 2]
            }

    Tools: curl (CLI), Postman (GUI), Python requests (automation)

    Example curl command:
        curl -X POST "http://192.168.4.1/control?band=6m&pattern=compact"

METHOD 3: SERIAL COMMAND LINE (Via USB or RS-232)
──────────────────────────────────────────────────
    Connection: USB-to-Serial FTDI adapter, 115200 baud, 8N1

    Commands:
        p<n>    - Set pattern (0-11)
                  p0 = OMNI, p1 = BASELINE, p2 = COMPACT, p3 = REVERSE, ...
                  p11 = F/B-MAX
        s       - Print status (band, pattern, SWR, element states)
        l       - List available patterns
        h       - Print help (command list)

    Examples:
        p1      → Set to BASELINE (all directors active)
        s       → Display: Band=2m, Pattern=BASELINE, SWR=1.2:1, Fwd=35 dBm
        l       → List: [0] OMNI, [1] BASELINE, [2] COMPACT, ...

    Tool: screen, minicom, or Arduino Serial Monitor

7.2 OPERATIONAL PROCEDURES

Procedure A: QUICK BAND CHANGE

    Touchscreen Method:
        1. Press [2M] button
        2. Pattern list updates on display
        3. Confirm pattern (e.g., [BASELINE])
        4. Wait 100 ms (element switching + settling)
        5. Resume operation

    WiFi Method:
        1. Send: GET /api (verify current band/pattern)
        2. Send: POST /control?band=2m&pattern=baseline
        3. Wait 100 ms
        4. Verify SWR < 1.5:1

    Time required: ~1 second (switching + settling)


Procedure B: MULTI-PATTERN FIELD DAY (70cm Example)

    Setup:
        1. Power on controller (USB-C, automatic startup)
        2. Verify WiFi SSID "BeamController" online
        3. Connect laptop to WiFi
        4. Open http://192.168.4.1 in web browser

    Operation (Contest):
        - Baseline (DX contacts): Send POST /control?band=70cm&pattern=baseline
        - Local fills (shorter range): Send /control?...pattern=compact
        - Noisy direction (RFI): Send /control?...pattern=reverse (beam away)
        - Low angle (satellite): Send /control?...pattern=max-g (maximum gain)

    Monitoring:
        - Continuously display SWR on screen (check for pattern validity)
        - If SWR > 2.0 in any pattern, stop and re-tune antenna
        - Log pattern changes and QSO results in contest log


Procedure C: SATELLITE PASS OPERATION (EME possible with proper tuning)

    1. Open satellite tracking app (Gpredict, ISS Detector, etc.)
    2. Identify pass: elevation angle, azimuth bearing, time window
    3. Pre-position antenna using pattern with maximum gain (BASELINE or MAX-G)
    4. Track satellite elevation:
       - Elevation < 30°: Use BASELINE (narrow beamwidth, max gain)
       - Elevation 30-60°: Use COMPACT (wider beamwidth, easier tracking)
       - Elevation > 60° overhead: Use OMNI or REVERSE (omnidirectional coverage)
    5. Switch patterns as needed via touchscreen or API
    6. Monitor SWR throughout pass (should remain < 2.0)

7.3 PERFORMANCE EXPECTATIONS

Pattern         Freq    Gain (dBd)  Beamwidth  F/B (dB)  Notes
──────────────────────────────────────────────────────────────────
OMNI            50 MHz  6.0         90°        8         Omnidirectional
BASELINE (5-el) 50 MHz  9.2         25°        22        Narrow main lobe
COMPACT (3-el)  50 MHz  6.5         40°        10        Portable config
REVERSE         50 MHz  3.0         120°       8         Backward lobe
CARDIOID        50 MHz  7.0         45°        15        Null in one direction
MAX-GAIN        50 MHz  9.5         24°        25        Optimized patterns
MAX-F/B         50 MHz  8.5         30°        28        High front-to-back

Actual gain may vary ±0.5 dBd depending on:
    - Boom material (aluminum vs stainless)
    - Feeding method (gamma, hairpin, 50Ω stub)
    - SWR at operating frequency (loss if SWR > 1.5:1)
    - Environmental coupling (reflections from nearby structures)

7.4 BAND-SPECIFIC OPERATING NOTES

6M (50 MHz) Notes:
    - Relays switch cleanly, < 0.2 dB loss expected
    - SWR typically < 1.3:1 across patterns
    - RF isolation not critical (long wavelengths)
    - Safe to co-locate with other 6M antennas (>2 meters separation)

2M (144 MHz) Notes:
    - Relay loss approaching limit (~0.25 dB)
    - SWR may vary 1.1 - 1.8:1 depending on pattern
    - RF isolation important (wavelength = 2 meters, control loops significant)
    - Keep control cables in shielded conduit if > 5 meters from antenna

70cm (432 MHz) Notes:
    - PIN diodes REQUIRED (relays cause > 5 dB loss)
    - SWR typically < 1.5:1 across patterns
    - RF isolation CRITICAL (wavelength = 69 cm)
    - Ferrite beads on all control lines MANDATORY
    - Microstrip bias network preferred over lumped components
    - UHF SWR meter recommended for field verification

33cm/20cm (902/1296 MHz) Notes:
    - PIN diodes with careful PCB layout (no > 2mm loops)
    - SMD component layout critical (0402 or smaller)
    - Microstrip transmission line for bias injection (λ/4 choke)
    - SWR < 1.3:1 difficult to achieve; aim for < 1.5:1
    - Minimal control cable run (< 0.5 meters preferred)

================================================================================

SECTION 8.0  TROUBLESHOOTING

Problem: Pattern does not change when button pressed

    Diagnosis:
        1. Check WiFi connection (SSID "BeamController" should appear)
        2. Verify battery/power indicator (blue LED on controller should be ON)
        3. Check element junction box: Blue LED should blink (element node online)
        4. If element LED is RED: Over-current or over-temperature fault

    Solutions:
        Option A (Touchscreen):
            a. Press [RESET] button
            b. Wait 3 seconds for controller to restart
            c. Select pattern again
            d. If still fails, proceed to Option C

        Option B (WiFi):
            a. Send GET /api to verify controller is responding
            b. If no response: Controller offline, check USB power
            c. If response received: Try POST /element?idx=0&state=2 (direct)
            d. If direct command works but pattern doesn't: Firmware issue

        Option C (Serial Debug):
            a. Connect USB-to-Serial to controller
            b. Open Serial Monitor (115200 baud)
            c. Type 's' for status
            d. Look for error message or pattern name mismatch
            e. Type 'p1' to manually set pattern 1 (BASELINE)

Problem: SWR high (> 2.0) after pattern change

    Common Causes:
        1. Reflector element shorted to boom when should be free
        2. Impedance mismatch at driven element (matching network mistuned)
        3. Damaged coax feedline (check with NanoVNA at shack)
        4. Element resonance drift (environmental or temperature)

    Diagnosis Procedure:
        a. Measure impedance with NanoVNA at antenna (using short test lead)
        b. Expected impedance baseline pattern: 45-55Ω + j0-5Ω @ 50.1 MHz
        c. If measured impedance significantly different:
           - Check NEC2 model matches physical antenna
           - Verify element lengths (use caliper, measure from element tip-to-tip)
           - Check element spacing on boom (measure with measuring tape)
        d. If impedance acceptable but SWR still high:
           - Problem is in coax or matching network
           - Check coax continuity with ohmmeter (should be < 1Ω per 100 feet)
           - Verify gamma match capacitor value (use LC meter if available)

    Solutions:
        Option 1 (Reflector Short Stuck):
            a. Unplug element control cable from relay/PIN PCB
            b. Manually short element to boom with temporary wire
            c. Measure SWR with NanoVNA
            d. If SWR drops 0.5+:1, relay/PIN is broken → replace

        Option 2 (Impedance Mismatch):
            a. Measure resonance: Adjust frequency until SWR minimum
            b. If resonance low (48 MHz when expecting 50.1):
               - Shorten element by 1-2 cm
               - Remeasure SWR
            c. If reactance X > 10Ω even at resonance:
               - Adjust gamma match capacitor (if present)
               - Or add series capacitor to matching network

        Option 3 (Coax Damaged):
            a. Replace 3 meters of coax starting at driven element
            b. Re-measure SWR
            c. If improves: Replace entire coax run

Problem: Relay clicks but element doesn't switch (relay malfunction)

    Symptoms:
        - Audible relay click/buzz when pattern changes
        - But SWR doesn't change (element still in previous state)
        - Red LED on element node blinks (driver failure or stuck contact)

    Diagnosis:
        1. Check relay contact continuity:
           a. Use ohmmeter across relay NO contact pair
           b. Measure resistance when relay energized (should be < 0.5Ω)
           c. Measure when de-energized (should be open > 10 MΩ)
        2. If contacts are stuck:
           a. Check for solder bridge between pins (inspect under magnification)
           b. Check for bent or damaged contact spring
           c. Replace relay if any contact is permanently stuck

    Solutions:
        Option 1 (Clean Contact Corrosion):
            a. De-solder relay from PCB
            b. Exercise relay contacts 20 times (energize/de-energize cycle)
            c. If contacts show black oxidation: Replace relay
            d. If contacts clean up: Re-solder relay and retest

        Option 2 (Replace Relay):
            a. Desolder failed relay from PCB
            b. Install new Omron G5V-2 relay (ensure correct part number)
            c. Apply dielectric grease to contacts for long-term protection
            d. Re-test pattern changes

Problem: Control cable allows RF coupling into element node (pattern distortion)

    Symptoms:
        - Pattern F/B ratio degrades 3+ dB when control relay energized
        - Spurious pattern nulls appear when switching patterns
        - Element node status LED flickers during high-power TX

    Solutions:
        1. Add ferrite bead to each control line:
           a. Slide ferrite (100Ω @ 100 MHz, 0402 SMD) onto control wire
           b. Solder ferrite directly before element PCB entry
           c. Re-measure pattern (F/B degradation should reduce 3+ dB)

        2. Shield control cable:
           a. Replace unshielded wire with shielded twisted-pair (CAT5e)
           b. Connect shield to boom GND at antenna ONLY (float at controller)
           c. Add ferrite toroid (T50-2, 3 turns) around twisted-pair at antenna
           d. Re-measure pattern

        3. Ground isolation:
           a. Verify separate power supply for relay coil (not shared with RF)
           b. Use separate wire for relay GND return (not through boom connection)
           c. Add 10µF capacitor across relay coil for RC filtering

Problem: Over-current fault (element node red LED stays ON)

    Symptoms:
        - Red LED on element node stays illuminated
        - Relay doesn't energize (GPIO stuck LOW)
        - SWR unchanged (element appears shorted)
        - Possible smell of burned electronics

    Causes:
        1. Shorted PIN diode (damaged by ESD or voltage overstress)
        2. Relay contact stuck in "shorted" position (welded)
        3. Element accidentally grounded (broken seal allowing moisture)
        4. Incorrect bias voltage applied (> 15V fries PIN diode)

    Diagnosis:
        a. Measure DC resistance across element to boom:
           - Normal (director open): > 10 MΩ
           - Shorted (reflector or stuck contact): < 1Ω
        b. Measure bias voltage applied to PIN cathode:
           - Should be 0V (off) or +12V (on), NOT intermediate
           - If intermediate (e.g., 5V), transistor driver is broken
        c. Measure coil current if relay-based:
           - Should be 0 mA (off) or 40 mA (on)
           - If current doesn't change, relay coil is open

    Solutions:
        Option 1 (Replace PIN Diode):
            a. De-solder failed PIN diode from PCB
            b. Install new PIN diode (MA4P504 or equivalent per band)
            c. Apply new RFC choke and bypass capacitor
            d. Verify bias voltage before re-mounting to boom

        Option 2 (Replace Relay):
            a. De-solder relay from PCB
            b. Inspect contacts: If welded, don't re-use
            c. Install new Omron G5V-2 relay
            d. Verify 5V supply before energizing

        Option 3 (Check Bias Voltage):
            a. Verify transistor (2N2222) is not shorted (measure Vce @ ON state)
            b. Check base resistor (1kΩ) for opens with ohmmeter
            c. Replace transistor or resistor as needed

================================================================================

[SECTION 9.0 - MAINTENANCE, SECTION 10.0 - PARTS LIST, APPENDICES A-D
 continue in file... truncated for length]

SECTION 9.0  MAINTENANCE

Annual Inspection Checklist:

    [ ] Inspect O-ring seals in enclosure (check for hardening, shrinkage)
    [ ] Check all RF connector threads for corrosion
    [ ] Re-apply dielectric grease to connector threads
    [ ] Verify all coax is properly cinch-clamped to boom
    [ ] Inspect element for fatigue cracks (especially near boom clamp)
    [ ] Test relay contacts with ohmmeter (< 0.5Ω when closed)
    [ ] Verify control cable shield is floating (not grounded at shack end)
    [ ] Check WiFi signal strength (-70 dBm or better)
    [ ] Perform SWR baseline test on all bands
    [ ] Re-calibrate NanoVNA measurement (SOLT calibration)

Preventive Maintenance:

    - Replace O-ring gasket every 2 years (becomes brittle, loses elasticity)
    - Clean relay contacts annually (de-solder and burnish if > 200 mΩ)
    - Replace coax every 5-10 years (UV degrades dielectric, increases loss)
    - Re-seal RF connectors every 3 years or after severe weather


================================================================================

END OF REMOTE ELEMENT SWITCHING MANUAL

For questions or additions to this manual, contact:
    ant_gear@example.com

Version History:
    v1.0 (April 2026): Initial release, 6-band support (6M through 20cm)

This manual is distributed under CC BY-SA 4.0 license.
Improvements and corrections welcomed.

================================================================================
