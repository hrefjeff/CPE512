/**************************************************************/
/* Histogram construction Program -- MPI version               */
/* September 2020 -- B. Earl Wells -- University of Alabama    */
/*                                    in Huntsville            */
/***************************************************************/
/*
This program illustrates the basic concepts of SPMD programming using 
MPI.  The program represents a common example that is used often in 
the CPE 412/512 text, creating a histogram of binned numbers.
The program is written in such a way that it is assumed that the
sequence of numbers is first read from a central source (in this case
a data file) by a single MPI process (the root process) and then 
partitioned into equal groups with each part being distributed 
(scattered) to the local memory domains of the other MPI
processes in the system. Each MPI process then locally computes its
own histogram of the data for the data in its own domain. This is
followed by a a coordinated communication/computation operation
where the local histogram information is transferred to the
root MPI process where it is combined into the final histogram
by using element-wise addition (i.e. a vector reduce operation).
The histogram output is then placed by the root process in a file
which is named histogram.txt

The following is a simplified version of the program which you will
be asked to augment in a number of ways in future homework 
assignments. Use the dmc.asc.edu or the Jetson Cluster system for
these assignments. 
Notes: This implementation utilizes a minimum set of MPI methods
       call that include Init, Finalize, Comm_size,
       Comm_rank, Send, and Recv.
       Abort is also used to illustrate its functionality.

To compile on dmc.asc.edu
   GNU Compiler
   module load openmpi/1.10.2-gnu-pmi2
   mpic++ histogram_MPI2.cpp -o histogram_MPI
      or
   Intel Compiler
   module load openmpi/1.10.2-intel-pmi2
   mpic++ histogram_MPI2.cpp -o histogram_MPI -std=c++11

To execute:
   
  On gnu compiler
  use run_script_mpi histogram_MPI_gnu.sh
  where add_num_MPI_gnu.sh is a script file that contains
     #!/bin/bash
     module load openmpi/1.10.2-gnu-pmi2
     srun ./histogram_MPI 100000 100 100.0 0.0 N 
     # to take the histogram on 100000 numbers with 100 bins with
     # histogram data being taken for numbers between 100 and 0 
     # and suppress numbers array output from printing

  On intel compiler
  use run_script_mpi histogram_MPI_intel.sh
  where add_num_MPI_intel.sh is a script file that contains
     #!/bin/bash
     module load openmpi/1.10.2-intel-pmi2
     srun ./histogram_MPI 100000 100 100.0 0.0 N 
     # to take the histogram on 100000 numbers with 100 bins with
     # histogram data being taken for numbers between 100 and 0 
     # and suppress numbers array output from printing
*/  

using namespace std;
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <random>
#include <exception>
#include <mpi.h> /* MPI Prototype Header Files */

/*
ONE-TO-ALL BROADCAST COMMUNICATION ROUTINE
Routine to transfer from the root MPI process the value of
the 'number' parameter to all other MPI processes in the system.
*/
void broadcast_scalar(void *number,MPI::Datatype mpi_datatype,int root, 
   int rank, int numprocs) {
   MPI::Status status;

   // root send value of number to each of the other processes
   // using a locally blocking point-to-point send
   if (rank==root) {
      int tag=123;
      for(int mpiproc=0;mpiproc<numprocs;mpiproc++) {
         if (mpiproc!=root) {
            MPI::COMM_WORLD.Send(number,1,mpi_datatype, 
                  mpiproc,tag);
         }
      }
   }
   // if not root process execute a blocking point-to-point receive
   // with the source being the root process and direct this data to
   // the local copy of 'num'
   else {
      MPI::COMM_WORLD.Recv(number,1,mpi_datatype,
               root, MPI_ANY_TAG,status);

   }

}
/*
Function used by get_parameters routine to prompt the user to 
interactively enter the next parameter and to return it to the 
calling routine
*/
template <class T>
T enter_next_param(const std::string prompt) {
   T num;
   while (1) {
      string input = "";
      cout << prompt << endl;
      getline(cin, input);
      stringstream myStream(input);
      if (myStream >> num) break;
      cout << "Invalid Input" << endl << endl;
   }
   return num;
}

/*
Routine to retrieve the run time parameters which include
1) the number of numbers in the number list,
2) number of equally spaced bins to be used by the histogram,
3) maximum value in bin range, 4) minimum value in bin range.
If there is a 5th run time parameter then outputing of the numbers 
array that was generated is suppressed 
If no parameters are supplied on the command line, then the program
interactively prompts the user for the information.
*/
void get_parameters(int *list_size,int *num_bins,double *max_bin_val, 
   double *min_bin_val,bool *num_list_out, int argc,char *argv[],
   int rank,int numprocs) {

   if(argc==1) {
      // One Command Line Argument Case:
      // case where user did not enter number of numbers on command line
      // In this case, only one of the MPI processes needs to communicate
      // directly with the user. Since there will always be a MPI process
      // with rank 0 this is the one that will perform the 
      // human interface communication.
      if (rank==0) {
         *list_size=enter_next_param<int>("Enter number of numbers in list:");
         *num_bins=enter_next_param<int>("Enter number of bins in histogram:");
         *max_bin_val=enter_next_param<double>("Enter Upper Range of histogram:");
         *min_bin_val=enter_next_param<double>("Enter Lower Range of histogram:");
         char print_out=enter_next_param<char>("Suppress numbers array printing (y or n):");
         if (print_out=='y' || print_out=='Y') *num_list_out=false; 
      }
      // since only the rank=0 MPI process is communicating with the
      // user, this process must send its value to all of the
      // other MPI process. It can do this with the broadcast_scalar()
      // broadcast routine.
      // NOTE: ALL MPI PROCESSES executed the broadcast_scalar function
      broadcast_scalar(list_size,MPI::INT,0,rank,numprocs);
      broadcast_scalar(num_bins,MPI::INT,0,rank,numprocs);
      broadcast_scalar(max_bin_val,MPI::DOUBLE,0,rank,numprocs);
      broadcast_scalar(min_bin_val,MPI::DOUBLE,0,rank,numprocs);
   }
   else if (argc==5 || argc==6) {
      // Two Command Line Argument case:
      // user supplied the number of numbers on the command line.
      // Each MPI process can retrieve it from there. No need to
      // broadcast it to the other processes because each have it at
      // run time.
      *list_size = atoi(argv[1]); // get list_size from command line
      *num_bins = atoi(argv[2]);  // get num_bins from command line
      *max_bin_val = atoi(argv[3]);  // get max_bin_val from command line
      *min_bin_val = atoi(argv[4]);  // get min_bin_val from command line
      if (argc==6) *num_list_out=false; // suppress output of numbers array
   }
   else {
      if (rank==0) {
         cout << "Usage: mpirun -np [num MPI processes] histogram_MPI [list size num_bins max_bin_val min_bin_val]" << endl;
      }
      MPI_Finalize(); // Terminate MPI
      exit(0); // Exit Program 
   }
}

/*
Routine that fills a 1-D array with Random Data with values
based on a normal distibution that has a mean of MEAN and a
standard deviation of STDEV
This simulates in some way what might happen if there was a 
single sequential data acquisition source that only one MPI process
could handle at a time
*/
#define MEAN  50.0f
#define STDEV 20.0f
void create_list(double *numbers,int list_size) {
   std::default_random_engine generator(123546);
   std::normal_distribution<double> distribution(MEAN,STDEV);

   for(int i=0;i<list_size;i++) {
      numbers[i]=distribution(generator);
   }
}

// Routine that outputs the numbers matrix to the screen 
void print_matrix(double *numbers,int list_size)
{
   for(int i=0;i<list_size;i++) {
      cout << numbers[i] << endl;
   }
}


/* ONE-TO-ALL SCATTER ROUTINE
Routine to divide and scatter the number data array that resides on the
root MPI process to all other MPI processes in the system.
The number data size is given by the'num_size' parameter its source 
address is given by the '*numbers' parameter, and the destination
group data associated with the current process is given by the
'*group' parameter.  */
void scatter(double *numbers,double *group,int num_size, int root, int rank,int numprocs)
{
   MPI::Status status;
   int tag = 234;
   
   // determine number of elements in subarray groups to be processed by
   // each MPI process assuming a perfectly even distribution of elements 
   const int number_elements_per_section = num_size/numprocs;

   // if root MPI process send portion of numbers array to each of the
   // the other MPI processes as well as make a copy of the portion
   // of the numbers array that is slated for the root MPI process
   if (rank==root) {
      int begin_element=0;

      for(int mpiproc=0;mpiproc<numprocs;mpiproc++) {
      
         // in MPI root process case just copy the appropriate subsection
         // locally from the numbers array over to the group array
         if (mpiproc==root) {
            for (int i=0;i<number_elements_per_section;i++) 
               group[i]=numbers[i+begin_element];
         }
         // if not the root process send the subsection data to
         // the next MPI process
         else {
            MPI::COMM_WORLD.Send(&numbers[begin_element],
               number_elements_per_section,
               MPI::DOUBLE,mpiproc,tag);
         }
         // point to next unsent or uncopied data in numbers array
         begin_element += number_elements_per_section;
      }
   }
   // if a non root process just receive the data
   else {
      MPI::COMM_WORLD.Recv(group,number_elements_per_section,MPI::DOUBLE,
               root,MPI_ANY_TAG,status);  
   }
}
/*
ALL-TO-ONE Reduce ROUTINE
Routine to communicate the local histograms that are present on each
of the MPI processes to the root MPI process to form a single histogram
of the entire data set. This is done by performing an element-by-element
vector addition as part of this communication operation. This type
of communication is called a reduction because the information of multiple
data structures has been combined by performing some form of associate 
operation into a smaller data structure. The location of this structure
is the MPI process that is designated as the root process in the function
arguments.
Parameters:
   histogram == location of the reduced histogram on the root process
   histogram_local == location of each MPI process computed histogram
   histogram_size == number of bins in the histogram
   root == MPI rank of the root MPI process
*/
void reduce(int *histogram, int *histogram_local,int histogram_size, 
   int root, int rank,int numprocs) {
   int tag = 123;
   // if MPI root process perform an element-wise addition of the histograms 
   // computed on each of the MPI processes. 
   if (rank==root) {
      // first initialize root MPI processes' histogram data structure with
      // the root's locally computed histogram
      for (int i=0;i<histogram_size;i++) {
         histogram[i]=histogram_local[i];
      }
      // then receive each of the other process' locally computed histogram
      // and add them element by element to the current histogram
      MPI::Status status;
      for(int mpiproc=0;mpiproc<numprocs;mpiproc++) {
         if (mpiproc!=root) {
            MPI::COMM_WORLD.Recv(histogram_local,histogram_size,MPI::INT,
            mpiproc,MPI_ANY_TAG,status);
            for (int i=0;i<histogram_size;i++) {
               histogram[i]+=histogram_local[i];
            }
         } 
      }
   }
   // if the current MPI process is not the root process then send this
   // process' local histogram data to the root MPI process
   else {
      MPI::COMM_WORLD.Send(histogram_local,histogram_size, MPI::INT, 
         root, tag);
   }
}

// MAIN ROUTINE: parallel histogram calculation
int main(int argc, char *argv[]) {

   int numprocs,rank;

   MPI::Init(argc,argv); // Initalize MPI environment
   rank= MPI::COMM_WORLD.Get_rank(); // get process identity number
   numprocs=MPI::COMM_WORLD.Get_size(); //get total number of processes

   //get run time parameters from the user
   // list_size == number of numbers to generate on root node
   // num_bins  == number of equally-spaced bins for the histogram
   // value range (max_val,min_val) that is being recorded in the histogram
   int list_size, num_bins;
   double max_val, min_val;
   bool print_flg=true; // default is to print numbers out to screen

   get_parameters(&list_size,&num_bins,&max_val,&min_val,&print_flg,
      argc,argv,rank,numprocs);
   // compute the range associated with each bin
   const double bin_range = (max_val-min_val)/num_bins;

   double *numbers;
   int *histogram;
   //  if root MPI Process (0) then
   if(rank==0) {

      // dynamically allocate from heap the histogram array on the root process
      try {
         histogram = new int[num_bins];
      }
      catch (exception& err) { 
         cout << err.what() << " on Rank=0 MPI Process for histogram "
              << "array :(" << endl << flush;
         MPI::COMM_WORLD.Abort(1); // abort the MPI Environment
      }

      // dynamically allocate from heap the numbers array on the root process
      try {
         numbers = new double[list_size];
      }
      catch (exception& err) { 
         cout << err.what() << " on Rank=0 MPI Process for numbers "
              << "array :(" << endl << flush;
         MPI::COMM_WORLD.Abort(1); // abort the MPI Environment
      }

      // initialize numbers array with random data
      create_list(numbers,list_size);

      // print the array if printing is enabled
      if (print_flg) { 
         cout << "numbers matrix =" << endl; 
         print_matrix(numbers,list_size);
         cout << endl;
      }
   }

   // dynamically allocate from heap the numbers_local array on each
   // of the MPI Processes
   const int numbers_local_array_sz = list_size/numprocs+1; 
   double *numbers_local;
   try {
      numbers_local = new double[numbers_local_array_sz];
   }
   catch (exception& err) { 
      cout << err.what() << " on Rank=" << rank << 
          " MPI Process for numbers_local array :(" << endl << flush;
      MPI::COMM_WORLD.Abort(1); // abort the MPI Environment
   }
 
   // dynamically allocate from heap the histogram_local array on each
   // of the MPI Processes
   int *histogram_local;
   try {
      histogram_local = new int[num_bins];
   }
   catch (exception& err) { 
      cout << err.what() << " on Rank=" << rank << 
          " MPI Process for histogram_local array :(" << endl << flush;
      MPI::COMM_WORLD.Abort(1); // abort the MPI Environment
   }

   // scatter the numbers array to all MPI processes in
   // the system
   scatter(numbers,numbers_local,list_size,0,rank,numprocs);

   // create local histogram of the set of numbers associated 
   // with the current MPI process

   // local list size for MPI process -- 
   //   set for ideal case (perfect load balance)
   const int local_list_size = list_size/numprocs; // set for ideal case 

   // clear out local histogram array, histogram_local
   for (int i=0;i<num_bins;i++) histogram_local[i]=0.0f;
 
   for (int i=0;i<local_list_size;i++) {
      double num;
      num = numbers_local[i];
      if (num>=min_val && num < max_val) {
         num -= min_val;
         histogram_local[(int) (num/bin_range)]++;
      }
   }
   // obtain final histogram on root MPI process by element-wise
   // summation of each of the local histograms present on each 
   // MPI process
   reduce(histogram,histogram_local,num_bins,0,rank,numprocs);

   // output sum from root MPI process
   if (rank==0) {
       cout << "Histogram Information" << endl;
       for (int i=0;i<num_bins;i++) {
          cout << setprecision(8) << min_val+bin_range/2+bin_range*i <<
             "  " << histogram[i] << endl;
       }
       cout << numprocs << " MPI Process OpenMPI Implementation"
            << endl << flush;
   }

   if (rank==0) {
      delete histogram;
      delete numbers;
   }
   delete histogram_local;
   delete numbers_local;

   // Terminate MPI Program -- perform necessary MPI housekeeping
   // clear out all buffers, remove handlers, etc.
   MPI::Finalize(); // Remove MPI Environment

}

