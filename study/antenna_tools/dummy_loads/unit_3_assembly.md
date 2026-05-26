# Unit 3 — Assembly and Setup
**[TM-TOOL-004 — Open Handout](../../antenna_tools/dummy_loads/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-TOOL-004. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

4-1 100W Dry Load

1. Mount SO-239 / N connector on heatsink. Center conductor passes through
 to an M4 brass stud on the heatsink surface.
2. Cut resistor leads to ≤10 mm each side. Solder all high-side leads
 to center stud. Solder all low-side leads to ground bus on heatsink.
3. Orient resistors flat against heatsink surface for maximum thermal contact.
 Apply thermal compound between each resistor body and heatsink.
4. Verify DC resistance: 49–51Ω between connector center and shell.

4-2 500W / 1 kW Oil Load

1. Drill and mount N connector through lid of can. Extend center pin via
 threaded brass rod to center of can interior.
2. Wire resistors star-fashion around the center rod. Ground bus: bare
 copper wire ring soldered to can walls.
3. Test resistor assembly in air before sealing. Verify SWR <1.1:1 at
 1.8 MHz through 50 MHz with NanoVNA.
4. Fill with mineral oil to within 25 mm of lid (room for thermal expansion).
 Seal lid with silicone RTV. Allow 24 hours to cure before transmitting.

---

## Assembly Quality

Chapter 4 specifies 8 construction/assembly steps.

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
