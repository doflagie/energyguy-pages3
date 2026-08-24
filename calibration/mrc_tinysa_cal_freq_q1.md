# MAINTENANCE REQUIREMENT CARD (MRC)
**SYSCOM MRC CONTROL NUMBER:** TSA-CAL-FREQ-Q1-N
**DATE:** August 2026
**PERIODICITY:** Q-1 (Quarterly)
**LOCATION:** RF Test Bench
**EQUIPMENT:** TinySA (Spectrum Analyzer) — Frequency Reference

## MAINTENANCE REQUIREMENT DESCRIPTION:
1. Verify TinySA's internal frequency reference against a known-accurate external standard.
2. Apply frequency correction if drift exceeds acceptable tolerance.
3. Log the measured error (ppm) to track long-term aging/drift trend.

## SAFETY PRECAUTIONS:
1. Allow TinySA to warm up (10-15 minutes, powered on) before calibrating — crystal reference drifts during warm-up.
2. Keep signal levels into the input within safe range per the unit's spec (no direct high-power connections).

## TOOLS, PARTS, MATERIALS, TEST EQUIPMENT (TPMTE):
1. [IV] GPS-Disciplined Field Calibration System (GPSDO — NEO-M8N + Si5351 + ESP32) — primary reference
2. [IV] Frequency counter (from calibration system), for cross-check
3. [IV] Alternate off-air reference if GPSDO unavailable: WWV/WWVH/CHU time-standard broadcast, or a known FM broadcast pilot tone
4. [IV] Coax jumpers/adapters to connect reference source to TinySA input

## PROCEDURE:
1. **Warm-Up:** Power on TinySA, let it run 10-15 minutes before proceeding.
2. **Generate Reference Signal:**
   - Set the GPSDO (Si5351 output) to a known frequency in TinySA's range (e.g., 10 MHz), or tune to an off-air standard (WWV 5/10/15 MHz, CHU 3.330/7.850/14.670 MHz) if the GPSDO isn't set up.
3. **Measure on TinySA:**
   - Sweep a narrow span centered on the reference frequency.
   - Place a marker on the peak and read the displayed frequency.
4. **Calculate Error:**
   - Error (ppm) = (displayed freq − reference freq) / reference freq × 1,000,000.
5. **Apply Correction (if needed):**
   - If error exceeds tolerance (baseline: within the unit's rated TCXO spec, typically low single-digit ppm), apply the frequency correction setting in the calibration/config menu per current firmware documentation.
   - Re-measure after correction to confirm.
6. **Log Result:**
   - Record date, measured ppm error (pre- and post-correction), and reference source used.
   - Compare against prior quarter's log entry to watch for accelerating drift (may indicate aging crystal or thermal issue).

---
**NET STATUS: SAT / UNSAT** — mark SAT if within tolerance (post-correction if applied); mark UNSAT and flag for annual external verification (TSA-CAL-EXT-A1-N) if drift is unusually large or accelerating.
