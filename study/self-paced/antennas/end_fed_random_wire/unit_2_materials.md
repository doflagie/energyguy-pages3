# Unit 2 — Materials and Construction
**[TM-ANT-021 — Open Handout](../../../antennas/end_fed_random_wire/tm.html)**
**TM Chapter:** Chapter 3
**ELOs:** Identify and gather all required materials; understand why each material is specified
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-ANT-021. Read **Chapter 3 — Materials and Construction** completely.

Then come back here.

---

## Chapter 3 Content

### 3-1. BILL OF MATERIALS

| Qty | Item | Specification |
|---|---|---|
| 1 | Matching transformer (unun) | 49:1 unun for EFHW (binocular core BN-43-2402 or FT-140-43); 9:1 unun for random wire (FT-140-43) |
| 1 length | Antenna wire | #22–#18 AWG stranded copper; length per design |
| 1 | End insulator | UV-stable; rated for wire tension |
| 1 | Counterpoise wire | #22 AWG; minimum 0.05λ at lowest operating frequency |
| 1 | Enclosure for transformer | UV-stable ABS; seal with silicone for waterproofing |
| 1 | SO-239 connector | Mount on enclosure; connect to coax feed line |

### 3-2. WIRE LENGTHS

```
EFHW wire length (feet) at design frequency
L = 468 / fMHz
```

```
EFQW wire length (feet)
L = 234 / fMHz
```

```
Avoid resonant random wire lengths (avoid these values for 40M EFRW)
Avoid 11, 22, 33, 44, 66, 88, 110 ft — these are resonant multiples on common bands
```

```
Recommended EFRW lengths (ft)
29, 35.5, 41, 58, 71 — not resonant on any common HF band
```

---

## Material Selection

Review the full materials list in Chapter 3.

Before building, verify every item on the materials list. Key considerations:
- Wire gauge affects conductor resistance and therefore efficiency — use the specified gauge or heavier
- Coax type affects velocity factor, which changes electrical length vs. physical length
- Ferrite core type (#43, #61, #67) is frequency-specific for matching transformers and chokes
- Connector types affect impedance continuity — SO-239, N, BNC, and SMA each have frequency limits
- Insulator material and placement affect wind loading and UV degradation

The materials list in Chapter 3 is the bill of materials for a tested, working antenna. Substitutions must be evaluated, not assumed equivalent.

---

## Self-Check Questions

**SC2-1.** List the three most critical materials or components specified in Chapter 3. Why are they critical?

**SC2-2.** What wire gauge does Chapter 3 specify? What is the consequence of using a smaller gauge?

**SC2-3.** If Chapter 3 specifies coaxial cable, what type is called for? What is its velocity factor?

**SC2-4.** Does Chapter 3 specify a particular ferrite or core type? What is its application in this antenna?

**SC2-5.** What is the consequence of using a connector type outside its rated frequency range?

---

## Answer Key

**SC2-1.** See Chapter 3. Identify items with specific part numbers, gauge specifications, or where substitution would change resonant frequency or impedance.

**SC2-2.** See Chapter 3. Smaller gauge = higher conductor resistance = lower efficiency and greater I²R losses.

**SC2-3.** See Chapter 3. Common types: RG-58 (0.66 VF), RG-8X (0.84), LMR-400 (0.85). Velocity factor changes electrical length: physical length = λ × VF.

**SC2-4.** See Chapter 3. Type #43 ferrite is general HF (1–50 MHz); #61 for upper HF/VHF; #67 for VHF. Wrong material means wrong impedance ratio at the operating frequency.

**SC2-5.** A connector used beyond its frequency rating introduces unpredictable impedance discontinuities, increasing reflected power and SWR.

---

## Checkpoint

Before proceeding:
- [ ] You have read Chapter 3 completely
- [ ] You can name the critical materials from memory
- [ ] You have sourced or confirmed availability of all required materials

**→ Proceed to [Unit 3](unit_3_assembly.md)**
