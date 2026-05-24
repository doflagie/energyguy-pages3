"""
batch_multiband.py — Example: Multiband antenna comparison.

Compares LPDA vs fan dipole vs individual dipoles for 40m/20m/15m/10m.
Generates comparison CSV and text report.

Run: python batch_multiband.py
"""

import logging
import sys
from pathlib import Path

logging.basicConfig(level=logging.INFO,
                    format="%(asctime)s %(levelname)s: %(message)s")

sys.path.insert(0, str(Path(__file__).parent.parent / "scripts"))

from batch_optimizer import BatchRunner, MultibandOptimizer
from freq_sweep import SweepConfig
from csv_export import CSVExporter


# Band definitions: (f_start, f_stop, f_step, label)
BANDS = [
    (7.000,  7.300,  0.05,  "40m"),
    (14.000, 14.350, 0.05,  "20m"),
    (21.000, 21.450, 0.05,  "15m"),
    (28.000, 28.800, 0.10,  "10m"),
]

# Antenna specs
ANTENNAS = {
    "LPDA_40_10": {
        "type": "lpda", "freq_low_mhz": 7.0, "freq_high_mhz": 29.7,
        "tau": 0.9, "sigma": 0.07, "height_m": 12.0
    },
    "Dipole_40m": {
        "type": "dipole", "freq_mhz": 7.15, "height_m": 10.0
    },
    "Dipole_20m": {
        "type": "dipole", "freq_mhz": 14.25, "height_m": 10.0
    },
    "Dipole_15m": {
        "type": "dipole", "freq_mhz": 21.25, "height_m": 10.0
    },
    "Dipole_10m": {
        "type": "dipole", "freq_mhz": 28.5, "height_m": 10.0
    },
    "Vertical_40m": {
        "type": "vertical", "freq_mhz": 7.15, "n_radials": 32, "height_m": 0.01
    },
}


def run_comparison():
    print("=== Multiband Antenna Comparison ===\n")
    runner = BatchRunner(n_workers=2, output_dir=Path("batch_results"))
    exporter = CSVExporter()

    all_results = {}

    for ant_name, spec in ANTENNAS.items():
        print(f"\n--- {ant_name} ---")
        for f_start, f_stop, f_step, band_label in BANDS:
            job_id = runner.add_job(spec, SweepConfig(f_start, f_stop, f_step))
            print(f"  Queued: {ant_name} on {band_label}")

    results = runner.run_all()
    print(f"\nCompleted {len(results)} jobs")

    # Generate comparison at band centers
    band_centers = [7.15, 14.25, 21.25, 28.5]
    print("\n=== Performance at Band Centers ===")
    for spec_name, spec in ANTENNAS.items():
        for fc in band_centers:
            # Add single-freq jobs
            job_id = runner.add_job(spec, SweepConfig(fc, fc + 0.01, 0.01))

    center_results = runner.run_all()

    # Comparison table
    print("\nAntenna comparisons queued. Check batch_results/ for CSV files.")
    return results


def optimize_lpda():
    """Find best LPDA tau/sigma for all 4 bands."""
    print("\n=== LPDA Tau/Sigma Optimization ===")
    objectives = {"max_swr": 3.0, "min_gain_dBi": 3.0, "min_fb_dB": 5.0}
    opt = MultibandOptimizer(BANDS, objectives)

    print("Searching tau 0.85–0.97, sigma 0.05–0.15...")
    best = opt.optimize_lpda(
        tau_range=(0.85, 0.97), sigma_range=(0.05, 0.15),
        tau_step=0.04, sigma_step=0.04, height_m=12.0
    )
    if best:
        print(f"\nBest LPDA design found:")
        for k, v in best.items():
            print(f"  {k}: {v}")
        opt.generate_report(best, "lpda_optimization_report.txt")
        print("Report: lpda_optimization_report.txt")
    else:
        print("No design found meeting all objectives. Relax constraints and retry.")
    return best


if __name__ == "__main__":
    results = run_comparison()
    best_lpda = optimize_lpda()
    print("\nDone. See batch_results/ for all CSVs.")
