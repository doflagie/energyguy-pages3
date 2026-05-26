# Unit 1 — Theory of Operation
**[TM-GEAR-002 — Open Handout](../../../antenna_gear/audio_filters/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the AUDIO FILTERS — CW, SSB, AND DSP; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-002. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 LC Notch Filter

A series LC circuit resonant at the interference frequency presents near-zero
impedance in parallel with the audio path, pulling the interference signal to
ground. At resonance f0 = 1/(2π&radic;LC), the notch depth depends
on component Q. Practical notch depth: 30–50 dB. A variable
capacitor makes the notch tunable across 200–5000 Hz.

2-2 Active CW Filter

A cascade of two Sallen-Key bandpass stages using LM833 (low-noise, 15 MHz
GBW) op-amps provides approximately 36 dB/octave roll-off outside the
passband. The center frequency Q is set by resistor ratios; Q = 10 for CW
(narrow, 50 Hz &minus;3 dB BW at 500 Hz center). This narrow
bandwidth dramatically improves CW copy in high-noise conditions.

2-3 DSP Audio Processor

An ESP32-S3 with I2S audio codec (PCM5102A output, INMP441 microphone-grade
input) implements digital filters in software. FIR filters achieve linear phase
(no transient distortion); IIR filters achieve steep rolloff with fewer taps.
Available modes: CW narrow (500 Hz), SSB bandpass, noise reduction
(spectral subtraction), and audio peak filtering (sharp peak at detected
CW frequency).

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

**SC1-1.** In one sentence, state the operating principle of the AUDIO FILTERS — CW, SSB, AND DSP as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of loss or degradation in performance?

**SC1-3.** What key specification(s) (frequency range, power rating, insertion loss, impedance ratio) does the TM state?

**SC1-4.** What does Chapter 2 say the AUDIO FILTERS — CW, SSB, AND DSP cannot do — what are its limitations?

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
- The operating principle of the AUDIO FILTERS — CW, SSB, AND DSP
- The primary loss or degradation source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
