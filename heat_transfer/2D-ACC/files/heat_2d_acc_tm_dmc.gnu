set output "heat_2d_acc_tm_dmc.png"
set term png
set title "Execution Time, 2D Heat Transfer -- DMC Cluster, OpenACC"
set grid
set xrange [32:32768]
set logscale x 2
set xlabel "n, Active Data Points in X and Y Dimension";
set ylabel "Execution Time in Seconds";
set key center top;
plot "heat_2d_serial_tm.txt" with linespoints t "Serial Ref (Intel Skylake Arch)","heat_2d_acc_kepler_tm.txt" with linespoints t "NVIDIA Kepler GPU","heat_2d_acc_pascal_tm.txt" with linespoints t "NVIDIA Pascal GPU","heat_2d_acc_volta_tm.txt" with linespoints t "NVIDIA Volta GPU";
