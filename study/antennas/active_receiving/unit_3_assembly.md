# Unit 3 — Assembly Procedures
**[TM-ANT-009 — Open Handout](../../antennas/active_receiving/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly in the correct sequence; achieve the physical dimensions specified
**Estimated time:** 25 minutes

---

## Step 1: Read the TM

Open TM-ANT-009. Read **Chapter 4 — Assembly Procedures** completely.

Then come back here.

---

## Chapter 4 Content

1. Deploy antenna wire from feed end in desired direction of maximum received signal. For Beverage: wire runs toward the desired signal direction. For K9AY: loop lies in vertical plane; null is broadside to loop axis.
2. Install termination resistor enclosure at far end of wire. Terminate wire to top of resistor; bottom of resistor to ground stake. Verify resistor value with ohmmeter before sealing enclosure.
3. Install feed transformer at near end. Wire terminal to antenna; ground to earth stake (Beverage: ground at feed end). Connect output to coax.
4. Route coax toward receiver. Ensure at least 5 m horizontal separation from any transmitting antenna to avoid overloading the receive preamp during transmit.
5. At receiver end, install T/R relay or receive antenna switch for TX/RX switching (this antenna must be disconnected from receiver before transmitting).

---

## Assembly Quality

Chapter 4 specifies 5 assembly steps.

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
