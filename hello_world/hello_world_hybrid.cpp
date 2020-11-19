/*
Jeffrey Allen
CS512

MPI / OpenMP Hybrid Example - Hello World - C++ Version
FILE: hello_world_hybrid.cpp

Uses Environment variable OMP_NUM_THREADS as the number of threads
that are created in parallel sections

Compilation on jetson
     
    === Using GNU Compiler ===
    module load gcc
    module load openmpi/1.10.2-gnu-pmi2
    mpic++ -fopenmp hello_world_hybrid.cpp -o hello_hybrid -fopenmp

    touch hello_hybrid.sh

    === Using Intel Compiler ===
    module load intel
    icpc -qopenmp hello_world_OMP.cpp -o hello_world_OMP_intel

    === Using NVIDIA hpc Compiler ===
    module load pgi/18.1
    pgc++ -mp hello_world_OMP.cpp -o hello_world_OMP_hpc


    run_script_hybrid hello_hybrid.sh

*/

using namespace std;
#include <iostream>
#include <omp.h>
#include <mpi.h>

int main (int argc, char *argv[]) {
    MPI_Status status;
    int nmtsks, rank;

    MPI_Init(&argc,&argv); // Initalize MPI environment
    MPI_Comm_size(MPI_COMM_WORLD,&nmtsks); //get total number of processes
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); // get process identity number

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
            cout << "Hello World from MPI process" << rank << " Thread " << tid << endl;
        }
        // Thread 0 determines number of threads in group and stores the
        // result in the shared variable, nthreads 
        if (tid == 0) {
            nthreads = omp_get_num_threads();
        }
    
    }  // All threads join master thread and disband

    // wait for processes to synchronize
    MPI_Barrier(MPI_COMM_WORLD);

    /* Only root MPI process does this */
    if (rank == 0) {
        cout << "Number of MPI Processes = " << nmtsks << ", # of OpenMP Threads" << nthreads <<endl
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