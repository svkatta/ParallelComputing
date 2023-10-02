#!/bin/sh
mpic++ main.cpp -o ./a.out
# for proc in 2 4 8 16 32
for proc in 2 
do
    for ((i=1000;i<=100000;i = i +1000)); 
    do
        mpirun -np $proc ./a.out $i
    done
done
rm -rf ./a.out 