#!/bin/sh
g++ *.cpp -o ./a.out
for i in 1 2 3 4 5 6 7 8 9 
do
    ./a.out $i
done
rm -rf ./a.out 