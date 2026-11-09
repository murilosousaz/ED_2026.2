CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Wpedantic -O2
SRC     = $(wildcard src/*.c)
OBJ     = $(SRC:.c=.o)
BIN     = bin/eventos

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJ) | bin
	$(CC) $(CFLAGS) -o $@ $(OBJ)

bin:
	mkdir -p bin

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf bin src/*.o
