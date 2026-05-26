# TinySA 30MHz Internal Calibrator - Homebrew Calibration Methods

## Project Overview

This project provides **cheap, homebrew methods** to calibrate the TinySA's 30MHz internal calibration signal **without any calibrated test equipment**.

### The Challenge

The TinySA spectrum analyzer includes a 30MHz internal calibration signal, but how do you calibrate the calibrator itself when you don't have access to expensive calibrated equipment like:
- Rubidium frequency standards ($500+)
- Cesium beam standards ($5000+)
- Calibrated signal generators ($1000+)
- GPS-disciplined oscillators (GPSDO) ($200+)

### The Solution

Use **freely available, highly accurate frequency references** that anyone can access:

1. **GPS Satellites** - Atomic clock accuracy via $10 GPS module
2. **Time Standard Radio Broadcasts** - WWV, CHU, etc.
3. **FM Broadcast Stations** - Known frequencies locked to atomic standards
4. **Crystal Characterization** - Temperature compensation techniques

---

## Quick Start

### Recommended Method: GPS-Based Calibration

**Cost:** ~$15-25
**Accuracy:** ±0.01 ppm (parts per million) = ±0.3 Hz @ 30MHz
**Time Required:** 2-4 hours

**What You Need:**
- $10-20 GPS module with 1PPS output (NEO-6M, NEO-7M, NEO-8M)
- Frequency counter or Arduino
- Basic components (resistors, capacitors)
- TinySA Ultra or TinySA Basic

**See:** [GPS Calibration Method](gps_calibration.md)

---

## All Calibration Methods

### Method 1: GPS 1PPS (Recommended)
- **Accuracy:** ±0.01 ppm
- **Cost:** $15-25
- **Difficulty:** Easy
- **Documentation:** [gps_calibration.md](gps_calibration.md)

### Method 2: Time Standard Radio Broadcasts
- **Accuracy:** ±0.1 ppm
- **Cost:** $0-10 (receiver or SDR)
- **Difficulty:** Medium
- **Documentation:** [radio_standard_calibration.md](radio_standard_calibration.md)

### Method 3: FM Broadcast Stations
- **Accuracy:** ±1-10 ppm
- **Cost:** $5-15
- **Difficulty:** Easy
- **Documentation:** [fm_broadcast_calibration.md](fm_broadcast_calibration.md)

### Method 4: Crystal Temperature Characterization
- **Accuracy:** ±2-5 ppm
- **Cost:** $10-20
- **Difficulty:** Hard
- **Documentation:** [crystal_characterization.md](crystal_characterization.md)

---

## What's Included

### Documentation Files

1. **README.md** (this file) - Project overview
2. **gps_calibration.md** - GPS 1PPS method (primary)
3. **radio_standard_calibration.md** - WWV/CHU/MSF broadcasts
4. **fm_broadcast_calibration.md** - FM station method
5. **crystal_characterization.md** - TCXO compensation
6. **construction_guide.md** - Hardware build instructions
7. **verification_procedures.md** - Testing and validation
8. **troubleshooting.md** - Common issues and fixes

### Example Projects

9. **examples/gps_30mhz_reference.md** - Complete GPS build
10. **examples/chu_calibration.md** - CHU time standard example
11. **examples/fm_station_calibration.md** - FM method walkthrough

### Schematics and Code

12. **schematics/** - Circuit diagrams
13. **code/** - Arduino sketches, Python scripts

---

## Understanding Frequency Accuracy

### Parts Per Million (ppm)

| Accuracy | @ 30MHz | @ 1GHz | Source |
|----------|---------|--------|--------|
| 0.001 ppm | 0.03 Hz | 1 Hz | Atomic clock |
| 0.01 ppm | 0.3 Hz | 10 Hz | GPS satellite |
| 0.1 ppm | 3 Hz | 100 Hz | TCXO, WWV |
| 1 ppm | 30 Hz | 1 kHz | Cheap crystal |
| 10 ppm | 300 Hz | 10 kHz | Uncalibrated |

### TinySA Specifications

**TinySA Basic:**
- Internal TCXO: ±1 ppm typical
- After calibration: ±0.5 ppm achievable

**TinySA Ultra:**
- Internal VCTCXO: ±0.5 ppm typical
- After calibration: ±0.1 ppm achievable

### Why Calibration Matters

At 1 GHz (TinySA's upper limit):
- **1 ppm error** = 1 kHz frequency error
- **0.1 ppm error** = 100 Hz frequency error
- **0.01 ppm error** = 10 Hz frequency error

For narrow-band signals (SSB, digital modes), even 100 Hz error is significant.

---

## How GPS Provides Atomic Clock Accuracy

### GPS Satellite System

Each GPS satellite carries **multiple atomic clocks** (rubidium and cesium):
- Accuracy: ±1 × 10⁻¹³ (0.00001 ppm)
- Cost per satellite: ~$50M
- **Your cost to access:** $10 GPS module

### 1PPS Signal

GPS modules output a **1 Pulse Per Second (1PPS)** signal:
- Synchronized to GPS atomic clocks
- Accuracy: ±50 nanoseconds typical
- Stability: ±0.01 ppm
- **Free, continuous, worldwide coverage**

### How We Use It

1. GPS provides accurate 1 Hz reference (1PPS)
2. We multiply 1 Hz → 30 MHz (multiply by 30,000,000)
3. Or: We measure TinySA's 30 MHz against GPS 1PPS
4. Calculate frequency error and apply correction

---

## Project Applications

### Direct Applications

- Calibrate TinySA 30MHz internal reference
- Calibrate NanoVNA frequency reference
- Build frequency counters
- Calibrate signal generators
- Calibrate receivers

### Indirect Applications

- Learn frequency synthesis techniques
- Understand PLL (Phase-Locked Loop) operation
- Learn GPS receiver technology
- Understand atomic time standards
- Practice precision measurement

---

## Required Background Knowledge

### Minimal Requirements

- Basic electronics (resistors, capacitors, LEDs)
- Ability to solder
- Ability to use breadboard
- Basic understanding of frequency/oscillators

### Helpful But Not Required

- Arduino programming (examples provided)
- RF circuit design
- PLL theory
- GPS technology

---

## Safety and Legal Notes

### Safety

- Low voltage DC circuits (3.3V, 5V)
- No high voltage or hazardous materials
- GPS modules are receive-only (no transmission)

### Legal Considerations

**GPS Reception:**
- Legal worldwide
- Receive-only, no licensing required
- No transmission involved

**FM Broadcast Reception:**
- Legal for personal use
- May not use for commercial gain
- Fair use for calibration purposes

**Time Standard Reception (WWV/CHU):**
- Legal worldwide
- Intended for public use
- Free service

---

## Success Criteria

### Your Calibration is Successful If:

```
✓ Frequency error < 1 ppm (30 Hz @ 30MHz)
✓ Drift < 0.5 ppm over 24 hours
✓ Repeatable measurements (±0.1 ppm)
✓ Temperature stability characterized
✓ Long-term stability verified (1 week+)
```

### Measurement Validation

Compare your results against multiple methods:
- GPS 1PPS vs. WWV broadcast
- Multiple GPS modules
- Multiple FM stations
- Long-term averaging (24h+)

---

## Cost Breakdown

### Minimum Budget: ~$15

```
GPS Module (NEO-6M):        $10
Breadboard:                  $3
Wire/headers:                $2
```

### Recommended Budget: ~$50

```
GPS Module (NEO-8M):        $20
Arduino Nano:               $10
Frequency counter module:   $10
Breadboard/components:       $5
Enclosure:                   $5
```

### Deluxe Setup: ~$100

```
GPS Module (NEO-M9N):       $40
Arduino Mega:               $20
OLED display:               $10
Precision components:       $15
Enclosure/mounting:         $10
Spare parts/experiments:     $5
```

---

## Time Investment

### Quick Calibration (GPS Method)

- **Setup:** 1-2 hours
- **Measurement:** 30 minutes
- **Verification:** 1 hour
- **Total:** 2-4 hours

### Complete Characterization

- **Build hardware:** 3-4 hours
- **Initial calibration:** 2 hours
- **Temperature testing:** 4-8 hours
- **Long-term monitoring:** 1 week
- **Documentation:** 2-3 hours
- **Total:** ~15 hours + 1 week passive

---

## Quick Reference: Which Method Should I Use?

### Choose GPS Method If:
- You want best accuracy (0.01 ppm)
- You can spend $15-20
- You want set-and-forget operation
- You need continuous monitoring

### Choose Radio Standard Method If:
- You already have an SDR or shortwave receiver
- You live near WWV/CHU transmitter
- You want to learn about time standards
- Cost is more important than ultimate accuracy

### Choose FM Broadcast Method If:
- You need quick-and-dirty calibration
- You have an FM radio or RTL-SDR
- 1-5 ppm accuracy is sufficient
- You want immediate results

### Choose Crystal Characterization If:
- You want to understand crystal physics
- You need offline operation
- You're building custom oscillators
- You want a learning project

---

## Learning Outcomes

After completing this project, you will understand:

### Frequency Standards
- How GPS provides atomic clock accuracy
- How time standard broadcasts work
- Parts per million (ppm) specifications
- Temperature effects on oscillators

### Practical Skills
- GPS module interfacing
- Frequency measurement techniques
- PLL operation and design
- Precision measurement methods
- Temperature compensation

### RF Techniques
- Frequency multiplication
- Harmonic generation and filtering
- Impedance matching
- Signal injection methods

---

## Additional Resources

### GPS Timing Information
- [GPS.gov - Timing](https://www.gps.gov/applications/timing/)
- [U-blox Timing Application Note](https://www.u-blox.com)

### Time Standard Broadcasts
- [NIST WWV/WWVH](https://www.nist.gov/pml/time-and-frequency-division/radio-stations/wwv)
- [NRC CHU Canada](https://nrc.canada.ca/en/certifications-evaluations-standards/canadas-official-time/time-signal)

### TinySA Resources
- [TinySA Wiki](https://tinysa.org/wiki/)
- [TinySA GitHub](https://github.com/erikkaashoek/tinySA)
- [Groups.io TinySA Forum](https://groups.io/g/tinysa)

### Forums and Communities
- r/amateurradio
- EEVBlog Forum
- Groups.io TinySA group

---

## File Structure

```
tinysa_nanovna/
├── README.md                           # This file
├── gps_calibration.md                  # GPS 1PPS method (PRIMARY)
├── radio_standard_calibration.md       # WWV/CHU method
├── fm_broadcast_calibration.md         # FM station method
├── crystal_characterization.md         # TCXO compensation
├── construction_guide.md               # Hardware builds
├── verification_procedures.md          # Testing methods
├── troubleshooting.md                  # Common issues
├── examples/
│   ├── gps_30mhz_reference.md         # Complete GPS build
│   ├── chu_calibration.md             # CHU example
│   └── fm_station_calibration.md      # FM example
├── schematics/
│   ├── gps_frequency_counter.txt      # ASCII schematic
│   ├── pll_30mhz.txt                  # PLL design
│   └── temperature_sensor.txt         # Temp monitoring
└── code/
    ├── gps_frequency_counter.ino      # Arduino sketch
    ├── ppm_calculator.py              # Python tool
    └── temperature_logger.py          # Temp logging
```

---

## Next Steps

1. **Read** [gps_calibration.md](gps_calibration.md) for the recommended method
2. **Order** a GPS module (NEO-6M or NEO-8M)
3. **Build** the simple frequency counter circuit
4. **Measure** your TinySA's 30MHz reference
5. **Calculate** the ppm error
6. **Apply** correction in TinySA settings
7. **Verify** against multiple methods

---

## Contributing

If you develop improvements or alternative methods:
- Document your approach
- Share measurement results
- Compare against GPS baseline
- Consider publishing online

---

## License

**Educational/Amateur Radio Use**
Free for personal, educational, and amateur radio use.
Commercial use requires permission.

---

## Quick Links

- [Start Here - GPS Method](gps_calibration.md)
- [Construction Guide](construction_guide.md)
- [Complete Example](examples/gps_30mhz_reference.md)
- [Troubleshooting](troubleshooting.md)

**Ready to calibrate? Start with [GPS Calibration Method](gps_calibration.md)!**

---

**Document Version:** 1.0
**Last Updated:** 2026-01-02
**Project Status:** In Development
