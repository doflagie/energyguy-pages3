# 30m COMPACT DIPOLE ANTENNA
## TECHNICAL DATA SHEET

**Document Number:** TM-30M-001 Rev A
**Equipment:** 30m COMPACT DIPOLE ANTENNA
**Classification:** UNCLASSIFIED — Amateur Radio / Field Use
**Date:** 2026-05-24
**Supersedes:** None (initial issue)

---

## RECORD OF CHANGES

| Change No. | Rev | Date | Description | By |
|:---:|:---:|:---:|---|:---:|
| 1 | A | 2026-05-24 | Initial formatted release | M. Martin |

---


---

## CHAPTER 1 — DESIGN SUMMARY

### 1.1 Center Frequency and Band Allocation

| Parameter | Value |
|-----------|-------|
| **Band** | 30m Amateur Radio |
| **Center frequency** | 10.125 MHz |
| **Band edge (low)** | 10.100 MHz |
| **Band edge (high)** | 10.150 MHz |
| **Band width** | 0.050 MHz |

### 1.2 Physical Dimensions

| Parameter | Value | Notes |
|-----------|-------|-------|
| **Full half-dipole length** | 7.042 m (23.10 ft) | Unloaded, λ/4 @ VF=0.95 |
| **Compact half-dipole length** | 3.521 m (11.55 ft) | With center loading coil |
| **Compaction factor** | 50% | Size reduction vs full-size |
| **Roll-up diameter** | ~1.06 m | Fully rolled on card |
| **Deployed span** | 7.042 m (23.10 ft) | Both arms extended |

### 1.3 RF Characteristics (Nominal)

| Parameter | Value |
|-----------|-------|
| **Feedpoint impedance** | 50Ω ± 10% (at resonance) |
| **SWR @ band center** | < 1.5:1 (after trimmer tuning) |
| **Radiation pattern** | Figure-8 (broadside, omnidirectional in azimuth) |
| **Polarization** | Vertical (horizontal deployment plane-wave) |
| **Gain** | 2.0–2.5 dBi (band-dependent, see NEC model) |
| **Efficiency** | 85–90% (high-Q coil at design frequency) |
| **Bandwidth (2:1 SWR)** | 0–0 kHz (estimated) |

---

## CHAPTER 2 — LOADING COIL DESIGN

### 2.1 Coil Specifications

| Parameter | Value | Notes |
|-----------|-------|-------|
| **Target inductance** | 10.42 μH | Per arm (center-loaded) |
| **Number of turns** | 17 | Close-wound, no gaps |
| **Wire gauge** | AWG 24 | 0.0201" diameter |
| **Total wire length** | 1.42 m (4.7 feet) | Both coils |
| **Form material** | 3/4" PVC | Schedule 40 PVC or equivalent |
| **Form inside radius** | 0.525" (1.33 cm) | Rigid substrate |
| **Coil length** | 0.342" (0.87 cm) | Close-wound on form |
| **Estimated Q factor** | 200 | High-Q design, low loss |
| **Coil DC resistance** | 0.0 mΩ | (estimate, AWG 24) |

### 2.2 Fabrication Procedure

#### Step 1: Form Preparation
- Obtain rigid PVC pipe, 3/4" PVC OD
- Cut to length: 0.34 + 1/4" for margin = 0.59"
- Deburr ends with 120-grit sandpaper
- Inspect for cracks or defects

#### Step 2: Coil Winding
1. Secure form in lathe or hand-wind jig
2. Feed magnet wire (AWG 24) through form center
3. Wind tight helical coil, pitch = wire diameter (0.511 mm)
4. Maintain tension for uniform spacing
5. **Target:** 17 turns, close-wound, no gaps
6. Mark first and last turn with marker pen

#### Step 3: Continuity & Inductance Verification
- Measure DC resistance end-to-end
  - Expected: 1.4m × 0.026Ω/m ≈ 0.04 mΩ
  - Acceptable range: ±20% of expected value
- Measure inductance with LCR meter or dip meter
  - Expected: 10.42 μH
  - Acceptable: 9.38–11.46 μH (±10%)
- If out of spec, recount turns and rewind if necessary

#### Step 4: Lead Soldering
- Tin first and last turn with rosin-core solder (60/40 or lead-free)
- Solder AWG 18 Kynar leads (~2 cm length) to tinned points
- Allow joint to cool naturally
- Verify solder wets evenly around turn circumference

#### Step 5: Encapsulation
- Wrap coil in 2 layers of electrical tape (insulation)
- Surround with polyurethane foam (low-density expanding type)
  - Purpose: mechanical protection, weatherproofing, arc prevention
- Alternatively, use 2-inch heat-shrink tubing + epoxy potting
- Allow cure time: 24 hours (polyurethane) or 48 hours (epoxy)

### 2.3 Quality Assurance

✓ Visual inspection: No cracks, loose turns, or exposed wire
✓ Continuity test: DC resistance within ±20% of expected
✓ Inductance test: 9.38–11.46 μH on LCR meter
✓ Mechanical test: Gently flex coil; should not shift or rattle
✓ Weatherproofing: No exposed copper; solder joints sealed

---

## CHAPTER 3 — TRAP RESONATOR ASSEMBLY

### 3.1 Component Specifications

| Component | Specification | Value |
|-----------|---|---|
| **Inductance** | Loading coil (from Section 2) | 10.42 μH (per arm) |
| **Capacitance** | Variable trimmer, parallel | 12.0 pF (Bourns 3296) |
| **Resonant frequency** | Design center | 10.125 MHz |
| **Trap inductance** | Calculated from LC resonance | 20.591 μH |
| **Expected Q** | Parallel LC circuit | ~150 (with trimmer) |

### 3.2 Resonance Formula

$$f_{resonance} = \frac{1}{2\pi\sqrt{LC}}$$

**Verification:** 20.591 μH || 12.0 pF should resonate at 10.125 MHz ± 1%

### 3.3 Trimmer Capacitor Adjustment Procedure

**Equipment required:**
- Dip meter (preferred) or VNA/RF analyzer
- Screwdriver (small slot type)
- SWR meter (optional, for final validation)

**Procedure:**

1. **Initial adjustment (approximate):**
   - Apply light finger pressure to trimmer capacitor
   - Adjust screw slowly, observing dip meter coil for signal dip
   - Target: Minimum dip at 10.125 MHz

2. **Fine tuning:**
   - Advance trimmer screw by 1/8 turn
   - Observe dip frequency shift
   - Iterate until dip is centered at band frequency
   - Document final screw position (e.g., "fully engaged, backed out 2.5 turns")

3. **SWR verification (optional):**
   - With transmitter in tune mode, monitor SWR meter
   - SWR should dip below 1.5:1 at band center
   - If not achieved, re-check trimmer and loading coil inductance

4. **Lock down:**
   - Once resonance is confirmed, apply small drop of clear nail polish to trimmer screw
   - Prevents accidental rotation from vibration

> **NOTE**
> Ceramic trimmer capacitors exhibit ±250 ppm/°C temperature coefficient.
> If antenna is tuned at 25°C but used at 50°C, resonance will shift ~100 kHz (HF bands).
> Retune as needed for seasonal operation.

---

## CHAPTER 4 — ANTENNA CARD ASSEMBLY

### 4.1 Card Geometry

| Parameter | Value |
|-----------|-------|
| **Substrate material** | HDPE sheet, 3 mm thick |
| **Length** | 160 mm (6.3 inches) |
| **Width** | 50 mm (2.0 inches) |
| **Edge groove (coil slot)** | 1/4" W × 3/8" D, routed along length |
| **Radiator wire notches** | 1/16" dia holes, 88 mm spacing |

### 4.2 Assembly Steps

1. **Substrate preparation:**
   - Cut HDPE card to dimensions above
   - Route edge groove for coil insertion (1/4" × 3/8")
   - Drill radiator wire retention notches (spaced every 1 inch)
   - Radius all edges (R = 3 mm) to prevent wire damage

2. **Coil installation:**
   - Insert fully-encapsulated loading coil into edge groove
   - Secure with epoxy every 50 mm to prevent movement
   - Leads should exit at card center for feed-point connection

3. **Radiator wire routing:**
   - Total wire length: 7.04 m (23.1 feet)
   - Thread into card notches, leaving 5 cm slack at each end
   - Secure with small cable ties at 100 mm intervals
   - Leave 3–4 cm free at arm terminus for trap soldering

4. **Center feed-point block:**
   - Fabricate HDPE block (30 × 30 × 15 mm) with center hole
   - Bond to top and bottom of card center using epoxy
   - Install BNC/SMA connector (female, chassis mount)
   - Mount 1:1 ferrite balun inside block
   - Solder loading coil leads to balun connection points

5. **Trap attachment:**
   - Solder encapsulated trap assembly to radiator wire terminus
   - Strain relief: braid around arm wire 5 cm above trap
   - Use heat-shrink tubing over solder joint for weatherproofing

6. **Final inspection:**
   - Verify no cold solder joints (dull, grainy appearance)
   - Check continuity: feedpoint center pin → trap on each arm
   - Measure DC resistance: should be < 0.5Ω per arm
   - Visually inspect for sharp edges, exposed conductors, or gaps

---

## CHAPTER 5 — ELECTRICAL TESTING

### 5.1 Impedance & SWR Measurement

**Required equipment:**
- SWR/power meter (capable of 10.10–10.15 MHz)
- Feedline: RG-8 or better, ~5 m length
- Transmitter (QRP < 5W recommended for initial test)
- Dummy load (50Ω, 1–5W rating)

**Procedure:**

1. **Baseline (equipment verification):**
   - Connect feedline to dummy load
   - Measure SWR; should read 1.0:1 across band
   - If not, inspect feedline for damage or loose connectors

2. **Initial antenna test:**
   - Connect feedline to antenna BNC connector
   - Apply low power (< 2W)
   - Measure SWR at 10.125 MHz
   - Record value: __________ (target < 3:1 before tuning)

3. **Trimmer adjustment (dip meter preferred):**
   - Using dip meter, approach trap resonator
   - Adjust trimmer screw until dip is maximum
   - Dip frequency should be 10.125 MHz ± 0.5%
   - Re-check SWR; should drop to < 1.5:1

4. **Band-edge SWR verification:**
   - Record SWR at band low edge: __________ @ 10.100 MHz
   - Record SWR at band center: __________ @ 10.125 MHz (target < 1.5:1)
   - Record SWR at band high edge: __________ @ 10.150 MHz
   - All edges should be < 2.5:1; center < 1.5:1

### 5.2 NEC2 Simulation

Load model file: `compact_dipole_30m.nec`

**Expected results:**
- **Impedance @ resonance:** 49–52Ω (matched to 50Ω)
- **Gain:** 2.0–2.5 dBi (typical for center-loaded dipole)
- **Efficiency:** 85–90%
- **3-dB beamwidth:** 70–90° (depends on height)
- **Front-to-back:** 10–20 dB (typical)

---

## CHAPTER 6 — DEPLOYMENT AND OPERATION

### 6.1 Quick Deployment (< 2 minutes)

1. Remove antenna from protective pouch
2. Gently unroll both radiator arms from card perimeter
3. Allow loading coils to expand fully in edge groove
4. Deploy both arms to horizontal (or vertical) orientation
5. Connect feedline (via 1:1 balun) to center BNC connector
6. Adjust trimmer with dip meter for minimum dip at 10.125 MHz
7. Verify SWR < 1.5:1 with meter
8. Ready to transmit

### 6.2 Power Handling Limits

**Maximum RF power (continuous CW):**
- Coil wire AWG 24: Limited by heating
- Estimated safe power: 25–75 W CW (antenna-dependent)
- Monitor coil temperature during extended transmission
- If temperature exceeds 60°C, reduce power and allow cooling

> **WARNING**
> Exceeding power limits will degrade Q, cause impedance shift, and potentially fail wire insulation.

### 6.3 Maintenance and Storage

- **After each use:** Inspect for physical damage, corrosion
- **Monthly (if stored outdoors):** Check solder joints, apply contact cleaner to connectors
- **Annually:** Remeasure inductance; adjust trimmers if resonance has drifted
- **Storage:** Dry location, room temperature; store rolled in protective pouch

---

## APPENDIX — PER-BAND SCHEMATICS

The following diagrams are located in this band folder (`30m/figures/`):

1. **30m_antenna_layout.png** — Scale deployment drawing with dimensions
2. **30m_coil_detail.png** — Loading coil cross-section, winding detail
3. **30m_trap_schematic.png** — Trap resonator circuit, L/C values

---

## REVISION HISTORY

| Date | Version | Author | Changes |
|------|---------|--------|---------|
| 2026-04-21 | 1.0 | Technical Manual Generator | Initial release |

---

**END OF 30M SPECIFICATION**
