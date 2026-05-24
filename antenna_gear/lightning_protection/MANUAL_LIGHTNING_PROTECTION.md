# TECHNICAL MANUAL TM-LSP-001
## Lightning Arrestors and Surge Protection for Amateur Radio Antenna Systems
### Revision A — April 2026

---

```
DISTRIBUTION STATEMENT A: Approved for unrestricted distribution.

This manual covers installation, operation, and maintenance of lightning
surge protection equipment for amateur radio antenna systems operating
on 160 meters through 23 centimeters.  All designs comply with NEC
Article 810, NFPA 780, and ARRL grounding practices.

WARNINGS APPEAR IN THIS MANUAL WHERE APPLICABLE.
ALL SAFETY WARNINGS MUST BE READ AND UNDERSTOOD BEFORE INSTALLATION.
```

---

## TABLE OF CONTENTS

| Chapter | Title | Page |
|---------|-------|------|
| 1 | Introduction and Safety | 3 |
| 2 | Lightning Physics | 5 |
| 3 | Protection Philosophy and Strategy | 9 |
| 4 | Protection Devices — Theory and Selection | 12 |
| 5 | GDT Arrestor Design and Installation | 19 |
| 6 | Multi-Stage Protection (GDT + MOV + TVS) | 25 |
| 7 | DC-Grounded and DC-Blocked Configurations | 31 |
| 8 | Station Grounding System | 36 |
| 9 | Band-by-Band Quick Reference | 44 |
| 10 | Installation Procedures | 48 |
| 11 | Testing and Measurement | 53 |
| 12 | Troubleshooting | 57 |
| Appendix A | Parts List Master | 61 |
| Appendix B | NEC/NFPA 780 Code Summary | 63 |
| Appendix C | Insertion Loss and VSWR Calculations | 65 |
| Appendix D | Schematic File Index | 67 |

---

## CHAPTER 1 — INTRODUCTION AND SAFETY

### 1.1 Purpose

This manual provides engineering-level documentation for the design, construction, installation, and maintenance of lightning surge protection systems for amateur radio stations. Coverage extends from 1.8 MHz (160 meters) through 1296 MHz (23 centimeters).

The designs presented are not commercial products. They are engineered solutions requiring understanding of the underlying theory. A technician who understands *why* the design works can adapt it; a technician who only follows steps cannot troubleshoot failures.

### 1.2 Scope

This manual covers:

- Gas-discharge tube (GDT) coaxial arrestors for SO-239 and N-type connectors
- Multi-stage protection cascades (GDT + MOV + TVS diode)
- DC-grounded arrestors for static charge bleed-off
- DC-blocked configurations for active antennas and bias-T feeds
- Station ground electrode systems and bonding
- Rotor cable and control line surge protection
- 3D-printable mounting hardware (ASA material, weatherproof)
- Insertion loss and VSWR specifications for all designs

### 1.3 Safety Warnings

---

> **WARNING — LETHAL VOLTAGE**
> Lightning strikes involve currents of 20,000 to 200,000 amperes and voltages
> that can reach millions of volts. No protection system prevents death from
> direct lightning attachment. These systems reduce equipment damage from induced
> surges and nearby strikes. They do NOT make it safe to operate during
> thunderstorms or to handle antenna feed lines during lightning activity.

---

> **WARNING — DISCONNECT ANTENNA BEFORE THUNDERSTORMS**
> The single most effective protection is physical disconnection. Disconnect
> all antenna feed lines from radio equipment and ground them at the entry panel
> before any thunderstorm arrives. Do not wait until lightning is visible.
> Thunderstorm cells can propagate surge energy via ground currents and
> electromagnetic induction well before the first visible lightning.

---

> **WARNING — GROUND ROD INSTALLATION**
> Driving ground electrodes near buried utilities can cause electrocution or
> utility damage. Call 811 (USA) — "Call Before You Dig" — at least 48 hours
> before installing ground rods. This is a legal requirement in all 50 states.

---

> **CAUTION — HIGH RF POWER**
> Sparkover voltage selection must account for peak RF voltage on the
> transmission line. Incorrect GDT selection causes the device to arc during
> normal transmit operation. See Chapter 4 for calculations.

---

### 1.4 Related Documents

| Document | Title |
|----------|-------|
| TM-CMC-001 | Common-Mode Chokes for RFI Suppression |
| TM-AF-001  | Audio Filters — CW and SSB |
| ARRL Handbook | Chapter on Station Grounding and Bonding |
| NEC Article 810 | Radio and Television Equipment |
| NEC Article 250 | Grounding and Bonding |
| NFPA 780 | Standard for Installation of Lightning Protection Systems |

### 1.5 Conventions

**SHALL** — mandatory requirement
**SHOULD** — recommended; deviation requires documented justification
**MAY** — optional

All impedance values are at 50Ω system impedance unless noted.
All frequency references are in MHz unless noted.
All voltage values are peak (not RMS) unless marked "rms."

---

## CHAPTER 2 — LIGHTNING PHYSICS

### 2.1 Formation of the Lightning Channel

Lightning develops from charge separation within cumulonimbus clouds. The mechanism involves ice crystal and water droplet interactions during strong updrafts, which segregate positive charge toward cloud tops and negative charge toward cloud base. When the electric field between cloud base and ground exceeds the dielectric breakdown strength of air (~3 MV/m), ionization begins.

The process is not instantaneous — it is a cascade:

**Step Leader (Descending):**
A pilot leader propagates downward from cloud base in a series of discrete steps, typically 50 meters per step at 50-microsecond intervals. The step leader is faintly luminous and not the visible lightning bolt. It creates an ionized channel filling with negative charge as it descends.

**Upward Streamers (Ascending):**
When the step leader descends to within 50–100 meters of the ground, the concentrated electric field at elevated objects (trees, towers, masts, ground rods) causes corona discharge. These upward streamers reach toward the descending step leader. The structure that makes the successful connection completes the circuit.

**Return Stroke:**
When upward streamer and step leader connect, an extraordinarily bright return stroke travels *upward* from ground to cloud at roughly one-third the speed of light. This is the visible lightning flash. Peak current: 20 kA typical, 200 kA extreme. Duration: 20–50 microseconds for the main stroke.

**Subsequent Strokes:**
A single lightning flash contains 3–5 return strokes on average, with each subsequent stroke lasting 200–500 microseconds and following the already-ionized channel.

### 2.2 Current and Voltage Parameters

The standard surge waveform used in equipment testing is the 8/20 µs waveform:
- 8 µs rise time to peak
- 20 µs decay time to half-peak

For antenna system design, the relevant parameters are:

| Parameter | Typical Value | Extreme Value |
|-----------|---------------|---------------|
| Peak current (direct attachment) | 20 kA | 200 kA |
| Peak current (induced, 100m) | 5 kA | 50 kA |
| Peak current (induced, tower base) | 500 A | 5 kA |
| Peak voltage at coax feedpoint | 5 kV | 50 kV |
| Rise time | 1–2 µs | 0.1 µs |
| Energy (total stroke) | 1–10 kJ | 100 kJ |

### 2.3 Lightning Electromagnetic Pulse (LEMP)

A return stroke radiates electromagnetic energy across a broad spectrum from DC to above 100 MHz. The near-field component of LEMP couples into antenna systems even when the strike is kilometers distant.

**Coupling mechanisms:**

*Conductive coupling:* Current flows through ground, enters the station ground system, and appears as common-mode voltage on all equipment connected to that ground.

*Inductive coupling:* The rapidly changing magnetic field of the return stroke induces voltage in any loop formed by antenna feed lines, cables, and equipment. A loop of 1 square meter at 1 km from a 20 kA return stroke experiences approximately 2 kV induced voltage.

*Capacitive coupling:* The changing electric field between cloud base and ground couples directly into elevated conductors (antennas). Vertical antennas and unshielded wire antennas are most susceptible.

### 2.4 Ground Potential Rise

During a direct lightning attachment to a ground rod or tower base, enormous current flows through the soil resistance. If the ground electrode resistance is 10Ω and the peak current is 20 kA, the electrode voltage rises 200,000 volts above remote earth potential. This potential gradient in the soil can destroy equipment connected to any other grounding point within tens of meters.

This is why the single-point grounding rule is not optional: multiple ground rods at different potentials during a surge will destroy equipment connected between them through differential voltage.

### 2.5 Static Charge Accumulation

Even without lightning, antennas accumulate static charge from:
- Wind-driven precipitation (rain, snow, dust)
- Corona from antenna tips in dry air
- Nearby cloud charge induction

A floating antenna can accumulate thousands of volts of static charge. This charge is released suddenly when sufficient voltage builds to arc through the receiver front-end. The GDT and DC-grounded designs provide continuous low-resistance paths to discharge static accumulation gradually, preventing the sudden arc discharge.

---

## CHAPTER 3 — PROTECTION PHILOSOPHY AND STRATEGY

### 3.1 What Protection Can and Cannot Do

**Cannot prevent:**
- Destruction from direct lightning attachment to the antenna
- Shock or electrocution from handling equipment during active lightning

**Can accomplish:**
- Diversion of induced surge energy away from equipment
- Clamping of residual voltage to levels the radio front-end can survive
- Continuous drainage of static charge accumulation
- Protection against all nearby-strike induced events (the most common scenario)

### 3.2 Hierarchy of Protection Measures

Listed in order of effectiveness:

1. **Physical disconnection** — disconnect all feed lines and ground them before thunderstorms; 100% effective
2. **Single-point grounding** — prevents ground potential differences from damaging equipment
3. **Low-resistance ground electrode** — provides a path that is more attractive to lightning than the equipment
4. **Entry panel arrestors** — diverts surge energy at the point of entry before it reaches equipment
5. **Multi-stage protection** — provides fine clamping after coarse GDT diversion
6. **In-shack bonding** — equalizes ground potentials among all equipment
7. **Static bleed (DC-grounded arrestor)** — prevents static charge accumulation

### 3.3 The Energy Diversion Concept

Protection does not absorb or destroy surge energy. It diverts surge current to the ground electrode before that current can reach equipment. The ground system then disperses the current into the soil.

The key insight: the ground electrode must present a lower impedance path to surge current than any path through the equipment. This means:

- Ground resistance must be low (< 25Ω, ideally < 10Ω)
- Ground bond conductors must be wide copper strap (not round wire — see Chapter 8)
- The path from arrestor to ground electrode must be as short and straight as possible

### 3.4 The Three-Stage Cascade

No single protection device optimizes all parameters. The three-stage cascade is the fundamental design pattern:

| Stage | Device | Speed | Current | Limitation |
|-------|--------|-------|---------|------------|
| 1 | GDT | ~100 ns | 2.5–20 kA | Slow sparkover; passes fast transients |
| 2 | MOV | ~10 ns | 1–10 kA | High capacitance (HF only) |
| 3 | TVS diode | < 1 ns | 0.5–50 kA | Lower current capacity; capacitance |

Decoupling inductors between stages prevent fast stages from conducting before slow stages have fired. The inductor is part of the protection design, not an incidental component.

### 3.5 When a Single GDT Is Adequate

A single GDT is adequate for:
- Static charge bleed-off (low energy, slow)
- Induced surges from distant strikes (> 500m) when the residual pre-sparkover voltage is within radio tolerance
- QRP stations (< 10W) where transmit peak voltage is well below GDT sparkover threshold

A single GDT is NOT adequate for:
- Modern stations with solid-state front-ends (destroyed by < 10 ns transients)
- Any station near (< 500m) frequent strike areas
- Stations with elevated antennas on metal towers

### 3.6 Economic Triage

Protection cost should be proportional to equipment cost. A $50,000 station justifies more comprehensive protection than a $500 station. However:

- A single GDT arrestor costs approximately $5–15 in components and provides substantial protection against static and distant-strike induced surges. There is no economic justification for skipping this step.
- Multi-stage protection adds $20–50 per feedline and provides substantially better protection. Warranted for any station with HF radio equipment.
- Complete station bonding with multiple ground rods costs several hundred dollars in materials plus labor, and is warranted for permanent installations in lightning-active areas.

---

## CHAPTER 4 — PROTECTION DEVICES: THEORY AND SELECTION

### 4.1 Gas-Discharge Tubes (GDT)

**Construction:** Two or three electrodes in a ceramic envelope, filled with inert gas (argon or nitrogen) at reduced pressure (~100 mbar). Below the sparkover voltage, the gas is non-conducting — the device is essentially open-circuit.

**Sparkover mechanism:** When voltage across the electrodes exceeds the DC sparkover voltage V_s, the gas ionizes through a glow discharge transitioning to an arc. The arc voltage V_arc is 10–25V — very low compared to sparkover voltage. Time to sparkover: approximately 100 nanoseconds.

**Recovery:** After the surge event, current ceases and the gas de-ionizes. The GDT self-resets within 200 µs. It can survive thousands of surge events.

**Critical parameter — follow current:** After sparkover, the GDT will sustain conduction on AC line voltage if present. The maximum AC follow current that self-extinguishes is typically 150A rms. Above this, a GDT may latch in conduction and become a short circuit. This is generally not a concern for antenna coaxial applications (no sustained AC voltage) but is critical for AC line protection.

#### 4.1.1 Sparkover Voltage Selection

The sparkover voltage V_s must exceed the maximum peak RF voltage present on the transmission line during normal transmit operation. Failure to observe this will cause the GDT to fire during transmit.

**Peak RF voltage calculation:**

```
V_peak = √(2 × P × Z₀)

where:
  P  = transmit power (watts)
  Z₀ = characteristic impedance (50Ω)

For matched load (SWR = 1:1):
  100W:   V_peak = √(10,000) = 100V
  500W:   V_peak = √(50,000) = 224V
  1500W:  V_peak = √(150,000) = 387V

For SWR = 3:1, maximum voltage node:
  V_peak_max = V_peak_matched × √SWR
  1500W, SWR 3:1: V_peak_max = 387 × 1.73 = 670V
```

**GDT selection table:**

| Power | SWR | V_peak | Required V_s | Recommended |
|-------|-----|--------|--------------|-------------|
| QRP ≤ 10W | any | 31V | ≥ 90V | 90V ← max protection |
| 100W | ≤ 2:1 | 141V | ≥ 150V | 150V |
| 500W | ≤ 2:1 | 316V | ≥ 350V | 350V |
| 1500W | ≤ 1.5:1 | 474V | ≥ 600V | 600V |
| 1500W | ≤ 3:1 | 670V | ≥ 1000V | 1000V (reduced margin) |

> **CAUTION:** For high-power stations with antennas presenting SWR > 2:1, calculate the actual voltage at the feedpoint (worst-case voltage maximum) before selecting GDT sparkover voltage. Selecting too-low V_s is the most common installation error.

#### 4.1.2 Capacitance and Insertion Loss

GDT shunt capacitance is typically 0.5–1.5 pF. This is extremely low and produces negligible insertion loss at all amateur frequencies:

```
f₋₃dB = 1 / (2π × Z₀/2 × C_GDT)    [shunt element in 50Ω line: Z₀/2 = 25Ω]

For C_GDT = 1.0 pF:
  f₋₃dB = 1 / (2π × 25 × 1×10⁻¹²) = 6.37 GHz

Insertion Loss (dB) ≈ 20 × log₁₀(1 + (f/f₋₃dB)²/2)

  144 MHz:  IL = 0.0024 dB
  432 MHz:  IL = 0.022 dB
  1296 MHz: IL = 0.098 dB  ← just within 0.1 dB spec
```

The GDT is the only protection device that meets the < 0.1 dB insertion loss specification through 1296 MHz. This is why it is used as the first stage in all designs.

#### 4.1.3 Recommended GDT Parts

| Part Number | V_s | I_peak | C | Application |
|-------------|-----|--------|---|-------------|
| EPCOS B88069X1140B232 | 90V | 2.5 kA | 1.0 pF | QRP and all VHF/UHF |
| Bourns 2040-090TR | 90V | 2.5 kA | 1.5 pF | QRP axial-lead |
| EPCOS B88069X3140B232 | 150V | 2.5 kA | 1.0 pF | 100W HF |
| EPCOS B88069X5140B502 | 350V | 5.0 kA | 1.0 pF | 500W HF |
| EPCOS B88069X8140B902 | 600V | 10 kA | 1.0 pF | 1500W HF |
| Bourns 2040-600TR | 600V | 10 kA | 1.5 pF | 1500W alt. |

### 4.2 Metal Oxide Varistors (MOV)

**Construction:** Zinc oxide granules in a sintered ceramic disc. Each ZnO grain junction is a back-to-back zener. The device conducts bidirectionally above the clamping voltage.

**Response time:** 10–50 ns — faster than GDT but much slower than TVS diode.

**Peak current:** 1–10 kA — less than GDT.

**Clamping voltage:** Not precise; typically 1.5–2.5× the rated working voltage. The clamping voltage rises under high current (non-ideal).

**Degradation:** Unlike GDT and TVS diodes, MOVs degrade with repeated energy absorption. Each surge reduces their clamping capacity. Degraded MOVs may fail short-circuit (catastrophic) or open-circuit.

**Capacitance:** 100–1000 pF depending on disc size and rating. This high capacitance makes MOVs unsuitable for VHF applications. They are restricted to HF designs (< 54 MHz) where the capacitive reactance is acceptable.

**Use in multi-stage design:** MOV provides the intermediate stage — faster than GDT but not as fast as TVS, higher current than TVS but not as high as GDT. Its high capacitance is acceptable since it is connected in the mid-stage between two inductors, not directly across the main signal path (at VHF, eliminate the MOV stage entirely).

| Part Number | V_r (working) | V_cl | I_peak | C | Application |
|-------------|---------------|------|--------|---|-------------|
| Littelfuse V150LA10BP | 150V | 243V | 1.0 kA | 180pF | Multi-stage HF |
| Littelfuse V275LA10BPX | 275V | 455V | 1.0 kA | 90pF | AC line protection |
| Bourns MOV-07D150K | 150V | 240V | 1.2 kA | 220pF | HF stage 2 |

### 4.3 TVS Diodes (Transient Voltage Suppressors)

**Construction:** Silicon avalanche diode designed for repetitive transient absorption. Bidirectional ("CA" suffix) versions use two diodes back-to-back and clamp both polarities.

**Response time:** < 1 ns — faster than any other surge protection device.

**Clamping voltage:** Precisely controlled by doping. V_clamp = V_BR + V_f × I_ratio ≈ V_BR × 1.3.

**Peak current:** Limited — typically 500 A for standard devices. Low-capacitance VHF-rated devices may handle only 200 A peak.

**Capacitance:** Strongly frequency-dependent. Standard TVS diodes have 50–500 pF; this is unacceptable at VHF. Low-capacitance RF-optimized TVS devices achieve < 5 pF.

#### 4.3.1 TVS Selection for VHF/UHF

At frequencies above 30 MHz, standard TVS diodes cause excessive insertion loss due to junction capacitance. The following parts are suitable:

| Part | V_BR | I_peak | C_j | f_max | Notes |
|------|------|--------|-----|-------|-------|
| SMBJ54CA | 54V | 600W | 130 pF | 20 MHz | HF only |
| BZW04-P18B | 20V | 3.5 kA | 50 pF | 80 MHz | High-current HF |
| SM6T39CA | 39V | 600W | 5 pF | 300 MHz | HF+2M |
| PSOT-36A | 36V | 600W | 1 pF | 1 GHz | 70cm–23cm |
| ESD9L5.0ST5G | 5V | 200A | 0.35 pF | 1.5 GHz | VHF/UHF/µwave |
| CDSOT23-T24C | 24V | 30A | 2 pF | 600 MHz | Low-energy ESD |

> **NOTE:** The ESD9L5.0ST5G (0.35 pF) is rated for only 200A peak. This is adequate as a final stage *after* the GDT has already diverted the main surge current. It is not adequate as a standalone device.

### 4.4 Decoupling Inductors

The inductors between protection stages are not optional impedance. They are functional components that determine how current is shared between stages during the fast rise of a lightning transient.

**Function:** A series inductor limits the rate of current change dI/dt. During the first 100 ns before GDT sparkover, the inductor limits current flow to the downstream (fast) TVS stage. After GDT sparkover, the inductor allows steady-state current to flow freely through the GDT path.

**Value selection:**

For HF (1.8–54 MHz): L1 = 100 nH, L2 = 47 nH
- Reactance at 54 MHz: X_L1 = 34Ω, X_L2 = 16Ω
- Insertion loss at 54 MHz: < 0.1 dB (acceptable)

For VHF (50–432 MHz): L1 = 22 nH, L2 = 10 nH
- Reactance at 432 MHz: X_L1 = 60Ω — this limits the VHF version to 432 MHz with acceptable IL
- For 1296 MHz: reduce L1 to 10 nH, L2 to 4.7 nH

**Physical implementation:** Use 0805 chip inductors (Coilcraft 0805CS series) on a 50Ω microstrip PCB. Avoid leaded inductors — their self-resonant frequency is unpredictable at VHF.

---

## CHAPTER 5 — GDT ARRESTOR DESIGN AND INSTALLATION

### 5.1 HF Arrestor — SO-239/SO-239 Through Line

**Application:** 1.8–54 MHz, up to 1500W (with appropriate V_s selection), weatherproof outdoor installation.

**Topology:** GDT connected from center conductor to chassis ground. Both connector outer conductors are bonded to chassis. Signal passes through without interruption; GDT only conducts during a surge event.

**Signal path:**

```
ANTENNA COAX → [SO-239 IN] → center conductor ─────── [SO-239 OUT] → TO SHACK
                                      │
                                    [GDT1]   (connected from center to chassis)
                                      │
                          CHASSIS GROUND ─────────── 1/2" copper strap ──── GROUND ROD
```

No series components in the signal path. GDT is a pure shunt element.

**Enclosure requirements:**
- Aluminum or copper alloy (NOT plastic for main body — must be RF-grounded chassis)
- IP54 minimum — gasketed lid, sealed connector flanges
- ASA 3D-printed enclosure acceptable if metallic ground plane is added inside
- Ground lug: two-hole copper lug, M6 stud, rated for #6 AWG or 1/2" strap

**Connector mounting:**
- SO-239 connector outer conductor must make full metal-to-metal contact with enclosure wall
- Use silver-plated copper washers as gaskets between connector flange and enclosure
- Apply anti-seize compound on stainless steel hardware in contact with aluminum
- RTV silicone sealer around exterior of connector flange (weatherseal)

**Performance specifications (1.0 pF GDT):**

| Frequency | Insertion Loss | VSWR |
|-----------|---------------|------|
| 1.8 MHz | < 0.001 dB | 1.001 |
| 7.0 MHz | < 0.001 dB | 1.001 |
| 14.0 MHz | < 0.001 dB | 1.001 |
| 28.0 MHz | < 0.001 dB | 1.001 |
| 50.0 MHz | < 0.001 dB | 1.001 |

### 5.2 VHF/UHF Arrestor — N-Type Through Line

**Application:** 50 MHz through 1296 MHz.

**Required changes from HF design:**
- N-type connectors (IP67; superior weatherproofing; specified above 300 MHz)
- GDT with C ≤ 1.0 pF (verify part number — some GDTs are rated 1.5 pF or higher)
- For single GDT above 1296 MHz: V_s must still exceed RF peak voltage

**Performance specifications (1.0 pF GDT):**

| Frequency | Insertion Loss | VSWR | Return Loss |
|-----------|---------------|------|-------------|
| 50 MHz | < 0.001 dB | 1.001 | 67 dB |
| 144 MHz | < 0.002 dB | 1.003 | 51 dB |
| 432 MHz | 0.022 dB | 1.027 | 41 dB |
| 902 MHz | 0.055 dB | 1.06 | 31 dB |
| 1296 MHz | 0.098 dB | 1.09 | 27 dB |

> **NOTE:** A dual-GDT (two GDTs in parallel, 2 pF total) fails the 0.1 dB spec at 1296 MHz. For 23cm installations, use a single GDT only.

**Connector specifications:**
- Amphenol 82-836: N-female, panel mount, silver-plated, IP67, O-ring face seal
- Maximum RF power: 1500W at 450 MHz (50Ω resistive load)
- PTFE dielectric — rated to 165°C; suitable for direct sunlight exposure

### 5.3 Sparkover Voltage Verification

Before installing any GDT arrestor, verify the sparkover voltage selection against actual operating conditions:

1. Determine maximum transmit power.
2. Measure actual SWR at the feedpoint (not at the shack — these differ).
3. Calculate V_peak at feedpoint using the formula in Section 4.1.1.
4. Select V_s ≥ 1.5 × V_peak for adequate margin.
5. Document the calculation in the station logbook or maintenance records.

For multi-band antennas, calculate V_peak at each operating band. SWR can vary significantly by band. Use the highest calculated V_peak to select V_s.

---

## CHAPTER 6 — MULTI-STAGE PROTECTION

### 6.1 Design Rationale

A single GDT provides adequate protection for distant-strike induced surges, but it cannot protect modern semiconductor front-ends against the pre-sparkover transient — the 100–200 nanoseconds before the GDT fires during which fast voltage spikes appear on the output side.

Modern HF receivers have solid-state front-ends (MOSFET or GaAsFET gates, PIN diode switches, mixers) rated for gate-oxide breakdown at 20–200V. A pre-sparkover spike of 500–2000V is enough to destroy these devices.

The multi-stage design ensures that even before the GDT fires, downstream voltage is limited by the TVS diode to its clamping voltage (approximately 60–80V).

### 6.2 HF Multi-Stage Design (1.8–54 MHz)

**Schematic topology:**

```
IN_center ── [L1: 100nH] ──┬── [L2: 47nH] ──┬── OUT_center
                            │                 │
                          [GDT1]           [TVS1+TVS2]
                          [MOV1]           back-to-back
                          90V, 2.5kA       54V, SMBJ54CA
                            │                 │
CHASSIS ────────────────────┴─────────────────┘
```

GDT1 is connected between the L1 input node and chassis.
MOV1 is connected between the L1/L2 junction and chassis.
TVS1 is connected between the L2 output and chassis.

**Stage activation sequence (2kV, 10kA surge):**

- t = 0 to 100 ns: Voltage rises toward 2kV. L1 slows current to TVS. MOV1 activates at ~200V, clamps to ~200V at mid-point. TVS1 activates at ~65V, clamps OUT to ~65V.
- t = 100 ns: GDT1 fires. Sparkover occurs. GDT1 clamps IN to ~20V (arc voltage).
- t > 100 ns: GDT1 in full arc conduction. Diverts remaining kA-level current. TVS1 sees only residual oscillation; handles < 500A. Radio sees < 75V throughout.

**Insertion loss — HF multi-stage:**

| Frequency | GDT IL | L1+L2 IL | TVS IL | Total IL |
|-----------|--------|----------|--------|----------|
| 1.8 MHz | < 0.001 dB | < 0.001 dB | < 0.001 dB | < 0.001 dB |
| 14.0 MHz | < 0.001 dB | 0.002 dB | 0.004 dB | 0.007 dB |
| 28.0 MHz | < 0.001 dB | 0.009 dB | 0.016 dB | 0.026 dB |
| 50.0 MHz | 0.001 dB | 0.029 dB | 0.052 dB | 0.082 dB ← within spec |

### 6.3 VHF Multi-Stage Design (50–432 MHz)

**Changes from HF design:**
- MOV stage eliminated (too much capacitance)
- L1 reduced to 22 nH, L2 to 10 nH
- TVS replaced with SM6T39CA (5 pF) for 2M/220MHz, or ESD9L5.0ST5G (0.35 pF) for 70cm and above

```
IN_center ── [L1: 22nH] ──┬── [L2: 10nH] ──┬── OUT_center
                           │                 │
                         [GDT1]           [TVS_VHF]
                         90V, 1pF         SM6T39CA or
                           │              ESD9L5.0ST5G
CHASSIS ───────────────────┴─────────────────┘
```

**Insertion loss — VHF multi-stage:**

| Frequency | GDT IL | L1+L2 IL | TVS IL | Total IL |
|-----------|--------|----------|--------|----------|
| 50 MHz | 0.001 dB | 0.002 dB | < 0.001 dB | 0.003 dB |
| 144 MHz | 0.002 dB | 0.016 dB | 0.001 dB | 0.019 dB |
| 432 MHz | 0.022 dB | 0.142 dB | 0.006 dB | 0.170 dB ← use smaller L |

> For 432 MHz, reduce to L1 = 10 nH, L2 = 4.7 nH to bring total IL below 0.1 dB.

### 6.4 PCB Layout Considerations

Multi-stage protection requires a PCB to control the physical geometry and impedance. Point-to-point wiring introduces parasitic inductance that defeats the design intent.

**50Ω microstrip trace width:** For FR-4 (ε_r = 4.4, h = 1.6 mm): W ≈ 3.0 mm for 50Ω.

**Layout rules:**
- Signal trace: continuous 50Ω microstrip from IN connector to OUT connector
- GDT and MOV connect via short (<5mm) stubs from main trace to chassis plane
- TVS diode placed directly across trace at output stage, body length along trace axis
- All ground connections return via uninterrupted copper pours to chassis
- Inductor L1 and L2 in main trace; no vias in the signal path if avoidable
- Pour-over (conformal coat) the completed PCB before installation

---

## CHAPTER 7 — DC-GROUNDED AND DC-BLOCKED CONFIGURATIONS

### 7.1 DC-Grounded Arrestor (Static Bleed)

**Purpose:** Provide a DC path from the antenna center conductor to ground for static charge drainage, while maintaining RF signal continuity and surge protection.

**Application:** Any antenna that is not DC-grounded by its feedpoint geometry. Loop antennas, dipoles fed at center, and most verticals have DC continuity between feedpoint center and shield (or ground). Yagis, log-periodic arrays, and some quad configurations may be DC-floating at the feedpoint.

**Implementation:** An RF choke (RFC) in parallel with the GDT, connected from center conductor to chassis ground.

```
CENTER CONDUCTOR ──┬──────────────────── CENTER CONDUCTOR
                   │
                 [RFC]  220µH minimum (for 160M); 1 mH preferred
                   │
                 [GDT]  (surge protection)
                   │
CHASSIS ───────────┘
```

The RFC provides a DC path (resistance = wire DC resistance, typically < 1Ω).

Under normal RF operation, the RFC presents high impedance (X_L = 2π × f × L):

```
RFC = 220 µH:
  X_L at 1.8 MHz = 2π × 1.8×10⁶ × 220×10⁻⁶ = 2487Ω
  
Insertion loss from shunt impedance in 50Ω system:
  IL = 20 × log₁₀(|1 + Z₀/(2 × Z_shunt)|)
  IL at 1.8 MHz = 20 × log₁₀(|1 + 50/(2 × 2487)|)
               = 20 × log₁₀(1.01) = 0.086 dB  ← just within 0.1 dB spec
```

For better performance at 160M, use RFC ≥ 1 mH. Trade-off: larger/heavier core.

**RFC insertion loss table (220 µH RFC, 50Ω system):**

| Frequency | X_RFC | IL (dB) |
|-----------|-------|---------|
| 1.8 MHz | 2487Ω | 0.086 dB |
| 3.5 MHz | 4838Ω | 0.023 dB |
| 7.0 MHz | 9677Ω | 0.006 dB |
| 14.0 MHz | 19,354Ω | 0.001 dB |
| 28.0 MHz | 38,708Ω | < 0.001 dB |

**RFC winding:** 220 µH on T-200-2 toroid (Iron Powder, Mix 2, µ = 10):
- A_L = 120 nH/N² for T-200-2
- N = √(220,000 / 120) = 42.8 → 43 turns
- Use #24 AWG enamel wire, spaced winding (one wire diameter gap between turns)
- Self-resonant frequency: measure and document; do not use RFC at or near f_SR

### 7.2 DC-Blocked Configuration (Active Antennas and Bias-T)

**Purpose:** Protect active antennas (loop amplifiers, LNA-equipped antennas) and bias-T fed antennas where DC power must pass to the antenna via the coaxial center conductor.

**The common error — split capacitor topology:**

A series capacitor blocking DC to chassis will prevent the GDT from draining static charge to ground. If the GDT is placed after the capacitor, DC cannot reach the antenna. If placed before, it shorts the DC bias to ground.

There is no single-device solution to the contradictory requirements of passing DC to the antenna while also providing a DC path to ground for static drain. A different topology is required.

**Correct approach — two-device separation:**

1. **At the antenna feedpoint:** Install a standard GDT arrestor. This provides surge protection and static bleed. Connect the GDT between center conductor and shield/ground. The GDT is DC-transparent (high impedance below V_s).

2. **At the shack entry (inside) or near the transceiver:** Install a bias-T injector. The bias-T consists of:
   - RFC (≥ 1 mH, or appropriate for operating frequency) from DC supply to center conductor
   - Blocking capacitor (C_block, ≥ 100 pF for HF) in series with signal path to protect radio

```
ANTENNA ────[FEEDPOINT GDT]────────────────────────────────── COAX ──────────────────────────────┬────── RADIO
                                                                                                  │
                                                             SHACK BIAS-T:                     [C_block]
                                                             [RFC] from DC supply               to radio
                                                             [C_block] in signal path           │
                                                             GDT on entry panel for surge       ┴
                                                                                              (chassis)
```

**DC current path:** DC supply → RFC → coax center conductor → antenna amplifier → coax outer conductor / chassis → return.

**Signal path:** Antenna → coax center conductor → C_block → radio RX input.

**Surge path:** Surge → coax center conductor → GDT at entry panel → chassis → ground rod.

The blocking capacitor must be rated for the DC voltage plus any surge voltage that appears before the GDT fires. Use a 250V-rated capacitor minimum.

**Bias-T specifications:**

| Parameter | Value |
|-----------|-------|
| RFC inductance | ≥ 1 mH (160M) or ≥ 10 µH (10M and above) |
| C_block capacitance | 100–1000 pF (select for low X_C at lowest operating frequency) |
| C_block voltage rating | ≥ 250V |
| RFC DC resistance | < 5Ω (limits DC voltage drop) |
| Maximum DC current | Rated by RFC wire gauge; typically 100–500 mA |

---

## CHAPTER 8 — STATION GROUNDING SYSTEM

### 8.1 Grounding Objectives

Station grounding serves four purposes, in order of safety importance:

1. **Personnel safety:** Prevent fatal shock from faulty equipment with AC line contact
2. **RF common-mode elimination:** Single-point reference prevents ground loop currents
3. **Static discharge:** Continuous path for charge accumulated on antennas
4. **Lightning diversion:** Guide surge energy away from equipment to earth

A ground system must simultaneously satisfy:
- Low DC resistance (safety, static drain): < 25Ω (NEC minimum), < 10Ω (preferred)
- Low RF impedance at operating frequencies (RF ground): use wide copper strap
- Physical continuity: no connections that can corrode open over years outdoors
- Single-point topology: all grounds bond to ONE point

### 8.2 Why Round Wire Fails at RF

At RF frequencies, current flows only on the surface of conductors due to the skin effect. Skin depth in copper:

```
δ = 66.1 / √f    [mm, f in Hz]

At 1.8 MHz:   δ = 0.049 mm (49 µm)
At 14 MHz:    δ = 0.018 mm (18 µm)
At 144 MHz:   δ = 5.5 µm
```

The effective cross-section of a conductor at RF is its *surface area*, not its geometric cross-section. A flat strap has dramatically more surface area (two wide faces) than a round wire of the same cross-sectional area. Additionally, flat straps have lower self-inductance per unit length than round wires of equivalent cross-section.

**Inductance comparison — 1 meter length:**

```
#6 AWG round wire (4.1 mm dia):   L ≈ 1.35 µH   Z at 14 MHz ≈ 119Ω
1/2" × 0.05" copper strap:        L ≈ 0.57 µH   Z at 14 MHz ≈ 50Ω
1" × 0.05" copper strap:          L ≈ 0.45 µH   Z at 14 MHz ≈ 39Ω
2" × 0.05" copper strap:          L ≈ 0.35 µH   Z at 14 MHz ≈ 31Ω
3" × 1/8" copper bus bar:         L ≈ 0.28 µH   Z at 14 MHz ≈ 25Ω
```

**Rule: NEVER use round wire for RF ground bonds. Use the widest practical copper strap for the shortest practical path.**

**Bonding strap impedance table (practical quick reference):**

| Strap type | Z at 7 MHz | Z at 14 MHz | Z at 50 MHz | Recommendation |
|------------|-----------|-------------|-------------|----------------|
| #6 AWG wire, 1m | 38Ω | 75Ω | 268Ω | Not for RF grounding |
| #6 AWG wire, 0.1m | 4Ω | 8Ω | 27Ω | Absolute minimum (short bond only) |
| 1/2" strap, 1m | 16Ω | 32Ω | 113Ω | Adequate; keep short |
| 1/2" strap, 0.3m | 5Ω | 10Ω | 34Ω | Good |
| 1" strap, 0.3m | 3.5Ω | 7Ω | 25Ω | Good |
| 2" strap, 0.1m | 1Ω | 2Ω | 7Ω | Excellent |
| 3/4" copper braid, 0.3m | 2Ω | 4Ω | 13Ω | Very good; flexible |

### 8.3 Ground Electrode System

**NEC minimum requirements (Article 810.21 and 250.52):**
- One 8-foot (2.44m) copper-clad steel rod, 5/8" (15.9mm) minimum OD
- Where rock prevents 8-foot depth: L-shape allowed (horizontal section at ≥ 18" depth)
- Supplemental electrode required if measured ground resistance exceeds 25Ω

**Best practice for permanent ham installations:**
- Two rods minimum; three or four for lightning-active areas
- Rod spacing: ≥ 2× rod length (16 feet minimum between rods)
- 10-foot (3.05m) solid copper or copper-clad steel, 5/8" OD
- Install as close as possible to entry panel (< 3m preferred)
- Connect rods to each other with #6 AWG solid copper at 18" depth (bury, do not surface run)
- All rods bonded to entry panel bus; all at same potential

**Ground rod driving procedure:**

1. Call 811 (USA) at least 48 hours before digging or driving.
2. Select location: moist soil preferred. Avoid proximity to septic systems, buried utilities.
3. Wet dry soil with water before driving — reduces resistance and effort.
4. Use slide hammer or SDS drill with rod driver attachment.
5. Drive to full depth. If rock encountered within 8 feet, proceed as NEC L-shape.
6. Leave 2 inches (50mm) above grade for clamp installation.
7. Install UL-listed two-bolt copper ground rod clamp.
8. Torque hardware to clamp manufacturer specification (typically 25–35 ft-lbs).
9. Connect entry panel bus to clamp via 1/2" minimum copper strap.

**Ground resistance measurement:**

The proper method is the fall-of-potential three-electrode method, requiring a ground resistance tester (instrument, not a DMM). Target resistance: < 10Ω.

Simple verification with a DMM:
1. Connect one DMM lead to the ground rod clamp.
2. Connect the other to the AC power system neutral at the main panel.
3. Read AC voltage (it will be non-zero — soil is not a perfect conductor).
4. Reading < 2V: acceptable ground continuity.
5. Reading > 5V: potential grounding problem; investigate before relying on the system.

This test is not a substitute for proper resistance measurement. It indicates continuity only, not adequacy of ground resistance.

### 8.4 Single-Point Entry Panel

The entry panel is the most important element of the station protection system. All cables entering the shack cross this one physical point. All cable shields bond to one copper bus bar at this point.

**Entry panel requirements:**
- Location: exterior wall or tower base, as close to ground rod as practical
- Material: aluminum plate (minimum 6mm thick) or copper plate
- Size: 300mm × 150mm minimum for a three-feedline station
- Ground bond: 1/2" minimum copper strap, ≤ 1 meter, straight run to ground rod

**Rule: No bends, no coils in the ground strap between entry panel and ground rod.** A coil introduces high inductance; a 90° bend is acceptable but straight is better. For the path that diverts kA of lightning current, inductance is the enemy.

**Cable entry:** Each coax feedline passes through its own round hole in the panel. The GDT arrestor is mounted at the panel, with its chassis bonded to the panel.

**Bonding topology:**

```
All GDT chassis → Panel copper bus bar → 1/2" strap → Ground rod clamp → Ground rod
```

All bonds from the panel to the ground rod must be at zero impedance (direct metal-to-metal, hardware tightened). All bonds from GDT chassis to panel bus must be short (< 50mm) and use wide copper straps or lugs.

### 8.5 In-Shack Bonding

All equipment inside the shack must be bonded to a single RF ground bus. This prevents differential ground voltages during a surge from creating destructive current through interconnecting cables.

**In-shack bus:** 1" × 0.05" copper bus bar, run along the back edge of the operating desk. Available as "copper bus bar" from electrical suppliers.

**Equipment bonding:**
- All radio equipment chassis → in-shack bus via shortest practical braid (< 0.5m)
- In-shack bus → entry panel bus via 2" copper strap through wall

**What NOT to do:**
- Do not install a separate ground rod inside the shack or through the floor. An interior rod at a different potential from the exterior rod during a surge will destroy equipment.
- Do not daisy-chain equipment grounds (radio → amplifier → radio) — this creates a ground loop.
- Do not use the building conduit system as a ground bus. It has too many joints and is not designed for RF.

### 8.6 NEC Compliance Notes

**NEC 250.106:** The antenna ground (ground rod at entry panel) and the AC power safety ground must be bonded together. These two systems must be at the same potential. Accomplish with #6 AWG or larger copper wire from entry panel bus to AC main panel ground bus.

**NEC 810.21:** Specifically addresses ham and broadcast antenna discharge units (including GDT arrestors). Requires:
- Listed (UL or equivalent) discharge unit
- Discharge unit connected to a grounding electrode
- Electrode at a minimum distance from building openings (typically 6 feet horizontal from window/door)

**NFPA 780:** Covers structural lightning protection (lightning rods, air terminals, bonding). Ham antenna installations do not require an NFPA 780 system unless a complete structural LPS is being installed. The NEC Article 810 requirements are the applicable standard for antenna discharge units.

---

## CHAPTER 9 — BAND-BY-BAND QUICK REFERENCE

### 9.1 HF Bands (1.8–54 MHz)

| Band | Freq | GDT V_s (100W) | GDT V_s (1.5kW) | TVS for Multi-Stage | Connector |
|------|------|----------------|------------------|---------------------|-----------|
| 160M | 1.8 MHz | 150V | 600–1000V | SMBJ54CA (130pF ok) | SO-239 |
| 80M | 3.5 MHz | 150V | 600–1000V | SMBJ54CA | SO-239 |
| 40M | 7.0 MHz | 150V | 600–1000V | SMBJ54CA | SO-239 |
| 30M | 10.1 MHz | 150V | 600V | SMBJ54CA | SO-239 |
| 20M | 14.0 MHz | 150V | 600V | SMBJ54CA | SO-239 |
| 17M | 18.1 MHz | 150V | 600V | SMBJ54CA | SO-239 |
| 15M | 21.0 MHz | 150V | 600V | SMBJ54CA | SO-239 |
| 12M | 24.9 MHz | 150V | 600V | SMBJ54CA / SM6T39CA | SO-239 |
| 10M | 28.0 MHz | 150V | 600V | SM6T39CA (5pF) | SO-239 or N |
| 6M | 50.0 MHz | 150V | 600V | SM6T39CA (5pF) | N-type |

Notes:
- V_s values assume SWR ≤ 2:1. Higher SWR requires higher V_s — calculate.
- 100W column assumes matched-load V_peak = 100V with 1.5× safety margin.
- 10M and 6M should use N-type for better weatherproofing on permanent installs.

### 9.2 VHF Bands (144–222 MHz)

| Band | Freq | GDT V_s | TVS Part | TVS Cap | Inductor L1/L2 | Connector |
|------|------|---------|---------|---------|----------------|-----------|
| 2M | 144 MHz | 90–150V | SM6T39CA | 5 pF | 22 nH / 10 nH | N-type |
| 1.25M | 222 MHz | 90–150V | SM6T39CA | 5 pF | 22 nH / 10 nH | N-type |

Notes:
- At 144 MHz, standard SMBJ54CA (130 pF) causes 5.2 dB insertion loss — do not use.
- SM6T39CA (5 pF): IL = 0.019 dB at 144 MHz — acceptable.
- Single GDT without multi-stage is acceptable for low-power QRP VHF.

### 9.3 UHF Bands (420–450 MHz)

| Band | Freq | GDT V_s | TVS Part | TVS Cap | Inductor L1/L2 | Connector |
|------|------|---------|---------|---------|----------------|-----------|
| 70cm | 432 MHz | 90–150V | PSOT-36A | 1 pF | 10 nH / 4.7 nH | N-type |

Notes:
- At 432 MHz with L1=22nH, L2=10nH: total IL = 0.17 dB — exceeds spec.
- Reduce to L1=10nH, L2=4.7nH: total IL ≈ 0.07 dB — within spec.
- ESD9L5.0ST5G (0.35 pF) is an alternative TVS — lower capacitance but only 200A peak.

### 9.4 Microwave (902 MHz and Above)

| Band | Freq | GDT V_s | Approach | TVS Part | Connector |
|------|------|---------|---------|----------|-----------|
| 33cm | 902 MHz | 90V | GDT only | None | N-type or SMA |
| 23cm | 1296 MHz | 90V | GDT only (single) | None | N-type |

Notes:
- At 902–1296 MHz, multi-stage protection adds too much insertion loss.
- Single 1.0 pF GDT meets the 0.1 dB spec through 1296 MHz.
- Two GDTs in parallel (2 pF total) fails the spec at 1296 MHz — use single GDT only.
- For 1296 MHz: verify GDT capacitance ≤ 1.0 pF — some types are rated 1.5 pF.
- SMA connectors acceptable for indoor/shack equipment; use N-type for weatherproof outdoor installations.

### 9.5 Common Coax Types by Band

| Band | Preferred Coax | Alt. Coax | Notes |
|------|---------------|-----------|-------|
| 160–20M | RG-213/LMR-400 | RG-8X | Loss acceptable; use low-loss for > 100 feet |
| 17–10M | LMR-400 | RG-213 | LMR preferred; lower loss |
| 6M–2M | LMR-400 | LMR-240 | Minimize length |
| 70cm | LMR-400 | LMR-300 | LMR-195 for short jumpers only |
| 33cm–23cm | LMR-400 | Hardline (1/2") | Every 0.1 dB matters at microwave |

---

## CHAPTER 10 — INSTALLATION PROCEDURES

### 10.1 Pre-Installation Planning

Before installing any protection equipment:

1. **Identify all conductors entering the building.** Include: coax feedlines, rotor cables, control cables, ethernet, telephone, AC power. Every conductor is a path for surge energy.

2. **Select entry panel location.** The panel shall be as close as possible to the point where conductors enter the building and as close as possible to the planned ground electrode. Maximum distance from panel to ground rod: 1 meter (preferred), 3 meters (acceptable).

3. **Verify local code requirements.** Some municipalities require a licensed electrician to install or modify ground electrode systems. Contact local authority having jurisdiction (AHJ) before installation.

4. **Call 811.** Mandatory in the USA before any digging or rod driving. Wait 48 hours for utility marking.

5. **Document the design.** Draw a sketch showing panel location, ground rod location, rod-to-panel strap path, and equipment to be protected. File this with station documentation.

### 10.2 Ground Electrode Installation

**Materials required:**
- 10-foot copper-clad steel ground rod(s), 5/8" OD
- UL-listed two-bolt copper ground rod clamp(s)
- Slide hammer or SDS drill with rod driver
- #6 AWG solid copper wire or 1/2" copper strap
- Wire brush and anti-oxide compound (Noalox or equivalent)
- Bucket of water (if soil is dry)

**Procedure:**

Step 1: Mark all rod locations. Minimum 16 feet between rods (2× rod length) for multiple rods. Maximum 3 meters from entry panel for primary rod.

Step 2: Wet dry soil at each location. Saturate to at least rod depth.

Step 3: Attach rod driver to SDS drill or use slide hammer. Drive rod vertically at marked location.

Step 4: If rock is encountered before 8-foot depth: switch to horizontal installation. Dig an 18-inch deep trench at least 8 feet long. Lay the rod horizontal (or at a shallow angle). This is NEC-compliant when rock prevents full vertical installation.

Step 5: Leave 2" (50mm) above grade. Install UL-listed copper clamp. Clean rod with wire brush before installing clamp. Apply anti-oxide compound.

Step 6: If installing multiple rods, run #6 AWG solid copper between rod clamps at 18" depth. Keep the wire in the ground; do not run it on the surface.

Step 7: Run 1/2" copper strap or #6 AWG from primary rod clamp to entry panel bus. Shortest straight path. Avoid bends where possible.

### 10.3 Entry Panel Installation

**Materials required:**
- Copper or aluminum plate, 300 × 150 × 6mm minimum
- GDT arrestors with chassis mounting hardware
- Copper bus bar 1/2" × 1/4" or heavier
- M4 and M6 stainless hardware
- Weatherproof sealant (non-acidic RTV — not acetic-cure near copper)
- Coax feedthrough fittings or panel-mount connectors

**Procedure:**

Step 1: Cut or drill coax and cable entry holes. Fit panel-mount connectors or weathertight cable glands.

Step 2: Mount GDT arrestors. Each arrestor chassis must make full metal contact with the panel. Use copper or stainless steel washers between arrestor flange and panel. Torque hardware firmly.

Step 3: Install copper bus bar across the back face of the panel. Connect all arrestor chassis ground tabs to the bus bar with short (< 50mm) copper straps or lugs.

Step 4: Mount panel at entry location. Seal the conduit/penetration with fire-rated sealant. Seal around all connectors with non-acidic RTV.

Step 5: Connect panel bus to ground rod with 1/2" copper strap. Shortest straight path. No coils.

Step 6: Connect AC safety ground: run #6 AWG from panel bus to AC main panel ground bus (NEC 250.106 bond).

### 10.4 Coax Arrestor Installation

**For each coax feedline:**

Step 1: Install GDT arrestor at entry panel. Antenna side connects to the outside coax run. Shack side connects to the interior coax run.

Step 2: Apply RTV sealant around both connectors at the arrestor body. For N-type connectors, verify O-ring face seal is present and undamaged.

Step 3: Support the arrestor. It shall not hang freely from the coax. Use the 3D-printed mast clamp bracket, wall mount bracket, or tower bracket from mounting_brackets_lsp.scad.

Step 4: Ground strap from arrestor chassis to entry panel bus. ≤ 150mm length. 1/2" copper strap preferred; #10 AWG lug and braid acceptable if strap is impractical.

Step 5: Apply weatherproof wrap (Coax-Seal or self-amalgamating tape) over all connections if the installation is in direct rain/UV exposure.

### 10.5 Rotor and Control Cable Protection

For each multi-conductor control cable:

Step 1: Install TVS diode per conductor at entry panel. Use a multi-pin terminal strip with TVS across each pin.

Step 2: Select TVS voltage rating per control line type:
- DC logic (5–12V): SMBJ12CA or SMBJ15CA
- DC supply (12–24V): SMBJ30CA
- AC rotor (24V AC): SMBJ27CA (bidirectional, must handle full AC cycle)

Step 3: All TVS cathodes (chassis ground side) bond to entry panel bus. Short leads (< 25mm).

Step 4: Test control functions after installation. Measure continuity through each conductor. Verify TVS diodes are not conducting at normal operating voltage (measure voltage drop — should be near zero for DC signals).

---

## CHAPTER 11 — TESTING AND MEASUREMENT

### 11.1 Required Test Equipment

| Instrument | Minimum Spec | Purpose |
|------------|-------------|---------|
| NanoVNA or equivalent | 50 kHz–1.5 GHz | Insertion loss, VSWR verification |
| DMM | AC/DC voltage, resistance | Continuity, ground resistance check |
| Ground resistance tester | Fall-of-potential method | Earth electrode resistance (optional; borrow or hire) |
| RF power meter (SWR meter) | Rated for operating power | Pre/post IL comparison at operating power |

### 11.2 Insertion Loss Verification

After installing each arrestor, verify insertion loss has not exceeded specification.

**Procedure:**

Step 1: Connect NanoVNA as a 2-port measurement (PORT 1 to input, PORT 2 to output via calibration cable).

Step 2: Perform SOLT calibration at the connectors that will mate to the arrestor.

Step 3: Connect arrestor. Measure S21 (forward gain/loss) across the operating frequency range.

Step 4: Insertion loss = |S21| in dB. Verify:
- HF (1.8–28 MHz): IL < 0.01 dB (typical result; spec is < 0.1 dB)
- VHF (144–432 MHz): IL < 0.1 dB
- 23cm (1296 MHz): IL < 0.1 dB

Step 5: Record measurements in station logbook. Document date, instrument used, and results.

**Acceptance criteria:** Any arrestor with IL > 0.1 dB at any operating frequency shall be investigated before placing in service. Common causes: incorrect GDT type (high capacitance), failed solder joint, wrong TVS diode part number.

### 11.3 VSWR Verification

**Procedure:**

Step 1: Measure S11 (return loss) in dB at all operating frequencies.

Step 2: Convert return loss to VSWR: VSWR = (1 + 10^(-RL/20)) / (1 - 10^(-RL/20))

Step 3: Verify VSWR < 1.1:1 at all operating frequencies.

**Expected results (1.0 pF GDT):**
- Below 432 MHz: VSWR < 1.03:1 (return loss > 40 dB)
- At 1296 MHz: VSWR ≈ 1.09:1 (return loss ≈ 27 dB)

### 11.4 Ground Resistance Test

**Simplified DMM test (see Section 8.3):**

Step 1: Set DMM to AC voltage range, 20V full scale.

Step 2: One lead to ground rod clamp (or entry panel ground bus).

Step 3: Other lead to AC safety neutral at main panel.

Step 4: Read voltage.
- < 2V: satisfactory
- 2–5V: marginal; investigate
- > 5V: problem; do not rely on this ground system

**Note:** This test verifies continuity and approximate resistance only. A proper fall-of-potential test with a dedicated instrument is required to verify ≤ 25Ω earth resistance per NEC 250.56.

### 11.5 Static Bleed Verification (DC-Grounded Design)

Step 1: Disconnect coax from radio equipment.

Step 2: Using DMM set to resistance: measure from antenna-side coax center conductor to coax outer conductor/shield.

Step 3: Expected reading: RFC DC resistance, typically 5–30Ω. Reading should be stable and repeatable.

Step 4: A reading of infinity (OL) indicates RFC is open or GDT is not connected to ground. Investigate.

Step 5: A reading near 0Ω indicates GDT is shorted (failed). Replace GDT.

### 11.6 Post-Lightning Event Inspection

After any direct or nearby lightning strike:

1. **Do not operate the station** until inspection is complete.
2. Visually inspect all arrestor enclosures for scorching, cracking, or melted components.
3. Check all coax connections for melting or arcing marks.
4. Measure DC resistance of each GDT (should read open on a DMM). A shorted GDT passes DC current.
5. Verify ground rod clamps are intact and not corroded open.
6. If a GDT has fired, replace it even if it appears intact (sparkover event may have degraded it).
7. Document the event: date, observed strike proximity, which devices fired.

---

## CHAPTER 12 — TROUBLESHOOTING

### 12.1 High VSWR or Increased SWR After Arrestor Installation

| Symptom | Cause | Solution |
|---------|-------|---------|
| SWR increased on all bands | Connector not mating properly | Inspect connectors; clean mating faces |
| SWR high above 30 MHz | TVS diode with wrong capacitance | Verify part number; use low-C type |
| SWR frequency-dependent | GDT self-resonance | Verify GDT C; check PCB layout |
| SWR varies with humidity | Moisture ingress | Reseal; check gasket and RTV |

### 12.2 Transmit Power Lost After Lightning

| Symptom | Cause | Solution |
|---------|-------|---------|
| No forward power | GDT shorted; shorts coax center to chassis | Measure DC resistance; replace GDT |
| Very low power | TVS diode shorted | Remove multi-stage PCB; verify without it |
| Radio sees high reflected power | Connector damage from surge | Inspect all connectors in path |

### 12.3 Receiver Desensitization (Noise Floor Raised)

| Symptom | Cause | Solution |
|---------|-------|---------|
| All bands noisy | GDT not properly grounded | Measure chassis-to-rod continuity |
| Noise on one feedline only | Arrestor not bonded to panel | Check GDT chassis bond to bus bar |
| Noise correlated with power line | AC safety ground bond missing | Install NEC 250.106 bond |
| Noise from control cables | TVS not installed on control lines | Add TVS per conductor at entry panel |

### 12.4 Radio Damage Despite Arrestor

| Symptom | Cause | Solution |
|---------|-------|---------|
| Radio damaged; GDT intact | GDT V_s too high for power level | Recalculate V_s; use correct value |
| Radio damaged; GDT fired | Surge exceeded GDT capacity | Consider multi-stage design |
| Radio damaged; no strike | Static discharge | Add DC-grounded (RFC+GDT) design |
| Radio damaged; arrestor bypassed | Coax connected directly (bypassing arrestor) | Verify no bypass paths exist |

### 12.5 GDT Fires During Normal Transmit

This is the most critical installation error — the GDT sparkover voltage is too low for the RF peak voltage on the line.

**Symptoms:** Transmitter reports high SWR or reduced power; arc marks on GDT; burned smell; may damage transmitter final stage.

**Immediate action:** Disconnect antenna. Do not transmit until corrected.

**Diagnosis:** Calculate V_peak = √(2 × P × Z₀ × SWR). Compare to installed GDT V_s. If V_peak > V_s, the selection was incorrect.

**Solution:** Replace GDT with correct V_s per Section 4.1.1. Document the calculation.

---

## APPENDIX A — PARTS LIST MASTER

### GDT Arrestor Components

| Reference | Qty | Part Number | Description | Source |
|-----------|-----|-------------|-------------|--------|
| GDT-90 | as req | EPCOS B88069X1140B232 | 90V, 2.5kA, 1.0pF GDT | Mouser |
| GDT-90 alt | as req | Bourns 2040-090TR | 90V, 2.5kA, axial | Mouser |
| GDT-150 | as req | EPCOS B88069X3140B232 | 150V, 2.5kA, 1.0pF | Mouser |
| GDT-350 | as req | EPCOS B88069X5140B502 | 350V, 5.0kA, 1.0pF | Mouser |
| GDT-600 | as req | EPCOS B88069X8140B902 | 600V, 10kA, 1.0pF | Mouser |
| J-SO239 | as req | Amphenol SO-239 | Panel mount N-female equiv | Amphenol |
| J-N | as req | Amphenol 82-836 | N-female panel, IP67 | Amphenol |

### Multi-Stage Protection Components

| Reference | Qty | Part Number | Description | Source |
|-----------|-----|-------------|-------------|--------|
| MOV1 | 1/stage | Littelfuse V150LA10BP | 150V MOV, 1kA | Mouser |
| TVS-HF | 1/stage | SMBJ54CA | 54V, 500A TVS (HF only) | Mouser |
| TVS-6M | 1/stage | SM6T39CA | 39V, 5pF TVS (HF+2M) | Mouser |
| TVS-70cm | 1/stage | PSOT-36A | 36V, 1pF TVS (to 1GHz) | Mouser |
| TVS-UHF | 1/stage | ESD9L5.0ST5G | 5V, 0.35pF TVS (to 1.5GHz) | Mouser |
| L1-HF | 1/stage | Coilcraft 0805CS-R10XJLB | 100nH, 0805 chip | Coilcraft |
| L2-HF | 1/stage | Coilcraft 0805CS-47NXJLB | 47nH, 0805 chip | Coilcraft |
| L1-VHF | 1/stage | Coilcraft 0805CS-22NXJLB | 22nH, 0805 chip | Coilcraft |
| L2-VHF | 1/stage | Coilcraft 0805CS-10NXJLB | 10nH, 0805 chip | Coilcraft |

### Grounding System Components

| Reference | Qty | Part Number | Description | Source |
|-----------|-----|-------------|-------------|--------|
| ROD | 1–4 | Standard | 10-ft copper-clad steel, 5/8" OD | Electrical supply |
| CLAMP | 1/rod | UL listed | Two-bolt copper ground rod clamp | Electrical supply |
| BSTR-2IN | 3m | Custom cut | 2" × 0.050" copper strip | Metal supplier |
| BSTR-1IN | 2m | Custom cut | 1" × 0.050" copper strip | Metal supplier |
| BRAID-3/4 | 2m | Standard | 3/4" tinned copper braid | eBay, RF Parts |
| LUG-6AWG | 10 | Standard | #6 AWG two-hole copper lug | Electrical supply |
| PANEL | 1 | Custom | 300×150×6mm aluminum plate | Metal supplier |
| SCRW-SS | 20 | M4/M6 | SS machine screws + lock washers | Hardware |

### Control Line Protection

| Reference | Qty | Part Number | Description | Source |
|-----------|-----|-------------|-------------|--------|
| TVS-15V | 10 | SMBJ15CA | 15V bidirectional, per DC logic line | Mouser |
| TVS-27V | 4 | SMBJ27CA | 27V bidirectional, per AC rotor line | Mouser |
| TVS-30V | 4 | SMBJ30CA | 30V bidirectional, per DC motor line | Mouser |

---

## APPENDIX B — NEC/NFPA 780 CODE SUMMARY

### NEC Article 810 — Radio and Television Equipment

**Section 810.20 — Receiving Equipment; Antenna Discharge Units:**
Discharge units (GDT arrestors) required on outdoor antennas connecting to receivers. Unit shall be listed, installed in accessible location, and connected to a grounding electrode.

**Section 810.21 — Antenna Discharge Units:**
- Ground conductor size: #10 AWG copper minimum
- Routing: not attached to building surface; most direct path; no sharp bends
- Connection: to electrode per 250.52 through 250.53, or to building structural steel

**Section 810.52 — Transmitting Equipment; Antenna Discharge Units:**
Same requirements as 810.20/21, but additionally the ground system shall be bonded to the AC power grounding system.

### NEC Article 250 — Grounding and Bonding

**Section 250.52 — Grounding Electrode Types:**
- Ground rods: 8-foot minimum, 5/8" OD minimum copper-clad steel
- Concrete-encased electrodes (Ufer grounds)
- Ground plates, ground rings

**Section 250.53 — Grounding Electrode Installation:**
If a single ground rod measures > 25Ω, an additional electrode must be installed.

**Section 250.56 — Resistance of Rod Electrodes:**
If the single electrode has a resistance to ground exceeding 25Ω, a supplemental electrode is required.

**Section 250.106 — Lightning Protection Systems:**
The lightning protection system grounding conductor shall be bonded to the power system grounding electrode system. This bond must exist — you cannot have the antenna system and AC system at different potentials.

### NFPA 780 — Lightning Protection (Summary)

NFPA 780 covers structural lightning protection (air terminals, down conductors, ground terminals) for buildings and structures. It is not specifically the standard for ham antenna discharge units (that is NEC 810), but portions are applicable:

- Ground electrode resistance: ≤ 25Ω at 60 Hz (same as NEC)
- Ground ring: recommended for large structures; 2/0 AWG bare copper at 18" depth encircling structure
- Bonding: all metallic systems (gas pipe, water pipe, AC service, antenna system) bonded together

For ham radio purposes: NEC 810 is the primary applicable standard. Consult NFPA 780 only if installing a full structural lightning protection system.

---

## APPENDIX C — INSERTION LOSS AND VSWR CALCULATIONS

### C.1 Shunt Capacitance in 50Ω Line

A shunt capacitor C across a 50Ω transmission line:

```
Equivalent circuit: Source (25Ω) → shunt C → Load (25Ω)
(The 50Ω line splits into two 25Ω halves for shunt element analysis)

f₋₃dB = 1 / (2π × Z₀/2 × C) = 1 / (2π × 25 × C)

IL (dB) ≈ 10 × log₁₀(1 + (f/f₋₃dB)²)    [for f << f₋₃dB]

Simplified for small IL:
IL (dB) ≈ 8.686 × (f/f₋₃dB)²/2 = 4.343 × (f/f₋₃dB)²

Example: C = 5 pF (SM6T39CA TVS)
  f₋₃dB = 1 / (2π × 25 × 5×10⁻¹²) = 1.27 GHz
  IL at 144 MHz = 4.343 × (144/1270)² = 0.056 dB   ← well within spec
  IL at 432 MHz = 4.343 × (432/1270)² = 0.50 dB    ← too high; use PSOT-36A instead
```

### C.2 Series Inductance in 50Ω Line

A series inductor L in a 50Ω transmission line:

```
IL (dB) = 20 × log₁₀(1 + X_L/(2 × Z₀))
         = 20 × log₁₀(1 + 2πfL/100)

Example: L = 22 nH at 144 MHz:
  X_L = 2π × 144×10⁶ × 22×10⁻⁹ = 19.9Ω
  IL = 20 × log₁₀(1 + 19.9/100) = 20 × log₁₀(1.199) = 1.58 dB  ← excessive!

Wait — this is wrong. The correct formula for a series L in a matched 50Ω system:

  Γ = Z_L / (Z_L + 2Z₀) where Z_L = jωL
  
  For small IL: IL ≈ 20 × log₁₀(|1 + jωL/(2Z₀)|)
             = 10 × log₁₀(1 + (ωL/2Z₀)²)
             = 10 × log₁₀(1 + (πfL/Z₀)²)

  L = 22 nH at 144 MHz:
  IL = 10 × log₁₀(1 + (π × 144×10⁶ × 22×10⁻⁹ / 50)²)
     = 10 × log₁₀(1 + (0.199)²)
     = 10 × log₁₀(1.040) = 0.17 dB

  Combined L1=22nH + L2=10nH = 32nH at 144 MHz:
  IL = 10 × log₁₀(1 + (π × 144×10⁶ × 32×10⁻⁹ / 50)²)
     = 10 × log₁₀(1 + (0.29)²)
     = 0.35 dB
```

This is why the VHF multi-stage design uses small inductor values. Each nanohenry matters at 144 MHz and above.

### C.3 Sparkover Voltage Derivation

```
RF peak voltage at feedpoint:
  V_peak = √(2 × P × Z₀)    [matched load]

Under standing waves (SWR), maximum voltage on the line:
  V_max = V_peak × √SWR     [voltage maximum of standing wave]

GDT must not fire at V_max:
  V_s > V_max × safety_margin
  V_s > √(2 × P × Z₀ × SWR) × 1.5

Example: P=500W, Z₀=50Ω, SWR=2.5:1
  V_peak_matched = √(2 × 500 × 50) = 224V
  V_max = 224 × √2.5 = 354V
  Required V_s > 354 × 1.5 = 531V → Use 600V GDT
```

---

## APPENDIX D — SCHEMATIC FILE INDEX

All schematics are ASCII text files located in:
`/home/merv/Public/claude/ant_gear/lightning_protection/schematics/`

| File | Content |
|------|---------|
| `sch_gdt_arrestor.txt` | GDT theory; HF SO-239 design; VHF N-type design; dual GDT; sparkover voltage selection; insertion loss table; parts list |
| `sch_multistage_protection.txt` | 3-stage cascade theory; HF version (GDT+MOV+TVS); VHF version; clamping sequence analysis; insertion loss tables for both versions; TVS selection table |
| `sch_dc_grounded_and_blocked.txt` | RFC static bleed design; insertion loss vs. RFC value; DC-blocked topology analysis (why split-C fails); correct two-device approach; bias-T specifications; parts list |
| `sch_grounding_system.txt` | Entry panel single-point design; bonding strap impedance comparison; skin depth formula; ground rod NEC requirements; in-shack star topology; rotor cable TVS protection; parts list |

OpenSCAD files located in:
`/home/merv/Public/claude/ant_gear/lightning_protection/openscad/`

| File | Content |
|------|---------|
| `mounting_brackets_lsp.scad` | 10 modules: mast clamp, wall mount, tower leg bracket, entry panel mount, ground strap guide, rod clamp cover, arrestor enclosure, coax strain relief, cable bundle clamp, enclosure lid |

---

```
END OF MANUAL

TM-LSP-001  Rev A
Lightning Arrestors and Surge Protection for Amateur Radio Antenna Systems
Classification: Unclassified / Amateur Radio Educational Material
Distribution: Unrestricted

Author: Station Documentation System
Date: April 2026
```
