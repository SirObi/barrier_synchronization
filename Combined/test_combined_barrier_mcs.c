#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "../MPI/mpi_mcs_barrier.h"
#include "../OpenMP/omp_mcs_barrier.h"

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Expected argc is at least 3\n");
		return 0;
	}

	int num_processes = atoi(argv[1]);
	int num_threads = atoi(argv[2]);
	int num_iterations = atoi(argv[3]);

    long int timespent_micro = 0;
    long int timespent_sec = 0;

    MPI_Init(&argc, &argv);
    mpi_mcs_init(num_processes);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	struct timeval t0, t1;
	gettimeofday(&t0, NULL);


	omp_mcs_init(num_threads);
#pragma omp parallel num_threads(num_threads)
	{
		//run_omp(num_threads, world_rank);

		int counter = 0;
		// do some work
		for (int i = 0; i < num_iterations; i ++) {
			counter++;
			printf("Running thread %d on proc %d\n", omp_get_thread_num(), world_rank);
			omp_mcs_barrier();
		}

	}
	omp_mcs_finalize();
	mpi_mcs_barrier();

	gettimeofday(&t1, NULL);
	timespent_micro += abs(t1.tv_usec - t0.tv_usec);
	timespent_sec += abs(t1.tv_sec - t0.tv_sec);

	// debug: printf("Time spent by process %d was %ld\n", world_rank, timespent_micro + timespent_sec * 1000000);
	printf("%ld\n", timespent_micro + timespent_sec * 1000000);

	MPI_Finalize();
}
