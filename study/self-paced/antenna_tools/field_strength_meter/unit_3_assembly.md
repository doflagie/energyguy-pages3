# Unit 3 — Assembly and Setup
**[TM-TOOL-005 — Open Handout](../../../antenna_tools/field_strength_meter/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-TOOL-005. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

4-1 Passive FSM

1. Mount 1N34A diode cathode toward meter positive terminal. R_load = 100 kΩ;
 C_filter = 10&micro;F 16V electrolytic. Meter series resistance sets FSD.
2. Connect probe antenna (50 cm rigid copper rod) via BNC to diode anode.
3. Mount in a small plastic enclosure. Keep RF lead from BNC to diode
 4-2 AD8307 Digital FSM

1. Mount AD8307 with 100 nF NP0 decoupling on each supply pin. INHI
 connects via 1 nF DC-blocking cap to SMA input; INLO to ground.
2. VOUT pin (25 mV/dB slope, intercept &minus;84 dBm) connects to
 ESP32 ADC GPIO (12-bit, 3.3V range). Scale: 3300 mV / 25 mV/dB =
 132 dB dynamic range from ADC alone, but ADC noise floor limits
 practical range to ~80 dB.
3. Add 10 kΩ + 100 nF low-pass filter between VOUT and ESP32
 ADC to reject RF on the DC output line.

---

## Assembly Quality

Chapter 4 specifies 6 construction/assembly steps.

The assembly directly determines measurement quality. Common errors:
- RF leads too long — lead inductance raises SWR and limits high-frequency performance
- Cold solder joints on RF nodes — high resistance causes signal loss and intermittent readings
- Ground loops — multiple ground paths at different potentials cause noise and calibration errors
- Ferrite winding errors — wrong turn count or direction reverses transformer polarity

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

**SC3-4.** Scan Chapter 4 for verification steps. Common checks: DC resistance, winding balance, null depth on test signal.

**SC3-5.** Stop assembly. Diagnose before proceeding — a winding error found before completion is much easier to fix than one discovered after the unit is boxed.

---

## Checkpoint

Before proceeding:
- [ ] You have read Chapter 4 completely
- [ ] You can state the number of assembly steps and the first and last steps
- [ ] You understand how assembly quality affects measurement accuracy

**→ Proceed to [Unit 4](unit_4_operation.md)**
