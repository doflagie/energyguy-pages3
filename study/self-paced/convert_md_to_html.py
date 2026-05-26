#!/usr/bin/env python3
"""
Convert all self-paced course markdown files to HTML.
Matches the home page layout: .container > header.banner + .main-wrapper > nav + main.

Usage: python3 convert_md_to_html.py
"""

import re
from pathlib import Path
import markdown

SELF_PACED = Path('/home/merv/Public/claude/page3/study/self-paced')

CAT_LABELS = {
    'calibration':       'Calibration',
    'calibration_ideas': 'Calibration Ideas',
    'antenna_tools':     'Antenna Tools',
    'antenna_gear':      'Antenna Gear',
    'antennas':          'Antennas',
}

EXTENSIONS = ['tables', 'fenced_code', 'toc', 'sane_lists']


def sidebar_nav(current_cat):
    cats = [
        ('antennas',          'Antennas (73)'),
        ('antenna_gear',      'Antenna Gear (19)'),
        ('antenna_tools',     'Antenna Tools (10)'),
        ('calibration',       'Calibration (35)'),
        ('calibration_ideas', 'Calibration Ideas (17)'),
    ]
    items = '\n'.join(
        f'                    <li><a href="/study/self-paced/{k}/index.html">{label}</a></li>'
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


def md_to_html(md_path, cat, cat_label):
    text = md_path.read_text(encoding='utf-8', errors='replace')

    title = md_path.stem.replace('_', ' ').title()
    for line in text.splitlines():
        if line.startswith('# '):
            title = line[2:].strip()
            break

    md = markdown.Markdown(extensions=EXTENSIONS)
    body = md.convert(text)

    # Fix internal .md links → .html
    body = re.sub(r'href="([^"#]*?)\.md(#[^"]*)?"',
                  lambda m: f'href="{m.group(1)}.html{m.group(2) or ""}"', body)

    nav = sidebar_nav(cat)

    return f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{title}</title>
    <link rel="stylesheet" href="/css/style.css">
    <style>
        .course-content table {{ border-collapse:collapse; width:100%; margin:1rem 0; }}
        .course-content th,
        .course-content td {{ border:1px solid #ccc; padding:.4rem .6rem; }}
        .course-content th {{ background:#f4f4f4; }}
        .course-content code {{ background:#f4f4f4; padding:.1em .3em; border-radius:3px; font-size:.9em; }}
        .course-content pre code {{ background:none; padding:0; }}
        .course-content pre {{ background:#f4f4f4; padding:1rem; overflow-x:auto; border-radius:4px; }}
        .course-content blockquote {{ border-left:3px solid #ccc; margin-left:0; padding-left:1rem; color:#666; }}
        .course-content hr {{ border:none; border-top:2px solid #ccc; margin:2rem 0; }}
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
            <main class="course-content">
{body}
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
    written = 0
    for cat_dir in sorted(SELF_PACED.iterdir()):
        if not cat_dir.is_dir() or cat_dir.name not in CAT_LABELS:
            continue
        cat = cat_dir.name
        cat_label = CAT_LABELS[cat]
        for course_dir in sorted(cat_dir.iterdir()):
            if not course_dir.is_dir():
                continue
            for md_path in sorted(course_dir.glob('*.md')):
                html = md_to_html(md_path, cat, cat_label)
                html_path = md_path.with_suffix('.html')
                html_path.write_text(html, encoding='utf-8')
                written += 1
        print(f"  {cat}: done")
    print(f"Total: {written} HTML files written.")


if __name__ == '__main__':
    main()
