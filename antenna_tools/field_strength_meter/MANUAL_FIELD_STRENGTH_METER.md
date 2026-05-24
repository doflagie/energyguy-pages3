================================================================================
TECHNICAL MANUAL
RF FIELD STRENGTH METER — ANTENNA TESTING AND RF DETECTION
TM-FSM-001 Rev A

FIELD STRENGTH METER SET, RF, PORTABLE
PASSIVE / ACTIVE / DIGITAL (CYD) VARIANTS
1 MHz – 1300 MHz, S-METER, dBm, V/m, DATA LOGGING

NSN: XXXX-XX-XXX-XXXX (LOCAL FABRICATION)
================================================================================

THIS DOCUMENT CONTAINS TECHNICAL INFORMATION FOR THE CONSTRUCTION,
OPERATION, CALIBRATION, AND MAINTENANCE OF THE RF FIELD STRENGTH
METER SET, PORTABLE.

PRIOR TO PERFORMING ANY PROCEDURE, READ ALL APPLICABLE WARNINGS,
CAUTIONS, AND NOTES. FAILURE TO COMPLY MAY RESULT IN INJURY OR
EQUIPMENT DAMAGE.

================================================================================
RECORD OF CHANGES
================================================================================

Rev    Date          Description                         By
----   ----------    ---------------------------------   ----
A      2026-04-25    Initial release                     ---

================================================================================
TABLE OF CONTENTS
================================================================================

CHAPTER 1  GENERAL INFORMATION
CHAPTER 2  THEORY OF OPERATION
CHAPTER 3  EQUIPMENT DESCRIPTION
CHAPTER 4  CONSTRUCTION
CHAPTER 5  CALIBRATION PROCEDURES
CHAPTER 6  OPERATING PROCEDURES
CHAPTER 7  TROUBLESHOOTING
CHAPTER 8  MAINTENANCE
CHAPTER 9  PARTS LIST
APPENDIX A  ANTENNA FACTOR TABLES
APPENDIX B  S-METER REFERENCE TABLE
APPENDIX C  BAND FREQUENCY REFERENCE
APPENDIX D  FIELD STRENGTH CONVERSION TABLES
APPENDIX E  DATA LOGGING CSV FORMAT
APPENDIX F  GLOSSARY

================================================================================
CHAPTER 1  GENERAL INFORMATION
================================================================================

1-1. PURPOSE AND SCOPE

1-1.1  This manual provides complete technical data for the construction,
operation, calibration, and field maintenance of the RF Field Strength
Meter Set (hereinafter "FSM" or "the instrument").

1-1.2  The FSM set consists of three instrument variants:

  a.  FSM-P (PASSIVE) — Battery-free germanium diode detector with
      analog moving-coil meter. Frequency range 100 kHz–500 MHz.
      No power required. Provides relative field strength indication.

  b.  FSM-A (ACTIVE) — ERA-3SM MMIC preamplifier with AD8307
      logarithmic detector and analog meter output. Frequency range
      1–500 MHz. Provides calibrated dBm readings. Includes audio
      tone output proportional to field strength.

  c.  FSM-D (DIGITAL) — ESP32-2432S028 (CYD) microcontroller with
      ILI9341 2.8" TFT display, AD8307 detector, SD card data logger
      with GPS coordinate tagging, and audio tone output. Frequency
      range 1 MHz–1300 MHz (with appropriate probe). Provides S-meter,
      dBm, V/m, dBμV/m display with Smith-chart-style direction
      finding capability. Firmware: field_strength_cyd.ino.

1-1.3  All three variants share a common probe set:
  - 50 cm telescoping whip antenna
  - λ/2 adjustable dipole with T-bar handle
  - 12.7 cm shielded single-turn loop (HF direction finding)
  - 3-element 2M/70cm mini-Yagi probe (VHF/UHF)
  - Calibrated terminated dipole (for traceable V/m measurements)

1-1.4  The FSM set is intended for:
  a.  Verifying antenna radiation patterns and relative gain
  b.  Locating sources of RF interference
  c.  Measuring antenna field strength during transmitter testing
  d.  Direction finding (locating hidden transmitters)
  e.  Characterizing propagation paths in the field
  f.  Compliance verification (non-traceable; for comparative use)

--------------------------------------------------------------------------------

1-2. PRINCIPAL CHARACTERISTICS

  Parameter              FSM-P          FSM-A          FSM-D
  ---------              -----          -----          -----
  Frequency range        100kHz–500MHz  1–500 MHz      1–1300 MHz
  Dynamic range          ~30 dB         85 dB          85 dB
  Sensitivity            ~0.5 mV/m      ~0.01 mV/m     ~0.01 mV/m
  Indication             Analog meter   Analog + audio CYD TFT display
  Calibration            Relative only  dBm ±2 dB      dBm ±2 dB
  Field strength         No             No             V/m with AF
  S-meter                No             Derived        Yes (IARU std)
  Data logging           No             No             SD card + GPS
  Audio output           No             Yes (555 VCO)  Yes (PWM tone)
  Direction finding      Loop probe     Loop probe     Loop + bearing
  GPS integration        No             No             Yes (NEO-6M)
  Power                  None           9V PP3         9V PP3 (×2)
  Battery life           N/A            ~7 hours       ~4 hours
  Dimensions             120×65×35 mm   160×100×48 mm  160×100×48 mm
  Weight                 ~150 g         ~300 g         ~380 g

--------------------------------------------------------------------------------

1-3. SAFETY SUMMARY

WARNING

1.  RF HAZARD — The FSM detects radiated RF fields. During
    measurement, the operator may be in the RF near field of a
    transmitting antenna. Observe all RF safety guidelines per
    ARRL RF Exposure guidelines or applicable regulations.
    Do not make measurements closer to a transmitting antenna
    than the minimum safe distance for the operating power.

2.  NO TRANSMITTER CONNECTION — These instruments are passive
    or low-level detection devices. They are NOT designed to
    accept transmitter output power. Maximum safe input at any
    probe connector is +10 dBm (10 mW). Connecting a transmitter
    output to any probe connector WILL destroy the instrument.

3.  BATTERY POLARITY — Reversed battery polarity will
    destroy MMIC devices (ERA-3SM, MAR-6SM) instantly.
    Verify polarity before every battery installation.

4.  STATIC DISCHARGE — ERA-3SM and ESP32 are static-sensitive.
    Use ESD precautions during assembly and service.

CAUTION

1.  The AD8307 input is damaged by input signals above +17 dBm.
    When measuring near a transmitter, always start with the
    maximum attenuator setting (−30 dB) and reduce only if needed.

2.  Do not operate the FSM-D in rain without additional
    weatherproofing for the GPS and SD card connections.
    The CYD display is not sealed against moisture.

3.  GPS fix requires clear sky view. Indoor or obstructed
    operation will prevent GPS position logging.

NOTE

1.  Field strength measurements are inherently influenced by
    reflections, ground conductivity, antenna orientation, and
    nearby metallic objects. All measurements should be interpreted
    as relative comparisons unless a full calibrated reference
    measurement sequence has been performed.

2.  The FSM-D antenna factor table in firmware is derived from
    theoretical free-space values. Actual antenna factor varies
    with frequency and probe construction. For precision
    measurements, perform user AF calibration per Chapter 5.

================================================================================
CHAPTER 2  THEORY OF OPERATION
================================================================================

2-1. FIELD STRENGTH AND POWER DENSITY

2-1.1  An electromagnetic wave propagating through free space carries
power. The power density S (W/m²) at any point is related to the
electric field intensity E (V/m) by:

    S = E² / (120π) = E² / 377         [Equation 2-1]

where 120π ≈ 377 Ω is the impedance of free space.

2-1.2  Conversely, given a received power Pr at a load resistance
RL, the equivalent open-circuit voltage Voc across an antenna is
related to the field strength by the effective height he:

    Voc = E × he                        [Equation 2-2]

For a half-wave dipole in free space:
    he = λ/π ≈ 0.318 λ                  [Equation 2-3]

Example at 14.175 MHz (λ = 21.17 m):
    he = 21.17 / π = 6.74 m
    If E = 1 V/m: Voc = 1 × 6.74 = 6.74 V (open circuit)
    Matched output into 50Ω: V = Voc/2 = 3.37 V, P = V²/50 = 227 mW = +23.6 dBm

This illustrates why a gain antenna at close range can overwhelm
the FSM even at low transmitter power. Always use the attenuator.

2-1.3  FREE-SPACE PATH LOSS

For a transmitter of power Pt into an isotropic antenna:
    E (V/m) = √(30 × Pt × Gt) / d      [Equation 2-4]

Where:
    Pt = transmitter power (watts)
    Gt = transmitter antenna gain (numeric: 1 = 0 dBi)
    d  = distance from transmitter (meters)

Example: 100W into a dipole (Gt = 1.64) at 10 meters:
    E = √(30 × 100 × 1.64) / 10 = √4920 / 10 = 70.1 / 10 = 7.0 V/m

Example: Same at 100 meters:
    E = 7.0 / 10 = 0.70 V/m

2-1.4  FIELD STRENGTH UNITS

  V/m:       SI unit, field amplitude
  dBμV/m:   20 × log10(E_V/m × 10⁶) = 20 log10(E) + 120 dB  (ref = 1 μV/m)
  dBV/m:    20 × log10(E)
  mV/m:     E × 1000
  μV/m:     E × 10⁶

  Common reference levels:
    S9 signal at antenna:      E ≈ 50 μV into 50Ω → E_field depends on AF
    VHF FM broadcast 30 km:    ~0.01–0.1 V/m (30–100 dBμV/m)
    AM broadcast 10 km:        ~0.1–1 V/m    (100–120 dBμV/m)
    10W QRP at 100m, 40M:      ~0.03 V/m     (~90 dBμV/m)
    100W at 10m from antenna:  7 V/m         (~137 dBμV/m)

--------------------------------------------------------------------------------

2-2. ANTENNA FACTOR

2-2.1  The antenna factor (AF) of a receive antenna is the ratio
of the incident electric field to the voltage measured at the
antenna terminals (into a standard 50Ω load):

    AF = E / V_terminal                  [Equation 2-5]
    AF (dB/m) = E_dBμVm − V_dBμV       [Equation 2-6]

2-2.2  For a λ/2 dipole (theoretical free space, 50Ω load):
    AF (dB/m) = 20 × log10(4π × f / c) = 20 log10(f_MHz) − 29.8
    where c = 3×10⁸ m/s

    At 14.175 MHz: AF = 20 log10(14.175) − 29.8 = 23.0 − 29.8 = −6.8 dB/m

    NOTE: A negative AF means the antenna produces a larger voltage
    than the field strength value (because the antenna is larger than 1 m
    in effective height at low HF frequencies).

2-2.3  For a short monopole of physical length L (L << λ):
    AF (dB/m) ≈ 20 log10(λ / (2πL)) + 6 dB (rough approximation)
    A 50 cm whip at 7 MHz: AF ≈ 20 log10(42 / (2π × 0.5)) ≈ 20 log10(13.4) ≈ +22 dB/m

2-2.4  The FSM firmware stores pre-computed AF values for three probe types
at each band. User calibration can apply a correction offset (±20 dB range)
stored in EEPROM. See Chapter 5 for calibration procedure.

--------------------------------------------------------------------------------

2-3. LOGARITHMIC DETECTOR OPERATION (AD8307)

2-3.1  The AD8307 is a successive-detection logarithmic amplifier
covering −75 to +17 dBm (92 dB dynamic range) from 1 MHz to 500 MHz.
Its output voltage is:

    V_out = slope × (P_in_dBm − intercept)
          = 25 mV/dB × (P_in_dBm + 84 dBm)    [Equation 2-7]

At −75 dBm input: V_out = 0.025 × (−75 + 84) = 0.225 V
At   0 dBm input: V_out = 0.025 × (  0 + 84) = 2.100 V
At +17 dBm input: V_out = 0.025 × ( 17 + 84) = 2.525 V

2-3.2  The AD8307 output is read by the ESP32 12-bit ADC (GPIO34).
With the 3.3V ADC reference:
    P_dBm = (ADC_count × 3.3 / 4095) × 40 − 84

2-3.3  The ERA-3SM preamplifier (20 dB gain, P1dB = +12 dBm) extends
the system minimum detectable signal to approximately:
    MDS = AD8307_min − ERA_gain = −75 − 20 = −95 dBm at probe input.

With a λ/2 dipole probe (AF ≈ −7 dB/m at 14 MHz), the minimum
detectable field is:
    E_min_dBuVm = MDS_dBm + 107 + AF = −95 + 107 + (−7) = +5 dBμV/m
    E_min = 10^(5/20) × 10⁻⁶ = 1.78 μV/m ≈ 0.002 mV/m (excellent sensitivity)

--------------------------------------------------------------------------------

2-4. S-METER CALIBRATION (IARU STANDARD)

2-4.1  The S-meter scale is defined by the International Amateur Radio
Union (IARU) as follows for HF (below 30 MHz):

    S1 = −121 dBm (input at 50Ω)
    Each S-unit = 6 dB
    S9 = −73 dBm = 50 μV at 50Ω

    Above S9: reported as "S9 + X dB"
    Example: −43 dBm = S9 + 30 dB

2-4.2  For VHF (above 30 MHz), IARU recommends:
    S9 = −93 dBm (stricter reference)
    Each S-unit still = 6 dB

2-4.3  FSM firmware uses the HF standard (S9 = −73 dBm) for all
bands. A VHF correction of +20 dB can be applied in the settings.

--------------------------------------------------------------------------------

2-5. DIRECTION FINDING WITH A LOOP ANTENNA

2-5.1  A small shielded loop antenna has a figure-8 radiation pattern:
maximum response broadside (perpendicular to loop plane), and sharp
nulls on the loop axis. The null is typically 20–30 dB below maximum.

2-5.2  SENSE AMBIGUITY

The basic loop has a 180° ambiguity: the null direction points both
toward and away from the transmitter. To resolve this ambiguity, one
of the following methods is used:
  a.  CARDIOID PATTERN: Combine loop with a vertical whip (add an
      "adcock" sense antenna). The combined pattern breaks the 180°
      symmetry, giving a single null direction.
  b.  ASYMMETRIC LOOP TECHNIQUE: Observe which side of the null
      the signal recovers more rapidly while moving toward the source.
  c.  POSITION AMBIGUITY RESOLUTION: Take two bearings from different
      positions; the intersection identifies the transmitter location.

2-5.3  FSM-D DIRECTION FINDER (Screen 2)

The CYD direction finder screen displays:
  - Current RF level in dBm (tracks in real time)
  - Peak level bearing (bearing where maximum was recorded)
  - Minimum level (null depth indicator)
  - Bearing readout updated by operator pressing BEAR+ / BEAR− buttons

Operator rotates the loop probe, manually incrementing the bearing
display with the touch buttons as the loop is rotated. The firmware
records the bearing at peak level. This provides a bearing to the
strongest source at the measurement frequency.

================================================================================
CHAPTER 3  EQUIPMENT DESCRIPTION
================================================================================

3-1. FSM-P (PASSIVE) DESCRIPTION

3-1.1  The FSM-P consists of:
  a.  1N34A germanium point-contact diode (envelope detector)
  b.  10-position rotary band switch with parallel LC tuned circuits
      providing ~6 dB selectivity per octave at each band
  c.  100 kΩ logarithmic-taper sensitivity potentiometer
  d.  100 μA FSD moving-coil panel meter (60×40 mm face)
  e.  ASA enclosure (120×65×35 mm) with meter face window

3-1.2  The FSM-P requires no external power. The detector diode
rectifies the received RF, producing a DC voltage proportional to
the RF envelope. The meter deflects in proportion to the detected
signal strength.

3-1.3  FREQUENCY RESPONSE

The FSM-P is inherently broadband. Without a tuned circuit, the
detector responds to all signals across its frequency range. The
band switch tuned circuit provides some selectivity, but a strong
out-of-band signal may still deflect the meter. This is acceptable
for relative measurements. For selective measurements, the FSM-A or
FSM-D is preferred.

3-1.4  MINIMUM DETECTABLE SIGNAL

With a 50 cm whip probe and 100 μA FSD meter at 1/4 deflection:
    At 7 MHz (40M):  approximately 1 mV/m (5 mV at meter for ≥5% deflection)
    At 14 MHz (20M): approximately 0.5 mV/m
    At 144 MHz (2M): approximately 0.2 mV/m

These values assume the sensitivity pot is at maximum and the band
filter is set to the correct band.

--------------------------------------------------------------------------------

3-2. FSM-A (ACTIVE) DESCRIPTION

3-2.1  The FSM-A adds an ERA-3SM MMIC preamplifier and AD8307
logarithmic detector to the passive detection chain. Key features:

  a.  Input protection: BAT54S Schottky clamp diodes limit to ±3.3V
  b.  4-position Pi attenuator (0 / −10 / −20 / −30 dB, all 50Ω)
  c.  ERA-3SM preamp: 20 dB gain, 0.1–3000 MHz, P1dB = +12 dBm
  d.  AD8307: −75 to +17 dBm, 25 mV/dB slope
  e.  LM358 op-amp meter driver with zero/span potentiometers
  f.  NE555 audio VCO: tone frequency proportional to field strength

3-2.2  ATTENUATOR

The 4-position rotary switch selects:
  Position 0 (DIRECT): Full ERA-3SM gain. Use for weak fields.
  Position 1 (−10 dB): Reduced gain. Use for moderate fields.
  Position 2 (−20 dB): Moderate attenuation. Use near transmitters.
  Position 3 (−30 dB): Maximum attenuation. Use very close to antennas
                        or for high-power transmitter near-field work.

CAUTION: Always start at −30 dB when first connecting to an unknown
signal environment. Progressively reduce attenuation until the meter
reads in the upper half of its scale.

3-2.3  AUDIO TONE

The 555 VCO frequency is controlled by the AD8307 output voltage.
The tone frequency increases with increasing field strength.
  Weak field (−75 dBm):    ~200 Hz low hum
  Medium field (−40 dBm):  ~800 Hz medium tone
  Strong field (0 dBm):    ~2500 Hz high pitch
  Overload (>+10 dBm):     ~3500 Hz continuous high tone

The audio output is a useful indicator when the meter is not
visible (e.g., climbing a tower, adjusting an antenna).

--------------------------------------------------------------------------------

3-3. FSM-D (DIGITAL / CYD) DESCRIPTION

3-3.1  The FSM-D uses the CYD board (ESP32-2432S028) as its
controller and display. All RF circuitry is identical to FSM-A
(ERA-3SM + AD8307) but the meter and audio VCO are replaced by
the CYD display and ESP32 PWM audio.

3-3.2  DISPLAY SCREENS

Six operating screens are provided, selectable by touch:

  SCREEN 0 — S-METER MAIN (default power-on screen)
    Horizontal S-meter bargraph (S1–S9+30 dB scale)
    Large dBm readout (2× character size)
    S-unit readout (e.g., "S6" or "S9+15dB")
    Field strength: V/m and dBμV/m
    Probe terminal voltage: μV or mV
    GPS coordinates (if fix acquired)
    Touch buttons: BND− | BND+ | HOLD | LOG | DIR | SET

  SCREEN 1 — CALIBRATED READOUT
    Extra-large dBm (3× characters)
    V/m and dBμV/m
    Antenna factor readout and user correction
    S-unit reading

  SCREEN 2 — DIRECTION FINDER
    Compass rose graphic
    Current/peak bearing pointers
    Peak and null dBm levels
    Touch buttons: RESET | BACK | BEAR+ | BEAR−

  SCREEN 3 — DATA LOGGER
    GPS status (fix/acquiring, coordinates, satellites, altitude)
    Log file name, entry count
    Recent log entries (last 5 lines of CSV)
    Touch buttons: AUTO (toggle) | LOG NOW | CLEAR | BACK

  SCREEN 4 — BAND SELECT
    15-button grid: 160M through 20cm
    Highlighted selected band

  SCREEN 5 — SETTINGS
    Attenuator (0/10/20/30 dB, toggles via ATT+ / ATT− buttons)
    Audio mode (Proportional / Pulse / Off)
    Probe type (WHIP / DIPOLE / LOOP — affects AF used)
    AF user correction offset (±20 dB in 1 dB steps)
    Log interval (Manual / 5s / 10s / 30s / 60s)
    Backlight brightness
    ADC offset and scale (calibration constants)
    SAVE: writes all settings to EEPROM

3-3.3  DATA LOGGING

Log entries are written to an SD card in CSV format. One file per
day, named FSM_YYYYMMDD.CSV. If no GPS fix, file is FSM_LOG.CSV.

Each entry records: UTC time, date, GPS coordinates, altitude,
satellite count, band name, frequency, attenuator setting, dBm level,
probe terminal voltage (μV), field strength (V/m), S-units, notes.

The log file is compatible with Excel, LibreOffice Calc, or Python
pandas for post-processing. A sample Python analysis script is
provided in Appendix E.

3-3.4  GPS INTEGRATION

The GPS module (u-blox NEO-6M or compatible) connects via UART2
(GPIO16 RX, GPIO17 TX) at 9600 baud. NMEA 0183 sentences $GPRMC
and $GPGGA are parsed for position, time, and altitude.

After cold power-on, GPS fix acquisition requires 90–180 seconds.
The green status LED (GPIO27) blinks once per second while acquiring
and twice per second when a fix is obtained.

3-3.5  AUDIO OUTPUT

GPIO25 PWM tone output feeds an RC low-pass filter and LM386 audio
amplifier driving an 8Ω speaker. Three modes:

  PROPORTIONAL: Continuous tone, frequency proportional to dBm level.
  PULSE: Geiger-counter-style click rate proportional to field strength.
  OFF: Audio disabled.

Mode is selected in Settings screen and saved to EEPROM.

================================================================================
CHAPTER 4  CONSTRUCTION
================================================================================

4-1. FSM-P CONSTRUCTION

4-1.1  PARTS REQUIRED

  See schematics/sch_passive_fsm.txt for complete schematic.

  Ref    Value           Description
  ---    -----           -----------
  D1     1N34A           Germanium diode (point-contact)
  R1     100 kΩ          Detector load
  R2     10 kΩ           Meter series resistor
  R3     1 kΩ trimmer    Zero adjust
  C1     100 pF NPO      RF bypass
  C2     10 μF/16V       Integrating capacitor
  VR1    100 kΩ log      Sensitivity pot
  M1     100 μA FSD      Panel meter, 60×40 mm
  S1     1×10 rotary     Band switch
  L1–L10 various         Band inductors (see schematic)
  C_band various         Band capacitors (see schematic)
  J1     BNC female      Probe input
  ENC    120×65×35       3D print ASA (enclosure_passive_fsm.scad)

4-1.2  PCB LAYOUT NOTES

  The detector circuit is simple enough for a 50×40 mm piece of
  single-sided FR-4 or even Manhattan-style construction on copper
  clad board. Key layout rules:

  a.  Keep detector diode D1 and load R1 directly at the input
      connector (J1) with leads < 10 mm. Long leads add inductance
      that degrades HF detection sensitivity.

  b.  Place C1 (100 pF RF bypass) directly across R1 leads to
      provide a low-impedance RF return path.

  c.  The meter M1 and C2 (integrating cap) are in the DC output
      path; lead length is not critical.

  d.  Route the band switch S1 with short connections from each
      position to its corresponding LC tuned circuit. Keep LC
      circuits near the switch, not near the detector.

4-1.3  DETECTOR DIODE SELECTION

  1N34A germanium point-contact diodes vary in quality. Before
  installation, check the forward voltage at 1 mA with a DMM:
    Good 1N34A:    150–250 mV forward voltage
    Silicon diode: 600–700 mV (do NOT use; too high for weak signals)
    BAT54 Schottky: 200–300 mV (acceptable substitute; better HF)
    OA47/OA90:     180–240 mV (acceptable European equivalents)

  The lower the forward voltage, the better the weak-signal sensitivity.

4-1.4  TOROID INDUCTOR WINDING — BAND FILTERS

  Wind toroid inductors on T-50-6 (green, for HF) or T-37-6
  (yellow/green, smaller) cores:

  Band  Target L    Core      Turns   Wire AWG
  ----  --------    ----      -----   --------
  160M  10 μH       T-50-6    31      #26
  80M    4.7 μH     T-50-6    21      #26
  40M    2.2 μH     T-50-6    15      #26
  30M    1.0 μH     T-37-6    10      #26
  20M    560 nH     T-37-6     7      #26
  15M    330 nH     T-37-6     5      #28
  10M    220 nH     T-37-6     4      #28
  6M     100 nH     T-25-6     3      #28
  2M     33 nH      air coil   4      #22 (8mm dia, 5mm long)
  70cm    8 nH      air coil   2      #22 (6mm dia, 4mm long)

  Measure inductance with LCR meter after winding; adjust turns
  ±1 to reach target value within ±10%.

4-1.5  ENCLOSURE ASSEMBLY

  Step 1:  3D print enclosure_passive_fsm.scad in PETG or ASA.
           Print body and lid separately. Deburr holes.

  Step 2:  Install M3 heat-set inserts (×4) at lid bolt holes.

  Step 3:  Mount BNC connector J1 on front face (12.5 mm hole).
           Torque: 0.6 N·m max.

  Step 4:  Mount rotary band switch S1 through lid hole (10.5 mm).
           Tighten hex nut, install knob.

  Step 5:  Mount sensitivity pot VR1 through lid hole (7.5 mm).
           Install knob.

  Step 6:  Mount panel meter M1 through meter aperture (62×42 mm).
           Secure with mounting screws from inside (M3, 4 corners if
           meter has tapped mounting holes) or with adhesive foam tape.
           Install acrylic glass window (1.5 mm, 64×44 mm, cut to fit).

  Step 7:  Install PCB on 4 mounting bosses with M2.5 screws.
           Connect BNC center pin to D1/C1 node.
           Connect band switch to PCB with 26 AWG hookup wire.

  Step 8:  Install lid; tighten 4× M3 screws.

--------------------------------------------------------------------------------

4-2. FSM-A CONSTRUCTION

4-2.1  PARTS REQUIRED

  See schematics/sch_active_fsm.txt for complete schematic.

  Ref    Value           Description
  ---    -----           -----------
  U1     ERA-3SM         MMIC preamp (SOT-89; Mini-Circuits)
  U2     AD8307AN        Log detector (DIP-8; Analog Devices)
  U3     LM358N          Dual op-amp (DIP-8)
  U4     NE555N          Timer (DIP-8; for audio VCO)
  U5     78L05           +5V LDO (TO-92; for AD8307 supply)
  D1     BAT54S          ESD protection (SOT-23 dual Schottky)
  R1     75 Ω 1/4W       ERA-3SM bias (9V supply: 75Ω)
  R2     51 Ω 1%         Output pad
  R3     51 Ω 1%         AD8307 input match
  R4     1 kΩ            AD8307 VOUT filter
  R5,R6  10 kΩ 1%        Op-amp gain network
  R7     15 kΩ           Meter series resistor
  R8,R9  10 kΩ, 100 kΩ   555 timing resistors
  VR1    10 kΩ trimmer   Meter calibration
  VR2    100 kΩ          Audio tone sensitivity
  C1–C8  various         Bypass, coupling (see schematic)
  M1     1 mA FSD        Panel meter
  SW_ATT 4-pos rotary    Input attenuator
  J1     SMA female      Probe input
  J2     3.5 mm mono     Audio output
  BT1    9V PP3          Battery
  SW_P   SPST rocker     Power switch
  ENC    160×100×48      3D print ASA (enclosure_active_fsm.scad)
  Att pads see table     Pi attenuator resistors (see sch_active_fsm.txt)

4-2.2  CRITICAL ASSEMBLY NOTES — ERA-3SM

  WARNING: ERA-3SM is an ESD-sensitive SOT-89 MMIC device.
  It is destroyed instantly by improper bias or reversed voltage.
  Before soldering, verify bias resistor R1 value (75Ω at 9V supply).
  Do not apply power without R1 installed.

  The ERA-3SM SOT-89 pinout:
    Pin 1 (tab, bottom): RF INPUT
    Pin 2: GND
    Pin 3: GND
    Pin 4: Vcc / RF OUTPUT (same pin; bias through R1)

  Solder with temperature-controlled iron (330°C max). Use minimum
  solder; the exposed collector/drain pad on pin 4 is the RF output.

4-2.3  AD8307 INSTALLATION

  The AD8307AN (DIP-8) is preferred for hand assembly. Use a DIP-8
  socket to allow replacement without thermal damage.

  Post-installation verification:
    1.  Apply 5V to AD8307 VS pin. Measure: 4.8–5.2V. Pass.
    2.  Apply known signal (e.g., −30 dBm at 10 MHz from signal generator).
        VOUT should read approximately:
        V = 0.025 × (−30 + 84) = 1.35V ± 0.15V.
    3.  If VOUT is out of range, check input matching resistor R3 (51Ω)
        and INLO bypass capacitor.

4-2.4  ATTENUATOR PAD CONSTRUCTION

  Pi attenuator resistors must be SMD (0402 or 0603) thin-film
  types rated ±1% for accurate calibration. Wirewound or carbon
  composition resistors are unsuitable.

  Use E96 series values (nearest standard):
    −10 dB: 95.3Ω shunt, 71.5Ω series
    −20 dB: 61.9Ω shunt, 249Ω series
    −30 dB: 53.6Ω shunt, 787Ω series

  Mount on a small piece of PCB with a 50Ω microstrip layout, or
  in a small metal box (brass or aluminum) for shielding.

--------------------------------------------------------------------------------

4-3. FSM-D CONSTRUCTION

4-3.1  PARTS REQUIRED (additions to FSM-A)

  Ref    Value           Description
  ---    -----           -----------
  U6     ESP32-2432S028  CYD board (AliExpress)
  U7     NEO-6M module   GPS module with antenna
  SD1    MicroSD module  SPI SD card adapter (3.3V level)
  U8     LM386N          Audio amplifier (DIP-8)
  R10,R11 10 kΩ          ADC protection resistors
  R12    330 kΩ          Battery divider high-side
  R13    56 kΩ           Battery divider low-side
  R14    1 kΩ            Audio RC filter
  C9     0.01 μF         Audio RC filter
  VR3    500 Ω           Volume control pot
  SP1    8Ω 0.25W        Miniature speaker (30 mm dia)
  SD_slot MicroSD holder  For FAT32 cards up to 32 GB

4-3.2  CYD FIRMWARE UPLOAD

  Step 1:  Install Arduino IDE 2.x. Install ESP32 board support
           package v3.x via Boards Manager.
  Step 2:  Install libraries: TFT_eSPI (Bodmer), XPT2046_Touchscreen.
  Step 3:  Configure TFT_eSPI/User_Setup.h:
             #define ILI9341_DRIVER
             #define TFT_CS 15, TFT_DC 2, TFT_RST 12
             #define TFT_MOSI 13, TFT_SCLK 14, TOUCH_CS 33
             #define SPI_FREQUENCY 40000000
  Step 4:  Copy field_strength_cyd.ino and fsm_datalogger.h to
           same project folder.
  Step 5:  Select board "ESP32 Dev Module", partition "Default 4MB".
  Step 6:  Upload. Verify boot message on serial monitor at 115200 baud.
  Step 7:  Verify all 6 screens accessible; verify touch response.
  Step 8:  Insert microSD card (FAT32 formatted). Verify SD:OK on display.
  Step 9:  Connect GPS module. Wait for fix (90–180s outdoors).
           Verify GPS coordinates appear on main screen.

4-3.3  WIRE ROUTING

  All ADC input wires (GPIO34, 35, 36) should be shielded or
  routed away from the TFT SPI lines (GPIO13, 14) to prevent
  clock pickup on the ADC inputs. Use twisted pairs or coax for
  the RF signal path from the attenuator/ERA-3SM to the AD8307.

================================================================================
CHAPTER 5  CALIBRATION PROCEDURES
================================================================================

5-1. CALIBRATION OVERVIEW

5-1.1  Three levels of calibration are defined:

  LEVEL 1 — FUNCTIONAL CHECK
    Verifies that the instrument powers on, deflects with RF present,
    and responds correctly to attenuator changes. No test equipment
    required beyond a working amateur transceiver.

  LEVEL 2 — dBm CALIBRATION (FSM-A and FSM-D)
    Establishes the dBm reading accuracy to ±2 dB. Requires a signal
    source of known output level (e.g., a 50Ω QRP transceiver measured
    with the TM-DL-001 power meter and directional coupler at a
    fixed attenuated output).

  LEVEL 3 — FIELD STRENGTH CALIBRATION (FSM-D only)
    Establishes the V/m and antenna factor accuracy. Requires a
    calibrated reference antenna and known transmit power (from
    the TM-DL-001 or TM-NB-001 power meter).

5-2. LEVEL 1 — FUNCTIONAL CHECK (ALL VARIANTS)

  Step 1:  With no probe connected, verify meter at zero (FSM-P, FSM-A)
           or dBm display shows < −85 dBm (FSM-D: noise floor).

  Step 2:  Connect a 50 cm whip to the input. Bring within 1 meter
           of a working VHF FM broadcast radio. Verify meter deflects
           (FSM-P), meter reads 1/4 or more FSD (FSM-A), or dBm
           display reads > −75 dBm (FSM-D).

  Step 3:  Increase attenuator setting one step. Verify reading drops.

  Step 4:  Test audio output (FSM-A / FSM-D): verify tone frequency
           increases as field strength increases.

  Step 5:  Test HOLD function (FSM-D): press HOLD, verify reading
           freezes. Press again to release.

  ACCEPT:  All steps show expected response.
  REJECT:  Any step shows no response → troubleshoot per Chapter 7.

5-3. LEVEL 2 — dBm CALIBRATION (FSM-A and FSM-D)

5-3.1  EQUIPMENT REQUIRED

  a.  RF signal source with known output power ±0.5 dB.
      Options:
      - QRP transmitter at low output with calibrated power meter
        (TM-DL-001 directional coupler): recommended
      - NanoVNA in transmit mode (requires calibration)
      - Commercial signal generator (HP8640B, etc.)

  b.  BNC 50Ω precision attenuator (20 dB, if not built in)
      to reduce transmitter output to FSM safe input range.

  c.  BNC-to-SMA adapter (as needed for FSM-D SMA input).

5-3.2  CALIBRATION PROCEDURE

  Step 1:  Set signal source to 14.175 MHz (20M band center),
           output level exactly −20 dBm (10 μW at 50Ω).

  Step 2:  Connect through FSM attenuator at −20 dB position.
           Effective input to ERA-3SM = −20 − 20 = −40 dBm.
           AD8307 input (after ERA-3SM) = −40 + 20 = −20 dBm.

  Step 3:  FSM-A: Adjust VR1 (meter calibration) until meter reads
           at the −20 dBm mark on the scale. If scale has no dBm
           markings, calculate expected deflection:
             VOUT = 0.025 × (−20 + 84) = 1.60V
             Meter current = 1.60V / (R7 + R_meter) mA
           Adjust R7 (15 kΩ) or VR1 to match.

  Step 4:  FSM-D: Navigate to Screen 5 (Settings). Check ADC offset
           and scale. On screen, the displayed dBm should read −40 dBm
           (the input to the ERA-3SM, after ERA-3SM gain and attenuation
           are factored in by the firmware).

  NOTE: The firmware computes:
    P_at_ADC = ad8307_raw_dBm (what AD8307 sees)
    P_at_input = P_at_ADC − ERA_gain = P_at_ADC − 20 dB
    P_displayed = P_at_input − atten_dB setting

  Step 5:  If FSM-D reads other than −40 dBm:
    Calculate correction:
      adc_offset = adc_offset + (V_measured − V_expected)
      where V_expected = 0.025 × (−20 + 84) = 1.60V
      and V_measured = (ADC_count × 3.3 / 4095)
    Or use the simpler formula:
      New offset = Old offset + (displayed_dBm − expected_dBm) × 0.025

  Step 6:  Repeat with −10 dBm input (expected FSM-D display = −30 dBm).
    Accept: both readings within ±2 dB of expected.
    Reject: if linearity is off (both points off in same direction:
    adjust offset; if they diverge: adjust scale factor).

  Step 7:  Press SAVE on Settings screen to store to EEPROM.

5-4. LEVEL 3 — FIELD STRENGTH CALIBRATION (FSM-D)

5-4.1  OVERVIEW

This procedure establishes the antenna factor (AF) for each probe
against a reference transmitter of known power and geometry.

5-4.2  REFERENCE SETUP

  a.  Set up a half-wave dipole at a known height (≥λ/4 above ground).
      Orient horizontally. Feed with a precision 50Ω coax feedline.

  b.  Connect a calibrated power meter (TM-DL-001 directional coupler
      + power meter) to the feedline. Note forward power P_fwd in watts.

  c.  Calculate expected field strength at distance d:
        E = √(30 × P_fwd × Gt) / d           [Equation 5-1]
        For half-wave dipole: Gt = 1.64 (2.15 dBi)
        E_dBuVm = 20 log10(E) + 120

5-4.3  MEASUREMENT

  Step 1:  Position FSM-D at distance d = 10 meters (measured)
           from the dipole's center, in the broadside direction.
           Align probe for maximum received signal.

  Step 2:  Note FSM-D displayed E_Vm reading.

  Step 3:  Calculate actual E_field from Equation 5-1.

  Step 4:  AF correction:
    Delta_AF = E_expected_dBuVm − E_displayed_dBuVm
    Enter this delta on the Settings screen as "AF User Correction".
    Press SAVE.

  Step 5:  Repeat for 2–3 bands. Note that the correction may vary
           by up to ±6 dB across bands. For best accuracy, store
           per-band corrections in a table (modify firmware band
           table as needed, or apply manually per band).

  Step 6:  Verify: return to reference position, re-check displayed
           E_Vm vs. calculated E_expected. Accept if within ±3 dB.

5-4.4  GPS TIMESTAMP VERIFICATION

  Step 1:  Verify GPS fix is active (GPS: FIX shown on Logger screen).
  Step 2:  Record a manual log entry while at a known position.
  Step 3:  Compare logged coordinates to GPS coordinates from a
           phone or dedicated GPS receiver.
  Accept: within 10 meters horizontal accuracy.

================================================================================
CHAPTER 6  OPERATING PROCEDURES
================================================================================

6-1. PREPARATIONS FOR USE

6-1.1  PRE-OPERATION CHECKLIST (ALL VARIANTS)

  [ ] Probe connected to input connector (correct probe for band)
  [ ] Battery installed and charged (FSM-A/D: verify voltage indicator)
  [ ] Attenuator set to maximum (−30 dB) for initial measurement
  [ ] Power switch ON
  [ ] Display/meter reads correctly (no alarm indication)
  [ ] Band selection correct for measurement frequency (FSM-D)
  [ ] GPS acquiring (FSM-D; allow 2–3 minutes outdoors)

6-1.2  PROBE SELECTION GUIDE

  Measurement type         Recommended probe
  ----------------         -----------------
  HF relative strength     50 cm telescoping whip
  HF calibrated (V/m)      λ/2 dipole (adjust to band)
  HF direction finding     12.7 cm shielded loop
  VHF/UHF relative         50 cm whip (shorter extension for 2M)
  VHF/UHF calibrated       Miniature Yagi or calibrated dipole
  VHF/UHF directional      3-element Yagi probe (forward lobe)
  Near-field / PCB scan     Small loop (10–30 mm dia)

--------------------------------------------------------------------------------

6-2. RELATIVE FIELD STRENGTH MEASUREMENTS

6-2.1  Relative measurements compare signal levels at different
positions or antenna orientations. Absolute accuracy is not required.

  Step 1:  Select appropriate probe. Connect to input.
  Step 2:  Set attenuator to −20 dB initially.
  Step 3:  Position instrument at measurement point.
  Step 4:  Note reading (FSM-P: meter position; FSM-A: meter dBm;
           FSM-D: dBm or S-units on main screen).
  Step 5:  Move to second position or change antenna orientation.
  Step 6:  Compare readings.

  NOTE: When comparing readings, the attenuator setting must remain
  constant between measurements. A change in attenuator setting
  introduces a known offset (10/20/30 dB) which must be accounted for.

6-2.2  ANTENNA PATTERN MEASUREMENT

To measure a transmitting antenna's horizontal pattern:
  a.  Set transmitter to known power, constant output.
  b.  Measure FSM reading at 8–16 azimuths around the antenna
      at a fixed radius (5–20 wavelengths from antenna).
  c.  Record readings (dBm or meter position) vs. azimuth angle.
  d.  Normalize to the maximum reading (0 dB).
  e.  Plot polar or rectangular pattern.

  NOTE: Ground reflections, nearby structures, and antenna
  mounting will all affect the measured pattern. This technique
  measures the installed radiation pattern, not the free-space
  pattern. Both are useful for practical antenna evaluation.

--------------------------------------------------------------------------------

6-3. CALIBRATED FIELD STRENGTH MEASUREMENTS (FSM-D)

6-3.1  For V/m field strength measurements, the following procedure
produces calibrated results ±3 dB (with Level 3 calibration performed):

  Step 1:  Select band matching measurement frequency (Screen 4).
  Step 2:  Select correct probe type in Settings (WHIP / DIPOLE / LOOP).
  Step 3:  Verify AF user correction is applied (Settings screen).
  Step 4:  Set attenuator to produce a reading in the upper half of the
           AD8307 range (aim for −50 to −10 dBm on display).
  Step 5:  Orient probe for maximum response (broadside for dipole,
           horizontal for whip).
  Step 6:  Read E_Vm and E_dBuVm from Screen 1 (Calibrated).
  Step 7:  Log the reading (Screen 3 LOG button or auto-log).

6-3.2  MULTIPLE POSITION SURVEY (DATA LOGGING)

  Step 1:  Ensure GPS fix is active.
  Step 2:  Set auto-log interval (Settings: 10s recommended).
  Step 3:  Walk survey route, holding FSM at consistent height (1–1.5 m).
  Step 4:  On completion, remove SD card. Import CSV into mapping
           software (QGIS, Google Earth with GPS Visualizer, or
           Python with folium library).
  Step 5:  Color-code measurement points by field strength level
           to create a coverage or interference map.

  TIP: Use the same frequency, attenuator, and probe throughout
  a survey session. Note attenuator setting in the Notes field of
  any manual log entries.

--------------------------------------------------------------------------------

6-4. DIRECTION FINDING PROCEDURE

6-4.1  HF DIRECTION FINDING WITH LOOP PROBE

  EQUIPMENT:
    FSM-D with shielded loop probe
    Compass (standalone or phone compass)

  Step 1:  Tune external receiver (or set FSM band) to the
           transmitter frequency.
  Step 2:  Navigate to Screen 2 (Direction Finder, press DIR on main screen).
  Step 3:  Press RESET to clear previous bearing data.
  Step 4:  Hold loop in vertical plane, horizontal grip.
  Step 5:  Slowly rotate loop. Watch dBm display. Find the orientation
           of MINIMUM signal (null).
  Step 6:  The null axis of the loop points toward (and away from)
           the transmitter.
  Step 7:  Note compass bearing of the loop axis at null. This is
           the bearing line to the transmitter (either direction).
  Step 8:  Move to a second position (≥100 m from first).
           Repeat steps 4–7.
  Step 9:  Plot both bearing lines on a map. Their intersection
           gives the transmitter location (resolves 180° ambiguity).

  NOTE: In urban environments with multiple reflections, nulls
  may be indistinct or multiple. Move to a more open location,
  or use 3 bearings to triangulate if possible.

6-4.2  VHF/UHF DIRECTION FINDING WITH YAGI PROBE

  The 3-element Yagi provides a directional forward lobe (~10 dBd)
  and a well-defined forward direction (maximum signal = transmitter
  direction). No ambiguity as with the loop.

  Step 1:  Mount Yagi probe on FSM-D input (SMA adapter).
  Step 2:  Navigate to Screen 2 (Direction Finder).
  Step 3:  Sweep horizontally to find MAXIMUM signal (peak, not null).
  Step 4:  Note bearing at peak.

  NOTE: The Yagi has significant sidelobe response. If the pattern
  seems broad or shows multiple peaks, try increasing attenuator
  to ensure AD8307 is not in saturation. A saturated AD8307 does
  not distinguish between different signal levels (all read near maximum).

================================================================================
CHAPTER 7  TROUBLESHOOTING
================================================================================

7-1. NO INDICATION (ALL VARIANTS)

  Check 1:  Power switch on? Verify battery voltage with DMM.
            FSM-A/D: ≥7V required.

  Check 2:  Is there a signal to detect? Confirm a transmitter is
            active on the measurement frequency.

  Check 3:  Correct probe connected? BNC/SMA connector seated?
            A BNC with only partial engagement does not make RF contact.

  Check 4:  Attenuator at maximum (−30 dB)? Try 0 dB setting.

  Check 5:  FSM-P only: verify diode D1 is germanium type and installed
            with correct polarity (cathode to load resistor R1, not GND).

  Check 6:  FSM-A/D: verify ERA-3SM bias voltage. Measure voltage
            at pin 4 of ERA-3SM: should be 4.0–5.0V. If zero or 9V
            (supply rail): bias resistor R1 is open or shorted.

7-2. READING TOO HIGH OR SATURATED

  Check 1:  Increase attenuator setting. If reading drops as
            expected, the ERA-3SM or AD8307 was in compression.
            Use more attenuation.

  Check 2:  FSM-D: if display shows approximately same value
            regardless of attenuator setting, AD8307 may be clipped
            at its maximum output. Increase attenuation by 20 dB.

  Check 3:  Check input for strong nearby transmitter (broadcast
            station, commercial RF source). Use band filter or
            move to a shielded location for lower-signal work.

7-3. ERRATIC OR NOISY READING (FSM-D)

  Check 1:  ADC noise: ensure ADC input (GPIO34) has 0.1 μF bypass
            to GND directly at the ESP32 pin. EMI from TFT SPI
            can couple into adjacent GPIO inputs.

  Check 2:  Ground loop: all shields, GND connections, and enclosure
            ground must be connected at a single star point. Multiple
            ground paths create ground loops that appear as noise.

  Check 3:  GPS module: ensure GPS UART2 lines (GPIO16/17) are
            routed away from ADC inputs. NMEA data stream at 9600 baud
            can alias into the ADC sampling.

  Check 4:  SD card clock (GPIO18) at 4 MHz: disable SD logging
            temporarily (Settings: Log interval = Manual). If
            noise reduces, SD SPI clock is coupling into ADC.
            Add ferrite bead on GPIO18 trace.

7-4. GPS NOT ACQUIRING

  Check 1:  Allow full 3 minutes for cold start. NEO-6M cold start
            is typically 1–3 minutes with clear sky.

  Check 2:  Verify GPS module receives +3.3V (or +5V per module spec).
            Some GPS modules with onboard LDO accept 3.3–5V.

  Check 3:  Confirm UART2 is configured: Serial2.begin(9600, SERIAL_8N1,
            16, 17). Verify no conflict with other GPIO users on 16/17.

  Check 4:  In firmware, verify GPS is polling: gps_poll() is called
            in main loop; check that Serial2.available() returns > 0.

  Check 5:  GPS antenna: module must have external active patch antenna
            with clear sky view. Most NEO-6M modules include an antenna;
            verify it is connected.

7-5. SD CARD NOT RECOGNIZED

  Check 1:  Card is FAT32 formatted, ≤32 GB. SDXC cards >32 GB
            require exFAT and different SD library; use ≤32 GB.

  Check 2:  SD module SPI connections: CS=5, SCK=18, MISO=19, MOSI=23.
            Verify no signal inversion or float on MISO (add pull-up
            if needed: 10 kΩ from MISO to 3.3V).

  Check 3:  Level translation: if SD module is 5V type, 5V on MOSI/SCK
            from module will damage ESP32 inputs. Use 3.3V-only SD module.

  Check 4:  Reduce SPI frequency in SD.begin() call from 4000000 to
            1000000 (1 MHz) for troubleshooting.

7-6. AUDIO TONE NOT WORKING

  Check 1:  Audio mode set to OFF? Check Settings screen.
  Check 2:  MUTE_PIN (GPIO26): verify HIGH (not muted).
  Check 3:  ledcSetup() channel 0 configured correctly in setup().
  Check 4:  Check RC filter (R=1kΩ, C=0.01μF) output with DMM AC.
            Should show AC voltage varying with tone.
  Check 5:  LM386 supply: verify 5V present at VS pin.
  Check 6:  Speaker: verify continuity; replace if open.

================================================================================
CHAPTER 8  MAINTENANCE
================================================================================

8-1. SCHEDULED MAINTENANCE

  Interval    Procedure
  --------    ---------
  Before use  Check battery voltage; verify probe connector is clean
  Weekly      Inspect probe connectors (BNC/SMA) for corrosion
  Monthly     Clean connector center pins with deoxit
              Verify SD card reads/writes (create test entry)
              Verify GPS acquires fix within 3 minutes
  6 months    Full Level 1 functional check
              Inspect detector diode (FSM-P): verify Vf < 300 mV
              Check all potentiometer wipers for smooth operation
  Annually    Level 2 dBm calibration check
              Battery replacement (alkaline, regardless of voltage)
              Inspect all PCB solder joints under magnification

8-2. BATTERY REPLACEMENT (FSM-A)

  Replace when battery indicator drops below 7.5V (FSM-A DMM reading).
  Replace both batteries in FSM-D (2× PP3 series) simultaneously.
  Do not mix alkaline and carbon-zinc types.

8-3. FIRMWARE UPDATE (FSM-D)

  Step 1:  Connect CYD via USB to computer running Arduino IDE.
  Step 2:  Open field_strength_cyd.ino and fsm_datalogger.h.
  Step 3:  Verify TFT_eSPI User_Setup.h matches CYD pinout.
  Step 4:  Upload. Verify boot on serial monitor at 115200 baud.
  Step 5:  Re-enter calibration constants (EEPROM may be reset
           if EEPROM address map changed between versions).

================================================================================
CHAPTER 9  PARTS LIST
================================================================================

See individual schematic files for complete BOMs:
  schematics/sch_passive_fsm.txt   (FSM-P parts)
  schematics/sch_active_fsm.txt    (FSM-A parts)
  schematics/sch_digital_cyd_fsm.txt (FSM-D additions)
  schematics/sch_probes.txt        (probe parts)

Supplier recommendations:
  Mouser Electronics:   mouser.com          (passive components, ICs)
  Digi-Key:             digikey.com         (all component types)
  Mini-Circuits:        minicircuits.com    (ERA-3SM, attenuator pads)
  Analog Devices:       analog.com          (AD8307)
  Amidon:               amidoncorp.com      (toroid cores)
  AliExpress:           aliexpress.com      (CYD board, GPS module)
  McMaster-Carr:        mcmaster.com        (hardware, enclosure materials)
  1N34A sources:        eBay, RF Parts, or Antique Radio Supply

================================================================================
APPENDIX A  ANTENNA FACTOR TABLES
================================================================================

A-1. THEORETICAL ANTENNA FACTOR (dB/m) — HALF-WAVE DIPOLE (free space, 50Ω)

  Band     f(MHz)   λ(m)     AF(dB/m)   he(m)   he/λ
  ----     ------   -----    --------   -----   ----
  160M     1.85     162.2    −30.6      51.6    0.318
  80M      3.75      80.0    −24.6      25.5    0.318
  40M      7.15      41.9    −18.5      13.3    0.318
  30M     10.125     29.6    −15.3       9.4    0.318
  20M     14.175     21.2    −12.5       6.7    0.318
  17M     18.118     16.6    −10.3       5.3    0.318
  15M     21.225     14.1     −9.0       4.5    0.318
  12M     24.940     12.0     −7.6       3.8    0.318
  10M     28.500     10.5     −6.4       3.3    0.318
  6M      50.000      6.0     −3.0       1.9    0.318
  2M     144.000      2.08    +7.2       0.66   0.318
  1.25M  222.000      1.35   +11.5       0.43   0.318
  70cm   432.000      0.694  +17.2       0.22   0.318
  33cm   902.000      0.332  +22.8       0.11   0.318
  20cm  1296.000      0.231  +26.2       0.074  0.318

  Formula: AF(dB/m) = 20 log10(4πf/c) = 20 log10(f_MHz) − 29.8

A-2. APPROXIMATE ANTENNA FACTOR — 50 cm TELESCOPING WHIP (estimated)

  Band     f(MHz)   AF(dB/m)   Notes
  ----     ------   --------   -----
  160M     1.85     +32        Very short monopole; poor efficiency
  80M      3.75     +26        Short; use dipole for calibrated work
  40M      7.15     +20        Usable for relative measurements
  20M     14.175    +14        Good general-purpose probe
  10M     28.500    +8         Near λ/4; good sensitivity
  6M      50.000    +6         ~λ/4 length at full extension
  2M     144.000    +3         Retract to ~48 cm; use for relative
  70cm   432.000    +1         Retract to ~16 cm; reasonable

A-3. APPROXIMATE ANTENNA FACTOR — 10 cm SHIELDED LOOP (estimated, near-field)

  These values are for near-field measurements only. The loop is
  an H-field probe; E/H ratio is not 377Ω in near field.

  Band     f(MHz)   AF(dB/m)   Useful range
  ----     ------   --------   ------------
  160M     1.85     +56        Very near field only
  80M      3.75     +50        Near transmitter (< λ/10)
  40M      7.15     +44        Useful to ~6m from source
  20M     14.175    +38        Useful to ~2m from source
  10M     28.500    +32        Useful to ~1m from source

================================================================================
APPENDIX B  S-METER REFERENCE TABLE
================================================================================

  S-Unit  dBm       dBuV(50Ω)  uV(50Ω)   Notes
  ------  -------   ---------  --------   -----
  S1      −121      −14.0      0.178      Barely audible
  S2      −115       −8.0      0.355
  S3      −109       −2.0      0.631
  S4      −103       +4.0      1.12
  S5       −97      +10.0      2.00
  S6       −91      +16.0      3.55
  S7       −85      +22.0      6.31       Weak but readable
  S8       −79      +28.0      11.2       Moderate
  S9       −73      +34.0      20.0       IARU S9 reference
  S9+10    −63      +44.0      63.2       Strong signal
  S9+20    −53      +54.0      200.0
  S9+30    −43      +64.0      632.0      Very strong
  S9+40    −33      +74.0      2000.0     Local station
  S9+60    −13      +94.0      20000.0    Adjacent
  S9+80     +7     +114.0     200000.0    Extreme proximity

================================================================================
APPENDIX C  BAND FREQUENCY REFERENCE
================================================================================

  Band     Allocation (USA)     Center      λ(m)    Notes
  ----     ----------------     ------      ----    -----
  160M     1.800–2.000 MHz      1.85 MHz    162.2   CW/SSB/digital
  80M      3.500–4.000 MHz      3.75 MHz     80.0   CW/SSB/digital
  40M      7.000–7.300 MHz      7.15 MHz     41.9   CW/SSB/digital
  30M     10.100–10.150 MHz    10.125 MHz    29.6   CW/digital (no phone)
  20M     14.000–14.350 MHz    14.175 MHz    21.2   CW/SSB/digital
  17M     18.068–18.168 MHz    18.118 MHz    16.6   CW/SSB/digital
  15M     21.000–21.450 MHz    21.225 MHz    14.1   CW/SSB/digital
  12M     24.890–24.990 MHz    24.940 MHz    12.0   CW/SSB/digital
  10M     28.000–29.700 MHz    28.500 MHz    10.5   CW/SSB/FM/digital
  6M      50.000–54.000 MHz    50.000 MHz     6.0   CW/SSB/FM
  2M     144.000–148.000 MHz  144.000 MHz     2.08  CW/SSB/FM/digital
  1.25M  222.000–225.000 MHz  222.000 MHz     1.35  FM/digital (limited)
  70cm   420.000–450.000 MHz  432.000 MHz     0.694 CW/SSB/FM/digital
  33cm   902.000–928.000 MHz  915.000 MHz     0.327 Digital/experimental
  20cm  1240.000–1300.000 MHz 1270.000 MHz    0.236 CW/SSB/FM/digital

================================================================================
APPENDIX D  FIELD STRENGTH CONVERSION TABLES
================================================================================

D-1. FIELD STRENGTH vs. dBm (at 50Ω, various probes)

  E (V/m)     dBμV/m    dBm(50Ω dipole 20M)    Notes
  -------     ------    -------------------    -----
  0.001        0.0      −119                   1 μV/m (S2 region HF)
  0.003        9.5      −109.5                 S3
  0.010       20.0      −100                   S4–S5
  0.030       29.5       −90.5                 S6
  0.050       34.0       −86                   S7
  0.100       40.0       −80                   S8
  0.300       49.5       −70.5                 S9+2dB
  1.000       60.0       −60                   S9+13dB
  3.000       69.5       −50.5                 S9+22dB
  10.00       80.0       −40                   S9+33dB
  30.00       89.5       −30.5                 Local transmitter at distance
  100.0      100.0       −20                   Very close to 100W antenna

  NOTE: dBm values assume λ/2 dipole at 14.175 MHz (AF = −12.5 dB/m)
  and include free-space conditions. Actual measurements will vary.

D-2. POWER DENSITY CONVERSION

  E (V/m)    S (W/m²)     S (mW/cm²)   Notes
  -------    --------     ----------   -----
  0.001       2.65×10⁻⁹   2.65×10⁻¹⁰  Negligible
  0.1         2.65×10⁻⁵   2.65×10⁻⁶
  1.0         2.65×10⁻³   2.65×10⁻⁴
  10.0        0.265        0.0265       Below IEEE C95.1 limits
  61.4        10.0         1.0          IEEE C95.1 limit (300 MHz–1.5 GHz)
  100.0       26.5         2.65         ABOVE RF safety limit

================================================================================
APPENDIX E  DATA LOGGING CSV FORMAT AND ANALYSIS
================================================================================

E-1. LOG FILE FORMAT

Column headers:
  UTC_Time, Date, Lat_deg, Lon_deg, Alt_m, Sats,
  Band, Freq_MHz, Atten_dB, dBm, V_rms_uV, E_Vm, S_units, Notes

Example line:
  14:23:45,2026-04-25,37.30234,-120.48122,52.3,8,40M,7.150,0,-45.2,3.89,0.107,4.7,auto

E-2. PYTHON ANALYSIS SCRIPT (basic)

  import pandas as pd
  import folium

  df = pd.read_csv('FSM_20260425.CSV')
  df['E_dBuVm'] = 20 * np.log10(df['E_Vm'] * 1e6)

  # Create coverage map
  m = folium.Map(location=[df['Lat_deg'].mean(), df['Lon_deg'].mean()], zoom_start=14)
  for _, row in df.iterrows():
      color = 'red' if row['dBm'] > -60 else 'orange' if row['dBm'] > -80 else 'green'
      folium.CircleMarker(
          location=[row['Lat_deg'], row['Lon_deg']],
          radius=5, color=color, fill=True,
          popup=f"{row['dBm']:.1f} dBm, {row['E_Vm']:.4f} V/m @ {row['UTC_Time']}"
      ).add_to(m)
  m.save('coverage_map.html')
  print(df[['Band','dBm','E_Vm','S_units']].describe())

================================================================================
APPENDIX F  GLOSSARY
================================================================================

AF      Antenna Factor. Ratio of incident E-field to antenna terminal
        voltage. AF (dB/m) = E_dBμV/m − V_dBμV at antenna terminals.

dBm     Decibels relative to 1 milliwatt, at 50Ω system impedance.

dBμV/m  Decibels relative to 1 microvolt per meter; field strength unit.

ERA-3SM Monolithic Microwave Integrated Circuit preamplifier, 20 dB gain,
        0.1–3000 MHz, made by Mini-Circuits. SOT-89 package.

Field Strength  The magnitude of the electric (or magnetic) field at a point
        in space, typically expressed in V/m or dBμV/m.

Friis   Friis Transmission Equation. Relates received power to transmit
        power, antenna gains, distance, and wavelength.

FSM     Field Strength Meter. Generic term for this instrument set.

IARU    International Amateur Radio Union. Sets standards including
        the S-meter calibration reference levels.

MDS     Minimum Detectable Signal. The weakest signal a receiver or
        detector can distinguish from the noise floor.

NMEA 0183  National Marine Electronics Association serial data standard.
        Used by GPS modules to output position and time data.

S-meter  Signal-strength meter. Calibrated to IARU standard:
        S9 = −73 dBm HF, S9 = −93 dBm VHF; 6 dB per S-unit.

V/m     Volts per meter. The SI unit of electric field strength.

================================================================================
END OF TECHNICAL MANUAL TM-FSM-001 Rev A
RF FIELD STRENGTH METER — ANTENNA TESTING AND RF DETECTION
================================================================================

DISTRIBUTION: Retain with instrument. For corrections, contact originating
technical authority.

================================================================================
