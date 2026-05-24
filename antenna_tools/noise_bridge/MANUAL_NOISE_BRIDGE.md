================================================================================
TECHNICAL MANUAL
RF NOISE BRIDGE — ANTENNA IMPEDANCE ANALYZER
TM-NB-001 Rev A

ANTENNA IMPEDANCE ANALYZER, WHEATSTONE BRIDGE TYPE
1–60 MHz, CALIBRATED R/X READOUT WITH SMITH CHART DISPLAY

NSN: XXXX-XX-XXX-XXXX (LOCAL FABRICATION)
================================================================================

THIS DOCUMENT CONTAINS TECHNICAL INFORMATION FOR THE CONSTRUCTION,
OPERATION, CALIBRATION, AND MAINTENANCE OF THE RF NOISE BRIDGE
ANTENNA IMPEDANCE ANALYZER.

PRIOR TO PERFORMING ANY PROCEDURE, READ ALL APPLICABLE WARNINGS,
CAUTIONS, AND NOTES. FAILURE TO COMPLY MAY RESULT IN INJURY OR
EQUIPMENT DAMAGE.

================================================================================
RECORD OF CHANGES
================================================================================

Rev    Date          Description                            By
----   ----------    ------------------------------------   ----
A      2026-04-24    Initial release                        ---

================================================================================
LIST OF EFFECTIVE PAGES
================================================================================

Chapter     Pages
-------     -----
Front       i - iv
1           1-1 through 1-8
2           2-1 through 2-6
3           3-1 through 3-14
4           4-1 through 4-20
5           5-1 through 5-12
6           6-1 through 6-10
7           7-1 through 7-8
8           8-1 through 8-6
9           9-1 through 9-4
A           A-1 through A-6
B           B-1 through B-4
C           C-1 through C-4
D           D-1 through D-2
E           E-1 through E-4

================================================================================
TABLE OF CONTENTS
================================================================================

CHAPTER 1  GENERAL INFORMATION ....................................... 1-1
  1.1      Purpose and Scope .......................................... 1-1
  1.2      Equipment Description ...................................... 1-1
  1.3      Principal Characteristics .................................. 1-2
  1.4      Equipment Identification ................................... 1-3
  1.5      Related Documents .......................................... 1-3
  1.6      Abbreviations and Symbols .................................. 1-4
  1.7      Safety Summary ............................................. 1-5

CHAPTER 2  THEORY OF OPERATION ....................................... 2-1
  2.1      Wheatstone Bridge Principle ................................ 2-1
  2.2      RF Noise Excitation ........................................ 2-2
  2.3      Bridge Balance Condition ................................... 2-3
  2.4      Null Detection Methods ..................................... 2-4
  2.5      Reactance Measurement ...................................... 2-5
  2.6      Smith Chart Theory ......................................... 2-6

CHAPTER 3  EQUIPMENT DESCRIPTION ..................................... 3-1
  3.1      Noise Generator Assembly ................................... 3-1
  3.2      Bridge Circuit Assembly .................................... 3-3
  3.3      Null Detector Assembly ..................................... 3-7
  3.4      Digital Readout Assembly (CYD) ............................. 3-9
  3.5      Power Supply ............................................... 3-12
  3.6      Enclosure and Mechanical ................................... 3-13

CHAPTER 4  CONSTRUCTION .............................................. 4-1
  4.1      Materials and Parts List ................................... 4-1
  4.2      Printed Circuit Board Fabrication .......................... 4-6
  4.3      Transformer Winding (T1) ................................... 4-7
  4.4      Noise Generator Assembly ................................... 4-9
  4.5      Bridge Circuit Assembly .................................... 4-10
  4.6      Null Detector Assembly ..................................... 4-12
  4.7      Digital Readout Assembly ................................... 4-14
  4.8      Enclosure Preparation ...................................... 4-16
  4.9      Final Integration .......................................... 4-18

CHAPTER 5  CALIBRATION PROCEDURES ................................... 5-1
  5.1      Calibration Overview ....................................... 5-1
  5.2      Test Equipment Required .................................... 5-2
  5.3      Calibration Standards Preparation .......................... 5-3
  5.4      Resistance Calibration (R-axis) ............................ 5-4
  5.5      Reactance Calibration (X-axis) ............................. 5-7
  5.6      Null Depth Verification .................................... 5-10
  5.7      CYD Digital Readout Calibration ............................ 5-11

CHAPTER 6  OPERATING PROCEDURES ..................................... 6-1
  6.1      Preparation for Use ........................................ 6-1
  6.2      Bridge Balance Procedure (Manual) .......................... 6-2
  6.3      Bridge Balance Procedure (CYD Assisted) .................... 6-5
  6.4      Smith Chart Plotting (Manual) .............................. 6-7
  6.5      Smith Chart Plotting (CYD Automatic) ....................... 6-8
  6.6      Interpreting Results ....................................... 6-9

CHAPTER 7  TROUBLESHOOTING ........................................... 7-1
  7.1      Troubleshooting Overview ................................... 7-1
  7.2      No Null Observed ........................................... 7-2
  7.3      Noisy or Unstable Null ..................................... 7-3
  7.4      R Reading Out of Range ..................................... 7-4
  7.5      X Reading Out of Range ..................................... 7-5
  7.6      CYD Display Problems ....................................... 7-6
  7.7      Battery/Power Problems ..................................... 7-7

CHAPTER 8  MAINTENANCE ............................................... 8-1
  8.1      Scheduled Maintenance ...................................... 8-1
  8.2      Potentiometer Service ...................................... 8-2
  8.3      Connector Inspection ....................................... 8-3
  8.4      Battery Replacement ........................................ 8-4
  8.5      Firmware Update ............................................ 8-5

CHAPTER 9  PARTS LIST ................................................ 9-1
  9.1      Complete Parts List ........................................ 9-1

APPENDIX A  REACTANCE TABLES ........................................ A-1
  A.1      Inductive Reactance (XL) vs. Frequency .................... A-1
  A.2      Capacitive Reactance (XC) vs. Frequency ................... A-3

APPENDIX B  BAND CALIBRATION DATA ................................... B-1
  B.1      Band Center Frequencies and Expected Loads ................. B-1
  B.2      Calibration Reference Values ............................... B-3

APPENDIX C  SMITH CHART REFERENCE ................................... C-1
  C.1      Reading the Smith Chart .................................... C-1
  C.2      Common Antenna Signatures on Smith Chart ................... C-3

APPENDIX D  CONVERSION FORMULAS ..................................... D-1

APPENDIX E  GLOSSARY ................................................. E-1

================================================================================
CHAPTER 1  GENERAL INFORMATION
================================================================================

1-1. PURPOSE AND SCOPE

1-1.1  This manual provides complete technical information for the
construction, operation, calibration, and maintenance of the RF Noise
Bridge Antenna Impedance Analyzer (hereinafter referred to as the
"noise bridge" or "instrument").

1-1.2  The noise bridge is a field-portable instrument for measuring
complex antenna impedance (R + jX) at radio frequencies from 1 to
60 MHz. It is intended for use in conjunction with an external receiver
or its built-in audio null detector to balance a Wheatstone bridge and
thereby determine the resistance and reactance components of an unknown
antenna or RF load.

1-1.3  This manual covers the following variants:
  a.  Basic noise bridge with analog readout dials only.
  b.  Noise bridge with CYD (ESP32-2432S028) digital readout,
      displaying R, X, |Z|, SWR, return loss, L/C equivalent, and
      Smith chart.

1-1.4  Users of this manual should have basic familiarity with RF
circuits, impedance measurements, and antenna theory.

--------------------------------------------------------------------------------

1-2. EQUIPMENT DESCRIPTION

1-2.1  The noise bridge consists of four functional assemblies:

  a.  NOISE GENERATOR — Produces wideband white noise from 1 to
      60 MHz minimum, using either a reverse-biased Zener diode or
      transistor noise source. Output is amplified by a low-noise
      MMIC amplifier to approximately −25 dBm broadband power into
      the bridge.

  b.  BRIDGE CIRCUIT — Wheatstone bridge topology with two fixed 51Ω
      arms and two variable arms: R_cal (0–200Ω 10-turn cermet
      potentiometer) and X_cal (variable capacitor and switched
      inductor network, ±200Ω reactance range at HF). A trifilar-
      wound toroid transformer (T1) provides impedance matching
      between the three bridge ports.

  c.  NULL DETECTOR — Detects bridge balance. Three modes:
      (1) External receiver via BNC connector for maximum sensitivity.
      (2) Built-in LM386 audio amplifier with headphone output.
      (3) AD8307 logarithmic detector for CYD display null bargraph.

  d.  DIGITAL READOUT (CYD) — ESP32-based module with 2.8-inch
      ILI9341 TFT display. Reads R and X potentiometer wipers via
      ADC, computes derived quantities (|Z|, θ, SWR, return loss,
      L/C equivalent), displays Smith chart with plotted impedance
      point and trail history.

1-2.2  All assemblies are housed in a weatherproof ASA enclosure
(180 × 120 × 55 mm) with O-ring lid seal, battery compartment (2× 9V
in series for 18V supply), SO-239 antenna connector, BNC noise input
and detector output connectors, and 3.5 mm headphone jack.

--------------------------------------------------------------------------------

1-3. PRINCIPAL CHARACTERISTICS

  Parameter                  Specification
  ---------                  -------------
  Frequency range            1–60 MHz (usable to 6M/50 MHz)
  Resistance range (R)       0–200 Ω
  Reactance range (X)        ±200 Ω (inductive or capacitive)
  Impedance reference (Z0)   50 Ω
  Noise generator output     −24 to −25 dBm (1–30 MHz broadband)
  Null depth (min)           >20 dB below noise floor
  Null depth (typical)       >30 dB
  R resolution (dial)        ~2 Ω (1/100 of range per major division)
  X resolution (dial)        ~4 Ω (1/50 of range per major division)
  CYD R resolution           0.2 Ω (12-bit ADC, 200Ω range)
  CYD X resolution           0.4 Ω (12-bit ADC, 200Ω full range)
  SWR display range          1.0:1 – 99.9:1
  Return loss display range  0–40 dB
  Band coverage              160M, 80M, 40M, 30M, 20M, 17M, 15M,
                             12M, 10M, 6M (touch-selectable on CYD)
  Battery                    2× 9V PP3 alkaline, series = 18V
  Battery life (typ.)        4–6 hours at full operation
  Operating temperature      −10°C to +50°C
  Storage temperature        −20°C to +60°C
  Dimensions                 180 × 120 × 55 mm (excl. connectors)
  Weight                     approximately 0.7 kg assembled
  Enclosure rating           IP53 (O-ring lid, sealed connectors)

--------------------------------------------------------------------------------

1-4. EQUIPMENT IDENTIFICATION

  Document Number:   TM-NB-001 Rev A
  Equipment Name:    RF Noise Bridge Antenna Impedance Analyzer
  Model Number:      TM-NB-001
  PCB Assembly:      NB-PCB-001 (main bridge board)
  PCB Assembly:      NB-PCB-002 (digital readout / CYD carrier)
  OpenSCAD Files:    enclosure_noise_bridge.scad
                     front_panel.scad
  Firmware:          noise_bridge_cyd.ino (ESP32)
  Smith Chart Lib:   smith_chart.h (C++ header, TFT_eSPI)

--------------------------------------------------------------------------------

1-5. RELATED DOCUMENTS

  TM-ANT-TRAP-001   Coaxial Cable Antenna Traps — Technical Manual
  TM-DL-001         RF Dummy Loads — Technical Manual
  ILI9341 DS        ILI9341 LCD Driver Datasheet (Ilitek)
  ESP32 TRM         ESP32 Technical Reference Manual (Espressif)
  TFT_eSPI          TFT_eSPI Library Documentation (Bodmer)
  AD8307 DS         AD8307 Log Amplifier Datasheet (Analog Devices)
  LM386 DS          LM386 Audio Amplifier Datasheet (Texas Instruments)
  BN-43-202 DS      BN-43-202 Binocular Core Specifications (Amidon)
  MAR-6SM DS        MAR-6SM MMIC Amplifier Datasheet (Mini-Circuits)
  ERA-3SM DS        ERA-3SM MMIC Amplifier Datasheet (Mini-Circuits)
  ARRL HB           ARRL Antenna Handbook (current edition)
  Laport            "Radio Antenna Engineering," E.A. Laport, 1952

--------------------------------------------------------------------------------

1-6. ABBREVIATIONS AND SYMBOLS

  ADC     Analog-to-Digital Converter
  ANT     Antenna
  BNC     Bayonet Neill-Concelman (RF connector, 50Ω)
  CYD     "Cheap Yellow Display" (ESP32-2432S028 dev board)
  DAC     Digital-to-Analog Converter
  dB      Decibels
  dBm     Decibels relative to 1 milliwatt
  DPDT    Double-Pole Double-Throw (switch)
  EEPROM  Electrically Erasable Programmable Read-Only Memory
  ESP32   Espressif ESP32 dual-core 32-bit microcontroller
  GDO     Grid Dip Oscillator (principle)
  GPIO    General Purpose Input/Output
  HF      High Frequency (3–30 MHz)
  I2C     Inter-Integrated Circuit (serial bus)
  ILI9341 Ilitek 2.8-inch TFT LCD controller
  L       Inductance (henries)
  LC      Inductor-Capacitor (reactive circuit)
  MMIC    Monolithic Microwave Integrated Circuit
  NTC     Negative Temperature Coefficient (thermistor)
  PCB     Printed Circuit Board
  pF      Picofarads
  Q       Quality factor (dimensionless)
  R       Resistance (ohms)
  RF      Radio Frequency
  RL      Return Loss (dB)
  RMS     Root Mean Square
  SMD     Surface Mount Device
  SO-239  UHF female coax connector (chassis mount)
  SPI     Serial Peripheral Interface
  SWR     Standing Wave Ratio
  TFT     Thin-Film Transistor (display type)
  VHF     Very High Frequency (30–300 MHz)
  X       Reactance (ohms, signed: +XC capacitive, −XL inductive)
  Z       Complex impedance (R + jX), ohms
  Z0      Characteristic impedance (50Ω reference)
  |Z|     Magnitude of impedance, √(R²+X²)
  θ       Impedance phase angle, arctan(X/R)
  Γ       Reflection coefficient (Gamma), complex
  μH      Microhenries
  Ω       Ohms

1-6.1  SIGN CONVENTION

NOTE: This instrument follows the electrical engineering convention
where inductive reactance is positive (+XL) and capacitive reactance
is negative (−XC). This is opposite to some older ham radio texts.
The Smith chart display follows the standard convention: inductive
above the real axis, capacitive below. Pot markings and CYD display
are consistent with this convention.

--------------------------------------------------------------------------------

1-7. SAFETY SUMMARY

WARNING

THE FOLLOWING WARNINGS APPLY THROUGHOUT THIS MANUAL. FAILURE TO
OBSERVE THESE WARNINGS MAY RESULT IN PERSONAL INJURY OR DEATH.

1.  RF RADIATION HAZARD — The noise bridge produces broadband RF
    noise that is transmitted from the antenna under test. Ensure
    that normal RF safety precautions are observed when connecting
    antennas.

2.  VOLTAGE HAZARD — Although the noise bridge operates from
    18V DC (2× 9V batteries), improper connection of a powered
    transmitter to the ANT port can introduce lethal RF voltages.
    NEVER connect a transmitting antenna to the noise bridge while
    a transmitter is operating on that antenna. ALWAYS disconnect
    the transmitter before making noise bridge measurements.

3.  BATTERY POLARITY — Reversed battery polarity will damage
    the MMIC amplifiers and ESP32. Double-check polarity before
    installing batteries.

CAUTION

1.  POWER LEVEL — The ANT port of this instrument is not designed
    to handle transmitter power. Maximum safe input is 0 dBm
    (1 mW). Connection of any transmitter output directly to the
    ANT port WILL destroy the bridge components.

2.  STATIC DISCHARGE — The MMIC devices (MAR-6SM, ERA-3SM) and
    ESP32 are static-sensitive. Use proper ESD precautions during
    assembly and service.

3.  FREQUENCY LIMIT — Measurement accuracy degrades above 30 MHz
    due to transformer and potentiometer parasitic effects. For
    50 MHz (6M band) measurements, use calibration data from
    Appendix B to correct for parasitic errors.

4.  EXTERNAL RECEIVER — When using an external receiver as the
    null detector, ensure the receiver is NOT connected to a
    transmitting antenna at the same time. Use only a dummy load
    or attenuated antenna connection.

NOTE

1.  The noise bridge measures impedance ONLY at the bridge
    terminals. Cable losses and impedance transformation by
    feedline are NOT accounted for. Measurements reflect the
    impedance at the instrument end of the feedline unless
    calibration corrections are applied.

2.  Measurements made with the CYD digital readout are based on
    potentiometer wiper positions. The display reads the dial
    settings, NOT an independent RF measurement. The null
    procedure must always be performed manually by the operator.

================================================================================
CHAPTER 2  THEORY OF OPERATION
================================================================================

2-1. WHEATSTONE BRIDGE PRINCIPLE

2-1.1  The RF noise bridge is an adaptation of the classic Wheatstone
bridge circuit to radio frequency operation. The Wheatstone bridge was
originally developed for DC resistance measurement and has been
extended to RF impedance measurement by replacing the DC galvanometer
with an RF null detector.

2-1.2  The general Wheatstone bridge consists of four impedance arms
arranged in a diamond (rhombus) configuration:

               Z1          Z2
    VIN  ------+------+------+------
               |      |      |
               Z3      G      Z4
               |      |      |
    GND  ------+------+------+------
               (UNKNOWN)   (REFERENCE)

When the bridge is balanced:

    Z1/Z2 = Z3/Z4    [Equation 2-1]

At balance, no current flows through the detector G, and the voltage
across G is zero (the null condition).

2-1.3  For this noise bridge, the bridge is configured as follows:

   Port A (NOISE IN) ───┬──── 51Ω (R_A) ────┬──── Port D (ANT)
                        │                    │
                      T1 winding           T1 winding
                        │                    │
                        └──── 51Ω (R_B) ────┘
                                  │
                             R_cal + jX_cal
                             (Variable Reference Arm)
                                  │
                               DETECTOR

In this configuration:
  - Two fixed 51Ω resistors form the stable reference arms
  - R_cal (0–200Ω) sets the resistance balance
  - X_cal (±200Ω) sets the reactance balance
  - The unknown antenna impedance hangs at Port D (ANT)
  - Balance (null) occurs when Z_unknown = R_cal + jX_cal

2-1.4  AT BALANCE:

    Z_antenna = R_cal + j(X_cal)    [Equation 2-2]

Where:
  Z_antenna = complex impedance of the antenna (Ω)
  R_cal     = dial reading of resistance potentiometer (Ω)
  X_cal     = dial reading of reactance network (Ω, signed)

This is the fundamental measurement equation of the noise bridge.

--------------------------------------------------------------------------------

2-2. RF NOISE EXCITATION

2-2.1  The bridge is excited with broadband white noise rather than
a swept sine wave. This approach has several advantages:

  a.  Single measurement covers all frequencies simultaneously.
      The user can tune the external receiver to any frequency within
      the noise band and observe the null at that frequency.

  b.  No signal generator required. The noise source is internal,
      battery-powered, and broadband.

  c.  The technique dates to the 1940s and is well proven for
      antenna work.

2-2.2  The noise source produces noise power from approximately
1 MHz to beyond 60 MHz. The spectral density is nominally flat
(white noise characteristic) within ±3 dB across the useful band.
Two noise source designs are provided:

  NGZ — ZENER DIODE NOISE SOURCE

    The 1N4733A zener diode (5.1V, 1W) is reverse-biased slightly
    above its breakdown voltage. The resulting avalanche noise current
    contains broadband noise components from audio to VHF. The noise
    is coupled through a DC-blocking capacitor to the MAR-6SM MMIC
    amplifier, which provides approximately 20 dB of gain and
    establishes the 50Ω output impedance.

    Advantage:  Simple, reliable, stable output level.
    Disadvantage:  Output level varies slightly with temperature
                   and battery voltage.

  NGT — TRANSISTOR NOISE SOURCE

    The 2N3904 NPN transistor is operated with its base-emitter
    junction reverse-biased at approximately −9V (the full battery
    voltage). The reverse-biased B-E junction produces noise by
    avalanche and shot noise mechanisms. Output coupling and
    amplification are identical to the NGZ design.

    Advantage:  Slightly higher noise output at lower frequencies.
    Disadvantage:  Requires negative supply rail.

2-2.3  Either noise source provides adequate excitation for bridge
null detection with an external receiver having >−80 dBm sensitivity.
For use with the built-in LM386 audio detector, the NGZ design is
recommended for its lower-noise output characteristics.

--------------------------------------------------------------------------------

2-3. BRIDGE BALANCE CONDITION

2-3.1  The bridge reaches null (minimum detector reading) when the
three conditions of Equation 2-2 are simultaneously satisfied:

  a.  R_cal = Re(Z_antenna)      (resistance arm balanced)
  b.  X_cal = Im(Z_antenna)      (reactance arm balanced)
  c.  The transformer T1 maintains impedance isolation between ports

2-3.2  The null is a sharp minimum, not a gentle slope. A properly
constructed bridge with a sensitive null detector will show a null
depth of 20–35 dB below the noise floor. The operator adjusts R_cal
and X_cal alternately, each pass bringing the null deeper, until the
minimum is found.

2-3.3  The balance procedure is inherently iterative because R_cal
and X_cal interact: changing one slightly shifts the optimal setting
for the other. Typically 3–4 iterations of the R/X/R/X cycle are
required to reach the deepest null. See Chapter 6 for the complete
balance procedure.

2-3.4  A perfect null cannot be achieved because:
  a.  The bridge arms have parasitic inductance and capacitance.
  b.  Transformer T1 has finite isolation.
  c.  Potentiometer and switch contacts add small impedance errors.
  d.  Stray coupling between bridge nodes via chassis capacitance.

These residual errors are minimized by careful PCB layout and are
characterized by the calibration procedure in Chapter 5.

--------------------------------------------------------------------------------

2-4. NULL DETECTION METHODS

2-4.1  EXTERNAL RECEIVER (Recommended)

Using an external shortwave or communications receiver provides the
highest sensitivity and allows the operator to choose the specific
frequency of measurement. Connect the receiver to the DET/RX BNC
connector. Tune the receiver to the desired frequency. Operate the
receiver in AM mode (BFO ON, but CW or SSB mode is preferred for
sharper null detection). Advance the RF gain for maximum sensitivity.
The broadband noise source appears as a continuous hiss; the null
appears as a sharp dip in the hiss.

NOTE: SSB or CW mode provides sharper null detection than AM mode
because the IF bandwidth is narrower, reducing background noise power
and making the null more distinct.

2-4.2  BUILT-IN AUDIO DETECTOR

The built-in LM386 audio amplifier detects the broadband noise
envelope after passing through an ERA-3SM preamplifier and BAT54A
envelope detector. The audio output appears at the PHONES 3.5 mm
jack. This mode is less frequency-selective than an external receiver
but requires no additional equipment. The null appears as a reduction
in hiss level at the headphone output.

NOTE: The built-in detector responds to the entire noise bandwidth
(1–60 MHz), not to a specific frequency. This means the null is
shallower than with a receiver-based detector because out-of-band
noise continues even when the bridge is nulled at the frequency
of interest. For best results with the built-in detector, use
the AD8307 CYD bargraph in conjunction with headphone monitoring.

2-4.3  AD8307 LOGARITHMIC DETECTOR (CYD Display)

The AD8307 logarithmic amplifier covers −75 to +17 dBm with a slope
of 25 mV/dB. Its output voltage is read by ESP32 ADC GPIO34 and
displayed as a bargraph on the CYD screen. This provides a visual
null indicator that is more accurate than headphone monitoring for
fine null adjustments. The AD8307 output is calibrated in the CYD
firmware to display approximately −20 to −55 dBm null detector
reading, depending on noise source output level.

--------------------------------------------------------------------------------

2-5. REACTANCE MEASUREMENT

2-5.1  The reactance arm (X_cal) consists of a switchable capacitor/
inductor network providing a calibrated ±200Ω range of reactance.

2-5.2  CAPACITIVE REACTANCE (XC, positive in this instrument's
convention):

    XC = 1 / (2π × f × C)         [Equation 2-3]

Where:
  f = frequency in Hz
  C = capacitance in farads
  XC = capacitive reactance in Ω (positive value)

The variable capacitor (12–230 pF, Jackson Bros. or equivalent air
variable) in parallel with switched capacitor banks (25, 50, 100, 200
pF) provides a total capacitance range of approximately 12 to 605 pF.

At 7.15 MHz (40M band center), capacitance range gives:
  12 pF:  XC = 1857 Ω (exceeds 200Ω range, use large capacitance)
  200 pF: XC = 111 Ω
  605 pF: XC = 37 Ω

Since the instrument X range is ±200Ω, the operator selects the
capacitor bank that brings the reactance into the 0–200Ω range before
fine adjustment with the variable capacitor.

2-5.3  INDUCTIVE REACTANCE (XL, negative in this instrument's
convention):

    XL = 2π × f × L               [Equation 2-4]

Where:
  f = frequency in Hz
  L = inductance in henries
  XL = inductive reactance in Ω (displayed as negative)

Switched inductor banks (22, 10, 4.7, 2.2 μH) in parallel provide
a total inductance range of approximately 1.5 to 22 μH. The DPDT
XL/XC direction switch connects either the capacitor network or the
inductor network into the X arm.

At 7.15 MHz (40M band center), inductance range gives:
  22 μH:  XL = 989 Ω (exceeds range)
  4.7 μH: XL = 211 Ω (near range limit)
  2.2 μH: XL = 99 Ω
  1.5 μH: XL = 67 Ω

2-5.4  REACTANCE SIGN CONVENTION

In this instrument, the front panel XL/XC switch is labeled:
  XC (capacitive):   X is POSITIVE, antenna is capacitive
                     (shorter than resonant length)
  XL (inductive):    X is NEGATIVE, antenna is inductive
                     (longer than resonant length)

The CYD display and Smith chart follow this convention. On the Smith
chart: XC (positive X) points are plotted BELOW the real axis.
XL (negative X) points are plotted ABOVE the real axis.

NOTE: This convention is consistent with standard Smith chart
orientation where inductive loads are in the upper hemisphere and
capacitive loads are in the lower hemisphere.

--------------------------------------------------------------------------------

2-6. SMITH CHART THEORY

2-6.1  The Smith chart is a graphical tool for displaying complex
impedance normalized to a reference impedance Z0 (50Ω for this
instrument). It maps the complex impedance plane to the unit disk
of the complex reflection coefficient plane, allowing visual
interpretation of antenna impedance and transmission line effects.

2-6.2  REFLECTION COEFFICIENT (GAMMA)

The reflection coefficient Γ at impedance Z with reference Z0 is:

    Γ = (Z − Z0) / (Z + Z0)        [Equation 2-5]

Where:
  Z  = R + jX (complex impedance of unknown, Ω)
  Z0 = 50 Ω (reference impedance)
  Γ  = complex reflection coefficient

In Cartesian form:
  Rn = R / Z0 (normalized resistance)
  Xn = X / Z0 (normalized reactance)
  D  = (Rn + 1)² + Xn²
  Re(Γ) = (Rn² + Xn² − 1) / D     [Equation 2-6]
  Im(Γ) = 2 × Xn / D               [Equation 2-7]

2-6.3  SMITH CHART FEATURES

The CYD Smith chart displays the following reference features:

  OUTER CIRCLE:
    The unit circle |Γ| = 1, representing purely reactive loads
    (R = 0). The left point (Γ = −1) is a SHORT CIRCUIT. The right
    point (Γ = +1) is an OPEN CIRCUIT.

  REAL AXIS:
    Horizontal line through chart center, representing purely
    resistive loads (X = 0). The center point (Γ = 0) represents
    the perfect match: Z = Z0 = 50Ω, SWR = 1.0.

  RESISTANCE CIRCLES (drawn in dark teal):
    Circles of constant normalized resistance r = R/Z0. Circles
    drawn for r = 0, 0.2, 0.5, 1.0, 2.0, 5.0.
    - r = 0:    Left edge (purely reactive)
    - r = 1:    Passes through center (50Ω circle)
    - r = 5:    Small circle near right (high resistance)

  REACTANCE ARCS (drawn in dark blue):
    Arcs of constant normalized reactance x = X/Z0. Arcs drawn
    for x = ±0.2, ±0.5, ±1.0, ±2.0, ±5.0.
    - Upper arcs: inductive (positive X)
    - Lower arcs: capacitive (negative X, positive in our XC convention)

2-6.4  SCREEN COORDINATE MAPPING

The CYD screen is 320 × 240 pixels in landscape mode. The Smith
chart is rendered with:
  Center:  (155, 145) pixels
  Radius:  110 pixels

Screen coordinate conversion:
  Screen_X = 155 + Re(Γ) × 110     [Equation 2-8]
  Screen_Y = 145 − Im(Γ) × 110     [Equation 2-9]

(Y is inverted because screen Y increases downward, but chart
Im(Γ) increases upward.)

2-6.5  SWR CIRCLES

SWR is related to |Γ| by:
    ρ = |Γ| = √(Re(Γ)² + Im(Γ)²)
    SWR = (1 + ρ) / (1 − ρ)        [Equation 2-10]

On the Smith chart, constant SWR appears as a circle centered on
Γ = 0 (chart center) with radius ρ. The CYD displays SWR circles
for SWR = 1.5, 2.0, and 3.0 as reference rings.

2-6.6  RETURN LOSS

Return loss (RL) in dB:
    RL = −20 × log10(ρ)            [Equation 2-11]

    SWR       ρ         RL (dB)
    -----     -----     -------
    1.0:1     0.000     ∞  (perfect match)
    1.1:1     0.048     26.4
    1.2:1     0.091     20.8
    1.5:1     0.200     14.0
    2.0:1     0.333     9.5
    3.0:1     0.500     6.0
    ∞:1       1.000     0.0 (total reflection)

================================================================================
CHAPTER 3  EQUIPMENT DESCRIPTION
================================================================================

3-1. NOISE GENERATOR ASSEMBLY

3-1.1  OVERVIEW

The noise generator (NG) produces the broadband RF noise signal that
excites the bridge. Two designs are provided (NGZ and NGT); either
may be installed. The output feeds the NOISE IN BNC connector (front
panel) and also connects directly to the bridge T1 noise port when
operating in self-contained mode.

3-1.2  NGZ — ZENER DIODE NOISE SOURCE

The circuit consists of the following functional blocks:

  BIAS NETWORK:
    R1 (4.7 kΩ)    Limits zener current to safe level
    D1 (1N4733A)   Zener diode operated slightly above 5.1V
                   breakdown, producing avalanche noise
    C1 (0.1 μF)    DC blocking / RF bypass to noise tap
    C2 (1000 pF)   RF coupling capacitor to MMIC input

  MMIC AMPLIFIER (U1 = MAR-6SM):
    MAR-6SM        Gain: 20 dB, NF: 2.7 dB, P1dB: +3 dBm
    R2 (130 Ω)     Bias resistor for 9V operation
                   I_bias = (9V − 5V) / 130Ω ≈ 31 mA (4V at device)
    C3 (1000 pF)   RF bypass at drain/collector
    RFC1 (100 nH)  RF choke at bias tap
    R3 (51 Ω)      Output pad to 50Ω (provides reverse match)
    C4 (1000 pF)   DC block at output

  OUTPUT:
    SMA or BNC connector to bridge NOISE IN
    Level: −24 to −25 dBm broadband (1–30 MHz)
    Impedance: 50Ω

3-1.3  SUPPLY VOLTAGE

  WARNING: The MAR-6SM operates from 5V bias voltage. At 18V
  supply (2× 9V batteries), a voltage regulator or a larger
  series bias resistor MUST be used. R2 value for 18V supply:
  R2 = (18V − 5V) / 31 mA = 419Ω → use 430Ω 1/4W.
  Calculate and verify bias voltage at U1 before applying power.

3-1.4  NGT — TRANSISTOR NOISE SOURCE

  Q1 (2N3904)     NPN transistor, reverse-biased B-E junction
  R4 (10 kΩ)      Base current limit
  C5 (1000 pF)    Output coupling to ERA-3SM amplifier
  U2 (ERA-3SM)    Gain: 20 dB, NF: 3.0 dB, P1dB: +12 dBm

  NOTE: The NGT noise source requires the full −9V rail (from the
  second 9V battery). For 9V single-supply operation, use NGZ only.

3-1.5  NOISE SOURCE ENABLE

GPIO 26 (DAC2) on the ESP32 controls the noise source ON/OFF via
an NPN switch transistor (2N3904). When GPIO26 is HIGH (DAC output
3.3V), the transistor is ON and noise source bias is applied.
When GPIO26 is LOW, the noise source is OFF. This allows the CYD
firmware to gate the noise source during display updates to reduce
RF interference with the display.

--------------------------------------------------------------------------------

3-2. BRIDGE CIRCUIT ASSEMBLY

3-2.1  BRIDGE TOPOLOGY

The bridge circuit is the heart of the instrument. It implements the
Wheatstone bridge adapted for RF impedance comparison.

ASCII SCHEMATIC:

                        +------------ NOISE IN (J3)
                        |
                    [T1-Primary]
                    /         \
         [R_A 51Ω]             [R_B 51Ω]
              |                     |
    [ANT Port J1]               [R_cal + X_cal]
              |                     |
              +-------[T1-Det]------+
                             |
                        DET OUT (J4)

T1 TRANSFORMER (BN-43-202 binocular core):
  Trifilar winding, 6 turns, three windings in parallel.
  Each winding: ~2–3 μH at HF.
  Core material: Type 43 ferrite (good from 1–50 MHz).
  Winding colors: Red (noise port), White (ANT port), Blue (DET port).

3-2.2  FIXED ARMS

R_A, R_B: 51Ω ±1%, 1/4W metal film (low noise, low TC)
  These are the two fixed arms of the bridge. They establish the
  bridge center point and must be matched to each other within
  ±0.5% for minimum inherent imbalance.

NOTE: Use 51.1Ω (E96 series) or hand-select matched pair from
51Ω ±5% resistors using a DMM or bridge. Matching within 0.1Ω
is preferred. Mismatch appears as a residual null offset that
cannot be removed by R_cal adjustment.

3-2.3  RESISTANCE CALIBRATION (R_cal)

  VR1: Bourns 3590S-2-201L, 200Ω, 10-turn cermet potentiometer
       (or equivalent: Vishay Spectrol 534, Beckman Industrial 72P)
  R5: 10Ω series resistor (sets R_cal minimum to 10Ω, not 0Ω)

  The 10Ω minimum is by design: R = 0Ω would place the bridge
  point directly at the ANT terminal, producing a degenerate balance
  condition. In practice, antenna impedances below 10Ω are rare.

  R_cal range: 10Ω (CW stop) to 210Ω (CCW stop)
  Displayed range: 0–200Ω (calibrated with R5 offset subtracted)

3-2.4  REACTANCE CALIBRATION (X_cal)

  The X_cal network is switched by the XL/XC front-panel toggle:

  XC MODE (Capacitive Reactance):
    CV1: Variable air capacitor, 12–230 pF (main tuning)
    C_banks: DIP switch or rotary switch:
      SW1-A: +25 pF
      SW1-B: +50 pF
      SW1-C: +100 pF
      SW1-D: +200 pF
    Total capacitance: 12 pF to 605 pF

    Reactance at band centers:

    Band  f(MHz)  Cmin=12pF   Cmax=605pF
    ----  ------  ----------  ----------
    160M  1.85    7,196Ω      142Ω
    80M   3.75    3,552Ω      70Ω
    40M   7.15    1,862Ω      37Ω
    20M  14.175    939Ω       19Ω
    10M  28.5      467Ω        9Ω

    For XC in the 0–200Ω range, select the capacitor bank that
    puts the required capacitance within the 12–230 pF range of
    the variable capacitor.

  XL MODE (Inductive Reactance):
    L_banks: Switched toroid inductors (T-37-6 or T-50-6 core):
      L1: 22 μH  (160M band low XL)
      L2: 10 μH  (80M–40M)
      L3: 4.7 μH (40M–20M)
      L4: 2.2 μH (20M–15M)
    Inductors may be paralleled to reach lower values.

    Reactance at band centers:

    Band  f(MHz)  L1=22μH  L2=10μH  L3=4.7μH  L4=2.2μH
    ----  ------  -------  -------  --------  --------
    160M  1.85    256Ω     116Ω     55Ω       26Ω
    80M   3.75    518Ω     236Ω     111Ω      52Ω
    40M   7.15    989Ω     449Ω     211Ω      99Ω
    20M  14.175  1959Ω     890Ω     419Ω      196Ω
    10M  28.5         (too high for range)     394Ω

3-2.5  TRANSFORMER T1 — TRIFILAR WINDING PROCEDURE

  Core: BN-43-202 binocular (two-hole) ferrite core, Type 43 material
        Dimensions: 19 mm OD, 9.5 mm ID per hole, 13 mm long
        Permeability: μ_i = 850

  Winding:
    1.  Cut three lengths of 26 AWG enameled wire, each 300 mm (12").
        Mark one end of each with color marker:
        Wire A = RED (Noise port)
        Wire B = WHITE (ANT port)
        Wire C = BLUE (DET port)

    2.  Hold all three wires parallel. Pass all three together
        through the first hole of the binocular core and back
        through the second hole. This completes ONE turn (trifilar).

    3.  Continue threading for 6 complete turns total. Keep all
        three wires parallel; do not cross or twist.

    4.  At each end, you will have the start and finish of each
        winding (6 wire ends total on each side = 12 wire ends).

    5.  Trim and scrape insulation. Measure inductance of each
        winding: should be 2–3 μH at 1 kHz. All three should
        be within 5% of each other.

    6.  Connections:
        Wire A START → Noise In (R10 coupling cap)
        Wire A FINISH → GND
        Wire B START → ANT port (J1 center pin)
        Wire B FINISH → GND
        Wire C START → DET port (J4 center pin)
        Wire C FINISH → GND via series R_cal/X_cal network

  CAUTION: The three windings must be IDENTICAL in length and
  winding direction. An error in winding count or direction will
  reduce null depth and produce incorrect readings.

--------------------------------------------------------------------------------

3-3. NULL DETECTOR ASSEMBLY

3-3.1  OVERVIEW

Three null detector modes are available. All share the DET/RX BNC
connector input. The selector switch (S2) routes the DET signal to
the desired detector.

3-3.2  EXTERNAL RECEIVER MODE

In this mode, S2 routes the detector port directly to the DET/RX BNC
on the front panel. The operator connects an external receiver.

No internal electronics are involved in this path. The DET/RX port
presents approximately 50Ω input impedance from the bridge T1
detector winding impedance.

3-3.3  BUILT-IN AUDIO DETECTOR

  U3 (ERA-3SM):    20 dB gain preamplifier ahead of detector
                   Bias resistor R6: 62Ω for 5V (use 270Ω for 18V)
                   Input: DET/RX port
                   Output: BAT54A envelope detector

  D2 (BAT54A):     Schottky diode, envelope detector
                   Followed by 10 kΩ load and 100 pF RF bypass

  U4 (LM386):      Low-voltage audio amplifier
                   Gain set by C6 (10 μF, pins 1–8) = 200×
                   Input: envelope detector output through R7 (10 kΩ)
                   Output: Zobel network (10Ω + 0.047 μF), then VR2
  VR2:             1 kΩ volume potentiometer
  J5:              3.5 mm headphone jack (front panel, PHONES)

3-3.4  AD8307 LOG DETECTOR (CYD Interface)

  U5 (AD8307):     Logarithmic detector, −75 to +17 dBm
                   Frequency range: 1 MHz to 500 MHz
                   Slope: 25 mV/dB
                   Intercept: approximately −84 dBm
                   Input impedance: 1 kΩ || 1.4 pF → match to 50Ω
                                    with 52.3Ω series resistor
                   VOUT pin: 0 to 2.5V over dynamic range

  Input matching:
    R8 (52.3Ω → use 51Ω ±1%): Series input match to 50Ω
    C7 (100 pF): RF bypass at AD8307 input

  Output conditioning:
    R9 (1 kΩ): Current limit for ADC protection
    D3 (BZX84C3V3): 3.3V Zener clamp on ADC input line
    C8 (0.1 μF): Low-pass filter, reduces ADC aliasing noise

  Connection:
    U5 VOUT → R9 → D3 → GPIO34 (ESP32 ADC1_CH6)

  Calibration:
    At null: Vout = ~0.5V (bridge nulled = low signal at DET)
    No null: Vout = ~1.5–2.0V (bridge not nulled = high signal)
    ADC value: 0–4095, 12-bit, 3.3V reference
    Code relationship: lower ADC value = better null

--------------------------------------------------------------------------------

3-4. DIGITAL READOUT ASSEMBLY (CYD)

3-4.1  HARDWARE

The CYD module is an ESP32-2432S028 development board:
  Processor:   ESP32 dual-core 240 MHz, 520 KB SRAM, 4 MB Flash
  Display:     ILI9341 2.8" TFT, 320 × 240 pixels, 16-bit color
  Touch:       XPT2046 4-wire resistive touch
  Connector:   Standard 38-pin ESP32 devkit pinout

CYD GPIO ASSIGNMENTS FOR THIS APPLICATION:

  GPIO  Function           Direction  Notes
  ----  --------           ---------  -----
  39    R_cal ADC          Input      ADC1_CH3 (VP), R pot wiper
  36    X_cal ADC          Input      ADC1_CH0 (VN), X pot wiper
  34    AD8307 null        Input      ADC1_CH6, null depth indicator
  35    Battery monitor    Input      ADC1_CH7, 1/3 voltage divider
  25    X polarity switch  Input      HIGH=inductive, LOW=capacitive
  26    Noise source EN    Output     DAC2, 0V=off, 3.3V=on
  21    TFT backlight      Output     PWM via ledcWrite()
  4     LED_R (debug)      Output     Active LOW
  13    TFT MOSI           SPI        Built-in CYD
  14    TFT CLK            SPI        Built-in CYD
  15    TFT CS             SPI        Built-in CYD
  2     TFT DC             GPIO       Built-in CYD
  12    TFT RST            GPIO       Built-in CYD
  33    Touch CS           GPIO       Built-in CYD (XPT2046)

NOTE: GPIO12 is shared between TFT_RST and the Touch CS
configuration. The CYD board routes this correctly internally.
Do NOT add external connections to GPIO12.

3-4.2  ADC CONNECTIONS

All ADC inputs use ADC1 channels only. ADC2 is unavailable when
WiFi is enabled. WiFi is not used in this application, but ADC1
is preferred to avoid conflicts.

R pot wiper → voltage divider protection:
  10 kΩ series resistor between wiper and GPIO39
  This limits current if pot wiper fails open and protects ADC

X pot wiper → same: 10 kΩ series to GPIO36

Battery monitor:
  18V battery → 100 kΩ → junction → 47 kΩ → GND
  GPIO35 = 18V × 47 / (100 + 47) = 18V × 0.320 = 5.75V max
  That exceeds 3.3V! Adjust divider:
  Use 220 kΩ / 56 kΩ: Vout = 18 × 56/276 = 3.65V (borderline)
  RECOMMENDED: Use 330 kΩ / 56 kΩ: Vout = 18 × 56/386 = 2.61V
  (ADC reads battery, convert: V_batt = ADC_V × 386 / 56)

3-4.3  FIRMWARE SCREENS

The CYD firmware provides five operating screens, selectable via
touch:

  SCREEN 1 — MAIN READOUT (default)
    Large text display:
      R = XX.X Ω
      X = ±XX.X Ω (XC or XL label)
      |Z| = XX.X Ω  Θ = ±XX.X°
      SWR = X.XX:1   RL = XX.X dB
      C = XXX pF  or  L = XXX nH  (frequency-dependent)
    NULL bargraph (30 segments, AD8307 reading)
    Battery indicator (top right)
    Touch buttons (bottom): BAND− | BAND+ | HOLD | CAL | SMITH

  SCREEN 2 — SMITH CHART
    Full Smith chart with resistance circles and reactance arcs
    Plotted point (current R, X) with 32-point trail history
    SWR reference circles at 1.5, 2.0, 3.0
    Band label and R/X readout in corner
    Touch buttons: BACK | CLEAR | PLOT | HOLD

  SCREEN 3 — BAND SELECT
    Grid of 10 band buttons (160M–6M)
    Selected band highlighted
    Shows band frequency when selected

  SCREEN 4 — CALIBRATION
    Step-by-step calibration guide:
      Step 1: Connect SHORT standard, zero R/X offsets
      Step 2: Connect 50Ω load, verify R=50, X=0
      Step 3: Connect 100Ω load, verify R=100, X=0
      Step 4: Apply scale factors and save to EEPROM
    Touch: STEP+ | STEP− | APPLY | RESET | BACK

  SCREEN 5 — HISTORY
    Table of last 10 measurements (band, R, X, SWR)
    Date/time from ESP32 RTC (set at upload, no GPS)

3-4.4  COMPUTED QUANTITIES

All derived quantities computed from R and X:

    |Z| = √(R² + X²)                         [Equation 3-1]
    θ   = arctan(X / R) × (180/π)            [Equation 3-2]
    ρ   = |Γ|  (from Equations 2-6, 2-7)     [Equation 3-3]
    SWR = (1 + ρ) / (1 − ρ)  (clamped 1–99) [Equation 3-4]
    RL  = −20 × log10(ρ) dB                  [Equation 3-5]

  If X > 0 (capacitive):
    C   = 1 / (2π × f_band × X) × 10¹²  pF  [Equation 3-6]

  If X < 0 (inductive):
    L   = (−X) / (2π × f_band) × 10⁹   nH  [Equation 3-7]

  Where f_band is the selected band center frequency in Hz.

--------------------------------------------------------------------------------

3-5. POWER SUPPLY

3-5.1  The noise bridge operates from 18V DC derived from two 9V
alkaline batteries (PP3/6F22 form factor) connected in series.

  Battery type:     PP3 alkaline (e.g., Duracell MN1604, Energizer 522)
  Quantity:         2 (series = 18V nominal)
  Capacity:         550–600 mAh (typical alkaline)
  Operating range:  16.5V (fresh) to 14V (end of life)

3-5.2  CURRENT CONSUMPTION

  Assembly          Current       18V Power
  --------          -------       ---------
  Noise Gen (NGZ)   35 mA         0.63 W
  Bridge/passives   2 mA          0.04 W
  LM386 (active)    8 mA          0.14 W
  AD8307            12 mA         0.22 W
  ERA-3SM (preamp)  25 mA         0.45 W
  ESP32 (active)    80–120 mA     1.8 W
  CYD TFT backlight 80 mA        1.44 W
  TOTAL (max)       ~362 mA       ~6.5 W

  Expected battery life: 550 mAh / 362 mA ≈ 1.5 hours at full load.

  To extend battery life:
    a.  Reduce TFT backlight (set GPIO21 PWM duty cycle to 50–60%).
    b.  Disable noise source (GPIO26 LOW) when not making measurements.
    c.  Use sleep mode between measurements.
    d.  Increase duty cycle: expect 4–6 hours typical field use.

3-5.3  BATTERY COMPARTMENT

The battery compartment is located in the rear of the enclosure,
accessible by removing the battery door (single thumb screw or
snap-fit latch). Two PP3 batteries are installed with correct
polarity (marked on compartment floor) and connected in series.

A 1-amp polyfuse (RXEF010 or equivalent) is installed in the
positive supply line. The polyfuse provides automatic reset
protection against battery reversal damage.

--------------------------------------------------------------------------------

3-6. ENCLOSURE AND MECHANICAL

3-6.1  ENCLOSURE DIMENSIONS

  Overall:         180 × 120 × 55 mm (body, excluding lid)
  Lid height:      15 mm (including O-ring skirt overlap)
  Wall thickness:  3.5 mm ASA plastic
  Print material:  ASA (acrylonitrile styrene acrylate)
    - UV resistant (outdoor use)
    - Temperature resistant to +80°C
    - Chemical resistant (fuel, oil splash resistant)

3-6.2  FRONT PANEL (Y = 0 face)

  Connector      Position    Cutout      Purpose
  ---------      --------    ------      -------
  J1 ANT         X=20mm      SO-239      Antenna under test
  J3 NOISE IN    X=55mm      BNC         External noise source
  J4 DET/RX      X=90mm      BNC         Detector output / ext RX
  J5 PHONES      X=130mm     3.5mm       Headphone output

3-6.3  LID (TOP FACE)

  Control        Position      Purpose
  -------        --------      -------
  CYD Display    Centered      ILI9341 TFT readout and touch
  VR1 (R pot)    X=30mm        R_cal 0–200Ω
  VR3 (X pot)    X=80mm        X_cal magnitude
  S_XC/XL        X=130mm       Reactance polarity (DPDT toggle)
  S_PWR          X=155mm Y=25  Power on/off (rocker)
  S_NOISE        X=155mm Y=55  Noise source on/off (rocker)

3-6.4  O-RING SEAL

  The lid seals to the body via an 80 × 3 mm standard O-ring seated
  in a groove on the underside of the lid. The groove is 3.5 mm wide
  × 1.5 mm deep. Use silicone grease (Dow Corning DC-4 or equivalent)
  on the O-ring before assembly. Do not use petroleum jelly on silicone
  O-rings.

================================================================================
CHAPTER 4  CONSTRUCTION
================================================================================

4-1. MATERIALS AND PARTS LIST

4-1.1  COMPLETE BILL OF MATERIALS

  Ref   Qty  Description                    Value/Part#         Source
  ---   ---  -----------                    -----------         ------

  NOISE GENERATOR (NGZ)
  D1    1    Zener diode                    1N4733A, 5.1V/1W    Mouser
  U1    1    MMIC amplifier                 MAR-6SM             Mini-Circuits
  R1    1    Resistor, 1/4W metal film      4.7 kΩ ±1%
  R2    1    Resistor, 1/4W metal film      430 Ω ±5% (18V)
  R3    1    Resistor, 1/4W metal film      51 Ω ±1%
  C1    1    Capacitor, ceramic             0.1 μF, 50V
  C2    1    Capacitor, ceramic             1000 pF, 50V
  C3    1    Capacitor, ceramic             1000 pF, 50V
  C4    1    Capacitor, ceramic             1000 pF, 50V
  RFC1  1    RF choke                       100 nH, 0207 axial
  Q_EN  1    NPN transistor (enable switch) 2N3904

  BRIDGE CIRCUIT
  T1    1    Trifilar transformer core      BN-43-202           Amidon
  T1-W  1    Enameled wire, 26 AWG          ~1 meter, 3 colors
  R_A   1    Resistor, 1/4W metal film      51 Ω ±1%
  R_B   1    Resistor, 1/4W metal film      51 Ω ±1% (match R_A)
  R5    1    Resistor, 1/4W metal film      10 Ω ±1%
  VR1   1    Potentiometer, 10-turn cermet  200 Ω, Bourns 3590S  Mouser
  CV1   1    Variable capacitor, air        12–230 pF           W8NX/eBay
  C-SW  1    DIP switch, 4-position         SPST                Mouser
  C_A   1    Capacitor, ceramic             25 pF, 100V NPO
  C_B   1    Capacitor, ceramic             50 pF, 100V NPO
  C_C   1    Capacitor, ceramic             100 pF, 100V NPO
  C_D   1    Capacitor, ceramic             200 pF, 100V NPO
  L1    1    Toroid inductor                22 μH, T-50-6 core  Amidon
  L2    1    Toroid inductor                10 μH, T-50-6 core
  L3    1    Toroid inductor                4.7 μH, T-37-6 core
  L4    1    Toroid inductor                2.2 μH, T-37-6 core
  S1    1    Toggle switch, DPDT            CW Industries        Mouser
  J1    1    SO-239 chassis connector       UHF female          Amphenol
  J3    1    BNC chassis connector          50Ω female          Amphenol
  J4    1    BNC chassis connector          50Ω female          Amphenol

  NULL DETECTOR
  U3    1    MMIC amplifier, preamp         ERA-3SM             Mini-Circuits
  R6    1    Bias resistor                  270 Ω ±5% (18V)
  D2    1    Schottky diode, detector       BAT54A              Mouser
  C6    1    Electrolytic capacitor         10 μF, 25V
  R7    1    Resistor                       10 kΩ ±5%
  U4    1    Audio amplifier                LM386N-4            Texas Instr.
  VR2   1    Potentiometer, linear taper    1 kΩ audio          Mouser
  J5    1    Phone jack, 3.5 mm             PJ-325              CUI
  C_Z1  1    Capacitor, ceramic             0.047 μF
  R_Z1  1    Resistor, 1/4W                 10 Ω (Zobel)
  U5    1    Logarithmic amplifier          AD8307AN            Analog Dev.
  R8    1    Resistor                       51 Ω ±1%
  R9    1    Resistor                       1 kΩ ±5%
  D3    1    Zener diode, 3.3V              BZX84C3V3           Mouser
  C7    1    Capacitor, ceramic             100 pF NPO
  C8    1    Capacitor, ceramic             0.1 μF

  DIGITAL READOUT (CYD)
  U6    1    CYD dev board                  ESP32-2432S028      AliExpress
  R10   1    Resistor                       10 kΩ (R ADC protect)
  R11   1    Resistor                       10 kΩ (X ADC protect)
  R12   1    Resistor                       330 kΩ (batt divider)
  R13   1    Resistor                       56 kΩ  (batt divider)
  S2    1    Toggle switch, SPDT            XC/XL polarity       Mouser

  POWER SUPPLY
  BT1   2    Battery, 9V alkaline           PP3 / 6F22 / MN1604  ---
  F1    1    Polyfuse, 1A                   RXEF010 or MF-R110   Mouser
  SW_P  1    Rocker switch, DPST            15 mm panel          Mouser
  SW_N  1    Rocker switch, SPST            15 mm panel          Mouser
  BH1   2    Battery holder, 9V clip                             Mouser

  ENCLOSURE
  ENC1  1    Enclosure body                 (3D print ASA)
  ENC2  1    Enclosure lid                  (3D print ASA)
  ENC3  1    Battery door                   (3D print ASA)
  OR1   1    O-ring, silicone               80 × 3 mm            McMaster
  FT1   4    Rubber feet                    3M SJ5012 or similar
  SCR1  6    Screw, M3 × 10 stainless       lid bolts
  SCR2  4    Screw, M3 × 6 stainless        CYD mounting
  INS1  4    Insert, M3 heat-set                                 McMaster
  PCB1  1    PCB, main bridge               NB-PCB-001 (gerbers)
  PCB2  1    PCB, CYD carrier               NB-PCB-002 (gerbers)

  HARDWARE/WIRING
  W1    1    Coax, RG-174                   ~0.5 m, noise to bridge
  W2    1    Coax, RG-174                   ~0.3 m, bridge to DET
  W3    --   Hook-up wire, 24 AWG           Various colors, ~2 m

--------------------------------------------------------------------------------

4-2. PCB FABRICATION

4-2.1  The main bridge PCB (NB-PCB-001) should be fabricated from
FR-4 with the following specifications:

  Board material:  FR-4, 1.6 mm thickness
  Copper weight:   1 oz (35 μm) both sides
  Finish:          HASL-LF or ENIG
  Solder mask:     Both sides
  Silkscreen:      Top side
  Minimum trace:   0.25 mm (10 mil)
  Minimum space:   0.25 mm (10 mil)
  Board size:      80 mm × 60 mm (approximate)

4-2.2  CRITICAL PCB LAYOUT RULES

  a.  The bridge components (T1, R_A, R_B, R_cal, X_cal) form a
      sensitive RF circuit. Route all bridge connections as SHORT
      as possible. Long traces add inductive parasitics that
      degrade null depth and introduce frequency-dependent errors.

  b.  Place T1 at the center of the bridge area with its three
      winding connections going directly to the three bridge nodes.
      Do not route T1 connections through vias if avoidable.

  c.  Provide a solid ground plane on the bottom layer. Connect
      all ground points to the plane with short vias or direct
      contact.

  d.  Keep noise generator circuitry physically separated from the
      detector circuitry. Cross-coupling between noise out and
      detector in creates a residual null floor that cannot be
      calibrated out.

  e.  Place all SMD bypass capacitors (100 nF) directly at each
      IC power supply pin, with the shortest possible trace to
      the ground plane.

  f.  The AD8307 input is sensitive to PCB contamination. Use a
      clean flux residue remover after soldering the AD8307 area.
      Flux residue can create a conductive path that degrades HF
      response.

--------------------------------------------------------------------------------

4-3. TRANSFORMER WINDING (T1)

Follow the procedure in Section 3-2.5. Additional construction notes:

4-3.1  WIRE SELECTION

  Use 26 AWG (0.4 mm) enameled (magnet) wire. Thinner wire (28–30
  AWG) is acceptable but more fragile. The three windings must be
  identical; cut all three wires to the same length (300 mm) before
  beginning.

4-3.2  HANDLING THE BINOCULAR CORE

  The BN-43-202 ferrite core is brittle. Do not drop it on a hard
  surface. Ferrite chips from impact damage on the winding area will
  cause transformer nonlinearity.

  Wrap the core with one layer of Teflon (PTFE) plumber's tape before
  winding to protect the wire insulation from core edges.

4-3.3  TURN COUNT VERIFICATION

  After winding, verify the turn count by measuring inductance:
  Target: 2–3 μH per winding at 1 kHz.
  Use an LCR meter or impedance bridge. If inductance is out of
  range, rewinding is required.

  Expected inductance formula:
    L ≈ N² × A_L
    A_L (BN-43-202) ≈ 3,400 nH per turn² at 1 kHz
    6 turns: L ≈ 36 × 3,400 nH = 122,400 nH ≈ 122 μH

NOTE: The above estimate is for a single winding without coupling.
Mutual inductance between the trifilar windings modifies this.
Typical measured value will be 2–5 μH per winding in the trifilar
configuration at RF frequencies due to distributed self-resonance
and leakage flux. A working transformer measures 2–4 μH per winding
at 1 MHz; this is within spec.

4-3.4  CONTINUITY CHECK

Before installation, verify with a DMM:
  a.  Each winding is continuous end-to-end (< 2 Ω).
  b.  No shorts between windings (should read > 1 MΩ).
  c.  No shorts from any winding to ferrite core (if core is
      conductive: some ferrites are slightly conductive at low
      voltage; verify no leakage at 50V if available).

--------------------------------------------------------------------------------

4-4. NOISE GENERATOR ASSEMBLY

4-4.1  Assemble the NGZ noise source on a small section of PCB
or Manhattan-style on a copper-clad board:

  Step 1:  Install bias resistor R1 (4.7 kΩ) and zener D1 (1N4733A).
           Verify D1 orientation: cathode (banded end) to +18V rail.

  Step 2:  Install coupling capacitors C1–C4 (0.1 μF and 1000 pF).
           Ceramic capacitors; orientation does not matter.

  Step 3:  Install RFC1 (100 nH choke). Keep lead length short.

  Step 4:  Install MAR-6SM MMIC (U1). Note orientation:
           Pin 1 = IN (tab toward input). SOT-89 package.
           The MMIC is static-sensitive. Use ESD precautions.

  Step 5:  Install bias resistor R2 (430 Ω for 18V supply).

  Step 6:  Install output resistor R3 (51 Ω).

  Step 7:  Install noise source enable transistor Q_EN (2N3904).
           Base → GPIO26 (via 1 kΩ series R), Emitter → GND,
           Collector → noise generator +18V rail.

  Step 8:  Apply power (18V) and measure voltage at U1 bias pin.
           Should read 4.5V–5.5V. If not, recheck R2 value.

  Step 9:  Verify noise output on spectrum analyzer or by connecting
           an external receiver: noise should be audible as broadband
           hiss at any frequency from 1–30 MHz.

--------------------------------------------------------------------------------

4-5. BRIDGE CIRCUIT ASSEMBLY

4-5.1  Assemble in this order to minimize rework:

  Step 1:  Install and verify T1 (per Section 4-3).

  Step 2:  Install fixed resistors R_A and R_B (matched 51 Ω pair).
           Measure each with DMM before soldering; select matched pair
           within ±0.2 Ω of each other.

  Step 3:  Install VR1 (200 Ω 10-turn cermet). Mount with shaft
           perpendicular to PCB top surface for panel coupling.

  Step 4:  Install R5 (10 Ω) in series with VR1 wiper output.

  Step 5:  Mount CV1 (air variable capacitor) on PCB with standoffs.
           Connect rotor to circuit ground, stator to X_cal node.
           CAUTION: Air variable capacitors have a maximum voltage
           rating; keep RF voltage across CV1 below 100V RMS at
           the highest power levels.

  Step 6:  Install switched capacitor banks (C_A through C_D) with
           DIP switch SW-C.

  Step 7:  Wind and install toroid inductors L1–L4:
           L1 (22 μH):  T-50-6 core, approximately 47 turns #28 AWG
           L2 (10 μH):  T-50-6 core, approximately 31 turns #28 AWG
           L3 (4.7 μH): T-37-6 core, approximately 21 turns #28 AWG
           L4 (2.2 μH): T-37-6 core, approximately 15 turns #28 AWG
           Verify inductance after winding; adjust turns ±1 as needed.

  Step 8:  Install DPDT toggle switch S1 (XL/XC direction).

  Step 9:  Install SMA/BNC connectors J1 (ANT), J3, J4. Use
           insulated connectors mounted directly to PCB edge or via
           short RG-174 coax pigtails (<50 mm) to chassis connectors.

4-5.2  BRIDGE WIRING SUMMARY

    T1 Winding A (RED): → R10 (coupling cap) → Noise Gen output
    T1 Winding B (WHT): → J1 center (ANT port), shield to GND
    T1 Winding C (BLU): → R_cal/X_cal network, other end to GND
    R_A: Between T1-A node and T1-B node (ANT side)
    R_B: Between T1-A node and T1-C node (R_cal side)
    R_cal (VR1+R5): Between T1-C node and GND
    X_cal (CV1/C_banks/L_banks/S1): Parallel with R_cal,
          switched in by XL/XC switch

--------------------------------------------------------------------------------

4-6. NULL DETECTOR ASSEMBLY

  Step 1:  Install ERA-3SM (U3) preamplifier.
           Bias resistor R6: 270 Ω for 18V supply.
           Verify bias current: ~25 mA (1V drop across 39Ω internal
           resistor = approximately correct bias point).

  Step 2:  Install BAT54A detector diode (D2).
           Load resistor: 10 kΩ.
           RF bypass: 100 pF ceramic.

  Step 3:  Install LM386 (U4) audio amplifier.
           Use DIP-8 socket to facilitate replacement.
           Add 10 μF capacitor (C6) between pins 1 and 8 for
           200× gain setting.
           Decouple pin 6 (Vs) with 100 μF + 0.1 μF to GND.

  Step 4:  Install Zobel network: 10 Ω + 0.047 μF in series from
           LM386 output (pin 5) to GND.

  Step 5:  Install volume pot VR2 (1 kΩ) and headphone jack J5.

  Step 6:  Install AD8307 (U5) log detector.
           CAUTION: AD8307 is available in DIP-8 (AD8307AN) and
           SOIC-8 (AD8307AR). The DIP package is easier for
           hand assembly; use a socket.
           Input match: 51 Ω series (R8) to INHI pin.
           Bypass INLO to GND with 100 pF (C7).
           Bypass VS pin with 0.1 μF ceramic (C_VS) to GND.
           Connect VOUT through 1 kΩ (R9) and 3.3V Zener (D3)
           to GPIO34.
           Verify: Apply known RF signal (e.g., −30 dBm at 10 MHz)
           to AD8307 input. Measure VOUT with DMM. Should read
           approximately:
             V = (Pin_dBm − (−84 dBm)) × 25 mV/dB
             V = (−30 − (−84)) × 0.025 = 54 × 0.025 = 1.35V
           If reading is within ±0.2V, AD8307 is functioning.

--------------------------------------------------------------------------------

4-7. DIGITAL READOUT ASSEMBLY

  Step 1:  Program the ESP32 BEFORE mounting in the enclosure.
           Use Arduino IDE 2.x with ESP32 board package 3.x.
           Required libraries:
             TFT_eSPI (by Bodmer, v2.5.x or later)
             XPT2046_Touchscreen (Paul Stoffregen)
             Wire (built-in)
             EEPROM (built-in for ESP32)
           Configure TFT_eSPI User_Setup.h for CYD:
             #define ILI9341_DRIVER
             #define TFT_CS   15
             #define TFT_DC    2
             #define TFT_RST  12
             #define TFT_MOSI 13
             #define TFT_SCLK 14
             #define TFT_MISO -1
             #define TOUCH_CS 33
             #define SPI_FREQUENCY       40000000
             #define SPI_TOUCH_FREQUENCY  2500000

  Step 2:  Flash firmware (noise_bridge_cyd.ino) and smith_chart.h.
           Verify all 5 screens appear on display.
           Verify touch calibration is possible on screen 4.

  Step 3:  Wire ADC inputs and control GPIOs per Section 3-4.1.

  Step 4:  Mount CYD board into carrier PCB (NB-PCB-002) using
           4× M3 × 6 mm screws and heat-set inserts.
           The display must align precisely with the lid window
           cutout (65 × 48 mm clear aperture in the lid).

  Step 5:  Install TFT lens (optional: 2.8-inch replacement
           touchscreen glass or acrylic protective lens to protect
           display from field damage).

--------------------------------------------------------------------------------

4-8. ENCLOSURE PREPARATION

4-8.1  3D PRINT REQUIREMENTS

  Files:    enclosure_noise_bridge.scad → STL
            front_panel.scad → STL (lid label plate, dial scales)

  Material: ASA filament (preferred for field use)
            PETG (acceptable for home/lab use)
            PLA (dry indoor use only; will warp in hot car)

  Slicer settings:
    Layer height:   0.2 mm
    Walls:          4 perimeters minimum
    Infill:         35% gyroid
    Supports:       Minimal (lid overhang only)
    Temperature:    ASA: 240°C nozzle / 100°C bed
                    Enclosure must be printed as two pieces
                    (body + lid) due to bed height limit

  Print time (approximate):
    Body:  8–12 hours
    Lid:   6–8 hours

4-8.2  POST-PRINT FINISHING

  Step 1:  Remove supports. Clean out any stringing.

  Step 2:  Install M3 heat-set inserts in lid bolt holes:
           6 inserts in body top rim (lid bolts)
           4 inserts in CYD mounting holes in lid interior

  Step 3:  Tap M3 threads in PCB boss holes (or use self-tapping
           M3 screws in printed bosses).

  Step 4:  Install rubber feet (3M SJ5012) in bottom corner recesses.

  Step 5:  Test-fit O-ring in lid groove. Apply thin film of
           silicone grease. O-ring should compress ~25% when lid
           bolts are tightened to approximately 0.5 N·m.

  Step 6:  Drill or ream connector holes if needed (3D print
           tolerances may be ±0.5 mm). Use stepped drill bit for
           SO-239 (21 mm), BNC (12 mm), and headphone jack (7 mm).

4-8.3  CONNECTOR INSTALLATION

  WARNING: Tighten connector hardware only to the manufacturer's
  recommended torque. Over-tightening will crack the ASA enclosure
  around connector holes. Use a washer to distribute clamping load.

  J1 (SO-239):  Mount with 4× #4-40 screws or single nut (if threaded
                body type). Apply lock washer. Torque: 0.8 N·m max.
  J3, J4 (BNC): Single hex nut. Torque: 0.8 N·m max.
  J5 (3.5 mm):  Snap-fit or nut mount per specific jack type.
  S1 (DPDT):    Panel thread, hex nut. Torque: 0.5 N·m max.
  S_PWR, S_N:   Rocker switches snap into rect cutout. No torque
                required; verify snap-fit retention.
  VR1, VR3:     Thread pot shaft through lid hole; secure with
                hex nut (provided with 3590S pots). Attach knob
                with set screw or press fit. Mount dial scale plate
                (from front_panel.scad) under knob with adhesive.

--------------------------------------------------------------------------------

4-9. FINAL INTEGRATION

  Step 1:  Mount main bridge PCB (NB-PCB-001) in body on PCB bosses.
           Connect J1/J3/J4 chassis connectors to PCB with short
           RG-174 coax pigtails. Keep pigtails < 50 mm.

  Step 2:  Connect VR1 (R pot) wiper and ends to PCB R_cal circuit.
           Route wires through grommet or hole in lid-to-body
           bulkhead.

  Step 3:  Connect VR3 (X pot) similarly.

  Step 4:  Connect S1 (XL/XC switch) to X_cal network.

  Step 5:  Connect battery holder wires to power switch S_PWR and
           polyfuse F1. Install batteries last (after all wiring
           complete and verified).

  Step 6:  Mount CYD carrier PCB (NB-PCB-002) in lid interior
           using heat-set inserts and M3 screws.

  Step 7:  Connect CYD GPIO wires through cable routing channel
           in lid interior. Keep RF-bearing wires (AD8307 output,
           noise source) away from CYD data lines. Use separate
           wire bundles.

  Step 8:  Test all functions with batteries installed but lid
           open:
             a.  Power on: CYD displays Main screen.
             b.  Touch BAND+: changes band.
             c.  Rotate R pot: R reading changes on CYD.
             d.  Rotate X pot: X reading changes on CYD.
             e.  Toggle XL/XC switch: X sign changes on CYD.
             f.  Connect external receiver to DET/RX; verify
                 broadband noise audible.
             g.  Connect a short to ANT port; rotate R pot to
                 minimum; should approach null at low R setting.
             h.  Connect a 50Ω dummy load to ANT port; adjust
                 R pot to mid-range (~50Ω marking); null should
                 improve.

  Step 9:  Close lid. Hand-tighten all 6 lid bolts evenly in
           star pattern.

  Step 10: Perform complete calibration per Chapter 5.

================================================================================
CHAPTER 5  CALIBRATION PROCEDURES
================================================================================

5-1. CALIBRATION OVERVIEW

5-1.1  Calibration establishes the accuracy relationship between:
  a.  The R pot mechanical position and the actual resistance it
      presents to the bridge.
  b.  The X pot / capacitor bank position and the actual reactance.
  c.  The CYD ADC readings and the actual R and X values.

5-1.2  Calibration is performed using a set of calibration
STANDARDS — known impedance references. The minimum required set:

  SHORT:   0 + j0 Ω     BNC short-circuit plug
  OPEN:    ∞ + j∞ Ω     BNC open connector
  50Ω:    50 + j0 Ω     BNC terminator, ±1% (e.g., Pomona BNC-T)
  100Ω:  100 + j0 Ω     Two 200Ω ±1% resistors in parallel
  200Ω:  200 + j0 Ω     Single 200Ω ±1% resistor (VNA cal std)

  These plugs should be measured on a calibrated NanoVNA or
  impedance analyzer before use. BNC terminator stubs are
  available from Pomona Electronics, SML, or fabricated from
  precision metal-film resistors in a BNC plug shell.

5-1.3  EQUIPMENT REQUIRED

  a.  External receiver with 50Ω input and AM/SSB capability,
      tunable from 1–30 MHz (e.g., SDR dongle + SDRSharp, or
      any communications receiver).
      OR
      Headphones for built-in audio null detection.

  b.  Calibration standards (see 5-1.2).

  c.  NanoVNA or precision DMM (for verifying standard values).

  d.  Computer with Arduino IDE (for EEPROM calibration save).

5-1.4  FREQUENCY SELECTION FOR CALIBRATION

Calibrate at the center frequency of the primary band of interest.
Recommended calibration frequency: 7.15 MHz (40M band center).
This frequency is in the middle of the instrument's frequency range
and provides well-behaved transformer characteristics.

Repeat calibration at a second frequency (14.175 MHz) to
characterize frequency-dependent errors.

--------------------------------------------------------------------------------

5-2. TEST EQUIPMENT REQUIRED

  Item  Description                    Specification
  ----  -----------                    -------------
  1     External receiver              1–30 MHz, AM/SSB
  2     Set of calibration standards   SHORT, 50Ω, 100Ω, 200Ω
  3     NanoVNA (optional verify)      50 kHz–300 MHz
  4     DMM with precision resistor    0.1% or better
  5     Headphones, 32–600 Ω           Standard 3.5 mm plug

--------------------------------------------------------------------------------

5-3. CALIBRATION STANDARDS PREPARATION

5-3.1  FABRICATING THE SHORT STANDARD

Materials:
  - BNC male plug (RG-58 crimp or solder type)
  - Bare copper wire, ~25 AWG

  Solder the copper wire directly across the center pin and
  body of the BNC plug. The short must be at the BNC mating
  plane, not at the end of a length of cable. Excess wire
  length adds inductive reactance at HF.

  Verify with DMM: <0.1 Ω from center pin to shell.
  Verify with NanoVNA at 14 MHz: |Z| <1 Ω preferred.

5-3.2  FABRICATING THE 50Ω STANDARD

Use a precision 50Ω BNC terminator plug. Commercial terminators
(Pomona 4287, Pasternack PE6016) are suitable. Alternatively:
  - Solder one 50Ω ±1% resistor (Vishay MRS25) between BNC
    center pin and shell. Keep leads as short as possible.
  - Verify at 14 MHz with NanoVNA: R > 49 Ω, |X| < 2 Ω.

5-3.3  FABRICATING THE 100Ω STANDARD

  Solder two 200Ω ±1% resistors in parallel in BNC plug.
  Result: 100Ω ±1%. Verify on NanoVNA: R > 99 Ω, |X| < 3 Ω.

5-3.4  FABRICATING THE 200Ω STANDARD

  Solder one 200Ω ±1% resistor in BNC plug.
  Verify on NanoVNA: R > 198 Ω, |X| < 5 Ω.

--------------------------------------------------------------------------------

5-4. RESISTANCE CALIBRATION (R-AXIS)

5-4.1  PURPOSE

This procedure establishes the relationship between VR1 (R pot)
angular position and the bridge R value (ohms) it presents to
the circuit. It also calibrates the CYD ADC offset and scale.

5-4.2  SETUP

  Step 1:  Power on instrument. External receiver connected to DET/RX.
           Receiver tuned to 7.15 MHz, SSB or CW mode, RF gain maximum.

  Step 2:  Set X_cal to zero: place XL/XC switch in XC position.
           Rotate CV1 to minimum capacitance (fully open plates).
           Ensure all capacitor bank switches are OFF.
           This minimizes X contribution to the balance condition.

5-4.3  SHORT STANDARD CALIBRATION (R = 0 Reference)

  Step 1:  Connect SHORT standard to ANT port (J1).

  Step 2:  Rotate VR1 (R pot) counterclockwise to minimum stop.
           This corresponds to R_cal = R5 = 10 Ω minimum.

  Step 3:  Listen for null on external receiver. The null is a
           reduction in the background hiss. Adjust VR1 slightly
           to find the null minimum.

  Step 4:  Record VR1 position (turn count from minimum stop):
           Turns to null = ___________
           ADC reading on CYD calibration screen (raw) = ___________
           Target R for short = 0 Ω (actual ~10 Ω with R5 offset)

  Step 5:  If CYD reads > 15 Ω for the short, adjust R_OFFSET in
           firmware calibration screen to correct.

5-4.4  50Ω STANDARD CALIBRATION

  Step 1:  Connect 50Ω standard to ANT port.

  Step 2:  Rotate VR1 to approximate midpoint (5 turns from stop).

  Step 3:  Adjust VR1 for deepest null on receiver. Fine-adjust
           for minimum audio level.

  Step 4:  Read CYD display: R = ___________
           Expected: 50 Ω ± 3 Ω

  Step 5:  If CYD reads high (e.g., 57 Ω), decrease R_SCALE.
           If CYD reads low (e.g., 43 Ω), increase R_SCALE.
           Adjustment formula:
             NEW_R_SCALE = OLD_R_SCALE × (50 / CYD_reading)
           Apply via CYD calibration screen.

  Step 6:  Repeat null-find and re-read CYD. Iterate until
           CYD reads 50 ± 2 Ω with 50Ω standard connected.

5-4.5  100Ω AND 200Ω STANDARD VERIFICATION

  Step 1:  Connect 100Ω standard. Null VR1. CYD should read
           100 ± 5 Ω. Record: ___________

  Step 2:  Connect 200Ω standard. Null VR1. CYD should read
           200 ± 10 Ω. Record: ___________

  Step 3:  If readings are accurate (within tolerance) for all
           three points, R calibration is complete. If linearity
           error exists (accurate at 50Ω but off at 200Ω), the
           pot or its bias circuit has a nonlinearity. Investigate
           before proceeding.

5-4.6  SAVE R CALIBRATION TO EEPROM

  On CYD calibration screen:
    Step 1:  Verify R_OFFSET and R_SCALE values are correct.
    Step 2:  Press SAVE to write to EEPROM.
    Step 3:  Power cycle the instrument. Verify CYD reads correct
             values on power-up (calibration is retained).

--------------------------------------------------------------------------------

5-5. REACTANCE CALIBRATION (X-AXIS)

5-5.1  PURPOSE

This procedure calibrates the XC (capacitive) and XL (inductive)
reactance readings. It characterizes the parasitic resistance of
the reactance network at the calibration frequency.

5-5.2  CAPACITIVE REACTANCE CALIBRATION

  Step 1:  Connect 50Ω standard to ANT port.
           Null R_cal to read R = 50 Ω (see step 5-4.4).

  Step 2:  Now introduce a capacitive reactance by temporarily
           substituting a known capacitor at the ANT port:
           Use a BNC-T adapter: 50Ω standard in series with
           a known capacitor (e.g., 100 pF, ±1% NPO/C0G type)
           or an LC network of known values.

  Alternative (preferred): Use a NanoVNA to measure a
  calibration antenna or network, then compare noise bridge
  readings to NanoVNA readings.

  Step 3:  Set X_cal to cancel the reactance of the reference:
           Select capacitor bank to appropriate range.
           Adjust CV1 until null deepens.
           Read CYD X display. Compare to expected XC.

  Step 4:  Adjust X_OFFSET and X_SCALE in CYD calibration
           to match expected values.

  Step 5:  Repeat for XL (inductive): connect a known
           inductor (e.g., 10 μH ±5%, resonance well above
           test frequency) in the ANT circuit and match.

5-5.3  X POLARITY VERIFICATION

  Step 1:  Connect a known capacitive load (capacitor > 100 pF
           at the test frequency). The CYD should display POSITIVE X
           when the XC/XL switch is in XC position and correctly
           nulled. Positive X = capacitive = XC convention.

  Step 2:  Connect a known inductive load. CYD should display
           NEGATIVE X when nulled in XL position.

  Step 3:  If signs are reversed, either reverse the GPIO25 polarity
           logic in firmware (change HIGH/LOW interpretation) or
           physically swap the XL/XC switch connections.

5-5.4  X CALIBRATION AT MULTIPLE FREQUENCIES

Because the X_cal network is frequency-dependent (XC = 1/ωC,
XL = ωL), the calibration is strictly correct only at the
calibration frequency. At other frequencies, apply the correction:

    X_corrected = X_displayed × (f_cal / f_actual)   [Equation 5-1]

NOTE: The CYD firmware implements this correction automatically
when the band is selected. The firmware stores X readings as
capacitor/inductor values (F or H) internally and recomputes X
at the selected band frequency. This is accurate as long as the
component values are correct; verify by spot-checking at multiple
bands.

--------------------------------------------------------------------------------

5-6. NULL DEPTH VERIFICATION

5-6.1  The null depth determines the measurement accuracy of the
bridge. A deeper null allows finer R and X resolution.

5-6.2  MINIMUM ACCEPTABLE NULL DEPTH: 20 dB

  This means the signal at the DET port at null should be at least
  20 dB below the signal when the bridge is deliberately imbalanced
  (e.g., with a 50Ω standard and R_cal at 0Ω).

5-6.3  PROCEDURE

  Step 1:  Connect 50Ω standard to ANT port.
  Step 2:  Set R_cal to null (approximately 50Ω position).
           Set X_cal to null (minimum reactance).
  Step 3:  Note AD8307 reading on CYD null bargraph at null:
           NULL_min = __________ dBm (approximately)

  Step 4:  Rotate R_cal to full CCW (0Ω position) to intentionally
           imbalance the bridge.
           NULL_off = __________ dBm

  Step 5:  Null depth = NULL_off − NULL_min = __________ dB

  ACCEPT: ≥20 dB null depth.
  REJECT: <20 dB — investigate transformer T1 winding, component
          placement, grounding, and stray coupling.

--------------------------------------------------------------------------------

5-7. CYD DIGITAL READOUT CALIBRATION

5-7.1  The CYD reads the R and X pot wipers via 12-bit ADC (0–4095
counts, 0–3.3V reference). Calibration maps ADC counts to Ω.

5-7.2  ADC LINEARITY VERIFICATION

  Step 1:  On CYD calibration screen, select RAW ADC mode.
  Step 2:  Rotate R pot from minimum to maximum (10 full turns).
           Verify ADC reading increases smoothly from near 0 to
           near 4095 (approximately). Actual max: ~4000 at 3.3V.
  Step 3:  Check for any discontinuities or jumps. A healthy
           pot with clean wiper shows smooth ADC increase.
           Dirty pots show erratic jumps; see Section 8-2 for
           cleaning procedure.

5-7.3  CALIBRATION CONSTANTS STORED IN EEPROM

  Variable      Default   Units   Description
  --------      -------   -----   -----------
  r_offset      0         Ω       R zero offset correction
  r_scale       1.000     ---     R scale multiplier
  x_offset      0         Ω       X zero offset correction
  x_scale       1.000     ---     X scale multiplier
  touch_cal[5]  factory   ---     Touch screen calibration array
  cal_valid     0xA5      byte    Magic byte to verify cal saved

  CAUTION: If EEPROM address 0 does not read 0xA5 on power-up,
  the firmware uses default values (uncalibrated). Always verify
  that SAVE was successful by power-cycling and re-checking values.

================================================================================
CHAPTER 6  OPERATING PROCEDURES
================================================================================

6-1. PREPARATION FOR USE

6-1.1  PRE-OPERATION CHECKLIST

  [ ] Batteries installed and charged (verify >15V on CYD battery
      indicator, or measure with DMM)
  [ ] Antenna under test connected to ANT (J1) port
  [ ] External receiver connected to DET/RX (J4) if using external
      null detection, OR headphones to PHONES (J5)
  [ ] Power switch ON: CYD powers up, Main screen displays
  [ ] Select correct band: press BAND+ or BAND− on touch screen
      to select the operating frequency of the antenna under test
  [ ] Noise source switch ON (S_NOISE)
  [ ] Verify noise audible on receiver or headphones

WARNING: DO NOT connect a transmitter output to the ANT port.
This instrument is a passive bridge and will be destroyed by
transmitter RF power. Disconnect all transmitters from the antenna
system before connecting the noise bridge.

6-1.2  EXTERNAL RECEIVER SETUP

  Set receiver to:
    Mode:     SSB (USB or LSB; either is acceptable) or CW
    Bandwidth: Narrowest available (500 Hz CW preferred for
               maximum null depth discrimination)
    Frequency: Exact desired measurement frequency
    AGC:       OFF (manual RF gain preferred)
    RF gain:   Maximum initially; reduce if receiver overloads

NOTE: The noise source produces broadband noise. The receiver
will overload if the antenna is close to the noise source output
level. Use a 10–20 dB attenuator ahead of the receiver if the
S-meter reads more than S9+10.

6-1.3  INITIAL SETTINGS

  VR1 (R pot):   Rotate to approximate expected R value.
                 Most antennas measure between 10 and 150 Ω.
                 Set to midpoint (5 turns, ~100 Ω) if unknown.

  X_cal:         Set XL/XC switch to XC (most common case for
                 non-resonant antennas off-frequency).
                 Set all capacitor bank switches OFF.
                 Rotate CV1 to midpoint.

--------------------------------------------------------------------------------

6-2. BRIDGE BALANCE PROCEDURE (MANUAL)

This is the fundamental measurement procedure. Master this
technique before relying on the CYD digital readout.

6-2.1  THE ITERATIVE NULL PROCEDURE

The bridge is balanced (nulled) by alternately adjusting R_cal
and X_cal until the signal at the null detector reaches its
minimum. This is an iterative process.

STEP 1: COARSE R ADJUSTMENT

  Slowly rotate VR1 (R pot) from minimum to maximum stop while
  monitoring the null detector output. Listen for a reduction in
  noise level (headphones) or watch the CYD null bargraph.

  Find the R pot position where the noise level is lowest.
  This is the coarse R null position.

  NOTE: At this point you may hear a broad reduction in noise.
  This coarse null may be 5–10 dB below the no-null level.
  It will sharpen significantly after X adjustment.

STEP 2: COARSE X ADJUSTMENT

  With R pot held at the coarse null position:

  For XC mode:
    a.  Adjust CV1 to find minimum detector reading.
    b.  If CV1 is at maximum capacitance (fully meshed) and the
        null is still improving, engage the next larger capacitor
        bank switch (SW1-A, +25 pF; then SW1-B, +50 pF, etc.)
        until the null begins to worsen, then back off one switch.

  For XL mode:
    a.  Select appropriate inductor bank for the band.
    b.  Adjust until minimum found.

  NOTE: If switching between XC and XL positions shows that
  the null is better in XC than XL, the antenna is capacitive
  (shorter than resonant or shortened loaded antenna).
  If XL gives a better null, the antenna is inductive (longer
  than resonant).

STEP 3: FINE R ADJUSTMENT

  With X_cal at the coarse null position, return to VR1 and
  fine-adjust for a deeper null. The null will be sharper now
  that X is approximately correct.

  The null dip at this step should be 15–25 dB below the off-
  null noise level. Adjust VR1 in very small increments.

STEP 4: FINE X ADJUSTMENT

  With R_cal at the fine null position, fine-adjust CV1 (or
  the inductor bank switch if the null is very close to a
  switch boundary) for the deepest null.

  CAUTION: The final null is very sharp. Small movements of
  CV1 (a few degrees of rotation) will significantly affect
  the null depth. Use slow, deliberate movements.

STEP 5: VERIFY FINAL NULL

  The deepest achievable null has been reached when additional
  adjustment in either direction only worsens the null. At this
  point:

  a.  Read R from VR1 dial (or CYD R display).
  b.  Read |X| from CV1 position / capacitor bank setting
      (or CYD X display).
  c.  Note XC or XL switch position for sign.

  The antenna impedance at the measurement frequency is:
    Z = R + jX  (where X is positive for XC, negative for XL)

6-2.2  TYPICAL ANTENNA READINGS

  Antenna type                    Expected R        Expected X
  -----------                     ----------        ----------
  Dipole at resonance             60–80 Ω           ~0 Ω
  Dipole shorter than resonant    30–60 Ω           +50 to +200 Ω (XC)
  Dipole longer than resonant     30–60 Ω           −50 to −200 Ω (XL)
  Vertical at resonance           20–50 Ω           ~0 Ω
  End-fed halfwave (EFHW)         1–5 kΩ (OOB*)     varies
  Multi-band trap antenna         40–80 Ω at trap f  ~0 Ω at trap f
  Beam (Yagi) fed                 20–100 Ω          ±0 to ±50 Ω
  Folded dipole                   280–320 Ω         ~0 Ω at resonance

  * End-fed halfwave antennas have impedance far outside this
    instrument's 200Ω range at fundamental frequency. Use a
    1:49 or 1:64 UNUN balun between the antenna and the instrument
    to bring impedance within measurement range.

6-2.3  ITERATING FOR DIFFICULT NULLS

Some antenna impedances are difficult to null due to:
  a.  Complex resonances in trap antennas
  b.  High SWR (|Z| far from 50Ω)
  c.  Antenna physically close to ground (low impedance, lossy)
  d.  Feedline acting as part of the antenna system

If a deep null cannot be achieved:
  Step 1:  Try inserting a 4:1 or 1:1 balun between instrument
           and feedline. This may resolve common-mode issues.
  Step 2:  Verify antenna is truly isolated from the instrument
           (check that coax shield currents are not coupling
           noise bridge output back to the detector port).
  Step 3:  Try measuring at a slightly different frequency where
           the antenna impedance may be closer to the instrument
           range.

--------------------------------------------------------------------------------

6-3. BRIDGE BALANCE PROCEDURE (CYD ASSISTED)

6-3.1  The CYD digital readout provides real-time R, X, and null
depth display. The null procedure is the same as Section 6-2 but
the operator watches the screen instead of listening.

6-3.2  CYD NULL BARGRAPH

  The null bargraph displays the AD8307 log detector output as
  a 30-segment bar. Segments light up from left (deep null, low
  signal) to right (no null, high signal).

  At perfect null:     5–10 segments lit (left-most)
  No null:             25–30 segments lit
  Good null (>20 dB):  <8 segments lit

6-3.3  SMITH CHART ASSISTED BALANCING

  Activate Screen 2 (SMITH) during measurement.
  The plotted point shows the current R, X reading.
  As you approach the null:
    a.  The plotted point moves on the Smith chart.
    b.  When the null is deep, the plotted point is stable.
    c.  When the null is shallow, the plotted point wanders.

  Use the Smith chart to identify whether the antenna is:
    - Near the center: Good match (Z ≈ 50Ω)
    - Left of center: Resistive < 50Ω
    - Right of center: Resistive > 50Ω
    - Upper hemisphere: Inductive (XL)
    - Lower hemisphere: Capacitive (XC)

6-3.4  TOUCH BUTTONS IN CYD OPERATION

  HOLD:   Freezes the current R, X display. Useful when the
          pot is being read physically and the display is
          updating faster than can be read.

  PLOT:   Stores the current R, X point to the trail history
          on the Smith chart.

  CAL:    Enters calibration screen.

  BAND+/−: Changes the selected band. The L/C computed
            value updates immediately.

--------------------------------------------------------------------------------

6-4. SMITH CHART PLOTTING (MANUAL)

6-4.1  PURPOSE

Manual Smith chart plotting provides a permanent graphical record
of antenna impedance. It is useful when the CYD is not available
or when a complete impedance sweep is required across multiple
frequencies.

6-4.2  REQUIRED ITEMS

  a.  Blank Smith chart (normalized to 50Ω). Print from ARRL
      Antenna Book or download from ARRL.org.
  b.  Fine-point pen or pencil.
  c.  Noise bridge with calibrated R and X dials.

6-4.3  PLOTTING PROCEDURE

  Step 1:  Perform the null procedure at the desired frequency
           (Section 6-2). Note R and X values.

  Step 2:  Compute normalized values:
             r_n = R / 50 Ω
             x_n = X / 50 Ω

  Step 3:  On the Smith chart, locate the resistance circle r = r_n.
           Circles are labeled on the real axis.

  Step 4:  On the same chart, locate the reactance arc x = x_n.
           Positive x = upper arc (inductive).
           Negative x = lower arc (capacitive).
           In this instrument's convention:
             XC (positive X) → lower arc (capacitive hemisphere)
             XL (negative X) → upper arc (inductive hemisphere)

  Step 5:  The impedance point is at the INTERSECTION of the
           resistance circle and reactance arc. Mark this point.

  Step 6:  Label the point with the frequency.

  Step 7:  To plot a frequency sweep, repeat for each frequency
           and connect points in order to show the impedance locus.

6-4.4  READING SWR FROM PLOTTED POINTS

  SWR can be read directly from the Smith chart:
  Construct a circle centered at the chart center (Γ = 0) passing
  through the plotted impedance point. The SWR = R value where this
  circle crosses the positive real axis (right of center).

  Alternatively, compute directly:
    ρ = √((r_n − 1)² + x_n²) / √((r_n + 1)² + x_n²)
    SWR = (1 + ρ) / (1 − ρ)

--------------------------------------------------------------------------------

6-5. SMITH CHART PLOTTING (CYD AUTOMATIC)

6-5.1  The CYD firmware automatically plots the current (R, X)
measurement point on the Smith chart display when Screen 2 is active
and the PLOT button is pressed, or when AUTO-PLOT mode is enabled.

6-5.2  MULTI-FREQUENCY SWEEP PROCEDURE

  Step 1:  Select SMITH screen on CYD.
  Step 2:  Enable noise source (S_NOISE on).
  Step 3:  Tune external receiver to first frequency of interest.
  Step 4:  Null the bridge (adjust R and X pots).
  Step 5:  Press PLOT. The point is added to the Smith chart trail.
  Step 6:  Change receiver frequency. Repeat steps 4–5.
  Step 7:  Continue across desired frequency range.

  Each PLOT press adds a point to the 32-point trail buffer.
  Older points appear as smaller, dimmer dots. The most recent
  point is the largest and brightest.

  Step 8:  Press CLEAR to erase trail and begin a new sweep.

6-5.3  SAVING SMITH CHART DATA

  NOTE: The CYD firmware does not currently provide non-volatile
  storage of Smith chart plot data. To preserve a measurement:
  a.  Photograph the CYD display.
  b.  Record R, X at each frequency in the history screen (Screen 5).
  c.  Export history via serial port (USB) with the firmware's
      serial debug output at 115200 baud.

--------------------------------------------------------------------------------

6-6. INTERPRETING RESULTS

6-6.1  RESONANT ANTENNA (Good Match)

Symptom:   R ≈ 50–75 Ω, X ≈ 0 Ω (within ±10 Ω).
Smith chart: Near center of chart.
SWR:       < 1.5:1
Action:    Antenna is resonant and well-matched. No tuning required.

6-6.2  CAPACITIVE ANTENNA (Short or Shortened)

Symptom:   R ≈ 20–60 Ω, X = +50 to +200 Ω (XC mode reads high).
Smith chart: Below center, in capacitive hemisphere.
SWR:       2:1 to 10:1 depending on |X|
Cause:     Antenna is shorter than resonant length.
Action:    Lengthen antenna, or add series inductance (loading coil)
           to resonate at operating frequency.

6-6.3  INDUCTIVE ANTENNA (Long or Loaded)

Symptom:   R ≈ 20–60 Ω, X = −50 to −200 Ω (XL mode reads high).
Smith chart: Above center, in inductive hemisphere.
SWR:       2:1 to 10:1 depending on |X|
Cause:     Antenna is longer than resonant length.
Action:    Shorten antenna, or add series capacitance.

6-6.4  HIGH-R ANTENNA

Symptom:   R > 150 Ω, X ≈ 0.
Smith chart: Right of center, near real axis.
SWR:       > 3:1
Cause:     Folded dipole, end-fed, or antenna with lossy ground system.
Action:    Impedance matching network required between antenna
           and feedline. Use transmatch or balun transformer.

6-6.5  LOW-R ANTENNA

Symptom:   R < 15 Ω, X ≈ 0.
Smith chart: Left of center, near real axis.
SWR:       > 3:1
Cause:     Short vertical antenna, antenna with poor ground or
           radial system, or trap antenna below trap frequency.
Action:    Improve ground system. Check feedline for common-mode
           current (use current choke/balun). Check for open or
           shorted coax.

6-6.6  OPEN OR SHORTED FEEDLINE

Symptom:   R near 0 Ω + X = extreme (>200 Ω) indicates SHORT
           at some point in feedline. R near 200 Ω + extreme X
           indicates OPEN.
Smith chart: Near outer edge of chart (high |Γ|).
SWR:       > 10:1
Action:    Check feedline continuity. TDR or ring-out with
           ohmmeter. Check all connectors for corrosion.

================================================================================
CHAPTER 7  TROUBLESHOOTING
================================================================================

7-1. TROUBLESHOOTING OVERVIEW

7-1.1  Approach all troubleshooting systematically. Determine which
functional assembly is at fault before replacing components:

  a.  If there is NO NOISE OUTPUT: Noise generator fault.
  b.  If noise is present but NO NULL is achievable: Bridge fault
      or transformer T1 problem.
  c.  If null is present but READINGS ARE INACCURATE: Calibration
      or potentiometer problem.
  d.  If CYD DISPLAY is wrong: ADC wiring, firmware, or CYD fault.

7-1.2  Use the following symptom tables to identify the fault.

--------------------------------------------------------------------------------

7-2. NO NULL OBSERVED

Symptom: Rotating VR1 and CV1 produces no change in detector
output. Noise level is constant regardless of control settings.

  Check 1:  NOISE SOURCE ON?
    Action:  Verify S_NOISE rocker switch is ON. Verify GPIO26
             output is HIGH when noise enabled (measure at Q_EN
             base with DMM: should be ~3V).
    Test:    Connect receiver to NOISE IN (J3) BNC. Verify noise
             audible at any frequency. If not, noise source is faulty.

  Check 2:  TRANSFORMER T1 CONTINUITY
    Action:  Remove PCB from enclosure. With power OFF, measure
             resistance of each T1 winding end-to-end.
             Expected: <5 Ω each.
             If any winding reads open: rewinding required.

  Check 3:  BRIDGE COMPONENT CONTINUITY
    Action:  Verify R_A and R_B are installed and read ~51 Ω each.
             Verify VR1 continuity (end-to-end: ~200 Ω + R5 = 210 Ω).

  Check 4:  DETECTOR PATH
    Action:  Connect receiver to DET/RX (J4) BNC directly.
             If noise is audible, detector port is working.
             If not, check T1 Winding C continuity and J4 connector.

  Check 5:  ANTENNA PORT (ANT J1) OPEN
    Action:  Verify a known standard (50Ω terminator) is connected
             to J1. An open ANT port produces no null.

--------------------------------------------------------------------------------

7-3. NOISY OR UNSTABLE NULL

Symptom: A null can be found but it is shallow (<15 dB) or drifts
as the controls are adjusted.

  Check 1:  STRAY RF PICKUP
    Action:  Move instrument away from computers, switching supplies,
             and other RF sources. Shield the instrument with a
             metal enclosure temporarily to verify.

  Check 2:  GROUND CONTINUITY
    Action:  Verify that all component grounds connect to a single
             solid RF ground. Lifted ground traces on PCB will cause
             unstable nulls.

  Check 3:  FERRITE CORE DAMAGE
    Action:  Inspect T1 binocular core for cracks. Even a hairline
             crack in the core area under the windings degrades
             transformer balance and reduces null depth.

  Check 4:  R_A / R_B MISMATCH
    Action:  Remove PCB and measure R_A and R_B in-circuit (power OFF).
             If mismatch > 1 Ω, replace with matched pair.

  Check 5:  PCB CONTAMINATION
    Action:  Clean PCB around T1, AD8307, and bridge node with
             isopropyl alcohol (90%+). Flux residue under components
             can create resistive leakage paths.

  Check 6:  LOOSE OR CORRODED CONNECTORS
    Action:  Inspect all BNC connectors. Oxidized BNC center pins
             add contact resistance in the null detector path.
             Clean with deoxit or contact cleaner. Verify connector
             shell ground connections.

--------------------------------------------------------------------------------

7-4. R READING OUT OF RANGE

Symptom: CYD displays R much higher or lower than expected for
the connected calibration standard.

  Check 1:  CALIBRATION CONSTANTS
    Action:  Go to CYD calibration screen. Verify r_offset and
             r_scale values are correct (saved from calibration).
             Reset to defaults if suspect.

  Check 2:  POT MECHANICAL FAILURE
    Action:  Rotate VR1 end-to-end. CYD raw ADC should sweep
             smoothly. If ADC value jumps or sticks, VR1 wiper
             is dirty or worn. See Section 8-2.

  Check 3:  ADC WIRING
    Action:  Measure voltage at GPIO39 with pot at minimum and
             maximum. Should vary from ~0.1V to ~3.1V.
             If stuck at 0V: R5 or 10kΩ series resistor open.
             If stuck at 3.3V: pot open or ADC pin floating.

  Check 4:  FEEDBACK OSCILLATION
    Action:  If reading oscillates rapidly, the ADC input is
             picking up RF from the noise source. Add additional
             0.1 μF capacitor directly at GPIO39 pin. This is a
             known issue with CYD board layout near high-power
             RF environments.

--------------------------------------------------------------------------------

7-5. X READING OUT OF RANGE

Symptom: CYD X reading is always at maximum, always at zero, or
does not track CV1 adjustment.

  Check 1:  XL/XC SWITCH
    Action:  Verify S1 contacts close properly in both positions.
             Measure switch contacts with DMM.

  Check 2:  CAPACITOR BANK SWITCHES
    Action:  Toggle each DIP switch and verify X reading changes
             on CYD. If no change, the switched capacitor may be
             open-circuit (lead-free solder joint failure on
             ceramic cap).

  Check 3:  INDUCTOR CONTINUITY (XL MODE)
    Action:  Measure each toroid inductor end-to-end with DMM.
             Expected: <5 Ω each. If any read open, rewinding or
             replacement required.

  Check 4:  CV1 OPERATION
    Action:  Rotate CV1 fully CW and CCW. X reading on CYD should
             swing from near 0 to near 200 Ω (in XC mode, without
             any bank switches engaged, at the calibration frequency).
             If no change, check CV1 wiring.

--------------------------------------------------------------------------------

7-6. CYD DISPLAY PROBLEMS

Symptom: CYD screen is blank, garbled, or touch is non-functional.

  Check 1:  POWER
    Action:  Verify 3.3V and 5V (from ESP32 regulator) present at
             CYD board. CYD requires USB-5V or 3.3V input.
             The 3590S pot and bridge run from 18V (via regulator);
             CYD has its own 3.3V LDO. Verify voltage at CYD 3V3 pin.

  Check 2:  TFT SPI CONNECTION
    Action:  Verify TFT_eSPI User_Setup.h matches CYD pinout.
             Default CYD requires: CS=15, DC=2, RST=12, MOSI=13,
             SCLK=14. If any of these are swapped, the display will
             be blank or show random pixels.

  Check 3:  FIRMWARE CRASH
    Action:  Connect USB to CYD board. Open serial monitor at
             115200 baud. Firmware prints boot messages and periodic
             debug data. If serial is silent, reflash firmware.
             If serial shows repeated reboot messages, there is a
             stack overflow or invalid memory access.

  Check 4:  TOUCH CALIBRATION LOST
    Action:  If touch inputs appear offset, touch calibration
             constants in EEPROM may be corrupted. On calibration
             screen, press RESET to restore factory touch cal,
             then re-calibrate touch.

--------------------------------------------------------------------------------

7-7. BATTERY / POWER PROBLEMS

Symptom: Instrument powers off unexpectedly, CYD battery indicator
shows low, or instrument will not power on.

  Check 1:  BATTERY VOLTAGE
    Action:  Measure battery voltage with DMM before installation.
             Two fresh PP3 alkaline: 9V each = 18V series.
             If < 15V total, replace both batteries simultaneously.
             NOTE: Replace BOTH batteries at the same time. One weak
             battery in a series string will be driven into reversal
             by the good battery, potentially damaging the battery.

  Check 2:  POLYFUSE TRIPPED
    Action:  Polyfuse F1 (1A) will trip on overcurrent (e.g., reversed
             battery polarity). Allow 5 minutes for polyfuse to cool
             and reset. If instrument powers on after 5 minutes and
             works normally, polyfuse has reset.
             If polyfuse trips repeatedly: short circuit in wiring.
             Locate and repair before resuming operation.

  Check 3:  POWER SWITCH CONTACTS
    Action:  Measure voltage at load side of S_PWR with switch ON.
             If zero, switch contacts are dirty or failed.
             Clean with contact cleaner or replace switch.

================================================================================
CHAPTER 8  MAINTENANCE
================================================================================

8-1. SCHEDULED MAINTENANCE

  Interval    Procedure
  --------    ---------
  Daily       Inspect connectors for physical damage
              Verify battery level before field deployment
  Monthly     Clean BNC connector center pins (deoxit)
              Check O-ring seal condition
              Verify CYD touch calibration is correct
  6 months    Clean potentiometer wipers (see 8-2)
              Inspect T1 winding for cracked insulation
              Check PCB for corrosion or solder joint cracking
  Annually    Full calibration check (per Chapter 5)
              Battery replacement (regardless of voltage reading)
              Inspect ferrite core (T1) for cracks
              Inspect variable capacitor (CV1) plates for corrosion

--------------------------------------------------------------------------------

8-2. POTENTIOMETER SERVICE

8-2.1  Cermet potentiometers (Bourns 3590S) are normally maintenance-
free but can develop noisy wipers after extended field use.

8-2.2  CLEANING PROCEDURE

  Step 1:  Remove pot from enclosure (disconnect wires and loosen
           shaft nut). Do not remove wiper/housing from shaft.

  Step 2:  Apply 1–2 drops of CRC QD Electronic Cleaner or
           deoxit D5 to the wiper access port (if present) or
           rotate the shaft while applying cleaner to the wiper
           track through the side opening.

  Step 3:  Rotate shaft 20 times end-to-end to work cleaner
           through the wiper path.

  Step 4:  Allow cleaner to evaporate fully before reinstalling.

  CAUTION: Do not use WD-40 or petroleum-based lubricants on
  cermet pots. These leave residue that attracts dust and
  accelerates wiper wear.

8-2.3  REPLACEMENT

  Bourns 3590S potentiometers have a rated life of >1,000,000
  shaft rotations. Failure before this point indicates mechanical
  damage or contamination. Replace with identical part.

  After replacement, perform full R and X axis calibration
  (Chapter 5) since mechanical tolerances vary between units.

--------------------------------------------------------------------------------

8-3. CONNECTOR INSPECTION

8-3.1  BNC CONNECTORS (J3, J4)

  Inspect center pin for corrosion (greenish deposits), bent
  or recessed pin (< 0 mm protrusion), and damaged center pin
  PTFE dielectric.

  Clean with isopropyl alcohol on a cotton swab. Do not use
  abrasives; BNC center pins are gold or silver plated.

8-3.2  SO-239 CONNECTOR (J1)

  Inspect for damage to threads, corrosion on contact surfaces,
  and cracked PTFE dielectric.

  Field corrosion protection: Apply a thin film of Vaseline
  or petroleum jelly to the threads only (not the contact area).
  Remove before reinstalling PL-259 plugs.

8-3.3  3.5mm HEADPHONE JACK (J5)

  Verify spring contact tension by inserting and removing a
  known-good 3.5 mm plug 10 times. If the contact fails to make,
  the jack must be replaced.

--------------------------------------------------------------------------------

8-4. BATTERY REPLACEMENT

8-4.1  REPLACEMENT SCHEDULE

  Replace batteries:
  a.  When CYD battery indicator drops below 2 of 5 bars.
  b.  When battery voltage measures < 14V (DMM at battery terminals).
  c.  Annually, regardless of voltage (prevents battery leakage
      damage to battery compartment).

WARNING: Do not mix old and new batteries. Replace both
simultaneously. Alkaline batteries can develop internal shorts
or leak when fully discharged or reversed.

8-4.2  PROCEDURE

  Step 1:  Power OFF (S_PWR to OFF position).
  Step 2:  Remove battery door (rear of enclosure).
  Step 3:  Disconnect battery clips. Remove old batteries.
           Dispose of per local regulations.
  Step 4:  Install new batteries. Verify polarity:
           Battery 1: positive (+) toward positive marking.
           Battery 2: positive (+) to negative (−) of Battery 1
           (series connection). Negative of Battery 2 to ground.
  Step 5:  Verify total voltage: 17–18V across the pair.
  Step 6:  Reinstall battery door.
  Step 7:  Power ON. Verify CYD battery indicator shows full.

--------------------------------------------------------------------------------

8-5. FIRMWARE UPDATE

8-5.1  PROCEDURE

  Required: Computer with Arduino IDE 2.x, USB-A to USB-Micro cable,
            ESP32 board support package, TFT_eSPI library.

  Step 1:  Connect CYD board to computer via USB.
           The ESP32 should appear as a COM port
           (Windows: Device Manager; Linux: /dev/ttyUSB0 or ttyACM0).

  Step 2:  Open noise_bridge_cyd.ino in Arduino IDE.
           Verify smith_chart.h is in the same directory.

  Step 3:  Select board: "ESP32 Dev Module" or "ESP32-2432S028".
           Select COM port.
           Set partition scheme: "Default 4MB with spiffs" or larger.

  Step 4:  Verify User_Setup.h in TFT_eSPI library matches CYD
           pinout (see Section 4-7.1).

  Step 5:  Press Upload (→). Verify compilation and upload succeed.
           Upload speed: 921600 baud (default).

  Step 6:  After upload, verify operation on serial monitor at
           115200 baud. Confirm boot message, screen drawing, and
           ADC readings are correct.

  CAUTION: Uploading new firmware erases the existing EEPROM
  calibration constants ONLY if the EEPROM address map changes.
  Verify calibration after any firmware update.

================================================================================
CHAPTER 9  PARTS LIST
================================================================================

9-1. COMPLETE PARTS LIST

See Section 4-1.1 for the complete Bill of Materials.

For reorder, the following suppliers are recommended:

  Mouser Electronics:    mouser.com (resistors, caps, ICs, switches)
  Digi-Key Electronics:  digikey.com (all component types)
  Mini-Circuits:         minicircuits.com (MAR-6SM, ERA-3SM MMICs)
  Amidon Associates:     amidoncorp.com (ferrite cores: BN-43-202,
                                         T-50-6, T-37-6)
  McMaster-Carr:         mcmaster.com (hardware, O-rings, fasteners)
  Bourns:                bourns.com (3590S cermet potentiometers)
  Analog Devices:        analog.com (AD8307 log amplifier)
  AliExpress/Amazon:     ESP32-2432S028 CYD board
  RF Parts:              rfparts.com (vacuum variable cap alternative)

================================================================================
APPENDIX A  REACTANCE TABLES
================================================================================

A-1. INDUCTIVE REACTANCE (XL) IN OHMS

  XL = 2π × f × L

  L(μH)    160M     80M     40M     30M     20M     17M     15M     10M     6M
           1.85MHz 3.75MHz 7.15MHz 10.125MHz 14.175MHz 18.118MHz 21.225MHz 28.5MHz 50MHz
  ------   ------  ------  ------  ------  ------  ------  ------  ------  ------
  1.0       11.6   23.6    44.9    63.6    89.1   113.9   133.4   179.1   314.2
  2.2       25.6   51.8    98.8   139.9   196.0   250.5   293.4   393.9   690.7
  4.7       54.7  110.6   210.9   298.7   418.7   534.9   626.8   840.1  1476
  10.0     116.2  235.6   449.5   636.1   890.6  1138.8  1334.0  1790.7  3142
  22.0     255.7  518.3   989.0  1399.4  1959.4  2505.3  2934.8  3939.5  6912

A-2. CAPACITIVE REACTANCE (XC) IN OHMS

  XC = 1 / (2π × f × C)

  C(pF)    160M    80M     40M     30M     20M     17M     15M     10M     6M
  ------   ------  ------  ------  ------  ------  ------  ------  ------  ------
  12       7196   3552    1862    1314     940     735     628     466     265
  25       3450   1702     893     630     451     353     301     224     127
  50       1725    851     446     315     225     176     151     112      64
  100       862    426     223     158     113      88      75      56      32
  200       431    213     111      79      56      44      37      28      16
  400       216    107      56      39      28      22      19      14       8
  600       144     71      37      26      19      15      13       9       5

NOTE: Reactance values in the 0–200 Ω measurement range of the
instrument are shown in normal type. Values outside the range
(>200 Ω) require appropriate capacitor or inductor bank selection
to bring into range.

================================================================================
APPENDIX B  BAND CALIBRATION DATA
================================================================================

B-1. BAND CENTER FREQUENCIES

  Band   Frequency    λ/2 (m)   50Ω Load Test R   X at null (target)
  ----   ---------    -------   ----------------   ------------------
  160M   1.850 MHz    81.1 m    50 Ω               0 Ω
  80M    3.750 MHz    40.0 m    50 Ω               0 Ω
  40M    7.150 MHz    20.9 m    50 Ω               0 Ω
  30M   10.125 MHz    14.8 m    50 Ω               0 Ω
  20M   14.175 MHz    10.6 m    50 Ω               0 Ω
  17M   18.118 MHz     8.27 m   50 Ω               0 Ω
  15M   21.225 MHz     7.06 m   50 Ω               0 Ω
  12M   24.940 MHz     6.01 m   50 Ω               0 Ω
  10M   28.500 MHz     5.26 m   50 Ω               0 Ω
  6M    50.000 MHz     3.00 m   50 Ω               0 Ω

B-2. KNOWN PARASITIC ERRORS BY FREQUENCY

The following corrections should be applied when high accuracy
is required at frequencies above 14 MHz:

  Frequency   R error   X error   Notes
  ---------   -------   -------   -----
  7 MHz        +0 Ω      +0 Ω    Calibration reference frequency
  14 MHz       +1 Ω      +2 Ω    Low parasitic error; negligible
  21 MHz       +3 Ω      +5 Ω    T1 self-resonance effects begin
  28 MHz       +6 Ω     +10 Ω   Apply correction for high accuracy
  50 MHz      +12 Ω     +20 Ω   Use correction or reduce confidence

  At 50 MHz, measurements are qualitative only. For accurate VHF
  antenna work, use a NanoVNA or VNA with proper SOLT calibration.

================================================================================
APPENDIX C  SMITH CHART REFERENCE
================================================================================

C-1. READING THE SMITH CHART

C-1.1  CHART ORIENTATION (CYD Display and Standard Charts)

          Inductive (XL > 0, upper hemisphere)
               |
  Short -------+------- Open
  (left edge)  |  Center = 50Ω  (right edge)
               |
          Capacitive (XC > 0, lower hemisphere)

NOTE: In this instrument's convention, XC (capacitive reactance)
is positive. On the Smith chart, positive X is plotted in the
LOWER hemisphere (below the real axis). This is the opposite of
most textbook conventions where XC < 0 and XL > 0.

To avoid confusion, the CYD Smith chart labels the upper half
"XL (Inductive)" and the lower half "XC (Capacitive)" directly
on the screen.

C-1.2  KEY REFERENCE POINTS

  Point              Location on Chart    Impedance
  -----              -----------------    ---------
  Perfect match      Center               50 + j0 Ω (SWR = 1.0)
  Short circuit      Left edge            0 + j0 Ω (SWR = ∞)
  Open circuit       Right edge           ∞ + j∞ Ω (SWR = ∞)
  Purely reactive    Outer circle         0 + jX Ω (any X)
  High resistance    Right of center      R > 50, X ≈ 0

C-1.3  SWR CIRCLES

  SWR = 1.5:1    ρ = 0.200    Circle radius = 0.200 × chart_r
  SWR = 2.0:1    ρ = 0.333    Circle radius = 0.333 × chart_r
  SWR = 3.0:1    ρ = 0.500    Circle radius = 0.500 × chart_r

--------------------------------------------------------------------------------

C-2. COMMON ANTENNA SIGNATURES ON THE SMITH CHART

C-2.1  RESONANT DIPOLE NEAR RESONANCE

A frequency sweep of a dipole through its resonant frequency shows
the impedance locus as a small clockwise arc near the real axis:
  - Below resonance: capacitive (lower hemisphere)
  - At resonance: on real axis (X ≈ 0)
  - Above resonance: inductive (upper hemisphere)
  The arc's crossing of the real axis identifies the resonant frequency.

C-2.2  MISMATCHED ANTENNA WITH QUARTER-WAVE FEEDLINE

A quarter-wave feedline transforms impedance to its inverse on
the normalized Smith chart. A 25Ω antenna impedance appears as
100Ω at the instrument (with a λ/4 of 50Ω coax). The point rotates
180° on the Smith chart, from left of center to right of center.

C-2.3  NON-RESONANT END-FED WIRE

A non-resonant end-fed wire exhibits very high impedance (near the
right edge of the chart, possibly cycling around the edge as frequency
changes). Use a 1:49 balun and remeasure; the impedance should move
toward center.

================================================================================
APPENDIX D  CONVERSION FORMULAS
================================================================================

  From R, X at frequency f:

    |Z|     = √(R² + X²)                          (Ω)
    θ       = arctan(X/R)                          (degrees)
    r_n     = R / 50                               (normalized)
    x_n     = X / 50                               (normalized)
    D       = (r_n + 1)² + x_n²
    Re(Γ)   = (r_n² + x_n² − 1) / D
    Im(Γ)   = 2x_n / D
    ρ       = √(Re(Γ)² + Im(Γ)²)
    SWR     = (1 + ρ) / (1 − ρ)
    RL      = −20 log10(ρ)                         (dB)

  If X > 0 (capacitive):
    C       = 1 / (2π × f × X)                    (farads)
    C_pF    = 10¹² / (2π × f × X)                 (picofarads)

  If X < 0 (inductive):
    L       = (−X) / (2π × f)                     (henries)
    L_nH    = 10⁹ × (−X) / (2π × f)              (nanohenries)

  SWR to return loss:
    RL (dB) = 20 log10[(SWR + 1) / (SWR − 1)]

  Return loss to SWR:
    SWR     = (10^(RL/20) + 1) / (10^(RL/20) − 1)

================================================================================
APPENDIX E  GLOSSARY
================================================================================

BALUN — Balanced-to-Unbalanced transformer. Used to convert between
balanced antenna terminals and unbalanced coax. Prevents common-mode
current on coax shield from affecting impedance measurements.

BRIDGE NULL — The condition in which the bridge is balanced, meaning
the voltage at the detector port is at its minimum value. Occurs when
the reference arm impedance equals the unknown impedance.

COMPLEX IMPEDANCE — A quantity Z = R + jX representing the combination
of resistance (R, real part) and reactance (X, imaginary part) of an
electrical network at a given frequency.

GAMMA (Γ) — The complex reflection coefficient, defined as
Γ = (Z − Z0) / (Z + Z0). The magnitude |Γ| = ρ ranges from 0
(perfect match) to 1 (total reflection).

NOISE FLOOR — The background noise level in the null detector in the
absence of any signal. The null depth is measured relative to the
noise floor.

NULL DEPTH — The ratio (in dB) between the detector signal off-null
(bridge unbalanced) and the detector signal at null (bridge balanced).
Higher null depth enables more accurate measurements.

POTENTIOMETER (10-TURN CERMET) — A precision multi-turn resistive
element with a metal alloy resistive track (cermet = ceramic-metal
composite). 10-turn types provide 10× the angular resolution of
single-turn pots, critical for fine R and X adjustment.

REACTANCE — The imaginary part of impedance. Capacitive reactance XC
opposes current by storing charge; inductive reactance XL opposes
current by storing magnetic flux. Both dissipate no power (ideal).

RETURN LOSS — A measure of impedance mismatch: RL = −20 log10(|Γ|) dB.
Higher RL indicates a better match (less reflected power).

SMITH CHART — A polar plot of complex reflection coefficient Γ with
overlaid constant-resistance circles and constant-reactance arcs,
allowing graphical impedance analysis. Invented by Philip H. Smith
of Bell Telephone Laboratories, 1939.

SWR — Standing Wave Ratio. The ratio of maximum to minimum voltage
(or current) on a transmission line due to impedance mismatch.
SWR = (1 + ρ) / (1 − ρ). SWR of 1.0 is a perfect match.

WHEATSTONE BRIDGE — A circuit of four impedance arms arranged to
allow comparison of an unknown impedance against known references.
Balance (null) condition: Z1/Z2 = Z3/Z4.

Z0 — Characteristic impedance of the reference system (50Ω for this
instrument). The impedance against which all measurements are
normalized.

================================================================================
END OF TECHNICAL MANUAL TM-NB-001 Rev A
RF NOISE BRIDGE ANTENNA IMPEDANCE ANALYZER
================================================================================

DISTRIBUTION: This manual is intended for use by qualified technicians
with RF electronics background. Retain this manual with the instrument.

For corrections or additions, contact the originating technical authority.

================================================================================
