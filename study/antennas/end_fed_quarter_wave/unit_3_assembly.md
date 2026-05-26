# Unit 3 — Assembly Procedures
**[TM-ANT-020 — Open Handout](../../antennas/end_fed_quarter_wave/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly in the correct sequence; achieve the physical dimensions specified
**Estimated time:** 25 minutes

---

## Step 1: Read the TM

Open TM-ANT-020. Read **Chapter 4 — Assembly Procedures** completely.

Then come back here.

---

## Chapter 4 Content

> **CAUTION — RF EXPOSURE:** Maintain minimum safe distance from all energized antenna elements during transmission.
At QRP power levels (≤5 W) the MPE boundary is typically

1. Wind matching transformer per design (49:1 unun: 3 trifilar turns primary on BN-43-2402 binocular core, or 2 turns primary and 14 turns secondary on FT-140-43 single core).
2. Seal transformer in weatherproof enclosure. Route wire terminal out one end, coax SO-239 on side.
3. Attach antenna wire to transformer wire terminal. Cut wire 5% long for trimming.
4. Attach counterpoise wire (0.05λ minimum) to transformer ground/coax-braid terminal. Tape counterpoise away from antenna wire.
5. Deploy antenna wire: raise far end as high as possible, at least 20 ft for HF operation. Attach end insulator and support halyard.
6. Run coax from SO-239 to station. Install common-mode choke (Mix-31 toroid, 8 turns) at shack entry.
7. Perform SWR sweep per Chapter 5.

---

## Assembly Quality

Chapter 4 specifies 7 assembly steps.

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
