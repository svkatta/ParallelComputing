#!/bin/sh
g++ main-sub.cpp -pthread -o ./a.out
for j in 10000 100000 1000000 10000000
do
    for ((i=0;i<10;i = i +1)); 
    do
        ./a.out 1 $j
    done
done
for j in 10000 100000 1000000 10000000
do
    for ((i=0;i<10;i = i +1)); 
    do
        ./a.out 16 $j
    done
done
for j in 10000 100000 1000000 10000000
do
    for ((i=0;i<10;i = i +1)); 
    do
        ./a.out 32 $j
    done
done
rm -rf ./a.out 