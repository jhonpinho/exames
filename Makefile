CC=gcc-11
CFLAGS=-Wall

all: main

main: main.o exames.o
	$(CC) $(CFLAGS) -o main main.o exames.o

main.o: main.c exames.h
	$(CC) $(CFLAGS) -c main.c

exames.o: exames.c exames.h
	$(CC) $(CFLAGS) -c exames.c

clean:
	rm -f main main.o exames.o
