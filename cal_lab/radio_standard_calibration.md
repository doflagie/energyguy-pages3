# Time Standard Radio Broadcast Calibration Method

## Overview

This method uses **free, over-the-air time standard broadcasts** (WWV, CHU, MSF, DCF77, etc.) to calibrate the TinySA's 30MHz reference with **0.1-1 ppm accuracy**.

### Why Radio Time Standards Work

- Transmitted from atomic clock-controlled transmitters
- Frequencies are **exact** within ±1×10⁻¹² (0.00001 ppm)
- Free, continuous broadcasts
- No special equipment needed (just a receiver)
- **Worldwide coverage** from multiple stations

### What You'll Achieve

```
Frequency Error: < 3 Hz @ 30MHz (0.1 ppm)
Accuracy: Limited by propagation and receiver quality
Cost: $0-20 (if you have receiver/SDR)
Time Required: 30-60 minutes
```

---

## Available Time Standard Broadcasts

### Worldwide Time Standard Stations

| Station | Country | Frequencies | Power | Coverage |
|---------|---------|-------------|-------|----------|
| **WWV** | USA (Colorado) | 2.5, 5, 10, 15, 20, 25 MHz | 2.5-10 kW | North America, Pacific |
| **WWVH** | USA (Hawaii) | 2.5, 5, 10, 15 MHz | 5-10 kW | Pacific, Asia |
| **CHU** | Canada | 3.330, 7.850, 14.670 MHz | 3-10 kW | North America |
| **MSF** | UK | 60 kHz | 60 kW | Europe |
| **DCF77** | Germany | 77.5 kHz | 50 kW | Europe |
| **JJY** | Japan | 40/60 kHz | 50 kW | Japan, East Asia |
| **BPC** | China | 68.5 kHz | 90 kW | China, Asia |
| **RBU** | Russia | 66.66 kHz | 10 kW | Russia |

**Note:** WWV at 10 MHz was discontinued in 2023 to save costs. Only 2.5, 5, 15, 20, and 25 MHz remain operational.

### Best Stations for Calibration

**North America:**
- **CHU** (Canada) - 7.850 MHz or 14.670 MHz (best)
- **WWV** - 5 MHz or 15 MHz
- **WWVH** - 5 MHz or 15 MHz

**Europe:**
- **DCF77** - 77.5 kHz (strongest in Europe)
- **MSF** - 60 kHz (UK and Western Europe)

**Asia:**
- **JJY** - 40 or 60 kHz
- **BPC** - 68.5 kHz

**Worldwide:**
- Any station you can receive clearly
- HF stations (3-20 MHz) work best for long distance
- LF stations (60-77 kHz) work best locally

---

## Required Materials

### Minimum Setup (Free if you have equipment)

| Item | Cost | Notes |
|------|------|-------|
| Shortwave receiver | $0-50 | Or any HF-capable radio |
| OR RTL-SDR dongle | $25-35 | USB SDR receiver |
| Wire antenna | $0-5 | Random wire, 20-50 feet |
| TinySA | $100-150 | Owner supplied |
| 3.5mm audio cable | $2 | Receiver to TinySA |

### Recommended Setup

| Item | Cost | Purpose |
|------|------|---------|
| RTL-SDR v3 | $30 | Clean, stable reception |
| Upconverter (for LF) | $20 | Receive 60-77 kHz stations |
| Antenna wire | $5 | 50-100 ft wire |
| Audio interface | $10 | Clean signal connection |

### Tools

- Computer (for SDR software)
- Basic audio editing software (Audacity, free)
- Calculator or spreadsheet

---

## Theory of Operation

### How Time Standard Broadcasts Work

1. **Master Clock:** Atomic clock at transmitter site (cesium or hydrogen maser)
2. **Frequency Synthesis:** Atomic clock generates exact carrier frequency
3. **Transmission:** High-power transmitter broadcasts on precise frequency
4. **Reception:** You receive the signal and use it as frequency reference

### Carrier Frequency as Reference

The **carrier frequency itself** is the atomic clock reference:
- WWV 5 MHz = exactly 5,000,000.00 Hz
- CHU 7.850 MHz = exactly 7,850,000.00 Hz
- DCF77 77.5 kHz = exactly 77,500.00 Hz

### Zero-Beat Method

**Concept:**
1. Use TinySA to generate local reference frequency
2. Tune to match broadcast carrier
3. Listen for zero-beat (no audible tone = perfect match)
4. Measure frequency difference

---

## Calibration Methods

### Method 1: Direct Frequency Measurement (Simplest)

Use TinySA as spectrum analyzer to measure broadcast carrier.

#### Procedure

1. **Tune TinySA to time standard:**
   - WWV: 5 MHz or 15 MHz
   - CHU: 7.850 MHz or 14.670 MHz
   - DCF77: 77.5 kHz

2. **Connect antenna to TinySA**

3. **Find carrier peak** on spectrum display

4. **Read frequency** using TinySA's marker

5. **Compare to known frequency:**
   ```
   Known CHU: 7,850,000.00 Hz
   TinySA reads: 7,850,050 Hz
   Error: +50 Hz = +6.4 ppm
   ```

6. **Calculate correction:**
   ```
   ppm_error = (measured - actual) / actual × 10^6
   ppm_error = (7,850,050 - 7,850,000) / 7,850,000 × 10^6
   ppm_error = +6.37 ppm
   ```

7. **Apply to 30MHz reference:**
   ```
   If TinySA is 6.37 ppm fast at 7.850 MHz,
   it's also 6.37 ppm fast at 30 MHz

   30 MHz error = 30,000,000 × 6.37/10^6 = 191 Hz
   Actual frequency = 30,000,191 Hz
   ```

8. **Enter correction** in TinySA config: **-6.37 ppm**

#### Advantages
- Simplest method
- Direct measurement
- No additional hardware

#### Disadvantages
- Limited by TinySA's uncalibrated reference (circular problem)
- Need good signal strength
- Propagation effects can cause error

---

### Method 2: Receiver + TinySA Beat Frequency (More Accurate)

Use a receiver to heterodyne against TinySA's tracking generator.

#### Setup

```
Antenna → Receiver → Audio Out → Computer/Oscilloscope
                ↑
            TinySA → Signal Generator Mode
```

#### Procedure

1. **Configure TinySA as signal generator:**
   - Set frequency to CHU: 7.850 MHz
   - Set output power: -10 dBm
   - Connect to receiver antenna input (via attenuator or loose coupling)

2. **Tune receiver to CHU 7.850 MHz**

3. **Adjust TinySA frequency** until you hear zero-beat:
   - High tone: TinySA frequency too high
   - Low tone: TinySA frequency too low
   - Silence (zero-beat): Perfect match

4. **Fine-tune for null:**
   - Adjust in 1 Hz steps
   - Listen in SSB or CW mode
   - Find quietest point

5. **Read TinySA frequency** when zero-beat achieved

6. **Calculate error:**
   ```
   CHU actual: 7,850,000 Hz
   TinySA setting at zero-beat: 7,850,045 Hz
   Error: +45 Hz = +5.73 ppm
   ```

7. **Apply correction** to 30 MHz reference

#### Advantages
- More accurate than direct measurement
- Less affected by propagation
- Works with weak signals

#### Disadvantages
- Requires receiver
- More setup complexity
- Skill needed to find zero-beat

---

### Method 3: Audio Tone Method (Most Accurate)

Use WWV/CHU's audio tones as secondary reference.

#### Background

WWV and CHU broadcast:
- **1000 Hz audio tone** (except top of minute)
- **500 Hz tone** (first hour tone, WWV only)
- Tone frequency accurate to ±0.001 Hz

#### Procedure

1. **Record 1000 Hz tone** from WWV or CHU:
   - Use SDR software
   - Or connect receiver audio to computer
   - Record 30-60 seconds of clean tone

2. **Analyze in audio software:**
   - Use Audacity: Analyze → Plot Spectrum
   - Find peak frequency
   - Should be exactly 1000.000 Hz

3. **Measure actual frequency:**
   - If you read 1000.5 Hz, your soundcard is 0.5 Hz fast
   - This doesn't help us directly (soundcard not connected to TinySA)

**Alternative: Tone Beat Method**

1. **Generate 1000 Hz with TinySA** (if capable)
2. **Mix with WWV 1000 Hz tone**
3. **Listen for beat frequency**
4. **Adjust TinySA tone generator** for zero-beat
5. **Calculate error**

**Limitation:** This calibrates the tone generator, not the 30 MHz reference. Only useful if tone generator uses same reference.

---

### Method 4: Harmonic Multiplication (Advanced)

Use harmonics of lower frequency to reach 30 MHz.

#### Concept

If you can accurately measure a lower frequency, multiply to 30 MHz:

```
CHU 7.850 MHz × 4 = 31.40 MHz (close to 30 MHz)
WWV 5 MHz × 6 = 30 MHz (perfect!)
```

#### Procedure (WWV 5 MHz Example)

**Problem:** WWV 10 MHz was discontinued, and 5 MHz × 6 = 30 MHz.

1. **Receive WWV 5 MHz**

2. **Generate harmonics:**
   - Feed into frequency multiplier (×6)
   - Or: Use non-linear device (diode, saturated amplifier)
   - Output contains 5, 10, 15, 20, 25, **30 MHz** harmonics

3. **Filter for 30 MHz:**
   - Use bandpass filter
   - Or just select with TinySA spectrum analyzer

4. **Compare 30 MHz harmonic** to TinySA's 30 MHz reference

5. **Measure beat frequency** or phase

#### Circuit for Harmonic Generation

```
WWV 5 MHz → 1N4148 diode → 30 MHz bandpass filter → TinySA input
                ↓
              Ground via resistor

Diode generates harmonics due to non-linearity
Filter passes only 6th harmonic (30 MHz)
```

#### Advantages
- Direct comparison at 30 MHz
- High accuracy possible
- Avoids frequency conversion math

#### Disadvantages
- Requires building hardware
- Needs signal processing
- Harmonic signal is weak

---

## Station Selection Guide

### Which Station Should You Use?

**North America:**

| Location | Best Station | Frequency | When |
|----------|--------------|-----------|------|
| Western USA/Canada | WWV | 5, 15 MHz | Daytime |
| Eastern USA/Canada | CHU | 7.850 MHz | Anytime |
| East Coast | WWV | 5 MHz | Night |
| Pacific | WWVH | 5, 15 MHz | Daytime |

**Europe:**
- **DCF77** 77.5 kHz (strongest, day/night)
- **MSF** 60 kHz (UK, Western Europe)

**Asia:**
- **JJY** 40 or 60 kHz
- **BPC** 68.5 kHz

### Propagation Considerations

**HF Stations (3-20 MHz):**

| Frequency | Daytime | Nighttime | Range |
|-----------|---------|-----------|-------|
| 2.5 MHz | Poor | Good | 0-1000 mi |
| 5 MHz | Good | Excellent | 0-2000 mi |
| 7.85 MHz (CHU) | Excellent | Good | 0-3000 mi |
| 10-15 MHz | Excellent | Fair | 0-5000 mi |
| 20-25 MHz | Fair | Poor | 0-3000 mi |

**LF Stations (60-77 kHz):**
- **Day and night:** Consistent
- **Range:** 500-1000 miles (ground wave)
- **Advantage:** Stable, minimal fading
- **Disadvantage:** Need upconverter for RTL-SDR

### Signal Quality Check

**Good signal:**
- S-meter reads S7 or higher
- Minimal fading
- Clean carrier, no distortion
- Stable over 5+ minutes

**Poor signal (don't use):**
- Weak (< S5)
- Rapid fading
- Noise/static
- Interference from other stations

---

## Propagation Error Correction

### Ionospheric Delay

Radio waves traveling through ionosphere are delayed:

**Effect on frequency:**
- Doppler shift from moving ionosphere: ±0.1 Hz typical
- Multipath causes fading, not frequency shift
- Average over 5-10 minutes to eliminate

**Correction:**
- Take multiple measurements (10+)
- Average results
- Discard outliers
- Prefer stable, strong signals

### Multipath Interference

Multiple paths cause fading:

**Symptoms:**
- Signal strength varies
- Apparent frequency wobble
- Beat notes in audio

**Solutions:**
- Use directional antenna
- Wait for stable propagation
- Choose higher frequency (less multipath)
- Measure during quiet ionosphere (noon, summer)

---

## Step-by-Step Calibration Example (CHU Method)

### Equipment

- RTL-SDR dongle
- 50-foot wire antenna
- Computer running SDR# or GQRX
- TinySA

### Procedure

**1. Setup Receiver (15 minutes)**

- Connect RTL-SDR to computer
- Connect antenna to RTL-SDR
- Launch SDR software
- Set frequency: 7.850 MHz
- Set mode: AM or USB
- Adjust RF gain for strong but not overloaded signal

**2. Find CHU Signal (5 minutes)**

- Look for carrier on waterfall
- Should see time code modulation (digital pulses)
- Listen for voice announcements (top of hour)
- Verify it's CHU (not another station)

**3. Measure Carrier Frequency (10 minutes)**

- Switch to CW or narrow filter mode
- Center on carrier
- Use frequency counter in SDR software
- Record frequency: ___________

**4. Calculate Error**

```
Known CHU frequency: 7,850,000.00 Hz
Measured frequency: 7,850,XXX.XX Hz  (fill in your reading)

Error (Hz) = Measured - Known
Error (ppm) = (Error / Known) × 10^6

Example:
Measured: 7,850,062 Hz
Error = 7,850,062 - 7,850,000 = +62 Hz
Error (ppm) = 62 / 7,850,000 × 10^6 = +7.90 ppm
```

**5. Apply to 30 MHz**

```
30 MHz error = 30,000,000 × (ppm_error / 10^6)

Example:
30 MHz error = 30,000,000 × 7.90 / 10^6 = 237 Hz
Actual 30 MHz = 30,000,237 Hz
```

**6. Enter Correction in TinySA**

- CONFIG → XTAL/Reference
- Enter: **-7.90 ppm** (opposite sign)
- SAVE

**7. Verification (10 minutes)**

- Re-measure CHU frequency
- Should now read 7,850,000 Hz (within ±10 Hz)
- If not, iterate

---

## Accuracy Limitations

### Factors Affecting Accuracy

| Factor | Typical Error | Mitigation |
|--------|---------------|------------|
| Transmitter accuracy | ±0.00001 ppm | None needed (perfect) |
| Ionospheric Doppler | ±0.1 Hz | Average multiple readings |
| Receiver stability | ±1 ppm | Use GPS-locked SDR |
| Multipath fading | ±1 Hz | Use strong, stable signal |
| Measurement resolution | ±1 Hz | Use FFT or counter |

**Achievable Accuracy:**
- Single measurement: ±1-10 ppm
- Averaged (10 readings): ±0.5-2 ppm
- Ideal conditions: ±0.1-0.5 ppm

---

## Troubleshooting

### Can't Receive Station

**No signal at all:**
- Check antenna connection
- Verify SDR is working (try FM broadcast)
- Try different time of day
- Try different frequency/station

**Weak signal:**
- Improve antenna (longer wire, outdoors)
- Try different frequency
- Wait for better propagation
- Check for local interference

### Frequency Unstable

**Reading jumps around:**
- Increase averaging time
- Use narrower filter
- Wait for stable conditions
- Improve antenna

**Slow drift:**
- Normal (ionosphere moving)
- Average over longer time
- Take multiple measurements

### SDR Frequency Offset

**SDR itself uncalibrated:**
- This is the problem we're trying to solve!
- Use "PPM correction" in SDR software
- Measure error against CHU
- Enter correction
- Iterate

---

## Alternative: Using RTL-SDR's Built-in Calibration

Many SDR programs allow you to calibrate the RTL-SDR against known frequency:

**SDR# Procedure:**

1. Tune to CHU 7.850 MHz
2. Note frequency error (e.g., reads 7.850.045 MHz)
3. Click Configure (gear icon)
4. Adjust "Frequency correction (ppm)" slider
5. Adjust until it reads exactly 7.850.000 MHz
6. Record ppm value
7. This ppm applies to TinySA's reference (if using same oscillator)

**Limitation:** This calibrates the SDR, not the TinySA. They're separate devices with separate oscillators.

---

## Advanced: Building a WWV-Locked 30MHz Reference

If you want **continuous** calibration:

### Concept

Build a PLL (Phase-Locked Loop) that:
1. Receives WWV 5 MHz
2. Multiplies by 6 to get 30 MHz
3. Locks crystal oscillator to result
4. Provides clean 30 MHz locked to WWV atomic clock

### Block Diagram

```
Antenna → Receiver → 5 MHz IF → PLL (×6) → 30 MHz output
                                   ↑
                              Local TCXO ←→ Feedback
```

### Components Needed

- WWV receiver or downconverter
- PLL IC (e.g., ADF4002, ADF4351)
- TCXO (30 MHz or 10 MHz)
- Loop filter components
- Power supply

**Complexity:** High (PLL design is advanced)
**Cost:** $50-100
**Result:** Continuous 30 MHz locked to WWV

---

## Comparison to GPS Method

| Aspect | GPS | WWV/CHU | Winner |
|--------|-----|---------|--------|
| **Accuracy** | 0.01 ppm | 0.1-1 ppm | GPS |
| **Cost** | $15-25 | $0-30 | WWV (if you have SDR) |
| **Setup time** | 2-4 hours | 30-60 min | WWV |
| **Availability** | Worldwide | Regional | GPS |
| **Learning value** | GPS tech | Radio propagation | Tie |
| **Cool factor** | Modern | Classic | WWV (nostalgia!) |

**Recommendation:** Use **GPS for best accuracy**, WWV/CHU for **quick check** or **learning experience**.

---

## Summary

### What We Accomplished

✓ Used free atomic clock broadcasts for calibration
✓ Achieved 0.1-1 ppm accuracy with no cost
✓ Learned about HF propagation and time standards
✓ Verified TinySA reference against multiple sources

### Key Takeaways

1. **Time standard broadcasts** are free atomic clock references
2. **Carrier frequency** is exact, derived from atomic clock
3. **Propagation effects** limit accuracy to ~0.1-1 ppm
4. **Averaging** multiple measurements improves accuracy
5. **CHU at 7.850 MHz** is best for North America
6. **DCF77 at 77.5 kHz** is best for Europe

### When to Use This Method

- You already have SDR or shortwave receiver
- You want to verify GPS calibration
- You enjoy learning about radio propagation
- Cost is more important than ultimate accuracy
- You live near a time standard transmitter

---

## References

- [NIST WWV/WWVH](https://www.nist.gov/pml/time-and-frequency-division/radio-stations/wwv)
- [CHU Canada](https://nrc.canada.ca/en/certifications-evaluations-standards/canadas-official-time/time-signal)
- [DCF77 Germany](https://www.ptb.de/cms/en/ptb/fachabteilungen/abt4/fb-44/ag-442/dissemination-of-legal-time/dcf77.html)
- [Time Station List](http://www.qrz.com/time.html)

**Happy calibration using 100-year-old technology!**
