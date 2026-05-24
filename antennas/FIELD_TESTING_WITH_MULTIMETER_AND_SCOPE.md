# Field Testing with Multimeter and Oscilloscope
## Fluke 15B+ and FNIRSI 100 MHz — Complementary Diagnostic Tools

---

## Overview

The **Fluke 15B+ multimeter** and **FNIRSI 100 MHz oscilloscope** are excellent companions to nanoVNA and tinySA for antenna field testing. Together, they provide a three-tier diagnostic system:

| Instrument | Specialization | What It Measures |
|---|---|---|
| **nanoVNA** | Antenna matching | Impedance, SWR, resonance, reflection coefficient |
| **tinySA** | Spectrum analysis | Frequency response, radiation pattern, harmonics |
| **Fluke 15B+** | Component diagnostics | DC resistance, continuity, voltage, basic safety checks |
| **FNIRSI scope** | Signal quality | Waveform, modulation, pulse shape, distortion |

---

## SECTION I — FLUKE 15B+ MULTIMETER FOR ANTENNA TESTING

### 1.1 Fluke 15B+ Capabilities & Limitations

**Specifications:**
- DC Voltage: 0.1 mV – 1000V accuracy ±0.5%
- AC Voltage: 0.1 mV – 1000V (50/60 Hz, not broadband)
- Resistance: 0.1Ω – 40 MΩ accuracy ±0.5%
- Current: 100 μA – 20A (depending on jack configuration)
- Continuity: Audible beep < 50Ω
- Diode testing: 0.1–2V forward drop detection
- Frequency: 10 Hz – 100 kHz (optional, depends on model variant)

**What it CAN'T do for antennas:**
- ✗ Measure impedance at RF (100 kHz+) — only DC/low-freq
- ✗ Measure SWR or reflection coefficient
- ✗ Detect resonance
- ✗ Measure RF voltage or current accurately at antenna frequencies

**What it CAN do for antenna diagnostics:**
- ✓ Verify DC resistance of coils (quality check)
- ✓ Test continuity of antenna wire (no open circuits)
- ✓ Measure resistance of solder joints
- ✓ Verify trap capacitor is not shorted
- ✓ Check feedline shield continuity
- ✓ Measure DC bias voltages in pre-amps or amplifiers

### 1.2 Pre-Test Component Verification

Before connecting antenna to nanoVNA, use multimeter to verify physical integrity:

#### Procedure 1: Continuity Test (Antenna Wire)

**Purpose:** Ensure antenna wire is not broken or open-circuited.

**Steps:**
1. Set Fluke 15B+ to **Ω (resistance)** mode
2. Disconnect antenna feedline (do NOT measure while connected to transmitter)
3. Touch red probe to center conductor at feedpoint, black probe to end of antenna arm
4. Expected reading: **< 1Ω** (good wire with low resistance)
5. If reading is **∞ (infinite)**: Wire is broken; visually inspect for cuts or corrosion
6. Repeat for both antenna arms and verify symmetry (left ≈ right resistance)

**Example data:**
```
Left arm:   0.4Ω  (14 AWG copper wire, 5.0m → ~0.08Ω/m × 5 = 0.4Ω) ✓
Right arm:  0.39Ω (similar)
Shield:     0.1Ω  (excellent ground continuity)
```

#### Procedure 2: Coil DC Resistance Measurement

**Purpose:** Verify loading coil wire is intact and estimate wire gauge/turns accuracy.

**Steps:**
1. Set Fluke 15B+ to **Ω mode**
2. Measure DC resistance across loading coil terminals (at both ends)
3. Compare to theoretical value: R_dc = ρ × L / A
   - ρ = resistivity (Cu: 1.68 × 10⁻⁸ Ω·m at 20°C)
   - L = total wire length (coil length × turns)
   - A = wire cross-sectional area (πr²)

**Example (40M compact dipole coil):**
```
Design parameters:
  Wire gauge: AWG #22
  Turns: 14
  Coil diameter: 0.75" (wound on 3/4" PVC form)
  Coil length (deployed): 2.0"
  Estimated wire length: π × 0.75" × 14 turns × 2.54 cm/inch = 84.4 cm

DC Resistance calculation:
  AWG #22 diameter: 0.644 mm
  Area: π × (0.322 mm)² = 0.326 mm² = 3.26 × 10⁻⁷ m²
  R_dc = (1.68 × 10⁻⁸ Ω·m) × (0.844 m) / (3.26 × 10⁻⁷ m²)
       = 0.0435 Ω ≈ 43.5 mΩ

Measured with Fluke 15B+: 0.045Ω = 45 mΩ ✓ MATCH
```

**Interpretation:**
- If measured R_dc **matches design calculation** (±10%): Coil is correctly wound
- If measured R_dc is **2–3× higher** than design: Possible corrosion on coil wire or poor contact at terminals
- If measured R_dc is **much lower** (< 50% design): Possible short turn or copper-plated coil (different material)

#### Procedure 3: Trap Capacitor DC Test

**Purpose:** Verify capacitor is not shorted or open.

**Steps:**
1. Set Fluke 15B+ to **diode test mode** (∆| symbol on display)
2. Disconnect capacitor from circuit (if not integrated)
3. Touch red probe to one terminal, black probe to other
4. Expected readings:
   - **Good capacitor:** Display shows brief resistance spike, then stabilizes to ∞ (open)
   - **Shorted capacitor:** Displays 0Ω continuously (capacitor has failed, path between terminals)
   - **Leaky capacitor:** Displays high resistance (100 kΩ–10 MΩ) but not ∞

**Example results:**
```
15 pF fixed mica capacitor:
  Diode mode: Brief spike to ~100Ω, then ∞ ✓ GOOD

Variable trimmer capacitor:
  Diode mode: Brief spike to ~50Ω, then ∞ ✓ GOOD
  (Resistance varies slightly as rotor turns due to diode mode's small DC bias)
```

#### Procedure 4: Feedline Shield Continuity

**Purpose:** Verify coaxial cable shield is continuous (no breaks in shield braid).

**Steps:**
1. Set Fluke 15B+ to **Ω mode**
2. Measure resistance between shield at feedpoint and shield at radio connector
3. Expected: **< 0.5Ω** for good cable
4. If > 5Ω: Cable has damaged or open shield; consider replacement

**Example data (32 ft RG-8):**
```
Measured shield resistance: 0.18Ω ✓ GOOD
(RG-8 shield resistance: ~0.005Ω per foot × 32 ft = 0.16Ω nominal)
```

### 1.3 Feed Network Component Verification

For antennas with matching networks, traps, or impedance transformers:

#### Coax Velocity Factor Verification

**Purpose:** Verify feedline velocity factor (affects impedance measurement in nanoVNA).

**Steps:**
1. Measure physical length of coax with tape measure: L_physical = _____ ft
2. Measure DC loop resistance (center to shield at opposite end):
   - For RG-58: ~1.0 Ω per 100 ft
   - For RG-8: ~0.3 Ω per 100 ft
   - For LMR-400: ~0.2 Ω per 100 ft

3. Use measured DC resistance to verify cable type

**Example (32 ft test cable):**
```
Measured center-to-shield resistance: 0.09Ω
Expected for RG-8 (0.3Ω/100 ft): 0.3 × (32/100) = 0.096Ω ✓ MATCH
Confirmed: Cable is RG-8 with VF ≈ 0.66
```

#### Balun/Transformer DC Resistance

**Purpose:** Verify transformer winding is intact (not open).

**Steps:**
1. Set Fluke 15B+ to **Ω mode**
2. Measure resistance across primary winding (low-impedance side)
3. Measure resistance across secondary winding (high-impedance side)
4. Expected: Both should be low (< 10Ω), not infinite

**Example (9:1 unun for end-fed antenna):**
```
Primary winding (50Ω side): 0.8Ω ✓ GOOD
Secondary winding (450Ω side): 6.2Ω ✓ GOOD
(Higher resistance on secondary is normal due to thinner wire)

If secondary reads ∞: Transformer winding is open, needs replacement
```

---

## SECTION II — FNIRSI 100 MHz OSCILLOSCOPE FOR ANTENNA TESTING

### 2.1 FNIRSI Specifications & Capabilities

**Typical FNIRSI 100 MHz oscilloscope specs:**
- Bandwidth: 100 MHz (can safely measure signals up to ~80 MHz)
- Channels: 2–4 input channels (depending on model)
- Sample rate: 500 MS/s (megasamples/second) — 2 ns time resolution
- Display: 3.5"–5" LCD color screen
- Time base: 10 ns/div to 5 sec/div
- Vertical range: ±10V per channel typical
- Triggering: Auto, normal, rising/falling edge
- Measurements: Frequency, period, voltage (Vpp, RMS, average)
- Data logging: Internal storage or USB export
- Power: Battery + USB charging

**Advantages for field testing:**
- ✓ Portable (battery-powered, ~1 lb, fits in backpack)
- ✓ Shows waveform quality in real-time
- ✓ Can measure frequency and voltage simultaneously
- ✓ Detects distortion and harmonics in time domain
- ✓ Monitors modulation quality (AM, FM, CW keying)
- ✓ Useful for debugging feed system problems

**Limitations:**
- ✗ NOT suitable for antenna impedance measurement
- ✗ Limited to ~80 MHz reliable measurement (100 MHz spec is upper limit)
- ✗ Requires RF probe for safe measurement (scope input limited to 10V)
- ✗ Cannot directly measure antenna radiation pattern
- ✗ AC coupled input (blocks DC, must use AC mode)

### 2.2 RF Probe Selection & Usage

**CRITICAL:** Do NOT connect antenna feedpoint directly to oscilloscope input. Use **high-voltage RF probe** rated for your frequency band.

**Recommended probe types:**

1. **Passive 10:1 probe** (1–100 MHz, ~$30–50)
   - Input impedance: ~10 MΩ || 10 pF
   - Attenuation: 10:1 (reduces signal by 10×)
   - Display reading must be multiplied by 10
   - Lower distortion, minimal loading
   - Best choice for antenna testing

2. **Active FET probe** (1–500 MHz, ~$100+)
   - Input impedance: >10 MΩ || <1 pF (minimal loading)
   - Low attenuation (1:1)
   - More expensive, better for precision measurement
   - May pick up noise if leads are long

3. **Homebrew probe** (high-impedance resistive divider)
   - 9 MΩ + 1 MΩ resistors in series
   - 9:1 attenuation, high impedance
   - Low cost (~$5), acceptable for field use

**Probe setup:**
```
RF Source (feedpoint)
    ↓
    +--- 1 MΩ resistor --- scope input
    |
    +--- 9 MΩ resistor --- ground
    |
```

### 2.3 Measuring Transmitted Signal Waveform

**Purpose:** Verify transmitter is producing clean signal without distortion or spurious emissions.

**Setup:**
1. Connect RF probe to antenna feedpoint or (safer) coupling loop near antenna
2. Set oscilloscope:
   - Input coupling: **AC** (blocks DC component)
   - Trigger: **Auto** (auto-triggers on rising edge)
   - Time base: **1 µs/div** for CW, **100 ns/div** for fast rise-time measurement
   - Vertical: **1 V/div** (adjust for best display)

3. Transmit steady test signal (CW, 3–5 watts)

**Expected waveform (CW):**
```
  ▲ Scope trace (1 MHz example):
  │
  │   ▲▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
  │  │▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁│
  │  └──────────────────────────────────┘
  │        1 oscillation = 1 period = 1 µs at 1 MHz
  │
  └──────────────────────────────────────
```

**Key measurements:**
- **Frequency:** Display typically shows auto-measured frequency (or count peaks)
- **Peak-to-peak voltage:** Vpp = (# divisions × V/div) × probe attenuation
  - Example: 2.5 div × 1 V/div × 10 (probe) = 25 Vpp at feedpoint
- **Rise time:** Time for signal to go from 10% to 90% amplitude
  - Fast rise (<10 ns) suggests clean signal
  - Slow rise (>100 ns) suggests impedance mismatch or cable loss

**Common waveform problems:**

```
Problem 1: Distorted/clipped peaks
  ▲
  │ ╱╲  ╱╲  ← Clipping (transmitter overdriven or antenna not loaded)
  │╱  ╲╱  ╲
  └─────────
  
  Diagnosis: Check power level, verify antenna SWR < 2.0

Problem 2: Ringing (damped oscillation)
  ▲
  │    ╱╲╱╲╱╲← Ringing (impedance mismatch, cable reflection)
  │  ╱│  │  │╲
  │ ╱ │  │  │ ╲
  └──────────────
  
  Diagnosis: Use nanoVNA to check SWR, TDR for cable faults

Problem 3: Envelope ripple (modulation artifact)
  ▲
  │  ┌─ ─ ─ ─ ─┐
  │ ╱ │ ╱╲╱╲╱╲╱ │ ╲ ← 100 Hz envelope ripple (PSU hum)
  ││  │╱  ╲╱  ╲  │
  └──────────────
  
  Diagnosis: Check transmitter power supply for AC hum
```

### 2.4 Measuring Signal Frequency at Feedpoint

**Purpose:** Verify transmitter frequency and detect frequency drift.

**Procedure:**
1. Set oscilloscope to **frequency measurement mode** (most FNIRSI models have auto frequency display)
2. Transmit steady CW signal
3. Oscilloscope displays:
   - **Frequency (Hz):** Direct readout
   - **Period (s):** 1/frequency

**Example data (40M antenna, design 7.150 MHz):**
```
Oscilloscope frequency readout: 7.1507 MHz
Design target: 7.1500 MHz
Difference: +0.7 kHz (+0.01%) ← Excellent agreement with nanoVNA

Note: Oscilloscope's ±0.1% frequency accuracy is sufficient for antenna work
```

### 2.5 Monitoring AM/FM Modulation Envelope

**Purpose:** Verify RF envelope matches transmitted audio (if using phone modulation).

**Setup (SSB/AM testing):**
1. Connect RF probe to feedpoint
2. Set oscilloscope:
   - Time base: **10 µs/div** (shows ~5 audio cycles at ~1 kHz audio)
   - Vertical: Adjust for best display of envelope
   - Input coupling: **AC**

**Expected waveform (SSB at 1 kHz audio tone):**
```
  ▲
  │      ╱╲    ╱╲    ╱╲        ← RF carrier (~ MHz)
  │    ╱╱  ╲╱╱╲╱╱  ╲╱╱╲      
  │  ╱╱      ╲╱      ╲╱╲      ← Audio envelope (1 kHz)
  │ ╱         │        │ ╲
  └────────────────────────
    One audio cycle ≈ 1 ms
```

**Key observations:**
- RF carrier frequency should be constant (no drift)
- Envelope should follow audio waveform smoothly (no distortion/clipping)
- Envelope should reach 0V only briefly (good modulation index)

### 2.6 Measuring Antenna Loading/Impedance Indirectly

While oscilloscope cannot measure impedance directly, it can reveal **impedance problems indirectly**:

**Method: Transmitted signal changes with SWR**

1. Measure signal voltage at feedpoint (using RF probe + scope)
2. **Disconnect antenna** and short feedpoint to ground
3. Remeasure signal voltage
4. **Calculate reflected voltage ratio:**
   ```
   If antenna loaded: V_antenna = V_short × (R_load / 50)
   ```

**Example:**
```
Short circuit (antenna disconnected):
  Voltage at feedpoint: 10 Vpp (from transmitter driving 50Ω termination)

Antenna connected:
  Voltage at feedpoint: 8 Vpp (antenna draws current, voltage divides)
  
Impedance estimate:
  Antenna draws: 10V − 8V = 2V across 50Ω = 40 mA current
  Antenna impedance: V / I = 8V / 40mA ≈ 200Ω
  
  (Rough estimate; nanoVNA is much more accurate)
```

**Better use:** Monitor voltage stability while tuning antenna
- If voltage fluctuates wildly during tuning: Impedance is changing rapidly (trap resonance nearby)
- If voltage changes smoothly: Antenna loading is gradual

---

## SECTION III — COMPLETE FIELD TEST SEQUENCE WITH ALL INSTRUMENTS

### 3.1 Integrated Field Test Workflow

**Time allocation: 60 minutes for complete antenna checkout**

#### Phase 1: Component Verification (10 min)

Use **Fluke 15B+ multimeter** to pre-screen antenna:

```
1. Measure arm continuity (should be < 1Ω)
   □ Left arm: ______ Ω
   □ Right arm: ______ Ω (should match ±0.1Ω)

2. Measure coil DC resistance
   □ Left coil: ______ Ω
   □ Right coil: ______ Ω (compare to theoretical ±10%)

3. Diode test trap capacitor
   □ Capacitor: ______ (should show spike, then ∞)

4. Feedline shield continuity
   □ Shield resistance: ______ Ω (should be < 0.5Ω)

VERDICT: Component integrity ☐ PASS ☐ FAIL
(If any FAIL, stop and repair before RF testing)
```

#### Phase 2: nanoVNA RF Matching (15 min)

Use **nanoVNA** to characterize antenna loading:

```
1. Calibrate nanoVNA (SOL method)
2. Scan full band, record:
   □ Resonance frequency: ______ MHz
   □ SWR at resonance: ______ :1
   □ Impedance R/jX: ______ / ______ Ω
   □ Trap frequency: ______ MHz (if applicable)

VERDICT: RF matching ☐ PASS (SWR < 1.5) ☐ MARGINAL ☐ FAIL
```

#### Phase 3: Transmitted Signal Quality (15 min)

Use **FNIRSI oscilloscope** to verify transmitter output:

```
1. Connect RF probe to feedpoint (use 10:1 probe)
2. Transmit low-power CW (3W)
3. Set scope: AC coupling, auto-trigger, 1 µs/div, 1 V/div

Measurements:
   □ Peak-to-peak voltage: ______ Vpp
   □ Frequency (scope readout): ______ MHz
   □ Rise time (if measurable): ______ ns
   □ Distortion/clipping: ☐ Clean ☐ Slight ☐ Severe

VERDICT: Signal quality ☐ PASS ☐ FAIL (check for clipping, ringing)
```

#### Phase 4: Spectrum Analysis with tinySA (15 min)

Use **tinySA** to verify radiated spectrum:

```
1. Set tinySA to band ±20%
2. Transmit steady CW at 3W
3. Record:
   □ Peak frequency: ______ MHz
   □ Peak power: ______ dBm
   □ −3 dB bandwidth: ______ kHz
   □ Calculated Q: ______
   □ Harmonics: < −40 dB ☐ YES ☐ NO

VERDICT: Spectrum ☐ PASS ☐ FAIL
```

#### Phase 5: Summary & Recommendation (5 min)

```
OVERALL TEST RESULT:
  ☐ PASS — All measurements acceptable
  ☐ MARGINAL — Minor issues, acceptable for operation
  ☐ FAIL — Significant problems, rework required

Corrective actions: _________________________________
Next steps: ________________________________________

Operator: _________________________ Date: __________
```

---

## SECTION IV — TROUBLESHOOTING WITH MULTIMETER AND SCOPE

### 4.1 Feed System Problems Diagnosed

**Problem: High SWR despite good component continuity**

```
Diagnostic sequence:

1. Multimeter: Check DC coil resistance
   → Matches design calc? → Coil OK
   → Deviates > 20%? → Check for corrosion on coil wire
   
2. nanoVNA: Check TDR on feedline
   → Impedance profile flat? → Cable OK
   → Discontinuity present? → Cable or connector fault

3. Oscilloscope: Monitor transmitted signal voltage
   → Voltage stable (changes < 10% with frequency tune)? → Transmitter OK
   → Voltage fluctuates wildly? → Impedance mismatch causing reflections

Resolution: Most likely cause is antenna physical length error or trap misalignment
```

**Problem: Signal shows ringing or overshoot on oscilloscope**

```
Oscilloscope waveform shows damped oscillation:
  ▲
  │    ╱╲╱╲╱╲
  │  ╱  ╲  ╲  
  │ ╱    ╲  ╲

Root causes (in order of likelihood):
1. Feedline impedance mismatch (high SWR)
   → Verify with nanoVNA TDR
   → Check connector quality with multimeter (resistance)

2. Cable too long or damaged
   → Measure DC shield resistance (should be < 0.5Ω for 30 ft)
   → TDR scan for discontinuity

3. Antenna impedance far from 50Ω
   → Use nanoVNA to characterize
   → Adjust antenna length or trap tuning

Resolution: Use nanoVNA Smith chart to identify source of reflection
```

**Problem: Oscilloscope shows high-frequency noise/jitter**

```
Oscilloscope trace shows noisy waveform:
  ▲
  │   ▂▂▂▂▂▂▂
  │ ▗▘▝ ▚▚▚ ▘▝  ← "Hairy" waveform
  │▝▚▚▚▚▚▚▚▝

Likely causes:
1. Poor RF probe grounding
   → Use short ground strap (< 2 inches) from probe ground to antenna ground
   → Move probe away from near-field radiation source

2. Nearby RF source (transmitter close to antenna)
   → Normal at 2–3 watts; acceptable if not severe
   → Increase distance or shield probe cable

3. Transmitter RF output noisy
   → Check transmitter specifications
   → May need shielding or filtering on output

Resolution: Move test equipment away from antenna, use shielded probe cable
```

---

## SECTION V — CARRYING & PROTECTING INSTRUMENTS IN FIELD

### 5.1 Field Kit Assembly

**Recommended pack order** (by function):

**Tier 1: Multimeter & Oscilloscope**
```
□ Fluke 15B+ (in protective case)
□ FNIRSI scope (with battery or USB power)
□ Scope carrying case or foam-padded bag
□ USB cables (scope data download, charging)
□ Probe/test leads for multimeter
```

**Tier 2: nanoVNA & tinySA**
```
□ nanoVNA + calibration kit (short, open, 50Ω load)
□ tinySA (or USB spectrum analyzer)
□ Test coax cables (marked with length)
□ SMA/BNC/N adapters
□ Calibrated torque wrench (1.2 N·m for SMA)
```

**Tier 3: RF Test Components**
```
□ RF probe (10:1 passive probe for oscilloscope)
□ Coupling loop (magnetic probe for non-contact monitoring)
□ 50Ω terminator (load for baseline tests)
□ Attenuators (if testing high power > 5W)
```

**Tier 4: Field Documentation**
```
□ Laminated quick reference cards
□ Field log notebook
□ Pencil and eraser (works in field better than pen)
□ Camera or smartphone for photos
```

**Total weight:** ~3 lbs with batteries and cables

### 5.2 Weather Protection

**Multimeter:**
- Keep in protective case when not in use
- Moisture/humidity: Store with desiccant packet
- Cold weather: Battery performance drops; warm battery in pocket before use
- Hot weather: Allow 10 min cool-down after transport

**Oscilloscope:**
- Use foam-lined protective case
- Avoid direct sunlight (LCD performance degrades > 60°C)
- Battery mode: Limit use to 2–3 hours per charge (bring USB power bank)
- Moisture: Keep USB connector covered when not in use

**RF Probe:**
- Store connector cap on when not in use
- Keep probe cable coiled loosely (tight coils increase capacitance, affecting high-freq response)
- Avoid kinks in probe cable

---

## SECTION VI — MEASUREMENT CORRELATION & CROSS-CHECKING

### 6.1 When Instruments Disagree

**Example: nanoVNA shows good SWR, but oscilloscope shows ringing**

```
Scenario:
  • nanoVNA: SWR = 1.3:1, impedance = 51Ω ✓
  • Oscilloscope: Waveform shows significant ringing ✗

Possible explanation:
  • SWR is correct (impedance IS 50Ω)
  • But CABLE has an impedance discontinuity causing reflections
  • nanoVNA measures from cable input; sees good match at its input
  • Oscilloscope at feedpoint sees reflections inside the cable

Resolution:
  → Use nanoVNA TDR mode to scan cable for discontinuities
  → Measure cable DC resistance (should be < 0.5Ω)
  → Check for water intrusion or kinked shield

Root cause identified: Often a connector issue mid-cable or at feedpoint
```

**Example: Multimeter shows correct coil resistance, but nanoVNA shows wrong resonance**

```
Scenario:
  • Multimeter coil DC R: 45 mΩ ✓ (matches design calc)
  • nanoVNA resonance: 7.250 MHz vs. design 7.150 MHz (−1.4% error) ✗

Possible explanation:
  • Coil is wound correctly (DC R is right)
  • But PHYSICAL LENGTH is wrong (too many turns, coil diameter different)
  • Or antenna ARM LENGTH is too long

Resolution:
  → Measure coil deployed length: should be 2.0" on 40M
  → Measure antenna arm length: should be 5.0m on 40M
  → If arm is 5.05m (+1%), that alone causes +1.4% frequency shift

Root cause identified: Antenna assembly error, not component fault
```

### 6.2 Agreement Checklist

**When all instruments agree, antenna is properly characterized:**

```
✓ Multimeter shows:
  - Arm continuity < 1Ω per arm
  - Coil DC R matches design ±10%
  - Trap capacitor tests good
  - Feedline shield intact

✓ nanoVNA shows:
  - Resonance within ±1% of design frequency
  - SWR < 1.5:1 at design frequency
  - Impedance R close to 50Ω
  - Trap resonance at expected frequency

✓ FNIRSI oscilloscope shows:
  - Waveform clean, no visible distortion
  - Frequency matches nanoVNA resonance
  - Signal voltage stable during tuning

✓ tinySA shows:
  - Spectrum peak matches nanoVNA resonance
  - Bandwidth matches design estimate
  - Harmonic suppression > 40 dB
  - Radiation pattern consistent with antenna type

RESULT: Antenna is READY FOR DEPLOYMENT ✓
```

---

## SECTION VII — QUICK REFERENCE — WHEN TO USE WHICH INSTRUMENT

| Situation | Best Instrument | Why |
|-----------|---|---|
| **"Is the antenna wire broken?"** | Fluke 15B+ | Quick continuity beep |
| **"What's the antenna's impedance?"** | nanoVNA | Only accurate RF impedance tool |
| **"Is the antenna resonant?"** | nanoVNA | Dip method, zero-reactance crossing |
| **"What's the SWR?"** | nanoVNA | Direct S11 measurement |
| **"Is the transmitter clean?"** | FNIRSI scope | Visual waveform inspection |
| **"What's the radiated spectrum?"** | tinySA | Spectrum analyzer job |
| **"Is the feedline good?"** | nanoVNA TDR + Fluke 15B+ | TDR + DC resistance check |
| **"Did I tune the trap correctly?"** | nanoVNA | Watch secondary resonance peak move |
| **"Is the antenna directional?"** | tinySA | Azimuth rotation test |
| **"Do the components measure right?"** | Fluke 15B+ | DC resistance check against design |
| **"Is the signal getting to the antenna?"** | FNIRSI + Fluke | Voltage check at feedpoint |
| **"Are there harmonics being radiated?"** | tinySA | Spectrum scan to 3× frequency |

---

## Summary: Complementary Instrument Roles

**Fluke 15B+ Multimeter:**
- **Pre-test component verification** (save time before connecting to RF instruments)
- **DC resistance checks** (coils, connections, feedline shield)
- **Continuity testing** (antenna wire integrity)
- **Safety diagnostics** (voltage checks, diode tests)

**FNIRSI 100 MHz Oscilloscope:**
- **Transmitter quality verification** (waveform shape, distortion)
- **Frequency measurement** (independent of nanoVNA)
- **Modulation envelope inspection** (for phone testing)
- **Signal integrity diagnosis** (ringing, noise, rise time)

**nanoVNA:**
- **Antenna impedance & matching** (core antenna testing)
- **SWR measurement** (standing wave ratio)
- **Resonance identification** (zero-reactance dip)
- **Cable fault detection** (TDR mode)
- **Trap tuning feedback** (secondary resonance tracking)

**tinySA:**
- **Spectrum analysis** (frequency response, bandwidth)
- **Radiation pattern** (azimuth rotation test)
- **Harmonic content** (spurious suppression)
- **Q factor measurement** (−3 dB bandwidth)

**Together, these four instruments provide a complete antenna characterization suite for field testing.**

---

**End of Multimeter & Oscilloscope Guide**

*Integrate this guidance with FIELD_TESTING_WITH_INSTRUMENTS.md for comprehensive field testing procedures.*

