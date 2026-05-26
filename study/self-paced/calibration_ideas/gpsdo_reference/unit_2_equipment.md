# Unit 2 — Equipment and Test Setup
**[TM-CAL-003 — Open Handout](../../../Calibration_Ideas/gpsdo_reference/tm.html)**
**TM Chapter:** Chapter 3
**ELOs:** Identify required equipment; understand test configuration
**Estimated time:** 20 minutes

---

## Step 1: Read the TM

Open TM-CAL-003. Read **Chapter 3 — Materials and Construction** completely.

Then come back here.

---

## Chapter 3 Content

### 3-1. BILL OF MATERIALS

| Qty | Item | Specification |
|---|---|---|
| 1 | u-blox NEO-M8N GPS module | With UART and TIMEPULSE |
| 1 | Si5351A clock breakout | Adafruit or equivalent, 25 MHz XTAL |
| 1 | ESP32 development board | Any variant with I2C and UART |
| 1 | Active GPS patch antenna | Magnetic mount, 3.3 V bias |
| 1 | 5 V / 500 mA supply | Clean, low-ripple |
| 1 | SMA panel connector | For 10 MHz output |
| 1 | Project enclosure | Diecast aluminum preferred for RF shielding |
| 4 | 100 nF bypass capacitors | On every power rail |
| 1 | 10 μF electrolytic | Bulk bypass for ESP32 supply |

### 3-2. WIRING

Connect NEO-M8N UART TX → ESP32 UART RX. Connect NEO-M8N TIMEPULSE → ESP32 GPIO (e.g., GPIO 4) with 100 Ω series resistor. Connect Si5351 SDA/SCL → ESP32 I2C (GPIO 21/22). Connect Si5351 CLK0 output → SMA panel connector via short 50 Ω trace.

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
