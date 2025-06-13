.PHONY: all dev release

CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c2x
CFLAGS_DEBUG = -g -O0
CFLAGS_RELEASE = -O2 -DNDEBUG
SRCS = ./src/*.c
BIN_NAME = tokek

all: dev

dev: build
	./$(BIN_NAME)

release:
	$(CC) $(CFLAGS) $(SRCS) -o ./$(BIN_NAME)

debug:
	lldb ./$(BIN_NAME)

build:
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $(SRCS) -o ./$(BIN_NAME)

