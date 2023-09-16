CC = gcc
CFLAGS = -Wall -std=c99 -g

# 
# Source
# 

all: encrypt decrypt

# Make encrypt
encrypt: encrypt.o io.o field.o aes.o
	gcc encrypt.o io.o field.o aes.o -o encrypt

encrypt.o: encrypt.c io.h field.h aes.h

# Make decrypt
decrypt: decrypt.o io.o field.o aes.o
	gcc decrypt.o io.o field.o aes.o -o decrypt

decrypt.o: decrypt.c io.h field.h aes.h

# 
# Unit Tests
# 

# Make fieldTest
fieldTest: field.o fieldTest.o
	gcc field.o fieldTest.o -o fieldTest

fieldTest.o: fieldTest.c field.h

# Make aesTest
aesTest: aes.o aesTest.o field.o
	gcc aes.o aesTest.o field.o -o aesTest

aesTest.o: aesTest.c aes.h field.h

# 
# Common
# 

io.o: io.c io.h field.h
aes.o: aes.c aes.h field.h
field.o: field.c field.h

# 
# Cleanup
# 

clean:
	rm -f *.o
	rm -f *.exe
	rm -f output.txt
	rm -f stderr.txt
	rm -f output.dat
