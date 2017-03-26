CC = mpicc

CCFLAGS = -Wall -O2 -std=c99

SRC = \
    a01_mpi_first.c
BIN = a01

all: build/$(BIN)

build:
	mkdir build

build/$(BIN): $(SRC) build
	$(CC) $(CCFLAGS) $(SRC) -o build/$(BIN)

clean:
	rm -rf build/

.PHONY: all
