# TECHNICAL MANUAL — PORTABLE COMMON-MODE CHOKES FOR RFI SUPPRESSION

**TM-CMC-001  Revision A**
**Common-Mode Chokes Covering 160M through 23cm Amateur Radio Bands**
**Toroidal, Snap-On Bead, Coiled Coax, and Sleeve Balun Designs**

---

**CLASSIFICATION:** Unclassified / For General Distribution
**DATE:** 2026-04-25
**SUPERSEDES:** None (initial issue)
**APPLICABLE EQUIPMENT:** CMC-1 Toroid HF, CMC-2 Snap-On Beads, CMC-3 Ugly Balun,
CMC-4 Bead-on-Coax, CMC-5 Sleeve Balun VHF/UHF

---

## TABLE OF CONTENTS

- [Chapter 1 — Introduction: Common-Mode Current and RFI](#chapter-1)
- [Chapter 2 — Ferrite Core Materials and Selection](#chapter-2)
- [Chapter 3 — Toroidal Wound Chokes — HF Bands (CMC-1)](#chapter-3)
- [Chapter 4 — Snap-On Ferrite Bead Chokes (CMC-2)](#chapter-4)
- [Chapter 5 — Ugly Balun and Coiled Coax Chokes (CMC-3)](#chapter-5)
- [Chapter 6 — Bead-on-Coax Wideband Choke (CMC-4)](#chapter-6)
- [Chapter 7 — Sleeve Balun for VHF and UHF (CMC-5)](#chapter-7)
- [Chapter 8 — Choking Impedance Measurements and Calculations](#chapter-8)
- [Chapter 9 — Weatherproof Enclosure and Mechanical Installation](#chapter-9)
- [Chapter 10 — RFI Troubleshooting Procedures](#chapter-10)
- [Chapter 11 — Band-by-Band Quick Reference](#chapter-11)
- [Appendix A — Ferrite Core Specification Tables](#appendix-a)
- [Appendix B — Choking Impedance Equations and Design Nomographs](#appendix-b)
- [Appendix C — Common-Mode Rejection Measurement Procedure](#appendix-c)
- [Appendix D — Parts Sources and Equivalents](#appendix-d)

---

<a name="chapter-1"></a>
## CHAPTER 1 — INTRODUCTION: COMMON-MODE CURRENT AND RFI

### 1.1 Purpose

This manual describes the design, construction, installation, and testing of
portable common-mode chokes for amateur radio RFI suppression.  Five distinct
choke designs are documented, covering all amateur bands from 160 meters (1.8 MHz)
through the 23-centimeter band (1296 MHz).

Common-mode chokes are the single most effective tool for:
- Eliminating RF current on the outside of coaxial cable feedlines
- Isolating antenna feedpoints from feedline radiation
- Suppressing received interference entering the station via feedlines
- Achieving proper balance at dipole and balanced antenna feedpoints
- Reducing TVI, BCI, and audio rectification complaints

### 1.2 Common-Mode Current — Definition and Effects

A coaxial transmission line carries two modes of current simultaneously:

**Differential mode (desired):** Signal current flows on the center conductor
in one direction and returns on the INSIDE surface of the outer conductor in
the opposite direction.  Fields cancel outside the cable; no radiation.

**Common-mode (undesired):** Current flows on the OUTSIDE surface of the outer
conductor.  This current is driven by antenna feedpoint imbalance, ground
loops, or coupling from nearby RF sources.  Common-mode current:

1. Causes feedline radiation, changing antenna pattern and efficiency
2. Carries received interference from noise sources in the shack into the receiver
3. Produces "RF in the shack" — RF burns on microphones, TVI, audio rectification
4. Creates ground loops between equipment creating hum and buzz
5. Allows transmit power to heat shack equipment via ground loops

**Measurement of common-mode current:** Insert a snap-on current probe (clamp-on
RF ammeter) around the coax feedline at the shack entry.  Any reading indicates
common-mode current.  A well-choked system will show near-zero on the probe.
Target: < 5 mA common-mode current at antenna feedpoint while transmitting.

### 1.3 The Choking Action

A common-mode choke places a series impedance Z_choke in the common-mode current
path.  The choke is transparent to differential mode (signal).

Common-mode rejection in dB:
```
CMR = 20 × log₁₀( 1 + |Z_choke| / |Z_CM_path| )
```

Where Z_CM_path is the impedance of the common-mode return path (typically
50–300 Ω for the antenna-feedline-ground system).

**Practical CMR targets:**

| Application              | CMR needed | Z_choke @ 150Ω path |
|--------------------------|------------|---------------------|
| Minimum (any benefit)    | 10 dB      | 474 Ω               |
| Field use, portable      | 20 dB      | 1500 Ω              |
| Fixed station, HF        | 25 dB      | 2820 Ω              |
| Contest station          | 30 dB      | 4743 Ω              |
| Extreme isolation (SO2R) | 40 dB      | 15000 Ω             |

### 1.4 Choke Type Selection Matrix

| Condition                          | Recommended Type       | Section |
|------------------------------------|------------------------|---------|
| HF bands, feedpoint balun          | Toroid wound (CMC-1)   | Ch. 3   |
| HF bands, quick install, no cutting| Snap-on beads (CMC-2)  | Ch. 4   |
| Field day, no ferrite available    | Ugly balun (CMC-3)     | Ch. 5   |
| Wideband 3–1300 MHz, one choke     | Bead-on-coax (CMC-4)   | Ch. 6   |
| VHF/UHF antenna feedpoint          | Sleeve balun (CMC-5)   | Ch. 7   |
| RFI entering shack on all cables   | Snap-on beads (CMC-2)  | Ch. 4   |
| 160M–10M wideband single choke     | Stacked FT-240-31+43   | Ch. 3   |

---

<a name="chapter-2"></a>
## CHAPTER 2 — FERRITE CORE MATERIALS AND SELECTION

### 2.1 Ferrite Chemistry and Permeability

Ferrite cores used for RF chokes are ceramic compounds of iron oxide with
manganese-zinc (MnZn) or nickel-zinc (NiZn) additives.  The material type
determines the complex permeability µ = µ' − jµ'' as a function of frequency.

**µ' (real component):** Reactive inductance; energy stored and returned.
  Produces inductive (jX) component of choke impedance.

**µ'' (imaginary component):** Loss; energy absorbed from RF signal.
  Produces resistive (R) component of choke impedance.

**For common-mode choke applications, BOTH R and jX contribute to choking.**
A resistive choke actually provides BETTER broad-frequency suppression than
a purely inductive one — the R component is non-resonant and wideband.

### 2.2 Core Material Comparison

| Mix | Base material | µ_initial | Peak loss freq | Best application         |
|-----|--------------|-----------|----------------|--------------------------|
| 31  | MnZn         | 1500      | 2–30 MHz       | 160M–10M, best HF choice |
| 43  | NiZn         | 850       | 10–100 MHz     | 40M–6M, upper HF/VHF     |
| 61  | NiZn         | 125       | 50–1000 MHz    | VHF/UHF bead-on-coax     |
| 52  | NiZn         | 250       | 100–1500 MHz   | UHF applications         |
| 77  | MnZn         | 2000      | 0.1–5 MHz      | 160M emphasis            |
| 75  | MnZn         | 5000      | <2 MHz         | LF/60Hz only             |

### 2.3 Core Size Designations

Fair-Rite / Amidon standard:
- **FT-240-xx**: 61mm OD × 35mm ID × 12.7mm height (large, standard HF)
- **FT-193-xx**: 49mm OD × 31mm ID × 12.7mm height (medium)
- **FT-140-xx**: 35.5mm OD × 23mm ID × 12.7mm height (portable HF)
- **FT-114-xx**: 29mm OD × 19mm ID × 7.5mm height (VHF)
- **FT-82-xx**: 21mm OD × 13mm ID × 6.35mm height (VHF/UHF)
- **FT-50-xx**: 12.7mm OD × 7.7mm ID × 6.35mm height (UHF)
- **FT-37-xx**: 9.5mm OD × 4.7mm ID × 3.2mm height (microwave)

**FT-240 cores** are the workhorse of HF station choke design.
**FT-82 and FT-50** are used for bead-on-coax VHF/UHF chokes.

### 2.4 Inductance Factor A_L

The inductance of N turns on a core:
```
L (nH) = A_L × N²
```

where A_L is in nH/N² (nH per turn squared).

| Core           | Mix 31  | Mix 43  | Mix 61  |
|----------------|---------|---------|---------|
| FT-240         | 1800    | 1100    | 195     |
| FT-140         | 1000    | 625     | 111     |
| FT-82          | 570     | 557     | 36      |
| FT-50          | 320     | 320     | 22      |

A_L values are nominal at 10 kHz; vary with frequency as µ changes.

### 2.5 Core Selection Procedure

1. Identify the lowest operating frequency (determines minimum A_L needed)
2. At lowest frequency: need |Z| ≥ 1000 Ω; L = |Z|/(2πf); N = √(L/A_L)
3. Verify N × coax OD fits within core ID (for wound chokes)
4. Verify impedance remains adequate across entire operating range
5. Confirm power handling: at intended transmit power, core temperature < 50°C

**Design example: wound choke for 80M–20M with FT-240-43, targeting |Z| ≥ 2000 Ω:**

  At 80M (3.5 MHz): L_needed = 2000 / (2π × 3.5×10⁶) = 90.9 µH
  N = √(90.9×10⁶ / 1100) = √82636 = 288 turns — far too many!

  Conclusion: FT-240-43 is not adequate for 80M.  Use Mix 31:
  At 80M: L_needed = 90.9 µH, A_L = 1800 → N = √(90.9×10⁶/1800) = 225 turns?
  Also too many — BUT at 3.5 MHz, Mix 31 has significant resistive component
  that adds to total |Z|.  The true impedance table (measured data) shows
  FT-240-31 at 10 turns provides |Z| = 3190 Ω at 3.5 MHz.

  This is why measured impedance data (see ODS file, Chapter 8) is more reliable
  than purely inductive calculations at the frequencies where loss dominates.

---

<a name="chapter-3"></a>
## CHAPTER 3 — TOROIDAL WOUND CHOKES — HF BANDS (CMC-1)

### 3.1 Design Variants

Three wound toroid configurations are documented (see also
schematics/sch_toroid_choke_hf.txt):

| Variant   | Core(s)        | Turns | Cable  | Best for              |
|-----------|---------------|-------|--------|-----------------------|
| CMC-1A    | FT-240-31     | 10    | RG-8X  | 160M–10M general      |
| CMC-1B    | FT-240-31+43 stacked | 8  | RG-8X  | 160M–10M maximum Z |
| CMC-1C    | FT-140-43     | 10    | RG-58  | 40M–6M portable/QRP   |

### 3.2 Winding Instructions — CMC-1A (FT-240-31, 10 turns RG-8X)

**Materials:**
- 1× FT-240-31 toroid core (61mm OD)
- 2.2m RG-8X (8.4mm OD)
- 2× PL-259 or N-type connectors
- Self-amalgamating tape (weatherproof wrap)
- Heavy cable tie × 2

**Procedure:**
1. Install connectors on both ends of the RG-8X.  Test continuity before winding
   (center to center; shield to shield; no shorts).
2. Place the FT-240-31 core flat on work surface.
3. Pass one connector-end of RG-8X down through the center hole of the core.
   Pull through leaving 25cm of pigtail below the core.
4. Bring the cable up and over the outer edge of the core, passing down through
   the center again.  This completes 1 turn.
5. Continue winding in the same rotational direction for all 10 turns.
6. Distribute turns evenly around approximately 270–300° of the core.
   Leave a gap of 60–90° between the first and last turn.
   DO NOT close-wind 360° — inter-winding capacitance rises sharply.
7. Secure turns with 2× cable ties through core center hole.
8. Wrap the entire coil with 3 layers of self-amalgamating tape for weatherproofing.

**Winding diagram:**

```
    Looking from above — turns wound CW, spread 300° around core:

                  ┌────── gap ──────┐
         Turn 10 /                   \ Turn 1
         Turn 9 │    FT-240-31       │
         Turn 8 │      CORE          │ Turn 2
         Turn 7 │                    │ Turn 3
         Turn 6  \                  / Turn 4
                   Turn 5 ─ Turn 5
```

**Maximum turns before cable fills core ID:**
- RG-8X (8.4mm) through FT-240 (35mm ID): max ~11 turns comfortably
- RG-58 (5mm) through FT-240 (35mm ID): max ~17 turns
- RG-174 (2.8mm) through FT-140 (23mm ID): max ~12 turns

### 3.3 Stacked Core Choke (CMC-1B)

For maximum impedance across 160M–10M, stack one FT-240-31 and one FT-240-43
core and wind both simultaneously as a single unit.

**Stacking procedure:**
1. Place both cores face-to-face (flat faces together).
2. Check alignment — hole centers should be concentric.
3. Wrap both cores together with 2 layers of self-amalgamating tape to bind them.
4. Wind 8 turns of RG-8X through both stacked cores simultaneously.
   Each pass through the combined double-hole aperture = 1 turn.
5. Resulting compound core behaves as the sum of individual core impedances.

### 3.4 Impedance Measurement — Wound Choke

**Required equipment:** VNA (NanoVNA or similar); 50Ω termination

**Procedure:**
1. Connect VNA PORT 1 to one coax pigtail of the choke.
2. Connect 50Ω termination to the other pigtail.
3. Configure VNA for S11 measurement, 1–54 MHz sweep.
4. The choke common-mode impedance is NOT directly measured this way —
   S11 measures the DIFFERENTIAL mode (transmission line) impedance, which
   should be flat at 50Ω ± 2Ω (VSWR < 1.1) for a well-built choke.

**True common-mode measurement:** (see Appendix C for full procedure)
Requires a 1:1 RF transformer (or direct injection) to drive the choke in
common-mode configuration.  The standard W1JR/W7EL measurement method uses
a coaxial fixture with the choke outer conductor driven as a one-port device.

**Simplified check:** Place a clip-on RF current probe around the choke output
coax.  Drive the choke input with RF at known power.  Current probe reading
directly shows common-mode current.  Repeat at multiple frequencies.
Choking impedance ≈ V_input / I_CM_measured.

---

<a name="chapter-4"></a>
## CHAPTER 4 — SNAP-ON FERRITE BEAD CHOKES (CMC-2)

### 4.1 Advantages of Snap-On Beads

Snap-on (split) ferrite cores require no disassembly of existing coax or cable
runs.  They can be field-installed in minutes on any cable regardless of
connector type.  Disadvantages: lower impedance per unit than wound toroids;
multiple beads required for adequate suppression.

### 4.2 Bead Material Selection

For HF (1.8–30 MHz): Mix 31 (Fair-Rite 0431xxxxxx or Laird 28B series)
For HF/VHF (7–144 MHz): Mix 43 (Fair-Rite 0443xxxxxx)
For wideband: mix 4× Mix 31 + 4× Mix 43 in series on same cable

Reference data: ferrite_impedance_table.ods, sheet "Snap-On Beads"

### 4.3 Required Bead Count

Minimum required beads for CMR ≥ 20 dB (Z_path assumed 150 Ω):
  Target |Z_choke| ≥ 1500 Ω

| Band  | Freq   | Z per Mix31 bead | Beads needed |
|-------|--------|-----------------|--------------|
| 160M  | 1.8 MHz| 195 Ω           | 8+           |
| 80M   | 3.5 MHz| 270 Ω           | 6            |
| 40M   | 7.0 MHz| 430 Ω           | 4            |
| 20M   | 14 MHz | 545 Ω           | 3            |
| 10M   | 28 MHz | 600 Ω           | 3            |
| 6M    | 50 MHz | 600 Ω           | 3 Mix43 better|
| 2M    | 144 MHz| 480 Ω           | 4 Mix43      |

**Practical recommendation:** 8 beads (4× Mix31 + 4× Mix43) covers all bands
adequately.  Install in a row along the cable; secure with cable ties.

### 4.4 Multiple-Pass Installation

For powerline and mains filtering where cable length allows:
Thread cable through same bead N times.  Impedance scales as N²:

| Passes | |Z| at 7 MHz (Mix31 1.125" bead) |
|--------|-----------------------------------|
| 1      | 430 Ω                             |
| 2      | 1720 Ω (4×)                        |
| 3      | 3870 Ω (9×)                        |
| 4      | 6880 Ω (16×)                        |

Note: f_SR (self-resonant frequency) decreases with multiple passes.
At 3 passes, f_SR drops to ~10 MHz — adequate for AM/HF powerline noise only.
Use single-pass multiple-bead approach for wideband HF/VHF.

### 4.5 Installation Locations

**Priority locations for snap-on beads:**

1. **Coax at shack bulkhead entry point** — most important; intercepts all
   common-mode current attempting to enter the shack.  Install 6–8 beads here.

2. **Radio power supply cable** — switching supplies generate wideband noise;
   beads on output DC cable prevent conducted noise.

3. **Microphone cable** — common entry point for RF during TX.
   Wind mic cable 3–4 turns through 1× large snap-on bead.

4. **Computer USB/monitor cables** — USB devices and LCD monitors are common
   broadband noise sources on HF.  Install at device end.

5. **Ethernet cable** — can radiate broadband digital noise and also conduct
   interference between equipment.

6. **Rotor control cable** — long runs of rotor cable act as effective antennas
   for picking up local noise.

---

<a name="chapter-5"></a>
## CHAPTER 5 — UGLY BALUN AND COILED COAX CHOKES (CMC-3)

### 5.1 Operating Principle

The "ugly balun" (coined by W7EL, Roy Lewallen, 1983) uses the inductance of
coax wound into an air-core coil.  No ferrite is required.  Coiling the coax
creates inductance that blocks common-mode current.  Differential mode
(the signal) is unaffected because the electromagnetic fields of the center
conductor and shield cancel within the cable.

### 5.2 Design Frequency Targeting

The ugly balun works best near its self-resonant frequency (f_SR) where
impedance is maximum.  Design f_SR to match the operating frequency.

f_SR is lowered by:
- More turns (increases L and C together)
- Smaller coil diameter (increases C)
- Closer winding spacing (increases C)

f_SR is raised by:
- Fewer turns
- Larger diameter
- Space-wound turns

**Coil inductance (Wheeler formula):**
```
L (µH) = [r² × N²] / [9r + 10l]
```
Where r = coil radius (inches), l = coil length (inches), N = turns

**Example: 9 turns RG-8X, 6" OD form (r=3"), space-wound l=3.5":**
  L = [9 × 81] / [9×3 + 10×3.5] = 729 / [27 + 35] = 11.8 µH
  C_distributed ≈ 15 pF (loose RG-8X)
  f_SR = 1 / (2π × √(11.8×10⁻⁶ × 15×10⁻¹²)) = 11.9 MHz

Design table: coil parameters vs. target f_SR:

| Form OD | Turns | Coax    | l (mm) | L (µH) | f_SR (MHz) | Best bands    |
|---------|-------|---------|--------|--------|------------|---------------|
| 2" (51mm)| 12   | RG-174  |  60    |  3.1   |   49       | 6M, VHF       |
| 3" (76mm)| 9    | RG-58   |  50    |  8.6   |   18       | 17M–10M       |
| 4" (102mm)| 8   | RG-58   |  50    |  11.8  |   14       | 20M–15M       |
| 6" (150mm)| 7   | RG-8X   |  70    |  14.7  |   11       | 30M–20M       |
| 8" (200mm)| 7   | RG-8X   | 100    |  21.0  |    7       | 40M           |

### 5.3 Portable Ugly Balun Construction (CMC-3A, field expedient)

When no ferrite is available, coil available feedline around any available form:

1. Find a cylindrical object 10–20cm diameter (water bottle, PVC pipe, coffee can).
2. Wind 8–12 turns of the coax feedline.
3. Secure with zip ties or tape.
4. Best performance at operating frequency; some benefit ±1 octave.

**This is the emergency field technique.**  Pre-built ferrite chokes are strongly
preferred for any planned installation.

### 5.4 Power Handling

Coiled coax handles full rated power of the coax used.  No ferrite to saturate
or overheat.  RG-8X handles 1500W at 14 MHz indefinitely.  RG-58 handles 500W.

The ugly balun is the ONLY choke type with no practical RF power limitation
(limited only by coax specs, not ferrite saturation).  This makes it attractive
for high-power 160M/80M operation where ferrite heating can occur.

---

<a name="chapter-6"></a>
## CHAPTER 6 — BEAD-ON-COAX WIDEBAND CHOKE (CMC-4)

### 6.1 Construction

Thread ferrite toroid beads over the coaxial cable before connector installation.
No winding required.  The beads sit on the cable's outer jacket and present
impedance to any current flowing on the outside of the cable.

**Critical:** Connectors (PL-259, N-type) must be installed AFTER threading
all beads onto the cable — connector body will not pass through bead aperture.

**Bead aperture vs. cable OD:**
- FT-82-43 (13mm ID) accepts RG-58 (5mm OD) with 8mm of clearance — easy
- FT-50-61 (7.7mm ID) accepts RG-58 (5mm OD) with 2.7mm clearance — OK
- FT-50-61 will NOT accept RG-8X (8.4mm) — use FT-82 or larger

### 6.2 Optimal Bead Mix by Frequency Range

| Purpose           | Bead selection               | Count | CMR at design freq |
|-------------------|------------------------------|-------|--------------------|
| HF only (40M–10M) | FT-82-43                     | 10    | 30+ dB at 14 MHz   |
| HF + VHF          | 6× FT-82-43 + 4× FT-82-61   | 10    | 25+ dB, 7–432 MHz  |
| VHF/UHF only      | FT-50-61                     | 20    | 30+ dB, 50–1300 MHz|
| Wideband 3–1300MHz| 4× FT-82-43 + 4× FT-50-61   | 8     | 20+ dB all bands   |

### 6.3 Securing the Bead Stack

1. Position beads in middle of coax run (not over connector).
2. Butt beads together; no gap needed.
3. Slide a 50mm length of heat-shrink (1" diameter) over the stack.
4. Heat to shrink; encases the beads as a unit.
5. Add cable tie at each end of heat-shrink to prevent migration.

Optional: use the 3D-printed bead rail (openscad/mounting_brackets.scad,
module bead_rail()) for a neater, field-serviceable installation.

---

<a name="chapter-7"></a>
## CHAPTER 7 — SLEEVE BALUN FOR VHF AND UHF (CMC-5)

### 7.1 Operating Principle

A sleeve balun (also called a choke sleeve or bazooka balun) uses a λ/4 shorted
coaxial stub formed by an outer conductive sleeve around the feed coax.

**The sleeve and the outer surface of the coax form a transmission line stub:**
- Shorted at the ANTENNA END (electrically bonded to outer conductor)
- Open at the TRANSMITTER END (free end of sleeve)

A λ/4 shorted stub presents infinite impedance at the open end.  This infinite
impedance blocks common-mode current at the operating frequency.

### 7.2 Band-Specific Sleeve Lengths

Using velocity factor 0.96 (air-spaced sleeve with end correction):

| Band | f₀ (MHz) | Physical length | Sleeve OD | Coax type  |
|------|---------|----------------|-----------|------------|
| 2M   | 144.0   | 500 mm         | 19 mm     | RG-58      |
| 1.25M| 222.0   | 325 mm         | 16 mm     | RG-58      |
| 70cm | 432.0   | 167 mm         | 12.7 mm   | RG-58/LMR-195|
| 33cm | 902.0   |  80 mm         |  9.5 mm   | LMR-195    |
| 23cm | 1296.0  |  56 mm         |  6.4 mm   | LMR-195    |

### 7.3 Sleeve Balun Construction

**Copper foil tape method (easiest, field-expedient):**

1. Cut 3mm closed-cell foam tape to sleeve length.
2. Wrap foam tape once around coax outer jacket.  Foam provides ~3mm gap between
   coax outer conductor and sleeve.
3. Apply 1.5"-wide (38mm) copper foil tape over foam.  Wrap around cable, length
   = sleeve length.  Overlap seams for electrical continuity.
4. At the antenna end: connect copper foil sleeve to coax outer conductor with
   a solder bridge — remove jacket for 5mm, tin copper foil edge, solder to braid.
5. Transmitter end of sleeve: simply leave free (open circuit).
6. Seal with self-amalgamating tape (2 layers), then UV-resistant vinyl tape.

**Copper tube method (more durable, permanent installation):**

1. Cut copper or brass tube to sleeve length (see table above).
2. Tube ID must be ≥ coax OD + 3mm for slip fit.
3. Slide tube over coax before connector installation.
4. Center tube at desired position; solder tube end to coax outer braid at
   the antenna end.
5. Weatherproof with self-amalgamating tape.

### 7.4 Bandwidth

Sleeve balun bandwidth (to −3 dB of peak impedance): approximately ±3–5%
of center frequency for copper sleeves with Q ≈ 20–50.

For 144 MHz:  ±3% = ±4.3 MHz → covers 139.7–148.3 MHz (all 2M ham use)
For 432 MHz:  ±3% = ±13 MHz → covers 419–445 MHz (entire 70cm band)

For narrower antenna designs, tune sleeve length ±2mm per MHz of f_SR error.
Shortening sleeve raises f_SR; lengthening lowers f_SR.

### 7.5 6M Sleeve Balun — Special Case

At 50 MHz, the sleeve would be 1437mm (nearly 1.5 meters) — impractical.
For 6M:
- Use FT-240-43 wound toroid (8 turns, |Z| ≈ 4866 Ω)
- Or use ugly balun coil (2" PVC pipe, 12 turns RG-174, f_SR ≈ 49 MHz)
- Or use 6× snap-on Mix 43 beads

---

<a name="chapter-8"></a>
## CHAPTER 8 — CHOKING IMPEDANCE MEASUREMENTS AND CALCULATIONS

### 8.1 Impedance Data Tables

Complete measured impedance data is contained in:
`data/ferrite_impedance_table.ods`

The spreadsheet contains 8 worksheets:
1. **Ferrite Materials** — core material properties comparison
2. **FT-240-31 10T** — 10-turn FT-240-31 impedance vs. frequency
3. **FT-240-43 12T** — 12-turn FT-240-43 impedance vs. frequency
4. **FT-240-43 8T** — 8-turn FT-240-43 impedance vs. frequency
5. **Stacked 31+43 8T** — stacked core combination data
6. **Snap-On Beads** — per-bead and N-bead stack data
7. **Sleeve Balun Dims** — band-by-band sleeve dimensions
8. **RFI Troubleshooting** — symptom-to-solution cross reference

### 8.2 Common-Mode Impedance Calculation

For a wound toroid choke, total impedance at frequency f:

**Inductive reactance:**
```
X_L = 2π × f × L = 2π × f × A_L × N²
```

**At frequencies where material is lossy (above loss peak frequency):**
  The R component dominates and is approximately:
```
R_choke ≈ µ'' × (2π × f × µ₀ × N² × A_c / l_e)
```
Where A_c = core cross-section, l_e = effective magnetic path length.

**In practice:** Use manufacturer-measured complex permeability data (Fair-Rite
#43 and #31 data sheets available at fair-rite.com) and compute Z = N² × Z_1turn.

**CMR from measured impedance:**
```
CMR (dB) = 20 × log₁₀(1 + |Z_choke| / |Z_CM_path|)
```

Where Z_CM_path is the measured or estimated impedance of the common-mode
path at the antenna feedpoint (typically 50–300 Ω; see Appendix C for measurement).

### 8.3 Insertion Loss of Choke (Differential Mode)

A well-built choke has negligible differential-mode insertion loss:
- Differential currents cancel in the ferrite; no net magnetizing force
- Core does not contribute to differential-mode impedance
- Only the coax series resistance contributes: RL = R_DC × l_coax

For 10 turns RG-8X (2.2m), center conductor DC resistance:
  RG-8X conductor R = 8.8 mΩ/m × 2.2m = 19.4 mΩ
  Insertion loss = R/(2 × Z_source) = 19.4×10⁻³ / (2 × 50) = 0.000194 → −0.0017 dB

**Differential insertion loss of a properly wound choke is negligible (<0.01 dB).**
Any significant insertion loss indicates a winding error (shorted turns) or
wrong core material (core is lossy at the operating frequency for differential mode).

### 8.4 Response Measurement — NanoVNA Method

A NanoVNA can measure the common-mode impedance of a wound choke using the
"open-short-through" method if the following fixture is used:

**Fixture:** Short the coax outer conductor to the coax center conductor at
one end of the choke.  Connect other end to PORT1 of NanoVNA.  Configure
NanoVNA for S11 measurement with OPEN calibration standard.  The one-port
measurement now sees the choke as a one-port impedance = the common-mode
impedance.

**Calibrate:** Place standard SHORT at measurement port; place OPEN; place LOAD.
Then connect fixture.  Read Z = R + jX on Smith chart or Z display.

**Alternative (requires two NanoVNA ports or external bridge):**
Connect a 50Ω two-port measurement where the outer conductor of the input
and output coax are JOINED (common-mode injection) while center conductors are
the + ports.  S21 attenuation equals the CMR of the choke.

---

<a name="chapter-9"></a>
## CHAPTER 9 — WEATHERPROOF ENCLOSURE AND MECHANICAL INSTALLATION

### 9.1 Enclosure Design

The weatherproof enclosure (openscad/enclosure_choke_hf.scad) protects wound
toroid chokes from the following outdoor hazards:

| Hazard           | Protection method                                    |
|------------------|------------------------------------------------------|
| Rain/moisture    | O-ring sealed lid + drainage hole at bottom          |
| UV degradation   | ASA material (rated for 10+ years outdoor)           |
| Ice loading      | 3.5mm walls + gusseted corners                       |
| Wind vibration   | Ferrite standoff shelf; bead retainer clips          |
| Connector moisture| Weatherproof cable gland / self-amal. tape at ports|
| Thermal cycling  | ASA TCC < 60 ppm/°C; minimal dimensional change      |

**NEVER use PLA for outdoor enclosures.** PLA softens at ~60°C — easily reached
in a dark enclosure in direct summer sun.  ASA or PETG minimum.

### 9.2 Printed Part List and Orientation

| Part               | File                        | Orientation   | Supports? |
|--------------------|-----------------------------|---------------|-----------|
| Choke body         | enclosure_choke_hf.scad     | Open side up  | None      |
| Choke lid          | enclosure_choke_hf.scad     | Flat side down| None      |
| Port gland ×2      | enclosure_choke_hf.scad     | Upright       | None      |
| Mast bracket       | mounting_brackets.scad      | Flat side down| None      |
| Wall bracket       | mounting_brackets.scad      | Any           | Optional  |
| Strain relief ×2   | mounting_brackets.scad      | Lower side down| None     |
| Core retainer ×2   | mounting_brackets.scad      | Flat side down| None      |
| Bead rail (opt)    | mounting_brackets.scad      | Side           | None      |

### 9.3 Assembly Sequence

1. **Install heat-set inserts:** M4 inserts in lid screw boss holes using
   230°C soldering iron.  Cool completely before threading any screws.

2. **Prepare coax:** Install connectors; test S11 (VSWR < 1.1 at operating freq).

3. **Wind core:** Per Chapter 3 procedures.  Wrap in self-amalgamating tape.

4. **Place in enclosure:** Set core on standoff shelf.  Route pigtails to coax ports.
   Maintain 5mm clearance between coil and enclosure walls.

5. **Install port connectors:** Thread connectors through port holes.  Apply
   silicone sealant around connector body from inside before tightening nut.

6. **Apply O-ring:** Place 3mm Viton or EPDM O-ring in lid groove.  Apply thin
   bead of RTV silicone over O-ring before closing lid.

7. **Close and secure:** 4× M4 × 12mm pan-head stainless screws.  Torque to
   0.5 N·m (hand tight + 1/4 turn).  Do not overtorque — will deform O-ring groove.

8. **Label:** Attach label to recess on lid:
   `CMC-1A  FT-240-31  10t RG-8X  160M–10M  Date: ______`

### 9.4 Mast Mounting

**U-bolt mast bracket** (mounting_brackets.scad, `ubolt_mast_bracket()`):
1. Print bracket with mast_d parameter matching actual mast diameter.
2. Slide two M6 U-bolts through bracket slots (open toward mast).
3. Position bracket on mast at desired height; engage U-bolts around mast.
4. Thread M6 wing nuts; tighten to 8 N·m (firm, not crushing plastic).
5. Mount choke enclosure to bracket with 4× M6 × 16mm stainless bolts.
6. Route coax with drip loop (lowest point of coax run below connector entry).

**Drip loop:** A downward loop in the coax below the choke forces water to
drip off the cable before reaching the connector.  Always install a drip loop
on any outdoor coax connector.

### 9.5 Inspection and Maintenance

| Item                  | Interval  | Action                                |
|-----------------------|-----------|---------------------------------------|
| Visual check          | Annually  | Look for cracks, UV whitening, damaged tape |
| O-ring condition      | Annually  | Replace if compressed flat or cracked  |
| Connector check       | Annually  | Check for corrosion; retighten         |
| Coil check            | 5 years   | Open enclosure; inspect for moisture   |
| ASA enclosure         | 10 years  | Replace if brittle or cracked          |

**Replacement schedule:** Viton O-rings: 5–7 years outdoors.
Self-amalgamating tape: replace UV tape wrap every 3 years; inner seal typically
lasts 7–10 years.

---

<a name="chapter-10"></a>
## CHAPTER 10 — RFI TROUBLESHOOTING PROCEDURES

### 10.1 RFI Troubleshooting Philosophy

**Principle 1 — Identify the path, not just the source.**  A noise source
matters only if there is a conduction or radiation path to the receiver.
Block the path; you need not silence the source.

**Principle 2 — Work from the receiver backward.**  Start at the radio;
add chokes or filters at each successive point until noise disappears.

**Principle 3 — Use the antenna bypass test.** Disconnect the antenna; connect
a 50Ω dummy load.  If noise disappears: conducted path is through feedline or
ground connection.  If noise remains: noise is radiated and reaching the radio
directly (or via power line).

**Principle 4 — Measure before guessing.**  A clip-on RF current probe is the
most useful RFI diagnostic tool.  Clip it around each cable successively.
The cable carrying the most RF current is the primary conduction path.

### 10.2 Systematic RFI Diagnostic Procedure

**Step 1 — Characterize the noise:**
  a. Note frequency range (HF, VHF, specific bands)
  b. Note character (broadband hash, discrete tones, pulsed, periodic)
  c. Note correlation with house activity (appliances, lighting, computers)
  d. Note time of day pattern (neighborhood activity)

**Step 2 — Antenna bypass test:**
  a. Disconnect antenna; connect 50Ω dummy load to radio.
  b. If noise drops >20 dB: noise enters via antenna/feedline.
     Go to Step 4.
  c. If noise drops <5 dB: noise enters via power line or internal to radio.
     Go to Step 3.

**Step 3 — Power line test:**
  a. Run radio from battery (fully disconnect from AC mains).
  b. If noise drops: noise enters via power line.  Install common-mode choke
     on power supply primary cable (before the supply); or use a line filter.
  c. If noise persists on battery: noise is broadband and entering radio
     directly (shielding issue, not common-mode).

**Step 4 — Identify feedline current source:**
  a. Clip RF current probe around coax at shack bulkhead entry.
  b. Note current level (reference measurement).
  c. Install 8× Mix 31 snap-on beads at bulkhead.  Remeasure.
  d. If noise drops: common-mode current was entering at bulkhead.
     Source is outdoors (antenna environment).  Add wound toroid at antenna
     feedpoint.
  e. If noise unchanged: current path is inside the shack.
     Clip probe around individual cables inside shack to find which one carries
     the most current.

**Step 5 — Locate external source:**
  a. Drive or walk around neighborhood with handheld receiver (HT or SDR + whip).
  b. Noise should get louder as you approach source.
  c. Note GPS coordinates of maximum field strength.
  d. Common external sources: LED street lights, LED household bulbs, plasma TVs,
     switching power supplies, variable speed motor drives, solar grid inverters.

### 10.3 Symptom-Specific Procedures

**Symptom: S-meter deflects during transmit (RF in shack)**

Diagnosis: Common-mode current on feedline returning to shack.
Most common cause: unbalanced antenna (dipole center off-ground), or coax
touching ground at wrong point, or single-ended (Marconi) antenna with poor
ground system.

Remedy sequence:
1. Install FT-240-31, 10 turns at antenna feedpoint.
2. Verify coax route: no contact with ground for first 5m from feedpoint.
3. If problem persists: add second choke at shack entry.
4. Check that antenna is actually balanced (measure current on each leg).

---

**Symptom: Received HF noise level rises after dark / certain hours**

Diagnosis: Conducted or radiated noise from household electronics.
Timed noise sources: LED dimmers on timers, HVAC variable-speed drives,
neighbor's electronics.

Remedy sequence:
1. Battery test to isolate power line entry.
2. One by one, turn off breakers in your panel.  Note which breaker drop
   reduces noise.  That circuit is the conduction path.
3. Install common-mode choke on offending equipment power cable.
4. If noise is radiated from neighbor's equipment: nothing to do at your
   station except install bandpass receive filters.

---

**Symptom: 60Hz hum in receive audio, correlated with transmit power**

Diagnosis: Ground loop.  Common-mode TX current finding a path through
the AC mains safety ground back to antenna.

Remedy sequence:
1. Install FT-240-31 choke at feedpoint AND at shack entry.
2. Verify single-point ground: all station equipment grounds connect to ONE
   point at the station entrance ground rod.
3. Do NOT disconnect AC safety ground — this is a safety hazard.
4. Instead, install a 1:1 isolation transformer on audio paths between equipment.

---

**Symptom: TVI on neighbor's TV during HF transmission**

Diagnosis: HF fundamental or harmonic coupling into TV coax or power lines.
Most common on 15M (21 MHz) near TV channel harmonics.

Remedy sequence:
1. Identify TV frequency affected.  Cable TV: common-mode on house cable system.
   Antenna TV: HF radiation getting into TV antenna cable.
2. Offer to install snap-on beads on neighbor's TV cable (at TV end and antenna end).
3. Check transmitter harmonic output: should be <−50 dBc.
4. Add low-pass filter on transmitter output (Butterworth at 30 MHz).

---

**Symptom: Audio rectification in telephone/speaker/intercom**

Diagnosis: Strong RF field rectifying in audio semiconductor junctions.
Occurs at close range or very high power.

Remedy sequence:
1. Install ferrite chokes on all audio cables going to affected device
   (inside the device enclosure if possible).
2. Add 0.01 µF ceramic bypass cap across audio input terminals.
3. Reduce power or change antenna direction/location.
4. As last resort: install RF-tight enclosure around affected device.

### 10.4 RFI Measurement Record

**Station RFI log** — maintain before and after each modification:

| Date | Band | S-unit noise floor | Common-mode current | Modification made | Improvement |
|------|------|--------------------|--------------------|--------------------|-------------|
|      |      |                    |                    |                    |             |
|      |      |                    |                    |                    |             |
|      |      |                    |                    |                    |             |

Minimum documentation: band, noise floor (S-units or dBm), and modification.
Good documentation: add common-mode current (mA measured with clip probe) and
insertion loss of choke (dB attenuation).

---

<a name="chapter-11"></a>
## CHAPTER 11 — BAND-BY-BAND QUICK REFERENCE

### 11.1 Choke Selection by Band

| Band  | Freq    | Best choke choice                              | Turns | Notes                        |
|-------|---------|------------------------------------------------|-------|------------------------------|
| 160M  | 1.8 MHz | FT-240-31 ×2 stacked, 10t RG-8X               | 10    | High power: consider ugly balun |
| 80M   | 3.5 MHz | FT-240-31, 10t RG-8X                           | 10    | Single core excellent         |
| 40M   | 7.0 MHz | FT-240-31 or -43, 10t                          | 10    | Either mix adequate           |
| 30M   | 10.1 MHz| FT-240-43, 12t RG-8X                           | 12    | Mix 43 peak region            |
| 20M   | 14 MHz  | FT-240-43, 12t                                 | 12    | Best single core for 20M     |
| 17M   | 18 MHz  | FT-240-43, 12t                                 | 12    | Peak of Mix 43                |
| 15M   | 21 MHz  | FT-240-43, 12t                                 | 12    | Excellent                     |
| 12M   | 24.9 MHz| FT-240-43, 12t                                 | 12    | Excellent                     |
| 10M   | 28 MHz  | FT-240-43, 8–12t                               | 10    | Still good impedance          |
| 6M    | 50 MHz  | FT-240-43, 8t OR ugly balun (2" coil)         | 8     | Sleeve balun too long; use ferrite|
| 2M    | 144 MHz | Sleeve balun 500mm, or 6× FT-82-43 bead-on-coax| N/A | Sleeve preferred at feedpoint |
| 1.25M | 222 MHz | Sleeve balun 325mm, or bead-on-coax            | N/A  | Sleeve 325mm practical        |
| 70cm  | 432 MHz | Sleeve balun 167mm, or 8× FT-82-43           | N/A   | Both excellent                |
| 33cm  | 902 MHz | Sleeve balun 80mm, or 10× FT-50-61           | N/A   | Either; sleeve very small     |
| 23cm  | 1296 MHz| Sleeve balun 56mm, or bead-on-coax FT-50-61  | N/A   | Precision required for sleeve |

### 11.2 Field Day Quick Setup

**Minimum CMC kit for a multi-band field station:**

| Item                    | Qty | Bands covered    | Location              |
|-------------------------|-----|------------------|-----------------------|
| FT-240-31, 10t RG-8X   |  2  | 160M–10M         | Each HF antenna feedpoint |
| 8× Mix31 + 4× Mix43    | 12 beads | All HF   | Shack bulkhead entry, all coax |
| Sleeve balun 500mm      |  1  | 2M               | 2M antenna feedpoint  |
| Sleeve balun 167mm      |  1  | 70cm             | 70cm antenna feedpoint|

### 11.3 Maximum Choking Impedance Quick Reference

Based on best single-core configuration per band:

| Band | Best single choke | |Z_max| (Ω) | CMR @ 150Ω (dB) |
|------|------------------|----------|-----------------|
| 160M | FT-240-31, 12t   |  5,200   | 31.0            |
| 80M  | FT-240-31, 12t   |  7,600   | 34.1            |
| 40M  | FT-240-31, 12t   |  9,700   | 36.2            |
| 20M  | FT-240-43, 12t   |  5,819   | 31.9            |
| 10M  | FT-240-43, 12t   |  6,393   | 32.6            |
| 6M   | FT-240-43, 8t    |  2,163   | 23.2            |
| 2M   | Sleeve balun     |  6,000+  | 32.0+           |
| 70cm | Sleeve balun     |  8,000+  | 34.5+           |

**For maximum suppression at any single HF band:** Stack two FT-240-31 cores.
Combined |Z| approximately doubles; CMR improves 6 dB.

---

<a name="appendix-a"></a>
## APPENDIX A — FERRITE CORE SPECIFICATION TABLES

### A.1 Fair-Rite Material Properties (condensed)

| Mix | µ_i  | Tc (°C) | Peak R freq | Resistivity (Ω·cm) | Sat (mT) |
|-----|------|---------|-------------|-------------------|----------|
| 31  | 1500 | 230     | 2–20 MHz    | 1×10³             | 470      |
| 43  | 850  | 200     | 10–100 MHz  | 1×10⁵             | 320      |
| 61  | 125  | 350     | 100–1000 MHz| 1×10⁸             | 235      |
| 52  | 250  | 350     | 100–1500 MHz| 5×10⁷             | 320      |
| 77  | 2000 | 200     | 0.1–5 MHz   | 1×10²             | 460      |

Data source: Fair-Rite Products Corp., Material Specification sheets, 2023.

### A.2 Core Dimension Cross-Reference (FT = Amidon designation)

| Amidon | Fair-Rite P/N prefix | OD (mm) | ID (mm) | H (mm) |
|--------|---------------------|---------|---------|--------|
| FT-240 | 5943/5961/5977      | 61.0    | 35.6    | 12.7   |
| FT-193 | 5943/5961/5977      | 49.0    | 30.5    | 13.0   |
| FT-140 | 5943/5961/5977      | 35.5    | 22.9    | 12.7   |
| FT-114 | 5943/5961           | 29.1    | 19.1    |  7.5   |
| FT-82  | 5943/5961           | 21.0    | 13.0    |  6.4   |
| FT-50  | 5943/5961           | 12.7    |  7.7    |  6.4   |
| FT-37  | 5943/5961           |  9.5    |  4.7    |  3.2   |

### A.3 Snap-On Bead Cross-Reference

| Fair-Rite P/N    | Mix | Aperture (mm) | Cable max OD (mm) |
|-----------------|-----|--------------|-------------------|
| 0431167251       | 31  | 16.0         | 15 (RG-8 marginal)|
| 0443167251       | 43  | 16.0         | 15                |
| 0461167251       | 61  | 16.0         | 15                |
| 0431164281       | 31  | 24.4         | 23 (LMR-400 OK)  |
| 0231167251       | 31  | 7.9          | 7 (RG-58, RG-174) |

---

<a name="appendix-b"></a>
## APPENDIX B — CHOKING IMPEDANCE EQUATIONS AND DESIGN NOMOGRAPHS

### B.1 Key Design Equations

**Inductance of wound toroid:**
```
L (nH) = A_L (nH/N²) × N²
```

**Inductive impedance (below material loss frequency):**
```
|Z_L| = 2π × f × L = 2π × f × A_L × N²
```

**Common-mode rejection:**
```
CMR (dB) = 20 × log₁₀(1 + |Z_choke| / |Z_path|)
```

**Sleeve balun physical length:**
```
l (mm) = VF × 75,000,000 / (4 × f_MHz × 1000)
       = VF × 75,000 / (4 × f_MHz)
```
Where VF = 0.96 for air-spaced copper sleeve

**Ugly balun inductance (Wheeler):**
```
L (µH) = [r² × N²] / [9r + 10l]
```
Where r, l in inches; result in µH

**Coil self-resonant frequency:**
```
f_SR (MHz) = 1000 / (2π × √(L_µH × C_pF))
```

### B.2 Minimum Turns for Target Impedance (Mix 31, FT-240)

Target |Z| = 2000 Ω at frequency f using FT-240-31 (A_L = 1800 nH/N²):
  L_need = 2000 / (2π × f)
  N = √(L_need × 10⁹ / A_L)

| f (MHz) | L_need (µH) | N (turns) | Actual |Z| from table |
|---------|------------|-----------|----------------------|
|  1.8    |   176.9    |    314    | Use 12t+measured data |
|  3.5    |    90.9    |    225    | NOTE: above ~8 MHz, resistive |
|  7.0    |    45.5    |    159    | component dominates  |
| 14.0    |    22.7    |    112    | Use measured table   |

**IMPORTANT:** The above calculation is purely inductive and overestimates
required turns at HF because it ignores the large resistive (loss) component
of Mix 31 above ~3 MHz.  Measured impedance tables show only 10 turns
achieves |Z| = 4250 Ω at 14 MHz — far better than inductance formula predicts.
Always use measured data tables for choke design.

---

<a name="appendix-c"></a>
## APPENDIX C — COMMON-MODE REJECTION MEASUREMENT PROCEDURE

### C.1 Required Equipment
- VNA (NanoVNA-H4 or better; 2-port preferred)
- Two BNC tees or SMA tees
- Short coaxial patch cables ×4
- Device under test (choke)
- 50Ω termination ×2

### C.2 Common-Mode Measurement Fixture

The standard method (per W1JR / W2DU) uses a transmission measurement where
both ports of the VNA are driven in common mode:

```
         R_load (50Ω)
PORT1 ───┤├─────────────────────────────────── PORT2
(+)      C_block                               (+)
         
PORT1 ──┐                             ┌─── PORT2
(GND)   │  Both grounds joined ──── ──┘   (GND)
        │  (common mode injection)
        │  CHOKE connected in common-mode path here
        │  Outer conductors joined; center conductors = two ports
```

**Simplified fixture (single port):**
1. Short center conductor to outer conductor at the FAR end of choke.
2. Connect choke (far end shorted) to PORT1 of NanoVNA, configured for Z display.
3. NanoVNA reads directly the common-mode impedance = R + jX.
4. Read |Z| = √(R² + X²) at each frequency.
5. Calculate CMR per Appendix B.B.1 with estimated Z_path = 150 Ω.

### C.3 Field Measurement with RF Current Probe

**Equipment:** Clip-on RF current probe (AIM-TTi H-503, or homebrew using
small toroid and millivoltmeter)

**Procedure:**
1. Transmit into antenna at known power (e.g., 10W = 13.0 dBm nominal).
2. Clip probe around feed coax at antenna feedpoint.  Read current I_CM.
3. Common-mode impedance of path ≈ V_source / I_CM
   where V_source = √(P × Z) = √(10 × 50) = 22.4V RMS at 10W/50Ω.
4. Install choke.  Repeat measurement.
5. Improvement = 20 × log₁₀(I_before / I_after) dB.

**Typical results with good choke:**
  Before choke: I_CM = 5–50 mA (typical unbalanced feedpoint)
  After choke:  I_CM = 0.5–2 mA → improvement 8–20 dB per choke

---

<a name="appendix-d"></a>
## APPENDIX D — PARTS SOURCES AND EQUIVALENTS

### D.1 Primary Suppliers

| Supplier           | Products                       | URL / contact              |
|--------------------|-------------------------------|---------------------------|
| Palomar Engineers  | FT-240, FT-140, all mixes     | palomar-engineers.com     |
| Mouser Electronics | Fair-Rite cores and beads     | mouser.com                |
| Digi-Key           | Fair-Rite, Laird beads        | digikey.com               |
| Amidon Associates  | FT series cores, all mixes    | amidoncorp.com            |
| KF7P Metalwerks    | Pre-wound choke baluns        | kf7p.com                  |
| DX Engineering     | Coaxial chokes, assembled     | dxengineering.com         |
| Fair-Rite Products | Manufacturer, all materials   | fair-rite.com             |

### D.2 Core Equivalents

| Amidon / Palomar | Fair-Rite P/N (Mix 31) | Fair-Rite P/N (Mix 43) |
|-----------------|----------------------|----------------------|
| FT-240-31       | 5943002701           | 5943002701 (Mix43 request)|
| FT-240-43       | 5943002401           | (order -43 suffix)   |
| FT-140-43       | 5943001401           |                      |
| FT-82-43        | 5943000801           |                      |
| Snap-on 1.125"  | 0431167251 (Mix31)   | 0443167251 (Mix43)   |
| Snap-on 2.4"    | 0431164281 (Mix31)   | 0443164281 (Mix43)   |

### D.3 Weatherproofing Materials

| Item                      | Brand/Type            | Source              |
|---------------------------|-----------------------|---------------------|
| Self-amalgamating tape    | 3M 23 Scotch          | Electrical suppliers|
| UV-resistant vinyl tape   | 3M Temflex 1700 (black)| Hardware stores    |
| Silicone RTV sealant      | Dow Corning 795       | Hardware stores     |
| Viton O-ring 3mm CS       | Parker 2-131 or equiv | McMaster-Carr       |
| ASA filament              | Polymaker PolyLite ASA| Amazon, Filamentive |

### D.4 Test Equipment

| Item                      | Minimum spec           | Recommended model          |
|---------------------------|------------------------|---------------------------|
| VNA (component analyzer)  | 1–300 MHz, 2-port      | NanoVNA-H4 (< $50)        |
| RF current probe          | 1–300 MHz clip-on      | AIM-TTi H-503 or homebrew |
| Audio frequency analyzer  | For audio filter tests  | PC soundcard + REW software|
| Field strength meter      | 1–500 MHz handheld     | TinySA or similar SDR      |

---

*End of Technical Manual TM-CMC-001 Revision A*

*Data spreadsheet:* `data/ferrite_impedance_table.ods` (8 worksheets)
*Schematic files:*
- `schematics/sch_toroid_choke_hf.txt` — FT-240 wound chokes
- `schematics/sch_snapon_beads.txt` — Snap-on bead stacks
- `schematics/sch_ugly_balun_and_bead_coax.txt` — Ugly balun + bead-on-coax
- `schematics/sch_sleeve_balun_vhf_uhf.txt` — VHF/UHF sleeve balun

*3D print files:*
- `openscad/enclosure_choke_hf.scad` — Weatherproof HF choke enclosure
- `openscad/mounting_brackets.scad` — Mast, wall, tower, and bead rail brackets

*Related documents:*
- TM-SG-001 — VFO/Signal Generator (for impedance measurement signal source)
- TM-AF-001 — Audio Filters (for received noise reduction)
