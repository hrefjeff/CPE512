#!/bin/bash
# ASC DMC
module load gcc/6.1.0_all
# 1 Process Reference Serial Simulation
# execute a n x n point 2d-heat transfer problem
# sweeping n from 4000 to 40,000 in steps of 4000
# for 50 iternations and suppress its output but
# record its execution time in gnuplot format
# in heat_serial.txt file
./heat_2d_serial 4000 50 G >  heat_serial.txt
./heat_2d_serial 8000 50 G >> heat_serial.txt
./heat_2d_serial 12000 50 G >> heat_serial.txt
./heat_2d_serial 16000 50 G >> heat_serial.txt
./heat_2d_serial 20000 50 G >> heat_serial.txt
./heat_2d_serial 24000 50 G >> heat_serial.txt
./heat_2d_serial 28000 50 G >> heat_serial.txt
./heat_2d_serial 32000 50 G >> heat_serial.txt
./heat_2d_serial 36000 50 G >> heat_serial.txt
./heat_2d_serial 40000 50 G >> heat_serial.txt
