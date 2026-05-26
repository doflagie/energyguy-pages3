# Unit 4 — Operating Procedures and Calculations
**[TM-GEAR-018 — Open Handout](../../antenna_gear/transmatch/tm.html)**
**TM Chapters:** Chapter 5, Appendix A
**ELOs:** Install and operate the TRANSMATCH — PORTABLE ANTENNA TUNER correctly; interpret performance data; compute derived quantities
**Estimated time:** 30 minutes (includes 3–4 practice problems)

---

## Step 1: Read the TM

Open TM-GEAR-018. Read **Chapter 5 — Operating Procedures** and **Appendix A** completely.

Then come back here.

---

## Chapter 5 Content

5-1 Initial Tuning on a New Band

1. Set C1 and C2 to midscale. Set L to minimum (roller at bottom).
2. Key the transmitter at 5 W (reduced power protects transistors during
 tuning on a badly mismatched antenna).
3. Watch the SWR indicator. Adjust L for minimum reflected LED; then
 adjust C1 and C2 alternately for minimum reflected, returning to L
 adjustment if a new minimum appears.
4. Iterate until reflected indicator is at minimum and forward indicator
 shows full power. Typical tuning time: 30–60 seconds.
5. Increase to full operating power. Verify SWR indicator does not change.

5-2 Band-to-Band Switching

1. Record successful L, C1, C2 positions for each band on the band card
 (laminated chart in the lid of the tuner).
2. On band change: set L, C1, C2 to the recorded positions; tune briefly
 at 5W to confirm the antenna has not changed.

---

## Appendix A — Reference Formulas

| Band | Typical C1, C2 range (pF) | Typical L range (&micro;H) |
|---|---|---|
| 160M (1.8 MHz) | 200–365 pF | 15–28 &micro;H |
| 80M (3.75 MHz) | 100–300 pF | 8–20 &micro;H |
| 40M (7.15 MHz) | 50–200 pF | 4–12 &micro;H |
| 20M (14.175 MHz) | 20–100 pF | 2–6 &micro;H |
| 10M (28.5 MHz) | 10–50 pF | 0.5–3 &micro;H |

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
State the installation steps you would take to put the TRANSMATCH — PORTABLE ANTENNA TUNER in service on a 40m (7.150 MHz) station.
List steps in order.

**P4-2.** From Chapter 5: what installation or setup detail produces the best RF performance
with the TRANSMATCH — PORTABLE ANTENNA TUNER? What is the tradeoff if you omit or shortcut that step?

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
