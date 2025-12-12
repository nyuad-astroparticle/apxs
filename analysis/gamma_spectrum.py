#!/usr/bin/env python3
"""Build an energy spectrum for all gamma tracks that reach the detector.

This collects every gamma hit in the detector volume across all ``*_nt_hits.csv``
files under ``build/output`` and plots the spectrum of their initial energies.
"""
import os
from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt

from accelerate import parallel, loadFile

# Writable cache to avoid matplotlib/fontconfig warnings
MPL_CACHE = Path(__file__).resolve().parent / ".matplotlib_cache"
MPL_CACHE.mkdir(exist_ok=True)
os.environ.setdefault("MPLCONFIGDIR", str(MPL_CACHE))
os.environ.setdefault("XDG_CACHE_HOME", str(MPL_CACHE))

PARTICLE_NAME = "gamma"
DETECTOR_VOLUME = "detectLogical_PV"
UNIQUE_KEYS = ["EventID", "TrackID", "filename"]


def _gamma_hits_in_file(path, target_volume):
    """Return gamma rows in the detector volume from a single file."""
    df = loadFile(path)
    mask = (df["Particle"] == PARTICLE_NAME) & (df["Volume"] == target_volume)
    hits = df.loc[mask, ["EventID", "TrackID", "InitialEnergy", "DepositedEnergy",
                         "ProcessName", "Volume", "ParentVolume", "filename"]]
    hits["filename"] = Path(path).name
    return hits


gamma_hits_in_file = parallel(_gamma_hits_in_file)


def main():
    data_dir = Path(__file__).resolve().parents[1] / "build" / "output"
    files = sorted(data_dir.glob("*_nt_hits*.csv"))
    if not files:
        print(f"No ntuple hit files found under {data_dir}")
        return

    print(f"Loading gamma hits from {len(files)} files in {data_dir} ...")
    hits_list = gamma_hits_in_file(files, target_volume=DETECTOR_VOLUME)
    hits = pd.concat(hits_list, ignore_index=True)

    if hits.empty:
        print("No gamma hits found in detector volume.")
        return

    unique_hits = hits.drop_duplicates(subset=UNIQUE_KEYS, keep="first")
    print(f"Total gamma hits (rows) in detector: {len(hits)}")
    print(f"Unique gamma tracks reaching detector: {len(unique_hits)}")

    # Energy spectrum on InitialEnergy (keV)
    min_e, max_e = unique_hits["InitialEnergy"].min(), unique_hits["InitialEnergy"].max()
    bins = 200

    fig, ax = plt.subplots(figsize=(10, 5))
    ax.hist(unique_hits["InitialEnergy"], bins=bins, color="tab:blue", edgecolor="black", alpha=0.7)
    ax.set_title(f"Gamma energy spectrum at detector ({DETECTOR_VOLUME})")
    ax.set_xlabel("InitialEnergy [keV]")
    ax.set_ylabel("Count (unique tracks)")
    ax.set_xlim(min_e, max_e)
    fig.tight_layout()

    output_path = Path(__file__).resolve().parent / "gamma_spectrum_detected.png"
    fig.savefig(output_path)
    print(f"Spectrum saved to {output_path}")


if __name__ == "__main__":
    main()
