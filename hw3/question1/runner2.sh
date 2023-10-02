#!/bin/sh
g++ main.cpp -pthread -o ./a.out
for i in 1 16 32
do
    for j in 10000 100000 1000000 10000000
    do
        ./a.out $i $j
    done
done
rm -rf ./a.out 