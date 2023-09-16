CC = gcc
CFLAGS = -Wall -std=c99 -g

# Create gq
gq: input.o queue.o types.o

# Create input
input.o: input.c input.h

# Create types
types.o: types.c types.h queue.h

# Create queue
queue.o: queue.c queue.h

# Cleanup
clean:
	rm -f *.o
	rm -f *.exe
	rm -f output.txt
	rm -f stderr.txt
