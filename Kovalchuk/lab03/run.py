#!/usr/bin/env python
from __future__ import division
from __future__ import print_function

import csv
import sys
import subprocess

from collections import defaultdict
from os import path


THREADS = [1,2,4]
SIZES = (
    list(range(100, 5000, 100))
#    list(range(250, 3000, 250)) 
#    + list(range(3000, 1000, 10000))
#    + list(range(10000, 1000000, 20000))
#    + list(range(1000000, 10000000, 500000))
)
ITERATIONS = 50


def avg(arr):
    return sum(arr) / len(arr)


def write_result(filename, data):
    f = open(filename, 'w')
    try:
        writer = csv.writer(f, delimiter=';', quoting=csv.QUOTE_MINIMAL)
        
        # header
        writer.writerow([''] + [str(x) + ' thread(s)' for x in THREADS])
        
        # data rows
        for s in SIZES:
            writer.writerow([s] + [data[t][s] for t in THREADS])
    finally:
        f.close()


def print_progressbar(percentage):
    header = '\r{:7.2f}%'.format(percentage)

    bars_count = int(percentage // 2)
    spaces_count = 50 - bars_count - 1
    bar = (
        '=' * bars_count 
        + ('>' if spaces_count >= 0 else '') 
        + ' ' * spaces_count
    )

    progress_bar = '{header} [{bar}]'.format(header=header, bar=bar)
    print(progress_bar, end='')


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('\e]31mUsage: run.py <progname>\e]39m', file=sys.stderr)
        sys.exit(1)

    prog_name = sys.argv[1]
    executable = path.join('build', prog_name)
    
    total_runs = len(SIZES) * ITERATIONS
    
    measures = defaultdict(dict)
    for num_threads in THREADS:
        print('\nNum threads: {}'.format(num_threads))
        run_count = 0
        for size in SIZES:
            times = []
            for iteration in range(ITERATIONS):
                run_count += 1
                print_progressbar(run_count / total_runs * 100) 
                proc = subprocess.Popen(
                    [executable, '1', '1', str(size), str(num_threads)],
                    stdout=subprocess.PIPE,
                )
                
                while True:
                    line = proc.stdout.readline()
                    if line != b'':
                        if line.decode('utf-8').startswith('Time: '):
                            times.append(float(line[len('Time: '):-1]))
                    else:
                        break

            measures[num_threads][size] = avg(times)    

    filename = '{}.csv'.format(prog_name)
    print('\nDone measuring. writing results to {}'.format(filename))
    write_result(filename, measures)


# vim: ts=4 sw=4 sts=4 tw=0 et :
