# Field Testing Antenna Suite with nanoVNA and tinySA
## Practical Measurement Procedures for Portable RF Analysis

---

## TABLE OF CONTENTS

1. [SECTION I — INSTRUMENT OVERVIEW](#section-i)
2. [SECTION II — NANOVNA IMPEDANCE MEASUREMENTS](#section-ii)
3. [SECTION III — TINYSA SPECTRUM AND RADIATION ANALYSIS](#section-iii)
4. [SECTION IV — FIELD TEST WORKFLOW](#section-iv)
5. [SECTION V — DATA LOGGING AND ANALYSIS](#section-v)
6. [APPENDIX A — QUICK REFERENCE CHECKLIST](#appendix-a)
7. [APPENDIX B — TROUBLESHOOTING MEASUREMENT ERRORS](#appendix-b)

---

## SECTION I — INSTRUMENT OVERVIEW {#section-i}

### 1.1 nanoVNA Specifications

The nanoVNA is a portable Vector Network Analyzer (VNA) covering 50 kHz – 900 MHz (extended models to 3 GHz).

**Key Capabilities:**
- Two-port S-parameter measurement (S11, S21)
- Impedance (Z), admittance (Y), reflection coefficient (Γ)
- SWR, return loss, magnitude/phase display
- TDR (time-domain reflectometry) for cable/feed-line analysis
- Marker functions for frequency-specific readouts
- CSV data export for post-processing

**Physical Interface:**
- 3.5" display with touch controls
- USB-C power and data connection
- Integrated battery (4-6 hour runtime)
- SMA female connectors (ports 1 and 2)

**Calibration:** Must be calibrated at start of test session using SOL (short-open-load) standard kit or 50Ω load.

### 1.2 tinySA Specifications

The tinySA is a portable spectrum analyzer covering 100 kHz – 350 MHz (extended to 960 MHz, 4.4 GHz on later models).

**Key Capabilities:**
- Real-time FFT spectrum display (resolution: 1 kHz)
- Peak search, signal tracking, marker readouts
- Amplitude calibration (dBm display with reference level setting)
- Modulation detection (AM, FM, CW)
- Port monitoring without requiring biasing (passive listening)
- Configurable spans (full-band or zoom)

**Physical Interface:**
- 2.4" TFT display
- USB-C power and control
- SMA female connector for antenna input
- Internal 50Ω termination (adaptive sensitivity)

**Advantage:** Non-destructive—does NOT inject power, only listens to RF environment. Safe to monitor active transmitters.

---

## SECTION II — NANOVNA IMPEDANCE MEASUREMENTS {#section-ii}

### 2.1 Pre-Test Calibration

**Equipment Required:**
- nanoVNA with calibration kit (short, open, load standards)
- Coaxial cable assembly (test port to antenna feedpoint)
- 50Ω load for calibration verification

**Procedure:**

1. **Power on nanoVNA** and allow 2-minute warm-up for stable reference oscillator.

2. **Connect calibration short standard** to Port 1:
   - Short: Two adjacent pins shorted together (typically included in kit)
   - Record the marker readout: should show S11 ≈ 0° with magnitude ≈ 1.0 (perfect reflection)

3. **Connect calibration open standard**:
   - Open: No connection between pins (or included open stub)
   - Record: S11 ≈ 180° with magnitude ≈ 1.0

4. **Connect 50Ω load standard**:
   - Precision 50Ω termination (e.g., Huber+Suhner, Minicircuits)
   - Record: S11 ≈ 0° with magnitude ≈ 0.0 (perfect absorption)
   - If magnitude > 0.05, recalibrate or verify load quality

5. **Store calibration**: nanoVNA firmware saves SOL calibration automatically to memory. Calibration valid for 2-4 hours; recalibrate if temperature changes >10°C or after 4 hours.

> **CAUTION**
> Do NOT connect antenna feedpoint during calibration. This will corrupt the reference plane and invalidate all subsequent measurements.

### 2.2 Impedance Measurement at Feedpoint

**Field Setup:**

1. **Erect antenna** at intended operating height (or as close as practical for test).

2. **Connect test cable** from nanoVNA Port 1 to antenna feedpoint:
   - Use coaxial cable with known velocity factor (VF ≈ 0.66 for RG-58/RG-8)
   - Note cable length for velocity factor compensation if performing TDR analysis
   - Secure connector with torque wrench (SMA: 1.2 N·m typical)

3. **Open nanoVNA main screen**:
   - Select **S11 measurement mode** (reflection only, no Port 2 needed)
   - Set frequency span to band of interest ±20% (e.g., 40M: 6.5–7.8 MHz)
   - Set stimulus power: **-10 dBm** (standard; safe for active circuits)

4. **Scan frequency**:
   - Press **START** to execute frequency sweep
   - Typical sweep time: 2–5 seconds depending on point density (101 points standard)

5. **Analyze impedance plot**:
   - Display should show: **Smith chart** (impedance plane) or **impedance magnitude/phase**
   - Target: Impedance circle passes through center (50Ω) at band center
   - Mark minimum SWR point using **marker tool**: move cursor to lowest S11 magnitude peak

### 2.3 Reading SWR Directly from nanoVNA

**Method 1: SWR from S11 magnitude**

The nanoVNA firmware calculates SWR automatically:
```
SWR = (1 + |Γ|) / (1 − |Γ|)

where Γ = S11 (magnitude of reflection coefficient)
```

**Display steps:**
1. Open nanoVNA **menu** → **Display** → **Format**
2. Select **SWR** format (or dual-display with impedance + SWR)
3. Read SWR value at cursor position (typically marked in status line)

**Interpretation:**
- SWR = 1.0 → Perfect match (50Ω, no reflection)
- SWR = 1.5 → Acceptable for most applications, 4% reflected power
- SWR = 2.0 → Marginal, 11% reflected power; requires tuning
- SWR > 3.0 → Poor match; antenna requires adjustment

**Method 2: Return loss (alternative view)**

Return loss (RL) in dB relates to |Γ|:
```
RL (dB) = −20 × log₁₀(|Γ|)
```

- RL = −6 dB → SWR ≈ 3.0
- RL = −10 dB → SWR ≈ 2.0
- RL = −14 dB → SWR ≈ 1.5
- RL = −20 dB → SWR ≈ 1.1 (excellent)

### 2.4 Finding Resonance (Dip Method)

The **resonance dip** is where impedance is purely resistive (imaginary part = 0).

**Procedure:**

1. **Set nanoVNA to impedance display** (real + imaginary):
   - Menu → Display → **Impedance R + jX**

2. **Locate zero crossing of reactance (jX)**:
   - Sweep across band center
   - Mark where reactance curve crosses zero line
   - This is **electrical resonance**

3. **Record resonance frequency**:
   - Use marker tool to note frequency at zero-reactance crossing
   - This may NOT be the lowest SWR point (loaded antennas often show offset)

4. **Read resistance at resonance**:
   - At the zero-reactance point, read the resistance (R) value
   - For 50Ω feedpoint: R should be close to 50Ω
   - If R = 25Ω, feedpoint impedance is low (use 1:2 transformer or antenna tuner)
   - If R = 100Ω, feedpoint impedance is high

> **NOTE**
> Resonance (zero reactance) and minimum SWR may occur at different frequencies, especially for antennas with trap loading. Always check BOTH points.

### 2.5 Trap Tuning with nanoVNA

Parallel LC traps in loaded antennas can be precisely tuned using nanoVNA feedback.

**Setup:**
1. Erect antenna with trap accessible (trap capacitor exposed for adjustment)
2. Connect nanoVNA to feedpoint as in Section 2.2
3. Scan band to establish baseline SWR curve

**Tuning procedure:**
1. **Identify trap resonance peak**: Secondary hump in SWR curve (usually 5–20% above band center)
2. **Adjust trap trimmer capacitor** (turn adjustment slug):
   - **Turning clockwise (CW):** Increases capacitance → lowers resonance frequency
   - **Turning counter-clockwise (CCW):** Decreases capacitance → raises resonance frequency
   - Make **0.5-turn increments** and re-scan after each adjustment

3. **Target condition**: Secondary peak should:
   - Move toward band center frequency
   - Merge with primary resonance as trap Q improves

4. **Final optimization**:
   - Adjust main loading coil inductance (if taps available) to place resonance at band center
   - Adjust trap capacitor to minimize SWR at resonance
   - Target: SWR < 1.5:1 across entire band (160M–6M typically 1.5–2.0)

### 2.6 TDR Analysis (Cable Fault Detection)

**Time-Domain Reflectometry** reveals cable faults and feed-line issues.

**Procedure:**

1. **Enable TDR mode**: Menu → **Analyze** → **TDR**

2. **Set velocity factor**:
   - RG-58 (polyethylene): VF = 0.66
   - RG-8 (polyethylene): VF = 0.66
   - LMR-400 (foam): VF = 0.85
   - Corrected distance = measured_distance × VF

3. **Scan with antenna disconnected** (TDR will show cable impedance profile):
   - Distance = 0: nanoVNA SMA connector (reference point)
   - Distance = L (cable length): Open circuit (antenna disconnected)

4. **Interpret TDR trace**:
   - Flat response: Cable is good, 50Ω characteristic impedance maintained
   - Sharp dip/step: Impedance discontinuity (kink in cable, wet connector, bad solder joint)
   - Attenuation ramp: Cable loss (acceptable for runs < 50 ft)

5. **Reconnect antenna and rescan**:
   - TDR will show impedance profile from nanoVNA through cable to antenna feedpoint
   - Sudden jump at antenna terminus: Confirms antenna is connected and presents load
   - Flat region before antenna: Indicates cable is properly terminated

---

## SECTION III — TINYSA SPECTRUM AND RADIATION ANALYSIS {#section-iii}

### 3.1 Passive Spectrum Monitoring (No Power Injection)

The tinySA is a **receiver-only** instrument—it listens without transmitting.

**Advantages:**
- Safe to use near active transmitters
- Can monitor interference sources
- Can verify radiation patterns indirectly (see Section 3.3)
- Non-destructive to circuits or components

### 3.2 Quick Start: Spectrum Scan

**Equipment:**
- tinySA with antenna connector adapter
- Small whip antenna or dipole probe (e.g., BNC-SMA converter + short monopole)
- USB power bank or DC supply (5V, 500 mA minimum)

**Procedure:**

1. **Power on tinySA**:
   - USB-C power connection or battery module
   - Wait 10 seconds for display initialization

2. **Select frequency band**:
   - Press **Menu** → **Range**
   - Set **Start frequency** to band lower limit (e.g., 40M: 7.0 MHz)
   - Set **Stop frequency** to band upper limit (e.g., 7.3 MHz)
   - Set **Center** and **Span** to auto-calculate

3. **Configure detection**:
   - Press **Menu** → **Detection** → **Peak Hold** (displays persistent peaks)
   - Sensitivity: Start at **−50 dBm reference level**, adjust downward if noise floor is high

4. **Start sweep**:
   - Press **Start** or **Scan** button
   - Spectrum will update in real-time, typically 5–10 second refresh rate

5. **Interpret display**:
   - Horizontal axis: Frequency (MHz)
   - Vertical axis: Power (dBm) relative to reference level (−50 dBm typical)
   - Green trace: Real-time spectrum
   - White dots: Peak markers (if peak-hold enabled)

### 3.3 Identifying Your Antenna's Transmitted Signal

If antenna is **active** (connected to transmitter):

1. **Set tinySA to band of interest** (see Section 3.2, step 2)

2. **Tune reference level**:
   - If transmitter output is strong (>0 dBm at feedpoint):
     - Start with **−10 dBm reference** to avoid clipping
   - If output is weak (< −10 dBm):
     - Use **−50 to −60 dBm reference** for sensitivity

3. **Transmit test signal** (CW or narrow modulation):
   - Low power initially (**< 5 watts**) to avoid tinySA overload
   - Use dip meter or keyer to ensure steady-state transmission

4. **Observe spectrum signature**:
   - Peak should appear at antenna resonance frequency
   - **Bandwidth at −3 dB** indicates antenna bandwidth:
     - Narrow peak (< 50 kHz on 40M): High Q, selective antenna
     - Broad peak (> 200 kHz on 40M): Low Q, wide-bandwidth antenna

5. **Monitor harmonic content**:
   - Harmonics should be > 40 dB down from fundamental
   - If harmonics are high, verify antenna doesn't resonate on harmonic bands (e.g., 40M antenna shouldn't be resonant on 20M)

### 3.4 Indirect Radiation Pattern Assessment via Frequency Response

While tinySA cannot directly measure radiation patterns (which require moving the antenna while transmitting), frequency response reveals pattern clues:

**Method: Monitor received signal strength while rotating antenna**

**Setup:**
1. Deploy **reference transmitter** at fixed distance (100–300 ft, line-of-sight)
2. Transmit **constant-power CW** on antenna's design frequency
3. Hold tinySA receiving probe at fixed position (perpendicular to antenna's main lobe)
4. **Rotate antenna** in azimuth (horizontal plane) while observing tinySA display

**Interpretation:**
- **Omnidirectional antenna**: Received signal level constant during rotation (±3 dB variation)
- **Directional antenna (Yagi, end-fed)**: Signal varies 5–15 dB as antenna rotates
  - Maximum when antenna points toward reference transmitter
  - Minimum when antenna points away
  - Ratio (max/min) indicates front-to-back ratio

**Example (40M Yagi, 3 elements):**
```
Azimuth     Signal Level (dBm)    Note
0° (broadside)    −35        Maximum
45°               −42        Side lobe
90° (endfire)     −50        Null zone
180° (back)       −55        Back lobe (−20 dB down)
```

### 3.5 Measuring Antenna Tuning via Spectrum Peak Width

Even without transmitting, a **loaded antenna's quality factor (Q)** is reflected in the spectrum peak width.

**Procedure:**

1. **Activate nearby transmitter** at antenna's design frequency (or higher power on nearby band)

2. **Monitor spectrum at test antenna location** with tinySA probe positioned near antenna feedpoint

3. **Measure bandwidth at −3 dB point**:
   - Locate peak on spectrum display
   - Use **marker tool** to note frequency where power drops to half (−3 dB below peak)
   - Record **low and high −3 dB frequencies**
   - Bandwidth (BW) = f_high − f_low

4. **Calculate Q**:
   ```
   Q = f_center / BW
   
   where f_center = (f_high + f_low) / 2
   ```

5. **Interpret**:
   - Compact dipole (loaded): Q ≈ 15–30 (BW ≈ 150–250 kHz on 40M)
   - Full-size dipole: Q ≈ 50–100 (BW ≈ 70–140 kHz on 40M)
   - Magnetic loop: Q ≈ 100–300 (BW ≈ 20–70 kHz on 40M)

High Q = narrow bandwidth, precise tuning required; low Q = broad bandwidth, forgiving tuning.

---

## SECTION IV — FIELD TEST WORKFLOW {#section-iv}

### 4.1 Complete Antenna Test Sequence (Single Session)

**Time allocation: 45 minutes per antenna**

**Pre-test (5 min):**
1. Erect antenna at operating height
2. Verify feedline connections (torque to spec)
3. Power on nanoVNA (2-minute warm-up)
4. Perform SOL calibration using standards kit

**nanoVNA measurements (15 min):**
1. Scan full band (±20%) to establish baseline SWR curve
2. Identify resonance dip (zero-reactance point)
3. Record resonance frequency, impedance at resonance, SWR at band center
4. If trap-loaded antenna: Tune trap capacitor for minimum SWR, iterate 2–3 cycles
5. Record final impedance Smith chart or screenshot

**tinySA monitoring (15 min):**
1. Set up reference transmitter (low power, < 5 W)
2. Connect tinySA probe near antenna feedpoint
3. Scan spectrum across band; note peak frequency and bandwidth
4. Rotate antenna 360° in azimuth while monitoring signal levels
5. Record max/min signal ratio (front-to-back or directivity)

**Optional TDR analysis (10 min):**
1. If feedline length > 30 ft or suspecting cable fault:
2. Perform TDR scan with antenna disconnected, then connected
3. Compare impedance profiles to detect discontinuities

**Post-test (documentation, 5 min):**
1. Record all measurements in field log (see Section 5.1)
2. Take photos of antenna configuration (angle, height, surroundings)
3. Transfer nanoVNA/tinySA data to laptop via USB

### 4.2 Adaptive Testing (Problem Diagnosis)

If antenna performs poorly (SWR > 2.5, unexpected resonance), follow diagnostic tree:

**Problem: SWR > 2.5 at band center**

1. **Check cable quality**:
   - Perform TDR scan (Section 2.6)
   - If TDR shows discontinuity: Cable is faulty, replace and re-test

2. **Check antenna connection**:
   - Visually inspect feedpoint for corrosion, loose solder
   - Measure DC resistance from center to shield at feedpoint (should be < 0.5Ω on good joint)
   - If resistance > 1Ω: Re-solder connections

3. **Check trap tuning** (if applicable):
   - Observe secondary resonance peak in nanoVNA Smith chart
   - If peak is offset from band center: Adjust trap capacitor per Section 2.5
   - If secondary peak is absent: Trap may be open (failed capacitor); inspect visually

4. **Check antenna length**:
   - Measure physical arm length vs. design spec
   - If measured > design by > 5 cm: Trim ends (unbalanced length is major SWR culprit)
   - If measured < design: Check for coiled or tangled wire

5. **Re-test after each correction**:
   - Sweep nanoVNA after each fix
   - SWR should improve monotonically

**Problem: Resonance frequency offset from design**

1. **Frequency too low** (f_res < f_design by > 2%):
   - Loading inductance too high (coil has more turns than intended)
   - Reduce coil turns by 0.5–1 turn and re-measure
   - Or: Antenna physically too long; trim 2–3 cm from each arm

2. **Frequency too high** (f_res > f_design by > 2%):
   - Loading inductance too low (coil has fewer turns)
   - Add 0.5–1 turn to coil and re-measure
   - Or: Antenna physically too short; extend by adding wire segment

3. **Unequal arms** (different resonance on left vs. right):
   - Arms should have equal inductance
   - Measure coil inductance on both arms with LCR meter (if available)
   - Trim or add wire to the longer arm to match electrical length

### 4.3 Environmental Corrections

RF characteristics vary with environment:

**Temperature effect** (HF bands primarily):
- Coil inductance changes ~−0.03%/°C (wire resistance increases with warmth)
- SWR typically stable within ±0.1 from 0°C to 40°C on compact dipoles
- Recalibrate nanoVNA if temperature change > 20°C during session

**Humidity effect** (VHF/UHF):
- Water absorption in insulation reduces Q and shifts resonance downward (< 1% effect)
- Allow antenna to dry 30 minutes after rain before critical measurements
- If measurements must be done in rain: Accept ±2% uncertainty

**Ground proximity effect** (HF antennas):
- Horizontal antennas: Resonance stable if height > 0.25λ
- Vertical antennas: Radial system and ground conductivity affect impedance significantly
- If testing mobile vertical on lossy ground: Expect SWR ±0.3 higher than installed at base station

**Surrounding structures**:
- Metal objects (buildings, fences, power lines) within 10 m can detune antenna by 2–5%
- Test in clear area if possible; document nearby structures in test report

---

## SECTION V — DATA LOGGING AND ANALYSIS {#section-v}

### 5.1 Field Log Template

Create one log entry per antenna per test session:

```
FIELD TEST LOG — [Antenna Name]
Test Date: ________________   Operator: ________________
Location: ________________   QTH Grid: ________________
Temperature: ____°C   Humidity: ____%   Wind: ____kt

ANTENNA CONFIGURATION:
  Erection height (AGL): ______ m
  Physical arm length (measured): ______ m (per design: ______ m)
  Form diameter / coil turns: ______ / ______
  Feedline type/length: ______ / ______ ft
  Nearby structures: ____________________

NANOVNA MEASUREMENTS:
  Calibration method: SOL kit   Date/Time of calibration: ________
  
  Resonance frequency (zero reactance): ______ MHz
  SWR at resonance: ______
  Impedance at resonance (R + jX): ______ + j______ Ω
  SWR at band center: ______
  SWR at band edges: ______ (low) / ______ (high)
  
  Trap tuning notes (if applicable):
    Initial secondary peak: ______ MHz
    After adjustment: ______ MHz
    Trimmer capacitor position: ______ turns CW from open

TINYSA MEASUREMENTS:
  Reference transmitter power: ______ W
  Spectrum peak frequency: ______ MHz
  Peak power level: ______ dBm
  −3 dB bandwidth: ______ kHz
  Calculated Q: ______
  
  Azimuth response (rotation test):
    0° (boresight): ______ dBm
    90°: ______ dBm
    180° (back): ______ dBm
    270°: ______ dBm
    Front-to-back ratio: ______ dB

CABLE/FEEDPOINT ANALYSIS:
  TDR scan (if performed): [checked if cable good / describe fault]
  Feedpoint visual inspection: [corrosion? loose solder?]
  DC resistance center-to-shield: ______ Ω

OVERALL ASSESSMENT:
  ☐ PASS — SWR < 1.5, resonance within ±1% of design, impedance 40–60Ω
  ☐ MARGINAL — SWR 1.5–2.0, minor tuning required
  ☐ FAIL — SWR > 2.0, significant deviation, requires rework

Corrective actions taken: ____________________
Next steps: ____________________

Operator signature: ___________________________
```

### 5.2 Post-Test Data Analysis

**Export nanoVNA data:**
1. Connect nanoVNA to laptop via USB-C
2. Use nanoVNA software (e.g., nanoVNA-Saver) to download measurements
3. Export as CSV: File → Export → S-Parameters (CSV)
4. File format: Frequency (MHz), S11 real, S11 imag, S21 (magnitude), phase

**Plotting in spreadsheet or Python:**

```python
import pandas as pd
import matplotlib.pyplot as plt

# Load nanoVNA CSV data
data = pd.read_csv('antenna_40m_test.csv')
freq = data['Frequency (MHz)']
s11_real = data['S11 Real']
s11_imag = data['S11 Imag']

# Calculate SWR
gamma_mag = (s11_real**2 + s11_imag**2)**0.5
swr = (1 + gamma_mag) / (1 - gamma_mag)

# Plot
fig, (ax1, ax2) = plt.subplots(2, 1)
ax1.plot(freq, swr, 'b-', linewidth=2)
ax1.axhline(1.5, color='g', linestyle='--', label='Target (1.5:1)')
ax1.set_ylabel('SWR')
ax1.set_title('40M Compact Dipole — Field Test')
ax1.legend()
ax1.grid()

# Impedance
z_real = 50 * (1 - s11_real + s11_imag**2) / ((1 - s11_real)**2 + s11_imag**2)
z_imag = 50 * (-2 * s11_imag) / ((1 - s11_real)**2 + s11_imag**2)
ax2.plot(freq, z_real, 'r-', label='Resistance (Ω)', linewidth=2)
ax2.plot(freq, z_imag, 'b--', label='Reactance (Ω)', linewidth=2)
ax2.axhline(50, color='g', linestyle=':', label='50Ω target')
ax2.set_xlabel('Frequency (MHz)')
ax2.set_ylabel('Impedance (Ω)')
ax2.legend()
ax2.grid()
plt.tight_layout()
plt.savefig('40m_impedance_analysis.png', dpi=150)
```

### 5.3 Comparison to Design Specifications

Create a summary table after testing:

| Parameter | Design Target | Field Measured | Delta | Status |
|-----------|---|---|---|---|
| Resonance frequency | 7.150 MHz | 7.163 MHz | +13 kHz (+0.18%) | ✓ Pass |
| SWR @ 7.150 MHz | < 1.5 | 1.42 | — | ✓ Pass |
| Impedance (R) @ resonance | 50 Ω | 51.3 Ω | +1.3 Ω | ✓ Pass |
| Arm length | 5.0 m | 4.98 m | −0.02 m | ✓ Pass |
| Q estimate | 15–20 | 18 | — | ✓ Pass |

**Acceptance criteria:**
- ✓ PASS: All parameters within ±2% of design and SWR < 1.5
- ⚠ MARGINAL: 1–2 parameters within ±3%, SWR 1.5–2.0; requires minor tuning
- ✗ FAIL: > 2 parameters outside ±3%, SWR > 2.0; requires rework

---

## SECTION VI — EQUIPMENT CARE AND MAINTENANCE {#section-vi}

### 6.1 nanoVNA Care

- **Storage**: Keep at room temperature (15–30°C). Avoid condensation by sealing in dry bag if used in humid environment.
- **Connectors**: Clean SMA connectors with isopropyl alcohol before storage (prevents oxidation).
- **Calibration kit**: Store standards in foam case, inspect for physical damage quarterly.
- **Battery**: If internal battery present, charge monthly even if not in use to prevent deep discharge.
- **Firmware updates**: Check nanoVNA project site (GitHub) quarterly for bug fixes and calibration improvements.

### 6.2 tinySA Care

- **Input protection**: Do NOT exceed input level > 0 dBm (1 mW) on SMA connector; exceeds automatic attenuation.
- **Antenna probe**: If using external probe antenna, store with cap on connector to prevent bent pins.
- **Display**: Avoid bright sunlight for extended periods; display contrast may fade.
- **Power**: tinySA draws minimal power; USB battery pack (5000 mAh) enables 6–8 hours field operation.

### 6.3 Test Cables and Connectors

- **Coaxial cable**: Replace if kinks, pinches, or moisture detected (TDR may help identify faults).
- **Adapters**: Keep SMA-to-BNC and SMA-to-N adapters, plus barrel connectors (50Ω rated) for various antenna connections.
- **Torque specification**: SMA female connectors require 1.2 N·m (10 in·lbf); use calibrated torque wrench to avoid over-tightening.

---

## APPENDIX A — QUICK REFERENCE CHECKLIST {#appendix-a}

### Before Each Test Session

- [ ] nanoVNA battery charged (> 50%)
- [ ] tinySA battery or USB power ready
- [ ] Calibration kit accessible
- [ ] Test cables in dry condition (no moisture or kinks)
- [ ] Antenna erected and secured
- [ ] Nearby RF sources documented (for reference when interpreting spectrum)

### Measurement Checklist

**nanoVNA:**
- [ ] Perform SOL calibration
- [ ] Scan full band with S11 mode
- [ ] Record resonance frequency and impedance
- [ ] Document SWR at band center and edges
- [ ] Tune traps (if applicable) and re-scan

**tinySA:**
- [ ] Set frequency span to ±20% of band
- [ ] Connect probe antenna or coax to feedpoint area
- [ ] Scan spectrum and record peak frequency
- [ ] Perform azimuth rotation test (record max/min levels)
- [ ] Calculate Q from −3 dB bandwidth

**Post-test:**
- [ ] Export nanoVNA CSV data
- [ ] Complete field log (Section 5.1)
- [ ] Take photographs of antenna configuration
- [ ] Store all data in dated folder structure

---

## APPENDIX B — TROUBLESHOOTING MEASUREMENT ERRORS {#appendix-b}

### Problem: nanoVNA shows erratic SWR readings

**Cause 1: Poor calibration**
- Solution: Repeat SOL calibration. Ensure short, open, and load standards are clean and undamaged.

**Cause 2: Loose SMA connector**
- Solution: Using calibrated torque wrench (1.2 N·m), re-tighten all connections. Replace worn connectors if slop is present.

**Cause 3: Feedline has high loss (> 50 ft long)**
- Solution: nanoVNA measures impedance **at instrument port**, not at antenna. For long cables, use **velocity factor correction** or accept ±0.2 SWR reading uncertainty. TDR can verify cable quality.

### Problem: tinySA spectrum shows only noise

**Cause 1: Reference transmitter too weak**
- Solution: Increase transmitter power to 5–10 W. Ensure transmission is on antenna's design frequency.

**Cause 2: tinySA sensitivity insufficient**
- Solution: Lower reference level to −60 or −70 dBm. Ensure probe is positioned near antenna feedpoint (within 10 cm).

**Cause 3: Probe antenna connectivity loose**
- Solution: Re-seat SMA connectors. Inspect for bent pins on male connectors.

### Problem: Resonance dip is very broad (> 500 kHz on 40M)

**Likely cause: High-loss environment**
- High ground loss (wet soil, salt water nearby)
- Cable moisture (water ingress into coax)
- Poor antenna construction (marginal solder joints, corroded connections)

**Solution:**
1. Inspect feedline with TDR (Section 2.6) for moisture
2. Dry antenna and cables in sun for 1 hour
3. Re-test to confirm improvement

### Problem: Impedance on Smith chart shows rotation but not correct location

**Cause: Cable velocity factor set incorrectly**
- nanoVNA typically assumes standard 0.66 VF. If using foam-dielectric cable (0.85 VF), impedance plot will show error.
- Solution: Adjust cable length **compensation** in nanoVNA settings if feature available, or accept plot rotation.

---

**End of Field Testing Guide**

*For integration into antenna technical chapters, reference this document in SECTION VI — TEST AND ALIGNMENT of each antenna manual.*

