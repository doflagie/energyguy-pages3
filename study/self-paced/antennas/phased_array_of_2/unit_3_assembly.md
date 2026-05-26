# Unit 3 — Assembly Procedures
**[TM-ANT-052 — Open Handout](../../../antennas/phased_array_of_2/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly in the correct sequence; achieve the physical dimensions specified
**Estimated time:** 25 minutes

---

## Step 1: Read the TM

Open TM-ANT-052. Read **Chapter 4 — Assembly Procedures** completely.

Then come back here.

---

## Chapter 4 Content

> **CAUTION — RF EXPOSURE:** Maintain minimum safe distance from all energized antenna elements during transmission.
At QRP power levels (≤5 W) the MPE boundary is typically

1. Extract all element lengths and spacings from NEC2 model file phased_array_of_2.nec (GW card dimensions). Convert from meters to inches or feet for construction.
2. Fabricate each element to ±1 mm tolerance (VHF/UHF) or ±0.5 in (HF). Label elements before assembly.
3. Assemble support structure. Verify element positions match NEC model geometry. Use non-conductive fasteners where element isolation is required.
4. Install feed connector at designed feedpoint location. Install any required matching network (gamma match, balun, transformer).
5. Connect feed line. Install common-mode choke (Mix-31 toroid) at feedpoint. Route feed line to minimize coupling with antenna elements.
6. Perform SWR measurement per Chapter 5 before installing at full height.

---

## Assembly Quality

Chapter 4 specifies 6 assembly steps.

Physical dimensions and assembly quality directly determine resonant frequency and feedpoint impedance:
- Antenna length errors shift resonance: ~1% length error ≈ ~1% frequency shift at HF
- Poor solder joints at the feedpoint create resistance that raises SWR and wastes power
- Loose connections in a matching network change the impedance transformation ratio
- Incorrect element spacing in a beam changes gain and front-to-back ratio
- Coax shield continuity to the feedpoint determines how much common-mode current flows on the shield

If Chapter 4 specifies a pre-installation check (e.g., ohmmeter continuity or pre-resonance trim), do it. These checks catch fabrication errors before the antenna is in the air.

---

## Self-Check Questions

**SC3-1.** How many assembly steps does Chapter 4 specify?

**SC3-2.** What is the specified length of the main radiating element? How was that length calculated?

**SC3-3.** Does Chapter 4 specify a feedpoint connection method? What is the correct solder or mechanical technique?

**SC3-4.** Does Chapter 4 require a pre-installation check? What does it verify?

**SC3-5.** If the assembled element is 2% shorter than specified, which direction will resonance shift? By approximately how much?

---

## Answer Key

**SC3-1.** Count the numbered steps in Chapter 4.

**SC3-2.** See Chapter 4. The design length comes from the formula in Appendix A (typically 468/f or 234/f in feet, adjusted for wire diameter and proximity effects).

**SC3-3.** See Chapter 4. Good RF solder joints are smooth, shiny, and mechanically sound. Cold joints show up as dull, granular surfaces.

**SC3-4.** Scan Chapter 4 for verification steps before installation. Common pre-checks: DC continuity across element, coax shield-to-ground, open circuit across element center.

**SC3-5.** Shorter element = higher resonant frequency. At HF, the shift is approximately proportional: 2% shorter ≈ 2% higher frequency.

---

## Checkpoint

Before proceeding:
- [ ] You have read Chapter 4 completely
- [ ] You can state the specified element length and its derivation
- [ ] You have assembled the antenna per Chapter 4

**→ Proceed to [Unit 4](unit_4_calibration.md)**
