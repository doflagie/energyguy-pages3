# Unit 1 — Theory of Operation
**[TM-GEAR-003 — Open Handout](../../../antenna_gear/baluns-and-ununs/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the BALUNS AND UN-UNS — CURRENT AND VOLTAGE TYPES; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-003. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 Current vs. Voltage Balun

A **current balun** (choke balun) enforces equal and opposite
currents in the two antenna elements by presenting high impedance (Z_choke
≥2000Ω) to common-mode current on the coax shield. The
differential mode (transmit signal) sees only the low impedance of the coax.
A current balun does NOT transform impedance; it only suppresses common-mode.

A **voltage balun** enforces equal and opposite voltages.
It is simpler to build but allows common-mode current if the antenna is
not symmetrical. Current baluns are preferred for dipoles and loops;
voltage baluns may be used when impedance transformation is also needed.

2-2 Transmission-Line Transformer (TLT)

TLTs (Guanella, trifilar) use transmission-line principles rather than
magnetic coupling alone. The characteristic impedance of the transmission line
on the core determines the transformation ratio. For a 4:1 Guanella: two
equal-impedance lines are connected in series at the input and parallel at
the output, giving a 4:1 impedance ratio.

Impedance ratio relationships:

1:1 choke:   Z_in = Z_out = 50Ω (no transformation; common-mode choke only)
4:1 Guanella: Z_in = 200Ω balanced → 50Ω unbalanced
9:1 trifilar: Z_in = 450Ω unbalanced → 50Ω unbalanced

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

**SC1-1.** In one sentence, state the operating principle of the BALUNS AND UN-UNS — CURRENT AND VOLTAGE TYPES as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of loss or degradation in performance?

**SC1-3.** What key specification(s) (frequency range, power rating, insertion loss, impedance ratio) does the TM state?

**SC1-4.** What does Chapter 2 say the BALUNS AND UN-UNS — CURRENT AND VOLTAGE TYPES cannot do — what are its limitations?

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
- The operating principle of the BALUNS AND UN-UNS — CURRENT AND VOLTAGE TYPES
- The primary loss or degradation source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
