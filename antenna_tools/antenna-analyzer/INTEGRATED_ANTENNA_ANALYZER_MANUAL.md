================================================================================
  INTEGRATED ANTENNA ANALYZER - TECHNICAL MANUAL
  Vector Network Analyzer for All Ham Bands (160M through 20cm)
  Model: ANT-VNA-100, Firmware v1.0.0
  Classification: UNCLASSIFIED
  Publication Date: 2026-04-24
  Navy Reference: NAVSEA Technical Doctype
================================================================================

TABLE OF CONTENTS
─────────────────
SECTION 1:   SCOPE AND APPLICABILITY
SECTION 2:   THEORY OF OPERATION
SECTION 3:   SYSTEM DESCRIPTION
SECTION 4:   EQUIPMENT AND MATERIALS (BOM)
SECTION 5:   INSTALLATION AND ASSEMBLY
SECTION 6:   CALIBRATION AND ALIGNMENT
SECTION 7:   OPERATION
SECTION 8:   TROUBLESHOOTING
SECTION 9:   MAINTENANCE AND CARE
SECTION 10:  PARTS LIST AND PROCUREMENT
APPENDIX A:  NEC2 ANTENNA MODELS
APPENDIX B:  SMITH CHART REFERENCE
APPENDIX C:  S-PARAMETER TABLES
APPENDIX D:  FREQUENCY ALLOCATION
APPENDIX E:  CALIBRATION DATA SHEETS


SECTION 1: SCOPE AND APPLICABILITY
===================================

1.1 PURPOSE
───────────
The Integrated Antenna Analyzer (ANT-VNA-100) is a portable vector network
analyzer designed for field measurement of antenna impedance, reflection
coefficient, VSWR, return loss, and resonance detection across the entire
amateur radio spectrum from 160M (1.8 MHz) through 20cm (1.3 GHz).

Primary applications:
  - Antenna tuning and optimization during field work
  - Impedance verification of antenna systems
  - Resonance detection and Q factor measurement
  - Real-time Smith chart display for impedance transformation
  - Data logging with GPS geolocation for propagation research
  - Verification of transmission line characteristics

1.2 APPLICABILITY
──────────────────
This manual applies to ANT-VNA-100 units with firmware v1.0.0 or later.
Intended users:
  - Amateur radio operators (General class and above)
  - RF engineers conducting antenna measurements
  - Field propagation researchers
  - Antenna design specialists
  - Radio amateurs engaged in QRP (low-power) transceiver optimization

Physical environment:
  - Operating temperature: -10°C to +70°C
  - Storage temperature: -20°C to +60°C
  - Humidity: 0-95% non-condensing
  - Altitude: Sea level to 3000 meters
  - Portable field use, vehicle-mounted, or tripod-mounted operation

1.3 SYSTEM CHARACTERISTICS
────────────────────────────
Frequency coverage: 1.8 MHz to 1.3 GHz (16 ham bands)
Measurement parameter: Complex reflection coefficient (Γ, S11)
Output impedance: 50Ω (standard RF reference)
Display: 3.5" TFT color LCD (320×240 pixels)
Connectivity: Bluetooth, WiFi (optional), SD card logging
Power: Rechargeable 9.6V LiFePO4 battery (3× 18650 cells)
Battery life: 8 hours continuous measurement
Physical size: 180mm × 130mm × 60mm (fits in shirt pocket)
Weight: 450g (without battery)


SECTION 2: THEORY OF OPERATION
===============================

2.1 REFLECTION MEASUREMENT FUNDAMENTALS
─────────────────────────────────────────

Antenna impedance is measured via the reflection coefficient Γ (gamma):
  Γ = (Z - Z₀) / (Z + Z₀)

Where:
  Z = antenna impedance (complex: R + jX)
  Z₀ = reference impedance (50Ω for RF systems)

Magnitude of Γ (|Γ|) ranges from 0 (matched) to 1 (open or short circuit).
Phase of Γ indicates whether antenna is capacitive or inductive.

The ANT-VNA-100 measures Γ by comparing:
  - Forward arm: Reference signal from RF source (directional coupler)
  - Reflected arm: Signal reflected from antenna under test (DUT)

Directional coupler provides -20 dB coupling, isolating forward and reflected
waves. The ratio of magnitudes and phase difference yields Γ directly.

2.2 S-PARAMETER THEORY
────────────────────────

S-parameters (scattering parameters) describe RF network behavior:
  S11 = Forward reflection coefficient (what we measure)
  S21 = Forward transmission (not measured in this analyzer)
  S12 = Reverse transmission
  S22 = Reverse reflection (load port, fixed at 50Ω)

Our measurement is S11 = Γ (reflection coefficient at DUT port).

Derived quantities:
  Return Loss (dB) = -20 log₁₀(|Γ|)
  VSWR = (1 + |Γ|) / (1 - |Γ|)
  Impedance: Z = 50 × (1 + Γ) / (1 - Γ)

Example interpretation:
  Γ = 0.0 (0 dB):     Perfectly matched (VSWR = 1.0)
  Γ = 0.33 (-9.5 dB): Moderately matched (VSWR = 2.0)
  Γ = 0.5 (-6 dB):    Poor match (VSWR = 3.0)
  Γ = 1.0 (-∞ dB):    Complete reflection (open or short)

2.3 AD9851 DDS SIGNAL GENERATION
──────────────────────────────────

Direct Digital Synthesis generates RF signals from 0.1 to 100 MHz with 1 Hz
resolution. The AD9851 uses a phase accumulator and sine lookup table:

  Phase accumulator increments at 200 MHz (33.333 MHz × 6 internal multiplier)
  Frequency word (FTW): 32-bit value controlling increment rate
  Output: Sine wave (reconstructed via low-pass filter)

Advantages:
  - Exact 1 Hz frequency resolution
  - Phase-coherent frequency switching (no transients)
  - Very low phase noise (-100 dBc/Hz @ 10 kHz offset)
  - Simple parallel data interface to ESP32

Limitations:
  - Maximum output: 100 MHz cleanly (200 MHz with harmonics)
  - Output amplitude: -5 to 0 dBm (no amplification)
  - Requires careful filtering to remove 200 MHz carrier

2.4 ADF4351 FRACTIONAL-N PLL SYNTHESIS
────────────────────────────────────────

The ADF4351 provides VHF/UHF coverage from 100 MHz to 1.4 GHz via phase-locked
loop (PLL) architecture:

  f_out = (INT + FRAC/2^25) × (f_ref / R)

Where:
  INT = integer divider (12 bits, 23-4095)
  FRAC = fractional numerator (25 bits, 0 to 2^25-1)
  f_ref = 25 MHz reference oscillator
  R = reference divider (default 1)

Charge pump with 2.5 mA current and loop filter capacitor maintain PLL lock.
Frequency update via SPI serial interface (3-wire: CLK, DATA, LE strobe).

Lock detect output indicates when PLL has achieved lock (typically < 10 ms).

2.5 AD8302 VECTOR GAIN/PHASE DETECTOR
───────────────────────────────────────

The AD8302 is the key IC enabling true vector (phase-aware) measurement:
  - VMAG output: 30 mV/dB, proportional to magnitude of signal ratio
  - VPHS output: 10 mV/degree, proportional to phase difference

Input network balun converts single-ended RF to differential form (required by
AD8302). Outputs are filtered (10 kΩ + 10 nF, ~1.6 kHz bandwidth) to remove
noise and RF ripple.

Two measurement modes:
  a) Forward/Reflected: Measures S11 directly (normal VNA mode)
  b) Swapped measurement: Repeats with INPA ↔ INPB swapped to resolve
     phase ambiguity (4-quadrant measurement when needed)

2.6 OSL ERROR CORRECTION (12-TERM ERROR MODEL)
────────────────────────────────────────────────

Raw S11 measurements include systematic errors:
  - Source mismatch (source impedance not exactly 50Ω)
  - Directivity (coupler leakage and directional accuracy)
  - Reflection tracking (frequency-dependent magnitude/phase drift)

OSL (Open-Short-Load) calibration corrects these errors via three standards:
  - Open: Known S11 = +1 (perfect reflection)
  - Short: Known S11 = -1 (inverted reflection)
  - Load: Known S11 = 0 (perfect 50Ω match)

Measured data for each standard:
  Γ_meas = e₀₀ + (e₁₁ × e₂₃ × Γ_true) / (1 + e₂₂ × Γ_true)

Where error terms are:
  e₀₀ = directivity (leakage from forward to reflected)
  e₁₁ = port match (source impedance error)
  e₂₃ = reflection tracking (frequency-dependent response)
  e₂₂ = load match (receiver impedance)

Firmware solves this system of equations to extract error terms, then applies
inverse transformation to correct measured data:
  Γ_corrected = (Γ_meas - e₀₀) / ((e₁₁ - e₀₀ × e₂₂) + e₂₃ × Γ_meas × e₂₂)

Result: Measurement accuracy typically ±3% magnitude, ±5° phase.

2.7 SMITH CHART THEORY AND INTERPRETATION
────────────────────────────────────────────

The Smith chart graphically represents complex impedance in normalized coordinates:
  Γ = (Z - 50) / (Z + 50)

Key chart features:
  - Center (0, 0): Γ = 0, Z = 50Ω (matched)
  - Right edge (1, 0): Open circuit (very high impedance)
  - Left edge (-1, 0): Short circuit (near-zero impedance)
  - Horizontal lines: Constant resistance
  - Vertical curves: Constant reactance

Common patterns:
  - Antenna at resonance: Right side of chart (small reactance)
  - Capacitive antenna (f > f_res): Upper half (positive reactance)
  - Inductive antenna (f < f_res): Lower half (negative reactance)
  - Moving along constant R circle as frequency changes: reactive tuning

VSWR circles on Smith chart:
  VSWR 1:1 = point at center (perfect match)
  VSWR 2:1 = circle with |Γ| = 0.33
  VSWR 3:1 = circle with |Γ| = 0.50
  VSWR ∞:1 = outer circle (complete mismatch)

Transmission line effect (moving along lossless line):
  Each wavelength of line rotation = one complete circle on Smith chart
  Quarter wavelength rotation = 90° on chart
  This enables impedance transformation matching calculations.

2.8 RESONANCE DETECTION ALGORITHM
───────────────────────────────────

Antenna resonance occurs when reactance = 0 (imaginary part of Z vanishes):
  Z_res = R + j0

The firmware finds resonance by:
  1. Sweep frequency across band
  2. Calculate impedance Z = R + jX at each point
  3. Find frequency where |X| is minimum
  4. Report resonant frequency, R_res, Q factor

Q factor (quality factor):
  Q = |X_measured| / R_measured (at frequency nearby resonance)

  High Q (sharpness): Resonance is narrow (tuned circuit)
  Low Q (broadness): Resonance is wide (damped)

Typical values:
  Small HF antenna: Q ≈ 20-50 (narrow resonance)
  Dipole: Q ≈ 5-10 (moderate resonance)
  Broadband antenna: Q ≈ 1-3 (very wide passband)

Firmware numerically finds the minimum by:
  1. Coarse search (1% frequency steps) to find approximate resonance
  2. Fine search (0.1% steps) around the approximate point
  3. Parabolic fitting to find exact frequency with sub-step resolution


SECTION 3: SYSTEM DESCRIPTION
==============================

3.1 BLOCK DIAGRAM (ASCII)
────────────────────────

         ┌─────────────────────────────────────────────────────┐
         │ BATTERY (9.6V LiFePO4, 3× 18650 cells)             │
         └────────┬────────────────────────────────────────────┘
                  │
         ┌────────v──────────────────────────────────────────┐
         │ POWER DISTRIBUTION (5V, 3.3V, ±5V rails)         │
         │ Buck: 9.6V → 5V (1.5A)                           │
         │ LDO:  5V → 3.3V (800mA digital)                  │
         │ LDO:  5V → 3.3V (200mA RF-isolated)              │
         │ CP:   5V → ±5V (100mA analog)                    │
         └────────┬──────────────────────────────────────────┘
                  │
         ┌────────v──────────────────────────────────────────┐
         │ ESP32 MICROCONTROLLER                            │
         │ - Frequency synthesis control (SPI)              │
         │ - Band/filter selection (GPIO)                   │
         │ - Display interface (SPI/parallel)               │
         │ - Data logging (SD card SPI)                     │
         │ - Bluetooth wireless                             │
         └────────┬──────────────────────────────────────────┘
                  │
    ┌─────────────┼─────────────┐
    │             │             │
    v             v             v
 ┌──────┐    ┌─────────┐    ┌──────────┐
 │DDS   │    │PLL      │    │DISPLAY   │
 │AD9851│    │ADF4351  │    │3.5" TFT  │
 │0.1-  │    │100-1.4G │    │ILI9341   │
 │100MHz│    │ Hz      │    │320×240px │
 └──┬───┘    └────┬────┘    └──────────┘
    │             │
    └─────────┬───┘
              │ RF signals (-5 to +5 dBm)
              v
    ┌──────────────────────────────────────┐
    │ BAND-SELECT RF SWITCH & FILTERING    │
    │ - Path selection (AD9851 or ADF4351) │
    │ - Band-specific filters (LP, BP, HP) │
    │ - PE4259 SPDT switches               │
    └──────┬───────────────────────────────┘
           │
           v
    ┌──────────────────────────────────────┐
    │ DIRECTIONAL COUPLER (multiband)      │
    │ - HF: FT50-43 toroid (-20dB)        │
    │ - VHF: PCB stripline (-20dB)        │
    │ - UHF: Microstrip (-20dB)           │
    │ Forward arm → AD8302 INPA            │
    │ Reflected arm → AD8302 INPB          │
    └──────┬───────────────────────────────┘
           │
           ├─→ [50Ω] ──→ DUT PORT (SMA)
           │            (antenna under test)
           │
           ├─→ [REF PORT (SMA)]
           │    (reference termination)
           │
           v
    ┌──────────────────────────────────────┐
    │ AD8302 VECTOR DETECTOR               │
    │ VMAG: magnitude (30mV/dB)            │
    │ VPHS: phase (10mV/°)                 │
    │ Output range: 0.6-3.2V / 0-1.8V      │
    └──────┬───────────────────────────────┘
           │
           v
    ┌──────────────────────────────────────┐
    │ ADS1115 ADC (16-bit, I2C)            │
    │ AIN0: VMAG (0.6-3.2V)                │
    │ AIN1: VPHS (0-1.8V)                  │
    │ AIN2: Battery voltage monitor        │
    │ AIN3: Overvoltage detect             │
    └──────┬───────────────────────────────┘
           │
           v
    ┌──────────────────────────────────────┐
    │ DATA PROCESSING (ESP32 firmware)     │
    │ - Voltage → dB and degrees           │
    │ - Calibration error correction       │
    │ - Impedance calculation              │
    │ - VSWR and return loss               │
    │ - Smith chart point generation       │
    └──────────────────────────────────────┘

3.2 FREQUENCY SYNTHESIZER SUBSYSTEM
─────────────────────────────────────

HF Band (1.8-100 MHz): AD9851 DDS
  - 33.333 MHz TCXO reference
  - 1 Hz frequency resolution
  - < 1 µs frequency update time
  - 3 switchable output filters (HF, VHF_low, VHF_high)

VHF/UHF Band (100-1400 MHz): ADF4351 PLL
  - 25 MHz TCXO reference
  - 1 kHz frequency resolution
  - 10-20 ms settling time to lock
  - Output power: -4 to +5 dBm (programmable)

Band switching criteria:
  f < 70 MHz:  HF filter + AD9851 (toroid coupler)
  70 MHz < f < 400 MHz: VHF filter + ADF4351 (stripline coupler)
  f > 400 MHz: UHF filter + ADF4351 (microstrip coupler)

Frequency stability:
  TCXO temperature coefficient: 0.1 ppm/°C
  Expected drift over 0-70°C: ±7 ppm maximum
  At 1 GHz: ±7 kHz worst-case drift (negligible for VNA measurement)

3.3 DIRECTIONAL COUPLER SUBSYSTEM
───────────────────────────────────

Three coupled-line sections provide -20 dB coupling across all bands:

  HF (1.8-70 MHz):
    - Fair-Rite FT50-43 toroid, 10T bifilar winding
    - Coupling factor: -20 ± 1 dB (stable 1.8-70 MHz)
    - Directivity: > 20 dB
    - Dielectric loss: minimal (ferrite Q ≈ 100)

  VHF (60-500 MHz):
    - PCB stripline (internal FR-4 layers)
    - Line spacing: 1.0 mm gap
    - Coupling factor: -20 ± 1 dB (stable 60-500 MHz)
    - Directivity: > 22 dB (phase velocity matching)

  UHF (400-1400 MHz):
    - Microstrip coupled-line on FR-4
    - Line width: 0.8 mm, gap: 0.635 mm
    - Coupling factor: -20 ± 1 dB (stable 400-1.4 GHz)
    - Directivity: > 24 dB (geometry optimization)

Absorptive terminations on unused ports prevent reflections.
All sections terminated in 50Ω via coupling capacitors and series resistors.

3.4 AD8302 VECTOR DETECTOR SUBSYSTEM
──────────────────────────────────────

Two AD8302 gain/phase detector ICs measure:
  - VMAG: Gain (magnitude ratio of forward/reflected)
  - VPHS: Phase difference between signals

Balun network (FT50-43 ferrite) converts 50Ω unbalanced coupler outputs to
differential form required by AD8302 internal circuits.

Output filtering: 10 kΩ + 10 nF per channel (1.6 kHz cutoff) removes:
  - RF ripple from AD9851/ADF4351 switching transients
  - ADC noise from high-frequency sources
  - Mechanical vibration effects in coupler

Balanced power supply (±5V analog via charge pump) reduces noise and improves
linearity. All digital supplies (3.3V/5V logic) isolated from ±5V analog via
ferrite filter to prevent switching noise coupling into VCO/detector.

3.5 DISPLAY AND USER INTERFACE
───────────────────────────────

Hardware:
  - 3.5" ILI9341 TFT display (320×240 pixels, 16-bit color)
  - Capacitive touch overlay (optional, future expansion)
  - Rotary encoder (band selection)
  - Pushbutton (measurement control)

Display modes:
  1. Main measurement screen (frequency, S11, VSWR, impedance)
  2. Real-time Smith chart (impedance point, circles, scale)
  3. Frequency sweep graph (magnitude vs frequency)
  4. Calibration status and diagnostics
  5. Menu system (band selection, sweep parameters, logging options)

Refresh rate: 2 Hz (500 ms update interval for smooth display)

Touch button areas (when capacitive touch enabled):
  - Top-left: Mode select (single frequency / sweep / resonance)
  - Top-center: Calibration menu
  - Top-right: Data logging on/off
  - Bottom: Band selection (cyclic through 16 bands)

3.6 DATA LOGGING (SD CARD + GPS)
──────────────────────────────────

Logging options:
  a) Manual save: User presses button, current sweep saved to SD card
  b) Auto-logging: Continuous measurement saved with timestamp
  c) Geolocation logging: Each measurement tagged with GPS coordinates

Data file format (CSV for compatibility):
  Frequency_Hz, VSWR, Return_Loss_dB, Z_Real, Z_Imag, Gamma_Mag, Gamma_Phase_Deg
  1000000, 1.5, -14.2, 42.3, -8.5, 0.22, -11.3
  ...

Metadata stored in separate file:
  Band: 40M
  Date: 2026-04-24 14:32:15 UTC
  Calibration: Valid (2026-04-20)
  Temperature: 25°C
  Battery: 9.2V
  GPS: 37.8044°N, 120.4506°W (Merced, CA)

3.7 CONNECTIVITY (BLUETOOTH & WIFI)
─────────────────────────────────────

Bluetooth (standard, no special hardware needed):
  - Profile: SPP (serial port profile)
  - Range: 10 meters typical
  - Baud rate: 115200 baud
  - Commands: FREQ, SWEEP, BAND, CALIBRATE, SAVE, STATUS

WiFi (optional expansion via external module):
  - Future capability for remote monitoring
  - Integration with cloud-based measurement database
  - Real-time data streaming to laptop/tablet
  - Firmware update via over-the-air (OTA) mechanism


SECTION 4: EQUIPMENT AND MATERIALS (BOM)
=========================================

4.1 CRITICAL COMPONENTS
────────────────────────

Quantity | Part Number | Description | Supplier | Cost
────────────────────────────────────────────────────────
1        | AD9851BCPZ  | DDS synthesizer, 0.1-100MHz | DigiKey | $35
1        | ADF4351BCPZ | PLL synthesizer, 35-4400MHz | DigiKey | $28
1        | AD8302ARMZ  | Vector detector, gain+phase | DigiKey | $22
1        | ADS1115IDGSR| 16-bit ADC, 4-channel I2C | DigiKey | $12
1        | ESP32-DEVKITC | Microcontroller, 240MHz | DigiKey | $18
1        | ILI9341     | TFT display driver + panel | Amazon | $25
1        | FT50-43     | Ferrite toroid (HF coupler) | Fair-Rite | $8
1        | PE4259-63   | RF SPDT switch, 3GHz | DigiKey | $15
2        | TP4056-ASM  | Battery charger module | Amazon | $3
1        | MP1496DS    | 5V buck converter | DigiKey | $12
3        | AMS1117-3.3 | 3.3V LDO regulators | DigiKey | $1
1        | MAX1044CPA  | ±5V charge pump | DigiKey | $8

PCB fabrication (multiboard, 4-layer):
  Main control board: $45 (quantity 5)
  RF filter board: $35
  Power distribution: $20
  Total PCB cost: $100

Mechanical (3D printed):
  Main enclosure: PETG, $12
  Display bezel: PETG, $5
  Boom mount adapter: Nylon, $8
  Battery compartment: PETG, $6
  Total mechanical: $31

Hardware (fasteners, connectors):
  SMA connectors (2×): $8
  M5 brass inserts (4×): $4
  U-bolt 8mm (1×): $3
  Coax cable (2m): $10
  USB micro connector: $2
  LED bicolor: $1
  Rotary encoder: $3
  Total hardware: $31

Passive components (discrete):
  Inductors (18650, 10nH-470nH): $25
  Capacitors (C0G/NPO, various): $20
  Resistors (film, 1%, various): $8
  Total passives: $53

Battery:
  3× 18650 LiFePO4 cells @ $8 each: $24
  Battery holder + BMS: $12
  Charging cable: $5
  Total battery: $41

TOTAL MATERIAL COST: ~$410 (single unit)
ESTIMATED PRODUCTION COST: ~$180 (high-volume, 100+ units)
SUGGESTED RETAIL PRICE: $899


SECTION 5: INSTALLATION AND ASSEMBLY
=====================================

5.1 PCB ASSEMBLY SEQUENCE
──────────────────────────

Order of population (largest to smallest components):
  1. Solder main ICs (ESP32, AD9851, ADF4351) using hot air or reflow
  2. Install power distribution (buck, LDO, charge pump ICs)
  3. Mount SMA connectors (use brass inserts for structural rigidity)
  4. Install RF passive components (inductors, capacitors, resistors)
  5. Mount battery connector (XT60 or JST-VH 2S)
  6. Solder GPIO header pins for debugging

Reflow profile (for lead-free solder):
  Preheat: 160-180°C for 60-90 seconds
  Ramp: 2-3°C/s to 250°C
  Peak: 230-250°C for 10-30 seconds
  Cool: Natural cooling to room temp

5.2 MECHANICAL ASSEMBLY
────────────────────────

Main enclosure assembly:
  1. Print all 5 enclosure parts in PETG (0.15mm layers)
  2. Sand interior surfaces to smooth finish
  3. Insert M5 brass threaded inserts in all mounting posts
  4. Install mounting PCBs on plastic posts with M3 screws
  5. Route power cables (battery, 5V, 3.3V) internally
  6. Mount SMA connectors through front panel
  7. Mount rotary encoder shaft through side aperture
  8. Install TFT display with anti-glare bezel
  9. Close rear access door (battery compartment)

Battery compartment:
  1. Insert 3× 18650 cells in parallel bores (with insulating sleeves)
  2. Mount TP4056 BMS module on PCB posts
  3. Solder series connection between cells (via BMS)
  4. Route power wires to main PCB (positive, negative, temperature)
  5. Attach door hinge at rear
  6. Install magnetic latch on front (keeps door closed in field)

Boom mount adapter:
  1. Print in strong nylon (100% infill for load-bearing)
  2. Sand clamp jaw surfaces smooth
  3. Drill U-bolt fastener holes to 8mm diameter
  4. Tap M5 threaded hole for analyzer tripod mount boss
  5. Assemble clamp jaws around 1" aluminum boom
  6. Screw analyzer base to center post

5.3 INITIAL POWER-ON PROCEDURE
────────────────────────────────

SAFETY: Do not connect antenna while analyzer is powered!

Step 1: Battery Check
  - Verify battery voltage with multimeter (should read 9.6V)
  - If < 8V, charge to full capacity first

Step 2: Powered-Off Self-Test
  - Before power-on, measure key voltages with multimeter:
    * +5V rail @ buck converter output: should read 5.0 ± 0.2V
    * +3.3V rail @ LDO output: should read 3.3 ± 0.1V
    * ±5V analog @ charge pump: should read ±5 ± 0.3V

Step 3: Power-On Sequence
  - Connect battery to analyzer (XT60 connector)
  - Wait 2 seconds for ESP32 boot-up
  - Display should show splash screen (ANT ANALYZER v1.0.0)
  - Listen for any audible issues (clicking, buzzing = problem)

Step 4: LED Diagnostics
  - Green LED (blinking): Normal operation, no active measurement
  - Red LED: Battery low (< 7.5V), charge immediately
  - No LED: Power problem, check battery connection

Step 5: Frequency Verification
  - Set band to 40M using rotary encoder
  - Should display "40M: 7-7.3 MHz"
  - Press button to start single-frequency sweep at 7.0 MHz
  - Display should show magnitude and phase readings

Step 6: RF Output Check
  - With 50Ω dummy load connected to DUT port
  - Measure RF output with oscilloscope at DUT port
  - HF bands (AD9851): Should see -5 to 0 dBm sine wave
  - VHF/UHF bands (ADF4351): Should see RF output after PLL locks

Typical power-on sequence takes ~5 seconds total.


SECTION 6: CALIBRATION AND ALIGNMENT
===================================

6.1 OSL CALIBRATION PROCEDURE
──────────────────────────────

Purpose: Remove systematic errors (directivity, port match, tracking) from
measurements. Procedure requires three calibration standards:
  1. Open: SMA to open air (Z = ∞)
  2. Short: SMA to 50Ω terminator soldered short (Z = 0)
  3. Load: High-quality 50Ω SMA terminator (Z = 50Ω, matched)

Required equipment:
  - OSL calibration kit (commercial 85052D or DIY alternatives)
  - 50Ω dummy load (1/4W or higher power rating)
  - SMA cables (LMR-195 or better, low-loss)
  - 50Ω terminator with SMA male connector

Procedure (via ESP32 firmware menu):

Step 1: Open Standard Calibration
  a. Select band and frequency range to calibrate
  b. Attach OPEN standard to DUT port (air gap, no mechanical contact)
  c. Press CALIBRATE button, select OPEN
  d. Firmware automatically sweeps frequency range
  e. Measures VMAG and VPHS at each frequency point
  f. Stores open standard data in NVS memory
  g. LED blinks green when complete

Step 2: Short Standard Calibration
  a. Remove open standard from DUT port
  b. Attach SHORT standard (50Ω load shorted internally)
  c. Press CALIBRATE, select SHORT
  d. Firmware sweeps frequency, measures and stores
  e. LED blinks green when complete

Step 3: Load Standard Calibration
  a. Remove short from DUT port
  b. Attach 50Ω LOAD terminator (precision quality)
  c. Press CALIBRATE, select LOAD
  d. Firmware sweeps and stores load reference data
  e. LED blinks green when complete

Step 4: Calibration Verification
  a. Disconnect calibration kit from DUT port
  b. Attach 50Ω dummy load to DUT port
  c. Press SWEEP to measure
  d. Display should show:
     * VSWR ≈ 1.0 across entire band
     * Return Loss ≈ -40 dB
     * Impedance ≈ 50 + j0 Ω
  e. If values deviate > 10%, recalibrate or check standards quality

Typical calibration accuracy:
  Magnitude: ±3% (±0.25 dB)
  Phase: ±5° (±0.09 radians)
  VSWR: ±2% in 1-3 range, ±5% in 3-10 range

Calibration data storage:
  - Stored in ESP32 Preferences (NVS flash memory)
  - Survives power-off and firmware updates
  - Can be manually cleared via menu (RESET CALIBRATION)
  - Timestamp recorded (useful for drift tracking)

Frequency bands requiring separate calibration:
  * 160M band: One calibration covers 1.8-2.0 MHz
  * 80M band: One calibration covers 3.5-4.0 MHz
  * ... (continue for all 16 bands)
  * Total time: ~15 minutes for complete 16-band calibration

6.2 FREQUENCY CALIBRATION
──────────────────────────

Purpose: Verify output frequency accuracy (nominally ±2.5 ppm TCXO tolerance)

Equipment needed:
  - Frequency counter with 10 MHz input range
  - OR laptop with software-defined radio (RTL-SDR)
  - OR measurement receiver (transceiver with clarifier dial)

Procedure (Quick Check):

Step 1: Set single frequency
  a. Select 40M band via rotary encoder
  b. Use menu to set frequency to 7.1000 MHz (exact)
  c. Connect frequency counter to DUT port SMA via 3dB coupler

Step 2: Measure output frequency
  a. Frequency counter should display 7.100 MHz ±0.002 MHz (±2 kHz)
  b. If error > 5 kHz at any band, TCXO may need trim

Step 3: Compensation (if needed)
  a. If frequency low: Increase TCXO load capacitance (trim cap)
  b. If frequency high: Decrease TCXO load capacitance
  c. Adjustment range: ±50 ppm (±0.35 kHz @ 7 MHz)

Typical frequency response (no trim cap needed):
  10M band:  +2.5 ppm (7 kHz high)
  40M band:   0.0 ppm (exact)
  2M band:   -1.2 ppm (170 Hz low)

6.3 PHASE CALIBRATION
──────────────────────

Purpose: Verify AD8302 phase measurement linearity and offset

Equipment:
  - Programmable phase shifter (or homebrew with cable delay)
  - Phase measurement reference (network analyzer or phase meter)

Procedure:

Step 1: Zero-phase reference
  a. Connect 50Ω terminator to both DUT and REF ports (matched)
  b. Measure phase output (should read near 0°, ±5° typical)
  c. If offset > 10°, may indicate balun phase mismatch

Step 2: Phase linearity check
  a. Use variable phase shifter (transmission line stub)
  b. Set phase shifter to 0°, 45°, 90°, 135°, 180°
  c. Measure analyzer phase output at each setting
  d. Should be linear ±3° across full range

Step 3: Correction (if needed)
  a. If linear offset (all points shifted same amount): Firmware offset adjustment
  b. If non-linear response: AD8302 balun impedance mismatch
  c. Firmware stores phase offset in preferences, applied automatically


SECTION 7: OPERATION
===================

7.1 SINGLE-FREQUENCY MEASUREMENT
──────────────────────────────────

Purpose: Measure antenna impedance at one specific frequency (quick check)

Procedure:

Step 1: Band and Frequency Selection
  a. Use rotary encoder to select desired band (16 total)
  b. Display shows "40M: 7.0 - 7.3 MHz"
  c. Use menu to enter exact frequency (e.g., 7.1 MHz)

Step 2: Connect Antenna
  a. Attach antenna to DUT port (SMA male connector)
  b. Attach 50Ω dummy load to REF port (if not already connected)
  c. Check cables for damage (exposed center conductor = short circuit)

Step 3: Measurement
  a. Press rotary encoder button to start measurement
  b. Green LED blinks during measurement
  c. Display updates every 500 ms with new data
  d. Takes ~2 seconds for stabilization (transient response)

Step 4: Interpret Results
  Display shows:
    Frequency: 7.1 MHz (current measurement frequency)
    VSWR: 1.3 (voltage standing wave ratio)
    Return Loss: -17.5 dB (how much power reflected)
    Impedance: 58 + j6 Ω (real and imaginary parts)
    S11 Magnitude: -0.145 (reflection coefficient)

Interpretation guide:
  VSWR < 1.5:  Antenna well-matched, good for transmission
  VSWR 1.5-2:  Acceptable for most applications
  VSWR 2-3:    Poor match, efficiency reduced by ~20%
  VSWR > 3:    Very poor, significant power loss

7.2 FREQUENCY SWEEP OPERATION
───────────────────────────────

Purpose: Measure antenna response across entire band (find resonance, Q factor)

Procedure:

Step 1: Band Selection
  a. Select band via rotary encoder
  b. Firmware automatically sets appropriate frequency range

Step 2: Sweep Parameters (optional)
  a. Default: 100 points per band (automatic spacing)
  b. Menu option to adjust sweep points (25, 50, 100, 200)
  c. Lower points = faster sweep, less detail
  d. Higher points = slower sweep, more detail

Step 3: Start Sweep
  a. Press rotary encoder button (hold for 2 seconds)
  b. Analyzer automatically sweeps entire band
  c. Progress indicator shows sweep completion (0-100%)
  d. Takes ~30 seconds for full band sweep (100 points)

Step 4: View Results
  a. Display shows frequency sweep graph (magnitude vs frequency)
  b. X-axis: Frequency in MHz
  c. Y-axis: VSWR or Return Loss (selectable via menu)
  d. Red curve: Antenna response
  e. Blue circle overlay: VSWR = 2:1 reference circle

Step 5: Resonance Analysis
  a. Display automatically finds minimum VSWR point
  b. Highlights resonant frequency in bright green
  c. Reports:
     * f_res: Resonant frequency
     * R_res: Resistance at resonance
     * Q: Quality factor (sharpness of resonance)
  d. Example output: "40M dipole: f_res = 7.08 MHz, R = 72Ω, Q = 9"

7.3 SMITH CHART INTERPRETATION
────────────────────────────────

Display mode: Press CHART button to show Smith chart overlay

Elements on Smith chart:
  * Red dot: Current antenna impedance point
  * Circular rings: VSWR circles (1:1, 2:1, 3:1, 5:1)
  * Horizontal lines: Constant resistance values
  * Curved lines: Constant reactance values
  * Center point (0,0): Perfect 50Ω match

Reading the Smith chart:
  - Right side (positive reactance, X > 0): Antenna is inductive
  - Left side (negative reactance, X < 0): Antenna is capacitive
  - Top: Inductive (coil-like)
  - Bottom: Capacitive (capacitor-like)

Example interpretations:

Dipole at resonance (40M):
  * Point appears on right side, slightly right of center
  * Reactance ≈ +5Ω (slightly inductive)
  * Impedance ≈ 70 + j5Ω
  * VSWR ≈ 1.4:1 (acceptable)
  * Location: Inside 2:1 VSWR circle

Dipole below resonance (lower frequency):
  * Point moves upper-right (more inductive)
  * Reactance becomes more positive (+20 to +50Ω)
  * VSWR increases (moves outward on chart)
  * Impedance: 60 + j30Ω → matching network needed

Dipole above resonance (higher frequency):
  * Point moves lower-left (more capacitive)
  * Reactance becomes negative (-20 to -50Ω)
  * VSWR increases (moves outward on chart)

Smith chart impedance transformation:
  * Each wavelength traveled → one complete rotation around chart center
  * Quarter wavelength → 90° rotation
  * Used for transmission line matching calculations

7.4 RESONANCE DETECTION MODE
──────────────────────────────

Purpose: Automatically find antenna resonance and report Q factor

Activation:
  a. Select band
  b. Press MENU → RESONANCE MODE
  c. Firmware performs fine frequency sweep (small steps)

Automatic algorithm:
  1. Coarse sweep: 1% frequency steps to find approx resonance
  2. Fine sweep: 0.1% steps around the approximate point
  3. Parabolic fitting: Calculate exact resonant frequency with sub-step resolution
  4. Q calculation: Measure bandwidth (-3dB points) and calculate Q = f_res / BW

Output example:
  Resonant frequency: 7.084 MHz
  Resistance @ f_res: 70 Ω
  Reactance @ f_res: +2 Ω (nearly zero, confirms resonance)
  Q factor: 8.2 (moderate sharpness)
  Bandwidth (-3dB): 863 kHz
  Frequency tolerance: ±1 kHz (measurement resolution)

Q factor interpretation:
  Q < 5:   Very broad resonance (use with matching networks)
  Q 5-15:  Normal resonance (typical antennas)
  Q 15-30: Narrow resonance (sharply tuned)
  Q > 30:  Very sharp (use with frequency stability control)

Practical use case:
  User builds 40M dipole, wants to know if it's resonant and sharp.
  Run RESONANCE MODE → Display shows f_res = 7.089 MHz, Q = 10
  → Dipole is well-tuned and ready for operation

7.5 DATA LOGGING AND EXPORT
─────────────────────────────

Logging options:
  1. Manual save: After measurement, press SAVE → data written to SD card
  2. Auto-logging: Continuous measurement saved with GPS timestamp
  3. Remote logging: Bluetooth stream to laptop (real-time)

File format (CSV, human-readable):

Header line:
  Band,Frequency_MHz,VSWR,Return_Loss_dB,Z_Real_Ohms,Z_Imag_Ohms,Gamma_Magnitude,Gamma_Phase_Degrees

Data lines:
  40M,7.100,1.30,-17.5,60.0,5.0,0.145,-11.3
  40M,7.110,1.29,-17.8,61.0,4.0,0.141,-10.5
  40M,7.120,1.28,-18.1,62.0,3.0,0.138,-9.8

Metadata file (same directory):
  {
    "analyzer_model": "ANT-VNA-100",
    "firmware_version": "1.0.0",
    "calibration_date": "2026-04-20T09:30:00Z",
    "calibration_valid": true,
    "measurement_date": "2026-04-24T14:32:15Z",
    "measurement_band": "40M",
    "measurement_frequency_start_mhz": 7.0,
    "measurement_frequency_stop_mhz": 7.3,
    "measurement_points": 100,
    "location_gps": "37.8044N 120.4506W",
    "temperature_celsius": 25,
    "battery_voltage": 9.2,
    "notes": "Dipole resonance check, field work"
  }

Accessing data:
  a. Remove SD card from analyzer
  b. Insert into laptop card reader
  c. Data files in root directory: ANT_*.CSV, ANT_*.JSON
  d. Open in spreadsheet (Excel, LibreOffice) or text editor
  e. Plot in Python (matplotlib), Excel, or Gnuplot


SECTION 8: TROUBLESHOOTING
==========================

8.1 NO DISPLAY OUTPUT
──────────────────────

Symptom: TFT screen is blank, no splash screen on power-up

Possible causes:
  1. Display not powered: Check +3.3V on TFT VCC pin
  2. Display not connected: Verify SPI connections (MOSI, CLK, CS, DC, RST)
  3. Firmware not initialized: Check ESP32 is booting (LED should blink)
  4. Display defective: Test with multimeter (should draw ~50mA at 3.3V)

Troubleshooting steps:
  a. Verify +3.3V at TFT VCC: should read 3.3 ± 0.2V with multimeter
  b. Check SPI signal integrity: Use oscilloscope on MOSI/CLK
     - MOSI/CLK should show ~1 MHz digital signals during boot
  c. Check ESP32 boot sequence: Should see activity on serial monitor (115200 baud)
  d. Measure display backlight current: Should be 30-80 mA
  e. If all above OK, display may be defective (replacement < $15)

8.2 NO RF OUTPUT (NO SYNTHESIZER)
──────────────────────────────────

Symptom: Frequency set, but no RF signal at DUT port (measured with power meter)

Possible causes:
  1. Wrong band selected: Synthesizer may be disabled for that band
  2. DDS not clocked: AD9851 REFIN oscillator absent
  3. PLL not locked: ADF4351 not receiving valid reference
  4. Filter attenuating output: Band-specific filter may have failed
  5. Output switched off: RF mute enabled (protection circuit active)

Troubleshooting steps (HF band, AD9851):
  a. Verify REFIN oscillator: 33.333 MHz should appear at AD9851 pin 3
     Use oscilloscope: should see ~300 mV peak sine wave @ 33.333 MHz
  b. Check parallel data bus: Measure D0-D7 pins
     Should see varying logic levels when frequency changes
  c. Verify FQ_UD strobe: Pin 28 should pulse ~1 Hz when sweeping
  d. Measure output (before filter): Pin 34 should show -5 to 0 dBm RF
  e. If output present but weak: Check output filter (L1, C1, L2, C2)
     Use LCR meter to verify component values

Troubleshooting steps (VHF/UHF band, ADF4351):
  a. Verify REFIN: 25 MHz should be present at REFIN pin
  b. Check SPI connections: CLK, DATA, LE must have correct timing
     CLK frequency ~1 MHz, DATA pulses with CLK, LE strobes after register write
  c. Monitor MUXOUT for lock detect: Should go HIGH (3.3V) within 100ms
  d. If no lock: Check loop filter capacitor (usually 220 pF), charge pump current
  e. Verify +5V on VCO supply (VDD pin): should read 5.0 ± 0.2V

8.3 INCORRECT FREQUENCY MEASUREMENT
─────────────────────────────────────

Symptom: Measured frequency is off by > 5 kHz (outside TCXO tolerance)

Possible causes:
  1. TCXO frequency offset: Normal ±2.5 ppm tolerance
  2. Temperature drift: TCXO output shifts ~0.1 ppm/°C
  3. Reference divider mismatch: Wrong R value in code
  4. Accumulator overflow: Bug in FTW calculation

Troubleshooting:
  a. If offset is consistent (same direction at all frequencies):
     Likely TCXO tolerance. Measure actual TCXO frequency:
     f_tcxo = 33.333 MHz × (measured_freq / expected_freq)
  b. If TCXO is off: Can trim with variable capacitor (±50 ppm range)
     OR accept tolerance and use frequency counter for absolute accuracy
  c. Temperature compensation: If frequency drifts with temperature,
     TCXO is working correctly (0.1 ppm/°C is spec'd)
  d. If error > 50 ppm: May indicate TCXO oscillator failure

8.4 VSWR MEASUREMENT SEEMS WRONG
──────────────────────────────────

Symptom: Antenna known good, but analyzer shows VSWR > 2 (poor match)

Possible causes:
  1. Not calibrated: OSL calibration required for accurate measurement
  2. Wrong 50Ω standard: Used mismatched load instead of precision 50Ω
  3. Loose SMA connector: Bad contact between cable and port
  4. Damaged coax cable: Center conductor oxidized or broken
  5. Reference port terminated incorrectly: Should have 50Ω load

Troubleshooting:
  a. First: Perform OSL calibration (all three standards)
     See Section 6.1 for detailed procedure
  b. Test with known 50Ω load:
     Install 50Ω terminator on DUT port
     Should measure VSWR = 1.0 ± 0.05 across entire band
     If VSWR ≠ 1.0, recalibrate
  c. Test SMA connectors:
     Connect DUT and REF cables directly together (short cable loop)
     Should measure VSWR ≈ 1.0 (within ±0.1)
     If not, coax cable may be damaged
  d. Verify reference port: Must have 50Ω terminator (not open circuit)
     If open, measurement will be wrong (directional coupler unbalanced)

8.5 OVERVOLTAGE PROTECTION TRIGGERED
──────────────────────────────────────

Symptom: Display shows "OVERVOLTAGE - MUTED", RF path disabled

Possible causes:
  1. Live transmitter accidentally connected to DUT port
  2. Lightning transient on antenna cable
  3. High-power adjacent transmitter coupling into analyzer
  4. AD8302 balun impedance mismatch (false trigger)

Troubleshooting:
  a. Do NOT continue measurement
  b. Disconnect antenna from DUT port immediately
  c. Check antenna and coax for damage (burned insulation, cracked connectors)
  d. Turn off transmitters in vicinity
  e. Allow analyzer to cool for 5 minutes
  f. Reconnect antenna and resume measurement
  g. Firmware automatically unmutes after successful new measurement

Prevention:
  - Never operate analyzer with transmitter connected
  - Use protective cover over DUT port when not measuring
  - Keep antenna cables away from transmitter feedlines
  - Use ferrite chokes on power and signal cables


SECTION 9: MAINTENANCE AND CARE
================================

9.1 BATTERY MAINTENANCE
────────────────────────

LiFePO4 chemistry (safer than Li-ion):
  - No self-heating tendency
  - Excellent cycle life (2000+ cycles @ 20% DoD)
  - Flat discharge curve (9.6V → 8V over 90% of capacity)

Monthly maintenance:
  a. Check visual appearance: No puffing, swelling, or corrosion
  b. Weigh battery pack (should be consistent, weight gain = water absorption)
  c. Measure voltage with multimeter: Should read 9.6V ± 0.2V when rested
  d. If voltage drifts: Battery may be aging (replace after 2000 cycles)

Long-term storage (> 6 months):
  a. Discharge to 50% capacity (4.8V) before storage
  b. Store in cool place (10-15°C ideal, 0-25°C acceptable)
  c. Every 3 months: Check voltage, recharge to 50% if drifted
  d. Before resuming use: Full charge, discharge/recharge cycle

Battery replacement procedure:
  a. Remove analyzer from enclosure (via rear door access)
  b. Disconnect battery connector (XT60 or JST-VH)
  c. Remove 3× 18650 cells from holders (push out from bottom)
  d. Dispose of old cells properly (Li+ recycling center, not trash)
  e. Insert new 3× 18650 LiFePO4 cells (observe polarity: + on top)
  f. Reconnect BMS module (positive/negative wires)
  g. Reconnect battery connector to main PCB

Cost: ~$24 for 3× 18650 LiFePO4 cells (AME, Samsung, Sanyo brands)
Time: 10 minutes

9.2 FIELD CARE AND WEATHER PROTECTION
───────────────────────────────────────

Analyzer is NOT waterproof (electronics are exposed, SMA ports unprotected)

Weather precautions:
  - Cover analyzer with rain protection (plastic bag, dry case)
  - Avoid direct water spray or condensation on SMA ports
  - If wet, allow 30 minutes air-dry before powering on
  - Store in waterproof dry bag when not in use

Temperature extremes:
  - Operating range: -10°C to +70°C (spec'd)
  - Avoid rapid temperature changes (condensation risk)
  - In freezing conditions: Allow 10 minutes warmup before use
  - In hot sun: Place in shade, monitor battery (thermal shutdown at >75°C)

Shock and vibration:
  - Analyzer designed for field use (rugged 3D-printed enclosure)
  - Dropped from 1m onto concrete: likely functional (has cushioning)
  - Long-distance transport: Pack in foam-lined hard case
  - Avoid placing heavy objects on analyzer (SMA ports are fragile)

9.3 FIRMWARE UPDATES
──────────────────────

New firmware versions available via:
  a) USB serial cable (direct programming via Arduino IDE)
  b) Over-the-air (OTA) via WiFi (future expansion)

To update firmware:
  1. Download latest firmware .ino file from project repository
  2. Install Arduino IDE (free, arduino.cc)
  3. Install ESP32 board support in IDE
  4. Connect analyzer to laptop via USB micro cable
  5. Open .ino file in IDE
  6. Select Board: "ESP32 Dev Module"
  7. Select COM port (usually COM3-COM9)
  8. Click "Upload"
  9. Wait 30 seconds for upload to complete
  10. LED blinks during reboot (normal)

Backup before updating:
  a. Insert SD card in analyzer
  b. Calibration data automatically saved to NVS (survives update)
  c. No data loss expected

9.4 PERIODIC INSPECTION CHECKLIST
────────────────────────────────────

Quarterly (every 3 months):
  ☐ Visual inspection: Check enclosure for cracks or damage
  ☐ SMA connectors: Verify no bent pins, corrosion on contact
  ☐ Battery voltage: Measure at rest (should be 9.6V ± 0.2V)
  ☐ Power-on test: Verify display, LED, RF output present
  ☐ Coax cables: Check for visible damage, kinks, exposed center conductor

Semi-annually (every 6 months):
  ☐ Calibration check: Measure 50Ω load, verify VSWR ≈ 1.0
  ☐ Battery capacity: Run full discharge/charge cycle, note runtime
  ☐ Frequency accuracy: Spot-check 3-4 bands with frequency counter
  ☐ Connector maintenance: Clean SMA contacts with isopropyl alcohol

Annually:
  ☐ Full recalibration: OSL calibration across all 16 bands
  ☐ Battery age assessment: If > 2000 cycles, plan replacement
  ☐ Firmware audit: Check for available updates
  ☐ Environmental exposure log: Note any water, temperature extremes, shocks


SECTION 10: PARTS LIST AND PROCUREMENT
======================================

[COMPLETE BOM TABLE - See Section 4.1 for detailed listing]

Recommended suppliers:
  DigiKey (www.digikey.com): ICs, passives, cables
  Amazon (amazon.com): Display, enclosure materials, tools
  Fair-Rite (fair-rite.com): Ferrite cores direct
  eBay: Alternative sourcing, clearance items
  AliExpress: Low-cost alternatives (longer lead times)

Lead times:
  Standard components: 1-2 weeks
  Specialized parts (AD8302, ADF4351): 2-4 weeks
  3D printing materials: 5-7 days
  PCB fabrication (JLCPCB, OSHpark): 5-10 days

Total build time (one unit): 40-60 hours
  PCB assembly: 15 hours (hand soldering, solder reflow)
  Mechanical assembly: 10 hours (3D printing, assembly, testing)
  Testing and calibration: 10 hours
  Debugging/tweaking: 15 hours (if needed)

Cost optimization:
  - Buy components in batches (25+ units): 30% cost reduction
  - Use JLCPCB SMT assembly: Save 10 hours labor, minor cost
  - Substitute display: Any 3.5" TFT ILI9341 controller compatible (costs $8-25)
  - DIY calibration standards: Build open/short from hardware store items


APPENDIX A: NEC2 ANTENNA MODELS
===============================

Reference antenna models for testing analyzer accuracy:

1. 10M REFERENCE DIPOLE (28.1 MHz)
   ─────────────────────────────────
   NEC model: test_dipole_10m.nec
   Properties:
     - Type: Half-wave dipole
     - Frequency: 28.1 MHz
     - Height: 10m AGL over average soil
     - Expected impedance: 45-55Ω (should near 50Ω at resonance)
     - Expected VSWR: 1.0-1.2:1

   Use case: Verify analyzer resonance detection on 10M band


2. 2M REFERENCE DIPOLE (144.2 MHz)
   ────────────────────────────────
   NEC model: test_dipole_2m.nec
   Properties:
     - Type: Half-wave dipole (free space)
     - Frequency: 144.2 MHz
     - Height: 6m AGL
     - Expected impedance: ~50Ω (ideal center-fed)
     - Expected VSWR: 1.0:1

   Use case: VHF band resonance check


3. 70cm GROUND PLANE VERTICAL (432 MHz)
   ────────────────────────────────────
   NEC model: test_groundplane_70cm.nec
   Properties:
     - Type: Vertical monopole on ground plane (4 radials)
     - Frequency: 430-432 MHz
     - Height: 5m AGL
     - Expected impedance: 35-40Ω (monopole characteristic)
     - Expected VSWR: 1.3-1.5:1 (slightly capacitive)

   Use case: UHF band analyzer verification


APPENDIX B: SMITH CHART REFERENCE
===================================

[ASCII Smith Chart representation]

The Smith chart is a graphical tool for impedance matching and visualization:

Key points on Smith chart:
  ┌─────────────────────────────────────────────┐
  │         SMITH CHART ORIENTATION             │
  ├─────────────────────────────────────────────┤
  │  LEFT edge (-1)        CENTER (0)  RIGHT (1)│
  │  SHORT circuit         MATCH       OPEN     │
  │  Z = 0Ω               Z = 50Ω      Z = ∞   │
  │                                             │
  │  TOP: Inductive (L)                         │
  │  BOTTOM: Capacitive (C)                     │
  │                                             │
  │  CIRCLES: Constant resistance lines         │
  │  CURVES: Constant reactance lines           │
  │  OUTER CIRCLE: VSWR reference circles       │
  └─────────────────────────────────────────────┘

Applications:
  1. Impedance transformation: Move along transmission line length
  2. Matching network design: Find capacitor/inductor values needed
  3. Transmission line stub calculation: λ/4, λ/2 stubs
  4. Antenna resonance tuning: Visual feedback of impedance change


APPENDIX C: S-PARAMETER REFERENCE TABLES
=========================================

Standard S-parameter definitions (2-port network):

[TABLE: S-parameters vs antenna types]

Antenna Type        | S11 (dB) | VSWR | Return Loss | Use Case
─────────────────────────────────────────────────────────────────
Resonant dipole     | -14 to -17 | 1.3-1.5 | 14-17 dB | Typical operation
Tuned monopole      | -12 to -15 | 1.5-1.8 | 12-15 dB | Ground-plane antenna
Broadband antenna   | -8 to -10  | 2.0-2.5 | 8-10 dB  | Wideband coverage
Mismatched load     | -5 to -6   | 3.0-4.0 | 5-6 dB   | Poor match, efficiency loss
Open circuit        | 0 dB       | ∞       | ∞ dB     | Complete reflection
50Ω load            | -40 dB     | 1.0:1   | 40 dB    | Perfect match (reference)

Conversion formulas:
  VSWR = (1 + |Γ|) / (1 - |Γ|)
  Return Loss (dB) = -20 log₁₀(|Γ|)
  Magnitude (dB) = 20 log₁₀(|Γ|)
  Power reflected = |Γ|² × Power incident
  Power transmitted = (1 - |Γ|²) × Power incident


APPENDIX D: FREQUENCY ALLOCATION TABLE (IARU Region 2, USA)
===========================================================

Band Name | Frequency Range | Type | US Call Authority | Notes
──────────────────────────────────────────────────────────────
160M      | 1.8-2.0 MHz     | HF   | FCC 47 CFR 97.3   | Class A only
80M       | 3.5-4.0 MHz     | HF   | FCC 47 CFR 97.3   | Daytime congestion
40M       | 7.0-7.3 MHz     | HF   | FCC 47 CFR 97.3   | Major HF band
30M       | 10.1-10.15 MHz  | HF   | FCC 47 CFR 97.3   | CW only, narrow
20M       | 14.0-14.35 MHz  | HF   | FCC 47 CFR 97.3   | Popular DX band
17M       | 18.07-18.17 MHz | HF   | FCC 47 CFR 97.3   | Secondary allocation
15M       | 21.0-21.45 MHz  | HF   | FCC 47 CFR 97.3   | Daytime propagation
12M       | 24.89-24.99 MHz | HF   | FCC 47 CFR 97.3   | Rare allocation
10M       | 28.0-29.7 MHz   | HF   | FCC 47 CFR 97.3   | Solar cycle dependent
6M        | 50.0-54.0 MHz   | VHF  | FCC 47 CFR 97.3   | Sporadic E conditions
2M        | 144.0-148.0 MHz | VHF  | FCC 47 CFR 97.3   | Primary repeater band
1.25M     | 222.0-225.0 MHz | VHF  | FCC 47 CFR 97.3   | 1.25cm band (deprecated)
70cm      | 430-450 MHz     | UHF  | FCC 47 CFR 97.3   | Repeater satellite band
33cm      | 902-928 MHz     | UHF  | FCC 47 CFR 97.3   | ISM shared, unlicensed
20cm      | 1.2-1.3 GHz     | UHF  | FCC 47 CFR 97.3   | ATV/repeater band


APPENDIX E: CALIBRATION DATA SHEETS (BLANK TEMPLATES)
======================================================

[Blank form for recording OSL calibration data]

OSL CALIBRATION RECORD
──────────────────────
Date: ____________
Operator: ____________
Location: ____________
Temperature: ____ °C
Humidity: ____ %
Calibration Kit Used: ____________ (model/serial)
Analyzer Serial: ____________

Band: ______ Frequency Range: _____ to _____ MHz

OPEN STANDARD MEASUREMENT:
Point | Frequency | VMAG(V) | VPHS(V) | Date
──────────────────────────────────────────
  1   |           |         |        |
  2   |           |         |        |
  3   |           |         |        |
 ...  |           |         |        |

SHORT STANDARD MEASUREMENT:
[Same table format]

LOAD (50Ω) MEASUREMENT:
[Same table format]

VERIFICATION (50Ω load @ mid-band frequency):
Expected VSWR: 1.0:1
Measured VSWR: _____:1
Acceptable: ☐ YES ☐ NO

Notes:
_______________________________________________________________________
_______________________________________________________________________

Operator Signature: _________________________ Date: __________


================================================================================
END OF INTEGRATED ANTENNA ANALYZER TECHNICAL MANUAL
Document version: 1.0.0, dated 2026-04-24
For support and updates, contact: [project repository URL]
================================================================================
