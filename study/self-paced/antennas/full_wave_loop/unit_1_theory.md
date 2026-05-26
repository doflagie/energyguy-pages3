# Unit 1 — Theory of Operation
**[TM-ANT-031 — Open Handout](../../../antennas/full_wave_loop/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the FULL-WAVE LOOP ANTENNA; identify key electrical characteristics
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-ANT-031. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. RADIATION PHYSICS

Full-wavelength closed loop (any shape: square, circle, triangle) fed at one point; inherently low radiation angle. A full-wave loop fed at any point resonates when the total conductor length equals one free-space wavelength: λ = 300/fMHz meters. The feed-point impedance of a square loop at resonance is approximately 100–150 Ω (real), varying with shape and height above ground. A circular loop has slightly higher radiation resistance than a square loop of the same perimeter.

### 2-2. RADIATION PATTERN

A horizontal full-wave loop at 0.5λ height produces a low-angle radiation pattern in the plane of the loop (edge-fire). Gain over a dipole at the same height is approximately 1.5–2 dB because the current distribution around the loop produces additive broadside contributions. Vertical loops produce an omnidirectional azimuth pattern with vertically polarized radiation; horizontal loops are horizontally polarized. Estimated gain: 4–6 dBi edge-fire.

### 2-3. IMPEDANCE AND MATCHING

Feed impedance: 100–150 Ω (2:1 balun to 50 Ω). The loop impedance at resonance is typically 100–150 Ω, requiring a 2:1 balun (or direct feed to 75 Ω coax) for a reasonable match to 50 Ω systems. Off-resonance the loop can be operated through an ATU. Harmonic resonances occur at odd and even multiples of the fundamental frequency, extending multiband coverage without retuning.

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

**SC1-1.** In one sentence, state the operating principle of the FULL-WAVE LOOP ANTENNA as described in Chapter 2.

**SC1-2.** What determines the resonant frequency of the FULL-WAVE LOOP ANTENNA? Name the primary physical parameter(s).

**SC1-3.** What feedpoint impedance does Chapter 2 predict for the FULL-WAVE LOOP ANTENNA in free space? How does that change over real ground?

**SC1-4.** What radiation pattern does the FULL-WAVE LOOP ANTENNA produce? What are the nulls and maxima directions?

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
- The operating principle of the FULL-WAVE LOOP ANTENNA
- What determines its resonant frequency
- The expected feedpoint impedance

**→ Proceed to [Unit 2](unit_2_materials.md)**
