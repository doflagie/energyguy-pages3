#!/usr/bin/env python3
"""
generate_sketches.py
5.6m Telescopic Whip Antenna — Technical Diagram Generator
Generates three PNG diagrams for the roll-up wire trap version.

Output files (same directory as script):
  whip_overview.png
  loading_coil_detail.png
  rollup_whip_assembly.png
"""

import os
import math
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.patches as mplpatches
from matplotlib.patches import FancyArrowPatch, FancyBboxPatch, Arc, Circle, Rectangle
from matplotlib.lines import Line2D

# ── colour palette ──────────────────────────────────────────────────────────
BLUE   = '#2196F3'
GREEN  = '#4CAF50'
RED    = '#F44336'
YELLOW = '#FFC107'
CYAN   = '#00BCD4'
ORANGE = '#FF9800'
WHITE  = '#FFFFFF'
LGRAY  = '#B0BEC5'
DGRAY  = '#37474F'
BGND   = '#121212'

plt.style.use('dark_background')

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))


# ════════════════════════════════════════════════════════════════════════════
# PNG 1 — whip_overview.png
# ════════════════════════════════════════════════════════════════════════════

def make_whip_overview():
    fig = plt.figure(figsize=(16, 10), facecolor=BGND)
    fig.suptitle('5.6 m Telescopic Whip — Overview', fontsize=16,
                 color=WHITE, fontweight='bold', y=0.97)

    ax_bar = fig.add_axes([0.04, 0.08, 0.44, 0.84])   # left panel
    ax_whip = fig.add_axes([0.54, 0.06, 0.42, 0.88])  # right panel

    # ── band data ──
    bands = [
        # (name, freq_MHz, QW_m, group)
        ('160m', 1.900,  39.47, 'HF'),
        ('80m',  3.750,  20.00, 'HF'),
        ('60m',  5.330,  14.07, 'HF'),
        ('40m',  7.150,  10.49, 'HF'),
        ('30m',  10.125,  7.41, 'HF'),
        ('20m',  14.175,  5.29, 'HF'),
        ('17m',  18.118,  4.14, 'HF'),
        ('15m',  21.225,  3.54, 'HF'),
        ('12m',  24.940,  3.01, 'HF'),
        ('10m',  28.500,  2.63, 'HF'),
        ('6m',   51.000,  1.47, 'VHF'),
        ('4m',   70.000,  1.07, 'VHF'),
        ('2m',   146.000, 0.514,'VHF'),
        ('1.25m',222.000, 0.338,'VHF'),
        ('70cm', 432.000, 0.173,'UHF'),
    ]

    colours = {'HF': BLUE, 'VHF': GREEN, 'UHF': RED}
    names  = [b[0] for b in bands]
    lengths = [b[2] for b in bands]
    cols   = [colours[b[3]] for b in bands]

    y_pos = list(range(len(bands)))

    # ── Panel A: horizontal bar chart ──
    ax_bar.set_facecolor(BGND)
    bars = ax_bar.barh(y_pos, lengths, color=cols, height=0.65, edgecolor=LGRAY,
                       linewidth=0.4)

    ax_bar.set_xscale('log')
    ax_bar.set_xlim(0.1, 60)
    ax_bar.axvline(x=5.6, color=YELLOW, linewidth=2, linestyle='--', label='5.6 m whip')
    ax_bar.set_yticks(y_pos)
    ax_bar.set_yticklabels(names, color=WHITE, fontsize=9)
    ax_bar.set_xlabel('Quarter-wave length (m)', color=LGRAY, fontsize=10)
    ax_bar.set_title('QW Length vs. Band', color=WHITE, fontsize=12, pad=8)
    ax_bar.tick_params(colors=LGRAY)
    ax_bar.spines['bottom'].set_color(DGRAY)
    ax_bar.spines['top'].set_color(DGRAY)
    ax_bar.spines['left'].set_color(DGRAY)
    ax_bar.spines['right'].set_color(DGRAY)
    ax_bar.grid(axis='x', color=DGRAY, linestyle=':', alpha=0.6)

    for i, (bar, b) in enumerate(zip(bars, bands)):
        x_label = b[2] * 1.05
        ax_bar.text(x_label, i, f'{b[2]:.2f} m', va='center',
                    color=WHITE, fontsize=7.5)

    legend_elems = [
        mpatches.Patch(color=BLUE,  label='HF (160–10m)'),
        mpatches.Patch(color=GREEN, label='VHF (6m–1.25m)'),
        mpatches.Patch(color=RED,   label='UHF (70cm)'),
        Line2D([0], [0], color=YELLOW, linestyle='--', label='5.6 m whip'),
    ]
    ax_bar.legend(handles=legend_elems, loc='lower right', fontsize=8,
                  facecolor=DGRAY, edgecolor=LGRAY, labelcolor=WHITE)

    # ── Panel B: whip diagram ──
    ax_whip.set_facecolor(BGND)
    ax_whip.set_xlim(-1.2, 1.2)
    ax_whip.set_ylim(-0.6, 6.2)
    ax_whip.set_aspect('equal')
    ax_whip.axis('off')
    ax_whip.set_title('Full Extension: 5.6 m', color=WHITE, fontsize=12, pad=8)

    # Radials
    radial_angles = [45, 135, 225, 315]
    radial_len = 0.55
    for ang in radial_angles:
        rad = math.radians(ang)
        ax_whip.plot([0, radial_len * math.cos(rad)],
                     [0, radial_len * math.sin(rad)],
                     color=CYAN, linewidth=2, solid_capstyle='round')

    # Base loading coil box
    coil_box = FancyBboxPatch((-0.30, -0.55), 0.60, 0.45,
                               boxstyle='round,pad=0.02',
                               linewidth=1.5, edgecolor=YELLOW,
                               facecolor=DGRAY)
    ax_whip.add_patch(coil_box)
    ax_whip.text(0, -0.325, 'LOADING\nCOIL', ha='center', va='center',
                 color=YELLOW, fontsize=7, fontweight='bold')

    # Whip body — tapered sections
    section_tops = [0.80, 1.50, 2.20, 2.92, 3.65, 4.50, 5.60]
    section_widths = [0.055, 0.046, 0.038, 0.030, 0.022, 0.015, 0.009]
    prev_top = 0.0
    for i, (top, w) in enumerate(zip(section_tops, section_widths)):
        rect = Rectangle((-w/2, prev_top), w, top - prev_top,
                          linewidth=1, edgecolor=LGRAY,
                          facecolor='#455A64' if i % 2 == 0 else '#546E7A')
        ax_whip.add_patch(rect)
        prev_top = top

    # Band marks
    marks = [
        (5.13, '20m', BLUE,   4.01, False),
        (4.01, '17m', CYAN,   4.01, True),
        (3.43, '15m', GREEN,  3.43, True),
        (2.92, '12m', YELLOW, 2.92, True),
        (2.55, '10m', ORANGE, 2.55, True),
        (1.40, '6m',  RED,    1.40, True),
        (0.488,'2m',  '#E91E63', 0.488, True),
    ]
    for (y_pos_m, label, col, pos_val, is_trap) in marks:
        ax_whip.axhline(y=y_pos_m, xmin=0.15, xmax=0.85, color=col,
                        linewidth=1.2, linestyle=':' if not is_trap else '-',
                        alpha=0.85)
        side = 0.25 if is_trap else -0.25
        ha_val = 'left' if is_trap else 'right'
        trap_txt = f'{label} TRAP\n{pos_val:.3f} m' if is_trap else f'{label}\n{pos_val:.2f} m'
        ax_whip.text(side * 4.5, y_pos_m, trap_txt, ha=ha_val, va='center',
                     color=col, fontsize=7.5, fontweight='bold')

    # Top label
    ax_whip.annotate('', xy=(0, 5.6), xytext=(0, 0),
                     arrowprops=dict(arrowstyle='<->', color=WHITE,
                                     lw=1.2, mutation_scale=12))
    ax_whip.text(0.18, 2.8, '5.6 m', ha='left', va='center',
                 color=WHITE, fontsize=10, fontweight='bold',
                 rotation=90)

    ax_whip.text(0, 5.85, '▲ TIP', ha='center', va='bottom',
                 color=LGRAY, fontsize=8)
    ax_whip.text(0, -0.58, '▼ BASE / FEED', ha='center', va='top',
                 color=LGRAY, fontsize=8)

    plt.savefig(os.path.join(SCRIPT_DIR, 'whip_overview.png'),
                dpi=150, bbox_inches='tight', facecolor=BGND)
    plt.close(fig)
    print('  whip_overview.png  saved.')


# ════════════════════════════════════════════════════════════════════════════
# PNG 2 — loading_coil_detail.png
# ════════════════════════════════════════════════════════════════════════════

def make_loading_coil_detail():
    fig = plt.figure(figsize=(16, 12), facecolor=BGND)
    fig.suptitle('5.6 m Whip — Loading Coil Details', fontsize=15,
                 color=WHITE, fontweight='bold', y=0.97)

    ax_eff  = fig.add_axes([0.05, 0.55, 0.40, 0.37])   # top-left
    ax_ind  = fig.add_axes([0.55, 0.55, 0.40, 0.37])   # top-right
    ax_lnet = fig.add_axes([0.05, 0.05, 0.40, 0.40])   # bottom-left
    ax_tor  = fig.add_axes([0.55, 0.05, 0.40, 0.40])   # bottom-right

    # ── Panel A: Efficiency bar chart ──
    bands_eff = ['160m', '80m', '60m', '40m', '30m', '20m']
    eff_vals  = [1.8, 13.9, 34.2, 60.7, 87.7, 98.0]

    def eff_colour(e):
        if e < 20:   return RED
        if e < 50:   return ORANGE
        return GREEN

    bar_cols = [eff_colour(e) for e in eff_vals]
    ax_eff.set_facecolor(BGND)
    bars = ax_eff.bar(bands_eff, eff_vals, color=bar_cols,
                      edgecolor=LGRAY, linewidth=0.5)
    ax_eff.set_ylim(0, 115)
    ax_eff.set_ylabel('Efficiency (%)', color=LGRAY, fontsize=9)
    ax_eff.set_title('Loading Coil Efficiency vs. Band', color=WHITE, fontsize=11)
    ax_eff.tick_params(colors=LGRAY)
    ax_eff.spines[:].set_color(DGRAY)
    ax_eff.grid(axis='y', color=DGRAY, linestyle=':', alpha=0.5)
    for bar, val in zip(bars, eff_vals):
        ax_eff.text(bar.get_x() + bar.get_width()/2, val + 2,
                    f'{val}%', ha='center', va='bottom',
                    color=WHITE, fontsize=8.5, fontweight='bold')
    legend_eff = [
        mpatches.Patch(color=RED,    label='< 20% (poor)'),
        mpatches.Patch(color=ORANGE, label='20–50% (marginal)'),
        mpatches.Patch(color=GREEN,  label='> 50% (good)'),
    ]
    ax_eff.legend(handles=legend_eff, fontsize=8, loc='upper left',
                  facecolor=DGRAY, edgecolor=LGRAY, labelcolor=WHITE)

    # ── Panel B: Inductance vs frequency ──
    ax_ind.set_facecolor(BGND)
    pts = [(1.900, 144.6), (3.750, 35.5), (5.330, 16.3), (7.150, 7.85), (10.125, 2.477)]
    freqs = [p[0] for p in pts]
    inds  = [p[1] for p in pts]

    ax_ind.set_xscale('log')
    ax_ind.set_yscale('log')
    ax_ind.plot(freqs, inds, color=CYAN, linewidth=2, marker='o',
                markersize=8, markerfacecolor=YELLOW, markeredgecolor=WHITE,
                markeredgewidth=1.2)
    for fx, lx in pts:
        ax_ind.annotate(f'{lx} µH', xy=(fx, lx), xytext=(fx * 1.15, lx * 1.3),
                        fontsize=8, color=YELLOW,
                        arrowprops=dict(arrowstyle='->', color=LGRAY, lw=0.8))
    ax_ind.set_xlabel('Frequency (MHz)', color=LGRAY, fontsize=9)
    ax_ind.set_ylabel('Inductance (µH)', color=LGRAY, fontsize=9)
    ax_ind.set_title('Required Loading Inductance vs. Frequency', color=WHITE, fontsize=11)
    ax_ind.tick_params(colors=LGRAY)
    ax_ind.spines[:].set_color(DGRAY)
    ax_ind.grid(color=DGRAY, linestyle=':', alpha=0.5, which='both')
    ax_ind.set_xlim(1.3, 15)
    ax_ind.set_ylim(1.0, 300)

    # ── Panel C: L-network schematic ──
    ax_lnet.set_facecolor(BGND)
    ax_lnet.set_xlim(0, 10)
    ax_lnet.set_ylim(0, 8)
    ax_lnet.axis('off')
    ax_lnet.set_title('L-Network + Loading Coil (Schematic)', color=WHITE, fontsize=11)

    def draw_inductor(ax, x, y, length=1.2, n_bumps=5, color=CYAN, lw=1.8):
        """Draw a simple inductor symbol (arcs)."""
        bump_w = length / n_bumps
        xs, ys = [], []
        for i in range(n_bumps):
            t = np.linspace(0, math.pi, 20)
            xs.extend(x + i * bump_w + bump_w/2 * (1 - np.cos(t)))
            ys.extend(y + bump_w/2 * np.sin(t))
        ax.plot(xs, ys, color=color, linewidth=lw)

    def draw_capacitor(ax, x, y, w=0.3, h=0.5, color=YELLOW, lw=1.8):
        """Draw a capacitor symbol (two parallel lines)."""
        ax.plot([x, x], [y, y + h/2 - 0.05], color=color, linewidth=lw)
        ax.plot([x, x], [y + h/2 + 0.05, y + h], color=color, linewidth=lw)
        ax.plot([x - w/2, x + w/2], [y + h/2 - 0.05, y + h/2 - 0.05],
                color=color, linewidth=lw * 1.5)
        ax.plot([x - w/2, x + w/2], [y + h/2 + 0.05, y + h/2 + 0.05],
                color=color, linewidth=lw * 1.5)

    # Coax input (left side)
    ax_lnet.text(0.3, 3.8, '50Ω\nCOAX', ha='center', va='center',
                 color=WHITE, fontsize=8, fontweight='bold',
                 bbox=dict(boxstyle='round', facecolor=DGRAY, edgecolor=CYAN))
    ax_lnet.plot([0.7, 2.0], [4.0, 4.0], color=WHITE, lw=1.8)   # series line

    # Series inductor L_s
    draw_inductor(ax_lnet, 2.0, 3.7, length=1.4, n_bumps=5, color=CYAN)
    ax_lnet.text(2.7, 3.3, 'L_s\n(series tune)', ha='center', va='top',
                 color=CYAN, fontsize=7.5)

    # Line continuing right
    ax_lnet.plot([3.4, 5.0], [4.0, 4.0], color=WHITE, lw=1.8)

    # Shunt capacitor C_p
    draw_capacitor(ax_lnet, 3.9, 2.8, color=YELLOW)
    ax_lnet.plot([3.9, 3.9], [2.8, 4.0], color=WHITE, lw=1.8)    # up to node
    ax_lnet.plot([3.9, 3.9], [2.8, 2.0], color=WHITE, lw=1.8)    # down to ground
    ax_lnet.plot([3.4, 4.4], [2.0, 2.0], color=LGRAY, lw=2)      # ground
    ax_lnet.plot([3.55, 4.25], [1.85, 1.85], color=LGRAY, lw=1.4)
    ax_lnet.plot([3.7, 4.1], [1.70, 1.70], color=LGRAY, lw=0.9)
    ax_lnet.text(4.6, 3.4, 'C_p\n(shunt)', ha='left', va='center',
                 color=YELLOW, fontsize=7.5)

    # Loading coil L_load
    draw_inductor(ax_lnet, 5.0, 3.7, length=1.6, n_bumps=6, color=GREEN, lw=2.2)
    ax_lnet.text(5.8, 3.2, 'L_load\n(base loading coil)', ha='center', va='top',
                 color=GREEN, fontsize=7.5)

    # Antenna element going up
    ax_lnet.plot([6.6, 7.2], [4.0, 4.0], color=WHITE, lw=1.8)
    ax_lnet.annotate('', xy=(7.2, 7.5), xytext=(7.2, 4.0),
                     arrowprops=dict(arrowstyle='->', color=WHITE, lw=1.8,
                                     mutation_scale=14))
    ax_lnet.text(7.5, 5.8, 'ANTENNA\nELEMENT', ha='left', va='center',
                 color=WHITE, fontsize=8)

    # Ground at coax
    ax_lnet.plot([0.7, 0.7], [3.5, 2.0], color=LGRAY, lw=1.5)
    ax_lnet.plot([0.3, 1.1], [2.0, 2.0], color=LGRAY, lw=2.0)
    ax_lnet.plot([0.4, 1.0], [1.85, 1.85], color=LGRAY, lw=1.4)
    ax_lnet.plot([0.55, 0.85], [1.70, 1.70], color=LGRAY, lw=0.9)

    # Node dots
    for nx, ny in [(3.9, 4.0), (5.0, 4.0), (6.6, 4.0)]:
        ax_lnet.plot(nx, ny, 'o', color=WHITE, markersize=4)

    # ── Panel D: Toroid cross-section ──
    ax_tor.set_facecolor(BGND)
    ax_tor.set_xlim(0, 10)
    ax_tor.set_ylim(0, 8)
    ax_tor.set_aspect('equal')
    ax_tor.axis('off')
    ax_tor.set_title('Toroid Core Size Comparison', color=WHITE, fontsize=11)

    # T200-2 (160m, 31T) — largest
    c1 = Circle((2.5, 5.5), 1.8, fill=True, facecolor='#B71C1C',
                 edgecolor=RED, linewidth=2)
    c1i = Circle((2.5, 5.5), 1.0, fill=True, facecolor=BGND,
                  edgecolor=RED, linewidth=1.5)
    ax_tor.add_patch(c1)
    ax_tor.add_patch(c1i)
    ax_tor.text(2.5, 5.5, 'T200-2\n31T / 160m\nOD=50.8mm', ha='center', va='center',
                color=WHITE, fontsize=6.5, fontweight='bold')

    # T130-2 (80m, 18T)
    c2 = Circle((6.5, 5.5), 1.2, fill=True, facecolor='#1A237E',
                 edgecolor=BLUE, linewidth=2)
    c2i = Circle((6.5, 5.5), 0.65, fill=True, facecolor=BGND,
                  edgecolor=BLUE, linewidth=1.5)
    ax_tor.add_patch(c2)
    ax_tor.add_patch(c2i)
    ax_tor.text(6.5, 5.5, 'T130-2\n18T / 80m\nOD=33mm', ha='center', va='center',
                color=WHITE, fontsize=6.5, fontweight='bold')

    # T106-2 (40m, 8T)
    c3 = Circle((2.8, 2.0), 0.85, fill=True, facecolor='#1B5E20',
                 edgecolor=GREEN, linewidth=1.8)
    c3i = Circle((2.8, 2.0), 0.45, fill=True, facecolor=BGND,
                  edgecolor=GREEN, linewidth=1.2)
    ax_tor.add_patch(c3)
    ax_tor.add_patch(c3i)
    ax_tor.text(2.8, 2.0, 'T106-2\n8T / 40m\nOD=26.9mm', ha='center', va='center',
                color=WHITE, fontsize=5.8, fontweight='bold')

    # T50-2 (30m, 7T) — smallest
    c4 = Circle((5.5, 2.0), 0.50, fill=True, facecolor='#F57F17',
                 edgecolor=YELLOW, linewidth=1.5)
    c4i = Circle((5.5, 2.0), 0.25, fill=True, facecolor=BGND,
                  edgecolor=YELLOW, linewidth=1.0)
    ax_tor.add_patch(c4)
    ax_tor.add_patch(c4i)
    ax_tor.text(5.5, 2.0, 'T50-2\n7T/30m\n12.7mm', ha='center', va='center',
                color=WHITE, fontsize=5.2, fontweight='bold')

    ax_tor.text(5.0, 0.3, '(all drawn to relative scale)', ha='center',
                color=LGRAY, fontsize=7.5, style='italic')

    plt.savefig(os.path.join(SCRIPT_DIR, 'loading_coil_detail.png'),
                dpi=150, bbox_inches='tight', facecolor=BGND)
    plt.close(fig)
    print('  loading_coil_detail.png  saved.')


# ════════════════════════════════════════════════════════════════════════════
# PNG 3 — rollup_whip_assembly.png
# ════════════════════════════════════════════════════════════════════════════

def make_rollup_assembly():
    fig = plt.figure(figsize=(18, 14), facecolor=BGND)
    fig.suptitle('5.6 m Roll-Up Wire — Assembly & Operation', fontsize=16,
                 color=WHITE, fontweight='bold', y=0.97)

    ax_wire  = fig.add_axes([0.02, 0.04, 0.38, 0.90])   # left (tall)
    ax_spool = fig.add_axes([0.46, 0.52, 0.50, 0.40])   # top-right
    ax_table = fig.add_axes([0.46, 0.04, 0.50, 0.42])   # bottom-right

    # ═══════════════════════════════
    # Panel A — wire diagram (tall)
    # ═══════════════════════════════
    ax_wire.set_facecolor(BGND)
    ax_wire.set_xlim(-2.2, 2.2)
    ax_wire.set_ylim(-2.8, 6.2)
    ax_wire.axis('off')
    ax_wire.set_title('Roll-Up Wire Assembly\n(Full Extension — 5.6 m)',
                      color=WHITE, fontsize=12, pad=6)

    # Radials
    for ang in [40, 140, 220, 320]:
        rad = math.radians(ang)
        rx = 1.1 * math.cos(rad)
        ry = 1.1 * math.sin(rad)
        ax_wire.plot([0, rx], [0, ry], color=CYAN, linewidth=2.2,
                     solid_capstyle='round')
        ax_wire.text(rx * 1.15, ry * 1.15, 'radial', ha='center', va='center',
                     color=CYAN, fontsize=6.5)

    # Module stack below feed point
    modules = [
        (-2.55, -2.05, YELLOW, 'LOADING\nCOIL MODULE\n(swappable)',   YELLOW),
        (-2.10, -1.60, CYAN,   'L-NETWORK\n(base match)',             CYAN),
        (-1.65, -1.15, GREEN,  'SWR BRIDGE\n+ COUPLER',               GREEN),
        (-1.20, -0.70, ORANGE, 'ESP32\nCONTROLLER',                   ORANGE),
    ]
    for (y_bot, y_top, col, lbl, tcol) in modules:
        box = FancyBboxPatch((-0.65, y_bot), 1.30, y_top - y_bot,
                              boxstyle='round,pad=0.04',
                              linewidth=1.8, edgecolor=col, facecolor=DGRAY)
        ax_wire.add_patch(box)
        ax_wire.text(0, (y_bot + y_top) / 2, lbl, ha='center', va='center',
                     color=tcol, fontsize=7, fontweight='bold')

    # Connecting line from bottom module to feed
    ax_wire.plot([0, 0], [-0.70, 0], color=WHITE, linewidth=2)
    ax_wire.plot([0, 0], [0, 0], 'o', color=WHITE, markersize=5)

    # Wire above feed
    ax_wire.plot([0, 0], [0, 5.6], color=WHITE, linewidth=2.5,
                 solid_capstyle='round')

    # Trap housings
    traps = [
        (0.488, '2m  TRAP',  '#E91E63'),
        (1.40,  '6m  TRAP',  RED),
        (2.55,  '10m TRAP',  ORANGE),
        (2.92,  '12m TRAP',  YELLOW),
        (3.43,  '15m TRAP',  GREEN),
        (4.01,  '17m TRAP',  CYAN),
    ]
    for (yp, label, col) in traps:
        # Trap box in-line with wire
        trap_box = FancyBboxPatch((-0.18, yp - 0.10), 0.36, 0.20,
                                   boxstyle='round,pad=0.02',
                                   linewidth=1.8, edgecolor=col,
                                   facecolor=BGND, zorder=5)
        ax_wire.add_patch(trap_box)
        ax_wire.text(0.25, yp, label, ha='left', va='center',
                     color=col, fontsize=8, fontweight='bold')
        ax_wire.text(-0.28, yp, f'{yp:.3f} m', ha='right', va='center',
                     color=LGRAY, fontsize=7)
        ax_wire.plot([-0.28, -0.18], [yp, yp], color=LGRAY, lw=0.8)

    # Top tip
    ax_wire.text(0, 5.75, '▲  TIP  (5.6 m)', ha='center', va='bottom',
                 color=LGRAY, fontsize=8)

    # Overall dimension line
    ax_wire.annotate('', xy=(1.6, 5.6), xytext=(1.6, 0.0),
                     arrowprops=dict(arrowstyle='<->', color=LGRAY,
                                     lw=1.2, mutation_scale=12))
    ax_wire.text(1.75, 2.8, '5.6 m', ha='left', va='center',
                 color=LGRAY, fontsize=9, rotation=90)

    # ═══════════════════════════════
    # Panel B — spool diagram
    # ═══════════════════════════════
    ax_spool.set_facecolor(BGND)
    ax_spool.set_xlim(-1.5, 2.5)
    ax_spool.set_ylim(-1.3, 1.4)
    ax_spool.set_aspect('equal')
    ax_spool.axis('off')
    ax_spool.set_title('Wire Spool (Deployed from Field Bag)\n160 mm diameter',
                        color=WHITE, fontsize=11, pad=6)

    # Spool body
    spool_outer = Circle((0, 0), 1.0, fill=False, edgecolor=LGRAY, linewidth=2)
    spool_inner = Circle((0, 0), 0.30, fill=True, facecolor=DGRAY,
                          edgecolor=LGRAY, linewidth=1.5)
    ax_spool.add_patch(spool_outer)
    ax_spool.add_patch(spool_inner)

    # Wound wire rings
    for r in [0.45, 0.60, 0.75, 0.88]:
        ring = Circle((0, 0), r, fill=False, edgecolor='#546E7A',
                       linewidth=1.5, linestyle='-')
        ax_spool.add_patch(ring)

    # Handle
    handle_box = FancyBboxPatch((0.90, -0.12), 0.55, 0.24,
                                 boxstyle='round,pad=0.05',
                                 linewidth=1.5, edgecolor=LGRAY, facecolor=DGRAY)
    ax_spool.add_patch(handle_box)
    ax_spool.text(1.175, 0, 'HANDLE', ha='center', va='center',
                  color=WHITE, fontsize=6.5)

    # Trap housings clipped around rim
    trap_clips = [
        (1.04, 0.0,  '2m',  '#E91E63'),
        (0.74, 0.74, '6m',  RED),
        (0.0,  1.04, '10m', ORANGE),
        (-0.74, 0.74,'12m', YELLOW),
        (-1.04, 0.0, '15m', GREEN),
        (-0.74,-0.74,'17m', CYAN),
    ]
    for (cx, cy, lbl, col) in trap_clips:
        clip_box = FancyBboxPatch((cx - 0.10, cy - 0.08), 0.20, 0.16,
                                   boxstyle='round,pad=0.02',
                                   linewidth=1.5, edgecolor=col, facecolor=DGRAY,
                                   zorder=5)
        ax_spool.add_patch(clip_box)
        ax_spool.text(cx, cy, lbl, ha='center', va='center',
                      color=col, fontsize=5.5, fontweight='bold')

    ax_spool.text(0, -1.20, 'Trap housings clip to spool rim — 6 traps total',
                  ha='center', va='center', color=LGRAY, fontsize=7.5,
                  style='italic')
    ax_spool.text(0, 0, '5.6 m\nwire', ha='center', va='center',
                  color=WHITE, fontsize=6.5)

    # ═══════════════════════════════
    # Panel C — band operation table
    # ═══════════════════════════════
    ax_table.set_facecolor(BGND)
    ax_table.axis('off')
    ax_table.set_title('Band Operation Summary', color=WHITE, fontsize=11, pad=6)

    col_labels = ['Band', 'Active Wire', 'Blocking Trap', 'Loading Coil']
    table_data = [
        ['160m', '5.6 m', 'none',   '144.6 µH  (T200-2 31T)'],
        ['80m',  '5.6 m', 'none',   '35.5 µH   (T130-2 18T)'],
        ['40m',  '5.6 m', 'none',   '7.85 µH   (T106-2 8T)'],
        ['20m',  '5.6 m', 'none',   '300pF cap (series trim)'],
        ['17m',  '4.01 m','17m blocks','none — near resonant'],
        ['15m',  '3.43 m','15m blocks','none — near resonant'],
        ['12m',  '2.92 m','12m blocks','none — near resonant'],
        ['10m',  '2.55 m','10m blocks','none — near resonant'],
        ['6m',   '1.40 m','6m blocks', 'none — near resonant'],
        ['2m',   '0.488 m','2m blocks','none — near resonant'],
    ]

    row_colours = [
        [BLUE,   BLUE,   RED,    YELLOW],
        [BLUE,   BLUE,   RED,    YELLOW],
        [BLUE,   BLUE,   RED,    YELLOW],
        [BLUE,   BLUE,   LGRAY,  YELLOW],
        [CYAN,   GREEN,  CYAN,   LGRAY],
        [CYAN,   GREEN,  CYAN,   LGRAY],
        [CYAN,   GREEN,  CYAN,   LGRAY],
        [CYAN,   GREEN,  CYAN,   LGRAY],
        [GREEN,  GREEN,  GREEN,  LGRAY],
        ['#E91E63','#E91E63','#E91E63',LGRAY],
    ]

    n_rows = len(table_data)
    n_cols = len(col_labels)
    col_widths  = [0.10, 0.12, 0.16, 0.40]
    col_starts  = [0.02, 0.14, 0.28, 0.46]
    row_h = 0.082
    hdr_y = 0.94

    # Header
    for ci, (lbl, xs) in enumerate(zip(col_labels, col_starts)):
        ax_table.text(xs, hdr_y, lbl, ha='left', va='center',
                      color=WHITE, fontsize=9, fontweight='bold',
                      transform=ax_table.transAxes)

    # Header divider line (drawn in axes-fraction coordinates via plot)
    ax_table.plot([0.02, 0.98], [hdr_y - 0.03, hdr_y - 0.03],
                  color=LGRAY, linewidth=1.0,
                  transform=ax_table.transAxes)

    for ri, row in enumerate(table_data):
        y_frac = hdr_y - 0.05 - ri * row_h
        bg_col = '#1C2833' if ri % 2 == 0 else '#212F3D'
        bg_rect = Rectangle((0.01, y_frac - 0.025), 0.97, row_h,
                              transform=ax_table.transAxes,
                              facecolor=bg_col, edgecolor='none', zorder=0)
        ax_table.add_patch(bg_rect)
        for ci, (cell, xs) in enumerate(zip(row, col_starts)):
            ax_table.text(xs, y_frac + 0.013, cell, ha='left', va='center',
                          color=row_colours[ri][ci], fontsize=8,
                          transform=ax_table.transAxes)

    ax_table.text(0.5, 0.01,
                  'HF bands (blue) use full 5.6 m wire + base loading coil.  '
                  'VHF/UHF bands use traps to shorten electrically.',
                  ha='center', va='bottom', color=LGRAY, fontsize=7.5,
                  style='italic', transform=ax_table.transAxes)

    plt.savefig(os.path.join(SCRIPT_DIR, 'rollup_whip_assembly.png'),
                dpi=150, bbox_inches='tight', facecolor=BGND)
    plt.close(fig)
    print('  rollup_whip_assembly.png  saved.')


# ════════════════════════════════════════════════════════════════════════════
# Main
# ════════════════════════════════════════════════════════════════════════════

if __name__ == '__main__':
    print('Generating 5.6 m whip diagrams…')
    make_whip_overview()
    make_loading_coil_detail()
    make_rollup_assembly()
    print('Done — all three PNG files written to:')
    print(f'  {SCRIPT_DIR}/')
