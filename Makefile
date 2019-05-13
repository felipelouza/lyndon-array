CC = gcc
CFLAGS += -Wall 
#CFLAGS += -g -O0
CFLAGS += -D_FILE_OFFSET_BITS=64 -m64 -O3 -fomit-frame-pointer -Wno-char-subscripts 

LFLAGS = -lm -lrt -ldl

INPUT = dataset/input.100.txt
ALG = 10

LIBOBJ = \
	lib/utils.o\
	lib/file.o\
	lib/bwt.o\
	lib/suffix-array.o\
	lib/lyndon-array.o\
	external/gsaca_cl/gsaca.o\
	external/malloc_count/malloc_count.o\
	sacak-lyndon.o
	
##
M64 = 0
DEBUG = 0
PERMUTED = 0
STEP_TIME = 1
CHECK = 1
SAVE_SPACE = 1
COMPUTE_BWT = 1
##

DEFINES = -DDEBUG=$(DEBUG) -DM64=$(M64) -DPERMUTED=$(PERMUTED) -DSTEP_TIME=$(STEP_TIME)  -DSAVE_SPACE=$(SAVE_SPACE) -DCOMPUTE_BWT=$(COMPUTE_BWT)

CFLAGS += $(DEFINES)

all: main remap

clean:
	\rm -f *.o ../*.o ../external/*.o lib/*o main remap

main: main.c ${LIBOBJ} 
	$(CC) -o main main.c ${LIBOBJ} $(CFLAGS) $(LFLAGS) 

remap: remap.c
	$(CC) -o $@ $< -O -g -Wall -lm 

run:
	./main $(INPUT) -A $(ALG)

valgrind:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes ./main $(INPUT) -A $(ALG)
