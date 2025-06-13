#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "signal.h"
#include "simulation.h"
#include "tui.h"

int main() {
	clear_screen();
	render_header();

	// Initialize display cache early
	init_display_cache();
	setup_signal_handlers();

	int total_tokens = get_total_tokens();
	int tokens_per_second = get_tokens_per_second();

	// Run simulation
	simulate_generation(tokens_per_second, total_tokens);

	// Cleanup
	cleanup_display_cache();

	return 0;
}
