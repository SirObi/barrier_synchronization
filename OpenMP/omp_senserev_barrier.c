#include <stdio.h>
#include <omp.h>

int sense = 0;
int count;
int number_threads;

void omp_senserev_init(int num_threads) {
	number_threads = num_threads;
	count = num_threads;
}

void omp_senserev_barrier() {
	int local_sense = !sense;

	if (__sync_fetch_and_sub(&count, 1) == 1) {
		count = number_threads;
		sense = local_sense;
	}
	else {
		while (sense != local_sense) {}
	}
}

void omp_senserev_finalize() {
	// Nothing to free on the heap
}
