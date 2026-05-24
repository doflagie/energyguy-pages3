"""
nec_generator.py
TM-MODEL-001 Rev A — NEC File Generator

Generates .NEC input files for NEC2, NEC4, and 4NEC2 from parametric
antenna specifications. Supports dipoles, verticals, Yagis, log-periodics,
quad loops, phased arrays, and ground-mounted verticals with radials.

Usage:
    from nec_generator import NECModel, Dipole, Yagi, VerticalWithRadials

    ant = Dipole(freq_mhz=14.25, height_m=10.0)
    model = ant.to_nec_model()
    model.write("dipole_20m.nec")

NEC coordinate system:
    X: horizontal (East)
    Y: horizontal (North)
    Z: vertical (up)
    All dimensions in meters unless scaled by GS card.
"""

from __future__ import annotations
import math
import textwrap
from dataclasses import dataclass, field
from typing import List, Optional, Tuple, Dict, Any
from pathlib import Path


# ─── Physical constants ───────────────────────────────────────────────────────
C_LIGHT = 299.792458e6   # m/s
Z0_FREE = 376.730        # Ohms (free-space impedance)


# ─── Wire segment recommendation ─────────────────────────────────────────────
def recommended_segments(length_m: float, freq_mhz: float, min_segs: int = 3) -> int:
    """NEC guideline: ~10 segments per wavelength, odd count preferred."""
    wavelength = C_LIGHT / (freq_mhz * 1e6)
    segs = max(min_segs, int(length_m / wavelength * 10))
    return segs if segs % 2 == 1 else segs + 1


# ─── Data classes ─────────────────────────────────────────────────────────────
@dataclass
class Wire:
    """NEC GW card: wire segment definition."""
    tag:     int
    segs:    int
    x1: float; y1: float; z1: float   # Start point (m)
    x2: float; y2: float; z2: float   # End point (m)
    radius:  float = 0.001             # Wire radius (m); 1mm default

    def to_card(self) -> str:
        return (f"GW {self.tag:3d} {self.segs:3d}"
                f" {self.x1:10.4f} {self.y1:10.4f} {self.z1:10.4f}"
                f" {self.x2:10.4f} {self.y2:10.4f} {self.z2:10.4f}"
                f" {self.radius:8.5f}")


@dataclass
class Excitation:
    """NEC EX card: voltage source excitation."""
    tag:     int = 1
    segment: int = 1
    v_real:  float = 1.0   # Volts real (default 1V for impedance calculation)
    v_imag:  float = 0.0

    def to_card(self) -> str:
        return f"EX 0 {self.tag:3d} {self.segment:3d} 0 {self.v_real:.4f} {self.v_imag:.4f}"


@dataclass
class FrequencyCard:
    """NEC FR card: frequency specification."""
    start_mhz: float
    stop_mhz:  float = 0.0
    n_steps:   int = 1
    step_mhz:  float = 0.0

    def to_card(self) -> str:
        if self.n_steps == 1:
            return f"FR 0 1 0 0 {self.start_mhz:.4f} 0"
        step = (self.stop_mhz - self.start_mhz) / max(1, self.n_steps - 1)
        return f"FR 0 {self.n_steps} 0 0 {self.start_mhz:.4f} {step:.6f}"


@dataclass
class RadiationPattern:
    """NEC RP card: radiation pattern request."""
    theta_start: float = 0.0      # degrees from Z axis
    theta_stop:  float = 180.0
    theta_inc:   float = 5.0
    phi_start:   float = 0.0
    phi_stop:    float = 360.0
    phi_inc:     float = 5.0
    normalize:   bool  = False
    power_gain:  bool  = True     # True = power gain; False = directive gain

    def to_card(self) -> str:
        n_theta = int((self.theta_stop - self.theta_start) / self.theta_inc) + 1
        n_phi   = int((self.phi_stop   - self.phi_start)   / self.phi_inc)   + 1
        xnda    = 0 if self.power_gain else 1
        return (f"RP {xnda} {n_theta} {n_phi} 1000 "
                f"{self.theta_start:.1f} {self.phi_start:.1f} "
                f"{self.theta_inc:.1f} {self.phi_inc:.1f}")


@dataclass
class GroundPlane:
    """NEC GN card: ground plane specification."""
    type:    int   = 1      # 0=free space, 1=perfect, 2=finite (Sommerfeld)
    eps_r:   float = 13.0   # Relative permittivity (good soil)
    sigma:   float = 0.005  # Conductivity (S/m); good soil = 0.005
    radials: int   = 0      # Number of radials for NEC-4 MININEC ground
    rad_len: float = 0.0    # Radial length (m)

    def to_card(self) -> str:
        if self.type == 0:
            return "GN -1"   # Free space
        if self.type == 1:
            return "GN 1"    # Perfect ground
        # Type 2: Sommerfeld (real ground)
        return f"GN 2 0 0 0 {self.eps_r:.2f} {self.sigma:.4f}"


@dataclass
class LoadCard:
    """NEC LD card: loading (lumped element on wire segment)."""
    tag:     int
    segment: int
    r:       float = 0.0    # Series resistance (Ω)
    l:       float = 0.0    # Series inductance (H)
    c:       float = 0.0    # Series capacitance (F)

    def to_card(self) -> str:
        return f"LD 0 {self.tag:3d} {self.segment:3d} 0 {self.r:.4f} {self.l:.8e} {self.c:.8e}"


@dataclass
class NetworkCard:
    """NEC NT card: two-port network (transmission line, transformer)."""
    tag1: int; seg1: int
    tag2: int; seg2: int
    y11r: float = 0.0; y11i: float = 0.0
    y12r: float = 0.0; y12i: float = 0.0
    y22r: float = 0.0; y22i: float = 0.0

    def to_card(self) -> str:
        return (f"NT {self.tag1} {self.seg1} {self.tag2} {self.seg2} "
                f"{self.y11r:.6e} {self.y11i:.6e} "
                f"{self.y12r:.6e} {self.y12i:.6e} "
                f"{self.y22r:.6e} {self.y22i:.6e}")


# ─── Main model container ─────────────────────────────────────────────────────
class NECModel:
    """Container for a complete NEC input file."""

    def __init__(self, title: str = "TM-MODEL-001 Antenna",
                 freq_mhz: float = 14.25):
        self.title       = title
        self.freq_mhz    = freq_mhz
        self.wires:       List[Wire]          = []
        self.excitations: List[Excitation]    = []
        self.frequencies: List[FrequencyCard] = []
        self.patterns:    List[RadiationPattern] = []
        self.loads:       List[LoadCard]      = []
        self.networks:    List[NetworkCard]   = []
        self.ground:      Optional[GroundPlane] = GroundPlane(type=0)
        self.comments:    List[str]           = []
        self._extra_cards: List[str]          = []   # Raw NEC card lines

    # ── Wire management ───────────────────────────────────────────────────────
    def add_wire(self, x1, y1, z1, x2, y2, z2,
                 radius=0.001, segs=None, tag=None) -> int:
        if tag is None:
            tag = len(self.wires) + 1
        if segs is None:
            length = math.sqrt((x2-x1)**2 + (y2-y1)**2 + (z2-z1)**2)
            segs = recommended_segments(length, self.freq_mhz)
        self.wires.append(Wire(tag, segs, x1, y1, z1, x2, y2, z2, radius))
        return tag

    def add_excitation(self, tag: int, segment: int = None, v: float = 1.0):
        if segment is None:
            # Default: center segment of the tagged wire
            wire = next(w for w in self.wires if w.tag == tag)
            segment = wire.segs // 2 + 1
        self.excitations.append(Excitation(tag, segment, v))

    def add_frequency(self, start_mhz, stop_mhz=None, n_steps=1):
        if stop_mhz is None or n_steps == 1:
            self.frequencies.append(FrequencyCard(start_mhz))
        else:
            self.frequencies.append(FrequencyCard(start_mhz, stop_mhz, n_steps))

    def add_pattern(self, theta_inc=5.0, phi_inc=5.0,
                    theta_start=0, theta_stop=180,
                    phi_start=0, phi_stop=360):
        self.patterns.append(RadiationPattern(
            theta_start, theta_stop, theta_inc,
            phi_start, phi_stop, phi_inc))

    def set_ground(self, type=1, eps_r=13.0, sigma=0.005):
        self.ground = GroundPlane(type=type, eps_r=eps_r, sigma=sigma)

    def add_load(self, tag, segment, r=0, l=0, c=0):
        self.loads.append(LoadCard(tag, segment, r, l, c))

    # ── File generation ───────────────────────────────────────────────────────
    def to_string(self) -> str:
        lines = []

        # Comment cards
        lines.append(f"CM {self.title}")
        lines.append(f"CM Generated by TM-MODEL-001 nec_generator.py")
        lines.append(f"CM Frequency: {self.freq_mhz} MHz")
        for c in self.comments:
            lines.append(f"CM {c}")
        lines.append("CE")

        # Geometry
        for wire in self.wires:
            lines.append(wire.to_card())

        # Ground plane
        if self.ground:
            lines.append(self.ground.to_card())

        lines.append("GE 1" if (self.ground and self.ground.type > 0) else "GE 0")

        # Loads
        for ld in self.loads:
            lines.append(ld.to_card())

        # Networks
        for nt in self.networks:
            lines.append(nt.to_card())

        # Excitations
        for ex in self.excitations:
            lines.append(ex.to_card())

        # Frequencies
        for fr in self.frequencies:
            lines.append(fr.to_card())

        # Extra user cards (EK, KH, etc.)
        for card in self._extra_cards:
            lines.append(card)

        # Radiation patterns
        for rp in self.patterns:
            lines.append(rp.to_card())

        lines.append("EN")
        return "\n".join(lines) + "\n"

    def write(self, path: str | Path):
        Path(path).write_text(self.to_string())

    def __str__(self) -> str:
        return self.to_string()


# ─── Antenna builder classes ──────────────────────────────────────────────────

class Dipole:
    """
    Center-fed half-wave dipole.
    Oriented along X axis, elevated to height_m, fed at center.
    """
    def __init__(self, freq_mhz: float = 14.25,
                 height_m: float = 10.0,
                 wire_radius_m: float = 0.001,
                 length_factor: float = 0.95,    # 0.95 = 5% end-effect shortening
                 orientation: str = "X"):        # "X" or "Y"
        self.freq_mhz      = freq_mhz
        self.height_m      = height_m
        self.wire_radius   = wire_radius_m
        self.length_factor = length_factor
        self.orientation   = orientation.upper()

    @property
    def half_length(self) -> float:
        wl = C_LIGHT / (self.freq_mhz * 1e6)
        return wl / 2 * self.length_factor / 2   # Half of the half-wave length

    def to_nec_model(self, ground_type: int = 0) -> NECModel:
        m = NECModel(
            title=f"Half-wave Dipole {self.freq_mhz} MHz h={self.height_m}m",
            freq_mhz=self.freq_mhz)
        m.comments.append(f"Half-length: {self.half_length:.3f} m")
        m.comments.append(f"Wire radius: {self.wire_radius*1000:.2f} mm")

        hl = self.half_length
        h  = self.height_m
        r  = self.wire_radius
        segs_half = max(11, recommended_segments(hl, self.freq_mhz, min_segs=5))
        # Ensure odd total number of segments: each half has same odd count
        if segs_half % 2 == 0:
            segs_half += 1

        if self.orientation == "X":
            m.add_wire(-hl, 0, h, 0, 0, h, radius=r, segs=segs_half, tag=1)
            m.add_wire(0, 0, h, hl, 0, h, radius=r, segs=segs_half, tag=2)
        else:
            m.add_wire(0, -hl, h, 0, 0, h, radius=r, segs=segs_half, tag=1)
            m.add_wire(0, 0, h, 0, hl, h, radius=r, segs=segs_half, tag=2)

        # Feed between center ends: EX on tag 1, last segment
        m.excitations.append(Excitation(tag=1, segment=segs_half))
        m.add_frequency(self.freq_mhz)
        m.set_ground(type=ground_type)
        m.add_pattern(theta_inc=5, phi_inc=5)
        return m


class VerticalWithRadials:
    """
    Quarter-wave vertical monopole over a radial ground system.
    Radials are horizontal, at ground level (z=0) or buried (z<0).
    """
    def __init__(self, freq_mhz: float = 7.15,
                 n_radials: int = 32,
                 radial_length_factor: float = 0.25,   # As fraction of λ
                 radial_height_m: float = 0.01,        # Height above ground
                 wire_radius_m: float = 0.002,
                 base_height_m: float = 0.01):
        self.freq_mhz      = freq_mhz
        self.n_radials     = n_radials
        self.radial_len_f  = radial_length_factor
        self.radial_h      = radial_height_m
        self.wire_radius   = wire_radius_m
        self.base_h        = base_height_m

    def to_nec_model(self, ground_type: int = 2,
                     eps_r: float = 13.0,
                     sigma: float = 0.005) -> NECModel:
        wl       = C_LIGHT / (self.freq_mhz * 1e6)
        vert_len = wl / 4 * 0.95   # Slightly shortened quarter-wave
        rad_len  = wl * self.radial_len_f
        r        = self.wire_radius

        m = NECModel(
            title=f"Vertical {self.freq_mhz} MHz, {self.n_radials} radials",
            freq_mhz=self.freq_mhz)

        segs_v = recommended_segments(vert_len, self.freq_mhz)
        m.add_wire(0, 0, self.base_h, 0, 0, self.base_h + vert_len,
                   radius=r, segs=segs_v, tag=1)

        segs_r = recommended_segments(rad_len, self.freq_mhz, min_segs=5)
        for i in range(self.n_radials):
            angle = 2 * math.pi * i / self.n_radials
            rx = rad_len * math.cos(angle)
            ry = rad_len * math.sin(angle)
            m.add_wire(0, 0, self.radial_h, rx, ry, self.radial_h,
                       radius=r*0.5, segs=segs_r, tag=i+2)

        m.excitations.append(Excitation(tag=1, segment=1))
        m.add_frequency(self.freq_mhz)
        m.set_ground(type=ground_type, eps_r=eps_r, sigma=sigma)
        m.add_pattern(theta_inc=2, phi_inc=5)
        return m


class Yagi:
    """
    Yagi-Uda antenna.
    All elements along Y axis; boom along X axis; elements parallel to Z=0.
    Reflector at x=0; driven element; directors ahead.
    """
    def __init__(self, freq_mhz: float = 144.2,
                 n_directors: int = 3,
                 height_m: float = 5.0,
                 wire_radius_m: float = 0.003):
        self.freq_mhz    = freq_mhz
        self.n_dirs      = n_directors
        self.height      = height_m
        self.wire_radius = wire_radius_m
        wl = C_LIGHT / (freq_mhz * 1e6)

        # Classic Yagi dimensions (Uda/Boom, normalized to λ)
        # Reflector: 0.482λ, spacing 0.2λ behind driven
        # Driven: 0.463λ  (dipole, slightly shortened)
        # Director 1: 0.440λ, spacing 0.2λ
        # Director N: 0.435λ, spacing 0.3λ each
        self.elements: List[Dict[str, float]] = []

        # Reflector
        self.elements.append({
            "name": "Reflector",
            "x": -0.2 * wl,
            "half_len": 0.482 * wl / 2,
            "driven": False
        })
        # Driven element
        self.elements.append({
            "name": "Driven",
            "x": 0.0,
            "half_len": 0.463 * wl / 2,
            "driven": True
        })
        # Directors
        for d in range(n_directors):
            director_hl = (0.440 - d * 0.002) * wl / 2  # Directors taper slightly
            director_x  = (0.2 + d * 0.3) * wl
            self.elements.append({
                "name": f"Director {d+1}",
                "x": director_x,
                "half_len": director_hl,
                "driven": False
            })

    def to_nec_model(self, ground_type: int = 0) -> NECModel:
        m = NECModel(
            title=f"Yagi {self.freq_mhz} MHz {self.n_dirs+2}-el",
            freq_mhz=self.freq_mhz)
        m.comments.append(f"Elements: Reflector + Driven + {self.n_dirs} Directors")

        driven_tag = None
        for tag, el in enumerate(self.elements, start=1):
            hl = el["half_len"]
            x  = el["x"]
            h  = self.height
            segs = recommended_segments(2*hl, self.freq_mhz)
            if segs % 2 == 0:
                segs += 1
            segs_half = segs // 2

            m.add_wire(x, -hl, h, x, 0, h,
                       radius=self.wire_radius, segs=segs_half, tag=tag*2-1)
            m.add_wire(x, 0, h, x, hl, h,
                       radius=self.wire_radius, segs=segs_half, tag=tag*2)

            if el["driven"]:
                driven_tag = tag * 2 - 1

        if driven_tag:
            m.excitations.append(Excitation(
                tag=driven_tag,
                segment=m.wires[driven_tag-1].segs))  # Last seg of first half

        m.add_frequency(self.freq_mhz)
        m.set_ground(type=ground_type)
        m.add_pattern(theta_inc=5, phi_inc=5)
        return m


class LogPeriodic:
    """
    Log-Periodic Dipole Array (LPDA).
    Tau and sigma define element progression and spacing.
    """
    def __init__(self, freq_low_mhz: float = 14.0,
                 freq_high_mhz: float = 30.0,
                 tau: float = 0.9,       # Length taper factor (0.7–0.98)
                 sigma: float = 0.07,    # Spacing factor (0.05–0.25)
                 height_m: float = 10.0,
                 wire_radius_m: float = 0.002):
        self.f_low    = freq_low_mhz
        self.f_high   = freq_high_mhz
        self.tau      = tau
        self.sigma    = sigma
        self.height   = height_m
        self.radius   = wire_radius_m

        wl_low  = C_LIGHT / (freq_low_mhz  * 1e6)
        wl_high = C_LIGHT / (freq_high_mhz * 1e6)

        l_max = wl_low  / 2 * 0.95
        l_min = wl_high / 2 * 0.95

        self.elements: List[Dict] = []
        l = l_max
        x = 0.0
        i = 0
        while l >= l_min:
            self.elements.append({"x": x, "half_len": l, "driven": (i == 0)})
            spacing = 4 * sigma * l
            x += spacing
            l *= tau
            i += 1

    def to_nec_model(self, ground_type: int = 0) -> NECModel:
        m = NECModel(
            title=f"LPDA {self.f_low}–{self.f_high} MHz τ={self.tau} σ={self.sigma}",
            freq_mhz=(self.f_low + self.f_high) / 2)
        m.comments.append(f"Elements: {len(self.elements)}")

        for tag_base, el in enumerate(self.elements, start=1):
            hl = el["half_len"]
            x  = el["x"]
            h  = self.height
            segs = max(5, recommended_segments(2*hl, (self.f_low + self.f_high)/2))
            if segs % 2 == 0:
                segs += 1
            s_half = segs // 2

            t1 = tag_base * 2 - 1
            t2 = tag_base * 2
            m.add_wire(x, -hl, h, x, 0, h, radius=self.radius, segs=s_half, tag=t1)
            m.add_wire(x, 0, h, x, hl, h, radius=self.radius, segs=s_half, tag=t2)

        # Feed at front element (highest frequency, tag 1)
        m.excitations.append(Excitation(tag=1, segment=m.wires[0].segs))
        m.add_frequency(self.f_low, self.f_high, n_steps=20)
        m.set_ground(type=ground_type)
        m.add_pattern(theta_inc=5, phi_inc=5)
        return m


class QuadLoop:
    """
    Square quad loop antenna. One wavelength per element.
    Supported on arms in a diamond orientation.
    """
    def __init__(self, freq_mhz: float = 21.3,
                 height_m: float = 12.0,
                 wire_radius_m: float = 0.001,
                 n_elements: int = 2):    # 1=single, 2=two-element (reflector+driven)
        self.freq_mhz  = freq_mhz
        self.height    = height_m
        self.radius    = wire_radius_m
        self.n_el      = n_elements

    def to_nec_model(self, ground_type: int = 0) -> NECModel:
        wl   = C_LIGHT / (self.freq_mhz * 1e6)
        side = wl / 4          # Side length of square loop = λ/4
        h    = self.height
        m    = NECModel(
            title=f"Quad Loop {self.freq_mhz} MHz",
            freq_mhz=self.freq_mhz)

        def add_quad(x_offset, tag_start, driven=False) -> int:
            """Add a square quad loop at x_offset, return next available tag."""
            s = side
            # Diamond orientation: top, right, bottom, left corners
            corners = [
                (x_offset, 0,  h + s),   # top
                (x_offset, s,  h),        # right
                (x_offset, 0,  h - s),   # bottom
                (x_offset, -s, h),        # left
            ]
            segs = recommended_segments(s, self.freq_mhz, min_segs=5)
            for i in range(4):
                p1 = corners[i]
                p2 = corners[(i+1) % 4]
                t = tag_start + i
                m.add_wire(p1[0], p1[1], p1[2],
                           p2[0], p2[1], p2[2],
                           radius=self.radius, segs=segs, tag=t)
            if driven:
                # Feed at bottom wire center
                feed_tag = tag_start + 2  # bottom wire tag
                m.excitations.append(Excitation(
                    tag=feed_tag, segment=segs//2+1))
            return tag_start + 4

        next_tag = add_quad(0, 1, driven=True)
        if self.n_el >= 2:
            add_quad(-wl * 0.2, next_tag, driven=False)  # Reflector behind

        m.add_frequency(self.freq_mhz)
        m.set_ground(type=ground_type)
        m.add_pattern(theta_inc=5, phi_inc=5)
        return m


class PhasedArray:
    """
    N-element phased vertical array with configurable spacing and phasing.
    Common patterns: cardioid (90/90), endfire (180/180), broadside (0/λ/2).
    """
    def __init__(self, freq_mhz: float = 7.15,
                 n_elements: int = 2,
                 spacing_m: float = None,       # None = λ/4 spacing
                 phase_deg: List[float] = None, # Phase of each element (degrees)
                 current_ratio: List[float] = None,  # Amplitude ratio
                 height_m: float = 0.01,
                 wire_radius_m: float = 0.002):
        self.freq_mhz = freq_mhz
        self.n_el     = n_elements
        wl = C_LIGHT / (freq_mhz * 1e6)
        self.spacing  = spacing_m if spacing_m else wl / 4
        self.phase    = phase_deg if phase_deg else [i * -90.0 for i in range(n_elements)]
        self.current  = current_ratio if current_ratio else [1.0] * n_elements
        self.height   = height_m
        self.radius   = wire_radius_m

    def to_nec_model(self, ground_type: int = 2,
                     eps_r: float = 13.0,
                     sigma: float = 0.005) -> NECModel:
        wl      = C_LIGHT / (self.freq_mhz * 1e6)
        vert_l  = wl / 4 * 0.95
        m       = NECModel(
            title=f"{self.n_el}-Element Phased Array {self.freq_mhz} MHz",
            freq_mhz=self.freq_mhz)

        segs = recommended_segments(vert_l, self.freq_mhz)
        for i in range(self.n_el):
            x = i * self.spacing
            m.add_wire(x, 0, self.height, x, 0, self.height + vert_l,
                       radius=self.radius, segs=segs, tag=i+1)
            # Complex voltage source for each element
            phase_rad = math.radians(self.phase[i])
            v_r = self.current[i] * math.cos(phase_rad)
            v_i = self.current[i] * math.sin(phase_rad)
            m.excitations.append(Excitation(tag=i+1, segment=1,
                                            v_real=v_r, v_imag=v_i))

        m.add_frequency(self.freq_mhz)
        m.set_ground(type=ground_type, eps_r=eps_r, sigma=sigma)
        m.add_pattern(theta_inc=2, phi_inc=2)
        return m


# ─── Convenience function ─────────────────────────────────────────────────────
def build_from_dict(spec: Dict[str, Any]) -> NECModel:
    """
    Build a NECModel from a flat specification dictionary.

    spec keys:
        type:       "dipole" | "vertical" | "yagi" | "lpda" | "quad" | "phased"
        freq_mhz:   center frequency
        height_m:   feed or antenna height above ground
        ... (type-specific keys)

    Returns NECModel ready for writing.
    """
    t = spec.get("type", "dipole").lower()
    f = spec.get("freq_mhz", 14.25)
    h = spec.get("height_m", 10.0)
    r = spec.get("wire_radius_m", 0.001)

    if t == "dipole":
        ant = Dipole(freq_mhz=f, height_m=h, wire_radius_m=r,
                     length_factor=spec.get("length_factor", 0.95),
                     orientation=spec.get("orientation", "X"))
    elif t == "vertical":
        ant = VerticalWithRadials(freq_mhz=f,
                                   n_radials=spec.get("n_radials", 32),
                                   wire_radius_m=r)
    elif t == "yagi":
        ant = Yagi(freq_mhz=f, n_directors=spec.get("n_directors", 3),
                   height_m=h, wire_radius_m=r)
    elif t == "lpda":
        ant = LogPeriodic(freq_low_mhz=spec.get("freq_low_mhz", f*0.5),
                          freq_high_mhz=spec.get("freq_high_mhz", f*2.0),
                          tau=spec.get("tau", 0.9), sigma=spec.get("sigma", 0.07),
                          height_m=h, wire_radius_m=r)
    elif t == "quad":
        ant = QuadLoop(freq_mhz=f, height_m=h, wire_radius_m=r,
                       n_elements=spec.get("n_elements", 2))
    elif t == "phased":
        ant = PhasedArray(freq_mhz=f,
                          n_elements=spec.get("n_elements", 2),
                          spacing_m=spec.get("spacing_m", None),
                          phase_deg=spec.get("phase_deg", None),
                          wire_radius_m=r)
    else:
        raise ValueError(f"Unknown antenna type: {t}")

    gnd = spec.get("ground_type", 0)
    return ant.to_nec_model(ground_type=gnd)


if __name__ == "__main__":
    import sys

    print("=== TM-MODEL-001 NEC Generator — Self-Test ===")

    # Dipole
    d = Dipole(freq_mhz=14.25, height_m=10.0)
    m = d.to_nec_model(ground_type=2)
    m.write("/tmp/test_dipole.nec")
    print(f"Dipole: {len(m.wires)} wires written to /tmp/test_dipole.nec")

    # Yagi
    y = Yagi(freq_mhz=144.2, n_directors=5, height_m=6.0, wire_radius_m=0.004)
    my = y.to_nec_model(ground_type=0)
    my.write("/tmp/test_yagi.nec")
    print(f"Yagi: {len(my.wires)} wires, /tmp/test_yagi.nec")

    # Vertical with radials
    v = VerticalWithRadials(freq_mhz=7.15, n_radials=32)
    mv = v.to_nec_model(ground_type=2)
    mv.write("/tmp/test_vertical.nec")
    print(f"Vertical+radials: {len(mv.wires)} wires, /tmp/test_vertical.nec")

    # LPDA
    lp = LogPeriodic(14.0, 30.0, tau=0.9, sigma=0.07, height_m=12.0)
    ml = lp.to_nec_model()
    ml.write("/tmp/test_lpda.nec")
    print(f"LPDA: {len(ml.elements)} elements, /tmp/test_lpda.nec")

    # dict-based build
    spec = {"type": "phased", "freq_mhz": 7.15, "n_elements": 4,
            "phase_deg": [0, -90, -180, -270], "height_m": 0.01}
    mp = build_from_dict(spec)
    mp.write("/tmp/test_phased.nec")
    print(f"Phased array: {len(mp.wires)} wires, /tmp/test_phased.nec")

    print("Self-test complete.")
