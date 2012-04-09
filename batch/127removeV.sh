#!/bin/bash

# get all primes
#primes=`gawk 'BEGIN { RS="\n| " } ; {print $0 }' ./primes/primes.txt`

n=127
r=3

for v in {1..40}
do
    for t in 1 2 3 4 5
    do
	./removeRandV ${n} ${r} ${v} ${t} | /home/alex/research/archer/archer
	sleep 2
    done
done
