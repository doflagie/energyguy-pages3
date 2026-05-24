# SLOT ANTENNA UHF ANTENNA
## TECHNICAL DATA SHEET

**Document Number:** TM-SLOT-ANTENNA-001 Rev A
**Equipment:** SLOT ANTENNA UHF ANTENNA
**Classification:** UNCLASSIFIED — Amateur Radio / Field Use
**Date:** 2026-05-24
**Supersedes:** None (initial issue)

---

## RECORD OF CHANGES

| Change No. | Rev | Date | Description | By |
|:---:|:---:|:---:|---|:---:|
| 1 | A | 2026-05-24 | Initial formatted release | M. Martin |

---


**Bands:** 70cm band (430–440 MHz)
**Approach:** Slot antenna: slot cut in metal sheet or PCB ground plane, complementary to dipole, wideband variant
**Compaction:** 0%

---

## CHAPTER 1 — DESCRIPTION

Slot antenna: slot cut in metal sheet or PCB ground plane, complementary to dipole, wideband variant

**Special parameters:** 36 mm slot on PCB ground plane, dipole slot pair configuration, 50Ω microstrip feed, wideband design for full 70cm band

---

## CHAPTER 2 — TECHNICAL CHARACTERISTICS

| Parameter | Value |
|-----------|-------|
| Frequency bands | 70cm band (430–440 MHz) |
| Feedpoint impedance | 50 Ω (nominal) |
| SWR target | < 2.0:1 |
| Efficiency | 70–95% |
| Gain | 0–12 dBi |

---

## CHAPTER 3 — COMPONENTS AND MATERIALS

See detailed specification in slot_antenna_uhf_specs.csv

---

## CHAPTER 4 — FABRICATION

1. Review design parameters and dimensions
2. Procure materials per component list
3. Fabricate elements per specifications
4. Install feedpoint connector
5. Assemble and secure structure
6. Verify physical dimensions match design
7. Test with antenna analyzer (nanoVNA)

---

## CHAPTER 5 — TESTING

Use nanoVNA and tinySA per FIELD_TESTING_WITH_INSTRUMENTS.md:

1. **Resonance:** Find zero-reactance dip on Smith chart
2. **SWR:** Measure across band, target < 2.0:1
3. **Impedance:** Record R and jX at center frequency
4. **Spectrum:** Use tinySA to verify radiation pattern

---

## CHAPTER 6 — TROUBLESHOOTING

**High SWR:** Check physical dimensions, verify feed connection
**Frequency offset:** Adjust element length or loading
**Impedance mismatch:** Use antenna tuner or feed network

---

## APPENDIX A — PARTS LIST

See slot_antenna_uhf_specs.csv

---

## APPENDIX B — DIAGRAMS

See figures/ folder for antenna layout, circuit schematic, construction detail

---

## APPENDIX C — NEC MODEL

See slot_antenna_uhf.nec for electromagnetic simulation

---

*Navy Technical Manual Level — Generated with NEC2 modeling*
