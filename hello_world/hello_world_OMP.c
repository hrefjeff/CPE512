/*
openMP Example - Hello World - C Version
FILE: hello_world_OMP.c

Uses Environment variable OMP_NUM_THREADS as the number of threads
that are created in parallel sections 

Compilation on dmc.asc.edu

   to compile the program
     
     Using GNU Compiler
     module load gcc
     gcc -fopenmp hello_world_OMP.c -o hello_world_OMP_gnu

     Using Intel Compiler
     module load intel
     icc -qopenmp hello_world_OMP.c -o hello_world_OMP_intel

     Using NVIDIA hpc Compiler
     module load pgi/18.1
     pgcc -mp hello_world_OMP.c -o hello_world_OMP_hpc

   to run on eight cores follow instructions presented in the handout "Invoking
   the Job Queuing System on the dmc.asc.edu system using the run_script_omp
   utility for OpenMP programs"

*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main (int argc, char *argv[]) {


   // thread public (shared) variable nthreads;
   int nthreads;

   // Fork a team of threads giving them their own
   // copy of private variable, tid, thread id
   #pragma omp parallel 
   {
      int tid; // private (local) variable thread id, tid
               // declared within the basic block

      // Obtain thread number
      tid = omp_get_thread_num();

      // print out Hello World Message -- do this as a noninterruptable op
      #pragma omp critical
      {
         printf("Hello World from thread = %d\n", tid);
         fflush(stdout);
      }
      // Thread 0 determines number of threads in group and stores the
      // result in the shared variable, nthreads 
      if (tid == 0) {
         nthreads = omp_get_num_threads();
      }
   }  // All threads join master thread and disband
 
   // output Number of Threads Message from the main thread
   printf("Number of Threads = %d\n", nthreads);
   printf("Program compiled using ");
      #ifdef __PGI
          printf("NVIDA HPC compiler\n");
      #elif __INTEL_COMPILER
          printf("Intel compiler\n");
      #elif __GNUC__
          printf("GNU compiler\n");
      #endif
      fflush(stdout);

}

