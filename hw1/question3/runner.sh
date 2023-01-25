#!/bin/sh
g++ *.cpp -o ./a.out
for i in 100 1000 10000 100000 1000000 10000000
do
    ./a.out $i
done