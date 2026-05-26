# NanoVNA Calibration - Complete Guide

## Overview

The NanoVNA requires **two types of calibration**:

1. **Frequency Calibration** - Timebase accuracy (similar to TinySA)
2. **Port Calibration** - SOL/SOLT for impedance measurements

This guide covers **cheap, homebrew methods** to calibrate your NanoVNA **without any calibrated test equipment**.

---

## Two-Part Calibration System

### Part 1: Frequency Calibration (Timebase)

**Problem:** NanoVNA's crystal oscillator may be off frequency

**Effect:**
- Frequency display incorrect
- Measurements at wrong frequencies
- Resonances appear shifted

**Solution:** Same as TinySA calibration
- Use GPS 1PPS, WWV/CHU, or FM broadcasts
- Measure crystal error
- Apply PPM correction
- **See:** [gps_calibration.md](gps_calibration.md) for methods

**Accuracy Achievable:** ±0.01 ppm (GPS method)

---

### Part 2: Port Calibration (Impedance)

**Problem:** NanoVNA needs reference standards to measure impedance accurately

**Standards Needed:**
- **SHORT** - 0Ω, direct connection
- **OPEN** - infinite Ω, nothing connected
- **LOAD** - exactly 50Ω non-inductive
- **THRU** - direct connection between ports (optional)

**Effect of Poor Calibration:**
- SWR readings incorrect
- Impedance measurements wrong
- Smith chart distorted
- Filter responses skewed

**Solution:** Build or verify calibration standards
- Homebrew 50Ω loads
- Verify SHORT quality
- Characterize OPEN capacitance
- Build THRU adapters

**Accuracy Achievable:** ±5% (good), ±1% (excellent)

---

## Quick Start Summary

### For Most Users (Recommended Path)

**Step 1: Frequency Calibration (1-2 hours)**
1. Use GPS or FM broadcast method
2. Apply PPM correction to NanoVNA
3. Verify with multiple sources

**Step 2: Port Calibration (30 minutes)**
1. Use supplied calibration kit (if you have one)
2. OR build homebrew standards
3. Perform SOL or SOLT calibration
4. Verify with known components

**Total Time:** 2-3 hours
**Total Cost:** $15-30 (if building everything)

---

### When You Need Calibration

**Frequency Calibration:**
- After purchasing NanoVNA (check error)
- Every 6-12 months (crystal aging)
- After firmware update
- Before precision measurements

**Port Calibration:**
- **Before every measurement session**
- After changing cables
- After changing frequency range
- When measurements look wrong

**Key Difference:**
- Frequency cal: Occasional (like tuning a guitar)
- Port cal: Every time (like zeroing a scale)

---

## Understanding NanoVNA Calibration Types

### SOLT Calibration (Full 2-Port)

**Standards:**
- **S**hort
- **O**pen
- **L**oad (50Ω)
- **T**hru

**Measures:** S11, S21, S12, S22
**Use For:** Filters, amplifiers, transmission lines
**Time:** 2-3 minutes

### SOL Calibration (1-Port)

**Standards:**
- **S**hort
- **O**pen
- **L**oad (50Ω)

**Measures:** S11 only
**Use For:** Antennas, impedance measurements, SWR
**Time:** 1-2 minutes

### Through-Only Calibration

**Standard:**
- **T**hru only

**Measures:** S21 transmission
**Use For:** Quick filter checks, cable loss
**Time:** 30 seconds

---

## NanoVNA Models and Differences

### NanoVNA Original (v1)

- Frequency: 50 kHz - 900 MHz
- Crystal: 8 MHz (needs calibration)
- Ports: CH0 and CH1
- Calibration storage: 5 slots
- **Typical error:** ±20-50 ppm uncalibrated

### NanoVNA-H (v2)

- Frequency: 50 kHz - 1.5 GHz
- Crystal: Better stability
- Same calibration procedure
- **Typical error:** ±10-20 ppm uncalibrated

### NanoVNA-H4 (v2plus4)

- Frequency: 50 kHz - 4.4 GHz
- Crystal: TCXO (better)
- Higher frequency needs better standards
- **Typical error:** ±2-5 ppm uncalibrated

### NanoVNA-F (v2 variant)

- Similar to H4
- Different firmware
- Same calibration methods

---

## Calibration Files Structure

This guide is organized as:

### Core Documents

1. **nanovna_calibration_overview.md** (this file) - Overview
2. **nanovna_frequency_cal.md** - Timebase calibration
3. **nanovna_cal_standards.md** - Building calibration standards
4. **nanovna_port_calibration.md** - SOL/SOLT procedures
5. **nanovna_verification.md** - Testing calibration quality

### Examples

6. **examples/nanovna_complete_calibration.md** - Full worked example
7. **examples/homebrew_50ohm_load.md** - Building precision loads

---

## Cost Breakdown

### Frequency Calibration

**Option 1: FM Broadcast (Free)**
- Cost: $0
- Uses: TinySA or RTL-SDR (if you have)
- Accuracy: ±1-5 ppm
- Time: 15-30 minutes

**Option 2: GPS Method ($15-25)**
- Cost: $15-25
- Accuracy: ±0.01 ppm
- Time: 2-4 hours (includes build)
- **Reuses:** TinySA GPS setup from earlier project

### Port Calibration Standards

**Minimum (Using Supplied Kit):**
- Cost: $0 (use kit that came with NanoVNA)
- Quality: Fair to good
- Verification: Needed

**DIY Homebrew Standards:**
- SHORT: $0-2 (SMA connector, copper)
- OPEN: $0 (SMA female, nothing)
- LOAD: $5-15 (precision resistors, SMA)
- THRU: $2-5 (SMA adapters)
- **Total: $10-25**

**Commercial Cal Kit:**
- Basic: $30-50 (SMA cal kit)
- Good: $100-200 (Keysight, Mini-Circuits)
- Excellent: $500+ (professional)

**Recommendation:** Use supplied kit, verify with known components, build DIY backup

---

## Calibration Quality Levels

### Level 1: Basic (Good Enough for Most)

**Frequency:**
- FM broadcast calibration
- ±5 ppm accuracy
- 15 minutes

**Port:**
- Supplied calibration kit
- No verification
- 2 minutes per calibration

**Total Time:** 30 minutes
**Cost:** $0
**Suitable For:** Antenna tuning, SWR checks, filter response

---

### Level 2: Good (Recommended)

**Frequency:**
- GPS calibration
- ±0.01 ppm accuracy
- 2-4 hours (one-time build)

**Port:**
- Supplied kit + verification
- Check LOAD with multimeter
- Build DIY backup standards
- 1 hour (one-time build)

**Total Time:** 3-5 hours (one-time)
**Cost:** $20-40
**Suitable For:** Precision impedance, component characterization, filter design

---

### Level 3: Excellent (For Critical Work)

**Frequency:**
- GPS + temperature compensation
- Long-term logging
- ±0.005 ppm accuracy
- 8+ hours (full characterization)

**Port:**
- Commercial cal kit
- DIY verification standards
- Multiple cross-checks
- TRL (Thru-Reflect-Line) calibration
- 2-3 hours

**Total Time:** 10+ hours
**Cost:** $100-300
**Suitable For:** Research, RF design, critical measurements

---

## Common Calibration Mistakes

### Mistake 1: Skipping Frequency Calibration

**Problem:** Measurements at wrong frequencies

**Example:**
- Design filter for 147.0 MHz
- NanoVNA is +30 ppm fast
- Actually measures at 147.0044 MHz
- Filter appears off-frequency

**Solution:** Check and calibrate timebase first

---

### Mistake 2: Not Re-Calibrating Ports

**Problem:** Port cal is NOT permanent

**Wrong:**
- Calibrate once
- Measure for days/weeks
- Wonder why readings drift

**Right:**
- Calibrate before each session
- Calibrate after cable change
- Calibrate for each frequency range

**Why:** Temperature, cable flex, connector wear all affect calibration

---

### Mistake 3: Poor Quality SHORT

**Problem:** SHORT isn't really 0Ω at high frequency

**Symptoms:**
- SWR not infinite at 1:1
- Smith chart doesn't center
- Return loss limited to 40 dB

**Solution:**
- Use low-inductance SHORT
- Keep connection short
- Verify with known good short

---

### Mistake 4: Incorrect LOAD Value

**Problem:** "50Ω" resistor is actually 47Ω or 56Ω, or inductive

**Symptoms:**
- SWR shows 1.1:1 instead of 1.0:1
- Smith chart offset from center
- Impedance readings consistently off

**Solution:**
- Measure LOAD with good DMM
- Use 1% tolerance resistors
- Verify non-inductive (measure at multiple frequencies)
- Build and test DIY loads

---

### Mistake 5: Calibrating Wrong Frequency Range

**Problem:** Calibrate 1-100 MHz, measure at 1 GHz

**Why It Fails:**
- Calibration only valid for calibrated range
- Must re-calibrate for different frequency span

**Solution:**
- Calibrate the exact frequency range you'll measure
- For sweeps: Calibrate full sweep range
- For spot frequency: Calibrate narrow span around frequency

---

## Verification Methods

### Quick Checks

**50Ω Load:**
- Should measure exactly 50.0Ω
- SWR should be 1.00:1
- Smith chart: Dead center

**SHORT:**
- Should measure 0Ω (or very low)
- SWR should be infinite (>100:1)
- Smith chart: Far left

**OPEN:**
- Should measure infinite Ω
- SWR should be infinite
- Smith chart: Far right

### Advanced Verification

**Known Components:**
- Measure 100Ω resistor (should be 100Ω)
- Measure quarter-wave stub (should be resonant)
- Measure commercial filter (compare to datasheet)

**Cross-Check:**
- Measure same component with TinySA
- Compare to another NanoVNA
- Measure known transmission line length

---

## Typical Accuracy Expectations

### Frequency Accuracy

| Calibration Method | Accuracy | At 1 GHz |
|-------------------|----------|----------|
| Uncalibrated | ±50 ppm | ±50 kHz |
| FM broadcast | ±5 ppm | ±5 kHz |
| GPS | ±0.01 ppm | ±10 Hz |

### Impedance Accuracy

| Calibration Quality | Impedance | SWR |
|--------------------|-----------|-----|
| No calibration | ±50% | ±0.5 |
| Basic SOL | ±10% | ±0.1 |
| Good SOL | ±5% | ±0.05 |
| Excellent SOL + verification | ±1% | ±0.01 |

### Frequency Response Accuracy

| Calibration | Amplitude | Phase |
|-------------|-----------|-------|
| Basic | ±1 dB | ±10° |
| Good | ±0.5 dB | ±5° |
| Excellent | ±0.2 dB | ±2° |

---

## What You'll Learn

### Frequency Calibration

- Crystal oscillator stability
- GPS timing technology
- Parts-per-million (ppm) calculations
- Temperature effects
- Long-term drift

### Port Calibration

- Vector network analyzer theory
- S-parameters (S11, S21, S12, S22)
- Smith chart interpretation
- Calibration standard requirements
- Error correction math (hidden from user, but you'll understand it)

### Practical Skills

- Building precision 50Ω loads
- SMA connector assembly
- Impedance measurement
- Filter characterization
- Transmission line analysis
- Troubleshooting RF circuits

---

## Tools Needed

### For Frequency Calibration

**Minimum:**
- NanoVNA
- Computer
- FM radio or RTL-SDR

**Recommended:**
- GPS module ($10)
- Arduino ($10)
- Breadboard ($3)

### For Port Calibration

**Minimum:**
- NanoVNA
- Supplied calibration kit
- Computer (optional but helpful)

**Recommended:**
- Digital multimeter (check resistors)
- Soldering iron
- SMA connectors and tools
- Precision resistors (1%)
- Heat shrink tubing

**Advanced:**
- Oscilloscope
- TinySA (for cross-checking)
- Second NanoVNA (comparison)
- Vector impedance meter

---

## Project Organization

### Phase 1: Frequency Calibration

**Document:** nanovna_frequency_cal.md

**Tasks:**
1. Choose method (GPS, WWV, FM)
2. Build hardware (if GPS)
3. Measure crystal error
4. Apply correction
5. Verify

**Time:** 2-4 hours (first time)
**Result:** Accurate frequency display

---

### Phase 2: Build Calibration Standards

**Document:** nanovna_cal_standards.md

**Tasks:**
1. Inventory supplied kit
2. Build DIY SHORT
3. Build DIY OPEN
4. Build DIY LOAD (50Ω)
5. Build DIY THRU
6. Verify all standards

**Time:** 2-3 hours
**Result:** Verified calibration standards

---

### Phase 3: Port Calibration Procedure

**Document:** nanovna_port_calibration.md

**Tasks:**
1. Connect calibration standards
2. Perform SOL or SOLT
3. Save calibration
4. Verify with known components

**Time:** 5-10 minutes (each measurement session)
**Result:** Accurate impedance measurements

---

### Phase 4: Verification and Testing

**Document:** nanovna_verification.md

**Tasks:**
1. Measure known resistors
2. Check commercial filters
3. Measure transmission lines
4. Cross-check with TinySA
5. Document accuracy limits

**Time:** 1-2 hours (one-time)
**Result:** Confidence in measurements

---

## Example Applications

### After Calibration, You Can:

**Antenna Work:**
- Measure antenna impedance
- Tune antenna to exact frequency
- Measure feedline loss
- Check SWR across band
- Design matching networks

**Filter Design:**
- Verify filter response
- Measure insertion loss
- Check bandwidth
- Characterize rejection
- Tune crystal filters

**Component Characterization:**
- Measure inductors (L and Q)
- Measure capacitors (C and ESR)
- Check resistor values at RF
- Characterize transformers
- Test baluns

**Transmission Lines:**
- Measure cable loss
- Find cable faults (TDR)
- Check connector quality
- Measure velocity factor
- Verify impedance

**Amplifier Testing:**
- Measure gain (S21)
- Check input/output match
- Measure return loss
- Characterize bandwidth

---

## Success Criteria

### Your NanoVNA is Well-Calibrated If:

**Frequency:**
- ✓ Known frequency reads within 1 kHz (at 1 GHz)
- ✓ Filter resonances match design frequency
- ✓ Stable over temperature
- ✓ Repeatable measurements

**Impedance:**
- ✓ 50Ω load measures 50.0Ω ± 2.5Ω
- ✓ SWR 1.00:1 on good load
- ✓ SHORT shows <1Ω
- ✓ OPEN shows >1000Ω
- ✓ Smith chart centers properly

**S-Parameters:**
- ✓ Through cable shows expected loss
- ✓ Filter matches datasheet ± 0.5 dB
- ✓ Phase measurements reasonable
- ✓ Reciprocal measurements agree (S12 = S21)

---

## Next Steps

### Recommended Learning Path

1. **Read this overview** - Understand what's needed ✓
2. **Start with frequency cal** - See [nanovna_frequency_cal.md](nanovna_frequency_cal.md)
3. **Build standards** - See [nanovna_cal_standards.md](nanovna_cal_standards.md)
4. **Learn port cal** - See [nanovna_port_calibration.md](nanovna_port_calibration.md)
5. **Practice and verify** - See [nanovna_verification.md](nanovna_verification.md)
6. **Study complete example** - See [examples/nanovna_complete_calibration.md](examples/nanovna_complete_calibration.md)

### Quick Start Options

**Option A: Just Want It To Work (2 hours)**
- FM frequency cal (30 min)
- Use supplied cal kit (5 min per session)
- Basic verification (30 min)
- Start measuring!

**Option B: Best Accuracy (1 weekend)**
- GPS frequency cal (4 hours)
- Build DIY standards (3 hours)
- Full verification (2 hours)
- Professional-grade results

**Option C: Deep Learning (1 week)**
- All methods, all standards
- Temperature characterization
- Cross-checking everything
- Understand VNA theory deeply

---

## Summary

### The Challenge

NanoVNA is an amazing $50 tool, but needs calibration for accurate measurements.

### The Solution

- **Frequency:** Use GPS/WWV/FM (same as TinySA) - $0-25
- **Port:** Build or verify calibration standards - $0-25
- **Total:** $0-50, 2-8 hours

### The Result

- Measurements as good as $5000 VNA
- Confidence in your data
- Deep understanding of RF measurement
- Capability to design and verify RF circuits

---

**Ready to start? Continue to [nanovna_frequency_cal.md](nanovna_frequency_cal.md)!**
