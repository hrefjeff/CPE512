set output "heat_2d_sync_sp_jetson.png"
set term png
set title "Speedup, 2D Heat Transfer, Jetson Cluster, (Synchronus Comm)"
set grid
set xrange[32:10000]
set logscale x 2
set xlabel "n, Active Data Points in X and Y Dimension";
set ylabel "Relative Speedup";
set key left top;
plot "heat_2d_sync_MPI_1_sp.txt" with linespoints t "1 MPI Procs","heat_2d_sync_MPI_2_sp.txt" with linespoints t "2 MPI Procs","heat_2d_sync_MPI_4_sp.txt" with linespoints t "4 MPI Procs", "heat_2d_sync_MPI_8_sp.txt" with linespoints t "8 MPI Procs","heat_2d_sync_MPI_16_sp.txt" with linespoints t "16 MPI Procs";

