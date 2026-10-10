CFLAGS = -std=c11 -Wall -Wextra -Wshadow -pedantic -ggdb
SANFLAGS = $(CFLAGS) -fsanitize=address,undefined

.PHONY: all
all: example example_asan

example: example.c vek.h
	$(CC) $(CFLAGS) example.c -o $@

example_asan: example.c vek.h
	$(CC) $(SANFLAGS) example.c -o $@