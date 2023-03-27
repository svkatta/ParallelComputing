#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv){
    int rank;   
    int cluster_size;

    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &cluster_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    int message_size = atoi(argv[1]);
    int n_trails = 1000;

    int message[message_size];
    float start_time,end_time;

    float total_time = 0;
    for (int i = 0; i < n_trails ; i++){
        MPI_Barrier(MPI_COMM_WORLD); // Synchronize all processes
        if (rank ==0){
            start_time = MPI_Wtime();
            MPI_Send(message, message_size, MPI_INT, 1, 0, MPI_COMM_WORLD); 
            MPI_Recv(message, message_size, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
            end_time = MPI_Wtime();
            total_time += end_time - start_time;
        }else if (rank ==1){
            MPI_Recv(message, message_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
            MPI_Send(message, message_size, MPI_INT, 0, 0, MPI_COMM_WORLD); 
        }

    }

    if(rank ==0){
        printf("Average time for %d trails with message size %d is %f\n",n_trails,message_size,(total_time)/(n_trails*2));
    }

    MPI_Finalize();
}
