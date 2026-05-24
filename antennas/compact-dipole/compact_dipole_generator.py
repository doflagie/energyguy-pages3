#!/usr/bin/env python3
"""
Compact Spiral-Wound Dipole Antenna Generator
Navy Technical Manual Level — All Bands, NEC Models, CSV Tables, PNG Schematics

Generates:
  - 14 NEC model files (band-center tuned)
  - 50+ PNG schematic diagrams (matplotlib)
  - Consolidated CSV winding tables
  - Per-band technical specifications

Design basis: Center-loaded shortened dipoles (50% HF, 60% VHF, ~95% UHF)
Scalable LC trap design with parallel resonators for manual tuning.
"""

import os
import csv
import math
from pathlib import Path
from dataclasses import dataclass
from typing import List, Tuple

import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from matplotlib.patches import FancyBboxPatch, FancyArrowPatch, Circle, Rectangle, Polygon
import numpy as np


# ============================================================================
# BAND DEFINITIONS & PARAMETERS
# ============================================================================

@dataclass
class BandData:
    """Per-band antenna design parameters"""
    name: str           # "160m", "80m", etc.
    band_key: str       # folder name "160m", "1p25m" (for 1.25m), etc.
    f_center: float     # MHz
    f_start: float      # MHz (band edge, for NEC sweep)
    f_end: float        # MHz
    compaction: float   # 0.50 for 50%, 0.60 for 60%, etc.
    pvc_form_id: str    # e.g., "1.5 PVC", "3/4 PVC"
    pvc_form_r: float   # radius in inches (for Wheeler formula)
    radiator_awg: int   # AWG of main dipole arms
    coil_awg: int       # AWG of loading coil wire
    trap_c_pf: float    # Trap capacitor value in pF

    def get_full_half_length_m(self) -> float:
        """Half-dipole length at full size (λ/4, velocity factor 0.95)"""
        return 71.3 / self.f_center

    def get_compact_half_length_m(self) -> float:
        """Compact half-dipole length with compaction factor"""
        return self.get_full_half_length_m() * self.compaction

    def get_loading_inductance_uh(self) -> float:
        """Loading coil inductance per arm, scaled from 40m baseline"""
        # L_40m = 20.9 μH @ 7.15 MHz
        return 20.9 * (7.15 / self.f_center) ** 2

    def get_trap_inductance_uh(self) -> float:
        """Trap resonator inductance: L = 1/((2πf)²C)"""
        f_rad_s = 2 * math.pi * self.f_center * 1e6  # radians/sec
        c_farad = self.trap_c_pf * 1e-12
        l_henry = 1.0 / (f_rad_s ** 2 * c_farad)
        return l_henry * 1e6  # Convert to μH

    def get_coil_params(self) -> Tuple[int, float]:
        """Calculate coil turns and length using Wheeler's formula"""
        l_target_uh = self.get_loading_inductance_uh()
        r = self.pvc_form_r  # radius in inches
        wire_dia_inches = self._awg_to_diameter(self.coil_awg)

        # Wheeler: L = r² × N² / (9r + 10ℓ)
        # where ℓ = N × wire_dia (close-wound)
        # Rearrange: L(9r + 10×N×dia) = r²×N²
        # L×9r + L×10×N×dia = r²×N²
        # r²×N² - L×10×dia×N - L×9r = 0

        a = r ** 2
        b = -l_target_uh * 10 * wire_dia_inches
        c = -l_target_uh * 9 * r

        disc = b**2 - 4*a*c
        if disc < 0:
            # Fallback: coarse estimate
            n_turns = int(math.sqrt(l_target_uh * (9*r) / (r**2)))
        else:
            n_turns = int((-b + math.sqrt(disc)) / (2*a))

        coil_len_inches = n_turns * wire_dia_inches
        return n_turns, coil_len_inches

    def _awg_to_diameter(self, awg: int) -> float:
        """AWG to diameter in inches (standard conversion)"""
        # d = 0.005 * 92^((36-AWG)/39)
        d_inches = 0.005 * (92 ** ((36 - awg) / 39))
        return d_inches


# All 14 bands
BANDS = [
    BandData(name="160m", band_key="160m", f_center=1.900, f_start=1.800, f_end=2.000,
             compaction=0.50, pvc_form_id="1.5\" PVC", pvc_form_r=0.750,
             radiator_awg=14, coil_awg=22, trap_c_pf=30.0),

    BandData(name="80m", band_key="80m", f_center=3.750, f_start=3.500, f_end=4.000,
             compaction=0.50, pvc_form_id="1\" PVC", pvc_form_r=0.625,
             radiator_awg=14, coil_awg=22, trap_c_pf=25.0),

    BandData(name="40m", band_key="40m", f_center=7.150, f_start=7.000, f_end=7.300,
             compaction=0.50, pvc_form_id="3/4\" PVC", pvc_form_r=0.525,
             radiator_awg=16, coil_awg=22, trap_c_pf=15.0),

    BandData(name="30m", band_key="30m", f_center=10.125, f_start=10.100, f_end=10.150,
             compaction=0.50, pvc_form_id="3/4\" PVC", pvc_form_r=0.525,
             radiator_awg=16, coil_awg=24, trap_c_pf=12.0),

    BandData(name="20m", band_key="20m", f_center=14.175, f_start=14.000, f_end=14.350,
             compaction=0.50, pvc_form_id="3/4\" PVC", pvc_form_r=0.525,
             radiator_awg=18, coil_awg=24, trap_c_pf=10.0),

    BandData(name="17m", band_key="17m", f_center=18.118, f_start=18.068, f_end=18.168,
             compaction=0.50, pvc_form_id="1/2\" PVC", pvc_form_r=0.400,
             radiator_awg=18, coil_awg=24, trap_c_pf=8.0),

    BandData(name="15m", band_key="15m", f_center=21.225, f_start=21.000, f_end=21.450,
             compaction=0.50, pvc_form_id="1/2\" PVC", pvc_form_r=0.400,
             radiator_awg=18, coil_awg=26, trap_c_pf=7.0),

    BandData(name="12m", band_key="12m", f_center=24.940, f_start=24.890, f_end=24.990,
             compaction=0.50, pvc_form_id="1/2\" PVC", pvc_form_r=0.400,
             radiator_awg=20, coil_awg=26, trap_c_pf=6.0),

    BandData(name="10m", band_key="10m", f_center=28.850, f_start=28.000, f_end=29.700,
             compaction=0.50, pvc_form_id="1/2\" PVC", pvc_form_r=0.400,
             radiator_awg=20, coil_awg=26, trap_c_pf=5.0),

    BandData(name="6m", band_key="6m", f_center=52.000, f_start=50.000, f_end=54.000,
             compaction=0.60, pvc_form_id="3/8\" PVC", pvc_form_r=0.280,
             radiator_awg=22, coil_awg=28, trap_c_pf=4.0),

    BandData(name="2m", band_key="2m", f_center=146.000, f_start=144.000, f_end=148.000,
             compaction=0.60, pvc_form_id="3/8\" PVC", pvc_form_r=0.280,
             radiator_awg=22, coil_awg=28, trap_c_pf=3.0),

    BandData(name="1.25m", band_key="1p25m", f_center=222.500, f_start=220.000, f_end=225.000,
             compaction=0.95, pvc_form_id="1/4\" FR4", pvc_form_r=0.200,
             radiator_awg=24, coil_awg=30, trap_c_pf=2.0),

    BandData(name="70cm", band_key="70cm", f_center=435.000, f_start=420.000, f_end=450.000,
             compaction=0.95, pvc_form_id="1/4\" ceramic", pvc_form_r=0.180,
             radiator_awg=24, coil_awg=30, trap_c_pf=1.5),

    BandData(name="33cm", band_key="33cm", f_center=915.000, f_start=902.000, f_end=928.000,
             compaction=0.95, pvc_form_id="1/8\" ceramic", pvc_form_r=0.125,
             radiator_awg=28, coil_awg=30, trap_c_pf=1.0),

    BandData(name="20cm", band_key="20cm", f_center=1270.000, f_start=1240.000, f_end=1300.000,
             compaction=0.95, pvc_form_id="1/8\" ceramic", pvc_form_r=0.125,
             radiator_awg=28, coil_awg=30, trap_c_pf=0.8),
]


# ============================================================================
# NEC MODEL GENERATION
# ============================================================================

def generate_nec_model(band: BandData, output_path: Path) -> None:
    """Generate NEC2 model file for given band"""
    arm_len_m = band.get_compact_half_length_m()
    l_load_uh = band.get_loading_inductance_uh()
    l_load_h = l_load_uh * 1e-6
    l_trap_uh = band.get_trap_inductance_uh()
    l_trap_h = l_trap_uh * 1e-6

    # Coil Q factor estimate
    r_coil_ohms = 0.5  # rough estimate for thin wire coil

    # NEC geometry: two arms, center-fed
    # GW format: tag, segments, x1, y1, z1, x2, y2, z2, wire_radius
    wire_r_m = band._awg_to_diameter(band.radiator_awg) * 0.0254 / 2  # inches to m

    nec_lines = [
        f"CM Compact spiral-wound dipole — {band.name} BAND",
        f"CM Center frequency: {band.f_center:.3f} MHz",
        f"CM Compact arm length: {arm_len_m:.3f} m | Full length: {band.get_full_half_length_m():.3f} m",
        f"CM Compaction: {band.compaction*100:.0f}%",
        f"CM Loading coil: {l_load_uh:.2f} μH per arm | Form: {band.pvc_form_id}",
        f"CM Trap resonator: {l_trap_uh:.3f} μH || {band.trap_c_pf:.1f} pF",
        f"CM Wire: AWG {band.radiator_awg} ({band._awg_to_diameter(band.radiator_awg):.4f} in)",
        f"CM Height above ground: 5.0 m (approx 0.5λ at lower HF bands)",
        "CE",
        "",
        "' Dipole arms, center-fed",
        f"GW 1 11 -{arm_len_m:.6f} 0.000 5.000  0.000 0.000 5.000  {wire_r_m:.8f}",
        f"GW 2 11  0.000 0.000 5.000  {arm_len_m:.6f} 0.000 5.000  {wire_r_m:.8f}",
        "",
        "' Loading coil inductances (LD 4 = series RL)",
        f"LD 4 1 6 6 {l_load_h:.10e} 0 {r_coil_ohms:.2f}",
        f"LD 4 2 6 6 {l_load_h:.10e} 0 {r_coil_ohms:.2f}",
        "",
        "GE 0",
        "GN 2 0 0 0 13 0.005",
        "EX 0 1 11 0 1 0",
        f"FR 0 {int((band.f_end - band.f_start) / 0.025)} 0 0 {band.f_start:.3f} {0.025:.3f}",
        "RP 0 91 360 1000 -90 0 1.0 1.0",
        "EN",
    ]

    with open(output_path, 'w') as f:
        f.write('\n'.join(nec_lines))


# ============================================================================
# PNG SCHEMATIC GENERATION
# ============================================================================

def draw_antenna_layout(band: BandData, output_path: Path) -> None:
    """Draw scaled antenna layout diagram with dimensions"""
    fig, ax = plt.subplots(figsize=(12, 6), dpi=100)
    ax.set_aspect('equal')

    arm_len_m = band.get_compact_half_length_m()
    full_len_m = band.get_full_half_length_m()

    # Draw outer wire (radiator)
    outer_pct = 0.3  # outer 30% is radiator
    outer_len_m = arm_len_m * outer_pct
    inner_len_m = arm_len_m - outer_len_m

    # Left arm
    ax.plot([-outer_len_m, 0], [0, 0], 'r-', linewidth=3, label='Outer radiator')
    ax.plot([0, inner_len_m], [0, 0], 'b-', linewidth=2, alpha=0.7, label='Loading coil section')

    # Right arm
    ax.plot([0, outer_len_m], [0, 0], 'r-', linewidth=3)
    ax.plot([0, -inner_len_m], [0, 0], 'b-', linewidth=2, alpha=0.7)

    # Feed point
    ax.plot(0, 0, 'ko', markersize=10, label='Feed point')

    # Trap locations
    ax.plot([-outer_len_m], [0], 'g^', markersize=12, label='Trap (L||C)')
    ax.plot([outer_len_m], [0], 'g^', markersize=12)

    # Dimensions
    ax.annotate('', xy=(-outer_len_m, -0.3), xytext=(0, -0.3),
                arrowprops=dict(arrowstyle='<->', color='black', lw=1.5))
    ax.text(-outer_len_m/2, -0.5, f'{outer_len_m:.2f}m\n({outer_pct*100:.0f}%)',
            ha='center', fontsize=10, bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.7))

    ax.annotate('', xy=(inner_len_m, -0.3), xytext=(0, -0.3),
                arrowprops=dict(arrowstyle='<->', color='black', lw=1.5))
    ax.text(inner_len_m/2, -0.5, f'{inner_len_m:.2f}m\n(loading)',
            ha='center', fontsize=10, bbox=dict(boxstyle='round', facecolor='lightblue', alpha=0.7))

    ax.text(-outer_len_m/2, 0.4, f'Radiator', ha='center', fontsize=9, color='red')
    ax.text(inner_len_m/2, 0.4, f'Coil', ha='center', fontsize=9, color='blue')

    # Title and info
    ax.set_title(f'{band.name} Compact Dipole — Center {band.f_center:.3f} MHz\n' +
                 f'Compact: {arm_len_m:.2f}m | Full: {full_len_m:.2f}m | Reduction: {(1-band.compaction)*100:.0f}%',
                 fontsize=12, fontweight='bold')

    ax.set_xlim(-arm_len_m*1.3, arm_len_m*1.3)
    ax.set_ylim(-1.2, 1.0)
    ax.set_xlabel('Distance (meters)', fontsize=10)
    ax.legend(loc='upper right', fontsize=9)
    ax.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig(output_path, dpi=150, bbox_inches='tight')
    plt.close()


def draw_coil_detail(band: BandData, output_path: Path) -> None:
    """Draw loading coil cross-section with turn annotations"""
    fig, ax = plt.subplots(figsize=(10, 6), dpi=100)

    n_turns, coil_len_in = band.get_coil_params()
    l_coil_uh = band.get_loading_inductance_uh()
    r_form = band.pvc_form_r

    # Draw coil form (circle cross-section)
    form_circle = Circle((0, 0), r_form, fill=False, edgecolor='brown', linewidth=3, label=f'Form ({band.pvc_form_id})')
    ax.add_patch(form_circle)

    # Draw wire cross-sections as small circles spiraling outward
    wire_dia_in = band._awg_to_diameter(band.coil_awg)
    wire_r = wire_dia_in / 2
    coil_pitch = wire_dia_in

    # Simplified: show turns as circles at radius r_form + wire_r
    for i in range(min(n_turns, 20)):  # Show first 20 turns
        angle = (i / 20) * 2 * math.pi
        x = (r_form + wire_r) * math.cos(angle)
        y = (r_form + wire_r) * math.sin(angle)
        circle = Circle((x, y), wire_r, fill=True, facecolor='lightcoral',
                       edgecolor='red', linewidth=1, alpha=0.8)
        ax.add_patch(circle)

    # Annotations
    ax.text(0, -r_form-0.5, f'{band.pvc_form_id} PVC Form\nID = {2*r_form:.3f}"',
            ha='center', fontsize=11, bbox=dict(boxstyle='round', facecolor='lightyellow'))

    ax.text(r_form+0.3, r_form+0.3, f'{n_turns} turns\nAWG {band.coil_awg}\nL = {l_coil_uh:.2f} μH',
            fontsize=10, bbox=dict(boxstyle='round', facecolor='lightgreen', alpha=0.8))

    ax.text(0, r_form+0.8, f'{band.name} Loading Coil Cross-Section\n' +
            f'Length: {coil_len_in:.2f}" | Wire spacing: {coil_pitch:.4f}"',
            ha='center', fontsize=12, fontweight='bold')

    ax.set_xlim(-1.5, 1.5)
    ax.set_ylim(-1.5, 1.5)
    ax.set_aspect('equal')
    ax.axis('off')
    plt.tight_layout()
    plt.savefig(output_path, dpi=150, bbox_inches='tight')
    plt.close()


def draw_trap_schematic(band: BandData, output_path: Path) -> None:
    """Draw trap resonator circuit diagram (L || C)"""
    fig, ax = plt.subplots(figsize=(10, 6), dpi=100)

    l_trap = band.get_trap_inductance_uh()
    c_trap = band.trap_c_pf

    # Draw circuit: Input --[ inductor ]--+--[ capacitor ]--+-- Ground
    #                                      |                 |
    #                                    Adjust for Q

    # Input terminals
    ax.plot([1, 3], [5, 5], 'k-', linewidth=2)
    ax.text(0.5, 5, 'IN', fontsize=12, fontweight='bold', ha='right', va='center')

    # Inductor symbol (coil)
    x_ind_start = 3
    n_coils = 6
    coil_width = 0.3
    for i in range(n_coils):
        x = x_ind_start + i * coil_width
        circle = Circle((x, 5), coil_width/2, fill=False, edgecolor='blue', linewidth=2)
        ax.add_patch(circle)
    ax.text(x_ind_start + n_coils*coil_width/2, 5.7, f'{l_trap:.2f} μH (Coil)',
            ha='center', fontsize=10, color='blue', fontweight='bold')

    # Junction
    x_junc = x_ind_start + n_coils * coil_width
    ax.plot([x_junc, x_junc], [5, 3], 'k-', linewidth=2)
    ax.plot([x_junc, x_junc], [3, 2], 'k-', linewidth=2)

    # Capacitor symbol (two parallel lines)
    cap_x = x_junc
    cap_len = 0.3
    ax.plot([cap_x-cap_len, cap_x+cap_len], [2.7, 2.7], 'r-', linewidth=3)
    ax.plot([cap_x-cap_len, cap_x+cap_len], [2.3, 2.3], 'r-', linewidth=3)
    ax.plot([cap_x, cap_x], [2.7, 2.3], 'k-', linewidth=2)
    ax.text(cap_x+0.5, 2.5, f'{c_trap:.1f} pF (Trimmer)', fontsize=10, color='red', fontweight='bold')

    # Ground
    ax.plot([cap_x, cap_x], [2.3, 2], 'k-', linewidth=2)
    ax.plot([cap_x-0.3, cap_x+0.3], [2, 2], 'k-', linewidth=3)
    ax.plot([cap_x-0.2, cap_x+0.2], [1.8, 1.8], 'k-', linewidth=2)
    ax.plot([cap_x-0.1, cap_x+0.1], [1.6, 1.6], 'k-', linewidth=1)
    ax.text(cap_x-0.7, 1.8, 'GND', fontsize=11, fontweight='bold')

    # Frequency and Q annotations
    resonant_f = 1e6 / (2 * math.pi * math.sqrt(l_trap*1e-6 * c_trap*1e-12))
    ax.text(4, 0.5, f'Resonant frequency: {resonant_f/1e6:.3f} MHz\nQ ≈ 200 (HF) / 300 (VHF)',
            fontsize=11, bbox=dict(boxstyle='round', facecolor='lightyellow', alpha=0.8))

    ax.set_title(f'{band.name} Parallel LC Trap Resonator\n' +
                 f'{band.f_center:.3f} MHz | L = {l_trap:.3f} μH | C = {c_trap:.1f} pF',
                 fontsize=12, fontweight='bold')

    ax.set_xlim(0, 8)
    ax.set_ylim(0, 6.5)
    ax.axis('off')
    plt.tight_layout()
    plt.savefig(output_path, dpi=150, bbox_inches='tight')
    plt.close()


# ============================================================================
# CSV GENERATION
# ============================================================================

def generate_winding_table_csv(band: BandData, output_path: Path) -> None:
    """Generate per-band winding table CSV"""
    n_turns, coil_len_in = band.get_coil_params()
    l_coil = band.get_loading_inductance_uh()
    l_trap = band.get_trap_inductance_uh()
    c_trap = band.trap_c_pf
    wire_dia = band._awg_to_diameter(band.coil_awg)
    wire_len_m = n_turns * 2 * math.pi * band.pvc_form_r * 0.0254

    rows = [
        ['COMPACT DIPOLE ANTENNA — ' + band.name, '', '', ''],
        ['Center Frequency', f'{band.f_center:.3f}', 'MHz', ''],
        ['', '', '', ''],

        ['LOADING COIL PARAMETERS', '', '', ''],
        ['Inductance Target', f'{l_coil:.2f}', 'μH', 'per arm'],
        ['Number of Turns', f'{n_turns}', 'turns', 'close-wound'],
        ['Wire Gauge', f'AWG {band.coil_awg}', '', f'{wire_dia:.4f}" diameter'],
        ['Wire Length Total', f'{wire_len_m:.2f}', 'm', f'{wire_len_m*3.28084:.2f} feet'],
        ['Form Material', band.pvc_form_id, '', 'rigid PVC or ceramic'],
        ['Form Inside Radius', f'{band.pvc_form_r:.3f}', 'inches', f'{band.pvc_form_r*2.54:.2f} cm'],
        ['Coil Length', f'{coil_len_in:.3f}', 'inches', f'{coil_len_in*2.54:.2f} cm'],
        ['Coil Pitch', f'{wire_dia:.4f}', 'inches', f'{wire_dia*2.54:.3f} cm (same as wire dia)'],
        ['', '', '', ''],

        ['TRAP RESONATOR PARAMETERS', '', '', ''],
        ['Inductance', f'{l_trap:.3f}', 'μH', 'parallel element'],
        ['Capacitance', f'{c_trap:.1f}', 'pF', 'variable trimmer (Bourns 3296)'],
        ['Resonant Frequency', f'{band.f_center:.3f}', 'MHz', 'design center'],
        ['', '', '', ''],

        ['RADIATOR PARAMETERS', '', '', ''],
        ['Wire Gauge', f'AWG {band.radiator_awg}', '', f'{band._awg_to_diameter(band.radiator_awg):.4f}" diameter'],
        ['Arm Length (Compact)', f'{band.get_compact_half_length_m():.3f}', 'm', f'{band.get_compact_half_length_m()*3.28084:.2f} feet'],
        ['Arm Length (Full)', f'{band.get_full_half_length_m():.3f}', 'm', f'{band.get_full_half_length_m()*3.28084:.2f} feet'],
        ['Compaction Factor', f'{band.compaction*100:.0f}%', '', 'size reduction vs full dipole'],
    ]

    with open(output_path, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerows(rows)


def generate_global_trap_specs_csv(bands: List[BandData], output_path: Path) -> None:
    """Generate consolidated trap specifications for all bands"""
    rows = [
        ['TRAP SPECIFICATIONS — ALL BANDS', '', '', '', '', ''],
        ['Band', 'Freq (MHz)', 'L (μH)', 'C (pF)', 'Form Size', 'Turns*', 'Coil Len (in)'],
    ]

    for band in bands:
        n_turns, coil_len_in = band.get_coil_params()
        rows.append([
            band.name,
            f'{band.f_center:.3f}',
            f'{band.get_trap_inductance_uh():.3f}',
            f'{band.trap_c_pf:.1f}',
            band.pvc_form_id,
            f'{n_turns}',
            f'{coil_len_in:.2f}',
        ])

    rows.append(['', '', '', '', '', '', ''])
    rows.append(['*Turns are for loading coil; trap uses parallel LC resonator'])

    with open(output_path, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerows(rows)


def generate_global_winding_tables_csv(bands: List[BandData], output_path: Path) -> None:
    """Generate consolidated winding table for all bands"""
    rows = [
        ['WINDING TABLES — ALL BANDS', '', '', '', '', '', '', ''],
        ['Band', 'f (MHz)', 'L_coil (μH)', 'Turns', 'Form', 'Wire (AWG)', 'Wire Len (m)', 'Coil Len (in)'],
    ]

    for band in bands:
        n_turns, coil_len_in = band.get_coil_params()
        l_coil = band.get_loading_inductance_uh()
        wire_dia = band._awg_to_diameter(band.coil_awg)
        wire_len_m = n_turns * 2 * math.pi * band.pvc_form_r * 0.0254

        rows.append([
            band.name,
            f'{band.f_center:.3f}',
            f'{l_coil:.2f}',
            f'{n_turns}',
            band.pvc_form_id,
            f'{band.coil_awg}',
            f'{wire_len_m:.1f}',
            f'{coil_len_in:.2f}',
        ])

    with open(output_path, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerows(rows)


# ============================================================================
# GLOBAL FIGURES
# ============================================================================

def draw_global_figures(base_path: Path) -> None:
    """Generate 8 global schematic figures"""

    # Fig 1: Dipole overview
    fig, ax = plt.subplots(figsize=(14, 6), dpi=100)

    # Side-view dipole with annotations
    arm_len = 2.5  # arbitrary units for diagram
    ax.plot([-arm_len, 0], [1, 1], 'r-', linewidth=4, label='Outer radiator')
    ax.plot([0, arm_len], [1, 1], 'r-', linewidth=4)

    # Loading coil zones
    outer = 0.3 * arm_len
    ax.plot([-outer, 0], [0.7, 0.7], 'b-', linewidth=3, alpha=0.7, label='Loading coil section')
    ax.plot([0, outer], [0.7, 0.7], 'b-', linewidth=3, alpha=0.7)

    # Traps
    ax.plot([-outer], [0.7], 'g^', markersize=15, label='Trap (L||C)')
    ax.plot([outer], [0.7], 'g^', markersize=15)

    # Feed
    ax.plot(0, 1, 'ko', markersize=12)
    ax.text(0.15, 1.15, 'Feed Point\n(50Ω)', fontsize=10, fontweight='bold')

    # Labels
    ax.text(-arm_len/2, 1.4, 'Radiator arm (straight wire)', ha='center', fontsize=11, color='red')
    ax.text(arm_len/2, 1.4, 'Radiator arm (straight wire)', ha='center', fontsize=11, color='red')
    ax.text(-outer/2, 0.35, 'Loading coil', ha='center', fontsize=10, color='blue')
    ax.text(outer/2, 0.35, 'Loading coil', ha='center', fontsize=10, color='blue')

    # Side view: coil form
    ax.add_patch(Rectangle((-outer-0.05, 0.6), 0.1, 0.2, facecolor='brown', alpha=0.6, label='PVC form'))
    ax.add_patch(Rectangle((outer-0.05, 0.6), 0.1, 0.2, facecolor='brown', alpha=0.6))

    ax.set_xlim(-arm_len*1.3, arm_len*1.3)
    ax.set_ylim(0, 1.8)
    ax.set_title('Compact Spiral-Wound Dipole — Overall Architecture\n' +
                 'Roll-up card design with center loading coils and end traps',
                 fontsize=13, fontweight='bold')
    ax.axis('off')
    ax.legend(loc='upper center', fontsize=10)
    plt.tight_layout()
    plt.savefig(base_path / 'figures' / 'fig_01_dipole_overview.png', dpi=150, bbox_inches='tight')
    plt.close()

    # Fig 3: Trap schematic (generic)
    fig, ax = plt.subplots(figsize=(10, 5), dpi=100)

    # Generic LC circuit
    ax.text(2, 4.5, 'PARALLEL LC TRAP', fontsize=12, fontweight='bold')
    ax.text(1, 3.5, 'Operates at band center frequency:', fontsize=10)
    ax.text(1, 3, 'Z_trap = R (series resistance only at resonance)', fontsize=9, style='italic')
    ax.text(1, 2.5, 'Isolation improves > 1 octave from resonance', fontsize=9, style='italic')

    ax.text(1, 1.5, 'Trimmer capacitor allows fine tuning ±5%', fontsize=10,
            bbox=dict(boxstyle='round', facecolor='lightyellow'))
    ax.text(1, 0.8, 'Use 0-50pF or 0-100pF Bourns 3296 trimmer', fontsize=9)
    ax.text(1, 0.2, 'Mount trap in weatherproof shrink tube', fontsize=9)

    ax.set_xlim(0, 5)
    ax.set_ylim(0, 5)
    ax.axis('off')
    plt.tight_layout()
    plt.savefig(base_path / 'figures' / 'fig_03_trap_schematic.png', dpi=150, bbox_inches='tight')
    plt.close()

    # Other figures: text-based (simplified for now)
    for fig_num, (title, desc) in enumerate([
        ('fig_02_roll_up_card_assembly.png', 'Roll-up antenna card: HDPE substrate, grooved wire channels,\ncoil wound in edge slot, connectors at center'),
        ('fig_04_parallel_resonator.png', 'Feed-point block: BNC connector, balun, center LC matching network'),
        ('fig_05_coil_winding_detail.png', 'Close-wound helical coil on PVC form, pitch = wire diameter'),
        ('fig_06_feedpoint_detail.png', 'Connector detail: BNC or SMA, 1:1 balun, ground plane optional'),
        ('fig_07_deployment_sequence.png', '1) Rolled card 2) Unroll radiators 3) Deploy fully 4) Adjust trimmers'),
        ('fig_08_tuning_procedure.png', 'Use dip meter: adjust trimmer for minimum dip at band center'),
    ], 2):
        fig, ax = plt.subplots(figsize=(10, 5), dpi=100)
        ax.text(0.5, 0.5, title.replace('.png', '').replace('_', ' ').upper(),
                ha='center', va='center', fontsize=14, fontweight='bold', transform=ax.transAxes)
        ax.text(0.5, 0.3, desc, ha='center', va='center', fontsize=11, transform=ax.transAxes,
                bbox=dict(boxstyle='round', facecolor='lightyellow', alpha=0.8))
        ax.axis('off')
        plt.tight_layout()
        plt.savefig(base_path / 'figures' / title, dpi=150, bbox_inches='tight')
        plt.close()


# ============================================================================
# MAIN EXECUTION
# ============================================================================

def main():
    base_path = Path('/home/merv/Public/claude/ant_t_r/compact-dipole')
    base_path.mkdir(parents=True, exist_ok=True)

    print("Compact Dipole Antenna Generator — Navy Technical Manual Level")
    print("=" * 70)

    # Generate NEC models
    print("\n[1/5] Generating NEC model files...")
    for band in BANDS:
        nec_path = base_path / band.band_key / f'compact_dipole_{band.band_key}.nec'
        generate_nec_model(band, nec_path)
        print(f"  ✓ {band.name}: {nec_path.name}")

    # Generate per-band PNGs
    print("\n[2/5] Generating per-band schematic diagrams...")
    for band in BANDS:
        fig_dir = base_path / band.band_key / 'figures'
        fig_dir.mkdir(parents=True, exist_ok=True)

        draw_antenna_layout(band, fig_dir / f'{band.band_key}_antenna_layout.png')
        draw_coil_detail(band, fig_dir / f'{band.band_key}_coil_detail.png')
        draw_trap_schematic(band, fig_dir / f'{band.band_key}_trap_schematic.png')
        print(f"  ✓ {band.name}: 3 diagrams")

    # Generate per-band CSV winding tables
    print("\n[3/5] Generating per-band winding tables...")
    for band in BANDS:
        csv_path = base_path / band.band_key / f'winding_table_{band.band_key}.csv'
        generate_winding_table_csv(band, csv_path)
        print(f"  ✓ {band.name}: {csv_path.name}")

    # Generate global CSV files
    print("\n[4/5] Generating consolidated CSV specifications...")
    generate_global_trap_specs_csv(BANDS, base_path / 'trap_specifications.csv')
    generate_global_winding_tables_csv(BANDS, base_path / 'winding_tables_all_bands.csv')
    print("  ✓ trap_specifications.csv")
    print("  ✓ winding_tables_all_bands.csv")

    # Generate global figures
    print("\n[5/5] Generating global schematic figures...")
    draw_global_figures(base_path)
    print("  ✓ 8 global figures")

    print("\n" + "=" * 70)
    print("Generation complete!")
    print(f"Output directory: {base_path}")
    print(f"Total files: ~111 (14 bands × 7 files + global)")


if __name__ == '__main__':
    main()
