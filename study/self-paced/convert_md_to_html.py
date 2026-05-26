#!/usr/bin/env python3
"""
Convert all self-paced course markdown files to HTML.
Wraps each .md in a full HTML page with site CSS and nav.
Output: alongside each .md as <basename>.html

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


def md_to_html(md_path, cat_label):
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

    html = f"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>{title}</title>
<link rel="stylesheet" href="../../../../css/style.css">
<style>
  .course-content {{ max-width:860px; margin:0 auto; }}
  .course-content table {{ border-collapse:collapse; width:100%; margin:1rem 0; }}
  .course-content th,
  .course-content td {{ border:1px solid var(--border,#ccc); padding:.4rem .6rem; }}
  .course-content th {{ background:var(--header-bg,#f4f4f4); }}
  .course-content code {{ background:var(--code-bg,#f4f4f4); padding:.1em .3em; border-radius:3px; font-size:.9em; }}
  .course-content pre code {{ background:none; padding:0; }}
  .course-content pre {{ background:var(--code-bg,#f4f4f4); padding:1rem; overflow-x:auto; border-radius:4px; }}
  .course-content blockquote {{ border-left:3px solid var(--border,#ccc); margin-left:0; padding-left:1rem; color:var(--muted,#666); }}
  .course-content hr {{ border:none; border-top:2px solid var(--border,#ccc); margin:2rem 0; }}
</style>
</head>
<body>
<header class="site-header">
  <div class="header-inner">
    <span class="site-title">KO6NNH — {cat_label}</span>
    <nav class="main-nav">
      <a href="../../../../index.html">page3 home</a>
      <a href="../../../index.html">study</a>
      <a href="../index.html">{cat_label}</a>
    </nav>
  </div>
</header>
<main class="course-content">
{body}
</main>
<footer class="site-footer">
  <p>KO6NNH Self-Paced OBT — {cat_label}</p>
</footer>
</body>
</html>
"""
    return html


def main():
    written = 0
    for cat_dir in sorted(SELF_PACED.iterdir()):
        if not cat_dir.is_dir() or cat_dir.name not in CAT_LABELS:
            continue
        cat_label = CAT_LABELS[cat_dir.name]
        for course_dir in sorted(cat_dir.iterdir()):
            if not course_dir.is_dir():
                continue
            for md_path in sorted(course_dir.glob('*.md')):
                html = md_to_html(md_path, cat_label)
                html_path = md_path.with_suffix('.html')
                html_path.write_text(html, encoding='utf-8')
                written += 1
        print(f"  {cat_dir.name}: done")
    print(f"Total: {written} HTML files written.")


if __name__ == '__main__':
    main()
