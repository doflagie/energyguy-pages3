# Unit 4 — Operating Procedures and Calculations
**[TM-GEAR-016 — Open Handout](../../../antenna_gear/rfi_mitigation/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Install and operate the RFI MITIGATION — IDENTIFICATION AND SUPPRESSION correctly; interpret performance data; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-GEAR-016. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

5-1 Noise Canceller Adjustment

1. Position the reference antenna to maximize noise pickup relative to signal:
 point it at the noise source (if known) or orient it to maximize S-meter
 noise level.
2. Enable the canceller. Adjust phase (φ) slowly through 0–360°
 while monitoring the noise level (S-meter or audio). A strong null
 will appear at the correct phase setting.
3. At the null, adjust amplitude (attenuation) for the deepest null.
 Alternate between phase and amplitude adjustments; a few iterations
 converge to the optimal setting.
4. Save settings for later use. Note: the optimal settings change if
 the noise source moves or changes character. Re-adjust if noise
 returns.

5-2 RFI Source Location with Sniffer Probe

1. Connect sniffer probe to TinySA. Set span to 1–30 MHz with
 10 kHz RBW. Look for peaks that correlate with the interference.
2. Move probe near suspected sources (switching power supplies, LED drivers,
 computer power bricks, solar charge controllers) until the signal peaks.
3. Identify the interference frequency and its harmonics. If the fundamental
 is a known utility frequency (switching PSU at 65 kHz, LED driver at
 120 Hz), that identifies the source.

---

## Appendix A — Reference Formulas

| Source | Typical frequency | Mitigation |
|---|---|---|
| Switching power supply | 50–500 kHz + harmonics | CMC on AC cord; ferrite on DC leads |
| LED driver | 100–500 kHz + harmonics | CMC; replace with linear driver |
| Solar MPPT charger | 50–200 kHz | CMC on battery leads; shielded enclosure |
| Plasma TV | Broadband HF + VHF | CMC on all cable TV/antenna leads |
| RF from own station | Operating frequency | Common-mode chokes at feedpoint |

---

## Key Formulas Summary

Key mathematical relationships from Appendix A:

*(See Appendix A in the TM)*

---

## Operating Notes

Chapter 5 specifies 7 operating steps.

Installation and operating discipline:
- Always verify polarity and orientation before making connections — RF transformers and baluns are phase-sensitive
- Route feedlines away from parallel conductors — parallel runs create mutual coupling that degrades isolation
- Ground all exposed metalwork at a single chassis point — multiple grounds create loops
- Record settings, frequencies, and power levels for every test — you need baseline data for comparisons

---

## Practice Problems

Work these before reading the answer key below.

**P4-1.** Using the operating procedure from Chapter 5 and the formulas from Appendix A:
State the installation steps you would take to put the RFI MITIGATION — IDENTIFICATION AND SUPPRESSION in service on a 40m (7.150 MHz) station.
List steps in order.

**P4-2.** From Chapter 5: what installation or setup detail produces the best RF performance
with the RFI MITIGATION — IDENTIFICATION AND SUPPRESSION? What is the tradeoff if you omit or shortcut that step?

**P4-3.** Chapter 5 specifies an operating procedure for a specific use case.
State the first three steps of that procedure from memory.

**P4-4.** Appendix A gives a formula for computing a result from measured values.
Pick one formula and compute a worked example using made-up but realistic values.
Show all work.

---

## Answer Key — Practice Problems

**P4-1.** Compare your list to Chapter 5. Steps should include: select mounting location → connect to feedline/antenna → verify polarity/orientation → apply power or signal → verify operation → record baseline.

**P4-2.** See Chapter 5. The most important installation detail is usually physical orientation, lead length, or ground bonding — the tradeoff if omitted is degraded isolation, increased SWR, or common-mode current leakage.

**P4-3.** See Chapter 5, steps 1–3. Copy exactly then close the TM and state from memory.

**P4-4.** See Appendix A for the formula. Your arithmetic is correct if your result has the right units and is physically plausible.

---

## Checkpoint

Before proceeding:
- [ ] You can state the operating procedure from memory (at least the first 5 steps)
- [ ] You can compute the derived quantity from Chapter 5 / Appendix A without looking
- [ ] You understand what a degraded or unexpected result tells you about the installation

**→ Proceed to [Unit 5](unit_5_calibration.md)**
