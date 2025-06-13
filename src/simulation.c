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

	printf("\nGenerated Tokens (%d tokens):\n", count);
	render_bar_term_width();

	// print in chunks of DEFAULT_CHUNK_SIZE chars
	for (int i = 0; i < chars_needed; i += CHUNK_SIZE) {
		int current_chunk_size = (i + CHUNK_SIZE < chars_needed) ? CHUNK_SIZE : chars_needed - i;
		printf("%.*s", current_chunk_size, token_display_cache + i);
	}

	render_bar_term_width();
}

void simulate_generation(int tokens_per_second, int total_tokens) {
	if (tokens_per_second <= 0) {
		printf("Error: Token generation speed must be positive.\n");
		return;
	}

	float sleep_time = 1.0 / tokens_per_second;

	// cache terminal width - only calculate once
	static int terminal_width = 0;
	static int progress_bar_width = 0;
	static int non_loader_char = 17;
	if (terminal_width == 0) {
		terminal_width = get_terminal_width();
		progress_bar_width = terminal_width - non_loader_char;
	}

	time_t start_time = time(NULL);
	int generated_tokens = 0;

	while (generated_tokens < total_tokens) {
		clear_screen();
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

		fflush(stdout); // ensure output is displayed immediately (especially important for TUI refresh rate)

		generated_tokens++;
		if (generated_tokens < total_tokens) {
			usleep(sleep_time * 1000000);
		}
	}

	// final display (still frame)
	clear_screen();
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
