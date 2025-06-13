#ifndef PERF_H
#define PERF_H

#include <time.h>

typedef struct {
	clock_t start_time;
	clock_t end_time;
	const char* operation_name;
} perf_timer_t;

void perf_start_timer(perf_timer_t* timer, const char* operation_name);
void perf_end_timer(perf_timer_t* timer);
double perf_get_elapsed_ms(perf_timer_t* timer);

#endif
