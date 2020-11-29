set output "heat_2d_acc_sp_dmc.png"
set term png
set title "Speedup, 2D Heat Transfer, DMC Cluster, OpenACC"
set grid
set xrange [32:32768]
set logscale x 2
set xlabel "n, Active Data Points in X and Y Dimension";
set ylabel "Speedup";
set key left top;
plot "heat_2d_acc_kepler_sp.txt" with linespoints t "NVIDIA kepler GPU","heat_2d_acc_pascal_sp.txt" with linespoints t "NVIDIA Pascal GPU","heat_2d_acc_volta_sp.txt" with linespoints t "NVIDIA Volta GPU";

