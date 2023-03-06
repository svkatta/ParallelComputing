#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*

arr 

temp arr


*/


int* arr;
int* tarr;
int* pos;


// utility helper functions
void validate_array(int* arr , int size){
    printf("created threads %d\n",nthreads);
    int current = arr[0];
    for(int i =0 ; i <size ;i++){
        if(arr[i] < current){
            printf("Not sorted array\n");
            return;
        }
    }
    printf("Sorted array\n");
}

void print_arr(int* arr , int size){
    for(int i =0 ; i <size ;i++){
        printf("%d ", arr[i]);
    }
    printf(" \n");
}

void swap(int *arr ,int i ,  int j){
    // printf("swapping %d %d \n",arr[i],arr[j]);
    int temp =arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

// serial partition function
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
// serial quick sort function
void s_quicksort(int * arr, int low, int high){ // [low , high]
    if (low < high) {
        int pivot_index = s_partition(arr, low, high);
        s_quicksort(arr, low, pivot_index - 1);
        s_quicksort(arr, pivot_index + 1, high);
    }
}


// parallel quick sort
#define MAX_THREADS 512
int nthreads =0;

struct qthread
{
    int id;              // 1 to 16
    pthread_t thread;    // thread identifier used to create
    int start;           // which part of array they are working on 
    int stop;

    int piviot ;
};



struct qsort_args {
    int start;
    int stop;
    int ava_threads;
};




//----------
int get_thread_to_write(int k){
    // take 2's and add 1 to remaining odd.
    int curr = 1;
    while(k %2 ==0){
        k = k /2;
        curr = curr*2;
    }

    return (k+1)*curr;
}


int partition(int *ptr) {

    arg_struct * args =  (arg_struct *)(ptr);
    
    // mark positions using piviot
    int sum =0;
    for (int i = args->low ;  i <= args->high; i++) {
        if (arr[i] >= pivot) {
            positions[i] = 1  ;
            sum++;
        }else{
            positions[i] = 0  ;
        }
    }
    // find cummulative sum

    // swap elements
    for (int i = args->low ;  i <= args->high; i++) {
        
        sum = sum + pos[i] + 
        pos[i] = 1  ;
        
        
        
        
    }


    // wait till all threads exchange
    pthread_barrier_wait(args->barrier);  
    for(int i = args->low ;  i <= args->high; i++) {
        arr[i] = tarr[i];
    }
    // return piviot index

    return i + 1;
}




void * p_quicksort(void * ptr) {
    qsort_args * args =  (qsort_args *)(ptr);

    if(args->start >= args->stop){
        return 0;
    }

    if(args->stop - args->start < 1000 || avaliablethreads ==1){
        s_quicksort(args->arr, args->start, args->stop);
    }



    // generate threads to do partition
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, nullptr, args->ava_threads);


    int pivot_index = partition(args->start, args->stop);


    // according to piviot_index divide no of avaliable threads
    pthread_t thread;
    int  iret;
    
    struct qsort_args  scargs1 = {args->start,pivot_index-1,};
    struct qsort_args  scargs2 = {pivot_index+1,args->stop,};

    iret = pthread_create( &thread, NULL, p_quicksort, (void*) &scargs1);
    p_quicksort((void*) &scargs2);

    pthread_join( thread, NULL);
    return 0;
}




// -----------------------------Main----------------------------- 
int main(){
    // array creation
    int size = 100000;
    // int* arr;
    // int* tarr;
    arr=  (int*)malloc(size * sizeof(int));
    tarr=  (int*)malloc(size * sizeof(int));
    for( int i = 0; i < size; i += 1) {
        arr[i] = rand();
    }
    // print_arr(arr,size);

    // // Thread creation and initlization
    // pthread_t p_threads[MAX_THREADS];
    // for (i=0; i< MAX_THREADS; i++) {
    //     pthread_mutex_init(&p_threads[i], NULL);
    // }


    // create thread
    pthread_t thread;
    int  iret;
    struct qsort_args  args = {0,size-1,16};

    iret = pthread_create( &thread, NULL, p_quicksort, (void*) &args);
    pthread_join( thread, NULL);


    // s_quicksort(arr,0,size-1);
    validate_array(arr,size);
    // print_arr(arr,size);
    return 0;
}