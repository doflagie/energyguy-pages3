#!/usr/bin/env python3
"""
generate_ant_tms.py — Navy TM-format HTML generator for 65 antenna directories.
Produces tm.html in each antenna subdirectory and adds a "View Technical Manual"
link to each index.html.

Mervyn Martin, KO6NNH — 26 May 2026
"""

import os
import csv
import re
from pathlib import Path

BASE_DIR = Path(__file__).parent  # /…/page3/antennas/

# ─────────────────────────────────────────────────────────────────────────────
# ANTENNA DATA — per-antenna parameters used by content generators
# Keys: tm_num, title, subtitle, bands, approach, special,
#       feed_z, swr_target, gain, efficiency, category
# ─────────────────────────────────────────────────────────────────────────────
ANTENNA_DATA = {
    "active_receiving": {
        "tm_num": "TM-ANT-009",
        "title": "ACTIVE RECEIVING ANTENNA",
        "subtitle": "Broadband 1.8–54 MHz Low-Noise Preamplified Antenna",
        "bands": "160M–6M (1.8–54 MHz)",
        "approach": "Small loop or whip with integrated low-noise amplifier (LNA), broadband 50 Ω output",
        "special": "LNA noise figure 0.8 dB, gain 20 dB typical, IP3 > +15 dBm, powered via bias-T",
        "feed_z": "50 Ω",
        "swr_target": "< 1.5:1",
        "gain": "LNA-dependent, 20 dB integrated",
        "efficiency": "80–95%",
        "category": "receiving",
    },
    "beverage_receiving": {
        "tm_num": "TM-ANT-010",
        "title": "BEVERAGE RECEIVING ANTENNA",
        "subtitle": "Terminated Long-Wire Low-Noise Directional Receiving Array, 160M–40M",
        "bands": "160M, 80M, 40M (1.8–7.3 MHz)",
        "approach": "Terminated long-wire broadside receiving array with ~600 Ω resistive terminator",
        "special": "120–240 m length, 600 Ω terminator at far end, 9:1 transformer at feed end, cardioid null pattern",
        "feed_z": "50 Ω (via 9:1 transformer)",
        "swr_target": "< 1.5:1",
        "gain": "~10 dBd forward, −20 dB back-to-front",
        "efficiency": "Receive only — noise figure is the key metric",
        "category": "receiving",
    },
    "cage_dipole": {
        "tm_num": "TM-ANT-011",
        "title": "CAGE DIPOLE ANTENNA",
        "subtitle": "Wide-Bandwidth Multi-Wire Cage-Construction Dipole, 80M–20M",
        "bands": "80M, 40M, 20M (3.5–14.35 MHz)",
        "approach": "Four to six parallel conductors per arm arranged in a cylindrical cage, producing broad bandwidth",
        "special": "4–6 parallel wires per arm, 30–50 cm cage diameter, inherent wide SWR bandwidth without tuner",
        "feed_z": "50 Ω",
        "swr_target": "< 1.5:1",
        "gain": "2.1 dBi broadside",
        "efficiency": "90–97%",
        "category": "dipole",
    },
    "collinear_array": {
        "tm_num": "TM-ANT-012",
        "title": "COLLINEAR ARRAY ANTENNA",
        "subtitle": "Stacked Phased Dipoles for VHF/UHF Gain, 2M and 70cm",
        "bands": "2M (144–148 MHz), 70cm (430–440 MHz)",
        "approach": "Stacked co-axial half-wave dipoles fed with in-phase phasing harness (coaxial stubs or phasing lines)",
        "special": "4-element 2M yields ~4 dBi gain; 8-element 70cm yields ~7 dBi gain, omnidirectional azimuth pattern",
        "feed_z": "50 Ω",
        "swr_target": "< 1.5:1",
        "gain": "4 dBi (4-el 2M), 7 dBi (8-el 70cm)",
        "efficiency": "85–95%",
        "category": "vertical",
    },
    "compact-dipole": {
        "tm_num": "TM-ANT-013",
        "title": "COMPACT TRAP DIPOLE",
        "subtitle": "Trap-Loaded Multiband Dipole, 160M–6M Portable",
        "bands": "160M–6M (multiband via traps)",
        "approach": "Center-fed dipole with LC trap pairs at element junctions to achieve multiband resonance at reduced physical length",
        "special": "Traps for each target band, loading coil in each arm for 160/80M sections, portable operation",
        "feed_z": "50 Ω",
        "swr_target": "< 2.0:1",
        "gain": "2 dBi per band",
        "efficiency": "75–90% per band",
        "category": "dipole",
    },
    "cross_polarized": {
        "tm_num": "TM-ANT-014",
        "title": "CROSS-POLARIZED ANTENNA",
        "subtitle": "Circular Polarization Array for Satellite/Weak-Signal, 2M/70cm",
        "bands": "2M (144–148 MHz), 70cm (430–440 MHz)",
        "approach": "Two crossed dipoles with 90° hybrid phasing network to produce circular polarization",
        "special": "90° phase shift via coaxial phasing harness, suitable for LEO satellite and meteor scatter work",
        "feed_z": "50 Ω",
        "swr_target": "< 1.5:1",
        "gain": "2.1 dBic (isotropic, circular)",
        "efficiency": "85–95%",
        "category": "special",
    },
    "cubical_quad": {
        "tm_num": "TM-ANT-015",
        "title": "CUBICAL QUAD ANTENNA",
        "subtitle": "Full-Wave Square Loop Directional Antenna, 40M–15M",
        "bands": "40M, 20M, 15M (7–21 MHz)",
        "approach": "Square full-wave loop driven element with square reflector loop, optional director loops for gain",
        "special": "Full-size quad, 2–3 elements, 10–15 dBi gain; feed impedance ~100 Ω, 2:1 balun at feedpoint",
        "feed_z": "100 Ω (2:1 balun to 50 Ω coax)",
        "swr_target": "< 1.5:1 after balun",
        "gain": "10–15 dBi (2–3 elements)",
        "efficiency": "90–97%",
        "category": "loop",
    },
    "delta_loop_40m": {
        "tm_num": "TM-ANT-016",
        "title": "DELTA LOOP ANTENNA — 40M",
        "subtitle": "Full-Wavelength Triangular Resonant Loop for 40M DX",
        "bands": "40M (7.0–7.3 MHz), harmonics on 20M, 15M",
        "approach": "Full-wavelength triangular loop with low radiation angle for DX propagation",
        "special": "130 ft perimeter, equilateral sides ~43.3 ft, 50 Ω feedpoint at base corner, low-angle DX pattern",
        "feed_z": "50 Ω",
        "swr_target": "< 2.0:1",
        "gain": "1.5–4 dBi (low angle)",
        "efficiency": "90–97%",
        "category": "loop",
    },
    "double_bazooka": {
        "tm_num": "TM-ANT-017",
        "title": "DOUBLE BAZOOKA DIPOLE",
        "subtitle": "Coaxial-Sleeve Wide-Bandwidth Dipole, 40M and 20M",
        "bands": "40M (7.0–7.3 MHz), 20M (14.0–14.35 MHz)",
        "approach": "Center-fed dipole with coaxial stub sections forming a sleeve balun and bandwidth-broadening structure",
        "special": "300 Ω twin-lead or coax sections per arm, inherent common-mode rejection, broader SWR than simple dipole",
        "feed_z": "50 Ω (via coax taper)",
        "swr_target": "< 1.5:1 across full band",
        "gain": "2.1 dBi broadside",
        "efficiency": "88–95%",
        "category": "dipole",
    },
    "elevated_radials": {
        "tm_num": "TM-ANT-018",
        "title": "ELEVATED RADIALS VERTICAL ANTENNA",
        "subtitle": "Ground-Independent Quarter-Wave Vertical with Elevated Counterpoise, 160M–40M",
        "bands": "160M, 80M, 40M (1.8–7.3 MHz)",
        "approach": "Quarter-wave vertical element above resonant elevated radials, eliminating dependence on lossy earth ground",
        "special": "8–16 radials at 1.5–2 m height, resonant radials at target frequency, base-fed 50 Ω",
        "feed_z": "50 Ω",
        "swr_target": "< 1.5:1",
        "gain": "0 dBi (omni, low angle)",
        "efficiency": "85–95% (superior to buried radial system with < 32 buried radials)",
        "category": "vertical",
    },
    "end_fed_half_wave": {
        "tm_num": "TM-ANT-019",
        "title": "END-FED HALF-WAVE (EFHW) ANTENNA",
        "subtitle": "Multiband End-Fed Half-Wave with 49:1 Matching Transformer, 160M–10M",
        "bands": "160M–10M (multiband on harmonics)",
        "approach": "86-ft wire resonant at 3.5 MHz; fed at high-impedance end via 49:1 unun transformer",
        "special": "86-ft wire, high-impedance end feed, 49:1 unun (or 9:1 unun with short counterpoise), harmonically resonant",
        "feed_z": "2500–5000 Ω at wire end → 50 Ω via transformer",
        "swr_target": "< 2.0:1 on harmonics",
        "gain": "2.1 dBi broadside",
        "efficiency": "80–93% (transformer loss ~ 0.5 dB)",
        "category": "endfed",
    },
    "end_fed_quarter_wave": {
        "tm_num": "TM-ANT-020",
        "title": "END-FED QUARTER-WAVE (EFQW) ANTENNA",
        "subtitle": "Single-Band End-Fed Quarter-Wave Vertical with Counterpoise, 80M–20M",
        "bands": "80M, 40M, 20M (separate antennas)",
        "approach": "Quarter-wave wire element fed at high-impedance end via 9:1 unun with short counterpoise",
        "special": "Length per band: 80M ≈ 65 ft, 40M ≈ 33 ft, 20M ≈ 16.5 ft; counterpoise 0.05λ minimum",
        "feed_z": "~1000–2000 Ω → 50 Ω via 9:1 unun",
        "swr_target": "< 2.0:1",
        "gain": "0 dBi (vertical, omnidirectional)",
        "efficiency": "80–92%",
        "category": "endfed",
    },
    "end_fed_random_wire": {
        "tm_num": "TM-ANT-021",
        "title": "END-FED RANDOM WIRE (EFRW) ANTENNA",
        "subtitle": "Broadband Non-Resonant Random-Length Wire with ATU, 160M–6M",
        "bands": "160M–6M (with ATU)",
        "approach": "Random-length wire (avoid multiples of λ/2 per band) fed via 9:1 or capacitive coupler into ATU",
        "special": "40–100 ft wire avoiding resonant lengths; recommended: 29, 35.5, 41, 58, 71 ft; ATU required",
        "feed_z": "Varies widely; 9:1 unun + ATU to 50 Ω",
        "swr_target": "< 2.0:1 after ATU",
        "gain": "2–4 dBi (length and frequency dependent)",
        "efficiency": "75–90% (ATU loss + wire resistance)",
        "category": "endfed",
    },
    "end_fed_zepp": {
        "tm_num": "TM-ANT-022",
        "title": "END-FED ZEPP ANTENNA",
        "subtitle": "Classic Zepp End-Fed Dipole with Open-Wire Feeder, 160M–10M",
        "bands": "160M, 80M, 40M, 20M, 15M, 10M",
        "approach": "Half-wave wire radiator fed at voltage-maximum (current-minimum) end via parallel open-wire feeder into balanced ATU",
        "special": "Open-wire feeder (300–450 Ω), high impedance at end, tuner required, excellent multiband performer",
        "feed_z": "50–600 Ω (frequency/length dependent via feeder)",
        "swr_target": "< 2.0:1 after ATU",
        "gain": "2.1–4 dBi (length dependent)",
        "efficiency": "88–96% (low-loss feeder)",
        "category": "endfed",
    },
    "extended_double_zepp": {
        "tm_num": "TM-ANT-023",
        "title": "EXTENDED DOUBLE ZEPP (EDZ) ANTENNA",
        "subtitle": "1.25-Wavelength Dipole with Open-Wire Feed for Enhanced Gain, 40M–10M",
        "bands": "40M, 20M, 15M, 10M",
        "approach": "Dipole with each arm extended to 0.625λ (1.25λ total), increasing broadside gain by ~2 dB over standard dipole",
        "special": "300 Ω twin-lead feeder, ~50 ft per arm at 40M, pattern narrows and gain increases to ~4 dBi",
        "feed_z": "~100–800 Ω (via feeder); ATU or 4:1 balun",
        "swr_target": "< 2.0:1 after matching",
        "gain": "4 dBi broadside",
        "efficiency": "88–95%",
        "category": "dipole",
    },
    "fan_dipole": {
        "tm_num": "TM-ANT-024",
        "title": "FAN DIPOLE ANTENNA",
        "subtitle": "Parallel Multiband Dipoles on Single Feedpoint, 160M–10M",
        "bands": "160M–10M (multiband, 5–7 band pairs)",
        "approach": "Multiple dipole pairs cut to separate bands, all connected at a common center feedpoint and flared outward",
        "special": "5–7 dipole pairs, 50 Ω parallel combined feed, each pair resonant at target band; small interaction between elements",
        "feed_z": "50 Ω",
        "swr_target": "< 1.5:1 per band",
        "gain": "2.1 dBi per band",
        "efficiency": "88–95% per band",
        "category": "dipole",
    },
    "flagpole": {
        "tm_num": "TM-ANT-025",
        "title": "FLAGPOLE ANTENNA",
        "subtitle": "Camouflaged HF/VHF Vertical Using Flagpole Mast Structure",
        "bands": "HF 40M–10M; VHF 2M/70cm (configuration dependent)",
        "approach": "End-fed vertical whip concealed within or as a flagpole structure; coil-loaded base matching network",
        "special": "5–10 ft whip; base loading coil for 40/80M; capacitive coupling via sleeve; aesthetically concealed",
        "feed_z": "50 Ω via base matching network",
        "swr_target": "< 2.0:1",
        "gain": "0 dBi (vertical, omnidirectional)",
        "efficiency": "70–88% (loading coil loss at HF)",
        "category": "vertical",
    },
    "flower_pot": {
        "tm_num": "TM-ANT-026",
        "title": "FLOWER POT ANTENNA",
        "subtitle": "Omnidirectional VHF/UHF Coaxial Dipole, 2M/70cm",
        "bands": "2M (144–148 MHz), 70cm (430–440 MHz)",
        "approach": "Coaxial dipole using the coax braid as the lower element and the center conductor (extended) as the upper element, using a sleeve/choke to decouple feed line",
        "special": "No external balun needed; 50 Ω direct feed; ~2.1 dBi gain; simple PVC housing",
        "feed_z": "50 Ω",
        "swr_target": "< 1.5:1",
        "gain": "2.1 dBi omnidirectional",
        "efficiency": "90–97%",
        "category": "vertical",
    },
    "folded_dipole": {
        "tm_num": "TM-ANT-027",
        "title": "FOLDED DIPOLE ANTENNA",
        "subtitle": "Two-Wire Folded Dipole with 300 Ω Feedpoint Impedance, 40M–10M",
        "bands": "40M, 20M, 10M (and harmonics)",
        "approach": "Standard folded dipole: two parallel conductors shorted at both ends, fed at center of one conductor",
        "special": "10–20 cm wire spacing; feed impedance 4× standard dipole = 300 Ω; wider bandwidth than simple dipole",
        "feed_z": "300 Ω (4:1 balun to 75 Ω, or 6:1 to 50 Ω)",
        "swr_target": "< 1.5:1 after matching",
        "gain": "2.1 dBi broadside",
        "efficiency": "92–97%",
        "category": "dipole",
    },
    "folded_dipole_2conductor": {
        "tm_num": "TM-ANT-028",
        "title": "FOLDED DIPOLE — 2-CONDUCTOR",
        "subtitle": "Twin Radiator Folded Dipole, 200 Ω Feed, 80M–10M",
        "bands": "80M–10M (multiband)",
        "approach": "Two-wire folded dipole with conductors of unequal diameter, producing impedance transformation ratio other than 4:1",
        "special": "Twin radiators, ~200 Ω nominal feedpoint, 4:1 impedance step-up typical; broadband SWR behavior",
        "feed_z": "200 Ω (4:1 balun to 50 Ω)",
        "swr_target": "< 2.0:1 after matching",
        "gain": "2.1 dBi",
        "efficiency": "90–96%",
        "category": "dipole",
    },
    "folded_dipole_3conductor": {
        "tm_num": "TM-ANT-029",
        "title": "FOLDED DIPOLE — 3-CONDUCTOR",
        "subtitle": "Triple Radiator Folded Dipole, 450 Ω Feed, 40M–10M",
        "bands": "40M–10M",
        "approach": "Three parallel conductors forming a triple folded dipole structure; feedpoint impedance = 9× standard dipole",
        "special": "Triple radiator, ~450 Ω nominal feedpoint, 9:1 unun to 50 Ω; narrow bandwidth; high efficiency due to radiation resistance",
        "feed_z": "450 Ω (9:1 balun to 50 Ω)",
        "swr_target": "< 2.0:1 after matching",
        "gain": "2.1 dBi",
        "efficiency": "90–96%",
        "category": "dipole",
    },
    "frame_loop": {
        "tm_num": "TM-ANT-030",
        "title": "FRAME LOOP RECEIVING/TRANSMITTING ANTENNA",
        "subtitle": "Large Square Tuned Loop for 40M–15M, NVIS and DX",
        "bands": "40M, 20M, 15M (7–21 MHz)",
        "approach": "Large square loop (perimeter > λ) tuned with series or parallel capacitor; high radiation resistance, low loss",
        "special": "10–20 ft sides, high Q tuned loop, series capacitor for resonance; broadside radiation; direction finding capable",
        "feed_z": "50–200 Ω (adjustable via feed tap position)",
        "swr_target": "< 2.0:1",
        "gain": "1.5–3 dBi",
        "efficiency": "85–95%",
        "category": "receiving",
    },
    "full_wave_loop": {
        "tm_num": "TM-ANT-031",
        "title": "FULL-WAVE LOOP ANTENNA",
        "subtitle": "Full-Wavelength Resonant Loop with Low Radiation Angle, 20M–10M",
        "bands": "20M, 15M, 10M (and harmonics)",
        "approach": "Full-wavelength closed loop (any shape: square, circle, triangle) fed at one point; inherently low radiation angle",
        "special": "~65 ft perimeter at 20M; 4–6 dBi gain on edge of loop at low elevation angle; excellent DX tool",
        "feed_z": "100–150 Ω (2:1 balun to 50 Ω)",
        "swr_target": "< 2.0:1 after matching",
        "gain": "4–6 dBi edge-fire",
        "efficiency": "90–97%",
        "category": "loop",
    },
    "g5rv": {
        "tm_num": "TM-ANT-032",
        "title": "G5RV MULTIBAND ANTENNA",
        "subtitle": "Classic 102-ft Wire with 34-ft Ladder-Line Matching Section, 160M–6M",
        "bands": "160M–6M (multiband with ATU)",
        "approach": "102-ft dipole with 34-ft open-wire feeder acting as impedance transformer into coax and ATU",
        "special": "102 ft main wire, 34 ft 300/450 Ω ladder-line section, ATU at transceiver end; design of L.B. Cebik / Louis Varney",
        "feed_z": "Variable per band via ladder-line (ATU required)",
        "swr_target": "< 2.0:1 after ATU",
        "gain": "2–6 dBi (band/elevation dependent)",
        "efficiency": "80–93%",
        "category": "dipole",
    },
    "gmrs_loaded_vertical": {
        "tm_num": "TM-ANT-033",
        "title": "GMRS LOADED VERTICAL ANTENNA",
        "subtitle": "Center-Loaded Dual-Frequency Vertical for 462/467 MHz GMRS",
        "bands": "462–462.725 MHz, 467–467.725 MHz (GMRS channels)",
        "approach": "Center-loaded vertical with dual-frequency LC traps for simultaneous GMRS main/sub-band coverage",
        "special": "Trap pair 462.5/467.5 MHz; 1.5 m radiator; 8 ground radials; base coax feed",
        "feed_z": "50 Ω",
        "swr_target": "< 1.5:1",
        "gain": "3–5 dBi omnidirectional",
        "efficiency": "85–93%",
        "category": "vertical",
    },
    "ground_plane": {
        "tm_num": "TM-ANT-034",
        "title": "GROUND PLANE ANTENNA",
        "subtitle": "Quarter-Wave Vertical with Elevated Radial Ground Plane, 2M/70cm/1.25M",
        "bands": "2M (144–148 MHz), 70cm (430–440 MHz), 1.25M (222–225 MHz)",
        "approach": "Quarter-wave vertical radiator above four ground-plane radials drooped 45° to raise feedpoint impedance to 50 Ω",
        "special": "4 radials at 90° spacing, drooped 45° below horizontal; 50 Ω direct feed; ~2 dBi gain",
        "feed_z": "50 Ω",
        "swr_target": "< 1.5:1",
        "gain": "2 dBi omnidirectional",
        "efficiency": "90–97%",
        "category": "vertical",
    },
    "helical_antenna_2m_satellite": {
        "tm_num": "TM-ANT-035",
        "title": "HELICAL ANTENNA — 2M SATELLITE",
        "subtitle": "Axial-Mode Helical for 144–146 MHz LEO Satellite, RHCP",
        "bands": "2M satellite (144–146 MHz)",
        "approach": "Solenoid helix operating in axial mode: multi-turn helical conductor producing circular polarization and high gain",
        "special": "3-turn helix, 4.5 in diameter, 4.5 in pitch, ~8 dBic gain, RHCP, 50 Ω direct feed via microstrip transformer",
        "feed_z": "50 Ω (via λ/4 matching strip)",
        "swr_target": "< 2.0:1",
        "gain": "8 dBic (circular polarization referenced)",
        "efficiency": "85–95%",
        "category": "special",
    },
    "hex_loop": {
        "tm_num": "TM-ANT-036",
        "title": "HEX LOOP ANTENNA",
        "subtitle": "Six-Sided Horizontally Polarized Loop, 20M–15M",
        "bands": "20M (14.0–14.35 MHz), 15M (21.0–21.45 MHz)",
        "approach": "Six-sided (hexagonal) full-wave loop fed at one side corner; horizontal polarization in broadside direction",
        "special": "~30 ft perimeter at 20M, supports broadside directivity, smoother current distribution than square loop",
        "feed_z": "100–150 Ω (2:1 balun to 50 Ω)",
        "swr_target": "< 2.0:1 after matching",
        "gain": "2–4 dBi broadside",
        "efficiency": "88–96%",
        "category": "loop",
    },
    "inverted_l_40m": {
        "tm_num": "TM-ANT-037",
        "title": "INVERTED-L ANTENNA — 40M",
        "subtitle": "Loaded Vertical with Top-Loading Horizontal Section, 40M Multiband",
        "bands": "40M (7.0–7.3 MHz), 80M with tuner, 20M harmonic",
        "approach": "L-shaped wire with vertical section for radiation and horizontal section providing capacitive top-loading",
        "special": "35 ft vertical + 15 ft horizontal; loading coil optional at junction; 50 Ω feedpoint; broadband RX trap optional",
        "feed_z": "50 Ω",
        "swr_target": "< 2.0:1",
        "gain": "0 dBi (vertical-dominant pattern)",
        "efficiency": "80–92%",
        "category": "vertical",
    },
    "inverted_vee_dipole": {
        "tm_num": "TM-ANT-038",
        "title": "INVERTED-VEE DIPOLE ANTENNA",
        "subtitle": "Center-Peak Dipole with Sloped Arms, 80M–20M",
        "bands": "80M, 40M, 20M (7–14 MHz range)",
        "approach": "Standard half-wave dipole with arms sloped downward from center apex at 45–60° included angle, reducing height requirements",
        "special": "45–60° apex angle; lower feed impedance vs. horizontal dipole (~50 Ω at 120° included angle); increased vertical component",
        "feed_z": "50 Ω (at 120° included angle) to 35 Ω (at 90°)",
        "swr_target": "< 2.0:1",
        "gain": "1.5–2 dBi (omnidirectional skewed pattern)",
        "efficiency": "88–96%",
        "category": "dipole",
    },
    "j_pole": {
        "tm_num": "TM-ANT-039",
        "title": "J-POLE ANTENNA",
        "subtitle": "End-Fed J-Monopole with Integral λ/4 Matching Stub, 2M/70cm",
        "bands": "2M (144–148 MHz), 70cm (430–440 MHz)",
        "approach": "End-fed half-wave radiator with quarter-wave parallel-line matching stub providing 50 Ω match at the feedpoint",
        "special": "Quarter-wave stub of 300 Ω ladder line or twin-lead; tap point on stub for 50 Ω; omnidirectional pattern",
        "feed_z": "50 Ω (tapped on matching stub)",
        "swr_target": "< 1.5:1",
        "gain": "3.5 dBi (end-fire vertical)",
        "efficiency": "90–97%",
        "category": "vertical",
    },
    "k9ay_loop_40m": {
        "tm_num": "TM-ANT-040",
        "title": "K9AY LOOP ANTENNA — 40M",
        "subtitle": "Directional Terminated Receiving Loop for Null Steering, 40M",
        "bands": "40M (7.0–7.3 MHz), useful 80M with retuning",
        "approach": "Tuned receiving loop with resistive termination at one point to produce cardioid directional pattern; bidirectional via switching",
        "special": "20 ft square loop, 1000 pF air-variable cap, 560–1000 Ω termination resistor, toroidal step-up transformer; front-to-back >20 dB",
        "feed_z": "50 Ω (via impedance step-up transformer)",
        "swr_target": "< 2.0:1",
        "gain": "−10 to −15 dBd (receive only; rejection is the metric)",
        "efficiency": "Receive only — noise figure and directivity are key metrics",
        "category": "receiving",
    },
    "kmr_400_magnetic_loop": {
        "tm_num": "TM-ANT-041",
        "title": "KMR-400 COMPACT MAGNETIC LOOP ANTENNA",
        "subtitle": "Switchable Multi-Band VHF/UHF Magnetic Loop, 2M–20cm",
        "bands": "2M (144 MHz), 1.25M (222 MHz), 70cm (432 MHz), 33cm (902 MHz), 20cm (1296 MHz)",
        "approach": "Small-diameter magnetic loop with tap-switched inductance per band; high-Q resonant loop",
        "special": "Loop diameter 25 cm, tap-switched per band, high Q per band; vacuum or silver-mica capacitor for VHF/UHF",
        "feed_z": "50 Ω",
        "swr_target": "< 1.5:1",
        "gain": "2–5 dBi per band",
        "efficiency": "80–93% per band",
        "category": "loop",
    },
    "loaded_vertical_hf": {
        "tm_num": "TM-ANT-042",
        "title": "LOADED VERTICAL HF ANTENNA",
        "subtitle": "Quarter-Wave Vertical with Center or Top Loading Coil, 160M–40M",
        "bands": "160M, 80M, 40M (1.8–7.3 MHz)",
        "approach": "Short vertical radiator with series loading coil at center or top to restore electrical length to λ/4",
        "special": "Loading coil at 2/3 height (optimal for radiation resistance); 8–16 buried radials or 4–8 elevated; tapped coil for band change",
        "feed_z": "50 Ω (base feed)",
        "swr_target": "< 2.0:1",
        "gain": "0 dBi (omnidirectional)",
        "efficiency": "60–85% (loading coil loss dominant)",
        "category": "vertical",
    },
    "log_periodic": {
        "tm_num": "TM-ANT-043",
        "title": "LOG PERIODIC ANTENNA — VHF",
        "subtitle": "Compact LPDA for 6M–2M Wideband Coverage",
        "bands": "6M (50–54 MHz) through 2M (144–148 MHz)",
        "approach": "Log periodic dipole array with boom-fed transposed element feed; frequency-independent operation",
        "special": "8–12 elements, 1.5–2 m boom, directional pattern, forward gain 5–8 dBi, SWR < 2:1 across full range",
        "feed_z": "50 Ω (balanced boom feed, common-mode choke recommended)",
        "swr_target": "< 2.0:1 across 50–150 MHz",
        "gain": "5–8 dBi directional",
        "efficiency": "88–95%",
        "category": "yagi",
    },
    "log_periodic_dipole_arrays_for_wideband_operation": {
        "tm_num": "TM-ANT-044",
        "title": "LOG PERIODIC DIPOLE ARRAY (LPDA) — WIDEBAND",
        "subtitle": "Frequency-Independent LPDA, 2M–33cm Continuous Coverage",
        "bands": "2M through 33cm (144 MHz–902 MHz) continuous",
        "approach": "Multi-element LPDA designed with log-periodic taper ratio τ ≈ 0.9 and spacing factor σ ≈ 0.05 for maximum gain/bandwidth",
        "special": "12–20 elements, 50 Ω balanced feed, 8–12 dBi forward gain, back lobe suppression > 20 dB",
        "feed_z": "50 Ω",
        "swr_target": "< 2.0:1 across band",
        "gain": "8–12 dBi",
        "efficiency": "88–95%",
        "category": "yagi",
    },
    "magnetic_loop_antennas": {
        "tm_num": "TM-ANT-045",
        "title": "MAGNETIC LOOP ANTENNA",
        "subtitle": "High-Q Manually Tuned Magnetic Loop for HF, 40M–10M",
        "bands": "40M–10M (7–30 MHz)",
        "approach": "Small-circumference high-Q resonant loop tuned with a large vacuum variable capacitor; small radiation resistance compensated by high Q",
        "special": "Loop dia 6–10 ft; vacuum variable capacitor REQUIRED (4–12 kV at 100W); null-null tuning; loop circumference 0.1–0.25λ",
        "feed_z": "50 Ω (small coupling loop ~0.2× main loop diameter, or gamma match)",
        "swr_target": "< 2.0:1",
        "gain": "2–4 dBi (at height above 0.1λ)",
        "efficiency": "75–95% (depends on Q; copper best)",
        "category": "magloop",
    },
    "moxon": {
        "tm_num": "TM-ANT-046",
        "title": "MOXON RECTANGLE ANTENNA",
        "subtitle": "Compact Two-Element Directional Beam, 20M–10M",
        "bands": "20M, 15M, 10M (14–29 MHz)",
        "approach": "Modified cubical quad with folded tips to form a compact rectangular two-element beam; one driven, one reflector",
        "special": "Compact footprint ~40% of Yagi at same gain; 7–10 dBi forward gain; 35–40 dB front-to-back ratio; single feedpoint",
        "feed_z": "50 Ω direct feed (driven element naturally 50 Ω resonant)",
        "swr_target": "< 1.5:1",
        "gain": "7–10 dBi (F/B > 35 dB)",
        "efficiency": "90–97%",
        "category": "yagi",
    },
    "near_vertical_incidence_skywave": {
        "tm_num": "TM-ANT-047",
        "title": "NEAR VERTICAL INCIDENCE SKYWAVE (NVIS) ANTENNA",
        "subtitle": "Low-Height Dipole or Inverted-V for Regional 0–500 km Propagation, 160M–40M",
        "bands": "160M, 80M, 40M (1.8–7.3 MHz)",
        "approach": "Horizontally polarized dipole or inverted-V at low height (0.05–0.15λ above ground) producing near-vertical radiation angle",
        "special": "Steep radiation angle (70–90°), near-vertical takeoff; height 15–35 ft at 40M; excellent for EmComm and regional nets",
        "feed_z": "50–75 Ω (height-dependent)",
        "swr_target": "< 2.0:1",
        "gain": "0–3 dBi at high angle",
        "efficiency": "70–88% (ground reflection loss)",
        "category": "dipole",
    },
    "ocfd_windom": {
        "tm_num": "TM-ANT-048",
        "title": "OCFD / WINDOM ANTENNA",
        "subtitle": "Off-Center Fed Dipole with 1/3-Point Feed and Open-Wire Feeder, 160M–6M",
        "bands": "160M–6M (multiband)",
        "approach": "102-ft wire dipole fed at 1/3 point (34 ft from one end) via parallel-wire feeder into balanced tuner",
        "special": "102 ft wire, fed at 34 ft point; open-wire feeder; harmonically resonant on multiple bands simultaneously; ATU required",
        "feed_z": "Variable per band; 200–600 Ω via feeder; ATU to 50 Ω",
        "swr_target": "< 2.0:1 after ATU",
        "gain": "2–4 dBi per band",
        "efficiency": "82–94% (feeder loss minimal)",
        "category": "dipole",
    },
    "octal_loop": {
        "tm_num": "TM-ANT-049",
        "title": "OCTAL LOOP ANTENNA",
        "subtitle": "Eight-Sided Low-Angle Resonant Loop, 10M–6M",
        "bands": "10M (28–29.7 MHz), 6M (50–54 MHz)",
        "approach": "Eight-sided (octagonal) full-wave loop producing lower current variation than square loop; low radiation angle",
        "special": "~50 ft perimeter at 10M; ~3 dBi gain at low elevation; smooth current distribution across all eight sides",
        "feed_z": "100–150 Ω (2:1 balun to 50 Ω)",
        "swr_target": "< 2.0:1 after matching",
        "gain": "3 dBi edge-fire",
        "efficiency": "90–97%",
        "category": "loop",
    },
    "off_center_fed_dipole": {
        "tm_num": "TM-ANT-050",
        "title": "OFF-CENTER FED DIPOLE (OCFD)",
        "subtitle": "Asymmetric Dipole Fed at 1/3 or 1/5 Point with 4:1 Balun, 40M–10M",
        "bands": "40M, 20M, 10M (and harmonics)",
        "approach": "Dipole fed off-center to present a workable impedance to a 4:1 balun plus 50 Ω coax; multiband without tuner",
        "special": "Feed at 1/3 point (~44:133 ft for 40M); 4:1 or 6:1 current balun; no tuner on design frequencies",
        "feed_z": "150–200 Ω at feed point → 50 Ω via 4:1 balun",
        "swr_target": "< 2.0:1 on design bands",
        "gain": "2.1 dBi broadside",
        "efficiency": "85–94%",
        "category": "dipole",
    },
    "patch_antenna_2.4_ghz": {
        "tm_num": "TM-ANT-051",
        "title": "PATCH ANTENNA — 2.4 GHz",
        "subtitle": "Microstrip Rectangular Patch on FR4, 2.4 GHz ISM Band",
        "bands": "2.4 GHz ISM (2.400–2.500 GHz)",
        "approach": "Rectangular conducting patch on FR4 dielectric substrate, resonant at patch length ~λ/2 in substrate; microstrip feed",
        "special": "~50 mm × 65 mm patch on FR4 (εr = 4.4) h = 1.6 mm; 50 Ω coaxial probe or edge feed; linear polarization",
        "feed_z": "50 Ω (probe feed at 50 Ω inset)",
        "swr_target": "< 2.0:1",
        "gain": "5–7 dBi broadside",
        "efficiency": "70–85% (FR4 dielectric loss)",
        "category": "special",
    },
    "phased_array_of_2": {
        "tm_num": "TM-ANT-052",
        "title": "PHASED ARRAY — 2-ELEMENT",
        "subtitle": "Side-by-Side Dipoles with 90° Phasing Harness, 2M/70cm",
        "bands": "2M (144–148 MHz), 70cm (430–440 MHz)",
        "approach": "Two co-planar dipoles spaced 0.5λ apart with phasing harness providing 90° current phase shift between elements",
        "special": "0.5λ spacing between elements; phasing harness from matched coax lengths; ~3–4 dBi gain, cardioid pattern",
        "feed_z": "50 Ω (phasing harness output)",
        "swr_target": "< 1.5:1",
        "gain": "3–4 dBi",
        "efficiency": "85–95%",
        "category": "special",
    },
    "phased_array_of_4": {
        "tm_num": "TM-ANT-053",
        "title": "PHASED ARRAY — 4-ELEMENT",
        "subtitle": "Square Quad Phased Array with Switchable Pattern, 2M/70cm",
        "bands": "2M (144–148 MHz), 70cm (430–440 MHz)",
        "approach": "Four dipoles in square arrangement with 1λ spacing; phase steering via switched coaxial harness for directional patterns",
        "special": "Square 1λ spacing, switchable patterns (broadside, endfire, cardioid); 7–9 dBi on selected heading; system uses PIN diode switching",
        "feed_z": "50 Ω (combined via hybrid divider)",
        "swr_target": "< 1.5:1",
        "gain": "7–9 dBi (steerable)",
        "efficiency": "82–93%",
        "category": "special",
    },
    "portable_magnetic_loop": {
        "tm_num": "TM-ANT-054",
        "title": "PORTABLE MAGNETIC LOOP ANTENNA",
        "subtitle": "Field-Portable High-Q Tunable Magnetic Loop, 160M–10M",
        "bands": "160M–10M (2–6 ft diameter, band-dependent)",
        "approach": "Portable magnetic loop with vacuum variable or motorized butterfly capacitor for field use at QRP power levels",
        "special": "2–6 ft loop diameter; vacuum or motor-tuned capacitor REQUIRED for HF power; collapses for transport",
        "feed_z": "50 Ω (small coupling loop)",
        "swr_target": "< 2.0:1",
        "gain": "2–4 dBi",
        "efficiency": "65–90% (Q-dependent; lower at 160M)",
        "category": "magloop",
    },
    "satellite_yagi_antenna": {
        "tm_num": "TM-ANT-055",
        "title": "SATELLITE YAGI ANTENNA",
        "subtitle": "RHCP Yagi for LEO Satellite Work, 2M/70cm",
        "bands": "2M (144–146 MHz uplink/downlink), 70cm (432–438 MHz)",
        "approach": "Boom-fed Yagi with twisted elements or cross-yagi plus phasing harness for right-hand circular polarization (RHCP)",
        "special": "3–5 elements per band; RHCP via 90° phasing or crossed element pairs; optimized for LEO satellite Doppler range",
        "feed_z": "50 Ω",
        "swr_target": "< 1.5:1",
        "gain": "8–12 dBic",
        "efficiency": "85–93%",
        "category": "yagi",
    },
    "shunt_fed": {
        "tm_num": "TM-ANT-056",
        "title": "SHUNT-FED VERTICAL ANTENNA",
        "subtitle": "Tapped Impedance-Matching Feed for Tower or Vertical, 80M–20M",
        "bands": "80M, 40M, 20M (3.5–14.35 MHz)",
        "approach": "Grounded vertical radiator (tower or monopole) fed by a shunt wire tapped at a point where impedance equals 50 Ω",
        "special": "Tapped shunt wire spaced 0.1–0.2λ from tower; allows tower to serve as antenna without isolating base; gamma or omega match variants",
        "feed_z": "50 Ω at tap point",
        "swr_target": "< 2.0:1",
        "gain": "0 dBi (omnidirectional vertical)",
        "efficiency": "85–95%",
        "category": "vertical",
    },
    "slim_jim": {
        "tm_num": "TM-ANT-057",
        "title": "SLIM JIM ANTENNA",
        "subtitle": "End-Fed Folded Dipole with J-Match, Portable 2M/70cm",
        "bands": "2M (144–148 MHz), 70cm (430–440 MHz)",
        "approach": "End-fed folded dipole (J-pole variant) using flat ribbon cable; J-match provides 50 Ω at feed tap",
        "special": "~5 ft per 2M band; ladder line or window line construction; coils up for transport; excellent portable/emergency antenna",
        "feed_z": "50 Ω (tapped J-match stub)",
        "swr_target": "< 1.5:1",
        "gain": "3 dBi omnidirectional",
        "efficiency": "90–97%",
        "category": "vertical",
    },
    "slinky_antenna_20m_portable": {
        "tm_num": "TM-ANT-058",
        "title": "SLINKY ANTENNA — 20M PORTABLE",
        "subtitle": "Spring-Steel Slinky Helical Loading Element for 20M Compact Dipole",
        "bands": "20M (14.0–14.35 MHz)",
        "approach": "Spring-steel slinky toys deployed as loading coils in each arm of a compact dipole, providing electrical length in minimal physical length",
        "special": "Spring-steel slinky as loading element; collapses to 2 in diameter for transport; ~18 ft deployed total; 50 Ω feedpoint",
        "feed_z": "50 Ω",
        "swr_target": "< 2.0:1",
        "gain": "0–1 dBi (loading reduces radiation resistance)",
        "efficiency": "60–80% (slinky resistance loss)",
        "category": "dipole",
    },
    "sloped_dipole": {
        "tm_num": "TM-ANT-059",
        "title": "SLOPED DIPOLE ANTENNA",
        "subtitle": "Center-Fed Sloped Dipole with Directional Low-Angle Radiation, 80M–20M",
        "bands": "80M, 40M, 20M (3.5–14.35 MHz)",
        "approach": "Standard center-fed dipole tilted at 30–45° from horizontal, producing asymmetric directional pattern with low-angle component",
        "special": "30–45° slope; single support at top; balanced feed; some gain in the downhill direction at low elevation angle",
        "feed_z": "50–75 Ω (slope-dependent)",
        "swr_target": "< 2.0:1",
        "gain": "2–4 dBi (downhill direction)",
        "efficiency": "88–96%",
        "category": "dipole",
    },
    "slot_antenna_uhf": {
        "tm_num": "TM-ANT-060",
        "title": "SLOT ANTENNA — UHF 70cm",
        "subtitle": "PCB Slot Antenna for 430–440 MHz Full-Band Coverage",
        "bands": "70cm (430–440 MHz)",
        "approach": "Slot antenna cut in PCB ground plane; complementary to dipole by Babinet's principle; wideband design",
        "special": "36 mm slot on PCB ground plane; 50 Ω microstrip feed at slot center; wideband for full 70cm band coverage",
        "feed_z": "50 Ω (microstrip inset or coax probe)",
        "swr_target": "< 2.0:1 across 430–440 MHz",
        "gain": "2 dBi omnidirectional in plane",
        "efficiency": "80–92%",
        "category": "special",
    },
    "t_antenna": {
        "tm_num": "TM-ANT-061",
        "title": "T-ANTENNA (INVERTED-T)",
        "subtitle": "Capacitively Top-Loaded Vertical for Low-Band Efficiency, 160M–40M",
        "bands": "160M, 80M, 40M (1.8–7.3 MHz)",
        "approach": "Short vertical element with horizontal top-loading sections in both directions; capacitive loading increases effective electrical length",
        "special": "Vertical + horizontal top-hat sections; loading more effective than base coil; 40% compaction at 160M possible",
        "feed_z": "50 Ω (base feed with series tuning coil)",
        "swr_target": "< 2.0:1",
        "gain": "0 dBi (omnidirectional)",
        "efficiency": "75–90% (ground loss dominant)",
        "category": "vertical",
    },
    "tape_antennas": {
        "tm_num": "TM-ANT-062",
        "title": "TAPE ANTENNAS",
        "subtitle": "Flexible Copper-Tape VHF/UHF Arrays, 2M/70cm/1.25M Packable",
        "bands": "2M, 70cm, 1.25M (144–902 MHz range)",
        "approach": "Flexible copper-tape strip elements formed into antenna shapes; roll-up portability; suitable for improvised or emergency deployment",
        "special": "Flexible copper tape, 85% compact vs. rigid equivalent; vacuum-formed array possible; deployed with spreaders",
        "feed_z": "50 Ω (coax pigtail at feedpoint)",
        "swr_target": "< 2.0:1",
        "gain": "2–5 dBi (configuration dependent)",
        "efficiency": "85–94%",
        "category": "special",
    },
    "tape_measure_yagi": {
        "tm_num": "TM-ANT-063",
        "title": "TAPE MEASURE YAGI ANTENNA",
        "subtitle": "Ultra-Portable Roll-Up Yagi from Measuring Tape, 2M/70cm",
        "bands": "2M (144–148 MHz), 70cm (430–440 MHz)",
        "approach": "Spring-steel measuring tape elements mounted on PVC boom; elements flex for storage and snap straight for use",
        "special": "3–5 elements per band; rolls up for storage; ~8 dBi at 2M on 3-element; excellent for fox hunting and SOTA",
        "feed_z": "50 Ω (direct boom feed or delta match)",
        "swr_target": "< 1.5:1",
        "gain": "8 dBi (3-element 2M), 10 dBi (5-element 70cm)",
        "efficiency": "88–95%",
        "category": "yagi",
    },
    "telescopic": {
        "tm_num": "TM-ANT-064",
        "title": "TELESCOPIC WHIP ANTENNA",
        "subtitle": "Collapsible Adjustable-Length Whip, 6M–2M Portable",
        "bands": "6M (50–54 MHz), 2M (144–148 MHz) via length adjustment",
        "approach": "Multi-segment collapsing whip antenna with base coil and tuning coil for band change; portable and field-deployable",
        "special": "5–10 ft deployed; compact storage (8–15 in collapsed); tapped loading coil or base-coil for HF extension",
        "feed_z": "50 Ω (base feed, coil-matched)",
        "swr_target": "< 2.0:1",
        "gain": "0–2 dBi (length/loading dependent)",
        "efficiency": "70–90%",
        "category": "vertical",
    },
    "telescopic_vertical": {
        "tm_num": "TM-ANT-065",
        "title": "TELESCOPIC VERTICAL ANTENNA",
        "subtitle": "Adjustable Multiband HF Vertical, 80M–20M",
        "bands": "80M, 40M, 20M (3.5–14.35 MHz)",
        "approach": "Telescoping multi-section vertical antenna adjusted to quarter-wave at target frequency; tapped loading coil for lower bands",
        "special": "Telescoping segments extending to quarter-wave per band; 75% compaction collapsed; base loading coil for 80M; counterpoise or radials required",
        "feed_z": "50 Ω (base, with matching)",
        "swr_target": "< 2.0:1",
        "gain": "0 dBi (omnidirectional)",
        "efficiency": "75–90%",
        "category": "vertical",
    },
    "terminated_folded_dipole": {
        "tm_num": "TM-ANT-066",
        "title": "TERMINATED FOLDED DIPOLE (TFD) ANTENNA",
        "subtitle": "Broadband Folded Dipole with End Termination Resistors, 40M–10M",
        "bands": "40M, 20M, 10M (7–30 MHz broadband)",
        "approach": "Folded dipole with 600 Ω resistors terminating both ends; traveling-wave operation gives flat SWR across wide frequency range",
        "special": "600 Ω balanced termination; wide SWR bandwidth (3:1 or greater frequency ratio); resistive loss reduces efficiency ~3 dB",
        "feed_z": "600 Ω balanced (9:1 balun to 50 Ω)",
        "swr_target": "< 2.0:1 across 7–30 MHz",
        "gain": "0 dBi (efficiency reduced by termination)",
        "efficiency": "50–65% (termination loss fundamental)",
        "category": "dipole",
    },
    "terminated_sloper_receiving": {
        "tm_num": "TM-ANT-067",
        "title": "TERMINATED SLOPER RECEIVING ANTENNA",
        "subtitle": "Broadside Directional Receiving Wire with 600 Ω Termination, 160M–40M",
        "bands": "160M, 80M, 40M (1.8–7.3 MHz)",
        "approach": "Long sloped wire with resistive termination at far end producing traveling-wave cardioid directional receive pattern",
        "special": "60–120 ft wire; 600 Ω termination; ~10 dB front-to-back ratio; broadband operation without retuning",
        "feed_z": "50 Ω (via 9:1 transformer at feed end)",
        "swr_target": "< 1.5:1",
        "gain": "0–3 dBd forward",
        "efficiency": "Receive only — noise figure and front-to-back ratio are key metrics",
        "category": "receiving",
    },
    "tilted_terminated_folded_dipole": {
        "tm_num": "TM-ANT-068",
        "title": "TILTED TERMINATED FOLDED DIPOLE (T2FD)",
        "subtitle": "Sloped Broadband Folded Dipole with End Termination, 80M–20M",
        "bands": "80M, 40M, 20M (3.5–14.35 MHz broadband)",
        "approach": "Folded dipole tilted 30–45° from horizontal with 600 Ω end termination; broadband with both directional and polarization benefits",
        "special": "30–45° tilt; 600 Ω balanced termination at both ends; maintains SWR < 2:1 across 3:1 frequency range",
        "feed_z": "600 Ω balanced (9:1 balun to 50 Ω)",
        "swr_target": "< 2.0:1 across design range",
        "gain": "0 dBi (termination loss ~3 dB)",
        "efficiency": "50–65%",
        "category": "dipole",
    },
    "trap_dipole": {
        "tm_num": "TM-ANT-069",
        "title": "TRAP DIPOLE ANTENNA",
        "subtitle": "Multiband Dipole with LC Trap Elements for 160M–10M",
        "bands": "160M–10M (multiband via LC traps at junctions)",
        "approach": "Standard half-wave dipole extended with LC trap pairs resonant at band edges; each trap isolates the inner element for its target band",
        "special": "Trap pairs for each band transition; tuned with dip meter or NanoVNA; traps must handle full transmit power (rated for operating power)",
        "feed_z": "50–75 Ω (varies by band)",
        "swr_target": "< 2.0:1 per band",
        "gain": "2.1 dBi per band",
        "efficiency": "75–90% per band (trap loss)",
        "category": "dipole",
    },
    "traveling_wave_vertical": {
        "tm_num": "TM-ANT-070",
        "title": "TRAVELING WAVE VERTICAL ANTENNA",
        "subtitle": "Non-Resonant Terminated Vertical with Resistive Base Termination, 160M–40M",
        "bands": "160M, 80M, 40M (1.8–7.3 MHz broadband)",
        "approach": "Long non-resonant vertical wire with 600 Ω resistive termination at the base; traveling wave mode produces flat SWR",
        "special": "600 Ω resistive termination absorbs reflected power; broadband operation; some power dissipated in load resistor",
        "feed_z": "600 Ω at base (9:1 unun to 50 Ω)",
        "swr_target": "< 2.0:1 across 1.8–7.3 MHz",
        "gain": "0 dBi (termination loss)",
        "efficiency": "50–65% (resistive termination loss)",
        "category": "vertical",
    },
    "v_beam": {
        "tm_num": "TM-ANT-071",
        "title": "V-BEAM ANTENNA",
        "subtitle": "Two-Leg Terminated V Antenna for Bidirectional DX, 40M–20M",
        "bands": "40M (7.0–7.3 MHz), 20M (14.0–14.35 MHz)",
        "approach": "Two long wire elements arranged in a V-shape from a common feedpoint; bidirectional pattern along the V bisector",
        "special": "60–120 ft legs; 120–180° apex angle; terminated or non-terminated variants; directional gain 4–8 dBi on bisector",
        "feed_z": "500–800 Ω at apex (10:1 unun or ATU)",
        "swr_target": "< 2.0:1 after matching",
        "gain": "4–8 dBi bidirectional along bisector",
        "efficiency": "75–88%",
        "category": "special",
    },
    "vertical_dipole": {
        "tm_num": "TM-ANT-072",
        "title": "VERTICAL DIPOLE ANTENNA",
        "subtitle": "Center-Fed Vertical Dipole, Omnidirectional, 2M/70cm",
        "bands": "2M (144–148 MHz), 70cm (430–440 MHz)",
        "approach": "Standard half-wave dipole oriented vertically; requires balun to prevent feed line radiation",
        "special": "Omnidirectional azimuth; low elevation angle; requires common-mode choke (sleeve balun) at feed; clean pattern",
        "feed_z": "73 Ω (λ/2 dipole at resonance); 50 Ω with coaxial sleeve",
        "swr_target": "< 1.5:1",
        "gain": "2.1 dBi omnidirectional",
        "efficiency": "90–97%",
        "category": "vertical",
    },
    "yagi_uda": {
        "tm_num": "TM-ANT-073",
        "title": "YAGI-UDA DIRECTIONAL ANTENNA",
        "subtitle": "Multi-Element Directional Parasitic Array, 6M/2M/70cm",
        "bands": "6M (50–54 MHz), 2M (144–148 MHz), 70cm (430–440 MHz)",
        "approach": "Driven element with parasitic reflector and one or more directors providing forward gain; boom-mounted",
        "special": "3–5 elements; 8–12 dBi gain; fixed or rotatable; reflector 5% longer than driven, directors 5% shorter",
        "feed_z": "50 Ω (via gamma match or T-match at driven element)",
        "swr_target": "< 1.5:1",
        "gain": "8–12 dBi forward",
        "efficiency": "88–95%",
        "category": "yagi",
    },
}

# ─────────────────────────────────────────────────────────────────────────────
# SHARED HTML PIECES
# ─────────────────────────────────────────────────────────────────────────────

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

FOOTER_HTML = """    <footer>
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

NAV_HTML = """        <nav>
            <h3>Main Navigation</h3>
            <ul>
                <li><a href="/">Home</a></li>
                <li><a href="../../about/bio.html">About</a></li>
            </ul>
            <details class="nav-group" open>
                <summary>Antenna Library</summary>
                <ul><li><a href="../index.html">Antenna Library</a></li></ul>
            </details>
            <details class="nav-group">
                <summary>Test Equipment</summary>
                <ul><li><a href="../../test_gear/index.html">Test Equipment Library</a></li></ul>
            </details>
            <details class="nav-group">
                <summary>Calibration Ideas</summary>
                <ul><li><a href="../../Calibration_Ideas/index.html">Calibration Standards</a></li></ul>
            </details>
            <details class="nav-group" open>
                <summary>Study &amp; Training</summary>
                <ul>
                    <li><a href="https://assets.doflagie.com/study/index.html">Ham Radio Flash Card Quiz</a></li>
                    <li><a href="https://assets.doflagie.com/study/morse.html">Morse Code Practice</a></li>
                </ul>
            </details>
        </nav>
"""


# ─────────────────────────────────────────────────────────────────────────────
# CONTENT GENERATORS BY CATEGORY
# ─────────────────────────────────────────────────────────────────────────────

def warn_hv():
    return """<div class="tm-warning">
<span class="admonition-label">WARNING</span>
HIGH VOLTAGE HAZARD: At 100 W output power the tuning capacitor of a magnetic loop antenna
develops 4,000–12,000 V across its terminals, depending on loop Q and operating frequency.
Use ONLY vacuum variable capacitors or high-voltage butterfly (split-stator) capacitors rated
for at least 5 kV RMS. Do NOT use DIY stacked aluminum-plate capacitors, rolled-foil capacitors,
or short lengths of coaxial cable as capacitors — these will arc and fail, causing equipment damage
and potential personal injury. Allow the capacitor to fully discharge before touching any part of
the loop structure after transmitting. Keep hands and metal objects away from the capacitor gap
during tuning.
</div>"""

def warn_rf():
    return """<div class="tm-caution">
<span class="admonition-label">CAUTION — RF EXPOSURE</span>
Maintain minimum safe distance from all energized antenna elements during transmission.
At QRP power levels (&le;5 W) the MPE boundary is typically &lt;1 m for HF antennas.
At 100 W the controlled exposure limit for HF antennas requires maintaining &ge;3–10 m
distance depending on frequency (per FCC OET Bulletin 65). Never touch feed-point hardware
or support structures while transmitting. Verify PTT key is open before antenna work.
</div>"""

def note_nec():
    return """<div class="tm-note">
<span class="admonition-label">NOTE</span>
The NEC2 model file for this antenna is included in the antenna directory. Run it with
xnec2c, 4nec2, or any NEC2-compatible engine to generate polar plots, impedance data,
and gain figures. The NanoVNA measurements in Chapter 5 should be compared against
NEC2 predictions — deviations &gt;3 dB or &gt;20% impedance indicate a construction error.
</div>"""

def note_receiving():
    return """<div class="tm-note">
<span class="admonition-label">NOTE — RECEIVE-ONLY ANTENNA</span>
This is a receiving antenna. Efficiency in the conventional sense does not apply;
the relevant metrics are noise figure, front-to-back ratio, and noise floor relative
to ambient. Do not transmit into this antenna — the matching network and termination
resistors are rated for received signal levels only.
</div>"""


def gen_dipole(d, ant):
    """Content generator for dipole-family antennas."""
    bands = ant["bands"]
    approach = ant["approach"]
    special = ant["special"]
    feed_z = ant["feed_z"]
    gain = ant["gain"]
    eff = ant["efficiency"]

    return f"""
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers design, construction, calibration, and field operation of the {ant['title'].title()} for amateur radio use.
The antenna is intended for operation on {bands}. All procedures are written for the tools available at the field station:
NanoVNA (vector network analyzer), TinySA (spectrum analyzer), and standard hand tools.</p>

<h3 class="section-heading">1-2. APPLICABLE REFERENCES</h3>
<ul>
<li>ARRL Antenna Book, 25th Edition, Chapter on Dipoles and Doublets</li>
<li>NEC2 model: {d}_spec.nec (in antenna directory)</li>
<li>FCC OET Bulletin 65 — RF Exposure Evaluation</li>
<li>ITU-R P.533 — HF propagation prediction methods</li>
</ul>

<h3 class="section-heading">1-3. SAFETY PRECAUTIONS</h3>
{warn_rf()}
<div class="tm-note">
<span class="admonition-label">NOTE</span>
Ensure antenna ends and feed point are clear of personnel and metal structures by at least 2 m before applying RF power.
</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. RADIATION PHYSICS</h3>
<p>{approach.capitalize()}. A dipole radiates because time-varying current in the conductor produces a time-varying magnetic field; the resulting displacement current produces a time-varying electric field; together these fields propagate outward as an electromagnetic wave. For a half-wave dipole at resonance the radiation resistance R<sub>r</sub> = 73 &Omega; and reactance X = 0, producing maximum current amplitude for a given drive voltage.</p>

<h3 class="section-heading">2-2. RADIATION PATTERN</h3>
<p>A horizontal half-wave dipole at height h above ground produces a bidirectional pattern broadside to the wire axis. At height &ge;0.5&lambda; the main lobe elevation angle &theta; &asymp; 15&ndash;30&deg;, ideal for DX paths. At height &lt;0.15&lambda; (NVIS regime) the main lobe is near-vertical. The E-plane pattern is a figure-eight; H-plane is omnidirectional. Gain broadside is 2.1 dBi relative to isotropic.</p>

<h3 class="section-heading">2-3. IMPEDANCE AND BANDWIDTH</h3>
<p>Feed impedance: {feed_z}. A longer or cage construction widens bandwidth because the increased effective conductor diameter reduces the Q of the resonance. For a simple wire dipole, the 2:1 SWR bandwidth at 40M is typically 100&ndash;200 kHz; a cage construction widens this to 300&ndash;500 kHz or the full band.</p>

<h3 class="section-heading">2-4. POLARIZATION AND PROPAGATION</h3>
<p>This antenna is linearly polarized. At HF, ionospheric propagation rotates polarization (Faraday rotation), so polarization match at the far end is largely unpredictable. Cross-polarization loss of 3&ndash;10 dB is common on skywave paths. For NVIS and groundwave propagation, horizontal polarization is preferred for local coverage.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; MATERIALS AND CONSTRUCTION</h2>
<h3 class="section-heading">3-1. BILL OF MATERIALS</h3>
<table class="tm-table">
<caption>Typical Materials for {ant['title'].title()}</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Per design</td><td>Copper wire (antenna elements)</td><td>#14 AWG solid or stranded copper, PVC-jacketed preferred for weather resistance</td></tr>
<tr><td>1</td><td>Feed-point insulator / center connector</td><td>SO-239 or UHF-F, weatherproof, UV-resistant housing</td></tr>
<tr><td>2</td><td>End insulators</td><td>Egg insulators or equivalent; rated for wire tension at operating temperature</td></tr>
<tr><td>As needed</td><td>Halyard / support rope</td><td>Dacron or polypropylene, non-conductive, UV-resistant</td></tr>
<tr><td>1</td><td>Choke balun (1:1 current balun)</td><td>Mix-31 ferrite cores, 8–12 turns of RG-142 or RG-8X through core; FB-31-6873 or equivalent</td></tr>
<tr><td>1</td><td>Coaxial feed line</td><td>RG-8X or RG-213 to station; minimize length for efficiency</td></tr>
</tbody>
</table>

<h3 class="section-heading">3-2. DIMENSION FORMULAS</h3>
<div class="formula-block"><span class="formula-label">Half-wave dipole element length (each arm, feet)</span>L<sub>arm</sub> = 234 / f<sub>MHz</sub></div>
<div class="formula-block"><span class="formula-label">Full dipole length (feet)</span>L<sub>total</sub> = 468 / f<sub>MHz</sub></div>
<div class="formula-block"><span class="formula-label">Example: 40M at 7.150 MHz</span>L<sub>arm</sub> = 234 / 7.150 = 32.7 ft &mdash; each arm</div>
<p>Note: The 468 constant assumes wire of #12&ndash;#14 AWG copper at ambient temperature; adjust down 1&ndash;2% for thick conductors or cage construction (velocity factor &lt;1).</p>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
{warn_rf()}
<div class="procedure"><ol>
<li>Calculate element lengths using the formula in Chapter 3. Add 5% extra wire for trimming (do not cut to final length until resonance is verified).</li>
<li>Solder or crimp conductors to center feedpoint connector. Orient SO-239 with center pin to one arm and braid to the other. Install 1:1 current balun at feedpoint.</li>
<li>Attach end insulators and support ropes to both wire ends. Tie off-load with a bowline knot &mdash; not a slip knot.</li>
<li>Raise antenna to operating height. Orient element perpendicular to desired direction of maximum radiation (broadside direction).</li>
<li>Connect RG-8X or RG-213 feed line from balun to shack. Route cable away from element to avoid coupling. Secure with UV-resistant cable ties.</li>
<li>Perform initial SWR measurement per Chapter 5 before first transmission.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<h3 class="section-heading">5-1. NANOVNA SWR AND IMPEDANCE SWEEP</h3>
{note_nec()}
<div class="procedure"><ol>
<li>Perform SOLT calibration on NanoVNA using the SOL (Short-Open-Load) kit at the antenna end of the feed line.</li>
<li>Set NanoVNA sweep range to cover ±10% of target center frequency (example: 40M &rarr; 6.5&ndash;7.8 MHz).</li>
<li>Connect NanoVNA to feedpoint. Navigate to CH0 S11 display. Select Smith Chart and SWR graphs.</li>
<li>Record: frequency of minimum SWR (f<sub>res</sub>), SWR at f<sub>res</sub>, SWR at band edges, R + jX at f<sub>res</sub>.</li>
<li>Resonance is confirmed when X &asymp; 0 and R &asymp; 73 &Omega; (simple dipole) or per design (matched system).</li>
<li>Compare measured f<sub>res</sub> to design frequency. If f<sub>res</sub> is too high, the element is short &mdash; lengthen each arm 1&ndash;2 in. If f<sub>res</sub> is too low, the element is long &mdash; trim each arm 1 in. Repeat until f<sub>res</sub> is within ±0.5% of design frequency.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<h3 class="section-heading">6-1. RESONANCE ADJUSTMENT</h3>
<p>Trim or extend element arms symmetrically to shift resonance. Each 1-inch change in total length shifts resonance by approximately f<sub>MHz</sub>/468 × 12 kHz for a 40M dipole. Adjust in 2-inch increments and re-measure SWR between adjustments. Finalize element length when SWR at design frequency is &lt;1.5:1 or as specified.</p>
<h3 class="section-heading">6-2. IMPEDANCE MATCHING</h3>
<p>If impedance at resonance differs from 50 &Omega;, adjust element height (increases ground effect), add a matching network (L-network, &lambda;/4 transformer, or series capacitor), or use a 4:1 balun for designs with higher feed-point impedance such as folded dipoles ({feed_z}).</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<h3 class="section-heading">7-1. ACCEPTANCE CRITERIA</h3>
<table class="tm-table">
<caption>Verification Criteria</caption>
<thead><tr><th>Parameter</th><th>Requirement</th><th>Pass/Fail</th></tr></thead>
<tbody>
<tr><td>SWR at resonance</td><td>{ant['swr_target']}</td><td>____</td></tr>
<tr><td>Resonant frequency</td><td>Within ±1% of design</td><td>____</td></tr>
<tr><td>Feed impedance (R)</td><td>As designed (±15%)</td><td>____</td></tr>
<tr><td>Feed reactance (X)</td><td>&lt;±10 &Omega; at resonance</td><td>____</td></tr>
<tr><td>Gain (NEC2 model)</td><td>{gain}</td><td>____</td></tr>
<tr><td>Efficiency</td><td>{eff}</td><td>____</td></tr>
</tbody>
</table>
<div class="procedure"><ol>
<li>Confirm SWR &lt;{ant['swr_target'].replace('< ','')} at center frequency.</li>
<li>Verify resonance frequency within ±1% of design center.</li>
<li>Confirm impedance real part within 15% of design value.</li>
<li>Record results in station log with date, antenna height, and feed line length.</li>
</ol></div>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Half-wave dipole element length (each arm, feet)</span>L<sub>arm</sub> = 234 / f<sub>MHz</sub></div>
<div class="formula-block"><span class="formula-label">Velocity factor correction (for insulated wire)</span>L<sub>adj</sub> = L<sub>arm</sub> &times; V<sub>f</sub> (V<sub>f</sub> &asymp; 0.95 for #14 AWG PVC)</div>
<div class="formula-block"><span class="formula-label">Radiation resistance (half-wave dipole, free space)</span>R<sub>r</sub> = 73.1 &Omega;</div>
<div class="formula-block"><span class="formula-label">Gain over isotropic (free space)</span>G = 2.14 dBi</div>
<div class="formula-block"><span class="formula-label">Height for DX angle (degrees elevation for max gain)</span>&theta; &asymp; arcsin(1 / (2h/&lambda;)) for h &gt; 0.25&lambda;</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table">
<caption>Typical NanoVNA Measurements — {ant['title'].title()}</caption>
<thead><tr><th>Band</th><th>f<sub>res</sub> (MHz)</th><th>SWR</th><th>R (&Omega;)</th><th>X (&Omega;)</th><th>Notes</th></tr></thead>
<tbody>
<tr><td>Per design</td><td>Design center</td><td>&lt;1.5:1</td><td>50–73</td><td>&minus;5 to +5</td><td>After trimming to resonance</td></tr>
<tr><td>Band edge low</td><td>f<sub>low</sub></td><td>&lt;2.0:1</td><td>40–65</td><td>&minus;30 to 0</td><td>Capacitive below resonance</td></tr>
<tr><td>Band edge high</td><td>f<sub>high</sub></td><td>&lt;2.0:1</td><td>40–65</td><td>0 to +30</td><td>Inductive above resonance</td></tr>
</tbody>
</table>
"""


def gen_vertical(d, ant):
    """Content generator for vertical-family antennas."""
    bands = ant["bands"]
    approach = ant["approach"]
    special = ant["special"]
    feed_z = ant["feed_z"]
    gain = ant["gain"]
    eff = ant["efficiency"]

    return f"""
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers design, construction, calibration, and field operation of the {ant['title'].title()} for amateur radio and communications use. Applicable frequency bands: {bands}. All calibration procedures use the NanoVNA and TinySA test instruments.</p>

<h3 class="section-heading">1-2. APPLICABLE REFERENCES</h3>
<ul>
<li>ARRL Antenna Book — Chapter on Vertical Antennas</li>
<li>NEC2 model: {d}.nec (in antenna directory)</li>
<li>FCC OET Bulletin 65 — RF Exposure Evaluation</li>
<li>Radial Systems for Vertical Antennas, Brown, Lewis &amp; Epstein (1937)</li>
</ul>

<h3 class="section-heading">1-3. SAFETY PRECAUTIONS</h3>
{warn_rf()}

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. RADIATION PHYSICS</h3>
<p>{approach.capitalize()}. A vertical radiator carries current that is vertically polarized; its electromagnetic wave radiates with E-field vertical. The ground (or radial counterpoise) serves as the electrical mirror-image of the above-ground element: a &lambda;/4 vertical above a perfect ground is equivalent to a &lambda;/2 dipole in free space, with radiation resistance R<sub>r</sub> = 36.6 &Omega;.</p>

<h3 class="section-heading">2-2. RADIATION PATTERN</h3>
<p>A quarter-wave vertical over an extensive ground plane radiates omnidirectionally in azimuth with a low-angle elevation lobe ideal for DX. The elevation angle of maximum radiation (&theta;<sub>max</sub>) depends on ground conductivity and number of radials: over a perfect ground &theta;<sub>max</sub> &asymp; 0&deg;; over average ground with 32+ radials, &theta;<sub>max</sub> &asymp; 5&ndash;15&deg;. The vertically polarized wave follows the earth&rsquo;s surface better than a horizontal wave at low angles.</p>

<h3 class="section-heading">2-3. IMPEDANCE AND BANDWIDTH</h3>
<p>Feed impedance: {feed_z}. The SWR bandwidth of a simple &lambda;/4 vertical at HF is approximately 5&ndash;8% of center frequency for 2:1 SWR, typically covering one amateur band. Loading coils reduce bandwidth in proportion to their Q; top loading preserves more bandwidth than base loading because it maintains higher current along more of the radiator length.</p>

<h3 class="section-heading">2-4. GROUND SYSTEM DESIGN</h3>
<p>Ground loss resistance R<sub>g</sub> appears directly in series with the radiation resistance. For maximum efficiency: R<sub>g</sub> &lt; R<sub>r</sub>. With buried radials, R<sub>g</sub> decreases as N<sub>radials</sub> increases: 32 radials each 0.25&lambda; gives R<sub>g</sub> &asymp; 3 &Omega;. Elevated resonant radials achieve similar performance with only 4&ndash;8 radials. For {special.split(';')[0].lower()} — follow this requirement closely for efficiency.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; MATERIALS AND CONSTRUCTION</h2>
<h3 class="section-heading">3-1. BILL OF MATERIALS</h3>
<table class="tm-table">
<caption>Materials for {ant['title'].title()}</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>1</td><td>Vertical element</td><td>Aluminum tubing 1&ndash;1.5 in OD or #12 AWG copper wire; length per formula</td></tr>
<tr><td>Per design</td><td>Radials</td><td>#16&ndash;#14 AWG copper; per ground system design above</td></tr>
<tr><td>1</td><td>Mast/base insulator</td><td>UV-resistant PVC or polycarbonate; must support element tension/weight</td></tr>
<tr><td>1</td><td>SO-239 or N-type connector</td><td>Weatherproof; mount at base</td></tr>
<tr><td>1</td><td>Feed line</td><td>RG-213 or LMR-400 for permanent install; RG-8X for portable use</td></tr>
<tr><td>As needed</td><td>Loading coil (if loaded vertical)</td><td>T-200 toroidal core or air-wound; Ql &gt; 200 for minimum loss</td></tr>
</tbody>
</table>

<h3 class="section-heading">3-2. DIMENSION FORMULAS</h3>
<div class="formula-block"><span class="formula-label">Quarter-wave vertical height (feet)</span>L = 234 / f<sub>MHz</sub></div>
<div class="formula-block"><span class="formula-label">Example: 40M at 7.150 MHz</span>L = 234 / 7.150 = 32.7 ft</div>
<div class="formula-block"><span class="formula-label">Resonant radial length (each radial, feet)</span>L<sub>rad</sub> = 246 / f<sub>MHz</sub> (resonant elevated radials)</div>
<div class="formula-block"><span class="formula-label">Loading coil inductance for shortened element</span>L (&mu;H) = Z<sub>0</sub> &times; (1 &minus; cos(&theta;)) / (2&pi;f &times; sin(&theta;)) where &theta; = electrical length of element</div>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
{warn_rf()}
<div class="procedure"><ol>
<li>Install base mounting hardware and base insulator. Verify insulator breakdown voltage rating exceeds 2&times; operating RF voltage.</li>
<li>Erect vertical element. For a guyed installation, use non-conductive guy ropes (Dacron); attach guys at top and 2/3 height.</li>
<li>Install radial system. For buried radials: trench radials 2&ndash;4 in deep in lawn. For elevated radials: support at 1.5&ndash;2 m above ground, run outward from base.</li>
<li>Bond all radials at the base via a radial bus ring (copper strap or ring terminal). Bond bus ring to coax braid at feedpoint.</li>
<li>Connect center conductor of feed line to element base. Connect braid to radial bus ring. Weather-seal with self-amalgamating tape and UV jacket tape.</li>
<li>Perform initial SWR sweep per Chapter 5 before first RF application.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<h3 class="section-heading">5-1. NANOVNA IMPEDANCE SWEEP</h3>
{note_nec()}
<div class="procedure"><ol>
<li>SOLT calibrate NanoVNA at the end of the feed line (antenna side).</li>
<li>Set sweep: center frequency = design band center; span = ±20% of center.</li>
<li>Connect CH0 port to antenna feedpoint. Enable Smith Chart and SWR displays.</li>
<li>Record: f<sub>res</sub> (X = 0), SWR at f<sub>res</sub>, R at f<sub>res</sub>, SWR bandwidth (2:1 SWR points).</li>
<li>Resonance target: X = 0 &plusmn;5 &Omega;, R = 36&ndash;52 &Omega; (ground losses shift R above 36.6 &Omega;).</li>
<li>If f<sub>res</sub> too high: element is short, extend by 1&ndash;2 in. If f<sub>res</sub> too low: element too long, trim 1 in. Repeat.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<h3 class="section-heading">6-1. ELEMENT LENGTH ADJUSTMENT</h3>
<p>Trim or extend the vertical element to set resonance. Each 1-inch change shifts f<sub>res</sub> by approximately 20&ndash;30 kHz at 40M. Adjust in 2-inch increments. If using a loading coil, adjust coil tap position to shift resonance; moving tap toward the feed end increases inductance and lowers resonance.</p>
<h3 class="section-heading">6-2. RADIAL SYSTEM OPTIMIZATION</h3>
<p>Measure R at resonance before and after adding radials. A decrease in R indicates reduced ground loss (desired). Continue adding radials until further additions change R by &lt;1 &Omega;. The point of diminishing returns is typically 16&ndash;32 radials for buried systems, 4&ndash;8 for elevated resonant systems.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table">
<caption>Acceptance Criteria</caption>
<thead><tr><th>Parameter</th><th>Requirement</th><th>Pass/Fail</th></tr></thead>
<tbody>
<tr><td>SWR at resonance</td><td>{ant['swr_target']}</td><td>____</td></tr>
<tr><td>Resonant frequency</td><td>Within ±1% of design</td><td>____</td></tr>
<tr><td>Feed impedance (R)</td><td>35&ndash;55 &Omega;</td><td>____</td></tr>
<tr><td>Feed reactance (X)</td><td>&lt;±10 &Omega;</td><td>____</td></tr>
<tr><td>Gain (NEC2)</td><td>{gain}</td><td>____</td></tr>
<tr><td>Efficiency</td><td>{eff}</td><td>____</td></tr>
</tbody>
</table>
<div class="procedure"><ol>
<li>Confirm SWR meets specification on all design bands.</li>
<li>Verify resonance frequency within ±1% of design center.</li>
<li>Confirm radial bond resistance &lt;0.1 &Omega; with ohmmeter from feedpoint braid to each radial tip.</li>
<li>Log: date, ground condition, radial count, SWR, R+jX at each band, transmitter output power used for test.</li>
</ol></div>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Quarter-wave element height (feet)</span>L = 234 / f<sub>MHz</sub></div>
<div class="formula-block"><span class="formula-label">Radiation resistance (quarter-wave over perfect ground)</span>R<sub>r</sub> = 36.6 &Omega;</div>
<div class="formula-block"><span class="formula-label">Efficiency (&eta;)</span>&eta; = R<sub>r</sub> / (R<sub>r</sub> + R<sub>g</sub> + R<sub>coil</sub>)</div>
<div class="formula-block"><span class="formula-label">Loading coil Q (air-wound)</span>Q<sub>L</sub> = X<sub>L</sub> / R<sub>coil</sub> where X<sub>L</sub> = 2&pi;f&middot;L</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table">
<caption>Typical NanoVNA Results — {ant['title'].title()}</caption>
<thead><tr><th>Band</th><th>f<sub>res</sub> (MHz)</th><th>SWR</th><th>R (&Omega;)</th><th>X (&Omega;)</th><th>2:1 BW (kHz)</th></tr></thead>
<tbody>
<tr><td>40M</td><td>7.150</td><td>1.3:1</td><td>38</td><td>+2</td><td>180</td></tr>
<tr><td>20M</td><td>14.175</td><td>1.4:1</td><td>41</td><td>&minus;3</td><td>350</td></tr>
</tbody>
</table>
"""


def gen_loop(d, ant):
    """Content generator for loop antennas (non-magnetic)."""
    bands = ant["bands"]
    approach = ant["approach"]
    special = ant["special"]
    feed_z = ant["feed_z"]
    gain = ant["gain"]
    eff = ant["efficiency"]

    return f"""
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers the {ant['title'].title()} for operation on {bands}. The loop topology provides an omnidirectional azimuth pattern (for vertical orientation) or a bidirectional pattern (for horizontal orientation), with inherent advantage over dipoles in low-band noise floor due to reduced sensitivity to vertically polarized man-made noise.</p>

<h3 class="section-heading">1-2. APPLICABLE REFERENCES</h3>
<ul>
<li>ARRL Antenna Book — Chapter on Loop Antennas</li>
<li>NEC2 model: {d}.nec (in antenna directory)</li>
<li>ON4UN&rsquo;s Low Band DXing — Loop antenna chapters</li>
<li>FCC OET Bulletin 65 — RF Exposure Evaluation</li>
</ul>

<h3 class="section-heading">1-3. SAFETY PRECAUTIONS</h3>
{warn_rf()}

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. RADIATION PHYSICS</h3>
<p>{approach.capitalize()}. A full-wave loop fed at any point resonates when the total conductor length equals one free-space wavelength: &lambda; = 300/f<sub>MHz</sub> meters. The feed-point impedance of a square loop at resonance is approximately 100&ndash;150 &Omega; (real), varying with shape and height above ground. A circular loop has slightly higher radiation resistance than a square loop of the same perimeter.</p>

<h3 class="section-heading">2-2. RADIATION PATTERN</h3>
<p>A horizontal full-wave loop at 0.5&lambda; height produces a low-angle radiation pattern in the plane of the loop (edge-fire). Gain over a dipole at the same height is approximately 1.5&ndash;2 dB because the current distribution around the loop produces additive broadside contributions. Vertical loops produce an omnidirectional azimuth pattern with vertically polarized radiation; horizontal loops are horizontally polarized. Estimated gain: {gain}.</p>

<h3 class="section-heading">2-3. IMPEDANCE AND MATCHING</h3>
<p>Feed impedance: {feed_z}. The loop impedance at resonance is typically 100&ndash;150 &Omega;, requiring a 2:1 balun (or direct feed to 75 &Omega; coax) for a reasonable match to 50 &Omega; systems. Off-resonance the loop can be operated through an ATU. Harmonic resonances occur at odd and even multiples of the fundamental frequency, extending multiband coverage without retuning.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; MATERIALS AND CONSTRUCTION</h2>
<h3 class="section-heading">3-1. BILL OF MATERIALS</h3>
<table class="tm-table">
<caption>Materials for {ant['title'].title()}</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>1 length</td><td>Loop conductor</td><td>#14&ndash;#12 AWG copper wire; length = 1008/f<sub>MHz</sub> feet (full wavelength)</td></tr>
<tr><td>2&ndash;4</td><td>Corner insulators / spreaders</td><td>UV-stable plastic or PVC; one per loop corner</td></tr>
<tr><td>1</td><td>Feed-point insulator with connector</td><td>SO-239 or N-type; weatherproof</td></tr>
<tr><td>1</td><td>2:1 current balun</td><td>Mix-31 or Mix-61 ferrite; handles operating power</td></tr>
<tr><td>1</td><td>Support structure</td><td>PVC mast, fiber-glass pole, or tree; non-conductive preferred</td></tr>
</tbody>
</table>

<h3 class="section-heading">3-2. PERIMETER FORMULA</h3>
<div class="formula-block"><span class="formula-label">Full-wave loop perimeter (feet)</span>P = 1005 / f<sub>MHz</sub> (accounts for velocity factor &asymp; 0.997)</div>
<div class="formula-block"><span class="formula-label">Side length for equilateral triangle loop (feet)</span>S = P / 3</div>
<div class="formula-block"><span class="formula-label">Side length for square loop (feet)</span>S = P / 4</div>
<div class="formula-block"><span class="formula-label">Example: 40M at 7.150 MHz</span>P = 1005 / 7.150 = 140.6 ft; square side = 35.1 ft</div>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
{warn_rf()}
<div class="procedure"><ol>
<li>Calculate loop perimeter per formula above. Cut conductor 5% long for trimming.</li>
<li>Install corner supports at spacing equal to side length. Attach conductor to corner insulators with UV-stable lacing or binding wire.</li>
<li>Close the loop — solder or crimp conductor ends to feedpoint connector terminals. Install 2:1 balun at feedpoint before connecting coax.</li>
<li>Raise loop to operating height. Horizontal polarization: loop plane horizontal. Vertical polarization: loop plane vertical, feed at bottom.</li>
<li>Route feed line downward from feedpoint, maintaining 90&deg; departure from loop plane for at least 0.5&lambda; to minimize coupling.</li>
<li>Perform SWR sweep per Chapter 5.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>SOLT calibrate NanoVNA at antenna end of feed line (or at feedpoint balun output).</li>
<li>Set sweep: fundamental resonance ±20%.</li>
<li>Verify f<sub>res</sub> where X = 0. Record R and SWR.</li>
<li>Expected: R = 100&ndash;150 &Omega; at resonance before matching. After 2:1 balun: R = 50&ndash;75 &Omega; at 50 &Omega; coax.</li>
<li>Trim loop perimeter in 6-inch increments to shift resonance to design frequency. Do not cut — fold wire back on itself until correct length found, then cut cleanly.</li>
<li>Check harmonic resonances at 2f<sub>0</sub>, 3f<sub>0</sub>; verify SWR acceptable for multiband use.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Trim loop perimeter to shift resonance. Each 1-ft change in total perimeter shifts resonance approximately 10&ndash;20 kHz at 40M. An ATU at the feed line allows operation across a 2:1 frequency range from a single cut length. For delta loops, adjusting the feed point position (base corner vs. apex) changes the feed impedance but not the resonant frequency.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table">
<caption>Acceptance Criteria</caption>
<thead><tr><th>Parameter</th><th>Requirement</th><th>Pass/Fail</th></tr></thead>
<tbody>
<tr><td>SWR at resonance</td><td>{ant['swr_target']}</td><td>____</td></tr>
<tr><td>Resonant frequency</td><td>Within ±1% of design</td><td>____</td></tr>
<tr><td>R at resonance</td><td>100&ndash;150 &Omega; (before matching)</td><td>____</td></tr>
<tr><td>Gain (NEC2)</td><td>{gain}</td><td>____</td></tr>
<tr><td>Efficiency</td><td>{eff}</td><td>____</td></tr>
</tbody>
</table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Full-wave loop perimeter (feet)</span>P = 1005 / f<sub>MHz</sub></div>
<div class="formula-block"><span class="formula-label">Radiation resistance (full-wave loop, free space)</span>R<sub>r</sub> &asymp; 100&ndash;150 &Omega; (shape-dependent)</div>
<div class="formula-block"><span class="formula-label">Gain over dipole at same height</span>&Delta;G &asymp; 1.5&ndash;2 dB</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table">
<caption>Typical Results — {ant['title'].title()}</caption>
<thead><tr><th>Band</th><th>f<sub>res</sub> (MHz)</th><th>SWR</th><th>R (&Omega;)</th><th>X (&Omega;)</th><th>Notes</th></tr></thead>
<tbody>
<tr><td>40M</td><td>7.150</td><td>1.4:1</td><td>120</td><td>+5</td><td>After 2:1 balun: SWR 1.6:1</td></tr>
<tr><td>20M</td><td>14.175</td><td>1.5:1</td><td>110</td><td>&minus;8</td><td>Harmonic resonance</td></tr>
</tbody>
</table>
"""


def gen_magloop(d, ant):
    """Content generator for magnetic loop antennas."""
    bands = ant["bands"]
    special = ant["special"]
    feed_z = ant["feed_z"]
    gain = ant["gain"]
    eff = ant["efficiency"]

    return f"""
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers the design, construction, and safe operation of the {ant['title'].title()} for amateur radio HF use on {bands}. Magnetic loops are compact, low-noise antennas with narrow bandwidth, requiring careful tuning for each frequency. Due to high circulating currents and voltages at the tuning capacitor, strict safety procedures must be followed.</p>

<h3 class="section-heading">1-2. APPLICABLE REFERENCES</h3>
<ul>
<li>ARRL Antenna Book — Small Transmitting Loops Chapter</li>
<li>G4FGQ Technical Notes on Magnetic Loops</li>
<li>W0BTU / AA5TB Small Transmitting Loop Calculators</li>
<li>NEC2 model: {d}.nec (in antenna directory)</li>
</ul>

<h3 class="section-heading">1-3. SAFETY PRECAUTIONS</h3>
{warn_hv()}
{warn_rf()}

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. RESONANT LOOP PHYSICS</h3>
<p>A magnetic loop (small transmitting loop, STL) is an electrically small resonant circuit. The loop inductance L is tuned to resonance by a capacitor C. At resonance, circulating current I<sub>circ</sub> = V<sub>in</sub> &times; Q / Z<sub>loop</sub> where Q is the loop quality factor. Q values of 100&ndash;1000 are typical; this produces circulating currents far exceeding the feed current, and capacitor voltages V<sub>C</sub> = I<sub>circ</sub> / (2&pi;f&middot;C) which can reach 4,000&ndash;12,000 V at 100 W.</p>

<h3 class="section-heading">2-2. RADIATION PATTERN AND GAIN</h3>
<p>A small horizontal loop radiates as a magnetic dipole with a figure-eight pattern in the plane of the loop (null perpendicular to loop plane). Gain: {gain}. The null depth exceeds 20 dB and can be used for interference rejection by rotating the loop. Bandwidth is inversely proportional to Q: BW<sub>3dB</sub> = f<sub>0</sub>/Q; typical BW = 3&ndash;15 kHz at HF. This narrow bandwidth means the loop must be retuned for every frequency change.</p>

<h3 class="section-heading">2-3. EFFICIENCY AND CONDUCTOR SELECTION</h3>
<p>Efficiency: {eff}. Loop efficiency &eta; = R<sub>r</sub>/(R<sub>r</sub> + R<sub>loss</sub>). Radiation resistance R<sub>r</sub> for a small loop scales as (A/&lambda;&sup2;)&sup2;, making large loop diameter critical for efficiency. Conductor resistance R<sub>loss</sub> must be minimized: use copper or aluminum tubing (1&ndash;2 in OD for HF transmitting loops). Connections must be silver-soldered or bolted with contact resistance &lt;0.001 &Omega;. Any high-resistance joint will dissipate power and possibly arc at high Q.</p>

<h3 class="section-heading">2-4. CAPACITOR REQUIREMENTS</h3>
<p>Vacuum variable capacitor (recommended): rated for peak voltage &ge;5 kV at operating power level. Split-stator butterfly capacitor: acceptable if plate gap is sufficient. Do NOT use: aluminum plate DIY capacitors, rolled-foil capacitors, silver-mica stacks, or coaxial stubs. Typical capacitance range: 10&ndash;500 pF for 40M&ndash;10M operation with 6&ndash;10 ft loop diameter.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; MATERIALS AND CONSTRUCTION</h2>
<h3 class="section-heading">3-1. BILL OF MATERIALS</h3>
<table class="tm-table">
<caption>Materials — {ant['title'].title()}</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>1 length</td><td>Main loop conductor</td><td>Copper or aluminum tubing 1&ndash;1.5 in OD; circumference per design frequency</td></tr>
<tr><td>1</td><td>Vacuum variable capacitor</td><td>Rated &ge;5 kV RMS, 10&ndash;500 pF range; Jennings, Cardwell, or equivalent</td></tr>
<tr><td>1</td><td>Coupling loop</td><td>Diameter 0.2&times; main loop; #12 AWG copper; used for 50 &Omega; feed</td></tr>
<tr><td>1</td><td>Capacitor drive mechanism</td><td>Reduction drive &ge;10:1 for fine tuning; motor drive for remote operation</td></tr>
<tr><td>1</td><td>Support frame</td><td>Non-conductive (PVC, wood, fiberglass); must resist loop torque</td></tr>
<tr><td>1</td><td>SO-239 connector</td><td>For coupling loop feed; coax shield bonds to coupling loop</td></tr>
</tbody>
</table>
{warn_hv()}

<h3 class="section-heading">3-2. DIMENSION FORMULAS</h3>
<div class="formula-block"><span class="formula-label">Loop inductance (circular loop, &mu;H)</span>L = 0.002 &times; D &times; (ln(4D/d) &minus; 2) where D = loop diameter (cm), d = conductor OD (cm)</div>
<div class="formula-block"><span class="formula-label">Resonant capacitance (&mu;F)</span>C = 1 / (4&pi;&sup2; &times; f&sup2; &times; L)</div>
<div class="formula-block"><span class="formula-label">Capacitor voltage at resonance (V peak)</span>V<sub>C</sub> = &radic;(2 &times; P &times; Q&sup2; &times; R<sub>r</sub>) where P = transmit power (W), Q = loop Q factor</div>
<div class="formula-block"><span class="formula-label">Example: 100 W, Q=300, R_r=0.08 &Omega;</span>V<sub>C</sub> &asymp; 8,500 V peak &mdash; REQUIRES VACUUM CAPACITOR</div>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
{warn_hv()}
<div class="procedure"><ol>
<li>Bend main loop tube to circular or square shape. Ensure all bends are smooth — kinks increase resistance.</li>
<li>Mount vacuum capacitor at top of loop. Solder or silver-braze all connections to minimum contact resistance. Verify &lt;0.001 &Omega; across each joint with milli-ohmmeter.</li>
<li>Construct coupling loop as a circle of diameter 0.2&times; main loop. Mount coupling loop at the bottom of the main loop, centered and in the same plane.</li>
<li>Connect coax braid and center conductor to coupling loop terminals. No balun required — the coupling loop is inherently isolated.</li>
<li>Mount assembly on non-conductive support. Ensure no metal within 1 m of loop conductor.</li>
<li>Install capacitor drive mechanism with &ge;10:1 reduction. Label rotation direction for capacitance increase.</li>
<li>Tag the capacitor: &ldquo;HIGH VOLTAGE — UP TO 12 kV AT 100W. DO NOT TOUCH WHEN TRANSMITTING.&rdquo;</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
{warn_hv()}
<div class="procedure"><ol>
<li>Begin with transmitter output &le;5 W for initial tuning. Do not apply full power until resonance is confirmed.</li>
<li>SOLT calibrate NanoVNA. Connect to coupling loop feedpoint.</li>
<li>Set NanoVNA sweep: target frequency ±10%.</li>
<li>Slowly rotate capacitor while watching SWR display. Resonance appears as a sharp dip (narrow bandwidth — sweep may miss it at first).</li>
<li>Narrow sweep span to ±2% once resonance found. Verify minimum SWR occurs at exact target frequency.</li>
<li>Record: f<sub>res</sub>, SWR<sub>min</sub>, 3 dB bandwidth (rotate cap slowly to find SWR = 1.4× minimum SWR points).</li>
<li>Calculate Q: Q = f<sub>res</sub> / BW<sub>3dB</sub>. High Q (&gt;200) indicates good construction quality.</li>
<li>Verify capacitor voltage is within rating before applying full power: V<sub>C</sub> = &radic;(2PQ&sup2;R<sub>r</sub>).</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Magnetic loops require tuning for every frequency change due to high Q and narrow bandwidth. Tune by rotating the variable capacitor until minimum SWR at the desired frequency. A good indicator of resonance is maximum receiver noise (on receive) or minimum reflected power (on transmit at low power). Fine-tune on the air by adjusting for maximum signal strength on a known station or beacon.</p>
<div class="tm-caution">
<span class="admonition-label">CAUTION</span>
Always tune at &le;5 W first. Verify SWR &lt;2:1 at low power before increasing to full power. A mis-tuned loop at full power reflects most power back to the transmitter.
</div>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table">
<caption>Acceptance Criteria — {ant['title'].title()}</caption>
<thead><tr><th>Parameter</th><th>Requirement</th><th>Pass/Fail</th></tr></thead>
<tbody>
<tr><td>SWR at resonance (after coupling loop optimization)</td><td>{ant['swr_target']}</td><td>____</td></tr>
<tr><td>Loop Q (minimum)</td><td>&ge;100 (at center band)</td><td>____</td></tr>
<tr><td>Bandwidth (3 dB)</td><td>Consistent with Q</td><td>____</td></tr>
<tr><td>Gain (NEC2)</td><td>{gain}</td><td>____</td></tr>
<tr><td>Efficiency</td><td>{eff}</td><td>____</td></tr>
<tr><td>Capacitor voltage (calculated)</td><td>&lt;80% of cap rating</td><td>____</td></tr>
</tbody>
</table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Loop inductance L (&mu;H, circular loop)</span>L = 0.002D[ln(4D/d) &minus; 2] (D, d in cm)</div>
<div class="formula-block"><span class="formula-label">Resonant capacitance</span>C = 1/(4&pi;&sup2;f&sup2;L) (&mu;F when L in &mu;H, f in MHz)</div>
<div class="formula-block"><span class="formula-label">Bandwidth</span>BW = f<sub>0</sub>/Q</div>
<div class="formula-block"><span class="formula-label">Efficiency</span>&eta; = R<sub>r</sub>/(R<sub>r</sub> + R<sub>loss</sub>)</div>
<div class="formula-block"><span class="formula-label">Capacitor peak voltage</span>V<sub>peak</sub> = &radic;(2&times;P) &times; Q/&radic;(R<sub>r</sub>)</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table">
<caption>Expected Measurements — {ant['title'].title()}</caption>
<thead><tr><th>Band</th><th>f<sub>res</sub></th><th>SWR</th><th>Q</th><th>BW (kHz)</th><th>Cap V (100W)</th></tr></thead>
<tbody>
<tr><td>40M</td><td>7.150 MHz</td><td>&lt;1.5:1</td><td>200&ndash;400</td><td>18&ndash;36 kHz</td><td>4&ndash;8 kV peak</td></tr>
<tr><td>20M</td><td>14.175 MHz</td><td>&lt;1.5:1</td><td>300&ndash;600</td><td>24&ndash;47 kHz</td><td>6&ndash;12 kV peak</td></tr>
<tr><td>15M</td><td>21.225 MHz</td><td>&lt;1.5:1</td><td>400&ndash;700</td><td>30&ndash;53 kHz</td><td>8&ndash;12 kV peak</td></tr>
</tbody>
</table>
"""


def gen_yagi(d, ant):
    """Content generator for Yagi/LPDA/directional antennas."""
    bands = ant["bands"]
    approach = ant["approach"]
    special = ant["special"]
    feed_z = ant["feed_z"]
    gain = ant["gain"]
    eff = ant["efficiency"]

    return f"""
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers design, construction, and calibration of the {ant['title'].title()} directional antenna for operation on {bands}. The antenna provides significant forward gain and front-to-back rejection for weak-signal, DX, and satellite work.</p>

<h3 class="section-heading">1-2. APPLICABLE REFERENCES</h3>
<ul>
<li>ARRL Antenna Book — Yagi and LPDA Chapters</li>
<li>Leeson, Physical Design of Yagi Antennas (ARRL, 1992)</li>
<li>NEC2 model: {d}.nec (in antenna directory)</li>
<li>FCC Part 97 — Antenna installation requirements</li>
</ul>

<h3 class="section-heading">1-3. SAFETY PRECAUTIONS</h3>
{warn_rf()}
<div class="tm-caution">
<span class="admonition-label">CAUTION — INSTALLATION</span>
Ensure mast/tower ground is bonded to earth ground with #6 AWG copper minimum. Disconnect rotator and feed line during electrical storms. Maintain safe distance from power lines &mdash; minimum 2&times; antenna wingspan plus 10 ft.
</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. PARASITIC COUPLING PHYSICS</h3>
<p>{approach.capitalize()}. Gain is achieved through parasitic coupling: the driver excites the reflector and director elements by near-field induction. The reflector (5% longer than the driven element) carries current that lags by approximately 160&deg; and re-radiates energy forward. Directors (5% shorter) carry current leading by approximately 140&deg; and focus energy forward. Each additional director adds approximately 1 dB of gain at optimal spacing (0.2&ndash;0.25&lambda;).</p>

<h3 class="section-heading">2-2. ELEMENT DESIGN RULES</h3>
<p>For a 3-element Yagi on VHF: driver = 0.473&lambda;, reflector = 0.505&lambda;, director = 0.440&lambda;. Spacing: reflector to driver = 0.2&lambda;, driver to director = 0.25&lambda;. These values produce forward gain of 7&ndash;8 dBd with front-to-back ratio of 20&ndash;25 dB. Gain estimate: {gain}. At UHF, dimensional tolerance is critical — elements must be within ±1 mm of design length for proper pattern formation.</p>

<h3 class="section-heading">2-3. FEED IMPEDANCE AND MATCHING</h3>
<p>Feed impedance: {feed_z}. The driven element impedance drops below 50 &Omega; when directors are added (typically 20&ndash;40 &Omega;). A gamma match, T-match, or delta match raises this to 50 &Omega;. The gamma match uses a parallel conductor tapped on the driven element to form an L-network; the shorting bar position and gamma rod length are adjusted for 50 &Omega; + j0 &Omega; at the design frequency.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; MATERIALS AND CONSTRUCTION</h2>
<h3 class="section-heading">3-1. BILL OF MATERIALS</h3>
<table class="tm-table">
<caption>Materials — {ant['title'].title()}</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>1</td><td>Boom</td><td>6061-T6 aluminum tubing, OD 1&ndash;1.5 in; length per design; non-conductive for VHF if elements through-bolted</td></tr>
<tr><td>Per design</td><td>Element stock</td><td>3/16&ndash;1/4 in aluminum rod for VHF; tape measure steel for portable UHF</td></tr>
<tr><td>1</td><td>Driven element</td><td>Folded or single dipole; see matching section below</td></tr>
<tr><td>1</td><td>Gamma match assembly</td><td>3/16 in rod, 6&ndash;10 in from center; 10&ndash;50 pF trimmer; beta match alternative</td></tr>
<tr><td>1</td><td>SO-239 or N-type connector</td><td>Weatherproof; mount at boom junction</td></tr>
</tbody>
</table>

<h3 class="section-heading">3-2. ELEMENT DIMENSION FORMULAS</h3>
<div class="formula-block"><span class="formula-label">Driven element half-length (in) for VHF</span>L<sub>de</sub> = 5616 / f<sub>MHz</sub> (inches, total)</div>
<div class="formula-block"><span class="formula-label">Reflector length (in)</span>L<sub>ref</sub> = L<sub>de</sub> &times; 1.05</div>
<div class="formula-block"><span class="formula-label">Director length (in)</span>L<sub>dir</sub> = L<sub>de</sub> &times; 0.95 (first director; adjust per NEC model)</div>
<div class="formula-block"><span class="formula-label">Refl-to-driven spacing</span>0.2&lambda; = 0.2 &times; (11811/f<sub>MHz</sub>) mm</div>
<div class="formula-block"><span class="formula-label">Driven-to-director spacing</span>0.25&lambda; = 0.25 &times; (11811/f<sub>MHz</sub>) mm</div>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
{warn_rf()}
<div class="procedure"><ol>
<li>Cut all elements to calculated lengths. Deburr cut ends. Label each element (reflector, driven, director-1, director-2 ...).</li>
<li>Mark element mounting positions on boom. Verify spacings with tape measure before drilling. Drilling error &gt;5 mm will degrade F/B ratio.</li>
<li>Mount parasitic elements through boom (metal boom) using nylon or PVC element mounts, or through metallic boom with direct element-to-boom contact (boom correction required for metallic boom).</li>
<li>Install driven element with insulated center block. Mount gamma match rod parallel to driver at 6&ndash;10 in from center; connect shorting bar and trimmer capacitor.</li>
<li>Attach coax feed: center pin to gamma rod, braid to boom ground. Route cable along boom toward rear to minimize feed-line distortion of pattern.</li>
<li>Perform initial SWR check per Chapter 5 before installing at height.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>SOLT calibrate NanoVNA at feed point of antenna (on bench, 3 ft above ground for initial check).</li>
<li>Set sweep: center frequency ±10%.</li>
<li>Adjust gamma match: slide shorting bar toward center to increase impedance; away from center to decrease. Adjust trimmer capacitor to null reactance.</li>
<li>Target: SWR &lt;1.5:1, R = 45&ndash;55 &Omega;, X = &minus;5 to +5 &Omega; at design frequency.</li>
<li>Once matched, verify SWR across full target band. Record: SWR at center, SWR at band edges, F/B ratio (compare field-strength meter readings front vs. back).</li>
<li>At final installed height (away from nearby objects), re-verify SWR — ground proximity affects impedance.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Gamma match adjustment: Moving the shorting bar changes the equivalent inductance in the match network; moving the trimmer changes reactance compensation. Typical procedure: set trimmer to midrange; slide shorting bar for minimum SWR; fine-adjust trimmer for minimum SWR; iterate. Director lengths can be adjusted ±2% to trade off gain vs. F/B ratio per NEC2 simulation.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table">
<caption>Acceptance Criteria — {ant['title'].title()}</caption>
<thead><tr><th>Parameter</th><th>Requirement</th><th>Pass/Fail</th></tr></thead>
<tbody>
<tr><td>SWR at design frequency</td><td>{ant['swr_target']}</td><td>____</td></tr>
<tr><td>Forward gain (NEC2)</td><td>{gain}</td><td>____</td></tr>
<tr><td>Front-to-back ratio</td><td>&ge;15 dB (3-element minimum)</td><td>____</td></tr>
<tr><td>Impedance at resonance</td><td>45&ndash;55 &Omega;, X &lt;±10 &Omega;</td><td>____</td></tr>
<tr><td>Efficiency</td><td>{eff}</td><td>____</td></tr>
</tbody>
</table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Wavelength in free space (m)</span>&lambda; = 300 / f<sub>MHz</sub></div>
<div class="formula-block"><span class="formula-label">Gain vs. boom length (estimate)</span>G(dBd) &asymp; 10 log<sub>10</sub>(2.5 &times; L<sub>boom</sub>/&lambda;) for L&gt;0.3&lambda;</div>
<div class="formula-block"><span class="formula-label">Boom correction factor for element length (metallic boom)</span>&Delta;L = &minus;0.0085 &times; d<sub>boom</sub> &times; d<sub>elem</sub> / &lambda;&sup2; (approximate)</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table">
<caption>Measured Results — {ant['title'].title()}</caption>
<thead><tr><th>Freq</th><th>SWR</th><th>R (&Omega;)</th><th>X (&Omega;)</th><th>F/B (dB)</th><th>Notes</th></tr></thead>
<tbody>
<tr><td>Band center</td><td>&lt;1.3:1</td><td>50</td><td>+2</td><td>20&ndash;25</td><td>After gamma match optimization</td></tr>
<tr><td>Band edge low</td><td>&lt;1.8:1</td><td>42</td><td>&minus;15</td><td>12&ndash;18</td><td>Pattern narrows at edges</td></tr>
<tr><td>Band edge high</td><td>&lt;1.8:1</td><td>58</td><td>+18</td><td>12&ndash;18</td><td>Director coupling increases</td></tr>
</tbody>
</table>
"""


def gen_endfed(d, ant):
    """Content generator for end-fed wire antennas."""
    bands = ant["bands"]
    approach = ant["approach"]
    special = ant["special"]
    feed_z = ant["feed_z"]
    gain = ant["gain"]
    eff = ant["efficiency"]

    return f"""
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers design, construction, and calibration of the {ant['title'].title()} for amateur radio operation on {bands}. End-fed wire antennas are popular for portable and emergency use because they require only one support point and can be deployed rapidly.</p>

<h3 class="section-heading">1-2. APPLICABLE REFERENCES</h3>
<ul>
<li>ARRL Antenna Book — End-Fed Antennas and Matching Networks</li>
<li>NEC2 model: {d}.nec (in antenna directory)</li>
<li>SOTA Antenna Notes — End-fed systems</li>
<li>FCC OET Bulletin 65 — RF Exposure Evaluation</li>
</ul>

<h3 class="section-heading">1-3. SAFETY PRECAUTIONS</h3>
{warn_rf()}
<div class="tm-caution">
<span class="admonition-label">CAUTION — END-FED CURRENTS ON COAX BRAID</span>
End-fed antennas without a proper counterpoise will drive common-mode current on the coax outer braid,
causing RF in the shack, SWR variations, and potential interference. Always install a 1:1 current choke
(sleeve choke or 8-turn coax on a Mix-31 toroid) at the feedpoint. Monitor for RF in the shack; if
present, add more choke or increase counterpoise length.
</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. END-FED MATCHING</h3>
<p>{approach.capitalize()}. An end-fed wire presents a high impedance at its end: ~2500&ndash;5000 &Omega; for a half-wave wire; ~1000&ndash;2000 &Omega; for a quarter-wave wire. This high impedance is transformed down to 50 &Omega; by the matching transformer or unun. Design values: {feed_z}. The transformation ratio n&sup2; = Z<sub>wire</sub>/Z<sub>coax</sub>; for a 49:1 unun, n = 7, so the wire impedance appears as 49&times;50 = 2450 &Omega; from the wire side.</p>

<h3 class="section-heading">2-2. RADIATION PATTERN</h3>
<p>An end-fed wire radiates in a pattern that varies with length and frequency. A half-wave wire at 10 ft height produces a bidirectional broadside pattern similar to a dipole. At harmonic frequencies the pattern has multiple lobes and nulls. Gain per design: {gain}. The pattern is affected by the run of the feed line if common-mode current is not controlled.</p>

<h3 class="section-heading">2-3. COUNTERPOISE REQUIREMENTS</h3>
<p>A counterpoise (short wire or coax braid) is required as a reference conductor for the feed voltage to work against. Without a counterpoise the coax braid serves as the counterpoise, causing RF current on the braid. Recommended counterpoise: 0.05&lambda; minimum length wire from the unun ground terminal, or a 1:1 choke isolating the antenna current from the coax outer.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; MATERIALS AND CONSTRUCTION</h2>
<h3 class="section-heading">3-1. BILL OF MATERIALS</h3>
<table class="tm-table">
<caption>Materials — {ant['title'].title()}</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>1</td><td>Matching transformer (unun)</td><td>49:1 unun for EFHW (binocular core BN-43-2402 or FT-140-43); 9:1 unun for random wire (FT-140-43)</td></tr>
<tr><td>1 length</td><td>Antenna wire</td><td>#22&ndash;#18 AWG stranded copper; length per design</td></tr>
<tr><td>1</td><td>End insulator</td><td>UV-stable; rated for wire tension</td></tr>
<tr><td>1</td><td>Counterpoise wire</td><td>#22 AWG; minimum 0.05&lambda; at lowest operating frequency</td></tr>
<tr><td>1</td><td>Enclosure for transformer</td><td>UV-stable ABS; seal with silicone for waterproofing</td></tr>
<tr><td>1</td><td>SO-239 connector</td><td>Mount on enclosure; connect to coax feed line</td></tr>
</tbody>
</table>

<h3 class="section-heading">3-2. WIRE LENGTHS</h3>
<div class="formula-block"><span class="formula-label">EFHW wire length (feet) at design frequency</span>L = 468 / f<sub>MHz</sub></div>
<div class="formula-block"><span class="formula-label">EFQW wire length (feet)</span>L = 234 / f<sub>MHz</sub></div>
<div class="formula-block"><span class="formula-label">Avoid resonant random wire lengths (avoid these values for 40M EFRW)</span>Avoid 11, 22, 33, 44, 66, 88, 110 ft — these are resonant multiples on common bands</div>
<div class="formula-block"><span class="formula-label">Recommended EFRW lengths (ft)</span>29, 35.5, 41, 58, 71 — not resonant on any common HF band</div>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
{warn_rf()}
<div class="procedure"><ol>
<li>Wind matching transformer per design (49:1 unun: 3 trifilar turns primary on BN-43-2402 binocular core, or 2 turns primary and 14 turns secondary on FT-140-43 single core).</li>
<li>Seal transformer in weatherproof enclosure. Route wire terminal out one end, coax SO-239 on side.</li>
<li>Attach antenna wire to transformer wire terminal. Cut wire 5% long for trimming.</li>
<li>Attach counterpoise wire (0.05&lambda; minimum) to transformer ground/coax-braid terminal. Tape counterpoise away from antenna wire.</li>
<li>Deploy antenna wire: raise far end as high as possible, at least 20 ft for HF operation. Attach end insulator and support halyard.</li>
<li>Run coax from SO-239 to station. Install common-mode choke (Mix-31 toroid, 8 turns) at shack entry.</li>
<li>Perform SWR sweep per Chapter 5.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>SOLT calibrate NanoVNA at end of feed line (shack end) with the antenna deployed at operating height.</li>
<li>Set sweep: target band ±15%.</li>
<li>Connect NanoVNA. Record SWR across band. Resonance appears as dip in SWR (may be very sharp for EFHW).</li>
<li>If SWR dip is at wrong frequency: trim wire end for higher frequency (too long), or add wire for lower frequency (too short). Change ~6 in for ~50 kHz shift at 40M.</li>
<li>Verify SWR on harmonics for multiband use (EFHW is harmonically resonant at 2f, 3f, ...).</li>
<li>Check common-mode current: touch coax braid — if RF is felt, add choke at feedpoint.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Trim wire for resonance. For multiband EFHW, the 49:1 unun presents high impedance at the design frequency; SWR on harmonics is naturally low if the wire is properly trimmed at the fundamental. If SWR is acceptable at fundamental but poor at harmonics, adjust ATU or add a series capacitor (50&ndash;100 pF) in the counterpoise for harmonic correction.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table">
<caption>Acceptance Criteria</caption>
<thead><tr><th>Parameter</th><th>Requirement</th><th>Pass/Fail</th></tr></thead>
<tbody>
<tr><td>SWR at design frequency</td><td>{ant['swr_target']}</td><td>____</td></tr>
<tr><td>Common-mode current on coax</td><td>Not perceptible at feed end</td><td>____</td></tr>
<tr><td>Gain (NEC2)</td><td>{gain}</td><td>____</td></tr>
<tr><td>Efficiency</td><td>{eff}</td><td>____</td></tr>
</tbody>
</table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Wire length for EFHW (feet)</span>L = 468 / f<sub>MHz</sub></div>
<div class="formula-block"><span class="formula-label">49:1 unun impedance transformation</span>Z<sub>wire</sub> = 49 &times; Z<sub>coax</sub> = 49 &times; 50 = 2450 &Omega;</div>
<div class="formula-block"><span class="formula-label">9:1 unun impedance transformation</span>Z<sub>wire</sub> = 9 &times; 50 = 450 &Omega;</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table">
<caption>Typical Measurements — {ant['title'].title()}</caption>
<thead><tr><th>Band</th><th>SWR</th><th>R (&Omega;)</th><th>X (&Omega;)</th><th>Notes</th></tr></thead>
<tbody>
<tr><td>40M (design)</td><td>&lt;2.0:1</td><td>40&ndash;65</td><td>&plusmn;20</td><td>After wire trimming</td></tr>
<tr><td>20M (2nd harmonic)</td><td>&lt;2.5:1</td><td>30&ndash;80</td><td>&plusmn;30</td><td>Harmonically resonant</td></tr>
<tr><td>15M (3rd harmonic)</td><td>&lt;3:1</td><td>25&ndash;90</td><td>&plusmn;40</td><td>May need ATU</td></tr>
</tbody>
</table>
"""


def gen_receiving(d, ant):
    """Content generator for receiving / low-noise directional antennas."""
    bands = ant["bands"]
    approach = ant["approach"]
    special = ant["special"]
    feed_z = ant["feed_z"]
    gain = ant["gain"]
    eff = ant["efficiency"]

    return f"""
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers the {ant['title'].title()} for low-noise directional reception on {bands}. This is a receive-only antenna. The primary performance metrics are front-to-back ratio, noise figure, and signal-to-noise ratio improvement over a reference dipole. Efficiency in the conventional sense does not apply to receiving antennas; sensitivity and directivity are the figures of merit.</p>
{note_receiving()}

<h3 class="section-heading">1-2. APPLICABLE REFERENCES</h3>
<ul>
<li>ON4UN&rsquo;s Low Band DXing — Receiving Antenna Chapters</li>
<li>K9AY Loop Documentation (Gary Breed, QST October 1997)</li>
<li>NEC2 model: {d}.nec (in antenna directory)</li>
<li>Beverage Antennas for Amateur Radio (ARRL Antenna Book)</li>
</ul>

<h3 class="section-heading">1-3. SAFETY PRECAUTIONS</h3>
<div class="tm-note">
<span class="admonition-label">NOTE</span>
This antenna is receive-only. Do NOT connect to a transmitter. The matching transformer and
termination resistors are sized for received signal levels only (&mu;V range). Transmitter power
will destroy the transformer, termination components, and possibly the receiver input.
</div>

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. DIRECTIVITY AND NOISE REJECTION</h3>
<p>{approach.capitalize()}. Receive-only antennas exploit directivity (front-to-back ratio) and aperture to separate signals from noise. A terminated long-wire or loop antenna achieves a cardioid or kidney-shaped pattern with deep null in one direction: F/B ratio typically 15&ndash;25 dB. This null can be steered toward interference sources (power-line noise, broadcast QRM) to dramatically improve signal-to-noise ratio on weak HF signals.</p>

<h3 class="section-heading">2-2. NOISE FIGURE AND SNR</h3>
<p>At HF below 30 MHz, external noise (atmospheric, man-made) dominates over receiver noise figure. A receiving antenna with poor efficiency but good directivity can outperform a high-gain antenna pointed at a noise source. The key metric is signal-to-noise ratio (SNR) improvement over the reference antenna (typically a 40M dipole), not absolute signal level. An SNR improvement of 10&ndash;20 dB (1&ndash;2 S-units) on a target station makes marginal copy copy into solid copy.</p>

<h3 class="section-heading">2-3. TERMINATION RESISTANCE</h3>
<p>Most directional receive antennas use a termination resistor (typically 500&ndash;900 &Omega;) to absorb backward-traveling waves and prevent re-radiation from the far end. This resistance determines the F/B ratio and the wave velocity factor along the antenna. {special.split(';')[0].strip() if ';' in special else special}. The termination must be a non-inductive resistor (carbon composition or metal film; not wirewound) mounted in a weatherproof housing.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; MATERIALS AND CONSTRUCTION</h2>
<h3 class="section-heading">3-1. BILL OF MATERIALS</h3>
<table class="tm-table">
<caption>Materials — {ant['title'].title()}</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>1 length</td><td>Antenna wire</td><td>#22&ndash;#18 AWG; length per design</td></tr>
<tr><td>1</td><td>Termination resistor</td><td>560&ndash;1000 &Omega; non-inductive (carbon film or metal film); weatherproofed; ¼ W sufficient for receive</td></tr>
<tr><td>1</td><td>Feed transformer</td><td>9:1 or step-up per design; FT-82-43 or FT-140-43 core; wound for low insertion loss</td></tr>
<tr><td>1</td><td>Support rope/stake</td><td>Non-conductive; anchor at wire ends</td></tr>
<tr><td>1</td><td>Coax feed line</td><td>RG-174 or RG-58 sufficient for receive; minimize length and bends</td></tr>
</tbody>
</table>

<h3 class="section-heading">3-2. KEY DIMENSIONS</h3>
<div class="formula-block"><span class="formula-label">Beverage wire length (m) for 160M operation</span>L &asymp; 0.75&lambda; to 2&lambda; at target frequency; longer is better for gain</div>
<div class="formula-block"><span class="formula-label">Height above ground</span>1&ndash;2 m typical; low is acceptable as long as not in contact with vegetation</div>
<div class="formula-block"><span class="formula-label">Termination resistance (optimum for Beverage)</span>R<sub>T</sub> = 120 &Omega; &times; ln(2h/a) where h = height (m), a = wire radius (m)</div>
<div class="formula-block"><span class="formula-label">K9AY loop size (ft, each side)</span>20 ft per side; direction of maximum response along loop axis</div>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
<div class="procedure"><ol>
<li>Deploy antenna wire from feed end in desired direction of maximum received signal. For Beverage: wire runs toward the desired signal direction. For K9AY: loop lies in vertical plane; null is broadside to loop axis.</li>
<li>Install termination resistor enclosure at far end of wire. Terminate wire to top of resistor; bottom of resistor to ground stake. Verify resistor value with ohmmeter before sealing enclosure.</li>
<li>Install feed transformer at near end. Wire terminal to antenna; ground to earth stake (Beverage: ground at feed end). Connect output to coax.</li>
<li>Route coax toward receiver. Ensure at least 5 m horizontal separation from any transmitting antenna to avoid overloading the receive preamp during transmit.</li>
<li>At receiver end, install T/R relay or receive antenna switch for TX/RX switching (this antenna must be disconnected from receiver before transmitting).</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
<div class="procedure"><ol>
<li>Connect receiving antenna to receiver. Set receiver to target band (40M or 160M). Use CW or AM mode with narrow filter to minimize noise bandwidth.</li>
<li>Listen for a strong reference station on a known bearing. Note S-meter reading.</li>
<li>Rotate antenna (K9AY/frame loop) or note signal versus antenna direction. Record signal level in each direction.</li>
<li>Adjust termination resistance: vary R<sub>T</sub> by ±20% and note effect on F/B ratio. Optimize for maximum ratio, not maximum signal level.</li>
<li>Use TinySA as signal tracer: inject a −40 dBm signal at antenna terminal; verify output at receiver connector is &ge;−80 dBm (insertion loss &le;40 dB is acceptable for receive-only).</li>
<li>Record: insertion loss, F/B at 40M, 80M, 160M; SNR vs. reference antenna on noise floor test (no signals, 40M, 2100 UTC).</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Adjust termination resistance for maximum F/B ratio using the on-air method: rotate array toward a known noise source (AC power line, broadcast station in null direction). Vary the termination resistance in small steps while monitoring noise level. Minimum noise in the null direction corresponds to optimal termination resistance. Fine-tune the feed transformer coupling for minimum insertion loss while maintaining impedance match.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table">
<caption>Acceptance Criteria — {ant['title'].title()}</caption>
<thead><tr><th>Parameter</th><th>Requirement</th><th>Pass/Fail</th></tr></thead>
<tbody>
<tr><td>SWR at receiver input</td><td>{ant['swr_target']}</td><td>____</td></tr>
<tr><td>Front-to-back ratio (measured)</td><td>&ge;15 dB (minimum useful)</td><td>____</td></tr>
<tr><td>Insertion loss (antenna term. to coax)</td><td>&le;40 dB</td><td>____</td></tr>
<tr><td>SNR vs. reference dipole on target band</td><td>Improvement &ge;3 dB on noise floor</td><td>____</td></tr>
<tr><td>T/R isolation (if switching)</td><td>&ge;60 dB during transmit</td><td>____</td></tr>
</tbody>
</table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Beverage wave velocity (m/s)</span>v = c / &radic;(1 + (60&sigma;/f)) where &sigma; = ground conductivity (S/m), f in Hz</div>
<div class="formula-block"><span class="formula-label">Front-to-back ratio (Beverage)</span>F/B (dB) &asymp; 20 log<sub>10</sub>[1 + e<sup>2&pi;L/&lambda;</sup>]</div>
<div class="formula-block"><span class="formula-label">Noise figure of antenna (approximate, HF)</span>NF<sub>ant</sub> = 10 log<sub>10</sub>(T<sub>sky</sub>/290) (always negative for external-noise-dominated HF)</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table">
<caption>Measured Performance — {ant['title'].title()}</caption>
<thead><tr><th>Band</th><th>F/B Ratio (dB)</th><th>SWR</th><th>SNR Improvement</th><th>Notes</th></tr></thead>
<tbody>
<tr><td>160M</td><td>18&ndash;22</td><td>&lt;2:1</td><td>+10 dB vs dipole</td><td>Best at night, 1.835 MHz</td></tr>
<tr><td>80M</td><td>16&ndash;20</td><td>&lt;2:1</td><td>+8 dB vs dipole</td><td>3.5 MHz region</td></tr>
<tr><td>40M</td><td>14&ndash;18</td><td>&lt;2:1</td><td>+6 dB vs dipole</td><td>7.0 MHz DX window</td></tr>
</tbody>
</table>
"""


def gen_special(d, ant):
    """Content generator for special/unique antenna types."""
    bands = ant["bands"]
    approach = ant["approach"]
    special = ant["special"]
    feed_z = ant["feed_z"]
    gain = ant["gain"]
    eff = ant["efficiency"]

    # Special handling for magnetic loop at VHF (KMR-400)
    is_vhf_loop = d == "kmr_400_magnetic_loop"

    extra_warn = warn_hv() if is_vhf_loop else ""

    return f"""
<h2 class="chapter-heading" id="ch1">CHAPTER 1 &mdash; GENERAL INFORMATION</h2>
<h3 class="section-heading">1-1. SCOPE</h3>
<p>This manual covers the {ant['title'].title()}, a specialized antenna for operation on {bands}. Design approach: {approach}.</p>

<h3 class="section-heading">1-2. APPLICABLE REFERENCES</h3>
<ul>
<li>ARRL Antenna Book — Specialized and Experimental Antennas</li>
<li>NEC2 model: {d}.nec (in antenna directory)</li>
<li>FCC OET Bulletin 65 — RF Exposure Evaluation</li>
</ul>

<h3 class="section-heading">1-3. SAFETY PRECAUTIONS</h3>
{warn_rf()}
{extra_warn}

<h2 class="chapter-heading" id="ch2">CHAPTER 2 &mdash; THEORY OF OPERATION</h2>
<h3 class="section-heading">2-1. DESIGN PRINCIPLES</h3>
<p>{approach.capitalize()}. Key parameters: {special}.</p>

<h3 class="section-heading">2-2. RADIATION CHARACTERISTICS</h3>
<p>Gain: {gain}. Efficiency: {eff}. Feed impedance: {feed_z}. The pattern and polarization depend on the specific antenna geometry; consult the NEC2 model for accurate polar plots and gain/elevation data.</p>

<h3 class="section-heading">2-3. PROPAGATION APPLICATION</h3>
<p>This antenna is optimized for its specific application (see subtitle). Operating it outside the designed frequency range or in a different orientation from the NEC2 model will result in degraded performance — consult the simulation before making substitutions.</p>

<h2 class="chapter-heading" id="ch3">CHAPTER 3 &mdash; MATERIALS AND CONSTRUCTION</h2>
<h3 class="section-heading">3-1. BILL OF MATERIALS</h3>
<table class="tm-table">
<caption>Materials — {ant['title'].title()}</caption>
<thead><tr><th>Qty</th><th>Item</th><th>Specification</th></tr></thead>
<tbody>
<tr><td>Per NEC model</td><td>Conductor elements</td><td>Dimensions from NEC2 wire list; use copper or aluminum per design frequency</td></tr>
<tr><td>1</td><td>Feed connector</td><td>SO-239 or N-type; {feed_z} system</td></tr>
<tr><td>1</td><td>Support hardware</td><td>UV-stable; sized for operating environment (wind, ice load)</td></tr>
<tr><td>As needed</td><td>Feed line</td><td>RG-213 or LMR-400; match to feed impedance {feed_z}</td></tr>
</tbody>
</table>

<h3 class="section-heading">3-2. CRITICAL DIMENSIONS</h3>
<div class="formula-block"><span class="formula-label">Primary dimension formula (wavelengths)</span>&lambda; (m) = 300 / f<sub>MHz</sub></div>
<div class="formula-block"><span class="formula-label">Design-specific parameters</span>{special}</div>
<p>All critical dimensions are embedded in the NEC2 model wire card (GW) definitions. Extract from {d}.nec for precise construction reference.</p>

<h2 class="chapter-heading" id="ch4">CHAPTER 4 &mdash; ASSEMBLY PROCEDURES</h2>
{warn_rf()}
<div class="procedure"><ol>
<li>Extract all element lengths and spacings from NEC2 model file {d}.nec (GW card dimensions). Convert from meters to inches or feet for construction.</li>
<li>Fabricate each element to ±1 mm tolerance (VHF/UHF) or ±0.5 in (HF). Label elements before assembly.</li>
<li>Assemble support structure. Verify element positions match NEC model geometry. Use non-conductive fasteners where element isolation is required.</li>
<li>Install feed connector at designed feedpoint location. Install any required matching network (gamma match, balun, transformer).</li>
<li>Connect feed line. Install common-mode choke (Mix-31 toroid) at feedpoint. Route feed line to minimize coupling with antenna elements.</li>
<li>Perform SWR measurement per Chapter 5 before installing at full height.</li>
</ol></div>

<h2 class="chapter-heading" id="ch5">CHAPTER 5 &mdash; CALIBRATION PROCEDURE</h2>
{note_nec()}
<div class="procedure"><ol>
<li>SOLT calibrate NanoVNA at feedpoint (antenna connector).</li>
<li>Set sweep range: ±10% of design center frequency.</li>
<li>Connect NanoVNA to antenna. Record SWR minimum and frequency, R + jX at resonance.</li>
<li>Compare measured impedance to NEC2 predicted values. Investigate discrepancies &gt;20%.</li>
<li>Verify SWR &le;{ant['swr_target'].replace('< ', '')} at operating frequency. If not, adjust matching per Chapter 6.</li>
<li>For gain verification: compare received signal strength (using TinySA as receiver) from a known beacon vs. reference antenna.</li>
</ol></div>

<h2 class="chapter-heading" id="ch6">CHAPTER 6 &mdash; TUNING AND ADJUSTMENT</h2>
<p>Element lengths can be adjusted by ±2% of design length to shift resonance and optimize impedance. For matching network adjustment: follow gamma-match, delta-match, or transformer-tap procedure per the specific implementation. Refer to NEC2 model for sensitivity analysis — the model can be re-run at ±2% element length to determine which element most affects resonance and F/B ratio.</p>

<h2 class="chapter-heading" id="ch7">CHAPTER 7 &mdash; VERIFICATION</h2>
<table class="tm-table">
<caption>Acceptance Criteria — {ant['title'].title()}</caption>
<thead><tr><th>Parameter</th><th>Requirement</th><th>Pass/Fail</th></tr></thead>
<tbody>
<tr><td>SWR at design frequency</td><td>{ant['swr_target']}</td><td>____</td></tr>
<tr><td>Resonant frequency</td><td>Within ±1% of design</td><td>____</td></tr>
<tr><td>Gain (NEC2 or field test)</td><td>{gain}</td><td>____</td></tr>
<tr><td>Efficiency</td><td>{eff}</td><td>____</td></tr>
</tbody>
</table>

<h2 class="chapter-heading" id="appa">APPENDIX A &mdash; CALCULATIONS AND FORMULAS</h2>
<div class="formula-block"><span class="formula-label">Wavelength in free space</span>&lambda; (m) = 300 / f<sub>MHz</sub></div>
<div class="formula-block"><span class="formula-label">Effective gain formula (over isotropic)</span>G<sub>dBi</sub> = 10 log<sub>10</sub>(4&pi; &times; A<sub>eff</sub> / &lambda;&sup2;) where A<sub>eff</sub> = effective aperture (m&sup2;)</div>
<div class="formula-block"><span class="formula-label">Special parameters</span>{special}</div>

<h2 class="chapter-heading" id="appb">APPENDIX B &mdash; EXAMPLE RESULTS</h2>
<table class="tm-table">
<caption>Expected Measurements — {ant['title'].title()}</caption>
<thead><tr><th>Freq</th><th>SWR</th><th>R (&Omega;)</th><th>X (&Omega;)</th><th>Notes</th></tr></thead>
<tbody>
<tr><td>Design center</td><td>{ant['swr_target']}</td><td>~50</td><td>~0</td><td>After full match/tune</td></tr>
<tr><td>Band edge low</td><td>&lt;2.5:1</td><td>~40</td><td>&le;&minus;20</td><td>Capacitive below resonance</td></tr>
<tr><td>Band edge high</td><td>&lt;2.5:1</td><td>~40</td><td>&ge;+20</td><td>Inductive above resonance</td></tr>
</tbody>
</table>
"""


# ─────────────────────────────────────────────────────────────────────────────
# CONTENT DISPATCH TABLE
# ─────────────────────────────────────────────────────────────────────────────
GENERATORS = {
    "dipole":    gen_dipole,
    "vertical":  gen_vertical,
    "loop":      gen_loop,
    "magloop":   gen_magloop,
    "yagi":      gen_yagi,
    "endfed":    gen_endfed,
    "receiving": gen_receiving,
    "special":   gen_special,
}


# ─────────────────────────────────────────────────────────────────────────────
# HTML PAGE BUILDER
# ─────────────────────────────────────────────────────────────────────────────

def build_tm_page(d, ant, content_html):
    tm_num = ant["tm_num"]
    title = ant["title"]
    subtitle = ant["subtitle"]
    page_title = f"{tm_num} {title}"

    return f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{page_title} - Merv's Brain Dump</title>
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

{NAV_HTML}

            <main>
                <div class="tm-cover">
                    <div class="tm-classification">UNCLASSIFIED</div>
                    <div class="tm-number">{tm_num}</div>
                    <div class="tm-title">{title}</div>
                    <div class="tm-subtitle">{subtitle}</div>
                    <div class="tm-meta">
                        Prepared by: Mervyn Martin, KO6NNH<br>
                        Merced, California &nbsp;&bull;&nbsp; 26 May 2026<br>
                        <em>Amateur Radio / Antenna Engineering &mdash; Not for commercial use</em>
                    </div>
                </div>

                <div class="tm-toc">
                    <h2>Table of Contents</h2>
                    <ol>
            <li><a href="#ch1">Chapter 1 &mdash; General Information</a></li>
            <li><a href="#ch2">Chapter 2 &mdash; Theory of Operation</a></li>
            <li><a href="#ch3">Chapter 3 &mdash; Materials and Construction</a></li>
            <li><a href="#ch4">Chapter 4 &mdash; Assembly Procedures</a></li>
            <li><a href="#ch5">Chapter 5 &mdash; Calibration Procedure</a></li>
            <li><a href="#ch6">Chapter 6 &mdash; Tuning and Adjustment</a></li>
            <li><a href="#ch7">Chapter 7 &mdash; Verification</a></li>
            <li><a href="#appa">Appendix A &mdash; Calculations and Formulas</a></li>
            <li><a href="#appb">Appendix B &mdash; Example Results</a></li>
                    </ol>
                </div>

{content_html}

                <div class="back-link">
                    <a href="../index.html">&larr; Back to Antenna Library</a>
                    &nbsp;&nbsp;&bull;&nbsp;&nbsp;
                    <a href="index.html">&larr; Back to {title.title()} Overview</a>
                </div>
            </main>
        </div>
    </div>

{FOOTER_HTML}
</body>
</html>"""


# ─────────────────────────────────────────────────────────────────────────────
# INDEX.HTML UPDATER  — add tm.html link near top of <main>
# ─────────────────────────────────────────────────────────────────────────────

def update_index_html(index_path, ant):
    """Insert a 'View Technical Manual' link into index.html after <main>."""
    try:
        text = index_path.read_text(encoding="utf-8")
    except Exception as e:
        return f"  SKIP index update ({e})"

    # Already has the link?
    if "tm.html" in text:
        return "  index.html already has tm.html link — skipped"

    # Find <main> open tag; insert link block right after it
    marker = "<main>"
    if marker not in text:
        return "  SKIP index update (no <main> tag found)"

    link_block = (
        f'\n<p><a href="tm.html">'
        f'&#9654; View Technical Manual ({ant["tm_num"]})</a></p>\n'
    )
    new_text = text.replace(marker, marker + link_block, 1)
    try:
        index_path.write_text(new_text, encoding="utf-8")
        return "  index.html updated with TM link"
    except Exception as e:
        return f"  ERROR updating index.html: {e}"


# ─────────────────────────────────────────────────────────────────────────────
# MAIN
# ─────────────────────────────────────────────────────────────────────────────

def main():
    generated = 0
    errors = []

    for d, ant in sorted(ANTENNA_DATA.items()):
        ant_dir = BASE_DIR / d
        if not ant_dir.is_dir():
            errors.append(f"MISSING DIRECTORY: {d}")
            continue

        category = ant.get("category", "special")
        gen_func = GENERATORS.get(category, gen_special)

        # Generate technical content
        content = gen_func(d, ant)

        # Build full HTML page
        html = build_tm_page(d, ant, content)

        # Write tm.html
        tm_path = ant_dir / "tm.html"
        try:
            tm_path.write_text(html, encoding="utf-8")
            generated += 1
            print(f"[OK] {ant['tm_num']} — {d}/tm.html ({len(html):,} bytes)")
        except Exception as e:
            errors.append(f"WRITE ERROR {d}: {e}")
            print(f"[ERROR] {d}: {e}")
            continue

        # Update index.html
        index_path = ant_dir / "index.html"
        if index_path.exists():
            msg = update_index_html(index_path, ant)
            print(f"       {msg}")
        else:
            print(f"       No index.html found in {d}")

    print(f"\n{'='*60}")
    print(f"COMPLETE: {generated}/{len(ANTENNA_DATA)} TM files generated.")
    if errors:
        print(f"ERRORS ({len(errors)}):")
        for e in errors:
            print(f"  {e}")
    else:
        print("No errors.")
    print("="*60)


if __name__ == "__main__":
    main()
