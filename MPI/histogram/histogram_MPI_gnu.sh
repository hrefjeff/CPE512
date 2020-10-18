#!/bin/bash
module load openmpi/1.10.2-gnu-pmi2
srun ./histogram_MPI 100000 100 100.0 0.0 N
   # to take the histogram on 100000 numbers with 100 bins with
   # histogram data being taken for numbers between 100 and 0
   # and suppress numbers array output from printing
