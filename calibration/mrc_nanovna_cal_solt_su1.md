# MAINTENANCE REQUIREMENT CARD (MRC)
**SYSCOM MRC CONTROL NUMBER:** NVNA-CAL-SOLT-SU1-N
**DATE:** August 2026
**PERIODICITY:** SU-1 (Pre-Session)
**LOCATION:** RF Test Bench
**EQUIPMENT:** NanoVNA (Vector Network Analyzer) — Port Calibration

## MAINTENANCE REQUIREMENT DESCRIPTION:
1. Perform SOL (1-port) or SOLT (2-port) calibration before every measurement session.
2. Calibrate only the exact frequency range that will be measured.
3. Verify calibration quality against a known-good load before trusting results.

**Perform before:** every measurement session — this is not an occasional check, it's required each time the frequency range or setup changes.

## SAFETY PRECAUTIONS:
1. Let the NanoVNA warm up (10-15 minutes, running any sweep) before calibrating — crystal frequency drifts while warming up.
2. Clean all connectors with isopropyl alcohol before calibrating; finger-tighten only, no cross-threading.

## TOOLS, PARTS, MATERIALS, TEST EQUIPMENT (TPMTE):
1. [IV] NanoVNA
2. [IV] SOLT calibration kit (Short, Open, Load standards; Thru adapter for 2-port)
3. [II] Isopropyl alcohol (99%), for connector cleaning
4. [IV] Microfiber cloth / connector brush

## PROCEDURE:
1. **Set Frequency Range:** Set START/STOP to exactly the range you intend to measure — do not calibrate wider than needed, and never measure outside a calibrated range.
2. **1-Port (SOL) Calibration — antennas, impedance, SWR:**
   - CAL → CALIBRATE → S11.
   - Connect SHORT to CH0 → press SHORT, wait for sweep.
   - Remove SHORT, connect OPEN to CH0 → press OPEN, wait for sweep.
   - Remove OPEN, connect 50Ω LOAD to CH0 → press LOAD, wait for sweep.
   - Press DONE/APPLY. Save to a cal slot (CAL → SAVE → slot 0-4).
3. **2-Port (SOLT) Calibration — filters, amplifiers, attenuators, cables:**
   - CAL → CALIBRATE → S11+S21+S22+S12 (full 2-port).
   - CH0 (PORT1): SHORT → OPEN → LOAD, as above.
   - Connect CH0 to CH1 via THRU adapter → press THRU.
   - Disconnect THRU. CH1 (PORT2): SHORT → OPEN → LOAD.
   - Press APPLY, save calibration.
4. **Verify:**
   - Connect the 50Ω LOAD standard.
   - Confirm SWR reads 1.00:1 and impedance reads 50Ω +j0 (Smith chart dead center).
   - If not near-perfect, re-clean connectors and re-calibrate before proceeding.

---
**NET STATUS: SAT / UNSAT** — mark SAT if the LOAD verification is clean; mark UNSAT and re-calibrate (checking standards quality and connector condition) if not.
