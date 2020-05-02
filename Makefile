#
# Builds the Cache Lab simulators and tools
# Note: requires a 64-bit x86-64 system
#
CC = gcc
CFLAGS = -g -Wall -Werror -std=c99 -m64

all: test-mm tracegen

test-mm: test-mm.c mm.o mm_ref.o cachelab.c cachelab.h
	$(CC) $(CFLAGS) -o test-mm test-mm.c cachelab.c mm.o mm_ref.o

mm.o: mm.c
	$(CC) $(CFLAGS) -O0 -c mm.c

mm_ref.o: mm_ref.c
	$(CC) $(CFLAGS) -O0 -c mm_ref.c

tracegen: tracegen.c mm.o mm_ref.o cachelab.c
	$(CC) $(CFLAGS) -O0 -o tracegen tracegen.c mm.o mm_ref.o cachelab.c

handin:
	cp -p mm.c turnin/${USER}-mm.c 

#
# Clean the src dirctory
#
clean:
	rm -rf *.o
	rm -rf *.tmp
	rm -f test-mm
	rm -f tracegen
	rm -f .csim_results .marker
