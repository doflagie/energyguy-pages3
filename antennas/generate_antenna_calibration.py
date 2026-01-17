#!/usr/bin/env python3
import os
import re
from pathlib import Path

import markdown


ROOT = "/home/merv/Public/ai/antennas"
LIST_PATH = "/home/merv/Public/ai/codex_notes/antennas.txt"
CSS_PATH = "/home/merv/Public/ai/pages/css/style.css"

CYD_GEAR = {
    "swr meter",
    "rf power meter",
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
    return slug or "antenna"


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

This guide outlines a cheap, homebrew method to calibrate or verify a {name} without any pre-calibrated equipment. It relies on known physics (wavelength, impedance, resonance) and public references (WWV, GPS, broadcast carriers).

## What you will build
- A simple reference or fixture based on known dimensions or components.
- A repeatable setup for measurements and comparisons.
"""
    if section == "theory_of_operation":
        return f"""# Theory of Operation

Calibration compares the {name} response to a known reference. For antennas, the reference comes from physical dimensions (wavelength), known impedance standards, and repeatable measurement setups. For supporting gear, the reference comes from basic circuit physics and stable sources.
"""
    if section == "calculations":
        return f"""# Calculations

## Wavelength relationships
- Wavelength (meters) = 300 / Frequency (MHz)
- Quarter-wave length = Wavelength / 4
- Half-wave length = Wavelength / 2

## Impedance
- V = I * R
- Power (W) = V^2 / R

## Example
If target frequency is 7.1 MHz:
- Wavelength = 300 / 7.1 = 42.25 m
- Half-wave = 21.1 m
"""
    if section == "construction_methods":
        return f"""# Construction Methods

1. Use measured cuts based on wavelength formulas.
2. Use common copper wire or tubing.
3. Include a feedpoint structure that allows adjustment.
4. Use a simple support structure to keep geometry consistent.
"""
    if section == "assembly_procedures":
        return f"""# Assembly Procedures

1. Cut elements to calculated length.
2. Assemble the feedpoint with a connector and strain relief.
3. Mount the structure in its intended orientation.
4. Verify continuity and no shorts before measurements.
"""
    if section == "tuning_methods":
        return f"""# Tuning Methods

1. Sweep across the target band.
2. Shorten or lengthen elements in small steps.
3. Record resonance shift per adjustment.
4. Lock the final configuration.
"""
    if section == "calibration_procedure":
        return f"""# Calibration Procedure

1. Use a stable reference signal (WWV, beacon, or known local carrier).
2. Measure resonance, SWR, or gain pattern.
3. Compare against expected values from calculations.
4. Apply corrections or document offsets.
"""
    if section == "verification":
        return f"""# Verification

Confirm performance at several frequencies or positions. Keep a log of environment, height, and orientation so future checks are comparable.
"""
    if section == "example_results":
        return f"""# Example Results

| Target | Measured | Offset | Notes |
|------:|---------:|------:|------|
| 7.100 MHz | 7.080 MHz | -20 kHz | Length reduced by 3 cm |
| 14.200 MHz | 14.210 MHz | +10 kHz | Good match |
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
    body = f"""# Antenna Calibration Library

<p class="intro-text">Homebrew calibration guides using basic physics and NIST-traceable references where applicable.</p>

<ul>
{os.linesep.join('                ' + item for item in items)}
</ul>
"""
    html = wrap_html(
        "Antenna Calibration Library",
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
