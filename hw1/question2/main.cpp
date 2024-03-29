#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

int routine(int skip,int * a);

int main(int argc, char *argv[]) {
    int * ptr;
    int inp = atoi(argv[1]); // skip len
    // printf("input argument %d \n",inp);

    ptr=  (int*)malloc(inp * 10000000* sizeof(int));

    int i;
    for ( i = 0; i < inp * 10000000; i += 5) {
        ptr[i] = rand();
    }

    int timetaken ;
    timetaken = routine(inp , ptr);
    printf("%d \n",timetaken);
    return 0;
}

int routine(int skip , int * a){
    int length=10000000;
    int i ,sum ;
    struct timeval start, end;
    
    sum =0 ;
    gettimeofday(&start, NULL);
    for (i = 0; i < skip * length; i += skip) {
        sum += a[i];
    }
    gettimeofday(&end, NULL);
    return ((end.tv_sec-start.tv_sec) * 1000000 + end.tv_usec-start.tv_usec );
}