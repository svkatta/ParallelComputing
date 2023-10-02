#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>


int blockmatmul( int * first , int* second , int * result , int n , int blocksize){
    struct timeval start, end;
    gettimeofday(&start, NULL);

    for(int a = 0 ; a < n/blocksize; a++){
        for (int b=0 ; b <  n/blocksize; b++){
            for (int c=0 ; c <  n/blocksize; c++){
                for (int i = 0; i < blocksize; i++) {
                    for (int j = 0; j < blocksize;j++) {
                        for (int k = 0; k < blocksize;k++) {
                                result[ a*n * blocksize + b*blocksize + i*n+j ] += first[ a*n*blocksize + c*blocksize  + i * n + k ] * second[ c*n*blocksize+ b* blocksize + k*n+j];
                            }
                    }
                }
            }
        }
    }
    gettimeofday(&end, NULL);
    int timetaken = ((end.tv_sec-start.tv_sec) * 1000000 + end.tv_usec-start.tv_usec );
    int computation_rate = (2 * n * n * n)/ timetaken;
    return computation_rate;
}



int matmul( int * first , int* second , int * result , int n){
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                    result[ i* n + j] += first[ i * n + k] * second[ k*n + j];
                }
        }
    }
    gettimeofday(&end, NULL);
    int timetaken = ((end.tv_sec-start.tv_sec) * 1000000 + end.tv_usec-start.tv_usec );
    int computation_rate = (2 * n * n * n)/ timetaken;
    return computation_rate;
}



int main(int argc, char *argv[]) {
    int * first ;
    int* second ;
    int * result;

    int isblock = atoi(argv[1]);
    int matsize = atoi(argv[2]);
    // printf("input argument %d \n",inp);

    first = (int*)malloc(matsize * matsize* sizeof(int));
    second = (int*)malloc(matsize * matsize* sizeof(int));
    result = (int*)malloc(matsize * matsize* sizeof(int));

    int i;
    for ( i = 0; i < matsize * matsize; i += 5) {
        first[i] = rand();
        second[i] = rand();
        result[i] = rand();
    }

    int cr;
    if(isblock){
        cr = blockmatmul(first,second,result,matsize,4);
        printf("Matrix size %d computation_rate block %d \n",matsize,cr);
    }else{
        cr = matmul(first,second,result,matsize);
        printf("Matrix size %d computation_rate %d \n",matsize,cr);
        
    }
    return 0;
}