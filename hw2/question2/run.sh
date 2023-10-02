#!/bin/sh
g++ *.cpp -o ./a.out
echo "ring"
for ((i=8;i<=30;i=i+1)); 
do
    ./a.out 0 $i -1
done
echo "Mesh k=0"
for ((i=4;i<=10;i=i+2)); 
do
    ./a.out 1 $i 0
done
echo "Mesh k=4"
for ((i=4;i<=10;i=i+2)); 
do
    ./a.out 1 $i 4
done
echo "Mesh k=16"
for ((i=4;i<=10;i=i+2)); 
do
    ./a.out 1 $i 16
done
echo "Tree k=0"
for ((i=2;i<=6;i=i+1)); 
do
    ./a.out 2 $i 0
done
echo "Tree k=4"
for ((i=2;i<=6;i=i+1)); 
do
    ./a.out 2 $i 4
done
echo "Tree k=16"
for ((i=2;i<=6;i=i+1)); 
do
    ./a.out 2 $i 16
done
rm -rf ./a.out 