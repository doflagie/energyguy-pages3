================================================================================
TECHNICAL MANUAL
FREQUENCY COUNTER, PORTABLE, 0.1 MHz – 3 GHz
MODEL TM-FC-001   REVISION A
================================================================================

LIST OF EFFECTIVE PAGES

Chapter/Section                                        Page
-------------------------------------------------------
Title Page / List of Effective Pages .................  1
Chapter 1 — General Information .....................  2
Chapter 2 — Technical Description ...................  4
Chapter 3 — Operating Instructions ..................  12
Chapter 4 — GPSDO Discipline Procedure ..............  18
Chapter 5 — Timebase Calibration ....................  22
Chapter 6 — Remote Monitoring (WiFi / BLE) ..........  26
Chapter 7 — Maintenance .............................  28
Chapter 8 — Troubleshooting ........................  30
Chapter 9 — Parts List .............................  34
Appendix A — Frequency Resolution Tables ............  38
Appendix B — Pi Attenuator Reference ................  40
Appendix C — Low-Pass Filter Component Values .......  41
Appendix D — Reciprocal Counter Theory ..............  42
Appendix E — Prescaler Frequency Chart ..............  43
Appendix F — Glossary ..............................  44

================================================================================
CHAPTER 1 — GENERAL INFORMATION
================================================================================

1.1  SCOPE
-----------
This manual provides operating, calibration, and maintenance instructions for
the TM-FC-001 Portable Frequency Counter.  The counter covers 0.1 MHz to
3 GHz in three measurement paths:

  (a) DIRECT PATH:     ESP32 PCNT hardware; 0.1 MHz – 40 MHz; resolution to
                        0.01 Hz (10-second gate).

  (b) RECIPROCAL PATH: ESP32 timer capture; 0.001 Hz – 40 MHz; resolution
                        δf = f² / (N × f_ref); superior at <1 MHz.

  (c) PRESCALER PATH:  ERA-3SM + MB506 ÷8/÷64; 40 MHz – 3 GHz; resolution
                        equivalent to direct path divided by prescale factor.

The instrument is built around the CYD (ESP32-2432S028) module with a 2.8-inch
ILI9341 TFT touchscreen display.  An optional GPS-Disciplined Oscillator (GPSDO)
using a u-blox NEO-6M module and a 10 MHz TCXO provides laboratory-grade
long-term accuracy of better than 1 Hz at any frequency in the HF range.

1.2  INTENDED USE
------------------
  (a) VFO and transmitter frequency verification on all amateur radio bands.
  (b) Crystal and oscillator frequency measurement.
  (c) Signal generator calibration.
  (d) Prescaler-equipped VHF/UHF beacon and synthesizer verification.
  (e) Period measurement of low-frequency signals and pulse timing.

1.3  SPECIFICATIONS
--------------------

  Parameter                    Specification
  ---------                    -------------
  Input frequency range        0.1 MHz – 3 GHz
  Direct-count range           0.1 – 40 MHz
  Prescaler range (÷8)         40 – 600 MHz (displayed × 8)
  Prescaler range (÷64)        600 – 3000 MHz (displayed × 64)
  Reciprocal range             0.001 Hz – 40 MHz (period mode)
  Direct count resolution      1/gate_time (0.01 Hz at 10s gate)
  Reciprocal resolution        f² / (N × f_ref)  (see Appendix A)
  Timebase (TCXO, Config A)    ±0.5 ppm (–10 to +70°C)
  Timebase (OCXO, Config B)    ±0.01 ppm (after 20-min warm-up)
  Timebase (GPSDO, Config C)   <1 ppb long-term (GPS-disciplined)
  Input impedance               50 Ω (default) / Hi-Z / 75 Ω (switch)
  Input protection              BAT54S dual Schottky; ±3.3V clamp
  Maximum safe input level      +20 dBm continuous (100 mW)
  Attenuator steps              0 / −10 / −20 / −30 dB
  Low-pass filter positions     Flat, 60 MHz, 200 MHz, 600 MHz, 1500 MHz, 3 GHz
  Gate times                    100 ms / 1 s / 10 s / 100 s
  Display                       2.8-inch TFT ILI9341, 320 × 240 pixels, touch
  Display frequency digits      Up to 12 significant digits (Hz shown as MHz/GHz)
  WiFi                          802.11 b/g/n AP; TCP port 7373; HTTP /freq
  Bluetooth                     BLE 4.2; service FFF0; notify characteristic FFF1
  GPS input                     u-blox NEO-6M; SMA J5; 3.3V UART 9600 baud
  1 PPS accuracy                GPS UTC ±100 ns (NEO-6M typical)
  Supply voltage                7.5 V – 12 V DC; or 2 × 9 V batteries
  Current draw                  ~180 mA typical (CYD + TCXO + GPSDO active)
  Battery life (2 × 9 V)        ~5 hours (TCXO only, GPS off)
  Operating temperature         0 to +50°C
  Enclosure                     ASA 3D printed; 170 × 100 × 52 mm

1.4  CONTROLS AND INDICATORS
------------------------------

  FRONT PANEL:
    J1   SMA IN (MAIN)    Main frequency input, direct/reciprocal path
    J2   SMA IN (PRESC)   Prescaler path input (60 MHz – 3 GHz)
    S1   ATTN             Pi attenuator rotary switch (0/10/20/30 dB)
    S2   LPF              Low-pass filter selection rotary switch
    S3   AMP              ERA-3SM preamplifier bypass/engage toggle

  LID:
    CYD  2.8-inch TFT touchscreen display (all controls and readings)
    LED  Gate indicator LED (illuminates during counting interval)
    S4   GATE TIME        Gate time selection rotary (100ms/1s/10s/100s)
    S5   MODE             Measurement mode rotary (DIRECT/PRESC/RECIP/PERIOD)

  REAR PANEL:
    J4   SMA OUT (10 MHz) 10 MHz reference output (TCXO/GPSDO)
    J5   SMA GPS          GPS antenna (NEO-6M; active patch or external)
    SW1  POWER            Rocker power switch
    P1   USB-C            USB power input (5 V → internal boost to 9 V)

1.5  ASSOCIATED DOCUMENTS
--------------------------
  schematics/sch_direct_count.txt       Direct-count PCNT chain
  schematics/sch_prescaler.txt          ERA-3SM + MB506 prescaler chain
  schematics/sch_timebase_gpsdo.txt     TCXO/OCXO/GPSDO timebase
  schematics/sch_input_protection.txt   Input protection and conditioning
  firmware/gpsdo.h                      GPSDO firmware module
  firmware/reciprocal_counter.h         Reciprocal counter firmware module
  firmware/freq_counter_cyd.ino         Main CYD firmware

================================================================================
CHAPTER 2 — TECHNICAL DESCRIPTION
================================================================================

2.1  SYSTEM OVERVIEW
---------------------
The TM-FC-001 uses three complementary measurement techniques to cover
0.1 MHz through 3 GHz.  A shared input conditioning chain feeds either the
direct/reciprocal path or the prescaler path based on the input frequency.

  INPUT SIGNAL
       │
  [BAT54S ESD clamp]
       │
  [Pi Attenuator S1: 0/−10/−20/−30 dB]
       │
  [LP Filter Bank S2: flat/60/200/600/1500/3000 MHz]
       │
  [ERA-3SM MMIC +20 dB, optional S3]
       │
  [LMV7219 Comparator → 74LVC14A Schmitt]
       │
       ├──── GPIO34 ─── DIRECT COUNT (PCNT_UNIT_0)  0.1–40 MHz
       │
       └──── GPIO35 ─── PRESCALER PATH (MB506 ÷8/÷64) → up to 3 GHz

2.2  DIRECT COUNT (PCNT GATE METHOD)
--------------------------------------
The ESP32 PCNT unit counts rising edges of the conditioned input signal
during a precise gate interval generated by esp_timer.

  Gate interval: set by S4 (100 ms / 1 s / 10 s / 100 s)

  Frequency = total_count / gate_time_seconds

  PCNT is a 16-bit signed counter (max +32,767).  At 40 MHz input, it
  overflows every 820 µs.  The firmware sets H_LIMIT to 30,000 counts;
  an overflow ISR increments a 64-bit accumulator, extending range without
  limit.

  At gate close:
    total = accum + pcnt_remainder
    f = total / gate_time × prescale_factor

  Resolution in direct mode:
    Gate 100 ms → 10 Hz resolution
    Gate 1 s    → 1 Hz resolution
    Gate 10 s   → 0.1 Hz resolution
    Gate 100 s  → 0.01 Hz resolution

  NOTE: Resolution equals gate_time⁻¹ regardless of input frequency.
  For <1 MHz work, use RECIPROCAL mode.

2.3  RECIPROCAL COUNTER METHOD
--------------------------------
The reciprocal counter measures the exact time required for N complete input
cycles, then computes frequency = N / elapsed_time.

  elapsed_time = esp_timer ticks × (1 / 80 MHz) × GPSDO_correction

  This yields resolution:
    δf = f_input² / (N × f_reference)

  Example at 14.175 MHz, N = 10,000, f_ref = 10 MHz:
    δf = (14.175 × 10⁶)² / (10,000 × 10 × 10⁶) ≈ 2 Hz

  Example at 1 kHz, N = 1,000, f_ref = 10 MHz:
    δf = (1,000)² / (1,000 × 10 × 10⁶) = 0.0001 Hz = 0.1 mHz

  In AUTO mode, firmware selects N automatically:
    f < 100 Hz:   N = 1  (single period, avoid multi-second wait)
    f < 1 kHz:    N = 100
    f < 10 kHz:   N = 1,000
    f < 100 kHz:  N = 10,000
    f < 1 MHz:    N = 100,000
    f > 1 MHz:    N = 1,000,000  (gate ≈ 70 ms at 14 MHz)

  The timer reference is the ESP32 APB clock (80 MHz).  Without GPSDO,
  APB accuracy is ±25 ppm.  With GPSDO locked, the correction factor
  brings accuracy to <1 ppb long-term.

2.4  PRESCALER PATH (40 MHz – 3 GHz)
---------------------------------------
The ERA-3SM MMIC preamplifier (+20 dB) drives the MB506 dual-modulus
prescaler.  Prescale ratio:

  MC pin LOW  (GPIO0 = 0): ÷8   → output = f_input / 8
  MC pin HIGH (GPIO0 = 1): ÷64  → output = f_input / 64

  Displayed frequency = PCNT_count × prescale_factor / gate_time

  Band selection (automatic, or set manually via MODE rotary):
    40–600 MHz:    ÷8 mode selected
    600–3000 MHz:  ÷64 mode selected

  NOTE: ERA-3SM requires 5 V supply.  When counter is powered from USB (5 V
  boost), ERA-3SM bias is supplied at correct voltage.  On 9 V battery: bias
  resistor R3 = 75 Ω limits ERA-3SM current to ~65 mA.

  MAXIMUM PRESCALER INPUT: +5 dBm into MB506 FIN+.  Use attenuator S1 to
  reduce signal level.  At full power from a transceiver (+50 dBm), use
  external 40 dB attenuator before counter input.

2.5  TIMEBASE OPTIONS
----------------------
Three configurations provide different accuracy levels:

  CONFIG A — TCXO ONLY
    FXO-HC73R-10 10 MHz TCXO (or equivalent ±0.5 ppm).
    VC (voltage control) pin driven by ESP32 GPIO25 DAC (0–3.3 V).
    Center frequency at DAC = 128 (1.65 V).
    Pull range: ±2.5 ppm over full DAC range (0–3.3 V).
    Immediate operation; no warm-up.
    Accuracy: ±0.5 ppm = ±7 Hz at 14.175 MHz.

  CONFIG B — OCXO
    Isotemp OCXO-131C 10 MHz (or equivalent ±0.01 ppm).
    Requires 12 V supply; warm-up 10–20 minutes.
    EFC input driven by ESP32 DAC → op-amp ×3 → 0–5 V.
    Accuracy: ±0.01 ppm = ±0.14 Hz at 14.175 MHz.

  CONFIG C — GPSDO (GPS-Disciplined Oscillator)
    Combines TCXO or OCXO with GPS 1 PPS discipline.
    Long-term accuracy: <1 ppb (GPS UTC ±100 ns typical).
    30–60 minutes required for initial discipline lock.
    See Chapter 4 for complete discipline procedure.

2.6  GPSDO ALGORITHM
----------------------
The GPSDO firmware (firmware/gpsdo.h) counts 10 MHz reference ticks
(on GPIO39, PCNT_UNIT_2) between consecutive GPS 1 PPS edges (GPIO26).

Each second (on PPS rising edge):
  1. Read accumulated tick count since last PPS.
  2. Compute error:
       ε_hz   = ticks − 10,000,000          (Hz error)
       ε_ppm  = ε_hz / 10                   (ppm error)
       ε_ns   = ε_hz × 100                  (phase error, ns)
  3. Apply PI controller:
       proportional = Kp × ε_ns      (Kp = 0.0002)
       integral    += Ki × ε_ns      (Ki = 0.00002)
       integral     = clamp(integral, DAC_MIN, DAC_MAX)
       control      = DAC_CENTER + proportional + integral − DAC_CENTER
  4. Write new DAC value (GPIO25 → TCXO VC pin).
  5. Lock detection: |ε_ns| < 200 for 30 consecutive seconds → LOCKED.
  6. Holdover: GPS signal absent → freeze DAC, set HOLDOVER status.

  Lock indicator colors on GPSDO screen:
    GREEN:  LOCKED (within 200 ns, 30 consecutive PPS)
    YELLOW: LOCKING (acquiring lock; percentage shown)
    ORANGE: HOLDOVER (GPS signal lost; last DAC held)
    GRAY:   GPS WAIT (no NMEA data received)

2.7  INPUT PROTECTION CHAIN
-----------------------------
  Stage 1: BAT54S dual Schottky diode clamp.
    Clamps input to –0.7 V / +3.3 V.
    Protects all downstream components from electrostatic discharge
    and brief RF overloads.

  WARNING: Input power above +20 dBm (100 mW) continuous will damage
  the BAT54S diodes and possibly the LVC14A.  Always start with maximum
  attenuation (−30 dB) for unknown signal sources.

  Stage 2: Pi attenuator bank (S1, 4 positions).
    Position 0: Bypass (0 dB)
    Position 1: −10 dB
    Position 2: −20 dB
    Position 3: −30 dB
    Use E96 series 1% thin-film resistors.

  Stage 3: Low-pass filter bank (S2, 6 positions).
    Rejects harmonics and aliasing above selected band.
    Position 1: Flat (no filter)
    Position 2: 60 MHz   (for HF/low VHF work)
    Position 3: 200 MHz  (for 2M and below)
    Position 4: 600 MHz  (for 70cm and below)
    Position 5: 1500 MHz (for 23cm/L-band)
    Position 6: 3 GHz    (full-range; ERA-3SM limits bandwidth)

  Stage 4: ERA-3SM preamplifier (S3, optional).
    +20 dB gain; use for signals weaker than −34 dBm.
    Bypass ERA-3SM for signals above −10 dBm (prevents LMV7219 saturation).

  Stage 5: LMV7219 comparator (sine/low-level to clean logic transition).
    Threshold: Vcc/2 = 1.65 V (10 kΩ voltage divider).
    Maximum toggle rate: ~70 MHz.
    For >70 MHz: signal goes directly to prescaler (MB506 handles it).

  Stage 6: 74LVC14A Schmitt trigger inverter (×2 inversions).
    Adds ~0.9 V hysteresis.  Produces clean 3.3 V CMOS edge.
    Output to GPIO34 (direct path) or GPIO35 (prescaler path output).

2.8  CYD DISPLAY — SCREEN DESCRIPTIONS
----------------------------------------
  FREQ screen (main):
    Large frequency readout with prefix (Hz/kHz/MHz/GHz).
    Mode badge: DIRECT / PRESCALE ÷N / RECIPROCAL / PERIOD.
    Gate time or N indicator.
    Attenuator setting badge.
    GPSDO status line (LOCKED / LOCKING XX% / HOLDOVER / GPS WAIT).
    Resolution and accuracy estimate.
    Period (reciprocal of displayed frequency).
    Session statistics: count and mean.
    HOLD / RESET STATS / START touch buttons.

  PERIOD screen:
    Period displayed in appropriate units (ns / µs / ms / s).
    N cycles measured; equivalent frequency.
    Resolution in Hz.

  SETTINGS screen:
    Mode (DIRECT / PRESCALE / RECIPROCAL / PERIOD).
    Gate time (100 ms / 1 s / 10 s / 100 s).
    Attenuator (0 / −10 / −20 / −30 dB).
    N cycles (10 / 100 / 1000 / 10000 / 100000 / AUTO).
    WiFi AP on/off; stream on/off.
    TCXO DAC value (diagnostic).

  GPSDO screen:
    Frequency error (Hz and ppb).
    Phase error (ns).
    Accumulated phase (ns).
    DAC value and lock counter progress.
    GPS time (UTC), satellite count, fix status.
    Session statistics: mean/min/max ppb error.

  NETWORK screen:
    WiFi AP SSID and password.
    IP address.
    TCP and HTTP endpoint descriptions.
    BLE advertising/connection status.
    Stream enable/disable toggle.

  STATS screen:
    Minimum, maximum, mean, standard deviation (session).
    Frequency spread in ppm.
    GPSDO accuracy estimate.
    RESET button.

2.9  WIFI AND BLUETOOTH
------------------------
  WiFi Access Point:
    SSID:     FreqCounter
    Password: 10MHz0001
    IP:       192.168.4.1 (default SoftAP address)
    TCP port 7373: raw text stream; one line per measurement.
      Format: <freq_hz>,<freq_str>\n
    HTTP GET http://192.168.4.1/freq: text/plain key=value response.
      Fields: freq_hz, freq_str, locked, accuracy_ppb.

  Bluetooth Low Energy:
    Device name:   FreqCounter
    Service UUID:  0000FFF0-0000-1000-8000-00805F9B34FB
    Characteristic FFF1: NOTIFY; value = freq_hz\n (UTF-8)
    Clients subscribe to notifications; counter sends each new measurement.

================================================================================
CHAPTER 3 — OPERATING INSTRUCTIONS
================================================================================

3.1  INITIAL POWER-ON
-----------------------
  1. Set attenuator S1 to maximum attenuation (−30 dB) before connecting
     any unknown signal source.

  2. Set filter S2 to FLAT (position 1) for initial setup.

  3. Set preamplifier S3 to BYPASS.

  4. Set MODE rotary S5 to DIRECT or allow firmware auto-selection.

  5. Connect 9 V battery or USB-C power supply.

  6. Set power switch SW1 to ON.

  7. The CYD display illuminates; startup screen shows "FC-001 Rev A /
     Initializing..." for approximately 1 second.

  8. Main FREQ screen appears.  GPSDO status shows "GPS WAIT" until GPS
     receiver acquires satellites (typically 1–5 minutes with clear sky view).

3.2  MEASURING A SIGNAL — DIRECT MODE
----------------------------------------
  Use for signals from 0.1 MHz to 40 MHz (HF, low VHF).

  1. Connect signal source to J1 (SMA MAIN INPUT).

  2. Set attenuator appropriate to signal level:
       −40 dBm to −20 dBm: 0 dB; ERA-3SM IN (if available).
       −20 dBm to 0 dBm:   0 dB; ERA-3SM bypass.
       0 dBm to +10 dBm:   −10 dB; ERA-3SM bypass.
       +10 dBm to +20 dBm: −20 dB.
       Above +20 dBm:      −30 dB (do not exceed without external attenuator).

  3. Set LP filter S2 to appropriate band:
       1.8–30 MHz (HF bands): position 2 (60 MHz cutoff).
       50–54 MHz (6M):        position 2 or 3.

  4. Touch SETTINGS tab.  Set Mode = DIRECT.  Set Gate Time as desired.
     Touch FREQ tab to return to main display.

  5. The counter begins measuring automatically.  Gate LED illuminates
     during each gate interval.

  6. Frequency is displayed on FREQ screen.

3.3  MEASURING A SIGNAL — RECIPROCAL MODE (HIGH RESOLUTION)
-------------------------------------------------------------
  Use for frequencies below 1 MHz, or when maximum resolution is required.

  1. Connect signal to J1.  Set attenuator and filter as in §3.2.

  2. In SETTINGS, set Mode = RECIPROCAL.  Set N = AUTO for automatic
     cycle-count selection.

  3. Return to FREQ screen.  The display shows resolution below the
     frequency readout (e.g., "±0.001 Hz").

  NOTE: For signals below 10 Hz, a single-period measurement may take
  more than 1 second.  The counter waits up to 5 seconds before timeout.
  Set N = 1 (SINGLE PERIOD) manually for very low frequencies.

3.4  MEASURING A SIGNAL — PERIOD MODE
----------------------------------------
  Use to display the period of a signal rather than its frequency.

  1. In SETTINGS, set Mode = PERIOD.

  2. PERIOD screen shows the period in appropriate units:
       T = 71.1 ns  (at 14.075 MHz)
       T = 500 µs   (at 2 kHz)
       T = 100 ms   (at 10 Hz)

  3. Resolution is the same as reciprocal mode.

3.5  MEASURING VHF/UHF/MICROWAVE — PRESCALER MODE
----------------------------------------------------
  Use for signals from 40 MHz to 3 GHz.

  1. Connect signal to J2 (SMA PRESC INPUT).

  2. Set attenuator S1 as appropriate.

  3. Set LP filter S2 to:
       40–200 MHz:   position 3 (200 MHz) or FLAT.
       200–600 MHz:  position 4 (600 MHz) or FLAT.
       600–1500 MHz: position 5 (1500 MHz) or FLAT.
       1.5–3 GHz:    position 6 (3 GHz).

  4. In SETTINGS, set Mode = PRESCALE.

  5. Prescale ratio is set automatically by firmware:
       < 600 MHz:  ÷8 (MC pin LOW)
       ≥ 600 MHz:  ÷64 (MC pin HIGH)

  6. Displayed frequency is PCNT count × prescale factor / gate time.

  CAUTION: At 3 GHz, ensure ERA-3SM preamplifier (S3) is set to IN.
  The MB506 requires a minimum input level of −5 dBm.  Use S3 IN unless
  signal is stronger than +5 dBm.

3.6  HOLD FUNCTION
-------------------
  Touch the HOLD area on FREQ screen to freeze the current reading.
  The display shows "HOLD" badge.  Measurements continue internally
  (statistics update) but the display does not change.
  Touch HOLD again to release.

3.7  STATISTICS
----------------
  Session statistics (min, max, mean, std dev) accumulate automatically.
  View on STATS tab.

  To reset statistics:
    Touch STATS tab.
    Touch [RESET] button at bottom of screen.

  Statistics reset automatically when mode or gate time changes.

3.8  DATA LOGGING VIA WIFI
----------------------------
  1. Enable WiFi in SETTINGS (touch WiFi row to toggle).
  2. Connect a PC or phone to WiFi SSID "FreqCounter" (password: 10MHz0001).
  3. Open a terminal and type:
       nc 192.168.4.1 7373
     Every measurement appears as one line:
       14175432.567,14.175432567 MHz
  4. To fetch a single reading via HTTP:
       curl http://192.168.4.1/freq
     Response:
       freq_hz=14175432.567
       freq_str=14.175432567 MHz
       locked=1
       accuracy_ppb=0.4

  5. To log to a file:
       nc 192.168.4.1 7373 >> frequency_log.txt

3.9  DATA LOGGING VIA BLUETOOTH
---------------------------------
  1. Enable BLE in SETTINGS.
  2. On a phone or PC, scan for BLE device "FreqCounter".
  3. Connect and subscribe to notifications on characteristic FFF1
     (UUID 0000FFF1-0000-1000-8000-00805F9B34FB).
  4. Each new measurement is sent as a notify event:
       Value: "14175432.567\n"
  5. Any BLE GATT client application (e.g., nRF Connect) can display
     and log the values.

================================================================================
CHAPTER 4 — GPSDO DISCIPLINE PROCEDURE
================================================================================

4.1  OVERVIEW
--------------
The GPSDO (GPS-Disciplined Oscillator) function disciplines the on-board
10 MHz TCXO to GPS UTC accuracy using a software PI control loop.  Initial
acquisition requires 30–60 minutes.  Once locked, the counter maintains
<1 ppb long-term frequency accuracy.

4.2  EQUIPMENT REQUIRED
------------------------
  (a) TM-FC-001 with Config C GPSDO option installed.
  (b) u-blox NEO-6M GPS module (on-board) with active patch antenna.
  (c) Clear sky view or window with GPS satellite visibility.
  (d) 30–60 minutes undisturbed operating time for initial lock.

4.3  GPSDO DISCIPLINE PROCEDURE — INITIAL LOCK
-----------------------------------------------
  STEP 1 — ANTENNA PLACEMENT
    Connect GPS patch antenna to SMA J5 (GPS ANT).
    Place antenna at window or outdoors with unobstructed sky view.
    Active patch antennas (5 V, internal LNA) are preferred.
    Avoid metal objects within 10 cm of antenna face.

  STEP 2 — GPS ACQUISITION
    Power on counter.  Touch GPSDO tab.
    Status reads "GPS WAIT" until NMEA data is received.
    Status reads "GPS WAIT" → "GPS WAIT (sats: N)" as satellites appear.
    Wait for "GPS WAIT → LOCKING" — fix acquired, discipline beginning.
    GPS fix indicated by green satellite count and UTC time displayed.

    NOTE: Cold start acquisition takes 1–5 minutes.  Warm start (same
    location within 4 hours) takes 30–60 seconds.

  STEP 3 — DISCIPLINE MONITORING
    Status changes to "LOCKING XX%" where XX is lock_counter / 30 × 100%.
    Watch phase_error_ns decrease toward zero as PI loop converges.
    Typical convergence:
      First 5 minutes:   phase error may be ±10,000 ns (TCXO drift)
      5–20 minutes:      phase error decaying toward ±500 ns
      20–60 minutes:     phase error settling to ±200 ns (lock threshold)

    Status reaches "LOCKED" when |phase_error| < 200 ns for 30 consecutive
    one-second intervals (30 consecutive 1 PPS edges in lock band).

  STEP 4 — LOCKED OPERATION
    LOCKED status shown in GREEN on GPSDO screen.
    Accuracy estimate shown as "±X.X ppb".
    Counter applies gpsdo_correction() factor to all frequency measurements.
    10 MHz reference output on SMA J4 is now GPS-disciplined.

  NOTE: Lock is maintained as long as GPS signal is present.  Momentary
  GPS dropouts (< 30 seconds) do not cause unlock; integrator holds DAC.

4.4  HOLDOVER
--------------
  If GPS signal is lost for more than ~5 seconds (no valid NMEA):
    Status changes to "HOLDOVER".
    DAC value is frozen at last disciplined value.
    TCXO continues at last corrected frequency.
    Holdover accuracy: ±0.5 ppm/hr typical for TCXO Config A.

  Holdover time and drift:
    Time       TCXO drift (±0.5 ppm/hr)    OCXO drift (±0.01 ppm/hr)
    1 min       ±0.008 ppm                   ±0.00017 ppm
    10 min      ±0.083 ppm                   ±0.0017 ppm
    1 hour      ±0.5 ppm                     ±0.01 ppm

  When GPS signal is restored, GPSDO resumes discipline automatically.
  Re-lock is faster than initial lock (integrator state preserved).

4.5  GPSDO PI LOOP TUNING
--------------------------
  Default parameters:
    Kp = 0.0002   (proportional gain)
    Ki = 0.00002  (integral gain)

  If the loop is unstable (oscillating DAC value):
    Reduce Kp by factor of 2 (set to 0.0001).
    If still unstable, reduce Ki by factor of 2 (set to 0.00001).

  If the loop converges too slowly (phase error persists > 60 min):
    Increase Kp by 50% (set to 0.0003).

  Loop bandwidth: approximately 0.01 Hz (100-second time constant).
  This is intentionally slow to reject GPS 1 PPS jitter (~30–100 ns RMS).

  CAUTION: Increasing Kp above 0.001 may cause oscillation.
  The TCXO VC pull sensitivity is approximately 1 ppm/V (10 Hz/V at 10 MHz).
  Full DAC range (0–3.3 V) provides only ±5 ppm pull range.

4.6  GPSDO STATISTICS INTERPRETATION
--------------------------------------
  GPSDO screen shows:
    Freq error:    instantaneous error vs 10 MHz nominal (ppb)
    Phase error:   instantaneous phase difference vs GPS PPS (ns)
    Phase accum:   accumulated phase over session (ns); should stay near 0
    DAC value:     0–255 (128 = center = 1.65 V = nominal TCXO frequency)
    Mean ppb:      running mean over session (should approach 0 when locked)
    Min/Max ppb:   session extremes

  Typical locked values:
    Freq error:   ±1 to ±5 ppb
    Phase error:  ±30 to ±100 ns
    DAC value:    slightly offset from 128 (compensates TCXO aging drift)

================================================================================
CHAPTER 5 — TIMEBASE CALIBRATION
================================================================================

5.1  PURPOSE
-------------
Timebase calibration verifies that the counter is within specified accuracy
and adjusts the TCXO voltage-control (VC) DAC center value to place the
oscillator at its nominal 10 MHz frequency.

5.2  CALIBRATION WITHOUT GPSDO (TCXO CONFIG A)
------------------------------------------------
  Equipment required:
    (a) Known-accurate frequency standard (GPSDO, rubidium, or GPS receiver
        1 PPS with time-base comparison).
    (b) Alternatively: CHU or WWV at 5, 10, or 15 MHz; receive on AM receiver
        and use the counter in audio mode to compare.

  Procedure:
    STEP 1: Allow counter to warm up for 10 minutes.

    STEP 2: Connect a 10 MHz reference signal to J1.  Set S1 to 0 dB.
            Set S2 to FLAT.  Set S3 to BYPASS.  Set MODE to DIRECT, GATE 10s.

    STEP 3: Note displayed frequency.  Calculate error:
              error_ppm = (displayed − 10,000,000) / 10.0

    STEP 4: If |error_ppm| > 0.5, adjust TCXO VC voltage:
              In SETTINGS screen, note "TCXO DAC: XXX / 255".
              Increment or decrement DAC in firmware (edit DAC_CENTER in
              gpsdo.h) and reflash, or use calibration mode if implemented.
              Each DAC step (~13 mV) ≈ 0.013 ppm frequency change at typical
              TCXO sensitivity of 1 ppm/V.

    STEP 5: After adjusting, re-measure.  Iterate until |error| < 0.5 ppm.

    STEP 6: Record calibration value in logbook.  TCXO aging is typically
            <1 ppm/year; annual calibration is adequate.

5.3  CALIBRATION WITH GPSDO (CONFIG C)
-----------------------------------------
  The GPSDO self-calibrates continuously.  No manual calibration is required
  once the GPSDO is locked.

  Verification procedure:
    STEP 1: Allow GPSDO to lock (30–60 minutes; status = LOCKED).

    STEP 2: Connect a calibrated 10 MHz reference to J1 (from another
            GPSDO, rubidium, or cesium standard if available).

    STEP 3: Set MODE to DIRECT, GATE 100s.  Read displayed frequency.

    STEP 4: Expected reading: 10,000,000.00 ± 0.01 Hz at 100s gate
            (±1 ppb × 10 MHz = ±0.01 Hz).

    STEP 5: If error > 0.01 Hz, check GPS antenna placement, satellite
            count, and phase_error_ns on GPSDO screen.

5.4  FREQUENCY MEASUREMENT ACCURACY VERIFICATION
--------------------------------------------------
  For each measurement mode, verify accuracy using a known reference:

  Direct mode, 1s gate, 10 MHz reference:
    Acceptance criterion: displayed value within ±1 Hz of 10,000,000 Hz.

  Reciprocal mode, N=10000, 10 MHz reference:
    Acceptance criterion: displayed value within ±2 Hz of 10,000,000 Hz.

  Prescaler mode ÷64, 1 GHz reference:
    Acceptance criterion: displayed value within ±1 kHz of reference.
    (1 kHz = 1 ppm at 1 GHz; limited by TCXO accuracy without GPSDO.)

5.5  REFERENCE OUTPUT VERIFICATION
------------------------------------
  The 10 MHz reference output on SMA J4 should read:

    TCXO Config A: 10,000,000 ± 5 Hz (±0.5 ppm)
    OCXO Config B: 10,000,000 ± 0.1 Hz (±0.01 ppm, after warm-up)
    GPSDO Config C: 10,000,000 ± 0.01 Hz (±1 ppb, when locked)

  Use a higher-accuracy counter or spectrum analyzer with frequency counter
  function to verify the reference output.  The output level is approximately
  +3 dBm into 50 Ω from the 74HC04 inverter + 51 Ω series resistor.

================================================================================
CHAPTER 6 — REMOTE MONITORING (WiFi / BLE)
================================================================================

6.1  WIFI ACCESS POINT MODE
-----------------------------
  The counter creates a WiFi access point (SoftAP).  Up to 4 clients can
  connect simultaneously.

  Connection details:
    SSID:     FreqCounter
    Password: 10MHz0001
    Security: WPA2-PSK
    IP:       192.168.4.1

  TCP stream (port 7373):
    Line format: <freq_hz>,<display_string>\n
    Example: 14175432.567,14.175432567 MHz\n
    One line per completed measurement.  Update rate varies with gate time.
    Gate 1s: ~1 line/second.  Gate 10s: ~1 line/10 seconds.

  HTTP API (port 80):
    GET /freq → text/plain response:
      freq_hz=14175432.567
      freq_str=14.175432567 MHz
      locked=1
      accuracy_ppb=0.4

  Logging example (Linux/Mac):
    nc 192.168.4.1 7373 | ts >> /tmp/freq_log.txt

  Python client example:
    import socket
    s = socket.socket(); s.connect(('192.168.4.1', 7373))
    for line in s.makefile():
        freq_hz = float(line.split(',')[0])
        print(f"f = {freq_hz/1e6:.9f} MHz")

6.2  BLUETOOTH LOW ENERGY
---------------------------
  The counter advertises as "FreqCounter" using BLE 4.2.

  GATT Profile:
    Service:    0000FFF0-0000-1000-8000-00805F9B34FB
    Characteristic FFF1:
      Properties: READ, NOTIFY
      Format:     UTF-8 string, freq_hz\n (e.g., "14175432.567\n")

  To read with nRF Connect (Android/iOS):
    1. Scan → connect to "FreqCounter".
    2. Expand service FFF0.
    3. Subscribe to notifications on characteristic FFF1.
    4. Frequency updates appear for each new measurement.

  To read with Python (bluepy):
    from bluepy.btle import Peripheral, UUID
    p = Peripheral("FC:xx:xx:xx:xx:xx", "random")
    ch = p.getCharacteristics(uuid=UUID(0xFFF1))[0]
    print(ch.read().decode())

6.3  SIMULTANEOUS CLIENTS
---------------------------
  WiFi TCP and BLE streams operate simultaneously.
  One WiFi TCP client receives the stream; additional clients connecting
  receive HTTP responses only.
  If TCP client disconnects, the next client is accepted.

================================================================================
CHAPTER 7 — MAINTENANCE
================================================================================

7.1  ROUTINE MAINTENANCE SCHEDULE
-----------------------------------
  Interval      Action
  --------      ------
  After each use  Inspect SMA connectors for bent center pins.
  Monthly         Verify input protection diodes (see §7.2).
  Annually        Frequency accuracy check (see §5.2 or §5.3).
  As needed       Reflash firmware for bug fixes and improvements.

7.2  INPUT PROTECTION DIODE CHECK
------------------------------------
  Equipment: Multimeter on diode test setting.

  STEP 1: Disconnect all cables.

  STEP 2: With meter leads (red = +), probe SMA center pin (+) to ground.
          Expected: 0.25–0.40 V (BAT54S Schottky forward voltage).
          If open: diode is blown; replace BAT54S (SOT-23).
          If 0.0 V: diode is shorted; replace BAT54S.

  STEP 3: Reverse probes (black = + to SMA center pin).
          Expected: 0.25–0.40 V (clamping to GND through lower diode).

  STEP 4: If either reading is abnormal, remove and replace D1 (BAT54S).

  NOTE: BAT54S is a SOT-23 package.  Pin 1 = cathode of D1a.  Pin 2 = anode
  of D1b.  Pin 3 = common anode of D1a / cathode of D1b.

7.3  BATTERY MAINTENANCE
-------------------------
  9V alkaline batteries: replace when voltage drops below 7.0 V.
  Voltage is displayed in serial debug output (115200 baud, GPIO1/3).
  A low-battery warning is not yet implemented in CYD firmware (future rev).

  CAUTION: Do not mix old and new batteries.  Do not use rechargeable 9V
  (8.4 V) with prescaler ERA-3SM bias circuit without recalculating R3.

7.4  FIRMWARE UPDATE
----------------------
  STEP 1: Install Arduino IDE with ESP32 board support.
          Add TFT_eSPI, BLEDevice, and Preferences libraries.

  STEP 2: Open firmware/freq_counter_cyd.ino.

  STEP 3: Configure User_Setup.h for CYD (ILI9341, HSPI, CYD pin map).

  STEP 4: Connect counter via USB-C.  Hold BOOT button during power-on
          to enter download mode (if required by board variant).

  STEP 5: Select Board = "ESP32 Dev Module", Port = /dev/ttyUSBx or COMx.

  STEP 6: Upload.  Firmware preserves EEPROM settings (Preferences NVS).

7.5  ENCLOSURE MAINTENANCE
----------------------------
  The ASA enclosure is UV and temperature resistant.  Clean with mild soap
  and water.  Do not use acetone or other ketone solvents on ASA.
  Acetone will melt ASA surfaces.

  SMA connector threads: apply one small drop of Loctite 222 (low-strength,
  removable) to front-panel SMA nuts after assembly to prevent loosening
  from vibration.

================================================================================
CHAPTER 8 — TROUBLESHOOTING
================================================================================

8.1  DISPLAY SHOWS CONSTANT INCORRECT FREQUENCY
-------------------------------------------------
  Symptom: Frequency does not change when signal is removed.
  Likely cause: PCNT input is picking up noise from ESP32 internal clock,
               USB, or WiFi RF.
  Action 1: Enable LP filter S2 (select appropriate band).
  Action 2: Verify 74LVC14A input is not floating; check C1 (100 pF) and
            pull-down to input pin on 74LVC14A.
  Action 3: Verify GPIO34 does not have internal pull-up enabled in firmware.
            (The firmware configures GPIO34 as input, no pull.)

8.2  DISPLAY SHOWS ZERO OR NO READING
---------------------------------------
  Symptom: Counter reads 0.000000 Hz or dashes for every gate.
  Check 1: Verify signal is connected to J1 or J2.
  Check 2: Verify signal level.  For weak signals (< −30 dBm): engage ERA-3SM
           (S3 = IN).  For signals < −40 dBm: ERA-3SM cannot provide
           adequate level; use external preamp or move antenna closer.
  Check 3: Verify 74LVC14A power (3.3 V).  Measure pin 14; expect 3.3 V ± 0.2 V.
  Check 4: Verify BAT54S clamp is not shorted (§7.2).
  Check 5: In SETTINGS, verify Mode is set correctly for input frequency.

8.3  PRESCALER READS WRONG FREQUENCY (FACTOR-OF-8 OR FACTOR-OF-64 ERROR)
--------------------------------------------------------------------------
  Symptom: Displayed frequency is exactly 1/8 or 1/64 of expected value.
  Cause: Prescale ratio is set incorrectly in firmware.
  Action: In SETTINGS screen, verify Mode = PRESCALE and prescale ratio
          shown is correct.  Auto-select requires an initial frequency
          estimate; on first measurement, firmware defaults to ÷8.
          If the signal is >600 MHz, touch MODE to force PRESCALE ÷64.
  Alternative: Verify MC pin voltage.
    ÷8 mode:  MC pin (GPIO0) = LOW (0 V).
    ÷64 mode: MC pin (GPIO0) = HIGH (3.3 V).

8.4  FREQUENCY DRIFTS OR JITTERS (EXCESSIVE VARIATION)
--------------------------------------------------------
  Symptom: Consecutive readings differ by more than expected resolution.
  Cause 1: Input signal is amplitude-modulated or has parasitic FM.
           Verify signal quality with oscilloscope.
  Cause 2: TCXO thermal instability.  Allow 5-minute warm-up.  Cover TCXO
           module with thermal cap (tcxo_shield_cap.scad) to reduce draft.
  Cause 3: Multiple signals (harmonics, intermod) entering input.
           Engage LP filter S2 appropriate to band.  Use attenuator if
           signal level is high.
  Cause 4: ESP32 internal clock domain interference.  Only ADC1 GPIOs are
           used (34, 35, 36, 39); WiFi RF can affect ADC readings.
           Disable WiFi stream when maximum stability is required.

8.5  GPSDO WILL NOT LOCK (STAYS AT "LOCKING XX%")
---------------------------------------------------
  Check 1: Verify GPS antenna has clear sky view.  Minimum: 4 satellites.
           GPSDO screen shows satellite count; need ≥ 4 for 3D fix.
  Check 2: Verify GPS UART wiring: TXD (NEO-6M) → GPIO16, RXD → GPIO17.
           At 9600 baud.  Check NMEA data on Serial monitor (Serial2.print
           debug in gpsdo_parse_nmea).
  Check 3: Verify 1 PPS signal on GPIO26.  Should be a 1 Hz rising edge,
           100 ms pulse width.  Confirm with oscilloscope.
  Check 4: Verify PCNT_UNIT_2 is counting on GPIO39 (10 MHz reference).
           In GPSDO screen, ref_ticks should be approximately 10,000,000
           (will show as freq_error = large value until locked).
  Check 5: Check that GPIO25 DAC output is varying.  Measure voltage on
           TCXO VC pin; should be approximately 1.65 V ± 0.5 V during
           initial discipline.

8.6  GPSDO LOCKED BUT ACCURACY IS POOR (>10 ppb)
---------------------------------------------------
  Check 1: Improve GPS antenna placement.  Accuracy improves with more
           satellites and better sky view.  PDOP < 2.0 is preferred.
  Check 2: Allow longer averaging time.  GPSDO screen "Mean ppb" should
           approach 0 after several hours of locked operation.
  Check 3: Verify 10 MHz reference is reaching PCNT_UNIT_2 on GPIO39.
           If any buffer (74HC04) is between TCXO and GPIO39, check for
           correct logic levels (0 to 3.3 V HCMOS swing).
  Check 4: Check for interference on GPIO39.  Any nearby switching power
           supply or clock source within 10 MHz ± loop bandwidth can
           corrupt the reference count.

8.7  DISPLAY BACKLIGHT OFF — NO DISPLAY
-----------------------------------------
  Check 1: Verify 3.3 V on TFT Vcc pin.
  Check 2: Verify GPIO21 (TFT_BL) is HIGH (analogWrite value > 0 in setup).
  Check 3: Check TFT SPI connections: CS=15, DC=2, RST=12, MOSI=13, SCLK=14.
           On CYD board, these are internal; verify no solder bridges.
  Check 4: Reflash firmware.  CYD TFT init failure is often a bad
           User_Setup.h configuration in TFT_eSPI library.

8.8  TOUCH NOT RESPONDING
--------------------------
  Check 1: Verify XPT2046 chip select GPIO33 is correctly configured.
           TFT_eSPI handles this; verify User_Setup.h entry:
             #define TOUCH_CS 33
  Check 2: Verify SPI clock speed is ≤2 MHz for XPT2046.
           TFT_eSPI defaults are correct; ensure no custom SPI clock override.
  Check 3: Perform touch calibration if display is rotated or mirrored.
           Enter calibration mode or adjust TFT rotation parameter.

================================================================================
CHAPTER 9 — PARTS LIST
================================================================================

9.1  ELECTRONIC COMPONENTS — MAIN BOARD
-----------------------------------------

  Ref       Qty  Value/Part          Description
  ---       ---  ----------          -----------
  U1        1    ESP32-2432S028      CYD module (ESP32 + ILI9341 2.8" TFT)
  U2        1    74LVC14A            Hex Schmitt trigger, 3.3V (SOT/DIP-14)
  U3        1    ERA-3SM             MMIC preamp, 0.1–3 GHz, SOT-89
  U4        1    MB506               Dual-modulus prescaler ÷8/÷64, DIP-8
  U5        1    FXO-HC73R-10        10 MHz TCXO ±0.5 ppm (Fox Electronics)
                 (alt: Raltron TF050-10.000-3.3)
  U6        1    74HC04              Hex inverter (TCXO output buffering)
  U7        —    Isotemp OCXO-131C   OCXO 10 MHz (Config B only; alt: CFPT-9301)
  U8        1    NEO-6M module       GPS receiver with active patch antenna
  D1        1    BAT54S              Dual Schottky clamp, SOT-23
  D2        1    BAT54               Level shift diode, SOT-23 (Config B, OCXO)
  C1        1    100 pF NPO/C0G     AC coupling (direct input path)
  C2,C3     2    100 nF X7R          74LVC14A supply bypass
  C4,C5     2    100 pF NPO          DC block, prescaler chain
  C6        1    100 nF X7R          MB506 supply bypass (HF)
  C7        1    10 µF electrolytic  MB506 supply bypass (LF)
  C8        1    100 pF NPO          MB506 FIN− AC ground
  C9        1    100 pF NPO          TCXO output coupling
  C10       1    100 nF X7R          TCXO supply bypass
  C11       1    100 pF NPO          Comparator threshold bypass
  R1        1    1 kΩ               74LVC14A unused input pull-down
  R2        1    50 Ω 1%            TCXO output series termination
  R3        1    75 Ω ¼W            ERA-3SM bias (9 V supply)
  R4        1    50 Ω 1%            FOUT series termination (prescaler)
  R5        1    10 kΩ              MB506 MC pin pull-down (÷8 default)
  R6        1    50 Ω               FOUT series (reduces ringing)
  R_TH1,2   2    10 kΩ              LMV7219 comparator threshold divider
  R_PU      1    1 kΩ               LMV7219 output pull-up
  R_HYS     1    1 MΩ               LMV7219 hysteresis (optional)
  R_Pi      9    per table           Pi attenuator resistors ±1% thin-film
  L_LP, C_LP  per table              LP filter bank inductors/capacitors
  U_COMP    1    LMV7219             High-speed comparator, SOT-23-5
  J1        1    SMA female chassis  Main frequency input (direct/reciprocal)
  J2        1    SMA female chassis  Prescaler input (60 MHz – 3 GHz)
  J4        1    SMA female chassis  10 MHz reference output
  J5        1    SMA female chassis  GPS antenna input
  S1        1    1×4 rotary          Attenuator selection
  S2        1    1×6 rotary          Low-pass filter selection
  S3        1    SPDT               ERA-3SM bypass switch
  S4        1    1×4 rotary          Gate time selection
  S5        1    1×4 rotary          Measurement mode selection
  S6        1    1×3 rotary          Input impedance (50/HiZ/75 Ω) — optional
  SW1       1    DPDT rocker         Power switch, 16 × 10 mm panel mount
  P1        1    USB-C female        Power input connector
  LED1      1    5 mm green LED      Gate indicator

9.2  PASSIVE FILTER COMPONENTS
--------------------------------
  (See Appendix C for calculated values; buy closest E12 values)

  60 MHz filter:   L1a = 265 nH (T-37-6, 12t 26AWG), C1a = 36 pF NPO, L1b = 265 nH
  200 MHz filter:  L2a = 82 nH, C2a = 11 pF, L2b = 82 nH
  600 MHz filter:  L3a = 27 nH, C3a = 3.9 pF, L3b = 27 nH
  1500 MHz filter: L4a = 11 nH, C4a = 1.5 pF, L4b = 11 nH

9.3  HARDWARE
--------------
  M3 heat-set inserts (Ruthex MK3-4):  8 pcs  (enclosure lid)
  M3 × 8 mm SHCS:                      8 pcs
  9V battery snap connectors:          2 pcs
  SMA panel-mount nut wrenches:        1 set

9.4  3D PRINTED PARTS
-----------------------
  enclosure_freq_counter.scad:
    box_body()          (1 pc, ASA, top face up)
    lid()               (1 pc, ASA, print upside down)
    battery_tray()      (1 pc, ASA)
    feet()              (4 pcs, TPU or rubber, print separately)

  probe_housing.scad:
    prescaler_module_box()  (1 pc, ASA)
    prescaler_lid()         (1 pc, ASA)
    tcxo_shield_cap()       (1 pc, PETG or ASA)
    gps_antenna_mount()     (1 pc, ASA)

================================================================================
APPENDIX A — FREQUENCY RESOLUTION TABLES
================================================================================

A.1  DIRECT COUNT MODE (resolution = 1 / gate_time)
------------------------------------------------------
  Gate Time    Resolution    Freq Range    Update Rate
  ---------    ----------    ----------    -----------
  100 ms       10 Hz         0–40 MHz      10 readings/s
  1 s          1 Hz          0–40 MHz      1 reading/s
  10 s         0.1 Hz        0–40 MHz      1 reading/10 s
  100 s        0.01 Hz       0–40 MHz      1 reading/100 s

A.2  RECIPROCAL COUNT MODE (resolution = f² / (N × f_ref))
------------------------------------------------------------
  N = Auto selection based on gate ≈ 1s

  Input Freq    Auto N      Resolution (Hz)   Gate Time (approx)
  ----------    ------      ---------------   ------------------
  1 Hz          1           0.000 000 1 Hz    1 s per cycle
  100 Hz        100         0.000 01 Hz       1 s
  1 kHz         1000        0.000 1 Hz        1 s
  10 kHz        10000       0.01 Hz           1 s
  100 kHz       100000      1 Hz              1 s
  1 MHz         1000000     100 Hz            1 s
  10 MHz        1000000     10000 Hz = 10 kHz 100 ms
  14.175 MHz    1000000     20 kHz            70 ms

  NOTE: At 14 MHz, N=10000 gives 2 Hz resolution in 0.7 ms gate.
        N=10000000 would give 2 mHz resolution in 700 ms gate.
        Firmware limits N to 1,000,000 by default; increase RC_N_MAX for
        laboratory use.

A.3  PRESCALER MODE
--------------------
  Direct-count resolution applies to the DIVIDED frequency.
  Displayed resolution = direct_resolution × prescale_factor.

  Input Freq    Prescale    PCNT Input    1s Gate Res    Displayed Res
  ----------    --------    ----------    -----------    -------------
  100 MHz       ÷8          12.5 MHz      1 Hz           8 Hz
  432 MHz       ÷8          54 MHz        1 Hz           8 Hz
  1296 MHz      ÷64         20.25 MHz     1 Hz           64 Hz
  2400 MHz      ÷64         37.5 MHz      1 Hz           64 Hz

================================================================================
APPENDIX B — PI ATTENUATOR REFERENCE
================================================================================

  Shunt-Series-Shunt Pi attenuator, 50 Ω nominal impedance.

  Atten (dB)   R_shunt (Ω)   R_series (Ω)   R_shunt (Ω)
  ----------   -----------   ------------   -----------
  0 dB         —             0 (bypass)     —
  10 dB        95.3          71.5           95.3
  20 dB        61.9          249            61.9
  30 dB        53.6          787            53.6

  Use E96 series 1% thin-film resistors (0402 or 0603).
  For high-power input (>0 dBm), use 0603 (100 mW rated).
  For input >+20 dBm, use external SMA attenuator (N-type rated) FIRST.

================================================================================
APPENDIX C — LOW-PASS FILTER COMPONENT VALUES
================================================================================

  3-pole Chebyshev LC T-filter, 0.1 dB passband ripple, 50 Ω.
  Configuration: shunt-series-shunt (L-C-L).

  fc (MHz)   L_a (nH)   C_mid (pF)   L_b (nH)   Core (approx)
  --------   --------   ----------   --------   -------------
  60         265        36           265        T-37-6, 12t 26AWG; or Bourns SRR3010A-271Y
  200        82         11           82         T-25-6, 7t 28AWG; or Coilcraft 0402CS-82N
  600        27         3.9          27         Coilcraft 0402CS-27NX; or Johanson 0402HP-27NX
  1500       11         1.5          11         Coilcraft 0402HP-10N; measure and tune
  3 GHz      Bypass (wire through; ERA-3SM limits upper edge)

  CAUTION at 1500 MHz and 3 GHz: component values approach parasitics.
  Use VNA to verify filter cutoff before installation.
  Shunt capacitors must be NPO/C0G for stability.

================================================================================
APPENDIX D — RECIPROCAL COUNTER THEORY
================================================================================

D.1  PRINCIPLE OF OPERATION
-----------------------------
  A conventional frequency counter measures the number of input cycles
  during a fixed gate interval:
    f = N_counted / T_gate
    Resolution: δf = 1 / T_gate  (independent of f)

  A reciprocal counter measures the time T for a fixed number of input
  cycles N:
    f = N / T
    δT = 1 / f_reference
    δf = (δT / T²) × N = (1 / (f_ref × T²)) × N
       = f² / (N × f_ref)

  Resolution IMPROVES with lower frequency (quadratic).

D.2  ESP32 IMPLEMENTATION
--------------------------
  Reference clock: ESP32 APB clock (80 MHz, ±25 ppm).
  Counter: PCNT counts N input edges.
  Timer: esp_timer reads microsecond timestamps at start and end.
  Resolution: limited to 1 µs (esp_timer resolution = 1 µs; APB = 12.5 ns;
  but esp_timer overhead rounds to ~1 µs for most measurements).

  At 14 MHz, T_gate ≈ 70 µs (N=1,000,000):
    δT = 1 µs → δf = (14e6)² / (1e6 × 80e6) = 2.45 Hz

  With GPSDO (correction factor applied to APB reference):
    f_ref_effective → 10 MHz TCXO accuracy (<1 ppb)
    δf_effective ≈ same formula but reference drift negligible

D.3  PERIOD MEASUREMENT
------------------------
  Period T = elapsed_time_s / N_cycles
    In nano-seconds: T_ns = elapsed_us × 1000 / N

  Example: 3.579545 MHz (NTSC colorburst)
    N = 1,000,000 cycles; elapsed ≈ 279.4 ms
    T = 279,400 µs / 1,000,000 = 0.2794 µs = 279.4 ns
    Accuracy (GPSDO): ±0.1 ns (1 ppb × 279 ns)

================================================================================
APPENDIX E — PRESCALER FREQUENCY CHART (ALL HAM BANDS)
================================================================================

  Band     Frequency       Path          Prescale   PCNT Input
  ----     ---------       ----          --------   ----------
  160M     1.8–2.0 MHz     DIRECT/RECIP  ÷1         1.8–2.0 MHz
  80M      3.5–4.0 MHz     DIRECT/RECIP  ÷1         3.5–4.0 MHz
  60M      5.3–5.4 MHz     DIRECT        ÷1         5.3–5.4 MHz
  40M      7.0–7.3 MHz     DIRECT        ÷1         7.0–7.3 MHz
  30M     10.1–10.15 MHz   DIRECT        ÷1         10.1–10.15 MHz
  20M     14.0–14.35 MHz   DIRECT        ÷1         14.0–14.35 MHz
  17M     18.068–18.168 MHz DIRECT       ÷1         18–18.2 MHz
  15M     21.0–21.45 MHz   DIRECT        ÷1         21–21.5 MHz
  12M     24.89–24.99 MHz  DIRECT        ÷1         24.9–25.0 MHz
  10M     28.0–29.7 MHz    DIRECT        ÷1         28–29.7 MHz
  6M      50.0–54.0 MHz    PRESC ÷8      ÷8         6.25–6.75 MHz
  2M     144–148 MHz       PRESC ÷8      ÷8         18.0–18.5 MHz
  1.25M  222–225 MHz       PRESC ÷8      ÷8         27.75–28.125 MHz
  70cm   420–450 MHz       PRESC ÷8      ÷8         52.5–56.25 MHz
  33cm   902–928 MHz       PRESC ÷64     ÷64        14.09–14.50 MHz
  23cm  1240–1300 MHz      PRESC ÷64     ÷64        19.38–20.31 MHz
  13cm  2300–2450 MHz      PRESC ÷64     ÷64        35.9–38.3 MHz
  9cm   3300–3500 MHz      PRESC ÷64*    ÷64        51.6–54.7 MHz*

  * MB506 is rated to 2.5 GHz typical; 3.5 GHz operation uncertain.
    For 9 cm band, consider HMC361S8G cascade (÷16) for reliable coverage.

================================================================================
APPENDIX F — GLOSSARY
================================================================================

  APB clock      Advanced Peripheral Bus clock; ESP32 internal 80 MHz
                 reference derived from 40 MHz crystal × PLL.

  Gate time      The interval during which input pulses are counted (direct
                 count mode).  Longer gate = finer frequency resolution.

  GPSDO          GPS-Disciplined Oscillator.  An oscillator (TCXO or OCXO)
                 whose frequency is continuously corrected to match GPS UTC
                 using a phase-locked loop or PI algorithm.

  Holdover       Operating mode where GPSDO maintains last disciplined
                 frequency with GPS signal absent.  Accuracy degrades at
                 TCXO drift rate over time.

  LPF            Low-Pass Filter.  Attenuates frequencies above cutoff.
                 Used to select band and reject harmonics/aliasing.

  OCXO           Oven-Controlled Crystal Oscillator.  Crystal maintained at
                 constant temperature (typically 70–80°C) for stability.
                 Accuracy ±0.01 ppm after warm-up.

  PCNT           ESP32 Pulse Counter peripheral.  16-bit hardware counter;
                 counts rising or falling edges on an input GPIO.

  Phase error    Difference in phase between GPS 1 PPS signal and the
                 divided-down reference oscillator.  Measured in nanoseconds.

  ppb            Parts Per Billion.  1 ppb = 10⁻⁹.  At 10 MHz, 1 ppb = 0.01 Hz.

  ppm            Parts Per Million.  1 ppm = 10⁻⁶.  At 10 MHz, 1 ppm = 10 Hz.

  Prescaler      A circuit that divides a high-frequency input by a fixed
                 ratio (÷8 or ÷64) to bring it within range of the counter.

  Reciprocal     Measurement method where frequency is computed as
  counting       N_cycles / elapsed_time rather than counts / gate_time.
                 Provides resolution proportional to f² × gate_time.

  TCXO           Temperature-Compensated Crystal Oscillator.  Compensation
                 circuit reduces frequency variation with temperature.
                 Accuracy ±0.5 ppm over operating temperature range.

  1 PPS          One Pulse Per Second.  Timing signal output from GPS
                 receivers; rising edge aligned to GPS UTC second boundary.

================================================================================
END OF MANUAL — TM-FC-001 REV A
================================================================================
