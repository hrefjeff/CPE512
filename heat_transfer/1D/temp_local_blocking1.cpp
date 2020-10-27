// 1-D temperature Example with Locally Blocking MPI_Send/MPI_Recv Routinges in
// main compute temp routine but with them ordered so that the MPI_Send/MPI_Recv
// are paired with one another insuring that there is no possibility of
// deadlock due to a MPI_Send routine not finishing because another MPI process
// has not reached its MPI_Recv -- i.e. the safe version. 
using namespace std;
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <mpi.h>

#define LEFT_TEMP 100  // temperature at lower/left boundary
#define RIGHT_TEMP -40 // temperature at upper/right boundary
#define MID_TEMP     0 // initial temperature at the other points

// routine to initialize the temperature vector and the temperature at the
// boundary
void init_temp(double *temp, int total_points, int rank, int numprocs) {
   const int points_per_proc = total_points/numprocs;

   // Set leftmost boundary condition on process 0
   if (rank == 0) {
      temp[0]=LEFT_TEMP;
   }
   // set rightmost boundary condition on last process
   else if (rank == numprocs-1) {
      temp[points_per_proc+1]=RIGHT_TEMP;
   }
   // set initial conditions on all non boundary or ghost points
   for (int i=1;i<=points_per_proc;i++) temp[i]=MID_TEMP;
}

void compute_temp(double * temp, int total_points, int num_iterations,
                  int rank, int numprocs) {

   // assuming total_points is an even multiple of numprocs
   const int points_per_process = total_points/numprocs;
   const int right_pr = rank+1; // rightmost adjacent process
   const int left_pr = rank-1;  // leftmost adjacent process

   double *temp_buf = new double [points_per_process + 2]; // allocate local storage for temp buffer 

   MPI_Status status;
   for (int i=0;i<num_iterations;i++) {
      if (rank%2==0) { // even numbered processes
         // Send/Receive Right
         MPI_Send(&temp[points_per_process],1,MPI_DOUBLE,right_pr,123,MPI_COMM_WORLD);
         MPI_Recv(&temp[points_per_process+1],1,MPI_DOUBLE,right_pr,123,MPI_COMM_WORLD,&status);
         // if not process 0 Send/Receive Left
         if (rank>0) {
            MPI_Send(&temp[1],1,MPI_DOUBLE,left_pr,123,MPI_COMM_WORLD);
            MPI_Recv(&temp[0],1,MPI_DOUBLE,left_pr,123,MPI_COMM_WORLD,&status);
         }
      }
      else { // odd numbered process
         // Receive/Send Left
         MPI_Recv(&temp[0],1,MPI_DOUBLE,left_pr,123,MPI_COMM_WORLD,&status);
         MPI_Send(&temp[1],1,MPI_DOUBLE,left_pr,123,MPI_COMM_WORLD);
         // if not last process Receive/Send Right
         if (rank < numprocs-1) {
            MPI_Recv(&temp[points_per_process+1],1,MPI_DOUBLE,right_pr,123,MPI_COMM_WORLD,&status);
            MPI_Send(&temp[points_per_process],1,MPI_DOUBLE,right_pr,123,MPI_COMM_WORLD);
         }
      }

      // average points -- do not update boundary conditions 
      // -- store in separate temp_buf array
      for (int j=1;j<=points_per_process;j++) {
         temp_buf[j]=0.5*(temp[j-1]+temp[j+1]);
      }
      // set current temperatures to new tempertures for next iteration
      for (int j=1;j<=points_per_process;j++) {
         temp[j]=temp_buf[j];
      }
   }

   delete temp_buf;
}

// routine to display temperature values at each point including the 
// boundary points
void print_temp(double * temp, int total_points,int rank, int numprocs) {
   char flg;
   int i;
   MPI_Status status;

   const int left_pr = rank-1;
   const int right_pr = rank+1;
   const int points_per_proc = total_points/numprocs;

   // wait for turn to print out local temp array 

   if (rank!=0) {
      // if not rank 0 wait until adjacent left process has completed
      MPI_Recv(&flg,1,MPI_CHAR,left_pr,123,MPI_COMM_WORLD,&status);
   }
   else {
      // if rank 0 then go ahead and print header and the left-most
      // boundary point
      cout << "global local temperature" << endl;
      cout << "    0    0 " << setw(5) << temp[0] << endl;
   }

   // when it is your turn print out all points that this process has computed.
   // information includes global index, local index, and temperature value.
   for (i=1;i<=points_per_proc;i++) {
      // print global index, local index, and temp of each point
      cout << setw(5) << rank*points_per_proc+i << setw(5) << i << " " << setw(5) << temp[i] << endl;
   }

   // if you are not the last MPI process send a synchronization message to rightmost adjacent process
   // to indicate that it is now that processes turn to send its data. if this is the last process then
   // the communication is skipped but the rightmost boundary condition is outputed instead.
   if (rank!=numprocs-1) {
      MPI_Send(&flg,1,MPI_CHAR,right_pr,123,MPI_COMM_WORLD);
   }
   else {
      // print global index, local index, and temp of points
      cout << setw(5) << points_per_proc*(rank+1)+1 << setw(5) << points_per_proc+1 << " "
         << setw(5) << temp[points_per_proc+1] << endl;
   }
} 
int main (int argc, char *argv[]){
   int numtasks,rank;

   MPI_Init(&argc,&argv); // initalize MPI environment
   MPI_Comm_size(MPI_COMM_WORLD,&numtasks); // find total number of MPI tasks
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);     // get unique task id number

   if (argc!=3) {
      if (rank==0) {
         cout << "Usage: mpirun -np [] temp_ex [Number of Points] [Number of Iterations]" << endl;
         cout << " Warning: Number of MPI processes must be a multiple of 2" << endl;
      }
      MPI_Finalize();
      exit(1);
   }

   int num_points = atoi(argv[1]); // get total number of points not counting boundary points 
   int num_iterations = atoi(argv[2]); // get total number of iterations to run simulation
   double *temp = new double [num_points/numtasks+2]; // dynamically allocate memory to store temp array

   init_temp(temp,num_points,rank,numtasks); // initialize temperature matrix

   compute_temp(temp,num_points,num_iterations,rank,numtasks); // compute new temps

   print_temp(temp,num_points,rank,numtasks); // print out the results

   delete temp;

   // Terminate MPI Program -- perform necessary MPI housekeeping
   // clear out all buffers, remove handlers, etc.
   MPI_Finalize();
   return (0);
}
