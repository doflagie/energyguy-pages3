# Unit 3 — Assembly and Setup
**[TM-GEAR-019 — Open Handout](../../../antenna_gear/variable_caps/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-019. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

4-1 Motorized Air-Variable

1. Couple the DC motor to the capacitor shaft via a flexible coupler.
 Flexible couplers compensate for shaft misalignment that would otherwise
 bind the capacitor bearings.
2. Mount two microswitches at the minimum and maximum capacitance positions
 (use the shaft angular position to trigger them via a cam or lever arm).
3. Couple the 10-turn pot to the capacitor shaft (1:1 ratio) for position
 feedback. Wire the pot as a voltage divider (0–3.3V range for
 ESP32 ADC).
4. L298N H-bridge or L293D motor driver provides bidirectional motor control
 from ESP32 GPIO signals. Use PWM for speed control if needed (slower
 near endpoints for finer positioning).

4-2 Stack-On Flat-Plate Capacitor Units

1. Cut copper foil to 50×75 mm sheets. Cut polymer dielectric
 (Kapton, LDPE, or polypropylene) to 50×80 mm (slightly larger
 to prevent edge breakdown).
2. Assemble: copper foil (top) / polymer / copper foil (bottom) / PVC backing strip.
 Offset the top and bottom foil tabs so they do not touch when stacking.
3. For parallel stacking: connect all top tabs together (plate A), all bottom
 tabs together (plate B). Each additional unit adds ~50–200 pF
 depending on dielectric thickness and area.
4. Secure units in a stack with velcro strips. The modular design allows
 capacitance to be added or removed without soldering.

---

## Assembly Quality

Chapter 4 specifies 8 construction/assembly steps.

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
