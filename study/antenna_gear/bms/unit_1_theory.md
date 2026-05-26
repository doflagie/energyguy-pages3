# Unit 1 — Theory of Operation
**[TM-GEAR-004 — Open Handout](../../antenna_gear/bms/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the BATTERY MANAGEMENT SYSTEM (BMS) — LiFePO4 AND LI-ION; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-004. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 LiFePO4 Cell Parameters

LiFePO4 (lithium iron phosphate) cells have a flat discharge curve from
approximately 3.05V to 3.30V, covering 20%–80% SOC. Key voltage limits:

| Condition | Single cell | 4S pack |
|---|---|---|
| Full charge cutoff | 3.60V | 14.40V |
| Nominal voltage | 3.20V | 12.80V |
| Discharge cutoff | 2.50V | 10.00V |
| OV protection trip | 3.65V | 14.60V |
| UV protection trip | 2.50V | 10.00V |

2-2 BQ76920 Protection IC

The BQ76920 monitors each cell individually via its internal 14-bit ADC
(1.22 mV resolution, 1% accuracy). It drives separate CHG (charge)
and DSG (discharge) MOSFET outputs for independent control of charging and
discharging. Communication via I2C allows the ESP32 host to read cell
voltages, set protection thresholds, and enable/disable balancing FETs.

2-3 Cell Balancing

Passive balancing dissipates excess energy from the highest-voltage cells
via internal 10 A balancing FETs (10Ω resistors, ~130 mW
per cell at 3.6V). Active balancing transfers charge from high cells to low
cells using a flyback or capacitor-based converter; more efficient but more
complex. Passive balancing is adequate for matched cells with <50 mV
initial spread.

---

## Why Theory Matters

You cannot build or use RF gear correctly without understanding how it works. Theory tells you:
- What the component does and how it produces that effect
- What the sources of loss, distortion, or error are — so you can recognize and minimize them
- What the valid operating range is — frequency, power, impedance — so you stay within specifications
- How to interpret results or system behavior that doesn't match expectations

If a component doesn't perform as expected, theory is where you look first.

---

## Self-Check Questions

**SC1-1.** In one sentence, state the operating principle of the BATTERY MANAGEMENT SYSTEM (BMS) — LiFePO4 AND LI-ION as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of loss or degradation in performance?

**SC1-3.** What key specification(s) (frequency range, power rating, insertion loss, impedance ratio) does the TM state?

**SC1-4.** What does Chapter 2 say the BATTERY MANAGEMENT SYSTEM (BMS) — LiFePO4 AND LI-ION cannot do — what are its limitations?

**SC1-5.** List two formulas or relationships from Chapter 2 that govern the component's behavior.

---

## Answer Key

**SC1-1.** See TM §2-1. Compare your sentence to the first substantive paragraph of Chapter 2.

**SC1-2.** See Chapter 2. Look for language about loss mechanisms, parasitic effects, frequency limits, or power constraints.

**SC1-3.** See Chapter 2. Look for numbers with units: %, dB, Hz, Ω, W, V.

**SC1-4.** See Chapter 2 and Chapter 1. Limitations are often stated as frequency range, power handling, or impedance range.

**SC1-5.** See Chapter 2. Equations or proportionality statements are the relationships that govern the component.

---

## Checkpoint

Before proceeding, state without looking:
- The operating principle of the BATTERY MANAGEMENT SYSTEM (BMS) — LiFePO4 AND LI-ION
- The primary loss or degradation source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
