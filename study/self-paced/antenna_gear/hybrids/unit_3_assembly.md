# Unit 3 — Assembly and Setup
**[TM-GEAR-009 — Open Handout](../../../antenna_gear/hybrids/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-009. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

4-1 Branch-Line Hybrid (144 MHz)

1. Calculate λ/4 at 144 MHz on FR4 (1.6 mm, εr=4.6):
 λ/4 = c/(4 × f × &radic;εeff) ≈ 350 mm / 4 = 87.5 mm.
 Account for effective permittivity (εeff ≈ 3.4): 87.5 / &radic;3.4 = 47.5 mm.
2. Draw the four-arm square layout: each arm 47.5 mm long. Shunt arms:
 2.9 mm wide (50Ω). Series arms: 5.0 mm wide (35.35Ω).
3. Mount SMA connectors at each of the four corners. The square layout
 minimizes parasitic coupling between arms.

4-2 Lumped-Element HF Hybrid (7–30 MHz)

1. Replace each λ/4 line section with an equivalent LC π-network:
 L = Z0/ω = 50/(2π×14e6) = 568 nH (shunt arms);
 C = 1/(ω×Z0) = 1/(2π×14e6×50) = 227 pF.
2. Use 1% tolerance components to maintain amplitude and phase balance.

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
