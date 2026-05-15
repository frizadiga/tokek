#include <stdio.h>
#include <stdlib.h>
#include "const.h"

int get_tokens_per_second() {
	int tokens_per_second = 0;
	int is_default = 0;
	char *env_tokens_per_second = getenv("TOKEK_TOKENS_PER_SECOND");
	if (env_tokens_per_second != NULL) {
		tokens_per_second = atoi(env_tokens_per_second);
		printf("Using tokens per second from environment: %d\n", tokens_per_second);
	} else {
		char input[100];
		printf("Enter tokens per second [default %d]: ", TOKENS_PER_SECOND);
		if (fgets(input, sizeof(input), stdin) == NULL || input[0] == '\n') {
			tokens_per_second = TOKENS_PER_SECOND;
			is_default = 1;
			printf("Using default value: %d tokens per second.\n", TOKENS_PER_SECOND);
		} else {
			tokens_per_second = atoi(input);
		}
	}

	if (!is_default && tokens_per_second <= 0) {
		printf("Invalid input. Using default value: %d tokens per second.\n", TOKENS_PER_SECOND);
		tokens_per_second = TOKENS_PER_SECOND;
	} else if (!is_default) {
		printf("Tokens per second set to: %d\n", tokens_per_second);
	}

	return tokens_per_second;
}

int get_total_tokens() {
	int total_tokens = 0;
	int is_default = 0;
	char *env_total_tokens = getenv("TOKEK_TOKENS_TOTAL");
	if (env_total_tokens != NULL) {
		total_tokens = atoi(env_total_tokens);
		printf("Using total tokens from environment: %d\n", total_tokens);
	} else {
		char input[100];
		printf("Enter total tokens to generate [default %d]: ", TOTAL_TOKENS);
		if (fgets(input, sizeof(input), stdin) == NULL || input[0] == '\n') {
			total_tokens = TOTAL_TOKENS;
			is_default = 1;
			printf("Using default value: %d total tokens.\n", TOTAL_TOKENS);
		} else {
			total_tokens = atoi(input);
		}
	}

	if (!is_default && total_tokens <= 0) {
		printf("Invalid input. Using default value: %d total tokens.\n", TOTAL_TOKENS);
		total_tokens = TOTAL_TOKENS;
	} else if (!is_default) {
		printf("Total tokens set to: %d\n", total_tokens);
	}

	return total_tokens;
}
