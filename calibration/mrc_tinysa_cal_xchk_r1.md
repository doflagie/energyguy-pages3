# MAINTENANCE REQUIREMENT CARD (MRC)
**SYSCOM MRC CONTROL NUMBER:** TSA-CAL-XCHK-R1-N
**DATE:** August 2026
**PERIODICITY:** R-1 (Situational)
**LOCATION:** RF Test Bench
**EQUIPMENT:** TinySA (Spectrum Analyzer) — Cross-Tool Check vs. NanoVNA

## MAINTENANCE REQUIREMENT DESCRIPTION:
1. Cross-check TinySA frequency/amplitude readings against the NanoVNA using a common signal.
2. Use as a quick confidence check anytime one instrument's calibration is in doubt (after a drop, before a critical measurement session, after firmware update, or when the two units disagree on a field measurement).

**Perform after:** any physical shock/drop to either instrument, any firmware update to either instrument, or whenever field results from the two units don't agree and you need to know which one to trust.

## SAFETY PRECAUTIONS:
1. Keep signal levels within both instruments' safe input ratings.
2. Warm up both instruments 10-15 minutes before comparing.

## TOOLS, PARTS, MATERIALS, TEST EQUIPMENT (TPMTE):
1. [IV] NanoVNA
2. [IV] TinySA
3. [IV] Coax jumpers/adapters, T-connector or splitter if comparing both instruments against one source simultaneously

## PROCEDURE:
1. **Warm-Up:** Power on both instruments, wait 10-15 minutes.
2. **Common Signal Source:**
   - Use the NanoVNA's internal source (a known, set frequency) as the reference, or use the calibration system's GPSDO/signal reference feeding both instruments via a splitter.
3. **Measure on TinySA:**
   - If using the NanoVNA as the source, connect its output directly to the TinySA input at a safe level.
   - Sweep a narrow span on TinySA centered on the source frequency; record displayed frequency and amplitude.
4. **Compare:**
   - Compare TinySA's reading against the NanoVNA's set/known value.
   - A small, consistent offset is normal instrument-to-instrument variance; a large or erratic disagreement flags one (or both) units as needing the full annual verification (TSA-CAL-EXT-A1-N) or the NanoVNA's own calibration check.
5. **Log Result:**
   - Record date, trigger reason (drop/firmware update/field disagreement/routine), and the measured offset.

---
**NET STATUS: SAT / UNSAT** — mark SAT if the two instruments agree within normal variance; mark UNSAT and escalate to TSA-CAL-EXT-A1-N (or the NanoVNA's own calibration procedure) if they disagree significantly.
