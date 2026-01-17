#!/usr/bin/env python3
import argparse
import os
import re
from datetime import datetime, timezone


GEAR_LIST_GENERIC = "/home/merv/Public/ai/codex_notes/gear_to_be_calibrated.txt"
GEAR_LIST_SPECIFIC = "/home/merv/Public/ai/codex_notes/specific_gear_to_be_calibrated.txt"
CAL_ROOT = "/home/merv/Public/ai/pages/calibration"
SPECIFIC_DETAILS = {
    "frequency standard gpsdo": {
        "models": "Leo Bodnar GPSDO, Trimble Thunderbolt, BG7TBL GPSDO",
        "range": "10 MHz output (typical) + 1PPS time reference",
        "connectors": "BNC or SMA",
        "use": "Provide a stable frequency/time reference for counters and analyzers",
    },
    "nanovna vector network analyzer": {
        "models": "NanoVNA V2/V2 Plus, NanoVNA H4, NanoVNA-H",
        "range": "50 kHz to 3 GHz (varies by model)",
        "connectors": "SMA",
        "use": "Measure antenna impedance/SWR, filters, and RF networks",
    },
    "tinysa spectrum analyzer": {
        "models": "TinySA Ultra, TinySA Basic",
        "range": "100 kHz to 6 GHz (Ultra); 100 kHz to 960 MHz (Basic)",
        "connectors": "SMA",
        "use": "Spectrum analysis, signal hunting, filter checking",
    },
}


def slugify(text):
    text = text.strip().lower()
    text = re.sub(r"[^a-z0-9]+", "-", text)
    return text.strip("-")


def read_gear_list(path):
    items = []
    if not os.path.exists(path):
        return items
    with open(path, "r", encoding="utf-8") as f:
        for raw in f:
            line = raw.strip()
            if not line or line.startswith("#"):
                continue
            items.append(line)
    return items


def write_file(path, content, overwrite):
    if os.path.exists(path) and not overwrite:
        return
    with open(path, "w", encoding="utf-8") as f:
        f.write(content)
    return True


def md_header(title):
    return f"# {title}\n\n"


def build_profile_section(details):
    return f"""\
## Gear Profile

- Example models: {details["models"]}
- Frequency range: {details["range"]}
- Connector type: {details["connectors"]}
- Typical use: {details["use"]}
"""


def build_generic_details(gear_name):
    name = gear_name.lower()
    if "antenna analyzer" in name:
        return {
            "models": "MFJ-259, RigExpert AA-30, NanoVNA (budget option)",
            "range": "1 to 30 MHz typical (some cover VHF/UHF)",
            "connectors": "SO-239, BNC, or SMA (varies)",
            "use": "Measure SWR, impedance, and resonance of antennas",
        }
    if "field strength" in name:
        return {
            "models": "Basic field strength meter, RF probe + DVM",
            "range": "HF/VHF/UHF depending on probe and detector",
            "connectors": "Probe lead or BNC/SMA adapter",
            "use": "Relative signal strength comparisons and pattern checks",
        }
    return {
        "models": "Generic bench or handheld units",
        "range": "Varies by model; verify datasheet",
        "connectors": "BNC, SMA, or binding posts",
        "use": "General measurement and calibration checks",
    }


def build_overview_md(gear_name, details):
    profile = build_profile_section(details)
    return md_header(f"{gear_name.title()} Calibration Overview") + f"""\
{profile}
## Purpose

This guide describes a **homebrew, no-calibration-needed** method to calibrate a {gear_name} using
public reference signals and first-principles checks. It borrows techniques from:

- `../gps_calibration.md`
- `../fm_broadcast_calibration.md`
- `../radio_standard_calibration.md`
- `../verification_procedures.md`

## What You Will Build

- A simple reference load (homebrew 50 ohm)
- A quarter-wave coax stub test fixture
- Optional GPS 1PPS counter (if you want tighter frequency checks)

## Expected Accuracy

- Frequency axis: 0.1 to 1 ppm (with GPS or time standards)
- SWR/impedance: limited by resistor tolerance (typically 1-2%)
"""


def build_theory_md(gear_name):
    return md_header(f"{gear_name.title()} Theory of Operation") + """\
## Calibration Philosophy

We use **absolute references** (GPS 1PPS, WWV/CHU, FM stations) and **known physics**
(speed of light, coax velocity factor, resonance) to validate the analyzer without relying on
any pre-calibrated lab equipment.

## Key Principles

1. **Frequency accuracy**: Check displayed frequency against GPS/WWV/CHU/FM carriers.
2. **Impedance accuracy**: Validate using a known load and known reactance from a stub or LC.
3. **Repeatability**: Measurements should be stable over time and temperature.

## Reference Sources

- **GPS 1PPS**: Atomic time tick (best accuracy).
- **WWV/CHU**: HF time standards (good accuracy).
- **FM broadcast**: Convenient local reference (moderate accuracy).
"""


def build_calculations_md(gear_name):
    return md_header(f"{gear_name.title()} Calculations") + """\
## Quarter-Wave Coax Stub

Use a shorted coax stub to create a known resonance:

```
L = (c / (4 * f)) * VF
```

Where:
- `L` = stub length (meters)
- `c` = 299,792,458 m/s
- `f` = frequency (Hz)
- `VF` = velocity factor of coax (e.g., 0.66 solid PE, 0.78 foam)

### Example

Target frequency: 14.200 MHz

```
L = (299,792,458 / (4 * 14,200,000)) * 0.66
L = 3.49 m
```

Cut slightly long, then trim while watching the analyzer until resonance hits target.

## 50 ohm Load (Homebrew)

Parallel resistor network:

```
R_total = 1 / (1/R1 + 1/R2 + ... + 1/Rn)
```

### Example

Four 200 ohm resistors in parallel:

```
R_total = 1 / (4/200) = 50 ohms
```

Use 1% or 0.1% resistors if possible.
"""


def build_construction_md(gear_name):
    return md_header(f"{gear_name.title()} Construction Methods") + """\
## Build a 50 ohm Load

- Use four 200 ohm, 1% resistors in parallel.
- Solder directly inside a BNC/SMA connector shell if possible.
- Keep leads short to reduce inductance.

## Build a Quarter-Wave Stub

- Choose coax with known velocity factor.
- Cut to calculated length (see Calculations).
- Short the far end (center to shield).
- Label the stub with its target frequency and VF.

## Optional GPS 1PPS Interface

- GPS module with 1PPS output.
- LED + resistor for lock indication.
- Optional ESP32/CYD counter (see optional code example).
"""


def build_assembly_md(gear_name):
    return md_header(f"{gear_name.title()} Assembly Procedures") + """\
## Assembly Steps

1. Build or verify the 50 ohm load.
2. Build one or more quarter-wave stubs (pick key bands).
3. Prepare short, known-good test leads.
4. Warm up the analyzer (10-15 minutes) before calibration checks.
"""


def build_tuning_md(gear_name):
    return md_header(f"{gear_name.title()} Tuning Methods") + """\
## Frequency Axis Check

- Measure WWV/CHU or FM carrier frequency.
- Compare to published frequency.
- If your analyzer supports frequency offset adjustment, apply correction.

## Impedance/SWR Check

- Connect the 50 ohm load.
- The analyzer should read close to 50 ohm and low SWR.
- Connect the shorted stub and locate the resonance dip; compare to expected.

## Trim and Iterate

- If stub resonance is off, trim length in small increments.
- Re-check until resonance aligns within expected tolerance.
"""


def build_calibration_md(gear_name):
    return md_header(f"{gear_name.title()} Calibration Procedure") + """\
## Step-by-Step

1. **Warm up** the analyzer for 10-15 minutes.
2. **Frequency check** using WWV/CHU or FM station:
   - Measure known carrier.
   - Calculate ppm error.
   - Apply correction if supported.
3. **Impedance check** using the 50 ohm load:
   - Confirm 45-55 ohm range.
   - Record SWR.
4. **Reactive check** using the shorted stub:
   - Find resonance dip.
   - Compare with calculated frequency.
5. **Document** the offsets and repeatability.
"""


def build_verification_md(gear_name):
    return md_header(f"{gear_name.title()} Verification") + """\
## Verification Checklist

- Re-measure the reference carrier after calibration.
- Measure a second independent reference (FM vs WWV/CHU).
- Confirm measurements are stable across 3-5 repetitions.

## Acceptance Targets

- Frequency accuracy: within 0.1-1 ppm of reference.
- Load accuracy: within 1-2% of 50 ohm.
"""


def build_example_md(gear_name):
    return md_header(f"{gear_name.title()} Example Results") + """\
## Example Log

```
Date: 2026-01-15
Gear: Example Analyzer

Reference: WWV 15 MHz
Measured: 15,000,012 Hz
Error: +0.8 ppm

Load Test: 50 ohm load
Measured: 50.9 ohm
SWR: 1.02

Stub Test (14.200 MHz target)
Measured dip: 14.198 MHz
Error: -0.14%
```
"""


def build_cyd_md(gear_name):
    return md_header("Optional ESP32/CYD Helper (If Used)") + """\
## When to Use

Use a Cheap Yellow Display (ESP32-2432S028R) if you want a **standalone frequency counter**
to check GPS 1PPS or an RF divider output.

## Example ESP32 Code

See `cyd_frequency_counter.ino` in this folder.
"""


def build_cyd_ino():
    return """\
// ESP32/CYD simple pulse counter for GPS 1PPS or divided RF.
// Displays counts per second over Serial. Adapt display code as needed.

volatile uint32_t pulse_count = 0;

void IRAM_ATTR onPulse() {
  pulse_count++;
}

void setup() {
  Serial.begin(115200);
  pinMode(25, INPUT_PULLUP); // Connect PPS or divider output here
  attachInterrupt(digitalPinToInterrupt(25), onPulse, RISING);
}

void loop() {
  uint32_t start = millis();
  pulse_count = 0;
  while (millis() - start < 1000) {
    // count for 1 second
    delay(1);
  }
  Serial.print("Pulses per second: ");
  Serial.println(pulse_count);
  delay(500);
}
"""


def build_index_html(gear_name, slug):
    return f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{gear_name.title()} Calibration - EnergyGuy LLC</title>
    <link rel="stylesheet" href="../../css/style.css">
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
            <nav>
                <h3>Calibration Navigation</h3>
                <ul>
                    <li><a href="../index.html">Calibration Home</a></li>
                </ul>
                <h3>{gear_name.title()}</h3>
                <ul>
                    <li><a href="overview.md">Overview</a></li>
                    <li><a href="theory_of_operation.md">Theory</a></li>
                    <li><a href="calculations.md">Calculations</a></li>
                    <li><a href="construction_methods.md">Construction</a></li>
                    <li><a href="assembly_procedures.md">Assembly</a></li>
                    <li><a href="tuning_methods.md">Tuning</a></li>
                    <li><a href="calibration_procedure.md">Calibration Procedure</a></li>
                    <li><a href="verification.md">Verification</a></li>
                    <li><a href="example_results.md">Example Results</a></li>
                    <li><a href="cyd_esp32_optional.md">Optional ESP32/CYD</a></li>
                </ul>
            </nav>
            <main>
                <h1>{gear_name.title()} Calibration</h1>
                <p>Use the links on the left to read the calibration build notes.</p>
                <p>Generated: {datetime.now(timezone.utc).strftime('%Y-%m-%d')}</p>
            </main>
        </div>
    </div>
</body>
</html>
"""


def generate_for_gear(gear_name, overwrite, verbose, details):
    slug = slugify(gear_name)
    gear_dir = os.path.join(CAL_ROOT, slug)
    os.makedirs(gear_dir, exist_ok=True)

    files = [
        ("overview.md", build_overview_md(gear_name, details)),
        ("theory_of_operation.md", build_theory_md(gear_name)),
        ("calculations.md", build_calculations_md(gear_name)),
        ("construction_methods.md", build_construction_md(gear_name)),
        ("assembly_procedures.md", build_assembly_md(gear_name)),
        ("tuning_methods.md", build_tuning_md(gear_name)),
        ("calibration_procedure.md", build_calibration_md(gear_name)),
        ("verification.md", build_verification_md(gear_name)),
        ("example_results.md", build_example_md(gear_name)),
        ("cyd_esp32_optional.md", build_cyd_md(gear_name)),
        ("cyd_frequency_counter.ino", build_cyd_ino()),
        ("index.html", build_index_html(gear_name, slug)),
    ]
    for name, content in files:
        path = os.path.join(gear_dir, name)
        wrote = write_file(path, content, overwrite)
        if verbose and wrote:
            print(f"Wrote {path}")
        elif verbose:
            print(f"Skipped {path} (exists)")

    return slug


def update_calibration_nav(slugs, names):
    index_path = os.path.join(CAL_ROOT, "index.html")
    with open(index_path, "r", encoding="utf-8", errors="ignore") as f:
        text = f.read()

    nav_block = "<h3>Calibration Equipment</h3>"
    if nav_block not in text:
        return

    pattern = re.compile(r"(<h3>Calibration Equipment</h3>\\s*<ul>)(.*?)(</ul>)", re.S)
    match = pattern.search(text)
    if not match:
        return
    before, body, after = match.groups()
    insert_items = []
    for slug, name in zip(slugs, names):
        if f'href=\"{slug}/index.html\"' in body:
            continue
        insert_items.append(f'                    <li><a href=\"{slug}/index.html\">{name.title()}</a></li>')
    if not insert_items:
        return
    body = body + "\\n" + "\\n".join(insert_items)
    text = text[:match.start()] + before + body + after + text[match.end():]

    with open(index_path, "w", encoding="utf-8") as f:
        f.write(text)


def main():
    parser = argparse.ArgumentParser(description="Generate calibration pages for gear list.")
    parser.add_argument("--overwrite", action="store_true", help="Overwrite existing files.")
    parser.add_argument("--verbose", action="store_true", help="Print files as they are written.")
    args = parser.parse_args()

    specific_list = read_gear_list(GEAR_LIST_SPECIFIC)
    generic_list = read_gear_list(GEAR_LIST_GENERIC)
    specific_lookup = {item.lower() for item in specific_list}
    gear_list = list(specific_list)
    for item in generic_list:
        if item.lower() not in specific_lookup:
            gear_list.append(item)
    if not gear_list:
        raise SystemExit("No gear found in gear_to_be_calibrated.txt")

    slugs = []
    for gear in gear_list:
        key = gear.lower()
        details = SPECIFIC_DETAILS.get(key) or build_generic_details(gear)
        slug = generate_for_gear(gear, args.overwrite, args.verbose, details)
        slugs.append(slug)

    update_calibration_nav(slugs, gear_list)


if __name__ == "__main__":
    main()
