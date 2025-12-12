#!/usr/bin/env python3
"""Plot histogram of EventIDs for gammas at the target energy reaching the detector."""
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

TARGET_ENERGY_KEV = 5.85868
ENERGY_TOLERANCE = 1e-4
PARTICLE_NAME = "gamma"
UNIQUE_ID_COLUMNS = ["EventID", "TrackID"]
DETECTOR_VOLUME = "detectLogical_PV"


def _gammas_in_file(path, target_energy, energy_tol, particle, volume_name):
    """Return unique gamma tracks at target energy in the detector for one file."""
    df = loadFile(path)
    mask = (
        (df["Particle"] == particle)
        & np.isclose(df["InitialEnergy"], target_energy, rtol=0, atol=energy_tol)
        & (df["Volume"] == volume_name)
    )
    subset = df.loc[mask, UNIQUE_ID_COLUMNS]
    if subset.empty:
        return pd.DataFrame(columns=[*UNIQUE_ID_COLUMNS, "filename"])

    subset = subset.drop_duplicates(subset=UNIQUE_ID_COLUMNS, keep="first")
    subset["filename"] = Path(path).name
    return subset


gammas_in_file = parallel(_gammas_in_file)


def main():
    base = Path(__file__).resolve().parents[1]
    candidate_dirs = [
        base / "build" / "sandhya_output",
        base / "build" / "output",
        base / "build_main" / "output",
    ]
    data_dir = None
    files = []
    for d in candidate_dirs:
        if d.exists():
            files = sorted(d.glob("*_nt_hits*.csv"))
            if files:
                data_dir = d
                break
    if not files:
        print("No ntuple hit files found under any of:", *(str(d) for d in candidate_dirs))
        return

    print(f"Gathering gamma EventIDs from {len(files)} files ...")
    rows = gammas_in_file(
        files,
        target_energy=TARGET_ENERGY_KEV,
        energy_tol=ENERGY_TOLERANCE,
        particle=PARTICLE_NAME,
        volume_name=DETECTOR_VOLUME,
    )
    gamma_df = pd.concat(rows, ignore_index=True)
    if gamma_df.empty:
        print("No matching gamma tracks found.")
        return

    total_tracks = len(gamma_df)
    unique_events = int(gamma_df["EventID"].nunique())
    print(f"Unique gamma tracks at {TARGET_ENERGY_KEV:.5f} keV in detector: {total_tracks}")
    print(f"Unique EventIDs among them: {unique_events}")

    # Histogram over EventID values
    bins = min(200, max(10, int(np.sqrt(unique_events))))
    fig, ax = plt.subplots(figsize=(10, 5))
    ax.hist(gamma_df["EventID"], bins=bins, color="tab:purple", edgecolor="black", alpha=0.7)
    ax.set_xlabel("EventID")
    ax.set_ylabel("Count (unique gamma tracks)")
    ax.set_title(f"EventID distribution for gamma {TARGET_ENERGY_KEV:.5f} keV in {DETECTOR_VOLUME}")
    fig.tight_layout()

    out_path = Path(__file__).resolve().parent / "gamma_eventid_hist.png"
    fig.savefig(out_path)
    print(f"Histogram saved to {out_path}")


if __name__ == "__main__":
    main()
