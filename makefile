# defining vars
CC=gcc
CFLAGS=-Wall -O
LDFLAGS=
EXEC=app

all: $(EXEC)

# normal app
app: main.o
	$(CC) -c app main.o

# make file for TDD
test: test.o
	$(CC) -o test test.o

# compiling c files
test.o: test.c
	$(CC) test.o -c test.c lib/minunit/minunit.h

main.o: main.c
	$(CC) main.o -c main.c

# utils
clean:
	rm *.o