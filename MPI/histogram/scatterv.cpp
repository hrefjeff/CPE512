#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int iCeil(int x, int y){
    return (x + y - 1) / y;
}
 
/**
 * @brief Illustrates how to use the variable version of a scatter.
 * @details A process is designed as root and begins with a buffer containig all
 * values, and prints them. It then dispatches these values to all the processes
 * in the same communicator. Other process just receive the dispatched value(s)
 * meant for them. Finally, everybody prints the value received. This
 * application is designed to cover all cases:
 * - Different send counts
 * - Different displacements
 * This application is meant to be run with 3 processes.
 *
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
    MPI_Init(&argc, &argv);
 
    // Get number of processes and check that 3 processes are used
    int numprocs;
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    if(numprocs != 3)
    {
        printf("This application is meant to be run with 3 processes.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
 
    // Determine root's rank
    int root_rank = 0;
 
    // Get my rank
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int numbers[10] = {
        101,
        102,
        103,
        104,
        105,
        106,
        107,
        108,
        109,
        110
    };

    if (my_rank == root_rank){
        printf("Values in the numers of root process:\n");
        for(int i = 0; i < 10; i++)
        {
            printf("%d) %d\n", i, numbers[i]);
        }
    }

    printf("\n\n\n");

    const int local_size = iCeil(10, numprocs);

    int *local_nums = new int[local_size];

    // Declare the counts
    int counts[3] = {2, 2, 3};

    // Declare the displacements
    int displacements[3] = {0, 4, 7};
 
    switch(my_rank)
    {
        case 0:
        {
            printf("\n");
            MPI_Scatterv(numbers, counts, displacements, MPI_INT, local_nums, 2, MPI_INT, root_rank, MPI_COMM_WORLD);
            printf("Process %d received 1. %d, 2. %d, 3. %d, and 4. %d.\n", my_rank, local_nums[0], local_nums[1], local_nums[2], local_nums[3]);
            break;
        }
        case 1:
        {
            MPI_Scatterv(NULL, NULL, NULL, MPI_INT, local_nums, 2, MPI_INT, root_rank, MPI_COMM_WORLD);
            printf("Process %d received 1. %d, 2. %d, 3. %d, and 4. %d.\n", my_rank, local_nums[0], local_nums[1], local_nums[2], local_nums[3]);
            break;
        }
        case 2:
        {
            MPI_Scatterv(NULL, NULL, NULL, MPI_INT, local_nums, 3, MPI_INT, root_rank, MPI_COMM_WORLD);
            printf("Process %d received 1. %d, 2. %d, and 3. %d\n", my_rank, local_nums[0], local_nums[1], local_nums[2]);
            break;
        }
    }
 
    MPI_Finalize();
 
    return 0;
}