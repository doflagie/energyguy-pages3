# Unit 3 — Assembly and Setup
**[TM-GEAR-012 — Open Handout](../../../antenna_gear/motorized_tuner/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-012. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

1. Mount air variable capacitors in the enclosure with motor shafts coupled
 via flexible shaft couplers. Use Hall-effect limit switches at both
 ends of travel to prevent over-rotation.
2. Mount the roller inductor vertically. Couple the DC motor to the roller
 shaft via a gear train (10:1 reduction for precise positioning).
3. Connect the 10-turn pots to the capacitor and inductor shafts via
 1:1 direct couplers. Wire pot center tap to ESP32 ADC via a 10 kΩ
 voltage divider.
4. Mount the SWR bridge inline between the radio connector and the T-network
 input. Connect VFWD and VREF to ESP32 ADC.
5. Wire L298N H-bridges: IN1/IN2 and IN3/IN4 to ESP32 GPIOs (PWM for
 speed control); OUT1/OUT2 and OUT3/OUT4 to motors. Enable pins to 3.3V
 for always-on.

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
