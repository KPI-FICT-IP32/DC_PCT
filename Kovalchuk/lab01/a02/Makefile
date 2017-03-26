CC = mpicc

CCFLAGS = -W -Wall -O2 -std=c99

SRC = \
    a2_numcalc.c \
    a2_funcs.c

ENTRY = a2_numcalc.c

DEFS_SINH = -DOUTFNAME='"out/sinh.out.txt"' -Dcalc_series_term=sinh_series_term
DEFS_SIN = -DOUTFNAME='"out/sin.out.txt"' -Dcalc_series_term=sin_series_term
DEFS_COSH = -DOUTFNAME='"out/cosh.out.txt"' -Dcalc_series_term=cosh_series_term
DEFS_COS = -DOUTFNAME='"out/cos.out.txt"' -Dcalc_series_term=cos_series_term

BIN = a02

all: build/sinh build/cosh build/sin build/cos
# all: build/sinh

build:
	mkdir build

build/sinh: build $(SRC)
	$(CC) $(CCFLAGS) $(DEFS_SINH) $(ENTRY) -o build/a02_sinh

build/cosh: build $(SRC)
	$(CC) $(CCFLAGS) $(DEFS_COSH) $(ENTRY) -o build/a02_cosh

build/sin: build $(SRC)
	$(CC) $(CCFLAGS) $(DEFS_SIN) $(ENTRY) -o build/a02_sin

build/cos: build $(SRC)
	$(CC) $(CCFLAGS) $(DEFS_COS) $(ENTRY) -o build/a02_cos

clean:
	rm -rf build/

.PHONY: all clean
