# Unit 3 — Assembly Procedures
**[TM-ANT-045 — Open Handout](../../../antennas/magnetic_loop_antennas/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly in the correct sequence; achieve the physical dimensions specified
**Estimated time:** 25 minutes

---

## Step 1: Read the TM

Open TM-ANT-045. Read **Chapter 4 — Assembly Procedures** completely.

Then come back here.

---

## Chapter 4 Content

> **WARNING:** HIGH VOLTAGE HAZARD: At 100 W output power the tuning capacitor of a magnetic loop antenna
develops 4,000–12,000 V across its terminals, depending on loop Q and operating frequency.
Use ONLY vacuum variable capacitors or high-voltage butterfly (split-stator) capacitors rated
for at least 5 kV RMS. Do NOT use DIY stacked aluminum-plate capacitors, rolled-foil capacitors,
or short lengths of coaxial cable as capacitors — these will arc and fail, causing equipment damage
and potential personal injury. Allow the capacitor to fully discharge before touching any part of
the loop structure after transmitting. Keep hands and metal objects away from the capacitor gap
during tuning.

1. Bend main loop tube to circular or square shape. Ensure all bends are smooth — kinks increase resistance.
2. Mount vacuum capacitor at top of loop. Solder or silver-braze all connections to minimum contact resistance. Verify <0.001 Ω across each joint with milli-ohmmeter.
3. Construct coupling loop as a circle of diameter 0.2× main loop. Mount coupling loop at the bottom of the main loop, centered and in the same plane.
4. Connect coax braid and center conductor to coupling loop terminals. No balun required — the coupling loop is inherently isolated.
5. Mount assembly on non-conductive support. Ensure no metal within 1 m of loop conductor.
6. Install capacitor drive mechanism with ≥10:1 reduction. Label rotation direction for capacitance increase.
7. Tag the capacitor: &ldquo;HIGH VOLTAGE — UP TO 12 kV AT 100W. DO NOT TOUCH WHEN TRANSMITTING.&rdquo;

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
