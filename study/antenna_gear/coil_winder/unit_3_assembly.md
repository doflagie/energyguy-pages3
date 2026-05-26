# Unit 3 — Assembly and Setup
**[TM-GEAR-006 — Open Handout](../../antenna_gear/coil_winder/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-006. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

1. Mount spindle motor to the main frame with its axis aligned with the
 former mandrel. Use a flexible coupler to compensate for minor
 shaft misalignment.
2. Mount traverse motor on a lead-screw linear stage. Lead screw pitch:
 2 mm per revolution (M8 or TR8×2). Mount wire guide on the
 linear stage carriage.
3. Wire stepper drivers: VMOT to 24V rail, GND, MS1/MS2/MS3 for
 microstepping (TMC2208 SPI-configured; A4988 uses jumpers for
 1/16-step mode). STEP/DIR to ESP32 GPIO.
4. Wire limit switches to ESP32 GPIO with 10 kΩ pull-ups.
 Connect HOME switch to traverse axis minus limit; END switch to plus limit.
5. SD card: SPI bus (SCK, MOSI, MISO, CS). SD CS = GPIO5; SD SPI shares
 bus with display if display is SPI type.

---

## Assembly Quality

Chapter 4 specifies 5 construction/assembly steps.

The assembly directly determines RF performance. Common errors:
- RF leads too long — lead inductance raises SWR and limits high-frequency performance
- Cold solder joints on RF nodes — high resistance causes signal loss and intermittent behavior
- Ground loops — multiple ground paths at different potentials cause noise and calibration errors
- Ferrite winding errors — wrong turn count or direction reverses transformer polarity or changes impedance ratio
- Incorrect winding direction on toroidal transformers — affects phase and common-mode rejection

If Chapter 4 specifies a verification step after assembly (e.g., "verify DC resistance = X before proceeding"), do it. Those checks exist because they are the most common failure points.

---

## Self-Check Questions

**SC3-1.** How many assembly steps does Chapter 4 specify?

**SC3-2.** What is the first assembly step? State it exactly from the TM.

**SC3-3.** Does Chapter 4 specify maximum lead length anywhere? If so, what is the limit and why?

**SC3-4.** Does Chapter 4 require a bench verification after assembly? What does it check?

**SC3-5.** What would you do if a winding resistance measurement came out wrong during assembly verification?

---

## Answer Key

**SC3-1.** Count the numbered steps in Chapter 4.

**SC3-2.** See Chapter 4, step 1. Copy it exactly.

**SC3-3.** RF lead length limits are typically 10–15 mm for HF circuits. Longer leads add ~1–2 nH per mm, raising inductive reactance at high frequencies.

**SC3-4.** Scan Chapter 4 for verification steps. Common checks: DC resistance, winding balance, null depth on test signal, impedance ratio.

**SC3-5.** Stop assembly. Diagnose before proceeding — a winding error found before completion is much easier to fix than one discovered after the unit is boxed.

---

## Checkpoint

Before proceeding:
- [ ] You have read Chapter 4 completely
- [ ] You can state the number of assembly steps and the first and last steps
- [ ] You understand how assembly quality affects RF performance

**→ Proceed to [Unit 4](unit_4_operation.md)**
