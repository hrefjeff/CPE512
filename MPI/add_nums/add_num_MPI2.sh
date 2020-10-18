#!/bin/bash
module load openmpi/1.10.2-gnu-pmi2
srun ./add_num_MPI2 2948 # where 2948 is the number of
                         # numbers you want to add
