# TECHNICAL MANUAL
## TM-SDR-001 Rev A
# SOFTWARE-DEFINED RADIO INTERFACE AND PRESELECTOR SYSTEM
### Portable HF/VHF/UHF SDR Front-End for Amateur Radio Field Operation

---

**SECURITY CLASSIFICATION:** UNCLASSIFIED

**DOCUMENT NUMBER:** TM-SDR-001 Rev A  
**DATE:** 2 May 2026  
**SUPERSEDES:** None (initial issue)  
**APPLICABLE TO:** SDR Interface Hub, HF Upconverter, Direct Sampling Interface,
Bias-T, Switchable Preselector Filters, Attenuator/Gain Control, ESP32 Controller,
CYD Touchscreen Display

**DISTRIBUTION:** Unrestricted

---

## TABLE OF CONTENTS

| Chapter | Title |
|---------|-------|
| 1 | General Information |
| 2 | Equipment Description |
| 3 | HF Upconverter Theory and Operation |
| 4 | Direct Sampling Interface |
| 5 | Bias-T Power Injector |
| 6 | Switchable Preselector Filters |
| 7 | Attenuator and Gain Control |
| 8 | ESP32 System Controller |
| 9 | CYD Touchscreen Display |
| 10 | SDR Software Setup |
| 11 | Filter Alignment Procedures |
| 12 | Operating Procedures |
| 13 | Troubleshooting |
| A | Band Table and Element Dimensions |
| B | Filter Component Values |
| C | Noise Figure Calculations |
| D | Parts List and Procurement |

---

## CHAPTER 1 — GENERAL INFORMATION

### 1.1 Purpose and Scope

This manual provides technical description, theory of operation, installation,
alignment, operating procedures, and troubleshooting guidance for the Software-
Defined Radio (SDR) Interface and Preselector System. The system extends the
capability of commercially available SDR dongles (RTL-SDR, RSP1A, Airspy HF+)
to provide calibrated, interference-free reception across all Amateur Radio
allocations from 1.8 MHz through 1.3 GHz.

The system was designed for portable field operation and bench-top station use.
It provides the RF front-end quality necessary for serious signal work while
remaining constructable from standard amateur-grade components.

### 1.2 Equipment Configuration

The complete SDR Interface System consists of the following assemblies:

| Assembly | Document | Description |
|----------|----------|-------------|
| HF Upconverter | TM-SDR-SCH-001 | 0–30 MHz → 125–155 MHz converter |
| Direct Sampling IF | TM-SDR-SCH-002 | Anti-alias LPF, impedance match |
| Bias-T (HF) | TM-SDR-SCH-003 Ver A | 0.1–50 MHz, DC inject, 500 mA |
| Bias-T (VHF/UHF) | TM-SDR-SCH-003 Ver B | 10 MHz–3 GHz, 300 mA |
| Preselector | TM-SDR-SCH-004 | 16-band switched BPF suite |
| ATT/Gain | TM-SDR-SCH-005 | PE4302 + SPF5189Z + AD8307 |
| ESP32 Controller | TM-SDR-FW-001 | WiFi AP, BT, web UI |
| CYD Display | TM-SDR-FW-002 | ILI9341 touch display |
| Hub Enclosure | TM-SDR-ENC-003 | 190×140×65 mm, RF shielded |
| Preselector Box | TM-SDR-ENC-002 | 180×120×50 mm, RF shielded |
| Upconverter Box | TM-SDR-ENC-001 | 90×65×36 mm, RF shielded |

### 1.3 Covered Ham Bands

The system covers all major U.S. Amateur Radio allocations:

| Band | Frequencies | Upconv | Direct |
|------|-------------|--------|--------|
| 160 Meters | 1.800–2.000 MHz | — | YES |
| 80 Meters | 3.500–4.000 MHz | — | YES |
| 40 Meters | 7.000–7.300 MHz | — | YES |
| 30 Meters | 10.100–10.150 MHz | — | YES |
| 20 Meters | 14.000–14.350 MHz | — | YES* |
| 17 Meters | 18.068–18.168 MHz | YES | NO |
| 15 Meters | 21.000–21.450 MHz | YES | NO |
| 12 Meters | 24.890–24.990 MHz | YES | NO |
| 10 Meters | 28.000–29.700 MHz | YES | NO |
| 6 Meters | 50.000–54.000 MHz | — | DIR |
| 2 Meters | 144.000–148.000 MHz | — | DIR |
| 1.25 Meters | 222.000–225.000 MHz | — | DIR |
| 70 cm | 420.000–450.000 MHz | — | DIR |
| 33 cm | 902.000–928.000 MHz | — | DIR |
| 20 cm / 23 cm | 1240–1300 MHz | — | DIR |

*20 meters falls just below the 14.4 MHz Nyquist limit for RTL-SDR direct sampling.
DIR = direct input to SDR without upconverter (RTL-SDR tunes these natively).

### 1.4 Operating Configurations

**Configuration A — HF with Direct Sampling (160m–20m):**
Antenna → Bias-T → Preselector BPF → ATT/LNA → Direct Sampling IF → RTL-SDR v3

**Configuration B — HF with Upconverter (17m–10m):**
Antenna → Bias-T → Preselector BPF → ATT/LNA → HF Upconverter → RTL-SDR v3

**Configuration C — VHF/UHF Direct (6m–23cm):**
Antenna → Bias-T → Preselector BPF → ATT/LNA → SDR SMA input (no upconverter)

**Configuration D — Wideband Panadapter:**
Antenna → (bypass preselector) → ATT → SDR SMA input

### 1.5 Conventions

In this manual:
- Frequencies are expressed in MHz unless noted.
- Impedances in ohms (Ω) at 50 ohms nominal.
- Connector labels: J1=input, J2=output, J3=power; full designators in schematics.
- Bold text: **WARNINGS**, **CAUTIONS**, **NOTES**.
- RF signal levels: dBm (dB relative to 1 milliwatt into 50Ω).

**WARNING:** Radio frequency signals above +10 dBm are present at some test
points during alignment. Do not contact RF circuits while transmitting. The
preselector system is a RECEIVE-ONLY device. Do not apply transmit power (do
not connect to transmitter output). Maximum safe input: +10 dBm. Apply external
attenuator if operating near a transmitting antenna.

---

## CHAPTER 2 — EQUIPMENT DESCRIPTION

### 2.1 HF Upconverter Assembly

The HF upconverter translates the 0–30 MHz HF frequency range to 125–155 MHz,
placing HF signals within the RTL-SDR v3's native tuning range. The converter
uses an SA612A double-balanced Gilbert-cell mixer with a 125 MHz TCXO local
oscillator.

**Principal Components:**
- SA612A NXP double-balanced mixer (DIP-8)
- 125.000 MHz TCXO, ±2.5 ppm
- SPF5189Z low-noise preamplifier (+19 dB, NF 0.6 dB), optional
- 7-pole Butterworth anti-alias LPF, 30 MHz cutoff
- 3-pole Chebyshev IF BPF, 125–155 MHz passband
- SPDT relay K1: upconverter vs. bypass path
- AMS1117-3.3 voltage regulator

**Specifications:**

| Parameter | Value |
|-----------|-------|
| Input frequency | 0.1–30 MHz |
| LO frequency | 125.000 MHz |
| IF output | 125.1–155 MHz |
| Conversion gain (LNA off) | −4.5 dB |
| Conversion gain (LNA on) | +14.5 dB |
| Cascade NF (LNA on) | 2.9 dB |
| Cascade NF (LNA off) | 5.0 dB |
| Cascade IIP3 (LNA on) | −12 dBm |
| Image rejection | >45 dB |
| LO leakage at IF | < −60 dBm |
| Supply | 5V USB, 140 mA max |

### 2.2 Direct Sampling Interface

The direct sampling interface prepares HF signals for the RTL-SDR v3 Q-branch
direct ADC input, providing anti-aliasing protection and proper impedance matching.

**Principal Components:**
- 7-pole elliptic anti-aliasing LPF, 30 MHz cutoff (>60 dB at 45 MHz)
- FT-37-43 1:6 impedance transformer (50Ω → ~1800Ω)
- SPDT relay K2: preselector/wideband selection
- Optional SPF5189Z LNA

**Key Limitation:** The RTL-SDR v3 ADC operates at 28.8 Msps, yielding a
Nyquist frequency of 14.4 MHz. Signals above 14.4 MHz will alias. Bands 17m
and above require the upconverter (Configuration B) or a higher-sample-rate
SDR such as the Airspy HF+ (912 ksps to 31 MHz) or RSP1A.

### 2.3 Bias-T Power Injectors

Two bias-T versions are provided:

**Version A (HF, 0.1–50 MHz):** Uses wound toroid RF choke and 10 nF series
blocking capacitor. Passes DC at 5V or 12V (jumper selectable) to remote LNA.
PTC fuse rated 500 mA. Reverse polarity protection via Schottky diode.

**Version B (VHF/UHF, 10 MHz–3 GHz):** Uses 1 µH SMD wirewound choke (SRF >500 MHz)
and 100 pF NP0 series blocking capacitor. Rated 300 mA, BAT54S reverse protection.
RF insertion loss < 0.3 dB to 1 GHz.

### 2.4 Preselector Filter Assembly

Sixteen switched bandpass filters cover all amateur bands from 160m through 23cm.
Filters are selected by a 74HC154 4-to-16 decoder driven by ESP32 GPIO outputs.
Relay switching (Axicom V23105 at HF; high-frequency relays or PIN diodes at VHF+).

**HF filter topology:** 3-pole Chebyshev LC bandpass, 50Ω, wound toroid inductors
on T50-2/T50-6 cores. NP0/C0G capacitors throughout for temperature stability.

**VHF filter topology:** Helical resonators (6m, 2m) or interdigital PCB structures.

**UHF filter topology:** Microstrip coupled resonators (FR4 PCB) or SAW filters
for 33cm and 23cm bands.

### 2.5 Attenuator and Gain Control

**PE4302 Digital Step Attenuator:** 6-bit, 0–31.5 dB in 0.5 dB steps. SPI
controlled. IIP3 = +43 dBm (transparent to system linearity). Operates 0–3 GHz.

**SPF5189Z LNA:** Gain +19 dB, NF 0.6 dB, IIP3 +27 dBm. Relay-bypass for
strong-signal environments. Default-on for weak signal work, default-off for
broadcast-dense environments.

**AD8307 Log Amplifier:** Calibrated RSSI readback, −75 to +17 dBm, 25 mV/dB
slope. Output voltage read by ESP32 ADC1. Enables automatic level control (ALC).

### 2.6 ESP32 Controller

The ESP32 module provides:
- Preselector band switching (4 GPIO address lines + enable)
- SPI control of PE4302 attenuator
- Relay control for LNA, upconverter bypass, bias-T
- AD8307 RSSI readback via ADC1
- WiFi Access Point: SSID "SDR-Controller" at 192.168.4.1
- Web server: full HTML/JavaScript band selector UI
- Bluetooth Serial: connects to CYD display and terminal
- USB UART: programming and debug

### 2.7 CYD Touchscreen Display

The ESP32-2432S028R (CYD) module provides:
- 320×240 ILI9341 TFT display
- XPT2046 resistive touchscreen
- Bluetooth master: auto-connects to "RF-SDRCtrl"
- Displays: band, attenuation, LNA/bypass/bias-T state, RSSI bar and history
- Touch controls: band grid (16 buttons), ATT+/−, toggle controls
- Double-buffered rendering at 10 FPS via TFT_eSprite

---

## CHAPTER 3 — HF UPCONVERTER THEORY AND OPERATION

### 3.1 Mixer Theory

A frequency mixer multiplies two signals: the RF input (f_RF) and a local
oscillator (f_LO), producing sum and difference products at the output:

    f_out = f_LO ± f_RF

With f_LO = 125 MHz and f_RF = 7.15 MHz:
    f_IF = 125 + 7.15 = 132.15 MHz (sum product, used)
    f_IF = 125 − 7.15 = 117.85 MHz (difference product, filtered by IF BPF)

The IF BPF (passband 125–155 MHz) passes sum products and rejects difference
products. The RTL-SDR is then tuned to the IF frequency (125–155 MHz range).

### 3.2 Image Frequency

Every mixer has an image frequency: the frequency that produces the same IF
as the desired signal but through the difference product:

    f_image = f_LO − f_IF = f_LO − (f_LO + f_RF) = −f_RF

For the sum-product upconverter, the image is at:
    f_image = 2×f_LO − f_RF

Example: f_RF = 7 MHz, f_LO = 125 MHz:
    f_image = 250 − 7 = 243 MHz

The image at 243 MHz is not an HF frequency; it cannot reach the mixer because
the 30 MHz input LPF provides >60 dB attenuation at 243 MHz. No image problem exists.

### 3.3 SA612A Gilbert Cell Operation

The SA612A implements a double-balanced Gilbert-cell topology. Key properties:

**Conversion gain:** +14 dB. This gain represents the transconductance of the
differential pairs converting the RF voltage to an IF current.

**Noise figure:** 4.5 dB (SSB). Dominated by the input transistor shot noise.
With an SPF5189Z LNA preceding it, the cascade NF approaches 2.9 dB (Friis).

**IIP3:** −14 dBm. The third-order input intercept determines the onset of
intermodulation distortion. Two strong signals f1 and f2 produce an intermodulation
product at 2f1−f2. When the input levels reach IIP3, the IMD product equals the
desired signal.

**Practical limit:** With LNA engaged, the −3 dB compression point of the cascade
is around −28 dBm input. For strong HF broadcast environments, bypass the LNA and
add 10–20 dB external attenuation.

### 3.4 125 MHz TCXO Local Oscillator

The 125 MHz TCXO provides:
- Frequency accuracy: ±2.5 ppm → ±312 Hz at 125 MHz
- Translated to HF: affects IF display frequency, not signal detection
- Phase noise: −130 dBc/Hz at 10 kHz offset → insignificant for SSB/CW
- Temperature stability: compensated over −40 to +85°C

The CMOS output (0/3.3V square wave) contains odd harmonics (375, 625 MHz, etc.)
These are suppressed by the IF BPF to > −60 dBc.

### 3.5 Setting SDR Software for Upconverter Operation

When using the upconverter, the SDR software must subtract the LO offset:

**RTL-SDR in GQRX:**
1. Set "Input controls" → "Frequency correction (ppm)" to match TCXO accuracy.
2. Enable LO correction: set offset = −125000000 Hz (−125 MHz).
3. GQRX then displays the actual HF frequency (e.g., tune to 132.15 MHz → shows 7.15 MHz).

**SDR++ :**
1. Open Source tab → RTL-SDR.
2. Set "Offset Tuning" to enabled.
3. Under "Frequency Manager" add custom offset: −125.000 MHz.
4. Band plan will display HF frequencies correctly.

**HDSDR:**
1. Options → Input → IF frequency (LO offset): enter 125000000.
2. Check "Invert": no (not inverted with sum-product upconverter).

---

## CHAPTER 4 — DIRECT SAMPLING INTERFACE

### 4.1 RTL-SDR v3 Direct Sampling Mode

The RTL2832U chip in the RTL-SDR v3 has a direct sampling input on the Q branch
that bypasses the R820T2 tuner. The signal is applied directly to the 28.8 Msps
8-bit ADC. This mode is activated in software:

**GQRX activation:**
1. Preferences → Input Controls → Device string:
   `rtl=0,direct_samp=2` (for Q branch)
   or `rtl=0,direct_samp=1` (for I branch)
2. Center frequency 0.0 Hz initially.
3. Sample rate: 2.048 MHz or 2.4 MHz for clean operation.

**SDR++ activation:**
1. Source → RTL-SDR → Direct Sampling → Q Branch.
2. Frequency offset: 0.

### 4.2 Anti-Aliasing Filter Theory

With a 28.8 Msps ADC, the Nyquist theorem states: signals above 14.4 MHz will
alias (fold back into the baseband spectrum). A signal at frequency f aliases to:

    f_alias = |f_signal − n × 28.8 MHz|

for integer n. Signals at 15 MHz appear at 28.8 − 15 = 13.8 MHz in the spectrum —
overlapping 20m. The anti-aliasing LPF at 30 MHz provides >60 dB attenuation
above 45 MHz, preventing aliasing from the UHF range. Signals in the 14.4–28.8 MHz
range are attenuated by the LPF rolloff (−3 dB at 30 MHz, −20 dB at 40 MHz).

For the bands 17m–10m (18–29 MHz), use upconverter mode.

### 4.3 Impedance Matching

The RTL-SDR v3 Q input impedance is approximately 2 kΩ at HF frequencies.
Driving this high-impedance input from 50Ω requires a step-up transformer:

**1:6 transformer on FT-37-43:**
- Primary: 4 turns (50Ω side)
- Secondary: 24 turns (to ADC)
- Turns ratio: 6:1
- Impedance ratio: 36:1
- 50Ω × 36 = 1800Ω (reasonable match to ~2 kΩ ADC input)

Alternatively, a 75Ω shunt resistor at the ADC input reduces the mismatch
reflection at the cost of ~5 dB additional attenuation.

---

## CHAPTER 5 — BIAS-T POWER INJECTOR

### 5.1 Theory of Operation

A bias-T is an RF/DC diplexer: it combines RF signal and DC power on a single
coax conductor without mutual interference:

**RF choke path (series, DC side):**
The choke (L) has high impedance at RF: XL = 2πfL. At 3.5 MHz with 94 µH:
    XL = 2π × 3.5e6 × 94e-6 = 2.07 kΩ → good isolation

**Capacitor path (series, RF side):**
The capacitor (C) has low impedance at RF: XC = 1/(2πfC). At 3.5 MHz with 10 nF:
    XC = 1/(2π × 3.5e6 × 10e-9) = 4.5 Ω → good RF pass

### 5.2 Operating the HF Bias-T

1. Connect antenna coax to J1 (ANTENNA).
2. Connect SDR input to J2 (RF OUT).
3. Connect DC power supply to J3 (12V or 5V per JP1 setting).
4. Verify correct voltage at remote end before connecting LNA:
   - Use multimeter on coax center-to-shield at remote end.
   - Should read within 0.5V of supply voltage (accounting for diode drop).
5. Connect remote LNA.
6. Verify LNA is operating: RSSI should increase 10–20 dB with LNA connected.

**CAUTION:** Never operate bias-T while transmitting into the same coax.
Transmit power will destroy the bias-T circuit and connected LNA.

### 5.3 Bias-T Troubleshooting

| Symptom | Probable Cause | Check |
|---------|---------------|-------|
| No DC at remote end | PTC fuse tripped | Measure across F1; if >0.5V, fuse tripped; wait 30 s or replace |
| No DC at remote end | Reversed polarity | Verify power supply polarity vs. J3 center-pin (+) |
| RF signal absent | Blocking cap C1 shorted | Measure DC at SDR input; should be 0V; if DC present, C1 failed |
| RF noise on signal | Power supply ripple | Add 100 µH + 100 µF LC filter at DC input |
| LNA not amplifying | Voltage insufficient for LNA Vmin | Measure remote end voltage; if <3V, increase supply or shorten coax |

---

## CHAPTER 6 — SWITCHABLE PRESELECTOR FILTERS

### 6.1 Bandpass Filter Theory

A bandpass filter passes a range of frequencies (passband) and rejects all others
(stopband). The quality factor Q = f_center / bandwidth defines selectivity:

    High Q: narrow bandwidth, sharper skirts, higher insertion loss
    Low Q: wide bandwidth, easier to build, lower insertion loss

For ham bands, Q values range from 7.5 (80m, BW 500 kHz) to 202 (30m, BW 50 kHz).
LC filters practical for Q up to approximately 200 with air/toroid inductors.
Higher Q requires crystal or ceramic filters (not practical for this application).

### 6.2 3-Pole Chebyshev Bandpass Filter Design

The 3-pole Chebyshev topology provides:
- 0.1 dB ripple in passband
- 18 dB/octave rolloff beyond cutoff (steeper than Butterworth)
- Practical insertion loss 0.6–1.5 dB for wound toroid inductors (Q > 100)

**Prototype to bandpass transformation:**
Given a desired f0 (center), BW (bandwidth), and 50Ω system impedance,
the element values are calculated as:

    L_shunt = g_k × Z0 / (ω0 × w)     where ω0 = 2πf0, w = BW/f0
    C_shunt = 1 / (ω0² × L_shunt)
    C_series = w / (ω0 × Z0 × g_k)
    L_series = 1 / (ω0² × C_series)

where g_k are the Chebyshev prototype element values:
    3-pole, 0.1 dB ripple: g1 = g3 = 1.0316, g2 = 1.1474

For a simplified 3-pole implementation (shunt resonators + series coupling caps):
    3 shunt resonators each tuned to f0 (Lp||Cp)
    2 series coupling capacitors Cs (sets bandwidth)

This simplified topology is practical for amateur construction and requires only
6 components (3 Lp, 3 Cp, 2 Cs — with Cs and shunt pair combining).

### 6.3 Toroid Inductor Winding

Toroids for HF preselector filters:

**T50-6 (iron powder, yellow/white, µ=8.5):**
- Suitable for 3.5–30 MHz
- AL = 40 nH/turn² (approx.)
- Maximum flux density: good for receive use
- Winding formula: N = √(L_µH / AL_µH) = √(L / 0.04)

**T50-2 (iron powder, red, µ=10):**
- Suitable for 1–30 MHz
- AL = 49 nH/turn² (approx.)
- Better choice for 160m and 80m

**Winding technique:**
1. Use 24 AWG enameled wire.
2. Wind turns evenly spaced around core.
3. Leave 6 mm leads for soldering.
4. Scrape insulation at lead ends with knife; tin with 60/40 solder.
5. Measure with LCR meter at test frequency; adjust turns ±1 for exact value.
6. Secure with a small bead of RTV silicone at 2 points (do not pot entirely).

**Inductance verification:** L = N² × AL (nH, with N in turns and AL in nH/turn²).

### 6.4 Filter Alignment

See Chapter 11 for complete alignment procedures. Brief overview:

1. Set NanoVNA for S21 sweep from 0.1×f0 to 3×f0.
2. Observe passband center frequency and bandwidth.
3. Adjust shunt inductors (compress/expand windings) to shift f0.
4. Adjust series coupling capacitor Cs to change bandwidth:
   - Larger Cs: wider BW, more loss
   - Smaller Cs: narrower BW, less loss
5. Target: center frequency ±5%, bandwidth ±20%, insertion loss < 1.5 dB.

### 6.5 Band Switching

The 74HC154 decoder selects one of 16 relay outputs based on a 4-bit code from
the ESP32. The decoder output is inverted (via 74HC04) and drives ULN2003A
Darlington transistors that sink current through the relay coils.

**Relay switching time:** ~5 ms (coil energize + armature travel + contact settle).
**Software delay:** 10 ms delay after issuing new band code before proceeding.
**Glitch prevention:** BPF_EN is deasserted before changing address bits (prevents
partial decode activation), then reasserted after address settles.

---

## CHAPTER 7 — ATTENUATOR AND GAIN CONTROL

### 7.1 Need for Attenuation

A strong signal overloads the mixer or ADC, producing intermodulation products
that appear as spurious signals throughout the spectrum. The third-order intercept
point (IP3) determines the signal level at which IMD becomes significant.

**SA612A IP3 = −14 dBm input:**
Third-order intermodulation (IM3) products appear when two equal signals at levels
P_in produce spurious signals at level:

    IM3_level = 3×P_in − 2×IIP3

Example: Two broadcast signals at −20 dBm each into SA612A:
    IM3 = 3×(−20) − 2×(−14) = −60 + 28 = −32 dBm (significant!)

By inserting 10 dB attenuation before the mixer:
    Effective P_in = −30 dBm
    IM3 = 3×(−30) − 2×(−14) = −90 + 28 = −62 dBm (much better)

**Rule:** Add attenuation until IMD spurs disappear from SDR display.

### 7.2 PE4302 Operation

The PE4302 is a 6-bit digital step attenuator. SPI word structure (LSB first):

```
Bit 0 (sent first): 0.5 dB
Bit 1:              1.0 dB
Bit 2:              2.0 dB
Bit 3:              4.0 dB
Bit 4:              8.0 dB
Bit 5 (sent last):  16.0 dB
```

Setting 31.5 dB (all bits 1): 0b111111 = 0x3F.
Setting 10 dB: 10 / 0.5 = 20 steps = 0b010100 = 0x14.

The latch enable (LE) pin must be pulsed HIGH for ≥1 µs after the 6 bits are
shifted in. See filter_switch.h `_setAttenuator()` for implementation.

### 7.3 Automatic Level Control (ALC)

When enabled, the ESP32 periodically reads the AD8307 RSSI voltage and adjusts
attenuation to maintain the SDR input near −30 dBm:

```
Target: −30 dBm ± 5 dB (−35 to −25 dBm range)
Upper threshold: −25 dBm → increase ATT by 3 dB
Lower threshold: −45 dBm → decrease ATT by 3 dB
```

The 5 dB hysteresis prevents hunting (oscillation between two states).

**ALC behavior with strong signals:**
- Broadcast station (S9+40): ALC will drive ATT to maximum 31.5 dB.
- If still overloaded: ALC disables LNA (reduces gain further).
- If still overloaded: operator alert via OLED/CYD display.

**ALC behavior with very weak signals:**
- DX signal (S1): ALC will enable LNA and set ATT to 0 dB.
- Final sensitivity limited by system NF (2.9 dB with LNA).

### 7.4 Noise Figure and Minimum Detectable Signal

**Friis cascade formula:**
    NF_total(linear) = NF1 + (NF2−1)/G1 + (NF3−1)/(G1×G2) + ...

With LNA on (typical DX conditions):
- BPF: NF=1.2× (0.7 dB), G=0.851 (−0.7 dB)
- PE4302 (0 dB): NF=1.4× (1.5 dB), G=0.708 (−1.5 dB)
- SPF5189Z: NF=1.148× (0.6 dB), G=79.4× (+19 dB)
- SA612A: NF=2.818× (4.5 dB), G=0.631 (−2 dB)

    NF_total = 1.2 + (1.4−1)/0.851 + (1.148−1)/0.602 + (2.818−1)/47.8
             = 1.2 + 0.47 + 0.25 + 0.038 = 1.96 → NF = 2.93 dB

**Minimum Detectable Signal (MDS) at 500 Hz bandwidth:**
    MDS = −174 dBm/Hz + NF + 10×log10(BW_Hz)
        = −174 + 2.93 + 10×log10(500)
        = −174 + 2.93 + 27.0
        = −144 dBm

This corresponds to approximately S0 (below S-meter noise floor) — excellent
sensitivity for a receive-only front-end.

---

## CHAPTER 8 — ESP32 SYSTEM CONTROLLER

### 8.1 Software Architecture

The ESP32 firmware (esp32_sdr_controller.ino) uses a cooperative multitasking
architecture (no RTOS required):

```
setup(): Initialize hardware, load NVS preferences, start WiFi AP,
         register web server routes, start Bluetooth Serial.

loop(): Handle web client, parse BT commands, parse serial commands,
        run ALC polling (every 100 ms), send BT status packets (every 2 s).
```

**FilterSwitch class (filter_switch.h):** Encapsulates all hardware control.
Public interface: selectBand(), setAttenuation(), setLNA(), setBiasT(), readRSSI_dBm().

### 8.2 WiFi Web Interface

Connect to WiFi AP "SDR-Controller" (pass: sdrcontrol), then browse to
http://192.168.4.1. The web UI provides:

- **Band grid:** 16 buttons (4×4). Tap to select band.
- **ATT slider:** 0–31 dB range. Drag to set.
- **Toggle buttons:** LNA, Bypass, Bias-T, ALC.
- **RSSI bar:** Real-time level display with 1-second refresh.
- **Status line:** Current configuration summary.

API endpoints (POST with JSON body):
```
POST /api/band     {"band":"40M"}
POST /api/att      {"att":10}
POST /api/lna      {"on":true}
POST /api/bypass   {"on":false}
POST /api/biastee  {"on":true}
POST /api/alc      {"on":true}
GET  /api/status   (returns JSON state)
```

### 8.3 Bluetooth Serial Protocol

The ESP32 broadcasts Bluetooth as "RF-SDRCtrl". Commands are newline-terminated ASCII:

| Command | Description | Example |
|---------|-------------|---------|
| BAND \<name\> | Select ham band | BAND 40M |
| ATT \<0-31\> | Set attenuation dB | ATT 10 |
| LNA ON/OFF | Enable/disable LNA | LNA ON |
| BYP ON/OFF | Bypass upconverter | BYP OFF |
| BT ON/OFF | Enable/disable bias-T | BT ON |
| ALC ON/OFF | Enable/disable ALC | ALC ON |
| STATUS | Request status packet | STATUS |
| RSSI | Read instantaneous RSSI | RSSI |
| HELP | List commands | HELP |

**Status packet format (sent every 2 s when CYD connected):**
```
BAND:40M,FREQ:7150000,ATT:0,LNA:1,BYP:0,BT:0,RSSI:-42.3
```

### 8.4 NVS Persistent Storage

The last-used band, attenuation, and LNA state are saved to NVS (Non-Volatile Storage,
ESP32 flash) using the Preferences library. These values are restored on power-up,
so the system remembers the last configuration.

---

## CHAPTER 9 — CYD TOUCHSCREEN DISPLAY

### 9.1 Hardware Configuration

The ESP32-2432S028R module uses:
- ILI9341 controller: 320×240 pixels, 16-bit color (RGB565)
- XPT2046: 12-bit resistive touch, SPI
- TFT_eSPI library (Bodmer): configured for CYD in User_Setup.h

Required User_Setup.h settings for TFT_eSPI:
```cpp
#define USER_SETUP_ID 206   // CYD specific
#define ILI9341_DRIVER
#define TFT_MOSI  13
#define TFT_SCLK  14
#define TFT_CS    15
#define TFT_DC    2
#define TFT_RST   -1
#define TFT_BL    21        // backlight
#define TOUCH_CS  33
#define SPI_FREQUENCY  40000000
```

### 9.2 Display Modes

**MODE_MAIN:** Primary operating display.
- Title bar with connection status indicator
- Current band (large text) + center frequency
- RSSI bar (color-coded: green/yellow/red)
- RSSI history sparkline (80-sample rolling graph)
- Controls row: ATT indicator, LNA/BYP/BT/ALC toggles
- Button row: ATT−, ATT+, LNA, BYPASS, BIAS-T, BANDS button
- Status bar: current configuration summary

**MODE_BANDS:** 4×4 band grid.
- 16 band buttons with name and center frequency
- Active band highlighted in accent color
- Tap any band to select and return to MAIN mode
- Tap title bar to return without changing band

### 9.3 BT Connection Management

The CYD operates as Bluetooth master. On startup it connects to "RF-SDRCtrl".
If the connection drops, it retries every 5 seconds. The green LED on the CYD
illuminates when connected; blue LED blinks during reconnection attempts.

Status packets from the controller update the display state. If no packets are
received within 5 seconds, the display shows the connection indicator as red.

---

## CHAPTER 10 — SDR SOFTWARE SETUP

### 10.1 GQRX (Linux, macOS)

GQRX is recommended for portable Linux operation (works on Raspberry Pi 4, PC).

**Installation (Debian/Ubuntu):**
```bash
sudo apt install gqrx-sdr
```

**RTL-SDR with upconverter:**
1. Launch GQRX.
2. File → I/O Devices → Select RTL-SDR (prefer rtlsdr-multi if multiple)
3. Device string: `rtl=0,offset_tune=1`
4. Input rate: 2.048 MHz or 2.4 MHz
5. Input controls: LO offset = −125000000 (for 125 MHz LO)
6. File → Save configuration as `sdr_upconverter.conf`

**Direct sampling (RTL-SDR v3):**
1. Device string: `rtl=0,direct_samp=2`
2. LO offset: 0
3. Input rate: 2.048 MHz

**Demodulation modes:**
- SSB: select USB or LSB; IF bandwidth 2.4 kHz; AGC fast
- CW: select CW; IF bandwidth 500 Hz; CW offset 600 Hz
- AM: select AM; IF bandwidth 6 kHz
- FM narrow (2m): select NFM; IF bandwidth 12 kHz
- WFM (broadcast): select WFM; IF bandwidth 200 kHz
- Digital modes (FT8, WSPR): set USB demod; record IQ or use audio pipe to WSJT-X

**Bookmarks:** Use bookmarks for common frequencies. Import ARRL frequency
allocation chart as CSV for automatic band plan overlay.

### 10.2 SDR++ (Linux, Windows, macOS)

SDR++ provides a polished interface with direct SDR-control panel integration.

**Installation:**
Download from GitHub (sdrpp): `sudo apt install sdrpp` (Debian repo) or AppImage.

**RTL-SDR source configuration:**
1. Source selector → RTL-SDR
2. Sample Rate: 2.048M
3. Direct Sampling: Q Branch (for HF direct sampling)
   OR: Direct Sampling: Off + Offset Tuning (for upconverter mode)
4. Frequency correction: enter TCXO ppm offset (see TCXO label)

**Upconverter offset:**
1. Module Manager → Frequency Manager
2. Add custom band with offset: −125.0 MHz
3. Select "Apply offset" checkbox in Source settings.

**Band plan plugin:**
SDR++ includes ITU band plans. For ham-specific overlay:
1. Download hamQTH or ARRL band plan CSV.
2. Place in `~/.config/sdrpp/bands/`.
3. Enable "Band plan" in Module Manager.

### 10.3 WSJT-X Integration (FT8/WSPR/JT65)

WSJT-X receives decoded digital mode signals from the SDR audio output.

**Setup chain:**
1. GQRX or SDR++: demodulate USB at desired frequency.
2. Audio: route SDR audio output to virtual audio cable (PulseAudio sink/source).
3. WSJT-X: Input → virtual audio source.
4. WSJT-X Mode: FT8 (or WSPR/JT65).
5. Set frequency in WSJT-X to match SDR display frequency.

**Virtual audio on Linux:**
```bash
pactl load-module module-null-sink sink_name=sdr_sink
pactl load-module module-loopback source=sdr_sink.monitor sink=<default_output>
```
Then set GQRX audio output to "sdr_sink"; WSJT-X input to "sdr_sink.monitor".

### 10.4 GNU Radio SDR Pipeline

For advanced signal processing, GNU Radio Companion (GRC) provides a flow-graph
based signal processing environment.

**Install:**
```bash
sudo apt install gnuradio python3-osmosdr
```

**Minimal RTL-SDR receive flow graph:**
```
RTL-SDR Source → (sample rate 2.048M) →
Low Pass Filter (cutoff 1.024M, transition 100k) →
Rational Resampler (20/1) → Audio Sink (rate 48000)
```

**Upconverter in GNU Radio:**
Set `rtlsdr_source_0.set_center_freq(center_freq + 125e6)` in the flow graph
to tune to the correct IF frequency when using the upconverter.

### 10.5 SDR Remote Operation via Network

The SDR Interface Hub WiFi AP provides basic web control. For full remote SDR:

**SoapyRemote (network SDR):**
```bash
# On SDR server (Raspberry Pi or PC with dongle):
sudo apt install soapysdr-server
SoapySDRServer --bind="0.0.0.0:55132"

# On client (GQRX, SDR++):
Device string: soapy=0,driver=remote,remote=tcp://192.168.x.x:55132
```

**RTL-TCP:**
```bash
# Server:
rtl_tcp -a 0.0.0.0 -p 1234

# Client GQRX device string:
rtl_tcp=192.168.x.x:1234
```
Note: rtl_tcp does not support direct sampling over network.

---

## CHAPTER 11 — FILTER ALIGNMENT PROCEDURES

### 11.1 Required Equipment

- NanoVNA-H4 (or NanoVNA-F) with calibration kit (SOLT)
- 50Ω SMA cables (×2), phase-matched ≤30 cm
- Plastic or non-metallic screwdriver for toroid adjustment
- Frequency counter (optional, for oscillator check)
- Spectrum analyzer or SDR for final verification

### 11.2 NanoVNA Calibration

Perform SOLT calibration before each alignment session:

1. Connect SMA calibration kit to Port 1 and Port 2 cables.
2. In NanoVNA app (NanoVNA-saver on PC):
   - Set frequency range: 0.1 × f_low to 3 × f_high of the filter being aligned.
   - Calibrate: OPEN, SHORT, LOAD at Port 1; THRU across both ports.
   - Save calibration.
3. Verify: THRU connected = S21 near 0 dB ±0.2 dB across entire sweep range.

### 11.3 Aligning HF Bandpass Filters (160m–10m)

For each filter (F01–F09):

**Step 1: Check resonance of individual shunt resonators.**
If accessible (before assembly), briefly check each Lp||Cp:
Connect NanoVNA Port 1 to resonator; observe reflection dip at f0.
Shunt resonator should show S11 minimum (minimum reflection) at f0.

**Step 2: Set sweep range.**
For 40m filter (f0 = 7.15 MHz): sweep 5 to 12 MHz.

**Step 3: Observe S21.**
Initial response may be off-center or too narrow/wide.

**Step 4: Center frequency correction.**
If passband is too high: add turns to shunt inductors (1 turn at a time).
If passband is too low: remove turns from shunt inductors.
Practical adjustment: ±1 turn changes frequency approximately ±3–5%.

**Step 5: Bandwidth adjustment.**
If bandwidth too narrow: increase coupling capacitor Cs by adding 10–20% parallel cap.
If bandwidth too wide: reduce Cs by reducing capacitor value.
Rule of thumb: doubling Cs doubles bandwidth; halving Cs halves bandwidth.

**Step 6: Insertion loss check.**
Measure S21 at center frequency. Target: < 1.0 dB.
If loss > 1.5 dB: suspect dirty solder joints, poor inductor Q, or mistuned filter.

**Acceptance criteria (per band):**

| Parameter | Specification | Measurement method |
|-----------|--------------|-------------------|
| Center frequency | ±5% of nominal f0 | S21 peak |
| Bandwidth | ±20% of nominal BW | −3 dB points |
| Insertion loss | < 1.0 dB HF; < 2.5 dB VHF | S21 at f0 |
| Adjacent-band rejection | > 40 dB HF; > 30 dB VHF | S21 at adjacent band center |
| Return loss | > 15 dB in passband | S11 |

**Step 7: Log measured values.**
Record measured f0, BW, IL, and adjacent rejection in logbook.
Date, ambient temperature, and final component values.

### 11.4 Aligning VHF/UHF Filters (6m–23cm)

**6m helical resonator filter (F10):**
1. Coarse tune: compress or expand coil for approximate f0 near 52 MHz.
2. NanoVNA sweep: 45–60 MHz. Observe S21 passband.
3. Coupling loops: adjust loop size for desired bandwidth (4 MHz target).
4. Final: S21 > −2 dB at 52 MHz, < −40 dB at 28 MHz, < −30 dB at 144 MHz.

**2m filter (F11) — lumped LC:**
1. Sweep: 120–180 MHz. L = 22 nH, C = 47 pF (resonance ≈ 157 MHz — adjust C).
2. For 146 MHz: use 27 pF (f = 1/(2π√(22e-9 × 27e-12)) ≈ 206 MHz — too high).
   Use C = 53 pF for 146 MHz: f = 1/(2π√(22e-9 × 53e-12)) = 147 MHz. Close.
3. Adjust by trimmer or parallel capacitor to center on 146 MHz.
4. Expected IL: 2–4 dB at 146 MHz with SMD lumped components.

**SAW filters (33cm, 23cm):** No alignment required. SAW filters are fixed-frequency.
Verify insertion loss only: install in circuit, measure S21 across passband.
Typical: −2 to −4 dB IL, very sharp skirts.

### 11.5 Full System Verification

After aligning all filters:

1. **Function test:** Select each band via ESP32 web UI or BT. Listen to a known
   beacon or broadcast on each band. Verify signal present.

2. **Relay switching test:** Rapidly switch between bands 20 times.
   Verify no stuck contacts (if stuck, relay may need replacement or contacts cleaned
   with Caig DeOxit D5).

3. **ALC function test:**
   - Enable ALC.
   - Connect a −10 dBm signal generator at the SMA input.
   - Observe: attenuation increases to maintain RSSI near −30 dBm.
   - Reduce signal to −80 dBm: attenuation decreases; LNA engages.
   - Verify ALC response time: < 500 ms to settle.

4. **Spurious signal test:**
   - Tune SDR to known quiet band (e.g., 40m, 14:00 UTC on weekday).
   - Observe entire 0–30 MHz spectrum in panadapter.
   - Note any persistent carriers, birdies, or digital noise.
   - If present: identify source (switching supplies, TCXO harmonics, relay coil).
   - Remediate: additional bypass capacitors, ferrite beads on supply lines, shielding.

---

## CHAPTER 12 — OPERATING PROCEDURES

### 12.1 Startup Procedure

1. Power on SDR interface hub via USB (5V, minimum 500 mA capable).
2. Observe CYD display: "Connecting BT..." → connection indicator turns green (~5 s).
3. Observe OLED on hub: band, ATT, LNA state displayed.
4. On CYD or web UI: select desired band.
5. Confirm upconverter bypass state matches band:
   - 160m–6m: bypass OFF (use direct sampling or upconverter per config)
   - 17m–10m: upconverter active (bypass OFF → upconverter in signal path)
   - 6m–23cm: bypass ON (direct to SDR, no upconverter)
6. Start SDR software. Configure device string and LO offset (if upconverter in use).
7. Confirm signal on known beacon (WWV 5/10/15 MHz, WWVH, etc.).

### 12.2 Band Change Procedure

1. On CYD tap BANDS → select band. (Or: BT command "BAND 40M".)
2. Wait 10 ms for relay to settle (firmware handles this delay automatically).
3. Re-tune SDR software to new band center frequency.
4. If changing between HF-below-Nyquist and HF-above-Nyquist:
   - Check upconverter bypass state.
   - 20m and below: upconverter may be bypassed (direct sampling).
   - 17m and above: upconverter must be in path.
5. Adjust ATT as needed (ALC handles this automatically if enabled).

### 12.3 Strong Signal Environment Operation

When operating near commercial broadcast transmitters, AM stations, or during contests:

1. Disable LNA: tap LNA button → OFF. (Or: "LNA OFF" via BT.)
2. Enable ALC or manually set ATT: start at 20 dB; reduce until IMD spurs disappear.
3. If RSSI bar shows red (≥ −20 dBm): increase ATT further.
4. If SA612A overloads (wideband noise floor rises): add 3 dB ATT, observe improvement.
5. For extreme overload (>0 dBm antenna input): add external 30 dB pad before SMA IN.

### 12.4 Weak Signal and DX Operation

1. Enable LNA: tap LNA button → ON.
2. Set ATT to 0 dB.
3. Enable ALC to prevent LNA overload from any transient strong signals.
4. Select narrowest bandwidth in SDR software (500 Hz for CW, 2.4 kHz for SSB).
5. Enable digital noise reduction or spectral subtraction if available (GQRX: DSP → NR).
6. For WSPR monitoring: ensure PC clock synchronized (NTP) to within 1 second.

### 12.5 Remote Operation Over Network

1. Configure SoapyRemote or rtl_tcp on Raspberry Pi with SDR dongle.
2. Ensure ESP32 SDR controller reachable on same network (static IP recommended).
3. From remote PC: connect GQRX to SoapyRemote device string.
4. Control preselector via http://192.168.x.x/ (ESP32 web UI).
5. Alternatively: SSH to Pi; run GQRX headless; control via gqrx remote CLI.

---

## CHAPTER 13 — TROUBLESHOOTING

### 13.1 No Signal on Any Band

| Check | Procedure | Expected |
|-------|-----------|----------|
| Antenna connected? | Visual inspection | SMA properly seated |
| Relay K1 functioning? | Apply 5V to relay coil via bench supply | Click heard; continuity at contacts |
| RTL-SDR recognized? | `lsusb` shows RTL2832U | USB device visible |
| GQRX SDR gain set? | Input controls → RF gain | Set to 20–40 dB |
| Direct sampling configured? | Device string has direct_samp=2 | Per 10.1 |
| Signal present on antenna? | Connect antenna directly to SDR (bypass everything) | Signal visible |

### 13.2 High Noise Floor

| Symptom | Cause | Remedy |
|---------|-------|--------|
| Noise floor rises with hub connected | SDR digital noise | Improve shielding; ferrite bead on USB cable |
| Periodic spurs every 1 MHz | RTL-SDR TCXO birdie | Normal; note and ignore; or switch to upconverter |
| Switching transient when changing bands | Relay arcing | Add 100 nF across relay contacts |
| 50/60 Hz hum on audio | Ground loop | Use ferrite choke on audio cable; isolate DC supply |
| TCXO harmonic at 125, 250, 375 MHz | LO leakage | Verify IF BPF attenuation; improve shielding |

### 13.3 Upconverter Issues

| Symptom | Cause | Remedy |
|---------|-------|--------|
| No signal 125–155 MHz on RTL-SDR | K1 relay not set | Verify GPIO_BYPASS LOW; check K1 coil voltage |
| Signal present but frequency offset | TCXO not 125.000 MHz | Verify TCXO module; set software ppm correction |
| IMD spurs visible | SA612A overloaded | Disable LNA; add 10 dB ATT |
| Signal present but very low level | LNA bypassed | Enable LNA or reduce path loss |
| Mirror image signals visible | IF BPF misaligned | Realign BPF per Ch.11; verify 125–155 MHz passband |

### 13.4 Preselector Issues

| Symptom | Cause | Remedy |
|---------|-------|--------|
| Wrong band selected | 74HC154 address wrong | Verify BPF_A0–A3 GPIO states match band table |
| Relay click but no signal change | Filter misaligned | Align per Ch.11 |
| No relay click on band change | ULN2003A not driving | Check +5V relay supply; verify GPIO_BPF_EN HIGH |
| Multiple bands active simultaneously | Decoder enable stuck HIGH | Verify BPF_EN deasserted before address change |
| Filter insertion loss too high | Mistuned or poor Q inductor | Realign; check inductor winding; verify NP0 caps |

### 13.5 ESP32 Controller Issues

| Symptom | Cause | Remedy |
|---------|-------|--------|
| No WiFi AP visible | ESP32 not powered or crashed | Check 5V supply; press EN (reset) button |
| Web UI loads but commands fail | WiFi signal weak | Move device or use direct connection |
| BT not connecting | Wrong BT name; CYD not in master mode | Verify BT_NAME in config.h matches CYD target name |
| Band changes not persistent | NVS write fails | Check flash wear (normal after >100k writes); reflash |
| ALC oscillating (hunts rapidly) | ALC hysteresis too small | Increase ALC_HYSTERESIS_DB to 8; reduce poll rate |

---

## APPENDIX A — BAND TABLE AND ELEMENT DIMENSIONS

### A.1 Ham Band Frequency Summary

| Band | Lower (MHz) | Upper (MHz) | Center (MHz) | BPF Code |
|------|-------------|-------------|--------------|----------|
| 160M | 1.800 | 2.000 | 1.900 | 0 |
| 80M  | 3.500 | 4.000 | 3.750 | 1 |
| 40M  | 7.000 | 7.300 | 7.150 | 2 |
| 30M  | 10.100 | 10.150 | 10.125 | 3 |
| 20M  | 14.000 | 14.350 | 14.175 | 4 |
| 17M  | 18.068 | 18.168 | 18.118 | 5 |
| 15M  | 21.000 | 21.450 | 21.225 | 6 |
| 12M  | 24.890 | 24.990 | 24.940 | 7 |
| 10M  | 28.000 | 29.700 | 28.850 | 8 |
| 6M   | 50.000 | 54.000 | 52.000 | 9 |
| 2M   | 144.000 | 148.000 | 146.000 | 10 |
| 1.25M | 222.000 | 225.000 | 223.500 | 11 |
| 70cm | 420.000 | 450.000 | 435.000 | 12 |
| 33cm | 902.000 | 928.000 | 915.000 | 13 |
| 23cm | 1240.000 | 1300.000 | 1270.000 | 14 |
| WIDE | — | — | — | 15 |

### A.2 IF Frequency Table (with 125 MHz LO)

| Ham Band | f_RF (MHz) | f_IF = 125+f_RF | SDR Tune (MHz) |
|----------|-----------|-----------------|----------------|
| 40M (7.15) | 7.150 | 132.150 | 132.150 |
| 30M | 10.125 | 135.125 | 135.125 |
| 20M | 14.175 | 139.175 | 139.175 |
| 17M | 18.118 | 143.118 | 143.118 |
| 15M | 21.225 | 146.225 | 146.225 |
| 12M | 24.940 | 149.940 | 149.940 |
| 10M | 28.850 | 153.850 | 153.850 |

---

## APPENDIX B — FILTER COMPONENT VALUES

### B.1 HF BPF Component Summary

| Band | f0 (MHz) | Lp (nH) | Cp (pF) | Cs (pF) | Core |
|------|---------|---------|---------|---------|------|
| 160M | 1.90 | 22000 | 320 | 39 | T50-2 |
| 80M | 3.75 | 5600 | 330 | 100 | T50-6 |
| 40M | 7.15 | 2200 | 220 | 33 | T50-6 |
| 30M | 10.125 | 1800 | 140 | 18 | T50-6 |
| 20M | 14.175 | 1000 | 120 | 22 | T50-6 |
| 17M | 18.118 | 680 | 110 | 12 | T50-6 |
| 15M | 21.225 | 560 | 100 | 22 | T50-6 |
| 12M | 24.940 | 390 | 100 | 10 | T50-6 |
| 10M | 28.85 | 330 | 82 | 56 | T50-6 |
| 6M | 52.0 | 100 | 82 | 47 | 0805CS |
| 2M | 146.0 | 22 | 53 | 6.8 | 0805CS |
| 1.25M | 223.5 | 12 | 39 | 4.7 | 0402CS |

---

## APPENDIX C — NOISE FIGURE CALCULATIONS

### C.1 Friis Formula Derivation

**Noise factor (linear):** F = SNR_in / SNR_out = 10^(NF_dB / 10)

**Two-stage cascade:**
    F_total = F1 + (F2 - 1) / G1

where G1 is the linear gain of stage 1.

**Full cascade (N stages):**
    F_total = F1 + (F2-1)/G1 + (F3-1)/(G1·G2) + ... + (FN-1)/(G1·G2·...·G(N-1))

**Key insight:** The first stage dominates. A high-gain, low-NF first stage
(LNA) masks the noise of subsequent stages.

### C.2 System NF Under Different Configurations

| Configuration | NF | Gain | Use case |
|--------------|-----|------|---------|
| LNA on, ATT=0 | 2.9 dB | +14.5 dB | DX, weak signals |
| LNA off, ATT=0 | 5.0 dB | −4.5 dB | General monitoring |
| LNA off, ATT=10 | 7.5 dB | −14.5 dB | Moderate strong signals |
| LNA off, ATT=20 | 12.0 dB | −24.5 dB | Strong signal, contest |
| LNA off, ATT=31 | 22.0 dB | −35.5 dB | Extreme strong signal |

---

## APPENDIX D — PARTS LIST AND PROCUREMENT

### D.1 Critical Components

| Part | Value/Type | Supplier | Approx. Cost |
|------|-----------|----------|-------------|
| SA612AN | DIP-8 DBM | Mouser, DigiKey | $2 |
| 125 MHz TCXO | ±2.5 ppm 3.3V | DigiKey (NDK) | $5–8 |
| SPF5189Z | LNA SOT-89 | Mouser, AliExpress | $2–4 |
| PE4302 | DSA SOT-26 | Mouser | $8–12 |
| AD8307 | Log amp SOIC-8 | DigiKey | $6–10 |
| 74HC154 | 4:16 decoder DIP-24 | DigiKey | $1 |
| ULN2003A | Darlington DIP-16 | DigiKey | $0.50 |
| AMS1117-3.3 | LDO SOT-223 | DigiKey | $0.50 |
| T50-6 toroid | iron powder yellow | Kits and Parts, Amidon | $0.50 ea |
| T50-2 toroid | iron powder red | Kits and Parts, Amidon | $0.50 ea |
| FT-37-43 | ferrite toroid | Amidon, Mouser | $1 |
| NP0 capacitors | 10–330 pF 50V 0805 | DigiKey, bulk | $0.05–0.10 ea |
| Axicom V23105 | SPDT relay 5V | Mouser, DigiKey | $2–4 |
| RTL-SDR v3 | Direct sampling | RTL-SDR Blog store | $30–40 |
| ESP32 DevKit | 38-pin | AliExpress, Amazon | $5–8 |
| CYD (2432S028R) | ESP32 CYD | AliExpress | $8–12 |
| NanoVNA-H4 | VNA | Amazon, AliExpress | $60–80 |
| SMA connectors | Edge mount 50Ω | Mouser, AliExpress | $1–2 ea |

### D.2 Estimated Build Cost

| Assembly | Est. Component Cost |
|----------|-------------------|
| HF Upconverter | $25–35 |
| Direct Sampling IF | $15–20 |
| Bias-T (both) | $10–15 |
| Preselector (16 filters) | $60–90 |
| ATT/Gain board | $25–35 |
| ESP32 controller | $15–20 |
| CYD display | $8–12 |
| Enclosures (3D printed filament) | $8–12 |
| Miscellaneous (connectors, wire) | $15–25 |
| **Total** | **$181–264** |

---

*End of Technical Manual TM-SDR-001 Rev A*

*Document prepared for amateur radio field and station use.*
*Not approved for classified or government applications.*
*Verify all component values before construction.*
*Perform complete alignment before operational use.*
