#!/bin/sh
mpic++ main.cpp -o ./a.out
# len=10000000
# for proc in 1 2 4 8 16
# do
#     mpirun -n $proc --map-by node:PE=1  ./a.out $len
#     # mpirun -np $proc   ./a.out $len
#     echo " "
# done
# echo "-----------"
# for proc in 1 2 4 8 16
# do
#     mpirun -n 1 --map-by node:PE=1 ./a.out $len
#     mpirun -n $proc --map-by node:PE=1 ./a.out $len
#     len=$((len*2))
#     echo " "
# done

mpirun -n 1 --map-by node:PE=1 ./a.out 1000000
mpirun -n 2 --map-by node:PE=1 ./a.out 1000000
echo " "

mpirun -n 1 --map-by node:PE=1 ./a.out 4000000
mpirun -n 4 --map-by node:PE=1 ./a.out 4000000
echo " "

mpirun -n 1 --map-by node:PE=1 ./a.out 12000000
mpirun -n 8 --map-by node:PE=1 ./a.out 12000000
echo " "

mpirun -n 1 --map-by node:PE=1 ./a.out 32000000
mpirun -n 16 --map-by node:PE=1 ./a.out 32000000
echo " "


rm -rf ./a.out 
