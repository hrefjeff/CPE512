#!/bin/bash
module load pgi
./heat_2d_serial 32    75 G  > heat_2d_serial_tm.txt 
./heat_2d_serial 64    75 G >> heat_2d_serial_tm.txt
./heat_2d_serial 128   75 G >> heat_2d_serial_tm.txt
./heat_2d_serial 256   75 G >> heat_2d_serial_tm.txt
./heat_2d_serial 512   75 G >> heat_2d_serial_tm.txt
./heat_2d_serial 1024  75 G >> heat_2d_serial_tm.txt
./heat_2d_serial 2048  75 G >> heat_2d_serial_tm.txt
./heat_2d_serial 4096  75 G >> heat_2d_serial_tm.txt
./heat_2d_serial 8192  75 G >> heat_2d_serial_tm.txt
./heat_2d_serial 12288 75 G >> heat_2d_serial_tm.txt
./heat_2d_serial 16384 75 G >> heat_2d_serial_tm.txt
./heat_2d_serial 30000 75 G >> heat_2d_serial_tm.txt
./heat_2d_serial 32768 75 G >> heat_2d_serial_tm.txt
