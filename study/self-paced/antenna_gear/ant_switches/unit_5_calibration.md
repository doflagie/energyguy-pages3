# Unit 5 — Calibration and Verification
**[TM-GEAR-001 — Open Handout](../../../antenna_gear/ant_switches/tm.html)**
**TM Chapters:** Chapter 6, Chapter 7
**ELOs:** Calibrate the ANTENNA SWITCHES — MANUAL, MOTORIZED, AND AUTO-BAND; verify performance against acceptance criteria; complete calibration log
**Estimated time:** 45 minutes (actual lab work required)

---

## Step 1: Read the TM

Open TM-GEAR-001. Read **Chapter 6 — Calibration** and **Chapter 7 — Verification and Acceptance** completely.

Then come back here.

---

## Chapter 6 — Calibration Content

1. Measure insertion loss with NanoVNA: calibrate SOLT at radio connector,
 then measure S21 to each antenna port in turn. Loss must be <0.1 dB
 at 1.8–30 MHz; <0.3 dB at 144 MHz.
2. Measure isolation: with radio port and Port 1 connected to NanoVNA,
 terminate Port 2 through Port N in 50Ω loads. S21 to each
 terminated port must be <&minus;50 dB at HF.
3. Verify auto-band sequencing: step through bands on the transceiver.
 Log which relay energizes at each band; confirm against the band map table
 in firmware.

---

## Chapter 7 — Verification Content

1. Insertion loss <0.1 dB at 7 MHz and 14 MHz on all ports.
2. Isolation ≥50 dB at 14 MHz between any two ports.
3. SWR at selected port <1.05:1 at all test frequencies (1.8, 7, 14, 28, 50 MHz)
 with a calibrated 50Ω load on the antenna port.
4. Auto-band: correct port selected within 100 ms of band change on all
 15 configured bands.
5. Log: date, ports tested, insertion loss at key bands, isolation, SWR, operator.

---

## Calibration vs. Verification — Distinction

**Calibration** (Chapter 6): active adjustments to bring the component within specification.
You are permitted to adjust, tweak, and repeat until the criterion is met.

**Verification** (Chapter 7): independent confirmation that the calibration succeeded.
No adjustments are permitted during verification. If a verification check fails, you
return to Chapter 6, not to Chapter 7.

---

## Acceptance Criterion

A criterion found in Chapter 6: **0.1 dB**. Confirm the exact criterion in the TM.

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

**SC5-5.** If the component passes all Chapter 7 checks, what does that tell you about its current state?

---

## Answer Key

**SC5-1.** See Chapter 7. The criterion is stated as a specific value with units.

**SC5-2.** Stop verification. Return to Chapter 6 and re-calibrate. Diagnose the cause before re-adjusting.

**SC5-3.** See Chapter 7, last numbered step. The log items are listed there.

**SC5-4.** Calibration allows adjustments; verification does not. Verification is independent confirmation.

**SC5-5.** It tells you the component is performing within its specified parameters at this time. It does not tell you how long it will remain in calibration — periodic re-verification is required.

---

## Checkpoint

You are ready for the final assessment when you can:
- [ ] State the acceptance criterion from memory
- [ ] State the difference between calibration and verification
- [ ] State the required log items from memory

**→ Proceed to [Final Assessment](final_assessment.md)**
