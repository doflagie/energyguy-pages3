# CALIFORNIA RF SITE ASSESSMENT PROCEDURE
## TM-FIELD-CA-001 Rev A

**Purpose:** Characterize a temporary operating site anywhere in California before antenna deployment. Covers geological interference, ground conductivity, noise floor, and path assessment using NanoVNA, TinySA, and compass.

**Scope:** HF/VHF QRP portable operations across California's physiographic provinces. See province quick-reference before site work.

**Time required:** 15–20 minutes full assessment. 5 minutes abbreviated go/no-go.

**Supersedes:** TM-FIELD-SAP-001 (Sierra Nevada only). That document remains valid for Sierra-specific detail.

---

## CALIFORNIA GEOLOGICAL PROVINCES — QUICK REFERENCE

Know your province before you arrive. It sets your expectations for every step below.

| Province | Dominant geology | Ground σ | Primary RF hazard |
|---|---|---|---|
| **Central Valley** | Deep alluvial sediments, irrigated | Excellent (>15 mS/m) | None — best ground in state |
| **Sierra Nevada** | Granite batholith, some contact zones | Very poor (<2 mS/m dry) | Low σ, some magnetite at contacts |
| **Coast Ranges** | Serpentinite, graywacke, shale | Poor–moderate, variable | Serpentinite ubiquitous — high µ + loss |
| **Klamath Mountains** | Ultramafic, serpentinite-heavy | Poor, very variable | Worst serpentinite concentration in state |
| **Transverse Ranges** | Metamorphic, granite, gabbro | Poor–moderate | Mixed; fault zones unpredictable |
| **Peninsular Ranges** | Granite, gabbro (San Diego area) | Poor | Similar to Sierra; gabbro adds loss |
| **Mojave Desert** | Dry alluvium, basalt, caliche | Very poor when dry | Caliche hardpan blocks ground current |
| **Cascades/Modoc** | Basalt, andesite, volcanic ash | Moderate (basalt better than granite) | Magnetite in basalt; pumice = near-zero σ |
| **Bay Area / SF** | Serpentinite, bay mud, Franciscan mélange | Variable: poor on hills, excellent in flats | Serpentinite on ridges; bay mud in lowlands |
| **Salton Sea / Imperial** | Saline lake beds, alluvium | Outstanding (>30 mS/m) | None — exceptional ground |
| **Owens Valley** | Alluvial, volcanic | Good–excellent | Minimal |
| **Death Valley** | Salt flats, alluvial fans, bedrock | Salt flats outstanding; alluvial fans poor | Extreme heat affects equipment |

**California's state rock is serpentinite.** It occurs throughout the Coast Ranges, Klamath Mountains, and Bay Area ridges. If you are operating anywhere between the Central Valley and the coast, assume serpentinite until proven otherwise.

---

## EQUIPMENT REQUIRED

| Item | Use |
|---|-----|
| NanoVNA (calibrated) | Ground conductivity, antenna loading, near-field survey |
| TinySA | Noise floor, path check, pattern distortion |
| Compass (baseplate) | Magnetic anomaly detection |
| Test coil (15 turns, ~5cm dia., air core, known L) | NanoVNA permeability probe |
| Known capacitor (100–470 pF) | Resonance reference with test coil |
| Two ground stakes, 30cm, insulated leads | NanoVNA ground impedance measurement |
| Short coax jumper, SMA | TinySA coupling |
| Site log card | Record results |

**Prepare test coil before departure:** Wind 15 turns of hookup wire on a 5cm form. Measure and record L with NanoVNA at home. This is your baseline.

---

## CAUTION

> **Lightning:** Mountain and desert thunderstorms develop rapidly. Do not drive stakes or erect any antenna if storms are within 10 miles or building over ridgelines. Suspend assessment immediately and shelter.

> **Desert heat:** NanoVNA and TinySA have thermal limits. In Death Valley and Mojave summer, keep instruments shaded. Calibration drifts with temperature — re-calibrate after any shade-to-sun transition.

> **Old mine workings:** California mine districts (Mother Lode, Cerro Gordo, Randsburg, etc.) may have unstable ground, open shafts, and toxic tailings. Do not enter mine structures. Keep clear of tailings piles.

---

## STEP 1 — PROVINCE IDENTIFICATION AND VISUAL GEOLOGY SURVEY (2 min)

Identify your province from the table above. Then walk the site:

**Look for — Hazards:**
- Green/gray-green rock → serpentinite (Coast Ranges, Klamath, Bay ridges) — high µ AND high loss
- Black heavy grains in stream gravel or dark inclusions in basalt → magnetite — high µ
- Brassy cubic crystals → pyrite — loss without shift (Mother Lode gold country)
- White hardpan layer in desert soil → caliche — blocks ground current, raises R dramatically
- Rust-orange surface staining → iron oxide — minor loss
- Disturbed ground, old adits, waste rock piles → mining district — unpredictable, avoid

**Look for — Bonuses:**
- Deep rich agricultural soil (Central Valley, coastal valleys) → excellent ground, vertical viable
- Dark bay mud or tidal flat → outstanding conductivity
- White salt crust or playa (Owens, Death Valley, Salton) → exceptional ground
- Irrigated field edge → treat as good alluvial
- Riparian flat, riverbed gravel → moderate to good, best available in mountains

**Record:** Province, dominant rock type, any anomalous minerals, any bonus conditions.

---

## STEP 2 — COMPASS MAGNETIC SURVEY (2 min)

Walk a 20-meter grid across the proposed antenna footprint.

**Method:**
1. Take a reference bearing to a distant landmark
2. Walk the antenna area in a grid, check bearing every 3–4 meters
3. Note any deviation from reference

**Interpret:**

| Deviation | Implication |
|---|---|
| < 2° | Clean — no significant ferromagnetic material |
| 2°–5° | Minor iron content — proceed with awareness |
| 5°–15° | Magnetite or pyrrhotite — expect coil shift, elevated loop losses |
| > 15° | Strong iron body — avoid magnetic loops; near field of any antenna compromised |

**California-specific note:** Serpentinite produces mild compass deflection (1°–4°) but severe RF loss. Do not use compass deviation alone to clear a serpentinite site — always follow with the coil test (Step 3).

---

## STEP 3 — NANOVNA PERMEABILITY PROBE (3 min)

**Method:**
1. Connect test coil to NanoVNA port 1 with known cap in parallel
2. In air, confirm resonant frequency matches home baseline (F_air)
3. Lower coil to within 2cm of ground at 3–4 points across the site
4. Record resonant frequency (F_ground) and Q at each point

**Interpret:**

| Observation | Likely cause | Action |
|---|---|---|
| F_ground same as F_air, high Q | Clean quartz/granite/alluvial | Proceed |
| F_ground same as F_air, low Q | Conductive sulfides (pyrite, galena) | Loss without permeability — avoid ground contact antennas |
| F_ground lower than F_air, moderate Q | Magnetite, basalt inclusions | Frequency shift = permeability — avoid magnetic loops |
| F_ground lower, low Q | Serpentinite, skarn, pyrrhotite | Both effects — worst case |
| F_ground higher than F_air | Unusual — check coil contact with ground | Re-test in clear air |

**Desert note:** Dry caliche and dry alluvium show no shift and minimal loss at the surface. The RF problem is deeper — caliche prevents ground current from reaching moist subsoil. Step 4 will catch this.

---

## STEP 4 — NANOVNA GROUND CONDUCTIVITY MEASUREMENT (5 min)

**Method:**
1. Drive two stakes 1 meter apart in proposed ground zone
2. Connect NanoVNA across the stakes
3. Sweep 1–30 MHz, note resistive component R at primary operating frequency

**Interpret:**

| Ground R (1m spacing) | Approx σ | California context | Antenna guidance |
|---|---|---|---|
| < 20Ω | > 30 mS/m | Central Valley irrigated, Salton, bay mud, salt flat | Vertical excellent; minimal radials needed |
| 20–100Ω | 5–30 mS/m | Good alluvial, coastal plains, Owens Valley | Vertical viable with 4 radials |
| 100–300Ω | 1–5 mS/m | Dry alluvial, valley grassland, some basalt | Elevated radials preferred; 4–8 minimum |
| 300–600Ω | 0.5–1 mS/m | Dry granite, some serpentinite, desert caliche | Dipole or EFHW at height preferred |
| > 600Ω | < 0.5 mS/m | Dry Sierra granite, pumice, dry caliche over bedrock | EFHW or dipole only; vertical very inefficient |

**Caliche warning:** If Step 1 showed caliche and R is very high despite visual alluvial soil, the hardpan is blocking ground current. Adding more radials at the surface helps less than usual — elevated radials are mandatory.

**Seasonal note:** Central Valley R drops 3–5× during irrigation season. Coastal sites improve after winter rain. Sierra and desert sites are 5–10× worse in dry season.

**Record on R_ground site log card.**

---

## STEP 5 — TINYSA NOISE FLOOR SURVEY (3 min)

**Method:**
1. Connect TinySA to short (30cm) wire whip
2. Sweep 1–30 MHz; note noise floor and any signals or hash
3. Rotate 360° slowly at 7 MHz and 14 MHz
4. Check 50 MHz and 144 MHz if VHF planned

**California-specific noise sources:**

| Observation | Likely source | Action |
|---|---|---|
| Broadband hash below 10 MHz, directional | High-voltage transmission line (California grid is dense) | Note bearing; orient antenna null toward it |
| Switching hash, omnidirectional | Solar inverter — very common in California | Move 50+ meters from buildings/RVs |
| Structured noise near 60 Hz harmonics | Agricultural pump motors | Common in Central Valley and Salton |
| VHF hash 30–50 MHz | Fire lookout, repeater infrastructure | Usually not a problem but note it |
| Quiet across all bands | Clean site — proceed | |
| Elevated noise near old mine workings | Electrolytic action in sulfide ore | Move site |

**Coastal note:** Salt air increases noise coupling from power infrastructure near the coast. Elevated noise floor is common within 500m of power lines in coastal zones.

---

## STEP 6 — PATH ASSESSMENT (optional, 2 min)

**Method:**
1. Tune TinySA to known beacon, repeater input, or WSPR frequency
2. Walk terrain breaks, note signal level vs position
3. Mark 6 dB drop point — this is your shadow boundary

**California terrain considerations:**

- **Ridge operating:** Position 10–15m back from crest — gains wind shelter with minimal path loss; reduces lightning exposure
- **Central Valley:** Flat terrain; horizon is the limit; noise floor more important than path here
- **Coastal bluffs:** Outstanding for DX but salt-air corrosion of connectors is accelerated; inspect and clean connections before and after
- **Desert basin:** Ground wave extends well on good ground (Salton, Owens); NVIS poor in dry desert due to low ionospheric absorption angle in summer
- **Mountain pass:** Often better than ridgeline — natural wind break, path in two directions, easier to access

---

## STEP 7 — SITE SCORING

| Factor | Good (2 pts) | Marginal (1 pt) | Poor (0 pts) |
|---|---|---|---|
| Visual geology | Alluvial, salt flat, irrigated, bay mud | Basalt, dry alluvial, mixed | Serpentinite, granite, caliche, mine tailings |
| Compass deviation | < 2° | 2°–10° | > 10° |
| Test coil | No shift, high Q | Minor shift or minor loss | Significant shift and/or low Q |
| Ground R | < 100Ω | 100–300Ω | > 300Ω |
| Noise floor | Clean | Minor hash | Significant noise |

**Score 8–10:** Deploy vertical or loop as planned.
**Score 5–7:** Use dipole or EFHW at height; add elevated radials if vertical required.
**Score < 5:** Relocate if possible. If fixed, EFHW or dipole only; note degraded performance.

---

## ANTENNA SELECTION SUMMARY

| Condition | Recommended | Avoid |
|---|---|---|
| High score, alluvial/valley/coastal | Vertical with 4 elevated radials | Nothing — take advantage of good ground |
| High score, salt flat or bay mud | Vertical with minimal radials | Overthinking it — just set up |
| Magnetite present (compass + coil shift) | Dipole or EFHW only | Magnetic loops — high near-field loss |
| Serpentinite (coil loss + mild shift) | EFHW or dipole at height | Vertical, magnetic loop |
| Caliche (high R despite alluvial surface) | EFHW or elevated dipole | Surface radial systems — caliche blocks them |
| Dry Sierra/Peninsular granite | EFHW — most forgiving | Verticals without serious elevated radial work |
| Desert dry (Mojave, Death Valley lowlands) | EFHW or dipole | Assume poor until R measurement proves otherwise |
| Volcanic (Cascades, Modoc) | Vertical viable — basalt σ moderate | Pumice — treat same as dry granite |

---

## QUICK FIELD CARD (laminate)

```
CALIFORNIA RF SITE ASSESSMENT

PROVINCE: ______________________

1. VISUAL
   □ Serpentinite (green rock)    □ Magnetite (black grains)
   □ Pyrite (brassy cubes)        □ Caliche (white hardpan)
   □ Mine workings nearby         □ Good alluvial/valley/salt

2. COMPASS  Deviation: ____°
   (>5° = iron body;  1-4° may be serpentinite — check coil)

3. COIL     F_air: ____MHz  F_ground: ____MHz
   Shift: ____%   Q: □High □Med □Low

4. GROUND R  ____Ω at 1m / ____MHz   Season: □Wet □Dry

5. NOISE    □ Clean  □ Solar hash  □ Power line  □ Other: ____

6. SCORE    ____/10

ANTENNA:
<5 → EFHW/dipole only
5–7 → dipole preferred, EFHW OK
8–10 → vertical viable with radials

Coastal bonus: check connectors for salt corrosion.
Desert caution: re-cal instruments after temperature change.

Log GPS + score on R_ground card.  KO6NNH
```

---

## REFERENCE — CALIFORNIA MINERALS AND FORMATIONS: RF SIGNATURES

| Material | Compass | Coil shift | Loss | Where found in CA |
|---|---|---|---|---|
| Magnetite | Yes | Large | Moderate | Basalt inclusions (Cascades, Coast Ranges), stream placers, contact zones |
| Serpentinite | Slight | Moderate | High | Coast Ranges, Klamath, Bay Area ridges — state rock, very common |
| Pyrrhotite | Yes | Moderate | High | Metamorphic aureoles, Klamath |
| Pyrite | No | Minimal | High | Mother Lode gold belt, hydrothermal veins statewide |
| Galena | No | Minimal | Moderate | Cerro Gordo, Mojave lead districts |
| Gabbro | No | Minimal–low | Low–mod | Peninsular Ranges, some Coast Ranges — denser than granite, slightly better σ |
| Basalt | Slight | Low–mod | Low | Cascades, Modoc Plateau, some Coast Ranges — magnetite inclusions vary |
| Pumice / volcanic ash | No | None | Very low | Lassen, Shasta area — near-zero conductivity |
| Caliche | No | None | High (R) | Mojave, desert foothills — hardpan blocks current, high R despite soil above |
| Dry alluvium | No | None | High (R) | Desert fans, Owens, Mojave — excellent when wet, very poor when dry |
| Irrigated alluvium | No | None | Very low | Central Valley — outstanding ground |
| Bay mud | No | None | Very low | SF Bay margins — outstanding ground |
| Salt flat / playa | No | None | Very low | Salton, Owens, Death Valley — outstanding |
| Granite (Sierra/Peninsular) | No | None | Low-R | Poor but predictable — most consistent poor ground in state |
| Franciscan mélange | Variable | Variable | Variable | Bay Area, Coast Ranges — mixed rock types, measure every site |

---

## CALIFORNIA GROUND CONDUCTIVITY BY PROVINCE (seasonal range)

| Province | Wet season σ | Dry season σ | Notes |
|---|---|---|---|
| Central Valley irrigated | 20–50 mS/m | 10–30 mS/m | Irrigation maintains conductivity year-round |
| Salton Sea basin | 30–100 mS/m | 20–50 mS/m | Saline soils, outstanding year-round |
| Coastal plains | 5–20 mS/m | 2–10 mS/m | Salt spray and marine layer help |
| Bay Area lowlands | 10–30 mS/m | 5–15 mS/m | Bay mud zones excellent |
| Owens Valley floor | 5–20 mS/m | 2–10 mS/m | Drops significantly in late summer |
| Cascades/basalt | 3–10 mS/m | 1–5 mS/m | Better than granite but not valley-class |
| Coast Ranges / serpentinite | 1–5 mS/m | 0.5–2 mS/m | Serpentinite zones lower end |
| Transverse/Peninsular granite | 1–3 mS/m | 0.3–1 mS/m | Similar to Sierra |
| Sierra Nevada granite | 0.5–3 mS/m | 0.2–0.8 mS/m | Worst in state when dry |
| Mojave / desert alluvial | 0.5–5 mS/m | 0.1–0.5 mS/m | Enormous seasonal swing |
| Death Valley salt flat | 50–200 mS/m | 30–100 mS/m | Best ground in California |

---

*TM-FIELD-CA-001 Rev A — KO6NNH — Merv's Dump*
*See also: TM-FIELD-SAP-001 (Sierra Nevada detail), R_ground_site_log_card*
