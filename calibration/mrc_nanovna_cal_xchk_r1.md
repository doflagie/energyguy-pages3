# MAINTENANCE REQUIREMENT CARD (MRC)
**SYSCOM MRC CONTROL NUMBER:** NVNA-CAL-XCHK-R1-N
**DATE:** August 2026
**PERIODICITY:** R-1 (Situational)
**LOCATION:** RF Test Bench
**EQUIPMENT:** NanoVNA (Vector Network Analyzer) — Cross-Tool Check vs. TinySA

## MAINTENANCE REQUIREMENT DESCRIPTION:
1. Cross-check NanoVNA frequency readings against the TinySA using a common signal.
2. Use as a quick confidence check anytime one instrument's calibration is in doubt (after a drop, before a critical measurement session, after firmware update, or when the two units disagree on a field measurement).

**Perform after:** any physical shock/drop to either instrument, any firmware update to either instrument, or whenever field results from the two units don't agree and you need to know which one to trust. Mirrors `TSA-CAL-XCHK-R1-N` from the TinySA's side of the same check.

## SAFETY PRECAUTIONS:
1. Keep signal levels within both instruments' safe input ratings.
2. Warm up both instruments 10-15 minutes before comparing.

## TOOLS, PARTS, MATERIALS, TEST EQUIPMENT (TPMTE):
1. [IV] NanoVNA
2. [IV] TinySA
3. [IV] Coax jumpers/adapters, T-connector or splitter if comparing both instruments against one source simultaneously

## PROCEDURE:
1. **Warm-Up:** Power on both instruments, wait 10-15 minutes.
2. **Set a Known Source Frequency:**
   - Set the NanoVNA's internal source (CH0) to output a known, fixed frequency and note the set value.
3. **Measure on TinySA:**
   - Connect NanoVNA CH0 output to the TinySA input at a safe level.
   - Sweep a narrow span on TinySA centered on the source frequency; record displayed frequency and amplitude.
4. **Compare:**
   - Compare TinySA's reading against the NanoVNA's set/known value.
   - A small, consistent offset is normal instrument-to-instrument variance; a large or erratic disagreement flags one (or both) units as needing the full annual verification (NVNA-CAL-EXT-A1-N or TSA-CAL-EXT-A1-N).
5. **Log Result:**
   - Record date, trigger reason (drop/firmware update/field disagreement/routine), and the measured offset.

---
**NET STATUS: SAT / UNSAT** — mark SAT if the two instruments agree within normal variance; mark UNSAT and escalate to the full annual verification card for whichever instrument is suspect if they disagree significantly.
