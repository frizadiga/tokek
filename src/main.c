#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "signal.h"
#include "simulation.h"
#include "tui.h"

int main() {
	clear_screen();
	render_header();

	setup_signal_handlers();

	int total_tokens = get_total_tokens();
	int tokens_per_second = get_tokens_per_second();

	// simulation process
	simulate_generation(tokens_per_second, total_tokens);

	// @NOTE: to future me - ensure cleanup just before exit
	cleanup_display_cache();

	return 0;
}
