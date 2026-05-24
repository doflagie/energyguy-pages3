# PCB Layout and Gerber Generation Notes — TM-CWND-PCB-001 Rev A

## Board Overview

**Board name:** Coil Winder Controller
**Dimensions:** 120mm × 80mm
**Layer count:** 2 (F.Cu, B.Cu)
**PCB thickness:** 1.6mm FR4
**Surface finish:** HASL or ENIG
**Min trace:** 0.25mm signal / 1.5mm power
**Min via:** 0.3mm drill / 0.6mm pad

---

## Component Placement (top view, coordinates from lower-left origin)

```
  ┌────────────────────────────────────────────────────────────────────────┐
  │  (0,80)                                               (120,80)        │
  │                                                                        │
  │  [TMC2208-A]     [TMC2208-B]      [ESP32-WROOM-32]                   │
  │   U1 @ 15,65     U2 @ 45,65       U3 @ 85,40                         │
  │                                                                        │
  │  [VMOT Cap]      [5V LDO]         [3V3 LDO]         [SD Card]        │
  │   C1,C2 @10,50   U4 @ 60,70       U5 @ 75,70        J6 @ 110,35     │
  │                                                                        │
  │  [Motor A Out]   [Motor B Out]    [Sense R × 4]                       │
  │   J1 @ 10,10     J2 @ 35,10       R1-R4 @ 25,52                      │
  │                                                                        │
  │  [DC Power In]   [I/O Header]     [Encoder HDR]     [Sensors HDR]    │
  │   J3 @ 5,5       J4 @ 60,10       J5 @ 80,10        J7 @ 100,10     │
  │                                                                        │
  │  (0,0)                                                (120,0)         │
  └────────────────────────────────────────────────────────────────────────┘
```

---

## Netlist Summary

| Net Name | Connected To |
|----------|-------------|
| VIN_24V | J3 pin 1, C1+, C2+, U1-VMOT, U2-VMOT |
| GND | J3 pin 2, C1−, C2−, U1-GND, U2-GND, U4-GND, U5-GND, all GND pins |
| +5V | U4-OUT, U1-VCC, U2-VCC, U3-5V, J4-5V |
| +3V3 | U5-OUT, U3-3V3, J4-3V3, J5-3V3, J7-3V3 |
| SPIN_STEP | U3-GPIO18 → U1-STEP |
| SPIN_DIR | U3-GPIO19 → U1-DIR |
| SPIN_EN | U3-GPIO5 → U1-EN |
| SPIN_MS1 | U3-GPIO23 → U1-MS1 |
| SPIN_MS2 | U3-GPIO22 → U1-MS2 |
| TRAV_STEP | U3-GPIO25 → U2-STEP |
| TRAV_DIR | U3-GPIO26 → U2-DIR |
| TRAV_EN | U3-GPIO27 → U2-EN |
| TRAV_MS1 | U3-GPIO14 → U2-MS1 |
| TRAV_MS2 | U3-GPIO12 → U2-MS2 |
| MOT_A_1A | U1-1A → J1 pin 1 |
| MOT_A_1B | U1-1B → J1 pin 2 |
| MOT_A_2A | U1-2A → J1 pin 3 |
| MOT_A_2B | U1-2B → J1 pin 4 |
| MOT_B_1A | U2-1A → J2 pin 1 |
| MOT_B_1B | U2-1B → J2 pin 2 |
| MOT_B_2A | U2-2A → J2 pin 3 |
| MOT_B_2B | U2-2B → J2 pin 4 |
| I2C_SDA | U3-GPIO21 → [4.7kΩ to +3V3] → J4-SDA |
| I2C_SCL | U3-GPIO22 → [4.7kΩ to +3V3] → J4-SCL |
| TURN_SENSOR | J7-pin1 → [10kΩ to +3V3] → U3-GPIO35 |
| WIRE_BREAK | J7-pin2 → [10kΩ to +3V3] → U3-GPIO13 |
| FOOT_PEDAL | J7-pin3 → [10kΩ to +3V3] → U3-GPIO15 |
| TRAV_LIM | J7-pin4 → [10kΩ to +3V3] → U3-GPIO39 |
| SPEED_POT | J7-pin5 → U3-GPIO36 |
| ENC_A | J5-pin1 → [10kΩ to +3V3] → U3-GPIO32 |
| ENC_B | J5-pin2 → [10kΩ to +3V3] → U3-GPIO33 |
| ENC_BTN | J5-pin3 → [10kΩ to +3V3] → U3-GPIO34 |
| SD_CS | U3-GPIO4 → J6-CS |
| SD_CLK | U3-GPIO14* → J6-CLK |
| SD_MOSI | U3-GPIO13* → J6-MOSI |
| SD_MISO | U3-GPIO12* → J6-MISO |

*Note: GPIO 12/13/14 overlap with TRAV_MS1/MS2 and WIRE_BREAK.
Resolve by using hardware SPI on GPIO 18/19/23 with VSPI bus,
and moving TRAV_MS1/MS2 to different pins (e.g. GPIO 2/4).
Update config.h accordingly.

---

## Bill of Materials (PCB)

| Ref | Component | Value/Part | Qty | Footprint |
|-----|-----------|------------|-----|-----------|
| U1 | TMC2208 module | StepStick-style | 1 | 2.54mm header |
| U2 | TMC2208 module | StepStick-style | 1 | 2.54mm header |
| U3 | ESP32-WROOM-32 DevKit | ESP32-DevKitC-V4 | 1 | 2.54mm header × 2 |
| U4 | 5V LDO regulator | LM7805 or L7805 | 1 | TO-220 |
| U5 | 3.3V LDO | AMS1117-3.3 | 1 | SOT-223 |
| C1 | Electrolytic cap | 100µF 35V | 1 | 8mm radial |
| C2 | Electrolytic cap | 100µF 35V | 1 | 8mm radial |
| C3 | Ceramic cap | 100nF | 1 | 0805 |
| C4 | Ceramic cap | 10µF | 1 | 1206 |
| R1–R4 | Current sense resistor | 0.1Ω 1% 1W | 4 | 2512 |
| R5–R12 | Pullup resistors | 10kΩ 0.1W | 8 | 0805 |
| R13–R14 | I2C pullup | 4.7kΩ | 2 | 0805 |
| R15 | LED current limit | 330Ω | 1 | 0805 |
| D1 | Status LED | Green 3mm | 1 | LED-3mm |
| J1 | Motor A connector | 4-pin 5.08mm | 1 | KF2EDGK-5.08-4P |
| J2 | Motor B connector | 4-pin 5.08mm | 1 | KF2EDGK-5.08-4P |
| J3 | Power input | 2-pin 5.08mm screw | 1 | KF2EDGK-5.08-2P |
| J4 | I/O header | 2×10 2.54mm | 1 | IDC-20 |
| J5 | Encoder header | 1×5 2.54mm | 1 | PH2.54-5P |
| J6 | SD card module | 6-pin 2.54mm | 1 | PH2.54-6P |
| J7 | Sensor header | 1×8 2.54mm | 1 | PH2.54-8P |
| SW1 | Reset button | Tactile SPST | 1 | 6×6mm |

---

## Generating Gerber Files from KiCad

1. Open KiCad PCB Editor (Pcbnew)
2. File → Fabrication Outputs → Gerbers (.gbr)
3. Select output directory: `coil_winder/pcb/gerbers/`
4. Check: F.Cu, B.Cu, F.Silkscreen, B.Silkscreen, F.Mask, B.Mask, Edge.Cuts
5. Click Generate Drill Files → Generate Drill File (Excellon format)
6. Zip contents of `gerbers/` folder for PCB manufacturer upload

**Recommended PCB fabs:** JLCPCB, PCBWay, OSH Park (US)
**JLCPCB defaults:** FR4 1.6mm, 1oz Cu, HASL, green silkscreen = ~$5/5pcs

---

## Trace Width Guidelines

| Net type | Minimum width | Recommended |
|----------|--------------|-------------|
| Signal (logic) | 0.25mm | 0.30mm |
| I2C/SPI | 0.25mm | 0.30mm |
| 5V power | 0.50mm | 0.80mm |
| 24V motor | 1.50mm | 2.00mm |
| GND pour | copper fill | copper fill |

Use copper pour on both layers for GND. This minimizes noise and thermal resistance.

---

## Design Review Checklist

- [ ] Decoupling capacitors within 5mm of each IC power pin
- [ ] Motor current traces ≥ 2mm wide, short as possible
- [ ] VMOT bulk cap (100µF) within 10mm of stepper driver VMOT pin
- [ ] ESP32 antenna area free of copper pour
- [ ] Reset button accessible from outside enclosure
- [ ] All connectors at board edges for easy cable routing
- [ ] Test points on: +24V, +5V, +3V3, GND, SPIN_STEP, TRAV_STEP
- [ ] Motor connector polarity marked (A+/A−/B+/B−)
- [ ] Power supply polarity protection (schottky diode or P-channel MOSFET on +24V)
