#!/bin/bash

data_dir="/home/$USER/research/data/all_cer/167"
mc_dir="/home/$USER/software/optimization/SDP-LR_v0.130301"

for file in $data_dir/*.rdy
do
    echo "Solving ${file}"
    ${mc_dir}/maxcut ${file} ${mc_dir}/p.maxcut.arl > ${file}.out
done
