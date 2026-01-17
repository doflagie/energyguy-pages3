# Building Homebrew NanoVNA Calibration Standards

## Overview

For accurate impedance measurements, the NanoVNA needs three calibration standards:
1. **SHORT** - 0Ω (as close as possible)
2. **OPEN** - ∞Ω (nothing connected)
3. **LOAD** - exactly 50Ω, non-inductive

This guide shows how to **build or verify** these standards using **cheap, homebrew methods** without calibrated equipment.

---

## Understanding Calibration Standards

### What NanoVNA Needs to Know

During calibration, NanoVNA measures known standards and calculates error correction factors:

**SHORT (0Ω):**
- Reflection coefficient: Γ = -1
- All power reflected, inverted phase
- Smith chart: Far left

**OPEN (∞Ω):**
- Reflection coefficient: Γ = +1
- All power reflected, same phase
- Smith chart: Far right

**LOAD (50Ω):**
- Reflection coefficient: Γ = 0
- No reflection, all power absorbed
- Smith chart: Dead center

### Real-World Imperfections

**No standard is perfect:**

**SHORT:**
- Has tiny inductance (~0.1-1 nH)
- Matters above 1 GHz
- Good enough: <0.5 nH for <1 GHz

**OPEN:**
- Has stray capacitance (~0.05-0.2 pF)
- Connector adds ~0.05 pF
- PCB adds more
- Good enough: <0.1 pF total

**LOAD:**
- Resistors have inductance
- Lead length matters
- Tolerance: ±1% is good, ±5% acceptable
- Temperature coefficient
- Power rating

---

## Part 1: Evaluating Supplied Cal Kit

### What Comes with NanoVNA

Most NanoVNAs include basic cal kit:
- SHORT: SMA plug with center pin shorted to ground
- OPEN: SMA plug with nothing connected
- LOAD: SMA plug with 50Ω resistor inside

**Quality varies widely:** Some are excellent, some are terrible

### Visual Inspection

**SHORT:**
- Remove outer shell
- Inspect solder joint
- Should be low-inductance (wide, flat connection)
- No wire loop!

**Good SHORT:**
```
 Pin
  │
  └─┬─ Wide copper strap
    │
   GND
```

**Bad SHORT (has inductance):**
```
 Pin
  │
  └─┐  Wire loop = inductor!
    └─ GND
```

**LOAD:**
- Open carefully (non-destructively if possible)
- Identify resistor
- Read color codes or markings
- Check if surface mount or through-hole
- Note lead length

**Common LOAD types:**
1. Single 51Ω resistor (cheap, okay to 500 MHz)
2. Two 100Ω in parallel (better)
3. SMD resistors (best, lowest inductance)

---

## Part 2: Building DIY SHORT

### Method 1: SMA Connector Short (Best)

**Parts:**
- SMA male connector (panel mount or cable end)
- Copper tape or braid
- Solder, soldering iron

**Construction:**

1. **Strip SMA connector** to access center pin

2. **Create low-inductance connection:**
   ```
   Method A: Copper tape
   - Cut copper tape (1/4" wide, 1/2" long)
   - Solder one end to center pin
   - Wrap around and solder to outer shell

   Method B: Copper braid
   - Cut braid from coax shield (1/2" long)
   - Flatten with hammer
   - Solder center pin to shell via braid
   ```

3. **Keep it flat and wide** (minimizes inductance)

4. **Test:**
   - Should measure <0.1Ω with DMM
   - No loose connections

**Inductance:** ~0.1-0.3 nH (excellent to 2 GHz)

### Method 2: Modified SMA Cap

**Parts:**
- SMA female cap
- Copper wire or foil

**Construction:**

1. **Drill out center** of SMA cap
2. **Insert copper foil or wire** through hole
3. **Solder to shell** on both sides
4. **Keep connection short and wide**

**Easier but slightly higher inductance**

### Method 3: PCB SHORT

**Parts:**
- Small piece of PCB (FR4)
- SMA edge mount connector
- Copper cladding

**Construction:**

1. **Mount SMA to PCB edge**
2. **Leave copper** around center pin
3. **Solder center pin to copper pour**
4. **Cut PCB to size**

**Advantage:** Very repeatable, professional
**Disadvantage:** Needs PCB tools

### Verifying SHORT Quality

**Test 1: DC Resistance**
- Measure with DMM
- Should be <0.1Ω
- If >1Ω, connection is poor

**Test 2: NanoVNA Test**
- Connect SHORT to NanoVNA
- Sweep 1-900 MHz
- Format: Smith chart
- Should stay at left edge
- If it curls inward → too much inductance

**Test 3: Return Loss**
- Format: LOGMAG
- Should show >40 dB return loss
- If <30 dB → poor short

---

## Part 3: Building DIY OPEN

### The Problem with OPEN

**OPEN is tricky:**
- Connector itself has capacitance (~0.05 pF)
- Exposed center pin radiates
- PCB adds capacitance
- Air gap dielectric

**Good news:** Small capacitance doesn't matter much below 500 MHz

### Method 1: SMA Female Connector (Simplest)

**Parts:**
- SMA female panel mount connector
- Nothing else!

**Construction:**
1. Install SMA connector on enclosure or PCB
2. Leave center socket empty
3. That's it!

**Capacitance:** ~0.05-0.1 pF (connector only)
**Good for:** <1 GHz

### Method 2: Calibrated OPEN with Defined Capacitance

**For better accuracy at high frequency:**

1. **Build or buy OPEN**
2. **Measure capacitance:**
   - Use NanoVNA in capacitance mode
   - Or use LCR meter
   - Or calculate from geometry

3. **Enter in NanoVNA cal:**
   - Some firmwares allow OPEN capacitance specification
   - Improves accuracy >500 MHz

**Typical values:** 0.02-0.15 pF

### Method 3: Delayed OPEN

**For microwave frequencies (>1 GHz):**

**Concept:** Add known length of transmission line before OPEN

**Construction:**
```
NanoVNA → SMA cable (known length) → OPEN

Example:
50mm coax (VF=0.66) → OPEN

Electrical delay = length / (c × VF)
                 = 0.05 / (3e8 × 0.66)
                 = 253 ps
```

**Enter delay in NanoVNA cal parameters**

### Verifying OPEN Quality

**Test 1: Visual**
- Center pin should not touch anything
- No solder bridges
- Clean connector

**Test 2: DMM**
- Infinite resistance
- If <1 MΩ → contamination or leakage

**Test 3: NanoVNA**
- Smith chart: Should stay at right edge
- If curls inward → too much capacitance (still usable)

---

## Part 4: Building DIY 50Ω LOAD

This is the **most critical** and **most challenging** standard.

### Understanding the Challenge

**50Ω resistor must be:**
- Exactly 50Ω (±1% or better)
- Non-inductive (no spirals or long leads)
- Stable with temperature
- Capable of handling signal power
- Broadband (works DC to GHz)

**Problem:** Most resistors fail at RF!

### Method 1: Through-Hole Resistor (Simple, Good to 200 MHz)

**Parts:**
- 51Ω resistor, 1/4W, 1% or better metal film
- OR 100Ω resistors × 2 in parallel
- SMA connector
- Short leads

**Construction:**

1. **Choose resistor:**
   - 51Ω 1% metal film (best)
   - Or two 100Ω in parallel (= 50Ω)

2. **Measure actual value with DMM:**
   ```
   Example:
   Resistor marked 51Ω
   DMM reads: 50.8Ω ✓ Good enough
   ```

3. **Install in SMA connector:**
   ```
   SMA center pin → Resistor lead (trimmed SHORT)
                    ↓
                 Resistor body
                    ↓
   SMA shell ← Resistor lead (trimmed SHORT)
   ```

4. **Keep leads <5mm total**
   - Long leads = inductance
   - Trim resistor leads
   - Solder directly to connector

5. **Strain relief:**
   - Add epoxy or heat shrink
   - Don't stress resistor body

**Performance:**
- DC - 200 MHz: Excellent
- 200-500 MHz: Good
- >500 MHz: Inductance matters

**Lead inductance:**
```
L ≈ 1 nH per mm of lead length

5mm leads = 5 nH
Reactance at 500 MHz: XL = 2πfL = 2π × 500e6 × 5e-9 = 15.7Ω
Impedance: Z = √(50² + 15.7²) = 52.4Ω
Error: +2.4Ω (still acceptable)
```

### Method 2: SMD Resistor (Best, Good to 2 GHz)

**Parts:**
- 50.0Ω SMD resistor (0805 or 1206 size)
- OR two 100Ω in parallel
- SMA connector
- Small PCB or connector adapter

**Construction:**

1. **Choose resistor:**
   - 49.9Ω, 0805, 1% (ideal)
   - Or 50.0Ω ±1%
   - Or two 100Ω in parallel

2. **Mount to SMA:**
   ```
   Method A: Direct solder
   SMA pin → SMD pad → SMD resistor → SMD pad → SMA shell

   Method B: Small PCB
   [PCB]
     │
   [SMA connector]─[trace]─[SMD]─[trace]─[ground plane]
   ```

3. **Keep traces short and wide**
   - Minimize trace inductance
   - Wide traces reduce inductance

4. **Ground plane** on bottom of PCB

**Performance:**
- DC - 1 GHz: Excellent
- 1-2 GHz: Very good
- >2 GHz: Usable

**Inductance:** ~0.5-1 nH total (very low)

### Method 3: Parallel Resistor Stack (Advanced, Best Performance)

**Concept:** Multiple resistors in parallel reduces inductance

**Parts:**
- Multiple 100Ω resistors (4-8 pieces)
- SMA connector or PCB
- Careful layout

**Construction:**

**Example: 4× 200Ω resistors**
```
200Ω  200Ω  200Ω  200Ω
  │     │     │     │
  └─────┴─────┴─────┘  = 50Ω total
```

**Layout:**
```
SMA center pin
    │
    ├─[R1 200Ω]─┐
    ├─[R2 200Ω]─┤
    ├─[R3 200Ω]─┤
    └─[R4 200Ω]─┘
                 │
            SMA shell
```

**Advantages:**
- Lower inductance (paralleling reduces L)
- Higher power handling
- Better RF performance

**Inductance:** ~0.2-0.4 nH (each resistor ~1.5 nH, divided by 4)

### Method 4: Commercial 50Ω Terminator (Best, but costs money)

**If you have budget:**

**Good ($10-20):**
- Mini-Circuits BLK-89-S+ (DC-18 GHz, SMA)
- Amphenol 132169 (DC-4 GHz)

**Excellent ($30-50):**
- Keysight/Agilent 909D (DC-18 GHz)
- Pasternack PE6TR1001 (DC-40 GHz)

**Advantage:** Calibrated, very low inductance, very flat 50Ω
**Disadvantage:** Costs money (defeats "homebrew" goal)

### Selecting Through-Hole Resistors

**Resistor types ranked:**

| Type | RF Performance | Cost | Availability |
|------|---------------|------|--------------|
| Carbon composition | Poor | Low | Common |
| Carbon film | Poor-Fair | Low | Common |
| Metal film | Good | Low | Common |
| Metal oxide | Fair-Good | Medium | Common |
| Wire-wound | **Terrible** | Low | Common |
| Non-inductive | Excellent | High | Rare |

**Never use wire-wound!** (pure inductor at RF)

**Best choice:** Metal film, short leads, 1% or better

### Measuring Resistor Values

**Without LCR meter:**

1. **Use quality DMM**
   - 4-wire (Kelvin) measurement if available
   - Let resistor stabilize (temp)
   - Read in Ω with 0.1Ω resolution

2. **Multiple resistors:**
   - Buy 10× 51Ω resistors
   - Measure all
   - Pick closest to 50.0Ω
   - Or select matched pair for parallel

**Example measurements:**
```
Bought 10× 51Ω ±1% resistors:
R1: 50.6Ω
R2: 51.2Ω
R3: 50.8Ω
R4: 51.0Ω
R5: 50.4Ω ← Best, use this one
R6: 51.4Ω
R7: 50.9Ω
R8: 51.1Ω
R9: 50.7Ω
R10: 51.3Ω

Choose R5 (50.4Ω, only +0.8% error)
```

### Verifying LOAD Quality

**Test 1: DC Resistance**
```
DMM measurement:
Target: 50.0Ω
Acceptable: 49.5-50.5Ω (±1%)
Good: 49.75-50.25Ω (±0.5%)
Excellent: 49.9-50.1Ω (±0.2%)
```

**Test 2: SWR on NanoVNA**
```
Connect LOAD to NanoVNA
Sweep 1-900 MHz
Format: SWR

Should read: 1.00:1 to 1.05:1 across band
If >1.1:1 → resistor not 50Ω or too inductive
```

**Test 3: Smith Chart**
```
Format: Smith chart
Should stay dead center (or very close)
If drifts right (capacitive) → lead capacitance
If drifts left (inductive) → lead inductance
```

**Test 4: Return Loss**
```
Format: LOGMAG (S11)
Should show <-26 dB (SWR 1.1:1)
Better: <-32 dB (SWR 1.05:1)
Excellent: <-40 dB (SWR 1.02:1)
```

**Test 5: Frequency Response**
```
Sweep 10-1000 MHz
Impedance should stay 50Ω ±2Ω
If impedance rises with frequency → inductive
If impedance falls → capacitive
```

---

## Part 5: Building DIY THRU

**Purpose:** Connects PORT 1 to PORT 2 for 2-port calibration

### Method 1: SMA Male-Male Adapter

**Parts:**
- SMA male to male barrel adapter ($2-5)

**That's it!** Just a simple adapter.

**Verification:**
- Should have low insertion loss (<0.3 dB to 1 GHz)
- Return loss >20 dB

### Method 2: SMA Cable

**Parts:**
- Short SMA cable (3-6 inches)
- Known good quality

**Advantage:** Can specify cable parameters in cal
**Disadvantage:** Cable loss, cable flex affects cal

**Better for:** Lower frequencies (<500 MHz)

### Method 3: PCB THRU

**Parts:**
- PCB with two SMA connectors
- 50Ω trace between them

**Construction:**
```
[SMA 1]───────[50Ω microstrip]───────[SMA 2]
       ╱                           ╲
  [Ground plane]               [Ground plane]
```

**Microstrip calculator:**
- Dielectric: FR4, εᵣ = 4.3
- Thickness: 1.6mm
- Target: 50Ω
- Result: ~3mm trace width

**Advantage:** Very repeatable, low loss
**Disadvantage:** Requires PCB fabrication

### Verifying THRU Quality

**Test 1: Insertion Loss (S21)**
```
Format: LOGMAG S21
Should show: 0 dB (±0.5 dB) DC-1 GHz

Barrel adapter: ~0.1-0.2 dB typical
Cable (6"): ~0.3-0.5 dB typical
```

**Test 2: Return Loss (S11)**
```
Should show: <-20 dB (good matching)
```

---

## Part 6: Testing and Verification

### Cross-Checking Standards

**Use known good components to verify:**

**Test 1: 100Ω Resistor**
```
1. Calibrate NanoVNA with your standards
2. Measure 100Ω resistor (1% metal film)
3. Should read 100Ω ±5Ω
4. If reads 90Ω or 110Ω → LOAD standard is wrong
```

**Test 2: Series Capacitor**
```
Known: 100pF capacitor @100MHz
Reactance: Xc = 1/(2πfC) = 1/(2π×100e6×100e-12) = 15.9Ω
Impedance: ~16Ω -j at 100MHz

Measure with NanoVNA:
Should show ~16Ω capacitive reactance
```

**Test 3: Known Filter**
```
If you have commercial filter with known specs:
- Measure with NanoVNA
- Compare to datasheet
- Should match within ±0.5 dB
```

### Comparing to Supplied Cal Kit

**If you have original cal kit:**

1. **Calibrate with supplied kit**
2. **Measure 50Ω LOAD**
3. **Note reading**

4. **Calibrate with DIY kit**
5. **Measure same 50Ω LOAD**
6. **Compare readings**

**Should agree within ±5%**

---

## Summary Table

| Standard | Difficulty | Cost | Performance |
|----------|-----------|------|-------------|
| **SHORT** | Easy | $0-2 | Excellent |
| - SMA short | Very easy | $2 | Good to 2 GHz |
| - Copper tape | Easy | $1 | Good to 1 GHz |
| **OPEN** | Very easy | $0-2 | Excellent |
| - SMA female | Very easy | $2 | Good to 1 GHz |
| - Nothing! | Very easy | $0 | Fair |
| **LOAD** | Medium | $2-15 | Varies |
| - Through-hole | Easy | $2 | Good to 200 MHz |
| - SMD | Medium | $5 | Good to 2 GHz |
| - Parallel stack | Hard | $10 | Excellent to 2 GHz |
| - Commercial | Easy | $15-50 | Excellent to 18 GHz |
| **THRU** | Easy | $2-5 | Good |
| - SMA adapter | Very easy | $3 | Good to 2 GHz |
| - Cable | Very easy | $5 | Good to 1 GHz |

---

## Recommended Starter Kit

### Budget Build ($10)

**Buy:**
- 10× 51Ω resistors 1% metal film ($1)
- 2× SMA connectors ($4)
- SMA male-male adapter ($3)
- Solder, wire (have)

**Build:**
- SHORT: Solder wire in SMA
- OPEN: Empty SMA female
- LOAD: Best 51Ω resistor in SMA
- THRU: Adapter

**Time:** 1-2 hours
**Performance:** Good to 500 MHz

### Better Build ($25)

**Buy:**
- SMD resistors 0805 50Ω ($2)
- 4× SMA connectors ($8)
- Small PCB ($5)
- Commercial 50Ω terminator ($10)

**Build:**
- SHORT: Low-inductance copper strap
- OPEN: SMA with defined capacitance
- LOAD: Commercial + DIY backup
- THRU: Barrel adapter

**Time:** 2-3 hours
**Performance:** Good to 1 GHz

---

## Next Steps

1. **Build or verify your calibration standards**
2. **Test each standard** with methods above
3. **Document** actual values (LOAD resistance, OPEN capacitance)
4. **Proceed to calibration procedure:**
   → [nanovna_port_calibration.md](nanovna_port_calibration.md)

---

**Now you have high-quality calibration standards for $10-25!**
