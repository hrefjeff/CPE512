#!/bin/bash
# script file to plot execution time and the other derived performance metrics
# of speedup, efficiency, and cost using gnuplot
# After ALL jobs have successfully executed this script can be applied
# Produces .png files which can be downloaded and viewed on client systems

# plot execution time
gnuplot ./heat_2d_sync_tm_jetson.gnu

#plot speedup using serial execution time as a reference
/cpe412/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_1_tm.txt  > heat_2d_sync_MPI_1_sp.txt
/cpe412/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_2_tm.txt  > heat_2d_sync_MPI_2_sp.txt
/cpe412/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_4_tm.txt  > heat_2d_sync_MPI_4_sp.txt
/cpe412/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_8_tm.txt  > heat_2d_sync_MPI_8_sp.txt
/cpe412/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_16_tm.txt > heat_2d_sync_MPI_16_sp.txt
gnuplot ./heat_2d_sync_sp_jetson.gnu

#plot efficiency using serial execution time as a reference and the specified number of processing cores
/cpe412/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_1_tm.txt 1  > heat_2d_sync_MPI_1_ef.txt
/cpe412/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_2_tm.txt 2 > heat_2d_sync_MPI_2_ef.txt
/cpe412/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_4_tm.txt 4 > heat_2d_sync_MPI_4_ef.txt
/cpe412/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_8_tm.txt 8 > heat_2d_sync_MPI_8_ef.txt
/cpe412/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_16_tm.txt 16 > heat_2d_sync_MPI_16_ef.txt
gnuplot ./heat_2d_sync_ef_jetson.gnu

#plot cost based on the parallel execution time and the specified number of processing cores
/cpe412/.bin/gnu_cost heat_2d_sync_MPI_1_tm.txt  1 >  heat_2d_sync_MPI_1_ct.txt
/cpe412/.bin/gnu_cost heat_2d_sync_MPI_2_tm.txt  2 >  heat_2d_sync_MPI_2_ct.txt
/cpe412/.bin/gnu_cost heat_2d_sync_MPI_4_tm.txt  4 >  heat_2d_sync_MPI_4_ct.txt
/cpe412/.bin/gnu_cost heat_2d_sync_MPI_8_tm.txt  8 >  heat_2d_sync_MPI_8_ct.txt
/cpe412/.bin/gnu_cost heat_2d_sync_MPI_16_tm.txt 16 > heat_2d_sync_MPI_16_ct.txt
gnuplot ./heat_2d_sync_cost_jetson.gnu

