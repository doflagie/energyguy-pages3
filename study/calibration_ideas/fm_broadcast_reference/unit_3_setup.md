# Unit 3 — Setup Procedures
**[TM-CAL-004 — Open Handout](../../Calibration_Ideas/fm_broadcast_reference/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute setup steps in correct sequence; verify readiness before calibration
**Estimated time:** 15 minutes

---

## Step 1: Read the TM

Open TM-CAL-004. Read **Chapter 4 — Assembly Procedures** completely.

Then come back here.

---

## Chapter 4 Content

1. Connect dipole or whip antenna to TinySA Ultra INPUT port.
2. Set TinySA Ultra to spectrum analyzer mode, center frequency 98 MHz (adjust to local strong station), span 10 MHz, RBW 3 kHz or narrower.
3. Identify the carrier peak of the target station. It will appear as the tallest narrowband spike within the station's 200 kHz channel.
4. Set TinySA to narrow span: center on carrier, span 500 kHz, RBW 1 kHz.
5. Engage TinySA frequency counter or cursor on carrier peak.
6. Record measured frequency.

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
