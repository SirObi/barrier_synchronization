#include <mpi.h>
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

    long int timespent_micro = 0;
    long int timespent_sec = 0;

    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	struct timeval t0, t1;
	gettimeofday(&t0, NULL);

	for (int i = 0; i < num_iterations; i++) {
		MPI_Barrier(MPI_COMM_WORLD);
	}

	gettimeofday(&t1, NULL);
	timespent_micro += abs(t1.tv_usec - t0.tv_usec);
	timespent_sec += abs(t1.tv_sec - t0.tv_sec);

	//printf("Thread %d t0: %ld\n", world_rank, t0.tv_usec);
	//printf("Thread %d t1: %ld\n", world_rank, t1.tv_usec);
	printf("Time spent by thread %d was %ld\n", world_rank, timespent_micro + timespent_sec * 1000000);

	// ??? is this even kosher? I don't think it gives you an average across processes
	printf("Average time spent was %ld microseconds\n", timespent_micro + timespent_sec * 1000000);

	MPI_Finalize();
}
