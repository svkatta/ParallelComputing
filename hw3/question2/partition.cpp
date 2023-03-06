#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <bits/stdc++.h>
using namespace std;

#define MAX_THREADS 16

int * arr;


struct qthread
{
    int id;              // 1 to 16
    pthread_t thread;    // thread identifier used to create
    int i ;
    int j;
    pthread_mutex_t mutex;
    
    
    int niwrites;                //0
    pthread_cond_t cond; //= PTHREAD_COND_INITIALIZER;
};
struct qthread p_threads[16];

struct arg_struct{
    int id;
};

int get_thread_to_write(int k){
    // take 2's and add 1 to remaining odd.
    int curr = 1;
    while(k %2 ==0){
        k = k /2;
        curr = curr*2;
    }

    return min((k+1)*curr,MAX_THREADS)-1;
}
int waitill(int k){
    // take 2's and add 1 to remaining odd.
    int curr = 0;
    while(k %2 ==0){
        k = k /2;
        curr += 1;
    }

    return curr;
}


void * partition(void *ptr) {
    arg_struct * args =  (arg_struct *)(ptr);
    int id = args->id+1;
    int idx = get_thread_to_write(id);
    // printf("inside thread id %d %d %d\n",id,idx,p_threads[id].niwrites);

    // wait till nwrites
    pthread_mutex_lock(&p_threads[args->id].mutex);
    while (p_threads[args->id].niwrites >0) {
        pthread_cond_wait(&p_threads[args->id].cond, &p_threads[args->id].mutex);
    }
    pthread_mutex_unlock(&p_threads[args->id].mutex);

    // printf("out of waiting thread id %d \n",id);

    pthread_mutex_lock(&p_threads[idx].mutex);
    p_threads[idx].i +=p_threads[args->id].i;
    p_threads[idx].niwrites -= 1;
    pthread_mutex_unlock(&p_threads[idx].mutex); 
    pthread_cond_signal(&p_threads[idx].cond); 
    // printf("final thread id %d %d %d\n",id,p_threads[id-1].j,p_threads[id-1].niwrites);
    return 0;
}


int main(){


    for(int i =0 ; i<16 ;i++){
        printf(" %2d %2d %2d \n",i+1,get_thread_to_write(i+1)+1,waitill(i+1));
    }

    arr= (int*)malloc(16 * sizeof(int));

    for(int i =0 ; i<16 ;i++){
        printf(" %2d ",i+1);
    }
    printf("\n");

    for(int i=0;i<16;i++){
        p_threads[i] = {i+1,0,rand()%10,0,PTHREAD_MUTEX_INITIALIZER,waitill(i+1),PTHREAD_COND_INITIALIZER};
        printf(" %2d ",p_threads[i].i);
    }
    printf("\n");

    // arg_struct args;
    int iret;
    arg_struct args[16];
    for(int i =0 ; i<16 ;i++){
        args[i] = {i};
        iret = pthread_create( &p_threads[i].thread, NULL, partition, (void*) &args[i]);
    }

    for(int i =0 ; i<16 ;i++){
        pthread_join( p_threads[i].thread, NULL);
    }



    for(int i =0 ; i<16 ;i++){
        printf(" %2d ",p_threads[i].i);
    }
    printf("\n");


    // for(int i =0 ; i<16 ;i++){
    //     printf(" %2d ",p_threads[i].i);
    // }
    // printf("\n");


}