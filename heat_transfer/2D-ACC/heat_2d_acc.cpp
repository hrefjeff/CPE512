// 2-D Temperature Example base serial code
// C++ version

/*
To compile on dmc.asc.edu
   GNU Compiler
      module load pgi
      pgc++ -acc -ta=tesla:cc3+ -Minfo=accel -o heat_2d_acc heat_2d_acc.cpp -fast

To execute on dmc.asc.edu
   GNU Compiler
      run_gpu run.sh
        #!/bin/bash
        ./heat_2d_acc 30000 75 C >heat_2d_acc_out.txt
        ./heat_2d_acc 30000 75 S >heat_2d_acc_out.txt

        # don't forgot toe "chmod 744 run.sh" this file
        # execute using: run_gpu run.sh
        # script runs
        # Enter Queue Name (default <cr>: gpu) class
*/

using namespace std;
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <sys/time.h>

#define TIMER_CLEAR  (tv1.tv_sec = tv1.tv_usec = tv2.tv_sec = tv2.tv_usec = 0)
#define TIMER_START     gettimeofday(&tv1, (struct timezone*)0)
#define TIMER_ELAPSED   (double) (tv2.tv_usec- tv1.tv_usec)/1000000.0+(tv2.tv_sec-tv1.tv_sec)
#define TIMER_STOP      gettimeofday(&tv2, (struct timezone*)0)
struct timeval tv1,tv2;


// Global Constants
const int ROOM_TEMP=20;       // temperature everywhere except the fireplace
const int FIREPLACE_TEMP=100; // temperature at upper/right boundary

int n;                // number of non boundary condition rows in problem 
int num_iterations;   // number of successive iterations before terminating

int total_rows;       // total number of rows including 
                      // including boundary condition rows

int total_cols;       // total number of columns including
                      // boundary condition columns

double * restrict temp;         // pointer to shared temperature
                      // array row-ordered storage

double * restrict temp_buf;     // next iteration pointer to shared temperature
                      // array row-ordered storage

// Old style macro to give the illusion of 2D memory
#define Temp(x,y) temp[(x)*total_cols+y] 
#define Temp_buf(x,y) temp_buf[(x)*total_cols+y] // *(temp_buf+x*total_cols+y)

// routine to initialize the temperature vector and the temperature at the
// boundary
void init_temp(void) {
    const int fireplace_start = 0.3 * (double) n;
    const int fireplace_end = 0.7 * (double) n;

    // Set leftmost boundary condition on process 0
    for (int row=0;row < total_rows; row++) {
        for (int col=0;col<total_cols;col++) {
            if (row == 0) {
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

void compute_temp() {

    #pragma acc data copy(temp[:total_rows*total_cols]), create(temp_buf[:total_rows*total_cols])
    for (int i=0;i<num_iterations;i++) {

        #pragma acc parallel
        {
            #pragma acc loop
            for (int j=1;j<=n;j++) {
                for (int k=1;k<=total_cols-2;k++) {
                    Temp_buf(j,k)=0.25*(Temp(j-1,k)+Temp(j+1,k)+
                            Temp(j,k-1)+Temp(j,k+1));
                }
            }

            #pragma acc loop
            for (int j=1;j<=n;j++) {
                for (int k=1;k<=total_cols-2;k++) { 
                    Temp (j,k)=Temp_buf(j,k);
                }
            }
        }
    }

}

// routine to display temperature values at each point including the 
// boundary points
void print_temp(void) {
    cout << "Temperature Matrix Including Boundary Points" << endl;
    for (int row=0;row<total_rows;row++) {
        for (int col=0;col<total_cols;col++) {
            cout << setw(5) << Temp(row,col) << " ";
        }
        cout << endl << flush;
    }
}

// Routine that performs a simple 64 integer checksum
// of the binary contents of the final Temp array
// This is used to perform a quick comparison of the
// results to insure that modifications to the original
// program did not affect the accuracy of the computation
unsigned long long int checksum(void) {
    void *ptr;
    unsigned long long int sum = 0;
    for (int row=0;row<total_rows;row++) {
        for (int col=0;col<total_cols;col++) {
            ptr=(void *) &Temp(row,col);
            sum += *(unsigned long long int *) ptr;
        }
    }
    return sum;
} 

int main (int argc, char *argv[]){

    if (argc!=3) {
        cout << "Usage: temp2d_serial " << argv[0] <<
            " [Dim n] [No. Iterations]"
            << endl;
        exit(1);
    }

    // get total number of points not counting boundary points
    // from first command line argument 
    // Warning No Error Checking 
    n = atoi(argv[1]);

    // get total number of iterations to run simulation
    // Warning No Error Checking
    num_iterations = atoi(argv[2]);

    // set total columns plus boundary points
    total_rows = n+2; // total rows plus boundary points

    // set total columns plus boundary points
    total_cols = n+2; // total columns plus boundary points

    // dynamically allocate shared memory to
    // temp and temp_buf arrays
    temp = new double [total_rows*total_cols]; 
    temp_buf = new double [total_rows*total_cols];

    // initialize temperature matrix
    init_temp();

    // begin timer
    TIMER_CLEAR;
    TIMER_START;

    // compute new temps
    compute_temp(); 

    // stop timer
    TIMER_STOP;

    cout << "Execution Time = " << TIMER_ELAPSED << " Seconds" << endl;
    cout << "64 bit Checksum = " << checksum() << endl;

    delete temp;
    delete temp_buf;
}
