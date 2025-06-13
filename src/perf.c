#include <stdio.h>
#include "perf.h"

void perf_start_timer(perf_timer_t *timer, const char *operation_name) {
	timer->operation_name = operation_name;
	timer->start_time = clock();
}

void perf_end_timer(perf_timer_t *timer) {
	timer->end_time = clock();
}

double perf_get_elapsed_ms(perf_timer_t *timer) {
	return ((double)(timer->end_time - timer->start_time) / CLOCKS_PER_SEC) * 1000.0;
}
