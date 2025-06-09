#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "core.h"
#include "tui.h"

void simulate_generation(int tokens_per_second, int total_tokens) {
	if (tokens_per_second <= 0) {
		printf("Error: Token generation speed must be positive.\n");
		return;
	}

	float sleep_time = 1.0 / tokens_per_second;
	int terminal_width = get_terminal_width();
	int progress_bar_width = terminal_width > 60 ? 40 : terminal_width - 20;

	time_t start_time = time(NULL);
	int generated_tokens = 0;

	while (generated_tokens < total_tokens) {
		clear_screen();
		print_header();

		printf("Tokens per second: %d\n", tokens_per_second);
		printf("Total tokens to generate: %d\n\n", total_tokens);

		printf("Progress: ");
		display_progress_bar(generated_tokens, total_tokens, progress_bar_width);
		printf("\n");

		display_generated_tokens(generated_tokens);

		printf("\nGeneration Time:\n");
		time_t current_time = time(NULL);
		int elapsed_seconds = difftime(current_time, start_time);
		printf("Elapsed: %.3f seconds\n", (float)elapsed_seconds);

		float expected_time = (float)total_tokens / tokens_per_second;
		printf("Expected: %.3f seconds\n", expected_time);

		printf("\nNote: Elapsed time may differ from expected time due to system performance limitations.\n");

		generated_tokens++;
		if (generated_tokens < total_tokens) {
			usleep(sleep_time * 1000000);
		}
	}

	clear_screen();
	print_header();

	printf("Tokens per second: %d\n\n", tokens_per_second);

	printf("Progress: ");
	display_progress_bar(generated_tokens, total_tokens, progress_bar_width);
	printf("\n");

	display_generated_tokens(generated_tokens);

	printf("\nGeneration Time:\n");
	time_t current_time = time(NULL);
	float elapsed_seconds = difftime(current_time, start_time);
	printf("Elapsed: %.3f seconds\n", elapsed_seconds);

	float expected_time = (float)total_tokens / tokens_per_second;
	printf("Expected: %.3f seconds\n", expected_time);

	printf("\nSimulation complete!\n");
}