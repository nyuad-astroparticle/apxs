#!/bin/bash
# Number of tasks 
#SBATCH --ntasks 2000 
# Set Memory
#SBATCH --mem 64GB
# Set WALLTIME
#SBATCH --time 15:00:00
# Set output and error files
#SBATCH -o start-job.%J.out
#SBATCH -e start-job.%J.err
# Send email when done
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=ibs257@nyu.edu

#----------------------------------#
#------------EXECUTION-------------#
#----------------------------------#

# Load modules
module purge
source load_modules.sh

# Run the application
cd /scratch/ibs257/apxs/build/
start_time=`date +%s`
mpiexec -n 3000 ./apxs ./macros/ferroannoriticanorthosite.mac
end_time=`date +%s`
echo execution time was `expr $end_time - $start_time` s.
echo "SIM IS DONE!"
# echo "MERGING!"
# ../hpc/merge.sh ./output > ./output/merge.csv
