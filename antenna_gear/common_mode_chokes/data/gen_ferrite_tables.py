#!/usr/bin/env python3
"""
gen_ferrite_tables.py
Generate ferrite_impedance_table.ods — ODS spreadsheet with ferrite core
material properties and choking impedance vs. frequency tables.
TM-CMC-001  Rev A

Uses Python stdlib only (zipfile + xml strings).  No external dependencies.
Output: ferrite_impedance_table.ods (same directory as this script)
"""

import zipfile
import io
import os
import sys

# ── Data ──────────────────────────────────────────────────────────────────────

# Sheet 1: Ferrite material properties
MAT_HEADERS = [
    "Mix", "µ_initial", "µ_real peak (MHz)", "Loss dominant range (MHz)",
    "Curie temp (°C)", "Primary HF use", "Common cores", "A_L FT-240 (nH/N²)",
    "A_L FT-140 (nH/N²)", "A_L FT-82 (nH/N²)", "Notes"
]

MAT_DATA = [
    ["31", 1500, "1-10",  "1-300",   230, "160M-6M broadband",      "FT-240-31, FT-140-31, beads", 1800, 1000, 570,  "Best all-HF choking material"],
    ["43", 850,  "10-30", "10-500",  200, "40M-2M, upper HF/VHF",   "FT-240-43, FT-140-43, FT-82-43", 1100, 625, 557, "Best 20M-70cm choice"],
    ["61", 125,  "50-200","200-1500",350, "VHF/UHF (50-1300 MHz)", "FT-50-61, FT-82-61, FT-114-61",  195, 111,  36,  "Low loss VHF; use for bead-on-coax"],
    ["52", 250,  "30-100","100-2000",350, "UHF (100-2000 MHz)",     "FT-37-52, FT-50-52",             248, 138,  40,  "Lower µ than 61; good 100-2000 MHz"],
    ["77", 2000, "0.1-2", "0.1-10",  200, "LF/160M (0.1-10 MHz)",  "FT-240-77, FT-140-77",           7500,4500,1300, "High µ; saturates at lower drive"],
    ["75", 5000, "0.01-1","0.01-2",  130, "Very LF, 60Hz-2 MHz",   "FT-240-75, pot cores",           8500,5100,1900, "Very high µ; avoid above 3 MHz"],
    ["67", 40,   "100-600","200-2000",500,"Microwave (200-2000 MHz)","FT-37-67, small beads",          22,  13,   5,  "Nickel-zinc; very low loss VHF+"],
    ["73", 2500, "0.1-2", "0.1-20",  150, "LF/AM broadcast, 160M",  "FT-240-73",                      7000,4000,1300,"High µ for low frequency"],
]

# Sheet 2: FT-240-31, 10 turns — impedance vs frequency
TOROID_HEADERS = ["Freq (MHz)", "R (Ω)", "jX (Ω)", "|Z| (Ω)", "CMR @150Ω path (dB)", "CMR @50Ω path (dB)", "Note"]

# FT-240-31, 10 turns
FT240_31_10T = [
    [1.8,   450,  2040, 2090, 22.9, 32.3, "160M good"],
    [3.5,   750,  3100, 3190, 26.5, 36.1, "80M excellent"],
    [7.0,  1500,  3900, 4180, 29.4, 38.4, "40M excellent"],
    [10.1, 2200,  3600, 4220, 29.5, 38.5, "30M peak region"],
    [14.0, 3200,  2800, 4250, 29.6, 38.6, "20M excellent"],
    [18.1, 3800,  1900, 4240, 29.6, 38.6, "17M excellent"],
    [21.0, 3900,  1200, 4080, 29.4, 38.4, "15M good"],
    [24.9, 3800,   600, 3850, 28.8, 37.8, "12M good"],
    [28.0, 3600,   200, 3605, 27.6, 37.1, "10M adequate"],
    [50.0, 1800,  -400, 1844, 21.8, 31.3, "6M marginal"],
    [144,   400,  -500,  640,  12.6, 22.1, "2M poor — use ferrite beads"],
]

# FT-240-43, 12 turns
FT240_43_12T = [
    [1.8,    80,   700,  705,  13.4, 22.9, "160M poor — use Mix 31"],
    [3.5,   210,  2450, 2460, 24.3, 33.8, "80M marginal"],
    [7.0,   550,  4200, 4236, 29.1, 38.6, "40M good"],
    [10.1, 1100,  5400, 5511, 31.3, 40.8, "30M excellent"],
    [14.0, 2800,  5100, 5819, 31.9, 41.3, "20M peak"],
    [18.1, 4100,  4200, 5874, 31.9, 41.4, "17M peak"],
    [21.0, 5000,  3600, 6153, 32.3, 41.8, "15M peak"],
    [24.9, 5800,  2800, 6434, 32.6, 42.2, "12M excellent"],
    [28.0, 6000,  2200, 6393, 32.6, 42.1, "10M excellent"],
    [50.0, 4800,   800, 4866, 30.2, 39.7, "6M very good"],
    [144,  1200,  -600, 1342, 19.0, 28.6, "2M adequate"],
    [222,   500,  -700,  860, 15.2, 24.7, "1.25M poor"],
]

# FT-240-43, 8 turns
FT240_43_8T = [
    [1.8,    36,   310,  312,  6.4,  15.9, "160M very poor"],
    [3.5,    93,  1089, 1093, 17.3, 26.8, "80M poor"],
    [7.0,   244,  1867, 1883, 22.0, 31.5, "40M marginal"],
    [14.0, 1244,  2267, 2587, 24.8, 34.3, "20M good"],
    [21.0, 2222,  1600, 2736, 25.2, 34.7, "15M good"],
    [28.0, 2667,   978, 2841, 25.6, 35.0, "10M good"],
    [50.0, 2133,   356, 2163, 23.2, 32.7, "6M good"],
    [144,   533,  -267,  596, 12.0, 21.5, "2M marginal"],
]

# Stacked FT-240-31 + FT-240-43, 8 turns
STACKED_31_43_8T = [
    [1.8,   486,  2350, 2400, 24.1, 33.6, "160M good (sum of individual)"],
    [3.5,   843,  3189, 3298, 26.8, 36.3, "80M excellent"],
    [7.0,  1744,  5767, 6025, 32.1, 41.6, "40M excellent"],
    [14.0, 4444,  7367, 8679, 35.3, 44.8, "20M outstanding"],
    [21.0, 7222,  5200, 8889, 35.5, 45.0, "15M outstanding"],
    [28.0, 8667,  3178, 9234, 35.8, 45.3, "10M outstanding"],
    [50.0, 6933,  1156, 7029, 33.4, 42.9, "6M excellent"],
]

# Sheet 3: Snap-on beads — impedance per bead and N-bead stacks
BEAD_HEADERS = [
    "Freq (MHz)", "|Z| Mix31 1pc (Ω)", "|Z| Mix31 6pc (Ω)", "|Z| Mix31 12pc (Ω)",
    "|Z| Mix43 1pc (Ω)", "|Z| Mix43 6pc (Ω)", "CMR Mix31-12pc @150Ω (dB)",
    "CMR Mix43-6pc @150Ω (dB)", "Recommended config"
]

BEAD_DATA = [
    [1.8,  195,  1170,  2340,   60,   360,  23.9, 7.6,  "12× Mix31"],
    [3.5,  270,  1620,  3240,  115,   690,  26.7,12.7,  "12× Mix31"],
    [7.0,  430,  2580,  5160,  190,  1140,  30.7,17.6,  "12× Mix31 or 6× Mix31"],
    [14.0, 545,  3270,  6540,  360,  2160,  32.8,23.2,  "6× Mix31 adequate"],
    [21.0, 590,  3540,  7080,  520,  3120,  33.5,26.4,  "6× Mix31"],
    [28.0, 600,  3600,  7200,  600,  3600,  33.6,27.6,  "6× Mix31 or Mix43"],
    [50.0, 600,  3600,  7200,  672,  4032,  33.6,28.6,  "6× Mix43 better at 6M"],
    [100,  510,  3060,  6120,  735,  4410,  32.2,29.4,  "6× Mix43"],
    [144,  480,  2880,  5760,  750,  4500,  31.6,29.6,  "6× Mix43 excellent"],
    [222,  400,  2400,  4800,  680,  4080,  29.8,28.7,  "6× Mix43"],
    [432,  250,  1500,  3000,  490,  2940,  26.0,25.8,  "6× Mix43"],
    [902,  120,   720,  1440,  290,  1740,  17.6,21.5,  "8× Mix43 or FT-50-61"],
]

# Sheet 4: Sleeve balun — dimensions and performance
SLEEVE_HEADERS = [
    "Band", "f₀ (MHz)", "λ/4 free space (mm)", "VF", "l_physical (mm)",
    "Sleeve OD rec (mm)", "Inner coax rec", "Z_peak (Ω)", "BW ±3dB (MHz)",
    "CMR peak (dB)", "Construction method"
]

SLEEVE_DATA = [
    ["6M",   50.1,  1497, 0.96, 1437, 25.4, "RG-8X",  2000,  3, 23.5, "Coiled coax preferred; sleeve 1.44m impractical"],
    ["2M",  144.0,   521, 0.96,  500, 19.1, "RG-58",  6000, 10, 32.0, "Copper tube 19mm, 500mm"],
    ["1.25M",222.0,  338, 0.96,  325, 16.0, "RG-58",  7000, 15, 33.4, "Copper foil tape 325mm"],
    ["70cm",432.0,   174, 0.96,  167, 12.7, "RG-58",  8000, 20, 34.5, "Copper tube 12mm, 167mm"],
    ["33cm",902.0,    83, 0.96,   80,  9.5, "LMR-195",8000, 40, 34.5, "Aluminum collar, 80mm"],
    ["23cm",1296.0,   58, 0.96,   56,  6.4, "LMR-195",7000, 65, 33.4, "Precision tube or PCB"],
]

# Sheet 5: RFI suppression recommendations by symptom
RFI_HEADERS = [
    "Symptom", "Likely cause", "Band(s)", "Recommended choke",
    "Installation point", "Expected improvement (dB)", "Notes"
]

RFI_DATA = [
    ["RF in shack audio", "Common-mode on feedline", "HF all", "FT-240-31 10t",
     "Feedline at shack entry", "20-30", "Most common HF RFI issue"],
    ["RF in microphone", "Common-mode on mic cable", "HF all", "FT-240-31 8t or 8× snap-on Mix31",
     "Back of radio mic input", "15-25", "Wind mic cable on core"],
    ["S-meter deflect on TX", "Common-mode current returning on coax", "HF all", "FT-240-31 10t",
     "Antenna feedpoint", "20-35", "Classic 'RF in the shack' symptom"],
    ["Computer TVI on 2M", "Common-mode on monitor cable", "2M", "6× snap-on Mix43",
     "Monitor cable at computer", "20-30", "Snap-on; no disassembly needed"],
    ["Power supply hum in receiver", "Common-mode on power cable", "All", "4× snap-on Mix31 folded",
     "Supply output cable", "15-25", "Fold cable for 2-pass through each bead"],
    ["USB audio adapter noise", "Common-mode on USB cable", "All", "1-2× Mix31 snap-on, 3-turn",
     "USB cable at adapter", "20-30", "Wind USB cable 3 turns through snap-on"],
    ["LED lighting interference", "Broadband noise via power lines", "HF all", "8× snap-on Mix31",
     "LED driver supply cable", "10-20", "May need to remove offending fixture"],
    ["Neighbor switching supply", "Conducted + radiated broadband", "HF all", "Ferrite at antenna + entry",
     "Both antenna feedpoint AND shack entry", "15-25", "Two chokes; also RX filter"],
    ["VHF antenna to 2M noise floor", "Common-mode path from adjacent HF", "2M", "6× snap-on Mix43 or sleeve",
     "2M coax at shack entry", "20-30", "Isolate from HF feedlines"],
    ["High SWR after choke install", "Choke absorbing differential power", "Any", "Check choke design",
     "N/A", "N/A", "Differential-mode loss means wrong winding or wrong impedance level"],
]

# ── ODS XML generation ─────────────────────────────────────────────────────────

def cell_string(val):
    return f'<table:table-cell office:value-type="string" calcext:value-type="string"><text:p>{val}</text:p></table:table-cell>'

def cell_float(val):
    if isinstance(val, int):
        return f'<table:table-cell office:value-type="float" office:value="{val}"><text:p>{val}</text:p></table:table-cell>'
    return f'<table:table-cell office:value-type="float" office:value="{val:.4f}"><text:p>{val}</text:p></table:table-cell>'

def cell_auto(val):
    if isinstance(val, (int, float)):
        return cell_float(val)
    return cell_string(str(val))

def header_cell(val):
    return (f'<table:table-cell table:style-name="header-cell" '
            f'office:value-type="string" calcext:value-type="string">'
            f'<text:p>{val}</text:p></table:table-cell>')

def make_row(cells):
    return '<table:table-row>' + ''.join(cells) + '</table:table-row>'

def make_sheet(name, headers, rows):
    lines = [f'<table:table table:name="{name}">']
    # Header row
    hcells = [header_cell(h) for h in headers]
    lines.append(make_row(hcells))
    # Data rows
    for row in rows:
        rcells = [cell_auto(v) for v in row]
        lines.append(make_row(rcells))
    lines.append('</table:table>')
    return '\n'.join(lines)

CONTENT_PREFIX = '''<?xml version="1.0" encoding="UTF-8"?>
<office:document-content
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:calcext="urn:org:documentfoundation:names:experimental:calc:xmlns:calcext:1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  office:version="1.3">
<office:automatic-styles>
  <style:style style:name="header-cell" style:family="table-cell">
    <style:table-cell-properties fo:background-color="#C0D9EF" fo:font-weight="bold"/>
  </style:style>
</office:automatic-styles>
<office:body>
<office:spreadsheet>
'''

CONTENT_SUFFIX = '''
</office:spreadsheet>
</office:body>
</office:document-content>'''

MANIFEST = '''<?xml version="1.0" encoding="UTF-8"?>
<manifest:manifest xmlns:manifest="urn:oasis:names:tc:opendocument:xmlns:manifest:1.0">
  <manifest:file-entry manifest:media-type="application/vnd.oasis.opendocument.spreadsheet" manifest:full-path="/"/>
  <manifest:file-entry manifest:media-type="text/xml" manifest:full-path="content.xml"/>
  <manifest:file-entry manifest:media-type="text/xml" manifest:full-path="styles.xml"/>
  <manifest:file-entry manifest:media-type="text/xml" manifest:full-path="meta.xml"/>
</manifest:manifest>'''

STYLES = '''<?xml version="1.0" encoding="UTF-8"?>
<office:document-styles
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  office:version="1.3">
<office:styles/>
</office:document-styles>'''

META = '''<?xml version="1.0" encoding="UTF-8"?>
<office:document-meta
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:dc="http://purl.org/dc/elements/1.1/"
  office:version="1.3">
<office:meta>
  <dc:title>Ferrite Core Impedance Tables TM-CMC-001</dc:title>
  <dc:creator>TM-CMC-001 Generator</dc:creator>
  <dc:description>Common-mode choke ferrite data: materials, impedance vs frequency, RFI guide</dc:description>
</office:meta>
</office:document-meta>'''

MIMETYPE = 'application/vnd.oasis.opendocument.spreadsheet'

def generate_ods(outpath):
    sheet1 = make_sheet("Ferrite Materials",   MAT_HEADERS,    MAT_DATA)
    sheet2 = make_sheet("FT-240-31 10T",       TOROID_HEADERS, FT240_31_10T)
    sheet3 = make_sheet("FT-240-43 12T",       TOROID_HEADERS, FT240_43_12T)
    sheet4 = make_sheet("FT-240-43 8T",        TOROID_HEADERS, FT240_43_8T)
    sheet5 = make_sheet("Stacked 31+43 8T",    TOROID_HEADERS, STACKED_31_43_8T)
    sheet6 = make_sheet("Snap-On Beads",       BEAD_HEADERS,   BEAD_DATA)
    sheet7 = make_sheet("Sleeve Balun Dims",   SLEEVE_HEADERS, SLEEVE_DATA)
    sheet8 = make_sheet("RFI Troubleshooting", RFI_HEADERS,    RFI_DATA)

    content = CONTENT_PREFIX + sheet1 + sheet2 + sheet3 + sheet4 + \
              sheet5 + sheet6 + sheet7 + sheet8 + CONTENT_SUFFIX

    buf = io.BytesIO()
    with zipfile.ZipFile(buf, 'w', compression=zipfile.ZIP_DEFLATED) as zf:
        # mimetype MUST be first and uncompressed
        zf.writestr(zipfile.ZipInfo('mimetype'), MIMETYPE)
        zf.writestr('META-INF/manifest.xml', MANIFEST)
        zf.writestr('content.xml',           content)
        zf.writestr('styles.xml',            STYLES)
        zf.writestr('meta.xml',              META)

    with open(outpath, 'wb') as f:
        f.write(buf.getvalue())
    print(f"Written: {outpath}  ({os.path.getsize(outpath):,} bytes)")

if __name__ == '__main__':
    script_dir = os.path.dirname(os.path.abspath(__file__))
    outpath = os.path.join(script_dir, 'ferrite_impedance_table.ods')
    generate_ods(outpath)
    print("Sheets: Ferrite Materials | FT-240-31 10T | FT-240-43 12T | FT-240-43 8T")
    print("        Stacked 31+43 8T | Snap-On Beads | Sleeve Balun Dims | RFI Troubleshooting")
