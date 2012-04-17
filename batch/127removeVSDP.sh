#!/bin/bash

data_dir="/home/$USER/research/data/127-v"

for file in $data_dir/*.dat-s
do
    echo "Solving ${file}"
    sdpa -ds ${file} -o ${file}.out
done
