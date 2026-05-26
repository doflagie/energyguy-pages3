#!/usr/bin/env python3
"""
generate_tms.py  —  Write tm.html for all 19 antenna_gear directories.
Run from any directory:  python3 generate_tms.py
Output: antenna_gear/<subdir>/tm.html  (19 files)
"""

from pathlib import Path

BASE = Path(__file__).parent   # antenna_gear/
DATE = "26 May 2026"
AUTHOR = "Mervyn Martin, KO6NNH"

# ─── HTML template ────────────────────────────────────────────────────────────

def html(tm_num, title, subtitle, body):
    nav = """        <nav>
            <h3>Main Navigation</h3>
            <ul>
                <li><a href="/">Home</a></li>
                <li><a href="/about/bio.html">About</a></li>
            </ul>
            <details class="nav-group" open>
                <summary>Antenna Gear</summary>
                <ul>
                    <li><a href="/antenna_gear/index.html">Antenna Gear Overview</a></li>
                    <li><a href="/antenna_gear/ant_switches/tm.html">Antenna Switches</a></li>
                    <li><a href="/antenna_gear/audio_filters/tm.html">Audio Filters</a></li>
                    <li><a href="/antenna_gear/baluns-and-ununs/tm.html">Baluns &amp; Un-Uns</a></li>
                    <li><a href="/antenna_gear/bms/tm.html">Battery Management (BMS)</a></li>
                    <li><a href="/antenna_gear/coax_traps/tm.html">Coax Traps</a></li>
                    <li><a href="/antenna_gear/coil_winder/tm.html">Coil Winder</a></li>
                    <li><a href="/antenna_gear/common_mode_chokes/tm.html">Common-Mode Chokes</a></li>
                    <li><a href="/antenna_gear/crystal_radio/tm.html">Crystal Radio</a></li>
                    <li><a href="/antenna_gear/hybrids/tm.html">RF Hybrids</a></li>
                    <li><a href="/antenna_gear/lightning_protection/tm.html">Lightning Protection</a></li>
                    <li><a href="/antenna_gear/mast_tower/tm.html">Mast &amp; Tower</a></li>
                    <li><a href="/antenna_gear/motorized_tuner/tm.html">Motorized Tuner</a></li>
                    <li><a href="/antenna_gear/power_supplies/tm.html">Power Supplies</a></li>
                    <li><a href="/antenna_gear/remote-element-switching/tm.html">Remote Element Switching</a></li>
                    <li><a href="/antenna_gear/rf_couplers/tm.html">RF Couplers</a></li>
                    <li><a href="/antenna_gear/rfi_mitigation/tm.html">RFI Mitigation</a></li>
                    <li><a href="/antenna_gear/roller_inductors/tm.html">Roller Inductors</a></li>
                    <li><a href="/antenna_gear/transmatch/tm.html">Transmatch</a></li>
                    <li><a href="/antenna_gear/variable_caps/tm.html">Variable Capacitors</a></li>
                </ul>
            </details>
            <details class="nav-group">
                <summary>Antenna Tools</summary>
                <ul>
                    <li><a href="/antenna_tools/index.html">Antenna Tools Overview</a></li>
                    <li><a href="/antenna_tools/antenna-analyzer/tm.html">Antenna Analyzer</a></li>
                    <li><a href="/antenna_tools/swr_meter/tm.html">SWR Meter</a></li>
                    <li><a href="/antenna_tools/rf_power_meter/tm.html">RF Power Meter</a></li>
                </ul>
            </details>
            <details class="nav-group">
                <summary>Calibration</summary>
                <ul>
                    <li><a href="/calibration/index.html">Calibration Library</a></li>
                    <li><a href="/Calibration_Ideas/index.html">Calibration Standards</a></li>
                </ul>
            </details>
        </nav>"""

    return f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{tm_num} {title} - Merv's Brain Dump</title>
    <link rel="stylesheet" href="../../css/style.css">
    <style>
.tm-cover{{border:3px solid #2c5f8d;padding:28px 36px;text-align:center;margin-bottom:28px;background:#f8f8f4}}
.tm-classification{{font-size:1.05em;font-weight:bold;color:#006600;letter-spacing:3px;margin-bottom:16px}}
.tm-number{{font-family:'Courier New',monospace;font-size:1.15em;font-weight:bold;color:#2c5f8d;margin-bottom:8px}}
.tm-title{{font-size:1.7em;font-weight:bold;color:#1a3a5c;text-transform:uppercase;letter-spacing:1px;margin:12px 0 8px;line-height:1.3}}
.tm-subtitle{{font-size:.98em;font-style:italic;color:#555;margin-bottom:12px}}
.tm-meta{{font-size:.88em;color:#555;margin-top:16px;border-top:1px solid #aaa;padding-top:12px}}
.chapter-body h1,.chapter-body h2{{color:#1a3a5c;border-bottom:2px solid #4a7ba7;padding-bottom:4px;margin-top:22px;margin-bottom:10px}}
.chapter-body h3{{color:#2c5f8d;margin-top:16px;margin-bottom:8px}}
.chapter-body table{{width:100%;border-collapse:collapse;margin:12px 0;font-size:.93em}}
.chapter-body th{{background:#4a7ba7;color:#fff;padding:7px 11px;text-align:left;font-size:.88em;text-transform:uppercase}}
.chapter-body td{{border:1px solid #c0ccd8;padding:6px 11px;vertical-align:top}}
.chapter-body tr:nth-child(even) td{{background:#f4f7fb}}
.chapter-body ol,.chapter-body ul{{margin:8px 0 8px 20px}}
.chapter-body li{{margin:5px 0;line-height:1.5}}
.chapter-body p{{margin:8px 0 10px}}
.chapter-body pre,.chapter-body code{{background:#f4f4ee;border:1px solid #ddd;padding:2px 5px;font-size:.9em}}
.chapter-body pre{{padding:10px 14px;overflow-x:auto;line-height:1.4}}
.back-link{{margin-top:28px;padding-top:12px;border-top:1px solid #c0ccd8;font-size:.9em}}
    </style>
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
{nav}
            <main>
                <div class="tm-cover">
                    <div class="tm-classification">UNCLASSIFIED</div>
                    <div class="tm-number">{tm_num}</div>
                    <div class="tm-title">{title}</div>
                    <div class="tm-subtitle">{subtitle}</div>
                    <div class="tm-meta">
                        Prepared by: {AUTHOR} &nbsp;&bull;&nbsp;
                        Merced, California &nbsp;&bull;&nbsp; {DATE}<br>
                        <em>Amateur Radio / Electronics &mdash; Not for commercial use</em>
                    </div>
                </div>
                <div class="chapter-body">
{body}
                </div>
                <div class="back-link">
                    <a href="../index.html">&larr; Antenna Gear</a>
                </div>
            </main>
        </div>
    </div>
    <footer>
        <div class="address-block">
            <h3>Merv's Brain Dump</h3>
            <p>Mervyn Martin (Merv), Proprietor</p>
            <p>Amateur Callsign: KO6NNH</p>
            <p>Merced, California 95340</p>
            <div class="contact-info">
                <div class="contact-item"><strong>Email:</strong><br>
                    <a href="mailto:d.ecaudata@gmail.com">d.ecaudata@gmail.com</a></div>
                <div class="contact-item"><strong>Website:</strong><br>
                    <a href="http://www.doflagie.com">www.doflagie.com</a></div>
                <div class="contact-item"><strong>GitHub:</strong><br>
                    <a href="https://github.com/doflagie" target="_blank">github.com/doflagie</a></div>
            </div>
            <div class="copyright">
                <p>&copy; 2026 Merv's Brain Dump. All rights reserved.</p>
                <p>Designs shared under <a href="https://creativecommons.org/licenses/by-sa/4.0/" target="_blank">CC BY-SA 4.0</a>.</p>
            </div>
        </div>
    </footer>
</body>
</html>"""


# ─── TM content ───────────────────────────────────────────────────────────────

TOMS = {}

# ── TM-GEAR-001  Antenna Switches ─────────────────────────────────────────────
TOMS['ant_switches'] = dict(
    tm_num="TM-GEAR-001",
    title="ANTENNA SWITCHES — MANUAL, MOTORIZED, AND AUTO-BAND",
    subtitle="High-Power SO-239/N-Type, Relay-Based, and ESP32-Controlled Designs",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers four antenna switch designs: a manual high-power rotary
switch (3&nbsp;kW PEP), a manual compact rotary (200W), an auto-band relay
switch (band-following with transceiver CI-V), and a motorized remote switch
(ESP32-controlled relays, all 15 bands).  All designs use SO-239 or N-type
connectors and 50&Omega; coaxial paths.</p>
<p><strong>Frequency coverage:</strong> 1.8&nbsp;MHz through 1.3&nbsp;GHz
depending on connector type and relay specifications.</p>
<p><strong>Power ratings:</strong> 100W CW to 3&nbsp;kW PEP depending on design.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 RF Switching Requirements</h3>
<p>An antenna switch routes a single transmitter/receiver to one of N antennas
without significant signal loss or impedance mismatch.  Key parameters:</p>
<ul>
<li><strong>Insertion loss:</strong> Signal attenuation through the selected
    path.  Target &lt;0.1&nbsp;dB at HF; &lt;0.3&nbsp;dB at VHF.</li>
<li><strong>Isolation:</strong> Suppression of signal on non-selected ports.
    Target &ge;50&nbsp;dB at HF to prevent signal bleed to unselected antennas.</li>
<li><strong>SWR:</strong> The switch must not introduce significant impedance
    mismatch.  Target SWR &lt;1.1:1 through the operating band.</li>
<li><strong>Power handling:</strong> Contacts and relay coils must survive
    the peak envelope power (PEP) plus any reflected power.</li>
</ul>
<h3>2-2 Relay Types</h3>
<p>Mechanical relays (Omron G2RL, Panasonic TQ2) provide low insertion loss
but are limited to &sim;10&nbsp;MHz switching rate and wear over millions of
cycles.  They are appropriate for all antenna switching applications.
PIN diode switches (MA4P504, HSMP-3814) switch in nanoseconds and have
no wear, but require bias current and have higher insertion loss at HF.</p>
<h3>2-3 Auto-Band Switching</h3>
<p>The auto-band switch monitors the transceiver's CI-V (Icom) or CAT
(Yaesu/Kenwood) bus to determine the operating frequency.  A lookup table
maps frequency ranges to antenna ports.  On a band change, the ESP32 toggles
the correct relay within 50&nbsp;ms — before the first transmitted audio
reaches the antenna.</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>Manual HV</th><th>Relay Auto</th><th>Motorized Remote</th></tr>
<tr><td>Switch element</td><td>Ceramic rotary, 3&nbsp;kW</td><td>Omron G2RL-1 relays</td><td>Omron G2RL-1 relays</td></tr>
<tr><td>Connectors</td><td>SO-239 (×5) or N</td><td>SO-239 (×9)</td><td>N-type (×9)</td></tr>
<tr><td>Controller</td><td>—</td><td>ESP32 + CI-V decoder</td><td>ESP32 + RS-485</td></tr>
<tr><td>Power</td><td>None</td><td>12V, 100&nbsp;mA</td><td>12V, 300&nbsp;mA</td></tr>
<tr><td>Enclosure</td><td>Aluminum box, 150×100mm</td><td>Same</td><td>IP65 outdoor box</td></tr>
<tr><td>Coax stubs</td><td>Keep &lt;25&nbsp;mm</td><td>Keep &lt;25&nbsp;mm</td><td>Keep &lt;15&nbsp;mm</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<h3>4-1 High-Power Manual Switch</h3>
<ol>
<li>Mount SO-239 connectors on the enclosure with &lt;25&nbsp;mm of center conductor
    exposed inside.  Use Teflon-insulated wire between connector centers and
    rotary switch contacts.</li>
<li>Star-route all grounds to a single chassis point.  Use heavy braid or
    1/4&rdquo; copper strap for ground bonds to minimize ground-loop inductance.</li>
<li>Verify DC continuity: selected port to radio port must read &lt;0.2&Omega;.
    All other ports must read open (&gt;1&nbsp;M&Omega;) to the radio port.</li>
</ol>
<h3>4-2 Relay Switch</h3>
<ol>
<li>Mount relays in a grid pattern with &lt;15&nbsp;mm of coax between each relay
    and its associated SO-239.  Longer stubs degrade VHF performance.</li>
<li>Bypass each relay coil with a 1N4007 flyback diode (cathode to +12V)
    to suppress the inductive spike when the relay de-energizes.</li>
<li>Mount ESP32 on a separate PCB with 10&nbsp;nF bypass caps on each GPIO pin
    driving a relay coil.  Relay switching transients can reset an unprotected
    microcontroller.</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<ol>
<li>Manual switch: rotate selector to desired antenna port before transmitting.
    Never rotate the switch while transmitting — arcing at the contacts can
    weld them.</li>
<li>Auto-band switch: confirm CI-V address matches the transceiver's configured
    address (default 0x56 for IC-7300, 0x70 for IC-705).  Monitor the CYD
    display to verify the switch follows band changes.</li>
<li>Remote switch: send band command via RS-485 or Wi-Fi API.  Verify LED
    indicator (or CYD display) confirms correct port selected before transmitting.</li>
<li>Never transmit into an open switch port (no antenna connected) — reflected
    power will be 100%, potentially damaging the transmitter PA.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Measure insertion loss with NanoVNA: calibrate SOLT at radio connector,
    then measure S21 to each antenna port in turn.  Loss must be &lt;0.1&nbsp;dB
    at 1.8&ndash;30&nbsp;MHz; &lt;0.3&nbsp;dB at 144&nbsp;MHz.</li>
<li>Measure isolation: with radio port and Port&nbsp;1 connected to NanoVNA,
    terminate Port&nbsp;2 through Port&nbsp;N in 50&Omega; loads.  S21 to each
    terminated port must be &lt;&minus;50&nbsp;dB at HF.</li>
<li>Verify auto-band sequencing: step through bands on the transceiver.
    Log which relay energizes at each band; confirm against the band map table
    in firmware.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Insertion loss &lt;0.1&nbsp;dB at 7&nbsp;MHz and 14&nbsp;MHz on all ports.</li>
<li>Isolation &ge;50&nbsp;dB at 14&nbsp;MHz between any two ports.</li>
<li>SWR at selected port &lt;1.05:1 at all test frequencies (1.8, 7, 14, 28, 50&nbsp;MHz)
    with a calibrated 50&Omega; load on the antenna port.</li>
<li>Auto-band: correct port selected within 100&nbsp;ms of band change on all
    15 configured bands.</li>
<li>Log: date, ports tested, insertion loss at key bands, isolation, SWR, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Band–Port Mapping Template</h2>
<table>
<tr><th>Band</th><th>Freq range (MHz)</th><th>Default port</th><th>Notes</th></tr>
<tr><td>160M</td><td>1.800–2.000</td><td>1</td><td>Inverted-L or T-antenna</td></tr>
<tr><td>80M</td><td>3.500–4.000</td><td>2</td><td>Dipole or vertical</td></tr>
<tr><td>40M</td><td>7.000–7.300</td><td>2</td><td>Shared with 80M</td></tr>
<tr><td>20M–10M</td><td>14.000–29.700</td><td>3</td><td>Yagi or beam</td></tr>
<tr><td>6M–2M</td><td>50–148</td><td>4</td><td>VHF verticals</td></tr>
<tr><td>70cm+</td><td>420–1300</td><td>5</td><td>UHF verticals</td></tr>
</table>

<h2 class="chapter-heading" id="appb">Appendix B — Relay Coil Power Budget</h2>
<p>Omron G2RL-1: 12V coil, 400&nbsp;mA inrush, 50&nbsp;mA hold.  For 8 relays
(all energized simultaneously in worst case): 8 &times; 50&nbsp;mA = 400&nbsp;mA
steady-state, 8 &times; 400&nbsp;mA = 3.2&nbsp;A inrush.  Power supply must
handle the inrush; use a 5&nbsp;A supply with a 1000&nbsp;&micro;F bulk
electrolytic on the 12V rail.</p>
""")

# ── TM-GEAR-002  Audio Filters ────────────────────────────────────────────────
TOMS['audio_filters'] = dict(
    tm_num="TM-GEAR-002",
    title="AUDIO FILTERS — CW, SSB, AND DSP",
    subtitle="LC Notch, Active CW BPF, LC SSB Bandpass, and DSP Audio Processing",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers four audio filter designs for improving receiver
selectivity and noise reduction: an LC notch filter (eliminating heterodyne
interference), an active CW bandpass filter (300&ndash;500&nbsp;Hz center,
&plusmn;50&nbsp;Hz bandwidth), an LC SSB bandpass filter (2.4&nbsp;kHz,
300&ndash;2700&nbsp;Hz passband), and a DSP audio processor (ESP32-based
FIR/IIR filtering with 16-bit 44.1&nbsp;kHz audio codec).</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 LC Notch Filter</h3>
<p>A series LC circuit resonant at the interference frequency presents near-zero
impedance in parallel with the audio path, pulling the interference signal to
ground.  At resonance f<sub>0</sub> = 1/(2&pi;&radic;LC), the notch depth depends
on component Q.  Practical notch depth: 30&ndash;50&nbsp;dB.  A variable
capacitor makes the notch tunable across 200&ndash;5000&nbsp;Hz.</p>
<h3>2-2 Active CW Filter</h3>
<p>A cascade of two Sallen-Key bandpass stages using LM833 (low-noise, 15&nbsp;MHz
GBW) op-amps provides approximately 36&nbsp;dB/octave roll-off outside the
passband.  The center frequency Q is set by resistor ratios; Q = 10 for CW
(narrow, 50&nbsp;Hz &minus;3&nbsp;dB BW at 500&nbsp;Hz center).  This narrow
bandwidth dramatically improves CW copy in high-noise conditions.</p>
<h3>2-3 DSP Audio Processor</h3>
<p>An ESP32-S3 with I2S audio codec (PCM5102A output, INMP441 microphone-grade
input) implements digital filters in software.  FIR filters achieve linear phase
(no transient distortion); IIR filters achieve steep rolloff with fewer taps.
Available modes: CW narrow (500&nbsp;Hz), SSB bandpass, noise reduction
(spectral subtraction), and audio peak filtering (sharp peak at detected
CW frequency).</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>LC Notch</th><th>Active CW</th><th>DSP</th></tr>
<tr><td>Inductors</td><td>1&nbsp;mH pot-core</td><td>—</td><td>—</td></tr>
<tr><td>Capacitors</td><td>VC 10&ndash;100&nbsp;pF + fixed NP0</td><td>1% polypropylene</td><td>—</td></tr>
<tr><td>Op-amps</td><td>—</td><td>LM833, 2&times;</td><td>—</td></tr>
<tr><td>DSP processor</td><td>—</td><td>—</td><td>ESP32-S3 WROOM</td></tr>
<tr><td>Audio codec</td><td>—</td><td>—</td><td>PCM5102A + INMP441</td></tr>
<tr><td>Power</td><td>&plusmn;12V or 9V battery</td><td>&plusmn;12V</td><td>5V USB-C</td></tr>
<tr><td>Audio connectors</td><td>3.5&nbsp;mm stereo (×2)</td><td>Same</td><td>Same</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<h3>4-1 LC Notch Filter</h3>
<ol>
<li>Wind 1&nbsp;mH inductor on a ferrite pot-core for the highest achievable Q
    (target Q &ge;50).  Measure inductance with LCR meter before mounting.</li>
<li>Connect a variable capacitor (10&ndash;100&nbsp;pF air-variable) in series
    with the inductor.  This LC series combination connects in shunt (in parallel)
    with the audio signal path between the receiver audio output and the headphone
    or speaker.</li>
<li>Tune by listening: with an interfering tone present, adjust the variable cap
    until the tone disappears.  Notch depth should be audibly dramatic
    (&gt;30&nbsp;dB).</li>
</ol>
<h3>4-2 Active CW Filter</h3>
<ol>
<li>Build two Sallen-Key stages.  Stage 1: f<sub>0</sub> = 500&nbsp;Hz, Q = 5.
    Stage 2: f<sub>0</sub> = 500&nbsp;Hz, Q = 10.  Series connection gives
    combined Q &sim;50 (approximate).</li>
<li>Power: use a &plusmn;12V regulated supply.  Use 100&nbsp;nF NP0 + 10&nbsp;&micro;F
    electrolytic bypass on each supply pin within 5&nbsp;mm of the op-amp.</li>
<li>Set gain &lt;6&nbsp;dB to avoid clipping.  Check output at maximum received
    signal level with an oscilloscope — no flat-topping.</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<ol>
<li>LC Notch: insert inline in the headphone lead.  Tune the variable cap while
    a heterodyne tone is present until the tone nulls.  Leave set; retune if
    the interferer changes frequency.</li>
<li>Active CW Filter: insert inline.  Turn on with the band switch.  The filter
    is narrowest in CW mode; switch to bypass or SSB mode if copying SSB.
    The filter does not switch fast enough for RTTY (use DSP mode).</li>
<li>DSP Filter: select mode on the CYD touchscreen.  CW NARROW enables a
    500&nbsp;Hz FIR bandpass.  NOISE REDUCE applies spectral subtraction;
    optimal for weak SSB signals with broadband noise.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Inject a 1&nbsp;kHz &minus;30&nbsp;dBV audio tone into the input.  Measure
    output level with a true-RMS voltmeter.  Passband insertion loss must be
    &lt;3&nbsp;dB for in-band signals.</li>
<li>Inject tones at 100, 200, 300, 3000, 4000&nbsp;Hz.  Verify attenuation
    &ge;30&nbsp;dB (LC notch) or &ge;40&nbsp;dB (active) for out-of-band signals.</li>
<li>DSP filter: use a sweep generator (PC software or TinySA audio output) to
    sweep 100&ndash;5000&nbsp;Hz.  Verify &minus;3&nbsp;dB points match the
    selected filter specification within 10%.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>CW filter: passband 450&ndash;550&nbsp;Hz, loss &lt;3&nbsp;dB; stopband
    outside 300&ndash;700&nbsp;Hz, attenuation &ge;30&nbsp;dB.</li>
<li>SSB filter: passband 300&ndash;2700&nbsp;Hz, loss &lt;3&nbsp;dB; &gt;50&nbsp;Hz
    and &gt;3000&nbsp;Hz at least 30&nbsp;dB down.</li>
<li>LC notch: tunable to any frequency in 200&ndash;5000&nbsp;Hz range; notch
    depth &ge;30&nbsp;dB at any tuned frequency.</li>
<li>No audible distortion on a steady 1&nbsp;kHz tone at maximum receiver audio
    output (no clipping).</li>
<li>Log: date, filter type, passband &minus;3&nbsp;dB points, stopband attenuation
    at key test frequencies, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Filter Design Formulas</h2>
<pre>LC series resonance (notch frequency):
  f0 = 1 / (2&pi; &times; sqrt(L &times; C))

Sallen-Key BPF center frequency:
  f0 = 1 / (2&pi; &times; R &times; C)    [equal-component design]

Q (quality factor):
  Q = f0 / BW_3dB

BW at -3 dB:
  BW = f0 / Q</pre>

<h2 class="chapter-heading" id="appb">Appendix B — Worked Example</h2>
<p>CW filter, f<sub>0</sub> = 500&nbsp;Hz, Q = 10:</p>
<pre>BW = 500 / 10 = 50 Hz  (-3 dB bandwidth)
-3 dB points: 475 Hz and 525 Hz
Skirt at 300 Hz: well into stopband (200 Hz away from center)
Skirt attenuation (2-pole): ~12 dB/octave; at 300 Hz (~0.8 octave below)
  attenuation ≈ 2 × 12 × log2(500/300) = ~16 dB per stage × 2 stages = ~32 dB total</pre>
""")

# ── TM-GEAR-003  Baluns and Un-Uns ────────────────────────────────────────────
TOMS['baluns-and-ununs'] = dict(
    tm_num="TM-GEAR-003",
    title="BALUNS AND UN-UNS — CURRENT AND VOLTAGE TYPES",
    subtitle="1:1 Current Balun, 4:1 Guanella, 6:1 and 9:1 Trifilar, VHF/UHF Air-Core",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers five balun and un-un designs for antenna feedpoint
impedance transformation and common-mode current suppression:
1:1 current balun (choke balun), 4:1 Guanella current balun,
6:1 trifilar transmission-line transformer, 9:1 trifilar (for 450&Omega;
end-fed wires), and 1:1 air-core VHF/UHF choke balun.</p>
<p>A <strong>balun</strong> (balanced-to-unbalanced) interfaces a coaxial line
(unbalanced) to a balanced antenna (dipole, loop).  An <strong>un-un</strong>
interfaces two unbalanced impedances (e.g., 50&Omega; coax to 450&Omega;
random wire).  Both suppress common-mode currents on the feedline.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Current vs. Voltage Balun</h3>
<p>A <strong>current balun</strong> (choke balun) enforces equal and opposite
currents in the two antenna elements by presenting high impedance (Z_choke
&ge;2000&Omega;) to common-mode current on the coax shield.  The
differential mode (transmit signal) sees only the low impedance of the coax.
A current balun does NOT transform impedance; it only suppresses common-mode.</p>
<p>A <strong>voltage balun</strong> enforces equal and opposite voltages.
It is simpler to build but allows common-mode current if the antenna is
not symmetrical.  Current baluns are preferred for dipoles and loops;
voltage baluns may be used when impedance transformation is also needed.</p>
<h3>2-2 Transmission-Line Transformer (TLT)</h3>
<p>TLTs (Guanella, trifilar) use transmission-line principles rather than
magnetic coupling alone.  The characteristic impedance of the transmission line
on the core determines the transformation ratio.  For a 4:1 Guanella: two
equal-impedance lines are connected in series at the input and parallel at
the output, giving a 4:1 impedance ratio.</p>
<p>Impedance ratio relationships:</p>
<pre>1:1 choke:   Z_in = Z_out = 50&Omega; (no transformation; common-mode choke only)
4:1 Guanella: Z_in = 200&Omega; balanced &rarr; 50&Omega; unbalanced
9:1 trifilar: Z_in = 450&Omega; unbalanced &rarr; 50&Omega; unbalanced</pre>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Design</th><th>Core</th><th>Wire</th><th>Turns</th><th>Freq range</th></tr>
<tr><td>1:1 current (choke)</td><td>FT-240-31</td><td>RG-303 coax, 12 passes</td><td>12</td><td>1.8&ndash;30 MHz</td></tr>
<tr><td>4:1 Guanella</td><td>2&times; FT-140-43</td><td>#16 bifilar, 8 turns</td><td>8</td><td>1.8&ndash;30 MHz</td></tr>
<tr><td>6:1 trifilar</td><td>FT-140-43</td><td>#18 trifilar, 10 turns</td><td>10</td><td>3.5&ndash;30 MHz</td></tr>
<tr><td>9:1 trifilar</td><td>FT-240-43</td><td>#16 trifilar, 9 turns</td><td>9</td><td>1.8&ndash;30 MHz</td></tr>
<tr><td>1:1 VHF/UHF air-core</td><td>None (air core)</td><td>RG-58 coax, 6 turns</td><td>6</td><td>50&ndash;450 MHz</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<h3>4-1 1:1 Choke Balun (FT-240-31)</h3>
<ol>
<li>Thread RG-303 (or RG-58) coaxial cable through the toroid 12 times.  Keep
    turns close-wound; secure with cable ties after every 3 turns.</li>
<li>Connect the input end to an SO-239 or N-type connector (coax center to
    center pin, shield to shell).  Same for the output end.</li>
<li>Measure choking impedance with NanoVNA: connect one port to the coax
    center, other port to the coax shield, and sweep 1.8&ndash;30&nbsp;MHz.
    The |Z| should exceed 2000&nbsp;&Omega; across most of the HF band.
    Core type #31 is optimized for 1.8&ndash;30&nbsp;MHz.</li>
</ol>
<h3>4-2 4:1 Guanella Balun</h3>
<ol>
<li>Wind two identical 8-turn bifilar windings on two separate FT-140-43 cores.
    Use color-coded wire to track polarity (dot convention).</li>
<li>Connect the two windings in series for the balanced (200&Omega;) port and
    in parallel for the unbalanced (50&Omega;) port.  Polarity is critical:
    the start of winding 1 connects to the finish of winding 2 at the balanced
    port center tap.</li>
<li>Test: connect a 200&Omega; resistor to the balanced port; measure 50&Omega;
    at the unbalanced port (SWR &lt;1.1:1).</li>
</ol>
<h3>4-3 9:1 Un-Un (End-Fed Wire)</h3>
<ol>
<li>Wind 9 turns of trifilar #16 AWG on an FT-240-43 core.  Three wires wound
    simultaneously, in the same direction.</li>
<li>Connect winding A in series with winding B; the junction is the 50&Omega;
    (unbalanced input) center tap.  Winding C is the 450&Omega; output
    winding.</li>
<li>Test: connect 450&Omega; resistor to high-impedance port; measure
    50&Omega; SWR at the coax port (&lt;1.2:1 across 1.8&ndash;30&nbsp;MHz).</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<ol>
<li>Mount the balun at the antenna feedpoint — not at the radio end.
    Feedpoint mounting minimizes the length of balanced feedline and
    the associated common-mode current problem.</li>
<li>Weatherproof outdoor installations: coat all solder joints and exposed
    windings with two coats of polyurethane or Plasti-Dip.  Wrap the
    core with self-amalgamating tape before mounting outdoors.</li>
<li>Check SWR after installation.  If SWR is higher than expected, the most
    common causes are: wrong impedance ratio for the antenna type, poor solder
    joint on the balun output terminals, or water ingress.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Measure impedance transformation ratio: connect a known resistive load
    (e.g., 200&Omega; for a 4:1 balun) to the balanced port; measure
    impedance at the unbalanced port with a NanoVNA.  Target: 50&Omega; &plusmn;10%.</li>
<li>Measure common-mode impedance (choke action): connect NanoVNA port 1
    to coax shield at input; port 2 to coax shield at output.
    Z_CM should exceed 2000&Omega; across 1.8&ndash;30&nbsp;MHz for type #31 core.</li>
<li>Measure insertion loss: S21 through the balun into a matched load.
    Target: &lt;0.3&nbsp;dB at all HF frequencies.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Impedance ratio correct to within &plusmn;10% at center frequency.</li>
<li>Common-mode impedance &ge;1000&Omega; at all specified frequencies.</li>
<li>Insertion loss &lt;0.3&nbsp;dB at 1.8&ndash;30&nbsp;MHz.</li>
<li>SWR &lt;1.2:1 at all test frequencies with correct load.</li>
<li>Power test: key a 100W transmitter into a dummy load through the balun
    for 30 seconds.  No smoke, no temperature rise exceeding 40&deg;C above ambient.</li>
<li>Log: date, design type, core material, turns, ratio test result, CM impedance, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Core Material Selection</h2>
<table>
<tr><th>Material</th><th>Best frequency range</th><th>Application</th></tr>
<tr><td>#31 ferrite</td><td>1&ndash;300 MHz</td><td>Choke baluns (1:1); excellent CMC for all HF</td></tr>
<tr><td>#43 ferrite</td><td>1&ndash;100 MHz</td><td>TLT baluns (4:1, 9:1); HF transformation</td></tr>
<tr><td>#61 ferrite</td><td>10&ndash;200 MHz</td><td>6M and VHF TLTs</td></tr>
<tr><td>#67 ferrite</td><td>50&ndash;500 MHz</td><td>VHF/UHF choke baluns and TLTs</td></tr>
<tr><td>Air core</td><td>50&ndash;1300 MHz</td><td>VHF/UHF choke (coax coil, no saturation risk)</td></tr>
</table>

<h2 class="chapter-heading" id="appb">Appendix B — Winding Polarity Verification</h2>
<p>Use a 1.5V battery and a voltmeter.  Connect battery across winding 1.
Measure voltage across winding 2.  If polarity is correct (dot-to-dot),
voltmeter reads positive.  If reversed, the windings are connected out of phase
and the balun will not function correctly.  Reverse one winding's connections
to correct.</p>
""")

# ── TM-GEAR-004  BMS ──────────────────────────────────────────────────────────
TOMS['bms'] = dict(
    tm_num="TM-GEAR-004",
    title="BATTERY MANAGEMENT SYSTEM (BMS) — LiFePO4 AND LI-ION",
    subtitle="BQ76920 4S LiFePO4, MPPT Integration, Cell Balancing, Field Power Pack",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers the design, construction, and maintenance of a 12.8V
nominal (4S LiFePO4) battery management system for field operation of
amateur radio stations.  The BMS uses a Texas Instruments BQ76920 IC for
cell voltage monitoring, overcharge/overdischarge/overcurrent protection,
and passive cell balancing.  A secondary discrete comparator circuit provides
backup protection.  Optional MPPT integration enables solar charging.</p>
<p><strong>Pack specifications:</strong> 12.8V nominal (10.0&ndash;14.4V operating
range), 50&nbsp;A maximum continuous discharge, 25&nbsp;A maximum charge.</p>
<p><strong>SAFETY — LITHIUM BATTERY HAZARD:</strong> LiFePO4 cells are safer
than other lithium chemistries but still present fire and explosion risk if
shorted, overcharged, or mechanically damaged.  Never short the pack terminals.
Never charge below 0&deg;C.  Never disassemble a swollen cell.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 LiFePO4 Cell Parameters</h3>
<p>LiFePO4 (lithium iron phosphate) cells have a flat discharge curve from
approximately 3.05V to 3.30V, covering 20%&ndash;80% SOC.  Key voltage limits:</p>
<table>
<tr><th>Condition</th><th>Single cell</th><th>4S pack</th></tr>
<tr><td>Full charge cutoff</td><td>3.60V</td><td>14.40V</td></tr>
<tr><td>Nominal voltage</td><td>3.20V</td><td>12.80V</td></tr>
<tr><td>Discharge cutoff</td><td>2.50V</td><td>10.00V</td></tr>
<tr><td>OV protection trip</td><td>3.65V</td><td>14.60V</td></tr>
<tr><td>UV protection trip</td><td>2.50V</td><td>10.00V</td></tr>
</table>
<h3>2-2 BQ76920 Protection IC</h3>
<p>The BQ76920 monitors each cell individually via its internal 14-bit ADC
(1.22&nbsp;mV resolution, 1% accuracy).  It drives separate CHG (charge)
and DSG (discharge) MOSFET outputs for independent control of charging and
discharging.  Communication via I2C allows the ESP32 host to read cell
voltages, set protection thresholds, and enable/disable balancing FETs.</p>
<h3>2-3 Cell Balancing</h3>
<p>Passive balancing dissipates excess energy from the highest-voltage cells
via internal 10&nbsp;A balancing FETs (10&Omega; resistors, ~130&nbsp;mW
per cell at 3.6V).  Active balancing transfers charge from high cells to low
cells using a flyback or capacitor-based converter; more efficient but more
complex.  Passive balancing is adequate for matched cells with &lt;50&nbsp;mV
initial spread.</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>Value / Part</th><th>Purpose</th></tr>
<tr><td>BMS IC</td><td>TI BQ76920</td><td>Cell monitoring and protection</td></tr>
<tr><td>Charge FET</td><td>IRF3205 or STP80NF55</td><td>Charge path switch (CHG)</td></tr>
<tr><td>Discharge FET</td><td>IRF3205 or STP80NF55</td><td>Discharge path switch (DSG)</td></tr>
<tr><td>Current sense resistor</td><td>5&nbsp;m&Omega; 1% (3W)</td><td>Overcurrent detection</td></tr>
<tr><td>I2C host</td><td>ESP32 WROOM-32</td><td>BQ76920 configuration and logging</td></tr>
<tr><td>Display</td><td>CYD 2.8&rdquo; ILI9341</td><td>SOC, cell voltages, current, temp</td></tr>
<tr><td>Temperature sensor</td><td>NTC 10k&Omega; B=3950</td><td>Cell and FET temperature monitoring</td></tr>
<tr><td>Fuse</td><td>ANL 60A or MIDI 40A</td><td>Ultimate short-circuit protection</td></tr>
<tr><td>Cells</td><td>EVE 280Ah or CATL 100Ah LiFePO4</td><td>Energy storage</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<ol>
<li>Connect cells in series (4S): Cell 1 (-) to pack negative; Cell 4 (+) to pack positive
    through the BMS FETs.  Use copper bus bars (&ge;8&nbsp;AWG equivalent) for all
    cell interconnects.</li>
<li>Solder the BQ76920 sense wires to each cell junction: VC0 to pack (&minus;),
    VC1 to Cell 1/2 junction, ..., VC4 to pack (+).  Keep sense leads &lt;100&nbsp;mm;
    twist in pairs; add 1k&Omega; series resistors to protect the BQ76920 inputs.</li>
<li>Install CHG and DSG MOSFETs on a common heatsink (thermal resistance
    &le;2&deg;C/W for 50&nbsp;A continuous).  Connect gate drives from BQ76920
    CHG and DSG outputs via 100&Omega; gate resistors.</li>
<li>Install the current sense resistor (5&nbsp;m&Omega;) in series with the
    discharge path negative bus.  Connect BQ76920 SRP and SRN to each side.</li>
<li>Connect ESP32 I2C (SCL, SDA, 4.7&nbsp;k&Omega; pull-ups to 3.3V) to
    BQ76920 SCL/SDA pins.</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<ol>
<li>First charge: connect a 14.6V constant-voltage charger (LiFePO4 profile,
    CC/CV).  Monitor all four cell voltages on the CYD display during the
    first charge cycle.  If any cell exceeds 3.65V before the others reach
    3.45V, the pack is unbalanced — stop and balance manually.</li>
<li>Normal operation: the CYD displays pack voltage, current, SOC (from
    Coulomb counting), and individual cell voltages.  SOC warning at 20%;
    shutdown at 10% (10.0V).</li>
<li>Temperature limits: if any cell or FET temperature exceeds 55&deg;C during
    discharge, reduce load.  Charging is inhibited below 0&deg;C by firmware.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Calibrate cell voltage offsets: measure each cell with a precision DMM
    (Fluke 15B+).  Compare to BQ76920 I2C readings.  Enter offset corrections
    in firmware NVS (ESP32 Preferences library).</li>
<li>Calibrate current sense: apply a known load (resistive, measured with
    the calibrated DMM).  Adjust firmware current-sense gain constant until
    displayed current matches measured.</li>
<li>Calibrate SOC: fully charge pack to 14.4V.  Reset Coulomb counter to
    100%.  Discharge completely to 10.0V.  Record Ah removed.  Compare to
    cell rated capacity.  Adjust C_nominal in firmware.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>All cell voltages within 20&nbsp;mV of each other at rest (balanced).</li>
<li>OV protection: charge one cell to 3.70V (using external bench supply on
    sense pins).  Verify BQ76920 opens CHG FET within 2 seconds.</li>
<li>UV protection: discharge to 9.8V.  Verify DSG FET opens within 1 second.</li>
<li>Overcurrent: apply a momentary 60&nbsp;A load for 100&nbsp;ms.  Verify
    DSG FET opens (BQ76920 OCD detection).</li>
<li>Log: date, cell batch numbers, initial balance spread (mV), OV trip voltage,
    UV trip voltage, OCD trip current, measured capacity (Ah), operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — LiFePO4 State of Charge vs. Voltage</h2>
<table>
<tr><th>SOC (%)</th><th>Cell voltage (V)</th><th>4S pack voltage (V)</th></tr>
<tr><td>100%</td><td>3.60</td><td>14.40</td></tr>
<tr><td>80%</td><td>3.35</td><td>13.40</td></tr>
<tr><td>50%</td><td>3.25</td><td>13.00</td></tr>
<tr><td>20%</td><td>3.10</td><td>12.40</td></tr>
<tr><td>0%</td><td>2.50</td><td>10.00</td></tr>
</table>
<p>Note: LiFePO4 voltage vs. SOC is very flat from 20%&ndash;80%.  Coulomb
counting is more accurate than voltage for SOC estimation in this range.</p>

<h2 class="chapter-heading" id="appb">Appendix B — BQ76920 Key I2C Registers</h2>
<table>
<tr><th>Register</th><th>Address</th><th>Key bits</th></tr>
<tr><td>SYS_STAT</td><td>0x00</td><td>OV, UV, SCD, OCD, OVRD_ALERT flags</td></tr>
<tr><td>CELLBAL1</td><td>0x01</td><td>CB1&ndash;CB5: balance FET enable (1=on)</td></tr>
<tr><td>SYS_CTRL1</td><td>0x04</td><td>ADC_EN, TEMP_SEL, SHUT_A, SHUT_B</td></tr>
<tr><td>SYS_CTRL2</td><td>0x05</td><td>CHG_ON, DSG_ON, CC_EN (Coulomb counter)</td></tr>
<tr><td>VC1_HI/LO</td><td>0x0C/0x0D</td><td>Cell 1 voltage (14-bit ADC)</td></tr>
</table>
""")

# ── TM-GEAR-005  Coax Traps ───────────────────────────────────────────────────
TOMS['coax_traps'] = dict(
    tm_num="TM-GEAR-005",
    title="COAXIAL CABLE TRAPS — MULTIBAND ANTENNA ISOLATION",
    subtitle="Parallel Resonant Traps, Wheeler Formula, All 15 Ham Bands, RG-58/8X/213",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers the design and construction of coaxial cable traps for
multiband dipoles and verticals.  A coaxial trap is a parallel resonant LC
circuit wound using coaxial cable: the inductance is formed by the coax coil;
the capacitance is the distributed capacitance of the coax itself.  At resonance,
the trap presents high impedance, electrically isolating the antenna element
beyond the trap from the portion below it, enabling multiband operation.</p>
<p>Coax types covered: RG-58/U, RG-8X, RG-213.  Bands: 160M through 23cm
(1.8&ndash;1296&nbsp;MHz) with band-specific former diameters and turn counts.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Resonant Circuit</h3>
<p>The coil inductance follows Wheeler's formula for a short single-layer solenoid:</p>
<pre>L (&micro;H) = r&sup2; &times; N&sup2; / (9r + 10l)
  r = coil radius (inches), N = turns, l = coil length (inches)</pre>
<p>The distributed capacitance of coaxial cable is approximately:</p>
<table>
<tr><th>Coax type</th><th>Capacitance (pF/m)</th><th>OD (mm)</th></tr>
<tr><td>RG-58/U</td><td>93.5</td><td>4.95</td></tr>
<tr><td>RG-8X</td><td>78.7</td><td>6.15</td></tr>
<tr><td>RG-213/U</td><td>96.8</td><td>10.29</td></tr>
</table>
<p>Resonant frequency:</p>
<pre>f0 = 1 / (2&pi; &times; sqrt(L &times; C))</pre>
<p>Q factor: typically 100&ndash;200 for coax traps.  Higher Q means sharper
isolation (narrower stop band) and lower insertion loss in the pass band.</p>
<h3>2-2 Effect on Antenna</h3>
<p>Below the trap resonant frequency, the trap appears as a short inductor
(low impedance) — the full antenna length is active.  At the trap resonant
frequency, high trap impedance isolates the antenna element beyond the trap.
Above the resonant frequency, the trap appears capacitive and can shorten
the resonant length of the inner portion of the antenna.</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Band pair</th><th>Coax</th><th>Former OD</th><th>Approximate turns</th></tr>
<tr><td>40M trap (7.1 MHz)</td><td>RG-8X</td><td>76mm</td><td>7</td></tr>
<tr><td>20M trap (14.175 MHz)</td><td>RG-8X</td><td>50mm</td><td>5</td></tr>
<tr><td>15M trap (21.225 MHz)</td><td>RG-58</td><td>35mm</td><td>4</td></tr>
<tr><td>10M trap (28.5 MHz)</td><td>RG-58</td><td>30mm</td><td>3</td></tr>
<tr><td>2M trap (146 MHz)</td><td>RG-58</td><td>16mm</td><td>2</td></tr>
</table>
<p>Former material: PVC pipe, HDPE pipe, or 3D-printed ABS/PETG.
Secure coax turns with UV-resistant cable ties or tape.</p>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<ol>
<li>Cut the coax to the calculated length: L_coax = N &times; &pi; &times; OD_former
    (one turn circumference times number of turns).</li>
<li>Wind N turns of coax tightly on the former.  Leave 75&nbsp;mm pigtails
    at each end for connection to the antenna element.</li>
<li>Strip 25&nbsp;mm of jacket and braid from each pigtail end to expose
    only the center conductor and dielectric.</li>
<li>At each end, connect the center conductor to the antenna element wire
    and the braid to the antenna element wire (both center and braid connect
    to the same antenna conductor — this is correct for a trap; the trap
    is in parallel with a short section of element).</li>
<li>Use the trap_calculator.py script to verify the calculated resonant
    frequency before cutting.  Input: coax type, former OD, number of turns.</li>
<li>Seal pigtail ends with self-amalgamating tape to prevent water ingress.
    Coat all exposed braid with liquid electrical tape.</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Tuning and Adjustment</h2>
<ol>
<li>Measure the trap resonant frequency using the dip method: couple an
    RF signal source (TinySA or signal generator) to the trap through a
    1-turn coupling loop; sweep the frequency while monitoring the S11 dip
    on the NanoVNA.  The resonant frequency is the dip minimum.</li>
<li>If the measured frequency is too low: remove one turn (the trap is too
    long).  If too high: add a turn or increase the coil pitch (spread turns
    slightly to reduce capacitance).</li>
<li>Target: f_trap within &plusmn;100&nbsp;kHz of the desired band center frequency.</li>
<li>After tuning, secure turns with a bead of weatherproof silicone or
    UV-resistant cable tie at each turn.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Measure trap Q: at resonance, measure the &minus;3&nbsp;dB bandwidth (BW)
    of the impedance peak.  Q = f0 / BW.  Target: Q &ge;100.</li>
<li>Verify isolation: connect the trap in series with a test transmission line.
    Measure S21 at f_trap.  Isolation must be &ge;20&nbsp;dB at f_trap
    for effective band isolation.</li>
<li>Measure insertion loss at off-resonant frequencies (other ham bands).
    Must be &lt;0.5&nbsp;dB at the bands the antenna is intended to operate on.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Resonant frequency within &plusmn;100&nbsp;kHz of target band center.</li>
<li>Q &ge;100 (BW &le;1% of center frequency).</li>
<li>Isolation &ge;20&nbsp;dB at resonant frequency.</li>
<li>Insertion loss &lt;0.5&nbsp;dB at all intended operating frequencies.</li>
<li>Weather seal: immerse trap in water for 10 minutes; verify no water
    ingress to coax ends.</li>
<li>Log: date, coax type, former OD, turns, measured f_trap, Q, isolation, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Wheeler's Formula Worked Example</h2>
<p>40M trap, RG-8X on 76&nbsp;mm former, 7 turns:</p>
<pre>Coil radius r = 76mm / 2 = 38mm = 1.496 inches
Coil length l ≈ 7 × 6.15mm (wire OD) = 43mm = 1.693 inches
L = r² × N² / (9r + 10l) = 1.496² × 49 / (9 × 1.496 + 10 × 1.693)
  = 2.238 × 49 / (13.464 + 16.93) = 109.7 / 30.39 = 3.61 μH

Length of 7 turns on 76mm former: 7 × π × 76mm / 1000 = 1.672 m RG-8X
C = 1.672 m × 78.7 pF/m = 131.6 pF

f0 = 1 / (2π × sqrt(3.61e-6 × 131.6e-12))
   = 1 / (2π × sqrt(4.75e-16))
   = 1 / (2π × 2.179e-8)
   = 7.30 MHz   (close to 40M; adjust turns to fine-tune)</pre>

<h2 class="chapter-heading" id="appb">Appendix B — Winding Table (RG-8X, Selected Bands)</h2>
<table>
<tr><th>Band</th><th>Former OD (mm)</th><th>Turns</th><th>Coax length (m)</th><th>f_calc (MHz)</th></tr>
<tr><td>40M</td><td>76</td><td>7</td><td>1.67</td><td>7.30</td></tr>
<tr><td>20M</td><td>50</td><td>5</td><td>0.79</td><td>14.25</td></tr>
<tr><td>15M</td><td>40</td><td>4</td><td>0.50</td><td>21.3</td></tr>
<tr><td>10M</td><td>35</td><td>3</td><td>0.33</td><td>28.6</td></tr>
</table>
""")

# ── TM-GEAR-006  Coil Winder ──────────────────────────────────────────────────
TOMS['coil_winder'] = dict(
    tm_num="TM-GEAR-006",
    title="MOTORIZED COIL WINDER — ESP32 CONTROLLER",
    subtitle="NEMA 17/23 Dual-Axis, DRV8825/TMC2208, SD Card, OLED Display",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers the motorized coil winder controller: a dual-axis
CNC system that winds RF coils with programmable turn count, pitch, and
wire gauge.  The spindle axis (NEMA&nbsp;23, 2.5&nbsp;A) rotates the winding
former; the traverse axis (NEMA&nbsp;17, 1.5&nbsp;A) advances the wire guide
one wire-diameter per revolution.  The ESP32 controls both axes via DRV8825
or TMC2208 stepper drivers.  Parameters are entered via a front-panel rotary
encoder + OLED display or via USB serial.  A winding program (number of turns,
pitch, wire gauge) is stored on an SD card.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Step Coordination</h3>
<p>For single-layer close-wound coils:
  traverse steps per spindle revolution = wire_OD / traverse_step_size
where traverse_step_size = lead_screw_pitch / steps_per_rev_microstepped.
The ESP32 uses a step-rate ratio to keep the axes coordinated.  Step pulses
are generated via hardware timers (ESP32 MCPWM or timer ISR) to maintain
precise step rates at speeds up to 1000&nbsp;RPM.</p>
<h3>2-2 TMC2208 Advantages</h3>
<p>The TMC2208 provides stealthChop (silent operation) and 256-step
micro-stepping, reducing vibration at low speeds.  Current sensing is
internal (no external sense resistors required).  The UART interface
allows current, chopper mode, and speed ramping to be configured from
the ESP32 at runtime, without hardware changes.</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>Spec / Part</th><th>Purpose</th></tr>
<tr><td>Spindle motor</td><td>NEMA 23, 2.5A, 200 steps/rev</td><td>Rotate winding former</td></tr>
<tr><td>Traverse motor</td><td>NEMA 17, 1.5A, 200 steps/rev</td><td>Advance wire guide</td></tr>
<tr><td>Spindle driver</td><td>TMC2208 or A4988</td><td>Microstepping drive</td></tr>
<tr><td>Traverse driver</td><td>TMC2208 or A4988</td><td>Microstepping drive</td></tr>
<tr><td>Controller</td><td>ESP32 WROOM-32</td><td>Step generation, UI, SD logging</td></tr>
<tr><td>Display</td><td>SSD1306 0.96&rdquo; OLED I2C</td><td>Turn counter, speed, status</td></tr>
<tr><td>Encoder</td><td>KY-040 rotary encoder + button</td><td>Parameter entry</td></tr>
<tr><td>SD card module</td><td>SPI, 3.3V</td><td>Winding program storage</td></tr>
<tr><td>Power supply</td><td>24V 5A (or 12V for NEMA 17)</td><td>Stepper VMOTs</td></tr>
<tr><td>Limit switches</td><td>2× microswitches on traverse</td><td>Home and end-of-travel</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction and Setup</h2>
<ol>
<li>Mount spindle motor to the main frame with its axis aligned with the
    former mandrel.  Use a flexible coupler to compensate for minor
    shaft misalignment.</li>
<li>Mount traverse motor on a lead-screw linear stage.  Lead screw pitch:
    2&nbsp;mm per revolution (M8 or TR8×2).  Mount wire guide on the
    linear stage carriage.</li>
<li>Wire stepper drivers: VMOT to 24V rail, GND, MS1/MS2/MS3 for
    microstepping (TMC2208 SPI-configured; A4988 uses jumpers for
    1/16-step mode).  STEP/DIR to ESP32 GPIO.</li>
<li>Wire limit switches to ESP32 GPIO with 10&nbsp;k&Omega; pull-ups.
    Connect HOME switch to traverse axis minus limit; END switch to plus limit.</li>
<li>SD card: SPI bus (SCK, MOSI, MISO, CS).  SD CS = GPIO5; SD SPI shares
    bus with display if display is SPI type.</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<ol>
<li>Home the traverse axis: press HOME button.  Traverse moves to minus
    limit switch and sets position to zero.</li>
<li>Enter winding parameters via encoder: wire gauge (AWG), turns, coil
    length (or pitch), wind direction (CW or CCW looking at spindle).</li>
<li>Press START.  The controller winds the specified turns, counting on
    the OLED display.  Press PAUSE to stop at any point; press RESUME
    to continue.</li>
<li>After completion: the controller stops and displays DONE + final turn count.
    The completed program is logged to the SD card with date, wire gauge,
    turns, and any errors.</li>
<li>For multi-layer coils: increment the layer counter in the program.
    The traverse will reverse direction automatically for each layer.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Spindle step calibration: command 10 revolutions; count actual revolutions
    with a mark on the former.  Adjust steps_per_rev in firmware if actual
    turns differ from commanded.</li>
<li>Traverse calibration: command 10&nbsp;mm of traverse travel.  Measure
    with a micrometer.  Adjust mm_per_step in firmware.</li>
<li>Wire gauge calibration: measure actual wire OD with a micrometer for
    each gauge in use.  Update the wire_od_mm lookup table in firmware.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Wind a test coil: 10 turns, 28&nbsp;AWG, close-wound, 20&nbsp;mm former.
    Expected coil length: 10 &times; 0.321&nbsp;mm (28&nbsp;AWG OD) = 3.21&nbsp;mm.
    Measure: must be within 0.5&nbsp;mm of expected.</li>
<li>Measure inductance with LCR meter.  Compare to Wheeler's formula.
    Agreement within 10% confirms turn count and pitch are correct.</li>
<li>Step alignment: at end of winding, traverse return to home position
    must be within 1 step of the home limit switch.</li>
<li>Log: date, test coil gauge and turns, measured length, measured
    inductance, step calibration values, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Wire Gauge Reference</h2>
<table>
<tr><th>AWG</th><th>OD bare (mm)</th><th>OD insulated (mm)</th><th>Max current (A)</th></tr>
<tr><td>14</td><td>1.628</td><td>1.95</td><td>15</td></tr>
<tr><td>18</td><td>1.024</td><td>1.30</td><td>7</td></tr>
<tr><td>22</td><td>0.644</td><td>0.85</td><td>3</td></tr>
<tr><td>26</td><td>0.405</td><td>0.55</td><td>1</td></tr>
<tr><td>28</td><td>0.321</td><td>0.45</td><td>0.5</td></tr>
<tr><td>30</td><td>0.255</td><td>0.35</td><td>0.3</td></tr>
</table>

<h2 class="chapter-heading" id="appb">Appendix B — Step Rate vs. RPM</h2>
<pre>Spindle: NEMA 23, 200 steps/rev, 1/16 microstepping = 3200 steps/rev
At 600 RPM: step rate = 600/60 × 3200 = 32,000 steps/sec
At 1200 RPM: 64,000 steps/sec  (near ESP32 timer limit; reduce to 1000 RPM max)

Traverse: M8 lead screw, 1.25 mm pitch
NEMA 17, 200 steps/rev, 1/16 step = 3200 steps/rev
mm per step = 1.25 / 3200 = 0.000391 mm/step  (0.391 μm resolution)</pre>
""")

# ── TM-GEAR-007  Common-Mode Chokes ───────────────────────────────────────────
TOMS['common_mode_chokes'] = dict(
    tm_num="TM-GEAR-007",
    title="COMMON-MODE CHOKES — RFI SUPPRESSION ON FEEDLINES",
    subtitle="Toroidal Coaxial Chokes, Snap-On Ferrite Beads, Sleeve Baluns, Ugly Balun",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers common-mode choke (CMC) construction for suppressing
RF on coaxial feedline shields.  Common-mode current — current flowing on
the outside of the coax shield — causes RFI, pattern distortion, and
RF in the shack.  Four CMC types are covered: toroidal coaxial choke
(FT-240 ferrite), snap-on ferrite bead choke, sleeve balun (1&ndash;4 passes
through a bead string), and the &ldquo;ugly balun&rdquo; (coax coil).</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Differential vs. Common Mode</h3>
<p>In a coaxial feedline, the desired signal travels as a differential mode:
equal and opposite currents on center conductor and inner surface of shield.
The core sees equal and opposite H-fields; they cancel.  The core presents
zero impedance to this mode.</p>
<p>Common-mode current flows on the outside of the shield.  The core sees
an unbalanced H-field and presents the full choking impedance Z_choke
(series impedance) in the common-mode path.  Result:</p>
<pre>CMR (dB) = 20 &times; log10(1 + Z_choke / Z_path)
Z_path = impedance to which common-mode current flows (typically 50&ndash;300&Omega;)

For Z_choke = 5000&Omega;, Z_path = 100&Omega;: CMR = 34 dB
For Z_choke = 1000&Omega;, Z_path = 100&Omega;: CMR = 21 dB</pre>
<h3>2-2 Core Material vs. Frequency</h3>
<p>Core material determines the frequency range of effective choking:</p>
<table>
<tr><th>Material</th><th>Best range</th><th>Peak &mu;"</th><th>Application</th></tr>
<tr><td>#31</td><td>1&ndash;300 MHz</td><td>3000</td><td>HF antenna feedpoints, coax chokes</td></tr>
<tr><td>#43</td><td>1&ndash;100 MHz</td><td>750</td><td>HF balun windings, TLTs</td></tr>
<tr><td>#61</td><td>10&ndash;300 MHz</td><td>125</td><td>VHF chokes, 6M and 2M feedpoints</td></tr>
<tr><td>#73</td><td>1&ndash;40 MHz</td><td>2500</td><td>HF snap-on beads, power line chokes</td></tr>
<tr><td>#75</td><td>0.5&ndash;10 MHz</td><td>5000</td><td>Audio and low-HF chokes</td></tr>
</table>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Design</th><th>Core</th><th>Passes</th><th>Z_choke at 14 MHz</th></tr>
<tr><td>Toroidal coax choke</td><td>FT-240-31, ×2 stacked</td><td>12 passes RG-303</td><td>&ge;5000&Omega;</td></tr>
<tr><td>Snap-on bead string</td><td>Mix #31, 31 mm OD (×6)</td><td>1 pass RG-58 through all 6</td><td>&ge;2000&Omega;</td></tr>
<tr><td>Sleeve balun</td><td>FT-50-61 (×6 stacked)</td><td>4 passes RG-58</td><td>&ge;2000&Omega; at 50&ndash;150 MHz</td></tr>
<tr><td>Ugly balun (coax coil)</td><td>Air core</td><td>8 turns RG-58, 100 mm dia</td><td>&ge;1000&Omega; at 14 MHz</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<h3>4-1 Toroidal Coaxial Choke</h3>
<ol>
<li>Stack two FT-240-31 toroids (same orientation).  Secure with a
    cable tie around the outside.</li>
<li>Thread RG-303 (or RG-58) coax through the stacked cores 12 times.
    Keep passes parallel and tight; each pass reduces the impedance uniformity
    slightly, so 12 is the practical maximum.</li>
<li>Mount in a weatherproof enclosure at the antenna feedpoint.
    Connect input coax to radio-side SO-239; output coax to antenna.</li>
</ol>
<h3>4-2 Snap-On Bead Choke</h3>
<ol>
<li>Open 6 type #31 snap-on ferrite beads (Laird 28A0300-0A2 or equivalent).</li>
<li>Snap all 6 beads onto the coax at the feedpoint, positioned within
    100&nbsp;mm of the antenna connection.  Close each bead securely.</li>
<li>Secure the bead cluster with a cable tie or wrap of self-amalgamating tape.</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<p>A CMC is a passive device; there is no operational procedure other than
installation.  Install CMCs at:</p>
<ul>
<li>Antenna feedpoint (highest priority)</li>
<li>Entry panel where coax enters the shack</li>
<li>Any point where the feedline changes direction sharply</li>
<li>Audio and control cables if RF on those cables is suspected</li>
</ul>
<p>Signs of inadequate common-mode chocking: RF in the shack (microphone
pickup, TVI), pattern distortion on receive, SWR that changes when
hands are placed near the feedline.</p>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Measure choking impedance |Z_choke| with a NanoVNA: connect port 1
    to the coax shield at one end; port 2 to the coax shield at the other
    end (center conductor floating).  Measure |Z| across the band.</li>
<li>Target: |Z_choke| &ge;2000&Omega; at all frequencies in the operating range.
    For type #31 stacked pair: Z should exceed 5000&Omega; from 7&ndash;30&nbsp;MHz.</li>
<li>Verify that differential-mode insertion loss is &lt;0.1&nbsp;dB: connect
    NanoVNA port 1 to input center/shield, port 2 to output center/shield.
    S21 must be &gt;&minus;0.1&nbsp;dB at all HF frequencies.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Z_choke &ge;2000&Omega; at all operating frequencies.</li>
<li>Differential-mode insertion loss &lt;0.1&nbsp;dB.</li>
<li>After installation: receive noise floor on the antenna must not increase
    (some CMC failure modes add noise; verify with SDR noise scan before/after).</li>
<li>Log: date, core type, design (toroid/bead/sleeve/ugly), measured Z_choke
    at 7 and 14&nbsp;MHz, differential insertion loss, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Z_choke vs. Passes on FT-240-31</h2>
<table>
<tr><th>Passes</th><th>Z_choke at 14 MHz (&Omega;)</th><th>Note</th></tr>
<tr><td>6</td><td>&sim;1500</td><td>Adequate; marginal for dipoles</td></tr>
<tr><td>9</td><td>&sim;3000</td><td>Good for most HF antennas</td></tr>
<tr><td>12</td><td>&sim;5000</td><td>Excellent; use for multiband or high-power</td></tr>
<tr><td>15</td><td>&sim;7000</td><td>Maximum practical (coax fills toroid bore)</td></tr>
</table>

<h2 class="chapter-heading" id="appb">Appendix B — Worked CMR Example</h2>
<p>Toroidal choke at dipole feedpoint: Z_choke = 5000&Omega;.
Path impedance to shack ground: Z_path = 100&Omega;.</p>
<pre>CMR = 20 &times; log10(1 + 5000/100) = 20 &times; log10(51) = 20 &times; 1.708 = 34.2 dB</pre>
<p>A 34&nbsp;dB reduction in common-mode current on the feedline.  This should
eliminate most feedline radiation and shack RF problems on HF.</p>
""")

# ── TM-GEAR-008  Crystal Radio ────────────────────────────────────────────────
TOMS['crystal_radio'] = dict(
    tm_num="TM-GEAR-008",
    title="CRYSTAL RADIO — AM BROADCAST AND SHORTWAVE",
    subtitle="Classic Galena Detector, TRF Circuit, Antenna Coupling, Headphone Transformer",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers four crystal radio designs in ascending complexity:
the classic galena (lead sulfide) detector set, a modern germanium diode
version, a tuned radio frequency (TRF) crystal set with antenna impedance
matching, and a headphone transformer for improved audio output.  Crystal
radios require no power supply — they operate entirely on the energy
intercepted by the antenna.</p>
<p><strong>Frequency range:</strong> AM broadcast band (530&ndash;1700&nbsp;kHz)
with the classic design; extended to shortwave (3&ndash;30&nbsp;MHz) with
the TRF design using smaller coils.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Tuned Circuit</h3>
<p>A parallel LC tank circuit resonates at the desired station frequency.
At resonance, the circuit impedance is maximum (Z = L/(R&times;C), where R
is coil resistance); off-resonant signals see low impedance and are bypassed.
Selectivity is determined by Q = &omega;L/R; higher Q means sharper tuning
but also higher loss.  For AM broadcast: L &asymp; 250&nbsp;&micro;H,
C = 15&ndash;365&nbsp;pF (variable).</p>
<h3>2-2 Detector Diode</h3>
<p>The detector diode (galena crystal, 1N34A germanium, or BAT42 Schottky)
rectifies the AM carrier, demodulating the audio.  A bypass capacitor
(100&nbsp;pF to 1&nbsp;&micro;F depending on audio frequency range)
filters the RF carrier, leaving the audio envelope.  Germanium diodes
have lower forward voltage (&sim;0.2V) than silicon; they work with weaker
signals.  Galena (natural PbS crystal) historically achieved the highest
sensitivity when point-contacting the sweet spot on the crystal.</p>
<h3>2-3 Antenna Coupling</h3>
<p>The antenna couples RF energy into the tuned circuit.  Over-coupling
loads the tank circuit, reducing Q and selectivity.  Under-coupling
reduces signal level.  Optimal coupling: the antenna resonant impedance
is tapped partway down the coil (inductive coupling) to match the low
antenna impedance (&sim;100&ndash;300&Omega;) to the high tank circuit
impedance (&sim;50&ndash;200&nbsp;k&Omega;).</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>Classic</th><th>Modern TRF</th></tr>
<tr><td>Coil</td><td>Oatmeal box, 90 turns #28 AWG</td><td>Litz wire, 75 turns on ferrite rod</td></tr>
<tr><td>Variable capacitor</td><td>Air-variable, 10&ndash;365 pF</td><td>Same; 2-gang for preselector</td></tr>
<tr><td>Detector</td><td>Galena + cat's whisker</td><td>1N34A or OA91 germanium</td></tr>
<tr><td>Headphones</td><td>High-impedance crystal earphone (2200&Omega;)</td><td>Same or 32&Omega; + transformer</td></tr>
<tr><td>Antenna coupling</td><td>1-turn coupling loop</td><td>Tapped coil or coupling coil</td></tr>
<tr><td>Ground</td><td>Cold water pipe</td><td>Counterpoise or cold water pipe</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<h3>4-1 Classic Coil on Oatmeal Box</h3>
<ol>
<li>Wind 90 turns of #28 AWG enameled copper wire on a 90&nbsp;mm OD
    cardboard tube (oatmeal box).  Wind in a single layer, close-wound.
    Mark the center tap at turn 45 for antenna connection point.</li>
<li>Scrape enamel from the wire at 10-turn intervals (turns 10, 20, ..., 80)
    and solder short leads.  These form taps for the antenna coupling
    adjustment.</li>
<li>Connect a variable air capacitor (10&ndash;365&nbsp;pF) in parallel
    with the full winding.  The capacitor shaft is the tuning control.</li>
<li>Connect the galena crystal or 1N34A diode in series with the headphones
    across the tank circuit.  Cathode toward the high-impedance (ungrounded)
    end of the tank.</li>
</ol>
<h3>4-2 Headphone Impedance Transformer</h3>
<ol>
<li>Wind an audio transformer on a laminated iron core: primary 2000
    turns #38 AWG (matches high crystal set impedance); secondary 60 turns
    #26 AWG (matches 32&Omega; headphone).</li>
<li>Turns ratio: n = sqrt(Z_primary / Z_secondary) = sqrt(2000/32) = 7.9:1.</li>
<li>Actual winding: 2000:250 turns is a practical compromise for ease
    of winding (ratio = 8:1 = 9&nbsp;dB power gain vs. direct connection).</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<ol>
<li>Connect a long outdoor wire antenna (30&nbsp;m or more) and a good ground
    (cold water pipe, driven rod, or extensive counterpoise).</li>
<li>Put on high-impedance crystal earphones (or low-impedance via transformer).
    Rotate the tuning capacitor slowly from maximum capacitance to minimum.
    Stations appear as increases in audio level at their resonant settings.</li>
<li>Adjust antenna coupling tap for best audio level without degrading
    selectivity.  Experiment: more coupling gives more volume but reduces
    the ability to separate adjacent stations.</li>
<li>For galena detector: use the cat&rsquo;s whisker to probe different spots
    on the crystal surface.  The sensitive spot is small — a light touch
    is more effective than pressure.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Calibrate the tuning dial: note the capacitor setting when each known
    station is tuned.  Mark a scale with station frequencies.  This converts
    the dial to a frequency scale using the known stations as references.</li>
<li>Estimate Q: measure the &minus;3&nbsp;dB bandwidth of the tank circuit
    at 1&nbsp;MHz with a signal generator and detector.  Q = f / BW.
    Target: Q &ge;100 for good selectivity (BW &le;10&nbsp;kHz).</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Receive at least three local AM broadcast stations, each distinctly
    tunable by the variable capacitor.</li>
<li>Adjacent station rejection: tune to one station and verify that the
    next station 10&nbsp;kHz away is at least 20&nbsp;dB weaker.
    (Measure with CYD ADC or compare audio levels subjectively.)</li>
<li>Log: date, antenna length and ground type, stations received, Q
    measurement result, detector type used.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Resonant Frequency Formula</h2>
<pre>f0 = 1 / (2&pi; &times; sqrt(L &times; C))

AM broadcast coverage:
  At C_max = 365 pF, f0 = 1/(2&pi;×sqrt(250e-6 × 365e-12)) = 527 kHz
  At C_min = 10 pF,  f0 = 1/(2&pi;×sqrt(250e-6 × 10e-12))  = 3.18 MHz
  (Reduce L to 50 μH for higher frequency coverage)</pre>

<h2 class="chapter-heading" id="appb">Appendix B — Crystal Detector Sensitivity Comparison</h2>
<table>
<tr><th>Detector type</th><th>Min detectable signal</th><th>Forward voltage</th><th>Notes</th></tr>
<tr><td>Galena (natural)</td><td>&sim;0.05 mV</td><td>0.05&ndash;0.15V</td><td>Variable; needs cat's whisker adjustment</td></tr>
<tr><td>1N34A germanium</td><td>&sim;0.1 mV</td><td>0.15&ndash;0.25V</td><td>Reliable; second best for sensitivity</td></tr>
<tr><td>BAT42 Schottky</td><td>&sim;0.2 mV</td><td>0.25&ndash;0.35V</td><td>Fast; better for shortwave; needs more signal</td></tr>
<tr><td>1N4148 silicon</td><td>&sim;2 mV</td><td>0.55&ndash;0.70V</td><td>Poor sensitivity; not recommended</td></tr>
</table>
""")

# ── TM-GEAR-009  Hybrids ──────────────────────────────────────────────────────
TOMS['hybrids'] = dict(
    tm_num="TM-GEAR-009",
    title="RF HYBRID COUPLERS — QUADRATURE, WILKINSON, AND RAT-RACE",
    subtitle="Branch-Line 90° Hybrid, Wilkinson 2-Way and 4-Way, Rat-Race 180° Coupler",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers four RF hybrid coupler designs for splitting and combining
RF power with controlled phase relationships: the branch-line quadrature hybrid
(90&deg; phase split, equal amplitude), the Wilkinson 2-way in-phase power
divider, the 4-way Wilkinson, and the rat-race (180&deg; hybrid ring).
Applications include circular polarization feeds, phased array antenna networks,
balanced amplifier stages, and antenna switching networks.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Branch-Line Quadrature Hybrid</h3>
<p>A 4-port network constructed from four &lambda;/4 transmission line sections.
Two shunt arms have characteristic impedance Z<sub>0</sub> = 50&Omega;;
two series (coupling) arms have Z<sub>0</sub>/&radic;2 = 35.35&Omega;.
S-parameter matrix at center frequency:</p>
<table>
<tr><th>Port pair</th><th>Magnitude</th><th>Phase</th></tr>
<tr><td>Input &rarr; Through (P1&rarr;P2)</td><td>&minus;3.01 dB</td><td>0&deg;</td></tr>
<tr><td>Input &rarr; Coupled (P1&rarr;P3)</td><td>&minus;3.01 dB</td><td>&minus;90&deg;</td></tr>
<tr><td>Input &rarr; Isolated (P1&rarr;P4)</td><td>&lt;&minus;40 dB</td><td>N/A</td></tr>
</table>
<h3>2-2 Wilkinson Power Divider</h3>
<p>A 3-port network that splits input power equally between two output ports
with 0&deg; phase (in-phase).  Ports 2 and 3 are isolated from each other
when both are terminated in Z<sub>0</sub>.  The &lambda;/4 arms have
characteristic impedance Z<sub>0</sub>&radic;2 = 70.7&Omega; for a 50&Omega; system.
An isolation resistor R = 2Z<sub>0</sub> = 100&Omega; connects ports 2 and 3.</p>
<h3>2-3 Rat-Race (180&deg; Hybrid Ring)</h3>
<p>A ring of transmission line (total circumference 1.5&lambda;) with four
ports: sum (&Sigma;) port provides in-phase combination; difference (&Delta;)
port provides 180&deg; phase difference between ports.  Ring impedance:
Z<sub>ring</sub> = Z<sub>0</sub>&radic;2 = 70.7&Omega;.  Used for balanced
mixers and antenna pattern summation/difference.</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Design</th><th>Implementation</th><th>PCB trace width (50&Omega; FR4)</th><th>Note</th></tr>
<tr><td>Branch-line hybrid</td><td>PCB microstrip or lumped LC at HF</td><td>2.9mm (50&Omega;); 5.0mm (35.35&Omega;)</td><td>Best for VHF/UHF</td></tr>
<tr><td>Wilkinson 2-way</td><td>PCB microstrip, 2&times; &lambda;/4 arms</td><td>1.7mm (70.7&Omega;)</td><td>100&Omega; isolation R between ports</td></tr>
<tr><td>Wilkinson 4-way</td><td>Two cascaded Wilkinson stages</td><td>Same</td><td>3&nbsp;dB + 3&nbsp;dB = 6&nbsp;dB split</td></tr>
<tr><td>Rat-race</td><td>PCB ring, 70.7&Omega; trace</td><td>1.7mm</td><td>3&lambda;/2 ring circumference</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<h3>4-1 Branch-Line Hybrid (144 MHz)</h3>
<ol>
<li>Calculate &lambda;/4 at 144&nbsp;MHz on FR4 (1.6&nbsp;mm, &epsilon;r=4.6):
    &lambda;/4 = c/(4 &times; f &times; &radic;&epsilon;eff) &asymp; 350&nbsp;mm / 4 = 87.5&nbsp;mm.
    Account for effective permittivity (&epsilon;eff &asymp; 3.4): 87.5 / &radic;3.4 = 47.5&nbsp;mm.</li>
<li>Draw the four-arm square layout: each arm 47.5&nbsp;mm long.  Shunt arms:
    2.9&nbsp;mm wide (50&Omega;).  Series arms: 5.0&nbsp;mm wide (35.35&Omega;).</li>
<li>Mount SMA connectors at each of the four corners.  The square layout
    minimizes parasitic coupling between arms.</li>
</ol>
<h3>4-2 Lumped-Element HF Hybrid (7&ndash;30&nbsp;MHz)</h3>
<ol>
<li>Replace each &lambda;/4 line section with an equivalent LC &pi;-network:
    L = Z<sub>0</sub>/&omega; = 50/(2&pi;&times;14e6) = 568&nbsp;nH (shunt arms);
    C = 1/(&omega;&times;Z<sub>0</sub>) = 1/(2&pi;&times;14e6&times;50) = 227&nbsp;pF.</li>
<li>Use 1% tolerance components to maintain amplitude and phase balance.</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<h3>5-1 Circular Polarization Feed</h3>
<ol>
<li>Connect the transceiver to Port 1 (input) of the branch-line hybrid.
    Connect a horizontally polarized element to Port 2 (0&deg;) and a
    vertically polarized element to Port 3 (&minus;90&deg;).</li>
<li>Port 4 (isolated) is terminated in a 50&Omega; load — do not leave it
    open; it must be terminated for correct operation.</li>
<li>Result: the two antenna elements receive equal power with 90&deg; phase
    offset, producing circular polarization.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Branch-line hybrid: with NanoVNA, measure S11 at Port 1 (must be
    &lt;&minus;20&nbsp;dB return loss at f<sub>0</sub>) and S21 to ports 2
    and 3 (&minus;3 &plusmn; 0.5&nbsp;dB each).  Measure S31 vs. S21 phase
    (must be &minus;90 &plusmn; 5&deg;).  S41 must be &lt;&minus;30&nbsp;dB
    (isolation).</li>
<li>Wilkinson: S11 &lt;&minus;20&nbsp;dB; S21 = S31 = &minus;3 &plusmn; 0.5&nbsp;dB;
    S23 (isolation) &lt;&minus;20&nbsp;dB at center frequency.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Amplitude balance: |S21| &minus; |S31| &lt;0.5&nbsp;dB.</li>
<li>Phase accuracy: &Delta;&phi; within &plusmn;5&deg; of design value (90&deg;
    or 0&deg;) at center frequency.</li>
<li>Return loss (input match): &lt;&minus;20&nbsp;dB at center frequency.</li>
<li>Isolation: &lt;&minus;20&nbsp;dB between output ports.</li>
<li>Power handling (Wilkinson): verify 100&Omega; isolation resistor rating.
    At 100W input, the resistor dissipates up to 50W if ports 2 and 3
    are mismatched — use 5W resistor minimum.</li>
<li>Log: date, design type, center frequency, S21/S31 balance, phase error,
    return loss, isolation, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Microstrip Impedance vs. Trace Width (FR4 1.6mm)</h2>
<table>
<tr><th>Z0 (&Omega;)</th><th>Trace width (mm)</th><th>Application</th></tr>
<tr><td>50</td><td>2.9</td><td>Main port connections</td></tr>
<tr><td>35.35</td><td>5.0</td><td>Branch-line series arms (Z0/&radic;2)</td></tr>
<tr><td>70.7</td><td>1.7</td><td>Wilkinson arms, rat-race ring (Z0&times;&radic;2)</td></tr>
</table>

<h2 class="chapter-heading" id="appb">Appendix B — Wavelength Scaling Table</h2>
<table>
<tr><th>Band</th><th>&lambda;/4 in air (mm)</th><th>&lambda;/4 on FR4 (mm)</th></tr>
<tr><td>6M (50 MHz)</td><td>1500</td><td>813</td></tr>
<tr><td>2M (144 MHz)</td><td>521</td><td>282</td></tr>
<tr><td>1.25M (222 MHz)</td><td>338</td><td>183</td></tr>
<tr><td>70cm (432 MHz)</td><td>174</td><td>94</td></tr>
<tr><td>23cm (1296 MHz)</td><td>58</td><td>31</td></tr>
</table>
""")

# ── TM-GEAR-010  Lightning Protection ─────────────────────────────────────────
TOMS['lightning_protection'] = dict(
    tm_num="TM-GEAR-010",
    title="LIGHTNING PROTECTION AND STATION GROUNDING",
    subtitle="Single-Point Entry Panel, GDT Arrestors, Multi-Stage Protection, NEC Article 810",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers station grounding and lightning protection per NEC
Article 810, NFPA 780, and ARRL station grounding practices.  The scope
includes: single-point entry panel construction, coaxial gas discharge tube
(GDT) arrestors, multi-stage protection (GDT + MOV + TVS), ground rod
installation, and bonding strap sizing.</p>
<p><strong>WARNING — LIGHTNING IS LETHAL.</strong>  This manual describes
protection against induced surges and near-miss events.  A direct strike
cannot be &ldquo;protected against&rdquo; — only diverted.  All station
equipment must be disconnected from antennas and utility power during
active thunderstorm activity.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Grounding Philosophy</h3>
<p>Station grounding serves four purposes in order of importance:</p>
<ol>
<li><strong>Safety:</strong> Prevent fatal shock from faulted equipment chassis.</li>
<li><strong>RF common-mode elimination:</strong> Single-point RF ground prevents loops.</li>
<li><strong>Static discharge:</strong> Continuous low-impedance path for accumulated charge.</li>
<li><strong>Lightning diversion:</strong> Guide induced surge energy away from equipment.</li>
</ol>
<p>A ground system must be: low-impedance at DC and RF (bonding), physically
continuous, connected in a star (single-point) topology, and compliant with
NEC 250 and Article 810.</p>
<h3>2-2 GDT Arrestors</h3>
<p>A gas discharge tube (GDT) is a two-electrode spark gap in a sealed tube
filled with inert gas (typically argon/neon at low pressure).  At normal
voltages, the GDT is an open circuit.  When the voltage exceeds the DC
sparkover voltage (typically 90&ndash;230&nbsp;V), the gas ionizes and
the GDT conducts, clamping the voltage.  The follow-through current must
be broken by a series fuse or the ionization quenches when the surge passes.</p>
<h3>2-3 Multi-Stage Protection</h3>
<p>A single GDT clamps most surges but may allow brief overvoltages during
the turn-on delay (&sim;1&nbsp;&micro;s).  A two-stage design adds a MOV
(metal oxide varistor) or TVS (transient voltage suppressor) in series/parallel
after the GDT to clamp faster (&lt;1&nbsp;ns) at lower voltages.  The series
impedance between stages slows the surge, giving the TVS time to respond.</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>Spec / Part</th><th>Purpose</th></tr>
<tr><td>Coax GDT arrestors</td><td>Polyphaser IS-B50LN-C2 or similar</td><td>Coax feedline protection</td></tr>
<tr><td>Entry panel</td><td>6mm copper plate, 300&times;150mm</td><td>Single-point bonding bus</td></tr>
<tr><td>Ground rods</td><td>8&prime; copper-clad steel (×2), connected by #6 AWG</td><td>Earth connection</td></tr>
<tr><td>Bonding straps</td><td>1&Prime; copper braid or 1/2&Prime; copper strap</td><td>Panel to ground rod, equipment to panel</td></tr>
<tr><td>AC protector</td><td>Whole-house SPD (200A rated)</td><td>AC mains surge at service entrance</td></tr>
<tr><td>TVS diodes</td><td>P6KE68A (60V standby, 600W peak)</td><td>Secondary protection on control lines</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<h3>4-1 Ground Rod Installation</h3>
<ol>
<li>Drive two 8&prime; ground rods at least 16&prime; apart (one rod length spacing
    minimum per NEC 250).  Rods must be buried full depth (8&prime;/2.4&nbsp;m).</li>
<li>Bond rods together with #6 AWG bare copper, buried 300&nbsp;mm below grade.
    Use irreversible compression connectors (Burndy or equivalent) — never
    rely on wire wrapped around the rod.</li>
<li>Run the ground conductor from the rod cluster to the entry panel via the
    shortest possible path (&lt;3&nbsp;m is ideal; avoid right-angle bends).</li>
</ol>
<h3>4-2 Entry Panel</h3>
<ol>
<li>Mount the copper panel on the exterior wall at the point where all antenna
    cables enter the shack.  Mount as close to the ground rod as possible.</li>
<li>All coaxial feedlines must pass through the panel via GDT-equipped
    bulkhead connectors.  All cable shields must bond directly to the panel
    at this point.</li>
<li>Bond the panel to the ground rod cluster with a minimum 1/2&Prime; copper
    strap (no wire — strap has lower RF impedance).</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<ol>
<li><strong>During thunderstorm operation:</strong> Do not operate within 10 miles
    of an active thunderstorm.  The standard amateur radio recommendation is:
    if you can hear thunder, disconnect and shelter indoors.</li>
<li><strong>When leaving the station unattended:</strong> Disconnect all antenna
    feedlines at the transceiver and connect the coax shields to ground
    with shorting caps or a dedicated disconnect switch.</li>
<li><strong>Annual inspection:</strong> Check all bonding connections for corrosion
    (green copper oxide is normal and protective; white powder indicates
    aluminum corrosion and loss of continuity).  Check GDT arrestors for
    carbon tracking (indicate previous discharge) — replace if discharged.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Measure ground resistance: use a fall-of-potential test (three-point
    method) or a clamp-on ground resistance tester.  Target: &lt;10&Omega;
    per NEC; &lt;5&Omega; recommended for RF stations.</li>
<li>Verify continuity of all bonding straps with a low-resistance ohmmeter:
    any station ground conductor must read &lt;0.1&Omega; end-to-end.</li>
<li>GDT sparkover voltage: verify with a high-voltage tester if available.
    The sparkover voltage should match the manufacturer rating (typically
    90&ndash;230&nbsp;V depending on type).</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Ground resistance &lt;5&Omega; (measure with fall-of-potential method).</li>
<li>All bonding straps continuous (&lt;0.1&Omega;).</li>
<li>All coax feedlines pass through the entry panel with no direct path
    that bypasses the GDT.</li>
<li>No RF current on coax shields within the shack (measure with clip-on RF
    current probe; target &lt;10&nbsp;mA during transmit on any feedline
    entering the shack).</li>
<li>Log: date, ground rod resistance, number of GDT units installed and
    tested, bonding strap resistance, RF current check, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — NEC Article 810 Summary</h2>
<ul>
<li>810-21(f)(1): antenna discharge unit must be connected to ground electrode</li>
<li>810-21(f)(2): ground conductor must be #10 AWG copper minimum</li>
<li>810-21(h): ground conductor must be run in shortest path, without sharp bends</li>
<li>810-20(a): antenna must be kept &ge;3 feet from power conductors</li>
<li>250-50: all grounding electrodes must be bonded together</li>
</ul>

<h2 class="chapter-heading" id="appb">Appendix B — Bonding Strap Sizing</h2>
<table>
<tr><th>Strap type</th><th>DC resistance / foot</th><th>RF impedance at 14 MHz</th><th>Use</th></tr>
<tr><td>1/4&Prime; copper strap</td><td>0.3 m&Omega;</td><td>Medium</td><td>Minimum for station bonding</td></tr>
<tr><td>1/2&Prime; copper strap</td><td>0.15 m&Omega;</td><td>Low</td><td>Panel to ground rod</td></tr>
<tr><td>1&Prime; copper braid</td><td>0.08 m&Omega;</td><td>Very low (skin effect)</td><td>Equipment chassis bonds</td></tr>
<tr><td>#6 AWG solid copper</td><td>0.4 m&Omega;</td><td>High (inductance)</td><td>Underground bonding only; not RF</td></tr>
</table>
""")

# ── TM-GEAR-011  Mast and Tower ───────────────────────────────────────────────
TOMS['mast_tower'] = dict(
    tm_num="TM-GEAR-011",
    title="MAST AND TOWER — PORTABLE AND FIXED INSTALLATIONS",
    subtitle="Wind Load Calculations, Pneumatic Mast, Crank-Up Tower, Aluminum Telescoping",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers structural requirements and installation procedures for
four antenna support types: aluminum telescoping push-up mast (10&ndash;12&nbsp;m),
pneumatic air mast (7&ndash;9&nbsp;m, rapid deployment), crank-up tower
(10&ndash;30&nbsp;m, non-crank-up portable variant), and guyed vertical mast.
Wind load calculations follow EIA/TIA-222-H and ASCE 7-22.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Wind Load Calculation</h3>
<p>The design wind pressure is:</p>
<pre>q (Pa) = 0.613 &times; V&sup2;   (V in m/s)
q (psf) = 0.00256 &times; V&sup2;  (V in mph)

Design defaults:
  Operational: 50 mph (22.4 m/s) &rarr; q = 308 Pa
  Operational max: 70 mph (31.3 m/s) &rarr; q = 600 Pa
  Survival: 90 mph (40.2 m/s) &rarr; q = 990 Pa</pre>
<p>The force on a cylindrical mast section (vertical member):</p>
<pre>F (N) = q &times; C_d &times; A
  C_d = 1.0 (cylinder drag coefficient)
  A = projected area = diameter (m) &times; height (m)</pre>
<h3>2-2 Guy Wire Tension</h3>
<p>For a guyed mast, the overturning moment M = F &times; h/2.  Guy wire
tension T = M / (r &times; cos&theta;), where r = guy radius and &theta; = angle
from horizontal.  For 45&deg; guys: T = M / (r &times; 0.707).
Use 3:1 safety factor on all guy wire ratings.</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>Telescoping</th><th>Pneumatic</th><th>Crank-Up</th></tr>
<tr><td>Sections</td><td>6&ndash;8 &times; 1.5m Al tube</td><td>Fiberglass, 4 sections</td><td>Steel lattice, 3&ndash;6</td></tr>
<tr><td>Material</td><td>6061-T6 aluminum</td><td;>Fiberglass tube</td><td>Galvanized steel</td></tr>
<tr><td>Guys</td><td>3&times; at each level</td><td>None (self-supporting to 9m)</td><td>3&times; per section</td></tr>
<tr><td>Guy wire</td><td>1/4&Prime; Phillystran or Dacron</td><td>—</td><td>3/16&Prime; EHS steel</td></tr>
<tr><td>Anchors</td><td>Screw anchors, 500&nbsp;kg</td><td>—</td><td>Concrete deadman</td></tr>
<tr><td>Base</td><td>Tripod or plate, 0.6m</td><td>Plate, 0.3m</td><td>Concrete base, 1m</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction and Erection</h2>
<h3>4-1 Telescoping Mast Erection</h3>
<ol>
<li>Install the base plate on level ground.  Anchor with 4&times;screw anchors
    or 4&times;250&nbsp;kg sandbags in field operation.</li>
<li>Extend sections one at a time, starting from the bottom.  Tighten each
    section clamp before extending the next.</li>
<li>Install guys at three levels: 1/3, 2/3, and top of full height.
    Guy radius must be &ge;40% of height (rule of thumb); recommended 60%.</li>
<li>Tension all guys evenly.  Mast vertical alignment: check with a plumb
    bob from the top.  Adjust guys until mast is plumb to within 1&deg;.</li>
</ol>
<h3>4-2 Pneumatic Mast</h3>
<ol>
<li>Extend collapsed mast horizontally.  Connect air supply (bicycle pump,
    foot pump, or compressor) to the base Schrader valve.</li>
<li>Pump to 2.5&ndash;3.5&nbsp;bar (36&ndash;51&nbsp;psi).  The sections
    extend sequentially from top to bottom.  Full extension: approximately
    60 pump strokes with a standard floor pump.</li>
<li>Once extended, set upright.  Install base plate or drive the ground
    spike.  Pneumatic masts are self-supporting to 9&nbsp;m with no antenna
    wind load; add guys for wind exposure or antenna weight &gt;2&nbsp;kg.</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<ol>
<li>Never erect a mast within fall distance of power lines.
    Required clearance: mast height + 3&nbsp;m.</li>
<li>For portable operation, lower the antenna before moving the mast.
    Telescoping sections must be collapsed before transport; partially
    extended masts have the lowest collapse load capacity.</li>
<li>Inspect all guy wire attachment points before each use.  Replace any
    guy wire showing kinks, broken strands, or corrosion at terminations.</li>
<li>In winds exceeding 50&nbsp;mph: lower the antenna or lower the mast
    entirely if the design survival wind speed is less than the forecast wind.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Measure actual mast height with a measuring tape or laser rangefinder.
    Record height at each clamp position for future reference.</li>
<li>Verify guy wire tension: each guy in a set of three should be tensioned
    equally.  Use a wire tension gauge; target 10&ndash;15% of rated breaking
    strength for operational tension.</li>
<li>Plumb verification: with all guys tensioned, measure horizontal offset
    at the top with a plumb line.  Must be &lt;1% of mast height.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Mast plumb within 1% of height (e.g., &lt;0.1&nbsp;m offset for a 10&nbsp;m mast).</li>
<li>All guy wires tensioned and attachment hardware secure (no loose
    turnbuckles, no shackle pins without cotter pins).</li>
<li>Wind load calculation completed and documented for the installed configuration
    (mast height, antenna size, design wind speed).</li>
<li>Log: date, mast type, height, antenna mounted, guy radius, design
    wind speed, tension measurements, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Worked Wind Load Example</h2>
<p>10&nbsp;m aluminum mast, 50&nbsp;mm OD, at 70&nbsp;mph (31.3&nbsp;m/s):</p>
<pre>q = 0.613 &times; 31.3&sup2; = 0.613 &times; 979 = 600 Pa
A = 0.050 m &times; 10 m = 0.50 m&sup2;
F = 600 &times; 1.0 &times; 0.50 = 300 N (67 lbf) on the mast alone

Add 2-element Yagi (projected area ~0.5 m&sup2;) at top:
F_antenna = 600 &times; 1.2 &times; 0.50 = 360 N
Total F = 300 + 360 = 660 N (148 lbf)
Overturning moment M = 660 &times; 5 m = 3300 N·m (mast + antenna CG)

Guy wire tension (3 guys, 45° angle, 6 m radius):
T = 3300 / (6 &times; 0.707 &times; 2/3) = 3300 / 2.83 = 1166 N per guy
Use 1/4" Phillystran (rated 2200 N) with 3:1 safety factor.</pre>

<h2 class="chapter-heading" id="appb">Appendix B — Mast Tube Wall Thickness Reference</h2>
<table>
<tr><th>OD (mm)</th><th>Wall (mm)</th><th>Max height (unguyed)</th><th>Max height (1 set guys)</th></tr>
<tr><td>32</td><td>2.0</td><td>4 m</td><td>6 m</td></tr>
<tr><td>38</td><td>2.5</td><td>5 m</td><td>8 m</td></tr>
<tr><td>50</td><td>3.0</td><td>6 m</td><td>10 m</td></tr>
<tr><td>63</td><td>3.5</td><td>8 m</td><td>12 m</td></tr>
</table>
""")

# ── TM-GEAR-012  Motorized Tuner ──────────────────────────────────────────────
TOMS['motorized_tuner'] = dict(
    tm_num="TM-GEAR-012",
    title="MOTORIZED ANTENNA TUNER — HF AND VHF/UHF",
    subtitle="T-Network, L-Network, Stub Tuner, ESP32 Controller, SWR-Guided Auto-Tune",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers a motorized antenna tuner (transmatch) covering all
15 amateur radio bands (1.8&ndash;1300&nbsp;MHz) in two sections: an HF
T-network tuner (1.8&ndash;30&nbsp;MHz, 100W) and a VHF/UHF stub or L-network
tuner (50&ndash;1300&nbsp;MHz, 50W).  Variable capacitors and a roller
inductor are driven by DC gear motors under ESP32 control.  A built-in
SWR bridge provides feedback for automatic SWR minimization.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 T-Network HF Tuner</h3>
<p>The balanced T-network (input cap C1, series inductor L, output cap C2)
can match a wide range of impedances to 50&Omega;.  Typical range:
5&ndash;2000&Omega;, with moderate losses at high transformation ratios.
L is a roller inductor (variable inductance by tapping along the coil);
C1 and C2 are motorized air variables.</p>
<h3>2-2 VHF Stub Tuner</h3>
<p>A short-circuit stub (adjustable-length transmission line) in shunt with
the antenna feedline cancels the reactive component of the antenna impedance.
The stub length is adjusted by a motorized sliding short-circuit piston.
A quarter-wave transformer section then converts the resistive remainder
to 50&Omega;.  Effective for narrowband VHF/UHF matching.</p>
<h3>2-3 SWR-Guided Auto-Tune Algorithm</h3>
<p>The ESP32 reads the SWR bridge ADC outputs (V_fwd, V_ref) and minimizes
the SWR by a coordinate descent algorithm: adjust L in one direction;
if SWR decreases, continue; if it increases, reverse.  Then adjust C1,
then C2.  Iterate until SWR &lt;1.5:1 or no further improvement.</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>Value / Part</th><th>Purpose</th></tr>
<tr><td>HF capacitors C1, C2</td><td>365&nbsp;pF air variable, 2500V rating</td><td>HF T-network tuning</td></tr>
<tr><td>Roller inductor</td><td>0&ndash;28&nbsp;&micro;H, silver-plated</td><td>Series inductance</td></tr>
<tr><td>C1, C2 motors</td><td>12V DC gear motor, 6 RPM</td><td>Motorize air variables</td></tr>
<tr><td>L motor</td><td>12V DC gear motor, 3 RPM</td><td>Drive roller inductor</td></tr>
<tr><td>Position sensors</td><td>10-turn wirewound pot (×3)</td><td>Report capacitor/inductor position</td></tr>
<tr><td>SWR bridge</td><td>Toroid coupler + 1N5711 (from TM-GEAR-015)</td><td>Forward/reflected sampling</td></tr>
<tr><td>Motor driver</td><td>L298N dual H-bridge (×2)</td><td>Bidirectional motor control</td></tr>
<tr><td>Controller</td><td>ESP32 WROOM-32</td><td>Auto-tune algorithm, display</td></tr>
<tr><td>Display</td><td>CYD 2.8&rdquo; ILI9341</td><td>SWR, tuner settings, band</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<ol>
<li>Mount air variable capacitors in the enclosure with motor shafts coupled
    via flexible shaft couplers.  Use Hall-effect limit switches at both
    ends of travel to prevent over-rotation.</li>
<li>Mount the roller inductor vertically.  Couple the DC motor to the roller
    shaft via a gear train (10:1 reduction for precise positioning).</li>
<li>Connect the 10-turn pots to the capacitor and inductor shafts via
    1:1 direct couplers.  Wire pot center tap to ESP32 ADC via a 10&nbsp;k&Omega;
    voltage divider.</li>
<li>Mount the SWR bridge inline between the radio connector and the T-network
    input.  Connect VFWD and VREF to ESP32 ADC.</li>
<li>Wire L298N H-bridges: IN1/IN2 and IN3/IN4 to ESP32 GPIOs (PWM for
    speed control); OUT1/OUT2 and OUT3/OUT4 to motors.  Enable pins to 3.3V
    for always-on.</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<h3>5-1 Manual Tuning</h3>
<ol>
<li>Select band on the CYD touchscreen.  The tuner recalls the last
    successful setting for that band and moves the components to that position.</li>
<li>Key the transmitter at reduced power (5&ndash;10W for initial tune).
    Observe the SWR on the CYD display.  Adjust C1, C2, and L buttons
    until SWR &lt;1.5:1.</li>
<li>Increase power to operating level and verify SWR does not change
    significantly (if it does, the tuner is not stable at operating power).</li>
</ol>
<h3>5-2 Auto-Tune</h3>
<ol>
<li>Key the transmitter at 10W.  Press AUTO on the CYD.  The ESP32 runs
    the coordinate descent algorithm.  Typical tuning time: 10&ndash;30 seconds.</li>
<li>The display shows SWR progress.  When SWR &lt;1.5:1, the algorithm stops
    and saves the band setting to NVS.</li>
<li>If auto-tune reaches minimum movement without achieving SWR &lt;1.5:1,
    the antenna may be outside the tuner&rsquo;s matching range for this band.
    Check antenna connections and feedline continuity.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>SWR bridge calibration: connect a calibrated 50&Omega; load.  Adjust
    the SWR bridge null trimmer until reflected reading = 0.  Adjust forward
    calibration trimmer until forward reading = applied power.</li>
<li>Position sensor calibration: with each component at minimum position,
    record the ADC reading.  With each at maximum, record the ADC reading.
    Enter these bounds in firmware for linear interpolation to real units
    (pF or &micro;H).</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Tune into a 50&Omega; reference load: SWR must read &lt;1.05:1 (verifies
    SWR bridge calibration).</li>
<li>Tune into a 200&Omega; non-reactive load: auto-tune must achieve
    SWR &lt;1.5:1 within 60 seconds on any HF band.</li>
<li>Band memory: select 40M, tune to best SWR, change to 20M and tune, then
    return to 40M — the tuner must return to the 40M setting within 3 turns.</li>
<li>Motor limit switch test: command full travel in each direction.  All three
    motors must stop at limit switches without stalling.</li>
<li>Log: date, SWR bridge calibration check, position sensor ranges, auto-tune
    test result (band, load, SWR achieved, time), operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — T-Network Matching Range</h2>
<pre>T-network with C1_max = C2_max = 365 pF, L_max = 28 μH:
  At 14 MHz:  matchable impedance range = ~5Ω to ~2000Ω
  At 7 MHz:   matchable impedance range = ~10Ω to ~1000Ω
  At 3.5 MHz: matchable impedance range = ~20Ω to ~500Ω
  (Rough estimates; actual range depends on losses and Q)</pre>

<h2 class="chapter-heading" id="appb">Appendix B — Coordinate Descent Auto-Tune Pseudocode</h2>
<pre>function auto_tune():
  repeat:
    for component in [L, C1, C2]:
      step component in positive direction
      if SWR improved: continue stepping
      else: step in negative direction until SWR stops improving
    if SWR < 1.5 or no_improvement: break
  save_band_settings(band, L_pos, C1_pos, C2_pos)</pre>
""")

# ── TM-GEAR-013  Power Supplies ───────────────────────────────────────────────
TOMS['power_supplies'] = dict(
    tm_num="TM-GEAR-013",
    title="POWER SUPPLIES — LINEAR, SWITCHING, AND FIELD PORTABLE",
    subtitle="Linear Regulated, Switching SMPS, LiFePO4 Pack, Metering and Protection",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers four power supply designs for amateur radio stations:
a 13.8V linear regulated supply (0&ndash;30A), a switching SMPS supply
(13.8V, 20A, compact), a LiFePO4 field portable pack (12.8V, see also
TM-GEAR-004 for BMS details), and a metering and protection module
(voltage, current, RF, over-current protection).</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Linear Regulated Supply</h3>
<p>A transformer (120V/18V, 40VA) followed by a full-wave bridge rectifier
and large filter capacitor (10,000&ndash;20,000&nbsp;&micro;F) provides
unregulated DC.  A series-pass transistor (multiple 2N3055 or IRF540 in
parallel) regulated by an op-amp error amplifier maintains constant output
voltage.  Linear supplies have extremely low ripple and noise (&lt;1&nbsp;mV
RMS) at the cost of lower efficiency (&sim;50%) and heat dissipation.</p>
<h3>2-2 Switching SMPS</h3>
<p>An SMPS converts AC to DC at high frequency (50&ndash;500&nbsp;kHz),
allowing much smaller transformers.  Efficiency: 85&ndash;92%.  The PWM
controller (UC3842 or SG3525) regulates output voltage by adjusting duty
cycle.  EMI filtering is critical for SMPS; they generate conducted and
radiated interference that can degrade receiver performance.</p>
<h3>2-3 Metering and Protection</h3>
<p>A metering module uses a series current shunt (100&nbsp;A / 75&nbsp;mV
shunt) and a voltage divider to display voltage and current on a CYD display.
Over-current protection uses a comparator circuit to trip a power relay if
current exceeds the set limit (adjustable 0&ndash;30&nbsp;A via a trimpot).</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>Linear</th><th>SMPS</th><th>LiFePO4 Portable</th></tr>
<tr><td>Transformer</td><td>120VAC/18V, 40VA</td><td>None (HF stage)</td><td>—</td></tr>
<tr><td>Filter cap</td><td>20,000&nbsp;&micro;F 35V</td><td>220&nbsp;&micro;F 35V (HF)</td><td>—</td></tr>
<tr><td>Series pass</td><td>4&times; 2N3055 on heatsink</td><td>IRF540 full-bridge</td><td>—</td></tr>
<tr><td>Controller</td><td>LM723 + op-amp error amp</td><td>SG3525 or UC3842</td><td>BQ76920 (see TM-GEAR-004)</td></tr>
<tr><td>Output voltage</td><td>Adjustable 12&ndash;15V</td><td>Fixed 13.8V</td><td>12.8V nominal</td></tr>
<tr><td>Output current</td><td>0&ndash;30A</td><td>20A continuous</td><td>50A max (BMS limited)</td></tr>
<tr><td>Ripple</td><td>&lt;1&nbsp;mV RMS</td><td>&lt;50&nbsp;mV RMS</td><td>Battery (no ripple)</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<h3>4-1 Linear Supply Heatsink Sizing</h3>
<p>The series-pass transistors dissipate: P = (V_in &minus; V_out) &times; I_out.
At worst case (13.8V output from 18V rectified): P = (18 &minus; 13.8) &times; 30 = 126W.
With four 2N3055s, each dissipates 31.5W.  2N3055 thermal resistance junction-to-case:
R&Theta;jc = 1.5&deg;C/W.  Required heatsink resistance: R&Theta;sa &le; (T_j_max &minus; T_a) / P &minus; R&Theta;jc
= (150 &minus; 50) / 31.5 &minus; 1.5 = 1.7&deg;C/W per transistor.</p>
<h3>4-2 SMPS EMI Filtering</h3>
<ol>
<li>Install an IEC inlet filter (common-mode choke + X/Y capacitors) on the
    AC input before the SMPS.  This keeps SMPS switching noise off the mains.</li>
<li>Add a ferrite bead choke (2 turns of DC output cable through #31 toroids)
    on the DC output to suppress conducted emissions from reaching the transceiver.</li>
<li>For receivers: use the linear supply; the SMPS may interfere with
    sensitive RX even with filtering.</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<ol>
<li>Linear supply: allow 5 minutes warm-up before setting critical output
    voltage.  Adjust the voltage trimmer to 13.8V (verified with DMM).
    Set current limit to 5A above the transceiver&rsquo;s peak draw.</li>
<li>SMPS: verify output voltage before connecting transceiver.
    Do not exceed rated current; SMPS enter fold-back current limiting and
    may drop voltage abruptly under severe overload.</li>
<li>LiFePO4 portable: check pack voltage before operation.  Below 12.4V
    (SOC &sim;20%), plan for recharging soon.  Do not discharge below 10.0V.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Calibrate voltage: set output with a precision voltmeter (4.5 digits
    or better).  Target: 13.800 &plusmn; 0.050V.</li>
<li>Calibrate current meter: apply a known resistive load and measure load
    current with an external shunt and DMM.  Adjust current meter shunt
    gain in firmware until displayed value matches.</li>
<li>Calibrate over-current trip: increase load until the trip circuit
    activates.  Trip current must be within 0.5A of the set value.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Output voltage: 13.8 &plusmn; 0.1V at no load and full rated load.</li>
<li>Ripple: &lt;10&nbsp;mV peak-to-peak (linear); &lt;100&nbsp;mV pk-pk (SMPS)
    measured with oscilloscope at full load.</li>
<li>Current limit: trips within 0.5A of set value on all supplies.</li>
<li>SMPS EMI: with transceiver in receive mode, key the supply at full load.
    Noise floor must not increase by more than 1 S-unit at any HF frequency.</li>
<li>Log: date, supply type, measured voltage at no load and full load,
    ripple measurement, current limit trip point, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Voltage Drop at Load</h2>
<pre>For a linear supply, the voltage drop across series wiring:
  V_drop = I &times; R_wire
  For #14 AWG, R = 8.4 mΩ/ft (2.76 mΩ/m)
  At 30A over 3m of #14 AWG (6m round trip):
  V_drop = 30 × 0.00276 × 6 = 0.50V
  This means the bench at the end of 3m of #14 AWG gets 13.8 − 0.5 = 13.3V.
  Use #10 AWG for long runs or supply &gt;20A.</pre>

<h2 class="chapter-heading" id="appb">Appendix B — Fuse Sizing</h2>
<table>
<tr><th>Application</th><th>Fuse rating</th><th>Type</th></tr>
<tr><td>Primary AC (30A supply)</td><td>15A slow-blow</td><td>IEC connector fuse</td></tr>
<tr><td>DC output (30A supply)</td><td>40A ATC automotive</td><td>Panel-mount fuse holder</td></tr>
<tr><td>Transceiver branch</td><td>25A ATC</td><td>Within 450mm of supply</td></tr>
<tr><td>LiFePO4 pack main fuse</td><td>60A ANL</td><td>Inline within 450mm of battery</td></tr>
</table>
""")

# ── TM-GEAR-014  Remote Element Switching ─────────────────────────────────────
TOMS['remote-element-switching'] = dict(
    tm_num="TM-GEAR-014",
    title="REMOTE ELEMENT SWITCHING — PIN DIODE AND RELAY SYSTEMS",
    subtitle="VHF/UHF PIN Diode Switching, Bias-T, Relay Sequencer, ESP32 Controller",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers remote switching of antenna parasitic elements (directors
and reflectors) to change the radiation pattern of Yagi-Uda and phased arrays.
Four switching technologies are covered: PIN diode RF switching (70&nbsp;cm
through 23&nbsp;cm), bias-T DC injection (for remote power), RF isolation
filters, and relay sequencer (HF and VHF use).  The ESP32 controller manages
switching sequences, CYD display, and BLE remote control.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 PIN Diode RF Switch</h3>
<p>A PIN (p-intrinsic-n) diode has a wide intrinsic region that stores
minority carriers under forward bias.  Under forward bias (15&ndash;50&nbsp;mA):
the diode becomes a small resistance (&lt;0.5&Omega;), passing RF.  Under
reverse bias (&minus;5&ndash;&minus;20V): the carriers sweep out and the
diode becomes a small capacitor (&lt;0.2&nbsp;pF), blocking RF.</p>
<p>For parasitic element switching: the element is connected to the boom
through the PIN diode.  Forward bias connects the element (director mode);
reverse bias opens the circuit (the element is now a passive wire,
which shortens effective reflector length).</p>
<h3>2-2 Bias-T for Remote Control</h3>
<p>The PIN diode bias current is injected onto the coaxial feedline through
a bias-T at the feed point.  A high-impedance choke (RFC, &gt;1&nbsp;k&Omega;
at operating frequency) in series with the bias supply passes DC but blocks RF.
At the element, the reverse bias circuit is isolated by capacitors in the RF path.</p>
<h3>2-3 Relay Sequencer for TX/RX Switching</h3>
<p>During transmit, the sequencer ensures: (1) preamp is bypassed BEFORE
(2) transmit relay closes BEFORE (3) PA is keyed.  Reverse on receive.
This prevents preamp burnout from TX power leakage.</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>VHF/UHF PIN switch</th><th>Relay sequencer</th></tr>
<tr><td>Switch diode</td><td>MA4P504 or HSMP-3814 (SOT-23)</td><td>—</td></tr>
<tr><td>RF choke</td><td>47&ndash;100 nH SMD (0402)</td><td>1&ndash;10 &micro;H toroid</td></tr>
<tr><td>Bias voltage</td><td>+12V (fwd) / &minus;5V (rev)</td><td>12V relay coil</td></tr>
<tr><td>Isolation capacitor</td><td>100 pF NP0 (0402)</td><td>—</td></tr>
<tr><td>Control transistor</td><td>2N2222 or BC817</td><td>2N2222 + relay</td></tr>
<tr><td>Controller</td><td>ESP32 WROOM-32</td><td>ESP32 WROOM-32</td></tr>
<tr><td>Relay (sequencer)</td><td>—</td><td>Omron G2RL-1, 12V</td></tr>
<tr><td>Sequencer delay</td><td>—</td><td>10&ndash;50 ms between steps</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<h3>4-1 PIN Diode Element Switch (70&nbsp;cm)</h3>
<ol>
<li>Mount MA4P504 diode on a small PCB (10&times;10&nbsp;mm) at the
    element attachment point on the boom.  Orient cathode toward the
    bias supply, anode toward the element.</li>
<li>Wire the RF choke (47&nbsp;nH SMD) in series between the bias supply
    conductor and the cathode.  This prevents RF from entering the bias circuit.</li>
<li>Connect 100&nbsp;pF NP0 capacitor in series with the RF path (between
    boom and element) to block DC while passing RF.</li>
<li>Run a twisted pair (bias supply and return) along the boom to the
    controller box.  Use shielded twisted pair for runs &gt;300&nbsp;mm.</li>
</ol>
<h3>4-2 Relay Sequencer</h3>
<ol>
<li>Connect Relay 1 (preamp bypass) to ESP32 GPIO with a 2N2222 driver.
    Relay 2 (TX/RX antenna relay) to a second GPIO + driver.</li>
<li>On PTT input (low = key): fire Relay 1 (bypass preamp) after 10&nbsp;ms
    delay; fire Relay 2 (TX relay) after another 20&nbsp;ms delay.
    Then assert PA key output.</li>
<li>On PTT release: de-assert PA key; wait 20&nbsp;ms; release Relay 2;
    wait 10&nbsp;ms; release Relay 1 (reconnect preamp).</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<ol>
<li>Select antenna pattern via CYD touchscreen or BLE remote.  The ESP32
    sets the correct PIN diode bias states for the chosen pattern (cardioid,
    broadside, endfire, or omnidirectional).</li>
<li>Pattern switching may occur on receive only; switching during transmit
    is not recommended (momentary impedance mismatch during transition).</li>
<li>Inspect all bias-T connections at antenna installation and after any
    storm.  Water ingress at a bias-T can short the bias supply, permanently
    forward-biasing one PIN diode and locking the pattern.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Verify PIN diode states: with a NanoVNA and TinySA, measure S21 through
    each switched element.  Forward-biased: S21 must be &lt;&minus;0.5&nbsp;dB
    (low loss connected state).  Reverse-biased: S21 must be &lt;&minus;35&nbsp;dB
    (high isolation open state).</li>
<li>Sequencer timing: measure delay between PTT closure and TX relay closure
    with an oscilloscope.  Must be &ge;20&nbsp;ms to protect the preamp.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>PIN diode insertion loss (forward bias) &lt;0.5&nbsp;dB at operating frequency.</li>
<li>PIN diode isolation (reverse bias) &ge;35&nbsp;dB at operating frequency.</li>
<li>Pattern repeatability: switching to the same pattern twice should give
    the same S21 phase through each element within &plusmn;5&deg;.</li>
<li>Sequencer: TX relay must not close before preamp bypass relay; verified
    with dual-trace oscilloscope measuring each relay coil drive.</li>
<li>Log: date, diode types, forward loss, reverse isolation, sequencer
    timing, pattern repeat error, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — PIN Diode Bias Current vs. Loss</h2>
<table>
<tr><th>Forward bias current</th><th>Diode resistance (MA4P504)</th><th>Insertion loss (70cm)</th></tr>
<tr><td>5 mA</td><td>2.0 &Omega;</td><td>0.7 dB</td></tr>
<tr><td>15 mA</td><td>0.7 &Omega;</td><td>0.5 dB</td></tr>
<tr><td>30 mA</td><td>0.3 &Omega;</td><td>0.4 dB</td></tr>
<tr><td>50 mA</td><td>0.15 &Omega;</td><td>0.35 dB</td></tr>
</table>

<h2 class="chapter-heading" id="appb">Appendix B — TX/RX Sequencer State Table</h2>
<table>
<tr><th>Time</th><th>PTT</th><th>Preamp bypass relay</th><th>TX relay</th><th>PA key</th></tr>
<tr><td>t=0</td><td>ASSERTED</td><td>OFF (preamp active)</td><td>OFF (RX path)</td><td>OFF</td></tr>
<tr><td>t=10ms</td><td>ASSERTED</td><td>ON (preamp bypassed)</td><td>OFF</td><td>OFF</td></tr>
<tr><td>t=30ms</td><td>ASSERTED</td><td>ON</td><td>ON (TX path)</td><td>OFF</td></tr>
<tr><td>t=40ms</td><td>ASSERTED</td><td>ON</td><td>ON</td><td>ON (transmitting)</td></tr>
<tr><td>t=release</td><td>RELEASED</td><td>ON</td><td>ON</td><td>OFF</td></tr>
<tr><td>t+20ms</td><td>RELEASED</td><td>ON</td><td>OFF (RX path)</td><td>OFF</td></tr>
<tr><td>t+30ms</td><td>RELEASED</td><td>OFF (preamp active)</td><td>OFF</td><td>OFF</td></tr>
</table>
""")

# ── TM-GEAR-015  RF Couplers ──────────────────────────────────────────────────
TOMS['rf_couplers'] = dict(
    tm_num="TM-GEAR-015",
    title="RF DIRECTIONAL COUPLERS — CONSTRUCTION AND CALIBRATION",
    subtitle="Toroidal Bruene Coupler, Transmission-Line Coupler, Resistive Bridge",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers three directional coupler designs for sampling forward
and reflected RF power: the toroidal Bruene coupler (HF 1.8&ndash;30&nbsp;MHz),
the transmission-line directional coupler (1&ndash;600&nbsp;MHz), and the
resistive bridge coupler (wideband, any frequency, low power only).
All produce separate forward (VFWD) and reflected (VREF) DC outputs that
drive SWR meters, power meters, or an ADC.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Bruene Toroidal Coupler</h3>
<p>A ferrite toroid transformer (T1) samples the current in the transmission
line (1-turn primary = center conductor).  A separate voltage sampling
network monitors the line voltage.  Combining current and voltage in the
correct phase relationship produces the forward and reflected wave voltages
separately.  Directivity is the ratio of the response to the desired
direction vs. the undesired: &ge;40&nbsp;dB with high-precision resistors.</p>
<h3>2-2 Transmission-Line Coupler</h3>
<p>Two parallel transmission lines run close together for &lambda;/4 at
the design frequency.  Electromagnetic coupling transfers a small fraction
of power to the secondary line.  Coupling factor C (&minus;dB) depends on
line spacing and length: C = &minus;20 log10(k) where k is the voltage
coupling coefficient.  Typical: &minus;20 to &minus;30&nbsp;dB.  Used in
the SWR meter and RF power meter designs.</p>
<h3>2-3 Resistive Bridge Coupler</h3>
<p>A 4-resistor Wheatstone bridge measures the complex reflection coefficient
directly.  One arm is the antenna under test; the opposite arm is the
reference (50&Omega;).  At balance (antenna = 50&Omega;), the bridge output
is zero.  Imbalance is proportional to (Z_ant &minus; 50) / (Z_ant + 50) = &Gamma;.
Limited to low power (&lt;1W); resistors must be non-inductive and matched.</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>Bruene Toroidal</th><th>T-Line</th><th>Resistive Bridge</th></tr>
<tr><td>Coupler core</td><td>FT-50-43 or BN-43-202</td><td>PCB microstrip</td><td>None (resistors only)</td></tr>
<tr><td>Secondary turns</td><td>20 turns #28 AWG</td><td>PCB coupled line</td><td>—</td></tr>
<tr><td>Termination</td><td>51&Omega; 1% each end</td><td>51&Omega; 0402 SMD</td><td>51&Omega; 1% (×4)</td></tr>
<tr><td>Detector diodes</td><td>1N5711 (×2)</td><td>1N5711 (×2)</td><td>1N5711 (×2)</td></tr>
<tr><td>Filter capacitors</td><td>10 nF + 10 &micro;F</td><td>100 pF + 10 nF</td><td>10 nF + 10 &micro;F</td></tr>
<tr><td>Connectors</td><td>SO-239 (×2) + SMA (×2 for FWD/REF)</td><td>SMA (×4)</td><td>SMA (×3)</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<h3>4-1 Bruene Coupler Winding</h3>
<ol>
<li>Wind 20 turns of #28 AWG Teflon wire on a BN-43-202 core.  This is the
    voltage sampling secondary; the center conductor of the transmission line
    threads through the core once as the 1-turn primary.</li>
<li>Connect resistors R1 and R2 (51&Omega; 1%) from each end of the secondary
    to the common center conductor (ground bus).  The midpoints of R1 and R2
    are the detector nodes (VFWD and VREF before the diodes).</li>
<li>Mount the detector diodes (1N5711 cathode toward the detector node)
    and filter capacitors on a small PCB within 20&nbsp;mm of the toroid.</li>
<li>Thread the SO-239 center conductor through the toroid.  Keep the center
    conductor straight through the toroid bore (not coiled) to maintain
    transmission line continuity.</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<p>Directional couplers are passive devices; no operational procedure is
required beyond installation.  Install the coupler inline between the
transmitter and the antenna (or load under test).  Connect VFWD and VREF
outputs to the meter or ADC inputs.</p>
<ul>
<li>Never exceed the power rating of the coupler (determined by the
    center conductor rating and connector type).</li>
<li>Do not operate with the VFWD or VREF ports open (floating) — load them
    with &ge;50&nbsp;k&Omega; (meter input) to prevent undefined voltage states.</li>
</ul>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Connect a calibrated 50&Omega; load to the output port.  Apply 10W at
    14.175&nbsp;MHz.  VFWD must read a consistent voltage; VREF must read
    &lt;1% of VFWD (indicating &lt;&minus;40&nbsp;dB directivity).</li>
<li>Connect a short circuit to the output port.  VREF must equal VFWD
    (SWR = &infin;; 100% reflection).  Directivity check: VFWD at short circuit
    should match VFWD at 50&Omega; load (forward response should not change
    with load — it measures only forward power).</li>
<li>Measure directivity: directivity (dB) = 20 log10(VFWD_50 / VREF_50).
    Target: &ge;30&nbsp;dB.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Directivity &ge;30&nbsp;dB at all operating frequencies.</li>
<li>Insertion loss &lt;0.2&nbsp;dB (measure S21 through the coupler with
    NanoVNA).</li>
<li>SWR at input port &lt;1.05:1 with a 50&Omega; load on the output
    (the coupler must not introduce a mismatch).</li>
<li>VFWD response: flat within &plusmn;1&nbsp;dB across the specified frequency
    range (the coupler must sample equally at all frequencies).</li>
<li>Log: date, coupler type, directivity at 7 and 14&nbsp;MHz, insertion
    loss, input SWR, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Coupling Factor vs. Secondary Turns</h2>
<pre>Bruene toroidal coupler, BN-43-202:
  Coupling factor C (dB) = -20 log10(N2)
  N2 = 10 turns: C = -20 dB
  N2 = 20 turns: C = -26 dB  (standard design)
  N2 = 30 turns: C = -30 dB

More turns = weaker coupling = smaller VFWD/VREF output voltage.
For a 1W signal into 50Ω (V_line = sqrt(50) = 7.07V pk):
  At N2=20: V_coupled = 7.07 / 20 = 0.35V pk → after diode: ~0.15V DC</pre>

<h2 class="chapter-heading" id="appb">Appendix B — Return Loss and SWR Conversions</h2>
<table>
<tr><th>SWR</th><th>Return loss (dB)</th><th>|&Gamma;|</th><th>% reflected</th></tr>
<tr><td>1.1:1</td><td>26.4</td><td>0.048</td><td>0.2%</td></tr>
<tr><td>1.5:1</td><td>14.0</td><td>0.200</td><td>4%</td></tr>
<tr><td>2.0:1</td><td>9.5</td><td>0.333</td><td>11%</td></tr>
<tr><td>3.0:1</td><td>6.0</td><td>0.500</td><td>25%</td></tr>
</table>
""")

# ── TM-GEAR-016  RFI Mitigation ───────────────────────────────────────────────
TOMS['rfi_mitigation'] = dict(
    tm_num="TM-GEAR-016",
    title="RFI MITIGATION — IDENTIFICATION AND SUPPRESSION",
    subtitle="Active Noise Canceller, CMC, Sniffer Probe, ESP32 Noise Monitor",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers RFI (radio frequency interference) mitigation for
amateur stations: active noise cancelling (phased auxiliary antenna nulling),
common-mode chokes (see also TM-GEAR-007), a near-field RFI sniffer probe,
and an ESP32-based noise monitor with CYD waterfall display for continuous
noise floor tracking.  The active noise canceller can suppress local noise
sources by 20&ndash;40&nbsp;dB, dramatically improving weak-signal copy.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Active Noise Canceller Principle</h3>
<p>A reference (auxiliary) antenna is aimed at the local noise source.
Its output (mostly noise) is phase-shifted and amplitude-adjusted to match
the noise component in the main antenna signal.  When subtracted from the
main signal, the noise cancels while the desired signal (from a distant
direction) is preserved:</p>
<pre>V_main = V_signal + V_noise_main
V_ref  = V_noise_ref  (negligible V_signal in reference direction)
V_out  = V_main &minus; A &times; e^(j&phi;) &times; V_ref &asymp; V_signal</pre>
<p>The phase (&phi;) and amplitude (A) are adjusted manually or automatically
until the noise is minimized.  Practical cancellation: 20&ndash;40&nbsp;dB,
limited by noise source geometry and antenna placement.</p>
<h3>2-2 Near-Field Sniffer Probe</h3>
<p>A small shielded loop (30&ndash;50&nbsp;mm diameter) coupled to a
SMA connector is held near suspected RFI sources.  The loop responds to
the magnetic component of the near-field.  Connected to an SDR or TinySA,
the probe identifies the source and its harmonic spectrum, enabling targeted
mitigation (ferrite beads, cable rerouting, shielding).</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>Noise Canceller</th><th>Sniffer Probe</th><th>Noise Monitor</th></tr>
<tr><td>Reference antenna</td><td>1&ndash;3m whip or small loop</td><td>30mm shielded loop</td><td>External antenna</td></tr>
<tr><td>RF preamp</td><td>ERA-3SM MMIC (+20 dB)</td><td>Optional ERA-3SM</td><td>SPF5189Z (+19 dB)</td></tr>
<tr><td>Phase shifter</td><td>0&ndash;360&deg; varactor or lumped LC</td><td>—</td><td>—</td></tr>
<tr><td>Attenuator</td><td>PE4302 6-bit step atten</td><td>—</td><td>PE4302 (optional)</td></tr>
<tr><td>Combiner</td><td>Op-amp subtractor (TL072)</td><td>—</td><td>—</td></tr>
<tr><td>SDR/receiver</td><td>External (TRX or RTL-SDR)</td><td>TinySA or RTL-SDR</td><td>RTL-SDR + ESP32-S3</td></tr>
<tr><td>Controller</td><td>ESP32 WROOM-32</td><td>—</td><td>ESP32-S3</td></tr>
<tr><td>Display</td><td>CYD 2.8&rdquo;</td><td>TinySA display</td><td>CYD waterfall</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<h3>4-1 Active Noise Canceller</h3>
<ol>
<li>Build the ERA-3SM preamplifier: MMIC in SOT-143 package, bias via 130&Omega;
    resistor from +9V, input/output 100&nbsp;pF DC-block caps.</li>
<li>Build the phase shifter: an all-pass filter network using a varactor
    diode (BB515 or SMV1248) whose capacitance is set by a 0&ndash;10V
    control voltage.  This provides 0&ndash;360&deg; phase range across
    the HF band.</li>
<li>Build the subtractor: TL072 dual op-amp configured as a differential
    amplifier.  Gain set by 10&nbsp;k&Omega; / 10&nbsp;k&Omega; resistors (gain = 1).
    Apply main signal to non-inverting input; reference signal (after
    phase shift and attenuation) to inverting input.</li>
</ol>
<h3>4-2 Near-Field Sniffer Probe</h3>
<ol>
<li>Wind a 3-turn loop of RG-174 coax, 35&nbsp;mm diameter.  Connect
    the shield at both ends to the outer conductor of the SMA connector;
    connect the center conductor at one end only to the SMA center pin.
    (The shield gap should be at the midpoint of the loop to form a
    Faraday-shielded loop; solder a jumper to close the shield everywhere
    except the deliberate gap.)</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<h3>5-1 Noise Canceller Adjustment</h3>
<ol>
<li>Position the reference antenna to maximize noise pickup relative to signal:
    point it at the noise source (if known) or orient it to maximize S-meter
    noise level.</li>
<li>Enable the canceller.  Adjust phase (&phi;) slowly through 0&ndash;360&deg;
    while monitoring the noise level (S-meter or audio).  A strong null
    will appear at the correct phase setting.</li>
<li>At the null, adjust amplitude (attenuation) for the deepest null.
    Alternate between phase and amplitude adjustments; a few iterations
    converge to the optimal setting.</li>
<li>Save settings for later use.  Note: the optimal settings change if
    the noise source moves or changes character.  Re-adjust if noise
    returns.</li>
</ol>
<h3>5-2 RFI Source Location with Sniffer Probe</h3>
<ol>
<li>Connect sniffer probe to TinySA.  Set span to 1&ndash;30&nbsp;MHz with
    10&nbsp;kHz RBW.  Look for peaks that correlate with the interference.</li>
<li>Move probe near suspected sources (switching power supplies, LED drivers,
    computer power bricks, solar charge controllers) until the signal peaks.</li>
<li>Identify the interference frequency and its harmonics.  If the fundamental
    is a known utility frequency (switching PSU at 65&nbsp;kHz, LED driver at
    120&nbsp;Hz), that identifies the source.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Noise canceller: verify that the reference signal is at least 10&nbsp;dB
    above the main antenna noise level.  If the reference is too weak,
    the canceller cannot achieve a deep null.  Add preamp gain or move
    the reference antenna closer to the noise source.</li>
<li>Noise monitor: calibrate the noise floor baseline by running 24 hours
    of background measurements at night when local noise is minimal.
    Save the baseline to SD card; any future increase indicates a new
    noise source.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Noise canceller: achieves at least 15&nbsp;dB noise reduction on a
    known local interference source (verified by S-meter before and after).</li>
<li>Sniffer probe: detects a 5&nbsp;mW 14&nbsp;MHz test signal from a
    1&nbsp;cm loop placed 50&nbsp;mm from the probe (sensitivity check).</li>
<li>Noise monitor: generates a waterfall display with no dropouts for
    24 hours of continuous operation.</li>
<li>Log: date, noise source identified and mitigated, dB reduction achieved,
    mitigation method (CMC, canceller, shielding), operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Common RFI Sources</h2>
<table>
<tr><th>Source</th><th>Typical frequency</th><th>Mitigation</th></tr>
<tr><td>Switching power supply</td><td>50&ndash;500 kHz + harmonics</td><td>CMC on AC cord; ferrite on DC leads</td></tr>
<tr><td>LED driver</td><td>100&ndash;500 kHz + harmonics</td><td>CMC; replace with linear driver</td></tr>
<tr><td>Solar MPPT charger</td><td>50&ndash;200 kHz</td><td>CMC on battery leads; shielded enclosure</td></tr>
<tr><td>Plasma TV</td><td>Broadband HF + VHF</td><td>CMC on all cable TV/antenna leads</td></tr>
<tr><td>RF from own station</td><td>Operating frequency</td><td>Common-mode chokes at feedpoint</td></tr>
</table>

<h2 class="chapter-heading" id="appb">Appendix B — CMC Placement Priority Order</h2>
<ol>
<li>Antenna feedpoint (always first)</li>
<li>Shack entry panel (coax entry)</li>
<li>All AC power cords within the shack</li>
<li>USB and serial cables to peripherals</li>
<li>Audio cables between transceiver and computer</li>
</ol>
""")

# ── TM-GEAR-017  Roller Inductors ─────────────────────────────────────────────
TOMS['roller_inductors'] = dict(
    tm_num="TM-GEAR-017",
    title="ROLLER INDUCTORS — MOTORIZED VARIABLE INDUCTANCE",
    subtitle="ESP32 + DRV8825 Controller, Limit Switches, Encoder Position, OLED Display",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers the motorized roller inductor controller for use in
antenna tuners (transmatches), transmatch remote positioning, and inductor
matching networks.  The controller drives a DRV8825 or A4988 stepper motor
driver to position a sliding contact along a large air-core inductor coil,
providing continuously variable inductance from 0 to the maximum value
(typically 20&ndash;30&nbsp;&micro;H for HF antenna tuner use).  Position
is tracked by encoder and limit switches; a front panel encoder + OLED
provides manual control.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Variable Inductance</h3>
<p>A roller inductor consists of a large single-layer air-core coil wound
on a threaded ceramic or PTFE former.  A sliding contact (the &ldquo;roller&rdquo;)
driven along the former by a threaded rod taps a variable number of turns.
Inductance is approximately proportional to turns squared (Wheeler&rsquo;s formula)
for a fixed coil geometry:</p>
<pre>L = r&sup2; &times; N&sup2; / (9r + 10l)   (L in &micro;H, r and l in inches)
For a fixed coil with N_total turns:
  L(n) &asymp; L_max &times; (n / N_total)&sup2;</pre>
<p>A 25&nbsp;&micro;H roller inductor with 40 turns has L = 0 at n=0, rising
to 25&nbsp;&micro;H at n=40.  The relationship is not exactly quadratic
due to coil end effects, so a calibration table maps encoder position to
actual inductance (measured with an LCR meter).</p>
<h3>2-2 Stepper Motor Positioning</h3>
<p>A NEMA&nbsp;17 stepper with 200 steps/rev, 1/16 microstepping = 3200 steps/rev.
The lead screw pitch (typically 1.25&nbsp;mm/rev for M8) gives a linear
resolution of 1.25 / 3200 = 0.391&nbsp;&micro;m per step — far more than
needed.  Position is tracked by step count from the home position (home limit
switch).  On power-up, the controller homes before accepting position commands.</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>Value / Part</th><th>Purpose</th></tr>
<tr><td>Roller inductor body</td><td>Commercial 20&ndash;30 &micro;H (Cardwell, Barker&amp;Williamson)</td><td>Variable inductance element</td></tr>
<tr><td>Drive motor</td><td>NEMA 17, 1.5A, 200 steps/rev</td><td>Rotate roller screw</td></tr>
<tr><td>Motor driver</td><td>DRV8825 or TMC2208 module</td><td>Microstepping drive</td></tr>
<tr><td>Controller</td><td>ESP32 WROOM-32</td><td>Step generation, position tracking</td></tr>
<tr><td>Limit switches</td><td>2&times; microswitches (home and end)</td><td>Hard stops + home reference</td></tr>
<tr><td>Position encoder</td><td>Optical encoder OR step counting</td><td>Position feedback</td></tr>
<tr><td>Display</td><td>SSD1306 0.96&rdquo; OLED</td><td>Inductance, position, status</td></tr>
<tr><td>Manual encoder</td><td>KY-040 rotary encoder</td><td>Manual position control</td></tr>
<tr><td>Power</td><td>12V 2A</td><td>Motor VMO T + 3.3V for ESP32</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction and Setup</h2>
<ol>
<li>Mount the roller inductor securely; the roller screw must rotate freely
    without binding.  Any binding causes lost steps, which corrupts the
    position tracking.</li>
<li>Couple the NEMA&nbsp;17 shaft to the roller inductor shaft via a flexible
    coupler or 3D-printed coupling gear.  Ensure axial alignment within 1&nbsp;mm.</li>
<li>Mount microswitches at the home (minimum inductance) and end (maximum
    inductance) positions.  Wire both normally-closed (NC) for fail-safe
    operation — a broken wire trips the limit rather than ignoring it.</li>
<li>Wire the DRV8825: VMOT to 12V; EN, STEP, DIR to ESP32 GPIO; M0/M1/M2
    for 1/16 step mode (all high); SLEEP high (always enabled).</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<h3>5-1 Homing Sequence</h3>
<ol>
<li>On power-up, the controller automatically homes: moves toward the home
    limit switch at 100 steps/sec until the switch opens; then backs off
    at 50 steps/sec until the switch closes again.  This position is zero.</li>
<li>After homing, move to the last-used position (stored in NVS).</li>
</ol>
<h3>5-2 Setting Inductance</h3>
<ol>
<li>Enter the desired inductance in &micro;H on the CYD or OLED display.
    The ESP32 looks up the corresponding step count in the calibration table
    and commands the motor.</li>
<li>For SWR-guided auto-tuning: the tuner controller (TM-GEAR-012) sends
    the target inductance via I2C or UART to the roller inductor controller.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Set the roller to each of 10 equally-spaced positions (0, 10, 20, ..., 100%
    of full travel).  Measure inductance at each position with an LCR meter
    at 100&nbsp;kHz.  Record the (step_count, L_uH) pairs as the calibration table.</li>
<li>Store the calibration table in ESP32 NVS.  The firmware uses linear
    interpolation between table points for positions between calibration points.</li>
<li>Calibration must be repeated if the motor coupler or limit switch position
    is changed.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Position repeatability: command 10&nbsp;&micro;H 10 times from random positions.
    Measured inductance must be within &plusmn;0.5&nbsp;&micro;H of target each time.</li>
<li>Limit switch test: command travel beyond the end-of-travel limit.  Motor
    must stop at the limit switch; verify no lost steps after the stop.</li>
<li>Home repeatability: home 5 times.  After each home, command 10&nbsp;&micro;H;
    measure inductance.  Must be within &plusmn;0.2&nbsp;&micro;H each run.</li>
<li>Log: date, inductor model, calibration table (10-point), home repeatability,
    position repeatability at 10 &micro;H and 20 &micro;H, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Common Roller Inductor Specifications</h2>
<table>
<tr><th>Model</th><th>Max L (&micro;H)</th><th>Turns</th><th>Current rating (A)</th></tr>
<tr><td>Cardwell 154-33</td><td>33</td><td>45</td><td>15</td></tr>
<tr><td>B&amp;W 850</td><td>25</td><td>40</td><td>20</td></tr>
<tr><td>Barker Williamson 851</td><td>50</td><td>56</td><td>20</td></tr>
<tr><td>Homebrew (see TM-GEAR-006)</td><td>Variable</td><td>Variable</td><td>Depends on wire gauge</td></tr>
</table>

<h2 class="chapter-heading" id="appb">Appendix B — Inductance Calibration Worksheet</h2>
<table>
<tr><th>Step count</th><th>% travel</th><th>L measured (&micro;H)</th></tr>
<tr><td>0</td><td>0%</td><td>___</td></tr>
<tr><td>320</td><td>10%</td><td>___</td></tr>
<tr><td>640</td><td>20%</td><td>___</td></tr>
<tr><td>960</td><td>30%</td><td>___</td></tr>
<tr><td>1280</td><td>40%</td><td>___</td></tr>
<tr><td>1600</td><td>50%</td><td>___</td></tr>
<tr><td>1920</td><td>60%</td><td>___</td></tr>
<tr><td>2240</td><td>70%</td><td>___</td></tr>
<tr><td>2560</td><td>80%</td><td>___</td></tr>
<tr><td>2880</td><td>90%</td><td>___</td></tr>
<tr><td>3200</td><td>100%</td><td>___</td></tr>
</table>
""")

# ── TM-GEAR-018  Transmatch ───────────────────────────────────────────────────
TOMS['transmatch'] = dict(
    tm_num="TM-GEAR-018",
    title="TRANSMATCH — PORTABLE ANTENNA TUNER",
    subtitle="HF T-Network (160M–10M), VHF/UHF L-Network (6M–20CM), 100W/50W",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers a compact portable transmatch (antenna tuner) for all
fifteen amateur radio bands from 160 meters through 20 centimeters.  Two
sections are provided: a balanced T-network for HF (1.8&ndash;30&nbsp;MHz,
100W continuous) and an L-network or stub tuner for VHF/UHF (50&ndash;1300&nbsp;MHz,
50W).  The transmatch transforms antenna impedances of approximately 5&ndash;2000&Omega;
to the 50&Omega; transceiver impedance, enabling operation with non-resonant
or slightly mistuned antennas.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 T-Network (HF)</h3>
<p>The T-network (input capacitor C1, series inductor L, output capacitor C2)
is the most common antenna tuner topology.  Both capacitors in shunt (to ground)
with an inductor in series create a low-pass filter whose cutoff and impedance
transformation ratio are controlled by the component values.  The T-network
can match any impedance in its range, though it introduces some loss at high
transformation ratios.  Loss increases at extremes: matching 2000&Omega; to 50&Omega;
introduces 1&ndash;2&nbsp;dB loss; matching near 50&Omega; introduces &lt;0.1&nbsp;dB.</p>
<h3>2-2 L-Network (VHF/UHF)</h3>
<p>An L-network (one shunt reactive element + one series reactive element)
provides a two-element matching solution.  It can match either upward or
downward impedances depending on component arrangement.  For VHF/UHF,
lumped LC components are replaced by transmission-line stub sections
(open or short-circuit stubs) for lower loss and higher power handling.</p>
<h3>2-3 SWR Indicator</h3>
<p>The built-in SWR indicator uses a directional coupler (see TM-GEAR-015)
with LED or analog meter to indicate SWR.  Tuning goal: minimum SWR
(minimum reflected indicator deflection).</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>HF Section</th><th>VHF/UHF Section</th></tr>
<tr><td>C1 (input)</td><td>365 pF air variable, 2500V</td><td>L-network cap, NP0 (switchable)</td></tr>
<tr><td>C2 (output)</td><td>365 pF air variable, 2500V</td><td>Same</td></tr>
<tr><td>Inductor L</td><td>Roller inductor, 0&ndash;28 &micro;H</td><td>Fixed coax stubs (switchable)</td></tr>
<tr><td>Band switch</td><td>DPDT rotary, 6-position</td><td>2-position HF/VHF toggle</td></tr>
<tr><td>SWR indicator</td><td>Toroid coupler + dual LEDs</td><td>Shared with HF</td></tr>
<tr><td>Connectors</td><td>SO-239 (in), SO-239 or balanced (out)</td><td>N-type or SO-239</td></tr>
<tr><td>Enclosure</td><td>200&times;150&times;80 mm aluminum</td><td>Same enclosure, rear panel</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<ol>
<li>Mount variable capacitors C1 and C2 on the front panel with lockwashers.
    Keep capacitor leads to the inductor and SWR bridge as short as possible
    (&lt;50&nbsp;mm) to minimize stray inductance and radiation.</li>
<li>Mount the roller inductor vertically on the center of the main board.
    The inductor lead to C1 and C2 should be heavy-gauge solid wire (#12 AWG
    or larger) capable of 100W continuous RF current.</li>
<li>Wire the balun output option: a 4:1 Guanella balun (see TM-GEAR-003)
    mounted on the rear panel provides a balanced output for dipoles and
    open-wire feedlines.</li>
<li>Install the SWR bridge (from TM-GEAR-015) inline between the radio
    input SO-239 and the T-network C1.  Connect VFWD to forward LED driver;
    VREF to reflected LED driver.</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<h3>5-1 Initial Tuning on a New Band</h3>
<ol>
<li>Set C1 and C2 to midscale.  Set L to minimum (roller at bottom).</li>
<li>Key the transmitter at 5&nbsp;W (reduced power protects transistors during
    tuning on a badly mismatched antenna).</li>
<li>Watch the SWR indicator.  Adjust L for minimum reflected LED; then
    adjust C1 and C2 alternately for minimum reflected, returning to L
    adjustment if a new minimum appears.</li>
<li>Iterate until reflected indicator is at minimum and forward indicator
    shows full power.  Typical tuning time: 30&ndash;60 seconds.</li>
<li>Increase to full operating power.  Verify SWR indicator does not change.</li>
</ol>
<h3>5-2 Band-to-Band Switching</h3>
<ol>
<li>Record successful L, C1, C2 positions for each band on the band card
    (laminated chart in the lid of the tuner).</li>
<li>On band change: set L, C1, C2 to the recorded positions; tune briefly
    at 5W to confirm the antenna has not changed.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Verify SWR indicator zero: connect 50&Omega; load to the antenna port.
    Transmit 10W.  Reflected LED must not illuminate.  Adjust SWR bridge
    null trimmer if reflected shows any indication.</li>
<li>Verify SWR indicator response: connect a 100&Omega; load (SWR 2:1).
    Reflected LED must illuminate at approximately 30% of the forward LED
    intensity (indicating &sim;2:1 SWR).</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Matches a 200&Omega; non-reactive load to SWR &lt;1.5:1 on all HF bands.</li>
<li>Matches a 450&Omega; random wire (via 9:1 un-un) to SWR &lt;2:1 on
    all HF bands.</li>
<li>Insertion loss with 50&Omega; load on both ports: &lt;0.2&nbsp;dB at 14&nbsp;MHz.</li>
<li>VHF section tunes 2M vertical to SWR &lt;1.5:1 from a 75&Omega; feedpoint.</li>
<li>All controls rotate smoothly over full range with no mechanical binding.</li>
<li>Log: date, match test (200&Omega; at 7 and 14&nbsp;MHz, SWR achieved),
    insertion loss check, VHF section test, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — T-Network Component Value Guidelines</h2>
<table>
<tr><th>Band</th><th>Typical C1, C2 range (pF)</th><th>Typical L range (&micro;H)</th></tr>
<tr><td>160M (1.8 MHz)</td><td>200&ndash;365 pF</td><td>15&ndash;28 &micro;H</td></tr>
<tr><td>80M (3.75 MHz)</td><td>100&ndash;300 pF</td><td>8&ndash;20 &micro;H</td></tr>
<tr><td>40M (7.15 MHz)</td><td>50&ndash;200 pF</td><td>4&ndash;12 &micro;H</td></tr>
<tr><td>20M (14.175 MHz)</td><td>20&ndash;100 pF</td><td>2&ndash;6 &micro;H</td></tr>
<tr><td>10M (28.5 MHz)</td><td>10&ndash;50 pF</td><td>0.5&ndash;3 &micro;H</td></tr>
</table>

<h2 class="chapter-heading" id="appb">Appendix B — Matching Range and Loss Chart</h2>
<table>
<tr><th>Antenna impedance</th><th>T-network loss (typical)</th><th>Notes</th></tr>
<tr><td>25&ndash;100 &Omega;</td><td>&lt;0.3 dB</td><td>Excellent match; low loss</td></tr>
<tr><td>100&ndash;300 &Omega;</td><td>0.3&ndash;0.7 dB</td><td>Good match</td></tr>
<tr><td>300&ndash;1000 &Omega;</td><td>0.7&ndash;1.5 dB</td><td>Acceptable; use 4:1 balun if possible</td></tr>
<tr><td>&gt;1000 &Omega;</td><td>1.5&ndash;3 dB</td><td>Marginal; consider 9:1 un-un</td></tr>
</table>
""")

# ── TM-GEAR-019  Variable Caps ────────────────────────────────────────────────
TOMS['variable_caps'] = dict(
    tm_num="TM-GEAR-019",
    title="VARIABLE CAPACITORS — AIR, MOTORIZED, AND STACK-ON FLAT-PLATE",
    subtitle="Air-Variable, Motorized ESP32 Controller, Homebrew Stack-On Units",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers variable capacitor designs for antenna tuners, transmatch
circuits, and magnetic loop antenna tuning: commercial air-variable capacitors
(motorized), homebrew stack-on flat-plate capacitors (DIY from copper foil and
polymer sheet), and a modular controller (ESP32 + encoder + OLED) for motorizing
any shaft-driven variable capacitor.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Air-Variable Capacitor</h3>
<p>A parallel-plate capacitor with interleaved rotor and stator plates (no
dielectric between the plates).  Capacitance is proportional to the overlap
area between rotor and stator plates: C = &epsilon;<sub>0</sub> &times; A / d,
where A is the overlap area and d is the plate spacing (typically 1.5&ndash;3&nbsp;mm
for HF/VHF use).  As the rotor turns, the overlap area changes continuously
from zero (minimum capacitance, plates disengaged) to maximum (full overlap).</p>
<h3>2-2 Voltage Breakdown</h3>
<p>Air breakdown occurs at approximately 30&nbsp;kV/cm (3&nbsp;MV/m).  For a
3&nbsp;mm plate spacing: V_breakdown = 3000V &times; 0.3&nbsp;cm = 900V peak.
At 100W into 50&Omega;: V_peak = &radic;(2 &times; 100 &times; 50) = 100V peak &mdash;
well within rating.  In a transmatch at high transformation ratio, voltages
across the capacitors can reach 1&ndash;3&nbsp;kV.  Use the capacitor voltage
rating appropriate for the transmatch power and impedance.</p>
<h3>2-3 Homebrew Stack-On Flat-Plate Capacitor</h3>
<p>Copper foil plates separated by a thin polymer dielectric (polyimide, LDPE,
or polypropylene film).  Stacking multiple units in parallel increases total
capacitance.  Stacking in series reduces total capacitance and increases
voltage rating.  Formula:</p>
<pre>C = &epsilon;0 &times; &epsilon;r &times; A / d    [Farads]
  &epsilon;0 = 8.854&times;10&minus;12 F/m
  &epsilon;r = relative permittivity (LDPE: 2.3; polypropylene: 2.2; polyimide: 3.5)
  A = plate area (m&sup2;); d = dielectric thickness (m)</pre>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>Air-variable (motorized)</th><th>Stack-on flat-plate</th></tr>
<tr><td>Capacitor body</td><td>Commercial air variable (365 pF, 500V)</td><td>50&times;75 mm copper foil sheets</td></tr>
<tr><td>Dielectric</td><td>Air (3mm gap)</td><td>0.05 mm polyimide (Kapton) or LDPE</td></tr>
<tr><td>Drive motor</td><td>12V DC gear motor, 6 RPM</td><td>—</td></tr>
<tr><td>Drive coupling</td><td>Flexible shaft coupler, 6mm-to-6mm</td><td>—</td></tr>
<tr><td>Position sensor</td><td>10-turn pot 10k&Omega;</td><td>—</td></tr>
<tr><td>Controller</td><td>ESP32 WROOM-32</td><td>—</td></tr>
<tr><td>Limit switches</td><td>2&times; microswitches (min and max)</td><td>—</td></tr>
<tr><td>OLED display</td><td>SSD1306 0.96&rdquo;</td><td>—</td></tr>
<tr><td>Backing</td><td>—</td><td>PVC strip, self-adhesive velcro</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<h3>4-1 Motorized Air-Variable</h3>
<ol>
<li>Couple the DC motor to the capacitor shaft via a flexible coupler.
    Flexible couplers compensate for shaft misalignment that would otherwise
    bind the capacitor bearings.</li>
<li>Mount two microswitches at the minimum and maximum capacitance positions
    (use the shaft angular position to trigger them via a cam or lever arm).</li>
<li>Couple the 10-turn pot to the capacitor shaft (1:1 ratio) for position
    feedback.  Wire the pot as a voltage divider (0&ndash;3.3V range for
    ESP32 ADC).</li>
<li>L298N H-bridge or L293D motor driver provides bidirectional motor control
    from ESP32 GPIO signals.  Use PWM for speed control if needed (slower
    near endpoints for finer positioning).</li>
</ol>
<h3>4-2 Stack-On Flat-Plate Capacitor Units</h3>
<ol>
<li>Cut copper foil to 50&times;75&nbsp;mm sheets.  Cut polymer dielectric
    (Kapton, LDPE, or polypropylene) to 50&times;80&nbsp;mm (slightly larger
    to prevent edge breakdown).</li>
<li>Assemble: copper foil (top) / polymer / copper foil (bottom) / PVC backing strip.
    Offset the top and bottom foil tabs so they do not touch when stacking.</li>
<li>For parallel stacking: connect all top tabs together (plate A), all bottom
    tabs together (plate B).  Each additional unit adds &sim;50&ndash;200&nbsp;pF
    depending on dielectric thickness and area.</li>
<li>Secure units in a stack with velcro strips.  The modular design allows
    capacitance to be added or removed without soldering.</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<h3>5-1 Motorized Air-Variable</h3>
<ol>
<li>On power-up, the controller moves to the stored last position (from NVS).</li>
<li>Enter the desired capacitance on the OLED display using the rotary encoder.
    The motor drives to the corresponding position (from the calibration table).</li>
<li>For magnetic loop antenna tuning: the ESP32 receives SWR data from the
    SWR bridge and automatically sweeps capacitance until SWR minimum is found.</li>
</ol>
<h3>5-2 Stack-On Capacitor</h3>
<p>Stack-on units are passive; simply connect the plate-A lead to one circuit
node and the plate-B lead to the other.  Add or remove units to increase
or decrease capacitance in approximately 100&ndash;200&nbsp;pF steps.</p>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Motorized cap: move the capacitor to 10 equally-spaced pot positions.
    Measure capacitance at each position with an LCR meter or NanoVNA (use
    the impedance measurement mode at a known frequency).  Store the
    (ADC_value, pF) table in ESP32 NVS.</li>
<li>Stack-on caps: measure each unit individually with an LCR meter at
    100&nbsp;kHz.  Label each unit with its measured capacitance.  Build
    a combination table: n units in parallel = n &times; C_unit.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Motorized cap positioning: command each of 5 target capacitances;
    measure actual capacitance; error must be &lt;5% of target.</li>
<li>Limit switch test: command travel past the end limits; controller must
    stop at the limit switches without mechanical damage to the capacitor.</li>
<li>Stack-on units: each unit must measure within 20% of its labeled value
    (LCR meter verification; flat-plate caps have significant tolerance).</li>
<li>HV rating: apply the rated voltage across the capacitor at maximum
    capacitance setting and hold for 10 seconds.  No arc-over or increase
    in leakage current.</li>
<li>Log: date, cap type, calibration table (10 points for motorized),
    stack-on unit values, positioning accuracy, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Flat-Plate Capacitor Design Formula</h2>
<pre>C = &epsilon;0 &times; &epsilon;r &times; A / d

Example: 50 × 75 mm LDPE sheet (0.05 mm thick):
  A = 0.050 × 0.075 = 0.00375 m&sup2;
  d = 0.05 mm = 5×10&minus;5 m
  &epsilon;r (LDPE) = 2.3
  C = 8.854e-12 × 2.3 × 0.00375 / 5e-5
  C = 8.854e-12 × 172.5 = 1527 pF &asymp; 1.5 nF per unit

For 100 pF per unit: reduce area or increase dielectric thickness.</pre>

<h2 class="chapter-heading" id="appb">Appendix B — Common Air-Variable Specifications</h2>
<table>
<tr><th>Designation</th><th>C_min (pF)</th><th>C_max (pF)</th><th>Plate spacing</th><th>HV rating</th></tr>
<tr><td>Compact HF (Eddystone)</td><td>5</td><td>150</td><td>1.5mm</td><td>500V</td></tr>
<tr><td>Standard HF (MFJ type)</td><td>10</td><td>365</td><td>2.5mm</td><td>1500V</td></tr>
<tr><td>HV Transmitter (surplus)</td><td>10</td><td>300</td><td>5.0mm</td><td>5000V</td></tr>
<tr><td>Butterfly (balanced)</td><td>5</td><td>100</td><td>2.0mm</td><td>1000V</td></tr>
</table>
""")


# ─── Write files ──────────────────────────────────────────────────────────────

if __name__ == "__main__":
    written = 0
    for subdir, tm in TOMS.items():
        out_dir = BASE / subdir
        if not out_dir.exists():
            print(f"  SKIP (dir not found): {subdir}")
            continue
        out_file = out_dir / "tm.html"
        content = html(**tm)
        out_file.write_text(content, encoding="utf-8")
        chars = len(content)
        print(f"  OK ({chars:,} chars): {subdir} — {tm['tm_num']} {tm['title'][:50]}")
        written += 1
    print(f"\nDone. {written} TMs written.")
