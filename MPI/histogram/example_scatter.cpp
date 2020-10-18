#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char *argv[]) {

    int proccess_rank, size;
    int ScatterData[4] = { 0 };
    int ReceiveData = 0;

    // Init environment
    MPI_Init(&argc, &argv);

    // Get rank
    MPI_Comm_rank(MPI_COMM_WORLD, &proccess_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (proccess_rank == 0) {
        ScatterData[0] = 10;
        ScatterData[1] = 20;
        ScatterData[2] = 30;
        ScatterData[3] = 40;
    }

    cout << "BEFORE <<< Process # " << proccess_rank << ". ReceiveData is: " << ReceiveData << endl;

    MPI_Scatter(ScatterData, 1, MPI_INT, &ReceiveData, 1, MPI_INT, 0, MPI_COMM_WORLD);

    cout << "AFTER >>>> Process # " << proccess_rank << ". ReceiveData is: " << ReceiveData << endl;

    MPI_Finalize(); // Terminate Enviroment

}