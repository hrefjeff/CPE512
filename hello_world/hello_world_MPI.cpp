/* 
   MPI Example - Hello World - (utilizing C functional bindings)
   FILE: hello_world_MPI.cpp

   Compilation on dmc.asc.edu

      GNU Compiler
      module load openmpi/1.10.2-gnu-pmi2
      mpic++ hello_world_MPI.cpp -o hello_world_MPI_gnu
            or
      Intel Compiler
      module load openmpi/1.10.2-intel-pmi2
      mpic++ hello_world_MPI.cpp -o hello_world_MPI_intel

   to run on eight cores follow instructions presented in the handout "Invoking
   the Job Queuing System on the dmc.asc.edu system using run_script_mpi utility
   for MPI programs"

*/

using namespace std;
#include <iostream>
#include <mpi.h>

int main (int argc, char *argv[]) {
   MPI_Status status;
   int nmtsks, rank;

   MPI_Init(&argc,&argv); // Initalize MPI environment
   MPI_Comm_size(MPI_COMM_WORLD,&nmtsks); //get total number of processes
   MPI_Comm_rank(MPI_COMM_WORLD,&rank); // get process identity number

   cout << "Hello World from MPI Process #" << rank << endl << flush;

   // wait for processes to synchronize
   MPI_Barrier(MPI_COMM_WORLD);

   /* Only root MPI process does this */
   if (rank == 0) {
      cout << "Number of MPI Processes = " << nmtsks << endl
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
  /* Terminate MPI Program -- clear out all buffers */
  MPI_Finalize();

}
