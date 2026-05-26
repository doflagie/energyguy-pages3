# Unit 5 — Calibration and Verification
**[TM-TOOL-009 — Open Handout](../../antenna_tools/sdr_interfaces/tm.html)**
**TM Chapters:** Chapter 6, Chapter 7
**ELOs:** Calibrate the SDR INTERFACES AND PRESELECTOR; verify performance against acceptance criteria; complete calibration log
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-TOOL-009. Read **Chapter 6 — Calibration** and **Chapter 7 — Verification and Acceptance** completely.

Then come back here.

---

## Chapter 6 — Calibration Content

1. Inject a known signal (&minus;50 dBm at 14.175 MHz) into the
 upconverter input. Verify the SDR displays the signal at the correct
 displayed frequency (offset by LO).
2. Preselector: verify each filter passes its intended band within 1 dB
 of bypass, and rejects adjacent bands by ≥40 dB.
3. Attenuator: apply 0 dBm CW; step through 0, 10, 20, 30 dB
 settings. Verify SDR S-meter drops by 10 dB per step
 (±1.5 dB).

---

## Chapter 7 — Verification Content

1. WWV reception check: with upconverter active and 40m preselector
 selected, verify WWV at 5.000 MHz or 10.000 MHz is audible
 at noise floor SNR ≥15 dB.
2. FM rejection: verify FM broadcast stations at 88–108 MHz are
 not visible while preselector is set to any HF band (≥60 dB
 suppression relative to bypass mode).
3. Log: date, upconverter LO error (measured vs. GPS reference), preselector
 insertion loss at band centers, attenuator step accuracy, operator.

---

## Calibration vs. Verification — Distinction

**Calibration** (Chapter 6): active adjustments to bring the tool within specification.
You are permitted to adjust, tweak, and repeat until the criterion is met.

**Verification** (Chapter 7): independent confirmation that the calibration succeeded.
No adjustments are permitted during verification. If a verification check fails, you
return to Chapter 6, not to Chapter 7.

---

## Acceptance Criterion

A criterion found in Chapter 6: **50 dB**. Confirm the exact criterion in the TM.

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
