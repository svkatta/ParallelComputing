#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

static int a[100000000]; 

int routine(int skip,int * a);

int main(int argc, char *argv[]) {
    
    int inp = atoi(argv[1]);
    // printf("input argument %d \n",inp);

    int i;
    for ( i = 0; i < 100000000; i += 5) {
        a[i] = rand();
    }

    int timetaken ;
    timetaken = routine(inp , a);
    printf("time taken %d \n",timetaken);
    return 0;
}

int routine(int len , int * a){
    int i ,sum ;
    struct timeval start, end;
    
    int sum1 =0 ;
    int sum2 =0;
    gettimeofday(&start, NULL);
    for (i = 0; i <  len; i=i+2) {
        sum1 += a[i];
        sum2 += a[i+2];
    }
    sum = sum1 + sum2;
    gettimeofday(&end, NULL);
    return ((end.tv_sec-start.tv_sec) * 1000000 + end.tv_usec-start.tv_usec );
}