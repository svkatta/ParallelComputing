#!/bin/sh
mpic++ main.cpp -o ./a.out
for len in 100000 1000000 5000000 10000000
do 
    for proc in 1 2 4 8 16 32
    do
        mpirun -np $proc ./a.out $len
    done
done 
rm -rf ./a.out 
