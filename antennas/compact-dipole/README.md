# Compact Spiral-Wound Dipole Antenna Suite
## TECHNICAL DATA SHEET

**Document Number:** TM-COMPACT-DIPO-001 Rev A
**Equipment:** Compact Spiral-Wound Dipole Antenna Suite
**Classification:** UNCLASSIFIED — Amateur Radio / Field Use
**Date:** 2026-05-24
**Supersedes:** None (initial issue)

---

## RECORD OF CHANGES

| Change No. | Rev | Date | Description | By |
|:---:|:---:|:---:|---|:---:|
| 1 | A | 2026-05-24 | Initial formatted release | M. Martin |

---


**Complete deliverable for portable, roll-up HF/VHF/UHF dipole antennas (160M–20cm)**

---

## 📦 Contents

### Master Documentation
- **COMPACT_DIPOLE_MANUAL.md** — Comprehensive Navy-format technical manual (26 KB)
  - Section I: Principles of operation
  - Section II: Technical characteristics (all bands)
  - Section III: Components & materials specifications
  - Section IV: Fabrication procedures (step-by-step, with safety warnings)
  - Section V: Winding table references
  - Section VI: Test and alignment procedures
  - Section VII: Troubleshooting
  - Appendices: Parts lists, schematic diagrams, NEC model references

### Per-Band Folders (14 total)
Each band folder contains:
```
160m/  80m/  40m/  30m/  20m/  17m/  15m/  12m/  10m/  6m/  2m/  1p25m/  70cm/  33cm/  20cm/
├── compact_dipole_<band>.nec          ← NEC2 electromagnetic model
├── compact_dipole_<band>.md           ← Band-specific technical chapter
├── winding_table_<band>.csv           ← Exact winding parameters (turns, wire gauge, lengths)
└── figures/
    ├── <band>_antenna_layout.png      ← Scale deployment drawing with dimensions
    ├── <band>_coil_detail.png         ← Loading coil cross-section & winding
    └── <band>_trap_schematic.png      ← Trap resonator circuit with L/C values
```

**Total per band: 7 files (NEC + MD + CSV + 3×PNG)**

### Global Specifications & Figures
- **trap_specifications.csv** — All-band trap resonator parameters
- **winding_tables_all_bands.csv** — Consolidated loading coil specifications
- **figures/** — 8 global schematic diagrams:
  - fig_01_dipole_overview.png — Complete antenna architecture
  - fig_02_roll_up_card_assembly.png — Roll-up deployment mechanism
  - fig_03_trap_schematic.png — Parallel LC trap circuit theory
  - fig_04_parallel_resonator.png — Feed-point matching network
  - fig_05_coil_winding_detail.png — Helical coil winding
  - fig_06_feedpoint_detail.png — BNC/SMA connector mounting
  - fig_07_deployment_sequence.png — 4-step unroll procedure
  - fig_08_tuning_procedure.png — Dip meter adjustment workflow

### Scripts (for future regeneration)
- **compact_dipole_generator.py** — Master generator (NEC + PNG + CSV)
- **generate_band_chapters.py** — Per-band markdown chapter generator

---

## 📊 Summary Table

| Band   | f_center | Arm Length | Loading L | Trap L | Form Size | Turns | Q |
|--------|----------|-----------|-----------|--------|-----------|-------|-----|
| 160M   | 1.90 MHz | 18.8m     | 296 μH    | 234 μH | 1.5" PVC  | 156   | 200 |
| 80M    | 3.75 MHz | 9.5m      | 76 μH     | 72 μH  | 1" PVC    | 65    | 200 |
| 40M    | 7.15 MHz | 5.0m      | 20.9 μH   | 33 μH  | 3/4" PVC  | 30    | 200 |
| 30M    | 10.13 MHz| 3.5m      | 10.4 μH   | 20.6 μH| 3/4" PVC  | 17    | 200 |
| 20M    | 14.18 MHz| 2.5m      | 5.3 μH    | 12.6 μH| 3/4" PVC  | 11    | 200 |
| 17M    | 18.12 MHz| 2.0m      | 3.25 μH   | 9.6 μH | 1/2" PVC  | 10    | 200 |
| 15M    | 21.23 MHz| 1.7m      | 2.37 μH   | 8.0 μH | 1/2" PVC  | 8     | 200 |
| 12M    | 24.94 MHz| 1.4m      | 1.72 μH   | 6.8 μH | 1/2" PVC  | 7     | 200 |
| 10M    | 28.85 MHz| 1.2m      | 1.28 μH   | 6.1 μH | 1/2" PVC  | 6     | 200 |
| 6M     | 52.0 MHz | 0.7m      | 0.39 μH   | 2.34 μH| 3/8" PVC  | 3     | 250 |
| 2M     | 146 MHz  | 0.24m     | 0.050 μH  | 0.395 μH| 3/8" PVC | 1     | 300 |
| 1.25M  | 223 MHz  | 0.16m     | 0.022 μH  | 0.256 μH| 1/4" FR4 | —     | 300 |
| 70cm   | 435 MHz  | 0.082m    | 0.0056 μH | 0.089 μH| 1/4" Cer | —     | 300 |
| 33cm   | 915 MHz  | 0.039m    | 0.0013 μH | 0.030 μH| 1/8" Cer | —     | 350 |
| 20cm   | 1270 MHz | 0.028m    | 0.00066 μH| 0.020 μH| 1/8" Cer | —     | 350 |

---

## 🎯 Design Characteristics

### Architecture
- **Center-loaded dipole:** Inductance at midpoint of each arm reduces physical length
- **Compaction:** 50% (HF: 160M–10M), 60% (VHF: 6M–2M), ~95% (UHF)
- **Feedpoint:** 50Ω, center-fed via 1:1 ferrite balun
- **Radiation pattern:** Figure-8 (broadside)

### Tuning
- **Parallel LC trap resonators:** Manual tuning via Bourns 3296 variable trimmer caps
- **Tuning range:** ±5% around band center
- **Method:** Dip meter or SWR meter to optimize trimmer position
- **Typical SWR:** < 1.5:1 @ band center after adjustment

### Performance (typical)
- **Impedance:** 50Ω ± 10%
- **Efficiency (HF):** 85–90%
- **Efficiency (VHF):** 80–85%
- **Efficiency (UHF):** 75–82%
- **Gain:** 2.0–2.5 dBi

### Form Factor
- **Roll-up card design:** HDPE substrate with edge groove for loading coil
- **Portable:** ~100–150g per antenna (HF), fits in backpack
- **Deployment time:** < 2 minutes
- **Weatherproofing:** Coils encapsulated in polyurethane foam; connectors in IP54 shrink tube

---

## 📖 How to Use This Deliverable

### For Fabrication
1. Select desired band (e.g., 40M)
2. Read per-band chapter: `40m/compact_dipole_40m.md` (Section 4 — Fabrication Procedures)
3. Obtain components from parts list (Appendix A in master manual)
4. Follow winding table: `40m/winding_table_40m.csv` for exact coil parameters
5. Reference diagrams in `40m/figures/` for visual guidance
6. Use NEC model (`40m/compact_dipole_40m.nec`) for verification in simulation software

### For Testing & Alignment
1. Connect feedline to antenna BNC connector
2. Use dip meter to approach trap resonator; adjust trimmer for minimum dip at band center
3. Verify impedance match using SWR meter (target < 1.5:1 @ band center)
4. Test radiation pattern using NEC2 simulator (4nec2, xnec2c, EZNEC)

### For Understanding Design Theory
1. Start with COMPACT_DIPOLE_MANUAL.md Section I (Principles of Operation)
2. Review global figures (figures/fig_01 through fig_08) for visual overview
3. Read Section II (Technical Characteristics) for all-band specifications
4. Consult band-specific chapters for RF details (Section II.1–2.4 in each band file)

---

## 🔧 Required Tools & Materials

### For Fabrication
- PVC pipe cutter or hacksaw
- Lathe or hand-wind jig (for coil winding)
- Soldering iron + rosin-core solder
- LCR meter or dip meter (for inductance verification)
- Wire strippers and magnifying glass

### For Testing
- SWR/power meter (1.8–1300 MHz range)
- Dip meter (for resonance verification)
- Feedline (RG-8 or better)
- QRP transmitter (< 5W initial testing)
- Dummy load (50Ω)

### For Simulation
- NEC2 software (free options: 4nec2, xnec2c)
- Linux/Windows/Mac compatible

---

## 📋 NEC Model Usage

All NEC files follow the 2-card standard format:
```nec
CM Comments (frequency, dimensions, design parameters)
CE
GW Geometry (wire segments)
LD Loading (inductance)
GE Geometry end
GN Ground (Sommerfeld-Norton, typical soil)
EX Excitation (1V source at feed point)
FR Frequency (sweep across band)
RP Radiation pattern (hemispherical)
EN End
```

**To run in 4nec2 (Linux):**
```bash
4nec2 40m/compact_dipole_40m.nec &
```
- Select "Calculate" menu
- View impedance, S11, gain, radiation pattern
- Export data to spreadsheet if desired

---

## 📞 Quick Reference

### Typical SWR Target Values
- Band center: **< 1.5:1** (after trimmer adjustment)
- Band edges: **< 2.5:1**
- Full band (7 MHz BW example): **< 3:1** acceptable

### Power Handling (Continuous CW)
- 160M–80M (AWG 22 coil): **50–75W**
- 40M–20M (AWG 22–24): **25–50W**
- 17M–6M (AWG 24–26): **10–25W**
- 2M and above: **5–10W** (small coils)

> **WARNING:** Exceeding power limits causes Q degradation and potential wire insulation failure.

### Temperature Coefficient
- Ceramic trimmer capacitors: ±250 ppm/°C
- Resonance shift across 0–50°C: ±50–100 kHz (HF bands)
- **Recommendation:** Retune seasonally or when operating in extreme temperature environments

---

## 📝 File Count Summary

- **14 band folders** × 7 files per band = **98 files**
- **Global figures:** 8 PNG files
- **Global specifications:** 3 CSV files + 1 master manual (MD)
- **Generator scripts:** 2 Python files
- **Total:** **~115 files**

---

## 🔗 Cross-References

- Comprehensive manual theory: **COMPACT_DIPOLE_MANUAL.md**
- Band-specific details: **<band>/compact_dipole_<band>.md**
- Component specifications: **trap_specifications.csv** and **winding_tables_all_bands.csv**
- Exact winding data: **<band>/winding_table_<band>.csv**
- Electromagnetic models: **<band>/compact_dipole_<band>.nec**
- Visual guidance: **figures/** (global) and **<band>/figures/** (per-band)

---

## 📄 License & Disclaimer

**Classification:** Unclassified — Educational and Amateur Radio (ITU Region 2)

**Distribution:** Public domain for licensed amateur radio and educational use

**Disclaimer:** User assumes responsibility for compliance with FCC regulations (47 CFR § 97), local zoning laws, and safe RF practices. Improper construction or operation may result in RF hazards or interference. Follow all safety warnings in the master manual.

---

## 🆘 Troubleshooting Quick Links

- **High SWR (> 3:1)?** → See COMPACT_DIPOLE_MANUAL.md Section VII.1
- **Coil inductance out of spec?** → Per-band chapter, Section 2.3 (QA procedure)
- **Frequency drift?** → Section VII.2 (temperature effects) or per-band tuning notes
- **Poor efficiency?** → Check coil Q, wire oxidation, and ground plane

---

**Last Updated:** 2026-04-21

**Document Control:** Maintained in `/home/merv/Public/claude/ant_t_r/compact-dipole/`

---

## Quick Start

1. **Read first:** COMPACT_DIPOLE_MANUAL.md Section I (Principles)
2. **Select band:** Choose from 160M–20cm
3. **Review design:** Read band-specific `compact_dipole_<band>.md`
4. **Build:** Follow fabrication procedures in Section IV
5. **Test:** Use procedures in Section VI (Electrical Testing)
6. **Simulate:** Load NEC model in 4nec2 or EZNEC
7. **Deploy:** Follow deployment sequence (Section 6.1 in per-band chapter)

**Ready to transmit!**
