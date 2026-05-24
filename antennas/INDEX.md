# Antenna Suite Documentation Index
## Complete Navigation Guide

---

## Quick Navigation by Use Case

### "I want to test an antenna right now"
1. **Print & bring:** `FIELD_TESTING_QUICK_REFERENCE.md` (pocket card)
2. **Follow:** Section VI from any antenna's `.md` file (or add custom section via template)
3. **Reference:** `FIELD_TESTING_WITH_INSTRUMENTS.md` for detailed procedures

### "I want to understand the complete process"
1. **Read:** `FIELD_TESTING_WITH_INSTRUMENTS.md` (comprehensive guide, all sections)
2. **Study:** `FIELD_TEST_EXAMPLE_40M_COMPACT_DIPOLE.md` (worked example with real data)
3. **Practice:** With any antenna from the suite using nanoVNA + tinySA

### "I want to add field testing to an antenna's manual"
1. **Review:** `FIELD_TESTING_INTEGRATION_TEMPLATE.md` (structure and examples)
2. **Customize:** Replace placeholders with antenna-specific parameters
3. **Optional:** Run Python script for batch updates to all 66 antennas

### "Something went wrong with my measurement"
1. **Consult:** `FIELD_TESTING_QUICK_REFERENCE.md` (problem diagnosis flowchart)
2. **Detailed help:** `FIELD_TESTING_WITH_INSTRUMENTS.md` (Appendix B — troubleshooting)
3. **Compare:** `FIELD_TEST_EXAMPLE_40M_COMPACT_DIPOLE.md` (see proper procedure)

---

## File Directory

### Master Documentation Files (in `/home/merv/Public/claude/ant_t_r/`)

```
FIELD_TESTING_WITH_INSTRUMENTS.md          (26 KB, 7 sections + 2 appendices)
FIELD_TESTING_INTEGRATION_TEMPLATE.md      (15 KB, template + examples + script)
FIELD_TESTING_QUICK_REFERENCE.md           (8 KB, quick lookup pocket card)
FIELD_TEST_EXAMPLE_40M_COMPACT_DIPOLE.md   (18 KB, complete worked example)
INDEX.md                                   (this file)
ANTENNA_SUITE_MANIFEST.md                  (comprehensive 66-antenna inventory)
```

### Antenna Folders (66 total, each containing)

```
<antenna-name>/
├── <antenna-name>.nec              (NEC2 electromagnetic model)
├── <antenna-name>.md               (Navy technical manual chapter)
├── <antenna-name>_specs.csv        (Technical specifications table)
└── figures/                        (3 PNG schematic diagrams)
    ├── <antenna-name>_antenna_layout.png
    ├── <antenna-name>_circuit_schematic.png
    └── <antenna-name>_construction_detail.png
```

### Special Case: Compact Dipole Suite (14 bands)

```
compact-dipole/
├── COMPACT_DIPOLE_MANUAL.md                (Master Navy technical manual)
├── compact_dipole_generator.py
├── trap_specifications.csv
├── winding_tables_all_bands.csv
├── figures/                                (Global diagrams)
│   ├── fig_01_dipole_overview.png
│   ├── fig_02_roll_up_card_assembly.png
│   └── ... (8 global diagrams total)
└── <band>/ (14 folders: 160m, 80m, 40m, 30m, 20m, 17m, 15m, 12m, 10m, 6m, 2m, 1p25m, 70cm, 33cm, 20cm)
    ├── compact_dipole_<band>.nec
    ├── compact_dipole_<band>.md
    ├── winding_table_<band>.csv
    └── figures/
        ├── <band>_antenna_layout.png
        ├── <band>_coil_detail.png
        └── <band>_trap_schematic.png
```

---

## Document Contents at a Glance

### FIELD_TESTING_WITH_INSTRUMENTS.md

**SECTION I — Instrument Overview**
- nanoVNA specs (50 kHz–900 MHz, impedance/SWR measurement)
- tinySA specs (100 kHz–350 MHz, spectrum analyzer)

**SECTION II — nanoVNA Impedance Measurements**
- Pre-test calibration (SOL method)
- Impedance measurement at feedpoint
- Reading SWR directly (formula: SWR = (1+Γ)/(1−Γ))
- Finding resonance (zero-reactance dip method)
- Trap tuning procedure (LC resonator adjustment)
- TDR analysis (cable fault detection)

**SECTION III — tinySA Spectrum Analysis**
- Passive spectrum monitoring (non-destructive)
- Quick-start spectrum scan
- Identifying transmitted signal
- Radiation pattern assessment (azimuth rotation test)
- Bandwidth and Q measurement
- Harmonic content verification

**SECTION IV — Field Test Workflow**
- Complete 45-minute antenna test sequence
- Adaptive testing (problem diagnosis)
- Environmental corrections (temperature, humidity, ground effects)

**SECTION V — Data Logging & Analysis**
- Field log template (minimum data required)
- nanoVNA CSV export and plotting
- Post-test comparison to design specs
- Acceptance criteria (PASS/MARGINAL/FAIL)

**SECTION VI — Equipment Care**
- nanoVNA maintenance and storage
- tinySA care procedures
- Test cable and connector management

**APPENDIX A — Quick Reference Checklist**
- Pre-test preparations
- Measurement checklist
- Post-test data handling

**APPENDIX B — Troubleshooting**
- Common measurement errors and fixes
- Problem-cause-solution format

---

### FIELD_TESTING_INTEGRATION_TEMPLATE.md

**Template Structure (for adding to antenna manuals)**
- Quick reference table (instrument settings for antenna type)
- Expected baseline performance
- nanoVNA procedure (customized per band/antenna type)
- tinySA measurement (antenna-type-specific)
- Result interpretation (pass/marginal/fail criteria)
- Field log customization

**Customization Examples**
- Example 1: Compact dipole (HF, load-tuned)
- Example 2: 3-element Yagi (VHF, directional)
- Example 3: Magnetic loop (UHF, high-Q)

**Batch Integration**
- Python script for programmatic updates
- Instructions for single vs. batch antenna updates

---

### FIELD_TESTING_QUICK_REFERENCE.md

(Designed for printing and lamination)

- **5-minute startup** procedures for both instruments
- **SWR conversion chart** (SWR ↔ return loss ↔ reflected power %)
- **Problem diagnosis tree** (flowchart for troubleshooting)
- **Trap tuning procedure** (step-by-step)
- **Impedance targets** by antenna type
- **Torque specifications** (SMA 1.2 N·m, N 1.7 N·m, etc.)
- **Equipment checklist** (what to carry to field)
- **Quick formulas** (SWR, return loss, Q)
- **Pass/fail criteria** quick check
- **Measurement error reference table**

---

### FIELD_TEST_EXAMPLE_40M_COMPACT_DIPOLE.md

**Real field test session (2026-04-21, Merced CA)**

- **Pre-test configuration** (antenna setup details)
- **nanoVNA measurements** (5-step procedure with actual data):
  - Calibration results (SHORT/OPEN/LOAD standards)
  - Antenna resonance identification (7.162 MHz, +0.17% error)
  - Impedance analysis (51.4Ω, excellent match)
  - SWR across band (1.28–2.08, target met)
  - Trap verification (7.80 MHz exact, no adjustment needed)
  - Q calculation (44.8 measured, 2.2× design estimate)

- **tinySA measurements** (5-step procedure):
  - Spectrum peak (−23 dBm @ 7.162 MHz)
  - Bandwidth measurement (165 kHz, Q ≈ 43)
  - Azimuth rotation test (omnidirectional, 0.1 dB variation)
  - Harmonic suppression (< −52 dB, excellent)

- **Cable diagnostics**:
  - TDR scan (cable good, no faults)
  - Feedpoint inspection (clean, no corrosion)

- **Comprehensive comparison table** (design vs. measured)
- **Assessment and recommendations** (ready for deployment)
- **Raw field log** (filled-out example)

---

## Quick Start Checklist

For your first field test with nanoVNA + tinySA:

**Before leaving home:**
- [ ] Read: FIELD_TESTING_WITH_INSTRUMENTS.md (Sections I–II)
- [ ] Print & laminate: FIELD_TESTING_QUICK_REFERENCE.md
- [ ] Study: FIELD_TEST_EXAMPLE_40M_COMPACT_DIPOLE.md
- [ ] Pack calibration kit (short, open, 50Ω load, standards)
- [ ] Pack test cables (marked with length)
- [ ] Pack power (batteries or USB battery pack)

**At the field site:**
1. Erect antenna at target height
2. Use quick reference checklist for startup
3. Perform SOL calibration (5 min)
4. Execute nanoVNA scan (follow Section II procedure)
5. Record resonance, SWR, impedance
6. Execute tinySA scan (follow Section III procedure)
7. Perform azimuth rotation test (record pattern)
8. Complete field log (minimum data template)
9. Compare to design specifications

**Back home:**
1. Export nanoVNA CSV data
2. Plot impedance/SWR curves in spreadsheet
3. Fill out comparison table (design vs. measured)
4. Archive in dated folder

---

## Reference: All 66 Antenna Types

See `ANTENNA_SUITE_MANIFEST.md` for complete list and description of:
- Compact dipoles (14 bands, 160M–20cm)
- Dipole variants (12 types)
- HF loaded/vertical (8 types)
- Loops & quads (8 types)
- Multiband/wire (6 types)
- Yagi & directional (8 types)
- VHF/UHF radiators (10 types)
- Phased/array (5 types)
- Receiving/specialized (6 types)

---

## Key Formulas

**SWR from reflection coefficient:**
```
SWR = (1 + |Γ|) / (1 − |Γ|)
```

**Return loss (dB):**
```
RL (dB) = −20 × log₁₀(|Γ|)
```

**Resonance frequency:**
```
f = 1 / (2π√LC)
```

**Quality factor (Q):**
```
Q = f_center / (f_high − f_low)    [where f_high/low are −3dB points]
```

**Dipole arm length (full-size):**
```
L_half = 71.3 / f_MHz  (meters)
```

---

## Document Statistics

| Document | Size | Sections | Purpose |
|----------|------|----------|---------|
| FIELD_TESTING_WITH_INSTRUMENTS.md | 26 KB | 7 + 2A | Master reference guide |
| FIELD_TESTING_INTEGRATION_TEMPLATE.md | 15 KB | Template + 3 ex | Per-antenna customization |
| FIELD_TESTING_QUICK_REFERENCE.md | 8 KB | 12 topics | Pocket card (laminate) |
| FIELD_TEST_EXAMPLE_40M_COMPACT_DIPOLE.md | 18 KB | Complete session | Worked example with data |
| **Total** | **67 KB** | **Comprehensive** | **Complete field testing suite** |

---

## Navigation Tips

1. **Bookmarks**: Create browser bookmarks for the 4 main guides
2. **Print & laminate**: Quick reference card goes in field kit
3. **Screenshots**: Take photos of quick reference flowcharts for phone
4. **Spreadsheet**: Set up CSV template for data logging (see Section V)
5. **Folder structure**: Store field test results in dated subdirectories

---

## Last Updated

2026-04-21

All field testing documentation is current and tested with real-world measurements (see worked example).

---

**Start here, then navigate by use case above** 👆

