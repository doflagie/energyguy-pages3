# AUTOMATIC DIRECTION FINDING SYSTEMS — HF/VHF/UHF
## Watson-Watt, Doppler, Phased Array, and Rotary Loop RDF Systems

---

```
DOCUMENT NUMBER:   TM-ADF-001 Rev A
SYSTEM:            Automatic Direction Finding (ADF) Suite
COVERAGE:          160M through 20CM (1.8 MHz – 1300 MHz)
CLASSIFICATION:    UNCLASSIFIED
PREPARED BY:       TM Engineering
DATE:              2026-05-02
```

---

## TABLE OF CONTENTS

- [Chapter 1 — System Description and Theory](#chapter-1)
- [Chapter 2 — Specifications](#chapter-2)
- [Chapter 3 — Controls and Indicators](#chapter-3)
- [Chapter 4 — Installation](#chapter-4)
- [Chapter 5 — Operating Procedures](#chapter-5)
- [Chapter 6 — Calibration Procedures](#chapter-6)
- [Chapter 7 — NEC Antenna Models](#chapter-7)
- [Chapter 8 — Firmware Architecture](#chapter-8)
- [Chapter 9 — Troubleshooting](#chapter-9)
- [Chapter 10 — Preventive Maintenance](#chapter-10)
- [Chapter 11 — Parts List](#chapter-11)
- [Appendix A — Band-by-Band Operating Notes](#appendix-a)
- [Appendix B — Watson-Watt Algorithm Derivation](#appendix-b)
- [Appendix C — Doppler Equation Derivation](#appendix-c)
- [Appendix D — Quick Reference Card](#appendix-d)

---

<a name="chapter-1"></a>
# CHAPTER 1 — SYSTEM DESCRIPTION AND THEORY

## 1.1 Overview of ADF Systems

This manual covers the ADF Suite, a family of four Automatic Direction Finding (RDF) systems designed for field portable and fixed-station use across the frequency range 1.8 MHz to 1300 MHz. Each system employs a different antenna and signal processing technique suited to a specific portion of the spectrum and a set of operating conditions. All four systems share a common hardware and software platform based on the ESP32 microcontroller, u-blox NEO-8M GPS receiver, QMC5883L digital compass, SSD1306 OLED display, WiFi web interface, and Bluetooth SPP command/data interface. An optional CYD (Cheap Yellow Display, ESP32-2432S028R) touchscreen provides a full-color navigational compass rose and system status display.

### Four ADF Techniques

| Technique | Firmware Module | Best Bands | Typical Accuracy | Field Setup Complexity | Relative Hardware Cost |
|---|---|---|---|---|---|
| Watson-Watt (Adcock) | esp32_adf_hf | 160M – 10M | ±3°–8° RMS | Moderate (4-element array) | Low–Moderate |
| Doppler Ring | esp32_adf_vhf | 2M – 20CM | ±1°–3° RMS | Low (single printed disk) | Low |
| Phased Array (RSSI) | esp32_adf_hf (alt mode) | 40M – 2M | ±5°–10° RMS | Moderate | Moderate |
| Rotary Loop | esp32_rotary_loop | 160M – 15M | ±2°–5° RMS | Low (single loop on motor) | Moderate |

### Common Platform Features

All four systems provide:

- **ESP32-WROOM-32** controller: dual-core 240 MHz, 4 MB flash, 520 KB SRAM
- **GPS position reference**: u-blox NEO-8M, NMEA at 9600 baud, 1PPS output for time-stamped bearings
- **Compass bearing reference**: QMC5883L 3-axis magnetometer, I2C, 10 Hz, with hard-iron calibration
- **SSD1306 OLED**: 128×64 pixels, I2C, 4-line operational display
- **WiFi AP**: 2.4 GHz, SSID "RF-ADF[type]", local web interface with SVG compass rose
- **Bluetooth SPP**: device name "RF-ADF[type]Finder", bidirectional command/data
- **CYD touchscreen** (optional slave display): ESP32-2432S028R, TFT_eSPI, BT master mode

### Selecting the Appropriate ADF Technique

The following guidelines govern technique selection:

- **Below 30 MHz, propagation research, fixed site or vehicle**: Deploy Watson-Watt (Adcock). Provides continuous bearing with no moving parts. Works on SSB, CW, and AM signals. Array size scales with wavelength; 40M band requires 10m baseline.
- **VHF/UHF foxhunting, vehicle tracking, emergency response**: Deploy Doppler Ring. Small disk fits on a 1/4" camera stud. Provides unambiguous 0–360° bearing. Works on FM signals. Array diameter 35mm to 1027mm depending on band.
- **Rapid deployment, single-element scanning**: Deploy Phased Array RSSI mode. Uses the same Watson-Watt hardware with reduced accuracy. Good when full Adcock baseline cannot be staked out.
- **LF through HF, maximum sensitivity, covert operation**: Deploy Rotary Loop. Highest sensitivity for weak signal work. Null finding is very precise. Loop is inconspicuous.

---

## 1.2 Watson-Watt (Adcock) HF RDF Theory

### The Adcock Array

The Adcock array consists of four vertical monopole elements arranged at the cardinal compass points (North, South, East, West) of a square with a common center. The array is used in two differential pairs: the N-S pair and the E-W pair. The North element and South element are connected in opposition (signals subtracted), and similarly for East and West. A fifth element, the sense antenna, is a vertical monopole or short whip at the array center.

The differential connection of each pair eliminates the horizontal component of incoming waves (which would otherwise introduce elevation-angle errors) and produces a figure-8 antenna pattern oriented along the pair axis.

### Signal Model

Consider an arriving plane wave from bearing θ (measured clockwise from North) with amplitude E, frequency f, and phase φ:

```
Signal at North element:   V_N = E × cos(2πft + φ) × [1 + sin(θ) × kd/2]
Signal at South element:   V_S = E × cos(2πft + φ) × [1 - sin(θ) × kd/2]
```

Where k = 2π/λ and d = array baseline (element spacing). For small arrays (kd << 1), the differential output of the N-S pair is:

```
V_NS = V_N - V_S = E × k × d × sin(θ) × cos(2πft + φ)
```

Similarly for the E-W pair:

```
V_EW = V_E - V_W = E × k × d × cos(θ) × cos(2πft + φ)
```

Both differential outputs are in phase with each other and with the incoming wave. The ratio of their amplitudes is independent of E, k, d, and f — it depends only on θ.

### The Watson-Watt Bearing Formula

The bearing of the incoming signal is extracted by:

```
θ = atan2(V_NS_rms, V_EW_rms)
```

Where V_NS_rms and V_EW_rms are the RMS values of the respective differential outputs. The atan2 function returns a value in the range -180° to +180°, giving bearing in the mathematical convention; this is converted to compass bearing (0–360°, clockwise from North) in software.

### The 180° Ambiguity

The differential outputs are proportional to sin(θ) and cos(θ) respectively. Because sin(θ) = sin(180°-θ) when combined with cos, the atan2 function alone cannot distinguish between θ and θ+180°. The system does not know if the transmitter is to the North or the South, for example.

**Sense antenna ambiguity resolution**: The sense antenna receives an omnidirectional signal V_SENSE proportional to E×cos(2πft+φ). When the sense signal is combined in quadrature (90° phase shift) with the loop output, the result is:

```
V_loop + jV_sense = cardioid pattern = 1 + sin(θ)   (for N-S aligned loop)
```

This cardioid has a single null and a single peak. The bearing of the transmitter is in the direction of the cardioid peak. By comparing the sense-enhanced bearing to the ambiguous atan2 bearing, the firmware selects the correct 180° quadrant.

In practice, the ESP32 firmware applies the sense signal by sampling the sense channel separately and comparing its phase to the N-S output. If the N-S and sense signals are in phase, the transmitter is in the positive sin(θ) half-space (North side). If out of phase, the transmitter is in the South half-space, and 180° is added to the raw atan2 result.

### Phase-Matched Feed Lines

For the Watson-Watt formula to be valid, the two outputs of each differential pair must arrive at the differential amplifier with equal electrical delays. Any phase imbalance between the North and South feed lines, for example, introduces a systematic bearing error. The error magnitude in degrees is approximately:

```
ΔΘ_error ≈ Δφ_degrees / (k × d × sin(θ))
```

Feed lines must be cut to equal electrical length (same physical length and same coax type). Phase matching to better than 5° is required for ±5° bearing accuracy. Verification is performed with a NanoVNA or network analyzer (see Section 6.1).

### Balun Function

Each element pair is connected to the differential amplifier through a current-mode balun (wound on FT-50-43 toroid core, 10 turns trifilar). The balun converts the balanced (floating) differential signal from the antenna pair to an unbalanced (single-ended) 50Ω output suitable for the INA128 instrumentation amplifier input. Without the balun, common-mode interference (QRN, power line fields) appears as a false bearing.

The FT-50-43 core is used for HF (1.8–54 MHz). For operation below 3 MHz, a second winding may be added to increase low-frequency inductance. The minimum acceptable common-mode rejection of the balun is 40 dB across the operating band.

### LNA Role

The differential outputs from a small Adcock array are weak. For a baseline of 3.5m (appropriate for 40M band), the signal at the INA128 input is approximately 20 dB below the signal at a single monopole. The INA128 instrumentation amplifier provides gain of 40–60 dB (set by external resistor) with common-mode rejection >80 dB. An optional GALI-84 MMIC LNA may be added before the INA128 to overcome feed line insertion loss on longer cable runs (>10m).

### Accuracy Factors

The following factors limit bearing accuracy in practice:

| Factor | Typical Error Contribution | Mitigation |
|---|---|---|
| Element phase imbalance | ±2°–5° | Match feed lines, verify with VNA |
| Element gain imbalance | ±1°–3° | Balance check (equal signal levels) |
| Site multipath | ±5°–20° | Select open site, average multiple readings |
| Compass error | ±1°–3° | Hard-iron calibration, check for local disturbances |
| GPS declination | ±0.5°–1° | Use current IGRF model |
| ADC noise | ±0.5°–1° | Average 8–16 readings |
| Ground asymmetry | ±3°–8° | Site survey, avoid asymmetric ground |

**Overall accuracy**: A well-calibrated system on a good site typically achieves ±3°–5° RMS. On a compromised site, ±8°–15° may be the practical limit.

---

## 1.3 Doppler RDF Theory

### Electronic Antenna Rotation

The Doppler direction finding principle exploits the frequency shift that occurs when a receiver is in motion relative to a transmitter. If the receiver antenna is moved in a circular arc at velocity v, the received frequency is shifted by:

```
Δf = (f₀ × v / c) × cos(θ - φ)
```

Where θ is the bearing to the transmitter, φ is the instantaneous angular position of the moving antenna, and c is the speed of light.

Rather than physically rotating an antenna, the Doppler RDF system electronically switches among four (or more) stationary elements arranged on a circle. Each element is connected to the receiver in sequence. This creates the same frequency modulation as if a single antenna were physically rotating, but with no moving parts and at a switching rate that can be precisely controlled.

### 1kHz FM Subcarrier Creation

The standard switching sequence activates each element in turn at a rate of 1000 Hz (for a 4-element array, each element is active for 250 µs, cycling through N, E, S, W). This produces a 1 kHz frequency modulation on the received FM signal. The phase of this 1 kHz subcarrier, relative to a 1 kHz reference signal generated by the firmware, is equal to the bearing θ of the transmitter.

The switching is implemented with a FST3253 analog multiplexer (or PE4259 RF switch for VHF/UHF). The FST3253 is rated for signals up to approximately 400 MHz with low on-resistance (5Ω typical). For 33cm and 20cm bands, a PE4259 SPDT switch is cascaded to achieve the required 1–1.3 GHz performance.

### Goertzel Algorithm for Phase Extraction

The Goertzel algorithm is a computationally efficient method for computing the discrete Fourier transform at a single frequency. For extracting the 1 kHz component from the ADC sample stream, it is far more efficient than a full FFT.

The algorithm operates as follows. For a block of N samples at sample rate f_s, with target frequency bin k = N × f_target / f_s:

```
ω = 2π × k / N
coeff = 2 × cos(ω)
```

Processing loop (for each sample x[n]):

```
s[n] = x[n] + coeff × s[n-1] - s[n-2]
```

At the end of the block, the complex DFT value at frequency f_target is:

```
X = s[N] - s[N-1] × exp(-jω)

Real part:      X_re = s[N] - s[N-1] × cos(ω)
Imaginary part: X_im = s[N-1] × sin(ω)
```

The phase of the 1 kHz component is:

```
φ_measured = atan2(X_im, X_re)
```

And the bearing is:

```
θ = φ_measured - φ_reference + offset_correction
```

Where φ_reference is the phase of the 1 kHz switching reference signal (known exactly from the firmware timer) and offset_correction accounts for array orientation and cable delays (determined during calibration).

### IQ Phase Extraction Alternative

An alternative to Goertzel is to multiply the ADC sample stream by a 1 kHz sine and cosine reference (I/Q demodulation), then low-pass filter and integrate:

```
I = sum(x[n] × cos(2π × 1000 × n / f_s))   over N samples
Q = sum(x[n] × sin(2π × 1000 × n / f_s))   over N samples

φ = atan2(Q, I)
```

Both methods give equivalent results. The Goertzel method requires fewer multiplications (2 per sample vs. 2 per sample plus filtering), so it is used in the ESP32 implementation.

### Array Diameter Optimum

The modulation depth (and thus signal-to-noise ratio of the bearing estimate) is proportional to the circumference of the switching circle. The optimum array diameter for maximum modulation depth while maintaining reasonable aperture size is:

```
d_opt = λ/2 ÷ π ≈ 0.159λ
```

In practice, a circle diameter of 0.5λ (half-wavelength) is used as a compromise between modulation depth and practical array size. For the 2M band (146 MHz), λ = 2.05m, so the array diameter is 1.027m. For the 20cm band (1296 MHz), the diameter is only 115mm.

**Effect of smaller array**: If the array is smaller than 0.5λ, the modulation depth decreases proportionally, reducing the SNR of the extracted phase and increasing bearing jitter. For arrays smaller than 0.2λ, the modulation depth may be too low for reliable bearing in typical field conditions.

### Ambiguity

The Doppler system is unambiguous. The electronic rotation direction is fixed (N → E → S → W → N), and the phase of the resulting FM subcarrier is directly proportional to the transmitter bearing in 0–360°. There is no 180° ambiguity as in the loop or Watson-Watt system (before sense resolution). This makes the Doppler system particularly suitable for rapid bearing acquisition without an additional sense antenna.

### Accuracy

At high SNR (>30 dB), the Doppler system achieves ±0.5°–1° RMS bearing accuracy. At 20 dB SNR (typical field condition), accuracy is ±1°–3° RMS. Below 10 dB SNR, bearing scatter increases rapidly. The Doppler method requires an FM signal (the transmitter must be frequency-modulating). It does not work on CW or SSB signals unless they are passed through a discriminator and the resulting audio contains identifiable tone components.

---

## 1.4 Phased Array RDF Theory

### Cardioid Pattern Formation

A 2-element phased array with 90° inter-element phase shift produces a cardioid radiation/reception pattern. The array factor for two elements spaced d apart with signal from bearing θ and phase progression ψ:

```
AF(θ) = 1 + exp(j × (k × d × cos(θ) + ψ))
```

For d = λ/4 and ψ = 90°, this produces a cardioid pointing in the direction of the lagging element. By selecting which element is lagging (via a relay or PIN switch), the pattern is steered left or right. The null of the cardioid points directly opposite the peak, 180° away.

### Butler Matrix for Multi-Beam Output

The Butler matrix is a passive beamforming network using hybrid couplers and fixed phase shifters. For a 4-element array, a 4×4 Butler matrix produces four simultaneous output ports, each corresponding to a beam pointing in one of four orthogonal directions (typically 45°, 135°, 225°, 315° for a 0°-referenced array). The RSSI at each port is compared to find the beam with the highest received signal level; the transmitter is within the ±45° sector of that beam.

### Delay Line Calculation

The 90° hybrid coupler and delay lines in the phased array use transmission line segments. For a required 90° phase shift at frequency f:

```
L = (c × VF) / (4 × f)
```

Where:
- L = physical length of the delay line (meters)
- c = 3 × 10⁸ m/s (speed of light in vacuum)
- VF = velocity factor of the transmission line (0.66 for RG-58, 0.82 for RG-6)
- f = frequency (Hz)

Example: 90° delay at 146 MHz using RG-58 (VF = 0.66):

```
L = (3×10⁸ × 0.66) / (4 × 146×10⁶) = 0.338m = 33.8 cm
```

### RSSI Comparison Method

The RSSI (Received Signal Strength Indicator) comparison method uses the AD8307 logarithmic detector to measure the received signal level at each output port of the Butler matrix or phased array. The bearing is estimated by finding the beam with the highest RSSI:

```
Coarse bearing = index of max(RSSI[0], RSSI[1], RSSI[2], RSSI[3]) × 90°
```

### Gaussian Interpolation for Sub-Step Resolution

The coarse bearing is refined by fitting a Gaussian curve to the three highest RSSI values:

```
θ_fine = θ_peak + (step/2) × (RSSI[peak+1] - RSSI[peak-1]) /
                              (2×RSSI[peak] - RSSI[peak-1] - RSSI[peak+1])
```

This sub-step interpolation typically reduces the RMS error from ±45° to ±5°–10° for a 4-element array with 90° step size.

### 2-Element vs. 4-Element Comparison

| Parameter | 2-Element Cardioid | 4-Element Butler Matrix |
|---|---|---|
| Number of beam positions | 2 (left/right) | 4 (N, E, S, W) |
| Angular resolution (coarse) | 180° | 90° |
| Angular resolution (with Gaussian) | ±15°–20° | ±5°–10° |
| Number of switch states | 2 | 4 |
| Ambiguity | 180° without sense | None with 4 beams |
| Hardware complexity | Low | Moderate |

---

## 1.5 Rotary Loop RDF Theory

### Loop Antenna Figure-8 Pattern

A small loop antenna has a figure-8 (bidirectional) azimuth pattern. The received voltage from a signal at bearing θ (measured from the loop axis) is:

```
V(θ) = V_max × cos(θ)
```

The power pattern is therefore:

```
G(θ) = cos²(θ)
```

This produces a sharp null when the loop plane is aligned with the direction of arrival (loop axis perpendicular to the incoming wave), and a peak when the loop plane is perpendicular to the direction of arrival (loop axis parallel to the signal direction).

In practice, bearing is determined by rotating the loop to find the null (minimum signal), which is much sharper than the peak and thus gives a more precise bearing. The null occurs at θ = 90° from the transmitter direction.

### Null Bandwidth and Bearing Resolution

The null depth and sharpness determine bearing resolution. For an ideal loop with null depth D (in dB below the peak):

```
Null half-angle = arcsin(10^(-D/20))
```

For a 30 dB null (which is achievable with a well-constructed loop), the null half-angle is:

```
θ_null = arcsin(10^(-30/20)) = arcsin(0.0316) ≈ 1.8°
```

This means bearing to within ±2° is theoretically achievable. In practice, site effects, loop asymmetry, and ground proximity degrade the null to 20–25 dB, giving a practical bearing resolution of ±3°–5°.

### Q Factor Effect on Null Sharpness

The loop's Q factor affects its frequency selectivity and sensitivity but does not directly affect the null sharpness of the pattern. However, a high-Q loop (Q > 50) will have a narrow bandwidth, requiring careful tuning to keep the loop resonant on the operating frequency. The null depth is primarily determined by the physical symmetry of the loop construction and its feed system, not the Q.

A practical Q of 50–150 at HF frequencies is typical for a well-constructed air-core loop. Higher Q improves sensitivity at a given frequency but narrows the usable bandwidth.

### Wheeler Formula for Small Loop Inductance

The inductance of a square single-turn loop of side length a (in meters) made from wire of radius r (in meters) is given by the Wheeler approximation:

```
L = (2 × μ₀ × a / π) × (ln(a/r) - 0.774)
```

For the 400mm square loop made from 4mm (radius = 2mm) copper tube:

```
a = 0.400 m
r = 0.002 m
L = (2 × 4π×10⁻⁷ × 0.400 / π) × (ln(0.400/0.002) - 0.774)
  = (3.2×10⁻⁷) × (ln(200) - 0.774)
  = (3.2×10⁻⁷) × (5.298 - 0.774)
  = (3.2×10⁻⁷) × 4.524
  = 1.45 µH
```

This sets the tuning capacitor requirement: for 7 MHz, the required capacitance is:

```
C = 1 / ((2πf)² × L)
  = 1 / ((2π × 7×10⁶)² × 1.45×10⁻⁶)
  = 356 pF
```

This is in the practical range for variable capacitors and varactor-based tuning.

### Tuning Requirement

To operate the loop as a receiving antenna with maximum sensitivity, the loop must be resonated at the operating frequency by a series or parallel capacitor. Resonance increases the voltage at the amplifier input by a factor equal to the Q of the circuit.

The tuning capacitor range must cover all operating frequencies. For 160M–15M (1.8–21 MHz), the required capacitance range is approximately:

| Band | Frequency | Required C |
|---|---|---|
| 160M | 1.85 MHz | 5090 pF |
| 80M | 3.65 MHz | 1307 pF |
| 40M | 7.15 MHz | 341 pF |
| 30M | 10.12 MHz | 170 pF |
| 20M | 14.2 MHz | 86 pF |
| 17M | 18.1 MHz | 53 pF |
| 15M | 21.2 MHz | 39 pF |

This range (39–5090 pF) is too wide for a single varactor. The implementation uses a bank of switched fixed capacitors (relay-switched) to select the approximate range, with a varactor (BB112, Cmin ≈ 10 pF, Cmax ≈ 100 pF) for fine tuning. The ESP32 tunes automatically by sweeping the varactor voltage and finding peak RSSI via the AD8307 detector.

### Motor Drive

The loop is rotated by a NEMA 17 stepper motor (200 steps/revolution) connected to the loop shaft through a 3:1 GT2 belt reduction. This gives:

```
Angular resolution = 360° / (200 × 3) = 0.6°/step
```

With DRV8825 microstepping at 1/16 step, the resolution is:

```
0.6° / 16 = 0.0375°/step (approximately 0.038°)
```

The firmware uses 2°/step for the coarse full-scan (360° in 180 steps) and 0.25°/step for fine scanning near the null. Total coarse scan time at 2°/step with 100ms settle time per step is approximately 3 minutes.

### Sense Antenna for 180° Ambiguity Resolution

The loop pattern has two nulls (at θ = ±90° from the loop axis) and produces a bearing modulo 180°. To resolve the 180° ambiguity, a small vertical sense antenna is added. The sense antenna signal is combined with the loop signal in quadrature to produce a cardioid pattern with a single null and a single peak. The transmitter is in the direction of the cardioid peak.

The firmware implements sense resolution by activating a relay to connect the sense antenna through a 90° hybrid (or RC phase shift network) and measuring whether the combined signal increases or decreases. If it increases, the transmitter is in the forward half-space; if it decreases, 180° is added to the null bearing.

---

## 1.6 GPS Integration

### Purpose

The GPS receiver provides two critical functions: absolute geographic position (latitude, longitude, altitude) for multi-point triangulation, and accurate time for time-difference-of-arrival (TDOA) applications (future capability).

### Magnetic Declination Correction

All bearings in this system are reported as **magnetic bearings** (referenced to magnetic North as read by the QMC5883L compass). To convert to **true bearings** (referenced to geographic North, required for mapping and triangulation), the magnetic declination must be applied:

```
True bearing = Magnetic bearing + Magnetic declination
```

The declination is positive (East) when magnetic North is East of true North and negative (West) when magnetic North is West of true North. The firmware computes declination from GPS latitude and longitude using a simplified IGRF (International Geomagnetic Reference Field) lookup table covering the continental United States. For operation outside this area, the user can override with the DECL command.

### PPS Signal

The GPS NEO-8M produces a 1 Pulse Per Second (PPS) output synchronized to UTC within ±30 ns. This PPS is connected to an ESP32 GPIO with an interrupt handler that timestamps each bearing reading. Time-stamped bearings from multiple sites, transmitted via WiFi or stored for later comparison, enable TDOA geolocation (transmitter location from time-of-arrival differences at known receiver positions).

### NMEA Sentence Parsing

The firmware parses the following NMEA sentences from the GPS:

```
$GPGGA — Global Positioning Fix Data
  Fields: UTC time, latitude, N/S, longitude, E/W, fix quality,
          number of satellites, HDOP, altitude, altitude units,
          geoid separation, correction age, differential reference station

$GPRMC — Recommended Minimum Navigation Information
  Fields: UTC time, status (A=active), latitude, N/S, longitude, E/W,
          speed over ground (knots), track angle (degrees true),
          date, magnetic variation, E/W
```

The GGA sentence provides position for declination calculation and triangulation display. The RMC sentence provides date and speed for dead reckoning (future use).

### Haversine Formula

The great-circle bearing from a known position to a target position (for waypoint display) is computed with the Haversine formula:

```
Given: (lat1, lon1) = observer position
       (lat2, lon2) = target position (in radians)

Δlat = lat2 - lat1
Δlon = lon2 - lon1

a = sin²(Δlat/2) + cos(lat1) × cos(lat2) × sin²(Δlon/2)
c = 2 × atan2(√a, √(1-a))
d = R × c    (R = 6371 km)

x = sin(Δlon) × cos(lat2)
y = cos(lat1) × sin(lat2) - sin(lat1) × cos(lat2) × cos(Δlon)
bearing = atan2(x, y) converted to degrees (0–360°)
```

---

## 1.7 Digital Compass Integration

### QMC5883L Sensor

The QMC5883L is a 3-axis Hall-effect magnetometer on a single chip, communicating over I2C at address 0x0D. Configuration parameters:

| Register | Value | Meaning |
|---|---|---|
| MODE | 0x01 | Continuous measurement mode |
| ODR | 0x0C | 200 Hz output data rate |
| RNG | 0x00 | ±2 Gauss full-scale range |
| OSR | 0xC0 | 512 oversample ratio |

The I2C address is fixed at 0x0D. The SDA and SCL lines require 4.7kΩ pull-up resistors to 3.3V.

### Hard-Iron Calibration

Hard-iron distortion is caused by permanently magnetized materials near the sensor (such as the battery, steel screws, or the DC-DC converter inductor). It appears as a fixed offset added to all three axes. The effect is to shift the center of the circular locus (X vs. Y plot during rotation) away from the origin.

**Calibration procedure**: Rotate the entire controller assembly slowly through two complete 360° rotations while the firmware records the minimum and maximum values of the X and Y magnetic field measurements. The hard-iron offsets are:

```
offset_X = (X_max + X_min) / 2
offset_Y = (Y_max + Y_min) / 2
```

The corrected readings are:

```
X_corr = X_raw - offset_X
Y_corr = Y_raw - offset_Y
```

### Soft-Iron Compensation

Soft-iron distortion is caused by magnetically permeable materials near the sensor (such as iron chassis parts). It distorts the circular locus into an ellipse. Full soft-iron correction requires fitting an ellipsoid to the 3D data and computing a 3×3 correction matrix. This is not implemented in the current firmware (noted as a limitation). For most field uses, hard-iron correction alone is sufficient.

### Tilt Compensation

When the compass is not held level, the X and Y field measurements are contaminated by the Z-axis field component. The MPU6050 accelerometer (on the same I2C bus, address 0x68) provides pitch and roll angles for tilt compensation:

```
X_tilt = X_corr × cos(pitch) + Z_corr × sin(pitch)
Y_tilt = Y_corr × cos(roll) + X_corr × sin(roll) × sin(pitch)
           - Z_corr × cos(pitch) × sin(roll)
```

The corrected heading is then:

```
heading = atan2(-Y_tilt, X_tilt)
```

### Magnetic Declination

The heading from the above calculation is magnetic heading. True heading is obtained by adding the magnetic declination (positive East, negative West). The firmware looks up declination from a stored table indexed by integer degrees of latitude and longitude.

**Heading accuracy**: After hard-iron calibration, with tilt compensation, the heading accuracy is typically ±1°–2° in benign environments. Near motors, steel structures, or power lines, errors of ±5°–15° are possible.

---

<a name="chapter-2"></a>
# CHAPTER 2 — SPECIFICATIONS

## 2.1 Watson-Watt HF ADF Specifications

| Parameter | Specification |
|---|---|
| Frequency range | 1.8 MHz – 54 MHz (160M – 6M) |
| Array aperture (λ/4 spacing) | 3.5m (40M) to 81m (160M) |
| Element height | λ/4 monopole; shortened elements acceptable with loss of sensitivity |
| Number of elements | 4 (N, S, E, W) + 1 sense antenna |
| Feed line impedance | 50Ω coaxial |
| Balun type | Current-mode, FT-50-43 toroid, 10 turns trifilar |
| LNA gain | 40 dB (INA128, Rg = 3Ω) |
| ADC sample rate | 10 kHz |
| ADC block size | 512 samples |
| ADC bits | 12 (ESP32 SAR ADC) |
| Bearing update rate | ~20 Hz (one bearing per 512-sample block) |
| Bearing smoothing | 8-reading boxcar average |
| SNR threshold for valid bearing | 10 dB (configurable) |
| Bearing accuracy (good site, cal'd) | ±3°–5° RMS |
| Bearing accuracy (typical field) | ±5°–8° RMS |
| Ambiguity resolution | Sense antenna, cardioid phase comparison |
| WiFi SSID | RF-ADFHF |
| WiFi password | none (open AP) |
| WiFi IP | 192.168.4.1 |
| Bluetooth SPP device name | RF-ADFHFinder |
| Bluetooth baud rate | 115200 (SPP transparent) |
| Controller | ESP32-WROOM-32 |
| OLED | SSD1306 128×64, I2C address 0x3C |
| GPS | u-blox NEO-8M, UART2, 9600 baud |
| Compass | QMC5883L, I2C address 0x0D |
| Power input | 12V DC, 2.1mm center-positive barrel jack |
| Power consumption | 600 mA typical, 1A max (with LNA) |
| Operating temperature | -10°C to +55°C |
| Storage temperature | -20°C to +70°C |
| Dimensions (controller box) | 115mm × 65mm × 30mm (approx.) |
| Weight (controller + cable) | ~450g |

## 2.2 Doppler VHF/UHF ADF Specifications

| Parameter | Specification |
|---|---|
| Bands supported | 2M (144–148 MHz), 1.25M (222–225 MHz), 70cm (420–450 MHz), 33cm (900–928 MHz), 20cm (1240–1300 MHz) |
| Array geometry | 4 elements on circle, λ/2 diameter |
| Array diameter | 1027mm (2M) / 676mm (1.25M) / 330mm (70cm) / 163mm (33cm) / 115mm (20cm) |
| Array disk material | PETG or PLA printed disk, 3mm thickness |
| Element type | 5mm aluminum rod, λ/4 length |
| Element length by band | 480mm (2M) / 308mm (1.25M) / 156mm (70cm) / 77mm (33cm) / 55mm (20cm) |
| Switch IC | FST3253 (below 400 MHz), PE4259 cascaded (above 400 MHz) |
| Switching rate | 1000 Hz (2M/1.25M/70cm), 2000 Hz (33cm/20cm) |
| ADC sample rate | 10 kHz (I/F audio input) |
| Goertzel block size | 1024 samples |
| Bearing update rate | ~10 Hz |
| Bearing smoothing | 4-reading average |
| SNR threshold | 15 dB (FM discriminator output) |
| Bearing accuracy (SNR >30 dB) | ±0.5°–1° RMS |
| Bearing accuracy (SNR 20 dB) | ±1°–3° RMS |
| Bearing accuracy (SNR 10 dB) | ±5°–10° RMS |
| Signal type required | FM (voice, data, tone) |
| Ambiguity | None (full 0–360°) |
| WiFi SSID | RF-ADFVHF |
| Bluetooth device name | RF-ADFVHFinder |
| Controller | ESP32-WROOM-32 |
| Power input | 12V DC |
| Power consumption | 400 mA typical |
| Tripod mount | 1/4"-20 or 3/8"-16 female insert in disk |

## 2.3 Rotary Loop Specifications

| Parameter | Specification |
|---|---|
| Bands | 160M – 15M (1.8 – 21.5 MHz) |
| Loop shape | Square |
| Loop conductor | 4mm copper tube (or 4mm heavy copper wire) |
| Loop side length | 400mm |
| Loop perimeter | 1600mm |
| Loop inductance | ~1.45 µH (Wheeler formula) |
| Loop Q (air-core, HF) | 50–150 typical |
| Tuning method | Switched capacitor bank + varactor BB112 |
| Capacitance range | 39–5090 pF (all HF bands) |
| Tuning voltage | 0–3.3V DAC (ESP32 internal DAC) |
| Faraday shield | Copper foil tape, 10mm gap at top |
| Buffer amplifier | J310 JFET common-source, gain ~15 dB |
| Motor type | NEMA 17, 200 steps/rev, 1.5A, 12V |
| Motor driver | DRV8825, 1/16 microstepping |
| Gear reduction | 3:1 GT2 belt, 20T motor pulley, 60T loop pulley |
| Step resolution | 0.0375°/step (1/16 microstep) |
| Coarse scan step | 2° |
| Fine scan step | 0.25° |
| Full scan time (coarse) | ~3 minutes |
| Position feedback | AS5048A absolute magnetic encoder (12-bit, 0.088°/count) or potentiometer |
| Sense antenna | Short vertical whip, relay-switched, RC phase shift |
| Null depth (free space) | >30 dB (ideal) |
| Null depth (field) | 20–25 dB typical |
| Bearing accuracy | ±2°–5° (field conditions) |
| WiFi SSID | RF-ADFLOOP |
| Bluetooth device name | RF-ADFLOOPFinder |
| Power input | 12V DC |
| Power consumption | 800 mA (motor running), 300 mA (idle) |
| Dimensions (base plate) | 200mm × 200mm |
| Mass (complete system) | ~1.2 kg |

## 2.4 GPS/Compass Subsystem Specifications

| Parameter | Specification |
|---|---|
| GPS module | u-blox NEO-8M (or NEO-6M pin-compatible) |
| GPS interface | UART, 9600 baud, 3.3V TTL |
| GPS protocol | NMEA 0183 v4.10 |
| GPS NMEA sentences | GGA, RMC, GSA (parsed); others ignored |
| GPS position accuracy | 2.5m CEP (without SBAS) / 1.5m CEP (with SBAS) |
| GPS cold start TTFF | <60 seconds (typical) |
| GPS hot start TTFF | <2 seconds |
| GPS PPS output | 1Hz, 3.3V TTL, rising edge on second |
| PPS accuracy | ±30 ns (locked) |
| GPS antenna | Active patch, 28 dBi typical, 3.3V LNA bias via 33Ω series resistor |
| Compass module | QMC5883L |
| Compass interface | I2C, 400 kHz, address 0x0D |
| Compass field range | ±8 Gauss full-scale (±2G selected) |
| Compass sensitivity | 3000 LSB/Gauss (±2G range) |
| Compass output rate | 200 Hz (OSR 512, 10 Hz effective after averaging) |
| Compass heading accuracy | ±1°–2° (after hard-iron cal, level) |
| Compass heading accuracy (tilt) | ±2°–4° (tilt-compensated with MPU6050) |
| Declination model | Simplified IGRF, CONUS table, ±1° accuracy |
| I2C bus | SDA GPIO21, SCL GPIO22, 3.3V, 4.7kΩ pull-ups |

---

<a name="chapter-3"></a>
# CHAPTER 3 — CONTROLS AND INDICATORS

## 3.1 Watson-Watt HF ADF Controls

### Front Panel Layout

The controller front panel provides:

- **SSD1306 OLED display** (128×64 pixels, 4 text lines plus status icons)
- **Rotary encoder with push switch** (band select and menu navigation)
- **Power LED** (green, 3mm, connected to 3.3V rail via 1kΩ)
- **Bluetooth status LED** (blue, 3mm, driven by firmware GPIO)

### OLED Display Layout

```
Line 1 (top):    [MODE]    [BAND]    e.g., "ADF-HF   40M"
Line 2:          BRG: 247°M (236°T)
Line 3:          SNR: 18.3dB  CONF:HI
Line 4 (bottom): GPS:3D  CMPS:OK  BT:ON
```

| Line | Content | Notes |
|---|---|---|
| Line 1 | Operating mode and active band | MODE = ADF-HF, ADF-PA (phased array), or SCAN |
| Line 2 | Magnetic bearing and true bearing | True bearing = magnetic + declination |
| Line 3 | Signal SNR in dB and confidence level | Confidence: HI (>20dB), MED (10–20dB), LO (<10dB), INVALID |
| Line 4 | GPS fix status, compass status, BT status | GPS: NO FIX, 2D, 3D; CMPS: OK, CAL, ERR; BT: OFF, ON |

### Rotary Encoder Functions

| Action | Function |
|---|---|
| Rotate clockwise | Step up through band table |
| Rotate counterclockwise | Step down through band table |
| Short press (<1 second) | Enter manual bearing offset entry mode |
| Long press (>2 seconds) | Enter main menu |
| Double-click | Toggle between ADF-HF and PHASED ARRAY mode |

### LED Indicators

| LED | State | Meaning |
|---|---|---|
| Power (green) | Solid on | 3.3V rail present, ESP32 running |
| Power (green) | Blinking 1 Hz | Initializing (first 5 seconds after power-up) |
| Power (green) | Off | No power |
| Bluetooth (blue) | Off | Bluetooth not connected |
| Bluetooth (blue) | Solid on | Bluetooth SPP session active |
| Bluetooth (blue) | Blinking 2 Hz | Data being transmitted or received |
| Bluetooth (blue) | Blinking 0.5 Hz | Advertising, waiting for connection |

### Band Table (Watson-Watt HF)

| Band Index | Band Name | Frequency | Element Length (λ/4) | Array Baseline |
|---|---|---|---|---|
| 0 | 160M | 1.85 MHz | 40.5m | 40.5m |
| 1 | 80M | 3.65 MHz | 20.5m | 20.5m |
| 2 | 60M | 5.33 MHz | 14.1m | 14.1m |
| 3 | 40M | 7.15 MHz | 10.5m | 10.5m |
| 4 | 30M | 10.12 MHz | 7.4m | 7.4m |
| 5 | 20M | 14.2 MHz | 5.3m | 5.3m |
| 6 | 17M | 18.1 MHz | 4.1m | 4.1m |
| 7 | 15M | 21.2 MHz | 3.5m | 3.5m |
| 8 | 12M | 24.9 MHz | 3.0m | 3.0m |
| 9 | 10M | 28.5 MHz | 2.6m | 2.6m |
| 10 | 6M | 50.5 MHz | 1.5m | 1.5m |

---

## 3.2 Doppler VHF ADF Controls

### OLED Display Layout (VHF Mode)

```
Line 1:    ADF-VHF  2M  146.520
Line 2:    BRG: 127°M (116°T)
Line 3:    RSSI: -82dBm  SNR:22dB
Line 4:    GPS:3D  CMPS:OK  BT:ON
```

| Line | Content |
|---|---|
| Line 1 | Mode, band, and frequency in MHz |
| Line 2 | Magnetic and true bearing |
| Line 3 | RSSI from AD8307 in dBm, and SNR estimate |
| Line 4 | GPS/compass/BT status (same as HF system) |

### Web Interface (VHF)

The VHF web interface provides:

- SVG compass rose with bearing needle (auto-refreshed via fetch() every 500ms)
- Band selection buttons (2M, 1.25M, 70cm, 33cm, 20cm)
- RSSI bar graph
- Signal quality indicator (Excellent / Good / Fair / Weak)
- JSON data endpoint at GET /api/status

---

## 3.3 Rotary Loop Controls

### OLED Display Layout (Loop Mode)

```
Line 1:    ADF-LOOP  40M  7.150
Line 2:    POS: 183.5°  NULL: 047°
Line 3:    DEPTH: 22dB  CONF:MED
Line 4:    GPS:3D  MOTOR:IDLE  BT:ON
```

| Line | Content |
|---|---|
| Line 1 | Mode, band, frequency |
| Line 2 | Current motor position and last-found null bearing |
| Line 3 | Null depth in dB and confidence level |
| Line 4 | GPS status, motor status (SCAN/IDLE/HOMING), BT status |

### Web Interface (Loop)

The loop web interface provides:

- SVG compass rose with needle and superimposed figure-8 pattern overlay showing loop orientation
- SCAN button: start full 360° scan
- STOP button: halt scan at current position
- HOME button: return loop to 0° reference position
- TUNE button: start auto-tune sequence (find varactor setting for peak RSSI)
- SENSE ON / SENSE OFF toggle: enable/disable sense antenna
- Band selector grid
- Current position readout and null bearing readout

---

## 3.4 CYD Touchscreen (All Systems)

The CYD (ESP32-2432S028R) acts as a Bluetooth SPP master and connects to the nearest available ADF device. It provides a 320×240 pixel color touchscreen display with four operating modes selected by a tab bar at the bottom of the screen.

### Tab Bar

```
[ CMPS ]  [ MAP ]  [ BAND ]  [ SET ]
```

### MODE_COMPASS

Full-screen compass rose, 280-pixel diameter, with:

- Compass rose ring showing cardinal and intercardinal points
- Bearing needle (red, 3-pixel width) pointing to bearing
- True North indicator (blue, N label)
- Magnetic North vs. True North offset indicator (small arc, labeled with declination value)
- Signal strength arc (green arc from 0° to scaled RSSI angle, at inner radius)
- Center display: bearing value in degrees, confidence level, band name
- Background: dark gray (#1A1A1A)
- Rose ring: white (#FFFFFF)
- Bearing needle: red (#FF2020)

### MODE_MAP

Reserved for future implementation. Current display: "MAP VIEW — FUTURE FEATURE" with GPS coordinates and last bearing.

### MODE_BANDS

Grid of band buttons, 11 HF + 5 VHF, arranged 4 per row:

```
[160M] [80M ] [60M ] [40M ]
[30M ] [20M ] [17M ] [15M ]
[12M ] [10M ] [6M  ] [    ]
[2M  ] [1.25M][70cm] [33cm]
[20cm] [    ] [    ] [    ]
```

Active band highlighted in yellow (#FFFF00) on dark background. Inactive bands in gray. Touch to select band (sends BAND n command via BT).

### MODE_SETTINGS

Settings screen providing:

- BT target selector: list of discovered ADF devices, tap to connect
- Declination override: ±30° range, +/- buttons
- Display brightness slider: 25%, 50%, 75%, 100%
- Compass calibration start button (sends CAL START to connected ADF)
- Software version display
- WiFi IP address of connected ADF device

---

## 3.5 WiFi Web Interface

All three ADF systems host a WiFi access point and HTTP server. The web interface is served as a single HTML page with embedded JavaScript and SVG.

### Compass Rose SVG

The compass rose is rendered as a scalable vector graphic (SVG) with:

- Outer ring with degree markings every 10°, labels every 30°
- Cardinal point labels (N, E, S, W) in larger font
- Intercardinal labels (NE, SE, SW, NW) in medium font
- Bearing needle: a red triangle rotated to the current bearing
- Center hub circle
- Optional second needle for true North offset

### JavaScript Auto-Refresh

The bearing display updates without page reload using:

```javascript
setInterval(function() {
    fetch('/api/bearing')
    .then(response => response.json())
    .then(data => {
        updateCompassNeedle(data.bearing_mag);
        document.getElementById('brg_text').textContent =
            data.bearing_mag.toFixed(1) + '° M  (' +
            data.bearing_true.toFixed(1) + '° T)';
        document.getElementById('snr_text').textContent =
            'SNR: ' + data.snr.toFixed(1) + ' dB';
    });
}, 500);
```

### HTTP API Endpoints

| Endpoint | Method | Request Body | Response | Description |
|---|---|---|---|---|
| / | GET | none | HTML page | Main web interface |
| /api/bearing | GET | none | JSON | Current bearing (HF/loop) |
| /api/status | GET | none | JSON | Full status (all systems) |
| /api/band | POST | {"band": n} | JSON ack | Set active band |
| /api/cal | POST | {"action": "start"/"end"} | JSON ack | Compass calibration |
| /api/decl | POST | {"decl": f} | JSON ack | Set declination override |
| /api/scan | POST | {"action": "scan"/"stop"/"home"} | JSON ack | Loop motor control |
| /api/tune | POST | none | JSON ack | Start loop auto-tune |

### JSON Response Format (GET /api/status)

```json
{
  "bearing_mag": 247.3,
  "bearing_true": 236.8,
  "snr_db": 18.3,
  "rssi_dbm": -82.1,
  "confidence": "HIGH",
  "band": "40M",
  "frequency_mhz": 7.15,
  "gps_lat": 37.3022,
  "gps_lon": -120.4830,
  "gps_fix": "3D",
  "gps_sats": 9,
  "compass_heading": 358.7,
  "declination": -13.5,
  "uptime_s": 1847,
  "bt_connected": true,
  "fw_version": "1.0.0"
}
```

---

<a name="chapter-4"></a>
# CHAPTER 4 — INSTALLATION

## 4.1 Watson-Watt Array Deployment

### Site Selection

The quality of a Watson-Watt array site has a major impact on bearing accuracy. Select sites that meet the following criteria:

1. Flat terrain within a radius of at least 2× the array aperture in all directions.
2. No metallic fences, pipes, towers, vehicles, or structures within 2× the array aperture.
3. No power lines overhead or within 1× the aperture (power line hum induces common-mode interference).
4. Consistent ground conductivity (avoid half-ground, half-pavement splits within the array).
5. Clear sky view for GPS (not required for RDF function, but needed for positioning).

Conduct a site survey by rotating a portable receiver antenna and listening for signal reflections or sudden bearing shifts. If more than ±5° bearing variation is observed while slowly walking around the site perimeter, select a different site.

### Element Alignment

1. Identify magnetic North with the system compass. Note the current declination value displayed by the firmware.
2. Drive the center stake at the selected center point.
3. Using a compass (corrected for declination to get true North/South), pace out the array baseline distance in each cardinal direction.
4. Drive the N, S, E, and W stakes at these positions. Verify alignment: N-S pair must be co-linear; E-W pair must be co-linear and perpendicular to N-S.
5. Stake alignment error: each element must be within ±2° of its nominal bearing from center, and within ±5% of its nominal distance from center.

### Element Installation

1. Attach the printed stake cup to the top of each driven ground stake.
2. Insert the element rod (aluminum, 6mm diameter, length per band table) into the stake cup.
3. Tighten the M3 set screw to secure the rod. The rod must be vertical: check with a small level.
4. For telescoping elements, extend to the required length and tighten the locking collar.
5. Connect the coax feed line to the SO-239 connector at the element base. Dress the coax to prevent it from touching the element rod.

### Feed Line Phase Matching

Feed lines from each element to the central junction box must be cut to identical electrical length.

1. Cut five lengths of RG-58 coax (4 element lines + 1 sense line). Starting length: add 20% to the measured physical distance from the farthest element to the center box.
2. Using a NanoVNA, measure the electrical length (phase of S11 with end open) for each piece of coax.
3. Trim the longer pieces until all four element lines match within ±5°.
4. Do not bend the coax sharply (minimum bend radius: 50mm for RG-58).
5. Route feed lines radially from the center, lying flat on the ground. Secure with cable stakes every 2m.
6. Label all coax: N, S, E, W, SENSE at both ends with heat-shrink labels.

### Junction Box

1. Place the junction box at the array center, next to the center ground stake.
2. Connect element coax cables to the N, S, E, W, and SENSE inputs as labeled on the PCB.
3. Connect the output coax (to controller box) to the single SO-239 output connector.
4. Bond the junction box metal enclosure to the center ground stake with a short #12 AWG ground wire.
5. Route controller output coax to the controller box location. This cable may be any length up to 10m without phase concerns (it carries the single-ended combined signal).

### Phase Matching Verification

1. Connect the NanoVNA CH0 port to the junction box output.
2. Place a short-circuit jumper at the far end of the North element coax.
3. Note the phase angle displayed at the operating frequency.
4. Repeat for South, East, and West elements.
5. Phase must agree within ±5° for all four elements.
6. If phase mismatch exceeds ±5°, re-trim or replace the offending coax.

---

## 4.2 Doppler Ring Array Mounting

### Printed Disk

The Doppler ring disk is a printed part (PETG or PLA, 3mm thickness) with the following features:

- Central 1/4"-20 female brass insert (for tripod mounting)
- Optional 3/8"-16 adapter insert
- 4 element posts at 90° intervals at the correct diameter for the selected band
- Element posts: M5 female thread, 20mm depth
- Center coax routing channel: 25mm diameter bore for RG-316 or RG-174
- Switch PCB mounting boss: 4× M2.5 standoffs in center

### Tripod and Mast Installation

1. Thread the tripod head stud (1/4"-20) into the disk center insert and tighten snugly.
2. Insert a 1-inch (25mm) aluminum tube through the center bore, extending 1m above the disk.
3. Secure the tube with the mast clamp printed onto the disk underside.
4. Elevate the assembled disk to at least 1m above the tripod apex (total height 2–3m above ground).
5. Level the disk: use a small bubble level placed on the disk surface. Adjust tripod legs until level.
6. Orient element 1 (labeled "N") toward magnetic North using a compass.

### Element Installation

1. Screw the element rod (5mm aluminum) into the element post, threading it 10mm into the post for secure seating.
2. Element rod lengths (pre-cut, one set per band):

| Band | Element Length | Total with post |
|---|---|---|
| 2M | 480mm | 500mm |
| 1.25M | 308mm | 328mm |
| 70cm | 156mm | 176mm |
| 33cm | 77mm | 97mm |
| 20cm | 55mm | 75mm |

3. All four elements in a set must be the same length. Verify with a steel rule before installation.

### Switch PCB and Coax

1. Install the switch PCB (FST3253-based) on the center boss standoffs. Secure with M2.5 × 6mm screws.
2. Connect the four element coax stubs (50Ω, 30mm each) from the PCB to the element post SMA connectors.
3. Route the RG-316 output coax down through the center mast tube to the receiver.
4. At the receiver, terminate in the appropriate connector (BNC for most receivers, SMA for SDRs).
5. Connect the switch control cable (4-wire, to ESP32 GPIOs for switch select lines) from the PCB down the mast exterior. Secure with cable ties every 300mm.

### Elevation

For best VHF/UHF performance, the array should be elevated to at least 1m above surrounding vegetation and terrain. A height of 2–3m is preferred. At low elevation, ground reflections can degrade bearing accuracy by ±5°–10°.

---

## 4.3 Rotary Loop Installation

### Base Mount

1. Set the base mount (printed or machined, 200mm × 200mm) on a level, stable surface (table, plywood sheet on ground, or tripod via M8 adapter bolt).
2. Verify level with a bubble level in two perpendicular orientations. Adjust table or shim base until level within ±1°.
3. Secure the base to the surface with at least two M5 screws or C-clamps.

### Motor Installation

1. Align the NEMA 17 motor (flange side up) with the 4 mounting holes on the underside of the base plate.
2. Insert M3 × 10mm socket-head screws through the base and thread into the motor flange.
3. Tighten evenly in a cross pattern. Final torque: 0.5 N·m (5 kgf·cm).
4. Verify the motor shaft protrudes upward through the base center bore.
5. Press the 20T GT2 pulley onto the motor shaft, key engaged. Tighten M3 set screw with shaft flat engaged.

### Loop Frame and Shaft

1. Construct the square loop from 4mm copper tube. Cut four 400mm sections and four 90° elbows.
2. Solder all joints using electrical (non-plumbing) solder and flux. Ensure all joints are electrically and mechanically sound.
3. Verify electrical continuity around the loop with a DMM: resistance should be less than 0.5Ω.
4. Mount the loop shaft adapter bracket to the top center of the loop using M3 stainless screws and lockwashers.
5. Thread the shaft (8mm aluminum, 150mm length) through the bearing block in the base plate center.
6. Press the 60T GT2 pulley onto the loop shaft, and align with motor pulley. Tighten M3 set screw.
7. Install the GT2 belt (300mm) around both pulleys. Belt tension: deflect 5mm with light finger pressure at mid-span.

### Faraday Shield

A Faraday shield (electrostatic shield) prevents capacitively-coupled electric field interference from contaminating the magnetic field measurement. The shield must be open (not a closed conductor) to avoid short-circuiting the magnetic flux.

1. Wrap copper foil tape (25mm width) around the outer surface of the copper tube loop.
2. Begin wrapping at the center of the bottom rail of the loop. Wrap completely around the three remaining sides.
3. Leave a 10mm gap at the top center of the top rail. This gap prevents the shield from acting as a shorted single-turn loop that would cancel the loop's signal.
4. Ensure the foil tape is continuous (no gaps other than the intentional top gap) and well-adhered to the copper tube.
5. Solder a ground wire from one end of the shield to the common ground of the buffer amplifier PCB.

### Buffer Amplifier

1. Fabricate or obtain the J310 JFET buffer amplifier PCB (see schematic in firmware repository).
2. Mount the PCB in the weatherproof enclosure at the base of the loop, near the feed point.
3. Connect the loop feed terminals to the amplifier input using two short (50mm) leads.
4. Ensure the input connections are balanced (equal capacitance to ground from each feed terminal to prevent conversion of electric field pickup to differential voltage).
5. Connect the amplifier output to a length of RG-316 coax, routed down through the hollow shaft center bore to the controller box.
6. The RG-316 coax inner conductor may be used to supply 12V DC to the buffer amplifier (bias tee arrangement: 33Ω + 47µH RF choke, 100µF bypass on amplifier end).

### AS5048A Position Encoder (Optional)

If using the AS5048A absolute magnetic encoder for loop position:

1. Attach the diametrically magnetized magnet to the top of the loop shaft (center of shaft face, adhesive + M3 screw).
2. Mount the AS5048A PCB on a standoff 2mm above the magnet surface, centered. Secure with M2 screws.
3. Connect I2C lines (SDA, SCL) and 3.3V/GND to the ESP32.
4. Verify operation: in firmware monitor, issue STATUS command and confirm position reads 0°–360° without discontinuity through full rotation.

---

## 4.4 Controller Box Installation

### Desktop Installation

1. Place the controller box on a stable, level surface within 1m of the antenna or coax feed point.
2. Ensure minimum clearance of 100mm on all sides for ventilation.
3. Connect the RF coax from the antenna/junction box to the RF IN connector on the controller front panel.
4. Connect the GPS antenna (active patch) to the GPS SMA connector. Route antenna for clear sky view.
5. Connect the compass module (if external) to the I2C header on the rear panel.
6. Connect power: 12V DC, 2.1mm center-positive barrel connector. Power supply rating: minimum 1.5A.

### Cable Routing

1. Route RF coax away from motor lead cables, switch control cables, and power cables. Minimum separation: 100mm. Cross at 90° when unavoidable.
2. GPS antenna cable: keep away from RF coax and motor cables. GPS cable may run parallel to power cable.
3. Motor cable (loop system): use shielded cable (overall braid shield, grounded at controller end only). Unshielded motor cables radiate significant RFI.
4. Tie cables with cable ties. Do not over-tighten (maximum loop stress: 10N on coax).

### GPS Antenna Placement

1. The active GPS patch antenna must have a clear view of the sky. Best placement: horizontal surface outdoors, facing up.
2. A minimum of 120° unobstructed view cone is required for a reliable 3D GPS fix.
3. Do not place the GPS antenna near metal surfaces (roofs, vehicles) that may block signals or cause multipath.
4. The GPS antenna cable may be extended up to 5m with RG-316 without significant signal loss. The LNA bias voltage (3.3V via 33Ω) must remain above 2.5V at the antenna end; verify with DMM if extension cable is used.

### Compass Orientation

1. The QMC5883L sensor must be oriented with its X-axis pointing toward the front of the controller (the side with the RF IN connector and OLED display).
2. If the compass is mounted on the ESP32 carrier PCB, ensure the PCB is mounted horizontally in the enclosure with the X-axis label aligned with the front of the enclosure.
3. If the compass is on a remote header (for use away from the motor and RF components), the same X-axis orientation applies.
4. Note and mark the compass orientation on the enclosure exterior with a small arrow label.

---

## 4.5 Power Requirements

### Voltage and Current

| System | Nominal Voltage | Typical Current | Peak Current | Minimum Supply |
|---|---|---|---|---|
| Watson-Watt HF | 12V DC | 600 mA | 1.0 A | 12V, 1.5A |
| Doppler VHF | 12V DC | 400 mA | 700 mA | 12V, 1.0A |
| Rotary Loop | 12V DC | 300 mA (idle) | 800 mA (motor) | 12V, 1.5A |

### Power Connector

All systems use a 2.1mm center-positive barrel jack (industry standard). The power supply must be regulated (ripple <100 mV at full load).

### Internal Regulation

The controller PCB includes:
- AMS1117-3.3 LDO (3.3V, 800 mA) for ESP32, OLED, compass, GPS
- 78L05 (5V, 100 mA) for optional logic-level accessories
- Motor driver (loop system) powered directly from 12V rail

### Power Sequencing

WARNING: Always follow this power-up sequence to avoid damage or false readings:

1. Verify all coax and antenna connections are made before applying power.
2. Apply 12V DC to the controller box. Wait for OLED splash screen.
3. For the loop system: do not move the loop manually while power is applied.
4. For the HF system: do not transmit RF into the array feed while the system is powered (receive only).
5. Do not use the USB micro-B connector for operational power. The USB port is for programming only. Operating the ESP32 from USB power while the 12V supply is also connected may cause damage to the AMS1117 regulator.

---

<a name="chapter-5"></a>
# CHAPTER 5 — OPERATING PROCEDURES

## 5.1 System Power-Up (All Systems)

Perform the following steps for any ADF system:

1. Verify all RF coax connections are secure. Check that coax connectors are fully seated and finger-tight. No open coax ends should be exposed.
2. Verify GPS antenna has clear sky view and is connected to the GPS SMA port.
3. Apply 12V DC power. The OLED will illuminate within 1 second with the splash screen: "ADF SUITE v1.0 / TM-ADF-001".
4. **Loop system only**: The motor will home the loop to the 0° reference position. This takes approximately 5 seconds. Do not obstruct the loop during homing.
5. After 3 seconds, the OLED switches to the operational display. The current mode and band are shown on Line 1.
6. The WiFi AP broadcasts within 5 seconds. SSID: "RF-ADF[type]" (e.g., "RF-ADFHF"). The AP is open (no password). IP address: 192.168.4.1.
7. Connect a phone or laptop to the WiFi AP. Open a browser and navigate to http://192.168.4.1 to access the web interface.
8. The Bluetooth SPP adapter begins advertising. Use a Bluetooth terminal app (e.g., Serial Bluetooth Terminal for Android) to connect to "RF-ADF[type]Finder". Baud rate setting is not applicable for SPP.
9. The CYD display (if present) powers on independently. It scans for and connects to the nearest ADF Bluetooth device automatically. When connected, the BT LED on the CYD flashes once and then stays steady.
10. GPS fix acquisition begins immediately. Cold start requires up to 60 seconds for first fix. OLED Line 4 shows "GPS:SRCH" until acquired, then "GPS:2D" or "GPS:3D".
11. **Compass calibration required on first use**, or after transporting the system to a new location. Proceed to Section 5.2.

---

## 5.2 Compass Calibration

Perform compass calibration at initial deployment and after any significant change in the operating environment (new location, new enclosure materials, or suspected ferrous material changes near the compass).

1. Issue the calibration start command via Bluetooth or web interface: `CAL START`
2. The OLED shows "CAL MODE — ROTATE SLOWLY". The bearing display suspends.
3. Slowly rotate the entire controller assembly (the compass must move with it) through two complete 360° rotations. Rotate about the vertical axis. Keep the unit level during rotation.
4. Rotation speed: approximately 60 seconds for two complete rotations (30°/second). Do not rotate faster; rapid rotation may cause the magnetometer to report erroneous values due to settling time.
5. Move away from any large metal objects, motors, or electronic equipment during calibration.
6. After completing the rotations (approximately 60 seconds), issue: `CAL END`
7. The firmware computes hard-iron offsets (X_offset, Y_offset) and saves them to ESP32 NVS (Non-Volatile Storage). NVS is retained through power cycles.
8. The OLED returns to the normal operational display. Line 4 shows "CMPS:OK" if calibration succeeded.
9. Verify calibration: point the system toward a known compass bearing (use a baseplate compass as a reference). The OLED magnetic heading should agree with the reference within ±3°.
10. If agreement is worse than ±5°, suspect magnetic interference in the immediate environment or a compass sensor failure. Check Section 9.4 Fault Codes.

---

## 5.3 Watson-Watt Bearing Acquisition

Prerequisites: Array deployed and aligned per Section 4.1. System powered up and GPS acquired.

1. Deploy the Adcock array per Section 4.1. Verify all four feed lines are connected and labeled.
2. Apply power per Section 5.1.
3. Select the band of the target signal. Use the encoder: rotate to the desired band on the OLED Line 1 display. Or, via Bluetooth: `BAND 3` (for 40M, for example). Or, via the web interface: tap the 40M button.
4. Tune the connected receiver (external, connected to the junction box output) to the target frequency. The system does not tune a receiver; it processes the IF or audio output.
5. Monitor the OLED Line 3 for SNR. Valid bearing requires SNR > 10 dB. If SNR is less than 10 dB, the signal is too weak or absent; the bearing display will show "INVALID" or dashes.
6. When the target signal is present with sufficient SNR, the bearing is displayed automatically on Line 2: "BRG: XXX°M (YYY°T)".
7. Read the magnetic bearing (M) for use with a magnetic compass. Read the true bearing (T) for map plotting and triangulation.
8. Observe the CONF indicator (Line 3): HI = high confidence; MED = moderate; LO = low; INVALID = no valid bearing.
9. Apply magnetic declination to convert: TRUE = MAGNETIC + DECLINATION (East declination is positive). The firmware does this automatically; verify the displayed declination value is correct for your location.
10. Record the bearing and GPS position. For triangulation, at least two positions are required (see Section 5.6).
11. Take at least 3 readings spaced 30 seconds apart and average them to reduce site multipath effects.

---

## 5.4 Doppler VHF Bearing Acquisition

Prerequisites: Doppler ring array mounted and aligned per Section 4.2. Receiver connected to array output coax.

1. Mount the Doppler ring array on tripod, 2–3m above ground. Orient element 1 (N) toward magnetic North.
2. Apply power to the controller per Section 5.1.
3. Select the correct band for the target frequency using encoder or `BAND n` command. The firmware sets the switching rate and expects elements of the correct length for the selected band.
4. Tune the FM receiver to the target frequency. Connect the receiver discriminator audio output to the ESP32 ADC input (GPIO34) via the 3.5mm audio jack on the front panel. Maximum audio input level: 1 Vpp (attenuate if required).
5. Set receiver squelch just below the target signal level. A fully-open squelch on a noisy band will produce random bearings.
6. Monitor OLED Line 3 RSSI. Valid bearing acquisition typically requires RSSI > -95 dBm for the 2M band.
7. The bearing is displayed on Line 2 when the Goertzel algorithm detects a valid 1 kHz phase reference. Typical bearing update rate: 10 Hz.
8. The Doppler system gives a direct magnetic bearing, no ambiguity resolution needed. The bearing is continuously updated as long as the signal is present.
9. For best accuracy, wait for at least 5 seconds of stable readings before recording. Bearing jitter (standard deviation) with good signal is typically < ±2°.
10. Record bearing and GPS position for triangulation.

---

## 5.5 Rotary Loop RDF Operation

Prerequisites: Loop system assembled and installed per Section 4.3. System powered up per Section 5.1.

1. Level the loop base on a stable surface. Verify motor home position (loop at 0° = North reference mark on shaft coupling).
2. Select the operating band: `BAND n` or use encoder. The firmware sets the correct varactor DAC voltage range for the selected band.
3. Issue the TUNE command: `TUNE` (or tap TUNE button on web interface). The firmware sweeps the varactor voltage from minimum to maximum, measuring RSSI at each step via the AD8307 detector, and sets the varactor to the voltage that produces maximum RSSI. This auto-tune process takes 3–8 seconds depending on band.
4. Issue the SCAN command: `SCAN` (or tap SCAN button). The motor begins rotating the loop at 2°/step with 100ms settle time per step. A full 360° scan takes approximately 3 minutes.
5. During the scan, the OLED updates with the current motor position (Line 2) and the best null candidate bearing found so far.
6. At scan completion, the firmware displays the null bearing on Line 2 and the null depth on Line 3.
7. Verify null depth is > 15 dB. If null depth is less than 15 dB, the bearing may be unreliable. Proceed with caution.
8. If null depth is > 15 dB, resolve the 180° ambiguity with the sense antenna:
   a. Issue: `SENSE ON`
   b. The firmware momentarily rotates the loop to the null bearing, then adds the sense antenna signal.
   c. The OLED displays the resolved bearing (single 0–360° value) and marks it with "(SENSE)".
   d. Issue: `SENSE OFF` after recording the bearing.
9. The transmitter is in the direction of the resolved bearing ± the bearing accuracy of the system.
10. Record the bearing and GPS position.

---

## 5.6 Multi-Point Triangulation

Multi-point triangulation determines the location of a transmitter from bearings taken at two or more known positions.

1. At location 1: Record:
   - GPS position: lat/lon in decimal degrees
   - True bearing to transmitter: θ₁ (true, from any ADF system)
   - Time and date
   - Confidence level and SNR

2. Move to location 2. Minimum separation from location 1: 500m. For best geometry, location 2 should be positioned such that the angle between bearing lines at the estimated transmitter location is between 60° and 120°. Avoid geometry where the two bearing lines are nearly parallel (cross-angle < 30°) as this produces very large position error.

3. At location 2: Record the same data as at location 1.

4. Optionally, obtain a third bearing from a third location. A third bearing dramatically reduces position uncertainty (converts a line-segment intersection to a triangle of error whose centroid is the best-estimate transmitter location).

5. Plot the bearing lines on a map (paper or digital):
   - From each location, draw a line at the measured true bearing, extending in the direction of the transmitter.
   - The lines intersect at or near the transmitter location.

6. For digital triangulation, use the web interface MAP view (future feature) or a navigation formula:

   ```
   Given: (lat1, lon1), θ₁ and (lat2, lon2), θ₂
   Convert bearings to unit direction vectors in ECEF
   Find intersection of the two great-circle bearing lines
   ```

7. Position accuracy of the triangulation depends on:
   - Bearing accuracy at each point (±5° bearing error propagates to position error proportional to distance)
   - Geometry (cross-angle near 90° is best)
   - Distance to the transmitter (longer range = larger position error for the same bearing error)

   Rule of thumb: position error ≈ bearing_error_km = range × tan(bearing_rms) where bearing_rms is in radians.

   For a transmitter at 10 km range and bearing accuracy of ±5° (0.087 rad): position error ≈ 10 × 0.087 = 870m.

---

<a name="chapter-6"></a>
# CHAPTER 6 — CALIBRATION PROCEDURES

## 6.1 Watson-Watt Balance Calibration

### Equipment Required

- Signal source: HF transceiver or signal generator, 1W output minimum at 50Ω
- Reference antenna: 50Ω dummy load with tap for transmitting, or an outdoor antenna oriented at a known bearing
- GPS for precise distance and bearing measurement
- NanoVNA for feed line phase verification (optional but recommended)

### Procedure

1. Place the calibration transmitter at a measured bearing from the array center. Best calibration bearing: 45° (NE). This excites both the N-S and E-W channels nearly equally, giving the most sensitive balance indication.
2. Measure the bearing from array center to transmitter precisely using GPS coordinates and the Haversine formula (see Section 1.6). Record this as θ_ref.
3. Apply power to the ADF system and select the appropriate band.
4. Issue: `STATUS` via Bluetooth. The system reports raw values: V_NS, V_EW, and the computed bearing θ_computed.
5. Compare θ_computed to θ_ref. If they agree within ±3°, no adjustment is needed.
6. If θ_computed differs from θ_ref by more than ±3°, adjust the software calibration values:
   a. Compute the gain correction needed for each channel.
   b. Issue: `GAIN NS 1.05` (or the required ratio) to scale the N-S channel.
   c. Issue: `GAIN EW 0.97` to scale the E-W channel.
   d. Re-check bearing.
7. Test at four bearings: 45°, 135°, 225°, 315°. All four should agree with reference within ±5°.
8. If systematic bearing error is present at all azimuths (rotating offset), issue: `OFFSET dd.d` where dd.d is the correction in degrees (positive = add to computed bearing).
9. Issue: `CAL SAVE` to write calibration values to NVS. Values are retained through power cycles.

---

## 6.2 Doppler Phase Calibration

### Procedure

1. Place the calibration transmitter (FM, tone on carrier or voice) at true North from the array center (or any known bearing; North is most convenient).
2. Compute the magnetic bearing equivalent: Magnetic_North = 0° + declination (e.g., if declination is -13.5°, magnetic bearing to true North = 346.5°).
3. Issue: `CAL START` via Bluetooth.
4. Note the bearing displayed. The firmware records this bearing and the difference from the known bearing as a phase correction.
5. Move the transmitter to the East position (or known bearing 90° from the first position).
6. Issue: `CAL ADD 90` to register a second calibration point.
7. Repeat for South and West.
8. Issue: `CAL END`. The firmware computes the best-fit phase offset from the four calibration points and saves it to NVS.
9. Verify: place transmitter at a new bearing (e.g., 127°). Confirm measured bearing agrees within ±2°.

---

## 6.3 Loop Null Depth Verification

### Purpose

The null depth of the loop antenna is a critical quality indicator. Insufficient null depth indicates a construction defect (loop asymmetry, shield gap closed, or feed point imbalance) that will degrade bearing accuracy.

### Procedure

1. Place a signal source (HF transmitter, 1W) at a known position.
2. Rotate the loop until the signal is at maximum on the AD8307 detector output. Note the position (this should be approximately when the loop plane faces the transmitter — loop axis approximately parallel to the signal direction).
3. Record the RSSI at this maximum position. Example: -60 dBm.
4. Issue: `MOVE +90` to rotate the loop 90° from the peak position. This places the loop in the null orientation (loop plane edge-on to the transmitter).
5. Wait for the motor to settle (2 seconds). Record the new RSSI. Example: -82 dBm.
6. Compute null depth: null_depth = RSSI_peak - RSSI_null = (-60) - (-82) = 22 dB.
7. Evaluate:
   - Null depth > 25 dB: Excellent. Bearing accuracy better than ±3°.
   - Null depth 15–25 dB: Acceptable. Bearing accuracy ±3°–5°.
   - Null depth 10–15 dB: Marginal. Bearing accuracy ±5°–10°. Investigate cause.
   - Null depth < 10 dB: Unacceptable. Do not use for RDF until repaired.
8. If null depth is inadequate, check:
   a. Faraday shield gap is open (10mm gap at top).
   b. No accidental conductor across the gap.
   c. Feed point connections are symmetrical (equal capacitance to ground from each terminal).
   d. Loop joints are all soldered and making good electrical contact.
   e. Buffer amplifier ground is correctly connected.

---

## 6.4 GPS/Compass Cross-Check

### Procedure

1. Drive or walk in a straight line over at least 100m. Record the GPS track bearing from the GPS RMC sentence ($GPRMC track angle field, degrees true).
2. Record the compass heading displayed on the ADF system OLED (magnetic heading, Line 4 area).
3. Convert compass heading to true: True_heading = Compass_heading + Declination.
4. Compare GPS track bearing (true) to compass true heading. They should agree within ±3°.
5. If the discrepancy exceeds ±5°, perform the following checks:
   a. Re-run compass calibration (Section 5.2).
   b. Verify the displayed declination value is correct for your location.
   c. Check for magnetic interference near the compass: move all motors, speakers, and ferrous tools away from the controller.
   d. Verify the GPS is reporting a valid track (requires movement; stationary GPS track bearing is meaningless).
6. After repairs, repeat the cross-check.

---

<a name="chapter-7"></a>
# CHAPTER 7 — NEC ANTENNA MODELS

## 7.1 Available NEC Models

The following NEC2 model files are included in the `nec_models/` directory of the ADF Systems repository:

| Filename | Antenna Type | Frequency | Key Result |
|---|---|---|---|
| adcock_hf_40m.nec | 4-element Adcock array | 7.15 MHz | Impedance per element, azimuth figure-8 pattern |
| adcock_hf_80m.nec | 4-element Adcock array | 3.65 MHz | Element impedance, ground effect comparison |
| adcock_hf_20m.nec | 4-element Adcock array | 14.2 MHz | High-angle wave rejection verification |
| doppler_ring_2m.nec | 4-element ring array | 146 MHz | Per-element pattern, phase vs. azimuth |
| doppler_ring_70cm.nec | 4-element ring array | 435 MHz | Element mutual coupling, VSWR |
| loop_rdf_40m.nec | Single-turn square loop | 7.15 MHz | Figure-8 pattern, null depth, impedance |
| loop_rdf_80m.nec | Single-turn square loop | 3.65 MHz | Pattern, sensitivity at half-resonance |
| loop_with_shield_40m.nec | Loop + Faraday shield | 7.15 MHz | Shield effect on pattern purity |
| sense_ant_40m.nec | Short vertical monopole | 7.15 MHz | Omnidirectional pattern, impedance |

## 7.2 Running NEC Models

### Software Requirements

Install xnec2c on Debian:

```bash
sudo apt install xnec2c
```

For command-line (batch) NEC runs, install nec2c:

```bash
sudo apt install nec2c
```

### Running with xnec2c (GUI)

1. Launch: `xnec2c filename.nec`
2. In the menu, select Calculations → Run NEC to compute the model.
3. View results:
   - Calculations → Impedance: shows feedpoint impedance vs. frequency (if a frequency sweep is defined)
   - Calculations → VSWR: shows VSWR referred to 50Ω
   - Calculations → Radiation Pattern: opens the pattern display window
   - Calculations → Current Distribution: shows current distribution along wires

### Running with nec2c (CLI)

```bash
nec2c -i adcock_hf_40m.nec -o adcock_hf_40m.out
```

Parse the .out file for EN (impedance) and RP (radiation pattern) cards.

---

## 7.3 Interpreting the Adcock Array Model

### Loading the Model

Open `adcock_hf_40m.nec`. The model contains:
- 4 vertical wire elements (N, S, E, W), each λ/4 = 10.5m long, 3mm radius
- Ground screen simulated with NEC perfect-ground option (GN 1 card) or Sommerfeld ground (GN card with εᵣ = 13, σ = 0.005 S/m for average ground)
- Excitation: delta-gap voltage source at base of one element (one at a time for pattern)

### Impedance at Element Base

Run the single-element version first (N element only, others absent). Expected result at 7.15 MHz for a λ/4 monopole over average ground:
- Input impedance: approximately 36 + j21 Ω (real part slightly above 36Ω free-space value due to ground loss)

Now run the full 4-element array with all elements present but only the N element excited. The impedance changes due to mutual coupling:
- Expected: 32 + j18 Ω (mutual impedance lowers resistance slightly)
- This value changes with element spacing; verify the model matches your actual spacing.

### Azimuth Pattern: N-S Pair

Configure the model to excite the N element with +1V and the S element with -1V (differential excitation). View the azimuth pattern at zero elevation:
- Expected: figure-8 pattern with maximum in N-S direction, nulls in E-W direction
- Null depth in free space: >40 dB
- Over real ground: null depth may be reduced to 25–35 dB

### E-W Pair Verification

Repeat with E and W elements excited in differential mode. Pattern should be the mirror of the N-S pattern, rotated 90°.

---

## 7.4 Interpreting the Doppler Ring Model

### Loading the Model

Open `doppler_ring_2m.nec`. The model contains:
- 4 vertical monopoles (λ/4 = 480mm length, 2.5mm radius) arranged on a circle of diameter 1027mm
- Ground plane: horizontal radials or perfect ground
- Excitation: voltage source at base of each element, one at a time

### Per-Element Far-Field Phase

For each element (activated in turn), compute the far-field complex E-field in the direction θ = 0° (North). Record the phase of the E-field for each element:

| Active Element | Expected Phase at θ = 0° |
|---|---|
| Element 1 (North) | 0° |
| Element 2 (East) | +90° |
| Element 3 (South) | +180° |
| Element 4 (West) | +270° |

This confirms that as the array switches N → E → S → W, the received signal phase progresses from 0° to 90° to 180° to 270° for a transmitter to the North. The resulting temporal phase modulation is:

```
φ(t) = θ_transmitter × sin(2π × f_switch × t)
```

where f_switch = 1000 Hz, confirming the Doppler FM subcarrier mechanism.

### VSWR and Mutual Coupling

Run frequency sweep from 140–150 MHz. Typical VSWR for the 2M array: <2:1 from 140–152 MHz with 5mm rod elements. Mutual coupling between adjacent elements typically reduces the loaded Q and broadens the VSWR bandwidth.

---

## 7.5 Pattern Optimization

### Omnidirectional Element Pattern

For best Doppler and Watson-Watt RDF performance, each individual element should be as omnidirectional as possible in the azimuth plane. Departures from omnidirectional response (due to mutual coupling, nearby structures, or ground asymmetry) introduce bearing errors.

**Watson-Watt**: Verify each monopole's azimuth gain variation is less than ±1 dB. Use the NEC model with real ground to check elevation-angle response; pattern should have maximum between 10° and 45° elevation (to reject low-angle skywave on HF).

**Loop**: In free space, the ideal loop figure-8 pattern should have null depth > 30 dB. The NEC model `loop_rdf_40m.nec` shows the expected null depth. Confirm:
- Null depth: >30 dB (free space model)
- Reduction over real ground: model with GN 2 card (Sommerfeld) at various soil conductivities
- Expected result: null depth 20–25 dB at 5m elevation over average ground

### Achieving Deep Nulls in Practice

To achieve the theoretical null depth in the NEC model with a physical loop:
1. Loop construction must be mechanically symmetric (all sides equal, corners square).
2. Feed point must be electrically symmetric (equal capacitance to ground from each terminal, equal lead length from loop to amplifier).
3. Faraday shield gap must be exactly at the electrical center of the top rail.
4. Buffer amplifier common-mode rejection must be > 40 dB.

---

<a name="chapter-8"></a>
# CHAPTER 8 — FIRMWARE ARCHITECTURE

## 8.1 File Structure

```
ant_gear/adf_systems/
├── firmware/
│   ├── esp32_adf_hf/
│   │   ├── esp32_adf_hf.ino       Main sketch
│   │   ├── config.h               GPIO, band tables, constants
│   │   ├── watson_watt.h          WatsonWatt class
│   │   ├── bearing.h              BearingDisplay class
│   │   ├── web_page.h             Embedded HTML/JS (PROGMEM)
│   │   └── bt_handler.h           Bluetooth SPP command parser
│   ├── esp32_adf_vhf/
│   │   ├── esp32_adf_vhf.ino      Main sketch
│   │   ├── config.h               VHF bands, switch pins, Doppler params
│   │   ├── doppler.h              DopplerRDF class
│   │   ├── bearing.h              (shared with HF, same file)
│   │   └── web_page.h             VHF web interface HTML
│   ├── esp32_rotary_loop/
│   │   ├── esp32_rotary_loop.ino  Main sketch
│   │   ├── config.h               Motor pins, scan parameters
│   │   ├── loop_rdf.h             LoopRDF class
│   │   ├── bearing.h              (shared)
│   │   └── web_page.h             Loop web interface HTML
│   └── cyd_adf_display/
│       └── cyd_adf_display.ino    CYD single-file sketch
├── nec_models/                    NEC2 model files
├── schematics/                    KiCad schematic files
├── enclosures/                    3D print files (STL/STEP)
└── manual/
    └── ADF_SYSTEMS_MANUAL.md      This document
```

---

## 8.2 Watson-Watt HF Firmware (esp32_adf_hf)

### config.h

Defines all hardware assignments and tunable parameters:

```cpp
// GPIO assignments
#define PIN_RF_SWITCH_NS   25    // DPDT relay coil for N-S pair
#define PIN_RF_SWITCH_EW   26    // DPDT relay coil for E-W pair
#define PIN_SENSE_RELAY    27    // Sense antenna relay
#define PIN_ADC_INPUT      34    // ADC input from INA128
#define PIN_LED_POWER      2     // Green LED
#define PIN_LED_BT         4     // Blue BT LED
#define PIN_ENCODER_A      18    // Rotary encoder A
#define PIN_ENCODER_B      19    // Rotary encoder B
#define PIN_ENCODER_SW     5     // Encoder push switch
#define PIN_GPS_RX         16    // GPS UART RX
#define PIN_GPS_TX         17    // GPS UART TX
#define PIN_GPS_PPS        35    // GPS 1PPS interrupt

// ADC parameters
#define ADC_SAMPLE_RATE    10000   // Hz
#define ADC_BLOCK_SIZE     512     // samples per bearing computation
#define ADC_VREF           3.3f    // Volts

// Algorithm constants
#define SNR_THRESHOLD_DB   10.0f   // Minimum SNR for valid bearing
#define BEARING_HISTORY    8       // Readings for boxcar average
#define SENSE_PHASE_THRESH 45.0f   // Degrees for sense ambiguity resolution

// Band table (11 entries: name, freq_MHz, element_length_m)
struct Band { const char* name; float freq; float el_len; };
const Band BANDS[] = {
    {"160M", 1.850, 40.5},
    {"80M",  3.650, 20.5},
    // ... (all 11 bands)
};
```

### watson_watt.h — WatsonWatt Class

```cpp
class WatsonWatt {
public:
    // Sample both ADC channels and compute bearing
    bool compute(float* adc_ns, float* adc_ew, float* adc_sense, int n);

    // Returns bearing in degrees magnetic (0–360°)
    float getBearingMag() const;

    // Returns SNR in dB
    float getSNR() const;

    // Returns confidence level (0=INVALID, 1=LO, 2=MED, 3=HI)
    int getConfidence() const;

    // Calibration setters
    void setNSGain(float g);
    void setEWGain(float g);
    void setOffset(float deg);

private:
    float _rms_ns, _rms_ew;          // RMS of each channel
    float _bearing_raw;               // atan2 result, degrees
    float _bearing_resolved;          // After sense ambiguity resolution
    float _snr_db;
    float _bearing_history[8];
    int   _history_idx;
    float _ns_gain, _ew_gain;
    float _offset_deg;

    float computeRMS(float* samples, int n);
    bool  resolveSenseAmbiguity(float* sense_samples, int n, float raw_bearing);
    float averageBearing();           // Circular mean of history buffer
};
```

### bearing.h — BearingDisplay Class

```cpp
class BearingDisplay {
public:
    void begin();
    void update();               // Call from main loop (non-blocking)

    // GPS access
    float getLatitude() const;
    float getLongitude() const;
    bool  hasGpsFix() const;
    int   getGpsFixType() const; // 0=none, 1=2D, 2=3D

    // Compass access
    float getCompassHeading() const;  // Degrees magnetic
    float getDeclination() const;     // Degrees, + = East
    void  startCalibration();
    void  stopCalibration();
    bool  isCalibrating() const;

    // Triangulation / waypoint
    float bearingToWaypoint(float lat2, float lon2) const;
    float distanceToWaypoint(float lat2, float lon2) const; // km

private:
    // NMEA parser state
    // QMC5883L reader
    // Calibration min/max tracking
    // Haversine computation
    // IGRF declination lookup
};
```

### esp32_adf_hf.ino — Main Sketch Flow

```
setup():
    init GPIO, UART, I2C, SPI
    init OLED (display splash)
    init GPS UART2
    init QMC5883L
    load NVS calibration data
    start WiFi AP
    start HTTP server
    start Bluetooth SPP
    start ADC timer (10 kHz, fills ring buffer)
    motor_home() [if phased array mode]
    display operational screen

loop():
    if adc_buffer_ready:
        ww.compute(ns_buf, ew_buf, sense_buf, BLOCK_SIZE)
        update OLED
        update BT output (if connected)
        update web API JSON cache
    bearingDisplay.update()   // Non-blocking GPS + compass update
    encoder.update()          // Check for rotation or press
    bt.processCommands()      // Parse incoming BT command line
    webServer.handleClient()  // Handle HTTP requests
```

---

## 8.3 Doppler VHF Firmware (esp32_adf_vhf)

### config.h (VHF Specific)

```cpp
// Switch control
#define PIN_SW_A0   25    // FST3253 A0 select
#define PIN_SW_A1   26    // FST3253 A1 select
#define PIN_SW_OE   27    // FST3253 output enable (active low)
#define PIN_ADC_IN  34    // Audio input from receiver discriminator

// Doppler parameters
#define SWITCH_RATE_HZ      1000    // Switch frequency for 2M/70cm
#define SWITCH_RATE_HZ_UHF  2000    // For 33cm/20cm bands
#define ADC_RATE            10000   // Audio ADC sample rate
#define GOERTZEL_N          1024    // Block size

// VHF band table
struct VHFBand { const char* name; float freq_mhz; float el_len_mm; float diam_mm; };
const VHFBand VHFBANDS[] = {
    {"2M",    146.0, 480.0, 1027.0},
    {"1.25M", 222.0, 308.0,  676.0},
    {"70cm",  435.0, 156.0,  330.0},
    {"33cm",  915.0,  77.0,  163.0},
    {"20cm", 1296.0,  55.0,  115.0}
};
```

### doppler.h — DopplerRDF Class

```cpp
class DopplerRDF {
public:
    void begin(int switch_rate_hz);
    void processSamples(float* samples, int n);   // Call with each ADC block

    float getBearing() const;      // Degrees magnetic, 0–360°
    float getRSSI() const;         // dBm (from AD8307 calibration curve)
    float getSNR() const;          // dB (phase coherence metric)
    bool  isValid() const;

    void startCalibration(float known_bearing_true);
    void addCalibrationPoint(float known_bearing_true);
    void endCalibration();
    void saveCalibration();

private:
    // Goertzel algorithm state
    float _s1, _s2;
    float _coeff;
    int   _sample_count;
    int   _N;

    // Phase reference (from switch ISR)
    float _ref_phase_rad;

    // Results
    float _bearing_mag;
    float _rssi_dbm;
    float _snr;
    bool  _valid;

    // Calibration
    float _phase_offset_rad;
    float _cal_sum;
    int   _cal_count;

    float goertzelPhase(float* x, int n, float freq, float sample_rate);
    float ad8307TodBm(int adc_counts);
};
```

### Timer ISR for Switching

The switch control is driven by a hardware timer ISR at the switch rate (1000 or 2000 Hz). Each ISR call advances the switch state machine:

```
ISR:
    state = (state + 1) % 4
    update switch GPIO outputs (A0, A1 pins)
    update reference phase accumulator: ref_phase += 2π/4
    set adc_trigger flag
```

The ADC is triggered by DMA at 10 kHz continuously. The Goertzel algorithm processes 1024-sample blocks corresponding to exactly 1024/10000 = 102.4 ms of audio, containing 102.4 cycles of the 1000 Hz subcarrier.

---

## 8.4 Rotary Loop Firmware (esp32_rotary_loop)

### config.h (Loop Specific)

```cpp
// Motor control (DRV8825)
#define PIN_STEP    25
#define PIN_DIR     26
#define PIN_ENABLE  27    // Active LOW
#define PIN_MS1     14    // Microstep select 1
#define PIN_MS2     12    // Microstep select 2
#define PIN_MS3     13    // Microstep select 3

// Position feedback
#define PIN_ENC_CS  5     // AS5048A SPI chip select
#define PIN_POT_ADC 35    // Potentiometer ADC (if encoder not present)

// Sense and tuning
#define PIN_SENSE_RELAY    32    // Sense antenna relay
#define PIN_VAR_DAC        25    // Varactor voltage (ESP32 DAC1)
#define PIN_CAP_RELAY_BASE 33    // First of 4 relay pins for cap bank

// Scan parameters
#define SCAN_STEP_DEG       2.0f    // Coarse scan step
#define SCAN_SETTLE_MS      100     // Settle time per step
#define FINE_STEP_DEG       0.25f   // Fine scan step (near null)
#define FINE_WINDOW_DEG     10.0f   // Fine scan window around null candidate

// Gear ratio: motor to loop
#define GEAR_RATIO          3.0f
#define MOTOR_STEPS_REV     200
#define MICROSTEP_DIV       16
#define DEG_PER_MICROSTEP   (360.0f / (MOTOR_STEPS_REV * MICROSTEP_DIV * GEAR_RATIO))
```

### loop_rdf.h — LoopRDF Class

```cpp
class LoopRDF {
public:
    void begin();
    void update();           // Non-blocking, call from main loop every iteration
    void home();             // Move to home position (blocking, max 5s)

    // Scan control
    void startScan();
    void stopScan();
    bool isScanComplete() const;

    // Tuning
    void autoTune();         // Sweep varactor, find peak RSSI (blocking ~8s)

    // Results
    float getNullBearing() const;    // Degrees magnetic
    float getNullDepth() const;      // dB
    float getConfidence() const;     // 0–1
    float getResolvedBearing() const; // After sense resolution, degrees mag

    // Sense
    void setSenseEnabled(bool en);
    void resolveAmbiguity();    // Compare with/without sense, set resolved bearing

    // Motor
    void moveTo(float degrees);      // Non-blocking move
    float getCurrentPosition() const;

private:
    // Motor state machine
    enum MotorState { IDLE, MOVING, SCANNING, HOMING };
    MotorState _motorState;
    float _targetPos, _currentPos;
    long  _stepTarget, _stepCurrent;
    unsigned long _lastStepTime;
    int   _stepInterval_us;

    // Scan data
    float _scanRSSI[181];     // RSSI at each 2° step (360°/2° = 180 points)
    int   _scanIdx;

    // Null finding
    float findNullGaussian();
    float findNullMinimum();

    // ADC
    float readRSSI();         // Returns dBm from AD8307
    float _varVoltage;        // Current varactor DAC voltage
};
```

### Non-Blocking Stepper Implementation

The motor update function is called every pass of the main loop. It uses microsecond timing to generate steps without delaying the loop:

```cpp
void LoopRDF::update() {
    if (_motorState == IDLE) return;
    unsigned long now = micros();
    if (now - _lastStepTime >= _stepInterval_us) {
        if (_stepCurrent != _stepTarget) {
            digitalWrite(PIN_DIR, (_stepTarget > _stepCurrent) ? HIGH : LOW);
            digitalWrite(PIN_STEP, HIGH);
            delayMicroseconds(2);
            digitalWrite(PIN_STEP, LOW);
            _stepCurrent += (_stepTarget > _stepCurrent) ? 1 : -1;
            _currentPos = _stepCurrent * DEG_PER_MICROSTEP;
            _lastStepTime = now;
        } else {
            _motorState = IDLE;
            // If scanning: record RSSI, advance scan index
            if (/* scan mode */) {
                _scanRSSI[_scanIdx++] = readRSSI();
                // Advance to next scan position or finish
            }
        }
    }
}
```

---

## 8.5 CYD Display (cyd_adf_display)

The CYD display firmware is a single .ino file that:

1. Initializes TFT_eSPI (ILI9341 320×240, SPI)
2. Scans for available ADF Bluetooth devices (SerialBT.begin as master)
3. Connects to the first discovered "RF-ADF" device
4. Displays the MODE_COMPASS screen by default
5. Processes BT data packets: JSON parsing for bearing, SNR, GPS, confidence
6. Updates compass rose needle angle using TFT rotation transform
7. Handles touch events (XPT2046 touch controller, I2C) for tab bar navigation
8. Sends band commands back to ADF device when BAND mode tab is active

The compass rose is drawn once as a static background sprite, then the needle is redrawn each update by restoring the needle area from the background sprite and painting the new needle.

---

## 8.6 Bluetooth SPP Command Reference

### Watson-Watt HF Commands

| Command | Syntax | Response | Description |
|---|---|---|---|
| BAND | BAND n | OK BAND n | Set band (0–10 per table) |
| STATUS | STATUS | JSON object | Report bearing, SNR, GPS, compass |
| CAL START | CAL START | OK CAL | Begin compass calibration rotation |
| CAL END | CAL END | OK SAVED / ERR | End calibration, save offsets |
| DECL | DECL f | OK DECL f | Override declination (degrees, + East) |
| OFFSET | OFFSET f | OK OFFSET f | Add fixed bearing offset (degrees) |
| GAIN NS | GAIN NS f | OK GAIN_NS f | Set N-S channel gain multiplier |
| GAIN EW | GAIN EW f | OK GAIN_EW f | Set E-W channel gain multiplier |
| RESET | RESET | (reboots) | Software reset |
| HELP | HELP | (lists commands) | Print command list |

### Doppler VHF Commands

| Command | Syntax | Response | Description |
|---|---|---|---|
| BAND | BAND n | OK BAND n | Set band (0–4, see VHF band table) |
| STATUS | STATUS | JSON object | Report bearing, RSSI, GPS, compass |
| CAL START | CAL START f | OK CAL | Begin calibration, f = known bearing (true) |
| CAL ADD | CAL ADD f | OK CAL_PT | Add calibration point at known bearing f |
| CAL END | CAL END | OK SAVED | End calibration, compute and save offset |
| SQUELCH | SQUELCH n | OK SQL n | Set RSSI squelch level in dBm |
| DECL | DECL f | OK DECL f | Override declination |
| HELP | HELP | (lists commands) | Print command list |

### Rotary Loop Commands

| Command | Syntax | Response | Description |
|---|---|---|---|
| SCAN | SCAN | OK SCANNING | Start full 360° coarse scan |
| STOP | STOP | OK STOPPED | Halt motor immediately |
| HOME | HOME | OK HOMING | Move to 0° reference position |
| MOVE | MOVE deg | OK MOVING | Move to absolute bearing position |
| BAND | BAND n | OK BAND n | Set band, auto-select cap bank |
| SENSE ON | SENSE ON | OK SENSE_ON | Enable sense antenna relay |
| SENSE OFF | SENSE OFF | OK SENSE_OFF | Disable sense antenna relay |
| TUNE | TUNE | OK TUNING | Start varactor auto-tune |
| STATUS | STATUS | JSON object | Report position, null, depth, GPS |
| HELP | HELP | (lists commands) | Print command list |

---

## 8.7 WiFi HTTP API

### Common Endpoints (All Systems)

| Endpoint | Method | Request | Response | Notes |
|---|---|---|---|---|
| / | GET | none | HTML (full page) | Main web UI |
| /api/status | GET | none | JSON | Full status object |
| /api/bearing | GET | none | JSON bearing subset | Optimized for high-rate polling |
| /api/band | POST | {"band":n} | {"ok":true} | Set active band |
| /api/cal | POST | {"action":"start"} or {"action":"end"} | {"ok":true} | Compass calibration |
| /api/decl | POST | {"decl":f} | {"ok":true} | Override declination |
| /api/reset | POST | none | (none, reboots) | Software reset |

### Loop-Specific Endpoints

| Endpoint | Method | Request | Response | Notes |
|---|---|---|---|---|
| /api/scan | POST | {"action":"scan"/"stop"/"home"} | {"ok":true} | Motor control |
| /api/tune | POST | none | {"ok":true} | Start auto-tune |
| /api/move | POST | {"bearing":f} | {"ok":true} | Move to position |
| /api/sense | POST | {"enable":true/false} | {"ok":true} | Sense relay control |

---

<a name="chapter-9"></a>
# CHAPTER 9 — TROUBLESHOOTING

## 9.1 Watson-Watt Troubleshooting Matrix

| Symptom | Probable Cause(s) | Corrective Action |
|---|---|---|
| Bearing always shows 0° or 360° | N-S channel missing or grounded; relay stuck N-S position; NS coax shorted | Check NS relay with DMM; verify NS coax continuity; check INA128 NS input |
| Bearing always shows 90° or 270° | E-W channel missing or grounded; EW relay stuck; EW INA128 failed | Check EW relay and coax; swap INA128 and test |
| Bearing rotates continuously with stationary transmitter | Phase-matched feed lines not connected; INA128 oscillating; ADC input overdriven | Check feed line connections; reduce INA128 gain (increase Rg); add 10pF input cap to INA128 |
| SNR always shows 0 or < 1 dB | Signal not present; ADC input disconnected; INA128 unpowered | Verify signal present with handheld receiver; check 3.3V to INA128; check ADC input pin |
| Bearing off by exactly 90° | N-S and E-W coaxes swapped at junction box | Swap NS and EW coax labels at junction box |
| Bearing off by exactly 180° | Sense antenna polarity reversed; sense relay coil polarity wrong | Reverse sense coax center/shield; check relay wiring |
| Large scatter in repeated bearings (>15° std dev) | High site multipath; low SNR; element imbalance; intermittent connection | Increase SNR threshold; average more readings; check element connections; change site |
| Bearing off by constant angle (not 0°/90°/180°) | Array orientation offset; calibration offset incorrect | Re-measure array orientation; re-run OFFSET calibration |
| No GPS fix after 10 minutes | GPS antenna obstructed; GPS antenna cable fault; GPS module not powered | Move antenna to open sky; check 3.3V bias at GPS SMA; check UART2 output with serial monitor |
| Compass shows wrong heading | Hard-iron calibration not performed; strong magnetic interference nearby; compass mounted wrong axis | Re-run CAL START/END; move away from metal objects and motors; verify X-axis pointing forward |
| WiFi AP not visible | ESP32 in error state; WiFi init failed; antenna fault | Observe OLED — should show "WiFi OK"; if not, check serial debug output; power cycle |
| BT LED never lights | BT device not discovered; BT name mismatch; SPP not initialized | Scan for devices on phone; verify name matches config.h; check bt_handler.h init |
| One antenna pair giving wrong bearing (±45° error) | One element of the pair has wrong length, loose connection, or reversed polarity | Test each element individually with short vertical receive antenna as reference; check coax polarity |
| Bearing OK for some bands, wrong for others | Feed line phase mismatch is frequency-dependent; element length wrong for some bands | Re-verify phase matching at affected band frequencies with NanoVNA |
| OLED blank after power-up | OLED not initialized; I2C address wrong (0x3C vs 0x3D); 3.3V absent | Check I2C address jumper on OLED module; verify 3.3V; check SDA/SCL wiring |
| System reboots repeatedly (watchdog) | Task overrun; ADC DMA overflow; heap exhaustion | Enable serial debug; reduce ADC block size; check for heap leaks in BT handler |
| Bearing flips 180° occasionally | Sense signal marginal; sense relay bounce; sense coax too long | Increase sense threshold in config.h; add 100ns relay filter in firmware; shorten sense coax |

---

## 9.2 Doppler Troubleshooting Matrix

| Symptom | Probable Cause(s) | Corrective Action |
|---|---|---|
| No 1kHz subcarrier detected (Goertzel output near zero) | Receiver not connected to ADC input; switch IC not switching; audio level too low | Verify 3.5mm audio jack connection; check switch control GPIO outputs with LED; increase receiver volume |
| Bearing always same regardless of signal direction | Switch IC failed (all elements tied together); phase offset near 0° | Test switch by measuring element RSSI individually; check FST3253 supply |
| Bearing correct for some directions, wrong for others | One element has wrong length or bad connection | Test each element with short vertical as reference; swap elements |
| Switch IC not switching (all GPIO correct) | FST3253 Vcc not connected; OE line stuck HIGH; ESD damage | Check FST3253 pin 16 for 3.3V; verify OE is driven LOW; replace IC |
| High phase noise (bearing jitter > 10°) | Low SNR signal; receiver audio clipping or distortion; ADC sample rate mismatch | Reduce audio input level; move closer to transmitter; verify ADC sample rate in timer |
| Bearing always 0° or stuck | Phase offset from calibration wrong; CAL data corrupted | Issue `CAL END` without preceding CAL START to clear; re-run calibration |
| Bearing 180° wrong | Element rotation direction reversed (S→W→N→E instead of N→E→S→W) | Swap A0/A1 pins in config.h, or swap element 1 and element 3 connectors |
| No bearing on 33cm or 20cm band | PE4259 switch not installed; frequency too high for FST3253 | Verify PE4259 is installed for 33cm/20cm bands; check switch configuration jumper |
| Bearing drifts slowly over time | Reference oscillator drift; receiver frequency drift; temperature effect | Verify ESP32 crystal clock stability; use temperature-compensated receiver |
| Array doesn't level on tripod | Tripod pan head not level; disk warp (PLA in heat) | Adjust tripod legs; replace with PETG print |

---

## 9.3 Rotary Loop Troubleshooting Matrix

| Symptom | Probable Cause(s) | Corrective Action |
|---|---|---|
| Motor doesn't move on SCAN command | ENABLE pin stuck HIGH; DRV8825 fault; motor not connected | Check PIN_ENABLE GPIO output (should go LOW); check DRV8825 FAULT LED (if present); check motor 4-wire cable |
| Motor moves but no position change (AS5048A) | Magnet missing or misaligned; SPI wiring error; I2C address conflict | Verify magnet is centered on shaft; check SPI CS, MOSI, MISO, SCK; use STATUS command to read raw encoder value |
| Motor steps but position reads wrong | Gear ratio constant wrong in config.h; microstep setting mismatch | Verify GEAR_RATIO = 3.0; verify DRV8825 MS1/MS2/MS3 pins match config.h MICROSTEP_DIV |
| Null depth < 10 dB | Faraday shield gap closed; feed point asymmetry; loop asymmetry | Check shield gap (10mm, must be open); measure capacitance from each terminal to ground; remeasure loop dimensions |
| Scan completes but no null found | Signal too weak; band mismatch; varactor not tuned | Verify signal SNR > 15 dB; run TUNE before SCAN; select correct band |
| Loop won't tune (TUNE command finds no peak) | Varactor reverse-biased; DAC1 not working; band capacitor relay failed | Check DAC1 output (GPIO25, should sweep 0–3.3V); check varactor polarity in schematic; check relay driver |
| Bearing off by exactly 90° | Loop shaft bracket mounted 90° wrong | Rotate shaft bracket 90° and re-tighten set screws |
| Motor oscillates back and forth during scan | Step rate too fast; supply voltage sag; DRV8825 Vref too low | Increase SCAN_STEP_INTERVAL_MS in config.h; check 12V under load; increase DRV8825 Vref to 0.75V |
| Scan position drifts over time (missed steps) | DRV8825 current too low; motor overheating | Increase DRV8825 Vref; allow motor to cool between scans |
| Sense resolution gives wrong result | Sense antenna directional (not omnidirectional); RC phase shift off-frequency | Use shorter sense wire (< λ/20); verify RC values match operating frequency |

---

## 9.4 Fault Codes Reference

| Fault Code | OLED Message | Cause | Action |
|---|---|---|---|
| E01 | "COMPASS FAIL" | QMC5883L not responding on I2C | Check I2C wiring, pull-ups; replace module |
| E02 | "GPS NO DATA" | GPS UART not receiving NMEA | Check GPS cable; verify 3.3V bias to GPS antenna |
| E03 | "ADC OVERFLOW" | ADC ring buffer overrun | Reduce ADC sample rate; optimize loop() |
| E04 | "CAL INVALID" | Calibration did not complete full rotation | Repeat calibration with full 2-turn rotation |
| E05 | "NS OPEN" | N-S channel input floating (no signal path) | Check NS coax and relay |
| E06 | "EW OPEN" | E-W channel input floating | Check EW coax and relay |
| E07 | "MOTOR FAULT" | DRV8825 FAULT line asserted | Check motor current, check for short in motor winding |
| E08 | "ENCODER ERR" | AS5048A not responding on SPI | Check SPI wiring; verify 3.3V to encoder |
| E09 | "WIFI FAIL" | WiFi AP initialization failed | Power cycle; check for antenna damage on ESP32 |
| E10 | "NVS CORRUPT" | Calibration data in NVS unreadable | Issue RESET; re-run all calibrations |
| E11 | "ADC CLIP" | ADC input voltage exceeds 3.3V | Add attenuator (voltage divider) at ADC input |
| E12 | "BT FAIL" | Bluetooth initialization failed | Power cycle; check for concurrent WiFi/BT resource conflict |

---

<a name="chapter-10"></a>
# CHAPTER 10 — PREVENTIVE MAINTENANCE

## 10.1 Maintenance Schedule

| Interval | Task | Reference |
|---|---|---|
| After each field use | Visual inspection of all cables and connectors | 10.2 |
| After each field use | Remove and dry any condensation from coax connectors | 10.2 |
| After each field use | Clean mud or debris from printed parts and element sockets | 10.4 |
| After each field use | Stow elements and coax in labeled bags to prevent loss | — |
| Monthly | Check element connection tightness (M3 set screws) | 10.2 |
| Monthly | Inspect coax for kinks, damage to jacket | 10.2 |
| Monthly | Verify LED indicators function correctly | 10.1 |
| Monthly | Confirm WiFi and BT initialize and connect normally | Chapter 5 |
| Quarterly | Re-run compass calibration | 5.2 |
| Quarterly | Verify GPS cold start fix in < 60 seconds | 5.1 |
| Quarterly | Check motor coupling set screws (loop system) | 10.3 |
| Quarterly | Check belt tension (loop system) | 10.3 |
| Quarterly | Verify DRV8825 Vref is 0.75V ± 0.05V (loop) | 10.3 |
| Annually | Inspect shaft bearing/bushing for wear (loop) | 10.3 |
| Annually | Check INA128 gain resistor soldering (HF system) | 10.4 |
| Annually | Clean loop copper surface (loop system) | 10.3 |
| Annually | Update GPS firmware if manufacturer release available | — |
| Annually | Full 4-point bearing calibration verification | 6.1, 6.2 |

---

## 10.2 Connector Maintenance

### BNC Connectors

1. Inspect BNC connectors on all element feed lines at the junction box and controller box inputs.
2. Look for: green oxidation on center pin, cracked or deformed dielectric, bent center pin, cracked outer ring.
3. Clean oxidized center pins with a cotton swab dampened with isopropyl alcohol (IPA, 90%+). Allow to dry before reconnecting.
4. Verify center pin retention: the pin should not move when pushed with a fingernail. A loose center pin indicates the coax needs re-termination.
5. Torque standard: finger-tight plus one-quarter turn with a 7/16" wrench. Do not over-tighten BNC; the bayonet lock prevents over-tightening, but over-forcing can crack the mating connector.

### SMA Connectors (GPS, VHF array)

1. Inspect SMA connectors for bent center pins (very common damage from improper mating).
2. A bent SMA center pin (0.9mm diameter) is difficult to straighten without breaking. Replace the connector rather than attempting to straighten.
3. Mate SMA connectors by first aligning by hand until the connector rotates freely, then tighten with a 5/16" torque wrench to 0.56 N·m (5 in-lb).
4. Clean SMA connectors with IPA and a foam swab. Allow to dry completely before mating.
5. Cap unused SMA connectors with SMA dust caps.

### Coax Inspection

1. Inspect the full length of all feed line coax once per month.
2. Flex each coax through its full range of motion. Listen and feel for crunching (broken shield braid or kinked center conductor).
3. If any coax shows physical damage (deep kinks, jacket cuts, exposed shield, delaminated foam dielectric), replace the entire run. Do not splice coax in a feed line run; any splice changes the electrical length.
4. Verify strain relief at all connector terminations. The coax must not be under tension at the connector backshell; movement must be absorbed by the cable, not the connector.

---

## 10.3 Loop System Maintenance

### Belt and Pulleys

1. Inspect the GT2 belt for cracks, missing teeth, or fraying of the edge cords. Replace if any damage is present.
2. Check belt tension: deflect the belt at mid-span with light finger pressure (approximately 2N). Correct deflection: 5mm. If deflection exceeds 10mm, the belt is too loose. If the belt cannot be deflected 2mm, it is too tight.
3. Adjust belt tension by loosening the two motor mounting screws, sliding the motor to the desired position, and retightening. Recheck tension.
4. Inspect pulley set screws quarterly. Tighten with M3 hex key. Apply one drop of thread-locker (medium strength, e.g., Loctite 243) to each set screw after final tightening.

### Shaft and Bearings

1. Check the loop shaft for lateral play in the bearing block. Acceptable play: < 0.5mm total lateral movement. Excess play causes loop wobble and degraded null depth.
2. Apply one drop of light machine oil (ISO 32 or sewing machine oil) to the bearing block shaft surfaces annually.
3. Do not use grease in the bearing block; grease accumulates dust and accelerates wear.

### Motor Driver Vref

The DRV8825 Vref sets the peak motor current. Incorrect Vref causes missed steps (too low) or motor overheating (too high).

1. With the motor connected and powered, set a DMM to DC volts, reference probe to GND.
2. Measure the voltage on the DRV8825 Vref trimmer (small potentiometer on the DRV8825 module, accessible with controller box open).
3. Correct Vref for NEMA 17 at 1.5A setting:

   ```
   Vref = I_max / 2 = 1.5 / 2 = 0.75V
   ```

4. Adjust the trimmer until Vref = 0.75V ± 0.05V. Use a small flat-blade screwdriver; turn slowly and re-measure.

### Copper Loop Cleaning

1. Once per year, clean the copper tube loop surface to remove oxidation.
2. Use 000-grade steel wool (fine grade). Rub gently along the length of each tube.
3. Do not clean the Faraday shield copper foil with steel wool. Wipe the shield foil with IPA on a cloth.
4. After cleaning, apply a light coat of lacquer spray to slow re-oxidation. Keep lacquer off the feed point terminals.

---

## 10.4 HF System Maintenance

### INA128 Gain Resistor

The INA128 instrumentation amplifier gain is set by a single external resistor (Rg) between pins 1 and 8. Gain formula:

```
G = 1 + (49.4kΩ / Rg)
```

For Rg = 3Ω: G = 1 + (49400/3) = 16,467 ≈ 84 dB.

1. Inspect the Rg solder joints annually. This resistor is small (0402 or 0603 package) and subject to solder joint cracking from thermal cycling.
2. Use magnification (×10 loupe) to inspect the solder fillets. Cold or cracked joints appear dull or fractured.
3. Reflow suspect joints with a fine-tipped soldering iron (320°C, clean tip, solder flux).
4. If the gain is suspected low (weak bearing output), measure Rg resistance in circuit with the IC powered off: resistance should match the installed value within ±2%.

### Phase-Matched Coax Re-Verification

After any coax replacement or repair:

1. Re-verify electrical length match per Section 4.1 (Phase Matching Verification).
2. If NanoVNA is not available, a useful field check is to connect all four element coax ends to the junction box and compare signal levels from a source at 45° to the array: V_NS and V_EW should be approximately equal (within ±3 dB) for a signal at 45°.

---

<a name="chapter-11"></a>
# CHAPTER 11 — PARTS LIST

## 11.1 Common Electronics (All Systems)

| Item | Qty | Specification | Typical Source |
|---|---|---|---|
| ESP32-WROOM-32 module | 1 | 4MB flash, PCB trace antenna, 38-pin module | AliExpress, Mouser |
| QMC5883L magnetometer module | 1 | I2C, 3.3V, breakout board with 4.7kΩ pull-ups | AliExpress |
| u-blox NEO-8M GPS module | 1 | UART, 3.3V, with active patch antenna | AliExpress (HMC module) |
| SSD1306 OLED display | 1 | 128×64, I2C, 3.3V, 0.96" | AliExpress |
| MPU6050 IMU module | 1 | I2C, 3.3V, gyro + accel, for tilt correction | AliExpress |
| AD8307 logarithmic detector | 1 | SOIC-8, 500 MHz, 92 dB dynamic range | Mouser, DigiKey |
| AMS1117-3.3 LDO regulator | 1 | SOT-223, 800 mA | DigiKey |
| 78L05 voltage regulator | 1 | TO-92, 100 mA, 5V | DigiKey |
| 2.1mm barrel jack (PCB) | 1 | Center positive, 2A rating | DigiKey |
| Green LED 3mm | 1 | 520 nm, 20 mA | DigiKey |
| Blue LED 3mm | 1 | 470 nm, 20 mA | DigiKey |
| Rotary encoder w/ switch | 1 | 20 PPR, 6mm D-shaft, momentary push | AliExpress |
| PCB enclosure | 1 | 115×65×30 mm, ABS, with panel cutout | AliExpress |
| Misc passives (R, C) | 1 set | 0402/0603 SMD, values per schematic | DigiKey |

## 11.2 Watson-Watt HF Specific Components

| Item | Qty | Specification | Notes |
|---|---|---|---|
| Aluminum element rods | 4× sets | 6mm OD, various lengths per band (see Section 3.1 table) | Cut to length; hardware store or online metals |
| RG-58C/U coaxial cable | 1 roll | 50Ω, solid PE dielectric, copper braid, foam jacket | 25m minimum for 40M array setup |
| PL-259 (UHF male) connectors | 10 | For RG-58 termination | DigiKey |
| SO-239 (UHF female) panel connectors | 6 | For junction box | DigiKey |
| FT-50-43 toroid core | 4 | 12mm OD, Fair-Rite type 43 material | Mouser, Amidon |
| Magnet wire | 1 spool | 24 AWG, enamel coated, for balun winding | Electronics distributors |
| INA128 instrumentation amplifier | 2 | DIP-8 or SOIC-8, Rg = 3Ω for 84dB gain | Mouser, DigiKey |
| DPDT relay, 12V coil | 2 | 10A contact rating, PCB mount | DigiKey |
| GALI-84 MMIC LNA | 2 | DC–6 GHz, 13 dB gain, 50Ω | Mini-Circuits |
| BAT46 Schottky diodes | 8 | 100V, 150 mA, for relay flyback protection | DigiKey |
| Element base stakes | 4 | Printed PETG, stake form | Print from STL file |
| Ground stake (center) | 1 | 30cm steel or fiberglass, 19mm OD | Garden supply |
| Junction box enclosure | 1 | NEMA-1, 150×100×75mm, ABS | Electrical supply |

## 11.3 Doppler VHF Specific Components

| Item | Qty | Specification | Notes |
|---|---|---|---|
| FST3253 dual 1:4 analog multiplexer | 1 | TSSOP-16, 3.3V, ≤400 MHz | DigiKey, Mouser |
| PE4259 SPDT RF switch | 2 | DC–3 GHz, SOT-26, for 33cm/20cm bands | pSemi/Mouser |
| BNC female panel connector | 4 | 50Ω, PCB or panel mount | DigiKey |
| 5mm aluminum rod | 1 set | Various lengths per band (see Section 2.2) | Hardware store |
| Doppler ring disk | 1 per band | PETG print, 3mm, band-specific diameter | Print from STL file |
| 1/4"-20 brass insert | 1 | Heat-set, for tripod mount | McMaster-Carr |
| RG-316 coaxial cable | 2m | 50Ω, 2.5mm OD, SMA connectors | Mouser |
| SMA female PCB connector | 5 | Edge-launch or right-angle | DigiKey |
| Camera tripod | 1 | Standard 1/4"-20 head, extend to 2m | Camera supply |

## 11.4 Rotary Loop Specific Components

| Item | Qty | Specification | Notes |
|---|---|---|---|
| 4mm copper tube | 2m | 4mm OD, refrigeration/plumbing grade | Plumbing supply |
| 90° copper elbow fittings | 4 | 4mm, sweat fit | Plumbing supply |
| Electrical solder, lead-free | 1 tube | 60/40 or 63/37 Sn/Pb (or SAC305) | Electronics supply |
| Copper foil tape (Faraday shield) | 1 roll | 25mm wide, conductive adhesive | DigiKey |
| GT2 timing belt | 1 | 300mm length, 6mm wide, 2mm pitch | Robotics supply |
| GT2 pulley, 20 tooth | 1 | 5mm bore, for motor shaft | Robotics supply |
| GT2 pulley, 60 tooth | 1 | 8mm bore, for loop shaft | Robotics supply |
| NEMA 17 stepper motor | 1 | 1.5A, 12V, 200 steps/rev, 40 N·cm holding | Robotics supply |
| DRV8825 driver module | 1 | With trim pot, heat sink | AliExpress |
| J310 JFET | 2 | N-channel, TO-92, for buffer amplifier | DigiKey |
| AS5048A magnetic encoder | 1 | SPI, 14-bit, absolute, for loop position | DigiKey |
| Diametrically magnetized magnet | 1 | 6mm diameter, for AS5048A | Supermagnete |
| Varactor diode BB112 | 2 | 2–10 pF, 30V, SOD-323 | DigiKey |
| Small signal relay, DPDT | 4 | 5V coil, for capacitor bank switching | DigiKey |
| Fixed capacitors (SMD) | 1 set | Values per band (see Section 1.5 table) | DigiKey |
| 8mm shaft, 150mm | 1 | Aluminum, straight, for loop shaft | Metals supply |
| Flanged bearing, 8mm | 2 | For loop shaft bearing block | Robotics supply |
| GT2 belt idler, 3mm bore | 1 | For belt tensioning (optional) | Robotics supply |

## 11.5 CYD Display Components

| Item | Qty | Specification | Notes |
|---|---|---|---|
| ESP32-2432S028R (CYD) | 1 | 320×240 TFT, ILI9341, XPT2046 touch, ESP32 | AliExpress ("Cheap Yellow Display") |
| USB-C cable | 1 | For programming and power | Standard |
| CYD enclosure (optional) | 1 | Printed PLA/PETG, with display cutout | Print from STL file |
| Battery + TP4056 (optional) | 1 set | 18650 + 3.7V charger + 5V boost | For portable CYD operation |

---

<a name="appendix-a"></a>
# APPENDIX A — BAND-BY-BAND OPERATING NOTES

## 160 Meters (1.8–2.0 MHz)

The 160-meter band presents the most challenging array deployment of any HF band. A quarter-wave element length of 40.5 meters requires a very large field site. The Watson-Watt baseline of 40.5 meters will rarely be achievable in portable field work; shortened elements (loaded or folded) can reduce the physical size at a cost of 10–20 dB in sensitivity. Ground conductivity strongly affects the elevation angle of arriving signals, and low-angle groundwave propagation at 160M means that the incoming angle is nearly horizontal — ideal for the vertical element. The Adcock array is the recommended technique; the rotary loop is an excellent alternative because its small physical size makes up for the element length limitation of the Adcock. Site effects (nearby buildings, buried conductors, soil moisture variation) have more impact at 160M than any other HF band. The band is used extensively for AM broadcast and amateur medium-wave operation; the primary RDF use case is broadcast station identification and NVIS propagation research.

## 80 Meters (3.5–4.0 MHz)

The 80M band is a workhorse HF RDF band. The λ/4 element length of 20.5 meters is achievable at a large park or open field. Skywave propagation begins at distances of about 300 km at night, with groundwave reliable to 400–600 km on typical soil. The Watson-Watt system performs well on 80M AM and SSB signals. Bearing accuracy on NVIS signals (arriving from near-vertical angles) is degraded relative to groundwave signals; use the SNR and confidence indicators to assess bearing quality. The rotary loop is effective on 80M with the 400mm loop, achieving null depths of 22–28 dB. Noise environment: 80M is moderately noisy (power line harmonics, plasma displays, switching supplies); select a site well away from power lines. Primary use case: amateur NVIS beacon tracking, broadcast station identification, emergency communications RDF support.

## 60 Meters (5.3–5.4 MHz)

The 60M band (five channelized frequencies in the US) is used primarily for NVIS emergency communications. Element length 14.1 meters. The Watson-Watt array is well-suited for 60M. Propagation is NVIS during daytime and transitions to skywave at night. The band has relatively low interference compared to 40M and 80M. Primary use case: emergency communications station identification and direction finding.

## 40 Meters (7.0–7.3 MHz)

The 40M band is the optimal band for HF RDF operations. The 10.5m element length is easily managed with telescoping aluminum rods. Watson-Watt accuracy is typically best on 40M due to the balanced element size and moderate frequency (less ground effect dispersion than 80M, less multipath than 20M). Interference environment: 40M is heavily used by broadcast stations, amateurs, and utilities worldwide. The ADF system should be used during relatively quiet periods or with a narrow-band bandpass filter in the receive chain to isolate the target signal. The Doppler ring technique is not applicable below VHF; Watson-Watt or rotary loop are the choices for 40M. Primary use cases: amateur NVIS, utility station identification, emergency preparedness.

## 30 Meters (10.1–10.15 MHz)

The 30M band is narrow (50 kHz), exclusively CW and data modes (no phone). Element length 7.4 meters is compact and portable. The Watson-Watt and rotary loop systems both work well on 30M. The rotary loop is preferred for CW signals, as its narrow bandwidth helps reject adjacent interference. The band has moderate propagation distance for RDF use. Primary use case: CW beacon tracking, digital mode station identification.

## 20 Meters (14.0–14.35 MHz)

The 20M band is the primary DX HF band. Element length 5.3 meters is very portable. Watson-Watt works well, but incoming signals from DX paths arrive at low elevation angles that may increase multipath from nearby terrain features. The 20M band is active 24 hours. Short skip (ground wave to 200km) and long skip (> 4000km) coexist; the ADF system cannot distinguish between them. Bearing accuracy on high-angle NVIS signals is reduced; best accuracy is achieved on low-angle groundwave signals below 200km distance. Primary use: DX station direction, propagation research, beacon tracking.

## 17 Meters (18.068–18.168 MHz)

The 17M band (WARC, non-contest) is quieter than 15/20M. Element length 4.1 meters. Watson-Watt performance is good. The band supports moderate DX; good for 1000–10,000 km path RDF on SSB signals. Primary use: propagation research.

## 15 Meters (21.0–21.45 MHz)

Element length 3.5 meters. Watson-Watt is compact and easily deployed. 15M supports long-distance DX when the solar cycle is favorable. The rotary loop is the highest band at which the 400mm loop is usefully sensitive (requires careful varactor tuning). Primary use: DX propagation research, amateur SSB/CW station identification.

## 12 Meters (24.89–24.99 MHz)

Element length 3.0 meters. Watson-Watt works well with reduced baseline. The 400mm rotary loop is marginal at 12M (sensitivity reduced by loop being electrically smaller relative to wavelength). Watson-Watt preferred. Primary use: propagation research, beacon tracking.

## 10 Meters (28.0–29.7 MHz)

Element length 2.6 meters. Watson-Watt is compact (2.6m elements). 10M opens during solar maximum to worldwide DX and closes completely during solar minimum. A very useful band for RDF when conditions are good. Primary use: DX beacon tracking, sporadic-E propagation research.

## 6 Meters (50–54 MHz)

Element length 1.5 meters. Watson-Watt array is small and easily portable. 6M exhibits sporadic-E and meteor scatter propagation; bearings may be unstable on propagated signals. On local groundwave signals (< 100km), Watson-Watt achieves excellent accuracy. Doppler ring is possible at 6M with a printed disk of 950mm diameter. Primary use: sporadic-E research, local emergency communications RDF, foxhunting.

## 2 Meters (144–148 MHz)

The 2M band is the primary VHF RDF band. The Doppler ring with 1027mm diameter array is the standard technique. Array fits on a standard camera tripod and can be deployed in 2 minutes. Bearing accuracy ±1°–3° on typical FM signals. Watson-Watt is also feasible with 480mm elements (compact). Primary use cases: foxhunting, APRS vehicle tracking, emergency ARES/RACES operations, repeater identification.

## 1.25 Meters (222–225 MHz)

The 1.25M band (North American allocation) uses a 676mm Doppler disk. The band is lightly used relative to 2M; primary users are amateur repeaters and some digital modes. The Doppler system works identically to 2M but with a smaller, lighter array disk. Primary use: foxhunting on 222 MHz band.

## 70 Centimeters (420–450 MHz)

The 70cm band uses a 330mm Doppler disk — compact enough to be handheld with a small mast. The band includes amateur FM, repeaters, satellite downlinks, and some digital modes. For the FST3253 switch (rated to ~400 MHz), performance at 450 MHz may be degraded; use the PE4259 at higher 70cm band segments. Primary use: foxhunting, satellite beacon tracking, ATV direction finding.

## 33 Centimeters (900–928 MHz)

The 33cm band includes ISM devices (915 MHz), amateur repeaters, and data systems. The 163mm Doppler disk is small and lightweight. The PE4259 switch is required at this frequency. Array element length is 77mm — short enough to print on the disk itself. Primary use: ISM transmitter locating, amateur 900 MHz operations.

## 20 Centimeters (1240–1300 MHz)

The 20cm band (23cm band in Region 1) is the highest band in this ADF suite. The 115mm Doppler disk is very small (fits in a shirt pocket) and is mounted directly on the receiver or on a small tripod adapter. The PE4259 switch is required; the ESP32 GPIO switching rate must be 2000 Hz for this band for adequate modulation depth. Element length 55mm is printed as stubs on the disk. Primary use: amateur microwave foxhunting, GPS L1/L2 antenna research (note: GPS is receive-only; GPS receivers cannot be direction-found in the traditional sense — this band is for amateur microwave transmitters).

---

<a name="appendix-b"></a>
# APPENDIX B — WATSON-WATT ALGORITHM DERIVATION

## B.1 Signal Model

Consider a linearly polarized plane wave arriving from azimuthal bearing θ (measured clockwise from True North) and elevation angle α above the horizontal. The electric field component at a point in space is:

```
E(t, r) = E₀ × cos(2πft - k·r + φ₀)
```

Where:
- E₀ = field amplitude (V/m)
- f = signal frequency (Hz)
- k = wave vector (magnitude 2π/λ, direction of propagation)
- r = position vector
- φ₀ = initial phase

## B.2 Adcock Element Signals

For the North element located at position (0, +d/2, 0) (d = array baseline):

```
V_N(t) = h_eff × E₀ × cos(2πft - k_y × d/2 + φ₀)
```

Where h_eff is the effective height of the monopole element, and k_y is the northward component of the wave vector:

```
k_y = (2π/λ) × sin(θ) × cos(α)
```

For the South element at (0, -d/2, 0):

```
V_S(t) = h_eff × E₀ × cos(2πft + k_y × d/2 + φ₀)
```

## B.3 Differential Output (N-S Pair)

The differential output of the N-S pair is:

```
V_NS = V_N - V_S
     = h_eff × E₀ × [cos(2πft - k_y×d/2 + φ₀) - cos(2πft + k_y×d/2 + φ₀)]
```

Using the identity cos(A - B) - cos(A + B) = 2×sin(A)×sin(B), with A = 2πft + φ₀ and B = k_y×d/2:

```
V_NS = 2 × h_eff × E₀ × sin(2πft + φ₀) × sin(k_y × d/2)
```

For small arrays (k_y × d/2 << 1 radian, i.e., d << λ), sin(k_y × d/2) ≈ k_y × d/2:

```
V_NS ≈ 2 × h_eff × E₀ × sin(2πft + φ₀) × (k_y × d/2)
     = h_eff × E₀ × d × k_y × sin(2πft + φ₀)
     = h_eff × E₀ × d × (2π/λ) × sin(θ) × cos(α) × sin(2πft + φ₀)
```

The phase-shifted form (using cos rather than sin) is achieved by a 90° phase reference, but for RMS computation the phase is irrelevant:

```
V_NS(t) = A × sin(θ) × cos(α) × cos(2πft + φ)
```

Where A = h_eff × E₀ × d × (2π/λ) × [constant amplitude factor].

Similarly for the E-W pair:

```
V_EW(t) = A × cos(θ) × cos(α) × cos(2πft + φ)
```

Both signals are at the same frequency, in phase with each other (same phase φ), with amplitudes proportional to sin(θ) and cos(θ) respectively.

## B.4 RMS Computation

The RMS value of V_NS over one or more complete cycles of the carrier:

```
V_NS_rms = (A/√2) × |sin(θ)| × cos(α)
V_EW_rms = (A/√2) × |cos(θ)| × cos(α)
```

The term cos(α) (elevation angle factor) affects both channels equally and cancels in the ratio. The amplitude factor A also cancels.

## B.5 The Watson-Watt Bearing Formula

The bearing is extracted using:

```
θ = atan2(V_NS_rms, V_EW_rms)
```

The atan2 function, defined as atan2(y, x) = arctan(y/x) with correct quadrant selection based on the signs of x and y, gives the bearing directly:

```
θ = atan2(sin(θ), cos(θ)) = θ    (by definition of atan2)
```

In practice, V_NS_rms and V_EW_rms are computed by the firmware as:

```
V_NS_rms = sqrt( (1/N) × sum_{n=0}^{N-1} v_NS[n]^2 )
V_EW_rms = sqrt( (1/N) × sum_{n=0}^{N-1} v_EW[n]^2 )
```

Note: The RMS computation loses sign information. The atan2 result is always in the first quadrant (0°–90°) without sign-preserving information. To recover the full 0°–360° range, the firmware uses the sample-by-sample correlation between the N-S and E-W waveforms, which carries sign information:

```
Sign_NS = sign of mean(v_NS × reference_cos)   // Cross-correlate with cosine reference
Sign_EW = sign of mean(v_EW × reference_cos)

θ_full = atan2(Sign_NS × V_NS_rms, Sign_EW × V_EW_rms)
```

Where reference_cos is a synthetic cosine generated by the firmware at the estimated signal frequency. In practice, the sense antenna provides the reference.

## B.6 Ambiguity Discussion

The atan2 function maps (sin θ, cos θ) → θ correctly for all θ in -180° to +180°. However, the magnitudes |sin θ| and |cos θ| (which is what the RMS values measure, because squaring loses the sign) map the same value for θ and θ+180°:

```
|sin(θ)| = |sin(θ + 180°)|
|cos(θ)| = |cos(θ + 180°)|
```

Therefore, without the sense antenna, the bearing is ambiguous by 180°: the atan2(|sin θ|, |cos θ|) result is indistinguishable between θ and θ+180°.

**Sense antenna resolution**: The sense antenna signal V_SENSE has no directional dependence (omnidirectional). When combined with the loop output (either N-S or E-W):

```
V_combined = V_loop + jV_sense
```

This produces a cardioid pattern: V_combined is maximum when the loop and sense signals are in phase, and V_combined = 0 when they cancel. The cardioid has only one null and one maximum.

The firmware compares the phase of V_SENSE with V_NS: if in phase (cos of phase difference > 0), the transmitter is in the positive sin(θ) half-space (bearing 0°–180° from the N direction). If out of phase, the transmitter is in the negative sin(θ) half-space (bearing 180°–360°). The 180° ambiguity is thereby resolved.

---

<a name="appendix-c"></a>
# APPENDIX C — DOPPLER EQUATION DERIVATION

## C.1 Classical Doppler Shift

A receiver moving with velocity v toward a stationary transmitter at frequency f₀ observes a shifted frequency:

```
f_received = f₀ × (1 + v/c)      (approaching)
f_received = f₀ × (1 - v/c)      (receding)
```

More precisely, for a receiver moving at angle β relative to the direction toward the transmitter:

```
f_received = f₀ × (1 + (v/c) × cos(β))
```

The instantaneous frequency shift is:

```
Δf(t) = f₀ × (v(t)/c) × cos(β(t))
```

## C.2 Rotating Receiver

If the receiver antenna moves in a circle of radius r at angular velocity ω (rad/s), the tangential velocity is v = r × ω, and the angle β between the velocity vector and the direction to the transmitter varies as:

```
β(t) = θ - φ(t) - 90°
```

Where θ = bearing to transmitter, φ(t) = ωt = instantaneous angular position of the antenna.

The term cos(β) becomes:

```
cos(θ - φ(t) - 90°) = sin(θ - φ(t)) = sin(θ)cos(φ(t)) - cos(θ)sin(φ(t))
```

The Doppler frequency shift is:

```
Δf(t) = (f₀ × v / c) × sin(θ - φ(t))
       = (f₀ × r × ω / c) × sin(θ - ωt)
```

This is a sinusoidal frequency modulation at the rotation rate ω (in Hz: f_switch = ω/(2π)), with phase equal to the bearing θ. The FM index (maximum frequency deviation) is:

```
β_FM = f₀ × r × ω / (c × f_switch × 2π)
     = f₀ × r / (c / (2π) × ...) 
```

Simplified: For the 2M band at 146 MHz, array radius r = 0.5m, ω = 2π × 1000 rad/s:

```
Δf_max = (146×10⁶ × 0.5 × 2π×1000) / (3×10⁸)
        = (146×10⁶ × 0.5 × 6283) / (3×10⁸)
        = 458×10⁶ / (3×10⁸)
        = 1.53 Hz   (peak Doppler shift)
```

This 1.53 Hz peak deviation, at a 1000 Hz modulation rate, produces a 1 kHz FM subcarrier with an FM index of:

```
m_FM = Δf_max / f_switch = 1.53 / 1000 = 0.00153 radians
```

This is a very small FM index; the sidebands are very weak. This is why the Goertzel algorithm must extract the phase of the subcarrier with high precision despite its very low modulation depth.

## C.3 Electronic Switching Equivalence

The electronic switching system does not continuously rotate a single antenna; instead, it discretely samples each of four fixed antennas in sequence. This discrete sampling creates a phase-stepped approximation of the continuously rotating receiver.

For a 4-element array switched at rate f_s = 1000 Hz, each element is active for T = 1/(4 × f_s) = 250 µs. The received signal from element i at angular position φ_i = i × 90° (for i = 0, 1, 2, 3) during its active interval is:

```
x_i(t) = E₀ × cos(2π f₀ t + k × r × cos(θ - φ_i) + φ_0)
```

The phase of the signal from element i is:

```
φ_signal_i = k × r × cos(θ - φ_i)
```

As i advances from 0 to 3 (N → E → S → W), the phase of the sampled signal steps as cos(θ - 0°), cos(θ - 90°), cos(θ - 180°), cos(θ - 270°), which is sin(θ), sin(θ - 90°), sin(θ - 180°), sin(θ - 270°). This is a sinusoidal phase sequence at the switching frequency, and its phase offset is exactly θ, the bearing to the transmitter.

The FM discriminator output of the receiver converts this phase modulation to an audio tone at frequency f_switch = 1000 Hz, with phase = θ.

## C.4 Goertzel Algorithm Equations

The Goertzel algorithm computes the DFT at a single frequency bin k₀ corresponding to the target frequency f_target = f_switch = 1000 Hz.

**Parameters**:
```
N = 1024       (block size, samples)
f_s = 10000    (sample rate, Hz)
f_target = 1000 Hz
k₀ = N × f_target / f_s = 1024 × 1000 / 10000 = 102.4
```

Since k₀ must be an integer for the standard Goertzel algorithm, choose N and f_s such that k₀ is exactly an integer. With N = 1000, f_s = 10000, k₀ = 100 exactly.

**Coefficient**:
```
ω₀ = 2π × k₀ / N = 2π × 100 / 1000 = 0.6283 rad/sample
coeff = 2 × cos(ω₀) = 2 × cos(0.6283) = 2 × 0.8090 = 1.6180
```

**Recursive loop** (for samples x[0] through x[N-1]):
```
Initialize: s_prev = 0, s_prev2 = 0

For n = 0 to N-1:
    s = x[n] + coeff × s_prev - s_prev2
    s_prev2 = s_prev
    s_prev = s
```

**Result extraction**:
```
X_re = s_prev - s_prev2 × cos(ω₀)
X_im = s_prev2 × sin(ω₀)

Magnitude:  |X| = sqrt(X_re² + X_im²)
Phase:      φ = atan2(X_im, X_re)   [radians]
```

**Bearing extraction**:
```
θ_measured = φ - φ_reference + calibration_offset

where φ_reference = phase of the 1000 Hz switching reference
     (computed from the switch ISR cycle counter in the firmware)
```

The SNR metric for the Goertzel result is:

```
SNR = 20 × log10(|X| / σ_noise)
```

Where σ_noise is the standard deviation of the ADC samples (estimated as the total block RMS minus the Goertzel tone contribution).

## C.5 Phase to Bearing Conversion

The phase extracted by Goertzel is in the range -π to +π radians. Convert to bearing:

```
θ_raw = φ_measured × (180° / π)   [range -180° to +180°]

θ_mag = θ_raw + calibration_offset_deg + array_orientation_offset_deg

if θ_mag < 0:   θ_mag += 360
if θ_mag >= 360: θ_mag -= 360

θ_true = θ_mag + declination   [true bearing]
```

---

<a name="appendix-d"></a>
# APPENDIX D — QUICK REFERENCE CARD

```
╔══════════════════════════════════════════════════════════════════════════════╗
║         ADF SUITE  —  QUICK REFERENCE  —  TM-ADF-001 Rev A                 ║
╠══════════════════════════════════════════════════════════════════════════════╣
║  WATSON-WATT HF (esp32_adf_hf)   WiFi: RF-ADFHF  BT: RF-ADFHFinder         ║
╠═══════════════╦════════════════════╦══════════════════════════════════════════╣
║ BAND n        ║ Set band 0–10      ║ 0=160M 1=80M 2=60M 3=40M 4=30M        ║
║               ║                    ║ 5=20M 6=17M 7=15M 8=12M 9=10M 10=6M   ║
║ STATUS        ║ Print full status  ║ JSON: bearing, SNR, GPS, compass       ║
║ CAL START     ║ Begin compass cal  ║ Rotate unit 2× full turns              ║
║ CAL END       ║ End compass cal    ║ Saves offsets to NVS                   ║
║ DECL f        ║ Declination        ║ f = degrees, + East, - West            ║
║ OFFSET f      ║ Bearing offset     ║ f = degrees added to computed bearing  ║
║ GAIN NS f     ║ N-S gain           ║ f = multiplier (e.g. 1.05)            ║
║ GAIN EW f     ║ E-W gain           ║ f = multiplier                         ║
║ RESET         ║ Software reset     ║ Reboots ESP32                          ║
║ HELP          ║ List commands      ║                                        ║
╠═══════════════╩════════════════════╩══════════════════════════════════════════╣
║  DOPPLER VHF (esp32_adf_vhf)      WiFi: RF-ADFVHF  BT: RF-ADFVHFinder      ║
╠═══════════════╦════════════════════╦══════════════════════════════════════════╣
║ BAND n        ║ Set band 0–4       ║ 0=2M 1=1.25M 2=70cm 3=33cm 4=20cm     ║
║ STATUS        ║ Full status        ║ JSON: bearing, RSSI, GPS, compass      ║
║ CAL START f   ║ Start Dopp cal     ║ f = known true bearing of transmitter  ║
║ CAL ADD f     ║ Add cal point      ║ f = known true bearing (0, 90, 180...) ║
║ CAL END       ║ End cal, save      ║ Saves phase offset to NVS              ║
║ SQUELCH n     ║ Set squelch        ║ n = RSSI threshold in dBm (neg number) ║
║ DECL f        ║ Declination        ║ f = degrees                            ║
║ HELP          ║ List commands      ║                                        ║
╠═══════════════╩════════════════════╩══════════════════════════════════════════╣
║  ROTARY LOOP (esp32_rotary_loop)   WiFi: RF-ADFLOOP  BT: RF-ADFLOOPFinder  ║
╠═══════════════╦════════════════════╦══════════════════════════════════════════╣
║ SCAN          ║ Start 360° scan    ║ Takes ~3 min; updates OLED during scan  ║
║ STOP          ║ Halt motor         ║ Motor stops at current position         ║
║ HOME          ║ Go to 0°           ║ Returns loop to reference north mark    ║
║ MOVE deg      ║ Go to bearing      ║ deg = 0.0–359.9, absolute              ║
║ BAND n        ║ Set band 0–6       ║ 0=160M 1=80M 2=60M 3=40M 4=30M        ║
║               ║                    ║ 5=20M 6=15M                            ║
║ SENSE ON      ║ Enable sense       ║ Activates sense relay for 180° resolution ║
║ SENSE OFF     ║ Disable sense      ║                                        ║
║ TUNE          ║ Auto-tune loop     ║ Sweeps varactor, finds peak RSSI (~8s) ║
║ STATUS        ║ Full status        ║ JSON: pos, null, depth, GPS, compass   ║
║ HELP          ║ List commands      ║                                        ║
╠═══════════════╩════════════════════╩══════════════════════════════════════════╣
║  WEB API  (all systems):  http://192.168.4.1                                ║
╠══════════════════════╦═════════════╦═══════════════════════════════════════════╣
║ GET /api/status      ║ GET         ║ Full JSON status                        ║
║ GET /api/bearing     ║ GET         ║ Bearing + SNR JSON subset               ║
║ POST /api/band       ║ POST        ║ {"band":n}                              ║
║ POST /api/cal        ║ POST        ║ {"action":"start"} or {"action":"end"}  ║
║ POST /api/decl       ║ POST        ║ {"decl":f}                              ║
║ POST /api/scan       ║ POST        ║ {"action":"scan"/"stop"/"home"}         ║
║ POST /api/tune       ║ POST        ║ (no body)                               ║
╠══════════════════════╩═════════════╩═══════════════════════════════════════════╣
║  OLED LINE LAYOUT (all systems):                                            ║
║  Line 1: MODE / BAND                                                        ║
║  Line 2: BRG: XXX°M (YYY°T)                                                ║
║  Line 3: SNR / RSSI / NULL DEPTH + CONFIDENCE                              ║
║  Line 4: GPS status / MOTOR status / BT status                             ║
╠═════════════════════════════════════════════════════════════════════════════╣
║  FAULT CODES:                                                               ║
║  E01=COMPASS FAIL  E02=GPS NO DATA  E03=ADC OVERFLOW  E04=CAL INVALID      ║
║  E05=NS OPEN       E06=EW OPEN      E07=MOTOR FAULT   E08=ENCODER ERR      ║
║  E09=WIFI FAIL     E10=NVS CORRUPT  E11=ADC CLIP      E12=BT FAIL          ║
╠═════════════════════════════════════════════════════════════════════════════╣
║  CONFIDENCE: HI=SNR>20dB  MED=SNR 10–20dB  LO=SNR<10dB  INVALID=no signal ║
║  TRUE BRG = MAG BRG + DECL  (+ East / - West)                              ║
╚═════════════════════════════════════════════════════════════════════════════╝
```

---

*End of TM-ADF-001 Rev A*

*DOCUMENT NUMBER: TM-ADF-001 Rev A*
*CLASSIFICATION: UNCLASSIFIED*
*PREPARED BY: TM Engineering*
*DATE: 2026-05-02*
