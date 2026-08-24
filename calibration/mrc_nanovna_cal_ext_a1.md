# MAINTENANCE REQUIREMENT CARD (MRC)
**SYSCOM MRC CONTROL NUMBER:** NVNA-CAL-EXT-A1-N
**DATE:** August 2026
**PERIODICITY:** A-1 (Annual)
**LOCATION:** RF Test Bench
**EQUIPMENT:** NanoVNA (Vector Network Analyzer) — Full External Verification

## MAINTENANCE REQUIREMENT DESCRIPTION:
1. Inspect and verify the SOLT calibration kit's own standards (Short/Open/Load/Thru) for wear, oxidation, or drift — a degraded standard silently corrupts every calibration done with it.
2. Verify measurement accuracy at multiple spot frequencies across the full operating range using a known-good reference (precision load, known cable, or a second calibrated instrument).
3. Document year-over-year drift/wear trend for both the instrument and the calibration kit.

## SAFETY PRECAUTIONS:
1. Warm up NanoVNA 10-15 minutes before starting.
2. Handle calibration standards by their bodies, not the connector threads/pins — skin oils and mechanical wear degrade precision standards over time.

## TOOLS, PARTS, MATERIALS, TEST EQUIPMENT (TPMTE):
1. [IV] SOLT calibration kit (the one in routine use)
2. [IV] A second, independent precision 50Ω load or known-good cable/attenuator (as an outside reference, ideally not from the same kit)
3. [IV] GPS-Disciplined Field Calibration System, for frequency reference during the sweep
4. [IV] TinySA, as an independent secondary reference source (see NVNA-CAL-XCHK-R1-N for the dedicated cross-check card)
5. [II] Isopropyl alcohol (99%), for connector cleaning/inspection

## PROCEDURE:
1. **Warm-Up:** Power on, wait 10-15 minutes.
2. **Inspect the Calibration Kit:**
   - Visually inspect SHORT, OPEN, and LOAD standards' connectors for oxidation, bent pins, or looseness.
   - Clean with IPA; note any standard that looks worn enough to warrant replacement.
3. **Fresh SOLT Calibration:**
   - Perform a full SOLT calibration (per NVNA-CAL-SOLT-SU1-N) across the widest range you routinely use.
4. **Cross-Check Against an Independent Reference:**
   - Measure the independent (non-kit) precision load or known cable.
   - Compare the result against its known/expected value (e.g., precision load should read very close to 50Ω +j0 across the band; a known cable's loss/electrical length should match its spec or prior baseline).
   - Log any deviation.
5. **Multi-Point Frequency Check:**
   - Using the GPSDO/signal reference, verify frequency accuracy at low, mid, and high points of the range you routinely use.
   - Log ppm error at each point.
6. **Cross-Check Against TinySA (optional but recommended):**
   - Run NVNA-CAL-XCHK-R1-N as part of this annual check if not already done recently.
7. **Log and Compare:**
   - Record kit condition notes, independent-reference deviation, and frequency error figures with date.
   - Compare against the prior year's NVNA-CAL-EXT-A1-N log entry for trend.

---
**NET STATUS: SAT / UNSAT** — mark SAT if the calibration kit is in good condition and all cross-checks are within tolerance; mark UNSAT and replace/retire any degraded standard, or service the instrument, if not.
