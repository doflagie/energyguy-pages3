# Unit 5 — Verification and Logging
**[TM-CAL-017 — Open Handout](../../Calibration_Ideas/field_strength_reference/tm.html)**
**TM Chapters:** Chapter 6, Chapter 7
**ELOs:** Apply adjustments; perform independent verification; produce calibration log
**Estimated time:** 45 minutes (hands-on lab)

---

## Step 1: Read the TM

Open TM-CAL-017. Read **Chapter 6** and **Chapter 7** completely.

Then come back here.

---

## Chapter 6 — Tuning and Adjustment

> **NOTE:** Field strength tests are sensitive to ground reflections (multipath). Conduct tests with antennas ≥1.5 m above ground for reduced ground bounce. Early morning or late evening provides reduced atmospheric refraction effects at VHF and above.

---

## Chapter 7 — Verification

1. Verify inverse-square-law (1/d): doubling distance should reduce E by 6 dB.
2. Measure at d = 3 m and d = 6 m: received signal should decrease by 6.0 ±1.0 dB.
3. Deviation >2 dB indicates near-field effects, ground reflections, or antenna orientation error.
4. Log: frequency, TX power, TX antenna type, distance, measured E, expected E.

---

## Calibration vs. Verification

These are two different things:

**Calibration (Chapter 5):** You apply reference inputs and record what the instrument reads. You may adjust the instrument during this process. The result is a set of measurements taken while you are actively tuning.

**Verification (Chapter 7):** After all adjustments are complete, you perform an independent confirmation measurement. You do not adjust during verification — you are testing whether the calibration held.

Locate the verification criterion in Chapter 7.

If verification fails, you do not re-adjust and re-verify in one step. You return to Chapter 5, re-calibrate, and then re-verify from scratch.

---

## Lab Checklist

Work through this with equipment in hand. Check each box when complete.

- [ ] Chapter 4 setup complete
- [ ] Chapter 5 calibration procedure complete
- [ ] Any adjustments applied per Chapter 6
- [ ] Chapter 7 verification measurement performed
- [ ] Verification criterion met
- [ ] Calibration log entry completed:

- [ ] Date
- [ ] Instrument type and model
- [ ] TM number and procedure
- [ ] Reference standard used
- [ ] Measured values
- [ ] Errors (% or ppm)
- [ ] Pass/Fail result
- [ ] Operator name

---

## Self-Check Questions

**SC5-1.** What is the verification acceptance criterion per Chapter 7? State it with units.

**SC5-2.** What is the difference between the Chapter 5 calibration and the Chapter 7 verification?

**SC5-3.** Your Chapter 7 verification fails. What is the correct next step per the TM?

**SC5-4.** List the required entries in the calibration log.

**SC5-5.** Why must the log be completed before the calibration is considered finished?

---

## Answer Key

**SC5-1.** See TM Chapter 7. Copy the criterion exactly with units and section number.

**SC5-2.** Chapter 5 is the active calibration — you apply references and may adjust. Chapter 7 is an independent confirmation after all adjustments are locked in — no further adjustment during verification.

**SC5-3.** Return to Chapter 5 and re-calibrate. Do not re-adjust and re-verify without re-doing the full calibration procedure.

**SC5-4.** See TM Chapter 7. List every item the TM specifies.

**SC5-5.** A calibration with no record did not happen in any traceable sense. The log is the evidence that the procedure was followed, the results were within specification, and who performed it.

---

## Checkpoint

TLO-1 is complete when:
- [ ] Verification criterion met (Chapter 7)
- [ ] Calibration log entry completed with all required fields
- [ ] You can explain the difference between calibration and verification

**→ Proceed to [Final Assessment](final_assessment.md)**
