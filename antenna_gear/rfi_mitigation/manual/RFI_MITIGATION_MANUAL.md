# TECHNICAL MANUAL
## TM-RFI-001 Rev A
# RADIO FREQUENCY INTERFERENCE (RFI) AND QRM MITIGATION
### Complete Home Station Noise Reduction System

---

**DOCUMENT NUMBER:** TM-RFI-001 Rev A  
**DATE:** 2 May 2026  
**SUPERSEDES:** None (initial issue)  
**APPLICABLE TO:** All ham shack installations; residential environment; co-location
with computers, peripherals, LED lighting, and household appliances.

---

## TABLE OF CONTENTS

| Chapter | Title |
|---------|-------|
| 1 | Fundamentals of RFI and QRM |
| 2 | RFI Sources in the Modern Home |
| 3 | Conducted vs. Radiated Interference |
| 4 | Common-Mode Current and Feed Line Noise |
| 5 | AC Mains EMI Filter |
| 6 | Common-Mode Chokes — Ferrite Selection and Winding |
| 7 | Active Noise Cancellation |
| 8 | Near-Field RFI Sniffer Probes |
| 9 | ESP32 Automated Noise Floor Monitor |
| 10 | CYD Display for Noise Monitoring |
| 11 | Grounding, Bonding, and Single-Point Ground Systems |
| 12 | Systematic RFI Hunt — Field Procedures |
| 13 | Device-Specific Remediation |
| 14 | Troubleshooting |
| A | Ferrite Core Quick Reference |
| B | Common RFI Frequencies and Signatures |
| C | NEC Model Interpretation Guide |
| D | Parts List and Sources |

---

## CHAPTER 1 — FUNDAMENTALS OF RFI AND QRM

### 1.1 Definitions

**RFI (Radio Frequency Interference):** Any RF signal that degrades the quality
of a desired radio communication. RFI may originate from intentional transmitters
(legal or illegal) or from unintentional emitters (electronic devices).

**QRM:** Amateur radio shorthand for manmade interference. Specifically denotes
non-atmospheric interference on a radio channel. Used to distinguish from QRN
(atmospheric noise, static) and QSB (fading).

**Noise floor:** The lowest signal level readable on a receiver. Determined by
thermal noise (unavoidable) plus all received manmade noise (RFI). Lower noise
floor = greater sensitivity = better weak-signal capability.

**S-meter scale (ITU standard):** Each S-unit = 6 dB. S9 = −73 dBm at HF.
    S0 = −121 dBm   (below noise floor of most receivers)
    S1 = −115 dBm
    S5 = −91 dBm
    S9 = −73 dBm
    S9+10 = −63 dBm
    S9+20 = −53 dBm
    S9+60 = −13 dBm   (very strong — possible damage threshold)

A noise floor of S7 (−79 dBm) means: any signal below S7 is buried in noise.
Reducing noise floor to S1 (−115 dBm) improves sensitivity by 6 S-units = 36 dB.
This is the difference between hearing a DX station and not hearing it.

### 1.2 Types of RF Noise

**Broadband noise:** Continuous spectral content across a wide frequency range.
Source: switching power supplies, motor brushes, SMPS inverters.
Signature on SDR: raised noise floor across entire panadapter display.

**Narrowband interference:** Strong signal on one or a few specific frequencies.
Source: oscillators (CPU clocks, PWM controllers), LO leakage, harmonics.
Signature: one or more discrete "birdies" (narrow spikes) on panadapter.

**Periodic impulsive noise:** Repeated bursts at a regular interval.
Source: microcontroller timers, PWM dimmer switching, motor commutation.
Signature: S-meter twitches rhythmically; audio has periodic "buzz" or "crackle."

**Conducted interference:** Noise travels via electrical conductors (mains wiring,
signal cables, coax braid). Enters shack via AC power line or coax feed line.

**Radiated interference:** Noise radiates as electromagnetic waves through space.
Enters shack via antenna, or couples directly to equipment through proximity.

### 1.3 Noise Figure, Noise Temperature, and MDS

**Thermal noise power** at temperature T in bandwidth B:
    P_noise = k × T × B
    = 1.38×10⁻²³ × 290 × B = 4.0×10⁻²¹ × B (watts)
    = −174 dBm/Hz + 10×log10(B)

At B = 2400 Hz (SSB bandwidth): P_noise = −174 + 33.8 = −140.2 dBm
At B = 500 Hz (CW): P_noise = −147 dBm

**Minimum Detectable Signal (MDS):** P_noise + receiver NF
    Good HF receiver (NF = 10 dB), SSB BW: MDS = −130 dBm = S0−9 dB
    SDR with LNA (NF = 3 dB), SSB BW: MDS = −137 dBm

**Effect of RFI on MDS:** If RFI raises the noise floor by 20 dB (S4 to S8),
then MDS worsens by 20 dB. You have lost all sensitivity advantage of a good
receiver/antenna combination.

**Conclusion:** Noise mitigation is as important as transmit power and antenna gain.
Reducing noise by 10 dB is equivalent to doubling transmit power of the other station.

---

## CHAPTER 2 — RFI SOURCES IN THE MODERN HOME

### 2.1 Survey of Common Home RFI Sources

The following categories cover the vast majority of residential RFI:

**Category 1: Switching Power Supplies (SMPS)**
Every wall wart, laptop charger, phone charger, and LED power supply contains
a switching regulator operating at 50 kHz–3 MHz. The switching creates harmonics
extending into the HF amateur bands.

| Device | Typical switching frequency | Harmonic bands affected |
|--------|---------------------------|------------------------|
| Phone charger (5W) | 100–400 kHz | 160m, 80m |
| Laptop charger (65W) | 60–200 kHz | 160m, 80m, 40m |
| LED bulb driver | 25–100 kHz | 160m, 80m |
| Computer ATX supply | 100–200 kHz | 160m, 80m, 40m |
| Solar MPPT charge controller | 50–150 kHz | 160m, 80m, 40m |
| Inverter/UPS | 50–60 Hz fundamental; PWM: 10–50 kHz | all bands |

**Category 2: LED Lighting**
Modern LED bulbs use switching drivers (same as SMPS). Additional noise from PWM
dimming. Some LED drivers intentionally spread-spectrum their switching frequency
to pass EMC testing — this creates broadband hash rather than discrete harmonics.

Particularly troublesome: LED strip lights with cheaply made constant-current drivers.
Cheap LED "corn bulb" replacements: notorious for severe HF interference.

**Category 3: Computers and Peripherals**
- USB 3.0 generates a strong noise signature at 2.5 GHz and harmonics down to HF
- HDMI cables radiate 165 MHz carrier and harmonics (affects VHF bands)
- Ethernet switches: broadband digital noise
- CPU core voltage regulators: switch at 200–500 kHz × number of phases
- GPU power regulators: especially during high GPU load

**Category 4: Smart Devices and Wireless**
- Smart meters: power line carrier (30–500 kHz) + ZigBee (2.4 GHz)
- WiFi routers: 2.4 GHz and 5 GHz broadband noise
- Baby monitors (older 49 MHz models): directly in ham bands
- Baby monitors (DECT 6.0): 1.9 GHz but local desensitization
- Plasma TVs (older): severe HF noise; modern OLED/LCD much better
- Touchscreen dimmer switches: often generate significant QRM

**Category 5: Appliances and Mechanical**
- Refrigerator compressor: impulsive noise on startup and stop; AC induction motor
- Vacuum cleaner: universal motor (brushes = arcing = broadband noise)
- Dimmer switches (TRIAC): generate harmonics of power line frequency
- Electric blankets / heating pads: similar to TRIAC dimmers
- Fluorescent lights (magnetic ballast): 50/60 Hz and harmonics
- CFL bulbs: switching power supply noise + ballast harmonics

### 2.2 Identifying Your Specific Noise Sources

Every home is different. Systematic identification is required before mitigation.

**STEP 1: Baseline measurement.** Note noise floor on each ham band with all shack
equipment on, nothing else running. Use SDR noise floor monitor (Chapter 9) or
S-meter plus careful note-taking.

**STEP 2: Circuit breaker test.** Turn off all circuit breakers except shack power.
Re-measure. Reduction = mains-conducted noise from rest of house.
Progressive re-energization of circuits identifies which circuit(s) contribute most.

**STEP 3: Device elimination.** With individual circuit identified, walk through the
circuit unplugging devices one at a time. When noise floor drops: that device is
the culprit.

**STEP 4: Near-field probe confirmation.** Use H-field sniffer (Chapter 8) near
confirmed device. Verify S-meter peaks near the device.

**STEP 5: Coupling path identification.** Once source identified, determine HOW the
noise reaches your antenna:
- Conducted via mains: mains filter at shack entry will help
- Conducted via coax braid: feed line choke at antenna will help
- Radiated direct to antenna: source must be physically suppressed

---

## CHAPTER 3 — CONDUCTED vs. RADIATED INTERFERENCE

### 3.1 Conducted Path

Noise travels through electrical conductors. Main conducted paths:

**AC mains wiring:** The mains power network is a giant antenna. Switching supply
noise generated by any device on the building's wiring appears on all outlets.
A SMPS in one room conducts noise to your shack's power outlets.

**Mitigations:**
- AC mains EMI filter at shack power entry (Chapter 5)
- Ferrite cores on offending device's power cord
- Separate circuit for shack (dedicated 20A circuit from panel)
- Replace offending device with low-noise alternative

**Coaxial feed line braid:** See Chapter 4 (common-mode current) for the detailed
treatment. This is often the dominant path for HF noise.

**Signal cables (audio, USB, Ethernet):** Noise coupling via signal reference ground.
Use shielded cables. Install ferrite clamps on both ends.

### 3.2 Radiated Path

Noise travels through space as electromagnetic radiation. Directly couples to antenna.

**Near-field coupling:** Distance < λ/2π ≈ λ/6. Fields fall off rapidly (1/r² or 1/r³).
At 7 MHz, near-field region extends to λ/6 = 7.1m. Sources within 7m couple strongly.

**Far-field:** Distance > λ. Fields fall off as 1/r. Neighbor's equipment can couple
at far field if strong enough.

**Mitigations for radiated path:**
- Physically separate antenna from noise sources (height, distance)
- Use directional antenna to null toward noise source
- Active noise cancellation (Chapter 7)
- Shielding the noise source itself (if possible/legal)

### 3.3 Which Path Dominates?

**Test:** Disconnect antenna. Does noise floor drop?
- YES: noise primarily enters via antenna/feed line (radiated or common-mode conducted)
- NO: noise enters via mains or ground (conducted via wiring)

**Test:** Connect dummy load instead of antenna. Does noise floor drop significantly?
- YES: noise primarily via antenna
- NO: noise is generated internally in the receiver or enters via power supply

---

## CHAPTER 4 — COMMON-MODE CURRENT AND FEED LINE NOISE

### 4.1 Why Coax Braid Carries Noise

A coaxial cable has two electrical modes:
1. **Differential mode:** Current flows forward on center conductor; return on
   inner surface of braid. Signal energy stays INSIDE the coax.
2. **Common-mode:** Current flows in the SAME direction on both conductors
   (like a single wire). This current flows on the OUTSIDE surface of the braid.

Common-mode current arises from:
- **Ground potential difference:** The antenna feedpoint ground and the shack ground
  are at different RF potentials. This drives current on the outside of the braid.
- **Station RF:** During transmit, current returns to station through the braid exterior
  instead of through the antenna radials/ground.
- **External noise coupling:** The entire coax exterior acts as a noise receiving antenna.
  Every noise source with a ground path can drive current on the braid.

### 4.2 Effect on Reception

The coax braid exterior is typically 10–30 meters long (antenna mast height + shack run).
At 7 MHz, λ = 42m; 20m of coax braid = λ/2 — a resonant antenna!

Noise current on the braid creates a received noise signal indistinguishable from
a signal arriving at the antenna. The NEC model (nec_models/dipole_cm_current.nec)
demonstrates how 30% common-mode current on a 3m stub distorts the dipole pattern
and fills in nulls — dramatically increasing noise pick-up.

**Quantitative effect:** A well-measured example:
- Before choke at antenna: noise floor S7 on 40m
- After choke at antenna (FT-240-31 × 2, 8 turns): noise floor S3
- Improvement: 4 S-units = 24 dB

This 24 dB improvement represents the noise that was being injected via the coax braid.

### 4.3 Choking the Common Mode

A common-mode choke (see Chapter 6) presents high impedance to the common-mode
current path while being transparent to the differential signal inside the coax.

Required choke impedance for good suppression:
    Suppression (dB) = 20 × log10(Z_CM / Z_load)
    For 20 dB: Z_CM / Z_load = 10, so Z_CM = 500Ω (into 50Ω load)
    For 30 dB: Z_CM = 1580Ω
    For 40 dB: Z_CM = 5000Ω

A 2× FT-240-31, 8-turn choke provides Z_CM > 10,000Ω at 7 MHz (see Chapter 6).
This gives theoretical suppression > 46 dB — excellent.

### 4.4 Choke Placement Strategy

**Primary choke:** At the antenna feed point. This is the highest-leverage placement.
- Prevents house noise from traveling up the braid
- Prevents transmitter RF from returning on the braid
- Forces the antenna feed impedance to be purely differential (balanced)

**Secondary choke:** At the shack entry point (where coax comes through the wall).
- Second line of defense for any residual common-mode current
- Particularly important if the outside coax run is in conduit with other wiring

**Tertiary (optional):** On the shack side, at the back of the transceiver.
- Protects the transceiver from RFI that enters after the shack entry choke
- Useful if the problem is computer noise coupling to coax via RF ground loops

### 4.5 Balanced Antennas

A truly balanced antenna (center-fed dipole with tuned feedline) inherently has
less common-mode current than an unbalanced antenna (vertical, end-fed):

**Dipole:** Feed point is balanced. With a choke balun at the feed point,
near-zero common-mode current. This is the quietest antenna configuration.

**Vertical / end-fed:** Inherently unbalanced. Common-mode current is normal
operation — the braid is the radial system. MUCH more noise pickup. For these
antennas, a choke at the shack entry is especially important.

**EFHW (end-fed half-wave):** Despite high-impedance match transformer, common-mode
is significant. Install 1:49 UNUN + choke at shack entry.

---

## CHAPTER 5 — AC MAINS EMI FILTER

### 5.1 Why Filter at the Mains Entry

The AC mains wiring in a house acts as a distributed antenna — any RF energy on the
wiring radiates and couples to nearby equipment. By filtering the AC power at the
point where it enters the shack, you:
1. Block conducted noise from reaching shack equipment
2. Block transmitter harmonics from entering the mains and radiating back into your antenna
3. Provide a clean reference ground for the shack

### 5.2 Filter Design

See schematic TM-RFI-SCH-001 for full circuit detail.

**Differential-mode (DM) filter:** X2 capacitors across Line-Neutral and series
inductors on each conductor. Attenuates noise between the two conductors.

**Common-mode (CM) filter:** Y2 capacitors from Line and Neutral to Earth Ground,
plus a bifilar common-mode choke (CM inductor). Attenuates noise common to
both conductors relative to earth.

**MOV transient suppressor:** Absorbs lightning-induced spikes. Required in any
outdoor antenna installation environment.

### 5.3 Installation

**WARNING:** Mains voltage installation must comply with NEC (USA), CEC (Canada),
or applicable local electrical codes. If unsure, consult a licensed electrician.

1. Install filter in a suitable enclosure (printed enclosure TM-RFI-ENC-001 or
   commercial metal project box).
2. Wire IEC C14 inlet to filter input; filter output to multiple IEC C13 outlets
   for shack equipment.
3. Earth ground: #12 AWG minimum from filter chassis ground terminal to
   building grounding electrode conductor at the main panel or to a local
   ground rod bonded to the main ground.
4. Place shack power distribution box on the bench; plug all shack equipment into it.

### 5.4 Safety Requirements

- All mains connections: minimum 14 AWG wire with appropriate ratings.
- X2 capacitors: rated specifically for across-the-line use. Do not substitute.
- Y2 capacitors: rated for Y2 service. Do not substitute general-purpose caps.
- Fuse: slow-blow, properly rated for the load (10A for typical shack).
- Earth ground: mandatory. The filter is ineffective and potentially dangerous without
  a solid earth ground connection.

---

## CHAPTER 6 — COMMON-MODE CHOKES: FERRITE SELECTION AND WINDING

### 6.1 Ferrite Material Properties

Ferrite cores work by absorbing (dissipating) RF energy in the magnetic material.
The key parameter is complex permeability: µ = µ' − j×µ''. The µ'' (imaginary,
loss) component is what we want — high µ'' = high absorption at target frequency.

**Select by frequency:**
| Mix | Best frequency range | Application |
|-----|---------------------|-------------|
| 31 | 1–30 MHz (best 1–10 MHz) | HF feed line chokes |
| 43 | 1–300 MHz (best 10–50 MHz) | HF + low VHF |
| 61 | 10–300 MHz | Upper HF + VHF |
| 75 | 0.1–5 MHz | AM broadcast suppression |
| 77 | 0.1–5 MHz | Low-frequency applications |

**For most ham station use:** Mix 31 is the right choice. Stacking two FT-240-31
cores covers 160m through 10m with excellent suppression.

### 6.2 Core Size Selection

**FT-140-31 (1.4" OD):** Fits up to RG-58 (5mm OD). Limited turns.
**FT-240-31 (2.4" OD):** Fits RG-8X (7.5mm OD) for 8 turns. Standard choice.
**FT-240-43:** Same size, Mix 43. Use when noise is above 30 MHz.

For RG-213 (10.3mm OD), FT-240 hole (25.4mm) allows 5–6 turns. Acceptable.
For RG-8 (10.3mm OD): same as RG-213. Use Palomar FB-73-6401 (larger, 38mm hole)
for 8 turns of large coax.

### 6.3 Winding Procedure for Coax Choke

See schematic TM-RFI-SCH-002 for detailed winding tables.

**For FT-240-31 × 2 stacked, RG-8X, 8 turns:**

1. Tape the two FT-240-31 cores together with 2–3 layers of Kapton tape.
2. Mark the center of a 2m piece of RG-8X coax with a marker.
3. Place the stacked cores on the coax, centered.
4. Make the first pass: bring both sides of coax through the hole together.
5. Continue looping the coax through the core hole, maintaining neat, tight turns.
6. Count 8 complete passes through the center hole.
7. Secure loose turns with cable ties or electrical tape.
8. Install in choke housing (TM-RFI-ENC-002).
9. Fit PL-259 connectors on both ends.
10. Test impedance with NanoVNA (see Section 6.4).

**Verification with NanoVNA:**
- Connect Port 1 to one end of choke.
- Terminate other end in 50Ω (or leave open for impedance measurement).
- Measure S11 (reflection): at each frequency, observe impedance.
- At 7 MHz: |Z| should exceed 10 kΩ (confirms effective choking).

### 6.4 Choke Quality Factor Expectations

A well-wound choke should meet these minimums:

| Frequency | Z_CM minimum | Achievable (2× FT-240-31, 8T) |
|-----------|-------------|-------------------------------|
| 1.8 MHz | 2 kΩ | 8 kΩ |
| 3.5 MHz | 5 kΩ | 16 kΩ |
| 7.0 MHz | 10 kΩ | 28 kΩ |
| 14 MHz | 10 kΩ | 40 kΩ |
| 28 MHz | 5 kΩ | 44 kΩ |

If measured values fall short: verify correct core mix (stamped on core or check
ferrite color); verify adequate number of turns; check for turn-to-turn shorts.

### 6.5 Feed Line Choke for Specific Antenna Types

**Center-fed dipole:** One choke at the feed point. Can use wound coax on form or
toroid choke. For simple installation, the wound coax ("ugly balun") is easiest.

**Vertical with radials:** Two chokes recommended: one at the base (feed point)
and one at the shack entry. The feed point choke prevents common-mode current
from flowing down the outside of the coax; the shack entry choke blocks any
that remains.

**End-fed half-wave:** Three chokes: at the UNUN/transformer output, at the shack
entry, and optionally one midway on long coax runs.

**Inverted-L or T-antenna:** At least shack entry choke; feed point choke if
transmission line is coax; otherwise use a proper ATU with balanced output.

---

## CHAPTER 7 — ACTIVE NOISE CANCELLATION

### 7.1 Principle

The active noise canceller uses a reference (auxiliary) antenna positioned near the
noise source to sample the interference. The reference signal is phase-shifted and
amplitude-adjusted to match the noise component in the main antenna, then subtracted.

This technique is highly effective when:
- The noise source is nearby (strong reference signal)
- The desired signal arrives from a different direction than the noise
- The noise is coherent (stable phase and amplitude) — most SMPS noise is coherent

**Limitations:**
- Only one noise direction can be cancelled at a time
- Phase adjustment must be re-tuned when frequency changes significantly
- Desired signal is slightly attenuated (~3 dB) in the cancel direction

### 7.2 Reference Antenna Selection

The reference antenna should pick up maximum noise with minimum desired signal:

**Short vertical whip (1–2m):** Best for noise arriving from high angles or nearby
horizontal sources. Place near the noise source, away from the main antenna direction.

**Small loop:** Aimed at the noise source. The null can be directed away from the
desired signal direction for best signal-to-noise cancellation ratio.

**Sense antenna (existing):** If you already have a small loop RDF antenna,
it can serve as the reference antenna for noise cancelling.

### 7.3 Adjustment Procedure

See schematic TM-RFI-SCH-003 for circuit details.

1. **Connect main antenna** to MAIN port (from feed line).
2. **Connect reference antenna** (short whip, aimed at noise source) to REF port.
3. **Connect output** to SDR or transceiver antenna input.
4. **Tune to noise frequency.** Maximize SDR display on noise birdie.
5. **Note reference level:** Is there signal on the reference port? Enable bypass
   momentarily to confirm reference picks up the noise.
6. **Phase adjustment:**
   - Slowly rotate RV1 (Phase 1) through full rotation.
   - Observe S-meter for dip. Stop near minimum.
   - Slowly rotate RV2 (Phase 2) through full rotation.
   - Observe deeper dip. Stop at minimum.
7. **Gain adjustment:**
   - Rotate RV3 (Gain) for deepest null.
   - The null deepens as gain approaches the correct setting; then rises again.
   - Set gain at deepest point.
8. **Iterate:** Phase 1 → Phase 2 → Gain, repeat until null bottoms out.
9. **Check signal preservation:** Tune to a known signal. It should be present
   (possibly slightly weaker). If signal is cancelled along with noise, the
   reference antenna is pointed at the signal direction — reposition reference.

**Expected cancellation:** 20–40 dB of noise reduction. Under ideal conditions,
a single null can achieve >50 dB cancellation (theoretically infinite). In practice,
noise source motion, multipath, and component drift limit practical null depth.

### 7.4 Digital Noise Cancellation Software

For SDR software-based noise cancellation:

**GQRX DSP options:**
- Noise Blanker (NB): impulsive noise (clicks, arc discharge)
- Noise Reduction (NR): LMS adaptive filter for continuous noise
- AGC: prevents noise bursts from desensitizing receiver

**SDR++ plugins:**
- Noise blanker plugin
- Audio NR via rtty/voice processing

**GNU Radio:** Implement LMS adaptive filter flow graph:
    Reference signal → LMS filter (tap weight adaptation)
    LMS output subtracted from main signal → cleaned output
    Filter adapts to minimize residual noise power.

---

## CHAPTER 8 — NEAR-FIELD RFI SNIFFER PROBES

### 8.1 Overview

Near-field probes allow locating noise sources to within centimeters. They exploit
the rapid fall-off of near-field electromagnetic fields (1/r² to 1/r³) to distinguish
the primary source from secondary radiators.

See schematic TM-RFI-SCH-004 and enclosure TM-RFI-ENC-004 for construction details.

### 8.2 H-Field Loop Probe Use

The H-field loop responds to magnetic field (current flow). Use it to find:
- Switching inductor cores in SMPS
- Transformer windings
- PCB traces carrying high-frequency switching current
- CPU voltage regulator inductors

**Usage:**
1. Connect probe to SDR via 20 dB attenuator (prevents overload near strong sources).
2. Tune SDR to noise frequency.
3. Hold probe near suspect device.
4. Move slowly across device surface — S-meter peaks over switching inductors.
5. Rotate probe: null (minimum) when loop plane points toward source.
   Peak when loop plane is parallel to current direction.
6. Most sensitive position: loop plane parallel to PCB with inductor on PCB.

### 8.3 E-Field Rod Probe Use

The E-field rod responds to electric field (voltage switching). Use it to find:
- High-voltage switching nodes (drain of FET in SMPS)
- PWM control signals
- Clock outputs on PCBs
- Any high-impedance rapidly-switching node

**Usage:**
1. ALWAYS use 20 dB attenuator — E-field near a SMPS drain can be hundreds of volts!
2. Tune SDR to noise fundamental frequency.
3. Touch probe tip toward PCB surface, 1–2cm distance.
4. Traverse PCB — peaks over switching transistor drain pins, gate drive traces.
5. Work from device exterior inward to identify highest-field location.

### 8.4 Probe Safety

- Never touch probe tip to live mains-voltage conductors.
- Use probe only on low-voltage (<50V) circuit boards or on the OUTSIDE of appliances.
- For investigating mains-powered devices: probe through plastic case only.
- If device case can be opened: unplug the device first, discharge capacitors,
  then investigate with device powered via isolation transformer.

---

## CHAPTER 9 — ESP32 AUTOMATED NOISE FLOOR MONITOR

### 9.1 Overview

The automated noise floor monitor (TM-RFI-FW-001) scans all amateur bands every
~22 seconds and logs the noise floor level. This enables:

- **Trending:** Is the noise floor rising over time? Indicates developing equipment fault.
- **Diurnal patterns:** Higher noise during day (appliances on) vs. night (appliances off).
- **Appliance correlation:** Note timestamp of noise rise; correlate with when devices
  were turned on.
- **Mitigation verification:** Log before and after installing a choke or filter.
  The improvement is precisely documented.

See schematics TM-RFI-SCH-005 for hardware, firmware TM-RFI-FW-001 for software.

### 9.2 Hardware Setup

**Components required:**
- ESP32 DevKit V1
- Si5351 breakout board (Adafruit or bare chip with 25 MHz crystal)
- SA612AN mixer (DIP-8)
- SPF5189Z LNA (SOT-89-3, on breakout or hand-soldered)
- AD8307 (SOIC-8 or DIP-8)
- 10.7 MHz ceramic BPF (Murata CFWLA10M7KA0-B0)
- Input LPF components (220 nH × 2, 68/120 pF capacitors)
- 3.3V regulator and decoupling
- Optional: SD card module for data logging

**Antenna:** A 20cm non-resonant vertical wire is adequate for noise monitoring
(the monitor does not need gain — it measures noise, not weak signals).
A small loop (~100mm) is quieter (less susceptible to electric field noise).

### 9.3 Web Dashboard

Connect to WiFi AP "RFI-Monitor" (password: rfimonitor), then browse to
http://192.168.4.1. The dashboard shows:

- **Band grid:** 11 cards (160m–2m), each showing current noise level in dBm
  with color coding (green/yellow/orange/red) and a mini bar.
- **History chart:** Rolling time-series plot, all bands overlaid, 40 data points.
- **Alert threshold:** Adjustable — set to your typical noise floor baseline.
  Any band exceeding threshold shows a red alert indicator.
- **Export CSV:** Downloads current snapshot as CSV for offline analysis.

### 9.4 Interpreting Results

**Normal noise floor ranges (typical suburban/rural ham station):**
| Band | Quiet (rural) | Typical suburban | High urban noise |
|------|--------------|-----------------|-----------------|
| 160m | −110 dBm | −90 dBm | −70 dBm |
| 80m | −115 dBm | −100 dBm | −85 dBm |
| 40m | −120 dBm | −105 dBm | −90 dBm |
| 20m | −120 dBm | −110 dBm | −95 dBm |
| 10m | −125 dBm | −115 dBm | −105 dBm |
| 6m | −125 dBm | −120 dBm | −110 dBm |

If your measured levels are significantly higher than "typical suburban," significant
noise reduction is achievable.

---

## CHAPTER 10 — CYD DISPLAY FOR NOISE MONITORING

### 10.1 Overview

The CYD (ESP32-2432S028R) touchscreen provides a portable, always-on noise floor
display. It connects via Bluetooth to the ESP32 noise monitor and shows:

- Per-band noise level in dBm (11 bands simultaneously)
- Color-coded bars: green (quiet) → yellow → orange → red (noisy/alert)
- Mini sparklines per band (40-point history)
- Alert indicators when any band exceeds threshold
- Touch control of alert threshold

### 10.2 TFT_eSPI Configuration

The CYD uses a specific pin mapping. Ensure User_Setup.h in TFT_eSPI library:
```cpp
#define USER_SETUP_ID 206
#define ILI9341_DRIVER
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST  -1
#define TFT_BL   21
#define TOUCH_CS 33
```

### 10.3 Display Layout

**Top row:** Title "RF Noise Monitor" + Bluetooth connection indicator + scan count.

**Main area:** 2-column × 6-row grid of band cards. Each card shows:
- Band name (e.g., "40M") in cyan
- Current noise level in dBm
- Color-coded fill bar (proportion of −40 to −120 dBm range)
- Mini sparkline (40-point history)

**Alert row:** Appears when any band exceeds threshold. Lists affected bands.

**Status bar:** Connection state, threshold value, web UI hint.

**Touch controls:**
- Tap left half of status bar: decrease threshold by 5 dBm
- Tap right half of status bar: increase threshold by 5 dBm

---

## CHAPTER 11 — GROUNDING, BONDING, AND SINGLE-POINT GROUND SYSTEMS

### 11.1 The Single-Point Ground (SPG) Principle

**Problem with multiple ground connections:** If the shack's coax shields, equipment
chassis, and power ground connect to earth at multiple points, ground loops form.
RF current flowing between ground points is indistinguishable from antenna signal.

**Solution:** All RF ground connections should converge at one point — the Single-Point
Ground (SPG). From the SPG, a single large conductor bonds to earth ground.

### 11.2 SPG Implementation

**SPG bus:** A copper busbar (25mm × 3mm copper strip or braid), typically mounted
at the entry point of feed lines through the shack wall.

**Connected to SPG:**
- All coax shield ground connections
- All equipment chassis ground connections
- Lightning arrestor grounds
- Any bonded conduit entry

**From SPG to earth:** Single conductor, minimum #6 AWG bare copper (or larger).
Shortest possible path to earth electrode. Avoid sharp bends (inductance).

### 11.3 Feed Line Entry Bonding

The NEC (National Electrical Code) and good amateur practice both require all antenna
feed lines to have a grounding/lightning arrestor at the building entry point.

**Recommended entry panel:**
1. Mount an outdoor-rated bulkhead panel at the point where coax enters the building.
2. Install coaxial lightning arrestors (Polyphaser, ICE, or similar) on each feed line.
3. Bond all lightning arrestor chassis to the SPG busbar.
4. Bond SPG busbar to building ground electrode with #6 AWG or better.

### 11.4 Equipment Grounding Inside Shack

**Star topology:** All equipment chassis connect via short conductors to the SPG bus.
Do NOT daisy-chain grounds (A → B → C → SPG creates ground loops A-B and B-C).

**Practical implementation:**
- Short (#14 AWG) pigtails from each equipment chassis to SPG bus
- SPG bus directly to earth ground via the entry panel

### 11.5 RF Ground Loops

An RF ground loop forms when two pieces of equipment are grounded via the signal
cable connection AND separately via their chassis/power ground paths. The loop area
acts as a receiving antenna for magnetic fields (magnetic flux through the loop).

**Identify loops:** Use H-field probe near connecting cables. If S-meter peaks when
probe is near a signal cable (USB, audio, HDMI), there is a ground loop in that cable.

**Break the loop:** Ferrite clamp on the cable (both ends) adds CM impedance and
disrupts the loop. For audio cables: use a transformer-coupled audio isolator (Hum-X
or Jensen ISO-MAX). For USB: use USB isolator (ADUM4160).

---

## CHAPTER 12 — SYSTEMATIC RFI HUNT: FIELD PROCEDURES

### 12.1 Investigation Protocol

**Phase 1: Documentation**
- Record baseline noise floor (all bands, time of day, day of week).
- Use noise monitor to generate 24-hour trend (if possible).
- Note any temporal correlation with known appliance schedules.

**Phase 2: Source Localization**
1. Circuit breaker test: identify which branch circuit carries the noise.
2. Device elimination on that circuit: identify which device.
3. Near-field probe confirmation: verify source by proximity.
4. Coupling path identification: conducted (mains) or radiated.

**Phase 3: Remediation**
Select the appropriate mitigation from Chapter 13.
Apply mitigation. Re-measure noise floor. Document improvement.

**Phase 4: Documentation of Completion**
Record: device found, coupling path, mitigation applied, improvement in dB.
File in station logbook. Future technicians benefit from this history.

### 12.2 RFI Hunt Field Log Template

```
DATE/TIME: _______________  OPERATOR: _______________
FREQUENCY: _______ MHz     NOISE LEVEL: _______ dBm (S __)
SUSPECTED SOURCE: ______________________________________
LOCATION IN HOME: ______________________________________

Circuit breaker test results:
  Breaker  |  Noise with ON  |  Noise with OFF
  ─────────────────────────────────────────────
           |                 |
           |                 |
           |                 |

Device elimination on circuit ___:
  Device removed              |  Noise level
  ──────────────────────────────────────────
                              |
                              |
  CULPRIT: __________________ |  _________ dBm

Coupling path: [  ] Mains conducted  [  ] Radiated  [  ] Feed line braid

Mitigation applied: ________________________________________
Noise level after: _________ dBm   Improvement: _____ dB (__ S-units)

Notes: __________________________________________________
```

### 12.3 Difficult-to-Locate Sources

**Problem:** Noise is intermittent and doesn't correlate with any obvious device.

**Approach:**
- Use noise monitor logging (Chapter 9). Review 24-hour trend.
- Note EXACT time of onset. Correlate with thermostat/appliance schedules.
- Check neighbor's property for smart meters, solar inverters, EV chargers.
- Cyclic noise with 20–30 min period: refrigerator compressor cycling.

**Problem:** Noise is present even with all circuits off at the panel.

**Approach:**
- Check with all equipment UNPLUGGED (not just off) — some devices have standby SMPS.
- Check if noise changes when transmitter is keyed (possible re-radiation/intermodulation).
- Disconnect antenna; if noise disappears, source is on a neighbor's property or
  public utility equipment (power line, smart meter, streetlight ballast).

---

## CHAPTER 13 — DEVICE-SPECIFIC REMEDIATION

### 13.1 Switching Power Supplies (Phone Chargers, Wall Warts)

**Best fix:** Replace with quality supply (Anker, Belkin, Apple OEM — EMC certified).
Look for FCC Part 15 Class B, EN55032 Class B markings.

**If replacement not possible:**
1. Ferrite clamp (Mix 31) on both AC side and DC output cable.
2. Y-capacitors (2200 pF to chassis/earth) from Line and Neutral:
   ONLY if chassis is earthed — floating chassis cannot use Y-caps.
3. Place device in grounded metal enclosure (ferrite-lined box is overkill for most).

**Expected improvement:** 15–30 dB with good ferrite placement.

### 13.2 LED Lighting

**Best fix:** Replace lamp with verified low-noise unit. Look for:
- EN55015 Class B certified (European EMC directive)
- "Flicker-free" LED drivers often have better EMI design
- Incandescent or halogen lamps: zero switching noise

**If replacement not possible:**
1. AC line filter on the lamp's power circuit (see Chapter 5, scaled down).
2. Ferrite ring (Mix 75 or Mix 31) on AC cord near lamp.
3. LED panel replacement: replace just the driver board with high-quality alternative
   (Meanwell LDD series or similar EMC-certified driver).

**Note on LED dimmers:** TRIAC dimmers (phase-cut) generate broadband hash in addition
to the LED driver noise. Replace with 0–10V PWM dimmer system with ferrite-filtered
output cable, or eliminate dimming entirely.

### 13.3 Computer Systems

**CPU/GPU VRM noise (200–500 kHz × harmonics into HF):**
1. Ferrite clamp on all cables exiting computer (power, monitor, USB, audio).
2. Computer case grounded to SPG (add ground wire from case to SPG bus).
3. If noise from monitor: ferrite on video cable both ends; grounded DVI/HDMI cable.

**USB 3.0 (2.5 GHz noise):**
1. Ferrite cores (Mix 43 for 2.5 GHz) on USB cables — snap-on type.
2. Shielded USB 3.0 cable.
3. Consider using USB 2.0 for peripherals that don't require high speed.

**Ethernet:**
1. Use shielded Cat6A cable (STP/SFTP).
2. Ground shield at switch end only (not both ends — ground loop risk).
3. Ferrite clamps on unshielded cable (both ends, Mix 31).
4. Consider fiber optic Ethernet (no RF coupling at all) for critical links.

### 13.4 Solar Inverters and Charge Controllers

Solar inverters are frequently the most severe HF noise source in modern homes.
Grid-tie inverters switch at 10–50 kHz and produce harmonics across HF.

**Mitigation options:**
1. Ferrite on DC cables from panels to inverter (Mix 31, multiple turns).
2. Ferrite on AC output cables.
3. Commercial EMI filter on AC output (Schaffner FN2410 or FN2070 series).
4. Verify inverter has current EMC certification; some cheaper inverters fail EMC.
5. Shielded conduit for DC wiring runs.
6. Contact installer/manufacturer — many countries have regulations prohibiting
   equipment that causes interference to licensed radio services.

**Important:** Filing an FCC complaint (in USA) or OFCOM complaint (UK) may be
appropriate if a commercial inverter is found to be non-compliant. Your Technician/
General/Extra license gives you standing as an affected licensed station.

### 13.5 Smart Meters

Smart meters use power-line carrier (PLC) and/or wireless (ZigBee, 900 MHz).
The PLC modulation (30–500 kHz) may be audible on 160m–40m as buzzing or tones.

**Options:**
1. Mains filter (Chapter 5): blocks PLC from entering shack circuits.
2. Contact utility: some utilities will relocate or replace smart meters
   near affected licensed stations.
3. Note: removing or bypassing a utility smart meter is illegal and dangerous.

### 13.6 Plasma TVs (Legacy)

Modern OLED and LCD TVs are much better; plasma TVs were notoriously bad.

If a plasma TV is the source:
1. First try: ferrite clamps on mains power cord (Mix 31, multiple turns wound).
2. Replace TV — plasma technology is obsolete; modern equivalents produce minimal noise.

---

## CHAPTER 14 — TROUBLESHOOTING

### 14.1 Choke Not Reducing Noise

| Symptom | Check | Remedy |
|---------|-------|--------|
| Choke installed, no improvement | Wrong core mix | Verify core markings; FT-240-31 (gray) for HF |
| Choke installed, no improvement | Insufficient turns | Add turns (8+ for FT-240-31) |
| Choke installed, no improvement | Noise radiated, not conducted | Disconnect antenna — does noise persist? |
| Choke installed, noise worse at some frequencies | SRF below operating frequency | Reduce turns; verify SRF with NanoVNA |
| Choke effective, noise returns in wet weather | Moisture in coax | Weatherproof choke housing with amalgamating tape |

### 14.2 Mains Filter Not Reducing Noise

| Symptom | Check | Remedy |
|---------|-------|--------|
| Filter installed, no improvement | Earth ground missing | Verify earth terminal wired to true earth |
| Filter installed, partial improvement | Filter bypassed by unfiltered cable | All shack equipment must use filtered outlets only |
| Filter creates hum on audio | Ground loop through audio cables | Add audio isolation transformer on audio path |
| Filter reduces noise on some bands only | CM choke SRF below noise frequency | Use 2-stage filter; Mix 43 for upper HF |

### 14.3 Active Canceller Not Achieving Null

| Symptom | Check | Remedy |
|---------|-------|--------|
| Null shallow (<10 dB) | Noise is incoherent (multiple sources) | Need separate cancellers for each source |
| Null shallow at some frequencies | Phase network inadequate range | Verify RV1 and RV2 provide full 0–360° |
| Good null at one frequency, poor at others | Phase network frequency-dependent | Retune for each frequency; not broadband without redesign |
| Null but desired signal also cancelled | Reference antenna sees desired signal | Reposition reference; aim null of reference toward signal |
| Null achieved then drifts | Noise source frequency drifting | Normal for some SMPS; retune periodically |

### 14.4 Noise Monitor Inaccurate Readings

| Symptom | Cause | Check |
|---------|-------|-------|
| All bands read same level | Si5351 not changing frequency | Verify I2C connection; Si5351 library installed |
| Readings don't correlate with S-meter | AD8307 calibration error | Apply AD8307_CAL_OFFSET_DB correction; compare to known signal |
| Reading too high (saturated) | Strong broadcast station | Add 10 dB attenuator at monitor antenna input |
| Reading too low (below −75 dBm) | LPF or IF filter issue | Verify 10.7 MHz ceramic filter orientation and soldering |

---

## APPENDIX A — FERRITE CORE QUICK REFERENCE

| Part Number | Mix | OD (mm) | Hole ID (mm) | Max coax OD for 8T |
|-------------|-----|---------|--------------|-------------------|
| FT-140-31 | 31 | 35.6 | 19.0 | 5.5 mm (RG-58) |
| FT-240-31 | 31 | 61.0 | 25.4 | 8.0 mm (RG-8X) |
| FT-240-43 | 43 | 61.0 | 25.4 | 8.0 mm (RG-8X) |
| FT-240-61 | 61 | 61.0 | 25.4 | 8.0 mm |
| FT-140-43 | 43 | 35.6 | 19.0 | 5.5 mm |
| 0431167281 | 31 | 28.5 | 7.0 | snap-on for USB |
| 0443006281 | 43 | 28.5 | 7.0 | snap-on for USB |
| FairRite 0431177281 | 31 | 28.5 | 12.0 | snap-on RG-58 |

Source: Fair-Rite Products Corp. AL values: see Fair-Rite catalog (fair-rite.com).
For Jim Brown's (K9YC) measured impedance data: see his application notes AN-01005.

---

## APPENDIX B — COMMON RFI FREQUENCIES AND SIGNATURES

| Frequency | Typical Source | Signature | Remedy |
|-----------|--------------|-----------|--------|
| 50/60 Hz + harmonics | Power line, dimmers | Tone/buzz at audio | Y-caps, CM choke |
| 25–100 kHz and harmonics | SMPS wall warts | Hash across HF | Replace/filter PSU |
| 65–200 kHz fundamental | Laptop charger | Strong carrier + harmonics | Replace/ferrite |
| 1.8–5 MHz carrier tones | Smart meter PLC | Buzzing tones | Mains filter |
| 14.318 MHz | Computer CPU clock × N | Narrowband birdie | Shielded cable, ferrite |
| 25.000 MHz × N | Crystal oscillator | Narrowband birdie | Filter oscillator |
| 27.120 MHz | ISM band devices | Continuous carrier | Identify device |
| 33–50 MHz harmonics | PWM controllers | Broadband | CM chokes on PWM cables |
| 165 MHz + harmonics | HDMI cable | VHF broadband | Shielded HDMI cable |
| 2500 MHz (down-conversion products) | USB 3.0 | HF hash at 2.5G/N | Ferrite, shielded USB 3.0 |
| Cyclic ~20 s | Fluorescent ballast | Rhythmic S-meter | Replace with LED + filter |
| Cyclic ~20 min | Refrigerator compressor | Burst on start/stop | CM choke on compressor leads |

---

## APPENDIX C — NEC MODEL INTERPRETATION GUIDE

### C.1 dipole_cm_current.nec

Run in xnec2c:
```bash
xnec2c /path/to/dipole_cm_current.nec
```

Model A (comment out GW 3 and CM EX on stub): Shows ideal dipole azimuth pattern.
Model B (include GW 3 and add CM excitation): Shows distorted pattern.

Compare the azimuth patterns at 10° elevation. The filled-in nulls in Model B
represent the noise pickup added by common-mode feed line current.

### C.2 shielded_loop_rfi.nec

Near-field probe pattern verification. Look at:
- Azimuth at elevation=0°: should be figure-8 (bidirectional)
- Broadside elevation pattern: should show deep null perpendicular to loop plane
- Peak: along loop axis (in-plane direction)

### C.3 noise_canceller_array.nec

Run each model separately (comment out other EX cards):
- Model 1: Omnidirectional main vertical
- Model 2: Short reference whip — slightly different pattern
- Model 3: Combined with cancellation — cardioid pattern with null at 0°

The cardioid pattern of Model 3 shows the desired signal preservation (90° direction)
while nulling the noise at 0°.

---

## APPENDIX D — PARTS LIST AND SOURCES

### D.1 Ferrite Cores

| Supplier | Part | Notes |
|----------|------|-------|
| Amidon Associates (amidon.com) | FT-240-31, FT-240-43 | Recommended primary supplier |
| Kits and Parts (kitsandparts.com) | Full range | Ham-friendly quantities |
| Fair-Rite Products (fair-rite.com) | All mixes | Manufacturer direct |
| Mouser Electronics | 5943000201 (FT-240-43 equiv.) | Verifiable specs |

### D.2 Mains Filter Components

| Component | Specification | Supplier |
|-----------|-------------|---------|
| X2 capacitor 0.1µF | 275V X2 | WIMA MKP-X2 / Vishay MKX |
| Y2 capacitor 2200pF | 250V Y2 | Vishay VY1222M31Y5US63V0 |
| CM choke core | FT-240-31 | Amidon |
| MOV | V275LA10A | Littelfuse (Mouser) |
| Fuse holder + fuse | IEC 5×20, 10A slow | Keystone 3577 |
| IEC C14 inlet | PCB or panel mount | CUI Devices GEC14-XXXXX |
| IEC C13 outlet | Panel mount | TE Connectivity 1-770068-1 |

### D.3 Active Noise Canceller Components

| Component | Specification | Notes |
|-----------|-------------|-------|
| J310 JFET | N-channel, VHF | Mouser, DigiKey |
| NE5532 op-amp | Dual, low-noise DIP-8 | Texas Instruments, Philips |
| FT-37-43 toroid | For RF transformer | Amidon |
| 10 kΩ pots | Linear, RK09 series | Bourns, Alps |
| SMA connectors | Edge mount | Rosenberger, Amphenol |

### D.4 Noise Monitor Hardware

| Component | Specification | Notes |
|-----------|-------------|-------|
| Si5351A | I2C clock gen | Adafruit #2045 breakout, $8 |
| SA612AN | DBM mixer DIP-8 | NXP, Mouser |
| SPF5189Z | LNA SOT-89 | Mouser |
| AD8307 | Log amp SOIC-8 | Analog Devices, DigiKey |
| Murata CFWLA10M7KA0-B0 | 10.7 MHz ceramic BPF | Mouser |
| SD card module | SPI, 3.3V | Amazon or AliExpress |

### D.5 Estimated Build Cost

| Assembly | Est. Cost |
|----------|----------|
| Mains EMI filter (PCB + components) | $35–55 |
| 3× coax feed line chokes (6 cores) | $30–45 |
| Active noise canceller | $25–40 |
| Sniffer probe set (2 probes) | $20–35 |
| ESP32 noise floor monitor | $30–45 |
| CYD display | $8–12 |
| Enclosures (filament, 3D printed) | $12–18 |
| **Total** | **$160–250** |

---

*End of Technical Manual TM-RFI-001 Rev A*

*This manual covers construction and operation of receive-side interference
mitigation equipment. Always comply with applicable electrical codes when
working with mains voltage. The Single-Point Ground system described herein
also constitutes best practice for lightning protection; consult a professional
for comprehensive lightning protection installation.*
