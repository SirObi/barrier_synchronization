#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
	int num_threads = atoi(argv[1]);
	int num_iterations = atoi(argv[2]);

    int counter[num_threads];
    memset(counter, 0, sizeof(counter));

    MPI_Init(NULL, NULL);
    long int timespent = 0;

	struct timeval t0, t1;
	gettimeofday(&t0, NULL);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	for (int i = 0; i < num_iterations; i++) {
		counter[world_rank]++;
		for (int p = 0; p < num_threads; p++) {
			if (p != world_rank){
				MPI_Send(&counter[world_rank], 1, MPI_INT, p, 0, MPI_COMM_WORLD);
			}
		}
		for (int p = 0; p < num_threads; p++) {
			if (p != world_rank){
				int update = 0;
				MPI_Recv(&update, 1, MPI_INT, p, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
				printf("Setting counter p %d to %d\n", p, update);
				counter[p] = update;
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	printf("I'm %d and my neighbours' counter was %d and %d\n",
			world_rank,
			counter[(world_rank - 1) % num_threads],
			counter[(world_rank + 1) % num_threads]);

	gettimeofday(&t1, NULL);
	timespent += (t1.tv_usec - t0.tv_usec);

	printf("Thread %d t0: %ld\n", world_rank, t0.tv_usec);
	printf("Thread %d t1: %ld\n", world_rank, t1.tv_usec);
	printf("Time spent was %ld\n", timespent);

	MPI_Finalize();
}
