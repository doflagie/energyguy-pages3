# Unit 3 — Setup Procedures
**[TM-CAL-002 — Open Handout](../../Calibration_Ideas/gps_1pps_reference/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute setup steps in correct sequence; verify readiness before calibration
**Estimated time:** 15 minutes

---

## Step 1: Read the TM

Open TM-CAL-002. Read **Chapter 4 — Assembly Procedures** completely.

Then come back here.

---

## Chapter 4 Content

1. Solder or connect the GPS module to 5 V power supply. Observe polarity. Module draws approximately 30–50 mA during acquisition.
2. Connect GPS patch antenna to module antenna port (IPEX or SMA connector).
3. Identify the 1PPS output pin. On NEO-M8N breakout boards, this is labeled TIMEPULSE or 1PPS. On bare modules, pin 3 (TIMEPULSE).
4. Connect a 100 Ω series resistor in-line with the 1PPS output to protect the module output stage from capacitive loading.
5. Connect 1PPS output to BNC connector on enclosure. Use short lead lengths (<10 cm) to minimize EMI pickup.
6. Place module in enclosure with antenna cable routed to exterior.
7. Apply power. Confirm LED blink on most breakout boards; 1 Hz blink indicates lock. Acquisition takes 60–120 seconds outdoors.
8. Verify 1PPS output with oscilloscope: 3.3 V (or 5 V) positive pulse, 100 ms wide, 1.000000 Hz repetition rate.

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
