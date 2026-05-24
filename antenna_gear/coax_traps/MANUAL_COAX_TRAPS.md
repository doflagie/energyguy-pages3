TECHNICAL MANUAL
COAXIAL CABLE ANTENNA TRAPS
DESIGN, CONSTRUCTION, AND ALIGNMENT
FOR AMATEUR RADIO MULTIBAND ANTENNA SYSTEMS


DOCUMENT NUMBER:   TM-ANT-TRAP-001
REVISION:          A
DATE:              2026-04-24
CLASSIFICATION:    UNCLASSIFIED


================================================================================
IMPORTANT NOTICE
================================================================================

This technical manual applies to coaxial cable antenna traps for amateur radio
use covering 160 through 20 centimeters (1.8 MHz through 1296 MHz).

Transmitter power levels associated with amateur radio can produce RF voltages
and currents sufficient to cause burns or RF interference with implanted medical
devices. Review all safety precautions in Chapter 1 before beginning work.

All electrical values, winding data, and dimensions in this manual are derived
from first-principles calculation. Verify all resonant frequencies with a vector
network analyzer (VNA) or antenna analyzer before installing in an antenna
system.

================================================================================
TABLE OF CONTENTS
================================================================================

CHAPTER 1  SAFETY AND PRECAUTIONS
CHAPTER 2  GENERAL DESCRIPTION
CHAPTER 3  THEORY OF OPERATION
CHAPTER 4  TRAP SPECIFICATIONS AND WINDING DATA
CHAPTER 5  MATERIALS AND TOOLS
CHAPTER 6  CONSTRUCTION PROCEDURES
CHAPTER 7  ALIGNMENT AND TESTING
CHAPTER 8  INSTALLATION IN ANTENNA SYSTEMS
CHAPTER 9  MAINTENANCE AND INSPECTION
CHAPTER 10 TROUBLESHOOTING
CHAPTER 11 REPAIR PROCEDURES
APPENDIX A QUICK REFERENCE WINDING TABLES
APPENDIX B NEC MODELING NOTES
APPENDIX C OPENSCAD FORMER SPECIFICATIONS
APPENDIX D CYD TRAP TESTER
APPENDIX E GLOSSARY


================================================================================
CHAPTER 1  SAFETY AND PRECAUTIONS
================================================================================

1-1.  SCOPE

1-1.1  This chapter defines safety precautions applicable to all procedures
in this manual. The technician shall read and understand all precautions before
beginning any procedure.

--------------------------------------------------------------------------------

WARNING
-------
HIGH VOLTAGE. RF voltages at transmitter output can exceed 300 VRMS at legal
amateur power levels (1500 watts, 50 ohm feed). A trap positioned within 10%
of the voltage maximum on an antenna element may develop several hundred volts
across its terminals. Do not handle traps on a live antenna system.
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
WARNING
-------
RF BURNS. RF current through skin tissue causes deep burns that heal slowly.
Short exposures above 10 milliamperes at HF frequencies are hazardous. Do not
contact antenna elements, feed lines, or traps while the transmitter is
energized, even at low power.
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
CAUTION
-------
DIELECTRIC HEATING. Certain plastics (including ABS and some PLA formulations)
absorb RF energy and heat at high power levels. All trap formers specified in
this manual use PETG or ASA material only. Do not substitute PLA, ABS, or
nylon for former material in any trap intended for use at power levels above
100 watts.
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
CAUTION
-------
SOLVENT HAZARD. Self-amalgamating tape, UV-curing adhesive, and polyurethane
weatherproofing compounds used in trap sealing may release irritating vapors.
Use in ventilated areas only.
--------------------------------------------------------------------------------

1-2.  GENERAL SAFETY PRECAUTIONS

1-2.1  De-energize. Ensure the transmitter is off and the antenna switch or
relay is in the BYPASS or DISCONNECT position before handling any antenna
component.

1-2.2  Ground. Ground the feedline at the operating position before climbing
to work on the antenna. This prevents static charge accumulation and provides
a path for any induced RF from nearby transmitters.

1-2.3  At height. Follow local regulations for working at height. All trap
assembly should be completed on the ground and the assembled antenna raised
into position. Do not perform soldering, measurement, or adjustment at height.

1-2.4  Lightning. Do not work on or near antennas during electrical storms.
Disconnect all coaxial feeds and connect antenna elements to ground through a
direct short to the station ground system whenever the antenna is not in use.

1-2.5  Chemical exposure. Some RF compounds (silver-bearing solders, flux
residues, dielectric greases) are skin and eye irritants. Use appropriate PPE.

1-2.6  Sharp edges. Cut coaxial cable ends are sharp. Wire ends after cutting.
Coil former edges may be sharp after printing; deburr before handling.


================================================================================
CHAPTER 2  GENERAL DESCRIPTION
================================================================================

2-1.  PURPOSE

2-1.1  A coaxial cable trap is a resonant circuit formed by winding a section
of coaxial cable into a coil. The winding inductance and the distributed cable
capacitance form a resonant L-C circuit without external components.

2-1.2  When installed in a dipole or vertical antenna element, traps act as
frequency-selective impedance elements that isolate portions of the antenna
at specific frequencies, making a single antenna system resonate on multiple
amateur radio bands.

2-2.  COVERAGE

2-2.1  This manual covers traps designed for the following amateur bands:

    Band     Frequency Range      Typical Trap Coax
    ------   ------------------   -----------------
    160M     1.800 - 2.000 MHz    RG-213, RG-8X
    80M      3.500 - 4.000 MHz    RG-213, RG-8X
    40M      7.000 - 7.300 MHz    RG-213, RG-8X, RG-58
    30M     10.100 - 10.150 MHz   RG-213, RG-8X, RG-58
    20M     14.000 - 14.350 MHz   RG-213, RG-8X, RG-58
    17M     18.068 - 18.168 MHz   RG-8X, RG-58
    15M     21.000 - 21.450 MHz   RG-8X, RG-58
    12M     24.890 - 24.990 MHz   RG-58
    10M     28.000 - 29.700 MHz   RG-58
    6M      50.000 - 54.000 MHz   RG-58
    2M     144.000 - 148.000 MHz  RG-58 (small)
    1.25M  222.000 - 225.000 MHz  Coaxial stubs
    70cm   420.000 - 450.000 MHz  Coaxial stubs
    33cm   902.000 - 928.000 MHz  Coaxial stubs
    23cm  1240.000 - 1300.000 MHz Coaxial stubs

NOTE: At frequencies above 144 MHz, wound coaxial traps become physically
impractical. Coaxial stub traps are used instead (see paragraph 3-7).

2-3.  TRAP TYPES

2-3.1  PARALLEL RESONANT TRAP (TYPE P). The most common trap type for
multiband dipoles. At resonance, the parallel L-C circuit presents maximum
impedance (high-Z). Current flow through the trap is blocked, effectively
isolating the antenna element beyond the trap.

2-3.2  SERIES RESONANT TRAP (TYPE S). Used in trap vertical antennas where
a resonant circuit to ground shorts out antenna current at specific frequencies.
At resonance, the series L-C circuit presents minimum impedance. Same physical
construction as Type P; circuit topology differs by wiring arrangement.

2-4.  COAX TYPES

2-4.1  RG-213/U. 50 ohm, 10.29 mm OD, 96.8 pF/m distributed capacitance,
0.66 velocity factor, solid polyethylene dielectric. Preferred for 160M through
40M traps where power handling is paramount. Can handle the full 1500W legal
limit with adequate trap Q.

2-4.2  RG-8X. 50 ohm, 6.15 mm OD, 78.7 pF/m, 0.78 VF, foamed polyethylene.
Good compromise between size and power handling for 80M through 20M. Not
recommended above 30 MHz due to dielectric heating at high power.

2-4.3  RG-58/U. 50 ohm, 4.95 mm OD, 93.5 pF/m, 0.66 VF, solid polyethylene.
Used for 40M and above traps at power levels up to 200W continuous. Adequate
for most portable and base station applications at 100W operating levels.


================================================================================
CHAPTER 3  THEORY OF OPERATION
================================================================================

3-1.  COAXIAL CABLE AS A DISTRIBUTED L-C ELEMENT

3-1.1  A section of coaxial cable wound into a coil has two simultaneous
electromagnetic properties:

    (a) INDUCTANCE: The spiral current path through the outer conductor braid
        creates a magnetic field equivalent to a single-layer air-core coil.
        The inner conductor carries equal and opposite current, but the
        contribution to net inductance is very small for tightly wound coils.

    (b) CAPACITANCE: The coaxial structure maintains its distributed
        capacitance regardless of how the cable is routed or wound. Each meter
        of coaxial cable contributes a fixed capacitance between inner and outer
        conductors, determined by the dielectric constant and geometry.

3-1.2  When these two properties occur in the same physical structure, a
resonant circuit is formed with no external components. The inductance and
capacitance are not "lumped" — they are distributed along the cable length —
but for frequencies below approximately 10% of the cable's quarter-wave
resonance, the lumped approximation is accurate within 2-5%.

3-2.  INDUCTANCE CALCULATION (WHEELER'S FORMULA)

3-2.1  For a single-layer air-core coil, H.A. Wheeler's formula gives
inductance as:

        N^2 * r^2
    L = ----------    (microhenries)
        9r + 10l

    where:
        N = number of turns
        r = coil radius in inches
        l = winding length in inches

3-2.2  This formula is accurate within 1% for coils where l/d < 1 (short coils,
which describes all coax traps). For coils where l/d > 1 use Nagaoka's formula.

3-2.3  The winding length l = N * cable_OD for close-wound (touching turns)
or l = N * (cable_OD + gap) for spaced windings. Close-winding is standard
practice for coax traps.

3-3.  CAPACITANCE CALCULATION

3-3.1  The total capacitance of the trap is the distributed capacitance of the
coax winding plus any external trimmer capacitor added for fine tuning:

    C_total = (L_cable * C_per_meter) + C_trimmer

    where:
        L_cable    = total length of coax wound on former (meters)
        C_per_meter = distributed capacitance of coax type (pF/m):
                      RG-213:  96.8 pF/m
                      RG-8X:   78.7 pF/m
                      RG-58:   93.5 pF/m
        C_trimmer  = trimmer capacitor value (pF), 0 if not used

3-3.2  The coax length for N turns on a former of diameter D:

    L_cable = N * pi * D    (meters, D in meters)

3-4.  RESONANT FREQUENCY

3-4.1  The resonant frequency of the trap is:

           1
    f = -------           (Hz)
        2*pi*sqrt(L*C)

    where L is in henries and C is in farads. Converting to practical units:

             1000
    f_MHz = -------
            2*pi*sqrt(L_uH * C_pF)

3-4.2  EXAMPLE. 40M trap, RG-213, 76mm former, 7.5 turns:

    r = 1.496 inches (76/2 mm / 25.4)
    l = 7.5 * 0.405 inches = 3.0375 inches  (RG-213 OD = 10.29 mm = 0.405")
    L = (1.496^2 * 7.5^2) / (9*1.496 + 10*3.0375)
      = (2.238 * 56.25) / (13.464 + 30.375)
      = 125.9 / 43.84
      = 2.87 uH

    Cable length = 7.5 * pi * 0.076 = 1.79 m
    C = 1.79 * 96.8 = 173 pF

    f = 1000 / (2 * pi * sqrt(2.87 * 173))
      = 1000 / (2 * pi * sqrt(496.5))
      = 1000 / (2 * pi * 22.28)
      = 1000 / 140.0
      = 7.14 MHz   (target: 7.15 MHz -- ACCEPT)

3-5.  QUALITY FACTOR (Q) AND BANDWIDTH

3-5.1  The Q factor of a resonant circuit determines the sharpness of
resonance and the efficiency (losses) of the trap. For a parallel resonant
coax trap:

    Q = R_p / (omega * L)

    where R_p is the parallel equivalent resistance (losses).

3-5.2  Typical Q values for coax traps at various frequencies:

    Band   Cable      Q (typical)   Q (good construction)
    -----  ---------  -----------   ---------------------
    160M   RG-213         80-130         150-200
    80M    RG-213        100-150         160-220
    40M    RG-213        100-130         140-180
    20M    RG-213         60-90          80-120
    10M    RG-58          40-70           60-90
    6M     RG-58          25-45           40-60

3-5.3  Q degrades at higher frequencies due to:
    (a) Skin effect in braid conductor (resistance increases as sqrt(f))
    (b) Dielectric losses in coax insulation
    (c) Proximity effect between adjacent turns at close winding

3-5.4  The -3 dB bandwidth of the trap resonance:

    BW_3dB = f0 / Q    (same units as f0)

3-5.5  For a 40M trap at 7.15 MHz with Q=109:

    BW = 7.15 MHz / 109 = 65.6 kHz

    This means the trap provides significant isolation (>3 dB) only within
    ±33 kHz of the design frequency. Outside this range, trap impedance falls
    and current flows through to the outer antenna element.

3-5.6  IMPACT OF Q ON ANTENNA EFFICIENCY. Trap losses manifest as heat in the
coax resistance and reduce antenna gain. The efficiency degradation is:

    Loss_dB ≈ 8.686 * (R_series / R_radiation)

    For typical 40M traps (R_series ~1.2 ohm, R_radiation ~70 ohm):

    Loss ≈ 8.686 * (1.2/70) = 0.15 dB per trap

    A two-trap dipole (4 traps, 2 per element) loses approximately:

    Total loss ≈ 4 * 0.15 = 0.6 dB at 40M

    This is the standard accepted penalty for trap antenna convenience.

3-6.  PARALLEL RESONANT TRAP BEHAVIOR

3-6.1  The impedance of a parallel L-C circuit at various frequencies relative
to resonance is:

    BELOW RESONANCE (f < f0): Circuit appears CAPACITIVE (C dominates)
    AT RESONANCE (f = f0):    Maximum impedance, purely resistive (Z = R_p)
    ABOVE RESONANCE (f > f0): Circuit appears INDUCTIVE (L dominates)

3-6.2  APPLICATION TO MULTIBAND DIPOLE. Consider a 40-20-10M trapped dipole
with traps located outward from the feed:

    INNER TRAPS (10M, 28.5 MHz):
    - At 10M: high-Z. Inner elements only are active. Short dipole.
    - At 20M: below resonance, trap looks capacitive. Capacitance
      combined with outer element wire allows 20M resonance at
      approximately the 20M half-wave length.
    - At 40M: far below resonance, trap looks capacitive. Both inner
      and outer elements contribute to 40M resonance. Antenna is
      slightly shorter than full-size 40M dipole due to trap loading.

    OUTER TRAPS (20M, 14.175 MHz):
    - At 20M: high-Z. Isolates outer 40M elements.
    - At 40M: below resonance, trap looks capacitive. Outer elements
      extend electrical length to 40M resonance.

3-6.3  SHORTENING FACTOR. The capacitive reactance of below-resonance traps
shortens the apparent electrical length of the antenna element. This is why
trap dipoles are physically shorter than full-size equivalent dipoles on their
lowest design frequency.

    Typical shortening: 15-25% on the lowest band of a 3-band trapped dipole.
    A 40M dipole physical length for a 40-20-10M trap antenna is typically
    18-19m (vs. 20.1m full-size).

3-7.  COAXIAL STUB TRAPS (VHF/UHF)

3-7.1  Above approximately 144 MHz, wound coaxial traps become impractical:
the inductance required is only nanohenries, and the physical dimensions
of the winding become smaller than the coax cable OD.

3-7.2  STUB TRAP PRINCIPLE. A short-circuited quarter-wave coaxial stub
presents infinite impedance at its design frequency (parallel resonance).
An open-circuited half-wave stub is the equivalent open-circuit version.

3-7.3  QUARTER-WAVE STUB CALCULATION:

    Physical length = (VF * lambda) / 4

    where:
        VF     = velocity factor of coax (0.66 RG-58, 0.78 RG-8X)
        lambda = free-space wavelength = 300/f_MHz (meters)

3-7.4  STUB LENGTHS FOR VHF/UHF BANDS (short-circuited quarter-wave):

    Band   Freq(MHz)   RG-58 QW Stub   RG-8X QW Stub
    -----  ---------   -------------   -------------
    2M       146.0        337.4 mm        399.3 mm
    1.25M    222.0        222.0 mm        262.6 mm
    70cm     440.0        112.1 mm        132.7 mm
    33cm     902.0         54.7 mm         64.7 mm
    23cm    1296.0         38.1 mm         45.0 mm

NOTE: Stub lengths must be trimmed after cutting. Start 10% long and trim
to resonance with NanoVNA. At 70 cm and above, connector length affects
stub electrical length significantly. Account for connector electrical length.


================================================================================
CHAPTER 4  TRAP SPECIFICATIONS AND WINDING DATA
================================================================================

4-1.  DESIGN PARAMETERS

4-1.1  Trap resonant frequencies are set to the center of each amateur band.
Construction tolerance and trimmer capacitor range permit adjustment to any
point within the band.

4-1.2  Former diameters are selected for practical printability (fits 200 mm
x 200 mm FDM print bed) and adequate Q.

4-1.3  Winding data was computed using the trap_calculator.py program (see
Appendix C). Recalculate if different former diameters are required.

4-2.  COMPLETE WINDING TABLE — PARALLEL RESONANT TRAPS

4-2.1  Table 4-1 gives winding data for all bands, all coax types.

TABLE 4-1. PARALLEL RESONANT COAXIAL TRAP WINDING DATA

     All former diameters are OD (outside of former tube, inside of coax winding)
     Turns: number of complete turns, close-wound
     L: computed inductance (microhenries)
     C: computed capacitance (picofarads)
     f: computed resonant frequency (MHz)
     Q: estimated Q factor
     Rp: parallel equivalent resistance at resonance (ohms)
     BW: -3 dB bandwidth (kHz)
     Len: coax length required (meters / feet)

  160M (1.850 MHz)
  ----------------
  RG-213 | Former: 120mm | 14 T | L=14.28uH | C=518pF | f=1.850MHz | Q=214 | Rp=35520 | BW=8.6kHz  | 5.35m/17.5ft
  RG-8X  | Former: 100mm | 16.5T| L=18.15uH | C=405pF | f=1.855MHz | Q=185 | Rp=39040 | BW=10.0kHz | 5.15m/16.9ft
  RG-58  | Former:  80mm | 18.5T| L=17.02uH | C=437pF | f=1.846MHz | Q=166 | Rp=32664 | BW=11.1kHz | 4.67m/15.3ft

  80M (3.750 MHz)
  ---------------
  RG-213 | Former:  90mm | 11 T | L=6.13uH  | C=296pF | f=3.738MHz | Q=151 | Rp=21682 | BW=24.8kHz | 3.05m/10.0ft
  RG-8X  | Former:  76mm | 12.5T| L=7.83uH  | C=231pF | f=3.741MHz | Q=130 | Rp=23918 | BW=28.8kHz | 2.94m/9.6ft
  RG-58  | Former:  60mm | 14 T | L=7.28uH  | C=249pF | f=3.743MHz | Q=116 | Rp=19894 | BW=32.3kHz | 2.66m/8.7ft

  40M (7.150 MHz)
  ---------------
  RG-213 | Former:  76mm |  7.5T| L=2.87uH  | C=173pF | f=7.134MHz | Q=109 | Rp=14024 | BW=65.6kHz | 1.79m/5.9ft
  RG-8X  | Former:  60mm |  9 T | L=3.59uH  | C=136pF | f=7.192MHz | Q=94  | Rp=15197 | BW=76.5kHz | 1.73m/5.7ft
  RG-58  | Former:  50mm | 10 T | L=3.37uH  | C=145pF | f=7.187MHz | Q=84  | Rp=12776 | BW=85.6kHz | 1.56m/5.1ft

  30M (10.125 MHz)
  ----------------
  RG-213 | Former:  60mm |  7.5T| L=1.85uH  | C=133pF | f=10.141MHz| Q=91  | Rp=10768 | BW=111kHz  | 1.38m/4.5ft
  RG-8X  | Former:  50mm |  8.5T| L=2.38uH  | C=105pF | f=10.069MHz| Q=79  | Rp=11915 | BW=128kHz  | 1.34m/4.4ft
  RG-58  | Former:  40mm |  9.5T| L=2.22uH  | C=113pF | f=10.069MHz| Q=71  | Rp=9932  | BW=142kHz  | 1.21m/4.0ft

  20M (14.175 MHz)
  ----------------
  RG-213 | Former:  50mm |  7 T | L=1.23uH  | C=103pF | f=14.110MHz| Q=77  | Rp=8452  | BW=184kHz  | 1.07m/3.5ft
  RG-8X  | Former:  40mm |  8 T | L=1.55uH  | C=81pF  | f=14.208MHz| Q=67  | Rp=9211  | BW=212kHz  | 1.03m/3.4ft
  RG-58  | Former:  35mm |  8.5T| L=1.46uH  | C=85pF  | f=14.252MHz| Q=60  | Rp=7796  | BW=236kHz  | 0.91m/3.0ft

  17M (18.118 MHz)
  ----------------
  RG-213 | Former:  45mm |  6 T | L=0.91uH  | C=85pF  | f=18.097MHz| Q=68  | Rp=7091  | BW=266kHz  | 0.88m/2.9ft
  RG-8X  | Former:  35mm |  8 T | L=1.15uH  | C=67pF  | f=18.055MHz| Q=59  | Rp=7725  | BW=307kHz  | 0.86m/2.8ft
  RG-58  | Former:  30mm |  8 T | L=1.08uH  | C=71pF  | f=18.090MHz| Q=53  | Rp=6517  | BW=342kHz  | 0.76m/2.5ft

  15M (21.225 MHz)
  ----------------
  RG-213 | Former:  40mm |  6 T | L=0.74uH  | C=75pF  | f=21.303MHz| Q=63  | Rp=6246  | BW=337kHz  | 0.78m/2.6ft
  RG-8X  | Former:  32mm |  7.5T| L=0.94uH  | C=59pF  | f=21.348MHz| Q=54  | Rp=6835  | BW=393kHz  | 0.75m/2.5ft
  RG-58  | Former:  25mm |  9 T | L=0.87uH  | C=65pF  | f=21.237MHz| Q=49  | Rp=5660  | BW=433kHz  | 0.69m/2.3ft

  12M (24.940 MHz)
  ----------------
  RG-213 | Former:  35mm |  6.5T| L=0.61uH  | C=68pF  | f=24.788MHz| Q=58  | Rp=5510  | BW=429kHz  | 0.70m/2.3ft
  RG-8X  | Former:  28mm |  7.5T| L=0.76uH  | C=53pF  | f=24.954MHz| Q=50  | Rp=6020  | BW=499kHz  | 0.68m/2.2ft
  RG-58  | Former:  22mm |  9 T | L=0.71uH  | C=58pF  | f=24.791MHz| Q=45  | Rp=4986  | BW=554kHz  | 0.62m/2.0ft

  10M (28.500 MHz)
  ----------------
  RG-213 | Former:  30mm |  7 T | L=0.50uH  | C=63pF  | f=28.394MHz| Q=55  | Rp=4863  | BW=518kHz  | 0.65m/2.1ft
  RG-8X  | Former:  25mm |  8 T | L=0.64uH  | C=49pF  | f=28.434MHz| Q=47  | Rp=5403  | BW=604kHz  | 0.62m/2.0ft
  RG-58  | Former:  20mm |  9 T | L=0.60uH  | C=53pF  | f=28.363MHz| Q=42  | Rp=4481  | BW=679kHz  | 0.57m/1.9ft

  6M (51.000 MHz)
  ---------------
  RG-213 | Former:  25mm |  5 T | L=0.25uH  | C=39pF  | f=51.010MHz| Q=41  | Rp=3279  | BW=1244kHz | 0.40m/1.3ft
  RG-8X  | Former:  20mm |  6 T | L=0.32uH  | C=31pF  | f=50.725MHz| Q=35  | Rp=3612  | BW=1449kHz | 0.39m/1.3ft
  RG-58  | Former:  16mm |  7 T | L=0.30uH  | C=33pF  | f=51.087MHz| Q=31  | Rp=2980  | BW=1648kHz | 0.35m/1.1ft

  2M (146.000 MHz)
  ----------------
  NOTE: Coax trap practical but marginal at this frequency.
        Use smallest available former (10-15mm).
        Wound traps at 2M: typically 2-4 turns RG-58, 10-12mm former.
        Verify resonance carefully; strays and connector effects are significant.
  RG-58  | Former:  10mm |  3 T | (calculate with trap_calculator.py)

  1.25M, 70cm, 33cm, 23cm
  ------------------------
  COAXIAL STUB TRAPS REQUIRED. See paragraph 3-7 and Table 4-2.

TABLE 4-2. COAXIAL STUB TRAP LENGTHS (VHF/UHF)

  Band    Freq(MHz)  Stub Type    RG-58 Length  RG-8X Length
  ------  ---------  ----------   -----------   -----------
  2M       146.000   QW short     337.4 mm      399.3 mm
  1.25M    222.000   QW short     222.0 mm      262.6 mm
  70cm     440.000   QW short     112.1 mm      132.7 mm
  33cm     902.000   QW short      54.7 mm       64.7 mm
  23cm    1296.000   QW short      38.1 mm       45.0 mm

  QW short = quarter-wave short-circuit stub (choke trap)
  All lengths include connector body length. Trim to resonance.
  Connector electrical length (PL-259): approx 8-12mm equivalent
  Connector electrical length (N-type): approx 5-8mm equivalent
  Connector electrical length (SMA):    approx 3-5mm equivalent

4-3.  POWER HANDLING

4-3.1  Maximum continuous carrier power at key life locations:

    Coax Type   1 MHz   7 MHz   14 MHz   28 MHz   50 MHz   Note
    ---------   -----   -----   ------   ------   ------   ----
    RG-213      1500W   1200W    900W     600W     350W    Full legal limit
    RG-8X        750W    600W    450W     300W     180W    100% duty OK
    RG-58        300W    200W    150W     100W      60W    Reduce for RTTY

NOTE: Trap coils run hotter than straight coax at the same power level.
Power handling figures above assume trap coils are in free air.
Derate by 30% for traps enclosed in weatherproof housings without ventilation.

4-4.  TRIMMER CAPACITOR SPECIFICATIONS

4-4.1  An optional trimmer capacitor connected in parallel with the trap
coil permits fine-tuning of the resonant frequency.

    Recommended trimmer range: 5-30 pF
    Voltage rating minimum: 500V (HF traps), 250V (VHF traps)
    Type: piston trimmer (NP0) or air variable
    Preferred: Sprague-Goodman GJM, Murata TZB4, or Voltronics equivalent
    Avoid: ceramic disc trimmers (microphonic, poor weather resistance)

4-4.2  Trimmer selection by band:

    Band     Trimmer Range   Frequency Effect per pF
    -----    ------------    -----------------------
    160M     15-30 pF        ~12 kHz/pF
    80M      10-25 pF        ~25 kHz/pF
    40M      5-20 pF         ~60 kHz/pF
    20M      5-15 pF        ~125 kHz/pF
    10M      3-10 pF        ~280 kHz/pF
    6M       2-8 pF         ~600 kHz/pF


================================================================================
CHAPTER 5  MATERIALS AND TOOLS
================================================================================

5-1.  MATERIALS LIST

5-1.1  COAXIAL CABLE. Purchase coax in bulk spools. Required lengths per trap
are given in Table 4-1. Purchase 20% extra for errors.

5-1.2  TRAP FORMERS. Print from designs in the openscad/ directory.
    Material: PETG or ASA
    Color: Black (UV stability)
    Temperature: PETG 235C/85C bed, ASA 255C/105C bed (enclosure required)

5-1.3  WEATHERPROOF ENCLOSURE HARDWARE.
    Qty  Description
     4   M4 x 20mm stainless steel hex bolts (per trap)
     4   M4 stainless nuts
     4   M4 stainless flat washers
     1   Roll self-amalgamating tape, 25mm wide, 1-2m per trap
     1   Bottle UV-curing adhesive or Loctite 5083 weatherseal
     4   Stainless steel hose clamps (100-120mm) for mast mounting

5-1.4  ELECTRICAL CONNECTIONS.
    - PL-259 / SO-239 connectors: NOT used on trap coil itself.
      Trap coax pigtails are soldered directly to antenna element wire.
    - Solder: 60/40 tin-lead, non-acid flux. 63/37 preferred for joint quality.
    - Antenna element wire: #14 AWG hard-drawn copper or aluminium.
    - Heat shrink tubing: 13mm (before shrink) for covering solder joints.

5-2.  TOOLS REQUIRED

    Soldering station, 50W minimum
    Wire strippers, coaxial
    Cable knife for stripping coax jacket
    Caliper (digital preferred, 0-150mm range)
    NanoVNA or other vector network analyzer (mandatory for alignment)
    Frequency counter (optional, useful for initial trap testing)
    Drill with 4.5mm and 5mm bits
    Deburring tool
    FDM 3D printer (200mm x 200mm bed minimum, PETG/ASA capable)
    Heat gun or lighter for heat shrink
    Zip ties, stainless 200mm (for anchoring trap on former during cure)


================================================================================
CHAPTER 6  CONSTRUCTION PROCEDURES
================================================================================

6-1.  PRINT THE FORMER

    STEP 1. Open trap_former.scad in OpenSCAD.
    STEP 2. Set former_od to the value from Table 4-1 for the desired band
            and coax type.
    STEP 3. Set cable_od to: RG-213=10.29, RG-8X=6.15, RG-58=4.95
    STEP 4. Set turns to the value from Table 4-1.
    STEP 5. Check console output: "Fits 200x200 bed: YES/SPLIT REQUIRED"
    STEP 6. If "SPLIT REQUIRED", enable split_former=true and print two halves.
    STEP 7. Export STL and slice with 0.2mm layer height, 30% infill, 3 walls.
    STEP 8. Print in PETG or ASA. Inspect for layer adhesion before proceeding.
    STEP 9. Deburr all edges. The groove helix must be smooth and continuous.

    NOTE: For 160M and 80M traps with RG-213, the former is large (120-90mm
    diameter). These may require splitting depending on printer bed clearance
    to gantry. Check total height (total_length in OpenSCAD console) against
    available Z height.

6-2.  PREPARE THE COAXIAL CABLE

    STEP 1. Cut coax to the length given in Table 4-1, plus 150mm extra at
            each end for pigtail connections.
    STEP 2. At one end, strip 50mm of outer jacket.
    STEP 3. Push back braid 20mm to expose inner dielectric.
    STEP 4. Do NOT strip the inner conductor at this time.
    STEP 5. Tin the braid tail lightly with solder. Do not use excess solder.
    STEP 6. Repeat for the other end.

    CAUTION: When stripping coax for trap winding, ensure the braid is NOT
    cut or opened along the wound section. A braid breach in the winding
    will alter the trap capacitance and prevent resonance at the design
    frequency.

6-3.  WIND THE COIL

    STEP 1. Route the first coax end through the cable port in the bottom
            flange. Leave 120mm pigtail below the flange.

    STEP 2. Begin winding the coax into the first groove on the former.
            Wind in a consistent direction (clockwise when viewed from top).

    STEP 3. Wind closely and firmly. Each turn should seat in the groove
            and contact adjacent turns. Do not allow gaps between turns.

    STEP 4. After completing the specified number of turns, route the coax
            end through the port in the top flange. Leave 120mm pigtail.

    STEP 5. Secure the winding at each end with 200mm stainless zip ties
            through the holes in the flanges. Tighten firmly.

    NOTE: For half-turn increments (e.g. "7.5 turns"), route the end coax
    through the flange at the 180-degree point (halfway around) rather than
    at the same angular position as the start.

    NOTE: Do not substitute electrical tape for zip ties. Tape loosens over
    time and allows the winding to unwind slightly, shifting resonant frequency.

6-4.  SOLDER THE CONNECTIONS

6-4.1  Parallel resonant trap (Type P) connection:

    The inner conductor of the coax at both ends is connected to the center
    of the antenna element wire passing through the trap. The outer braid
    at both ends connects to the SAME conductor (inner or outer, per
    antenna design). See Figure 6-1 (dipole connection).

    DIPOLE TRAP WIRING (inner element side = feed side):

         Element wire (feed side) ----+-----+---- Element wire (outer side)
                                      |     |
                            Coax inner|     |Coax inner
                            End A     |     | End B
                                      [Trap Coil]
                            Coax braid|     |Coax braid
                            End A ----+-----+---- End B

    Both inner conductors connect to element wire on respective sides.
    Both braid ends connect together (and may optionally connect to element
    wire through the trap -- see note below).

    NOTE: In some commercial trap designs, the braid is NOT connected to
    the element wire; the trap acts as a pure series insert. In others,
    the braid is connected to the element wire, creating a parallel
    connection. Both configurations are valid; performance is nearly
    identical. This manual recommends connecting braid to element wire
    on both sides for mechanical security and reduced voltage stress on
    the coax pigtails.

6-4.2  SOLDER PROCEDURE:

    STEP 1. Cut element wire to length (leave 30mm extra for trimming).
    STEP 2. Strip 15mm of insulation from element wire at trap connection.
    STEP 3. Pre-tin element wire end.
    STEP 4. Pre-tin coax pigtail inner conductor (after stripping back 15mm).
    STEP 5. Twist element wire and coax inner conductor together.
    STEP 6. Apply heat and solder to create a full mechanical and electrical
            joint. Do not leave a cold joint.
    STEP 7. Allow to cool without movement (15 seconds minimum).
    STEP 8. Repeat for outer braid connection to element wire.
    STEP 9. Cover with 40mm heat shrink. Heat uniformly from center outward.
    STEP 10.Cover joint and heat shrink with self-amalgamating tape, minimum
             2 layers, extending 50mm past the shrink on each side.

6-5.  FINAL ASSEMBLY

    STEP 1. Install completed coil assembly in lower enclosure half.
    STEP 2. Route cable pigtails through strain relief ports.
    STEP 3. Verify pigtails protrude at least 80mm from enclosure ends.
    STEP 4. Seat upper enclosure half. Verify no cable is pinched.
    STEP 5. Install M4 bolts. Tighten to finger-tight plus 1/4 turn.
            Do not over-tighten; ASA/PETG will crack under excessive torque.
    STEP 6. Apply bead of Loctite 5083 or similar to clamshell joint.
    STEP 7. Wrap entire enclosure with 2 layers self-amalgamating tape
            over and around cable entry points.
    STEP 8. Verify drain holes are clear and oriented downward in final
            antenna installation.


================================================================================
CHAPTER 7  ALIGNMENT AND TESTING
================================================================================

7-1.  EQUIPMENT REQUIRED

    7-1.1  NanoVNA v2 (4GHz), NanoVNA-H, or equivalent vector network analyzer
           with S11 measurement capability (return loss / SWR mode).
    7-1.2  Coupling loop (3 turns RG-58, 25mm diameter) for non-contact testing.
    7-1.3  Optional: CYD Trap Tester (see Appendix D).

7-2.  PRE-ALIGNMENT CHECK

    STEP 1. Before connecting the VNA, visually inspect the completed trap:
            (a) Winding is fully seated and consistent
            (b) No gaps or crossovers in the coil
            (c) Cable jacket is not kinked or compressed
            (d) Solder joints are clean and fully flowed
            (e) All zip ties are tight

    STEP 2. With a capacitance meter set to the 200-pF range, measure the
            capacitance between inner and outer conductors of one pigtail.
            Expected value is within 10% of the C value in Table 4-1.
            A value significantly different indicates a coax measurement error
            or unintended braid break.

7-3.  VNA MEASUREMENT PROCEDURE

    STEP 1. Calibrate VNA with SOLT (Short-Open-Load-Through) at the
            measurement port. Use the calibration standards provided with
            the VNA or a calibration kit appropriate for the test frequency.

    STEP 2. Connect a coupling loop to port 1 of the VNA. Do NOT connect
            the trap directly to the VNA — the coil must be tested in its
            operating environment (as a resonant circuit in free space).

    STEP 3. Place the coupling loop 5-10mm from the trap winding. Maintain
            consistent spacing throughout the measurement.

    STEP 4. Set VNA sweep range to ±20% of the design frequency.
            Example for 40M trap (7.15 MHz): sweep 5.7 - 8.6 MHz.

    STEP 5. Enable S11 log magnitude display.

    STEP 6. Observe the resonance dip. The trap resonance appears as a
            sharp dip (minimum) in S11 (or peak in return loss) at the
            resonant frequency.

    STEP 7. Record:
            (a) Measured resonant frequency (f_measured)
            (b) Depth of dip (dB below off-resonance level)
            (c) Marker bandwidth at -3 dB from dip

    STEP 8. Calculate Q:

            Q = f_measured / BW_3dB

    STEP 9. Compare with Table 4-1 values. Acceptable tolerances:

            Frequency: ±2% of design value
            Q:         ≥70% of estimated value in Table 4-1

7-4.  TRIMMING PROCEDURES

7-4.1  FREQUENCY TOO HIGH (f_measured > f_target * 1.02):

    Option A: Add a trimmer capacitor in parallel with the trap.
              Mount on the access port in the former. Start at maximum
              capacitance and reduce while monitoring VNA.

    Option B: Add one half-turn to the winding (increase N by 0.5).
              Unwind enough cable from the end to add the additional half-turn.
              Re-secure with zip tie.

7-4.2  FREQUENCY TOO LOW (f_measured < f_target * 0.98):

    Option A: Remove a half-turn from the winding. Unwind one pass from
              the end of the coil. Re-secure.

    Option B: Reduce trimmer capacitor value.

    CAUTION: Removing turns reduces total coax length, slightly decreasing
    available pigtail length. Verify adequate pigtail length remains after
    any turn count change.

7-4.3  Q TOO LOW (Q < 0.70 * estimated):

    (a) Inspect braid for break or poor continuity. Measure DC resistance
        between braid at both ends: should be <0.5 ohm.
    (b) Check for turn-to-turn shorts (inner conductor touching braid
        at a tight bend). Inspect winding visually.
    (c) Verify former material is PETG or ASA, not ABS or PLA.
    (d) Verify no metallic foreign objects near winding (clamps, screws).
    (e) For 10M and 6M traps: coiling causes proximity effect losses.
        Space turns by one cable OD if Q is inadequate. Recalculate
        resonant frequency (will shift high) and re-trim.

7-5.  INSTALLED ANTENNA TESTING

7-5.1  After traps are installed in the antenna:

    STEP 1. Connect NanoVNA between feedline and antenna (or use
            antenna analyzer at the feedpoint).
    STEP 2. Sweep each band. For a trapped dipole, observe SWR minimum
            on each design band.
    STEP 3. Acceptable SWR at design frequency: <2.0:1 without a tuner,
            <1.5:1 preferred.
    STEP 4. If resonant frequency is off, adjust the corresponding element
            length (see NEC model notes for which element controls which band).

7-5.2  PATTERN AND GAIN VERIFICATION.

    The NEC models (nec_models/ directory) provide predicted patterns and
    gain values. Field verification is practical only with calibrated
    test equipment. For routine installation, SWR verification on all bands
    is sufficient evidence of correct installation.


================================================================================
CHAPTER 8  INSTALLATION IN ANTENNA SYSTEMS
================================================================================

8-1.  TRAPPED DIPOLE

8-1.1  STANDARD 40-20-10M DIPOLE.

    Element half-lengths (one side from center):
        Feed to 10M trap:  2.64m (inner element, 10M resonant)
        10M trap to 20M trap: 2.62m
        20M trap to tip:    4.79m
        TOTAL half-length: 10.05m

    Feedline: 50 ohm coax, RG-213 or LMR-400 for permanent installations.
    Feed impedance: approximately 65-75 ohm on 40M, 50-65 ohm on 20M,
                    40-60 ohm on 10M. SWR below 2:1 on all bands without tuner.

    BALUN: Install a 1:1 current balun (choke balun) at the feedpoint.
           Minimum: 10 turns RG-213 on FT-240-43 toroid core.
           This prevents common-mode current on feedline and pattern distortion.

8-1.2  STANDARD 5-BAND DIPOLE (80-40-20-15-10M).

    Element half-lengths (one side from center):
        Feed to 10M trap:  2.64m
        10M trap to 15M trap: 0.90m
        15M trap to 20M trap: 1.72m
        20M trap to 40M trap: 4.79m
        40M trap to tip:    10.05m
        TOTAL half-length: 20.10m

    Height: Minimum 10m AGL for HF performance. 15-18m preferred.
    Note: 80M performance is marginal on this antenna; efficiency
          is reduced approximately 3-5 dBd due to antenna shortening.
          Dedicated 80M elements with 80M/40M trapped dipole are superior.

8-1.3  TRAP PLACEMENT CONSIDERATIONS.

    (a) Traps must be in the antenna element, not in the feedline.
    (b) Trap bodies must not contact mast, support structure, or
        other conductive objects. Minimum clearance: 50mm.
    (c) Orient trap drain holes downward. Traps that collect water
        develop dielectric losses and shift resonant frequency.
    (d) Secure antenna element to trap flanges with 3mm stainless
        wire wrapped twice through flange holes and twisted tight.
        Do not rely on solder joints alone to support mechanical load.

8-2.  TRAP VERTICAL

8-2.1  TRAP VERTICAL INSTALLATION.

    (a) Mount the vertical element on a non-conductive support or
        directly on a metal mast through a base insulator.
    (b) All three base-of-antenna requirements apply:
        - Install minimum 32 radials, at least 8m long, on or above ground.
        - Ground rod at base: 1.8m copper-clad steel minimum.
        - Bond all radials to ground rod and to antenna ground point.
    (c) Trap bodies add weight at the balance point. Use a tapered
        element (aluminum tubing, large diameter at base) if possible.

8-3.  MAST MOUNTING

8-3.1  Traps may be mounted on a fiberglass or wooden support mast.

    (a) Use the saddle slot in the enclosure top with a 100mm stainless
        hose clamp. Tighten until the enclosure does not rotate.
    (b) Do not mount traps directly on conductive mast without insulator.
        A conductive mast within 50mm of the trap winding will detune it.
    (c) For portable installations, mast-mount using UV-resistant nylon
        cable ties through flange mounting holes.


================================================================================
CHAPTER 9  MAINTENANCE AND INSPECTION
================================================================================

9-1.  INSPECTION INTERVALS

    Annual: Full visual inspection and VNA measurement
    After severe weather: Visual inspection
    After lightning event: Full electrical check and VNA measurement

9-2.  ANNUAL INSPECTION PROCEDURE

    STEP 1. Visually inspect enclosure for cracking, UV degradation,
            or mechanical damage.
    STEP 2. Inspect cable pigtails for jacket cracking or corrosion at
            solder connections.
    STEP 3. Inspect antenna element-to-trap connections for corrosion.
            Clean with fine-grit emery cloth if oxidized. Re-solder if
            connection is mechanically loose.
    STEP 4. Measure resonant frequency of each trap with VNA or CYD tester.
            Compare with Table 4-1 values. Accept if within ±3%.
    STEP 5. Measure Q of each trap. Accept if Q ≥ 60% of Table 4-1 value.
            Replace trap if Q has degraded significantly from original value.
    STEP 6. Inspect drain holes. Clear with 3mm drill if plugged.
    STEP 7. Re-wrap cable entry points with fresh self-amalgamating tape
            if existing tape shows cracking.

9-3.  EXPECTED SERVICE LIFE

    Properly installed coax traps have an expected service life of 10-15 years
    before Q degradation requires replacement. Factors that reduce service life:

    (a) Continuous operation above rated power levels
    (b) Trapped moisture (degraded dielectric constant of coax)
    (c) UV degradation of outer jacket
    (d) Physical damage from ice loading or mechanical stress
    (e) Repeated thermal cycling (high-power operation, daily temperature swings)


================================================================================
CHAPTER 10  TROUBLESHOOTING
================================================================================

TABLE 10-1. COAXIAL TRAP TROUBLESHOOTING

  SYMPTOM                        PROBABLE CAUSE              CORRECTIVE ACTION
  ------------------------------ -------------------------   ----------------------
  Trap does not resonate          Braid or conductor open     Check DC resistance
  (no dip seen on VNA)            Former not PETG/ASA         Reprint with correct
                                  Inner conductor shorted     material
                                  to braid inside winding     Inspect winding

  Resonant frequency 5-20% high  Too few turns wound         Re-count turns vs.
                                  Turn gap too large          Table 4-1. Wind tighter.
                                  Wrong former diameter       Verify with caliper.
                                  Capacitance lower than      Measure coax cap with
                                  expected                    LCR meter.

  Resonant frequency 5-20% low   Too many turns              Reduce one half-turn.
                                  Former diameter too large   Verify former OD.
                                  Braid shorted turn-to-turn  Inspect for bridge solder

  Q lower than expected           Wrong coax type             Verify coax marking.
                                  Proximity effect            Space turns 1x OD.
                                  Metallic objects near coil  Clear 50mm around coil.
                                  PL-259 connectors on coil   Never connect PL-259 to
                                  pigtails under load         trap pigtails under power.

  SWR high on one band, OK others Trap on that band off-freq  Re-align trap.
                                  Element length incorrect    Trim element wire.

  SWR high all bands              Feedline common mode        Install choke balun.
                                  Feedpoint corrosion         Clean and re-solder.
                                  Open in antenna element     Check continuity.

  Trap runs hot during operation  Over-power condition        Reduce power. Use
                                  Wrong coax for power level  RG-213 for high power.
                                  Trapped moisture in coax    Replace coax section.

  Resonant frequency drifts       Loose winding               Re-secure zip ties.
  (changes seasonally)            Temperature coefficient     Normal; ±1% acceptable.
                                  Moisture intrusion          Improve weathersealing.


================================================================================
CHAPTER 11  REPAIR PROCEDURES
================================================================================

11-1.  COAXIAL CABLE REPLACEMENT

11-1.1  If coax cable in a trap has a braid break, dielectric failure, or
significant degradation, replace the coax entirely. Removing individual turns
from a damaged winding is not practical.

    STEP 1. Remove trap from antenna installation.
    STEP 2. Remove bolts from enclosure. Separate halves.
    STEP 3. Cut all zip ties. Unwind coax from former.
    STEP 4. Cut coax pigtails at solder joints.
    STEP 5. Inspect former for damage. Clean groove surfaces.
    STEP 6. Re-wind with new coax per Chapter 6 procedures.
    STEP 7. Verify resonant frequency before re-installation.

11-2.  FORMER REPLACEMENT

11-2.1  If the former is cracked or deformed:

    STEP 1. Print replacement former (see Chapter 6, Step 1-9).
    STEP 2. Transfer coax winding to replacement former. This requires
            temporary coiling of the coax between formers; do not un-wind
            completely as maintaining coil "memory" aids re-winding.
    STEP 3. Verify resonant frequency after re-winding.

11-3.  ENCLOSURE REPLACEMENT

11-3.1  Print replacement enclosure halves from trap_enclosure.scad.
The enclosure is field-replaceable without re-winding the trap coil.


================================================================================
APPENDIX A  QUICK REFERENCE WINDING TABLES
================================================================================

A-1.  PARALLEL RESONANT TRAPS — SHORT FORM

  Note: For full specifications including Q, Rp, BW, see Table 4-1.
        Values for RG-213 unless noted.

  Band   Target    Former   Turns   Coax Len   Former
         MHz       mm               meters     OD
  -----  -------   ------   -----   --------   ------
  160M   1.850     120      14      5.35       120mm
  80M    3.750      90      11      3.05        90mm
  40M    7.150      76       7.5    1.79        76mm
  30M   10.125      60       7.5    1.38        60mm
  20M   14.175      50       7      1.07        50mm
  17M   18.118      45       6      0.88        45mm
  15M   21.225      40       6      0.78        40mm
  12M   24.940      35       6.5    0.70        35mm
  10M   28.500      30       7      0.65        30mm
  6M    51.000      25       5      0.40        25mm

A-2.  MULTI-BAND DIPOLE ELEMENT DIMENSIONS

  Configuration     Half-element Length (one side from center)
  ---------------   ------------------------------------------
  3-BAND (40/20/10) Feed -> 2.64m -> [10M trap] -> 2.62m -> [20M trap] -> 4.79m
  5-BAND (80-10)    Feed -> 2.64m -> [10M] -> 0.90m -> [15M] -> 1.72m -> [20M] ->
                    4.79m -> [40M] -> 10.05m

A-3.  COAX STUB TRAP LENGTHS (SHORT-CIRCUIT QW)

  Band     f (MHz)   RG-58 (mm)
  ------   -------   ----------
  2M        146.0       337
  1.25M     222.0       222
  70cm      440.0       112
  33cm      902.0        55
  23cm     1296.0        38


================================================================================
APPENDIX B  NEC MODELING NOTES
================================================================================

B-1.  FILES PROVIDED

  nec_models/trapped_dipole_40_20_10.nec     — 3-band dipole
  nec_models/trapped_dipole_80_40_20_15_10.nec — 5-band dipole
  nec_models/trap_vertical_40_20_10.nec      — 3-band vertical with radials
  nec_models/reference_dipole_untrapped.nec  — reference dipoles for comparison

B-2.  SOFTWARE

    Recommended: EZNEC (Windows), xnec2c (Linux/Mac), 4nec2 (Windows)
    Format: NEC-2 compatible
    For best accuracy with traps: EZNEC Pro/4 with NEC-4.2 kernel

B-3.  TRAP MODELING LIMITATIONS

B-3.1  Traps are modeled as lumped LD loads at the trap insertion points.
The LD card specifies series R, L, C values. These are the series EQUIVALENT
of the parallel RLC trap at or near resonance.

B-3.2  At non-design frequencies, the series equivalent changes with frequency.
The NEC model files use fixed L,C values (frequency-independent approximation).
For multi-frequency comparative analysis, use EZNEC's built-in trap model
which accounts for frequency-dependent impedance.

B-3.3  Trap body length (100-200mm) is not modeled. The physical trap
occupies 100-200mm of element length. The NEC model treats the trap as a
point element at the insertion position. This introduces error of approximately
±5% in resonant frequency prediction.

B-4.  RUNNING THE MODELS

    (1) Ensure proper ground model (GE card). The models include real earth
        (medium ground, ep=13, sigma=0.005 S/m).
    (2) Run separate frequency sweeps for each design band.
    (3) For the 5-band model, sweep each band separately (10 bands, 5 sweeps).
    (4) Compare gain and pattern vs. the reference dipole to quantify trap loss.


================================================================================
APPENDIX C  OPENSCAD FORMER SPECIFICATIONS
================================================================================

C-1.  FILES PROVIDED

  openscad/trap_former.scad    — Single-band winding former with flanges
  openscad/trap_enclosure.scad — Two-piece weatherproof enclosure
  openscad/trap_assembly.scad  — Visual assembly preview (not for printing)

C-2.  KEY PARAMETERS

  trap_former.scad:
    former_od     — Set from Table 4-1 (former OD column)
    cable_od      — Set to coax OD: RG-213=10.29, RG-8X=6.15, RG-58=4.95
    turns         — Set from Table 4-1 (Turns column)
    split_former  — Set true if former > ~175mm diameter (160M/80M RG-213)

  trap_enclosure.scad:
    trap_od       — Set to former_od + 2*former_wall + cable_od + 3 (clearance)
    trap_length   — Set to total_length from trap_former.scad console output
    cable_od      — Match coax type

C-3.  SLICING PARAMETERS

  Profile:         0.20mm layer height
  Infill:          30% (former), 40% (enclosure)
  Pattern:         Gyroid infill
  Walls:           3 perimeters (former), 4 perimeters (enclosure)
  Top/bottom:      4 layers
  Supports:        None required
  Brim:            5mm brim for ASA enclosures (adhesion)
  Material:        PETG or ASA. Do NOT use PLA or ABS.


================================================================================
APPENDIX D  CYD TRAP TESTER
================================================================================

D-1.  DESCRIPTION

The CYD Trap Tester is an Arduino sketch for the ESP32-2432S028 (CYD) module.
It displays measured trap resonant frequency, Q factor, and PASS/FAIL status
against table target values for all amateur bands 160M through 2M.

D-2.  HARDWARE REQUIRED

    ESP32-2432S028 (CYD) — 2.8" ILI9341 TFT with resistive touch
    Si5351A breakout module — signal source, I2C address 0x60
    Coupling loop — 3 turns RG-58, 25mm ID, with low-pass filter

D-3.  CONNECTIONS

    Si5351A SDA: GPIO21
    Si5351A SCL: GPIO22
    ADC input:   GPIO39 through 100 kohm resistor to GND + 100pF to GND

D-4.  USAGE

    1. Press UP/DN to select target band.
    2. Hold coupling loop near trap winding (5-10mm clearance).
    3. Press SCAN. Display shows frequency, Q, and PASS/FAIL.
    4. Adjust trap (trimmer, turns) and re-scan until PASS.

D-5.  ACCURACY

    Frequency: ±0.5% (limited by Si5351A crystal accuracy)
    Q:         ±20% (limited by 12-bit ADC dynamic range)
    Range:     500 kHz to 160 MHz

D-6.  FILE LOCATION

    cyd/trap_tester_cyd.ino


================================================================================
APPENDIX E  GLOSSARY
================================================================================

BALUN
    Balanced-to-unbalanced transformer. Placed at antenna feedpoint to
    prevent common-mode current on coaxial feedline outer shield.

BANDWIDTH (-3 dB)
    Frequency range over which a resonant circuit's response is within
    3 dB of maximum. For a trap, indicates the range of effective isolation.
    BW = f0 / Q.

CHOKE TRAP
    See PARALLEL RESONANT TRAP.

COAXIAL STUB
    A section of coaxial cable, one end shorted or open, used as a
    resonant or reactive element. Quarter-wave short stub = high impedance
    at design frequency.

CLOSE-WOUND
    Winding style where adjacent turns touch without spacing. Standard for
    coax traps. Spacing increases Q slightly but shifts resonant frequency.

DISTRIBUTED CAPACITANCE
    Capacitance that exists along the full length of a coaxial cable between
    inner conductor and outer braid, as opposed to a lumped capacitor.

GDO
    Grid Dip Oscillator (or Gate Dip Oscillator). Instrument that detects
    resonance of a nearby circuit by measuring power absorbed from a sweep
    oscillator. The CYD Trap Tester implements GDO principles.

NEC
    Numerical Electromagnetics Code. Software for computing antenna patterns
    and impedances by the method of moments. NEC-2 is public domain.

PARALLEL RESONANT TRAP
    L-C circuit where inductor and capacitor are in parallel. At resonance,
    presents maximum (high) impedance. Used as current choke in antenna elements.

PROXIMITY EFFECT
    Increased conductor resistance caused by interaction of magnetic fields
    between adjacent conductors. In close-wound coils, proximity effect
    reduces Q at higher frequencies.

Q FACTOR
    Quality factor of a resonant circuit. Q = f0 / BW. Higher Q = sharper
    resonance, lower losses, narrower bandwidth.

RG-58
    50 ohm coaxial cable, 4.95mm OD, 93.5 pF/m. Standard lightweight HF coax.

RG-8X
    50 ohm coaxial cable, 6.15mm OD, 78.7 pF/m, foam dielectric (0.78 VF).
    Lower loss than RG-58 for same OD.

RG-213
    50 ohm coaxial cable, 10.29mm OD, 96.8 pF/m. Low-loss HF cable, full
    legal power rating.

SERIES RESONANT TRAP
    L-C circuit where inductor and capacitor are in series. At resonance,
    presents minimum (low) impedance. Used as short circuit to ground in
    trap vertical antennas.

SOLT
    Short-Open-Load-Through. Standard VNA calibration procedure using four
    known reference standards.

VELOCITY FACTOR (VF)
    Ratio of signal propagation speed in a cable to speed of light in vacuum.
    Affects electrical length calculations for stub traps.

WHEELER'S FORMULA
    Mathematical expression for inductance of a short single-layer solenoid.
    L = r^2 * N^2 / (9r + 10l), result in microhenries (inch units).

WINDING TABLE
    Tabulation of coil winding parameters (turns, former diameter, coax type,
    resulting L, C, resonant frequency) for each design. See Table 4-1 and
    trap_winding_tables.ods.


================================================================================
END OF TECHNICAL MANUAL TM-ANT-TRAP-001 REV A
================================================================================
