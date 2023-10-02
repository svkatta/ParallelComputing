#!/bin/sh
g++ main.cpp -pthread -o ./a.out
for j in 1 2 4 8 16 32 64 
do
    for ((i=0;i<10;i = i +1)); 
    do
        ./a.out $j 10000000
    done
done
rm -rf ./a.out 