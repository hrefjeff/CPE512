/*
   MPI Example - Hello World - C++ Version
   using the C++ bindings -- Only fully supported in MPI 1 for openmpi
   FILE: hello_world_MPI2.cpp

   Compilation on dmc.asc.edu

      GNU Compiler
      module load openmpi/1.10.2-gnu-pmi2
      mpic++ hello_world_MPI2.cpp -o hello_world_MPI_gnu
            or
      Intel Compiler
      module load openmpi/1.10.2-intel-pmi2
      mpic++ hello_world_MPI2.cpp -o hello_world_MPI_gnu_intel

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

   MPI::Init(argc,argv); // Initalize MPI environment

   nmtsks=MPI::COMM_WORLD.Get_size(); //get total number of processes

   rank= MPI::COMM_WORLD.Get_rank(); // get process identity number

   cout << "Hello World from MPI Process #" << rank << endl << flush;

   // wait for processes to synchronize
   MPI::COMM_WORLD.Barrier();

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
  MPI::Finalize();

}

