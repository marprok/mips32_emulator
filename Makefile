CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99 -g

main: main.o mips32.o
	$(CC) -o main main.o mips32.o
main.o: main.c 
	$(CC) $(CFLAGS) -c main.c
mips32.o: mips32.c mips32.h
	$(CC) $(CFLAGS) -c mips32.c
assembler: assembler.c
	$(CC) $(CFLAGS) assembler.c -o assembler
clean:
	rm *.o main assembler
