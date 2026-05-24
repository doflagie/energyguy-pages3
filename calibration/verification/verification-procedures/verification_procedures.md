# Verification and Measurement Procedures

## Overview

After calibrating your TinySA using GPS, time standard broadcasts, or FM stations, you need to **verify** the calibration worked and **characterize** long-term performance.

This document provides:
1. Immediate verification procedures
2. Cross-checking against multiple references
3. Long-term stability testing
4. Temperature characterization
5. Uncertainty analysis

---

## Immediate Verification (Post-Calibration)

### Quick Check (5 minutes)

**Purpose:** Confirm calibration was applied correctly

**Procedure:**

1. **Re-measure calibration source:**
   - GPS 1PPS → Should read 1.000000 Hz (±0.001 Hz)
   - CHU 7.850 MHz → Should read 7,850,000 Hz (±10 Hz)
   - FM 100.1 MHz → Should read 100,100,000 Hz (±100 Hz)

2. **Check for improvement:**
   ```
   BEFORE calibration:
   CHU measured: 7,850,152 Hz
   Error: +152 Hz (+19.4 ppm)

   AFTER calibration:
   CHU measured: 7,850,003 Hz
   Error: +3 Hz (+0.38 ppm)
   ```

3. **Verify correction was saved:**
   - Power cycle TinySA
   - Re-measure
   - Should still be accurate

**Success Criteria:**
- ✓ Error reduced by >90%
- ✓ Final error < 1 ppm (< 30 Hz @ 30MHz)
- ✓ Calibration survives power cycle

---

## Cross-Reference Validation (30 minutes)

### Multiple Independent Sources

**Purpose:** Confirm calibration against different references

**Procedure:**

1. **GPS vs. WWV/CHU:**
   ```
   Method A (GPS):
   - Measured 30MHz error: +5.2 ppm
   - Applied correction: -5.2 ppm

   Method B (CHU 7.850 MHz):
   - Measure CHU frequency
   - Calculate ppm error
   - Should read ±0.5 ppm of GPS result
   ```

2. **GPS vs. FM Broadcast:**
   ```
   After GPS calibration, measure FM station:
   - Should read exact frequency (±1 ppm)
   - Example: 100.1 MHz → 100,100,000 Hz ±100 Hz
   ```

3. **Multiple FM Stations:**
   ```
   Measure 3-5 different FM stations
   All should read exact frequency
   If one is off, that station is poorly calibrated
   ```

### Acceptance Criteria

**All methods should agree within:**
- GPS vs. Time Standard: ±0.1-0.5 ppm
- GPS vs. FM Broadcast: ±1-2 ppm
- FM vs. FM: ±0.5 ppm (same market)

**Example Results (Good):**

| Method | Error (ppm) | Agreement |
|--------|-------------|-----------|
| GPS | +5.23 | Reference |
| CHU | +5.19 | ±0.04 ppm ✓ |
| FM 1 | +5.21 | ±0.02 ppm ✓ |
| FM 2 | +5.18 | ±0.05 ppm ✓ |

**Example Results (Problem):**

| Method | Error (ppm) | Agreement |
|--------|-------------|-----------|
| GPS | +5.23 | Reference |
| CHU | +5.22 | ±0.01 ppm ✓ |
| FM 1 | +8.45 | ±3.22 ppm ✗ Bad station |
| FM 2 | +5.19 | ±0.04 ppm ✓ |

---

## Short-Term Stability Test (1 hour)

### Allan Deviation (Simplified)

**Purpose:** Measure frequency stability over short time scales

**Equipment Needed:**
- GPS frequency counter (from construction guide)
- TinySA 30MHz signal
- Computer for logging

**Procedure:**

1. **Set up continuous measurement:**
   - GPS 1PPS gates TinySA 30MHz
   - Measure every second
   - Log to file

2. **Run for 1 hour (3600 measurements)**

3. **Calculate statistics:**
   ```python
   import numpy as np
   import pandas as pd

   # Load data
   data = pd.read_csv('frequency_log.csv')
   freq = data['Frequency_Hz'].values

   # Calculate Allan deviation for τ = 1s
   expected = 30000000
   y = (freq - expected) / expected  # Fractional frequency

   allan_1s = np.std(y)
   print(f"Allan deviation (1s): {allan_1s*1e6:.3f} ppm")

   # Calculate for τ = 10s (average groups of 10)
   y_10s = []
   for i in range(0, len(y)-10, 10):
       y_10s.append(np.mean(y[i:i+10]))
   allan_10s = np.std(y_10s) / np.sqrt(2)
   print(f"Allan deviation (10s): {allan_10s*1e6:.3f} ppm")
   ```

### Typical Results

**Good TCXO:**
```
Allan deviation (1s):  0.1-0.5 ppm
Allan deviation (10s): 0.01-0.1 ppm
Allan deviation (100s): 0.005-0.05 ppm
```

**Poor crystal:**
```
Allan deviation (1s):  1-5 ppm
Allan deviation (10s): 0.5-2 ppm
```

**Interpretation:**
- Lower = better
- Should improve with longer averaging
- If increases with time → drift problem
- If flat → hitting measurement noise floor

---

## Long-Term Stability Test (24 hours)

### 24-Hour Frequency Log

**Purpose:** Characterize drift, temperature effects, aging

**Setup:**

1. **Stable environment:**
   - Indoors, away from windows
   - Minimize temperature swings
   - Avoid moving equipment

2. **Continuous logging:**
   - Measure every 1-5 minutes
   - Log frequency, temperature
   - Save to CSV file

3. **Run for 24+ hours**

### Python Logging Script

```python
import serial
import time
from datetime import datetime

ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
time.sleep(2)

log_file = open('24hr_stability.csv', 'w')
log_file.write('Timestamp,Frequency_Hz,Error_ppm,Temperature_C\n')

print("24-Hour Stability Test Started")
print("Logging every 60 seconds")
print("Press Ctrl+C to stop\n")

try:
    while True:
        line = ser.readline().decode('utf-8').strip()

        if 'Frequency:' in line and 'Error:' in line:
            # Parse frequency and error from Arduino output
            # Example: "Frequency: 30000150 Hz Error: +5.000 ppm"

            parts = line.split()
            freq_hz = float(parts[1])
            error_ppm = float(parts[4])

            timestamp = datetime.now().isoformat()

            log_file.write(f'{timestamp},{freq_hz},{error_ppm},25.0\n')
            log_file.flush()

            print(f'{timestamp}: {freq_hz} Hz ({error_ppm:+.3f} ppm)')

        time.sleep(60)  # Log every minute

except KeyboardInterrupt:
    print("\n\nTest stopped by user")
    log_file.close()
    ser.close()

    print("Generating report...")

    # Generate summary
    import pandas as pd
    import matplotlib.pyplot as plt

    df = pd.read_csv('24hr_stability.csv')

    print("\n24-Hour Stability Report")
    print("=" * 50)
    print(f"Measurements: {len(df)}")
    print(f"Mean frequency: {df['Frequency_Hz'].mean():.1f} Hz")
    print(f"Std deviation: {df['Frequency_Hz'].std():.1f} Hz")
    print(f"Min frequency: {df['Frequency_Hz'].min():.1f} Hz")
    print(f"Max frequency: {df['Frequency_Hz'].max():.1f} Hz")
    print(f"Peak-peak drift: {df['Frequency_Hz'].max() - df['Frequency_Hz'].min():.1f} Hz")
    print(f"Mean error: {df['Error_ppm'].mean():.3f} ppm")
    print(f"Error std dev: {df['Error_ppm'].std():.3f} ppm")
    print("=" * 50)

    # Plot
    df['Timestamp'] = pd.to_datetime(df['Timestamp'])

    plt.figure(figsize=(12, 8))

    plt.subplot(2, 1, 1)
    plt.plot(df['Timestamp'], df['Frequency_Hz'])
    plt.ylabel('Frequency (Hz)')
    plt.title('TinySA 30MHz Stability - 24 Hours')
    plt.grid(True)

    plt.subplot(2, 1, 2)
    plt.plot(df['Timestamp'], df['Error_ppm'])
    plt.ylabel('Error (ppm)')
    plt.xlabel('Time')
    plt.grid(True)

    plt.tight_layout()
    plt.savefig('24hr_stability.png', dpi=150)
    print("\nPlot saved as: 24hr_stability.png")
```

### Interpreting Results

**Good Result:**
```
Peak-peak drift: < 30 Hz (< 1 ppm)
Standard deviation: < 10 Hz (< 0.3 ppm)
Trend: Flat or slow linear drift
Temperature sensitivity: < 0.5 ppm/°C
```

**Typical Result:**
```
Peak-peak drift: 30-150 Hz (1-5 ppm)
Standard deviation: 10-50 Hz (0.3-1.7 ppm)
Trend: Follows temperature
Temperature sensitivity: 0.5-2 ppm/°C
```

**Poor Result:**
```
Peak-peak drift: > 300 Hz (> 10 ppm)
Standard deviation: > 100 Hz (> 3 ppm)
Trend: Erratic or large jumps
→ Possible crystal problem or loose connection
```

---

## Temperature Characterization

### Temperature Coefficient Measurement

**Purpose:** Quantify frequency vs. temperature relationship

**Equipment:**
- Temperature sensor (DS18B20 or thermistor)
- GPS frequency counter
- Refrigerator and hair dryer

**Procedure:**

1. **Cold soak:**
   - Place TinySA in refrigerator
   - Wait 30 minutes
   - Measure frequency at 5°C

2. **Warm-up:**
   - Remove from fridge
   - Measure frequency every 2 minutes
   - Record temperature simultaneously
   - Until reaches room temp (20°C)

3. **Heat test:**
   - Use hair dryer (gentle, low heat)
   - Measure frequency every 2 minutes
   - Record temperature
   - Up to 40°C (don't overheat!)

4. **Cool-down:**
   - Turn off heat
   - Measure as it cools
   - Back to room temp

### Data Analysis

```python
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy import stats

# Load data
df = pd.read_csv('temperature_test.csv')

temp = df['Temperature_C'].values
error_ppm = df['Error_ppm'].values

# Linear fit
slope, intercept, r_value, p_value, std_err = stats.linregress(temp, error_ppm)

print("Temperature Coefficient Analysis")
print("=" * 50)
print(f"Temperature range: {temp.min():.1f} to {temp.max():.1f} °C")
print(f"Frequency range: {error_ppm.min():.2f} to {error_ppm.max():.2f} ppm")
print(f"Slope: {slope:.3f} ppm/°C")
print(f"R²: {r_value**2:.4f}")
print("=" * 50)

# Plot
plt.figure(figsize=(10, 6))
plt.scatter(temp, error_ppm, alpha=0.5, label='Measurements')
plt.plot(temp, slope*temp + intercept, 'r-', label=f'Fit: {slope:.3f} ppm/°C')
plt.xlabel('Temperature (°C)')
plt.ylabel('Frequency Error (ppm)')
plt.title('Crystal Temperature Coefficient')
plt.legend()
plt.grid(True)
plt.savefig('temperature_coefficient.png', dpi=150)
plt.show()
```

### Typical Coefficients

| Crystal Type | Temp Coefficient | Frequency Change (0-40°C) |
|--------------|------------------|---------------------------|
| AT-cut (room temp) | -0.035 ppm/°C/°C | ±1-2 ppm |
| TCXO (cheap) | -0.5 to -2 ppm/°C | ±10-40 ppm |
| TCXO (good) | -0.1 to -0.5 ppm/°C | ±2-10 ppm |
| OCXO | -0.001 to -0.01 ppm/°C | ±0.02-0.2 ppm |

### Temperature Compensation

Once you know TC, compensate manually:

```
Current temp: 35°C
Reference temp: 25°C
Temperature coefficient: -0.8 ppm/°C
ΔT = 35 - 25 = +10°C

Frequency shift = -0.8 ppm/°C × 10°C = -8 ppm
Apply correction: +8 ppm to compensate
```

---

## Measurement Uncertainty Analysis

### Sources of Uncertainty

**GPS Method:**

| Source | Uncertainty (ppm) | Notes |
|--------|-------------------|-------|
| GPS 1PPS jitter | ±0.001 | Negligible |
| Counter resolution | ±0.00003 | 1 count in 30M |
| Temperature drift | ±0.1-1.0 | During measurement |
| Short-term noise | ±0.01 | Average multiple |

**Total (RSS):** ±0.1-1.0 ppm (dominated by temperature)

**WWV/CHU Method:**

| Source | Uncertainty (ppm) | Notes |
|--------|-------------------|-------|
| Transmitter | ±0.00001 | Atomic clock |
| Ionosphere | ±0.01-0.1 | Doppler shift |
| Multipath | ±0.05-0.5 | Fading |
| Receiver | ±0.1-1.0 | Measurement |

**Total (RSS):** ±0.1-1.0 ppm

**FM Broadcast:**

| Source | Uncertainty (ppm) | Notes |
|--------|-------------------|-------|
| Station accuracy | ±0.01-2.0 | GPS-locked vs. free |
| TinySA resolution | ±0.1-1.0 | At 100 MHz |
| Averaging | ±0.5 | Multiple stations |

**Total (RSS):** ±1-5 ppm (depends on station quality)

### Reducing Uncertainty

1. **Average multiple measurements:**
   ```
   σ_average = σ / sqrt(N)

   Example:
   Single measurement: ±1 ppm
   Average 10 measurements: ±0.32 ppm
   Average 100 measurements: ±0.1 ppm
   ```

2. **Temperature stabilization:**
   - Let equipment warm up 30+ minutes
   - Measure in stable environment
   - Correct for temperature coefficient

3. **Multiple methods:**
   - GPS (primary)
   - WWV/CHU (verify)
   - FM (quick check)
   - All should agree within combined uncertainty

---

## Acceptance Testing

### Final Validation Checklist

Before considering calibration complete:

**Immediate Checks:**
- [ ] Error reduced from >10 ppm to <1 ppm
- [ ] Calibration survives power cycle
- [ ] Verified against second independent source
- [ ] No unexpected jumps or instabilities

**Short-Term Checks (1 hour):**
- [ ] Frequency stable to ±0.5 ppm over 1 hour
- [ ] No large temperature swings
- [ ] Measurements repeatable

**Long-Term Checks (24 hours):**
- [ ] Drift < 1 ppm over 24 hours
- [ ] Temperature correlation understood
- [ ] No sudden frequency jumps

**Documentation:**
- [ ] Recorded calibration date
- [ ] Noted calibration method
- [ ] Documented temperature coefficient
- [ ] Saved reference measurements
- [ ] Planned re-calibration date (1 year)

---

## Periodic Re-Calibration

### When to Re-Calibrate

**Mandatory:**
- After firmware update (may reset cal)
- After opening case (may affect crystal)
- After drop or impact
- If measurements show >2 ppm error

**Recommended:**
- Every 6-12 months (crystal aging)
- After extreme temperature exposure
- Before critical measurements

**Optional:**
- Before each use (paranoid)
- Monthly (if doing precision work)

### Quick Cal Check

Don't need full re-calibration - just verify:

```
1. Measure GPS 1PPS or CHU
2. Note error
3. If < 1 ppm: OK, no action
4. If 1-3 ppm: Minor adjustment
5. If > 3 ppm: Full re-calibration needed
```

---

## Documentation Template

### Calibration Record

```
TinySA Calibration Record
=========================

Date: _____________
Operator: _____________
Method: [ ] GPS  [ ] WWV/CHU  [ ] FM  [ ] Other:_______

Initial Error: _______ Hz (_______ ppm) @ 30MHz
Final Error: _______ Hz (_______ ppm) @ 30MHz

Reference Source(s):
1. __________________________
2. __________________________
3. __________________________

Measurements:
Before: _______ Hz
After:  _______ Hz

Temperature During Cal: _______°C
Temperature Coefficient: _______ ppm/°C

Verification:
Method 1: _______ ppm error
Method 2: _______ ppm error
Agreement: ✓ / ✗

24-Hour Stability: _______ ppm p-p

Notes:
_____________________________________________
_____________________________________________

Next Calibration Due: _____________

Signature: _____________ Date: _____________
```

---

## Troubleshooting Verification Failures

### Problem: Large Discrepancy Between Methods

**Example:**
```
GPS shows: +5.2 ppm
CHU shows: +11.8 ppm
Difference: 6.6 ppm (too large!)
```

**Possible causes:**
1. GPS not locked (check 1PPS is blinking)
2. CHU propagation poor (multipath, fading)
3. Measurement error (recheck connections)
4. Temperature changed between measurements

**Solution:**
- Re-measure with stable conditions
- Use third method (FM) to arbitrate
- Trust GPS if all other checks pass

### Problem: Frequency Jumps

**Example:**
```
Measurement 1: 30,000,150 Hz
Measurement 2: 30,000,148 Hz
Measurement 3: 30,002,580 Hz ← Jump!
Measurement 4: 30,000,149 Hz
```

**Possible causes:**
1. Loose connection (intermittent)
2. Power supply noise
3. Counter overflow (if using 16-bit counter)
4. Software bug

**Solution:**
- Check all connections
- Add decoupling capacitors
- Use 32-bit counter
- Re-upload firmware

### Problem: Temperature Sensitivity Too High

**Example:**
```
Measured TC: -5.2 ppm/°C
Expected: -0.5 to -2 ppm/°C
```

**Possible causes:**
1. Poor quality crystal
2. Aging crystal
3. Measurement error
4. Other temperature-sensitive component

**Solution:**
- Re-measure carefully
- Consider replacing crystal (if accessible)
- Use tighter temperature control
- Apply software compensation

---

## Advanced Verification: Beat Frequency Method

### Concept

Mix TinySA 30MHz with GPS-locked 30MHz reference, listen to beat note.

**Setup:**
```
GPS-locked 30MHz ──┐
                   ├──→ Mixer ──→ Low-pass filter ──→ Audio amp ──→ Speaker
TinySA 30MHz ──────┘

Beat frequency = |f1 - f2|
```

**If calibration perfect:**
- Beat frequency = 0 Hz (silence)

**If error exists:**
- Beat frequency = error in Hz
- Example: 150 Hz error → hear 150 Hz tone

**Advantages:**
- Very sensitive (can hear 1 Hz difference)
- Real-time monitoring
- No counter needed

**Disadvantages:**
- Requires building mixer circuit
- Need GPS-locked 30MHz reference
- Can't measure sign of error (only magnitude)

---

## Summary

### Verification Completed

✓ Immediate post-calibration check
✓ Cross-reference against multiple sources
✓ Short-term stability (1 hour)
✓ Long-term stability (24 hours)
✓ Temperature characterization
✓ Uncertainty analysis
✓ Acceptance testing
✓ Documentation

### Key Metrics

**Good Calibration:**
- Final error: < 1 ppm
- Agreement between methods: < 0.5 ppm
- 1-hour stability: < 0.5 ppm
- 24-hour drift: < 1 ppm
- Temperature coefficient: Known and compensated

**Next Steps:**
- Use calibrated TinySA with confidence
- Re-calibrate annually
- Document all measurements

---

**Your TinySA is now calibrated to atomic clock accuracy!**
