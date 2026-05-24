"""
freq_sweep.py
TM-SWEEP-001 Rev A — Automated Frequency Sweep Engine

Generates per-frequency NEC models, runs them in sequence or parallel,
parses output, and assembles a structured SweepResult with pandas DataFrame.

Usage:
    from nec_generator import Dipole, NECModel
    from freq_sweep import FreqSweep, SweepConfig, sweep_dipole

    # Simple one-liner
    result = sweep_dipole(14.0, 14.35, 0.05, height_m=10.0)
    result.to_csv("dipole_20m_sweep.csv")
    print(result.to_dataframe())

    # Custom builder
    def my_yagi(freq_mhz):
        from nec_generator import Yagi
        ant = Yagi(freq_mhz=freq_mhz, n_directors=5, height_m=6.0)
        return ant.to_nec_model(ground_type=0)

    cfg    = SweepConfig(144.0, 148.0, 0.1)
    sweep  = FreqSweep(my_yagi, cfg)
    result = sweep.run()
"""

from __future__ import annotations

import logging
from concurrent.futures import ProcessPoolExecutor, as_completed
from dataclasses import dataclass, field
from pathlib import Path
from typing import Callable, List, Optional

import numpy as np

log = logging.getLogger(__name__)


# ─── Configuration ────────────────────────────────────────────────────────────

@dataclass
class SweepConfig:
    """Frequency sweep parameters."""
    freq_start_mhz:  float
    freq_stop_mhz:   float
    freq_step_mhz:   float = 0.1
    pattern_inc_deg: float = 5.0   # Angular increment for RP card (degrees)
    z0:              float = 50.0  # Reference impedance for SWR
    n_parallel:      int   = 1     # Parallel NEC processes (>1 requires fork-safe)
    keep_nec_files:  bool  = False # Keep .nec/.out files after run
    engine_name:     str   = "auto" # "nec2c", "nec4", "auto"

    @property
    def freq_list(self) -> List[float]:
        freqs = []
        f = self.freq_start_mhz
        while f <= self.freq_stop_mhz + 1e-9:
            freqs.append(round(f, 6))
            f += self.freq_step_mhz
        return freqs

    @property
    def n_steps(self) -> int:
        return len(self.freq_list)


# ─── Result Container ─────────────────────────────────────────────────────────

class SweepResult:
    """
    Container for a completed frequency sweep.

    Attributes
    ----------
    freq_points : list of NECFreqPoint
    config : SweepConfig
    """

    def __init__(self, freq_points: list, config: SweepConfig):
        self.freq_points = freq_points
        self.config      = config

    # ── DataFrame Export ─────────────────────────────────────────────────────

    def to_dataframe(self):
        """Return pandas DataFrame with one row per frequency."""
        import pandas as pd
        rows = []
        for pt in self.freq_points:
            rows.append({
                "freq_mhz":    pt.freq_mhz,
                "r_ohm":       pt.r_ohm,
                "x_ohm":       pt.x_ohm,
                "z_mag_ohm":   pt.z_mag,
                "z_phase_deg": pt.z_phase_deg,
                "swr_50":      pt.swr_50,
                "swr_75":      pt.swr_75,
                "gain_dBi":    pt.gain_dbi_max,
                "gain_theta":  pt.gain_theta_deg,
                "gain_phi":    pt.gain_phi_deg,
                "fb_dB":       pt.fb_ratio_db,
                "eff_pct":     pt.efficiency_pct,
                "bw_3db_E":    pt.bw_3db_e_deg,
                "bw_3db_H":    pt.bw_3db_h_deg,
                "rl_dB":       pt.return_loss_db,
                "swr_ok":      (pt.swr_50 < 2.0) if np.isfinite(pt.swr_50) else False,
                "gain_ok":     (pt.gain_dbi_max > 0.0) if np.isfinite(pt.gain_dbi_max) else False,
            })
        return pd.DataFrame(rows)

    # ── CSV Export ────────────────────────────────────────────────────────────

    def to_csv(self, path: str | Path):
        """Write sweep results to CSV."""
        self.to_dataframe().to_csv(path, index=False, float_format="%.4f")
        log.info("Sweep CSV written: %s", path)

    # ── Frequency Finders ─────────────────────────────────────────────────────

    def best_swr_freq(self) -> Optional[float]:
        """Return frequency (MHz) with lowest SWR."""
        valid = [pt for pt in self.freq_points if np.isfinite(pt.swr_50)]
        if not valid:
            return None
        return min(valid, key=lambda p: p.swr_50).freq_mhz

    def best_gain_freq(self) -> Optional[float]:
        """Return frequency (MHz) with highest gain."""
        valid = [pt for pt in self.freq_points if np.isfinite(pt.gain_dbi_max)]
        if not valid:
            return None
        return max(valid, key=lambda p: p.gain_dbi_max).freq_mhz

    def swr_bandwidth_mhz(self, threshold: float = 2.0) -> float:
        """Return bandwidth (MHz) where SWR <= threshold."""
        valid = [pt for pt in self.freq_points
                 if np.isfinite(pt.swr_50) and pt.swr_50 <= threshold]
        if not valid:
            return 0.0
        freqs = [pt.freq_mhz for pt in valid]
        return round(max(freqs) - min(freqs), 4)

    def summary(self) -> str:
        """Return formatted summary string."""
        df = self.to_dataframe()
        best_swr = self.best_swr_freq()
        best_gain = self.best_gain_freq()
        bw = self.swr_bandwidth_mhz()

        lines = [
            f"=== Sweep {self.config.freq_start_mhz}–{self.config.freq_stop_mhz} MHz ===",
            f"  Points:           {len(self.freq_points)}",
            f"  Best SWR freq:    {best_swr} MHz" if best_swr else "  Best SWR: N/A",
            f"  Min SWR:          {df.swr_50.min():.2f}" if len(df) else "",
            f"  Max gain:         {df.gain_dBi.max():.1f} dBi" if len(df) else "",
            f"  Best gain freq:   {best_gain} MHz" if best_gain else "",
            f"  2:1 SWR BW:       {bw:.3f} MHz",
        ]
        return "\n".join(l for l in lines if l)

    # ── Plotting ──────────────────────────────────────────────────────────────

    def plot_swr(self, show: bool = True, save_path: str | Path = None):
        """Plot SWR vs frequency."""
        import matplotlib.pyplot as plt
        df = self.to_dataframe()
        fig, ax = plt.subplots(figsize=(10, 5))
        ax.plot(df.freq_mhz, df.swr_50, 'b-', linewidth=1.5, label="SWR (50Ω)")
        ax.axhline(y=2.0, color='orange', linestyle='--', linewidth=1, label="2:1 SWR")
        ax.axhline(y=1.5, color='green',  linestyle=':', linewidth=1, label="1.5:1 SWR")
        ax.set_xlabel("Frequency (MHz)")
        ax.set_ylabel("SWR")
        ax.set_title(f"SWR vs Frequency  {self.config.freq_start_mhz}–{self.config.freq_stop_mhz} MHz")
        ax.set_ylim(1.0, min(10.0, df.swr_50.max() * 1.1))
        ax.legend()
        ax.grid(True, alpha=0.3)
        if save_path:
            fig.savefig(save_path, dpi=150, bbox_inches='tight')
        if show:
            plt.show()
        return fig

    def plot_gain(self, show: bool = True, save_path: str | Path = None):
        """Plot gain vs frequency."""
        import matplotlib.pyplot as plt
        df = self.to_dataframe()
        fig, ax = plt.subplots(figsize=(10, 5))
        ax.plot(df.freq_mhz, df.gain_dBi, 'r-', linewidth=1.5, label="Max Gain (dBi)")
        ax.set_xlabel("Frequency (MHz)")
        ax.set_ylabel("Gain (dBi)")
        ax.set_title(f"Gain vs Frequency  {self.config.freq_start_mhz}–{self.config.freq_stop_mhz} MHz")
        ax.legend()
        ax.grid(True, alpha=0.3)
        if save_path:
            fig.savefig(save_path, dpi=150, bbox_inches='tight')
        if show:
            plt.show()
        return fig


# ─── Sweep Engine ─────────────────────────────────────────────────────────────

class FreqSweep:
    """
    Automated frequency sweep.

    Parameters
    ----------
    model_builder : Callable[[float], NECModel]
        Function that accepts freq_mhz and returns a configured NECModel.
    config : SweepConfig
    """

    def __init__(self, model_builder: Callable, config: SweepConfig):
        self.builder = model_builder
        self.config  = config

    def run(self) -> SweepResult:
        """Run sweep sequentially."""
        from nec_runner import NECRunner, NECEngine
        from nec_parser import NECOutputParser

        engine_map = {
            "nec2c": NECEngine.NEC2, "nec2": NECEngine.NEC2,
            "nec4": NECEngine.NEC4, "auto": NECEngine.AUTO
        }
        engine = engine_map.get(self.config.engine_name.lower(), NECEngine.AUTO)
        runner = NECRunner(engine=engine, keep_files=self.config.keep_nec_files)
        parser = NECOutputParser()

        points = []
        freqs  = self.config.freq_list
        log.info("Starting sweep: %d steps, %.3f–%.3f MHz",
                 len(freqs), freqs[0], freqs[-1])

        for i, f in enumerate(freqs):
            model = self.builder(f)
            # Ensure pattern inc matches config
            if model.patterns:
                model.patterns[0].theta_inc = self.config.pattern_inc_deg
                model.patterns[0].phi_inc   = self.config.pattern_inc_deg

            result = runner.run(model, output_stem=f"sweep_{f:.4f}MHz")
            if result.success:
                pts = parser.parse(result.output_file)
                if pts:
                    points.extend(pts)
                else:
                    log.warning("No parsed data at %.4f MHz", f)
            else:
                log.warning("NEC failed at %.4f MHz: rc=%d", f, result.returncode)

            if (i + 1) % 10 == 0:
                log.info("  Progress: %d/%d (%.0f%%)", i+1, len(freqs),
                         100*(i+1)/len(freqs))

        points.sort(key=lambda p: p.freq_mhz)
        log.info("Sweep complete: %d valid points", len(points))
        return SweepResult(points, self.config)

    def run_parallel(self) -> SweepResult:
        """Run sweep with parallel NEC processes."""
        from nec_runner import NECRunner, NECEngine
        from nec_parser import NECOutputParser

        engine_map = {
            "nec2c": NECEngine.NEC2, "auto": NECEngine.AUTO,
            "nec4": NECEngine.NEC4
        }
        engine = engine_map.get(self.config.engine_name.lower(), NECEngine.AUTO)
        parser = NECOutputParser()
        freqs  = self.config.freq_list

        def _run_one(f: float):
            runner = NECRunner(engine=engine, keep_files=self.config.keep_nec_files)
            model  = self.builder(f)
            result = runner.run(model, output_stem=f"sweep_{f:.4f}MHz")
            if result.success:
                return parser.parse(result.output_file)
            return []

        n_workers = min(self.config.n_parallel, len(freqs))
        points = []
        with ProcessPoolExecutor(max_workers=n_workers) as pool:
            futures = {pool.submit(_run_one, f): f for f in freqs}
            for fut in as_completed(futures):
                pts = fut.result()
                if pts:
                    points.extend(pts)

        points.sort(key=lambda p: p.freq_mhz)
        return SweepResult(points, self.config)


# ─── Convenience Functions ────────────────────────────────────────────────────

def sweep_dipole(freq_low: float, freq_high: float, step: float = 0.05,
                 height_m: float = 10.0, ground_type: int = 2) -> SweepResult:
    """
    Quick sweep of a half-wave dipole across a frequency range.

    Parameters
    ----------
    freq_low, freq_high : float  (MHz)
    step : float  Frequency step in MHz
    height_m : float  Height above ground in metres
    ground_type : int  0=free space, 1=perfect, 2=Sommerfeld real ground

    Returns
    -------
    SweepResult
    """
    from nec_generator import Dipole

    def builder(f):
        ant = Dipole(freq_mhz=f, height_m=height_m)
        m = ant.to_nec_model(ground_type=ground_type)
        m.add_pattern(theta_inc=5, phi_inc=5)
        return m

    cfg = SweepConfig(freq_low, freq_high, step)
    return FreqSweep(builder, cfg).run()


def sweep_yagi(freq_low: float, freq_high: float, step: float = 0.1,
               n_directors: int = 3, height_m: float = 5.0) -> SweepResult:
    """Quick sweep of a Yagi-Uda antenna."""
    from nec_generator import Yagi

    center = (freq_low + freq_high) / 2

    def builder(f):
        ant = Yagi(freq_mhz=f, n_directors=n_directors, height_m=height_m)
        m = ant.to_nec_model(ground_type=0)
        m.add_pattern()
        return m

    cfg = SweepConfig(freq_low, freq_high, step)
    return FreqSweep(builder, cfg).run()
