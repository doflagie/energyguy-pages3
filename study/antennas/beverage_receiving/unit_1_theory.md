# Unit 1 — Theory of Operation
**[TM-ANT-010 — Open Handout](../../antennas/beverage_receiving/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the BEVERAGE RECEIVING ANTENNA; identify key electrical characteristics
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-ANT-010. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. DIRECTIVITY AND NOISE REJECTION

Terminated long-wire broadside receiving array with ~600 ω resistive terminator. Receive-only antennas exploit directivity (front-to-back ratio) and aperture to separate signals from noise. A terminated long-wire or loop antenna achieves a cardioid or kidney-shaped pattern with deep null in one direction: F/B ratio typically 15–25 dB. This null can be steered toward interference sources (power-line noise, broadcast QRM) to dramatically improve signal-to-noise ratio on weak HF signals.

### 2-2. NOISE FIGURE AND SNR

At HF below 30 MHz, external noise (atmospheric, man-made) dominates over receiver noise figure. A receiving antenna with poor efficiency but good directivity can outperform a high-gain antenna pointed at a noise source. The key metric is signal-to-noise ratio (SNR) improvement over the reference antenna (typically a 40M dipole), not absolute signal level. An SNR improvement of 10–20 dB (1–2 S-units) on a target station makes marginal copy copy into solid copy.

### 2-3. TERMINATION RESISTANCE

Most directional receive antennas use a termination resistor (typically 500–900 Ω) to absorb backward-traveling waves and prevent re-radiation from the far end. This resistance determines the F/B ratio and the wave velocity factor along the antenna. 120–240 m length, 600 Ω terminator at far end, 9:1 transformer at feed end, cardioid null pattern. The termination must be a non-inductive resistor (carbon composition or metal film; not wirewound) mounted in a weatherproof housing.

---

## Why Theory Matters for Antenna Construction

You cannot build a working antenna without understanding the underlying physics. Theory tells you:
- What determines resonant frequency — and therefore how cutting or loading errors affect performance
- What radiation pattern the antenna produces and why physical layout matters
- What feedpoint impedance to expect — so you know whether a matching network is needed
- What the sources of loss are: conductor resistance, ground losses, impedance mismatch

If the antenna doesn't resonate where expected, or SWR is high, theory is where you diagnose the cause.

---

## Self-Check Questions

**SC1-1.** In one sentence, state the operating principle of the BEVERAGE RECEIVING ANTENNA as described in Chapter 2.

**SC1-2.** What determines the resonant frequency of the BEVERAGE RECEIVING ANTENNA? Name the primary physical parameter(s).

**SC1-3.** What feedpoint impedance does Chapter 2 predict for the BEVERAGE RECEIVING ANTENNA in free space? How does that change over real ground?

**SC1-4.** What radiation pattern does the BEVERAGE RECEIVING ANTENNA produce? What are the nulls and maxima directions?

**SC1-5.** List two formulas or relationships from Chapter 2 that govern the antenna's electrical behavior.

---

## Answer Key

**SC1-1.** See TM §2-1. Compare your sentence to the first substantive paragraph of Chapter 2.

**SC1-2.** See Chapter 2. For most antennas the primary parameter is physical length relative to wavelength. Loading (coils, capacitors) shifts this.

**SC1-3.** See Chapter 2. Free-space feedpoint impedance is a theoretical value; ground proximity, height, and nearby conductors modify it significantly.

**SC1-4.** See Chapter 2. Directional patterns are usually shown in terms of azimuth and elevation radiation patterns.

**SC1-5.** See Chapter 2 and Appendix A. The key equation usually relates length to frequency, or impedance to element geometry.

---

## Checkpoint

Before proceeding, state without looking:
- The operating principle of the BEVERAGE RECEIVING ANTENNA
- What determines its resonant frequency
- The expected feedpoint impedance

**→ Proceed to [Unit 2](unit_2_materials.md)**
