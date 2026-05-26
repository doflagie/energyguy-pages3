# Unit 3 — Assembly and Setup
**[TM-GEAR-011 — Open Handout](../../antenna_gear/mast_tower/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-011. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

4-1 Telescoping Mast Erection

1. Install the base plate on level ground. Anchor with 4×screw anchors
 or 4×250 kg sandbags in field operation.
2. Extend sections one at a time, starting from the bottom. Tighten each
 section clamp before extending the next.
3. Install guys at three levels: 1/3, 2/3, and top of full height.
 Guy radius must be ≥40% of height (rule of thumb); recommended 60%.
4. Tension all guys evenly. Mast vertical alignment: check with a plumb
 bob from the top. Adjust guys until mast is plumb to within 1°.

4-2 Pneumatic Mast

1. Extend collapsed mast horizontally. Connect air supply (bicycle pump,
 foot pump, or compressor) to the base Schrader valve.
2. Pump to 2.5–3.5 bar (36–51 psi). The sections
 extend sequentially from top to bottom. Full extension: approximately
 60 pump strokes with a standard floor pump.
3. Once extended, set upright. Install base plate or drive the ground
 spike. Pneumatic masts are self-supporting to 9 m with no antenna
 wind load; add guys for wind exposure or antenna weight >2 kg.

---

## Assembly Quality

Chapter 4 specifies 7 construction/assembly steps.

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
