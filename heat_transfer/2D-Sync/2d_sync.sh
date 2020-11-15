#!/bin/bash
#module load gcc/6.1.0_all
module load openmpi/1.10.2-gnu-pmi2
srun ./2dsync 10 100 
# execute a 10000 x 10000 point 2d-heat transfer problem 
# for 5 iternations and suppress its output 
