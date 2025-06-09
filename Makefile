CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c2x
SRCS = ./src/*.c

all: program

program:
	$(CC) $(SRCS) -o ./tokek
	# $(CC) $(CFLAGS) $(SRCS) -o ./tokek

dev:
	$(CC) $(SRCS) -o ./tokek && ./tokek
	# $(CC) $(CFLAGS) $(SRCS) -o ./tokek && ./tokek
