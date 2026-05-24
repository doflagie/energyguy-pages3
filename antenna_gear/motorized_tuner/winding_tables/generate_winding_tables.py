# Requires: odfpy (pip install odfpy)
# Usage: python3 generate_winding_tables.py [--output-dir DIR] [--verbose]

import argparse
import csv
import math
import os
import sys

from odf.opendocument import OpenDocumentSpreadsheet
from odf.style import Style, TableColumnProperties, TableRowProperties, TextProperties, ParagraphProperties
from odf.number import NumberStyle, Number
from odf import table as odftable
from odf.table import Table, TableColumn, TableRow, TableCell
from odf.text import P

# ---------------------------------------------------------------------------
# Wire data: AWG -> (bare_mm, ins_mm, ohm_per_m)
# ---------------------------------------------------------------------------
WIRE_DATA = {
    10: (2.588, 2.9,   0.00328),
    12: (2.053, 2.35,  0.00521),
    14: (1.628, 1.9,   0.00828),
    16: (1.291, 1.55,  0.01313),
    18: (1.024, 1.25,  0.02087),
    20: (0.812, 1.0,   0.03313),
    22: (0.644, 0.8,   0.05261),
    24: (0.511, 0.65,  0.08408),
    26: (0.405, 0.52,  0.1328),
    28: (0.321, 0.42,  0.2120),
    30: (0.255, 0.34,  0.3386),
}

# ---------------------------------------------------------------------------
# Former diameters in mm
# ---------------------------------------------------------------------------
FORMERS_MM = [25, 38, 51, 76, 102]

# ---------------------------------------------------------------------------
# Band table: name -> (freq_min_MHz, freq_max_MHz, L_min_uH, L_max_uH)
# ---------------------------------------------------------------------------
BANDS = [
    ("160m", 1.8,    2.0,    10.0,  80.0),
    ("80m",  3.5,    4.0,    5.0,   40.0),
    ("60m",  5.3,    5.4,    3.0,   25.0),
    ("40m",  7.0,    7.3,    2.0,   15.0),
    ("30m",  10.1,   10.15,  1.0,   10.0),
    ("20m",  14.0,   14.35,  0.5,   7.0),
    ("17m",  18.068, 18.168, 0.3,   5.0),
    ("15m",  21.0,   21.45,  0.2,   4.0),
    ("12m",  24.89,  24.99,  0.2,   3.0),
    ("10m",  28.0,   29.7,   0.1,   2.5),
    ("6m",   50.0,   54.0,   0.05,  1.0),
]

# ---------------------------------------------------------------------------
# AWG recommendation per former size
# ---------------------------------------------------------------------------
def recommended_awg(former_mm):
    if former_mm <= 25:
        return 24
    elif former_mm <= 38:
        return 22
    elif former_mm <= 51:
        return 22
    elif former_mm <= 76:
        return 20
    else:
        return 18


# ---------------------------------------------------------------------------
# Wheeler single-layer formula
# L_uH = (d_in**2 * N**2) / (18*d_in + 40*l_in)
# ---------------------------------------------------------------------------
def wheeler_L(d_in, N, l_in):
    return (d_in**2 * N**2) / (18.0 * d_in + 40.0 * l_in)


def wheeler_dL_dN(d_in, N, l_in):
    """Derivative of L with respect to N."""
    return (2.0 * d_in**2 * N) / (18.0 * d_in + 40.0 * l_in)


# ---------------------------------------------------------------------------
# Newton-Raphson inversion: given target L_uH, d_in, l_in -> N (float)
# ---------------------------------------------------------------------------
def solve_turns_nr(target_L, d_in, l_in, n_iter=15, tol=0.001):
    # Initial guess from rearranged formula (approximate, treating l_in fixed)
    denom = 18.0 * d_in + 40.0 * l_in
    if denom <= 0 or d_in <= 0:
        return 1.0
    N = math.sqrt(target_L * denom / (d_in**2))
    if N < 1.0:
        N = 1.0
    for _ in range(n_iter):
        L_calc = wheeler_L(d_in, N, l_in)
        dL = wheeler_dL_dN(d_in, N, l_in)
        if abs(dL) < 1e-12:
            break
        delta = (L_calc - target_L) / dL
        N -= delta
        if N < 1.0:
            N = 1.0
        if abs(delta) < tol:
            break
    return N


# ---------------------------------------------------------------------------
# Full calculation for one combination
# Two-pass: first pass uses l_in = d_in, second uses actual wound length
# ---------------------------------------------------------------------------
def calculate_row(band_name, f_min, f_max, target_L, former_mm, awg):
    bare_mm, ins_mm, ohm_per_m = WIRE_DATA[awg]
    d_in = former_mm / 25.4
    ins_in = ins_mm / 25.4
    f_center = (f_min + f_max) / 2.0

    # Pass 1: assume l_in = d_in (square coil)
    l_in_est = d_in
    N_float = solve_turns_nr(target_L, d_in, l_in_est)
    N = max(1, round(N_float))

    # Pass 2: compute actual wound length, re-solve
    wound_mm = N * ins_mm
    l_in_actual = wound_mm / 25.4
    N_float2 = solve_turns_nr(target_L, d_in, l_in_actual)
    N = max(1, round(N_float2))

    # Recompute wound length with final N
    wound_mm = N * ins_mm
    l_in_final = wound_mm / 25.4

    # Wire length = circumference * turns
    wire_length_m = (N * math.pi * former_mm) / 1000.0
    dc_resistance = wire_length_m * ohm_per_m

    # Skin-effect adjusted AC resistance
    r_ac = dc_resistance * math.sqrt(f_center)

    # Reactance at center frequency
    x_l = 2.0 * math.pi * f_center * target_L  # XL = 2π·f_MHz·L_uH (units work out)

    # Q factor
    q_factor = x_l / r_ac if r_ac > 0 else 0.0

    # Stray capacitance: 0.7 pF per cm of winding length
    c_stray_pf = 0.7 * (wound_mm / 10.0)

    # Self-resonant frequency
    if target_L > 0 and c_stray_pf > 0:
        f_srf = 30.0 / math.sqrt(target_L * c_stray_pf)
    else:
        f_srf = 9999.0

    srf_warning = "WARN" if f_srf < 2.0 * f_center else ""

    return {
        "band": band_name,
        "f_center": f_center,
        "former_mm": former_mm,
        "former_in": round(former_mm / 25.4, 3),
        "target_L": target_L,
        "awg": awg,
        "turns": N,
        "wound_mm": round(wound_mm, 1),
        "dc_resistance": round(dc_resistance, 4),
        "q_factor": round(q_factor, 1),
        "f_srf": round(f_srf, 1),
        "srf_warning": srf_warning,
        "l_in_final": l_in_final,
        "d_in": d_in,
    }


# ---------------------------------------------------------------------------
# Generate all rows
# ---------------------------------------------------------------------------
def generate_all_rows(verbose=False):
    rows = []
    for band_name, f_min, f_max, l_min, l_max in BANDS:
        for former_mm in FORMERS_MM:
            awg = recommended_awg(former_mm)
            for target_L in [l_min, l_max]:
                if verbose:
                    print(f"  {band_name:5s}  former={former_mm:3d}mm  L={target_L:6.2f}uH  AWG={awg}")
                row = calculate_row(band_name, f_min, f_max, target_L, former_mm, awg)
                rows.append(row)
    return rows


# ---------------------------------------------------------------------------
# ODS colour helpers
# ---------------------------------------------------------------------------
BLUE_BG    = "#BDD7EE"
ORANGE_BG  = "#F4B942"
LGRAY_BG   = "#F2F2F2"
WHITE_BG   = "#FFFFFF"
HEADER_FG  = "#000000"


def make_styles(doc):
    """Create and register all needed styles. Return a dict of style names."""

    def add_style(name, family, props):
        s = Style(name=name, family=family)
        for p in props:
            s.addElement(p)
        doc.automaticstyles.addElement(s)
        return name

    styles = {}

    # Table column widths
    for width, suffix in [("2.5cm", "narrow"), ("3.5cm", "medium"), ("4.5cm", "wide")]:
        sname = f"co_{suffix}"
        s = Style(name=sname, family="table-column")
        s.addElement(TableColumnProperties(columnwidth=width, breakbefore="auto"))
        doc.automaticstyles.addElement(s)
        styles[sname] = sname

    # Row heights
    s = Style(name="ro_normal", family="table-row")
    s.addElement(TableRowProperties(rowheight="0.6cm", breakbefore="auto"))
    doc.automaticstyles.addElement(s)
    styles["ro_normal"] = "ro_normal"

    # Cell styles
    cell_defs = {
        "ce_header":     (BLUE_BG,   True,  "0.05pt solid #000000"),
        "ce_warn":       (ORANGE_BG, False, "0.05pt solid #000000"),
        "ce_normal":     (WHITE_BG,  False, "0.05pt solid #000000"),
        "ce_alt":        (LGRAY_BG,  False, "0.05pt solid #000000"),
        "ce_title":      (BLUE_BG,   True,  "0.05pt solid #000000"),
    }
    for cname, (bg, bold, border) in cell_defs.items():
        s = Style(name=cname, family="table-cell")
        from odf.style import TableCellProperties
        tcp = TableCellProperties(backgroundcolor=bg, border=border, wrapoption="wrap", verticalalign="middle")
        s.addElement(tcp)
        tp = TextProperties(fontweight="bold" if bold else "normal", color=HEADER_FG)
        s.addElement(tp)
        pp = ParagraphProperties(textalign="center")
        s.addElement(pp)
        doc.automaticstyles.addElement(s)
        styles[cname] = cname

    return styles


def make_cell(text, style_name):
    tc = TableCell(stylename=style_name, valuetype="string")
    tc.addElement(P(text=str(text)))
    return tc


def make_number_cell(value, style_name):
    tc = TableCell(stylename=style_name, valuetype="float",
                   attributes={"office:value": str(value)})
    tc.addElement(P(text=str(value)))
    return tc


# ---------------------------------------------------------------------------
# Build Summary sheet
# ---------------------------------------------------------------------------
def build_summary_sheet(doc, styles, all_rows):
    sheet = Table(name="Summary")

    # Use medium column width for all
    for _ in range(len(FORMERS_MM) + 1):
        col = TableColumn(stylename="co_medium")
        sheet.addElement(col)

    # Title row
    tr = TableRow(stylename="ro_normal")
    tc = TableCell(stylename="ce_title", valuetype="string",
                   attributes={"table:number-columns-spanned": str(len(FORMERS_MM) + 1)})
    tc.addElement(P(text="Motorized Antenna Tuner — Winding Summary (Recommended Turns for Center Inductance)"))
    tr.addElement(tc)
    for _ in range(len(FORMERS_MM)):
        tr.addElement(TableCell(stylename="ce_title", valuetype="string",
                                attributes={"table:number-columns-repeated": "1"}))
    sheet.addElement(tr)

    # Header row
    tr = TableRow(stylename="ro_normal")
    tr.addElement(make_cell("Band", "ce_header"))
    for fmm in FORMERS_MM:
        tr.addElement(make_cell(f"Former {fmm}mm", "ce_header"))
    sheet.addElement(tr)

    # One row per band, showing turns for center L at each former
    band_rows = {}
    for row in all_rows:
        key = (row["band"], row["former_mm"])
        if key not in band_rows:
            band_rows[key] = []
        band_rows[key].append(row)

    for idx, (band_name, f_min, f_max, l_min, l_max) in enumerate(BANDS):
        l_center = (l_min + l_max) / 2.0
        tr = TableRow(stylename="ro_normal")
        cstyle = "ce_alt" if idx % 2 == 1 else "ce_normal"
        tr.addElement(make_cell(band_name, cstyle))
        for fmm in FORMERS_MM:
            awg = recommended_awg(fmm)
            r = calculate_row(band_name, f_min, f_max, l_center, fmm, awg)
            warn = r["srf_warning"]
            cs = "ce_warn" if warn else cstyle
            tr.addElement(make_cell(f"{r['turns']} (AWG{awg})", cs))
        sheet.addElement(tr)

    doc.spreadsheet.addElement(sheet)


# ---------------------------------------------------------------------------
# Build per-band sheet
# ---------------------------------------------------------------------------
BAND_COLS = [
    "Former_mm", "Former_in", "Target_L_uH", "AWG", "Turns",
    "Wound_length_mm", "DC_resistance_ohm", "Q_factor", "f_SRF_MHz", "SRF_Warning"
]


def build_band_sheet(doc, styles, band_name, band_rows):
    sheet = Table(name=band_name)

    col_widths = ["co_narrow", "co_narrow", "co_medium", "co_narrow", "co_narrow",
                  "co_medium", "co_medium", "co_medium", "co_medium", "co_medium"]
    for w in col_widths:
        sheet.addElement(TableColumn(stylename=w))

    # Header
    tr = TableRow(stylename="ro_normal")
    for col in BAND_COLS:
        tr.addElement(make_cell(col, "ce_header"))
    sheet.addElement(tr)

    for idx, row in enumerate(band_rows):
        cstyle = "ce_alt" if idx % 2 == 1 else "ce_normal"
        warn = row["srf_warning"]
        w_style = "ce_warn" if warn else cstyle

        tr = TableRow(stylename="ro_normal")
        tr.addElement(make_cell(row["former_mm"], cstyle))
        tr.addElement(make_cell(row["former_in"], cstyle))
        tr.addElement(make_cell(row["target_L"], cstyle))
        tr.addElement(make_cell(row["awg"], cstyle))
        tr.addElement(make_cell(row["turns"], cstyle))
        tr.addElement(make_cell(row["wound_mm"], cstyle))
        tr.addElement(make_cell(row["dc_resistance"], cstyle))
        tr.addElement(make_cell(row["q_factor"], cstyle))
        tr.addElement(make_cell(row["f_srf"], w_style))
        tr.addElement(make_cell(row["srf_warning"], w_style))
        sheet.addElement(tr)

    doc.spreadsheet.addElement(sheet)


# ---------------------------------------------------------------------------
# Wire Data sheet
# ---------------------------------------------------------------------------
def build_wire_sheet(doc, styles):
    sheet = Table(name="Wire_Data")
    for _ in range(5):
        sheet.addElement(TableColumn(stylename="co_medium"))

    headers = ["AWG", "Bare_dia_mm", "Ins_dia_mm", "Ohm_per_m", "Typical_current_A"]
    current_ratings = {
        10: 30, 12: 20, 14: 15, 16: 13, 18: 10,
        20: 7,  22: 5,  24: 3.5, 26: 2.2, 28: 1.4, 30: 0.9
    }
    tr = TableRow(stylename="ro_normal")
    for h in headers:
        tr.addElement(make_cell(h, "ce_header"))
    sheet.addElement(tr)

    for idx, (awg, (bare, ins, opm)) in enumerate(sorted(WIRE_DATA.items())):
        cstyle = "ce_alt" if idx % 2 == 1 else "ce_normal"
        tr = TableRow(stylename="ro_normal")
        tr.addElement(make_cell(awg, cstyle))
        tr.addElement(make_cell(bare, cstyle))
        tr.addElement(make_cell(ins, cstyle))
        tr.addElement(make_cell(opm, cstyle))
        tr.addElement(make_cell(current_ratings.get(awg, "—"), cstyle))
        sheet.addElement(tr)

    doc.spreadsheet.addElement(sheet)


# ---------------------------------------------------------------------------
# Instructions sheet
# ---------------------------------------------------------------------------
def build_instructions_sheet(doc, styles):
    sheet = Table(name="Instructions")
    sheet.addElement(TableColumn(stylename="co_wide"))

    lines = [
        ("ce_title",  "Motorized Antenna Tuner — Winding Table Instructions"),
        ("ce_header", "Wheeler Single-Layer Formula"),
        ("ce_normal", "L (µH) = d² × N² / (18×d + 40×l)   where d = coil diameter (inches), l = winding length (inches)"),
        ("ce_normal", "Accuracy: ±1% for l/d between 0.4 and 4.0. Outside this range error increases."),
        ("ce_normal", ""),
        ("ce_header", "How to Use These Tables"),
        ("ce_normal", "1. Select your target band and required inductance range from the band sheet."),
        ("ce_normal", "2. Choose a former diameter based on available materials and current/power requirements."),
        ("ce_normal", "3. Use the recommended AWG or substitute — adjust turns using the Wheeler formula if needed."),
        ("ce_normal", "4. Wind tightly, close-spaced. Any spacing increases inductance slightly."),
        ("ce_normal", "5. Verify with an inductance meter or VNA after winding."),
        ("ce_normal", ""),
        ("ce_header", "Q Factor Notes"),
        ("ce_normal", "Q ≈ XL / R_ac, where R_ac = R_dc × √f (skin-effect approximation)."),
        ("ce_normal", "Higher Q = lower loss. For efficient tuners, target Q > 100 at operating frequency."),
        ("ce_normal", "Larger wire (lower AWG number) and larger formers improve Q."),
        ("ce_normal", "Silver-plated wire can improve Q at VHF; not needed for HF."),
        ("ce_normal", ""),
        ("ce_header", "Self-Resonant Frequency (SRF) Warning"),
        ("ce_normal", "Coils have distributed stray capacitance, causing self-resonance above which they behave capacitively."),
        ("ce_normal", "Rule: f_SRF must be at least 2× the operating frequency for reliable inductive behavior."),
        ("ce_normal", "Stray capacitance estimate: C_stray ≈ 0.7 pF per cm of winding length."),
        ("ce_normal", "f_SRF (MHz) ≈ 30 / √(L_µH × C_stray_pF)"),
        ("ce_normal", "Cells marked WARN indicate f_SRF < 2 × band center frequency."),
        ("ce_normal", "To fix: use fewer turns on a larger former, or use a different inductance strategy."),
        ("ce_normal", ""),
        ("ce_header", "Motorized Tuner Notes"),
        ("ce_normal", "This table supports the motorized roller inductor design."),
        ("ce_normal", "The roller contact shorts out turns from the bottom; full turns = maximum L."),
        ("ce_normal", "Winding uniformity is critical — irregular pitch causes non-linear tuning."),
        ("ce_normal", "Secure wire ends mechanically before soldering; tension causes creep over time."),
        ("ce_normal", ""),
        ("ce_header", "Formula References"),
        ("ce_normal", "Wheeler, H.A., 'Simple Inductance Formulas for Radio Coils,' Proc. IRE, 1928."),
        ("ce_normal", "AWG resistance data: from standard wire tables (20°C, annealed copper)."),
        ("ce_normal", "Skin effect approximation valid within ±20% for HF frequencies."),
    ]

    for style, text in lines:
        tr = TableRow(stylename="ro_normal")
        tc = TableCell(stylename=style, valuetype="string")
        tc.addElement(P(text=text))
        tr.addElement(tc)
        sheet.addElement(tr)

    doc.spreadsheet.addElement(sheet)


# ---------------------------------------------------------------------------
# Write CSV
# ---------------------------------------------------------------------------
CSV_COLS = [
    "Band", "Freq_MHz_center", "Former_mm", "Former_in", "Target_L_uH",
    "AWG", "Turns", "Wound_length_mm", "DC_resistance_ohm",
    "Q_factor", "f_SRF_MHz", "SRF_Warning"
]


def write_csv(all_rows, path):
    with open(path, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=CSV_COLS)
        writer.writeheader()
        for row in all_rows:
            writer.writerow({
                "Band":             row["band"],
                "Freq_MHz_center":  round(row["f_center"], 4),
                "Former_mm":        row["former_mm"],
                "Former_in":        row["former_in"],
                "Target_L_uH":      row["target_L"],
                "AWG":              row["awg"],
                "Turns":            row["turns"],
                "Wound_length_mm":  row["wound_mm"],
                "DC_resistance_ohm": row["dc_resistance"],
                "Q_factor":         row["q_factor"],
                "f_SRF_MHz":        row["f_srf"],
                "SRF_Warning":      row["srf_warning"],
            })


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
def main():
    parser = argparse.ArgumentParser(
        description="Generate HF inductor winding tables as ODS spreadsheet and CSV."
    )
    parser.add_argument("--output-dir", default=None,
                        help="Directory for output files (default: same as script)")
    parser.add_argument("--verbose", action="store_true",
                        help="Print progress messages")
    args = parser.parse_args()

    if args.output_dir:
        out_dir = args.output_dir
        os.makedirs(out_dir, exist_ok=True)
    else:
        out_dir = os.path.dirname(os.path.abspath(__file__))

    ods_path = os.path.join(out_dir, "winding_tables.ods")
    csv_path = os.path.join(out_dir, "winding_tables.csv")

    if args.verbose:
        print("Generating winding table data...")

    all_rows = generate_all_rows(verbose=args.verbose)

    if args.verbose:
        print(f"  Total rows: {len(all_rows)}")
        warn_count = sum(1 for r in all_rows if r["srf_warning"])
        print(f"  SRF warnings: {warn_count}")

    # --- ODS ---
    if args.verbose:
        print("Building ODS spreadsheet...")

    doc = OpenDocumentSpreadsheet()
    styles = make_styles(doc)

    if args.verbose:
        print("  Building Summary sheet...")
    build_summary_sheet(doc, styles, all_rows)

    for band_name, f_min, f_max, l_min, l_max in BANDS:
        if args.verbose:
            print(f"  Building {band_name} sheet...")
        band_rows = [r for r in all_rows if r["band"] == band_name]
        build_band_sheet(doc, styles, band_name, band_rows)

    if args.verbose:
        print("  Building Wire_Data sheet...")
    build_wire_sheet(doc, styles)

    if args.verbose:
        print("  Building Instructions sheet...")
    build_instructions_sheet(doc, styles)

    doc.save(ods_path)
    if args.verbose:
        print(f"  Saved: {ods_path}")

    # --- CSV ---
    if args.verbose:
        print("Writing CSV backup...")
    write_csv(all_rows, csv_path)
    if args.verbose:
        print(f"  Saved: {csv_path}")

    print(f"Generated: winding_tables.ods and winding_tables.csv")


if __name__ == "__main__":
    main()
