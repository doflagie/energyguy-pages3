# Unit 3 — Assembly and Setup
**[TM-TOOL-003 — Open Handout](../../antenna_tools/antenna_modeling/tm.html)**
**TM Chapter:** Chapter 4
**ELOs:** Execute assembly steps in the correct sequence; verify build quality before operation
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-TOOL-003. Read **Chapter 4 — Construction and Assembly** completely.

Then come back here.

---

## Chapter 4 Content

4-1 Dipole Example

from nec_generator import Dipole, NECModel
ant = Dipole(freq_mhz=14.25, height_m=10.0)
model = ant.to_nec_model()
model.write("dipole_20m.nec")
This generates a half-wave dipole resonant at 14.25 MHz, 10 m above real
ground (εr=13, σ=0.005 S/m). The GW card for each half-element
uses `recommended_segments()` to set segment count automatically.

4-2 Vertical with Radials
from nec_generator import VerticalWithRadials
ant = VerticalWithRadials(freq_mhz=7.1, n_radials=32, radial_length_m=10.0,
                          height_m=0.1)
model = ant.to_nec_model()
model.write("vertical_40m.nec")
4-3 Frequency Sweep
from freq_sweep import FrequencySweep
sweep = FrequencySweep(model, start_mhz=1.8, stop_mhz=30.0, n_steps=200)
results = sweep.run()
sweep.plot_impedance(results)
sweep.plot_swr(results, z0=50.0)

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
