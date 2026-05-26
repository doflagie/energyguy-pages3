# Unit 2 — Construction and Materials
**[TM-GEAR-003 — Open Handout](../../../antenna_gear/baluns-and-ununs/tm.html)**
**TM Chapter:** Chapter 3
**ELOs:** Identify required components and materials; understand component selection criteria
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-003. Read **Chapter 3 — Equipment and Materials** completely.

Then come back here.

---

## Chapter 3 Content

| Design | Core | Wire | Turns | Freq range |
|---|---|---|---|---|
| 1:1 current (choke) | FT-240-31 | RG-303 coax, 12 passes | 12 | 1.8–30 MHz |
| 4:1 Guanella | 2× FT-140-43 | #16 bifilar, 8 turns | 8 | 1.8–30 MHz |
| 6:1 trifilar | FT-140-43 | #18 trifilar, 10 turns | 10 | 3.5–30 MHz |
| 9:1 trifilar | FT-240-43 | #16 trifilar, 9 turns | 9 | 1.8–30 MHz |
| 1:1 VHF/UHF air-core | None (air core) | RG-58 coax, 6 turns | 6 | 50–450 MHz |

---

## Component Selection

Review the equipment and materials list in Chapter 3 carefully.

Before building, verify every item on the materials list. Key considerations:
- Use the specified component types — substitutions may affect performance or frequency coverage
- Non-inductive components are required in RF circuits; standard wirewound resistors are unsuitable
- Toroids and ferrite cores are frequency-specific; use the specified core material (#43, #61, #67, etc.)
- Connector types affect impedance — match the specified connector to avoid SWR errors
- Wire gauge and insulation type affect current capacity and voltage breakdown

The quality of your materials sets the ceiling on the component's performance.

---

## Self-Check Questions

**SC2-1.** List the three most critical components or materials specified in Chapter 3. Why are they critical?

**SC2-2.** Does Chapter 3 specify non-inductive resistors? If so, where are they used and why does it matter?

**SC2-3.** What connector type(s) does Chapter 3 specify, and what is the frequency/power justification?

**SC2-4.** Does Chapter 3 specify a particular ferrite core material or type? What is its significance?

**SC2-5.** What would be the consequence of substituting a standard wirewound resistor for a non-inductive type in an RF application?

---

## Answer Key

**SC2-1.** See Chapter 3. Identify items with specific part numbers, special materials, or critical tolerances — these are the ones that most affect performance.

**SC2-2.** See Chapter 3. Non-inductive types are used wherever standard inductive wirewound resistors would add series inductance that degrades high-frequency performance.

**SC2-3.** See Chapter 3. SO-239, N-type, BNC, and SMA each have different frequency and power ratings.

**SC2-4.** See Chapter 3. Ferrite #43 is for 1–100 MHz; #61 for 10–200 MHz; #67 for 50–500 MHz. Wrong material = wrong permeability = wrong coupling or suppression.

**SC2-5.** A wirewound resistor has several microhenries of series inductance. At HF and VHF, this adds inductive reactance proportional to frequency, destroying the resistor's value as a termination or load.

---

## Checkpoint

Before proceeding:
- [ ] You have read Chapter 3 completely
- [ ] You can name the critical components from memory
- [ ] You understand why non-inductive and correct ferrite materials are required

**→ Proceed to [Unit 3](unit_3_assembly.md)**
