#!/bin/sh
g++ main-sub.cpp -pthread -o ./a.out
# echo "threads used 4 array size 10000000"
# for ((i=0;i<10;i = i +1)); 
# do
#     ./a.out 4 1000000
# done
for j in 1 2 4 8 16 32 64 
do
    for ((i=0;i<10;i = i +1)); 
    do
        ./a.out $j 10000000
    done
done
rm -rf ./a.out 