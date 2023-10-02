#!/bin/sh
g++ *.cpp -o ./a.out

echo "ring Mesh k=0"
for ((i=3;i<=20;i=i+1)); 
do
    ./a.out -1 $i 0
done
echo "ring Mesh k=4"
for ((i=3;i<=20;i=i+1)); 
do
    ./a.out -1 $i 4
done
echo "ring Mesh k=16"
for ((i=3;i<=20;i=i+1)); 
do
    ./a.out -1 $i 16
done
rm -rf ./a.out 