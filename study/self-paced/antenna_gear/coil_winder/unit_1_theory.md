# Unit 1 — Theory of Operation
**[TM-GEAR-006 — Open Handout](../../../antenna_gear/coil_winder/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the MOTORIZED COIL WINDER — ESP32 CONTROLLER; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-006. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 Step Coordination

For single-layer close-wound coils:
 traverse steps per spindle revolution = wire_OD / traverse_step_size
where traverse_step_size = lead_screw_pitch / steps_per_rev_microstepped.
The ESP32 uses a step-rate ratio to keep the axes coordinated. Step pulses
are generated via hardware timers (ESP32 MCPWM or timer ISR) to maintain
precise step rates at speeds up to 1000 RPM.

2-2 TMC2208 Advantages

The TMC2208 provides stealthChop (silent operation) and 256-step
micro-stepping, reducing vibration at low speeds. Current sensing is
internal (no external sense resistors required). The UART interface
allows current, chopper mode, and speed ramping to be configured from
the ESP32 at runtime, without hardware changes.

---

## Why Theory Matters

You cannot build or use RF gear correctly without understanding how it works. Theory tells you:
- What the component does and how it produces that effect
- What the sources of loss, distortion, or error are — so you can recognize and minimize them
- What the valid operating range is — frequency, power, impedance — so you stay within specifications
- How to interpret results or system behavior that doesn't match expectations

If a component doesn't perform as expected, theory is where you look first.

---

## Self-Check Questions

**SC1-1.** In one sentence, state the operating principle of the MOTORIZED COIL WINDER — ESP32 CONTROLLER as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of loss or degradation in performance?

**SC1-3.** What key specification(s) (frequency range, power rating, insertion loss, impedance ratio) does the TM state?

**SC1-4.** What does Chapter 2 say the MOTORIZED COIL WINDER — ESP32 CONTROLLER cannot do — what are its limitations?

**SC1-5.** List two formulas or relationships from Chapter 2 that govern the component's behavior.

---

## Answer Key

**SC1-1.** See TM §2-1. Compare your sentence to the first substantive paragraph of Chapter 2.

**SC1-2.** See Chapter 2. Look for language about loss mechanisms, parasitic effects, frequency limits, or power constraints.

**SC1-3.** See Chapter 2. Look for numbers with units: %, dB, Hz, Ω, W, V.

**SC1-4.** See Chapter 2 and Chapter 1. Limitations are often stated as frequency range, power handling, or impedance range.

**SC1-5.** See Chapter 2. Equations or proportionality statements are the relationships that govern the component.

---

## Checkpoint

Before proceeding, state without looking:
- The operating principle of the MOTORIZED COIL WINDER — ESP32 CONTROLLER
- The primary loss or degradation source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
