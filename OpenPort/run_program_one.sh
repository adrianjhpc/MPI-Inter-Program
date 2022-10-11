#!/bin/bash
#SBATCH --job-name=prog_one
#SBATCH -o prog_one.%j.out
#SBATCH -e prog_one.%j.err
#SBATCH --time=0:02:0
#SBATCH --nodes=1
#SBATCH --tasks-per-node=128
#SBATCH --cpus-per-task=1
#SBATCH --ntasks-per-core=1
#SBATCH --partition=standard
#SBATCH --qos=short

srun --cpu-bind=cores ./test_program_one

