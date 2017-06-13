#!/bin/bash
### Job name
#PBS -N countsort
### Output files 
#PBS -e countsort.err
#PBS -o countsort.log
### Mail to user
#PBS  -M oliversumari@gmail.com
### Queque name (small, medium, long, verylong) batch is default queue
#PBS -q batch
#PBS -l nodes=1:ppn=16
./countsort