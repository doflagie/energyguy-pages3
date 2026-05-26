# Unit 5 — Calibration and Verification
**[TM-TOOL-003 — Open Handout](../../../antenna_tools/antenna_modeling/tm.html)**
**TM Chapters:** Chapter 6, Chapter 7
**ELOs:** Calibrate the NEC ANTENNA MODELING SYSTEM; verify performance against acceptance criteria; complete calibration log
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-TOOL-003. Read **Chapter 6 — Calibration** and **Chapter 7 — Verification and Acceptance** completely.

Then come back here.

---

## Chapter 6 — Calibration Content

Three optimizers are available:

- **genetic_optimizer.py:** Genetic algorithm; best for
 multi-parameter problems with discontinuous cost functions. Suitable
 for Yagi element lengths + spacings.
- **pso_optimizer.py:** Particle swarm; faster convergence
 for smooth continuous cost surfaces. Suitable for matching network
 component values.
- **batch_optimizer.py / batch_multiband.py:** Evaluates a
 parameter grid; useful for initial design-space exploration before running
 a stochastic optimizer.

Cost function examples: maximize F/B ratio, minimize SWR at target frequency,
maximize gain over ground at 20° elevation.

---

## Chapter 7 — Verification Content

1. Run the included test case (half-wave dipole in free space at 14.25 MHz).
 Expected: feedpoint impedance 73+j42.5Ω, gain 2.15 dBi.
2. Verify against ARRL Antenna Book reference tables for dipole impedance vs.
 height above ground. Deviations >5% indicate a segmentation or ground
 parameter error.
3. For a calibration-quality check: compare modeled resonant frequency against
 NanoVNA measurement of a physical antenna. Agreement within 2% is typical
 for wire antennas over flat ground.
4. Log: NEC version, Python version, test antenna type, modeled vs. reference
 gain, modeled vs. measured resonant frequency.

---

## Calibration vs. Verification — Distinction

**Calibration** (Chapter 6): active adjustments to bring the tool within specification.
You are permitted to adjust, tweak, and repeat until the criterion is met.

**Verification** (Chapter 7): independent confirmation that the calibration succeeded.
No adjustments are permitted during verification. If a verification check fails, you
return to Chapter 6, not to Chapter 7.

---

## Acceptance Criterion

Locate the acceptance criterion in Chapter 6 or Chapter 7.

Write the exact criterion from the TM here before you start the lab:
_________________________________________________

---

## Lab Checklist

Work through each item in sequence. Do not skip steps.

- [ ] All required equipment gathered and ready
- [ ] Equipment warmed up per TM requirements
- [ ] All Chapter 4 setup steps verified complete
- [ ] Chapter 6 calibration performed (all steps)
- [ ] Calibration criterion met (recorded below)
- [ ] Chapter 7 verification performed (all steps, no adjustments)
- [ ] All verification criteria passed
- [ ] Calibration log entry completed

---

## Calibration Log Entry

- [ ] Date
- [ ] Equipment used
- [ ] Measurements taken
- [ ] Results vs. criteria
- [ ] Operator name

Record your results:

| Item | Measured Value | Acceptance Criterion | Pass/Fail |
|------|---------------|---------------------|-----------|
| (from Ch 7) | | | |
| (from Ch 7) | | | |
| (from Ch 7) | | | |

---

## Self-Check Questions

**SC5-1.** What is the acceptance criterion for this calibration (from Chapter 7)?

**SC5-2.** During verification, one reading fails. What is the correct action per the TM?

**SC5-3.** List the items required in a calibration log entry per Chapter 7.

**SC5-4.** What is the difference between calibration and verification in this context?

**SC5-5.** If the tool passes all Chapter 7 checks, what does that tell you about its current state?

---

## Answer Key

**SC5-1.** See Chapter 7. The criterion is stated as a specific value with units.

**SC5-2.** Stop verification. Return to Chapter 6 and re-calibrate. Diagnose the cause before re-adjusting.

**SC5-3.** See Chapter 7, last numbered step. The log items are listed there.

**SC5-4.** Calibration allows adjustments; verification does not. Verification is independent confirmation.

**SC5-5.** It tells you the tool is performing within its specified accuracy at this time. It does not tell you how long it will remain in calibration — periodic re-calibration is required.

---

## Checkpoint

You are ready for the final assessment when you can:
- [ ] State the acceptance criterion from memory
- [ ] State the difference between calibration and verification
- [ ] State the required log items from memory

**→ Proceed to [Final Assessment](final_assessment.md)**
