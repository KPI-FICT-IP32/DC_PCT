CC = mpicc

CCFLAGS = -Wall -O2 -std=c99

SRC = lab02a02_mpi_int_nonblock.c
	
BIN = integrate-nonblock

all: build/$(BIN) 

build:
	mkdir build

build/$(BIN): $(SRC) build
	$(CC) $(CCFLAGS) $(SRC) -o build/$(BIN)

clean:
	rm -rf build/

.PHONY: all
