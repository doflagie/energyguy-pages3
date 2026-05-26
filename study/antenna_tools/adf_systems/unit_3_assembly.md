# Unit 3 — Assembly and Setup
**[TM-TOOL-001 — Open Handout](../../antenna_tools/adf_systems/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-TOOL-001. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

4-1 Watson-Watt Loop Construction

1. Wind two identical shielded loops: 30 cm diameter, 3 turns #18 AWG,
 Faraday shield (gap at top, not a closed loop).
2. Mount loops orthogonally on a common center mast; orient one loop N-S,
 other E-W.
3. Run equal-length coax (within 5 mm) from each loop to the receiver
 switching point.
4. Connect sense antenna (vertical whip, λ/4 at center frequency) at
 same switching point.

4-2 Butler Matrix PCB

1. Fabricate 4-port 3 dB 90° hybrid couplers on FR4 (Z0=35.4Ω
 microstrip, λ/4 length at design frequency).
2. Connect hybrids per the Butler Matrix topology:
 E1,E2 → Hybrid #1,#2; outputs of Hybrid #1,#2 → Hybrid #3,#4
 with a 45° fixed phase shifter between Hybrid #2 output and Hybrid #4 input.
3. Verify phase relationships with NanoVNA S21 phase measurements before
 installing array elements.
4. Mount array elements at corners of a square, λ/4 side length, connected
 to matrix element ports E1–E4.

4-3 Doppler Commutator

1. Mount 4 or 8 vertical elements equally spaced on a circle of radius
 λ/4 at operating frequency.
2. Wire each element through an RF relay (PIN diode or mechanical) to a
 common output coax.
3. Drive relay sequence from ESP32 GPIO at rotation rate f_rot (start at
 200 Hz for HF).
4. Connect common output to receiver; extract FM component in software
 (GNU Radio or custom DSP).

---

## Assembly Quality

Chapter 4 specifies 12 construction/assembly steps.

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
