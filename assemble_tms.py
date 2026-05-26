#!/usr/bin/env python3
"""
assemble_tms.py — Assemble multi-section HTML directories into Navy TM-format
technical manuals.  Extracts <main> content from existing section files and
re-wraps it in the standard TM frame (cover block, TOC, chapter headings,
WARNING/CAUTION/NOTE boxes, TM CSS).

Usage:
  python3 assemble_tms.py calibration          # all 29 instruments
  python3 assemble_tms.py antennas             # 8 antennas with full sections
  python3 assemble_tms.py calibration nanovna-vector-network-analyzer  # one

Output: <subdir>/tm.html  (adds "View Technical Manual" link to index.html)
"""

import os, re, sys

PAGE3 = os.path.dirname(os.path.abspath(__file__))

# ── Section files → chapter number/letter + title ──────────────────────────
SECTION_ORDER = [
    ('overview.html',              '1', 'GENERAL INFORMATION'),
    ('theory_of_operation.html',   '2', 'THEORY OF OPERATION'),
    ('construction_methods.html',  '3', 'MATERIALS AND CONSTRUCTION'),
    ('assembly_procedures.html',   '4', 'ASSEMBLY PROCEDURES'),
    ('calibration_procedure.html', '5', 'CALIBRATION PROCEDURE'),
    ('tuning_methods.html',        '6', 'TUNING AND ADJUSTMENT'),
    ('verification.html',          '7', 'VERIFICATION'),
    ('calculations.html',          'A', 'CALCULATIONS AND FORMULAS'),
    ('example_results.html',       'B', 'EXAMPLE RESULTS'),
]
KNOWN_SECTIONS = {f for f, _, _ in SECTION_ORDER}
SKIP_FILES     = {'index.html', 'cyd_esp32_optional.html'}

# ── TM prefix by section tree ───────────────────────────────────────────────
TM_PREFIX = {
    'calibration': 'TM-INST',
    'antennas':    'TM-ANT',
    'test_gear':   'TM-TGEAR',
}

# ── HTML template pieces ─────────────────────────────────────────────────────
NAV_CAL = """
        <nav>
            <h3>Main Navigation</h3>
            <ul>
                <li><a href="/">Home</a></li>
                <li><a href="../../about/bio.html">About</a></li>
            </ul>
            <details class="nav-group" open>
                <summary>Calibration Library</summary>
                <ul><li><a href="../index.html">Calibration Library</a></li></ul>
            </details>
            <details class="nav-group" open>
                <summary>Test Equipment</summary>
                <ul><li><a href="../../test_gear/index.html">Test Equipment Library</a></li></ul>
            </details>
            <h3>Standards</h3>
            <ul>
                <li><a href="../../Calibration_Ideas/index.html">Calibration Standards</a></li>
            </ul>
        </nav>
"""
NAV_ANT = """
        <nav>
            <h3>Main Navigation</h3>
            <ul>
                <li><a href="/">Home</a></li>
                <li><a href="../../about/bio.html">About</a></li>
            </ul>
            <details class="nav-group" open>
                <summary>Antenna Library</summary>
                <ul><li><a href="../index.html">Antenna Library</a></li></ul>
            </details>
            <details class="nav-group" open>
                <summary>Test Equipment</summary>
                <ul><li><a href="../../test_gear/index.html">Test Equipment Library</a></li></ul>
            </details>
        </nav>
"""
NAV_TG = """
        <nav>
            <h3>Main Navigation</h3>
            <ul>
                <li><a href="/">Home</a></li>
                <li><a href="../../about/bio.html">About</a></li>
            </ul>
            <details class="nav-group" open>
                <summary>Test Equipment</summary>
                <ul><li><a href="../index.html">Test Equipment Library</a></li></ul>
            </details>
            <h3>Standards</h3>
            <ul>
                <li><a href="../../Calibration_Ideas/index.html">Calibration Standards</a></li>
            </ul>
        </nav>
"""
NAV_BY_TREE = {'calibration': NAV_CAL, 'antennas': NAV_ANT, 'test_gear': NAV_TG}

FOOTER = """
    <footer>
        <div class="address-block">
            <h3>Merv's Brain Dump</h3>
            <p>Mervyn Martin (Merv), Proprietor</p>
            <p>Amateur Radio: KO6NNH &nbsp;&bull;&nbsp; Merced, California 95340</p>
            <div class="copyright">
                <p>&copy; 2026 Merv's Brain Dump. All rights reserved.</p>
                <p>Documentation shared under <a href="https://creativecommons.org/licenses/by-sa/4.0/">CC BY-SA 4.0</a>.</p>
            </div>
        </div>
    </footer>
"""

TM_CSS = """<style>
.tm-cover{border:3px solid #2c5f8d;padding:28px 36px;text-align:center;margin-bottom:28px;background:#f8f8f4}
.tm-classification{font-size:1.05em;font-weight:bold;color:#006600;letter-spacing:3px;margin-bottom:16px}
.tm-number{font-family:'Courier New',monospace;font-size:1.15em;font-weight:bold;color:#2c5f8d;margin-bottom:8px}
.tm-title{font-size:1.7em;font-weight:bold;color:#1a3a5c;text-transform:uppercase;letter-spacing:1px;margin:12px 0 8px;line-height:1.3}
.tm-subtitle{font-size:.98em;font-style:italic;color:#555;margin-bottom:12px}
.tm-meta{font-size:.88em;color:#555;margin-top:16px;border-top:1px solid #aaa;padding-top:12px}
.tm-toc{background:#f0f4f8;border:1px solid #c0ccd8;padding:18px 28px;margin-bottom:28px}
.tm-toc h2{font-size:1.05em;color:#2c5f8d;border-bottom:1px solid #4a7ba7;padding-bottom:6px;margin-bottom:10px;text-transform:uppercase;letter-spacing:1px}
.tm-toc ol{margin:0;padding-left:22px}
.tm-toc li{margin:4px 0;font-size:.93em}
.tm-toc a{color:#2c5f8d;text-decoration:none}
.tm-toc a:hover{text-decoration:underline}
.chapter-heading{background:#2c5f8d;color:#fff;padding:9px 18px;margin-top:32px;margin-bottom:16px;font-size:1.15em;font-weight:bold;text-transform:uppercase;letter-spacing:1px}
.chapter-body h1,.chapter-body h2{color:#1a3a5c;border-bottom:2px solid #4a7ba7;padding-bottom:4px;margin-top:22px;margin-bottom:10px}
.chapter-body h3{color:#2c5f8d;margin-top:16px;margin-bottom:8px}
.chapter-body table{width:100%;border-collapse:collapse;margin:12px 0;font-size:.93em}
.chapter-body th{background:#4a7ba7;color:#fff;padding:7px 11px;text-align:left;font-size:.88em;text-transform:uppercase}
.chapter-body td{border:1px solid #c0ccd8;padding:6px 11px;vertical-align:top}
.chapter-body tr:nth-child(even) td{background:#f4f7fb}
.chapter-body ol,.chapter-body ul{margin:8px 0 8px 20px}
.chapter-body li{margin:5px 0;line-height:1.5}
.chapter-body p{margin:8px 0 10px}
.chapter-body pre,.chapter-body code{background:#f4f4ee;border:1px solid #ddd;padding:2px 5px;font-size:.9em}
.chapter-body blockquote{border-left:4px solid #4a7ba7;margin:12px 0;padding:8px 16px;background:#f0f5ff;color:#333}
.back-link{margin-top:28px;padding-top:12px;border-top:1px solid #c0ccd8;font-size:.9em}
</style>"""


# ── Utility functions ────────────────────────────────────────────────────────

def extract_main(path):
    """Return the innerHTML of <main>...</main>, or '' if not found."""
    try:
        html = open(path, encoding='utf-8', errors='replace').read()
    except FileNotFoundError:
        return ''
    m = re.search(r'<main[^>]*>(.*?)</main>', html, re.DOTALL)
    return m.group(1).strip() if m else ''


def strip_first_h1(content):
    """Remove the first <h1>…</h1> from content (it becomes the chapter heading)."""
    return re.sub(r'<h1[^>]*>.*?</h1>', '', content, count=1, flags=re.DOTALL).strip()


def get_h1_text(content):
    """Return plain text of first <h1> in content, or ''."""
    m = re.search(r'<h1[^>]*>(.*?)</h1>', content, re.DOTALL)
    return re.sub(r'<[^>]+>', '', m.group(1)).strip() if m else ''


def dir_title(dirname):
    """Convert directory name to a display title."""
    return dirname.replace('-', ' ').replace('_', ' ').title()


def get_instrument_title(subdir_path):
    """Best-effort title extraction from overview.html."""
    ov = extract_main(os.path.join(subdir_path, 'overview.html'))
    t = get_h1_text(ov)
    return t if t else dir_title(os.path.basename(subdir_path))


def extra_appendix_label(idx):
    """C, D, E, … for additional appendices beyond B."""
    return chr(ord('C') + idx)


def friendly_filename(fname):
    """Convert a filename like oscilloscope_timebase_cal.html to a readable title."""
    stem = os.path.splitext(fname)[0]
    return stem.replace('_', ' ').replace('-', ' ').title()


# ── TM page generator ────────────────────────────────────────────────────────

def build_tm(tm_number, title, subtitle, tree, subdir_path):
    """
    Build a complete TM HTML string for one instrument/antenna directory.
    Returns the HTML string.
    """
    chapters_html = []
    toc_entries   = []

    # -- Standard sections (Chapters 1-7, Appendices A-B) -------------------
    for fname, ch_num, ch_title in SECTION_ORDER:
        content = extract_main(os.path.join(subdir_path, fname))
        if not content:
            continue
        body = strip_first_h1(content)
        anchor = f'ch{ch_num}'
        if ch_num.isdigit():
            heading = f'CHAPTER {ch_num} &mdash; {ch_title}'
        else:
            heading = f'APPENDIX {ch_num} &mdash; {ch_title}'
        toc_entries.append((anchor, heading))
        chapters_html.append(
            f'<h2 class="chapter-heading" id="{anchor}">{heading}</h2>\n'
            f'<div class="chapter-body">\n{body}\n</div>\n'
        )

    # -- Extra instrument-specific files (become additional appendices) ------
    extra_idx = 0
    all_html = [f for f in sorted(os.listdir(subdir_path))
                if f.endswith('.html') and f not in KNOWN_SECTIONS and f not in SKIP_FILES
                and f != 'index.html' and f != 'tm.html']
    for fname in all_html:
        content = extract_main(os.path.join(subdir_path, fname))
        if not content or len(content) < 100:
            continue
        body = strip_first_h1(content)
        letter  = extra_appendix_label(extra_idx)
        heading = f'APPENDIX {letter} &mdash; {friendly_filename(fname).upper()}'
        anchor  = f'chapp{letter.lower()}'
        toc_entries.append((anchor, heading))
        chapters_html.append(
            f'<h2 class="chapter-heading" id="{anchor}">{heading}</h2>\n'
            f'<div class="chapter-body">\n{body}\n</div>\n'
        )
        extra_idx += 1

    # -- TOC -----------------------------------------------------------------
    toc_html = '\n'.join(
        f'            <li><a href="#{a}">{h}</a></li>'
        for a, h in toc_entries
    )

    nav = NAV_BY_TREE.get(tree, NAV_CAL)
    depth = '../../'   # subdir is 2 levels deep in all three trees

    return f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{tm_number} {title} - Merv's Brain Dump</title>
    <link rel="stylesheet" href="{depth}css/style.css">
    {TM_CSS}
</head>
<body>
    <div class="container">
        <header>
            <div class="banner">
                <div>
                    <h1>Merv's Brain Dump</h1>
                    <p class="tagline">Amateur Radio &bull; Electronics &bull; Engineering</p>
                </div>
            </div>
        </header>
        <div class="main-wrapper">
            {nav}
            <main>
                <div class="tm-cover">
                    <div class="tm-classification">UNCLASSIFIED</div>
                    <div class="tm-number">{tm_number}</div>
                    <div class="tm-title">{title}</div>
                    <div class="tm-subtitle">{subtitle}</div>
                    <div class="tm-meta">
                        Prepared by: Mervyn Martin, KO6NNH &nbsp;&bull;&nbsp;
                        Merced, California &nbsp;&bull;&nbsp; 26 May 2026<br>
                        <em>Amateur Radio / Electronics &mdash; Not for commercial calibration use</em>
                    </div>
                </div>

                <div class="tm-toc">
                    <h2>Table of Contents</h2>
                    <ol>
{toc_html}
                    </ol>
                </div>

                {''.join(chapters_html)}

                <div class="back-link">
                    <a href="../index.html">&larr; Back to index</a>
                </div>
            </main>
        </div>
    </div>
    {FOOTER}
</body>
</html>
"""


def add_tm_link_to_index(index_path):
    """Insert a TM link near the top of <main> in index.html if not already present."""
    if not os.path.exists(index_path):
        return False
    content = open(index_path, encoding='utf-8').read()
    if 'tm.html' in content:
        return False   # already linked
    # Insert after <main>
    link = ('<p style="margin-bottom:14px;">'
            '<a href="tm.html" style="font-weight:bold;font-size:1.1em;color:#1a3a5c;">'
            '&#9654; View Technical Manual (TM format)</a></p>\n')
    new = re.sub(r'(<main[^>]*>)', r'\1\n                ' + link, content, count=1)
    if new == content:
        return False
    open(index_path, 'w', encoding='utf-8').write(new)
    return True


# ── Main processing ──────────────────────────────────────────────────────────

def process_tree(tree_name, target_subdir=None):
    tree_path = os.path.join(PAGE3, tree_name)
    prefix    = TM_PREFIX.get(tree_name, 'TM-MISC')

    # Collect subdirs that have at least overview.html
    subdirs = sorted(
        d for d in os.listdir(tree_path)
        if os.path.isdir(os.path.join(tree_path, d))
        and os.path.exists(os.path.join(tree_path, d, 'overview.html'))
    )

    if target_subdir:
        if target_subdir not in subdirs:
            print(f'ERROR: {target_subdir} not found or has no overview.html')
            return
        subdirs = [target_subdir]

    generated = skipped = errors = 0

    for i, dirname in enumerate(subdirs, 1):
        subdir_path = os.path.join(tree_path, dirname)
        tm_number   = f'{prefix}-{i:03d}'
        title       = get_instrument_title(subdir_path).upper()
        subtitle    = f'Construction, Theory, Calibration and Verification Procedures'
        out_path    = os.path.join(subdir_path, 'tm.html')

        try:
            html = build_tm(tm_number, title, subtitle, tree_name, subdir_path)
            # Require at least two chapters to be worth writing
            if html.count('chapter-heading') < 2:
                print(f'  SKIP {dirname} (insufficient content)')
                skipped += 1
                continue
            open(out_path, 'w', encoding='utf-8').write(html)
            link_added = add_tm_link_to_index(os.path.join(subdir_path, 'index.html'))
            print(f'  OK   {tm_number}  {dirname}{"  [+link]" if link_added else ""}')
            generated += 1
        except Exception as e:
            print(f'  ERR  {dirname}: {e}')
            errors += 1

    print(f'\n{tree_name}: Generated {generated}  Skipped {skipped}  Errors {errors}')


if __name__ == '__main__':
    args = sys.argv[1:]
    if not args:
        print('Usage: python3 assemble_tms.py <tree> [subdir]')
        print('  tree: calibration | antennas | test_gear')
        sys.exit(1)

    tree = args[0]
    subdir = args[1] if len(args) > 1 else None

    if tree not in TM_PREFIX:
        print(f'Unknown tree "{tree}". Use: calibration | antennas | test_gear')
        sys.exit(1)

    process_tree(tree, subdir)
