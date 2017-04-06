CC=gcc
CC_FLAGS=-Wall -O2 -std=c99 -fopenmp

all: build/a0 build/a1 build/a2 build/a3 build/a4 build/a5

build:
	mkdir build

build/a0: build a0_openmp_num_threads.c
	$(CC) $(CC_FLAGS) a0_openmp_num_threads.c -o build/a0

build/a1: build a1_openmp_get_thread_num.c
	$(CC) $(CC_FLAGS) a1_openmp_get_thread_num.c -o build/a1

build/a2: build a2_openmp_vec_prod_parallel.c
	$(CC) $(CC_FLAGS) a2_openmp_vec_prod_parallel.c -o build/a2

build/a3: build a3_openmp_vec_prod_parallel_reduce.c
	$(CC) $(CC_FLAGS) a3_openmp_vec_prod_parallel_reduce.c -o build/a3

build/a4: build a4_openmp_vec_prod_parallel_critical.c
	$(CC) $(CC_FLAGS) a4_openmp_vec_prod_parallel_critical.c -o build/a4

build/a5: build a5_openmp_vec_prod_parallel_for_reduce.c
	$(CC) $(CC_FLAGS) a5_openmp_vec_prod_parallel_for_reduce.c -o build/a5

clean:
	rm -rf build/

.PHONY: all
.PHONY: clean

# vim: set ts=8 sw=8 sts=8 tw=0 noet :
