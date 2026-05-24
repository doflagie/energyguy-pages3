# TECHNICAL MANUAL
## AUTOMATED NEC ANTENNA MODELING WORKFLOW SYSTEM
### TM-ANT-MODEL-001 Revision A

---

**DISTRIBUTION STATEMENT:** Unrestricted. Approved for public release.

**WARNING:** This system controls RF transmitting equipment. Ensure transmitter output is reduced to safe levels during antenna tuner operations. High SWR conditions may damage transmitter finals. Do not operate tuner while transmitting at power levels above 10W until match is confirmed.

---

## TABLE OF CONTENTS

- Chapter 1 — Introduction and Theory
- Chapter 2 — System Installation and Configuration
- Chapter 3 — Antenna Modeling Procedures
- Chapter 4 — Batch Processing and Multiband Optimization
- Chapter 5 — Optimization Algorithms
- Chapter 6 — Web API Operation
- Chapter 7 — ESP32 Automatic Antenna Tuner
- Chapter 8 — CYD NEC Viewer Operation
- Chapter 9 — Calibration and Verification
- Chapter 10 — Troubleshooting
- Appendix A — NEC Card Reference
- Appendix B — Antenna Modeling Standards
- Appendix C — Glossary
- Appendix D — Abbreviations and Acronyms
- Appendix E — Python API Reference
- Appendix F — Example NEC Files

---

# CHAPTER 1 — INTRODUCTION AND THEORY

## 1.1 Purpose and Scope

### 1.1.1 Purpose

This manual provides installation, operating, and calibration instructions for the Automated NEC Antenna Modeling Workflow System, document number TM-ANT-MODEL-001. The system automates the generation, execution, and post-processing of antenna models using the Numerical Electromagnetics Code (NEC) method-of-moments engine. Hardware integration extends the system to control a physical L-network antenna tuner via ESP32 microcontroller and provides real-time display of modeling results on a CYD (ESP32 + ILI9341 TFT) touchscreen.

### 1.1.2 Scope

This manual covers:

- (a) NEC file generation from parametric antenna specifications
- (b) Automated frequency sweep and radiation pattern generation
- (c) CSV and 3D export of antenna performance data
- (d) Genetic Algorithm (GA) and Particle Swarm Optimization (PSO) for antenna geometry tuning
- (e) Cloud-accessible REST API for remote modeling jobs
- (f) ESP32 firmware for NEC model-assisted automatic antenna tuning
- (g) CYD touchscreen firmware for results visualization

This manual does not cover antenna construction, tower installation, or RF safety beyond operational precautions.

## 1.2 System Overview

### 1.2.1 System Architecture

The system consists of four interconnected subsystems:

```
┌─────────────────────────────────────────────────────┐
│              HOST COMPUTER (Linux)                  │
│  ┌──────────────┐  ┌──────────────┐  ┌───────────┐ │
│  │ nec_generator│  │ freq_sweep   │  │ optimizer │ │
│  │ nec_runner   │  │ nec_parser   │  │ GA / PSO  │ │
│  │ pattern_3d   │  │ csv_export   │  │           │ │
│  └──────┬───────┘  └──────┬───────┘  └─────┬─────┘ │
│         └────────────────►│◄────────────────┘       │
│                    ┌──────┴───────┐                  │
│                    │  web_api.py  │ Port 8000        │
│                    │  (FastAPI)   │                  │
│                    └──────┬───────┘                  │
└───────────────────────────┼─────────────────────────┘
                            │ WiFi / LAN
              ┌─────────────┼─────────────┐
              ▼                           ▼
   ┌─────────────────────┐   ┌─────────────────────────┐
   │  ESP32 Auto-Tuner   │   │    CYD NEC Viewer        │
   │  - L/C stepper ctrl │   │    - SWR vs freq chart   │
   │  - SWR detector ADC │   │    - Polar pattern plot  │
   │  - NVS tune map     │   │    - Opt convergence     │
   │  - BT Serial ctrl   │   │    - Live tuner status   │
   └──────────┬──────────┘   └─────────────────────────┘
              │ RF
   ┌──────────┴──────────┐
   │   L-Network Tuner   │
   │   + Antenna System  │
   └─────────────────────┘
```

### 1.2.2 Software Components

**Table 1-1. Software Module Inventory**

| File | TM Number | Function |
|------|-----------|----------|
| `nec_generator.py` | TM-MODEL-001 | Parametric NEC file generation |
| `nec_runner.py` | TM-RUNNER-001 | NEC engine subprocess control |
| `nec_parser.py` | TM-PARSER-001 | NEC output file parsing |
| `freq_sweep.py` | TM-SWEEP-001 | Automated frequency sweep |
| `csv_export.py` | TM-CSV-001 | CSV and HTML report export |
| `pattern_3d.py` | TM-PATTERN-001 | 3D radiation pattern export |
| `genetic_optimizer.py` | TM-GA-001 | Genetic algorithm optimizer |
| `pso_optimizer.py` | TM-PSO-001 | Particle swarm optimizer |
| `batch_optimizer.py` | TM-BATCH-001 | Batch job management |
| `web_api.py` | TM-API-001 | REST API server (FastAPI) |
| `esp32_tuner.ino` | TM-TUNER-001 | ESP32 antenna tuner firmware |
| `cyd_nec_viewer.ino` | TM-CYD-001 | CYD touchscreen viewer firmware |

## 1.3 Applicable Documents and References

The following documents are referenced in this manual:

- NEC-2 User's Guide (Burke, Poggio; LLNL NEC-2 Part III)
- NEC-4 User's Manual (Burke; LLNL)
- Stutzman, W.L. & Thiele, G.A., *Antenna Theory and Design*, 3rd Ed.
- Balanis, C.A., *Antenna Theory: Analysis and Design*, 4th Ed.
- ARRL Antenna Book, 24th Ed.
- Milligan, T.A., *Modern Antenna Design*, 2nd Ed.
- Clerc, M. & Kennedy, J., "The Particle Swarm — Explosion, Stability, and Convergence," IEEE TEC Vol. 6 No. 1, 2002
- MIL-STD-498, Software Development and Documentation

## 1.4 NEC Method of Moments Theory

### 1.4.1 Integral Equation Formulation

NEC solves for currents on a wire structure by enforcing the boundary condition that the tangential electric field on a perfect electric conductor (PEC) wire surface is zero. This yields the Electric Field Integral Equation (EFIE):

```
E_tan^inc(r) = -E_tan^scat(r)  for r on wire surface
```

The scattered field is produced by the unknown current distribution **J**(r') flowing on the wire. NEC expands J in a set of basis functions (sinusoidal + constant segments) and applies Galerkin's method to produce a matrix equation:

```
[Z] · [I] = [V]
```

Where:
- [Z] is the N×N impedance matrix (mutual and self impedances of all wire segments)
- [I] is the vector of unknown segment currents
- [V] is the excitation vector (voltages at source segments)

Solving this system (LU decomposition for small N, iterative methods for large N) yields the current on every wire segment, from which far-field radiation patterns, near-field, and input impedance are computed.

**NOTE:** The accuracy of NEC results is directly dependent on the quality of the wire segmentation. See paragraph 1.4.2 for segmentation guidelines.

### 1.4.2 Wire Segmentation Guidelines

Correct segmentation is the most common source of modeling errors. The following guidelines shall be followed:

**(1)** Segment length shall not exceed λ/10 at the highest frequency of interest.

**(2)** Segment length shall not be less than approximately twice the wire radius (length/radius ratio ≥ 2, recommended ≥ 10).

**(3)** Adjacent connected segments shall not differ in length by more than a factor of 2.

**(4)** Segments at junctions (T-junctions, crosses) shall be of equal or nearly equal length for accurate current junction modeling.

**(5)** Use an odd number of segments per half-element for dipoles and Yagi elements. This places a segment center at the element center, which is where the feed and peak current reside.

**(6)** The function `recommended_segments()` in `nec_generator.py` applies guideline (1) automatically. Verify results with a segmentation convergence test (see paragraph 9.1.3).

**Table 1-2. Recommended Segment Counts**

| Frequency | λ (m) | λ/10 (m) | Dipole segments (each half) |
|-----------|-------|-----------|------------------------------|
| 1.8 MHz | 166.5 | 16.6 | 5–11 |
| 7.15 MHz | 41.9 | 4.19 | 11–15 |
| 14.25 MHz | 21.0 | 2.10 | 11–15 |
| 144.2 MHz | 2.08 | 0.208 | 11–21 |
| 432 MHz | 0.694 | 0.069 | 11–21 |

### 1.4.3 NEC2 vs NEC4

NEC-2 is the public-domain version. NEC-4 is a licensed upgrade developed at Lawrence Livermore National Laboratory. Key differences are:

**Table 1-3. NEC2 vs NEC4 Feature Comparison**

| Feature | NEC-2 | NEC-4 |
|---------|-------|-------|
| Buried wires | Not supported | Supported (LD5 card) |
| Insulated wire coating | Not supported | Supported |
| Wire-to-ground junction | Approximate | Improved |
| Multi-wire junctions | Limited accuracy | Improved |
| License | Public domain | Government license required |
| Linux binary | `nec2c` (apt) | Separate install |

For typical above-ground antenna modeling (dipoles, Yagis, phased arrays, LPDAs), NEC-2 and NEC-4 produce equivalent results. Use NEC-4 only when buried radials or insulated wire coating is critical to the model.

### 1.4.4 Ground Modeling

NEC provides three ground models, selected by the GN card:

**(1) Free Space (GN -1):** No ground. Use for antennas far from ground, upper HF Yagis, VHF/UHF arrays, optimization runs where speed is critical.

**(2) Perfect Ground (GN 1):** Infinite perfectly conducting ground plane. Computationally fast. Provides upper bound on gain for horizontally polarized antennas; lower bound for verticals. Acceptable approximation for VHF monopoles with good radial systems.

**(3) Sommerfeld-Norton Real Ground (GN 2):** Frequency-dependent complex ground with permittivity εr and conductivity σ. Required for accurate results with ground-mounted verticals, low dipoles (< 0.3λ height), and field strength predictions.

**Table 1-4. Typical Ground Parameters**

| Ground Type | εr | σ (S/m) | NEC GN Card |
|-------------|-----|---------|-------------|
| Sea water | 80 | 5.0 | `GN 2 0 0 0 80.0 5.0` |
| Rich farm soil | 20 | 0.03 | `GN 2 0 0 0 20.0 0.030` |
| Good soil | 13 | 0.005 | `GN 2 0 0 0 13.0 0.005` |
| Average soil | 13 | 0.002 | `GN 2 0 0 0 13.0 0.002` |
| Poor/rocky | 7 | 0.001 | `GN 2 0 0 0 7.0 0.001` |
| City (concrete) | 5 | 0.001 | `GN 2 0 0 0 5.0 0.001` |

**NOTE:** The Merced, CA area (San Joaquin Valley floor) is predominantly clay loam with εr ≈ 13–20, σ ≈ 0.003–0.010 S/m depending on moisture content. Use εr=15, σ=0.005 as a conservative average for field work.

### 1.4.5 Frequency Limitations

NEC is a thin-wire code. The following limits apply:

- Minimum wire radius: approximately λ/1000 at the highest frequency modeled
- Maximum wire radius: approximately λ/100 (below this, the thin-wire approximation degrades)
- Maximum crossing angle between connected wires: 30° (shallower angles degrade junction model)
- Minimum crossing angle for unconnected crossing wires (T-junctions coded with separate tags): 45°

### 1.4.6 Engine Selection

Use `NECEngine.AUTO` in `nec_runner.py` for automatic detection. The search order is: `nec2c`, `nec2`, `nec4`, `necpp`. Override with `NECRunner(executable_path="/usr/bin/nec2c")`.

4NEC2 is a Windows GUI application. Under Linux it runs via Wine but is not recommended for automated batch processing. Use `nec2c` for all scripted operations.

## 1.5 Antenna Performance Parameters

### 1.5.1 Gain and Directivity

**Gain (G)** includes the effect of losses; **Directivity (D)** does not. The relationship is:

```
G = η · D
```

where η is the radiation efficiency (0 < η ≤ 1).

Both are expressed in dBi (decibels relative to an isotropic radiator) or dBd (relative to a half-wave dipole; 0 dBd = 2.15 dBi).

NEC reports **power gain** (includes all specified losses) by default when the RP card parameter XNDA = 0. Use XNDA = 1 for directive gain.

### 1.5.2 Radiation Pattern

The radiation pattern describes gain as a function of direction (θ, φ) in spherical coordinates:

- **θ (theta):** elevation angle from Z-axis (zenith). θ=0 is straight up; θ=90° is horizontal.
- **φ (phi):** azimuth angle from X-axis (East). φ=0°=East, φ=90°=North.

Key pattern cuts:
- **E-plane:** plane containing the electric field vector and the direction of maximum radiation
- **H-plane:** plane perpendicular to E-plane, containing the magnetic field vector
- **Azimuth pattern (φ sweep):** horizontal directivity, typically at θ=90° (horizontal plane)
- **Elevation pattern (θ sweep):** vertical directivity showing **takeoff angle** (critical for HF skip)

**Takeoff angle** (also called elevation angle of maximum radiation, or EMRA) is the angle above the horizon of peak gain. Lower takeoff angles favor long-distance (DX) propagation.

### 1.5.3 Front-to-Back Ratio

The **front-to-back (F/B) ratio** compares forward gain to the gain 180° behind:

```
F/B (dB) = G_forward(dBi) - G_back(dBi)
```

High F/B (>20 dB) is important for:
- Rejecting interference from behind the beam
- Reducing QRM on crowded bands
- Phased arrays used for RDF (radio direction finding)

The `calc_fb_ratio()` method in `nec_parser.py` computes this from the parsed radiation pattern array.

### 1.5.4 Impedance

The complex feed impedance is Z = R + jX (ohms). At resonance, X = 0 and Z = R_rad + R_loss, where R_rad is the radiation resistance.

Typical resonant impedances:
- Half-wave dipole in free space: ~73Ω
- Half-wave dipole at λ/4 height over real ground: 50–90Ω (height-dependent)
- Quarter-wave vertical over perfect ground: ~36Ω
- Quarter-wave vertical over 32 radials, real ground: ~50Ω

### 1.5.5 SWR and Return Loss

The **Voltage Standing Wave Ratio (VSWR)**, commonly called SWR, measures the quality of impedance match between antenna and feedline:

```
Γ = (Z_L - Z_0) / (Z_L + Z_0)      (reflection coefficient)

SWR = (1 + |Γ|) / (1 - |Γ|)        (ranges from 1.0 to ∞)

Return Loss = -20 · log₁₀(|Γ|)  dB  (larger = better match)
```

Where Z_L is the load (antenna) impedance and Z_0 is the system characteristic impedance (typically 50Ω).

**Table 1-5. SWR to Return Loss Conversion**

| SWR | |Γ| | Return Loss (dB) | Power Reflected (%) |
|-----|------|-----------------|---------------------|
| 1.0 | 0.000 | ∞ | 0.0 |
| 1.5 | 0.200 | 14.0 | 4.0 |
| 2.0 | 0.333 | 9.5 | 11.1 |
| 3.0 | 0.500 | 6.0 | 25.0 |
| 5.0 | 0.667 | 3.5 | 44.4 |
| 10.0 | 0.818 | 1.7 | 66.9 |

### 1.5.6 Bandwidth

Bandwidth definitions vary by application:

- **2:1 SWR bandwidth:** frequency range where SWR ≤ 2.0 (most common for HF)
- **3dB gain bandwidth:** range where gain ≥ peak - 3dB (for directional antennas)
- **Pattern bandwidth:** range where pattern shape remains acceptable

`SweepResult.swr_bandwidth_mhz()` computes the 2:1 SWR bandwidth automatically from sweep data.

### 1.5.7 Efficiency

**Radiation efficiency** η = P_radiated / P_input. NEC computes this when conductor losses (LD cards) or ground losses are included. For lossless wire models it returns 100%.

In practice:
- Resonant dipoles with good conductor: 95–98%
- Short loaded whips: 20–60% depending on loading coil Q
- Stealth wire with resistive loss: calculated per load specification

---

# CHAPTER 2 — SYSTEM INSTALLATION AND CONFIGURATION

## 2.1 Hardware Requirements

**Table 2-1. Minimum Host System Requirements**

| Resource | Minimum | Recommended |
|----------|---------|-------------|
| OS | Debian 12 / Ubuntu 22.04 | Debian Forky (testing) |
| CPU | x86_64 or ARM64, 2 cores | 4+ cores for parallel sweeps |
| RAM | 2 GB | 8 GB |
| Disk | 500 MB | 2 GB |
| Network | LAN for API service | |
| Python | 3.10 | 3.11+ |

## 2.2 NEC Engine Installation

### 2.2.1 Installing nec2c on Debian/Ubuntu

**(1)** Update package list:
```bash
sudo apt update
```

**(2)** Install nec2c:
```bash
sudo apt install nec2c
```

**(3)** Verify installation:
```bash
which nec2c
nec2c --version 2>&1 | head -3
```

Expected output: path `/usr/bin/nec2c` and version string.

### 2.2.2 Verifying NEC2 Operation

**(1)** Create a test file `/tmp/test_dipole.nec`:
```
CM Test dipole 14.25 MHz
CE
GW 1 11 -5.0 0.0 10.0 5.0 0.0 10.0 0.001
GE 0
EX 0 1 6 0 1.0 0.0
FR 0 1 0 0 14.25 0
RP 0 37 73 1000 0 0 5 5
EN
```

**(2)** Run NEC:
```bash
nec2c /tmp/test_dipole.nec /tmp/test_dipole.out
```

**(3)** Verify output file exists and contains `ANTENNA INPUT PARAMETERS`:
```bash
grep "ANTENNA INPUT PARAMETERS" /tmp/test_dipole.out
```

**CAUTION:** If nec2c produces a zero-length output file or exits with non-zero return code, review input file syntax. Common errors: missing `CE` card, missing `GE` card, incorrect card column spacing. See Table 10-2 for error codes.

### 2.2.3 NEC4 Installation

NEC-4 requires a LLNL license. After receiving the license and source distribution:

**(1)** Compile with Fortran compiler (gfortran):
```bash
gfortran -O2 -o nec4 nec4.f
```

**(2)** Install to `/usr/local/bin`:
```bash
sudo cp nec4 /usr/local/bin/
```

**(3)** Configure runner:
```python
runner = NECRunner(engine=NECEngine.NEC4, executable_path="/usr/local/bin/nec4")
```

## 2.3 Python Environment Setup

### 2.3.1 Virtual Environment Creation

**(1)** Navigate to antenna modeling directory:
```bash
cd /home/merv/Public/claude/ant_gear/antenna_modeling
```

**(2)** Create virtual environment:
```bash
python3 -m venv venv
```

**(3)** Activate environment:
```bash
source venv/bin/activate
```

### 2.3.2 Installing Dependencies

**(1)** Install from requirements.txt:
```bash
pip install -r scripts/requirements.txt
```

**(2)** Verify key packages:
```bash
python -c "import numpy, pandas, fastapi, matplotlib; print('OK')"
```

### 2.3.3 Self-Test

**(1)** Run nec_generator self-test:
```bash
python scripts/nec_generator.py
```

Expected output: "Self-test complete." with file paths for test NEC files.

**(2)** Test NEC runner:
```bash
python -c "
from scripts.nec_runner import NECRunner
r = NECRunner()
print('Engine:', r.engine.value, r.executable)
"
```

## 2.4 Web API Server Deployment

### 2.4.1 Local Development Deployment

**(1)** Activate virtual environment (see paragraph 2.3.1).

**(2)** Start API server:
```bash
python scripts/web_api.py
```

**(3)** Verify at `http://localhost:8000/health`:
```bash
curl http://localhost:8000/health
```

Expected: `{"status":"ok","nec_engine":"nec2c",...}`

### 2.4.2 Systemd Service

**(1)** Create service file `/etc/systemd/system/nec-api.service`:
```ini
[Unit]
Description=NEC Antenna Modeling API
After=network.target

[Service]
User=merv
WorkingDirectory=/home/merv/Public/claude/ant_gear/antenna_modeling
ExecStart=/home/merv/Public/claude/ant_gear/antenna_modeling/venv/bin/uvicorn \
    scripts.web_api:app --host 0.0.0.0 --port 8000
Restart=on-failure
RestartSec=5

[Install]
WantedBy=multi-user.target
```

**(2)** Enable and start:
```bash
sudo systemctl daemon-reload
sudo systemctl enable nec-api
sudo systemctl start nec-api
sudo systemctl status nec-api
```

### 2.4.3 Firewall Configuration

**(1)** Allow API port through firewall:
```bash
sudo ufw allow 8000/tcp comment "NEC API"
```

**(2)** Restrict to LAN only (recommended):
```bash
sudo ufw allow from 192.168.1.0/24 to any port 8000
```

## 2.5 ESP32 Tuner Firmware Installation

### 2.5.1 Arduino IDE Setup

**(1)** Install Arduino IDE 2.x from arduino.cc.

**(2)** Add ESP32 board package: File → Preferences → Additional Boards Manager URLs:
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

**(3)** Install: Tools → Board → Boards Manager → search "esp32" → install "esp32 by Espressif Systems" version ≥ 3.0.

### 2.5.2 Required Libraries

Install via Arduino Library Manager (Tools → Manage Libraries):

| Library | Author | Version |
|---------|--------|---------|
| ArduinoJson | Benoit Blanchon | ≥ 7.0 |

Built-in (ESP32 core): WiFi, HTTPClient, BluetoothSerial, Preferences, ArduinoOTA

### 2.5.3 config.h Modification

**(1)** Open `firmware/esp32_tuner/config.h`.

**(2)** Set WiFi credentials:
```cpp
#define WIFI_SSID        "YOUR_ACTUAL_SSID"
#define WIFI_PASSWORD    "YOUR_ACTUAL_PASSWORD"
```

**(3)** Set API server IP:
```cpp
#define API_HOST         "192.168.1.100"   // IP of host running web_api.py
```

**(4)** Verify pin assignments match your hardware wiring. See Table 2-2.

**Table 2-2. ESP32 Tuner Pin Assignments**

| GPIO | Signal | Direction | Notes |
|------|--------|-----------|-------|
| 18 | SR_CLK | Output | 74HC595 clock |
| 23 | SR_DATA | Output | 74HC595 data |
| 5 | SR_LATCH | Output | 74HC595 latch |
| 26 | CAP_STEP | Output | Stepper step pulse |
| 27 | CAP_DIR | Output | Stepper direction |
| 34 | CAP_HOME | Input (pullup) | Hall sensor, active LOW |
| 32 | SWR_FWD | ADC input | Forward detector (0–3.3V) |
| 33 | SWR_REF | ADC input | Reflected detector (0–3.3V) |
| 25 | BYPASS | Output | Bypass relay |

### 2.5.4 Flashing Procedure

**(1)** Connect ESP32 via USB.

**(2)** Select board: Tools → Board → ESP32 Arduino → ESP32 Dev Module.

**(3)** Select correct COM port.

**(4)** Set Upload Speed: 921600.

**(5)** Click Upload.

**(6)** Open Serial Monitor (115200 baud) and verify boot message:
```
=== TM-TUNER-001 NEC Auto-Tuner v1.0.0 ===
WiFi: connecting to YOURSSID...
WiFi: connected, IP=192.168.1.xxx
POST: Running self-test...
Homing capacitor stepper...
Cap: HOME OK
POST: PASS
```

### 2.5.5 OTA Updates

After initial flash, subsequent updates may be pushed over-the-air:

**(1)** In Arduino IDE: Tools → Port → Network Ports → select "AutoTuner".

**(2)** Password when prompted: `antenna2026` (set in `config.h` `OTA_PASSWORD`).

## 2.6 CYD NEC Viewer Firmware Installation

### 2.6.1 TFT_eSPI Library Configuration

The TFT_eSPI library requires manual configuration of `User_Setup.h` for the CYD hardware.

**(1)** Locate the library: typically `~/Arduino/libraries/TFT_eSPI/User_Setup.h`.

**(2)** Apply these settings (comment out any conflicting driver/pin definitions):
```cpp
#define ILI9341_DRIVER
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST  -1
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_MISO 12
#define TOUCH_CS 33
#define SPI_FREQUENCY       40000000
#define SPI_TOUCH_FREQUENCY  2500000
```

### 2.6.2 WiFi Configuration in cyd_nec_viewer.ino

Edit the WiFi credentials near the top of `cyd_nec_viewer.ino`:
```cpp
#define WIFI_SSID        "YOUR_SSID"
#define WIFI_PASSWORD    "YOUR_PASSWORD"
#define API_HOST         "192.168.1.100"
```

### 2.6.3 Flashing Procedure

Same as paragraph 2.5.4. Select the CYD's COM port. After flash, the display should show the splash screen followed by the main layout.

### 2.6.4 Touchscreen Calibration

**(1)** Touch and hold the top-left corner of the screen for 3 seconds.

**(2)** Follow the on-screen prompts to touch each calibration point.

**(3)** Calibration values are stored in NVS and persist across power cycles.

---

# CHAPTER 3 — ANTENNA MODELING PROCEDURES

## 3.1 NEC File Generation

### 3.1.1 Interactive Use (Python REPL)

**(1)** Activate virtual environment and start Python:
```bash
cd antenna_modeling && source venv/bin/activate && python3
```

**(2)** Build a 20m dipole at 10m height, write to file:
```python
from scripts.nec_generator import Dipole
ant = Dipole(freq_mhz=14.25, height_m=10.0)
model = ant.to_nec_model(ground_type=2)
model.add_pattern(theta_inc=5, phi_inc=5)
model.write("/tmp/dipole_20m.nec")
print(model.to_string())
```

**(3)** Inspect the generated file to verify geometry before running.

### 3.1.2 Parametric Antenna Types

**Table 3-1. Antenna Builder Classes**

| Class | Parameters | Notes |
|-------|-----------|-------|
| `Dipole` | freq_mhz, height_m, wire_radius_m, length_factor, orientation | orientation="X" or "Y" |
| `VerticalWithRadials` | freq_mhz, n_radials, radial_length_factor, radial_height_m, wire_radius_m | Use ground_type=2 |
| `Yagi` | freq_mhz, n_directors, height_m, wire_radius_m | Classic Uda dimensions |
| `LogPeriodic` | freq_low_mhz, freq_high_mhz, tau, sigma, height_m, wire_radius_m | See paragraph 5.5.2 |
| `QuadLoop` | freq_mhz, height_m, wire_radius_m, n_elements | n_elements=2 adds reflector |
| `PhasedArray` | freq_mhz, n_elements, spacing_m, phase_deg, current_ratio | Use ground_type=2 |

### 3.1.3 Ground Plane Selection

**Table 3-2. Ground Type Selection Guide**

| Antenna Type | Recommended ground_type | Notes |
|-------------|------------------------|-------|
| High Yagi (>0.5λ height) | 0 (free space) | Ground effect negligible |
| Low dipole (<0.3λ height) | 2 (Sommerfeld) | Ground coupling significant |
| Quarter-wave vertical | 2 (Sommerfeld) | Always use real ground |
| Horizontal loop, high | 0 or 2 | Compare both |
| Phased array | 2 | Ground interaction critical |
| VHF/UHF beam | 0 | Ground usually irrelevant |
| Optimization (for speed) | 0 | Switch to 2 for final verification |

### 3.1.4 Wire Segmentation Guidance

Use `recommended_segments()` automatically unless:

- The antenna contains a loaded element (coil, trap): increase segments near load
- The antenna has very short elements (VHF directors): check length/radius ratio
- Running a convergence test: vary segment count manually

Override example:
```python
model.add_wire(-5.0, 0, 10.0, 0, 0, 10.0, radius=0.001, segs=21, tag=1)
```

### 3.1.5 Excitation Configuration

For a single-source antenna, the EX card sets voltage (1V default) at the feed segment:
```python
model.add_excitation(tag=1, segment=11)   # Feed at center of tag 1
```

For phased arrays with multiple sources, the `PhasedArray` class sets complex voltages automatically based on phase_deg and current_ratio parameters.

### 3.1.6 Loading Elements

Add lumped R-L-C loads to model coils, traps, or resistive losses:
```python
# Loading coil: 10µH inductor at center of element
model.add_load(tag=3, segment=5, r=5.0, l=10e-6, c=0)
# Resistive loss: 2Ω distributed resistance
model.add_load(tag=1, segment=6, r=2.0)
```

**NOTE:** Capacitance C in NEC LD cards is in Farads. Use C=0 for pure R-L loads to avoid creating an R-L-C resonance that may not represent your physical loading element.

### 3.1.7 Transmission Line Modeling

The NT card models lossless transmission lines between two segments. Useful for modeling feedlines, phasing lines, and transmission-line transformers:
```python
# Quarter-wave 75Ω phasing line between elements
from nec_generator import NetworkCard
import math
wl = 299.792e6 / (7.15e6)
# Y-parameter of quarter-wave TL: Y = j/(Z0*tan(theta)) approx
# For exact modeling, compute Y11, Y12, Y22 from TL theory
model.networks.append(NetworkCard(
    tag1=1, seg1=1, tag2=2, seg2=1,
    y11r=0, y11i=0.02, y12r=0, y12i=-0.02, y22r=0, y22i=0.02
))
```

## 3.2 Running NEC Simulations

### 3.2.1 NECRunner Command Line

```python
from scripts.nec_runner import NECRunner, NECEngine
from scripts.nec_generator import Dipole

runner = NECRunner(engine=NECEngine.AUTO, keep_files=True)
ant    = Dipole(freq_mhz=14.25, height_m=10.0)
model  = ant.to_nec_model(ground_type=2)
result = runner.run(model)

print("Success:", result.success)
print("Duration:", result.duration_s, "s")
print("Output:", result.output_file)
```

### 3.2.2 Interpreting NEC Engine Output

Normal NEC output files contain these major sections in order:

1. **Structure Specification** — wire geometry echo
2. **Antenna Structure** — segment data
3. **Antenna Input Parameters** — feed impedance per frequency
4. **Radiation Patterns** — gain vs θ, φ
5. **Power Budget** — efficiency

A successful run produces all sections. A failed run may produce partial output or empty file.

### 3.2.3 Common NEC Errors

**Table 3-3. NEC Error Conditions**

| Symptom | Probable Cause | Corrective Action |
|---------|---------------|-------------------|
| Empty .out file | Input file format error | Check CE/GE cards present |
| "MATRIX SINGULAR" | Two wires overlap exactly | Check for duplicate GW cards |
| Infinite impedance | No excitation segment | Verify EX card tag/segment numbers |
| SWR = 99.9 | Z = near-open circuit | Feed not at resonant point; check geometry |
| Gain > 30 dBi | Unrealistic model | Check segmentation, ground type |
| Negative gain | Ohmic loss > radiation | Load resistance too high |
| All patterns = 0 | RP card missing/wrong | Add RP card before EN card |

### 3.2.4 Segmentation Warnings

NEC prints warnings in the output when:
- Segment length/diameter ratio < 2 (near-lumped approximation)
- Adjacent segments differ by >2:1 length

These warnings do not halt execution but indicate potentially degraded accuracy. Correct by adjusting segment count or wire radius.

## 3.3 Frequency Sweep Procedures

### 3.3.1 Sweep Configuration

```python
from scripts.freq_sweep import FreqSweep, SweepConfig
from scripts.nec_generator import Dipole

def my_builder(freq_mhz):
    ant = Dipole(freq_mhz=freq_mhz, height_m=10.0)
    m = ant.to_nec_model(ground_type=2)
    m.add_pattern(theta_inc=5, phi_inc=5)
    return m

cfg = SweepConfig(
    freq_start_mhz = 14.000,
    freq_stop_mhz  = 14.350,
    freq_step_mhz  = 0.025,
    pattern_inc_deg = 5.0,
    z0             = 50.0,
    engine_name    = "auto"
)
sweep  = FreqSweep(my_builder, cfg)
result = sweep.run()
```

### 3.3.2 Step Size Selection

**Table 3-4. Recommended Sweep Step Sizes**

| Band | Bandwidth (MHz) | Recommended Step | Points |
|------|-----------------|-----------------|--------|
| 160m (1.8–2.0) | 0.2 | 0.005 | 41 |
| 80m (3.5–4.0) | 0.5 | 0.010 | 51 |
| 40m (7.0–7.3) | 0.3 | 0.010 | 31 |
| 20m (14.0–14.35) | 0.35 | 0.025 | 15 |
| 15m (21.0–21.45) | 0.45 | 0.025 | 19 |
| 10m (28.0–29.7) | 1.7 | 0.050 | 35 |
| 6m (50–54) | 4.0 | 0.100 | 41 |
| 2m (144–148) | 4.0 | 0.100 | 41 |
| 70cm (420–450) | 30.0 | 0.500 | 61 |

### 3.3.3 Interpreting SWR Sweep Output

Print summary to console:
```python
print(result.summary())
```

Plot SWR vs frequency:
```python
result.plot_swr(save_path="swr_sweep.png")
```

Key metrics to examine:
- Minimum SWR and its frequency (resonant frequency)
- 2:1 SWR bandwidth (`result.swr_bandwidth_mhz()`)
- Shape of impedance locus (capacitive or inductive off-resonance)

### 3.3.4 Identifying Resonant Frequency

At resonance: X = 0 and SWR is minimum. In the sweep DataFrame:
```python
df = result.to_dataframe()
resonant_row = df.loc[df.x_ohm.abs().idxmin()]
print(f"Resonant freq: {resonant_row.freq_mhz:.3f} MHz, R={resonant_row.r_ohm:.1f}Ω")
```

## 3.4 Radiation Pattern Analysis

### 3.4.1 Exporting 3D Patterns

```python
from scripts.pattern_3d import Pattern3D

# Get pattern at best-gain frequency
best_pt = min(result.freq_points, key=lambda p: abs(p.freq_mhz - result.best_gain_freq()))
p3d = Pattern3D.from_freq_point(best_pt)
p3d.to_json("pattern.json", freq_mhz=best_pt.freq_mhz)
p3d.to_vtk("pattern.vtk")    # Import to ParaView
p3d.to_ply("pattern.ply")    # Import to Blender/MeshLab
```

### 3.4.2 Visualizing in Matplotlib

```python
p3d.to_matplotlib_3d(freq_mhz=best_pt.freq_mhz, cmap="plasma", show=True)
```

### 3.4.3 Takeoff Angle for HF Propagation

For HF DX, seek antennas with takeoff angle < 15°. Inspect the elevation pattern:
```python
# Find elevation angle (90° - theta) of max gain
print(f"Takeoff angle: {90 - best_pt.gain_theta_deg:.1f}°")
```

Low takeoff angles require high antenna height. Rule of thumb for dipoles: height > 0.4λ for takeoff angle < 25°.

---

# CHAPTER 4 — BATCH PROCESSING AND MULTIBAND OPTIMIZATION

## 4.1 Batch Job Configuration

```python
from scripts.batch_optimizer import BatchRunner
from scripts.freq_sweep import SweepConfig

runner = BatchRunner(n_workers=4, output_dir="./batch_results")
runner.add_job(
    {"type": "dipole", "freq_mhz": 14.25, "height_m": 10.0},
    SweepConfig(14.0, 14.35, 0.025)
)
runner.add_job(
    {"type": "yagi", "freq_mhz": 144.2, "n_directors": 5},
    SweepConfig(144.0, 148.0, 0.1)
)
results = runner.run_all()
```

Results are automatically written to CSV in `output_dir`. Each job gets its own file named by job_id.

## 4.2 Multiband Sweep

```python
bands = [
    (7.000,  7.300,  0.020, "40m"),
    (14.000, 14.350, 0.025, "20m"),
    (21.000, 21.450, 0.025, "15m"),
    (28.000, 29.700, 0.050, "10m"),
]
lpda_spec = {
    "type": "lpda", "freq_low_mhz": 7.0, "freq_high_mhz": 29.7,
    "tau": 0.9, "sigma": 0.07, "height_m": 12.0
}
df_multiband = runner.run_multiband_sweep(lpda_spec, bands)
print(df_multiband.groupby("band")[["swr_50","gain_dBi"]].describe())
```

## 4.3 Antenna Comparison Procedure

Compare multiple antenna types at a single frequency:

**(1)** Define all antenna specs to compare (see Table 4-1 example).

**(2)** Call `BatchRunner.compare_antennas()`:
```python
specs = [
    {"type": "dipole",   "freq_mhz": 14.25, "height_m": 10.0, "label": "Dipole_10m"},
    {"type": "dipole",   "freq_mhz": 14.25, "height_m": 20.0, "label": "Dipole_20m"},
    {"type": "yagi",     "freq_mhz": 14.25, "n_directors": 5,  "label": "Yagi_5el"},
    {"type": "quad",     "freq_mhz": 14.25, "n_elements": 2,   "label": "Quad_2el"},
]
comp = runner.compare_antennas(specs, freq_mhz=14.25)
print(comp.to_string(index=False))
```

**(3)** Sort by gain_dBi or SWR to identify best design.

---

# CHAPTER 5 — OPTIMIZATION ALGORITHMS

## 5.1 Optimization Theory Overview

### 5.1.1 Search Space and Objective Function

Antenna optimization seeks a vector **x** = [x₁, x₂, ..., xₙ] of geometry parameters (element lengths, spacings, wire radii) that maximizes a scalar objective function f(**x**). This function evaluates the NEC model performance at the candidate geometry.

The search space is bounded: each parameter xᵢ has minimum and maximum bounds defined by `GeneParameter.min_val` and `GeneParameter.max_val`. The space is generally non-convex with multiple local optima.

### 5.1.2 GA vs PSO: Guidance

**Table 5-1. Algorithm Selection Guide**

| Condition | Recommended Algorithm |
|-----------|----------------------|
| Unknown landscape, many local optima | GA (more exploratory) |
| Smooth objective, continuous parameters | PSO (faster convergence) |
| Integer constraints (tap positions, element count) | GA (natural support) |
| Limited run time (< 1 hour) | PSO (fewer evaluations for same quality) |
| Reproducible results required | Both (set random seed) |
| First run on a new antenna type | PSO for speed, then GA to verify |

### 5.1.3 Convergence Criteria

Both GA and PSO terminate when:
- Maximum iterations/generations reached (`n_generations` or `n_iterations`)
- No improvement in best fitness for `early_stop_gens` / `early_stop_iters` consecutive cycles

Monitor convergence plots to assess whether the algorithm has converged prematurely (plateau early in run) or has not converged (still improving at termination). If still improving: increase max iterations. If plateau is early: reduce population/swarm size and rerun, or check if objective function is too flat.

## 5.2 Genetic Algorithm Operation

### 5.2.1 Real-Valued Chromosomes

Each individual's chromosome is a real-valued vector of length N (N = number of `GeneParameter` objects). Values are clamped to [min_val, max_val] after every mutation and crossover operation.

### 5.2.2 Tournament Selection

k individuals are randomly drawn from the population; the one with the highest fitness is selected as a parent. Tournament size k controls selection pressure: k=2 is low pressure (diversity), k=7 is high pressure (exploitation). Default k=3 is a good balance.

### 5.2.3 Uniform Crossover

Each gene in the offspring chromosome is independently drawn from parent 1 or parent 2 with equal probability. This allows genes from both parents to combine freely, unlike one-point crossover which preserves contiguous gene blocks.

### 5.2.4 Gaussian Mutation

Each gene mutates with probability `mutation_rate`. The mutation adds Gaussian noise with σ = `sigma_fraction × parameter_range`. Default σ_fraction = 0.1 (10% of range). Reducing this late in a run can improve local refinement.

### 5.2.5 Population Size and Generation Count

**Table 5-2. GA Population Sizing Guide**

| Problem Size (params) | Population | Generations | NEC Evaluations |
|-----------------------|------------|-------------|-----------------|
| 2–4 | 20 | 50 | ~1000 |
| 5–8 | 30 | 75 | ~2250 |
| 9–15 | 50 | 100 | ~5000 |
| 16–30 | 75 | 150 | ~11250 |

NEC evaluations = population × generations (approximate; elitism reduces this slightly).

## 5.3 Particle Swarm Optimization

### 5.3.1 Velocity Update Equation

The PSO velocity update (each iteration, each particle):

```
v_{i}(t+1) = χ · [w·v_{i}(t) + c₁·r₁·(p_best_{i} - x_{i}(t))
                              + c₂·r₂·(g_best - x_{i}(t))]
```

Where:
- χ = constriction factor (keeps velocities from diverging)
- w = inertia weight (decays linearly from w_start to w_end)
- c₁ = cognitive coefficient (attraction to personal best)
- c₂ = social coefficient (attraction to global best)
- r₁, r₂ = random scalars uniformly drawn from [0,1]
- p_best = particle's personal best position
- g_best = swarm's global best position

### 5.3.2 Constriction Factor

The Clerc-Kennedy constriction factor is:

```
χ = 2 / |2 - φ - √(φ² - 4φ)|,   φ = c₁ + c₂
```

With c₁ = c₂ = 2.05, φ = 4.10, χ ≈ 0.7298. This guarantees convergence without velocity clamping (though clamping is still applied as a safety measure).

**NOTE:** Setting φ ≤ 4 gives χ = 1 (no constriction, equivalent to standard PSO). Always use φ > 4 with constriction enabled.

### 5.3.3 Swarm Size

For antenna optimization with NEC (expensive function evaluations): 20–30 particles is optimal. Larger swarms improve global search but at linear computational cost.

## 5.4 Optimization Procedures

### 5.4.1 Defining Optimization Parameters

```python
from scripts.genetic_optimizer import GeneParameter
import math

# For 2m Yagi, NEC wavelength at 144.2 MHz
wl = 299.792e6 / (144.2e6)

params = [
    GeneParameter("refl_hl",  0.47*wl/2, 0.53*wl/2),   # Reflector half-length
    GeneParameter("driv_hl",  0.44*wl/2, 0.50*wl/2),   # Driven half-length
    GeneParameter("dir1_hl",  0.42*wl/2, 0.46*wl/2),   # Director 1 HL
    GeneParameter("dir1_sp",  0.15*wl,   0.30*wl),      # Director 1 spacing
]
```

### 5.4.2 Setting Objective Weights

```python
from scripts.genetic_optimizer import FitnessFunction

objectives = {
    "swr_target":    1.5,    # Ideal SWR
    "swr_weight":    3.0,    # High weight: SWR is primary objective
    "gain_min_dBi":  10.0,   # Minimum acceptable gain
    "gain_weight":   2.0,
    "fb_min_dB":     20.0,   # Minimum F/B ratio
    "fb_weight":     1.5,
    "eff_min_pct":   80.0,
    "eff_weight":    0.5,
}
fitness_fn = FitnessFunction(objectives)
```

**NOTE:** Weights determine trade-offs when objectives conflict. If the optimizer finds high gain but poor SWR, increase `swr_weight` relative to `gain_weight`.

### 5.4.3 Running GA Optimization

```python
from scripts.genetic_optimizer import GeneticOptimizer
from scripts.nec_runner import NECRunner

runner = NECRunner()

def my_builder(genes):
    # Construct NECModel from genes dict
    ...

opt = GeneticOptimizer(
    params=params, fitness_fn=fitness_fn,
    model_builder=my_builder, runner=runner,
    pop_size=30, n_generations=75,
    checkpoint_path="ga_checkpoint.json"
)
result = opt.run()
print("Best fitness:", result.best_fitness)
print("Best genes:",   result.best_genes)
```

### 5.4.4 Running PSO Optimization

```python
from scripts.pso_optimizer import PSOOptimizer, PSOConfig

cfg = PSOConfig(n_particles=25, n_iterations=120, early_stop_iters=25)
opt = PSOOptimizer(params, fitness_fn, my_builder, runner, config=cfg)
result = opt.run()
```

### 5.4.5 Validating Optimization Results

After optimization completes:

**(1)** Build the final model from best genes:
```python
final_model = result.best_nec_model(my_builder)
final_model.add_pattern(theta_inc=2, phi_inc=2)  # Fine pattern for final check
```

**(2)** Run NEC and parse results:
```python
nec_result  = runner.run(final_model)
from scripts.nec_parser import NECOutputParser
pts = NECOutputParser().parse(nec_result.output_file)
print(f"Final SWR: {pts[0].swr_50:.2f}, Gain: {pts[0].gain_dbi_max:.1f} dBi")
```

**(3)** Compare to objectives. If any objective is unmet, either:
- Increase population/iteration count
- Expand parameter bounds
- Relax conflicting objective weights

---

# CHAPTER 6 — WEB API OPERATION

## 6.1 API Architecture

The web_api.py FastAPI service receives antenna spec and sweep requests, queues background NEC jobs, and returns results via polling. All jobs are asynchronous: submit, get job_id, poll for completion, retrieve results. This allows long-running optimizations without HTTP timeout.

Interactive API documentation (Swagger UI) is available at: `http://[host]:8000/docs`

## 6.2 Starting the API Server

```bash
python scripts/web_api.py
# Or via uvicorn directly:
uvicorn scripts.web_api:app --host 0.0.0.0 --port 8000
```

Check health:
```bash
curl http://localhost:8000/health
```

## 6.3 Submitting and Polling Jobs

### 6.3.1 Submit Sweep Job

```bash
curl -X POST http://localhost:8000/model/run \
  -H "Content-Type: application/json" \
  -d '{
    "spec": {"type":"dipole","freq_mhz":14.25,"height_m":10.0},
    "sweep": {"start":14.0,"stop":14.35,"step":0.025}
  }'
```

Response: `{"job_id":"abc12345","status_url":"/model/status/abc12345"}`

### 6.3.2 Poll Status

```bash
curl http://localhost:8000/model/status/abc12345
```

Response: `{"status":"running","progress_pct":45.0,"eta_s":12.3,...}`

### 6.3.3 Retrieve Results

```bash
# JSON results
curl http://localhost:8000/model/results/abc12345

# CSV download
curl http://localhost:8000/model/results/abc12345/csv -o results.csv

# 3D pattern at 14.225 MHz
curl http://localhost:8000/model/results/abc12345/pattern3d/14.225
```

### 6.3.4 Submit Optimization Job

```bash
curl -X POST http://localhost:8000/model/optimize \
  -H "Content-Type: application/json" \
  -d '{
    "spec_template": {"type":"yagi","freq_mhz":144.2},
    "params": [
      {"name":"dir1_hl","min":0.38,"max":0.46},
      {"name":"dir1_sp","min":0.15,"max":0.30}
    ],
    "objectives": {"swr_target":1.5,"gain_min_dBi":10.0},
    "algorithm": "pso"
  }'
```

---

# CHAPTER 7 — ESP32 AUTOMATIC ANTENNA TUNER

## 7.1 Hardware Description

The tuner implements an L-network topology. Two reactive elements (variable inductor L and variable capacitor C) provide the impedance transformation between the 50Ω transceiver output and the antenna feed point.

**L-Network Configuration (low-pass, shunt-C):**
```
TX ──┬── Series L ──────┬── ANT
     │                  │
     ═ Shunt C          ═ Shunt C (output)
     │                  │
    GND                GND
```

The inductor uses a toroidal core with relay-switched taps (8 positions, controlled by 74HC595 shift register). The capacitor uses a stepper motor driving a variable capacitor (200 steps = full travel).

## 7.2 L-Network Design Theory

The L-network can match any impedance Z_L to Z_0 = 50Ω provided |Z_L| is within the network's transformation range. Maximum transformation ratio is approximately Q²+1, where Q is the network Q factor.

For a shunt-C / series-L low-pass network:
```
C_shunt = (1/ω) · √((R_L - R_0) / (R_0 · R_L²))
L_series = √(R_0 · (R_L - R_0)) / ω
```

The 8-tap inductor provides discrete L values. The stepper capacitor provides continuous C over its range. The NEC model predicts antenna impedance Z_L at the feed point; the L-network design equations then compute target L and C values, which are converted to tap number and step count.

## 7.3 NEC-Assisted Tuning Strategy

**(1)** Operator specifies operating frequency.

**(2)** ESP32 queries web API for predicted antenna impedance at that frequency.

**(3)** Firmware computes target L and C from impedance prediction.

**(4)** Firmware steps to predicted position.

**(5)** Firmware measures SWR with ADC.

**(6)** Firmware performs fine search (±FINE_TUNE range) to minimize SWR.

**(7)** Final L/C position saved to NVS tune map.

This hybrid approach reduces search time vs blind stepping: NEC prediction typically gets within 1–2 taps of optimal, reducing fine-search time from minutes to seconds.

## 7.4 Operating Procedures

### 7.4.1 Initial Setup

**(1)** Apply power. Verify boot messages on Serial Monitor.

**(2)** Allow homing sequence to complete (stepper moves to home position).

**(3)** Confirm WiFi connection: "WiFi: connected, IP=x.x.x.x".

**(4)** Set antenna type: via BT Serial or USB Serial, send: `SETANT dipole`

### 7.4.2 Tuning to a Frequency

Via BT Serial (connect from phone/laptop to "AutoTuner"):
```
TUNE 14.225
```

Response: periodic JSON status reports showing SWR approaching target.

Via USB Serial: same command at 115200 baud.

### 7.4.3 Band Sweep Tuning

**CAUTION:** Transmit power should be reduced to ≤10W during band sweep. The tuner is seeking minimum SWR at each frequency; high VSWR during seeking may exceed transceiver PA ratings.

```
SWEEP 7.0 7.3 0.1
```

The tuner steps from 7.0 to 7.3 MHz in 100 kHz increments, tunes at each step, and stores results in NVS. Future tunes on this band retrieve the NVS map, skipping the coarse search.

### 7.4.4 Retrieving Tune Map

```
GETMAP
```

Returns CSV: `freq_khz,l_tap,c_steps`

---

# CHAPTER 8 — CYD NEC VIEWER OPERATION

## 8.1 Display Layout

The 320×240 display is divided into three zones:

```
┌────────────────────────────────┐  ← Row 0-19: Top bar (title, WiFi status)
│   TM-CYD-001  [SWEEP]    WiFi │
├────────────────────────────────┤  ← Row 20-199: Chart area (180px)
│                                │
│         CHART AREA             │
│                                │
├────────────────────────────────┤  ← Row 200-239: Mode buttons (40px)
│  SWEEP  │ PATTERN │  OPT │TUNE│
└────────────────────────────────┘
```

## 8.2 Mode Descriptions

### 8.2.1 SWEEP Mode

Displays SWR vs frequency (upper half) and gain vs frequency (lower half). Color coding: green SWR < 2.0, yellow 2.0–3.0, red > 3.0. Orange dashed line at 2:1 SWR reference.

Data source: polls `GET /model/results/{job_id}` every 2 seconds.

To set the active job ID: currently set by editing `currentJobId` in firmware, or future implementation via touch keyboard. A future revision will add a QR code scan to receive job_id.

### 8.2.2 PATTERN Mode

Displays 2D polar radiation pattern. Tap anywhere in the chart area to toggle between E-plane and H-plane. Concentric reference circles at -3, -10, -20 dB from peak. Cardinal direction labels (N/S/E/W).

### 8.2.3 OPT Mode

Displays optimization convergence: fitness score vs iteration/generation. Shows current best fitness and iteration count. Updates every 2 seconds during active optimization.

### 8.2.4 TUNE Mode

Displays live tuner status: SWR bargraph (1.0–5.0), current L-tap and C-position, bypass status. "TUNE NOW" button sends tune command to tuner API. Frequency +/- buttons adjust target frequency in 25 kHz steps.

## 8.3 Touchscreen Calibration

**(1)** Long-press (3 seconds) the top-left corner of the display.

**(2)** Touch the center of each crosshair marker as prompted.

**(3)** Calibration is complete when the main display reappears.

**(4)** If touch is erratic after calibration, repeat the procedure.

---

# CHAPTER 9 — CALIBRATION AND VERIFICATION

## 9.1 NEC Model Verification

### 9.1.1 Known-Good Test Cases

Use these benchmarks to verify correct NEC installation and parser operation:

**Table 9-1. Expected Results for Standard Antenna Models**

| Antenna | Frequency | Expected Gain (dBi) | Expected SWR (50Ω) | Reference |
|---------|-----------|--------------------|--------------------|-----------|
| Half-wave dipole, free space | Any | 2.15 | ~2.9 (73Ω) | Balanis Ch.4 |
| Half-wave dipole, h=λ/2, real gnd | 14.25 MHz | ~7.5–8.0 | Varies | ARRL Antenna Book |
| Quarter-wave vertical, perf. gnd | Any | 5.19 | ~1.44 (36Ω) | Stutzman Ch.3 |
| Quarter-wave vertical, 32 radials | 7.15 MHz | ~5.0 | ~1.0 (50Ω) | Measured |
| 3-el Yagi, free space | 144.2 MHz | ~8.5–9.0 | Varies | Yagi design tables |

**NOTE:** Gain values depend heavily on height above ground, ground constants, and exact geometry. Use free-space models for initial verification; compare against Balanis or Stutzman analytical results.

### 9.1.2 Running Verification

```python
from scripts.nec_generator import Dipole
from scripts.nec_runner import NECRunner
from scripts.nec_parser import NECOutputParser

# Standard free-space dipole
ant    = Dipole(freq_mhz=14.25, height_m=1000.0)   # Effectively free space
model  = ant.to_nec_model(ground_type=0)
model.add_pattern(theta_inc=5, phi_inc=5)
result = NECRunner().run(model)
pts    = NECOutputParser().parse(result.output_file)

# Expected: gain ≈ 2.15 dBi, SWR ≈ 2.9 (R≈73Ω)
print(f"Gain: {pts[0].gain_dbi_max:.2f} dBi  (expect ≈2.15)")
print(f"R:    {pts[0].r_ohm:.1f}Ω  (expect ≈73)")
```

Acceptable tolerance: ±0.2 dBi gain, ±5Ω impedance.

### 9.1.3 Segmentation Convergence Test

**(1)** Model the antenna with the default segment count.

**(2)** Double the segment count.

**(3)** Compare impedance and gain at the same frequency.

**(4)** If results change by > 0.5 dBi or > 10Ω, increase segments further until convergence.

```python
def test_convergence(freq_mhz=14.25, height_m=10.0):
    results = []
    for seg_mult in [0.5, 1.0, 2.0, 4.0]:
        from scripts.nec_generator import Dipole, recommended_segments
        ant = Dipole(freq_mhz=freq_mhz, height_m=height_m)
        # Manually set segments
        model = ant.to_nec_model()
        for w in model.wires:
            w.segs = max(3, int(w.segs * seg_mult))
        result = NECRunner().run(model)
        pts = NECOutputParser().parse(result.output_file)
        if pts:
            results.append((int(sum(w.segs for w in model.wires)),
                           pts[0].r_ohm, pts[0].gain_dbi_max))
            print(f"Segs: {results[-1][0]:4d}  R: {results[-1][1]:.1f}Ω  G: {results[-1][2]:.2f} dBi")
    return results
```

## 9.2 ESP32 Tuner SWR Detector Calibration

### 9.2.1 Zero Calibration (No RF)

**(1)** Ensure no RF is applied to the tuner.

**(2)** Send command via BT or Serial: `CALZERO`

**(3)** Firmware reads ADC baseline and stores in NVS.

### 9.2.2 Reference Calibration (Dummy Load)

**(1)** Connect a calibrated 50Ω dummy load to the tuner output.

**(2)** Apply low-power RF (1–5W) at a known frequency.

**(3)** The tuner should report SWR ≈ 1.0.

**(4)** If SWR reading is incorrect, adjust `V_DIVIDER_RATIO` in `config.h` or use `CALREF` command with the measured actual SWR.

---

# CHAPTER 10 — TROUBLESHOOTING

## 10.1 Diagnostic Flow

**Figure 10-1. Fault Isolation Flowchart**

```
Problem reported
      │
      ▼
Is nec2c installed?   NO  → apt install nec2c
      │ YES
      ▼
Does nec2c run on manual input?  NO → Check PATH, permissions
      │ YES
      ▼
Does NECRunner detect engine?  NO → Set executable_path= explicitly
      │ YES
      ▼
Does nec_generator produce valid .nec?  NO → Check spec dict, antenna type
      │ YES
      ▼
Does nec_parser extract impedance?  NO → Check output file sections
      │ YES
      ▼
Does freq_sweep return data?  NO → Check SweepConfig range, step
      ▼
System functional
```

## 10.2 Python Script Errors

**Table 10-1. Common Python Errors**

| Error | Probable Cause | Resolution |
|-------|---------------|------------|
| `FileNotFoundError: nec2c` | NEC not in PATH | `sudo apt install nec2c` |
| `ImportError: No module named pandas` | venv not activated | `source venv/bin/activate` |
| `NECFreqPoint has no pattern data` | RP card missing | Add `model.add_pattern()` before running |
| `Empty freq_points list` | NEC run failed | Check `result.success`, `result.stderr` |
| `KeyError: freq_mhz` | Old CSV format | Regenerate CSV from current code |
| `ZeroDivisionError in calc_swr` | SWR = 1.0 exactly | Normal (perfect match); handled by code |

## 10.3 ESP32 Tuner Issues

**Table 10-2. ESP32 Tuner Faults**

| Symptom | Check | Resolution |
|---------|-------|------------|
| No Serial output | USB cable, COM port | Try different cable; select correct port |
| WiFi: FAILED | SSID/password | Edit config.h, reflash |
| Cap: HOME TIMEOUT | Hall sensor wiring | Check GPIO34 pull-up, sensor polarity |
| SWR always 99.9 | No RF applied | Apply RF signal; check divider ratio |
| SWR reading wrong | Calibration | Run CALZERO, then CALREF with dummy load |
| API request fails | Server IP/port | Verify API_HOST in config.h |

## 10.4 CYD Display Issues

**Table 10-3. CYD Display Faults**

| Symptom | Cause | Resolution |
|---------|-------|------------|
| White screen | TFT_eSPI User_Setup.h wrong | Re-check all pin definitions |
| No touch response | TOUCH_CS wrong | Set TOUCH_CS=33 in User_Setup.h |
| Touch offset/inverted | Not calibrated | Run calibration procedure (paragraph 8.3) |
| No data shown | WiFi not connected | Check SSID/password in .ino file |
| Blank chart area | Job ID not set | Set currentJobId in firmware |

---

# APPENDIX A — NEC CARD QUICK REFERENCE

**Table A-1. Frequently Used NEC Cards**

| Card | Format | Purpose |
|------|--------|---------|
| `CM` | `CM comment` | Comment (multiple allowed) |
| `CE` | `CE` | End of comments (required) |
| `GW` | `GW tag segs x1 y1 z1 x2 y2 z2 rad` | Wire segment |
| `GS` | `GS 0 0 scale` | Scale all dimensions |
| `GN` | `GN type [n] [z1] [z2] [eps] [sigma]` | Ground specification |
| `GE` | `GE [flag]` | Geometry end (required) |
| `LD` | `LD type tag seg1 seg2 R L C` | Lumped load |
| `NT` | `NT t1 s1 t2 s2 y11r y11i y12r y12i y22r y22i` | Network (TL) |
| `EX` | `EX 0 tag seg 0 Vr Vi` | Voltage excitation |
| `FR` | `FR 0 n 0 0 f0 df` | Frequency (n steps, step df MHz) |
| `RP` | `RP 0 nt np xnda thStart phStart thInc phInc` | Radiation pattern |
| `EN` | `EN` | End of input (required) |

---

# APPENDIX B — ANTENNA MODELING STANDARDS

**Table B-1. Wire Radius by Band and Wire Gauge**

| Wire Gauge | Diameter (mm) | Radius (m) | Suitable for |
|------------|--------------|-----------|--------------|
| 14 AWG | 1.628 | 0.000814 | HF dipoles, Yagi elements |
| 12 AWG | 2.053 | 0.001027 | HF dipoles, high-power |
| 10 AWG | 2.588 | 0.001294 | Vertical radiators |
| 1/4" alum | 6.35 | 0.003175 | VHF/UHF Yagi elements |
| 3/8" alum | 9.53 | 0.004763 | UHF elements, strong wind |

---

# APPENDIX C — GLOSSARY

**ADC** — Analog-to-Digital Converter. Converts analog voltage to digital number.

**ARRL** — American Radio Relay League. Publisher of the Antenna Book and other amateur radio references.

**BW** — Bandwidth. Frequency range over which an antenna meets specified performance criteria.

**CYD** — Cheap Yellow Display. ESP32 development board with ILI9341 2.8" TFT touchscreen, commonly used in maker projects.

**dBd** — Decibels relative to a half-wave dipole. 0 dBd = 2.15 dBi.

**dBi** — Decibels relative to an isotropic radiator. Standard gain unit in NEC output.

**Directivity** — Ratio of peak radiated power density to average radiated power density over all directions. Excludes losses.

**EFIE** — Electric Field Integral Equation. Mathematical formulation underlying the NEC MoM solution.

**EMRA** — Elevation angle of Maximum Radiation. Also called takeoff angle. Key parameter for HF DX propagation.

**F/B** — Front-to-Back ratio. See paragraph 1.5.3.

**Gain** — Directivity × efficiency. Includes resistive losses. See paragraph 1.5.1.

**GA** — Genetic Algorithm. Evolutionary optimization method. See Chapter 5.

**HF** — High Frequency. 3–30 MHz amateur radio range.

**LLNL** — Lawrence Livermore National Laboratory. Developer of NEC-4.

**LPDA** — Log-Periodic Dipole Array. Wideband antenna with multiple dipole elements.

**MoM** — Method of Moments. Numerical technique used by NEC to solve integral equations.

**NEC** — Numerical Electromagnetics Code. Wire-antenna modeling program based on MoM.

**NVS** — Non-Volatile Storage. Flash memory on ESP32 used for persistent settings.

**OTA** — Over-The-Air update. Firmware update via WiFi without physical USB connection.

**PSO** — Particle Swarm Optimization. Swarm intelligence optimization method. See Chapter 5.

**PSRAM** — Pseudo-Static RAM. Optional external RAM chip on some ESP32 modules.

**RP card** — Radiation Pattern card. NEC input card requesting far-field pattern computation.

**SWR** — Standing Wave Ratio. See paragraph 1.5.5.

**TFT** — Thin Film Transistor display. LCD technology used in CYD.

**UHF** — Ultra High Frequency. 300 MHz–3 GHz.

**VHF** — Very High Frequency. 30–300 MHz.

**λ** — Wavelength. λ(m) = c/f = 299.792×10⁶ / f(Hz).

---

# APPENDIX D — ABBREVIATIONS AND ACRONYMS

| Abbreviation | Meaning |
|-------------|---------|
| ADC | Analog-to-Digital Converter |
| API | Application Programming Interface |
| BLE | Bluetooth Low Energy |
| BT | Bluetooth |
| CSV | Comma-Separated Values |
| CURL | Command URL (HTTP client utility) |
| DX | Long-distance (amateur radio) |
| EFIE | Electric Field Integral Equation |
| ESP32 | Espressif Systems ESP32 SoC |
| GA | Genetic Algorithm |
| GPIO | General Purpose Input/Output |
| HTML | HyperText Markup Language |
| HTTP | HyperText Transfer Protocol |
| HF | High Frequency |
| JSON | JavaScript Object Notation |
| LAN | Local Area Network |
| LPDA | Log-Periodic Dipole Array |
| MoM | Method of Moments |
| NEC | Numerical Electromagnetics Code |
| NVS | Non-Volatile Storage |
| OTA | Over-The-Air (firmware update) |
| PEC | Perfect Electric Conductor |
| PLY | Polygon File Format (3D mesh) |
| PSO | Particle Swarm Optimization |
| REST | Representational State Transfer |
| RF | Radio Frequency |
| SWR | Standing Wave Ratio |
| TFT | Thin Film Transistor |
| TL | Transmission Line |
| UHF | Ultra High Frequency |
| VHF | Very High Frequency |
| VTK | Visualization Toolkit (3D format) |
| WiFi | Wireless Fidelity (IEEE 802.11) |

---

# APPENDIX E — PYTHON API QUICK REFERENCE

**nec_generator.py:** `Dipole(freq_mhz, height_m, ...)`, `Yagi(freq_mhz, n_directors, ...)`, `LogPeriodic(freq_low_mhz, freq_high_mhz, tau, sigma, ...)`, `VerticalWithRadials(freq_mhz, n_radials, ...)`, `QuadLoop(freq_mhz, ...)`, `PhasedArray(freq_mhz, n_elements, phase_deg, ...)`, `build_from_dict(spec_dict)`, `NECModel.write(path)`, `NECModel.to_string()`

**nec_runner.py:** `NECRunner(engine, executable_path, work_dir, timeout_s)`, `.run(model)` → `NECResult`, `.run_file(path)` → `NECResult`, `NECResult.success`, `.output_file`, `.duration_s`

**nec_parser.py:** `NECOutputParser().parse(path)` → `List[NECFreqPoint]`, `NECFreqPoint.freq_mhz`, `.r_ohm`, `.x_ohm`, `.swr_50`, `.gain_dbi_max`, `.fb_ratio_db`, `.efficiency_pct`, `.pattern`

**freq_sweep.py:** `FreqSweep(builder_fn, config)`, `.run()` → `SweepResult`, `SweepResult.to_dataframe()`, `.to_csv(path)`, `.best_swr_freq()`, `.swr_bandwidth_mhz()`, `sweep_dipole(f_low, f_high, step, height_m)`, `sweep_yagi(f_low, f_high, step, n_directors)`

**csv_export.py:** `CSVExporter().export_sweep(result, path)`, `.export_comparison(dict, path)`, `.export_pattern_3d(pattern, freq_mhz, path)`, `HTMLReporter().generate_report(result, path)`

**pattern_3d.py:** `Pattern3D.from_freq_point(freq_point)`, `.to_json(path, freq_mhz)`, `.to_vtk(path)`, `.to_ply(path)`, `.to_csv(path)`, `.to_matplotlib_3d(show=True)`

**genetic_optimizer.py:** `GeneParameter(name, min_val, max_val)`, `FitnessFunction(objectives_dict)`, `GeneticOptimizer(params, fitness_fn, builder, runner, pop_size, n_generations)`, `.run()` → `GAResult`, `GAResult.best_genes`, `.best_fitness`, `.convergence`, `.best_nec_model(builder)`

**pso_optimizer.py:** `PSOConfig(n_particles, n_iterations, w_start, w_end, c1, c2)`, `PSOOptimizer(params, fitness_fn, builder, runner, config)`, `.run()` → `PSOResult`

**batch_optimizer.py:** `BatchRunner(n_workers, output_dir)`, `.add_job(spec, sweep_cfg)`, `.run_all()`, `.run_multiband_sweep(spec, bands)`, `.compare_antennas(specs, freq_mhz)`, `MultibandOptimizer(bands, objectives)`, `.optimize_lpda(tau_range, sigma_range)`

**web_api.py:** `POST /model/run`, `GET /model/status/{id}`, `GET /model/results/{id}`, `GET /model/results/{id}/csv`, `GET /model/results/{id}/pattern3d/{freq}`, `POST /model/optimize`, `GET /model/optimize/results/{id}`, `POST /antenna/generate`, `GET /health`

---

# APPENDIX F — EXAMPLE NEC FILES

## F.1 Half-Wave Dipole 20m

```
CM Half-wave dipole 14.25 MHz h=10m Sommerfeld ground
CM Generated by TM-MODEL-001 nec_generator.py
CE
GW   1  11  -5.1987   0.0000  10.0000   0.0000   0.0000  10.0000  0.00100
GW   2  11   0.0000   0.0000  10.0000   5.1987   0.0000  10.0000  0.00100
GN 2 0 0 0 13.00 0.0050
GE 1
EX 0   1  11 0 1.0000 0.0000
FR 0 1 0 0 14.2500 0
RP 0 37 73 1000 0.0 0.0 5.0 5.0
EN
```

## F.2 Quarter-Wave Vertical with 32 Radials (40m)

```
CM Quarter-wave vertical 7.15 MHz 32 radials
CM Generated by TM-MODEL-001 nec_generator.py
CE
GW   1  11   0.0000   0.0000   0.0100   0.0000   0.0000   9.9282  0.00200
GW   2   5   0.0000   0.0000   0.0100  10.4631   0.0000   0.0100  0.00100
GW   3   5   0.0000   0.0000   0.0100  10.2251   2.0647   0.0100  0.00100
GW   4   5   0.0000   0.0000   0.0100   9.5378   3.9762   0.0100  0.00100
GW   5   5   0.0000   0.0000   0.0100   8.4339   5.6822   0.0100  0.00100
[... 28 more radials ...]
GN 2 0 0 0 13.00 0.0050
GE 1
EX 0   1   1 0 1.0000 0.0000
FR 0 1 0 0 7.1500 0
RP 0 37 73 1000 0.0 0.0 5.0 5.0
EN
```

## F.3 5-Element Yagi 2m

```
CM 5-element Yagi 144.2 MHz free space
CM Generated by TM-MODEL-001 nec_generator.py
CE
GW   1  11  -0.0000  -0.4828   6.0000  -0.0000   0.0000   6.0000  0.00400
GW   2  11  -0.0000   0.0000   6.0000  -0.0000   0.4828   6.0000  0.00400
GW   3  11   0.0000  -0.4602   6.0000   0.0000   0.0000   6.0000  0.00400
GW   4  11   0.0000   0.0000   6.0000   0.0000   0.4602   6.0000  0.00400
GW   5  11   0.4165  -0.4382   6.0000   0.4165   0.0000   6.0000  0.00400
GW   6  11   0.4165   0.0000   6.0000   0.4165   0.4382   6.0000  0.00400
GW   7  11   0.8330  -0.4362   6.0000   0.8330   0.0000   6.0000  0.00400
GW   8  11   0.8330   0.0000   6.0000   0.8330   0.4362   6.0000  0.00400
GW   9  11   1.2495  -0.4350   6.0000   1.2495   0.0000   6.0000  0.00400
GW  10  11   1.2495   0.0000   6.0000   1.2495   0.4350   6.0000  0.00400
GN -1
GE 0
EX 0   3  11 0 1.0000 0.0000
FR 0 1 0 0 144.2000 0
RP 0 37 73 1000 0.0 0.0 5.0 5.0
EN
```

---

*End of Technical Manual TM-ANT-MODEL-001 Revision A*

*Prepared: 2026-05-01*
*Location: Merced, CA Field Station*
*System: Public/claude/ant_gear/antenna_modeling*
