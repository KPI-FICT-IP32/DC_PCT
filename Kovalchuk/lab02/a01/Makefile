CC = mpicc

CCFLAGS = -Wall -O2 -std=c99

SRC = lab02a01_mpi_simple_nonblock.c
SRC_BM_NONBLOCK = a01_benchmark_nonblock.c
SRC_BM_BLOCK = a01_benchmark_block.c
SRC_PITFALL = a01_nonblock_pitfall.c
	
BIN = mpi-simple-nonblock
BIN_BM_NONBLOCK = benchmark-nonblock
BIN_BM_BLOCK = benchmark-block
BIN_PITFALL = nonblock-pitfall

all: build/$(BIN) build/$(BIN_BM_BLOCK) build/$(BIN_BM_NONBLOCK) build/$(BIN_PITFALL)

build:
	mkdir build

build/$(BIN): $(SRC) build
	$(CC) $(CCFLAGS) $(SRC) -o build/$(BIN)

build/$(BIN_BM_NONBLOCK): $(SRC_BM_NONBLOCK) build
	$(CC) $(CCFLAGS) -D_POSIX_C_SOURCE=199309L -lrt $(SRC_BM_NONBLOCK) -o build/$(BIN_BM_NONBLOCK)

build/$(BIN_BM_BLOCK): $(SRC_BM_BLOCK) build
	$(CC) $(CCFLAGS) -D_POSIX_C_SOURCE=199309L -lrt $(SRC_BM_BLOCK) -o build/$(BIN_BM_BLOCK)

build/$(BIN_PITFALL): $(SRC_PITFALL) build
	$(CC) $(CCFLAGS) $(SRC_PITFALL) -o build/$(BIN_PITFALL)


clean:
	rm -rf build/

.PHONY: all
