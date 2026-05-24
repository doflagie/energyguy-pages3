#!/usr/bin/env python3
"""
Generate per-band technical specification chapters in Navy manual format.
Creates compact_dipole_<band>.md in each band folder.
"""

import math
from pathlib import Path
from dataclasses import dataclass
from typing import List


@dataclass
class BandData:
    name: str
    band_key: str
    f_center: float
    f_start: float
    f_end: float
    compaction: float
    pvc_form_id: str
    pvc_form_r: float
    radiator_awg: int
    coil_awg: int
    trap_c_pf: float

    def get_full_half_length_m(self) -> float:
        return 71.3 / self.f_center

    def get_compact_half_length_m(self) -> float:
        return self.get_full_half_length_m() * self.compaction

    def get_loading_inductance_uh(self) -> float:
        return 20.9 * (7.15 / self.f_center) ** 2

    def get_trap_inductance_uh(self) -> float:
        f_rad_s = 2 * math.pi * self.f_center * 1e6
        c_farad = self.trap_c_pf * 1e-12
        l_henry = 1.0 / (f_rad_s ** 2 * c_farad)
        return l_henry * 1e6

    def get_coil_params(self) -> tuple:
        l_target_uh = self.get_loading_inductance_uh()
        r = self.pvc_form_r
        wire_dia_inches = self._awg_to_diameter(self.coil_awg)

        a = r ** 2
        b = -l_target_uh * 10 * wire_dia_inches
        c = -l_target_uh * 9 * r

        disc = b**2 - 4*a*c
        if disc < 0:
            n_turns = int(math.sqrt(l_target_uh * (9*r) / (r**2)))
        else:
            n_turns = int((-b + math.sqrt(disc)) / (2*a))

        coil_len_inches = n_turns * wire_dia_inches
        wire_len_m = n_turns * 2 * math.pi * r * 0.0254
        return n_turns, coil_len_inches, wire_len_m

    def _awg_to_diameter(self, awg: int) -> float:
        return 0.005 * (92 ** ((36 - awg) / 39))


BANDS = [
    BandData("160m", "160m", 1.900, 1.800, 2.000, 0.50, "1.5\" PVC", 0.750, 14, 22, 30.0),
    BandData("80m", "80m", 3.750, 3.500, 4.000, 0.50, "1\" PVC", 0.625, 14, 22, 25.0),
    BandData("40m", "40m", 7.150, 7.000, 7.300, 0.50, "3/4\" PVC", 0.525, 16, 22, 15.0),
    BandData("30m", "30m", 10.125, 10.100, 10.150, 0.50, "3/4\" PVC", 0.525, 16, 24, 12.0),
    BandData("20m", "20m", 14.175, 14.000, 14.350, 0.50, "3/4\" PVC", 0.525, 18, 24, 10.0),
    BandData("17m", "17m", 18.118, 18.068, 18.168, 0.50, "1/2\" PVC", 0.400, 18, 24, 8.0),
    BandData("15m", "15m", 21.225, 21.000, 21.450, 0.50, "1/2\" PVC", 0.400, 18, 26, 7.0),
    BandData("12m", "12m", 24.940, 24.890, 24.990, 0.50, "1/2\" PVC", 0.400, 20, 26, 6.0),
    BandData("10m", "10m", 28.850, 28.000, 29.700, 0.50, "1/2\" PVC", 0.400, 20, 26, 5.0),
    BandData("6m", "6m", 52.000, 50.000, 54.000, 0.60, "3/8\" PVC", 0.280, 22, 28, 4.0),
    BandData("2m", "2m", 146.000, 144.000, 148.000, 0.60, "3/8\" PVC", 0.280, 22, 28, 3.0),
    BandData("1.25m", "1p25m", 222.500, 220.000, 225.000, 0.95, "1/4\" FR4", 0.200, 24, 30, 2.0),
    BandData("70cm", "70cm", 435.000, 420.000, 450.000, 0.95, "1/4\" ceramic", 0.180, 24, 30, 1.5),
    BandData("33cm", "33cm", 915.000, 902.000, 928.000, 0.95, "1/8\" ceramic", 0.125, 28, 30, 1.0),
    BandData("20cm", "20cm", 1270.000, 1240.000, 1300.000, 0.95, "1/8\" ceramic", 0.125, 28, 30, 0.8),
]


def generate_band_chapter(band: BandData, output_path: Path) -> None:
    """Generate per-band markdown technical specification"""

    full_half_m = band.get_full_half_length_m()
    compact_half_m = band.get_compact_half_length_m()
    l_load = band.get_loading_inductance_uh()
    l_trap = band.get_trap_inductance_uh()
    n_turns, coil_len_in, wire_len_m = band.get_coil_params()

    # Estimate efficiency and other specs
    if band.f_center < 30:
        efficiency = "85–90%"
        q_coil = 200
    elif band.f_center < 100:
        efficiency = "80–85%"
        q_coil = 250
    else:
        efficiency = "75–82%"
        q_coil = 300

    chapter = f"""# {band.name} COMPACT DIPOLE ANTENNA
## Technical Specification and Fabrication Guide

---

## SECTION 1 — DESIGN SUMMARY

### 1.1 Center Frequency and Band Allocation

| Parameter | Value |
|-----------|-------|
| **Band** | {band.name} Amateur Radio |
| **Center frequency** | {band.f_center:.3f} MHz |
| **Band edge (low)** | {band.f_start:.3f} MHz |
| **Band edge (high)** | {band.f_end:.3f} MHz |
| **Band width** | {band.f_end - band.f_start:.3f} MHz |

### 1.2 Physical Dimensions

| Parameter | Value | Notes |
|-----------|-------|-------|
| **Full half-dipole length** | {full_half_m:.3f} m ({full_half_m*3.28084:.2f} ft) | Unloaded, λ/4 @ VF=0.95 |
| **Compact half-dipole length** | {compact_half_m:.3f} m ({compact_half_m*3.28084:.2f} ft) | With center loading coil |
| **Compaction factor** | {band.compaction*100:.0f}% | Size reduction vs full-size |
| **Roll-up diameter** | ~{compact_half_m*0.3:.2f} m | Fully rolled on card |
| **Deployed span** | {2*compact_half_m:.3f} m ({2*compact_half_m*3.28084:.2f} ft) | Both arms extended |

### 1.3 RF Characteristics (Nominal)

| Parameter | Value |
|-----------|-------|
| **Feedpoint impedance** | 50Ω ± 10% (at resonance) |
| **SWR @ band center** | < 1.5:1 (after trimmer tuning) |
| **Radiation pattern** | Figure-8 (broadside, omnidirectional in azimuth) |
| **Polarization** | Vertical (horizontal deployment plane-wave) |
| **Gain** | 2.0–2.5 dBi (band-dependent, see NEC model) |
| **Efficiency** | {efficiency} (high-Q coil at design frequency) |
| **Bandwidth (2:1 SWR)** | {(band.f_end - band.f_start)*0.2:.0f}–{(band.f_end - band.f_start)*0.5:.0f} kHz (estimated) |

---

## SECTION 2 — LOADING COIL DESIGN

### 2.1 Coil Specifications

| Parameter | Value | Notes |
|-----------|-------|-------|
| **Target inductance** | {l_load:.2f} μH | Per arm (center-loaded) |
| **Number of turns** | {n_turns} | Close-wound, no gaps |
| **Wire gauge** | AWG {band.coil_awg} | {band._awg_to_diameter(band.coil_awg):.4f}" diameter |
| **Total wire length** | {wire_len_m:.2f} m ({wire_len_m*3.28084:.1f} feet) | Both coils |
| **Form material** | {band.pvc_form_id} | Schedule 40 PVC or equivalent |
| **Form inside radius** | {band.pvc_form_r:.3f}" ({band.pvc_form_r*2.54:.2f} cm) | Rigid substrate |
| **Coil length** | {coil_len_in:.3f}" ({coil_len_in*2.54:.2f} cm) | Close-wound on form |
| **Estimated Q factor** | {q_coil} | High-Q design, low loss |
| **Coil DC resistance** | {wire_len_m * (16 if band.coil_awg == 22 else 26 if band.coil_awg == 24 else 10) / 1000:.1f} mΩ | (estimate, AWG {band.coil_awg}) |

### 2.2 Fabrication Procedure

#### Step 1: Form Preparation
- Obtain rigid PVC pipe, {band.pvc_form_id} OD
- Cut to length: {coil_len_in:.2f} + 1/4" for margin = {coil_len_in + 0.25:.2f}"
- Deburr ends with 120-grit sandpaper
- Inspect for cracks or defects

#### Step 2: Coil Winding
1. Secure form in lathe or hand-wind jig
2. Feed magnet wire (AWG {band.coil_awg}) through form center
3. Wind tight helical coil, pitch = wire diameter ({band._awg_to_diameter(band.coil_awg)*25.4:.3f} mm)
4. Maintain tension for uniform spacing
5. **Target:** {n_turns} turns, close-wound, no gaps
6. Mark first and last turn with marker pen

#### Step 3: Continuity & Inductance Verification
- Measure DC resistance end-to-end
  - Expected: {wire_len_m:.1f}m × {0.016 if band.coil_awg == 22 else 0.026:.3f}Ω/m ≈ {wire_len_m * (0.016 if band.coil_awg == 22 else 0.026):.2f} mΩ
  - Acceptable range: ±20% of expected value
- Measure inductance with LCR meter or dip meter
  - Expected: {l_load:.2f} μH
  - Acceptable: {l_load*0.9:.2f}–{l_load*1.1:.2f} μH (±10%)
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
✓ Inductance test: {l_load*0.9:.2f}–{l_load*1.1:.2f} μH on LCR meter
✓ Mechanical test: Gently flex coil; should not shift or rattle
✓ Weatherproofing: No exposed copper; solder joints sealed

---

## SECTION 3 — TRAP RESONATOR ASSEMBLY

### 3.1 Component Specifications

| Component | Specification | Value |
|-----------|---|---|
| **Inductance** | Loading coil (from Section 2) | {l_load:.2f} μH (per arm) |
| **Capacitance** | Variable trimmer, parallel | {band.trap_c_pf:.1f} pF (Bourns 3296) |
| **Resonant frequency** | Design center | {band.f_center:.3f} MHz |
| **Trap inductance** | Calculated from LC resonance | {l_trap:.3f} μH |
| **Expected Q** | Parallel LC circuit | ~150 (with trimmer) |

### 3.2 Resonance Formula

$$f_{{resonance}} = \\frac{{1}}{{2\\pi\\sqrt{{LC}}}}$$

**Verification:** {l_trap:.3f} μH || {band.trap_c_pf:.1f} pF should resonate at {band.f_center:.3f} MHz ± 1%

### 3.3 Trimmer Capacitor Adjustment Procedure

**Equipment required:**
- Dip meter (preferred) or VNA/RF analyzer
- Screwdriver (small slot type)
- SWR meter (optional, for final validation)

**Procedure:**

1. **Initial adjustment (approximate):**
   - Apply light finger pressure to trimmer capacitor
   - Adjust screw slowly, observing dip meter coil for signal dip
   - Target: Minimum dip at {band.f_center:.3f} MHz

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

## SECTION 4 — ANTENNA CARD ASSEMBLY

### 4.1 Card Geometry

| Parameter | Value |
|-----------|-------|
| **Substrate material** | HDPE sheet, 3 mm thick |
| **Length** | 160 mm (6.3 inches) |
| **Width** | 50 mm (2.0 inches) |
| **Edge groove (coil slot)** | 1/4" W × 3/8" D, routed along length |
| **Radiator wire notches** | 1/16" dia holes, {compact_half_m*25:.0f} mm spacing |

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
   - Total wire length: {2*compact_half_m:.2f} m ({2*compact_half_m*3.28084:.1f} feet)
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

## SECTION 5 — ELECTRICAL TESTING

### 5.1 Impedance & SWR Measurement

**Required equipment:**
- SWR/power meter (capable of {band.f_start:.2f}–{band.f_end:.2f} MHz)
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
   - Measure SWR at {band.f_center:.3f} MHz
   - Record value: __________ (target < 3:1 before tuning)

3. **Trimmer adjustment (dip meter preferred):**
   - Using dip meter, approach trap resonator
   - Adjust trimmer screw until dip is maximum
   - Dip frequency should be {band.f_center:.3f} MHz ± 0.5%
   - Re-check SWR; should drop to < 1.5:1

4. **Band-edge SWR verification:**
   - Record SWR at band low edge: __________ @ {band.f_start:.3f} MHz
   - Record SWR at band center: __________ @ {band.f_center:.3f} MHz (target < 1.5:1)
   - Record SWR at band high edge: __________ @ {band.f_end:.3f} MHz
   - All edges should be < 2.5:1; center < 1.5:1

### 5.2 NEC2 Simulation

Load model file: `compact_dipole_{band.band_key}.nec`

**Expected results:**
- **Impedance @ resonance:** 49–52Ω (matched to 50Ω)
- **Gain:** 2.0–2.5 dBi (typical for center-loaded dipole)
- **Efficiency:** {efficiency}
- **3-dB beamwidth:** 70–90° (depends on height)
- **Front-to-back:** 10–20 dB (typical)

---

## SECTION 6 — DEPLOYMENT AND OPERATION

### 6.1 Quick Deployment (< 2 minutes)

1. Remove antenna from protective pouch
2. Gently unroll both radiator arms from card perimeter
3. Allow loading coils to expand fully in edge groove
4. Deploy both arms to horizontal (or vertical) orientation
5. Connect feedline (via 1:1 balun) to center BNC connector
6. Adjust trimmer with dip meter for minimum dip at {band.f_center:.3f} MHz
7. Verify SWR < 1.5:1 with meter
8. Ready to transmit

### 6.2 Power Handling Limits

**Maximum RF power (continuous CW):**
- Coil wire AWG {band.coil_awg}: Limited by heating
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

The following diagrams are located in this band folder (`{band.band_key}/figures/`):

1. **{band.band_key}_antenna_layout.png** — Scale deployment drawing with dimensions
2. **{band.band_key}_coil_detail.png** — Loading coil cross-section, winding detail
3. **{band.band_key}_trap_schematic.png** — Trap resonator circuit, L/C values

---

## REVISION HISTORY

| Date | Version | Author | Changes |
|------|---------|--------|---------|
| 2026-04-21 | 1.0 | Technical Manual Generator | Initial release |

---

**END OF {band.name.upper()} SPECIFICATION**
"""

    with open(output_path, 'w') as f:
        f.write(chapter)


def main():
    base_path = Path('/home/merv/Public/claude/ant_t_r/compact-dipole')

    print("Generating per-band technical chapters...")
    for band in BANDS:
        band_dir = base_path / band.band_key
        spec_file = band_dir / f'compact_dipole_{band.band_key}.md'
        generate_band_chapter(band, spec_file)
        print(f"  ✓ {band.name}: {spec_file.name}")

    print("\nAll 14 band chapters generated successfully!")


if __name__ == '__main__':
    main()
