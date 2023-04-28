#!/bin/sh
mpic++ main.cpp -o ./a.out
# len=10000000
# for ((proc=1;proc<=16;proc=proc+1)); 
# do
#     mpirun -np $proc ./a.out $len
#     echo ""
# done
# echo "-----"

# blen=10000000
# for ((proc=1;proc<=16;proc=proc+1)); 
# do  
#     len=$((blen*proc))
#     mpirun -n 1 --map-by node:PE=1 ./a.out $len
#     mpirun -n $proc --map-by node:PE=1 ./a.out $len
#     echo " "
# done

# mpirun -n 1 --map-by node:PE=1 ./a.out 1000000
# mpirun -n 2 --map-by node:PE=1 ./a.out 1000000
# echo " "

mpirun -n 1 --map-by node:PE=1 ./a.out 27000000
mpirun -n 6 --map-by node:PE=1 ./a.out 27000000
echo " "

mpirun -n 1 --map-by node:PE=1 ./a.out 125000000
mpirun -n 10 --map-by node:PE=1 ./a.out 125000000
echo " "


mpirun -n 1 --map-by node:PE=1 ./a.out 216000000
mpirun -n 12 --map-by node:PE=1 ./a.out 216000000
echo " "

mpirun -n 1 --map-by node:PE=1 ./a.out 343000000
mpirun -n 14 --map-by node:PE=1 ./a.out 343000000
echo " "


rm -rf ./a.out 
