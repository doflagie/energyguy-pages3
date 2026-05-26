# Unit 3 — Setup Procedures
**[TM-CAL-005 — Open Handout](../../Calibration_Ideas/ac_mains_reference/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute setup steps in correct sequence; verify readiness before calibration
**Estimated time:** 15 minutes

---

## Step 1: Read the TM

Open TM-CAL-005. Read **Chapter 4 — Assembly Procedures** completely.

Then come back here.

---

## Chapter 4 Content

> **WARNING:** De-energize mains before making any connections to transformer primary.

1. Mount transformer and voltage divider in plastic enclosure.
2. Connect transformer primary to IEC inlet or hardwired mains cable with strain relief. Fuse the primary at 0.5 A.
3. Connect 10kΩ/1kΩ divider across secondary. Center tap of divider to BNC ground.
4. Connect junction of divider (1kΩ side) to BNC center conductor.
5. Mount BNC panel connector on enclosure.
6. Close enclosure. Apply label: 'AC MAINS 60 Hz REFERENCE — SECONDARY ONLY'.
7. Apply power. Measure BNC output with oscilloscope: should show clean 60 Hz sinusoid at approximately 0.6V RMS (6.3V secondary × 1k/(10k+1k)).
8. Verify no DC offset (<50 mV DC on meter).

---

## Setup is Part of Calibration

Chapter 4 specifies 8 setup steps.

A calibration is only valid if the setup is correct. Common setup errors that invalidate results:
- Skipping warm-up time
- Wrong input range or measurement mode selected
- Reference connected to wrong terminal
- Ground loops or interference from adjacent equipment

If Chapter 4 specifies a verification at the end of setup (e.g., "confirm instrument reads X before proceeding"), do it. That check exists because someone discovered a failure mode that made it necessary.

---

## Self-Check Questions

**SC3-1.** How many steps does Chapter 4 specify for setup?

**SC3-2.** What is the first step? State it exactly from the TM.

**SC3-3.** Does Chapter 4 require any verification before proceeding to calibration? If so, state it.

**SC3-4.** What is the last step of Chapter 4 (immediately before you begin the Chapter 5 calibration)?

**SC3-5.** What would you do if a setup step produced an unexpected result?

---

## Answer Key

**SC3-1.** Count the numbered steps in Chapter 4. The answer is in the TM.

**SC3-2.** See Chapter 4, step 1. Copy it exactly.

**SC3-3.** Scan Chapter 4 for steps containing "verify," "check," "confirm," or "measure." If present, they are hold points.

**SC3-4.** See the last numbered step in Chapter 4.

**SC3-5.** Stop. Do not proceed to calibration. Diagnose the unexpected result — it may indicate a setup error, a faulty reference, or a failed instrument under test.

---

## Checkpoint

Before proceeding:
- [ ] You have read Chapter 4 completely
- [ ] You can state the number of setup steps
- [ ] You understand setup errors invalidate calibration results

**→ Proceed to [Unit 4](unit_4_calibration.md)**
