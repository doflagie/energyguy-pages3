# SIERRA NEVADA RF SITE ASSESSMENT PROCEDURE
## TM-FIELD-SAP-001 Rev A

**Purpose:** Characterize a temporary operating site in the Sierra Nevada before antenna deployment. Covers geological interference, ground conductivity, noise floor, and path assessment using NanoVNA, TinySA, and compass.

**Scope:** HF/VHF QRP portable operations. Granite terrain, mixed geology, seasonal moisture variation.

**Time required:** 15–20 minutes for full assessment. 5 minutes for abbreviated go/no-go.

---

## EQUIPMENT REQUIRED

| Item | Use |
|------|-----|
| NanoVNA (calibrated) | Ground conductivity, antenna loading, near-field survey |
| TinySA | Noise floor, path check, pattern distortion |
| Compass (baseplate) | Magnetic anomaly detection |
| Test coil (15 turns, ~5cm dia., air core, known L) | NanoVNA permeability probe |
| Known capacitor (100–470 pF) | Resonance reference with test coil |
| Two ground stakes, 30cm, insulated leads | NanoVNA ground impedance measurement |
| Short coax jumper, SMA | TinySA coupling |
| Site log card | Record results |

**Prepare test coil before departure:** Wind 15 turns of hookup wire on a 5cm form (medicine bottle cap works). Measure and record L at home with NanoVNA. This is your baseline.

---

## CAUTION

> Mountain thunderstorms develop rapidly. Do not drive stakes or erect any antenna structure if storms are within 10 miles or building over ridgelines. The assessment procedure is electrical — suspend it and shelter immediately.

---

## STEP 1 — VISUAL GEOLOGY SURVEY (2 min)

Before unpacking, walk the site and note:

**Look for:**
- Green/black rock surfaces → serpentinite or chlorite schist (iron-bearing, elevated µ and loss)
- Rust-orange staining on rock faces → iron oxide, hematite, goethite (loss, some shift)
- Brassy/gold cubic crystals in rock → pyrite (loss, no shift)
- Dark heavy grains in stream gravel → magnetite (high µ, large shift)
- White quartz veins → electrically dead silica (poor but predictable ground)
- Disturbed ground, old adits, tailings → mining district (unpredictable, avoid if possible)
- Alluvial flat, dry streambed, meadow → best available ground in Sierra terrain

**Record:** Dominant rock type, any anomalous minerals visible, proximity to water.

---

## STEP 2 — COMPASS MAGNETIC SURVEY (2 min)

Walk a 20-meter grid across the proposed antenna footprint with compass held level.

**Method:**
1. Take a reference bearing to a distant landmark on approach to site
2. Walk the antenna area in a grid, check bearing every 3–4 meters
3. Note any deviation from reference bearing

**Interpret:**

| Compass deviation | Implication |
|---|---|
| < 2° | Clean site — no significant ferromagnetic material |
| 2°–5° | Minor iron content — monitor, proceed with awareness |
| 5°–15° | Magnetite present — expect frequency shift in test coil, elevated loop losses |
| > 15° | Strong iron body — avoid for magnetic loop antennas; verticals and dipoles degraded in near field |

**Note:** The radius of compass deviation and the radius of antenna near-field loading are similar at HF. If the compass deflects across your antenna footprint, your antenna is operating in a compromised near field.

---

## STEP 3 — NANOVNA PERMEABILITY PROBE (3 min)

Detects ferromagnetic minerals quantitatively.

**Method:**
1. Connect test coil to NanoVNA port 1 with known cap in parallel
2. In air, note resonant frequency F_air — should match your pre-trip baseline
3. Lower coil to within 2cm of ground surface at several points across the site
4. Note resonant frequency F_ground at each point

**Interpret:**

| F_ground vs F_air | Implication |
|---|---|
| Shift < 2% | Non-magnetic ground (quartz, granite) |
| Shift 2–10% | Weak ferromagnetic content — minor effect |
| Shift > 10% | Significant magnetite or pyrrhotite — measurable antenna loading |
| Broad shallow resonance (low Q) + small shift | Conductive sulfides (pyrite, galena) — loss without much permeability |
| Broad resonance + large shift | Serpentinite or skarn — worst case, both effects |

**Record:** F_air, F_ground at 3–4 points, Q estimate (eyeball dip width).

---

## STEP 4 — NANOVNA GROUND CONDUCTIVITY MEASUREMENT (5 min)

Determines whether a vertical or elevated antenna is the better choice.

**Method:**
1. Drive two stakes 1 meter apart in the proposed antenna ground zone
2. Connect NanoVNA port 1 to one stake, port 2 (or ground) to the other
3. Sweep 1–30 MHz, note resistive component R at your primary operating frequency
4. Optionally: lay a 5-turn loop (50cm diameter) flat on the ground, measure insertion loss vs same loop in air

**Interpret:**

| Ground R (1m stake spacing) | Approximate σ | Antenna guidance |
|---|---|---|
| < 50Ω | > 10 mS/m | Reasonable vertical performance with modest radials |
| 50–200Ω | 2–10 mS/m | Elevated radials strongly preferred; 4+ radials minimum |
| 200–500Ω | 0.5–2 mS/m | Typical Sierra granite — dipole at height preferred over vertical |
| > 500Ω | < 0.5 mS/m | Very poor — use EFHW or dipole; vertical only with 8+ elevated radials |

**Seasonal note:** Same site after spring snowmelt may read 5–10× lower R than late summer. Measure, don't assume.

**Record on R_ground site log card.**

---

## STEP 5 — TINYSA NOISE FLOOR SURVEY (3 min)

Confirms the site is electrically quiet before committing to setup.

**Method:**
1. Connect TinySA to a short (30cm) wire whip
2. Sweep 1–30 MHz, note noise floor and any discrete signals or hash
3. Rotate 360° slowly at 7 MHz and 14 MHz — note any directional noise sources
4. Check 50 MHz and 144 MHz if VHF operation planned

**Interpret:**

| Observation | Action |
|---|---|
| Flat noise floor at thermal noise level | Site is clean — proceed |
| Broadband hash, frequency-independent | Nearby power line, solar panel, or switching supply — note direction, avoid or accept |
| Discrete carriers or birdies | Identify before operating; may mask weak signals |
| Directional noise source | Note bearing; will affect receive on that heading |
| Elevated noise near old mine workings | Electrolytic action in sulfide ore can generate wideband noise — move site |

---

## STEP 6 — PATH ASSESSMENT (optional, 2 min)

For VHF or known HF path targets.

**Method:**
1. Tune TinySA to a known beacon, repeater input, or WSPR beacon frequency
2. Walk the ridge or terrain break slowly, note signal level vs position
3. Mark the point where signal drops 6 dB — this is your shadow boundary

**Use:** Pick the operating position that gives you path without full exposure to ridgeline wind and weather. On a Sierra ridgeline, 10 meters back from the crest often gives 90% of the path with significant wind shelter.

---

## STEP 7 — SITE SCORING

| Factor | Good (2 pts) | Marginal (1 pt) | Poor (0 pts) |
|---|---|---|---|
| Visual geology | Clean granite/alluvial | Mixed, some iron staining | Serpentinite, mine tailings |
| Compass deviation | < 2° | 2°–10° | > 10° |
| Test coil shift | < 2% | 2–10% | > 10% |
| Ground R | < 100Ω | 100–300Ω | > 300Ω |
| Noise floor | Clean | Minor hash | Significant noise |

**Score 8–10:** Deploy vertical or loop as planned.
**Score 5–7:** Use dipole or EFHW at height; add elevated radials if vertical required.
**Score < 5:** Relocate if possible; if fixed, use only dipole/EFHW, note degraded performance in log.

---

## ANTENNA SELECTION SUMMARY

| Site score | Recommended antenna | Notes |
|---|---|---|
| High score, dry granite | EFHW or dipole at height | Best efficiency on poor but predictable ground |
| High score, alluvial/riparian | Vertical with 4 elevated radials | Take advantage of better ground |
| Magnetite present (compass + shift) | Dipole or EFHW only | Avoid magnetic loops; vertical near-field compromised |
| Sulfides present (loss, no shift) | Any elevated antenna | Ground contact antennas lose efficiency; keep current off ground |
| Mixed/unknown geology | EFHW — most forgiving | Current maximum away from feedpoint, least ground-dependent |

---

## QUICK FIELD CARD (laminate separately)

```
SIERRA RF SITE ASSESSMENT — QUICK CHECK

1. VISUAL   □ Clean granite  □ Iron staining  □ Mine signs
2. COMPASS  Deviation: ____°  (>5° = iron body present)
3. COIL     F_air: ____MHz  F_ground: ____MHz  Shift: ____%
4. GROUND R ____Ω at 1m spacing / ____MHz
5. NOISE    □ Clean  □ Hash  □ Discrete carriers
6. SCORE    ____/10

ANTENNA CHOICE:
<5 → EFHW/dipole only
5-7 → dipole preferred, EFHW OK
8-10 → vertical viable with radials

Log GPS + score on R_ground card.
```

---

## REFERENCE — SIERRA NEVADA MINERAL RF SIGNATURES

| Mineral | Compass | Coil shift | Loss | Distribution |
|---|---|---|---|---|
| Magnetite | Yes | Large (>10%) | Moderate | Black grains, stream placers, contact zones |
| Pyrrhotite | Yes | Moderate | High | Metamorphic zones, contact aureoles |
| Pyrite | No | Minimal | High | Gold districts, hydrothermal veins |
| Galena | No | Minimal | Moderate | Lead districts (rare in Sierra) |
| Serpentinite | Slight | Moderate | High | Green rock, ultramafic intrusions |
| Hematite | No | Minimal | Low | Red/rust staining, mostly benign |
| Quartz/granite | No | None | Low | Dominant — poor but predictable |
| Alluvial soil | No | None | Low-mod | Valley floors — best Sierra ground |

---

*TM-FIELD-SAP-001 Rev A — KO6NNH — Merv's Dump*
