# K9AY LOOP 40M ANTENNA
## TECHNICAL DATA SHEET

**Document Number:** TM-K9AY-LOOP-40-001 Rev A
**Equipment:** K9AY LOOP 40M ANTENNA
**Classification:** UNCLASSIFIED — Amateur Radio / Field Use
**Date:** 2026-05-24
**Supersedes:** None (initial issue)

---

## RECORD OF CHANGES

| Change No. | Rev | Date | Description | By |
|:---:|:---:|:---:|---|:---:|
| 1 | A | 2026-05-24 | Initial formatted release | M. Martin |

---


**Bands:** 40M (7.0–7.3 MHz)
**Approach:** Directional receiving loop, tuned with variable capacitor for null rejection, non-radiating
**Compaction:** 0%

---

## CHAPTER 1 — DESCRIPTION

Directional receiving loop, tuned with variable capacitor for null rejection, non-radiating

**Special parameters:** 20 ft square loop, 1000 pF air-gap variable capacitor, cardioid null pattern, impedance step-up transformer

---

## CHAPTER 2 — TECHNICAL CHARACTERISTICS

| Parameter | Value |
|-----------|-------|
| Frequency bands | 40M (7.0–7.3 MHz) |
| Feedpoint impedance | 50 Ω (nominal) |
| SWR target | < 2.0:1 |
| Efficiency | 70–95% |
| Gain | 0–12 dBi |

---

## CHAPTER 3 — COMPONENTS AND MATERIALS

See detailed specification in k9ay_loop_40m_specs.csv

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

See k9ay_loop_40m_specs.csv

---

## APPENDIX B — DIAGRAMS

See figures/ folder for antenna layout, circuit schematic, construction detail

---

## APPENDIX C — NEC MODEL

See k9ay_loop_40m.nec for electromagnetic simulation

---

*Navy Technical Manual Level — Generated with NEC2 modeling*
