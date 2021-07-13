import argparse
import sys
import subprocess
import os
import shutil
import random

import numpy as np
import pickle

def main():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    # general/data
    parser.add_argument('--input-width', type=int, default=32, metavar='N',
                        help='linear dimension of model')
    #parser.add_argument('--n-train', type=int, default=10000, metavar='N',
    #                    help='number of training samples')

    parser.add_argument('--data-folder', type=str, default='/home/chung/work.0030/datasets', metavar='S',
                        help='folder that store training data set')


    parser.add_argument('--equil', type=int, default=4)
    parser.add_argument('--qunch', type=int, default=1)
    parser.add_argument('--nconf', type=int, default=4)
    parser.add_argument('--decor', type=int, default=1)


    parser.add_argument('--k0', type=int, default=4, metavar='N') # integral part
    parser.add_argument('--k1', type=int, default=0, metavar='N') # decimal part to two significant in our convention


    parser.add_argument('--ncpus', type=int, default=10)

    args = parser.parse_args()

    randint = random.randint(0, 100000)
    exe_name = 'gen.{}'.format(randint)

    data_folder = "{}/{}-{}-{}-{}".format(args.data_folder, args.equil, args.qunch, args.nconf, args.decor)
    # generate data
    if not os.path.isdir(data_folder):
        os.makedirs(data_folder)


    temperature = args.k0+args.k1/100
    print("temperature", temperature)
    try:
        subprocess.call(['g++', '-std=c++11', '-O2',
                         '-DL=%d' % args.input_width,
                         '-DEQUIL=%d' % 10**args.equil, 
                         '-DQUNCH=%d' % 10**args.qunch, 
                         '-DNCONF=%d' % 10**args.nconf, 
                         '-DDECOR=%d' % 10**args.decor, 
                         '-DK0={}'.format(args.k0),
                         '-DK1={}'.format(args.k1),
                         '-DK={}'.format(temperature),
                         'src/metro_replica.cpp', '-o', exe_name])
    except OSError:
        print('... compilation failed', file=sys.stderr)
        sys.exit()
    else:
        print('... compilation successiful', file=sys.stderr)


    command = 'seq {} | xargs -I{{}} -n 1 -P {} ./{} {} {{}} '.format(
                args.ncpus, args.ncpus, exe_name, data_folder)
    os.system(command)

    print('... data generated', file=sys.stderr)
    os.remove(exe_name)


if __name__ == '__main__':
    main()
