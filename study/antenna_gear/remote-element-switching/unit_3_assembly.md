# Unit 3 — Assembly and Setup
**[TM-GEAR-014 — Open Handout](../../antenna_gear/remote-element-switching/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-014. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

4-1 PIN Diode Element Switch (70 cm)

1. Mount MA4P504 diode on a small PCB (10×10 mm) at the
 element attachment point on the boom. Orient cathode toward the
 bias supply, anode toward the element.
2. Wire the RF choke (47 nH SMD) in series between the bias supply
 conductor and the cathode. This prevents RF from entering the bias circuit.
3. Connect 100 pF NP0 capacitor in series with the RF path (between
 boom and element) to block DC while passing RF.
4. Run a twisted pair (bias supply and return) along the boom to the
 controller box. Use shielded twisted pair for runs >300 mm.

4-2 Relay Sequencer

1. Connect Relay 1 (preamp bypass) to ESP32 GPIO with a 2N2222 driver.
 Relay 2 (TX/RX antenna relay) to a second GPIO + driver.
2. On PTT input (low = key): fire Relay 1 (bypass preamp) after 10 ms
 delay; fire Relay 2 (TX relay) after another 20 ms delay.
 Then assert PA key output.
3. On PTT release: de-assert PA key; wait 20 ms; release Relay 2;
 wait 10 ms; release Relay 1 (reconnect preamp).

---

## Assembly Quality

Chapter 4 specifies 7 construction/assembly steps.

The assembly directly determines RF performance. Common errors:
- RF leads too long — lead inductance raises SWR and limits high-frequency performance
- Cold solder joints on RF nodes — high resistance causes signal loss and intermittent behavior
- Ground loops — multiple ground paths at different potentials cause noise and calibration errors
- Ferrite winding errors — wrong turn count or direction reverses transformer polarity or changes impedance ratio
- Incorrect winding direction on toroidal transformers — affects phase and common-mode rejection

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

**SC3-4.** Scan Chapter 4 for verification steps. Common checks: DC resistance, winding balance, null depth on test signal, impedance ratio.

**SC3-5.** Stop assembly. Diagnose before proceeding — a winding error found before completion is much easier to fix than one discovered after the unit is boxed.

---

## Checkpoint

Before proceeding:
- [ ] You have read Chapter 4 completely
- [ ] You can state the number of assembly steps and the first and last steps
- [ ] You understand how assembly quality affects RF performance

**→ Proceed to [Unit 4](unit_4_operation.md)**
