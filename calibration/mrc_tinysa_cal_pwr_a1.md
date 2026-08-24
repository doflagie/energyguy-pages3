# MAINTENANCE REQUIREMENT CARD (MRC)
**SYSCOM MRC CONTROL NUMBER:** TSA-CAL-PWR-A1-N
**DATE:** August 2026
**PERIODICITY:** A-1 (Annual)
**LOCATION:** RF Test Bench
**EQUIPMENT:** TinySA (Spectrum Analyzer) — Signal Generator Output

## MAINTENANCE REQUIREMENT DESCRIPTION:
1. Verify the TinySA's built-in signal generator (LOW/HIGH output) delivers the power level it reports.
2. Record any offset between commanded and actual output level.
3. Check output level consistency across the frequency range used for signal-source work.

## SAFETY PRECAUTIONS:
1. Confirm the external power meter's input rating before connecting — don't exceed its max safe input.
2. Warm up TinySA 10-15 minutes before calibrating.
3. Use good-quality, known-loss cables/adapters between TinySA output and the power meter; a lossy or damaged jumper will masquerade as an output error.

## TOOLS, PARTS, MATERIALS, TEST EQUIPMENT (TPMTE):
1. [IV] External power meter (or the calibration system's power reference)
2. [IV] Known-loss coax jumper(s) and adapters (characterize their loss first if not already known)
3. [IV] GPS-Disciplined Field Calibration System, for frequency reference during the sweep

## PROCEDURE:
1. **Warm-Up:** Power on TinySA, wait 10-15 minutes.
2. **Baseline Cable Loss:**
   - If not already known, measure/record the loss of the jumper(s) connecting TinySA's output to the power meter (via NanoVNA S21 or prior characterization).
3. **Set Known Output:**
   - Command the TinySA's signal generator to a known frequency and power level (e.g., 0 dBm at a representative low, mid, and high frequency you commonly use as a source).
4. **Measure Actual Output:**
   - Connect to the power meter through the characterized jumper.
   - Record the measured power, correct for jumper loss, and compare to the commanded level.
5. **Log Offset:**
   - Record the offset (dB) at each test frequency and power level.
   - If the offset is consistent, note it as a standing correction factor for future use of the signal generator.
   - If the offset varies significantly with frequency, log the full offset curve rather than a single number.
6. **Repeat at a Second Power Level (optional but recommended):**
   - Check at least one additional commanded level (e.g., -20 dBm) to confirm the offset holds across the working range, not just at one point.

---
**NET STATUS: SAT / UNSAT** — mark SAT if offsets are small and consistent with prior years (or successfully characterized as a correction factor); mark UNSAT if the output has become erratic, level-dependent in an unpredictable way, or has drifted well beyond the unit's rated accuracy.
