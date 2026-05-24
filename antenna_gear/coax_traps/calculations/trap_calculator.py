#!/usr/bin/env python3
"""
Coaxial Cable Trap Calculator
Generates winding tables for parallel and series resonant coax traps.
Outputs CSV and ODS (LibreOffice Calc) winding tables.

Physics:
  - Coil inductance: Wheeler's formula (short single-layer solenoid)
  - Trap capacitance: distributed capacitance of coax cable wound on former
  - Resonance: f = 1 / (2*pi*sqrt(L*C))
  - Q factor: estimated from wire gauge losses and dielectric losses
"""

import math
import csv
import sys

# ---------------------------------------------------------------------------
# Coax cable parameters
# ---------------------------------------------------------------------------
COAX = {
    "RG-58":  {"od_mm": 4.95,  "c_pf_m": 93.5,  "vf": 0.66, "z0": 50, "name": "RG-58/U"},
    "RG-8X":  {"od_mm": 6.15,  "c_pf_m": 78.7,  "vf": 0.78, "z0": 50, "name": "RG-8X"},
    "RG-213": {"od_mm": 10.29, "c_pf_m": 96.8,  "vf": 0.66, "z0": 50, "name": "RG-213/U"},
}

# ---------------------------------------------------------------------------
# Ham band center frequencies (MHz) and designations
# ---------------------------------------------------------------------------
BANDS = [
    ("160M",  1.850,  "1.800-2.000"),
    ("80M",   3.750,  "3.500-4.000"),
    ("40M",   7.150,  "7.000-7.300"),
    ("30M",  10.125,  "10.100-10.150"),
    ("20M",  14.175,  "14.000-14.350"),
    ("17M",  18.118,  "18.068-18.168"),
    ("15M",  21.225,  "21.000-21.450"),
    ("12M",  24.940,  "24.890-24.990"),
    ("10M",  28.500,  "28.000-29.700"),
    ("6M",   51.000,  "50.000-54.000"),
    ("2M",  146.000,  "144.000-148.000"),
    ("1.25M",222.000, "222.000-225.000"),
    ("70cm", 440.000, "420.000-450.000"),
    ("33cm", 902.000, "902.000-928.000"),
    ("23cm",1296.000, "1240.000-1300.000"),
]

# Practical former diameters (mm) indexed by band name
# Chosen for buildability (max ~150mm for largest, fits 200mm print bed)
FORMER_DIAMETERS = {
    "160M":  {"RG-213": 120.0, "RG-8X": 100.0, "RG-58": 80.0},
    "80M":   {"RG-213":  90.0, "RG-8X":  76.0, "RG-58": 60.0},
    "40M":   {"RG-213":  76.0, "RG-8X":  60.0, "RG-58": 50.0},
    "30M":   {"RG-213":  60.0, "RG-8X":  50.0, "RG-58": 40.0},
    "20M":   {"RG-213":  50.0, "RG-8X":  40.0, "RG-58": 35.0},
    "17M":   {"RG-213":  45.0, "RG-8X":  35.0, "RG-58": 30.0},
    "15M":   {"RG-213":  40.0, "RG-8X":  32.0, "RG-58": 25.0},
    "12M":   {"RG-213":  35.0, "RG-8X":  28.0, "RG-58": 22.0},
    "10M":   {"RG-213":  30.0, "RG-8X":  25.0, "RG-58": 20.0},
    "6M":    {"RG-213":  25.0, "RG-8X":  20.0, "RG-58": 16.0},
    "2M":    {"RG-213":  15.0, "RG-8X":  12.0, "RG-58": 10.0},
    "1.25M": {"RG-213":  12.0, "RG-8X":  10.0, "RG-58":  8.0},
    "70cm":  {"RG-213":   0.0, "RG-8X":   0.0, "RG-58":  0.0},  # stubs only
    "33cm":  {"RG-213":   0.0, "RG-8X":   0.0, "RG-58":  0.0},
    "23cm":  {"RG-213":   0.0, "RG-8X":   0.0, "RG-58":  0.0},
}

# ---------------------------------------------------------------------------
# Physics functions
# ---------------------------------------------------------------------------

def wheeler_inductance_uh(N, r_in, l_in):
    """
    Wheeler's short-coil formula.
    N = turns, r_in = coil radius (inches), l_in = winding length (inches)
    Returns inductance in microhenries.
    Valid for l/d < 1 (short coils), error < 1%.
    """
    if l_in <= 0 or r_in <= 0 or N <= 0:
        return 0.0
    return (r_in ** 2 * N ** 2) / (9 * r_in + 10 * l_in)


def coax_length_m(N, D_mm):
    """Length of coax for N turns on a former of diameter D_mm (meters)."""
    return N * math.pi * D_mm * 1e-3


def resonant_freq_mhz(L_uh, C_pf):
    """f = 1/(2*pi*sqrt(LC)), returns MHz."""
    if L_uh <= 0 or C_pf <= 0:
        return 0.0
    L_h = L_uh * 1e-6
    C_f = C_pf * 1e-12
    return 1.0 / (2.0 * math.pi * math.sqrt(L_h * C_f)) / 1e6


def q_factor_estimate(f_mhz, L_uh, coax_key):
    """
    Rough Q estimate for coax trap coil.
    Based on empirical data for typical coax trap construction.
    Q degrades at higher frequencies due to skin effect and dielectric loss.
    Typical measured Q for coax traps: 50-200.
    """
    if L_uh <= 0:
        return 0.0
    # Empirical base Q, falls off with sqrt(f) due to skin effect
    base_q = {
        "RG-58":  85.0,
        "RG-8X":  95.0,
        "RG-213": 110.0,
    }
    bq = base_q.get(coax_key, 90.0)
    # Q degrades at higher frequencies (dielectric, skin effect)
    return bq / math.sqrt(f_mhz / 7.0)


def parallel_resistance_ohms(Q, f_mhz, L_uh):
    """R_p = Q * omega * L (parallel equivalent resistance at resonance)."""
    omega = 2.0 * math.pi * f_mhz * 1e6
    return Q * omega * L_uh * 1e-6


def swr_bandwidth_khz(f_mhz, Q):
    """
    -3 dB bandwidth = f0/Q for a parallel resonant circuit.
    Returns bandwidth in kHz.
    """
    return (f_mhz * 1e3) / Q


def find_turns(target_f_mhz, coax_key, D_mm, max_turns=50):
    """
    Iterate to find N that produces the target resonant frequency.
    Returns (N, L_uh, C_pf, actual_freq_mhz, coax_len_m, Q, Rp_ohm, BW_khz)
    or None if no solution found.
    """
    coax = COAX[coax_key]
    od_mm = coax["od_mm"]
    c_pf_m = coax["c_pf_m"]

    # Convert to inch units for Wheeler formula
    D_in = D_mm / 25.4
    r_in = D_in / 2.0
    od_in = od_mm / 25.4

    best = None
    best_err = 1e9

    # Search N from 1 to max_turns (also try fractional half-turns via 0.5 steps)
    for n10 in range(5, max_turns * 10 + 1):
        N = n10 / 10.0
        l_in = N * od_in  # close-wound, winding length = N * cable OD
        L_uh = wheeler_inductance_uh(N, r_in, l_in)
        if L_uh <= 0:
            continue
        len_m = coax_length_m(N, D_mm)
        C_pf = len_m * c_pf_m
        f = resonant_freq_mhz(L_uh, C_pf)
        if f <= 0:
            continue
        err = abs(f - target_f_mhz)
        if err < best_err:
            best_err = err
            Q = q_factor_estimate(f, L_uh, coax_key)
            Rp = parallel_resistance_ohms(Q, f, L_uh)
            BW = swr_bandwidth_khz(f, Q)
            best = (N, L_uh, C_pf, f, len_m, Q, Rp, BW)

    if best is None:
        return None

    # Accept if within 10% of target
    if abs(best[3] - target_f_mhz) / target_f_mhz > 0.10:
        return None
    return best


def stub_quarter_wave_mm(f_mhz, vf):
    """Quarter-wave stub length in mm."""
    c = 299.792458  # mm/ns
    lambda_mm = (c * 1e6 / f_mhz) * (1.0 / 1e6) * 1e3  # mm
    return lambda_mm * vf / 4.0


def stub_half_wave_mm(f_mhz, vf):
    """Half-wave stub length in mm."""
    return stub_quarter_wave_mm(f_mhz, vf) * 2.0


# ---------------------------------------------------------------------------
# Table generation
# ---------------------------------------------------------------------------

HEADER = [
    "Band", "Freq_MHz", "Coax_Type", "Former_Diam_mm",
    "Turns", "Winding_Length_mm", "Coax_Length_m", "Coax_Length_ft",
    "Inductance_uH", "Capacitance_pF", "Actual_Freq_MHz", "Error_pct",
    "Q_Factor", "Rp_Ohm", "BW_3dB_kHz",
    "Trap_Type", "Notes"
]


def mm_in(mm):
    return mm / 25.4


def build_table():
    rows = []
    for band, f_mhz, f_range in BANDS:
        for coax_key in ["RG-213", "RG-8X", "RG-58"]:
            D_mm = FORMER_DIAMETERS.get(band, {}).get(coax_key, 0.0)

            # VHF/UHF: use coaxial stubs
            if D_mm == 0.0 or f_mhz >= 440.0:
                coax = COAX[coax_key]
                vf = coax["vf"]
                qw_mm = stub_quarter_wave_mm(f_mhz, vf)
                hw_mm = stub_half_wave_mm(f_mhz, vf)
                notes = (f"Coax trap impractical. Use coaxial stubs: "
                         f"QW short stub = {qw_mm:.1f} mm, "
                         f"HW open stub = {hw_mm:.1f} mm (VF={vf})")
                row = [
                    band, f"{f_mhz:.3f}", coax_key, "N/A",
                    "N/A", "N/A", "N/A", "N/A",
                    "N/A", "N/A", "N/A", "N/A",
                    "N/A", "N/A", "N/A",
                    "STUB", notes
                ]
                rows.append(row)
                continue

            if f_mhz >= 144.0:
                max_t = 8
            elif f_mhz >= 50.0:
                max_t = 15
            else:
                max_t = 50

            result = find_turns(f_mhz, coax_key, D_mm, max_turns=max_t)
            if result is None:
                notes = "No solution found for given constraints"
                row = [
                    band, f"{f_mhz:.3f}", coax_key, f"{D_mm:.1f}",
                    "N/A", "N/A", "N/A", "N/A",
                    "N/A", "N/A", "N/A", "N/A",
                    "N/A", "N/A", "N/A",
                    "PARALLEL", notes
                ]
                rows.append(row)
                continue

            N, L_uh, C_pf, f_act, len_m, Q, Rp, BW = result
            coax_od = COAX[coax_key]["od_mm"]
            wind_len_mm = N * coax_od
            err_pct = (f_act - f_mhz) / f_mhz * 100.0
            len_ft = len_m * 3.28084

            # Round N to nearest 0.5
            N_disp = round(N * 2) / 2
            if N_disp == int(N_disp):
                N_str = str(int(N_disp))
            else:
                N_str = f"{N_disp:.1f}"

            notes = (f"Former {D_mm:.0f}mm dia. Trim with {int(C_pf*0.1)}-"
                     f"{int(C_pf*0.05):.0f}pF capacitor for fine tune. "
                     f"Weatherseal with self-amalgamating tape.")

            row = [
                band, f"{f_mhz:.3f}", coax_key, f"{D_mm:.1f}",
                N_str, f"{wind_len_mm:.1f}", f"{len_m:.2f}", f"{len_ft:.2f}",
                f"{L_uh:.3f}", f"{C_pf:.1f}", f"{f_act:.4f}", f"{err_pct:+.2f}",
                f"{Q:.0f}", f"{Rp:.0f}", f"{BW:.1f}",
                "PARALLEL", notes
            ]
            rows.append(row)

    return rows


def write_csv(rows, filepath):
    with open(filepath, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(HEADER)
        writer.writerows(rows)
    print(f"CSV written: {filepath}")


def write_ods(rows, filepath):
    try:
        from odf.opendocument import OpenDocumentSpreadsheet
        from odf.style import Style, TextProperties, TableColumnProperties
        from odf.text import P
        from odf.table import Table, TableColumn, TableRow, TableCell
        from odf import style as odfs

        doc = OpenDocumentSpreadsheet()

        # --- Styles ---
        # Header style
        hdr_style = Style(name="HeaderCell", family="table-cell")
        hdr_style.addElement(TextProperties(fontweight="bold", color="#FFFFFF"))
        doc.automaticstyles.addElement(hdr_style)

        # Column widths
        col_style_wide = Style(name="ColWide", family="table-column")
        col_style_wide.addElement(TableColumnProperties(columnwidth="3.5cm"))
        doc.automaticstyles.addElement(col_style_wide)

        col_style_narrow = Style(name="ColNarrow", family="table-column")
        col_style_narrow.addElement(TableColumnProperties(columnwidth="2.2cm"))
        doc.automaticstyles.addElement(col_style_narrow)

        col_style_notes = Style(name="ColNotes", family="table-column")
        col_style_notes.addElement(TableColumnProperties(columnwidth="12cm"))
        doc.automaticstyles.addElement(col_style_notes)

        table = Table(name="Trap Winding Tables")

        for col in HEADER[:-1]:
            table.addElement(TableColumn(stylename="ColNarrow"))
        table.addElement(TableColumn(stylename="ColNotes"))

        # Header row
        tr = TableRow()
        for col in HEADER:
            tc = TableCell(valuetype="string", stylename="HeaderCell")
            tc.addElement(P(text=col))
            tr.addElement(tc)
        table.addElement(tr)

        # Data rows
        for row in rows:
            tr = TableRow()
            for val in row:
                tc = TableCell(valuetype="string")
                tc.addElement(P(text=str(val)))
                tr.addElement(tc)
            table.addElement(tr)

        doc.spreadsheet.addElement(table)
        doc.save(filepath)
        print(f"ODS written: {filepath}")

    except ImportError:
        print("odfpy not available; ODS not written. Use CSV instead.")


def write_series_resonant_table(filepath):
    """
    Series resonant traps are used in trap verticals.
    At resonance, series RLC has MINIMUM impedance (near zero).
    These are placed as 'traps to ground' in trap verticals.
    Same LC values as parallel but different circuit topology.
    """
    rows = []
    for band, f_mhz, f_range in BANDS:
        for coax_key in ["RG-213", "RG-8X", "RG-58"]:
            D_mm = FORMER_DIAMETERS.get(band, {}).get(coax_key, 0.0)
            if D_mm == 0.0 or f_mhz >= 440.0:
                continue
            result = find_turns(f_mhz, coax_key, D_mm, max_turns=50)
            if result is None:
                continue
            N, L_uh, C_pf, f_act, len_m, Q, Rp, BW = result
            coax_od = COAX[coax_key]["od_mm"]
            wind_len_mm = N * coax_od
            err_pct = (f_act - f_mhz) / f_mhz * 100.0
            len_ft = len_m * 3.28084
            N_disp = round(N * 2) / 2
            N_str = str(int(N_disp)) if N_disp == int(N_disp) else f"{N_disp:.1f}"
            # Series resonant Rp is very LOW (series resistance only)
            Rs = (2.0 * math.pi * f_act * 1e6 * L_uh * 1e-6) / Q
            notes = f"Series trap for vertical. Rs={Rs:.2f} ohms at resonance. Low-Z to ground."
            row = [
                band, f"{f_mhz:.3f}", coax_key, f"{D_mm:.1f}",
                N_str, f"{wind_len_mm:.1f}", f"{len_m:.2f}", f"{len_ft:.2f}",
                f"{L_uh:.3f}", f"{C_pf:.1f}", f"{f_act:.4f}", f"{err_pct:+.2f}",
                f"{Q:.0f}", f"{Rs:.2f}", f"{BW:.1f}",
                "SERIES", notes
            ]
            rows.append(row)
    return rows


def print_summary():
    print("=" * 80)
    print("COAXIAL TRAP WINDING CALCULATOR — SUMMARY")
    print("=" * 80)
    for band, f_mhz, f_range in BANDS[:10]:
        print(f"\n{band} ({f_mhz} MHz):")
        for coax_key in ["RG-213", "RG-8X", "RG-58"]:
            D_mm = FORMER_DIAMETERS.get(band, {}).get(coax_key, 0.0)
            if D_mm == 0.0:
                continue
            result = find_turns(f_mhz, coax_key, D_mm)
            if result is None:
                print(f"  {coax_key:8s}: No solution")
                continue
            N, L_uh, C_pf, f_act, len_m, Q, Rp, BW = result
            N_disp = round(N * 2) / 2
            print(f"  {coax_key:8s}: {N_disp:.1f}T on {D_mm:.0f}mm former, "
                  f"L={L_uh:.2f}uH, C={C_pf:.0f}pF, "
                  f"f={f_act:.4f}MHz, Q={Q:.0f}, Rp={Rp:.0f}ohm, "
                  f"coax={len_m:.2f}m")


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    import os
    outdir = os.path.dirname(os.path.abspath(__file__))

    print("Building parallel resonant trap table...")
    parallel_rows = build_table()

    csv_path = os.path.join(outdir, "trap_winding_tables_parallel.csv")
    write_csv(parallel_rows, csv_path)

    ods_path = os.path.join(outdir, "trap_winding_tables.ods")
    # Build combined ODS with both tables
    try:
        from odf.opendocument import OpenDocumentSpreadsheet
        from odf.text import P
        from odf.table import Table, TableColumn, TableRow, TableCell
        from odf.style import Style, TextProperties, TableColumnProperties

        doc = OpenDocumentSpreadsheet()

        hdr_style = Style(name="H", family="table-cell")
        hdr_style.addElement(TextProperties(fontweight="bold"))
        doc.automaticstyles.addElement(hdr_style)

        def make_sheet(doc, name, rows):
            table = Table(name=name)
            for _ in HEADER[:-1]:
                table.addElement(TableColumn())
            table.addElement(TableColumn())
            tr = TableRow()
            for col in HEADER:
                tc = TableCell(valuetype="string", stylename="H")
                tc.addElement(P(text=col))
                tr.addElement(tc)
            table.addElement(tr)
            for row in rows:
                tr = TableRow()
                for val in row:
                    tc = TableCell(valuetype="string")
                    tc.addElement(P(text=str(val)))
                    tr.addElement(tc)
                table.addElement(tr)
            doc.spreadsheet.addElement(table)

        make_sheet(doc, "Parallel Resonant Traps", parallel_rows)

        print("Building series resonant trap table...")
        series_rows = write_series_resonant_table(ods_path)
        make_sheet(doc, "Series Resonant Traps", series_rows)

        doc.save(ods_path)
        print(f"ODS written: {ods_path}")

    except ImportError:
        print("odfpy not installed. Run: pip install odfpy")
        series_rows = write_series_resonant_table("")
        csv_s = os.path.join(outdir, "trap_winding_tables_series.csv")
        write_csv(series_rows, csv_s)

    print_summary()
    print("\nDone.")
