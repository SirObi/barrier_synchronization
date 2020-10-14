#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <mpi.h>
#include <math.h>
#include <string.h>
#include "mpi_diss_barrier.h"

/*
    From the MCS Paper: The scalable, distributed dissemination barrier with only local spinning.

    type flags = record
        myflags : array [0..1] of array [0..LogP - 1] of Boolean
		partnerflags : array [0..1] of array [0..LogP - 1] of ^Boolean

    processor private parity : integer := 0
    processor private sense : Boolean := true
    processor private localflags : ^flags

    shared nodes : array [0..P-1] of flags
        //nodes[i] is allocated in shared memory
	//locally accessible to processor i

    //on processor i, localflags points to nodes[i]
    //initially nodes[i].myflags[r][k] is false for all i, r, k
    //if j = (i+2^k) mod P, then for r = 0 , 1:
    //    nodes[i].partnerflags[r][k] points to nodes[j].myflags[r][k]

    procedure dissemination_barrier
        for instance : integer :0 to LogP-1
	    localflags^.partnerflags[parity][instance]^ := sense
	    repeat until localflags^.myflags[parity][instance] = sense
	if parity = 1
	    sense := not sense
	parity := 1 - parity
*/

typedef struct {
	bool **myflags;
	bool **partnerflags;
} Flags;

Flags *nodes;
int numberthreads = 0;

void mpi_diss_init(int num_threads){
	nodes = malloc(sizeof(Flags) * num_threads);
	numberthreads = num_threads;

	for (int i = 0; i < num_threads; i++) {
		nodes[i].myflags = malloc(sizeof(int*) * 2);
		for (int r = 0; r < 2; r ++) {
			//initially nodes[i].myflags[r][k] is false for all i, r, k
			nodes[i].myflags[r] = malloc(sizeof(int) * log(num_threads) - 1);
			memset(nodes[i].myflags[r], 0, sizeof(nodes[i].myflags[r]));
		}
	}
}

void mpi_diss_barrier(){
	printf("starting barrier\n");
	unsigned int parity = 0;
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int sense = true;
	Flags localflags = nodes[world_rank];

	for (int k = 0; k < log(numberthreads) - 1; k++) {
		int awaitfrom;
		for (int j = 0; j < numberthreads; j++) {
			if (j != world_rank) {
				if (j == (world_rank + (int)pow(2, k)) % numberthreads) {
					localflags.partnerflags[parity][k] = sense;
					MPI_Send(&localflags.partnerflags[parity][k], 1, MPI_INT, j, 0, MPI_COMM_WORLD);
				}

				if (world_rank == (j + (int)pow(2, k)) % numberthreads) {
					awaitfrom = j;
					printf("Process %d awaiting message from process %d\n", world_rank, awaitfrom);
				}
			}
		}

		while(localflags.myflags[parity][k] != sense) {
			printf("process %d trying to get message \n", world_rank);
			printf("value of myflags at parity %d and k %d is %d\n", parity, k, localflags.myflags[parity][k]);
			printf("its address was %p\n", &localflags.myflags[parity][k]);
			bool obi = 0;
			printf("awaitfrom was %d\n", awaitfrom);
			MPI_Recv(&obi, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//MPI_Recv(&localflags.myflags[parity][k], 1, MPI_C_BOOL, awaitfrom, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("process %d got message \n", world_rank);
		}
	}

	if (parity == 1) {
		sense = !sense;
	}
	parity -= 1;
}

// Work session so far:
// understand how to share variables in MPi (from 6:30 PM)
// implement test for builtin barrier
// understand missing bits from the paper's description of the algorithm (until 8 PM)
// using Feynman technique really helpful
// Another 2 hours trying to debug some horrible error
// ended up finding out the library solves 80% of the problem for you

void mpi_diss_finalize(){
	free(nodes);
}
