# Unit 3 — Assembly and Setup
**[TM-GEAR-015 — Open Handout](../../../antenna_gear/rf_couplers/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-015. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

4-1 Bruene Coupler Winding

1. Wind 20 turns of #28 AWG Teflon wire on a BN-43-202 core. This is the
 voltage sampling secondary; the center conductor of the transmission line
 threads through the core once as the 1-turn primary.
2. Connect resistors R1 and R2 (51Ω 1%) from each end of the secondary
 to the common center conductor (ground bus). The midpoints of R1 and R2
 are the detector nodes (VFWD and VREF before the diodes).
3. Mount the detector diodes (1N5711 cathode toward the detector node)
 and filter capacitors on a small PCB within 20 mm of the toroid.
4. Thread the SO-239 center conductor through the toroid. Keep the center
 conductor straight through the toroid bore (not coiled) to maintain
 transmission line continuity.

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
