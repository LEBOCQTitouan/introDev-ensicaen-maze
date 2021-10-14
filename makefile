# defining vars
CC=gcc

CFLAGS=-Wall -O
LDFLAGS=

LIB=lib/

all: $(EXEC)

sources_files:= $(shell find ./lib -name '*.c' -not -path "./lib/minunit/*")
override object_files= $(sources_files:$(source_directory)/%.c=$(object_directory)/%.o)

# normal app
app: app.o $(object_files)
	$(CC) -o app app.o $(object_files) $(LDFLAGS)
	make clean

# make file for TDD
test: test.o $(object_files)
	$(CC) -lrt -lm -o test test.o $(LIB)minunit/minunit.h $(object_files) $(LDFLAGS)
	make clean

# compiling c mains
test.o: test.c 
	$(CC) test.o -c test.c $(CFLAGS)

app.o: app.c
	$(CC) app.o -c app.c $(CFLAGS)

# utils
clean:
	rm *.o