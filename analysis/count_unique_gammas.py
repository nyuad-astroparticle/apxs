#!/usr/bin/env python3
"""Count unique gamma tracks at a target energy across simulation output files.

This script reuses the parallel helpers from ``analysis/accelerate.py`` to load
the ``*_nt_hits.csv`` files under ``build/output`` in parallel, then counts the
number of unique gamma tracks whose initial energy is close to the requested
value. Uniqueness is defined by the ``(EventID, TrackID)`` pair within each
file.
"""
import os
from pathlib import Path
import numpy as np
import pandas as pd

# Keep matplotlib cache writable when accelerate imports matplotlib
MPL_CACHE = Path(__file__).resolve().parent / ".matplotlib_cache"
MPL_CACHE.mkdir(exist_ok=True)
os.environ.setdefault("MPLCONFIGDIR", str(MPL_CACHE))
os.environ.setdefault("XDG_CACHE_HOME", str(MPL_CACHE))

from accelerate import parallel, loadFile


TARGET_ENERGY_KEV = 5.85868
ENERGY_TOLERANCE = 1e-4
PARTICLE_NAME = "gamma"
UNIQUE_ID_COLUMNS = ["EventID", "TrackID"]


def _count_gammas_in_file(path, target_energy, energy_tol, id_columns, particle):
    """Return unique gamma tracks (with process/volumes) in a single file."""
    df = loadFile(path)
    energy_match = np.isclose(df["InitialEnergy"], target_energy, rtol=0, atol=energy_tol)
    particle_match = df["Particle"] == particle
    subset = df.loc[energy_match & particle_match]
    if subset.empty:
        return pd.DataFrame(columns=[*id_columns, "ProcessName", "ParentVolume", "Volume", "filename"])

    unique_tracks = subset.drop_duplicates(subset=id_columns, keep="first")
    unique_tracks = unique_tracks[[*id_columns, "ProcessName", "ParentVolume", "Volume"]]
    unique_tracks["filename"] = Path(path).name
    return unique_tracks

# Parallelized wrapper kept separate so the worker stays picklable
count_gammas_in_file = parallel(_count_gammas_in_file)


def main():
    data_dir = Path(__file__).resolve().parents[1] / "build" / "output"
    files = sorted(data_dir.glob("*_nt_hits*.csv"))

    if not files:
        print(f"No ntuple hit files found under {data_dir}")
        return

    print(f"Scanning {len(files)} files from {data_dir} ...")
    gamma_rows = count_gammas_in_file(
        files,
        target_energy=TARGET_ENERGY_KEV,
        energy_tol=ENERGY_TOLERANCE,
        id_columns=UNIQUE_ID_COLUMNS,
        particle=PARTICLE_NAME,
    )
    gamma_df = pd.concat(gamma_rows, ignore_index=True)

    total = len(gamma_df)
    decay_count = (gamma_df["ProcessName"] == "RadioactiveDecay").sum()
    print(f"Total unique {PARTICLE_NAME} tracks at {TARGET_ENERGY_KEV:.5f} keV (±{ENERGY_TOLERANCE}): {total}")
    print(f"Of these, created by RadioactiveDecay: {decay_count}")

    per_file = gamma_df.groupby("filename").size().reset_index(name="count")
    per_file = per_file[per_file["count"] > 0].sort_values("filename")
    if not per_file.empty:
        print("\nPer-file counts (only files with matches):")
        print(per_file.to_string(index=False))

    # Process bar chart
    process_counts = gamma_df["ProcessName"].value_counts().sort_values(ascending=False)
    volume_counts = gamma_df["ParentVolume"].value_counts().sort_values(ascending=False)

    plots_dir = Path(__file__).resolve().parent
    process_plot = plots_dir / "gamma_process_counts.png"
    volume_plot = plots_dir / "gamma_origin_volume_counts.png"

    if not process_counts.empty:
        ax = process_counts.plot(kind="bar", title="Processes for gamma at target energy")
        ax.set_xlabel("ProcessName")
        ax.set_ylabel("Count")
        ax.figure.tight_layout()
        ax.figure.savefig(process_plot)
        ax.figure.clf()
        print(f"\nSaved process bar chart to {process_plot}")

    if not volume_counts.empty:
        ax = volume_counts.plot(kind="bar", title="Origin volumes (ParentVolume) for gamma at target energy")
        ax.set_xlabel("ParentVolume")
        ax.set_ylabel("Count")
        ax.figure.tight_layout()
        ax.figure.savefig(volume_plot)
        ax.figure.clf()
        print(f"Saved origin volume bar chart to {volume_plot}")


if __name__ == "__main__":
    main()
