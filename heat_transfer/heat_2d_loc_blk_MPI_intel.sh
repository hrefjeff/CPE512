#!/bin/bash
# ASC DMC
module load openmpi/1.10.2-intel-pmi2
# 1 Process Reference Serial Simulation
# execute a n x n point 2d-heat transfer problem
# sweeping n from 4000 to 40,000 in steps of 4000
# for 50 iternations and suppress its output but
# record its execution time in gnuplot format 
# in heat_serial_tm.txt file
./heat_2d_serial 4000 50 G >  heat_serial_tm.txt
./heat_2d_serial 8000 50 G >> heat_serial_tm.txt
./heat_2d_serial 12000 50 G >> heat_serial_tm.txt
./heat_2d_serial 16000 50 G >> heat_serial_tm.txt
./heat_2d_serial 20000 50 G >> heat_serial_tm.txt
./heat_2d_serial 24000 50 G >> heat_serial_tm.txt
./heat_2d_serial 28000 50 G >> heat_serial_tm.txt
./heat_2d_serial 32000 50 G >> heat_serial_tm.txt
./heat_2d_serial 36000 50 G >> heat_serial_tm.txt
./heat_2d_serial 40000 50 G >> heat_serial_tm.txt
#
# 1 MPI Process Case
# execute a n x n point 2d-heat transfer problem
# sweeping n from 4000 to 40,000 in steps of 4000
# for 50 iternations and suppress its output but
# record its execution time in gnuplot format 
# in heat_loc_blk_MPI_1_tm.txt file
srun -n 1 ./heat_2d_loc_blk_MPI 4000 50 G >  heat_loc_blk_MPI_1_tm.txt
srun -n 1 ./heat_2d_loc_blk_MPI 8000 50 G >> heat_loc_blk_MPI_1_tm.txt
srun -n 1 ./heat_2d_loc_blk_MPI 12000 50 G >> heat_loc_blk_MPI_1_tm.txt
srun -n 1 ./heat_2d_loc_blk_MPI 16000 50 G >> heat_loc_blk_MPI_1_tm.txt
srun -n 1 ./heat_2d_loc_blk_MPI 20000 50 G >> heat_loc_blk_MPI_1_tm.txt
srun -n 1 ./heat_2d_loc_blk_MPI 24000 50 G >> heat_loc_blk_MPI_1_tm.txt
srun -n 1 ./heat_2d_loc_blk_MPI 28000 50 G >> heat_loc_blk_MPI_1_tm.txt
srun -n 1 ./heat_2d_loc_blk_MPI 32000 50 G >> heat_loc_blk_MPI_1_tm.txt
srun -n 1 ./heat_2d_loc_blk_MPI 36000 50 G >> heat_loc_blk_MPI_1_tm.txt
srun -n 1 ./heat_2d_loc_blk_MPI 40000 50 G >> heat_loc_blk_MPI_1_tm.txt
#
#
# 2 MPI Process Case
# execute a n x n point 2d-heat transfer problem
# sweeping n from 4000 to 40,000 in steps of 4000
# for 50 iternations and suppress its output but
# record its execution time in gnuplot format 
# in heat_loc_blk_MPI_2_tm.txt file
srun -n 2 ./heat_2d_loc_blk_MPI 4000 50 G >  heat_loc_blk_MPI_2_tm.txt
srun -n 2 ./heat_2d_loc_blk_MPI 8000 50 G >> heat_loc_blk_MPI_2_tm.txt
srun -n 2 ./heat_2d_loc_blk_MPI 12000 50 G >> heat_loc_blk_MPI_2_tm.txt
srun -n 2 ./heat_2d_loc_blk_MPI 16000 50 G >> heat_loc_blk_MPI_2_tm.txt
srun -n 2 ./heat_2d_loc_blk_MPI 20000 50 G >> heat_loc_blk_MPI_2_tm.txt
srun -n 2 ./heat_2d_loc_blk_MPI 24000 50 G >> heat_loc_blk_MPI_2_tm.txt
srun -n 2 ./heat_2d_loc_blk_MPI 28000 50 G >> heat_loc_blk_MPI_2_tm.txt
srun -n 2 ./heat_2d_loc_blk_MPI 32000 50 G >> heat_loc_blk_MPI_2_tm.txt
srun -n 2 ./heat_2d_loc_blk_MPI 36000 50 G >> heat_loc_blk_MPI_2_tm.txt
srun -n 2 ./heat_2d_loc_blk_MPI 40000 50 G >> heat_loc_blk_MPI_2_tm.txt
#
# 4 MPI Process Case
# execute a n x n point 2d-heat transfer problem
# sweeping n from 4000 to 40,000 in steps of 4000
# for 50 iternations and suppress its output but
# record its execution time in gnuplot format 
# in heat_loc_blk_MPI_4_tm.txt file
srun -n 4 ./heat_2d_loc_blk_MPI 4000 50 G >  heat_loc_blk_MPI_4_tm.txt
srun -n 4 ./heat_2d_loc_blk_MPI 8000 50 G >> heat_loc_blk_MPI_4_tm.txt
srun -n 4 ./heat_2d_loc_blk_MPI 12000 50 G >> heat_loc_blk_MPI_4_tm.txt
srun -n 4 ./heat_2d_loc_blk_MPI 16000 50 G >> heat_loc_blk_MPI_4_tm.txt
srun -n 4 ./heat_2d_loc_blk_MPI 20000 50 G >> heat_loc_blk_MPI_4_tm.txt
srun -n 4 ./heat_2d_loc_blk_MPI 24000 50 G >> heat_loc_blk_MPI_4_tm.txt
srun -n 4 ./heat_2d_loc_blk_MPI 28000 50 G >> heat_loc_blk_MPI_4_tm.txt
srun -n 4 ./heat_2d_loc_blk_MPI 32000 50 G >> heat_loc_blk_MPI_4_tm.txt
srun -n 4 ./heat_2d_loc_blk_MPI 36000 50 G >> heat_loc_blk_MPI_4_tm.txt
srun -n 4 ./heat_2d_loc_blk_MPI 40000 50 G >> heat_loc_blk_MPI_4_tm.txt
#
# 8 MPI Process Case
# execute a n x n point 2d-heat transfer problem
# sweeping n from 4000 to 40,000 in steps of 4000
# for 50 iternations and suppress its output but
# record its execution time in gnuplot format 
# in heat_loc_blk_MPI_8_tm.txt file
srun -n 8 ./heat_2d_loc_blk_MPI 4000 50 G >  heat_loc_blk_MPI_8_tm.txt
srun -n 8 ./heat_2d_loc_blk_MPI 8000 50 G >> heat_loc_blk_MPI_8_tm.txt
srun -n 8 ./heat_2d_loc_blk_MPI 12000 50 G >> heat_loc_blk_MPI_8_tm.txt
srun -n 8 ./heat_2d_loc_blk_MPI 16000 50 G >> heat_loc_blk_MPI_8_tm.txt
srun -n 8 ./heat_2d_loc_blk_MPI 20000 50 G >> heat_loc_blk_MPI_8_tm.txt
srun -n 8 ./heat_2d_loc_blk_MPI 24000 50 G >> heat_loc_blk_MPI_8_tm.txt
srun -n 8 ./heat_2d_loc_blk_MPI 28000 50 G >> heat_loc_blk_MPI_8_tm.txt
srun -n 8 ./heat_2d_loc_blk_MPI 32000 50 G >> heat_loc_blk_MPI_8_tm.txt
srun -n 8 ./heat_2d_loc_blk_MPI 36000 50 G >> heat_loc_blk_MPI_8_tm.txt
srun -n 8 ./heat_2d_loc_blk_MPI 40000 50 G >> heat_loc_blk_MPI_8_tm.txt
#
# 16 MPI Process Case
# execute a n x n point 2d-heat transfer problem
# sweeping n from 4000 to 40,000 in steps of 4000
# for 50 iternations and suppress its output but
# record its execution time in gnuplot format 
# in heat_loc_blk_MPI_16_tm.txt file
srun -n 16 ./heat_2d_loc_blk_MPI 4000 50 G >  heat_loc_blk_MPI_16_tm.txt
srun -n 16 ./heat_2d_loc_blk_MPI 8000 50 G >> heat_loc_blk_MPI_16_tm.txt
srun -n 16 ./heat_2d_loc_blk_MPI 12000 50 G >> heat_loc_blk_MPI_16_tm.txt
srun -n 16 ./heat_2d_loc_blk_MPI 16000 50 G >> heat_loc_blk_MPI_16_tm.txt
srun -n 16 ./heat_2d_loc_blk_MPI 20000 50 G >> heat_loc_blk_MPI_16_tm.txt
srun -n 16 ./heat_2d_loc_blk_MPI 24000 50 G >> heat_loc_blk_MPI_16_tm.txt
srun -n 16 ./heat_2d_loc_blk_MPI 28000 50 G >> heat_loc_blk_MPI_16_tm.txt
srun -n 16 ./heat_2d_loc_blk_MPI 32000 50 G >> heat_loc_blk_MPI_16_tm.txt
srun -n 16 ./heat_2d_loc_blk_MPI 36000 50 G >> heat_loc_blk_MPI_16_tm.txt
srun -n 16 ./heat_2d_loc_blk_MPI 40000 50 G >> heat_loc_blk_MPI_16_tm.txt

