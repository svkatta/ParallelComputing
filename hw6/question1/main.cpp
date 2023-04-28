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

// void s_quicksort(int * arr, int low, int high){ // [low , high]
//     if (low < high) {
//         int pivot_index = s_partition(arr, low, high);
//         s_quicksort(arr, low, pivot_index - 1);
//         s_quicksort(arr, pivot_index + 1, high);
//     }
// }

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void s_quicksort(int * arr, int low, int high){
    qsort(arr+low, high-low+1, sizeof(int), cmpfunc);
}

bool validate_array(int* arr , int size){
    int current = arr[0];
    for(int i =1 ; i <size ;i++){
        if(arr[i] < current){
            return false;
        }
        current = arr[i];
    }
    return true;
}


int gather(int* arr,int low,int high){
    int process_rank;         // process rank [0,N-1]
    int size_of_cluster;      // N processes

    MPI_Comm_size(MPI_COMM_WORLD, &size_of_cluster);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    int send_size = high - low + 1;
    if(process_rank !=0){
        MPI_Send(&send_size, 1 , MPI_INT, 0, 5, MPI_COMM_WORLD);
        MPI_Send(arr, send_size , MPI_INT, 0, 6, MPI_COMM_WORLD);
    }else{
        int recv_size = 0;
        int recv_arr[send_size];
        int j = 0;
        for(int i = 1 ; i < size_of_cluster ; i++){
            MPI_Recv(&recv_size, 1 , MPI_INT, i, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(arr+high+1+j, recv_size , MPI_INT, i, 6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            j=j+recv_size;
        }
    }

    return 0;
}



int p_quicksort( MPI_Comm comm_world,int* arr ,int low, int high ,int * recv_arr){

    int process_rank;         // process rank [0,N-1]
    int size_of_cluster;      // N processes

    MPI_Comm_size(comm_world, &size_of_cluster);
    MPI_Comm_rank(comm_world, &process_rank);

    if (size_of_cluster == 1) {
        // printf("size of cluster is 1 \n");
        s_quicksort(arr,low,high);
        MPI_Barrier(MPI_COMM_WORLD);
        // gather(arr,low,high);
        return 0;}

    int pivot;

    // communicate piviot to all processes
    int bcast_proc = rand()%size_of_cluster;
    if( bcast_proc == process_rank){ 
        pivot = arr[high];
    }
    MPI_Bcast(&pivot, 1, MPI_INT, bcast_proc, comm_world);


    int color = (process_rank < size_of_cluster/2) ? 0 : 1;
    MPI_Comm new_comm;
    MPI_Comm_split(comm_world,color,process_rank,&new_comm);


    //partition array using piviot such that sending part is on the right
    int i = low - 1;
    for (int j = low; j <= high ; j++) {
        if (color == 0 && arr[j] < pivot) {
            i++;                  // [low,i] < pivot
            swap(arr,i,j);
        }
        if (color == 1 && arr[j] >= pivot ) {
            i++;                  // [low,i] >= pivot
            swap(arr,i,j);
        }
    }

    int comm_rank = (process_rank %(size_of_cluster/2)) + ( (process_rank < (size_of_cluster/2)) ?  (size_of_cluster/2) : 0);

    int recv_size = 0;
    int send_size = high - i; //[i+1,high]


    if (color==0){
        MPI_Send(&send_size, 1 , MPI_INT, comm_rank, 0, comm_world);
        MPI_Send(arr+i+1, high-i , MPI_INT, comm_rank, 1, comm_world);

        MPI_Recv(&recv_size, 1 , MPI_INT, comm_rank, 2, comm_world, MPI_STATUS_IGNORE);
        MPI_Recv(recv_arr, recv_size , MPI_INT, comm_rank, 3, comm_world, MPI_STATUS_IGNORE);
    }else{
        MPI_Recv(&recv_size, 1 , MPI_INT, comm_rank, 0, comm_world, MPI_STATUS_IGNORE);
        MPI_Recv(recv_arr, recv_size , MPI_INT, comm_rank, 1, comm_world, MPI_STATUS_IGNORE);

        MPI_Send(&send_size, 1 , MPI_INT, comm_rank, 2, comm_world);
        MPI_Send(arr+i+1, high-i , MPI_INT, comm_rank, 3, comm_world);
    }

    for(int k = 0 ; k <recv_size  ; k++){
        arr[i+1+k] = recv_arr[k];
    }
    high = i+recv_size;

    p_quicksort(new_comm , arr,low,high,recv_arr);
    return 0;
}




int main(int argc, char** argv){

    MPI_Init(&argc, &argv);          // initializes the MPI environment and starts MPI communications

    // get processor rank and size of cluster
    int process_rank ;
    int size_of_cluster;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size_of_cluster);



    int array_size = atoi(argv[1]) /size_of_cluster;
    // int arr[array_size * size_of_cluster];  
    // int recv_arr[array_size * size_of_cluster];
    int * arr;
    int * recv_arr;
    arr      =  (int*)malloc(array_size * size_of_cluster * sizeof(int));
    recv_arr = (int*)malloc(array_size * size_of_cluster * sizeof(int));
    srand(0); // set random seed
    for(int i =0 ; i < process_rank ; i++){
        rand();
    }
    srand(rand()%1000);
    for(int i = 0; i < array_size; i++){
        arr[i] = rand();
    }
    srand(42);           // set random seed

    float start_time, end_time;
    start_time = MPI_Wtime();
    p_quicksort(MPI_COMM_WORLD, arr,0,array_size-1,recv_arr);
    end_time = MPI_Wtime();


    if (process_rank == 0) {
        printf(" %f ",end_time-start_time);
    }

    MPI_Finalize();          // cleans up the MPI environment and ends MPI communications
    return 0;
}

