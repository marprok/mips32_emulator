CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99

mips32: main.o
	$(CC) -o mips32 main.o
main.o: main.c
	$(CC) $(CFLAGS) -c main.c
clean:
	rm *.o mips32
