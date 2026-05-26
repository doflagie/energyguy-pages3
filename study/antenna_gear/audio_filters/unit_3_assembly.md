# Unit 3 — Assembly and Setup
**[TM-GEAR-002 — Open Handout](../../antenna_gear/audio_filters/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-002. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

4-1 LC Notch Filter

1. Wind 1 mH inductor on a ferrite pot-core for the highest achievable Q
 (target Q ≥50). Measure inductance with LCR meter before mounting.
2. Connect a variable capacitor (10–100 pF air-variable) in series
 with the inductor. This LC series combination connects in shunt (in parallel)
 with the audio signal path between the receiver audio output and the headphone
 or speaker.
3. Tune by listening: with an interfering tone present, adjust the variable cap
 until the tone disappears. Notch depth should be audibly dramatic
 (>30 dB).

4-2 Active CW Filter

1. Build two Sallen-Key stages. Stage 1: f0 = 500 Hz, Q = 5.
 Stage 2: f0 = 500 Hz, Q = 10. Series connection gives
 combined Q ~50 (approximate).
2. Power: use a ±12V regulated supply. Use 100 nF NP0 + 10 &micro;F
 electrolytic bypass on each supply pin within 5 mm of the op-amp.
3. Set gain <6 dB to avoid clipping. Check output at maximum received
 signal level with an oscilloscope — no flat-topping.

---

## Assembly Quality

Chapter 4 specifies 6 construction/assembly steps.

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
