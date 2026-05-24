# Four-Instrument Field Test Workflow
## Fluke 15B+ | FNIRSI 100 MHz | nanoVNA | tinySA

---

## Why Four Instruments?

Each instrument answers different questions:

| Question | Instrument | Output |
|---|---|---|
| **Is the antenna wire intact?** | Fluke 15B+ | Continuity beep, Ω reading |
| **What's the antenna impedance?** | nanoVNA | Z = R ± jX (Ω) |
| **Is the antenna resonant?** | nanoVNA | f_res (MHz), dip depth |
| **What's the SWR?** | nanoVNA | SWR:1 ratio |
| **Is the transmitter output clean?** | FNIRSI | Waveform shape, Vpp |
| **What spectrum does antenna radiate?** | tinySA | Frequency response, harmonics |

---

## The Four-Phase Field Test

```
TIME    PHASE              INSTRUMENT    PURPOSE
────────────────────────────────────────────────────────────────
5 min   Component Check    Fluke 15B+    Continuity, resistance, safety
15 min  RF Impedance       nanoVNA       SWR, resonance, matching
15 min  Signal Quality     FNIRSI scope  Waveform, distortion, frequency
15 min  Spectrum Analysis  tinySA        Radiation, harmonics, pattern
────────────────────────────────────────────────────────────────
60 min  TOTAL (complete antenna checkout)
```

---

## PHASE 1: COMPONENT VERIFICATION (5 MIN)

### Setup
- **Instrument:** Fluke 15B+ multimeter
- **Cable:** Test leads
- **Mode:** Ω (resistance) and diode test

### Measurements

**Step 1: Antenna Wire Continuity**
```
Test: Measure DC resistance from feedpoint to tip of each arm
Expected: < 1Ω per arm
Bad sign: ∞ (infinite) = open wire, needs repair
Good sign: Both arms ± 0.1Ω (symmetric)
```

**Step 2: Coil DC Resistance**
```
Test: Measure resistance across loading coil terminals
Expected: ±10% of design calculation
Bad sign: 2–3× higher = corrosion on coil
Bad sign: Much lower (< 50% design) = short turns or different wire
Good sign: ±5% of design = correctly wound
```

**Step 3: Trap Capacitor**
```
Test: Diode mode (∆| symbol), brief resistance spike then ∞
Expected: Good capacitor shows spike then open
Bad sign: 0Ω = shorted capacitor, needs replacement
Bad sign: < 10 kΩ continuously = leaky, marginal
Good sign: Spike drops to ∞ = OK for operation
```

**Step 4: Feedline Integrity**
```
Test: Measure shield resistance over entire cable length
Expected: < 0.5Ω for 30 ft RG-8 or LMR-400
Bad sign: > 5Ω = broken shield, cable needs replacement
Good sign: < 0.2Ω = excellent continuity
```

### Decision Point
```
IF any component fails:
  → STOP RF testing
  → Repair physical component
  → Retest with multimeter before proceeding

IF all pass:
  → Proceed to Phase 2 (nanoVNA)
```

---

## PHASE 2: RF IMPEDANCE CHARACTERIZATION (15 MIN)

### Setup
- **Instrument:** nanoVNA
- **Connection:** Test cable from Port 1 to antenna feedpoint
- **Calibration:** SOL (short, open, 50Ω load)

### Measurements

**Step 1: Calibration (3 min)**
```
SHORT:  S11 ≈ 0° magnitude 1.0  ✓
OPEN:   S11 ≈ 180° magnitude 1.0 ✓
LOAD:   S11 ≈ 0° magnitude 0.0  ✓
```

**Step 2: Full-Band Sweep**
```
Set nanoVNA:
  Start frequency: [Band low − 20%]
  Stop frequency: [Band high + 20%]
  Points: 101 (default)
  Display: Smith chart (impedance)

Scan and observe:
  □ Primary resonance dip visible?
  □ Secondary peak (if trap-loaded)?
  □ Impedance circle passes through center?
```

**Step 3: Resonance Identification**
```
Method: Zero-reactance crossing (where jX = 0)
  
Marker tool → sweep across band center
Find point where imaginary impedance crosses zero

Record:
  f_resonance = ______ MHz
  R at resonance = ______ Ω
  Design target = 7.150 MHz (example)
  Error = ______ % (should be ±1%)
```

**Step 4: SWR Characterization**
```
Record SWR at three points:

  Low edge (7.0 MHz):    SWR = ______ :1
  Band center (7.15):    SWR = ______ :1
  High edge (7.3 MHz):   SWR = ______ :1

Target: SWR < 1.5:1 across band
Accept: SWR < 2.0:1 at edges
Reject: Any SWR > 2.5:1 requires rework
```

**Step 5: Optional TDR (Cable Check)**
```
IF feedline > 50 ft OR suspecting cable fault:
  
  Switch to TDR mode
  Set velocity factor: 0.66 (RG-8) or 0.85 (foam)
  Scan with antenna disconnected
  
  Expected: Flat impedance profile, step at cable end
  Bad sign: Impedance dip or rise = moisture or break in cable
```

### Decision Point
```
IF SWR > 2.5 at design freq AND pattern looks wrong:
  → Suspect antenna mechanical error (length, coil turns)
  → Return to Phase 1, verify components
  → Check physical dimensions vs. design

IF SWR good but resonance offset > ±2%:
  → Suspect coil inductance wrong (too many/few turns)
  → Or antenna arm too long/short
  → Small trim adjustment may fix

IF all passes:
  → Proceed to Phase 3 (oscilloscope)
```

---

## PHASE 3: TRANSMITTED SIGNAL QUALITY (15 MIN)

### Setup
- **Instrument:** FNIRSI 100 MHz oscilloscope
- **Probe:** 10:1 passive RF probe (non-contact or near feedpoint)
- **Transmitter:** 3–5 watts CW on antenna frequency

### Measurements

**Step 1: Oscilloscope Configuration**
```
Input coupling:   AC (blocks DC)
Trigger:         Auto (rising edge)
Time base:       1 µs/div (adjust as needed)
Vertical scale:  1 V/div (adjust for good display)
Probe attenuation: 10:1 (multiply display readings by 10)
```

**Step 2: RF Waveform Inspection**
```
Transmit steady CW at design frequency

Expected waveform: Clean sine wave
┌────────────────────────────────────────┐
│    ▁▂▃▄▅▆▇█▇▆▅▄▃▂▁                   │
│  ▄            ▄                        │
│ ▂              ▂                       │
│▁                ▁                      │
└────────────────────────────────────────┘

Good signs:
  ✓ Smooth sine wave, no distortion
  ✓ Peaks equal on both sides
  ✓ No ringing or damped oscillation
  ✓ Clean transitions

Bad signs:
  ✗ Clipped peaks (looks like flat-top)
  ✗ Ringing (damped oscillation on peaks)
  ✗ Noisy waveform ("hairy" appearance)
  ✗ Unsymmetrical peaks (left ≠ right)
```

**Step 3: Voltage Measurement**
```
Count divisions from peak+ to peak−
Vpp(measured) = # divisions × V/div × 10 (probe attenuation)

Example:
  Display: 2.5 divisions
  Scale: 1 V/div
  Probe: 10:1
  Vpp at feedpoint = 2.5 × 1 × 10 = 25 Vpp

Convert to power (if needed):
  P = Vpp² / (8 × Z)
  P = (25)² / (8 × 50) = 625 / 400 = 1.56 watts ✓
```

**Step 4: Frequency Verification**
```
FNIRSI displays frequency automatically (or count cycles)

Recorded frequency = ______ MHz
nanoVNA resonance = ______ MHz (from Phase 2)
Difference = ______ kHz

Should match within ±1% (±7.15 kHz for 40M)
If difference > ±2%: Antenna or transmitter frequency issue
```

**Step 5: Rise Time (Optional)**
```
For fast pulse measurements:
  Measure 10%–90% rise time

Fast rise (< 50 ns): Clean antenna loading, good match
Slow rise (> 100 ns): Poor impedance match, cable loss, or RLC loading

For CW, rise time is less critical
```

### Decision Point
```
IF waveform is clean AND frequency matches nanoVNA:
  → Transmitter and antenna loading are good
  → Proceed to Phase 4 (tinySA)

IF waveform shows clipping or distortion:
  → Transmitter may be overdriven OR antenna SWR very high
  → Check nanoVNA SWR result (Phase 2)
  → If SWR > 2.0: Use antenna tuner or reduce power

IF frequency doesn't match nanoVNA:
  → Likely transmitter frequency drift or incorrect frequency set
  → Verify radio frequency display
  → Allow transmitter to warm up (15 min) and recheck
```

---

## PHASE 4: SPECTRUM ANALYSIS (15 MIN)

### Setup
- **Instrument:** tinySA spectrum analyzer
- **Antenna:** Same antenna under test, still transmitting
- **Mode:** Spectrum scan with peak hold

### Measurements

**Step 1: Frequency Configuration**
```
tinySA Menu → Range:
  Start frequency: [Band low] MHz
  Stop frequency: [Band high + 20%] MHz
  Span: auto-calculated

Example (40M):
  Start: 7.0 MHz
  Stop: 7.3 MHz
  Ref level: −50 dBm (adjust based on local RF)
```

**Step 2: Spectrum Peak Scan**
```
Transmit steady CW 3W
Enable peak hold (persistent display)
Sweep across frequency range

Expected result:
  Sharp peak at design frequency (7.150 MHz)
  Peak width ~150–250 kHz at −3 dB
  Sides fall off steeply
  
┌──────────────────────────────────────┐
│               ▲                       │
│              ╱│╲                      │
│            ╱  │  ╲                    │
│          ╱    │    ╲                  │
│────────╱──────┼──────╲────────────── │
│       6.5    7.15    7.3 MHz  Freq  │
└──────────────────────────────────────┘
```

**Step 3: Bandwidth Measurement**
```
Use tinySA marker tool to find −3 dB points
(−3 dB = half power = 0.707 × peak voltage)

Mark low side (f_low) and high side (f_high) where power drops

Measured BW = f_high − f_low = ______ kHz

Calculate Q:
  Q = f_center / BW
  Q = 7.15 / ______ = ______

Compare to nanoVNA Q estimate (Phase 2):
  Match? Good cross-verification ✓
  Differ > 20%? Check for environmental differences
```

**Step 4: Harmonic Content**
```
Without moving antenna or transmitter:
  Extend spectrum display to 3× design frequency

Example (40M = 7.15 MHz):
  Scan to 21.45 MHz to look for 3× harmonic

Record harmonic levels:
  2× (14.3 MHz): ______ dBm (target < −40 dB below fundamental)
  3× (21.45 MHz): ______ dBm (target < −40 dB below fundamental)

Good antenna: Harmonics > 40 dB down
Poor result: Harmonics within 20 dB = significant re-radiation ✗
```

**Step 5: Radiation Pattern (Azimuth Test)**
```
Optional: Deploy reference transmitter at 100+ meters
With tinySA monitoring antenna feedpoint region:
  Rotate antenna 360° in azimuth
  Record signal level at 4 cardinal points

Expected for omnidirectional antenna:
  0°: −22 dBm
  90°: −22.5 dBm
  180°: −22.5 dBm
  270°: −22.5 dBm
  Variation: < ±2 dB = good omnidirectional

Expected for directional antenna:
  0°: −20 dBm (max, toward reference)
  90°: −26 dBm (side lobe)
  180°: −35 dBm (back lobe, front-to-back ≈ 15 dB)
  270°: −26 dBm (side lobe)
```

### Decision Point
```
IF spectrum peak matches nanoVNA resonance frequency:
  ✓ Frequency agreement confirmed (cross-check passed)

IF bandwidth matches design estimate:
  ✓ Q factor confirmed, antenna loading is as designed

IF harmonics > 40 dB down:
  ✓ Clean spectrum, no spurious re-radiation

IF pattern matches antenna type (omnidirectional/directional):
  ✓ Radiation characteristics confirmed

ALL CHECKS PASS → ANTENNA IS READY FOR DEPLOYMENT ✓
```

---

## SUMMARY TABLE: 60-Minute Complete Antenna Characterization

| Phase | Time | Instrument | What We're Testing | Success Criteria |
|---|---|---|---|---|
| 1. Component | 5 min | Fluke 15B+ | Physical integrity | Continuity OK, resistance matches design, no shorts |
| 2. RF Impedance | 15 min | nanoVNA | Antenna loading/tuning | Resonance ±1%, SWR < 1.5:1, impedance 40–60Ω |
| 3. Signal Quality | 15 min | FNIRSI | Transmitter output | Clean waveform, no distortion, frequency matches |
| 4. Spectrum | 15 min | tinySA | Radiated power/pattern | Peak at design freq, BW matches design, harmonics −40 dB |
| **TOTAL** | **60 min** | **4 instruments** | **Complete antenna** | **All phases PASS** |

---

## Data Collection Template

### Pre-Test
```
Antenna: ___________________    Test date: ________
Location: ___________________  QTH grid: ________
Temperature: ___°C   Humidity: ___%   Wind: ___kt
```

### Phase 1: Multimeter
```
Arm continuity (left/right):   ___/___  Ω
Coil resistance (left/right):  ___/___  Ω
Capacitor test:                _______ (spike/open)
Feedline shield resistance:    _______ Ω
```

### Phase 2: nanoVNA
```
Resonance frequency:           _______ MHz
SWR @ resonance:               _______ :1
SWR @ band center:             _______ :1
Impedance R:                   _______ Ω
Impedance jX:                  _______ Ω
Trap resonance (if applicable):_______ MHz
```

### Phase 3: FNIRSI
```
Frequency (scope display):     _______ MHz
Peak-to-peak voltage:          _______ Vpp
Waveform quality:              ☐ Clean  ☐ Slight distortion  ☐ Severe
```

### Phase 4: tinySA
```
Spectrum peak frequency:       _______ MHz
Peak power:                    _______ dBm
Bandwidth (−3 dB):             _______ kHz
Calculated Q:                  _______
Harmonic suppression:          _______ dB @ 2×
                               _______ dB @ 3×
```

### Final Assessment
```
Overall result: ☐ PASS  ☐ MARGINAL  ☐ FAIL

Specific issues (if any):
_____________________________________________
_____________________________________________

Corrective action taken:
_____________________________________________

Next test date:  ________________
Operator: ________________  Signature: ________________
```

---

## Equipment Packing Checklist

**Multimeter:**
- [ ] Fluke 15B+ in protective case
- [ ] Test leads (red/black probes)
- [ ] Battery (if not internal)
- [ ] Protective pouch

**Oscilloscope:**
- [ ] FNIRSI scope with screen protector
- [ ] USB charging cable
- [ ] Battery or power bank
- [ ] Foam-lined carrying case

**nanoVNA:**
- [ ] nanoVNA unit
- [ ] Calibration kit (short, open, 50Ω load)
- [ ] Test coax cables (marked with length)
- [ ] SMA adapters/extensions
- [ ] USB cable (for data export)
- [ ] Protective case

**tinySA:**
- [ ] tinySA unit
- [ ] USB power or batteries
- [ ] SMA to antenna connector adapter
- [ ] Protective case

**Accessories:**
- [ ] RF probe (10:1 passive) for oscilloscope
- [ ] Torque wrench (SMA 1.2 N·m)
- [ ] Test antenna or coupling loop
- [ ] 50Ω load resistor
- [ ] Field log notebook and pencil
- [ ] Laminated quick reference cards

**Total field kit weight:** ~4–5 lbs (easily portable)

---

## Advantages of Four-Instrument Approach

1. **Rapid fault isolation:** Each instrument targets specific aspect
   - Multimeter says "component is good"
   - nanoVNA says "antenna impedance is right"
   - Scope says "transmitter is clean"
   - tinySA says "spectrum is acceptable"

2. **Cross-verification:** Different methods confirm same result
   - Resonance frequency from nanoVNA AND tinySA should match
   - Q calculated from nanoVNA impedance AND tinySA bandwidth should agree
   - Voltage measured from scope AND power calculation should be consistent

3. **Comprehensive diagnostics:** No blind spots
   - Component faults caught early (multimeter)
   - RF matching verified (nanoVNA)
   - Signal integrity confirmed (scope)
   - Radiated pattern checked (tinySA)

4. **Field-ready toolkit:** All instruments battery-powered, portable
   - No need for AC power
   - Can test in remote locations
   - Total time per antenna: ~60 minutes
   - One person can operate all four instruments

---

## When to Use Simplified Testing

**If time is limited, prioritize in this order:**

```
MINIMUM (5 min):
  □ Multimeter continuity (antenna wire OK?)
  □ nanoVNA SWR scan (is antenna loaded?)

STANDARD (25 min):
  □ Multimeter (component check)
  □ nanoVNA (RF impedance, SWR, resonance)

COMPLETE (60 min):
  □ Multimeter (component check)
  □ nanoVNA (RF impedance, SWR, resonance)
  □ FNIRSI (signal quality, frequency)
  □ tinySA (spectrum, radiation pattern)

RECOMMENDED: Run complete test (60 min) before first deployment
Then use simplified test (5–25 min) for periodic verification
```

---

## Quick Decision Tree

```
START FIELD TEST

      │
      ├─→ [Multimeter] Wire continuity OK?
      │         │
      │         NO → REPAIR antenna/feedline, retest
      │         │
      │         YES
      │         │
      ├─→ [nanoVNA] SWR < 1.5 at design freq?
      │         │
      │         NO → Adjust antenna length or tune trap
      │         │    Retest with nanoVNA
      │         │
      │         YES
      │         │
      ├─→ [FNIRSI] Waveform clean? Frequency match?
      │         │
      │         NO → Check transmitter power level
      │         │    Verify frequency programming
      │         │
      │         YES
      │         │
      ├─→ [tinySA] Spectrum peak at design freq?
      │         │   Harmonics > 40 dB down?
      │         │
      │         NO → Verify antenna connection
      │         │    Check for loose connectors
      │         │
      │         YES
      │         │
      └─→ [READY] All tests PASS ✓
              Antenna is ready for deployment
```

---

**Field test with four instruments provides confidence that antenna will perform as designed.**

