# Unit 3 — Setup Procedures
**[TM-CAL-006 — Open Handout](../../../Calibration_Ideas/precision_voltage_reference/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute setup steps in correct sequence; verify readiness before calibration
**Estimated time:** 15 minutes

---

## Step 1: Read the TM

Open TM-CAL-006. Read **Chapter 4 — Assembly Procedures** completely.

Then come back here.

---

## Chapter 4 Content

1. Pre-regulate supply to 9 V using LM7809 with 100 nF input/output bypass capacitors.
2. For LT1021 (series type): connect IN pin to 9 V through 1kΩ resistor. Connect OUT to output terminal. Connect GND to ground. Add 100 nF on output.
3. For LM4040 (shunt type): connect through 1kΩ from supply to LM4040 anode. Output is taken across the LM4040. Load current must be <15 mA.
4. Solder all components on perfboard with short, tidy leads. Shield board from air currents if TC performance is critical.
5. Mount in small enclosure. Label output: '5.000 V REF ±0.1%'.
6. Allow 15 minutes warm-up before first measurement.

---

## Setup is Part of Calibration

Chapter 4 specifies 6 setup steps.

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
