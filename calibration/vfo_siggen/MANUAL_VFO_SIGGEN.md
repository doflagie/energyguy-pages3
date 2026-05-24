# TECHNICAL MANUAL: VFO AND SIGNAL GENERATOR
## TM-SG-001  Revision A
### Multiband Coverage 1.8 MHz – 1.3 GHz; DDS, Si5351, ADF4351 Synthesis

---

**DISTRIBUTION:** Unrestricted  
**PREPARED BY:** Amateur Radio Technical Documentation Project  
**DATE:** 2026  
**SUPERSEDES:** None (Initial Release)

---

## TABLE OF CONTENTS

- [Chapter 1 — Introduction and Theory of Operation](#chapter-1)
- [Chapter 2 — DDS Signal Generator (AD9851)](#chapter-2)
- [Chapter 3 — Si5351 Multiband VFO](#chapter-3)
- [Chapter 4 — ADF4351 VHF/UHF/Microwave Synthesizer](#chapter-4)
- [Chapter 5 — Output Level Control and Calibration](#chapter-5)
- [Chapter 6 — Modulation: AM, FM, CW](#chapter-6)
- [Chapter 7 — Frequency Sweep Mode](#chapter-7)
- [Chapter 8 — Reference Oscillators and Frequency Stability](#chapter-8)
- [Chapter 9 — Firmware and CYD Interface](#chapter-9)
- [Chapter 10 — Construction and Alignment](#chapter-10)
- [Chapter 11 — Frequency Calibration Procedures](#chapter-11)
- [Chapter 12 — Troubleshooting](#chapter-12)
- [Appendix A — Band and Frequency Tables](#appendix-a)
- [Appendix B — Spurious Emission Specifications](#appendix-b)
- [Appendix C — Filter Design Data](#appendix-c)
- [Appendix D — Component Substitutions](#appendix-d)

---

<a name="chapter-1"></a>
## CHAPTER 1 — INTRODUCTION AND THEORY OF OPERATION

### 1.1 Purpose and Scope

This manual covers the design, construction, calibration, and operation of a portable multiband VFO and signal generator covering 1.8 MHz through 1.3 GHz, suitable for all HF, VHF, UHF, and lower microwave amateur radio bands.

Three synthesizer technologies are integrated into a single instrument:

| Synthesizer | Frequency Range | Technology | Resolution | Noise Floor |
|-------------|----------------|------------|------------|-------------|
| AD9851 DDS | 0 – 40 MHz | Direct Digital Synthesis | 0.042 Hz | −140 dBc/Hz |
| Si5351A | 8 kHz – 160 MHz | Fractional-N PLL × 3 outputs | ~0.001 Hz | −120 dBc/Hz |
| ADF4351 | 35 MHz – 4.4 GHz | Fractional-N PLL + VCO | 2.4 kHz | −100 dBc/Hz |

### 1.2 Theory: Direct Digital Synthesis (DDS)

A DDS generates a frequency by accumulating a phase value in a register at the system clock rate, using the accumulated phase to address a sine lookup table, and converting the result to an analog signal via a DAC.

**Phase accumulator:**
```
phase(n) = [phase(n-1) + FTW] mod 2^32

Where:
  FTW = Frequency Tuning Word = f_out × 2^32 / f_clk
  f_clk = AD9851 system clock (150 MHz with 6× multiplier)
```

**Frequency resolution:**
```
Δf = f_clk / 2^32 = 150,000,000 / 4,294,967,296 = 0.035 Hz per FTW LSB
```

This sub-0.1 Hz resolution is essentially infinite for most amateur radio purposes.

**DDS spurious output:**  
The DAC output contains:
1. Fundamental at f_out (desired)
2. Harmonics at N × f_out
3. Aliased images at N × f_clk ± f_out
4. Phase truncation spurs (24 bits of 32-bit accumulator drive the lookup table; 8 bits discarded produce deterministic spurious signals)

The reconstruction filter (7-pole low-pass, f_c = 35 MHz) suppresses items 2 and 3 above 35 MHz by more than 55 dB.

### 1.3 Theory: Phase-Locked Loop (PLL) Synthesis

A PLL synthesizer generates a frequency by locking a voltage-controlled oscillator (VCO) to a multiple of a stable reference frequency.

**Basic PLL equation:**
```
f_out = f_ref × (N + FRAC/MOD) / R

Where:
  f_ref = reference oscillator (TCXO)
  N     = integer divider (main divide ratio)
  FRAC  = fractional numerator (0 to MOD-1)
  MOD   = fractional modulus (2 to 4095 for ADF4351)
  R     = reference counter (1 for undivided reference)
```

**Si5351 additional divider:**  
The Si5351 adds a post-VCO output divider (4 to 900) enabling very low output frequencies from a 600–900 MHz internal VCO.

**ADF4351 output divider:**  
The ADF4351 VCO runs at 2.2–4.4 GHz; programmable dividers (÷1 through ÷64) extend coverage to 35 MHz minimum output.

**PLL phase noise:**  
Phase noise (PN) is the primary quality metric for synthesized oscillators.

```
PN(f) = PN_ref + 20·log₁₀(N) + F(f)

Where:
  PN_ref = reference oscillator phase noise
  N      = total frequency multiplication ratio
  F(f)   = noise contribution of PLL components

At 10 kHz offset, 146 MHz, N ≈ 58:
  20·log₁₀(58) = +35 dB degradation vs. reference oscillator
  If TCXO PN = −140 dBc/Hz at 10 kHz:
  Output PN ≈ −140 + 35 = −105 dBc/Hz (typical for Si5351 at 2M)
```

### 1.4 Instrument Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                      SIGNAL GENERATOR                           │
│                                                                  │
│  25 MHz TCXO ──┬──> AD9851 DDS ──────────────┐                  │
│                │                              │                  │
│                └──> Si5351A ─────────────────>├──> LPF/BPF bank │
│                                               │                  │
│  10 MHz TCXO ──────> ADF4351 PLL ────────────>│                  │
│                                               │                  │
│                                    [relay select]                │
│                                               │                  │
│                                         ERA-3SM amp              │
│                                               │                  │
│                                    π-att (10+20+40 dB)           │
│                                               │                  │
│                                    HMC1119 fine att              │
│                                               │                  │
│                                    AD8307 power monitor          │
│                                               │                  │
│                                         SMA SIGOUT               │
│                                                                  │
│  ESP32 (CYD) ──── SPI ──> DDS, ADF4351, HMC1119                 │
│               ──── I2C ──> Si5351, PCF8574 relays                │
│               ──── ADC ──> AD8307, battery, AF input            │
└─────────────────────────────────────────────────────────────────┘
```

---

<a name="chapter-2"></a>
## CHAPTER 2 — DDS SIGNAL GENERATOR (AD9851)

### 2.1 Specifications

| Parameter | Value | Notes |
|-----------|-------|-------|
| Frequency range | 0 Hz – 40 MHz | Above 40 MHz: spurious degrades |
| Frequency resolution | 0.035 Hz | At 150 MHz system clock |
| System clock | 150 MHz | 25 MHz TCXO × 6× internal multiplier |
| Output level | ~0 dBm | Into 50 Ω via 4:1 output transformer |
| Spurious (< 40 MHz) | < −50 dBc | After 7-pole LPF |
| Phase noise (1 kHz offset, 14 MHz) | −125 dBc/Hz | Typical |
| Output impedance | 50 Ω | |
| Tuning time | < 5 µs | SPI write to output |

### 2.2 Frequency Coverage — HF Ham Bands

| Band | Range | F_center | Notes |
|------|-------|---------|-------|
| 160M | 1.800–2.000 MHz | 1.900 MHz | Excellent; low DDS spur |
| 80M | 3.500–4.000 MHz | 3.750 MHz | Excellent |
| 60M | 5.330–5.406 MHz | 5.368 MHz | Excellent |
| 40M | 7.000–7.300 MHz | 7.150 MHz | Excellent |
| 30M | 10.100–10.150 MHz | 10.125 MHz | Excellent; narrow band |
| 20M | 14.000–14.350 MHz | 14.175 MHz | Excellent |
| 17M | 18.068–18.168 MHz | 18.118 MHz | Good |
| 15M | 21.000–21.450 MHz | 21.225 MHz | Good |
| 12M | 24.890–24.990 MHz | 24.940 MHz | Adequate |
| 10M | 28.000–29.700 MHz | 28.500 MHz | Use Si5351 above 30 MHz |

### 2.3 FTW Calculation

The Frequency Tuning Word (FTW) is the 32-bit value written to the AD9851:

```
FTW = round(f_out × 4,294,967,296 / 150,000,000)

Examples:
  f = 14.000 MHz: FTW = 14,000,000 × 4,294,967,296 / 150,000,000 = 401,043,867
  f = 7.100 MHz:  FTW = 7,100,000 × 4,294,967,296 / 150,000,000 = 203,388,413
  f = 1.900 MHz:  FTW = 1,900,000 × 4,294,967,296 / 150,000,000 = 54,466,756
```

### 2.4 Output Filter

The 7-pole elliptic low-pass filter passes 1.8–30 MHz with < 0.5 dB loss and rejects the 150 MHz system clock and its aliases by > 55 dB.

**Component values (50 Ω, f_c = 35 MHz):**

| Component | Value | Part |
|-----------|-------|------|
| L1, L4 | 220 nH | 15t #24 AWG on T50-6 |
| L2, L3 | 390 nH | 20t #24 AWG on T50-6 |
| C1, C4 | 68 pF NP0 | 0402 |
| C2, C3 | 100 pF NP0 | 0402 |

---

<a name="chapter-3"></a>
## CHAPTER 3 — Si5351 MULTIBAND VFO

### 3.1 Specifications

| Parameter | Value | Notes |
|-----------|-------|-------|
| Frequency range | 8 kHz – 160 MHz | CLK0 (single output) |
| Simultaneous outputs | 3 (CLK0/1/2) | Independent frequencies |
| Phase noise (100 kHz offset) | −120 dBc/Hz | Typical at 10 MHz |
| Phase noise (10 kHz offset) | −110 dBc/Hz | Typical at 100 MHz |
| Frequency resolution | ~0.001 Hz | 20-bit fractional |
| Reference input | 25 MHz TCXO | ±0.5 ppm |
| Output drive | 2/4/6/8 mA | Register selectable |
| Output type | CMOS square wave | Needs LPF for sine output |

### 3.2 Simultaneous Output Applications

The Si5351 enables three simultaneous clocks at different frequencies:

| Output | Typical Use | Example Frequency |
|--------|------------|------------------|
| CLK0 | Main VFO / carrier | 7.100 MHz |
| CLK1 | BFO for SSB/CW | 4.915 MHz |
| CLK2 | Calibration reference | 10.000 MHz |

**SSB BFO offset:**
- USB: CLK1 = IF_center + 1.400 kHz (e.g., 4.916.4 kHz)
- LSB: CLK1 = IF_center − 1.400 kHz (e.g., 4.913.6 kHz)

### 3.3 Band Coverage with Output Dividers

| Band | Freq (MHz) | PLL VCO (MHz) | Output Divider | Setting |
|------|-----------|--------------|---------------|---------|
| 6M | 50–54 | 800–864 | ÷16 | Best performance |
| 2M | 144–148 | 576–592 | ÷4 | Good; near spec limit |
| 1.25M | 222–225 | 888–900 | ÷4 | At spec limit; test required |

**Note:** Above 160 MHz, use ADF4351.

---

<a name="chapter-4"></a>
## CHAPTER 4 — ADF4351 VHF/UHF/MICROWAVE SYNTHESIZER

### 4.1 Specifications

| Parameter | Value | Notes |
|-----------|-------|-------|
| Frequency range | 35 MHz – 4.4 GHz | With output dividers |
| VCO range | 2.2 – 4.4 GHz | Internal VCO |
| Phase noise (10 kHz, 435 MHz) | −100 dBc/Hz | Typical |
| Phase noise (100 kHz, 435 MHz) | −115 dBc/Hz | Typical |
| Reference spurious | −65 to −75 dBc | At f_PD offset |
| Fractional spurious | −70 to −85 dBc | At FRAC/MOD offsets |
| Output power | −4 to +5 dBm | 4 steps (register controlled) |
| Lock time | 50–200 µs | Fractional-N |
| Reference input | 10 MHz TCXO | ±0.5 ppm |

### 4.2 Band Coverage

| Band | Freq Range | VCO Freq | Divider | Notes |
|------|-----------|---------|---------|-------|
| 6M | 50–54 MHz | 800–864 MHz | ÷16 | Use DDS or Si5351 preferred |
| 2M | 144–148 MHz | 576–592 MHz | ÷4 | Good performance |
| 1.25M | 222–225 MHz | 888–900 MHz | ÷4 | Excellent |
| 70cm | 420–450 MHz | 840–900 MHz | ÷2 | Excellent |
| 33cm | 902–928 MHz | 902–928 MHz | ÷1 | Direct VCO |
| 23cm | 1240–1300 MHz | 2480–2600 MHz | ÷2 | |
| 13cm | 2300–2450 MHz | 2300–2450 MHz | ÷1 | High power ADF4350 preferable |
| 9cm | 3300–3500 MHz | 3300–3500 MHz | ÷1 | Direct VCO; experimental |

### 4.3 Integer vs. Fractional-N Mode

**Integer-N mode (FRAC = 0):**
- No fractional spurs
- Frequency resolution = f_PD = 10 MHz (only multiples of 10 MHz available)
- Better phase noise than fractional mode
- Use for fixed-frequency applications or where step size can be large

**Fractional-N mode:**
- Resolution = f_PD / MOD (2.44 kHz at MOD=4095, f_PD=10 MHz)
- Fractional spurs at f_PD/MOD offsets (typically −75 dBc)
- Use when fine frequency steps required (VFO operation)

**Recommendation:** For VFO operation across 2M (144–148 MHz, span 4 MHz):
- f_PD = 10 MHz; MOD = 4095; step size = 2.44 kHz
- Provides 1638 steps across the 4 MHz band (adequate for most uses)

---

<a name="chapter-5"></a>
## CHAPTER 5 — OUTPUT LEVEL CONTROL AND CALIBRATION

### 5.1 Level Control Architecture

```
Synthesizer output (+0 to +5 dBm)
         │
    [ERA-3SM amplifier: +13 dBm max P1dB]  ← bypassed if not needed
         │
    [Switched π-attenuator: 0/10/20/40 dB — relay controlled]
         │
    [HMC1119 digital attenuator: 0–31.75 dB, 0.25 dB steps]
         │
    [AD8307 directional tap: power measurement]
         │
    SMA SIGOUT (50 Ω)
```

### 5.2 Output Level Range

| Attenuator state | Output level |
|-----------------|-------------|
| 0 dB (all bypassed) | +13 dBm |
| 10 dB switched | +3 dBm |
| 20 dB switched | −7 dBm |
| 30 dB switched | −17 dBm |
| 40 dB switched | −27 dBm |
| 50 dB switched | −37 dBm |
| 60 dB switched | −47 dBm |
| 70 dB switched | −57 dBm |
| 70 + 16 dB fine | −73 dBm |
| 70 + 31.75 dB fine | ~−88 dBm (beyond spec; noise floor limit) |

**Specified output range: −73 dBm to +13 dBm (86 dB total range)**

### 5.3 π-Attenuator Design Values

All resistors 1%, 0402 SMD (required for VHF accuracy):

| Section | R_series | R_shunt | Impedance |
|---------|---------|--------|----------|
| 10 dB | 35.1 Ω | 96.2 Ω | 50 Ω |
| 20 dB | 40.9 Ω | 61.1 Ω | 50 Ω |
| 40 dB | 49.5 Ω | 50.5 Ω | 50 Ω |

### 5.4 AD8307 Power Monitor Calibration

The AD8307 output voltage is proportional to RF input power in dBm:

```
V_out = slope × (P_in_dBm − intercept)
P_in_dBm = V_out / slope + intercept

Nominal values:
  slope     = 25 mV/dB (calibrate to actual; typically 23–27 mV/dB)
  intercept = −84 dBm (output = 0 V at −84 dBm input)
```

**Calibration procedure (Section 11.3):**  
Compare AD8307 reading against a known reference (NanoVNA, spectrum analyzer, or calibrated attenuator + known source) and adjust `cal_offset_dB` in firmware settings.

---

<a name="chapter-6"></a>
## CHAPTER 6 — MODULATION: AM, FM, CW

### 6.1 AM Modulation

**Method:** Audio-rate update of HMC1119 digital attenuator.

```
Carrier with AM modulation:
  v(t) = A_c [1 + m·a(t)] · cos(2πf_c·t)

  Where m = modulation depth (0 to 1 = 0 to 100%)
  a(t) = normalized audio signal (−1 to +1)
```

**Implementation:**  
At each audio sample (8 kHz), read the AF input ADC, compute envelope, and update HMC1119:
```
envelope = 1 + m × (audio_sample − 2048) / 2048
atten_dB = atten_base − 20 × log10(envelope)
```

**Audio bandwidth:** Limited to 4 kHz (Nyquist at 8 kHz sample rate). Adequate for AM voice quality.

**Modulation depth setting:** Adjust `am_depth` parameter (0.0 to 1.0 = 0 to 100%).

### 6.2 FM Modulation

**DDS FM (HF bands):**  
Update AD9851 FTW register at audio sample rate. Deviation proportional to audio amplitude.

```
FTW_offset = audio_normalized × (fm_deviation_hz × 2^32 / f_clk)
FTW_out = FTW_carrier + FTW_offset
```

**Update rate:** 8 kHz. SPI write time: ~2 µs. No bandwidth limitation from SPI.

**FM deviation settings:**

| Mode | Deviation | Application |
|------|-----------|-------------|
| NBFM | ±2.5 kHz | VHF repeater input |
| WBFM | ±75 kHz | Broadcast FM test |
| Narrow CW chirp | ±100 Hz | Filter measurement |
| Custom | Programmable | Any deviation |

**ADF4351 FM (VHF/UHF):**  
Update FRAC register. Minimum deviation step = f_PD/MOD = 10 MHz/4095 = 2.44 kHz.
Not suitable for NBFM (step too coarse). Use for coarse frequency shift keying only.

### 6.3 CW Keying

**Shaped keying** prevents key clicks that would cause splatter on adjacent frequencies.

```
Rise/fall shape: raised cosine (Hanning window)
Rise time: 5 ms (ARRL recommended minimum for clean CW)
```

**Implementation:** HMC1119 attenuation ramped over 20 steps × 0.25 ms each = 5 ms rise.

**Connecting a key:**
- Mechanical key: connect between KEY jack and GND
- Keyer: connect paddle output (dit/dah lines) to KEY jack
- Computer keying: GPIO32 controlled by software (set GPIO LOW for key-down)

**Sidetone:** Generated by ESP32 LEDC peripheral. Frequency adjustable 400–900 Hz.

---

<a name="chapter-7"></a>
## CHAPTER 7 — FREQUENCY SWEEP MODE

### 7.1 Overview

Sweep mode steps the output frequency from a start frequency to a stop frequency, measuring output level (and optionally external return loss) at each step.

**Primary applications:**
- Filter frequency response measurement (plot insertion loss vs. frequency)
- Antenna SWR sweep (external bridge required)
- Crystal filter characterization
- Bandpass filter tuning
- Attenuator frequency flatness verification

### 7.2 Sweep Parameters

| Parameter | Range | Default | Notes |
|-----------|-------|---------|-------|
| Start frequency | 1 Hz – 4 GHz | 1 MHz | Must be < Stop |
| Stop frequency | 1 Hz – 4 GHz | 30 MHz | |
| Step size | 1 Hz – 1 MHz | Auto | 0 = 500 evenly-spaced points |
| Dwell time | 1–1000 ms | 10 ms | Per step; longer = more averaging |
| Scale | Linear / Log | Linear | Log for wide spans |
| Output mode | CSV serial / CYD display | Both | |

### 7.3 Filter Measurement Procedure

**Equipment:**
- Signal generator (this unit)
- AD8307-based level meter (built-in) or external spectrum analyzer

**Simple insertion loss measurement:**

1. Set sweep range to cover filter passband + 1 decade each side.
2. Connect SIGOUT directly to AD8307 (reference sweep, no filter).  Record peak level P_ref.
3. Insert filter between SIGOUT and measurement point.
4. Run sweep again.  Record attenuation at each frequency as P_ref − P_measured.
5. CYD display shows difference curve automatically if reference stored.

**−3 dB bandwidth reading:**  
The firmware calculates and displays the −3 dB bandwidth after each sweep.

### 7.4 Serial Output Format

Sweep data is streamed as CSV over USB serial (115200 baud):

```
freq_hz,level_dBm
1000000,-3.24
1010000,-3.21
...
30000000,-47.82
```

Import into Python/pandas, GNU Octave, or LibreOffice Calc for plotting.

**Python quick-plot:**
```python
import pandas as pd, matplotlib.pyplot as plt
df = pd.read_csv('sweep.csv')
plt.plot(df.freq_hz/1e6, df.level_dBm)
plt.xlabel('Frequency (MHz)'); plt.ylabel('Level (dBm)')
plt.grid(True); plt.show()
```

---

<a name="chapter-8"></a>
## CHAPTER 8 — REFERENCE OSCILLATORS AND FREQUENCY STABILITY

### 8.1 Reference Oscillator Options

| Type | Accuracy | Warm-up | Cost | Notes |
|------|---------|---------|------|-------|
| Crystal XO | ±30–100 ppm | Instant | $0.50 | Adequate for most use |
| TCXO | ±0.5–2 ppm | Instant | $5–15 | Recommended standard |
| OCXO | ±0.01–0.1 ppm | 5–15 min | $20–100 | For precision calibration |
| GPSDO | ±0.001 ppm | GPS lock | $50–200 | Best; requires GPS antenna |

**Recommended for this instrument:** 25 MHz TCXO (±0.5 ppm) for Si5351/DDS, plus 10 MHz TCXO (±0.5 ppm) for ADF4351.

**Frequency error at ±0.5 ppm:**

| Band | Frequency | Error at ±0.5 ppm |
|------|-----------|------------------|
| 160M | 1.900 MHz | ±0.95 Hz |
| 20M | 14.175 MHz | ±7.1 Hz |
| 2M | 146.000 MHz | ±73 Hz |
| 70cm | 435.000 MHz | ±217 Hz |
| 23cm | 1296.000 MHz | ±648 Hz |

For all HF bands, ±0.5 ppm TCXO accuracy is more than adequate. For microwave, upgrade to OCXO or GPS-disciplined reference.

### 8.2 Si5351 Correction Factor

The Si5351 library accepts a frequency correction in parts-per-billion (ppb):

```cpp
si5351.set_correction(offset_ppb, SI5351_PLL_INPUT_XO);
```

**Determining correction:**
1. Set Si5351 to 10.000 MHz output.
2. Compare against a GPS-disciplined reference (WWV, GPS receiver 1 PPS).
3. Measure frequency offset: Δf Hz.
4. Correction = Δf / 10,000,000 × 10^9 ppb.
5. Example: measured 10,000,050 Hz → correction = +50,000 ppb.

### 8.3 Frequency Drift vs. Temperature

TCXO drift (typical):
- −10°C to +60°C: < ±0.5 ppm
- After power-on: < 0.1 ppm change per hour (no warm-up required)

Crystal (no temperature compensation):
- Drift: ±5–50 ppm over operating temperature range
- After power-on: 1–5 ppm drift in first 5 minutes

For field use: allow 5 minutes warmup before critical measurements, even with TCXO.

---

<a name="chapter-9"></a>
## CHAPTER 9 — FIRMWARE AND CYD INTERFACE

### 9.1 CYD Display Screens

**SCREEN 1 — VFO (Main):**
- Large frequency display (MHz.kHz format)
- Band indicator and synthesizer source
- RF ON/OFF toggle
- Output level (set and measured)
- Modulation mode indicator
- Battery voltage
- Tuning step indicator

**SCREEN 2 — SWEEP:**
- Real-time sweep trace (frequency vs. level dBm)
- Peak frequency and level
- −3 dB bandwidth calculation and display
- Sweep start/stop/step controls

**SCREEN 3 — LEVEL:**
- Output level bar (−73 to +13 dBm)
- Switched attenuator status (10/20/40 dB)
- Fine attenuator setting
- AD8307 measured level
- Amplifier bypass status

**SCREEN 4 — MOD:**
- Modulation mode selection (NONE/AM/FM/CW)
- FM deviation setting
- AM depth setting
- CW key status (real-time UP/DOWN)

**SCREEN 5 — SETTINGS:**
- Calibration offset (±dB)
- Reference oscillator status
- Sweep range configuration
- Encoder step mode

### 9.2 Encoder Operation

| Action | VFO screen | Level screen | Settings screen |
|--------|-----------|-------------|----------------|
| Rotate CW | Frequency up | Fine atten up | Cal offset up |
| Rotate CCW | Frequency down | Fine atten down | Cal offset down |
| Short press | Cycle tuning step | — | — |
| Hold (> 1s) | Cycle band | Toggle amp bypass | Save settings |

**Tuning steps (HF):** 1 Hz → 10 Hz → 100 Hz → 1 kHz → 10 kHz → 100 kHz → 1 MHz → 10 MHz

### 9.3 Serial Commands

All commands are sent at 115200 baud, terminated with newline:

| Command | Description | Example |
|---------|-------------|---------|
| `freq <Hz>` | Set output frequency | `freq 14175000` |
| `level <dBm>` | Set output level | `level -20` |
| `rf on\|off` | Enable/disable RF | `rf on` |
| `sweep <f1> <f2> <step> <dwell>` | Run sweep | `sweep 1000000 30000000 100000 10` |
| `cal <dB>` | Set calibration offset | `cal -0.5` |
| `save` | Save settings to NVS | `save` |
| `status` | Print current status | `status` |
| `help` | List commands | `help` |

### 9.4 Required Libraries

Install via Arduino IDE Library Manager:

| Library | Author | Version |
|---------|--------|---------|
| TFT_eSPI | Bodmer | ≥2.5 |
| Etherkit Si5351 | Jason Milldrum (NT7S) | ≥2.1 |
| Wire | Built-in | — |
| SPI | Built-in | — |
| Preferences | Built-in | — |

**TFT_eSPI configuration (User_Setup.h):**
```cpp
#define ILI9341_DRIVER
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST  -1
#define SPI_FREQUENCY   40000000
#define SPI_READ_FREQUENCY 20000000
#define SPI_TOUCH_FREQUENCY 2500000
#define TOUCH_CS 33
```

---

<a name="chapter-10"></a>
## CHAPTER 10 — CONSTRUCTION AND ALIGNMENT

### 10.1 PCB Layout Guidelines

**Critical layout rules for RF circuitry:**

1. **Ground plane:** Full copper pour on bottom layer. Stitch vias every 5 mm around RF signal paths.

2. **Power supply decoupling:** Place 100 nF capacitor within 2 mm of every IC power pin. Place 10 µF capacitor within 10 mm of each major IC.

3. **RF signal traces:** Keep all RF signal paths < 25 mm. Use 50 Ω trace width (2.9 mm on FR-4, 1.6 mm, ε_r=4.4). Do not route RF traces over split in ground plane.

4. **AD9851 analog/digital ground:** Split ground plane under AD9851 between AGND (pins 6,7) and DGND (pins 27,29). Join at single star point near decoupling capacitors.

5. **ADF4351:** VP pin (VCO power) on isolated power island with LC filter (1 µH + 10 µF). This is the most noise-sensitive node in the design.

6. **Output attenuator:** All π-attenuator resistors must be 0402 SMD. Through-hole resistors have unacceptable lead inductance above 50 MHz.

7. **SMA connectors:** Use edge-mount SMA for < 3 GHz; 2.92mm (K-connector) for 23cm band.

8. **Shield walls:** For ADF4351 and output filter, consider solderable brass shield walls (Tin-Plated Steel, 0.2 mm) soldered to PCB ground plane.

### 10.2 Initial Power-Up Sequence

1. **Before applying power:** Verify all bypass capacitors installed. Check for shorts between VDD and GND with ohmmeter (> 100 Ω expected).

2. **First power-up:** Apply 3.3V via bench supply with 500 mA current limit. Measure current. Expected: 150–250 mA (ESP32 dominant). If > 400 mA, shut down immediately and check for short.

3. **Verify 5V boost:** Measure 5V rail after boost converter. Should read 4.9–5.1V. Check TPS61023 EN pin is high.

4. **TCXO verification:** Probe TCXO output on oscilloscope. Should show 25 MHz sine wave or clipped square, 0.3–3.3 V p-p.

5. **Si5351 first test:** Send I2C command `si5351.set_freq(1400000000ULL, SI5351_CLK0)`. Probe CLK0 output on oscilloscope. Should show 14 MHz square wave.

6. **AD9851 first test:** Write FTW for 10 MHz. Measure IOUT with spectrum analyzer or frequency counter. Should show 10 MHz ± 5 ppm.

7. **ADF4351 first test:** Program for 146 MHz, ÷4 divider. Check LD pin HIGH (locked). Measure output on spectrum analyzer.

### 10.3 Shield Can Installation

Solder 50 × 50 mm tinned steel shield can over ADF4351 + loop filter:
- Solder tabs to ground plane around perimeter
- Close lid only after confirming correct operation
- Add vent hole (0.5 mm) to prevent thermal issues

---

<a name="chapter-11"></a>
## CHAPTER 11 — FREQUENCY CALIBRATION PROCEDURES

### 11.1 Required Equipment

**Minimum:**
- AM/FM receiver (to verify AM broadcast frequency accuracy)
- Frequency counter (for HF) or known-good VFO for comparison

**Recommended:**
- GPS-disciplined frequency counter (e.g., GPS-disciplined GPSDO reference)
- Spectrum analyzer
- NanoVNA (for VHF/UHF power level calibration)

### 11.2 DDS Calibration (25 MHz TCXO)

**Method 1 — WWV comparison:**
1. Set DDS output to 10.000000 MHz.
2. Mix with WWV 10 MHz signal (AM broadcast; tune shortwave radio to 10 MHz).
3. Listen for zero-beat or measure audio difference tone.
4. The audio frequency = DDS error.
5. Correct: adjust `cal_offset_dB` or modify DDS_SYSCLK_HZ constant.

**Method 2 — GPS 1PPS gating:**
1. Connect GPS 1PPS output to ESP32 GPIO32.
2. Add gate-timing code: count DDS CLK pulses between two 1PPS rising edges.
3. Count should equal exactly 150,000,000 (for 150 MHz TCXO×6).
4. If differs by Δ: new SYSCLK = 150,000,000 × (1 + Δ/150,000,000).

**Method 3 — Spectrum analyzer reference:**
1. Set DDS to 14.000 MHz exactly.
2. Compare to spectrum analyzer internal reference (if calibrated).
3. Read frequency directly from spectrum analyzer peak.
4. Error = measured − 14.000000 MHz.
5. Correction: DDS_SYSCLK_HZ × (1 − error/14e6).

### 11.3 AD8307 Level Calibration

**Procedure:**
1. Connect SIGOUT to calibrated power meter or known-good spectrum analyzer.
2. Set output level to 0.0 dBm (all attenuators, ERA-3SM in path).
3. Read actual level on calibrated instrument: P_actual.
4. Calibration offset = P_actual − 0.0 dBm.
5. Enter as `cal_offset_dB` in Settings screen or via serial `cal <value>`.

**Calibration at multiple levels:**
Perform at +10, 0, −10, −30 dBm and record. If slope error > 0.5 dB/decade, adjust AD8307_SLOPE constant in firmware.

### 11.4 Si5351 TCXO Calibration

1. Enable CLK2 as calibration output: `si5351.set_freq(1000000000ULL, SI5351_CLK2)` (10 MHz output).
2. Measure CLK2 on frequency counter.
3. If measured = 10,000,150 Hz (example): offset = +150 Hz at 10 MHz = +15 ppb.
4. Apply correction: `si5351.set_correction(15000, SI5351_PLL_INPUT_XO)`.
5. Re-measure; iterate until error < 1 Hz at 10 MHz.

**Storing calibration:**
```cpp
prefs.putInt("si5351_cal", correction_ppb);
// On startup:
int corr = prefs.getInt("si5351_cal", 0);
si5351.set_correction(corr, SI5351_PLL_INPUT_XO);
```

### 11.5 ADF4351 Reference Calibration

The ADF4351 reference is set by the 10 MHz TCXO.

1. Set ADF4351 to 432.100 MHz (70cm calling frequency).
2. Compare to frequency counter (GPS-referenced if possible).
3. Frequency error: Δf Hz.
4. TCXO correction: Δf/432.1e6 ppm.
5. Correct by selecting a TCXO with lower tolerance, or trim VCO with reference divider.

**Reference divider approach:**  
If TCXO is measured at 10.000050 MHz (50 Hz high), adjust R-counter:
- R=1: f_PD = 10.000050 MHz → slight error in all outputs
- No in-loop correction available; use better TCXO or add GPS discipline.

### 11.6 Output Level Calibration Record Sheet

```
Date: _______________   Instrument S/N: _______________

Reference instrument: _______________________________
Reference instrument cal date: _____________________

LEVEL CALIBRATION TABLE:
Set (dBm)  Measured (dBm)  Error (dB)  Att setting
---------  --------------  ----------  -----------
  +10.0    ___________     _______     amp in, 0dB att
   0.0     ___________     _______     amp in, 10dB att
  -10.0    ___________     _______     amp in, 20dB att
  -20.0    ___________     _______     amp in, 30dB att
  -30.0    ___________     _______     amp in, 40dB att
  -40.0    ___________     _______     amp in, 50dB att
  -50.0    ___________     _______     60 dB att
  -60.0    ___________     _______     70 dB att + 0 fine
  -70.0    ___________     _______     70 dB + fine 13 dB
  -73.0    ___________     _______     70 dB + fine 16 dB

Calibration offset applied: _______ dB
```

---

<a name="chapter-12"></a>
## CHAPTER 12 — TROUBLESHOOTING

### 12.1 No Output

| Symptom | Check | Action |
|---------|-------|--------|
| No output at SMA | RF OFF state | Set RF to ON via CYD or `rf on` command |
| No output | HMC1119 full att | Check fine_att_db in firmware settings |
| No output | ERA-3SM failed | Check 5V rail, R_bias current; replace if no output |
| No output from DDS | TCXO not running | Probe TCXO output; check 3.3V supply |
| No output from Si5351 | CLK0 disabled | Call `si5351.output_enable(SI5351_CLK0, 1)` |
| No output from ADF4351 | Not locked | Check LD pin; check reference; rebuild registers |

### 12.2 Frequency Wrong

| Symptom | Cause | Fix |
|---------|-------|-----|
| DDS 10× too high | Wrong SYSCLK constant | Set DDS_SYSCLK_HZ = actual f_clk |
| DDS ±ppm error | TCXO not exactly 25 MHz | Calibrate per Section 11.2 |
| Si5351 off by fixed amount | Wrong correction | Set si5351.set_correction() per Section 11.4 |
| ADF4351 not locking | Wrong register sequence | Write R5→R4→R3→R2→R1→R0 in order |
| ADF4351 wrong frequency | INT/FRAC calculation error | Verify VCO = f_PD × (INT + FRAC/MOD) |

### 12.3 High Spurious Output

| Symptom | Cause | Fix |
|---------|-------|-----|
| Strong harmonic(s) | LPF not selected | Check relay K_lpf; verify selectLPF() called |
| Reference spur at ±10 MHz | ADF4351 spur (normal) | Add external BPF; spur is inherent to design |
| DDS alias at 150 MHz − f | LPF cutoff too high | Use 7-pole filter as designed |
| Broadband noise floor high | ADF4351 VP noise | Check LC filter on VP pin (pin 9) |

### 12.4 Level Control Issues

| Symptom | Cause | Fix |
|---------|-------|-----|
| Level too high, can't reduce | HMC1119 not responding | Check SPI; GPIO17 CS; power |
| Level steps wrong | Relay not switching | Check PCF8574 I2C address (0x20); check relay drive MOSFET |
| AD8307 reads wrong level | Cal offset incorrect | Recalibrate per Section 11.3 |
| AD8307 reads constant | ADC input floating | Verify GPIO36 connected to AD8307 OUT via 12.5 kΩ |

### 12.5 CYD Display Issues

| Symptom | Cause | Fix |
|---------|-------|-----|
| Blank display | TFT not initializing | Check User_Setup.h pin assignments for CYD |
| Touch not working | Touch CS wrong | TOUCH_CS must be GPIO33 for CYD |
| Display glitching | SPI conflict with synthsizers | Verify synths use VSPI (GPIO18/23); TFT uses HSPI |
| Encoder not responding | Interrupt not firing | Verify GPIO26/27 not used by TFT; check pull-ups |

---

<a name="appendix-a"></a>
## APPENDIX A — BAND AND FREQUENCY TABLES

### A.1 Ham Band Quick Reference

| Band | Freq Range | Calling | Synth | LPF f_c |
|------|-----------|---------|-------|---------|
| 160M | 1.800–2.000 MHz | 1.900 MHz | DDS | 2.5 MHz |
| 80M | 3.500–4.000 MHz | 3.750 MHz | DDS | 4.5 MHz |
| 60M | 5.330–5.406 MHz | 5.368 MHz | DDS | 8.5 MHz |
| 40M | 7.000–7.300 MHz | 7.200 MHz | DDS | 8.5 MHz |
| 30M | 10.100–10.150 MHz | 10.125 MHz | DDS | 20 MHz |
| 20M | 14.000–14.350 MHz | 14.225 MHz | DDS | 20 MHz |
| 17M | 18.068–18.168 MHz | 18.118 MHz | DDS | 20 MHz |
| 15M | 21.000–21.450 MHz | 21.225 MHz | DDS | 35 MHz |
| 12M | 24.890–24.990 MHz | 24.940 MHz | DDS | 35 MHz |
| 10M | 28.000–29.700 MHz | 28.500 MHz | DDS | 35 MHz |
| 6M | 50.000–54.000 MHz | 52.525 MHz | Si5351 | 60 MHz |
| 2M | 144.000–148.000 MHz | 146.520 MHz | Si5351 | 160 MHz |
| 1.25M | 222.000–225.000 MHz | 223.500 MHz | ADF4351 | BPF |
| 70cm | 420.000–450.000 MHz | 432.100 MHz | ADF4351 | BPF |
| 33cm | 902.000–928.000 MHz | 915.000 MHz | ADF4351 | BPF |
| 23cm | 1240–1300 MHz | 1296.100 MHz | ADF4351 | BPF |

---

<a name="appendix-b"></a>
## APPENDIX B — SPURIOUS EMISSION SPECIFICATIONS

### B.1 DDS (AD9851, 150 MHz clock)

| Spurious type | Level | Notes |
|--------------|-------|-------|
| 2nd harmonic (after LPF) | < −50 dBc | LPF reduces 2× above f_c |
| 3rd harmonic | < −60 dBc | LPF strongly attenuates |
| DAC alias (f_clk − f) | < −55 dBc | LPF reduces to < −70 dBc at 2× f |
| Phase truncation spurs | −60 to −80 dBc | Near fundamental |
| Noise floor | −140 dBc/Hz | At 1 kHz offset |

### B.2 Si5351 (CMOS output, with LPF)

| Spurious type | Level (no filter) | Level (with LPF) |
|--------------|------------------|-----------------|
| 3rd harmonic | −20 dBc | < −50 dBc |
| 5th harmonic | −30 dBc | < −60 dBc |
| PLL reference spurs | −80 dBc | < −80 dBc |
| Sub-harmonic | −60 dBc | < −60 dBc |

### B.3 ADF4351

| Spurious type | Level | Notes |
|--------------|-------|-------|
| Reference spur | −65 to −75 dBc | At f_PD offsets |
| Fractional spurs | −70 to −85 dBc | At f_PD/MOD offsets |
| 2nd harmonic | −20 dBc (no filter) | Add BPF for better performance |
| Phase noise floor | −150 dBc/Hz | |

---

<a name="appendix-c"></a>
## APPENDIX C — FILTER DESIGN DATA

### C.1 Low-Pass Filter Values (7-Pole, 50 Ω)

| f_c (MHz) | L1=L4 (nH) | L2=L3 (nH) | C1=C4 (pF) | C2=C3 (pF) |
|----------|-----------|-----------|-----------|-----------|
| 2.5 | 3300 | 5600 | 1000 | 1500 |
| 4.5 | 1800 | 3300 | 560 | 820 |
| 8.5 | 1000 | 1800 | 330 | 470 |
| 20 | 390 | 680 | 120 | 180 |
| 35 | 220 | 390 | 68 | 100 |
| 60 | 120 | 220 | 39 | 56 |
| 160 | 47 | 82 | 15 | 22 |

*All inductors wound on T50-6 (yellow) toroids. All capacitors NP0/C0G, 0402.*

### C.2 Toroid Winding Reference (T50-6, A_L = 3.6 nH/N²)

| L (nH) | Turns | Wire |
|--------|-------|------|
| 100 | 5 | #24 AWG |
| 220 | 8 | #24 AWG |
| 390 | 10 | #24 AWG |
| 680 | 14 | #26 AWG |
| 1000 | 17 | #26 AWG |
| 1800 | 22 | #26 AWG |
| 3300 | 30 | #28 AWG |
| 5600 | 39 | #28 AWG |

*Formula: N = √(L_nH / A_L) where A_L = 3.6 nH/N² for T50-6*

---

<a name="appendix-d"></a>
## APPENDIX D — COMPONENT SUBSTITUTIONS

| Original | Substitute | Notes |
|---------|-----------|-------|
| AD9851 | AD9850 | Max 40 MHz (no 6× mult); use 125 MHz external clock |
| Si5351A | Si5351B or C | B adds VCXO, C adds external ref input |
| ADF4351 | ADF4350 | Min 137.5 MHz (no 70 MHz divider path) |
| ERA-3SM | GALI-39+ | Higher P1dB (+16 dBm), better at 1 GHz |
| ERA-3SM | MAR-6SM | Lower gain (20 dB) but wide bandwidth to 2 GHz |
| HMC1119 | PE4302 | 0.5 dB steps (vs 0.25 dB); DC–3 GHz |
| 10 MHz TCXO | 10 MHz OCXO | Better stability; needs warm-up |
| G6K-2F-Y relay | IM23-5 | Smaller; same specs; 5V coil |
| T50-6 toroid | T37-6 | Same material (Iron Powder #6); smaller; slightly lower Q |

---

*End of TM-SG-001 Rev A*

*For issues or corrections, file at the project repository.*
