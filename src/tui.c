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

void render_header() {
	printf("🦎 Tokek: Token Generation Simulator\n");
	printf("------------------------------------\n\n");
}

void render_progress_bar(int current, int total, int width) {
	int progress_width = (int)((float)current / total * width);
	int percentage = (int)((float)current / total * 100);

	printf("[");
	for (int i = 0; i < width; i++) {
		printf("%c", (i < progress_width) ? '=' : ' ');
	}
	printf("] %d%%", percentage);
}

void render_bar_term_width() {
	printf("\n");
	for (int i = 0; i < get_terminal_width(); i++) {
		printf("-");
	}
	printf("\n");
}

void cleanup_display_cache() {
	if (token_display_cache) {
		free(token_display_cache);
		token_display_cache = NULL;
		cache_size = 0;
	}
}
