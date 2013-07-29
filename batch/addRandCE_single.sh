#!/bin/bash

# get all primes
primes=`gawk 'BEGIN { RS="\n| " } ; {print $0 }' /home/alex/research/folkman/c++/primes/docs/primes_small.txt`
data=/home/alex/research/data/all_cer/small_sdp

for n in $primes
do
    for t in {1..100}
    do
	name=${n}_${t}
	./addRandCE ${n} ${data}/log.txt ${data}/${name} | /home/alex/research/archer/archer
	sleep 2
    done
done

