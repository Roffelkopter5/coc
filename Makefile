CC = gcc
CFLAGS = -Wall -Wextra -pedantic -D_XOPEN_SOURCE=700 -std=c11 -g

OBJS = $(patsubst src/%.c,build/%.o,$(wildcard src/*.c))

.PHONY: all
.SILENT: clean

all: build coc

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

coc: $(OBJS)
	$(CC) $^ -o $@ 

build:
	mkdir -p build

clean:
	rm -rf build
	rm coc