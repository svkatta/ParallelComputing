#!/bin/sh
g++ *.cpp -o ./a.out
for ((i=1000;i<=10000000;i = i +1000)); 
do
    ./a.out $i
done
rm -rf ./a.out 