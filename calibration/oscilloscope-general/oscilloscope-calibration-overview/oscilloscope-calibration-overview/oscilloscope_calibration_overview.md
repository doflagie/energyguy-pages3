# DSO1013D Plus Oscilloscope Calibration - Complete Guide

## Overview

The DSO1013D Plus (Hantek/Owon) oscilloscope requires calibration for accurate measurements. This guide provides **cheap, homebrew methods** using **no calibrated test equipment**.

### What Needs Calibration

**Oscilloscopes have three main calibration aspects:**

1. **Timebase (Horizontal)** - Time/frequency accuracy
2. **Voltage (Vertical)** - Amplitude accuracy
3. **Probe Compensation** - Probe frequency response

---

## Calibration Requirements

### 1. Timebase Calibration

**What it affects:**
- Time measurements (period, pulse width)
- Frequency measurements
- Phase measurements
- Bode plot accuracy

**Typical uncalibrated error:** ±50-100 ppm (±0.005-0.01%)

**Goal:** Calibrate to ±1 ppm or better

**Method:** GPS 1PPS (same as TinySA/NanoVNA)

---

### 2. Voltage Calibration

**What it affects:**
- Amplitude measurements
- DC level measurements
- Peak-to-peak voltage
- RMS calculations

**Typical uncalibrated error:** ±3-5%

**Goal:** Calibrate to ±1% or better

**Method:** Precision voltage references

---

### 3. Probe Compensation

**What it affects:**
- Square wave shape
- Rise time measurements
- High-frequency response

**Typical state:** Needs adjustment every time probe is connected

**Goal:** Flat frequency response, no overshoot/undershoot

**Method:** Built-in cal signal (if available) or external square wave

---

## Quick Summary

| Calibration | Accuracy Goal | Cost | Time | Difficulty |
|-------------|--------------|------|------|------------|
| **Timebase** | ±1 ppm | $15-25 | 2-4 hours | Easy |
| **Voltage** | ±1% | $5-15 | 1-2 hours | Easy |
| **Probe Comp** | Flat response | $0 | 5 min | Very Easy |

**Total: $20-40, one weekend**

---

## Understanding the DSO1013D Plus

### Specifications

**Typical specs:**
- Bandwidth: 100 MHz
- Sample Rate: 1 GSa/s max
- Channels: 2
- Vertical: 2 mV/div to 5 V/div
- Horizontal: 2 ns/div to 50 s/div
- Timebase crystal: Usually 25 MHz or 100 MHz
- Input impedance: 1 MΩ || 15-20 pF

### Internal Architecture

**Timebase:**
```
Crystal Oscillator (25 MHz typical)
    ↓
PLL (multiplies to higher frequencies)
    ↓
Sample clock (up to 1 GHz)
    ↓
ADC sampling
```

**Vertical:**
```
Input → Attenuator → Amplifier → ADC → DSP → Display
         (Voltage divider calibrated in firmware)
```

---

## Calibration Access

### Internal Calibration (Firmware)

Most DSO1013D scopes have internal calibration menus:

**Accessing calibration mode:**
1. Power off scope
2. Hold down specific button while powering on
3. Or navigate to hidden UTILITY → CAL menu

**Warning:** Different firmware versions have different access methods. Check manual or online forums for your specific model.

**What can be calibrated:**
- DC offset (each channel, each V/div setting)
- AC gain (each channel, each V/div setting)
- Timebase frequency
- Trigger levels

### Our Approach

**We'll use external reference methods:**
- Don't need to access internal cal menus
- Verify scope's existing calibration
- If scope is off, we'll know by how much
- Can document errors and apply mental corrections
- Or: Note corrections in label on scope

---

## Calibration Strategy

### Phase 1: Timebase (Frequency)

**Method:** GPS 1PPS (reuse from TinySA project)

**Procedure:**
1. Generate GPS-locked 1 Hz signal (1 PPS)
2. Measure pulse width or period on scope
3. Should read exactly 1.000000 seconds
4. Calculate error
5. Apply correction (if possible) or document

**Accuracy:** ±0.01 ppm

---

### Phase 2: Voltage References

**Method:** Precision voltage references

**Build 4 reference voltages:**
1. **1.25V** - LM4040-1.25 (±0.1%)
2. **2.50V** - LM4040-2.5 (±0.1%)
3. **5.00V** - LM4040-5.0 (±0.1%)
4. **10.0V** - Precision divider from 5V

**Procedure:**
1. Build reference circuit
2. Measure on scope at various V/div settings
3. Compare to known reference values
4. Calculate errors
5. Document or apply corrections

**Accuracy:** ±0.5-1%

---

### Phase 3: Probe Compensation

**Method:** Built-in cal output or external square wave

**Procedure:**
1. Connect probe to cal signal (usually 1 kHz square wave)
2. Adjust probe compensation trimmer
3. Square wave should have flat top, no overshoot
4. Repeat for each probe

**Time:** 2 minutes per probe

---

## Cost Breakdown

### Minimum Budget: $5

**If reusing GPS from TinySA calibration:**
- Voltage references: LM4040 ICs × 3 ($3)
- Resistors for divider ($1)
- Breadboard (have)
- **Total: $5**

### Recommended Budget: $25

**New build:**
- GPS module + Arduino ($20 - reuse from TinySA)
- Voltage references ($5)
- Components, breadboard ($5)
- **Total: $30**

### Deluxe Budget: $50

**For best accuracy:**
- GPS module ($15)
- Arduino ($10)
- Precision voltage references × 4 ($10)
- REF02 +10V reference ($5)
- 0.1% resistors for dividers ($5)
- Enclosure, connectors ($10)
- **Total: $55**

---

## What You'll Achieve

### Before Calibration

**Timebase:**
- Unknown error (could be ±100 ppm)
- Example: 1 ms measures as 1.0001 ms
- At 1 MHz: Could be off by 100 Hz

**Voltage:**
- Unknown error (could be ±5%)
- Example: 5.00V measures as 5.25V
- Can't trust amplitude measurements

**Probes:**
- Frequency-dependent errors
- Overshoot on square waves
- Poor rise time

### After Calibration

**Timebase:**
- Error known to ±0.01 ppm
- Example: 1 ms measures 1.000000 ms ±10 ns
- Frequency measurements accurate to ±1 Hz

**Voltage:**
- Error known to ±1%
- Example: 5.00V measures 5.00V ±0.05V
- Can trust amplitude within ±1%

**Probes:**
- Flat frequency response
- Clean square waves
- Accurate rise time measurements

---

## Tools Needed

### Essential

- DSO1013D oscilloscope (obviously!)
- Computer
- Soldering iron
- Multimeter (any quality, for assembly)
- Breadboard
- Wire, components

### From Previous Projects

- GPS module + Arduino (TinySA calibration)
- Frequency counter (if built)

### Optional

- Good quality DMM (for voltage verification)
- Second oscilloscope (cross-checking)
- Signal generator (verification)

---

## Project Files Structure

### Documentation

1. **oscilloscope_calibration_overview.md** (this file)
2. **oscilloscope_timebase_cal.md** - Timebase calibration
3. **oscilloscope_voltage_cal.md** - Voltage reference building
4. **oscilloscope_probe_comp.md** - Probe compensation
5. **oscilloscope_verification.md** - Testing and verification

### Examples

6. **examples/dso1013d_complete_calibration.md** - Full worked example

---

## Typical Errors Found

### Common Issues

**Timebase errors:**
- ±20-50 ppm typical for uncalibrated scope
- ±100 ppm for very cheap scopes
- Temperature-dependent drift

**Voltage errors:**
- DC offset: ±50-100 mV
- Gain error: ±2-5% per channel
- Different error for each V/div setting
- Nonlinearity at low voltages

**Probe issues:**
- Under-compensated: rounded edges, slow rise time
- Over-compensated: overshoot, ringing
- Damaged probes: total distortion

---

## Calibration Frequency

### How Often to Calibrate

**Timebase:**
- Initial: Full GPS calibration
- Check: Monthly with FM broadcast (5 min)
- Re-calibrate: Annually or if suspected drift

**Voltage:**
- Initial: Full calibration with references
- Check: Before critical measurements
- Re-calibrate: Every 6 months

**Probe Compensation:**
- **Every time you connect a probe!**
- After moving probe
- After temperature change
- Takes 30 seconds - no excuse not to do it

---

## Limitations

### What We Can't Calibrate

**Without opening scope:**
- Internal ADC linearity
- Sample rate accuracy (beyond timebase)
- Bandwidth (3dB point)
- Input protection circuits

**What requires factory calibration:**
- Non-linearity correction
- Temperature compensation tables
- Trigger level DAC
- Attenuator switching

**Good news:** Our external calibration catches most errors!

---

## Safety Notes

### Working with Oscilloscopes

**Safe:**
- External calibration circuits (5-10V max)
- Probe compensation
- Timebase measurement

**Caution:**
- Opening scope case (capacitors retain charge)
- High voltage probes (>50V)
- Mains-powered circuits

**Never:**
- Connect scope to mains voltage directly
- Short scope inputs
- Apply >300V (typical input limit)

**For this project:** Everything is low voltage (≤10V), very safe.

---

## Success Criteria

### Well-Calibrated Oscilloscope

**Timebase:**
- ✓ 1 second GPS pulse measures 1.000 s ±1 ms
- ✓ 1 kHz signal measures 1.000 kHz ±1 Hz
- ✓ Frequency measurements match counter

**Voltage:**
- ✓ 5.00V reference measures 5.00V ±0.05V
- ✓ 1.25V reference measures 1.25V ±0.01V
- ✓ All channels agree ±1%

**Probes:**
- ✓ Square wave has flat top
- ✓ No overshoot or ringing
- ✓ Fast, clean edges

---

## Applications After Calibration

### What You Can Now Measure Accurately

**Time domain:**
- Pulse widths (±1%)
- Period/frequency (±0.01%)
- Rise/fall times
- Phase shifts
- Timing diagrams

**Voltage:**
- DC levels (±1%)
- AC amplitudes (±1-2%)
- Peak-to-peak voltages
- RMS values (if scope calculates)

**Waveforms:**
- PWM duty cycle
- Signal integrity
- Noise measurements
- Transient response

**Digital:**
- Logic levels
- Setup/hold times
- Clock jitter (if low enough)

---

## Comparison to Professional Calibration

### Commercial Calibration Service

**Cost:** $100-300
**Time:** 2-4 weeks turnaround
**Accuracy:** ±0.5% voltage, ±1 ppm timebase
**Includes:** Certificate, traceable to NIST

### Our DIY Calibration

**Cost:** $20-40
**Time:** 1 weekend
**Accuracy:** ±1% voltage, ±0.01 ppm timebase
**Includes:** Documentation, understanding

**Conclusion:** DIY is 10× cheaper, comparable accuracy, you keep your scope!

---

## Learning Outcomes

### Technical Knowledge

- How oscilloscopes work internally
- Voltage reference circuits
- GPS timing applications
- Probe loading effects
- Calibration methodology

### Practical Skills

- Building precision circuits
- Voltage reference design
- Oscilloscope operation
- Measurement uncertainty
- Troubleshooting techniques

### Confidence

- Trust your measurements
- Understand error sources
- Design reliable circuits
- Debug effectively

---

## Next Steps

1. **Read:** [oscilloscope_timebase_cal.md](oscilloscope_timebase_cal.md)
2. **Build:** GPS timebase reference (or reuse from TinySA)
3. **Read:** [oscilloscope_voltage_cal.md](oscilloscope_voltage_cal.md)
4. **Build:** Voltage reference circuit
5. **Calibrate:** Follow procedures
6. **Verify:** Test with known signals
7. **Document:** Record calibration data

---

## Quick Start Options

### Option A: Timebase Only (2 hours)

**If you already have GPS setup:**
1. Connect GPS 1PPS to scope
2. Measure pulse period
3. Calculate error
4. **Done** - know timebase accuracy

**Cost:** $0 (reuse GPS)
**Result:** Know frequency errors

---

### Option B: Voltage Only (2 hours)

**Build simple reference:**
1. Buy LM4040-5.0 ($2)
2. Build basic circuit
3. Measure on scope
4. Calculate errors

**Cost:** $5
**Result:** Know voltage errors

---

### Option C: Complete Calibration (1 weekend)

**Full system:**
1. Timebase with GPS (4 hours)
2. Voltage references (2 hours)
3. Probe compensation (30 min)
4. Verification (2 hours)

**Cost:** $20-40
**Result:** Fully calibrated scope!

---

## Summary

### The Challenge

DSO1013D needs calibration for accurate measurements, but factory calibration costs $100-300 and takes weeks.

### The Solution

- **Timebase:** GPS provides atomic clock accuracy ($15-25)
- **Voltage:** Precision references are cheap ($5-15)
- **Probes:** Built-in cal signal (free)
- **Total:** $20-40, one weekend

### The Result

- Timebase accurate to ±0.01 ppm
- Voltage accurate to ±1%
- Probes properly compensated
- Confidence in measurements
- Deep understanding of oscilloscope operation

---

**Ready to calibrate? Continue to [oscilloscope_timebase_cal.md](oscilloscope_timebase_cal.md)!**
