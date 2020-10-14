#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <mpi.h>
#include <math.h>
#include <string.h>

int numberthreads = 0;

void mpi_diss_init(int num_threads){
	numberthreads = num_threads;
}

void mpi_diss_barrier(){
	int datatosend;
	int datatoreceive;
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	for (int k = 0; k < log(numberthreads) - 1; k++) {
		for (int j = 0; j < numberthreads; j++) {
			if (j != world_rank) {
				if (j == (world_rank + (int)pow(2, k)) % numberthreads) {
					MPI_Send(&datatosend, 1, MPI_INT, j, 0, MPI_COMM_WORLD);
				}

				if (world_rank == (j + (int)pow(2, k)) % numberthreads) {
					MPI_Recv(&datatoreceive, 1, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}
			}
		}
	}
}

void mpi_diss_finalize(){}
