#!/usr/bin/env python3
import os
import re
from pathlib import Path

import markdown


ROOT = "/home/merv/Public/ai/NIST"
CSS_PATH = "/home/merv/Public/ai/pages/css/style.css"


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


METHODS = [
    ("WWV/WWVH Time Signal Reference", "wwv_wwvh_reference"),
    ("GPS 1PPS Timing Reference", "gps_1pps_reference"),
    ("GPS-Disciplined Oscillator (GPSDO)", "gpsdo_reference"),
    ("FM Broadcast Carrier Reference", "fm_broadcast_reference"),
    ("AC Mains Frequency Reference", "ac_mains_reference"),
    ("Precision Voltage Reference (LM4040/LT1021)", "precision_voltage_reference"),
    ("Zener + Divider Multi-Voltage Reference", "zener_divider_reference"),
    ("Precision Resistor Network (Decade Box)", "precision_resistor_network"),
    ("Wheatstone Bridge Null Method", "wheatstone_bridge"),
    ("4-Wire Kelvin Resistance Standard", "kelvin_resistance_standard"),
    ("RC Time-Constant Standard (GPS-Referenced)", "rc_time_constant_standard"),
    ("LC Resonance Standard (Frequency-Referenced)", "lc_resonance_standard"),
    ("Coax Cable Capacitance Standard", "coax_capacitance_standard"),
    ("50-Ohm Dummy Load Power Standard", "dummy_load_power_standard"),
    ("Fixed Attenuator Pad Standards", "attenuator_pad_standards"),
    ("Known-Length Dipole Antenna Standard", "known_length_dipole_standard"),
    ("Field-Strength Reference Setup", "field_strength_reference"),
]


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


def section_text(title, section):
    if section == "overview":
        return f"""# {title}

This method provides a cheap, homebrew calibration standard that is traceable to NIST through public references or physics-based ratios. It is intended for building confidence in measurements without pre-calibrated equipment.
"""
    if section == "theory_of_operation":
        return f"""# Theory of Operation

The {title} method compares your instrument to a stable reference. Traceability is obtained by tying the reference to NIST time/frequency signals, published standards, or fundamental physics (ratios, resonance, or known constants).
"""
    if section == "calculations":
        return """# Calculations

## Frequency
- Error (ppm) = ((measured - expected) / expected) * 1,000,000

## Voltage and current
- V = I * R
- P = V^2 / R

## Reactance
- Xc = 1 / (2 * pi * f * C)
- Xl = 2 * pi * f * L
"""
    if section == "construction_methods":
        return """# Construction Methods

Build the reference using inexpensive parts and short, stable wiring. Keep leads short, use shielding where needed, and label test points.
"""
    if section == "assembly_procedures":
        return """# Assembly Procedures

1. Collect components and verify values with basic checks.
2. Assemble on perfboard or a small enclosure.
3. Add clear input/output connectors and ground points.
4. Power up and confirm stable operation.
"""
    if section == "tuning_methods":
        return """# Tuning Methods

1. Allow warm-up time for stability.
2. Adjust trimmers or settings to align with the reference.
3. Record the final settings and any correction factors.
"""
    if section == "calibration_procedure":
        return """# Calibration Procedure

1. Connect your instrument to the reference.
2. Measure at multiple points across the range.
3. Calculate error and apply corrections or record offsets.
"""
    if section == "verification":
        return """# Verification

Verify at a few independent points not used during calibration. Record results in a log for repeatability.
"""
    if section == "example_results":
        return """# Example Results

| Expected | Measured | Error | Notes |
|---------:|---------:|------:|------|
| 10.000 MHz | 9.999800 MHz | -20 ppm | Corrected |
| 5.000 V | 4.995 V | -0.1% | Acceptable |
"""
    return f"# {section.replace('_', ' ').title()}\n"


def method_specific_overrides(title):
    overrides = {}
    if title == "WWV/WWVH Time Signal Reference":
        overrides["overview"] = """# WWV/WWVH Time Signal Reference

Use NIST time signals (WWV/WWVH) as a frequency and time standard. These carriers are traceable to NIST by definition and are widely available on HF.
"""
        overrides["calibration_procedure"] = """# Calibration Procedure

1. Receive WWV/WWVH on 2.5, 5, 10, 15, or 20 MHz.
2. Zero-beat your oscillator or compare SDR FFT peak to WWV.
3. Compute frequency error in ppm.
"""
    if title == "GPS 1PPS Timing Reference":
        overrides["overview"] = """# GPS 1PPS Timing Reference

GPS 1PPS outputs are traceable to UTC and NIST. Use the pulse to calibrate timebase and frequency counters.
"""
        overrides["calculations"] = """# Calculations

## Timebase error
- Error (ppm) = ((measured_period - 1.000000 s) / 1.000000 s) * 1,000,000
"""
    if title == "GPS-Disciplined Oscillator (GPSDO)":
        overrides["overview"] = """# GPS-Disciplined Oscillator (GPSDO)

Lock a TCXO/OCXO to GPS 1PPS to create a 10 MHz reference with NIST traceability.
"""
    if title == "FM Broadcast Carrier Reference":
        overrides["overview"] = """# FM Broadcast Carrier Reference

Use the center carrier of a licensed FM station as a practical frequency reference. It is regulated and traceable through FCC requirements.
"""
    if title == "AC Mains Frequency Reference":
        overrides["overview"] = """# AC Mains Frequency Reference

Use line frequency as a low-frequency reference. Short-term accuracy is limited, but long-term averages are stable.
"""
    if title == "Precision Voltage Reference (LM4040/LT1021)":
        overrides["overview"] = """# Precision Voltage Reference (LM4040/LT1021)

Build a stable 5 V or 10 V reference using a precision reference IC. Ratio measurements provide traceability without a calibrated meter.
"""
    if title == "Zener + Divider Multi-Voltage Reference":
        overrides["overview"] = """# Zener + Divider Multi-Voltage Reference

Use a stable zener plus a resistor divider network to generate multiple reference voltages with known ratios.
"""
    if title == "Precision Resistor Network (Decade Box)":
        overrides["overview"] = """# Precision Resistor Network (Decade Box)

Build a resistor network from 0.1% parts. Ratio measurements provide a traceable standard.
"""
    if title == "Wheatstone Bridge Null Method":
        overrides["overview"] = """# Wheatstone Bridge Null Method

Null measurements reduce meter accuracy requirements. Use ratio balance to compare unknowns to known standards.
"""
    if title == "4-Wire Kelvin Resistance Standard":
        overrides["overview"] = """# 4-Wire Kelvin Resistance Standard

Use a known length of wire or resistor with 4-wire connections to build a repeatable resistance standard.
"""
    if title == "RC Time-Constant Standard (GPS-Referenced)":
        overrides["overview"] = """# RC Time-Constant Standard (GPS-Referenced)

Use a GPS 1PPS pulse and an RC circuit to compute C or R from the time constant.
"""
    if title == "LC Resonance Standard (Frequency-Referenced)":
        overrides["overview"] = """# LC Resonance Standard (Frequency-Referenced)

Measure resonant frequency with a GPS- or WWV-referenced counter and solve for L or C.
"""
    if title == "Coax Cable Capacitance Standard":
        overrides["overview"] = """# Coax Cable Capacitance Standard

Use published capacitance-per-length specs to create a capacitance reference from a measured length of coax.
"""
    if title == "50-Ohm Dummy Load Power Standard":
        overrides["overview"] = """# 50-Ohm Dummy Load Power Standard

Build a 50-ohm load from precision resistors and compute RF power from measured voltage.
"""
    if title == "Fixed Attenuator Pad Standards":
        overrides["overview"] = """# Fixed Attenuator Pad Standards

Build fixed pads using precision resistors. Verify attenuation by ratio using known signal levels.
"""
    if title == "Known-Length Dipole Antenna Standard":
        overrides["overview"] = """# Known-Length Dipole Antenna Standard

Use wavelength-based geometry to create a known resonance point for antenna measurements.
"""
    if title == "Field-Strength Reference Setup":
        overrides["overview"] = """# Field-Strength Reference Setup

Use a fixed transmitter power, known antenna, and consistent geometry to create a repeatable field-strength reference.
"""
    return overrides


def build_method(title, slug):
    method_dir = os.path.join(ROOT, slug)
    os.makedirs(method_dir, exist_ok=True)

    overrides = method_specific_overrides(title)
    for section in SECTIONS:
        md_path = os.path.join(method_dir, f"{section}.md")
        content = overrides.get(section, section_text(title, section))
        write_md(md_path, content)
        write_html_from_md(md_path)

    links = []
    for section in SECTIONS:
        links.append(f'<li><a href="{section}.html">{section.replace("_", " ").title()}</a></li>')

    index_body = f"""# {title}

<ul>
{os.linesep.join('                ' + item for item in links)}
</ul>
"""
    index_html = wrap_html(
        title,
        rel_css_path(method_dir),
        render_markdown(index_body),
    )
    with open(os.path.join(method_dir, "index.html"), "w", encoding="utf-8") as f:
        f.write(index_html)


def build_root_index():
    items = []
    for title, slug in METHODS:
        items.append(f'<li><a href="{slug}/index.html">{title}</a></li>')
    body = f"""# NIST-Traceable Calibration Standards

<p class="intro-text">Cheap, homebrew standards traceable to NIST through public references and fundamental physics.</p>

<ul>
{os.linesep.join('                ' + item for item in items)}
</ul>
"""
    html = wrap_html(
        "NIST-Traceable Calibration Standards",
        os.path.relpath(CSS_PATH, ROOT).replace(os.sep, "/"),
        render_markdown(body),
    )
    with open(os.path.join(ROOT, "index.html"), "w", encoding="utf-8") as f:
        f.write(html)


def main():
    os.makedirs(ROOT, exist_ok=True)
    for title, slug in METHODS:
        build_method(title, slug)
    build_root_index()


if __name__ == "__main__":
    main()
