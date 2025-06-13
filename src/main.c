#include <stdio.h>
#include <stdlib.h>
#include "core.h"
#include "tui.h"

int main() {
	clear_screen();
	print_header();

	// Get tokens per second from environment or user input
	int tokens_per_second = 0;
	char *env_tokens_per_second = getenv("TOKEK_TOKENS_PER_SECOND");
	if (env_tokens_per_second != NULL) {
		tokens_per_second = atoi(env_tokens_per_second);
		printf("Using tokens per second from environment: %d\n", tokens_per_second);
	} else {
		printf("Enter tokens per second: ");
		scanf("%d", &tokens_per_second);
	}

	if (tokens_per_second <= 0) {
		printf("Error: Tokens per second must be positive.\n");
		return 1;
	}

	// Get total tokens to generate from environment or user input
	int total_tokens = 0;
	char *env_total_tokens = getenv("TOKEK_TOKENS_TOTAL");
	if (env_total_tokens != NULL) {
		total_tokens = atoi(env_total_tokens);
		printf("Using total tokens from environment: %d\n", total_tokens);
	} else {
		printf("Enter total tokens to generate: ");
		scanf("%d", &total_tokens);
	}

	if (total_tokens <= 0) {
		printf("Error: Total tokens must be positive.\n");
		return 1;
	}

	// Run simulation
	simulate_generation(tokens_per_second, total_tokens);

	return 0;
}

