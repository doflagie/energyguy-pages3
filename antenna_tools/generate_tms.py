#!/usr/bin/env python3
"""
generate_tms.py  —  Write tm.html for all 10 antenna_tools directories.
Run from any directory:  python3 generate_tms.py
Output: antenna_tools/<subdir>/tm.html  (10 files)
"""

from pathlib import Path

BASE = Path(__file__).parent  # antenna_tools/

DATE = "26 May 2026"
AUTHOR = "Mervyn Martin, KO6NNH"

# ─── HTML template ────────────────────────────────────────────────────────────

def html(tm_num, title, subtitle, body, back_label="Antenna Tools"):
    nav = """        <nav>
            <h3>Main Navigation</h3>
            <ul>
                <li><a href="/">Home</a></li>
                <li><a href="/about/bio.html">About</a></li>
            </ul>
            <details class="nav-group">
                <summary>Antennas</summary>
                <ul class="nav-scroll">
                    <li><a href="/antennas/phased_array_of_2/tm.html">2-Element Phased Array</a></li>
                    <li><a href="/antennas/phased_array_of_4/tm.html">4-Element Phased Array</a></li>
                    <li><a href="/antennas/dipole/tm.html">Dipole</a></li>
                    <li><a href="/antennas/yagi_uda/tm.html">Yagi-Uda</a></li>
                    <li><a href="/antennas/end_fed_half_wave/tm.html">EFHW</a></li>
                    <li><a href="/antennas/vertical/tm.html">Vertical</a></li>
                </ul>
            </details>
            <details class="nav-group">
                <summary>Antenna Gear</summary>
                <ul>
                    <li><a href="/antenna_gear/index.html">Antenna Gear Overview</a></li>
                    <li><a href="/antenna_gear/baluns-and-ununs/">Baluns &amp; Un-Uns</a></li>
                    <li><a href="/antenna_gear/coax_traps/">Coax Traps</a></li>
                    <li><a href="/antenna_gear/lightning_protection/">Lightning Protection</a></li>
                </ul>
            </details>
            <details class="nav-group" open>
                <summary>Antenna Tools</summary>
                <ul>
                    <li><a href="/antenna_tools/index.html">Antenna Tools Overview</a></li>
                    <li><a href="/antenna_tools/adf_systems/tm.html">ADF Systems</a></li>
                    <li><a href="/antenna_tools/antenna-analyzer/tm.html">Antenna Analyzer</a></li>
                    <li><a href="/antenna_tools/antenna_modeling/tm.html">Antenna Modeling</a></li>
                    <li><a href="/antenna_tools/dummy_loads/tm.html">Dummy Loads</a></li>
                    <li><a href="/antenna_tools/field_strength_meter/tm.html">Field Strength Meter</a></li>
                    <li><a href="/antenna_tools/noise_bridge/tm.html">Noise Bridge</a></li>
                    <li><a href="/antenna_tools/rf_power_meter/tm.html">RF Power Meter (QRP)</a></li>
                    <li><a href="/antenna_tools/rf_power_meters/tm.html">RF Power Meters (General)</a></li>
                    <li><a href="/antenna_tools/sdr_interfaces/tm.html">SDR Interfaces</a></li>
                    <li><a href="/antenna_tools/swr_meter/tm.html">SWR Meter</a></li>
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
                    <a href="../index.html">&larr; {back_label}</a>
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

# ── TM-TOOL-001  ADF Systems ─────────────────────────────────────────────────
TOMS['adf_systems'] = dict(
    tm_num="TM-TOOL-001",
    title="ADF SYSTEMS — ANTENNA DIRECTION FINDING",
    subtitle="Watson-Watt, Doppler, Phased Array, and Rotary Loop Methods",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers four antenna direction finding (ADF) system architectures
used in amateur radio and field operations: Watson-Watt two-channel amplitude
comparison, Doppler frequency-shift bearing, switched phased-array (Butler
Matrix), and rotary loop servo.  All four systems produce a bearing to an RF
source; choice depends on frequency range, required accuracy, and available
hardware.</p>
<p><strong>Covered frequency range:</strong> 1.8&ndash;450 MHz depending on antenna
array geometry.  The Butler Matrix design covers HF through VHF with a single
mechanical layout scaled to frequency.</p>
<p><strong>Bearing accuracy targets:</strong></p>
<ul>
<li>Watson-Watt: &plusmn;5&deg; RMS in clear-field conditions</li>
<li>Doppler: &plusmn;2&deg; RMS, dependent on rotation rate and SNR</li>
<li>4-element Butler Matrix: &plusmn;3&deg; RMS interpolated</li>
<li>Rotary servo loop: &plusmn;1&deg; mechanical, &plusmn;2&deg; bearing</li>
</ul>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Watson-Watt Method</h3>
<p>Two orthogonal loop antennas (North-South and East-West) produce sine and
cosine voltage components of received signal amplitude as a function of bearing.
A third omnidirectional element (sense antenna) resolves the 180&deg; ambiguity.</p>
<p>Bearing calculation:</p>
<pre>bearing = atan2(V_NS, V_EW)   (degrees, corrected for magnetic declination)</pre>
<p>Each channel must be gain-matched to within &plusmn;0.5 dB; phase-matched to
within &plusmn;2&deg; across the operating band.  Gain/phase imbalance directly
adds to bearing error.</p>

<h3>2-2 Doppler Method</h3>
<p>An element sequentially commutated around a circle of diameter d causes a
frequency modulation on the received carrier equal to:</p>
<pre>f_d = (v / &lambda;) &times; cos(&theta; &minus; &phi;)</pre>
<p>where v = element tangential velocity = &pi;df_rot, &lambda; = wavelength,
&theta; = bearing to source, &phi; = current element angle.  A PLL or IQ
discriminator extracts &theta; from the FM sidebands.  Rotation rate
f_rot is typically 100&ndash;600 Hz; higher rates extend the capture range
but increase bandwidth requirements.</p>

<h3>2-3 Butler Matrix (4-Element Switched Array)</h3>
<p>Four elements in a square array with &lambda;/4 spacing feed a passive Butler
Matrix beamforming network.  The matrix produces four orthogonal beams at
&plusmn;45&deg; and &plusmn;135&deg; simultaneously.  RSSI comparison across the
four beam ports yields a bearing estimate; interpolation between adjacent
beam peaks resolves bearing to approximately 3&deg; RMS.</p>
<p>The Butler Matrix is constructed from four 3&nbsp;dB 90&deg; hybrid couplers
and two fixed 45&deg; phase shifters interconnected as a 4&times;4 passive network.
No active components in the RF path; loss is approximately 0.5&ndash;1.5&nbsp;dB
depending on hybrid quality.</p>

<h3>2-4 GPS Compass Integration</h3>
<p>Magnetic bearing from an ADF system must be converted to true bearing for
navigation use.  GPS compass integration provides:</p>
<ul>
<li>Vehicle/platform heading reference (eliminates mount alignment error)</li>
<li>Magnetic declination correction (from GPS position + WMM model)</li>
<li>True bearing output = ADF magnetic bearing &minus; declination + heading offset</li>
</ul>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Item</th><th>Watson-Watt</th><th>Doppler</th><th>Butler Matrix</th></tr>
<tr><td>Antenna elements</td><td>2 loops + sense whip</td><td>4&ndash;8 vertical elements</td><td>4 vertical elements</td></tr>
<tr><td>Element spacing</td><td>Loop aperture sets sensitivity</td><td>&lambda;/4 radius circle</td><td>&lambda;/4 square</td></tr>
<tr><td>RF channels</td><td>2 (plus sense)</td><td>1 commutated</td><td>4 simultaneous</td></tr>
<tr><td>SDR / receiver</td><td>Dual-channel coherent</td><td>Single channel</td><td>4-channel or switched</td></tr>
<tr><td>Controller</td><td>ESP32 or PC</td><td>ESP32 (PWM commutation)</td><td>ESP32 (RSSI ADC)</td></tr>
<tr><td>Coax</td><td>Matched-length pairs</td><td>Switched relay tree</td><td>Fixed equal-length runs</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction and Assembly</h2>
<h3>4-1 Watson-Watt Loop Construction</h3>
<ol>
<li>Wind two identical shielded loops: 30&nbsp;cm diameter, 3 turns #18 AWG,
    Faraday shield (gap at top, not a closed loop).</li>
<li>Mount loops orthogonally on a common center mast; orient one loop N-S,
    other E-W.</li>
<li>Run equal-length coax (within 5&nbsp;mm) from each loop to the receiver
    switching point.</li>
<li>Connect sense antenna (vertical whip, &lambda;/4 at center frequency) at
    same switching point.</li>
</ol>

<h3>4-2 Butler Matrix PCB</h3>
<ol>
<li>Fabricate 4-port 3&nbsp;dB 90&deg; hybrid couplers on FR4 (Z<sub>0</sub>=35.4&Omega;
    microstrip, &lambda;/4 length at design frequency).</li>
<li>Connect hybrids per the Butler Matrix topology:
    E1,E2 &rarr; Hybrid #1,#2; outputs of Hybrid #1,#2 &rarr; Hybrid #3,#4
    with a 45&deg; fixed phase shifter between Hybrid #2 output and Hybrid #4 input.</li>
<li>Verify phase relationships with NanoVNA S21 phase measurements before
    installing array elements.</li>
<li>Mount array elements at corners of a square, &lambda;/4 side length, connected
    to matrix element ports E1&ndash;E4.</li>
</ol>

<h3>4-3 Doppler Commutator</h3>
<ol>
<li>Mount 4 or 8 vertical elements equally spaced on a circle of radius
    &lambda;/4 at operating frequency.</li>
<li>Wire each element through an RF relay (PIN diode or mechanical) to a
    common output coax.</li>
<li>Drive relay sequence from ESP32 GPIO at rotation rate f_rot (start at
    200&nbsp;Hz for HF).</li>
<li>Connect common output to receiver; extract FM component in software
    (GNU Radio or custom DSP).</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<h3>5-1 Watson-Watt Bearing Measurement</h3>
<ol>
<li>Tune receiver to target signal.  Confirm adequate SNR (&gt;20&nbsp;dB for
    &plusmn;5&deg; accuracy).</li>
<li>Enable 2-channel sampling.  Record V_NS and V_EW amplitude values.</li>
<li>Compute bearing: <code>brg = atan2(V_NS, V_EW)</code>.  Apply declination
    correction.</li>
<li>Confirm sense: enable sense antenna and verify the 180&deg; correct quadrant.</li>
<li>Average 10 readings; discard outliers more than 15&deg; from median.</li>
</ol>

<h3>5-2 Butler Matrix Bearing Estimate</h3>
<ol>
<li>Read RSSI on all four beam ports simultaneously (or in rapid sequence
    &lt;10&nbsp;ms total).</li>
<li>Identify the two highest-RSSI ports (adjacent beams straddle the signal).</li>
<li>Interpolate: bearing = beam_angle_1 + 45&deg; &times; (RSSI_1 / (RSSI_1 + RSSI_2)).</li>
<li>Apply platform heading offset if mounted on a moving vehicle.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<h3>6-1 Watson-Watt Channel Balance</h3>
<ol>
<li>Inject equal-amplitude, in-phase signal into both channels simultaneously
    from a common splitter.  Verify RSSI within 0.3&nbsp;dB.</li>
<li>If unbalanced: add fixed attenuator pad (1&ndash;3&nbsp;dB) to the stronger
    channel at the receiver input.</li>
<li>Inject signal into NS channel only.  Verify 90&deg; + known bearing reads
    correctly.  Repeat for EW channel.</li>
</ol>

<h3>6-2 Butler Matrix Beam Verification</h3>
<ol>
<li>Place a known CW signal at each of the four expected beam-peak azimuths
    in turn (0&deg;, 90&deg;, 180&deg;, 270&deg;).</li>
<li>Verify that the corresponding beam port shows maximum RSSI and adjacent
    ports show &ge;3&nbsp;dB lower level.</li>
<li>Record beam-center azimuths.  Apply offset table in software if beam
    centers deviate more than 5&deg; from design.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<p>After calibration, conduct a bearing verification test using a known-location
transmitter:</p>
<ol>
<li>Position a low-power test transmitter at a measured azimuth from the
    array (use GPS or compass, to within &plusmn;1&deg;).</li>
<li>Take 20 bearing readings; compute mean and standard deviation.</li>
<li>Acceptance criterion: mean error &lt;5&deg;, standard deviation &lt;3&deg;.</li>
<li>If failed: re-check element spacing, coax phase lengths, and channel balance.</li>
<li>Record results in calibration log: date, frequency, test azimuth, mean
    bearing, standard deviation, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Design Parameters Quick Reference</h2>
<table>
<tr><th>Parameter</th><th>Watson-Watt</th><th>Doppler</th><th>Butler 4-el</th></tr>
<tr><td>Accuracy (typical)</td><td>&plusmn;5&deg;</td><td>&plusmn;2&deg;</td><td>&plusmn;3&deg;</td></tr>
<tr><td>Min SNR required</td><td>20 dB</td><td>15 dB</td><td>10 dB</td></tr>
<tr><td>Multipath sensitivity</td><td>High</td><td>Medium</td><td>Low</td></tr>
<tr><td>Moving platform</td><td>Poor</td><td>Good</td><td>Good</td></tr>
<tr><td>Hardware complexity</td><td>Low</td><td>Medium</td><td>High</td></tr>
</table>

<h2 class="chapter-heading" id="appb">Appendix B — Worked Bearing Example</h2>
<p>Watson-Watt on 14.225 MHz.  Measured: V_NS = 0.82 V, V_EW = 0.57 V.</p>
<pre>bearing = atan2(0.82, 0.57) = atan2(0.82, 0.57) = 55.2 deg
Declination (Merced, CA) = +12.3 deg East
True bearing = 55.2 + 12.3 = 67.5 deg True</pre>
<p>Sense antenna confirms NE quadrant (not SW ambiguity).  Final reported
bearing: <strong>068&deg; True</strong>.</p>
""")

# ── TM-TOOL-002  Antenna Analyzer ─────────────────────────────────────────────
TOMS['antenna-analyzer'] = dict(
    tm_num="TM-TOOL-002",
    title="ANTENNA ANALYZER — VECTOR IMPEDANCE MEASUREMENT",
    subtitle="AD8302 Phase/Magnitude Detector, AD9851 DDS, ADF4351 Synthesizer",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers the construction and operation of a homebrew antenna
analyzer covering 0.1&nbsp;MHz to 1.3&nbsp;GHz using a two-stage RF source
architecture: an AD9851 DDS for HF (0.1&ndash;30&nbsp;MHz) and an ADF4351
fractional-N PLL synthesizer for VHF/UHF (30&nbsp;MHz&ndash;1.3&nbsp;GHz).
The measurement front-end is an AD8302 gain/phase detector providing
simultaneous magnitude and phase readout.  Output is displayed on a CYD
(ESP32 + 2.8&rdquo; touchscreen) with Smith chart, SWR plot, and R+jX numerical
display.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 AD8302 Gain/Phase Detector</h3>
<p>The AD8302 accepts two RF signals on INPA and INPB (50&Omega; each) and
produces two DC outputs:</p>
<ul>
<li><strong>VMAG:</strong> Proportional to |V_A / V_B| in dB (30&nbsp;mV/dB,
    range &minus;30 to 0&nbsp;dBm per input).</li>
<li><strong>VPHS:</strong> Proportional to phase difference (10&nbsp;mV/&deg;,
    0&deg; to 180&deg; range).</li>
</ul>
<p>INPA receives the reference (forward-coupled) signal; INPB receives the
reflected signal.  The ratio VREFL/VFWD and the phase shift &Delta;&phi;
together yield the complex reflection coefficient &Gamma;:</p>
<pre>&Gamma; = |V_refl / V_fwd| &angle; &Delta;&phi;
Z_ant = Z0 &times; (1 + &Gamma;) / (1 &minus; &Gamma;)   (Z0 = 50 &Omega;)</pre>

<h3>2-2 Directional Coupler</h3>
<p>A multiband RF coupler (TM-TOOL-002-SCH-004) separates the forward and
reflected waves on the transmission line to the DUT (antenna).  Coupling
factor is &minus;20&nbsp;dB on the HF section and &minus;20&nbsp;dB on the
VHF/UHF section.  Directivity must be &ge;20&nbsp;dB across the band to keep
reflected signal isolation adequate for accurate &Gamma; measurement.</p>

<h3>2-3 Frequency Sources</h3>
<p><strong>AD9851 DDS</strong> (HF, 0.1&ndash;30&nbsp;MHz): 125&nbsp;MHz reference
clock, 32-bit frequency word, phase noise &minus;100&nbsp;dBc/Hz at 1&nbsp;kHz offset.
Controlled via 3-wire SPI from ESP32.  Sweep rate: ~1000 points/second.</p>
<p><strong>ADF4351 PLL</strong> (VHF/UHF, 30&nbsp;MHz&ndash;1.3&nbsp;GHz): Fractional-N
synthesizer, 32.768&nbsp;MHz TCXO reference, phase noise &minus;90&nbsp;dBc/Hz
at 10&nbsp;kHz.  Integer-boundary spurs require spur-avoidance in sweep
software when crossing integer multiples of the PFD reference frequency.</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>Part / Value</th><th>Purpose</th></tr>
<tr><td>Gain/phase detector</td><td>AD8302 LFCSP-16</td><td>|&Gamma;| and &angle;&Gamma; measurement</td></tr>
<tr><td>HF source</td><td>AD9851 DDS module</td><td>0.1&ndash;30 MHz sweep</td></tr>
<tr><td>VHF/UHF source</td><td>ADF4351 module</td><td>30&ndash;1300 MHz sweep</td></tr>
<tr><td>Directional coupler</td><td>Custom PCB</td><td>FWD/REF separation</td></tr>
<tr><td>Protection diodes</td><td>BAV99 + 100&Omega;</td><td>AD8302 input protection</td></tr>
<tr><td>Controller</td><td>ESP32 WROOM-32</td><td>SPI, ADC, display driver</td></tr>
<tr><td>Display</td><td>ILI9341 2.8&rdquo; CYD</td><td>Smith chart, SWR plot</td></tr>
<tr><td>Band switch</td><td>PE4259 SP4T</td><td>HF/VHF/UHF path selection</td></tr>
<tr><td>RF connectors</td><td>SMA female (×4)</td><td>DUT, source, ref ports</td></tr>
<tr><td>Power</td><td>5V USB-C, 500 mA</td><td>All rails via LDO regulators</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction and Assembly</h2>
<h3>4-1 PCB Layout Notes</h3>
<p>Route the RF signal paths (DDS/PLL output &rarr; coupler &rarr; DUT port) as
50&Omega; microstrip (width 2.9&nbsp;mm on 1.6&nbsp;mm FR4 with &epsilon;r=4.6).
Keep reference and reflected coupler outputs equal length to the AD8302 inputs.
Length mismatch &gt;5&nbsp;mm introduces a phase error of approximately
&Delta;&phi; = 360&deg; &times; &Delta;L / &lambda;.</p>
<h3>4-2 AD8302 Bias</h3>
<p>The AD8302 requires &plusmn;5V dual supply.  Derive from 5V USB with a
MAX1044 charge pump for the &minus;5V rail.  Bypass each supply pin with
10&nbsp;nF NP0 + 100&nbsp;nF X5R within 3&nbsp;mm of the device.</p>
<h3>4-3 DDS Output Filtering</h3>
<p>The AD9851 output contains harmonics and alias products.  A 7-pole
elliptic low-pass filter (cutoff 35&nbsp;MHz) reduces spurious outputs to
&lt;&minus;60&nbsp;dBc before the coupler.  Without this filter, harmonic
reflections from the DUT appear as bearing errors at sub-harmonic frequencies.</p>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<h3>5-1 SWR Sweep</h3>
<ol>
<li>Connect antenna to DUT port (SMA).  Select band (HF or VHF/UHF).</li>
<li>Enter start/stop frequency and number of sweep points (101 or 201).</li>
<li>Press SWEEP.  The display plots SWR vs. frequency in real time.</li>
<li>Identify the resonance (SWR minimum).  Press MARKER; the instrument
    displays f_res, SWR, R, X at the marker frequency.</li>
</ol>
<h3>5-2 Smith Chart Display</h3>
<ol>
<li>Select SMITH mode.  The sweep traces the impedance locus on the Smith chart.</li>
<li>Clockwise rotation with increasing frequency = capacitive reactance dominant
    (antenna too short).  Counter-clockwise = inductive (antenna too long).</li>
<li>At resonance, the locus crosses the real axis; R at crossing = feedpoint
    resistance.  Ideal dipole: 72&Omega; (free space), lower over ground.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<h3>6-1 SOLT Calibration</h3>
<p>Before measuring an antenna, perform a one-port SOLT calibration at the
DUT SMA connector (not at the instrument chassis):</p>
<ol>
<li>Connect SHORT (shorted SMA cap).  Press CAL &rarr; SHORT.</li>
<li>Connect OPEN (SMA cap with no connection).  Press CAL &rarr; OPEN.</li>
<li>Connect 50&Omega; LOAD (SMA terminator).  Press CAL &rarr; LOAD.</li>
<li>Press CAL &rarr; DONE.  Calibration plane is now at DUT connector.</li>
</ol>
<p>After SOLT cal, short should read SWR &gt;50:1, open should read SWR &gt;50:1,
and 50&Omega; load should read SWR &lt;1.05:1 across the calibrated band.</p>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Connect a precision 50&Omega; load (Pasternack PE6010 or calibrated terminator).
    Verify SWR &lt;1.05:1 at all calibrated frequencies.</li>
<li>Connect a known 100&Omega; resistor (1%, non-inductive).  Verify R reads
    100 &plusmn; 5&Omega;, X reads 0 &plusmn; 5&Omega; at 1&nbsp;MHz.</li>
<li>Connect a 50&Omega; + 50&nbsp;nH load (resistor in series with known inductor).
    Verify jX reads within &plusmn;10% of calculated inductive reactance at 10&nbsp;MHz.</li>
<li>If any check fails, repeat SOLT calibration.  Persistent errors indicate
    coupler asymmetry or AD8302 bias fault.</li>
<li>Log: date, calibration kit used, frequency range, short SWR, open SWR,
    load SWR, 100&Omega; R/X readings.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Formulas</h2>
<pre>SWR = (1 + |&Gamma;|) / (1 &minus; |&Gamma;|)
|&Gamma;| = (SWR &minus; 1) / (SWR + 1)
Return loss (dB) = &minus;20 log10(|&Gamma;|)
Z_ant = 50 &times; (1 + &Gamma;) / (1 &minus; &Gamma;)    [complex arithmetic]</pre>

<h2 class="chapter-heading" id="appb">Appendix B — Worked Example</h2>
<p>AD8302 reads VMAG = 2.10 V, VPHS = 0.75 V at 14.250 MHz.</p>
<pre>Magnitude ratio (dB) = (2.10 &minus; 1.80) / 0.030 = +10 dB  [AD8302 midpoint = 1.80V]
|&Gamma;| = 10^(10/20) = 0.316  (not a valid interpretation here &mdash; see note)

Correct: VMAG represents |V_INPA / V_INPB|.
If VMAG midpoint (ratio = 1) = 900 mV, then:
  ratio_dB = (2100 &minus; 900) / 30 = +40 dB  (INPA is 40 dB stronger than INPB)
  |&Gamma;| = 10^(&minus;40/20) = 0.01  &rarr; SWR = 1.02:1 (near-perfect match)</pre>
""")

# ── TM-TOOL-003  Antenna Modeling ─────────────────────────────────────────────
TOMS['antenna_modeling'] = dict(
    tm_num="TM-TOOL-003",
    title="NEC ANTENNA MODELING SYSTEM",
    subtitle="NEC2/NEC4 File Generation, Running, Parsing, and Optimization",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers the NEC antenna modeling system: a Python toolkit that
generates NEC2/NEC4 input decks, runs the engine, parses output, and displays
radiation patterns, impedance, and gain.  The system supports dipoles,
verticals, Yagis, log-periodics, quad loops, phased arrays, and ground-mounted
verticals with buried radials.  An optional REST API exposes the toolkit to
web front-ends or remote callers.</p>
<p><strong>Engines supported:</strong> nec2c (Linux, free), NEC4 (licensed),
4nec2 (Windows GUI via Wine), necpp (open-source alternative).</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 NEC Card Format Summary</h3>
<p>NEC input is a plain-text deck of two-character card mnemonics, one per line:</p>
<table>
<tr><th>Card</th><th>Purpose</th><th>Key fields</th></tr>
<tr><td>CM</td><td>Comment</td><td>Free text</td></tr>
<tr><td>CE</td><td>End comments</td><td>(no fields)</td></tr>
<tr><td>GW</td><td>Wire segment</td><td>tag, segs, x1 y1 z1, x2 y2 z2, radius</td></tr>
<tr><td>GS</td><td>Scale geometry</td><td>scale factor (e.g., 0.3048 = feet to meters)</td></tr>
<tr><td>GE</td><td>End geometry</td><td>0=free space, 1=ground present</td></tr>
<tr><td>GN</td><td>Ground parameters</td><td>type, &epsilon;r, &sigma; (good earth: 13, 0.005)</td></tr>
<tr><td>EX</td><td>Excitation (source)</td><td>tag, segment, V_real, V_imag</td></tr>
<tr><td>FR</td><td>Frequency</td><td>start MHz, step MHz, N steps</td></tr>
<tr><td>RP</td><td>Radiation pattern</td><td>theta/phi start, step, N points, mode</td></tr>
<tr><td>EN</td><td>End of input</td><td>(last card)</td></tr>
</table>
<h3>2-2 Segment Count Guidelines</h3>
<p>NEC accuracy depends on adequate segmentation.  Rule: 10&ndash;20 segments
per wavelength, odd count preferred for symmetric elements with a center source.
Minimum 3 segments per wire.  The system function:</p>
<pre>def recommended_segments(length_m, freq_mhz, min_segs=3):
    wavelength = 299.792458e6 / (freq_mhz * 1e6)
    segs = max(min_segs, int(length_m / wavelength * 10))
    return segs if segs % 2 == 1 else segs + 1</pre>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Installation and Dependencies</h2>
<h3>3-1 Python Environment</h3>
<p>Python 3.10 or later is required.  Install dependencies:</p>
<pre>pip install -r requirements.txt</pre>
<p>Core packages: numpy, scipy, pandas, matplotlib, fastapi, uvicorn, pydantic,
httpx, tqdm, requests.  Optional: scikit-rf (Smith chart), plotly (interactive
3D patterns).</p>
<h3>3-2 NEC2 Engine Installation (Debian/Ubuntu)</h3>
<pre>sudo apt install nec2c</pre>
<p>Verify: <code>nec2c --version</code> should return without error.  The runner
(nec_runner.py) auto-detects available engines in the order: nec2c, nec4,
necpp, 4nec2.</p>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Generating NEC Input Files</h2>
<h3>4-1 Dipole Example</h3>
<pre>from nec_generator import Dipole, NECModel
ant = Dipole(freq_mhz=14.25, height_m=10.0)
model = ant.to_nec_model()
model.write("dipole_20m.nec")</pre>
<p>This generates a half-wave dipole resonant at 14.25 MHz, 10 m above real
ground (&epsilon;r=13, &sigma;=0.005 S/m).  The GW card for each half-element
uses <code>recommended_segments()</code> to set segment count automatically.</p>
<h3>4-2 Vertical with Radials</h3>
<pre>from nec_generator import VerticalWithRadials
ant = VerticalWithRadials(freq_mhz=7.1, n_radials=32, radial_length_m=10.0,
                          height_m=0.1)
model = ant.to_nec_model()
model.write("vertical_40m.nec")</pre>
<h3>4-3 Frequency Sweep</h3>
<pre>from freq_sweep import FrequencySweep
sweep = FrequencySweep(model, start_mhz=1.8, stop_mhz=30.0, n_steps=200)
results = sweep.run()
sweep.plot_impedance(results)
sweep.plot_swr(results, z0=50.0)</pre>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Running and Parsing</h2>
<h3>5-1 Running the Engine</h3>
<pre>from nec_runner import NECRunner, NECEngine
runner = NECRunner(engine=NECEngine.NEC2)
result = runner.run(model)
if not result.success:
    print(result.stderr)</pre>
<p>The runner writes a temporary .nec file, invokes nec2c as a subprocess,
captures stdout/stderr, and returns a NECResult object.  Timeout default:
60 seconds (adjustable for large sweeps).</p>
<h3>5-2 Parsing Output</h3>
<pre>from nec_parser import NECOutputParser
parser = NECOutputParser()
points = parser.parse(result.output_file)
# points: list of {freq_mhz, gain_dbi, z_real, z_imag, swr, theta, phi}</pre>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Optimization</h2>
<p>Three optimizers are available:</p>
<ul>
<li><strong>genetic_optimizer.py:</strong> Genetic algorithm; best for
    multi-parameter problems with discontinuous cost functions.  Suitable
    for Yagi element lengths + spacings.</li>
<li><strong>pso_optimizer.py:</strong> Particle swarm; faster convergence
    for smooth continuous cost surfaces.  Suitable for matching network
    component values.</li>
<li><strong>batch_optimizer.py / batch_multiband.py:</strong> Evaluates a
    parameter grid; useful for initial design-space exploration before running
    a stochastic optimizer.</li>
</ul>
<p>Cost function examples: maximize F/B ratio, minimize SWR at target frequency,
maximize gain over ground at 20&deg; elevation.</p>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Run the included test case (half-wave dipole in free space at 14.25 MHz).
    Expected: feedpoint impedance 73+j42.5&Omega;, gain 2.15 dBi.</li>
<li>Verify against ARRL Antenna Book reference tables for dipole impedance vs.
    height above ground.  Deviations &gt;5% indicate a segmentation or ground
    parameter error.</li>
<li>For a calibration-quality check: compare modeled resonant frequency against
    NanoVNA measurement of a physical antenna.  Agreement within 2% is typical
    for wire antennas over flat ground.</li>
<li>Log: NEC version, Python version, test antenna type, modeled vs. reference
    gain, modeled vs. measured resonant frequency.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Common NEC Errors</h2>
<table>
<tr><th>Error</th><th>Cause</th><th>Fix</th></tr>
<tr><td>SEGMENT FAILURE</td><td>Wire too short for segment count</td><td>Reduce min_segs or increase length</td></tr>
<tr><td>Impedance = 0+j0</td><td>Source on wrong tag/segment</td><td>Verify EX card tag matches GW tag</td></tr>
<tr><td>Gain &gt;30 dBi</td><td>Wire below ground (Z &lt; 0)</td><td>Ensure all Z coordinates &ge; 0 or use buried-radial GN type</td></tr>
<tr><td>NaN impedance</td><td>Singular matrix (parallel wires touching)</td><td>Increase wire separation &gt;2 &times; radius</td></tr>
</table>

<h2 class="chapter-heading" id="appb">Appendix B — Dipole Resonant Length Formula</h2>
<pre>L_half (meters) = (142.5 / f_MHz) &times; k
  k = velocity factor:
    Bare wire in free space: k = 0.975
    Wire near ground (h &lt; &lambda;/4): k = 0.94&ndash;0.97
    Insulated wire: k = 0.93&ndash;0.97 depending on insulation</pre>
""")

# ── TM-TOOL-004  Dummy Loads ───────────────────────────────────────────────────
TOMS['dummy_loads'] = dict(
    tm_num="TM-TOOL-004",
    title="RF DUMMY LOADS — CONSTRUCTION AND USE",
    subtitle="100W Dry, 500W Oil-Cooled, 1 kW Oil-Cooled, and VNA Calibration Loads",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers three power-rated dummy load designs (100W dry,
500W oil-cooled, 1&nbsp;kW oil-cooled) and a precision 50&Omega; VNA calibration
terminator.  All designs target &lt;1.1:1 SWR from 1.8&nbsp;MHz through 450&nbsp;MHz
and &lt;1.5:1 SWR through 1.3&nbsp;GHz (VNA terminator only).</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Resistor Network Fundamentals</h3>
<p>Non-inductive resistors in parallel sets combine power ratings and
maintain the 50&Omega; impedance.  For N identical resistors each of value
R in parallel: Z = R / N.  Use:</p>
<ul>
<li>100W load: 4 &times; 200&Omega; 25W resistors in parallel = 50&Omega;, 100W</li>
<li>500W load: 4 &times; 200&Omega; 125W in parallel = 50&Omega;, 500W</li>
<li>1&nbsp;kW load: 8 &times; 400&Omega; 125W in parallel = 50&Omega;, 1000W</li>
</ul>
<h3>2-2 Parasitic Inductance and Frequency Limit</h3>
<p>Each resistor lead has approximately 5&ndash;10 nH of lead inductance.
At 150 MHz: X_L = 2&pi; &times; 150e6 &times; 8e-9 = 7.5&Omega;, causing SWR to rise
above 1.5:1.  Minimizing lead length to &le;10&nbsp;mm each side and using
star wiring (all high ends bonded to a single center stud) reduces effective
inductance to &lt;2&nbsp;nH, extending usable range to 450&nbsp;MHz.</p>
<h3>2-3 Oil Cooling</h3>
<p>Transformer oil (mineral oil) provides thermal conductivity of
0.135&nbsp;W/m&middot;K, approximately 10&times; better than still air.  Resistors
submerged in oil can dissipate their full rated power continuously at
25&deg;C ambient.  Seal the container; oil expands approximately 7% from
20&deg;C to 100&deg;C.</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Item</th><th>100W Dry</th><th>500W Oil</th><th>1 kW Oil</th></tr>
<tr><td>Resistors</td><td>4&times;200&Omega; 25W (Ohmite)</td><td>4&times;200&Omega; 125W</td><td>8&times;400&Omega; 125W</td></tr>
<tr><td>Connector</td><td>SO-239 or N-type</td><td>N-type</td><td>N-type</td></tr>
<tr><td>Heatsink</td><td>150&times;100&times;40mm extruded Al</td><td>—</td><td>—</td></tr>
<tr><td>Container</td><td>—</td><td>2L metal paint can</td><td>5L metal can</td></tr>
<tr><td>Oil</td><td>—</td><td>1.5L mineral oil</td><td>4L mineral oil</td></tr>
<tr><td>Center stud</td><td>M4 brass bolt</td><td>M5 brass bolt</td><td>M6 brass bolt</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<h3>4-1 100W Dry Load</h3>
<ol>
<li>Mount SO-239 / N connector on heatsink.  Center conductor passes through
    to an M4 brass stud on the heatsink surface.</li>
<li>Cut resistor leads to &le;10&nbsp;mm each side.  Solder all high-side leads
    to center stud.  Solder all low-side leads to ground bus on heatsink.</li>
<li>Orient resistors flat against heatsink surface for maximum thermal contact.
    Apply thermal compound between each resistor body and heatsink.</li>
<li>Verify DC resistance: 49&ndash;51&Omega; between connector center and shell.</li>
</ol>
<h3>4-2 500W / 1 kW Oil Load</h3>
<ol>
<li>Drill and mount N connector through lid of can.  Extend center pin via
    threaded brass rod to center of can interior.</li>
<li>Wire resistors star-fashion around the center rod.  Ground bus: bare
    copper wire ring soldered to can walls.</li>
<li>Test resistor assembly in air before sealing.  Verify SWR &lt;1.1:1 at
    1.8&nbsp;MHz through 50&nbsp;MHz with NanoVNA.</li>
<li>Fill with mineral oil to within 25&nbsp;mm of lid (room for thermal expansion).
    Seal lid with silicone RTV.  Allow 24 hours to cure before transmitting.</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<ol>
<li>Always confirm dummy load is connected before transmitting.  A
    momentary key-down into open coax can damage the final transistor.</li>
<li>For CW/SSB testing: key down for &le;10 seconds, off for &ge;30 seconds
    until thermal equilibrium is reached (approximately 15 minutes at full power).</li>
<li>For the oil loads: after extended use, check for oil seepage at the
    lid seal.  If oil temperature exceeds 70&deg;C (hot to the touch), reduce
    duty cycle or allow cooling.</li>
<li>Never use the 100W dry load above 100W continuous; use the oil loads
    for higher power or high duty cycle modes (FT8, WSPR, digital).</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Calibrate NanoVNA with SOLT at the measurement connector (not at the
    NanoVNA port).</li>
<li>Sweep 1.8&nbsp;MHz to 450&nbsp;MHz.  Record SWR at 1.8, 3.5, 7, 14, 21,
    28, 50, 144, 222, 432&nbsp;MHz.</li>
<li>Acceptance criterion: SWR &lt;1.1:1 at 1.8&ndash;50&nbsp;MHz;
    SWR &lt;1.3:1 at 50&ndash;150&nbsp;MHz; SWR &lt;1.5:1 at 150&ndash;450&nbsp;MHz.</li>
<li>Record DC resistance: must be 49.0&ndash;51.0&Omega;.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Thermal test (100W load): apply 100W (from dummy transmitter or keyed
    transceiver) for 60 seconds.  Heatsink must not exceed 80&deg;C (use
    contact thermometer or thermal camera).</li>
<li>Thermal test (oil loads): apply rated power for 10 minutes.  Oil must
    not bubble or emit smoke.  If oil reaches 80&deg;C, reduce power or
    duty cycle in future use.</li>
<li>Post-thermal SWR check: repeat SWR sweep immediately after thermal test.
    SWR must not have increased by more than 0.1 compared to cold measurement.</li>
<li>Log: date, resistors used, DC resistance, SWR at key bands, thermal
    test result, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Power Derating Table</h2>
<table>
<tr><th>Ambient temp</th><th>100W load max</th><th>500W load max</th><th>1 kW load max</th></tr>
<tr><td>25&deg;C</td><td>100W continuous</td><td>500W continuous</td><td>1000W continuous</td></tr>
<tr><td>40&deg;C</td><td>70W continuous</td><td>350W continuous</td><td>700W continuous</td></tr>
<tr><td>50&deg;C</td><td>50W continuous</td><td>250W continuous</td><td>500W continuous</td></tr>
</table>

<h2 class="chapter-heading" id="appb">Appendix B — Resistor Selection</h2>
<p>Non-inductive (wirewound non-inductive or thick-film) resistors only.
Preferred types: Ohmite 270 series (wirewound NI), Vishay Dale RH series,
Caddock MP series.  Standard carbon film and metal film resistors are
acceptable for the VNA calibration terminator (&le;100&nbsp;mW) but NOT for
power loads (they are slightly inductive and not rated for RF).</p>
""")

# ── TM-TOOL-005  Field Strength Meter ─────────────────────────────────────────
TOMS['field_strength_meter'] = dict(
    tm_num="TM-TOOL-005",
    title="FIELD STRENGTH METER — CONSTRUCTION AND USE",
    subtitle="Passive Germanium, Active Op-Amp, and Digital CYD Versions",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers three field strength meter (FSM) designs in ascending
order of complexity: a passive germanium detector (no batteries), an active
op-amp version with calibrated dB scale, and a digital CYD-based FSM with
logging.  All measure relative field strength; absolute calibration in
V/m is possible with the active version using a known reference transmitter.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Passive Detector</h3>
<p>An RF signal intercepted by the probe antenna develops a voltage across
a germanium point-contact diode (1N34A).  The diode rectifies the RF to
DC; a 10&micro;F capacitor integrates (smooths) the rectified DC; a
100&micro;A FSD panel meter displays the result.  Sensitivity is limited by
the diode forward voltage (~0.2V for germanium vs. ~0.6V for silicon);
minimum detectable field from a 50&nbsp;cm whip at 7&nbsp;MHz is approximately
0.5&nbsp;mV/m.</p>
<h3>2-2 Active Version</h3>
<p>A common-base RF amplifier (BF199 or 2N3904) precedes the detector
diode, providing 20&ndash;30&nbsp;dB of gain before detection.  An op-amp
(LM386 or similar) drives a meter with logarithmic compression for a
dB-proportional scale.  Sensitivity improves to approximately 5&nbsp;&micro;V/m.</p>
<h3>2-3 Digital CYD FSM</h3>
<p>An AD8307 logarithmic amplifier (0.1&ndash;500&nbsp;MHz, &minus;74 to +17&nbsp;dBm,
25&nbsp;mV/dB) replaces the detector and meter.  Its output drives an ESP32
ADC; the CYD display shows field strength in dBm (relative to 50&Omega;)
and logs readings with GPS timestamp.  Absolute calibration ties the ADC
reading to a known field (calibrated reference transmitter at known distance).</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>Passive</th><th>Active</th><th>Digital CYD</th></tr>
<tr><td>Detector</td><td>1N34A germanium diode</td><td>1N34A or BAT42</td><td>AD8307 log amp</td></tr>
<tr><td>Amplifier</td><td>None</td><td>BF199 CE + LM386</td><td>Internal to AD8307</td></tr>
<tr><td>Display</td><td>100&micro;A panel meter</td><td>50&micro;A + dB scale</td><td>CYD ILI9341 2.8&rdquo;</td></tr>
<tr><td>Probe</td><td>50 cm whip, BNC</td><td>50 cm whip, BNC</td><td>SMA + 50&Omega; input</td></tr>
<tr><td>Power</td><td>None (passive)</td><td>9V battery</td><td>5V USB-C</td></tr>
<tr><td>Controller</td><td>—</td><td>—</td><td>ESP32 WROOM-32</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<h3>4-1 Passive FSM</h3>
<ol>
<li>Mount 1N34A diode cathode toward meter positive terminal.  R_load = 100&nbsp;k&Omega;;
    C_filter = 10&micro;F 16V electrolytic.  Meter series resistance sets FSD.</li>
<li>Connect probe antenna (50&nbsp;cm rigid copper rod) via BNC to diode anode.</li>
<li>Mount in a small plastic enclosure.  Keep RF lead from BNC to diode
    &lt;20&nbsp;mm to minimize stray capacitance.</li>
<li>Optional: add a rotary attenuator (100&nbsp;k&Omega; potentiometer in the
    antenna lead) for sensitivity control near strong transmitters.</li>
</ol>
<h3>4-2 AD8307 Digital FSM</h3>
<ol>
<li>Mount AD8307 with 100&nbsp;nF NP0 decoupling on each supply pin.  INHI
    connects via 1&nbsp;nF DC-blocking cap to SMA input; INLO to ground.</li>
<li>VOUT pin (25&nbsp;mV/dB slope, intercept &minus;84&nbsp;dBm) connects to
    ESP32 ADC GPIO (12-bit, 3.3V range).  Scale: 3300&nbsp;mV / 25&nbsp;mV/dB =
    132&nbsp;dB dynamic range from ADC alone, but ADC noise floor limits
    practical range to &sim;80&nbsp;dB.</li>
<li>Add 10&nbsp;k&Omega; + 100&nbsp;nF low-pass filter between VOUT and ESP32
    ADC to reject RF on the DC output line.</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<h3>5-1 Antenna Pattern Measurement</h3>
<ol>
<li>Set up a reference transmitter at fixed power and distance from the
    antenna under test.  Distance must be &ge;2 &times; far-field criterion:
    d_ff = 2D&sup2;/&lambda; (D = antenna aperture).</li>
<li>Zero the FSM: rotate the test antenna to maximum signal; note the meter
    reading as the reference (0&nbsp;dB or 100%).</li>
<li>Rotate antenna in 15&deg; steps through 360&deg;.  Record reading at each
    step.  Compute relative pattern in dB: &Delta;dB = 20 log10(V/V_ref).</li>
<li>Plot the resulting pattern.  Front-to-back ratio, 3&nbsp;dB beamwidth, and
    sidelobe levels are directly readable from the plot.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<h3>6-1 Absolute Calibration (Active and Digital FSMs)</h3>
<ol>
<li>Place a calibrated signal generator (or TinySA in generator mode) at
    a measured distance r from the FSM probe.</li>
<li>Set generator output to a known level P (dBm) into a calibrated antenna
    with known gain G (dBi).  Compute E-field at distance r:</li>
</ol>
<pre>E (V/m) = sqrt(30 &times; P_watts &times; G_linear) / r</pre>
<ol start="3">
<li>Record FSM reading at this field level.  This is the calibration reference
    point.  Adjust offset constant in firmware so that the CYD display
    shows the calculated E-field value.</li>
<li>Repeat at &minus;10, &minus;20, &minus;30&nbsp;dB relative levels using known
    attenuators.  Verify linearity within &plusmn;1&nbsp;dB across the range.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Passive FSM: connect a 0&nbsp;dBm signal (from TinySA or signal generator)
    at 7&nbsp;MHz via 50&Omega; coax.  Meter should deflect to a repeatable scale
    reading.  Deflection should decrease by approximately half (6&nbsp;dB) when
    a 6&nbsp;dB attenuator is inserted.</li>
<li>Digital FSM: apply &minus;40&nbsp;dBm, &minus;50&nbsp;dBm, &minus;60&nbsp;dBm.
    Verify CYD readings within &plusmn;2&nbsp;dBm of expected values.
    (AD8307 typical accuracy: &plusmn;1&nbsp;dB from &minus;74 to +10&nbsp;dBm.)</li>
<li>Log: date, version (passive/active/digital), calibration signal source,
    calibration power level, measured offset, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — E-Field / Power Density Conversions</h2>
<pre>E (V/m) = sqrt(P_density_W/m2 &times; 120&pi;)
P_density (W/m2) = P_tx (W) &times; G_linear / (4&pi; &times; r2)
E (V/m) from dipole: E = sqrt(30 &times; P_tx &times; G) / r   (far field)</pre>

<h2 class="chapter-heading" id="appb">Appendix B — Worked Example</h2>
<p>100&nbsp;mW into a dipole (G = 2.15&nbsp;dBi = 1.64 linear) at 10&nbsp;m distance at 14&nbsp;MHz:</p>
<pre>E = sqrt(30 &times; 0.1 &times; 1.64) / 10 = sqrt(4.92) / 10 = 0.222 V/m = 222 mV/m</pre>
<p>The AD8307 input sees approximately &minus;37&nbsp;dBm into 50&Omega; from a
matched probe tuned to 14&nbsp;MHz.  The CYD should display approximately
0.22&nbsp;V/m after calibration.</p>
""")

# ── TM-TOOL-006  Noise Bridge ─────────────────────────────────────────────────
TOMS['noise_bridge'] = dict(
    tm_num="TM-TOOL-006",
    title="RF NOISE BRIDGE — IMPEDANCE MEASUREMENT",
    subtitle="Wheatstone Bridge, 0.5–60 MHz, R + jX Measurement, Null Detector",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers the construction and operation of an RF noise bridge
for antenna impedance measurement from 0.5 to 60&nbsp;MHz.  The bridge measures
complex impedance Z = R + jX by balancing a calibrated reference arm against
the unknown antenna.  Null detection uses either an external communications
receiver, a built-in audio amplifier, or an AD8307 logarithmic detector
with digital readout.  Accuracy: R &plusmn;5&Omega; (0&ndash;200&Omega; range),
X &plusmn;10&Omega; (&minus;200 to +200&Omega; range).</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Bridge Circuit</h3>
<p>A standard 4-arm Wheatstone bridge topology adapted for RF.  Two fixed
51&Omega; arms establish the bridge reference.  One variable arm (R_cal + jX_cal)
is the calibrated reference.  The unknown arm (Z_ant) is the antenna under test.
The null detector sees zero voltage when the bridge is balanced:</p>
<pre>Z_ant = R_cal + jX_cal   (at null)</pre>
<p>A 1:1:1 trifilar-wound transformer (T1) on a BN-43-202 core drives the two
bridge legs from the noise source.  Trifilar winding ensures &lt;1&deg; phase
imbalance between the two drive ports across 0.5&ndash;60&nbsp;MHz.</p>

<h3>2-2 Noise Source</h3>
<p>Two designs are provided:</p>
<ul>
<li><strong>NGZ (Zener):</strong> 5.1V zener diode biased into avalanche
    breakdown produces wideband white noise.  Output amplified by a MAR-6
    MMIC (+20&nbsp;dB, DC&ndash;2&nbsp;GHz) to approximately &minus;25&nbsp;dBm
    into 50&Omega;.</li>
<li><strong>NGT (Transistor):</strong> Reverse-biased NPN transistor base-emitter
    junction noise; higher output level and more uniform spectral density
    than zener.  Amplified by ERA-3SM MMIC.</li>
</ul>

<h3>2-3 Null Detection Modes</h3>
<table>
<tr><th>Mode</th><th>Hardware</th><th>Sensitivity</th><th>Notes</th></tr>
<tr><td>A — External RX</td><td>Communications receiver/SDR</td><td>&lt;&minus;60&nbsp;dBm</td><td>Best; frequency-selective</td></tr>
<tr><td>B — Audio amplifier</td><td>ERA-3SM MMIC + headphones</td><td>&minus;50&nbsp;dBm</td><td>Standalone; no RX needed</td></tr>
<tr><td>C — AD8307</td><td>Log detector + CYD display</td><td>&minus;70&nbsp;dBm</td><td>Digital null indication</td></tr>
</table>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>Value / Part</th><th>Quantity</th></tr>
<tr><td>Bridge transformer T1</td><td>Trifilar, BN-43-202, 6 turns #26 AWG</td><td>1</td></tr>
<tr><td>Fixed bridge resistors R1, R2</td><td>51&Omega; 1% metal film, 1/4W</td><td>2</td></tr>
<tr><td>Variable resistance R_cal</td><td>0&ndash;200&Omega; wirewound pot (non-inductive)</td><td>1</td></tr>
<tr><td>Variable reactance X_cal</td><td>0&ndash;100 pF variable cap + 0&ndash;50&micro;H roller inductor</td><td>1 each</td></tr>
<tr><td>Noise source MMIC</td><td>MAR-6SM or ERA-3SM</td><td>1</td></tr>
<tr><td>Zener D1</td><td>BZX55C5V1, 5.1V 500mW</td><td>1</td></tr>
<tr><td>RF bypass capacitors</td><td>100 pF NP0</td><td>4</td></tr>
<tr><td>RF choke RFC1</td><td>10&micro;H SRF &gt;50&nbsp;MHz</td><td>1</td></tr>
<tr><td>Connectors</td><td>BNC female: noise out, DET, ANT</td><td>3</td></tr>
<tr><td>Power</td><td>9V battery (80 mA typical)</td><td>1</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<h3>4-1 Transformer Winding</h3>
<ol>
<li>Wind three identical windings of 6 turns #26 AWG simultaneously on a
    BN-43-202 binocular core.  Keep the three windings in the same rotational
    direction (color-code the wire starts).</li>
<li>Check winding: with an ohmmeter, verify all three windings are equal
    resistance (&plusmn;0.1&Omega;) and have no shorts between windings.</li>
<li>Test leakage inductance: connect one winding to NanoVNA port 1 with
    port 2 shorted to the other two windings.  Leakage inductance should be
    &lt;0.1&micro;H at 10&nbsp;MHz.</li>
</ol>
<h3>4-2 Calibration Dials</h3>
<p>Mark the R_cal potentiometer dial in 10&Omega; steps from 0 to 200&Omega; using
a resistance meter.  Mark the X_cal capacitor dial in terms of &minus;jX at
10&nbsp;MHz using the formula X_C = 1/(2&pi;fC).  Mark the inductor in terms
of +jX at 10&nbsp;MHz using X_L = 2&pi;fL.  Actual X depends on operating
frequency; provide a reactance chart (see Appendix A) for other frequencies.</p>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<h3>5-1 Measuring Antenna Impedance</h3>
<ol>
<li>Connect antenna to ANT BNC.  Connect communications receiver (tuned to
    a clear frequency in the antenna's operating range) to DET BNC.</li>
<li>Enable noise source (power switch ON).  Receiver S-meter should show
    S3&ndash;S7 noise level.  If no noise: check battery, MMIC bias.</li>
<li>Set R_cal to 50&Omega; (center scale) and X_cal to zero (cap at minimum,
    inductor at minimum).</li>
<li>Adjust R_cal for minimum S-meter reading.  Note: the null may be sharp;
    sweep slowly.</li>
<li>If null is imperfect at all R_cal values, the antenna has significant
    reactance.  Adjust X_cal (capacitive for a high-SWR dipole that is
    too long; inductive for one that is too short) until the null is
    satisfactorily deep (&ge;10&nbsp;dB below noise floor).</li>
<li>Read R and X from the calibration dials.  Reported impedance:
    Z_ant = R_cal + jX_cal at the measurement frequency.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Connect a precision 50&Omega; non-inductive resistor to ANT port.  Set
    R_cal = 50&Omega;, X_cal = 0.  Verify null at &ge;20&nbsp;dB below noise floor.
    If null is &lt;20&nbsp;dB: re-check bridge transformer balance.</li>
<li>Connect a known 100&Omega; resistor.  Set R_cal = 100&Omega;, X_cal = 0.
    Verify null is achieved at that setting.</li>
<li>Connect a 47&nbsp;pF NP0 capacitor.  At 10&nbsp;MHz:
    X_C = 1/(2&pi;&times;10e6&times;47e-12) = &minus;338&Omega;.
    Set R_cal = 0&Omega; (resistor is pure reactance), X_cal to
    &minus;338&Omega; (capacitive).  Verify null.</li>
<li>Record calibration date and reference components used in log.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Measure a known 50&Omega; dummy load: R must read 50 &plusmn; 3&Omega;,
    X must read 0 &plusmn; 10&Omega;.</li>
<li>Measure a known antenna of known resonant frequency (verified by NanoVNA).
    At resonance, X should read 0 &plusmn; 15&Omega;; R should agree with NanoVNA
    feedpoint resistance within 10%.</li>
<li>Log: date, reference impedances measured, null depth achieved, measured
    vs. expected values, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Reactance vs. Frequency Chart</h2>
<table>
<tr><th>Component</th><th>1 MHz</th><th>7 MHz</th><th>14 MHz</th><th>28 MHz</th></tr>
<tr><td>47 pF cap</td><td>&minus;3386&Omega;</td><td>&minus;484&Omega;</td><td>&minus;242&Omega;</td><td>&minus;121&Omega;</td></tr>
<tr><td>100 pF cap</td><td>&minus;1592&Omega;</td><td>&minus;227&Omega;</td><td>&minus;114&Omega;</td><td>&minus;57&Omega;</td></tr>
<tr><td>1 &micro;H inductor</td><td>+6.3&Omega;</td><td>+44&Omega;</td><td>+88&Omega;</td><td>+176&Omega;</td></tr>
<tr><td>10 &micro;H inductor</td><td>+63&Omega;</td><td>+440&Omega;</td><td>+879&Omega;</td><td>+1759&Omega;</td></tr>
</table>

<h2 class="chapter-heading" id="appb">Appendix B — Worked Example</h2>
<p>Measuring a 40m dipole at 7.100&nbsp;MHz.  Null achieved at R_cal = 72&Omega;,
X_cal = +18&Omega; (inductive).  This means the dipole is slightly too long
(excess inductive reactance).  To resonate it: shorten each element by:</p>
<pre>&Delta;L = &lambda; &times; (X / (2 &times; R)) &times; k   (approximate)
&Delta;L = 42.3m &times; (18 / 144) &times; 0.97 = 2.5 cm per side</pre>
""")

# ── TM-TOOL-007  RF Power Meter (QRP compact) ─────────────────────────────────
TOMS['rf_power_meter'] = dict(
    tm_num="TM-TOOL-007",
    title="COMPACT PORTABLE RF POWER METER",
    subtitle="All 15 Ham Bands, 1.8 MHz–1.3 GHz, 1W–1 kW, Dual HF/VHF/UHF Section",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers a compact portable RF power meter covering all fifteen
amateur radio bands from 160 meters (1.8&nbsp;MHz) through 20&nbsp;cm
(1.3&nbsp;GHz).  The meter uses two directional coupler sections optimized
separately for HF (1.8&ndash;30&nbsp;MHz, toroid directional coupler) and
VHF/UHF (50&ndash;1300&nbsp;MHz, PCB microstrip coupler).  Power ranges are
switchable: 1W, 10W, 100W, 1000W.  Accuracy: &plusmn;5% typical, &plusmn;10%
worst case.  Insertion loss: &lt;0.1&nbsp;dB.  Size: 160&times;100&times;50&nbsp;mm.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Directional Coupler Principle</h3>
<p>A toroid transformer (T1) with 1 primary turn (the center conductor of
the transmission line) and N secondary turns couples a fraction of the
forward and reflected waves.  The coupling factor C (dB) = &minus;20 log10(N)
for a toroid coupler.  Typical values:</p>
<table>
<tr><th>Power range</th><th>N2 turns</th><th>Coupling</th><th>Detector input (at full scale)</th></tr>
<tr><td>1000W</td><td>32</td><td>&minus;30 dB</td><td>1.0W = 0 dBm</td></tr>
<tr><td>100W</td><td>10</td><td>&minus;20 dB</td><td>100W &rarr; 1 mW = 0 dBm</td></tr>
<tr><td>10W</td><td>10</td><td>&minus;20 dB</td><td>10W &rarr; 0.1 mW</td></tr>
<tr><td>1W</td><td>3</td><td>&minus;10 dB</td><td>1W &rarr; 0.1W</td></tr>
</table>
<h3>2-2 Schottky Diode Detector</h3>
<p>1N5711 Schottky diodes detect the coupled RF.  Forward voltage ~0.25V
at low signal levels (square-law region: V_out &prop; P_in) and ~0.4V at
high levels (linear region: V_out &prop; V_in).  The ADS1115 16-bit ADC
samples V_fwd and V_ref; firmware converts to power using a
calibration table stored in NVS.</p>
<h3>2-3 SWR Computation</h3>
<pre>|&Gamma;| = sqrt(P_ref / P_fwd)
SWR = (1 + |&Gamma;|) / (1 &minus; |&Gamma;|)</pre>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>Value / Part</th><th>Purpose</th></tr>
<tr><td>HF coupler core</td><td>FT-114-43 or BN-43-202</td><td>1.8&ndash;30 MHz coupling</td></tr>
<tr><td>VHF/UHF coupler</td><td>PCB microstrip, FR4</td><td>50&ndash;1300 MHz coupling</td></tr>
<tr><td>Detector diodes</td><td>1N5711 Schottky (4&times;)</td><td>RF to DC conversion</td></tr>
<tr><td>ADC</td><td>ADS1115 16-bit I2C</td><td>DC voltage measurement</td></tr>
<tr><td>Controller</td><td>ESP32 WROOM-32</td><td>ADC readout, display driver</td></tr>
<tr><td>Display</td><td>CYD 2.8&rdquo; ILI9341</td><td>Power, SWR, band display</td></tr>
<tr><td>RF connectors</td><td>N-type female (×2)</td><td>Input/output (IN, OUT)</td></tr>
<tr><td>Band switch</td><td>Front-panel toggle</td><td>HF / VHF-UHF section select</td></tr>
<tr><td>Range switch</td><td>4-position rotary</td><td>1W / 10W / 100W / 1000W</td></tr>
<tr><td>Power</td><td>9V battery or USB-C</td><td>ESP32, ADC, display</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<h3>4-1 HF Toroid Winding</h3>
<ol>
<li>Thread the main transmission line center conductor through the toroid
    core once (1 primary turn).</li>
<li>Wind the secondary winding on the same core: N2 turns (select per Table
    2-1 for desired power range).  Use #26 AWG Teflon-insulated wire.</li>
<li>Wind FWD and REF termination resistors (R1, R2 = 51&Omega; 1%) directly
    at the secondary winding ends.  Lead length &le;10&nbsp;mm each.</li>
</ol>
<h3>4-2 PCB Microstrip VHF/UHF Coupler</h3>
<p>The microstrip directional coupler for 50&ndash;1300&nbsp;MHz is a PCB
structure: two parallel 50&Omega; microstrip traces (2.9&nbsp;mm wide on 1.6&nbsp;mm
FR4) separated by a 0.1&nbsp;mm gap and coupled over a &lambda;/4 length at the
band center.  Design frequency: 700&nbsp;MHz (&lambda;/4 at 700&nbsp;MHz in FR4
= 28&nbsp;mm).  See PCB layout guide for Gerber file dimensions.</p>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<ol>
<li>Connect transmitter to IN (N-type), antenna (or dummy load) to OUT.
    Select HF or VHF/UHF section.  Select power range (start at highest
    range and reduce until needle is &gt;30% FSD).</li>
<li>Key transmitter.  CYD displays forward power (W), reflected power (W),
    and SWR in real time.  Update rate: 10&nbsp;Hz.</li>
<li>For digital modes (FT8, WSPR): set display to AVERAGE mode (5-second
    rolling average) to see mean power through the duty cycle.</li>
<li>At end of session: record peak power, SWR, and band in operating log.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Connect a calibrated RF power source (TinySA in generator mode, or
    signal generator with known output) to the IN port.  Load OUT with
    a calibrated 50&Omega; dummy load.</li>
<li>Apply &minus;10&nbsp;dBm, 0&nbsp;dBm, +10&nbsp;dBm, +20&nbsp;dBm
    at 14&nbsp;MHz (HF calibration).  Record ADC voltage vs. known power.</li>
<li>Fit a calibration curve (linear or polynomial) to the V_det vs. P_in
    data.  Store coefficients in ESP32 NVS.  Accuracy after calibration:
    &plusmn;5% over the calibrated range.</li>
<li>Repeat calibration procedure at 144&nbsp;MHz for the VHF/UHF section.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Apply known 5W at 14.175&nbsp;MHz.  Display must show 5.0 &plusmn; 0.3W.</li>
<li>Apply known 5W at 145.0&nbsp;MHz.  Display must show 5.0 &plusmn; 0.3W.</li>
<li>Short OUT connector (SWR = &infin;).  Verify CYD shows SWR &ge;99:1
    and forward/reflected power are approximately equal.</li>
<li>Insertion loss verification: compare power reading with coupler in-line
    vs. direct connection.  Difference must be &lt;0.2&nbsp;dB.</li>
<li>Log: date, calibration source, power levels checked, measured vs. expected
    values, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Power / SWR Quick-Reference</h2>
<table>
<tr><th>SWR</th><th>|&Gamma;|</th><th>Return Loss (dB)</th><th>% Power Reflected</th></tr>
<tr><td>1.0:1</td><td>0.000</td><td>&infin;</td><td>0%</td></tr>
<tr><td>1.5:1</td><td>0.200</td><td>14 dB</td><td>4%</td></tr>
<tr><td>2.0:1</td><td>0.333</td><td>9.5 dB</td><td>11%</td></tr>
<tr><td>3.0:1</td><td>0.500</td><td>6 dB</td><td>25%</td></tr>
<tr><td>5.0:1</td><td>0.667</td><td>3.5 dB</td><td>44%</td></tr>
</table>

<h2 class="chapter-heading" id="appb">Appendix B — Toroid Core Selection Guide</h2>
<table>
<tr><th>Core</th><th>Material</th><th>HF range</th><th>Note</th></tr>
<tr><td>FT-114-43</td><td>#43 ferrite</td><td>1&ndash;100 MHz</td><td>Best for 1.8&ndash;30 MHz coupling</td></tr>
<tr><td>BN-43-202</td><td>#43 ferrite</td><td>1&ndash;100 MHz</td><td>Binocular; good alt for T1</td></tr>
<tr><td>FT-50-61</td><td>#61 ferrite</td><td>10&ndash;200 MHz</td><td>Better for 50&ndash;150 MHz</td></tr>
<tr><td>FT-50-67</td><td>#67 ferrite</td><td>50&ndash;500 MHz</td><td>VHF/UHF auxiliary coupler</td></tr>
</table>
""")

# ── TM-TOOL-008  RF Power Meters (General) ────────────────────────────────────
TOMS['rf_power_meters'] = dict(
    tm_num="TM-TOOL-008",
    title="RF POWER METERS — TYPES AND OPERATION",
    subtitle="Bird Model 43 Style, Inline Directional, and Terminating Wattmeters",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers three categories of RF power meter in common amateur
use: the Bird Model 43 slug-type (directional, interchangeable elements),
the inline directional wattmeter (fixed range, inline measurement), and the
terminating wattmeter (power absorbed by a built-in load).  Operating
frequency range: 1.8&nbsp;MHz to 1.3&nbsp;GHz depending on element / design.
Power ranges: 1W through 3&nbsp;kW.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Bird Model 43 Slug Type</h3>
<p>The Bird 43 is an inline directional wattmeter using interchangeable
plug-in elements (slugs).  Each slug contains a toroid directional coupler
wound for a specific frequency range and power range.  The slug's flat face
orientation determines forward vs. reflected reading: flat toward the meter
face = forward power; flat away = reflected.  Output is a DC voltage
proportional to forward (or reflected) power, driving an analog panel meter.</p>
<p>Slug PCB dimensions: 25.0&nbsp;mm OD &times; 25.4&nbsp;mm length.  Retention:
O-ring at body midpoint.  Contact: 2&nbsp;mm banana pin.  The flatted-D
cross-section prevents 90&deg; orientation errors.</p>
<h3>2-2 Inline Directional Meter</h3>
<p>Uses a toroid coupler (1-turn primary = center conductor; N-turn secondary)
plus Schottky diode detectors (1N5711) for forward and reflected channels.
An ADS1115 16-bit I2C ADC digitizes both channels; ESP32 computes forward
power, reflected power, and SWR in firmware.  CYD display shows real-time
readings.  Typical frequency range 1.8&ndash;600&nbsp;MHz with appropriate
coupler design.</p>
<h3>2-3 Terminating Wattmeter</h3>
<p>A terminating wattmeter absorbs all transmitted power in an internal
50&Omega; load (dry or oil-cooled) and measures the absorbed power via
a coupler before the load.  Because there is no transmitted power beyond
the instrument, no antenna is needed during testing.  Used for final
amplifier output power verification and transmitter tuning.</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<h3>3-1 HF Plug-In Element (Bird 43 Compatible)</h3>
<p>Slug body: 6061-T6 aluminum, black anodized, D-shaped cross section.
PCB: 22&times;18&nbsp;mm FR4.  Key components per power/band range:</p>
<table>
<tr><th>Range</th><th>N2 turns</th><th>Core</th><th>Frequency</th></tr>
<tr><td>100W / HF</td><td>6</td><td>Fair-Rite 5943000401 (#43, 9mm)</td><td>1.8&ndash;30 MHz</td></tr>
<tr><td>250W / HF</td><td>10</td><td>Same</td><td>1.8&ndash;30 MHz</td></tr>
<tr><td>1000W / HF</td><td>32</td><td>BN-43-202</td><td>1.8&ndash;30 MHz</td></tr>
<tr><td>100W / VHF</td><td>6</td><td>Fair-Rite #67, 9mm</td><td>50&ndash;175 MHz</td></tr>
<tr><td>100W / UHF</td><td>Microstrip coupled</td><td>PCB</td><td>420&ndash;1000 MHz</td></tr>
</table>
<h3>3-2 Bird 43 Body</h3>
<p>The Bird 43 body (if homebrew): SO-239 or N-type input/output connectors
on a milled aluminum body with a 25.0&nbsp;mm diameter slug cavity.  Spring-pin
contact engages the slug banana jack.  Retention sleeve threads onto body.</p>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Slug Construction</h2>
<ol>
<li>Mill slug body to dimensions (OD 25.0&nbsp;mm, length 25.4&nbsp;mm).
    Machine the D-flat on one side.  Drill and tap center hole for contact
    pin (2&nbsp;mm banana jack, gold-plated).</li>
<li>Fabricate PCB (22&times;18&nbsp;mm).  Drill 3&nbsp;mm center hole for
    slug body contact.  Route coupler, diode, and filter on PCB.</li>
<li>Wind secondary winding on core per Table 3-1.  Solder to PCB pads.</li>
<li>Install O-ring in groove.  Press PCB into slug body; solder banana pin.</li>
<li>Mark power range and frequency on flat face (laser engraving or paint
    pen).</li>
</ol>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<h3>5-1 Bird 43 Use</h3>
<ol>
<li>Insert correct slug for operating frequency and expected power level.
    Orient flat face for forward or reflected measurement.</li>
<li>Connect IN to transmitter, OUT to antenna (or dummy load).  Key
    transmitter at target power.  Read panel meter.  For reflected power:
    rotate slug 180&deg; (flat face away from meter).</li>
<li>Compute SWR from P_fwd and P_ref readings.</li>
</ol>
<h3>5-2 Inline Digital Meter</h3>
<ol>
<li>Insert meter inline between transmitter and antenna (any orientation;
    no slug rotation needed).</li>
<li>CYD displays P_fwd (W), P_ref (W), SWR simultaneously.  Log readings
    to SD card if installed.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Connect calibrated power source to IN; calibrated 50&Omega; load to OUT.</li>
<li>Apply 10W at 14.175&nbsp;MHz.  Panel meter (Bird 43) or CYD should read
    10 &plusmn; 1W.  Adjust calibration pot R_cal (Bird 43 body) if needed.
    For digital inline meter: update calibration coefficient in NVS.</li>
<li>Verify at 50W and 100W.  Response must be linear within &plusmn;5%.</li>
<li>Repeat for each slug / frequency range in use.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>Power measurement within &plusmn;5% of reference (calibrated signal source)
    at all test frequencies.</li>
<li>SWR reading within &plusmn;0.1 of calculated SWR for a known mismatch
    (e.g., 100&Omega; load gives SWR 2.0:1 into 50&Omega; system).</li>
<li>Insertion loss with slug installed: &lt;0.1&nbsp;dB measured by comparing
    received signal with and without meter inline.</li>
<li>Log: date, slug serial number (or homebrew tag), frequency, measured
    power vs. reference, SWR accuracy, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Common Bird 43 Slug Equivalents</h2>
<table>
<tr><th>Bird part</th><th>Power</th><th>Frequency</th><th>Homebrew N2</th></tr>
<tr><td>5H</td><td>5W</td><td>25&ndash;60 MHz</td><td>6 turns, #67 core</td></tr>
<tr><td>50H</td><td>50W</td><td>25&ndash;60 MHz</td><td>10 turns, #67 core</td></tr>
<tr><td>100D</td><td>100W</td><td>400&ndash;1000 MHz</td><td>Microstrip PCB element</td></tr>
<tr><td>250B</td><td>250W</td><td>2&ndash;30 MHz</td><td>10 turns, #43 core</td></tr>
</table>

<h2 class="chapter-heading" id="appb">Appendix B — SWR from P_fwd / P_ref</h2>
<pre>|&Gamma;| = sqrt(P_ref / P_fwd)
SWR = (1 + |&Gamma;|) / (1 &minus; |&Gamma;|)

Example: P_fwd = 100W, P_ref = 5W
|&Gamma;| = sqrt(5/100) = sqrt(0.05) = 0.224
SWR = (1.224) / (0.776) = 1.58:1</pre>
""")

# ── TM-TOOL-009  SDR Interfaces ───────────────────────────────────────────────
TOMS['sdr_interfaces'] = dict(
    tm_num="TM-TOOL-009",
    title="SDR INTERFACES AND PRESELECTOR",
    subtitle="HF Upconverter, 16-Band Preselector, Bias-T, Attenuator/LNA, Direct Sampling",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers five SDR interface circuits that extend or improve
the performance of RTL-SDR and similar low-cost SDR receivers:
(1)&nbsp;HF upconverter (0&ndash;30&nbsp;MHz to 125&ndash;155&nbsp;MHz),
(2)&nbsp;16-band switchable bandpass preselector,
(3)&nbsp;dual-band bias-T for remote LNA power,
(4)&nbsp;PE4302 digital step attenuator with SPF5189Z LNA option, and
(5)&nbsp;direct-sampling HF modification.  All circuits interface via
SMA connectors; the preselector and attenuator are ESP32-controlled.</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 HF Upconverter</h3>
<p>An SA612A double-balanced mixer converts HF input (0.1&ndash;30&nbsp;MHz)
to IF (125.1&ndash;155&nbsp;MHz) by mixing with a 125.000&nbsp;MHz TCXO local
oscillator.  The RTL-SDR is then tuned to 125&ndash;155&nbsp;MHz to receive
HF signals without needing direct-sampling mode.  An SPDT relay bypasses
the upconverter for 6m and above (where RTL-SDR tunes directly).
Optional SPF5189Z LNA (+19&nbsp;dB, NF = 0.6&nbsp;dB) precedes the mixer.</p>
<table>
<tr><th>Parameter</th><th>Value</th></tr>
<tr><td>Input frequency</td><td>0.1&ndash;30 MHz</td></tr>
<tr><td>LO frequency</td><td>125.000 MHz (&plusmn;2.5 ppm TCXO)</td></tr>
<tr><td>IF output</td><td>125.1&ndash;155 MHz</td></tr>
<tr><td>Conversion gain (passive)</td><td>&minus;2 dB</td></tr>
<tr><td>Conversion gain (with LNA)</td><td>+17 dB</td></tr>
<tr><td>Image frequency</td><td>&gt;155 MHz (suppressed &gt;45 dB by IF BPF)</td></tr>
<tr><td>LO leakage to RF port</td><td>&lt;&minus;40 dBm</td></tr>
</table>

<h3>2-2 16-Band Switchable Preselector</h3>
<p>Sixteen bandpass filters (one per amateur band, 160m through 23cm plus
one bypass) select with an SP4T&times;4 relay tree driven by a 4-to-16 decoder
(74HC154) from ESP32 GPIO[3:0].  Filter topology: 3-pole Chebyshev BPF
for 160m&ndash;10m; interdigital for 2m/1.25m; stripline/SAW for 70cm and
above.  Rejection of out-of-band signals: &ge;50&nbsp;dB adjacent band,
&ge;60&nbsp;dB for FM broadcast (88&ndash;108&nbsp;MHz) when tuned to HF.</p>

<h3>2-3 Bias-T</h3>
<p>A bias-T injects DC power onto coax to supply a remote LNA or active
antenna.  An RF choke (L, &gt;10&micro;H SRF) passes DC while blocking RF;
a DC-blocking capacitor (C) passes RF while blocking DC.  The HF version
(Version A) covers 0.1&ndash;50&nbsp;MHz; the VHF/UHF version (Version B)
covers 10&nbsp;MHz&ndash;3&nbsp;GHz.</p>

<h3>2-4 PE4302 Digital Step Attenuator</h3>
<p>The PE4302 provides 0&ndash;31.5&nbsp;dB attenuation in 0.5&nbsp;dB steps
via 6-bit SPI control.  Frequency range DC&ndash;3&nbsp;GHz, IP3 = +40&nbsp;dBm,
NF = attenuation setting + 0.5&nbsp;dB.  Combined with an SPF5189Z LNA
(+19&nbsp;dB gain, NF = 0.6&nbsp;dB), the combination provides adjustable
gain from &minus;31.5&nbsp;dB (full atten) to +18.5&nbsp;dB (no atten, LNA on).</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Module</th><th>Key ICs</th><th>Connectors</th><th>Supply</th></tr>
<tr><td>HF Upconverter</td><td>SA612A, 125 MHz TCXO, K1 relay</td><td>SMA in/out</td><td>5V USB, 80 mA</td></tr>
<tr><td>Preselector</td><td>74HC154, 4&times; SP4T relays, 16 BPF modules</td><td>SMA in/out</td><td>5V, 200 mA</td></tr>
<tr><td>Bias-T (HF)</td><td>L = 10&micro;H RFC, C = 100 nF</td><td>SMA in/out/DC</td><td>5V or 12V from SDR</td></tr>
<tr><td>Bias-T (VHF)</td><td>L = 100 nH, C = 10 nF NP0</td><td>SMA</td><td>3.3V/5V/12V</td></tr>
<tr><td>Attenuator/LNA</td><td>PE4302, SPF5189Z</td><td>SMA in/out</td><td>3.3V</td></tr>
<tr><td>Controller</td><td>ESP32 WROOM-32</td><td>USB-C</td><td>5V</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Installation and Setup</h2>
<h3>4-1 Stacking Order</h3>
<p>Connect SDR interface modules in this order from antenna to SDR:</p>
<pre>Antenna &rarr; Bias-T (if using remote LNA) &rarr; Preselector &rarr;
Attenuator/LNA (optional) &rarr; HF Upconverter (HF only) &rarr; RTL-SDR</pre>
<h3>4-2 Preselector Band Selection</h3>
<p>The ESP32 selects the correct filter automatically if configured with
the current receive frequency.  Manual override: set GPIO[3:0] to the
filter index (0&ndash;15) using the CYD touchscreen or serial command
<code>BAND &lt;n&gt;</code>.</p>
<h3>4-3 Upconverter LO Alignment</h3>
<p>The RTL-SDR must be told to offset the displayed frequency by the LO
value.  In SDR# or GQRX: set &ldquo;LO offset&rdquo; or &ldquo;frequency correction&rdquo;
to +125.000&nbsp;MHz.  In GNU Radio: subtract 125e6 from the reported
frequency to get the true HF frequency.</p>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<h3>5-1 HF Reception with Upconverter</h3>
<ol>
<li>Enable bypass relay for frequencies &ge;60&nbsp;MHz (GPIO or front-panel
    switch).  For 0.1&ndash;30&nbsp;MHz: use upconverter path.</li>
<li>In SDR software: tune to 125 + f_HF (MHz).  E.g., 40m = 7.15&nbsp;MHz
    &rarr; tune SDR to 132.15&nbsp;MHz.</li>
<li>Enable preselector for the band in use to reduce out-of-band
    interference.  FM broadcast (88&ndash;108&nbsp;MHz) causes severe IMD
    in RTL-SDR at HF; preselector eliminates this.</li>
</ol>
<h3>5-2 Adjusting Gain</h3>
<ol>
<li>Start with maximum attenuation (31.5&nbsp;dB) on PE4302.  Reduce in
    6&nbsp;dB steps until the noise floor drops and desired signals become
    visible.</li>
<li>Enable SPF5189Z LNA only for weak-signal work (&lt;&minus;100&nbsp;dBm);
    for strong-signal environments (near broadcast transmitters), keep
    LNA off to avoid IMD.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Inject a known signal (&minus;50&nbsp;dBm at 14.175&nbsp;MHz) into the
    upconverter input.  Verify the SDR displays the signal at the correct
    displayed frequency (offset by LO).</li>
<li>Preselector: verify each filter passes its intended band within 1&nbsp;dB
    of bypass, and rejects adjacent bands by &ge;40&nbsp;dB.</li>
<li>Attenuator: apply 0&nbsp;dBm CW; step through 0, 10, 20, 30&nbsp;dB
    settings.  Verify SDR S-meter drops by 10&nbsp;dB per step
    (&plusmn;1.5&nbsp;dB).</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>WWV reception check: with upconverter active and 40m preselector
    selected, verify WWV at 5.000&nbsp;MHz or 10.000&nbsp;MHz is audible
    at noise floor SNR &ge;15&nbsp;dB.</li>
<li>FM rejection: verify FM broadcast stations at 88&ndash;108&nbsp;MHz are
    not visible while preselector is set to any HF band (&ge;60&nbsp;dB
    suppression relative to bypass mode).</li>
<li>Log: date, upconverter LO error (measured vs. GPS reference), preselector
    insertion loss at band centers, attenuator step accuracy, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — SDR Software LO Offset Settings</h2>
<table>
<tr><th>Software</th><th>Setting</th><th>Value for 125 MHz LO</th></tr>
<tr><td>SDR#</td><td>Shift frequency</td><td>+125000000 Hz</td></tr>
<tr><td>GQRX</td><td>LNB LO (Tools &rarr; Offset)</td><td>125.000 MHz</td></tr>
<tr><td>GNU Radio</td><td>Frequency subtract block</td><td>125e6 Hz offset in flowgraph</td></tr>
<tr><td>CubicSDR</td><td>Not supported natively; use IF math</td><td>—</td></tr>
</table>

<h2 class="chapter-heading" id="appb">Appendix B — Image Frequency Calculator</h2>
<pre>For any LO:
  Image freq = LO + f_if_desired = LO + (LO + f_hf) = 2&times;LO + f_hf
  With LO = 125 MHz, f_hf = 7.15 MHz:
  f_image = 2&times;125 + 7.15 = 257.15 MHz
  This is above the RTL-SDR's tuning range &rarr; image is rejected
  by the IF BPF at 125&ndash;155 MHz (&gt;45 dB suppression).</pre>
""")

# ── TM-TOOL-010  SWR Meter ────────────────────────────────────────────────────
TOMS['swr_meter'] = dict(
    tm_num="TM-TOOL-010",
    title="SWR METER — CONSTRUCTION AND USE",
    subtitle="Dual Directional Coupler, HF/VHF Coverage, Analog and Digital Display",
    body="""
<h2 class="chapter-heading" id="ch1">Chapter 1 — Introduction and Scope</h2>
<p>This manual covers construction and operation of a compact portable SWR
meter covering all fifteen amateur radio bands from 160m (1.8&nbsp;MHz) through
20cm (1.3&nbsp;GHz).  Two directional coupler sections — a tandem-match
toroid coupler for HF (1.8&ndash;30&nbsp;MHz) and a PCB microstrip coupler
for VHF/UHF (50&ndash;1300&nbsp;MHz) — sample forward and reflected power.
Schottky diode detectors convert RF to DC; a meter driver circuit drives
an analog panel meter (or digital display showing SWR numerically).</p>

<h2 class="chapter-heading" id="ch2">Chapter 2 — Theory of Operation</h2>
<h3>2-1 Tandem-Match Directional Coupler (HF)</h3>
<p>Two toroids wound on BN-43-202 or FT-50-43 cores provide &minus;30&nbsp;dB
coupling.  The forward arm measures V_fwd (proportional to forward wave
voltage); the reflected arm measures V_ref (proportional to reflected wave).
Termination resistors (51&Omega; 1%) on each secondary provide the
directionality; directivity is typically &gt;35&nbsp;dB across 1.8&ndash;30&nbsp;MHz.</p>
<h3>2-2 Diode Detector Circuits</h3>
<p>1N5711 Schottky diodes detect the coupled RF.  In the square-law region
(V_in &lt; 50&nbsp;mV peak): V_dc &prop; P_rf.  In the linear region
(V_in &gt; 100&nbsp;mV): V_dc &prop; V_rf.  The meter driver compensates
for detector law by adjusting the meter scale or applying a lookup table
in firmware.</p>
<h3>2-3 SWR Calculation</h3>
<pre>SWR = (V_fwd + V_ref) / (V_fwd &minus; V_ref)   [analog cross-needle meter]
SWR = (1 + sqrt(P_ref/P_fwd)) / (1 &minus; sqrt(P_ref/P_fwd))   [digital meter]</pre>
<h3>2-4 Range Switching</h3>
<p>Resistive dividers on the detector outputs set the effective full-scale
power range.  Switching from 100W to 10W range adds 10&times; attenuation
before the panel meter, maintaining accuracy across the power range.  Six
ranges: 5W, 20W, 50W, 200W, 500W, 1500W (switchable by front-panel rotary).</p>

<h2 class="chapter-heading" id="ch3">Chapter 3 — Equipment and Materials</h2>
<table>
<tr><th>Component</th><th>HF Section</th><th>VHF/UHF Section</th></tr>
<tr><td>Coupler cores</td><td>2&times; BN-43-202</td><td>PCB microstrip (FR4)</td></tr>
<tr><td>Secondary winding</td><td>20 turns #28 AWG each</td><td>Coupled line, 0.1mm gap</td></tr>
<tr><td>Termination resistors</td><td>51&Omega; 1% metal film</td><td>51&Omega; 0402 SMD</td></tr>
<tr><td>Detector diodes</td><td>1N5711 (×4)</td><td>BAT42 SMD (×4)</td></tr>
<tr><td>Filter caps</td><td>10 nF, 10&micro;F</td><td>100 pF NP0, 10 nF</td></tr>
<tr><td>Meter driver</td><td>LM324 op-amp</td><td>Same</td></tr>
<tr><td>Panel meter</td><td>100&micro;A FSD (×2)</td><td>CYD digital (optional)</td></tr>
<tr><td>Range switch</td><td>6-position rotary</td><td>Shared with HF section</td></tr>
<tr><td>Section switch</td><td>DPDT toggle HF/VHF</td><td>Same</td></tr>
<tr><td>Connectors</td><td>SO-239 female (×2) or N</td><td>N-type (×2)</td></tr>
</table>

<h2 class="chapter-heading" id="ch4">Chapter 4 — Construction</h2>
<h3>4-1 HF Toroid Coupler Winding</h3>
<ol>
<li>Wind each BN-43-202 core with 20 turns #28 AWG Teflon wire.  Mark
    start of winding (dot convention: current into dot = positive).</li>
<li>For the forward coupler: connect winding start (dot end) to
    termination R1 (51&Omega;); winding finish to forward detector D1 (1N5711
    cathode).  The free R1 end connects to the center conductor bus.</li>
<li>For the reflected coupler: reverse winding connections &mdash; winding
    start connects to reflected detector D2; winding finish to R2 (51&Omega;)
    to center conductor bus.  This reversal makes the circuit directional.</li>
<li>Thread the coax center conductor through both toroid cores (1 primary
    turn each) before mounting in the chassis.</li>
</ol>
<h3>4-2 Meter Driver</h3>
<p>An LM324 quad op-amp provides two independent channels (FWD and REF)
with adjustable zero and gain.  Configuration: non-inverting amplifier with
gain set by R_gain (start at gain = 10); output drives panel meter through
a 4.7&nbsp;k&Omega; series resistor.  Adjust R_zero (10-turn pot) for
zero reading at no RF input.</p>
<h3>4-3 Calibration Circuits</h3>
<p>A precision 100&Omega; trimmer pot in the FWD channel allows full-scale
calibration with a known power level.  A separate trimmer in the REF channel
permits null balancing with a matched load (zero reflected power should give
zero meter deflection on the REF meter).</p>

<h2 class="chapter-heading" id="ch5">Chapter 5 — Operating Procedures</h2>
<ol>
<li>Select HF or VHF section switch.  Select power range (start high;
    reduce until FWD needle is at 50&ndash;80% FSD for best accuracy).</li>
<li>Connect transmitter to IN (SO-239 or N-type); antenna to OUT.</li>
<li>Key transmitter briefly (1&ndash;2 seconds).  Read FWD and REF needles.</li>
<li>Compute SWR: set the FWD meter scale to its reference mark; read SWR
    directly from the ratio scale, or compute from the formula in §2-3.</li>
<li>For antenna tuning: adjust tuner while watching REF needle.  Minimum
    REF deflection = maximum power transfer = lowest SWR.</li>
</ol>

<h2 class="chapter-heading" id="ch6">Chapter 6 — Calibration</h2>
<ol>
<li>Connect a calibrated 50&Omega; dummy load to OUT.  Apply known RF power
    (e.g., 10W at 14.175&nbsp;MHz from a calibrated transmitter).</li>
<li>Adjust FWD calibration trimmer until FWD meter reads exactly full scale
    for the selected range (e.g., at the 10W mark on the 10W range).</li>
<li>Verify REF meter reads zero (or minimum).  Adjust REF null trimmer if
    deflection &gt;2% of FSD.</li>
<li>Apply a known 100&Omega; load (SWR 2.0:1).  Verify SWR reads
    2.0 &plusmn; 0.2.</li>
<li>Repeat calibration procedure for VHF section at 145&nbsp;MHz.</li>
</ol>

<h2 class="chapter-heading" id="ch7">Chapter 7 — Verification and Acceptance</h2>
<ol>
<li>SWR accuracy: measured SWR must be within &plusmn;0.1 SWR units
    for SWR &lt;3:1; within &plusmn;10% for SWR 3:1&ndash;10:1.</li>
<li>Power accuracy: forward power reading within &plusmn;5% of actual power
    (verified with calibrated power reference).</li>
<li>Directivity check: with 50&Omega; load on OUT, REF deflection must be
    &lt;2% of FWD deflection at full power (indicates &ge;34&nbsp;dB directivity).</li>
<li>Insertion loss: &lt;0.1&nbsp;dB at 1.8&ndash;30&nbsp;MHz; &lt;0.2&nbsp;dB
    at 50&ndash;150&nbsp;MHz; &lt;0.5&nbsp;dB at 150&ndash;450&nbsp;MHz.</li>
<li>Log: date, calibration load used, forward calibration power, SWR accuracy
    check results, insertion loss, operator.</li>
</ol>

<h2 class="chapter-heading" id="appa">Appendix A — Toroid Core Selection</h2>
<table>
<tr><th>Core</th><th>Material</th><th>Al (nH/turn²)</th><th>Best range</th></tr>
<tr><td>BN-43-202</td><td>#43 ferrite</td><td>189</td><td>1.8&ndash;50 MHz</td></tr>
<tr><td>FT-50-43</td><td>#43 ferrite</td><td>523</td><td>1.8&ndash;50 MHz (higher impedance)</td></tr>
<tr><td>FT-50-61</td><td>#61 ferrite</td><td>69</td><td>10&ndash;200 MHz</td></tr>
<tr><td>FT-50-67</td><td>#67 ferrite</td><td>40</td><td>50&ndash;500 MHz</td></tr>
</table>

<h2 class="chapter-heading" id="appb">Appendix B — Worked SWR Example</h2>
<p>FWD meter reads 8.5 divisions on 10W scale = 8.5W.  REF meter reads
0.85 divisions on 10W scale = 0.85W.</p>
<pre>|&Gamma;| = sqrt(0.85 / 8.5) = sqrt(0.1) = 0.316
SWR = (1 + 0.316) / (1 &minus; 0.316) = 1.316 / 0.684 = 1.92:1</pre>
<p>Acceptable for most antenna systems; fine-tune if SWR &gt;2.0:1 desired.</p>
""")


# ─── Write files ──────────────────────────────────────────────────────────────

if __name__ == "__main__":
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
    print(f"\nDone. {len(TOMS)} TMs written.")
