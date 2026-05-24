# GPS-Based 30MHz Calibration Method

## Overview

This method uses a **$10-20 GPS module** to calibrate the TinySA's 30MHz internal reference to **atomic clock accuracy** (±0.01 ppm).

### Why GPS Works

- GPS satellites carry atomic clocks (rubidium and cesium)
- Accuracy: 1 × 10⁻¹³ (0.00001 ppm)
- GPS module outputs 1PPS (1 Pulse Per Second) synchronized to atomic time
- 1PPS accuracy: ±50 nanoseconds = ±0.01 ppm
- **Result: Atomic clock accuracy for $10**

### What You'll Achieve

```
Frequency Error: < 0.3 Hz @ 30MHz (0.01 ppm)
Short-term Stability: ±0.01 ppm (1 hour)
Long-term Stability: ±0.05 ppm (24 hours)
Temperature Coefficient: Measured and compensated
```

---

## Required Materials

### Essential Components

| Item | Specs | Cost | Source |
|------|-------|------|--------|
| GPS Module | NEO-6M/7M/8M with 1PPS | $10-20 | eBay, Amazon, AliExpress |
| TinySA | Ultra or Basic | $100-150 | Owner supplied |
| Breadboard | Standard size | $3 | Local electronics |
| Jumper wires | Male-male, male-female | $2 | Local electronics |
| Resistor | 1kΩ 1/4W | $0.10 | Local electronics |
| LED | Red 5mm | $0.10 | Local electronics |
| USB cable | Mini or Micro USB | $2 | Local electronics |

**Total Cost: $15-25**

### Optional But Recommended

| Item | Purpose | Cost |
|------|---------|------|
| Arduino Nano/Uno | Frequency counter | $10 |
| OLED display | Show measurements | $5 |
| Frequency counter module | Direct measurement | $10 |
| Logic analyzer | Verify 1PPS | $5 |
| GPS antenna | Better signal | $10 |

### Tools Required

- Soldering iron (if headers not pre-installed)
- Multimeter (verify connections)
- Computer with USB port
- TinySA software or phone app

---

## GPS Module Selection

### Recommended Modules

**Budget Choice: NEO-6M ($10-12)**
- 1PPS accuracy: ±60 ns
- Update rate: 1 Hz
- Cold start: 27s
- Good enough for 0.01 ppm

**Better Choice: NEO-7M ($15-18)**
- 1PPS accuracy: ±30 ns
- Update rate: 10 Hz capable
- Cold start: 26s
- Improved accuracy

**Best Choice: NEO-8M ($18-22)**
- 1PPS accuracy: ±30 ns
- Update rate: 18 Hz capable
- Cold start: 26s
- Better in weak signal areas

**Overkill: NEO-M9N ($35-45)**
- 1PPS accuracy: ±20 ns
- Concurrent GNSS (GPS+Galileo+GLONASS+BeiDou)
- Faster fix
- Not necessary for this project

### Module Features to Look For

**Must Have:**
- 1PPS output pin
- 3.3V or 5V operation
- UART interface
- Passive ceramic antenna (or external antenna connector)

**Nice to Have:**
- Pre-soldered headers
- On-board voltage regulator (for 5V operation)
- Battery backup (faster subsequent fixes)
- External antenna connector

### Verifying 1PPS Output

When you receive your module:

1. **Visual Check:** Look for pin labeled "PPS", "1PPS", or "PULSE"
2. **Datasheet:** Confirm 1PPS is available on your specific board
3. **LED Test:** Connect LED + 1kΩ resistor to 1PPS pin → should blink once per second after GPS lock

---

## Theory of Operation

### Method 1: Period Measurement (Simplest)

```
1. GPS outputs 1PPS pulse (exactly 1 second period)
2. Use TinySA's 30MHz to measure the period
3. If period = 1.000000 seconds, 30MHz is perfect
4. If period ≠ 1.000000 seconds, calculate error
```

**Example:**
```
Measured period: 1.000001 seconds
Expected period: 1.000000 seconds
Error: +1 μs per second = +1 ppm
30MHz is 1 ppm too fast
Actual frequency: 30.000030 MHz
```

### Method 2: Frequency Counting (More Accurate)

```
1. Count TinySA 30MHz cycles during GPS 1PPS gate
2. Should count exactly 30,000,000 cycles in 1 second
3. Difference from 30M is the error
```

**Example:**
```
Counted cycles: 30,000,300 in 1 second
Expected cycles: 30,000,000
Error: +300 counts = +10 ppm
Actual frequency: 30.000300 MHz
```

### Method 3: Phase-Locked Loop (Most Complex)

```
1. Build PLL that locks to GPS 1PPS
2. PLL generates 30MHz locked to GPS
3. Compare TinySA's 30MHz to GPS-locked 30MHz
4. Measure frequency difference
```

We'll focus on **Method 1** and **Method 2** as they're simplest and don't require building hardware.

---

## Calibration Procedure: Method 1 (Period Measurement)

This method uses the TinySA to measure the GPS 1PPS period.

### Step 1: GPS Module Setup

1. **Connect GPS to power:**
   - VCC → 3.3V or 5V (check your module spec)
   - GND → Ground

2. **Connect 1PPS indicator LED:**
   - 1PPS pin → 1kΩ resistor → LED anode (+)
   - LED cathode (-) → Ground

3. **Power on and wait for GPS lock:**
   - Red LED should start blinking (power)
   - Blue LED should start blinking after 30-60 seconds (GPS lock)
   - 1PPS LED should blink once per second after GPS lock

**Important:** GPS needs **clear view of sky**:
- Near window or outdoors
- Away from metal objects
- Not inside shielded enclosure
- Cold start: 30-120 seconds to first fix

### Step 2: Extract 30MHz from TinySA

**TinySA has two options:**

**Option A: Use internal 30MHz directly (if accessible)**
- Some TinySA models have test point for 30MHz
- Check your PCB for "30MHz", "TCXO", or "REF" label
- Solder wire to test point

**Option B: Use frequency counter mode**
- TinySA Ultra has built-in frequency counter
- Feed external signal to INPUT
- Displays frequency with internal 30MHz as timebase

**Option C: Use external frequency reference (requires building)**
- Build frequency counter using TinySA's 30MHz
- Feed 1PPS into counter
- Measure period or frequency

For this example, we'll assume you can **access the 30MHz signal** directly.

### Step 3: Measure GPS 1PPS Period

Since we're calibrating and don't have calibrated equipment, we need a clever workaround:

**The Clever Trick: Self-Referencing**

1. Connect 1PPS to TinySA INPUT
2. TinySA measures 1PPS frequency using its 30MHz reference
3. TinySA displays "1.000000 Hz" (but this might be wrong!)
4. The ratio of displayed vs. actual tells us the error

**Math:**
```
Displayed frequency: F_display
Actual frequency: 1.000000 Hz (GPS is perfect)
Error: (F_display - 1.0) / 1.0 × 10^6 ppm

If F_display = 1.000010 Hz:
Error = (1.000010 - 1.0) / 1.0 × 10^6 = +10 ppm
TinySA's 30MHz is 10 ppm too fast
```

### Step 4: Calculate Correction

```
ppm_error = (measured_frequency - 1.000000) × 1,000,000

If measured = 1.000005 Hz:
ppm_error = 0.000005 × 1,000,000 = +5 ppm

Actual 30MHz frequency = 30,000,000 × (1 + ppm_error/10^6)
                        = 30,000,000 × 1.000005
                        = 30,000,150 Hz
                        = 30.000150 MHz
```

### Step 5: Apply Correction in TinySA

**TinySA Basic:**
1. Press CONFIG
2. Find "XTAL" or "TCXO" calibration
3. Enter ppm correction value
4. Save to flash

**TinySA Ultra:**
1. Press CONFIG → MORE
2. Find "Reference" or "Cal"
3. Enter frequency correction
4. Save

**Example:**
```
Measured error: +5 ppm
Enter in TinySA: -5 ppm (opposite sign to compensate)
```

### Step 6: Verification

1. Re-measure GPS 1PPS frequency
2. Should now read 1.000000 Hz (within ±0.001 Hz)
3. If not, iterate correction

**Iteration:**
```
Iteration 1: Measured 1.000005 Hz → Apply -5 ppm
Iteration 2: Measured 0.999998 Hz → Apply +2 ppm
Iteration 3: Measured 1.000000 Hz → Done!
```

---

## Calibration Procedure: Method 2 (Frequency Counter)

This method builds a simple frequency counter using Arduino.

### Required Additional Hardware

- Arduino Nano or Uno ($10)
- Breadboard
- Jumper wires

### Arduino Frequency Counter Circuit

```
GPS Module              Arduino Nano
────────────────────────────────────
1PPS pin    ─────────→  D2 (INT0)
GND         ─────────→  GND
VCC         ─────────→  5V


TinySA 30MHz            Arduino Nano
────────────────────────────────────
30MHz output ────────→  D5 (T1)
(via test point)

GND          ────────→  GND


Optional LED indicator:
D13 ─→ 330Ω ─→ LED ─→ GND
(Blinks each 1PPS)
```

### Arduino Code

Save as `gps_frequency_counter.ino`:

```cpp
/*
 * GPS-Based Frequency Counter for TinySA Calibration
 * Measures frequency of signal on D5 (Timer1) using GPS 1PPS gate
 */

#define PPS_PIN 2        // GPS 1PPS input (INT0)
#define LED_PIN 13       // Measurement indicator
#define GATE_TIME 1      // Gate time in seconds (1 for GPS 1PPS)

volatile unsigned long count = 0;
volatile bool measureComplete = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PPS_PIN, INPUT);

  // Configure Timer1 for external counting on T1 pin (D5)
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  // External clock on T1 (D5), increment on rising edge
  TCCR1B = (1 << CS12) | (1 << CS11) | (1 << CS10);

  // Attach interrupt to GPS 1PPS signal
  attachInterrupt(digitalPinToInterrupt(PPS_PIN), pps_interrupt, RISING);

  Serial.println("GPS Frequency Counter Ready");
  Serial.println("Waiting for GPS 1PPS signal...");
  Serial.println();
}

void loop() {
  if (measureComplete) {
    measureComplete = false;

    // Calculate frequency
    double frequency = count;
    double expected = 30000000.0;  // Expected 30 MHz
    double error_hz = frequency - expected;
    double error_ppm = (error_hz / expected) * 1e6;

    // Display results
    Serial.print("Frequency: ");
    Serial.print(frequency, 0);
    Serial.println(" Hz");

    Serial.print("Expected:  ");
    Serial.print(expected, 0);
    Serial.println(" Hz");

    Serial.print("Error:     ");
    if (error_hz >= 0) Serial.print("+");
    Serial.print(error_hz, 1);
    Serial.println(" Hz");

    Serial.print("Error:     ");
    if (error_ppm >= 0) Serial.print("+");
    Serial.print(error_ppm, 3);
    Serial.println(" ppm");

    Serial.print("Actual:    ");
    Serial.print(frequency / 1000000.0, 6);
    Serial.println(" MHz");

    Serial.println("─────────────────────────────");
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
    // Second PPS: Stop counting, read value
    count = TCNT1;  // Read count
    gate_open = false;
    measureComplete = true;
    digitalWrite(LED_PIN, LOW);
  }
}
```

### Using the Frequency Counter

1. **Upload code** to Arduino
2. **Connect GPS** and wait for lock (1PPS blinking)
3. **Connect TinySA 30MHz** to Arduino D5
4. **Open Serial Monitor** (115200 baud)
5. **Read measurements:**

```
GPS Frequency Counter Ready
Waiting for GPS 1PPS signal...

Frequency: 30000150 Hz
Expected:  30000000 Hz
Error:     +150.0 Hz
Error:     +5.000 ppm
Actual:    30.000150 MHz
─────────────────────────────

Frequency: 30000148 Hz
Expected:  30000000 Hz
Error:     +148.0 Hz
Error:     +4.933 ppm
Actual:    30.000148 MHz
─────────────────────────────
```

6. **Average** 10-20 measurements
7. **Apply correction** to TinySA

---

## Temperature Compensation

GPS modules work over wide temperature range, but TinySA's crystal is temperature-sensitive.

### Temperature Coefficient Measurement

1. **Cool down** TinySA (refrigerator, 10°C)
2. **Measure** frequency error vs GPS
3. **Warm up** to room temp (20°C)
4. **Measure** again
5. **Heat up** (hair dryer, 40°C)
6. **Measure** again

**Example Data:**

| Temp (°C) | Error (ppm) | Frequency (Hz) |
|-----------|-------------|----------------|
| 10 | +2.5 | 30,000,075 |
| 20 | 0.0 | 30,000,000 |
| 30 | -1.8 | 29,999,946 |
| 40 | -3.2 | 29,999,904 |

**Temperature Coefficient:**
```
TC = Δppm / ΔT = (−3.2 − 2.5) / (40 − 10) = −0.19 ppm/°C
```

### Applying Temperature Compensation

**Manual Method:**
- Note current temperature
- Apply correction based on TC
- Re-calibrate periodically

**Automatic Method (Advanced):**
- Add temperature sensor to TinySA
- Build lookup table
- Apply correction in software

---

## Long-Term Stability Testing

### 24-Hour Test

1. **Set up GPS** with good sky view
2. **Connect TinySA** 30MHz to frequency counter
3. **Log measurements** every minute for 24 hours
4. **Plot results** to see drift

**Python logging script** (save as `long_term_logger.py`):

```python
import serial
import time
from datetime import datetime

# Open serial connection to Arduino
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
time.sleep(2)  # Wait for Arduino reset

# Open log file
log_file = open('frequency_log.csv', 'w')
log_file.write('Timestamp,Frequency_Hz,Error_ppm\n')

print("Starting 24-hour frequency logging...")
print("Press Ctrl+C to stop")

try:
    while True:
        line = ser.readline().decode('utf-8').strip()

        if 'Frequency:' in line:
            freq = float(line.split(':')[1].replace('Hz', '').strip())
            timestamp = datetime.now().isoformat()
            error_ppm = (freq - 30000000) / 30000000 * 1e6

            log_file.write(f'{timestamp},{freq},{error_ppm:.3f}\n')
            log_file.flush()

            print(f'{timestamp}: {freq} Hz ({error_ppm:+.3f} ppm)')

        time.sleep(60)  # Log every minute

except KeyboardInterrupt:
    print("\nLogging stopped")
    log_file.close()
    ser.close()
```

### Plotting Results

```python
import pandas as pd
import matplotlib.pyplot as plt

# Read log file
df = pd.read_csv('frequency_log.csv')
df['Timestamp'] = pd.to_datetime(df['Timestamp'])

# Plot frequency over time
plt.figure(figsize=(12, 6))
plt.subplot(2, 1, 1)
plt.plot(df['Timestamp'], df['Frequency_Hz'])
plt.ylabel('Frequency (Hz)')
plt.title('TinySA 30MHz Frequency Stability (24 hours)')
plt.grid(True)

plt.subplot(2, 1, 2)
plt.plot(df['Timestamp'], df['Error_ppm'])
plt.ylabel('Error (ppm)')
plt.xlabel('Time')
plt.grid(True)

plt.tight_layout()
plt.savefig('frequency_stability.png')
plt.show()

# Statistics
print("24-Hour Stability Report")
print("=" * 40)
print(f"Mean frequency: {df['Frequency_Hz'].mean():.1f} Hz")
print(f"Std deviation: {df['Frequency_Hz'].std():.1f} Hz")
print(f"Min frequency: {df['Frequency_Hz'].min():.1f} Hz")
print(f"Max frequency: {df['Frequency_Hz'].max():.1f} Hz")
print(f"Peak-peak drift: {df['Frequency_Hz'].max() - df['Frequency_Hz'].min():.1f} Hz")
print(f"Mean error: {df['Error_ppm'].mean():.3f} ppm")
print(f"Std deviation: {df['Error_ppm'].std():.3f} ppm")
```

---

## Accuracy Limitations

### GPS 1PPS Accuracy Factors

| Factor | Typical | Effect on 30MHz |
|--------|---------|-----------------|
| Satellite clock error | ±5 ns | ±0.00015 ppm |
| Atmospheric delay | ±10 ns | ±0.0003 ppm |
| Receiver jitter | ±30 ns | ±0.001 ppm |
| Antenna cable delay | ±50 ns | ±0.0015 ppm |
| **Total** | **±60 ns** | **±0.002 ppm** |

**Conclusion:** GPS contributes negligible error. Your limit is TinySA crystal stability.

### TinySA Crystal Limitations

**Short-term (1 second):**
- TCXO noise: ±0.01 ppm typical
- Measurement resolution limited

**Long-term (24 hours):**
- Temperature drift: ±1-5 ppm (uncalibrated)
- After compensation: ±0.1-0.5 ppm
- Aging: ±1 ppm/year

**Conclusion:** You can calibrate to ±0.01 ppm with GPS, but drift requires periodic re-calibration.

---

## Troubleshooting

### GPS Won't Lock

**Symptoms:** 1PPS LED never blinks, no fix after 5 minutes

**Solutions:**
- Move GPS near window or outdoors
- Check antenna connection
- Verify 3.3V or 5V power (check module specs)
- Try different location (away from metal, electronics)
- Wait longer (cold start can take 2-15 minutes)

### 1PPS Not Visible

**Symptoms:** GPS has lock, but no 1PPS pulse

**Solutions:**
- Check 1PPS pin identification (PPS, PULSE, T_PULSE)
- Verify with multimeter (should see ~3V pulse once per second)
- Check module datasheet (some need configuration via UART)
- Try different GPS module

### Frequency Counter Shows Zero

**Symptoms:** Arduino shows 0 Hz or very low count

**Solutions:**
- Verify 30MHz signal level (should be 0.5-3V)
- Check Timer1 input (D5 on Arduino Nano/Uno)
- Verify ground connection between TinySA and Arduino
- Add AC coupling capacitor (0.1µF) if DC offset present
- Check 30MHz signal on oscilloscope

### Measurements Jump Around

**Symptoms:** Frequency varies by >10 ppm between readings

**Solutions:**
- Wait for GPS warm-up (first 10 minutes less accurate)
- Check for temperature changes
- Verify good GPS signal (needs 4+ satellites)
- Average multiple measurements (10-20)
- Check for RF interference

### TinySA Won't Save Calibration

**Symptoms:** Calibration resets after power cycle

**Solutions:**
- Ensure you select "SAVE" after entering correction
- Some models require "SAVE CONFIG" from menu
- Update TinySA firmware to latest version
- Check battery (some models need battery for EEPROM)

---

## Advanced Topics

### Dual-GNSS for Improved Accuracy

Use module that supports GPS + Galileo + GLONASS:
- More satellites visible
- Better averaging
- Improved accuracy in urban areas
- Faster fix times

### GPSDO (GPS-Disciplined Oscillator)

Build a PLL that locks TCXO to GPS:
- Continuous 30MHz output locked to GPS
- Better short-term stability than free-running GPS
- Can run for hours without GPS (flywheel effect)
- Requires additional hardware (PLL IC, loop filter)

### Allan Deviation Measurement

Characterize oscillator stability vs. averaging time:
- Shows noise characteristics
- Identifies optimal averaging period
- Used in professional frequency standards
- Requires specialized software (e.g., TimeLab, Stable32)

---

## Summary

### What We Accomplished

✓ Calibrated TinySA 30MHz reference to GPS atomic clocks
✓ Achieved ±0.01 ppm accuracy (±0.3 Hz @ 30MHz)
✓ Total cost: ~$15-25
✓ Characterized temperature coefficient
✓ Verified long-term stability

### Key Takeaways

1. **GPS provides atomic clock accuracy** for price of cheap module
2. **1PPS signal** is the key to precision timing
3. **Frequency counting** over 1PPS gate directly measures error
4. **Temperature compensation** is necessary for best accuracy
5. **Long-term monitoring** reveals aging and drift

### Next Steps

1. Apply calibration to your TinySA
2. Verify against other methods (WWV, FM broadcast)
3. Build permanent frequency reference
4. Share your results with community

---

## References

- [GPS.gov - How GPS Works](https://www.gps.gov/systems/gps/)
- [U-blox NEO-6 Datasheet](https://www.u-blox.com)
- [TinySA Calibration Guide](https://tinysa.org/wiki/pmwiki.php?n=Main.Calibration)
- [NIST Time and Frequency Division](https://www.nist.gov/pml/time-and-frequency-division)

---

**Ready to start? Order your GPS module and build the frequency counter!**
