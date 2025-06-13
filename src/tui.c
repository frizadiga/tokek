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

// Pre-allocated buffers to avoid runtime allocations
static char *main_output_buffer = NULL;
static char *progress_bar_buffer = NULL;
static char *bar_buffer = NULL;
static int main_output_buffer_size = 0;
static int progress_bar_buffer_size = 0;
static int bar_buffer_size = 0;

// Cached terminal dimensions
static int cached_terminal_width = 0;
static int cached_progress_bar_width = 0;

void init_display_cache(void) {
	// Pre-allocate buffers on startup
	int width = get_terminal_width();

	// Main output buffer - generous size to avoid reallocations
	main_output_buffer_size = width * 50 + 10000;
	main_output_buffer = malloc(main_output_buffer_size);

	// Progress bar buffer
	progress_bar_buffer_size = width + 50;
	progress_bar_buffer = malloc(progress_bar_buffer_size);

	// Bar buffer for horizontal lines
	bar_buffer_size = width + 10;
	bar_buffer = malloc(bar_buffer_size);

	// Pre-fill bar buffer
	memset(bar_buffer, '-', width);
	bar_buffer[width] = '\n';
	bar_buffer[width + 1] = '\0';
}

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

void clear_from_cursor() {
#ifdef _WIN32
	clear_screen(); // Fallback for Windows
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

	cached_progress_bar_width = cached_terminal_width - 17;
	return cached_terminal_width;
}

int get_progress_bar_width() {
	if (cached_progress_bar_width > 0) {
		return cached_progress_bar_width;
	}
	get_terminal_width();
	return cached_progress_bar_width;
}

void render_header() {
	printf("🦎 Tokek: Token Generation Simulator\n");
	printf("------------------------------------\n\n");
}

// Optimized progress bar - writes to provided buffer
void render_progress_bar(int current, int total, int width, char *buffer) {
	int progress_width = (int)((float)current / total * width);
	int percentage = (int)((float)current / total * 100);

	int pos = 0;
	buffer[pos++] = '[';

	// Use memset for faster fill operations
	memset(buffer + pos, '=', progress_width);
	pos += progress_width;

	memset(buffer + pos, ' ', width - progress_width);
	pos += width - progress_width;

	pos += sprintf(buffer + pos, "] %d%%", percentage);
	buffer[pos] = '\0';
}

void render_bar_term_width() {
	if (bar_buffer) {
		printf("\n%s", bar_buffer);
	}
}

void cleanup_display_cache() {
	if (main_output_buffer) {
		free(main_output_buffer);
		main_output_buffer = NULL;
		main_output_buffer_size = 0;
	}
	if (progress_bar_buffer) {
		free(progress_bar_buffer);
		progress_bar_buffer = NULL;
		progress_bar_buffer_size = 0;
	}
	if (bar_buffer) {
		free(bar_buffer);
		bar_buffer = NULL;
		bar_buffer_size = 0;
	}
}
