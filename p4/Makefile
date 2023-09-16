CC = gcc
CFLAGS = -Wall -std=c99 -g

# Default target
schedule: schedule.o input.o catalog.o

# Create input
input.o: input.c input.h

# Create catalog
catalog.o: catalog.c catalog.h input.h

# Cleanup
clean:
	rm -f *.o
	rm -f *.exe
	rm -f output.txt
	rm -f stderr.txt