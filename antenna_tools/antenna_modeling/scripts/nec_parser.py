"""
nec_parser.py
TM-PARSER-001 Rev A — NEC Output File Parser

Parses NEC2/NEC4 .out files. Extracts impedance, gain, SWR,
radiation patterns, and efficiency. Handles multiple frequency steps
and differences between NEC2 and NEC4 output formats.

Usage:
    parser = NECOutputParser()
    points = parser.parse(Path("dipole.out"))
    for pt in points:
        print(f"{pt.freq_mhz:.3f} MHz  SWR={pt.swr_50:.2f}  Gain={pt.gain_dbi_max:.1f} dBi")
"""

from __future__ import annotations

import logging
import math
import re
from dataclasses import dataclass, field
from pathlib import Path
from typing import List, Optional, Tuple

import numpy as np

log = logging.getLogger(__name__)


# ─── Data Containers ─────────────────────────────────────────────────────────

@dataclass
class PatternPoint:
    theta_deg: float
    phi_deg:   float
    gain_dbi:  float
    e_theta:   float = 0.0   # E-field theta component (V/m normalized)
    e_phi:     float = 0.0   # E-field phi component


@dataclass
class NECFreqPoint:
    """All antenna performance parameters at one frequency."""
    freq_mhz:      float
    r_ohm:         float       # Feed impedance real part
    x_ohm:         float       # Feed impedance imaginary part
    swr_50:        float       # SWR referenced to 50Ω
    swr_75:        float       # SWR referenced to 75Ω
    gain_dbi_max:  float       # Maximum gain (dBi)
    gain_theta_deg: float      # Theta of max gain direction
    gain_phi_deg:   float      # Phi of max gain direction
    fb_ratio_db:   float       # Front-to-back ratio (dB)
    efficiency_pct: float      # Radiation efficiency (%)
    bw_3db_e_deg:  float       # 3dB beamwidth E-plane (degrees)
    bw_3db_h_deg:  float       # 3dB beamwidth H-plane (degrees)
    pattern:       Optional[np.ndarray] = field(default=None, repr=False)
    # pattern shape: [n_theta, n_phi, 3]  cols: theta_deg, phi_deg, gain_dBi

    @property
    def z_mag(self) -> float:
        return math.sqrt(self.r_ohm**2 + self.x_ohm**2)

    @property
    def z_phase_deg(self) -> float:
        return math.degrees(math.atan2(self.x_ohm, self.r_ohm))

    @property
    def return_loss_db(self) -> float:
        gamma = self._gamma(50.0)
        if abs(gamma) >= 1.0:
            return 0.0
        return -20 * math.log10(abs(gamma))

    def _gamma(self, z0: float) -> complex:
        z = complex(self.r_ohm, self.x_ohm)
        return (z - z0) / (z + z0)


# ─── Parser ───────────────────────────────────────────────────────────────────

class NECOutputParser:
    """
    Parse NEC2/NEC4 .out files into a list of NECFreqPoint objects.

    NEC output is structured into blocks separated by lines of asterisks.
    Key sections:
      - "ANTENNA INPUT PARAMETERS" — feed impedance per frequency
      - "RADIATION PATTERNS"       — gain vs theta/phi
      - "POWER BUDGET"             — efficiency
    """

    # ── Compiled patterns ──────────────────────────────────────────────────────
    _RE_FREQ      = re.compile(r"FREQUENCY\s*=\s*([\d.]+(?:E[+-]?\d+)?)\s*MHZ", re.I)
    _RE_FREQ2     = re.compile(r"FREQ\s*=\s*([\d.]+)\s*MHz", re.I)
    _RE_IMPEDANCE = re.compile(
        r"IMPEDANCE\s*=\s*([-\d.E+]+)\s*\+\s*J\s*([-\d.E+]+)", re.I)
    _RE_IMPED_ALT = re.compile(
        r"([-\d.]+)\s+([-\d.]+)\s+REAL\s+IMAG", re.I)
    # NEC antenna input parameters table line:
    # TAG  SEG  VOLTAGE(REAL  IMAG)  CURRENT(REAL  IMAG)  IMPEDANCE(REAL  IMAG)  ADMITTANCE POWER
    _RE_INPUT_TABLE = re.compile(
        r"^\s*\d+\s+\d+\s+"               # TAG SEG
        r"([-\d.E+]+)\s+([-\d.E+]+)\s+"   # voltage real imag
        r"([-\d.E+]+)\s+([-\d.E+]+)\s+"   # current real imag
        r"([-\d.E+]+)\s+([-\d.E+]+)"      # impedance real imag
    )
    _RE_RP_LINE   = re.compile(
        r"^\s*([\d.]+)\s+([\d.]+)\s+"      # theta phi
        r"([-\d.]+)\s+([-\d.]+)\s+"        # V-pol dB, H-pol dB (or total then v/h)
        r"([-\d.]+)"                        # total dB (some formats)
    )
    _RE_EFFICIENCY = re.compile(
        r"RADIATION\s+EFFICIENCY\s*=\s*([-\d.]+)\s*%", re.I)
    _RE_POWER      = re.compile(
        r"RADIATED\s+POWER\s*=\s*([-\d.E+]+)", re.I)

    # ── Public API ────────────────────────────────────────────────────────────

    def parse(self, output_file: Path) -> List[NECFreqPoint]:
        """
        Parse a NEC .out file.

        Returns
        -------
        List[NECFreqPoint]
            One entry per frequency in the output.
            Empty list if parsing fails.
        """
        text = Path(output_file).read_text(errors="replace")
        return self._parse_text(text)

    def parse_string(self, text: str) -> List[NECFreqPoint]:
        return self._parse_text(text)

    # ── Internal Parsing ─────────────────────────────────────────────────────

    def _parse_text(self, text: str) -> List[NECFreqPoint]:
        blocks = self._split_freq_blocks(text)
        if not blocks:
            # Single-frequency file (no block separators)
            blocks = [text]

        results = []
        for block in blocks:
            pt = self._parse_block(block)
            if pt is not None:
                results.append(pt)

        log.info("Parsed %d frequency points from NEC output", len(results))
        return results

    def _split_freq_blocks(self, text: str) -> List[str]:
        """Split multi-frequency output at frequency announcement lines."""
        # NEC prints a separator and "FREQUENCY = x.xx MHZ" for each step
        pattern = re.compile(r"(?=\s*FREQUENCY\s*=\s*[\d.]+\s*MHZ)", re.I)
        parts = pattern.split(text)
        return [p for p in parts if p.strip()]

    def _parse_block(self, block: str) -> Optional[NECFreqPoint]:
        """Parse one frequency block into a NECFreqPoint."""
        freq = self._extract_frequency(block)
        if freq is None:
            return None

        r, x     = self._extract_impedance(block)
        eff      = self._extract_efficiency(block)
        pattern  = self._extract_pattern(block)

        swr_50   = self.calc_swr(r, x, 50.0)
        swr_75   = self.calc_swr(r, x, 75.0)

        if pattern is not None and len(pattern) > 0:
            gain_max, theta_max, phi_max = self.calc_max_gain(pattern)
            fb       = self.calc_fb_ratio(pattern)
            bw_e     = self.calc_3db_beamwidth(pattern, plane='E')
            bw_h     = self.calc_3db_beamwidth(pattern, plane='H')
        else:
            gain_max = theta_max = phi_max = float('nan')
            fb = bw_e = bw_h = float('nan')

        return NECFreqPoint(
            freq_mhz=freq,
            r_ohm=r,
            x_ohm=x,
            swr_50=swr_50,
            swr_75=swr_75,
            gain_dbi_max=gain_max,
            gain_theta_deg=theta_max,
            gain_phi_deg=phi_max,
            fb_ratio_db=fb,
            efficiency_pct=eff,
            bw_3db_e_deg=bw_e,
            bw_3db_h_deg=bw_h,
            pattern=pattern
        )

    # ── Field Extractors ─────────────────────────────────────────────────────

    def _extract_frequency(self, block: str) -> Optional[float]:
        m = self._RE_FREQ.search(block)
        if m:
            return float(m.group(1))
        m = self._RE_FREQ2.search(block)
        if m:
            return float(m.group(1))
        return None

    def _extract_impedance(self, block: str) -> Tuple[float, float]:
        """Return (R, X) ohms. Tries multiple NEC output formats."""
        # Format 1: IMPEDANCE = R + J X
        m = self._RE_IMPEDANCE.search(block)
        if m:
            return float(m.group(1)), float(m.group(2))

        # Format 2: ANTENNA INPUT PARAMETERS table
        # Look for table header, then parse data line
        if "ANTENNA INPUT PARAMETERS" in block.upper():
            for line in block.splitlines():
                m = self._RE_INPUT_TABLE.match(line)
                if m:
                    return float(m.group(5)), float(m.group(6))

        # Format 3: simple two-column R X line
        for line in block.splitlines():
            parts = line.split()
            if len(parts) >= 2:
                try:
                    r, x = float(parts[0]), float(parts[1])
                    if 0.1 < abs(r) < 1e6:   # Sanity range
                        return r, x
                except ValueError:
                    pass

        log.warning("Could not extract impedance from block")
        return float('nan'), float('nan')

    def _extract_efficiency(self, block: str) -> float:
        m = self._RE_EFFICIENCY.search(block)
        if m:
            return float(m.group(1))
        # Try power budget approach: efficiency = radiated / input
        # (less reliable, skip for now)
        return float('nan')

    def _extract_pattern(self, block: str) -> Optional[np.ndarray]:
        """
        Extract radiation pattern data.
        Returns ndarray shape [N, 3]: [theta_deg, phi_deg, gain_dBi]
        or None if no pattern in block.
        """
        if "RADIATION PATTERNS" not in block.upper():
            return None

        points = []
        in_pattern = False
        for line in block.splitlines():
            if "RADIATION PATTERNS" in line.upper():
                in_pattern = True
                continue
            if not in_pattern:
                continue
            # Skip header lines and separator lines
            if re.match(r"^\s*[-*=]+\s*$", line):
                continue
            if re.match(r"^\s*(THETA|PHI|DEGREES|GAIN)", line, re.I):
                continue
            parts = line.split()
            if len(parts) < 3:
                continue
            try:
                theta = float(parts[0])
                phi   = float(parts[1])
                # NEC output columns vary: some have V-pol, H-pol, total
                # Try to get total gain (last numeric column or 3rd)
                gain = float(parts[-1])
                if not math.isfinite(gain):
                    gain = float(parts[2])
                points.append([theta, phi, gain])
            except (ValueError, IndexError):
                continue

        if not points:
            return None
        return np.array(points, dtype=float)

    # ── Derived Calculations ─────────────────────────────────────────────────

    @staticmethod
    def calc_swr(r: float, x: float, z0: float = 50.0) -> float:
        """Calculate SWR from impedance."""
        if not (math.isfinite(r) and math.isfinite(x)):
            return float('nan')
        z = complex(r, x)
        gamma = abs((z - z0) / (z + z0))
        if gamma >= 1.0:
            return 99.9
        return round((1 + gamma) / (1 - gamma), 3)

    @staticmethod
    def calc_max_gain(pattern: np.ndarray) -> Tuple[float, float, float]:
        """
        Find maximum gain and its direction.

        Returns
        -------
        (gain_dBi, theta_deg, phi_deg)
        """
        if pattern is None or len(pattern) == 0:
            return float('nan'), float('nan'), float('nan')
        idx      = np.argmax(pattern[:, 2])
        return pattern[idx, 2], pattern[idx, 0], pattern[idx, 1]

    @staticmethod
    def calc_fb_ratio(pattern: np.ndarray) -> float:
        """
        Front-to-back ratio in dB.
        Assumes forward direction = phi where max gain occurs at low theta.
        Uses max gain vs 180° rotated phi at same theta.
        """
        if pattern is None or len(pattern) == 0:
            return float('nan')
        idx_max  = np.argmax(pattern[:, 2])
        theta_f  = pattern[idx_max, 0]
        phi_f    = pattern[idx_max, 1]
        phi_b    = (phi_f + 180.0) % 360.0

        # Find points near back direction
        back_mask = (
            (np.abs(pattern[:, 0] - theta_f) < 10.0) &
            (np.abs(((pattern[:, 1] - phi_b + 180) % 360) - 180) < 15.0)
        )
        if not np.any(back_mask):
            return float('nan')
        gain_back = np.max(pattern[back_mask, 2])
        return round(pattern[idx_max, 2] - gain_back, 2)

    @staticmethod
    def calc_3db_beamwidth(pattern: np.ndarray, plane: str = 'E') -> float:
        """
        Estimate 3dB beamwidth in E-plane (theta sweep at max-gain phi)
        or H-plane (phi sweep at max-gain theta).

        Returns degrees.
        """
        if pattern is None or len(pattern) == 0:
            return float('nan')

        idx_max = np.argmax(pattern[:, 2])
        gain_max = pattern[idx_max, 2]
        threshold = gain_max - 3.0

        if plane.upper() == 'E':
            # E-plane: vary theta at fixed phi
            phi_fix = pattern[idx_max, 1]
            mask    = np.abs(((pattern[:, 1] - phi_fix + 180) % 360) - 180) < 5.0
            sub     = pattern[mask]
            if len(sub) < 3:
                return float('nan')
            above   = sub[sub[:, 2] >= threshold, 0]
        else:
            # H-plane: vary phi at fixed theta
            theta_fix = pattern[idx_max, 0]
            mask      = np.abs(pattern[:, 0] - theta_fix) < 5.0
            sub       = pattern[mask]
            if len(sub) < 3:
                return float('nan')
            above     = sub[sub[:, 2] >= threshold, 1]

        if len(above) < 2:
            return float('nan')
        return round(float(np.max(above) - np.min(above)), 1)
