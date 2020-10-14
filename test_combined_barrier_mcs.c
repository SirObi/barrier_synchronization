#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "mpi_mcs_barrier.h"
#include "omp_mcs_barrier.h"


int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Expected argc is at least 3\n");
		return 0;
	}

	//printf("Starting test\n");

	int num_processes = atoi(argv[1]);
	int num_threads = atoi(argv[2]);
	int num_iterations = atoi(argv[3]);

    long int timespent_micro = 0;
    long int timespent_sec = 0;


    long int timespent = 0;

    MPI_Init(&argc, &argv);
    mpi_mcs_init(num_processes);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	struct timeval t0, t1;
	gettimeofday(&t0, NULL);

//	for (int i = 0; i < num_iterations; i++) {
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
//	}

	gettimeofday(&t1, NULL);
	timespent_micro += abs(t1.tv_usec - t0.tv_usec);
	timespent_sec += abs(t1.tv_sec - t0.tv_sec);

	//printf("Thread %d t0: %ld\n", world_rank, t0.tv_usec);
	//printf("Thread %d t1: %ld\n", world_rank, t1.tv_usec);

	// ??? is this even kosher? I don't think it gives you an average across processes
	printf("Time spent by process %d was %ld\n", world_rank, timespent_micro + timespent_sec * 1000000);


	MPI_Finalize();
}
