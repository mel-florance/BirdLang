#pragma once

#include <time.h>

class Profiler {
public:
	Profiler():
		start(0),
		end(0) {}

	double getReport() {
		return ((double)end - (double)start) / CLOCKS_PER_SEC;
	}

	clock_t start;
	clock_t end;
};
