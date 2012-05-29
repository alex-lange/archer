#!/bin/bash

# get all primes
primes=`gawk 'BEGIN { RS="\n| " } ; {print $0 }' /home/alex/research/folkman/c++/primes/docs/low_primes2.txt`
data=/home/alex/research/data/all_cer/167
sa=/home/alex/research/maxcut/sa

for n in $primes
do
    for t in {1..10}
    do
	name=${n}_${t}
	echo ${name}
	#./addRandCE ${n} ${data}/log.txt ${data}/${name} | /home/alex/research/archer/archer
	${sa}/maxcut_sa ${n} ${sa}/data/primes/${name}
	sleep 2
    done
done
