# Unit 5 — Verification and Field Testing
**[TM-ANT-070 — Open Handout](../../antennas/traveling_wave_vertical/tm.html)**
**TM Chapter:** Chapter 7
**ELOs:** Perform verification measurements per Chapter 7; confirm acceptance criteria; complete log
**Estimated time:** 30 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-ANT-070. Read **Chapter 7 — Verification** completely.

Then come back here.

---

## Chapter 7 Content

| Parameter | Requirement | Pass/Fail |
|---|---|---|
| SWR at resonance | < 2.0:1 across 1.8–7.3 MHz | ____ |
| Resonant frequency | Within ±1% of design | ____ |
| Feed impedance (R) | 35–55 Ω | ____ |
| Feed reactance (X) | <±10 Ω | ____ |
| Gain (NEC2) | 0 dBi (termination loss) | ____ |
| Efficiency | 50–65% (resistive termination loss) | ____ |

1. Confirm SWR meets specification on all design bands.
2. Verify resonance frequency within ±1% of design center.
3. Confirm radial bond resistance <0.1 Ω with ohmmeter from feedpoint braid to each radial tip.
4. Log: date, ground condition, radial count, SWR, R+jX at each band, transmitter output power used for test.

---

## Verification vs. Tuning — Distinction

**Tuning** (Chapter 6) was active: you were permitted to adjust until the criterion was met.

**Verification** (Chapter 7) is passive confirmation: no adjustments are permitted. You are measuring the finished, tuned antenna and confirming it meets specification. If verification fails, return to Chapter 6 for re-tuning, then repeat the full Chapter 7 verification.

---

## Acceptance Criterion

Criterion: **2.0:1 across 1**

Write the exact criterion from Chapter 7 here before starting:
_________________________________________________

---

## Verification Checklist

- [ ] Antenna tuned and stable (no adjustments since completing Chapter 6)
- [ ] NanoVNA or SWR bridge connected at the feedpoint
- [ ] SWR measured at design frequency — result recorded below
- [ ] Resonant frequency (minimum SWR point) recorded
- [ ] All criteria from Chapter 7 checked
- [ ] Pass/fail determined
- [ ] Verification log entry completed

---

## Verification Log Entry

- [ ] Date and time
- [ ] Antenna type and construction date
- [ ] Operating frequency
- [ ] Height and orientation
- [ ] Measured SWR and resonant frequency
- [ ] Pass/fail determination
- [ ] Operator name

Record your results:

| Check | Measured Value | Acceptance Criterion | Pass/Fail |
|-------|---------------|---------------------|-----------|
| SWR at design frequency | | | |
| Resonant frequency | | | |
| (additional checks from Ch. 7) | | | |

---

## Self-Check Questions

**SC5-1.** What is the SWR acceptance criterion from Chapter 7? State it exactly.

**SC5-2.** Verification shows SWR = 2.4:1 at the design frequency. What is the correct action?

**SC5-3.** List the required items in a verification log entry per Chapter 7.

**SC5-4.** What is the difference between the Chapter 6 tuning procedure and the Chapter 7 verification?

**SC5-5.** After passing Chapter 7 verification, what can you conclude about the antenna?

---

## Answer Key

**SC5-1.** See Chapter 7. The criterion is stated with a specific SWR value and conditions (frequency, height, orientation).

**SC5-2.** Verification failed. Do not make adjustments during verification. Return to Chapter 6, re-tune, then repeat the complete Chapter 7 verification.

**SC5-3.** See Chapter 7, final steps. Items typically include: date, antenna type, frequency, height/orientation, SWR measurement, pass/fail, operator name.

**SC5-4.** Tuning (Ch. 6) permits active adjustments. Verification (Ch. 7) permits none — it is independent confirmation of the tuned state.

**SC5-5.** The antenna meets the specified performance criteria at this time, in this location, at this height and orientation. It does not guarantee performance at a different site, height, or orientation without re-verification.

---

## Checkpoint

You are ready for the final assessment when you can:
- [ ] State the acceptance criterion from memory
- [ ] State the correct response to a failed verification check
- [ ] State the required log items from memory

**→ Proceed to [Final Assessment](final_assessment.md)**
