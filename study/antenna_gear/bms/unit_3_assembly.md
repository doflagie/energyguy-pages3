# Unit 3 — Assembly and Setup
**[TM-GEAR-004 — Open Handout](../../antenna_gear/bms/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-004. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

1. Connect cells in series (4S): Cell 1 (-) to pack negative; Cell 4 (+) to pack positive
 through the BMS FETs. Use copper bus bars (≥8 AWG equivalent) for all
 cell interconnects.
2. Solder the BQ76920 sense wires to each cell junction: VC0 to pack (&minus;),
 VC1 to Cell 1/2 junction, ..., VC4 to pack (+). Keep sense leads <100 mm;
 twist in pairs; add 1kΩ series resistors to protect the BQ76920 inputs.
3. Install CHG and DSG MOSFETs on a common heatsink (thermal resistance
 ≤2°C/W for 50 A continuous). Connect gate drives from BQ76920
 CHG and DSG outputs via 100Ω gate resistors.
4. Install the current sense resistor (5 mΩ) in series with the
 discharge path negative bus. Connect BQ76920 SRP and SRN to each side.
5. Connect ESP32 I2C (SCL, SDA, 4.7 kΩ pull-ups to 3.3V) to
 BQ76920 SCL/SDA pins.

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
