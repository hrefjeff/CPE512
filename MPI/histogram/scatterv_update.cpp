#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int iCeil(int x, int y){
    return (x + y - 1) / y;
}
 
/**
 *       +-----------------------------------------+
 *       |                Process 0                |
 *       +-----+-----+-----+-----+-----+-----+-----+
 *       | 100 |  0  | 101 | 102 |  0  |  0  | 103 |
 *       +-----+-----+-----+-----+-----+-----+-----+
 *         |            |     |                |
 *         |            |     |                |
 *         |            |     |                |
 *         |            |     |                |
 *         |            |     |                |
 *         |            |     |                |
 * +-----------+ +-------------------+ +-----------+
 * | Process 0 | |    Process 1      | | Process 2 |
 * +-+-------+-+ +-+-------+-------+-+ +-+-------+-+
 *   | Value |     | Value | Value |     | Value |
 *   |  100  |     |  101  |  102  |     |  103  |
 *   +-------+     +-------+-------+     +-------+ 
 *                
 **/
int main(int argc, char* argv[])
{

    // Get number of processes and check that 3 processes are used
    int numprocs;
    
    // Get my rank
    int rank;

    // Determine root's rank
    int root_rank = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int total_size = iCeil(17, numprocs)*numprocs;
    int *numbers = new int[total_size];

    if (rank == root_rank) {

        int start = 100;
        for (int i=0; i<17; i++){
            numbers[i] = start + i; 
        }

        printf("Values in the numers of root process:\n");
        for (int i = 0; i < 17; i++)
        {
            printf("%d) %d\n", i, numbers[i]);
        }

    }

    printf("\n\n\n");

    // Delcare the local arrays
    const int local_size = iCeil(17, numprocs);
    int *local_nums = new int[local_size];

    // Declare the displacements (pointer to start of count)
    int *start_points = new int[numprocs];

    // Declare the counts (number of items to be sent)
    int *counts = new int[numprocs];

    if (rank == root_rank){
        start_points[0] = 0;
        int start_result = 0;
        int result = 0;
        for (int i=0; i<numprocs; i++){
            result = 0;
            if (rank < (17 % numprocs)){
                result = iCeil(17, numprocs);
                counts[i] = result;
            } else {
                result = 17/numprocs;
                counts[i] = result;
            }
            if(i > 0) {
                start_result += result;
                start_points[i] = start_result;
            }
        }

        for (int i=0; i<numprocs; i++){
            printf("Rank %d : counts[ %d ] = %d, start_points[ %d ] = %d \n", rank, i, counts[i], i, start_points[i]);
        }
    }

    MPI_Scatterv(numbers, &counts[rank], &start_points[rank], MPI_INT, local_nums, counts[rank], MPI_INT, root_rank, MPI_COMM_WORLD);
 
    switch(rank)
    {
        case 0:
        {
            printf("Process %d received 1. %d, 2. %d, 3. %d, 4. %d, 5. %d, and 6. %d.\n", 
            rank, local_nums[0], local_nums[1], local_nums[2], local_nums[3], local_nums[4], local_nums[5]);
            break;
        }
        case 1:
        {
            printf("Process %d received 1. %d, 2. %d, 3. %d, 4. %d, 5. %d, and 6. %d.\n",
            rank, local_nums[0], local_nums[1], local_nums[2], local_nums[3], local_nums[4], local_nums[5]);
            break;
        }
        case 2:
        {
            printf("Process %d received 1. %d, 2. %d, 3. %d, 4. %d, and 5. %d.\n", 
            rank, local_nums[0], local_nums[1], local_nums[2], local_nums[3], local_nums[4]);
            break;
        }
    }
 
    if (rank==0) {
      delete numbers;
   }
   delete local_nums;

    MPI_Finalize();
 
    return 0;
}