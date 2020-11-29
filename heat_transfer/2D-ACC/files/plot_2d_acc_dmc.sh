#!/bin/bash
gnuplot ./heat_2d_acc_tm_dmc.gnu
gnuplot ./heat_2d_acc_tm2_dmc.gnu
#
/home/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_acc_kepler_tm.txt  > heat_2d_acc_kepler_sp.txt
/home/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_acc_pascal_tm.txt  > heat_2d_acc_pascal_sp.txt
/home/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_acc_volta_tm.txt  > heat_2d_acc_volta_sp.txt
gnuplot ./heat_2d_acc_sp_dmc.gnu

