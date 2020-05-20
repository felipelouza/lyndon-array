CC = g++
CFLAGS += -Wall 
#CFLAGS += -g -O0
CFLAGS += -D_FILE_OFFSET_BITS=64 -m64 -O3 -fomit-frame-pointer -Wno-char-subscripts -std=c++11

LFLAGS = -lm -lrt -ldl

INPUT = ../dataset/input.100.txt
ALG = 1

LIBOBJ = \
	lib/utils.o\
	lib/file.o\
	lib/algorithms.o\
	lib/duval.o\
	../external/malloc_count/malloc_count.o
	
##
M64 = 0
DEBUG = 0
##

DEFINES = -DDEBUG=$(DEBUG) -DM64=$(M64) 

CFLAGS += $(DEFINES)

all: main 

clean:
	\rm -f *.o ../external/*.o lib/*o remap 

main: main.cpp ${LIBOBJ} 
	$(CC) -o remap main.cpp ${LIBOBJ} $(CFLAGS) $(LFLAGS) 

run:
	./remap $(INPUT) -A $(ALG)

valgrind:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes ./remap $(INPUT) -A $(ALG)
