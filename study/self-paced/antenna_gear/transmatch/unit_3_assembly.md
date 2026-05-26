# Unit 3 — Assembly and Setup
**[TM-GEAR-018 — Open Handout](../../../antenna_gear/transmatch/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-018. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

1. Mount variable capacitors C1 and C2 on the front panel with lockwashers.
 Keep capacitor leads to the inductor and SWR bridge as short as possible
 (<50 mm) to minimize stray inductance and radiation.
2. Mount the roller inductor vertically on the center of the main board.
 The inductor lead to C1 and C2 should be heavy-gauge solid wire (#12 AWG
 or larger) capable of 100W continuous RF current.
3. Wire the balun output option: a 4:1 Guanella balun (see TM-GEAR-003)
 mounted on the rear panel provides a balanced output for dipoles and
 open-wire feedlines.
4. Install the SWR bridge (from TM-GEAR-015) inline between the radio
 input SO-239 and the T-network C1. Connect VFWD to forward LED driver;
 VREF to reflected LED driver.

---

## Assembly Quality

Chapter 4 specifies 4 construction/assembly steps.

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
