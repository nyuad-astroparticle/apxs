#!/bin/bash
# Number of tasks
#SBATCH --ntasks 2048
# Set Memory
#SBATCH --mem 64GB
# Set WALLTIME
#SBATCH --time 12:00:00
# Set output and error files
#SBATCH -o start-job.%J.out
#SBATCH -e start-job.%J.err
# Send email when done
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=ss14729@nyu.edu

#----------------------------------#
#------------EXECUTION-------------#
#----------------------------------#

# Load modules
module purge
source load_modules.sh

# Run the application
cd /scratch/ss14729/apxs/build1/
start_time=`date +%s`
# mpiexec -n 2048 ./apxs -g ./geometry/new_geom/output.gdml -m ./macros/test_new_geom.mac > /dev/null 2>&1 [this is when you don't want to print anything on .out or .err files]
mpiexec -n 2048 ./apxs -g ./geometry/lxs_dwarf_calib/out_lxsdwarfcalib.gdml -m ./macros/test_new_geom.mac 
end_time=`date +%s`
echo execution time was `expr $end_time - $start_time` s.
echo "SIMULATION COMPLETED!"

# echo "MERGING!"
# ../hpc/merge.sh ./output > ./output/merge.csv

