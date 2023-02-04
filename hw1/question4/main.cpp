#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

int routine(int n, int * a, int * b, int * result){
    int i,j;
    struct timeval start, end;
    
    gettimeofday(&start, NULL);
    for ( i = 0; i < n ; i ++){
        for ( j = 0; j < n ; j ++){
            result [ i ] += a [ i *n + j ]* b [ j ];
        }
    }
    gettimeofday(&end, NULL);
    return ((end.tv_sec-start.tv_sec) * 1000000 + end.tv_usec-start.tv_usec );
}

int routinerev(int n, int * a, int * b, int * result){
    int i,j;
    struct timeval start, end;
    
    gettimeofday(&start, NULL);
    for (i = 0; i < n ; i ++){
        result [ i ] = 0;
    }
    for ( j = 0; j < n ; j ++){
        for ( i = 0; i < n ; i ++){
            result [ i ] += a [i*n + j]* b [ j ];
        }
    }
    gettimeofday(&end, NULL);
    return ((end.tv_sec-start.tv_sec) * 1000000 + end.tv_usec-start.tv_usec );
}

int main(int argc, char *argv[]) {

    int rrev = atoi(argv[1]);
    int inp = atoi(argv[2]);

    int * a      = (int*)malloc(inp*inp * sizeof(int));
    int * b      = (int*)malloc(inp * sizeof(int)); 
    int * result = (int*)malloc(inp * sizeof(int));

    int i,j;
    for ( i = 0; i < inp*inp; i += 5) {
        a[i] = 1;
    }
    for ( i = 0; i < inp; i += 5) {
        b[i] = rand();
        result[i]= rand();
    }

    int timetaken ;
    if(rrev == 0){
        timetaken = routine(inp,a,b,result);
        printf("time taken %d \n",timetaken);
    }else{
        timetaken = routinerev(inp,a,b,result);
        printf("time taken in loops reversed %d \n",timetaken);
    }
    
    return 0;
}



