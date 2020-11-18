#!/bin/bash
module load pgi/18.1
./heat_2d_openmp 4 10000 5 S

# arg 1 = # of processes
# arg 2 = # of points 2d-heat transfer problem 
# arg 3 = # of iterations
# arg 4 (optional) = S for optional suppresion
