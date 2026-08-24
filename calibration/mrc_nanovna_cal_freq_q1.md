# MAINTENANCE REQUIREMENT CARD (MRC)
**SYSCOM MRC CONTROL NUMBER:** NVNA-CAL-FREQ-Q1-N
**DATE:** August 2026
**PERIODICITY:** Q-1 (Quarterly)
**LOCATION:** RF Test Bench
**EQUIPMENT:** NanoVNA (Vector Network Analyzer) — Frequency Reference

## MAINTENANCE REQUIREMENT DESCRIPTION:
1. Verify NanoVNA's internal frequency reference against a known-accurate external standard.
2. Apply frequency correction if drift exceeds acceptable tolerance.
3. Log the measured error (ppm) to track long-term aging/drift trend.

## SAFETY PRECAUTIONS:
1. Allow NanoVNA to warm up (10-15 minutes, running a sweep) before calibrating — crystal reference drifts during warm-up.
2. Keep injected signal levels within the unit's safe input rating.

## TOOLS, PARTS, MATERIALS, TEST EQUIPMENT (TPMTE):
1. [IV] GPS-Disciplined Field Calibration System (GPSDO — NEO-M8N + Si5351 + ESP32) — primary reference
2. [IV] Frequency counter (from calibration system), for cross-check
3. [IV] Alternate off-air reference if GPSDO unavailable: WWV/WWVH/CHU time-standard broadcast, or a known FM broadcast pilot tone
4. [IV] Coax jumpers/adapters to connect reference source to NanoVNA input

## PROCEDURE:
1. **Warm-Up:** Power on NanoVNA, run a sweep, let it run 10-15 minutes before proceeding.
2. **Generate Reference Signal:**
   - Set the GPSDO (Si5351 output) to a known frequency in NanoVNA's range (e.g., 10 MHz), or tune to an off-air standard (WWV 5/10/15 MHz, CHU 3.330/7.850/14.670 MHz) if the GPSDO isn't set up.
3. **Measure on NanoVNA:**
   - Use the receiver/spectrum-style measurement (or feed the reference into CH0 and observe the response) with a narrow span centered on the reference frequency.
   - Place a marker on the peak and read the displayed frequency.
4. **Calculate Error:**
   - Error (ppm) = (displayed freq − reference freq) / reference freq × 1,000,000.
5. **Apply Correction (if needed):**
   - If error exceeds tolerance (baseline: within the unit's rated TCXO spec, typically low single-digit ppm), apply the frequency correction setting in the config/expert menu per current firmware documentation.
   - Re-measure after correction to confirm.
6. **Log Result:**
   - Record date, measured ppm error (pre- and post-correction), and reference source used.
   - Compare against prior quarter's log entry to watch for accelerating drift.
   - **Note:** A drifted frequency reference on the NanoVNA shifts where markers/resonances appear (e.g., an antenna's measured resonant frequency), so this check matters even though SOLT calibration corrects amplitude/phase errors — it does not correct a frequency reference error.

---
**NET STATUS: SAT / UNSAT** — mark SAT if within tolerance (post-correction if applied); mark UNSAT and flag for annual external verification (NVNA-CAL-EXT-A1-N) if drift is unusually large or accelerating.
