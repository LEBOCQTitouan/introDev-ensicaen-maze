# defining vars
CC=gcc 				# compiler

CFLAGS=-Wall -O 	# compiling flags
LDFLAGS=

EXEC=app			#	name of the app

LIB=lib/

all: $(EXEC)

# normal app
app: app.o $(LIB)mazeDisplay/mazeDisplay.c $(LIB)BetterTerminal/betterTerminal.c
	$(CC) -o app app.o $(LIB)mazeDisplay/mazeDisplay.c $(LIB)BetterTerminal/betterTerminal.c $(LDFLAGS)
	make clean

# make file for TDD
test: test.o $(LIB)mazeDisplay/mazeDisplay.c $(LIB)BetterTerminal/betterTerminal.c
	$(CC) -o test test.o $(LIB)mazeDisplay/mazeDisplay.c $(LIB)BetterTerminal/betterTerminal.c $(LDFLAGS)
	make clean

# compiling c mains
test.o: test.c $(LIB)minunit/minunit.h 
	$(CC) test.o -c test.c $(LIB)minunit/minunit.h $(CFLAGS)

app.o: app.c
	$(CC) app.o -c app.c $(CFLAGS)

# utils
clean:
	rm *.o