#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv){
    int rank;   
    int cluster_size;

    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &cluster_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int comm_rank = (rank %(cluster_size/2)) + ((rank< cluster_size/2) ?  (cluster_size/2) : 0);


    int message_size = atoi(argv[1]);
    int n_trails = 1000;

    char message[message_size];
    float start_time,end_time;

    float total_time = 0;
    MPI_Barrier(MPI_COMM_WORLD); // Synchronize all processes
    for (int i = 0; i < n_trails ; i++){
        if (rank < (cluster_size/2)){
            start_time = MPI_Wtime();
            MPI_Send(message, message_size, MPI_CHAR, comm_rank, 0, MPI_COMM_WORLD); 
            MPI_Recv(message, message_size, MPI_CHAR, comm_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
            end_time = MPI_Wtime();
            total_time += end_time - start_time;
        }else{
            MPI_Recv(message, message_size, MPI_CHAR, comm_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
            MPI_Send(message, message_size, MPI_CHAR, comm_rank, 0, MPI_COMM_WORLD); 
        }
    }

    // gather all the times
    float all_times[cluster_size];
    MPI_Gather(&total_time, 1, MPI_FLOAT, all_times, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if(rank ==0){
        //printf("Average time for %d trails with message size %d is %f\n",n_trails,message_size,(total_time)/(n_trails*2));
        float total_sum=0;
        for (int i = 0; i < cluster_size; i++){
            total_sum += all_times[i];
        }
        printf("%f \n", (total_sum)/(cluster_size*n_trails));
    }

    MPI_Finalize();
}

