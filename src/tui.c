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

// static cache for token display
static char *token_display_cache = NULL;
static int cache_size = 0;
static int chunk_size = 80; // default chunk size for display

void clear_screen() {
#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD count;
	DWORD cellCount;
	COORD homeCoords = {0, 0};

	if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		cellCount = csbi.dwSize.X * csbi.dwSize.Y;
		FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoords, &count);
		FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count);
		SetConsoleCursorPosition(hConsole, homeCoords);
	}
#else
	printf("\033[2J\033[H");
	fflush(stdout);
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
	int percentage = (int)((float)current / total * 100);

	printf("[");
	for (int i = 0; i < width; i++) {
		printf("%c", (i < progress_width) ? '=' : ' ');
	}
	printf("] %d%%", percentage);
}

void display_generated_tokens(int count) {
	const char *sample_text =
	    "This is a fallback sample text for token generation. "
	    "It will be used if the actual sample text fails to load. "
	    "The purpose of this text is to provide a backup solution for the token generation simulation. ";

	int sample_length = strlen(sample_text);
	int chars_needed = count * 5;

	// only regenerate cache if we need more characters
	if (chars_needed > cache_size) {
		cache_size = chars_needed + 400; // Allocate extra to avoid frequent reallocs
		token_display_cache = realloc(token_display_cache, cache_size + 1);

		for (int i = 0; i < cache_size; i++) {
			token_display_cache[i] = sample_text[i % sample_length];
		}
		token_display_cache[cache_size] = '\0';
	}

	printf("\nGenerated Tokens (%d tokens):\n", count);
	printf("\n---\n");

	// print in chunks of chunk_size characters
	for (int i = 0; i < chars_needed; i += chunk_size) {
		int current_chunk_size = (i + chunk_size < chars_needed) ? chunk_size : chars_needed - i;
		printf("%.*s", current_chunk_size, token_display_cache + i);
	}

	printf("\n---\n");
}

void cleanup_display_cache() {
	if (token_display_cache) {
		free(token_display_cache);
		token_display_cache = NULL;
		cache_size = 0;
	}
}
