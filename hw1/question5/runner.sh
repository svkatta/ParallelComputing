#!/bin/sh
g++ *.cpp -o ./a.out
for j in 0 1
do
    for i in 100 300 500 700 1000
    do
        ./a.out $j $i
    done
done
rm -rf a.out