**Helix math formulas** for your **hybrid PVC + lawn chair webbing + 2" copper tape normal-mode helical (barber-pole) antenna** focus on geometry, resonance estimation, and tuning. W6NBC’s practical designs (the direct inspiration for this build) are largely empirical—start long and trim—but the underlying math helps with initial sizing.

This is a **normal-mode helix** (electrically small: circumference ≪ λ, height ≪ λ/2), behaving like a shortened, continuously loaded vertical dipole. It radiates broadside (omnidirectional horizontally) with vertical polarization.

### 1. Basic Geometric Formulas
- **Wavelength (λ)**:  
  λ (meters) = 300 / f (MHz)  
  λ (inches) ≈ 11808 / f (MHz)  

  For 146 MHz (2m center): λ ≈ 2.055 m (80.9 inches).

- **Free-space half-wave dipole reference**:  
  ≈ 468 / f (MHz) feet or 5616 / f (MHz) inches.  
  At 146 MHz: ~38.5 inches (your helix will be much shorter physically but electrically similar).

- **Helix circumference (C) per turn** (based on PVC diameter):  
  C = π × D (where D = PVC outer diameter)  
  Example: 1.25" Schedule 40 PVC → D ≈ 1.66" actual OD → C ≈ 5.2 inches per turn.  
  (Wider tape/webbing fills most of this.)

- **Pitch / Turn spacing (S)**: Distance along the pipe between adjacent turns (center-to-center).  
  Typically 1.5–3 inches for VHF (small gap for the 2" tape/webbing).

- **Pitch angle (α)**:  
  α = arctan(S / C)  
  Usually 10–30° for normal-mode shortened designs.

- **Total axial height of helix (h)**:  
  h = N × S (N = number of turns).

- **Total conductor (tape) length (L)**:  
  L ≈ N × √(C² + S²)  
  This is the "unrolled" length that determines electrical resonance (roughly 0.95–1.0 × half-wave for resonance, adjusted for loading and PVC velocity factor ~0.9–0.95).

### 2. Resonance and Shortening Estimation
For normal-mode helical dipoles:  
- **Approximate resonant frequency (f)**:  
  f (MHz) ≈ (constant × c) / L_total  
  Where L_total is the total tape length in meters, c = 3×10^8 m/s. A common empirical shortening factor for these designs gives resonance when total tape length is close to a half-wave (adjusted downward by PVC and wide conductor effects).

- **Rule of thumb from W6NBC-style builds**: Start with total tape ~40–54 inches for 2m (longer than free-space half-wave due to helix inductance). The helix shortens the physical height to ~40–50% of a J-pole while maintaining good efficiency (>90% with wide tape).

- **Velocity factor adjustment**: PVC + webbing slightly slows the wave → add 5–10% extra length initially.

### 3. Tuning Formulas / Guidelines (Practical Math)
- **SWR match (impedance)**: Primarily set by **feedpoint offset**.  
  Target ratio: ~3.5 turns above feed / 4.5 turns below (total ~8 turns).  
  This offsets the feed from exact center to raise the lowered radiation resistance of the shortened dipole toward 50 Ω.

- **Frequency shift**:  
  Trim equal amounts from **both ends** to raise frequency (shortens total L).  
  Trim **only top** for SWR tweaks (changes offset ratio).  
  Approximate sensitivity: Small changes (~½–1" per end) shift resonance by a few MHz.

- **Bandwidth estimate**: Wider 2" tape + webbing increases it (often full 2m band at <1.5:1 SWR) compared to thin wire.

### 4. Example Calculation for Your 2m Hybrid (146 MHz)
- Target: ~18–20" helix height on 36" total PVC.
- Assume 1.25–2" PVC (C ≈ 4–6.3" per turn).
- Start with N ≈ 9–10 turns, S ≈ 2–2.25" → h ≈ 20".
- Total L ≈ 10 × √(C² + S²) ≈ 50–55" tape initially.
- After tuning: ~8 turns total (3.5 top / 4.5 bottom).

Use an antenna analyzer for final tweaks—these formulas give an excellent starting point.

### Quick Design Spreadsheet Tips
You can plug into a simple calculator or Python code:
```python
import math
f_mhz = 146
lambda_in = 11808 / f_mhz
print("Half-wave ref:", round(lambda_in / 2, 1), "inches")
D_in = 1.25
C_in = math.pi * D_in
S_in = 2.2
N = 9
h_in = N * S_in
L_in = N * math.sqrt(C_in**2 + S_in**2)
print("Helix height:", round(h_in,1), "inches")
print("Total tape:", round(L_in,1), "inches")
```

These are the core formulas for your hybrid build. For other bands, scale proportionally (more turns/longer PVC for lower frequencies). Let me know your target frequency, PVC diameter, or desired height, and I can run specific numbers!