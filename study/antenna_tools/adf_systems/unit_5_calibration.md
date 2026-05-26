# Unit 5 — Calibration and Verification
**[TM-TOOL-001 — Open Handout](../../antenna_tools/adf_systems/tm.html)**
**TM Chapters:** Chapter 6, Chapter 7
**ELOs:** Calibrate the ADF SYSTEMS — ANTENNA DIRECTION FINDING; verify performance against acceptance criteria; complete calibration log
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-TOOL-001. Read **Chapter 6 — Calibration** and **Chapter 7 — Verification and Acceptance** completely.

Then come back here.

---

## Chapter 6 — Calibration Content

6-1 Watson-Watt Channel Balance

1. Inject equal-amplitude, in-phase signal into both channels simultaneously
 from a common splitter. Verify RSSI within 0.3 dB.
2. If unbalanced: add fixed attenuator pad (1–3 dB) to the stronger
 channel at the receiver input.
3. Inject signal into NS channel only. Verify 90° + known bearing reads
 correctly. Repeat for EW channel.

6-2 Butler Matrix Beam Verification

1. Place a known CW signal at each of the four expected beam-peak azimuths
 in turn (0°, 90°, 180°, 270°).
2. Verify that the corresponding beam port shows maximum RSSI and adjacent
 ports show ≥3 dB lower level.
3. Record beam-center azimuths. Apply offset table in software if beam
 centers deviate more than 5° from design.

---

## Chapter 7 — Verification Content

After calibration, conduct a bearing verification test using a known-location
transmitter:

1. Position a low-power test transmitter at a measured azimuth from the
 array (use GPS or compass, to within ±1°).
2. Take 20 bearing readings; compute mean and standard deviation.
3. Acceptance criterion: mean error <5°, standard deviation <3°.
4. If failed: re-check element spacing, coax phase lengths, and channel balance.
5. Record results in calibration log: date, frequency, test azimuth, mean
 bearing, standard deviation, operator.

---

## Calibration vs. Verification — Distinction

**Calibration** (Chapter 6): active adjustments to bring the tool within specification.
You are permitted to adjust, tweak, and repeat until the criterion is met.

**Verification** (Chapter 7): independent confirmation that the calibration succeeded.
No adjustments are permitted during verification. If a verification check fails, you
return to Chapter 6, not to Chapter 7.

---

## Acceptance Criterion

A criterion found in Chapter 6: **0.3 dB**. Confirm the exact criterion in the TM.

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
