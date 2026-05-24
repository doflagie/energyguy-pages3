"""
batch_optimizer.py
TM-BATCH-001 Rev A — Batch Processing and Multiband Optimization

Runs NEC models across multiple antenna configurations and frequency bands.
Parallelizes jobs across available CPU cores. Supports resumable runs via
JSON checkpoint files. Finds best designs for multiband operation.

Usage:
    from batch_optimizer import BatchRunner, MultibandOptimizer
    from nec_generator import build_from_dict
    from freq_sweep import SweepConfig

    runner = BatchRunner(n_workers=4)
    runner.add_job({"type":"dipole","freq_mhz":14.25,"height_m":10.0},
                   SweepConfig(14.0, 14.35, 0.05))
    runner.add_job({"type":"yagi","freq_mhz":144.2,"n_directors":5},
                   SweepConfig(144.0, 146.0, 0.1))
    results = runner.run_all()
"""

from __future__ import annotations

import json
import logging
import time
import uuid
from concurrent.futures import ProcessPoolExecutor, as_completed
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List, Optional, Tuple

import numpy as np

log = logging.getLogger(__name__)


# ─── Job Container ────────────────────────────────────────────────────────────

@dataclass
class BatchJob:
    job_id:      str
    spec:        dict
    sweep_cfg:   object   # SweepConfig
    result:      object   = field(default=None, repr=False)
    status:      str      = "pending"   # pending, running, complete, failed
    error:       str      = ""
    duration_s:  float    = 0.0


# ─── Batch Runner ─────────────────────────────────────────────────────────────

class BatchRunner:
    """
    Manages and executes a queue of NEC sweep jobs.

    Parameters
    ----------
    n_workers : int
        Number of parallel NEC processes.
    output_dir : Path, optional
        Directory for CSV and checkpoint files.
    """

    def __init__(self, n_workers: int = 4,
                 output_dir: Optional[Path] = None):
        self.n_workers  = n_workers
        self.output_dir = Path(output_dir) if output_dir else Path("batch_output")
        self.output_dir.mkdir(parents=True, exist_ok=True)
        self._jobs: Dict[str, BatchJob] = {}

    def add_job(self, spec: dict, sweep_cfg) -> str:
        """Add a job. Returns job_id."""
        job_id = str(uuid.uuid4())[:8]
        self._jobs[job_id] = BatchJob(job_id=job_id, spec=spec, sweep_cfg=sweep_cfg)
        log.info("Job %s added: %s @ %.2f–%.2f MHz",
                 job_id, spec.get("type","?"),
                 sweep_cfg.freq_start_mhz, sweep_cfg.freq_stop_mhz)
        return job_id

    def run_all(self) -> Dict[str, object]:
        """
        Execute all pending jobs in parallel.
        Returns dict of {job_id: SweepResult}.
        """
        pending = [j for j in self._jobs.values() if j.status == "pending"]
        log.info("BatchRunner: %d jobs, %d workers", len(pending), self.n_workers)

        def _run_job(job: BatchJob):
            from freq_sweep import FreqSweep
            from nec_generator import build_from_dict

            t0 = time.monotonic()
            try:
                def builder(freq_mhz):
                    s = dict(job.spec)
                    s["freq_mhz"] = freq_mhz
                    m = build_from_dict(s)
                    m.add_pattern(theta_inc=5, phi_inc=5)
                    return m
                sweep  = FreqSweep(builder, job.sweep_cfg)
                result = sweep.run()
                return job.job_id, result, None, time.monotonic() - t0
            except Exception as e:
                return job.job_id, None, str(e), time.monotonic() - t0

        with ProcessPoolExecutor(max_workers=self.n_workers) as pool:
            futures = {pool.submit(_run_job, j): j for j in pending}
            for fut in as_completed(futures):
                job_id, result, error, dur = fut.result()
                job = self._jobs[job_id]
                job.duration_s = dur
                if result is not None:
                    job.result = result
                    job.status = "complete"
                    out_csv = self.output_dir / f"{job_id}.csv"
                    result.to_csv(out_csv)
                    log.info("Job %s complete (%.1fs) → %s", job_id, dur, out_csv)
                else:
                    job.status = "failed"
                    job.error  = error
                    log.warning("Job %s FAILED: %s", job_id, error)

        return {jid: j.result for jid, j in self._jobs.items()
                if j.result is not None}

    def run_multiband_sweep(self, spec: dict,
                            bands: List[Tuple[float, float, float, str]]):
        """
        Sweep one antenna spec across multiple bands.

        Parameters
        ----------
        spec : dict   build_from_dict spec (freq_mhz will be overridden per band)
        bands : list of (f_start, f_stop, f_step, label)

        Returns
        -------
        pandas DataFrame  with band label column
        """
        import pandas as pd
        from freq_sweep import FreqSweep, SweepConfig
        from nec_generator import build_from_dict

        all_dfs = []
        for f_start, f_stop, f_step, label in bands:
            def builder(freq_mhz, _spec=spec):
                s = dict(_spec)
                s["freq_mhz"] = freq_mhz
                m = build_from_dict(s)
                m.add_pattern()
                return m
            cfg    = SweepConfig(f_start, f_stop, f_step)
            result = FreqSweep(builder, cfg).run()
            df     = result.to_dataframe()
            df.insert(0, "band", label)
            all_dfs.append(df)

        if not all_dfs:
            return pd.DataFrame()
        combined = pd.concat(all_dfs, ignore_index=True)
        out_path = self.output_dir / f"multiband_{spec.get('type','ant')}.csv"
        combined.to_csv(out_path, index=False, float_format="%.4f")
        log.info("Multiband sweep → %s", out_path)
        return combined

    def compare_antennas(self, specs: List[dict], freq_mhz: float):
        """
        Run all specs at a single frequency, return comparison DataFrame.
        """
        import pandas as pd
        from nec_runner import NECRunner
        from nec_parser import NECOutputParser
        from nec_generator import build_from_dict

        runner = NECRunner()
        parser = NECOutputParser()
        rows   = []

        for spec in specs:
            s = dict(spec)
            s["freq_mhz"] = freq_mhz
            try:
                model  = build_from_dict(s)
                model.add_pattern()
                result = runner.run(model)
                if result.success:
                    pts = parser.parse(result.output_file)
                    if pts:
                        pt = pts[0]
                        rows.append({
                            "type":    spec.get("type", "?"),
                            "label":   spec.get("label", spec.get("type", "?")),
                            "freq_mhz": freq_mhz,
                            "swr_50":  pt.swr_50,
                            "gain_dBi": pt.gain_dbi_max,
                            "fb_dB":   pt.fb_ratio_db,
                            "r_ohm":   pt.r_ohm,
                            "x_ohm":   pt.x_ohm,
                        })
            except Exception as e:
                log.warning("Compare failed for %s: %s", spec.get("type"), e)

        df = pd.DataFrame(rows)
        if not df.empty:
            df.sort_values("gain_dBi", ascending=False, inplace=True)
        return df


# ─── Multiband Optimizer ──────────────────────────────────────────────────────

class MultibandOptimizer:
    """
    Finds antenna designs that perform across multiple bands simultaneously.
    Particularly useful for LPDA parameter sweeps.

    Parameters
    ----------
    bands : List[Tuple[float, float, float, str]]
        List of (f_start_MHz, f_stop_MHz, f_step_MHz, label)
    objectives : dict
        Keys: max_swr (float), min_gain_dBi (float), min_fb_dB (float)
    """

    def __init__(self, bands: List[Tuple], objectives: Dict):
        self.bands      = bands
        self.objectives = objectives

    def _meets_objectives(self, df_band) -> bool:
        """Check if all frequency points in a band meet objectives."""
        max_swr = self.objectives.get("max_swr", 3.0)
        min_gain = self.objectives.get("min_gain_dBi", 0.0)
        min_fb   = self.objectives.get("min_fb_dB", 0.0)

        if df_band.empty:
            return False
        if df_band["swr_50"].max() > max_swr:
            return False
        if df_band["gain_dBi"].min() < min_gain:
            return False
        if "fb_dB" in df_band.columns and df_band["fb_dB"].min() < min_fb:
            return False
        return True

    def optimize_lpda(self,
                      tau_range: Tuple[float, float] = (0.85, 0.97),
                      sigma_range: Tuple[float, float] = (0.05, 0.15),
                      tau_step: float = 0.02,
                      sigma_step: float = 0.02,
                      height_m: float = 12.0) -> Optional[Dict]:
        """
        Grid search LPDA tau/sigma parameter space.
        Returns best design as dict, or None if no solution found.
        """
        import pandas as pd
        from nec_generator import LogPeriodic
        from nec_runner import NECRunner
        from nec_parser import NECOutputParser
        from freq_sweep import FreqSweep, SweepConfig

        f_low  = min(b[0] for b in self.bands)
        f_high = max(b[1] for b in self.bands)

        best_result = None
        best_score  = float('-inf')

        tau_vals   = np.arange(tau_range[0],   tau_range[1]   + 1e-9, tau_step)
        sigma_vals = np.arange(sigma_range[0], sigma_range[1] + 1e-9, sigma_step)

        log.info("LPDA grid search: tau=%s sigma=%s → %d combinations",
                 tau_range, sigma_range, len(tau_vals) * len(sigma_vals))

        for tau in tau_vals:
            for sigma in sigma_vals:
                all_ok = True
                band_scores = []

                for f_start, f_stop, f_step, label in self.bands:
                    def builder(freq_mhz, _tau=tau, _sigma=sigma):
                        ant = LogPeriodic(f_low, f_high, tau=_tau, sigma=_sigma,
                                          height_m=height_m)
                        m = ant.to_nec_model(ground_type=0)
                        m.frequencies.clear()
                        m.add_frequency(freq_mhz)
                        m.add_pattern()
                        return m

                    from freq_sweep import FreqSweep, SweepConfig
                    cfg    = SweepConfig(f_start, f_stop, f_step)
                    result = FreqSweep(builder, cfg).run()
                    df     = result.to_dataframe()

                    if not self._meets_objectives(df):
                        all_ok = False
                        break
                    band_scores.append(df["gain_dBi"].mean())

                if all_ok:
                    score = np.mean(band_scores)
                    if score > best_score:
                        best_score  = score
                        best_result = {
                            "tau": tau, "sigma": sigma,
                            "mean_gain_dBi": score,
                            "f_low_mhz": f_low, "f_high_mhz": f_high,
                            "height_m": height_m,
                        }
                        log.info("New best: tau=%.3f sigma=%.3f gain=%.1f dBi",
                                 tau, sigma, score)

        return best_result

    def generate_report(self, results: Dict, output_path: str | Path):
        """Write text report of optimization results."""
        lines = ["=" * 60, "  MULTIBAND OPTIMIZATION REPORT", "=" * 60, ""]
        if not results:
            lines.append("No designs found meeting all objectives.")
        else:
            for k, v in results.items():
                lines.append(f"  {k}: {v}")
        lines += ["", "OBJECTIVES:"]
        for k, v in self.objectives.items():
            lines.append(f"  {k}: {v}")
        Path(output_path).write_text("\n".join(lines))
