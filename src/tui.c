#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#endif
#include "tui.h"

void clear_screen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

int get_terminal_width() {
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w.ws_col;
#endif
}

void print_header() {
	printf("Tokek: Token Generation Simulator\n");
	printf("---------------------------------\n\n");
}

void display_progress_bar(int current, int total, int width) {
	int progress_width = (int)((float)current / total * width);
	printf("[");

	for (int i = 0; i < width; i++) {
		if (i < progress_width) {
			printf("=");
		} else {
			printf(" ");
		}
	}
	printf("] %d%%", (int)((float)current / total * 100));
}

void display_generated_tokens(int count) {
	const char *sample_text =
	    "This is a fallback sample text for token generation. "
	    "It will be used if the actual sample text fails to load. "
	    "The purpose of this text is to provide a backup solution for the token generation simulation. ";

	int sample_length = strlen(sample_text);

	printf("\nGenerated Tokens (%d tokens):\n", count);
	printf("\n---\n");

	int chars_to_display = count * 5;
	for (int i = 0; i < chars_to_display; i += 80) {
		int chunk_size = i + 80 < chars_to_display ? 80 : chars_to_display - i;
		for (int j = 0; j < chunk_size; j++) {
			printf("%c", sample_text[(i + j) % sample_length]);
		}
	}

	printf("\n---\n");
}

