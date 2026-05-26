# Unit 1 — Theory of Operation
**[TM-TOOL-001 — Open Handout](../../antenna_tools/adf_systems/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the ADF SYSTEMS — ANTENNA DIRECTION FINDING; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-TOOL-001. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 Watson-Watt Method

Two orthogonal loop antennas (North-South and East-West) produce sine and
cosine voltage components of received signal amplitude as a function of bearing.
A third omnidirectional element (sense antenna) resolves the 180° ambiguity.

Bearing calculation:

bearing = atan2(V_NS, V_EW) (degrees, corrected for magnetic declination)
Each channel must be gain-matched to within ±0.5 dB; phase-matched to
within ±2° across the operating band. Gain/phase imbalance directly
adds to bearing error.

2-2 Doppler Method

An element sequentially commutated around a circle of diameter d causes a
frequency modulation on the received carrier equal to:

f_d = (v / λ) × cos(θ &minus; φ)
where v = element tangential velocity = πdf_rot, λ = wavelength,
θ = bearing to source, φ = current element angle. A PLL or IQ
discriminator extracts θ from the FM sidebands. Rotation rate
f_rot is typically 100–600 Hz; higher rates extend the capture range
but increase bandwidth requirements.

2-3 Butler Matrix (4-Element Switched Array)

Four elements in a square array with λ/4 spacing feed a passive Butler
Matrix beamforming network. The matrix produces four orthogonal beams at
±45° and ±135° simultaneously. RSSI comparison across the
four beam ports yields a bearing estimate; interpolation between adjacent
beam peaks resolves bearing to approximately 3° RMS.

The Butler Matrix is constructed from four 3 dB 90° hybrid couplers
and two fixed 45° phase shifters interconnected as a 4×4 passive network.
No active components in the RF path; loss is approximately 0.5–1.5 dB
depending on hybrid quality.

2-4 GPS Compass Integration

Magnetic bearing from an ADF system must be converted to true bearing for
navigation use. GPS compass integration provides:

- Vehicle/platform heading reference (eliminates mount alignment error)
- Magnetic declination correction (from GPS position + WMM model)
- True bearing output = ADF magnetic bearing &minus; declination + heading offset

---

## Why Theory Matters

You cannot use a measurement tool correctly without understanding how it works. Theory tells you:
- What the tool measures and how it converts the quantity to a readable output
- What the sources of error are — so you can recognize and minimize them
- What the valid operating range is — so you stay within its specifications
- How to interpret results that don't match expectations

If a measurement looks wrong, theory is where you look first.

---

## Self-Check Questions

**SC1-1.** In one sentence, state the operating principle of the ADF SYSTEMS — ANTENNA DIRECTION FINDING as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of measurement error or uncertainty?

**SC1-3.** What key specification(s) (accuracy, range, frequency coverage) does the TM state?

**SC1-4.** What does Chapter 2 say the ADF SYSTEMS — ANTENNA DIRECTION FINDING cannot do — what are its limitations?

**SC1-5.** List two formulas or relationships from Chapter 2 that govern the tool's operation.

---

## Answer Key

**SC1-1.** See TM §2-1. Compare your sentence to the first substantive paragraph of Chapter 2.

**SC1-2.** See Chapter 2. Look for language about error sources, accuracy limits, parasitic effects, or frequency dependence.

**SC1-3.** See Chapter 2. Look for numbers with units: %, ppm, Hz, Ω, dB, W.

**SC1-4.** See Chapter 2 and Chapter 1. Limitations are often stated as frequency range, power limits, or accuracy bounds.

**SC1-5.** See Chapter 2. Equations or proportionality statements are the relationships that govern the tool.

---

## Checkpoint

Before proceeding, state without looking:
- The operating principle of the ADF SYSTEMS — ANTENNA DIRECTION FINDING
- The primary error source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
