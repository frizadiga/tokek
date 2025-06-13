.PHONY: all dev release

CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c2x
SRCS = ./src/*.c
BIN_NAME = tokek

all: dev

dev:
	$(CC) $(CFLAGS) $(SRCS) -o ./tokek && ./$(BIN_NAME)

release:
	$(CC) $(CFLAGS) $(SRCS) -o ./$(BIN_NAME)
