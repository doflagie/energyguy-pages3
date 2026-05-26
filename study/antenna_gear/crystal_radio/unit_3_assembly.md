# Unit 3 — Assembly and Setup
**[TM-GEAR-008 — Open Handout](../../antenna_gear/crystal_radio/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-008. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

4-1 Classic Coil on Oatmeal Box

1. Wind 90 turns of #28 AWG enameled copper wire on a 90 mm OD
 cardboard tube (oatmeal box). Wind in a single layer, close-wound.
 Mark the center tap at turn 45 for antenna connection point.
2. Scrape enamel from the wire at 10-turn intervals (turns 10, 20, ..., 80)
 and solder short leads. These form taps for the antenna coupling
 adjustment.
3. Connect a variable air capacitor (10–365 pF) in parallel
 with the full winding. The capacitor shaft is the tuning control.
4. Connect the galena crystal or 1N34A diode in series with the headphones
 across the tank circuit. Cathode toward the high-impedance (ungrounded)
 end of the tank.

4-2 Headphone Impedance Transformer

1. Wind an audio transformer on a laminated iron core: primary 2000
 turns #38 AWG (matches high crystal set impedance); secondary 60 turns
 #26 AWG (matches 32Ω headphone).
2. Turns ratio: n = sqrt(Z_primary / Z_secondary) = sqrt(2000/32) = 7.9:1.
3. Actual winding: 2000:250 turns is a practical compromise for ease
 of winding (ratio = 8:1 = 9 dB power gain vs. direct connection).

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
