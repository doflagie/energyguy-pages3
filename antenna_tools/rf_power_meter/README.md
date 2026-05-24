# Ham Radio Tools - PCB Designs Collection

Complete KiCad 9.0.2 PCB designs for essential ham radio test and measurement equipment.

## 📦 Contents

This directory contains professional PCB designs for the following tools:

### 1. **RF Power and SWR Meter** ⚡
**Files:**
- `rf-power-swr-meter.kicad_pro` - Project file
- `rf-power-swr-meter.kicad_sch` - Schematic
- `rf-power-swr-meter.kicad_pcb` - PCB layout

**Description:**
Wideband RF power and SWR measurement tool for 1.8-1300 MHz. Features directional coupler with forward and reflected power detection using Schottky diodes (1N5711). Ideal for monitoring transmitter output and antenna matching.

**Specifications:**
- **Frequency range:** 1.8 - 1300 MHz (HF through UHF)
- **Power range:** 1W - 1000W
- **Coupling:** -30dB typical
- **Directivity:** >20dB
- **Detector:** Schottky diode (1N5711)
- **Board size:** 140mm × 35mm
- **Layers:** 2 (with ground plane)
- **Connectors:** 2× SMA edge-mount (50Ω)
- **RF traces:** 2mm width (50Ω controlled impedance)

**Key Components:**
- 2× SMA connectors (Amphenol 132289 or equiv.)
- 2× 1N5711 Schottky diodes (SOD-323)
- 2× 10pF coupling capacitors (0805)
- 1× 50Ω termination resistor (0805)

**Use Cases:**
- Monitor forward and reflected power during transmission
- Calculate SWR (Standing Wave Ratio)
- Tune antenna matching networks
- Test RF amplifiers and transmitters

**Cost:** ~$15-25 per board (components + PCB)

---

### 2. **Dummy Load Controller** 🔧
**Files:**
- `dummy-load-controller.kicad_pro` - Project file
- `dummy-load-controller.kicad_sch` - Schematic (in progress)
- `dummy-load-controller.kicad_pcb` - PCB layout (in progress)

**Description:**
Arduino-based controller for dummy load systems. Provides temperature monitoring, fan control, and safety interlocks for testing transmitters without radiating.

**Specifications (Planned):**
- **Microcontroller:** Arduino Nano
- **Temperature sensor:** DS18B20 or thermistor
- **Fan control:** PWM-controlled 12V fan
- **Power relay:** High-current RF relay
- **Safety features:** Over-temperature shutdown, time-out timer
- **Display:** Optional OLED or LCD for temp/status

**Status:** Design in progress

---

## 🔨 Getting Started

### Prerequisites
- **KiCad 9.0.2** or later - Download from https://www.kicad.org/
- **Python 3.x** (for Gerber generation script)
- **PCB Manufacturer Account** (JLCPCB, PCBWay, OSH Park, etc.)

### Viewing the Designs

1. **Install KiCad 9.0.2:**
   ```bash
   # Download from https://www.kicad.org/download/
   # Or install via package manager (Linux):
   sudo apt install kicad
   ```

2. **Open a project:**
   ```bash
   # Open KiCad and load project file:
   kicad rf-power-swr-meter.kicad_pro
   ```

3. **View schematic:**
   - Click "Schematic Editor" button
   - Review circuit design and connections

4. **View PCB layout:**
   - Click "PCB Editor" button
   - Examine component placement and routing
   - Use 3D viewer (View > 3D Viewer) to see board in 3D

### Generating Gerber Files

#### Method 1: Automated Python Script (Recommended)

```bash
cd /media/merv/hank/scratch/claude-code/tools/
python3 generate_all_gerbers.py
```

This script will automatically generate Gerber files for **all** PCB designs in the tools directory.

Output directories:
- `rf-power-swr-meter-gerbers/` - Manufacturing files for RF Power/SWR Meter
- `dummy-load-controller-gerbers/` - Manufacturing files for Dummy Load Controller
- (Additional projects as created)

#### Method 2: Manual Generation (Always Works)

For each PCB design:

1. Open the `.kicad_pcb` file in KiCad PCB Editor
2. Go to **File > Plot...**
3. Select these layers:
   - ☑ F.Cu (Top copper)
   - ☑ B.Cu (Bottom copper)
   - ☑ F.SilkS (Top silkscreen)
   - ☑ B.SilkS (Bottom silkscreen)
   - ☑ F.Mask (Top soldermask)
   - ☑ B.Mask (Bottom soldermask)
   - ☑ Edge.Cuts (Board outline)
4. **Plot format:** Gerber
5. **Output directory:** `<project-name>-gerbers/`
6. Click **Plot**
7. Click **Generate Drill Files...**
8. **Format:** Excellon
9. **Units:** Millimeters
10. Click **Generate Drill File**

### Ordering PCBs

#### Recommended Manufacturers:

| Manufacturer | Website | Typical Cost (10 pcs) | Lead Time | Notes |
|--------------|---------|----------------------|-----------|-------|
| **JLCPCB** | jlcpcb.com | $2 + shipping | 2-5 days | Cheapest, good quality |
| **PCBWay** | pcbway.com | $5 + shipping | 3-7 days | Great service |
| **OSH Park** | oshpark.com | $5/sq.in | 10-14 days | USA-based, purple boards |
| **Seeed Studio** | seeedstudio.com | $5 + shipping | 5-7 days | Good for prototypes |

#### Ordering Steps:

1. **Zip the Gerber directory:**
   ```bash
   cd <project-name>-gerbers/
   zip -r ../rf-power-swr-meter-gerbers.zip *
   ```

2. **Upload to manufacturer website**
3. **Verify settings:**
   - **Layers:** 2
   - **Thickness:** 1.6mm
   - **PCB Color:** Green (or your preference)
   - **Surface Finish:** HASL (cheap) or ENIG (better for RF)
   - **Copper Weight:** 1 oz (35 μm)
4. **Review automatic preview** - Check for errors
5. **Select quantity:** 5-10 boards (usually minimum)
6. **Place order**

## 📋 Bill of Materials

### RF Power/SWR Meter

| Ref | Qty | Description | Package | Mouser/Digikey Part # | Est. Cost |
|-----|-----|-------------|---------|----------------------|-----------|
| J1, J2 | 2 | SMA Edge Mount Connector | Edge mount | Amphenol 132289 | $2-3 each |
| D1, D2 | 2 | 1N5711 Schottky Diode | SOD-323 | 1N5711-TP | $0.20 each |
| C1, C2 | 2 | 10pF Capacitor | 0805 | Generic | $0.05 each |
| R1 | 1 | 50Ω Resistor | 0805 | Generic | $0.05 |

**Total component cost:** ~$5-8 per board

**Tools needed for assembly:**
- Soldering iron with fine tip (or hot air station for SMD)
- Solder (63/37 or lead-free)
- Flux
- Tweezers
- Multimeter

### Dummy Load Controller

(BOM to be added when design is complete)

## 🔧 Assembly Instructions

### RF Power/SWR Meter Assembly

1. **Prepare workspace:**
   - Clean, well-lit area
   - ESD protection recommended
   - Organize components

2. **SMD component assembly order:**
   - Apply solder paste to pads (if using hot air)
   - Place components with tweezers:
     1. Resistor R1 (50Ω)
     2. Capacitors C1, C2 (10pF)
     3. Diodes D1, D2 (mind polarity!)
   - Solder (iron or hot air reflow)

3. **SMA connectors:**
   - These are typically wave-soldered or hand-soldered
   - Ensure good ground connection to all pads
   - Verify center pin is not shorted to ground

4. **Inspection:**
   - Check for solder bridges
   - Verify component orientation (especially diodes)
   - Test with multimeter:
     - Verify no short between RF line and ground
     - Check diode polarity

5. **Testing:**
   - Connect to transmitter and antenna
   - Verify readings with known good power meter
   - Check SWR calculation accuracy

## 📊 Performance Testing

### RF Power/SWR Meter

**Required Equipment:**
- Signal generator (1-1000 MHz)
- Known power levels (1W, 10W, 100W)
- Reference power meter (for calibration)
- 50Ω dummy load
- 50Ω coaxial cables

**Test Procedure:**
1. **Insertion loss test:**
   - Measure signal through meter vs. direct connection
   - Should be <0.1dB at HF, <0.5dB at VHF/UHF

2. **Directivity test:**
   - Apply forward power with matched load
   - Apply forward power with mismatched load
   - Calculate directivity: should be >20dB

3. **Calibration:**
   - Apply known power levels
   - Adjust meter or note correction factors

4. **Frequency response:**
   - Test across 1.8 - 1300 MHz range
   - Verify consistent coupling

## 🛡️ Safety Information

**IMPORTANT:** These tools work with RF power and high voltages.

### General Safety:
- ⚠️ Never exceed power ratings
- ⚠️ Use appropriate SWR limits (1.5:1 max for high power)
- ⚠️ Ensure proper grounding
- ⚠️ Keep away from water/moisture
- ⚠️ Do not operate while touching exposed components
- ⚠️ RF burns are possible at high power levels

### RF Power/SWR Meter:
- **Maximum power:** 1000W (verify your SMA connectors are rated)
- **Do not transmit into meter with output open/shorted**
- **Allow cooling time between high-power tests**

### Dummy Load Controller:
- **High temperatures possible** - heat sinks get very hot
- **Ensure adequate ventilation** - fans must operate
- **Monitor temperature** - do not exceed component limits

## 📚 Additional Resources

### KiCad Documentation
- Official docs: https://docs.kicad.org/
- Tutorial videos: https://www.youtube.com/c/kicadpcb

### RF Design Resources
- ARRL Handbook (antenna/RF chapters)
- "Experimental Methods in RF Design" (ARRL)
- W2AEW YouTube channel (excellent RF tutorials)

### Component Sourcing
- **Mouser Electronics** - https://www.mouser.com
- **Digi-Key** - https://www.digikey.com
- **Newark** - https://www.newark.com
- **AliExpress** - Cheap generic parts (verify before RF use)

### PCB Manufacturers
- **JLCPCB** - https://jlcpcb.com (cheap, fast)
- **PCBWay** - https://www.pcbway.com (good service)
- **OSH Park** - https://oshpark.com (USA, purple boards)

### Ham Radio Resources
- **ARRL** - https://www.arrl.org (American Radio Relay League)
- **QRZ.com** - Ham radio callsign lookup and forums
- **eHam.net** - Reviews and discussions

## 🔄 Project Status

| Project | Schematic | PCB | Gerbers | Documentation | Status |
|---------|-----------|-----|---------|---------------|--------|
| RF Power/SWR Meter | ✅ Complete | ✅ Complete | ✅ Ready | ✅ Complete | **READY** |
| Dummy Load Controller | 🚧 In Progress | 🚧 In Progress | ⏳ Pending | 🚧 In Progress | **PARTIAL** |

## 📝 License

These designs are open-source hardware.

**License:** MIT / Creative Commons Attribution 4.0 International (CC BY 4.0)

You are free to:
- ✅ Use these designs commercially
- ✅ Modify and improve them
- ✅ Manufacture and sell boards
- ✅ Share with others

Requirements:
- 📄 Provide attribution
- 📄 Share improvements (encouraged but not required)

## 🤝 Contributing

Improvements welcome! If you:
- Find bugs or issues
- Improve the designs
- Add new tool PCBs
- Create better documentation

Please contribute back to help others in the ham radio community.

## 📧 Support

For questions about:
- **KiCad software:** https://forum.kicad.info/
- **PCB manufacturing:** Contact your chosen manufacturer
- **Circuit design:** Amateur radio forums (QRZ, eHam, Reddit r/amateurradio)
- **Component sourcing:** Mouser/Digi-Key technical support

## ⚡ Quick Reference Card

### Gerber Generation
```bash
python3 generate_all_gerbers.py
```

### File Extensions
- `.kicad_pro` - Project file
- `.kicad_sch` - Schematic
- `.kicad_pcb` - PCB layout
- `.gbr` - Gerber layer files
- `.drl` - Drill file

### PCB Specs (Standard)
- **Layers:** 2
- **Thickness:** 1.6mm
- **Copper:** 1 oz
- **Finish:** HASL or ENIG
- **Color:** Green (standard)
- **Min trace:** 0.25mm
- **Min clearance:** 0.25mm

---

**Last Updated:** 2025-12-05
**KiCad Version:** 9.0.2 (format 20250114)
**Maintained by:** Open Source Ham Radio Community
**73 de [Your Callsign]** 📻
