#!/bin/bash
module load openmpi/1.10.2-intel-pmi2
#redirect standard out to hello_world_MPI_gnu.txt file
srun ./hello_world_MPI_intel >hello_world_MPI_intel.txt
