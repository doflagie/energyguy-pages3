# Unit 1 — Theory of Operation
**[TM-ANT-060 — Open Handout](../../../antennas/slot_antenna_uhf/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the SLOT ANTENNA — UHF 70cm; identify key electrical characteristics
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-ANT-060. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. DESIGN PRINCIPLES

Slot antenna cut in pcb ground plane; complementary to dipole by babinet's principle; wideband design. Key parameters: 36 mm slot on PCB ground plane; 50 Ω microstrip feed at slot center; wideband for full 70cm band coverage.

### 2-2. RADIATION CHARACTERISTICS

Gain: 2 dBi omnidirectional in plane. Efficiency: 80–92%. Feed impedance: 50 Ω (microstrip inset or coax probe). The pattern and polarization depend on the specific antenna geometry; consult the NEC2 model for accurate polar plots and gain/elevation data.

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

**SC1-1.** In one sentence, state the operating principle of the SLOT ANTENNA — UHF 70cm as described in Chapter 2.

**SC1-2.** What determines the resonant frequency of the SLOT ANTENNA — UHF 70cm? Name the primary physical parameter(s).

**SC1-3.** What feedpoint impedance does Chapter 2 predict for the SLOT ANTENNA — UHF 70cm in free space? How does that change over real ground?

**SC1-4.** What radiation pattern does the SLOT ANTENNA — UHF 70cm produce? What are the nulls and maxima directions?

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
- The operating principle of the SLOT ANTENNA — UHF 70cm
- What determines its resonant frequency
- The expected feedpoint impedance

**→ Proceed to [Unit 2](unit_2_materials.md)**
