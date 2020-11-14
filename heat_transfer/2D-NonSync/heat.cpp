// 2-D temperature Example with paired Locally Blocking 
// MPI_Send/MPI_Recv Routines

/*
To compile on dmc.asc.edu
   GNU Compiler
      module load openmpi/1.10.2-gnu-pmi2
      mpic++ heat_2d_loc_blk_MPI.cpp -o heat_2d_loc_blk_MPI -O3

         or

   Intel Compiler
      module load openmpi/1.10.2-intel-pmi2
      mpic++ heat_2d_loc_blk_MPI.cpp -o heat_2d_loc_blk_MPI -O3

To execute on dmc.asc.edu
   GNU Compiler
      run_script heat_2d_loc_blk_MPI_gnu.sh
      where _gnut_2d_loc_blk_MPI.sh is a script file that contains
         #!/bin/bash
         module load openmpi/1.10.2-gnu-pmi2
         srun ./heat_2d_loc_blk_MPI 10000 5 S
         # execute a 10000 x 10000 point 2d-heat transfer problem
         # for 5 iternations and suppress its output
   Intel Compiler
      run_script heat_2d_loc_blk_MPI_intel.sh
      where heat_2d_loc_blk_MPI_intel.sh is a script file that contains
         #!/bin/bash
         module load openmpi/1.10.2-intel-pmi2
         srun ./heat_2d_loc_blk_MPI 10000 5 S
         # execute a 10000 x 10000 point 2d-heat transfer problem
         # for 5 iternations and suppress its output
*/

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <mpi.h>

// Global Constants
const int ROOM_TEMP=20;       // temperature everywhere except the fireplace
const int FIREPLACE_TEMP=100; // temperature at upper/middle boundary

int n;                // number of non boundary condition rows in problem 
int num_iterations;   // number of successive iterations before terminating

int numprocs,rank;   // number of MPI processes and process ID
int up_pr;           // logical up processor   
int down_pr;         // logical down processor

int active_rows_on_proc; // number of non boundary condition/ghost
                         // point rows on MPI process
int total_rows_on_proc;  // total number of rows per MPI process
                          // including boundary condition/ghost
                          // point rows

int total_cols;           // total number of columns including
                          // boundary condition columns

double *temp;             // pointer to MPI process local temperature
                          // array row-ordered storage

// Old style macro to give the illusion of 2D memory
#define Temp(x,y) temp[(x)*total_cols+y] 

// routine to initialize the temperature vector and the temperature at the
// boundary
// initialization occurs separately on each MPI process for the region
// that the processes is assigned as well as additional boundary condition/
// ghost point rows
void init_temp(void) {
    const int fireplace_start = 0.3 * (double) n;
    const int fireplace_end = 0.7 * (double) n;

    // Set leftmost boundary condition on process 0
    for (int row=0;row < total_rows_on_proc; row++) {
        for (int col=0;col<total_cols;col++) {
            if (rank == 0 && row == 0) {
                if (col<=fireplace_start || col > fireplace_end) {
                    Temp(row,col) = ROOM_TEMP; // temp[row*total_cols+col];
                }
                else {
                    Temp(row,col) = FIREPLACE_TEMP;
                }
            }
            else {
                Temp(row,col) = ROOM_TEMP;
            }
        }
    }
}
// main compute/communication region -- allows for temperature diffusion 
// to occur one iteration at a time until the specified number of 
// iterations has been performed
//
// Section 1: Nearest-neighbor ghost point reconciliation
//
// Section 2: Local Computation Phase -- stenciled computation
//                                       using ghost points
// 
void compute_temp() {
    MPI_Status status;
    MPI_Request req1,req2;
    #define Temp_buf(x,y) temp_buf[(x)*total_cols+y] // *(temp_buf+x*total_cols+y)
    double *temp_buf = new double[total_rows_on_proc*total_cols];


    // communication phase using Blocking Receives

    // to be replaced with other communication methods in assignment
    // Begin of communication phase
    for (int i=0;i<num_iterations;i++) {

        int r1 = 0;
        int r2 = 0;
        
        if ( rank < numprocs-1 ) {
            MPI_Isend(&temp[active_rows_on_proc*total_cols],total_cols,MPI_DOUBLE,down_pr,123,MPI_COMM_WORLD,&req1);
            r1 = 1;
        }

        if ( rank != 0 ) {
            MPI_Isend(&temp[total_cols],total_cols,MPI_DOUBLE,up_pr,123,MPI_COMM_WORLD,&req2);
            r2 = 1;
        }

        // Receiving and putting into ghost points
        if ( rank < numprocs-1 ){
            MPI_Recv(&temp[(active_rows_on_proc+1)*total_cols],total_cols,MPI_DOUBLE,down_pr,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        } 

        if ( rank != 0 ) {
            MPI_Recv(&temp[0],total_cols,MPI_DOUBLE,up_pr,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        }

        if (r1 == 1) MPI_Wait(&req1, &status);
        if (r2 == 1) MPI_Wait(&req2, &status);

        // End of communication phase

        // local stenciled computation phase 
        for (int j=1;j<=active_rows_on_proc;j++) {
            for (int k=1;k<=total_cols-2;k++) {
                Temp_buf(j,k)=0.25*(Temp(j-1,k)+Temp(j+1,k)+
                        Temp(j,k-1)+Temp(j,k+1));
            }
        }
        for (int j=1;j<=active_rows_on_proc;j++) {
            for (int k=1;k<=total_cols-2;k++) { 
                Temp (j,k)=Temp_buf(j,k);
            }
        }
    }
    delete temp_buf;
}
// routine to display temperature values at each point including the 
// boundary points
void print_temp(void) {
    char flg;
    MPI_Status status;

    // wait for turn to print out local temp array 
    if (rank!=0) {
        // if not rank 0 wait until adjacent left process has completed
        MPI_Recv(&flg,1,MPI_CHAR,up_pr,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
    }
    else {
        // if rank 0 then go ahead and print header and the upper
        // row of boundary points
        std::cout << "Temperature Matrix Including Boundary Points" << std::endl;
        for (int col=0;col<total_cols;col++) {
            std::cout << std::setw(5) << Temp(0,col) << " ";
        }
        std::cout << std::endl << std::flush;
    }

    // when it is your turn print out all points that this process has computed.
    for (int row=1;row<=active_rows_on_proc;row++) {
        for (int col=0;col<total_cols;col++) {
            std::cout << std::setw(5) << Temp(row,col) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::flush;

    // if you are not the last MPI process send a synchronization message to 
    // down most adjacent process to indicate that it is now that processes
    // turn to send its data. if this is the last process then
    // the communication is skipped but the lower boundary condition 
    // row is outputed instead.
    if (rank!=numprocs-1) {
        MPI_Send(&flg,1,MPI_CHAR,down_pr,123,MPI_COMM_WORLD);
        // if root process wait until output of all other
        // elements from the other processes before continuing
        // via a signal communication from numprocs-1
        if (rank==0) {
            MPI_Recv(&flg,1,MPI_CHAR,numprocs-1,MPI_ANY_TAG,
                MPI_COMM_WORLD,&status);
        }
    }
    else {
        // print out last row of boundary condition points
        for (int col=0;col<total_cols;col++) {
            std::cout << std::setw(5) << Temp(active_rows_on_proc+1,col) << " ";
        }
        std::cout << std::endl << std::flush;
        // send synchronization back to root so it can continue
        MPI_Send(&flg,1,MPI_CHAR,0,123,MPI_COMM_WORLD);
    }
}
// Routine that performs a simple 64 integer checksum
// of the binary contents of the final Temp array
// This is used to perform a quick comparison of the
// results to insure that modifications to the original
// program did not affect the accuracy of the computation
unsigned long long int checksum(void) {
    char flg;
    MPI_Status status;
    unsigned long long int *num_ptr,sum = 0;
    double num;
    num_ptr = (unsigned long long int *) &num;

    // wait for turn to compute checksum for portion of 
    // Temperature data
    if (rank!=0) {
        // if not rank 0 wait until adjacent left process has completed
        MPI_Recv(&sum,sizeof(double),MPI_CHAR,up_pr,MPI_ANY_TAG,
                MPI_COMM_WORLD,&status);
    }
    else {
        // if rank 0 then go ahead and compute checksum for the upper
        // row of boundary points
        for (int col=0;col<total_cols;col++) {
            num=Temp(0,col);
            sum += (*num_ptr);
        }
    }

    // when it is your turn then go ahead and compute checksum for
    // out all active points that this process is to process
    for (int row=1;row<=active_rows_on_proc;row++) {
        for (int col=0;col<total_cols;col++) {
            num=Temp(row,col);
            sum += (*num_ptr);
        }
    }

    // if you are not the last MPI process send current checksum which also
    // serves as a synchronization message to the 
    // down most adjacent process to indicate that it is now that processes
    // turn to send its data. if this is the last process then
    // the communication is skipped but the lower boundary condition 
    // row's checksum is added to the checksum.
    if (rank!=numprocs-1) {
        MPI_Send(&sum,sizeof(double),MPI_CHAR,down_pr,123,MPI_COMM_WORLD);
        if (rank==0) {
            MPI_Recv(&sum,sizeof(double),MPI_CHAR,numprocs-1,MPI_ANY_TAG,MPI_COMM_WORLD,
                    &status);
        }
    }
    else {
        // compute checksum for last row of boundary condition row
        for (int col=0;col<total_cols;col++) {
            num=Temp(active_rows_on_proc+1,col);
            sum += (*num_ptr);
        }
        MPI_Send(&sum,sizeof(double),MPI_CHAR,0,123,MPI_COMM_WORLD);
    }
    return sum;
} 


int main (int argc, char *argv[]){

    MPI_Init(&argc,&argv); // initalize MPI environment
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs); // find total number of MPI tasks
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);     // get unique task id number

    if (argc!=3 && argc!=4) {
        if (rank==0) {
            std::cout << "Usage: mpirun -np [No. procs] " << argv[0] <<
                " [Dim n] [No. Iterations] [Suppress Output]"
                << std::endl;
        }
        MPI_Finalize();
        exit(0);
    }

    // Determine the following MPI process local but program
    // global constants at run time
    //   n,num_iterations,up_pr, down_pr,
    //   active_rows_on_proc,total_rows_on_proc,
    //   total_cols, and temp array   

    // get total number of points not counting boundary points
    // from first command line argument 
    // Warning No Error Checking 
    n = atoi(argv[1]);

    // get total number of iterations to run simulation
    // Warning No Error Checking
    num_iterations = atoi(argv[2]);

    // define the logical upper-most MPI process ID
    up_pr = rank-1;

    // define the logical down-most MPI process ID
    down_pr = rank+1;

    // define the number of rows that the MPI process
    // is to process 
    active_rows_on_proc = n/numprocs;

    // set total rows on MPI process including boundary 
    // points
    total_rows_on_proc = active_rows_on_proc+2;

    // set total columns plus boundary points
    total_cols = n+2; // total columns plus boundary points

    // dynamically allocate memory to store the MPI process
    // local temp array
    temp = new double [total_rows_on_proc*total_cols]; 

    // initialize MPI processtemperature matrix
    init_temp();

    MPI_Barrier(MPI_COMM_WORLD);
    double time = MPI_Wtime();

    // compute temperatures returning after specified number
    // of iterations
    compute_temp(); 

    // time interval calculation associated with MPI process
    time = MPI_Wtime()-time; // new time = end time - start time

    // taking the maximum of the individual MPI process times
    double parallel_time;
    MPI_Reduce(&time,&parallel_time,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);

    // print temp array output
    if (argc!=4) {
        print_temp();
        if (rank==0) {
            // print time in normal human readable format
            std::cout << "Execution Time = " << parallel_time << " Seconds"
                << std::endl;
        }
    }

    // print time or checksum data by itself without temp array output
    // print out the results if there is no suppress output argument
    if (argc==4) {
        // print time in gnuplot format
        if (*argv[3]=='G') {
            if (rank==0) {
                std::cout << n << " " << parallel_time << std::endl;
            }
        }
        // print 64 bit checksum
        else if (*argv[3]=='C') {
            // compute 64 bit data checksum 
            unsigned long long int ck_sum=checksum();
            if (rank==0) {
                std::cout << "64 bit Checksum = " << ck_sum << std::endl;
            }
        }
        else {
            if (rank==0) {
                // print time in normal human readable format
                std::cout << "Execution Time = " << parallel_time << " Seconds"
                     << std::endl;
            }
        }
    }
    delete temp;

    // Terminate MPI Program -- perform necessary MPI housekeeping
    // clear out all buffers, remove handlers, etc.
    MPI_Finalize();
    return (0);
}
