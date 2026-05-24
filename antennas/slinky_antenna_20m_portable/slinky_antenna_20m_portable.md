# SLINKY ANTENNA 20M PORTABLE ANTENNA
## TECHNICAL DATA SHEET

**Document Number:** TM-SLINKY-ANTEN-001 Rev A
**Equipment:** SLINKY ANTENNA 20M PORTABLE ANTENNA
**Classification:** UNCLASSIFIED — Amateur Radio / Field Use
**Date:** 2026-05-24
**Supersedes:** None (initial issue)

---

## RECORD OF CHANGES

| Change No. | Rev | Date | Description | By |
|:---:|:---:|:---:|---|:---:|
| 1 | A | 2026-05-24 | Initial formatted release | M. Martin |

---


**Bands:** 20M (14.0–14.35 MHz)
**Approach:** Compact loading using helical spring (slinky toy) as radiator or loading element, field-portable, collapsible
**Compaction:** 85%

---

## CHAPTER 1 — DESCRIPTION

Compact loading using helical spring (slinky toy) as radiator or loading element, field-portable, collapsible

**Special parameters:** Spring-steel slinky as loading coil, collapses to 2 in for transport, ~18 ft deployed length, 50Ω feedpoint, full electrical length at resonance

---

## CHAPTER 2 — TECHNICAL CHARACTERISTICS

| Parameter | Value |
|-----------|-------|
| Frequency bands | 20M (14.0–14.35 MHz) |
| Feedpoint impedance | 50 Ω (nominal) |
| SWR target | < 2.0:1 |
| Efficiency | 70–95% |
| Gain | 0–12 dBi |

---

## CHAPTER 3 — COMPONENTS AND MATERIALS

See detailed specification in slinky_antenna_20m_portable_specs.csv

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

See slinky_antenna_20m_portable_specs.csv

---

## APPENDIX B — DIAGRAMS

See figures/ folder for antenna layout, circuit schematic, construction detail

---

## APPENDIX C — NEC MODEL

See slinky_antenna_20m_portable.nec for electromagnetic simulation

---

*Navy Technical Manual Level — Generated with NEC2 modeling*
