# Unit 1 — Theory of Operation
**[TM-ANT-045 — Open Handout](../../antennas/magnetic_loop_antennas/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the MAGNETIC LOOP ANTENNA; identify key electrical characteristics
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-ANT-045. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. RESONANT LOOP PHYSICS

A magnetic loop (small transmitting loop, STL) is an electrically small resonant circuit. The loop inductance L is tuned to resonance by a capacitor C. At resonance, circulating current Icirc = Vin × Q / Zloop where Q is the loop quality factor. Q values of 100–1000 are typical; this produces circulating currents far exceeding the feed current, and capacitor voltages VC = Icirc / (2πf·C) which can reach 4,000–12,000 V at 100 W.

### 2-2. RADIATION PATTERN AND GAIN

A small horizontal loop radiates as a magnetic dipole with a figure-eight pattern in the plane of the loop (null perpendicular to loop plane). Gain: 2–4 dBi (at height above 0.1λ). The null depth exceeds 20 dB and can be used for interference rejection by rotating the loop. Bandwidth is inversely proportional to Q: BW3dB = f0/Q; typical BW = 3–15 kHz at HF. This narrow bandwidth means the loop must be retuned for every frequency change.

### 2-3. EFFICIENCY AND CONDUCTOR SELECTION

Efficiency: 75–95% (depends on Q; copper best). Loop efficiency η = Rr/(Rr + Rloss). Radiation resistance Rr for a small loop scales as (A/λ²)², making large loop diameter critical for efficiency. Conductor resistance Rloss must be minimized: use copper or aluminum tubing (1–2 in OD for HF transmitting loops). Connections must be silver-soldered or bolted with contact resistance <0.001 Ω. Any high-resistance joint will dissipate power and possibly arc at high Q.

### 2-4. CAPACITOR REQUIREMENTS

Vacuum variable capacitor (recommended): rated for peak voltage ≥5 kV at operating power level. Split-stator butterfly capacitor: acceptable if plate gap is sufficient. Do NOT use: aluminum plate DIY capacitors, rolled-foil capacitors, silver-mica stacks, or coaxial stubs. Typical capacitance range: 10–500 pF for 40M–10M operation with 6–10 ft loop diameter.

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

**SC1-1.** In one sentence, state the operating principle of the MAGNETIC LOOP ANTENNA as described in Chapter 2.

**SC1-2.** What determines the resonant frequency of the MAGNETIC LOOP ANTENNA? Name the primary physical parameter(s).

**SC1-3.** What feedpoint impedance does Chapter 2 predict for the MAGNETIC LOOP ANTENNA in free space? How does that change over real ground?

**SC1-4.** What radiation pattern does the MAGNETIC LOOP ANTENNA produce? What are the nulls and maxima directions?

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
- The operating principle of the MAGNETIC LOOP ANTENNA
- What determines its resonant frequency
- The expected feedpoint impedance

**→ Proceed to [Unit 2](unit_2_materials.md)**
