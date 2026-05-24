# Construction Guide - Calibration Hardware

## Overview

This guide provides detailed construction instructions for homebrew calibration hardware including:

1. **GPS Frequency Counter** - Count 30MHz cycles using GPS 1PPS gate
2. **GPS-Locked 30MHz Reference** - Continuous GPS-disciplined oscillator
3. **WWV Harmonic Generator** - Multiply 5MHz to 30MHz
4. **Temperature Monitor** - Log crystal temperature vs. frequency
5. **Precision Frequency Divider** - Divide 30MHz for easier counting

---

## Project 1: GPS Frequency Counter

### Overview

**Function:** Count TinySA's 30MHz signal for exactly 1 second (GPS 1PPS gate)

**Cost:** ~$20
**Build Time:** 1-2 hours
**Difficulty:** Easy

### Circuit Diagram

```
GPS Module NEO-6M/7M/8M
┌─────────────────────┐
│  VCC  GND  TX  RX  PPS │
└──┬────┬────────────┬──┘
   │    │            │
   │    │            │ GPS 1PPS (gate signal)
   5V  GND           │
   │    │            │
   │    │            └────────────→ Arduino D2 (INT0)
   │    │
   │    │
   │    └─────────────────────────→ Arduino GND
   │
   └──────────────────────────────→ Arduino 5V

TinySA 30MHz Output
   │
   │  (from test point or buffer)
   │
   └──────────────────────────────→ Arduino D5 (T1 - Timer1 external clock)


Optional: LED Indicator

Arduino D13 ──→ 330Ω ──→ LED+ ──→ LED- ──→ GND
                             (blinks on measurement)

Optional: OLED Display

Arduino A4 (SDA) ──→ OLED SDA
Arduino A5 (SCL) ──→ OLED SCL
Arduino VCC ──→ OLED VCC
Arduino GND ──→ OLED GND
```

### Parts List

| Qty | Part | Specs | Cost | Source |
|-----|------|-------|------|--------|
| 1 | GPS Module | NEO-6M/7M/8M with 1PPS | $10-20 | eBay, Amazon |
| 1 | Arduino Nano | ATmega328P | $10 | Amazon, AliExpress |
| 1 | Breadboard | 400 or 830 tie-points | $3 | Local electronics |
| 1 | LED | Red, 5mm | $0.10 | Local electronics |
| 1 | Resistor | 330Ω, 1/4W | $0.10 | Local electronics |
| 10 | Jumper wires | M-M and M-F | $2 | Local electronics |
| 1 | USB cable | Mini USB | $2 | Local electronics |
| 1 | OLED display (opt) | 0.96" 128x64 I2C | $5 | Amazon, eBay |

**Total:** ~$20-25

### Build Steps

#### Step 1: Prepare GPS Module (15 min)

1. **Check for pre-soldered headers:**
   - If yes: Skip to Step 2
   - If no: Solder male headers to GPS module

2. **Identify pins:**
   - VCC (sometimes labeled 3.3V or 5V)
   - GND
   - TX (transmit data)
   - RX (receive data)
   - PPS or 1PPS (pulse per second output)

3. **Note voltage requirement:**
   - Some modules are 3.3V only
   - Most have onboard regulator for 5V
   - Check your module's datasheet

#### Step 2: Prepare Arduino (10 min)

1. **Install Arduino IDE** on computer (if not already installed)
   - Download from arduino.cc
   - Install USB drivers

2. **Upload test sketch:**
   ```cpp
   void setup() {
     pinMode(LED_BUILTIN, OUTPUT);
   }
   void loop() {
     digitalWrite(LED_BUILTIN, HIGH);
     delay(500);
     digitalWrite(LED_BUILTIN, LOW);
     delay(500);
   }
   ```

3. **Verify Arduino works** (LED should blink)

#### Step 3: Connect GPS Module (10 min)

**Breadboard Layout:**

```
Breadboard Rails:
Top +  ──→ Arduino 5V
Top -  ──→ Arduino GND

GPS Module:
VCC ──→ Top + rail (5V)
GND ──→ Top - rail (GND)
PPS ──→ Arduino D2 directly
TX  ──→ (leave unconnected for basic frequency counter)
RX  ──→ (leave unconnected for basic frequency counter)
```

**Important:** Some GPS modules have 3.3V logic on PPS output. Arduino is 5V tolerant, so direct connection is safe. If your module has open-drain PPS, add 10kΩ pullup to 5V.

#### Step 4: Add LED Indicator (5 min)

```
Arduino D13 ──→ 330Ω resistor ──→ LED long leg (+)
LED short leg (-) ──→ GND rail
```

LED will blink each time measurement is taken.

#### Step 5: Connect TinySA 30MHz Signal (20 min)

**Option A: If TinySA has test point**

1. **Locate 30MHz test point** on TinySA PCB
   - Look for "TCXO", "30M", "REF", or "XTAL" label
   - May need magnifying glass

2. **Solder wire** to test point:
   - Use thin wire (30 AWG wrapping wire)
   - Short solder time (avoid heat damage)
   - Length: 6-12 inches

3. **Add series capacitor** (optional but recommended):
   - 0.1µF ceramic capacitor
   - AC couples signal, removes DC offset
   - Protects Arduino input

4. **Connect:**
   ```
   TinySA 30MHz ──→ 0.1µF cap ──→ Arduino D5 (T1)
   TinySA GND ──→ Arduino GND
   ```

**Option B: If using external buffer**

1. **Build simple buffer:**
   ```
   TinySA 30MHz ──→ 74HC04 inverter input
                    74HC04 output ──→ Arduino D5
                    74HC04 VCC ──→ 5V
                    74HC04 GND ──→ GND
   ```

2. **Advantage:** Isolates TinySA from Arduino

**Option C: If no 30MHz access**

Build oscillator with crystal to test:
```
30MHz crystal ──→ 74HC04 oscillator circuit ──→ Arduino D5
```
(See separate schematic below)

#### Step 6: Upload Frequency Counter Code (10 min)

**Code:** (From gps_calibration.md, repeated here for convenience)

```cpp
/*
 * GPS-Based Frequency Counter for TinySA Calibration
 * Measures frequency on D5 using GPS 1PPS gate on D2
 */

#define PPS_PIN 2        // GPS 1PPS input
#define LED_PIN 13       // Measurement indicator
#define INPUT_PIN 5      // T1 - Timer1 external clock input

volatile unsigned long count = 0;
volatile bool measureComplete = false;
unsigned long measurements[10];
int measurementIndex = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PPS_PIN, INPUT);
  pinMode(INPUT_PIN, INPUT);

  // Configure Timer1 for external counting on T1 pin (D5)
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  // External clock on T1, rising edge
  TCCR1B = (1 << CS12) | (1 << CS11) | (1 << CS10);

  // Attach interrupt to GPS 1PPS
  attachInterrupt(digitalPinToInterrupt(PPS_PIN), pps_interrupt, RISING);

  Serial.println("GPS Frequency Counter");
  Serial.println("====================");
  Serial.println("Waiting for GPS lock (1PPS signal)...");
  Serial.println();
}

void loop() {
  if (measureComplete) {
    measureComplete = false;

    // Store measurement
    measurements[measurementIndex] = count;
    measurementIndex++;

    // Calculate statistics
    double frequency = count;
    double expected = 30000000.0;
    double error_hz = frequency - expected;
    double error_ppm = (error_hz / expected) * 1e6;

    // Display
    Serial.print("Measurement #");
    Serial.println(measurementIndex);
    Serial.print("Frequency: ");
    Serial.print(frequency, 0);
    Serial.println(" Hz");
    Serial.print("Error:     ");
    if (error_hz >= 0) Serial.print("+");
    Serial.print(error_hz, 1);
    Serial.print(" Hz (");
    if (error_ppm >= 0) Serial.print("+");
    Serial.print(error_ppm, 3);
    Serial.println(" ppm)");

    // Calculate average if we have multiple measurements
    if (measurementIndex >= 5) {
      unsigned long sum = 0;
      for (int i = 0; i < measurementIndex; i++) {
        sum += measurements[i];
      }
      double avg = (double)sum / measurementIndex;
      double avg_error = (avg - expected) / expected * 1e6;

      Serial.print("Average (");
      Serial.print(measurementIndex);
      Serial.print(" samples): ");
      Serial.print(avg, 1);
      Serial.print(" Hz (");
      if (avg_error >= 0) Serial.print("+");
      Serial.print(avg_error, 3);
      Serial.println(" ppm)");

      // Reset after 10 measurements
      if (measurementIndex >= 10) {
        measurementIndex = 0;
        Serial.println("\n--- Starting new average ---\n");
      }
    }

    Serial.println("------------------------------");
    Serial.println();
  }

  delay(100);
}

void pps_interrupt() {
  static bool gate_open = false;

  if (!gate_open) {
    // First PPS: Start counting
    TCNT1 = 0;  // Reset counter
    gate_open = true;
    digitalWrite(LED_PIN, HIGH);
  } else {
    // Second PPS: Stop counting
    count = TCNT1;  // Read count
    gate_open = false;
    measureComplete = true;
    digitalWrite(LED_PIN, LOW);
  }
}
```

#### Step 7: Test and Calibrate (30 min)

1. **Power on GPS module**
   - Red LED should be on (power)
   - Wait 30-120 seconds for GPS lock
   - Blue LED should start blinking (GPS fix)

2. **Open Serial Monitor**
   - Arduino IDE → Tools → Serial Monitor
   - Set baud rate: 115200

3. **Watch for measurements:**
   ```
   GPS Frequency Counter
   ====================
   Waiting for GPS lock (1PPS signal)...

   Measurement #1
   Frequency: 30000150 Hz
   Error:     +150.0 Hz (+5.000 ppm)
   ------------------------------
   ```

4. **Let it run** for 10+ measurements

5. **Note average PPM error**

6. **Apply to TinySA:**
   - CONFIG → Reference
   - Enter correction (opposite sign)
   - SAVE

### Troubleshooting

**No GPS lock (no measurements):**
- Move GPS near window
- Check power connections
- Wait longer (cold start: 2-15 min)
- Verify antenna is connected

**Count shows 0 or very low:**
- Check 30MHz signal connection
- Verify D5 (T1) connection
- Check ground between TinySA and Arduino
- Measure 30MHz with oscilloscope (should be 0.5-3V)

**Erratic measurements:**
- Check for loose connections
- Add 0.1µF decoupling caps to power rails
- Shield 30MHz wire from interference
- Verify GPS has good signal (4+ satellites)

**Measurements don't match expected:**
- This is normal! That's the error we're measuring
- If >100 ppm, check connections
- If negative values, swap signal polarity

---

## Project 2: 74HC390 Divide-by-N Counter

### Overview

Divide 30MHz by 30,000 to get 1000 Hz, easier to measure with audio equipment.

**Cost:** ~$5
**Build Time:** 30 minutes
**Difficulty:** Medium

### Circuit Diagram

```
30MHz Input
   │
   ├──→ 0.1µF cap ──→ 74HC390 (1A input)
   │                  Divide by 10
   │                  QA output ──→ 74HC390 (1B input)
   │                               Divide by 10
   │                               QB output (3 MHz)
   │                                  │
   └──────────────────────────────────┴──→ 74HC390 (2A input)
                                           Divide by 10
                                           QA output ──→ 74HC390 (2B input)
                                                        Divide by 10
                                                        QB output (30 kHz)
                                                           │
                                                           └──→ CD4024 (CLK)
                                                                Divide by 32
                                                                Q5 output (937.5 Hz)

Or better: Use 74HC4040 12-bit counter
30MHz ──→ 74HC4040 ──→ Q15 output = 30MHz / 32768 = 915.5 Hz
```

### Simplified: Using Single IC

**Best Option: 74HC4040 (12-stage binary counter)**

```
                          74HC4040
                    ┌──────────────────┐
 30MHz Input ──→ CLK│1              16│VCC ──→ 5V
                    │                  │
            GND ──→ │2              15│Q12
                    │                  │
                    │3 Q1           14│Q6
                    │                  │
                    │4 Q2           13│Q5
                    │                  │
                    │5 Q3           12│Q7
                    │                  │
                    │6 Q4           11│Q4
                    │                  │
                    │7 MR (reset)   10│Q9
                    │   tie LOW        │
                    │8 GND           9│Q11 ──→ Output
                    └──────────────────┘
                                        (30MHz / 2048 = 14.65 kHz)

Q12 output = 30MHz / 4096 = 7.32 kHz (good for audio)
Q13 output = 30MHz / 8192 = 3.66 kHz (good for audio)
```

### Parts List

| Qty | Part | Cost |
|-----|------|------|
| 1 | 74HC4040 | $0.50 |
| 2 | 0.1µF ceramic cap | $0.20 |
| 1 | Breadboard | $3 |
| 1 | Wire | $1 |

### Assembly

1. **Insert 74HC4040** on breadboard
2. **Connect power:**
   - Pin 16 (VCC) → 5V
   - Pin 8 (GND) → GND
   - 0.1µF cap across VCC and GND (close to IC)

3. **Connect input:**
   - Pin 10 (CLK) → 30MHz signal (via 0.1µF AC coupling cap)

4. **Connect reset:**
   - Pin 11 (MR) → GND (never reset)

5. **Select output:**
   - Pin 2 (Q12) → Output (7.32 kHz)
   - Or Pin 1 (Q13) → Output (3.66 kHz)

6. **Measure output** with frequency counter, oscilloscope, or audio interface

---

## Project 3: Temperature Logging System

### Overview

Monitor TinySA crystal temperature and frequency simultaneously to characterize temperature coefficient.

**Cost:** ~$15
**Build Time:** 1 hour
**Difficulty:** Medium

### Circuit Diagram

```
DS18B20 Digital Temperature Sensor
┌───────────────────┐
│ GND  DQ  VCC      │
└──┬────┬────┬──────┘
   │    │    │
  GND   │   5V
        │
        ├──→ 4.7kΩ pullup ──→ 5V
        │
        └──→ Arduino D3 (1-Wire data)


Optional: Analog thermistor (cheaper)
┌─ 5V
│
├──→ 10kΩ NTC thermistor
│         │
│         ├──→ Arduino A0 (voltage divider midpoint)
│         │
│         └──→ 10kΩ resistor
│               │
└─ GND ─────────┘

Mount thermistor directly on TinySA case near crystal
```

### Parts List

| Qty | Part | Cost |
|-----|------|------|
| 1 | DS18B20 temp sensor | $2 |
| 1 | 4.7kΩ resistor | $0.10 |
| 1 | Arduino Nano | $10 (reuse from Project 1) |
| 1 | Thermal paste | $3 (optional) |

### Arduino Code

```cpp
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 3  // DS18B20 on D3
#define PPS_PIN 2       // GPS 1PPS
#define FREQ_PIN 5      // 30MHz input

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

volatile unsigned long freq_count = 0;
volatile bool measureComplete = false;

void setup() {
  Serial.begin(115200);
  pinMode(PPS_PIN, INPUT);
  pinMode(FREQ_PIN, INPUT);

  // Start temperature sensor
  sensors.begin();

  // Configure Timer1 for frequency counting
  TCCR1A = 0;
  TCCR1B = (1 << CS12) | (1 << CS11) | (1 << CS10);
  TCNT1 = 0;

  attachInterrupt(digitalPinToInterrupt(PPS_PIN), pps_interrupt, RISING);

  Serial.println("Timestamp,Temperature_C,Frequency_Hz,Error_ppm");
}

void loop() {
  if (measureComplete) {
    measureComplete = false;

    // Read temperature
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0);

    // Calculate frequency error
    double frequency = freq_count;
    double expected = 30000000.0;
    double error_ppm = (frequency - expected) / expected * 1e6;

    // Log to serial (CSV format)
    unsigned long timestamp = millis() / 1000;
    Serial.print(timestamp);
    Serial.print(",");
    Serial.print(tempC, 2);
    Serial.print(",");
    Serial.print(frequency, 0);
    Serial.print(",");
    Serial.println(error_ppm, 3);
  }

  delay(100);
}

void pps_interrupt() {
  static bool gate = false;
  if (!gate) {
    TCNT1 = 0;
    gate = true;
  } else {
    freq_count = TCNT1;
    gate = false;
    measureComplete = true;
  }
}
```

### Temperature Testing Procedure

1. **Attach sensor to TinySA:**
   - Use thermal paste
   - Position near crystal (usually under metal shield)
   - Secure with tape

2. **Cool down:**
   - Place TinySA in refrigerator
   - Wait 30 minutes
   - Temperature: ~5°C

3. **Warm up naturally:**
   - Remove from fridge
   - Place in room
   - Log data for 2-3 hours
   - Temperature: 5°C → 20°C

4. **Heat up:**
   - Use hair dryer
   - Gentle heating
   - Log data
   - Temperature: 20°C → 40°C

5. **Plot results:**
   - Temperature (X-axis) vs. PPM error (Y-axis)
   - Calculate slope = ppm/°C
   - This is your temperature coefficient

---

## Schematics Directory

Let me create the schematics folder with ASCII circuit diagrams.

### Additional Circuits

**Simple 30MHz Crystal Oscillator (for testing):**

```
                        +5V
                         │
                         ├──→ 10kΩ
                         │
        ┌────────────────┼────────────────┐
        │                │                │
    ┌───┴───┐        ┌───┴───┐       ┌───┴───┐
    │       │        │       │       │       │
    │ 74HC04│        │ 74HC04│       │ 74HC04│
    │  INV  │        │  INV  │       │  INV  │
    │       │        │       │       │       │
    └───┬───┘        └───┬───┘       └───┬───┘
        │                │                │
        │     30MHz      │                │
        └────XTAL────────┘                │
        │                                 │
        ├──→ 22pF cap                     │
        │         │                       │
       GND        GND                     └──→ Output

        └──→ 22pF cap
                 │
                GND

Notes:
- Use 74HC04 hex inverter
- First three inverters form oscillator
- 30MHz crystal (parallel resonance)
- 22pF load capacitors
- 1MΩ feedback resistor
- Output buffered by additional inverters
```

---

## Assembly Tips

### Soldering Best Practices

1. **Clean tip** before each joint
2. **Tin tip** with fresh solder
3. **Heat pad and lead** together (2-3 seconds)
4. **Apply solder** to joint, not tip
5. **Remove solder**, then **remove iron**
6. **Don't move** joint for 2-3 seconds

### Breadboard Layout

**Good practices:**
- Keep power rails continuous
- Use both breadboard sides (mirror layout)
- Keep signal wires short
- Separate analog and digital grounds (join at one point)
- Add 0.1µF bypass caps near each IC

**Avoid:**
- Long jumper wires (increases noise)
- Crossing wires (hard to debug)
- Missing ground connections
- Sharing power between noisy and quiet circuits

### Debugging Checklist

Before applying power:
- [ ] Check all power connections (VCC, GND)
- [ ] Verify polarity (especially ICs, LEDs, capacitors)
- [ ] Look for shorts between power rails
- [ ] Verify IC orientation (notch/dot = pin 1)

After applying power:
- [ ] Check voltage at each IC (should be 5V ±0.5V)
- [ ] Feel ICs for heat (shouldn't be hot)
- [ ] Verify clocks are running (use LED or scope)
- [ ] Check logic levels with multimeter

---

## Summary

### Projects Built

✓ GPS Frequency Counter - Count 30MHz with 1PPS gate
✓ Frequency Divider - Divide 30MHz to audio range
✓ Temperature Logger - Characterize temperature coefficient

### Skills Learned

- GPS module interfacing
- Timer/counter configuration
- Frequency measurement techniques
- Temperature sensing
- Circuit construction
- Debugging procedures

### Next Steps

1. Build GPS frequency counter (easiest, most useful)
2. Test with TinySA 30MHz signal
3. Log measurements for 24 hours
4. Optionally: Add temperature logging
5. Characterize your TinySA's performance

---

**Ready to build? Start with the GPS Frequency Counter!**
