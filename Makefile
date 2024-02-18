CC = gcc
CFLAGS = -Wall -Wextra -pedantic -D_XOPEN_SOURCE=700 -std=c11 
EFLAGS = -Wformat=2 -Wshadow -Wconversion -Wunreachable-code -Wpointer-arith -Wcast-align -Wshadow -Wuninitialized -Winit-self -Wincompatible-pointer-types -Wredundant-decls -Wnull-dereference -Wstrict-prototypes -Wmissing-attributes -Wstrict-overflow -Wstringop-overflow=4 -Wmissing-noreturn -Wsuggest-attribute=noreturn -Wsuggest-attribute=const -Wsuggest-attribute=pure -Wsuggest-attribute=format -Wlogical-op -fmax-errors=0 -O0 -ggdb3 -fanalyzer -D_FORTIFY_SOURCE=2

OBJS = $(patsubst src/%.c,build/%.o,$(wildcard src/*.c))

.PHONY: all
.SILENT: clean

all: build coc

build/%.o: src/%.c
	$(CC) $(CFLAGS) $(EFLAGS) -c $< -o $@

coc: $(OBJS)
	$(CC) $^ -o $@ 

build:
	mkdir -p build

clean:
	rm -rf build
	rm coc