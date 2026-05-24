TECHNICAL MANUAL
RF DUMMY LOADS AND POWER TERMINATIONS
DESIGN, CONSTRUCTION, TESTING, AND MAINTENANCE
FOR AMATEUR RADIO STATIONS


DOCUMENT NUMBER:   TM-DL-001
REVISION:          A
DATE:              2026-04-24
CLASSIFICATION:    UNCLASSIFIED


================================================================================
IMPORTANT NOTICE
================================================================================

This manual covers dummy loads rated from 100 watts to 1 kilowatt. Voltages
and currents associated with these power levels are hazardous. The legal
amateur radio power limit of 1500 watts produces 274 Vrms in a 50-ohm load.
This voltage will cause cardiac arrest on contact.

Read all safety precautions in Chapter 1 completely before beginning any
construction or testing procedure.

================================================================================
TABLE OF CONTENTS
================================================================================

CHAPTER 1   SAFETY AND PRECAUTIONS
CHAPTER 2   GENERAL DESCRIPTION
CHAPTER 3   THEORY OF OPERATION
CHAPTER 4   100W DRY (AIR-COOLED) DUMMY LOAD
CHAPTER 5   500W OIL-COOLED (CANTENNA) DUMMY LOAD
CHAPTER 6   1kW OIL-COOLED DUMMY LOAD
CHAPTER 7   PRECISION VNA CALIBRATION LOADS
CHAPTER 8   DIRECTIONAL COUPLER SUBSYSTEM
CHAPTER 9   MATERIALS AND PROCUREMENT
CHAPTER 10  CONSTRUCTION PROCEDURES
CHAPTER 11  TESTING AND VERIFICATION
CHAPTER 12  MAINTENANCE AND INSPECTION
CHAPTER 13  TROUBLESHOOTING
CHAPTER 14  REPAIR PROCEDURES
APPENDIX A  POWER DERATING CURVES
APPENDIX B  DUTY CYCLE CALCULATIONS
APPENDIX C  SWR SPECIFICATIONS BY BAND
APPENDIX D  RESISTOR SELECTION GUIDE
APPENDIX E  OIL SPECIFICATIONS
APPENDIX F  CYD POWER MONITOR
APPENDIX G  GLOSSARY


================================================================================
CHAPTER 1  SAFETY AND PRECAUTIONS
================================================================================

1-1.  SCOPE

1-1.1  This chapter defines mandatory safety precautions for all work
described in this manual. Non-compliance can result in death, serious injury,
fire, or equipment damage.

--------------------------------------------------------------------------------
WARNING
-------
LETHAL VOLTAGE. At legal amateur power limits (1500W into 50 ohms), the RMS
voltage is 274 volts; peak voltage is 387 volts. This voltage is lethal.
Cardiac arrest can result from brief contact at these levels.
NEVER connect a dummy load to a transmitter and then touch any internal
component. De-energize the transmitter and wait 30 seconds before opening
any load enclosure. RF energy dissipates through the resistors and oil;
the external connector and housing may remain at mains voltage potential
if a transmitter power supply fault exists.
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
WARNING
-------
FIRE HAZARD. Mineral oil is combustible at elevated temperatures.
Flash point of naphthenic mineral oil: approximately 130-145 degrees C.
Oil in a sealed container under operating conditions may develop pressure.
NEVER operate an oil-cooled load without a functioning pressure relief.
NEVER seal an oil-cooled load with an air-tight cover.
NEVER operate an oil-cooled load in areas with open flames or sparks.
If oil is seen smoking (gray or white vapors), shut down immediately.
Spilled mineral oil is a slip hazard and fire accelerant.
Keep an ABC fire extinguisher within arm's reach during high-power testing.
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
WARNING
-------
RF BURNS. RF energy at frequencies above approximately 300 kHz is absorbed
by skin tissue and causes thermal burns. These burns may be deep, with delayed
onset of pain. Do not contact antenna ports, feed lines, connectors, or any
part of the load's RF signal path while the transmitter is keyed or capable
of being keyed.
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
CAUTION
-------
HOT SURFACES. Heatsinks, resistor bodies, oil containers, and fan outlets
reach temperatures in excess of 100 degrees C under rated operation. Allow
10 minutes cool-down after high-power operation before handling any thermally
exposed component.
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
CAUTION
-------
OIL HANDLING. Mineral oil is a skin and eye irritant. Wear nitrile gloves
and eye protection when handling. Dispose of spent mineral oil per local
regulations (classify as used motor oil). Do not pour down drains.
--------------------------------------------------------------------------------

1-2.  GENERAL SAFETY REQUIREMENTS

1-2.1  Fusing. All DC supply circuits (fan, relay, CYD monitor) shall be
fused within 150mm of the power supply terminals.

1-2.2  Grounding. The chassis of all dummy loads shall be bonded to the
station ground system via a minimum #12 AWG ground conductor. This prevents
static buildup and provides a path for fault currents.

1-2.3  Interlock. High-power loads (500W and above) should be controlled
via an antenna switch relay that physically disconnects the transmitter when
the CYD monitor trips on over-temperature. Do not rely on software interlocks
alone at power levels above 500W.

1-2.4  Ventilation. Air-cooled loads shall be operated in areas with
unrestricted air circulation. Do not enclose in cabinets without forced-air
ventilation. Oil-cooled loads require free air circulation around the exterior
surface of the container.

1-2.5  Operating environment. Do not operate any dummy load outdoors in rain.
Connectors and cooling fans are not weatherproof in these designs.


================================================================================
CHAPTER 2  GENERAL DESCRIPTION
================================================================================

2-1.  PURPOSE OF DUMMY LOADS

2-1.1  A dummy load (also called a phantom antenna, phantom load, or RF
termination) is a non-radiating resistive termination designed to absorb
transmitter output power for testing purposes. Principal uses include:

    (a) Initial alignment and test of transmitters without causing interference
    (b) Transmitter power measurement
    (c) Transceiver audio and ALC circuit adjustment
    (d) SWR bridge and directional coupler calibration
    (e) VNA and antenna analyzer SOLT calibration standard (50-ohm "load")
    (f) Safe dissipation of stored energy in transmitter power supply capacitors
        via the RF output port

2-2.  TYPES COVERED IN THIS MANUAL

2-2.1  DRY (AIR-COOLED). Resistors mounted on aluminum heatsink, force-cooled
with a brushless fan. Compact, portable, suitable for bench use or field
operations. Power limited to 100W continuous (250W with derating). Preferred
for 6M-2M applications where oil loads degrade VHF performance.

2-2.2  CANTENNA (OIL-COOLED). Resistors submerged in mineral oil in a metal
can (adapted from the original ARRL "Cantenna" design). Self-cooling via oil
thermal mass. Low-cost construction. 500W rated. Standard HF field dummy load.

2-2.3  LARGE OIL-COOLED. Welded aluminum tank with multiple resistor banks
in oil. 1kW rated, with thermal mass for extended operation. Requires external
forced-air for continuous 1kW duty.

2-2.4  PRECISION VNA LOAD. SMD thin-film resistors on controlled-impedance
microwave PCB substrate. Designed for accuracy and low VSWR from DC to 6 GHz.
Power limited to 1-2W. Used only as measurement reference, not power dissipation.

2-3.  FREQUENCY COVERAGE

2-3.1  All loads in this manual cover the following amateur bands:

    Band     Freq        DRY    CAN    1kW    VNA
    ------   --------    ---    ---    ---    ---
    160M     1.8 MHz     Yes    Yes    Yes    Yes
    80M      3.5 MHz     Yes    Yes    Yes    Yes
    40M      7.0 MHz     Yes    Yes    Yes    Yes
    30M      10.1 MHz    Yes    Yes    Yes    Yes
    20M      14.0 MHz    Yes    Yes    Yes    Yes
    17M      18.1 MHz    Yes    Yes    Yes    Yes
    15M      21.0 MHz    Yes    Yes    Yes    Yes
    12M      24.9 MHz    Yes    Yes    Yes    Yes
    10M      28.0 MHz    Yes    Yes    Yes    Yes
    6M       50.0 MHz    Yes    Deg    Deg    Yes
    2M      146.0 MHz    Yes     No     No    Yes
    1.25M   222.0 MHz    Deg     No     No    Yes
    70cm    440.0 MHz     No     No     No    Yes
    33cm    902.0 MHz     No     No     No    Yes
    23cm   1296.0 MHz     No     No     No    Yes

    Yes = Full spec. Deg = Performance degraded but functional. No = Not rated.

2-3.2  Oil-cooled loads become unsuitable above 50 MHz primarily due to the
electrical length of lead wires within the oil tank. At 146 MHz, even a 50mm
lead wire presents significant inductance that degrades impedance match.


================================================================================
CHAPTER 3  THEORY OF OPERATION
================================================================================

3-1.  RESISTIVE TERMINATION PRINCIPLES

3-1.1  An ideal 50-ohm dummy load presents a purely resistive impedance of
50 ohms from DC to infinite frequency. In practice, parasitic elements
(inductance in wire leads, capacitance between components and chassis,
dielectric effects in supporting materials) cause the impedance to deviate
from 50 ohms as frequency increases.

3-1.2  The reflection coefficient of a load is defined as:

        Z_load - Z_source
    r = -----------------
        Z_load + Z_source

    For Z_source = 50 ohms and Z_load = R + jX:

        (R - 50) + jX
    r = --------------
        (R + 50) + jX

3-1.3  Magnitude of r (rho) determines SWR:

    |r| = sqrt((R-50)^2 + X^2) / sqrt((R+50)^2 + X^2)

    SWR = (1 + |r|) / (1 - |r|)

    Return Loss (dB) = -20 * log10(|r|)

3-2.  PARASITIC REACTANCE AND SWR DEGRADATION

3-2.1  Every resistor has series lead inductance and shunt capacitance.
For a non-inductive resistor:

    Z_actual(f) = R + j*omega*L_lead - j/(omega*C_shunt)

    where L_lead ~ 1 nH/mm of lead length
          C_shunt ~ 0.5-2 pF for typical power resistors

3-2.2  At 10 MHz, 10mm of lead wire: X_L = 2*pi*10e6*10e-9 = 0.63 ohm
       This alone causes SWR of 1.013 -- negligible.

       At 200 MHz, same lead: X_L = 12.6 ohm
       SWR = (50 + 12.6) / (50 - 12.6) = ... this is now inductive,
       not simple but: |r| = sqrt((R-50)^2 + 12.6^2) / sqrt((R+50)^2 + 12.6^2)
       Assuming R=50: |r| = 12.6/sqrt(2500+158) = 12.6/51.5 = 0.245
       SWR = 1.64:1 -- SIGNIFICANT

3-2.3  This is why lead length must be minimized for VHF/UHF loads, and
precision VNA loads use SMD components with < 1mm of effective lead length.

3-2.4  DISTRIBUTED RESISTANCE TECHNIQUE. Using multiple resistors in
parallel, arranged in a star pattern, reduces effective lead inductance
by paralleling the inductances. 4 resistors in parallel: L_eff = L/4.
This is why all designs in this manual use multiple-resistor networks
rather than a single high-value power resistor.

3-3.  POWER DISSIPATION AND THERMAL MANAGEMENT

3-3.1  Joule's Law governs power dissipation:

    P = I^2 * R = V^2 / R = V * I

    For 50-ohm load at power P:
    V_rms = sqrt(P * 50)
    I_rms = sqrt(P / 50)

TABLE 3-1. VOLTAGE AND CURRENT AT KEY POWER LEVELS

    Power    V_rms     V_peak    I_rms     I_peak
    -----    -----     ------    -----     ------
     10W      22.4V     31.6V    0.447A    0.632A
     50W      50.0V     70.7V    1.000A    1.414A
    100W      70.7V    100.0V    1.414A    2.000A
    200W     100.0V    141.4V    2.000A    2.828A
    500W     158.1V    223.6V    3.162A    4.472A
   1000W     223.6V    316.2V    4.472A    6.325A
   1500W     273.9V    387.3V    5.477A    7.746A


3-3.2  HEAT FLOW MODEL. The thermal resistance network from resistor junction
to ambient:

    P = (T_junction - T_ambient) / (theta_jc + theta_cs + theta_sa)

    where:
    theta_jc = junction-to-case thermal resistance (resistor datasheet)
    theta_cs = case-to-heatsink (with thermal compound: ~0.1 C/W)
    theta_sa = heatsink-to-ambient (depends on heatsink, airflow)

    For maximum continuous operation:
    T_junction_max = typically 150-200C (resistor dependent)
    T_ambient = 25C standard (50C worst case in hot weather)

    Required theta_total = (T_jmax - T_ambient) / P

    EXAMPLE: 100W load, four 25W resistors, T_jmax=150C, T_amb=25C:
    theta_total = (150 - 25) / 25 = 5.0 C/W per resistor
    theta_jc (for 25W resistor): ~3 C/W (datasheet)
    theta_cs: 0.1 C/W with compound
    Required theta_sa = 5.0 - 3.0 - 0.1 = 1.9 C/W

    This heatsink requirement (1.9 C/W per 25W resistor) requires a
    moderate heatsink with forced-air cooling.

3-3.3  OIL COOLING ADVANTAGE. Oil immersion eliminates theta_cs (the
critical resistance between resistor case and heat sink) by providing
full-surface contact between the resistor body and the heat transfer
medium. Effective theta_cs of oil-immersed resistor: <0.05 C/W.
The oil then conducts and convects heat to the container walls.
For large oil volumes: the thermal mass provides a "buffer" for duty-cycle
operation. See Appendix B for duty cycle calculations.

3-4.  SWR AND RETURN LOSS SPECIFICATIONS

3-4.1  TABLE 3-2. SWR SPECIFICATIONS BY DESIGN TYPE AND FREQUENCY

    Frequency     DRY Load    Oil Load    VNA Load
    ----------    --------    --------    --------
    1-30 MHz      <1.05:1     <1.05:1     <1.02:1
    30-100 MHz    <1.15:1     <1.20:1     <1.05:1
    100-300 MHz   <1.30:1     <1.50:1     <1.10:1
    300-500 MHz   <1.60:1     N/A         <1.15:1
    500M-1 GHz    <2.50:1     N/A         <1.20:1
    1-3 GHz       N/A         N/A         <1.30:1
    3-6 GHz       N/A         N/A         <1.50:1

    All values assume specified connector type (N or SMA).
    SO-239 connector degrades SWR above 150 MHz.

3-5.  DUTY CYCLE LIMITATIONS

3-5.1  Rated power is the continuous carrier power the load can sustain
indefinitely. Many amateur modes use duty cycles less than 100%:

    Mode         Typical Duty Cycle    Effective Average Power
    -----        ------------------    -----------------------
    CW           40-50%                40-50% of peak
    SSB voice    20-30%                20-30% of peak
    AM carrier   100% (carrier)        100% minimum
    FM           100%                  100%
    FT8/JS8      50%                   50%
    RTTY         100%                  100%
    WSPR         100% (when TX)        20% (20-min cycle)

3-5.2  The effective thermal load is the AVERAGE power, not the peak power.
For a 100W load operating with 30% duty cycle SSB, the average thermal
load is only 30W -- well within continuous ratings. However, peak voltages
and currents during the active portion are still full-power values and must
not exceed the resistor voltage/current ratings.

3-5.3  PEAK VOLTAGE LIMITATION. Resistors have maximum working voltage
independent of power. Common limits:

    Resistor type        Max voltage
    ----------------     ----------
    25W wirewound HF     500-750V
    50W ceramic          600-1000V
    100W ceramic         1000-1500V

    For SSB, peak power can momentarily exceed average by 4:1 (6 dB).
    At 1kW average SSB power: peak = 4kW, V_peak = sqrt(4000 x 50) = 447V
    Verify this is within resistor maximum voltage rating.


================================================================================
CHAPTER 4  100W DRY (AIR-COOLED) DUMMY LOAD
================================================================================

4-1.  DESCRIPTION

4-1.1  The 100W dry load uses four 200-ohm, 25W non-inductive resistors in
parallel, mounted on a finned aluminum heatsink with forced-air cooling via
an 80mm brushless DC fan. It is compact (150 x 100 x 80mm), portable, and
suitable for all amateur HF and VHF bands when equipped with an N connector.

4-2.  PERFORMANCE SUMMARY

    Impedance:     50 ohm ± 2%
    Power (fan):   100W continuous, 150W for 5 min, 200W for 30 sec
    Power (no fan):50W continuous, 75W for 5 min
    Frequency:     DC to 500 MHz (N connector)
    SWR:           < 1.05:1 (DC-30 MHz), < 1.3:1 (to 300 MHz)
    Connector:     N-type female (UHF/SO-239 for HF-only version)
    Cooling:       80mm 12VDC brushless fan
    Temperature:   NTC thermistor with CYD monitor or standalone

4-3.  RESISTOR NETWORK

4-3.1  Four 200-ohm resistors in parallel = 50 ohms. This configuration
provides four times the power handling of a single resistor while also
reducing effective lead inductance by a factor of four.

4-3.2  STAR WIRING. All resistor hot ends connect to a single center stud
adjacent to the connector. All cold ends connect to the heatsink chassis.
Lead length must be < 10mm for VHF performance.

4-4.  HEATSINK REQUIREMENTS

4-4.1  The heatsink thermal resistance must satisfy:

    theta_sa < (T_case_max - T_ambient - P*theta_jc) / P

    For Ohmite 270-1-R200E (25W ceramic, theta_jc = 3.5 C/W):
    T_case_max = 155C (rated), T_amb = 25C, P = 25W per resistor

    theta_sa < (155 - 25 - 25*3.5) / 25 = (155 - 25 - 87.5) / 25 = 1.7 C/W

    Required: heatsink theta_sa < 1.7 C/W per resistor position.
    With 80mm fan: Wakefield 433-40 series achieves 0.6-0.9 C/W -- adequate.
    Without fan: typical 150x100mm heatsink = 2-3 C/W -- MARGINAL to FAIL.
    Fan is mandatory for rated 100W continuous operation.


================================================================================
CHAPTER 5  500W OIL-COOLED (CANTENNA) DUMMY LOAD
================================================================================

5-1.  DESCRIPTION

5-1.1  The cantenna uses ten 500-ohm, 50W non-inductive resistors submerged
in mineral oil in a standard 1-gallon steel paint can. The oil provides
efficient heat transfer and serves as the dielectric between the resistors
and the can wall (ground). The original design was described in the ARRL
Handbook and has been in widespread use since the 1960s.

5-2.  PERFORMANCE SUMMARY

    Impedance:     50 ohm ± 2%
    Power:         500W continuous (adequate oil and ventilation)
                   750W for 3 minutes, 1kW for 60 seconds
    Frequency:     DC to 60 MHz (full spec), DC to 150 MHz degraded
    SWR:           < 1.05:1 (DC-30 MHz)
    Connector:     N-type female (bottom center)
    Cooling:       Oil thermal mass, natural convection
    Oil volume:    3.5 liters (1-gallon can)
    Weight:        ~4 kg fully assembled

5-3.  OIL SELECTION

5-3.1  See Appendix E for complete oil specifications.

5-3.2  PREFERRED: Naphthenic mineral oil (transformer oil). Available from
electrical supply distributors as "transformer oil Type II" or equivalent.
Shell Diala S4 ZX-I, Chevron Transformer Oil, or similar.

5-3.3  ACCEPTABLE: Food-grade mineral oil from pharmacy or food supply.
    Higher cost but readily available. Performance equivalent.

5-3.4  CAUTION: Do not use cooking oil (polyunsaturates oxidize and form
    deposits), motor oil (sulfur additives cause corrosion), or synthetic
    lubricants (potential fire hazard at high temperatures).

5-4.  THERMAL CALCULATIONS

5-4.1  Oil thermal capacity (3.5 liters):

    Mass = 3.5 L * 0.86 kg/L = 3.0 kg
    Specific heat = 1.8 kJ/(kg*C)
    Thermal capacity = 3.0 * 1800 = 5400 J/C

    Temperature rise rate at 500W:
    dT/dt = 500W / 5400 J/C = 0.093 C/second

    Time to reach 80C limit (from 25C ambient):
    t = (80 - 25) / 0.093 = 591 seconds = 9.9 minutes

5-4.2  At 500W the cantenna provides approximately 10 minutes of
continuous operation before requiring a cool-down period. At 250W,
continuous operation exceeds 20 minutes. At 100W, continuous
indefinitely (oil radiates approximately 10W/C above ambient
through can walls; steady state at 100W: T_oil ≈ 25 + 100/10 = 35C).


================================================================================
CHAPTER 6  1kW OIL-COOLED DUMMY LOAD
================================================================================

6-1.  DESCRIPTION

6-1.1  The 1kW load uses twenty 1000-ohm, 50W non-inductive resistors in
five banks of four, submerged in 4.5 liters of mineral oil in a welded
aluminum tank. The larger oil volume and aluminum walls provide greater
thermal capacity and radiation area compared to the cantenna.

6-2.  PERFORMANCE SUMMARY

    Impedance:     50 ohm ± 2%
    Power:         500W continuous (natural cooling)
                   1kW for 6.5 minutes (then shutdown required)
                   1kW continuous (with external forced-air on tank walls)
    Frequency:     DC to 50 MHz (full spec)
    SWR:           < 1.05:1 (DC-30 MHz)
    Connector:     N-type female
    Oil volume:    4.5 liters
    Weight:        ~8 kg fully assembled

6-3.  OPERATION AT EXTENDED DUTY

6-3.1  Without external cooling, the 1kW load reaches oil temperature limits
(80C) in approximately 6-7 minutes at full 1kW input. Standard procedure:

    STEP 1. Verify oil level is above top resistor bank.
    STEP 2. Verify temperature monitor is operating (CYD or standalone).
    STEP 3. Begin testing. Monitor oil temperature continuously.
    STEP 4. At 80C, reduce power or switch to antenna.
    STEP 5. Allow oil to cool to 40C before resuming 1kW operation.
            Cooling time approximately 15-20 minutes (natural convection).

6-3.2  With external forced air (12V box fan, 150 CFM, blowing across
tank exterior), thermal equilibrium shifts:
    At 500W: T_steady_state ≈ 25 + 500/25 = 45C (sustainable)
    At 1kW: T_steady_state ≈ 25 + 1000/25 = 65C (within limits)
    25 W/C is achievable for a 150x150x320mm aluminum tank with fan.

6-4.  OVER-TEMPERATURE PROTECTION

6-4.1  The relay circuit (described in sch_1kw_oil.txt) opens the antenna
switch relay when oil temperature exceeds 95C. The relay is de-energized
(failsafe open) on alarm, so a power failure also disconnects the load.

6-4.2  The CYD monitor (cyd/power_monitor_cyd.ino) provides:
    - Continuous temperature display
    - Warning alarm at 80C
    - Relay trip at 95C
    - Relay reset after temperature falls below 90C
    - Cumulative energy tracking


================================================================================
CHAPTER 7  PRECISION VNA CALIBRATION LOADS
================================================================================

7-1.  PURPOSE

7-1.1  The precision load serves as the "LOAD" standard in the SOLT
(Short-Open-Load-Through) calibration of vector network analyzers. Unlike
power loads, this standard must be accurate to within a few milliohms of
50 ohms from DC to multi-gigahertz frequencies.

7-2.  DESIGN REQUIREMENTS

7-2.1  Return loss > 30 dB (SWR < 1.07:1) from DC to 3 GHz.
       This requires:
       (a) Resistor tolerance: 0.1% or better
       (b) Thin-film resistor construction (no wirewound)
       (c) Controlled-impedance PCB substrate (Rogers preferred)
       (d) Minimized parasitic elements (SMD, short leads)
       (e) Precision connector (SMA or N-type, machined body)

7-2.2  SUBSTRATE SELECTION. FR4 epoxy substrate (common PCB material)
has a relative permittivity that varies with frequency and absorbs moisture.
Above 2 GHz, FR4 causes significant loss and phase shift. Rogers RO4003C
or equivalent low-loss microwave laminate is required for DC-6 GHz service.
FR4 is acceptable for DC-2 GHz with awareness of degraded return loss.

7-3.  CALIBRATION UNCERTAINTY

7-3.1  The reference load is only as good as its component accuracy.
Contributing uncertainties:

    Source                   Magnitude    Return Loss Impact
    ----------------------   ---------   ------------------
    Resistor tolerance        ±0.1%        -54 dB (negligible)
    Resistor temperature coef ±25 ppm/C    -60 dB/C (negligible)
    PCB substrate loss        <0.1 dB      -40 dB at 3 GHz
    Connector return loss     <0.02 dB     -34 dB (SMA quality)
    PCB moisture absorption   Variable     Significant if FR4

7-3.2  STORAGE. Store precision loads in sealed container with desiccant.
Do not expose to high humidity or condensing environments. Moisture absorbed
in the substrate increases permittivity and shifts impedance.

7-4.  MAXIMUM POWER

7-4.1  Precision loads are rated for 1W maximum (CW) or 5W for 1 second.
Do NOT use precision loads for transmitter testing. These loads are for
VNA reference ports only, which provide < 0 dBm (1 mW).


================================================================================
CHAPTER 8  DIRECTIONAL COUPLER SUBSYSTEM
================================================================================

8-1.  PURPOSE

8-1.1  The directional coupler samples forward and reflected power from the
main line without significantly loading the transmission line. Output ports
drive the CYD power monitor for real-time SWR and power display.

8-2.  COUPLER TYPES

8-2.1  BRUENE TOROID COUPLER (1-30 MHz). Uses a ferrite toroid with a
1-turn primary (thru-line) and 10-turn secondary (20 dB coupling).
Advantages: low insertion loss (<0.15 dB), high directivity (>25 dB),
simple construction, handles full 1.5kW. Best for HF applications.

8-2.2  RESISTIVE BRIDGE COUPLER (1-500 MHz). Uses a resistive Wheatstone
bridge to sample forward and reflected voltage. Advantages: broadband DC-500 MHz
flat response, no reactive elements, compact PCB implementation.
Disadvantage: 0.8 dB insertion loss (acceptable for test applications).

8-3.  DETECTOR CIRCUIT

8-3.1  Schottky diode envelope detector converts sampled RF to DC voltage
proportional to RF amplitude. The DC voltage is read by the CYD's ADC.

    V_dc = detector_efficiency * coupling_factor * V_rf

    For BAT54A, 20 dB coupler, at 14 MHz (well above cutin, linear region):
    V_dc ≈ 0.9 * 0.1 * V_line_rms

    For 100W into 50 ohms:
    V_line_rms = sqrt(100*50) = 70.7V
    V_dc ≈ 0.9 * 0.1 * 70.7 = 6.36V -- exceeds ESP32 ADC 3.3V limit!

    SOLUTION: Use voltage divider (10:1) at ADC input, or
              Select coupling factor 30 dB (1:32 voltage coupling).

8-3.2  ADC INPUT PROTECTION. Always install 3.3V Zener clamp (BZX84C3V3)
at each ADC input pin in series with 1k resistor. The ESP32 ADC is damaged
by voltages exceeding 3.3V. The Schottky detector can output 5-10V on RF
bursts even with 20-30 dB coupling at high power levels.


================================================================================
CHAPTER 9  MATERIALS AND PROCUREMENT
================================================================================

9-1.  RESISTORS

9-1.1  CRITICAL REQUIREMENT: ALL resistors must be NON-INDUCTIVE types.
Wirewound resistors have significant inductance that becomes dominant at
HF frequencies. A supposedly "non-inductive" bifilar-wound wirewound
resistor typically has residual inductance of 100-500 nH, giving
reactance of 6-30 ohms at 10 MHz -- unacceptable.

9-1.2  APPROVED TYPES:

    Type                  Manufacturer/Part#    Rated
    ------------------    --------------------  -----
    Ceramic body axial    Ohmite 270-1-R200E    200R 25W
    Ceramic body axial    Ohmite RH-50E500R     500R 50W
    Ceramic body axial    Ohmite RH-50E1K       1000R 50W
    Ceramic body axial    TE Conn. HSC50        (various, 50W)
    Thick-film cermet     Vishay RH050          25W, various
    Thin-film SMD         Vishay FC-series 0402 63mW, VNA use
    Thin-film SMD         Susumu RG-series 0603 100mW, VNA use

9-1.3  DO NOT USE:
    - Standard wirewound resistors (inductive, even "non-inductive" types)
    - Carbon composition (high temperature coefficient, noisy)
    - Metal oxide film (parasitic reactance above 50 MHz)
    - Any resistor with a coil or winding visible through the body

9-2.  CONNECTORS

    Application           Connector    Notes
    ------------------    ---------    ----------------------------
    HF only (<30 MHz)     SO-239/PL259 UHF series, adequate for HF
    HF through 2M         N-type       Required above 150 MHz
    HF through 6M (pref.) N-type       Better than SO-239 at 50 MHz
    VNA reference load    SMA          Best accuracy DC-6 GHz
    VNA reference (med.)  N-type       Good to 3 GHz
    Directional coupler   SMA          Sample ports

9-3.  THERMAL COMPOUNDS

    Dow Corning TC-5026 or equivalent
    Thermal conductivity: > 3.5 W/(m*K)
    Apply thin, uniform layer between resistor body and heatsink
    Do not use silicone grease (< 1 W/(m*K), inadequate)

9-4.  OIL

    See Appendix E for complete specifications.


================================================================================
CHAPTER 10  CONSTRUCTION PROCEDURES
================================================================================

10-1.  100W DRY LOAD CONSTRUCTION

STEP 1.  Print or obtain enclosure from openscad/enclosure_100w_dry.scad.
         If printed: ASA material, 4 perimeters, 25% infill.

STEP 2.  Mount heatsink to rear panel. Apply thermal compound to mating
         surfaces. Secure with four M3 stainless screws.

STEP 3.  Mount N-connector to front panel. Apply silver solder to connection.
         Clean with isopropyl alcohol after soldering.

STEP 4.  Mount resistors on heatsink. Apply thermal compound to each resistor
         mounting face. Mount with 6-32 screws and locking washers.

STEP 5.  STAR WIRE the resistors. Use #16 AWG or larger, keep leads < 10mm.
         All hot ends to N-connector center pin.
         All cold ends to heatsink ground stud.

STEP 6.  Mount fan on top panel with M3 x 20mm screws, fan guard facing out.

STEP 7.  Install NTC thermistor adjacent to resistors with cable tie. Route
         cable to CYD or standalone cutoff circuit.

STEP 8.  DC-test: measure 50 ohm ± 2 ohm between N-connector center and shell.
         If out of range, check for solder bridges or wrong resistor values.

STEP 9.  RF-test: use NanoVNA, sweep DC-500 MHz, verify SWR per Table 3-2.

STEP 10. POWER TEST: apply 10W RF for 60 seconds. Verify temperature rise
         is < 5C above ambient within 30 seconds of power application.
         (At 10W, steady-state heatsink temperature should stabilize at
         approximately 25 + 10*theta_sa = 25 + 10*0.7 = 32C with fan running).

10-2.  CANTENNA CONSTRUCTION

STEP 1.  Obtain 1-gallon clean steel paint can with lid.
         Remove any paint, clean interior with acetone or IPA.

STEP 2.  Print base adapter plate and lid from openscad/enclosure_cantenna.scad.

STEP 3.  Drill center of can bottom for N-connector (21mm hole).
         Drill carefully -- can bottom is thin. Use step drill.

STEP 4.  Cut and thread brass center stud (1/4"-20 x 250mm).
         Install PTFE standoff on stud, insert through N-connector,
         solder N-connector center pin to stud with silver solder.
         This joint must be electrically and mechanically perfect.
         (Re-work if there is any solder void or cold joint.)

STEP 5.  Mount N-connector in can bottom hole. The connector shell is
         soldered or bolted to can base (ground connection).

STEP 6.  Fabricate ground rings (copper or brass, OD=120mm, 3mm wide).
         One ring for each of two resistor banks (lower and upper).

STEP 7.  Wire lower bank (R1-R5):
         Solder each resistor hot end to center stud at appropriate heights.
         Solder each cold end to lower ground ring.
         Verify ring is soldered to can base (or connected by copper strap).

STEP 8.  Wire upper bank (R6-R10):
         Same as lower bank. Upper ring connects by copper strap to can wall.

STEP 9.  DC test: 50 ohm ± 2 ohm. If incorrect, check all parallel connections.

STEP 10. Fill with mineral oil to 15mm above top resistor bank.
         Install pressure relief plug (1/8" NPT with 0.5mm orifice).
         Install lid with NTC probe.

STEP 11. RF test and power test per Chapter 11.


================================================================================
CHAPTER 11  TESTING AND VERIFICATION
================================================================================

11-1.  DC RESISTANCE VERIFICATION

PROCEDURE:
    STEP 1. With no power applied, measure DC resistance between connector
            center pin and shell using a precision LCR meter or 4-wire DMM.
    STEP 2. Acceptable values: 49.0 - 51.0 ohm for all designs.
    STEP 3. For VNA loads: 49.9 - 50.1 ohm required.
    STEP 4. If measured resistance is incorrect, check all parallel connections
            and resistor values before applying RF power.

11-2.  NanoVNA SWEEP PROCEDURE

    STEP 1. Calibrate NanoVNA with SOLT at the measurement connector type
            (SMA, N-type, or SO-239 per load design).
    STEP 2. Connect dummy load to NanoVNA port 1.
    STEP 3. Enable S11 display in logmag and Smith chart formats.
    STEP 4. Set sweep range: 1 MHz to maximum rated frequency.
    STEP 5. Verify:
            (a) Smith chart plot stays within the 1.5:1 SWR circle
                from DC to 30 MHz.
            (b) Marker at 14 MHz: return loss > 26 dB.
            (c) Marker at 144 MHz: return loss > 14 dB.
            (d) At maximum rated frequency, return loss > 10 dB.

11-3.  LOW-POWER RF TEST

    STEP 1. Apply 1W RF from calibrated source or transceiver on low power.
    STEP 2. Verify no arcing, burning smell, or visible damage.
    STEP 3. Monitor temperature for 2 minutes. Heatsink or oil should
            warm slightly. Normal.

11-4.  RATED POWER TEST

    WARNING: Rated power tests produce hazardous voltages. Stand clear
    of load during test. Do not touch any part of the RF circuit.

    STEP 1. Connect load to transmitter via appropriate coaxial cable
            (RG-8 or LMR-400 for 500W+).
    STEP 2. Set transmitter to CW mode.
    STEP 3. Key for 5 seconds at 50% rated power. Release.
    STEP 4. Monitor temperature for 30 seconds.
    STEP 5. If no issues, key for 30 seconds at rated power.
    STEP 6. Verify SWR meter on transmitter reads < 1.3:1.
    STEP 7. For oil loads: verify oil temperature rises predictably
            per the rate table in paragraph 5-4.1.


================================================================================
CHAPTER 12  MAINTENANCE AND INSPECTION
================================================================================

12-1.  INSPECTION INTERVALS

    After each high-power session: visual inspection
    Quarterly (active use):        electrical inspection
    Annually:                      full inspection including oil sampling

12-2.  VISUAL INSPECTION

    STEP 1. Inspect connector for corrosion, loose shell, damaged threads.
    STEP 2. Inspect enclosure for heat discoloration (indicates overload).
    STEP 3. For oil loads: inspect oil level (should cover top bank).
            Oil should be clear amber to pale yellow. Dark brown or black
            oil indicates overheating and must be replaced.
    STEP 4. Inspect fan (dry load): spin fan blade freely by hand.
            Any resistance indicates failed bearing, replace fan.

12-3.  OIL REPLACEMENT

12-3.1  Replace oil annually or if oil is discolored or cloudy.

    STEP 1. Allow load to cool to room temperature (minimum 30 minutes
            after last use).
    STEP 2. Place oil-collection container (minimum 5L) below drain.
    STEP 3. Remove pressure relief plug or open fill port.
    STEP 4. Drain oil completely. Allow to drip dry for 15 minutes.
    STEP 5. Inspect interior. If carbon deposits present, flush with
            a small amount of fresh oil, drain, then refill.
    STEP 6. Fill with fresh mineral oil to specified level.
    STEP 7. Re-install pressure relief. Verify for leaks.

12-4.  RESISTOR RESISTANCE CHECK

12-4.1  If DC resistance has shifted significantly (>2% from nominal 50 ohm),
individual resistors may be failing. To identify the defective resistor:

    STEP 1. For dry loads: disconnect all parallel resistors. Measure each
            individually. Replace any resistor measuring outside ±5% of
            rated value.
    STEP 2. For oil loads: this requires partial draining. Drain to below
            the bank in question. Measure each resistor with Kelvin clips.


================================================================================
CHAPTER 13  TROUBLESHOOTING
================================================================================

TABLE 13-1. DUMMY LOAD TROUBLESHOOTING

  SYMPTOM                    PROBABLE CAUSE          CORRECTIVE ACTION
  -------------------------  ----------------------  ----------------------
  SWR >2:1 all bands         Open resistor           Check DC resistance.
                             Loose connector         Tighten/re-solder.
                             Wrong R value           Verify resistors.

  SWR good at HF, high VHF  Lead inductance too high  Shorten leads <10mm.
                             Wrong connector          Use N-type, not SO-239.
                             Ground loop in wiring    Use star ground.

  Load runs hot, SWR OK      Inadequate cooling      Add fan or increase flow.
                             Wrong resistor type      Verify non-inductive.
                             Overpower condition      Reduce transmitter power.

  Oil smokes or smells       Oil temperature > 120C  Shut down immediately.
  (oil-cooled loads)         Pressure relief blocked Open relief valve.
                             Wrong oil type           Replace with mineral oil.

  VNA load reads wrong       Resistor value drift     Replace 0402 SMDs.
  impedance after storage    Moisture in substrate   Bake at 70C for 2 hours.
                             Connector damage         Replace connector.

  Relay trips spuriously     Thermistor fault         Check thermistor
  (CYD monitor)              CYD ADC calibration      resistance at room temp.
                             Noise on ADC input       Add 10uF cap to ADC pin.

  No power reading on CYD    Coupler polarity wrong   Swap FWD/REF detector.
                             Detector diode open     Replace BAT54.
                             ADC pin damage          Check for >3.3V on pin.


================================================================================
CHAPTER 14  REPAIR PROCEDURES
================================================================================

14-1.  RESISTOR REPLACEMENT (DRY LOAD)

    STEP 1. De-energize and allow to cool 10 minutes.
    STEP 2. Disconnect resistor hot lead from center stud.
    STEP 3. Remove mounting screws.
    STEP 4. Clean heatsink surface with IPA, remove old thermal compound.
    STEP 5. Install replacement resistor with fresh thermal compound.
    STEP 6. Re-solder lead. Verify correct connection.
    STEP 7. DC test: verify 50 ohm ± 2 ohm.
    STEP 8. Perform full RF and power test per Chapter 11.

14-2.  CONNECTOR REPLACEMENT

14-2.1  Connector replacement requires de-soldering the center conductor
connection from the resistor wiring. For oil-cooled loads, partial drain
is required before connector replacement.

    STEP 1. Drain oil to below the bottom resistor bank (oil loads).
    STEP 2. Heat center pin connection with 80W iron to loosen solder.
    STEP 3. Remove connector body fasteners.
    STEP 4. Install replacement connector. Use silver solder (Ag bearing)
            for center pin connection.
    STEP 5. Verify DC resistance: 50 ohm ± 2 ohm.
    STEP 6. Refill oil (if applicable).
    STEP 7. RF test per Chapter 11.

14-3.  FAN REPLACEMENT (DRY LOAD)

    STEP 1. Disconnect fan power leads.
    STEP 2. Remove four fan mounting screws.
    STEP 3. Install replacement fan (same size, same or higher CFM rating).
            Do not substitute a fan with lower CFM than original.
    STEP 4. Re-connect power leads. Verify correct polarity.
    STEP 5. Verify fan starts when power is applied and runs smoothly.


================================================================================
APPENDIX A  POWER DERATING CURVES
================================================================================

A-1.  GENERAL DERATING PRINCIPLES

A-1.1  All power ratings in this manual assume:
    (a) Ambient temperature: 25C
    (b) Fan in operating condition (for dry loads)
    (c) Oil level at specified height (for oil loads)
    (d) Adequate external air circulation (for oil loads)

A-1.2  TEMPERATURE DERATING (AIR-COOLED):

    T_ambient (C)    Derating Factor    Max 100W Load Power
    -------------    ---------------    -------------------
         0                1.15               115W
        25                1.00               100W
        35                0.90                90W
        40                0.85                85W
        50                0.70                70W
        60                0.55                55W

    Formula: P_max = P_rated * (1 - (T_amb - 25) * 0.015)
    (Approximate linear derating, valid 25-65C ambient)

A-2.  DUTY CYCLE DERATING TABLE

    Duty    Peak Power Multiplier    Example: 100W load allows:
    -----   --------------------    -------------------------
    100%    1.00 x rated            100W continuous
     75%    1.15 x rated            115W at 75% duty
     50%    1.30 x rated            130W at 50% duty
     30%    1.60 x rated            160W at 30% duty
     20%    1.90 x rated            190W at 20% duty
     10%    2.50 x rated            250W at 10% duty

    CAUTION: Peak power must still be within resistor voltage rating.
    At 250W peak into 50R: V_peak = sqrt(250 * 50 * 2) = 158V (within
    typical 500V resistor rating). Safe.

A-3.  OIL LOAD DUTY CYCLE AT 1kW

    Cycle time (min on / min off)    Safe for:
    -------------------------------- --------
    1 / 9   (10% duty)               Indefinite
    3 / 7   (30% duty)               Indefinite if oil < 60C
    5 / 5   (50% duty)               Monitor oil temperature
    6 / 14  (30%, 20-min cycle)      Standard WSPR-like profile
    Continuous (100% at 1kW)         6.5 minutes maximum, then 20 min cool

A-4.  EXPECTED SWR vs FREQUENCY (DRY LOAD, N CONNECTOR)

    Frequency   Typical SWR     Return Loss    Specification
    ---------   -----------     -----------    -------------
    1 MHz        1.01:1          46 dB          <1.05:1
    7 MHz        1.01:1          46 dB          <1.05:1
    14 MHz       1.02:1          40 dB          <1.05:1
    28 MHz       1.03:1          36 dB          <1.05:1
    50 MHz       1.06:1          31 dB          <1.15:1
    100 MHz      1.12:1          25 dB          <1.30:1
    146 MHz      1.18:1          22 dB          <1.30:1
    200 MHz      1.28:1          19 dB          <1.60:1
    300 MHz      1.45:1          15 dB          <1.60:1
    440 MHz      1.80:1          11 dB          N/A
    500 MHz      2.10:1           8 dB          N/A


================================================================================
APPENDIX B  DUTY CYCLE CALCULATIONS
================================================================================

B-1.  THERMAL MASS CALCULATION (OIL LOADS)

    Energy budget (oil thermal mass only):
    E_max = m_oil * Cp_oil * (T_max - T_initial)
    E_max = m_oil * 1800 J/(kg*C) * delta_T

    For 3.5L mineral oil (m=3.0 kg), T_initial=25C, T_max=80C:
    E_max = 3.0 * 1800 * 55 = 297,000 J = 297 kJ

    At P watts (all power goes to oil):
    t_max = E_max / P = 297,000 / P  (seconds)

TABLE B-1. MAXIMUM CONTINUOUS OPERATION TIME (500W CANTENNA)

    Power (W)    Time to 80C oil temp (minutes)
    ---------    ------------------------------
      100W             49.5 min (indefinite at ~35C steady state)
      250W             19.8 min
      500W              9.9 min
      750W              6.6 min
     1000W              5.0 min (exceeds 500W resistor rating!)

TABLE B-2. MAXIMUM CONTINUOUS OPERATION TIME (1kW OIL LOAD)

    Power (W)    Time to 80C oil temp (minutes)
    ---------    ------------------------------
      500W             16.5 min
      750W             11.0 min
     1000W              8.3 min (with no wall radiation)
     1000W             >30 min  (with 25W/C wall radiation, see Chapter 6)


================================================================================
APPENDIX C  SWR SPECIFICATIONS BY BAND
================================================================================

TABLE C-1. COMPLIANCE MATRIX BY BAND AND LOAD TYPE

    Band    Freq      DRY SWR   OIL SWR   VNA SWR    Band Edge
    ------  -------   -------   -------   -------    ---------
    160M    1.850MHz  <1.02:1   <1.02:1   <1.01:1    1.8-2.0 MHz
    80M     3.750MHz  <1.02:1   <1.02:1   <1.01:1    3.5-4.0 MHz
    40M     7.150MHz  <1.02:1   <1.02:1   <1.01:1    7.0-7.3 MHz
    30M    10.125MHz  <1.02:1   <1.02:1   <1.01:1    10.1-10.15 MHz
    20M    14.175MHz  <1.03:1   <1.03:1   <1.01:1    14.0-14.35 MHz
    17M    18.118MHz  <1.03:1   <1.03:1   <1.01:1    18.07-18.17 MHz
    15M    21.225MHz  <1.03:1   <1.04:1   <1.01:1    21.0-21.45 MHz
    12M    24.940MHz  <1.04:1   <1.04:1   <1.01:1    24.89-24.99 MHz
    10M    28.500MHz  <1.04:1   <1.05:1   <1.02:1    28.0-29.7 MHz
    6M     51.000MHz  <1.08:1   <1.20:1   <1.03:1    50-54 MHz
    2M    146.000MHz  <1.20:1   N/A       <1.05:1    144-148 MHz
    1.25M 222.000MHz  <1.40:1   N/A       <1.08:1    222-225 MHz
    70cm  440.000MHz  N/A       N/A       <1.12:1    420-450 MHz
    33cm  902.000MHz  N/A       N/A       <1.20:1    902-928 MHz
    23cm 1296.000MHz  N/A       N/A       <1.30:1    1240-1300 MHz


================================================================================
APPENDIX D  RESISTOR SELECTION GUIDE
================================================================================

D-1.  APPROVED NON-INDUCTIVE RESISTORS

    Part Number               Manufacturer   R (ohm)  Power   Package
    ------------------------  -----------    -------  -----   -------
    Ohmite 270-1-R200E        Ohmite         200      25W    Axial ceramic
    Ohmite 270-1-R500E        Ohmite         500      25W    Axial ceramic
    Ohmite RH-25E200R         Ohmite         200      25W    Axial ceramic
    Ohmite RH-50E500R         Ohmite         500      50W    Axial ceramic
    Ohmite RH-50E1K           Ohmite         1000     50W    Axial ceramic
    Ohmite RH-100E500R        Ohmite         500      100W   Axial ceramic
    TE Conn. HSC25R200J       TE Conn.       200      25W    Axial ceramic
    TE Conn. HSC50R500J       TE Conn.       500      50W    Axial ceramic
    Vishay RH050-1-R200J      Vishay         200      25W    Axial cermet
    Vishay FC0402E1000BTT1    Vishay         100      63mW   0402 SMD (VNA)
    Vishay FC0603E2000BT24    Vishay         200      100mW  0603 SMD (VNA)
    Susumu RG1005P-101-B-T5   Susumu         100      63mW   0402 SMD (VNA)

D-2.  RESISTOR DERATING

    All ceramic body resistors: derate 10%/1000m above sea level.
    In oil: full rated power acceptable (oil provides cooling).
    In air without heatsink: 10% of rated power only.
    With adequate heatsink + fan: full rated power.


================================================================================
APPENDIX E  OIL SPECIFICATIONS
================================================================================

E-1.  APPROVED OIL TYPES

    Type                 Source          Notes
    ---------------      ----------      ----------------------------------
    Naphthenic mineral   Electrical      Best dielectric, lowest cost in
    oil, Type II         distributor     bulk. Shell Diala, Chevron, etc.
    transformer oil

    Food-grade mineral   Pharmacy,       More expensive, readily available,
    oil                  food supply     identical electrical properties.

    White mineral oil    Industrial      Suitable. Verify flash point >130C.
    (technical grade)    distributor

E-2.  OIL NOT APPROVED

    Motor oil            (Sulfur additives degrade insulation)
    Synthetic lubricants (Flammable at high temperature)
    Cooking oil          (Polymerizes, forms deposits)
    Silicone oil         (High cost, but acceptable if used; do not mix types)
    Hydraulic fluid      (May contain flammable additives)
    Water/glycol         (Corrosion, poor dielectric strength)

E-3.  OIL PROPERTIES

    Property            Typical Mineral Oil    Minimum Required
    -----------------   -------------------    ----------------
    Dielectric strength > 30 kV/mm             > 15 kV/mm
    Flash point           135C                 > 130C
    Kinematic viscosity   9-12 cSt at 40C      No requirement
    Density               0.86 kg/L            N/A
    Specific heat         1.8 kJ/(kg*C)        N/A
    Thermal conductivity  0.13 W/(m*K)         N/A

E-4.  OIL DISPOSAL

    Spent mineral oil is classified as used petroleum product.
    Dispose via automotive oil recycling centers (accepted at most
    service stations and auto parts stores). Do not pour down drains.


================================================================================
APPENDIX F  CYD POWER MONITOR
================================================================================

F-1.  DESCRIPTION

    The CYD Power Monitor (cyd/power_monitor_cyd.ino) is an Arduino
    sketch for the ESP32-2432S028 module providing:
    - Forward power display (watts, average, peak)
    - Reflected power
    - SWR with color-coded PASS/WARN/FAIL indicator
    - Oil/heatsink temperature from NTC
    - Duty cycle percentage
    - Cumulative energy in watt-seconds or watt-hours
    - Fan speed control (PWM, temperature-proportional)
    - Over-temperature relay control

F-2.  HARDWARE CONNECTIONS

    Signal           CYD GPIO    Notes
    ------------     --------    -----------
    Forward power    GPIO39      Coupler FWD detector output
    Reflected power  GPIO36      Coupler REF detector output
    Temperature      GPIO35      10k NTC voltage divider
    Fan PWM          GPIO26      0-12V via MOSFET or fan driver
    Relay            GPIO27      Active HIGH = relay on (normal)
    LED red          GPIO4       Active LOW
    TFT backlight    GPIO21      PWM via ledcWrite

F-3.  CALIBRATION

    STEP 1. Apply known power (e.g., 10.0W from calibrated source).
    STEP 2. Touch and hold display for 1 second to enter calibration mode.
    STEP 3. Read displayed V_fwd value.
    STEP 4. Tap APPLY. CYD stores K = 10.0 / V_fwd^2 in EEPROM.
    STEP 5. Verify power reading matches source. Adjust if needed.

F-4.  ACCURACY

    Forward power: ±10% of reading (after calibration)
    Reflected power: ±15% of reading
    SWR:           ±0.1:1 (at SWR < 3:1)
    Temperature:   ±2C (NTC accuracy dominant)
    Duty cycle:    ±1% (timing based)


================================================================================
APPENDIX G  GLOSSARY
================================================================================

CANTENNA
    Colloquial name for the ARRL oil-cooled dummy load design using a
    surplus metal can as the oil container. Originated circa 1960s.

DERATING
    Reduction of rated parameter (power, temperature, frequency) to account
    for adverse operating conditions such as elevated ambient temperature,
    high altitude, or reduced cooling.

DIRECTIONAL COUPLER
    A 4-port device that samples forward and reflected power in a transmission
    line with a defined coupling factor and direction isolation.

DIRECTIVITY
    The ability of a directional coupler to distinguish forward from reflected
    power. Higher directivity = more accurate SWR measurement. > 20 dB needed.

DUTY CYCLE
    Ratio of transmit time to total time. 100% = continuous carrier.
    Lower duty cycle allows operation at power levels above continuous rating.

NON-INDUCTIVE RESISTOR
    A resistor constructed so that its internal current paths produce
    minimum magnetic flux, minimizing parasitic inductance. Required for
    RF dummy loads. Ceramic body resistors, thin-film SMD, and bifilar-wound
    ceramic types are typically non-inductive.

RETURN LOSS
    The ratio (in dB) of incident to reflected power at a port.
    High return loss (e.g., 30 dB) = low reflected power = good match.
    Return Loss (dB) = -20 * log10(|rho|).

RMSWR (rho)
    Reflection coefficient magnitude. rho = |V_reflected / V_incident|.
    rho = 0 = perfect match. rho = 1 = total reflection (open or short).

SOLT
    Short-Open-Load-Through calibration procedure for VNA. The Load standard
    is the precision 50-ohm dummy load described in Chapter 7.

SWR
    Standing Wave Ratio = (1 + rho) / (1 - rho). Indicates quality of
    impedance match. SWR = 1.0:1 is perfect match. SWR < 1.5:1 is good.

THERMAL RESISTANCE (theta)
    Rate of temperature rise per watt of heat input. Lower is better.
    Unit: degrees C per watt (C/W).

================================================================================
END OF TECHNICAL MANUAL TM-DL-001 REV A
================================================================================
