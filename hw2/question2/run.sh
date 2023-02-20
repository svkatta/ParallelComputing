#!/bin/sh
g++ *.cpp -o ./a.out
echo "ring"
for ((i=10;i<=30;i=i+5)); 
do
    ./a.out 0 $i
done
rm -rf ./a.out 