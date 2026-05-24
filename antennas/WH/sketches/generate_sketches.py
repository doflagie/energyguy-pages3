#!/usr/bin/env python3
"""
generate_sketches.py — TM-EF-SKT-001
Generates technical sketch PNGs for EFQW antenna manual.
Run: python3 generate_sketches.py
Output: efqw_overview.png, trap_detail.png, rollup_assembly.png
"""

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.lines as mlines
import matplotlib.patches as patches
from matplotlib.patches import FancyArrowPatch, Arc
import numpy as np
import os

OUT_DIR = os.path.dirname(os.path.abspath(__file__))

DARK  = '#1a1a2e'
PANEL = '#16213e'
BLUE  = '#0f3460'
CYAN  = '#00d4ff'
GREEN = '#00ff88'
YELLOW= '#ffd700'
RED   = '#ff4444'
WHITE = '#e8e8e8'
GRAY  = '#888888'
ORANGE= '#ff8c00'

def save(fig, name):
    path = os.path.join(OUT_DIR, name)
    fig.savefig(path, dpi=150, bbox_inches='tight',
                facecolor=DARK, edgecolor='none')
    plt.close(fig)
    print(f"  Saved: {path}")

# ── SKETCH 1: EFQW Overview (all 15 bands, size comparison) ───────────────────
def sketch_overview():
    fig, axes = plt.subplots(1, 2, figsize=(16, 10), facecolor=DARK)
    fig.suptitle('END-FED QUARTER-WAVE ANTENNA — ALL BANDS\nTM-EF-SKT-001  Rev A',
                 color=CYAN, fontsize=13, fontweight='bold', fontfamily='monospace')

    bands = [
        ('160m', 1.900, 38.29), ('80m',  3.750, 19.40),
        ('40m',  7.150, 10.18), ('30m', 10.125,  7.19),
        ('20m', 14.175,  5.13), ('17m', 18.118,  4.01),
        ('15m', 21.225,  3.43), ('12m', 24.940,  2.92),
        ('10m', 28.500,  2.55), ('6m',  51.000,  1.40),
        ('2m', 146.000,  0.488),('1.25m',222.00, 0.321),
        ('70cm',446.000, 0.160),('33cm', 906.00, 0.079),
        ('20cm',1297.00, 0.055),
    ]

    # Left: bar chart of element lengths
    ax = axes[0]
    ax.set_facecolor(PANEL)
    names  = [b[0] for b in bands]
    lengths = [b[2] for b in bands]
    colors  = [CYAN if i < 9 else GREEN if i < 12 else YELLOW for i in range(len(bands))]
    y = np.arange(len(bands))
    bars = ax.barh(y, lengths, color=colors, edgecolor=DARK, linewidth=0.5)
    ax.set_yticks(y)
    ax.set_yticklabels(names, color=WHITE, fontfamily='monospace', fontsize=9)
    ax.set_xlabel('Element Length (meters)', color=WHITE, fontfamily='monospace')
    ax.set_title('QUARTER-WAVE ELEMENT LENGTHS', color=CYAN,
                 fontfamily='monospace', fontsize=10)
    ax.tick_params(colors=WHITE)
    ax.spines[:].set_color(GRAY)
    ax.set_xscale('log')
    for bar, (band, freq, L) in zip(bars, bands):
        ax.text(L*1.05, bar.get_y()+bar.get_height()/2,
                f'{L:.3f}m', va='center', ha='left',
                color=WHITE, fontsize=7, fontfamily='monospace')
    legend_elems = [
        mpatches.Patch(color=CYAN,  label='HF (160m–10m)'),
        mpatches.Patch(color=GREEN, label='VHF (6m–1.25m)'),
        mpatches.Patch(color=YELLOW,label='UHF (70cm–20cm)'),
    ]
    ax.legend(handles=legend_elems, facecolor=PANEL, edgecolor=GRAY,
              labelcolor=WHITE, fontsize=8, loc='lower right')
    ax.set_xlim(0.04, 80)

    # Right: antenna geometry diagram
    ax2 = axes[1]
    ax2.set_facecolor(PANEL)
    ax2.set_xlim(-1.5, 2.5)
    ax2.set_ylim(-0.5, 5.5)
    ax2.set_aspect('equal')
    ax2.axis('off')
    ax2.set_title('EFQW PORTABLE GEOMETRY', color=CYAN,
                  fontfamily='monospace', fontsize=10)

    # Ground level
    ax2.axhline(0.3, color=GRAY, linewidth=1, linestyle='--')
    ax2.text(-1.4, 0.1, 'GROUND LEVEL', color=GRAY, fontsize=7, fontfamily='monospace')

    # Pole
    ax2.add_patch(patches.Rectangle((-0.05, 0.3), 0.08, 4.0, color='#8B4513', zorder=3))
    ax2.text(0.15, 3.5, 'Fiberglass\npole', color=WHITE, fontsize=7, fontfamily='monospace')

    # Element wire
    ax2.plot([0, 0], [0.5, 4.5], color=CYAN, linewidth=3, zorder=4, label='QW element')
    ax2.annotate('', xy=(0, 4.5), xytext=(0, 0.5),
                 arrowprops=dict(arrowstyle='->', color=CYAN, lw=1.5))
    ax2.text(0.1, 4.6, 'TIP (open)', color=CYAN, fontsize=7, fontfamily='monospace')

    # Feedpoint
    ax2.add_patch(patches.Rectangle((-0.4, 0.45), 0.8, 0.3, color=BLUE,
                                     edgecolor=YELLOW, linewidth=1.5, zorder=5))
    ax2.text(-0.38, 0.55, 'FEEDPOINT\nL-NETWORK', color=YELLOW, fontsize=6,
             fontfamily='monospace')

    # Coax
    ax2.plot([-0.8, -0.4], [0.6, 0.6], color=ORANGE, linewidth=2, zorder=4)
    ax2.text(-1.4, 0.65, '50Ω coax\n→ TRX', color=ORANGE, fontsize=7, fontfamily='monospace')

    # Radials
    angles = [30, 150, 210, 330]
    r_len = 1.0
    for ang in angles:
        dx = r_len * np.cos(np.radians(ang))
        dy = r_len * np.sin(np.radians(ang)) * 0.2
        ax2.plot([0, dx], [0.3, 0.3+dy], color=GREEN, linewidth=1.5, zorder=4)
    ax2.text(-1.4, 0.0, 'Radials (4×,\nlaid on ground)', color=GREEN,
             fontsize=7, fontfamily='monospace')

    # Dimension arrows
    ax2.annotate('', xy=(0.8, 4.5), xytext=(0.8, 0.6),
                 arrowprops=dict(arrowstyle='<->', color=WHITE, lw=1))
    ax2.text(0.9, 2.5, 'QW =\nλ/4×VF', color=WHITE, fontsize=7, fontfamily='monospace')

    # Band labels at different heights (conceptual)
    for i, (bname, freq, L_m) in enumerate(bands[:5]):
        frac = L_m / bands[0][2]
        ypos = 0.6 + frac * 3.9
        ax2.plot([0, 0.05], [ypos, ypos], color=YELLOW, linewidth=0.5, alpha=0.6)

    ax2.text(-1.4, 5.2, 'VF = 0.97 (HF wire)', color=GRAY, fontsize=7, fontfamily='monospace')
    ax2.text(-1.4, 5.0, 'VF = 0.95 (VHF/UHF rod)', color=GRAY, fontsize=7, fontfamily='monospace')

    plt.tight_layout(rect=[0, 0, 1, 0.93])
    save(fig, 'efqw_overview.png')

# ── SKETCH 2: Trap Detail ──────────────────────────────────────────────────────
def sketch_trap_detail():
    fig, axes = plt.subplots(2, 2, figsize=(14, 10), facecolor=DARK)
    fig.suptitle('LC TRAP CONSTRUCTION DETAIL\nTM-EF-SKT-002  Rev A',
                 color=CYAN, fontsize=13, fontweight='bold', fontfamily='monospace')

    # ── Subplot A: Trap schematic ──────────────────────────────────────────────
    ax = axes[0][0]
    ax.set_facecolor(PANEL)
    ax.set_xlim(0, 10)
    ax.set_ylim(0, 8)
    ax.axis('off')
    ax.set_title('A: PARALLEL LC TRAP CIRCUIT', color=CYAN,
                 fontfamily='monospace', fontsize=9)

    # Wire in / out
    ax.annotate('', xy=(2.5, 4), xytext=(0.5, 4),
                arrowprops=dict(arrowstyle='->', color=CYAN, lw=2))
    ax.text(0.3, 4.2, 'Wire\nIN', color=CYAN, fontsize=8, fontfamily='monospace')
    ax.annotate('', xy=(9.5, 4), xytext=(7.5, 4),
                arrowprops=dict(arrowstyle='->', color=CYAN, lw=2))
    ax.text(9.3, 4.2, 'Wire\nOUT', color=CYAN, fontsize=8, fontfamily='monospace')

    # L symbol (zigzag)
    lx = [2.5, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 5.5]
    ly = [4,   5.5, 6.0, 5.5, 6.0, 5.5, 6.0, 5.5, 4  ]
    ax.plot(lx, ly, color=YELLOW, linewidth=2)
    ax.text(3.5, 6.5, 'L₁  (toroid)', color=YELLOW, fontsize=8, fontfamily='monospace')

    # C symbol (parallel plates)
    cx = [2.5, 2.5]
    for yp in [2.5, 1.8]:
        ax.plot([2.5, 5.5], [yp, yp], color=GREEN, linewidth=2.5)
    ax.plot([2.5, 2.5], [4, 2.5], color=GREEN, linewidth=1.5)
    ax.plot([5.5, 5.5], [4, 2.5], color=GREEN, linewidth=1.5)
    ax.plot([2.5, 2.5], [1.8, 0.8], color=GREEN, linewidth=1.5)
    ax.plot([5.5, 5.5], [1.8, 0.8], color=GREEN, linewidth=1.5)
    ax.plot([2.5, 5.5], [0.8, 0.8], color=GREEN, linewidth=1.5)
    ax.text(3.5, 0.2, 'C₁  (NP0 cap)', color=GREEN, fontsize=8, fontfamily='monospace')

    # Connecting wires
    ax.plot([2.5, 2.5], [5.5, 4.0], color=WHITE, linewidth=1.5)
    ax.plot([5.5, 5.5], [5.5, 4.0], color=WHITE, linewidth=1.5)
    ax.plot([7.5, 5.5], [4.0, 4.0], color=CYAN, linewidth=2)

    ax.text(0.5, 7.5, 'At f₀: Z_trap → HIGH (open circuit)',
            color=WHITE, fontsize=8, fontfamily='monospace')
    ax.text(0.5, 7.0, 'Below f₀: inductive', color=YELLOW, fontsize=7, fontfamily='monospace')
    ax.text(0.5, 6.5, 'Above f₀: capacitive', color=GREEN, fontsize=7, fontfamily='monospace')

    # ── Subplot B: Impedance vs frequency ─────────────────────────────────────
    ax = axes[0][1]
    ax.set_facecolor(PANEL)
    ax.set_title('B: TRAP IMPEDANCE vs FREQUENCY', color=CYAN,
                 fontfamily='monospace', fontsize=9)
    f = np.linspace(0.5, 3.0, 1000)
    f0 = 1.5
    Q = 200
    L = 1e-6; C = 1/(4*np.pi**2*f0**2*1e12*L)*1e12*1e-12
    omega = 2*np.pi*f*1e7
    omega0 = 2*np.pi*f0*1e7
    ZL = omega * L
    ZC = 1/(omega * C)
    Zp = np.abs(ZL*ZC / (ZL - ZC + ZL*ZC/( Q*np.sqrt(L/C) )))
    # Simplified magnitude: Q*sqrt(L/C) at resonance, Lorentzian shape
    Zp = Q*np.sqrt(L/C) / (1 + Q**2*(f/f0 - f0/f)**2)
    ax.semilogy(f, Zp, color=CYAN, linewidth=2)
    ax.axvline(f0, color=YELLOW, linewidth=1, linestyle='--', label=f'f₀ = resonance')
    ax.axhline(50, color=ORANGE, linewidth=1, linestyle=':', label='Z_wire = 50Ω')
    ax.fill_between(f, 50, Zp, where=Zp>50, alpha=0.2, color=CYAN)
    ax.set_xlabel('Frequency (normalized)', color=WHITE, fontfamily='monospace')
    ax.set_ylabel('|Z| (Ω)', color=WHITE, fontfamily='monospace')
    ax.tick_params(colors=WHITE)
    ax.spines[:].set_color(GRAY)
    ax.legend(facecolor=PANEL, edgecolor=GRAY, labelcolor=WHITE, fontsize=7)
    ax.set_facecolor(PANEL)
    ax.text(0.6, 5000, 'INDUCTIVE', color=YELLOW, fontsize=8, fontfamily='monospace')
    ax.text(1.7, 5000, 'CAPACITIVE', color=GREEN, fontsize=8, fontfamily='monospace')
    ax.text(1.4, 20000, f'Z_peak = Q√(L/C)\n≈ 5,000–20,000 Ω', color=CYAN,
            fontsize=7, fontfamily='monospace')

    # ── Subplot C: Toroid winding cross-section ────────────────────────────────
    ax = axes[1][0]
    ax.set_facecolor(PANEL)
    ax.set_title('C: TOROID WINDING TECHNIQUE', color=CYAN,
                 fontfamily='monospace', fontsize=9)
    ax.set_xlim(-3, 3); ax.set_ylim(-3, 3)
    ax.set_aspect('equal'); ax.axis('off')

    toroid_outer = plt.Circle((0,0), 2.0, fill=False, edgecolor=ORANGE, linewidth=3)
    toroid_inner = plt.Circle((0,0), 1.0, fill=False, edgecolor=ORANGE, linewidth=3)
    toroid_fill  = patches.Wedge((0,0), 2.0, 0, 360, width=1.0,
                                  facecolor='#8B4513', alpha=0.6, edgecolor=ORANGE)
    ax.add_patch(toroid_fill)
    ax.add_patch(toroid_outer)
    ax.add_patch(toroid_inner)

    N = 7
    for i in range(N):
        angle = (i / N) * 360
        r_mid = 1.5
        x = r_mid * np.cos(np.radians(angle))
        y = r_mid * np.sin(np.radians(angle))
        turn = plt.Circle((x*0.95, y*0.95), 0.18, color=CYAN, zorder=5)
        ax.add_patch(turn)

    ax.text(0, -2.7, f'N = 7 turns shown (40m example)', color=WHITE,
            fontsize=8, fontfamily='monospace', ha='center')
    ax.text(0, -3.0, 'Pass wire through hole for each turn', color=GRAY,
            fontsize=7, fontfamily='monospace', ha='center')
    ax.annotate('', xy=(1.2, 0.8), xytext=(2.3, 1.5),
                arrowprops=dict(arrowstyle='->', color=CYAN))
    ax.text(2.2, 1.7, 'Each\nturn', color=CYAN, fontsize=7, fontfamily='monospace')
    ax.text(-1.0, 0.0, 'Core\n(T50-2\nor T50-6)', color=ORANGE, fontsize=7,
            fontfamily='monospace', ha='center')

    # ── Subplot D: Trap housing cross-section ──────────────────────────────────
    ax = axes[1][1]
    ax.set_facecolor(PANEL)
    ax.set_title('D: TRAP HOUSING CROSS-SECTION (3D PRINTED)', color=CYAN,
                 fontfamily='monospace', fontsize=9)
    ax.set_xlim(0, 10); ax.set_ylim(0, 8)
    ax.axis('off')

    # Housing outline
    housing = patches.FancyBboxPatch((1, 1), 8, 6, boxstyle='round,pad=0.1',
                                      facecolor='#2a5298', edgecolor=WHITE, linewidth=2)
    ax.add_patch(housing)
    ax.text(5, 7.3, '20mm', color=WHITE, fontsize=8, fontfamily='monospace', ha='center')
    ax.annotate('', xy=(9.1, 7.1), xytext=(0.9, 7.1),
                arrowprops=dict(arrowstyle='<->', color=WHITE, lw=1))

    # Toroid inside
    toroid = plt.Circle((4.0, 4.2), 1.4, fill=False, edgecolor=ORANGE, linewidth=3)
    toroid_c = plt.Circle((4.0, 4.2), 0.7, facecolor='#8B4513', edgecolor=ORANGE, linewidth=2)
    ax.add_patch(patches.Wedge((4.0,4.2), 1.4, 0, 360, width=0.7,
                               facecolor='#8B4513', alpha=0.5, edgecolor=ORANGE))
    ax.add_patch(toroid)
    ax.text(4.0, 4.2, 'Core', color=WHITE, fontsize=7, fontfamily='monospace', ha='center')

    # Capacitor
    ax.add_patch(patches.Rectangle((6.5, 3.7), 1.5, 1.0, facecolor=GREEN, alpha=0.7,
                                   edgecolor=WHITE))
    ax.text(7.25, 4.15, 'C₁', color=DARK, fontsize=9, fontfamily='monospace', ha='center')

    # Wire grommets
    for gy in [2.5, 5.5]:
        ax.add_patch(plt.Circle((1.0, gy), 0.25, facecolor=GRAY, edgecolor=WHITE))
    ax.text(0.2, 2.4, 'Wire\ngland', color=GRAY, fontsize=6, fontfamily='monospace')

    # Lid outline
    lid = patches.FancyBboxPatch((1, 6.4), 8, 1.0, boxstyle='round,pad=0.05',
                                  facecolor='#1a3a6e', edgecolor=WHITE,
                                  linewidth=1.5, linestyle='--')
    ax.add_patch(lid)
    ax.text(5, 6.85, '── SNAP-FIT LID ──', color=WHITE, fontsize=7,
            fontfamily='monospace', ha='center')

    ax.text(1.2, 0.6, 'PETG print, UV-stable; 15mm total length', color=GRAY,
            fontsize=7, fontfamily='monospace')

    plt.tight_layout(rect=[0, 0, 1, 0.93])
    save(fig, 'trap_detail.png')

# ── SKETCH 3: Roll-Up Assembly ─────────────────────────────────────────────────
def sketch_rollup():
    fig, axes = plt.subplots(1, 2, figsize=(16, 10), facecolor=DARK)
    fig.suptitle('EFQW ROLL-UP MULTIBAND ANTENNA — ASSEMBLY DIAGRAM\nTM-EF-SKT-003  Rev A',
                 color=CYAN, fontsize=13, fontweight='bold', fontfamily='monospace')

    # ── Left: Deployed antenna diagram ────────────────────────────────────────
    ax = axes[0]
    ax.set_facecolor(PANEL)
    ax.set_xlim(-3, 4)
    ax.set_ylim(-1, 12)
    ax.axis('off')
    ax.set_title('DEPLOYED (40m configuration)', color=CYAN,
                 fontfamily='monospace', fontsize=10)

    # Ground
    ax.fill_between([-3, 4], [-1, -1], [0, 0], color='#3a2a1a', alpha=0.6)
    ax.axhline(0, color='#5a4a3a', linewidth=2)
    ax.text(-2.8, -0.7, 'GROUND / EARTH', color='#5a4a3a', fontsize=8, fontfamily='monospace')

    # Support pole
    ax.add_patch(patches.Rectangle((-0.06, 0), 0.10, 10, color='#ddd', alpha=0.5, zorder=3))

    # Feedpoint box
    ax.add_patch(patches.FancyBboxPatch((-0.6, 0.8), 1.2, 0.6,
                 boxstyle='round,pad=0.05', facecolor=BLUE, edgecolor=YELLOW, linewidth=1.5))
    ax.text(0, 1.1, 'FEEDPOINT\n+L-NET', color=YELLOW, fontsize=6.5,
            fontfamily='monospace', ha='center', va='center')

    # Coax
    ax.plot([-1.5, -0.6], [1.1, 1.1], color=ORANGE, linewidth=2.5, zorder=4)
    ax.text(-2.8, 1.2, '50Ω coax\nto TRX', color=ORANGE, fontsize=7.5, fontfamily='monospace')

    # Radials
    radial_angles = [210, 240, 300, 330]
    for ang in radial_angles:
        dx = 2.5 * np.cos(np.radians(ang))
        dy = 0.1 * np.sin(np.radians(ang))
        ax.plot([0, dx], [0.2, 0.2+dy], color=GREEN, linewidth=1.5, zorder=4)
    ax.text(-2.8, -0.1, 'Radials × 4\n(quarter-wave each)', color=GREEN,
            fontsize=7.5, fontfamily='monospace')

    # Trap positions (40m config: traps at 2.55, 2.92, 3.43, 5.13, end at 10.18)
    trap_pos = [
        (2.55, '10m TRAP\nf₀=28.5MHz'),
        (2.92, '12m TRAP\nf₀=24.9MHz'),
        (3.43, '15m TRAP\nf₀=21.2MHz'),
        (5.13, '20m TRAP\nf₀=14.2MHz'),
    ]
    element_top = 10.18
    y_scale = 9.0 / element_top
    y_base = 1.4

    # Main wire
    ax.plot([0, 0], [y_base, y_base + element_top*y_scale], color=CYAN, linewidth=2.5, zorder=5)

    for pos, label in trap_pos:
        y = y_base + pos * y_scale
        ax.add_patch(patches.Rectangle((-0.25, y-0.12), 0.5, 0.24,
                     facecolor=YELLOW, edgecolor=DARK, linewidth=1, zorder=6))
        ax.plot([0.25, 1.5], [y, y], color=GRAY, linewidth=0.8, linestyle='--')
        ax.text(1.55, y-0.05, label, color=YELLOW, fontsize=6.5, fontfamily='monospace')

    # Tip
    ax.plot([0], [y_base + element_top*y_scale], 'o', color=RED, markersize=6, zorder=7)
    ax.text(0.15, y_base + element_top*y_scale, 'TIP (open)\n40m end',
            color=RED, fontsize=7, fontfamily='monospace')

    # Dimension arrow
    ax.annotate('', xy=(2.8, y_base + element_top*y_scale),
                xytext=(2.8, y_base),
                arrowprops=dict(arrowstyle='<->', color=WHITE, lw=1.2))
    ax.text(3.0, (y_base + y_base + element_top*y_scale)/2,
            f'{element_top}m\n(40m config)', color=WHITE, fontsize=7.5, fontfamily='monospace')

    # ── Right: Roll-up spool and deployment steps ──────────────────────────────
    ax2 = axes[1]
    ax2.set_facecolor(PANEL)
    ax2.set_xlim(0, 10)
    ax2.set_ylim(0, 12)
    ax2.axis('off')
    ax2.set_title('ROLL-UP SPOOL + DEPLOYMENT STEPS', color=CYAN,
                  fontfamily='monospace', fontsize=10)

    # Spool diagram
    ax2.add_patch(patches.Rectangle((0.5, 8.5), 4.5, 3.0, facecolor='#3a3a5a', edgecolor=WHITE))
    spool_center = (2.75, 10.0)
    ax2.add_patch(plt.Circle(spool_center, 1.2, facecolor='#2a2a4a', edgecolor=CYAN))
    ax2.add_patch(plt.Circle(spool_center, 0.4, facecolor='#1a1a3a', edgecolor=GRAY))
    theta = np.linspace(0, 6*np.pi, 200)
    for i, r_base in enumerate([0.5, 0.7, 0.9, 1.1]):
        r = r_base + 0.02 * theta / (2*np.pi)
        x = spool_center[0] + r * np.cos(theta)
        y = spool_center[1] + r * np.sin(theta)
        mask = (x >= 0.5) & (x <= 5.0) & (y >= 8.5) & (y <= 11.5)
        ax2.plot(x[mask], y[mask], color=CYAN, linewidth=0.8, alpha=0.6)
    ax2.text(2.75, 7.9, 'WIRE SPOOL\n(traps pre-installed)', color=CYAN,
             fontsize=8, fontfamily='monospace', ha='center')
    ax2.text(4.7, 9.8, '← Antenna wire\nwound here', color=WHITE,
             fontsize=7, fontfamily='monospace')

    # Trap pills attached to spool
    for i, (pos, label) in enumerate(trap_pos):
        ty = 11.2 - i * 0.5
        ax2.add_patch(patches.FancyBboxPatch((5.5, ty-0.15), 1.2, 0.3,
                      boxstyle='round,pad=0.05', facecolor=YELLOW, edgecolor=DARK))
        ax2.text(6.1, ty, label.split('\n')[0], color=DARK, fontsize=5.5,
                 fontfamily='monospace', ha='center', va='center')

    # Deployment steps
    steps = [
        ('1', 'Extend fiberglass pole 5–10m'),
        ('2', 'Attach feedpoint box to pole\n   base (velcro + bungee)'),
        ('3', 'Lay out 4 radials at base\n   (or tie elevated)'),
        ('4', 'Unreel wire vertically up pole\n   for selected band length'),
        ('5', 'Secure tip to pole tip\n   with loose loop'),
        ('6', 'Connect 50Ω coax, tune C\n   for minimum SWR'),
    ]
    for i, (num, step) in enumerate(steps):
        y = 7.0 - i * 1.1
        ax2.add_patch(plt.Circle((0.8, y), 0.3, facecolor=CYAN, zorder=5))
        ax2.text(0.8, y, num, color=DARK, fontsize=10, fontweight='bold',
                 fontfamily='monospace', ha='center', va='center', zorder=6)
        ax2.text(1.3, y, step, color=WHITE, fontsize=7.5, fontfamily='monospace', va='center')

    # Band selection table
    ax2.text(0.3, 0.9, 'BAND SELECTION:', color=CYAN, fontsize=8,
             fontfamily='monospace', fontweight='bold')
    band_rows = [
        ('10m', '2.55m', 'Unroll to 10m trap'),
        ('12m', '2.92m', 'Unroll to 12m trap'),
        ('15m', '3.43m', 'Unroll to 15m trap'),
        ('20m', '5.13m', 'Unroll to 20m trap'),
        ('40m', '10.18m','Unroll full length'),
    ]
    ax2.text(0.3, 0.5, f"{'Band':<6} {'Length':<8} Action", color=GRAY,
             fontsize=7, fontfamily='monospace')
    for j, (b, l, a) in enumerate(band_rows):
        ax2.text(0.3, 0.1 - j*0.35, f"{b:<6} {l:<8} {a}",
                 color=WHITE, fontsize=7, fontfamily='monospace')

    plt.tight_layout(rect=[0, 0, 1, 0.93])
    save(fig, 'rollup_assembly.png')

if __name__ == '__main__':
    print("Generating EFQW antenna sketches...")
    sketch_overview()
    sketch_trap_detail()
    sketch_rollup()
    print("Done.")
