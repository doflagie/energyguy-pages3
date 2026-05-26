# Portable Sundial Reference for Field Work
## Altitude-Based Time Determination (No Compass Required)

---

## OVERVIEW

These seven methods determine time from **celestial observation only**—no batteries, electronics, or magnetic compasses required. They work anywhere on Earth after simple latitude adjustment, and operate whenever celestial objects are visible (sun by day, bright stars by night).

**Key advantages for RF propagation field work:**
- **Zero power dependency:** Work indefinitely at remote sites
- **Equipment-independent:** Unfailing backup if GPS/electronics fail
- **Cost:** Print once (~$0), laminate (~$5), use lifetime
- **Reliability:** Celestial mechanics unchanged since ancient times
- **Redundancy:** Carry multiple methods in ~200g backpack kit
- **Calibration-free:** No drift, no maintenance, no recalibration except seasonal declination shift

**Core principle:** Sun altitude varies predictably by time of day, latitude, and date. Measure altitude → look up time.

---

## EQUATION OF TIME (Critical Correction for Accuracy)

### What It Is

**The Equation of Time** = difference between **apparent solar time** (what a sundial shows) and **mean solar time** (what a clock/UTC shows).

**Why it exists:**
1. Earth's orbit is elliptical, not circular → sun appears to move at varying speeds throughout the year
2. Earth's axis is tilted relative to orbital plane → creates seasonal asymmetries
3. These two effects combine to create a time difference of up to ±16 minutes depending on the date

**Critical for RF field work:** Propagation conditions are logged to precise UTC times. A ±15-minute error can miss important band-opening windows or D-region collapse timing.

### Equation of Time Table (Full Year)

| Date | Correction | | Date | Correction |
|------|---|---|------|---|
| Jan 1 | −3 min | | Jul 1 | −6 min |
| Jan 10 | −8 min | | Jul 10 | −5 min |
| Jan 20 | −11 min | | Jul 20 | −3 min |
| Jan 31 | −14 min | | Aug 1 | 0 min |
| Feb 10 | −14 min | | Aug 10 | +6 min |
| Feb 20 | −13 min | | Aug 20 | +11 min |
| Mar 1 | −12 min | | Aug 31 | +16 min |
| Mar 10 | −11 min | | Sep 10 | +15 min |
| Mar 20 | −7 min | | Sep 20 | +8 min |
| Mar 31 | −1 min | | Oct 1 | +4 min |
| Apr 10 | +4 min | | Oct 10 | −1 min |
| Apr 20 | +7 min | | Oct 20 | −7 min |
| May 1 | +4 min | | Nov 1 | −14 min |
| May 10 | +2 min | | Nov 10 | −16 min |
| May 20 | 0 min | | Nov 20 | −14 min |
| May 31 | −3 min | | Dec 1 | −11 min |
| Jun 10 | −5 min | | Dec 10 | −6 min |
| Jun 20 | −6 min | | Dec 21 | −3 min |
| Jun 30 | −4 min | | Dec 31 | −2 min |

### How to Apply (3 Steps)

**Step 1:** Your sundial reads time. Example: 15:45 (apparent solar time)

**Step 2:** Look up correction for today's date. Example: Nov 15 = −15 minutes

**Step 3:** Add correction to sundial reading:
- **If correction is negative (−):** Subtract from sundial time
  - Sundial reads 15:45, correction = −15 min → UTC = 15:45 − 15 min = **15:30 UTC**
  
- **If correction is positive (+):** Add to sundial time
  - Sundial reads 10:20, correction = +6 min → UTC = 10:20 + 6 min = **10:26 UTC**

### Quick Reference (Rough Approximation)

| Season | Typical Correction | Approximate UTC Adjustment |
|--------|---|---|
| **Jan–Feb** | −10 to −14 min | Subtract 12 minutes |
| **Mar–Apr** | −2 to +8 min | Subtract 0–2 minutes |
| **May–Jun** | −6 to 0 min | Subtract 3 minutes |
| **Jul–Aug** | −2 to +15 min | Add 6 minutes |
| **Sep–Oct** | −8 to +15 min | Add 3 minutes |
| **Nov–Dec** | −16 to −3 min | Subtract 10 minutes |

### Graphical Representation (Equation of Time Curve)

```
        +15 min │
                │      ╱╲
        +10 min │     ╱  ╲
                │    ╱    ╲
         +5 min │   ╱      ╲
                │  ╱        ╲___
             0 min├─────────────────╱───
                │                 ╱
         −5 min │                ╱
                │               ╱
        −10 min │              ╱
                │             ╱
        −15 min │____________╱
                │
       Jan  Feb  Mar  Apr  May  Jun  Jul  Aug  Sep  Oct  Nov  Dec

Peaks:    Nov (~+16 min), Jul/Aug (~+14 min)
Valleys:  Feb (~−14 min), May (~−3 min)
Zero crossings: Apr 16, Jun 15, Sep 1, Dec 25
```

### Application Examples (RF Propagation Logging)

**Example 1: Winter morning check-in**
- Date: February 10
- Sundial reading: 08:30
- Correction from table: −14 minutes
- **UTC time: 08:30 − 14 min = 08:16 UTC**
- **Log:** "08:16 UTC — 20m band opening observed, strong signals from EU"

**Example 2: Summer afternoon measurement**
- Date: August 15
- Sundial reading: 14:45
- Correction from table: +10 minutes
- **UTC time: 14:45 + 10 min = 14:55 UTC**
- **Log:** "14:55 UTC — 10m band peak, sunspot activity peak"

**Example 3: Autumn evening session**
- Date: October 10
- Sundial reading: 18:00
- Correction from table: −1 minute
- **UTC time: 18:00 − 1 min = 17:59 UTC**
- **Log:** "17:59 UTC — 40m evening propagation begins"

### Why This Matters for Your Work

1. **Propagation windows:** D-region ionization follows UTC time precisely; a 15-minute error shifts your logged window by an hour
2. **Comparison with other data:** Solar indices (A-index, K-index) published on UTC; your logs must match UTC
3. **Cross-site propagation:** When comparing your propagation notes with other operators' logs, UTC accuracy is critical
4. **Historical records:** Field notebooks documenting band behavior become reference data; equation of time correction ensures long-term usability

### Storage on Field Notebook

Print this equation of time table **directly on your 8x10 field notebook template** (in the REFERENCE DATA section) so it's always available without needing separate lookup.

Laminate the complete notebook so the correction table is waterproof and available in rain/field conditions.

---

## 1. CAPUCHIN DIAL (Simplest, Most Portable)

### What It Is
A vertical stick and a simple lookup table. Oldest form, documented in medieval texts.

### How to Build
1. Print a vertical line on cardstock (8" = 20 cm)
2. At the **bottom**, mark "0° horizon"
3. Mark altitude angles at **regular intervals** up the line:
   - Every 10° mark a hash with angle label
   - Use a protractor to set angles initially, then draw scale

### How to Use
1. **Hold vertical:** Use plumb bob (printed on notebook margin or weighted string)
2. **Sight sun:** Hold stick so sun casts minimal shadow on the scale (sun should be on horizon line extended)
3. **Read angle:** Where sun shadow touches the marked line = sun altitude in degrees
4. **Look up time:** Use table below (latitude-specific; adjust each month)

### Time Lookup Table (Merced, CA — Latitude 37.3°N)

**January (Sun declination ≈ −20°)**
| Altitude | Time from Noon | | Altitude | Time from Noon |
|----------|---|---|----------|---|
| 5° | ±5 h 45 min | | 25° | ±2 h 30 min |
| 10° | ±4 h 45 min | | 30° | ±1 h 45 min |
| 15° | ±3 h 45 min | | 32° | 0 h (noon) |

**April (Sun declination ≈ +10°)**
| Altitude | Time from Noon | | Altitude | Time from Noon |
|----------|---|---|----------|---|
| 15° | ±4 h 30 min | | 45° | ±1 h 15 min |
| 25° | ±3 h | | 55° | 0 h (noon) |
| 35° | ±1 h 45 min | | | |

**July (Sun declination ≈ +20°)**
| Altitude | Time from Noon | | Altitude | Time from Noon |
|----------|---|---|----------|---|
| 20° | ±4 h 30 min | | 50° | ±1 h |
| 30° | ±3 h 15 min | | 72° | 0 h (noon) |
| 40° | ±2 h | | | |

**October (Sun declination ≈ −10°)**
| Altitude | Time from Noon | | Altitude | Time from Noon |
|----------|---|---|----------|---|
| 10° | ±5 h | | 40° | ±1 h 30 min |
| 20° | ±3 h 45 min | | 50° | 0 h (noon) |
| 30° | ±2 h 30 min | | | |

### Accuracy
- **±10–15 minutes** with careful measurement
- **±5 minutes** with multiple readings averaged
- Accuracy degrades near sunrise/sunset (altitude <10°)
- Best accuracy near solar noon (altitude highest)

---

## 2. SHEPHERD'S DIAL (Marked Staff)

### What It Is
A vertical staff with hour markings. The sun's shadow length indicates time—more precise than Capuchin if properly calibrated.

### How to Build
1. Use a straight rod or dowel, 12 inches (30 cm) long
2. Mark from bottom: calibration points at specific intervals
3. At each hour distance, mark time label

### Construction by Latitude
**For latitude 37°N (Merced):**

Hold staff vertical, measure distance from bottom to shadow tip:

| Distance from Base | Time | | Distance | Time |
|---|---|---|---|---|
| 1 cm | 6 AM / 6 PM | | 8 cm | 10 AM / 2 PM |
| 2 cm | 7 AM / 5 PM | | 10 cm | 11 AM / 1 PM |
| 4 cm | 8 AM / 4 PM | | 12 cm | Noon |
| 6 cm | 9 AM / 3 PM | | | |

### How to Use
1. Place staff on level ground, perfectly vertical (check with phone level or plumb bob)
2. Point a chalk/pencil shadow directly at sun's shadow on staff base
3. Measure distance from staff base to shadow tip
4. Read time from calibration marks
5. **Refine:** Recalibrate every 2 weeks (sun declination shifts; marks drift with thermal expansion)

### Accuracy
- **±5–10 minutes** with careful alignment
- **±2 minutes** with metal staff (less thermal drift) and regular recalibration
- Loses accuracy within 2 hours of sunrise/sunset

### Maintenance
- Check staff straightness weekly
- Recalibrate on known time (e.g., noon by sun meridian passage)
- Mark thermal drift correction at top of staff

---

## 3. ASTROLABE / RING DIAL (Most Portable, Pre-Calculated)

### What It Is
A printed or hand-drawn circular dial with altitude scale and pre-computed time rings for different months.

### Simplified Printable Version

```
         ↑ North
    [Thin cord with bead]
         |
    ╱────────╲
   /  ASTRO  \
  |  LAB      |  ← Align "0" down toward ground
  |   37°N    |     (gravity aligns bead)
   \          /
    ╲────────╱
      
Printed on cardstock, laminated:
- Outer ring: altitude 0° to 90° (marked every 5°)
- Inner rings: hour scales for Jan/Apr/Jul/Oct
- Bead hangs at altitude angle
```

### How to Build
1. Print concentric rings (see PDF template below)
2. Laminate with 3mil sheet for waterproofing
3. Attach small weight (bead, shot, etc.) on 12" cord
4. Mark one edge as "0° elevation" reference

### How to Use
1. **Suspend:** Hold by top, let bead hang freely
2. **Sight sun:** Align "0° elevation" edge with horizon (use one eye near edge)
3. **Read altitude:** Note where bead points on outer ring
4. **Find month:** Use inner ring for current month
5. **Read time:** Follow angle to hour marking

### Accuracy
- **±10–15 minutes** (bead swing, parallax error)
- Best within 3 hours of solar noon
- Advantage: one compact tool for all months and latitudes

---

## 4. HAND / FIST METHOD (Emergency Only)

### Quick Approximation
If no instrument available:
- **Closed fist** at arm's length ≈ 10° altitude
- **Hand width** ≈ 15° altitude
- **Thumb width** ≈ 2° altitude

**Example:** If sun is 2 fist-widths above horizon = ~20° altitude → near 8 AM or 4 PM

**Accuracy:** ±30 minutes (rough estimate only)

---

## DAILY CALIBRATION PROCEDURE

Before field session:
1. **Set known time:** Sync watch/phone to atomic clock (time.nist.gov or GPS)
2. **Measure altitude:** Use your sundial method
3. **Record both:** Altitude angle + UTC time in DAILY CALIBRATION box on notebook
4. **Calculate error:** Expected altitude (from table) vs. observed
5. **Apply correction:** Add/subtract drift to all subsequent readings

**Example:**
- Expected altitude at 12:00 UTC = 55°
- Observed altitude = 53°
- Error = −2° → subtract 2° from all future readings

---

## SEASONAL ADJUSTMENT

The sun's declination (angle north/south of celestial equator) changes daily.

**Quick table for your location:**

| Month | Declination | Noon Altitude (37°N) | Adjustment |
|-------|---|---|---|
| Jan 1 | −23° | 32° | Use Jan table |
| Feb 15 | −8° | 46° | Use Feb table |
| Apr 15 | +10° | 55° | Use Apr table |
| Jun 21 | +23° | 73° | Use Jun table |
| Aug 15 | +18° | 68° | Use Aug table |
| Oct 15 | −8° | 46° | Use Oct table |
| Dec 21 | −23° | 32° | Use Dec table |

**Month-by-month:** Recalibrate every 2 weeks for accuracy better than ±10 minutes.

---

## TROUBLESHOOTING

| Problem | Cause | Fix |
|---------|-------|-----|
| Time off by hours | Reading AM/PM backwards | Mark AM/PM zones, recheck table |
| Altitude reading too high | Staff/stick tilted | Use level, check plumb bob |
| Readings drift over days | Thermal expansion, seasonal shift | Recalibrate weekly, track drift |
| Can't see sun (overcast) | Clouds obstruct direct sight | No altitude method works; use shadows when breaks appear |
| Shadow too short/long | Wrong staff length | Check against known time at solar noon |

---

## REFERENCES & FURTHER READING

- **Astrolabe construction:** "The Astrolabe of the World" (Chaucer's medieval text, still accurate)
- **Nautical Almanac:** USNO publishes annual sun declination tables
- **Latitude 37°N:** Merced, CA; adjust tables by 1–2° for San Francisco (37.8°) or Fresno (36.7°)
- **GPS verification:** Use phone GPS time as confirmation before field session

---

---

## 5. GUNTER'S QUADRANT (Simplified Astrolabe Alternative)

### What It Is
A quarter-circle instrument (90° arc) with pre-printed hour curves and altitude scale. Combines altitude measurement with time calculation in one step—no table lookups needed. Invented by Edmund Gunter (1581–1626), still highly practical for field work.

### How It Works
**Principle:** Instead of measuring altitude and then cross-referencing declination tables (like astrolabe), Gunter's Quadrant has the declination relationships **baked into curved lines** on the dial. You measure altitude, follow the curve for your date, read time directly.

### How to Build (Cardstock Version)

```
            ↑ 
        [Sight notches]
            |
        ┌───────┐
        │ 90°   │  ← Arc edge (altitude scale 0–90°)
        │       │
    ────●───────┴────  ← Horizontal baseline
        │       |
        │  Hour |
        │ curves│  (curves labeled Jan–Dec)
        │ for   │
        │ dates │
        │       │
    [Plumb bob hangs from center hole]
```

**Construction steps:**
1. Print quarter-circle on cardstock (radius 6–8 inches)
2. **Outer arc:** Altitude scale 0° at left edge to 90° at top (every 10°)
3. **Interior curves:** Eight curved lines labeled for months:
   - Curve for Jan/Dec (winter, low sun)
   - Curve for Feb/Nov
   - Curve for Mar/Sep
   - Curve for Apr/Aug
   - Curve for May/Jul
   - Curve for Jun (summer, high sun)
4. **Hour scale:** Along each curved line, mark hour divisions (or use reference below)
5. **Center hole:** Plumb bob attachment
6. **Baseline edge:** Horizontal reference (align with horizon when measuring)

### How to Use

**Daytime (Sun Measurement):**

1. **Hold horizontal:** Align quadrant baseline with true horizon (or use level)
2. **Sight sun:** Look along top edge; adjust quadrant until sun's edge just touches the altitude arc
3. **Read altitude:** Where sun edge touches arc = degrees of altitude
4. **Find date curve:** Locate current date's curve (e.g., "July" if measuring July 15)
5. **Read time:** Follow vertical line from altitude point to intersect the date curve, then read time on hour markings

**Nighttime (Star Measurement):**

1. **Hold vertical:** Let plumb bob hang freely
2. **Sight bright star:** Align quadrant so star appears at edge of arc
3. **Read altitude:** Where your sightline touches arc
4. **For Polaris:** Use special "Polaris curve" (constant +89° altitude year-round)
5. **For other stars:** Use reference table (see below) to find declination, then interpolate between nearest month curves
6. **Read time:** Same as daytime—find date curve, read where altitude line crosses it

### Quick Hour Curve Reference (Gunter's Quadrant for 37°N latitude)

**Reading method:** Altitude 45° on Jan curve → trace to time scale → read 10:00 AM or 2:00 PM (depending on hemisphere of curve)

**Pre-printed on dial (approximate hours for each altitude on each date curve):**

| Altitude | Jan | Mar | May | Jul | Sep | Nov |
|----------|-----|-----|-----|-----|-----|-----|
| 10° | 7:30 / 4:30 | 8:00 / 4:00 | 8:30 / 3:30 | 8:45 / 3:15 | 8:00 / 4:00 | 7:30 / 4:30 |
| 20° | 8:15 / 3:45 | 8:45 / 3:15 | 9:15 / 2:45 | 9:30 / 2:30 | 8:45 / 3:15 | 8:15 / 3:45 |
| 30° | 9:00 / 3:00 | 9:30 / 2:30 | 10:00 / 2:00 | 10:15 / 1:45 | 9:30 / 2:30 | 9:00 / 3:00 |
| 45° | 10:00 / 2:00 | 10:30 / 1:30 | 11:00 / 1:00 | 11:15 / 0:45 | 10:30 / 1:30 | 10:00 / 2:00 |
| 60° | 11:15 / 0:45 | 11:45 / 0:15 | 12:15 / 23:45 | 12:30 / 23:30 | 11:45 / 0:15 | 11:15 / 0:45 |

*(Left side: AM hours; right side: PM hours. Read time nearest to your measured altitude.)*

### Accuracy
- **±10–15 minutes** with careful altitude measurement
- **±5 minutes** with quality instrument and averaged readings
- No table lookups → faster in field than astrolabe
- Pre-curved hour lines → less parallax error than astrolabe cross-referencing

### Advantages for RF Field Work
- **One-step reading:** No declination tables needed. Altitude + date → time directly
- **Portable:** Quarter-circle smaller than full astrolabe
- **Fast:** Read time in 30 seconds (vs. 2 minutes with astrolabe table lookup)
- **Accurate:** ±5–10 minutes typical
- **Works day/night:** Same method for sun or bright stars
- **Robust:** Simple curved line design less prone to user error than astrolabe

### Disadvantages
- Construction more complex than Capuchin dial (requires accurate curve printing)
- Fewer hour curves than full astrolabe (interpolation needed between curves)
- Requires fine-grained altitude reading (small errors in altitude = larger time errors)

---

## 6. NOCTURNAL (Night Time Determination via Polaris)

### What It Is
Medieval instrument using the Big Dipper's pointer stars and Polaris to read time at night. Works anywhere in Northern Hemisphere; no magnetic compass needed.

### How It Works
**Principle:** The pointer stars of Ursa Major (Big Dipper) rotate around Polaris once per sidereal day (~23h 56m). This rotation is nearly synchronized with solar time, with a known correction.

### How to Build (Cardstock Version)

```
          [Polaris pointer hole — center]
              ↓
     ┌─────────●─────────┐
     │        /          │
     │   Date rings      │
     │   (inner scale)   │
     │                   │
     │   Hour markings   │
     │   (outer ring)    │
     │                   │
     └───────────────────┘
     
Attach pointer arm
(rotates freely through center hole)
```

**Construction:**
1. Print concentric rings on cardstock (diameter 6–8 inches)
2. Inner rings: labeled for each month (Jan, Feb, Mar, etc.)
3. Outer ring: hour scale 0–24 (or 0–12)
4. Center hole: Polaris alignment point
5. Attach rotating pointer arm through center (brad fastener)
6. Laminate for field durability

### How to Use

**Step 1: Locate Polaris (North Star)**
- Find Big Dipper (Ursa Major)
- Follow pointer stars (Dubhe & Merak) 5 star-lengths north to Polaris
- Polaris appears same brightness as pointer stars

**Step 2: Align Instrument**
1. Hold nocturnal at arm's length, eye level
2. Look through center hole at Polaris (keep star centered)
3. Pointer arm should hang down naturally or be held level

**Step 3: Read Time**
1. Rotate outer disk until pointer arm aligns with pointer stars (Dubhe & Merak)
2. Where pointer intersects the **current month's ring** = time
3. Read hour value on outer scale

**Step 4: Apply Seasonal Correction**
Modern solar time differs from star time by ~4 minutes per day (accumulates through year).

| Month | Correction | Example |
|-------|---|---|
| Jan 1 | −14 min | Nocturnal reads 22:45 → UTC is 22:31 |
| Mar 1 | −8 min | Nocturnal reads 23:00 → UTC is 22:52 |
| Jun 1 | +4 min | Nocturnal reads 00:30 → UTC is 00:34 |
| Sep 1 | +8 min | Nocturnal reads 01:15 → UTC is 01:23 |
| Dec 1 | −12 min | Nocturnal reads 00:00 → UTC is 23:48 |

### Accuracy
- **±10–15 minutes** with clear sky and careful star alignment
- **±5 minutes** with multiple readings
- Works independent of: latitude (adjust month ring), longitude, weather (if stars visible)
- **No drift** — sidereal time is constant

### Advantages for RF Field Work
- **Night propagation monitoring:** Many band openings peak at night
- **Independent check:** Not affected by atmospheric refraction like solar methods
- **Portable:** Fits in pocket
- **Free:** Once built, requires no maintenance

---

## 7. ASTROLABE (Universal Day/Night Time & Latitude Measurement)

### What It Is
Sophisticated medieval instrument for astronomical measurement. Measures altitude of sun/moon/stars to determine time, latitude, and other data. Works 24/7.

### Principle
**Altitude + Star catalog = Time + Latitude**

When you know:
- A star's altitude above horizon
- Its declination (from almanac table)
- Your latitude (from maps or previous readings)

You can calculate UTC time to within a few minutes.

### Simplified Field Astrolabe (6" diameter)

```
    Sighting arm (alidade)
    ←————————→ (rotates, points at star)
        |
    ┌───●───────────┐
    │   │   Outer   │
    │   │   scale:  │  Altitude degrees
    │   │   0–90°   │
    │   │           │
    │ Inner scale:  │
    │ Star declinations
    │ and hour rings
    │           │
    └───────────●───┘
         Weight/plumb bob
         (ensures vertical alignment)
```

### How to Build

**Materials:**
- Cardstock or thin aluminum (6" diameter circle)
- Printed scales (see below)
- Lightweight pointer arm (brad fastener)
- Fishing line + small weight (for plumb bob)
- Laminate 3mil sheet

**Scales to Print:**
1. **Outer ring (0–90°):** Altitude in degrees
2. **Middle rings:** Hour scales for each month
3. **Inner markings:** Declinations of bright stars (Polaris, Vega, Betelgeuse, etc.)

### Quick Bright Star Reference (Northern Hemisphere)

| Star | Declination | Best Months | Notes |
|------|---|---|---|
| Polaris | +89° | All year | Nearly stationary, ~1° from pole |
| Vega | +39° | Apr–Oct | Summer evening, bright |
| Deneb | +45° | Jun–Oct | Summer, Cygnus constellation |
| Altair | +9° | Jul–Sep | Summer, medium brightness |
| Betelgeuse | +7° | Oct–Mar | Winter, very bright, red |
| Sirius | −17° | Nov–Mar | Winter, brightest star |
| Regulus | +12° | Feb–May | Spring, Leo constellation |

### How to Use (Simplified for Time Measurement)

**Method A: Bright Star at Night**

1. **Sight star:** Hold astrolabe vertical (check with plumb bob)
2. **Align alidade:** Rotate pointer arm until star appears at edge, visible through both sights
3. **Read altitude:** Note where arm crosses outer scale (in degrees)
4. **Find star's declination:** Look up star in table (or print on inner scale)
5. **Cross-reference:** On the hour ring for current date, find where your measured altitude intersects the star's declination curve
6. **Read time:** Hour marking at intersection = UTC time

**Example:**
- Measure Vega at 35° altitude
- Vega's declination = +39°
- Current date: July 15
- July 15 hour ring + 35° altitude + Vega declination curve intersection → read 22:30 UTC

**Method B: Sun During Day**

1. **Sight sun:** Never look directly; use shadow method
   - Hold astrolabe at angle so sun's shadow just covers the sighting hole
   - Read the altitude where shadow reaches the scale
2. **Follow Method A steps 4–6** using sun's declination for current date
3. **Advantage:** More precise than Capuchin dial because instrument is calibrated

### Quick Sun Declination (Current Year)

| Date | Declination |
|---|---|
| Jan 1 | −23.0° |
| Feb 1 | −17.5° |
| Mar 1 | −8.0° |
| Apr 1 | +8.5° |
| May 1 | +18.5° |
| Jun 21 | +23.5° (summer solstice) |
| Jul 1 | +23.0° |
| Aug 1 | +18.0° |
| Sep 1 | +8.0° |
| Oct 1 | −4.0° |
| Nov 1 | −15.0° |
| Dec 1 | −21.5° |
| Dec 21 | −23.5° (winter solstice) |

### Accuracy
- **±5–10 minutes** with careful sighting and table lookup
- **±2–3 minutes** with bright stars and clear sky
- **No seasonal drift** — star positions are fixed in tables

### Advantages for RF Field Work
- **24/7 operation:** Works day or night, independent of weather (if any star visible)
- **Precise:** Most accurate portable method available
- **Educational:** Builds understanding of celestial navigation and astronomy
- **Cross-check:** Can verify latitude independently
- **Historical continuity:** Same method used by Columbus, explorers for centuries

### Disadvantages
- Learning curve steeper than nocturnals or dials
- Requires star catalog or printed reference tables
- Sighting errors larger for fainter stars

---

## COMPARISON TABLE: All Seven Methods

| Method | Day/Night | Accuracy | Complexity | Portability | Setup Time | Drift |
|--------|-----------|----------|-----------|-------------|-----------|-------|
| **Capuchin Dial** | Day only | ±10–15 min | Trivial | Pocket | <1 min | Small |
| **Shepherd's Staff** | Day only | ±5–10 min | Simple | Backpack | 2 min | Medium |
| **Gunter's Quadrant** | Day/Night | ±5–10 min | Moderate | Pocket | <1 min | None |
| **Astrolabe (Sun)** | Day only | ±5–10 min | Moderate | Pocket | 2 min | Minimal |
| **Nocturnal** | Night only | ±10–15 min | Simple | Pocket | 1 min | None |
| **Astrolabe (Stars)** | Night only | ±5–10 min | Moderate | Pocket | 3 min | None |
| **Hand/Fist** | Day only | ±30 min | Trivial | Always | <30 sec | N/A |

---

## RECOMMENDED FIELD KITS FOR RF PROPAGATION WORK

**Option A: Fastest Field Reading**
1. **Primary:** Gunter's Quadrant — one-step altitude→time, works day/night, fastest reading
2. **Backup:** Capuchin dial — independent check, trivial complexity
3. **Night specialist:** Nocturnal — for night-only sessions

**Option B: Maximum Accuracy**
1. **Primary:** Astrolabe — most capable, handles all scenarios, ±5 min accuracy
2. **Backup (day):** Capuchin dial — independent confirmation
3. **Backup (night):** Nocturnal — independent confirmation

**Option C: Minimal Kit (Backpack)**
1. **Primary:** Gunter's Quadrant — handles 95% of field cases
2. **Fallback:** Hand/fist estimation

**Total kit weight:** 200–400 grams (cardstock laminated)  
**Cost:** $0 if printed + laminated  
**Learning time:** 20 min (Gunter's) to 1.5 hours (all methods)

---

## FIELD NOTES

**Preferred method for RF field work:** Astrolabe (day/night) + Nocturnal backup (night-only propagation checks).

**For remote/extended field sessions:** Gunter's Quadrant primary (compact, one-step) + Nocturnal backup + Capuchin dial (independent check). Total kit: 3 instruments, <200g, fits in shirt pocket, zero power.

**Critical for propagation research:** Night HF band behavior (20–40–80m) often peaks 2–4 hours after local sunset due to ionospheric D-region decay. Accurate UTC time critical for logging best propagation windows. Battery-independent methods ensure reliable time logging even if all electronics fail.

**Long-term field deployments:** Multiple printed sets laminated and bound with field notebooks. No consumables, no charging, no electronic dependencies. Same methods used by navigators for 500+ years; proven reliability under all field conditions.

**Minimal kit for backpack:**
- Laminated 8x10 Capuchin scale card with plumb bob drawn
- 12" wooden dowel with hour marks
- Small clinometer or simple marked altitude ruler

Weight: ~200 grams. Effective to ±10 min anywhere on Earth after local calibration.
