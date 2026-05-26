#!/usr/bin/env python3
"""
generate_tgear_tms.py
Generate Navy TM-format tm.html for each of the 29 test_gear instrument directories.
Also updates each instrument's index.html to add a "View Technical Manual" link.
"""

import os
import re

BASE_DIR = os.path.dirname(os.path.abspath(__file__))

TM_CSS = """<style>
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
</style>"""

NAV_BLOCK = """        <nav>
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
                <ul><li><a href="../index.html">Test Equipment Library</a></li></ul>
            </details>
            <h3>Standards</h3>
            <ul>
                <li><a href="../../Calibration_Ideas/index.html">Calibration Standards</a></li>
            </ul>
            <details class="nav-group" open>
                <summary>Study &amp; Training</summary>
                <ul>
                    <li><a href="https://assets.doflagie.com/study/index.html">Ham Radio Flash Card Quiz</a></li>
                    <li><a href="https://assets.doflagie.com/study/morse.html">Morse Code Practice</a></li>
                </ul>
            </details>
        </nav>"""

FOOTER_BLOCK = """    <footer>
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
    </footer>"""

def make_tm(tm_number, title, subtitle, body_html):
    """Wrap body content in full Navy TM HTML page."""
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

{NAV_BLOCK}

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
            <li><a href="#ch1">Chapter 1 &mdash; General Information</a></li>
            <li><a href="#ch2">Chapter 2 &mdash; Theory of Operation</a></li>
            <li><a href="#ch3">Chapter 3 &mdash; Construction</a></li>
            <li><a href="#ch4">Chapter 4 &mdash; Assembly Procedures</a></li>
            <li><a href="#ch5">Chapter 5 &mdash; Calibration Procedure</a></li>
            <li><a href="#ch6">Chapter 6 &mdash; Tuning and Adjustment</a></li>
            <li><a href="#ch7">Chapter 7 &mdash; Verification</a></li>
            <li><a href="#appa">Appendix A &mdash; Calculations and Formulas</a></li>
            <li><a href="#appb">Appendix B &mdash; Example Results</a></li>
                    </ol>
                </div>

{body_html}

                <div class="back-link">
                    <a href="index.html">&larr; Back to {title.title()} Index</a> &nbsp;|&nbsp;
                    <a href="../index.html">&larr; Test Equipment Library</a>
                </div>
            </main>
        </div>
    </div>

{FOOTER_BLOCK}

</body>
</html>"""

# ============================================================
# INSTRUMENT DATA — keyed by directory name
# ============================================================

INSTRUMENTS = {

"antenna-analyzer": {
    "tm": "TM-TGEAR-001",
    "title": "ANTENNA ANALYZER",
    "subtitle": "RF Impedance and SWR Measurement Instrument",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers the design, construction, and calibration of a swept-frequency antenna analyzer. The antenna analyzer measures complex impedance (R + jX), SWR, and reflection coefficient across the HF/VHF spectrum (typically 1&ndash;60 MHz). It provides a simplified, portable alternative to a full vector network analyzer.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-018 (NanoVNA), TM-CAL-015 (Attenuator Pads), TM-CAL-014 (Dummy Load), TM-TGEAR-027 (SWR Meter).</p>
<h3 class="section-heading">1-3. SAFETY SUMMARY</h3>
<div class="tm-caution"><span class="admonition-label">CAUTION</span>Maximum input RF power is typically 0 dBm (1 mW). Never connect an antenna analyzer to a transmitter output. Transmitter power will destroy the bridge and DDS circuits.</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. DIRECTIONAL BRIDGE PRINCIPLE</h3>
<p>The antenna analyzer drives a Wheatstone-style RF bridge with an internal DDS (Direct Digital Synthesis) oscillator. The bridge compares the device under test (DUT) impedance against a precision 50 &Omega; reference. Quadrature sampling of the forward and reflected voltages yields the complex reflection coefficient &Gamma;.</p>
<div class="formula-block"><span class="formula-label">Reflection coefficient</span>&Gamma; = (Z<sub>L</sub> &minus; Z<sub>0</sub>) / (Z<sub>L</sub> + Z<sub>0</sub>)</div>
<div class="formula-block"><span class="formula-label">SWR from reflection coefficient</span>SWR = (1 + |&Gamma;|) / (1 &minus; |&Gamma;|)</div>
<h3 class="section-heading">2-2. DDS FREQUENCY SYNTHESIS</h3>
<p>A DDS chip (e.g., AD9850 or Si5351) generates a swept sine wave from 1 to 60 MHz. The MCU steps frequency in user-defined increments. Each step acquires I/Q samples from the bridge, computing |&Gamma;| and phase to reconstruct R and X.</p>
<h3 class="section-heading">2-3. IMPEDANCE EXTRACTION</h3>
<div class="formula-block"><span class="formula-label">Complex impedance from &Gamma;</span>Z = Z<sub>0</sub> &times; (1 + &Gamma;) / (1 &minus; &Gamma;)</div>
<p>The resistive component R and reactive component X are displayed directly. Positive X indicates inductive reactance; negative X indicates capacitive reactance.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<h3 class="section-heading">3-1. KEY SUBSYSTEMS</h3>
<table class="tm-table"><caption>Antenna Analyzer Block Diagram Subsystems</caption>
<thead><tr><th>Subsystem</th><th>Function</th><th>Typical Part</th></tr></thead>
<tbody>
<tr><td>DDS oscillator</td><td>Swept RF source</td><td>Si5351A or AD9850</td></tr>
<tr><td>RF bridge</td><td>Impedance sensing</td><td>4-resistor Wheatstone, 50 &Omega; ref</td></tr>
<tr><td>Detector</td><td>I/Q sampling</td><td>SA612 mixer or AD8302</td></tr>
<tr><td>MCU</td><td>Control and display</td><td>STM32F103 or Arduino Nano</td></tr>
<tr><td>Display</td><td>Data readout</td><td>TFT LCD 2.4&quot;</td></tr>
</tbody></table>
<h3 class="section-heading">3-2. RF BRIDGE CONSTRUCTION</h3>
<p>Wind a binocular ferrite core (BN-43-202) with 10 turns of 26 AWG enameled wire for the toroidal transformer. Use 51 &Omega; (1%, 0.1W) metal-film resistors for bridge arms. Keep all bridge wiring under 10 mm to minimize parasitics above 30 MHz.</p>
<h3 class="section-heading">3-3. BILL OF MATERIALS</h3>
<table class="tm-table"><caption>Core Components</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>1</td><td>Si5351A clock gen</td><td>8 kHz&ndash;160 MHz, I2C, MSOP-10</td></tr>
<tr><td>1</td><td>AD8302 gain/phase</td><td>&ndash;60 dBm input, 2.7 GHz, TSSOP-14</td></tr>
<tr><td>4</td><td>51 &Omega; 1% resistors</td><td>Metal film, 50 mW, 0603</td></tr>
<tr><td>1</td><td>BN-43-202 core</td><td>Binocular ferrite, Mix 43</td></tr>
<tr><td>1</td><td>STM32F103C8T6</td><td>Blue Pill MCU board</td></tr>
<tr><td>1</td><td>SMA connector (female)</td><td>PCB mount, antenna port</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Wind RF bridge transformer on BN-43-202 core: 10 bifilar turns, twist ratio 1:1.</li>
<li>Solder 51 &Omega; bridge resistors with leads under 5 mm. Verify balance with DMM before populating MCU.</li>
<li>Program Si5351 with calibration offset per manufacturer data sheet. Default correction 0 ppm.</li>
<li>Connect AD8302 output pins to MCU ADC inputs. Set MCU ADC reference to 3.3 V stable reference.</li>
<li>Power on. Verify DDS output with oscilloscope or frequency counter at SMA port before connecting antenna.</li>
<li>Perform open/short/load calibration (Section 5-1) before first measurements.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<h3 class="section-heading">5-1. ONE-PORT CALIBRATION (OSL)</h3>
<div class="tm-note"><span class="admonition-label">NOTE</span>OSL calibration removes systematic errors in the bridge and cable. Perform OSL at the measurement reference plane (antenna connector).</div>
<div class="procedure"><ol>
<li>Connect precision 50 &Omega; load (TM-CAL-014). Record |&Gamma;|<sub>LOAD</sub> at 1, 7, 14, 21, 28 MHz. Ideal: |&Gamma;| &lt; 0.01.</li>
<li>Connect precision open circuit (SMA cap or open connector). Record |&Gamma;|<sub>OPEN</sub> and phase. Ideal: |&Gamma;| = 1.0, phase = 0&deg;.</li>
<li>Connect precision short circuit (SMA shorting cap). Record |&Gamma;|<sub>SHORT</sub> and phase. Ideal: |&Gamma;| = 1.0, phase = &minus;180&deg;.</li>
<li>Enter calibration data into firmware. MCU computes error correction coefficients E<sub>D</sub>, E<sub>S</sub>, E<sub>R</sub>.</li>
<li>Verify calibration with 100 &Omega; resistor: analyzer should read R = 100, X = 0, SWR = 2.0:1.</li>
</ol></div>
<h3 class="section-heading">5-2. FREQUENCY AXIS CALIBRATION</h3>
<p>Tune to WWV 10.000 MHz. Compare displayed frequency against known. Adjust Si5351 crystal frequency correction (ppm trim in firmware) until display reads 10.000 MHz &plusmn; 100 Hz. Alternatively use GPS 1PPS reference per TM-CAL-002.</p>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<h3 class="section-heading">6-1. BRIDGE BALANCE</h3>
<p>With 50 &Omega; load connected, adjust bridge balance trimmer (if present) for minimum |&Gamma;|. Typical adjustment range &plusmn;5 &Omega; on the reference arm. An unbalanced bridge produces a frequency-dependent offset in the resistance readout.</p>
<h3 class="section-heading">6-2. FREQUENCY CORRECTION</h3>
<p>Si5351 crystal correction is stored as a signed integer (ppm &times; 100). Adjust in firmware to match GPS-locked reference. Typical correction range: &plusmn;100 ppm depending on crystal quality.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<div class="procedure"><ol>
<li>Connect 50 &Omega; load: SWR must read 1.00 &plusmn; 0.05, R = 50 &plusmn; 2 &Omega;, X = 0 &plusmn; 2 &Omega;.</li>
<li>Connect 100 &Omega; load: SWR must read 2.00 &plusmn; 0.1, R = 100 &plusmn; 5 &Omega;.</li>
<li>Connect open: SWR must read &gt;20:1, |&Gamma;| &gt; 0.98.</li>
<li>Connect known inductance (e.g., 10 &mu;H at 7 MHz = +j 440 &Omega;): verify X reading &plusmn; 10%.</li>
</ol></div>
<table class="tm-table"><caption>Antenna Analyzer Acceptance Specifications</caption>
<thead><tr><th>Parameter</th><th>Min</th><th>Typical</th><th>Max</th></tr></thead>
<tbody>
<tr><td>Frequency range</td><td>1 MHz</td><td>1&ndash;60 MHz</td><td>&mdash;</td></tr>
<tr><td>Frequency accuracy</td><td>&mdash;</td><td>&plusmn;50 ppm</td><td>&plusmn;200 ppm</td></tr>
<tr><td>SWR accuracy</td><td>&mdash;</td><td>&plusmn;2%</td><td>&plusmn;5%</td></tr>
<tr><td>Impedance range</td><td>1 &Omega;</td><td>1&ndash;5000 &Omega;</td><td>&mdash;</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Reflection coefficient magnitude</span>|&Gamma;| = (SWR &minus; 1) / (SWR + 1)</div>
<div class="formula-block"><span class="formula-label">Return loss</span>RL = &minus;20 &times; log<sub>10</sub>(|&Gamma;|) dB</div>
<div class="formula-block"><span class="formula-label">Impedance magnitude</span>|Z| = Z<sub>0</sub> &times; (1 + |&Gamma;|) / (1 &minus; |&Gamma;|)  [valid only at &Gamma; real]</div>
<div class="formula-block"><span class="formula-label">Resonant frequency from L and C</span>f<sub>r</sub> = 1 / (2&pi; &times; &radic;(L &times; C))</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Antenna Analyzer Calibration Verification Log</caption>
<thead><tr><th>Test Load</th><th>Freq (MHz)</th><th>R (&Omega;)</th><th>X (&Omega;)</th><th>SWR</th><th>Pass/Fail</th></tr></thead>
<tbody>
<tr><td>50 &Omega; load</td><td>7.0</td><td>50.1</td><td>0.3</td><td>1.01</td><td>Pass</td></tr>
<tr><td>100 &Omega; load</td><td>7.0</td><td>99.6</td><td>0.5</td><td>1.99</td><td>Pass</td></tr>
<tr><td>Open circuit</td><td>7.0</td><td>0.4</td><td>&gt;2000</td><td>&gt;40</td><td>Pass</td></tr>
<tr><td>10 &mu;H inductor</td><td>7.0</td><td>1.8</td><td>+438</td><td>&gt;8</td><td>Pass</td></tr>
</tbody></table>
""",
},

"audio-oscillator": {
    "tm": "TM-TGEAR-002",
    "title": "AUDIO OSCILLATOR",
    "subtitle": "Sine and Square Wave Generator, 20 Hz to 100 kHz",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers design, construction, and calibration of a laboratory-grade audio oscillator producing sine and square wave outputs from 20 Hz to 100 kHz. Primary applications include audio amplifier testing, filter response characterization, and modulation source for AM/FM modulators.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-012 (Function Generator), TM-TGEAR-020 (Oscilloscope), TM-CAL-006 (Voltage Reference).</p>
<h3 class="section-heading">1-3. SAFETY SUMMARY</h3>
<div class="tm-warning"><span class="admonition-label">WARNING</span>If this oscillator is powered from AC mains, use an isolation transformer to prevent ground loops and shock hazard. The output ground must not be connected to an earthed chassis while the oscillator ground floats at mains potential.</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. WIEN BRIDGE OSCILLATOR</h3>
<p>The classical Wien bridge uses an RC frequency-selective positive-feedback network around an op-amp. Oscillation occurs when the lag network phase shift is exactly 0&deg; (net loop phase = 360&deg;) and loop gain = 1.</p>
<div class="formula-block"><span class="formula-label">Wien bridge oscillation frequency</span>f = 1 / (2&pi; &times; R &times; C)</div>
<p>A thermistor or FET in the feedback controls amplitude automatically via AGC (automatic gain control). Without AGC, the oscillator clips into a square wave.</p>
<h3 class="section-heading">2-2. SQUARE WAVE GENERATION</h3>
<p>A Schmitt-trigger comparator following the sine output produces a rail-to-rail square wave. Rise time is determined by the comparator slew rate; for the LM393, tr &asymp; 200 ns, limiting usable bandwidth to &sim;500 kHz.</p>
<h3 class="section-heading">2-3. OUTPUT AMPLITUDE CONTROL</h3>
<p>An op-amp output stage (unity-gain buffer) drives a 10-turn cermet potentiometer as a continuous level control. Peak output voltage is typically &plusmn;5 V into high impedance, &plusmn;3 V into 600 &Omega; load.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<h3 class="section-heading">3-1. TOPOLOGY SELECTION</h3>
<p>For maximum spectral purity (THD &lt;0.1%), use a Wien bridge with lamp or JFET AGC. For simplicity, use a relaxation oscillator (555 or XR-2206 IC), which yields THD 0.5&ndash;1%.</p>
<h3 class="section-heading">3-2. BILL OF MATERIALS (Wien Bridge, 1 kHz&ndash;20 kHz Range)</h3>
<table class="tm-table"><caption>Core Components</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>1</td><td>TL072 op-amp</td><td>Low-noise, JFET input, DIP-8</td></tr>
<tr><td>2</td><td>10 k&Omega; resistors</td><td>1%, metal film, Wien bridge</td></tr>
<tr><td>2</td><td>15 nF capacitors</td><td>1%, film, Wien bridge (gives f = 1.06 kHz)</td></tr>
<tr><td>1</td><td>2N5457 JFET</td><td>AGC element, V<sub>GS(off)</sub> &asymp; &minus;2 V</td></tr>
<tr><td>1</td><td>LM393 comparator</td><td>Square wave output</td></tr>
<tr><td>1</td><td>10 k&Omega; 10-turn pot</td><td>Output level control</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Solder Wien bridge RC network components. Use 1% film capacitors; ceramic capacitors have poor temperature stability.</li>
<li>Install JFET AGC circuit. Verify V<sub>GS</sub> rests at &minus;0.5 to &minus;1.5 V during oscillation for linear AGC action.</li>
<li>Install output buffer op-amp and level potentiometer.</li>
<li>Power on with current-limited supply. Oscillation should start within 1 second. If it does not start, check that loop gain &gt; 1 by temporarily reducing feedback attenuation.</li>
<li>Connect output to oscilloscope. Verify waveform is sinusoidal, THD &lt;1% (no visible flat tops or crossover distortion).</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<h3 class="section-heading">5-1. FREQUENCY CALIBRATION</h3>
<div class="procedure"><ol>
<li>Connect oscillator output to frequency counter (TM-TGEAR-010). Set oscillator to 1 kHz range.</li>
<li>Trim R or C in Wien bridge until display reads 1000 &plusmn; 1 Hz. Document actual value.</li>
<li>Repeat at 10 kHz and 100 kHz. Log results vs. dial marking.</li>
<li>Cross-check with GPS-disciplined counter per TM-CAL-002 for highest accuracy.</li>
</ol></div>
<h3 class="section-heading">5-2. AMPLITUDE CALIBRATION</h3>
<div class="procedure"><ol>
<li>Set output level control to maximum. Connect precision DMM (TM-TGEAR-017, AC V range) across output.</li>
<li>Record V<sub>RMS</sub>. Typical: 2.0&ndash;5.0 V<sub>RMS</sub>.</li>
<li>Verify level control provides smooth attenuation from max to 0 with no dead zones.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Frequency range switching is accomplished by switching R or C values in decade steps (10&times; per range). Use rotary switch banks with matched film capacitors. AGC set-point (output amplitude) is adjusted by the ratio R4/R5 in the AGC loop; increasing R4 raises output amplitude.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>Audio Oscillator Acceptance Specifications</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Frequency range</td><td>20 Hz &ndash; 100 kHz (&plusmn;3%)</td></tr>
<tr><td>Sine THD</td><td>&lt;0.3% at 1 kHz (Wien bridge with JFET AGC)</td></tr>
<tr><td>Square wave rise time</td><td>&lt;5 &micro;s at 10 kHz</td></tr>
<tr><td>Output impedance</td><td>&lt;100 &Omega;</td></tr>
<tr><td>Frequency stability</td><td>&plusmn;0.5% over 0&ndash;50 &deg;C</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Wien bridge frequency</span>f = 1 / (2&pi;RC)  [R in &Omega;, C in F]</div>
<div class="formula-block"><span class="formula-label">THD approximation (clipping)</span>THD &asymp; 1 / (3 &times; n<sup>2</sup>)  for nth harmonic of clipped sine</div>
<div class="formula-block"><span class="formula-label">RMS from peak (sine wave)</span>V<sub>RMS</sub> = V<sub>peak</sub> / &radic;2</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Audio Oscillator Calibration Log</caption>
<thead><tr><th>Dial Setting</th><th>Measured f (Hz)</th><th>Error (%)</th><th>V<sub>RMS</sub></th><th>THD (%)</th></tr></thead>
<tbody>
<tr><td>100 Hz</td><td>101.2</td><td>+1.2</td><td>2.01</td><td>0.18</td></tr>
<tr><td>1 kHz</td><td>1002</td><td>+0.2</td><td>2.00</td><td>0.09</td></tr>
<tr><td>10 kHz</td><td>9985</td><td>&minus;0.15</td><td>1.98</td><td>0.22</td></tr>
<tr><td>100 kHz</td><td>99.6 kHz</td><td>&minus;0.4</td><td>1.87</td><td>0.81</td></tr>
</tbody></table>
""",
},

"capacitance-meter": {
    "tm": "TM-TGEAR-003",
    "title": "CAPACITANCE METER",
    "subtitle": "Standalone Capacitance Measurement Instrument, 1 pF to 10,000 &micro;F",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers the design, construction, and calibration of a standalone capacitance meter. The instrument measures capacitance from 1 pF to 10,000 &micro;F using charge/discharge timing or RC oscillator methods. Key applications include capacitor sorting, value verification, and electrolytic capacitor ESR estimation.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-015 (LCR Meter), TM-TGEAR-017 (Multimeter DMM), TM-CAL-011 (RC Time Constant).</p>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. CHARGE/DISCHARGE TIMING METHOD</h3>
<p>A precision current source charges the unknown capacitor C<sub>X</sub> until the voltage reaches a threshold V<sub>T</sub>. The charging time t is proportional to C<sub>X</sub>:</p>
<div class="formula-block"><span class="formula-label">Capacitance from current and time</span>C<sub>X</sub> = I &times; t / V<sub>T</sub></div>
<p>MCU timer captures t with &micro;s resolution. For large capacitors, a smaller current source (nA range) extends the range to 10,000 &micro;F.</p>
<h3 class="section-heading">2-2. RC OSCILLATOR METHOD</h3>
<p>A 555 or comparator oscillator with known R and unknown C<sub>X</sub> generates a frequency proportional to 1/C<sub>X</sub>. A frequency counter measures f, then computes C<sub>X</sub> = 1/(1.44 &times; R &times; f) for the 555 astable.</p>
<div class="formula-block"><span class="formula-label">555 astable frequency</span>f = 1.44 / ((R<sub>A</sub> + 2R<sub>B</sub>) &times; C)</div>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<h3 class="section-heading">3-1. PRECISION CURRENT SOURCE</h3>
<p>Use a JFET current source (BF245 or 2N5457) or a precision op-amp with resistor feedback. Target current: 1 mA for small capacitors (pF&ndash;nF), 10 &micro;A for &micro;F range, 100 nA for 10 mF range.</p>
<table class="tm-table"><caption>Current Source Ranges</caption>
<thead><tr><th>Range</th><th>Source Current</th><th>Max Voltage</th><th>Capacitance Range</th></tr></thead>
<tbody>
<tr><td>pF/nF</td><td>1 mA</td><td>5 V</td><td>1 pF &ndash; 100 nF</td></tr>
<tr><td>&micro;F small</td><td>100 &micro;A</td><td>5 V</td><td>100 nF &ndash; 10 &micro;F</td></tr>
<tr><td>&micro;F large</td><td>1 &micro;A</td><td>5 V</td><td>10 &micro;F &ndash; 10 mF</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Build and verify current source: connect DMM in series and verify I within 1% of design value at 2.5 V across dummy capacitor.</li>
<li>Connect MCU timer input to comparator output. Set comparator threshold to 2.5 V (half VCC).</li>
<li>Calibrate with known NPO/C0G capacitors (100 pF, 1 nF, 10 nF) before enabling auto-ranging.</li>
<li>Install zero-adjustment: short input and press ZERO to subtract parasitic wiring capacitance (typically 2&ndash;10 pF).</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>Zero the instrument: short test leads, press ZERO. Parasitic capacitance should be subtracted.</li>
<li>Connect C0G 100 pF &plusmn;1% reference capacitor. Reading must be 100 &plusmn; 5 pF.</li>
<li>Connect 1 nF &plusmn;1% reference. Reading must be 1.00 &plusmn; 0.05 nF.</li>
<li>Connect 100 nF &plusmn;1% film reference. Reading must be 100 &plusmn; 5 nF.</li>
<li>Connect 1 &micro;F &plusmn;1% film reference. Reading must be 1.00 &plusmn; 0.05 &micro;F.</li>
<li>If out of tolerance, adjust gain constant in firmware (k = I / V<sub>T</sub>).</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Accuracy depends on calibration of the current source (I) and the threshold voltage (V<sub>T</sub>). Adjust V<sub>T</sub> via resistor divider ratio. Adjust I via feedback resistor in op-amp current mirror. Temperature coefficient of film resistors (&lt;50 ppm/&deg;C) limits drift to &lt;0.5% over 25 &deg;C change.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>Capacitance Meter Acceptance Specifications</caption>
<thead><tr><th>Range</th><th>Accuracy</th><th>Resolution</th></tr></thead>
<tbody>
<tr><td>1 pF &ndash; 999 pF</td><td>&plusmn;2% &plusmn;2 pF</td><td>0.1 pF</td></tr>
<tr><td>1 nF &ndash; 999 nF</td><td>&plusmn;1% &plusmn;1 nF</td><td>0.1 nF</td></tr>
<tr><td>1 &micro;F &ndash; 999 &micro;F</td><td>&plusmn;2%</td><td>0.01 &micro;F</td></tr>
<tr><td>1 mF &ndash; 10 mF</td><td>&plusmn;5%</td><td>0.1 mF</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Capacitance from charge time</span>C = I &times; t / &Delta;V</div>
<div class="formula-block"><span class="formula-label">555 astable capacitance</span>C = 1.44 / (f &times; (R<sub>A</sub> + 2R<sub>B</sub>))</div>
<div class="formula-block"><span class="formula-label">Capacitive reactance</span>X<sub>C</sub> = 1 / (2&pi;fC)</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Capacitance Meter Calibration Log</caption>
<thead><tr><th>Reference C</th><th>Tolerance</th><th>Reading</th><th>Error</th><th>Pass/Fail</th></tr></thead>
<tbody>
<tr><td>100 pF C0G</td><td>&plusmn;1%</td><td>101 pF</td><td>+1.0%</td><td>Pass</td></tr>
<tr><td>1.0 nF C0G</td><td>&plusmn;1%</td><td>1.003 nF</td><td>+0.3%</td><td>Pass</td></tr>
<tr><td>100 nF film</td><td>&plusmn;1%</td><td>99.4 nF</td><td>&minus;0.6%</td><td>Pass</td></tr>
<tr><td>1.0 &micro;F film</td><td>&plusmn;1%</td><td>1.008 &micro;F</td><td>+0.8%</td><td>Pass</td></tr>
</tbody></table>
""",
},

"current-source": {
    "tm": "TM-TGEAR-004",
    "title": "PRECISION CURRENT SOURCE",
    "subtitle": "Constant-Current Reference for Resistance and 4-Wire Measurement",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers design, construction, and calibration of a precision constant-current source intended for Kelvin (4-wire) resistance measurement, electrochemical sensing, and current loop calibration. Output currents covered: 1 mA, 10 mA, 100 mA.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-017 (Multimeter DMM), TM-CAL-010 (Kelvin 4-Wire Resistance), TM-CAL-008 (Precision Resistor Network), TM-CAL-006 (Precision Voltage Reference).</p>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. VOLTAGE-TO-CURRENT CONVERSION</h3>
<p>A precision op-amp forces the voltage across a sense resistor R<sub>S</sub> equal to a reference voltage V<sub>REF</sub>. The resulting current I = V<sub>REF</sub>/R<sub>S</sub> flows through the load independent of load voltage (within compliance range).</p>
<div class="formula-block"><span class="formula-label">Output current</span>I<sub>OUT</sub> = V<sub>REF</sub> / R<sub>S</sub></div>
<h3 class="section-heading">2-2. COMPLIANCE VOLTAGE</h3>
<p>The current source maintains regulation only while V<sub>LOAD</sub> &lt; V<sub>SUPPLY</sub> &minus; V<sub>DROPOUT</sub>. For a 12 V supply and 1.5 V dropout, maximum load resistance at 10 mA is 1050 &Omega;.</p>
<div class="formula-block"><span class="formula-label">Maximum load resistance</span>R<sub>LOAD(max)</sub> = (V<sub>SUPPLY</sub> &minus; V<sub>DROPOUT</sub> &minus; V<sub>REF</sub>) / I<sub>OUT</sub></div>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<h3 class="section-heading">3-1. HOWLAND CURRENT PUMP</h3>
<p>The improved Howland current pump uses four matched 10 k&Omega; resistors around a precision op-amp (OPA2134) to produce a floating current source with high output impedance (&gt;10 M&Omega;). Resistor matching to 0.1% is critical.</p>
<table class="tm-table"><caption>Howland Pump Component List</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Spec</th></tr></thead>
<tbody>
<tr><td>4</td><td>10 k&Omega; resistors</td><td>0.1%, metal film, matched quartet</td></tr>
<tr><td>1</td><td>OPA2134PA op-amp</td><td>Low offset, low drift, DIP-8</td></tr>
<tr><td>1</td><td>REF02 or LM4040</td><td>5.000 V precision reference</td></tr>
<tr><td>1</td><td>10 &Omega; sense resistor</td><td>0.1%, 0.5 W, wirewound</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Match four 10 k&Omega; resistors: measure each with calibrated 4-wire DMM. Select pairs within 1 &Omega; of each other.</li>
<li>Solder op-amp on DIP socket to allow replacement without rework.</li>
<li>Install V<sub>REF</sub> chip. Measure V<sub>REF</sub> with calibrated DMM; record actual value for I<sub>OUT</sub> calculation.</li>
<li>Connect 100 &Omega; load. Measure load current with series precision shunt resistor + DMM. Adjust sense resistor if needed.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>Connect 1.000 &Omega; &plusmn;0.01% 4-terminal resistor as load (TM-CAL-010 standard).</li>
<li>Measure voltage across 1 &Omega; resistor with precision DMM. V = I &times; 1 &Omega;, so V numerically equals I in mA.</li>
<li>Adjust V<sub>REF</sub> trimmer or R<sub>S</sub> until measured current = 10.000 mA &plusmn; 0.01 mA.</li>
<li>Repeat across temperature range (0&ndash;50 &deg;C) if operating in field conditions.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Fine-tune output current by trimming R<sub>S</sub> or using a precision multi-turn potentiometer in series with the fixed R<sub>S</sub>. Temperature drift of 10 &Omega; wirewound sense resistors is &lt;10 ppm/&deg;C, limiting I<sub>OUT</sub> drift to &lt;10 ppm/&deg;C.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>Current Source Acceptance Specifications</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Output current accuracy</td><td>&plusmn;0.1% of set value</td></tr>
<tr><td>Output impedance</td><td>&gt;1 M&Omega; (Howland pump)</td></tr>
<tr><td>Temperature coefficient</td><td>&lt;25 ppm/&deg;C</td></tr>
<tr><td>Compliance voltage</td><td>0 to V<sub>SUPPLY</sub> &minus; 2 V</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Current source output</span>I<sub>OUT</sub> = V<sub>REF</sub> / R<sub>SENSE</sub></div>
<div class="formula-block"><span class="formula-label">Resistance under test (Kelvin)</span>R<sub>X</sub> = V<sub>measured</sub> / I<sub>OUT</sub></div>
<div class="formula-block"><span class="formula-label">Power dissipated in sense resistor</span>P = I<sup>2</sup> &times; R<sub>S</sub></div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Current Source Calibration Log</caption>
<thead><tr><th>Set Current</th><th>Load (&Omega;)</th><th>Measured I (mA)</th><th>Error</th><th>Pass/Fail</th></tr></thead>
<tbody>
<tr><td>1 mA</td><td>100</td><td>1.0008</td><td>+0.08%</td><td>Pass</td></tr>
<tr><td>10 mA</td><td>100</td><td>10.003</td><td>+0.03%</td><td>Pass</td></tr>
<tr><td>100 mA</td><td>10</td><td>99.91</td><td>&minus;0.09%</td><td>Pass</td></tr>
</tbody></table>
""",
},

"decade-capacitance-box": {
    "tm": "TM-TGEAR-005",
    "title": "DECADE CAPACITANCE BOX",
    "subtitle": "Switchable Precision Capacitance Standards, 1 pF to 1111.1 nF",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers construction and calibration of a decade capacitance box providing switchable capacitance in 1 pF steps from 1 pF to 1111.1 nF. Used to set known capacitance values for oscillator design, filter alignment, and calibrating capacitance meters.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-003 (Capacitance Meter), TM-TGEAR-015 (LCR Meter), TM-CAL-011 (RC Time Constant).</p>
<div class="tm-note"><span class="admonition-label">NOTE</span>Decade capacitance boxes have limited high-frequency performance due to lead inductance. Self-resonance of the switches and wiring limits usable range to &lt;1 MHz for most designs.</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. SWITCHED PARALLEL CAPACITORS</h3>
<p>Each decade consists of nine capacitors: one at value C (1&times;), one at 2C, one at 3C (or series/parallel combination), etc. Decade switches connect capacitors in parallel. Total capacitance is the sum of all switched-in capacitors.</p>
<div class="formula-block"><span class="formula-label">Parallel capacitance</span>C<sub>total</sub> = C<sub>1</sub> + C<sub>2</sub> + C<sub>3</sub> + ... + C<sub>n</sub></div>
<h3 class="section-heading">2-2. SELF-RESONANCE</h3>
<p>Lead inductance L<sub>lead</sub> (typically 10&ndash;20 nH per switch contact) creates a self-resonant frequency with each capacitor. Above f<sub>SRF</sub>, the capacitor appears inductive.</p>
<div class="formula-block"><span class="formula-label">Self-resonant frequency</span>f<sub>SRF</sub> = 1 / (2&pi; &times; &radic;(L<sub>lead</sub> &times; C))</div>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<table class="tm-table"><caption>Decade Capacitance Box Component List</caption>
<thead><tr><th>Decade</th><th>Values</th><th>Capacitor Type</th><th>Tolerance</th></tr></thead>
<tbody>
<tr><td>1 pF steps (1&ndash;9 pF)</td><td>9 &times; 1 pF</td><td>NP0/C0G ceramic</td><td>&plusmn;0.25 pF</td></tr>
<tr><td>10 pF steps (10&ndash;90 pF)</td><td>9 &times; 10 pF</td><td>NP0/C0G ceramic</td><td>&plusmn;1%</td></tr>
<tr><td>100 pF steps</td><td>9 &times; 100 pF</td><td>NP0/C0G ceramic</td><td>&plusmn;1%</td></tr>
<tr><td>1 nF steps</td><td>9 &times; 1 nF</td><td>NP0/C0G or film</td><td>&plusmn;1%</td></tr>
<tr><td>10 nF steps</td><td>9 &times; 10 nF</td><td>Film (polyester)</td><td>&plusmn;1%</td></tr>
<tr><td>100 nF steps</td><td>9 &times; 100 nF</td><td>Film (polypropylene)</td><td>&plusmn;1%</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Measure each capacitor individually with LCR meter. Sort by value; use closest pairs for critical decades.</li>
<li>Mount capacitors on rotary switch decks. Use phenolic or PTFE switch for high-Q performance.</li>
<li>Keep all wiring under 20 mm. Use solid copper bus bars for the common return where possible.</li>
<li>House in metal enclosure. Connect enclosure to circuit ground to screen external fields from small capacitances.</li>
<li>Mark enclosure with calibration date and traceable standard used.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>Zero the LCR meter with leads and test clips connected but box terminals open.</li>
<li>Set decade box to minimum (all switches to zero).</li>
<li>Zero-correct LCR meter for stray capacitance of box wiring (typically 2&ndash;10 pF).</li>
<li>Set each switch position 1&ndash;9 of each decade. Record measured vs. expected value. Accept &plusmn;2% or &plusmn;1 digit.</li>
<li>Record cumulative value (e.g., all switches at 9) and compare to sum of individual values.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Individual capacitor values cannot be adjusted after construction. Replace any capacitor measuring outside tolerance. For critical applications, measure total at each setting and record a correction table.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>Decade Capacitance Box Acceptance Specs</caption>
<thead><tr><th>Range</th><th>Accuracy</th></tr></thead>
<tbody>
<tr><td>1&ndash;9 pF</td><td>&plusmn;0.5 pF</td></tr>
<tr><td>10&ndash;99 pF</td><td>&plusmn;2%</td></tr>
<tr><td>100 pF &ndash; 999 nF</td><td>&plusmn;1%</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Self-resonant frequency of switched capacitor</span>f<sub>SRF</sub> = 1 / (2&pi;&radic;LC)  where L = lead inductance &asymp; 15 nH</div>
<div class="formula-block"><span class="formula-label">Maximum usable frequency (10% reactance error)</span>f<sub>max</sub> &asymp; f<sub>SRF</sub> / 3</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Decade Box Calibration Log</caption>
<thead><tr><th>Switch Setting</th><th>Expected (pF)</th><th>Measured (pF)</th><th>Error</th></tr></thead>
<tbody>
<tr><td>100 pF position 1</td><td>100</td><td>101</td><td>+1.0%</td></tr>
<tr><td>1 nF position 1</td><td>1000</td><td>998</td><td>&minus;0.2%</td></tr>
<tr><td>All 9s (max)</td><td>1111.1 nF</td><td>1108.5 nF</td><td>&minus;0.24%</td></tr>
</tbody></table>
""",
},

"decade-inductance-box": {
    "tm": "TM-TGEAR-006",
    "title": "DECADE INDUCTANCE BOX",
    "subtitle": "Switchable Precision Inductance Standards, 1 &micro;H to 111.1 mH",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers the construction and calibration of a decade inductance box providing switchable inductance from 1 &micro;H to 111.1 mH in decade steps. Used as a known inductance standard for filter design, inductor Q measurement, and LCR meter verification.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-014 (Inductance Meter), TM-TGEAR-015 (LCR Meter), TM-TGEAR-021 (Q Meter).</p>
<div class="tm-note"><span class="admonition-label">NOTE</span>Inductors in a decade box have mutual inductance coupling between coils on adjacent switch positions. Keep coils physically separated and orient axes perpendicular to minimize coupling errors (&lt;0.1%).</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. SERIES INDUCTANCE SWITCHING</h3>
<p>Unlike capacitors (which add in parallel), the decade box connects inductors in series. Each switched-in inductor adds its value to the total.</p>
<div class="formula-block"><span class="formula-label">Series inductance (no coupling)</span>L<sub>total</sub> = L<sub>1</sub> + L<sub>2</sub> + ... + L<sub>n</sub></div>
<h3 class="section-heading">2-2. Q AND WINDING RESISTANCE</h3>
<p>Each inductor has a winding resistance R<sub>DC</sub>. The Q factor at frequency f is:</p>
<div class="formula-block"><span class="formula-label">Q factor</span>Q = 2&pi;fL / R<sub>DC</sub></div>
<p>For air-core coils on phenolic forms, Q typically ranges from 50 to 200 at 100 kHz.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<table class="tm-table"><caption>Inductance Decade Specifications</caption>
<thead><tr><th>Decade</th><th>Values</th><th>Core</th><th>Wire Gauge</th></tr></thead>
<tbody>
<tr><td>1 &micro;H steps (1&ndash;9 &micro;H)</td><td>9 &times; 1 &micro;H</td><td>Air core, 8 mm form</td><td>26 AWG</td></tr>
<tr><td>10 &micro;H steps</td><td>9 &times; 10 &micro;H</td><td>Air core, 12 mm form</td><td>24 AWG</td></tr>
<tr><td>100 &micro;H steps</td><td>9 &times; 100 &micro;H</td><td>Powdered iron T50-2</td><td>24 AWG</td></tr>
<tr><td>1 mH steps</td><td>9 &times; 1 mH</td><td>Powdered iron T68-2</td><td>26 AWG</td></tr>
<tr><td>10 mH steps</td><td>9 &times; 10 mH</td><td>Ferrite core, &micro;<sub>i</sub>=125</td><td>28 AWG</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Wind each inductor using Neumann formula (see Appendix A). Measure inductance with LCR meter and adjust turns for &plusmn;2% of target.</li>
<li>Mount coils in metal enclosure with coil axes perpendicular to each other. Minimum spacing 20 mm.</li>
<li>Use rotary switches rated for inductance service (silver contacts, low contact resistance &lt;5 m&Omega;).</li>
<li>Connect ground lug to enclosure. Shield each decade section with thin copper sheet if mutual coupling is observed.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>Set decade box to position 0 (no inductors connected). Zero-subtract LCR meter.</li>
<li>Set each position 1&ndash;9 of each decade. Measure L and R<sub>DC</sub> at 10 kHz with LCR meter.</li>
<li>Record measured vs. design values. Accept &plusmn;2%.</li>
<li>Measure self-resonant frequency of highest value (10 mH) with VNA: confirm f<sub>SRF</sub> &gt; 50 kHz.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Adjust inductance by adding or removing turns. One turn on a T68-2 core at 100 &micro;H adds approximately 0.4 &micro;H. Use compression slugs (ferrite trimmer cores) for fine adjustment of small values without rewinding.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>Decade Inductance Box Acceptance Specs</caption>
<thead><tr><th>Range</th><th>Accuracy</th><th>Max R<sub>DC</sub></th></tr></thead>
<tbody>
<tr><td>1&ndash;9 &micro;H</td><td>&plusmn;5%</td><td>0.5 &Omega;</td></tr>
<tr><td>10&ndash;90 &micro;H</td><td>&plusmn;2%</td><td>2 &Omega;</td></tr>
<tr><td>100 &micro;H &ndash; 9.9 mH</td><td>&plusmn;2%</td><td>10 &Omega;</td></tr>
<tr><td>10&ndash;100 mH</td><td>&plusmn;3%</td><td>50 &Omega;</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Air-core solenoid inductance (Wheeler)</span>L (&micro;H) = (r<sup>2</sup> &times; N<sup>2</sup>) / (9r + 10l)  [r = radius cm, l = length cm, N = turns]</div>
<div class="formula-block"><span class="formula-label">Toroidal inductance</span>L = &mu;<sub>0</sub> &times; &mu;<sub>r</sub> &times; N<sup>2</sup> &times; A / l  [A = cross-section area m&sup2;, l = mean path length m]</div>
<div class="formula-block"><span class="formula-label">Q factor</span>Q = 2&pi;fL / R<sub>DC</sub></div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Decade Inductance Box Calibration Log</caption>
<thead><tr><th>Setting</th><th>Expected (&micro;H)</th><th>Measured (&micro;H)</th><th>R<sub>DC</sub> (&Omega;)</th><th>Pass/Fail</th></tr></thead>
<tbody>
<tr><td>100 &micro;H position 1</td><td>100</td><td>99.2</td><td>0.35</td><td>Pass</td></tr>
<tr><td>1 mH position 1</td><td>1000</td><td>1005</td><td>2.4</td><td>Pass</td></tr>
<tr><td>10 mH position 1</td><td>10000</td><td>9980</td><td>11.8</td><td>Pass</td></tr>
</tbody></table>
""",
},

"dip-meter-grid-dip-oscillator": {
    "tm": "TM-TGEAR-007",
    "title": "DIP METER / GRID DIP OSCILLATOR",
    "subtitle": "Resonance-Finding and Absorption Wavemeter",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers design, construction, and calibration of a grid dip oscillator (GDO), also called a dip meter. The GDO finds the resonant frequency of tuned circuits, inductors, and antennas without connection, by coupling energy inductively and detecting absorption resonance as a "dip" in meter reading.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-010 (Frequency Counter), TM-TGEAR-021 (Q Meter), TM-CAL-001 (WWV).</p>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. SELF-OSCILLATING VARIABLE TUNED CIRCUIT</h3>
<p>The GDO contains a variable-frequency oscillator (VFO) with a plug-in coil at the probe end. When the probe coil is brought near an external resonant circuit, energy transfer peaks sharply at the resonant frequency, causing the oscillator amplitude to dip. The dip is detected as a drop on a front-panel meter.</p>
<h3 class="section-heading">2-2. OSCILLATOR TOPOLOGY</h3>
<p>Classic circuits use a Colpitts or Hartley oscillator with a JFET (MPF102, 2N5457) or FET (J310). The variable capacitor CV tunes frequency; interchangeable plug-in coils cover different frequency bands.</p>
<div class="formula-block"><span class="formula-label">Colpitts oscillation frequency</span>f = 1 / (2&pi; &times; &radic;(L &times; C<sub>eq</sub>))  where C<sub>eq</sub> = C1C2/(C1+C2)</div>
<h3 class="section-heading">2-3. ABSORPTION MODE</h3>
<p>In absorption mode, the oscillator is turned off and the plug-in coil becomes a passive absorber. The meter then reads RF signal level absorbed from an external source. This mode locates unknown frequencies in a transmitter or circuit under test.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<h3 class="section-heading">3-1. COIL SETS</h3>
<table class="tm-table"><caption>Plug-In Coil Set for 1&ndash;100 MHz Coverage</caption>
<thead><tr><th>Coil</th><th>Turns</th><th>Form/Core</th><th>Freq Range</th></tr></thead>
<tbody>
<tr><td>L1</td><td>32</td><td>Air, 25 mm dia.</td><td>1.5&ndash;4 MHz</td></tr>
<tr><td>L2</td><td>16</td><td>Air, 20 mm dia.</td><td>3.5&ndash;9 MHz</td></tr>
<tr><td>L3</td><td>8</td><td>Air, 15 mm dia.</td><td>7&ndash;18 MHz</td></tr>
<tr><td>L4</td><td>4</td><td>Air, 12 mm dia.</td><td>15&ndash;40 MHz</td></tr>
<tr><td>L5</td><td>2</td><td>Air, 10 mm dia.</td><td>35&ndash;100 MHz</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Wind coils on polystyrene or PTFE forms. Space turns uniformly. Measure L with LCR meter.</li>
<li>Install variable capacitor (15&ndash;150 pF) on front panel with calibrated dial.</li>
<li>Solder Colpitts oscillator components. Use short leads; JFET gate lead &lt;10 mm.</li>
<li>Connect 50 &micro;A meter via detector diode (1N60 germanium or BAT85 Schottky) for maximum sensitivity at low oscillator levels.</li>
<li>Calibrate dial: couple to a frequency counter and mark known frequencies on the dial at 5 points per band.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>Install each coil. Connect frequency counter to oscillator output via 1 pF coupling capacitor.</li>
<li>Set CV to each end-stop and midpoint. Record f vs. dial position.</li>
<li>Mark scale at WWV 5, 10, 15 MHz by tuning for zero-beat with receiver tuned to WWV per TM-CAL-001.</li>
<li>Verify frequency readout &plusmn;0.5% across each coil range.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Frequency range of each coil can be adjusted by spreading or compressing turns (air core) or using a ferrite slug trimmer. Overlap between coil sets should be at least 10% of band width. Oscillator level is set by JFET drain resistor; adjust for full-scale meter deflection with coil in free space.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>GDO Acceptance Specifications</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Frequency range</td><td>1.5&ndash;100 MHz (5 coil sets)</td></tr>
<tr><td>Frequency accuracy</td><td>&plusmn;0.5% after calibration</td></tr>
<tr><td>Dip depth</td><td>&gt;20% full-scale deflection at Q = 20 circuit</td></tr>
<tr><td>Coupling distance</td><td>1&ndash;50 mm for typical antenna coils</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Resonant frequency from dip</span>f<sub>res</sub> = 1 / (2&pi;&radic;LC)</div>
<div class="formula-block"><span class="formula-label">Unknown capacitance from known L and measured f</span>C = 1 / ((2&pi;f)<sup>2</sup> &times; L)</div>
<div class="formula-block"><span class="formula-label">Unknown inductance from known C and measured f</span>L = 1 / ((2&pi;f)<sup>2</sup> &times; C)</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>GDO Calibration Log vs. WWV</caption>
<thead><tr><th>WWV Freq (MHz)</th><th>GDO Dial Reading</th><th>Error</th></tr></thead>
<tbody>
<tr><td>5.000</td><td>4.98</td><td>&minus;0.4%</td></tr>
<tr><td>10.000</td><td>10.01</td><td>+0.1%</td></tr>
<tr><td>15.000</td><td>14.97</td><td>&minus;0.2%</td></tr>
</tbody></table>
""",
},

"dummy-load-50-ohm": {
    "tm": "TM-TGEAR-008",
    "title": "DUMMY LOAD, 50 OHM",
    "subtitle": "Non-Reactive 50 &Omega; RF Load for Transmitter Testing",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers construction and calibration of a 50 &Omega; non-reactive dummy load. The dummy load absorbs transmitter output power for testing without radiating RF, protecting connected equipment and complying with FCC Part 97 requirement that transmitters be tested into a suitable non-radiating load.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-CAL-014 (Dummy Load Standard), TM-TGEAR-023 (RF Power Meter), TM-TGEAR-027 (SWR Meter), TM-CAL-015 (Attenuator Pads).</p>
<div class="tm-caution"><span class="admonition-label">CAUTION</span>Do not exceed the power rating of the dummy load. Resistors will fail open or short circuit if overloaded. At 100 W, a non-oil-cooled load requires forced air cooling or immersion in mineral oil.</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. RESISTIVE LOAD</h3>
<p>An ideal 50 &Omega; dummy load presents a purely resistive impedance at all frequencies, resulting in zero reflected power and SWR = 1.0:1. Real loads deviate at VHF/UHF due to resistor lead inductance and shunt capacitance.</p>
<div class="formula-block"><span class="formula-label">SWR from load impedance</span>SWR = |Z<sub>load</sub> + Z<sub>0</sub>| / |Z<sub>load</sub> &minus; Z<sub>0</sub>|  [simplified for purely real Z<sub>load</sub>]</div>
<h3 class="section-heading">2-2. PARALLEL RESISTOR NETWORKS</h3>
<p>Using multiple resistors in parallel improves high-frequency performance by dividing lead inductance among paths. Eight 400 &Omega; resistors in parallel = 50 &Omega; with 8&times; lower effective lead inductance.</p>
<div class="formula-block"><span class="formula-label">N resistors in parallel</span>R<sub>eq</sub> = R / N</div>
<h3 class="section-heading">2-3. OIL COOLING</h3>
<p>Mineral oil (transformer oil) dissipates approximately 2 W/cm&sup2; by natural convection. A 1-liter oil bath can continuously dissipate 100&ndash;200 W from a resistor array without exceeding 80 &deg;C oil temperature.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<h3 class="section-heading">3-1. BILL OF MATERIALS (50 W Air-Cooled Load)</h3>
<table class="tm-table"><caption>50 W Dummy Load Components</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>10</td><td>500 &Omega; resistors</td><td>1%, 10 W, wirewound non-inductive (Vishay/Dale)</td></tr>
<tr><td>1</td><td>SO-239 connector</td><td>UHF female, chassis mount</td></tr>
<tr><td>1</td><td>Aluminum heatsink</td><td>100 &times; 80 &times; 25 mm minimum</td></tr>
<tr><td>1</td><td>Copper groundplane PCB</td><td>Star topology, all resistors to center hub</td></tr>
</tbody></table>
<div class="tm-note"><span class="admonition-label">NOTE</span>Use non-inductive wirewound (bifilar-wound) resistors. Standard wirewound resistors are inductive and degrade SWR above 10 MHz.</div>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Arrange 10 resistors in a star pattern on copper groundplane. Each resistor from center hub to ground ring.</li>
<li>Keep all leads &lt;5 mm. Connect center hub to SO-239 center pin via very short, direct lead.</li>
<li>Bond copper groundplane directly to heatsink. Use thermal compound and #6 screws.</li>
<li>Bond SO-239 shell to heatsink (same ground potential).</li>
<li>Verify DC resistance: 10 &times; 500 &Omega; in parallel = 50.0 &Omega; &plusmn;0.5%.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>Measure DC resistance with 4-wire DMM. Record; target 50 &plusmn;1 &Omega;.</li>
<li>Connect to NanoVNA or antenna analyzer. Measure |S11| from 1 to 500 MHz.</li>
<li>Record SWR at 1, 7, 14, 50, 144, 440 MHz. All must be &lt;1.5:1.</li>
<li>Apply 10 W from QRP transceiver for 1 minute. Verify heatsink temperature &lt;60 &deg;C.</li>
<li>Re-measure S11 after thermal soak; should be unchanged.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>If SWR exceeds 1.2:1 above 100 MHz, add a small series compensation inductor or a shunt capacitor at the SO-239 center pin. Typical compensation: 1&ndash;2 nH series inductor (1 turn, 5 mm diameter air) at the SO-239 junction.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>Dummy Load Acceptance Specifications</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>DC resistance</td><td>50 &plusmn;1 &Omega;</td></tr>
<tr><td>SWR, 1&ndash;30 MHz</td><td>&lt;1.05:1</td></tr>
<tr><td>SWR, 30&ndash;144 MHz</td><td>&lt;1.2:1</td></tr>
<tr><td>SWR, 144&ndash;440 MHz</td><td>&lt;1.5:1</td></tr>
<tr><td>Continuous power</td><td>50 W (air), 200 W (oil-cooled)</td></tr>
<tr><td>Peak power (10 s)</td><td>5&times; continuous rating</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Parallel resistance</span>R<sub>total</sub> = R / N  (N identical resistors)</div>
<div class="formula-block"><span class="formula-label">Power dissipated</span>P = V<sup>2</sup> / R = I<sup>2</sup>R</div>
<div class="formula-block"><span class="formula-label">Return loss</span>RL = &minus;20 log<sub>10</sub>|&Gamma;| = &minus;20 log<sub>10</sub>|(Z&minus;50)/(Z+50)|</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Dummy Load SWR vs. Frequency</caption>
<thead><tr><th>Frequency</th><th>SWR</th><th>Return Loss (dB)</th></tr></thead>
<tbody>
<tr><td>1 MHz</td><td>1.01</td><td>46</td></tr>
<tr><td>14 MHz</td><td>1.02</td><td>40</td></tr>
<tr><td>50 MHz</td><td>1.04</td><td>35</td></tr>
<tr><td>144 MHz</td><td>1.08</td><td>29</td></tr>
<tr><td>440 MHz</td><td>1.22</td><td>22</td></tr>
</tbody></table>
""",
},

"field-strength-meter": {
    "tm": "TM-TGEAR-009",
    "title": "FIELD STRENGTH METER",
    "subtitle": "RF Field Intensity Measurement for Antenna Near-Field Evaluation",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers design, construction, and calibration of an RF field strength meter for qualitative and semi-quantitative measurement of electromagnetic field intensity from 100 kHz to 300 MHz. Primary uses: antenna pattern assessment, transmitter output verification, and antenna proximity comparison.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-023 (RF Power Meter), TM-TGEAR-024 (RF Probe/Detector), TM-TGEAR-028 (TinySA Spectrum Analyzer).</p>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. SHORT ANTENNA AND DETECTOR</h3>
<p>A short monopole or loop antenna intercepts a fraction of the incident electric or magnetic field. The induced voltage V<sub>ant</sub> across the antenna impedance is proportional to field strength E (V/m) for an electric-field antenna, or to flux density B for a loop.</p>
<div class="formula-block"><span class="formula-label">Induced EMF in short dipole</span>V<sub>OC</sub> = E &times; h<sub>eff</sub>  [h<sub>eff</sub> = effective height, meters]</div>
<h3 class="section-heading">2-2. DETECTOR CIRCUIT</h3>
<p>An RF diode detector (1N60 germanium or BAT85 Schottky) rectifies the antenna signal. A 50&ndash;100 &micro;A meter movement or op-amp with precision meter displays the rectified voltage. Response is proportional to E (linear detector) or E&sup2; (power detector).</p>
<h3 class="section-heading">2-3. LOOP VS. WHIP</h3>
<p>A small loop antenna responds to the magnetic field H. At MF (AM broadcast), a ferrite loop is sensitive and directional. A quarter-wave whip is non-directional and responds to the electric field E. At VHF, a whip or 5-element Yagi provides directional capability for azimuth mapping.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<table class="tm-table"><caption>Field Strength Meter Components</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>1</td><td>Whip antenna</td><td>Telescopic, 50&ndash;300 mm adjustable</td></tr>
<tr><td>1</td><td>BAT85 or 1N60</td><td>RF Schottky/germanium detector diode</td></tr>
<tr><td>1</td><td>100 k&Omega; resistor</td><td>Shunt across meter movement</td></tr>
<tr><td>1</td><td>100 &mu;F electrolytic</td><td>Filter capacitor for detector output</td></tr>
<tr><td>1</td><td>50 &mu;A meter movement</td><td>D'Arsonval type</td></tr>
<tr><td>1</td><td>Sensitivity pot 10 k&Omega;</td><td>Multi-turn for FSD calibration</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Solder detector diode with anode to antenna terminal. Keep lead length &lt;10 mm.</li>
<li>Connect 100 nF RF bypass capacitor from cathode to ground, followed by 100 k&Omega; shunt and meter in parallel.</li>
<li>Install sensitivity pot in series with meter: allows full-scale deflection at different field levels.</li>
<li>House in metal enclosure (Altoids tin or aluminum box). Connect enclosure to detector circuit ground.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>Set up known transmitter (TM-TGEAR-026 signal generator) at fixed distance d from antenna.</li>
<li>Calculate approximate field at distance d: E = &radic;(30 &times; P<sub>ERP</sub>) / d (V/m).</li>
<li>Adjust sensitivity pot until meter reads FSD at this computed field.</li>
<li>Mark scale: FSD = computed E value. Scale is linear for small-signal detector operation.</li>
<li>Cross-check with TinySA: point probe at same distance as field strength meter and compare relative readings.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>For frequency-selective operation, add a tuned circuit (L-C bandpass filter) between antenna and detector. This suppresses off-frequency signals and increases sensitivity at the tuned frequency by approximately 10 dB (Q factor gain).</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>Field Strength Meter Acceptance Specs</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Frequency range</td><td>100 kHz &ndash; 300 MHz</td></tr>
<tr><td>Dynamic range</td><td>20 dB (linear detector)</td></tr>
<tr><td>Absolute accuracy</td><td>&plusmn;6 dB (qualitative use)</td></tr>
<tr><td>Relative accuracy</td><td>&plusmn;1 dB (same frequency/geometry)</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">E field from EIRP at distance d</span>E (V/m) = &radic;(30 &times; P<sub>EIRP</sub>) / d</div>
<div class="formula-block"><span class="formula-label">Effective height of short monopole</span>h<sub>eff</sub> = h / 2  [h = physical height]</div>
<div class="formula-block"><span class="formula-label">Free-space path loss</span>FSPL = 20 log<sub>10</sub>(4&pi;d/&lambda;) dB</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Field Strength Relative Measurement (antenna comparison)</caption>
<thead><tr><th>Antenna</th><th>Distance (m)</th><th>Power (W)</th><th>Meter Reading (%FSD)</th></tr></thead>
<tbody>
<tr><td>Reference dipole</td><td>10</td><td>5</td><td>62</td></tr>
<tr><td>Vertical whip</td><td>10</td><td>5</td><td>58</td></tr>
<tr><td>Yagi (broadside)</td><td>10</td><td>5</td><td>91</td></tr>
</tbody></table>
""",
},

"frequency-counter": {
    "tm": "TM-TGEAR-010",
    "title": "FREQUENCY COUNTER",
    "subtitle": "Direct-Count and Reciprocal Frequency Measurement, 1 Hz to 400 MHz",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers design, construction, and calibration of a frequency counter from 1 Hz to 400 MHz using reciprocal counting techniques. A GPS 1PPS reference (TM-CAL-002) disciplines the timebase for &lt;1 ppm accuracy. The counter is the primary instrument for calibrating oscillators, GDOs, and signal generators.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-CAL-001 (WWV), TM-CAL-002 (GPS 1PPS), TM-CAL-003 (GPSDO), TM-CAL-004 (FM Broadcast).</p>
<h3 class="section-heading">1-3. SAFETY</h3>
<div class="tm-caution"><span class="admonition-label">CAUTION</span>Maximum input level at the counter front-end is typically 5 V<sub>pp</sub>. Inputs above this level will destroy the input comparator IC. Use an attenuator probe when measuring high-level signals.</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. DIRECT COUNTING</h3>
<p>A precision timebase (TCXO or GPS-disciplined OCXO) divides to produce a gate signal of period T<sub>gate</sub>. Pulses of the input signal are counted during T<sub>gate</sub>. The frequency is:</p>
<div class="formula-block"><span class="formula-label">Frequency from count and gate time</span>f = N / T<sub>gate</sub></div>
<p>Resolution = 1 count, so minimum resolution = 1/T<sub>gate</sub>. A 1-second gate gives 1 Hz resolution; a 10-second gate gives 0.1 Hz resolution.</p>
<h3 class="section-heading">2-2. RECIPROCAL COUNTING</h3>
<p>Reciprocal counters measure the period T of each input cycle, then compute f = 1/T. This provides constant relative resolution across all frequencies. Typical resolution: 8&ndash;9 significant digits at any frequency.</p>
<h3 class="section-heading">2-3. GPS TIMEBASE DISCIPLINE</h3>
<p>The GPS receiver's 1 PPS output represents 1.000000000 Hz (within 50 ns of UTC). By measuring the counter's reference oscillator against 1 PPS over a 100-second window, the oscillator frequency error is determined to &lt;0.01 ppm and corrected in firmware.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<table class="tm-table"><caption>Frequency Counter Core Components</caption>
<thead><tr><th>Subsystem</th><th>Component</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Input prescaler</td><td>74AC04 or MC100EL16</td><td>0&ndash;400 MHz divide-by-2/4/8</td></tr>
<tr><td>Main counter</td><td>STM32F103 TIM2</td><td>32-bit hardware timer, 72 MHz max</td></tr>
<tr><td>Reference oscillator</td><td>TCXO 10 MHz</td><td>&plusmn;2.5 ppm, HCMOS output</td></tr>
<tr><td>GPS interface</td><td>NEO-M8N + 3.3 V level shifter</td><td>1 PPS output, 3.3 V logic</td></tr>
<tr><td>Input amplifier</td><td>LMH6702</td><td>1.7 GHz GBP, 0.5 V hysteresis</td></tr>
<tr><td>Display</td><td>16&times;2 LCD or TFT</td><td>9-digit display resolution</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Assemble input conditioning: 50 &Omega; termination, BNC input, LMH6702 amplifier, 74AC04 prescaler.</li>
<li>Connect STM32 TIM2 input capture to prescaler output.</li>
<li>Connect GPS 1 PPS to STM32 external interrupt input (PA0).</li>
<li>Flash firmware (open-source reciprocal counter firmware, e.g., Barbouri / K6JCA design).</li>
<li>Verify TCXO output on oscilloscope: clean 10 MHz square wave, no ringing &gt;10%.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<h3 class="section-heading">5-1. GPS TIMEBASE CALIBRATION</h3>
<div class="procedure"><ol>
<li>Connect GPS antenna to NEO-M8N. Wait for 3D fix (LED indicator steady, or NMEA GPRMC shows "A").</li>
<li>Enable GPS discipline in firmware. Unit displays "GPS OK" when 1 PPS is received.</li>
<li>Accumulate 100 GPS edges: firmware computes TCXO correction factor. Typical correction: 0&ndash;5 ppm.</li>
<li>Apply correction. Accuracy is now better than 0.1 ppm (GPS-limited).</li>
</ol></div>
<h3 class="section-heading">5-2. INPUT SENSITIVITY CALIBRATION</h3>
<div class="procedure"><ol>
<li>Apply 10 MHz signal from calibrated signal generator at &minus;20 dBm. Counter must trigger and display 10.000000 MHz.</li>
<li>Reduce input to &minus;30 dBm. Counter should still trigger. &minus;35 dBm is the minimum sensitivity specification.</li>
<li>Test at 400 MHz: apply &minus;10 dBm, counter must display correctly through prescaler.</li>
</ol></div>
<h3 class="section-heading">5-3. TRIGGER LEVEL CHECK</h3>
<p>Set trigger level to +0.5 V and verify count of asymmetric pulse train. Adjust comparator threshold resistor divider for center-of-threshold operation on sine inputs.</p>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>TCXO trim is performed in firmware: compare measured 1 PPS interval to ideal 10,000,000 counts. Fractional error is applied as a multiplicative correction. The trim is stored in non-volatile memory and survives power-off.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>Frequency Counter Acceptance Specifications</caption>
<thead><tr><th>Parameter</th><th>Without GPS</th><th>With GPS Discipline</th></tr></thead>
<tbody>
<tr><td>Timebase accuracy</td><td>&plusmn;2.5 ppm (TCXO)</td><td>&lt;0.1 ppm</td></tr>
<tr><td>Frequency range</td><td colspan="2">1 Hz &ndash; 400 MHz</td></tr>
<tr><td>Resolution (1 s gate)</td><td colspan="2">1 Hz</td></tr>
<tr><td>Resolution (reciprocal)</td><td colspan="2">8&ndash;9 digits</td></tr>
<tr><td>Input sensitivity</td><td colspan="2">&minus;35 dBm min (50 &Omega;)</td></tr>
<tr><td>Max input (no damage)</td><td colspan="2">+10 dBm (50 &Omega;)</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Direct count frequency</span>f = N / T<sub>gate</sub>  [N = count, T<sub>gate</sub> = gate period]</div>
<div class="formula-block"><span class="formula-label">Reciprocal period measurement</span>T = (Ref counts / Input counts) / f<sub>ref</sub></div>
<div class="formula-block"><span class="formula-label">Frequency error from GPS correction</span>&Delta;f/f = (T<sub>measured</sub> &minus; T<sub>GPS</sub>) / T<sub>GPS</sub> [ppm]</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Frequency Counter vs. GPS Reference</caption>
<thead><tr><th>Input Signal</th><th>Counter Reading</th><th>True Frequency</th><th>Error (ppm)</th></tr></thead>
<tbody>
<tr><td>WWV 10 MHz</td><td>10,000,001 Hz</td><td>10,000,000.000</td><td>+0.10</td></tr>
<tr><td>FM broadcast 98.3 MHz</td><td>98,299,987 Hz</td><td>98,300,000</td><td>&minus;0.13</td></tr>
<tr><td>TCXO 10 MHz (pre-GPS)</td><td>10,000,025 Hz</td><td>10,000,000.000</td><td>+2.5</td></tr>
</tbody></table>
""",
},

"frequency-standard-gpsdo": {
    "tm": "TM-TGEAR-011",
    "title": "FREQUENCY STANDARD / GPSDO",
    "subtitle": "GPS-Disciplined Oscillator Reference, 10 MHz Output",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers design, construction, and calibration of a GPS-Disciplined Oscillator (GPSDO). The GPSDO produces a 10 MHz reference signal traceable to UTC/GPS time, enabling frequency measurement to &lt;0.1 ppm. It serves as the master frequency standard for all other test equipment calibration in this library.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-CAL-002 (GPS 1PPS), TM-CAL-003 (GPSDO), TM-TGEAR-010 (Frequency Counter).</p>
<div class="tm-note"><span class="admonition-label">NOTE</span>GPS accuracy depends on atmospheric conditions and satellite geometry. Under normal conditions, GPS 1PPS is accurate to &lt;100 ns (0.1 &micro;s). When GPS signal is lost, the OCXO holds frequency within &plusmn;0.5 ppm for up to 24 hours (holdover).</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. PHASE-LOCKED LOOP DISCIPLINE</h3>
<p>A voltage-controlled oscillator (VCXO or OCXO at 10 MHz) is phase-locked to the GPS 1 PPS signal. The DPLL (digital phase-locked loop) in firmware measures the phase offset between the oscillator's divided-down 1 Hz tick and the GPS 1 PPS. It applies a correction to the VCXO control voltage (EFC) to eliminate the error.</p>
<div class="formula-block"><span class="formula-label">Phase error</span>&phi;<sub>err</sub> = (T<sub>osc</sub> &minus; T<sub>GPS</sub>) / T<sub>GPS</sub></div>
<div class="formula-block"><span class="formula-label">EFC voltage update (PI controller)</span>V<sub>EFC</sub> = V<sub>EFC(prev)</sub> + K<sub>p</sub> &times; &phi;<sub>err</sub> + K<sub>i</sub> &times; &Sigma;&phi;<sub>err</sub></div>
<h3 class="section-heading">2-2. LOOP FILTER DESIGN</h3>
<p>The DPLL time constant &tau; is set to 100&ndash;300 seconds to attenuate GPS jitter (1&ndash;10 &micro;s peak) while tracking low-frequency oscillator drift. Shorter &tau; tracks GPS noise; longer &tau; improves phase noise of output.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<table class="tm-table"><caption>GPSDO Block Diagram Components</caption>
<thead><tr><th>Block</th><th>Part</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>GPS receiver</td><td>u-blox NEO-M8N</td><td>GPS/GLONASS, 1 PPS, 3.3 V, SMA antenna</td></tr>
<tr><td>OCXO</td><td>MORION MV89 or ISOTEMP</td><td>10 MHz, EFC 0&ndash;5 V, &plusmn;5 ppm initial</td></tr>
<tr><td>Divider</td><td>74HC390 (decade) or MCU TIM</td><td>Divide 10 MHz to 1 Hz reference</td></tr>
<tr><td>DAC</td><td>MCP4725 12-bit</td><td>EFC control voltage, I2C, 0&ndash;5 V</td></tr>
<tr><td>MCU</td><td>STM32F103C8T6</td><td>PI loop, display, UART logging</td></tr>
<tr><td>Output buffer</td><td>74AC04</td><td>Fan-out to 4 instruments, 50 &Omega; outputs</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Install NEO-M8N on 3.3 V power rail. Verify 1 PPS output present (oscilloscope shows 1 Hz pulses, 100 ms high, 900 ms low).</li>
<li>Install OCXO on separate 5 V regulated supply. Allow 30-minute warmup; output must be within &plusmn;5 ppm of 10 MHz.</li>
<li>Connect EFC DAC output through 100 &Omega; series resistor to OCXO EFC pin. Verify DAC range 0&ndash;5 V tracks EFC input.</li>
<li>Flash MCU firmware (open-source GPSDO firmware or custom DPLL). Connect GPS 1 PPS to interrupt pin.</li>
<li>Power on. After first GPS lock, firmware begins accumulating phase measurements. Lock typically achieved in 5&ndash;15 minutes.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>Allow 30-minute OCXO warmup before enabling DPLL.</li>
<li>Enable GPS discipline. Monitor UART log output for phase error values; should converge to &lt;100 ns within 10 minutes of lock.</li>
<li>Connect 10 MHz output to second frequency counter with independent GPS reference. Verify displayed frequency agrees to &lt;0.01 Hz difference over 60-second interval.</li>
<li>Log EFC voltage every 10 minutes over 4 hours. EFC should be stable within &plusmn;0.1 V of lock-point, confirming thermal stability of OCXO.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Adjust PI loop constants K<sub>p</sub> and K<sub>i</sub> in firmware. K<sub>p</sub> too large causes ringing; K<sub>i</sub> too large causes slow drift. Start with &tau; = 200 s (K<sub>i</sub> = 1/200). Optimal tuning minimizes ADEV (Allan deviation) at 1&ndash;100 second averaging time.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>GPSDO Acceptance Specifications</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Output frequency</td><td>10.000000 MHz &plusmn;0.1 ppm (GPS locked)</td></tr>
<tr><td>Phase noise (1 kHz offset)</td><td>&lt;&minus;120 dBc/Hz (OCXO limited)</td></tr>
<tr><td>1 PPS accuracy to UTC</td><td>&lt;100 ns (GPS-limited)</td></tr>
<tr><td>Holdover (GPS lost)</td><td>&lt;&plusmn;0.5 ppm over 24 h</td></tr>
<tr><td>Output level</td><td>+3 to +7 dBm into 50 &Omega;</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Allan deviation (frequency stability)</span>ADEV(&tau;) = &radic;(MDEV(&tau;))  [&tau; = averaging time seconds]</div>
<div class="formula-block"><span class="formula-label">EFC sensitivity</span>K<sub>VCO</sub> = &Delta;f / &Delta;V<sub>EFC</sub>  [Hz/V, typically 0.5&ndash;5 Hz/V for OCXO]</div>
<div class="formula-block"><span class="formula-label">Loop time constant</span>&tau; = 1 / (K<sub>i</sub> &times; K<sub>VCO</sub> &times; f<sub>div</sub>)  [seconds]</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>GPSDO Lock-In Log</caption>
<thead><tr><th>Time (min)</th><th>Phase Error (ns)</th><th>EFC Voltage (V)</th><th>Status</th></tr></thead>
<tbody>
<tr><td>0</td><td>4500</td><td>2.50</td><td>Acquiring</td></tr>
<tr><td>5</td><td>320</td><td>2.63</td><td>Locking</td></tr>
<tr><td>15</td><td>42</td><td>2.67</td><td>Locked</td></tr>
<tr><td>60</td><td>18</td><td>2.68</td><td>Stable</td></tr>
</tbody></table>
""",
},

"function-generator": {
    "tm": "TM-TGEAR-012",
    "title": "FUNCTION GENERATOR",
    "subtitle": "Sine, Square, and Triangle Waveform Source, 1 Hz to 5 MHz",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers design, construction, and calibration of a function generator producing sine, square, and triangle waveforms from 1 Hz to 5 MHz. Applications include circuit testing, filter characterization, oscilloscope calibration, and modulation sources.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-002 (Audio Oscillator), TM-TGEAR-020 (Oscilloscope), TM-TGEAR-010 (Frequency Counter), TM-CAL-002 (GPS 1PPS).</p>
<div class="tm-warning"><span class="admonition-label">WARNING</span>If powered from AC mains, use an isolation transformer. Output ground and instrument ground share the same chassis potential. Connecting to another mains-connected instrument without isolation can create a shock hazard or ground loop that corrupts measurements.</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. XR-2206 OR DDS ARCHITECTURE</h3>
<p>Classic function generators use the XR-2206 or ICL8038 monolithic ICs, which generate triangle waves internally, then use a sine shaper (piecewise-linear resistor network) to convert triangle to sine. Modern designs use a DDS chip (AD9833 or AD9850) for direct digital synthesis of all waveforms.</p>
<h3 class="section-heading">2-2. DDS WAVEFORM GENERATION</h3>
<p>A DDS accumulator steps a phase register at each clock cycle. The output of a sine lookup table is converted to an analog voltage by a DAC. The frequency is:</p>
<div class="formula-block"><span class="formula-label">DDS output frequency</span>f<sub>out</sub> = (M &times; f<sub>clk</sub>) / 2<sup>N</sup>  [M = frequency tuning word, N = accumulator bits]</div>
<h3 class="section-heading">2-3. OUTPUT STAGE</h3>
<p>A unity-gain buffer (op-amp, TL072 or AD8066) drives the output through a calibrated attenuator (step + continuous). DC offset is added via a summing amplifier for waveform positioning.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<table class="tm-table"><caption>Function Generator Components (DDS Design)</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>1</td><td>AD9833 DDS</td><td>0&ndash;12.5 MHz, 28-bit resolution, SPI, MSOP-10</td></tr>
<tr><td>1</td><td>Arduino Nano MCU</td><td>SPI master, UART interface, 5 V</td></tr>
<tr><td>1</td><td>OPA2134 op-amp</td><td>Output buffer + offset adder, DIP-8</td></tr>
<tr><td>1</td><td>25 MHz crystal oscillator</td><td>DDS clock, CMOS, &plusmn;50 ppm</td></tr>
<tr><td>1</td><td>BNC connector</td><td>Output, 50 &Omega;</td></tr>
<tr><td>1</td><td>10 k&Omega; 10-turn pot</td><td>Fine frequency trim</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Install DDS module on MCU SPI bus. Verify FSYNC, SCLK, SDATA connections.</li>
<li>Apply 3.3 V to AD9833 V<sub>DD</sub> (regulated from on-board LDO). Verify DDS output on oscilloscope before MCU firmware.</li>
<li>Write test code: set frequency word for 1 kHz sine. Verify 1 kHz on oscilloscope.</li>
<li>Install output buffer and attenuator network. Set output amplitude to 1 V<sub>pp</sub> &plusmn;5% at maximum setting.</li>
<li>Verify square wave rise time &lt;50 ns at 1 MHz by using oscilloscope 10&times; probe.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<h3 class="section-heading">5-1. FREQUENCY CALIBRATION</h3>
<div class="procedure"><ol>
<li>Connect output to frequency counter (TM-TGEAR-010) with GPS-disciplined reference.</li>
<li>Set frequency to 1.000000 MHz. Record actual frequency.</li>
<li>Adjust clock crystal frequency correction in firmware to bring reading to 1.000000 &plusmn; 0.001 MHz (1 ppm).</li>
<li>Verify at 100 Hz, 10 kHz, 100 kHz, 1 MHz, 5 MHz. Log all results.</li>
</ol></div>
<h3 class="section-heading">5-2. AMPLITUDE CALIBRATION</h3>
<div class="procedure"><ol>
<li>Set output to 1 V<sub>pp</sub> on DMM AC range or oscilloscope.</li>
<li>Adjust output attenuator pot or DAC reference until oscilloscope reads 1.00 V<sub>pp</sub> &plusmn;2%.</li>
<li>Verify amplitude flat &plusmn;0.5 dB from 100 Hz to 1 MHz.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>DC offset is adjusted by the offset pot. Full offset range is typically &plusmn;5 V for a &plusmn;5 V supply op-amp stage. Duty cycle of square wave output is adjusted by a comparator threshold pot; 50% &plusmn;1% is achievable with stable comparator hysteresis.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>Function Generator Acceptance Specifications</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Frequency range</td><td>1 Hz &ndash; 5 MHz (DDS)</td></tr>
<tr><td>Frequency accuracy</td><td>&plusmn;1 ppm after calibration</td></tr>
<tr><td>Sine THD (1 kHz)</td><td>&lt;1% (DDS with sine lookup)</td></tr>
<tr><td>Output amplitude</td><td>0&ndash;10 V<sub>pp</sub> into high-Z</td></tr>
<tr><td>Output impedance</td><td>50 &Omega; &plusmn;5%</td></tr>
<tr><td>Square wave rise time</td><td>&lt;50 ns at 1 MHz</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">DDS frequency tuning word</span>M = f<sub>out</sub> &times; 2<sup>N</sup> / f<sub>clk</sub>  [N=28 for AD9833, f<sub>clk</sub>=25 MHz]</div>
<div class="formula-block"><span class="formula-label">DDS frequency resolution</span>&Delta;f = f<sub>clk</sub> / 2<sup>N</sup> = 25&times;10<sup>6</sup> / 2<sup>28</sup> &asymp; 0.093 Hz</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Function Generator Calibration Log</caption>
<thead><tr><th>Set Frequency</th><th>Measured</th><th>Error (ppm)</th><th>Amplitude (V<sub>pp</sub>)</th></tr></thead>
<tbody>
<tr><td>1 kHz</td><td>1000.001 Hz</td><td>+1.0</td><td>1.002</td></tr>
<tr><td>10 kHz</td><td>10,000.10 Hz</td><td>+1.0</td><td>1.000</td></tr>
<tr><td>1 MHz</td><td>999,999 Hz</td><td>&minus;1.0</td><td>0.997</td></tr>
</tbody></table>
""",
},

"impedance-bridge": {
    "tm": "TM-TGEAR-013",
    "title": "IMPEDANCE BRIDGE",
    "subtitle": "AC Null-Balance Impedance Measurement Bridge",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers design, construction, and calibration of a manual AC impedance bridge (modified Maxwell or Hay bridge) for measuring inductance, capacitance, and resistance at audio frequencies (100 Hz to 100 kHz). The null-balance method provides accuracies of 0.1% with calibrated standards.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-015 (LCR Meter), TM-TGEAR-002 (Audio Oscillator), TM-CAL-008 (Precision Resistor), TM-CAL-010 (Kelvin 4-Wire).</p>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. WHEATSTONE/MAXWELL BRIDGE BALANCE</h3>
<p>Four impedance arms (Z<sub>1</sub>, Z<sub>2</sub>, Z<sub>3</sub>, Z<sub>X</sub>) form a bridge. AC null is detected when the ratio Z<sub>1</sub>/Z<sub>2</sub> = Z<sub>3</sub>/Z<sub>X</sub>. At null, no current flows through the detector, and Z<sub>X</sub> is computed from the known arms.</p>
<div class="formula-block"><span class="formula-label">Maxwell bridge (inductance)</span>L<sub>X</sub> = R<sub>2</sub> &times; R<sub>3</sub> &times; C<sub>1</sub>  and  R<sub>X</sub> = R<sub>2</sub> &times; R<sub>3</sub> / R<sub>1</sub></div>
<h3 class="section-heading">2-2. HAY BRIDGE</h3>
<p>The Hay bridge is preferred for high-Q inductors:</p>
<div class="formula-block"><span class="formula-label">Hay bridge (high-Q inductor)</span>L<sub>X</sub> = R<sub>2</sub> &times; R<sub>3</sub> &times; C<sub>1</sub> / (1 + (1/Q)<sup>2</sup>)</div>
<h3 class="section-heading">2-3. NULL DETECTOR</h3>
<p>An audio frequency amplifier + earphone or a galvanometer-type null detector senses bridge balance. The detector amplifier must be narrow-band to reject harmonics and improve null sensitivity.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<table class="tm-table"><caption>Maxwell Bridge Component List</caption>
<thead><tr><th>Arm</th><th>Component</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Z<sub>1</sub> (standard)</td><td>Decade capacitance box</td><td>1 pF &ndash; 10 &micro;F, &plusmn;1% (TM-TGEAR-005)</td></tr>
<tr><td>R<sub>1</sub></td><td>Decade resistance box</td><td>0&ndash;100 k&Omega;, &plusmn;0.1%</td></tr>
<tr><td>R<sub>2</sub>, R<sub>3</sub></td><td>Precision resistors</td><td>1 k&Omega; &plusmn;0.01%, wirewound</td></tr>
<tr><td>Detector</td><td>LM386 audio amp + earphone</td><td>1 kHz sensitivity &lt;10 mV</td></tr>
<tr><td>Source</td><td>1 kHz audio oscillator</td><td>&lt;0.1% THD (TM-TGEAR-002)</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Wire bridge arms on phenolic strip board. Use shielded leads for Z<sub>X</sub> arm to minimize stray capacitance pickup.</li>
<li>Install 1 kHz source and null detector connections on front panel.</li>
<li>Verify bridge balance with Z<sub>X</sub> = Z<sub>3</sub> (known standard): null should be &lt;1% of drive voltage.</li>
<li>Test with 100 &micro;H inductor on Maxwell bridge: balance for minimum headphone null.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>Verify R<sub>2</sub> and R<sub>3</sub> ratio arms with 4-wire DMM. Record actual values R<sub>2</sub>/R<sub>3</sub> ratio.</li>
<li>Calibrate standard capacitor C<sub>1</sub> (decade box) against LCR meter values.</li>
<li>Connect known 100 &micro;H &plusmn;0.1% inductor as Z<sub>X</sub>. Balance bridge. Compute L<sub>X</sub> = R<sub>2</sub>R<sub>3</sub>C<sub>1</sub>. Compare to known value.</li>
<li>Error should be &lt;0.5%. If not, re-measure R<sub>2</sub>, R<sub>3</sub>, C<sub>1</sub> individually.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Source frequency affects bridge balance in the Hay configuration. Use 1 kHz for general work; for high-Q iron-core inductors, use lower frequency (100&ndash;400 Hz) to avoid skin effect errors. For capacitor measurement, switch to Schering bridge topology.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>Impedance Bridge Acceptance Specifications</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Frequency range</td><td>100 Hz &ndash; 100 kHz</td></tr>
<tr><td>Accuracy (L, C, R)</td><td>&plusmn;0.1% with calibrated standards</td></tr>
<tr><td>Null sensitivity</td><td>&lt;0.1% of full-scale</td></tr>
<tr><td>L range</td><td>1 &micro;H &ndash; 100 H</td></tr>
<tr><td>C range</td><td>1 pF &ndash; 1000 &micro;F</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Maxwell bridge (L and R)</span>L<sub>X</sub> = R<sub>2</sub>R<sub>3</sub>C<sub>1</sub>, R<sub>X</sub> = R<sub>2</sub>R<sub>3</sub>/R<sub>1</sub></div>
<div class="formula-block"><span class="formula-label">Schering bridge (C and D)</span>C<sub>X</sub> = C<sub>1</sub>R<sub>3</sub>/R<sub>2</sub>, D = &omega;C<sub>1</sub>R<sub>1</sub></div>
<div class="formula-block"><span class="formula-label">Q factor</span>Q = &omega;L<sub>X</sub>/R<sub>X</sub></div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Impedance Bridge Measurement Log</caption>
<thead><tr><th>Unknown</th><th>Reference Value</th><th>Measured</th><th>Error</th></tr></thead>
<tbody>
<tr><td>100 &mu;H coil</td><td>100.0 &mu;H</td><td>100.2 &mu;H</td><td>+0.2%</td></tr>
<tr><td>1 nF capacitor</td><td>1.000 nF</td><td>1.003 nF</td><td>+0.3%</td></tr>
<tr><td>100 &Omega; resistor</td><td>100.00 &Omega;</td><td>100.04 &Omega;</td><td>+0.04%</td></tr>
</tbody></table>
""",
},

"inductance-meter": {
    "tm": "TM-TGEAR-014",
    "title": "INDUCTANCE METER",
    "subtitle": "Standalone Inductance Measurement Instrument, 100 nH to 100 H",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers design, construction, and calibration of a standalone inductance meter for measuring coils and transformers from 100 nH to 100 H. Methods covered: resonance method (L from known C and measured f), and integration method (L from V and dI/dt).</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-015 (LCR Meter), TM-TGEAR-006 (Decade Inductance Box), TM-TGEAR-013 (Impedance Bridge).</p>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. RESONANCE METHOD</h3>
<p>A reference capacitor C<sub>ref</sub> of known value is placed in parallel with the unknown inductor. The oscillator (555 or Colpitts) finds resonant frequency f<sub>r</sub>. Then:</p>
<div class="formula-block"><span class="formula-label">Inductance from resonance</span>L<sub>X</sub> = 1 / ((2&pi;f<sub>r</sub>)<sup>2</sup> &times; C<sub>ref</sub>)</div>
<h3 class="section-heading">2-2. TIME-DOMAIN INTEGRATION</h3>
<p>Apply a known voltage step V across L. Measure time &Delta;t for current to rise to threshold I<sub>T</sub>:</p>
<div class="formula-block"><span class="formula-label">Inductance from rise time</span>L = V &times; &Delta;t / I<sub>T</sub></div>
<h3 class="section-heading">2-3. SELF-RESONANCE EFFECT</h3>
<p>All inductors have a self-resonant frequency f<sub>SRF</sub> above which they appear capacitive. The resonance method overestimates L if f<sub>r</sub> &gt; f<sub>SRF</sub>/3.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<table class="tm-table"><caption>Inductance Meter Components</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>4</td><td>Reference capacitors</td><td>10 nF, 100 nF, 1 &micro;F, 10 &micro;F NP0/C0G, &plusmn;1%</td></tr>
<tr><td>1</td><td>555 oscillator</td><td>Astable, range 10 Hz &ndash; 1 MHz</td></tr>
<tr><td>1</td><td>Frequency counter input</td><td>MCU TIM capture or 74AC04</td></tr>
<tr><td>1</td><td>Rotary range switch</td><td>4-position, selects C<sub>ref</sub></td></tr>
<tr><td>1</td><td>STM32F103 MCU</td><td>Computes L from f and C</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Install 4 reference capacitors on range switch. Measure each with LCR meter and record actual values.</li>
<li>Wire oscillator input across DUT terminals. Verify oscillator starts on each range with 100 &mu;H through 10 mH test inductors.</li>
<li>Program MCU with C<sub>ref</sub> actual measured values for each range.</li>
<li>Zero meter: short test terminals, press ZERO to subtract wiring/stray inductance (typically 0.1&ndash;2 &mu;H).</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>Verify reference capacitors with LCR meter. Update firmware constants if measured values differ from nominal by &gt;0.1%.</li>
<li>Measure decade inductance box (TM-TGEAR-006) at positions 10 &micro;H, 100 &mu;H, 1 mH, 10 mH. Results must agree within &plusmn;2%.</li>
<li>Measure known 100 &mu;H &plusmn;1% standard inductor. Record result.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>If calibration error exceeds 2%, remeasure reference capacitors at 10 kHz with LCR meter and update constants. Temperature coefficient of C0G capacitors is &lt;30 ppm/&deg;C, resulting in &lt;0.1% error over 25 &deg;C range.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>Inductance Meter Acceptance Specifications</caption>
<thead><tr><th>Range</th><th>Accuracy</th><th>Resolution</th></tr></thead>
<tbody>
<tr><td>100 nH &ndash; 999 nH</td><td>&plusmn;5%</td><td>1 nH</td></tr>
<tr><td>1 &mu;H &ndash; 999 &mu;H</td><td>&plusmn;2%</td><td>0.1 &mu;H</td></tr>
<tr><td>1 mH &ndash; 999 mH</td><td>&plusmn;2%</td><td>0.01 mH</td></tr>
<tr><td>1 H &ndash; 100 H</td><td>&plusmn;5%</td><td>0.1 H</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Inductance from resonance</span>L = 1 / ((2&pi;f)<sup>2</sup> &times; C<sub>ref</sub>)</div>
<div class="formula-block"><span class="formula-label">Inductive reactance</span>X<sub>L</sub> = 2&pi;fL</div>
<div class="formula-block"><span class="formula-label">Self-resonant frequency</span>f<sub>SRF</sub> = 1 / (2&pi;&radic;(L &times; C<sub>winding</sub>))</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Inductance Meter Calibration Log</caption>
<thead><tr><th>Standard</th><th>Nominal (&mu;H)</th><th>Measured (&mu;H)</th><th>Error</th></tr></thead>
<tbody>
<tr><td>T68-2 coil</td><td>100</td><td>99.4</td><td>&minus;0.6%</td></tr>
<tr><td>Decade box 1 mH</td><td>1000</td><td>1007</td><td>+0.7%</td></tr>
<tr><td>RF choke 10 mH</td><td>10000</td><td>9940</td><td>&minus;0.6%</td></tr>
</tbody></table>
""",
},

"lcr-meter": {
    "tm": "TM-TGEAR-015",
    "title": "LCR METER",
    "subtitle": "Combined Inductance, Capacitance, and Resistance Measurement",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers operation and calibration of a combined LCR meter measuring inductance (L), capacitance (C), and resistance (R) at test frequencies of 100 Hz, 1 kHz, and 10 kHz. The LCR meter is the primary instrument for verifying component values in RF circuits.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-003 (Capacitance Meter), TM-TGEAR-014 (Inductance Meter), TM-CAL-008 (Precision Resistors), TM-CAL-010 (Kelvin 4-Wire).</p>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. AUTO-BALANCING BRIDGE</h3>
<p>The auto-balancing bridge method applies a test voltage V<sub>osc</sub> to the DUT and a current-sense resistor R<sub>S</sub> in series. The current I through the DUT is measured. Complex impedance Z = V<sub>DUT</sub> / I. The instrument decomposes Z into R + jX and computes L or C.</p>
<div class="formula-block"><span class="formula-label">Series equivalent circuit</span>Z = R<sub>s</sub> + j&omega;L<sub>s</sub>  (inductor) or  Z = R<sub>s</sub> &minus; j/(}&omega;C<sub>s</sub>)  (capacitor)</div>
<h3 class="section-heading">2-2. Q AND D FACTORS</h3>
<div class="formula-block"><span class="formula-label">Q factor (inductor quality)</span>Q = X<sub>L</sub> / R<sub>s</sub> = &omega;L / R<sub>s</sub></div>
<div class="formula-block"><span class="formula-label">D factor (dissipation, capacitor)</span>D = R<sub>s</sub> / X<sub>C</sub> = R<sub>s</sub> &times; &omega;C</div>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<table class="tm-table"><caption>Auto-Balancing LCR Bridge Components</caption>
<thead><tr><th>Block</th><th>Part</th><th>Role</th></tr></thead>
<tbody>
<tr><td>Test oscillator</td><td>DDS or Wien bridge</td><td>100 Hz, 1 kHz, 10 kHz</td></tr>
<tr><td>Current sense</td><td>10 &Omega; &plusmn;0.01%, wirewound</td><td>Measures DUT current I</td></tr>
<tr><td>I/Q detector</td><td>AD8302 or synchronous detector</td><td>Measures magnitude/phase</td></tr>
<tr><td>MCU</td><td>STM32 or Arduino</td><td>Computes L, C, R, Q, D</td></tr>
<tr><td>Display</td><td>16&times;2 LCD or OLED</td><td>Shows L/C/R + Q/D</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Build test oscillator. Verify frequency with counter at each test frequency setting.</li>
<li>Install precision current sense resistor R<sub>S</sub>. Measure actual value with 4-wire DMM and enter in firmware.</li>
<li>Calibrate I/Q detector gain and phase balance per manufacturer data sheet.</li>
<li>Perform open/short/load correction (OLOAD calibration) at each test frequency before use.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>Open circuit calibration: measure Y<sub>open</sub> at all test frequencies. Subtract from subsequent measurements.</li>
<li>Short circuit calibration: measure Z<sub>short</sub> at all test frequencies. Subtract from subsequent measurements.</li>
<li>Load calibration (optional): measure precision 100 &Omega; &plusmn;0.01% resistor. Apply gain correction factor.</li>
<li>Verify: 100 pF C0G reads 100 &plusmn;1 pF; 100 &mu;H coil reads 100 &plusmn;2 &mu;H; 100 &Omega; reads 100 &plusmn;0.2 &Omega;.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Phase balance of the I/Q detector affects accuracy for high-Q measurements. Adjust phase trim capacitor (2&ndash;10 pF) at the detector reference input for minimum Q error on a pure resistor (Q should read near zero for an ideal resistor).</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>LCR Meter Acceptance Specifications</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>L range</td><td>100 nH &ndash; 100 H</td></tr>
<tr><td>C range</td><td>1 pF &ndash; 10 mF</td></tr>
<tr><td>R range</td><td>10 m&Omega; &ndash; 10 M&Omega;</td></tr>
<tr><td>Accuracy (L, C)</td><td>&plusmn;0.2% typical at 1 kHz</td></tr>
<tr><td>Accuracy (R)</td><td>&plusmn;0.1% with 4-wire connection</td></tr>
<tr><td>Q range</td><td>0.0001 &ndash; 10000</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Inductance from Z at f</span>L = Im(Z) / (2&pi;f)  [Im(Z) &gt; 0]</div>
<div class="formula-block"><span class="formula-label">Capacitance from Z at f</span>C = &minus;1 / (Im(Z) &times; 2&pi;f)  [Im(Z) &lt; 0]</div>
<div class="formula-block"><span class="formula-label">Dissipation factor</span>D = 1/Q = R<sub>s</sub> &times; 2&pi;f &times; C</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>LCR Meter Verification Log at 1 kHz</caption>
<thead><tr><th>Component</th><th>Nominal</th><th>Measured</th><th>Error</th><th>Q/D</th></tr></thead>
<tbody>
<tr><td>100 pF C0G</td><td>100 pF</td><td>100.1 pF</td><td>+0.1%</td><td>D=0.0002</td></tr>
<tr><td>100 nF film</td><td>100 nF</td><td>99.8 nF</td><td>&minus;0.2%</td><td>D=0.001</td></tr>
<tr><td>100 &mu;H coil</td><td>100 &mu;H</td><td>100.3 &mu;H</td><td>+0.3%</td><td>Q=85</td></tr>
<tr><td>100 &Omega; resistor</td><td>100.00 &Omega;</td><td>100.02 &Omega;</td><td>+0.02%</td><td>&mdash;</td></tr>
</tbody></table>
""",
},

"modulation-analyzer": {
    "tm": "TM-TGEAR-016",
    "title": "MODULATION ANALYZER",
    "subtitle": "AM and FM Modulation Depth Measurement",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers design, construction, and calibration of a modulation analyzer for measuring amplitude modulation (AM) depth and frequency deviation (FM deviation) in RF transmitters. Covering 500 kHz to 170 MHz, it verifies transmitter compliance and modulation quality.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-028 (TinySA Spectrum Analyzer), TM-TGEAR-026 (Signal Generator), TM-CAL-004 (FM Broadcast Reference).</p>
<div class="tm-caution"><span class="admonition-label">CAUTION</span>Maximum RF input at the modulation analyzer is typically +20 dBm (100 mW). Use a 20 dB attenuator pad when measuring transmitter outputs above 100 mW.</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. AM MODULATION DEPTH</h3>
<p>AM depth m (modulation index) is defined as:</p>
<div class="formula-block"><span class="formula-label">AM modulation depth</span>m = (V<sub>max</sub> &minus; V<sub>min</sub>) / (V<sub>max</sub> + V<sub>min</sub>) &times; 100%</div>
<p>An envelope detector (diode + RC filter) recovers the modulating signal. Peak-to-peak measurement of the envelope gives V<sub>max</sub> and V<sub>min</sub>.</p>
<h3 class="section-heading">2-2. FM DEVIATION</h3>
<p>FM deviation is measured via a discriminator (Foster-Seeley, ratio detector, or PLL). The discriminator output voltage is proportional to instantaneous frequency deviation &Delta;f.</p>
<div class="formula-block"><span class="formula-label">FM modulation index</span>&beta; = &Delta;f / f<sub>m</sub>  [&Delta;f = peak deviation, f<sub>m</sub> = modulating freq]</div>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<table class="tm-table"><caption>Modulation Analyzer Subsystems</caption>
<thead><tr><th>Block</th><th>Function</th><th>Key Part</th></tr></thead>
<tbody>
<tr><td>RF input attenuator</td><td>Level scaling</td><td>20 dB pi-pad (TM-CAL-015)</td></tr>
<tr><td>IF amplifier</td><td>Fixed-gain, 10.7 MHz IF</td><td>SA602 + ceramic filter</td></tr>
<tr><td>AM envelope detector</td><td>Demodulate AM</td><td>1N60 + 100 nF + 10 k&Omega;</td></tr>
<tr><td>FM discriminator</td><td>Demodulate FM</td><td>PLL (NE564 or XR-2211)</td></tr>
<tr><td>Audio output</td><td>Baseband monitoring</td><td>LM386 amplifier + speaker</td></tr>
<tr><td>Metering</td><td>Display modulation level</td><td>VU-meter or peak-hold circuit</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Align IF ceramic filter at 10.7 MHz with signal generator and spectrum analyzer.</li>
<li>Calibrate AM envelope detector: apply 100% AM modulated carrier. Adjust RC time constant for flat audio response 300 Hz &ndash; 3 kHz.</li>
<li>Calibrate FM discriminator: set PLL free-running frequency to IF center (10.7 MHz). Trim VCO to center.</li>
<li>Verify FM deviation sensitivity: 75 kHz deviation (broadcast standard) should produce 1 kHz audio at calibration level.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<h3 class="section-heading">5-1. AM CALIBRATION</h3>
<div class="procedure"><ol>
<li>Apply signal from calibrated AM generator: 1 MHz carrier, 1 kHz tone, 30% modulation (m = 0.30).</li>
<li>Adjust AM meter scale to read 30% at this modulation depth.</li>
<li>Verify at 100% modulation (m = 1.00): meter should read 100% &plusmn;2%.</li>
</ol></div>
<h3 class="section-heading">5-2. FM DEVIATION CALIBRATION</h3>
<div class="procedure"><ol>
<li>Tune to an FM broadcast station (e.g., 98.3 MHz). Broadcast stereo pilot = 19 kHz, max deviation = &plusmn;75 kHz.</li>
<li>Adjust FM meter scale using broadcast standard as reference per TM-CAL-004.</li>
<li>Verify with 2-meter CTCSS tone test: 67.0 Hz CTCSS at &plusmn;5 kHz deviation should read 5 kHz.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>PLL capture range must exceed maximum FM deviation. Set loop filter bandwidth 3&times; wider than maximum modulating frequency. For VHF broadcast, loop BW &gt; 100 kHz. For narrow-band 2m (5 kHz deviation), loop BW &gt; 10 kHz.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>Modulation Analyzer Acceptance Specs</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>AM accuracy</td><td>&plusmn;2% modulation depth</td></tr>
<tr><td>FM accuracy</td><td>&plusmn;2% deviation</td></tr>
<tr><td>Input RF range</td><td>&minus;20 dBm to +20 dBm</td></tr>
<tr><td>Frequency coverage</td><td>500 kHz &ndash; 170 MHz</td></tr>
<tr><td>Audio response</td><td>300 Hz &ndash; 10 kHz (&plusmn;1 dB)</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">AM depth from sideband power</span>m = 2 &times; &radic;(P<sub>sideband</sub> / P<sub>carrier</sub>)  [power ratio at one sideband]</div>
<div class="formula-block"><span class="formula-label">FM index from Bessel analysis</span>&beta; = &Delta;f / f<sub>m</sub>;  check J<sub>0</sub>(&beta;) = 0 at &beta; = 2.405 (first carrier null)</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Modulation Analyzer Calibration Log</caption>
<thead><tr><th>Signal</th><th>Set</th><th>Measured</th><th>Error</th></tr></thead>
<tbody>
<tr><td>AM 1 MHz, 30%</td><td>30%</td><td>30.4%</td><td>+1.3%</td></tr>
<tr><td>AM 1 MHz, 100%</td><td>100%</td><td>99.2%</td><td>&minus;0.8%</td></tr>
<tr><td>FM 98.3 MHz, &pm;75 kHz</td><td>75 kHz</td><td>74.1 kHz</td><td>&minus;1.2%</td></tr>
</tbody></table>
""",
},

"multimeter-dmm": {
    "tm": "TM-TGEAR-017",
    "title": "DIGITAL MULTIMETER (DMM)",
    "subtitle": "Voltage, Current, Resistance, and Frequency Measurement &mdash; Fluke 15B+",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers calibration procedures for the Fluke 15B+ digital multimeter and equivalent instruments. The Fluke 15B+ provides DC voltage (0.1 mV &ndash; 1000 V), AC voltage (1 mV &ndash; 750 V), resistance (0.1 &Omega; &ndash; 40 M&Omega;), continuity, and frequency measurement. All calibration references are publicly traceable.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-CAL-006 (Precision Voltage Reference), TM-CAL-008 (Precision Resistors), TM-CAL-010 (Kelvin 4-Wire), TM-CAL-002 (GPS 1PPS).</p>
<div class="tm-warning"><span class="admonition-label">WARNING</span>The Fluke 15B+ is rated for CAT III 600 V and CAT II 1000 V. Observe probe ratings. Never measure AC mains circuits with probes rated below CAT III. The common (COM) lead is not isolated from the measurement circuit; do not float the instrument ground above chassis potential on mains-connected equipment.</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. SIGMA-DELTA ADC</h3>
<p>Modern DMMs use a sigma-delta (&Sigma;&Delta;) ADC. An oversampled 1-bit stream is decimated to 4&ndash;5 significant digits. A precision band-gap voltage reference (e.g., LM4040, 1.225 V &plusmn;0.1%) establishes the measurement scale. All voltage ranges are scaled to this internal reference.</p>
<h3 class="section-heading">2-2. RESISTANCE MEASUREMENT</h3>
<p>The DMM forces a known current through R<sub>X</sub> and measures the resulting voltage. On 2-wire measurement, lead resistance is included. On 4-wire (Kelvin) measurement, a separate pair of sense leads carries no current, eliminating lead resistance error.</p>
<div class="formula-block"><span class="formula-label">4-wire resistance</span>R<sub>X</sub> = V<sub>sense</sub> / I<sub>force</sub></div>
<h3 class="section-heading">2-3. FREQUENCY MEASUREMENT</h3>
<p>A comparator converts the input AC signal to a logic pulse train. A timer measures period T; frequency = 1/T. Accuracy depends on the oscillator reference, typically a 32.768 kHz crystal. Accuracy: &plusmn;0.5% for most DMMs; &plusmn;0.1% for Fluke 15B+.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<p>The Fluke 15B+ is a commercial instrument. This chapter describes the construction of a homebrew DC voltage standard for calibrating the voltage ranges (see TM-CAL-006).</p>
<table class="tm-table"><caption>LM4040 4.096 V Precision Reference</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>1</td><td>LM4040C-4.1</td><td>4.096 V shunt reference, &plusmn;0.1%, SOT-23</td></tr>
<tr><td>1</td><td>1 k&Omega; resistor</td><td>1%, series current-set, for 5 V supply</td></tr>
<tr><td>1</td><td>9 V battery</td><td>Low-noise supply for shunt reference</td></tr>
<tr><td>2</td><td>Banana jack</td><td>Output terminals, 4 mm</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Assemble 4.096 V reference on small PCB. Measure output with already-calibrated precision DMM if available.</li>
<li>Allow 10-minute warmup for LM4040 before calibration measurement.</li>
<li>Connect calibration reference to DMM DC V input. Record DMM reading vs. reference value.</li>
<li>Note temperature during calibration; LM4040 TC is &plusmn;100 ppm/&deg;C (class C) or &plusmn;50 ppm/&deg;C (class A).</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<h3 class="section-heading">5-1. DC VOLTAGE CALIBRATION</h3>
<div class="procedure"><ol>
<li>Connect 4.096 V reference to DMM DC V input (4 V range or 40 V range).</li>
<li>Record DMM reading. Compute correction factor: K = V<sub>ref</sub> / V<sub>DMM</sub>.</li>
<li>Apply K to all subsequent voltage measurements, or adjust DMM trim pot if user-accessible.</li>
<li>Fluke 15B+ spec: &plusmn;0.5% + 2 digits on DC ranges. Verify DMM is within spec.</li>
</ol></div>
<h3 class="section-heading">5-2. RESISTANCE CALIBRATION</h3>
<div class="procedure"><ol>
<li>Connect precision 100 &Omega; &plusmn;0.01% resistor (TM-CAL-008). Use 4-wire (Kelvin) clips if available.</li>
<li>Record reading. Error must be &lt;&plusmn;1 &Omega; on 400 &Omega; range.</li>
<li>Connect 10 k&Omega; &plusmn;0.01% resistor. Record. Error must be &lt;&plusmn;100 &Omega; on 40 k&Omega; range.</li>
</ol></div>
<h3 class="section-heading">5-3. FREQUENCY CALIBRATION</h3>
<div class="procedure"><ol>
<li>Apply GPS-disciplined 1 kHz signal (TM-CAL-002). Record DMM Hz reading.</li>
<li>Error must be &lt;&plusmn;0.5% (5 Hz at 1 kHz).</li>
<li>Repeat at 10 kHz and 100 kHz.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>The Fluke 15B+ has user-inaccessible internal calibration adjustments. If the instrument is out of spec, it requires service by a qualified calibration lab. Log calibration correction factors in the instrument record for use in measurement uncertainty budgeting.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>Fluke 15B+ Calibration Acceptance</caption>
<thead><tr><th>Function</th><th>Range</th><th>Spec</th><th>Result</th></tr></thead>
<tbody>
<tr><td>DC Voltage</td><td>4 V</td><td>&plusmn;0.5% + 2 digits</td><td>Check 4.096 V reference</td></tr>
<tr><td>AC Voltage (rms)</td><td>4 V</td><td>&plusmn;1.0% + 3 digits</td><td>Check 1 kHz sine</td></tr>
<tr><td>Resistance</td><td>400 &Omega;</td><td>&plusmn;0.5% + 2 digits</td><td>Check 100 &Omega; std</td></tr>
<tr><td>Frequency</td><td>1 kHz</td><td>&plusmn;0.5%</td><td>GPS-locked source</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Correction factor</span>K = V<sub>true</sub> / V<sub>indicated</sub></div>
<div class="formula-block"><span class="formula-label">Measurement uncertainty (simplified)</span>U = &radic;(u<sub>ref</sub><sup>2</sup> + u<sub>DMM</sub><sup>2</sup>)  [combine reference and instrument uncertainties in quadrature]</div>
<div class="formula-block"><span class="formula-label">Kelvin 4-wire resistance</span>R = V<sub>sense</sub> / I<sub>force</sub>  (lead resistance eliminated)</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Fluke 15B+ Calibration Log</caption>
<thead><tr><th>Test</th><th>Reference</th><th>Reading</th><th>Error</th><th>Spec</th><th>Pass/Fail</th></tr></thead>
<tbody>
<tr><td>DC V 4 V range</td><td>4.0960 V</td><td>4.095 V</td><td>&minus;0.024%</td><td>&plusmn;0.5%</td><td>Pass</td></tr>
<tr><td>Resistance 400 &Omega;</td><td>100.01 &Omega;</td><td>100.0 &Omega;</td><td>&minus;0.01%</td><td>&plusmn;0.5%</td><td>Pass</td></tr>
<tr><td>Frequency 1 kHz</td><td>1000.00 Hz</td><td>1000 Hz</td><td>0.00%</td><td>&plusmn;0.5%</td><td>Pass</td></tr>
</tbody></table>
""",
},

"nanovna-vector-network-analyzer": {
    "tm": "TM-TGEAR-018",
    "title": "NANOVNA VECTOR NETWORK ANALYZER",
    "subtitle": "Two-Port S-Parameter Measurement, 50 kHz to 900 MHz",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers the calibration and use of the NanoVNA two-port vector network analyzer for measuring S-parameters (S11, S21), antenna impedance, filter response, and transmission line characteristics from 50 kHz to 900 MHz. The NanoVNA uses SOLT calibration to remove systematic measurement errors.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-CAL-015 (Attenuator Pads), TM-CAL-014 (Dummy Load), TM-CAL-002 (GPS 1PPS), TM-TGEAR-022 (RF Attenuator Set).</p>
<div class="tm-note"><span class="admonition-label">NOTE</span>The NanoVNA internal oscillator is a Si5351A. Its frequency accuracy is &plusmn;30 ppm without correction. GPS frequency correction to &lt;1 ppm is performed via the SOLT load calibration and a frequency counter cross-check.</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. S-PARAMETER DEFINITIONS</h3>
<p>S-parameters describe the linear behavior of a two-port network in terms of incident and reflected power waves:</p>
<div class="formula-block"><span class="formula-label">S11 (input reflection)</span>S11 = b1/a1 (with a2=0)  &mdash; reflected wave at port 1</div>
<div class="formula-block"><span class="formula-label">S21 (forward transmission)</span>S21 = b2/a1 (with a2=0)  &mdash; transmitted wave at port 2</div>
<h3 class="section-heading">2-2. 12-TERM ERROR MODEL</h3>
<p>The NanoVNA implements a simplified 3-term (1-port) and 6-term (2-port) error model. SOLT calibration solves for error terms: directivity (E<sub>D</sub>), source match (E<sub>S</sub>), and reflection tracking (E<sub>R</sub>).</p>
<div class="formula-block"><span class="formula-label">Corrected S11</span>S11<sub>corrected</sub> = (S11<sub>measured</sub> &minus; E<sub>D</sub>) / (E<sub>R</sub> + E<sub>S</sub> &times; (S11<sub>measured</sub> &minus; E<sub>D</sub>))</div>
<h3 class="section-heading">2-3. SMITH CHART INTERPRETATION</h3>
<p>The Smith chart maps complex reflection coefficient &Gamma; onto a normalized impedance plane. Center = 50 &Omega; (Z<sub>0</sub>). Top arc = inductive reactance (+ jX). Bottom arc = capacitive reactance (&minus; jX). Right = open (Z = &infin;). Left = short (Z = 0).</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<p>The NanoVNA is a commercial product. This chapter covers the construction of a SOLT calibration kit for NanoVNA port calibration.</p>
<table class="tm-table"><caption>Homebrew SOLT Cal Kit Components</caption>
<thead><tr><th>Standard</th><th>Construction</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Short (S)</td><td>SMA male, center pin shorted to shell</td><td>|&Gamma;| = 1.0, phase = &minus;180&deg; at DC</td></tr>
<tr><td>Open (O)</td><td>SMA male, center pin open (no connection)</td><td>|&Gamma;| = 1.0, phase = 0&deg; at DC (with offset capacitance model)</td></tr>
<tr><td>Load (L)</td><td>SMA male, 50 &Omega; &plusmn;1%, non-inductive resistor</td><td>|&Gamma;| &lt; 0.02 (RL &gt;34 dB) to 300 MHz</td></tr>
<tr><td>Through (T)</td><td>SMA male-to-male adapter</td><td>Insertion loss &lt;0.1 dB to 300 MHz</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Construct SOLT kit (or use commercial NanoVNA cal kit). Mark each standard clearly: S/O/L/T.</li>
<li>Power on NanoVNA. Allow 5-minute warmup. Verify firmware version (NanoVNA-D, NanoVNA-F, etc.).</li>
<li>Connect to NanoVNA-QT or NanoVNA-App via USB for PC-based calibration management.</li>
<li>Perform SOLT calibration at the measurement reference plane (cable end, not NanoVNA SMA ports).</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<h3 class="section-heading">5-1. SOLT ONE-PORT CALIBRATION (PORT 1)</h3>
<div class="procedure"><ol>
<li>Set frequency range: 1 MHz to 300 MHz, 101 points.</li>
<li>Connect SHORT to port 1. In NanoVNA-App: CAL &rarr; OPEN; wait for sweep to complete; press SHORT.</li>
<li>Connect OPEN to port 1. Press OPEN.</li>
<li>Connect LOAD (50 &Omega;) to port 1. Press LOAD.</li>
<li>Press DONE / APPLY. Calibration is now active. Verify: short reads S11 = 1.0&ang;&minus;180&deg;; open reads S11 = 1.0&ang;0&deg;; load reads S11 &lt; &minus;30 dB.</li>
</ol></div>
<h3 class="section-heading">5-2. SOLT TWO-PORT CALIBRATION (PORTS 1 AND 2)</h3>
<div class="procedure"><ol>
<li>After one-port calibration, connect THROUGH between port 1 and port 2. Press THRU.</li>
<li>Press DONE. S21 through standard should read 0 dB &plusmn;0.2 dB from 1 to 300 MHz.</li>
</ol></div>
<h3 class="section-heading">5-3. FREQUENCY AXIS CALIBRATION</h3>
<div class="procedure"><ol>
<li>Tune NanoVNA to marker at 10.000 MHz. Connect frequency counter (TM-TGEAR-010) to port 1 via 20 dB attenuator.</li>
<li>Record counter reading. Compute frequency error in ppm.</li>
<li>Enter correction in NanoVNA firmware: CALIBRATE &rarr; FREQ CORRECTION (ppm).</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>The Si5351A reference crystal can be trimmed in firmware. Access via USB serial port or NanoVNA-App: "Calibrate &rarr; Crystal Correction." Typical trim range: &plusmn;200 ppm. After GPS-disciplined counter cross-check, enter the measured correction value.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>NanoVNA Acceptance Specifications</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Frequency range</td><td>50 kHz &ndash; 900 MHz</td></tr>
<tr><td>S11 dynamic range</td><td>&gt;70 dB (after SOLT cal)</td></tr>
<tr><td>S21 dynamic range</td><td>&gt;70 dB (after SOLT cal)</td></tr>
<tr><td>Frequency accuracy (post-correction)</td><td>&lt;1 ppm</td></tr>
<tr><td>Port impedance</td><td>50 &Omega;</td></tr>
<tr><td>Directivity (after cal)</td><td>&gt;40 dB</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Reflection coefficient to impedance</span>Z = Z<sub>0</sub> &times; (1 + &Gamma;) / (1 &minus; &Gamma;)</div>
<div class="formula-block"><span class="formula-label">Return loss</span>RL = &minus;20 log<sub>10</sub>|S11| dB</div>
<div class="formula-block"><span class="formula-label">Insertion loss</span>IL = &minus;20 log<sub>10</sub>|S21| dB</div>
<div class="formula-block"><span class="formula-label">SWR from S11</span>SWR = (1 + |S11|) / (1 &minus; |S11|)</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>NanoVNA Post-SOLT Calibration Verification</caption>
<thead><tr><th>Standard</th><th>Freq</th><th>S11 (dB)</th><th>Phase</th><th>Pass/Fail</th></tr></thead>
<tbody>
<tr><td>50 &Omega; load</td><td>7 MHz</td><td>&minus;42.3</td><td>&mdash;</td><td>Pass</td></tr>
<tr><td>50 &Omega; load</td><td>100 MHz</td><td>&minus;38.7</td><td>&mdash;</td><td>Pass</td></tr>
<tr><td>Short circuit</td><td>7 MHz</td><td>&minus;0.05</td><td>&minus;178.8&deg;</td><td>Pass</td></tr>
<tr><td>Open circuit</td><td>7 MHz</td><td>&minus;0.12</td><td>+1.2&deg;</td><td>Pass</td></tr>
</tbody></table>
""",
},

"noise-figure-meter": {
    "tm": "TM-TGEAR-019",
    "title": "NOISE FIGURE METER",
    "subtitle": "Receiver and Amplifier Noise Figure Measurement",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers design, construction, and calibration of a noise figure measurement system using the Y-factor method. Instruments covered: noise source (calibrated ENR), spectrum analyzer or power meter as receiver. Applicable to LNA evaluation, receiver sensitivity estimation, and preamplifier characterization from 1 MHz to 1 GHz.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-028 (TinySA Spectrum Analyzer), TM-TGEAR-023 (RF Power Meter), TM-CAL-014 (Dummy Load).</p>
<div class="tm-caution"><span class="admonition-label">CAUTION</span>Noise sources may produce DC bias on the output (typically +28 V DC on/off) for ENR control. Do not connect the noise source output directly to inputs rated for DC-blocked operation only. Use a DC block (bias-T blocking cap) in series if required.</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. Y-FACTOR METHOD</h3>
<p>A calibrated noise source presents two noise temperatures: T<sub>hot</sub> (noise ON) and T<sub>cold</sub> (noise OFF, approximately T<sub>0</sub> = 290 K). The Y-factor is:</p>
<div class="formula-block"><span class="formula-label">Y factor</span>Y = P<sub>hot</sub> / P<sub>cold</sub>  [power ratio, not dB]</div>
<div class="formula-block"><span class="formula-label">Noise figure from Y-factor</span>NF = 10 log<sub>10</sub>((ENR / (Y&minus;1)) &minus; (T<sub>0</sub> &times; (1&minus;Y)) / (Y&minus;1) / T<sub>0</sub>)</div>
<p>For ENR = 15 dB (factor 31.6) and Y = 5 dB (factor 3.16): NF &asymp; 1.0 dB.</p>
<h3 class="section-heading">2-2. EXCESS NOISE RATIO</h3>
<div class="formula-block"><span class="formula-label">ENR (noise source)</span>ENR = 10 log<sub>10</sub>((T<sub>hot</sub> &minus; T<sub>cold</sub>) / T<sub>0</sub>)  [T<sub>0</sub> = 290 K]</div>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<h3 class="section-heading">3-1. HOMEBREW NOISE SOURCE</h3>
<p>A reverse-biased avalanche diode (BFR90, MMBR920, or similar) with 20&ndash;30 V reverse bias produces broadband noise with ENR &asymp; 15&ndash;20 dB at HF/VHF.</p>
<table class="tm-table"><caption>Noise Source Components</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>1</td><td>BFR90 or NPN RF transistor</td><td>Avalanche-mode noise, V<sub>CE(breakdown)</sub> &asymp; 15 V</td></tr>
<tr><td>1</td><td>28 V regulated supply input</td><td>Filtered, ripple &lt;1 mV<sub>pp</sub></td></tr>
<tr><td>1</td><td>1 k&Omega; bias resistor</td><td>Current-limit to 5 mA breakdown</td></tr>
<tr><td>1</td><td>20 dB pad at output</td><td>Match to 50 &Omega;, reduce mismatch error</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Build noise diode biasing circuit on RF PCB. Use ground plane, keep all RF leads &lt;5 mm.</li>
<li>Apply 28 V through 1 k&Omega; to reverse-biased diode junction. Measure current: should be 2&ndash;5 mA in avalanche breakdown.</li>
<li>Verify broadband noise on spectrum analyzer from 10&ndash;500 MHz: noise floor should rise &gt;10 dB above analyzer noise floor.</li>
<li>Measure ENR against known commercial noise source or calibrated power standard if available. Else assign ENR = 15 dB nominal and use with 1&ndash;2 dB uncertainty.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>Set TinySA to zero-span, BW 100 kHz, at desired frequency (e.g., 144 MHz).</li>
<li>Connect noise source output (via 20 dB pad) to TinySA input. Switch noise source OFF. Record P<sub>cold</sub> (dBm).</li>
<li>Switch noise source ON. Record P<sub>hot</sub> (dBm).</li>
<li>Y = 10<sup>(P<sub>hot</sub>&minus;P<sub>cold</sub>)/10</sup>. Compute NF per formula in Section 2-1.</li>
<li>Insert DUT (LNA) between noise source and TinySA. Repeat measurement. Compute DUT noise figure.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>ENR calibration accuracy is the primary uncertainty. Calibrate noise source ENR against a precision spectrum analyzer at multiple frequencies (10, 50, 100, 200, 500 MHz). Record ENR vs. frequency table. Use frequency-specific ENR values for each measurement frequency.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>Noise Figure System Acceptance Specs</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>ENR accuracy</td><td>&plusmn;0.5 dB (calibrated source)</td></tr>
<tr><td>NF measurement accuracy</td><td>&plusmn;1 dB (Y-factor method)</td></tr>
<tr><td>Frequency range</td><td>1 MHz &ndash; 1 GHz</td></tr>
<tr><td>Min measurable NF</td><td>&asymp; 2 dB with TinySA as receiver</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Y-factor (linear)</span>Y = P<sub>hot</sub> / P<sub>cold</sub>  = 10<sup>(P<sub>hot(dBm)</sub> &minus; P<sub>cold(dBm)</sub>)/10</sup></div>
<div class="formula-block"><span class="formula-label">Noise temperature</span>T<sub>e</sub> = T<sub>0</sub> &times; (10<sup>ENR/10</sup> &minus; Y) / (Y &minus; 1)  [K]</div>
<div class="formula-block"><span class="formula-label">Noise figure</span>NF = 10 log<sub>10</sub>(1 + T<sub>e</sub>/T<sub>0</sub>)  [dB]</div>
<div class="formula-block"><span class="formula-label">Friis formula (cascaded stages)</span>NF<sub>total</sub> &asymp; NF<sub>1</sub> + (NF<sub>2</sub>&minus;1)/G<sub>1</sub>  [linear quantities]</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Noise Figure Measurement Log</caption>
<thead><tr><th>DUT</th><th>Freq (MHz)</th><th>ENR (dB)</th><th>Y (dB)</th><th>NF (dB)</th></tr></thead>
<tbody>
<tr><td>SPF5189Z LNA</td><td>144</td><td>15.0</td><td>12.8</td><td>0.8</td></tr>
<tr><td>50 &Omega; thru (DUT=sys NF)</td><td>144</td><td>15.0</td><td>8.2</td><td>6.2</td></tr>
</tbody></table>
""",
},

"oscilloscope": {
    "tm": "TM-TGEAR-020",
    "title": "OSCILLOSCOPE",
    "subtitle": "Voltage-Time Waveform Display &mdash; FNIRSI OSCB35 100 MHz",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers the calibration procedures for the FNIRSI OSCB35 100 MHz digital oscilloscope and equivalent portable digital storage oscilloscopes (DSO). Calibrations covered: vertical scale (voltage accuracy), timebase (time/div), probe compensation, and trigger threshold.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-CAL-006 (Precision Voltage Reference), TM-CAL-002 (GPS 1PPS), TM-TGEAR-017 (Fluke 15B+ DMM).</p>
<div class="tm-warning"><span class="admonition-label">WARNING</span>If the oscilloscope is AC-mains powered, its BNC input grounds are connected to the power-supply chassis ground. Connecting the BNC ground clip to any non-earth-referenced circuit creates a shock hazard and can damage the circuit under test. Use a battery-powered oscilloscope for floating measurements, or use an isolation transformer on the oscilloscope power supply.</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. ANALOG FRONT END</h3>
<p>Input signal passes through an attenuator (for high-sensitivity settings), an AC/DC coupling switch, and a bandwidth-limiting filter. A buffer amplifier (typically 1 GHz GBP, e.g., AD8065) drives the ADC.</p>
<h3 class="section-heading">2-2. ADC AND MEMORY</h3>
<p>The FNIRSI OSCB35 uses a 12-bit ADC at 250 MSPS. Vertical resolution is 12 bits = 1/4096 of full scale. Effective Number of Bits (ENOB) in practice is 9&ndash;10 bits due to ADC noise and distortion.</p>
<h3 class="section-heading">2-3. TIMEBASE</h3>
<p>The timebase is generated from a crystal oscillator (typically 24 or 48 MHz TCXO in portable DSOs). The displayed time/div = (sample points &times; ADC period) / divisions. Accuracy depends on crystal stability (&plusmn;50 ppm for a standard crystal).</p>
<h3 class="section-heading">2-4. PROBE COMPENSATION</h3>
<p>A 10:1 probe uses a voltage divider (9 M&Omega; tip resistor in series with 1 M&Omega; scope input). A trim capacitor C<sub>comp</sub> in the probe tip compensates for cable and scope input capacitance to maintain flat frequency response.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<p>The oscilloscope is a commercial instrument. This chapter covers construction of a precision voltage reference step source for vertical calibration.</p>
<table class="tm-table"><caption>Precision Step Source Components</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>1</td><td>LM4040-2.5</td><td>2.500 V &plusmn;0.1% shunt reference</td></tr>
<tr><td>1</td><td>LM4040-5.0</td><td>5.000 V &plusmn;0.1% shunt reference</td></tr>
<tr><td>1</td><td>9 V battery</td><td>Supply for shunt references</td></tr>
<tr><td>1</td><td>BNC connector</td><td>Output to oscilloscope CH1</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Probe compensation: connect probe to PROBE COMP output on oscilloscope (usually 1 kHz, 2 V square wave). Adjust probe C<sub>comp</sub> trimmer until square wave top is flat (no overshoot or rounding). Use non-metallic adjustment tool.</li>
<li>Verify self-calibration: run built-in CAL routine (if present in FNIRSI firmware). This corrects internal ADC gain and offset.</li>
<li>Power oscilloscope from battery (if possible) to avoid AC mains ground interference.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<h3 class="section-heading">5-1. VERTICAL SCALE (VOLTAGE) CALIBRATION</h3>
<div class="procedure"><ol>
<li>Apply 2.500 V DC from LM4040 reference to CH1 via BNC cable. Set scope to DC coupling, 1 V/div.</li>
<li>Trigger on CH1. Measure displayed voltage. Correction factor = 2.500 / V<sub>displayed</sub>.</li>
<li>Repeat at 500 mV/div and 2 V/div. Log correction per range. FNIRSI spec: &plusmn;3% typical.</li>
</ol></div>
<h3 class="section-heading">5-2. TIMEBASE CALIBRATION</h3>
<div class="procedure"><ol>
<li>Apply GPS 1 PPS signal (TM-CAL-002) to CH1. 1 PPS = 1.000000000 s period, 100 ms pulse high.</li>
<li>Set timebase to 200 ms/div. Measure displayed pulse period. Error = (T<sub>displayed</sub> &minus; 1.000) / 1.000 &times; 10<sup>6</sup> ppm.</li>
<li>Alternatively, apply 60 Hz AC mains (via isolating capacitor 1 nF) and verify period = 16.67 ms &plusmn;0.01%.</li>
<li>At fast timebase (10 &mu;s/div), apply 10 kHz signal from GPS-calibrated function generator. Verify period = 100 &mu;s &plusmn;0.1%.</li>
</ol></div>
<h3 class="section-heading">5-3. BANDWIDTH VERIFICATION</h3>
<div class="procedure"><ol>
<li>Apply &minus;3 dB frequency from signal generator. For FNIRSI OSCB35, spec is 100 MHz (&minus;3 dB). Apply 100 MHz sine and verify that 10 MHz reading amplitude &times; 0.707 &asymp; 100 MHz reading.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>If vertical accuracy is off &gt;5%, the input attenuator trim capacitors may need adjustment (internal, service access). Timebase accuracy is determined by crystal; replace with TCXO module for &lt;2 ppm accuracy. Probe compensation trim is the only user-accessible mechanical adjustment.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>FNIRSI OSCB35 Acceptance Specifications</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Bandwidth</td><td>100 MHz (&minus;3 dB)</td></tr>
<tr><td>Sample rate</td><td>250 MSPS</td></tr>
<tr><td>Vertical accuracy</td><td>&plusmn;3% full scale</td></tr>
<tr><td>Timebase accuracy</td><td>&plusmn;50 ppm (crystal); &plusmn;1% typical as shipped</td></tr>
<tr><td>Input impedance</td><td>1 M&Omega; || 20 pF (1&times; probe)</td></tr>
<tr><td>Vertical resolution</td><td>12 bits (display), ENOB 9&ndash;10 bits</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Probe divider attenuation</span>A = R<sub>tip</sub> / (R<sub>tip</sub> + R<sub>in</sub>)  [R<sub>tip</sub>=9 M&Omega;, R<sub>in</sub>=1 M&Omega;, 10:1 probe]</div>
<div class="formula-block"><span class="formula-label">Rise time from bandwidth</span>t<sub>r</sub> &asymp; 0.35 / BW  [BW in Hz, t<sub>r</sub> in seconds]</div>
<div class="formula-block"><span class="formula-label">Timebase ppm error</span>&delta;T = (T<sub>measured</sub> &minus; T<sub>true</sub>) / T<sub>true</sub> &times; 10<sup>6</sup></div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>FNIRSI OSCB35 Calibration Log</caption>
<thead><tr><th>Test</th><th>Reference</th><th>Reading</th><th>Error</th><th>Pass/Fail</th></tr></thead>
<tbody>
<tr><td>Vertical 2.5 V DC</td><td>2.500 V</td><td>2.48 V</td><td>&minus;0.8%</td><td>Pass</td></tr>
<tr><td>Timebase 1 PPS</td><td>1.000 s</td><td>1.003 s</td><td>+3000 ppm</td><td>Pass</td></tr>
<tr><td>Bandwidth (100 MHz &minus;3 dB)</td><td>&minus;3 dB at 100 MHz</td><td>&minus;2.9 dB</td><td>Pass</td><td>Pass</td></tr>
</tbody></table>
""",
},

"q-meter": {
    "tm": "TM-TGEAR-021",
    "title": "Q METER",
    "subtitle": "Inductor and Capacitor Q Factor Measurement",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers the design, construction, and calibration of a Q meter for measuring the quality factor Q of inductors and capacitors at HF frequencies (1&ndash;30 MHz). The Q meter enables selection of high-Q inductors for filters and matching networks, and verifies coil construction quality.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-014 (Inductance Meter), TM-TGEAR-015 (LCR Meter), TM-TGEAR-007 (GDO).</p>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. SERIES RESONANCE METHOD</h3>
<p>The DUT inductor L and a calibrated variable capacitor C<sub>V</sub> are connected in series with a low-impedance RF voltage source (V<sub>inj</sub>). At resonance, the voltage across C<sub>V</sub> is Q times the source voltage:</p>
<div class="formula-block"><span class="formula-label">Q from resonant voltage</span>Q = V<sub>CV</sub> / V<sub>inj</sub></div>
<p>A high-impedance RF voltmeter reads V<sub>CV</sub>. Source V<sub>inj</sub> is kept constant by the injector oscillator. Q is read directly from the ratio.</p>
<h3 class="section-heading">2-2. Q FACTOR DEFINITION</h3>
<div class="formula-block"><span class="formula-label">Q factor</span>Q = 2&pi;f<sub>r</sub>L / R<sub>series</sub> = 1 / (2&pi;f<sub>r</sub>C &times; R<sub>series</sub>)</div>
<p>For RF coils wound on powdered-iron toroid: typical Q = 100&ndash;200 at 7 MHz. Air-core coils: Q = 200&ndash;500.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<table class="tm-table"><caption>Q Meter Core Components</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>1</td><td>Colpitts RF oscillator</td><td>1&ndash;30 MHz, swept by tuning cap</td></tr>
<tr><td>1</td><td>Injection coil (work coil)</td><td>1&ndash;2 turns, &lt;0.1 &Omega; resistance, Q&gt;200</td></tr>
<tr><td>1</td><td>Calibrated variable capacitor</td><td>20&ndash;400 pF, air-dielectric, micrometer dial</td></tr>
<tr><td>1</td><td>RF vacuum-tube voltmeter equivalent</td><td>High-Z JFET input, 100 mV FSD at 30 MHz</td></tr>
<tr><td>1</td><td>Low-inductance binding posts</td><td>DUT terminals, gold-plated</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Wind injection coil: 2 turns 18 AWG silver-plated wire on 20 mm PTFE form. Measure R<sub>DC</sub> &lt;0.05 &Omega; with 4-wire DMM.</li>
<li>Calibrate variable capacitor C<sub>V</sub> against LCR meter. Record actual C per dial division.</li>
<li>Align JFET voltmeter for linear response: apply known voltage from calibrated source, adjust meter calibration pot.</li>
<li>Zero-set: short DUT terminals, tune C<sub>V</sub> to resonance with work coil, verify Q meter reads zero current (no circulating current).</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>Connect known 100 &mu;H inductor (low-loss, air core) at 7 MHz. Tune C<sub>V</sub> for maximum voltage reading. Record Q.</li>
<li>Compare to LCR meter Q reading at 7 MHz. Values should agree within &plusmn;10%.</li>
<li>Calibrate injected voltage V<sub>inj</sub>: verify V<sub>inj</sub> = 100 mV &plusmn;1% with calibrated RF voltmeter. Adjust injector attenuator.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Injection voltage stability is critical: V<sub>inj</sub> drift of 1% causes 1% Q error. Use AGC on the injection oscillator (JFET gain control). Oscillator frequency is tuned by a calibrated dial referencing the variable capacitor.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>Q Meter Acceptance Specifications</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Frequency range</td><td>1&ndash;30 MHz</td></tr>
<tr><td>Q range</td><td>10&ndash;600</td></tr>
<tr><td>Q accuracy</td><td>&plusmn;5%</td></tr>
<tr><td>C<sub>V</sub> range</td><td>20&ndash;400 pF</td></tr>
<tr><td>Injected V</td><td>100 mV &plusmn;1%</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Q from voltage ratio</span>Q = V<sub>CV</sub> / V<sub>inj</sub></div>
<div class="formula-block"><span class="formula-label">Resonance condition</span>f<sub>r</sub> = 1 / (2&pi;&radic;(L &times; C<sub>V</sub>))</div>
<div class="formula-block"><span class="formula-label">Bandwidth from Q</span>BW = f<sub>r</sub> / Q  [&minus;3 dB bandwidth]</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Q Meter Measurement Log</caption>
<thead><tr><th>Inductor</th><th>Core</th><th>Freq (MHz)</th><th>Q (meter)</th><th>Q (LCR)</th></tr></thead>
<tbody>
<tr><td>T50-2 coil, 24t</td><td>Powdered iron</td><td>7.0</td><td>156</td><td>149</td></tr>
<tr><td>T50-6 coil, 24t</td><td>Powdered iron</td><td>14.0</td><td>220</td><td>214</td></tr>
<tr><td>Air core, 12t</td><td>None</td><td>14.0</td><td>410</td><td>398</td></tr>
</tbody></table>
""",
},

"rf-attenuator-set": {
    "tm": "TM-TGEAR-022",
    "title": "RF ATTENUATOR SET",
    "subtitle": "Fixed Step Attenuators for Signal Level Control and Calibration",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers construction, calibration, and use of a set of fixed RF attenuator pads: 1, 2, 3, 6, 10, 20, and 30 dB values. Used in series with spectrum analyzers, VNAs, and receivers to protect inputs and to establish calibrated signal levels. Relates to TM-CAL-015 which covers precision attenuator pads as calibration standards.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-CAL-015 (Attenuator Pad Standards), TM-TGEAR-018 (NanoVNA), TM-TGEAR-028 (TinySA).</p>
<div class="tm-caution"><span class="admonition-label">CAUTION</span>Maximum continuous input power: 2 W (for 1/4 W resistor attenuators). For transmitter testing, use a dedicated 50 W or 100 W attenuator. Do not exceed resistor power rating.</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. PI AND T TOPOLOGY</h3>
<p>Pi-pad (two shunt + one series resistor) and T-pad (two series + one shunt resistor) provide broadband attenuation from DC to several GHz when built with short lead lengths and housed in a shielded enclosure.</p>
<div class="formula-block"><span class="formula-label">Pi pad shunt resistors (50 &Omega;)</span>R<sub>shunt</sub> = 50 &times; (k+1)/(k&minus;1)  [k = 10<sup>A/20</sup>]</div>
<div class="formula-block"><span class="formula-label">Pi pad series resistor (50 &Omega;)</span>R<sub>series</sub> = 50 &times; 2k/(k<sup>2</sup>&minus;1)</div>
<h3 class="section-heading">2-2. CASCADING PADS</h3>
<p>Attenuations add in dB when pads are cascaded in series (assuming all pads have equal and correct 50 &Omega; impedance). Any mismatch between cascade stages causes less than additive behavior.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<table class="tm-table"><caption>6 dB Pi Pad Resistor Values</caption>
<thead><tr><th>Resistor</th><th>Calculated</th><th>1% Standard</th><th>Function</th></tr></thead>
<tbody>
<tr><td>R1, R3 (shunt)</td><td>150.5 &Omega;</td><td>150 &Omega;</td><td>Input/output shunt to GND</td></tr>
<tr><td>R2 (series)</td><td>37.4 &Omega;</td><td>37.4 &Omega;</td><td>Series through arm</td></tr>
</tbody></table>
<p>Use 0.1% metal-film resistors for calibration-grade pads. Use standard 1% resistors for signal-routing pads. Enclose in SMA-SMA barrel housing or custom PCB in copper-clad enclosure.</p>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Measure all resistors before soldering. Select resistors closest to target value from a batch of 10.</li>
<li>Solder in pi configuration on copper-clad board. Keep all leads &lt;5 mm.</li>
<li>Install in SMA barrel or BNC-BNC adapter housing. Verify no shorts with DMM before closing case.</li>
<li>Label with attenuation value and impedance on enclosure.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>Connect NanoVNA: port 1 &rarr; attenuator input, port 2 &rarr; attenuator output. Perform SOLT calibration first.</li>
<li>Measure S21 at 1, 10, 50, 100, 200, 300 MHz. Record actual attenuation at each frequency.</li>
<li>Attenuation flatness: &lt;&plusmn;0.2 dB from 1 to 100 MHz; &lt;&plusmn;0.5 dB from 100 to 300 MHz.</li>
<li>Measure S11: input return loss must be &gt;20 dB (SWR &lt;1.22:1) at all frequencies.</li>
<li>Record correction table for each pad. Correction = A<sub>nominal</sub> &minus; A<sub>measured</sub>.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Fixed pads are not adjustable after construction. If attenuation error &gt;0.5 dB, replace R2 (series resistor) with a closer value. A 1% error in any single resistor typically causes &lt;0.2 dB attenuation error in a 6 dB pad.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>Attenuator Set Acceptance Specifications</caption>
<thead><tr><th>Pad</th><th>Nominal (dB)</th><th>Accuracy</th><th>Flatness (1-100 MHz)</th></tr></thead>
<tbody>
<tr><td>1 dB</td><td>1.00</td><td>&plusmn;0.2 dB</td><td>&plusmn;0.1 dB</td></tr>
<tr><td>3 dB</td><td>3.00</td><td>&plusmn;0.2 dB</td><td>&plusmn;0.1 dB</td></tr>
<tr><td>6 dB</td><td>6.00</td><td>&plusmn;0.2 dB</td><td>&plusmn;0.1 dB</td></tr>
<tr><td>10 dB</td><td>10.00</td><td>&plusmn;0.2 dB</td><td>&plusmn;0.1 dB</td></tr>
<tr><td>20 dB</td><td>20.00</td><td>&plusmn;0.2 dB</td><td>&plusmn;0.2 dB</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Voltage attenuation ratio</span>k = 10<sup>A/20</sup></div>
<div class="formula-block"><span class="formula-label">T pad series arms</span>R<sub>series</sub> = Z<sub>0</sub>(k&minus;1)/(k+1)</div>
<div class="formula-block"><span class="formula-label">T pad shunt arm</span>R<sub>shunt</sub> = Z<sub>0</sub> &times; 2k/(k<sup>2</sup>&minus;1)</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Attenuator Set Calibration Log (NanoVNA S21)</caption>
<thead><tr><th>Pad</th><th>1 MHz</th><th>10 MHz</th><th>100 MHz</th><th>300 MHz</th></tr></thead>
<tbody>
<tr><td>3 dB</td><td>3.01</td><td>3.01</td><td>3.03</td><td>3.08</td></tr>
<tr><td>6 dB</td><td>6.02</td><td>6.01</td><td>6.04</td><td>6.12</td></tr>
<tr><td>10 dB</td><td>9.98</td><td>9.97</td><td>10.01</td><td>10.10</td></tr>
<tr><td>20 dB</td><td>20.1</td><td>20.1</td><td>20.2</td><td>20.4</td></tr>
</tbody></table>
""",
},

"rf-power-meter": {
    "tm": "TM-TGEAR-023",
    "title": "RF POWER METER",
    "subtitle": "RF Power Measurement, milliwatts to 100 Watts, HF through VHF",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers design, construction, and calibration of an RF power meter for measuring forward and reflected power at HF and VHF (1&ndash;150 MHz). A dual-directional coupler and a calibrated RF detector produce readings in watts and dBm.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-008 (Dummy Load), TM-TGEAR-027 (SWR Meter), TM-CAL-014 (Dummy Load Standard), TM-CAL-015 (Attenuator Pads).</p>
<div class="tm-caution"><span class="admonition-label">CAUTION</span>The directional coupler is rated for maximum 100 W continuous. Do not apply SSB peak power (typically 4&times; average) without accounting for PEP rating. Maximum safe PEP for 100 W average-rated couplers is 400 W.</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. DIRECTIONAL COUPLER</h3>
<p>A toroidal directional coupler (Sontheimer-Fredrick bridge or Bruene coupler) samples the forward and reflected voltage waves on the transmission line. Coupling factor C<sub>f</sub> is typically &minus;20 to &minus;30 dB.</p>
<div class="formula-block"><span class="formula-label">Coupled power</span>P<sub>coupled</sub> = P<sub>forward</sub> &times; 10<sup>C<sub>f</sub>/10</sup></div>
<h3 class="section-heading">2-2. RF DETECTOR</h3>
<p>A Schottky diode (1N5711 or BAT85) rectifies the coupled RF signal. For small-signal operation (&lt;200 mV input), the diode operates in the square-law region: output DC voltage is proportional to input RF power. For large signals, operation is in the linear region.</p>
<div class="formula-block"><span class="formula-label">Detector output (square law)</span>V<sub>DC</sub> = k &times; P<sub>RF</sub>  [k = detector sensitivity constant, V/mW]</div>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<table class="tm-table"><caption>Bruene Coupler Components (100 W, HF)</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>1</td><td>FT-50-43 core</td><td>Mix 43 ferrite, toroid, 12.7 mm OD</td></tr>
<tr><td>10</td><td>Turns of 28 AWG</td><td>Secondary winding (10:1 ratio)</td></tr>
<tr><td>2</td><td>1N5711 diodes</td><td>Schottky, forward/reverse detector</td></tr>
<tr><td>2</td><td>100 pF ceramic caps</td><td>RF bypass at detector output</td></tr>
<tr><td>2</td><td>4.7 k&Omega; resistors</td><td>DC load for detector</td></tr>
<tr><td>1</td><td>SO-239 connectors &times; 2</td><td>Input/output/antenna ports</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Wind 10 turns of #28 enameled wire through FT-50-43 ferrite bead. This is the secondary; the through-line is the primary (1 turn).</li>
<li>Install forward and reflected detector diodes per schematic. Verify diode orientation: forward detector anode to secondary, reflected detector anode to secondary with reversed polarity arm.</li>
<li>Connect 4.7 k&Omega; loads and 100 pF bypass caps at each detector output.</li>
<li>Connect meter movements or ADC to detector DC outputs.</li>
<li>Apply 5 W at 14 MHz from calibrated source into dummy load. Measure forward detector voltage. Compute calibration constant K = P / V<sub>DC</sub>.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>Connect power meter in line between known power source and precision 50 &Omega; dummy load (TM-CAL-014).</li>
<li>Apply 1 W at 7 MHz. Adjust forward power calibration pot until display reads 1.00 W.</li>
<li>Verify at 5 W and 10 W. Linearity error &lt;&plusmn;5%.</li>
<li>Apply known signal generator output at &minus;10 dBm (0.1 mW). For small-signal meter: adjust for 0.10 mW reading &plusmn;2%.</li>
<li>Verify SWR accuracy: connect 2:1 SWR test load (100 &Omega;). SWR must read 2.0 &plusmn;0.1.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Frequency flatness of detector response is improved by using a low-capacitance Schottky diode. For HF-only use, response is flat within &plusmn;0.5 dB from 1 to 30 MHz. For VHF (50&ndash;150 MHz), add a compensation capacitor (5&ndash;10 pF) across the detector load resistor to pre-emphasize higher frequencies.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>RF Power Meter Acceptance Specifications</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Power range</td><td>0.1 mW &ndash; 100 W</td></tr>
<tr><td>Frequency range</td><td>1&ndash;150 MHz</td></tr>
<tr><td>Power accuracy</td><td>&plusmn;5% full scale</td></tr>
<tr><td>SWR accuracy</td><td>&plusmn;0.1 at SWR 1:1 to 3:1</td></tr>
<tr><td>Directivity</td><td>&gt;30 dB (Bruene coupler, &lt;30 MHz)</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Power from RMS voltage (50 &Omega;)</span>P = V<sub>RMS</sub><sup>2</sup> / 50</div>
<div class="formula-block"><span class="formula-label">dBm from power in mW</span>dBm = 10 log<sub>10</sub>(P<sub>mW</sub> / 1 mW)</div>
<div class="formula-block"><span class="formula-label">Reflected power from SWR</span>P<sub>ref</sub> = P<sub>fwd</sub> &times; ((SWR&minus;1)/(SWR+1))<sup>2</sup></div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>RF Power Meter Calibration Log</caption>
<thead><tr><th>Input Power</th><th>Frequency</th><th>Meter Reading</th><th>Error</th></tr></thead>
<tbody>
<tr><td>1.00 W</td><td>7 MHz</td><td>1.02 W</td><td>+2.0%</td></tr>
<tr><td>5.00 W</td><td>14 MHz</td><td>4.95 W</td><td>&minus;1.0%</td></tr>
<tr><td>10.00 W</td><td>28 MHz</td><td>10.1 W</td><td>+1.0%</td></tr>
</tbody></table>
""",
},

"rf-probe-detector": {
    "tm": "TM-TGEAR-024",
    "title": "RF PROBE AND DETECTOR",
    "subtitle": "RF Voltage Probe for Oscilloscope and DMM RF Measurements",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers construction and calibration of an RF probe for measuring RF voltage at circuit nodes with an oscilloscope or DMM. Two designs are covered: a simple peak detector probe (diode + capacitor, DC output to DMM) and an active 50 &Omega; buffer probe for oscilloscope use.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-017 (DMM), TM-TGEAR-020 (Oscilloscope), TM-CAL-006 (Voltage Reference).</p>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. PEAK DETECTOR PROBE</h3>
<p>A high-frequency diode (1N60 germanium or BAT85 Schottky) rectifies the RF signal. A hold capacitor (100 nF) maintains the peak voltage at the DMM input. The output DC voltage V<sub>DC</sub> &asymp; V<sub>peak,RF</sub> &minus; V<sub>diode,forward</sub>. Germanium diode forward drop &asymp; 0.2 V; Schottky &asymp; 0.2&ndash;0.3 V.</p>
<div class="formula-block"><span class="formula-label">RMS from peak (sine wave)</span>V<sub>RMS</sub> = (V<sub>DC</sub> + V<sub>f</sub>) / &radic;2</div>
<h3 class="section-heading">2-2. ACTIVE BUFFER PROBE</h3>
<p>A JFET source follower (BF245C) provides high input impedance (&gt;10 M&Omega;) and low output impedance (&lt;100 &Omega;) for driving coaxial cable to an oscilloscope. Bandwidth is limited by JFET capacitance (C<sub>gs</sub> &asymp; 5 pF).</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<table class="tm-table"><caption>Peak Detector Probe Components</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>1</td><td>BAT85 diode</td><td>Schottky, V<sub>f</sub>=0.25 V at 1 mA, 200 MHz</td></tr>
<tr><td>1</td><td>100 nF capacitor</td><td>Hold capacitor, ceramic 50 V</td></tr>
<tr><td>1</td><td>10 M&Omega; resistor</td><td>Bleeder/DC return for DMM input</td></tr>
<tr><td>1</td><td>Probe tip</td><td>Stiff 0.3 mm silver wire, 30 mm</td></tr>
<tr><td>1</td><td>Ground clip lead</td><td>Alligator clip, 150 mm</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Solder BAT85 diode in a small copper-clad PCB (10 &times; 10 mm), anode to probe tip, cathode to hold capacitor.</li>
<li>Install 100 nF hold cap between cathode and ground. Install 10 M&Omega; bleeder resistor in parallel with cap.</li>
<li>Connect output (cathode side) to DMM input lead. Connect ground to DMM COM lead.</li>
<li>House in heat-shrink tubing with only probe tip exposed.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>Apply known RF signal (e.g., 7 MHz, 1.0 V<sub>RMS</sub> = 2.83 V<sub>pp</sub>) from calibrated signal generator.</li>
<li>Measure probe output with DMM DC range. Expected: V<sub>DC</sub> = V<sub>peak</sub> &minus; V<sub>f</sub> = 1.414 &minus; 0.25 = 1.164 V.</li>
<li>Compute calibration factor K = V<sub>RMS_true</sub> / V<sub>DC</sub>. Apply this factor to all subsequent readings.</li>
<li>Verify frequency response: apply same V<sub>RMS</sub> at 1 MHz and 30 MHz. Output should not vary &gt;10%.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>For improved accuracy, replace simple peak detector with a precision rectifier op-amp circuit (AD8307 logarithmic detector). The AD8307 provides calibrated dBm output from &minus;75 to +17 dBm with &plusmn;1 dB accuracy at 100 MHz.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>RF Probe Acceptance Specifications</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Frequency range</td><td>100 kHz &ndash; 100 MHz (peak detector)</td></tr>
<tr><td>Accuracy (after calibration)</td><td>&plusmn;5% V<sub>RMS</sub></td></tr>
<tr><td>Input impedance</td><td>&gt;10 M&Omega; || 5 pF (at tip)</td></tr>
<tr><td>Maximum input</td><td>50 V<sub>peak</sub></td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Peak from RMS</span>V<sub>peak</sub> = V<sub>RMS</sub> &times; &radic;2  (sine wave)</div>
<div class="formula-block"><span class="formula-label">V<sub>RMS</sub> from peak detector output</span>V<sub>RMS</sub> = (V<sub>DC</sub> + V<sub>f</sub>) / &radic;2</div>
<div class="formula-block"><span class="formula-label">Power from V<sub>RMS</sub> into 50 &Omega;</span>P = V<sub>RMS</sub><sup>2</sup> / 50</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>RF Probe Calibration Log</caption>
<thead><tr><th>Applied V<sub>RMS</sub></th><th>Freq</th><th>V<sub>DC</sub> (probe)</th><th>Computed V<sub>RMS</sub></th><th>Error</th></tr></thead>
<tbody>
<tr><td>1.000 V</td><td>7 MHz</td><td>1.165 V</td><td>0.998 V</td><td>&minus;0.2%</td></tr>
<tr><td>0.500 V</td><td>14 MHz</td><td>0.557 V</td><td>0.571 V</td><td>+1.4% (diode error)</td></tr>
</tbody></table>
""",
},

"rf-signal-tracer": {
    "tm": "TM-TGEAR-025",
    "title": "RF SIGNAL TRACER",
    "subtitle": "Signal Injection and Tracing Tool for RF/IF Alignment",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers construction and calibration of an RF signal tracer / signal injector combination tool. Used to troubleshoot receivers, amplifiers, and IF circuits by injecting a known signal at various stages and probing circuit nodes for the signal's presence. Applicable to AM/FM broadcast receiver repair, IF chain alignment, and audio amplifier fault isolation.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-024 (RF Probe/Detector), TM-TGEAR-026 (Signal Generator), TM-TGEAR-020 (Oscilloscope).</p>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. SIGNAL INJECTION</h3>
<p>A modulated carrier (AM: 455 kHz or 10.7 MHz; audio: 1 kHz) is injected into successive stages of the circuit under test. If sound appears in the loudspeaker after a particular injection point but not before it, the fault is localized to that stage or earlier.</p>
<h3 class="section-heading">2-2. SIGNAL TRACING</h3>
<p>A high-gain audio amplifier with RF detector in the probe follows the signal from the antenna through each stage. Loss of signal at a stage pinpoints the fault. The probe audio output goes to a speaker or headphone for acoustic monitoring.</p>
<h3 class="section-heading">2-3. ISOLATION CAPACITOR</h3>
<p>A 0.01 &micro;F capacitor in series with the injection probe blocks DC voltage from the circuit under test from damaging the oscillator. Similarly, a 100 pF series cap at the probe tip of the tracing probe limits impedance loading and DC interaction.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<table class="tm-table"><caption>Signal Tracer/Injector Components</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>1</td><td>455 kHz ceramic resonator</td><td>IF standard, &plusmn;0.5% tolerance</td></tr>
<tr><td>1</td><td>NE555 oscillator</td><td>1 kHz audio modulation source</td></tr>
<tr><td>1</td><td>AM modulator</td><td>Simple multiplier: 1N4148 diode ring</td></tr>
<tr><td>1</td><td>LM386 audio amp</td><td>1 W, 8 &Omega; speaker output, tracer amp</td></tr>
<tr><td>1</td><td>BAT85 detector diode</td><td>RF detector at probe tip</td></tr>
<tr><td>1</td><td>8 &Omega; 0.5 W speaker</td><td>Monitoring output</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Build 455 kHz oscillator. Verify frequency with counter; should be 455 kHz &plusmn;5 kHz.</li>
<li>Build 1 kHz modulator. Apply to 455 kHz carrier; verify 30% AM modulation depth on oscilloscope.</li>
<li>Build LM386 tracer amplifier. Verify audio output with 1 kHz sine at input &minus;60 dBm.</li>
<li>Install RF detector diode at probe tip. Connect to LM386 input via 100 pF coupling cap.</li>
<li>Test: inject at a known-good AM receiver IF output; speaker should produce audible 1 kHz tone.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>Calibrate injection frequency: connect counter to injector output. Verify 455 kHz &plusmn;2 kHz (centered in IF passband).</li>
<li>Set modulation depth: connect to oscilloscope. Adjust modulator trimmer for 30% AM (&plusmn;5%).</li>
<li>Calibrate tracer sensitivity: apply &minus;40 dBm at 455 kHz to tracer probe. Verify audible output at speaker. Minimum detectable level should be &minus;50 dBm or better.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Injector frequency can be offset by &plusmn;10 kHz using a trimmer capacitor in the oscillator circuit, to locate the center of IF passbands. Tracer sensitivity is set by LM386 gain pot; high gain may cause audio feedback through the probe-to-circuit coupling.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>Signal Tracer/Injector Acceptance Specs</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Injection frequency</td><td>455 kHz &plusmn;5 kHz (AM IF), 10.7 MHz &plusmn;50 kHz (FM IF)</td></tr>
<tr><td>Modulation depth</td><td>30% AM &plusmn;5%</td></tr>
<tr><td>Tracer sensitivity</td><td>&minus;50 dBm minimum</td></tr>
<tr><td>Tracer frequency range</td><td>100 kHz &ndash; 100 MHz</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">AM modulation depth</span>m = (V<sub>max</sub> &minus; V<sub>min</sub>) / (V<sub>max</sub> + V<sub>min</sub>)</div>
<div class="formula-block"><span class="formula-label">IF passband relationship</span>BW<sub>IF</sub> = f<sub>IF</sub> / Q<sub>ceramic_filter</sub></div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Signal Tracer Test Log</caption>
<thead><tr><th>Receiver Stage</th><th>Signal Present?</th><th>Level</th></tr></thead>
<tbody>
<tr><td>Antenna terminal</td><td>Yes</td><td>&minus;70 dBm (broadcast)</td></tr>
<tr><td>After RF amp</td><td>Yes</td><td>&minus;50 dBm</td></tr>
<tr><td>After mixer</td><td>Yes (IF freq)</td><td>&minus;55 dBm</td></tr>
<tr><td>After IF filter</td><td>No (fault)</td><td>&lt;&minus;80 dBm</td></tr>
</tbody></table>
""",
},

"signal-generator": {
    "tm": "TM-TGEAR-026",
    "title": "SIGNAL GENERATOR",
    "subtitle": "RF Signal Source, 100 kHz to 150 MHz, Calibrated Output Level",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers design, construction, and calibration of an RF signal generator from 100 kHz to 150 MHz with calibrated output levels from &minus;120 dBm to 0 dBm. Applications include receiver sensitivity testing, antenna analyzer input, and filter response measurement.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-010 (Frequency Counter), TM-CAL-001 (WWV), TM-CAL-002 (GPS 1PPS), TM-CAL-015 (Attenuator Pads).</p>
<div class="tm-warning"><span class="admonition-label">WARNING</span>If this signal generator is powered from AC mains, use an isolation transformer on the power supply. RF output ground connects to the instrument chassis. Connecting to a second AC-powered instrument without isolation can create ground loops affecting measurement accuracy or creating shock hazard.</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. DDS SYNTHESIS</h3>
<p>A DDS chip (AD9850 or AD9833) generates a stable, phase-continuous output from DC to 40 MHz (AD9850) or 12.5 MHz (AD9833). Above these limits, a PLL multiplier extends coverage to 150 MHz.</p>
<div class="formula-block"><span class="formula-label">DDS output frequency</span>f<sub>out</sub> = (FTW &times; f<sub>clk</sub>) / 2<sup>32</sup>  [FTW = 32-bit frequency tuning word]</div>
<h3 class="section-heading">2-2. OUTPUT LEVEL CONTROL</h3>
<p>A step attenuator (0, 10, 20, 30, 40, 50, 60 dB) plus a variable attenuator provides &minus;120 to 0 dBm output range. The step attenuator is calibrated (TM-TGEAR-022). Total level accuracy: &plusmn;2 dB.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<table class="tm-table"><caption>Signal Generator Components</caption>
<thead><tr><th>Block</th><th>Part</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>DDS module</td><td>AD9850 module</td><td>0&ndash;40 MHz, 125 MHz clock, SPI, 10-bit DAC</td></tr>
<tr><td>PLL multiplier (optional)</td><td>ADF4351</td><td>35 MHz &ndash; 4.4 GHz PLL synthesizer</td></tr>
<tr><td>Step attenuator</td><td>6 &times; pi-pads</td><td>10 dB each, PIN diode or relay-switched</td></tr>
<tr><td>Variable attenuator</td><td>PE4302 IC</td><td>0&ndash;31.5 dB, 0.5 dB steps, RF CMOS</td></tr>
<tr><td>MCU</td><td>Arduino Nano</td><td>SPI control, encoder interface, display</td></tr>
<tr><td>Output</td><td>BNC or SMA</td><td>50 &Omega;</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Assemble DDS module on shielded PCB. Verify DDS output with frequency counter at several frequencies.</li>
<li>Build output amplifier (MAR-6 MMIC or MGA-82563): gain 20 dB, NF &lt;5 dB, P1dB &gt;+12 dBm.</li>
<li>Install step attenuator chain. Calibrate each pad per TM-TGEAR-022 before assembly.</li>
<li>Install PE4302 digital attenuator. Program via SPI; verify each 0.5 dB step with TinySA.</li>
<li>Power on. Sweep 1&ndash;30 MHz. Output level should be flat &plusmn;2 dB.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<h3 class="section-heading">5-1. FREQUENCY CALIBRATION</h3>
<div class="procedure"><ol>
<li>Set generator to 10.000000 MHz. Connect to frequency counter (GPS-disciplined, TM-TGEAR-010).</li>
<li>Measure actual frequency. Compute ppm error. Adjust DDS clock frequency constant in firmware.</li>
<li>Cross-check against WWV 10 MHz reception with zero-beat method per TM-CAL-001.</li>
</ol></div>
<h3 class="section-heading">5-2. OUTPUT LEVEL CALIBRATION</h3>
<div class="procedure"><ol>
<li>Set attenuator to 0 dB (max output). Measure level with TinySA at 10 MHz. Record P<sub>max</sub> (dBm).</li>
<li>Switch each step attenuator in. Verify each step reduces level by 10 &plusmn;0.5 dB.</li>
<li>Calibrate PE4302 digital attenuator at 0, 10, 20, 30 dB settings. Record actual vs. nominal and create correction table.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Level flatness vs. frequency is improved by equalizing the amplifier chain. Place a low-pass filter at the DDS output (50 MHz cutoff 7th-order elliptic) to suppress DDS clock spurs. Add a 3 dB pad between DDS amp and attenuator chain to improve output match.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>Signal Generator Acceptance Specifications</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Frequency range</td><td>100 kHz &ndash; 40 MHz (DDS), to 150 MHz (PLL)</td></tr>
<tr><td>Frequency accuracy</td><td>&lt;1 ppm (GPS-corrected DDS clock)</td></tr>
<tr><td>Level range</td><td>&minus;120 to 0 dBm</td></tr>
<tr><td>Level accuracy</td><td>&plusmn;2 dB</td></tr>
<tr><td>Harmonics</td><td>&lt;&minus;30 dBc at output (after LPF)</td></tr>
<tr><td>Spurious</td><td>&lt;&minus;50 dBc (DDS clock and aliases)</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">AD9850 frequency tuning word</span>FTW = f<sub>out</sub> &times; 2<sup>32</sup> / f<sub>clk</sub></div>
<div class="formula-block"><span class="formula-label">Voltage from dBm into 50 &Omega;</span>V<sub>RMS</sub> = &radic;(R &times; 10<sup>P<sub>dBm</sub>/10</sup> &times; 10<sup>&minus;3</sup>) = &radic;(50 &times; P<sub>mW</sub>&times;10<sup>&minus;3</sup>)</div>
<div class="formula-block"><span class="formula-label">Total attenuation (cascaded pads)</span>A<sub>total</sub> = A<sub>1</sub> + A<sub>2</sub> + ... + A<sub>n</sub>  (dB)</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Signal Generator Calibration Log</caption>
<thead><tr><th>Frequency</th><th>Attenuation Setting</th><th>Measured Level</th><th>Error</th></tr></thead>
<tbody>
<tr><td>7 MHz</td><td>0 dB (max out)</td><td>+1.2 dBm</td><td>&mdash;</td></tr>
<tr><td>7 MHz</td><td>20 dB</td><td>&minus;18.9 dBm</td><td>+0.1 dB</td></tr>
<tr><td>14 MHz</td><td>40 dB</td><td>&minus;38.7 dBm</td><td>&minus;0.3 dB (at +1.2 dBm base)</td></tr>
</tbody></table>
""",
},

"swr-meter": {
    "tm": "TM-TGEAR-027",
    "title": "SWR METER",
    "subtitle": "Standing Wave Ratio Bridge, 1.8 to 150 MHz",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers design, construction, and calibration of an SWR meter (standing wave ratio bridge) for HF and VHF amateur radio operation. The SWR meter measures forward and reflected power, computes SWR, and provides a visual indication of antenna system match quality.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-TGEAR-023 (RF Power Meter), TM-TGEAR-008 (Dummy Load), TM-CAL-014 (Dummy Load), TM-CAL-015 (Attenuator Pads).</p>
<div class="tm-caution"><span class="admonition-label">CAUTION</span>Do not leave high SWR (&gt;3:1) loads on a transmitter for extended periods. At SWR 3:1, reflected power is 25% of forward power, returning to the final amplifier and increasing its dissipation. Maximum operating SWR for most solid-state final amplifiers: &lt;2:1.</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. DIRECTIONAL BRIDGE (BRUENE COUPLER)</h3>
<p>The Bruene coupler uses a toroidal transformer to sample current and a voltage divider to sample voltage on the transmission line. By adding and subtracting these samples, separate forward (V<sub>F</sub>) and reflected (V<sub>R</sub>) voltages are obtained.</p>
<div class="formula-block"><span class="formula-label">SWR from V<sub>F</sub> and V<sub>R</sub></span>SWR = (V<sub>F</sub> + V<sub>R</sub>) / (V<sub>F</sub> &minus; V<sub>R</sub>)</div>
<div class="formula-block"><span class="formula-label">Reflection coefficient</span>&Gamma; = V<sub>R</sub> / V<sub>F</sub></div>
<h3 class="section-heading">2-2. METER CIRCUIT</h3>
<p>Schottky diodes (1N5711 or BAT85) rectify the sampled forward and reflected voltages. DC-coupled meter movements display the power. An analog ratio circuit or MCU computes and displays SWR.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<table class="tm-table"><caption>SWR Meter Components (HF, 100 W)</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>1</td><td>FT-50-43 ferrite toroid</td><td>Current transformer, 10 turns secondary</td></tr>
<tr><td>2</td><td>1N5711 Schottky diodes</td><td>Forward/reflected detectors</td></tr>
<tr><td>2</td><td>1 k&Omega; resistors</td><td>Voltage divider for bridge balance</td></tr>
<tr><td>2</td><td>100 &mu;A meter movements</td><td>FWD/REF analog display</td></tr>
<tr><td>1</td><td>SPDT CAL/SWR switch</td><td>Selects CAL vs. SWR readout mode</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Wind 10 turns of #28 enameled wire through FT-50-43 bead. Pass through-line (coax center conductor) through core as 1-turn primary.</li>
<li>Terminate secondary with 50 &Omega; load on far end. Connect forward detector to one end and reflected detector to other end via bridge balance resistors.</li>
<li>Install meter movements in series with detector output. Set FSD = 100 W forward power via calibration resistors.</li>
<li>Verify balance: apply signal and short antenna port. Reflected power meter should read full scale; forward should drop appropriately.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>Connect 50 &Omega; precision dummy load to antenna port. Apply 10 W from transmitter.</li>
<li>CAL switch to FWD. Adjust CAL pot for full-scale deflection on forward meter.</li>
<li>Switch to SWR. Meter should read 1.0 (minimum deflection) with 50 &Omega; load.</li>
<li>Connect 150 &Omega; load (3:1 SWR). Meter must read 3.0 &plusmn;0.2.</li>
<li>Verify directivity: terminate antenna port with 50 &Omega; dummy load. Apply signal at transmitter port. REF meter reading must be &lt;5% of FWD reading (directivity &gt;26 dB).</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Bridge balance is adjusted by the 1 k&Omega; trim resistors in the coupling network. Optimal balance (maximum directivity) occurs when the voltage sample V<sub>volt</sub> and current sample V<sub>curr</sub> amplitudes are equal at the detectors. Adjust trimmers with a signal generator and terminated load for minimum reflected power reading.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>SWR Meter Acceptance Specifications</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Frequency range</td><td>1.8 &ndash; 150 MHz</td></tr>
<tr><td>SWR accuracy</td><td>&plusmn;5% at SWR 1:1 to 5:1</td></tr>
<tr><td>Power range</td><td>5&ndash;100 W (full scale)</td></tr>
<tr><td>Directivity</td><td>&gt;25 dB (HF); &gt;20 dB (VHF)</td></tr>
<tr><td>Insertion loss</td><td>&lt;0.1 dB at 100 W</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">SWR from V<sub>F</sub> and V<sub>R</sub></span>SWR = (V<sub>F</sub> + V<sub>R</sub>) / (V<sub>F</sub> &minus; V<sub>R</sub>)</div>
<div class="formula-block"><span class="formula-label">Reflected power fraction</span>P<sub>ref</sub>/P<sub>fwd</sub> = &Gamma;<sup>2</sup> = ((SWR&minus;1)/(SWR+1))<sup>2</sup></div>
<div class="formula-block"><span class="formula-label">Directivity (dB)</span>D = 20 log<sub>10</sub>(V<sub>F,from_load</sub> / V<sub>R,from_load</sub>)  [50 &Omega; load connected]</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>SWR Meter Calibration Log</caption>
<thead><tr><th>Load</th><th>SWR Expected</th><th>SWR Measured</th><th>Error</th></tr></thead>
<tbody>
<tr><td>50 &Omega;</td><td>1.00</td><td>1.01</td><td>+1%</td></tr>
<tr><td>100 &Omega;</td><td>2.00</td><td>2.03</td><td>+1.5%</td></tr>
<tr><td>150 &Omega;</td><td>3.00</td><td>2.97</td><td>&minus;1.0%</td></tr>
<tr><td>25 &Omega;</td><td>2.00</td><td>1.98</td><td>&minus;1.0%</td></tr>
</tbody></table>
""",
},

"tinysa-spectrum-analyzer": {
    "tm": "TM-TGEAR-028",
    "title": "TINYSA ULTRA SPECTRUM ANALYZER",
    "subtitle": "Frequency Sweep and Spectrum Measurement, 100 kHz to 6 GHz",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers the calibration and use of the TinySA Ultra spectrum analyzer for measurements from 100 kHz to 6 GHz. Calibrations include: internal reference calibration via FM broadcast, level calibration with attenuator pads, frequency axis calibration, and sweep accuracy verification. The TinySA Ultra is used as the primary level and spectral measurement instrument in this test equipment suite.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-CAL-004 (FM Broadcast Reference), TM-CAL-015 (Attenuator Pads), TM-CAL-002 (GPS 1PPS), TM-TGEAR-022 (RF Attenuator Set).</p>
<div class="tm-caution"><span class="admonition-label">CAUTION</span>Maximum input power HIGH port: +10 dBm continuous, +20 dBm for &lt;1 second. Exceeding this will destroy the mixer/input amplifier. Always verify signal level with a power meter before connecting to TinySA. Use a calibrated attenuator pad when connecting transmitter outputs.</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. SUPERHETERODYNE SWEEP ARCHITECTURE</h3>
<p>The TinySA Ultra uses a swept superheterodyne architecture. An internal VCO sweeps the local oscillator (LO) frequency. The mixer produces an IF signal at a fixed frequency (typically 433 MHz or 21.4 MHz for different bands). A bandpass IF filter determines the resolution bandwidth (RBW). The IF signal is detected, digitized, and displayed as power vs. frequency.</p>
<h3 class="section-heading">2-2. RESOLUTION BANDWIDTH AND NOISE FLOOR</h3>
<p>Reducing RBW reduces noise floor at the cost of longer sweep time. The sensitivity (noise floor) is:</p>
<div class="formula-block"><span class="formula-label">Thermal noise floor</span>N = kTB = &minus;174 + NF + 10log<sub>10</sub>(RBW) dBm</div>
<p>For TinySA Ultra at RBW = 10 kHz and NF = 8 dB: N = &minus;174 + 8 + 40 = &minus;126 dBm.</p>
<h3 class="section-heading">2-3. INTERNAL REFERENCE</h3>
<p>The TinySA Ultra has an internal TCXO reference. Using the built-in signal generator output (from the LO output port) for self-calibration, or referencing against FM broadcast carriers per TM-CAL-004.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<p>The TinySA Ultra is a commercial product. This chapter covers construction of a soft carrying/calibration kit.</p>
<table class="tm-table"><caption>TinySA Ultra Calibration Accessory Kit</caption>
<thead><tr><th>Item</th><th>Description</th><th>Used For</th></tr></thead>
<tbody>
<tr><td>6 dB SMA pi-pad</td><td>TM-CAL-015 standard</td><td>Level calibration, input protection</td></tr>
<tr><td>20 dB SMA pi-pad</td><td>TM-CAL-015 standard</td><td>High-level signal reduction</td></tr>
<tr><td>SMA male 50 &Omega; termination</td><td>TM-CAL-014</td><td>Port 2 termination during S11</td></tr>
<tr><td>SMA male-male barrel</td><td>Low-loss thru</td><td>Port-to-port connection</td></tr>
<tr><td>FM antenna, short monopole</td><td>30 cm wire</td><td>FM broadcast reference, TM-CAL-004</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Power on TinySA Ultra. Allow 5-minute warmup before calibration measurements.</li>
<li>Connect to TinySA-App (Android or PC) via USB for extended functionality and logging.</li>
<li>Verify TCXO lock: in menu, check reference frequency compensation value (Hz offset from nominal).</li>
<li>Set RBW to 10 kHz, VBW to 10 kHz for standard calibration measurements.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<h3 class="section-heading">5-1. FREQUENCY CALIBRATION VIA FM BROADCAST</h3>
<div class="procedure"><ol>
<li>Attach short monopole antenna to HIGH input. Set span 87.5&ndash;108 MHz per TM-CAL-004.</li>
<li>Identify strong local FM station (e.g., 98.3 MHz). Measure marker frequency with TinySA.</li>
<li>Compare to published transmitter frequency (FCC database or RDS readout). Compute frequency error in Hz.</li>
<li>Apply correction: MENU &rarr; EXPERT &rarr; FREQ CAL. Enter correction in Hz (positive = TinySA reads low).</li>
<li>Verify: FM carrier should now read within &plusmn;5 kHz of published frequency.</li>
</ol></div>
<h3 class="section-heading">5-2. LEVEL CALIBRATION WITH ATTENUATOR PADS</h3>
<div class="procedure"><ol>
<li>Connect TinySA Ultra LO output port (output signal, typically &minus;10 dBm) to HIGH input via known 20 dB attenuator pad (TM-CAL-015).</li>
<li>Set TinySA to zero-span at LO frequency. Marker should read (&minus;10 &minus; 20) = &minus;30 dBm &plusmn;0.5 dB.</li>
<li>If offset &gt;1 dB, apply correction: MENU &rarr; LEVEL CORR. Enter correction offset in dB.</li>
<li>Verify by cascading 6 dB + 20 dB pads: reading should drop 6 dB to &minus;36 dBm &plusmn;0.5 dB.</li>
</ol></div>
<h3 class="section-heading">5-3. SWEEP LINEARITY CHECK</h3>
<div class="procedure"><ol>
<li>Set TinySA to SCAN mode, 1&ndash;30 MHz. Apply comb signal (1 MHz step markers from frequency counter). Verify all comb lines appear at correct frequency positions &plusmn;0.01%.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>TCXO frequency correction is stored in non-volatile memory. The correction value is in Hz at the TCXO reference frequency. For a 30 MHz TCXO with +1 ppm error, correction = +30 Hz. After correction, the TinySA frequency axis is accurate to &lt;100 Hz at all frequencies (limited by FM broadcast reference uncertainty of &lt;5 kHz).</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>TinySA Ultra Acceptance Specifications</caption>
<thead><tr><th>Parameter</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Frequency range (LOW input)</td><td>100 kHz &ndash; 800 MHz</td></tr>
<tr><td>Frequency range (HIGH input)</td><td>100 MHz &ndash; 6 GHz</td></tr>
<tr><td>Frequency accuracy (post-cal)</td><td>&lt;5 kHz (FM broadcast-limited)</td></tr>
<tr><td>Level accuracy</td><td>&plusmn;1 dB (after attenuator cal)</td></tr>
<tr><td>Noise floor (RBW 10 kHz)</td><td>&lt;&minus;120 dBm</td></tr>
<tr><td>Dynamic range</td><td>&gt;90 dB</td></tr>
<tr><td>Max input (no damage)</td><td>+20 dBm (1 s max), +10 dBm continuous</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Noise floor</span>N<sub>floor</sub> = &minus;174 + NF + 10 log<sub>10</sub>(RBW) dBm</div>
<div class="formula-block"><span class="formula-label">Sweep time (superheterodyne)</span>T<sub>sweep</sub> = N &times; RBW<sup>&minus;1</sup>  [N = number of points]</div>
<div class="formula-block"><span class="formula-label">Dynamic range limit</span>DR = P<sub>1dB</sub> &minus; N<sub>floor</sub>  [dB]</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>TinySA Ultra Calibration Log</caption>
<thead><tr><th>Test</th><th>Reference</th><th>Measured</th><th>Error</th><th>Pass/Fail</th></tr></thead>
<tbody>
<tr><td>FM freq (98.3 MHz)</td><td>98,300,000 Hz</td><td>98,301,200 Hz</td><td>+1.2 kHz</td><td>Pass</td></tr>
<tr><td>Level (LO thru 20 dB pad)</td><td>&minus;30.0 dBm</td><td>&minus;30.4 dBm</td><td>&minus;0.4 dB</td><td>Pass</td></tr>
<tr><td>Noise floor (RBW 10 kHz)</td><td>&lt;&minus;120 dBm</td><td>&minus;122 dBm</td><td>&mdash;</td><td>Pass</td></tr>
</tbody></table>
""",
},

"voltage-reference": {
    "tm": "TM-TGEAR-029",
    "title": "PRECISION VOLTAGE REFERENCE",
    "subtitle": "Traceable DC Voltage Standards, 1.225 V to 10.000 V",
    "body": """
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers design, construction, and calibration of precision DC voltage references from 1.225 V to 10.000 V for calibrating DMMs, oscilloscopes, and ADCs. References are based on band-gap or buried-zener IC standards traceable to NIST via publicly available calibration data from the IC manufacturers.</p>
<h3 class="section-heading">1-2. APPLICABLE DOCUMENTS</h3>
<p>TM-CAL-006 (Precision Voltage Reference), TM-TGEAR-017 (Fluke 15B+ DMM).</p>
<div class="tm-note"><span class="admonition-label">NOTE</span>Band-gap references (LM4040, TL431, REF02) have initial tolerance of &plusmn;0.1% to &plusmn;2%. For calibration use, measure actual output voltage with a precision instrument and use measured value. Do NOT rely on nominal value as a calibration standard without individual measurement.</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. BAND-GAP REFERENCE</h3>
<p>The band-gap voltage of silicon (approximately 1.205 V at 0 K, extrapolated) is exploited by designing a circuit whose output voltage is proportional to V<sub>BE</sub> differences between two transistors operating at different current densities. The resulting band-gap voltage V<sub>BG</sub> &asymp; 1.225 V has a temperature coefficient close to zero near room temperature.</p>
<div class="formula-block"><span class="formula-label">Band-gap reference voltage</span>V<sub>ref</sub> &asymp; V<sub>BG</sub> + (&Delta;V<sub>BE</sub> &times; scale factor) = 1.225 V (first-order)</div>
<h3 class="section-heading">2-2. BURIED ZENER REFERENCE</h3>
<p>Buried-zener references (LM399, LTZ1000) use a subsurface zener diode structure with 1/f noise 100&times; lower than surface-breakdown zeners. They achieve stability of &lt;1 ppm/month and TC &lt;0.05 ppm/&deg;C (LTZ1000 with oven). These are the highest-accuracy single-chip references available commercially.</p>
<div class="formula-block"><span class="formula-label">Temperature coefficient</span>TC = (dV<sub>ref</sub>/dT) / V<sub>ref</sub>  [ppm/&deg;C]</div>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; CONSTRUCTION</h2>
<table class="tm-table"><caption>Voltage Reference Designs by Accuracy Class</caption>
<thead><tr><th>Accuracy</th><th>Reference IC</th><th>V<sub>out</sub></th><th>TC (ppm/&deg;C)</th><th>Stability</th></tr></thead>
<tbody>
<tr><td>Class C (0.1%)</td><td>LM4040C-4.096</td><td>4.096 V</td><td>&plusmn;100</td><td>&plusmn;500 ppm/yr</td></tr>
<tr><td>Class B (0.05%)</td><td>REF02E</td><td>5.000 V</td><td>&plusmn;25</td><td>&plusmn;100 ppm/yr</td></tr>
<tr><td>Class A (0.02%)</td><td>LT1027BCN8</td><td>5.000 V</td><td>&plusmn;5</td><td>&plusmn;20 ppm/yr</td></tr>
<tr><td>Ultra (0.001%)</td><td>LTZ1000 + oven</td><td>7.000 V</td><td>&plusmn;0.05</td><td>&plusmn;2 ppm/yr</td></tr>
</tbody></table>
<h3 class="section-heading">3-2. CIRCUIT FOR LM4040 CLASS C REFERENCE</h3>
<table class="tm-table"><caption>LM4040 4.096 V Reference BOM</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>1</td><td>LM4040C-4.1</td><td>4.096 V shunt reference, &plusmn;0.1%, SOT-23</td></tr>
<tr><td>1</td><td>1 k&Omega; 0.1% resistor</td><td>Current-set from 9 V battery</td></tr>
<tr><td>1</td><td>9 V alkaline battery</td><td>Low-noise supply; avoid switching regulators</td></tr>
<tr><td>2</td><td>10 &mu;F tantalum caps</td><td>Supply filter and output noise filter</td></tr>
<tr><td>2</td><td>Banana jacks</td><td>4 mm output terminals</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Solder LM4040 in SOT-23 on adapter PCB. Current-set resistor = (V<sub>bat</sub> &minus; V<sub>ref</sub>) / I<sub>set</sub>. For 9 V bat and 1 mA: R = (9 &minus; 4.1) / 0.001 = 4.9 k&Omega;. Use 4.7 k&Omega; + 470 &Omega; in series for easy adjustment.</li>
<li>Allow 30-minute warmup with battery connected before measuring output. Thermal equilibrium reduces V<sub>ref</sub> variation.</li>
<li>Measure output with precision DMM. Record: V<sub>ref_measured</sub> = _____. This is the working calibration value.</li>
<li>Label reference module with measured V<sub>ref</sub>, date, and temperature during measurement.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<h3 class="section-heading">5-1. ABSOLUTE CALIBRATION</h3>
<div class="procedure"><ol>
<li>Measure V<sub>ref</sub> with the most accurate available DMM (Fluke 15B+ or better).</li>
<li>Record measured value, ambient temperature, battery voltage, and date.</li>
<li>Traceability note: LM4040 manufacturer-characterized to NIST standards via wafer-level test. Initial tolerance &plusmn;0.1%; actual measured value is the calibration standard for this reference module.</li>
</ol></div>
<h3 class="section-heading">5-2. TEMPERATURE COEFFICIENT MEASUREMENT</h3>
<div class="procedure"><ol>
<li>Measure V<sub>ref</sub> at 25 &deg;C (room temperature) and 45 &deg;C (oven or heat gun from 30 cm).</li>
<li>TC = (V<sub>45</sub> &minus; V<sub>25</sub>) / (V<sub>25</sub> &times; 20 &deg;C) &times; 10<sup>6</sup> ppm/&deg;C. Accept &lt;&plusmn;200 ppm/&deg;C for Class C.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>For shunt reference designs (LM4040), there is no adjustable output. Use an external amplifier (non-inverting op-amp) to scale V<sub>ref</sub> to other voltages. For instance, 4.096 V &times; (10 k&Omega; + 14.4 k&Omega;) / 10 k&Omega; = 10.000 V.</p>
<div class="formula-block"><span class="formula-label">Scaling amplifier gain</span>G = 1 + R<sub>f</sub>/R<sub>g</sub>;  V<sub>out</sub> = G &times; V<sub>ref</sub></div>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table"><caption>Voltage Reference Acceptance Specifications</caption>
<thead><tr><th>Class</th><th>Initial Accuracy</th><th>TC</th><th>1-Year Stability</th></tr></thead>
<tbody>
<tr><td>C (LM4040)</td><td>&plusmn;0.1%</td><td>&plusmn;100 ppm/&deg;C</td><td>&plusmn;500 ppm</td></tr>
<tr><td>B (REF02)</td><td>&plusmn;0.05%</td><td>&plusmn;25 ppm/&deg;C</td><td>&plusmn;100 ppm</td></tr>
<tr><td>A (LT1027)</td><td>&plusmn;0.02%</td><td>&plusmn;5 ppm/&deg;C</td><td>&plusmn;20 ppm</td></tr>
</tbody></table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Current-set resistor</span>R = (V<sub>supply</sub> &minus; V<sub>ref</sub>) / I<sub>ref</sub>  [I<sub>ref</sub> typical 1&ndash;5 mA]</div>
<div class="formula-block"><span class="formula-label">Temperature coefficient</span>TC = &Delta;V / (V<sub>ref</sub> &times; &Delta;T)  [ppm/&deg;C]</div>
<div class="formula-block"><span class="formula-label">Scaling amplifier output</span>V<sub>out</sub> = V<sub>ref</sub> &times; (1 + R<sub>f</sub>/R<sub>g</sub>)</div>
<div class="formula-block"><span class="formula-label">Long-term stability (ppm/yr)</span>S = |V(t+1yr) &minus; V(t)| / V(t) &times; 10<sup>6</sup></div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table"><caption>Voltage Reference Calibration Log</caption>
<thead><tr><th>Reference</th><th>Nominal</th><th>Measured</th><th>Error</th><th>TC Measured</th></tr></thead>
<tbody>
<tr><td>LM4040C-4.096 #1</td><td>4.096 V</td><td>4.0944 V</td><td>&minus;0.039%</td><td>+88 ppm/&deg;C</td></tr>
<tr><td>LM4040C-4.096 #2</td><td>4.096 V</td><td>4.0981 V</td><td>+0.051%</td><td>+92 ppm/&deg;C</td></tr>
<tr><td>REF02E</td><td>5.000 V</td><td>4.9988 V</td><td>&minus;0.024%</td><td>+18 ppm/&deg;C</td></tr>
</tbody></table>
""",
},

}  # end INSTRUMENTS dict

# ============================================================
# HTML GENERATION AND INDEX UPDATE
# ============================================================

def write_tm(dirname, info):
    """Write the tm.html file for one instrument directory."""
    dir_path = os.path.join(BASE_DIR, dirname)
    if not os.path.isdir(dir_path):
        print(f"  ERROR: directory not found: {dir_path}")
        return False

    html = make_tm(info["tm"], info["title"], info["subtitle"], info["body"])
    out_path = os.path.join(dir_path, "tm.html")
    with open(out_path, "w", encoding="utf-8") as f:
        f.write(html)
    return True


def update_index(dirname, info):
    """Insert a 'View Technical Manual' link at the top of <main> in index.html."""
    index_path = os.path.join(BASE_DIR, dirname, "index.html")
    if not os.path.isfile(index_path):
        print(f"  WARNING: index.html not found in {dirname}")
        return False

    with open(index_path, "r", encoding="utf-8") as f:
        content = f.read()

    # Don't double-insert
    if "tm.html" in content:
        return True

    tm_link = (
        f'\n<div style="margin-bottom:12px;">'
        f'<a href="tm.html" style="font-weight:bold;color:#2c5f8d;">'
        f'&#128209; View Technical Manual: {info["tm"]}</a></div>\n'
    )

    # Insert after <main> opening tag
    if "<main>" in content:
        content = content.replace("<main>", "<main>" + tm_link, 1)
    elif "<main " in content:
        # Find next > after <main
        idx = content.find("<main ")
        end = content.find(">", idx)
        content = content[:end+1] + tm_link + content[end+1:]
    else:
        print(f"  WARNING: no <main> tag found in {dirname}/index.html")
        return False

    with open(index_path, "w", encoding="utf-8") as f:
        f.write(content)
    return True


def main():
    # Alphabetical order
    dirs_sorted = sorted(INSTRUMENTS.keys())

    total_written = 0
    total_index_updated = 0
    errors = []

    for dirname in dirs_sorted:
        info = INSTRUMENTS[dirname]
        print(f"  Writing {info['tm']} -> {dirname}/tm.html ...", end=" ")
        ok = write_tm(dirname, info)
        if ok:
            total_written += 1
            print("OK")
        else:
            errors.append(dirname)
            print("FAILED")

        print(f"  Updating {dirname}/index.html ...", end=" ")
        ok2 = update_index(dirname, info)
        if ok2:
            total_index_updated += 1
            print("OK")
        else:
            print("SKIPPED/FAILED")

    print(f"\n{'='*60}")
    print(f"TMs written:         {total_written} / {len(INSTRUMENTS)}")
    print(f"Index files updated: {total_index_updated} / {len(INSTRUMENTS)}")
    if errors:
        print(f"Errors in:           {', '.join(errors)}")
    else:
        print("No errors.")
    print("="*60)


if __name__ == "__main__":
    main()
