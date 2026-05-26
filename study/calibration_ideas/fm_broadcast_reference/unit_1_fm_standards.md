# Unit 1 — FM Broadcast Standards and Traceability
**[TM-CAL-004 — Open Handout](../../Calibration_Ideas/fm_broadcast_reference/tm.html)**
**ELOs:** 1.1, 1.2, 2.5  
**Estimated time:** 20 minutes  
**TM sections to read first:** Chapter 1, Section 2-1, Section 2-2

---

## Step 1: Read the TM

Open TM-CAL-004. Read:
- Chapter 1 (all — it's short)
- Section 2-1: FM Broadcast Frequency Standards
- Section 2-2: Traceability

Then come back here.

---

## What This Unit Covers

You just read that FM stations are required by FCC law to hold their carrier frequency within a specified tolerance. This unit makes sure you understand exactly what that tolerance is, why it makes FM carriers useful as a reference, and what the limits of that usefulness are.

---

## The FM Broadcast Band

The FM broadcast band runs from **88.1 MHz to 107.9 MHz**. Channels are assigned at **200 kHz spacing** — so 88.1, 88.3, 88.5 ... 107.7, 107.9 MHz. Each station is assigned one of these specific frequencies. That assignment is a matter of FCC license record — it is not an estimate, and it does not drift.

What *can* drift is the station's actual transmitted carrier relative to its assigned frequency. That drift is what FCC Part 73.1545 controls.

---

## FCC Part 73.1545 — The Tolerance

The FCC sets different tolerances by station class:

| Station Type | FCC Tolerance | Typical In Practice |
|---|---|---|
| Commercial FM (full power) | ±2,000 Hz | ±100–500 Hz |
| Low Power FM (LPFM) | ±3,000 Hz | ±2,000 Hz |
| Translator | ±2,000 Hz | ±1,000 Hz |

The key number for this course is **±2 kHz** for commercial stations. That is the *worst case you should ever see* from a licensed commercial FM station. Modern stations with GPS-disciplined synthesizers typically do far better — major stations run ±100 Hz routinely.

Why the difference between FCC limit and actual practice? The FCC sets a legal floor. Modern station automation uses GPS-locked frequency synthesis because it's cheap and reliable, so stations run well inside the limit.

---

## Traceability — What That Means Here

"Traceability" in calibration means you can draw an unbroken chain from your measurement back to a primary standard (usually NIST). FM carriers are not NIST-traceable in the formal sense. The chain is:

**FCC license (assigned frequency) → station GPS/synthesizer → transmitted carrier → your measurement**

The FCC compliance requirement is what gives you confidence. Stations are legally required to stay within ±2 kHz and subject to enforcement. For field calibration purposes, this is adequate — but know the limits.

---

## The Numbers in ppm

Parts per million (ppm) is how frequency error is expressed relative to the carrier frequency. It lets you compare errors across different frequencies on a common scale.

The worst-case FM tolerance in ppm:

```
±2,000 Hz at 100 MHz = ±2,000 / 100,000,000 × 1,000,000 = ±20 ppm
```

Typical accuracy from a commercial station: **±5 ppm**.

These numbers tell you what FM carriers are good for:
- VHF receiver alignment: yes
- Spectrum analyzer calibration check: yes
- Frequency counter verification at VHF: yes
- Replacing a GPSDO for precision work: no

---

## Self-Check Questions

Answer these before moving to Unit 2. Write your answers, then check against the key below.

**SC1-1.** What is the FCC Part 73.1545 carrier frequency tolerance for a commercial FM station?

**SC1-2.** What is the FCC tolerance for a Low Power FM (LPFM) station?

**SC1-3.** What is the channel spacing in the FM broadcast band, and what is the frequency range?

**SC1-4.** Express the worst-case commercial FM tolerance in ppm (show your calculation).

**SC1-5.** A major FM station uses GPS-disciplined frequency synthesis. What typical accuracy does TM-CAL-004 state for such a station?

---

## Answer Key

**SC1-1.** ±2,000 Hz (±2 kHz). *(TM §2-1)*

**SC1-2.** ±3,000 Hz (±3 kHz). *(TM §2-1, Table)*

**SC1-3.** 200 kHz spacing; 88.1 MHz to 107.9 MHz. *(TM §2-1)*

**SC1-4.** ±2,000 Hz / 100,000,000 Hz × 10⁶ = **±20 ppm**. *(TM §2-2)*

**SC1-5.** ±100 Hz. *(TM §2-1, Table)*

---

## Checkpoint

Before proceeding to Unit 2, you must be able to state — without looking:
- The FCC tolerance for commercial FM (in Hz)
- The FCC tolerance for LPFM (in Hz)
- FM band range and channel spacing
- Worst-case accuracy in ppm

If any of those are unclear, re-read TM §2-1 and §2-2.

**→ Proceed to [Unit 2](unit_2_station_selection.md)**
