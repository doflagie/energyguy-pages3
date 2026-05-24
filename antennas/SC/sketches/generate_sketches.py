#!/usr/bin/env python3
"""
Screwdriver Antenna Project — Sketch Generator
Generates three engineering sketch PNGs for the screwdriver antenna project.
"""

import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.patches import FancyArrowPatch, Rectangle, Circle, Arc
import matplotlib.patheffects as pe

# ── Color palette ─────────────────────────────────────────────────────────────
BLUE   = '#2196F3'
GREEN  = '#4CAF50'
RED    = '#F44336'
YELLOW = '#FFC107'
CYAN   = '#00BCD4'
ORANGE = '#FF9800'
WHITE  = '#FFFFFF'
DGRAY  = '#424242'

plt.style.use('dark_background')

OUT_DIR = '/home/merv/Public/claude/SC/sketches'

# ══════════════════════════════════════════════════════════════════════════════
# PNG 1 — screwdriver_overview.png
# ══════════════════════════════════════════════════════════════════════════════

def draw_overview():
    fig = plt.figure(figsize=(14, 8))
    fig.patch.set_facecolor('#1a1a1a')

    # Two panels: left 2/3, right 1/3
    ax_ant = fig.add_axes([0.03, 0.05, 0.60, 0.90])  # antenna diagram
    ax_bar = fig.add_axes([0.68, 0.08, 0.29, 0.84])  # bar chart

    ax_ant.set_facecolor('#1a1a1a')
    ax_ant.set_xlim(-120, 120)
    ax_ant.set_ylim(-30, 460)
    ax_ant.axis('off')
    ax_ant.set_title('Screwdriver Antenna — Overview', color=WHITE, fontsize=13, pad=8)

    # ── Scale mapping (mm → plot units, stretched for visibility) ─────────────
    # Real heights (mm): base=15, motor=50, coil=110, whip=2400
    # We'll compress whip for diagram, keep proportions clear.
    BASE_BOT  = 0
    BASE_TOP  = 18
    MOTOR_BOT = BASE_TOP
    MOTOR_TOP = MOTOR_BOT + 55
    COIL_BOT  = MOTOR_TOP
    COIL_TOP  = COIL_BOT + 120
    WHIP_BOT  = COIL_TOP
    WHIP_TOP  = WHIP_BOT + 260   # compressed whip

    CX = 0   # centre x

    # ── Base / ball mount ─────────────────────────────────────────────────────
    bw = 24
    base_rect = Rectangle((CX - bw/2, BASE_BOT), bw, BASE_TOP - BASE_BOT,
                           linewidth=1.5, edgecolor='#888888', facecolor='#555555')
    ax_ant.add_patch(base_rect)
    ax_ant.text(CX, (BASE_BOT + BASE_TOP)/2, 'BASE\nMOUNT', color=WHITE,
                fontsize=6, ha='center', va='center')

    # ── Motor housing ─────────────────────────────────────────────────────────
    mw = 50
    motor_rect = Rectangle((CX - mw/2, MOTOR_BOT), mw, MOTOR_TOP - MOTOR_BOT,
                            linewidth=1.5, edgecolor=YELLOW, facecolor='#2a2a2a')
    ax_ant.add_patch(motor_rect)
    ax_ant.text(CX, (MOTOR_BOT + MOTOR_TOP)/2, 'MOTOR\nHOUSING\n(28BYJ-48)', color=YELLOW,
                fontsize=6.5, ha='center', va='center')

    # ── Coil housing ──────────────────────────────────────────────────────────
    cw = 80
    coil_rect = Rectangle((CX - cw/2, COIL_BOT), cw, COIL_TOP - COIL_BOT,
                           linewidth=2, edgecolor=CYAN, facecolor='#1e2a2a')
    ax_ant.add_patch(coil_rect)

    # Coil windings — horizontal lines
    n_turns = 41
    for i in range(n_turns):
        y = COIL_BOT + 8 + i * (COIL_TOP - COIL_BOT - 16) / n_turns
        ax_ant.plot([CX - cw/2 + 4, CX + cw/2 - 4], [y, y],
                    color='#555566', linewidth=0.7)

    # Wiper position (at 20m band, ~6T from bottom)
    wiper_frac = 6.0 / 41.0
    wiper_y = COIL_BOT + 8 + wiper_frac * (COIL_TOP - COIL_BOT - 16)
    # Active (lower) portion of coil
    act_rect = Rectangle((CX - cw/2 + 4, COIL_BOT + 8), cw - 8,
                          wiper_y - COIL_BOT - 8,
                          linewidth=0, facecolor=CYAN, alpha=0.18)
    ax_ant.add_patch(act_rect)
    # Wiper dashed line
    ax_ant.plot([CX - cw/2, CX + cw/2], [wiper_y, wiper_y],
                color=ORANGE, linewidth=2, linestyle='--')
    ax_ant.text(CX + cw/2 + 4, wiper_y, '← WIPER', color=ORANGE, fontsize=6, va='center')

    # Band dots on coil right edge
    band_turns = {'80m': 41.0, '40m': 15.54, '20m': 6.01, '10m': 1.26}
    band_colors = {'80m': RED, '40m': ORANGE, '20m': GREEN, '10m': CYAN}
    for band, turns in band_turns.items():
        frac = turns / 41.0
        by = COIL_BOT + 8 + frac * (COIL_TOP - COIL_BOT - 16)
        ax_ant.plot(CX + cw/2, by, 'o', color=band_colors[band], markersize=5)
        ax_ant.text(CX + cw/2 + 4, by + 2, band, color=band_colors[band], fontsize=6)

    ax_ant.text(CX, (COIL_BOT + COIL_TOP)/2 + 20, '41T #14AWG\nQ ≈ 350', color=CYAN,
                fontsize=6.5, ha='center', va='center')

    # ── Whip ──────────────────────────────────────────────────────────────────
    # Taper from cw/6 at base to 0 at tip
    whip_base_w = 12
    for seg in range(40):
        frac0 = seg / 40
        frac1 = (seg + 1) / 40
        y0 = WHIP_BOT + frac0 * (WHIP_TOP - WHIP_BOT)
        y1 = WHIP_BOT + frac1 * (WHIP_TOP - WHIP_BOT)
        w0 = whip_base_w * (1 - frac0)
        w1 = whip_base_w * (1 - frac1)
        xs = [CX - w0, CX + w0, CX + w1, CX - w1, CX - w0]
        ys = [y0, y0, y1, y1, y0]
        ax_ant.fill(xs, ys, color='#888888', linewidth=0)
    # Whip outline
    ax_ant.plot([CX - whip_base_w, CX, CX + whip_base_w],
                [WHIP_BOT, WHIP_TOP, WHIP_BOT], color='#aaaaaa', linewidth=1)

    # ── 4 Radials ─────────────────────────────────────────────────────────────
    radial_len = 70
    for angle_deg in [210, 240, 300, 330]:
        angle = np.radians(angle_deg)
        rx = CX + radial_len * np.cos(angle)
        ry = BASE_BOT + radial_len * np.sin(angle) * 0.5  # flatten perspective
        ax_ant.plot([CX, rx], [BASE_BOT + BASE_TOP/2, ry],
                    color='#777777', linewidth=1.5)

    # ── Dimension callouts ────────────────────────────────────────────────────
    ann_x = -70
    # Whip dimension
    ax_ant.annotate('', xy=(ann_x, WHIP_TOP), xytext=(ann_x, WHIP_BOT),
                    arrowprops=dict(arrowstyle='<->', color=WHITE, lw=1))
    ax_ant.text(ann_x - 4, (WHIP_BOT + WHIP_TOP)/2, '2.4 m\nwhip',
                color=WHITE, fontsize=7, ha='right', va='center')

    # Coil dimension
    ax_ant.annotate('', xy=(ann_x - 18, COIL_TOP), xytext=(ann_x - 18, COIL_BOT),
                    arrowprops=dict(arrowstyle='<->', color=CYAN, lw=1))
    ax_ant.text(ann_x - 22, (COIL_BOT + COIL_TOP)/2, '75mm OD\ncoil',
                color=CYAN, fontsize=7, ha='right', va='center')

    # Motor label arrow
    ax_ant.annotate('Motor\n(5V stepper)', xy=(CX + mw/2, (MOTOR_BOT + MOTOR_TOP)/2),
                    xytext=(85, (MOTOR_BOT + MOTOR_TOP)/2),
                    color=YELLOW, fontsize=7,
                    arrowprops=dict(arrowstyle='->', color=YELLOW, lw=1))

    # Coil label arrow
    ax_ant.annotate('41T #14AWG\n75mm OD', xy=(CX + cw/2, (COIL_BOT + COIL_TOP)/2),
                    xytext=(85, (COIL_BOT + COIL_TOP)/2 + 20),
                    color=CYAN, fontsize=7,
                    arrowprops=dict(arrowstyle='->', color=CYAN, lw=1))

    ax_ant.text(CX, -20, '4 × radials (down-slope)', color='#777777',
                fontsize=7, ha='center')

    # ── Right panel: Efficiency bar chart ─────────────────────────────────────
    ax_bar.set_facecolor('#1a1a1a')

    bands   = ['160m', '80m', '60m', '40m', '30m', '20m', '17m', '15m', '12m', '10m']
    effvals = [0.13,   1.2,   3.8,   9.5,   25.2,  52.0,  73.7,  85.5,  93.3,  97.9]
    bar_colors = []
    for v in effvals:
        if v < 20:
            bar_colors.append(RED)
        elif v < 60:
            bar_colors.append(ORANGE)
        else:
            bar_colors.append(GREEN)

    y_pos = range(len(bands))
    bars = ax_bar.barh(list(y_pos), effvals, color=bar_colors, height=0.65,
                       edgecolor='#333333', linewidth=0.5)
    ax_bar.set_yticks(list(y_pos))
    ax_bar.set_yticklabels(bands, color=WHITE, fontsize=9)
    ax_bar.set_xlim(0, 105)
    ax_bar.set_xlabel('Efficiency (%)', color=WHITE, fontsize=9)
    ax_bar.tick_params(colors=WHITE, labelsize=8)
    ax_bar.spines['bottom'].set_color('#555555')
    ax_bar.spines['left'].set_color('#555555')
    ax_bar.spines['top'].set_visible(False)
    ax_bar.spines['right'].set_visible(False)
    ax_bar.set_title('Antenna Efficiency\n2.4m Whip', color=WHITE, fontsize=10, pad=6)

    for bar, val in zip(bars, effvals):
        ax_bar.text(val + 1, bar.get_y() + bar.get_height()/2,
                    f'{val}%', color=WHITE, fontsize=7, va='center')

    ax_bar.text(50, -1.5, 'Short whip = low Rrad\nEfficiency set by coil Q',
                color='#aaaaaa', fontsize=7, ha='center', va='top', style='italic')

    fig.savefig(f'{OUT_DIR}/screwdriver_overview.png', dpi=150, bbox_inches='tight',
                facecolor=fig.get_facecolor())
    plt.close(fig)
    print('  screwdriver_overview.png — OK')


# ══════════════════════════════════════════════════════════════════════════════
# PNG 2 — coil_mechanism_detail.png
# ══════════════════════════════════════════════════════════════════════════════

def draw_coil_detail():
    fig = plt.figure(figsize=(14, 10))
    fig.patch.set_facecolor('#1a1a1a')
    fig.suptitle('Coil Mechanism Detail', color=WHITE, fontsize=14, y=0.97)

    ax_A = fig.add_axes([0.03, 0.50, 0.44, 0.44])   # Panel A — cross-section
    ax_B = fig.add_axes([0.55, 0.50, 0.42, 0.44])   # Panel B — L vs position
    ax_C = fig.add_axes([0.03, 0.04, 0.44, 0.42])   # Panel C — motor schematic
    ax_D = fig.add_axes([0.55, 0.04, 0.42, 0.42])   # Panel D — stepper table

    for ax in [ax_A, ax_C, ax_D]:
        ax.set_facecolor('#1a1a1a')

    # ── Panel A: Cutaway cross-section ─────────────────────────────────────────
    ax_A.set_xlim(-60, 130)
    ax_A.set_ylim(-20, 130)
    ax_A.axis('off')
    ax_A.set_title('A — Coil Assembly (Cutaway)', color=WHITE, fontsize=10, pad=4)

    OD = 38   # outer radius in plot units
    CX = 0
    BOT = 5
    TOP = 110
    winding_h = TOP - BOT - 10

    # Outer cylinder walls
    ax_A.add_patch(Rectangle((CX - OD, BOT), 5, TOP - BOT, facecolor='#555555',
                              edgecolor='#888888', linewidth=1.5))
    ax_A.add_patch(Rectangle((CX + OD - 5, BOT), 5, TOP - BOT, facecolor='#555555',
                              edgecolor='#888888', linewidth=1.5))
    ax_A.text(CX - OD - 2, (BOT + TOP)/2, '75mm OD', color='#888888',
              fontsize=6.5, ha='right', va='center', rotation=90)

    # Wiper position (10T from bottom → 20m band)
    wiper_turns = 6.0
    wiper_frac  = wiper_turns / 41.0
    wiper_y = BOT + 5 + wiper_frac * winding_h

    # Active coil (CYAN, below wiper)
    for i in range(41):
        y_turn = BOT + 5 + i * winding_h / 41
        col = CYAN if y_turn <= wiper_y else DGRAY
        ax_A.plot([CX - OD + 5, CX + OD - 5], [y_turn, y_turn],
                  color=col, linewidth=1.2, solid_capstyle='round')

    # Threaded rod centre line (dashed)
    ax_A.plot([CX, CX], [BOT - 10, TOP + 10], color='#666666',
              linewidth=1.5, linestyle='--')
    for y in np.arange(BOT, TOP, 6):
        ax_A.plot([CX - 3, CX + 3], [y, y + 3], color='#555555', linewidth=0.8)
    ax_A.text(CX + 2, TOP + 5, '¼-20\nrod', color='#888888', fontsize=6, ha='center')

    # Wiper bracket
    wp_x1, wp_x2 = CX - OD + 5, CX + OD - 5
    ax_A.plot([wp_x1, wp_x2], [wiper_y, wiper_y], color=ORANGE, linewidth=3)
    ax_A.add_patch(Rectangle((CX - 5, wiper_y - 4), 10, 8,
                              facecolor=YELLOW, edgecolor=ORANGE, linewidth=1.5))
    ax_A.text(CX + OD + 5, wiper_y, '← WIPER (moves\n   for tuning)',
              color=ORANGE, fontsize=7, va='center')

    # Wiper nut on rod
    ax_A.add_patch(Rectangle((CX - 4, wiper_y - 3), 8, 6,
                              facecolor='#888800', edgecolor=YELLOW, linewidth=1))

    # Inductance direction arrow
    ax_A.annotate('Inductance\nincreases ↑', xy=(CX - OD - 15, wiper_y + 20),
                  xytext=(CX - OD - 15, wiper_y - 15),
                  color=GREEN, fontsize=6.5,
                  arrowprops=dict(arrowstyle='->', color=GREEN, lw=1.5))

    # Top connection to whip
    ax_A.plot([CX - 8, CX + 8], [TOP, TOP], color=WHITE, linewidth=2)
    ax_A.annotate('→ Whip base', xy=(CX + 8, TOP), xytext=(CX + OD + 5, TOP),
                  color=WHITE, fontsize=7, va='center',
                  arrowprops=dict(arrowstyle='->', color=WHITE, lw=1))

    # Bottom tap (4T)
    tap_y = BOT + 5 + (4.0/41.0) * winding_h
    ax_A.plot([CX + OD - 5, CX + OD + 20], [tap_y, tap_y],
              color=BLUE, linewidth=2)
    ax_A.text(CX + OD + 22, tap_y, '4T tap\n→ coax centre', color=BLUE,
              fontsize=6.5, va='center')

    # Bottom chassis connection
    ax_A.plot([CX - 8, CX + 8], [BOT, BOT], color='#888888', linewidth=2)
    ax_A.text(CX, BOT - 8, '→ coax shield / chassis GND',
              color='#888888', fontsize=6.5, ha='center')

    # ── Panel B: Inductance vs. turns ─────────────────────────────────────────
    ax_B.set_facecolor('#1a1a1a')
    N = np.linspace(0.5, 41, 300)
    L = 14.0625 * N**2 / (85.73 + 5.842 * N)
    ax_B.semilogy(N, L, color=CYAN, linewidth=2)

    band_points = {
        '10m': (1.26,  0.238),
        '12m': (2.18,  0.680),
        '15m': (3.10,  1.30),
        '17m': (4.16,  2.21),
        '20m': (6.01,  4.21),
        '30m': (9.57,  9.09),
        '40m': (15.54, 19.24),
        '60m': (23.75, 35.33),
        '80m': (41.0,  72.16),
    }
    dot_colors = [RED, RED, ORANGE, ORANGE, GREEN, GREEN, ORANGE, RED, RED]
    for (band, (n, l)), dc in zip(band_points.items(), dot_colors):
        ax_B.plot(n, l, 'o', color=dc, markersize=7, zorder=5)
        offset_x = 1.2 if n < 30 else -4
        ax_B.text(n + offset_x, l * 1.1, band, color=dc, fontsize=7)

    ax_B.set_xlim(0, 43)
    ax_B.set_ylim(0.1, 200)
    ax_B.set_xlabel('Active Turns (from bottom)', color=WHITE, fontsize=9)
    ax_B.set_ylabel('Inductance (µH)', color=WHITE, fontsize=9)
    ax_B.set_title('B — Coil Inductance vs. Wiper Position', color=WHITE, fontsize=10, pad=4)
    ax_B.tick_params(colors=WHITE, labelsize=8)
    for sp in ax_B.spines.values():
        sp.set_color('#555555')
    ax_B.grid(True, alpha=0.2, color='#444444')
    ax_B.yaxis.label.set_color(WHITE)
    ax_B.xaxis.label.set_color(WHITE)

    # ── Panel C: Motor control schematic ──────────────────────────────────────
    ax_C.set_xlim(0, 100)
    ax_C.set_ylim(0, 80)
    ax_C.axis('off')
    ax_C.set_title('C — Motor Control Schematic', color=WHITE, fontsize=10, pad=4)

    # 5V rail
    ax_C.plot([0, 100], [75, 75], color=RED, linewidth=2)
    ax_C.text(50, 77, '+5V', color=RED, fontsize=8, ha='center')
    # GND rail
    ax_C.plot([0, 100], [5, 5], color='#666666', linewidth=2)
    ax_C.text(50, 3, 'GND', color='#666666', fontsize=8, ha='center')

    # ESP32 box
    ax_C.add_patch(Rectangle((2, 35), 20, 30, facecolor='#003366',
                              edgecolor=BLUE, linewidth=1.5))
    ax_C.text(12, 56, 'ESP32', color=WHITE, fontsize=7.5, ha='center', va='top', weight='bold')
    for i, gpio in enumerate(['GPIO25', 'GPIO26', 'GPIO27', 'GPIO14']):
        y = 50 - i * 5
        ax_C.text(12, y, gpio, color=CYAN, fontsize=5.5, ha='center')
        ax_C.plot([22, 30], [y, y], color=CYAN, linewidth=1)

    # ULN2003 box
    ax_C.add_patch(Rectangle((30, 35), 20, 30, facecolor='#1a1a2e',
                              edgecolor=YELLOW, linewidth=1.5))
    ax_C.text(40, 62, 'ULN2003', color=YELLOW, fontsize=6.5, ha='center', va='top', weight='bold')
    for i, pin in enumerate(['IN1', 'IN2', 'IN3', 'IN4']):
        y = 53 - i * 5
        ax_C.text(32, y, pin, color='#cccc00', fontsize=5.5, ha='left')
    for i, pin in enumerate(['OUT1', 'OUT2', 'OUT3', 'OUT4']):
        y = 53 - i * 5
        ax_C.text(48, y, pin, color=ORANGE, fontsize=5.5, ha='right')
        ax_C.plot([50, 58], [y, y], color=ORANGE, linewidth=1)

    # Motor circle
    motor_cx, motor_cy = 72, 50
    ax_C.add_patch(Circle((motor_cx, motor_cy), 14, facecolor='#1a2a1a',
                           edgecolor=GREEN, linewidth=1.5))
    ax_C.text(motor_cx, motor_cy + 2, '28BYJ-48', color=GREEN,
              fontsize=6, ha='center', va='center', weight='bold')
    ax_C.text(motor_cx, motor_cy - 4, 'STEPPER', color=GREEN,
              fontsize=5.5, ha='center')
    for i, pin in enumerate(['IN1', 'IN2', 'IN3', 'IN4', '5V']):
        angle = np.radians(120 + i * 30)
        px = motor_cx + 14 * np.cos(angle)
        py = motor_cy + 14 * np.sin(angle)
        ax_C.plot([px, px - 5*np.cos(angle)], [py, py - 5*np.sin(angle)],
                  color=GREEN, linewidth=1)

    # Home switch
    ax_C.add_patch(Rectangle((2, 10), 18, 14, facecolor='#1a1a1a',
                              edgecolor='#888888', linewidth=1))
    ax_C.text(11, 19, 'HOME\nSWITCH', color='#aaaaaa', fontsize=6, ha='center', va='top')
    ax_C.text(11, 11, 'SPDT + 10k', color='#777777', fontsize=5.5, ha='center')
    ax_C.plot([20, 30], [17, 17], color='#888888', linewidth=1)
    ax_C.text(25, 18.5, '→ GPIO', color='#888888', fontsize=5.5, ha='center')

    # Power connections
    ax_C.plot([motor_cx, motor_cx], [64, 75], color=RED, linewidth=1.5)
    ax_C.plot([40, 40], [65, 75], color=RED, linewidth=1.5)
    ax_C.plot([12, 12], [35, 5], color='#666666', linewidth=1.5)
    ax_C.plot([40, 40], [35, 5], color='#666666', linewidth=1.5)
    ax_C.plot([motor_cx, motor_cx], [36, 5], color='#666666', linewidth=1.5)

    # ── Panel D: Stepper sequence table ───────────────────────────────────────
    ax_D.set_xlim(0, 100)
    ax_D.set_ylim(0, 100)
    ax_D.axis('off')
    ax_D.set_title('D — 28BYJ-48 Half-Step Sequence', color=WHITE, fontsize=10, pad=4)

    col_headers = ['Step', 'IN1', 'IN2', 'IN3', 'IN4']
    seq = [
        [1, 1, 1, 0, 0],
        [2, 0, 1, 0, 0],
        [3, 0, 1, 1, 0],
        [4, 0, 0, 1, 0],
        [5, 0, 0, 1, 1],
        [6, 0, 0, 0, 1],
        [7, 1, 0, 0, 1],
        [8, 1, 0, 0, 0],
    ]
    col_x = [10, 28, 46, 64, 82]
    row_h = 9
    header_y = 88

    # Header row
    for cx, hdr in zip(col_x, col_headers):
        ax_D.add_patch(Rectangle((cx - 8, header_y - 3), 17, 8,
                                  facecolor='#333333', edgecolor='#555555', linewidth=0.5))
        ax_D.text(cx, header_y + 1, hdr, color=YELLOW, fontsize=8,
                  ha='center', va='center', weight='bold')

    for row_i, row in enumerate(seq):
        y = header_y - row_h * (row_i + 1)
        for col_i, val in enumerate(row):
            cx = col_x[col_i]
            if col_i == 0:
                fc = '#2a2a2a'
                tc = WHITE
                txt = str(val)
            else:
                fc = '#0a3a0a' if val == 1 else '#2a2a2a'
                tc = GREEN   if val == 1 else DGRAY
                txt = str(val)
            ax_D.add_patch(Rectangle((cx - 8, y - 3), 17, 7.5,
                                      facecolor=fc, edgecolor='#333333', linewidth=0.5))
            ax_D.text(cx, y + 0.5, txt, color=tc, fontsize=9,
                      ha='center', va='center', weight='bold')

    ax_D.text(50, 3, '1 = coil energised   0 = off',
              color='#888888', fontsize=7.5, ha='center')

    fig.savefig(f'{OUT_DIR}/coil_mechanism_detail.png', dpi=150, bbox_inches='tight',
                facecolor=fig.get_facecolor())
    plt.close(fig)
    print('  coil_mechanism_detail.png — OK')


# ══════════════════════════════════════════════════════════════════════════════
# PNG 3 — rollup_assembly.png
# ══════════════════════════════════════════════════════════════════════════════

def draw_rollup():
    fig = plt.figure(figsize=(14, 10))
    fig.patch.set_facecolor('#1a1a1a')
    fig.suptitle('Roll-Up Screwdriver Antenna — Assembly & Kit', color=WHITE,
                 fontsize=14, y=0.97)

    ax_A = fig.add_axes([0.03, 0.04, 0.34, 0.90])   # Panel A — assembly diagram
    ax_B = fig.add_axes([0.42, 0.52, 0.55, 0.40])   # Panel B — band table
    ax_C = fig.add_axes([0.42, 0.04, 0.55, 0.44])   # Panel C — spool diagram

    for ax in [ax_A, ax_B, ax_C]:
        ax.set_facecolor('#1a1a1a')

    # ── Panel A: Assembly diagram ──────────────────────────────────────────────
    ax_A.set_xlim(-80, 80)
    ax_A.set_ylim(-30, 310)
    ax_A.axis('off')
    ax_A.set_title('A — Roll-Up Assembly', color=WHITE, fontsize=10, pad=4)

    CX = 0
    # Scale: 2.4m total wire → 240 plot units; 0 at base
    SCALE = 100   # plot units per metre
    TOTAL_H = 2.4 * SCALE   # 240

    # ── Stack of boxes at bottom ───────────────────────────────────────────────
    BOX_W = 36
    box_stack = [
        (0,   20, '#1a3a1a', GREEN,   'ESP32 + MOTOR\nCONTROL'),
        (22,  16, '#1a1a3a', BLUE,    'SWR BRIDGE'),
        (40,  16, '#1a1a3a', BLUE,    'L-NETWORK'),
        (58,  22, '#0a2a3a', CYAN,    'LOADING COIL\nMODULE'),
    ]
    for (y0, height, fc, ec, label) in box_stack:
        ax_A.add_patch(Rectangle((CX - BOX_W/2, y0), BOX_W, height,
                                  facecolor=fc, edgecolor=ec, linewidth=1.5))
        ax_A.text(CX, y0 + height/2, label, color=ec, fontsize=5.5,
                  ha='center', va='center')

    # Base of wire (top of loading coil box = 80)
    WIRE_BOT = 80
    wire_x = CX

    # 4 Radials from base
    for angle_deg in [210, 240, 300, 330]:
        angle = np.radians(angle_deg)
        rx = CX + 55 * np.cos(angle)
        ry = WIRE_BOT + 28 * np.sin(angle)
        ax_A.plot([CX, rx], [WIRE_BOT, ry], color='#777777', linewidth=1.5)

    # Wire segments and trap boxes
    trap_2m_h = (0.488 * SCALE) + WIRE_BOT   # plot y
    trap_6m_h = (1.40  * SCALE) + WIRE_BOT
    wire_top  = WIRE_BOT + TOTAL_H

    # Wire below 2m trap
    ax_A.plot([wire_x, wire_x], [WIRE_BOT, trap_2m_h - 8],
              color='#aaaaaa', linewidth=2.5)
    # 2m trap box
    tw = 22
    ax_A.add_patch(Rectangle((CX - tw/2, trap_2m_h - 8), tw, 16,
                              facecolor='#0a2a0a', edgecolor=GREEN, linewidth=2))
    ax_A.text(CX, trap_2m_h, '2m TRAP\n(146 MHz)', color=GREEN,
              fontsize=6, ha='center', va='center', weight='bold')
    ax_A.text(CX + tw/2 + 2, trap_2m_h,
              f'← {0.488:.3f}m', color='#888888', fontsize=6, va='center')

    # Wire 2m trap to 6m trap
    ax_A.plot([wire_x, wire_x], [trap_2m_h + 8, trap_6m_h - 8],
              color='#aaaaaa', linewidth=2.5)
    # 6m trap box
    ax_A.add_patch(Rectangle((CX - tw/2, trap_6m_h - 8), tw, 16,
                              facecolor='#0a2a0a', edgecolor=GREEN, linewidth=2))
    ax_A.text(CX, trap_6m_h, '6m TRAP\n(51 MHz)', color=GREEN,
              fontsize=6, ha='center', va='center', weight='bold')
    ax_A.text(CX + tw/2 + 2, trap_6m_h,
              f'← {1.40:.2f}m', color='#888888', fontsize=6, va='center')

    # Wire 6m trap to top
    ax_A.plot([wire_x, wire_x], [trap_6m_h + 8, wire_top],
              color='#aaaaaa', linewidth=2.5)

    # Dimension lines (right side)
    dim_x = 50
    # Total
    ax_A.annotate('', xy=(dim_x, wire_top), xytext=(dim_x, WIRE_BOT),
                  arrowprops=dict(arrowstyle='<->', color=WHITE, lw=1))
    ax_A.text(dim_x + 2, (wire_top + WIRE_BOT)/2, '2.40m\ntotal', color=WHITE,
              fontsize=6.5, va='center')

    # Active section labels
    ax_A.text(-48, (WIRE_BOT + trap_2m_h)/2, 'HF\nACTIVE', color=BLUE,
              fontsize=6, ha='center', va='center',
              bbox=dict(boxstyle='round', facecolor='#001a33', edgecolor=BLUE, linewidth=0.8))
    ax_A.text(-48, (trap_2m_h + trap_6m_h)/2, '6m\nSECTION', color=GREEN,
              fontsize=6, ha='center', va='center',
              bbox=dict(boxstyle='round', facecolor='#001a00', edgecolor=GREEN, linewidth=0.8))
    ax_A.text(-48, (trap_6m_h + wire_top)/2, '2m\nSECTION', color=CYAN,
              fontsize=6, ha='center', va='center',
              bbox=dict(boxstyle='round', facecolor='#001a1a', edgecolor=CYAN, linewidth=0.8))

    ax_A.text(CX, wire_top + 6, 'TIP', color='#aaaaaa', fontsize=7, ha='center')

    # ── Panel B: Band operation table ─────────────────────────────────────────
    ax_B.set_xlim(0, 100)
    ax_B.set_ylim(0, 100)
    ax_B.axis('off')
    ax_B.set_title('B — Band Operation Summary', color=WHITE, fontsize=10, pad=4)

    col_hdr  = ['Band', 'Wire Used', 'Loading', 'Wiper']
    col_x    = [8, 28, 58, 88]
    row_data = [
        ('160m', '2.4m full', '282µH ext coil', 'N/A'),
        ('80m',  '2.4m full', '72µH (max)',     '41T'),
        ('40m',  '2.4m full', '19.2µH',         '15.5T'),
        ('20m',  '2.4m full', '4.21µH',         '6.0T'),
        ('10m',  '2.4m full', '0.24µH',         '1.3T'),
        ('6m',   '1.40m',     'no coil',         'trap 1.40m'),
        ('2m',   '0.49m',     'no coil',         'trap 0.49m'),
    ]
    row_colors = [ORANGE, RED, ORANGE, GREEN, CYAN, GREEN, CYAN]
    row_h = 10
    header_y = 90

    for cx, hdr in zip(col_x, col_hdr):
        ax_B.add_patch(Rectangle((cx - 8, header_y - 4), 38, 9,
                                  facecolor='#333333', edgecolor='#555555', linewidth=0.5))
        ax_B.text(cx, header_y, hdr, color=YELLOW, fontsize=7.5,
                  ha='center', va='center', weight='bold')

    for ri, (row, rc) in enumerate(zip(row_data, row_colors)):
        y = header_y - row_h * (ri + 1)
        fc_row = '#1e1e1e' if ri % 2 == 0 else '#252525'
        ax_B.add_patch(Rectangle((0, y - 4), 100, 9,
                                  facecolor=fc_row, edgecolor='#333333', linewidth=0.3))
        for cx, cell in zip(col_x, row):
            ax_B.text(cx, y + 0.5, cell, color=rc, fontsize=7,
                      ha='center', va='center')

    # ── Panel C: Wire spool diagram ────────────────────────────────────────────
    ax_C.set_xlim(-120, 120)
    ax_C.set_ylim(-70, 90)
    ax_C.axis('off')
    ax_C.set_title('C — Roll-Up Screwdriver Kit (Spool)', color=WHITE, fontsize=10, pad=4)

    SCX, SCY = 0, 0
    SR = 60    # spool outer radius
    HR = 18    # hub radius
    FLANGE_T = 8

    # Main spool body (side view) — drawn as rectangle representing the hub
    ax_C.add_patch(Rectangle((SCX - HR, SCY - SR), 2*HR, 2*SR,
                              facecolor='#2a1a0a', edgecolor='#886644', linewidth=2))

    # Left flange
    ax_C.add_patch(Rectangle((SCX - HR - FLANGE_T, SCY - SR), FLANGE_T, 2*SR,
                              facecolor='#3a2a1a', edgecolor='#aa8855', linewidth=2))
    # Right flange
    ax_C.add_patch(Rectangle((SCX + HR, SCY - SR), FLANGE_T, 2*SR,
                              facecolor='#3a2a1a', edgecolor='#aa8855', linewidth=2))

    # Wire wound on hub (suggest with curved lines)
    for r in [HR + 4, HR + 9, HR + 14, HR + 19]:
        for side in [-1, 1]:
            arc_a = Arc((SCX, SCY), 2*r, 2*r, angle=0,
                        theta1=10, theta2=170 if side == 1 else -170,
                        color='#777755', linewidth=0.8)
            ax_C.add_patch(arc_a)

    # Trap housing clips on RIGHT flange
    for i, (label, color) in enumerate([('2m TRAP', GREEN), ('6m TRAP', GREEN)]):
        clip_y = SR - 15 - i * 22
        ax_C.add_patch(Rectangle((SCX + HR + FLANGE_T + 2, SCY + clip_y - 7), 28, 14,
                                  facecolor='#0a2a0a', edgecolor=color, linewidth=1.5))
        ax_C.text(SCX + HR + FLANGE_T + 16, SCY + clip_y, label,
                  color=color, fontsize=5.5, ha='center', va='center', weight='bold')
        # Clip hook
        ax_C.plot([SCX + HR + FLANGE_T, SCX + HR + FLANGE_T + 2],
                  [SCY + clip_y, SCY + clip_y], color=color, linewidth=2)

    # Loading coil slots on LEFT flange
    bands_L = ['160m', '80m', '60m', '40m', '30m']
    for i, (band, bc) in enumerate(zip(bands_L, [RED, RED, ORANGE, ORANGE, GREEN])):
        slot_y = SR - 10 - i * 22
        ax_C.add_patch(Rectangle((SCX - HR - FLANGE_T - 32, SCY + slot_y - 7), 30, 14,
                                  facecolor='#001a2a', edgecolor=bc, linewidth=1.5))
        ax_C.text(SCX - HR - FLANGE_T - 17, SCY + slot_y, band,
                  color=bc, fontsize=6, ha='center', va='center', weight='bold')
        # Slot connector
        ax_C.plot([SCX - HR - FLANGE_T, SCX - HR - FLANGE_T - 2],
                  [SCY + slot_y, SCY + slot_y], color=bc, linewidth=1.5)

    # Hanging hole in handle
    ax_C.add_patch(Circle((SCX, SCY + SR + 15), 6,
                           facecolor='#1a1a1a', edgecolor='#888888', linewidth=2))
    ax_C.text(SCX, SCY + SR + 15, '⊙', color='#888888', fontsize=8, ha='center', va='center')

    # Labels
    ax_C.text(SCX + HR + FLANGE_T + 35, SCY - SR + 5, 'TRAP\nCLIPS',
              color=GREEN, fontsize=7, ha='left', va='bottom')
    ax_C.text(SCX - HR - FLANGE_T - 35, SCY - SR + 5, 'COIL\nSLOTS',
              color=CYAN, fontsize=7, ha='right', va='bottom')
    ax_C.text(SCX, SCY + SR + 28, '⬆ HANGING HOLE',
              color='#888888', fontsize=7, ha='center')
    ax_C.text(SCX, SCY - SR - 15, '160mm diameter  ·  ROLL-UP SCREWDRIVER KIT',
              color=WHITE, fontsize=8.5, ha='center', weight='bold')

    fig.savefig(f'{OUT_DIR}/rollup_assembly.png', dpi=150, bbox_inches='tight',
                facecolor=fig.get_facecolor())
    plt.close(fig)
    print('  rollup_assembly.png — OK')


# ══════════════════════════════════════════════════════════════════════════════
if __name__ == '__main__':
    print('Generating screwdriver antenna sketches...')
    draw_overview()
    draw_coil_detail()
    draw_rollup()
    print('Done — 3 PNG files written to', OUT_DIR)
