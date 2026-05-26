# Unit 1 — Theory of Operation
**[TM-CAL-008 — Open Handout](../../../Calibration_Ideas/precision_resistor_network/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand how the reference standard generates or maintains its reference value; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-CAL-008. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

### 2-1. RESISTOR TYPES

For precision standards, use bulk metal foil (BMF) or metal film resistors. Wire-wound resistors are excellent for DC but introduce inductance at AC. Carbon film and carbon composition types are inadequate for calibration use due to high TC and poor long-term stability.

| Type | Tolerance | TC (ppm/°C) | Stability | Notes |
|---|---|---|---|---|
| Bulk metal foil (BMF) | 0.01% | 0.2–2 | Excellent | Vishay Z201, best choice |
| Metal film | 0.1% | 10–25 | Good | Yageo MFR, Multicomp |
| Wirewound | 0.1% | 5–10 | Excellent DC | Inductive at AC |
| Thick film (SMD) | 1% | 100–200 | Fair | Not suitable |
| Carbon film | 5% | 200–500 | Poor | Do not use |

### 2-2. 4-WIRE KELVIN CONNECTIONS

For resistances below 100 Ω, contact resistance in connectors and switch contacts introduces significant error. Use 4-wire (Kelvin) connections: separate current-force and voltage-sense terminals for each resistor position. See TM-CAL-010 for full Kelvin procedure.

---

## Why Theory Matters for Calibration

You cannot calibrate what you do not understand. Theory tells you:
- What the instrument or standard is supposed to do — so you recognize when it is not doing it
- What the sources of error are — so you know which ones your calibration procedure addresses
- What the limits of the calibration are — so you know when to stop and call it good

If a calibration measurement produces a surprising result, theory is where you look first.

---

## Self-Check Questions

**SC1-1.** In one sentence, state the operating principle of the PRECISION RESISTOR NETWORK (DECADE BOX) as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of measurement error or uncertainty?

**SC1-3.** What key specification(s) (accuracy, range, resolution) does the TM state for this standard?

**SC1-4.** What safety precaution does §1-3 specify? State it exactly.

**SC1-5.** What references does §1-2 cite? List at least two.

---

## Answer Key

**SC1-1.** See TM §2-1. Compare your sentence to the first substantive paragraph of Chapter 2.

**SC1-2.** See Chapter 2. Look for language about error sources, uncertainty, drift, or limiting factors.

**SC1-3.** See Chapter 2. Look for numbers with units: %, ppm, Hz, Ω, dB.

**SC1-4.** See TM §1-3. Copy the safety text exactly.

**SC1-5.** See TM §1-2. The references list is there.

---

## Checkpoint

Before proceeding, state without looking:
- The operating principle of the PRECISION RESISTOR NETWORK (DECADE BOX)
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_equipment.md)**
