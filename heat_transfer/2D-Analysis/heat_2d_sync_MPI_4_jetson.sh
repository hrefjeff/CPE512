#!/bin/bash
# UAH Jetson Cluster

#
# 4 MPI Process Case
# execute a n x n point 2d-heat transfer problem sweeping n from
# 32 to 32768 geometrically by a factor of 2 for each run and
# executing for 50 iterations and suppressing actual data output
# but recording the execution time of each run in the file
# "heat_2d_sync_MPI_4_tm.txt". The data is in x y format, where 
# x is the value of n and y is the execution time in seconds
srun ./heat_2d_sync_MPI    32 50 G >  heat_2d_sync_MPI_4_tm.txt
srun ./heat_2d_sync_MPI    64 50 G >> heat_2d_sync_MPI_4_tm.txt
srun ./heat_2d_sync_MPI   128 50 G >> heat_2d_sync_MPI_4_tm.txt
srun ./heat_2d_sync_MPI   256 50 G >> heat_2d_sync_MPI_4_tm.txt
srun ./heat_2d_sync_MPI   512 50 G >> heat_2d_sync_MPI_4_tm.txt
srun ./heat_2d_sync_MPI  1024 50 G >> heat_2d_sync_MPI_4_tm.txt
srun ./heat_2d_sync_MPI  2048 50 G >> heat_2d_sync_MPI_4_tm.txt
srun ./heat_2d_sync_MPI  4096 50 G >> heat_2d_sync_MPI_4_tm.txt
srun ./heat_2d_sync_MPI  8192 50 G >> heat_2d_sync_MPI_4_tm.txt
srun ./heat_2d_sync_MPI 10000 50 G >> heat_2d_sync_MPI_4_tm.txt
