#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <utility>
using namespace std;

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


//------//
int* arr;                     // main array which contains numbers
int* temp_arr;                // temp_arr used to exchange numbers in parition 
#define MAX_THREADS 16

struct qthread
{
    int id;              // 1 to 16
    pthread_t thread;    // thread identifier used to create
    
    //---------
    pair<int, int> csum;       // this pair is used to communication for cummulative sum
    pthread_mutex_t mutex;
    int niwrites;                //0
    pthread_cond_t cond; //= PTHREAD_COND_INITIALIZER;
};
struct qthread threads[MAX_THREADS];

struct par_args{                     // parition args
    int id;                          // id of thread;
    int id_low;                      // id in [id_low,id_high] are threads working on parition
    int id_high;

    int low;                         // [low,high] part of arr belong to this parition thread
    int high;
    int piviot;                      // piviot element
    int arr_start;                   // start of arr
    pthread_barrier_t * barrier1;    // first barrier
    pthread_barrier_t * barrier2;    // second barrier
    int retval;                      // used to communicate final piviot index
};
struct par_args pargs[MAX_THREADS];

void * p_partition(void * ptr){        // O(P) version
    par_args * args =  (par_args *)(ptr);
    int low = args->low;
    int high = args->high;
    int piviot = args->piviot;
    int arr_start = args->arr_start;
    int i = low - 1;
    pair<int,int> count(0,0);         // no of elements < piviot
    for (int j = low; j <= high ; j++) {
        if (arr[j] < piviot) {
            i++;
            swap(arr,i,j);
            count.first++;
        }else{
            count.second++;
        }
    }
    // printf("threadid: %d count: %d \n",args->id,count);
    // threads[args->id].niwrites = 1;
    // threads[args->id].i = count;


    //  now communication between threads
    pthread_mutex_lock(&threads[args->id].mutex);
    while (threads[args->id].niwrites >0) {
        pthread_cond_wait(&threads[args->id].cond, &threads[args->id].mutex);
    }
    pthread_mutex_unlock(&threads[args->id].mutex);

    if(args->id != args->id_high){
        pthread_mutex_lock(&threads[args->id +1].mutex);
        threads[args->id+1].csum.first += count.first + threads[args->id].csum.first;
        threads[args->id+1].csum.second += count.second + threads[args->id].csum.second; 
        threads[args->id +1].niwrites -= 1;
        pthread_mutex_unlock(&threads[args->id +1].mutex); 
        pthread_cond_signal(&threads[args->id +1].cond); 
    }
    count.first = threads[args->id].csum.first  ;
    count.second = threads[args->id].csum.second  ;
    // printf("threadid: %d count1: %d \n",args->id,count.first);
    // printf("threadid: %d count2: %d \n",args->id,count.second);
    
    //now count has cummlative sum write to temp arr

    for (int j = low; j <= high ; j++) {
        if (arr[j] < piviot) {
            temp_arr[arr_start+count.first] = arr[j] ;
            count.first++;
        }
    }
    threads[args->id].csum.first  = count.first;
    pthread_barrier_wait(args->barrier1);
    //communicate threads
    int before_piviot_count = threads[args->id_high].csum.first;

    for (int j = low; j <= high ; j++) {
        if (arr[j] >= piviot) {
            temp_arr[arr_start+before_piviot_count + count.second] = arr[j] ;
            count.second++;
        }
    }

    pthread_barrier_wait(args->barrier2);

    for (int j = low; j <= high ; j++) {
        arr[j] = temp_arr[j]  ;
    }

    if(args->id == args->id_high){
        args->retval = before_piviot_count+arr_start;
    }


    return 0;
}

struct pqs_args{
    int at_low;          // thread from [low,high] are avaliable for parallel quick sort
    int at_high;
    int start;           // [start,stop]     need to be sorted by parallel quick sort 
    int stop;
};

void * p_quicksort(void * ptr) {                 //[start,stop]
    pqs_args * args =  (pqs_args *)(ptr);

    if(args->start >= args->stop){
        return 0;
    }

    int at_low = args->at_low;
    int at_high = args->at_high;
    int ava_threads = at_high -at_low+1;
    int start = args->start;
    int stop = args->stop;
    int piviot = arr[args->stop];

    if(args->stop - args->start < 3 || ava_threads ==1){
        s_quicksort(arr, args->start, args->stop);
        return 0;
    }

    // printf("pqs Tlow:%d Thigh:%d totalT:%d start:%2d stop:%2d \n", at_low,at_high,ava_threads,start,stop);

    // parallel parition 
    pthread_barrier_t barrier1,barrier2;;
    pthread_barrier_init(&barrier1, nullptr, ava_threads);
    pthread_barrier_init(&barrier2, nullptr, ava_threads);
    
    int step = (stop-start+1)/(ava_threads);
    for(int i =at_low ; i<= at_high ;i++){
        threads[i].niwrites = (i==at_low)? 0 : 1;
        threads[i].csum ={0,0};

        pargs[i] = {i,at_low,at_high,
                    start+(i-at_low)*step,
                    (i!=at_high) ? start+(i-at_low+1)*step-1 : stop,
                    piviot,start,
                    &barrier1,&barrier2,0};
        // printf("created thread %d %d %d \n",pargs[i].low,pargs[i].high,piviot);
        pthread_create(&threads[i].thread, NULL, p_partition, (void*) &pargs[i]);
    }
    
    for(int i =at_low ; i<= at_high ;i++){
        pthread_join(threads[i].thread, NULL); // join threads
    }
    pthread_barrier_destroy(&barrier1); // destroy the barrier object
    pthread_barrier_destroy(&barrier2); // destroy the barrier object

    // printf("---------finished partititon %d \n" , pargs[at_high].retval);
    swap(arr,pargs[at_high].retval,args->stop);

    // left and right quick sort
    pthread_t thread;
    int pivot_index = pargs[at_high].retval;
    // int x = (at_high+ at_low )/(2);
    struct pqs_args  pqsargs1 = {at_low ,   (at_high+ at_low )/(2)    ,start,pivot_index-1};
    struct pqs_args  pqsargs2 = { ((at_high+ at_low )/(2))+1  ,at_high,pivot_index+1,stop};
    int iret = pthread_create( &thread, NULL, p_quicksort, (void*) &pqsargs1);
    p_quicksort((void*) &pqsargs2);

    pthread_join( thread, NULL);
    return 0;
}


int main(){
    int size = 1000000;

    // Array Creation 
    temp_arr=  (int*)malloc(size * sizeof(int));
    arr=  (int*)malloc(size * sizeof(int));
    for( int i = 0; i < size; i += 1) {
        arr[i] = rand()%30;
    }
    // for(int i =0 ; i <size ;i++){
    //     printf("%2d ", i);
    // }
    // printf("\n");
    // print_arr(arr,size);
    //--------------------


    // 
    struct pqs_args  args = {0,3,0,size-1};
    p_quicksort((void*) &args);

    // print_arr(arr,size);
    // print_arr(temp_arr,size);
    // for(int i =0 ; i <MAX_THREADS ;i++){
    //     printf("%2d ", threads[i].csum.second);
    // }
    // printf(" \n");

    // print_arr(arr,size);

    // // s_quicksort(arr,0,size-1);
    validate_array(arr,size);
    // // print_arr(arr,size);
    return 0;
}

void validate_array(int* arr , int size){
    // printf("created threads %d\n",nthreads);
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
        printf("%2d ", arr[i]);
    }
    printf(" \n");
}