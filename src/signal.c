#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "signal.h"
#include "tui.h"

void cleanup_handler(int sig) {
	(void)sig; // suppress unused parameter warning
	cleanup_display_cache();
	printf("\nCleaning up...\n");
	fflush(stdout);
	exit(0);
}

// prevent memleaks on abnormal termination
void setup_signal_handlers(void) {
	signal(SIGINT, cleanup_handler);
	signal(SIGTERM, cleanup_handler);
}
