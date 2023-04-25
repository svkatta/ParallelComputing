#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <assert.h> 


void swap(int *arr ,int i ,  int j){
    int temp =arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

int s_partition(int *arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr,i,j);
        }
    }
    swap(arr,i + 1,high);
    return i + 1;
}

void s_quicksort(int * arr, int low, int high){ // [low , high]
    if (low < high) {
        int pivot_index = s_partition(arr, low, high);
        s_quicksort(arr, low, pivot_index - 1);
        s_quicksort(arr, pivot_index + 1, high);
    }
}

void pick_pivots(int* arr, int low , int high ,int np, int * pivots){
    int size = (high - low + 1)/p;
    for(int i = 0 ; i < np ; i++){
        pivots[i] = arr[low + i*size];
    }

}


int scatter(int* arr,int low,int high){
    int process_rank;         // process rank [0,N-1]
    int size_of_cluster;      // N processes

    MPI_Comm_size(MPI_COMM_WORLD, &size_of_cluster);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);


    return 0;
}



int p_bucketsort( MPI_Comm comm_world,int* arr ,int low, int high ){

    int process_rank;         // process rank [0,N-1]
    int p;      // N processes

    MPI_Comm_size(comm_world, &p);
    MPI_Comm_rank(comm_world, &process_rank);
    printf("process rank is %d total processors %d\n",process_rank,p);
    if (p == 1) {
        // printf("size of cluster is 1 \n");
        s_quicksort(arr,low,high);
        MPI_Barrier(MPI_COMM_WORLD);
        // gather(arr,low,high);
        return 0;
    }

    // collect the piviots from array
    s_quicksort(arr,low,high);
    pick_pivots(arr,low,high,p-1,pivots);
    
    // comunicate the pivots to all processes
    int *pivots = (int*) malloc(p-1 * sizeof(int));
    int *sendbuf = (int*) malloc(p * p-1 * sizeof(int));
    int *recvbuf = (int*) malloc(p * p-1 * sizeof(int));

    MPI_Alltoall(pivots, p-1, MPI_INT, recv_arr, p-1, MPI_INT, comm_world);
    

    // select the pivots
    s_quicksort(recvbuf,0,p*(p-1)-1);
    pick_pivots(recvbuf,0,p*(p-1)-1,p-1,pivots);
    // divide the array into buckets





    return 0;
}



int main(int argc, char** argv){

    MPI_Init(&argc, &argv);          // initializes the MPI environment and starts MPI communications
    // get processor rank and size of cluster
    int process_rank ;
    int size_of_cluster;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size_of_cluster);

    int ps_n = atoi(argv[1]);           // n or no of elements in total array
    int * arr = (int*) malloc((ps_n/size_of_cluster)* sizeof(int));

    // scatter the array
    

    p_bucketsort(MPI_COMM_WORLD, arr, 0, 10);


    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();          // cleans up the MPI environment and ends MPI communications
    return 0;
}
