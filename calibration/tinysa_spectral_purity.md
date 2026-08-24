# Transmitter Spectral Purity Testing (TinySA)

## Overview

Spectral purity testing checks a transmitter's harmonics and spurious emissions against its fundamental — verifying the rig isn't radiating significant energy outside the intended signal. The TinySA is purpose-built for this: real spectrum analyzer architecture, calibrated, with the dynamic range to resolve spurs well below the fundamental.

---

## Mandatory Protection Chain

**Never connect a transmitter directly to the TinySA input.** Its front-end is not rated for TX power and will be destroyed instantly. Build this chain every time:

1. **Dummy load** — TX output terminates here, rated for full TX power.
2. **Sampling port — prefer loose coupling.** A loosely-coupled directional (tandem match) coupler, high coupling factor (e.g., -30 to -50 dB), is the preferred method: it taps off only a tiny fraction of forward power, often reducing or eliminating the need for a separate attenuator pad, and adds minimal insertion loss to the main TX path. Because harmonic testing is a *relative* measurement (dBc), the exact coupling factor doesn't need to be precisely known — it applies equally to the fundamental and its harmonics and cancels out of the ratio. **This only holds if the coupling is reasonably flat across the frequency range being swept** (fundamental through the harmonics of interest). A proper transmission-line-style tandem coupler is flat over a wide band; an ad hoc proximity tap (bare wire laid near the feedline) can have coupling that shifts noticeably with frequency, skewing dBc readings even though spur presence/absence is still valid. Use a real tandem coupler when precise dBc numbers matter; a proximity tap is fine only for a quick go/no-go check.
3. **Fixed attenuator pad** — add if the loose coupling alone doesn't bring the sample comfortably under the TinySA's safe input, rated for the actual power hitting it. Check your specific TinySA model's max safe input (basic TinySA and TinySA Ultra differ).
4. **TinySA input** (LOW port for RX/SA measurements).

**Math check before every session:** TX power (dBm) − coupler (dB) − pad (dB, if used) = power at TinySA input. Confirm this stays well under the max safe input with margin.

**Verify with a wattmeter/power meter first** — don't trust the calculation alone on the first connection of a new setup, and don't rely on TinySA's own internal attenuator as your only protection against full TX power.

---

## Procedure

1. Connect the transmitter through the full protection chain above into the TinySA LOW input.
2. Set span to cover the fundamental plus at least the 2nd through 4th harmonics (e.g., for 7 MHz, span 0-30+ MHz).
3. Set RBW appropriately narrow to resolve close-in spurs without making the sweep too slow (TinySA auto-RBW is a reasonable starting point).
4. Set reference level (top of screen) just above the expected fundamental peak so it doesn't clip, and set scale (dB/div) to keep both the fundamental and the noise floor visible.
5. Key the transmitter at a known, low power level into the dummy load.
6. Note the fundamental peak amplitude and frequency (use MARKER to place a marker directly on the peak).
7. Use the DELTA marker function: place the reference marker on the fundamental, then move a second marker to each harmonic to read dBc (dB relative to carrier) directly.
8. Step through 2nd, 3rd, 4th (and higher, if of interest) harmonics, recording frequency and dBc for each.
9. Widen the span and sweep well above and below the harmonic series for unrelated spurs — mixing products, switching power supply noise, VFO/PLL spurs, etc.
10. Record: fundamental frequency/power, each harmonic's frequency and dBc, any unexplained spurs and their dBc.

### Tips

- Use MAX HOLD while keying the transmitter a few times to catch intermittent spurs.
- If a peak looks suspicious, key down and confirm it disappears — anything present with the TX off is TinySA's own noise floor or ambient RF, not a transmitter spur.
- Repeat at more than one TX power level (e.g., low and full power) — some spurs only appear near saturation.

---

## Interpreting Results

FCC Part 97.307 sets spurious emission limits, which vary by power level and by whether the fundamental is below or above 30 MHz — check the current table rather than relying on a remembered figure. As a general health-check baseline, hams typically look for:

- Harmonics at least 40 dB down from the fundamental at minimum
- A clean rig typically shows 50-60+ dB down
- No unexplained spurs above the noise floor outside the harmonic series

---

## Quick Reference

**Protection chain:** Dummy load → loosely-coupled tandem coupler (preferred) → attenuator pad (if needed) → TinySA LOW input (verify math with wattmeter first)

**Key markers:** Fundamental = reference marker; harmonics = delta marker readings in dBc

**Catch intermittent spurs:** MAX HOLD across several keying cycles

**Never:** Connect a transmitter directly to the TinySA input
