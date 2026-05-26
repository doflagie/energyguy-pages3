# Unit 2 — Materials and Construction
**[TM-ANT-054 — Open Handout](../../antennas/portable_magnetic_loop/tm.html)**
**TM Chapter:** Chapter 3
**ELOs:** Identify and gather all required materials; understand why each material is specified
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-ANT-054. Read **Chapter 3 — Materials and Construction** completely.

Then come back here.

---

## Chapter 3 Content

### 3-1. BILL OF MATERIALS

| Qty | Item | Specification |
|---|---|---|
| 1 length | Main loop conductor | Copper or aluminum tubing 1–1.5 in OD; circumference per design frequency |
| 1 | Vacuum variable capacitor | Rated ≥5 kV RMS, 10–500 pF range; Jennings, Cardwell, or equivalent |
| 1 | Coupling loop | Diameter 0.2× main loop; #12 AWG copper; used for 50 Ω feed |
| 1 | Capacitor drive mechanism | Reduction drive ≥10:1 for fine tuning; motor drive for remote operation |
| 1 | Support frame | Non-conductive (PVC, wood, fiberglass); must resist loop torque |
| 1 | SO-239 connector | For coupling loop feed; coax shield bonds to coupling loop |

> **WARNING:** HIGH VOLTAGE HAZARD: At 100 W output power the tuning capacitor of a magnetic loop antenna
develops 4,000–12,000 V across its terminals, depending on loop Q and operating frequency.
Use ONLY vacuum variable capacitors or high-voltage butterfly (split-stator) capacitors rated
for at least 5 kV RMS. Do NOT use DIY stacked aluminum-plate capacitors, rolled-foil capacitors,
or short lengths of coaxial cable as capacitors — these will arc and fail, causing equipment damage
and potential personal injury. Allow the capacitor to fully discharge before touching any part of
the loop structure after transmitting. Keep hands and metal objects away from the capacitor gap
during tuning.

### 3-2. DIMENSION FORMULAS

```
Loop inductance (circular loop, μH)
L = 0.002 × D × (ln(4D/d) &minus; 2) where D = loop diameter (cm), d = conductor OD (cm)
```

```
Resonant capacitance (μF)
C = 1 / (4π² × f² × L)
```

```
Capacitor voltage at resonance (V peak)
VC = &radic;(2 × P × Q² × Rr) where P = transmit power (W), Q = loop Q factor
```

```
Example: 100 W, Q=300, R_r=0.08 Ω
VC ≈ 8,500 V peak — REQUIRES VACUUM CAPACITOR
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
