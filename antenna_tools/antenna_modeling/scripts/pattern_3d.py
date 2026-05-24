"""
pattern_3d.py
TM-PATTERN-001 Rev A — 3D Radiation Pattern Export

Converts NEC radiation pattern arrays to multiple 3D visualization formats:
  JSON   — web viewer (Three.js / custom HTML)
  VTK    — ParaView / VisIt (legacy ASCII PolyData)
  PLY    — mesh viewers (Blender, MeshLab)
  CSV    — generic
  Matplotlib 3D surface — inline visualization

Usage:
    from nec_parser import NECOutputParser
    from pattern_3d import Pattern3D

    parser = NECOutputParser()
    pts = parser.parse(Path("yagi.out"))
    p3d = Pattern3D.from_freq_point(pts[0])
    p3d.to_json("yagi_pattern.json", freq_mhz=144.2)
    p3d.to_vtk("yagi_pattern.vtk")
    p3d.to_matplotlib_3d()
"""

from __future__ import annotations

import json
import logging
import math
import struct
from pathlib import Path
from typing import Dict, List, Optional

import numpy as np

log = logging.getLogger(__name__)


class Pattern3D:
    """
    3D radiation pattern container and exporter.

    Parameters
    ----------
    theta_deg : np.ndarray  shape [N]
    phi_deg   : np.ndarray  shape [M]
    gain_dBi  : np.ndarray  shape [N, M]  (or flat [K, 3] from parser)
    """

    def __init__(self, theta_deg: np.ndarray, phi_deg: np.ndarray,
                 gain_dBi: np.ndarray):
        self.theta = np.asarray(theta_deg, dtype=float)
        self.phi   = np.asarray(phi_deg,   dtype=float)
        self.gain  = np.asarray(gain_dBi,  dtype=float)

    @classmethod
    def from_freq_point(cls, freq_point) -> "Pattern3D":
        """Build from a NECFreqPoint with a .pattern array."""
        pat = freq_point.pattern
        if pat is None or len(pat) == 0:
            raise ValueError("NECFreqPoint has no pattern data. "
                             "Ensure RP card was in the NEC model.")

        # pat shape: [K, 3]  cols: theta, phi, gain_dBi
        thetas = np.unique(pat[:, 0])
        phis   = np.unique(pat[:, 1])

        # Reshape to 2D grid
        n_t = len(thetas)
        n_p = len(phis)
        gain_grid = np.full((n_t, n_p), float('nan'))

        for row in pat:
            ti = np.searchsorted(thetas, row[0])
            pi = np.searchsorted(phis,   row[1])
            if ti < n_t and pi < n_p:
                gain_grid[ti, pi] = row[2]

        return cls(thetas, phis, gain_grid)

    @classmethod
    def from_flat_array(cls, flat: np.ndarray) -> "Pattern3D":
        """Build from flat [K, 3] array [theta, phi, gain_dBi]."""
        thetas = np.unique(flat[:, 0])
        phis   = np.unique(flat[:, 1])
        gain   = np.full((len(thetas), len(phis)), float('nan'))
        for row in flat:
            ti = int(np.searchsorted(thetas, row[0]))
            pi = int(np.searchsorted(phis,   row[1]))
            if ti < len(thetas) and pi < len(phis):
                gain[ti, pi] = row[2]
        return cls(thetas, phis, gain)

    # ── Coordinate Conversion ─────────────────────────────────────────────────

    def to_cartesian(self, normalize: bool = True):
        """
        Convert spherical gain surface to cartesian coordinates.

        Returns
        -------
        x, y, z : np.ndarray  shape [n_theta, n_phi]
        gain_norm : np.ndarray  (same shape, gain normalized 0–1)
        """
        gain_valid = np.where(np.isfinite(self.gain), self.gain, np.nanmin(self.gain))
        if normalize:
            g_max = np.nanmax(gain_valid)
            g_min = np.nanmin(gain_valid)
            # Map gain to radius 0.05–1.0 (avoid zero-radius artifacts)
            radius = 0.05 + 0.95 * (gain_valid - g_min) / max(g_max - g_min, 0.01)
        else:
            # Linear amplitude: r = 10^(dBi/20)
            radius = 10 ** (gain_valid / 20)
            radius /= np.max(radius)

        T, P = np.meshgrid(np.radians(self.theta), np.radians(self.phi),
                           indexing='ij')
        x = radius * np.sin(T) * np.cos(P)
        y = radius * np.sin(T) * np.sin(P)
        z = radius * np.cos(T)
        return x, y, z, radius

    # ── JSON Export ───────────────────────────────────────────────────────────

    def to_json(self, path: str | Path, freq_mhz: float = None):
        """
        Export to JSON for Three.js or custom web viewer.
        Format:
          {"freq_mhz": ..., "max_gain_dBi": ...,
           "points": [{"theta":..,"phi":..,"gain_dBi":..,"x":..,"y":..,"z":..}, ...]}
        """
        x, y, z, r = self.to_cartesian()
        points = []
        for ti, theta in enumerate(self.theta):
            for pi, phi in enumerate(self.phi):
                g = self.gain[ti, pi]
                if not math.isfinite(g):
                    continue
                points.append({
                    "theta": round(float(theta), 1),
                    "phi":   round(float(phi),   1),
                    "gain_dBi": round(float(g), 2),
                    "x": round(float(x[ti, pi]), 4),
                    "y": round(float(y[ti, pi]), 4),
                    "z": round(float(z[ti, pi]), 4),
                })

        obj = {
            "freq_mhz": freq_mhz,
            "max_gain_dBi": round(float(np.nanmax(self.gain)), 2),
            "min_gain_dBi": round(float(np.nanmin(self.gain)), 2),
            "n_points": len(points),
            "points": points
        }
        Path(path).write_text(json.dumps(obj, indent=None, separators=(',', ':')))
        log.info("Pattern JSON → %s (%d points)", path, len(points))

    # ── VTK Export ────────────────────────────────────────────────────────────

    def to_vtk(self, path: str | Path):
        """
        Export to legacy ASCII VTK PolyData for ParaView / VisIt.
        Gain encoded as POINT_DATA scalar field.
        """
        x, y, z, _ = self.to_cartesian()
        n_t, n_p   = x.shape

        # Points and connectivity
        pts   = []
        cells = []
        gain_vals = []

        for ti in range(n_t):
            for pi in range(n_p):
                pts.append((x[ti, pi], y[ti, pi], z[ti, pi]))
                gain_vals.append(self.gain[ti, pi] if math.isfinite(self.gain[ti, pi]) else 0.0)

        # Build quad cells (wrap phi)
        for ti in range(n_t - 1):
            for pi in range(n_p):
                p0 = ti * n_p + pi
                p1 = ti * n_p + (pi + 1) % n_p
                p2 = (ti + 1) * n_p + (pi + 1) % n_p
                p3 = (ti + 1) * n_p + pi
                cells.append((p0, p1, p2, p3))

        n_pts   = len(pts)
        n_cells = len(cells)

        lines = [
            "# vtk DataFile Version 3.0",
            "NEC Radiation Pattern",
            "ASCII",
            "DATASET POLYDATA",
            f"POINTS {n_pts} float",
        ]
        for px, py, pz in pts:
            lines.append(f"{px:.5f} {py:.5f} {pz:.5f}")

        lines.append(f"POLYGONS {n_cells} {n_cells * 5}")
        for c in cells:
            lines.append(f"4 {c[0]} {c[1]} {c[2]} {c[3]}")

        lines += [
            f"POINT_DATA {n_pts}",
            "SCALARS gain_dBi float 1",
            "LOOKUP_TABLE default",
        ]
        for g in gain_vals:
            lines.append(f"{g:.4f}")

        Path(path).write_text("\n".join(lines))
        log.info("VTK → %s", path)

    # ── PLY Export ────────────────────────────────────────────────────────────

    def to_ply(self, path: str | Path):
        """
        Export to ASCII PLY with vertex color (gain → colormap).
        """
        x, y, z, _ = self.to_cartesian()
        n_t, n_p   = x.shape

        verts = []
        g_min = np.nanmin(self.gain)
        g_max = np.nanmax(self.gain)
        dg    = max(g_max - g_min, 0.01)

        for ti in range(n_t):
            for pi in range(n_p):
                gval = self.gain[ti, pi] if math.isfinite(self.gain[ti, pi]) else g_min
                norm = (gval - g_min) / dg   # 0–1
                # Colormap: blue(0) → green(0.5) → red(1.0)
                r = int(min(255, max(0, 255 * min(1.0, 2 * norm - 1.0))))
                g = int(min(255, max(0, 255 * (1 - abs(2 * norm - 1)))))
                b = int(min(255, max(0, 255 * min(1.0, 1.0 - 2 * norm))))
                verts.append((x[ti, pi], y[ti, pi], z[ti, pi], r, g, b))

        faces = []
        for ti in range(n_t - 1):
            for pi in range(n_p):
                p0 = ti * n_p + pi
                p1 = ti * n_p + (pi + 1) % n_p
                p2 = (ti + 1) * n_p + (pi + 1) % n_p
                p3 = (ti + 1) * n_p + pi
                faces.append((p0, p1, p2, p3))

        lines = [
            "ply", "format ascii 1.0",
            f"element vertex {len(verts)}",
            "property float x", "property float y", "property float z",
            "property uchar red", "property uchar green", "property uchar blue",
            f"element face {len(faces)}",
            "property list uchar int vertex_indices",
            "end_header"
        ]
        for vx, vy, vz, r, g, b in verts:
            lines.append(f"{vx:.5f} {vy:.5f} {vz:.5f} {r} {g} {b}")
        for f in faces:
            lines.append(f"4 {f[0]} {f[1]} {f[2]} {f[3]}")

        Path(path).write_text("\n".join(lines))
        log.info("PLY → %s", path)

    # ── CSV Export ────────────────────────────────────────────────────────────

    def to_csv(self, path: str | Path):
        """Simple CSV: theta_deg, phi_deg, gain_dBi, gain_linear, x_norm, y_norm, z_norm"""
        x, y, z, r = self.to_cartesian()
        rows = ["theta_deg,phi_deg,gain_dBi,gain_linear,x_norm,y_norm,z_norm"]
        for ti, theta in enumerate(self.theta):
            for pi, phi in enumerate(self.phi):
                g = self.gain[ti, pi]
                rows.append(f"{theta:.1f},{phi:.1f},{g:.3f},{r[ti,pi]:.5f},"
                             f"{x[ti,pi]:.5f},{y[ti,pi]:.5f},{z[ti,pi]:.5f}")
        Path(path).write_text("\n".join(rows))
        log.info("Pattern CSV → %s", path)

    # ── Matplotlib 3D ─────────────────────────────────────────────────────────

    def to_matplotlib_3d(self, ax=None, freq_mhz: float = None,
                         cmap: str = "plasma", show: bool = True):
        """
        Plot 3D surface radiation pattern.

        Parameters
        ----------
        ax : Axes3D, optional
        freq_mhz : float, optional  (for title)
        cmap : str  colormap name
        show : bool  call plt.show()

        Returns
        -------
        matplotlib Axes3D
        """
        import matplotlib.pyplot as plt
        from mpl_toolkits.mplot3d import Axes3D

        x, y, z, r = self.to_cartesian()

        if ax is None:
            fig = plt.figure(figsize=(10, 8))
            ax  = fig.add_subplot(111, projection='3d')

        norm = (self.gain - np.nanmin(self.gain)) / max(
            np.nanmax(self.gain) - np.nanmin(self.gain), 0.01)
        import matplotlib.cm as cm
        colors = cm.get_cmap(cmap)(norm)

        ax.plot_surface(x, y, z, facecolors=colors, alpha=0.85,
                        linewidth=0, antialiased=True)

        title = "3D Radiation Pattern"
        if freq_mhz:
            title += f" — {freq_mhz:.3f} MHz"
        title += f"\nMax Gain: {np.nanmax(self.gain):.1f} dBi"
        ax.set_title(title)
        ax.set_xlabel("X (East)")
        ax.set_ylabel("Y (North)")
        ax.set_zlabel("Z (Up)")

        # Add colorbar
        m = cm.ScalarMappable(cmap=cmap)
        m.set_array(self.gain)
        plt.colorbar(m, ax=ax, label="Gain (dBi)", shrink=0.5)

        if show:
            plt.show()
        return ax


class MultiFreqPattern:
    """Store patterns at multiple frequencies; export animated JSON."""

    def __init__(self):
        self._patterns: Dict[float, Pattern3D] = {}

    def add_freq(self, freq_mhz: float, pattern: Pattern3D):
        self._patterns[freq_mhz] = pattern

    def to_animated_json(self, path: str | Path):
        """
        Export multi-frame JSON for animated web viewer.
        Each frequency is one animation frame.
        """
        frames = []
        for freq in sorted(self._patterns.keys()):
            p3d = self._patterns[freq]
            x, y, z, r = p3d.to_cartesian()
            pts = []
            for ti in range(len(p3d.theta)):
                for pi in range(len(p3d.phi)):
                    g = p3d.gain[ti, pi]
                    if math.isfinite(g):
                        pts.append([round(float(x[ti,pi]),4),
                                    round(float(y[ti,pi]),4),
                                    round(float(z[ti,pi]),4),
                                    round(float(g),2)])
            frames.append({"freq_mhz": freq, "max_gain_dBi": float(np.nanmax(p3d.gain)),
                           "points": pts})

        Path(path).write_text(json.dumps({"frames": frames}, separators=(',', ':')))
        log.info("Animated pattern JSON → %s (%d frames)", path, len(frames))
