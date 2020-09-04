#!/bin/bash
#module load gcc/6.1.0
module load openmpi/1.10.2-gnu-pmi2
#redirect standard out to hello_world_MPI_gnu.txt file
srun ./hello_world_MPI_gnu >hello_world_MPI_gnu.txt
