#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "const.h"
#include "simulation.h"
#include "tui.h"

// Pre-allocated static buffers
static char *token_display_cache = NULL;
static char *progress_buffer = NULL;
static int cache_size = 0;

void render_generated_tokens(int count) {
	int sample_length = strlen(SAMPLE_TEXT);
	int chars_needed = count * 5;

	// Expand cache if needed
	if (chars_needed > cache_size) {
		cache_size = chars_needed + 1000; // Extra buffer to reduce reallocations
		token_display_cache = realloc(token_display_cache, cache_size + 1);

		// Fast fill using pattern repetition
		int full_copies = cache_size / sample_length;
		int remainder = cache_size % sample_length;

		for (int i = 0; i < full_copies; i++) {
			memcpy(token_display_cache + i * sample_length, SAMPLE_TEXT, sample_length);
		}
		if (remainder > 0) {
			memcpy(token_display_cache + full_copies * sample_length, SAMPLE_TEXT, remainder);
		}

		token_display_cache[cache_size] = '\0';
	}

	int terminal_width = get_terminal_width();

	printf("\nGenerated Tokens (%d tokens):\n\n", count);

	// Print top bar
	for (int i = 0; i < terminal_width; i++) {
		putchar('-');
	}
	putchar('\n');

	// Print token content (limit to avoid buffer overflow)
	int display_chars = (chars_needed < cache_size) ? chars_needed : cache_size;
	fwrite(token_display_cache, 1, display_chars, stdout);

	putchar('\n');

	// Print bottom bar
	for (int i = 0; i < terminal_width; i++) {
		putchar('-');
	}
	putchar('\n');
}

void simulate_generation(int tokens_per_second, int total_tokens) {
	if (tokens_per_second <= 0) {
		printf("Error: Token generation speed must be positive.\n");
		return;
	}

	// Pre-allocate progress buffer
	int progress_bar_width = get_progress_bar_width();
	progress_buffer = malloc(progress_bar_width + 50);

	float sleep_time = 1.0 / tokens_per_second;
	time_t start_time = time(NULL);
	int generated_tokens = 0;
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

		// Render progress bar to buffer first
		render_progress_bar(generated_tokens, total_tokens, progress_bar_width, progress_buffer);
		printf("Progress: %s\n", progress_buffer);

		render_generated_tokens(generated_tokens);

		// Time calculations
		time_t current_time = time(NULL);
		int elapsed_seconds = difftime(current_time, start_time);
		float expected_time = (float)total_tokens / tokens_per_second;

		printf("\nGeneration Time:\n");
		printf("Elapsed: %.3f seconds\n", (float)elapsed_seconds);
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

	printf("Tokens per second: %d\n", tokens_per_second);
	printf("Total tokens to generate: %d\n\n", total_tokens);

	render_progress_bar(generated_tokens, total_tokens, progress_bar_width, progress_buffer);
	printf("Progress: %s\n", progress_buffer);

	render_generated_tokens(generated_tokens);

	time_t current_time = time(NULL);
	float elapsed_seconds = difftime(current_time, start_time);
	float expected_time = (float)total_tokens / tokens_per_second;

	printf("\nGeneration Time:\n");
	printf("Elapsed: %.3f seconds\n", elapsed_seconds);
	printf("Expected: %.3f seconds\n", expected_time);
	printf("\nSimulation complete!\n");

	// Cleanup
	if (progress_buffer) {
		free(progress_buffer);
		progress_buffer = NULL;
	}
}
