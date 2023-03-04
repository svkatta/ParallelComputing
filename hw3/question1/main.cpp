#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void validate_array(int* arr , int size);
void print_arr(int* arr , int size);
int nthreads =0;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;


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

int partition(int *arr, int low, int high) {
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
        int pivot_index = partition(arr, low, high);
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
    pthread_mutex_lock( &mutex1 );  // blocking call 
    if(nthreads<32){
        nthreads++;
        create_thread=true;
    }
    pthread_mutex_unlock( &mutex1 );

    if(!create_thread){
        s_quicksort(args->arr, args->start, args->stop);
        return 0;
    }

    int pivot_index = partition(args->arr, args->start, args->stop);
    
    struct arg_struct  scargs1,scargs2;
    pthread_t thread;
    int  iret;

    scargs1.arr = args->arr;
    scargs1.start = args->start;
    scargs1.stop = pivot_index-1;

    scargs2.arr = args->arr;
    scargs2.start = pivot_index+1;
    scargs2.stop = args->stop;

    iret = pthread_create( &thread, NULL, p_quicksort, (void*) &scargs1);
    // printf("created thread\n");
    p_quicksort((void*) &scargs2);

    pthread_join( thread, NULL);
    return 0;
}


int main(){
    int size = 100000;
    int* arr;
    arr=  (int*)malloc(size * sizeof(int));
    for( int i = 0; i < size; i += 1) {
        arr[i] = rand();
    }
    // print_arr(arr,size);

    // int size = 10;
    // int arr[] = {6,1,7,9,13,8,2,5,4,11};

    pthread_mutex_init(&mutex1, NULL);

    pthread_t thread;
    int  iret;

    struct arg_struct  args ;
    args.arr = arr;
    args.start=0;
    args.stop=size-1;

    iret = pthread_create( &thread, NULL, p_quicksort, (void*) &args);
    pthread_join( thread, NULL);


    // s_quicksort(arr,0,size-1);
    validate_array(arr,size);
    // print_arr(arr,size);
    return 0;
}

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