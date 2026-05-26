# Unit 2 — Materials and Construction
**[TM-ANT-038 — Open Handout](../../../antennas/inverted_vee_dipole/tm.html)**
**TM Chapter:** Chapter 3
**ELOs:** Identify and gather all required materials; understand why each material is specified
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-ANT-038. Read **Chapter 3 — Materials and Construction** completely.

Then come back here.

---

## Chapter 3 Content

### 3-1. BILL OF MATERIALS

| Qty | Item | Specification |
|---|---|---|
| Per design | Copper wire (antenna elements) | #14 AWG solid or stranded copper, PVC-jacketed preferred for weather resistance |
| 1 | Feed-point insulator / center connector | SO-239 or UHF-F, weatherproof, UV-resistant housing |
| 2 | End insulators | Egg insulators or equivalent; rated for wire tension at operating temperature |
| As needed | Halyard / support rope | Dacron or polypropylene, non-conductive, UV-resistant |
| 1 | Choke balun (1:1 current balun) | Mix-31 ferrite cores, 8–12 turns of RG-142 or RG-8X through core; FB-31-6873 or equivalent |
| 1 | Coaxial feed line | RG-8X or RG-213 to station; minimize length for efficiency |

### 3-2. DIMENSION FORMULAS

```
Half-wave dipole element length (each arm, feet)
Larm = 234 / fMHz
```

```
Full dipole length (feet)
Ltotal = 468 / fMHz
```

```
Example: 40M at 7.150 MHz
Larm = 234 / 7.150 = 32.7 ft — each arm
```

Note: The 468 constant assumes wire of #12–#14 AWG copper at ambient temperature; adjust down 1–2% for thick conductors or cage construction (velocity factor <1).

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
