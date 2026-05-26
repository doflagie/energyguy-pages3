# Final Assessment — TERMINATED FOLDED DIPOLE (TFD) ANTENNA
**[TM-ANT-066 — Open Handout](../../antennas/terminated_folded_dipole/tm.html)**
**Course:** TM-ANT-066
**Reference:** TM-ANT-066 handout
**Questions:** 15
**Passing score:** 13/15 (87%). All calculation questions (Q6–Q9) must be correct.
**Time limit:** 20 minutes
**Notes:** Closed handout. No references during the assessment.

---

## Questions

**Q1.** What is the operating principle of the TERMINATED FOLDED DIPOLE (TFD) ANTENNA?
State it in one sentence using technical terms.

**Q2.** Folded dipole with 600 ω resistors terminating both ends; traveling-wave operation gives flat swr across wide frequency 
Explain the significance of this principle for antenna performance.

**Q3.** What determines the resonant frequency of the TERMINATED FOLDED DIPOLE (TFD) ANTENNA?
Name the primary physical parameter(s) and state whether increasing that parameter raises or lowers resonant frequency.

**Q4.** Chapter 3 specifies particular materials for the TERMINATED FOLDED DIPOLE (TFD) ANTENNA.
Name two materials where a substitution would change RF performance and explain the effect.

**Q5.** What is the first step of the Chapter 4 assembly procedure?
Why must the steps be performed in the specified order?

**Q6.** [Calculation] Using the formula from Appendix A (`Larm = 234 / fMHz`):
Compute the element length for a frequency 10% higher than the design frequency.
Show all work and state the result with units.

**Q7.** [Calculation] Using the formula: `Ladj = Larm × Vf (Vf ≈ 0.95 for #14 AWG PVC)`
Calculate a result for a specific antenna scenario. Show all work.

**Q8.** [Calculation] Your NanoVNA shows minimum SWR at 7.350 MHz.
The design frequency is 7.150 MHz. Compute the percentage deviation and the required length correction.
Show all work.

**Q9.** [Calculation] Compute the reflected power percentage for SWR = 2.5:1.
Formula: RP(%) = ((SWR − 1)/(SWR + 1))² × 100. Show all work.

**Q10.** During Chapter 5 calibration, your SWR at the design frequency is 3.8:1.
What does Chapter 6 direct you to do? What adjustment direction is required if resonance is above the design frequency?

**Q11.** What is the SWR acceptance criterion stated in Chapter 7 for the TERMINATED FOLDED DIPOLE (TFD) ANTENNA?
State it exactly: value and condition.

**Q12.** What is the difference between the Chapter 6 tuning procedure and the
Chapter 7 verification procedure? Which permits adjustments?

**Q13.** During Chapter 7 verification, SWR measures 2.6:1. You re-trim the element
and re-measure: SWR is now 1.7:1. Is the verification valid? What is the correct action?

**Q14.** List four required items in a verification log entry per Chapter 7.

**Q15.** After completing Chapter 7 verification with all checks passing, what can you
conclude about the TERMINATED FOLDED DIPOLE (TFD) ANTENNA? What can you NOT conclude?

---

*— Turn page for answer key —*

---

&nbsp;

---

&nbsp;

---

&nbsp;

---

## Answer Key

*All answers directly verifiable from TM-ANT-066.*

**A1.** See TM §2-1. The answer is the first substantive statement of Chapter 2.
*TM ref: §2-1*

**A2.** See TM §2-1. The significance is stated in Chapter 2, usually in terms of bandwidth, efficiency, or pattern.
*TM ref: §2-1*

**A3.** See Chapter 2. For wire antennas, physical length is primary. Loading elements (coils, capacitors) change effective electrical length. Longer = lower resonant frequency.
*TM ref: Ch. 2, App. A*

**A4.** See Chapter 3. Critical materials include wire gauge (affects conductor loss), coax type (velocity factor changes matching section length), and ferrite type (affects impedance transformation at frequency).
*TM ref: Ch. 3*

**A5.** See Chapter 4, step 1. The sequence exists because each step has physical or electrical dependencies on prior steps.
*TM ref: Ch. 4*

**A6.** Formula: `Larm = 234 / fMHz`. Frequency 10% higher = element 10% shorter. Confirm your result is physically plausible (shorter wire = higher frequency).
*TM ref: App. A*

**A7.** Formula: `Ladj = Larm × Vf (Vf ≈ 0.95 for #14 AWG PVC)`. Show substitution, arithmetic, result with units.
*TM ref: App. A*

**A8.** Deviation = (7.350 − 7.150) / 7.150 × 100 = 2.8% above design frequency. Antenna is electrically short. Required correction: lengthen element by approximately 2.8%.
*TM ref: App. A, Ch. 6*

**A9.** RP(%) = ((2.5 − 1)/(2.5 + 1))² × 100 = (1.5/3.5)² × 100 = (0.4286)² × 100 = 18.4% reflected.
*TM ref: App. A*

**A10.** See Chapter 6. High SWR at design frequency with resonance above → antenna is short → lengthen the radiating element. Chapter 6 specifies the trim procedure.
*TM ref: Ch. 6*

**A11.** Criterion: SWR <2.0:1 across 7–30 MHz at center frequency. Check your wording against Chapter 7 — numeric value and conditions must match exactly.
*TM ref: Ch. 7*

**A12.** Tuning (Ch. 6) allows active adjustments to bring the antenna within specification. Verification (Ch. 7) confirms the result and allows no adjustments. Only tuning permits adjustments.
*TM ref: Ch. 6, Ch. 7*

**A13.** No — the verification is invalid. Making adjustments during verification invalidates it. Correct action: return to Chapter 6 tuning with the current element length, complete tuning, then restart the full Chapter 7 verification with no further adjustments.
*TM ref: Ch. 7*

**A14.** See Chapter 7, final steps. Required items typically include: date, antenna type, operating frequency, height/orientation, SWR measurement, pass/fail determination, operator name.
*TM ref: Ch. 7*

**A15.** You CAN conclude: the antenna meets the specified SWR and resonance criteria at this time, at this location and height, in this orientation. You CANNOT conclude: performance will be identical at a different site, height, or orientation; or that the antenna will remain in spec without re-verification.
*TM ref: Ch. 7*

---

## Score Routing

| Score | Action |
|-------|--------|
| 13–15 (all calc correct) | Course complete. Sign off and proceed. |
| 13–15 (calc error) | Review Unit 4. Re-work practice problems P4-1 through P4-4. Retake final. |
| 10–12 | Review weak units per question map below. Retake final. |
| Below 10 | Complete course review from Unit 1. Retake final. |

**Question → Unit map:**
Q1–Q3 → Unit 1 (Theory); Q4–Q5 → Units 2–3 (Materials/Assembly);
Q6–Q9 → Unit 4 (Calculations); Q10 → Unit 4; Q11–Q15 → Unit 5 (Verification).
