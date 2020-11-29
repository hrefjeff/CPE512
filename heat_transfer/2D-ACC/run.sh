#!/bin/bash
./heat_2d_acc 30000 75 C >heat_2d_acc_out.txt
./heat_2d_acc 30000 75 S >heat_2d_acc_out.txt

# don't forgot toe "chmod 744 run.sh" this file
# execute using: run_gpu run.sh
# script runs
# Enter Queue Name (default <cr>: gpu) class