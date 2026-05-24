# TECHNICAL MANUAL — PORTABLE CW/SSB AUDIO FILTER

**TM-AF-001  Revision A**
**Passive LC, Active Op-Amp, DSP, and Adaptive Notch Audio Filters**
**For CW and SSB Amateur Radio Operation**

---

**CLASSIFICATION:** Unclassified / For General Distribution
**DATE:** 2026-04-25
**SUPERSEDES:** None (initial issue)
**APPLICABLE EQUIPMENT:** AF-1 Passive LC Filter, AF-2 Active CW Filter,
AF-3 Variable Bandwidth Filter, AF-4 Notch Filter, AF-5 DSP Filter

---

## TABLE OF CONTENTS

- [Chapter 1 — Introduction and Theory of Audio Filtering](#chapter-1)
- [Chapter 2 — Passive LC SSB Bandpass Filter (AF-1)](#chapter-2)
- [Chapter 3 — Active Op-Amp CW Bandpass Filter (AF-2)](#chapter-3)
- [Chapter 4 — Variable Bandwidth Filter with Switchable Q (AF-3)](#chapter-4)
- [Chapter 5 — Notch Filters — Manual and Adaptive (AF-4)](#chapter-5)
- [Chapter 6 — DSP Audio Filter — ESP32 Implementation (AF-5)](#chapter-6)
- [Chapter 7 — Enclosure and Mechanical Assembly](#chapter-7)
- [Chapter 8 — Alignment and Test Procedures](#chapter-8)
- [Chapter 9 — Filter Response Plots and Insertion Loss](#chapter-9)
- [Chapter 10 — Battery Operation and Power Management](#chapter-10)
- [Chapter 11 — Troubleshooting](#chapter-11)
- [Appendix A — Component Reference and Substitution Guide](#appendix-a)
- [Appendix B — Filter Design Equations and Tables](#appendix-b)
- [Appendix C — FIR Coefficient Generation (Python/scipy)](#appendix-c)
- [Appendix D — Parts Lists Master Index](#appendix-d)

---

<a name="chapter-1"></a>
## CHAPTER 1 — INTRODUCTION AND THEORY OF AUDIO FILTERING

### 1.1 Purpose

This manual describes the design, construction, alignment, and operation of
portable audio filters for use with CW and SSB amateur radio receivers and
transceivers.  Audio filtering reduces interference, removes adjacent-channel
splatter, eliminates heterodyne beat notes from carriers, and shapes the received
audio to match hearing requirements for CW copying accuracy.

The filter suite documented here covers four hardware topologies and one firmware-
based DSP implementation:

| Unit | Designation | Type           | Bandwidth      | Primary Use     |
|------|-------------|----------------|----------------|-----------------|
| AF-1 | Passive LC  | LC ladder      | 300–3000 Hz    | SSB passband    |
| AF-2 | Active CW   | SVF op-amp     | 28–140 Hz      | CW selectivity  |
| AF-3 | Variable BW | 4th-order SVF  | 200–2700 Hz    | Multi-mode      |
| AF-4 | Notch       | Twin-T / LMS   | 50 Hz null     | Heterodyne kill |
| AF-5 | DSP ESP32   | FIR/IIR/LMS    | Programmable   | All modes       |

### 1.2 Audio Frequency Fundamentals

**Voice (SSB):** Intelligible speech occupies 300–3000 Hz.  Reducing the upper
limit below 2500 Hz reduces hiss without appreciable intelligibility loss.
Reducing below 1800 Hz causes a "telephone" quality reduction acceptable for DX
contacts.

**CW:** Morse code consists of a single carrier frequency shifted on and off.
The operator selects a pitch (typically 400–800 Hz) via receiver offset (RIT) or
BFO.  A narrowband filter centered on that pitch improves signal-to-noise ratio
directly: for Gaussian noise, SNR improvement = 10 × log₁₀(BW_original / BW_filter).

Example: reducing from 2700 Hz SSB bandwidth to 500 Hz CW filter:
  SNR improvement = 10 × log₁₀(2700/500) = 7.3 dB

**Heterodyne (birdie, beat note):** When a carrier or an image falls within the
passband, it produces a steady audio tone at the beat frequency.  A notch filter
at that exact frequency can provide 30–60 dB rejection.

### 1.3 Filter Parameter Definitions

**Center frequency (f₀):** The geometric mean of the lower and upper −3 dB points.
  f₀ = √(f_lo × f_hi)

**Bandwidth (BW):** f_hi − f_lo at −3 dB points.

**Quality factor (Q):** Q = f₀ / BW.  High Q = narrow, selective filter.

**Order (n):** Number of reactive elements (poles).  Each pole contributes 6 dB/
octave rolloff outside the passband.  A 4th-order filter has 24 dB/octave.

**Insertion loss (IL):** IL = 20 × log₁₀(V_out / V_in) at passband center with
matched source and load impedances.  Passive filters have inherent IL; active
filters can be designed for IL = 0 dB or gain.

**Stopband attenuation:** Minimum attenuation at specified frequencies outside
the passband.

**Group delay:** Time delay of envelope through filter.  Non-constant group delay
causes phase distortion.  Critical for SSB intelligibility if variations exceed
~1 ms across the voice band.  FIR filters are linear phase (constant group delay);
IIR filters are not.

### 1.4 Filter Topology Comparison

| Topology         | IL     | Selectivity | Phase   | Tunable | Battery |
|------------------|--------|-------------|---------|---------|---------|
| Passive LC       | 1–2 dB | Moderate    | Good    | Fixed   | None    |
| Active SVF       | 0 dB   | High        | Mod     | Yes     | ~35 hr  |
| FIR (DSP)        | 0 dB   | Very high   | Linear  | Yes     | ~6.6 hr |
| IIR (DSP)        | 0 dB   | High        | Nonlin  | Yes     | ~6.6 hr |
| LMS Adaptive     | 0 dB   | Auto-null   | Varies  | Auto    | ~6.6 hr |

---

<a name="chapter-2"></a>
## CHAPTER 2 — PASSIVE LC SSB BANDPASS FILTER (AF-1)

### 2.1 Design Overview

The AF-1 passive LC filter provides a 300–3000 Hz audio bandpass for SSB use
with a 600 Ω source and load impedance.  The design cascades:
- A 3rd-order Butterworth highpass section at 300 Hz
- A 3rd-order Butterworth lowpass section at 3000 Hz

This yields a 6th-order composite bandpass with −18 dB/octave rolloffs on both
sides of the passband.  The filter requires no power supply.

### 2.2 Component Values

**Highpass section (300 Hz, 600 Ω):**

| Ref | Value  | Function             |
|-----|--------|----------------------|
| C1  | 820 nF | Series highpass cap  |
| L2  | 150 mH | Shunt inductor       |
| C3  | 820 nF | Series highpass cap  |

**Lowpass section (3000 Hz, 600 Ω):**

| Ref | Value  | Function              |
|-----|--------|-----------------------|
| L4  | 33 mH  | Series lowpass ind.   |
| C5  | 180 nF | Shunt capacitor       |
| L6  | 33 mH  | Series lowpass ind.   |

### 2.3 Inductor Construction

**L2 (150 mH):** Wind 412 turns of #32 AWG enamel on two stacked T-94A-77 (Mix 77)
toroids.  A_L = 884 nH/N² per core; stacked doubles A_L to ~1768 nH/N².
  N = √(150e6 / 1768) ≈ 291 turns (adjusted; verify with LC meter).
  DCR target: ≤ 60 Ω.
  Split winding into two equal layers to reduce winding capacitance.

**L4, L6 (33 mH each):** Wind 137 turns of #28 AWG on single T-94A-77.
  N = √(33e6 / 884) = 193 turns.  DCR target: ≤ 18 Ω.

Alternative: Bourns SRR1260 series wirewound power chokes (pre-made, low cost).
  SRR1260-151Y = 150 mH nominal (acceptable for L2).
  SRR1260-330Y = 33 mH nominal (for L4, L6).
  Verify A_C (self-resonant frequency) > 20 kHz before use.

### 2.4 Frequency Derivation

Highpass −3 dB frequency:
  f_HP = 1 / (2π × √(L2 × C1)) [for ladder; this is an approximation]
  Exact: determined by prototype normalization.  With values above, f_HP ≈ 295–310 Hz.

Lowpass −3 dB frequency:
  f_LP ≈ 1 / (2π × √(L4 × C5)) ≈ 2900–3100 Hz with values above.

Tolerance of 5% capacitors and 10% inductors can shift cutoffs ±10%.
For precise cutoffs, measure components and adjust C1/C3 within ±15% of nominal.

### 2.5 Impedance Matching

The filter is designed for 600 Ω.  Most modern radio headphone outputs present
8–32 Ω (driving headphones directly) or 600 Ω (classic AF outputs).

**Input matching for low-impedance source (8–32 Ω):**
  Use input transformer T1: 8Ω:600Ω or 32Ω:600Ω (Xicon 42TU series).
  T1 winding ratio: N = √(600/Z_source) = √(600/8) = 8.66:1

**Output matching for headphones (32–600 Ω):**
  If driving 32 Ω headphones from 600 Ω filter output:
  Add output transformer T2: 600Ω:32Ω
  Or accept ~18 dB additional insertion loss due to impedance mismatch
  (Ls = 10 × log₁₀(4 × 600 × 32 / (600+32)²) = −16.3 dB mismatch loss).

**Practical recommendation:** Insert this filter between the receiver audio out
and a 600 Ω amplifier stage, or between an auxiliary headphone amplifier and
headphones via output transformer.

### 2.6 Insertion Loss Measurement

**Required equipment:**
- Audio oscillator or signal generator (TM-SG-001 recommended)
- AC voltmeter or oscilloscope with dBu capability

**Procedure:**
1. Connect generator to filter input through 600 Ω series resistor (to set source impedance).
2. Connect 600 Ω load resistor across filter output (terminate properly).
3. Measure V_in at filter input terminals (before series 600 Ω).
4. Measure V_out at filter output.
5. IL = 20 × log₁₀(V_out / V_in) − 6 dB (correction for source voltage divider).

Expected midband IL: 1.0–2.0 dB.  If IL > 3 dB, check inductor DCR.

---

<a name="chapter-3"></a>
## CHAPTER 3 — ACTIVE OP-AMP CW BANDPASS FILTER (AF-2)

### 3.1 Design Overview

The AF-2 active filter uses the state-variable (Kerwin-Huelsman-Newcomb) topology
implemented with two NE5532 low-noise dual op-amps.  Key performance:

- Center frequency: 400 / 500 / 600 / 700 / 800 Hz (5-position switch)
- Q range: 5 to 25 (continuously variable via front-panel potentiometer)
- Passband insertion loss: 0 dB nominal (with gain = 1 summing stage)
- Noise figure: adequate for all modern receivers (op-amp noise ≪ receiver noise floor)
- Power: ±9V from 2× PP3 alkaline batteries; ~35 hour life

### 3.2 State-Variable Filter Operation

The SVF topology is a feedback system with three integrators:

```
                         R_FB (Q control)
                    ┌────────────────────────────┐
 IN ──[ R1 ]──┬─── U1A (summing) ──────────────►│
              │                                  │
 LP ──[ R_LP ]┘                                  │
              │                                  │
 BP ──[ R_Q ]─┘                                  │
                                                 │
              U1B (integrator 1) → BP output ────┘
                                   │
              U2A (integrator 2) → LP output ─── (LP feedback to U1A)
```

The three outputs are available simultaneously:
- **HP output** at U1A output: highpass transfer function
- **BP output** at first integrator output: bandpass (the primary output)
- **LP output** at second integrator output: lowpass

The same circuit provides notch by summing HP + LP outputs.

### 3.3 Center Frequency Calculation

Both integrators must have the same time constant τ = R_INT × C_INT:
  f₀ = 1 / (2π × R_INT × C_INT) = 1 / (2π × 10kΩ × C_INT)

Required C_INT for each frequency position:

| SW Position | f₀ (Hz) | C_INT (calc) | C_INT (standard) | Error  |
|-------------|---------|--------------|------------------|--------|
| 1           | 400     | 39.8 nF      | 39 nF            | −2.0%  |
| 2           | 500     | 31.8 nF      | 33 nF            | +3.8%  |
| 3           | 600     | 26.5 nF      | 27 nF            | +1.9%  |
| 4           | 700     | 22.7 nF      | 22 nF            | −3.1%  |
| 5           | 800     | 19.9 nF      | 18 nF            | −4.5%  |

Use NP0/C0G capacitors (1% or better) for low temperature coefficient.
Polypropylene film acceptable if NP0 unavailable (TCC < 50 ppm/°C).

Actual f₀ after assembly: measure with audio oscillator; peak at BP output.
Trim C_INT if f₀ error > 5% (parallel small caps or substitute tighter values).

### 3.4 Q Control

Q = R_Q / R_FB where R_FB = 10 kΩ (fixed).

The front-panel pot (250 kΩ linear) with 50 kΩ series minimum stop provides:
  R_Q_min = 50 kΩ → Q_min = 5
  R_Q_max = 300 kΩ → Q_max = 30 (practical limit; higher Q may oscillate)

**Q calibration marks for pot dial:**
  Set Q marker positions by injecting audio oscillator at f₀ and sweeping ±BW:
  Mark the knob position where −3 dB BW equals f₀/Q for each target Q.

| Q  | BW @700 Hz | R_Q (kΩ) |
|----|-----------|----------|
|  5 | 140 Hz    |  50      |
| 10 |  70 Hz    | 100      |
| 15 |  47 Hz    | 150      |
| 20 |  35 Hz    | 200      |
| 25 |  28 Hz    | 250      |

**Stability limit:** At Q > 30, phase margin may become insufficient and the
filter can self-oscillate.  Install a 50 kΩ minimum stop resistor in series
with the pot wiper to prevent reaching Q > 30.

### 3.5 Single-Supply Operation

The NE5532 requires a split supply (± rails).  For a single 9V battery:
1. Install R_A = R_B = 100 kΩ from +9V to GND.  Junction = V_mid = 4.5V.
2. Bypass V_mid with 10 µF electrolytic (+ toward V_mid).
3. All op-amp (+) inputs bias to V_mid via 100 kΩ.
4. Input and output couple through 10 µF NP electrolytic capacitors.

This eliminates the need for a dual battery but halves the output voltage swing
(headroom = 4.5V vs 9V for split supply).

**Preferred for portable use:** Two PP3 batteries (18V total) provide ±9V split
supply, full output swing, and 35+ hours battery life.

### 3.6 CW Pitch Preference and Frequency Selection

Most operators develop a preferred CW pitch.  Standard practice:

| Operator style | Preferred pitch | SW position |
|----------------|----------------|-------------|
| Classic (ARRL) | 700–800 Hz     | 4 or 5      |
| Modern contest | 600–700 Hz     | 3 or 4      |
| DX (weak sig)  | 500–600 Hz     | 2 or 3      |
| Low-fatigue    | 400–500 Hz     | 1 or 2      |

The radio's BFO or RIT should be adjusted so that the desired station's CW signal
beats at the filter center frequency.  The filter then maximizes SNR for that
exact pitch.

---

<a name="chapter-4"></a>
## CHAPTER 4 — VARIABLE BANDWIDTH FILTER WITH SWITCHABLE Q (AF-3)

### 4.1 Design Overview

The AF-3 extends the SVF topology to a cascaded 4th-order bandpass for improved
out-of-band rejection (−80 dB/decade vs −40 dB/decade for 2nd-order).  Two
identical biquad sections are cascaded; their Q values are switched together.

Bandwidth modes:
- CW narrow: 200 Hz (Q per section ≈ 3.5 × √2 = 4.95 for Butterworth cascade)
- CW standard: 500 Hz
- SSB narrow: 1500 Hz
- SSB wide: 2700 Hz

Center frequency: switched 400–800 Hz (same 5-position switch as AF-2).

### 4.2 4th-Order Butterworth Cascade

For a maximally-flat 4th-order Butterworth bandpass, two 2nd-order sections
are cascaded.  Section Q values are:
  Q_1 = 1 / (2 sin(π/8)) = 1 / 0.7654 = 1.307 × Q_overall
  Q_2 = 1 / (2 sin(3π/8)) = 1 / 1.848 = 0.541 × Q_overall

For narrow CW (overall Q = f₀/BW = 700/200 = 3.5):
  Q_1 = 4.57;  Q_2 = 1.89

For SSB (overall Q = 700/2700 = 0.26):
  Q_1 = 0.34;  Q_2 = 0.14 (overdamped — broadband response, not resonant)

**Switched R_Q values for each BW mode (R_INT = 10kΩ, R_FB = 10kΩ):**

| BW Mode  | Target BW | Q_overall | R_Q_sect1 | R_Q_sect2 |
|----------|-----------|-----------|-----------|-----------|
| CW 200 Hz|   200 Hz  | 3.50      | 45.7 kΩ   | 18.9 kΩ   |
| CW 500 Hz|   500 Hz  | 1.40      | 18.3 kΩ   | 7.57 kΩ   |
| SSB 1500 Hz| 1500 Hz | 0.47      | 6.14 kΩ   | 2.54 kΩ   |
| SSB 2700 Hz| 2700 Hz | 0.26      | 3.40 kΩ   | 1.41 kΩ   |

Standard resistors (1% metal film, nearest values):
  CW 200: R_Q_1 = 47kΩ (−2.8%), R_Q_2 = 20kΩ (+5.8%)
  CW 500: R_Q_1 = 18kΩ (−1.6%), R_Q_2 = 7.5kΩ (−0.9%)
  SSB 1500: R_Q_1 = 6.2kΩ (+0.9%), R_Q_2 = 2.49kΩ (−2.0%)
  SSB 2700: R_Q_1 = 3.4kΩ (0.0%), R_Q_2 = 1.4kΩ (−0.7%)

Use a 4-position 2-pole rotary switch (2 poles for 2 sections, 4 throws for 4 modes).

### 4.3 Response Shapes

The 4th-order response provides significantly steeper skirts than 2nd-order:

**CW 200 Hz mode at 700 Hz center:**

| Frequency | 2nd-order attenuation | 4th-order attenuation |
|-----------|----------------------|-----------------------|
|   700 Hz  | 0 dB (ref)           | 0 dB (ref)            |
|   750 Hz  | −0.9 dB              | −1.8 dB               |
|   800 Hz  | −3.4 dB              | −6.8 dB               |
|   900 Hz  | −10 dB               | −20 dB                |
|  1000 Hz  | −15 dB               | −30 dB                |
|  1400 Hz  | −22 dB               | −44 dB                |

The 4th-order (cascaded) filter doubles the dB rejection at every offset frequency,
making it substantially more effective for adjacent-signal rejection.

---

<a name="chapter-5"></a>
## CHAPTER 5 — NOTCH FILTERS — MANUAL AND ADAPTIVE (AF-4)

### 5.1 Heterodyne Interference

When a carrier (AM broadcast station, spurious emission, or adjacent SSB signal)
falls within the passband, it produces a steady audio tone.  This "birdie" or
"heterodyne" at beat frequency f_beat can be 30–50 dB stronger than the desired
signal, making copying impossible.

A notch filter (band-reject) at f_beat removes the interferer with minimal
effect on surrounding audio.

### 5.2 Twin-T Passive Notch

The twin-T network is the simplest passive notch, requiring no power supply.
Theoretical notch depth: unlimited (infinite in ideal simulation).
Practical notch depth: 30–45 dB with 1% components (tolerance-limited).

**Design equations:**
  f_notch = 1 / (2π × R × C)
  Component values: two series R, one shunt 2C (upper T); two series C, one shunt R/2 (lower T)

**Critical matching:** All three R values and both C values must be matched for
maximum notch depth.  Measure and sort components; use pairs within 0.5% of each
other.

**Fixed 800 Hz notch:**
  C = 100 nF (PP film, measured), R = 2.0 kΩ (two 1% resistors in series to make exact value)

**Notch depth verification:**
  Drive with audio generator at 800 Hz.  At minimum output (rotate notch trim if
  variable), depth should be ≥ 30 dB.  If depth < 25 dB, one component is
  mismatched; re-measure all six components individually.

### 5.3 Active Bootstrapped Notch (Deep Null)

Adding an op-amp follower bootstrapped to the twin-T shunt network increases
notch depth from ~35 dB to ~55–65 dB.

The op-amp (U3A, NE5532) senses the null point voltage and drives it to hold
the potential at the shunt node at near-zero, which forces the null deeper.
The improvement is limited by the op-amp's open-loop gain at f_notch.

NE5532 open-loop gain at 800 Hz ≈ 80 dB → notch improvement potential ~80 dB
Practical limit (PCB parasitics, component tolerance): 55–65 dB null

### 5.4 Variable Notch (300–3500 Hz Range)

**Option A (two-gang pot):** Replace fixed R and R with dual-gang 5kΩ pot.
The R/2 element can be derived from a JFET in the ohmic region tracking the
pot output, or from a half-value tap using a two-resistor voltage divider plus
an op-amp follower driving the shunt element through a JFET.

**Option B (switched C, two-gang R):** Use a two-gang pot over two capacitor
ranges.  More practical for construction:
- Range 1 (C=100nF, R=1–5kΩ): covers 318–1592 Hz
- Range 2 (C=47nF, R=1–5kΩ): covers 677–3386 Hz
- Switch SW_RANGE selects capacitor value.

### 5.5 NE567 Tone Decoder Auto-Notch

The NE567 is a PLL that locks to a tone within ±14% of its center frequency.
Used for auto-notch, the VCO tracks the interfering carrier frequency.
The tracked VCO output, phase-shifted 180°, is summed with the input to produce
cancellation (notch).

**Limitations:**
- Lock range: ±14% of f₀ (only one frequency at a time)
- Convergence time: ~50 ms for initial lock
- Effective only for single-carrier interference

**Setting f₀ (initial center frequency):**
  f₀ = 1 / (1.1 × R_T × C_T)
  For 800 Hz: R_T = 12 kΩ, C_T = 0.1 µF → f₀ = 758 Hz ✓
  Adjust R_T trim pot until lock LED illuminates at the known heterodyne frequency.

### 5.6 LMS Adaptive Auto-Notch (DSP)

The Least Mean Squares (LMS) algorithm adapts a digital notch filter that
automatically tracks and cancels any steady narrowband interference.

**Algorithm:**
  1. Maintain a delay line of N recent samples: x[n-1], x[n-2], ..., x[n-N]
  2. Form estimate of interference: ŷ[n] = Σ w[i] × x[n-1-i]
  3. Error signal: e[n] = x[n] − ŷ[n]  (error = signal without interference)
  4. Adapt weights: w[i] += 2μ × e[n] × x[n-1-i]

The error signal e[n] is the output (desired signal with interference suppressed).

**Advantages over hardware notch:**
- Tracks multiple simultaneous tones automatically
- No manual tuning required
- Notch depth: 40–60 dB steady-state
- Tracks drifting carriers as they move in frequency

**Parameters:**
  μ (step size) = 0.005 nominal
  N (filter length) = 32 taps

**Convergence time:** typically 0.5–2 seconds from startup or after a carrier
frequency change.  Larger μ converges faster but has higher residual noise.

---

<a name="chapter-6"></a>
## CHAPTER 6 — DSP AUDIO FILTER — ESP32 IMPLEMENTATION (AF-5)

### 6.1 System Architecture

The AF-5 uses an ESP32-WROOM-32 as the signal processing core.  Audio is
captured via an INMP441 I2S MEMS microphone (repurposed as line-in ADC with
appropriate level conditioning) or the WM8978 audio codec, processed in 32-sample
blocks at 8 kHz sample rate, and output through a PCM5102A I2S stereo DAC.

**Block diagram:**

```
Radio audio out
      │
  [C_coupling]
      │
  [Level adjust (R divider + bias to 1.65V)]
      │
  [INMP441 or WM8978 ADC]
      │ I2S1 (GPIO14/15/12)
  [ESP32 — Core 1 audio task]
      │ Processing:
      │   1. HP prefilter (DC block, 38 Hz)
      │   2. Main filter (FIR, IIR, or LMS)
      │   3. AGC
      │ I2S0 (GPIO25/26/27)
  [PCM5102A DAC]
      │
  [200Ω series]
      │
  Headphones
```

### 6.2 Filter Modes

| Mode | Name           | Coefficients       | Latency | Use                  |
|------|----------------|-------------------|---------|----------------------|
| 0    | SSB 300–3kHz   | FIR_SSB (127 tap)  | ~12 ms  | SSB voice            |
| 1    | CW 700 Hz      | FIR_CW_700 (127t)  | ~12 ms  | CW at 700 Hz         |
| 2    | CW 500 Hz      | FIR_CW_500 (127t)  | ~12 ms  | CW at 500 Hz         |
| 3    | CW 400 Hz      | FIR_CW_400 (127t)  | ~12 ms  | CW at 400 Hz         |
| 4    | Notch 800 Hz   | FIR_NOTCH_800 (63) | ~8 ms   | Static notch         |
| 5    | IIR CW 700 Hz  | Biquad (2-state)   | ~4 ms   | Low-latency CW       |
| 6    | IIR Notch 800  | Biquad (2-state)   | ~4 ms   | Low-latency notch    |
| 7    | BYPASS         | None               | ~4 ms   | A/B comparison       |
| 8    | LMS Auto-Notch | Adaptive LMS       | ~4 ms   | Carrier rejection    |

**Mode selection:** Rotate encoder to cycle through modes.
**Bypass toggle:** Press encoder pushbutton (hardware relay bypasses ESP32 signal path).

### 6.3 FIR Filter Implementation

The FIR filter processes each input sample x[n] as:

  y[n] = Σᵢ h[i] × x[n-i]    for i = 0 to N-1

where h[i] is the coefficient array and x[n-i] is the delay line.

**Direct-form implementation** (see firmware `fir_apply()`):
- Circular buffer stores N most recent samples
- Each new sample: 127 multiply-accumulate operations
- At 8 kHz sample rate: 127 × 8000 = 1.016 × 10⁶ MAC/second
- ESP32 at 240 MHz: easily executes in time (>200 MACs/µs available)

**Coefficient symmetry:** FIR bandpass filters have symmetric coefficients
(h[i] = h[N-1-i]).  This can be exploited to halve the number of multiplications
using the "symmetric convolution" technique.  The firmware uses the simple
direct-form; optimize if CPU headroom becomes an issue.

### 6.4 IIR Biquad Implementation

The IIR biquad (Direct Form II transposed) processes one sample per call:

```
  y[n] = b[0]×x[n] + s[0]
  s[0] = b[1]×x[n] − a[0]×y[n] + s[1]
  s[1] = b[2]×x[n] − a[1]×y[n]
```

Only 2 state variables per section.  Much lower latency than FIR (effective
delay = 2 samples = 250 µs at 8 kHz) but non-linear phase and less selectivity.

**Use IIR mode when:**
- CW copying with real-time timing critical (contesting)
- Operator sensitive to the ~12 ms latency of 127-tap FIR

### 6.5 LMS Adaptive Notch Algorithm

The LMS adaptive notch is implemented in `lms_autonotch()`.  Key parameters:

- **Delay line length N = 32:** Longer N allows tracking more tones but uses more CPU
- **Step size μ = 0.005:** Controls convergence/stability tradeoff

**Setting μ via serial command:**
  `mu 0.003` — slower convergence, lower residual noise; use for weak signal copying
  `mu 0.010` — faster convergence; use when carrier drifts rapidly

**Stability condition:** μ < 1 / (N × P_x) where P_x is input signal power
  Approximate check: if output shows excessive noise or oscillation, reduce μ.

### 6.6 AGC (Automatic Gain Control)

The AGC adjusts audio output level to a constant target (g_agc_target = 0.5
normalized = −6 dBFS RMS).

Attack and release rates:
- Attack: gain multiplied by 0.999 per sample (slow: -0.87 dB/second)
- Release: exponential average toward target/level (medium-fast)

**Effect on CW copying:** The slow attack allows the burst of a dash or
dit to pass at normal amplitude while long carriers (from QRM) are progressively
attenuated.  This behavior is desirable and resembles a good hardware AGC.

**Disabling AGC:** Set g_agc_target = 0 and g_agc_gain = 1.0 in firmware
(add a serial command `agc off` if desired).

### 6.7 Serial Monitor Commands

Connect USB-C to computer; open terminal at 115200 baud.

| Command      | Function                                              |
|--------------|-------------------------------------------------------|
| `help`       | List all commands                                     |
| `mode N`     | Set filter mode 0–8                                   |
| `bypass on`  | Enable bypass (relay engages)                         |
| `bypass off` | Enable filter                                         |
| `mu X`       | Set LMS step size (0.001 to 0.05)                     |
| `status`     | Print current mode, AGC gain, LMS μ, sample rate      |
| `save`       | Save current settings to NVS (persist across power cycles) |

---

<a name="chapter-7"></a>
## CHAPTER 7 — ENCLOSURE AND MECHANICAL ASSEMBLY

### 7.1 Enclosure Dimensions

Outer dimensions: 160 × 100 × 55 mm
Print material: ASA (preferred; UV stable) or PETG
Print bed requirement: 200 × 200 mm minimum
Lid separation: 12 mm deep lid + 43 mm box body

Two-part print (body + lid, printed separately):
- Body: print flat on bed (no support needed); estimated 14 hours at 0.2 mm layers
- Lid: print inverted (display window faces down, no support needed for rectangular opening)

### 7.2 Front Panel Layout

Left to right across front panel at centerline:

| X (mm) | Control           | Component         |
|--------|-------------------|-------------------|
|   20   | AUDIO IN jack     | 3.5mm TRS         |
|   40   | AUDIO OUT jack    | 3.5mm TRS         |
|   68   | BYPASS SW         | DPDT toggle       |
|   86   | POWER SW          | SPST toggle       |
|  104   | STATUS LED        | 3mm green         |
|  130   | MODE ENCODER      | Rotary encoder    |
|  148   | BW ENCODER        | Rotary encoder (AF-3) |

Label each connector above the jack using self-adhesive label tape or by
printing a label panel overlay (front_label() module in OpenSCAD).

### 7.3 Rear Panel Layout

| Component        | Position      | Notes                      |
|------------------|---------------|---------------------------|
| USB-C charge port| Center        | TP4056 module input       |
| Battery door     | Left-center   | 60×(variable) mm opening  |

### 7.4 Internal Mounting

- PCB/modules: 4× M3 heat-set inserts in standoffs (cast into box body)
- Standoff height: 8 mm (clears solder points below PCB)
- Battery tray: slides into rear opening; retained by door or screw
- Wire routing: route audio wires along left wall; power wires along right wall

### 7.5 Heat-Set Insert Installation

M3 heat-set inserts (M3×4mm OD=5mm) installed with soldering iron:
1. Set iron to 230°C (450°F)
2. Place insert on top of bore (D_M3_HS = 5.5 mm bore)
3. Press gently with iron tip until flush with surface
4. Allow 60 seconds to cool before applying force

---

<a name="chapter-8"></a>
## CHAPTER 8 — ALIGNMENT AND TEST PROCEDURES

### 8.1 Required Test Equipment

- Audio oscillator or signal generator covering 100 Hz–5 kHz (TM-SG-001 recommended)
- AC voltmeter or oscilloscope with dBu measurement
- Digital multimeter (VOM) for DC checks and component measurement
- Optional: Spectrum analyzer or PC soundcard with ARTA/REW software

### 8.2 Initial Power-Up Check (Active Filters AF-2, AF-3, AF-4, AF-5)

**Before first power application:**
1. Verify supply voltage with DMM across battery terminals: AF-2/AF-3: ±9V (each battery ≥ 8.5V).  AF-5: 3.7–4.2V (Li-Ion), or 5V USB.
2. Set all potentiometers to midpoint.
3. Set mode switch to SSB (AF-3) or CW 700 Hz (AF-2).

**First power-on:**
1. Apply power; measure current: AF-2/AF-3 each rail should be 5–15 mA.  AF-5: 200–350 mA from 3.3V.
2. With no audio input, measure DC at op-amp outputs (AF-2/AF-3): should be within ±0.5V of GND (or V_mid for single supply).
3. Monitor output jack for DC voltage: should be near 0V (or V_mid through coupling cap).

**If output DC > 1V:** Check op-amp supply pins; verify coupling capacitors; check for solder bridges.

### 8.3 Frequency Response Measurement (AF-2 Active CW Filter)

1. Set frequency switch to 700 Hz position.
2. Set Q pot to midpoint (Q ≈ 12–15).
3. Connect audio generator to input; set level to −10 dBu (245 mV RMS).
4. Connect AC voltmeter to output through 10 kΩ load.
5. Set generator to 700 Hz; note output level. This is your 0 dB reference.
6. Vary frequency; record output at each point:

| f (Hz) | Expected att. (dB) | Measured (dB) | Pass? |
|--------|-------------------|---------------|-------|
|   200  |   −35 (Q=15)      |               |       |
|   400  |   −18             |               |       |
|   500  |    −9             |               |       |
|   600  |    −3             |               |       |
|   700  |     0 (ref)       |               |       |
|   800  |    −3             |               |       |
|   900  |    −9             |               |       |
|  1000  |   −16             |               |       |
|  1400  |   −30             |               |       |

Pass criterion: −3 dB points within ±10% of f₀ ± BW/2.

### 8.4 Notch Depth Measurement (AF-4)

1. Connect audio generator at 800 Hz to input.
2. Measure output voltage at generator frequency.
3. Tune notch (if variable) for minimum output.
4. Record output level.
5. Notch depth = (reference level) − (null level) in dB.

Pass criterion:
- Passive twin-T: ≥ 25 dB
- Active bootstrapped: ≥ 45 dB
- DSP FIR: ≥ 40 dB (FIR_NOTCH_800)
- LMS adaptive (steady state): ≥ 35 dB

### 8.5 LMS Auto-Notch Convergence Test (AF-5 mode 8)

1. Set AF-5 to mode 8 (AUTO NOTCH via serial or encoder).
2. Connect audio generator at 800 Hz tone mixed with speech or pink noise.
3. Observe output on oscilloscope or listen on headphones.
4. After 1–2 seconds: 800 Hz component should be strongly attenuated.
5. Change generator to 600 Hz. Verify notch shifts to track within 2 seconds.
6. With μ = 0.005 nominal: steady-state attenuation should reach −35 dB or better.

### 8.6 Bypass Test

1. Connect generator at −10 dBu, 1000 Hz.
2. Measure output in FILTER mode.
3. Toggle bypass switch (or press encoder on AF-5).
4. Measure output in BYPASS mode.
5. Difference (insertion loss of bypass path) should be ≤ 0.1 dB.

---

<a name="chapter-9"></a>
## CHAPTER 9 — FILTER RESPONSE PLOTS AND INSERTION LOSS

### 9.1 Theoretical Response Tables

**AF-1 Passive LC (6th-order composite, 300–3000 Hz):**

| f (Hz) | Expected att (dB) | Notes            |
|--------|-------------------|------------------|
|    50  |    −50            | Below HP cutoff  |
|   100  |    −27            |                  |
|   200  |    −9             |                  |
|   300  |    −3             | HP −3 dB point   |
|   500  |    −0.5           |                  |
|  1000  |    −0             | Reference        |
|  2000  |    −0.5           |                  |
|  3000  |    −3             | LP −3 dB point   |
|  4000  |    −9             |                  |
|  6000  |    −27            |                  |
|  9000  |    −50            | Above LP cutoff  |

**AF-2 Active CW SVF (700 Hz, Q=10):**

| f (Hz) | Expected att (dB) |
|--------|-------------------|
|   350  |    −20            |
|   500  |    −8.9           |
|   600  |    −4.2           |
|   630  |    −3.0 (−3dB)    |
|   700  |     0             |
|   770  |    −3.0 (−3dB)    |
|   800  |    −4.2           |
|   900  |    −8.9           |
|  1400  |    −20            |

**AF-5 FIR SSB 300–3000 Hz (127 taps, Kaiser β=6):**

| f (Hz) | Expected att (dB) |
|--------|-------------------|
|   100  |    −52            |
|   200  |    −32            |
|   300  |    −3             |
|   500  |    −0.3           |
|  1000  |     0             |
|  2000  |    −0.3           |
|  3000  |    −3             |
|  4000  |    −30            |
|  5000  |    −50            |

### 9.2 Insertion Loss Budget Summary

| Filter | Midband IL | Note                                       |
|--------|------------|--------------------------------------------|
| AF-1   | 1.5–2.0 dB | Inductor DCR loss; reduce with lower-DCR L |
| AF-2   | 0 dB       | Op-amp active; gain can be added           |
| AF-3   | 0–1 dB     | 2 cascaded stages; output buffer restores  |
| AF-4   | 0.1–0.5 dB | Passive twin-T; active bootstrapped = 0dB  |
| AF-5   | 0 dB       | DSP; gain set by AGC                       |

### 9.3 Measurement Record Sheet

**Unit serial/date:** _____________  **Operator:** _____________

| Measurement         | Spec       | Measured | Date | Pass |
|--------------------|------------|----------|------|------|
| AF-1 midband IL     | ≤ 2.5 dB   |          |      |      |
| AF-1 HP −3 dB      | 300±30 Hz  |          |      |      |
| AF-1 LP −3 dB      | 3000±100Hz |          |      |      |
| AF-2 center freq   | ±5%        |          |      |      |
| AF-2 Q=10 BW       | 70±10 Hz   |          |      |      |
| AF-4 notch depth   | ≥25 dB     |          |      |      |
| AF-5 FIR HP edge   | 300±20 Hz  |          |      |      |
| AF-5 FIR LP edge   | 3000±50 Hz |          |      |      |
| AF-5 LMS notch     | ≥35 dB ss  |          |      |      |
| Bypass IL          | ≤ 0.1 dB   |          |      |      |

---

<a name="chapter-10"></a>
## CHAPTER 10 — BATTERY OPERATION AND POWER MANAGEMENT

### 10.1 Battery Options by Filter Type

| Filter | Power required | Battery option        | Estimated life |
|--------|---------------|-----------------------|----------------|
| AF-1   | None (passive)| N/A                   | Unlimited      |
| AF-2   | ±9V @ 16 mA   | 2× PP3 alkaline       | 35 hours       |
| AF-3   | ±9V @ 24 mA   | 2× PP3 alkaline       | 23 hours       |
| AF-4   | ±9V @ 10 mA   | 2× PP3 alkaline (opt) | 55 hours       |
| AF-5   | 3.3V @ 365 mA | 18650 Li-Ion 2500 mAh | 6.6 hours      |

### 10.2 Battery Monitoring

**AF-2 / AF-3 / AF-4 (PP3):**
Monitor battery voltage with DMM at battery terminal.
- PP3 fresh: 9.6V
- PP3 nominal: 9.0V
- Replace when: < 7.5V (filter frequency will drift below 7V)

**AF-5 (18650 Li-Ion):**
ESP32 firmware reads V_bat via GPIO35 divider (100kΩ / 100kΩ).
Display shows battery level bar.
- Cutoff voltage: 3.0V (TP4056 or BMS protection circuit)
- Replace/recharge when: display shows LOW (< 3.3V)

### 10.3 Low-Power Optimization (AF-5)

To extend Li-Ion battery life on the AF-5:

1. Reduce ESP32 CPU frequency: `setCpuFrequencyMhz(80)` (reduces power ~40%)
   Note: confirm 8 kHz audio processing remains stable at 80 MHz before field use.

2. Disable Bluetooth and WiFi (not used by default; confirm off):
   `btStop(); WiFi.disconnect(true); WiFi.mode(WIFI_OFF);`

3. Turn off OLED when inactive (display off after 60 seconds):
   `oled.ssd1306_command(SSD1306_DISPLAYOFF);`
   Saves ~5 mA.

4. Use sleep mode between audio blocks if PCM5102A supports hardware-triggered wake:
   ESP32 light sleep between I2S DMA interrupts (~20 µs sleep per 32-sample block).

With all optimizations: estimated battery life extends from 6.6 to ~9–10 hours.

### 10.4 USB-C Charging (AF-5)

The TP4056 charger module accepts 5V USB-C input.
Charge rate: set by PROG resistor (1.2 kΩ = 1A; 2.4 kΩ = 500 mA).
Charge LED: RED = charging; BLUE/GREEN = charge complete.
Full charge time (18650 2500 mAh at 1A): ~2.5 hours.

**Do not charge during high-current RF operation** (signal generator running with
PA stages active).  RF current spikes can interfere with charger IC.  Charge
separately or use a USB power bank for dedicated clean 5V supply.

---

<a name="chapter-11"></a>
## CHAPTER 11 — TROUBLESHOOTING

### 11.1 AF-1 Passive LC — No Output

| Symptom            | Likely Cause                   | Remedy                          |
|--------------------|--------------------------------|---------------------------------|
| No audio out       | Open inductor winding          | Measure DCR; >5× expected = open|
| Weak output        | Source impedance mismatch      | Add input transformer           |
| Audio, no filtering| Shorted capacitor              | Measure C with LCR meter        |
| Frequency shifted  | Wrong capacitor value installed| Verify markings; measure with LCR|

### 11.2 AF-2/AF-3 Active — No Output

| Symptom              | Likely Cause                  | Remedy                        |
|----------------------|-------------------------------|-------------------------------|
| No output            | Dead battery                  | Measure supply voltage        |
| Constant DC at output| Op-amp oscillation            | Add bypass caps to supply     |
| Very weak output     | Missing V_mid bias (S/S mode) | Check R_A/R_B divider         |
| Wrong center freq    | Wrong C_INT installed          | Verify against frequency table|
| No Q (flat response) | R_Q disconnected or open pot  | Measure pot end-to-end        |
| Oscillation          | Q too high (pot at max)       | Install 50kΩ series min stop  |

### 11.3 AF-4 Notch — Insufficient Depth

| Symptom               | Likely Cause                   | Remedy                       |
|-----------------------|--------------------------------|------------------------------|
| Notch < 20 dB         | Component mismatch > 2%        | Sort and match R and C pairs |
| Notch at wrong freq   | Miscalculated R value          | Recalculate; use LCR meter   |
| No null at any setting| Open notch path                | Check both T networks        |
| NE567 won't lock      | f₀ too far from interference   | Adjust R_T trim pot          |

### 11.4 AF-5 DSP — No Audio

| Symptom              | Likely Cause                   | Remedy                        |
|----------------------|--------------------------------|-------------------------------|
| No sound from DAC    | I2S0 wiring error              | Verify GPIO25/26/27 continuity|
| Distorted output     | Input level too high (clipping) | Reduce input level divider   |
| High noise floor     | ADC floating (no input bias)   | Verify 1.65V bias at GPIO34  |
| Firmware won't start | Boot issues with GPIO0/12      | Release GPIO12 low at boot   |
| LMS won't converge   | μ too small for signal level   | Increase mu to 0.01–0.02     |
| Wrong filter         | Mode number out of range       | `mode 0` via serial; check enum|

---

<a name="appendix-a"></a>
## APPENDIX A — COMPONENT REFERENCE AND SUBSTITUTION GUIDE

### A.1 Op-Amps

| Specified | Substitute   | Notes                                    |
|-----------|-------------|------------------------------------------|
| NE5532    | NE5534 (×1) | Single; use DIP-8 with one channel tied  |
| NE5532    | OPA2134     | Premium; 10 pA bias, lower noise         |
| NE5532    | TL072       | Acceptable; higher noise, lower supply   |
| NE5532    | LM358       | NOT recommended; slew rate too low       |

### A.2 Capacitors

| Type     | Application                          | Substitution                          |
|----------|--------------------------------------|---------------------------------------|
| PP film  | LC filter C, SVF C_INT               | Polystyrene (PS) for audio frequency  |
| NP0/C0G  | SVF C_INT 1% or better               | Polypropylene if NP0 unavailable      |
| Electrolytic NP | Coupling capacitors          | Bipolar electrolytic or film ≥ 10µF  |
| Electrolytic   | Supply bypass                 | Standard electrolytic (+ toward +V)  |

### A.3 Inductors

| Specified  | Notes                                          |
|------------|------------------------------------------------|
| 150 mH     | Wind on T-94A-77 ×2 stacked; or Bourns SRR1260-151Y |
| 33 mH      | Wind on T-94A-77 ×1; or Bourns SRR1260-330Y   |

Self-resonant frequency (SRF) of audio chokes must be > 20 kHz.
Measure SRF: apply AC; find frequency where impedance is maximum (parallel resonance).

---

<a name="appendix-b"></a>
## APPENDIX B — FILTER DESIGN EQUATIONS AND TABLES

### B.1 Butterworth Normalized Prototype Elements (Ladder)

| n | g1     | g2     | g3     | g4     | g5     |
|---|--------|--------|--------|--------|--------|
| 1 | 2.0000 |        |        |        |        |
| 2 | 1.4142 | 1.4142 |        |        |        |
| 3 | 1.0000 | 2.0000 | 1.0000 |        |        |
| 4 | 0.7654 | 1.8478 | 1.8478 | 0.7654 |        |
| 5 | 0.6180 | 1.6180 | 2.0000 | 1.6180 | 0.6180 |

Denormalization: L = g × Z₀ / ωc;  C = g / (Z₀ × ωc)  for lowpass
                 C = 1/(g × Z₀ × ωc); L = Z₀ / (g × ωc) for highpass

### B.2 SVF Component Calculation

Given: f₀ (Hz), Q, R_INT (Ω)

  C_INT = 1 / (2π × f₀ × R_INT)
  R_Q   = Q × R_FB  (R_FB = R_INT nominally)
  BW    = f₀ / Q

### B.3 Twin-T Notch Frequency Table

C = 100 nF, vary R for different notch frequencies:

| f_notch (Hz) | R (kΩ)  | R/2 (kΩ) |
|-------------|---------|----------|
| 300         | 5.305   | 2.653    |
| 400         | 3.979   | 1.990    |
| 500         | 3.183   | 1.592    |
| 600         | 2.653   | 1.326    |
| 700         | 2.274   | 1.137    |
| 800         | 1.989   | 0.995    |
| 1000        | 1.592   | 0.796    |
| 1500        | 1.061   | 0.531    |
| 2000        | 0.796   | 0.398    |
| 3000        | 0.531   | 0.265    |

---

<a name="appendix-c"></a>
## APPENDIX C — FIR COEFFICIENT GENERATION (Python/scipy)

```python
#!/usr/bin/env python3
"""
FIR coefficient generator for TM-AF-001 audio filters.
Requires: numpy, scipy
Usage: python3 gen_fir.py
"""

import numpy as np
from scipy.signal import firwin, freqz
import matplotlib.pyplot as plt

Fs = 8000       # sample rate Hz
N  = 127        # number of taps (odd for Type I linear phase)

def gen_bandpass(f_lo, f_hi, N, beta, label):
    nyq = Fs / 2
    h = firwin(N, [f_lo/nyq, f_hi/nyq], pass_zero=False,
               window=('kaiser', beta))
    print_coeffs(h, label)
    return h

def gen_notch(f_lo, f_hi, N, beta, label):
    nyq = Fs / 2
    h = firwin(N, [f_lo/nyq, f_hi/nyq], pass_zero=True,
               window=('kaiser', beta))
    print_coeffs(h, label)
    return h

def print_coeffs(h, name):
    print(f"\n// {name}")
    print(f"const float {name}[{len(h)}] PROGMEM = {{")
    for i, c in enumerate(h):
        end = ',\n' if (i+1) % 6 == 0 and i < len(h)-1 else ', '
        print(f"    {c:.6f}f", end=end)
    print("\n};")

def plot_response(filters, labels):
    fig, ax = plt.subplots(figsize=(12, 6))
    freqs = np.linspace(0, Fs/2, 4096)
    for h, lbl in zip(filters, labels):
        w, H = freqz(h, worN=8192, fs=Fs)
        dB = 20 * np.log10(np.abs(H) + 1e-10)
        ax.plot(w, dB, label=lbl)
    ax.set_xlim(0, 4000)
    ax.set_ylim(-80, 5)
    ax.set_xlabel("Frequency (Hz)")
    ax.set_ylabel("Attenuation (dB)")
    ax.set_title("TM-AF-001 FIR Filter Responses")
    ax.legend()
    ax.grid(True)
    plt.tight_layout()
    plt.savefig("filter_responses.png", dpi=150)
    plt.show()

if __name__ == "__main__":
    filters = []
    labels  = []

    # SSB bandpass 300-3000 Hz
    h = gen_bandpass(300, 3000, 127, 6, "FIR_SSB")
    filters.append(h); labels.append("SSB 300-3kHz")

    # CW bandpass 450-950 Hz (center 700 Hz)
    h = gen_bandpass(450, 950, 127, 8, "FIR_CW_700")
    filters.append(h); labels.append("CW 700Hz")

    # CW bandpass 350-650 Hz (center 500 Hz)
    h = gen_bandpass(350, 650, 127, 8, "FIR_CW_500")
    filters.append(h); labels.append("CW 500Hz")

    # CW bandpass 300-500 Hz (center 400 Hz)
    h = gen_bandpass(300, 500, 127, 8, "FIR_CW_400")
    filters.append(h); labels.append("CW 400Hz")

    # Notch 750-850 Hz
    h = gen_notch(750, 850, 63, 10, "FIR_NOTCH_800")
    filters.append(h); labels.append("Notch 800Hz")

    plot_response(filters, labels)
    print("\n// All coefficients generated successfully.")
    print("// Copy into fir_coefficients.h")
```

Run this script to regenerate exact coefficients and verify the plot matches
the expected response table in Chapter 9.  The coefficients in `fir_coefficients.h`
were computed with this script; reproduction should yield identical values within
float32 rounding.

---

<a name="appendix-d"></a>
## APPENDIX D — PARTS LISTS MASTER INDEX

All detailed parts lists are in the schematic files:

| File                                | Content                        |
|-------------------------------------|-------------------------------|
| schematics/sch_lc_ssb_bandpass.txt  | AF-1 passive LC parts list    |
| schematics/sch_active_cw_filter.txt | AF-2 active CW SVF parts list |
| schematics/sch_variable_bw_filter.txt | AF-3 variable BW parts list |
| schematics/sch_notch_filter.txt     | AF-4 notch filter parts list  |
| schematics/sch_dsp_power.txt        | AF-5 DSP board and power      |

**Common items across all units:**

| Item             | Value / Part      | Qty total | Notes                    |
|------------------|-------------------|-----------|--------------------------|
| 3.5mm TRS jacks  | Panel mount stereo| 8 (2/unit)| Switchcraft 35RAPC3BV4   |
| DPDT toggle SW   | Panel mount 6.4mm | 4 (1/unit)| NKK M2012 or equiv.      |
| SPST toggle SW   | Panel mount 6.4mm | 4 (1/unit)| Power switch             |
| PP3 connectors   | I-clip snap       | 8+        | For battery leads        |
| 3mm LED, green   | T-1 package       | 4+        | Power indicators         |
| Heat-set inserts | M3×4mm            | 16 (4/enc)| McMaster 94180A333 equiv |
| M3×8mm screws    | Pan head, SS      | 16        | Lid attachment           |

---

*End of Technical Manual TM-AF-001 Revision A*

*This document is maintained in:*
`/home/merv/Public/claude/ant_gear/audio_filters/MANUAL_AUDIO_FILTER.md`

*Related documents:*
- TM-SG-001 — VFO/Signal Generator (for filter alignment)
- TM-CR-001 — Crystal Radio (passive audio detection)
