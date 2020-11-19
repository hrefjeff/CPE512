/*
OpenMP Example - Hello World - C/C++ Version
FILE: hello_world_OMP.cpp

Uses Environment variable OMP_NUM_THREADS as the number of threads
that are created in parallel sections 

Compilation on dmc.asc.edu

   to compile the program 
     
     Using GNU Compiler
     module load gcc
     g++ -fopenmp hello_world_OMP.cpp -o hello_world_OMP_gnu

     Using Intel Compiler
     module load intel
     icpc -qopenmp hello_world_OMP.cpp -o hello_world_OMP_intel

     Using NVIDIA hpc Compiler
     module load pgi/18.1
     pgc++ -mp hello_world_OMP.cpp -o hello_world_OMP_hpc

   to run on eight cores follow instructions presented in the handout "Invoking
   the Job Queuing System on the dmc.asc.edu system using the run_script_omp
   utility for OpenMP programs"

*/

using namespace std;
#include <iostream>
#include <stdlib.h>
#include <omp.h>

int main (int argc, char *argv[]) {


   // thread public (shared) variable nthreads;
   int nthreads;

   // How to set the amount of threads at runtime
   int p = atoi(argv[1]);
   omp_set_num_threads(p);

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
         cout << "Hello World from thread = " << tid << endl <<flush;
      }
      // Thread 0 determines number of threads in group and stores the
      // result in the shared variable, nthreads 
      if (tid == 0) {
         nthreads = omp_get_num_threads();
      }
   }  // All threads join master thread and disband
 
   // output Number of Threads Message from the main thread
   cout << "Number of Threads = "  << nthreads << endl
      << "Program compiled using "
      #ifdef __PGI
          << "NVIDA HPC compiler"
      #elif __INTEL_COMPILER
          << "Intel compiler"
      #elif __GNUC__
          << "GNU compiler"
      #endif
      << endl << flush;

}

