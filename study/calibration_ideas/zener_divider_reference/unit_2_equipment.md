# Unit 2 — Equipment and Test Setup
**[TM-CAL-007 — Open Handout](../../Calibration_Ideas/zener_divider_reference/tm.html)**
**TM Chapter:** Chapter 3
**ELOs:** Identify required equipment; understand test configuration
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-CAL-007. Read **Chapter 3 — Materials and Construction** completely.

Then come back here.

---

## Chapter 3 Content

### 3-1. BILL OF MATERIALS

| Qty | Item | Specification |
|---|---|---|
| 1 | LM336Z-2.5 | 2.5 V adjustable reference |
| 1 | 50kΩ trim pot | Multiturn, Bourns 3296W |
| 2 | 10kΩ resistor | 0.1%, metal film, for trim network |
| 4 | Precision resistor | 0.1% for divider outputs |
| 2 | 100 nF capacitor | C0G, output bypass |
| 4 | Banana jack | Color coded per voltage |
| 1 | 9 V battery | With snap connector |

---

## Equipment Readiness

Review the equipment list in Chapter 3 carefully.

Before beginning any calibration, verify every item on the equipment list is:
- Present and in working order
- Within its own calibration period (do not calibrate with an uncalibrated reference)
- Set up per the configuration specified in Chapter 3

The reference quality is the ceiling on your calibration quality. A reference with unknown accuracy produces a calibration with unknown accuracy.

---

## Self-Check Questions

**SC2-1.** List the three most important pieces of equipment specified in Chapter 3.

**SC2-2.** What reference standard(s) does the TM specify for this calibration?

**SC2-3.** Does Chapter 3 specify any warm-up time or settling requirement? If so, state it.

**SC2-4.** What is the test configuration? (How is the equipment connected or arranged?)

**SC2-5.** What would be the consequence of using a reference standard that is itself out of calibration?

---

## Answer Key

**SC2-1.** See Chapter 3 equipment table or list. Identify the three items that appear first or are marked most critical.

**SC2-2.** See Chapter 3. The reference standard is the source of known-good values used to check the instrument under test.

**SC2-3.** See Chapter 3. Warm-up time is often specified for instruments that use oscillators or amplifiers — they need thermal stabilization before measurements are valid.

**SC2-4.** See Chapter 3. Describe the connection or arrangement in your own words, then verify against the TM.

**SC2-5.** The calibration error of the reference propagates directly into your calibration result. If the reference is 5% off and you do not know it, your calibration will also be 5% off — and you will not know that either.

---

## Checkpoint

Before proceeding, you must be able to:
- List the required equipment from memory
- State the reference standard and its specified accuracy

**→ Proceed to [Unit 3](unit_3_setup.md)**
