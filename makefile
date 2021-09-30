# defining vars
CC=gcc 				# compiler

CFLAGS=-Wall -O 	# compiling flags
LDFLAGS=

EXEC=app			# name of the app

LIB=lib/

all: $(EXEC)

# normal app
app: app.o $(LIB)maze/maze.c $(LIB)mazeDisplay/mazeDisplay.c $(LIB)mazeGenerator/mazeGenerator $(LIB)BetterTerminal/betterTerminal.c
	$(CC) -o app app.o $(LIB)maze/maze.c $(LIB)mazeDisplay/mazeDisplay.c $(LIB)mazeGenerator/mazeGenerator $(LIB)BetterTerminal/betterTerminal.c $(LDFLAGS)
	make clean

# make file for TDD
test: test.o $(LIB)maze/maze.c $(LIB)minunit/minunit.h $(LIB)mazeDisplay/mazeDisplay.c $(LIB)mazeGenerator/mazeGenerator.c $(LIB)BetterTerminal/betterTerminal.c
	$(CC) -lrt -lm -o test test.o $(LIB)maze/maze.c $(LIB)minunit/minunit.h $(LIB)mazeDisplay/mazeDisplay.c $(LIB)mazeGenerator/mazeGenerator.c $(LIB)BetterTerminal/betterTerminal.c $(LDFLAGS)
	make clean

# compiling c mains
test.o: test.c 
	$(CC) test.o -c test.c $(CFLAGS)

app.o: app.c
	$(CC) app.o -c app.c $(CFLAGS)

# utils
clean:
	rm *.o