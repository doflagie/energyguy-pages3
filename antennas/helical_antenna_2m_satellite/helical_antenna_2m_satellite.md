# HELICAL ANTENNA 2M SATELLITE ANTENNA
## TECHNICAL DATA SHEET

**Document Number:** TM-HELICAL-ANTE-001 Rev A
**Equipment:** HELICAL ANTENNA 2M SATELLITE ANTENNA
**Classification:** UNCLASSIFIED — Amateur Radio / Field Use
**Date:** 2026-05-24
**Supersedes:** None (initial issue)

---

## RECORD OF CHANGES

| Change No. | Rev | Date | Description | By |
|:---:|:---:|:---:|---|:---:|
| 1 | A | 2026-05-24 | Initial formatted release | M. Martin |

---


**Bands:** 2M satellite (144–146 MHz)
**Approach:** Solenoid helix in axial mode: circular polarization for satellite work, gain via multi-turn helical structure
**Compaction:** 0%

---

## CHAPTER 1 — DESCRIPTION

Solenoid helix in axial mode: circular polarization for satellite work, gain via multi-turn helical structure

**Special parameters:** 3-turn helix, 4.5 in diameter, 4.5 in pitch, ~8 dBic gain, right-hand circular polarization, 50Ω feedpoint

---

## CHAPTER 2 — TECHNICAL CHARACTERISTICS

| Parameter | Value |
|-----------|-------|
| Frequency bands | 2M satellite (144–146 MHz) |
| Feedpoint impedance | 50 Ω (nominal) |
| SWR target | < 2.0:1 |
| Efficiency | 70–95% |
| Gain | 0–12 dBi |

---

## CHAPTER 3 — COMPONENTS AND MATERIALS

See detailed specification in helical_antenna_2m_satellite_specs.csv

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

See helical_antenna_2m_satellite_specs.csv

---

## APPENDIX B — DIAGRAMS

See figures/ folder for antenna layout, circuit schematic, construction detail

---

## APPENDIX C — NEC MODEL

See helical_antenna_2m_satellite.nec for electromagnetic simulation

---

*Navy Technical Manual Level — Generated with NEC2 modeling*
