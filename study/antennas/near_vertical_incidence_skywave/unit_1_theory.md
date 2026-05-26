# Unit 1 — Theory of Operation
**[TM-ANT-047 — Open Handout](../../antennas/near_vertical_incidence_skywave/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the NEAR VERTICAL INCIDENCE SKYWAVE (NVIS) ANTENNA; identify key electrical characteristics
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-ANT-047. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. RADIATION PHYSICS

Horizontally polarized dipole or inverted-v at low height (0.05–0.15λ above ground) producing near-vertical radiation angle. A dipole radiates because time-varying current in the conductor produces a time-varying magnetic field; the resulting displacement current produces a time-varying electric field; together these fields propagate outward as an electromagnetic wave. For a half-wave dipole at resonance the radiation resistance Rr = 73 Ω and reactance X = 0, producing maximum current amplitude for a given drive voltage.

### 2-2. RADIATION PATTERN

A horizontal half-wave dipole at height h above ground produces a bidirectional pattern broadside to the wire axis. At height ≥0.5λ the main lobe elevation angle θ ≈ 15–30°, ideal for DX paths. At height <0.15λ (NVIS regime) the main lobe is near-vertical. The E-plane pattern is a figure-eight; H-plane is omnidirectional. Gain broadside is 2.1 dBi relative to isotropic.

### 2-3. IMPEDANCE AND BANDWIDTH

Feed impedance: 50–75 Ω (height-dependent). A longer or cage construction widens bandwidth because the increased effective conductor diameter reduces the Q of the resonance. For a simple wire dipole, the 2:1 SWR bandwidth at 40M is typically 100–200 kHz; a cage construction widens this to 300–500 kHz or the full band.

### 2-4. POLARIZATION AND PROPAGATION

This antenna is linearly polarized. At HF, ionospheric propagation rotates polarization (Faraday rotation), so polarization match at the far end is largely unpredictable. Cross-polarization loss of 3–10 dB is common on skywave paths. For NVIS and groundwave propagation, horizontal polarization is preferred for local coverage.

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

**SC1-1.** In one sentence, state the operating principle of the NEAR VERTICAL INCIDENCE SKYWAVE (NVIS) ANTENNA as described in Chapter 2.

**SC1-2.** What determines the resonant frequency of the NEAR VERTICAL INCIDENCE SKYWAVE (NVIS) ANTENNA? Name the primary physical parameter(s).

**SC1-3.** What feedpoint impedance does Chapter 2 predict for the NEAR VERTICAL INCIDENCE SKYWAVE (NVIS) ANTENNA in free space? How does that change over real ground?

**SC1-4.** What radiation pattern does the NEAR VERTICAL INCIDENCE SKYWAVE (NVIS) ANTENNA produce? What are the nulls and maxima directions?

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
- The operating principle of the NEAR VERTICAL INCIDENCE SKYWAVE (NVIS) ANTENNA
- What determines its resonant frequency
- The expected feedpoint impedance

**→ Proceed to [Unit 2](unit_2_materials.md)**
