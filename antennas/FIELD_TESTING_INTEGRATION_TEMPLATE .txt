# Integration Template: Adding Field Testing Procedures to Antenna Manuals
## Using nanoVNA and tinySA with Your Antenna Suite

---

## Overview

This template shows how to add a new **SECTION VI.B — FIELD TESTING WITH INSTRUMENTS** to any antenna in your suite, referencing the master guide: `FIELD_TESTING_WITH_INSTRUMENTS.md`

Each antenna's technical chapter can include customized field testing procedures tailored to its specific design characteristics (frequency band, directivity, impedance, trap loading, etc.).

---

## Template: SECTION VI.B — FIELD TESTING WITH INSTRUMENTS

### B.1 Quick Reference — This Antenna

**Recommended instrument settings for [ANTENNA NAME]:**

| Instrument | Parameter | Setting | Rationale |
|-----------|-----------|---------|-----------|
| nanoVNA | Frequency span | [Band center ± 20%] | Captures full band plus sidelobe region |
| nanoVNA | Point density | 101 points | Standard resolution for adequate curve detail |
| nanoVNA | Stimulus power | −10 dBm | Safe level; sufficient for good S/N ratio |
| tinySA | Frequency span | [Band center ± 30%] | Broader view to detect harmonic content |
| tinySA | Reference level | [−30 to −50 dBm] | Adjust based on local RF environment |
| tinySA | Detection | Peak hold | Shows maximum signal across all sweeps |

**Expected performance (baseline):**
- Resonance frequency: [Design target] MHz ± 1%
- SWR @ resonance: [Expected value, e.g., 1.3:1]
- Feedpoint impedance (R): [Expected range, e.g., 45–55Ω]
- Feedpoint impedance (jX): [Expected range, e.g., ±10Ω]
- Bandwidth (−3 dB): [Expected range, e.g., 150–200 kHz on 40M]
- Expected Q: [Calculated Q value, e.g., 20–25]

---

### B.2 nanoVNA Measurement Procedure (Customized)

**Setup for [ANTENNA NAME]:**

1. **Pre-calibration check** (2 min):
   - Ensure feedline is [type/length, e.g., "RG-8, 30 ft, velocity factor 0.66"]
   - Verify antenna is erected at [recommended height, e.g., "10 m AGL horizontal orientation"]
   - Connect nanoVNA Port 1 to feedpoint via [connector type, e.g., "PL-259/SO-239 via adapter"]

2. **Calibration** (3 min):
   - Perform standard SOL calibration per Section 2.1
   - Specific check for this antenna: [if applicable, e.g., "If trap-loaded, ensure traps are accessible before measurement"]

3. **Initial sweep** (5 min):
   - Set frequency start/stop: [Band low MHz] to [Band high MHz]
   - Expected plot features:
     - **Primary resonance dip** at [design frequency] MHz
     - **Secondary peak** (if trap-loaded) at [trap frequency] MHz
     - **Impedance circle** should pass through or near center (50Ω) of Smith chart

4. **Detailed measurements**:
   - **Resonance frequency** (zero-reactance point):
     - Mark on nanoVNA screen using frequency cursor
     - Expected: [Design frequency] ± [tolerance]
     - Record to 0.01 MHz precision
   
   - **Impedance at resonance**:
     - Switch to impedance (R + jX) display format
     - At the zero-reactance frequency, read R value
     - Expected: [Design impedance, e.g., "50Ω ± 5Ω"]
     - If R deviates > ±10Ω, check antenna length (see Section IV.2, adaptive testing)
   
   - **SWR at band center, edges**:
     - Record SWR at three points: [low band MHz], [center MHz], [high band MHz]
     - Expected SWR < [threshold, e.g., "1.5:1"] across band
     - If SWR varies > 0.3 across band: Antenna may be under-loaded; consult Section IV.2

5. **Trap tuning** [if applicable]:
   - Locate secondary resonance peak in Smith chart
   - Current peak at: __________ MHz (record during test)
   - Target peak at: [Design trap frequency] MHz
   - Adjust trap trimmer capacitor per Section 2.5:
     - If peak is above target: Turn capacitor clockwise to lower frequency
     - If peak is below target: Turn capacitor counter-clockwise to raise frequency
     - Make 0.5-turn increments and re-scan after each adjustment
   - Completion criterion: Secondary peak merges with primary resonance, SWR < [target] at design frequency

6. **Cable verification** [optional]:
   - If feedline > 50 ft or antenna showing unexpected impedance:
   - Perform TDR scan per Section 2.6
   - Look for discontinuity at feedline terminus; should show step transition to antenna load

---

### B.3 tinySA Spectrum Monitoring (Customized)

**Setup for [ANTENNA NAME]:**

1. **Frequency configuration** (2 min):
   - Set tinySA start frequency: [Band low] MHz
   - Set tinySA stop frequency: [Band high] MHz
   - Expected peak location: [Design frequency] MHz

2. **Sensitivity setup**:
   - Set reference level based on local RF environment:
     - **Quiet location** (rural): −60 dBm reference
     - **Typical location** (suburban): −50 dBm reference
     - **High RF area** (near town): −30 dBm reference
   - Enable **Peak hold** to persistently display maximum signal across sweeps

3. **Baseline spectrum scan** (3 min):
   - Activate reference transmitter at [design frequency] MHz, [power] watts
   - Monitor tinySA display for spectrum peak
   - Expected peak location: [Design frequency] MHz
   - Expected peak height: [Estimated power level based on tx power and distance, e.g., "−30 dBm at 100 ft"]

4. **Bandwidth measurement**:
   - Record frequency at peak
   - Measure **−3 dB bandwidth** (where power drops to half of peak):
     - Record **f_low** (frequency at −3 dB below peak on low side)
     - Record **f_high** (frequency at −3 dB below peak on high side)
     - Bandwidth = f_high − f_low
   - Expected bandwidth: [Design Q × band center / 1000], e.g., "200 kHz on 40M for Q ≈ 20"
   - If measured BW significantly broader than expected: High loss environment or antenna Q is lower than design

5. **Radiation pattern indication** (azimuth rotation):
   - Deploy reference transmitter at fixed distance ([distance] ft, line of sight)
   - While monitoring tinySA, slowly rotate antenna in horizontal plane
   - Record signal level (dBm) at four cardinal points: 0°, 90°, 180°, 270°
   - Expected pattern for [antenna type]:
     - **[If omnidirectional]**: Level variation < ±3 dB around full rotation
     - **[If directional]**: Maximum at 0° (toward transmitter), minimum at 180°
     - Expected front-to-back ratio: [Estimated, e.g., "10–15 dB for 3-element Yagi"]
   - If measured pattern differs by > 5 dB from expectation: Check for nearby metal structures or antenna asymmetry

6. **Harmonic content check** [if transmitting on higher frequencies]:
   - Scan spectrum to 2× and 3× design frequency
   - Record harmonic peaks
   - Expected harmonic suppression: [e.g., "> 40 dB down from fundamental"]
   - If harmonics are strong (> −30 dB from fundamental): Antenna may have unintended resonance on harmonic band

---

### B.4 Interpretation of Results

**All measurements pass if:**
- ✓ Resonance within [±1%] of design frequency
- ✓ SWR < [threshold] at design frequency
- ✓ Impedance R within [acceptable range, e.g., "45–55Ω"]
- ✓ Measured Q within [acceptable range, e.g., "±20% of design"]
- ✓ Radiation pattern matches antenna type (omnidirectional or expected directivity)

**Minor tuning required if:**
- ⚠ Resonance within [±2%] of design frequency
- ⚠ SWR [threshold to 0.5 above threshold, e.g., "1.5–2.0"]
- ⚠ Impedance R within [wider tolerance, e.g., "40–60Ω"]
- Corrective action: Adjust trap capacitor [if applicable] or antenna length by [trim/extend amount, e.g., "2–3 cm per arm"]

**Rework required if:**
- ✗ Resonance > [±3%] from design frequency
- ✗ SWR > [upper limit, e.g., "2.5"] at design frequency
- ✗ Impedance R > [maximum acceptable, e.g., "30Ω or 70Ω"]
- ✗ Radiation pattern significantly differs from expected (e.g., unbalanced directivity)
- Corrective action: See Section IV.2 (adaptive testing, diagnostic flowchart)

---

### B.5 Field Test Log Customization

Use the standard log template (FIELD_TESTING_WITH_INSTRUMENTS.md, Section 5.1), with these specific entries for [ANTENNA NAME]:

```
FIELD TEST LOG — [ANTENNA NAME]
[Standard log header per Section 5.1]

ANTENNA CONFIGURATION — [ANTENNA NAME]:
  Type: [e.g., "Trap-loaded dipole, center-fed, spiral-wound"]
  Designed frequency: [e.g., "7.150 MHz (40M)"]
  Design compaction: [e.g., "50%"]
  Designed arm length: [e.g., "5.0 m per arm"]
  Designed impedance: [e.g., "50Ω ±5Ω at resonance"]
  Designed SWR: [e.g., "< 1.5:1 @ 7.150 MHz"]
  
  [Measured configuration]:
  Physical arm length: ______ m (target ______ m, delta ______%)
  Loading coil data:
    - Coil diameter: ______ inches
    - Coil length (deployed): ______ inches
    - Wire gauge: ______ AWG
    - Turns counted: ______ (target ______ from Table ___)
  Trap capacitor:
    - Type: ______ (fixed/variable trimmer)
    - Capacitance: ______ pF (measured or marked)
    - Trimmer position: ______ turns CW from open

MEASURED RESULTS — [ANTENNA NAME]:
  nanoVNA (@ feed point):
    - Resonance frequency: ______ MHz (target ______ MHz, delta ______%)
    - SWR @ 7.150 MHz: ______ (target < 1.5)
    - Impedance @ resonance: ______ + j______ Ω
    - Impedance @ band center: ______ + j______ Ω
    - Measured Q (from −3 dB BW): ______ (target ______ ±10%)
  
  tinySA (spectrum):
    - Peak frequency: ______ MHz
    - Peak power: ______ dBm
    - −3 dB BW measured: ______ kHz (target ______ kHz ±10%)
    - Calculated Q: ______ (check against nanoVNA Q)
    - Harmonic content: fundamental −XX dB (2× ______), −YY dB (3× ______)
  
  Radiation pattern (azimuth rotation):
    - Antenna type expectation: [omnidirectional / directional / other]
    - 0° (boresight):   ______ dBm
    - 90°:              ______ dBm
    - 180° (back):      ______ dBm
    - 270°:             ______ dBm
    - Max/min ratio: ______ dB (expected ______dB for this type)

COMPARISON TO DESIGN:
[Use table format from Section 5.3]

ASSESSMENT AND CORRECTIONS:
  ☐ PASS — All parameters within tolerance, no action required
  ☐ MARGINAL — [Specify which parameters] — requires minor tuning:
    Corrective action: [e.g., "Adjust trap capacitor 0.5 turn CW and re-test"]
  ☐ FAIL — [Specify failures] — requires significant rework:
    Corrective action: [e.g., "Trim 2 cm per arm, retune traps, re-calibrate"]

Repeat after correction:
  [Re-test results, date, operator signature]
```

---

## Usage Instructions

### For Single Antenna Update

1. **Locate antenna's technical chapter**: `/home/merv/Public/claude/ant_t_r/[antenna-name]/[antenna-name].md`

2. **Find existing Section VI (TEST AND ALIGNMENT)**, typically around 70–80% through the file

3. **Insert new subsection** after Section VI (before Section VII or appendices):

```markdown
### VI.B — Field Testing with nanoVNA and tinySA

[Insert customized template content above, replacing [ANTENNA NAME], [Band center], etc.]

For detailed procedures and general troubleshooting, see master guide:
`../FIELD_TESTING_WITH_INSTRUMENTS.md`
```

4. **Update antenna table** at top of manual with expected field test values

5. **Save and test** with one field session to verify customization is accurate

### For Batch Update (All 66 Antennas)

Create a Python script to programmatically insert the template:

```python
#!/usr/bin/env python3
import os
from pathlib import Path

antenna_dir = Path('/home/merv/Public/claude/ant_t_r')
template_file = antenna_dir / 'FIELD_TESTING_INTEGRATION_TEMPLATE.md'

# Read template
with open(template_file, 'r') as f:
    template = f.read()

# Process each antenna folder
for antenna_folder in antenna_dir.iterdir():
    if not antenna_folder.is_dir() or antenna_folder.name.startswith('.'):
        continue
    
    antenna_name = antenna_folder.name.replace('_', ' ').title()
    md_file = antenna_folder / f'{antenna_folder.name}.md'
    
    if not md_file.exists():
        print(f"Skipping {antenna_name}: no .md file")
        continue
    
    # Read existing manual
    with open(md_file, 'r') as f:
        content = f.read()
    
    # Check if Section VI.B already exists
    if '### VI.B' in content or 'Field Testing with nanoVNA' in content:
        print(f"Skipping {antenna_name}: field testing section already present")
        continue
    
    # Insert Section VI.B after Section VI
    # Find Section VII or Appendix (insertion point)
    insertion_point = content.rfind('## SECTION VII') or content.rfind('## APPENDIX')
    
    if insertion_point == -1:
        # No Section VII found; insert before end
        insertion_point = len(content) - 100
    
    # Customize template for this antenna
    # (extract band/frequency from antenna specifications or filename)
    customized = template.replace('[ANTENNA NAME]', antenna_name)
    # [Additional customizations based on antenna type/band]
    
    # Insert
    new_content = content[:insertion_point] + '\n\n' + customized + '\n\n' + content[insertion_point:]
    
    # Write back
    with open(md_file, 'w') as f:
        f.write(new_content)
    
    print(f"✓ Updated {antenna_name}")

print("\nBatch update complete!")
```

---

## Customization Examples

### Example 1: Compact Dipole (HF, Load-tuned)

```markdown
### B.1 Quick Reference — Compact Spiral-Wound Dipole (40M)

[Customized settings]:
- nanoVNA span: 6.8–7.5 MHz (covers entire 40M band plus adjacent)
- Expected resonance: 7.150 MHz (±0.050 MHz tolerance)
- Expected SWR @ 7.150: < 1.5:1
- Expected impedance: 50Ω ± 5Ω
- Expected Q: 18–22 (BW ≈ 170–200 kHz at −3 dB)

[Expected nanoVNA plot features]:
- Primary dip (loaded dipole): 7.150 MHz
- Secondary peak (trap resonance): ~7.8 MHz (initially, before tuning)
- After trap adjustment: secondary peak at ~7.15 MHz
- Impedance circle should enter center of Smith chart at resonance

[Expected tinySA response]:
- Peak at 7.150 MHz when transmitting on design frequency
- Bandwidth narrower than full-size dipole (~150–200 kHz vs. 50–100 kHz)
- Azimuth pattern: Omnidirectional (± 3 dB variation, no directivity)
```

### Example 2: Yagi Array (VHF, Directional)

```markdown
### B.1 Quick Reference — 3-Element Yagi (2M)

[Customized settings]:
- nanoVNA span: 144–148 MHz (full 2M band)
- Expected resonance: 146.0 MHz (±0.5 MHz per boom length tolerance)
- Expected SWR @ 146.0: < 1.3:1
- Expected impedance: 50Ω ± 10Ω (reflector spacing affects impedance)
- Expected Q: 40–60 (high Q due to narrow-band design)

[Expected nanoVNA plot features]:
- Sharp dip at design frequency (high Q)
- Impedance circle shows significant rotation (indicates directivity)
- SWR increases rapidly outside ±1 MHz from center

[Expected tinySA response]:
- Peak at 146.0 MHz when transmitting
- Narrow bandwidth (~40–60 kHz at −3 dB)
- Azimuth pattern: Directional with pronounced max at 0° (toward boom axis)
  - 0°: maximum (reference)
  - 90°: −3 dB (side lobe)
  - 180°: −20 dB (back lobe, front-to-back ≈ 20 dB)
  - Verify directivity by rotating and comparing levels
```

### Example 3: Magnetic Loop (UHF, Compact)

```markdown
### B.1 Quick Reference — KMR-400 Magnetic Loop (70cm)

[Customized settings]:
- nanoVNA span: 430–440 MHz (includes 70cm band)
- Expected resonance: 435.0 MHz (±1.0 MHz depending on capacitor tolerance)
- Expected SWR @ 435.0: < 2.0:1 (magnetic loops typically higher SWR)
- Expected impedance: 25–75Ω (depends on loop-to-feedline coupling)
- Expected Q: 100–150 (very high Q, very narrow bandwidth)

[Expected nanoVNA plot features]:
- Extremely sharp dip (narrow bandwidth ~ 5–15 kHz)
- Impedance circle may show large circle due to low impedance
- SWR rises rapidly outside ±0.5 MHz from resonance

[Critical tuning]:
- Trap capacitor must be adjusted for minimum SWR at exactly 435.0 MHz
- Even ±0.1 MHz error gives SWR > 2.0 (tight tolerance)
- After erection, perform nanoVNA sweep immediately and adjust capacitor

[Expected tinySA response]:
- Peak at exactly 435.0 MHz
- Extremely narrow spectrum peak (width < 20 kHz)
- Azimuth pattern: Omnidirectional in horizontal plane (loop plane horizontal)
  - Pattern is figure-8 in vertical plane (if loop vertical, radiation is broadside to loop)
```

---

## Summary

- **Master guide**: Read `FIELD_TESTING_WITH_INSTRUMENTS.md` for complete procedures
- **Per-antenna customization**: Use this template to add band-specific, antenna-type-specific measurement guidance
- **Field log**: Track all measurements with standardized log template
- **Data archival**: Export nanoVNA CSV, photograph antenna configuration, store with date stamps
- **Iteration**: Re-test after any tuning or repair to verify improvement

---

**End of Integration Template**

*Last updated: 2026-04-21*
