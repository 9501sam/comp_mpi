#!/bin/bash
#SBATCH --job-name=Mandelbort
#SBATCH --ntasks-per-node=<# of processes>
#SBATCH --cpus-per-task=<# of threads per process>
#SBATCH --time=00:03:00
#SBATCH --account=ACD113087
#SBATCH --partition=ctest

module load gcc/12.3.0
module load openmpi/4.1.6

srun ./mandelbrot <testcase/X.txt> <output.png>
