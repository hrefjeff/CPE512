#!/bin/bash
module load gcc/6.1.0_all
./heat_2d_serial 10 100 
# execute a 10000 x 10000 point 2d-heat transfer problem 
# for 5 iternations and suppress its output 
