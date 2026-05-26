# Unit 1 — Theory of Operation
**[TM-GEAR-001 — Open Handout](../../../antenna_gear/ant_switches/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the ANTENNA SWITCHES — MANUAL, MOTORIZED, AND AUTO-BAND; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-001. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 RF Switching Requirements

An antenna switch routes a single transmitter/receiver to one of N antennas
without significant signal loss or impedance mismatch. Key parameters:

- **Insertion loss:** Signal attenuation through the selected
 path. Target 2-2 Relay Types

Mechanical relays (Omron G2RL, Panasonic TQ2) provide low insertion loss
but are limited to ~10 MHz switching rate and wear over millions of
cycles. They are appropriate for all antenna switching applications.
PIN diode switches (MA4P504, HSMP-3814) switch in nanoseconds and have
no wear, but require bias current and have higher insertion loss at HF.

2-3 Auto-Band Switching

The auto-band switch monitors the transceiver's CI-V (Icom) or CAT
(Yaesu/Kenwood) bus to determine the operating frequency. A lookup table
maps frequency ranges to antenna ports. On a band change, the ESP32 toggles
the correct relay within 50 ms — before the first transmitted audio
reaches the antenna.

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

**SC1-1.** In one sentence, state the operating principle of the ANTENNA SWITCHES — MANUAL, MOTORIZED, AND AUTO-BAND as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of loss or degradation in performance?

**SC1-3.** What key specification(s) (frequency range, power rating, insertion loss, impedance ratio) does the TM state?

**SC1-4.** What does Chapter 2 say the ANTENNA SWITCHES — MANUAL, MOTORIZED, AND AUTO-BAND cannot do — what are its limitations?

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
- The operating principle of the ANTENNA SWITCHES — MANUAL, MOTORIZED, AND AUTO-BAND
- The primary loss or degradation source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
