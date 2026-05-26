# Unit 2 — Materials and Construction
**[TM-ANT-044 — Open Handout](../../antennas/log_periodic_dipole_arrays_for_wideband_operation/tm.html)**
**TM Chapter:** Chapter 3
**ELOs:** Identify and gather all required materials; understand why each material is specified
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-ANT-044. Read **Chapter 3 — Materials and Construction** completely.

Then come back here.

---

## Chapter 3 Content

### 3-1. BILL OF MATERIALS

| Qty | Item | Specification |
|---|---|---|
| 1 | Boom | 6061-T6 aluminum tubing, OD 1–1.5 in; length per design; non-conductive for VHF if elements through-bolted |
| Per design | Element stock | 3/16–1/4 in aluminum rod for VHF; tape measure steel for portable UHF |
| 1 | Driven element | Folded or single dipole; see matching section below |
| 1 | Gamma match assembly | 3/16 in rod, 6–10 in from center; 10–50 pF trimmer; beta match alternative |
| 1 | SO-239 or N-type connector | Weatherproof; mount at boom junction |

### 3-2. ELEMENT DIMENSION FORMULAS

```
Driven element half-length (in) for VHF
Lde = 5616 / fMHz (inches, total)
```

```
Reflector length (in)
Lref = Lde × 1.05
```

```
Director length (in)
Ldir = Lde × 0.95 (first director; adjust per NEC model)
```

```
Refl-to-driven spacing
0.2λ = 0.2 × (11811/fMHz) mm
```

```
Driven-to-director spacing
0.25λ = 0.25 × (11811/fMHz) mm
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
