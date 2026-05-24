"""
pso_optimizer.py
TM-PSO-001 Rev A — Particle Swarm Optimizer for Antenna Design

PSO with constriction factor (Clerc-Kennedy), adaptive inertia weight decay,
and velocity clamping. Typically converges faster than GA for continuous
parameter spaces with smooth objective functions.

Drop-in compatible with genetic_optimizer.py: GeneParameter and FitnessFunction
are shared classes; GAResult and PSOResult have the same interface.

Usage:
    from pso_optimizer import PSOOptimizer, PSOConfig
    from genetic_optimizer import GeneParameter, FitnessFunction
    from nec_runner import NECRunner

    cfg = PSOConfig(n_particles=30, n_iterations=150)
    opt = PSOOptimizer(params, fitness_fn, builder, NECRunner(), config=cfg)
    result = opt.run()
    print(result.best_genes)
"""

from __future__ import annotations

import json
import logging
import math
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Callable, Dict, List, Optional

import numpy as np

log = logging.getLogger(__name__)


# ─── PSO Configuration ────────────────────────────────────────────────────────

@dataclass
class PSOConfig:
    """Particle Swarm Optimizer configuration."""
    n_particles:       int   = 30
    n_iterations:      int   = 200
    w_start:           float = 0.9     # Initial inertia weight
    w_end:             float = 0.4     # Final inertia weight (linear decay)
    c1:                float = 2.05    # Cognitive coefficient
    c2:                float = 2.05    # Social coefficient
    v_max_fraction:    float = 0.2     # Max velocity as fraction of parameter range
    constriction:      bool  = True    # Use Clerc-Kennedy constriction factor
    early_stop_iters:  int   = 30      # Stop if no improvement for N iterations
    checkpoint_every:  int   = 20
    checkpoint_path:   Optional[Path] = None

    @property
    def constriction_factor(self) -> float:
        """Clerc-Kennedy constriction factor χ."""
        phi = self.c1 + self.c2
        if phi <= 4.0:
            return 1.0   # Degenerate case
        return 2.0 / abs(2.0 - phi - math.sqrt(phi ** 2 - 4 * phi))


# ─── Particle ─────────────────────────────────────────────────────────────────

@dataclass
class Particle:
    position:      np.ndarray
    velocity:      np.ndarray
    best_position: np.ndarray
    best_fitness:  float      = float('-inf')
    curr_fitness:  float      = float('-inf')
    nec_result:    Optional[object] = field(default=None, repr=False)


# ─── PSO Result ───────────────────────────────────────────────────────────────

class PSOResult:
    """Result container for a completed PSO run. Same interface as GAResult."""

    def __init__(self, best_pos: np.ndarray, best_fit: float,
                 params, convergence: List[float],
                 gbest_history: List[np.ndarray], duration_s: float,
                 best_nec_result=None):
        self._best_pos       = best_pos
        self._best_fit       = best_fit
        self._params         = params
        self.convergence     = convergence
        self.gbest_history   = gbest_history
        self.duration_s      = duration_s
        self._best_nec_result = best_nec_result

    @property
    def best_genes(self) -> Dict[str, float]:
        return {p.name: float(self._best_pos[i])
                for i, p in enumerate(self._params)}

    @property
    def best_fitness(self) -> float:
        return self._best_fit

    def to_csv(self, path: str | Path):
        import pandas as pd
        rows = [{"iteration": i, "best_fitness": f}
                for i, f in enumerate(self.convergence)]
        pd.DataFrame(rows).to_csv(path, index=False)
        log.info("PSO convergence CSV → %s", path)

    def plot_convergence(self, path: str | Path = None, show: bool = True):
        import matplotlib.pyplot as plt
        fig, ax = plt.subplots(figsize=(10, 5))
        ax.plot(self.convergence, 'g-', linewidth=1.5)
        ax.set_xlabel("Iteration")
        ax.set_ylabel("Best Fitness")
        ax.set_title(f"PSO Convergence  (best={self.best_fitness:.3f}, "
                     f"{len(self.convergence)} iter)")
        ax.grid(True, alpha=0.3)
        if path:
            fig.savefig(path, dpi=150, bbox_inches='tight')
        if show:
            plt.show()
        return fig

    def best_nec_model(self, builder: Callable):
        return builder(self.best_genes)

    def __repr__(self):
        bg = ", ".join(f"{k}={v:.4f}" for k, v in list(self.best_genes.items())[:4])
        return f"PSOResult(fitness={self.best_fitness:.3f}, {bg}...)"


# ─── PSO Optimizer ────────────────────────────────────────────────────────────

class PSOOptimizer:
    """
    Particle Swarm Optimizer for antenna geometry.

    Shares GeneParameter and FitnessFunction with GeneticOptimizer.

    Parameters
    ----------
    params : List[GeneParameter]
    fitness_fn : FitnessFunction
    model_builder : Callable[[Dict[str,float]], NECModel]
    runner : NECRunner
    config : PSOConfig
    """

    def __init__(self, params, fitness_fn, model_builder: Callable,
                 runner, config: PSOConfig = None):
        self.params     = params
        self.fitness_fn = fitness_fn
        self.builder    = model_builder
        self.runner     = runner
        self.config     = config or PSOConfig()
        self.n_dims     = len(params)

        # Velocity bounds
        self._v_max = np.array([p.range * self.config.v_max_fraction
                                for p in params])
        # Position bounds
        self._lb    = np.array([p.min_val for p in params])
        self._ub    = np.array([p.max_val for p in params])

    # ── Initialization ────────────────────────────────────────────────────────

    def initialize_swarm(self) -> List[Particle]:
        particles = []
        for _ in range(self.config.n_particles):
            pos = np.array([p.random() for p in self.params])
            vel = np.array([np.random.uniform(-v, v) for v in self._v_max])
            particles.append(Particle(
                position=pos.copy(),
                velocity=vel,
                best_position=pos.copy(),
                best_fitness=float('-inf')
            ))
        return particles

    # ── Evaluation ────────────────────────────────────────────────────────────

    def _evaluate(self, particle: Particle) -> Particle:
        genes_dict = {p.name: float(particle.position[i])
                      for i, p in enumerate(self.params)}
        try:
            model  = self.builder(genes_dict)
            result = self.runner.run(model)
            if not result.success:
                particle.curr_fitness = self.fitness_fn._penalize("NEC failed")
                return particle
            from nec_parser import NECOutputParser
            pts = NECOutputParser().parse(result.output_file)
            if not pts:
                particle.curr_fitness = self.fitness_fn._penalize("no parsed output")
                return particle
            particle.curr_fitness = self.fitness_fn.evaluate(pts[0])
            particle.nec_result   = pts[0]
        except Exception as e:
            particle.curr_fitness = self.fitness_fn._penalize(str(e))
        return particle

    # ── Velocity / Position Update ────────────────────────────────────────────

    def update_velocity(self, p: Particle, gbest: np.ndarray, w: float) -> np.ndarray:
        r1 = np.random.uniform(0, 1, self.n_dims)
        r2 = np.random.uniform(0, 1, self.n_dims)
        c1, c2 = self.config.c1, self.config.c2

        cog     = c1 * r1 * (p.best_position - p.position)
        social  = c2 * r2 * (gbest - p.position)
        new_vel = w * p.velocity + cog + social

        if self.config.constriction:
            chi     = self.config.constriction_factor
            new_vel = chi * new_vel

        # Velocity clamping
        new_vel = np.clip(new_vel, -self._v_max, self._v_max)
        return new_vel

    def update_position(self, p: Particle) -> np.ndarray:
        new_pos = p.position + p.velocity
        # Reflection at bounds (prevents crowding at edges)
        for i in range(self.n_dims):
            if new_pos[i] < self._lb[i]:
                new_pos[i] = self._lb[i]
                p.velocity[i] = -p.velocity[i] * 0.5
            elif new_pos[i] > self._ub[i]:
                new_pos[i] = self._ub[i]
                p.velocity[i] = -p.velocity[i] * 0.5
            # Apply integer constraint if needed
            new_pos[i] = self.params[i].clamp(new_pos[i])
        return new_pos

    # ── Main Loop ─────────────────────────────────────────────────────────────

    def run(self) -> PSOResult:
        """Run PSO and return PSOResult."""
        cfg  = self.config
        t0   = time.monotonic()

        swarm = self.initialize_swarm()
        swarm = [self._evaluate(p) for p in swarm]

        # Initialize personal bests
        for p in swarm:
            p.best_fitness  = p.curr_fitness
            p.best_position = p.position.copy()

        # Global best
        gbest_particle  = max(swarm, key=lambda p: p.curr_fitness)
        gbest_pos       = gbest_particle.best_position.copy()
        gbest_fit       = gbest_particle.curr_fitness
        gbest_nec       = gbest_particle.nec_result

        convergence     = [gbest_fit]
        gbest_history   = [gbest_pos.copy()]
        no_impr_iters   = 0

        log.info("PSO start: %d particles, %d dims, max %d iterations",
                 cfg.n_particles, self.n_dims, cfg.n_iterations)

        for it in range(1, cfg.n_iterations + 1):
            # Adaptive inertia: linear decay w_start → w_end
            w = cfg.w_start - (cfg.w_start - cfg.w_end) * (it / cfg.n_iterations)

            for p in swarm:
                p.velocity  = self.update_velocity(p, gbest_pos, w)
                p.position  = self.update_position(p)
                p           = self._evaluate(p)

                if p.curr_fitness > p.best_fitness:
                    p.best_fitness  = p.curr_fitness
                    p.best_position = p.position.copy()

                if p.curr_fitness > gbest_fit:
                    gbest_fit = p.curr_fitness
                    gbest_pos = p.position.copy()
                    gbest_nec = p.nec_result
                    no_impr_iters = 0

            no_impr_iters += 1
            convergence.append(gbest_fit)
            gbest_history.append(gbest_pos.copy())

            if it % 10 == 0:
                mean_fit = np.mean([p.curr_fitness for p in swarm])
                log.info("Iter %3d/%d  gbest=%.3f  mean=%.3f  w=%.3f",
                         it, cfg.n_iterations, gbest_fit, mean_fit, w)

            if cfg.checkpoint_path and it % cfg.checkpoint_every == 0:
                self._save_checkpoint(it, gbest_pos, gbest_fit, convergence)

            if no_impr_iters >= cfg.early_stop_iters:
                log.info("Early stop at iter %d (no improvement for %d iters)",
                         it, cfg.early_stop_iters)
                break

        duration = time.monotonic() - t0
        log.info("PSO complete: gbest=%.3f in %.1fs", gbest_fit, duration)
        return PSOResult(gbest_pos, gbest_fit, self.params,
                         convergence, gbest_history, duration, gbest_nec)

    # ── Checkpoint ────────────────────────────────────────────────────────────

    def _save_checkpoint(self, it: int, pos: np.ndarray, fit: float,
                         conv: List[float]):
        path = self.config.checkpoint_path
        if not path:
            return
        data = {
            "iteration":   it,
            "best_genes":  {p.name: float(pos[i]) for i, p in enumerate(self.params)},
            "best_fitness": float(fit),
            "convergence": [float(f) for f in conv],
        }
        Path(path).write_text(json.dumps(data, indent=2))
        log.debug("PSO checkpoint → %s", path)
