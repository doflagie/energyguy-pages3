# MAINTENANCE REQUIREMENT CARD (MRC)
**SYSCOM MRC CONTROL NUMBER:** TSA-SPUR-R1-N
**DATE:** August 2026
**PERIODICITY:** R-1 (Situational) / A-1 (Annual)
**LOCATION:** RF Test Bench
**EQUIPMENT:** TinySA (Spectrum Analyzer) — Transmitter Spurious Emission Check

## MAINTENANCE REQUIREMENT DESCRIPTION:
1. Build protection chain (dummy load, coupler, pad) before connecting any transmitter to the TinySA.
2. Sweep fundamental plus 2nd-4th harmonics, record dBc for each.
3. Sweep wide for unrelated spurs (mixing products, supply noise, PLL spurs).
4. Compare results against FCC Part 97.307 and prior baseline.

**Perform after:** any transmitter or amplifier repair/modification, new/repaired antenna or feedline install, or as an annual station compliance check.

## SAFETY PRECAUTIONS:
1. **Never connect a transmitter directly to the TinySA input** — front-end is not rated for TX power and will be destroyed instantly.
2. Terminate the transmitter into a dummy load rated for full TX power before keying.
3. Verify the sample level reaching the TinySA with a wattmeter/power meter before the first connection of any new setup — do not trust the coupler/pad math alone.
4. Do not rely on the TinySA's internal attenuator as the only protection against full TX power.
5. Key the transmitter only long enough to capture the reading — avoid unnecessary dwell time into the dummy load.

## TOOLS, PARTS, MATERIALS, TEST EQUIPMENT (TPMTE):
1. [IV] TinySA (Basic or Ultra)
2. [IV] Dummy load, rated for full TX power
3. [IV] Loosely-coupled tandem (directional) coupler, -30 to -50 dB coupling factor
4. [IV] Fixed attenuator pad (rated for actual power), as needed if coupling alone is insufficient
5. [IV] Wattmeter/power meter, for pre-connection verification
6. [IV] Coax jumpers and adapters matching connector types in the chain

## PROCEDURE:
1. **Build the Protection Chain:**
   - Dummy load → loosely-coupled tandem coupler (preferred) → attenuator pad (if needed) → TinySA LOW input.
   - Calculate: TX power (dBm) − coupler (dB) − pad (dB, if used) = power at TinySA input. Confirm well under the TinySA's safe input rating.
   - Verify with the wattmeter before connecting the TinySA.
2. **Set Up the Sweep:**
   - Set span to cover the fundamental plus at least the 2nd through 4th harmonics.
   - Set RBW narrow enough to resolve close-in spurs (auto-RBW is a reasonable start).
   - Set reference level just above the expected fundamental peak; set scale to keep fundamental and noise floor both visible.
3. **Measure the Fundamental:**
   - Key the transmitter at a known, low power level into the dummy load.
   - Place a marker on the fundamental peak; note frequency and amplitude.
4. **Measure Harmonics:**
   - Use DELTA marker mode: reference marker on the fundamental, second marker on each harmonic.
   - Record frequency and dBc for the 2nd, 3rd, and 4th harmonics (and higher, if of interest).
5. **Sweep for Unrelated Spurs:**
   - Widen span well above and below the harmonic series.
   - Use MAX HOLD across several keying cycles to catch intermittent spurs.
   - Key down and confirm any suspicious peak disappears — anything present with TX off is TinySA noise floor or ambient RF, not a transmitter spur.
6. **Repeat at Full Power:**
   - Some spurs only appear near saturation — repeat steps 3-5 at full rated TX power.
7. **Record and Compare:**
   - Log fundamental frequency/power, each harmonic's frequency and dBc, and any unexplained spurs.
   - Compare against FCC Part 97.307 (verify current table — limits vary by power level and by fundamental above/below 30 MHz) and against this equipment's prior baseline reading.

---
**NET STATUS: SAT / UNSAT** — mark SAT if all harmonics and spurs meet Part 97.307 limits and are consistent with baseline; mark UNSAT and inspect the transmitter/amplifier before further use if not.
