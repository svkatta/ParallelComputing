#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

static int a[10000000]; 

int routine1(int skip,int * a);
int routine2(int skip,int * a);
int routine3(int skip,int * a);
int routine4(int skip,int * a);
int routine5(int skip,int * a);
int routine6(int skip,int * a);

int main(int argc, char *argv[]) {
    
    int inp = atoi(argv[1]);
    int len = atoi(argv[2]);
    printf("%d %d ",inp,len);
    
    int i,timetaken;

    // for ( i = 0; i < 10000000; i += 5) {
    //     a[i] = rand();
    // }

    
    if (inp ==1){
        timetaken = routine1(len , a);
    }else if (inp == 2)
    {
        timetaken = routine2(len , a);
    }
    else if (inp == 3)
    {
        timetaken = routine3(len , a);
    }
    else if (inp == 4)
    {
        timetaken = routine4(len , a);
    }
    else if (inp == 5)
    {
        timetaken = routine5(len , a);
    }
    else if (inp == 6)
    {
        timetaken = routine6(len , a);
    }
    
    
    printf("time taken %d \n",timetaken);
    return 0;
}

int routine1(int len , int * a){
    int i ,sum ;
    struct timeval start, end;
    
    int sum1 =0 ;
    gettimeofday(&start, NULL);
    for (i = 0; i <  len; i=i+1) {
        sum1 += a[i];
    }
    gettimeofday(&end, NULL);
    return ((end.tv_sec-start.tv_sec) * 1000000 + end.tv_usec-start.tv_usec );
}

int routine2(int len , int * a){
    int i ,sum ;
    struct timeval start, end;
    
    int sum1 =0 ;
    int sum2 =0;
    gettimeofday(&start, NULL);
    for (i = 0; i <  len; i=i+2) {
        sum1 += a[i];
        sum2 += a[i+1];
    }
    sum = sum1 + sum2;
    gettimeofday(&end, NULL);
    return ((end.tv_sec-start.tv_sec) * 1000000 + end.tv_usec-start.tv_usec );
}

int routine3(int len , int * a){
    int i ,sum ;
    struct timeval start, end;
    
    int sum1 =0, sum2 =0 , sum3=0;
    gettimeofday(&start, NULL);
    for (i = 0; i <  len; i=i+3) {
        sum1 += a[i];
        sum2 += a[i+1];
        sum3 += a[i+2];
    }
    sum = sum1 + sum2 + sum3;
    gettimeofday(&end, NULL);
    return ((end.tv_sec-start.tv_sec) * 1000000 + end.tv_usec-start.tv_usec );
}

int routine4(int len , int * a){
    int i ,sum ;
    struct timeval start, end;
    
    int sum1 =0, sum2 =0 , sum3=0, sum4=0;
    gettimeofday(&start, NULL);
    for (i = 0; i <  len; i=i+4) {
        sum1 += a[i];
        sum2 += a[i+1];
        sum3 += a[i+2];
        sum4 += a[i+3];
    }
    sum = sum1 + sum2 + sum3+ sum4;
    gettimeofday(&end, NULL);
    return ((end.tv_sec-start.tv_sec) * 1000000 + end.tv_usec-start.tv_usec );
}

int routine5(int len , int * a){
    int i ,sum ;
    struct timeval start, end;
    
    int sum1 =0, sum2 =0 , sum3=0, sum4=0, sum5=0;
    gettimeofday(&start, NULL);
    for (i = 0; i <  len; i=i+5) {
        sum1 += a[i];
        sum2 += a[i+1];
        sum3 += a[i+2];
        sum4 += a[i+3];
        sum5 += a[i+4];
    }
    sum = sum1 + sum2 + sum3+ sum4+ sum5;
    gettimeofday(&end, NULL);
    return ((end.tv_sec-start.tv_sec) * 1000000 + end.tv_usec-start.tv_usec );
}

int routine6(int len , int * a){
    int i ,sum ;
    struct timeval start, end;
    
    int sum1 =0, sum2 =0 , sum3=0, sum4=0, sum5=0,sum6=0;
    gettimeofday(&start, NULL);
    for (i = 0; i <  len; i=i+6) {
        sum1 += a[i];
        sum2 += a[i+1];
        sum3 += a[i+2];
        sum4 += a[i+3];
        sum5 += a[i+4];
        sum6 += a[i+5];
    }
    sum = sum1 + sum2 + sum3+ sum4+ sum5+ sum6;
    gettimeofday(&end, NULL);
    return ((end.tv_sec-start.tv_sec) * 1000000 + end.tv_usec-start.tv_usec );
}