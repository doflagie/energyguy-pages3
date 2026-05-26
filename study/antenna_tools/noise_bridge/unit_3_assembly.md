# Unit 3 — Assembly and Setup
**[TM-TOOL-006 — Open Handout](../../antenna_tools/noise_bridge/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-TOOL-006. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

4-1 Transformer Winding

1. Wind three identical windings of 6 turns #26 AWG simultaneously on a
 BN-43-202 binocular core. Keep the three windings in the same rotational
 direction (color-code the wire starts).
2. Check winding: with an ohmmeter, verify all three windings are equal
 resistance (±0.1Ω) and have no shorts between windings.
3. Test leakage inductance: connect one winding to NanoVNA port 1 with
 port 2 shorted to the other two windings. Leakage inductance should be
 4-2 Calibration Dials

Mark the R_cal potentiometer dial in 10Ω steps from 0 to 200Ω using
a resistance meter. Mark the X_cal capacitor dial in terms of &minus;jX at
10 MHz using the formula X_C = 1/(2πfC). Mark the inductor in terms
of +jX at 10 MHz using X_L = 2πfL. Actual X depends on operating
frequency; provide a reactance chart (see Appendix A) for other frequencies.

---

## Assembly Quality

Chapter 4 specifies 3 construction/assembly steps.

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
