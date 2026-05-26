# Unit 1 — Theory of Operation
**[TM-ANT-044 — Open Handout](../../../antennas/log_periodic_dipole_arrays_for_wideband_operation/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the LOG PERIODIC DIPOLE ARRAY (LPDA) — WIDEBAND; identify key electrical characteristics
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-ANT-044. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. PARASITIC COUPLING PHYSICS

Multi-element lpda designed with log-periodic taper ratio τ ≈ 0.9 and spacing factor σ ≈ 0.05 for maximum gain/bandwidth. Gain is achieved through parasitic coupling: the driver excites the reflector and director elements by near-field induction. The reflector (5% longer than the driven element) carries current that lags by approximately 160° and re-radiates energy forward. Directors (5% shorter) carry current leading by approximately 140° and focus energy forward. Each additional director adds approximately 1 dB of gain at optimal spacing (0.2–0.25λ).

### 2-2. ELEMENT DESIGN RULES

For a 3-element Yagi on VHF: driver = 0.473λ, reflector = 0.505λ, director = 0.440λ. Spacing: reflector to driver = 0.2λ, driver to director = 0.25λ. These values produce forward gain of 7–8 dBd with front-to-back ratio of 20–25 dB. Gain estimate: 8–12 dBi. At UHF, dimensional tolerance is critical — elements must be within ±1 mm of design length for proper pattern formation.

### 2-3. FEED IMPEDANCE AND MATCHING

Feed impedance: 50 Ω. The driven element impedance drops below 50 Ω when directors are added (typically 20–40 Ω). A gamma match, T-match, or delta match raises this to 50 Ω. The gamma match uses a parallel conductor tapped on the driven element to form an L-network; the shorting bar position and gamma rod length are adjusted for 50 Ω + j0 Ω at the design frequency.

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

**SC1-1.** In one sentence, state the operating principle of the LOG PERIODIC DIPOLE ARRAY (LPDA) — WIDEBAND as described in Chapter 2.

**SC1-2.** What determines the resonant frequency of the LOG PERIODIC DIPOLE ARRAY (LPDA) — WIDEBAND? Name the primary physical parameter(s).

**SC1-3.** What feedpoint impedance does Chapter 2 predict for the LOG PERIODIC DIPOLE ARRAY (LPDA) — WIDEBAND in free space? How does that change over real ground?

**SC1-4.** What radiation pattern does the LOG PERIODIC DIPOLE ARRAY (LPDA) — WIDEBAND produce? What are the nulls and maxima directions?

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
- The operating principle of the LOG PERIODIC DIPOLE ARRAY (LPDA) — WIDEBAND
- What determines its resonant frequency
- The expected feedpoint impedance

**→ Proceed to [Unit 2](unit_2_materials.md)**
