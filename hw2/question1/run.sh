#!/bin/sh
g++ *.cpp -o ./a.out
echo "ring"
for ((i=10;i<=128;i=i+1)); 
do
    ./a.out 0 $i
done
rm -rf ./a.out 