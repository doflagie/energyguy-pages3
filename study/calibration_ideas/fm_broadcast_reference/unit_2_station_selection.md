# Unit 2 — Selecting and Looking Up Reference Stations
**[TM-CAL-004 — Open Handout](../../Calibration_Ideas/fm_broadcast_reference/tm.html)**
**ELOs:** 1.3, 1.4  
**Estimated time:** 15 minutes  
**TM sections to read first:** Section 3-1, Section 3-2

---

## Step 1: Read the TM

Open TM-CAL-004. Read:
- Section 3-1: Required Equipment
- Section 3-2: Preparation

Then come back here.

---

## What This Unit Covers

A frequency reference is only as good as you know it to be. Before you put a TinySA in front of an FM carrier and call it a calibration, you need to know:
1. What frequency that station is *legally assigned* to transmit on
2. Whether that station is a good choice for calibration work

Both come from the same source: the FCC FM Query database.

---

## The FCC FM Query Database

The FCC maintains a public database of all licensed FM stations. The URL in TM-CAL-004 is:

```
transition.fcc.gov/fcc-bin/fmq.pl
```

You can search by state, city, or call sign. What you are looking for, for each station:

| Field | What It Tells You |
|---|---|
| Call sign | Station identity (e.g., KXYZ) |
| Channel number | Multiply by 0.2, add 87.9 to get frequency — or just read the frequency directly |
| Assigned frequency (MHz) | The number you use as your reference |
| ERP (kW) | Effective radiated power — affects signal strength at your location |
| Transmitter location | Distance and direction from your site |

Record the assigned frequency to the nearest 100 Hz from the database. This is the number you will compare against your measured value.

---

## Station Selection Criteria

Not every FM station makes an equally good calibration reference. The TM specifies:

**Power:** Choose stations with **≥25 kW ERP**. Higher power means a stronger, cleaner carrier at your location. A weak signal gives a noisy carrier peak, and a noisy peak gives a less precise frequency reading.

**Distance:** Choose stations **10–100 km** away. 

- Too close (<10 km): The signal may overdrive your TinySA input, causing intermodulation and a distorted carrier peak.  
- Too far (>100 km): Signal is weak, carrier peak is noisy, measurement is less precise.  
- 10–100 km is the sweet spot: strong signal, low noise, no overload.

**How many:** Use **2–3 stations** for calibration. You will average the errors across stations to reduce the effect of any one station being slightly off.

**Spread across the band:** Don't pick three stations clustered in one corner of the band (e.g., 88.1, 88.3, 88.5). Pick stations spread across the 88–108 MHz range so your calibration represents the whole band.

---

## Pre-Lab Task

Do this now, before you reach the lab in Unit 5. It takes a few minutes and the lab goes faster if you have this ready.

1. Go to the FCC FM Query database.
2. Search for FM stations near your location.
3. Identify 3 stations that meet the criteria (≥25 kW ERP, 10–100 km, spread across the band).
4. For each station, record:
   - Call sign
   - Assigned frequency (MHz), to 4 decimal places (e.g., 98.1000)
   - ERP (kW)
   - Approximate distance from your location

Keep this list with you for Unit 5.

---

## Self-Check Questions

**SC2-1.** What is the name of the database you use to find an FM station's assigned frequency?

**SC2-2.** What minimum ERP should your chosen stations have?

**SC2-3.** What distance range is specified for calibration reference stations?

**SC2-4.** Why is a station that is too close (under 10 km) a poor choice?

**SC2-5.** How many stations should you measure to establish your calibration correction?

---

## Answer Key

**SC2-1.** FCC FM Query database (transition.fcc.gov/fcc-bin/fmq.pl). *(TM §3-2)*

**SC2-2.** ≥25 kW ERP. *(TM §3-2)*

**SC2-3.** 10–100 km. *(TM §3-2)*

**SC2-4.** Not explicitly stated in the TM — but TM §5-1 notes the caution about multipath and overload effects. The TM's selection criteria exist to ensure "best signal quality." Accept: too close may overdrive the input or degrade measurement quality.

**SC2-5.** 2–3 stations. *(TM §3-2)*

---

## Checkpoint

Before proceeding, you must be able to state the two station selection criteria (power and distance) and explain why each matters.

You should also have your 3 local station assignments recorded and ready for Unit 5.

**→ Proceed to [Unit 3](unit_3_equipment_setup.md)**
