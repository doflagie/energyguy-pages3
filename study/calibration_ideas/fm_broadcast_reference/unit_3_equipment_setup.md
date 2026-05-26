# Unit 3 — Setting Up the TinySA
**[TM-CAL-004 — Open Handout](../../Calibration_Ideas/fm_broadcast_reference/tm.html)**
**ELOs:** 1.5, 1.6  
**Estimated time:** 20 minutes  
**TM sections to read first:** Chapter 3 (equipment list), Chapter 4 (all steps)

---

## Step 1: Read the TM

Open TM-CAL-004. Read:
- Section 3-1: Required Equipment (the table)
- Chapter 4: Assembly Procedures (all 6 steps)

Then come back here.

---

## What This Unit Covers

The TinySA setup is a two-stage zoom. You start wide to find the carrier, then narrow down to measure it precisely. Each stage has specific settings. Getting the settings wrong — especially RBW — is the most common source of measurement error in this procedure.

---

## Stage 1: Wide Sweep — Finding the Carrier

| Setting | Value | Why |
|---|---|---|
| Mode | Spectrum analyzer | Not tracking generator mode |
| Center frequency | ~98 MHz (or near a known local station) | Gets you into the middle of the FM band |
| Span | 10 MHz | Wide enough to see several stations at once |
| RBW | ≤3 kHz | Narrow enough to resolve individual carriers |

At these settings, you will see a series of peaks separated by 200 kHz. Each peak is one FM station's carrier. They look like vertical spikes, with the tallest representing the strongest local station.

**What to look for:** The carrier is the single tallest, sharpest spike within a 200 kHz channel. There will be other content nearby — audio sidebands, the 19 kHz stereo pilot tone, a 38 kHz stereo subcarrier — but these are much lower in amplitude. The carrier is the dominant peak.

**What not to mistake for the carrier:**
- 19 kHz and 38 kHz offset peaks — these are audio subcarriers, not the carrier
- The broad "skirt" at the base of the carrier — that's the modulated audio, not the center

**Identify your target station.** Note which channel it appears in. That is the peak you will zoom in on.

---

## Stage 2: Narrow Sweep — Measuring the Carrier

Once you have identified the target carrier, zoom in:

| Setting | Value | Why |
|---|---|---|
| Center frequency | On the carrier peak | Put it in the center of your display |
| Span | 500 kHz | Tight enough to isolate the carrier cleanly |
| RBW | 1 kHz | Narrow enough for precise frequency reading |

At 1 kHz RBW, the carrier peak will appear as a clean, narrow spike. Use the **MARKER PEAK** function to place a marker at the top of the peak. The marker frequency readout is your measured frequency.

**Write this number down.** To be useful, record it to the nearest 100 Hz — for example, 98,101,700 Hz (not just "98.1 MHz").

---

## Why RBW Matters

RBW (Resolution Bandwidth) determines how narrow a signal the analyzer can resolve. A 10 kHz RBW smears a carrier across 10 kHz of display width — you cannot determine the center precisely. A 1 kHz RBW shows the carrier as a narrow spike and lets the marker find the true center.

Rule: **RBW must be narrower than the measurement precision you need.** For this calibration, you need precision to ±1 kHz or better, so 1 kHz RBW is the minimum. Narrower (300 Hz) is better if your TinySA firmware supports it without excessive sweep time.

---

## The Six Steps in Order

This is a condensed version of TM-CAL-004 Chapter 4 so you can check your understanding:

1. Connect antenna to TinySA INPUT port
2. Set: center 98 MHz, span 10 MHz, RBW ≤3 kHz → find the carrier spike
3. Identify the tallest narrowband spike in the target channel
4. Set: center on carrier, span 500 kHz, RBW 1 kHz
5. Engage MARKER PEAK on the carrier
6. Record the measured frequency (to nearest 100 Hz)

---

## Self-Check Questions

**SC3-1.** What span and RBW settings does TM-CAL-004 specify for the initial FM carrier identification sweep?

**SC3-2.** What span and RBW settings are used for precision carrier measurement?

**SC3-3.** After zooming in, what TinySA function does TM-CAL-004 specify to read the carrier frequency?

**SC3-4.** A student sets RBW to 100 kHz for the precision measurement sweep. What problem does this cause?

**SC3-5.** During the wide sweep, you see a peak at your target channel plus smaller peaks ±19 kHz and ±38 kHz offset. Which peak is the carrier?

---

## Answer Key

**SC3-1.** Span 10 MHz, RBW 3 kHz or narrower. *(TM Ch. 4, step 2)*

**SC3-2.** Span 500 kHz, RBW 1 kHz. *(TM Ch. 4, step 4)*

**SC3-3.** MARKER PEAK function (or cursor on carrier peak). *(TM Ch. 4, step 5)*

**SC3-4.** At 100 kHz RBW, the carrier is smeared across 100 kHz of display width — the marker will not find the true center frequency. Measurement error will be large. RBW must be ≤ the required measurement precision.

**SC3-5.** The carrier is the tallest, sharpest spike at the channel center. The ±19 kHz and ±38 kHz peaks are the stereo pilot and subcarrier — lower in amplitude, not the carrier.

---

## Checkpoint

Before proceeding, you must be able to state both sets of TinySA settings (wide and narrow) from memory, and explain what RBW does.

**→ Proceed to [Unit 4](unit_4_error_calculation.md)**
