#!/usr/bin/env python3
"""
Generate Navy TM-format HTML technical manuals for NIST-traceable calibration standards.
Output: tm.html in each standard's subdirectory.
"""

import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))

NAV = """
        <nav>
            <h3>Main Navigation</h3>
            <ul>
                <li><a href="/">Home</a></li>
                <li><a href="../../about/bio.html">About</a></li>
            </ul>
            <details class="nav-group" open>
                <summary>Antenna Library</summary>
                <ul><li><a href="../../antennas/index.html">Antenna Library</a></li></ul>
            </details>
            <details class="nav-group" open>
                <summary>Test Equipment Library</summary>
                <ul><li><a href="../../test_gear/index.html">Test Equipment Library</a></li></ul>
            </details>
            <h3>Standards</h3>
            <ul>
                <li><a href="../index.html">Calibration Standards</a></li>
            </ul>
            <details class="nav-group" open>
                <summary>Study &amp; Training</summary>
                <ul>
                    <li><a href="https://assets.doflagie.com/study/index.html">Ham Radio Flash Card Quiz</a></li>
                    <li><a href="https://assets.doflagie.com/study/morse.html">Morse Code Practice</a></li>
                </ul>
            </details>
        </nav>
"""

FOOTER = """
    <footer>
        <div class="address-block">
            <h3>Merv's Brain Dump</h3>
            <p>Mervyn Martin (Merv), Proprietor</p>
            <p>Amateur Radio: KO6NNH</p>
            <p>Merced, California 95340</p>
            <div class="contact-info">
                <div class="contact-item">
                    <strong>Email:</strong><br>
                    <a href="mailto:d.ecaudata@gmail.com">d.ecaudata@gmail.com</a>
                </div>
                <div class="contact-item">
                    <strong>Website:</strong><br>
                    <a href="http://www.doflagie.com">www.doflagie.com</a>
                </div>
                <div class="contact-item">
                    <strong>GitHub:</strong><br>
                    <a href="https://github.com/doflagie" target="_blank">github.com/doflagie</a>
                </div>
            </div>
            <div class="copyright">
                <p>&copy; 2026 Merv's Brain Dump. All rights reserved.</p>
                <p>Documentation shared under <a href="https://creativecommons.org/licenses/by-sa/4.0/" target="_blank">CC BY-SA 4.0</a>.</p>
                <p>Amateur Radio | Electronics | Engineering | Knowledge Preservation</p>
            </div>
        </div>
    </footer>
"""

TM_CSS = """
<style>
/* Navy TM Styles */
.tm-cover {
    border: 3px solid #2c5f8d;
    padding: 30px 40px;
    text-align: center;
    margin-bottom: 30px;
    background: #f8f8f4;
}
.tm-classification {
    font-size: 1.1em;
    font-weight: bold;
    color: #006600;
    letter-spacing: 3px;
    margin-bottom: 20px;
}
.tm-number {
    font-family: 'Courier New', monospace;
    font-size: 1.2em;
    font-weight: bold;
    color: #2c5f8d;
    margin-bottom: 10px;
}
.tm-title {
    font-size: 1.8em;
    font-weight: bold;
    color: #1a3a5c;
    text-transform: uppercase;
    letter-spacing: 1px;
    margin: 15px 0 10px;
    line-height: 1.3;
}
.tm-subtitle {
    font-size: 1.0em;
    font-style: italic;
    color: #555;
    margin-bottom: 15px;
}
.tm-meta {
    font-size: 0.9em;
    color: #555;
    margin-top: 20px;
    border-top: 1px solid #aaa;
    padding-top: 15px;
}
.tm-toc {
    background: #f0f4f8;
    border: 1px solid #c0ccd8;
    padding: 20px 30px;
    margin-bottom: 30px;
}
.tm-toc h2 {
    font-size: 1.1em;
    color: #2c5f8d;
    border-bottom: 1px solid #4a7ba7;
    padding-bottom: 8px;
    margin-bottom: 12px;
    text-transform: uppercase;
    letter-spacing: 1px;
}
.tm-toc ol {
    margin: 0;
    padding-left: 25px;
}
.tm-toc li {
    margin: 4px 0;
    font-size: 0.95em;
}
.tm-toc a {
    color: #2c5f8d;
    text-decoration: none;
}
.tm-toc a:hover {
    text-decoration: underline;
}
.chapter-heading {
    background: #2c5f8d;
    color: #ffffff;
    padding: 10px 20px;
    margin-top: 35px;
    margin-bottom: 20px;
    font-size: 1.2em;
    font-weight: bold;
    text-transform: uppercase;
    letter-spacing: 1px;
}
.section-heading {
    color: #1a3a5c;
    border-bottom: 2px solid #4a7ba7;
    padding-bottom: 4px;
    margin-top: 25px;
    margin-bottom: 12px;
    font-size: 1.05em;
}
.subsection-heading {
    color: #2c5f8d;
    margin-top: 18px;
    margin-bottom: 8px;
    font-size: 1.0em;
}
.tm-warning {
    background: #fff0f0;
    border-left: 5px solid #cc0000;
    border-top: 1px solid #cc0000;
    border-bottom: 1px solid #cc0000;
    border-right: 1px solid #cc0000;
    padding: 12px 16px;
    margin: 16px 0;
}
.tm-caution {
    background: #fffbf0;
    border-left: 5px solid #cc7700;
    border-top: 1px solid #cc7700;
    border-bottom: 1px solid #cc7700;
    border-right: 1px solid #cc7700;
    padding: 12px 16px;
    margin: 16px 0;
}
.tm-note {
    background: #f0f5ff;
    border-left: 5px solid #4a7ba7;
    border-top: 1px solid #4a7ba7;
    border-bottom: 1px solid #4a7ba7;
    border-right: 1px solid #4a7ba7;
    padding: 12px 16px;
    margin: 16px 0;
}
.admonition-label {
    font-weight: bold;
    font-size: 0.9em;
    letter-spacing: 2px;
    display: block;
    margin-bottom: 6px;
}
.tm-warning .admonition-label { color: #cc0000; }
.tm-caution .admonition-label { color: #cc7700; }
.tm-note .admonition-label { color: #2c5f8d; }
.tm-table {
    width: 100%;
    border-collapse: collapse;
    margin: 15px 0;
    font-size: 0.95em;
}
.tm-table th {
    background: #4a7ba7;
    color: #ffffff;
    padding: 8px 12px;
    text-align: left;
    font-size: 0.9em;
    text-transform: uppercase;
    letter-spacing: 0.5px;
}
.tm-table td {
    border: 1px solid #c0ccd8;
    padding: 7px 12px;
    vertical-align: top;
}
.tm-table tr:nth-child(even) td {
    background: #f4f7fb;
}
.tm-table caption {
    font-style: italic;
    font-size: 0.9em;
    color: #555;
    margin-bottom: 5px;
    text-align: left;
}
.procedure ol {
    margin: 10px 0 10px 20px;
}
.procedure li {
    margin: 8px 0;
    line-height: 1.5;
}
.formula-block {
    background: #f4f7fb;
    border: 1px solid #c0ccd8;
    padding: 12px 20px;
    margin: 12px 0;
    font-family: 'Courier New', monospace;
    font-size: 0.95em;
}
.formula-block .formula-label {
    font-family: Georgia, serif;
    font-size: 0.85em;
    color: #555;
    font-style: italic;
    display: block;
    margin-bottom: 5px;
}
.back-link {
    margin-top: 30px;
    padding-top: 15px;
    border-top: 1px solid #c0ccd8;
    font-size: 0.9em;
}
</style>
"""


def tm_page(tm_number, title, subtitle, dir_name, chapters_html):
    toc_items = [
        "Chapter 1 &mdash; General Information",
        "Chapter 2 &mdash; Theory of Operation",
        "Chapter 3 &mdash; Materials and Construction",
        "Chapter 4 &mdash; Assembly Procedures",
        "Chapter 5 &mdash; Calibration Procedure",
        "Chapter 6 &mdash; Tuning and Adjustment",
        "Chapter 7 &mdash; Verification",
        "Appendix A &mdash; Calculations and Formulas",
        "Appendix B &mdash; Example Results",
    ]
    toc_html = "\n".join(
        f'            <li><a href="#ch{i+1}">{item}</a></li>'
        for i, item in enumerate(toc_items[:7])
    ) + "\n" + \
    '            <li><a href="#appa">Appendix A &mdash; Calculations and Formulas</a></li>\n' + \
    '            <li><a href="#appb">Appendix B &mdash; Example Results</a></li>'

    return f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{tm_number} {title} - Merv's Brain Dump</title>
    <link rel="stylesheet" href="../../css/style.css">
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
            {NAV}
            <main>
                <div class="tm-cover">
                    <div class="tm-classification">UNCLASSIFIED</div>
                    <div class="tm-number">{tm_number}</div>
                    <div class="tm-title">{title}</div>
                    <div class="tm-subtitle">{subtitle}</div>
                    <div class="tm-meta">
                        Prepared by: Mervyn Martin, KO6NNH<br>
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

                {chapters_html}

                <div class="back-link">
                    <a href="../index.html">&larr; Back to Calibration Standards</a>
                </div>
            </main>
        </div>
    </div>
    {FOOTER}
</body>
</html>
"""


def warning(text):
    return f'<div class="tm-warning"><span class="admonition-label">WARNING</span>{text}</div>'

def caution(text):
    return f'<div class="tm-caution"><span class="admonition-label">CAUTION</span>{text}</div>'

def note(text):
    return f'<div class="tm-note"><span class="admonition-label">NOTE</span>{text}</div>'

def chapter(num, title, anchor, body):
    return f'<h2 class="chapter-heading" id="{anchor}">CHAPTER {num} &mdash; {title}</h2>\n{body}\n'

def section(num, title, body):
    return f'<h3 class="section-heading">{num}. {title}</h3>\n{body}\n'

def subsection(title, body):
    return f'<h4 class="subsection-heading">{title}</h4>\n{body}\n'

def procedure(steps):
    items = "\n".join(f"<li>{s}</li>" for s in steps)
    return f'<div class="procedure"><ol>\n{items}\n</ol></div>'

def table(caption, headers, rows):
    th = "".join(f"<th>{h}</th>" for h in headers)
    trs = "\n".join(
        "<tr>" + "".join(f"<td>{c}</td>" for c in row) + "</tr>"
        for row in rows
    )
    cap = f"<caption>{caption}</caption>" if caption else ""
    return f'<table class="tm-table">{cap}<thead><tr>{th}</tr></thead><tbody>\n{trs}\n</tbody></table>'

def formula(label, expr):
    return f'<div class="formula-block"><span class="formula-label">{label}</span>{expr}</div>'

def ul(items):
    lis = "\n".join(f"<li>{i}</li>" for i in items)
    return f"<ul>\n{lis}\n</ul>"

def p(text):
    return f"<p>{text}</p>\n"

def appchapter(anchor, letter, title, body):
    return f'<h2 class="chapter-heading" id="{anchor}">APPENDIX {letter} &mdash; {title}</h2>\n{body}\n'


# ═══════════════════════════════════════════════════════════════════════════════
# TM-CAL-001: WWV/WWVH Time Signal Reference
# ═══════════════════════════════════════════════════════════════════════════════

def tm_wwv():
    ch1 = chapter(1, "GENERAL INFORMATION", "ch1",
        section("1-1", "SCOPE",
            p("This manual covers the use of NIST shortwave time and frequency radio broadcasts "
              "(WWV and WWVH) as a primary frequency reference for calibrating frequency counters, "
              "receivers, signal generators, and other RF test equipment. The method requires only "
              "a shortwave receiver and is suitable for the amateur radio station or electronics workbench.")) +
        section("1-2", "APPLICABLE REFERENCES",
            ul(["NIST Special Publication 432: NIST Time and Frequency Services",
                "ITU-R TF.460: Standard Frequency and Time Signal Emissions",
                "ARRL Handbook: Frequency and Time Standards",
                "FCC Rules Part 25 &mdash; Satellite Communications"])) +
        section("1-3", "SAFETY PRECAUTIONS",
            note("No hazardous voltages or currents are involved in normal use of this standard. "
                 "All measurements are made at receiver output levels.") +
            note("Antenna installation safety: observe local building codes and maintain safe "
                 "clearance from power lines when installing shortwave antennas."))
    )

    ch2 = chapter(2, "THEORY OF OPERATION", "ch2",
        section("2-1", "NIST TIME AND FREQUENCY BROADCASTS",
            p("NIST operates two shortwave time and frequency stations: WWV at Fort Collins, Colorado, "
              "and WWVH at Kekaha, Hawaii. Both broadcast continuous signals that serve as NIST primary "
              "frequency standards, traceable to the NIST F-2 cesium fountain atomic clock.") +
            p("WWV broadcast frequencies: 2.5, 5.0, 10.0, 15.0, and 20.0 MHz. "
              "WWVH broadcast frequencies: 2.5, 5.0, 10.0, and 15.0 MHz. "
              "Carrier frequency accuracy: ±1&times;10<sup>&minus;12</sup> relative, equivalent to "
              "&lt;0.001 Hz error at 10 MHz.") +
            table("WWV/WWVH Carrier Frequencies",
                  ["Freq (MHz)", "WWV", "WWVH", "Best Reception (EST)"],
                  [["2.5", "Yes", "Yes", "Night, low-band propagation"],
                   ["5.0", "Yes", "Yes", "Night, 0000&ndash;1200"],
                   ["10.0", "Yes", "Yes", "Daytime, most reliable"],
                   ["15.0", "Yes", "Yes", "Daytime, high solar activity"],
                   ["20.0", "Yes", "No", "Daytime, high solar activity"]])
        ) +
        section("2-2", "TRACEABILITY CHAIN",
            p("NIST F-2 cesium fountain &rarr; GPS carrier-phase comparison &rarr; "
              "master clock system &rarr; broadcast transmitters. The carrier is phase-coherent "
              "with UTC at the transmitter. Propagation delay introduces error "
              "(approximately 3.3 ms/1000 km); for frequency calibration this cancels out "
              "when measuring frequency rather than phase.") +
            note("For frequency calibration (not timing), propagation delay is irrelevant. "
                 "The received carrier frequency is identical to the transmitted frequency within "
                 "the stated accuracy. Doppler shift from ionospheric movement is small "
                 "(&lt;0.1 Hz at 10 MHz) and averages to zero over minutes."))
    )

    ch3 = chapter(3, "MATERIALS AND CONSTRUCTION", "ch3",
        section("3-1", "REQUIRED EQUIPMENT",
            table("Equipment List",
                  ["Item", "Description", "Specification", "Notes"],
                  [["1", "Shortwave receiver", "Coverage 2.5&ndash;20 MHz, SSB/AM", "Any general coverage HF receiver"],
                   ["2", "Frequency counter", "Resolution 1 Hz or better", "Gate time 1 s minimum"],
                   ["3", "BNC coax cable", "RG-58 or RG-174, 1&ndash;2 m", "Receiver output to counter"],
                   ["4", "Shortwave antenna", "Any wire, 5&ndash;30 m preferred", "Outdoor or attic preferred"],
                   ["5", "Headphones (optional)", "Any impedance", "Monitoring only"]])) +
        section("3-2", "ANTENNA CONSIDERATIONS",
            p("Any wire antenna will receive WWV/WWVH under favorable conditions. A 15 m (50 ft) "
              "end-fed wire antenna in the attic or outdoors provides adequate signal on most "
              "frequencies. Noise level in the shack is more limiting than antenna size.") +
            note("A NanoVNA or TinySA can serve as the frequency counter for this procedure "
                 "if the reference output connector is accessible. The TinySA Ultra internal "
                 "reference can itself be calibrated by this method."))
    )

    ch4 = chapter(4, "ASSEMBLY PROCEDURES", "ch4",
        section("4-1", "SETUP",
            procedure([
                "Connect shortwave antenna to receiver antenna input.",
                "Connect receiver line output or headphone output to frequency counter input via "
                "attenuator pad if needed. (Most receivers output 100&ndash;600 mV; most counters "
                "accept 100 mV minimum.)",
                "Set receiver to AM or SSB mode. For frequency calibration, USB mode gives a "
                "stable audio tone from the 1 kHz second-tick.",
                "Set frequency counter gate time to 10 seconds or longer for best accuracy.",
                "Allow equipment to warm up for 15 minutes minimum."
            ]))
    )

    ch5 = chapter(5, "CALIBRATION PROCEDURE", "ch5",
        caution("If calibrating the frequency counter's internal oscillator (TCXO/OCXO), "
                "refer to the counter manufacturer's manual. Some counters require internal "
                "adjustment access; incorrect adjustment can degrade accuracy.") +
        section("5-1", "DIRECT CARRIER METHOD (PREFERRED)",
            p("This method measures the received carrier directly with the frequency counter. "
              "It requires a receiver with a BFO output or accessible IF/detector output.") +
            procedure([
                "Tune receiver to 10.000 MHz (best daytime signal) or 5.000 MHz (night).",
                "Switch receiver to CW or USB mode. Zero-beat the carrier: the audio beat note "
                "approaches zero Hz as receiver VFO aligns with the carrier.",
                "Measure the receiver VFO frequency at zero-beat. This equals 10.000000 MHz "
                "(within &plusmn;5 Hz for consumer receivers, &plusmn;100 Hz for older equipment).",
                "If the counter shows 10.000100 MHz at zero-beat, the counter's reference is "
                "high by 10 Hz at 10 MHz = 1 ppm high.",
                "Apply correction: freq<sub>true</sub> = freq<sub>displayed</sub> &times; (1 &minus; error<sub>ppm</sub>/10<sup>6</sup>).",
                "Repeat on 5.000 MHz and 15.000 MHz to verify consistency.",
                "Record result in calibration log."
            ])) +
        section("5-2", "AUDIO BEAT NOTE METHOD",
            p("If direct carrier access is not available, beat the carrier against a known "
              "oscillator and count the beat note frequency.") +
            procedure([
                "Connect a 10 MHz signal generator (or GPSDO output) to one input of a "
                "signal mixer or the RF input of a second receiver.",
                "Receive WWV on the first receiver.",
                "Mix the two signals; the difference frequency (beat note) is the error "
                "between your oscillator and WWV.",
                "Count the beat note with the frequency counter. Any non-zero reading is "
                "the error of your signal generator at 10 MHz.",
                "Error in ppm = beat<sub>Hz</sub> / 10 (at 10 MHz reference)."
            ]))
    )

    ch6 = chapter(6, "TUNING AND ADJUSTMENT", "ch6",
        section("6-1", "ADJUSTING A FREQUENCY COUNTER REFERENCE",
            procedure([
                "Identify the trimmer capacitor (TCXO) or voltage adjustment pin (VCTCXO/OCXO) "
                "in the frequency counter.",
                "While monitoring the counter reading against WWV, adjust the trimmer in small "
                "increments until the displayed frequency matches expected value.",
                "Allow 5 minutes after each adjustment for thermal stabilization before "
                "reading final value.",
                "Do not over-adjust. Most TCXO trimmers have a range of &plusmn;10 ppm."
            ])) +
        section("6-2", "ADJUSTING A SIGNAL GENERATOR",
            procedure([
                "Set the generator to 10.000 MHz and connect output to the frequency counter.",
                "Compare counter reading against WWV-calibrated reference.",
                "Adjust the generator's frequency calibration control (typically a front-panel "
                "vernier or internal trimmer) to match.",
                "Lock the calibration control (if equipped) and record the setting."
            ]))
    )

    ch7 = chapter(7, "VERIFICATION", "ch7",
        section("7-1", "INDEPENDENT VERIFICATION",
            procedure([
                "After calibration, tune to a second WWV frequency (e.g., if calibrated on "
                "10 MHz, verify on 5 MHz).",
                "Repeat the frequency measurement. Error should be &lt;1 ppm if calibration "
                "was performed correctly.",
                "Verify at least 24 hours later to confirm stability: TCXO drift should "
                "be &lt;0.5 ppm/day.",
                "Enter all readings in the calibration log with date, temperature, and "
                "receiving conditions (fair/poor/excellent)."
            ])) +
        section("7-2", "ACCEPTANCE CRITERIA",
            table("Calibration Acceptance",
                  ["Instrument", "Acceptable Error", "Reject Threshold"],
                  [["TCXO-based counter", "&lt;1 ppm", "&gt;5 ppm"],
                   ["Crystal counter (no temp comp)", "&lt;10 ppm", "&gt;50 ppm"],
                   ["Signal generator", "&lt;5 ppm", "&gt;20 ppm"],
                   ["Receiver VFO", "&lt;100 Hz at 10 MHz", "&gt;500 Hz"]]))
    )

    appa = appchapter("appa", "A", "CALCULATIONS AND FORMULAS",
        section("A-1", "FREQUENCY ERROR",
            formula("Parts-per-million error",
                    "error<sub>ppm</sub> = (f<sub>measured</sub> &minus; f<sub>nominal</sub>) / f<sub>nominal</sub> &times; 10<sup>6</sup>") +
            formula("Correction factor",
                    "f<sub>true</sub> = f<sub>displayed</sub> &times; (1 &minus; error<sub>ppm</sub> / 10<sup>6</sup>)") +
            formula("Hz error from ppm (10 MHz)",
                    "error<sub>Hz</sub> = error<sub>ppm</sub> &times; 10 (at 10 MHz)")) +
        section("A-2", "PROPAGATION DELAY",
            formula("One-way propagation delay (ionospheric, approximate)",
                    "delay &asymp; 3.3 ms per 1000 km") +
            note("Propagation delay affects <em>phase</em> and <em>timing</em> measurements only. "
                 "For frequency calibration, the Doppler shift from a quasi-static ionosphere "
                 "averages to zero over 30 seconds or more. Time-of-day synchronization requires "
                 "propagation delay correction; frequency calibration does not."))
    )

    appb = appchapter("appb", "B", "EXAMPLE RESULTS",
        table("Typical Calibration Results",
              ["Instrument", "Reference", "Displayed", "Error", "Action"],
              [["TinySA Ultra (internal ref)", "10.000000 MHz (WWV)", "10.000020 MHz", "+2 ppm", "Acceptable; log"],
               ["Old HP frequency counter", "10.000000 MHz (WWV)", "10.000180 MHz", "+18 ppm", "Adjust TCXO"],
               ["Receiver VFO", "10.000000 MHz (WWV)", "9.999800 MHz", "&minus;200 Hz / &minus;20 ppm", "Note offset; apply correction"],
               ["Signal generator (old)", "5.000000 MHz (WWV)", "5.000045 MHz", "+9 ppm", "Adjust if accessible"]])
    )

    return tm_page(
        "TM-CAL-001",
        "WWV/WWVH TIME SIGNAL REFERENCE",
        "NIST Shortwave Time and Frequency Broadcasts as a Primary Frequency Standard",
        "wwv_wwvh_reference",
        ch1 + ch2 + ch3 + ch4 + ch5 + ch6 + ch7 + appa + appb
    )


# ═══════════════════════════════════════════════════════════════════════════════
# TM-CAL-002: GPS 1PPS Reference
# ═══════════════════════════════════════════════════════════════════════════════

def tm_gps1pps():
    ch1 = chapter(1, "GENERAL INFORMATION", "ch1",
        section("1-1", "SCOPE",
            p("This manual covers use of a GPS receiver's 1 pulse-per-second (1PPS) output "
              "as a timing reference for calibrating frequency counters, oscilloscopes, and "
              "time-interval measurement equipment. GPS 1PPS is traceable to UTC within "
              "&plusmn;100 ns under normal operating conditions.")) +
        section("1-2", "REFERENCES",
            ul(["IS-GPS-200 (GPS Interface Specification)",
                "u-blox NEO-M8N Product Summary",
                "NIST SP-432: Time and Frequency Services"])) +
        section("1-3", "SAFETY",
            note("GPS receiver modules operate at 3.3 V or 5 V DC. No shock hazard under "
                 "normal operating conditions.") +
            note("Outdoor GPS antenna installation: maintain safe clearance from power lines."))
    )

    ch2 = chapter(2, "THEORY OF OPERATION", "ch2",
        section("2-1", "GPS TIMING PRINCIPLE",
            p("GPS satellites carry cesium and rubidium atomic clocks synchronized to the "
              "GPS master control segment, which in turn is steered to UTC(USNO). The GPS "
              "receiver continuously solves for the time offset between the receiver clock "
              "and GPS time. Once a navigation solution is obtained (position locked), "
              "the 1PPS output is synchronized to UTC(GPS) within the stated accuracy.") +
            p("Typical 1PPS accuracy specifications:") +
            table("GPS Receiver 1PPS Accuracy",
                  ["Module", "1PPS Accuracy (RMS)", "1PPS Accuracy (Peak)"],
                  [["u-blox NEO-M8N (standard)", "&plusmn;30 ns RMS", "&plusmn;60 ns peak"],
                   ["u-blox NEO-M8N (timing mode)", "&plusmn;20 ns RMS", "&plusmn;30 ns peak"],
                   ["Generic SiRF IV module", "&plusmn;100 ns RMS", "&plusmn;300 ns peak"],
                   ["Generic NEO-6M", "&plusmn;50 ns RMS", "&plusmn;100 ns peak"]])) +
        section("2-2", "1PPS SIGNAL CHARACTERISTICS",
            p("The 1PPS pulse is a TTL-level (3.3 V or 5 V, depending on module) positive "
              "pulse, typically 100 ms wide, with the <em>rising edge</em> coinciding with "
              "the UTC second boundary. The pulse width is configurable on most modules.") +
            note("The 1PPS output is valid only after the receiver achieves a 3D position "
                 "lock (typically 60&ndash;120 seconds outdoors, longer with indoor antenna). "
                 "Always verify lock status before using 1PPS for calibration."))
    )

    ch3 = chapter(3, "MATERIALS AND CONSTRUCTION", "ch3",
        section("3-1", "BILL OF MATERIALS",
            table("Required Components",
                  ["Qty", "Item", "Specification", "Notes"],
                  [["1", "GPS receiver module", "NEO-M8N preferred; NEO-6M acceptable", "Must have 1PPS output pin"],
                   ["1", "GPS patch antenna", "Active, 25&times;25 mm or 35&times;35 mm", "Magnetic mount for mobility"],
                   ["1", "SMA or IPEX cable", "50 &Omega;, length as needed", "GPS antenna to module"],
                   ["1", "5 V power supply", "100 mA minimum, clean DC", "USB supply acceptable"],
                   ["1", "SMA/BNC adapter", "For 1PPS output", "Or BNC connector direct"],
                   ["1", "50 &Omega; BNC cable", "RG-58, 0.5&ndash;1 m", "1PPS to counter/scope"]])) +
        section("3-2", "LEVEL SHIFTING",
            p("If the GPS module outputs 3.3 V logic and the counter requires 5 V TTL, "
              "a simple level-shift circuit is needed.") +
            formula("Voltage divider for 5V to 3.3V (GPS antenna power only)",
                    "Not required for 1PPS: most counters accept 3.3 V on TTL input") +
            note("The NanoVNA, TinySA Ultra, and most modern frequency counters accept "
                 "3.3 V logic on TTL inputs. No level shifting is required for these instruments."))
    )

    ch4 = chapter(4, "ASSEMBLY PROCEDURES", "ch4",
        procedure([
            "Solder or connect the GPS module to 5 V power supply. Observe polarity. "
            "Module draws approximately 30&ndash;50 mA during acquisition.",
            "Connect GPS patch antenna to module antenna port (IPEX or SMA connector).",
            "Identify the 1PPS output pin. On NEO-M8N breakout boards, this is labeled "
            "TIMEPULSE or 1PPS. On bare modules, pin 3 (TIMEPULSE).",
            "Connect a 100 &Omega; series resistor in-line with the 1PPS output to protect "
            "the module output stage from capacitive loading.",
            "Connect 1PPS output to BNC connector on enclosure. Use short lead lengths "
            "(&lt;10 cm) to minimize EMI pickup.",
            "Place module in enclosure with antenna cable routed to exterior.",
            "Apply power. Confirm LED blink on most breakout boards; 1 Hz blink indicates lock. "
            "Acquisition takes 60&ndash;120 seconds outdoors.",
            "Verify 1PPS output with oscilloscope: 3.3 V (or 5 V) positive pulse, 100 ms wide, "
            "1.000000 Hz repetition rate."
        ])
    )

    ch5 = chapter(5, "CALIBRATION PROCEDURE", "ch5",
        caution("Do not use 1PPS for calibration until GPS lock is confirmed. "
                "Many modules continue to output 1PPS without lock; unlocked 1PPS "
                "can be off by milliseconds.") +
        section("5-1", "FREQUENCY COUNTER GATE CALIBRATION",
            procedure([
                "Set frequency counter to 1 Hz external gate input or 1 s internal gate.",
                "Apply 1PPS to the counter's external trigger or gate input.",
                "Measure a 10 MHz oscillator with 10 s gate time. The count should be "
                "exactly 100,000,000 &plusmn; 1 count for a perfect oscillator.",
                "Record: counts = f<sub>osc</sub> / f<sub>1PPS</sub> &times; gate time.",
                "Error in ppm = (actual counts &minus; expected counts) / (expected counts) &times; 10<sup>6</sup>.",
                "Example: gate = 10 s, expected 100,000,000 counts, actual 100,000,210 counts &rarr; +2.1 ppm error."
            ])) +
        section("5-2", "OSCILLOSCOPE TIMEBASE CALIBRATION",
            procedure([
                "Connect 1PPS to oscilloscope input. Set trigger to rising edge.",
                "Set timebase to 200 ms/div (2 seconds full screen for a 10-div display).",
                "The 1PPS pulse should appear at exactly 1.000000 s intervals.",
                "Measure the displayed period using cursors or automated measurement.",
                "Compare displayed period to 1.000000 s. Error &gt;0.5% indicates timebase drift.",
                "For 2&ndash;div cursor placement: set cursors at 0 ms and 1000 ms displayed. "
                "Count actual division spacing. Error = (measured &minus; 1000 ms) / 1000 ms &times; 100%."
            ]))
    )

    ch6 = chapter(6, "TUNING AND ADJUSTMENT", "ch6",
        section("6-1", "GPS RECEIVER CONFIGURATION",
            p("The u-blox NEO-M8N timing mode can be enabled via UBX configuration commands "
              "to reduce 1PPS jitter from ~30 ns to ~20 ns. This requires UART or USB "
              "connection to the module and u-center software or custom serial commands.") +
            note("For most amateur calibration work, the default timing mode is entirely "
                 "adequate. Enabling timing mode is optional.")) +
        section("6-2", "PULSE WIDTH ADJUSTMENT",
            p("Default 1PPS pulse width is 100 ms. For triggering older counters or "
              "oscilloscopes with high trigger thresholds, a one-shot circuit can reshape "
              "the pulse width. The pulse <em>timing</em> (rising edge position) is not "
              "affected by pulse width adjustments."))
    )

    ch7 = chapter(7, "VERIFICATION", "ch7",
        procedure([
            "Confirm GPS lock: check lock indicator LED (1 Hz blink on most modules) or "
            "parse NMEA GPRMC sentence for validity character 'A' (active).",
            "Measure 1PPS frequency with calibrated frequency counter. Should read "
            "1.000000 Hz &plusmn;0.000001 Hz (1 &mu;Hz).",
            "Compare 1PPS timing to WWV second tick (audio): within 0.5 s over 10 s is normal "
            "for ionospheric delay; phase difference should be constant.",
            "Log: date, time, GPS module model, satellite count, 1PPS frequency measured, "
            "ambient temperature."
        ])
    )

    appa = appchapter("appa", "A", "CALCULATIONS AND FORMULAS",
        formula("Frequency error from 1PPS gate count",
                "error<sub>ppm</sub> = (N<sub>actual</sub> &minus; N<sub>expected</sub>) / N<sub>expected</sub> &times; 10<sup>6</sup>") +
        formula("Expected counts (10 MHz oscillator, 10 s gate)",
                "N<sub>expected</sub> = f<sub>osc</sub> &times; t<sub>gate</sub> = 10&times;10<sup>6</sup> &times; 10 = 100,000,000") +
        formula("1PPS accuracy in seconds per day",
                "drift<sub>s/day</sub> = accuracy<sub>ns</sub> &times; 10<sup>&minus;9</sup> (this is per-pulse jitter, not accumulation)")
    )

    appb = appchapter("appb", "B", "EXAMPLE RESULTS",
        table("GPS 1PPS Calibration Log",
              ["Date", "GPS Module", "Sats", "Counter Ref (10 MHz)", "Error", "Notes"],
              [["2026-05-26", "NEO-M8N", "9", "10.000000 MHz", "0 ppm", "Timing mode enabled"],
               ["2026-05-26", "NEO-6M", "7", "9.999978 MHz", "&minus;2.2 ppm", "TCXO drift"],
               ["2026-05-26", "NEO-M8N", "11", "FNIRSI scope 1 ms/div", "+0.3%", "Acceptable"]])
    )

    return tm_page(
        "TM-CAL-002",
        "GPS 1PPS TIMING REFERENCE",
        "GPS Receiver 1 Pulse-per-Second Output as a UTC-Traceable Timing Standard",
        "gps_1pps_reference",
        ch1 + ch2 + ch3 + ch4 + ch5 + ch6 + ch7 + appa + appb
    )


# ═══════════════════════════════════════════════════════════════════════════════
# TM-CAL-003: GPSDO Reference
# ═══════════════════════════════════════════════════════════════════════════════

def tm_gpsdo():
    ch1 = chapter(1, "GENERAL INFORMATION", "ch1",
        section("1-1", "SCOPE",
            p("This manual covers construction and calibration use of a GPS-Disciplined "
              "Oscillator (GPSDO) providing a 10 MHz sine or square wave reference traceable "
              "to UTC via GPS. The GPSDO provides better short-term stability than 1PPS alone "
              "and is suitable as a lab reference for all RF calibration work.")) +
        section("1-2", "REFERENCES",
            ul(["TM-CAL-002: GPS 1PPS Timing Reference",
                "u-blox NEO-M8N Integration Manual",
                "Silicon Labs Si5351A Datasheet",
                "Analog Devices AD9833 Datasheet"])) +
        section("1-3", "SAFETY",
            note("GPSDO construction involves 5 V DC circuits only. No shock hazard.") +
            caution("The GPSDO output must be terminated in 50 &Omega;. Open-circuit or "
                    "short-circuit output damages the Si5351 output stage."))
    )

    ch2 = chapter(2, "THEORY OF OPERATION", "ch2",
        section("2-1", "GPSDO PRINCIPLE",
            p("A GPSDO phase-locks a voltage-controlled crystal oscillator (VCXO) or "
              "temperature-compensated crystal oscillator (TCXO) to the GPS 1PPS signal "
              "using a phase-locked loop (PLL) or proportional-integral (PI) control algorithm.") +
            p("Long-term frequency accuracy follows GPS (&asymp;10<sup>&minus;12</sup> relative). "
              "Short-term stability (seconds) follows the free-running crystal oscillator, "
              "which is better than GPS for short intervals due to GPS noise.")) +
        section("2-2", "IMPLEMENTATION: NEO-M8N + Si5351",
            p("The Si5351A clock generator synthesizes multiple output frequencies from "
              "an onboard 25 MHz crystal. Output frequencies are set by integer and fractional "
              "division registers over I2C. The ESP32 microcontroller reads GPS NMEA data, "
              "monitors 1PPS, and adjusts Si5351 PLL registers to phase-lock the output.") +
            table("GPSDO Performance (NEO-M8N + Si5351A, SW PLL)",
                  ["Parameter", "Value", "Notes"],
                  [["Frequency range", "1 Hz &ndash; 160 MHz", "Si5351A spec"],
                   ["Output frequency (typical)", "10.000000 MHz", "Adjustable"],
                   ["Long-term accuracy", "&lt;1&times;10<sup>&minus;10</sup>", "GPS-limited"],
                   ["Short-term stability (1 s)", "&asymp;10<sup>&minus;9</sup>", "Crystal-limited"],
                   ["Output level", "~3 dBm into 50 &Omega;", "Si5351 CLK0"],
                   ["Lock time", "5&ndash;15 minutes", "After GPS position fix"]]))
    )

    ch3 = chapter(3, "MATERIALS AND CONSTRUCTION", "ch3",
        section("3-1", "BILL OF MATERIALS",
            table("GPSDO Components",
                  ["Qty", "Item", "Specification"],
                  [["1", "u-blox NEO-M8N GPS module", "With UART and TIMEPULSE"],
                   ["1", "Si5351A clock breakout", "Adafruit or equivalent, 25 MHz XTAL"],
                   ["1", "ESP32 development board", "Any variant with I2C and UART"],
                   ["1", "Active GPS patch antenna", "Magnetic mount, 3.3 V bias"],
                   ["1", "5 V / 500 mA supply", "Clean, low-ripple"],
                   ["1", "SMA panel connector", "For 10 MHz output"],
                   ["1", "Project enclosure", "Diecast aluminum preferred for RF shielding"],
                   ["4", "100 nF bypass capacitors", "On every power rail"],
                   ["1", "10 &mu;F electrolytic", "Bulk bypass for ESP32 supply"]])) +
        section("3-2", "WIRING",
            p("Connect NEO-M8N UART TX &rarr; ESP32 UART RX. Connect NEO-M8N TIMEPULSE "
              "&rarr; ESP32 GPIO (e.g., GPIO 4) with 100 &Omega; series resistor. "
              "Connect Si5351 SDA/SCL &rarr; ESP32 I2C (GPIO 21/22). "
              "Connect Si5351 CLK0 output &rarr; SMA panel connector via short 50 &Omega; trace."))
    )

    ch4 = chapter(4, "ASSEMBLY PROCEDURES", "ch4",
        procedure([
            "Verify all components with multimeter before assembly.",
            "Install ESP32 board on DIN rail or in enclosure, securing with 4 M3 screws.",
            "Mount Si5351 breakout board adjacent to ESP32. Keep I2C traces short (&lt;5 cm).",
            "Mount NEO-M8N module. Route UART and TIMEPULSE connections to ESP32.",
            "Install SMA connector on enclosure panel. Solder short RG-174 pigtail from "
            "Si5351 CLK0 output to SMA center pin. Minimize trace length.",
            "Install GPS antenna SMA bulkhead connector on enclosure lid or side panel.",
            "Wire 5 V supply to all modules. Verify +5 V on each module before applying full power.",
            "Load ESP32 firmware (simple PLL algorithm: measure 1PPS &rarr; count Si5351 cycles "
            "&rarr; adjust frequency word in PI loop).",
            "Apply power. Observe serial debug output: GPS acquiring, then position fix, "
            "then TIMEPULSE valid, then PLL locking.",
            "Allow 15 minutes for full phase lock. Output frequency should stabilize to "
            "&lt;1 Hz of 10.000000 MHz."
        ])
    )

    ch5 = chapter(5, "CALIBRATION PROCEDURE", "ch5",
        section("5-1", "INITIAL FREQUENCY CALIBRATION",
            procedure([
                "Connect GPSDO 10 MHz output to frequency counter via 50 &Omega; BNC cable.",
                "Allow GPSDO to lock completely (GPS fix acquired + 15 min warm-up).",
                "Measure output frequency. Should read 10.000000 &plusmn;0.010 MHz within "
                "5 minutes of lock.",
                "If frequency is off by more than 1 Hz, check Si5351 crystal frequency "
                "setting in firmware. The on-board crystal may be 25.000 or 26.000 MHz; "
                "verify and update firmware constant.",
                "Monitor frequency for 30 minutes. Deviation from 10.000000 should be "
                "&lt;1 Hz after full thermal stabilization.",
                "Record: frequency, GPS satellite count, ambient temperature."
            ])) +
        section("5-2", "LONG-TERM VERIFICATION",
            procedure([
                "Run GPSDO continuously for 24 hours.",
                "Record frequency every hour (automate with frequency counter if possible).",
                "Plot frequency vs. time. Drift &gt;0.1 Hz over 24 hours indicates "
                "PLL loop filter or crystal aging issue.",
                "Compare to WWV at 10 MHz using zero-beat method (see TM-CAL-001). "
                "Agreement within 1 Hz confirms NIST traceability."
            ]))
    )

    ch6 = chapter(6, "TUNING AND ADJUSTMENT", "ch6",
        section("6-1", "PLL LOOP FILTER TUNING",
            p("The PI control loop has two parameters: proportional gain (K<sub>p</sub>) "
              "and integral gain (K<sub>i</sub>). Too high K<sub>p</sub> causes oscillation; "
              "too low causes slow lock. Start with K<sub>p</sub> = 0.1 and K<sub>i</sub> = 0.01, "
              "then increase K<sub>p</sub> until oscillation appears, then back off 50%.") +
            note("The Si5351A frequency resolution is approximately 0.01 Hz at 10 MHz due to "
                 "the 27-bit PLL multisynth fractional divider. This limits PLL granularity "
                 "but is adequate for most calibration applications."))
    )

    ch7 = chapter(7, "VERIFICATION", "ch7",
        procedure([
            "Measure GPSDO output against WWV 10 MHz carrier using zero-beat method. "
            "Error should be &lt;1 Hz.",
            "Confirm GPS lock: check satellite count in NMEA GPGGA sentence (&ge;4 sats required).",
            "Verify 1PPS LED or debug output shows TIMEPULSE active.",
            "Connect GPSDO to TinySA Ultra reference input (if equipped) and confirm TinySA "
            "frequency readouts are consistent with GPSDO.",
            "Log: date, firmware version, GPS module serial, lock time, frequency error."
        ])
    )

    appa = appchapter("appa", "A", "CALCULATIONS AND FORMULAS",
        formula("Si5351A output frequency",
                "f<sub>out</sub> = f<sub>xtal</sub> &times; PLL_mult / (output_divider)") +
        formula("PPM error from integer count",
                "error<sub>ppm</sub> = (f<sub>measured</sub> &minus; 10,000,000) / 10") +
        formula("Allan deviation (GPSDO estimate)",
                "ADEV(&tau; = 1 s) &asymp; 1&times;10<sup>&minus;9</sup> (crystal limited)")
    )

    appb = appchapter("appb", "B", "EXAMPLE RESULTS",
        table("GPSDO Lock and Stability Log",
              ["Time After Power-On", "GPS Sats", "10 MHz Frequency", "Deviation", "PLL State"],
              [["0 min", "0", "&mdash;", "&mdash;", "Acquiring"],
               ["2 min", "5", "&mdash;", "&mdash;", "Fix, no PLL"],
               ["5 min", "8", "9.999998 MHz", "&minus;0.2 Hz", "PLL locking"],
               ["15 min", "9", "10.000000 MHz", "&lt;0.05 Hz", "Locked"],
               ["1 hour", "9", "10.000000 MHz", "&lt;0.02 Hz", "Stable"]])
    )

    return tm_page(
        "TM-CAL-003",
        "GPS-DISCIPLINED OSCILLATOR (GPSDO)",
        "10 MHz Reference Oscillator Disciplined to GPS/UTC via Phase-Locked Loop",
        "gpsdo_reference",
        ch1 + ch2 + ch3 + ch4 + ch5 + ch6 + ch7 + appa + appb
    )


# ═══════════════════════════════════════════════════════════════════════════════
# TM-CAL-004: FM Broadcast Carrier Reference
# ═══════════════════════════════════════════════════════════════════════════════

def tm_fm_broadcast():
    ch1 = chapter(1, "GENERAL INFORMATION", "ch1",
        section("1-1", "SCOPE",
            p("This manual covers use of FM broadcast station carriers as a VHF frequency "
              "reference for calibrating spectrum analyzers, antenna analyzers, receivers, "
              "and signal generators in the 88&ndash;108 MHz range. FM stations maintain "
              "carrier frequencies within &plusmn;2 kHz of their assigned frequency per "
              "FCC Part 73; in practice most are within &plusmn;500 Hz.")) +
        section("1-2", "REFERENCES",
            ul(["FCC Part 73.1545: FM Carrier Frequency Tolerance",
                "FCC AM/FM Query: transition.fcc.gov/fcc-bin/fmq.pl",
                "ARRL Handbook: VHF/UHF Measurement Techniques"])) +
        section("1-3", "SAFETY",
            note("No hazardous voltages involved in this procedure. All measurements are "
                 "performed at receiver or spectrum analyzer input levels."))
    )

    ch2 = chapter(2, "THEORY OF OPERATION", "ch2",
        section("2-1", "FM BROADCAST FREQUENCY STANDARDS",
            p("FCC Rules 73.1545 require FM stations to maintain carrier frequency within "
              "&plusmn;2,000 Hz of assigned channel. Channels are assigned in the range "
              "88.1 to 107.9 MHz at 200 kHz spacing. In practice, modern station automation "
              "with GPS-disciplined frequency synthesis typically achieves &plusmn;100&ndash;500 Hz.") +
            p("The carrier is accessible without decoding: a spectrum analyzer or antenna "
              "analyzer with tracking generator can measure the carrier center frequency "
              "directly.") +
            table("FM Reference Quality Tiers",
                  ["Station Type", "Typical Accuracy", "FCC Limit"],
                  [["Major station with GPS synth", "&plusmn;100 Hz", "&plusmn;2 kHz"],
                   ["Typical commercial station", "&plusmn;500 Hz", "&plusmn;2 kHz"],
                   ["Low-power FM (LPFM)", "&plusmn;2 kHz", "&plusmn;3 kHz"],
                   ["Translator station", "&plusmn;1 kHz", "&plusmn;2 kHz"]])) +
        section("2-2", "TRACEABILITY",
            p("FM stations certified under FCC 73.1545 are required to maintain compliance. "
              "For the purpose of this standard, the FM carrier provides a VHF reference "
              "accurate to approximately &plusmn;2 kHz / 100 MHz &asymp; &plusmn;20 ppm worst case, "
              "&plusmn;5 ppm typical. This is adequate for VHF receiver alignment and spectrum "
              "analyzer calibration checks."))
    )

    ch3 = chapter(3, "MATERIALS AND CONSTRUCTION", "ch3",
        section("3-1", "REQUIRED EQUIPMENT",
            table("Equipment",
                  ["Item", "Description", "Notes"],
                  [["TinySA Ultra", "Spectrum analyzer / frequency counter", "Preferred; direct carrier frequency readout"],
                   ["Dipole or whip antenna", "88&ndash;108 MHz, &lambda;/4 or &lambda;/2", "~80 cm for 88 MHz"],
                   ["BNC coax cable", "50 &Omega;, 0.5&ndash;2 m", ""],
                   ["FCC FM database", "List of local stations and assigned frequencies", "fcc.gov lookup"]])) +
        section("3-2", "PREPARATION",
            p("Before calibrating, look up the assigned frequencies of 2&ndash;3 local FM "
              "stations using the FCC FM Query database. Record: call letters, channel number, "
              "assigned frequency (MHz), and transmitter location. Choose high-power stations "
              "(&ge;25 kW ERP) at moderate distance (10&ndash;100 km) for best signal quality."))
    )

    ch4 = chapter(4, "ASSEMBLY PROCEDURES", "ch4",
        procedure([
            "Connect dipole or whip antenna to TinySA Ultra INPUT port.",
            "Set TinySA Ultra to spectrum analyzer mode, center frequency 98 MHz (adjust to "
            "local strong station), span 10 MHz, RBW 3 kHz or narrower.",
            "Identify the carrier peak of the target station. It will appear as the tallest "
            "narrowband spike within the station's 200 kHz channel.",
            "Set TinySA to narrow span: center on carrier, span 500 kHz, RBW 1 kHz.",
            "Engage TinySA frequency counter or cursor on carrier peak.",
            "Record measured frequency."
        ])
    )

    ch5 = chapter(5, "CALIBRATION PROCEDURE", "ch5",
        caution("Multipath reflections can shift the apparent carrier frequency by up to "
                "several hundred Hz in urban environments. Measure from a location with "
                "clear line-of-sight to the transmitter if possible.") +
        section("5-1", "PROCEDURE",
            procedure([
                "Identify local FM station; obtain assigned frequency from FCC database.",
                "Measure carrier center frequency with TinySA using MARKER PEAK function.",
                "Record: assigned frequency f<sub>assigned</sub>, measured frequency f<sub>meas</sub>.",
                "Compute error: error<sub>Hz</sub> = f<sub>meas</sub> &minus; f<sub>assigned</sub>.",
                "Compute ppm: error<sub>ppm</sub> = error<sub>Hz</sub> / f<sub>assigned</sub> &times; 10<sup>6</sup>.",
                "If TinySA reads f<sub>meas</sub> = 98.102.4 kHz and assigned is 98.100.0 kHz, "
                "error = +2.4 kHz / 98.1 MHz = +24.5 ppm &rarr; TinySA internal reference is high.",
                "Repeat on 2 additional stations. Average the errors.",
                "Apply correction offset in TinySA reference calibration menu.",
                "Verify correction by re-measuring all stations: residual error should "
                "be &lt;2 kHz on all."
            ]))
    )

    ch6 = chapter(6, "TUNING AND ADJUSTMENT", "ch6",
        p("Most spectrum analyzers (including TinySA) provide a reference oscillator "
          "calibration function accessible through the system menu. Enter the correction "
          "value in ppm or Hz as determined in Chapter 5. Consult instrument documentation "
          "for specific menu path.")
    )

    ch7 = chapter(7, "VERIFICATION", "ch7",
        procedure([
            "After calibration, measure 3 different FM stations on different frequencies "
            "across the 88&ndash;108 MHz band.",
            "Compare all measured frequencies against FCC database values.",
            "Residual errors &lt;2 kHz on all stations confirm adequate calibration.",
            "If any station shows error &gt;3 kHz, verify that station assignment in "
            "FCC database is current (stations occasionally shift channels).",
            "Log: date, station call signs, assigned frequencies, measured frequencies, errors."
        ])
    )

    appa = appchapter("appa", "A", "CALCULATIONS AND FORMULAS",
        formula("Carrier frequency error (Hz)",
                "error<sub>Hz</sub> = f<sub>measured</sub> &minus; f<sub>assigned</sub>") +
        formula("Error in ppm",
                "error<sub>ppm</sub> = error<sub>Hz</sub> / f<sub>assigned</sub> &times; 10<sup>6</sup>") +
        formula("Example: 98.1 MHz station, measured 98,102,400 Hz",
                "error = +2400 Hz; error<sub>ppm</sub> = 2400 / 98,100,000 &times; 10<sup>6</sup> = +24.5 ppm")
    )

    appb = appchapter("appb", "B", "EXAMPLE RESULTS",
        table("FM Carrier Calibration",
              ["Station", "Assigned (MHz)", "Measured (MHz)", "Error (Hz)", "Error (ppm)"],
              [["KXYZ", "98.100", "98.1002", "+200 Hz", "+2.0 ppm"],
               ["KABC", "103.500", "103.5011", "+1100 Hz", "+10.6 ppm"],
               ["KDEF", "107.700", "107.7007", "+700 Hz", "+6.5 ppm"]])
    )

    return tm_page(
        "TM-CAL-004",
        "FM BROADCAST CARRIER REFERENCE",
        "FCC-Licensed FM Stations as VHF Frequency Standards (88&ndash;108 MHz)",
        "fm_broadcast_reference",
        ch1 + ch2 + ch3 + ch4 + ch5 + ch6 + ch7 + appa + appb
    )


# ═══════════════════════════════════════════════════════════════════════════════
# TM-CAL-005: AC Mains Frequency Reference
# ═══════════════════════════════════════════════════════════════════════════════

def tm_ac_mains():
    ch1 = chapter(1, "GENERAL INFORMATION", "ch1",
        section("1-1", "SCOPE",
            p("This manual covers use of the 60 Hz AC power grid as a frequency reference "
              "for calibrating oscilloscopes, audio oscillators, and low-frequency measurement "
              "equipment. The US power grid maintains 60.00 Hz as a long-term average; "
              "short-term deviation is &plusmn;0.5 Hz with an NERC target of &plusmn;0.02 Hz.")) +
        section("1-2", "REFERENCES",
            ul(["NERC: Frequency Response Obligation",
                "IEEE Std 1547: Interconnection Standards for Distributed Resources",
                "NIST SP-432: Time and Frequency Services"])) +
        section("1-3", "SAFETY PRECAUTIONS",
            warning("AC MAINS VOLTAGE IS LETHAL. The 120 V / 60 Hz power grid can cause "
                    "cardiac arrest or severe burns on contact. NEVER connect test equipment "
                    "directly to mains wiring without an isolation transformer. All connections "
                    "must be made with the circuit de-energized.") +
            caution("Use a commercially available isolation transformer rated for full mains "
                    "voltage and current. Do not construct a home-built autotransformer for "
                    "this application. The transformer provides both voltage reduction and "
                    "electrical isolation from the power grid.") +
            note("This standard is most useful for long-interval (60-second average) frequency "
                 "measurements. Short-term (1-second) grid frequency can deviate by up to "
                 "&plusmn;0.5 Hz, which is &plusmn;0.83% error."))
    )

    ch2 = chapter(2, "THEORY OF OPERATION", "ch2",
        section("2-1", "GRID FREQUENCY CONTROL",
            p("North American electric utilities regulate grid frequency under NERC "
              "Reliability Standard BAL-003. The target is 60.000 Hz; the allowed deviation "
              "band is &plusmn;0.036 Hz (2.16 rpm at 3600 RPM turbines). "
              "The long-term (24-hour) average is maintained to 60.000000 Hz by time error "
              "correction procedures: utilities add or subtract generation to return the "
              "accumulated cycle count to zero.") +
            p("For frequency calibration, the <em>long-term average</em> is the reference. "
              "A 60-second measurement averages out short-term deviations and provides "
              "a reference accurate to &plusmn;0.01 Hz (&plusmn;170 ppm at 60 Hz)."))
    )

    ch3 = chapter(3, "MATERIALS AND CONSTRUCTION", "ch3",
        warning("All construction involving AC mains connections must be performed by "
                "a qualified person with appropriate tools and PPE. Verify mains is "
                "de-energized before making any connections.") +
        section("3-1", "ISOLATION CIRCUIT",
            p("Use a small 120V:6.3V or 120V:12V power transformer (wall-wart transformer "
              "or chassis-mount type). The secondary provides a safe, isolated 60 Hz signal "
              "at a level suitable for oscilloscope or frequency counter input.") +
            table("Suggested Components",
                  ["Item", "Specification", "Notes"],
                  [["Isolation transformer", "120V primary, 6.3V secondary, 100 mA", "Mouser or Jameco"],
                   ["Voltage divider", "10k&Omega; / 1k&Omega; divider on secondary", "Reduces to ~0.6V RMS"],
                   ["BNC connector", "Panel mount", "Output to counter/scope"],
                   ["Enclosure", "ABS plastic box", "Fully enclose primary-side wiring"]]))
    )

    ch4 = chapter(4, "ASSEMBLY PROCEDURES", "ch4",
        warning("De-energize mains before making any connections to transformer primary.") +
        procedure([
            "Mount transformer and voltage divider in plastic enclosure.",
            "Connect transformer primary to IEC inlet or hardwired mains cable with strain relief. "
            "Fuse the primary at 0.5 A.",
            "Connect 10k&Omega;/1k&Omega; divider across secondary. Center tap of divider to BNC ground.",
            "Connect junction of divider (1k&Omega; side) to BNC center conductor.",
            "Mount BNC panel connector on enclosure.",
            "Close enclosure. Apply label: 'AC MAINS 60 Hz REFERENCE &mdash; SECONDARY ONLY'.",
            "Apply power. Measure BNC output with oscilloscope: should show clean 60 Hz "
            "sinusoid at approximately 0.6V RMS (6.3V secondary &times; 1k/(10k+1k)).",
            "Verify no DC offset (&lt;50 mV DC on meter)."
        ])
    )

    ch5 = chapter(5, "CALIBRATION PROCEDURE", "ch5",
        section("5-1", "OSCILLOSCOPE TIMEBASE",
            procedure([
                "Connect AC mains reference output to oscilloscope channel 1.",
                "Set timebase to 5 ms/div (50 ms full screen for 10-div).",
                "The 60 Hz waveform should show exactly 3 full cycles in 50 ms "
                "(period = 16.667 ms).",
                "Measure period with oscilloscope cursors: place cursor 1 at a positive zero "
                "crossing, cursor 2 at the next positive zero crossing.",
                "Displayed period should be 16.67 ms &plusmn;0.5%.",
                "For better accuracy, measure 10 cycles: 10 &times; 16.667 ms = 166.67 ms.",
                "Error% = (measured_ms &minus; 166.67) / 166.67 &times; 100%."
            ])) +
        section("5-2", "FREQUENCY COUNTER",
            procedure([
                "Connect reference to frequency counter input via 50 &Omega; coax.",
                "Set gate time to 60 seconds for best accuracy.",
                "Expected reading: 60.000 Hz &plusmn;0.5 Hz instantaneous, "
                "60.000 Hz &plusmn;0.02 Hz over 60-second gate.",
                "If counter reads 59.995 Hz with 60 s gate, short-term grid deviation is present; "
                "repeat measurement and average 5 readings."
            ]))
    )

    ch6 = chapter(6, "TUNING AND ADJUSTMENT", "ch6",
        note("The AC mains frequency cannot be adjusted; it is a reference only. "
             "Instrument adjustments based on mains frequency should account for the "
             "&plusmn;0.5 Hz short-term variation. Use a 60-second or longer gate time "
             "for all calibration measurements.")
    )

    ch7 = chapter(7, "VERIFICATION", "ch7",
        procedure([
            "Compare measured grid frequency against NIST Internet Time Service "
            "(time.nist.gov) live frequency readout, if available.",
            "Alternatively, compare against GPS 1PPS &mdash; count 60 Hz cycles per GPS second. "
            "Expected: 60 &plusmn;0.5 per GPS second.",
            "Record ambient frequency on grid monitoring websites such as "
            "gridstatus.io or fnetpublic.utk.edu (Eastern Interconnect)."
        ])
    )

    appa = appchapter("appa", "A", "CALCULATIONS AND FORMULAS",
        formula("Period from frequency",
                "T = 1/f = 1/60 = 0.016667 s = 16.667 ms") +
        formula("Time error from frequency error",
                "t<sub>error</sub>/day = (f<sub>error</sub>/f<sub>nominal</sub>) &times; 86400 s") +
        formula("Example: 1 Hz error at 60 Hz",
                "time error = (1/60) &times; 86400 = 1440 s/day = 24 min/day fast or slow")
    )

    appb = appchapter("appb", "B", "EXAMPLE RESULTS",
        table("AC Mains Calibration Log",
              ["Date/Time", "Gate (s)", "Measured (Hz)", "Period (ms)", "Notes"],
              [["2026-05-26 09:00", "60", "60.003", "16.666", "Morning peak load"],
               ["2026-05-26 14:00", "60", "59.998", "16.667", "Afternoon nominal"],
               ["2026-05-26 22:00", "60", "60.001", "16.667", "Evening, near nominal"]])
    )

    return tm_page(
        "TM-CAL-005",
        "AC MAINS FREQUENCY REFERENCE",
        "60 Hz Power Grid as a Low-Frequency Calibration Standard",
        "ac_mains_reference",
        ch1 + ch2 + ch3 + ch4 + ch5 + ch6 + ch7 + appa + appb
    )


# ═══════════════════════════════════════════════════════════════════════════════
# TM-CAL-006: Precision Voltage Reference
# ═══════════════════════════════════════════════════════════════════════════════

def tm_prec_voltage():
    ch1 = chapter(1, "GENERAL INFORMATION", "ch1",
        section("1-1", "SCOPE",
            p("This manual covers construction and use of a precision voltage reference "
              "based on IC band-gap reference circuits. The reference provides stable, "
              "accurate DC voltages traceable to the SI volt through the component "
              "manufacturer's calibration chain. Suitable for calibrating digital multimeters, "
              "analog panel meters, and ADC input circuits.")) +
        section("1-3", "SAFETY",
            note("Operates from a 9&ndash;15 V DC supply. No shock hazard under normal conditions.") +
            caution("Band-gap reference ICs are sensitive to electrostatic discharge. Handle "
                    "with ESD precautions. Do not allow output to be connected to voltages "
                    "exceeding the supply rail."))
    )

    ch2 = chapter(2, "THEORY OF OPERATION", "ch2",
        section("2-1", "BAND-GAP REFERENCE PRINCIPLE",
            p("Band-gap voltage references exploit the temperature-stable forward voltage "
              "of silicon junctions. A PTAT (proportional-to-absolute-temperature) current "
              "is summed with a complementary CTAT current; when properly ratioed, the sum "
              "equals the silicon band-gap extrapolated voltage of approximately 1.205 V. "
              "This voltage is independent of temperature to first order.") +
            p("Precision reference ICs (LM4040, LT1021, LM399, LTZ1000) implement this "
              "principle with additional trimming and, in high-precision devices, "
              "temperature-controlled substrates.") +
            table("Voltage Reference IC Comparison",
                  ["IC", "Voltage", "Initial Acc.", "Temp. Coeff.", "Notes"],
                  [["LM4040A-5.0", "5.000 V", "&plusmn;0.1%", "100 ppm/&deg;C", "Shunt reg, low cost"],
                   ["LM4040B-5.0", "5.000 V", "&plusmn;0.2%", "100 ppm/&deg;C", "Economy grade"],
                   ["LT1021-5", "5.000 V", "&plusmn;0.05%", "5 ppm/&deg;C", "Excellent, SOT-23"],
                   ["LM399", "6.95 V", "&plusmn;0.3%", "0.3 ppm/&deg;C", "Heated substrate, best TC"],
                   ["LTZ1000", "7.0 V", "&plusmn;0.05%", "0.05 ppm/&deg;C", "Ultra-precision, requires heater"]]))
    )

    ch3 = chapter(3, "MATERIALS AND CONSTRUCTION", "ch3",
        section("3-1", "BILL OF MATERIALS (LT1021-5 VERSION)",
            table("Components",
                  ["Qty", "Item", "Value/Spec"],
                  [["1", "LT1021CCZ-5 or LT1021BCZ-5", "5.000 V precision reference, TO-92 or DIP-8"],
                   ["1", "Voltage regulator", "LM7809 or LM317, 9&ndash;12 V input"],
                   ["1", "Electrolytic capacitor", "10 &mu;F / 25 V"],
                   ["2", "Ceramic capacitor", "100 nF, C0G/NP0 on reference input and output"],
                   ["1", "Resistor", "1k&Omega; 0.1%, current limiting for shunt types"],
                   ["1", "BNC or banana jack", "Output connector"],
                   ["1", "9 V battery or DC input", "For portable use"]]))
    )

    ch4 = chapter(4, "ASSEMBLY PROCEDURES", "ch4",
        procedure([
            "Pre-regulate supply to 9 V using LM7809 with 100 nF input/output bypass capacitors.",
            "For LT1021 (series type): connect IN pin to 9 V through 1k&Omega; resistor. "
            "Connect OUT to output terminal. Connect GND to ground. Add 100 nF on output.",
            "For LM4040 (shunt type): connect through 1k&Omega; from supply to LM4040 anode. "
            "Output is taken across the LM4040. Load current must be &lt;15 mA.",
            "Solder all components on perfboard with short, tidy leads. Shield board "
            "from air currents if TC performance is critical.",
            "Mount in small enclosure. Label output: '5.000 V REF &plusmn;0.1%'.",
            "Allow 15 minutes warm-up before first measurement."
        ])
    )

    ch5 = chapter(5, "CALIBRATION PROCEDURE", "ch5",
        section("5-1", "OUTPUT VOLTAGE MEASUREMENT",
            procedure([
                "Allow reference to warm up for 15 minutes.",
                "Measure output with the best available DMM (Fluke 15B+ or equivalent).",
                "Record: V<sub>meas</sub>, ambient temperature, supply voltage.",
                "Compare to nominal 5.000 V: error% = (V<sub>meas</sub> &minus; 5.000) / 5.000 &times; 100%.",
                "For LT1021BCZ-5: accept &plusmn;0.05% (&plusmn;2.5 mV). Trim with external resistor if needed.",
                "For LM4040A: accept &plusmn;0.1% (&plusmn;5.0 mV). Not adjustable; select units."
            ])) +
        section("5-2", "USING THE REFERENCE TO CALIBRATE A DMM",
            procedure([
                "Connect reference output to DMM V input.",
                "DMM should read within its specification of the reference voltage.",
                "If DMM reads 4.985 V and reference is known 5.000 V, DMM error = &minus;3.0%.",
                "Apply correction: actual voltage = DMM reading &times; (V<sub>ref</sub> / V<sub>dmm</sub>).",
                "Verify at multiple voltage settings using a precision resistor divider "
                "to derive additional reference points."
            ]))
    )

    ch6 = chapter(6, "TUNING AND ADJUSTMENT", "ch6",
        p("The LT1021 can be trimmed by connecting a small resistance (&lt;100 &Omega;) "
          "in series with the internal trim network (pins 4 and 5 on DIP-8 package) to "
          "adjust the output voltage up or down by up to 0.2%. A 50 &Omega; trimmer pot "
          "provides the full adjustment range. Set at room temperature after full thermal "
          "stabilization.")
    )

    ch7 = chapter(7, "VERIFICATION", "ch7",
        procedure([
            "Verify output at T = 25 &plusmn;5&deg;C after 15-minute warm-up.",
            "Remeasure after 24 hours: long-term stability should be &lt;50 ppm/month for LT1021.",
            "Verify supply rejection: change input supply voltage from 9 V to 12 V. "
            "Output should change &lt;1 mV (&lt;0.02%).",
            "Log all readings with date and temperature."
        ])
    )

    appa = appchapter("appa", "A", "CALCULATIONS AND FORMULAS",
        formula("Voltage error in ppm",
                "error<sub>ppm</sub> = (V<sub>meas</sub> &minus; V<sub>nom</sub>) / V<sub>nom</sub> &times; 10<sup>6</sup>") +
        formula("Temperature error (LT1021, 5 ppm/&deg;C)",
                "&Delta;V = V<sub>nom</sub> &times; TC &times; &Delta;T = 5.000 &times; 5&times;10<sup>&minus;6</sup> &times; &Delta;T") +
        formula("Example: 10&deg;C temperature change with LT1021",
                "&Delta;V = 5.000 &times; 5&times;10<sup>&minus;6</sup> &times; 10 = 0.00025 V = 0.25 mV")
    )

    appb = appchapter("appb", "B", "EXAMPLE RESULTS",
        table("Voltage Reference Output Log",
              ["Date", "IC", "Supply (V)", "Temp (&deg;C)", "Output (V)", "Error (mV)", "Error (ppm)"],
              [["2026-05-26", "LT1021BCZ-5", "9.12", "23", "4.9998", "&minus;0.2", "&minus;40 ppm"],
               ["2026-05-26", "LM4040A-5.0", "9.12", "23", "5.0038", "+3.8", "+760 ppm"],
               ["2026-05-26", "LT1021BCZ-5", "12.00", "23", "5.0000", "0.0", "0 ppm"]])
    )

    return tm_page(
        "TM-CAL-006",
        "PRECISION VOLTAGE REFERENCE",
        "Band-Gap IC References (LM4040, LT1021) for DC Voltage Calibration",
        "precision_voltage_reference",
        ch1 + ch2 + ch3 + ch4 + ch5 + ch6 + ch7 + appa + appb
    )


# ═══════════════════════════════════════════════════════════════════════════════
# TM-CAL-007: Zener + Divider Multi-Voltage Reference
# ═══════════════════════════════════════════════════════════════════════════════

def tm_zener():
    ch1 = chapter(1, "GENERAL INFORMATION", "ch1",
        section("1-1", "SCOPE",
            p("This manual covers construction of a multi-voltage reference using "
              "temperature-compensated zener diodes and precision resistor dividers. "
              "The circuit provides multiple calibrated DC reference voltages from a single "
              "stable source. Less precise than band-gap references but adequate for "
              "&plusmn;0.5% calibration work."))
    )

    ch2 = chapter(2, "THEORY OF OPERATION", "ch2",
        section("2-1", "TEMPERATURE-COMPENSATED ZENER",
            p("Zener diodes exhibit a negative temperature coefficient (TC) below "
              "approximately 5.6 V and a positive TC above 5.6 V. At ~5.6 V zener voltage, "
              "TC &asymp; 0. The 1N829 and LM336 are designed to operate at this compensation point.") +
            p("A forward-biased diode (TC &asymp; &minus;2 mV/&deg;C) in series with a "
              "6.2 V zener (TC &asymp; +2 mV/&deg;C) achieves near-zero overall TC. "
              "The LM336 IC integrates this compensation internally.") +
            table("Zener Reference Devices",
                  ["Device", "Voltage", "TC", "Accuracy", "Notes"],
                  [["LM336-2.5", "2.5 V", "&lt;20 ppm/&deg;C", "&plusmn;1%", "Adjustable with ext R"],
                   ["LM385-2.5", "2.5 V", "&lt;50 ppm/&deg;C", "&plusmn;1%", "Micropower"],
                   ["1N829", "6.2 V", "&lt;10 ppm/&deg;C", "&plusmn;0.5%", "Hermetic zener"],
                   ["1N5234", "6.2 V", "&lt;100 ppm/&deg;C", "&plusmn;5%", "General purpose"]])) +
        section("2-2", "RESISTOR DIVIDER",
            p("A precision resistor divider derives lower voltages from the master reference. "
              "Use 0.1% tolerance resistors for &lt;0.2% division error."))
    )

    ch3 = chapter(3, "MATERIALS AND CONSTRUCTION", "ch3",
        section("3-1", "BILL OF MATERIALS",
            table("Components (LM336-2.5 Based)",
                  ["Qty", "Item", "Specification"],
                  [["1", "LM336Z-2.5", "2.5 V adjustable reference"],
                   ["1", "50k&Omega; trim pot", "Multiturn, Bourns 3296W"],
                   ["2", "10k&Omega; resistor", "0.1%, metal film, for trim network"],
                   ["4", "Precision resistor", "0.1% for divider outputs"],
                   ["2", "100 nF capacitor", "C0G, output bypass"],
                   ["4", "Banana jack", "Color coded per voltage"],
                   ["1", "9 V battery", "With snap connector"]]))
    )

    ch4 = chapter(4, "ASSEMBLY PROCEDURES", "ch4",
        procedure([
            "Connect LM336Z-2.5 with 1k&Omega; bias resistor from supply positive to anode (+). "
            "Connect cathode (K) to positive output terminal. Ground is common.",
            "Connect 50k&Omega; trimmer between ADJ pin and a midpoint in the 10k&Omega; divider "
            "for fine adjustment.",
            "Build precision divider: for 1.000 V from 2.500 V use R1=1.5k&Omega; and R2=1.0k&Omega; "
            "(1.000/2.500 = 0.400 ratio; choose R2/(R1+R2) = 0.400).",
            "Add 100 nF C0G capacitor at each output to suppress HF noise.",
            "Verify all connections before applying power.",
            "Apply 9 V. Measure reference output. Adjust trimmer to exactly 2.500 V "
            "using reference DMM."
        ])
    )

    ch5 = chapter(5, "CALIBRATION PROCEDURE", "ch5",
        procedure([
            "Allow 15-minute warm-up.",
            "Measure master reference output. Trim to 2.500 V &plusmn;1 mV.",
            "Measure all divider outputs. Record vs. calculated values.",
            "Example: if 1.000 V nominal divider output reads 1.002 V, "
            "divider error = +0.2%, acceptable for &plusmn;0.5% calibration.",
            "If divider error exceeds &plusmn;0.5%, verify resistor values and divider wiring."
        ])
    )

    ch6 = chapter(6, "TUNING AND ADJUSTMENT", "ch6",
        p("Use the trimmer pot to set master reference to exactly 2.500 V at room temperature. "
          "Readjust if ambient temperature changes more than 10&deg;C.")
    )

    ch7 = chapter(7, "VERIFICATION", "ch7",
        procedure([
            "Verify master reference against precision voltage reference (TM-CAL-006).",
            "Verify all divider outputs match calculations within &plusmn;0.5%.",
            "Log date, temperature, and all voltages."
        ])
    )

    appa = appchapter("appa", "A", "CALCULATIONS AND FORMULAS",
        formula("Resistor divider output",
                "V<sub>out</sub> = V<sub>ref</sub> &times; R2 / (R1 + R2)") +
        formula("Example: 1.000 V from 2.500 V",
                "R2/(R1+R2) = 0.400; e.g., R1=1.5k&Omega;, R2=1.0k&Omega;")
    )

    appb = appchapter("appb", "B", "EXAMPLE RESULTS",
        table("Zener Reference Outputs",
              ["Output", "Nominal (V)", "Measured (V)", "Error (mV)"],
              [["Master ref", "2.500", "2.500", "0"],
               ["Divider 1", "1.000", "1.001", "+1"],
               ["Divider 2", "1.250", "1.251", "+1"],
               ["Divider 3", "2.000", "2.001", "+1"]])
    )

    return tm_page(
        "TM-CAL-007",
        "ZENER + DIVIDER MULTI-VOLTAGE REFERENCE",
        "Temperature-Compensated Zener Diode with Precision Resistor Divider",
        "zener_divider_reference",
        ch1 + ch2 + ch3 + ch4 + ch5 + ch6 + ch7 + appa + appb
    )


# ═══════════════════════════════════════════════════════════════════════════════
# TM-CAL-008: Precision Resistor Network
# ═══════════════════════════════════════════════════════════════════════════════

def tm_resistor_network():
    ch1 = chapter(1, "GENERAL INFORMATION", "ch1",
        section("1-1", "SCOPE",
            p("This manual covers construction of a precision decade resistance box and "
              "fixed resistor standards for calibrating ohmmeters, resistance bridges, "
              "and ADC reference circuits. Resistors are available with &plusmn;0.01% "
              "tolerance and 2 ppm/&deg;C TC from Vishay and similar vendors."))
    )

    ch2 = chapter(2, "THEORY OF OPERATION", "ch2",
        section("2-1", "RESISTOR TYPES",
            p("For precision standards, use bulk metal foil (BMF) or metal film resistors. "
              "Wire-wound resistors are excellent for DC but introduce inductance at AC. "
              "Carbon film and carbon composition types are inadequate for calibration use "
              "due to high TC and poor long-term stability.") +
            table("Resistor Technology Comparison",
                  ["Type", "Tolerance", "TC (ppm/&deg;C)", "Stability", "Notes"],
                  [["Bulk metal foil (BMF)", "0.01%", "0.2&ndash;2", "Excellent", "Vishay Z201, best choice"],
                   ["Metal film", "0.1%", "10&ndash;25", "Good", "Yageo MFR, Multicomp"],
                   ["Wirewound", "0.1%", "5&ndash;10", "Excellent DC", "Inductive at AC"],
                   ["Thick film (SMD)", "1%", "100&ndash;200", "Fair", "Not suitable"],
                   ["Carbon film", "5%", "200&ndash;500", "Poor", "Do not use"]])) +
        section("2-2", "4-WIRE KELVIN CONNECTIONS",
            p("For resistances below 100 &Omega;, contact resistance in connectors and "
              "switch contacts introduces significant error. Use 4-wire (Kelvin) connections: "
              "separate current-force and voltage-sense terminals for each resistor position. "
              "See TM-CAL-010 for full Kelvin procedure."))
    )

    ch3 = chapter(3, "MATERIALS AND CONSTRUCTION", "ch3",
        section("3-1", "DECADE BOX BILL OF MATERIALS",
            table("Components",
                  ["Qty", "Item", "Specification"],
                  [["4", "Rotary switch", "12-position, make-before-break, low contact resistance"],
                   ["40", "Metal film resistors", "1 &Omega; through 10k&Omega; in decade steps, 0.1%"],
                   ["4", "Binding posts or BNC", "Input/output terminals"],
                   ["1", "Aluminum enclosure", "Hammond 1590BB or similar"],
                   ["1", "PC board", "For resistor mounting"]]))
    )

    ch4 = chapter(4, "ASSEMBLY PROCEDURES", "ch4",
        procedure([
            "Mount resistors in each decade: 1&Omega;&ndash;9&Omega; (nine 1&Omega;), "
            "10&Omega;&ndash;90&Omega; (nine 10&Omega;), etc.",
            "Use low-thermal-EMF solder (SAC305 or rosin-flux silver solder). "
            "Avoid no-clean flux which may cause thermal EMF at solder joints.",
            "Keep lead lengths short and symmetrical within each decade.",
            "Wire switch wipers in series: decade 4 wiper &rarr; decade 3 input &rarr; "
            "decade 2 input &rarr; decade 1 input &rarr; output terminal.",
            "Verify each decade with ohmmeter before final assembly.",
            "Mount in enclosure with clear dial markings."
        ])
    )

    ch5 = chapter(5, "CALIBRATION PROCEDURE", "ch5",
        procedure([
            "Measure each individual resistor value before assembly. Record actual values.",
            "After assembly, measure at each switch position using Fluke 15B+ or better DMM.",
            "For positions &lt;10 &Omega;, use 4-wire Kelvin measurement (TM-CAL-010).",
            "Record actual vs. nominal value at each position in calibration table.",
            "Apply correction factors in use: V<sub>actual</sub> = switch setting &times; (R<sub>meas</sub> / R<sub>nominal</sub>)."
        ])
    )

    ch6 = chapter(6, "TUNING AND ADJUSTMENT", "ch6",
        note("Decade boxes are not adjustable after construction. Select resistors with "
             "measured values closest to nominal at time of assembly. Keep spares for "
             "replacement if any resistor drifts."))

    ch7 = chapter(7, "VERIFICATION", "ch7",
        procedure([
            "Verify 1 &Omega;, 10 &Omega;, 100 &Omega;, 1k &Omega;, 10k &Omega; positions against "
            "NIST-certified reference resistors if available.",
            "For everyday use: compare to Wheatstone bridge null measurement (TM-CAL-009).",
            "Log calibration values; re-check annually."
        ])
    )

    appa = appchapter("appa", "A", "CALCULATIONS AND FORMULAS",
        formula("Series resistance total",
                "R<sub>total</sub> = R<sub>thousands</sub> + R<sub>hundreds</sub> + R<sub>tens</sub> + R<sub>ones</sub>") +
        formula("Contact resistance error (switch)",
                "error% = R<sub>contact</sub> / R<sub>nominal</sub> &times; 100 (significant below 10 &Omega;)")
    )

    appb = appchapter("appb", "B", "EXAMPLE RESULTS",
        table("Decade Box Calibration",
              ["Switch Setting (&Omega;)", "Measured (&Omega;)", "Error (%)", "Notes"],
              [["1", "1.002", "+0.2%", "Contact R dominant"],
               ["10", "10.05", "+0.5%", "Within 0.1% resistor spec + switch"],
               ["100", "100.08", "+0.08%", "Good"],
               ["1000", "999.7", "&minus;0.03%", "Excellent"],
               ["9999", "9998.2", "&minus;0.018%", "Excellent"]])
    )

    return tm_page(
        "TM-CAL-008",
        "PRECISION RESISTOR NETWORK (DECADE BOX)",
        "Switchable Precision Resistance Standards from 1 &Omega; to 9999 &Omega;",
        "precision_resistor_network",
        ch1 + ch2 + ch3 + ch4 + ch5 + ch6 + ch7 + appa + appb
    )


# ═══════════════════════════════════════════════════════════════════════════════
# TM-CAL-009: Wheatstone Bridge
# ═══════════════════════════════════════════════════════════════════════════════

def tm_wheatstone():
    ch1 = chapter(1, "GENERAL INFORMATION", "ch1",
        section("1-1", "SCOPE",
            p("This manual covers construction and use of a Wheatstone bridge for precision "
              "resistance measurement. At null balance, accuracy depends only on the ratio "
              "of precision resistors and is independent of meter calibration, supply voltage "
              "variation, or meter sensitivity. This method can measure resistance to "
              "&plusmn;0.01% with 0.01% ratio-arm resistors."))
    )

    ch2 = chapter(2, "THEORY OF OPERATION", "ch2",
        section("2-1", "BRIDGE BALANCE CONDITION",
            p("The Wheatstone bridge consists of four resistors in a diamond configuration "
              "with a battery across one diagonal and a null detector across the other.") +
            formula("Bridge balance (null condition)",
                    "R<sub>x</sub> = R<sub>s</sub> &times; R<sub>b</sub> / R<sub>a</sub>") +
            p("At null: R<sub>x</sub> is the unknown, R<sub>s</sub> is the standard (decade box), "
              "R<sub>a</sub> and R<sub>b</sub> are the ratio arms (precision fixed resistors). "
              "No current flows through the detector at balance; its internal resistance does "
              "not affect accuracy.") +
            table("Bridge Configurations",
                  ["Ratio R<sub>b</sub>/R<sub>a</sub>", "R<sub>x</sub> Range", "Notes"],
                  [["1:1 (equal arms)", "1 &Omega;&ndash;10k &Omega;", "Highest accuracy"],
                   ["10:1", "10&ndash;100k &Omega;", "Extends range"],
                   ["100:1", "100 &Omega;&ndash;1M &Omega;", "Higher ratio, lower sensitivity"]]))
    )

    ch3 = chapter(3, "MATERIALS AND CONSTRUCTION", "ch3",
        section("3-1", "BILL OF MATERIALS",
            table("Wheatstone Bridge Components",
                  ["Qty", "Item", "Specification"],
                  [["2", "Precision ratio-arm resistors (R<sub>a</sub>, R<sub>b</sub>)", "1k&Omega; 0.01% or 0.1%"],
                   ["1", "Decade resistance box", "TM-CAL-008 or commercial"],
                   ["1", "Galvanometer or null detector", "1 &mu;A sensitivity preferred"],
                   ["1", "1.5 V battery or USB power", "Stable DC supply"],
                   ["1", "Push-button switch", "For galvanometer protective circuit"],
                   ["4", "Binding posts", "For R<sub>x</sub> unknown connection"]]))
    )

    ch4 = chapter(4, "ASSEMBLY PROCEDURES", "ch4",
        procedure([
            "Wire bridge: battery+ &rarr; junction of R<sub>a</sub> and R<sub>x</sub>. "
            "Battery&minus; &rarr; junction of R<sub>b</sub> and R<sub>s</sub>.",
            "Galvanometer: one terminal to junction of R<sub>a</sub>/R<sub>b</sub>, "
            "other to junction of R<sub>x</sub>/R<sub>s</sub>.",
            "Add protective switch (normally open) in series with galvanometer. "
            "Close briefly to check null; prevents galvanometer deflection from extreme imbalance.",
            "Connect unknown R<sub>x</sub> to external binding posts.",
            "Mount all components in labeled enclosure."
        ])
    )

    ch5 = chapter(5, "CALIBRATION PROCEDURE", "ch5",
        procedure([
            "Connect unknown resistor to R<sub>x</sub> terminals.",
            "Set decade box (R<sub>s</sub>) to estimated value of unknown.",
            "Briefly close galvanometer switch. Observe deflection direction.",
            "Adjust R<sub>s</sub>: if galvanometer deflects right, R<sub>s</sub> is low (increase); "
            "if left, R<sub>s</sub> is high (decrease).",
            "Iterate until galvanometer reads zero with switch held closed (null balance).",
            "Calculate: R<sub>x</sub> = R<sub>s</sub> &times; (R<sub>b</sub> / R<sub>a</sub>).",
            "Read R<sub>s</sub> value from decade box. Apply ratio-arm correction if R<sub>b</sub> &ne; R<sub>a</sub>."
        ])
    )

    ch6 = chapter(6, "TUNING AND ADJUSTMENT", "ch6",
        p("Verify ratio arms R<sub>a</sub> and R<sub>b</sub> are equal by null test: "
          "replace R<sub>x</sub> with a shorting link, set R<sub>s</sub> = 0, "
          "check galvanometer reads null. If not, ratio arms are unequal; "
          "measure them with 4-wire method and apply correction factor.")
    )

    ch7 = chapter(7, "VERIFICATION", "ch7",
        procedure([
            "Measure a 0.01% certified reference resistor. Compare result against "
            "certified value. Error should be &lt;0.05%.",
            "Measure three different unknown resistors on the decade box range. "
            "Compare bridge result vs. direct 4-wire DMM measurement.",
            "Log date, ratio arm values, and results."
        ])
    )

    appa = appchapter("appa", "A", "CALCULATIONS AND FORMULAS",
        formula("Unknown resistance at null",
                "R<sub>x</sub> = R<sub>s</sub> &times; R<sub>b</sub> / R<sub>a</sub>") +
        formula("Equal-arm bridge (R<sub>a</sub> = R<sub>b</sub>)",
                "R<sub>x</sub> = R<sub>s</sub>") +
        formula("Bridge sensitivity (for galvanometer selection)",
                "I<sub>G</sub> = E &times; &Delta;R / (4 &times; R &times; R<sub>G</sub>) (small imbalance, equal arms)")
    )

    appb = appchapter("appb", "B", "EXAMPLE RESULTS",
        table("Bridge Measurement Log",
              ["R<sub>x</sub> (nominal)", "R<sub>s</sub> at null", "Ratio", "R<sub>x</sub> (measured)", "4-Wire Check"],
              [["100 &Omega;", "99.87 &Omega;", "1:1", "99.87 &Omega;", "99.86 &Omega;"],
               ["1k &Omega;", "999.2 &Omega;", "1:1", "999.2 &Omega;", "999.1 &Omega;"],
               ["10k &Omega;", "9982 &Omega;", "1:1", "9982 &Omega;", "9981 &Omega;"]])
    )

    return tm_page(
        "TM-CAL-009",
        "WHEATSTONE BRIDGE NULL METHOD",
        "Ratio-Based Precision Resistance Measurement Without Meter Calibration",
        "wheatstone_bridge",
        ch1 + ch2 + ch3 + ch4 + ch5 + ch6 + ch7 + appa + appb
    )


# ═══════════════════════════════════════════════════════════════════════════════
# TM-CAL-010: Kelvin 4-Wire Resistance Standard
# ═══════════════════════════════════════════════════════════════════════════════

def tm_kelvin():
    ch1 = chapter(1, "GENERAL INFORMATION", "ch1",
        section("1-1", "SCOPE",
            p("This manual covers the 4-wire Kelvin measurement method for precision "
              "resistance calibration. By separating current-forcing leads from "
              "voltage-sensing leads, lead and contact resistance is eliminated from "
              "the measurement. Applicable for resistances from 0.001 &Omega; to 10k &Omega;."))
    )

    ch2 = chapter(2, "THEORY OF OPERATION", "ch2",
        section("2-1", "KELVIN PRINCIPLE",
            p("In a standard 2-wire resistance measurement, the DMM forces current through "
              "the same leads it uses to measure voltage. Lead resistance (typically "
              "0.1&ndash;0.5 &Omega; per lead) adds directly to the measured value, "
              "introducing significant error for low resistances.") +
            p("In 4-wire Kelvin measurement, dedicated current-force leads (F+, F&minus;) "
              "carry the test current, and separate voltage-sense leads (S+, S&minus;) "
              "measure the voltage drop across the unknown resistor only. "
              "The voltage-sense leads carry negligible current (DMM input impedance "
              "&gt;10 M&Omega;), so their resistance does not affect the measurement.") +
            formula("Kelvin measurement",
                    "R<sub>x</sub> = V<sub>sense</sub> / I<sub>force</sub>"))
    )

    ch3 = chapter(3, "MATERIALS AND CONSTRUCTION", "ch3",
        section("3-1", "KELVIN PROBES AND FIXTURES",
            table("Equipment",
                  ["Item", "Description"],
                  [["DMM with 4-wire mode", "Fluke 15B+ does NOT have 4-wire; requires Fluke 87V or bench meter"],
                   ["Kelvin clip probes", "Paired force/sense clips per terminal (4 total)"],
                   ["Reference resistors", "Certified 0.01% standard resistors: 0.1, 1, 10, 100, 1k &Omega;"],
                   ["4-wire socket", "DUT mounting with separated F+, F&minus;, S+, S&minus; connections"]])) +
        note("The Fluke 15B+ is a 2-wire meter and cannot perform true Kelvin measurements. "
             "For 4-wire work, use a bench DMM with Hi-Force, Lo-Force, Hi-Sense, Lo-Sense terminals, "
             "or construct a current-source circuit with separate voltmeter.")
    )

    ch4 = chapter(4, "ASSEMBLY PROCEDURES", "ch4",
        section("4-1", "DIY KELVIN FIXTURE",
            procedure([
                "Build a simple current source: LM334 or constant-current diode set to "
                "1 mA through unknown resistor.",
                "Measure voltage across resistor with separate high-impedance voltmeter leads.",
                "R<sub>x</sub> = V / 0.001 A.",
                "Calibrate current source: force through 100 &Omega; standard, measure V, "
                "actual I = V/100."
            ]))
    )

    ch5 = chapter(5, "CALIBRATION PROCEDURE", "ch5",
        procedure([
            "Zero the meter on a shorting bar: connect F+ to S+ and F&minus; to S&minus;. "
            "The meter should read &lt;0.01 &Omega;. If not, apply offset null.",
            "Connect reference resistor in proper 4-wire configuration: "
            "F+ to one end, S+ to same end (inner), F&minus; to other end, S&minus; to same end (inner).",
            "Measure and record. Compare to certified value.",
            "Error ppm = (measured &minus; certified) / certified &times; 10<sup>6</sup>.",
            "Repeat for all decades: 0.1, 1, 10, 100, 1k &Omega;."
        ])
    )

    ch6 = chapter(6, "TUNING AND ADJUSTMENT", "ch6",
        note("Standard resistors are not adjustable. Select from characterized lot for best accuracy.")
    )

    ch7 = chapter(7, "VERIFICATION", "ch7",
        procedure([
            "Compare 10 &Omega; standard against Wheatstone bridge (TM-CAL-009).",
            "Agreement within 0.05% confirms both methods are consistent.",
            "Log all results; recertify standards annually."
        ])
    )

    appa = appchapter("appa", "A", "CALCULATIONS AND FORMULAS",
        formula("4-Wire resistance",
                "R<sub>x</sub> = V<sub>sense</sub> / I<sub>force</sub>") +
        formula("Contact resistance error in 2-wire measurement (example)",
                "Error = 2 &times; R<sub>lead</sub> / R<sub>x</sub>. For R<sub>lead</sub>=0.2&Omega;, R<sub>x</sub>=1&Omega;: error = 40%") +
        formula("Same measurement 4-wire",
                "Error &asymp; 0 (lead resistance eliminated)")
    )

    appb = appchapter("appb", "B", "EXAMPLE RESULTS",
        table("Kelvin vs 2-Wire Comparison",
              ["R<sub>x</sub> nominal", "2-Wire (DMM)", "4-Wire (Kelvin)", "Lead R (calculated)"],
              [["0.1 &Omega;", "0.42 &Omega;", "0.099 &Omega;", "0.16 &Omega;/lead"],
               ["1 &Omega;", "1.34 &Omega;", "1.001 &Omega;", "0.17 &Omega;/lead"],
               ["10 &Omega;", "10.34 &Omega;", "10.003 &Omega;", "0.17 &Omega;/lead"],
               ["100 &Omega;", "100.35 &Omega;", "100.02 &Omega;", "0.17 &Omega;/lead"]])
    )

    return tm_page(
        "TM-CAL-010",
        "4-WIRE KELVIN RESISTANCE STANDARD",
        "Elimination of Lead Resistance Error in Low-Resistance Measurements",
        "kelvin_resistance_standard",
        ch1 + ch2 + ch3 + ch4 + ch5 + ch6 + ch7 + appa + appb
    )


# ═══════════════════════════════════════════════════════════════════════════════
# TM-CAL-011: RC Time Constant Standard
# ═══════════════════════════════════════════════════════════════════════════════

def tm_rc_time():
    ch1 = chapter(1, "GENERAL INFORMATION", "ch1",
        section("1-1", "SCOPE",
            p("This manual covers construction and use of precision RC time-constant circuits "
              "as time and frequency calibration references for oscilloscopes and time-interval "
              "counters. With GPS-disciplined timing and C0G/NP0 capacitors, time constant "
              "accuracy of &plusmn;0.1% is achievable."))
    )

    ch2 = chapter(2, "THEORY OF OPERATION", "ch2",
        section("2-1", "RC EXPONENTIAL RESPONSE",
            formula("Time constant definition",
                    "&tau; = R &times; C") +
            formula("Voltage at time t (charging)",
                    "V(t) = V<sub>supply</sub> &times; (1 &minus; e<sup>&minus;t/&tau;</sup>)") +
            formula("Time to reach 63.2% of supply",
                    "t<sub>63%</sub> = &tau; = R &times; C") +
            formula("10&ndash;90% rise time",
                    "t<sub>r</sub> = 2.197 &times; &tau;") +
            table("RC Standard Values",
                  ["R (&Omega;)", "C", "&tau;", "f<sub>&minus;3dB</sub>", "Notes"],
                  [["1k", "100 nF", "100 &mu;s", "1592 Hz", "Scope timebase ref"],
                   ["1k", "1 &mu;F", "1 ms", "159.2 Hz", "Easy to measure"],
                   ["10k", "100 nF", "1 ms", "159.2 Hz", "Low current"],
                   ["100k", "10 nF", "1 ms", "159.2 Hz", "Very low current"]])) +
        section("2-2", "COMPONENT STABILITY",
            p("Capacitor types for precision RC standards:") +
            ul(["C0G/NP0: TC = 0 &plusmn;30 ppm/&deg;C. Best choice for precision RC standards.",
                "X7R: TC = &plusmn;15% over temperature. Not acceptable for standards.",
                "Polystyrene: TC &asymp; &minus;120 ppm/&deg;C. Very stable, good for audio range.",
                "Mica: TC &asymp; &plusmn;50 ppm/&deg;C. Good, but large and expensive."]) +
            note("Resistor types: use 0.1% metal film or better. See TM-CAL-008. "
                 "Avoid wirewound in AC circuits due to inductance."))
    )

    ch3 = chapter(3, "MATERIALS AND CONSTRUCTION", "ch3",
        section("3-1", "BILL OF MATERIALS",
            table("RC Time Constant Standard Kit",
                  ["Qty", "Item", "Specification"],
                  [["2", "Metal film resistor", "1k&Omega; 0.1%, for R1 and verification R"],
                   ["2", "C0G/NP0 capacitor", "100 nF &plusmn;1%, 50 V"],
                   ["1", "GPS 1PPS source", "See TM-CAL-002"],
                   ["1", "SPDT signal switch", "Low leakage, for step-function drive"],
                   ["1", "BNC connectors", "Input and output"]]))
    )

    ch4 = chapter(4, "ASSEMBLY PROCEDURES", "ch4",
        procedure([
            "Solder 1k&Omega; resistor and 100 nF C0G capacitor in series on small PCB. "
            "Mount cap vertically to minimize stray capacitance to ground.",
            "Connect output across capacitor to BNC output connector.",
            "Connect input (drive side of resistor) to BNC input connector via signal switch.",
            "Keep lead lengths short (&lt;15 mm) to minimize stray inductance and capacitance.",
            "Mount in small shielded enclosure. Ground the enclosure.",
            "Connect switch control line to GPS 1PPS output for step-function generation."
        ])
    )

    ch5 = chapter(5, "CALIBRATION PROCEDURE", "ch5",
        procedure([
            "Set oscilloscope timebase to 200 &mu;s/div (2 ms full screen).",
            "Apply a step function to the RC input (switch from 0V to +5V at GPS 1PPS).",
            "Display the exponential charge curve on scope.",
            "Measure time for output to reach 63.2% of +5V = 3.16 V. This is &tau;.",
            "Compare measured &tau; against calculated: &tau; = R &times; C = 1000 &times; 100&times;10<sup>&minus;9</sup> = 100 &mu;s.",
            "Error ppm = (measured &tau; &minus; calculated &tau;) / calculated &tau; &times; 10<sup>6</sup>.",
            "If error exceeds &plusmn;500 ppm: remeasure R and C with 4-wire method.",
            "The RC standard then calibrates the scope: adjust scope timebase until "
            "displayed &tau; matches calculated value."
        ])
    )

    ch6 = chapter(6, "TUNING AND ADJUSTMENT", "ch6",
        note("The RC standard is not adjustable post-construction. Select R and C with "
             "measured values. Calculate &tau;<sub>true</sub> = R<sub>meas</sub> &times; C<sub>meas</sub> "
             "using measured component values as the reference, not nominal values.")
    )

    ch7 = chapter(7, "VERIFICATION", "ch7",
        procedure([
            "Verify calculated &tau; by oscilloscope cursors at two time-constant multiples: "
            "at 2&tau;, V = 86.5% of supply. Both measurements should be consistent.",
            "Cross-check: apply 1 kHz square wave to RC input. Measure &minus;3 dB frequency "
            "with NanoVNA. f<sub>&minus;3dB</sub> = 1/(2&pi;&tau;) = 1/(2&pi;&times;100&mu;s) = 1592 Hz.",
            "Log: R measured, C measured, &tau; calculated, &tau; measured, scope timebase error %."
        ])
    )

    appa = appchapter("appa", "A", "CALCULATIONS AND FORMULAS",
        formula("Time constant", "&tau; = R &times; C") +
        formula("&minus;3 dB frequency", "f<sub>&minus;3dB</sub> = 1 / (2&pi;&tau;)") +
        formula("10&ndash;90% rise time", "t<sub>10-90</sub> = 2.197 &times; R &times; C") +
        formula("Scope timebase error from measured vs. expected &tau;",
                "error% = (t<sub>scope</sub> &minus; t<sub>calculated</sub>) / t<sub>calculated</sub> &times; 100%")
    )

    appb = appchapter("appb", "B", "EXAMPLE RESULTS",
        table("RC Time Constant Measurements",
              ["R (meas)", "C (meas)", "&tau; (calc)", "&tau; (scope)", "Error", "f-3dB (NanoVNA)"],
              [["999.8 &Omega;", "100.2 nF", "100.18 &mu;s", "100.0 &mu;s", "&minus;0.18%", "1589 Hz (calc 1589)"]])
    )

    return tm_page(
        "TM-CAL-011",
        "RC TIME-CONSTANT STANDARD",
        "Precision RC Circuit for Oscilloscope Timebase Calibration",
        "rc_time_constant_standard",
        ch1 + ch2 + ch3 + ch4 + ch5 + ch6 + ch7 + appa + appb
    )


# ═══════════════════════════════════════════════════════════════════════════════
# TM-CAL-012: LC Resonance Standard
# ═══════════════════════════════════════════════════════════════════════════════

def tm_lc_resonance():
    ch1 = chapter(1, "GENERAL INFORMATION", "ch1",
        section("1-1", "SCOPE",
            p("This manual covers construction of LC resonant circuits as fixed frequency "
              "standards and markers. When constructed with stable components (toroid inductors, "
              "NP0 capacitors), resonant frequency accuracy of &plusmn;0.1% is achievable. "
              "Used for calibrating frequency counters, VFOs, and antenna analyzers."))
    )

    ch2 = chapter(2, "THEORY OF OPERATION", "ch2",
        section("2-1", "SERIES AND PARALLEL RESONANCE",
            formula("Resonant frequency",
                    "f<sub>r</sub> = 1 / (2&pi; &times; &radic;(L &times; C))") +
            formula("Characteristic impedance",
                    "Z<sub>0</sub> = &radic;(L/C)") +
            formula("Q factor",
                    "Q = f<sub>r</sub> / BW = Z<sub>0</sub> / R<sub>loss</sub>") +
            table("Resonator Behavior",
                  ["Type", "At Resonance", "Impedance", "Use"],
                  [["Series RLC", "XL = XC", "Minimum (R only)", "Pass filter, dip meter"],
                   ["Parallel RLC", "XL = XC", "Maximum (&asymp;QZ<sub>0</sub>)", "Trap filter, grid dip"]]) +
            p("For NanoVNA calibration use: series resonator in a shunt fixture shows |S11| "
              "minimum (reflection minimum) at resonant frequency."))
    )

    ch3 = chapter(3, "MATERIALS AND CONSTRUCTION", "ch3",
        section("3-1", "BILL OF MATERIALS (10 MHz STANDARD)",
            table("Components",
                  ["Qty", "Item", "Specification"],
                  [["1", "Toroid core", "T50-6 (yellow) for 10&ndash;40 MHz; T50-2 (red) for 1&ndash;10 MHz"],
                   ["1", "Enameled wire", "#26 AWG for small toroids"],
                   ["1", "NP0/C0G capacitor", "Calculated value (see App A), &plusmn;1%"],
                   ["1", "BNC chassis connector", "52mm SMA or BNC"],
                   ["1", "Shielded enclosure", "Altoids tin or machined aluminum box"]]))
    )

    ch4 = chapter(4, "ASSEMBLY PROCEDURES", "ch4",
        procedure([
            "Calculate required inductance for target frequency and chosen capacitor. "
            "See Appendix A.",
            "Wind toroid: for T50-6 with #26 AWG, 15 turns &asymp; 0.8 &mu;H. "
            "Adjust turns for calculated inductance.",
            "Measure wound toroid inductance with NanoVNA or LCR meter before soldering.",
            "Solder toroid and NP0 capacitor in series. Keep lead lengths &lt;5 mm.",
            "Mount assembly in shielded enclosure with BNC connector. "
            "Short lead from series resonator to BNC center conductor; ground to BNC shell.",
            "Seal enclosure. Label with designed frequency."
        ])
    )

    ch5 = chapter(5, "CALIBRATION PROCEDURE", "ch5",
        procedure([
            "Connect resonator BNC to NanoVNA PORT 1. Set NanoVNA for |S11| display, "
            "centered on expected frequency, 2 MHz span.",
            "Identify the |S11| dip minimum: this is resonant frequency.",
            "Compare to expected: error% = (f<sub>meas</sub> &minus; f<sub>calc</sub>) / f<sub>calc</sub> &times; 100%.",
            "If frequency is low, capacitance is high or inductance high: reduce C or L. "
            "If high, increase C or L. Capacitor substitution is easier than rewinding.",
            "Final frequency should be within &plusmn;0.1% of design target.",
            "Verify against WWV/GPS reference for absolute accuracy."
        ])
    )

    ch6 = chapter(6, "TUNING AND ADJUSTMENT", "ch6",
        p("Parallel a small NP0 trimmer capacitor (5&ndash;30 pF) with the main capacitor "
          "to enable fine frequency adjustment. Trim with insulated tool to avoid hand "
          "capacitance effect.")
    )

    ch7 = chapter(7, "VERIFICATION", "ch7",
        procedure([
            "Measure Q factor: Q = f<sub>r</sub> / BW<sub>&minus;3dB</sub> where BW is the "
            "&minus;3 dB bandwidth from NanoVNA marker delta function.",
            "Q &gt;50 at 10 MHz confirms good inductor construction. Low Q indicates "
            "lossy core material or excess resistance in connections.",
            "Compare resonant frequency against GPS-disciplined frequency counter.",
            "Log: f<sub>designed</sub>, f<sub>measured</sub>, Q, date, temperature."
        ])
    )

    appa = appchapter("appa", "A", "CALCULATIONS AND FORMULAS",
        formula("Required L for target f and chosen C",
                "L = 1 / ((2&pi;f)<sup>2</sup> &times; C)") +
        formula("Example: f = 7.000 MHz, C = 100 pF",
                "L = 1 / ((2&pi;&times;7&times;10<sup>6</sup>)<sup>2</sup> &times; 100&times;10<sup>&minus;12</sup>) = 5.17 &mu;H") +
        formula("T50-6 toroid inductance formula",
                "L (&mu;H) = N<sup>2</sup> &times; A<sub>L</sub> / 10<sup>6</sup>, A<sub>L</sub> = 3.0 nH/turn<sup>2</sup> for T50-6")
    )

    appb = appchapter("appb", "B", "EXAMPLE RESULTS",
        table("LC Standard Build Log",
              ["Target f", "Core", "Turns", "C (NP0)", "f measured", "Q", "Error"],
              [["7.000 MHz", "T50-6", "17", "100 pF", "7.012 MHz", "72", "+0.17%"],
               ["10.000 MHz", "T50-6", "13", "100 pF", "10.005 MHz", "85", "+0.05%"],
               ["14.000 MHz", "T50-6", "10", "47 pF", "14.02 MHz", "91", "+0.14%"]])
    )

    return tm_page(
        "TM-CAL-012",
        "LC RESONANCE STANDARD",
        "Toroid-Capacitor Resonant Circuits as Fixed Frequency Markers",
        "lc_resonance_standard",
        ch1 + ch2 + ch3 + ch4 + ch5 + ch6 + ch7 + appa + appb
    )


# ═══════════════════════════════════════════════════════════════════════════════
# TM-CAL-013: Coax Capacitance Standard
# ═══════════════════════════════════════════════════════════════════════════════

def tm_coax_cap():
    ch1 = chapter(1, "GENERAL INFORMATION", "ch1",
        section("1-1", "SCOPE",
            p("This manual covers use of known-length coaxial cable sections as precision "
              "capacitance standards. Coaxial cable has a well-characterized capacitance per "
              "unit length; a measured length provides a calculable capacitance suitable for "
              "calibrating LCR meters, capacitance bridges, and DMM capacitance functions."))
    )

    ch2 = chapter(2, "THEORY OF OPERATION", "ch2",
        section("2-1", "COAX CAPACITANCE",
            p("The capacitance per unit length of coaxial cable is determined by the "
              "dielectric constant of the insulation and the conductor geometry:") +
            formula("Capacitance per unit length",
                    "C/l = 2&pi;&epsilon;<sub>0</sub>&epsilon;<sub>r</sub> / ln(D/d)") +
            p("where D = inner diameter of outer conductor, d = outer diameter of center "
              "conductor, &epsilon;<sub>r</sub> = relative permittivity of dielectric.") +
            table("Coax Capacitance Reference Values",
                  ["Cable Type", "pF/ft", "pF/m", "V<sub>F</sub>", "Z<sub>0</sub>"],
                  [["RG-58A/U", "28.5", "93.5", "0.66", "50 &Omega;"],
                   ["RG-174/U", "26.0", "85.3", "0.66", "50 &Omega;"],
                   ["RG-8X (mini-8)", "26.0", "85.3", "0.78", "50 &Omega;"],
                   ["RG-213/U", "29.0", "95.1", "0.66", "50 &Omega;"],
                   ["RG-6 (75 &Omega;)", "16.0", "52.5", "0.82", "75 &Omega;"],
                   ["LMR-400", "23.9", "78.4", "0.85", "50 &Omega;"]]))
    )

    ch3 = chapter(3, "MATERIALS AND CONSTRUCTION", "ch3",
        section("3-1", "STANDARD PREPARATION",
            p("Cut a section of RG-58 to an exact measured length. Accuracy of the capacitance "
              "standard depends directly on length measurement accuracy.") +
            procedure([
                "Measure RG-58 section to &plusmn;1 mm using steel rule.",
                "Install BNC connectors on both ends. Verify center pin fully seated and "
                "dielectric flush with BNC shoulder.",
                "Mark cable: length in cm, calculated capacitance, date.",
                "Short one end (connect center to shield at one BNC).",
                "The other BNC becomes the measurement port."
            ]))
    )

    ch4 = chapter(4, "ASSEMBLY PROCEDURES", "ch4",
        note("For best accuracy, cut cable square and ensure BNC compression/crimp is "
             "fully bottomed. Improperly installed connectors add 0.5&ndash;2 pF each.")
    )

    ch5 = chapter(5, "CALIBRATION PROCEDURE", "ch5",
        procedure([
            "Calculate expected capacitance: C = (pF/ft) &times; length(ft).",
            "Example: 1 m of RG-58 = 3.281 ft &times; 28.5 pF/ft = 93.5 pF.",
            "Measure with DMM capacitance function or LCR meter: connect shorting BNC "
            "to one end, measure at open BNC.",
            "Compare measured vs. calculated. Error reflects meter accuracy.",
            "Error = (measured &minus; calculated) / calculated &times; 100%.",
            "If meter reads 91.2 pF and calculated is 93.5 pF: error = &minus;2.5%.",
            "Apply correction factor to future capacitance measurements."
        ])
    )

    ch6 = chapter(6, "TUNING AND ADJUSTMENT", "ch6",
        p("The standard length is fixed. For a higher-accuracy standard, verify the actual "
          "capacitance/length ratio by measuring a long section (&ge;3 m) and back-calculating "
          "from a well-calibrated instrument. Record the actual pF/m for that cable spool.")
    )

    ch7 = chapter(7, "VERIFICATION", "ch7",
        procedure([
            "Prepare two lengths: 0.5 m and 1.0 m of same cable reel.",
            "1.0 m should measure exactly 2&times; the 0.5 m reading (within &plusmn;1%).",
            "This ratio check is independent of meter accuracy and verifies consistency.",
            "Log: cable type, length, calculated C, measured C, date."
        ])
    )

    appa = appchapter("appa", "A", "CALCULATIONS AND FORMULAS",
        formula("Total capacitance",
                "C<sub>total</sub> = (C/m) &times; l<sub>meters</sub> = (C/ft) &times; l<sub>feet</sub>") +
        formula("RG-58, 1.524 m (5 ft)",
                "C = 28.5 &times; 5 = 142.5 pF") +
        formula("Velocity factor and capacitance relationship",
                "V<sub>F</sub> = 1/&radic;&epsilon;<sub>r</sub>, so C/m = 1/(Z<sub>0</sub> &times; V<sub>F</sub> &times; c)")
    )

    appb = appchapter("appb", "B", "EXAMPLE RESULTS",
        table("Coax Capacitance Standards",
              ["Cable", "Length (m)", "C calc (pF)", "C meas (pF)", "Error", "Notes"],
              [["RG-58", "0.500", "46.8", "46.2", "&minus;1.3%", "DMM capacitance"],
               ["RG-58", "1.000", "93.5", "92.9", "&minus;0.6%", "LCR meter at 1 kHz"],
               ["RG-174", "1.000", "85.3", "84.8", "&minus;0.6%", "LCR meter at 1 kHz"]])
    )

    return tm_page(
        "TM-CAL-013",
        "COAX CABLE CAPACITANCE STANDARD",
        "Known-Length Coaxial Cable Sections as Precision Capacitance References",
        "coax_capacitance_standard",
        ch1 + ch2 + ch3 + ch4 + ch5 + ch6 + ch7 + appa + appb
    )


# ═══════════════════════════════════════════════════════════════════════════════
# TM-CAL-014: Dummy Load Power Standard
# ═══════════════════════════════════════════════════════════════════════════════

def tm_dummy_load():
    ch1 = chapter(1, "GENERAL INFORMATION", "ch1",
        section("1-1", "SCOPE",
            p("This manual covers construction and calibration of a 50 &Omega; non-reactive "
              "dummy load for measuring RF power. The load is used with a calibrated "
              "voltmeter or directional coupler to verify transmitter power output and "
              "calibrate wattmeters.")) +
        section("1-3", "SAFETY",
            warning("RF power above 1 watt represents significant electric field intensity "
                    "at close range. Do not contact the antenna output of a transmitter "
                    "during operation. Always connect dummy load before applying RF drive.") +
            caution("Resistor thermal rating: ensure dummy load power dissipation rating "
                    "exceeds transmitter peak output by at least 2&times;. "
                    "An oil-cooled load handles higher power but requires sealed construction."))
    )

    ch2 = chapter(2, "THEORY OF OPERATION", "ch2",
        section("2-1", "RESISTIVE LOAD PRINCIPLE",
            p("Power delivered to a resistive load equals P = V<sup>2</sup>/R = I<sup>2</sup>R. "
              "For a 50 &Omega; load driven by an RF source matched to 50 &Omega;:") +
            formula("Power from RMS voltage",
                    "P (W) = V<sub>RMS</sub><sup>2</sup> / 50") +
            formula("RMS voltage for given power",
                    "V<sub>RMS</sub> = &radic;(P &times; 50)") +
            table("Power &mdash; Voltage Reference",
                  ["Power (W)", "V<sub>RMS</sub> (V)", "V<sub>peak</sub> (V)", "I<sub>RMS</sub> (A)"],
                  [["0.1", "2.24", "3.16", "0.045"],
                   ["1.0", "7.07", "10.0", "0.141"],
                   ["5.0", "15.8", "22.4", "0.316"],
                   ["10.0", "22.4", "31.6", "0.447"],
                   ["100.0", "70.7", "100.0", "1.41"]]))
    )

    ch3 = chapter(3, "MATERIALS AND CONSTRUCTION", "ch3",
        section("3-1", "DRY LOAD (UP TO 25 W)",
            table("Components",
                  ["Qty", "Item", "Specification"],
                  [["10", "Non-inductive resistor", "500 &Omega; 5 W each, carbon film or metal oxide"],
                   ["1", "BNC or PL-259 chassis connector", "For RF connection"],
                   ["1", "Copper-clad PCB or ground plane", "For resistor mounting"],
                   ["1", "Aluminum enclosure", "With ventilation or heatsink"]])) +
        section("3-2", "CONSTRUCTION NOTES",
            p("Ten 500 &Omega; resistors in parallel = 50 &Omega; total. Each carries 1/10 "
              "of total power. For 25 W load, each resistor handles 2.5 W (rated 5 W). "
              "Mount resistors radially from center BNC connector with leads &lt;10 mm. "
              "Keep all leads equal length to maintain SWR flatness to &ge;100 MHz.") +
            note("Carbon composition resistors have good non-inductance but poor power "
                 "stability. Metal oxide film is preferred: non-inductive and stable. "
                 "Avoid wirewound at RF frequencies due to inductance."))
    )

    ch4 = chapter(4, "ASSEMBLY PROCEDURES", "ch4",
        procedure([
            "Drill BNC mounting hole in center of ground plane. Mount BNC chassis connector.",
            "Cut 10 resistor leads to equal length (15 mm each). Bend leads outward radially.",
            "Solder all resistor leads to BNC center pin simultaneously (one end) "
            "and to ground plane (other end). Use minimum solder.",
            "Verify 50 &Omega; with DMM at DC. Should read 50 &Omega; &plusmn;5%.",
            "Install in enclosure with ventilation holes (or heat-sink adhesive for "
            "high power).",
            "Apply label: '50 &Omega; 25 W DUMMY LOAD &mdash; NON-INDUCTIVE'."
        ])
    )

    ch5 = chapter(5, "CALIBRATION PROCEDURE", "ch5",
        section("5-1", "DC RESISTANCE VERIFICATION",
            procedure([
                "Measure DC resistance at BNC terminals with calibrated ohmmeter.",
                "Should read 50.0 &plusmn;2.5 &Omega; (5%).",
                "4-wire measurement preferred for accuracy (TM-CAL-010)."
            ])) +
        section("5-2", "RF POWER CALIBRATION",
            procedure([
                "Connect load to NanoVNA PORT 1. Measure |S11| from 1&ndash;100 MHz.",
                "SWR &lt;1.5:1 (|S11| &lt; &minus;14 dB) across full range confirms non-reactive construction.",
                "For power measurement: connect calibrated Bird 43 wattmeter or "
                "TinySA tracking generator source in series with load.",
                "Measure V<sub>RMS</sub> at load terminals: P<sub>calc</sub> = V<sup>2</sup>/50.",
                "Compare P<sub>calc</sub> against wattmeter reading. Agreement &lt;10% is adequate."
            ]))
    )

    ch6 = chapter(6, "TUNING AND ADJUSTMENT", "ch6",
        note("Resistance can be adjusted within &plusmn;10% by replacing individual "
             "resistors. For exact 50.00 &Omega;: measure all 10 resistors before assembly, "
             "select from characterized lot, combine to achieve closest total.")
    )

    ch7 = chapter(7, "VERIFICATION", "ch7",
        procedure([
            "Apply 5 W from QRP transmitter to dummy load for 60 seconds. "
            "Verify resistors do not reach unsafe temperature (&lt;100&deg;C surface).",
            "Measure |S11| after thermal cycling to confirm mechanical stability.",
            "Log: DC resistance, |S11| at 10 and 50 MHz, power test level, date."
        ])
    )

    appa = appchapter("appa", "A", "CALCULATIONS AND FORMULAS",
        formula("Parallel resistance",
                "R<sub>total</sub> = R<sub>each</sub> / N for N equal resistors") +
        formula("10 &times; 500 &Omega;",
                "R = 500/10 = 50 &Omega;") +
        formula("SWR from R<sub>load</sub> and Z<sub>0</sub>",
                "SWR = R<sub>load</sub>/Z<sub>0</sub> if R<sub>load</sub> &gt; Z<sub>0</sub>, else Z<sub>0</sub>/R<sub>load</sub>") +
        formula("Maximum power per resistor",
                "P<sub>each</sub> = P<sub>total</sub> / N")
    )

    appb = appchapter("appb", "B", "EXAMPLE RESULTS",
        table("Dummy Load Verification",
              ["Measurement", "Expected", "Measured", "Pass/Fail"],
              [["DC resistance", "50.0 &Omega;", "50.4 &Omega;", "Pass"],
               ["|S11| at 10 MHz", "&lt;&minus;14 dB", "&minus;28 dB", "Pass"],
               ["|S11| at 50 MHz", "&lt;&minus;14 dB", "&minus;22 dB", "Pass"],
               ["|S11| at 100 MHz", "&lt;&minus;14 dB", "&minus;17 dB", "Pass"],
               ["Thermal (5 W, 60 s)", "&lt;100&deg;C", "68&deg;C", "Pass"]])
    )

    return tm_page(
        "TM-CAL-014",
        "50-OHM DUMMY LOAD POWER STANDARD",
        "Non-Reactive Resistive Load for RF Power Measurement and Transmitter Testing",
        "dummy_load_power_standard",
        ch1 + ch2 + ch3 + ch4 + ch5 + ch6 + ch7 + appa + appb
    )


# ═══════════════════════════════════════════════════════════════════════════════
# TM-CAL-015: Fixed Attenuator Pad Standards
# ═══════════════════════════════════════════════════════════════════════════════

def tm_attenuator():
    ch1 = chapter(1, "GENERAL INFORMATION", "ch1",
        section("1-1", "SCOPE",
            p("This manual covers design, construction, and calibration of fixed attenuator "
              "pads for use as calibration standards. Pi and T-topology pads can be "
              "constructed from precision resistors to provide attenuation values from "
              "1 dB to 30 dB with &plusmn;0.1 dB accuracy and SWR &lt;1.2:1 to 500 MHz."))
    )

    ch2 = chapter(2, "THEORY OF OPERATION", "ch2",
        section("2-1", "PAD TOPOLOGIES",
            p("A matched attenuator pad maintains input and output impedance equal to Z<sub>0</sub> "
              "(typically 50 &Omega;) at all attenuation settings. This ensures that "
              "the pad does not disturb the impedance environment of connected equipment.") +
            formula("Attenuation ratio",
                    "k = 10<sup>A<sub>dB</sub>/20</sup>") +
            subsection("Pi Pad (50 &Omega;)",
                formula("Shunt resistors R1 = R3",
                        "R1 = Z<sub>0</sub> &times; (k+1) / (k&minus;1)") +
                formula("Series resistor R2",
                        "R2 = Z<sub>0</sub> &times; 2k / (k<sup>2</sup>&minus;1)")) +
            subsection("T Pad (50 &Omega;)",
                formula("Series resistors R1 = R3",
                        "R1 = Z<sub>0</sub> &times; (k&minus;1) / (k+1)") +
                formula("Shunt resistor R2",
                        "R2 = Z<sub>0</sub> &times; 2k / (k<sup>2</sup>&minus;1)")) +
            table("Pi Pad Standard Values (50 &Omega; system)",
                  ["A<sub>dB</sub>", "k", "R1 = R3 (&Omega;)", "R2 (&Omega;)", "Nearest 1% std"],
                  [["3", "1.412", "292.4", "17.6", "294 / 17.8"],
                   ["6", "1.995", "150.5", "37.4", "150 / 37.4"],
                   ["10", "3.162", "96.2", "71.2", "97.6 / 71.5"],
                   ["20", "10.00", "61.1", "247.5", "61.9 / 249"]])
        )
    )

    ch3 = chapter(3, "MATERIALS AND CONSTRUCTION", "ch3",
        section("3-1", "BILL OF MATERIALS (6 dB PAD EXAMPLE)",
            table("Components",
                  ["Qty", "Item", "Specification"],
                  [["2", "150 &Omega; resistor", "0.1%, 50 mW, metal film (R1, R3 shunt)"],
                   ["1", "37.4 &Omega; resistor", "0.1%, 50 mW, metal film (R2 series)"],
                   ["1", "SMA double-ended enclosure", "Mini-circuits style, or BNC-BNC barrel with PCB"],
                   ["1", "Small PCB", "50 &Omega; microstrip or air-wired"]])) +
        section("3-2", "CONSTRUCTION",
            p("Wire attenuator in pi configuration on small copper-clad PCB. "
              "Keep resistor leads &lt;5 mm. Ground both shunt resistors directly to "
              "the case/shield. Use SMA or BNC connectors on both ends."))
    )

    ch4 = chapter(4, "ASSEMBLY PROCEDURES", "ch4",
        procedure([
            "Measure all resistors with calibrated ohmmeter. Select parts closest to "
            "calculated values.",
            "Solder shunt resistors (R1, R3) directly from center conductor pads to "
            "ground plane. Keep vertical.",
            "Solder series resistor (R2) between input and output center pads. "
            "Keep horizontal, minimize lead length.",
            "Install in SMA or BNC barrel enclosure. Verify no short circuits before applying signal.",
            "Label case: impedance, attenuation in dB, date."
        ])
    )

    ch5 = chapter(5, "CALIBRATION PROCEDURE", "ch5",
        procedure([
            "Connect pad to TinySA: source on input, TinySA on output.",
            "Set TinySA to 0&ndash;200 MHz, measure insertion loss.",
            "Verify attenuation = A<sub>measured</sub> at multiple frequencies: "
            "1, 10, 30, 100, 200 MHz.",
            "Attenuation should be flat within &plusmn;0.2 dB from DC to 100 MHz.",
            "Measure input reflection: |S11| should be &lt;&minus;20 dB (SWR &lt;1.22) if resistor "
            "values are correct.",
            "Record actual attenuation vs. designed value. Correction = A<sub>designed</sub> &minus; A<sub>measured</sub>."
        ])
    )

    ch6 = chapter(6, "TUNING AND ADJUSTMENT", "ch6",
        p("Attenuator pads are not tunable after construction. If attenuation is off by "
          "&gt;0.5 dB, check individual resistor values and replace as needed. "
          "A 1% error in R2 causes approximately 0.1 dB error in attenuation at high attenuation values.")
    )

    ch7 = chapter(7, "VERIFICATION", "ch7",
        procedure([
            "Cascade two 6 dB pads: total should measure 12.0 &plusmn;0.2 dB.",
            "Compare measured attenuation against NanoVNA S21 measurement for cross-check.",
            "Log: attenuation at 1 MHz, 10 MHz, 100 MHz; SWR at each end; date."
        ])
    )

    appa = appchapter("appa", "A", "CALCULATIONS AND FORMULAS",
        formula("Attenuation voltage ratio",
                "k = 10<sup>A<sub>dB</sub>/20</sup>") +
        formula("Pi shunt resistors",
                "R<sub>shunt</sub> = Z<sub>0</sub>(k+1)/(k&minus;1)") +
        formula("Pi series resistor",
                "R<sub>series</sub> = Z<sub>0</sub> &times; 2k / (k<sup>2</sup>&minus;1)") +
        formula("Total power at input for measuring attenuation (dB from V ratio)",
                "A<sub>dB</sub> = 20 &times; log<sub>10</sub>(V<sub>in</sub>/V<sub>out</sub>)")
    )

    appb = appchapter("appb", "B", "EXAMPLE RESULTS",
        table("Attenuator Pad Verification",
              ["Pad", "Designed (dB)", "1 MHz (dB)", "10 MHz (dB)", "100 MHz (dB)", "SWR in"],
              [["6 dB Pi pad", "6.00", "6.02", "6.01", "6.05", "1.02"],
               ["10 dB Pi pad", "10.00", "9.97", "9.98", "10.03", "1.03"],
               ["20 dB Pi pad", "20.00", "20.1", "20.1", "20.2", "1.04"]])
    )

    return tm_page(
        "TM-CAL-015",
        "FIXED ATTENUATOR PAD STANDARDS",
        "Precision Pi and T-Pad Attenuators for RF Level Calibration",
        "attenuator_pad_standards",
        ch1 + ch2 + ch3 + ch4 + ch5 + ch6 + ch7 + appa + appb
    )


# ═══════════════════════════════════════════════════════════════════════════════
# TM-CAL-016: Known-Length Dipole Standard
# ═══════════════════════════════════════════════════════════════════════════════

def tm_dipole():
    ch1 = chapter(1, "GENERAL INFORMATION", "ch1",
        section("1-1", "SCOPE",
            p("This manual covers use of a physically measured half-wave dipole antenna as "
              "a frequency standard and antenna analyzer calibration reference. "
              "At resonance, the dipole exhibits a known impedance (~70 &Omega;) and "
              "a predictable resonant frequency based on physical length. This provides "
              "a quick sanity check for NanoVNA calibration and antenna analyzers."))
    )

    ch2 = chapter(2, "THEORY OF OPERATION", "ch2",
        section("2-1", "HALF-WAVE DIPOLE RESONANCE",
            formula("Free-space half-wave dipole resonant frequency",
                    "f<sub>r</sub> (MHz) = 150 / L<sub>m</sub>") +
            formula("With end effect (velocity factor ~0.95 for wire)",
                    "f<sub>r</sub> (MHz) = 142.5 / L<sub>m</sub>  or  f<sub>r</sub> (MHz) = 468 / L<sub>ft</sub>") +
            p("Where L is the <em>total</em> tip-to-tip length. At resonance, feedpoint "
              "impedance is approximately 70&ndash;73 &Omega; in free space; ground effects "
              "reduce this to 20&ndash;50 &Omega; near ground. Height above ground affects "
              "both resonant frequency (lowered slightly) and feedpoint impedance.") +
            table("Dipole Resonant Frequencies vs. Total Length",
                  ["Total Length (m)", "Total Length (ft)", "Resonant Freq (MHz)", "Band"],
                  [["20.07", "65.8", "7.1", "40m CW"],
                   ["10.03", "32.9", "14.2", "20m SSB"],
                   ["6.69", "21.9", "21.3", "15m"],
                   ["4.78", "15.7", "29.8", "10m"],
                   ["2.00", "6.56", "71.3", "4m/VHF"]])) +
        section("2-2", "NANOVANA MEASUREMENT",
            p("The NanoVNA measures complex impedance Z = R + jX. At resonance: X = 0 "
              "(or near zero). The resonant frequency is observed as the zero-crossing of "
              "the reactance curve, or as the minimum of |S11|."))
    )

    ch3 = chapter(3, "MATERIALS AND CONSTRUCTION", "ch3",
        section("3-1", "DIPOLE CONSTRUCTION",
            table("Materials",
                  ["Item", "Specification"],
                  [["Wire", "#14&ndash;#18 AWG bare copper or aluminum, measured to &plusmn;5 mm"],
                   ["Center insulator", "Ceramic or UHMW polyethylene"],
                   ["End insulators", "Ceramic strain insulators"],
                   ["Feed coax", "RG-58 or RG-174, 1/2 wavelength preferred (reduces common mode)"],
                   ["Ferrite choke", "5 turns #14 AWG through T240-43 or 3 FB-31-5621 beads"]]))
    )

    ch4 = chapter(4, "ASSEMBLY PROCEDURES", "ch4",
        procedure([
            "Calculate total dipole length for target frequency: L(m) = 142.5 / f(MHz).",
            "Cut wire to calculated length, measuring total tip-to-tip at final assembled length. "
            "Include connector pigtail in total length.",
            "Connect center of dipole to coax center; both halves are half of total length.",
            "Install ferrite choke balun at feedpoint to reduce common mode current.",
            "String dipole horizontally at height &ge;&lambda;/4 above ground for near-free-space behavior.",
            "Record exact measured length in calibration log."
        ])
    )

    ch5 = chapter(5, "CALIBRATION PROCEDURE", "ch5",
        procedure([
            "Connect dipole feed coax to NanoVNA PORT 1 (through ferrite choke).",
            "Set NanoVNA center frequency to designed f<sub>r</sub>, span &plusmn;10%.",
            "Display |S11| and reactance (X) vs. frequency.",
            "Find frequency where X = 0 (reactance zero crossing). This is f<sub>resonant</sub>.",
            "Compare f<sub>resonant</sub> to calculated: error% = (f<sub>meas</sub> &minus; f<sub>calc</sub>) / f<sub>calc</sub> &times; 100.",
            "If f<sub>meas</sub> &lt; f<sub>calc</sub>: dipole is electrically longer than expected "
            "(nearby objects, velocity factor, or ground proximity effects).",
            "If NanoVNA reads f<sub>meas</sub> correctly compared to GPS/WWV reference, "
            "the NanoVNA frequency axis is calibrated."
        ])
    )

    ch6 = chapter(6, "TUNING AND ADJUSTMENT", "ch6",
        p("To trim dipole to exact target frequency: shorten from tips in 5 cm increments "
          "if frequency is too low; add wire clips if too high. Re-measure after each "
          "adjustment. Height, proximity to trees/buildings, and ground conductivity "
          "all affect resonant frequency.")
    )

    ch7 = chapter(7, "VERIFICATION", "ch7",
        procedure([
            "Compare dipole f<sub>resonant</sub> against WWV frequency to verify NanoVNA "
            "frequency accuracy.",
            "Verify feedpoint R at resonance: R = 50&ndash;90 &Omega; depending on height. "
            "Values outside 20&ndash;120 &Omega; suggest calibration error or connection issue.",
            "Log: wire length, height, f<sub>calc</sub>, f<sub>meas</sub>, R at resonance, date."
        ])
    )

    appa = appchapter("appa", "A", "CALCULATIONS AND FORMULAS",
        formula("Total dipole length for resonance",
                "L<sub>total</sub> (m) = 142.5 / f (MHz)") +
        formula("Each leg length",
                "L<sub>leg</sub> = L<sub>total</sub> / 2") +
        formula("Velocity factor adjustment",
                "L<sub>actual</sub> = L<sub>free-space</sub> &times; V<sub>F</sub> (typically 0.95 for wire)")
    )

    appb = appchapter("appb", "B", "EXAMPLE RESULTS",
        table("Dipole Resonance Measurements",
              ["Target f", "L total (m)", "Height (m)", "f<sub>calc</sub> (MHz)", "f<sub>meas</sub> (MHz)", "R (&Omega;)"],
              [["7.100 MHz", "20.07", "6.1", "7.100", "7.032", "48", ],
               ["14.200 MHz", "10.03", "5.0", "14.200", "14.155", "62"],
               ["21.300 MHz", "6.69", "4.5", "21.300", "21.24", "61"]])
    )

    return tm_page(
        "TM-CAL-016",
        "KNOWN-LENGTH DIPOLE STANDARD",
        "Physically Measured Half-Wave Dipole as Antenna Analyzer Frequency Reference",
        "known_length_dipole_standard",
        ch1 + ch2 + ch3 + ch4 + ch5 + ch6 + ch7 + appa + appb
    )


# ═══════════════════════════════════════════════════════════════════════════════
# TM-CAL-017: Field Strength Reference
# ═══════════════════════════════════════════════════════════════════════════════

def tm_field_strength():
    ch1 = chapter(1, "GENERAL INFORMATION", "ch1",
        section("1-1", "SCOPE",
            p("This manual covers use of a calibrated transmitting antenna and power source "
              "to establish a reference electromagnetic field strength. The known field is "
              "used to calibrate receiving antennas, field strength meters, and EMC "
              "pre-compliance measurement setups.")) +
        section("1-3", "SAFETY",
            warning("Transmitting RF energy above 1 W can cause interference to other radio "
                    "services. Conduct tests on a licensed frequency (amateur bands if "
                    "licensed) or at power levels below 10 mW. Comply with all local "
                    "regulations.") +
            caution("Maintain minimum safe distance from transmitting antenna to avoid "
                    "RF burns or biohazard. For 100 mW and below, &gt;0.5 m is adequate. "
                    "For QRP (5 W), maintain &gt;3 m from antenna."))
    )

    ch2 = chapter(2, "THEORY OF OPERATION", "ch2",
        section("2-1", "FAR-FIELD ELECTRIC FIELD STRENGTH",
            formula("Far-field electric field strength",
                    "E (V/m) = &radic;(30 &times; P<sub>W</sub> &times; G) / d") +
            p("Where P<sub>W</sub> = transmitter power in watts, G = antenna gain (linear ratio, "
              "not dB), d = distance in meters from antenna.") +
            formula("Half-wave dipole gain (linear)",
                    "G<sub>dipole</sub> = 1.64 (2.15 dBi)") +
            formula("Isotropic antenna gain",
                    "G<sub>isotropic</sub> = 1.0 (0 dBi)") +
            table("Field Strength Reference Table (Dipole, G=1.64)",
                  ["Power (W)", "Distance (m)", "E (V/m)", "E (dB&mu;V/m)"],
                  [["0.1", "1", "7.01", "137"],
                   ["0.1", "3", "2.34", "127"],
                   ["1.0", "3", "7.40", "137"],
                   ["5.0", "3", "16.5", "144"],
                   ["5.0", "10", "4.96", "134"]])) +
        section("2-2", "NEAR FIELD VS. FAR FIELD",
            p("The far-field approximation (above) applies only beyond the Rayleigh distance.") +
            formula("Far-field boundary (electrically short antenna)",
                    "d<sub>far</sub> = &lambda; / (2&pi;) &asymp; 0.159 &lambda;") +
            p("At HF (3&ndash;30 MHz), &lambda; = 10&ndash;100 m, so far-field begins at "
              "1.6&ndash;16 m. Tests at &lt;3 m at HF may be in the near-field "
              "where the 1/d law does not hold."))
    )

    ch3 = chapter(3, "MATERIALS AND CONSTRUCTION", "ch3",
        section("3-1", "REFERENCE SETUP",
            table("Equipment",
                  ["Item", "Specification"],
                  [["QRP transmitter or signal generator", "Calibrated power output, e.g., 100 mW &plusmn;10%"],
                   ["Half-wave dipole (TX)", "See TM-CAL-016; length matched to test frequency"],
                   ["Calibrated dummy load + directional coupler", "Bird 43 or TinySA with coupler for power monitoring"],
                   ["Calibrated receive antenna", "Field-calibrated loop, calibrated dipole, or standard gain horn"],
                   ["Spectrum analyzer / receiver", "TinySA Ultra, for measuring received signal level"],
                   ["Steel tape measure", "For setting exact TX&ndash;RX distance"]]))
    )

    ch4 = chapter(4, "ASSEMBLY PROCEDURES", "ch4",
        procedure([
            "Set up TX dipole outdoors, clear of buildings, height &ge;1 m above ground.",
            "Position receive antenna at measured distance d (e.g., 3.000 m &plusmn;10 mm).",
            "Both antennas must be co-polarized (both horizontal or both vertical).",
            "Align antenna boresights for maximum received signal.",
            "Connect transmitter to TX dipole through directional coupler for power monitoring.",
            "Connect receive antenna to TinySA or receiver."
        ])
    )

    ch5 = chapter(5, "CALIBRATION PROCEDURE", "ch5",
        procedure([
            "Transmit CW at calibrated power P<sub>W</sub> (measured at TX antenna feedpoint).",
            "Record received signal level S<sub>rx</sub> (dBm or dB&mu;V/m) at known distance.",
            "Calculate expected E: E = &radic;(30 &times; P &times; G) / d.",
            "If receive antenna is a calibrated dipole: received power P<sub>rx</sub> = "
            "E<sup>2</sup> &times; G<sub>rx</sub> &times; &lambda;<sup>2</sup> / (480 &pi;<sup>2</sup>).",
            "Compare expected vs. measured received level. Difference is receive antenna "
            "(or receiver) calibration error.",
            "Iterate at multiple distances (3 m, 5 m, 10 m) to verify 1/d falloff."
        ])
    )

    ch6 = chapter(6, "TUNING AND ADJUSTMENT", "ch6",
        note("Field strength tests are sensitive to ground reflections (multipath). "
             "Conduct tests with antennas &ge;1.5 m above ground for reduced ground bounce. "
             "Early morning or late evening provides reduced atmospheric refraction effects "
             "at VHF and above.")
    )

    ch7 = chapter(7, "VERIFICATION", "ch7",
        procedure([
            "Verify inverse-square-law (1/d): doubling distance should reduce E by 6 dB.",
            "Measure at d = 3 m and d = 6 m: received signal should decrease by 6.0 &plusmn;1.0 dB.",
            "Deviation &gt;2 dB indicates near-field effects, ground reflections, or "
            "antenna orientation error.",
            "Log: frequency, TX power, TX antenna type, distance, measured E, expected E."
        ])
    )

    appa = appchapter("appa", "A", "CALCULATIONS AND FORMULAS",
        formula("Electric field (far field, from transmitted power)",
                "E (V/m) = &radic;(30 &times; P (W) &times; G) / d (m)") +
        formula("dB&mu;V/m from V/m",
                "E (dB&mu;V/m) = 20 &times; log<sub>10</sub>(E &times; 10<sup>6</sup>)") +
        formula("Effective aperture of half-wave dipole",
                "A<sub>eff</sub> = G &times; &lambda;<sup>2</sup> / (4&pi;) = 1.64 &times; &lambda;<sup>2</sup> / (4&pi;)") +
        formula("Received power from E field",
                "P<sub>rx</sub> (W) = E<sup>2</sup> &times; A<sub>eff</sub> / (120&pi;)")
    )

    appb = appchapter("appb", "B", "EXAMPLE RESULTS",
        table("Field Strength Measurements",
              ["Freq", "P<sub>TX</sub> (mW)", "d (m)", "E calc (V/m)", "E meas (V/m)", "Error (dB)"],
              [["14.2 MHz", "100", "3.0", "2.35", "2.28", "&minus;0.3 dB"],
               ["14.2 MHz", "100", "6.0", "1.17", "1.14", "&minus;0.2 dB"],
               ["14.2 MHz", "100", "10.0", "0.704", "0.690", "&minus;0.2 dB"]])
    )

    return tm_page(
        "TM-CAL-017",
        "FIELD STRENGTH REFERENCE",
        "Calibrated RF Field for Receive Antenna and Field-Strength Meter Calibration",
        "field_strength_reference",
        ch1 + ch2 + ch3 + ch4 + ch5 + ch6 + ch7 + appa + appb
    )


# ═══════════════════════════════════════════════════════════════════════════════
# GENERATION
# ═══════════════════════════════════════════════════════════════════════════════

GENERATORS = {
    'wwv_wwvh_reference':       tm_wwv,
    'gps_1pps_reference':       tm_gps1pps,
    'gpsdo_reference':          tm_gpsdo,
    'fm_broadcast_reference':   tm_fm_broadcast,
    'ac_mains_reference':       tm_ac_mains,
    'precision_voltage_reference': tm_prec_voltage,
    'zener_divider_reference':  tm_zener,
    'precision_resistor_network': tm_resistor_network,
    'wheatstone_bridge':        tm_wheatstone,
    'kelvin_resistance_standard': tm_kelvin,
    'rc_time_constant_standard': tm_rc_time,
    'lc_resonance_standard':    tm_lc_resonance,
    'coax_capacitance_standard': tm_coax_cap,
    'dummy_load_power_standard': tm_dummy_load,
    'attenuator_pad_standards': tm_attenuator,
    'known_length_dipole_standard': tm_dipole,
    'field_strength_reference': tm_field_strength,
}


if __name__ == '__main__':
    generated = 0
    errors = 0
    for dirname, gen_fn in GENERATORS.items():
        out_path = os.path.join(BASE_DIR, dirname, 'tm.html')
        try:
            html = gen_fn()
            with open(out_path, 'w', encoding='utf-8') as f:
                f.write(html)
            print(f'  OK  {dirname}/tm.html')
            generated += 1
        except Exception as e:
            print(f'  ERR {dirname}: {e}')
            errors += 1
    print(f'\nGenerated: {generated}  Errors: {errors}')
