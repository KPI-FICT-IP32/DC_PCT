#!/usr/bin/env python
import logging

from math import cos, log as ln

from mpi4py import MPI


log = logging.getLogger(__name__)


def func(x):
    return x * cos(ln(x))


def check_runge(i2, i, epsilon):
    return abs(i2 - i) / 3.0 < epsilon


def integrate(func, start, end, precission):
    num_iterations = 1
    last_res = 0
    res = -1
    step = 0

    while not check_runge(res, last_res, precission):
        num_iterations *= 2
        last_res = res
        res = 0
        step = (end - start) / num_iterations
        for i in range(num_iterations):
            # res += step * func(end - i * step)
            res += func(end - i * step) * step

    return res


def read_file(filename):
    with open(filename, 'r') as f:
        (start, end, precission) = map(float, f.readline().split())
    return (start, end, precission)


def write_file(filename, data):
    with open(filename, 'w') as f:
        print("%lg\n" % data, end='', file=f)


if __name__ == '__main__':
    comm = MPI.COMM_WORLD
    np = comm.Get_size()
    rank = comm.Get_rank()

    input_ = None
    if rank == 0:
        try:
            input_ = read_file('input.txt')
        except Exception as e:
            log.exception(e)
            comm.Abort(1)

    start, end, precission = comm.bcast(input_, root=0)

    step = (end - start) / np
    res = integrate(
        func=func,
        start=start + rank * step,
        end=start + (rank + 1) * step,
        precission=precission / np,
    )

    if rank != 0:
        comm.send(res, dest=0)

    if rank == 0:
        requests = [comm.irecv(source=i) for i in range(1, np)]
        results = MPI.Request.waitall(requests)
        res += sum(results)
        write_file('output.txt', res)
