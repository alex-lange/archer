#!/bin/bash

# get all primes
data=/home/alex/research/data/fv445/19_2

for n in 19
do
    for t in {1..1000}
    do
	./genERGraphs ${n} ${t}
	sleep 2
    done
done
