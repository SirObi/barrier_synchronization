#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Expected argc is at least 3\n");
		return 0;
	}

	int num_threads = atoi(argv[1]);
	int num_iterations = atoi(argv[2]);

	long int timespent_micro[omp_get_max_threads()];
    memset(timespent_micro, 0, sizeof(timespent_micro));

	long int timespent_sec[omp_get_max_threads()];
    memset(timespent_sec, 0, sizeof(timespent_sec));

	#pragma omp parallel num_threads(num_threads)
    {
    	struct timeval t0, t1;
    	gettimeofday(&t0, NULL);

		for (int i = 0; i < num_iterations; i++) {
        	#pragma omp barrier
		}

    	gettimeofday(&t1, NULL);
    	timespent_micro[omp_get_thread_num()] += (t1.tv_usec - t0.tv_usec);
    	timespent_sec[omp_get_thread_num()] += (t1.tv_sec - t0.tv_sec);
    }

    int time_sum_micro = 0;
    int time_sum_sec = 0;
	for (int i = 0; i < num_threads; i++) {
		time_sum_micro += abs(timespent_micro[omp_get_thread_num()]);
		time_sum_sec += abs(timespent_sec[omp_get_thread_num()]);
	}

	// debug: printf("Number of threads %d\n", num_threads);
	// debug: printf("Average time spent was %ld microseconds\n", (time_sum_micro + time_sum_sec * 1000000) / (long int) num_threads);
	printf("%d\n", time_sum_micro + time_sum_sec * 1000000);
}
