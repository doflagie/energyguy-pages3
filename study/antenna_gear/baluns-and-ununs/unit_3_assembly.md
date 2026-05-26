# Unit 3 — Assembly and Setup
**[TM-GEAR-003 — Open Handout](../../antenna_gear/baluns-and-ununs/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-003. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

4-1 1:1 Choke Balun (FT-240-31)

1. Thread RG-303 (or RG-58) coaxial cable through the toroid 12 times. Keep
 turns close-wound; secure with cable ties after every 3 turns.
2. Connect the input end to an SO-239 or N-type connector (coax center to
 center pin, shield to shell). Same for the output end.
3. Measure choking impedance with NanoVNA: connect one port to the coax
 center, other port to the coax shield, and sweep 1.8–30 MHz.
 The |Z| should exceed 2000 Ω across most of the HF band.
 Core type #31 is optimized for 1.8–30 MHz.

4-2 4:1 Guanella Balun

1. Wind two identical 8-turn bifilar windings on two separate FT-140-43 cores.
 Use color-coded wire to track polarity (dot convention).
2. Connect the two windings in series for the balanced (200Ω) port and
 in parallel for the unbalanced (50Ω) port. Polarity is critical:
 the start of winding 1 connects to the finish of winding 2 at the balanced
 port center tap.
3. Test: connect a 200Ω resistor to the balanced port; measure 50Ω
 at the unbalanced port (SWR 4-3 9:1 Un-Un (End-Fed Wire)

1. Wind 9 turns of trifilar #16 AWG on an FT-240-43 core. Three wires wound
 simultaneously, in the same direction.
2. Connect winding A in series with winding B; the junction is the 50Ω
 (unbalanced input) center tap. Winding C is the 450Ω output
 winding.
3. Test: connect 450Ω resistor to high-impedance port; measure
 50Ω SWR at the coax port (<1.2:1 across 1.8–30 MHz).

---

## Assembly Quality

Chapter 4 specifies 9 construction/assembly steps.

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
