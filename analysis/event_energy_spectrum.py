#!/usr/bin/env python3
"""Build a deposited-energy spectrum grouped by EventID and filename (all particles)."""
import os
from pathlib import Path
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from accelerate import parallel, loadFile

# Writable cache to avoid matplotlib/fontconfig warnings
MPL_CACHE = Path(__file__).resolve().parent / ".matplotlib_cache"
MPL_CACHE.mkdir(exist_ok=True)
os.environ.setdefault("MPLCONFIGDIR", str(MPL_CACHE))
os.environ.setdefault("XDG_CACHE_HOME", str(MPL_CACHE))

NBINS = 100


def _event_energy_in_file(path):
    """Sum deposited energy per EventID within a single file."""
    df = loadFile(path)
    grouped = df.groupby("EventID", as_index=False)["DepositedEnergy"].sum()
    grouped["filename"] = Path(path).name
    return grouped


event_energy_in_file = parallel(_event_energy_in_file)


def main():
    base = Path(__file__).resolve().parents[1]
    data_dir = base / "build" / "output"
    files = sorted(data_dir.glob("*_nt_hits*.csv"))
    if not files:
        print(f"No ntuple hit files found under {data_dir}")
        return

    print(f"Loading event energy sums from {len(files)} files in {data_dir} ...")
    frames = event_energy_in_file(files)
    data = pd.concat(frames, ignore_index=True)

    if data.empty:
        print("No data found.")
        return

    # Spectrum over per-event deposited energy
    energies = data["DepositedEnergy"].to_numpy()
    e_min, e_max = energies.min(), energies.max()
    bins = np.linspace(e_min, e_max, NBINS + 1) if e_max > e_min else NBINS

    fig, ax = plt.subplots(figsize=(10, 5))
    ax.hist(energies, bins=bins, color="tab:blue", edgecolor="black", alpha=0.7)
    ax.set_xlabel("DepositedEnergy per EventID [keV]")
    ax.set_ylabel("Count")
    ax.set_title("Per-event deposited energy spectrum (all particles)")
    fig.tight_layout()

    out_path = Path(__file__).resolve().parent / "event_energy_spectrum.png"
    fig.savefig(out_path)
    print(f"Spectrum saved to {out_path}")


if __name__ == "__main__":
    main()
