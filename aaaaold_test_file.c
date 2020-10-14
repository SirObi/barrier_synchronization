#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
	int threads = atoi(argv[1]);
	int num_iterations = atoi(argv[2]);
    int counter[threads];
    memset(counter, 0, sizeof(counter));
    long int timespent[omp_get_max_threads()];
    memset(timespent, 0, sizeof(timespent));

    #pragma omp parallel num_threads(threads)
    {
    	struct timeval t0, t1;
    	gettimeofday(&t0, NULL);

		for (int i = 0; i < num_iterations; i++) {
//			printf("Thread %d will increment counter\n",  omp_get_thread_num());
			counter[omp_get_thread_num()]++;



//			for (int j = 0; j < threads; j++) {
//				printf("Thread %d: %d ", j, counter[j]);
//			}
//			printf("\n\n");
			#pragma omp barrier
		}

    	gettimeofday(&t1, NULL);
    	timespent[omp_get_thread_num()] += (t1.tv_usec - t0.tv_usec);
    	printf("Thread %d t0: %ld\n", omp_get_thread_num(), t0.tv_usec);
    	printf("Thread %d t1: %ld\n", omp_get_thread_num(), t1.tv_usec);
    	printf("Time spent was %ld\n", timespent[omp_get_thread_num()]);

    }
}
