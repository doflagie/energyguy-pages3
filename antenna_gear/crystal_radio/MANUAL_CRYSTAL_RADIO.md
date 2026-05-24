# TECHNICAL MANUAL: CRYSTAL RADIO RECEIVERS
## TM-CR-001  Revision B
### AM Broadcast and All Ham Bands 160M through 20cm; Passive and Regenerative Operation

---

**DISTRIBUTION:** Unrestricted  
**PREPARED BY:** Amateur Radio Technical Documentation Project  
**DATE:** 2026  
**SUPERSEDES:** None (Initial Release)

---

## TABLE OF CONTENTS

- [Chapter 1 — Introduction and Theory of Operation](#chapter-1)
- [Chapter 2 — Classic Crystal Radio](#chapter-2)
- [Chapter 3 — Regenerative and Q-Multiplier Designs](#chapter-3)
- [Chapter 4 — TRF (Tuned Radio Frequency) Design](#chapter-4)
- [Chapter 5 — Antenna Coupling and Antenna Systems](#chapter-5)
- [Chapter 6 — Headphone Matching and Audio Output](#chapter-6)
- [Chapter 7 — Variable Capacitor Tuning and Vernier Dial](#chapter-7)
- [Chapter 8 — Construction, Alignment, and Enclosures](#chapter-8)
- [Chapter 9 — Selectivity and Sensitivity Measurement](#chapter-9)
- [Chapter 10 — Troubleshooting](#chapter-10)
- [Chapter 11 — Ham Band Crystal Radios: HF through Microwave](#chapter-11)
- [Appendix A — Component Reference](#appendix-a)
- [Appendix B — Frequency and Component Tables](#appendix-b)
- [Appendix C — Coil Winding Reference](#appendix-c)
- [Appendix D — Performance Data Sheets](#appendix-d)

---

<a name="chapter-1"></a>
## CHAPTER 1 — INTRODUCTION AND THEORY OF OPERATION

### 1.1 Purpose and Scope

This manual covers the design, construction, alignment, and performance measurement of crystal radio receivers for the AM broadcast band (530–1700 kHz) and all amateur radio bands from 160 meters (1.8 MHz) through the 20-centimeter band (1240–1300 MHz).

All designs documented here operate without any battery or power supply.  All energy required for audio output is extracted directly from the received radio signal.

Three primary designs are covered:

| Design | Battery | Selectivity | Sensitivity | Complexity |
|--------|---------|-------------|-------------|------------|
| Classic crystal radio | None | Moderate | Fair | Low |
| Passive Q-multiplier (double-tuned) | None | Good | Good | Moderate |
| Regenerative with transistor | 1.5 V AA | Excellent | Excellent | Moderate |
| TRF triple-tuned | None | Very good | Good | High |

### 1.2 Crystal Radio Operating Principle

A crystal radio operates as follows:

**Step 1 — Signal Capture:**  
The antenna (ferrite rod loopstick or long wire) intercepts electromagnetic waves from AM broadcast transmitters.  The antenna converts the radio wave's electric field into a voltage across the tank circuit.

**Step 2 — Frequency Selection (Tuning):**  
The tank circuit (inductor L + variable capacitor C) resonates at a single frequency.  At resonance, the circuit presents maximum impedance, concentrating energy at the selected frequency and rejecting others.

```
Resonant frequency:   f = 1 / (2π√LC)
Bandwidth (−3 dB):   BW = f₀ / Q
Q factor:             Q = 2πf₀L / R_loss
```

**Step 3 — Detection (Demodulation):**  
The AM carrier contains a high-frequency RF component (the carrier) amplitude-modulated by audio frequencies.  The germanium diode rectifies the RF signal (passes only positive half-cycles), and the RF bypass capacitor (C_bypass) filters out the RF, leaving only the audio envelope.

**Step 4 — Audio Output:**  
The demodulated audio signal drives a high-impedance crystal earphone directly, or an 8 Ω headphone through an impedance-matching transformer.

### 1.3 AM Signal Structure

An AM broadcast signal:

```
V(t) = A_c [1 + m·cos(2πf_a·t)] · cos(2πf_c·t)

Where:
  A_c = carrier amplitude
  m   = modulation depth (0 to 1; 100% modulation typical)
  f_a = audio frequency (300–5000 Hz for voice; up to 10 kHz for music)
  f_c = carrier frequency (530–1700 kHz)
```

After detection:  
```
V_audio = A_c · m · cos(2πf_a·t)   [envelope extracted by diode + C_bypass]
```

The detected audio voltage is small: typically 10–200 mV peak at the detector output for a receivable signal.

### 1.4 Why Germanium Diode — NOT Silicon

Crystal radios produce very small RF voltages at the tank circuit.  The detector diode must begin conducting at these small voltages.

| Diode type | Forward voltage V_f | Minimum signal for detection |
|-----------|--------------------|-----------------------------|
| 1N34A germanium | 0.15–0.25 V | < 0.5 mV/m (good sensitivity) |
| OA47, OA79 germanium | 0.10–0.20 V | Comparable to 1N34A |
| BAT43 Schottky | 0.20–0.30 V | Marginally usable |
| 1N914, 1N4148 silicon | 0.55–0.65 V | **WILL NOT WORK in most crystal radios** |
| 1N4001 silicon | 0.65–0.70 V | **WILL NOT WORK** |

**Critical Note:** Never substitute silicon signal diodes in a crystal radio.  The 1N914 and 1N4148 are the most commonly available small-signal diodes but are absolutely unsuitable for crystal radio detection.  Always use germanium (1N34A, OA47, OA91, OA79, AAZ15, 1N270) or Schottky (BAT43, 1N5711) types.

### 1.5 Ferrite Rod Antenna Theory

The ferrite rod antenna (loopstick) functions as both a magnetic loop antenna and a high-Q inductor for the tank circuit.

**Magnetic flux concentration:**  
Ferrite material (µ_r = 125 to 2500) concentrates the ambient magnetic field through the coil, increasing the effective area of the antenna.

```
Effective aperture:  A_eff = µ_r × (physical_area_of_coil)
Antenna sensitivity: V_antenna = 2π × f × B_field × A_eff
```

For mix-77 ferrite (µ_r = 2000), a 9.5 mm rod with 50 mm winding has:
```
Physical area = π × (0.00475)² × 0.050 = 3.54 × 10⁻⁶ m²
Effective area = 2000 × 3.54×10⁻⁶ = 7.08 × 10⁻³ m² (≈ 70 cm²)
```

This is equivalent to a 94 mm × 94 mm air-core loop — useful even without an external antenna.

**Ferrite rod mix selection:**

| Mix | µᵢ | Best frequency range | Notes |
|-----|------|---------------------|-------|
| Mix 61 | 125 | 200 kHz – 10 MHz | Low loss; good Q; lower sensitivity |
| Mix 77 | 2000 | 10 kHz – 2 MHz | Higher sensitivity; slightly lower Q |
| Mix 73 | 2500 | 10 kHz – 1 MHz | Highest sensitivity for AM broadcast |
| Mix 43 | 850 | 100 kHz – 10 MHz | Not recommended (lossy at audio) |

**Recommendation:** Use mix-77 for maximum sensitivity on AM broadcast.  Use mix-61 for 160M operation where lower loss at 2 MHz is important.

---

<a name="chapter-2"></a>
## CHAPTER 2 — CLASSIC CRYSTAL RADIO

### 2.1 Circuit Description

The classic crystal radio consists of four elements:

1. **Ferrite rod tank circuit** (L1 + C1): selects the station
2. **Antenna coupling tap**: connects antenna to tank with minimal loading
3. **Germanium diode detector** (D1): rectifies AM envelope
4. **RF bypass capacitor** (C2) + earphone: filters and reproduces audio

**Schematic overview:**
```
ANT ─── [ant tap on L1] ─── L1 (220µH) ─┬─ [D1: 1N34A] ─── AF out
                                          │                       │
                                       [C1: 365pF]             [C2: 0.01µF]
                                          │                       │
                                         GND ────────────────── GND ── [earphone]
```

### 2.2 Component Specifications

**L1 — Ferrite Rod Coil (220 µH):**
- Core: 7-inch (178 mm) ferrite rod, 9.5 mm diameter, mix-77 preferred
- Winding: 110 turns of #28 AWG enameled copper (magnet wire)
- Wind length: 50 mm; single layer, close-wound
- Inductance: 220 µH ± 10% (adjust by spreading or compressing turns)
- Taps: at 20 turns (low-Z ant), at 50 turns (medium ant)
- Q (measured): 80–150 on good core; 50–80 typical

**C1 — Variable Capacitor (365 pF):**
- Type: single-gang air variable, broadcast type
- Range: 15 pF (minimum, plates fully open) to 365 pF (fully meshed)
- Shaft: 6 mm; use 10:1 vernier dial for fine tuning
- Tuning range with L1=220µH:
  - C_max = 365 pF: f_min = 563 kHz (covers 530 kHz AM low end)
  - C_min = 15 pF: f_max = 2.77 MHz (covers 160M amateur)

**D1 — Germanium Detector:**
- Part: 1N34A (primary recommendation)
- Alternates: OA79, OA47, OA91, AAZ15, 1N270
- Polarity: anode toward tank, cathode toward earphone output
- **Never substitute silicon diodes.**

**C2 — RF Bypass:**
- Value: 0.01 µF ceramic disc (10 nF)
- Purpose: shorts RF to ground; passes only audio envelope
- Alternatives: 1000 pF (narrower audio), 0.1 µF (wider; more bass)

### 2.3 Performance Specifications

| Parameter | Value | Conditions |
|-----------|-------|------------|
| Frequency range | 530–1700 kHz | AM broadcast |
| Optional 160M range | 1.8–2.0 MHz | See Section 2.6 |
| Q (unloaded coil) | 80–150 | Ferrite rod, #28 AWG, mix-77 |
| Selectivity (−3 dB) | 7–13 kHz at 1 MHz | Q = 80–150 |
| Minimum detectable E | 5–20 mV/m | 15 m long wire, good ground |
| Audio output (strong stn) | 0.1–2 mW | Crystal earphone |
| Range (50 kW AM) | 10–100 km | Depends on antenna and ground |
| Battery required | None | Fully passive |

### 2.4 Coil Winding Procedure

**Materials:**
- 7-inch (178 mm) ferrite rod, 9.5 mm diameter, mix-77
- #28 AWG enameled (magnet wire), ~20 m
- 3D-printed coil sleeve (coil_former.scad)
- Nail varnish (for securing winding ends)

**Procedure:**

1. Slide coil sleeve onto rod; position at center (50 mm from one end).

2. Thread wire end through bottom flange slot; leave 75 mm lead.  This is the GND/cold end.

3. Wind 20 turns close-wound, single layer.  After turn 20, bring out a loop of wire through the flange slot.  Label this loop **ANT TAP**.  Continue winding without cutting wire.

4. Continue winding to turn 50.  Bring out another loop and label **MED TAP**.  Continue winding.

5. Wind to 110 turns total.  Thread wire end through top flange slot; leave 75 mm lead.  This is the HOT/detector end.

6. Apply one drop of nail varnish to each end of the winding to prevent unwinding.

7. Measure inductance with LC meter:
   - Target: 220 µH ± 10%
   - Too low (< 200 µH): compress turns closer together
   - Too high (> 242 µH): spread turns apart slightly

8. Slide coil assembly to desired position on rod.  Optimal position: 30–50 mm from one end (adjust for best signal).

**Inductance vs. Turns (approximate, 9.5 mm rod, mix-77, 50 mm winding):**

| Turns | Inductance (µH) |
|-------|----------------|
| 50 | ~50 |
| 80 | ~130 |
| 100 | ~185 |
| 110 | ~220 |
| 130 | ~295 |
| 150 | ~390 |

### 2.5 Detector Circuit Assembly

Detector may be assembled on a small tagboard (terminal strip) or PCB.

```
ASSEMBLY POINTS:
  TP1 — Tank hot end (top of L1/junction of C1 and L1)
  TP2 — AF output (cathode of D1)
  TP3 — Ground

  TP1 ─── [D1 anode] ─── [D1 cathode] ─── TP2
                                             │
                                         [C2: 0.01µF]
                                             │
                                            TP3 (GND)

  TP2 ─── [J2 tip: 3.5mm] ─── earphone (+)
  TP3 ─── [J2 sleeve] ─────── earphone (−)

  Optional: [R1: 470 kΩ] between TP2 and TP3
  R1 provides a DC return path and prevents static charge buildup on earphone.
  Use with crystal earphones that have no internal resistance path to ground.
```

### 2.6 160-Meter (1.8–2.0 MHz) Variant

The same detector circuit is used.  Replace the ferrite rod coil with an air-core coil:

**Air-core coil (78 µH for 160M):**
- Form: 50 mm diameter × 50 mm long (print from aircore_form in coil_former.scad)
- Wire: #24 AWG enameled
- Turns: 30 turns, single layer
- L = 78 µH (calculated); verify with LC meter; target 75–82 µH
- No ferrite required; ferrite losses increase at 2 MHz

**Capacitor:**  
Use 100 pF fixed + 100 pF variable (or 200 pF variable if available).

```
f at C=200pF: f = 1/(2π√(78µH × 200pF)) = 1.27 MHz (too low)
f at C=100pF: f = 1/(2π√(78µH × 100pF)) = 1.80 MHz ✓ (bottom of 160M)
f at C=50pF:  f = 1/(2π√(78µH × 50pF))  = 2.55 MHz (top of range)
```

For best coverage of 1.8–2.0 MHz: 100 pF fixed + 50 pF variable, or adjust winding turns.

**Antenna for 160M:**  
Long-wire minimum 10 m; 20–30 m much better.  Good earth ground essential.

---

<a name="chapter-3"></a>
## CHAPTER 3 — REGENERATIVE AND Q-MULTIPLIER DESIGNS

### 3.1 Design A — Passive Double-Tuned Q-Multiplier (Zero Battery)

**Principle:**  
Two identical tuned circuits coupled together provide improved selectivity over a single circuit.  At critical coupling (k = 1/Q), the response is maximally flat with twice the equivalent Q.

**Critical coupling coefficient:**
```
k_critical = 1/Q_single

For Q = 100: k_critical = 0.01 (1% coupling between coils)
```

**Performance improvement:**

| Configuration | −3 dB BW at 1 MHz | −40 dB BW at 1 MHz | Adjacent rejection |
|---------------|-------------------|--------------------|--------------------|
| Single-tuned, Q=80 | 12.5 kHz | 397 kHz | ~10 dB at 10 kHz |
| Double-tuned, Q=80 | 7.3 kHz | 84 kHz | ~18 dB at 10 kHz |

**Components:**
- L1, L2: 220 µH each (identical windings on same ferrite rod)
- C1, C2: 365 pF each (dual-gang variable capacitor, same shaft)
- D1: 1N34A (same as classic)
- C3: 0.01 µF RF bypass

**Dual-gang capacitor:**  
Both L1C1 and L2C2 must track in frequency.  A dual-gang 365+365 pF variable capacitor (two sections on same shaft) is required.  Tracking error is typically < 2 pF across the band for quality air variables.

**Coupling adjustment:**  
Slide L2 along the ferrite rod toward or away from L1.  
- Too close (< 10 mm gap): double-humped response (two peaks when sweeping)
- Correct (15–25 mm gap): single sharp peak, maximum volume
- Too far (> 40 mm gap): attenuated signal; very narrow, quiet

Use the coupling slider from coil_former.scad for repeatable positioning.

### 3.2 Design B — Transistor Regenerative (1.5 V)

**Principle:**  
A transistor amplifies a portion of the tank signal and injects it back in-phase through a tickler coil.  This reduces effective tank resistance and raises Q dramatically.

```
Q_effective = Q₀ / (1 − A×β)

At A×β = 0.9:  Q_eff = 10 × Q₀
At A×β = 0.99: Q_eff = 100 × Q₀
At A×β = 1.0:  Q_eff → ∞ (oscillation — avoid this)
```

**WARNING:** If regeneration is advanced past the oscillation threshold, the receiver becomes a transmitter.  It will heterodyne with other stations on the dial and cause audible interference on nearby AM radios.  Always operate just below oscillation.  The whistle or hiss in the earphone marks the oscillation threshold; back off VR1 until silence resumes, then advance slightly.

**Tickler coil L2:**
- 20 turns #28 AWG, wound adjacent to L1 on same ferrite rod
- Position: at ground end of L1
- Winding direction: reverse polarity from L1 (experiment; if regen decreases when advancing VR1, reverse L2 leads)

**Biasing:**
```
+1.5V ─── [R_c: 470Ω] ─── collector Q1 ─── top of L2 tickler
                                 Q1 (2N3904)
base ─── [R_b: 10kΩ] ─── [VR1: 100kΩ regen pot] ─── GND
emitter ─── GND
```

**Regen control procedure:**
1. Set VR1 to minimum (regen off).
2. Tune C1 to receive a station.
3. Slowly advance VR1.  Station becomes louder and clearer.
4. Stop when a hiss or whistle begins to appear.
5. Back VR1 off slightly until station is clean and selective.
6. This is the operating point.

**Battery current drain:** 0.5–2 mA from 1.5 V AA cell → 300–600 hours battery life.  Detection is still passive; battery only energizes the Q-multiplier transistor.  If battery fails, the receiver reverts to standard single-tuned crystal radio operation.

### 3.3 Sensitivity Comparison

| Design | Min. detectable E | Bandwidth | Battery |
|--------|------------------|-----------|---------| 
| Classic crystal | 10–20 mV/m | 15 kHz | None |
| Passive double-tuned | 5–10 mV/m | 8 kHz | None |
| Regenerative (1.5V) | 0.2–1 mV/m | 1–3 kHz | 1.5V AA |

The regenerative design approaches superheterodyne performance for strong-signal AM reception.

---

<a name="chapter-4"></a>
## CHAPTER 4 — TRF (TUNED RADIO FREQUENCY) DESIGN

### 4.1 Principle

A TRF receiver uses multiple identical resonant circuits all tuned to the same frequency.  Cascading n stages multiplies the selectivity benefit:

```
BW_n = BW₁ × √(2^(1/n) − 1)

Where:
  BW₁ = bandwidth of single stage (f₀/Q)
  n = number of cascaded identical stages
  BW_n = bandwidth of n-stage system

Examples at 1 MHz, Q=80 (BW₁ = 12.5 kHz):
  n=1: BW = 12.5 kHz         (reference)
  n=2: BW = 12.5 × 0.644 = 8.0 kHz
  n=3: BW = 12.5 × 0.510 = 6.4 kHz  (TRF triple-tuned)
```

### 4.2 Triple-Tuned TRF Design

Three stages with capacitive inter-stage coupling.

**Stage coupling:**  
Coupling capacitors C_k12 and C_k23 connect successive stages.  Smaller coupling capacitor = narrower passband = more selective.

| C_k (pF) | Coupling type | Response |
|----------|--------------|---------|
| 5 pF | Undercoupled | Very narrow; some insertion loss |
| 10 pF | Near-critical | Best balance (recommended) |
| 15 pF | Critical | Maximum flat passband |
| 22 pF | Overcoupled | Double hump |
| 47 pF | Overcoupled | Two distinct peaks |

Start with 10 pF NP0/C0G capacitors.  Adjust for maximum volume with single-peaked response.

**Triple-gang variable capacitor options:**

- **Option A:** True triple-gang 365 pF variable (vintage radio suppliers, surplus).  All three sections on one shaft; tracking ±2 pF.
- **Option B:** Three separate 365 pF capacitors with ganged shaft via 3D-printed flexible coupling.  Requires alignment.
- **Option C:** Dual-gang for stages 1 and 2; fixed capacitor on stage 3 resonated at band center (1115 kHz).  Compromise design; stage 3 gives selectivity boost only at band center.

### 4.3 Selectivity Comparison

| Design | −3 dB BW | −40 dB BW | Adjacent (10 kHz off) |
|--------|---------|----------|----------------------|
| Classic (Q=80) | 12.5 kHz | 397 kHz | −3 dB |
| Double-tuned | 8 kHz | 84 kHz | −18 dB |
| Triple-tuned | 5 kHz | 40 kHz | −25 dB |

For AM broadcasting, adjacent channel spacing is 10 kHz (North America) or 9 kHz (ITU).  Triple-tuned provides −25 dB adjacent channel rejection, adequate to separate most co-channel situations.

### 4.4 Tracking Alignment

All three gangs must be at the same resonant frequency simultaneously.

**Equipment:** Signal generator (or known local station) + earphone

**Procedure:**
1. Set all capacitors to mid-range.
2. Inject test signal at 1000 kHz into antenna terminal.
3. Peak L1 by sliding coil position on rod (or adjusting L1 trimmer).
4. Peak L2 (inter-stage coupling input) similarly.
5. Peak L3 (output stage) similarly.
6. Repeat steps 3–5 until no further improvement.
7. Check tracking at 600 kHz (low end): adjust low-end trimmers if available.
8. Check tracking at 1600 kHz (high end): adjust high-end trimmers.

Note: Triple-gang air variables often have individual gang trimmer screws.  Use these for end-of-band tracking corrections.

---

<a name="chapter-5"></a>
## CHAPTER 5 — ANTENNA COUPLING AND ANTENNA SYSTEMS

### 5.1 Antenna Selection

| Antenna type | Signal level | Directivity | Best use |
|-------------|-------------|------------|---------|
| Ferrite rod (loopstick) | Moderate | Figure-8 | Compact; local stations |
| 1 m whip | Low–Moderate | Omnidirectional | Indoor; portable |
| 5–10 m wire | Good | Omnidirectional | Outdoor; suburban |
| 15–20 m wire | Excellent | Omnidirectional | Best for DX |
| 30+ m wire | Maximum | Omnidirectional | DX; requires good ground |

### 5.2 Antenna Coupling Methods

**Method 1 — Direct tap:**  
Connect antenna wire to low-impedance tap on L1 (20 turns from GND end).  
Simplest.  Some tank loading; acceptable for local reception.

**Method 2 — Link coupling (recommended for long wire):**  
Wind 5 turns of #28 AWG adjacent to GND end of L1.  Connect antenna and ground to this link winding.  No direct connection to main tank.  Tank Q is fully preserved.

```
Link coil transformer ratio: N_main/N_link = 110/5 = 22:1
Link port impedance (at 1 MHz): ~400 Ω  [matches long-wire feed impedance]
```

**Method 3 — Series capacitor:**  
Connect a 10–47 pF NP0 cap between antenna and tank HOT end.  
Sets coupling level independently of winding.  Easy to adjust.

**Method 4 — Tuned loop:**  
Separate 1 m loop, resonated with ~2500 pF total variable capacitor.  
Coupled to crystal radio tank by proximity.  Very directional; useful for DX.

### 5.3 Ground System

The ground (earth connection) is as important as the antenna.  

**Ground resistance targets:**
- < 1 Ω: excellent; maximum sensitivity
- 1–10 Ω: adequate for most purposes
- 10–50 Ω: fair; some sensitivity loss
- > 50 Ω: poor; significant loss; use counterpoise

**Counterpoise:**  
When earth ground is unavailable (upper floors, apartments), lay 5–20 m of wire on the floor or along a baseboard.  Connect to crystal radio GND terminal.  Not as effective as earth ground but usable.

### 5.4 Ferrite Rod Orientation

The ferrite rod has a figure-8 radiation/reception pattern:
- **Maximum:** when rod is broadside to the transmitter (perpendicular to signal direction)
- **Null:** when rod is end-on to the transmitter (rod points toward station)

Use this property to:
1. Null a strong local station to hear a weaker station on the same frequency.
2. Determine bearing to an AM transmitter (null direction = toward or away from station).

---

<a name="chapter-6"></a>
## CHAPTER 6 — HEADPHONE MATCHING AND AUDIO OUTPUT

### 6.1 Impedance Matching Requirement

The crystal radio detector develops audio across its output impedance.  Connecting the wrong load impedance wastes nearly all available power.

**Available power calculation:**  
Maximum power is delivered when load impedance equals source impedance.  For a crystal radio with Z_source = 20 kΩ:

| Load type | Load Z | Power delivered | Loss vs. matched |
|-----------|--------|----------------|-----------------|
| Crystal earphone | 500 kΩ | 96% | −0.2 dB |
| Crystal earphone | 100 kΩ | 80% | −1.0 dB |
| 2000 Ω magnetic | 2 kΩ | 9% | −10.5 dB |
| 8 Ω headphone (direct) | 8 Ω | 0.04% | −34 dB |
| 8 Ω headphone (50:1 xfmr) | 20 kΩ | 92% | −0.35 dB |

A matching transformer with 50:1 turns ratio recovers 34 dB of audio power when using 8 Ω headphones.

### 6.2 Transformer Design

**Turns ratio for 8 Ω headphones:**

```
N = √(Z_source / Z_load)

For Z_source = 20 kΩ, Z_load = 8 Ω:
N = √(20000/8) = √2500 = 50:1

Primary turns:   500 (on T-94A-77 Mix 77 toroid, #36 AWG)
Secondary turns: 10  (#28 AWG)
```

**Core:** Amidon T-94A-77 (Mix 77, A_L = 900 nH/N²)

**Primary inductance (at 500 turns):**
```
L = A_L × N² = 900 nH × (500)² = 225 H
```
This provides excellent low-frequency response (f_low = Z_source/(2π × L) = 14 Hz).

**Winding procedure (abbreviated):**
1. Wrap core with 2 layers electrical tape.
2. Wind secondary first: 10 turns #28 AWG.  Mark leads S+ and S−.
3. Wind primary: 500 turns #36 AWG.  Wind in same direction as secondary.  Mark leads P+ and P−.
4. Strip enamel, tin, and test.  Primary R = 150–400 Ω typical.
5. Verify polarity: P+ → S+ (same winding sense).

**Frequency response:** ±1 dB from 100 Hz to 15 kHz (exceeds AM audio bandwidth).

### 6.3 Earphone Types

**Crystal earphone (piezoelectric):**  
- Impedance: 50 kΩ – 2 MΩ
- Sensitivity: very high (best for weak signals)
- No transformer needed
- Fragile; vulnerable to voltage spikes
- Connect 470 kΩ–1 MΩ shunt resistor across earphone for DC protection

**High-impedance magnetic earphone (vintage):**  
- Impedance: 2000–4000 Ω
- Good sensitivity; more robust than crystal type
- Small transformer (5:1) may improve matching

**Modern 8 Ω headphones:**  
- Require 50:1 transformer (see Section 6.2)
- Lower sensitivity per watt but comfortable to wear
- Good for strong local stations; marginal for DX

**Speaker (8 Ω, full-size):**  
- Require same 50:1 transformer
- Usable only with very strong stations or regenerative design
- Audio power from crystal radio is typically 0.1–2 mW; barely audible on speaker

---

<a name="chapter-7"></a>
## CHAPTER 7 — VARIABLE CAPACITOR TUNING AND VERNIER DIAL

### 7.1 365 pF Broadcast Variable Capacitor

The 365 pF air-variable is the standard capacitor for AM broadcast crystal radios.  Its full-range capacitance change (15–365 pF) covers the entire AM band with a single coil.

**Tuning law:**  
The capacitance vs. rotation relationship is approximately logarithmic (designed for linear frequency tuning):
```
C(θ) ≈ C_min + (C_max − C_min) × sin²(θ/2)

Where θ = rotation angle (0 to 180° typical full range)
```

For linear frequency dial calibration, use the reciprocal relationship:
```
f(θ) = 1 / (2π√(L × C(θ)))
```

**Capacitance per degree:**  
At 1 MHz, C ≈ 115 pF.  1° rotation ≈ 1.9 pF → frequency change ≈ 8.3 kHz per degree.  
Direct drive is too coarse for fine tuning; 10:1 reduction is strongly recommended.

### 7.2 Vernier Dial

The 3D-printed vernier dial (enclosure_crystal_radio.scad: vernier_dial()) provides a 10:1 mechanical reduction.

**Specifications:**
- Dial diameter: 80 mm
- Scale: 0–100 divisions (10 major, 10 minor per major)
- Effective resolution: < 0.5 pF per division
- Pointer: fixed on enclosure; dial rotates

**Scale calibration procedure:**
1. Set C1 to maximum (plates fully meshed): 365 pF.  Mark dial "0" or "MAX."
2. Tune to a known station (e.g., 1000 kHz if a 1 MHz station exists locally).  Record dial reading.
3. Tune to another known station (e.g., 700 kHz and 1400 kHz).  Record readings.
4. Plot frequency vs. dial position; fit a calibration curve.
5. Engrave or mark the calibrated dial face.

**Dual-gang alignment:**  
For double-tuned or TRF designs, the two (or three) gangs may not track perfectly.  Individual trimmer capacitors (5–10 pF) across each section allow tracking correction.

### 7.3 Dual-Gang and Triple-Gang Options

**Single-gang:** 365 pF; for classic crystal radio only.

**Dual-gang:** 365+365 pF; for passive double-tuned Q-multiplier.  Track both L1C1 and L2C2 simultaneously.  Surplus types from vintage AM radios.

**Triple-gang:** 365+365+365 pF; for TRF triple-tuned design.  Rare; available from vintage radio suppliers or fabricated from three singles with shaft couplers.

---

<a name="chapter-8"></a>
## CHAPTER 8 — CONSTRUCTION, ALIGNMENT, AND ENCLOSURES

### 8.1 Construction Approach

Crystal radios are traditionally built on open baseboards.  This has several practical advantages:
- All components accessible for measurement
- No shielding interactions to affect Q
- Visual appeal (traditional aesthetic)
- Easy to modify and experiment

The printed baseboard (enclosure_crystal_radio.scad) provides:
- Ferrite rod saddle brackets
- Variable capacitor mount with vernier dial support
- Detector tagboard standoffs
- Binding posts for ANT and GND
- 3.5 mm earphone jack mount

### 8.2 Component Layout

Recommended layout for classic crystal radio (left-to-right):

```
[ANT post]──[GND post]    [earphone jack]
[L1 ferrite rod ───────────────────────]
              [C1 variable cap]
              [D1 detector]
              [C2 RF bypass]
```

Critical spacing rules:
- Keep D1 and C2 close to the tank hot end.  Long leads add stray capacitance and reduce Q.
- Route all ground connections to a single star ground point near C1/GND terminal.
- Keep antenna lead as short as possible (< 100 mm) from binding post to tap.
- No RF-conducting materials (metal) within 50 mm of the ferrite rod ends.

### 8.3 3D-Printed Parts

**coil_former.scad — Parts:**

| Part | Qty | Description |
|------|-----|-------------|
| coil_sleeve_half | 2 | Main coil sleeve (two halves snap together) |
| coupling_slider | 1 | Secondary coil position, adjustable |
| aircore_form | 1 | 160M air-core coil form (50 mm dia) |
| rod_end_cap | 2 | Secures rod in saddle brackets |
| coil_spacer | 2–3 | Maintains coil separation in TRF/double-tuned |
| winding_shuttle | 1 | Assists winding fine wire onto toroid cores |

**enclosure_crystal_radio.scad — Parts:**

| Part | Qty | Description |
|------|-----|-------------|
| baseboard | 1 | Main platform, 200×120×12 mm |
| vernier_dial | 1 | 80 mm dial for variable capacitor |
| cap_bracket | 1 | Variable capacitor mounting pedestal |
| rod_saddle | 2 | Ferrite rod saddle mounts |
| rod_saddle_cap | 2 | Snap-over rod retaining caps |
| pcb_standoff | 4 | For detector tagboard |
| regen_knob | 1 | For regeneration control pot (Design B) |

**Print settings (PETG):**
- Layer height: 0.2 mm
- Perimeters: 3 (for strength)
- Infill: 25–40% (gyroid or honeycomb)
- Temperature: 230°C nozzle, 80°C bed
- Cooling: minimal (50% max for PETG adhesion)

**Do not use conductive filament** (carbon-loaded PLA/PETG) near the tank circuit or ferrite rod.  Conductive material will absorb RF and kill Q.

### 8.4 Soldering Notes

- Use rosin-core solder (60/40 or 63/37).
- Keep solder joints small and clean.  Large solder blobs add stray capacitance.
- Use short, direct lead lengths.  Every 10 mm of wire above the baseboard adds ~10 pF.
- The 1N34A germanium diode is sensitive to heat.  Hold with needle-nose pliers as heat sink, and solder quickly (< 3 seconds).
- Strip enamel from magnet wire with fine sandpaper (400 grit); tin immediately after stripping.

### 8.5 Initial Alignment

1. **Verify coil inductance** before installation.  Target: 220 µH ± 10%.
2. **Verify capacitor range:** check with DMM capacitance function or LC meter; confirm 15–365 pF swing.
3. **Check diode polarity:** anode connects to tank HOT end (cathode banded end goes toward earphone).
4. **Verify C2 (RF bypass):** 0.01 µF.  Ceramic disc or monolithic ceramic; any lead orientation.
5. **Tune to a strong station:** rotate C1 slowly through full range while listening.  Station should appear as clear audio at one capacitor position.
6. **Check both ends of band:** minimum C (plates open) should reach 530–600 kHz; maximum C should tune 1400–1700 kHz.  Adjust L1 position or turn count if band limits are wrong.
7. **Null check:** rotate ferrite rod to find directional null on a station.  Null should be 90° off maximum.  If no null is observed, check that no metal objects are within 50 mm of rod ends.

---

<a name="chapter-9"></a>
## CHAPTER 9 — SELECTIVITY AND SENSITIVITY MEASUREMENT

### 9.1 Selectivity Measurement

**Equipment required:**
- Signal generator (calibrated output level, sweep or step frequency)
- RF voltmeter, spectrum analyzer, or SDR receiver for reference
- Earphone for audio monitoring OR 1 kΩ load resistor + DMM (AC mV)

**Method 1 — Bandwidth measurement (−3 dB):**

1. Inject AM signal at 1000 kHz, modulated 400 Hz, from signal generator through 50 Ω source (or directly to ANT terminal via 100 pF coupling cap).
2. Tune crystal radio to peak audio output.  Record this as reference level P₀.
3. Step generator frequency above 1000 kHz in 1 kHz increments.  Record audio level at each step.
4. Find frequency where level drops to P₀/√2 (= P₀ − 3 dB, or 70.7% of peak voltage).
5. This frequency offset is the upper half-bandwidth.  Repeat for lower half.
6. Total −3 dB BW = upper offset + lower offset.

**Method 2 — Using SDR as reference:**

1. Connect SDR dongle to the crystal radio's antenna terminal (via 100 pF series cap).
2. Use SDR++ or GQRX to observe the received spectrum.
3. Set crystal radio to a station.  The tank circuit selectivity will shape the spectrum visible on the SDR.
4. Measure −3 dB points directly on SDR waterfall/spectrum display.

**Typical results:**

| Design | Measured −3 dB BW | Notes |
|--------|-----------------|-------|
| Classic (Q=80) | 10–15 kHz | Good single-layer ferrite coil |
| Classic (Q=150) | 5–8 kHz | Excellent coil, mix-77 core |
| Double-tuned | 6–9 kHz | Dual-gang, critical coupling |
| Regenerative | 1–5 kHz | Depends on regen setting |
| TRF triple-tuned | 4–7 kHz | Three coils, 10 pF coupling caps |

### 9.2 Sensitivity Measurement

**Minimum detectable signal:**  
The minimum E-field (mV/m) that produces just-audible audio.

**Method:**
1. Position a calibrated field-strength meter and loop antenna outdoors.
2. Reduce crystal radio antenna to the ferrite rod only (disconnect long wire).
3. Record the minimum E-field at which the station is just barely audible.
4. Express in mV/m.

**Typical results:**

| Antenna | Signal level for barely audible output |
|---------|--------------------------------------|
| Ferrite rod only | 20–50 mV/m |
| 1 m whip on rod | 10–20 mV/m |
| 10 m wire, earth ground | 2–5 mV/m |
| 20 m wire, ground rod | 1–2 mV/m |
| Regenerative (1.5V) | 0.2–0.5 mV/m |

### 9.3 Audio Power Measurement

**Method:**
1. Replace earphone with precision 470 kΩ resistor across detector output.
2. Measure AC voltage with true-RMS DMM.
3. Calculate power: P = V² / R = V² / 470000.

**Example:** 100 mV measured across 470 kΩ:  
P = (0.1)² / 470000 = 21 nW  

For crystal earphone (1 MΩ): recalculate with earphone impedance.

---

<a name="chapter-10"></a>
## CHAPTER 10 — TROUBLESHOOTING

### 10.1 No Audio

| Symptom | Possible cause | Check / Fix |
|---------|---------------|------------|
| No audio at any tuning position | Diode reversed | Check D1 polarity: anode to tank, cathode to output |
| No audio at any position | Diode open | Test D1 on DMM diode setting (should read ~0.2 V forward) |
| No audio, earphone OK | C2 shorted | Remove C2; test without it; replace if shorted |
| No audio, C1 range wrong | Coil inductance off | Measure L1; adjust turns |
| No audio | Broken wire | Check all connections with DMM continuity |
| No audio (regen design) | Battery dead | Check battery voltage; replace |

### 10.2 Poor Selectivity

| Symptom | Cause | Fix |
|---------|-------|-----|
| Several stations heard at once | Q too low | Check for metal near coil; remove stray conductors |
| Wide, flat tuning curve | C2 too large | Reduce C2 to 1000 pF (narrower AF, sharper tuning) |
| Poor selectivity | Antenna over-coupled | Move to lower-impedance tap or reduce link turns |
| Double-tuned: two peaks | Over-coupled | Separate L2 further from L1 |
| Can't null a station | Metal near rod | Move rod away from metal chassis or brackets |

### 10.3 Weak Signal

| Symptom | Cause | Fix |
|---------|-------|-----|
| Very faint audio | Poor ground | Improve ground connection; add ground rod |
| Weak audio, good selectivity | Short antenna | Extend antenna wire |
| Weak audio | Diode threshold too high | Replace diode; try OA47 or OA91 (lower V_f) |
| Weak audio, regen design | Regen too low | Advance VR1 |
| Weak audio (all stations) | C1 range wrong | Check C1 tracks full 15–365 pF range |

### 10.4 Regen Oscillation

| Symptom | Cause | Fix |
|---------|-------|-----|
| Whistling in earphone | Oscillation | Back off VR1; reduce regen |
| Cannot receive any station (oscillating) | Tickler wrong direction | Reverse L2 connections |
| Oscillates at low regen setting | Loop gain too high | Reduce R_c from 470Ω to 1 kΩ |
| Oscillates and creates interference | Operating above threshold | Back off VR1; never operate in oscillation |

### 10.5 Mechanical Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| Tuning jumpy, station skips | Loose cap shaft | Tighten shaft bearing; apply small amount of lubricant |
| Dial doesn't return to same position | Backlash | Use vernier dial; reduce drive shaft play |
| Coil slips on rod | Loose sleeve | Tighten sleeve split; add small wedge of tape |
| Rod rattles | End cap loose | Tighten rod end cap set screws |

---

<a name="chapter-11"></a>
## CHAPTER 11 — HAM BAND CRYSTAL RADIOS: HF THROUGH MICROWAVE

### 11.1 What Modes Are Detectable — Critical Caveat

A crystal radio is an AM envelope detector.  It recovers audio only when the received signal has an AM-modulated carrier — a continuous sine wave whose amplitude varies at audio rates.

**Detectable without modification (passive crystal radio):**
- AM phone (A3E): full carrier AM, 100% modulation
- CW (A1A): on/off keying produces audio clicks; with regen, produces tone
- SSTV and FAX carriers: AM-detected envelope is an image signal
- ATV (fast-scan TV): vestigial sideband with picture carrier; AM-detectable
- Unmodulated carrier: produces DC only (no audio; useful for RF detection)

**NOT detectable by passive crystal radio:**
- SSB (J3E): no carrier; produces unintelligible garble without a BFO
- FM (F3E): frequency variations produce no amplitude change; no audio output
- Digital modes (FT8, JS8, PSK31): narrow-band; no decodable output

**Practical implication:**  
Most VHF/UHF ham activity is FM or SSB, neither of which a passive crystal radio can demodulate.  The regenerative design (Chapter 3) partially compensates: operating near self-oscillation, it becomes a self-heterodyne receiver and can resolve CW and even SSB.  For bands above 6M, regeneration is essentially required for useful reception.

**Where AM does exist on ham bands:**
- 80M: 3.870–3.900 MHz (traditional AM window, US)
- 40M: 7.290 MHz region (international AM activity)
- 20M: 14.286 MHz (international AM window)
- 10M: 29.000–29.200 MHz (significant AM activity; 10M is a great AM band)
- 6M: 50.400 MHz region (6M AM is active)
- VHF/UHF ATV: picture carriers at 439.25 MHz (70cm), 1253 MHz (23cm)

---

### 11.2 HF Ham Bands (80M through 10M) — Air-Core Coil Designs

Above 2 MHz, ferrite rod losses increase substantially.  All HF ham band crystal radios use **air-core coils** wound on cylindrical forms (PVC pipe, 3D-printed tube, or polystyrene).

**General design rules for HF ham bands:**
- Air-core coil on 25–50 mm diameter form
- Single-layer, close-wound #22–#26 AWG silver-plated or enameled wire
- Variable capacitor: 15–100 pF range (smaller than AM broadcast)
- Detector diode: 1N34A acceptable; BAT43 or 1N5711 Schottky preferred above 15 MHz
- RF bypass C2: reduce to 470–1000 pF (1 nF) for HF (0.01 µF is too large above 7 MHz)
- Antenna: resonant dipole or long-wire preferred; ferrite rod not useful above 5 MHz

**Schematic (same for all HF ham bands; only L and C values change):**
```
ANT ─── [link winding 2–4t] ─── (coupled to) ─── L1 (see band table)
                                                     │
                                                  [C1: see band table]
                                                     │
                                                    GND
         L1 hot end ─── [D1: BAT43] ─── AF out
                                           │
                                       [C2: 1 nF]  [earphone]
                                           │
                                          GND
```

**HF Ham Band Component Table:**

| Band | Freq range | L (µH) | Turns | Form dia | C range | C center | Q typical |
|------|-----------|--------|-------|---------|---------|---------|----------|
| 80M | 3.5–4.0 MHz | 22 | 25t | 50 mm | 35–80 pF | 50 pF | 150–250 |
| 40M | 7.0–7.3 MHz | 8 | 15t | 45 mm | 30–65 pF | 45 pF | 200–300 |
| 30M | 10.1–10.15 MHz | 4 | 11t | 40 mm | 25–58 pF | 55 pF | 200–300 |
| 20M | 14.0–14.35 MHz | 2.2 | 8t | 35 mm | 22–50 pF | 35 pF | 250–400 |
| 17M | 18.068–18.168 MHz | 1.3 | 6t | 30 mm | 18–42 pF | 29 pF | 250–400 |
| 15M | 21.0–21.45 MHz | 0.9 | 5t | 28 mm | 15–35 pF | 24 pF | 250–400 |
| 12M | 24.89–24.99 MHz | 0.6 | 4t | 25 mm | 12–28 pF | 20 pF | 250–400 |
| 10M | 28.0–29.7 MHz | 0.45 | 4t | 22 mm | 10–22 pF | 14 pF | 250–400 |

*Turns wound on PVC pipe or 3D-printed air-core form; #22 AWG silver-plated wire; single layer.*  
*All C values NP0/C0G; use air-variable trimmer or polyvaricon for C1.*

**Coil winding notes (HF ham bands):**
- Use #22 AWG silver-plated wire for highest Q above 7 MHz
- Silver plating reduces skin-effect resistance at HF; improves Q by 20–30%
- Space-wind (1 wire diameter gap between turns) above 14 MHz for reduced distributed capacitance
- Coil form diameter affects inductance: A_L ≈ 0.004 × d² / l (µH; d and l in mm, single layer)
- Self-supporting coils (no form) are possible for 20M and above; wind on a drill bit, slip off

**Capacitor selection for HF bands:**
- 365 pF broadcast variable has too much range (mostly wasted); use polyvaricon or trimmer
- Polyvaricon 5–60 pF (Bourns, Murata): compact, dual-gang available, low Q loss
- Air-variable 7–45 pF (broadcast trimmer): highest Q; mount on PTFE standoffs
- For compact portable use: 3–30 pF compression trimmer per band, switched

**Sensitivity expectations (HF ham bands, 10 m dipole, regenerative):**

| Band | Passive detection | Regenerative (1.5V) | Notes |
|------|-----------------|---------------------|-------|
| 80M | 1–5 mV/m | 0.05–0.2 mV/m | AM window active evenings |
| 40M | 1–3 mV/m | 0.05–0.1 mV/m | AM zone ~7.290 MHz |
| 20M | 2–8 mV/m | 0.1–0.5 mV/m | 14.286 MHz AM window |
| 10M | 0.5–2 mV/m | 0.02–0.1 mV/m | Best band for AM activity |

**10M is the recommended starting point for HF ham crystal radio construction.**  
Signal levels are often very high (amateur stations run 100–1500W; propagation can be excellent).  AM activity near 29.000–29.200 MHz is consistent.  The smaller coil is easier to construct precisely.

---

### 11.3 6-Meter Band (50–54 MHz)

**Band characteristics:**  
6M is the "magic band" — propagation can suddenly open to intercontinental range.  AM activity around 50.400 MHz is traditional.  FM simplex (52.525 MHz) is not receivable.

**Circuit considerations:**  
At 50 MHz, lead length becomes critical.  Keep all leads < 10 mm.  Stray capacitance of 1–2 pF changes resonant frequency significantly.  Use a small PCB or Manhattan-style construction.

**Component values:**

| Parameter | Value | Notes |
|-----------|-------|-------|
| L1 | 0.17 µH | 3 turns #18 AWG, 20 mm dia, 6 mm long, self-supporting |
| C1 | 3–20 pF | Air-variable trimmer or NPO disc; 1–2 pF stray budget |
| C2 (RF bypass) | 100–470 pF | 100 pF NP0; larger reduces audio HF response |
| D1 | BAT43 or 1N5711 | Schottky preferred; solder SMD (SOD-123) for short leads |
| Antenna coupling | 1 turn link | 1 turn of wire threaded through L1; coax to dipole |

**Coil construction:**  
Wind 3 turns of #18 AWG bare silver-plated wire on a 20 mm mandrel (3/4" dowel).  Remove mandrel; coil is self-supporting.  Spread or compress turns to adjust inductance.

```
Resonant frequency check:
  f = 1/(2π√LC)
  L = 0.17 µH, C = 10 pF: f = 1/(2π × √(0.17e-6 × 10e-12)) = 122 MHz (too high)
  L = 0.17 µH, C = 60 pF: f = 50 MHz ✓
  
  Stray capacitance of PCB + detector will be 5–15 pF → add only 45–55 pF variable.
```

**Sensitivity:**  
6M passive crystal radio: detectable signal ≥ 5–20 mV/m with resonant dipole.  
With regeneration: ≥ 0.2–1 mV/m; CW and SSB become intelligible.

---

### 11.4 2-Meter Band (144–148 MHz)

**Band characteristics:**  
2M is predominantly FM.  SSB and CW activity exists near 144.000–144.300 MHz (weak signal/EME segment).  AM is uncommon.  A passive crystal radio can detect AM beacons and ATV carriers; regenerative design can resolve SSB and CW.

**Construction approach changes significantly at 2M:**
- No wound wire coils; use **coax stub resonator** or **hairpin line**
- No binding posts; use direct coax pigtail to antenna
- Diode mounted directly at the resonator hot end; zero lead length
- Use SMA or BNC connectors throughout
- Build on a ground-plane PCB or copper-clad board

**Coax stub resonator (λ/4 at 146 MHz):**
```
  λ/4 at 146 MHz = 300/(146 × 4) × velocity_factor
  For RG-58 (VF = 0.66): λ/4 = 300/(584) × 0.66 = 0.338 m = 338 mm
  For RG-316 (VF = 0.695): λ/4 = 355 mm

  Short-circuit stub (λ/4): open-circuit at top (high impedance at resonance).
  Variable tuning: slide a shorting ring along the stub ±10 mm for ±5 MHz range.
```

**Hairpin resonator (preferred for compact construction):**
```
  Hairpin: two parallel conductors, 5 mm spacing, shorted at one end
  Length for 146 MHz: 490 mm total (245 mm each leg), bent in U-shape
  Use 1.6 mm dia silver-plated wire or #14 AWG
  Variable capacitor: 2–10 pF air trimmer across the open end
  Detector diode: mount directly across open end of hairpin
```

**Schematic (hairpin resonator):**
```
  Dipole ant (50Ω) ──[coax]── input loop (1 turn, 10mm dia, at shorted end)
                                    │
                    ╔══════════════════════╗
                    ║   HAIRPIN RESONATOR   ║
                    ║  245mm × 5mm spacing  ║
                    ╚══════════════════════╝
                                    │
                        [C_tune: 2–10 pF trimmer] to GND
                                    │
                        [D1: HSMS-2850] ──── AF out
                                               │
                                           [C2: 100pF]  [earphone]
                                               │
                                              GND
```

**Component values (2M):**

| Parameter | Value | Notes |
|-----------|-------|-------|
| Resonator | Hairpin, 245 mm legs | #14 AWG silver-plated; 5 mm spacing |
| C1 (tuning) | 2–10 pF air trimmer | Johanson 9301 or equivalent |
| C2 (RF bypass) | 100 pF NP0 | SMD 0402; lead length critical |
| D1 | HSMS-2850 (SOT-23) | Avago/Broadcom Schottky; V_f = 0.25 V |
| Antenna coupling | 1-turn loop at shorted end | Loosely coupled; 10–20 mm dia |
| Connector | SMA or BNC | No binding posts |

**Diode choice at 2M:**  
The 1N34A point-contact germanium diode has junction capacitance (C_j) of 0.5–1 pF and cutoff frequency ~500 MHz.  It works at 144 MHz but is marginal.  HSMS-2850 Schottky has C_j < 0.1 pF, f_c > 6 GHz — far better for VHF/UHF.

---

### 11.5 1.25-Meter Band (222–225 MHz)

222 MHz is lightly used but active in some regions for weak-signal SSB/CW.  Construction follows the 2M approach with scaled dimensions.

**Hairpin resonator for 222 MHz:**

| Parameter | Value |
|-----------|-------|
| Leg length | 160 mm each |
| Conductor spacing | 5 mm |
| Wire | #16 AWG silver-plated |
| Tuning cap C1 | 1–6 pF trimmer |
| Detector D1 | HSMS-2850 or BAT15-03W |

---

### 11.6 70-Centimeter Band (420–450 MHz)

**Band characteristics:**  
70cm carries FM repeaters (not detectable), SSB/CW weak signal (432.100 MHz calling), and **Amateur Television (ATV)**.  ATV fast-scan video uses AM with a picture carrier — directly detectable.

**ATV picture carrier (US):**
- 439.25 MHz: common ATV output frequency (picture carrier)
- Detected signal: video envelope (horizontal and vertical sync; image)
- Audio subcarrier: 4.5 MHz above picture carrier; not directly audible from envelope detection

**Construction: interdigital cavity or microstrip resonator**

For 70cm, hairpin lines become impractically short.  Use an **interdigital bandpass filter cavity** as both resonator and input matching, or build a **microstrip λ/4 resonator** on FR-4 PCB.

**Microstrip λ/4 resonator on FR-4 (ε_r = 4.4, h = 1.6 mm):**
```
  λ/4 at 435 MHz in FR-4 medium:
  λ_guide = λ_free / √ε_eff  where ε_eff ≈ (ε_r + 1)/2 = 2.7 for 50Ω trace
  λ_guide = (300/435) × (1/√2.7) = 689 mm × 0.608 = 419 mm
  λ/4 = 105 mm (50Ω microstrip, 2.9 mm wide on 1.6 mm FR-4)

  End-shorted λ/4 stub: open at hot end (high Z); detector diode mounted at hot end.
  Input tap: 1/4 to 1/3 from shorted end for ~50Ω match.
```

**Schematic:**
```
  Yagi/dish ──[SMA]── input tap (λ/4 from short, ~26 mm from GND end)
                            │
              [λ/4 microstrip resonator, 50Ω, 105 mm]
                            │ (hot end)
              [D1: HSMS-2850 SOT-23] ──── AF out
              [C2: 47 pF NP0 0402]  ────  GND
```

**Component values (70cm):**

| Parameter | Value | Notes |
|-----------|-------|-------|
| Resonator | λ/4 microstrip, 105 mm | 50Ω trace on FR-4; or coax stub |
| Tuning | Stub length ± 5 mm | Or 1–3 pF SMD trimmer at open end |
| D1 | HSMS-2850 (SOT-23) | Mount directly on microstrip; zero leads |
| C2 | 47 pF NP0 | SMD 0402 directly at diode pads |
| Antenna | Yagi or dish | 9–14 dBd gain recommended |
| Connector | SMA | Board-edge or pigtail |

**ATV reception note:**  
To receive ATV video, connect video output (AF output of crystal detector) to composite video input of a monitor.  The detected signal will show sync bars and image if a strong ATV station is transmitting.  Audio requires a separate 4.5 MHz discriminator.

---

### 11.7 33-Centimeter Band (902–928 MHz)

The 33cm band overlaps with the 902–928 MHz ISM band.  Amateur activity includes ATV, weak-signal SSB/CW, and experimental digital links.

**At 915 MHz, microstrip dimensions:**
```
  λ/4 at 915 MHz in FR-4:
  ε_eff ≈ 2.7 for 50Ω trace
  λ/4 = (300/915) × (1/√2.7) / 4 = 49.7 mm ≈ 50 mm
```

| Parameter | Value | Notes |
|-----------|-------|-------|
| Resonator | λ/4 microstrip, 50 mm | 50Ω trace, 2.9 mm wide on 1.6 mm FR-4 |
| D1 | HSMS-2850 or HSMS-285C | Best: beam-lead or SOT-23 at this frequency |
| C2 | 22 pF NP0 0402 | Directly at diode; lead length = 0 |
| Antenna | Dish, horn, or patch | 15+ dBd gain strongly recommended |
| Connector | SMA or 3.5mm coax | Keep all coax runs < 150 mm |

**Construction notes:**  
Above 900 MHz, PCB parasitics dominate.  Use Rogers RO4003C or RO4350B for better results (lower loss tangent than FR-4).  FR-4 is usable for experimental work.  All component leads must be SMD 0402 or smaller, soldered directly to the PCB without standoffs.

---

### 11.8 20-Centimeter Band (1240–1300 MHz)

The 20cm band (L-band) is the most challenging for crystal radio construction.  At 1.3 GHz, even 1 mm of wire is a significant fraction of a wavelength.

**Band characteristics:**  
ATV activity near 1253–1270 MHz.  Weak-signal SSB/CW at 1296.100 MHz.  Microwave beacon activity.

**λ/4 resonator at 1296 MHz:**
```
  λ/4 at 1296 MHz in FR-4 (ε_eff = 2.7):
  λ/4 = (300/1296) × (1/√2.7) / 4 = 35.2 mm

  For Rogers RO4003C (ε_eff ≈ 2.55):
  λ/4 = (300/1296) × (1/√2.55) / 4 = 36.2 mm
```

**Recommended diodes for 1.3 GHz:**

| Diode | C_j (pF) | f_c (GHz) | V_f (V) | Package | Notes |
|-------|---------|----------|--------|---------|-------|
| HSMS-2850 | 0.10 | 6.0 | 0.25 | SOT-23 | Best all-round; use at all UHF/µwave bands |
| HSMS-285C | 0.10 | 6.0 | 0.25 | SC-79 | Smaller package; better for 1 GHz+ |
| HSMS-2860 | 0.18 | 4.5 | 0.25 | SOT-23 | High sensitivity; lower f_c |
| 1PS79SB series | 0.10 | 10+ | 0.20 | SOD-523 | Very small; excellent at microwave |
| BAT15-03W | 0.05 | 15+ | 0.22 | SOD-323 | Premium; best for 23cm and above |

**Circuit approach:**  
The crystal detector at 1.3 GHz is best implemented as a matched detector using a λ/4 probe in a coax cavity, or as a microstrip circuit on low-loss substrate.

```
  COAX PROBE DETECTOR (1296 MHz):
  
  N-connector body ── outer conductor (GND)
                          │
  Center pin of N-conn ── λ/4 probe (inner conductor extension, 36 mm)
                                         │
                          HSMS-2850 diode (SOT-23, direct solder to probe tip)
                                         │
                          [C2: 22 pF NP0, 0402] to GND
                                         │
                          Coax pigtail ── AF output to earphone
```

**Antenna for 23cm:**  
A crystal radio at 1.3 GHz needs a high-gain antenna to collect adequate signal.  Minimum recommended: 17-element Yagi or 30 cm offset dish.  ATV picture carriers are typically 5–50 W ERP; at 1–5 km distance, power density may be sufficient.

---

### 11.9 Unified Signal Path Diagram (All Bands)

```
  BAND        RESONATOR TYPE         DETECTOR        ANTENNA
  ────────────────────────────────────────────────────────────
  AM bcst     Ferrite rod (220 µH)   1N34A           Long wire / loopstick
  160M        Air-core (78 µH)       1N34A           Long wire
  80M         Air-core (22 µH)       1N34A / BAT43   Dipole / long wire
  40M         Air-core (8 µH)        1N34A / BAT43   Dipole
  30–15M      Air-core (0.9–4 µH)    BAT43           Dipole
  12–10M      Air-core (0.45–0.6µH)  BAT43/1N5711    Dipole / Yagi
  6M          3-turn air coil        BAT43/1N5711    Dipole / Yagi
  2M          Hairpin resonator      HSMS-2850       Yagi (9 dBd min)
  1.25M       Hairpin resonator      HSMS-2850       Yagi
  70cm        Microstrip λ/4         HSMS-2850       Yagi (12 dBd min)
  33cm        Microstrip λ/4         HSMS-2850       Patch / Yagi
  20cm        Coax probe / microstrip HSMS-285C      Dish (20 dBd min)
```

### 11.10 Regenerative Requirement Summary

| Band | Passive crystal OK? | Regen needed for CW/SSB? | Primary detectable mode |
|------|--------------------|--------------------------|-----------------------|
| 80M | Yes | Helpful | AM phone (3.870–3.900 MHz) |
| 40M | Yes | Helpful | AM phone (~7.290 MHz) |
| 30M | Marginal | Yes | CW/data (no AM allocations) |
| 20M | Yes | Helpful | AM (14.286 MHz); SSB needs regen |
| 17M | Marginal | Yes | SSB/CW only |
| 15M | Marginal | Helpful | AM activity exists |
| 12M | Marginal | Yes | SSB/CW only |
| 10M | Yes | Not required for AM | AM (29.0–29.2 MHz); FM above 29.5 not receivable |
| 6M | Marginal | Yes | AM (~50.4 MHz); FM not receivable |
| 2M | Poor | Required | SSB/CW (144–144.3 MHz); FM not receivable |
| 1.25M | Poor | Required | SSB/CW; rare AM |
| 70cm | Poor | Required | ATV carrier (AM-detectable) |
| 33cm | Poor | Required | ATV; beacons |
| 20cm | Poor | Required | ATV; beacons; experimental |

---

<a name="appendix-a"></a>
## APPENDIX A — COMPONENT REFERENCE

### A.1 Detector Diodes

| Part | V_f (approx) | Package | Notes |
|------|-------------|---------|-------|
| 1N34A | 0.15–0.25 V | DO-7 glass | Standard; widely available |
| OA79 | 0.10–0.20 V | DO-7 glass | Excellent; European origin |
| OA47 | 0.10–0.18 V | DO-7 glass | Very sensitive; best for DX |
| OA91 | 0.15–0.22 V | DO-7 glass | Good general purpose |
| AAZ15 | 0.12–0.20 V | DO-7 glass | Equivalent to OA series |
| 1N270 | 0.15–0.25 V | DO-7 glass | Good alternative to 1N34A |
| BAT43 | 0.20–0.30 V | DO-35 | Schottky; good but slightly less sensitive |
| 1N5711 | 0.15–0.25 V | DO-35 | Schottky; excellent for LF |

### A.2 Variable Capacitors

| Type | Range (pF) | Notes |
|------|-----------|-------|
| Single-gang broadcast | 15–365 | Classic crystal radio; direct drive |
| Dual-gang broadcast | 2×15–365 | Double-tuned; same shaft |
| Triple-gang broadcast | 3×15–365 | TRF; rare; surplus from vintage radios |
| Polyvaricon (dual) | 2×5–60 | Compact; lower Q than air-var; for portable use |
| Trimmer (ceramic) | 5–30 | For tracking adjustment and fine-tuning |

### A.3 Ferrite Materials

| Mix | Manufacturer | µᵢ | Best use | Part numbers |
|-----|-------------|-----|---------|-------------|
| Mix 61 | Fair-Rite | 125 | HF/AM; low loss | 7" rod: Fair-Rite 4261377001 |
| Mix 77 | Fair-Rite | 2000 | AM broadcast; high sensitivity | 7" rod: Fair-Rite 4277377001 |
| Mix 73 | Fair-Rite | 2500 | AM/LW; max sensitivity | Specialty; less common |
| Mix T35 | Amidon | similar | Mix 77 equivalent | T35 rod stock |

---

<a name="appendix-b"></a>
## APPENDIX B — FREQUENCY AND COMPONENT TABLES

### B.1 Tuning Capacitor Required for 220 µH Coil

| Frequency (kHz) | Capacitance (pF) | Station examples |
|-----------------|-----------------|-----------------|
| 530 | 410 pF | AM low end |
| 600 | 320 pF | — |
| 700 | 235 pF | — |
| 800 | 180 pF | — |
| 900 | 142 pF | — |
| 1000 | 115 pF | Common reference frequency |
| 1100 | 95 pF | — |
| 1200 | 80 pF | — |
| 1300 | 68 pF | — |
| 1400 | 59 pF | — |
| 1500 | 51 pF | — |
| 1600 | 45 pF | — |
| 1700 | 40 pF | AM high end |
| 1800 | 35 pF | 160M low |
| 2000 | 29 pF | 160M high |

*Formula: C = 1/(4π²f²L); f in Hz, L = 220×10⁻⁶ H*

### B.2 Q and Bandwidth Table (L=220µH, various Q values)

| Q | −3dB BW at 1 MHz | −3dB BW at 600 kHz | −3dB BW at 1600 kHz |
|---|-----------------|-------------------|-------------------|
| 50 | 20 kHz | 12 kHz | 32 kHz |
| 80 | 12.5 kHz | 7.5 kHz | 20 kHz |
| 100 | 10 kHz | 6 kHz | 16 kHz |
| 150 | 6.7 kHz | 4 kHz | 10.7 kHz |
| 200 | 5 kHz | 3 kHz | 8 kHz |

---

<a name="appendix-c"></a>
## APPENDIX C — COIL WINDING REFERENCE

### C.1 Wire Sizes

| AWG | Diameter (mm) | Resistance (Ω/m) | Turns/cm (close wound) |
|-----|-------------|-----------------|----------------------|
| 24 | 0.511 | 0.084 | 18 |
| 26 | 0.405 | 0.134 | 23 |
| 28 | 0.321 | 0.213 | 29 |
| 30 | 0.255 | 0.339 | 37 |
| 32 | 0.202 | 0.538 | 46 |
| 36 | 0.127 | 1.36 | 74 |
| 38 | 0.101 | 2.16 | 93 |

Turns per cm = 10 / (wire diameter in mm × 1.05)  
(includes 5% enamel thickness factor)

### C.2 Ferrite Rod Inductance (9.5 mm rod, mix-77, 50 mm winding)

| Turns | L (µH) approx |
|-------|--------------|
| 50 | 50 |
| 80 | 130 |
| 100 | 185 |
| 110 | 220 |
| 120 | 260 |
| 150 | 390 |

Measured inductance takes priority over calculated.  Adjust turn count to achieve target L.

### C.3 Audio Transformer Winding (T-94A-77, A_L = 900 nH/N²)

| Turns | L at A_L=900nH/N² |
|-------|------------------|
| 100 | 9 H |
| 200 | 36 H |
| 300 | 81 H |
| 500 | 225 H |
| 700 | 441 H |

For secondary (10 turns): L_s = 900 nH × 100 = 90 µH (low; negligible effect on audio)

---

<a name="appendix-d"></a>
## APPENDIX D — PERFORMANCE DATA SHEETS

### D.1 Classic Crystal Radio — Design Record

```
Date built: _______________   Builder: _______________

COIL L1:
  Core mix: _______   Rod length: _______   Rod diameter: _______
  Turns: _______   Wire AWG: _______   Winding length: _______
  Measured inductance: _______ µH   (target: 220 µH)
  Q measured at 1 MHz: _______   (target: > 80)

CAPACITOR C1:
  Type: _______   Measured range: _______ pF to _______ pF

DIODE D1:
  Part number: _______   V_f measured: _______ V

DETECTOR C2:
  Value: _______ µF/nF/pF

ANTENNA:
  Type: _______   Length: _______ m
  Ground: _______

PERFORMANCE:
  Lowest station tuned: _______ kHz
  Highest station tuned: _______ kHz
  −3 dB bandwidth at 1 MHz: _______ kHz
  Minimum detectable E-field: _______ mV/m
  Number of stations receivable: _______
  Strongest station: _______ kHz / _______ kW / _______ km distance
```

### D.2 Regenerative Crystal Radio — Design Record

```
Date built: _______________

TRANSISTOR Q1: _______   h_FE measured: _______
R_b: _______ kΩ   R_c: _______ Ω   VR1 range: _______ kΩ
Tickler L2 turns: _______   Position: _______

PERFORMANCE WITH REGEN:
  VR1 at oscillation threshold: _______ (dial position)
  Operating point VR1: _______
  −3 dB bandwidth (operating point): _______ kHz
  Minimum detectable E-field: _______ mV/m
  Battery current at operating point: _______ mA
```

---

*End of TM-CR-001 Rev A*

*For questions or corrections, file an issue at the project repository or contact the documentation project maintainer.*
