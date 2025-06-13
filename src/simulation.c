#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "const.h"
#include "simulation.h"
#include "tui.h"

// static cache for token display
static char *token_display_cache = NULL;
static int cache_size = 0;
static char *output_buffer = NULL;
static int output_buffer_size = 0;

void render_generated_tokens(int count) {
	int sample_length = strlen(SAMPLE_TEXT);
	int chars_needed = count * 5;

	// only regenerate cache if we need more chars
	if (chars_needed > cache_size) {
		cache_size = chars_needed + 400; // allocate extra to avoid frequent reallocs
		token_display_cache = realloc(token_display_cache, cache_size + 1);

		for (int i = 0; i < cache_size; i++) {
			token_display_cache[i] = SAMPLE_TEXT[i % sample_length];
		}
		token_display_cache[cache_size] = '\0';
	}

	// Pre-calculate output size and allocate buffer
	int terminal_width = get_terminal_width();
	int header_size = 50;                                                 // approximate size for header text
	int bars_size = terminal_width * 2 + 4;                               // two bars plus newlines
	int total_output_size = header_size + bars_size + chars_needed + 100; // extra buffer

	if (output_buffer_size < total_output_size) {
		output_buffer = realloc(output_buffer, total_output_size);
		output_buffer_size = total_output_size;
	}

	// Build entire output in buffer first
	int pos = 0;
	pos += sprintf(output_buffer + pos, "\nGenerated Tokens (%d tokens):\n", count);

	// Add top bar
	pos += sprintf(output_buffer + pos, "\n");
	for (int i = 0; i < terminal_width; i++) {
		output_buffer[pos++] = '-';
	}
	pos += sprintf(output_buffer + pos, "\n");

	// Add token content
	int chunk_end = (chars_needed < cache_size) ? chars_needed : cache_size;
	memcpy(output_buffer + pos, token_display_cache, chunk_end);
	pos += chunk_end;

	// Add bottom bar
	pos += sprintf(output_buffer + pos, "\n");
	for (int i = 0; i < terminal_width; i++) {
		output_buffer[pos++] = '-';
	}
	pos += sprintf(output_buffer + pos, "\n");

	output_buffer[pos] = '\0';

	// Single printf call for entire token display
	printf("%s", output_buffer);
}

void simulate_generation(int tokens_per_second, int total_tokens) {
	if (tokens_per_second <= 0) {
		printf("Error: Token generation speed must be positive.\n");
		return;
	}

	float sleep_time = 1.0 / tokens_per_second;

	// Initialize terminal width cache
	get_terminal_width();
	int progress_bar_width = get_progress_bar_width();

	time_t start_time = time(NULL);
	int generated_tokens = 0;

	// Use move_cursor_home instead of clear_screen for better performance
	bool first_render = true;

	while (generated_tokens < total_tokens) {
		if (first_render) {
			clear_screen();
			first_render = false;
		} else {
			move_cursor_home();
			clear_from_cursor();
		}

		render_header();

		printf("Tokens per second: %d\n", tokens_per_second);
		printf("Total tokens to generate: %d\n\n", total_tokens);

		printf("Progress: ");
		render_progress_bar(generated_tokens, total_tokens, progress_bar_width);
		printf("\n");

		render_generated_tokens(generated_tokens);

		printf("\nGeneration Time:\n");
		time_t current_time = time(NULL);
		int elapsed_seconds = difftime(current_time, start_time);
		printf("Elapsed: %.3f seconds\n", (float)elapsed_seconds);

		float expected_time = (float)total_tokens / tokens_per_second;
		printf("Expected: %.3f seconds\n", expected_time);
		printf("\nNote: Elapsed time may differ from expected time due to system performance limitations.\n");

		fflush(stdout);

		generated_tokens++;
		if (generated_tokens < total_tokens) {
			usleep(sleep_time * 1000000);
		}
	}

	// Final display
	move_cursor_home();
	clear_from_cursor();
	render_header();

	printf("Tokens per second: %d\n\n", tokens_per_second);

	printf("Progress: ");
	render_progress_bar(generated_tokens, total_tokens, progress_bar_width);
	printf("\n");

	render_generated_tokens(generated_tokens);

	printf("\nGeneration Time:\n");
	time_t current_time = time(NULL);
	float elapsed_seconds = difftime(current_time, start_time);
	printf("Elapsed: %.3f seconds\n", elapsed_seconds);

	float expected_time = (float)total_tokens / tokens_per_second;
	printf("Expected: %.3f seconds\n", expected_time);

	printf("\nSimulation complete!\n");
}
