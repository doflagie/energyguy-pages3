#!/usr/bin/env python3
import os
import re
from pathlib import Path

import markdown


ROOT = "/home/merv/Public/ai/test_gear"
LIST_PATH = "/home/merv/Public/ai/codex_notes/test gear.txt"
CSS_PATH = "/home/merv/Public/ai/pages/css/style.css"


CYD_GEAR = {
    "frequency counter",
    "rf power meter",
    "swr meter",
    "field strength meter",
    "modulation analyzer",
}


SECTIONS = [
    "overview",
    "theory_of_operation",
    "calculations",
    "construction_methods",
    "assembly_procedures",
    "tuning_methods",
    "calibration_procedure",
    "verification",
    "example_results",
]


def slugify(name):
    slug = re.sub(r"[^a-z0-9]+", "-", name.lower()).strip("-")
    return slug or "gear"


def rel_css_path(target_dir):
    return os.path.relpath(CSS_PATH, target_dir).replace(os.sep, "/")


def wrap_html(title, css_href, body_html):
    return f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{title} - EnergyGuy LLC</title>
    <link rel="stylesheet" href="{css_href}">
</head>
<body>
    <div class="container">
        <header>
            <div class="banner">
                <div>
                    <h1>EnergyGuy LLC</h1>
                    <p class="tagline">Amateur Radio • Electronics • Engineering</p>
                </div>
            </div>
        </header>
        <div class="main-wrapper">
            <main>
{body_html}
            </main>
        </div>
    </div>
</body>
</html>
"""


def render_markdown(md_text):
    body = markdown.markdown(md_text, extensions=["tables", "fenced_code", "toc"])
    return "\n".join("                " + line for line in body.splitlines())


def write_md(path, text):
    with open(path, "w", encoding="utf-8") as f:
        f.write(text)


def write_html_from_md(md_path):
    with open(md_path, "r", encoding="utf-8", errors="ignore") as f:
        md_text = f.read()
    title = Path(md_path).stem.replace("_", " ").title()
    body_html = render_markdown(md_text)
    html = wrap_html(title, rel_css_path(os.path.dirname(md_path)), body_html)
    html_path = os.path.splitext(md_path)[0] + ".html"
    with open(html_path, "w", encoding="utf-8") as f:
        f.write(html)


def build_section_text(gear_name, section):
    name = gear_name
    if section == "overview":
        return f"""# {name} Calibration (Homebrew)

This guide shows a cheap, homebrew way to calibrate a {name} without any pre-calibrated equipment. It relies on public references (WWV, GPS, AC mains, broadcast carriers) and basic physics.

## What you will build
- One or more simple reference sources (frequency, amplitude, impedance, or level).
- A repeatable procedure to compare your instrument to the reference.

## Required references
- WWV/WWVH time signals (frequency reference)
- GPS 1PPS (timing reference)
- AC mains (low-frequency reference)
- Simple known components (resistors, capacitors, inductors)
"""
    if section == "theory_of_operation":
        return f"""# Theory of Operation

The {name} measures a physical property (frequency, amplitude, impedance, or modulation). Calibration is a comparison between the instrument reading and a reference derived from physics or known component values. The key is repeatability: a stable source and a stable measurement setup.

## Core idea
- Convert the measurement into a known value using a reference.
- Quantify the error as a ratio or offset.
- Apply correction or record a correction table.
"""
    if section == "calculations":
        return f"""# Calculations

Use these formulas to compute expected values and compare them to measured values.

## Frequency reference
- Error (ppm) = ((measured - expected) / expected) * 1,000,000

## Voltage and current references
- V = I * R
- I = V / R

## Reactance for capacitors and inductors
- Xc = 1 / (2 * pi * f * C)
- Xl = 2 * pi * f * L

## Example
If a 10 MHz reference is measured as 9,999,800 Hz:
- Error = (9,999,800 - 10,000,000) / 10,000,000
- Error = -20 ppm
"""
    if section == "construction_methods":
        return f"""# Construction Methods

Build simple fixtures from inexpensive parts.

## Example fixtures
- Frequency reference: GPS module 1PPS and a small divider or counter.
- Amplitude reference: precision resistor divider and a stable supply.
- Impedance reference: known resistors/capacitors or a simple bridge.

## Notes
- Use short leads and good grounding.
- Shield sensitive circuits when possible.
"""
    if section == "assembly_procedures":
        return f"""# Assembly Procedures

1. Collect components and verify values with basic measurements.
2. Assemble the reference fixture on perfboard.
3. Label test points and verify connections.
4. Power up and confirm stable operation.
"""
    if section == "tuning_methods":
        return f"""# Tuning Methods

1. Set the reference to the target range (frequency or level).
2. Allow warm-up time for stabilization.
3. Adjust trimmers or settings for minimum error.
4. Record the final settings.
"""
    if section == "calibration_procedure":
        return f"""# Calibration Procedure

1. Connect the reference to the {name}.
2. Measure at several points across the range.
3. Record the deviation and compute correction values.
4. Apply corrections (if the instrument allows) or record a correction table.
"""
    if section == "verification":
        return f"""# Verification

After calibration, verify the readings at a few points not used in calibration. If the error is consistent and within tolerance, calibration is complete.
"""
    if section == "example_results":
        return f"""# Example Results

## Example calibration table

| Expected | Measured | Error | Correction |
|---------:|---------:|------:|-----------:|
| 10.000 MHz | 9.999800 MHz | -20 ppm | +20 ppm |
| 14.000 MHz | 13.999650 MHz | -25 ppm | +25 ppm |
"""
    return f"# {section.replace('_', ' ').title()}\n"


def cyd_text(gear_name):
    return f"""# Optional ESP32/CYD Helper

Some {gear_name} calibrations are easier with a Cheap Yellow Display (ESP32-2432S028R). This sketch counts pulses and shows a stable readout for comparisons.

```cpp
#include <Arduino.h>

const int inputPin = 34;  // frequency input
volatile uint32_t edges = 0;

void IRAM_ATTR onEdge() {{
  edges++;
}}

void setup() {{
  Serial.begin(115200);
  pinMode(inputPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(inputPin), onEdge, RISING);
}}

void loop() {{
  edges = 0;
  uint32_t start = millis();
  delay(1000);
  uint32_t elapsed = millis() - start;
  uint32_t hz = (edges * 1000UL) / (elapsed ? elapsed : 1);
  Serial.print(\"Freq: \");
  Serial.print(hz);
  Serial.println(\" Hz\");
}}
```
"""


def cyd_ino_text():
    return """#include <Arduino.h>

const int inputPin = 34;  // frequency input
volatile uint32_t edges = 0;

void IRAM_ATTR onEdge() {
  edges++;
}

void setup() {
  Serial.begin(115200);
  pinMode(inputPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(inputPin), onEdge, RISING);
}

void loop() {
  edges = 0;
  uint32_t start = millis();
  delay(1000);
  uint32_t elapsed = millis() - start;
  uint32_t hz = (edges * 1000UL) / (elapsed ? elapsed : 1);
  Serial.print("Freq: ");
  Serial.print(hz);
  Serial.println(" Hz");
}
"""


def build_gear(gear_name):
    slug = slugify(gear_name)
    gear_dir = os.path.join(ROOT, slug)
    os.makedirs(gear_dir, exist_ok=True)

    for section in SECTIONS:
        md_path = os.path.join(gear_dir, f"{section}.md")
        write_md(md_path, build_section_text(gear_name, section))
        write_html_from_md(md_path)

    if gear_name.lower() in CYD_GEAR:
        md_path = os.path.join(gear_dir, "cyd_esp32_optional.md")
        write_md(md_path, cyd_text(gear_name))
        write_html_from_md(md_path)
        ino_path = os.path.join(gear_dir, "cyd_frequency_counter.ino")
        with open(ino_path, "w", encoding="utf-8") as f:
            f.write(cyd_ino_text())

    # index.html
    links = []
    for section in SECTIONS:
        links.append(f'<li><a href="{section}.html">{section.replace("_", " ").title()}</a></li>')
    if gear_name.lower() in CYD_GEAR:
        links.append('<li><a href="cyd_esp32_optional.html">Optional ESP32/CYD</a></li>')

    index_body = f"""# {gear_name} Calibration

<ul>
{os.linesep.join('                ' + item for item in links)}
</ul>
"""
    index_html = wrap_html(
        f"{gear_name} Calibration",
        rel_css_path(gear_dir),
        render_markdown(index_body),
    )
    with open(os.path.join(gear_dir, "index.html"), "w", encoding="utf-8") as f:
        f.write(index_html)


def build_root_index(gear_names):
    items = []
    for name in gear_names:
        slug = slugify(name)
        items.append(f'<li><a href="{slug}/index.html">{name}</a></li>')
    body = f"""# Test Gear Calibration Library

<p class="intro-text">Homebrew calibration guides using NIST-traceable references and basic physics.</p>

<ul>
{os.linesep.join('                ' + item for item in items)}
</ul>
"""
    html = wrap_html(
        "Test Gear Calibration Library",
        os.path.relpath(CSS_PATH, ROOT).replace(os.sep, "/"),
        render_markdown(body),
    )
    with open(os.path.join(ROOT, "index.html"), "w", encoding="utf-8") as f:
        f.write(html)


def main():
    os.makedirs(ROOT, exist_ok=True)
    with open(LIST_PATH, "r", encoding="utf-8", errors="ignore") as f:
        gear_names = [line.strip() for line in f if line.strip()]
    for name in gear_names:
        build_gear(name)
    build_root_index(gear_names)


if __name__ == "__main__":
    main()
