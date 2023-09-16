CC = gcc
CFLAGS = -Wall -std=c99 -g

# Default target
ident: ident.o io.o syntax.o

# Create io
io.o: io.c io.h

# Create syntax
syntax.o: syntax.c syntax.h io.h

# Cleanup
clean:
	rm -f *.o
	rm -f *.exe
	rm -f output.txt
	rm -f stderr.txt