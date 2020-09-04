/* 
   MPI Example - Hello World - (C version)
   FILE: hello_world_MPI.c

   Compilation on dmc.asc.edu

      GNU Compiler
      module load openmpi/1.10.2-gnu-pmi2
      mpicc hello_world_MPI.c -o hello_world_MPI_gnu
            or
      Intel Compiler
      module load openmpi/1.10.2-intel-pmi2
      mpicc hello_world_MPI.c -o hello_world_MPI_intel

   to run on eight cores follow instructions presented in the handout "Invoking
   the Job Queuing System on the dmc.asc.edu system using run_script_mpi utility
   for MPI programs"

*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {

   MPI_Status status;
   int nmtsks, rank;

   MPI_Init(&argc,&argv); // Initalize MPI environment
   MPI_Comm_size(MPI_COMM_WORLD,&nmtsks); //get total number of processes
   MPI_Comm_rank(MPI_COMM_WORLD,&rank); // get process identity number

   printf("Hello World from MPI Process #%d\n",rank);
   fflush(stdout);

   // wait for processes to synchronize
   MPI_Barrier(MPI_COMM_WORLD);

   /* Only root MPI process does this */
   if (rank == 0) {
      printf("Number of MPI Processes = %d\n",nmtsks);
      printf("Program compiled using the ");
      #ifdef __PGI
         // Compiled with the NVIDA HPC Compiler
         printf("NVIDA HPC compiler\n");
      #elif  __INTEL_COMPILER
         // Compiled with Intel Compiler
         printf("Intel compiler\n");
      #elif __GNUC__
         // Compiled with GNU Compiler
         printf("GNU compiler\n");
      #endif
      fflush(stdout);
   }

  /* Terminate MPI Program -- clear out all buffers */
  MPI_Finalize();

}
