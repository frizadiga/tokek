.PHONY: all dev clean release debug build format

CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c2x
CFLAGS_DEBUG = -g -O0
CFLAGS_RELEASE = -O2 -DNDEBUG
DEBUGGER = lldb
SRCS = $(wildcard ./src/*.c)
BIN_NAME = tokek
TOKEK_TOKENS_PER_SECOND=20
TOKEK_TOKENS_TOTAL=100
ENV_INJECT = TOKEK_TOKENS_PER_SECOND=$(TOKEK_TOKENS_PER_SECOND) TOKEK_TOKENS_TOTAL=$(TOKEK_TOKENS_TOTAL)

all: dev

dev: build
	$(ENV_INJECT) ./$(BIN_NAME)

build:
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $(SRCS) -o ./$(BIN_NAME)

release:
	$(CC) $(CFLAGS) $(CFLAGS_RELEASE) $(SRCS) -o ./$(BIN_NAME)

debug: build
	$(DEBUGGER) ./$(BIN_NAME)

clean:
	rm -rf ./$(BIN_NAME) ./src/*.o *.dSYM

format:
	clang-format -i $(SRCS) $(wildcard ./src/*.h)
