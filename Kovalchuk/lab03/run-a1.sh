#!/bin/bash
#PBS -S /bin/bash
#PBS -l nodes=1:ppn=2,walltime=00:05:00
#PBS -o /home/localusers/ip32/ip3212/labs/Kovalchuk/lab03/logs/a1.out
#PBS -e /home/localusers/ip32/ip3212/labs/Kovalchuk/lab03/logs/a1.err
cd /home/localusers/ip32/ip3212/labs/Kovalchuk/lab03
export OMP_NUM_THREADS=4

export MPIEXEC="/opt/ompi/1.3.3/gcc/4.1/bin/mpiexec"
"$MPIEXEC" ./build/a1 5
