"""
genetic_optimizer.py
TM-GA-001 Rev A — Genetic Algorithm Antenna Optimizer

Real-valued genetic algorithm for antenna geometry optimization.
Fitness function evaluates NEC model performance across multiple objectives.
Supports elitism, tournament selection, uniform crossover, Gaussian mutation.
Parallel population evaluation via ProcessPoolExecutor.
Checkpointing every N generations for resumable runs.

Usage:
    from genetic_optimizer import GeneticOptimizer, GeneParameter, FitnessFunction
    from nec_runner import NECRunner
    from nec_generator import Yagi

    params = [
        GeneParameter("dir1_hl", 0.40, 0.50),
        GeneParameter("dir2_hl", 0.38, 0.48),
        GeneParameter("dir1_sp", 0.15, 0.35),
    ]
    objectives = {"swr_target": 1.5, "gain_min_dBi": 10.0, "fb_min_dB": 20.0}
    fitness_fn = FitnessFunction(objectives)

    def builder(genes):
        ant = Yagi(freq_mhz=144.2, n_directors=2)
        # ... override element dimensions from genes dict ...
        return ant.to_nec_model()

    opt = GeneticOptimizer(params, fitness_fn, builder, NECRunner())
    result = opt.run()
    print("Best SWR:", result.best_individual.nec_result.swr_50)
"""

from __future__ import annotations

import json
import logging
import math
import random
import time
from concurrent.futures import ProcessPoolExecutor, as_completed
from dataclasses import dataclass, field
from pathlib import Path
from typing import Callable, Dict, List, Optional

import numpy as np

log = logging.getLogger(__name__)


# ─── Parameter Definition ─────────────────────────────────────────────────────

@dataclass
class GeneParameter:
    """Single optimizable parameter."""
    name:       str
    min_val:    float
    max_val:    float
    is_integer: bool = False

    def clamp(self, val: float) -> float:
        v = float(np.clip(val, self.min_val, self.max_val))
        return round(v) if self.is_integer else v

    def random(self) -> float:
        v = random.uniform(self.min_val, self.max_val)
        return self.clamp(v)

    @property
    def range(self) -> float:
        return self.max_val - self.min_val


# ─── Individual ───────────────────────────────────────────────────────────────

@dataclass
class Individual:
    genes:      np.ndarray
    fitness:    float = float('-inf')
    nec_result: Optional[object] = field(default=None, repr=False)

    def copy(self) -> "Individual":
        return Individual(genes=self.genes.copy(), fitness=self.fitness)


# ─── Fitness Function ─────────────────────────────────────────────────────────

class FitnessFunction:
    """
    Multi-objective fitness combining SWR, gain, F/B, and bandwidth.
    Returns scalar score; higher = better.

    Parameters (objectives dict keys)
    ----------------------------------
    swr_target    : float  Target SWR (penalty grows as SWR/target)
    swr_weight    : float  Weight for SWR term (default 2.0)
    gain_min_dBi  : float  Minimum acceptable gain; below this → heavy penalty
    gain_weight   : float  Weight for gain term (default 1.0)
    fb_min_dB     : float  Minimum F/B ratio; below this → penalty
    fb_weight     : float  Weight for F/B term (default 0.5)
    bw_min_mhz    : float  Minimum 2:1 SWR bandwidth; below → penalty
    bw_weight     : float  Weight for bandwidth term (default 0.3)
    eff_min_pct   : float  Minimum efficiency % (default 50.0)
    eff_weight    : float  (default 0.2)
    """

    def __init__(self, objectives: Dict):
        self.obj = objectives

    def evaluate(self, freq_point) -> float:
        """
        Evaluate fitness of a NECFreqPoint.
        Returns float fitness score (higher = better).
        """
        score = 0.0

        # ── SWR term ──
        swr = freq_point.swr_50
        if not math.isfinite(swr):
            return self._penalize("non-finite SWR")
        target = self.obj.get("swr_target", 1.5)
        w_swr  = self.obj.get("swr_weight", 2.0)
        if swr > 10:
            return self._penalize(f"SWR={swr:.1f} too high")
        swr_score = max(0.0, (target / swr) ** 2)
        score += w_swr * swr_score

        # ── Gain term ──
        gain   = freq_point.gain_dbi_max
        g_min  = self.obj.get("gain_min_dBi", 0.0)
        w_gain = self.obj.get("gain_weight", 1.0)
        if math.isfinite(gain):
            gain_score = gain / max(g_min, 0.1)
            if gain < g_min:
                gain_score *= 0.3   # Penalty for below minimum
            score += w_gain * gain_score

        # ── F/B term ──
        fb     = freq_point.fb_ratio_db
        fb_min = self.obj.get("fb_min_dB", 10.0)
        w_fb   = self.obj.get("fb_weight", 0.5)
        if math.isfinite(fb):
            fb_score = min(fb / max(fb_min, 1.0), 2.0)   # Cap at 2x target
            if fb < fb_min:
                fb_score *= 0.5
            score += w_fb * fb_score

        # ── Efficiency term ──
        eff     = freq_point.efficiency_pct
        eff_min = self.obj.get("eff_min_pct", 50.0)
        w_eff   = self.obj.get("eff_weight", 0.2)
        if math.isfinite(eff):
            eff_score = eff / 100.0
            if eff < eff_min:
                eff_score *= 0.4
            score += w_eff * eff_score

        return score

    def evaluate_sweep(self, freq_points: list) -> float:
        """Average fitness across multiple frequency points (for bandwidth sweep)."""
        if not freq_points:
            return self._penalize("no freq points")
        scores = [self.evaluate(pt) for pt in freq_points]
        return float(np.mean(scores))

    def _penalize(self, reason: str = "") -> float:
        log.debug("Fitness penalty: %s", reason)
        return -1000.0


# ─── GA Result ────────────────────────────────────────────────────────────────

class GAResult:
    """Result container for a completed GA run."""

    def __init__(self, best: Individual, params: List[GeneParameter],
                 convergence: List[float], population: List[Individual],
                 duration_s: float):
        self.best_individual  = best
        self._params          = params
        self.convergence      = convergence
        self.population_final = population
        self.duration_s       = duration_s

    @property
    def best_genes(self) -> Dict[str, float]:
        return {p.name: float(self.best_individual.genes[i])
                for i, p in enumerate(self._params)}

    @property
    def best_fitness(self) -> float:
        return self.best_individual.fitness

    def to_csv(self, path: str | Path):
        import pandas as pd
        rows = [{"generation": i, "best_fitness": f}
                for i, f in enumerate(self.convergence)]
        pd.DataFrame(rows).to_csv(path, index=False)
        log.info("GA convergence CSV → %s", path)

    def plot_convergence(self, path: str | Path = None, show: bool = True):
        import matplotlib.pyplot as plt
        fig, ax = plt.subplots(figsize=(10, 5))
        ax.plot(self.convergence, 'b-', linewidth=1.5)
        ax.set_xlabel("Generation")
        ax.set_ylabel("Best Fitness")
        ax.set_title(f"GA Convergence  (best={self.best_fitness:.3f}, {len(self.convergence)} gen)")
        ax.grid(True, alpha=0.3)
        if path:
            fig.savefig(path, dpi=150, bbox_inches='tight')
        if show:
            plt.show()
        return fig

    def best_nec_model(self, builder: Callable):
        """Reconstruct NECModel from best genes."""
        return builder(self.best_genes)

    def __repr__(self):
        bg = ", ".join(f"{k}={v:.4f}" for k, v in list(self.best_genes.items())[:4])
        return f"GAResult(fitness={self.best_fitness:.3f}, {bg}...)"


# ─── Genetic Algorithm ────────────────────────────────────────────────────────

class GeneticOptimizer:
    """
    Real-valued genetic algorithm for antenna optimization.

    Parameters
    ----------
    params : List[GeneParameter]
        Parameters to optimize (defines search space).
    fitness_fn : FitnessFunction
        Objective function.
    model_builder : Callable[[Dict[str,float]], NECModel]
        Function that accepts genes dict and returns NECModel.
    runner : NECRunner
        NEC engine runner.
    pop_size : int          Population size per generation (default 50)
    n_generations : int     Maximum generations (default 100)
    mutation_rate : float   Per-gene mutation probability (default 0.1)
    crossover_rate : float  Probability of crossover vs cloning (default 0.8)
    tournament_size : int   Tournament selection k (default 3)
    elitism : int           Number of elite individuals carried forward (default 2)
    checkpoint_every : int  Save checkpoint every N generations (default 10)
    checkpoint_path : Path  Checkpoint file path
    early_stop_gens : int   Stop if no improvement for N gens (default 20)
    """

    def __init__(self, params: List[GeneParameter],
                 fitness_fn: FitnessFunction,
                 model_builder: Callable,
                 runner,
                 pop_size: int = 50,
                 n_generations: int = 100,
                 mutation_rate: float = 0.1,
                 crossover_rate: float = 0.8,
                 tournament_size: int = 3,
                 elitism: int = 2,
                 checkpoint_every: int = 10,
                 checkpoint_path: Path = None,
                 early_stop_gens: int = 20,
                 n_parallel: int = 1):
        self.params           = params
        self.fitness_fn       = fitness_fn
        self.builder          = model_builder
        self.runner           = runner
        self.pop_size         = pop_size
        self.n_gen            = n_generations
        self.mut_rate         = mutation_rate
        self.cross_rate       = crossover_rate
        self.tournament_k     = tournament_size
        self.elitism          = elitism
        self.ckpt_every       = checkpoint_every
        self.ckpt_path        = Path(checkpoint_path) if checkpoint_path else None
        self.early_stop       = early_stop_gens
        self.n_parallel       = n_parallel
        self.n_genes          = len(params)

    # ── Population Init ───────────────────────────────────────────────────────

    def initialize_population(self) -> List[Individual]:
        return [Individual(genes=np.array([p.random() for p in self.params]))
                for _ in range(self.pop_size)]

    # ── Evaluation ────────────────────────────────────────────────────────────

    def _evaluate_one(self, ind: Individual) -> Individual:
        """Evaluate a single individual (run NEC and compute fitness)."""
        genes_dict = {p.name: float(ind.genes[i]) for i, p in enumerate(self.params)}
        try:
            model  = self.builder(genes_dict)
            result = self.runner.run(model)
            if not result.success:
                ind.fitness = self.fitness_fn._penalize("NEC run failed")
                return ind
            from nec_parser import NECOutputParser
            pts = NECOutputParser().parse(result.output_file)
            if not pts:
                ind.fitness = self.fitness_fn._penalize("no parsed output")
                return ind
            ind.fitness    = self.fitness_fn.evaluate(pts[0])
            ind.nec_result = pts[0]
        except Exception as e:
            log.debug("Evaluation error: %s", e)
            ind.fitness = self.fitness_fn._penalize(str(e))
        return ind

    def evaluate_population(self, pop: List[Individual]) -> List[Individual]:
        return [self._evaluate_one(ind) for ind in pop]

    # ── Selection ─────────────────────────────────────────────────────────────

    def tournament_select(self, pop: List[Individual]) -> Individual:
        contestants = random.sample(pop, min(self.tournament_k, len(pop)))
        return max(contestants, key=lambda ind: ind.fitness).copy()

    # ── Crossover ─────────────────────────────────────────────────────────────

    def uniform_crossover(self, p1: Individual, p2: Individual):
        """Uniform crossover: each gene independently from p1 or p2."""
        mask = np.random.randint(0, 2, size=self.n_genes).astype(bool)
        c1_genes = np.where(mask, p1.genes, p2.genes)
        c2_genes = np.where(mask, p2.genes, p1.genes)
        return Individual(genes=c1_genes), Individual(genes=c2_genes)

    # ── Mutation ──────────────────────────────────────────────────────────────

    def gaussian_mutate(self, ind: Individual, sigma_fraction: float = 0.1) -> Individual:
        """Gaussian mutation: add noise to each gene with probability mut_rate."""
        genes = ind.genes.copy()
        for i, p in enumerate(self.params):
            if random.random() < self.mut_rate:
                sigma  = p.range * sigma_fraction
                genes[i] = p.clamp(genes[i] + random.gauss(0, sigma))
        return Individual(genes=genes)

    # ── Main Loop ─────────────────────────────────────────────────────────────

    def run(self) -> GAResult:
        """Run genetic algorithm and return GAResult."""
        t0 = time.monotonic()
        pop = self.initialize_population()
        pop = self.evaluate_population(pop)
        pop.sort(key=lambda ind: ind.fitness, reverse=True)

        convergence        = [pop[0].fitness]
        best               = pop[0].copy()
        no_improvement_gen = 0

        log.info("GA start: %d individuals, %d params, max %d generations",
                 self.pop_size, self.n_genes, self.n_gen)

        for gen in range(1, self.n_gen + 1):
            # Elitism: preserve top individuals
            new_pop = [ind.copy() for ind in pop[:self.elitism]]

            # Fill remaining slots
            while len(new_pop) < self.pop_size:
                p1 = self.tournament_select(pop)
                if random.random() < self.cross_rate:
                    p2  = self.tournament_select(pop)
                    c1, c2 = self.uniform_crossover(p1, p2)
                else:
                    c1, c2 = p1.copy(), self.tournament_select(pop).copy()
                new_pop.append(self.gaussian_mutate(c1))
                if len(new_pop) < self.pop_size:
                    new_pop.append(self.gaussian_mutate(c2))

            # Evaluate new population (skip elites already evaluated)
            to_eval = new_pop[self.elitism:]
            evaluated = self.evaluate_population(to_eval)
            pop = new_pop[:self.elitism] + evaluated
            pop.sort(key=lambda ind: ind.fitness, reverse=True)

            gen_best = pop[0].fitness
            convergence.append(gen_best)

            if gen_best > best.fitness:
                best = pop[0].copy()
                no_improvement_gen = 0
            else:
                no_improvement_gen += 1

            if gen % 5 == 0:
                mean_fit = np.mean([ind.fitness for ind in pop])
                log.info("Gen %3d/%d  best=%.3f  mean=%.3f  no_impr=%d",
                         gen, self.n_gen, gen_best, mean_fit, no_improvement_gen)

            if self.ckpt_path and gen % self.ckpt_every == 0:
                self._save_checkpoint(gen, best, convergence)

            if no_improvement_gen >= self.early_stop:
                log.info("Early stop at gen %d (no improvement for %d generations)",
                         gen, self.early_stop)
                break

        duration = time.monotonic() - t0
        log.info("GA complete: best fitness=%.3f in %.1fs", best.fitness, duration)
        return GAResult(best, self.params, convergence, pop, duration)

    # ── Checkpoint ────────────────────────────────────────────────────────────

    def _save_checkpoint(self, gen: int, best: Individual, convergence: List[float]):
        if not self.ckpt_path:
            return
        data = {
            "generation":  gen,
            "best_genes":  {p.name: float(best.genes[i])
                            for i, p in enumerate(self.params)},
            "best_fitness": float(best.fitness),
            "convergence":  [float(f) for f in convergence],
        }
        self.ckpt_path.write_text(json.dumps(data, indent=2))
        log.debug("Checkpoint saved: gen %d → %s", gen, self.ckpt_path)
