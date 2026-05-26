# Unit 5 — Calibration and Verification
**[TM-TOOL-002 — Open Handout](../../antenna_tools/antenna-analyzer/tm.html)**
**TM Chapters:** Chapter 6, Chapter 7
**ELOs:** Calibrate the ANTENNA ANALYZER — VECTOR IMPEDANCE MEASUREMENT; verify performance against acceptance criteria; complete calibration log
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-TOOL-002. Read **Chapter 6 — Calibration** and **Chapter 7 — Verification and Acceptance** completely.

Then come back here.

---

## Chapter 6 — Calibration Content

6-1 SOLT Calibration

Before measuring an antenna, perform a one-port SOLT calibration at the
DUT SMA connector (not at the instrument chassis):

1. Connect SHORT (shorted SMA cap). Press CAL → SHORT.
2. Connect OPEN (SMA cap with no connection). Press CAL → OPEN.
3. Connect 50Ω LOAD (SMA terminator). Press CAL → LOAD.
4. Press CAL → DONE. Calibration plane is now at DUT connector.

After SOLT cal, short should read SWR >50:1, open should read SWR >50:1,
and 50Ω load should read SWR <1.05:1 across the calibrated band.

---

## Chapter 7 — Verification Content

1. Connect a precision 50Ω load (Pasternack PE6010 or calibrated terminator).
 Verify SWR <1.05:1 at all calibrated frequencies.
2. Connect a known 100Ω resistor (1%, non-inductive). Verify R reads
 100 ± 5Ω, X reads 0 ± 5Ω at 1 MHz.
3. Connect a 50Ω + 50 nH load (resistor in series with known inductor).
 Verify jX reads within ±10% of calculated inductive reactance at 10 MHz.
4. If any check fails, repeat SOLT calibration. Persistent errors indicate
 coupler asymmetry or AD8302 bias fault.
5. Log: date, calibration kit used, frequency range, short SWR, open SWR,
 load SWR, 100Ω R/X readings.

---

## Calibration vs. Verification — Distinction

**Calibration** (Chapter 6): active adjustments to bring the tool within specification.
You are permitted to adjust, tweak, and repeat until the criterion is met.

**Verification** (Chapter 7): independent confirmation that the calibration succeeded.
No adjustments are permitted during verification. If a verification check fails, you
return to Chapter 6, not to Chapter 7.

---

## Acceptance Criterion

A criterion found in Chapter 6: **50Ω**. Confirm the exact criterion in the TM.

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
