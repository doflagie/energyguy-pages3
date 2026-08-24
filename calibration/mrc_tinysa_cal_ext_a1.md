# MAINTENANCE REQUIREMENT CARD (MRC)
**SYSCOM MRC CONTROL NUMBER:** TSA-CAL-EXT-A1-N
**DATE:** August 2026
**PERIODICITY:** A-1 (Annual)
**LOCATION:** RF Test Bench
**EQUIPMENT:** TinySA (Spectrum Analyzer) — Full External Verification

## MAINTENANCE REQUIREMENT DESCRIPTION:
1. Verify frequency accuracy at multiple spot frequencies across the full operating range (not just one reference point).
2. Verify amplitude accuracy against a known reference level.
3. Sanity-check RBW/filter shape and noise floor against prior-year baseline.
4. Document year-over-year drift trend for the unit.

## SAFETY PRECAUTIONS:
1. Warm up TinySA 10-15 minutes before starting.
2. Keep all injected signal levels within the TinySA's safe input rating — use the calibration system's signal/level reference output, not a raw high-power source.

## TOOLS, PARTS, MATERIALS, TEST EQUIPMENT (TPMTE):
1. [IV] GPS-Disciplined Field Calibration System (GPSDO + frequency counter + signal/level reference)
2. [IV] SOLT calibration kit (for any associated port/connector verification)
3. [IV] NanoVNA, as an independent secondary reference source (see TSA-CAL-XCHK-R1-N for the dedicated cross-check card)
4. [IV] Coax jumpers/adapters

## PROCEDURE:
1. **Warm-Up:** Power on, wait 10-15 minutes.
2. **Multi-Point Frequency Check:**
   - Using the GPSDO/signal reference, generate known signals at low, mid, and high points of the TinySA's usable range (e.g., ~1 MHz, ~150 MHz, near top of range).
   - Measure and log ppm error at each point. A frequency-dependent error pattern (not just a flat offset) indicates something beyond simple ppm correction — note for further investigation.
3. **Amplitude Accuracy Check:**
   - Inject a known reference level (from the calibration system's signal/level reference) at one or more frequencies.
   - Compare TinySA's displayed amplitude (dBm) against the known reference value; log the offset.
4. **Noise Floor / RBW Sanity Check:**
   - With no input signal (terminated in 50Ω), record the displayed noise floor at a standard RBW setting.
   - Compare against last year's baseline — a rising noise floor can indicate front-end degradation.
5. **Cross-Check Against NanoVNA (optional but recommended):**
   - Run TSA-CAL-XCHK-R1-N as part of this annual check if not already done recently.
6. **Log and Compare:**
   - Record all figures (frequency error at 3 points, amplitude offset, noise floor) with date.
   - Compare against the prior year's TSA-CAL-EXT-A1-N log entry for trend.

---
**NET STATUS: SAT / UNSAT** — mark SAT if all figures are within the unit's rated tolerances and consistent with the historical trend; mark UNSAT and consider service/replacement if drift has become erratic or noise floor has risen significantly.
