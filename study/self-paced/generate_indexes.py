#!/usr/bin/env python3
"""
Generate HTML index pages for each self-paced course category.
Matches the home page layout: .container > header.banner + .main-wrapper > nav + main.

Usage: python3 generate_indexes.py
"""

import re
from pathlib import Path

SELF_PACED = Path('/home/merv/Public/claude/page3/study/self-paced')
PAGE3      = Path('/home/merv/Public/claude/page3')

CAT_MAP = {
    'calibration':       ('Calibration',       'calibration'),
    'calibration_ideas': ('Calibration Ideas', 'Calibration_Ideas'),
    'antenna_tools':     ('Antenna Tools',     'antenna_tools'),
    'antenna_gear':      ('Antenna Gear',      'antenna_gear'),
    'antennas':          ('Antennas',          'antennas'),
}

CAT_COUNTS = {
    'antennas':          73,
    'antenna_gear':      19,
    'antenna_tools':     10,
    'calibration':       35,
    'calibration_ideas': 17,
}

UNIT_LABELS = {
    'unit_1_theory.md':        'Unit 1 — Theory',
    'unit_2_materials.md':     'Unit 2 — Materials',
    'unit_2_construction.md':  'Unit 2 — Construction',
    'unit_2_components.md':    'Unit 2 — Components',
    'unit_2_equipment.md':     'Unit 2 — Equipment',
    'unit_3_assembly.md':      'Unit 3 — Assembly',
    'unit_3_construction.md':  'Unit 3 — Construction',
    'unit_3_setup.md':         'Unit 3 — Setup',
    'unit_4_calibration.md':   'Unit 4 — Calibration',
    'unit_4_operation.md':     'Unit 4 — Operation',
    'unit_4_alignment.md':     'Unit 4 — Alignment',
    'unit_5_calibration.md':   'Unit 5 — Calibration',
    'unit_5_verification.md':  'Unit 5 — Verification',
    'final_assessment.md':     'Final Assessment',
}


def sidebar_nav(current_cat):
    cats = [
        ('antennas',          'Antennas'),
        ('antenna_gear',      'Antenna Gear'),
        ('antenna_tools',     'Antenna Tools'),
        ('calibration',       'Calibration'),
        ('calibration_ideas', 'Calibration Ideas'),
    ]
    items = '\n'.join(
        f'                    <li><a href="/study/self-paced/{k}/index.html">{label} ({CAT_COUNTS[k]})</a></li>'
        for k, label in cats
    )
    return f"""            <nav>
                <h3>Main Navigation</h3>
                <ul>
                    <li><a href="/">Home</a></li>
                    <li><a href="/study/index.html">Study &amp; Training</a></li>
                </ul>
                <details class="nav-group" open>
                    <summary>Self-Paced Courses</summary>
                    <ul>
{items}
                    </ul>
                </details>
            </nav>"""


def get_title_and_tm(readme_path):
    text = readme_path.read_text(encoding='utf-8', errors='replace')
    lines = text.splitlines()
    title = ''
    tm_num = ''
    tm_href = ''
    for line in lines:
        if line.startswith('# ') and not title:
            title = line[2:].strip()
            title = re.sub(r'^Self-Paced Course\s*[—-]\s*', '', title)
        m = re.search(r'\*\*\[([^\]]+) — Open Handout\]\(([^)]+)\)\*\*', line)
        if m:
            tm_num  = m.group(1)
            tm_href = m.group(2)
            break
    return title, tm_num, tm_href


def make_index(cat, cat_label, tm_dir_name):
    cat_dir = SELF_PACED / cat
    course_dirs = sorted(d for d in cat_dir.iterdir() if d.is_dir())

    rows = []
    for cdir in course_dirs:
        readme = cdir / 'README.md'
        if not readme.exists():
            continue
        title, tm_num, tm_href = get_title_and_tm(readme)
        if not title:
            title = cdir.name.replace('_', ' ').replace('-', ' ').title()

        unit_links = []
        for fname, label in UNIT_LABELS.items():
            fpath = cdir / fname
            if fpath.exists():
                html_fname = fname.replace('.md', '.html')
                unit_links.append(f'<a href="{cdir.name}/{html_fname}">{label}</a>')

        tm_link = (f'<a class="tm-link" href="{tm_href}">{tm_num}</a>'
                   if tm_href else tm_num)

        rows.append(f"""
    <tr>
      <td class="course-title"><a href="{cdir.name}/README.html">{title}</a></td>
      <td class="tm-col">{tm_link}</td>
      <td class="units-col">{'  ·  '.join(unit_links)}</td>
    </tr>""")

    nav = sidebar_nav(cat)

    return f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Self-Paced Courses — {cat_label}</title>
    <link rel="stylesheet" href="/css/style.css">
    <style>
        .course-table {{ width:100%; border-collapse:collapse; margin-top:1.5rem; }}
        .course-table th {{ text-align:left; padding:.4rem .6rem; border-bottom:2px solid #ccc; }}
        .course-table td {{ padding:.35rem .6rem; border-bottom:1px solid #eee; vertical-align:top; }}
        .tm-link {{ font-family:monospace; white-space:nowrap; }}
        .units-col {{ font-size:.85em; }}
        .units-col a {{ margin-right:.2em; }}
        .course-title a {{ font-weight:600; }}
    </style>
</head>
<body>
    <div class="container">
        <header>
            <div class="banner">
                <div>
                    <h1>Merv's Brain Dump</h1>
                    <p class="tagline">When you build it, it teaches.</p>
                </div>
            </div>
        </header>
        <div class="main-wrapper">
{nav}
            <main>
                <h1>Self-Paced Courses — {cat_label}</h1>
                <p>{len(rows)} courses. Click a course title to open its README, or jump directly to any unit.
                The <strong>TM</strong> link opens the handout for that course.</p>

                <table class="course-table">
                    <thead>
                        <tr>
                            <th>Course</th>
                            <th>TM</th>
                            <th>Units</th>
                        </tr>
                    </thead>
                    <tbody>{''.join(rows)}
                    </tbody>
                </table>
            </main>
        </div>
    </div>
    <footer>
        <div class="address-block">
            <h3>Merv's Brain Dump</h3>
            <p>Mervyn Martin (Merv), Proprietor</p>
            <p>Amateur Callsign: KO6NNH</p>
            <p>Merced, California 95340</p>
            <div class="copyright">
                <p>&copy; 2026 Merv's Brain Dump. All rights reserved.</p>
                <p>Documentation shared under <a href="https://creativecommons.org/licenses/by-sa/4.0/" target="_blank">CC BY-SA 4.0</a> unless otherwise noted.</p>
            </div>
        </div>
    </footer>
</body>
</html>
"""


def main():
    for cat, (cat_label, tm_dir_name) in CAT_MAP.items():
        cat_dir = SELF_PACED / cat
        if not cat_dir.exists():
            print(f"  SKIP (missing): {cat}")
            continue
        html = make_index(cat, cat_label, tm_dir_name)
        out = cat_dir / 'index.html'
        out.write_text(html, encoding='utf-8')
        print(f"  OK: {cat}/index.html")


if __name__ == '__main__':
    main()
