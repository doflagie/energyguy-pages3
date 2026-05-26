# Unit 1 — Theory of Operation
**[TM-ANT-071 — Open Handout](../../../antennas/v_beam/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the V-BEAM ANTENNA; identify key electrical characteristics
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-ANT-071. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. DESIGN PRINCIPLES

Two long wire elements arranged in a v-shape from a common feedpoint; bidirectional pattern along the v bisector. Key parameters: 60–120 ft legs; 120–180° apex angle; terminated or non-terminated variants; directional gain 4–8 dBi on bisector.

### 2-2. RADIATION CHARACTERISTICS

Gain: 4–8 dBi bidirectional along bisector. Efficiency: 75–88%. Feed impedance: 500–800 Ω at apex (10:1 unun or ATU). The pattern and polarization depend on the specific antenna geometry; consult the NEC2 model for accurate polar plots and gain/elevation data.

### 2-3. PROPAGATION APPLICATION

This antenna is optimized for its specific application (see subtitle). Operating it outside the designed frequency range or in a different orientation from the NEC2 model will result in degraded performance — consult the simulation before making substitutions.

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

**SC1-1.** In one sentence, state the operating principle of the V-BEAM ANTENNA as described in Chapter 2.

**SC1-2.** What determines the resonant frequency of the V-BEAM ANTENNA? Name the primary physical parameter(s).

**SC1-3.** What feedpoint impedance does Chapter 2 predict for the V-BEAM ANTENNA in free space? How does that change over real ground?

**SC1-4.** What radiation pattern does the V-BEAM ANTENNA produce? What are the nulls and maxima directions?

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
- The operating principle of the V-BEAM ANTENNA
- What determines its resonant frequency
- The expected feedpoint impedance

**→ Proceed to [Unit 2](unit_2_materials.md)**
