# Unit 3 — Assembly and Setup
**[TM-TOOL-010 — Open Handout](../../../antenna_tools/swr_meter/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-TOOL-010. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

4-1 HF Toroid Coupler Winding

1. Wind each BN-43-202 core with 20 turns #28 AWG Teflon wire. Mark
 start of winding (dot convention: current into dot = positive).
2. For the forward coupler: connect winding start (dot end) to
 termination R1 (51Ω); winding finish to forward detector D1 (1N5711
 cathode). The free R1 end connects to the center conductor bus.
3. For the reflected coupler: reverse winding connections — winding
 start connects to reflected detector D2; winding finish to R2 (51Ω)
 to center conductor bus. This reversal makes the circuit directional.
4. Thread the coax center conductor through both toroid cores (1 primary
 turn each) before mounting in the chassis.

4-2 Meter Driver

An LM324 quad op-amp provides two independent channels (FWD and REF)
with adjustable zero and gain. Configuration: non-inverting amplifier with
gain set by R_gain (start at gain = 10); output drives panel meter through
a 4.7 kΩ series resistor. Adjust R_zero (10-turn pot) for
zero reading at no RF input.

4-3 Calibration Circuits

A precision 100Ω trimmer pot in the FWD channel allows full-scale
calibration with a known power level. A separate trimmer in the REF channel
permits null balancing with a matched load (zero reflected power should give
zero meter deflection on the REF meter).

---

## Assembly Quality

Chapter 4 specifies 4 construction/assembly steps.

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
