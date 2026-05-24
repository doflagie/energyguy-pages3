# Dipole Antenna Design Reference
### For Amateur Radio Builders — Theory, Variants, Troubleshooting & Linux Tools

---

## Table of Contents

1. [Fundamentals](#1-fundamentals)
2. [Standard Half-Wave Dipole](#2-standard-half-wave-dipole)
3. [Variants](#3-variants)
4. [When to Choose Which Variant](#4-when-to-choose-which-variant)
5. [Feed Systems and Impedance Matching](#5-feed-systems-and-impedance-matching)
6. [Practical Construction](#6-practical-construction)
7. [Measurement with NanoVNA and TinySA](#7-measurement-with-nanovna-and-tinySA)
8. [Linux Software](#8-linux-software)
9. [Troubleshooting](#9-troubleshooting)
10. [Quick Reference Tables](#10-quick-reference-tables)

---

## 1. Fundamentals

### 1.1 What is a Dipole?

A dipole is the simplest resonant antenna: two equal-length conductive elements fed at the center. At resonance the antenna presents a largely resistive impedance to the feed line, making power transfer efficient. Every other antenna can be understood as a modification of this baseline.

**Why resonance matters:** A resonant antenna has zero (or near-zero) reactance at its feed point. Non-resonant antennas have a reactive component that reflects power back toward the transmitter, reducing efficiency and potentially damaging the final amplifier. The SWR (Standing Wave Ratio) on the feed line is the practical indicator of this mismatch.

### 1.2 Electromagnetic Basics

Current flows from the feed point outward along both elements, reverses direction at the tips, and sets up a standing wave. At resonance:

- **Current maximum** at the center (feed point)
- **Voltage maximum** at the tips

This is why the feed point impedance is relatively low (~73 Ω in free space) and the tip voltage is high — the tips must be well insulated.

The antenna radiates because accelerating charges emit electromagnetic radiation. Maximum radiation is broadside (perpendicular) to the wire; minimum radiation is off the ends (figure-8 pattern in the horizontal plane).

### 1.3 Core Equations

#### Half-Wave Physical Length

The free-space half-wavelength:

```
λ/2 (metres)  = 150 / f(MHz)
λ/2 (feet)    = 492 / f(MHz)
```

In practice, wire has an **end effect** and a small **velocity factor** (≈ 0.95–0.97 for bare copper, lower for insulated wire), so the resonant length is shorter than the free-space formula predicts:

```
L (metres) = 143 / f(MHz)     ← practical bare-wire dipole
L (feet)   = 468 / f(MHz)     ← the classic "468 formula"
```

Each element (half the dipole) is therefore:

```
Element (metres) = 71.5 / f(MHz)
Element (feet)   = 234 / f(MHz)
```

**Why 468 and not 492?** The 5% shortening accounts for the capacitive end effect — the electric field at the wire tip couples to nearby objects and ground, effectively making the antenna behave electrically longer than it is physically. Starting 5% short of the free-space value gives a good first cut that you then trim to resonance.

#### Radiation Resistance

A half-wave dipole in free space has a radiation resistance of approximately **73 Ω**. This is the resistive part of the feed impedance. The rest of the impedance is loss resistance (wire, connections) and, at the resonant frequency, zero reactance.

The radiation resistance varies with height above ground:

| Height (λ)  | Approx. Rr (Ω) |
|-------------|----------------|
| 0.125 (λ/8) | ~25            |
| 0.25 (λ/4)  | ~35            |
| 0.5 (λ/2)   | ~65            |
| 1.0 (λ)     | ~80            |
| Free space  | ~73            |

This is why a dipole installed low (< λ/4) shows poor SWR on 50 Ω coax — the impedance has dropped well below 50 Ω.

#### Bandwidth

The 2:1 SWR bandwidth of a thin-wire dipole is roughly:

```
BW (%) ≈ 6 × (wire_diameter / element_length)
```

Thicker wire = broader bandwidth. For 40 m (7 MHz) with #14 AWG wire (2 mm), bandwidth at 2:1 SWR is typically 150–200 kHz. For 80 m with the same wire, it narrows to 80–100 kHz because the element is longer relative to wire diameter.

---

## 2. Standard Half-Wave Dipole

### 2.1 Design Process

1. Calculate element length: `L_each (m) = 71.5 / f(MHz)`
2. Add 5–10% for trimming: start long, cut to resonance
3. Install at least λ/4 above ground (λ/2 preferred)
4. Feed with 50 Ω coax through a 1:1 current balun

### 2.2 Example: 40 m (7.150 MHz)

```
Each element = 71.5 / 7.150 = 9.99 m  (≈ 10 m)
Total length  = 19.98 m                (≈ 20 m)
Start length  = 21 m total (add 5% trimming margin)
```

### 2.3 Feed Impedance in Practice

A horizontal dipole at λ/2 height presents ~65–73 Ω, giving a comfortable SWR on 50 Ω coax (≈ 1.4:1). At λ/4 height, impedance drops to ~35–40 Ω, giving SWR of ~1.4:1 but with a different mismatch character.

Most transceivers tolerate up to 3:1 SWR. For better efficiency, bring the antenna higher or use a 1:1.5 matching transformer.

### 2.4 Radiation Pattern

- **Horizontal dipole:** Broadside, figure-8. Favours directions perpendicular to the wire.
- **Vertical dipole:** Omnidirectional in the horizontal plane; useful for local/regional communication.

---

## 3. Variants

---

### 3.1 Inverted-V Dipole

**Configuration:** The center is elevated (on a mast) and both elements slope downward at an angle. The included angle between the elements is typically 90°–120°.

**Why it works:** The sloping elements reduce the required horizontal space while keeping the feed point high. The downward slope lowers the effective feed impedance slightly (toward 50 Ω) because ground proximity affects the two elements differently than a flat dipole.

**Feed impedance:** At 90° included angle, Rr ≈ 50–60 Ω — an excellent match to 50 Ω coax, often without a balun (though one is still recommended to suppress common mode currents).

**Radiation pattern:** Slightly compressed vertically compared to flat dipole; more omnidirectional than a flat dipole. Some low-angle radiation enhancement at the apex.

**Practical notes:**
- Minimum apex height: λ/4 (λ/2 strongly preferred)
- Angle of 90° optimises impedance match; 120° is easier to support and gives slightly more directional gain broadside
- Tips can be only 1–2 m above ground without significant performance penalty

**Best for:** Single mast or tower with limited horizontal run. Classic HF portable or field-day antenna.

---

### 3.2 Sloper (Slanted Dipole)

**Configuration:** One element goes vertically (or nearly so) downward from an elevated feed point; the other is horizontal or at a shallow angle. Alternatively, the entire dipole is tilted at 30°–60° from horizontal.

**Why it works:** The asymmetric orientation creates an asymmetric radiation pattern with some directional preference toward the low end of the sloper. There is also a vertical polarisation component, aiding NVIS (Near Vertical Incidence Skywave) and low-angle DX simultaneously.

**Feed impedance:** Highly variable (30–75 Ω) depending on angle and proximity to a tower. A tower acts as a counterpoise and significantly affects resonant length and impedance.

**Practical notes:**
- If attached to a tower, the tower should ideally be λ/4 tall at the operating frequency
- Multiple slopers on one mast point in different directions for selectable directivity
- Use 50 Ω coax; SWR varies — test with NanoVNA and trim accordingly
- The low end requires a solid anchor (tree, stake) but only one tall support needed

**Best for:** One tall support available (tower, tree). Wanting some directivity without a beam.

---

### 3.3 Folded Dipole

**Configuration:** A standard half-wave dipole but the tips are connected by a second parallel wire (or the same wire folded back) of the same length, with the second wire short-circuited at the ends.

**Why it works:** The folded structure transforms the feed impedance by a factor of n² where n is the number of conductors (2 for standard fold = 4× = ~292 Ω). This matches 300 Ω twin-lead or 450 Ω ladder line directly.

**Feed impedance:** ~292 Ω (4× the standard 73 Ω)

**Radiation pattern:** Identical to a standard dipole.

**Advantages:**
- High impedance is ideal for balanced transmission lines (ladder line), reducing feed line loss
- Wider bandwidth than a simple dipole (the parallel conductor acts as a Q-reducing element)
- Mechanically robust: the folded tip connections add structural strength

**Practical notes:**
- Spacing between conductors: 12–50 mm (smaller spacing → closer to 4× impedance)
- Use with an ATU (antenna tuner) and balanced line for multiband operation
- Connect the short at both ends before measuring

**Best for:** Ladder-line fed all-band stations with an ATU. When bandwidth matters (e.g., wide 80 m coverage).

---

### 3.4 Fan Dipole (Multi-Band)

**Configuration:** Two or more dipole pairs connected at the same feed point, each cut for a different band, fanned outward at slight angles (5°–15° separation).

**Why it works:** Each dipole pair resonates independently on its own band. When operating on 40 m, the 20 m elements present a high impedance (being near λ) and have minimal effect. On 20 m, the 40 m elements present a moderate impedance but can be trimmed to minimise interaction.

**Feed impedance:** Roughly 50–70 Ω on each band if trimmed correctly; interaction between elements can shift resonant frequency slightly.

**Practical notes:**
- Trim each band individually: adjust the highest-frequency pair first, then lower bands
- Keep the angle between pairs as small as practical (5°–10°) to minimise interaction
- A 1:1 balun at the feed is important to keep common-mode currents from coupling between pairs
- Physical support needed for the spread tips — can fan over an attic space well

**Best for:** Multiband operation without a tuner. Limited space where a single-band per wire arrangement is needed.

---

### 3.5 Trap Dipole

**Configuration:** Each element of the dipole contains LC traps — parallel resonant circuits — inserted at specific distances from the feed point.

**How traps work:** A trap resonates at the higher frequency (e.g., 21 MHz). At that frequency it presents a very high impedance, electrically isolating the outer portion of the element. The inner portion acts as the 21 MHz dipole. At lower frequencies (e.g., 14 MHz, 7 MHz), the trap presents a small inductive reactance that slightly loads the longer outer section, shortening the required physical length.

**Design steps:**
1. Choose trap frequency (highest desired band)
2. Trap coil: `L (μH) = 25330 / (f² × C(pF))`
3. Inner element length (to trap): standard 468 formula for highest band
4. Outer element: adjusted shorter than a pure λ/2 due to trap inductance — requires trimming

**Feed impedance:** ~50–70 Ω on each band; can vary with trap Q. Low trap Q (lossy traps) reduces efficiency, particularly on the lower bands.

**Practical notes:**
- Commercial traps (e.g., Unadilla) are weatherproofed and well characterised
- DIY traps: use silver-mica or NPO capacitors; avoid ceramic disc (thermally unstable)
- Seal trap coils with weatherproof lacquer or self-amalgamating tape
- Traps add weight; reinforce the wire-to-trap connection
- Efficiency is lower than a full-size dipole on the lowest band because the outer portion sees trap loss

**Best for:** Multiband operation where fan-dipole element spread is impractical. Compact installation where a single straight wire must cover multiple bands.

---

### 3.6 Off-Centre Fed Dipole (OCFD / Windom)

**Configuration:** The feed point is offset from centre — typically at the 1/3 point (33%) from one end. Total length is the classic ~40 m (for 80/40/20/10 coverage).

**Why it works:** The impedance at the 1/3 point is a harmonic match across multiple HF bands. On 80 m (fundamental), the impedance is ~100–200 Ω; on 40 m (2nd harmonic), 20 m (4th), and 10 m (8th), the impedance at this offset point fortuitously remains within a usable range after transformation.

**Feed impedance:** ~200–300 Ω at the feed point. A 4:1 balun transforms this to 50–75 Ω coax.

**Radiation pattern:** Asymmetric and changes with band. On 80 m, similar to dipole; on 20 m and above, complex multi-lobe patterns develop — some beneficial, some not.

**Practical notes:**
- A quality 4:1 current balun (voltage type will have common-mode problems) is essential
- The asymmetry means the counterpoise effects of the coax shield are more pronounced — use a choke or W2DU ferrite bead balun
- Actual impedance at the offset feed is installation-dependent; expect to adjust
- Works surprisingly well on 5 bands from a single wire

**Best for:** Attic or rooftop installations where one straight wire is needed for multiband HF. When a tuner is unavailable but multiband coverage is required.

---

### 3.7 Bazooka Dipole (Coax Sleeve / Coax Dipole)

**Configuration:** The outer braid of the coaxial feed line is used as one element; the inner conductor extends as the other element. A sleeve of coax (braid of a second section) is folded back over the feedline coax for approximately λ/4, forming a choke integral to the antenna.

**Why it works:** The quarter-wave sleeve presents a high impedance to RF currents on the outside of the coax, acting as a built-in balun. Feed impedance is ~50–73 Ω.

**Practical notes:**
- Mechanically simple; no separate balun needed
- The sleeve is weatherproof if properly taped
- The coax must be rated for the power level throughout
- Gain and pattern are identical to a standard dipole

**Best for:** When a clean, self-contained antenna with integral balun is needed. VHF/UHF dipoles where coax dimensions are manageable.

---

### 3.8 Loaded / Shortened Dipole (Small Space)

**Configuration:** The physical element length is shorter than λ/2. The missing reactance is restored by adding inductance (loading coils) at the base, centre, or along the elements.

**Why it's necessary:** When space constrains total length to less than λ/2, the antenna is capacitively reactive at the target frequency. Loading coils add the inductive reactance needed to cancel this, bringing the antenna to resonance.

**Types of loading:**

| Type | Description | Efficiency |
|------|-------------|------------|
| Base loading | Coil at feed point | Lower (coil in high-current zone) |
| Centre loading | Coil partway along each element | Moderate |
| Linear loading | Folded-back wire sections along each element | Higher (distributed) |
| End loading (hat) | Capacity hat (radial spokes) at tips | Highest (no resistive coil loss) |

**Efficiency penalty:** A loaded antenna has lower radiation resistance. At 50% physical length, radiation resistance can drop to 10–20 Ω. The loading coil's Q determines how much power is wasted as heat. High-Q coils (silver-plated, large diameter, low winding density) minimise loss.

**Calculating a loading coil:**
```
Xs (Ω) = reactance needed to resonate the shortened element
       ≈ 1/(2π × f × Ctip)    (Ctip = capacitance of short element)

Approximate: Xs ≈ 168 × (1 - l/λ½) × Rr_full   [rule of thumb — model with XNEC2C]

L (μH) = Xs / (2π × f)
```

**Practical notes:**
- Always use a NanoVNA to find the actual resonant dip after construction; theory is a starting point
- Increase coil length (fewer turns per inch, larger diameter) to maximise Q
- Capacity hats are most efficient but require rigid spokes (aluminium rod)
- Even a 50%-length loaded dipole with good coils outperforms a poor takeoff angle dipole

**Best for:** Attic installations, balconies, stealth antennas, portable setups where space is the primary constraint.

---

### 3.9 Vertical Dipole

**Configuration:** A half-wave dipole oriented vertically, either freestanding or alongside a mast (offset feed).

**Why it works:** Vertical polarisation produces omnidirectional coverage in the horizontal plane with a low radiation angle — beneficial for DX. No ground radials are required (unlike a quarter-wave vertical), since the lower element acts as the counterpoise.

**Feed impedance:** ~73 Ω in free space; lower near ground due to coupling.

**Practical notes:**
- Needs supports only at top and bottom; feed at centre (middle of the antenna height)
- The lower element must be kept clear of ground by at least λ/8
- Common-mode currents on the coax shield are a significant problem — use a λ/4 choke or ferrite choke immediately at the feed point
- For 40 m a vertical dipole is ≈ 20 m tall — typically used at VHF/UHF or for shorter HF bands (15/10 m)

**Best for:** VHF/UHF omnidirectional coverage. HF DX on higher bands (15, 10 m) where height is achievable. Marine/portable omnidirectional HF.

---

### 3.10 G5RV Antenna

**Configuration:** A flat-top dipole, total length 31.1 m (102 ft), fed with a specific length of 300 Ω twin-lead (or 450 Ω ladder line) — 10.06 m (33 ft) — followed by 50 Ω coax to the transceiver.

**Why it works:** The matching section transforms the feed impedance of the flat-top at multiple HF bands to a range that the coax can handle (though an ATU is usually still needed). The G5RV was designed by Louis Varney (G5RV) as a multiband compromise.

**Performance:** Works reasonably on 20, 17, 15, 12, 10 m. Compromised on 40 m (high reactance); poor on 80 m without an ATU.

**Practical notes:**
- An ATU almost always needed for SWR below 2:1 on all bands
- Mini G5RV (half-size, 15.5 m) covers 10–17 m without ATU problems
- The specific matching stub length is critical — don't substitute random coax

**Best for:** All-band HF with an ATU when one simple wire of known length is needed.

---

## 4. When to Choose Which Variant

| Scenario | Recommended Variant | Reason |
|----------|--------------------|-------------------------------------------------|
| Single mast, open field | Inverted-V | One support, good impedance match, omnidirectional |
| Rooftop, long narrow space | Standard dipole or OCFD | Can run along roof ridge |
| Rooftop, limited horizontal | Sloper (off roof edge) | Uses vertical run, one attachment point |
| Attic, full band | Standard dipole (detuned by structure) | Trim to resonance inside; use NanoVNA |
| Attic, multiband | Fan dipole | No tuner needed; fits in attic spread |
| Attic, very short | Loaded dipole | Capacity-hat or coil-loaded; model first |
| Multiband, one wire, ATU available | G5RV or OCFD | Simple installation, tuner corrects mismatch |
| Multiband, no ATU | Trap dipole or fan dipole | Self-resonant on each band |
| DX, low-angle, no ground radials | Vertical dipole | Low radiation angle, omnidirectional |
| Wide bandwidth (contest, CW+SSB) | Folded dipole + ladder line + ATU | High-Q, wide coverage |
| Portable/field day | Inverted-V or sloper | Minimal hardware, quick to deploy |
| Stealth (HOA, apartment) | Loaded dipole or bazooka | Compact, coax visible only |

---

## 5. Feed Systems and Impedance Matching

### 5.1 Why Impedance Matching Matters

Maximum power transfer occurs when source impedance equals load impedance (conjugate match). A mismatch reflects power back toward the transmitter. The reflected power isn't lost — it bounces back and forth on the feed line — but feed line loss increases with SWR, and the mismatch stresses the final amplifier in solid-state transceivers, which often fold back power to protect themselves.

### 5.2 Coaxial Cable

- **RG-58**: 50 Ω, 5 mm, lossy at VHF. Use only for short runs (<10 m) on HF.
- **RG-8X / Mini-8**: 50 Ω, lower loss. Good general HF cable.
- **RG-213 / LMR-400**: 50 Ω, low loss, larger diameter. Preferred for long runs.
- **RG-6 (75 Ω)**: Cheap satellite TV cable. 75 Ω mismatch to most transceivers, but usable with matching.

Feed line loss approximately doubles for each 3:1 increase in SWR at a given frequency. At 2:1 SWR, extra loss is < 0.5 dB on most HF runs — acceptable.

### 5.3 Balanced Transmission Lines (Ladder Line)

| Type | Impedance | Velocity Factor | Loss |
|------|-----------|----------------|------|
| 300 Ω twin-lead | 300 Ω | 0.82 | Higher (dielectric) |
| 450 Ω window line | 450 Ω | 0.91 | Very low |
| 600 Ω open wire | 600 Ω | 0.97 | Lowest |

Ladder line has dramatically lower loss than coax at high SWR — a key advantage when using a tuner to match a non-resonant antenna. At 10:1 SWR, coax loss skyrockets; ladder line loss barely increases.

**Why balanced line must use a balanced tuner or balun:** If ladder line is connected directly to an unbalanced (coax) system without a balun, RF flows on the outside of the coax braid, causing RFI and erratic SWR readings.

### 5.4 Baluns

A **balun** (balanced-to-unbalanced) transformer connects a balanced antenna to an unbalanced feed line.

| Balun Type | Ratio | Best Use | Notes |
|------------|-------|----------|-------|
| 1:1 Current (choke) | 1:1 | Dipole to 50 Ω coax | Suppresses common-mode; use with resonant dipoles |
| 4:1 Voltage | 4:1 | Folded dipole, OCFD | Transforms 200 Ω → 50 Ω; can pass common-mode |
| 4:1 Current | 4:1 | OCFD, high-power | Better common-mode rejection than voltage type |
| 9:1 | 9:1 | End-fed, random wire | Not for dipoles |

**Always use a 1:1 current balun (choke balun) at the dipole feed point.** Even if impedance is well matched, without a balun the outer coax braid becomes part of the antenna, creating unpredictable patterns and RFI.

**DIY choke balun:** Wind 8–12 turns of RG-58 or RG-8X through a FT-240-43 toroid (for HF). This presents >1000 Ω choking impedance across most of the HF spectrum.

---

## 6. Practical Construction

### 6.1 Wire Selection

| Wire | Advantage | Disadvantage |
|------|-----------|--------------|
| #14 AWG hard-drawn copper | Strong, low loss | Heavier, less flexible |
| #14 AWG stranded copper | Flexible, easy to handle | Stretches under load |
| #22 AWG enamelled magnet wire | Invisible, light | Breaks if tensioned; stealth only |
| Copper-clad steel (CCS) | High strength, low sag | Slightly higher loss |
| Stainless steel wire | Marine/salt environment | Higher resistivity — avoid for HF |

For typical HF dipoles, **#14 or #16 AWG hard-drawn copper** is the standard. Stranded is fine for temporary or portable setups.

**Insulated vs. bare wire:** Insulated wire has a velocity factor of ~0.97–0.98 instead of 1.0. Use the 468 formula (which assumes bare wire) and expect to trim slightly less. Insulation also helps prevent oxidation and shorting if elements contact metal supports.

### 6.2 Insulators

- **Tip insulators:** Ceramic egg insulators for permanent installations; PVC end caps or nylon spacers for portable work
- **Center insulator:** Commercial SO-239 dipole center connector or DIY from polycarbonate/UHMW-PE. Avoid PVC (high dielectric loss at RF)
- **Strain relief:** The coax at the feed point must be supported mechanically — never let the coax weight pull on the center connector. A drip loop prevents water ingress

### 6.3 Support and Tension

- **Minimum support height:** λ/4 above ground — λ/2 preferred
- **Wire tension:** Sufficient to prevent sag but not enough to stretch the wire. Copper yields under tension; once stretched, it won't spring back
- **Rope selection:** Black polyester halyard (UV resistant). Avoid nylon (UV degrades quickly); avoid metal cable (detuning)
- **Tree mounting:** Trees sway. Use a pulley and counterweight to allow movement without breaking the wire or support rope

### 6.4 Weatherproofing

- **Coax connectors:** Self-amalgamating tape over all outdoor connectors. Never use standard PVC electrical tape outdoors — it unravels and traps moisture
- **Solder joints:** Use rosin-core (not acid-core) solder; seal with heat-shrink tubing
- **Coax end:** Terminate unused coax ends with a 50 Ω dummy load or at minimum a weather cap to prevent moisture ingress
- **Balun enclosures:** Use UV-resistant ABS or polycarbonate boxes rated for outdoor use

---

## 7. Measurement with NanoVNA and TinySA

### 7.1 NanoVNA Basics

The NanoVNA is a vector network analyser that measures S-parameters — S11 (return loss/SWR) and S21 (through loss/gain). For antenna work, S11 is the primary measurement.

**Before connecting an antenna:**
1. Perform a full 2-port SOLT (Short, Open, Load, Through) calibration at the end of the measurement cable
2. Calibration reference plane is at the calibration point — any cable between the NanoVNA and antenna port becomes part of the measurement unless accounted for
3. Calibrate at the operating temperature; parameters drift slightly with temperature

**Key displays:**
- **SWR plot:** Target < 2:1 across your operating range
- **Smith Chart:** The centre is 50 Ω. Resonance = real axis (zero reactance). Above centre = inductive; below = capacitive
- **Return Loss (dB):** > 10 dB ≈ SWR < 2:1; > 20 dB ≈ SWR < 1.2:1
- **Phase:** Crosses zero at resonance (no reactance). Negative phase = capacitive (antenna is too long); positive = inductive (too short)

**Trimming to resonance:**
1. Sweep a range ±15% either side of target frequency
2. Locate the SWR minimum
3. If minimum is below target: antenna is too long → shorten each element equally by 1–2 cm
4. If minimum is above target: too short → add wire or adjust environment (more height usually lowers resonant frequency slightly)

**Why the SWR minimum may not be at 50 Ω:** If the antenna's radiation resistance is not 50 Ω (e.g., low dipole with Rr ≈ 35 Ω), the SWR minimum will be flat but above 1:1. The Smith chart will show the resonant point to the left of centre (resistance < 50 Ω). This is normal — the antenna is resonant but mismatched. A 1:1.4 transformer or moving the antenna higher fixes this.

### 7.2 NanoVNA Measurement Procedure

1. Calibrate with short coax extension (the actual cable you'll use as feed line)
2. Connect antenna
3. Set start/stop frequency around expected resonant band ± 20%
4. Observe Smith chart and SWR plot simultaneously
5. Note frequency of SWR minimum and its value
6. Note frequency where phase crosses zero (true resonance)
7. **Compare the two:** If SWR minimum and zero-phase are at the same frequency, the antenna is resonant and the mismatch is purely resistive. If they differ, there's a complex reactance situation — check for coax common-mode currents (add balun) or nearby metal objects

### 7.3 TinySA Usage

The TinySA is a spectrum analyser, not a VNA. It cannot directly measure SWR or impedance. Its role in antenna work:

- **Received noise floor check:** Hold the TinySA near the antenna feed point; sweep HF. A properly working antenna picks up band noise. A broken antenna or feed shows a flat noise floor.
- **Transmitter spurious output check:** Connect via an attenuator (minimum 30 dB) to the coax; key a low-power carrier; check for harmonics and spurious emissions. Useful after building or modifying an antenna or filter.
- **Interference hunting:** Walk around with TinySA + small whip to locate noise sources degrading received signal

**Never connect a transmitter directly to the TinySA without appropriate attenuation.** The input is limited to +10 dBm continuous (+20 dBm peak). At 5 W transmit power (+37 dBm), use at minimum 40 dB of attenuator — a 20 dB pad plus another 20 dB pad in series.

---

## 8. Linux Software

### 8.1 NanoVNA-Saver

**Purpose:** Full PC interface for NanoVNA. Displays SWR, Smith chart, return loss, TDR (Time Domain Reflectometry), cable loss, and more. Allows data export and calibration save/load.

**Installation:**
```bash
# Via pip (recommended)
pip3 install NanoVNASaver

# Or from source
git clone https://github.com/NanoVNA-Saver/nanovna-saver
cd nanovna-saver
pip3 install -r requirements.txt
python3 nanovna-saver.py
```

**Key features for antenna work:**
- **TDR (Time Domain Reflectometry):** Shows impedance discontinuities along the feed line — excellent for locating coax damage, bad connectors, or impedance steps
- **Marker analysis:** Place markers at specific frequencies; read R, X, |Z|, SWR, phase simultaneously
- **Band markers:** Overlay ham band boundaries on the sweep for visual reference
- **Calibration management:** Save and reload calibrations for different measurement setups

**Typical workflow:**
1. Connect NanoVNA via USB
2. Select port in NanoVNA-Saver
3. Load or perform calibration
4. Set sweep range (e.g., 3.5–30 MHz for full HF)
5. Sweep → examine Smith chart and SWR

### 8.2 TinySA Software (TinySA-App / tinySA-Ultra)

```bash
# TinySA Python app
pip3 install tinySA

# Or the Qt-based tinySA Ultra app
git clone https://github.com/erikkaashoek/tinySA
cd tinySA
pip3 install -r requirements.txt
python3 tinysa.py
```

**Capabilities:** Waterfall display, marker/delta marker, RBW control, average/max hold. Useful for monitoring band noise and identifying interference sources before and during antenna work.

### 8.3 XNEC2C — Antenna Modelling

**Purpose:** NEC2-based antenna modelling with a GTK graphical interface. Models antenna geometry, computes radiation patterns, impedance versus frequency, and gain. Essential for designing loaded dipoles, trap dipoles, and unusual configurations before building.

**Installation:**
```bash
sudo apt install xnec2c
```

**Workflow for a dipole:**
1. Define wire segments: two elements, each split into 5–11 segments
2. Place a voltage source (excitation) at the centre junction
3. Set frequency range
4. Compute → view 3D radiation pattern, SWR vs. frequency, impedance

**Why model first?** A loaded dipole's resonant length and efficiency are non-obvious. An XNEC2C model lets you iterate the coil inductance and placement in seconds rather than hours of physical trimming.

**XNEC2C input example** (simple 40 m dipole at 10 m height):
```
CM 40m Dipole
CM Height 10m over average ground
CE
GW 1 11 -9.98 0 10 0 0 10 0.001
GW 2 11 0 0 10 9.98 0 10 0.001
GE 1
GN 2 0 0 0 13 0.005
EX 0 1 6 0 1.0 0.0
FR 0 21 0 0 6.5 0.05
EN
```

### 8.4 Other Useful Linux Tools

| Tool | Purpose | Install |
|------|---------|---------|
| **WSPR-X** | WSPR beacon — tests antenna propagation objectively | `apt install wsjtx` |
| **WSJT-X** | FT8/FT4 — real-world antenna comparison | `apt install wsjtx` |
| **Gpredict** | Satellite pass prediction — for VHF/UHF dipoles | `apt install gpredict` |
| **Fldigi** | Digital modes; audio spectrum analyser tab useful for noise floor checking | `apt install fldigi` |
| **gnuradio** | SDR signal processing; combine with RTL-SDR for receive testing | `apt install gnuradio` |
| **cocoaNEC** (via Wine) | NEC2 GUI, alternative to XNEC2C | Wine + installer |

---

## 9. Troubleshooting

### 9.1 SWR Is High Across the Entire Band

**Symptom:** SWR > 3:1 everywhere; no clear minimum.

**Causes and fixes:**

| Cause | Diagnosis | Fix |
|-------|-----------|-----|
| Open circuit in antenna | NanoVNA Smith chart shows capacitive arc only; no resonance | Check all solder joints and connectors; test continuity |
| Short circuit at feed | Smith chart shows left side only (resistance near 0) | Check coax centre/braid short; check balun windings |
| Coax connector failure | Wiggle test — SWR jumps | Replace PL-259 connectors; re-solder; use N-type for permanent installations |
| Wrong frequency range | Minimum is outside sweep range | Widen sweep ± 50%; the antenna may be resonant elsewhere |
| Common-mode current on coax | SWR changes as you touch or move the coax | Install a 1:1 choke balun at the feed point |

### 9.2 Resonance Is Correct but SWR Won't Go Below 2:1

**Symptom:** Clear SWR minimum at target frequency, but minimum value is 2:1 or higher.

**Cause:** Antenna feed impedance is not 50 Ω. The antenna IS resonant (zero reactance) but the radiation resistance differs from 50 Ω.

**Diagnosis:** On Smith chart, the minimum falls to the left of centre (Rr < 50 Ω, antenna too low) or right of centre (Rr > 50 Ω, unusual geometry).

**Fixes:**
- Raise the antenna (increases Rr toward 50–73 Ω)
- Use a matching transformer (1.5:1 for dipole at λ/4 height)
- Inverted-V geometry naturally lowers Rr toward 50 Ω — more suitable for single-mast installation

### 9.3 Resonant Frequency Is Too Low (Antenna Too Long)

**Symptom:** SWR minimum is below target frequency.

**Why:** The antenna is electrically longer than λ/2 at the target frequency. This can be due to:
- Wire measured incorrectly (common — double-check with a steel tape, not a cloth measure)
- Insulated wire used with bare-wire formula
- Nearby metal objects (rain gutters, metal roof) coupling and electrically lengthening the antenna
- Low height above ground

**Fix:**
- Shorten each element by equal amounts; trim in small increments (1–2 cm at a time on VHF; 5–10 cm on HF)
- Rule of thumb: **1 cm of total shortening (0.5 cm each side) shifts resonance up by approximately:**
  ```
  Δf (MHz) ≈ 0.5 / total_length_metres
  ```
  On a 20 m long 40 m dipole: 1 cm moves resonance ≈ 25 kHz

### 9.4 Resonant Frequency Is Too High (Antenna Too Short)

**Symptom:** SWR minimum is above target frequency.

**Fix:**
- Add wire — clip or loop extra wire onto each tip temporarily to confirm direction of shift before permanently splicing
- Lower the antenna slightly (ground proximity lowers resonant frequency)
- If wire was cut too short, splice additional length using a high-quality solder joint and weatherproof carefully

### 9.5 SWR Changes When It Rains

**Cause:** Wet wire has different surface conductivity; wet insulators conduct. Both load the antenna and shift resonance.

**Fix:**
- Use genuine ceramic egg insulators, not PVC (PVC absorbs water)
- Ensure coax connectors are fully weatherproofed with self-amalgamating tape
- If coax is lying on wet ground near the antenna, lift or re-route it

### 9.6 RFI in Shack During Transmit (RF on Coax Shield)

**Symptom:** RF feedback into microphone, computer, or USB devices during transmit; SWR appears to change as you touch equipment.

**Cause:** Common-mode current on coax outer conductor. The coax outer braid is RF-hot — effectively part of the antenna. This happens when no balun is used, or a voltage-type 4:1 balun is used where a current balun is needed.

**Fix:**
1. Install a 1:1 current balun (choke balun) immediately at the dipole feed point
2. Add ferrite bead chokes (W2DU style) at the transceiver end of the coax
3. Route coax away from the antenna in a direction perpendicular to the antenna wire
4. Ensure shack station ground (bonded chassis ground, not RF ground)

### 9.7 Antenna Appears Resonant but Performance Is Poor

**Symptom:** SWR is good, but received signals are weak and transmitted signal reports are poor.

**Possible causes:**

| Cause | Diagnosis | Fix |
|-------|-----------|-----|
| Antenna too low | Height < λ/8 | Raise antenna; even 2–3 m improvement helps dramatically |
| High feed line loss | Measure feed line loss with NanoVNA TDR; expect < 1 dB/30 m at HF | Replace RG-58 with LMR-400 or similar |
| Lossy loading coils (loaded dipole) | Coil gets warm during transmit | Rewind with larger diameter, fewer turns per inch |
| Nearby metal absorbing/detuning | Move antenna away from metal roof, gutters | Even 0.5 m clearance improves matters |
| Local noise floor masking receive | Check with TinySA — is noise floor elevated? | Find and suppress noise sources; filtering |

### 9.8 TDR Shows Impedance Bump in Feed Line

**Symptom:** NanoVNA-Saver TDR shows a reflection somewhere along the feed line.

**Cause:** A damaged connector, a kink in the coax, water ingress, or an impedance transition.

**Diagnosis:** TDR displays reflections versus distance. Positive spike = impedance increase (open tendency); negative spike = impedance decrease (short tendency).
- At connector: replace connector
- Mid-cable: water has entered — cut and re-splice with waterproof joint

---

## 10. Quick Reference Tables

### Half-Wave Dipole Lengths by Band

| Band | Freq (MHz) | Each Element (m) | Total (m) | Each Element (ft) | Total (ft) |
|------|-----------|-----------------|-----------|------------------|------------|
| 160 m | 1.900 | 37.6 | 75.3 | 123.2 | 246.3 |
| 80 m | 3.750 | 19.1 | 38.1 | 62.4 | 124.8 |
| 40 m | 7.150 | 10.0 | 20.0 | 32.7 | 65.5 |
| 30 m | 10.125 | 7.06 | 14.1 | 23.1 | 46.2 |
| 20 m | 14.175 | 5.04 | 10.1 | 16.5 | 33.1 |
| 17 m | 18.118 | 3.95 | 7.89 | 12.9 | 25.9 |
| 15 m | 21.225 | 3.37 | 6.74 | 11.0 | 22.1 |
| 12 m | 24.940 | 2.87 | 5.73 | 9.39 | 18.8 |
| 10 m | 28.400 | 2.52 | 5.03 | 8.24 | 16.5 |
| 6 m | 51.000 | 1.40 | 2.80 | 4.59 | 9.18 |
| 2 m | 146.000 | 0.49 | 0.98 | 1.60 | 3.21 |
| 70 cm | 446.000 | 0.16 | 0.32 | 0.52 | 1.05 |

*Formula used: each element = 71.5 / f(MHz). Add 5% when initially cutting.*

### Variant Comparison Summary

| Variant | Bands | Feed Z | Balun | Space | Difficulty |
|---------|-------|--------|-------|-------|------------|
| Standard dipole | 1 | ~65 Ω | 1:1 | Long | Easy |
| Inverted-V | 1 | ~50 Ω | 1:1 | Medium | Easy |
| Sloper | 1 | 30–75 Ω | 1:1 | Small | Easy |
| Folded dipole | 1 | ~292 Ω | 4:1 or ladder | Long | Moderate |
| Fan dipole | 2–4 | ~50 Ω | 1:1 | Long + spread | Moderate |
| Trap dipole | 2–3 | ~50 Ω | 1:1 | Long | Moderate |
| OCFD | 4–5 | ~200 Ω | 4:1 | Long | Moderate |
| Loaded dipole | 1 | Low | 1:1 | Short | Hard |
| Bazooka | 1 | ~50 Ω | None (integral) | Long | Easy |
| G5RV | 5–6 + ATU | Variable | Balun + ATU | 31 m | Easy |
| Vertical dipole | 1 | ~73 Ω | 1:1 | Tall | Moderate |

---

*Document prepared for WRXN788 — rev. 2026-04*
