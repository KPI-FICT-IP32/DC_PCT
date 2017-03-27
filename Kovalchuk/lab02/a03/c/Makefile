CC = mpicc

CCFLAGS = -Wall -O2 -std=c99

SRC = lab02a03_mpi_int_collective.c
	
BIN = integrate-collective

all: build/$(BIN) 

build:
	mkdir build

build/$(BIN): $(SRC) build
	$(CC) $(CCFLAGS) $(SRC) -o build/$(BIN)

clean:
	rm -rf build/

.PHONY: all
