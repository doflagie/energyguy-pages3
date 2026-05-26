# Unit 3 — Setup Procedures
**[TM-CAL-003 — Open Handout](../../Calibration_Ideas/gpsdo_reference/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute setup steps in correct sequence; verify readiness before calibration
**Estimated time:** 15 minutes

---

## Step 1: Read the TM

Open TM-CAL-003. Read **Chapter 4 — Assembly Procedures** completely.

Then come back here.

---

## Chapter 4 Content

1. Verify all components with multimeter before assembly.
2. Install ESP32 board on DIN rail or in enclosure, securing with 4 M3 screws.
3. Mount Si5351 breakout board adjacent to ESP32. Keep I2C traces short (<5 cm).
4. Mount NEO-M8N module. Route UART and TIMEPULSE connections to ESP32.
5. Install SMA connector on enclosure panel. Solder short RG-174 pigtail from Si5351 CLK0 output to SMA center pin. Minimize trace length.
6. Install GPS antenna SMA bulkhead connector on enclosure lid or side panel.
7. Wire 5 V supply to all modules. Verify +5 V on each module before applying full power.
8. Load ESP32 firmware (simple PLL algorithm: measure 1PPS → count Si5351 cycles → adjust frequency word in PI loop).
9. Apply power. Observe serial debug output: GPS acquiring, then position fix, then TIMEPULSE valid, then PLL locking.
10. Allow 15 minutes for full phase lock. Output frequency should stabilize to <1 Hz of 10.000000 MHz.

---

## Setup is Part of Calibration

Chapter 4 specifies 10 setup steps.

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
