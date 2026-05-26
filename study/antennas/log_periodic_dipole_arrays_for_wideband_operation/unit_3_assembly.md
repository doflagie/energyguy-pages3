# Unit 3 — Assembly Procedures
**[TM-ANT-044 — Open Handout](../../antennas/log_periodic_dipole_arrays_for_wideband_operation/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly in the correct sequence; achieve the physical dimensions specified
**Estimated time:** 25 minutes

---

## Step 1: Read the TM

Open TM-ANT-044. Read **Chapter 4 — Assembly Procedures** completely.

Then come back here.

---

## Chapter 4 Content

> **CAUTION — RF EXPOSURE:** Maintain minimum safe distance from all energized antenna elements during transmission.
At QRP power levels (≤5 W) the MPE boundary is typically

1. Cut all elements to calculated lengths. Deburr cut ends. Label each element (reflector, driven, director-1, director-2 ...).
2. Mark element mounting positions on boom. Verify spacings with tape measure before drilling. Drilling error >5 mm will degrade F/B ratio.
3. Mount parasitic elements through boom (metal boom) using nylon or PVC element mounts, or through metallic boom with direct element-to-boom contact (boom correction required for metallic boom).
4. Install driven element with insulated center block. Mount gamma match rod parallel to driver at 6–10 in from center; connect shorting bar and trimmer capacitor.
5. Attach coax feed: center pin to gamma rod, braid to boom ground. Route cable along boom toward rear to minimize feed-line distortion of pattern.
6. Perform initial SWR check per Chapter 5 before installing at height.

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
