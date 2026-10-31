# Field Antenna Notes
*Last updated: 2026-09-29 (rev 4)*

## 5.6m Telescoping Whip

Primary HF field antenna — already in field kit. Covers 20m bare, 40m and 80m with loading coils, 10m–17m by retraction.

### Connector and Adapters

Base connector is M10 screw thread. Connect via M10 adapter base: M10→SMA-female for H4M/NanoVNA/TinySA; M10→BNC-female for (tr)uSDX; M10→SO-239 for Z-match/ATU-100. See mirror_mount_stud.md for stud/mount options.

### Tuning Principle — Shorten from the Top Only

- **Always retract top sections** to shorten — never adjust from the base
- Base is the feedpoint — moving it ruins antenna geometry
- Current maximum is at the base, minimum at the tip — top sections contribute least to radiation
- Shortening from top: moves resonance up in frequency, minimal effect on radiation pattern
- Each section retracted raises resonant frequency

### Approximate Resonant Lengths

Full 5.6m whip resonates near 13 MHz. Extend or retract to reach target band:

| Band | Target freq | Approx length | Notes |
|---|---|---|---|
| 20m | 14.050 MHz | ~5.0–5.2m | Check bare first — may need slight retraction |
| 17m | 18.090 MHz | ~3.9m | Retract top sections |
| 15m | 21.050 MHz | ~3.35m | Retract further |
| 12m | 24.910 MHz | ~2.83m | |
| 10m | 28.050 MHz | ~2.51m | Significant retraction |

Use NanoVNA to find actual resonant length — these are starting points, not cut dimensions.

### 20m — Bare Whip

5.6m = 0.26 wavelengths at 14.2 MHz — close to quarter wave without modification.

- Connect NanoVNA at feedpoint (SOLT calibrated at feedpoint end)
- Check SWR dip location with whip fully extended
- If dip is above 20m (>14.35 MHz): whip is slightly short — extend any partially retracted sections
- If dip is below 20m (<14.0 MHz): whip is slightly long — retract top section slightly
- Adjust in small increments, re-check each time

### 40m — Loading Coil Required

5.6m is too short for 40m (λ/4 = 10m) — needs a loading coil to electrically lengthen it.

**Coil placement:** Center load at the 2.8m point (midpoint of whip) — more efficient than base loading, lower loss, better radiation resistance.

**Target inductance:** 22–33 µH

**Starting point:** ~60–70 turns of 18 AWG on a 1.5" diameter form
- Wind long (more turns than needed)
- Remove turns to tune upward in frequency
- Fewer turns needed if whip is shortened from top

**Field storage:** Neoprene camera lens pouch — coils are cylinders, lens cases are designed for cylinders. A pouch sized for a 50mm or 85mm prime lens fits a coil wound on a 1.5" form (~50–54mm OD with windings). Drawstring top accommodates leads at both ends. Leave enough lead length inside the pouch to flex without kinking at the winding exit — the leads are the fragile part. $5–8 for a mixed-size pack on Amazon.

**Smith chart reading:**
- Dot left of horizontal centerline = inductive = too many turns, remove some
- Dot right of horizontal centerline = capacitive = too few turns, add some
- Dot on centerline = resonance

**NanoVNA settings for 40m:**
- Frequency range: 6.5–8.0 MHz
- CH0 S11 — one trace SWR, one trace Smith chart
- Markers: 7.000, 7.200 MHz + float marker on dip

**If using the multiband slider coil:** adjust slider to the ~22–33 µH position (roughly the mid-range of the winding) for 40m — no separate fixed coil needed. See multiband slider section below.

### Multiband Slider Loading Coil (80m–30m)

A sliding-contact loading coil — a solenoid with a movable clip or band that selects how many turns are in circuit. Slide toward more turns = lower frequency; slide toward fewer turns = higher frequency. One coil covers 80m, 60m, 40m, and 30m on the 5.6m whip. Replaces the fixed 40m coil and the fixed 80m coil.

**Coil placement:** Center load at the 2.8m point (midpoint of whip). Same mount position for all bands — only the slider moves.

**Band coverage — approximate slider positions:**

| Band | Freq | Inductance needed | Slider position |
|---|---|---|---|
| 80m | 3.65 MHz | 80–130 µH | Near maximum turns |
| 60m | 5.33 MHz | 35–55 µH | ~60% of winding |
| 40m | 7.1 MHz | 22–33 µH | ~40% of winding |
| 30m | 10.1 MHz | 10–15 µH | ~20% of winding |
| 20m | 14.2 MHz | Minimal | Whip nearly resonant bare; slider at minimum or bypass |

Slider positions are approximate — NanoVNA is the definitive guide. Mark confirmed positions on the form with a permanent marker after first field session.

**Tuning procedure:**
1. SOLT calibrate NanoVNA at feedpoint
2. Set NanoVNA range for target band (see table below)
3. Attach coil at 2.8m, connect to feedpoint
4. Observe SWR dip — note frequency relative to target
5. Slide toward more turns if dip is too high; fewer turns if dip is too low
6. Re-observe dip — iterate until dip is on target frequency
7. Confirm on Smith chart — dot on horizontal centerline = resonance
8. Mark slider position on form

**NanoVNA frequency ranges by band:**

| Band | Start | Stop |
|---|---|---|
| 80m | 3.0 MHz | 4.5 MHz |
| 60m | 4.8 MHz | 6.0 MHz |
| 40m | 6.5 MHz | 8.0 MHz |
| 30m | 9.5 MHz | 11.0 MHz |

**Bandwidth narrows on lower bands:** 80m center-loaded whip has high Q and narrow bandwidth — tune to your operating frequency (net, calling, QRP watering hole) and recheck if moving more than ~50 kHz. 40m and 30m are more forgiving.

**Slider maintenance:** The sliding contact is bare metal on bare wire — it oxidizes. If SWR is unexpectedly high, work the slider back and forth a few times to break through surface oxide. A drop of DeOxit D5 on the contact every few sessions keeps it clean. Do not let the slider sit at the same position for months.

**Counterpoise — band-dependent:**
- 80m: λ/4 = 20.5m (67 ft) — use 2–3 staked radials at fixed position, or shorter elevated counterpoise + Z-match
- 40m: λ/4 = 10.5m — practical with linked counterpoise
- 30m: λ/4 = 7.4m — manageable
- Lower bands need more wire; upper bands less

**80m performance note:** Radiation resistance is low (2–5Ω) on a heavily-loaded short vertical. Mainly NVIS regional at QRP — workable CW contact is possible, not a guarantee. 40m and 30m will outperform 80m on this antenna.

**Storage:** Larger case than the 40m coil — 100–135mm prime lens pouch or 70–200mm zoom lens pouch. Label the pouch. Mark band positions on the form after field calibration.

### NanoVNA Tuning Procedure

1. SOLT calibrate at the feedpoint end of your coax — not at the NanoVNA body
2. Connect calibrated port to whip feedpoint
3. Note SWR dip frequency
4. Adjust whip length (20m), coil turns (fixed coil), or slider position (slider coil) to move dip to target
5. Confirm on Smith chart — dot on horizontal centerline = resonance
6. Verify SWR: target <2:1 at operating frequency, <1.5:1 ideal

Recalibrate if cable length changes between sessions.

### Feedpoint Impedance

Expected at resonance over a modest radial system: **15–35Ω**

This is normal for a vertical over limited ground — not a fault. The Z-match and ATU-100 both handle this range easily. Do not try to tune away from resonance to hit 50Ω — tune for resonance first, let the tuner handle the impedance.

### Counterpoise

λ/4 radials per band give best results. Frame of wheelchair or walker provides 1–2m effective conductor — covers 10m/12m. Supplemental wire needed for lower bands.

All lengths are free-space λ/4 — cut long and trim to resonance with NanoVNA.

| Band | Freq | λ/4 | Cut to (long) |
|---|---|---|---|
| 80m | 3.650 MHz | 20.53m (67.4 ft) | 20.75m |
| 60m | 5.3715 MHz | 13.96m (45.8 ft) | 14.15m |
| 40m | 7.150 MHz | 10.48m (34.4 ft) | 10.65m |
| 30m | 10.125 MHz | 7.40m (24.3 ft) | 7.55m |
| 20m | 14.175 MHz | 5.29m (17.4 ft) | 5.40m |
| 17m | 18.100 MHz | 4.14m (13.6 ft) | 4.20m |
| 15m | 21.225 MHz | 3.53m (11.6 ft) | 3.60m |
| 12m | 24.940 MHz | 3.01m (9.9 ft) | 3.10m |
| 10m | 28.500 MHz | 2.63m (8.6 ft) | 2.70m |
| 6m | 50.500 MHz | 1.49m (4.9 ft) | 1.55m |

### Ten-Band Linked Counterpoise — K6ARK Banana Plug Method

A single wire 20.53m long with K6ARK 2.5mm banana plug taps at each band's λ/4 distance from the feedpoint. Disconnect at the band tap you're using; fold back or coil the excess. One wire covers 80m through 6m, including 60m.

**Tap positions — cumulative from feedpoint ground lug:**

| Band tap | Distance from feedpoint | Segment from prev tap |
|---|---|---|
| 6m | 1.49m (4.9 ft) | 1.49m from feedpoint |
| 10m | 2.63m (8.6 ft) | 1.14m from 6m tap |
| 12m | 3.01m (9.9 ft) | 0.38m from 10m tap |
| 15m | 3.53m (11.6 ft) | 0.52m from 12m tap |
| 17m | 4.14m (13.6 ft) | 0.61m from 15m tap |
| 20m | 5.29m (17.4 ft) | 1.15m from 17m tap |
| 30m | 7.40m (24.3 ft) | 2.11m from 20m tap |
| 40m | 10.48m (34.4 ft) | 3.08m from 30m tap |
| 60m | 13.96m (45.8 ft) | 3.48m from 40m tap |
| 80m end | 20.53m (67.4 ft) | 6.57m from 60m tap |

**Construction:**
1. Cut 30 AWG PTFE wire to 20.75m (slightly long)
2. Starting at the feedpoint end, mark each tap position with a paint pen before installing connectors
3. At each tap position, install K6ARK link: male banana soldered to feedpoint-side wire, female banana in adhesive heat-shrink sleeve on far side
4. Label each tap in the heat shrink before gluing (band name — 6m, 10m, etc.)
5. After all taps installed, connect all links and verify total length = ~20.53m
6. Trim 80m end to resonance with NanoVNA (SOLT calibrated at feedpoint)
7. Working inward: verify each tap at its band with NanoVNA, trim segment if needed

**Important:** The 10m-to-12m (0.38m) and 12m-to-15m (0.52m) segments are short — mark and label these taps clearly before installing connectors. Color-coded heat shrink labels prevent confusion.

**Using the counterpoise:**
- 80m: all links connected, full wire deployed
- 60m: disconnect at 60m tap, fold back the outer 6.57m
- 40m: disconnect at 40m tap, fold back the outer 9.05m
- 20m: disconnect at 20m tap, fold back outer 15.24m
- Higher bands: disconnect at the appropriate tap, coil the excess

**Field tip:** The 40m-to-80m total wire (9.05m from 40m tap) is still long to fold back. For fixed-position 80m operation, stake it out as 2–3 radials instead.

**Ground surface matters:** Grass/soil outperforms concrete. More radials = lower feedpoint resistance, closer to 50Ω. Check NanoVNA with 2 radials, add 2 more, compare.

**40m fixed position:** 10.5m trailing wire is awkward in tight spaces. Stake 2–3 shorter radials around the position instead.

### Trapped Counterpoise — Nine-Band Single Wire

A trapped counterpoise uses parallel LC circuits (traps) placed at each band's λ/4 position. At its resonant frequency each trap presents very high impedance — stopping current at that point. Below resonance, the trap looks inductive, contributing electrical length so each wire section is physically *shorter* than free-space λ/4.

**Advantage over linked design:** Deploy once, automatic — no connectors to mate per band change. Works on whatever band you transmit.

**Cost:** 8 traps, each requiring a toroid and NP0 capacitor. Build time: 4–8 hours at the bench. The K6ARK linked design is faster to build and easier to troubleshoot. Trapped is better for a semi-permanent portable setup.

**Trap construction — parallel LC:**

Each trap is a toroid coil with a capacitor wired in parallel across it. The parallel combination presents very high impedance at resonance, near-zero impedance away from resonance.

**Iron powder AL values (iron powder only — NOT ferrite):**
- T37-2 (red, 0.37" OD): AL = 4.0 nH/T²
- T50-2 (red, 0.50" OD): AL = 5.7 nH/T²

L (nH) = AL × n². Ferrite cores (Mix 43, 73) have AL 10× higher — do not use ferrite values for these calculations.

**Component values — starting points (trim with NanoVNA):**

| Band | Target freq | Core | Turns | Inductance | Starting cap | Vol. rating |
|---|---|---|---|---|---|---|
| 6m | 50.5 MHz | T37-2 (red) | 8T | ~256 nH | 39 pF NP0 | 50V min |
| 10m | 28.5 MHz | T37-2 (red) | 14T | ~784 nH | 39 pF NP0 | 50V min |
| 12m | 24.94 MHz | T37-2 (red) | 16T | ~1024 nH | 39 pF NP0 | 50V min |
| 15m | 21.225 MHz | T37-2 (red) | 19T | ~1444 nH | 39 pF NP0 | 50V min |
| 17m | 18.1 MHz | T37-2 (red) | 22T | ~1936 nH | 39 pF NP0 | 50V min |
| 20m | 14.175 MHz | T37-2 (red) | 29T | ~3364 nH | 39 pF NP0 | 50V min |
| 30m | 10.125 MHz | T37-2 (red) | 33T | ~4356 nH | 56 pF NP0 | 50V min |
| 40m | 7.15 MHz | T50-2 (red) | 30T | ~5130 nH | 100 pF NP0 | 50V min |

40m uses T50-2 (larger core, 0.5" OD) because T37-2 won't physically fit 30 turns on the 0.37" OD form.
**NP0/C0G capacitors only** — X7R and Y5V dielectrics drift with temperature and voltage, killing resonance accuracy.
At <20W, 50V voltage rating is adequate throughout the counterpoise.
Wind with 26–28 AWG enameled (magnet wire) or 30 AWG PTFE. T37-2 fits 33T of 30 AWG PTFE; strip insulation mechanically before soldering — do not burn PTFE (toxic fumes above 260°C).

**Building each trap:**
1. Wind turns as specified — close-wound on toroid
2. Solder cap across the winding leads — parallel connection
3. **Test with NanoVNA before doing anything else** (see below)
4. Adjust: add a turn to lower frequency, remove a turn to raise; use larger cap to lower, smaller to raise
5. When on frequency: weatherproof with heat shrink, then self-amalgamating tape

**NanoVNA trap testing — S21 method:**
- Connect trap in series between CH0 and CH1 (in-line)
- Sweep through target frequency
- Look for a sharp S21 attenuation DIP — that is the parallel resonant frequency
- Dip should be deep (>20 dB) and narrow — indicates good Q
- Shallow dip = resistive loss (poor core choice or damaged cap)
- Adjust cap value or turns to move dip to exact target frequency

**Assembly — install order and section lengths:**

Trap inductance below resonance adds electrical length to lower bands. This means each physical wire section is *shorter* than the free-space segment from the linked counterpoise table. Start with the linked tap positions as a first approximation, then trim after assembly.

Install traps from feedpoint outward (6m trap first, 40m trap last):

1. Install 6m trap at ~1.49m from feedpoint
2. Connect NanoVNA at feedpoint end; verify 6m resonance; trim this section to tune
3. Install 10m trap; verify 10m resonance; trim section between 6m and 10m traps
4. Continue outward, one trap at a time, verifying each band before proceeding
5. 80m requires no trap — the full wire end is the 80m counterpoise; trim the final section

**Tuning rule:** Work from highest band (6m) to lowest (40m). Trimming a shorter inner section does not affect the outer sections' resonances. Going the other direction does. Expect 2–3 passes to nail all bands.

**Enclosure options (field portable):**
- Minimum: heat shrink over trap body (keeps leads together, protects from shorts)
- Better: heat shrink + self-amalgamating tape (weatherproof)
- Best: small epoxy-filled heat shrink (heaviest, most durable)

**Wire between traps:** 30 AWG PTFE — same as the linked design. Solder trap leads directly to wire, then heat shrink over junction.

**When assembled:** A trapped counterpoise is a single wire that automatically presents ~λ/4 on every band without any connector changes. Clip ground lug, lay wire out, done.

### Tuner Integration

- **Z-match:** preferred for field use — passive, instant, no tune sequence; handles 15–35Ω feedpoint directly
- **ATU-100:** backup or shack use; stores memories for fast re-tune; handles up to ~10:1 SWR
- **NanoVNA pre-characterization trick:** measure feedpoint impedance at home, note it, pre-set Z-match to that impedance before going to the field — speeds up field tuning considerably

### Performance at QRP

At 5–20W into a resonant whip with a modest radial system:
- 20m and up: competitive for domestic and DX contacts
- 40m with loading coil: domestic and regional, some DX under good conditions
- CW gives effective 10–15 dB advantage over SSB — compensates for modest antenna

### Power Handling — FT-991A (100W)

The coil is the weak link at 100W, not the whip itself — voltage/current across the
loading coil scale with √P, so 5W→100W is a 20× power jump → ~4.5× more coil voltage
and current. Two failure modes appear that don't exist at QRP: turn-to-turn/PVC surface
tracking under field moisture/dust, and coil self-heating (most of the "missing"
efficiency on low bands becomes heat, concentrated in a thin-wire coil).

**By band:**

| Band | 100W with this whip? | Why |
|---|---|---|
| 20m–6m (bare whip) | Yes, no caveats | No coil in circuit |
| 40m, 30m | Yes, existing coil is fine | Low reactance + high R_rad keeps coil voltage under ~700V, well within margin for standard magnet wire with any real air gap |
| 80m | Only with a coil built for it | Full-winding reactance drives coil voltage to ~2kV+ at 100W |
| 160m | Only with a coil built for it | Highest reactance/lowest R_rad — worst case, ~4.4kV across the coil at 100W |

Full derivation (worked voltage/current/heat tables per band, plus a complete 100W coil
redesign — 4" PVC form, #14 AWG, wider turn spacing) is in `5.6M_telescopic_whip.md`.

**Caveat:** that derivation was done for the *base-loaded* tapped-coil version of this
whip (coil at the feedpoint). This file's coil is *center-loaded* at the 2.8m point —
different topology, different exact inductance/turn-count numbers, though the same
underlying physics (which bands stress the coil and why) carries over. If building the
center-loaded coil for 100W use, redo the same worked-example method (current from
R_rad + R_coil + R_ground, then V_coil = I × X_L, then I²R for heat) for the actual
inductance values in the slider-coil table above rather than assuming the base-loaded
numbers apply directly.

### H4M Use

H4M TX power 10–30 mW — whip resonance is critical at this power level, mismatch wastes most of the available signal. Tune with NanoVNA before transmitting. For real contacts use (tr)uSDX; H4M TX is for antenna testing and short-range work.

### Weatherproofing

**Sections (stainless steel):** Handle rain without corrosion. Wet sections shift resonance 1–3% lower — retune if operating in steady rain. Wipe sections dry before collapsing to prevent grit from grinding the friction joints.

**M10 base:** Not sealed — water enters threads and electrical contact area. Wrap M10 adapter joint with self-amalgamating tape when operating in rain; unwrap and inspect contact after wet use.

**Wind:** Primary field risk.
- Each section must be firmly friction-locked before extending the next — loose sections collapse in gusts
- Thin top sections (1–2mm diameter) kink permanently in a sustained gust; retract top 1–2 sections above ~15 mph sustained wind
- On mirror mount / rebar stake: the whip acts as a long lever on the mount — guy the lower whip to the rebar with a rubber band or light cord in gusty conditions

**After wet field use:** Extend all sections, wipe dry, then collapse. Never store collapsed and wet — joints seize.

**Summary:** Fine in light rain with feedpoint taped; retract top sections in wind above 15–20 mph. Not a storm antenna.

---

## TinySA Ultra — Field Use

*Full menu reference: `tinysa_ultra_menu.md` (both repos)*

Spectrum analyzer, not a receiver. Measures RF signals — power levels, frequency, harmonics, spurious emissions. Complements the NanoVNA (antenna impedance) and H4M (signal demodulation). All three together cover the full RF test picture.

**Frequency coverage:** 100 kHz – 5.3 GHz
- LOW port (SMA): 100 kHz – 960 MHz — HF through UHF
- HIGH port (SMA): 100 MHz – 5.3 GHz — VHF through microwave

**What it measures:** Signal level in dBm, frequency, bandwidth, harmonics, spurious outputs. Purpose-built analyzer — better dynamic range and accuracy than the spectrum display on H4M or NanoVNA.

### Field Uses

| Task | How |
|---|---|
| Check (tr)uSDX harmonics | Connect via attenuator, TX into dummy load, observe 2nd/3rd harmonic levels |
| Verify LPF performance | Compare fundamental vs harmonic levels before and after LPF |
| Measure TX output power | Calibrated dBm reading at antenna port — verify rig is outputting expected power |
| Identify unknown signals | Frequency and level measurement during band survey |
| Check antenna system loss | Compare signal level at rig vs at feedpoint — confirms coax/connector loss |
| Calibration reference | GPS-disciplined calibration system feeds known signal — verify TinySA accuracy |

### Connecting to (tr)uSDX for Harmonic Check

1. (tr)uSDX into dummy load (SO-239)
2. Short coax from dummy load BNC tap to TinySA LOW input
3. Use 30 dB SMA attenuator inline — (tr)uSDX at 5W = +37 dBm, TinySA max input is +10 dBm
4. Set TinySA frequency range to cover fundamental + 2nd/3rd harmonic
5. Key up (tr)uSDX, observe spectrum

**Harmonic limits (FCC Part 97):** Spurious emissions must be at least 43 dB below fundamental (at 5W). Any harmonic within 43 dB of fundamental = LPF problem.

### Connector

SMA female on both ports. Use SMA M-M barrel or right-angle adapter from connector bag. Same adapter pool as H4M and NanoVNA — no special connectors needed.

### Antennas for TinySA Ultra

Two ports, two antenna strategies.

**LOW port (100 kHz – 960 MHz) — HF through UHF:**

| Antenna | Notes |
|---|---|
| 5.6m telescoping whip | Already in kit; good HF band survey; doesn't need to be resonant for monitoring |
| Long random wire | Any spare wire clipped to SMA center pin via adapter; 10–20 ft gives good HF sensitivity |
| YouLoop passive loop | HF weak signal; directional — rotate to null interference (build: § YouLoop — Passive Magnetic Loop Construction, below) |

**HIGH port (100 MHz – 5.3 GHz) — VHF through microwave:**

| Antenna | Notes |
|---|---|
| ANT500 adjustable whip | Already in H4M kit; tune to λ/4 for band of interest; same whip serves both H4M and TinySA |
| Fixed whip cut to frequency | 2m=49cm, 70cm=16cm, ADS-B=6.9cm, 2.4GHz=3.1cm |

**For monitoring:** any antenna works — RX only, mismatch just reduces sensitivity, no damage risk.

**For harmonic checking your own TX:** no antenna — dummy load + coupling tap + 30 dB attenuator inline. Never connect TinySA to a live transmitter through an antenna.

**No new hardware needed:** ANT500 (already in H4M kit) covers HIGH port; 5.6m whip covers LOW port. Same adapters from connector bag.

### vs NanoVNA vs H4M

| Instrument | Primary use | What it can't do |
|---|---|---|
| NanoVNA | Antenna impedance, SWR, Smith chart | Measure absolute power levels |
| TinySA Ultra | Signal levels, harmonics, spectrum | Demodulate audio, antenna impedance |
| H4M | Demodulate, capture, band survey | Accurate power measurement |

Use all three together for a complete field RF picture — they don't overlap, they stack.

---

## YouLoop — Passive Magnetic Loop Construction

Receive-only noise-cancelling passive loop (NCPL design, the same topology Airspy's commercial YouLoop uses) — feeds both the TinySA LOW port and the ATS-25 Max BNC jack referenced elsewhere in this file. Two loop halves of coax joined by a deliberate cross-connection at one end (the "Möbius loop" trick that gives it common-mode noise rejection) and a small ferrite transformer at the other.

### Parts

| Part | Spec | Notes |
|---|---|---|
| Loop coax, 2× | ~1–1.5m each (RG-58 fine for DIY; official unit uses semi-rigid RG402/RG141) | Total loop circumference ~2–3m → diameter ~0.6m. Keep it in this range — bigger captures more signal but starts losing "electrically small loop" behavior at the top of HF, same tradeoff covered in the Carpet Loop evaluation below |
| Ferrite core | BN-73-302, 2-hole ("binocular") wideband core | The balun/transformer body |
| Magnet wire | 26–28 AWG enameled (same wire used for the 49:1 unun DIY build above) | For winding the core |
| Feedline pigtail | Short coax, BNC | Keep total feedline under ~10m — fully passive design, no gain to spare |
| Heat-shrink tubing | Enough for the crossover joint and core windings | Slide on *before* soldering |

### Construction

1. **Crossover junction (one end of the loop):** strip back each coax end to expose center conductor and shield. Solder loop-half A's **center conductor to loop-half B's shield**, and loop-half A's **shield to loop-half B's center conductor** — the cross, not a straight splice. This deliberate swap is the core of the design's noise rejection. Heat-shrink the joint for strain relief and insulation.
2. **Balun junction (other end of the loop):** wind the core with 4 turns from loop-half A through the binocular core, and 4 turns from loop-half B through the core on the other side — same "ugly transformer" winding technique as the 49:1 unun, different turns/core for a different job (coupling two coax ends together, not stepping impedance).
3. **Feedline:** connect the pigtail's center conductor and shield across the remaining winding. Tie the pigtail's shield to the loop coax's shield specifically **on the primary (loop) side of the balun** — this specific grounding choice measurably lowers noise pickup versus the other way around.
4. **Mechanical:** crossover joint just needs heat-shrink and tape. Balun junction is cleaner in a small project box or a short length of PVC pipe.

### Performance

- Fully passive, no tuning across 10 kHz–30 MHz (usable to ~300 MHz with reduced performance as the core rolls off).
- Rotate the whole loop to null a specific noise source or interferer — the capability the Carpet Loop below can't offer, since that one's fixed in place.
- Stays electrically small across essentially the whole HF range at this size, unlike a room-perimeter loop, which only behaves as a "true" small loop at the bottom of the SW bands.

---

## ATS-25 Max — Receive Antennas

SI4732-based RX-only receiver (AM/FM/LW/SW + SSB). ANT jack: **BNC**, twist-lock — shared connector, twist off the stock whip to substitute a build. RX-only antenna design is a different problem from the TX antennas above: optimize for signal-above-noise-floor and avoiding front-end overload, not radiated efficiency. No matching network or counterpoise required — the SI4732 front end tolerates mismatch far better than a transceiver.

**Why it's different from TX antenna design:**

- No ground-loss concern — receive antennas don't need a low-loss RF return path the way a transmitting vertical does; any reasonable wire or loop works.
- Height/noise floor matters more than antenna theory — getting the wire away from local switching supplies, LED drivers, and routers usually helps more than geometry.
- Overload, not weak signal, is the risk with long wires near strong local AM broadcast stations — the opposite failure mode from TX antennas.

**Build options, cheapest first:**

| Option | Build | Notes |
|---|---|---|
| Stock telescopic whip | None — already on unit | Fine for FM and daytime SW; extend fully for SW, retract for local FM |
| Long-wire lead, no solder | BNC-to-binding-post or BNC-to-alligator-clip adapter (~$5) + 10–20m any thin wire | Fastest real SWL upgrade over the stock whip; twist onto ANT jack, toss wire out a window or into a tree |
| Long-wire lead, soldered | Wire soldered to male BNC (center pin = signal, shell = ground/floating); heat-shrink strain relief | Permanent version of the above |
| YouLoop passive loop | Two ~1m loops of RG-58 (or any 50Ω coax) into a binocular-ferrite transformer, terminated male BNC — full build: § YouLoop — Passive Magnetic Loop Construction, above | Best option in RF-noisy environments — couples to the magnetic field, not the electric field most local noise rides on; rotate to null a specific noise source or station; no tuning needed across the whole SW range; same loop concept as the TinySA LOW-port YouLoop above |
| MW (AM broadcast) | None — use the internal ferrite bar | External BNC antenna not needed for MW; a separate *unconnected* tuned loop placed nearby can inductively boost weak AM DX if wanted |

**Parts already on hand:** the (tr)uSDX RF-test chain already uses BNC (dummy load, T-splitter, whip-base adapter) — check that parts bin before ordering new BNC hardware for options 2–4 above.

### Indoor/Stealth Option — Carpet Loop

A documented indoor SWL antenna (David Moisan, N1EU): 25–30 ft of 4- or 5-conductor cable run around a room's perimeter under the carpet, back to a small L-network tuner (365 pF variable cap + 2P6T band/longwire/ground switch + static-discharge diodes), feeding the receiver via BNC/phono/screw terminal. Solves a different problem than the YouLoop above — not better performance, but a durable stealth install where no outdoor wire or window-mounted loop is possible (apartment, HOA, spare room).

**Where it sits between "loop" and "wire":** whether a loop behaves as a true small magnetic loop (H-field selective, clean null pattern) or as a large single-turn frame (E-field sensitive, wire-like) depends on circumference relative to wavelength — and a room-perimeter loop crosses that boundary across the SW range:

| Band | λ | Loop circumference (~9m) | Electrical size | Behavior |
|---|---|---|---|---|
| 90m/75m SW (~3 MHz) | 100m | 9m | ~0.09λ | Electrically small — true small-loop behavior |
| 19m SW (~15 MHz) | 20m | 9m | ~0.45λ | Medium loop — current distribution non-uniform, more wire-like |
| 10m SW (~30 MHz) | 10m | 9m | ~0.9λ | Large loop — multi-lobed, essentially wire-like |

Compare to the YouLoop above (~1m diameter, ~3m circumference), which stays electrically small across the whole HF range — the carpet loop only behaves as a "true" small loop at the low end of SW. The original design's own "longwire mode" switch position is a tacit admission of this: at higher SW frequencies it's tuned and used more like a resonated random wire than a magnetic loop.

**What it actually delivers:**

- Noise rejection mainly via common-mode balance (multi-conductor cable feeding a balanced loop back to one point rejects local household E-field noise) rather than via small-loop H-field selectivity at most SW frequencies — a different mechanism than the YouLoop, but a real one.
- The L-network acts as a passive preselector, reducing front-end overload versus a broadband random wire — genuine benefit, independent of the loop-size question.
- **No rotation/nulling once installed** — the single biggest capability gap versus the YouLoop. This is a fixed noise-reduced compromise antenna, not a directional nulling tool; don't expect it to null a specific interferer the way a rotatable loop does.
- Durable underfoot — cable under carpet reported intact after 3 years of foot traffic.

**Verdict:** legitimate choice when the constraint is "can't run anything outdoors," not when the goal is directional weak-signal DX work — for that, the YouLoop (rotatable) remains the better tool.

---

## NanoVNA — Field Tuning Procedure

*Full menu reference: `nanovna_menu.md` (both repos)*

### Calibration — Do This First Every Session

SOLT calibration removes cable and connector losses from the measurement. Calibrate at the antenna end of your coax — not at the NanoVNA body.

1. Connect calibration kit to the far end of your coax (feedpoint end)
2. Perform SOLT: Short → Open → Load (50Ω) → Through in sequence
3. Save calibration
4. Connect antenna to calibrated port — measurement now starts at the feedpoint
5. Recalibrate if cable length changes between sessions

**Why it matters:** Calibrating at the NanoVNA end measures the cable + antenna. Calibrating at the feedpoint end measures only the antenna. At HF the difference is small for short cables but significant for longer runs.

### Display Setup

Two traces on CH0 S11:
- Trace 1: SWR — shows resonance dip clearly
- Trace 2: Smith chart — shows impedance character (inductive vs capacitive)

Markers:
- Fixed markers at band edges and operating frequency
- Float marker: place on the SWR dip — shows exact resonant frequency

### Band Settings

**40m:**
- Frequency range: 6.5–8.0 MHz
- Markers: 7.000, 7.030 (QRP), 7.200 MHz + float on dip

**20m:**
- Frequency range: 13.5–15.0 MHz
- Markers: 14.000, 14.057 (QRP), 14.225 MHz + float on dip

**17m:**
- Frequency range: 17.5–19.0 MHz
- Markers: 18.068, 18.090 MHz + float

**15m / 12m / 10m:** Set range ±1 MHz around band, markers at band edges + QRP calling frequency.

### Reading the Smith Chart

The Smith chart center = 50Ω resistive = perfect match. The horizontal centerline = purely resistive (no reactance).

| Dot position | Meaning | Action |
|---|---|---|
| Left of center on horizontal line | Inductive reactance — too long / too many coil turns | Shorten whip from top, or remove coil turns |
| Right of center on horizontal line | Capacitive reactance — too short / too few turns | Extend whip, or add coil turns |
| On horizontal centerline | Resonance — reactance cancelled | Good — note SWR value |
| Center of chart | 50Ω — perfect match | Done |
| Left half, off centerline | Inductive + resistive — common for short verticals | Tune toward centerline |

**Typical vertical feedpoint:** Dot will be left of center on a shortened vertical — inductive. Resonance puts it on the centerline, usually well left of center (low resistance, 15–35Ω). This is normal.

### Tuning Sequence — Whip

1. Full extension — note SWR dip frequency
2. If dip is too low (below target band): retract top section slightly, recheck
3. If dip is too high (above target band): extend further if possible, or add loading coil
4. Iterate in small steps — one section at a time
5. Confirm resonance on Smith chart — dot on horizontal centerline
6. Note final length for repeatability

### Tuning Sequence — Loading Coil (40m)

1. Wind coil long (~70 turns) — start below target frequency
2. Connect, note SWR dip and Smith chart position
3. If dot left of centerline (inductive, dip too low): remove 2–3 turns, recheck
4. If dot right of centerline (capacitive, dip too high): add turns
5. Iterate — each turn change moves resonance ~50–100 kHz at 40m
6. Final: dot on centerline, SWR dip at target frequency

### Acceptable SWR Targets

| Result | Interpretation |
|---|---|
| SWR < 1.5:1 | Excellent — minimal tuner correction needed |
| SWR 1.5–2:1 | Good — Z-match or ATU-100 handles easily |
| SWR 2–3:1 | Acceptable — tuner required, some loss |
| SWR > 3:1 | Antenna not resonant — retune before transmitting |

At QRP power levels, every dB matters. Aim for <2:1 before keying up.

### Pre-Characterization Trick

Measure feedpoint impedance at home with a known ground system. Note the impedance at each operating frequency. Pre-set the Z-match to that impedance before going to the field — speeds field tuning from minutes to seconds.

---

## Linked EFHW — End-Fed Half Wave

Most efficient HF option at QRP power levels. No ground system required — all power goes into radiation, not ground losses. ~85% efficiency. Resonant by design — no tuner needed when links are set correctly.

### Why EFHW Works

A half-wave antenna presents very high impedance at the feedpoint (~2500–3000Ω at resonance). The 49:1 unun steps this down to ~50Ω for coax feed. Because it's a half-wave, current is at a maximum in the middle and zero at the ends — the feedpoint at the end sees high voltage, low current, high impedance. The unun handles the mismatch.

No counterpoise required in theory — unlike a quarter-wave vertical which needs a ground plane. In practice, a short counterpoise wire (1–2m) from the unun chassis improves performance and reduces common-mode current on the coax shield.

### 49:1 Unun

Transforms ~2450Ω to ~50Ω. The critical component — quality matters.

**Commercial options (QRP rated):**
- **K6ARK EFHW Matching Unit Kit** — compact, field-optimized, purpose-built for portable EFHW; **kit in use**
- QRPGuys EFHW kit — ~$15, wound on FT-140-43, well-documented
- LNR Precision — pre-built, field-proven
- PackTenna — popular POTA option

**DIY:** FT-140-43 or FT-240-43 toroid; 2 turns primary, 14 turns secondary for 49:1. House in a small weatherproof enclosure with SO-239 for coax and a wire terminal for the antenna element.

**Connector at unun:** SO-239 → use SO-239→SMA M adapter from connector bag to feed from coax to rig or Z-match.

### Wire Lengths — Half Wave per Band

All lengths include ~1.5% velocity factor correction for insulated wire. Cut long, trim to resonance with NanoVNA — expect final trim of 2–5% depending on wire insulation.

| Band | Frequency | Half-wave length | Cut to (long) |
|---|---|---|---|
| 80m | 3.650 MHz | 40.5m (132.8 ft) | 40.75m |
| 60m | 5.3715 MHz | 27.5m (90.2 ft) | 27.7m |
| 40m | 7.150 MHz | 20.7m (67.9 ft) | 20.9m |
| 30m | 10.125 MHz | 14.6m (47.9 ft) | 14.8m |
| 20m | 14.200 MHz | 10.4m (34.1 ft) | 10.6m |
| 17m | 18.090 MHz | 8.2m (26.9 ft) | 8.35m |
| 15m | 21.050 MHz | 7.05m (23.1 ft) | 7.20m |
| 12m | 24.910 MHz | 5.95m (19.5 ft) | 6.10m |
| 10m | 28.050 MHz | 5.29m (17.4 ft) | 5.45m |
| 6m | 50.125 MHz | 2.95m (9.7 ft) | 3.05m |

**60m note:** US 60m band is channelized — 5 channels at 5.3305, 5.3465, 5.3570, 5.3715, 5.4035 MHz. A single-frequency EFHW element won't cover all channels. Cut to Ch 4 (5.3715 MHz) as the reference; other channels will need a tuner or are close enough at QRP CW bandwidths.

**6m note:** The 49:1 unun must work at 50 MHz. Mix 43 cores (FT-140-43, FT-240-43) are rated 1–30 MHz — verify your specific unun's 6m performance before building the 6m link. If 6m is not needed, omit that link and start the wire at the 10m half-wave length.

### Linked Design — No Tuner, Multiband

A single continuous wire with break points (links) at each band's half-wave length. Connect links for longer bands, disconnect for shorter — active wire is the resonant section, remainder is electrically disconnected.

**Link connector options:**

| Option | Notes |
|---|---|
| 2.5mm banana plug (K6ARK method) | Lightweight, one-handed, female parks in heat shrink sleeve at link point — recommended |
| Anderson Powerpole | Robust, weatherproof, but two-handed to mate |
| Alligator clip | Quick and dirty, can work loose |
| Small bullet connector | Compact, field-solderable |

**K6ARK Link Construction — 2.5mm Banana Plugs**
*(Adam K6ARK, K6ARK Portable Radio YouTube channel)*

Parts per link:
- 2.5mm mini banana plug (male) × 1
- 2.5mm mini banana socket (female) × 1
- Adhesive heat shrink, ~3" length
- Super glue (CA glue)

Procedure:
1. Solder male banana plug to the wire at the link position (longer/lower-band side)
2. Solder female banana socket to the wire end (shorter/higher-band side)
3. Slide a 3" piece of adhesive heat shrink over the female socket
4. Slide the female socket into the heat shrink tube — the tube becomes a sheath
5. Apply a drop of super glue to hold the female in the tube
6. Result: female socket parks in its sleeve at the link point when open; slides out and mates to male when closed

**Why 2.5mm not 4mm:** Lighter, smaller, correct scale for thin antenna wire (28–30 AWG). Standard 4mm lab banana plugs are oversized and heavy for a wire antenna link.

**One-handed operation:** Push female onto male to close link; pull female back into sleeve to open. No tools. The sleeve keeps the female connector exactly where it needs to be — never hunts for a loose connector in the field.

**Construction:**
1. Cut wire to 80m half-wave length (40.75m — slightly long) for full 9-band
2. Mark all link positions with paint pen before installing any connectors
3. At each band's half-wave length from feedpoint, install a K6ARK link — male on the outer (lower-band) side, female-in-sleeve on the inner (higher-band) side
4. Label each link in the heat shrink before gluing — band name and distance from feedpoint
5. Tune each band in isolation: connect links out to that band, trim from the far end of that section, verify with NanoVNA, then add the next outward link
6. Band change: connect/disconnect one link — 5 seconds

**Cat5 version:** Strip insulation at each tap point instead of cutting — use pairs as linked conductor. One patch cable becomes a multiband EFHW.

### 10-Band Linked Layout — K6ARK Banana Plug Method

Total wire: 40.5m (cut to 40.75m). 9 links, one per band edge except 80m (wire end). All distances are cumulative from the unun feedpoint.

| Band | Link position (from feedpoint) | Segment to next link | Label |
|---|---|---|---|
| 6m | 2.95m (9.7 ft) | 2.95m from feedpoint | "6m" |
| 10m | 5.29m (17.4 ft) | 2.34m from 6m link | "10m" |
| 12m | 5.95m (19.5 ft) | 0.66m from 10m link | "12m" |
| 15m | 7.05m (23.1 ft) | 1.10m from 12m link | "15m" |
| 17m | 8.20m (26.9 ft) | 1.15m from 15m link | "17m" |
| 20m | 10.40m (34.1 ft) | 2.20m from 17m link | "20m" |
| 30m | 14.60m (47.9 ft) | 4.20m from 20m link | "30m" |
| 40m | 20.70m (67.9 ft) | 6.10m from 30m link | "40m" |
| 60m | 27.50m (90.2 ft) | 6.80m from 40m link | "60m" |
| 80m end | 40.50m (132.8 ft) | 13.00m from 60m link | — (wire end) |

**To operate each band:** Connect all links out to that band's position; disconnect the next outward link.
- 6m: disconnect at 6m link — 2.95m active
- 10m: connect through 10m link; disconnect at 12m link — 5.29m active
- 60m: connect through 60m link; disconnect at 80m end — 27.50m active
- 80m: all 9 links connected — 40.5m active

**Short segments:** The 10m→12m segment (0.66m / 26") is short — mark and label both ends of this segment before installing connectors. Color-coded heat shrink prevents confusion.

**Tuning order:** Work from shortest band outward. Tune 6m element first, then add the 10m section and tune, then 12m, etc. Trimming a shorter inner section does not affect outer sections.

### Short Counterpoise

Even though EFHW doesn't require a ground plane, add a 1–2m wire from the unun chassis to reduce common-mode current on the coax:
- Alligator clip to unun chassis ground lug
- Lay wire on ground or clip to frame (wheelchair/walker)
- Measurably improves SWR stability when coax is moved

### Deployment Configurations

| Configuration | Description | Best use |
|---|---|---|
| Sloper | Far end high in tree, unun end lower at rig | DX — low angle radiation |
| Inverted-V | Apex high, both wire ends down at ~45° | Omnidirectional, POTA |
| Horizontal | Both ends at equal height | NVIS on 40m/80m — regional |
| Vertical | Unun at base, wire straight up (balloon or pole) | DX, low-angle |

**Sloper** is the most common POTA deployment: throw far end over a branch, hang unun at convenient rig height, lay coax to operating position.

### Tuning

Resonant by design — if wire is cut correctly, SWR should be <2:1 without a tuner.

Verify with NanoVNA (SOLT calibrated at unun output):
- SWR dip should fall within the target band
- If dip is high: wire slightly short — add a few inches
- If dip is low: wire slightly long — trim from the far end

Once tuned and marked, the linked EFHW repeats reliably — same link configuration = same resonance every time.

### Integration with Field Kit

- Wheelchair Mode 2 / Walker Mode 2: 6–7m fiberglass fishing pole holds wire vertical from RAM mount; unun at frame level, coax to rig in lap or basket
- Throw far end over tree for sloper from any position
- Entire antenna coils into a jacket pocket: wire + unun + short counterpoise

### Trapped EFHW — Multiband Without Links

A trapped EFHW replaces the linked connector system with inline LC traps. Each trap stops current at its resonant frequency, making the wire electrically end-fed half-wave on each trapped band. The physical wire is shorter than free-space because trap inductance below resonance adds electrical length.

**Trade-off vs linked design:**

| | Linked | Trapped |
|---|---|---|
| Band change | Mate/unmate one connector | Automatic — transmit on any band |
| Build time | 1–2 hours | 4–8 hours |
| Troubleshooting | Simple | Requires NanoVNA per trap |
| Wire length | Full free-space half-wave | Shorter (~60–62 ft for 40m+20m+30m) |
| Repair in field | Replace connector | Need spare trap |

**Practical trapped EFHW:** 49:1 unun → 20m element → 20m trap → ~5m wire (30m element) → 30m trap → ~remaining wire (40m element). Total ~60–62 ft (vs 67 ft free-space 40m EFHW). The trap inductance loads each section below resonance, shortening required physical length. See EFHW Micro-Trap Construction section for component values, build procedure, and section length layouts for 3-, 4-, and 5-band configurations.

---

## EFHW Micro-Trap Construction
*(T37-2 iron powder, K6ARK-style field build)*

**Iron powder AL values — NOT ferrite:**
- T37-2 (red, 0.37" OD): AL = 4.0 nH/T²
- T44-2 (red, 0.44" OD): AL = 5.2 nH/T²
- T50-2 (red, 0.50" OD): AL = 5.7 nH/T²

T37-2 is preferred for QRP traps — small, light, adequate power handling at <20W. T50-2 if more turns are needed (40m) or higher power.

**Design target:** 2–5 µH inductance per trap. Higher inductance = better Q and sharper trap notch; lower = more compact.

**Component values (iron powder AL = 4.0 nH/T² for T37-2):**

| Trap | Target freq | Core | Turns | Inductance | Cap | Notes |
|---|---|---|---|---|---|---|
| 20m | 14.175 MHz | T37-2 | 29T | ~3.314 µH | 39 pF NP0 | Confirmed by K6ARK video |
| 30m | 10.125 MHz | T37-2 | 33T | ~4.356 µH | 56 pF NP0 | 56 pF for larger inductance |
| 40m | 7.15 MHz | T50-2 | 30T | ~5.130 µH | 100 pF NP0 | T50-2 for turn count |
| 60m | 5.3715 MHz | T50-2 | 28T | ~4.469 µH | 200 pF NP0 | Starts ~5.32 MHz; spread turns to raise to 5.37 MHz |

Additional band traps use same AL values — extend table from counterpoise section as needed.
**NP0/C0G capacitors only.** 100V rating recommended for EFHW traps (tip voltage ~224V peak at 20W — at the trap, voltage is lower but leave margin).

**PCB strip substrate — OSH Park "QRP Antenna Trap" board (preferred):**

OSH Park offers a purpose-designed PCB for this trap construction: **"QRP Antenna Trap"** — search OSH Park shared projects. The board is already sized to fit through T37/T50 toroid holes, has the SMD cap footprint in the correct center position, and has the end-hole wire pass-throughs already drilled. Eliminates the Dremel cutting and drilling step. Order 3 minimum (OSH Park batches in 3s) — enough for a 20m/30m/40m trapped EFHW.

Parts per trap:
- T37-2 toroid × 1
- 24–26 AWG enameled magnet wire (wind coil)
- SMD NP0 cap (100V, correct value) × 1
- OSH Park "QRP Antenna Trap" PCB × 1 (or: cut 1 hole wide × 6 holes long from perf board — round ends, taper to fit toroid, drill end holes larger for wire pass-through)
- 5-minute two-part epoxy
- Heat shrink to cover finished trap

PCB preparation (OSH Park board — no cutting needed):
1. Verify board fits through wound toroid hole — it should by design
2. Solder SMD cap at center position per board silkscreen

Winding and assembly:
1. Wind the specified turns of 24–26 AWG enameled wire onto the toroid — close-wound
2. Thread the PCB strip through the center of the wound toroid
3. Pass the winding leads through the end holes of the PCB strip
4. Solder winding leads to cap pads — this puts cap in parallel with the winding
5. **Test resonance before potting** (see tuning procedure below)
6. When on frequency: fill toroid center and strip with 5-minute epoxy
7. Slide heat shrink over entire assembly; shrink down

**Wire-through-toroid tuning procedure:**

This method lets you measure the trap's resonant frequency without final assembly:

1. Cut a 3–5" wire with a BNC connector at one end
2. Thread the wire through the center of the toroid (just passed through, not wound)
3. Connect BNC to NanoVNA CH0 (or TinySA)
4. Sweep through target frequency
5. Look for a dip in return loss (S11) — that dip is the resonant frequency of the parallel LC

Alternatively, use S21 method: trap in series between CH0 and CH1; look for deep attenuation dip.

**Fine tuning by coil adjustment:**

| Action | Effect |
|---|---|
| Compress turns together | Lowers resonant frequency |
| Spread turns apart | Raises resonant frequency |
| Add a turn | Lowers frequency |
| Remove a turn | Raises frequency |
| Larger cap value | Lowers frequency |
| Smaller cap value | Raises frequency |

Tune to the *bottom* of the target band — trap performance extends across the band, and tuning to the lower band edge ensures the antenna covers the full band.

**Assembly sequence — multi-band trapped EFHW:**

Build order matters: always work from the unun outward, tuning each element before adding the next trap.

1. Solder and test 49:1 unun — verify SWR with NanoVNA (SOLT at unun output)
2. Connect 20m element wire to unun output — length ~10.4m (34 ft), start long
3. Tune 20m element to resonance by trimming from the far end
4. Mark 20m element length; solder 20m trap to far end of 20m element
5. Connect ~5m wire to far end of 20m trap (this becomes the 30m element addition)
6. Reconnect NanoVNA at unun; tune combined length for 30m resonance; trim far end
7. Solder 30m trap; connect remaining wire for 40m
8. Tune for 40m resonance; trim final section
9. Verify all three bands — expect minor interaction, tweak outermost section last

**Finished length:** ~60–62 ft total wire (compare to 67 ft free-space 40m EFHW). Trap loading shortens the required physical length on each band.

### Trapped EFHW — Section Lengths

Each trap is inductive below its resonant frequency — it adds electrical length to the wire, so each physical section is *shorter* than the free-space half-wave segment. The inner element (first section) resonates at its band without any trap influence — it is the same length as the linked EFHW. Each outer section is shorter than free-space by approximately 15–25%, and the shortening compounds outward.

**Practical band limits for trapped EFHW:**

The 10m→12m free-space segment is only 0.66m (26") — too short to wire a trap plus reasonable antenna wire segments around it. This makes any configuration that includes both 10m and 12m impractical. 6m adds very short wire (2.95m total) and requires unun coverage to 50 MHz. Practical trapped EFHW designs use 2–5 bands, 15m–40m or 15m–80m.

**Recommended configurations:**

| Config | Bands | Traps | Total wire | Notes |
|---|---|---|---|---|
| 20m/30m/40m | 3 | 2 | ~60–62 ft (18.3–18.9m) | Proven — K6ARK video design |
| 20m/30m/40m/60m | 4 | 3 | ~78–80 ft (23.8–24.4m) | Adds 60m; good 4-band portable |
| 15m/20m/30m/40m | 4 | 3 | ~55–57 ft (16.8–17.4m) | Adds 15m easily |
| 15m/20m/30m/40m/60m | 5 | 4 | ~73–77 ft (22.3–23.5m) | 5-band practical maximum |
| 15m/17m/20m/30m/40m | 5 | 4 | ~53–55 ft (16.2–16.8m) | 17m segment tight (0.9–1.0m) |

**Why not more bands:** 10m→12m segment (0.66m) and 12m→15m (1.10m) are physically too short for trap hardware. 80m adds a 15–18m section (4th–5th trap), making the total wire 90+ ft — impractical for portable.

---

**3-band layout: 20m/30m/40m** *(K6ARK video design)*

| Section | Span | Free-space | Start long | Est. actual | Tune to |
|---|---|---|---|---|---|
| 1 | Feedpoint → 20m trap | 10.4m (34.1 ft) | 10.6m (34.8 ft) | ~10.4m | 20m resonance |
| 2 | 20m trap → 30m trap | 4.2m (13.8 ft) | 4.5m (14.8 ft) | ~3.2–3.6m | 30m resonance |
| 3 | 30m trap → far end | 6.1m (20.0 ft) | 6.2m (20.3 ft) | ~4.5–5.0m | 40m resonance |

Total actual: ~18.3–19.0m ≈ 60–62 ft

**4-band layout: 20m/30m/40m/60m**

| Section | Span | Free-space | Start long | Est. actual | Tune to |
|---|---|---|---|---|---|
| 1 | Feedpoint → 20m trap | 10.4m (34.1 ft) | 10.6m (34.8 ft) | ~10.4m | 20m resonance |
| 2 | 20m trap → 30m trap | 4.2m (13.8 ft) | 4.5m (14.8 ft) | ~3.2–3.6m | 30m resonance |
| 3 | 30m trap → 40m trap | 6.1m (20.0 ft) | 6.2m (20.3 ft) | ~4.5–5.0m | 40m resonance |
| 4 | 40m trap → far end | 6.8m (22.3 ft) | 6.9m (22.6 ft) | ~5.0–5.8m | 60m resonance |

Total actual: ~23–25m ≈ 75–82 ft (vs 90.2 ft free-space 60m half-wave)

**4-band layout: 15m/20m/30m/40m**

| Section | Span | Free-space | Start long | Est. actual | Tune to |
|---|---|---|---|---|---|
| 1 | Feedpoint → 15m trap | 7.05m (23.1 ft) | 7.2m (23.6 ft) | ~7.05m | 15m resonance |
| 2 | 15m trap → 20m trap | 3.35m (11.0 ft) | 3.5m (11.5 ft) | ~2.6–3.0m | 20m resonance |
| 3 | 20m trap → 30m trap | 4.2m (13.8 ft) | 4.5m (14.8 ft) | ~3.0–3.4m | 30m resonance |
| 4 | 30m trap → far end | 6.1m (20.0 ft) | 6.2m (20.3 ft) | ~4.3–4.8m | 40m resonance |

Total actual: ~17–18m ≈ 56–59 ft

**5-band layout: 15m/17m/20m/30m/40m**

| Section | Span | Free-space | Start long | Est. actual | Tune to |
|---|---|---|---|---|---|
| 1 | Feedpoint → 15m trap | 7.05m (23.1 ft) | 7.2m (23.6 ft) | ~7.05m | 15m resonance |
| 2 | 15m trap → 17m trap | 1.15m (3.8 ft) | 1.2m (3.9 ft) | ~0.9–1.0m | 17m resonance |
| 3 | 17m trap → 20m trap | 2.20m (7.2 ft) | 2.3m (7.5 ft) | ~1.7–1.9m | 20m resonance |
| 4 | 20m trap → 30m trap | 4.2m (13.8 ft) | 4.5m (14.8 ft) | ~3.0–3.4m | 30m resonance |
| 5 | 30m trap → far end | 6.1m (20.0 ft) | 6.2m (20.3 ft) | ~4.2–4.7m | 40m resonance |

Total actual: ~17–18m ≈ 56–59 ft. Section 2 (17m, ~0.9m) is short — leave adequate wire-to-trap lead length, label both sides.

---

**Free-space reference — all 9 bands (linked layout segments):**

| Band | Half-wave | Segment from prev | Trapped viability |
|---|---|---|---|
| 6m | 2.95m | 2.95m from feedpoint | Unun freq limit; omit |
| 10m | 5.29m | 2.34m from 6m | Segment OK; 10m→12m is not |
| 12m | 5.95m | **0.66m from 10m** | Too short for trap — skip |
| 15m | 7.05m | 1.10m from 12m | Feasible inner element |
| 17m | 8.20m | 1.15m from 15m | Short but workable |
| 20m | 10.40m | 2.20m from 17m | Standard inner element |
| 30m | 14.60m | 4.20m from 20m | Good |
| 40m | 20.70m | 6.10m from 30m | Good |
| 60m | 27.50m | 6.80m from 40m | Good; 60m is channelized — cut to Ch 4 (5.3715 MHz) |
| 80m | 40.50m | 13.00m from 60m | Impractical for portable |

**General rule:** Start every section at free-space length; trim outward band-by-band with NanoVNA. Actual section lengths cannot be pre-calculated precisely without knowing the antenna wire's characteristic impedance — empirical trimming is the method.

**30 AWG PTFE for trap windings:** Works for T37-2 — fits 33 turns. Strip mechanically, do not burn (toxic above 260°C). For counterpoise traps at low power levels, adequate. For EFHW traps, 24–26 AWG enameled is better (higher current in the coil near the feedpoint).

---

## Common Mode Choke

Prevents RF from flowing back down the outside of the coax shield. Without one, the coax acts as part of the antenna — SWR changes when you touch the feedline, RF gets into the rig, and pattern distorts. Required any time coax feeds a balanced antenna.

### When You Need One

| Situation | Why |
|---|---|
| Coax-fed dipole | Dipole is balanced; coax is unbalanced — without choke, shield radiates |
| EFHW + 49:1 unun | Common-mode current flows back on coax shield from unun |
| Vertical with coax feedline | Coax shield acts as additional counterpoise — changes pattern and SWR |
| SWR shifts when touching coax | Coax is radiating — choke needed at feedpoint |
| RF feedback in rig audio | Common mode current reaching the shack |

### Ferrite Material — Mix 31

For HF field use (1–30 MHz): **Mix 31** is the standard. High impedance across the full HF spectrum, excellent common-mode suppression.

- FT-240-31 toroid — full-size, highest performance
- FT-140-31 — smaller, lighter, adequate for QRP
- Snap-on ferrite clamp (mix 31) — most portable field option

Mix 43 is a common substitute — slightly lower impedance at HF but usable. Mix 73 is for AM broadcast frequencies — wrong material for HF.

### Target: >1000Ω Common Mode Impedance

Below 1000Ω the choke is only partially effective. More turns or more cores = more impedance.

| Core | Turns | Common mode Z at 7 MHz | Weight |
|---|---|---|---|
| FT-240-31, 7–8 turns coax | 7 | >2000Ω | ~3 oz |
| FT-140-31, 7–8 turns coax | 7 | ~1000Ω | ~1.5 oz |
| Snap-on clamp ×1 (mix 31) | 1 pass | ~200Ω | 0.5 oz |
| Snap-on clamp ×4 (mix 31) | 1 pass each | ~800Ω | 2 oz |

For serious suppression: wound toroid. For quick field use: 4 snap-on clamps stacked at the feedpoint.

### Field Construction — Wound Choke

1. Pass RG-8X or RG-174 through FT-240-31 toroid, 7–8 turns
2. Secure with a zip tie — coax shouldn't slide
3. Weather-seal connections with self-amalgamating tape
4. Mount at feedpoint — choke goes between coax and antenna feedpoint

**Coax size note:** RG-174 (thin, flexible) fits more turns through a smaller toroid. RG-8X is harder to wind tightly but lower loss. For QRP field use: RG-174 wound on FT-140-31 is a good balance of size and performance.

### Field Construction — Snap-On Clamps

Fastest field option. Stack 3–4 mix 31 snap-on clamps on the coax at the feedpoint.

- Clip on, done — no winding, no tools
- Stack them: each clamp adds impedance
- Available on Amazon as "ferrite clamp" or "ferrite core" — verify mix 31 for HF
- Small enough to keep in connector bag

### Placement

1. **At the feedpoint** — most effective; stops common-mode before it gets on the coax
2. **At the rig** — second-best; stops coax from re-radiating in the operating position
3. **Both** — ideal for problem installations

For field use: feedpoint placement is standard. Add a second choke at the rig if RF feedback persists.

### Symptoms Indicating a Choke Is Needed

- SWR reading changes when you move or touch the coax
- SWR drops when coax is on ground vs. in the air
- RF in rig audio or keyer during TX
- Rig body warms during TX at QRP levels
- NanoVNA SWR changes when you move the measurement cable

### In the Field Kit

Two options — both are small enough for the connector bag or a separate pouch:

**Option A — Snap-on clamps:** 4× mix 31 snap-on clamps. Stack at feedpoint. ~2 oz total. Instant deployment, no tools.

**Option B — Wound choke:** FT-140-31 with 7 turns RG-174, coax connectors on each end. ~1.5 oz. More effective, slightly more setup. Connects inline between antenna feedpoint and rig coax.

---

## Z-Match Tuner

Passive manual antenna tuner — two capacitors, no relays, no power. Preferred over ATU-100 for field and patio operating.

**Why Z-match for field:**
- Instant — turn knobs, done; no tune sequence, no button press
- Silent — no relay clicks during tuning
- Resonant circuit attenuates harmonics beyond the rig's LPF — bonus suppression
- Handles wide impedance range including 15–35Ω whip feedpoints directly
- No power consumption

**Coverage:** 3–28 MHz (80m through 10m) — all HF operating bands.

**Two controls:**
- Main tuning capacitor — primary resonance adjustment
- Bandset/loading capacitor — coarse range selection, re-peak main after each change

**Tuning procedure:**
1. Set both capacitors to midrange
2. Key up at low power
3. Adjust main cap — minimum SWR or peak received noise
4. Adjust bandset cap, re-peak main cap
5. Iterate 2–3 passes — match found in ~30 seconds once practiced

**Balanced output tap:** Check unit for balanced output — enables direct ladder line or Cat5 twisted pair feedline without a balun.

**Pre-characterization:** Measure whip feedpoint impedance with NanoVNA at home, pre-set Z-match to that impedance before field deployment — speeds field tuning from 30 seconds to a few seconds.

**vs ATU-100:**

| | Z-match | ATU-100 |
|---|---|---|
| Tuning | Manual, instant | Auto, tune sequence |
| Power | None | ~100 mA |
| Speed | Immediate | 2–5 seconds |
| Memory | None | Stores tuned positions |
| Best for | Field/patio manual operating | Shack backup, scanning |

---

## ATU-100 Automatic Antenna Tuner

Automatic L-network tuner. Useful as field backup and shack installation — less ideal than Z-match for deliberate QRP manual operating.

**Specs:** Handles up to ~10:1 SWR, stores tuning memories for fast re-tune, ~100 mA operating current from 12V.

**Placement:** Between rig and feedline — always on the rig side of the choke balun.

**Tuning sequence:**
1. Key up at low power
2. Press tune button (or auto-senses on TX)
3. Relays click through L-network combinations
4. SWR match found and stored — 2–5 seconds
5. Subsequent transmissions on same frequency: instant recall, no sequence

**Memory:** Stores frequency/tuning pairs — change band, re-tune once, stores the new position. Near-instant re-tune on known frequencies.

**Field use:** Backup when Z-match is unavailable. Also useful for rapid band changes during scanning — auto-tune on each new frequency.

**Limits:** Does not fix receive noise from antenna mismatch — tune the antenna properly first, use ATU-100 for residual mismatch only. Loss negligible at QRP (20W) with a reasonably matched antenna.

---

## 49:1 Unun

Impedance transformer for end-fed antennas. Steps the very high feedpoint impedance of an EFHW (~2500Ω) down to ~50Ω for coax feed.

### Why 49:1

Half-wave antenna feedpoint impedance: ~2450–3000Ω at resonance. Coax characteristic impedance: 50Ω. Ratio: ~49:1 to ~60:1. The 49:1 ratio (7:1 turns ratio) is the standard because it's achievable on a single toroid with a clean winding geometry and hits the center of the impedance range.

**64:1 alternative:** Used when feedpoint impedance runs higher (some EFHW configurations, especially on non-resonant lengths). If SWR won't go below 2:1 with a 49:1 and the antenna is confirmed resonant, try 64:1.

### Construction — DIY

**Core:** FT-140-43 (QRP ≤10W) or FT-240-43 (up to 100W)
- Mix 43 is standard for EFHW ununs — good performance 3–30 MHz
- Mix 31 works but is optimized for common-mode suppression, not impedance transformation

**Winding:**
- Primary: 2 turns (coax side)
- Secondary: 14 turns (antenna wire side)
- 2:14 = 1:7 turns ratio → 1:49 impedance ratio

**Wire:** 26–28 AWG enameled magnet wire for QRP. Bifilar wind if possible — both windings together reduces leakage inductance.

**Housing:** Weatherproof enclosure, SO-239 for coax, binding post or banana jack for wire element. Small ground lug for 1–2m counterpoise wire.

### Commercial Options

| Source | Notes |
|---|---|
| **K6ARK EFHW Matching Unit Kit** | Compact, field-optimized, purpose-built for portable EFHW — **kit in use** |
| QRPGuys | ~$15 kit, FT-140-43, well-documented, popular |
| LNR Precision | Pre-built, field-proven |
| PackTenna | POTA-optimized, compact |
| Chelegance | MC-750 includes unun + wire |

### In the Field

- SO-239 output → use SO-239→SMA M adapter to feed coax to rig or Z-match
- Short counterpoise wire (1–2m) from chassis ground lug — reduces common-mode on coax shield
- Verify SWR with NanoVNA at unun output (SOLT cal at SO-239 port) before deploying

---

## Getting a Line Into a Tree

Standard technique for hanging wire antennas in trees:

### Launcher Options

| Method | Notes |
|---|---|
| Pocket Rocket (pneumatic) | Legal CA; looks professional; good for 30–50 ft heights |
| Slingshot | Legal CA; cheap; slightly less control than pneumatic |
| Fishing rod underhand cast | No equipment needed; 20–30 ft max; fine for inverted-V apex |
| Tennis ball + drilled hole | Field-expedient weight; free |

**Leave No Trace:** no bark damage, no permanent attachment, retrieve everything at pack-out.

### Arborist Throw Bag Technique

Purpose-built for getting lines into trees — used by tree climbers, adapted for ham use.

**Equipment:**
- Sherrill or Weaver throw bag: 8–12 oz beanbag weight (heavier = more control, less wind drift)
- Throw line: 1.5–2mm braided polyester or dyneema — smooth, low-tangle, high-strength
- Throw line cube or winder to prevent tangles during throw

**Technique:**
1. Load bag into launcher or hold in hand for underhand throw
2. Aim for a crotch or branch at target height — not the highest point, a usable one
3. Throw line pays out from the cube behind the bag
4. Bag falls on the far side of branch — pull gently to confirm it's seated
5. Tie antenna wire or pilot line to throw line, haul up
6. Retrieve throw bag: pull from one side, bag slides back over branch

**One-handed adaptation:**
- Use pocket rocket or slingshot with throw bag — launcher held between knees or clamped to frame while line is handled
- Throw line winder clips to frame or bag — free hand manages the line
- Pre-tie the antenna wire to the throw line before throwing — one operation, not two

**vs monofilament:**
- Monofilament: lighter, tangles less on short throws, retrieves clean, cheap
- Throw line: heavier bag = clears higher branches, less wind effect, more repeatable
- Monofilament first, upgrade to throw bag if heights >40 ft or wind is a factor

### Balloon + Kite Messenger Technique

Adapted from kite messenger technique — a slider that rides up the string and releases a payload at height.

**How kite messengers work:**
- Clip onto angled string, wind catches a flap/wing, propels messenger UP the string
- Trigger at top releases payload — drop line, weight, whatever

**Balloon application:**
- Light steady breeze angles balloon string over target branch
- Messenger rides up angled string, releases drop line at top
- Drop line falls over branch — haul back, pull up antenna
- Requires some wind to angle the string — calm air = vertical string = messenger won't travel

**Simpler balloon approach (no messenger):**
- Float balloon on fishing reel line
- Maneuver balloon over target branch with horizontal guide line or pole
- Balloon drags antenna wire over branch
- No messenger, no release mechanism needed

**Messenger advantage:**
- Places line on a specific branch at height with precision
- Reusable once release mechanism is worked out
- Drop line falls exactly where the messenger stops on the string

**One-handed release mechanism:**
- Gravity-trigger or string-tug release — nothing requiring two hands to arm or set
- Keep it dead simple

## Cat 5 / Cat 6 Cable as Antenna Wire

Surprisingly capable for field and emergency use.

**What you get:**
- 8 conductors in 4 twisted pairs
- 24 AWG solid copper (Cat5e/6)
- Parallel conductors for slightly more current capacity

**Uses:**
- **Random wire** — strip jacket, use one or more conductors, rest as counterpoise
- **Dipole** — one pair each side
- **NVIS** — flat, low deployment, good for 40m/80m regional contacts
- **Emergency wire** — common, free, cuts to length

**Twisted pair as feedline:**
- Two parallel conductors with consistent spacing = transmission line
- Characteristic impedance: ~100–120Ω (varies by cable spec)
- Velocity factor: ~0.64–0.69 (solid polyethylene dielectric)
- Works as open-wire feedline substitute at HF
- Untwist the pair at both ends — minimizes conductor coupling at feedpoint and load
- Z-match tuner handles 100–120Ω directly — good match for field kit
- Loss negligible vs proper ladder line at 40m/20m QRP power levels
- Best use: fan dipole fed with twisted pair from RJ-45 junction box to rig — entire system is one Cat5 cable

**Compared to proper ladder line:**

| Type | Impedance | VF | Loss |
|---|---|---|---|
| 450Ω window line | 450Ω | 0.91 | Very low |
| 300Ω twin-lead | 300Ω | 0.82 | Low |
| Cat5 twisted pair | ~100–120Ω | 0.65 | Low at HF QRP |

**Limits:**
- 24 AWG marginal above 25W — fine for QRP, gets warm at 100W
- Not UV-resistant long term
- Solid core breaks with repeated flexing — temporary use only, not permanent installation

**Multiband EFHW from 4 pairs:**
- Each pair cut to a different EFHW length = 4-band array in concept
- Each element needs its own 49:1 transformer — shared feedpoint causes impedance interaction
- Simpler: use pairs as a fan dipole instead — one feedpoint, each pair is one band's half-element, more predictable behavior

**Field kit suggestion:**
- 50-foot patch cable in the bag
- Gives dipole on 40m, random wire on 20m, or NVIS on 80m depending on deployment
- Costs nothing if spare cable is on hand

## RJ-45 as Field Antenna Connector

**What you get:**
- 8 contacts map directly to 4 twisted pairs — one contact pair per antenna element
- Keyed connector — no wrong orientation
- Locking tab — won't pull out accidentally
- Cheap, common, field-replaceable everywhere
- Weatherproof panel-mount versions available

**Wiring scheme:**
- Both conductors of each pair paralleled at the jack = one feedpoint per element
- At antenna end: untwist and spread pairs to respective elements
- Junction box with panel-mount RJ-45 jack at feedpoint — patch cable plugs straight in

**Limitations:**
- Rated for low-voltage signal use, not RF — minor loss at higher frequencies
- Contact geometry not optimized for RF
- 24 AWG contacts — fine at QRP, marginal at 100W

**Best use case:**
Complete field antenna system rolls up into a jacket pocket. Throw one end over a branch, plug patch cable into junction box, deploy. Entire antenna is a standard patch cable.

## Fishing Reel Band Selector Antenna (Ham Jazz RAS Episode 4)

Ham Jazz KG7LLR, Oak Bay BC. The fishing reel replaces fixed tap points — unreel to resonant length, reel IS the band selector. Demonstrated on bicycle, IC-7300, Vito Man Jump 1800 power station.

**Mount — bottle cage boss screws:**
- Every bicycle has two bottle cage boss screws — threaded steel inserts bonded to the whole frame
- Flat blade clamp fastened directly onto boss screw — frame becomes counterpoise/ground plane
- "3824 coupler" (portable whip adapter, PL-259) mounted to clamp — feedpoint connector
- Jewelry wire (enameled, tip scraped bare for conductivity) on bait-caster fishing reel
- Reel wire runs UP to a tree (~7.5 ft high) — nearly vertical element

**Wheelchair/walker equivalent:**
- Any threaded frame insert or tube clamp in contact with metal frame
- Same flat blade clamp, same coupler, same reel

**Operation:**
- Unreel to resonant length for target band — no tuner required
- Band change: reel in or out — one hand, seconds
- Frame = counterpoise through metal clamp contact

**SWR achieved (from transcript):**

| Band | Freq | SWR | Wire length |
|---|---|---|---|
| 10m | ~28 MHz | 1.2:1 | Short |
| 12m | 24.3 MHz | ~1:1 | Reel out more |
| 15m | 21.5 MHz | ~1:1 | Reel out more |
| 20m | 14.0 MHz | 1.19:1 | Reel out more |
| 40m | 7.15 MHz | 1.2:1 | ~65 ft full extension |
| 80m | 3.729 MHz | 1.52:1 | 65 ft + 18 ft cap hat |

**40m:** Just unreel to ~65 ft — natural resonance at 7.15 MHz, no extra loading needed.

**80m — capacity hat:**
- 18 ft of hookup wire connected at feedpoint, walked out horizontally
- Reel wire goes over tree at ~7.5 ft — inverted-L geometry
- Capacity hat increases effective electrical length to reach 80m resonance
- High Q, narrow bandwidth — tune for specific net frequency (3.729 MHz BC net)
- On wheelchair: capacity hat clips to push handles; on walker: clips to crossbar

**One-handed operation:**
- Flat blade clamp onto boss screw — one fastener
- Fishing reel cranks one-handed — band change without tools
- Alligator clip at coupler — one-handed connect/disconnect

## Balloon Antenna

Fair-weather supplemental antenna — calm days only, not primary field antenna.

**What works:**
- 28–30 AWG wire or monofilament with wire — light enough for balloons
- Mylar balloons hold helium days vs hours for latex party balloons
- Multiple balloons on one line for redundancy and extra lift
- Calm air — stays up hours to a day depending on balloon size and wire weight

**What kills it:**
- Wind — even light breeze causes drag on wire, pulls balloon down or sideways
- Heat — sun warms gas, balloon rises then falls as it cools at night
- Rain — water on balloon adds weight rapidly
- Latex leaks — party balloons lose significant lift in 4–8 hours

**Antenna configurations:**
- **Vertical wire** — balloon suspends wire vertically, near-ideal radiation pattern
- **Inverted-L** — balloon holds vertical section, wire runs horizontally at top
- **NVIS (80m/40m)** — low horizontal wire, balloon just keeps it off the ground

**For wheelchair/walker:**
- Balloon on fishing reel — adjustable height, reel in if wind picks up
- Calm park days only
- High novelty factor — draws a crowd, good for public outreach

**Bottom line:** Calm day in a park — works great. Any wind — impractical. Fair-weather only.

## Counterpoise for 5.6m Whip — Wheelchair and Walker

Ideal counterpoise is λ/4 per band. Frame provides 1–2m effective conductor — covers most of 10m/12m requirement; supplemental wire needed for lower bands.

| Band | λ/4 Counterpoise |
|---|---|
| 40m | 10.5m (34.4 ft) |
| 20m | 5.03m (16.5 ft) |
| 17m | 3.9m (12.8 ft) |
| 15m | 3.35m (11 ft) |
| 12m | 2.83m (9.3 ft) |
| 10m | 2.51m (8.2 ft) |

**Practical solution — linked counterpoise:**
- One wire with alligator clip break points at each band's λ/4 length
- Matches fishing reel band change — change band, move counterpoise clip simultaneously
- Trails along ground from frame base
- Pre-clipped, coiled in pouch — deploy and clip without tools

**40m note:** 10.5m trailing wire is manageable in a park, awkward in tight spaces. For fixed position, 2–3 shorter radials staked around the position is cleaner.

### Linked Counterpoise Construction

1. Cut one wire to 40m λ/4 length (10.5m)
2. At each shorter band's λ/4 distance from the feedpoint end, attach an alligator clip — this is the break point for that band
3. Label each clip with band (paint pen or heat-shrink)
4. Clip the appropriate label to the feedpoint ground lug — wire from clip to end is active, remainder coils behind
5. Band change: unclip, move to next band's clip, reclip — 5 seconds

**Color code by band:** 40m = no clip (full wire), 20m = red clip, 17m = orange, 15m = yellow, 12m = green, 10m = blue. Consistent colors across both counterpoise and fishing reel labels — one-handed band change without reading labels.

### Staked Radial System — Fixed Position

For fixed operating positions (park bench, table, POTA site):

**Materials:** 18 AWG wire cut to λ/4 per band, alligator clip at one end, landscape staple or wire flag at the other.

**Deployment one-handed:**
1. Clip alligator clip to feedpoint ground lug from chair height
2. Lay wire along ground in desired direction — gravity does the work
3. Push landscape staple into ground at end with foot — no bending required
4. Repeat for 2–4 radials at equal angles

**Storage:** Pre-coiled with clip attached, rubber band to hold coil, labeled by band. Drop into pouch, pull out and deploy without sorting.

### Ground Surface Performance

| Surface | Radial performance | Notes |
|---|---|---|
| Grass/soil | Best | Radials couple to ground, lower feedpoint R |
| Gravel | Good | Close to soil coupling |
| Concrete/pavement | Poor | Add longer counterpoise wire instead |
| Asphalt | Poor | Same as concrete |

On poor ground: longer counterpoise (elevated if possible) works better than ground radials. Frame of wheelchair or walker elevated above pavement is inherently better than a wire laid on concrete.

## Wheelchair Antenna

Metal frame is a conductor and partial ground plane — load it as the radiating element.

**Mode 1: Fixed position — fishing pole on RAM mount**
- RAM mount (B-size, tube clamp) on wheelchair tubing — locks one-handed
- 6–7m fiberglass fishing pole vertical from mount
- Linked EFHW from tip, 49:1 unun at frame level, coax to rig in lap
- 2–4 radials staked into ground, reachable from chair height
- ~85% efficiency, 5–10 min setup

**Mode 2: Rolling — loaded whip on RAM mount**
- 1–1.5m loaded whip in RAM mount
- Counterpoise clipped to frame, trailing or coiled on footrest
- ATU-100 for matching
- 20–35% efficiency, no setup

**Mode 3: Frame as antenna (Ham Jazz RAS bicycle technique)**
- Wire element clipped to frame upright, tap points at each band's resonant length
- Banana clips at each tap — no tuner required
- Frame acts as partial ground plane + radiator
- Counterpoise wire trailing from frame base
- Consistent frame geometry means tap points stay calibrated — mark once

**Ground system:**
- Grass/soil: 2–4 radials make significant difference
- Concrete/pavement: poor ground, add longer counterpoise
- Pre-clipped radials (alligator clips + push-peg stakes) in pouch — deploy without tools

## Walker Antenna

Overlooked platform — metal frame, naturally vertical, moves with operator, non-threatening.

**Platform advantages:**
- Already in hand for stability — no additional support needed
- Naturally vertical orientation — correct for vertical antenna element
- Tennis ball feet are non-conductive — natural RF isolation at base
- Higher feedpoint than wheelchair — element clears operator's body
- Front basket or pouch carries rig and battery
- Fold-flat walkers collapse for transport, deploy in seconds

**Mode 1: Tapped vertical on frame upright (Ham Jazz RAS bicycle technique)**
- Wire element clipped to one vertical upright
- Tap points at each band's resonant length — 10m through 80m
- Banana clips at each tap — no tuner required
- Counterpoise trailing from frame base
- Band change: move banana clip — 5 seconds
- 30–60% efficiency, 2 min setup

**Mode 2: Fishing pole on walker frame**
- RAM mount or hose clamp mounts 6–7m pole to upright
- Linked EFHW from tip, 49:1 unun at frame level, coax to rig in basket
- ~85% efficiency, 5 min setup

**Operating position:**
- Stand behind walker, key on leg clamp, rig in basket
- Walker provides stability — frees operating hand for key
- Fully self-contained, self-supported HF station operable anywhere operator can stand

| Mode | Antenna | Efficiency | Setup |
|---|---|---|---|
| Walker tapped frame | Frame + wire taps | 30–60% | 2 min |
| Walker fishing pole | Linked EFHW | ~85% | 5 min |
| Wheelchair fixed | Linked EFHW + pole | ~85% | 5–10 min |
| Wheelchair rolling | Loaded whip 1.5m | 20–35% | None |
| Wheelchair frame | Frame + wire taps | 30–60% | 2 min |

## Ham Jazz — Rubbish Antenna Series (RAS)

YouTube channel focused on functional antennas built from junk bin parts.
Series title: "Rubbish Antennas" — field-expedient, low-cost, no-tuner designs.
Worth monitoring for additional field antenna techniques.

## Tapped Dipole — 5 Bands, No Tuner (Ham Jazz RAS #17)

Single continuous wire per leg, resonant taps at each band. Band change = move the connector. No tuner required.

**Construction:**
- Cut two wires to 20m half-element length (longest band)
- Strip ~1" of insulation at the 17m, 15m, 12m, and 10m resonant points on each leg
- Dual banana plug to BNC adapter at feedpoint — clip banana plugs to whichever tap you want

**How it works:**
- Each tap is the resonant half-element length for that band
- Wire beyond the tap is electrically invisible — not connected
- Connect at 20m tap: full wire in use, resonant on 20m
- Connect at 17m tap: shorter section active, resonant on 17m
- 5-second band change

**Cost: 43¢** — wire only; banana-to-BNC adapter is reusable

**Cat5 version:**
- Strip jacket at each tap point instead of individual wire insulation
- Use pairs as tapped conductor
- RJ-45 junction box at center feedpoint, banana clips at each tap
- Entire antenna remains one Cat5 cable

**Compared to DX Commander:**

| | Tapped Dipole | DX Commander |
|---|---|---|
| Bands | 5 (one at a time) | 5 (simultaneous) |
| Band change | 5 seconds, move clip | None needed |
| Cost | 43¢ | Kit price |
| Complexity | Minimal | Moderate |
| Best for | POTA, field portable | Fixed station, contesting |

### Deployment

**Inverted-V (most common POTA):**
- Apex over tree branch or fishing pole tip at 25–35 ft
- Both legs drop at ~45° angle to ground anchors
- Omnidirectional — good for POTA contacts in any direction
- Feedpoint hangs at apex — coax drops straight down to rig

**Flat top (horizontal dipole):**
- Both ends in trees at equal height — 30 ft minimum for HF
- Feedpoint at center, coax drops vertically
- Broadside direction has most gain — orient toward target region
- Higher is better; nearby trees detune less than ground proximity

**One end in tree (sloper):**
- One end high, feedpoint end lower at operating position
- Slope angle ~30–45° is good
- Asymmetric — some directional gain toward low end
- Easy to deploy alone: throw one end, tie feedpoint to a stake or frame

**Height matters:**
- <15 ft: high-angle NVIS radiation — regional contacts, 40m/80m
- 25–35 ft: mix of NVIS and low-angle — general purpose
- >50 ft: low-angle DX radiation — longer distance contacts

**Near-object detuning:** Trees, buildings, and vehicles within λ/4 detune the antenna. If SWR is unexpectedly high after deployment, move the nearest leg further from the obstruction. Wet trees detune more than dry ones.

## Field Wire Selection

### 30 AWG PTFE 600V — Light Field Antennas (<20W)

30 AWG PTFE insulated wire is the right choice for lightweight field antennas at QRP to 20W. Light, strong enough, and PTFE insulation outlasts PVC in outdoor conditions.

**Current handling:**
- 20W into 50Ω: 0.63A RMS — 30 AWG handles it (rated ~0.86A continuous)
- QRP 5W: 0.32A — no concern

**Voltage rating:**
- EFHW tip (far end from transformer) at 20W: √(20 × 2500) ≈ 224V peak
- Dipole tip at 20W: same ~224V at ends
- 600V rating: ample margin at all feedpoint impedances under 20W

**Efficiency on resonant antennas:**
- 30 AWG: ~0.34Ω/ft
- 40m EFHW (~67 ft per leg): ~23Ω wire resistance vs 73Ω radiation resistance → ~76% efficiency — acceptable
- 20m EFHW (~34 ft): ~11.5Ω wire vs 73Ω → ~86% efficiency — good
- Full-size dipoles: similar, acceptable losses at QRP

**PTFE advantages over PVC:**
- Dielectric loss at HF: negligible — PTFE has extremely low loss tangent
- UV and weather resistance: does not crack or harden like PVC in sun or cold
- Temperature range: handles heat and freeze cycles without insulation failure
- Stays flexible in cold — PVC stiffens below ~10°C; PTFE does not

**Where 30 AWG works well:**
- Linked EFHW wire — full-size resonant, deploy and retrieve without weight penalty
- Tapped dipole elements
- Counterpoise runs (all bands)
- Any resonant full-size HF wire antenna at ≤20W

**Where 30 AWG is marginal or wrong:**
- Loaded short antennas with low radiation resistance — 80m loaded whip: Rrad ≈ 2–5Ω, 30 AWG wire resistance (~6Ω for 5.6m) dominates and kills efficiency; use 22–24 AWG for the whip itself
- Permanent installations — 30 AWG fatigues at repeated sharp bends; use heavier wire for fixed runs

**Handling note — PTFE is slippery:**
- Standard overhand knots and square knots slip on PTFE insulation
- Use bight loops (figure-8, bowline) for mechanical attachment points
- Or: tape wrap + half-hitch, or crimp sleeve at feedpoint terminals
- Banana plug junctions grip well — PTFE wire works fine with banana clip taps

**Practical use:**
- Wind EFHW wire on a small fishing reel or card winder — 30 AWG coils tightly, adds negligible weight
- Carry a pre-cut linked EFHW set with banana plug links already attached — deploy by uncoiling
- Color-code counterpoise cuts with tape or paint marks

### Wire Gauge Comparison — Field HF Antennas

| Wire | Resistance | Weight | Best for |
|---|---|---|---|
| 30 AWG PTFE | 0.34 Ω/ft | Very light | EFHW, dipoles, counterpoise — ≤20W field use |
| 28 AWG | 0.21 Ω/ft | Light | Better efficiency on longer runs |
| 26 AWG | 0.13 Ω/ft | Moderate | Loaded antennas where Rrad is low |
| 24 AWG (Cat5) | 0.084 Ω/ft | Moderate | Fan dipole, solid core — field temporary only |
| 22 AWG | 0.053 Ω/ft | Heavier | Whip element, loading coil winding, >20W |
| 18 AWG | 0.021 Ω/ft | Heavy | DX Commander elements, permanent installation |

## Cat5/6 + RJ-45 Complete Antenna System

A multiband wire antenna that fits in a jacket pocket.

### Multiband Options

**Single-band dipole (cleanest):**
- Parallel all 4 pairs on each side = 8 conductors per leg, more current capacity
- Cut to dipole length for target band
- One feedpoint, predictable behavior

**Fan dipole (multiband, recommended):**
- One pair per leg per band — up to 4 bands from one cable
- All pairs feed from single RJ-45 jack
- Each pair cut to a different band's half-element length
- More predictable than multiband EFHW — no separate transformers needed

**Multiband EFHW (possible but complex):**
- Each pair cut to a different EFHW length
- Each element needs its own 49:1 transformer — shared feedpoint causes impedance interaction
- Fan dipole is simpler for the same result

### Junction Box
- Panel-mount RJ-45 jack at feedpoint
- Weatherproof enclosure
- Coax pigtail from box to rig
- Patch cable plugs straight in — keyed, locking, one-handed connection

### One-Handed Deployment
1. Throw weighted end over tree branch (1/2 oz weight on monofilament pilot line)
2. Haul up patch cable using pilot line
3. Plug RJ-45 into junction box at feedpoint
4. Connect coax from junction box to rig
- Complete setup under 5 minutes, one hand

### Limits
- RJ-45 contacts not optimized for RF — minor loss acceptable at QRP
- 24 AWG marginal above 25W — QRP only
- Solid core breaks with repeated sharp bending — temporary field use, not permanent

## RF Connector Adapters — Field Storage

### Container

Makeup bag with clear zipper pouches. Purpose-built for small parts, popular with electronics techs and hams. Clear windows let you identify contents without opening — important one-handed.

**What to look for:**
- Multiple individual pouches, not one large pocket
- Zipper pulls large enough for one-handed operation
- Semi-rigid base so bag doesn't fold and crush SMA center pins
- Mesh or clear on both sides of each pouch

### Organization — Sort by Connector Family

| Pouch | Contents |
|---|---|
| SMA | SMA-SMA M-M, F-F, right-angle, barrel |
| BNC | BNC-BNC, BNC-SMA, BNC barrel |
| N | N-SMA, N-BNC, N barrel |
| Misc | PL-259/SO-239, BNC-banana, attenuators |

Attenuators in their own labeled slot — a 30 dB attenuator looks identical to a barrel adapter until you key up.

### Bag Contents — KO6NNH Field Kit

Sized for this specific gear: (tr)uSDX (BNC), H4M (SMA), NanoVNA (SMA), TinySA (SMA), RTL-SDR (SMA), Z-match/ATU-100 (SO-239).

**Must have — covers all field paths:**

| Adapter | Qty | Use |
|---|---|---|
| SMA M-M barrel | 2 | Extend SMA cables; H4M accessories |
| SMA F-F barrel | 1 | Join two SMA male cables |
| SMA right-angle M-F | 1 | H4M/RTL-SDR body — lever protection |
| BNC M to SMA F | 2 | (tr)uSDX or BNC coax ↔ H4M / NanoVNA / TinySA |
| BNC F to SMA M | 1 | Reverse path |
| SO-239 to BNC M | 1 | Z-match / ATU-100 ↔ BNC coax |
| SO-239 to SMA M | 1 | Z-match / ATU-100 ↔ SMA coax |
| BNC M-M barrel | 1 | Join two BNC cables |

**Nice to have:**

| Adapter | Why |
|---|---|
| N M to SMA F | Weatherproof outdoor connectors ↔ SDR gear |
| N M to BNC M | Outdoor ↔ (tr)uSDX chain |
| 10 dB SMA attenuator | Finer control when 30 dB is too much |
| BNC T-splitter | NanoVNA monitor while transmitting into dummy load |

**Leave out:** F-connector, RCA, MCX — nothing in this kit uses them.

**Four critical paths to verify before each field session:**
1. H4M SMA → antenna in use
2. NanoVNA SMA → feedpoint under test
3. (tr)uSDX BNC → Z-match SO-239
4. H4M SMA → 30 dB attenuator → dummy load coupling point

### Protect SMA Center Pins

Cut craft foam squares to line each pouch. SMA barrel adapters will catch center pins on each other and bend them. One layer of foam stops this completely.

### Connector Chaining — Field Rules

- **1 adapter:** fine
- **2 adapters:** acceptable with care
- **3 adapters in series:** stop — get a direct adapter instead

Each junction adds insertion loss (0.1–0.3 dB), a small VSWR bump, and a mechanical failure point. Chain caps at the weakest link: cheap BNC connectors limit the system to ~1 GHz, blocking ADS-B (1090 MHz) and similar.

**Mechanical rule:** Each adapter adds lever arm. PCB-mounted SMA connectors (H4M, RTL-SDR dongles) are especially vulnerable — a bump while a chain of adapters is attached can crack a PCB pad. Use a right-angle SMA adapter directly at the body; lever stress then runs parallel to the board instead of against the solder joint.

**BNC bayonet:** Can work loose without visible indication. Finger-tighten after every move or position change.

### Alternatives for Bench Use

**Plano 3500 micro tackle box:** Hard shell, individual compartments, latches shut. Better pin protection, less packable. Good for bench kit or gear box that travels in a hard case.

**Altoids tin + foam:** Holds 6–8 adapters, shirt-pocket size, free. No visibility into it — fine if contents are consistent and memorized.

---

## Field Antenna Selection — Scenario Table

Match operating situation to antenna. All assume QRP (5–20W) and one-handed operation from Merced CA.

| Scenario | Antenna | Why |
|---|---|---|
| POTA site, trees >40 ft | Linked EFHW, sloper or inverted-V | Best efficiency, multiband, no radials needed |
| POTA site, trees 20–40 ft | EFHW horizontal or tapped dipole | Compromise for limited height |
| Open park, grass, no trees | 5.6m whip + 2–3 staked radials | Deploys anywhere, no vertical support |
| Open park, concrete, no stakes | 5.6m whip + frame elevated counterpoise | Chair frame above pavement beats wire on concrete |
| Fixed wheelchair, trees available | Fishing pole + EFHW from RAM mount | ~85% efficiency, one-handed |
| Fixed wheelchair, no trees | 5.6m whip on RAM mount + staked radials | Primary WC field configuration |
| Rolling / pedestrian mobile | 1.5m loaded whip + ATU-100 | Works while moving; lower efficiency |
| Walker operating | Frame tapped vertical or fishing pole + EFHW | Basket carries rig; frame = counterpoise |
| 40m regional, evening | 5.6m slider at 40m + 10.48m counterpoise | Good regional propagation; simple deploy |
| 20m DX, daytime | EFHW or 5.6m bare at ~5m | Both work; EFHW more efficient |
| NVIS / regional emergency (40m/80m) | Horizontal dipole or EFHW at <15 ft | High angle = NVIS skip; low wire = regional |
| Fast deploy (<5 min) | 5.6m whip bare (10–20m) | On the air fast; tune after QSO if needed |
| Band survey / site recon | H4M + 5.6m whip for RX; TinySA for TX check | Characterize site before full deployment |
| VHF/UHF voice, local | Baofeng + Nagoya NA-771 | Repeater access, simplex, weather |
| VHF fixed position (ARES/RACES) | 2m J-pole on fishing pole | Best VHF gain for portable fixed deploy |
| Compact / lightweight priority | Linked EFHW + unun only | Entire HF antenna is wire; fewest parts |
| No trees, need HF multiband fast | Linked EFHW sloper from fishing pole on RAM mount | Pole + wire covers 40m–10m in 5 min |

**Default field kit sequence:**
1. Arrive → assess trees and ground surface
2. Trees available: fishing pole + linked EFHW sloper; 1–2m counterpoise from unun
3. No trees: 5.6m whip + staked counterpoise; slider to band
4. On concrete / pavement: elevate counterpoise on chair arms; LOCAL below 2 MHz
5. Verify resonance with NanoVNA; adjust; make contact

---

## Field Dummy Load

A 50Ω resistive load for TX testing: harmonic checks with TinySA, rig keying without antenna, sidetone monitoring without RF.

**HN-31 Cantenna** — bench item only. Heavy (5 lbs oil-filled), rated 150W. Not portable.

**Field options:**

| Option | Rating | Connector | Notes |
|---|---|---|---|
| BNC 50Ω terminator plug | 1–2W brief | BNC male | $3; fine for low-power keying check and sidetone; not sustained TX at 5W |
| QRPGuys Mini Dummy Load kit | 10W continuous, 20W peak | SMA or BNC | ~$12; fits in pocket; SMA version fits connector bag |
| MFJ-264 or equivalent BNC load | 300W | BNC female | Heavier than mini; field-usable if carried |
| DIY: 4× 200Ω 2W resistors in parallel | 8W continuous | Any housing | 50Ω; solder in parallel in a BNC housing; tiny |

**Recommended for field kit:** QRPGuys Mini Dummy Load, SMA version. Weighs ~15g, handles (tr)uSDX at 5W continuously. Fits in connector bag alongside attenuators.

**Harmonic check connection:**

```
(tr)uSDX BNC → BNC M-to-SMA F → QRPGuys SMA load → SMA T-splitter → 30 dB SMA attenuator → TinySA LOW port
```

Or use the BNC T-splitter method:
```
(tr)uSDX → BNC dummy load → BNC T-splitter tap → 30 dB attenuator → TinySA
```

At 5W (+37 dBm): 30 dB attenuator → +7 dBm at TinySA — within +10 dBm max. FCC Part 97 limit: harmonics ≥43 dBc below fundamental at >5W.

**Sidetone monitoring:** Key the rig into the dummy load; plug headphones into (tr)uSDX audio jack; listen to sidetone without any RF on the antenna. Useful for paddle/keyer practice at full speed in a noisy environment.

---

## Ground Effects on Antennas — Theory

Ground affects an antenna two separate ways: **far-field pattern shaping** (height in wavelengths bends the radiation angle) and **near-field loss** (soil conductivity eats power before it radiates). They depend on different variables and don't trade off against each other — a site can be great for one and bad for the other.

**Far-field: ground as reflector.** The direct wave and the ground-reflected wave combine; the phase relationship depends on height in wavelengths, not feet.

- Horizontal wire (dipole, EFHW) at <0.25λ: reflection phase-flips, producing high-angle NVIS radiation — good for regional 40m/80m, useless for DX.
- Horizontal wire at ≥0.5λ: low-angle lobes emerge — needed for DX. Same wire, different height, different antenna.
- Vertical: ground reflection reinforces low-angle radiation at any practical height — the appeal of verticals for DX — but only if the near-field ground loss (below) doesn't eat the gain first.

**Near-field: ground as resistor.** Within roughly 1–2λ of the antenna, RF current induces eddy currents in the soil. Poor conductivity turns that into I²R heat instead of radiated power — this is what the [R_ground survey](#r_ground-field-measurement--nanovna-method) below measures.

- Verticals suffer most: current maximum is at/near ground level, right where the loss happens. A ground-mounted vertical with an inadequate radial field can lose 50%+ of its power into the dirt. This is the whole reason [counterpoise](counterpoise.md) length, count, and elevation matter so much for the 5.6m whip.
- Horizontal antennas are far less loss-sensitive — the high-current center is up in the air, away from the lossy region. Ground mainly reshapes their pattern (above), not their efficiency. This is part of why the EFHW needs no ground system for efficiency (only a short counterpoise for common-mode suppression).

| Antenna type | Height sensitivity (pattern) | Ground-loss sensitivity |
|---|---|---|
| Low horizontal (<0.25λ) | High-angle NVIS | Low |
| High horizontal (≥0.5λ) | Low-angle DX | Low |
| Vertical, ground-mounted, few radials | Always low-angle capable | Very high |
| Vertical, extensive radials / elevated counterpoise | Always low-angle capable | Low–moderate |

**Practical takeaway:** for the 5.6m whip, the question at a new site is really two independent questions — "how good is the dirt / how good is my radial system" (loss, addressed by counterpoise design and radial count) and, for any horizontal wire in the kit, "how high can I get it" (pattern, NVIS vs DX). Soil conductivity varies by an order of magnitude or more between wet valley clay and dry decomposed granite — see the seasonal σ map below.

---

## R_ground Field Measurement — NanoVNA Method

Soil conductivity (σ) varies by season, soil type, and moisture. Measuring it at each site builds an empirical map that improves antenna modeling accuracy. This procedure uses equipment already in the field kit.

**Purpose:** Compare actual feedpoint impedance to theoretical model → infer ground loss → estimate σ. Relative site-to-site comparison, not absolute measurement.

**Reference antenna:** 5.6m whip resonant on 40m, 4 radials staked at 90° intervals.

**Procedure:**

1. Deploy 5.6m whip with slider coil at 40m position
2. Stake 4 radials, each 10.5m, equally spaced around base
3. SOLT calibrate NanoVNA at feedpoint (calibrate at feedpoint end, not at NanoVNA)
4. Set NanoVNA: 6.5–8.0 MHz, CH0 S11
5. Tune slider until SWR dip hits 7.150 MHz (resonance)
6. Read the feedpoint resistance at resonance: the real part of Z on Smith chart, or Z readout
7. Record R_feed

**Interpreting R_feed:**

| R_feed (Ω) | Ground condition | Approximate σ | Typical site |
|---|---|---|---|
| 36–42 | Excellent | >10 mS/m | Wet valley floor, irrigated farmland |
| 42–55 | Good | 5–10 mS/m | Grass, moist soil |
| 55–70 | Average | 1–5 mS/m | Mixed soil, semi-arid |
| 70–90 | Poor | 0.5–1 mS/m | Dry foothills, gravel |
| >90 | Very poor | <0.5 mS/m | Rocky ridgeline, dry decomposed granite |

Theory: Rrad for a resonant quarter-wave vertical = ~36Ω. Measured R_feed = Rrad + R_ground_loss. Lower R_feed = lower ground loss = better conductivity.

**Four-season windows:** Wet (Jan–Mar), Declining (Apr–May), Dry (Jun–Sep), Recovering (Oct–Dec). Central CA soil conductivity varies significantly — valley floor wet season may be 10× better than ridgeline dry.

**Record on site log card:**

| Field | Entry |
|---|---|
| Date | |
| Location / GPS | |
| Season window | Wet / Declining / Dry / Recovering |
| Soil type | Clay / Loam / Sand / Gravel / Rock |
| R_feed (Ω) | |
| Estimated σ (mS/m) | |
| M3 map value | (from ITU M3 map for comparison) |
| Notes | |

**Compare to ITU M3 map:** The M3 map gives predicted median ground conductivity. Your measured σ tells you whether the actual site is above or below the map prediction — important for propagation modeling.

---

## VHF/UHF Field Antennas

For local voice: RACES/ARES coordination, repeater access, simplex at the site, NOAA weather, APRS monitoring.

### Baofeng Antenna Upgrade

**Stock rubber duck:** Compromise whip tuned for neither 2m nor 70cm. Works close range; 1–3 dB below a proper whip.

**Nagoya NA-771:** 15" SMA male dual-band whip. Proper element geometry. Noticeable improvement on 2m; adequate on 70cm. Fits both UV-5R and UV-5R Mini (SMA female radio body). **Recommended upgrade — already in kit.** ~$12.

**ANT500 adjustable whip (already in H4M kit):** Extends to 5.6m equivalent at HF, shortens to VHF/UHF at shorter lengths. Useful for TinySA signal survey on any band. Not a practical Baofeng replacement — too long for portable voice.

### 2m Field Antennas

| Antenna | Gain | Deploy | Notes |
|---|---|---|---|
| Nagoya NA-771 on radio | ~2 dBi | Instant | Already in kit |
| Rollup J-pole (tape measure) | ~3 dBi | 2 min | Coax feed, SMA or BNC; hang from tree or pole |
| Slim-jim on 450Ω ladder line | ~3 dBi | 5 min | Better pattern than J-pole; haul up on fishing pole |
| Webbing J-pole (field TM version) | ~3–4 dBi | 5 min | `/claude/web_j_pole/` project; 3" webbing construction, 16-band |
| 5/8λ vertical (makeshift) | ~4 dBi | 10 min | Needs matching stub; not standard kit |
| Yagi (tape measure 3-el) | 7–8 dBi | 15 min | Directional; for weak-signal or SAR point-to-point |

**For most POTA/ARES field use:** Rollup J-pole on a fishing pole gives 3 dBi for under 2 oz; hang it, plug in the Baofeng, done. A proper J-pole beats the rubber duck by a full S-unit on receive.

**J-pole construction — simple field version:**
- 2m J-pole: total length 155 cm; ~97 cm radiating element + ~49 cm matching stub + feed gap; coax feed at gap (~3 cm from short)
- Use 450Ω window line or 300Ω twin-lead as the radiating element
- Roll up on a card or stuff in a ziploc; takes seconds to deploy
- Feed with 50Ω coax, no balun needed at 5W

**Webbing J-pole project:** `/claude/web_j_pole/` — 3" webbing construction covers 2m and 70cm (and 16 other bands); Navy TM format documentation.

### APRS on 144.390 MHz

Receive-only monitoring is useful for checking traffic and locating other hams in the area. The UV-5R Mini is adequate for APRS monitoring with the stock duck. Key up requires being on frequency — UV-5R Mini needs the channel programmed to 144.390 MHz, FM, no CTCSS.

### VHF/UHF Purity Check — Baofeng

Same procedure as the HF purity check with TinySA. Key differences:
- UV-5R Mini: +33 dBm (2W) — use 30 dB SMA attenuator minimum
- UV-5R: +37 dBm (5W) — use 30 dB + 10 dB stacked attenuators
- **Critical harmonic:** 3rd harmonic of 146 MHz = **438 MHz** — inside 70cm amateur band; FCC Part 97 requires ≥40 dBc
- Check this before field use; documented in `baofeng_UV5R_purity_test.md` and `baofeng_uv5r_mini.md`

---

## Center-Fed Dipole — Single Band

The simplest resonant HF antenna. Half-wave total length, fed at center. Reliable starting point when trees are available and you need one band reliably.

**Why:** Feedpoint impedance at resonance ~50–72Ω — nearly a direct 50Ω coax match. No balun required at QRP power. Easy to understand, easy to troubleshoot.

### Element Lengths (each leg = λ/4)

| Band | Freq | Each leg | Cut long |
|---|---|---|---|
| 40m | 7.150 MHz | 10.48m (34.4 ft) | 10.65m |
| 20m | 14.200 MHz | 5.28m (17.3 ft) | 5.40m |
| 15m | 21.200 MHz | 3.54m (11.6 ft) | 3.60m |
| 10m | 28.300 MHz | 2.65m (8.7 ft) | 2.70m |
| 6m | 50.300 MHz | 1.49m (4.9 ft) | 1.55m |

Cut long, trim to resonance with NanoVNA. Inverted-V legs are slightly shorter than flat-top legs — start with the cut-long values above and trim.

### Construction

1. Cut two equal legs of 30 AWG PTFE wire
2. Strip ~5mm at each inner end; solder or crimp to center connector (BNC female or SO-239 chassis mount)
3. Center conductor → one leg; shield/ground → other leg
4. Add a common mode choke at the feedpoint (4× Mix 31 snap-on clamps) if you notice SWR varying when touching coax
5. Attach a short piece of paracord or monofilament to the center for hauling up

### Deployment — Inverted-V

- Apex as high as possible — 25+ ft gives good low-angle radiation; 15 ft works for NVIS
- Both legs droop at ~45° from apex — total included angle 90°
- Tips anchored to ground stakes, trees, or chair footrests
- Feedpoint at apex: haul up with throw bag line over branch; coax hangs down from center
- One-handed: throw line → haul apex → stake one leg → clip other leg to frame/chair

**Feedpoint impedance in inverted-V:** ~35–50Ω at 90° leg angle — slightly lower than flat top. Still a direct coax match at QRP.

### Ground Interaction — Apex vs. Ends

The apex (feedpoint) and the drooping ends interact with ground through two different mechanisms, because current and voltage are inverted between them:

| Location | Current | Voltage | Ground interaction |
|---|---|---|---|
| Apex (feedpoint) | Maximum | Near-zero | Height in λ sets far-field pattern angle (see [Ground Effects on Antennas — Theory](#ground-effects-on-antennas--theory)); negligible I²R ground loss since it's up high |
| Ends (low, near ground) | Near-zero | Maximum | Capacitive detuning + minor dielectric loss — not I²R ground loss (too little current there to couple resistively into soil) |

The ends being close to the ground does **not** cost power the way a vertical's low-current-node-free base does — there's essentially no current there to dissipate into lossy soil. Instead, the high-voltage ends couple capacitively to whatever's nearby (damp soil, wet grass, foliage), which **electrically lengthens** the wire and pulls resonance down. This is why inverted-V legs consistently trim shorter than flat-top legs (line above) — the end-to-ground capacitance is already doing some of the "lengthening" for you, so less wire is needed to hit the same resonant frequency.

Practical effects:

- **Steeper leg angle (lower ends, closer to ground) → more detuning, more trimming needed, and a somewhat higher-angle pattern** — the low end sections contribute less low-angle radiation than a flat-top would at the same apex height.
- **Shallower leg angle (ends kept higher, ≥45° from vertical / closer to flat-top) → pattern and resonance stay closer to the flat-top ideal.** Use shallow angles when trees allow it and DX performance matters more than compact footprint.
- Always trim to resonance with the NanoVNA *after* final deployment height and leg angle — cut-long dimensions in the table above assume a nominal 90° included angle; wetter ground or lower ends will pull resonance further down than the table accounts for.
- Keep end insulators clear of anything touchable — the voltage antinode there is also where an RF burn risk exists at higher power, though negligible at 5W QRP.

### vs EFHW

| | Center-Fed Dipole | Linked EFHW |
|---|---|---|
| Efficiency | ~90–95% | ~85% |
| Ground required | No | No |
| Tuner | Not needed (one band) | Not needed (any linked band) |
| Multiband | No (one band per build) | Yes |
| Feedpoint height | Must be elevated | Can be at low height; wire goes up |
| Coax | Required | Required |
| Complexity | Minimal | Moderate (links, unun) |

**Use when:** One band only, trees available, simplest possible setup. For multiband or when feedpoint access is difficult: EFHW.

---

## Random Wire — Non-Resonant End-Fed

Any length of wire fed at one end, through a tuner, with a counterpoise. Not resonant by design — works on multiple bands because the tuner handles the mismatch.

**When to use:** Wire is what you have — cut to whatever length clears the trees. No need to pre-cut to resonant length. Works any time the tuner can match.

### Wire Lengths — What to Avoid

Some wire lengths present very high or very low impedance on common HF bands — the tuner may not handle these extremes.

**Avoid these lengths:**
| Length | Problem |
|---|---|
| 5.0m | λ/2 on 10m — very high Z, hard to match |
| 10.1m | λ/2 on 14 MHz — very high Z |
| 20.3m | λ/2 on 7 MHz — very high Z |
| 40.5m | λ/2 on 3.6 MHz — very high Z |

**Good lengths (avoid half-wave multiples):**
- 7m, 9m, 12m, 17m, 23m — work well across 40m–10m with a Z-match

**Rule of thumb:** Odd multiples of λ/4 (quarter-wave, 3/4-wave, etc.) present moderate impedance — easier for the tuner. Half-wave multiples present very high impedance at the feedpoint — Z-match handles it, ATU-100 may not.

### Setup

1. Run wire at whatever angle available (sloper works well)
2. Feed at rig end through Z-match or ATU-100
3. Counterpoise: λ/4 on lowest band you plan to use; stake or clip to frame
4. Tune Z-match to minimum SWR on each band
5. Expect retuning each band change — not resonant, must re-tune

### Efficiency vs Resonant Antenna

Random wire via tuner: 60–80% typical. Losses in tuner matching network + feedline mismatch. Acceptable at QRP — 5W with 75% efficiency = 3.75W effective. Adequate for CW contacts when resonant antenna isn't available.

**Z-match advantage:** Handles impedance extremes (50Ω to several kΩ) better than ATU-100's L-network range. For random wire, Z-match is the preferred tuner.

---

## Propagation — Band Characteristics from Merced CA

Solar cycle 25 near maximum 2025–2026. From 37°N, 120°W (central California).

### Band Guide

| Band | Hours (local) | Range | Conditions | Notes |
|---|---|---|---|---|
| 80m | Evening–dawn | Regional 500–1500 km | High noise at QRP; NVIS night | Good for 40m when 40 is too long |
| 60m | Day + night | Regional NVIS | 5 channels, CW/digital only | Reliable NVIS fill-in; less crowded |
| 40m | Evening–morning | Regional; DX overnight | Workhorse; best 2100–0600 local | Best first-contact band for CW QRP |
| 30m | All day | Medium distance | No phone; CW/digital only; quiet | Reliable daytime when 20m unstable |
| 20m | 0800–1800 local | Continental/DX | Opens at sunrise; closes near sunset | Primary daytime DX band at QRP |
| 17m | Day | DX, long path | Near solar max = excellent | Often open when 20m is crowded |
| 15m | 0900–1700 local | DX worldwide | Solar max: daily openings | 10–15 dB better than solar minimum |
| 12m | Day, variable | DX | Opens same time as 15m or later | Check before transmitting |
| 10m | 1000–1600 local | Worldwide or sporadic | Near solar max: daily DX openings | 10m QRP = worldwide at cycle peak |
| 6m | Sporadic-E | Regional to DX | May–August mainly; unpredictable | 10m opening + 6m Es = great DX |

### Solar Indices — What to Watch

| Index | Good | Poor | Where to get it |
|---|---|---|---|
| SFI (Solar Flux) | >150 | <100 | proppy.hamqsl.com, DX cluster |
| A-index | <10 | >20 | Quiet ionosphere = good propagation |
| K-index | <3 | >4 | Geomagnetic stability — check before DX attempt |

**Rule:** SFI > 150 + A < 10 + K < 3 = excellent conditions on 10m–20m. If K > 4, 40m/80m may be disrupted but 20m–10m often recovers first.

### Gray Line — Best DX Window

Gray line (terminator between day and night) at Merced:
- Sunrise: roughly 0600–0730 local (seasonal)
- Sunset: roughly 1900–2030 local (seasonal)

Gray line moment: 40m and 80m open for worldwide DX as the ionosphere transitions. 10–20 minutes. Use the astrolabe in the field kit to determine local sunrise/sunset precisely.

### QRP Calling Frequencies (CW) — from Merced CA

| Band | Frequency | Notes |
|---|---|---|
| 40m | 7.030 MHz | QRP watering hole; SKCC traffic; start here |
| 40m | 7.000–7.025 MHz | Extra-only corner; quieter |
| 20m | 14.057 MHz | QRP calling |
| 20m | 14.000–14.025 MHz | Extra-only corner; DX |
| 30m | 10.106–10.116 MHz | CW segment; no phone |
| 15m | 21.060 MHz | QRP; solar max = good |
| 10m | 28.060 MHz | QRP; solar max = worldwide |

**SKCC:** Straight Key Century Club. Member sked page: sked.skccgroup.com. 40m and 20m main activity. Patient operators — good for first field contacts.

---

## Sloper Antenna

A sloper is a wire antenna running at an angle downward from one high attachment point to a low stake — typically 20–45° from horizontal. Easier to deploy than a flat-top dipole (one high point instead of two), slightly directional toward the low end, and DX-capable because of lower radiation angle than an inverted-V at the same apex height.

### Types

**Half-wave sloper:** Wire is a full half-wavelength, fed at the high end. One end high (mast, tree, fence top), wire slopes to a ground stake. Feed with coax — impedance 50–100Ω depending on slope angle. For T-60, this is the practical choice.

**Quarter-wave sloper:** One quarter-wave wire slopes down from mast top; the mast or structure provides the return path. Impedance ~25–40Ω at the mast.

### 40m Sloper for T-60 (Active Build)

Box 2 provision feed point, fence attachment at ~10 ft:
- **Wire length:** ~20.4m (67 ft) half-wave at 7.15 MHz — cut to 20.7m, trim to resonance
- **High end:** Box 2 coax port on fence (~10 ft height)
- **Low end:** Ground stake 15–20m from fence toward back of yard
- **Slope angle:** ~20–25° at 10 ft with a 20m run
- **Feed:** Coax to Box 2; 1:1 choke balun at feedpoint (Mix 31, 7T RG-174) to keep feedline RF off coax shield

**Directional property:** Favors the direction toward the low end. Aim low end NE (~30°) for Europe, SE (~130°) for South America. For general use, aim toward the open backyard.

**Tuning:** Trim from the far (low) end. NanoVNA at feedpoint (Box 2 box), SOLT cal. Target < 2:1 at 7.030–7.200 MHz. Fine-tune at the QRP watering hole (7.030 MHz).

### Sloper vs Inverted-V Comparison

| | Inverted-V | Half-wave sloper |
|---|---|---|
| High points needed | 1 apex | 1 high + 1 low stake |
| Directional | No | Yes — toward low end |
| Radiation angle | High (NVIS/regional) | Lower = better DX |
| Solo deployment | Easy | Easy |
| 40m height needed | ≥15 ft apex for HF | 10 ft is workable |
| Efficiency | ~85% | ~80–85% |

---

## NVIS — Near Vertical Incidence Skywave

NVIS produces a nearly vertical radiation pattern. Signals go nearly straight up and return to ground within 0–600 km. Zero skip zone — you can work stations 30 km away that a DX-aimed antenna would miss.

### When NVIS Matters from Merced CA

- 40m NVIS covers 100–500 km: LA (530 km), SF (210 km), Sacramento (160 km), Reno (320 km)
- 80m NVIS covers 100–300 km: same area, better at night
- Emergency comm (ARES/RACES): shelter-to-shelter contact within 300 km
- POTA: contact hunters at nearby parks; grid square chase within NVIS range
- Gray line: 40m NVIS into NVIS is reliable during regional gray line openings

### The NVIS Band/Time Window

| Band | Best hours | NVIS range | Notes |
|---|---|---|---|
| 80m | Night: 1800–0800 local | 100–300 km | Noisy daytime; excellent nighttime regional |
| 60m | Day + night | 100–400 km | 5 channels; less crowded than 40m; reliable |
| 40m | Day: 0800–1800 local | 100–500 km | NVIS during day; DX at night (different mode) |

### Antenna Geometry

NVIS requires a *horizontal* antenna at *low height*. Low height forces high radiation angles — exactly what NVIS needs.

**Height targets:**
- 40m: 4–8m (13–26 ft) for NVIS-dominated pattern; even lower works
- 80m: 8–12m (26–40 ft); lower is fine for pure NVIS but efficiency drops
- Rule: height < 0.15λ produces NVIS. For 40m: 0.15λ = 6.4m (21 ft). For 80m: 12.5m (41 ft).

**Flat-top beats inverted-V for NVIS:** Inverted-V legs slope downward, partially canceling vertical-angle radiation. A flat horizontal dipole at 10 ft is excellent 40m NVIS. An inverted-V at the same apex height is less effective for NVIS but more effective for DX.

### Field NVIS Deployment

**Simplest:** Hang a center-fed dipole as nearly horizontal as trees and supports allow, at 8–15 ft. Coax drops straight down from center. Cut for operating band. No tuner needed.

**Using fishing pole:**
- Pole to 5–6m, dipole center at pole tip
- Both legs slope slightly downward to low stakes (5–15° from horizontal — near-flat)
- 1:1 choke balun at feedpoint; coax to rig

**Random wire at low height:** Any non-resonant horizontal wire at < 20 ft works as NVIS with a Z-match tuner. Cat5/6 at fence height is NVIS.

**NVIS is forgiving:** Exact height and orientation matter less than for DX. Get the wire horizontal and low, use any tuner, and it works.

---

## T/R Switching — Transmitter and Receiver on Same Antenna

The T-60 transmits at 60W. The SW-7800 front-end maximum safe input is approximately −10 to −20 dBm (0.01–0.1 mW). Switching without isolation permanently damages the receiver. A coax antenna switch physically disconnects the receive port before transmit.

### Switch: MFJ-1702C

**Recommended:** MFJ-1702C (2-position coax switch, SO-239 connectors, rated 1.5 kW — well within T-60's 60W)
- COMMON → antenna feedline
- PORT 1 → T-60 output SO-239
- PORT 2 → SW-7800 input SO-239
- Lever selects TX or RX position; clearly labeled

All connectors are SO-239 — PL-259 jumpers throughout, no adapters needed.

### Switching Sequence — CW

1. MFJ-1702C to RX; SW-7800 on antenna — listen, find clear frequency
2. T-60 to OPERATE, VFO/crystal zero-beated to target (see below)
3. MFJ-1702C to TX (T-60 to antenna)
4. Transmit
5. Transmission complete — T-60 to STANDBY
6. MFJ-1702C back to RX (SW-7800 to antenna)
7. Listen for response

**The T-60 has no PTT line.** No auto-trigger for the switch. Manual lever on MFJ-1702C only option with this rig. This is normal for vintage CW operation — the switching rhythm becomes part of the CW operating style.

### Zero-Beat Procedure

1. MFJ-1702C to RX; SW-7800 tuned to target frequency; BFO pitch at natural 600–800 Hz
2. Note the pitch of the received carrier
3. T-60 STANDBY; adjust VFO or verify crystal is on frequency
4. Key T-60 briefly — SW-7800 should hear own sidetone if monitoring (break-in or split)
5. Alternatively: tune T-60 external VFO until SW-7800 audio tone matches the note you heard from the DX station
6. MFJ-1702C to TX; transmit on the zero-beat frequency

With T-60 crystal control: crystal frequency is fixed. Zero-beat check is a one-time verification that crystal frequency matches where you're operating. With VFO: zero-beat every QSY.

### Connector Diagram

```
Antenna
  |
SO-239 feedline
  |
MFJ-1702C COMMON
  ├── PORT 1 → coax → T-60 SO-239 output
  └── PORT 2 → coax → SW-7800 SO-239 antenna input
```

---

## Feedline Coax — Field Comparison

At 5W, coax loss matters more proportionally than at QRO. 1 dB loss at 100W leaves 79W — fine. 1 dB at 5W leaves 4W — you notice it.

### Loss per 100 ft at HF

| Cable | 7 MHz | 14 MHz | 28 MHz | 50 MHz | Weight | Notes |
|---|---|---|---|---|---|---|
| RG-174 | ~0.6 dB | ~0.9 dB | ~1.4 dB | ~1.9 dB | 0.5 oz/ft | Field kit cable; fine ≤25 ft on HF |
| RG-58A | ~0.35 dB | ~0.5 dB | ~0.75 dB | ~1.0 dB | 1.2 oz/ft | Step up for 25–50 ft HF runs |
| RG-8X | ~0.20 dB | ~0.30 dB | ~0.45 dB | ~0.60 dB | 1.6 oz/ft | Best portable-to-shack bridge cable |
| LMR-400 | ~0.05 dB | ~0.07 dB | ~0.12 dB | ~0.17 dB | 3.8 oz/ft | Infrastructure; not field-portable |

For a 25 ft run of RG-174 at 14 MHz: ~0.22 dB — less than 5% power loss. Negligible for field HF work.

### Field Rule

- Run ≤ 25 ft, HF bands: RG-174 fine
- Run 25–50 ft at 28 MHz or above: RG-58A or better
- Run 50–100 ft to shack: RG-8X minimum
- 144 MHz and above beyond 10 ft: RG-174 unacceptable; LMR-195 or LMR-240

### Connector Loss at HF

Each PL-259/SO-239 joint: ~0.1 dB. Each BNC: ~0.05 dB. Each SMA: ~0.05 dB. An adapter chain of 3 pieces adds ~0.3–0.5 dB — comparable to several feet of RG-174. Minimize adapter stacks.

---

## Fiber-Optic RF Link — Feedpoint to Shack

Analog or digital RF-over-fiber (RFoF) between the antenna feedpoint and the radio, in place of coax. Primary use case: noise-floor-critical receive antennas (Beverage, K9AY, R_ground remote heads, phased-array elements) where coax shield common-mode pickup sets the noise floor, not the antenna itself.

### Why Fiber Instead of Coax

- Complete galvanic isolation — no ground loop, no lightning path along the line, no shield current
- Fiber radiates nothing and picks up nothing over the run — immune to RFI/EMI regardless of run length
- Removes the #1 failure mode of long receive-antenna feedlines: house noise riding in on the coax shield despite chokes

### Architecture

| Stage | Feedpoint End | Shack End |
|---|---|---|
| Analog RFoF | Antenna → (optional LNA) → E/O laser module → fiber | Fiber → O/E photodiode + RF amp → short coax → radio |
| Digital RFoF | Antenna → SDR receiver → Ethernet-over-fiber media converter | Media converter → Ethernet → PC/SDR client |

### Power at the Feedpoint — The Catch

Fiber carries no power. The feedpoint box needs its own supply:
- Battery + solar panel (same power budget class as the portable field lab)
- Separate copper pair bundled with the fiber (hybrid cable) carrying DC
- PoE-style DC injection over a thin 2-conductor run alongside the fiber strand

This is the part that kills casual builds — budget for power delivery to the feedpoint box before the fiber link itself.

### Analog RFoF — Dynamic Range Limits

Direct-modulated laser diode: noise floor set by RIN (relative intensity noise), upper limit set by laser IM3. For HF (1.8–30 MHz) receive, repurposed CATV return-path fiber modules (5–42 MHz spec, cheap surplus) cover the band with adequate dynamic range. Not rated for TX power — analog fiber only carries drive-level signal; transmit power still needs a PA physically at the feedpoint, fed from the same battery/solar supply.

### Digital RFoF — the Practical DIY Path

SDR (RTL-SDR, HackRF, or similar) sits at the feedpoint, digitizes locally, sends I/Q over Ethernet-over-fiber media converters — standard, cheap, off-the-shelf gear. No RIN/IMD to worry about, large dynamic range, but needs compute + power at both ends and adds latency — fine for receive-only noise-floor work, not for low-latency full-duplex transceive.

### Field Rule

- Receive-only, noise-floor-critical site (Beverage, phased-array element, R_ground remote head): digital RFoF (SDR + fiber Ethernet) is the practical build
- Transmit: PA must live at the feedpoint end; fiber only carries the drive signal up (analog) or command/data (digital)
- Full duplex on one fiber: WDM (different wavelength each direction) if both TX drive and RX feedback need to share a single strand
- Always budget separate power delivery to the feedpoint box — that's the real constraint, not the fiber run

---

## SWR and the (tr)uSDX

### What SWR Means at the Rig

SWR is impedance mismatch. High SWR causes reflected power back to the finals. With a tuner in line: the tuner absorbs the mismatch, the rig sees 50Ω — reflected power at the rig is minimal regardless of antenna SWR. **The tuner is where the mismatch is resolved.**

Without a tuner between rig and mismatched feedpoint:

| SWR at rig port | Reflected % | Power delivered | Effect at 5W |
|---|---|---|---|
| 1:1 | 0% | 100% = 5.0 W | Ideal |
| 1.5:1 | 4% | 96% = 4.8 W | Negligible |
| 2:1 | 11% | 89% = 4.5 W | Acceptable |
| 3:1 | 25% | 75% = 3.75 W | Tune better |
| 5:1 | 44% | 56% = 2.8 W | Fold-back likely |

### (tr)uSDX Behavior Under High SWR

IRF510 MOSFET finals are damage-sensitive to high reflected power. Firmware fold-back:
- SWR > 3:1: output power begins reducing to protect finals
- SWR > 5:1: heavy fold-back; may see < 1W output even at full drive setting
- Fold-back is protective, not a fault — rig is working correctly

**Key rule:** Tune for antenna resonance first, then engage the tuner. Do not rely on the tuner alone to cover a severely mismatched antenna. ATU-100 handles 10:1 SWR — but that means significant reflected power on the coax between rig and tuner if the tuner is not immediately at the rig port. Put the tuner physically next to the rig.

### When NanoVNA SWR Reports Are Misleading

NanoVNA reads SWR at the SOLT calibration plane. Calibrate at the rig port: measures what the rig sees (tuner output). Calibrate at the antenna feedpoint: measures the true antenna SWR (usually what you want). Feedline loss between these two points can make a bad antenna look better when measured from the rig end — lossy cable "absorbs" the reflected wave.

---

## First-Transmit Site Checklist

Complete before keying up at any new location. Takes 5 minutes; prevents blown finals, unintentional QRM, and mystery RF burns.

**Coax and connectors:**
- [ ] All PL-259/SO-239 connections hand-tight
- [ ] BNC bayonets clicked and finger-tightened
- [ ] SMA adapters seated, no cross-threading
- [ ] No coax kinks or sharp bends; coax not touching antenna wire

**Antenna:**
- [ ] NanoVNA SOLT calibrated at feedpoint end of coax (not at NanoVNA body)
- [ ] SWR < 2:1 at operating frequency; < 1.5:1 preferred
- [ ] Smith chart: dot on horizontal centerline = resonance
- [ ] Antenna physically clear — not touching metal, wet concrete, or vegetation at feedpoint

**Ground system:**
- [ ] Counterpoise clipped to unun ground lug (EFHW) or feedpoint ground (whip)
- [ ] Counterpoise wire deployed and not doubled back on itself
- [ ] Staked radials pushed in at correct depth if fixed position

**RF chain:**
- [ ] Common mode choke installed at feedpoint (4× Mix 31 snap-on for coax-fed antennas)
- [ ] Tuner adjusted: Z-match tuned to minimum SWR; ATU-100 tune sequence completed
- [ ] H4M / NanoVNA / TinySA: 30 dB attenuator inline if receiver is to remain connected during transmit

**Operating position:**
- [ ] Listen 60 seconds — confirm frequency is clear
- [ ] Log open: date, UTC time, frequency, band ready to write
- [ ] Key cable connected: sidetone fires on each dit

**Safety:**
- [ ] No overhead power lines within 30 ft
- [ ] No thunder audible — if yes, stop and pack up
- [ ] Receive instruments with no attenuator protection disconnected from antenna

**RFAFO** — Radiate Frequencies And Find Out. This checklist exists so you don't.

---

## DX Azimuths from Merced CA (37.3°N, 120.5°W)

Short-path bearings for directional antennas. Omnidirectional antennas (vertical whip, inverted-V) need no pointing.

| Destination | Azimuth (short path) | Distance | Best band/time |
|---|---|---|---|
| Europe (London) | 28° NNE | 8,500 km | 20m/15m daytime; 40m gray line |
| Scandinavia (Stockholm) | 16° NNE | 8,900 km | 15m/17m daytime; 20m solar max |
| Western Russia | 22° NNE | 8,700 km | 15m/20m daytime |
| Japan/Korea | 314° WNW | 8,300 km | 20m/17m gray line morning |
| Russia (Far East/Vladivostok) | 330° NNW | 7,700 km | 20m/15m daytime |
| Australia (Sydney) | 237° WSW | 12,000 km | 20m/17m afternoon; long path ~0630 local |
| New Zealand (Auckland) | 231° SW | 10,700 km | 20m/17m afternoon |
| Hawaii (Honolulu) | 244° WSW | 3,900 km | Any band; reliable QRP target |
| Alaska (Anchorage) | 356° N | 3,500 km | 40m/20m; 80m short skip |
| Mexico City | 152° SSE | 3,100 km | 40m evening; 20m day |
| South America (Buenos Aires) | 134° SE | 11,000 km | 17m/15m afternoon |
| Brazil (São Paulo) | 127° SE | 10,200 km | 20m/15m afternoon |
| East Coast USA (New York) | 72° ENE | 4,100 km | 40m evening; 20m day |

### Key Bearings to Memorize

- **Europe: ~30° NE** — primary DX target from CA; morning local on 20m/15m
- **Japan/Korea: ~315° WNW** — gray line target; tune at local dawn on 20m/17m
- **Australia: ~240° WSW** — afternoon DX; easy on 20m/17m at solar max
- **South America: ~130° SE** — afternoon; 17m/15m solar max

### Dipole Orientation for DX

A horizontal dipole radiates broadside (perpendicular to the wire axis). To favor a bearing, orient the wire *perpendicular* to that bearing:
- Europe at 28°: wire runs 118°–298° (SE–NW, roughly parallel to the coast)
- Japan at 314°: wire runs 44°–224° (NE–SW)
- Australia at 237°: wire runs 327°–147° (NW–SE)

Covering Europe and Japan simultaneously requires a roughly NE–SW wire orientation — each is close to broadside to that axis. A vertical antenna is omnidirectional and needs no orientation.

---

## Portable Mast Options

The 6–7m fiberglass fishing pole is already in the field kit. Other options when more height or rigidity is needed.

| Mast | Height | Collapsed | Weight | Cost | Notes |
|---|---|---|---|---|---|
| 6–7m fiberglass fishing pole | 6–7m | ~60 cm | ~150 g | $15–25 | Already in kit; lean, stake, or tie; soft tip |
| 10m squid pole (Spiderbeam/similar) | 10m | ~65 cm | ~380 g | $60–90 | Better 40m EFHW height; limited US retail |
| SOTAbeams 6m travel mast | 6m | ~55 cm | ~180 g | $35–50 | Stiffer than fishing poles; SOTA-designed |
| MFJ-1910 33-ft fiberglass | 10m | ~60 cm | ~450 g | $75 | US-available; solid for semi-permanent POTA |
| Jackite 31-ft push-up | 9.4m | ~133 cm | ~700 g | $85 | Large collapsed size; best when you drive in |
| SOTAbeams 12m travel mast | 12m | ~75 cm | ~720 g | $130 | Premium; 40m EFHW at ideal height |

### Field Recommendation

Already have the fishing pole: use it. It is adequate for 10m–80m EFHW from 6m height. The only reason to upgrade is needing > 7m height, which mainly benefits:
- 40m EFHW at > 20° elevation angle (needs ~10m apex)
- 80m EFHW approaching half-wave height
- Situations where the fishing pole flex is a problem in wind

**Securing the mast:**
- Soft ground: push base 15–20 cm into soil, lean slightly into wind
- Against tree: lash base to tree trunk at knee height with one paracord wrap
- Hard surface: camera tripod + pole adapter (Amazon); or RAM mount to fixed structure
- One-handed tip: wrap base loop of paracord around boot, step on it to anchor while extending

---

## Weather Effects on Antennas

### Rain

**Wire sag:** Rain adds weight, causing wire sag. A 20m dipole sagging 0.5m at center shifts resonant frequency 1–3% lower. SWR increases. If SWR drifts during rain, retune with Z-match or retrim the wire after the session.

**Ground improvement:** Wet soil dramatically lowers R_ground. If you measure lower feedpoint R than usual, the rain is working for you. R_ground on wet grass can be half the value of dry summer soil at the same site.

**Connector ingress:** Water in a PL-259 joint adds 1–6 dB loss immediately. Wrap outdoor connections with self-amalgamating tape before deployment when rain is expected. The unun/feedpoint box is the highest-risk item — cover it.

**The wire itself is fine wet.** Water on wire changes surface resistance negligibly. Focus protection on connectors and transformers, not the wire.

### Wind

**Antenna swing:** A swinging wire Doppler-shifts the signal slightly. Heavy swing makes CW copy noticeably worse. Options: stake both ends of the wire to reduce swing amplitude; add a small weight (0.5 oz) to the far end of the wire.

**Mast in wind:** Fishing pole at 6m with full 20m EFHW hanging from the tip catches significant windage. Push the base 20 cm into soil; add a single paracord guy from the upper section to a stake at 45°. One guy line is enough for the fishing pole.

**BNC connectors:** Wind vibration walks BNC bayonets loose. Finger-tighten after setting up and again after the first wind gust.

### Temperature

Minimal effect at HF. Wire length changes ~0.001% per °C — negligible. Cold temperatures stiffen RG-174's PVC jacket — connectors seat harder, SMA threads are more brittle. Let coax warm before making SMA connections below 5°C.

### Lightning — Protocol

**Audible thunder:** Stop transmitting. Disconnect coax from rig at the rig port.

**Visible lightning:** Disconnect coax and take down the antenna. Fold down the fishing pole and move away from the wire.

**30/30 rule:** If flash-to-thunder interval is less than 30 seconds (~6 miles), you are in the danger zone. Do not return to the antenna until 30 minutes after the last thunder.

**Why:** A 10m fishing pole with a 20m wire hanging from it is an efficient lightning collector. Lightning arrestors help fixed permanent installations where the coax runs through a building entry. For field portable, the correct protection is physical disconnection and distance — not a suppressor device.

**Field procedure:** Pack antenna, go to car or solid structure. The POTA log can wait. If the rig is in a dry bag away from the antenna, it is safe where it is.

## Loading Coil Calculation

A loading coil makes a short antenna resonant by replacing the missing inductive reactance. The antenna is electrically shorter than a quarter wave, so its feedpoint looks capacitive (negative reactance). The coil must supply equal and opposite inductive reactance.

**Step 1 — Measure the antenna's capacitive reactance with NanoVNA:**

Set NanoVNA to the target band. Connect to antenna feedpoint (SOLT cal done). Read feedpoint impedance as R + jX. If X is negative (e.g., −300Ω), the antenna is capacitive and needs a coil with X_L = +300Ω.

**Step 2 — Calculate required inductance:**

```
X_L = 2πfL  →  L = X_L / (2πf)

Example: 40m (7.1 MHz), X_C measured = 320Ω
L = 320 / (2π × 7.1×10⁶) = 320 / 44,611,000 = 7.17 µH
```

For center loading at the midpoint of the antenna, multiply by ~3–4 (center load is less efficient than base load — more inductance needed for same result). Field rule: if NanoVNA shows −300 to −400Ω at a center-load point on 40m, target 22–33 µH. This matches the empirical range already documented.

**Step 3 — Calculate turns from core AL value:**

```
N = √(L / A_L)    (L in same units as A_L)

T50-2 core: A_L = 5.7 nH/T²
For 25 µH = 25,000 nH:  N = √(25,000 / 5.7) = √4,386 = 66 turns

T37-2 core: A_L = 4.0 nH/T²
For 25 µH: N = √(25,000 / 4.0) = √6,250 = 79 turns
```

**Air-core solenoid (Wheeler formula):**

```
L (µH) = (N² × r²) / (9r + 10l)    [r and l in inches]

r = coil radius, l = coil winding length
```

For 60 turns on a 1.5" diameter form (r = 0.75"), winding length 3" (l = 3"):
```
L = (3600 × 0.5625) / (6.75 + 30) = 2025 / 36.75 = 55 µH
```

**Practical sequence:** Wind coil ~20% long → measure with NanoVNA → remove turns to raise resonance toward target → mark final position.

**80m center load (2.8m point on 5.6m whip):** X_C ≈ 900–1200Ω → target 80–130 µH. Air-core solenoid on 2" form, ~100–130 turns 18 AWG.

---

## Balun / Choke / Unun Theory

Three distinct devices; only one provides balance.

**Definitions:**

- **Balun**: Balanced to Unbalanced transformer. Connects a balanced feed (dipole legs, ladder line) to unbalanced feedline (coax). Provides both impedance transformation and common-mode suppression.
- **Choke (current balun)**: Forces equal and opposite currents in coax conductors by choking common-mode current. Does NOT transform impedance. Best for HF field use.
- **Unun**: Unbalanced to Unbalanced transformer. Changes impedance between two unbalanced ports. No balance provided. Used at EFHW feedpoint (49:1), verticals, matching networks.

**Common mode current:** Current that flows down the outside of the coax shield rather than inside. Causes RF in the shack, SWR that changes when you touch the coax, RF burns, distorted radiation pattern.

**Voltage balun vs current balun:**

| | Voltage balun | Current balun (choke) |
|---|---|---|
| Forces | Equal voltages at each port | Equal currents |
| Common mode | Does NOT block | Blocks |
| Impedance transform | Yes (e.g., 4:1) | 1:1 only |
| Field use | Avoid at HF | Preferred |

A voltage balun can allow substantial common-mode current to flow — it forces voltages equal but doesn't prevent common mode. For field operations, always use a current balun (choke).

**Which device where:**

| Antenna | Device at feedpoint | Why |
|---|---|---|
| Center-fed dipole | 1:1 current choke balun | Balanced antenna, unbalanced feedline |
| Inverted-V | 1:1 current choke balun | Same as dipole |
| EFHW | 49:1 unun + coax choke behind it | Unbalanced both sides; unun = impedance only; choke blocks feedline common mode |
| Vertical (coax-fed) | Coax choke at feedpoint | Unbalanced antenna; choke only, no impedance transform needed |
| Random wire | None at feedpoint — Z-match handles impedance | Z-match is unbalanced; no balance needed |
| Ladder line to Z-match | Nothing — ladder line IS balanced | Z-match balanced port handles it |

**Why a 49:1 unun is NOT a balun:** EFHW feedpoint is single-ended (hot wire + nothing). The unun transforms 2450Ω → 50Ω, both ports unbalanced. Common mode on the coax after the unun is suppressed by a separate choke wound on a Mix 31 toroid.

**Mix selection:** Mix 31 for HF chokes (1–30 MHz, high µ, high loss for common mode). Mix 43 for HF-VHF. Mix 73 for 160m/AM only. Mix 61 for VHF/UHF. For field HF choke: 4× Mix 31 snap-on clamps stacked, or 7 turns RG-174 on FT-140-31.

---

## Impedance Matching — L-Network Theory

An ATU-100, Z-match, or any antenna tuner works by inserting a reactive network between the rig (50Ω) and the antenna feedline (whatever Z the antenna presents). Understanding the network makes the tuner decisions in this file make sense.

**L-network basics:**

Two reactive elements (one series, one shunt) in an L shape. Can transform any impedance to any other impedance provided the larger impedance is on the shunt-element side.

```
High-Z side      Series L      Low-Z side
    ○────UUUU────○────────○
                 |
                 C (shunt)
                 |
                GND
```

- Series L raises impedance; shunt C lowers it
- Swapping arrangement (shunt L, series C) changes network behavior
- Two configurations of L-network cover all cases

**Q factor and loss:**

The network Q = √(Z_high/Z_low − 1). Higher impedance ratio = higher Q = more reactive energy = more I²R loss in coil resistance.

```
Random wire: 500Ω → 50Ω, Q = √(500/50 − 1) = √9 = 3
Loaded whip: 25Ω → 50Ω, Q = √(50/25 − 1) = 1
```

Higher Q also means better harmonic suppression (bandpass effect). The Z-match's coupled-resonator design achieves inherently high Q, giving harmonic attenuation beyond the (tr)uSDX's internal LPF.

**T-network (pi-network):** Two shunt elements + one series element. Can handle higher impedance ratios and provides more adjustment range. Knight T-60 pi-network output tank is this topology.

**ATU-100 internal:** L-network or T-network with binary-switched inductors and caps. Microcontroller sweeps combinations, measures SWR at each, stores best match in memory.

**Z-match internal:** Coupled resonant circuit (transformer with resonating caps). Primary coil + capacitor form a resonant bandpass filter. Output tap on the primary coil adjusts transformation ratio. The resonance gives it inherent harmonic suppression that an L-network alone doesn't provide.

**Component stress at QRP (5W):**

```
V = √(P × Z) = √(5 × 200) = 31.6V RMS across 200Ω random wire
I = √(P / Z) = √(5 / 25) = 0.45A through 25Ω whip loading coil
```

Both are low. At 60W (T-60): V = 109V, I = 1.55A — verify coil wire gauge (18 AWG handles 2.3A safely).

---

## FT8 / Digital Modes — Field Operation

FT8 and related weak-signal digital modes (FT4, JS8Call, WSPR) work differently from CW and SSB in ways that affect field antenna, power, and setup choices.

**FT8 protocol:**

- 15-second TX/RX slots synchronized to UTC
- Each transmission is 12.64 seconds of GFSK audio tones
- Software decodes to −22 dB S/N — approximately 17 dB better than CW, 25+ dB better than SSB
- 5W FT8 routinely makes 10,000+ km contacts that would require 500W SSB

**Duty cycle — (tr)uSDX thermal:**

FT8 runs 100% duty cycle for ~13 seconds per 15-second slot. The (tr)uSDX IRF510 finals are not rated for sustained 100% duty cycle at full output. Recommended: reduce TX power to 2–3W for FT8 operation. At 2W FT8, S/N advantage still makes it competitive with 20W SSB.

**Timing — critical requirement:**

FT8 requires computer clock synchronized to ±1 second of UTC. Options:
- Internet NTP (automatic when online)
- GPS sync: H4M has GPS; can set tablet clock from GPS before going off-grid
- WWV: sync by ear to UTC tick marks before leaving cell coverage

**Computer interface:**

The (tr)uSDX hi-band has USB audio built in (firmware-dependent). Single USB cable carries: audio in/out + CAT control + power. No separate audio cables needed. Tablet running WSJT-X Android connects via USB-C OTG cable.

| Device | Interface | Notes |
|---|---|---|
| (tr)uSDX | USB audio + CAT | Single cable via USB-OTG; firmware must support |
| Lenovo Tab 8 | USB-C OTG host | WSJT-X Android or Xastir |
| Amazon Fire HD 10 | USB-C OTG | ADB sideload required for WSJT-X |
| Laptop | USB | Direct connection, full WSJT-X |

**Field setup sequence:**
1. Set computer clock (NTP or GPS sync before departure)
2. Connect rig via USB-OTG
3. Open WSJT-X → set call, grid, power
4. Tune antenna to <2:1 SWR on target band
5. Set TX power to 2–3W
6. Enable auto-sequencing — WSJT-X handles the exchange
7. Monitor: watch for decodes, answer CQ or call CQ

**WSPR (Weak Signal Propagation Reporter):**

Even simpler than FT8. 2-minute TX/RX cycle. Beacon mode: rig transmits position/power, worldwide network of receivers logs your signal on wspr.rocks. Useful for antenna comparison: park on wspr.rocks, count spots over 1 hour with antenna A vs antenna B.

**FT8 calling frequencies** (from band plan): 7.074, 10.136, 14.074, 18.100, 21.074, 28.074 MHz.

---

## 60m — Field Operation

Five channelized frequencies; USB only; special rules separate from all other amateur bands.

**Channels (US allocation):**

| Channel | Frequency | Notes |
|---|---|---|
| 1 | 5.3305 MHz | Emergency/FEMA/NWS coordination |
| 2 | 5.3465 MHz | General |
| 3 | 5.3570 MHz | SHARES network; FT8 calling |
| 4 | 5.3715 MHz | General; most active for ragchew |
| 5 | 5.4035 MHz | General |

**Rules:**
- USB phone only (no LSB, no FM)
- CW, RTTY, data permitted within 2.8 kHz channel bandwidth (2017 FCC update)
- 100W PEP ERP max — regardless of license class
- Secondary allocation; primary users (federal agencies) have priority
- If federal user is on channel: QSY immediately, no argument

**FT8 on 60m:** 5.357.0 MHz dial; FT8 audio tone ~1000 Hz → RF at 5357.0 + 0.001 = ~5.358 MHz; within channel 3 bandwidth. Works; moderately active.

**Antenna for 60m:**

Half-wave resonant EFHW at 5.371 MHz = 27.84m wire (channel 4). The linked EFHW 60m link is at 27.50m — close to channel 3/4; adjust and verify with NanoVNA.

60m counterpoise: λ/4 at 5.371 MHz = 13.96m (tap position in linked counterpoise). Disconnect counterpoise at 13.96m tap and fold back 80m extension.

**Propagation:** Fills the day/night gap between 40m and 80m. During daytime when 40m is long and 80m has D-layer absorption, 60m covers 300–1500 km reliably. Useful for Merced → Sacramento, LA basin, Bay Area midday when 40m is unreliable.

**Key note:** Cannot use a single-resonance dipole or random wire + auto-tuner to cover all 5 channels; the tuner can compensate SWR but cannot change the antenna's resonant frequency across 73 kHz of spread. Best practice: EFHW cut for one channel, accept slight SWR on adjacent channels and use ATU-100/Z-match for correction.

---

## Inverted-L Antenna

A vertical wire that bends horizontal at height — effectively a top-loaded vertical. Combines vertical aperture for low-angle radiation with a horizontal capacity hat for increased electrical length without full height.

**Why use it:** 80m quarter-wave vertical requires 20m (65 ft) of height. Most portable masts top out at 8–12m. An inverted-L bends the remainder horizontal, achieving electrical resonance with 8–12m actual vertical height.

**Geometry:**

```
           ←——— horizontal section ———→
           |
           | vertical
           | section
           |
    [feedpoint + radials]
```

**Dimensions for DX Commander mast (8.4m vertical):**

| Band | λ/4 total | Vertical | Horizontal needed | Impedance |
|---|---|---|---|---|
| 80m | 20.5m | 8.4m | 12.1m | ~35–65Ω |
| 40m | 10.4m | 8.4m | 2.0m | ~50–70Ω |
| 60m | 13.9m | 8.4m | 5.5m | ~40–60Ω |

At 8.4m vertical, the 80m inverted-L is nearly a full quarter-wave electrically and matches directly to 50Ω coax with 2–4 radials.

**Feed:**

Base coax feed (SO-239 at ground level or on the mast base). Two to four radials (wire on ground, any direction). For the DX Commander mast: attach vertical wire to mast tip, bend horizontally at the mast top and run toward house or fence. Single coax run from Box 2 provision.

**Radiation pattern:**

Mostly vertical polarization (good for DX); the horizontal section adds a slight tilt toward its direction. Not as directional as a sloper. Low-angle radiation good on 80m.

**Vs sloper:** Sloper has wire running diagonally; inverted-L has distinct vertical + horizontal sections. Inverted-L generally better for 80m DX; sloper easier to install from one point. Both feed from the bottom.

**Construction:**
1. Attach wire to mast tip (use existing DX Commander mast tie-off point)
2. Run wire down to feedpoint at Box 2 base
3. Bend and run remaining horizontal wire along fence, toward house, or stake out
4. Install 2–4 radials at feedpoint (any direction, any length >10m)
5. Connect coax: center conductor to antenna wire, shield to radials
6. Verify resonance with NanoVNA; trim horizontal section to tune

---

## Portable Magnetic Loop

A small transmitting loop (STL) is the right antenna when wire antennas cannot be deployed: concrete parks, urban areas, wheelchair-fixed positions. Very different operating behavior from wire antennas.

**Physics:**

Loop circumference ~0.1λ → electrically small. High Q (100–1000) → narrow bandwidth (50–500 Hz on 40m at QRP). Must retune for every frequency change. Efficiency 70–90% for well-built loop at resonance; drops off very rapidly off resonance.

**Critical: bandwidth is Hz, not kHz.** Moving 10 kHz on 40m requires retuning. An auto-tuner cannot follow this — loop tuning is done by a variable capacitor on the loop itself, not an external ATU.

**Diameter vs frequency:**

| Band | Diameter for portable use | Notes |
|---|---|---|
| 40m | 1.5–2.0m | Largest practical backpack size |
| 20m | 0.8–1.2m | Comfortable portable size |
| 15m/10m | 0.5–0.8m | Compact; very efficient |

**Radiation pattern:** Figure-8 broadside to loop plane. Null toward the loop face. Rotate for directional use or NVIS (lay horizontal for max high-angle).

**NVIS with magnetic loop:** Horizontal orientation gives high-angle radiation naturally. On 40m/80m at low height, outperforms a vertical for regional NVIS use.

**Commercial options:**

| Unit | Bands | Max power | Notes |
|---|---|---|---|
| Alpha Loop | 40m–10m | 25W | Good build quality, compact |
| Chameleon MPAS Lite | 40m–10m | 100W | Modular, versatile |
| Alex Loop | 40m–10m | 25W | Lightweight |
| MLA-30+ | RX only | — | Wideband receive; no TX |

**Pool Noodle Magnetic Loop (PN project):** `/claude/PN/` — DIY, 160m–10m + VHF quads; vacuum/HV caps required for TX; separate project from this section.

**High voltage at capacitor:** At 5W into a resonant 40m loop, voltage across the capacitor can reach 500–2000V peak. Commercial vacuum or butterfly caps required. DIY variable caps (air gap, pipe cap) work at QRP if carefully gapped. Do not use a standard poly tuning cap.

**Field kit addition:** A portable commercial loop (Alpha Loop or similar) fills the "no trees / no space" scenario in the antenna selection table. Weight: 0.5–1.5 kg, fits in a small bag.

---

## Mobile / Vehicle Operation

A vehicle roof provides an excellent antenna ground plane — typically better than any portable radial system. POTA park-and-operate is one of the fastest activation setups.

**Vehicle body as counterpoise:**

Car roof: ~2–4 m² metal; electrically, ~6–8m effective ground plane at HF. Outperforms 4 staked radials in most cases. Keeps RF off the feedline.

**Mounting options:**

| Mount | Installation | Loss | Notes |
|---|---|---|---|
| Mag mount | None (magnetic hold) | Moderate (contact resistance) | Fast deploy; can slide; clean roof needed |
| NMO mount | 3/4" roof hole, permanent | Minimal | Best; weatherproof; SO-239 interior |
| Trunk lip / hatch clip | No hole | High (poor contact) | Temporary; significant loss |
| Screwdriver antenna | NMO or 1" ball mount | Low | Remote-tuned coil; covers 40m–10m |

For POTA: mag mount on center of roof, dual-band 2m/70cm whip for local/ARES, or HF screwdriver for all-band coverage.

**RF in the cab:**

RF flows back on the outside of the coax shield (common mode), enters the cab, causes RF burns, audio hum, rig lockup. Prevent with:
- Ferrite choke (Mix 31 snap-ons × 4) at antenna base before entering cab
- Short coax run inside cab (< 1m)
- Bond cab ground points with braided strap if choke alone insufficient

**Park-and-operate setup (POTA):**

1. Park facing target bearing (helps if using a directional antenna)
2. Turn off engine (eliminates alternator noise)
3. Attach mag mount to roof center
4. Route coax through window seal or door jam (foam protects coax)
5. Connect rig to car 12V (cigarette lighter with Powerpole adapter, or direct to battery)
6. Confirm SWR with NanoVNA or rig SWR meter
7. Log on tablet via Bluetooth or paper

**Noise from vehicle:**
- Alternator: S3–5 whine on HF, follows engine RPM; mostly eliminated with engine off
- Switching regulators (engine computer, accessories): may persist after engine off; ferrite beads on rig power leads
- Ignition noise: strongest on AM BCB, less on HF above 7 MHz

**HF from a vehicle:**

A loaded quarter-wave whip on the roof + vehicle body ground plane is a clean, low-noise antenna. A 5.6m telescoping whip mounted on an NMO base covers 10m–20m bare and 40m with center loading coil — same kit as field, different mounting.

---

## Portable Yagi — Tape Measure 2m

A 3-element Yagi from measuring tape and PVC pipe. Folds flat, weighs < 300g, provides 6–7 dBd gain for satellite, fox hunting, and VHF DX.

**Element dimensions (144 MHz / 2m):**

| Element | Length | Notes |
|---|---|---|
| Reflector | 1041 mm | |
| Driven element | 991 mm total | Split: 2× 495.5 mm |
| Director | 946 mm | |

**Spacing:**

| | Distance from reflector |
|---|---|
| Driven element | 330 mm |
| Director | 495 mm (165 mm beyond DE) |

**Feed:**

Feedpoint impedance ~28Ω. Use a hairpin match: fold back ~75 mm on each driven element half; bridge with a 150 mm piece of RG-174 coax (inner conductor bridging); connect coax center to one side, shield to other. This brings impedance near 50Ω. Check with NanoVNA.

Alternatively: 4:1 balun (wound on BNC barrel, 4 turns binocular core).

**Boom:** 1/2" PVC conduit, 500 mm total. Drill holes for 1/4" bolts; clamp elements with hose clamps or through-hole + wing nut. Folds: bend measuring tape elements at 100 mm from each end → pack flat.

**Gain:** ~7 dBi (5 dBd) — beats J-pole by 2–3 S-units; 2× a Nagoya whip.

**Uses in field kit:**
- 2m fox hunting / ARDF
- FM satellite (AO-91, AO-92, SO-50): manual point at satellite; 10-min pass window; rotate by feel as sat moves
- POTA VHF credit (park/SOTA VHF activations count)
- ARES/RACES: simplex to a distant node when repeater is down

**Manual satellite tracking:** Point beam at satellite's orbital path in the sky. At AOS (acquisition of signal) point low toward the horizon at the satellite's rise azimuth. Track visually or with a printed pass chart. No motorized mount needed for 2m FM LEO satellites.

**Azimuth calculation for Merced:** Most LEO passes for NOAA-15/SO-50 type satellites are visible from NW to SE or NE to SW; check heavens-above.com for pass table before leaving cell coverage.

---

## Field Antenna A/B Comparison

How to evaluate two antennas at a field site without transmitting or involving another station.

**Receive comparison (preferred):**

Reciprocity: receive performance = transmit performance. A 3 dB receive improvement = 3 dB transmit improvement. No TX needed.

1. Find an active station on the target band (CW signal, beacon, digital mode signal, broadcast)
2. Note S-meter reading with antenna A for 30 seconds — record peak and average
3. Swap to antenna B — do this in < 2 minutes (ionosphere changes slowly; propagation drift errors accumulate over longer swap times)
4. Note S-meter reading for 30 seconds
5. Difference in S-units = antenna delta

**S-unit reference:**

| Difference | Power ratio | What you hear |
|---|---|---|
| 1 S-unit | 4× (6 dB) | Clear improvement |
| 0.5 S-unit | 2× (3 dB) | Marginal, borderline perceptible |
| 2 S-units | 16× (12 dB) | Dramatic — significant antenna difference |

**Noise floor comparison:**

On a quiet band (no signals), the better antenna has the higher noise floor. More antenna = more thermal/galactic noise = higher S-meter floor. If antenna B shows S2 noise and antenna A shows S1 noise, antenna B is receiving better.

**Pitfalls:**

- Swap time > 2 minutes: propagation can shift by 2+ S-units in 5 minutes on HF; always swap fast
- Different bands: resonant vs non-resonant comparison is meaningless; always same band
- Different heights: height changes pattern, not just gain; keep the test controlled
- SWR difference: the rig sees different impedances; use ATU-100 (stores memory) or Z-match on both to ensure rig is matched both ways

**WSPR antenna comparison:**

Put rig on WSPR beacon mode (very low power, usually 200–500 mW) and log spots on wspr.rocks for 1 hour per antenna. More spots + more distant spots = better antenna. This is the most rigorous field comparison method available at QRP.

**POTA naturalist log:** Record antenna A/B S-meter readings, time, band, reference station callsign in the naturalist log. Over many activations builds a real antenna database.

---

## Coax Velocity Factor Measurement

Unknown coax salvage or verifying manufacturer spec — useful when building quarter-wave stubs or tuned feedlines.

**NanoVNA method:**

1. Cut coax to a known length (1.0m is convenient)
2. Attach one end to NanoVNA CH0; leave far end OPEN
3. Set frequency range 1–500 MHz (or narrower for long cables)
4. On Smith chart, look for the frequency where the open stub transitions from capacitive arc to the far right (open-circuit side)
5. Find first minimum-SWR point or the first frequency where the impedance is purely resistive (on the Smith chart centerline, right side = anti-resonance for open stub)

**Calculation:**

```
VF = (f_resonant × 4 × L) / c

f in MHz, L in meters, c = 299.8 m/µs

Example: 1.0m coax, first resonance at 168 MHz
VF = (168 × 4 × 1.0) / 299.8 = 672 / 299.8 = 0.82  → LMR-400 or RG-6
```

**Alternate: shorted stub method:**

Short the far end. First resonance = quarter wave (low-impedance point on Smith chart). Same formula applies.

**Reference values for comparison:**

| Cable | VF |
|---|---|
| RG-174 | 0.66 |
| RG-58A | 0.66 |
| RG-8X | 0.82 |
| LMR-400 | 0.85 |
| 450Ω window line | 0.90–0.95 |
| Open wire (bare) | 0.97–0.99 |

**Making quarter-wave stubs:**

```
L (m) = VF × 75 / f_MHz

Example: quarter-wave stub at 14.2 MHz, RG-58 (VF=0.66):
L = 0.66 × 75 / 14.2 = 49.5 / 14.2 = 3.49m
```

Quarter-wave open stub = high impedance at f → bandpass notch for harmonics. Quarter-wave shorted stub = low impedance at f → trap.

---

## RF Exposure at QRP

At 5W, RF safety is not a practical concern for field operation. Knowing why lets you operate confidently and answer questions on the air.

**FCC categorical exemptions (Part 97.13):**

Stations below these power levels are categorically exempt — no evaluation required:

| Band | Exempt below |
|---|---|
| 40m (7 MHz) | 200W PEP |
| 20m (14 MHz) | 150W PEP |
| 15m (21 MHz) | 100W PEP |
| 2m (144 MHz) | 35W PEP |
| 70cm (440 MHz) | 25W PEP |

At 5W QRP: categorically exempt on all bands. At 60W (T-60): still exempt at all HF frequencies.

**What the limits actually mean:**

MPE (Maximum Permissible Exposure):
- Controlled (operator and family): 1–5 mW/cm² depending on band
- Uncontrolled (public): 0.2–1 mW/cm²

At 5W into a dipole at 10m height: power density at ground = < 0.01 mW/cm². Threshold exceeded only at < 0.5m from the antenna while transmitting.

**Practical rules:**
- QRP (5W): safe at any distance > 0.3m from antenna during TX
- Keep hands off antenna during TX — not because of RF safety but because body detunes the antenna
- 60W home station (T-60): safe at 1m+ from antenna; no concern for typical operating positions
- Higher-gain antennas (Yagi) increase exposure in the beam direction at close range; for field portable this is not an issue

**The one real concern at QRP:** RF burns from touching a high-voltage feedpoint (EFHW tip, loop capacitor). This is electric field hazard, not power density. Keep hands off the wire and cap during TX — not because of MPE but because ~224V at 20W (~112V at 5W) into an EFHW tip will feel like touching a live circuit.


---

## Amateur Extra — Complete Band Plan

Mode allocations and Extra-only segments. Extra-only segments marked *.
No Extra-only segments above 10m — Technician+ has full VHF/UHF privileges.

*Full reference: extra_band_plan.md (both repos)*

### 2200m — 135.7–137.8 kHz (secondary)

1W EIRP max. Notify ARRL WD2XSH coordinator before first operation. All modes; CW and WSPR most practical (2.1 kHz bandwidth). Antenna loss counts against EIRP limit. WSPR most active ~137.5 kHz.

### 630m — 472–479 kHz (secondary)

5W EIRP max (1W near UTC-coordinated Navy/NDB stations). Must notify UTC coordinator before operating. All modes: CW, phone, RTTY/data. WSPR most active at 472.200 kHz. Loaded vertical antenna required; high Q critical.

### 160m — 1.800–2.000 MHz

| Segment | Modes |
|---|---|
| 1.800–1.900 | CW, RTTY/data |
| 1.838–2.000 | Phone (USB), image |

### 80m — 3.500–4.000 MHz

| Segment | Modes | Note |
|---|---|---|
| 3.500–3.525 | CW | Extra only* |
| 3.525–3.600 | CW | General+ |
| 3.570–3.600 | RTTY/data | within CW segment |
| 3.600–3.700 | Phone LSB | Extra only* |
| 3.700–4.000 | Phone LSB, image | General+ |

### 60m — 5 channels (channelized)

5.3305 / 5.3465 / 5.3570 / 5.3715 / 5.4035 MHz — USB only, 100W PEP ERP max. No CW, no RTTY, no data, no FM.

### 40m — 7.000–7.300 MHz

| Segment | Modes | Note |
|---|---|---|
| 7.000–7.025 | CW | Extra only* |
| 7.025–7.125 | CW, RTTY/data | General+ |
| 7.125–7.175 | Phone LSB | Extra only* |
| 7.175–7.300 | Phone LSB, image | General+ |

### 30m — 10.100–10.150 MHz (WARC — no phone, no Extra-only segments)

| Segment | Modes |
|---|---|
| 10.100–10.150 | CW |
| 10.130–10.150 | RTTY/data |

### 20m — 14.000–14.350 MHz

| Segment | Modes | Note |
|---|---|---|
| 14.000–14.025 | CW | Extra only* |
| 14.025–14.150 | CW | General+ |
| 14.070–14.095 | RTTY/data | within CW segment |
| 14.150–14.175 | Phone USB | Extra only* |
| 14.175–14.350 | Phone USB, image | General+ |

### 17m — 18.068–18.168 MHz (WARC — no Extra-only segments)

| Segment | Modes |
|---|---|
| 18.068–18.110 | CW |
| 18.100–18.110 | RTTY/data |
| 18.110–18.168 | Phone USB, image |

### 15m — 21.000–21.450 MHz

| Segment | Modes | Note |
|---|---|---|
| 21.000–21.025 | CW | Extra only* |
| 21.025–21.200 | CW | General+ |
| 21.070–21.110 | RTTY/data | within CW segment |
| 21.200–21.225 | Phone USB | Extra only* |
| 21.225–21.450 | Phone USB, image | General+ |

### 12m — 24.890–24.990 MHz (WARC — no Extra-only segments)

| Segment | Modes |
|---|---|
| 24.890–24.930 | CW |
| 24.920–24.930 | RTTY/data |
| 24.930–24.990 | Phone USB, image |

### 10m — 28.000–29.700 MHz

| Segment | Modes |
|---|---|
| 28.000–28.300 | CW |
| 28.050–28.150 | RTTY/data |
| 28.300–29.700 | Phone USB, image |
| 29.300–29.510 | Satellite downlinks |
| 29.520–29.590 | FM repeater inputs |
| 29.600 | FM simplex calling |
| 29.610–29.700 | FM repeater outputs |

### 6m — 50.0–54.0 MHz (Technician+ — no Extra-only segments)

| Segment | Modes | Note |
|---|---|---|
| 50.0–50.1 | CW only | beacon subband |
| 50.1–50.3 | CW, SSB (USB) | DX weak signal window |
| 50.3–54.0 | All modes | FM, repeaters, digital |
| 52.525 | FM simplex calling | primary 6m FM |

Sporadic-E season: May–July primary, Dec–Jan secondary. Watch 50.313 (FT8) and 50.125 (SSB) for openings.

### 2m — 144.0–148.0 MHz (Technician+ — no Extra-only segments)

| Segment | Modes | Note |
|---|---|---|
| 144.0–144.1 | CW only | |
| 144.1–144.275 | CW, SSB (USB) | weak signal corridor |
| 144.200 | SSB/CW national calling | |
| 144.390 | APRS simplex | H4M passive beacon |
| 145.800–146.0 | Satellite | |
| 146.0–148.0 | FM, repeaters | |
| 146.520 | FM simplex calling | national 2m FM |

Repeater offset: +600 kHz above 147.0, −600 kHz below 147.0.

### 1.25m / 222 MHz — 222.0–225.0 MHz (Technician+ — no Extra-only segments)

| Segment | Modes |
|---|---|
| 222.0–222.15 | Weak signal, CW, SSB |
| 222.100 | SSB/CW national calling |
| 223.500 | FM simplex calling |
| 224.0–225.0 | Repeaters (−1.6 MHz offset) |

### 70cm — 420–450 MHz (Technician+ — no Extra-only segments)

| Segment | Modes | Note |
|---|---|---|
| 420–432 | CW, SSB, ATV | some regional restrictions |
| 432.100 | SSB/CW national calling | |
| 435–438 | Satellite | uplink/downlink |
| 440–445 | FM repeater inputs | +5 MHz offset |
| 445–450 | FM repeater outputs | |
| 446.000 | FM simplex calling | national 70cm FM |

### 33cm — 902–928 MHz (secondary — shared with Part 15)

902.100 SSB/CW calling; 906.000 FM simplex; 927.500 FM repeater outputs. Lightly used; Part 15 spread-spectrum devices coexist.

### 23cm — 1240–1300 MHz (secondary)

| Segment | Modes |
|---|---|
| 1240–1246 | ATV |
| 1270–1295 | Repeaters |
| 1296.100 | SSB/CW national calling |
| 1294.500 | FM simplex calling |
| 1296.4–1296.6 | EME |

### Extra-Only Segments Summary

HF only — no Extra-only segments above 10m:

| Band | CW Extra-only | Phone Extra-only |
|---|---|---|
| 80m | 3.500–3.525 | 3.600–3.700 |
| 40m | 7.000–7.025 | 7.125–7.175 |
| 20m | 14.000–14.025 | 14.150–14.175 |
| 15m | 21.000–21.025 | 21.200–21.225 |
| 160/30/17/12/10m | none | none |
| 6m–23cm | none | none |

Extra-only CW corners are quieter and preferred for DX.

### Key Rules

- CW permitted throughout all HF bands — CW-only segments exclude all other modes from that segment
- LSB below 10 MHz, USB at 10 MHz and above
- RTTY/data segments: FT8, PSK31, JS8Call, WSPR — segment edges are gentlemen's agreements
- WARC bands (30m/17m/12m): no Extra-only segments
- 60m: USB only, 5 channels, 100W ERP max — no exceptions
- 2200m/630m: EIRP caps (1W/5W), coordinator notification required before first use
- VHF/UHF: Extra adds no privileges above 10m

### Calling Frequencies Reference

| Band | FT8 | SSB calling | FM calling | QRP CW |
|---|---|---|---|---|
| 2200m | — | — | — | ~0.1375 |
| 630m | — | — | — | ~0.4752 (WSPR: 0.4722) |
| 160m | 1.840 | 1.845 USB | — | 1.810 |
| 80m | 3.573 | 3.750 LSB | — | 3.560 |
| 40m | 7.074 | 7.200 LSB | — | 7.030 |
| 30m | 10.136 | — | — | 10.106 |
| 20m | 14.074 | 14.225 USB | — | 14.057 |
| 17m | 18.100 | 18.130 USB | — | 18.096 |
| 15m | 21.074 | 21.300 USB | — | 21.060 |
| 12m | 24.915 | 24.940 USB | — | 24.906 |
| 10m | 28.074 | 28.400 USB | 29.600 | 28.060 |
| 6m | 50.313 | 50.125 USB | 52.525 | 50.090 |
| 2m | 144.174 | 144.200 USB | 146.520 | 144.050 |
| 1.25m | — | 222.100 USB | 223.500 | — |
| 70cm | 432.174 | 432.100 USB | 446.000 | 432.010 |
| 23cm | 1296.174 | 1296.100 USB | 1294.500 | — |

---

## Field Antenna Quick Reference

| Technique / Item | Best Use | Notes |
|---|---|---|
| 5.6m telescoping whip | HF 10m–80m field vertical | Primary field antenna; shorten from top only; 20m bare, 40m center load at 2.8m (22–33 µH), 80m center load at 2.8m (80–130 µH); feedpoint Z 15–35Ω normal; tune with NanoVNA |
| NanoVNA — field tuning | Antenna resonance measurement | SOLT cal at feedpoint end; SWR + Smith chart traces; dot left of centerline = inductive (shorten/remove turns), right = capacitive (lengthen/add turns), on centerline = resonance |
| TinySA Ultra — field use | RF signal measurement | Spectrum analyzer 100 kHz–5.3 GHz; measures dBm, harmonics, spurious; check (tr)uSDX harmonics via dummy load + 30 dB attenuator; FCC limit: harmonics ≥43 dB below fundamental at 5W; SMA female both ports |
| H4M PortaPack — field use | SDR RX/TX, band survey, capture | HackRF-based; 1 MHz–6 GHz; Mayhem firmware; TX 10–30 mW max — resonance critical at this power; use 5.6m whip via M10→SMA adapter for HF RX, ANT500 for VHF/UHF; for real HF contacts use (tr)uSDX; menu reference: portapack_h4m_menu.md; field quick ref: portapack_h4m_ham_quickref.md |
| ATS-25 Max — receive antennas | SWL/SW monitoring, RX-only | ANT jack is BNC (twist-lock, shared with stock whip); no matching/counterpoise needed (RX front end tolerates mismatch); long-wire via BNC-to-binding-post adapter is the fastest SWL upgrade; YouLoop passive magnetic loop best in RF-noisy environments (nulls local noise/interference by rotation); MW uses internal ferrite bar, no external antenna needed; reuse (tr)uSDX BNC parts bin before buying new hardware |
| Carpet loop — indoor SWL | Stealth install, no outdoor wire possible | Room-perimeter loop (~9m/25-30ft cable) under carpet + L-network tuner (365pF cap, band/longwire/ground switch); electrically small (true loop behavior) only at low SW (~3 MHz, ~0.09λ), wire-like by 10m SW (~30 MHz, ~0.9λ); no rotation/nulling once installed — unlike YouLoop; noise rejection mainly from common-mode balance, not H-field selectivity; durable underfoot |
| YouLoop — passive magnetic loop build | RX noise rejection with nulling, HF weak-signal | Two ~1-1.5m coax loop halves; crossover junction (center↔shield swap) at one end, BN-73-302 binocular ferrite balun (4T + 4T magnet wire) at the other, BNC feedline (<10m); 10 kHz-30 MHz no tuning; rotate to null interference; stays electrically small across HF unlike room-perimeter loops; feeds both TinySA LOW port and ATS-25 Max |
| Feedpoint impedance reference | Matching chain selection | EFHW feedpoint ~2450Ω → 49:1 unun required; half-wave dipole ~73Ω → direct 50Ω coax; quarter-wave vertical ~36Ω theory, 15–35Ω over real ground → Z-match or ATU; random wire 150–3000Ω varies → Z-match preferred; impedance determines which transformer is needed before the feedline |
| Linked EFHW + 49:1 unun | HF 80m–6m, no tuner | Most efficient QRP HF option (~85%); no ground required; half-wave resonant with link connectors at each band length; K6ARK 2.5mm banana plug links; 1–2m counterpoise from unun chassis; K6ARK EFHW Matching Unit Kit in use; 10-band layout: 6m=2.95m, 10m=5.29m, 12m=5.95m, 15m=7.05m, 17m=8.20m, 20m=10.40m, 30m=14.60m, 40m=20.70m, 60m=27.50m, 80m end=40.50m |
| Band change — linked EFHW | Field band-change procedure | Disconnect tap connector at operating band; fold/coil excess wire behind that tap; counterpoise: disconnect at matching tap; no re-tune needed — antenna is resonant at each tap position; only SWR change is from folded wire capacitance if excess trails on ground — coil it up |
| Multiband slider loading coil | 80m–30m on 5.6m whip | Slider moves along winding — more turns = lower band, fewer = higher; covers 80m/60m/40m/30m; mark band positions on form after first field session; DeOxit slider contact periodically |
| Loading coil storage | Protect coil in field | Neoprene lens pouch — 100–135mm prime or 70–200mm zoom size for slider coil; leave slack in leads; label pouch |
| Common mode choke | Suppress coax shield RF | Mix 31 ferrite; >1000Ω target; 4× snap-on clamps at feedpoint (instant) or FT-140-31 wound 7T RG-174 (more effective); needed when SWR shifts on touching coax or RF in rig audio |
| Balun vs choke vs unun | Which transformer where | Dipole center: 1:1 choke balun (balanced feedpoint + common-mode suppression); EFHW: 49:1 unun at feedpoint + coax choke behind it (unun ≠ balun — it's single-ended); vertical: coax choke at base feedpoint only; random wire: no balun — Z-match handles impedance; voltage balun not needed for QRP field ops |
| Z-match tuner | Field antenna matching | Passive, instant, no power; two-cap tune (30 sec practiced); harmonic suppression bonus; handles 50Ω–several kΩ; preferred over ATU-100 for field; pre-characterize feedpoint Z at home |
| ATU-100 | Backup/shack tuner | Auto L-network; stores memories; 2–5 sec tune; handles 10:1 SWR; use after antenna is resonant, not instead of tuning |
| Z-match vs ATU-100 | Which tuner for which situation | Z-match: passive/no power/instant/wider impedance range/harmonic suppression — field first choice; ATU-100: auto/power-required/memory/handles 10:1 — backup or when Z-match can't match; neither replaces antenna tuning: resonant antenna first, tuner corrects residual mismatch only |
| 49:1 unun | EFHW impedance transform | Steps ~2450Ω feedpoint to ~50Ω; FT-140-43 QRP core; 2T primary / 14T secondary; K6ARK EFHW Matching Unit Kit in use; short counterpoise from chassis ground lug |
| Linked counterpoise | Ground system for whip | Color-coded alligator clip break points at each band's λ/4; coil in pouch; deploy and clip one-handed; staked radials for fixed position |
| Counterpoise deployment | One-handed field setup | Clip to feedpoint ground lug from chair/standing height; lay wire along ground; push stake with foot; color code: 40m=full wire, 20m=red, 17m=orange, 15m=yellow, 12m=green, 10m=blue |
| Ground surface — counterpoise | Radial effectiveness | Grass/soil=best; gravel=good; concrete/asphalt=poor (use longer elevated counterpoise instead); frame elevated above pavement beats wire on concrete |
| Dipole height effects | Radiation angle | <15 ft = NVIS high-angle (regional 40m/80m); 25–35 ft = general purpose; >50 ft = low-angle DX; wet trees detune nearby elements — move legs away |
| Ground: pattern vs loss | Two separate effects | Height in λ shapes far-field pattern (verticals stay low-angle at any height; horizontals need ≥0.5λ for DX, <0.25λ = NVIS); soil σ sets near-field loss (verticals lose power to poor ground at the feedpoint, horizontals barely affected since current max is aloft) — a site can be good for one and bad for the other |
| Kite messenger technique | Precision line placement | Messenger rides angled balloon string to height, releases drop line over branch; needs breeze to angle string; simpler: guide balloon over branch with horizontal pole |
| 49:1 unun — DIY | Wind your own | FT-140-43 core; 2T primary / 14T secondary; 26–28 AWG enameled wire; SO-239 coax port + wire terminal + ground lug for counterpoise; weatherproof enclosure |
| Arborist throw bag | Line into tall trees | 8–12 oz Sherrill bag + 1.5–2mm braided throw line; seats on branch, retrieves clean; better than monofilament above 40 ft or in wind |
| Tapped dipole deployment | Hang dipole in field | Inverted-V (POTA, omnidirectional); flat top (gain broadside); sloper (easy solo deploy); height: <15 ft=NVIS, 25–35 ft=general, >50 ft=DX; wet trees detune — move legs away from obstruction |
| Line into tree — pocket rocket / slingshot | Hang wire antennas, HF | Legal CA; 1/2 oz weight + monofilament pilot line |
| Line into tree — balloon | Calm days only | Mylar > latex; kite messenger for precision placement |
| Cat5/6 wire as antenna | Random wire, dipole, NVIS | 24 AWG, QRP only; 50 ft patch cable in jacket pocket |
| Cat5/6 twisted pair as feedline | HF feedline to Z-match | ~100–120Ω, VF 0.65; Z-match handles impedance directly |
| RJ-45 junction box | Feedpoint connector | Keyed, locking, one-handed; weatherproof versions available |
| Cat5/6 + RJ-45 complete system | Multiband fan dipole | Entire antenna is one patch cable; under 5 min one-handed |
| Tapped dipole (Ham Jazz RAS #17) | 5-band HF, no tuner | Single wire per leg, banana clip taps; 43¢, 5-second band change |
| Fishing reel band selector (Ham Jazz RAS Ep 4) | 10m–80m, no tuner | Unreel to resonant length; bottle cage boss screw mount |
| Balloon antenna | HF vertical, inverted-L | Fair-weather only; fishing reel for height control |
| Wheelchair antenna — fishing pole | HF fixed position | RAM mount, linked EFHW, ~85% efficiency, 5–10 min |
| Wheelchair antenna — rolling whip | Mobile HF | 1.5m loaded whip, ATU-100, 20–35% efficiency, no setup |
| Wheelchair antenna — frame as antenna | 10m–80m no tuner | Wire taps on frame upright, banana clips, frame = counterpoise |
| Walker antenna — tapped frame | 10m–80m no tuner | Frame upright + wire taps; tennis ball feet = RF isolation |
| Walker antenna — fishing pole | HF portable | RAM mount or hose clamp, linked EFHW, ~85% efficiency |
| Linked counterpoise — 10-band (80m–6m) | Ground system for 5.6m whip | Single 20.53m wire; K6ARK 2.5mm banana plug taps at each λ/4 from feedpoint (6m=1.49m, 10m=2.63m, 12m=3.01m, 15m=3.53m, 17m=4.14m, 20m=5.29m, 30m=7.40m, 40m=10.48m, 60m=13.96m, 80m=20.53m); disconnect at band tap and fold back excess |
| Trapped counterpoise — 9-band | Deploy once, automatic band coverage | 8 parallel LC traps (T37-2 for 6m–30m, T50-2 for 40m); AL = 4.0 nH/T² (T37-2), 5.7 nH/T² (T50-2); NP0 caps only; 39 pF for 6m–20m, 56 pF for 30m, 100 pF for 40m; tune high-to-low band order |
| Trapped EFHW — multiband | HF multiband without link connectors | Auto band coverage; build time 4–8 hrs vs 1–2 hrs linked; configs: 3-band 20/30/40m ~60–62 ft (K6ARK proven); 4-band 15/20/30/40m ~56–59 ft; 5-band 15/17/20/30/40m ~56–59 ft (17m section ~0.9m — label both ends before soldering); 10m→12m too short for traps — skip; start each section at free-space length, trim to resonance; OSH Park QRP Antenna Trap PCB fits T37/T50 |
| OSH Park QRP Antenna Trap PCB | Trap substrate for EFHW/counterpoise traps | Purpose-designed board, fits through T37/T50 hole, SMD cap footprint, wire pass-through holes; order 3 minimum (OSH Park batch); search "QRP Antenna Trap" in OSH Park shared projects |
| 2.5mm banana plug links (K6ARK) | EFHW and dipole band links | 2.5mm male + female + adhesive heat shrink + CA glue; female parks in heat shrink sleeve at link point when open; one-handed band change; lighter than 4mm lab banana plugs |
| 30 AWG PTFE wire — field antennas | EFHW, dipoles, counterpoise ≤20W | 0.34Ω/ft; handles 0.63A (20W/50Ω); 600V covers EFHW tip voltage (~224V peak at 20W); PTFE outlasts PVC outdoors; slippery — use bight knots or crimp at attachment points; avoid on loaded short antennas where Rrad < 10Ω |
| VHF/UHF handheld — UV-5R Mini | Local 2m/70cm voice | ~2W, SMA female, Bluetooth app programming; Nagoya NA-771 SMA whip improves range over stock duck; 3rd harmonic of 146 MHz = 438 MHz (in 70cm band) — run purity test; +33 dBm high power, use 30 dB attenuator |
| VHF/UHF handheld — UV-5R | Local 2m/70cm voice | 4–8W, SMA female, CHIRP programmable; more range than Mini; 3rd harmonic of 146 MHz = 438 MHz (in 70cm band) — run purity test before use |
| RF connector adapter bag | Field connector kit | Makeup bag + clear zipper pouches; sort by family; foam lining protects SMA pins |
| Rite in the Rain #8511 | Field log sheets | Laser-printed antenna templates, site log cards; survives soaking; pencil/ballpoint/Sharpie only — no rollerball, gel, or fountain pen; not inkjet |
| Thermal laminator (home prep) | Laminated field cards | Fellowes Neptune 3 125 (12.5", 10 mil rated, hot/cold) or Apache AL13P (~$35, 3–10 mil); 5 mil min for field cards, 7 mil for stiff card-like stock; grease pencil works on any smooth laminate |
| Field card workflow (home prep) | Iterative card design | 1) Print draft on RITR #8511; 2) field test — pencil corrections in conditions; 3) revise and reprint; 4) repeat until right; 5) laminate final at 7 mil for grease pencil reuse — don't laminate until form has been used in the field |
| Field antenna selection | Which antenna for which scenario | Trees >40 ft: EFHW sloper; open grass: 5.6m whip + staked counterpoise; concrete: whip + elevated frame counterpoise; WC fixed: fishing pole EFHW; rolling: loaded whip + ATU; walker: frame vertical; fast deploy: bare whip 10–20m |
| Field dummy load | 50Ω termination for TX testing | QRPGuys Mini SMA (10W, ~15g) recommended; BNC terminator dongle (1–2W) for brief keying only; HN-31 = bench only; harmonic check: dummy load → T-splitter → 30 dB att → TinySA |
| R_ground measurement | Soil conductivity at field sites | 5.6m whip on 40m + 4 staked radials; NanoVNA feedpoint Z at resonance; R_feed = Rrad (36Ω) + R_ground_loss; 36–42Ω = excellent, 55–70Ω = average, >90Ω = poor; log site+season for σ map |
| Center-fed dipole — single band | Simple resonant HF per band | Each leg λ/4: 40m=10.48m, 20m=5.28m, 15m=3.54m, 10m=2.65m; inverted-V apex high; 50Ω direct coax feed; ~90–95% efficiency; no tuner; cut long, trim to resonance |
| Random wire | Multiband HF with tuner | Any non-resonant length; avoid half-wave multiples (5m/10m/20m/40m); use Z-match (wider range); counterpoise = λ/4 on lowest band; retune each band change; 60–80% efficiency |
| Propagation — Merced CA | Band selection by time/conditions | 40m: evenings/overnight regional-DX; 20m: 0800–1800 continental/DX; 15m/12m/10m: daytime solar max excellent; 30m: quiet daytime reliable; 80m: regional NVIS night; solar indices: SFI>150 + A<10 + K<3 = best |
| Gray line window | Best DX on 40m/80m | Sunrise/sunset ±20 min; ionosphere transition; use astrolabe for exact local time; 10–20 min window; 40m opens worldwide |
| QRP CW calling freqs | Where to call from field | 40m: 7.030 (QRP/SKCC); 20m: 14.057; 30m: 10.106; 15m: 21.060; 10m: 28.060; 40m Extra corner 7.000–7.025 quieter |
| 2m J-pole — field VHF | Repeater/ARES fixed position | 155 cm total; ~97 cm radiating element + ~49 cm matching stub + feed gap; 450Ω window line or twin-lead; ~3 dBi; haul up on fishing pole; beats Baofeng rubber duck by ~1 S-unit |
| Nagoya NA-771 upgrade | Baofeng antenna improvement | 15" SMA male dual-band whip; fits UV-5R and UV-5R Mini; noticeable 2m improvement; already in kit |
| Webbing J-pole project | 16-band field J-pole | `/claude/web_j_pole/`; 3" webbing + 2" tape construction; 2m + 70cm + 14 other bands; Navy TM format |
| Sloper antenna — 40m | T-60 home station / solo field deploy | Half-wave ~20.4m (67 ft); high end to fence/mast, low end to ground stake; 20–45° slope; directional toward low end; NE for Europe, SE for South America; 1:1 choke balun at feedpoint; active build at Box 2 provision |
| NVIS deployment | Regional 0–500 km comm on 40m/80m | Horizontal dipole at < 0.15λ height (40m: < 6m, 80m: < 12m); flat-top beats inverted-V for NVIS; 40m NVIS covers LA/SF/Sacramento from Merced; 80m nighttime; random wire at fence height works |
| T/R switching — T-60 + SW-7800 | Same antenna for TX and RX | MFJ-1702C 2-port coax switch; T-60 → PORT 1, SW-7800 → PORT 2, antenna → COMMON; manual lever; zero-beat: tune SW-7800 to target, match T-60 VFO pitch to received tone, switch to TX |
| Feedline coax selection | Field cable trade-offs | Loss per 100 ft: RG-174 (0.6/0.9/1.4 dB at 7/14/28 MHz, 0.5 oz/ft); RG-58A (0.35/0.5/0.75 dB, 1.2 oz/ft); RG-8X (0.2/0.3/0.45 dB, 1.6 oz/ft); RG-174 fine ≤25 ft HF; RG-58 for 25–50 ft; RG-8X for shack runs; at 5W, 1 dB = 20% power — keep runs short |
| SWR effect on (tr)uSDX | When SWR matters | With tuner: rig sees 50Ω regardless of antenna SWR; without tuner: 2:1 = 89% power, 3:1 = 75%, 5:1 triggers fold-back; put ATU-100 physically at rig port; tune antenna first, tuner second |
| Fiber-optic RF link (RFoF) | Noise-floor-critical receive antennas (Beverage, phased-array element) | Galvanic isolation — no ground loop/lightning path/shield current; fiber radiates and picks up nothing; digital RFoF (SDR at feedpoint + fiber Ethernet) is the practical DIY path; analog RFoF (CATV return-path modules, 5–42 MHz) works for HF RX only, not TX power; feedpoint box needs its own battery/solar supply — fiber carries no power; TX needs a PA at the feedpoint fed separately |
| First-transmit site checklist | Pre-key verification at new site | Coax connections tight → NanoVNA SWR < 2:1 at feedpoint → counterpoise deployed → choke at feedpoint → tuner adjusted → frequency clear 60s → sidetone fires → no thunder audible |
| DX azimuths from Merced CA | Pointing directional antennas | Europe: 28° NE; Japan/Korea: 314° WNW; Australia: 237° WSW; Hawaii: 244°; South America: 130° SE; dipole wire runs perpendicular to target bearing |
| Portable mast options | When fishing pole isn't enough | 6–7m fishing pole already in kit ($15, 150g); 10m squid pole (380g, $60) for better 40m EFHW; Jackite 31-ft (700g, $85) for drive-in sites; MFJ-1910 33-ft (450g, $75) semi-permanent; rebar stake as mast base — slip pole base over 3/8" rebar driven 12" into ground, no guy wires needed for 6–7m poles in soft ground |
| Weather effects — rain | Rain impact on antenna performance | Wire sag shifts resonance 1–3% lower — retune; wet ground lowers R_ground (helps); wrap coax connectors with self-amalgamating tape; wire is fine wet, protect the unun/feedpoint |
| Weather effects — wind | Wind impact on antenna performance | Swinging wire degrades CW copy — stake wire ends; push mast base deeper, add one guy line; finger-tighten BNC bayonets after first gust |
| Weather effects — lightning | Lightning safety protocol | Audible thunder: stop TX, disconnect coax at rig; visible lightning: disconnect + take down antenna; 30/30 rule: < 30 sec flash-to-thunder = danger zone, wait 30 min after last thunder; go to car |
| Extra band plan | Which mode goes where | HF: CW bottom, RTTY/data middle, Phone top; Extra-only CW: 3.500–3.525/7.000–7.025/14.000–14.025/21.000–21.025; Extra-only Phone: 3.600–3.700/7.125–7.175/14.150–14.175/21.200–21.225; WARC (30/17/12m) no Extra-only; 60m USB only 5 channels; MF: 2200m 1W EIRP (WD2XSH notify), 630m 5W EIRP (UTC notify); VHF/UHF: Technician+ full privileges — Extra adds nothing above 10m; see extra_band_plan.md |
| 6m sporadic-E field | When 6m opens unexpectedly | Check 50.313 MHz FT8 or 50.125 USB — if signals appear from >500 km you have E-skip; 5.6m whip (M10→SMA adapter) is ~0.93λ on 6m (resonant, no tuner needed); H4M can RX 50 MHz for recon; E-skip season May–July; openings last 5–60 min — get on fast; 10m/15m often open same time (same ionosphere layer) |
| FM simplex — national calling | Emergency / POTA check-in | 146.520 MHz (2m national): CQ for 30 sec, monitor 2 min; if no reply check local repeater; 446.000 MHz (70cm national): same procedure; 52.525 MHz (6m national): less active, worth a monitor; Baofeng UV-5R/Mini covers all three; these are the first calls in an emergency if cell is down |
| VHF weak signal — SSB/CW | Tropo, EME, meteor scatter | 144.200 MHz national SSB/CW calling (USB); 432.100 MHz 70cm calling; rubber duck/Nagoya inadequate — need J-pole minimum, Yagi preferred; tropo ducting: sudden 2m/70cm range extension (100–1000 km) often at dawn; H4M can ID tropo openings via signal strength; (tr)uSDX hi-band reaches 144 MHz territory if modified — otherwise Baofeng only |
| FM satellite — easy mode | Satellite QSOs with Baofeng | SO-50: 145.850 MHz down (FM), 436.795 MHz up (FM), 67 Hz CTCSS for access; AO-91/AO-92 similar; full-duplex: TX up, listen down simultaneously; Nagoya on both radios works for FM sats — no beam needed for low passes; passes.cl or heavens-above.com for pass predictions; doppler shift ±3 kHz — tune 436.793 up, 145.852 down at AOS; 10-min window typical pass |
| 2200m / 630m MF field | Low band experimenters | 2200m: 135.7–137.8 kHz, 1W EIRP; 630m: 472–479 kHz, 5W EIRP; both secondary — notify coordinator before use; antenna: loaded vertical required, electrically short (λ/4 at 475 kHz = 158m — use loaded short vertical with high-Q coil); propagation: NVIS-like for regional, groundwave for local; WSPR most used; not practical for field kit — home station operation only |
| Loading coil calculation | Inductance for short antenna | Measure X_C at feedpoint with NanoVNA (negative reactance); L (µH) = X_C / (2πf); turns: N = √(L_nH / A_L); T50-2 A_L=5.7 nH/T², T37-2 A_L=4.0 nH/T²; air-core: L(µH) = N²r² / (9r+10l) [Wheeler, inches]; wind long, remove turns to raise frequency |
| Balun / choke / unun — which | Feedpoint transformer selection | Current choke balun (1:1): dipole, inverted-V, inverted-L — chokes common mode, no impedance transform; unun (49:1): EFHW feedpoint — impedance only, both sides unbalanced; coax choke: behind EFHW unun + at vertical base — blocks shield current; voltage balun: avoid at HF field use (doesn't suppress common mode); random wire: no balun, Z-match handles |
| Impedance matching — L-network | Why tuners work | Two reactive elements (series L + shunt C or reverse) match any two impedances; Q = √(Z_high/Z_low − 1); higher Q = more harmonic suppression + more coil loss; Z-match uses coupled resonator = inherently high Q; at 5W: component stress negligible; at 60W check coil wire gauge |
| FT8 / digital modes field | Weak signal digital operations | FT8 decodes to −22 dB S/N (~17 dB better than CW, ~25 dB better than SSB); 15-sec TX/RX slots; 100% duty cycle — reduce (tr)uSDX to 2–3W to avoid thermal shutdown; requires UTC clock sync ±1 sec (NTP/GPS/WWV); (tr)uSDX hi-band: single USB cable = audio + CAT + power; WSPR beacon: antenna A/B comparison via wspr.rocks spot count |
| 60m field operation | Channelized band (5 channels) | Channels: 5.3305/5.3465/5.3570/5.3715/5.4035 MHz; USB only (CW/RTTY/data also permitted per 2017 FCC); 100W ERP max all classes; FT8 on ch 3 at 5.3570 MHz dial; EFHW 60m link at 27.50m (near ch 3/4); counterpoise tap at 13.96m; propagation fills 40m/80m daytime gap for 300–1500 km; federal users have priority — QSY immediately if heard |
| Inverted-L antenna | 80m from 8–10m mast | Vertical section runs up mast, bends horizontal at top; DX Commander 8.4m + 12.1m horizontal = 20.5m = λ/4 80m; impedance ~35–65Ω, direct 50Ω coax; 2–4 radials at base; mostly vertical polarization for DX; trim horizontal tail to tune; better than loaded short vertical for 80m efficiency |
| Portable magnetic loop | No-tree / concrete site option | Small transmitting loop; diameter ~0.1λ; bandwidth 50–500 Hz — must retune every few kHz; horizontal = NVIS, vertical = DX; efficiency 70–90% on resonance; cap voltage 500–2000V at 5W — commercial vacuum/butterfly cap required; Alpha Loop/Chameleon MPAS Lite/Alex Loop (~$150–350); fills "open concrete" gap in scenario table |
| Mobile / vehicle operation | POTA park-and-operate | Car roof = excellent ground plane (2–4 m² metal, beats 4 radials); mag mount center of roof; NMO mount (3/4" hole) eliminates contact resistance; choke at antenna base before entering cab (Mix 31 × 4 snap-ons); engine off = eliminates alternator noise; window-seal coax routing; 5.6m whip on NMO = same kit as field, different mount |
| Portable Yagi — tape measure | 2m satellite / fox hunt / POTA VHF | 3-element; reflector 1041 mm, DE 991 mm (2× 495.5 mm), director 946 mm; spacing R–DE 330 mm, DE–dir 165 mm; hairpin match for ~50Ω; boom = 1/2" PVC; folds flat; ~7 dBi gain; beats J-pole by 2–3 S-units; use for SO-50/AO-91 satellite manual tracking, fox hunting, VHF POTA credit |
| Antenna A/B comparison | Which antenna is better at this site | Find active station; note S-meter with antenna A; swap in < 2 min; note antenna B; 1 S-unit = 6 dB = 4× power; noise floor comparison works on quiet band (more noise floor = better antenna); WSPR beacon 1 hr per antenna — count spots on wspr.rocks; record in POTA naturalist log for site database |
| Coax VF measurement | Identify unknown coax | Connect 1m coax to NanoVNA CH0; far end open; find first anti-resonance frequency (Smith chart: right-side centerline); VF = (f_MHz × 4 × L_m) / 299.8; shorted end method: find first series resonance; check against table: RG-174=0.66, RG-8X=0.82, LMR-400=0.85; quarter-wave stub: L(m) = VF × 75 / f_MHz |
| RF exposure at QRP | Safety at 5W / 60W | Categorically exempt (no evaluation needed): 5W at any HF band; 60W at HF still exempt; MPE thresholds: 40m exempt below 200W, 20m below 150W; 5W safe at any distance > 0.3m from antenna; real concern: EFHW wire tip / loop cap = ~224V at 20W (~112V at 5W) — electric shock hazard, not RF exposure |
| EFHW vs dipole vs random wire | Which antenna for which goal | EFHW: no ground needed, multiband, ~85% efficiency, needs 49:1 unun + coax choke, needs one end at height — best for trees/poles; Dipole: balanced, ~90–95%, direct 50Ω coax, needs two anchor points — best for single band/quiet feedline; Random wire: fastest deploy, any wire length, needs Z-match, ~60–80%, needs λ/4 counterpoise — best for speed or when no resonant wire available |
| EFHW half-wave lengths | Wire cut lengths, all bands | 80m=40.5m (133 ft); 60m=27.5m (90 ft); 40m=20.7m (68 ft); 30m=14.6m (48 ft); 20m=10.4m (34 ft); 17m=8.2m (27 ft); 15m=7.05m (23 ft); 12m=5.95m (20 ft); 10m=5.29m (17 ft); 6m=2.95m (10 ft); all ×1.015 VF for insulated wire; cut 2–3% long and trim to NanoVNA resonance |
| Antenna wire storage / packing | Tangle prevention, protect links | EFHW: figure-8 coil on hand, rubber band at center; links: park female banana in heat-shrink sleeve before coiling — never loose in bag; counterpoise: separate pouch per wire, label ends; 30 AWG PTFE: small reel or tight figure-8; do not coil SMA-ended wire tight — 6" minimum bend radius; SMA pins: keep in foam pouch, never loose |
| Site assessment — 60-second | Before opening the bag | Trees: height and anchor potential (>10m for EFHW sloper); ground: grass vs concrete ratio; bearing: compass to primary DX target (Europe=28°, Japan=314°); power lines: avoid deploying wire within 2× wire length horizontally; nearby metal: chain-link fence detunes nearby antennas; check wind direction for wire layout |
| Pre-deployment band scan | What's active before rigging | H4M waterfall: tune 40m/20m in AM wide mode — note noise floor and any strong local interference; TinySA: sweep target band, check for local transmitters or switching supply hash above −60 dBm; if noise floor is S7+, consider different site or null antenna toward noise; 5-minute scan before 2-hour setup avoids disappointment |
| Wire-to-foliage contact | Detuning when wire touches leaves | Wire on leaves/branches shifts resonance 1–5% lower and raises feedpoint R by 5–15Ω (wet foliage = lossy dielectric); symptoms: SWR drifts after deploy, SWR drifts with wind; fix: haul wire higher to clear canopy, add tension to keep wire taut; acceptable: brief contact at center of wire is worse than contact at tips; dry leaves = less effect than wet |
| Mast guying | Keeping the pole up | Fishing pole (6m): push base 15–20 cm into soft soil; one paracord guy from upper section to stake at 45° handles moderate wind; Squid pole (10m): guy at 2/3 height (6–7m) minimum, add second near base for wind; Push-up / painter pole: two guys at 90° from each other at 45° angle, 3m radius; car roof rack: lash to rack rail, no stake needed; never leave unguyed above 8m in gusty conditions |
| Seasonal ground performance | R_ground by season — Merced CA | Spring (Mar–May): wet soil, R_ground 36–50Ω, best vertical performance; Early summer (Jun): soil drying, R_ground 50–70Ω; Late summer (Aug–Sep): dry hardpan, R_ground 80–120Ω, add more radials; Fall (Oct–Nov): first rains, R_ground recovering; foliage May–Oct: overhead wire sags, leaves detune elevated dipoles; compensate dry season: lay extra radials, water grass at base |
| ATU-100 — field memory | Using stored tune settings | Press button at matched frequency to store in memory (LED flash confirms); on next power-up, auto-retrieves last tune for current frequency; field strategy: tune each band once at home or first site of the day, rely on memory for fast band changes; memory survives power cycle; if SWR is poor after band change, press tune button once to re-search from stored starting point |
| Loaded whip vs full EFHW — efficiency | When setup time matters | Loaded 5.6m whip: ~50–65% radiation efficiency (rest lost in coil/ground resistance); EFHW: ~85–90%; difference: 1.3–2.5 dB = about 0.5–1 S-unit on receive at the other end; at 5W: whip delivers ~2.5–3.25W radiated, EFHW ~4.25–4.5W; rule: if deploy time difference is < 5 min, use EFHW; if concrete site with no trees and time is short, whip + ATU is the right call |
