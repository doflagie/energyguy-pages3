# Unit 1 — Theory of Operation
**[TM-ANT-021 — Open Handout](../../../antennas/end_fed_random_wire/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the END-FED RANDOM WIRE (EFRW) ANTENNA; identify key electrical characteristics
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-ANT-021. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. END-FED MATCHING

Random-length wire (avoid multiples of λ/2 per band) fed via 9:1 or capacitive coupler into atu. An end-fed wire presents a high impedance at its end: ~2500–5000 Ω for a half-wave wire; ~1000–2000 Ω for a quarter-wave wire. This high impedance is transformed down to 50 Ω by the matching transformer or unun. Design values: Varies widely; 9:1 unun + ATU to 50 Ω. The transformation ratio n² = Zwire/Zcoax; for a 49:1 unun, n = 7, so the wire impedance appears as 49×50 = 2450 Ω from the wire side.

### 2-2. RADIATION PATTERN

An end-fed wire radiates in a pattern that varies with length and frequency. A half-wave wire at 10 ft height produces a bidirectional broadside pattern similar to a dipole. At harmonic frequencies the pattern has multiple lobes and nulls. Gain per design: 2–4 dBi (length and frequency dependent). The pattern is affected by the run of the feed line if common-mode current is not controlled.

### 2-3. COUNTERPOISE REQUIREMENTS

A counterpoise (short wire or coax braid) is required as a reference conductor for the feed voltage to work against. Without a counterpoise the coax braid serves as the counterpoise, causing RF current on the braid. Recommended counterpoise: 0.05λ minimum length wire from the unun ground terminal, or a 1:1 choke isolating the antenna current from the coax outer.

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

**SC1-1.** In one sentence, state the operating principle of the END-FED RANDOM WIRE (EFRW) ANTENNA as described in Chapter 2.

**SC1-2.** What determines the resonant frequency of the END-FED RANDOM WIRE (EFRW) ANTENNA? Name the primary physical parameter(s).

**SC1-3.** What feedpoint impedance does Chapter 2 predict for the END-FED RANDOM WIRE (EFRW) ANTENNA in free space? How does that change over real ground?

**SC1-4.** What radiation pattern does the END-FED RANDOM WIRE (EFRW) ANTENNA produce? What are the nulls and maxima directions?

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
- The operating principle of the END-FED RANDOM WIRE (EFRW) ANTENNA
- What determines its resonant frequency
- The expected feedpoint impedance

**→ Proceed to [Unit 2](unit_2_materials.md)**
