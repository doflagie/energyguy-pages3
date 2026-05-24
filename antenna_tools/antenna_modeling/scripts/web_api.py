"""
web_api.py
TM-API-001 Rev A — NEC Antenna Modeling Web API

FastAPI-based REST service for antenna modeling and optimization.
Accepts antenna specs, runs NEC in background, returns structured results.
Designed for cloud deployment (Docker-friendly, stateless job queue).

Run:
    python web_api.py                        # Development
    uvicorn web_api:app --host 0.0.0.0 --port 8000  # Production

API endpoints:
    POST /model/run              Submit sweep job
    GET  /model/status/{job_id}  Poll job status
    GET  /model/results/{job_id} Get results JSON
    GET  /model/results/{job_id}/csv  Download CSV
    GET  /model/results/{job_id}/pattern3d/{freq_mhz}  Get 3D pattern
    POST /model/optimize         Submit optimization job
    GET  /model/optimize/results/{job_id}  Get optimization results
    POST /antenna/generate       Generate NEC file (no run)
    GET  /health                 Health check
"""

from __future__ import annotations

import asyncio
import json
import logging
import os
import time
import uuid
from datetime import datetime
from pathlib import Path
from typing import Any, Dict, List, Optional

import uvicorn
from fastapi import BackgroundTasks, FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import PlainTextResponse, StreamingResponse
from pydantic import BaseModel, Field

log = logging.getLogger(__name__)

# ─── Pydantic Models ──────────────────────────────────────────────────────────

class SweepRequest(BaseModel):
    spec: Dict[str, Any] = Field(
        ...,
        example={"type": "dipole", "freq_mhz": 14.25, "height_m": 10.0})
    sweep: Dict[str, float] = Field(
        default={"start": 14.0, "stop": 14.35, "step": 0.05},
        description="start/stop/step in MHz")
    pattern_inc_deg: float = 5.0
    engine: str = "auto"

class OptimizeRequest(BaseModel):
    spec_template: Dict[str, Any]
    params: List[Dict[str, Any]] = Field(
        ...,
        description="List of {name, min, max} dicts")
    objectives: Dict[str, float] = Field(
        default={"swr_target": 1.5, "gain_min_dBi": 0.0, "fb_min_dB": 10.0})
    algorithm: str = Field(default="pso", description="ga or pso")
    config: Dict[str, Any] = Field(default_factory=dict)

class GenerateRequest(BaseModel):
    type: str
    freq_mhz: float = 14.25
    height_m: float = 10.0
    ground_type: int = 0
    n_directors: int = 3
    tau: float = 0.9
    sigma: float = 0.07
    n_elements: int = 2
    n_radials: int = 32
    wire_radius_m: float = 0.001


# ─── Job Store ────────────────────────────────────────────────────────────────

class NECJob:
    def __init__(self, job_id: str, job_type: str = "sweep"):
        self.job_id      = job_id
        self.job_type    = job_type   # "sweep" or "optimize"
        self.status      = "pending"  # pending, running, complete, failed
        self.progress    = 0.0        # 0.0–1.0
        self.result      = None
        self.error       = None
        self.created_at  = datetime.utcnow().isoformat()
        self.completed_at: Optional[str] = None
        self.eta_s: Optional[float] = None

    def to_dict(self) -> dict:
        return {
            "job_id": self.job_id,
            "status": self.status,
            "progress_pct": round(self.progress * 100, 1),
            "eta_s": self.eta_s,
            "created_at": self.created_at,
            "completed_at": self.completed_at,
            "error": self.error,
        }


class JobStore:
    """In-memory job store. Replace with Redis for production scaling."""
    def __init__(self):
        self._jobs: Dict[str, NECJob] = {}
        self._semaphore: Optional[asyncio.Semaphore] = None

    def create(self, job_type: str = "sweep") -> NECJob:
        job_id = str(uuid.uuid4())[:12]
        job    = NECJob(job_id, job_type)
        self._jobs[job_id] = job
        return job

    def get(self, job_id: str) -> Optional[NECJob]:
        return self._jobs.get(job_id)

    def require(self, job_id: str) -> NECJob:
        job = self.get(job_id)
        if job is None:
            raise HTTPException(status_code=404, detail=f"Job {job_id} not found")
        return job


# ─── App Setup ────────────────────────────────────────────────────────────────

app       = FastAPI(
    title="NEC Antenna Modeling API",
    description="TM-API-001 — Cloud-based NEC2/NEC4 modeling service",
    version="1.0.0"
)
store     = JobStore()
NEC_ENGINE = "not_found"

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)

MAX_CONCURRENT = asyncio.Semaphore(10)   # Initialized properly in startup


@app.on_event("startup")
async def startup():
    global NEC_ENGINE, MAX_CONCURRENT
    MAX_CONCURRENT = asyncio.Semaphore(
        int(os.environ.get("MAX_CONCURRENT_NEC", 10)))
    try:
        from nec_runner import NECRunner
        runner     = NECRunner()
        NEC_ENGINE = runner.engine.value
        log.info("NEC engine: %s (%s)", NEC_ENGINE, runner.executable)
    except Exception as e:
        log.warning("NEC engine not found at startup: %s", e)


# ─── Background Tasks ─────────────────────────────────────────────────────────

def _run_sweep_job(job: NECJob, request: SweepRequest):
    from freq_sweep import FreqSweep, SweepConfig
    from nec_generator import build_from_dict

    try:
        job.status   = "running"
        sweep_cfg    = SweepConfig(
            freq_start_mhz=request.sweep["start"],
            freq_stop_mhz=request.sweep["stop"],
            freq_step_mhz=request.sweep.get("step", 0.05),
            pattern_inc_deg=request.pattern_inc_deg,
            engine_name=request.engine,
        )
        n_steps = sweep_cfg.n_steps

        def builder(freq_mhz):
            s = dict(request.spec)
            s["freq_mhz"] = freq_mhz
            m = build_from_dict(s)
            m.add_pattern(theta_inc=request.pattern_inc_deg,
                          phi_inc=request.pattern_inc_deg)
            return m

        sweep       = FreqSweep(builder, sweep_cfg)
        result      = sweep.run()

        # Package result
        df          = result.to_dataframe()
        job.result  = {
            "freq_points": df.to_dict(orient="records"),
            "summary": {
                "best_swr_freq_mhz": result.best_swr_freq(),
                "min_swr_50":        float(df.swr_50.min()),
                "max_gain_dBi":      float(df.gain_dBi.max()),
                "swr_bw_2to1_mhz":  result.swr_bandwidth_mhz(),
                "n_points":          len(result.freq_points),
            },
            "_sweep_result": result   # Store for CSV/pattern endpoints
        }
        job.status   = "complete"
        job.progress = 1.0
    except Exception as e:
        job.status = "failed"
        job.error  = str(e)
        log.exception("Sweep job %s failed", job.job_id)
    finally:
        job.completed_at = datetime.utcnow().isoformat()


def _run_optimize_job(job: NECJob, request: OptimizeRequest):
    from genetic_optimizer import GeneParameter, FitnessFunction, GeneticOptimizer
    from pso_optimizer import PSOOptimizer, PSOConfig
    from nec_runner import NECRunner
    from nec_generator import build_from_dict

    try:
        job.status = "running"
        params     = [GeneParameter(p["name"], p["min"], p["max"])
                      for p in request.params]
        fitness_fn = FitnessFunction(request.objectives)
        runner     = NECRunner()

        def builder(genes_dict):
            s = dict(request.spec_template)
            s.update(genes_dict)
            return build_from_dict(s)

        if request.algorithm.lower() == "ga":
            from genetic_optimizer import GeneticOptimizer
            cfg = request.config
            opt = GeneticOptimizer(
                params, fitness_fn, builder, runner,
                pop_size=cfg.get("pop_size", 30),
                n_generations=cfg.get("n_generations", 50),
            )
            result = opt.run()
        else:
            cfg    = PSOConfig(
                n_particles=request.config.get("n_particles", 20),
                n_iterations=request.config.get("n_iterations", 100),
            )
            opt    = PSOOptimizer(params, fitness_fn, builder, runner, config=cfg)
            result = opt.run()

        job.result = {
            "best_genes":     result.best_genes,
            "best_fitness":   result.best_fitness,
            "convergence":    result.convergence,
            "duration_s":     result.duration_s,
            "algorithm":      request.algorithm,
        }
        job.status   = "complete"
        job.progress = 1.0
    except Exception as e:
        job.status = "failed"
        job.error  = str(e)
        log.exception("Optimize job %s failed", job.job_id)
    finally:
        job.completed_at = datetime.utcnow().isoformat()


# ─── Routes ───────────────────────────────────────────────────────────────────

@app.get("/health")
def health():
    return {"status": "ok", "nec_engine": NEC_ENGINE, "version": "1.0.0",
            "active_jobs": sum(1 for j in store._jobs.values()
                               if j.status == "running")}


@app.post("/model/run", status_code=202)
def submit_sweep(req: SweepRequest, background_tasks: BackgroundTasks):
    job = store.create("sweep")
    background_tasks.add_task(_run_sweep_job, job, req)
    log.info("Sweep job %s queued: %s %.2f–%.2f MHz",
             job.job_id, req.spec.get("type"), req.sweep["start"], req.sweep["stop"])
    return {"job_id": job.job_id, "status_url": f"/model/status/{job.job_id}"}


@app.get("/model/status/{job_id}")
def get_status(job_id: str):
    return store.require(job_id).to_dict()


@app.get("/model/results/{job_id}")
def get_results(job_id: str):
    job = store.require(job_id)
    if job.status != "complete":
        raise HTTPException(status_code=202, detail=f"Job status: {job.status}")
    result = dict(job.result)
    result.pop("_sweep_result", None)   # Don't serialize the heavy object
    return result


@app.get("/model/results/{job_id}/csv")
def get_csv(job_id: str):
    job = store.require(job_id)
    if job.status != "complete":
        raise HTTPException(status_code=202, detail="Not complete")
    sweep = job.result.get("_sweep_result")
    if sweep is None:
        raise HTTPException(status_code=404, detail="No sweep result stored")

    import io
    buf = io.StringIO()
    sweep.to_dataframe().to_csv(buf, index=False, float_format="%.4f")
    buf.seek(0)
    return StreamingResponse(iter([buf.getvalue()]),
                             media_type="text/csv",
                             headers={"Content-Disposition":
                                      f'attachment; filename="results_{job_id}.csv"'})


@app.get("/model/results/{job_id}/pattern3d/{freq_mhz}")
def get_pattern3d(job_id: str, freq_mhz: float):
    job = store.require(job_id)
    if job.status != "complete":
        raise HTTPException(status_code=202, detail="Not complete")
    sweep = job.result.get("_sweep_result")
    if sweep is None:
        raise HTTPException(status_code=404, detail="No sweep data")

    # Find closest frequency point
    pts   = sweep.freq_points
    if not pts:
        raise HTTPException(status_code=404, detail="No frequency points")
    pt    = min(pts, key=lambda p: abs(p.freq_mhz - freq_mhz))
    if pt.pattern is None:
        raise HTTPException(status_code=404, detail="No pattern data at this frequency")

    from pattern_3d import Pattern3D
    p3d = Pattern3D.from_freq_point(pt)
    import io, json as _json
    buf = io.StringIO()
    buf.write('{"freq_mhz":' + str(pt.freq_mhz) + ',"max_gain_dBi":' +
              str(round(float(pt.gain_dbi_max), 2)) + ',"points":[')

    x, y, z, r = p3d.to_cartesian()
    first = True
    for ti in range(len(p3d.theta)):
        for pi in range(len(p3d.phi)):
            import math
            g = p3d.gain[ti, pi]
            if not math.isfinite(g):
                continue
            if not first:
                buf.write(",")
            first = False
            buf.write(f'{{"t":{p3d.theta[ti]:.1f},"p":{p3d.phi[pi]:.1f},'
                      f'"g":{g:.2f},'
                      f'"x":{x[ti,pi]:.4f},"y":{y[ti,pi]:.4f},"z":{z[ti,pi]:.4f}}}')
    buf.write("]}")
    buf.seek(0)
    return PlainTextResponse(buf.getvalue(), media_type="application/json")


@app.post("/model/optimize", status_code=202)
def submit_optimize(req: OptimizeRequest, background_tasks: BackgroundTasks):
    job = store.create("optimize")
    background_tasks.add_task(_run_optimize_job, job, req)
    return {"job_id": job.job_id, "status_url": f"/model/status/{job.job_id}"}


@app.get("/model/optimize/results/{job_id}")
def get_optimize_results(job_id: str):
    job = store.require(job_id)
    if job.status != "complete":
        raise HTTPException(status_code=202, detail=f"Job status: {job.status}")
    return job.result


@app.post("/antenna/generate", response_class=PlainTextResponse)
def generate_nec(req: GenerateRequest):
    from nec_generator import build_from_dict
    spec = req.dict()
    try:
        model = build_from_dict(spec)
        model.add_pattern()
        return model.to_string()
    except Exception as e:
        raise HTTPException(status_code=422, detail=str(e))


# ─── Entry Point ──────────────────────────────────────────────────────────────

if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO,
                        format="%(asctime)s %(levelname)s %(name)s: %(message)s")
    uvicorn.run(app, host="0.0.0.0", port=8000, reload=False)
