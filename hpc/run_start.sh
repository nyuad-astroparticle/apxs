#!/bin/bash
# Number of tasks
#SBATCH --ntasks 4096
# Set Memory
#SBATCH --mem 64GB
# Set WALLTIME
#SBATCH --time 10:00:00
# Set output and error files
#SBATCH -o start-job.%J.out
#SBATCH -e start-job.%J.err
# Send email when done
#SBATCH --mail-type=END,FAIL,BEGIN
#SBATCH --mail-user=ss14729@nyu.edu

#----------------------------------#
#------------EXECUTION-------------#
#----------------------------------#

# Load modules
module purge
source load_modules.sh
# source /scratch/ss14729/geant4/install/bin/geant4.sh
# module load gcc cmake mpich expat

# Run the application
cd /scratch/ss14729/apxs/build6/
start_time=`date +%s`
mpiexec -n 4096 ./apxs -g ./geometry/new_geom/output.gdml -m ./macros/test_new_geom.mac
# mpiexec -n 10 ./apxs -g ./geometry/new_geom/output.gdml -m ./macros/test_new_geom.mac
end_time=`date +%s`
echo execution time was `expr $end_time - $start_time` s.
echo "SIMULATION COMPLETED!"

# echo "MERGING!"
# ../hpc/merge.sh ./output > ./output/merge.csv

