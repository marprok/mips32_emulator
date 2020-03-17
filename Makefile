CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99 -g

main: main.o mips32.o
	$(CC) -o main main.o mips32.o
main.o: main.c 
	$(CC) $(CFLAGS) -c main.c
mips32.o: mips32.c mips32.h
	$(CC) $(CFLAGS) -c mips32.c
assembler: assembler.o tokenizer.o
	$(CC) assembler.o tokenizer.o -o assembler
assembler.o: assembler.c tokenizer.h
	$(CC) $(CFLAGS) -c assembler.c
tokenizer.o: tokenizer.c tokenizer.h
	$(CC) $(CFLAGS) -c tokenizer.c
clean:
	rm *.o main assembler
