#!/bin/bash
# test for existence of serial version of 2D Heat Transfer file
  if test -f heat_2d_serial.cpp
  then
    echo "Loading NVIDIA HPC PGI compiler -- disregard circular reference message"
    module load pgi 
    echo "Compiling heat_2d_serial.cpp using the HPC compiler"
    pgc++ -o heat_2d_serial heat_2d_serial.cpp -fast
  else
    if test -f heat_2d_serial.c
    then
      echo "Loading NVIDIA HPC PGI compiler -- disregard circular reference message"
      module load pgi 
      echo "Compiling heat_2d_serial.c using the HPC compiler"
      pgcc -o heat_2d_serial heat_2d_serial.c -fast
    else
      echo "Error: File heat_2d_serial.cpp or heat_2d_serial.c not found!"
      echo "One of these files must be present in current directory before executing this script"
      exit
    fi

  fi

# test for existence of openacc version of 2D Heat Transfer file
  if test -f heat_2d_acc.cpp
  then
    echo "Compiling heat_2d_acc.cpp using the HPC compiler"
     pgc++ -acc -ta=tesla:cc3+ -Minfo=accel -o heat_2d_acc heat_2d_acc.cpp -fast 
  else
    if test -f heat_2d_acc.c
       then
          echo "Compiling heat_2d_acc.c using the HPC compiler"
          pgcc -acc -ta=tesla:cc3+ -Minfo=accel -o heat_2d_acc heat_2d_acc.c -fast 
    else
       echo "Error: File heat_2d_acc.cpp or heat_2d_acc.c not found!"
       echo "One of these files must be present in current directory before executing this script"
       exit
    fi

  fi
chmod 755 *.sh
# Serial Reference Run
# Queue Name: class
# Number of cores: 1
# Time Limit: 00:20:00 HH:MM:SS
# Memory limit: 18gb
# Nome of job: heat2dserial
# DMC Node Architecture Constraint: skylake
echo -e "Scheduling Serial Job on Single Skylake class of Intel Processing Core\n"
echo -e "class\n1\n00:20:00\n18gb\nheat2dserial\nskylake\n" | run_script heat_2d_serial_dmc.sh >heat_2d_acc_dmc.txt

# NVIDIA kelper class GPU Run
# Queue Name: class
# Number of CPU cores: 1 
# Time Limit: 00:20:00 HH:MM:SS
# Memory limit: 18gb
# Number of GPUs: 1
# Nome of job: heat2dkepler
echo -e "Scheduling OpenACC Job on Kepler class of NVIDIA GPU\n"
echo -e "class\n1\n00:20:00\n18gb\n1\nkepler\nheat2dkepler\n" | run_gpu heat_2d_acc_kepler_dmc.sh >> heat_2d_acc_dmc.txt 
#
# NVIDIA pascal class GPU Run
# Queue Name: class
# Number of CPU cores: 1 
# Time Limit: 00:20:00 HH:MM:SS
# Memory limit: 18gb
# Number of GPUs: 1
# Nome of job: heat2dpascal
echo -e "Scheduling OpenACC Job on Pascal class of NVIDIA GPU\n"
echo -e "class\n1\n00:20:00\n18gb\n1\npascal\nheat2dpascal\n" | run_gpu heat_2d_acc_pascal_dmc.sh >> heat_2d_acc_dmc.txt 
#
# NVIDIA volta class GPU Run
# Queue Name: class
# Number of CPU cores: 1 
# Time Limit: 00:20:00 HH:MM:SS
# Memory limit: 18gb
# Number of GPUs: 1
# Nome of job: heat2dvolta
echo -e "Scheduling OpenACC Job on Volta class of NVIDIA GPU\n"
echo -e "class\n1\n00:20:00\n18gb\n1\nvolta\nheat2dvolta\n" | run_gpu heat_2d_acc_volta_dmc.sh >> heat_2d_acc_dmc.txt 
#
squeue
