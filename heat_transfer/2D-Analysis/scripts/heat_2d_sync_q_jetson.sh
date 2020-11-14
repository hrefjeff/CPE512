#!/bin/bash
#
# Serial Reference Run
# Number of Nodes: 1 
echo -e "Scheduling Serial Job on Jetson Cluster\n"
echo -e "1\n\n" | run_script_mpi heat_2d_serial_jetson.sh >heat_2d_serial_tm.txt
#
# 1 MPI Process Run
# Number of Nodes: 1 
echo -e "Scheduling 1 MPI Process Job on Jetson Cluster\n"
echo -e "1\n\n" | run_script_mpi heat_2d_sync_MPI_1_jetson.sh >heat_2d_sync_MPI_1_tm.txt
#
# 2 MPI Process Run
# Number of Nodes: 2 
echo -e "Scheduling 2 MPI Process Job on Jetson Cluster\n"
# DMC Node Architecture Constraint: first argument
echo -e "2\n\n" | run_script_mpi heat_2d_sync_MPI_2_jetson.sh >heat_2d_sync_MPI_2_tm.txt
#
# 4 MPI Process Run
# Number of Nodes: 4 
echo -e "Scheduling 4 MPI Process Job on Jetson Cluster\n"
echo -e "4\n\n" | run_script_mpi heat_2d_sync_MPI_4_jetson.sh >heat_2d_sync_MPI_4_tm.txt
#
# 8 MPI Process Run
# Number of Nodes: 8 
echo -e "Scheduling 8 MPI Process Job on Jetson Cluster\n"
echo -e "8\n\n" | run_script_mpi heat_2d_sync_MPI_8_jetson.sh >heat_2d_sync_MPI_8_tm.txt
#
# 16 MPI Process Run
# Number of Nodes: 16 
echo -e "Scheduling 16 MPI Process Job on Jetson Cluster\n"
echo -e "16\n\n" | run_script_mpi heat_2d_sync_MPI_16_jetson.sh >heat_2d_sync_MPI_16_tm.txt

