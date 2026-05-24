"""
optimize_yagi.py — Example: Optimize a 2m Yagi with GA and PSO.

Optimizes 3 director half-lengths and spacings for:
  - SWR < 1.5 at 144.2 MHz
  - Gain > 10 dBi
  - F/B ratio > 20 dB

Runs both GA (50 gen, 30 pop) and PSO (100 iter, 20 particles).
Compares results and writes winner to yagi_optimized.nec.

Run: python optimize_yagi.py
"""

import logging
import sys
from pathlib import Path

logging.basicConfig(level=logging.INFO,
                    format="%(asctime)s %(levelname)s: %(message)s")

sys.path.insert(0, str(Path(__file__).parent.parent / "scripts"))

from nec_generator import Yagi, NECModel
from nec_runner import NECRunner
from genetic_optimizer import GeneParameter, FitnessFunction, GeneticOptimizer
from pso_optimizer import PSOOptimizer, PSOConfig


# ─── Parameters ───────────────────────────────────────────────────────────────
# Optimize 3 directors: half-lengths (m) and their boom spacings (m)
# Also optimize driven element half-length and reflector half-length

TARGET_FREQ_MHZ = 144.2
C_LIGHT         = 299.792458e6
WL              = C_LIGHT / (TARGET_FREQ_MHZ * 1e6)   # ~2.08m

params = [
    GeneParameter("refl_hl",  0.46 * WL/2, 0.52 * WL/2),   # Reflector half-length
    GeneParameter("driv_hl",  0.44 * WL/2, 0.50 * WL/2),   # Driven half-length
    GeneParameter("d1_hl",    0.42 * WL/2, 0.47 * WL/2),   # Director 1 half-length
    GeneParameter("d2_hl",    0.40 * WL/2, 0.46 * WL/2),
    GeneParameter("d3_hl",    0.38 * WL/2, 0.44 * WL/2),
    GeneParameter("refl_sp",  0.15 * WL,   0.25 * WL),      # Reflector spacing (behind driven)
    GeneParameter("d1_sp",    0.15 * WL,   0.30 * WL),      # Director 1 spacing
    GeneParameter("d2_sp",    0.20 * WL,   0.35 * WL),
    GeneParameter("d3_sp",    0.20 * WL,   0.40 * WL),
]

objectives = {
    "swr_target":    1.5,
    "swr_weight":    3.0,
    "gain_min_dBi":  10.0,
    "gain_weight":   2.0,
    "fb_min_dB":     20.0,
    "fb_weight":     1.5,
}


# ─── Model Builder ────────────────────────────────────────────────────────────

def build_custom_yagi(genes: dict, freq_mhz: float = TARGET_FREQ_MHZ) -> NECModel:
    """Build a Yagi from optimized gene dict."""
    import math
    from nec_generator import Wire, Excitation, FrequencyCard, RadiationPattern, GroundPlane

    m = NECModel(title=f"Optimized Yagi {freq_mhz} MHz", freq_mhz=freq_mhz)
    h = 6.0   # Height above ground (m)
    r = 0.004  # Wire radius (m)

    # Element X positions (boom axis)
    x_refl = -genes["refl_sp"]
    x_driv = 0.0
    x_d1   = genes["d1_sp"]
    x_d2   = genes["d1_sp"] + genes["d2_sp"]
    x_d3   = genes["d1_sp"] + genes["d2_sp"] + genes["d3_sp"]

    elements = [
        (x_refl, genes["refl_hl"], False, "Reflector"),
        (x_driv, genes["driv_hl"], True,  "Driven"),
        (x_d1,   genes["d1_hl"],   False, "Director_1"),
        (x_d2,   genes["d2_hl"],   False, "Director_2"),
        (x_d3,   genes["d3_hl"],   False, "Director_3"),
    ]

    driven_tag = None
    for tag_base, (x, hl, is_driven, name) in enumerate(elements, start=1):
        segs = max(9, int(2 * hl / WL * 10))
        if segs % 2 == 0:
            segs += 1
        sh = segs // 2 + 1
        t1 = tag_base * 2 - 1
        t2 = tag_base * 2
        m.add_wire(x, -hl, h, x, 0, h, radius=r, segs=sh, tag=t1)
        m.add_wire(x, 0,   h, x, hl, h, radius=r, segs=sh, tag=t2)
        if is_driven:
            driven_tag = t1

    if driven_tag:
        wire = next(w for w in m.wires if w.tag == driven_tag)
        m.excitations.append(Excitation(tag=driven_tag, segment=wire.segs))

    m.add_frequency(freq_mhz)
    m.set_ground(type=0)   # Free space for optimization speed
    m.add_pattern(theta_inc=10, phi_inc=10)   # Coarser pattern for speed during opt
    return m


# ─── Run Optimization ─────────────────────────────────────────────────────────

def run_ga(runner):
    print("\n--- Genetic Algorithm (50 gen, 30 pop) ---")
    fitness_fn = FitnessFunction(objectives)
    opt = GeneticOptimizer(
        params=params,
        fitness_fn=fitness_fn,
        model_builder=build_custom_yagi,
        runner=runner,
        pop_size=30,
        n_generations=50,
        mutation_rate=0.12,
        crossover_rate=0.8,
        elitism=2,
        early_stop_gens=15,
    )
    result = opt.run()
    print(f"GA best fitness: {result.best_fitness:.3f}")
    print(f"GA best genes:   {result.best_genes}")
    result.to_csv("ga_convergence.csv")
    return result


def run_pso(runner):
    print("\n--- Particle Swarm (100 iter, 20 particles) ---")
    fitness_fn = FitnessFunction(objectives)
    cfg = PSOConfig(n_particles=20, n_iterations=100, early_stop_iters=20)
    opt = PSOOptimizer(
        params=params,
        fitness_fn=fitness_fn,
        model_builder=build_custom_yagi,
        runner=runner,
        config=cfg,
    )
    result = opt.run()
    print(f"PSO best fitness: {result.best_fitness:.3f}")
    print(f"PSO best genes:   {result.best_genes}")
    result.to_csv("pso_convergence.csv")
    return result


# ─── Main ─────────────────────────────────────────────────────────────────────

if __name__ == "__main__":
    print(f"Yagi Optimizer — {TARGET_FREQ_MHZ} MHz, λ={WL*1000:.0f}mm")
    print(f"Objectives: SWR<{objectives['swr_target']}, "
          f"Gain>{objectives['gain_min_dBi']} dBi, "
          f"F/B>{objectives['fb_min_dB']} dB\n")

    runner = NECRunner()

    ga_result  = run_ga(runner)
    pso_result = run_pso(runner)

    # Pick winner
    if ga_result.best_fitness >= pso_result.best_fitness:
        winner, winner_name = ga_result, "GA"
    else:
        winner, winner_name = pso_result, "PSO"

    print(f"\n=== WINNER: {winner_name} (fitness={winner.best_fitness:.3f}) ===")
    print("Best genes:")
    for k, v in winner.best_genes.items():
        print(f"  {k:12s}: {v*1000:.1f} mm")

    # Write final optimized NEC file
    final_model = build_custom_yagi(winner.best_genes, TARGET_FREQ_MHZ)
    # Switch to fine-grained pattern for final output
    final_model.patterns.clear()
    final_model.add_pattern(theta_inc=5, phi_inc=5)
    final_model.write("yagi_optimized.nec")
    print("\nOptimized design written: yagi_optimized.nec")
    print("Verify with: nec2c yagi_optimized.nec yagi_optimized.out")
