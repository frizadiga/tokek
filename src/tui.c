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
static int cached_terminal_width = 0;
static int cached_progress_bar_width = 0;

// use cursor positioning instead of full screen clear
void move_cursor_home() {
#ifdef _WIN32
	COORD homeCoords = {0, 0};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), homeCoords);
#else
	printf("\033[H");
#endif
}

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
#endif
}

// clear from cursor to end of screen (more efficient)
void clear_from_cursor() {
#ifdef _WIN32
	// @TODO: Windows implementation really complex, fall back to full clear
	clear_screen();
#else
	printf("\033[J");
#endif
}

int get_terminal_width() {
	if (cached_terminal_width > 0) {
		return cached_terminal_width;
	}

#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	cached_terminal_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	cached_terminal_width = w.ws_col;
#endif

	// cache progress bar width calculation
	cached_progress_bar_width = cached_terminal_width - 17; // non_loader_char

	return cached_terminal_width;
}

int get_progress_bar_width() {
	if (cached_progress_bar_width > 0) {
		return cached_progress_bar_width;
	}
	get_terminal_width(); // This will set both cached values
	return cached_progress_bar_width;
}

void render_header() {
	printf("🦎 Tokek: Token Generation Simulator\n");
	printf("------------------------------------\n\n");
}

// pre-allocate progress bar string to avoid repeated allocations
static char *progress_bar_buffer = NULL;
static int progress_bar_buffer_size = 0;

void render_progress_bar(int current, int total, int width) {
	// ensure buffer is large enough
	int needed_size = width + 20; // extra space for percentage text
	if (progress_bar_buffer_size < needed_size) {
		progress_bar_buffer = realloc(progress_bar_buffer, needed_size);
		progress_bar_buffer_size = needed_size;
	}

	int progress_width = (int)((float)current / total * width);
	int percentage = (int)((float)current / total * 100);

	// build progress bar in buffer first
	int pos = 0;
	progress_bar_buffer[pos++] = '[';

	for (int i = 0; i < width; i++) {
		progress_bar_buffer[pos++] = (i < progress_width) ? '=' : ' ';
	}

	pos += sprintf(progress_bar_buffer + pos, "] %d%%", percentage);
	progress_bar_buffer[pos] = '\0';

	// single printf call
	printf("%s", progress_bar_buffer);
}

void render_bar_term_width() {
	static char *bar_buffer = NULL;
	static int bar_buffer_size = 0;

	int width = get_terminal_width();

	if (bar_buffer_size < width + 2) {
		bar_buffer = realloc(bar_buffer, width + 2);
		// use memset for faster initialization
		memset(bar_buffer, '-', width);
		bar_buffer[width] = '\n';
		bar_buffer[width + 1] = '\0';
		bar_buffer_size = width + 2;
	}

	printf("\n%s", bar_buffer);
}

void cleanup_display_cache() {
	if (token_display_cache) {
		free(token_display_cache);
		token_display_cache = NULL;
		cache_size = 0;
	}
	if (progress_bar_buffer) {
		free(progress_bar_buffer);
		progress_bar_buffer = NULL;
		progress_bar_buffer_size = 0;
	}
}
