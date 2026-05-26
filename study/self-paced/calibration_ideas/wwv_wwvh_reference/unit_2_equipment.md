# Unit 2 — Equipment and Test Setup
**[TM-CAL-001 — Open Handout](../../../Calibration_Ideas/wwv_wwvh_reference/tm.html)**
**TM Chapter:** Chapter 3
**ELOs:** Identify required equipment; understand test configuration
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-CAL-001. Read **Chapter 3 — Materials and Construction** completely.

Then come back here.

---

## Chapter 3 Content

### 3-1. REQUIRED EQUIPMENT

| Item | Description | Specification | Notes |
|---|---|---|---|
| 1 | Shortwave receiver | Coverage 2.5–20 MHz, SSB/AM | Any general coverage HF receiver |
| 2 | Frequency counter | Resolution 1 Hz or better | Gate time 1 s minimum |
| 3 | BNC coax cable | RG-58 or RG-174, 1–2 m | Receiver output to counter |
| 4 | Shortwave antenna | Any wire, 5–30 m preferred | Outdoor or attic preferred |
| 5 | Headphones (optional) | Any impedance | Monitoring only |

### 3-2. ANTENNA CONSIDERATIONS

Any wire antenna will receive WWV/WWVH under favorable conditions. A 15 m (50 ft) end-fed wire antenna in the attic or outdoors provides adequate signal on most frequencies. Noise level in the shack is more limiting than antenna size.

> **NOTE:** A NanoVNA or TinySA can serve as the frequency counter for this procedure if the reference output connector is accessible. The TinySA Ultra internal reference can itself be calibrated by this method.

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
