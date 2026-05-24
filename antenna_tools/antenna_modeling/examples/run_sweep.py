"""
run_sweep.py — Example: Frequency sweeps for 20m dipole and 2m Yagi.

Demonstrates:
  - freq_sweep.py API (sweep_dipole, sweep_yagi convenience functions)
  - CSV and HTML export
  - 3D pattern export at peak gain frequency
  - Console summary output

Run: python run_sweep.py
Requires: nec2c installed and in PATH
"""

import logging
import sys
from pathlib import Path

logging.basicConfig(level=logging.INFO,
                    format="%(asctime)s %(levelname)s: %(message)s")

# Add scripts dir to path
sys.path.insert(0, str(Path(__file__).parent.parent / "scripts"))

from freq_sweep import sweep_dipole, sweep_yagi, FreqSweep, SweepConfig
from csv_export import CSVExporter, HTMLReporter
from pattern_3d import Pattern3D


def run_dipole_20m():
    print("\n=== 20m Half-Wave Dipole Sweep ===")
    result = sweep_dipole(
        freq_low=14.000,
        freq_high=14.350,
        step=0.025,
        height_m=10.0,
        ground_type=2   # Sommerfeld real ground
    )
    print(result.summary())

    # Export
    exporter = CSVExporter()
    exporter.export_sweep(result, "dipole_20m_sweep.csv")
    exporter.generate_summary_txt(result, "dipole_20m_summary.txt")

    # HTML report (requires matplotlib)
    try:
        HTMLReporter().generate_report(result, "dipole_20m_report.html")
        print("HTML report: dipole_20m_report.html")
    except Exception as e:
        print(f"HTML report skipped: {e}")

    # 3D pattern at best gain frequency
    best_gain_freq = result.best_gain_freq()
    if best_gain_freq:
        best_pt = min(result.freq_points,
                      key=lambda p: abs(p.freq_mhz - best_gain_freq))
        if best_pt.pattern is not None:
            p3d = Pattern3D.from_freq_point(best_pt)
            p3d.to_json(f"dipole_20m_pattern_{best_gain_freq:.3f}MHz.json",
                        freq_mhz=best_gain_freq)
            p3d.to_csv(f"dipole_20m_pattern_{best_gain_freq:.3f}MHz.csv")
            print(f"3D pattern at {best_gain_freq:.3f} MHz exported")

    return result


def run_yagi_2m():
    print("\n=== 2m 5-Element Yagi Sweep ===")

    from nec_generator import Yagi

    def yagi_builder(freq_mhz):
        ant = Yagi(freq_mhz=freq_mhz, n_directors=3, height_m=6.0,
                   wire_radius_m=0.004)
        m = ant.to_nec_model(ground_type=0)
        m.add_pattern(theta_inc=5, phi_inc=5)
        return m

    cfg    = SweepConfig(freq_start_mhz=144.0, freq_stop_mhz=148.0,
                         freq_step_mhz=0.2)
    result = FreqSweep(yagi_builder, cfg).run()

    print(result.summary())

    exporter = CSVExporter()
    exporter.export_sweep(result, "yagi_2m_sweep.csv")

    # 3D pattern at best SWR
    best_swr_freq = result.best_swr_freq()
    if best_swr_freq:
        best_pt = min(result.freq_points,
                      key=lambda p: abs(p.freq_mhz - best_swr_freq))
        if best_pt.pattern is not None:
            p3d = Pattern3D.from_freq_point(best_pt)
            p3d.to_json(f"yagi_2m_pattern_{best_swr_freq:.1f}MHz.json",
                        freq_mhz=best_swr_freq)
            try:
                p3d.to_vtk(f"yagi_2m_pattern_{best_swr_freq:.1f}MHz.vtk")
                print(f"VTK pattern at {best_swr_freq:.1f} MHz → ParaView ready")
            except Exception as e:
                print(f"VTK export: {e}")

    return result


if __name__ == "__main__":
    print("TM-SWEEP-001 — Frequency Sweep Examples")
    print("Requires: nec2c in PATH\n")

    dipole_result = run_dipole_20m()
    yagi_result   = run_yagi_2m()

    print("\n=== Comparison Summary ===")
    print(f"Dipole 20m: max gain={dipole_result.to_dataframe().gain_dBi.max():.1f} dBi, "
          f"min SWR={dipole_result.to_dataframe().swr_50.min():.2f}")
    print(f"Yagi 2m:    max gain={yagi_result.to_dataframe().gain_dBi.max():.1f} dBi, "
          f"min SWR={yagi_result.to_dataframe().swr_50.min():.2f}")
