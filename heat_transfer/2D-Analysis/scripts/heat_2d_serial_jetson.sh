#!/bin/bash
# ASC DMC

#
# 1 Process Reference Serial Simulation
# execute an n x n point 2d-heat transfer problem sweeping n from
# 32 to 32768 geometrically by a factor of 2 for each run and
# executing for 50 iterations and suppressing actual data output
# but recording the execution time of each run in the file
# "heat_2d_serial_tm.txt". The data is in x y format, where x is
# the value of n and y is the execution time in seconds
./heat_2d_serial    32 50 G >  heat_2d_serial_tm.txt
./heat_2d_serial    64 50 G >> heat_2d_serial_tm.txt
./heat_2d_serial   128 50 G >> heat_2d_serial_tm.txt
./heat_2d_serial   256 50 G >> heat_2d_serial_tm.txt
./heat_2d_serial   512 50 G >> heat_2d_serial_tm.txt
./heat_2d_serial  1024 50 G >> heat_2d_serial_tm.txt
./heat_2d_serial  2048 50 G >> heat_2d_serial_tm.txt
./heat_2d_serial  4096 50 G >> heat_2d_serial_tm.txt
./heat_2d_serial  8192 50 G >> heat_2d_serial_tm.txt
./heat_2d_serial 10000 50 G >> heat_2d_serial_tm.txt
