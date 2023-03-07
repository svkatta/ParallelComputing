#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <assert.h>     

bool validate_array(int* arr , int size);
void print_arr(int* arr , int size);


int nthreads =0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int max_threads = 8;


struct arg_struct {
    int* arr;
    int start;
    int stop;
};

void swap(int *arr ,int i ,  int j){
    // printf("swapping %d %d \n",arr[i],arr[j]);
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





void * p_quicksort(void * ptr) {
    arg_struct * args =  (arg_struct *)(ptr);

    if(args->start >= args->stop){
        return 0;
    }

    if(args->stop - args->start < 1000){
        s_quicksort(args->arr, args->start, args->stop);
    }

    bool create_thread=false;
    // if(nthreads<max_threads){
        pthread_mutex_lock( &mutex1 );  // blocking call 
        if(nthreads<max_threads){
            nthreads++;
            create_thread=true;
        }
        pthread_mutex_unlock( &mutex1 );
    // }

    if(!create_thread){
        s_quicksort(args->arr, args->start, args->stop);
        return 0;
    }

    int pivot_index = s_partition(args->arr, args->start, args->stop);
    
    pthread_t thread;
    int  iret;
    struct arg_struct  scargs1 = {args->arr,args->start,pivot_index-1};
    struct arg_struct  scargs2 = {args->arr,pivot_index+1,args->stop};


    iret = pthread_create( &thread, NULL, p_quicksort, (void*) &scargs1);
    // printf("created thread\n");
    p_quicksort((void*) &scargs2);

    pthread_join( thread, NULL);
    return 0;
}


int main(int argc, char *argv[]){
    max_threads = atoi(argv[1]);
    int size = atoi(argv[2]);
    int* arr;
    srand(10);
    arr=  (int*)malloc(size * sizeof(int));
    for( int i = 0; i < size; i += 1) {
        arr[i] = rand();
    }

    pthread_mutex_init(&mutex1, NULL);

    // create thread
    pthread_t thread;
    int  iret;
    struct arg_struct  args = {arr,0,size-1};

    struct timeval start, end;
    gettimeofday(&start, NULL);
    iret = pthread_create( &thread, NULL, p_quicksort, (void*) &args);
    pthread_join( thread, NULL);
    gettimeofday(&end, NULL);
    // printf("Sorting array size:%d , Threads:%d , Time Taken:%ld \n",size,max_threads,(end.tv_sec-start.tv_sec) * 1000 + (end.tv_usec-start.tv_usec)/1000);
    printf("%ld \n",(end.tv_sec-start.tv_sec) * 1000 + (end.tv_usec-start.tv_usec)/1000);

    // s_quicksort(arr,0,size-1);
    assert(validate_array(arr,size));
    assert(nthreads==max_threads);
    // print_arr(arr,size);
    return 0;
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

void print_arr(int* arr , int size){
    for(int i =0 ; i <size ;i++){
        printf("%d ", arr[i]);
    }
    printf(" \n");
}