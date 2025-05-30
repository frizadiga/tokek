#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#endif

// @fn: clear screen (cross-platform)
void clear_screen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

// @fn: get terminal width
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

// @fn: display a progress bar
void display_progress_bar(int current, int total, int width) {
	int progress_width = (int)((float)current / total * width);
	printf("[");

	for (int i = 0; i < width; i++) {
		if (i < progress_width) {
			printf("=");
		} else {
			printf(" ");
		}
	}
	printf("] %d%%", (int)((float)current / total * 100));
}

// @fn: display sample generated tokens
void display_generated_tokens(int count, int max_display) {
	const char *sample_text =
	    "This is a fallback sample text for token generation. "
	    "It will be used if the actual sample text fails to load. "
	    "The purpose of this text is to provide a backup solution for the token generation simulation. ";

	int sample_length = strlen(sample_text);
	int display_count = count < max_display ? count : max_display;

	printf("\nGenerated Tokens (%d tokens):\n", count);
	printf("+%s+\n", "------------------------------------------------------------------");

	int chars_to_display = display_count * 5; // Roughly 5 chars per token
	for (int i = 0; i < chars_to_display; i += 80) {
		int chunk_size = i + 80 < chars_to_display ? 80 : chars_to_display - i;
		printf("| ");
		for (int j = 0; j < chunk_size; j++) {
			printf("%c", sample_text[(i + j) % sample_length]);
		}
		printf(" |\n");
	}

	printf("+%s+\n", "------------------------------------------------------------------");
}

// @fn: simulate token generation
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

		printf("Token Generation Simulator\n");
		printf("=========================\n\n");

		printf("Tokens per second: %d\n", tokens_per_second);
		printf("Total tokens to generate: %d\n\n", total_tokens);

		printf("Progress: ");
		display_progress_bar(generated_tokens, total_tokens, progress_bar_width);
		printf("\n");

		display_generated_tokens(generated_tokens, 100);

		printf("\nGeneration Time\n");
		printf("---------------\n");
		time_t current_time = time(NULL);
		int elapsed_seconds = difftime(current_time, start_time);
		printf("Elapsed: %.3f seconds\n", (float)elapsed_seconds);

		float expected_time = (float)total_tokens / tokens_per_second;
		printf("Expected: %.3f seconds\n", expected_time);

		printf("\nNote: Elapsed time may differ from expected time due to system performance limitations.\n");

		// Generate next token
		generated_tokens++;
		if (generated_tokens < total_tokens) {
			usleep(sleep_time * 1000000); // Convert to microseconds
		}
	}

	// Final update
	clear_screen();
	printf("Token Generation Simulator\n");
	printf("=========================\n\n");

	printf("Tokens per second: %d\n\n", tokens_per_second);

	printf("Progress: ");
	display_progress_bar(generated_tokens, total_tokens, progress_bar_width);
	printf("\n");

	display_generated_tokens(generated_tokens, 100);

	printf("\nGeneration Time\n");
	printf("---------------\n");
	time_t current_time = time(NULL);
	float elapsed_seconds = difftime(current_time, start_time);
	printf("Elapsed: %.3f seconds\n", elapsed_seconds);

	float expected_time = (float)total_tokens / tokens_per_second;
	printf("Expected: %.3f seconds\n", expected_time);

	printf("\nSimulation complete!\n");
}

int main() {
	clear_screen();
	printf("Tokek: Token Generation Simulator\n");
	printf("=========================\n\n");

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
