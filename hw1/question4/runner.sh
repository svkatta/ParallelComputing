#!/bin/sh
g++ *.cpp -o ./a.out
for j in 0 1
do
    for i in 1000 10000 
    do
        ./a.out $j $i
    done
done
rm -rf a.out