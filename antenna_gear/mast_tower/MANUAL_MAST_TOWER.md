# TECHNICAL MANUAL
## TM-MAST-001 Rev A
### PORTABLE ANTENNA MAST AND TOWER SYSTEMS
#### Telescoping Masts, Crank-Up Tower, Pneumatic Lift, Guy Systems, Rotator Integration

---

**DISTRIBUTION:** Unrestricted  
**CLASSIFICATION:** Unclassified  
**MANUAL NUMBER:** TM-MAST-001 Rev A  
**DATE:** 2026-04-26  
**AUTHORITY:** Field Engineering Reference — Portable HF/VHF Antenna Elevation Systems

---

## SAFETY SUMMARY

> **WARNING**  
> A telescoping antenna mast in or near a thunderstorm is a LIFE-THREATENING hazard.  
> Lower all masts to horizontal before any thunderstorm approaches within audible range (within 10 miles, if you can hear thunder). Do not erect masts within 30 minutes after the last audible thunder.

> **WARNING**  
> NEVER erect or lower a mast near overhead power lines.  
> Minimum safe clearance: mast height × 1.5 from any overhead wire.  
> Contact with a power line is invariably fatal.

> **CAUTION**  
> Masts > 20 ft require a minimum 2-person crew for raising and lowering.  
> Never work alone with masts > 15 ft in unsupported operations.

> **CAUTION**  
> Inspect all guy wires, anchors, and locking hardware before each raise.  
> Failure of any single guy anchor can collapse the entire mast without warning.

---

## TABLE OF CONTENTS

1. [Introduction and System Overview](#chapter-1)
2. [Site Selection and Pre-Installation Survey](#chapter-2)
3. [Fiberglass Telescoping Mast — Installation and Operation](#chapter-3)
4. [Aluminum Telescoping Mast with Guy Stabilization](#chapter-4)
5. [Pneumatic Mast Systems](#chapter-5)
6. [Crank-Up Tower — Assembly and Operation](#chapter-6)
7. [Guy Wire Anchor Systems](#chapter-7)
8. [Rotator Mounting and Control](#chapter-8)
9. [Tilt-Over Base Installation and Operation](#chapter-9)
10. [Wind Load Theory and Design Calculations](#chapter-10)
11. [3D-Printed Hardware — Fabrication and Inspection](#chapter-11)
12. [Test and Alignment Procedures](#chapter-12)
13. [Fault Isolation and Troubleshooting](#chapter-13)
14. [Disassembly, Transport, and Storage](#chapter-14)
15. [Safety Standards and Emergency Procedures](#chapter-15)

**Appendices:**  
A. [Wind Load Quick Reference Tables](#appendix-a)  
B. [Knot and Wire Termination Reference](#appendix-b)  
C. [3D-Printed Part Specifications and Inspection Criteria](#appendix-c)  
D. [CYD Mast Monitor Firmware Reference](#appendix-d)  
E. [Parts List and Sources](#appendix-e)

---

<a name="chapter-1"></a>
## CHAPTER 1 — INTRODUCTION AND SYSTEM OVERVIEW

### 1.1 Purpose

This manual provides complete installation, operational, maintenance, safety, and troubleshooting procedures for the TM-MAST-001 series of portable antenna elevation systems. These systems are designed for field deployment by amateur radio operators, emergency communication teams, and field survey teams requiring rapid, repeatable antenna elevation without permanent infrastructure.

### 1.2 System Configurations

**TM-MAST-001A — Fiberglass Telescoping Mast (20–40 ft)**  
Lightweight, non-conductive, corrosion-resistant. Optimal for portable field work where total system weight is critical. Sections nest for transport in a 2m carry bag. Single-operator raise to 20 ft; two-operator preferred above 20 ft.

**TM-MAST-001B — Aluminum Telescoping Mast with Guy Stabilization**  
Higher load capacity; supports directional antennas with rotator. Requires guying at heights > 15 ft. More resistant to side-loads. Standard military push-up mast configuration.

**TM-MAST-001C — Pneumatic Rapid-Deployment Mast (20 ft)**  
Air-pressure extension; mast rises in < 30 seconds. Requires compressed air source (80–120 PSI). Field compressor or pre-charged cylinder. Self-contained operation; single operator.

**TM-MAST-001D — Crank-Up Tower (30–40 ft)**  
Gear-driven cable raise; supports heavy antennas (up to 15 kg) with rotator. Nested lattice sections; requires vehicle transport. Two-person erection. Base tripod or hitch-mount options.

### 1.3 System Components — Common to All Configurations

| Component | Function | Drawing Reference |
|-----------|----------|------------------|
| Mast sections | Primary structural elevation | spec_* files |
| Guy wire collars | Lateral stabilization attachment | mast_collars.scad |
| Guy wires | Lateral stabilization | spec_guy_anchor_systems.txt |
| Ground anchors | Guy wire earth attachment | guy_tensioners_anchors.scad |
| Tensioners | Guy wire pre-tension adjustment | guy_tensioners_anchors.scad |
| Base plate | Mast-to-earth interface | base_plates_mounts.scad |
| Top hardware | Antenna and rotator attachment | base_plates_mounts.scad |

### 1.4 Design Standards

Structural design follows:
- EIA/TIA-222-H: *Structural Standard for Antenna Supporting Structures and Antennas*  
- ASCE 7-22: *Minimum Design Loads for Buildings and Other Structures*  
- ANSI B30.5: *Mobile and Locomotive Cranes* (rigging practices)

All calculations use minimum 3.0× safety factor on structural components, 2.5× on hardware.

---

<a name="chapter-2"></a>
## CHAPTER 2 — SITE SELECTION AND PRE-INSTALLATION SURVEY

### 2.1 Site Selection Criteria

Before deploying any mast system, the following site conditions must be evaluated:

**MANDATORY CHECKS:**
- [ ] No overhead power lines within (mast height × 1.5) of intended mast location
- [ ] No buried utilities within anchor placement area (call 811 / USA Dig Safe where applicable)
- [ ] Soil capable of supporting required anchor loads (test by probing)
- [ ] Sufficient space for guy wire anchor radius (minimum: mast height × 0.5 from base)
- [ ] Vehicle or pedestrian clearance in mast collapse zone (all directions, mast height radius)
- [ ] No prohibited antenna structures (check local regulations, HOA, federal land)

**PREFERRED CONDITIONS:**
- Open field with firm undisturbed soil at anchor points
- Prevailing wind direction known; orient mast approach for upwind raise
- Within reach of radio operating position (< 25m feedline run)
- Not in natural drainage path (flooding risk during rain events)
- Tree or terrain windbreak available (reduces effective wind load 15–30%)

### 2.2 Soil Assessment

The holding capacity of ground anchors depends entirely on soil condition.

| Soil Type | Description | Anchor Capacity | Notes |
|-----------|-------------|-----------------|-------|
| Dense clay | Heavy soil, forms shapes | High | Best; use standard anchors |
| Loam | Mixed organic/mineral | Moderate | Standard anchors; full depth |
| Sandy soil | Granular, loose | Low | Use larger plate; deeper |
| Saturated | Wet, standing water | Very Low | Double up anchors or deadman |
| Fill/disturbed | Recent excavation | Variable | Probe deeply; avoid if possible |
| Rocky | Bedrock near surface | Unsuitable | Surface plate + masonry anchors |

**FIELD SOIL PROBE TEST:**
Push a 6mm steel rod by hand. If it penetrates >300mm easily: soft soil, use large anchors. If hard by 200mm: dense, standard anchors. If deflects on rock at <100mm: use surface mount.

### 2.3 Wind Assessment

Observe local conditions for a minimum of 10 minutes before raising:

| Beaufort | Speed | Description | Mast Guidance |
|----------|-------|-------------|---------------|
| 0–3 | < 12 mph | Calm to gentle breeze | All mast heights OK |
| 4 | 13–18 mph | Moderate breeze | Secure guys before raise |
| 5 | 19–24 mph | Fresh breeze | 2-person raise; >20 ft not recommended |
| 6 | 25–31 mph | Strong breeze | Experienced operators only; 20 ft max |
| 7+ | > 32 mph | Near gale | **Do not raise. Lower/secure existing masts.** |

### 2.4 Pre-Deployment Checklist

```
□ All mast sections inspected — no cracks, delamination, or bent sections
□ All hose clamps / locking hardware functional
□ Guy wires: crimps, knots, or splices intact; no fraying; no kinks
□ All ground anchors straight, undamaged
□ Tensioners free-moving; threads lubricated with dry PTFE
□ Base plate attachment hardware complete and correct count
□ Antenna weight within rated capacity for mast height
□ Rotator cable continuity checked (if applicable)
□ CYD monitor charged and operational (if deployed)
□ Weather forecast reviewed — no thunderstorm within 4 hours
□ Operating crew briefed on safety zones and emergency lower procedure
```

---

<a name="chapter-3"></a>
## CHAPTER 3 — FIBERGLASS TELESCOPING MAST

### 3.1 Theory of Operation

Pultruded fiberglass tubes nest concentrically. Each inner section slides inside the next larger outer section with 1–3mm annular clearance. The joints are friction-held and compressed by hose clamps. A collar at the joint face establishes maximum extension and provides a hard stop.

Non-conductivity is the primary operational advantage. The mast can be placed directly beneath or adjacent to antenna elements without RF coupling. Unlike metal masts, no static buildup or RF grounding considerations exist for the mast structure itself.

### 3.2 Section Extension Procedure

> **CAUTION:** Never extend a section without locking the section below it first. Cascading extension without locking allows the entire mast to telescope closed under antenna load.

1. **Position the nested mast assembly vertically** in or near the base plate socket. One operator steadies the mast.
2. **Extend Section 5** (second from bottom) upward 1.2m, leaving 300mm overlap inside Section 6. Lock with hose clamp; torque until firm, not crushing.
3. **Continue upward**, extending Section 4, 3, 2, then 1 in sequence.
4. **Before extending the top section**, attach all antenna hardware. Working at ground level eliminates ladder work.
5. **Install guy ring collars** at the required levels (see guying schedule, Chapter 7).
6. **Final check**: shake base section — verify no section rotates or drops.

### 3.3 Section Locking Torque

| Clamp Size | Tube OD | Correct Torque | Feel |
|------------|---------|----------------|------|
| #24 | 44–57mm | 2–3 N·m | Firm finger-tight + 1/4 turn |
| #28 | 57–76mm | 3–4 N·m | Firm; nut driver not socket wrench |

Over-torquing hose clamps on fiberglass causes surface crazing (white stress marks). If crazing appears within 50mm of clamp location, reduce clamp torque and inspect for structural damage.

### 3.4 Guying Schedule

```
HEIGHT   LEVELS   GUY RADIUS
20 ft    1 level  at 60% height; radius = mast height × 0.7
30 ft    2 levels at 40% and 75% height; same radius formula
40 ft    3 levels at 33%, 60%, and 85% height
```

---

<a name="chapter-4"></a>
## CHAPTER 4 — ALUMINUM TELESCOPING MAST

### 4.1 Structural Properties and Load Limits

Aluminum telescoping masts (6061-T6) offer substantially higher section modulus than equivalent fiberglass, but are electrically conductive and heavier. When used as an antenna support, the mast must be electrically isolated from the antenna element at the top — typically by a fiberglass or PVC stub mast of 500–1000mm mounted atop the aluminum mast.

**CRITICAL:** A 30-foot aluminum mast in 70 mph wind without guying will yield in the base section. Guying is not optional for heights >15 ft. See wind load calculations in Chapter 10 and Appendix A.

### 4.2 Locking Pin System

Military-surplus and commercial push-up masts use spring-loaded locking pins. Each section is marked with a color band indicating the extension order. Common color coding:

| Color | Section | Notes |
|-------|---------|-------|
| Red | Top | Extend first when pushing up; retract last |
| Orange | 2nd | |
| Yellow | 3rd | |
| Green | 4th | |
| Blue | 5th | |
| White | Base | Fixed; never extended |

Push pins inward against spring to release section; extend; release pin into locking hole.

### 4.3 Guy Wire Installation

**Pre-tension each guy wire before the next level is extended.** Attempting to tension guys after the mast is fully extended is more difficult and less safe.

1. Extend and lock bottom 2 sections.
2. Install Level 1 guy ring collar at appropriate height.
3. Connect pre-cut guy wires to collar eyes using snap hooks or Nicopress loops.
4. Stake or anchor Level 1 guy wires; tension to hand-tight (no sag).
5. Continue extending sections through Level 2 collar height.
6. Install Level 2 collar; attach guys; anchor; tension.
7. Extend to full height; attach top hardware.
8. Tension all guy levels starting from top, working down.
9. Final plumb check; adjust opposing guys to center.

### 4.4 Plumb and Alignment

A mast that is not plumb (vertical) applies off-axis bending loads to all sections and anchors, reducing structural margin and increasing fatigue. Measure plumb in two perpendicular planes.

**Method — Plumb Bob:**  
Suspend a plumb bob on a string from the top section. The string should hang directly over the base plate center point when the mast is plumb. Measure any offset. Adjust the windward/leeward guy pair for one axis, then repeat for the perpendicular axis.

**Method — Spirit Level:**  
Apply a 4-foot spirit level to each face of the base section. Adjust until bubble centers in both planes.

**Method — CYD/MPU-6050 Tilt Monitor:**  
MPU-6050 IMU clipped to base section. CYD displays tilt angle in X and Y axes. Adjust guys until both axes read < 0.5° from vertical.

**Acceptable plumb tolerance:** ± 1° of vertical (all methods).

---

<a name="chapter-5"></a>
## CHAPTER 5 — PNEUMATIC MAST SYSTEMS

### 5.1 Principle of Operation

In a pneumatic mast, each nested aluminum tube section acts as a piston inside the next larger tube (the cylinder). Compressed air introduced at the base manifold acts on the annular cross-sectional area between piston and cylinder, producing an upward force. Sections extend sequentially from top to bottom (smallest annular area requires least pressure; extends first).

Once fully extended, spring-loaded ball detents or manual lock pins engage notches in each section, providing mechanical support independent of air pressure. Air pressure is then released; the mast stands on mechanical locks.

### 5.2 Air Supply System

**SCBA/Paintball Cylinder Approach (recommended for field use):**
- 68 cubic inch / 4500 PSI aluminum cylinder
- Single-stage regulator: 4500 PSI → 0–150 PSI adjustable output
- Needle valve for controlled lowering
- 3-way valve: RAISE / HOLD / LOWER
- Inline 0–160 PSI gauge

**12V Compressor Approach:**
- Portable tire inflator (e.g., Viair 88P, 150 PSI capable)
- 12V vehicle or battery power
- Integrated 0–160 PSI gauge
- Time to pressure: ~60 seconds for 20 ft system

**Minimum supply pressure for extension:**
- Calculate: P_min = F_gravity / A_annular  
- F_gravity = (mast section weight + antenna weight) × g  
- Typical 20 ft mast: 4 kg × 9.81 = 39 N; A_annular(top section) ≈ 0.000267 m²  
- P_min = 39 / 0.000267 = 146 Pa = 0.021 PSI (gravity only — negligible)  
- **Working pressure 80–100 PSI provides very high force margin; regulates extension speed**

### 5.3 Seal Maintenance

Piston ring seals (PETG, Buna-N O-ring, or PTFE) require periodic inspection and lubrication.

**Service interval:** Every 50 raises or 6 months, whichever comes first.

**Lubrication procedure:**
1. Lower mast fully; release all air pressure.
2. Disassemble sections from top.
3. Wipe each piston ring groove clean with lint-free cloth.
4. Apply thin film of silicone grease (Molykote 111 or equivalent) to O-ring and piston ring outer surface.
5. **Do not use petroleum grease** — degrades Buna-N O-rings within months.
6. Reassemble; test extend/retract 3 times before operational use.

**Leak test:**
Pressurize to 80 PSI; apply soapy water to all joints. Any bubble formation = leak. Re-lubricate or replace seal ring before operational use.

### 5.4 Pneumatic Safety

> **WARNING**  
> Rapid seal failure with mast extended causes uncontrolled collapse.  
> The mast does not fall to one side — sections telescope rapidly downward.  
> Maintain the minimum 1.0× mast-height clearance around the base at all times during pneumatic operation.

**Pre-raise checklist:**
```
□ All personnel clear of mast radius (≥ mast height)
□ Air supply pressure confirmed at 80–120 PSI before opening raise valve
□ Pressure relief valve functional (test pop at 125 PSI before field deployment)
□ Lower mast cable/safety strap attached (prevents free-fall on seal failure)
□ Antenna and cable weight within rated section capacity
```

---

<a name="chapter-6"></a>
## CHAPTER 6 — CRANK-UP TOWER

### 6.1 Structural Description

The crank-up tower consists of three nested triangular lattice sections:

- **Section A (base):** Fixed to base; largest face width; houses winch drum.
- **Section B (middle):** Slides up inside Section A when cranked; houses intermediate pulley.
- **Section C (top):** Slides up inside Section B; antenna/rotator platform at top.

Sections lock at full extension via stainless locking bars through chord faces. The winch cable attaches to the top of Section C and routes down through pulleys.

### 6.2 Winch Operation

**RAISING:**
1. Disengage ratchet pawl lock (pull release lever or flip latch).
2. Begin cranking clockwise (standard configuration).
3. Section B extends first until it locks; winch continues pulling Section C.
4. Section C extends to full height; lock pins engage automatically (spring detent).
5. Re-engage ratchet pawl.
6. **Never release the crank without first engaging the ratchet.** Free-fall will break the cable and collapse the tower.

**SAFE CRANKING SPEED:** One full revolution per 2 seconds (comfortable sustained pace).

**LOWERING (CONTROLLED DESCENT ONLY):**
1. Ensure ratchet pawl is functional.
2. Release pawl; maintain hand on crank.
3. Pay out cable slowly; one revolution per 3–4 seconds.
4. Never allow free-spool. Tower will collapse at terminal velocity.

### 6.3 Cable Inspection

The crank cable is a critical safety component. Inspect before every raise.

| Defect | Threshold | Action |
|--------|-----------|--------|
| Broken wires | Any broken strand | Replace cable |
| Kinking | Any permanent kink | Replace cable |
| Corrosion | Surface rust = warn; pitting = replace | As noted |
| Swage/crimp | Any movement detected | Replace terminal |
| Sheave groove | Wear groove > 1mm deep | Replace sheave |

**Cable replacement interval:** Every 3 years or 200 raises, whichever comes first. Cable failure causes complete tower collapse.

### 6.4 Guy Wire Requirements — Crank-Up Tower

The crank-up tower is self-supporting during cranking operations but must be guyed for sustained antenna loading at height.

**Required:** At least 2 levels of guying for 30 ft tower with antenna > 2 kg.  
**Recommended:** Guy Level 1 (Section A top) before beginning crank-up; Level 2 (Section B top) once Section B is locked.

---

<a name="chapter-7"></a>
## CHAPTER 7 — GUY WIRE ANCHOR SYSTEMS

### 7.1 Anchor Selection by Application

Correct anchor selection is as critical as anchor installation. An undersized anchor that pulls out in a wind event will collapse the mast — potentially on personnel or equipment.

**Rule:** Anchor holding capacity ≥ 3.0 × maximum operating guy wire tension.

See Appendix A for guy tension by mast height and wind speed.

### 7.2 Screw Anchor Installation

Screw anchors are the preferred portable anchor for soft-to-medium soil.

**INSTALLATION PROCEDURE:**
1. Insert the T-handle adapter (printed, guy_tensioners_anchors.scad) over the anchor head.
2. Position anchor at correct radius from mast base.
3. Push down firmly; rotate clockwise. The helical plate begins to bite.
4. Drive the anchor to full depth — the helical plate must be below undisturbed soil level (below the topsoil zone, ≥ 200mm).
5. **Capacity check:** With anchor installed, attempt to pull upward with both hands. If detectable movement (> 5mm): reinstall deeper or supplement with second anchor.
6. Attach guy wire to anchor eye; route wire at correct angle (not sharply bent around anchor).

**DO NOT:**
- Drive at an angle toward the mast (reduces pullout capacity dramatically)
- Drive into disturbed soil (recent trenching, digging)
- Reuse an anchor installed in soft soil without re-probing capacity

### 7.3 Guy Wire Tensioning Procedure

> **PRINCIPLE:** Tension all guy wires at one level simultaneously. Tensioning one wire while others are slack puts the mast under eccentric load and can cause it to lean.

1. Connect all wires at a given level to their anchors.
2. Pre-tension each wire by hand: take out all visible sag. All wires should be equally taut.
3. Starting with the **top** guy level (most critical for mast plumb), incrementally tighten opposing wires alternately.
4. Check plumb; adjust as needed.
5. Work **downward** through levels.
6. Final pre-tension target: 75–100 N per wire (about 7.5–10 kgf, roughly the force of a 10 kg downward pull on the wire).
7. Lock all tensioner jam nuts.

**Correct wire tension by feel:**  
Pluck the wire like a guitar string. At 100 N tension on a 6m wire, the audible frequency is approximately 20–30 Hz (a low bass note). Wire that makes no sound is too loose; wire that makes a high-pitched ring is likely overtensioned.

### 7.4 In-Line Tensioner Operation

The printed in-line tensioner (Module 1, guy_tensioners_anchors.scad) uses an M6 threaded rod captured in each end. Rotation of the body increases or decreases tension.

- Hex body accepts an 8mm or 10mm open-end wrench.
- Clockwise rotation (from above): increases wire length → loosens tension.
- Counter-clockwise: decreases wire length → increases tension.
- Jam nut (M6 nut) on each rod end. Tighten jam nuts after final adjustment to prevent vibration-induced loosening.

**Tensioner inspection:** Before each use, apply body weight to guy wire while grasping tensioner. Any movement of the threaded rod in the body indicates stripped thread — replace immediately.

### 7.5 Emergency Re-Anchoring

If a guy anchor begins to pull out during operation (indicated by mast lean, visible anchor movement, or audible pop):

**IMMEDIATE ACTION:**
1. Reduce antenna wind exposure if possible (lower antenna if rotator has clear path to min-area heading).
2. **Do not attempt to re-drive a pulling anchor under load.** It will not hold.
3. Lay an additional anchor in-line with the failing one, 600mm further out. Connect with a short extension of wire.
4. Tension the new anchor before releasing load on the failing anchor.
5. Drive a supplemental anchor at 45° offset angle; connect in parallel.

If the mast begins to lean significantly (> 5°): **prepare to lower immediately.** A leaning mast under wind load will fall.

---

<a name="chapter-8"></a>
## CHAPTER 8 — ROTATOR MOUNTING AND CONTROL

### 8.1 Rotator Installation at Mast Top

**Step 1 — Install rotator adapter:**
Mount the rotator tower top plate adapter (base_plates_mounts.scad: rotator_tower_topplate) or rotator saddle cage (mast_collars.scad: rotator_saddle_cage) to the mast before raising.

**Step 2 — Rotator placement:**
- Mount rotator body in U-bolt saddle or cage.
- Ensure rotator output shaft vertical axis is aligned with mast axis (within 2°).
- Misalignment causes binding, excess motor current, and premature rotator failure.

**Step 3 — Antenna mast stub:**
- Insert 38mm OD × 1.2m aluminum stub mast into rotator output shaft collar.
- Tighten both U-bolts evenly; check stub is vertical.
- Attach antenna to top of stub mast.

**Step 4 — Cable routing:**
- Run rotator cable down mast exterior.
- Secure with strain relief clips (mast_collars.scad) every 600mm.
- Leave a 200mm drip loop at the rotator connector.
- At mast base: coil 300mm service loop before running to controller.

### 8.2 CYD Rotator Controller Setup

The ESP32 + CYD controller provides touchscreen bearing display, automatic tracking, and BLE interface to logging software.

**Initial setup:**
1. Power on CYD; verify display initializes (white background → home screen).
2. Navigate to CALIBRATE → SET NORTH.
3. Point antenna beam toward **true north** (GPS or Polaris bearing).
4. Press SET NORTH. Controller stores heading offset in NVS.
5. Navigate to CONFIG → DECLINATION. Enter local magnetic declination (Merced CA: 12.5° East = +12.5).
6. Rotate antenna to known bearing (use satellite dish or GPS bearing). Verify display matches ± 3°.

**Touch controls:**
- **CCW / CW buttons:** Manual rotation in 5° increments. Hold for continuous rotation.
- **STOP:** Halt rotation immediately; hold bearing.
- **Compass rose touch:** Tap desired bearing on the displayed compass; antenna slews automatically.
- **SET NORTH:** Calibration only (see above).

**BLE pairing:**  
Device name: `MAST-RTR-001`. UUID matches TM-BMS-001 peripheral convention. Python client script in Appendix D connects from laptop for automated logging.

---

<a name="chapter-9"></a>
## CHAPTER 9 — TILT-OVER BASE INSTALLATION AND OPERATION

### 9.1 Base Installation

The tilt-over base consists of a fixed ground plate (welded steel, 600mm × 400mm × 8mm) with a heavy hinge. The hinge pin axis is perpendicular to the intended raise direction.

**Ground plate anchoring:**  
- Concrete mounting: 4× M16 × 100mm Wedge anchors; installed per manufacturer specs. Cure time 24 hours before loading.
- Soil mounting: 4× ground screws (22mm × 750mm) at corners. Test pullout before use.

**Hinge alignment:**  
Level the fixed base plate with wedges (plumb_wedge() in base_plates_mounts.scad) before anchoring. Once anchored, the hinge axis must be horizontal within ± 0.5° or the mast will arc laterally during raise, increasing structural loading.

### 9.2 Raise Procedure (2-Person Minimum)

> **RULE:** Assign roles before beginning the raise. Operator 1 = haul line. Operator 2 = tag line near mast. A third person on a second tag line is strongly preferred for masts > 25 ft.

**With gin pole:**
1. Assemble gin pole to mast base section (side mount, perpendicular to raise).
2. Route haul rope through pulley at gin pole tip; secure other end at mast 2/3 height.
3. Confirm all guy wires attached at collars; far-side wires staked and slack.
4. Operator 1 hauls rope (2:1 MA via pulley). Rate: hand-over-hand, controlled.
5. Operator 2 manages near-side tag line: controls lateral swing.
6. Stop at 30°, 60°, 80° to allow guy wire slack adjustment on leeward side.
7. At 85°–90°: insert lock bolts at hinge. Torque M16 to 45 N·m.
8. Tension upper guy level; lower; tension lower level.

**Required haul force (gin pole, 2:1 MA):**  
- 20 ft mast (8 kg mast + 3 kg antenna): horizontal moment ≈ 550 N·m at 2m gin pole arm = 275 N pull (28 kgf). Manageable for one person.  
- 30 ft mast (12 kg + 5 kg): ≈ 800 N pull at gin pole. **Two-person haul required.**

### 9.3 Lower Procedure

1. Reduce wind exposure: rotate antenna to minimum area heading.
2. Remove lock bolts (2×) at tilt hinge.
3. Connect haul rope; apply tension before removing bolts to prevent free-fall.
4. Slowly pay out haul rope: one hand over hand at a time.
5. As mast approaches 30°: place padded support (sawhorse, foam) under antenna.
6. At horizontal: support antenna end; avoid impact on ground hardware.

### 9.4 Tilt Hinge Maintenance

| Component | Interval | Procedure |
|-----------|----------|-----------|
| Hinge pin | Before each raise | Apply anti-seize compound (Loctite 771) to pin |
| Pin cap | Before each raise | Install printed cap (tiltover_hinge_cap()); prevents grit |
| Lock bolts | Every 10 raises | Inspect threads; replace if any thread damage |
| Base plate | Annually | Inspect welds; check anchor bolt torque |

---

<a name="chapter-10"></a>
## CHAPTER 10 — WIND LOAD THEORY AND DESIGN CALCULATIONS

### 10.1 Wind Pressure Fundamentals

Wind pressure on a structure is calculated from Bernoulli's equation for dynamic pressure:

```
q = 0.5 × ρ_air × V²
```

At sea level, ρ_air = 1.225 kg/m³:

```
q = 0.613 × V²   [Pa, V in m/s]
q = 0.00256 × V² [psf, V in mph]
```

This is the **free-stream dynamic pressure** — the theoretical maximum pressure on a flat plate perpendicular to wind.

### 10.2 Drag on Cylindrical Mast Sections

A circular cylinder is not a flat plate. The drag coefficient C_d accounts for flow attachment, separation, and wake formation:

```
F_drag = q × C_d × A_projected
```

- **C_d = 1.2** for mast tubes (accounts for surface roughness and Reynolds number effects at field wind speeds)
- **A_projected = D × L** (diameter × section length)

### 10.3 Overturning Moment and Base Reactions

For a guyed mast, the base is a pinned reaction point. Wind loads above the lowest guy level create bending between the lowest guy attachment and the base. The critical section is the base section.

```
M_base = Σ(F_i × h_i)  [summed for sections below lowest guy]
σ_bending = M_base / S  [S = section modulus of base tube]
```

**For 6061-T6 aluminum:** Yield stress = 276 MPa. Minimum design safety factor = 2.0 against yield.

### 10.4 Guy Wire Tension Calculation

For a single guy level at height h_g with anchor radius r_a:

```
Guy angle: θ = arctan(h_g / r_a)
Horizontal reaction: T_H = F_wind_above (windward guy only)
Guy tension:         T_guy = T_H / sin(θ)
Vertical component:  T_V = T_guy × cos(θ)  [adds to mast compression]
```

At 70% radius (preferred): θ = 35°; sin(35°) = 0.574

This means:  
`T_guy = F_wind / 0.574 = 1.74 × F_wind`

A horizontal wind force of 500 N above the guy level requires a guy tension of 870 N. **This is why anchor capacity must far exceed the apparent wind load.**

### 10.5 Complete Example — 30-Foot Aluminum Mast, 70 mph Wind

```
Wind pressure: q = 0.613 × 31.3² = 600 Pa
Height correction: K_z = 1.2

SECTION LOADS:
  Sect 1 (38mm, 1.5m):  F = 600 × 1.2 × 0.057 = 41 N at h = 8.5m
  Sect 2 (44mm, 1.5m):  F = 600 × 1.2 × 0.066 = 47 N at h = 7.0m
  Sect 3 (50mm, 1.5m):  F = 600 × 1.2 × 0.075 = 54 N at h = 5.5m
  Sect 4 (57mm, 1.5m):  F = 600 × 1.2 × 0.086 = 62 N at h = 4.0m
  Sect 5 (63mm, 1.5m):  F = 600 × 1.2 × 0.095 = 68 N at h = 2.5m
  Antenna (EPA=0.10m²): F = 600 × 1.2 × 0.10  = 72 N at h = 9.0m

TOTAL WIND FORCE: 344 N
OVERTURNING MOMENT (unguyed): 2,750 N·m → EXCEEDS YIELD → MUST GUY

WITH GUYS AT 3.6m (Level 1) AND 7.2m (Level 2):
  Section below Level 1: Sections 4, 5 only
  M_base = 62×4.0 + 68×2.5 = 418 N·m
  σ_base = 418 / (8.09×10⁻⁶) = 52 MPa
  Safety factor: 276 / 52 = 5.3 — ADEQUATE

UPPER GUY TENSION (Level 2, supports loads above 7.2m):
  F_above = 41+47+72 = 160 N (sections 1, 2 + antenna)
  T_guy = 160 / sin(35°) = 279 N per windward guy wire
  Anchor required: 279 × 3.0 (SF) = 837 N minimum

LOWER GUY TENSION (Level 1, supports loads above 3.6m):
  F_above = 41+47+54+72 = 214 N (sections 1–3 + antenna)
  T_guy = 214 / 0.574 = 373 N
  Anchor required: 373 × 3.0 = 1,120 N minimum
```

### 10.6 Design Wind Speed Selection

| Application | Design Speed | Comment |
|-------------|-------------|---------|
| Casual POTA/SOTA portable | 50 mph | Short-duration; operator present |
| EMCOMM field deployment | 70 mph | 8-hour unattended ops |
| Semi-permanent field station | 90 mph | Extended deployment |
| Permanent installation | 100+ mph per local code | Structure permit required |

---

<a name="chapter-11"></a>
## CHAPTER 11 — 3D-PRINTED HARDWARE FABRICATION AND INSPECTION

### 11.1 Print Specifications

All structural printed parts use:

| Parameter | Value |
|-----------|-------|
| Material | PETG (general); ASA (>55°C ambient) |
| Perimeters | 5 |
| Top/Bottom layers | 6 |
| Infill | 50% gyroid |
| Layer height | 0.2mm |
| Nozzle | 0.4mm (0.6mm for faster large prints) |
| Bed temperature | 80°C (PETG) / 100°C (ASA) |
| Enclosure | Recommended for ASA; optional for PETG |

**Do NOT use PLA for any outdoor or load-bearing parts.** PLA begins to soften above 60°C (direct sun on dark-colored part can exceed this). PLA also becomes brittle with UV exposure.

### 11.2 Post-Print Inspection — All Load-Bearing Parts

**Visual:**
- [ ] No visible layer separation (delamination)
- [ ] No infill voids visible at perimeter intersections
- [ ] No stringing or webbing in functional bore surfaces
- [ ] Perimeter count correct (count visible perimeter lines on cross-section)

**Dimensional:**
- [ ] Bore diameters: measure with calipers. Maximum deviation from nominal: ± 0.3mm
- [ ] Bolt hole diameters: M4 holes 4.3–4.5mm; M6 holes 6.3–6.5mm; M8 holes 8.4–8.6mm
- [ ] Wall thickness: measure at 3 points. Minimum: 90% of designed thickness

**Functional:**
- [ ] Split collar: halves fit flush when bolted. No gap > 0.5mm at mating face.
- [ ] Guy ring collar: eyes open and capture a test loop without hang-up.
- [ ] Tensioner: M6 rod threads smoothly through nut without binding.

### 11.3 Rated Loads (Printed PETG, 50% Gyroid)

These are conservative estimates based on standard PETG tensile strength (~45 MPa).

| Part | Rated Working Load | Safety Factor | Design Basis |
|------|--------------------|---------------|-------------|
| split_collar() | 500 N shear per bolt pair | 3× | Shear through perimeter wall |
| guy_ring_collar() eye | 1,200 N tension | 3× | Eye wall cross-section |
| guy_tensioner() | 900 N tension | 3× | Threaded rod engagement |
| ground_spike_collar() | 800 N bending | 3× | Cantilever at base |
| hitch_mast_mount() | 600 N lateral | 3× | Column bending, 500mm arm |

**Replace any part showing:**
- Permanent deformation (stretched, bowed, or cracked)
- Any visible crack, regardless of size
- Any pull-out or rotation of a captured nut
- Visible UV whitening or chalking on ASA (reprint)

---

<a name="chapter-12"></a>
## CHAPTER 12 — TEST AND ALIGNMENT PROCEDURES

### 12.1 Initial Installation Test (First Raise)

After assembly and first raise, before attaching antenna load:

```
TEST 1 — PLUMB VERIFICATION
  Tool: Plumb bob or spirit level
  Standard: Mast vertical ±1° in both planes
  Method: Adjust guys until plumb bob over base center, or level reads center
  Record: Plumb angle N-S: _____° E-S: _____° Date: _______

TEST 2 — GUY TENSION VERIFICATION
  Tool: Lugal line tension meter or pluck test (subjective)
  Standard: All guys at same level within 20% tension of each other
  Method: Pluck test — identical audible pitch on all wires at a level
  Record: Tensions approximately equal? Y / N

TEST 3 — SECTION LOCK VERIFICATION
  Tool: Hand shake test
  Standard: No section movement > 3mm under 50 N lateral hand force
  Method: At base: grip mast; apply lateral force in 4 directions; check movement
  Record: All sections locked? Y / N

TEST 4 — BASE PLATE INTEGRITY
  Tool: Visual + anchor probe
  Standard: No anchor movement; base plate level and stable
  Method: Grip each guy wire; attempt to manually pull anchor; observe any movement
  Record: All anchors firm? Y / N
```

### 12.2 Rotator Calibration Test

```
TEST 5 — ROTATOR POINTING ACCURACY
  Tool: GPS compass or Polaris bearing; QMC5883L on CYD
  Standard: Display heading matches actual antenna pointing ±5°
  Method:
    1. Command rotation to 0° (North). Verify physical pointing vs. GPS North.
    2. Command rotation to 90° (East). Verify vs. GPS East.
    3. Command rotation to 180°, 270°. Verify each.
  Record: Max error: _____° Pass/Fail: _______

TEST 6 — ROTATOR CABLE RESISTANCE
  Tool: Multimeter, ohms mode
  Standard: Each conductor < 10Ω total loop resistance
  Method: At controller end, short each wire pair at rotator end; measure loop R
  Record: Highest R: _____Ω
```

### 12.3 Mast Load Test (Optional but Recommended for New Systems)

With mast fully extended and guyed, attach a temporary load of 50% rated antenna weight to the top:

- Observe any visible mast deflection.
- Observe any anchor creep.
- Hold load for 10 minutes minimum.
- No visible permanent set; anchor position unchanged.

---

<a name="chapter-13"></a>
## CHAPTER 13 — FAULT ISOLATION AND TROUBLESHOOTING

### 13.1 Troubleshooting Philosophy

Follow the electron path from effect to cause. For mechanical systems: follow the force path. When the mast is leaning, trace which guy is loose, which anchor has yielded, which section has unlocked. Never blame "the wind" without first investigating the mechanical failure mode.

### 13.2 Fault Table

| Symptom | Probable Cause | Diagnostic Step | Corrective Action |
|---------|----------------|-----------------|-------------------|
| Mast leaning | Guy loose / anchor pulled out | Inspect all guys; probe anchors | Re-tension or replace anchor |
| Section slips down | Hose clamp loose; locking pin not engaged | Shake section; inspect clamp/pin | Retighten clamp; reinstall pin |
| Mast vibrating in wind | Insufficient guy pre-tension | Pluck test — no audible resonance | Increase pre-tension to 75–100 N |
| Section won't extend | Grit in sliding surface; deformed tube | Inspect bore; wipe clean | Clean and re-lubricate with PTFE |
| Rotator not rotating | Motor overload; cable break; controller fault | Measure motor voltage; check cable R | Reduce antenna load; replace cable |
| Rotator heading incorrect | Compass offset wrong; mounting rotation | Re-calibrate; verify mast/antenna alignment | Recalibrate SET NORTH |
| Pneumatic: won't extend | Insufficient pressure; seal failure | Check gauge; soap-bubble test | Increase pressure; replace seal |
| Pneumatic: rapid collapse | Seal failure while extended | **Emergency: evacuate collapse zone** | Lower controlled if possible; inspect seals |
| Guy wire broke | Fatigue at crimp; overload | Inspect all terminations | Replace wire + all crimps at level |
| Base anchor pulling out | Soil too soft; anchor too small | Probe adjacent soil | Deadman anchor; larger screw |
| CYD no tilt reading | MPU-6050 I2C fault | Verify wiring continuity; I2C scan | Replace sensor; check address (0x68/0x69) |
| CYD wrong wind reading | Anemometer reed switch dirty | Count pulses at known speed | Clean/replace reed; recalibrate |

### 13.3 Post-Storm Inspection

After any storm with sustained winds > 35 mph:

```
□ Inspect all mast sections for impact damage, dents, or bends
□ Check all hose clamp positions (vibration can loosen)
□ Probe each anchor — verify no displacement
□ Inspect all guy wire terminations (crimps, knots) — no slippage
□ Inspect rotator cable for chafing at strain relief clips
□ Verify mast plumb; re-tension guys as required
□ Inspect base plate hardware (welds, anchor bolts)
□ Check CYD/sensor hardware for weather damage
□ Document any findings; repair before next deployment
```

---

<a name="chapter-14"></a>
## CHAPTER 14 — DISASSEMBLY, TRANSPORT, AND STORAGE

### 14.1 Breakdown Sequence

Reverse of installation:

1. Lower antenna to minimum wind exposure heading (rotator).
2. Disconnect all feedlines; coil and bag.
3. Release guy tension — **all levels simultaneously**, starting with the lowest level.
4. Lower mast to horizontal (tilt-over) or begin collapsing sections from top.
5. Remove guy ring collars; bag hardware.
6. Extract ground anchors; clean soil from threads; bag.
7. Nest mast sections; secure in carry bag.
8. Disassemble tripod or remove base plate.
9. Coil all guy wires on a reel or in a figure-8 coil; avoid kinks.

### 14.2 Storage

**Fiberglass sections:** Store nested in UV-protected bag or rack. Avoid direct sun (UV degrades polyester resin over years).

**Aluminum sections:** Rinse with fresh water after coastal/salt-air deployment. Inspect for pitting annually. Light coating of ACF-50 or CorrosionX inhibitor.

**Guy wires:** Coil 300–500mm diameter; store dry. Galvanized steel: apply light oil before long-term storage. Dyneema: no treatment needed; store out of UV.

**Printed parts:** Store in sealed bag with silica gel if humidity > 70%. Inspect for UV chalking (ASA) annually.

**Winch (crank-up):** Grease drum shaft; wrap cable lightly with oil rag; cover housing.

### 14.3 Transport

| Component | Packaged Length | Diameter | Weight |
|-----------|----------------|---------|--------|
| 20 ft fiberglass (4 sections nested) | 2.0m | 80mm | 5 kg |
| 30 ft aluminum (5 sections nested) | 2.0m | 90mm | 9 kg |
| Pneumatic mast (4 sections nested) | 1.8m | 75mm | 7 kg |
| Crank-up tower (3 sections, folded) | 2.1m × 0.3m | — | 28 kg |
| Guy wire kit (20 ft system) | 400mm coil bag | — | 2 kg |
| Anchor kit (6 screws, T-handle) | 600mm bag | — | 3 kg |
| Printed hardware bag | 300mm box | — | 0.5 kg |

---

<a name="chapter-15"></a>
## CHAPTER 15 — SAFETY STANDARDS AND EMERGENCY PROCEDURES

### 15.1 Safety Zone

**At all times:**
- Personnel within the mast collapse zone (radius = mast height) must be aware and prepared for emergency lower.
- Establish a clear path out of the collapse zone before raising any mast.
- Do NOT stand directly beneath an extended mast.

### 15.2 Lightning Emergency

**If a thunderstorm approaches:**

1. Monitor sky. If you hear thunder: **begin lowering immediately.** Do not wait for rain.
2. Lower mast to horizontal; disconnect all cables.
3. Move all personnel and equipment to hard-roofed shelter or vehicle.
4. Do not re-erect until 30 minutes after the last thunder.

**If lightning strikes while mast is up:**

1. Do not touch any part of the mast system.
2. Evacuate immediately to shelter.
3. Call for medical assistance if anyone was in contact with mast or cables.
4. After 30 minutes with no additional lightning: approach carefully; inspect for damage.
5. Check all electrical connections before re-using associated radio equipment (lightning induces voltage surges; can destroy radios, computers, BMS systems).

### 15.3 Mast Collapse Emergency

If a mast collapses unexpectedly:

1. Move all personnel immediately away from the collapse zone.
2. Do not attempt to stop or catch the mast.
3. Account for all personnel after collapse.
4. If anyone is struck by falling mast or cables: call for medical assistance immediately.
5. Do not disturb collapsed equipment until all personnel are confirmed safe.
6. Document the failure: which anchor, which section, wind conditions, what was attached.

### 15.4 Electrical Contact Emergency

If a mast or feedline contacts a power line:

> **WARNING:** Do not touch any part of the mast, feedline, or anything in contact with the mast.

1. Immediately call 911 and the power company.
2. Establish a safety perimeter of 10m (30 ft) minimum from all metal that could conduct power.
3. Do not attempt to lower the mast or disconnect cables.
4. Wait for utility crew to de-energize the line.
5. Ground step voltage: stay away from energized ground — do not run; shuffle feet or hop to move away from energized soil.

---

<a name="appendix-a"></a>
## APPENDIX A — WIND LOAD QUICK REFERENCE TABLES

### Table A-1: Guy Wire Tension (each wire, windward, 3-guy configuration)

Anchor radius = 70% of mast height; single guy level shown; add levels per schedule.

| Mast Height | 30 mph | 50 mph | 70 mph | 90 mph |
|-------------|--------|--------|--------|--------|
| 15 ft       | 45 N   | 125 N  | 244 N  | 403 N  |
| 20 ft       | 60 N   | 167 N  | 325 N  | 537 N  |
| 30 ft*      | 108 N  | 300 N  | 585 N  | 966 N  |
| 40 ft*      | 192 N  | 533 N  | 1040 N | 1716 N |

*Multiple guy levels — values shown for upper level (highest tension).

### Table A-2: Minimum Anchor Holding Capacity Required (3.0× safety factor applied)

| Mast Height | 70 mph (operational) | 90 mph (survival) |
|-------------|---------------------|-------------------|
| 20 ft       | 975 N (99 kgf)      | 1,611 N (164 kgf) |
| 30 ft       | 1,755 N (179 kgf)   | 2,898 N (295 kgf) |
| 40 ft       | 3,120 N (318 kgf)   | 5,148 N (524 kgf) |

### Table A-3: Anchor Capacity by Type and Soil

| Anchor Type | Soft Clay/Sand | Medium Loam | Dense Clay |
|-------------|----------------|-------------|------------|
| Screw 12mm × 450mm | 400 N | 700 N | 1,100 N |
| Screw 16mm × 600mm | 900 N | 1,500 N | 2,400 N |
| Screw 22mm × 750mm | 2,000 N | 3,500 N | 6,000 N |
| Driven rebar 19mm × 600mm | 800 N | 1,400 N | 2,200 N |
| Deadman plate 600×200×6mm | 3,000 N | 5,000 N | 12,000 N |
| Vehicle (parked, firm ground) | 4,000 N | 7,000 N | 10,000 N |

---

<a name="appendix-b"></a>
## APPENDIX B — KNOT AND WIRE TERMINATION REFERENCE

### B-1: Dyneema Terminations

**Double Bowline (quick field knot):**  
Retains ~65% of breaking strength. Use for temporary connections only.  
Never use a standard (single) bowline on Dyneema — slips under cyclic load.

**Brummel Splice (best):**  
Retains ~95% of breaking strength. Time: 10–15 min per splice.  
Requires Dyneema splice fid. Preferred for all permanent guy wire terminations.

**Figure-8 Loop:**  
Retains ~70–75%. Faster than Brummel; use where periodic replacement is expected.

### B-2: Steel Wire Rope — Nicopress Crimp

**Required sleeves per termination:** 3 (minimum 2 for emergency; 3 for rated capacity)  
**Sleeve positioning:** First sleeve at loop base; second 25mm toward dead end; third at dead end.  
**Tool:** Go/no-go gauge for each sleeve size. A properly swaged sleeve must pass the no-go gauge.  
**Thimble:** Always use a matching thimble inside the loop. Prevents kinking and localizes wear.

**Visual inspection:**  
- Sleeve deformation: oval → acceptable; cracked/split → replace
- Wire inside sleeve: any broken wire visible → replace
- Thimble: any cracks → replace

---

<a name="appendix-c"></a>
## APPENDIX C — 3D-PRINTED PART SPECIFICATIONS AND INSPECTION

### C-1: Print Parameter Summary

| Part | Module | Material | Infill | Perimeters | Notes |
|------|--------|----------|--------|------------|-------|
| split_collar() | mast_collars.scad | PETG | 50% gyroid | 5 | 2 prints per collar |
| guy_ring_collar() | mast_collars.scad | PETG | 50% gyroid | 5 | Print eyes upright |
| rotator_saddle_cage() | mast_collars.scad | PETG | 40% cubic | 4 | No supports needed |
| guy_tensioner() | guy_tensioners_anchors.scad | PETG | 50% gyroid | 5 | Print vertically |
| guy_equalizer_3() | guy_tensioners_anchors.scad | PETG | 50% gyroid | 5 | Flat on bed |
| ground_spike_collar() | base_plates_mounts.scad | PETG | 50% gyroid | 5 | Vertical |
| hitch_mast_mount() | base_plates_mounts.scad | PETG | 50% gyroid | 5 | Print insert down |
| tripod_foot_pad() | base_plates_mounts.scad | PETG | 40% gyroid | 4 | Flat on bed |
| plumb_wedge() | base_plates_mounts.scad | PETG | 30% gyroid | 4 | Flat on bed |

### C-2: Dimensional Tolerances (acceptance criteria)

| Dimension | Nominal | Tolerance |
|-----------|---------|-----------|
| Mast bore (clamp collars) | mast_od + 0.6mm | +0.3mm / -0.0mm |
| Bolt holes M4 | 4.4mm | ±0.2mm |
| Bolt holes M6 | 6.4mm | ±0.2mm |
| Bolt holes M8 | 8.5mm | ±0.2mm |
| Eye inner diameter | 8.0mm | ±0.5mm |
| Split gap (collars) | 4.0mm | ±1.0mm |
| Wall thickness | Per design | -0% / +10% |

---

<a name="appendix-d"></a>
## APPENDIX D — CYD MAST MONITOR FIRMWARE REFERENCE

### D-1: Hardware Configuration

```
ESP32 (CYD board) pin assignments:
  GPIO 21 / 22  — I2C SDA/SCL: MPU-6050 (0x68), QMC5883L (0x0D)
  GPIO 25 / 26  — HX711 DOUT/SCK: base load cell (optional)
  GPIO 34        — Anemometer reed switch (interrupt, falling edge)
  GPIO 27        — Rotator motor CW (H-bridge input)
  GPIO 14        — Rotator motor CCW (H-bridge input)
  GPIO 12        — Motor enable PWM (speed control)
  CYD TFT       — ILI9341 via SPI (built-in); touch via XPT2046

  Power: 5V USB; CYD has onboard 3.3V LDO.
  Anemometer supply: 3.3V; 10kΩ pull-up on GPIO 34.
```

### D-2: BLE Advertising

- Device name: `MAST-RTR-001`
- Service UUID: `6E400001-B5A3-F393-E0A9-E50E24DCCA9E` (Nordic UART profile)
- TX Characteristic (notify): sends `BEARING=145.3\nTILT=0.8\nWIND=12.4\n` at 2 Hz
- RX Characteristic (write): accepts `TARGET=090\n` to set bearing target

### D-3: Python BLE Client (Linux/Windows)

```python
import asyncio
from bleak import BleakClient

DEVICE = "MAST-RTR-001"
TX_UUID = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"  # notify
RX_UUID = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"  # write

async def main():
    async with BleakClient(DEVICE) as client:
        def on_data(char, data):
            print(data.decode().strip())

        await client.start_notify(TX_UUID, on_data)

        # Command antenna to 90° (East)
        await client.write_gatt_char(RX_UUID, b"TARGET=090\n")
        await asyncio.sleep(30)

asyncio.run(main())
```

---

<a name="appendix-e"></a>
## APPENDIX E — PARTS LIST AND SOURCES

### E-1: Mast Tubing Sources (US)

| Supplier | Material | Notes |
|----------|----------|-------|
| McMaster-Carr (mcmaster.com) | Fiberglass tubes, 6061-T6 aluminum | Fast shipping; good selection |
| Online Metals (onlinemetals.com) | 6061-T6 aluminum, cut to length | Cut-to-order service |
| Grainger | Both fiberglass and aluminum | Local pickup available |
| DragonPlate (dragonplate.com) | High-grade fiberglass, carbon fiber | Premium; SOTA use |
| Max-Gain Systems (max-gain.com) | Commercial push-up masts, 1.5" standard | Drop-in military replacement |

### E-2: Guy Wire and Rigging Hardware Sources

| Item | Source | Notes |
|------|--------|-------|
| Dyneema SK75 3mm | eBay, Amazon, Rope & Twine suppliers | Buy in 100m reels; cheaper |
| 3/16" 7×19 galvanized | McMaster-Carr; West Marine | Per-foot or coil |
| Nicopress sleeves | Ham Radio Outlet; Rohn distributors | Get tool too |
| Turnbuckles 5/16" | Amazon; hardware store | SS preferred |
| Snap hooks SS | Amazon; West Marine | 316 SS for coastal |

### E-3: Ground Anchor Sources

| Item | Source | Notes |
|------|--------|-------|
| Screw anchors 12–22mm | Amazon; hardware stores | Search "ground anchor screw" |
| Duckbill anchors | Ferno; forestry suppliers | Excellent capacity in soft soil |
| Rebar (19mm) | Local hardware/building supply | Cut to 600mm with grinder |
| Galv. steel plate (deadman) | Metal supplier; Fastenal | Cut to 600×200mm |

### E-4: Rotators

| Model | Source | Load Capacity | Field Use |
|-------|--------|---------------|-----------|
| Yaesu G-450A | HRO; DX Engineering | 7 kgf | Light-duty field |
| Yaesu G-800DXA | HRO; DX Engineering | 15 kgf | Medium Yagi field |
| Ham-IV (MFJ-4116) | MFJ Enterprises | 15 kgf | Budget alternative |
| SatNOGS v3 | Print-it-yourself | 5 kgf AZ/EL | Open source; Arduino |

---

*End of TM-MAST-001 Rev A*

*Prepared by: Field Engineering Reference — TM-MAST-001*  
*Applicable systems: TM-MAST-001A/B/C/D*  
*Related manuals: TM-BMS-001 (power), TM-PWR-001 (power supplies), TM-ANT-001 (antenna suite)*
