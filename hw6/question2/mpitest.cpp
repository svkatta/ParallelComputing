#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char **argv) {
    int rank, size, n = 6;
    int *data, *sendbuf, *recvbuf;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Allocate memory for data and buffers
    data = (int*) malloc(n * sizeof(int));
    sendbuf = (int*) malloc(n * size * sizeof(int));
    recvbuf = (int*) malloc(n * size * sizeof(int));
    for(int i = 0; i < n * size; i++){
        sendbuf[i] = rank+1;
        recvbuf[i] = 0;
    }

    // // Initialize data for each processor
    // for (int i = 0; i < n; i++) {
    //     data[i] = rank+1 ;
    // }

    // // Copy local data to send buffer
    // for (int i = 0; i < n; i++) {
    //     sendbuf[rank * n + i] = data[i];
    // }

    if(rank == 1 ){
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < n; j++) {
                printf("%d ", sendbuf[i * n + j]);
            }
            printf("\n");
        }
    }

    // Perform all-to-all communication
    MPI_Alltoall(sendbuf, n, MPI_INT, recvbuf, n, MPI_INT, MPI_COMM_WORLD);

    // Print received data for each processor
    if(rank == 1 ){
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < n; j++) {
                printf("%d ", recvbuf[i * n + j]);
            }
            printf("\n");
        }
    }

    // Free memory
    free(data);
    free(sendbuf);
    free(recvbuf);

    MPI_Finalize();
    return 0;
}
