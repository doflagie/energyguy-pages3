# Unit 1 — Theory of Operation
**[TM-GEAR-013 — Open Handout](../../../antenna_gear/power_supplies/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the POWER SUPPLIES — LINEAR, SWITCHING, AND FIELD PORTABLE; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-013. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 Linear Regulated Supply

A transformer (120V/18V, 40VA) followed by a full-wave bridge rectifier
and large filter capacitor (10,000–20,000 &micro;F) provides
unregulated DC. A series-pass transistor (multiple 2N3055 or IRF540 in
parallel) regulated by an op-amp error amplifier maintains constant output
voltage. Linear supplies have extremely low ripple and noise (2-2 Switching SMPS

An SMPS converts AC to DC at high frequency (50–500 kHz),
allowing much smaller transformers. Efficiency: 85–92%. The PWM
controller (UC3842 or SG3525) regulates output voltage by adjusting duty
cycle. EMI filtering is critical for SMPS; they generate conducted and
radiated interference that can degrade receiver performance.

2-3 Metering and Protection

A metering module uses a series current shunt (100 A / 75 mV
shunt) and a voltage divider to display voltage and current on a CYD display.
Over-current protection uses a comparator circuit to trip a power relay if
current exceeds the set limit (adjustable 0–30 A via a trimpot).

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

**SC1-1.** In one sentence, state the operating principle of the POWER SUPPLIES — LINEAR, SWITCHING, AND FIELD PORTABLE as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of loss or degradation in performance?

**SC1-3.** What key specification(s) (frequency range, power rating, insertion loss, impedance ratio) does the TM state?

**SC1-4.** What does Chapter 2 say the POWER SUPPLIES — LINEAR, SWITCHING, AND FIELD PORTABLE cannot do — what are its limitations?

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
- The operating principle of the POWER SUPPLIES — LINEAR, SWITCHING, AND FIELD PORTABLE
- The primary loss or degradation source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
