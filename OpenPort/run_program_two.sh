#!/bin/bash
#SBATCH --job-name=prog_two
#SBATCH -e prog_two.%j.err
#SBATCH -o prog_two.%j.out
#SBATCH --time=0:02:0
#SBATCH --nodes=1
#SBATCH --tasks-per-node=128
#SBATCH --cpus-per-task=1
#SBATCH --ntasks-per-core=1
#SBATCH --partition=standard
#SBATCH --qos=short

srun --cpu-bind=cores ./test_program_two

