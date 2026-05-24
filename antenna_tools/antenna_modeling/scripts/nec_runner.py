"""
nec_runner.py
TM-RUNNER-001 Rev A — NEC Engine Interface

Runs NEC2, NEC4, and 4NEC2 from Python. Handles subprocess invocation,
stdout/stderr capture, engine detection, output file management, and timeout.

Supported engines:
    nec2c      — Linux NEC2 (Debian: apt install nec2c)
    nec4       — NEC4 (licensed, Linux)
    4nec2      — Windows GUI engine via Wine
    necpp      — NEC++ alternative engine

Usage:
    runner = NECRunner(engine=NECEngine.NEC2)
    result = runner.run(model)
    if result.success:
        parser = NECOutputParser()
        points = parser.parse(result.output_file)
"""

from __future__ import annotations

import logging
import os
import shutil
import subprocess
import tempfile
import time
from dataclasses import dataclass, field
from enum import Enum, auto
from pathlib import Path
from typing import Optional

log = logging.getLogger(__name__)


# ─── Engine Enumeration ───────────────────────────────────────────────────────

class NECEngine(Enum):
    NEC2     = "nec2c"
    NEC4     = "nec4"
    FOURNEC2 = "4nec2"
    NECPP    = "necpp"
    AUTO     = "auto"


# ─── Result Container ─────────────────────────────────────────────────────────

@dataclass
class NECResult:
    input_file:  Path
    output_file: Optional[Path]
    stdout:      str
    stderr:      str
    returncode:  int
    duration_s:  float
    engine_used: NECEngine = NECEngine.NEC2

    @property
    def success(self) -> bool:
        return (self.returncode == 0 and
                self.output_file is not None and
                self.output_file.exists() and
                self.output_file.stat().st_size > 0)

    def __repr__(self) -> str:
        status = "OK" if self.success else f"FAILED(rc={self.returncode})"
        return (f"NECResult({status}, engine={self.engine_used.value}, "
                f"t={self.duration_s:.2f}s, out={self.output_file})")


# ─── Engine Runner ────────────────────────────────────────────────────────────

class NECRunner:
    """
    Wraps NEC2/NEC4/4NEC2 subprocess execution.

    Parameters
    ----------
    engine : NECEngine
        Engine to use. NECEngine.AUTO detects first available engine in PATH.
    executable_path : str or Path, optional
        Explicit path to NEC executable. Overrides engine detection.
    work_dir : str or Path, optional
        Working directory for .nec and .out files. Defaults to system tempdir.
    timeout_s : float
        Maximum seconds to wait for NEC subprocess. Default 120s.
    keep_files : bool
        If True, do not delete .nec/.out files after run. Useful for debugging.
    """

    # Search order for auto-detection
    _CANDIDATES = ["nec2c", "nec2", "nec4", "necpp", "4nec2.exe"]

    def __init__(self,
                 engine: NECEngine = NECEngine.AUTO,
                 executable_path: Optional[str | Path] = None,
                 work_dir: Optional[str | Path] = None,
                 timeout_s: float = 120.0,
                 keep_files: bool = False):
        self.timeout_s   = timeout_s
        self.keep_files  = keep_files
        self._work_dir   = Path(work_dir) if work_dir else None

        if executable_path:
            self._exe    = Path(executable_path)
            self._engine = engine if engine != NECEngine.AUTO else NECEngine.NEC2
        else:
            self._exe, self._engine = self._detect_engine(engine)

        log.info("NECRunner: engine=%s executable=%s", self._engine.value, self._exe)

    # ── Engine Detection ──────────────────────────────────────────────────────

    def _detect_engine(self, requested: NECEngine) -> tuple[Path, NECEngine]:
        """Find NEC executable in PATH. Returns (path, engine_enum)."""
        if requested != NECEngine.AUTO:
            exe = shutil.which(requested.value)
            if exe:
                return Path(exe), requested
            raise FileNotFoundError(
                f"Requested engine '{requested.value}' not found in PATH. "
                f"Install with: apt install nec2c  (or see manual Chapter 2.2)")

        for candidate in self._CANDIDATES:
            exe = shutil.which(candidate)
            if exe:
                for e in NECEngine:
                    if e.value == candidate:
                        return Path(exe), e
                return Path(exe), NECEngine.NEC2   # unknown binary, treat as nec2

        raise FileNotFoundError(
            "No NEC engine found in PATH. "
            "Install nec2c: sudo apt install nec2c\n"
            "Or set executable_path= explicitly.")

    @property
    def engine(self) -> NECEngine:
        return self._engine

    @property
    def executable(self) -> Path:
        return self._exe

    # ── Work Directory ────────────────────────────────────────────────────────

    def _get_work_dir(self) -> Path:
        if self._work_dir:
            self._work_dir.mkdir(parents=True, exist_ok=True)
            return self._work_dir
        return Path(tempfile.mkdtemp(prefix="nec_"))

    # ── Run from NECModel ─────────────────────────────────────────────────────

    def run(self, model, output_stem: str = None) -> NECResult:
        """
        Run a NECModel.

        Parameters
        ----------
        model : NECModel
            Model object with to_string() method.
        output_stem : str, optional
            Base filename (no extension). Auto-generated if None.

        Returns
        -------
        NECResult
        """
        work_dir = self._get_work_dir()
        stem     = output_stem or f"nec_{int(time.time()*1000)}"
        nec_file = work_dir / f"{stem}.nec"
        out_file = work_dir / f"{stem}.out"

        nec_file.write_text(model.to_string())
        log.debug("Wrote NEC input: %s (%d bytes)", nec_file, nec_file.stat().st_size)

        return self.run_file(nec_file, out_file)

    # ── Run from File ─────────────────────────────────────────────────────────

    def run_file(self, nec_path: Path,
                 out_path: Optional[Path] = None) -> NECResult:
        """
        Run NEC on an existing .nec file.

        Parameters
        ----------
        nec_path : Path
            Path to .nec input file.
        out_path : Path, optional
            Desired output file path. If None, replaces .nec suffix with .out.

        Returns
        -------
        NECResult
        """
        nec_path = Path(nec_path)
        if out_path is None:
            out_path = nec_path.with_suffix(".out")

        cmd = self._build_command(nec_path, out_path)
        log.info("Running: %s", " ".join(str(c) for c in cmd))

        t0 = time.monotonic()
        try:
            proc = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=self.timeout_s,
                cwd=nec_path.parent
            )
        except subprocess.TimeoutExpired as e:
            duration = time.monotonic() - t0
            log.error("NEC timeout after %.1fs: %s", duration, nec_path)
            return NECResult(
                input_file=nec_path,
                output_file=None,
                stdout="",
                stderr=f"TIMEOUT after {self.timeout_s}s",
                returncode=-1,
                duration_s=duration,
                engine_used=self._engine
            )
        except FileNotFoundError:
            raise FileNotFoundError(
                f"NEC executable not found: {self._exe}\n"
                "Check installation and PATH.")

        duration = time.monotonic() - t0

        # nec2c writes output to <input_stem>.out by default
        # Some versions accept output path as second arg; handle both cases
        actual_out = out_path if out_path.exists() else nec_path.with_suffix(".out")
        if not actual_out.exists():
            # Try alternate: nec2c writes to input_file.out in same dir
            alt = nec_path.parent / (nec_path.stem + ".out")
            actual_out = alt if alt.exists() else None

        if proc.returncode != 0:
            log.warning("NEC returned %d. stderr: %s", proc.returncode, proc.stderr[:500])
        else:
            log.info("NEC complete in %.2fs → %s", duration, actual_out)

        result = NECResult(
            input_file=nec_path,
            output_file=actual_out,
            stdout=proc.stdout,
            stderr=proc.stderr,
            returncode=proc.returncode,
            duration_s=duration,
            engine_used=self._engine
        )

        if not self.keep_files and self._work_dir is None:
            # Cleanup temp files unless user wants to keep them
            # Keep output file — caller needs it; they are responsible for cleanup
            pass

        return result

    # ── Command Builder ───────────────────────────────────────────────────────

    def _build_command(self, nec_file: Path, out_file: Path) -> list:
        """Build subprocess argument list for the selected engine."""
        engine = self._engine

        if engine == NECEngine.NEC2:
            # nec2c input.nec output.out   (or just nec2c < input.nec for older)
            return [str(self._exe), str(nec_file), str(out_file)]

        elif engine == NECEngine.NEC4:
            return [str(self._exe), str(nec_file), str(out_file)]

        elif engine == NECEngine.NECPP:
            return [str(self._exe), "-i", str(nec_file), "-o", str(out_file)]

        elif engine == NECEngine.FOURNEC2:
            # 4NEC2 under Wine: 4nec2.exe /i input.nec /o output.out
            wine = shutil.which("wine") or "wine"
            return [wine, str(self._exe), "/i", str(nec_file), "/o", str(out_file)]

        return [str(self._exe), str(nec_file), str(out_file)]

    # ── Version Query ─────────────────────────────────────────────────────────

    def version(self) -> str:
        """Return engine version string."""
        try:
            r = subprocess.run([str(self._exe), "--version"],
                               capture_output=True, text=True, timeout=5)
            v = r.stdout.strip() or r.stderr.strip()
            return v[:200] if v else f"{self._engine.value} (version unknown)"
        except Exception:
            return f"{self._engine.value} (version query failed)"


# ─── Convenience function ─────────────────────────────────────────────────────

def run_model(model, engine: NECEngine = NECEngine.AUTO,
              keep_files: bool = False) -> NECResult:
    """One-shot: create runner, run model, return result."""
    runner = NECRunner(engine=engine, keep_files=keep_files)
    return runner.run(model)
