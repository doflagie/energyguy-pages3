# Unit 3 — Assembly and Setup
**[TM-TOOL-002 — Open Handout](../../../antenna_tools/antenna-analyzer/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-TOOL-002. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

4-1 PCB Layout Notes

Route the RF signal paths (DDS/PLL output → coupler → DUT port) as
50Ω microstrip (width 2.9 mm on 1.6 mm FR4 with εr=4.6).
Keep reference and reflected coupler outputs equal length to the AD8302 inputs.
Length mismatch >5 mm introduces a phase error of approximately
Δφ = 360° × ΔL / λ.

4-2 AD8302 Bias

The AD8302 requires ±5V dual supply. Derive from 5V USB with a
MAX1044 charge pump for the &minus;5V rail. Bypass each supply pin with
10 nF NP0 + 100 nF X5R within 3 mm of the device.

4-3 DDS Output Filtering

The AD9851 output contains harmonics and alias products. A 7-pole
elliptic low-pass filter (cutoff 35 MHz) reduces spurious outputs to
<&minus;60 dBc before the coupler. Without this filter, harmonic
reflections from the DUT appear as bearing errors at sub-harmonic frequencies.

---

## Assembly Quality

Work through all construction steps in Chapter 4 in sequence.

The assembly directly determines measurement quality. Common errors:
- RF leads too long — lead inductance raises SWR and limits high-frequency performance
- Cold solder joints on RF nodes — high resistance causes signal loss and intermittent readings
- Ground loops — multiple ground paths at different potentials cause noise and calibration errors
- Ferrite winding errors — wrong turn count or direction reverses transformer polarity

If Chapter 4 specifies a verification step after assembly (e.g., "verify DC resistance = X before proceeding"), do it. Those checks exist because they are the most common failure points.

---

## Self-Check Questions

**SC3-1.** How many assembly steps does Chapter 4 specify?

**SC3-2.** What is the first assembly step? State it exactly from the TM.

**SC3-3.** Does Chapter 4 specify maximum lead length anywhere? If so, what is the limit and why?

**SC3-4.** Does Chapter 4 require a bench verification after assembly? What does it check?

**SC3-5.** What would you do if a winding resistance measurement came out wrong during assembly verification?

---

## Answer Key

**SC3-1.** Count the numbered steps in Chapter 4.

**SC3-2.** See Chapter 4, step 1. Copy it exactly.

**SC3-3.** RF lead length limits are typically 10–15 mm for HF circuits. Longer leads add ~1–2 nH per mm, raising inductive reactance at high frequencies.

**SC3-4.** Scan Chapter 4 for verification steps. Common checks: DC resistance, winding balance, null depth on test signal.

**SC3-5.** Stop assembly. Diagnose before proceeding — a winding error found before completion is much easier to fix than one discovered after the unit is boxed.

---

## Checkpoint

Before proceeding:
- [ ] You have read Chapter 4 completely
- [ ] You can state the number of assembly steps and the first and last steps
- [ ] You understand how assembly quality affects measurement accuracy

**→ Proceed to [Unit 4](unit_4_operation.md)**
