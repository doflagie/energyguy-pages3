# Unit 1 — Theory of Operation
**[TM-ANT-065 — Open Handout](../../antennas/telescopic_vertical/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the TELESCOPIC VERTICAL ANTENNA; identify key electrical characteristics
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-ANT-065. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. RADIATION PHYSICS

Telescoping multi-section vertical antenna adjusted to quarter-wave at target frequency; tapped loading coil for lower bands. A vertical radiator carries current that is vertically polarized; its electromagnetic wave radiates with E-field vertical. The ground (or radial counterpoise) serves as the electrical mirror-image of the above-ground element: a λ/4 vertical above a perfect ground is equivalent to a λ/2 dipole in free space, with radiation resistance Rr = 36.6 Ω.

### 2-2. RADIATION PATTERN

A quarter-wave vertical over an extensive ground plane radiates omnidirectionally in azimuth with a low-angle elevation lobe ideal for DX. The elevation angle of maximum radiation (θmax) depends on ground conductivity and number of radials: over a perfect ground θmax ≈ 0°; over average ground with 32+ radials, θmax ≈ 5–15°. The vertically polarized wave follows the earth&rsquo;s surface better than a horizontal wave at low angles.

### 2-3. IMPEDANCE AND BANDWIDTH

Feed impedance: 50 Ω (base, with matching). The SWR bandwidth of a simple λ/4 vertical at HF is approximately 5–8% of center frequency for 2:1 SWR, typically covering one amateur band. Loading coils reduce bandwidth in proportion to their Q; top loading preserves more bandwidth than base loading because it maintains higher current along more of the radiator length.

### 2-4. GROUND SYSTEM DESIGN

Ground loss resistance Rg appears directly in series with the radiation resistance. For maximum efficiency: Rg < Rr. With buried radials, Rg decreases as Nradials increases: 32 radials each 0.25λ gives Rg ≈ 3 Ω. Elevated resonant radials achieve similar performance with only 4–8 radials. For telescoping segments extending to quarter-wave per band — follow this requirement closely for efficiency.

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

**SC1-1.** In one sentence, state the operating principle of the TELESCOPIC VERTICAL ANTENNA as described in Chapter 2.

**SC1-2.** What determines the resonant frequency of the TELESCOPIC VERTICAL ANTENNA? Name the primary physical parameter(s).

**SC1-3.** What feedpoint impedance does Chapter 2 predict for the TELESCOPIC VERTICAL ANTENNA in free space? How does that change over real ground?

**SC1-4.** What radiation pattern does the TELESCOPIC VERTICAL ANTENNA produce? What are the nulls and maxima directions?

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
- The operating principle of the TELESCOPIC VERTICAL ANTENNA
- What determines its resonant frequency
- The expected feedpoint impedance

**→ Proceed to [Unit 2](unit_2_materials.md)**
