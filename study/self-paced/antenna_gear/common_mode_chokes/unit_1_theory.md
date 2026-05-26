# Unit 1 — Theory of Operation
**[TM-GEAR-007 — Open Handout](../../../antenna_gear/common_mode_chokes/tm.html)**
**TM Chapter:** Chapter 2
**ELOs:** Understand the operating principle of the COMMON-MODE CHOKES — RFI SUPPRESSION ON FEEDLINES; identify key specifications
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-GEAR-007. Read **Chapter 2 — Theory of Operation** completely.

Then come back here.

---

## Chapter 2 Content

2-1 Differential vs. Common Mode

In a coaxial feedline, the desired signal travels as a differential mode:
equal and opposite currents on center conductor and inner surface of shield.
The core sees equal and opposite H-fields; they cancel. The core presents
zero impedance to this mode.

Common-mode current flows on the outside of the shield. The core sees
an unbalanced H-field and presents the full choking impedance Z_choke
(series impedance) in the common-mode path. Result:

CMR (dB) = 20 × log10(1 + Z_choke / Z_path)
Z_path = impedance to which common-mode current flows (typically 50–300Ω)

For Z_choke = 5000Ω, Z_path = 100Ω: CMR = 34 dB
For Z_choke = 1000Ω, Z_path = 100Ω: CMR = 21 dB
2-2 Core Material vs. Frequency
Core material determines the frequency range of effective choking:

| Material | Best range | Peak μ" | Application |
|---|---|---|---|
| #31 | 1–300 MHz | 3000 | HF antenna feedpoints, coax chokes |
| #43 | 1–100 MHz | 750 | HF balun windings, TLTs |
| #61 | 10–300 MHz | 125 | VHF chokes, 6M and 2M feedpoints |
| #73 | 1–40 MHz | 2500 | HF snap-on beads, power line chokes |
| #75 | 0.5–10 MHz | 5000 | Audio and low-HF chokes |

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

**SC1-1.** In one sentence, state the operating principle of the COMMON-MODE CHOKES — RFI SUPPRESSION ON FEEDLINES as described in Chapter 2.

**SC1-2.** What does Chapter 2 identify as the primary source(s) of loss or degradation in performance?

**SC1-3.** What key specification(s) (frequency range, power rating, insertion loss, impedance ratio) does the TM state?

**SC1-4.** What does Chapter 2 say the COMMON-MODE CHOKES — RFI SUPPRESSION ON FEEDLINES cannot do — what are its limitations?

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
- The operating principle of the COMMON-MODE CHOKES — RFI SUPPRESSION ON FEEDLINES
- The primary loss or degradation source(s)
- At least one key specification with its value

**→ Proceed to [Unit 2](unit_2_construction.md)**
