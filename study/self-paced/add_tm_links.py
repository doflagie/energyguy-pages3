#!/usr/bin/env python3
"""
Add TM handout links to the top of every self-paced course markdown file.

Inserts a bold link on the line immediately after the first # heading in each
README.md, unit_*.md, and final_assessment.md. Safe to re-run — skips files
that already contain the link.

Usage: python3 add_tm_links.py
"""

import re
from pathlib import Path

STUDY = Path('/home/merv/Public/claude/page3/study/self-paced')
PAGE3 = Path('/home/merv/Public/claude/page3')

# Maps study subdir name → page3 TM directory name (handles case differences)
CAT_MAP = {
    'calibration':       'calibration',
    'calibration_ideas': 'Calibration_Ideas',
    'antenna_tools':     'antenna_tools',
    'antenna_gear':      'antenna_gear',
    'antennas':          'antennas',
}


def get_tm_num(tm_path):
    src = tm_path.read_text(encoding='utf-8', errors='replace')
    m = re.search(r'<div class="tm-number">(.*?)</div>', src)
    return m.group(1).strip() if m else None


def add_link(md_path, link_md):
    text = md_path.read_text(encoding='utf-8')
    # Idempotency: skip if link already present
    if link_md.split('(')[0] in text:   # check the label part before the URL
        return 'already'
    lines = text.splitlines(keepends=True)
    for i, line in enumerate(lines):
        if line.startswith('# '):
            lines.insert(i + 1, link_md + '\n')
            md_path.write_text(''.join(lines), encoding='utf-8')
            return 'ok'
    return 'no-heading'


def main():
    updated = already = skipped = 0

    for cat_dir in sorted(STUDY.iterdir()):
        cat = cat_dir.name
        if cat not in CAT_MAP or not cat_dir.is_dir():
            continue
        tm_dir_name = CAT_MAP[cat]

        for course_dir in sorted(cat_dir.iterdir()):
            if not course_dir.is_dir():
                continue
            subdir   = course_dir.name
            tm_path  = PAGE3 / tm_dir_name / subdir / 'tm.html'

            if not tm_path.exists():
                print(f"  MISSING TM: {tm_dir_name}/{subdir}/tm.html")
                skipped += 1
                continue

            tm_num = get_tm_num(tm_path)
            if not tm_num:
                print(f"  NO TM-NUM: {tm_dir_name}/{subdir}/tm.html")
                skipped += 1
                continue

            # Relative path: from study/self-paced/<cat>/<subdir>/ up to page3/<tm_dir>/<subdir>/tm.html
            rel = f'../../../{tm_dir_name}/{subdir}/tm.html'
            link_md = f'**[{tm_num} — Open Handout]({rel})**'

            for md_file in sorted(course_dir.glob('*.md')):
                result = add_link(md_file, link_md)
                if result == 'ok':
                    updated += 1
                elif result == 'already':
                    already += 1
                else:
                    print(f"  NO-HEADING: {md_file.relative_to(STUDY)}")
                    skipped += 1

    print(f"Done.  {updated} updated,  {already} already had link,  {skipped} skipped.")


if __name__ == '__main__':
    main()
