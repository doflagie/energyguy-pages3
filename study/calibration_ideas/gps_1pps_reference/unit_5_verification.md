# Unit 5 — Verification and Logging
**[TM-CAL-002 — Open Handout](../../Calibration_Ideas/gps_1pps_reference/tm.html)**
**TM Chapters:** Chapter 6, Chapter 7
**ELOs:** Apply adjustments; perform independent verification; produce calibration log
**Estimated time:** 45 minutes (hands-on lab)

---

## Step 1: Read the TM

Open TM-CAL-002. Read **Chapter 6** and **Chapter 7** completely.

Then come back here.

---

## Chapter 6 — Tuning and Adjustment

### 6-1. GPS RECEIVER CONFIGURATION

The u-blox NEO-M8N timing mode can be enabled via UBX configuration commands to reduce 1PPS jitter from ~30 ns to ~20 ns. This requires UART or USB connection to the module and u-center software or custom serial commands.

> **NOTE:** For most amateur calibration work, the default timing mode is entirely adequate. Enabling timing mode is optional.

### 6-2. PULSE WIDTH ADJUSTMENT

Default 1PPS pulse width is 100 ms. For triggering older counters or oscilloscopes with high trigger thresholds, a one-shot circuit can reshape the pulse width. The pulse *timing* (rising edge position) is not affected by pulse width adjustments.

---

## Chapter 7 — Verification

1. Confirm GPS lock: check lock indicator LED (1 Hz blink on most modules) or parse NMEA GPRMC sentence for validity character 'A' (active).
2. Measure 1PPS frequency with calibrated frequency counter. Should read 1.000000 Hz ±0.000001 Hz (1 μHz).
3. Compare 1PPS timing to WWV second tick (audio): within 0.5 s over 10 s is normal for ionospheric delay; phase difference should be constant.
4. Log: date, time, GPS module model, satellite count, 1PPS frequency measured, ambient temperature.

---

## Calibration vs. Verification

These are two different things:

**Calibration (Chapter 5):** You apply reference inputs and record what the instrument reads. You may adjust the instrument during this process. The result is a set of measurements taken while you are actively tuning.

**Verification (Chapter 7):** After all adjustments are complete, you perform an independent confirmation measurement. You do not adjust during verification — you are testing whether the calibration held.

The TM states a verification criterion of: **within 0**. Confirm the exact wording in Chapter 7.

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
