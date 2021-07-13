#!/bin/bash

##for l in 8 16 32; do
#for l in 64; do
#for i in `seq 0 1 2`; do
#for j in `seq 0 20 80`; do
#    if [ $i == 0 ] && [ $j == 0 ]; then continue; fi
#    echo $l, $i, $j
#    python -u generate.py --input-width $l --k0 $i --k1 $j --equil 4 --qunch 1 --nconf 4 --decor 1
#done
#done
#done

python -u generate.py --input-width 8 --k0 1 --k1 0 --equil 4 --qunch 4 --nconf 1 --decor 1
