#include <stdio.h>
#include <stdlib.h>

int get_tokens_per_second() {
	int tokens_per_second = 0;
	char *env_tokens_per_second = getenv("TOKEK_TOKENS_PER_SECOND");
	if (env_tokens_per_second != NULL) {
		tokens_per_second = atoi(env_tokens_per_second);
		printf("Using tokens per second from environment: %d\n", tokens_per_second);
	} else {
		printf("Enter tokens per second: ");
		scanf("%d", &tokens_per_second);
	}

	int def_tokens_per_second = 10; // default value

	if (tokens_per_second <= 0) {
		printf("Invalid input. Using default value: %d tokens per second.\n", def_tokens_per_second);
		tokens_per_second = def_tokens_per_second;
	} else {
		printf("Tokens per second set to: %d\n", tokens_per_second);
	}

	return tokens_per_second;
}

int get_total_tokens() {
	int total_tokens = 0;
	char *env_total_tokens = getenv("TOKEK_TOKENS_TOTAL");
	if (env_total_tokens != NULL) {
		total_tokens = atoi(env_total_tokens);
		printf("Using total tokens from environment: %d\n", total_tokens);
	} else {
		printf("Enter total tokens to generate: ");
		scanf("%d", &total_tokens);
	}

	int def_total_tokens = 100; // default value

	if (total_tokens <= 0) {
		printf("Invalid input. Using default value: %d total tokens.\n", def_total_tokens);
		total_tokens = def_total_tokens;
	} else {
		printf("Total tokens set to: %d\n", total_tokens);
	}

	return total_tokens;
}
