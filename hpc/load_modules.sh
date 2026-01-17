#!/bin/bash
# A little script to load the required modules for compiling this beautiful simulation

# HPC Modules -----------------------------------------------
module load cmake
module load gcc
module load mpich
module load expat

# Geant4 ----------------------------------------------------
source /scratch/ss14729/geant4/install/bin/geant4.sh
export G4MPI_DIR="/scratch/ss14729/geant4/g4mpi/lib64/G4mpi-11.3.2/"

export XercesC_DIR="/scratch/ss14729/geant4/xerces-c-3.3.0/install/"
export CMAKE_PREFIX_PATH="$CMAKE_PREFIX_PATH:$XercesC_DIR"

# Now you should be all set to compile with the ENABLE_MPI flag on ccmake.
