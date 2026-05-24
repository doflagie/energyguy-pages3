# TM-WJP-001 Rev A
## TECHNICAL MANUAL — WEBBING J-POLE ANTENNA SYSTEM
### 2-Inch Conductor Tape on 3-Inch Lawn Chair Webbing
### HF / VHF / UHF Portable Field Antenna

---

**DISTRIBUTION:** Unrestricted

**APPLICABLE DOCUMENTS:**
- TM-WJP-CAL-001 — Webbing J-Pole Dimensions, All Bands
- TM-WJP-CAL-002 — Tape Transmission Line Parameters
- TM-WJP-CAL-003 — Feed Point Location Tables
- TM-WJP-NEC-001 through 009 — NEC2 Antenna Models
- TM-WJP-SCH-001 — Construction Diagrams
- TM-WJP-SCH-002 — Feed Point Detail

---

## TABLE OF CONTENTS

1. Purpose and Scope
2. Theory of Operation
3. Design Parameters and Formulas
4. Material Selection
5. Construction Procedure
6. Feed Point Installation
7. Deployment Configurations
8. Manual Tuning Procedures
9. NEC Model Reference
10. Band-by-Band Data and Operating Notes
11. Troubleshooting
12. Appendix A — Complete Dimension Tables (Metric and Imperial)
13. Appendix B — Materials and Sources
14. Appendix C — Velocity Factor and Dielectric Theory
15. Appendix D — Common-Mode Choke Winding Reference

---

## CHAPTER 1 — PURPOSE AND SCOPE

### 1.1 Purpose

This manual covers the design, construction, calibration, and field use of the Webbing J-Pole Antenna System. This antenna is a portable, rollup J-pole made from hardware-store materials: 3-inch nylon lawn chair webbing as the structural substrate and 2-inch adhesive conductor tape (aluminum or copper) as the radiating element and matching stub.

The design covers amateur radio operation from 160 meters (1.8 MHz) through 23 centimeters (1.3 GHz), with practical portable designs for 40m through 70cm as the primary operating range.

### 1.2 Scope

This manual covers:
- Antenna theory applicable to this construction
- Complete design calculations for all HF/VHF/UHF amateur bands
- Step-by-step construction from raw materials
- Feed point connector installation for portable (clip) and permanent (soldered) use
- Deployment techniques for field use
- Manual SWR tuning procedure
- NEC2 antenna model reference and operating guidance

### 1.3 Design Philosophy

The Webbing J-Pole was developed to fill the need for a portable, broadband antenna that:
- Weighs less than 500 grams including coax for HF operation
- Rolls to a compact package fitting in a daypack
- Requires no tools to deploy (HF) or minimal soldering (VHF/UHF)
- Covers all amateur bands with separate dedicated antennas per band
- Uses materials available at any hardware store, home goods store, or online retailer
- Can be field-fabricated in under 30 minutes if the original is lost or damaged

### 1.4 Limitations

1. **Low stub impedance (≈13Ω):** The face-to-face conductor geometry produces a very low characteristic impedance matching stub. This works but requires the feed point to be very close to the shorted end and produces slightly narrower bandwidth than a standard 300Ω J-pole. The optional foam spacer modification (Section 4.3) corrects this.

2. **Velocity factor uncertainty:** Nylon webbing varies in weave density and composition. The published velocity factor of 0.95 is a reasonable starting estimate; final tuning will be required.

3. **Power limit:** Conductor tape adhesive softens above 80°C. At HF with high SWR or high-Q loading, current concentration could heat narrow areas. Do not exceed 100W continuous into mismatched loads. For 100W portable use, maintain SWR < 2:1.

4. **UHF accuracy:** Above 70cm (446 MHz), the tape width (50.8mm) becomes a significant fraction of a wavelength. Antenna behavior deviates from simple J-pole theory. Use NEC models as approximate guides and finalize dimensions with NanoVNA measurement.

---

## CHAPTER 2 — THEORY OF OPERATION

### 2.1 The J-Pole Antenna

The J-pole (correctly: J-match antenna, or J-antenna) is an end-fed half-wave antenna matched to coaxial feedline by a quarter-wave parallel-line stub. It was developed for early aviation use where a compact vertically polarized antenna was needed without ground radials.

**Physical description:**
- A half-wave (λ/2) radiating element extending from one end of a quarter-wave (λ/4) parallel-line matching section
- The matching section is shorted at its far end
- The coaxial feedline connects across the parallel lines at a point determined by the feed impedance (50Ω for most coax)

**Why it works:**
The λ/2 element is fed at one of its current nodes (ends), where impedance is theoretically infinite (several thousand ohms in practice due to conductor width). The λ/4 parallel stub acts as an impedance transformer, stepping this very high impedance down to 50Ω at the correct tap position.

### 2.2 Impedance Transformation in the Stub

The stub forms a quarter-wave transmission line resonator. At the shorted end, impedance is zero (current maximum). At the open end (where the radiating element connects), impedance is very high. At intermediate points, impedance varies continuously.

The impedance at a distance *d* from the short circuit:

```
Z(d) = jZ₀ × tan(βd)
```

where β = 2π/λ_electrical and Z₀ is the stub characteristic impedance.

When the feed point is placed at position d, the resistive part of the antenna impedance (transformed through the stub) equals the feed impedance (50Ω). The reactive part is cancelled by the stub's reactance at that same point.

The optimal feed distance from the short circuit:

```
d_feed ≈ (1/β) × arcsin(√(Z_feed / R_ant))
```

where R_ant is the end-fed radiation resistance of the half-wave element (approximately 1500–3000Ω for a wide tape conductor).

### 2.3 The Flat-Tape Conductor

Conventional J-poles use round wire or tubing. This design uses flat tape, which changes several parameters:

**Effective radius:** A flat tape of width *w* behaves electrically like a round wire of radius:
```
r_eq = w / (2π)
```

For 2-inch (50.8mm) tape: r_eq = 50.8 / (2π) = 8.09mm

This is a large effective radius. Effects:
- Higher capacitance per unit length (distributed loading)
- Lower radiation resistance at the element end (slightly higher than for thin wire, not lower — the fat conductor reduces the standing-wave ratio along the element)
- Wider bandwidth due to lower Q of the radiating element
- Better mechanical stability in wind

**Equivalent gain:** A J-pole using a fat conductor approaches the theoretical 0 dBd (same gain as a half-wave dipole in free space) with less sensitivity to conductor diameter than thin-wire antennas. The wide tape improves efficiency at high frequencies where skin effect would otherwise concentrate current on the surface of a small conductor.

### 2.4 Stub Characteristic Impedance — Parallel-Plate Line

Two wide flat conductors separated by a thin dielectric form a parallel-plate transmission line. For conductor width *w*, separation *d*, and dielectric permittivity εᵣ:

```
Z₀ = (η₀ / √εᵣ) × (d / w)    [valid when w >> d]
```

For this construction: w = 50.8mm, d = 3mm (webbing), εᵣ = 3.0 (nylon):

```
Z₀ = (377 / √3.0) × (3 / 50.8) = 217.7 × 0.0591 = 12.9Ω
```

This is very low compared to the 50Ω target feed impedance. The matching is still achievable — the feed tap point will be proportionally closer to the shorted end.

**Optional improvement:** Insert a strip of closed-cell foam (camping pad material, ε_r ≈ 1.2) between the two tape strips in the stub section, increasing separation to ~25mm:

```
Z₀_foam = (377 / √1.2) × (25 / 50.8) = 344 × 0.492 = 169Ω
```

This increases stub impedance to ~170Ω, similar to RG-8 coax folded into a stub, providing conventional J-pole feed behavior.

### 2.5 Velocity Factor

The electrical length of a transmission line depends on the phase velocity of the signal:

```
VF = v_phase / c = 1 / √εᵣ_eff
```

For the stub (face-to-face on nylon):
```
εᵣ_eff = εᵣ_nylon = 3.0 → VF_stub = 0.577
```

For the radiating element (single face, air on one side):
```
εᵣ_eff = (εᵣ_nylon + εᵣ_air) / 2 = (3.0 + 1.0) / 2 = 2.0 → VF_rad = 0.707
```

**Design compromise:** The dimension tables in this manual use VF = 0.95 uniformly. This is LONGER than the true electrical length of both elements. The effect is that the antenna will resonate at a frequency BELOW the design frequency. The extra length provides trim material — cut from the top of the radiating element to bring resonance up to the target frequency. See Chapter 8 for the trimming procedure.

---

## CHAPTER 3 — DESIGN PARAMETERS AND FORMULAS

### 3.1 Dimensional Formulas

All dimensions use VF = 0.95 as the initial design value:

```
L_rad (m) = 0.95 × 150 / f_MHz          (half-wave radiating element)
L_stub (m) = 0.95 × 75 / f_MHz           (quarter-wave matching stub)
L_total (m) = L_rad + L_stub              (total webbing/tape length)
d_feed (m) = L_stub / 5                   (starting feed point, from short)
```

Equivalent formulas in feet and inches:

```
L_rad (ft) = 0.95 × 492 / f_MHz          (from 300/f × 0.95 × 3.281)
L_stub (ft) = 0.95 × 246 / f_MHz
L_rad (in) = 0.95 × 5905 / f_MHz
L_stub (in) = 0.95 × 2953 / f_MHz
```

### 3.2 Stub Characteristic Impedance

```
Z₀ = (η₀ / √εᵣ) × (d / w)    [parallel-plate approximation, w/d > 5]
```

where:
- η₀ = 377 Ω (free-space impedance)
- εᵣ = dielectric relative permittivity (nylon = 3.0, foam = 1.2, air = 1.0)
- d = conductor separation (m)
- w = conductor width (m)

### 3.3 Feed Point Distance

Starting estimate from short circuit:

```
d_feed_start = L_stub / 5    (λ/20 position)
```

Theoretical optimum (requires knowledge of R_ant):

```
d_feed_opt = L_stub × arcsin(√(Z_feed / R_ant)) / (π/2)
```

For Z_feed = 50Ω and R_ant = 2000Ω:

```
d_feed_opt = L_stub × arcsin(0.158) / 1.571 = L_stub × 0.159 / 1.571 = L_stub / 9.9 ≈ L_stub / 10
```

For the low-Z₀ (13Ω) construction with R_ant = 2000Ω:

```
d_feed_opt ≈ L_stub / 50    (very close to shorted end)
```

**In practice:** slide the feed point from the short upward until minimum SWR is found. The theoretical value is a starting point only.

### 3.4 Antenna Gain

In free space, a J-pole radiating element behaves as a half-wave dipole with:
- **Gain:** 2.15 dBi (same as dipole, 0 dBd)
- **Pattern:** Donut (torus) shape, maximum radiation broadside to element axis
- **Polarization:** Along the element axis (vertical when element is vertical)

The matching stub is not a radiating element (it is balanced and currents cancel). However, currents on the coax shield can cause the feedline to radiate if no common-mode choke is installed. Always use a ferrite choke at the feed point.

---

## CHAPTER 4 — MATERIAL SELECTION

### 4.1 Conductor Tape

**Option A — Aluminum Foil Tape (recommended for HF/VHF portable):**
- Width: 2 inches (50.8mm)
- Thickness: 0.10mm minimum (prefer 0.15mm for durability)
- Adhesive: Acrylic pressure-sensitive adhesive (PSA), conductive or standard
- Conductivity: σ_Al = 3.5×10⁷ S/m
- Weight: 3.6 g/m (2-inch width)
- Cost: Low (HVAC/ductwork tape, hardware stores)
- Source examples: 3M 425, Nashua 322, generic HVAC aluminum tape

**Option B — Copper Foil Tape (recommended for UHF/high-power):**
- Width: 2 inches (50.8mm)
- Thickness: 0.10mm minimum
- Adhesive: Conductive copper (NOT standard copper tape — must specify "conductive adhesive" for electrical continuity between strips)
- Conductivity: σ_Cu = 5.8×10⁷ S/m (65% better than aluminum)
- Weight: 10.2 g/m
- Cost: Higher than aluminum (hobby electronics suppliers)
- Source examples: 3M 1181, Venture Tape 1522CW

**Selection guidance:**
- For HF (160m–10m): Aluminum tape. Conductivity difference is negligible at HF.
- For VHF (6m–2m): Either. Copper slightly better efficiency.
- For UHF (70cm–23cm): Copper. Skin depth at 1.3 GHz is 1.8 μm (Cu) vs 2.2 μm (Al); copper reduces resistive loss in shallow skin depth.
- For highest power or repeated deployment: Copper. More durable adhesive bond.

**DO NOT USE:**
- Duct tape (fabric reinforced, not conductive)
- Adhesive-backed copper tape without "conductive adhesive" specification (the adhesive between strips has high resistance)
- Foil tape thinner than 0.08mm (tears during deployment)

### 4.2 Webbing Substrate

- **Type:** Polypropylene or nylon lawn chair webbing (flat woven strap)
- **Width:** 3 inches (76.2mm) ± 3mm
- **Thickness:** 2.5–4.0mm (nominal 3mm)
- **Weave:** Tight weave preferred over open-mesh; the tighter weave reduces effective εᵣ_eff variation
- **Material:** Polypropylene preferred over nylon for outdoor/wet use:
  - Polypropylene: εᵣ ≈ 2.3, VF_stub ≈ 0.66, absorbs < 0.02% water
  - Nylon: εᵣ ≈ 3.0–3.5, VF_stub ≈ 0.55–0.57, can absorb 4–8% water by weight (shifts resonant frequency in rain)
- **Color:** Any. Light colors (white, beige) simplify printing dimension marks.
- **Source:** Fabric stores, hardware stores, online webbing suppliers. 50-yard rolls are economical.

**Test for suitability:** Press the tape firmly onto a sample of webbing. The tape should bond securely and not peel when flexed. If the webbing has a silicone or UV-protective coating, lightly sand with 220 grit first.

### 4.3 Optional Foam Spacer (Stub Impedance Improvement)

To raise stub Z₀ from 13Ω to approximately 50–170Ω, insert a foam spacer between the two tape strips in the stub section:

- **Material:** Closed-cell polyethylene foam (camping pad, foam pipe insulation)
- **Width:** 50mm (2 inches, matches tape width)
- **Thickness:** 25mm (for Z₀ ≈ 50Ω target)
- **Length:** Same as L_stub
- **εᵣ_foam:** ≈ 1.2

The spacer is placed on top of Strip A (front face) in the stub section only. Strip B (back face) adheres to the spacer's outer face. The webbing remains the structural backbone outside the stub region.

This adds approximately 100g to the stub section of an 80m antenna (negligible for field use) and substantially improves bandwidth.

### 4.4 Feed Connector

- **160m–10m (HF portable):** Banana-jack clip connectors with full insulation on jaws. Use with RG-8X minicoax.
- **10m–6m:** BNC female chassis mount, or N-type female if operating > 75W.
- **2m–1.25m:** BNC or N-type. At 222 MHz, N-type preferred (BNC is rated to 4 GHz but insertion loss rises above 500 MHz).
- **70cm:** SMA female or N-type female. Avoid BNC above 500 MHz.
- **33cm–23cm:** SMA female, silver-plated contacts, PTFE dielectric.

### 4.5 Coaxial Cable

- **HF (160m–10m):** RG-8X (low loss, flexible) or RG-58 (lighter). Keep feedline length as long as needed — feedline length does not affect J-pole tuning with proper choke.
- **VHF (6m–2m):** LMR-400 for runs > 10m. RG-58 acceptable for short runs.
- **UHF (70cm–23cm):** LMR-400, LMR-600, or Times Microwave LMR series. Minimize cable length above 450 MHz.

---

## CHAPTER 5 — CONSTRUCTION PROCEDURE

### 5.1 Materials Checklist

Per antenna:
- [ ] 3-inch webbing, length = L_total + 100mm (extra for hem and short-circuit fold)
- [ ] 2-inch conductor tape (Strip A), length = L_total + 50mm
- [ ] 2-inch conductor tape (Strip B), length = L_stub + 50mm
- [ ] Feed point connector (type per band, Section 4.4)
- [ ] Coaxial cable with connector at radio end
- [ ] Ferrite common-mode choke (FT-240-43 core + 5–10 turns, or ferrite beads)
- [ ] Isopropyl alcohol (IPA), 90% or higher
- [ ] Sharpie permanent marker
- [ ] Ruler or tape measure
- [ ] Sharp scissors or utility knife
- [ ] Binder clips × 2 (temporary, while adhesive cures)
- [ ] Rubber bands or velcro strap (for rolled storage)

### 5.2 Layout and Measurement

**Step 1:** Lay webbing flat on a clean surface. Place a ruler along the length.

**Step 2:** Mark three positions from the BOTTOM end:
- Mark A at d_feed (NEC starting point from TM-WJP-CAL-003)
- Mark B at L_stub (stub top = where Strip B ends)
- Mark C at L_total (top of Strip A = top of antenna)

**Step 3:** Verify measurements. Example for 20m (14.175 MHz):
- Mark A: 1.006m from bottom
- Mark B: 5.026m from bottom
- Mark C: 15.079m from bottom

**Step 4:** Write band name, frequency, and date on webbing with Sharpie.

### 5.3 Applying Strip A (Front Face)

**Step 1:** Clean the front face of the webbing with IPA from bottom to Mark C.

**Step 2:** Starting at Mark C (top), peel back 50mm of backing from Strip A. Press the tape at the top of the webbing, adhesive side down, centered on the webbing face. Leave 10mm of tape extending past the webbing top (this creates a folded hem later).

**Step 3:** Progressively peel the backing and press the tape down the front face. Work from top to bottom, smoothing as you go. Eliminate air bubbles and wrinkles. Use a flat tool (credit card, ruler edge) to burnish the tape.

**Step 4:** At the BOTTOM end, leave 25mm of tape extending past the webbing edge. This will fold onto the back face as the short-circuit connection.

**Step 5:** Fold the 10mm top extension over the top edge of the webbing. Press firmly. This hems the top edge and prevents the tape from peeling under mechanical stress.

### 5.4 Applying Strip B (Back Face — Stub Section)

**Step 1:** On the BACK face of the webbing, measure from the bottom up to Mark B (L_stub). Mark this position with a Sharpie on the edge of the webbing.

**Step 2:** Clean the back face from bottom to Mark B with IPA.

**Step 3:** Starting at Mark B, apply Strip B downward (top to bottom) on the back face, centered. Stop at the bottom edge of the webbing.

**Step 4:** Leave 25mm of Strip B extending past the bottom edge (same as Strip A).

### 5.5 Short-Circuit Fold

The short circuit connects Strip A and Strip B at the bottom end.

**Step 1:** Fold the 25mm extension of Strip A upward onto the back face. The adhesive side of Strip A's extension will now be in contact with the exposed face of Strip B's extension.

**Step 2:** Press firmly. The two tape layers bond adhesively and electrically.

**Step 3:** Cut a 30mm × 30mm piece of tape and apply over the fold on the back face to reinforce the connection.

**Step 4:** For VHF/UHF antennas: solder a short jumper wire or additional copper strip across the fold to ensure reliable DC and RF continuity through the bend.

**Step 5:** Verify with ohmmeter: resistance from Strip A (measured at top) to Strip B (measured at stub top) should be < 0.5Ω (through the short circuit only — the elements themselves are open to each other along their length).

**CAUTION:** Do not measure between Strip A and Strip B at the same HEIGHT — they should be open (no dielectric contact) except at the short-circuit fold.

### 5.6 Top Hem

Fold the 10mm extension of Strip A over the top of the webbing. Secure with a strip of tape or a small grommet. This top end is the open (high-impedance) end of the radiating element and requires no electrical connection.

For field use as a tie point: cut a 10mm hole through the webbing at 20mm from the top. Pass a loop of paracord through the hole. This is the tie point for mast attachment.

### 5.7 Marking the Feed Zone

With a contrasting color Sharpie (red or white paint pen), mark:
- The d_feed starting point (solid line across webbing)
- d_feed ± 50% as dotted lines above and below (search range)
- "FEED START" label

This makes it easy to find the feed zone when deploying in low light or with cold hands.

---

## CHAPTER 6 — FEED POINT INSTALLATION

Refer to TM-WJP-SCH-002 for detailed connector drawings.

### 6.1 HF Portable Clip Connector (160m – 10m)

**Step 1:** Prepare coaxial cable. Strip 40mm of jacket from the radio end. Braid back 20mm. Expose 10mm of center conductor. Tin both.

**Step 2:** Attach center conductor to one alligator clip (banana-clip style). Attach shield to second clip. Insulate all metal on both clips except the jaw (heat-shrink tube works well).

**Step 3:** Cut 20mm notch in webbing at the d_feed mark, centered. This allows each clip to reach its respective tape face.

**Step 4:** Clip center to Strip A (front), shield to Strip B (back), both at the d_feed mark.

**Step 5:** Install common-mode choke (Section 6.4).

**Step 6:** Route coax down the webbing surface. Secure every 300mm with a cable tie through the webbing mesh.

### 6.2 Semi-Permanent Solder Tab (6m – 2m)

Refer to TM-WJP-SCH-002, Section 2.

**Step 1:** At the d_feed mark, clean both tape surfaces with IPA.

**Step 2:** Pre-tin a 50×15mm copper tab. Solder the center conductor to the tab.

**Step 3:** Press the tinned tab firmly against Strip A (front). Apply minimal heat (< 2 seconds) with iron tip. The tinned surface bonds to the tape. Then add a small drop of solder at one end of the tab for mechanical security.

**Step 4:** Repeat for the shield connection to Strip B (back).

**Step 5:** Seal the back face with a thin coat of clear RTV silicone. Let cure 24 hours before outdoor use.

### 6.3 UHF Direct-Solder Connection (70cm – 23cm)

**Step 1:** With a sharp utility knife, carefully cut two 10×10mm windows through the webbing at the d_feed mark. One window on each face, offset by 3mm (webbing thickness).

**Step 2:** Through the front window, solder the SMA center pin to Strip A using the tinned bridge technique.

**Step 3:** Through the back window (or from the back face), solder the SMA body/ground to Strip B.

**Step 4:** Fill windows with a thin coat of UV-cure adhesive or RTV.

**Step 5:** Calibrate on NanoVNA before field use. At 70cm, a few mm error in feed point causes significant SWR change.

### 6.4 Common-Mode Choke (Required for All Bands)

**HF choke (1.8–30 MHz):** Wind 10 turns of the coaxial feedline on an FT-240-43 toroid (43 mix, 2.4-inch OD) immediately below the feed point connection. Place choke within 100mm of the feed tab or clip.

**VHF choke (50–148 MHz):** 5 turns on FT-240-43, or 6× ferrite beads (material 73) threaded on the coax.

**UHF choke (446 MHz and above):** 5× ferrite beads (material 43) on the coax. Alternatively, use a quarter-wave choke sleeve fabricated from 25mm copper tape wrapped around the coax for λ/4 electrical length.

---

## CHAPTER 7 — DEPLOYMENT CONFIGURATIONS

### 7.1 Vertical (Primary Mode)

This is the standard configuration producing an omnidirectional horizontally-polarized pattern in the horizontal plane (donut around the vertical element).

**Minimum mount height for HF:**

| Band | Element length | Recommended base height |
|------|---------------|------------------------|
| 40m | 19.9m | 1.5m (base at waist height, top at 21.4m) |
| 20m | 10.1m | 2.0m (base, top at 12.1m) |
| 10m | 5.0m | 2.5m (tree branch, top at 7.5m) |
| 6m | 2.8m | 1.0m (top at 3.8m) |
| 2m | 0.98m | Any height |

For HF: the short circuit end (bottom of antenna) should be at least 1–2m above ground for best pattern. The radiating element top should be as high as possible — every additional meter of height at 40m adds approximately 0.3 dB of low-angle gain.

**Mounting methods:**
- Fishing rod or fiberglass mast: tie webbing to mast with cable ties at 1m intervals
- Tree branch: throw paracord over branch, attach to antenna top grommet
- Push-up pole: cable-tie antenna to side of pole, coax routed down opposite side

### 7.2 Sloper

In this mode the antenna is deployed at a 30–60 degree angle from vertical. The high end (top of radiating element) is attached to a tall support; the low end (stub bottom/short circuit) is anchored near ground level or held at operator height.

**Characteristics:**
- Directive pattern with maximum radiation toward the low-angle direction opposite the high support
- Lower height requirement for the support vs. full vertical
- Good for NVIS (near-vertical incidence skywave) at 30–45 degree slope
- Useful when the operating site has a single tall tree or support

**Sloper NEC model:** TM-WJP-NEC-009 (40m sloper at 45°).

### 7.3 Horizontal

Lay the antenna flat in a horizontal direction (along a fence line, tied between two trees). The element runs horizontally and the polarization is horizontal.

**Characteristics:**
- Figure-8 (dipole-like) radiation pattern, maximum broadside to element axis
- Minimum radiation off the ends
- Good for point-to-point links where directionality is useful
- Horizontal polarization can reduce certain types of local interference (many manmade noise sources are vertically polarized)

**Horizontal NEC model:** TM-WJP-NEC-007 (2m horizontal).

### 7.4 Portable Deployment Sequence

1. Unroll antenna, starting from the rolled end (open top of radiating element)
2. Identify bottom (short-circuit fold) and top (open end)
3. Attach top grommet to support point
4. Hang antenna vertically; ensure it hangs free (not wrapped around support)
5. Attach coax clips to the marked feed point on both faces
6. Route coax away from antenna at 90° for first 1m (reduces coupling)
7. Install choke on coax within 100mm of feed point
8. Run coax to radio; allow slack for wind movement
9. Check SWR; optimize feed point position if > 2:1 (Chapter 8)

---

## CHAPTER 8 — MANUAL TUNING PROCEDURES

### 8.1 Required Equipment

- NanoVNA (preferred) or SWR meter + transmitter at < 5W
- Marking pen (Sharpie)
- Scissors or utility knife
- Ruler or tape measure
- Paper and pencil for recording positions and SWR

### 8.2 Initial SWR Check

**Procedure 8-1: First-Use SWR Check**

1. Connect NanoVNA S11 port to antenna feedline. Configure sweep: 0.5× to 1.5× the target frequency. Example for 20m (14.175 MHz): sweep 7–21 MHz.

2. Observe S11. A working J-pole shows:
   - Deep null (S11 minimum) at the resonant frequency
   - Null depth > 20 dB = excellent (SWR < 1.22:1)
   - Null depth 15–20 dB = good (SWR 1.4:1 to 1.22:1)
   - Null depth < 15 dB = feed point needs adjustment

3. Note the frequency of the S11 minimum. If the minimum is:
   - BELOW target frequency: antenna is too long → proceed to Procedure 8-2
   - AT target frequency but SWR > 2:1: feed point is wrong → proceed to Procedure 8-3
   - ABOVE target frequency: antenna is too short → do not cut; extend by adding tape to top

### 8.3 Length Adjustment

**Procedure 8-2: Resonance Frequency Correction**

1. Remove 2% of the radiating element length from the top (open end). Example: 20m antenna L_rad = 10.05m → remove 10.05 × 0.02 = 201mm from top.

2. Re-measure SWR. Resonance should have shifted upward.

3. Repeat in 2% increments until the resonant dip is at or slightly above the target frequency.

4. The final length provides a margin for seasonal variation (temperature changes permittivity of nylon by ±5%).

**Trim table (approximate, start here):**

| Antenna is resonant at | Remove from top |
|-----------------------|----------------|
| 3% below target | 3% of L_rad |
| 5% below target | 5% of L_rad |
| 10% below target | 9% of L_rad |
| 15% below target | 13% of L_rad |

Do not attempt to tune an antenna that is more than 25% off frequency — the stub will also be wrong. Rebuild from the correct calculated dimensions.

### 8.4 Feed Point Adjustment

**Procedure 8-3: Feed Point Optimization**

This procedure assumes the antenna resonant frequency is correct (Procedure 8-2 complete).

1. Start with feed point at the calculated d_feed starting position.

2. If SWR at resonance > 2:1: move feed point upward by 5% of L_stub. Remeasure.

3. If SWR at resonance increases when moving up: move downward from the original position.

4. Continue in 5% steps of L_stub until minimum SWR is found. The minimum is typically 1.1:1 to 1.4:1.

5. Mark the optimal feed point with a different color Sharpie or paint pen.

6. For subsequent field deployments, clip directly to this mark.

**If no minimum SWR is found across the full stub range:**
- The stub may be too short or too long (resonant frequency correction required first)
- Check that the short circuit is electrically solid (measure DC continuity)
- Check that Strip A and Strip B are NOT shorted except at the short-circuit fold

### 8.5 Fine Tuning for Band Segments

For operation across a wide band (e.g., 40m is 7.0–7.3 MHz), the J-pole will be resonant at one frequency. For off-center frequencies, SWR may rise:

- At the design frequency: SWR typically 1.1:1 – 1.3:1
- 100 kHz from design (10m): SWR typically 1.3:1 – 1.6:1
- 200 kHz from design (40m): SWR typically 1.5:1 – 2.5:1

A wide tape conductor improves bandwidth. The 2-inch tape design typically achieves 2:1 SWR bandwidth:
- 40m: ± 100–150 kHz
- 20m: ± 200–300 kHz
- 10m: ± 500–800 kHz
- 6m: ± 1.0–1.5 MHz
- 2m: ± 3–5 MHz

For wider 40m coverage: build to resonate at 7.150 MHz (band center). SWR < 2:1 covers most of the 40m phone and CW segments.

---

## CHAPTER 9 — NEC MODEL REFERENCE

### 9.1 Available Models

| File | Band | Freq (MHz) | Configuration |
|------|------|-----------|---------------|
| TM-WJP-NEC-001 | 40m | 7.150 | Vertical, free space |
| TM-WJP-NEC-002 | 20m | 14.175 | Vertical, free space |
| TM-WJP-NEC-003 | 10m | 28.500 | Vertical, free space |
| TM-WJP-NEC-004 | 6m | 51.000 | Vertical, free space |
| TM-WJP-NEC-005 | 2m | 146.000 | Vertical, free space |
| TM-WJP-NEC-006 | 70cm | 446.000 | Vertical, free space |
| TM-WJP-NEC-007 | 2m | 146.000 | Horizontal, free space |
| TM-WJP-NEC-008 | 80m | 3.750 | Vertical, free space |
| TM-WJP-NEC-009 | 40m | 7.150 | 45° sloper, free space |

### 9.2 NEC Model Parameters

**Wire equivalent radius:** All models use r = 8.09mm (50.8mm / 2π), the theoretical equivalent radius for a flat tape conductor.

**Stub wire separation:** Models use 25mm center-to-center. This is a modeling constraint — the NEC thin-wire approximation requires separation > 2 × radius (16.2mm minimum); 25mm provides adequate margin.

**Ground model:** All models use free space (GE 0). For elevated antenna simulations near real ground, add the Sommerfeld-Norton ground card:
```
GN  2  0  0  0  13  0.005
```
This approximates average soil (εᵣ=13, σ=0.005 S/m). Adjust for local ground conditions.

### 9.3 What NEC Models Show Accurately

**Accurate in NEC:**
- Radiation pattern shape (elevation and azimuth)
- Relative gain vs. frequency (pattern changing with frequency)
- Comparison of vertical vs. sloper vs. horizontal orientations
- Effect of height above ground (with GN card)

**Not accurate in NEC (for this antenna):**
- Feed point impedance: NEC models 25mm-separated round wires giving Z₀ ≈ 148Ω; actual stub is face-to-face with Z₀ ≈ 13Ω. The NEC feed point location (Segment 3, ≈22% of stub) does not correspond to the actual optimal feed point (3–8% of stub for Z₀=13Ω construction).
- Absolute SWR: dependent on stub Z₀ and conductor loss, both approximate in NEC.
- Tape conductor loss: models use lossless conductors. To add aluminum conductivity (3.5×10⁷ S/m), insert `LD 5 0 0 0 3.5E7` before the EX card. This line sets all wires to aluminum conductivity.

### 9.4 Running the NEC Models

**EZNEC (Windows):**
1. Open EZNEC 5.0 or later
2. File → Import NEC Input → select TM-WJP-NEC-00X.nec
3. Run → Azimuth Pattern or Elevation Pattern
4. Note: EZNEC renames sources and segments; verify source is on Wire 2

**4nec2 (Windows):**
1. File → Open → select .nec file
2. Run → Near Field / Far Field
3. Generate Azimuth Plot at theta=90° (horizontal plane)

**xnec2c (Linux):**
```
xnec2c -i TM-WJP-NEC-001_40m.nec
```
Then: View → Far Field, select azimuth or elevation.

**nec2 command line:**
```
nec2 < TM-WJP-NEC-001_40m.nec > TM-WJP-NEC-001_40m.out
grep "RADIATION PATTERNS" TM-WJP-NEC-001_40m.out
```

---

## CHAPTER 10 — BAND-BY-BAND DATA AND OPERATING NOTES

### 10.1 160 Meters (1.850 MHz)

**Dimensions:** L_rad = 76.99m, L_stub = 38.51m, L_total = 115.5m

**Assessment:** Impractical as a dedicated J-pole. At 115m total length, this requires a very large field to deploy vertically. Consider a 160m loaded dipole (see PNL project) or inverted-L instead.

**If deployed:** Use as an inverted-L or sloper. Hang the radiating element horizontally; stub hangs vertically. The stub bottom (short circuit) near ground level where the coax connects.

### 10.2 80 Meters (3.750 MHz)

**Dimensions:** L_rad = 38.00m, L_stub = 19.00m, L_total = 57.0m

**Assessment:** Practical as a field antenna in parks or open areas. The antenna can be deployed as a vertical inverted-L or sloper between two trees. Roll fits in a small bag.

**Deployment:** String the 57m over tall trees. Position the stub section vertically; let the radiating element drape at an angle or string to a second support. Near-vertical angle at the top of the stub radiates NVIS at this frequency.

### 10.3 40 Meters (7.150 MHz)

**Dimensions:** L_rad = 19.93m, L_stub = 9.97m, L_total = 29.9m

**Assessment:** Excellent portable antenna. 30m antenna fits in a daypack. Can deploy vertically from a single 10m mast or tall tree.

**Notes:** The most versatile HF band for portable work. The 40m webbing J-pole fits a 10m collapsible fiberglass mast. Build to 7.150 MHz for CW/SSB phone coverage.

**Weight:** 483g aluminum, 684g copper.

### 10.4 30 Meters (10.125 MHz)

**Dimensions:** L_rad = 14.07m, L_stub = 7.04m, L_total = 21.1m

**Assessment:** Excellent. Compact roll. Primary use: WSPR and digital modes (WSPR beacon, FT8).

**Notes:** 30m is CW/digital only (no phone) per band plan. J-pole bandwidth at 30m is wide enough to cover the entire band (100 kHz × 3–4 = 300–400 kHz 2:1 SWR bandwidth).

### 10.5 20 Meters (14.175 MHz)

**Dimensions:** L_rad = 10.05m, L_stub = 5.03m, L_total = 15.1m

**Assessment:** Excellent DX antenna. Best band for J-pole portable performance at moderate height.

**Notes:** Most-used HF DX band. 15m roll is compact; 244g aluminum. Build centered at 14.175 MHz. SWR < 2:1 covers 14.0–14.35 MHz (full 20m band) when deployed at height ≥ 8m.

### 10.6 17 Meters through 10 Meters

**Dimensions:** See Appendix A. Rolls shrink to 12m (17m), 10m (15m), 8.6m (12m), and 7.5m (10m).

**Assessment:** Superb portable performance. At 10m, the 7.5m roll is easily carried. Good for solar-cycle peak DX.

**Notes:** For 10m–15m, build several antennas and store as a set. Total weight for four antennas (10m–17m, aluminum) ≈ 550g.

### 10.7 6 Meters (51.000 MHz)

**Dimensions:** L_rad = 2.79m, L_stub = 1.40m, L_total = 4.2m

**Assessment:** Excellent. The 6m J-pole fits in a jacket pocket when rolled. Can be deployed from a camera tripod.

**Notes:** 6m opening season (May–September at mid-latitudes) makes this antenna extremely useful for portable contesting. Wide bandwidth — 2:1 SWR covers ≥ 50.0–51.5 MHz when properly tuned. Use BNC connector and RG-58.

**Construction note:** At 6m, the tape width (50.8mm) is 0.87% of wavelength — still well within thin-tape approximation. Use copper tape for slightly better efficiency at VHF.

### 10.8 2 Meters (146.000 MHz)

**Dimensions:** L_rad = 0.976m, L_stub = 0.488m, L_total = 1.464m

**Assessment:** Superb. The 2m J-pole is 1.46m long and 76mm wide — a flat strip you can roll to the size of a napkin.

**Notes:** The classic J-pole application. Perfect for emergency communications (EMCOMM), portable hill-top operation, and APRS. Deploy on a camera tripod, walking stick, or fence post. Use copper tape for slightly lower loss at VHF. Solder the feed tabs rather than using clips.

**Fabrication precision:** At 2m, 10mm length error = 2% → resonance off by ~3 MHz. Measure to ±2mm during construction.

**Ferrite choke:** Essential. Without a choke, body capacitance on the feedline shifts resonance and degrades pattern. Use 6 ferrite beads (73 material) on RG-58.

### 10.9 1.25 Meters (222.000 MHz)

**Dimensions:** L_rad = 0.642m, L_stub = 0.321m, L_total = 0.963m

**Assessment:** Excellent. Under 1m long. Solder the feed connection.

**Notes:** 1.25m band is US-only. Not widely used but seeing renewed interest for APRS and digital modes. Same construction as 2m with shorter dimensions.

### 10.10 70 Centimeters (446.000 MHz)

**Dimensions:** L_rad = 0.320m, L_stub = 0.160m, L_total = 0.479m

**Assessment:** Excellent. 0.48m × 76mm — shirt pocket size. Use copper tape.

**Notes:** The 70cm J-pole is one of the most practical antennas in this system. A pair (2m + 70cm) fits in a single shirt pocket and covers the two most popular portable FM and digital bands.

**Construction note:** At 446 MHz, tape width (50.8mm) = 7.5% of λ = 67.3mm. This is beginning to be significant. The NEC model is approximate. Measure final tuning with NanoVNA. Use SMA connector.

**Tuning range:** Start at d_feed from the table. At 70cm, optimal feed position varies ±5mm from the calculated value. Use a solder dab at d_feed and re-solder if SWR > 1.5:1 after initial measurement.

### 10.11 33 Centimeters and 23 Centimeters

**Assessment:** Marginal with 2-inch tape. The tape width (50.8mm) at 906 MHz = 15.3% of λ (= 331mm). The antenna is electrically "wide" and tape-antenna modeling becomes less accurate.

**Recommendation:** For 33cm and 23cm, use narrower tape — 1 inch (25.4mm) or 0.75 inch. Scale all dimensions by the same formula. The J-pole geometry still applies.

If 2-inch tape is used at 33cm: treat the NEC model as approximate, tune entirely by NanoVNA measurement. Final antenna will likely differ from calculated dimensions by 8–15%.

---

## CHAPTER 11 — TROUBLESHOOTING

### 11.1 SWR Does Not Dip Below 3:1 Anywhere

**Most likely causes:**
1. Short circuit at bottom is open (not making contact). Check with ohmmeter between bottom of Strip A and bottom of Strip B. Should be < 1Ω.
2. Strip A and Strip B are shorted somewhere in the middle of the stub (webbing fold is pinching tape). Check with ohmmeter — should be open between tapes except at short-circuit bottom.
3. Coaxial cable has internal break. Test cable with ohmmeter separately.
4. Feed connector is not making good contact with both tapes simultaneously.
5. Common-mode current is dominating — the SWR meter is reading feedline resonance. Move the choke and remeasure.

### 11.2 SWR Minimum Is Well Below Target Frequency (> 5% low)

**Cause:** Antenna is too long.
**Fix:** Trim 5% from the top of the radiating element. Repeat until resonance reaches target frequency.

### 11.3 SWR Minimum Is Above Target Frequency

**Cause:** Antenna has been over-trimmed, or initial tape application left a gap.
**Fix:** Extend Strip A at the top by adding a 50mm piece of tape. Overlap by 20mm to ensure electrical continuity.

### 11.4 SWR Is > 2:1 at Resonant Dip Despite Adjustment

**Cause:** Feed point impedance is not 50Ω. The tap is wrong.
**Fix:**
1. Slide the feed point closer to the shorted end (toward the bottom).
2. The optimal point for the face-to-face Z₀=13Ω construction is L_stub/20 to L_stub/50 from the short — much closer than the initial NEC-derived d_feed/5 starting point.
3. For Z₀≈13Ω, the optimal d_feed is often only 3–5 cm on a 20m J-pole (5.03m stub).

### 11.5 SWR Changes When Hand Approaches the Antenna

**Cause:** No common-mode choke, or choke is too far from feed point.
**Fix:** Install ferrite choke within 100mm of the feed point. Add additional choke turns if the effect persists.

### 11.6 Antenna Resonates on All Bands (Every λ/2 Multiple)

**Cause:** This is expected behavior for a J-pole — the half-wave element resonates at odd multiples of the design frequency. However, the matching stub only provides the correct 50Ω match at the design frequency. At harmonic resonances, SWR will likely be high. This is not a problem; simply use the antenna on its design band.

### 11.7 Tape Peeling Off Webbing in Field Conditions

**Cause:** Humidity, temperature cycling, or UV exposure degrading the adhesive.
**Fix:**
1. Re-apply tape after cleaning surface with IPA.
2. Use UV-resistant tape (3M 425 is UV-rated).
3. Apply a thin coat of clear silicone spray over the tape surface after construction (allows the antenna to breathe while protecting edges).
4. For high-humidity climates: use copper foil tape with acrylic conductive adhesive (more moisture-resistant than standard aluminum tape adhesive).

---

## APPENDIX A — COMPLETE DIMENSION TABLES

### Metric Dimensions (VF = 0.95)

| Band | Freq (MHz) | L_rad (m) | L_stub (m) | L_total (m) | d_feed start (m) |
|------|-----------|----------|-----------|------------|-----------------|
| 160m | 1.850 | 76.99 | 38.51 | 115.49 | 7.70 |
| 80m | 3.750 | 38.00 | 19.00 | 57.00 | 3.80 |
| 60m | 5.355 | 26.61 | 13.31 | 39.92 | 2.66 |
| 40m | 7.150 | 19.93 | 9.97 | 29.90 | 1.99 |
| 30m | 10.125 | 14.07 | 7.04 | 21.11 | 1.41 |
| 20m | 14.175 | 10.05 | 5.03 | 15.08 | 1.01 |
| 17m | 18.118 | 7.87 | 3.93 | 11.80 | 0.787 |
| 15m | 21.225 | 6.72 | 3.36 | 10.07 | 0.672 |
| 12m | 24.940 | 5.71 | 2.86 | 8.57 | 0.571 |
| 10m | 28.500 | 5.00 | 2.50 | 7.50 | 0.500 |
| 6m | 51.000 | 2.79 | 1.40 | 4.19 | 0.279 |
| 2m | 146.000 | 0.976 | 0.488 | 1.464 | 0.098 |
| 1.25m | 222.000 | 0.642 | 0.321 | 0.963 | 0.064 |
| 70cm | 446.000 | 0.320 | 0.160 | 0.479 | 0.032 |
| 33cm | 906.000 | 0.157 | 0.079 | 0.236 | 0.016 |
| 23cm | 1296.000 | 0.110 | 0.055 | 0.165 | 0.011 |

### Imperial Dimensions (feet and inches)

| Band | L_rad (ft) | L_stub (ft) | L_total (ft) | d_feed (in) |
|------|-----------|------------|-------------|------------|
| 160m | 252.6 | 126.3 | 378.9 | 303 |
| 80m | 124.7 | 62.3 | 187.0 | 150 |
| 60m | 87.3 | 43.7 | 130.9 | 105 |
| 40m | 65.4 | 32.7 | 98.1 | 78.4 |
| 30m | 46.2 | 23.1 | 69.3 | 55.4 |
| 20m | 33.0 | 16.5 | 49.5 | 39.6 |
| 17m | 25.8 | 12.9 | 38.7 | 31.0 |
| 15m | 22.0 | 11.0 | 33.0 | 26.4 |
| 12m | 18.8 | 9.4 | 28.1 | 22.5 |
| 10m | 16.4 | 8.2 | 24.6 | 19.7 |
| 6m | 9.2 | 4.6 | 13.7 | 11.0 |
| 2m | 3.20 | 1.60 | 4.80 | 3.84 |
| 1.25m | 2.11 | 1.05 | 3.16 | 2.53 |
| 70cm | 1.05 | 0.52 | 1.57 | 1.26 |
| 33cm | 0.52 | 0.26 | 0.77 | 0.62 |
| 23cm | 0.36 | 0.18 | 0.54 | 0.43 |

---

## APPENDIX B — MATERIALS AND SOURCES

| Item | Specification | Source |
|------|-------------|--------|
| Aluminum tape | 2-inch, 0.10–0.15mm thick | Hardware, HVAC suppliers, Amazon |
| Copper foil tape | 2-inch, conductive adhesive | Online (Venture, 3M, Bertech) |
| Webbing | 3-inch nylon or polypropylene | Fabric stores, Amazon, webbing suppliers |
| Ferrite core HF | FT-240-43 (2.4-inch) | Palomar Engineers, Fair-Rite |
| Ferrite beads VHF | 43-mix or 73-mix | Mouser, Digi-Key, Fair-Rite |
| Coax HF | RG-8X or RG-58 | HRO, DX Engineering |
| Coax VHF | LMR-400 | Times Microwave, HRO |
| BNC connectors | Female chassis, UG-1094 | Mouser, Amphenol |
| SMA connectors | Female right-angle PCB | Mouser, SMA-RPF-RA |
| Foam spacer | 25mm closed-cell PE foam | Hardware, camping supplies |

---

## APPENDIX C — VELOCITY FACTOR AND DIELECTRIC THEORY

### Dielectric Properties of Common Webbing Materials

| Material | εᵣ | VF_stub | VF_element | Water absorption |
|---------|-----|---------|-----------|----------------|
| Nylon (polyamide) | 3.0–3.5 | 0.53–0.58 | 0.69–0.73 | 4–8% by weight |
| Polypropylene | 2.2–2.3 | 0.66–0.67 | 0.76–0.78 | < 0.02% |
| Polyester (PET) | 3.2 | 0.56 | 0.72 | 0.4% |
| Cotton (dry) | 2.0–2.5 | 0.63–0.71 | — | 7–8% (poor choice) |
| Air | 1.0 | 1.00 | — | — |
| Closed-cell PE foam | 1.1–1.2 | 0.91–0.95 | — | < 0.1% |

### Effect of Moisture on Resonance

When nylon webbing absorbs moisture, εᵣ_nylon increases from 3.0 to approximately 4.5–8.0 at 10% water content. This decreases the velocity factor and shifts the resonant frequency downward by 10–20%.

**To minimize moisture sensitivity:**
- Use polypropylene webbing instead of nylon
- After construction, apply one coat of clear conformal coating (MG Chemicals 419) to the non-conductive surfaces
- Store rolled antennas in sealed plastic bags with a silica gel desiccant packet

### Verifying Velocity Factor

To measure the actual VF of your construction:
1. Build a 500mm test section (one strip each face, shorted at one end)
2. Connect to NanoVNA
3. Measure first resonance (series resonance = λ/4 line short-circuited)
4. f_res_1 = measured frequency, f_free_space = c/(4×0.500) = 150 MHz
5. VF_measured = f_res_1 / f_free_space

Typical results: VF = 0.57–0.68 depending on webbing and tape.

---

## APPENDIX D — COMMON-MODE CHOKE WINDING REFERENCE

### Choke Turns on FT-240-43 Toroid

| Band | Turns | Wire | Impedance at band freq |
|------|-------|------|----------------------|
| 160m–80m | 12 | RG-58 | 3500–5000Ω |
| 40m–30m | 10 | RG-58 | 2500–3500Ω |
| 20m–15m | 8 | RG-58 | 1500–2500Ω |
| 10m | 6 | RG-58 | 1000–1500Ω |
| 6m–2m | 4–5 | RG-58 | 500–1000Ω |

### Ferrite Bead Choke (VHF/UHF)

Thread coax through ferrite beads (43 or 73 material):
- 2m–1.25m: 6 beads, 73 material
- 70cm: 5 beads, 43 material
- 33cm–23cm: 4 beads, 43 material, keep cluster within 25mm of feed

### Quarter-Wave Choke Sleeve (70cm and above)

For UHF, a sleeve choke is more effective than ferrite beads:
1. Cut a strip of copper tape: width = 25mm, length = 0.95 × λ/4
2. Wrap the tape around the coax jacket (touching the metal shield through the jacket is fine)
3. One end of the sleeve is at the feed point (electrically connected to coax outer conductor)
4. Other end is open (not connected to anything)
5. The sleeve resonates and presents high impedance to common-mode current

Sleeve lengths:

| Band | Design freq | Free-space λ/4 | Sleeve length (VF=0.95) |
|------|------------|---------------|------------------------|
| 70cm | 446 MHz | 168mm | 160mm |
| 33cm | 906 MHz | 82.8mm | 78.6mm |
| 23cm | 1296 MHz | 57.9mm | 55.0mm |

---

*End of Document — TM-WJP-001 Rev A*

*For corrections or additions, contact: d.ecaudata@gmail.com*
