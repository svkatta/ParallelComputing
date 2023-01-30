#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

static int a[10000][10000];
static int b[10000];
static int result[10000];

int routine(int n){
    int i,j;
    struct timeval start, end;
    
    gettimeofday(&start, NULL);
    for (i = 0; i < n ; i ++){
        result [ i ] = 0;
    }
    for ( i = 0; i < n ; i ++){
        for ( j = 0; j < n ; j ++){
            result [ i ] += a [ i ][ j ]* b [ j ];
        }
    }
    gettimeofday(&end, NULL);
    return ((end.tv_sec-start.tv_sec) * 1000000 + end.tv_usec-start.tv_usec );
}

int routinerev(int n){
    int i,j;
    struct timeval start, end;
    
    gettimeofday(&start, NULL);
    for (i = 0; i < n ; i ++){
        result [ i ] = 0;
    }
    for ( j = 0; j < n ; j ++){
        for ( i = 0; i < n ; i ++){
            result [ i ] += a [ i ][ j ]* b [ j ];
        }
    }
    gettimeofday(&end, NULL);
    return ((end.tv_sec-start.tv_sec) * 1000000 + end.tv_usec-start.tv_usec );
}


int main(int argc, char *argv[]) {

    int rrev = atoi(argv[1]);
    int inp = atoi(argv[2]);

    int timetaken ;
    if(rrev == 0){
        timetaken = routine(inp);
        printf("time taken %d \n",timetaken);
    }else{
        timetaken = routinerev(inp);
        printf("time taken in loops reversed %d \n",timetaken);
    }
    
    return 0;
}



