CC = gcc
CFLAGS = -Wall -std=c99 -g

# Default target
all: infix_10 infix_12

# Create infix_10
infix_10: infix.o number_10.o operation.o
	gcc infix.o number_10.o operation.o -o infix_10

number_10.o: number_10.c number.h operation.h

# Create infix_12
infix_12: infix.o number_12.o operation.o
	gcc infix.o number_12.o operation.o -o infix_12

number_12.o: number_12.c number.h operation.h

# Common
infix.o: infix.c operation.h number.h
operation.o: operation.c operation.h

# Cleanup
clean:
	rm -f *.o
	rm -f *.exe
	rm -f output.txt