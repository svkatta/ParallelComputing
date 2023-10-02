#!/bin/sh
g++ *.cpp -o ./a.out
for ((i=1;i<=80;i++)); 
do
    ./a.out $i
done
rm -rf ./a.out 