#!/usr/bin/env python3
"""
Add Navy TM document headers to antenna spec files that lack them.
Converts: # ANTENNA_NAME / ## Technical Specification and Fabrication Guide
To: # ANTENNA_NAME / ## TECHNICAL DATA SHEET + TM document header
"""

import os
import re
import sys

BASE_DIR = '/home/merv/Public/claude/ant_t_r'
DATE = '2026-05-24'

# Files and directories to skip (already have proper TM headers or are not data sheets)
SKIP_NAMES = {
    'COMPACT_DIPOLE_MANUAL.md',
    'EFQW_ANTENNA_MANUAL.md',
    'SCREWDRIVER_ANTENNA_MANUAL.md',
    'WEBBING_JPOLE_MANUAL.md',
    'POOL_NOODLE_LOOP_MANUAL.md',
    'POOL_NOODLE_ANTENNA_MANUAL.md',
    'FIELD_TESTING_INTEGRATION_TEMPLATE.md',
    'FIELD_TESTING_QUICK_REFERENCE.md',
    'FIELD_TESTING_WITH_INSTRUMENTS.md',
    'FIELD_TESTING_WITH_MULTIMETER_AND_SCOPE.md',
    'FIELD_TEST_EXAMPLE_40M_COMPACT_DIPOLE.md',
    'FOUR_INSTRUMENT_FIELD_TEST_GUIDE.md',
    'ANTENNA_SUITE_MANIFEST.md',
    'MISSING_ANTENNA_TYPES_ANALYSIS.md',
    'RECOMMENDED_ADDITIONAL_TESTING_EQUIPMENT.md',
    'INDEX.md',
    'dipole_antenna_guide.md',
    'j-pole_grok.md',
    'yagi_grok.md',
    'slim_jim.md',
    'pvc_copper_tape_antenna.md',
    'pvc_copper_tape_webbing_antenna.md',
    'helical_pvc_copper_tape_webbing_antenna.md',
    'helical_pvc_copper_tape_webbing_antenna_tuning.md',
    'helical_pvc_copper_tape_webbing_antenna_tuning_math.md',
    'web_foil_antennas.md',
    '5.6M_telescopic_whip.md',
    'DELIVERABLE_SUMMARY.txt',
}

TM_HEADER_MARKER = 'Document Number:'

def tm_number_from_path(filepath):
    """Derive TM document number from file path."""
    parts = filepath.replace(BASE_DIR, '').strip('/').split('/')
    # Use parent dir + filename stem
    if len(parts) >= 2:
        parent = parts[-2].upper()[:12]
    else:
        parent = parts[0].upper()[:12]
    # Clean to alphanumeric + hyphen
    parent = re.sub(r'[^A-Z0-9]', '-', parent).strip('-')
    return f'TM-{parent}-001'

def already_has_tm(content):
    return TM_HEADER_MARKER in content

def add_tm_header(filepath, content):
    lines = content.split('\n')

    # Extract title from first line (# TITLE)
    title = ''
    for line in lines:
        if line.startswith('# '):
            title = line[2:].strip()
            break

    tm_num = tm_number_from_path(filepath)

    # Build new header block
    header = f"""# {title}
## TECHNICAL DATA SHEET

**Document Number:** {tm_num} Rev A
**Equipment:** {title}
**Classification:** UNCLASSIFIED — Amateur Radio / Field Use
**Date:** {DATE}
**Supersedes:** None (initial issue)

---

## RECORD OF CHANGES

| Change No. | Rev | Date | Description | By |
|:---:|:---:|:---:|---|:---:|
| 1 | A | {DATE} | Initial formatted release | M. Martin |

---

"""

    # Remove original title line and "## Technical Specification..." subtitle
    out_lines = []
    skip_next_blank = False
    found_title = False
    found_subtitle = False

    for i, line in enumerate(lines):
        if not found_title and line.startswith('# '):
            found_title = True
            continue  # Skip original title
        if found_title and not found_subtitle and line.startswith('## ') and 'technical' in line.lower():
            found_subtitle = True
            continue  # Skip "Technical Specification and Fabrication Guide"
        if found_title and not found_subtitle and line.strip() == '---':
            found_subtitle = True  # Treat --- after title as end of original header
            continue
        if found_title and not found_subtitle and line.strip() == '':
            continue  # Skip blank lines after title before subtitle
        out_lines.append(line)

    # Convert SECTION headings to CHAPTER
    final_lines = []
    for line in out_lines:
        # "## SECTION I" → "## CHAPTER 1" etc (Roman numerals)
        m = re.match(r'^(#{1,3})\s+SECTION\s+([IVX]+|[0-9]+)\s*[—–-]?\s*(.*)', line)
        if m:
            hashes = m.group(1)
            num = m.group(2)
            rest = m.group(3)
            # Convert Roman numerals to Arabic
            roman_map = {'I': '1', 'II': '2', 'III': '3', 'IV': '4', 'V': '5',
                        'VI': '6', 'VII': '7', 'VIII': '8', 'IX': '9', 'X': '10'}
            arabic = roman_map.get(num, num)
            sep = ' — ' if rest else ''
            line = f'{hashes} CHAPTER {arabic}{sep}{rest}'
        final_lines.append(line)

    return header + '\n'.join(final_lines)

def process_file(filepath):
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()

    if already_has_tm(content):
        return False  # Already has TM header

    new_content = add_tm_header(filepath, content)

    with open(filepath, 'w', encoding='utf-8') as f:
        f.write(new_content)

    return True

def main():
    updated = []
    skipped = []

    for root, dirs, files in os.walk(BASE_DIR):
        # Skip hidden dirs
        dirs[:] = [d for d in dirs if not d.startswith('.')]

        for fname in files:
            if not fname.endswith('.md'):
                continue
            if fname in SKIP_NAMES:
                continue

            filepath = os.path.join(root, fname)

            try:
                result = process_file(filepath)
                if result:
                    rel = filepath.replace(BASE_DIR + '/', '')
                    updated.append(rel)
                else:
                    rel = filepath.replace(BASE_DIR + '/', '')
                    skipped.append(rel)
            except Exception as e:
                print(f'ERROR: {filepath}: {e}', file=sys.stderr)

    print(f'Updated {len(updated)} files:')
    for f in sorted(updated):
        print(f'  + {f}')

    print(f'\nSkipped {len(skipped)} files (already have TM headers):')
    for f in sorted(skipped):
        print(f'  = {f}')

if __name__ == '__main__':
    main()
