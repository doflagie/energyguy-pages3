"""
csv_export.py
TM-CSV-001 Rev A — CSV Export and HTML Report Generation

Exports antenna modeling SweepResults to CSV and self-contained HTML reports
with embedded matplotlib charts. Also exports single-frequency summaries.

Usage:
    from csv_export import CSVExporter, HTMLReporter
    exporter = CSVExporter()
    exporter.export_sweep(result, "dipole_sweep.csv")
    HTMLReporter().generate_report(result, "dipole_report.html")
"""

from __future__ import annotations

import base64
import io
import logging
import math
from pathlib import Path
from typing import Dict, Optional

import numpy as np

log = logging.getLogger(__name__)


class CSVExporter:
    """Export SweepResult and comparison data to CSV files."""

    def export_sweep(self, sweep_result, output_path: str | Path,
                     include_summary_row: bool = True):
        """
        Write full sweep to CSV.

        Columns: freq_mhz, r_ohm, x_ohm, z_mag_ohm, phase_deg, swr_50, swr_75,
                 gain_dBi, gain_theta, gain_phi, fb_ratio_dB, efficiency_pct,
                 bw_3dB_E, bw_3dB_H, return_loss_dB, vswr_ok, gain_ok
        """
        import pandas as pd
        df = sweep_result.to_dataframe()

        if include_summary_row:
            summary = self._build_summary_row(df)
            df = pd.concat([df, summary], ignore_index=True)

        df.to_csv(output_path, index=False, float_format="%.4f", na_rep="NaN")
        log.info("Sweep CSV → %s (%d rows)", output_path, len(df))

    def export_comparison(self, results_dict: Dict[str, object],
                          output_path: str | Path):
        """
        Multi-antenna comparison CSV.
        results_dict: {antenna_name: SweepResult}
        Columns prefixed by antenna name.
        """
        import pandas as pd
        frames = []
        for name, result in results_dict.items():
            df = result.to_dataframe()
            df = df.rename(columns={c: f"{name}_{c}" for c in df.columns if c != "freq_mhz"})
            frames.append(df)

        if not frames:
            return
        combined = frames[0]
        for df in frames[1:]:
            combined = combined.merge(df, on="freq_mhz", how="outer")
        combined.sort_values("freq_mhz", inplace=True)
        combined.to_csv(output_path, index=False, float_format="%.4f", na_rep="NaN")
        log.info("Comparison CSV → %s", output_path)

    def export_pattern_3d(self, pattern: np.ndarray, freq_mhz: float,
                          output_path: str | Path):
        """
        Export 3D pattern array to CSV.
        Columns: theta_deg, phi_deg, gain_dBi, gain_linear, x_norm, y_norm, z_norm
        """
        import pandas as pd
        if pattern is None or len(pattern) == 0:
            log.warning("Empty pattern, skipping export")
            return

        rows = []
        max_gain = np.max(pattern[:, 2])
        for row in pattern:
            theta_r = math.radians(row[0])
            phi_r   = math.radians(row[1])
            g_dbi   = row[2]
            g_lin   = 10 ** ((g_dbi - max_gain) / 20)   # Normalized 0-1
            x = g_lin * math.sin(theta_r) * math.cos(phi_r)
            y = g_lin * math.sin(theta_r) * math.sin(phi_r)
            z = g_lin * math.cos(theta_r)
            rows.append({"theta_deg": row[0], "phi_deg": row[1],
                         "gain_dBi": g_dbi, "gain_linear": g_lin,
                         "x_norm": x, "y_norm": y, "z_norm": z})

        df = pd.DataFrame(rows)
        df.to_csv(output_path, index=False, float_format="%.6f")
        log.info("Pattern CSV → %s (%d points)", output_path, len(df))

    def generate_summary_txt(self, sweep_result, output_path: str | Path):
        """Human-readable text summary of sweep results."""
        df = sweep_result.to_dataframe()
        lines = [
            "=" * 60,
            "  ANTENNA PERFORMANCE SUMMARY",
            f"  Sweep: {sweep_result.config.freq_start_mhz:.3f} – {sweep_result.config.freq_stop_mhz:.3f} MHz",
            f"  Steps: {len(sweep_result.freq_points)}",
            "=" * 60,
            "",
            "IMPEDANCE / SWR:",
            f"  Min SWR (50Ω): {df.swr_50.min():.2f} @ {df.loc[df.swr_50.idxmin(),'freq_mhz']:.3f} MHz",
            f"  2:1 SWR BW:    {sweep_result.swr_bandwidth_mhz():.3f} MHz",
            "",
            "GAIN:",
            f"  Max Gain:      {df.gain_dBi.max():.1f} dBi @ {df.loc[df.gain_dBi.idxmax(),'freq_mhz']:.3f} MHz",
            f"  Min Gain:      {df.gain_dBi.min():.1f} dBi",
            "",
            "F/B RATIO:",
            f"  Max F/B:       {df.fb_dB.max():.1f} dB" if 'fb_dB' in df.columns else "  F/B: N/A",
            "",
            "BEAMWIDTH:",
            f"  E-plane 3dB:   {df.bw_3db_E.mean():.1f}°" if 'bw_3db_E' in df.columns else "",
            f"  H-plane 3dB:   {df.bw_3db_H.mean():.1f}°" if 'bw_3db_H' in df.columns else "",
            "",
        ]
        Path(output_path).write_text("\n".join(l for l in lines if l is not None))
        log.info("Summary TXT → %s", output_path)

    # ── Internal ──────────────────────────────────────────────────────────────

    def _build_summary_row(self, df):
        import pandas as pd
        numeric = df.select_dtypes(include=[float, int])
        summary = numeric.mean().to_frame().T
        summary.insert(0, "freq_mhz", "MEAN")
        return summary


class HTMLReporter:
    """Generate self-contained HTML report with embedded charts."""

    def generate_report(self, sweep_result, output_path: str | Path):
        """
        Create HTML file with SWR chart, gain chart, and summary table.
        All images are base64-embedded — no external dependencies.
        """
        df = sweep_result.to_dataframe()

        swr_img  = self._chart_to_b64(sweep_result.plot_swr(show=False))
        gain_img = self._chart_to_b64(sweep_result.plot_gain(show=False))

        html = self._build_html(df, swr_img, gain_img, sweep_result)
        Path(output_path).write_text(html)
        log.info("HTML report → %s", output_path)

    def _chart_to_b64(self, fig) -> str:
        buf = io.BytesIO()
        fig.savefig(buf, format="png", dpi=120, bbox_inches="tight")
        buf.seek(0)
        import matplotlib.pyplot as plt
        plt.close(fig)
        return base64.b64encode(buf.read()).decode("ascii")

    def _build_html(self, df, swr_b64, gain_b64, sweep_result) -> str:
        table_rows = ""
        for _, row in df.head(50).iterrows():
            ok = "✓" if row.get("swr_ok", False) else "✗"
            table_rows += (
                f"<tr><td>{row.freq_mhz:.3f}</td>"
                f"<td>{row.r_ohm:.1f}</td><td>{row.x_ohm:.1f}</td>"
                f"<td>{row.swr_50:.2f}</td>"
                f"<td>{row.gain_dBi:.1f}</td>"
                f"<td>{ok}</td></tr>\n"
            )

        return f"""<!DOCTYPE html>
<html lang="en">
<head><meta charset="UTF-8">
<title>Antenna Sweep Report</title>
<style>
  body {{ font-family: monospace; background:#1a1a1a; color:#d4d4d4; margin:20px; }}
  h1,h2 {{ color:#7ec8e3; }}
  table {{ border-collapse:collapse; width:100%; }}
  th {{ background:#333; color:#7ec8e3; padding:6px; }}
  td {{ padding:4px 8px; border-bottom:1px solid #333; }}
  tr:hover {{ background:#2a2a2a; }}
  img {{ max-width:100%; margin:10px 0; }}
  .summary {{ background:#222; padding:15px; margin:10px 0; border-left:3px solid #7ec8e3; }}
</style>
</head>
<body>
<h1>Antenna Performance Report</h1>
<div class="summary">
  Sweep: {sweep_result.config.freq_start_mhz:.3f} – {sweep_result.config.freq_stop_mhz:.3f} MHz &nbsp;|&nbsp;
  {sweep_result.config.n_steps} steps &nbsp;|&nbsp;
  Min SWR: {df.swr_50.min():.2f} &nbsp;|&nbsp;
  Max Gain: {df.gain_dBi.max():.1f} dBi
</div>
<h2>SWR vs Frequency</h2>
<img src="data:image/png;base64,{swr_b64}" alt="SWR chart">
<h2>Gain vs Frequency</h2>
<img src="data:image/png;base64,{gain_b64}" alt="Gain chart">
<h2>Data Table</h2>
<table>
<tr><th>Freq MHz</th><th>R Ω</th><th>X Ω</th><th>SWR</th><th>Gain dBi</th><th>OK</th></tr>
{table_rows}
</table>
</body></html>"""
