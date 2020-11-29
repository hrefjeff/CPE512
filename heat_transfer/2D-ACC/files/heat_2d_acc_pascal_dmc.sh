#!/bin/bash
module load pgi
./heat_2d_acc 32    75 G  > heat_2d_acc_pascal_tm.txt 
./heat_2d_acc 64    75 G >> heat_2d_acc_pascal_tm.txt
./heat_2d_acc 128   75 G >> heat_2d_acc_pascal_tm.txt
./heat_2d_acc 256   75 G >> heat_2d_acc_pascal_tm.txt
./heat_2d_acc 512   75 G >> heat_2d_acc_pascal_tm.txt
./heat_2d_acc 1024  75 G >> heat_2d_acc_pascal_tm.txt
./heat_2d_acc 2048  75 G >> heat_2d_acc_pascal_tm.txt
./heat_2d_acc 4096  75 G >> heat_2d_acc_pascal_tm.txt
./heat_2d_acc 8192  75 G >> heat_2d_acc_pascal_tm.txt
./heat_2d_acc 12288 75 G >> heat_2d_acc_pascal_tm.txt
./heat_2d_acc 16384 75 G >> heat_2d_acc_pascal_tm.txt
./heat_2d_acc 30000 75 G >> heat_2d_acc_pascal_tm.txt
./heat_2d_acc 32768 75 G >> heat_2d_acc_pascal_tm.txt
