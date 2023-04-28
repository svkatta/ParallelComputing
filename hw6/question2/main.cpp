#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <assert.h> 

int cum_sum(int* arr , int size){
    int count =0;
    for(int i = 0 ; i < size ; i++){
        count += arr[i];
    }
    return count;
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
    int size = (high - low + 1)/(np+1);
    for(int i = 0 ; i < np ; i++){
        pivots[i] = arr[low + (i+1)*size];
        // printf("pivots %d %d \n",low + (i+1)*size,pivots[i]);
    }
}


int scatter(int* arr,int low,int high){
    int process_rank;         // process rank [0,N-1]
    int size_of_cluster;      // N processes

    MPI_Comm_size(MPI_COMM_WORLD, &size_of_cluster);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);


    return 0;
}


int find_index(int arr[], int low, int high, int value) {
    int mid;
    while (low <= high) {
        mid = low + (high - low) / 2;

        if (arr[mid] == value) {
            return mid;
        }
        else if (arr[mid] < value) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    return mid; // element not found
}


int* get_counts( int* arr , int low , int high , int* piviots, int np ){ // (piviot_i , piviot_i+1]
    int* send_counts = (int*) malloc((np+1) * sizeof(int));
    int current = low-1;
    for(int i = 0 ; i < np ; i++){
        send_counts[i] = find_index(arr,low,high,piviots[i]) - current ;
        // printf("--- %d \n",send_counts[i]);
        current = current + send_counts[i];
    }
    send_counts[np] = high - current;
    return send_counts;
}

int* get_displs(int* arr , int p){
    int* send_displs = (int*) malloc(p * sizeof(int));
    int count =0;
    for(int i = 0 ; i < p ; i++){
        send_displs[i] = count;
        count = count + arr[i];
    }
    return send_displs;
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int p_bucketsort( MPI_Comm comm_world,int* arr ,int low, int high ){
    int print_rank =-1;
    int process_rank;         // process rank [0,N-1]
    int p;      // N processes

    MPI_Comm_size(comm_world, &p);
    MPI_Comm_rank(comm_world, &process_rank);
    // printf("process rank is %d total processors %d  %d %d\n",process_rank,p,low,high);
    if (p == 1) {
        // printf("size of cluster is 1 \n");
        // s_quicksort(arr,low,high);
        qsort(arr, high-low+1, sizeof(int), cmpfunc);
        // MPI_Barrier(MPI_COMM_WORLD);
        // gather(arr,low,high);
        return 0;
    }


    // collect the piviots from array
    int *pivots = (int*) malloc((p-1) * sizeof(int));

    // s_quicksort(arr,low,high);
    qsort(arr, high-low+1, sizeof(int), cmpfunc);
    pick_pivots(arr,low,high,p-1,pivots);

    if(process_rank == print_rank ){
        printf("rank %d  local picked piviots",process_rank);
        for(int i = 0 ; i < p-1 ; i++){
            printf("%d ",pivots[i]);
        }
        printf("\n---\n");
    }
    
    
    // comunicate the pivots to all processes
    int *sendbuf = (int*) malloc(p * (p-1) * sizeof(int));
    int *recvbuf = (int*) malloc(p * (p-1) * sizeof(int));
    for(int j=0 ; j < p ; j++){
        for(int i = 0 ; i < p-1 ; i++){
            sendbuf[j*(p-1)+i] = pivots[i];
        }
    }
    if(process_rank == print_rank  ){
        for (int i = 0; i < p; i++) {
            for (int j = 0; j < (p-1); j++) {
                printf("%d ", sendbuf[i * (p-1) + j]);
            }
            printf("\n");
        }
        printf("---\n");
    }
    

    MPI_Alltoall(sendbuf, p-1, MPI_INT, recvbuf, p-1, MPI_INT, MPI_COMM_WORLD);

    if(process_rank == print_rank  ){
        for (int i = 0; i < p; i++) {
            for (int j = 0; j < (p-1); j++) {
                printf("%d ", recvbuf[i * (p-1) + j]);
            }
            printf("\n");
        }
    }
    
    
    // select the pivots
    s_quicksort(recvbuf,0,p*(p-1)-1);
    
    pick_pivots(recvbuf,0,p*(p-1)-1,p-1,pivots);

    

    if(process_rank == print_rank ){
        for(int i = 0 ; i < p*(p-1) ; i++){
            printf("%d ",recvbuf[i]);
        }
        printf("\n");
        printf(" global picked piviots ");
        for (int i = 0; i < p-1; i++) {
            printf("%d ", pivots[i ]);
        }
        printf("\n");
    }

    

    // divide the array into buckets and communicate

    int* send_counts;
    int* recv_counts = (int*) malloc(p * sizeof(int));

    send_counts = get_counts(arr,low,high,pivots,p-1);
    const int* send_displs = get_displs(send_counts,p);
    MPI_Alltoall(send_counts, 1, MPI_INT, recv_counts,1, MPI_INT, MPI_COMM_WORLD);
    const int* receive_displs = get_displs(recv_counts,p);

    if(process_rank == print_rank ){
        for (int i = 0; i < p; i++) {
            printf("  %d %d %d %d \n", send_counts[i],send_displs[i], recv_counts[i],receive_displs[i] );
        }
    }
    

    int* recv_vals = (int*) malloc(cum_sum(recv_counts,p) * sizeof(int));
    MPI_Alltoallv(arr, send_counts,send_displs , MPI_INT, 
                  recv_vals, recv_counts, receive_displs, MPI_INT, MPI_COMM_WORLD);
    if(process_rank == print_rank){
    for(int i = low ; i <= high ; i++){
        printf("%d ",arr[i]);
    }
    printf("\n");
    for(int i = 0 ; i < cum_sum(recv_counts,p) ; i++){
        printf("%d ",recv_vals[i]);
    }
    printf("\n");
    }
    
    qsort(recv_vals, cum_sum(recv_counts,p), sizeof(int), cmpfunc);
    // s_quicksort(recv_vals,0,cum_sum(recv_counts,p)-1);
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
    int array_size = ps_n/size_of_cluster; // size of array in each process
    int * arr = (int*) malloc(array_size * sizeof(int));

    // scatter the array
    srand(0); // set random seed
    for(int i =0 ; i < process_rank ; i++){
        rand();
    }
    srand(rand()%1000);
    for(int i = 0; i < array_size ; i++){
        arr[i] = rand();
    }


    float start_time, end_time;
    start_time = MPI_Wtime();
    p_bucketsort(MPI_COMM_WORLD, arr, 0, array_size-1);
    // s_quicksort(arr,0,array_size-1);
    // int pivots[3] = {541336356,1073738567,1609637829};
    // int* send_counts = get_counts(arr,0,array_size-1,pivots,4-1);
    // for(int i = 0 ; i < 4 ; i++){
    //     printf("%d ",send_counts[i]);
    // }
    // printf("\n");
    MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();


    if (process_rank == 0) {
        printf("%f ",end_time-start_time);
    }
    MPI_Finalize();          // cleans up the MPI environment and ends MPI communications
    return 0;
}



// int main(int argc, char** argv){
//     int pivots[3] = {1,5,10};
//     int np = 3;
//     int arr[10] = {1,2,3,4,5,6,7,8,9,10};
//     int* counts = get_counts(arr,0,10-1,pivots,np);
//     for(int i = 0 ; i < np ; i++){
//         printf("%d ",counts[i]);
//     }
//     printf("\n");
//     printf("%d ",find_index(arr,0,10-1,5));
//     printf("%d ",find_index(arr,0,10-1,1));
//     printf("%d ",find_index(arr,0,10-1,13));
//     printf("%d ",find_index(arr,0,10-1,-12));
// }


    // s_quicksort(arr,0,array_size-1);
    // assert(validate_array(arr,array_size));
    // MPI_Finalize();          // cleans up the MPI environment and ends MPI communications
    // return 0;