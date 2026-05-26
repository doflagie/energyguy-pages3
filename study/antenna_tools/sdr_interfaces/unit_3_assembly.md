# Unit 3 — Assembly and Setup
**[TM-TOOL-009 — Open Handout](../../antenna_tools/sdr_interfaces/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-TOOL-009. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

4-1 Stacking Order

Connect SDR interface modules in this order from antenna to SDR:

Antenna → Bias-T (if using remote LNA) → Preselector →
Attenuator/LNA (optional) → HF Upconverter (HF only) → RTL-SDR
4-2 Preselector Band Selection
The ESP32 selects the correct filter automatically if configured with
the current receive frequency. Manual override: set GPIO[3:0] to the
filter index (0–15) using the CYD touchscreen or serial command
`BAND `.

4-3 Upconverter LO Alignment

The RTL-SDR must be told to offset the displayed frequency by the LO
value. In SDR# or GQRX: set &ldquo;LO offset&rdquo; or &ldquo;frequency correction&rdquo;
to +125.000 MHz. In GNU Radio: subtract 125e6 from the reported
frequency to get the true HF frequency.

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
