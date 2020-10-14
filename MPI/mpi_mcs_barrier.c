#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <mpi.h>
#include <math.h>
#include <string.h>


typedef struct {
	int upstreamnodes[4];
	int downstreamnode;
	int fanoutparent;
	int fanoutchildren[2];
} DistributedTreeNode;

DistributedTreeNode node;
int mpi_num_threads = 0;

void mpi_mcs_init(int num_threads){
	mpi_num_threads = num_threads;
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

// Set up arrival tree
	// Define which nodes fan into this node
	for (int j = 0; j < 4; j++) {
		if (4 * world_rank + j + 1 < mpi_num_threads) {
			node.upstreamnodes[j] = 4 * world_rank + j + 1;
		}
		else {
			node.upstreamnodes[j] = -1;
		}
	}

	// Define which nodes are immediately downstream from this node
	if (world_rank == 0) {
		node.downstreamnode = -1;
	}
	else {
		node.downstreamnode = (int)floor((world_rank-1) / 4);
	}


// Set up departure tree
	// Define what the node's first fan-out child is (or -1)
	if (2*world_rank + 1 >= num_threads) {
		node.fanoutchildren[0] = -1;
	}
	else {
		node.fanoutchildren[0] = 2*world_rank+1;
	}

	// Define what the node's second fan-out child is (or -1)
	if (2*world_rank + 2 >= num_threads) {
		node.fanoutchildren[1] = -1;
	}
	else {
		node.fanoutchildren[1] = 2*world_rank+2;
	}

	// Define node's fan-out parent
	if (world_rank != 0) {
		node.fanoutparent = (world_rank - 1) / 2;
	}
	else {
		node.fanoutparent = -1;
	}
}

void mpi_mcs_barrier(){
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

// Arrival phase
	// Wait until child nodes have reached the barrier
	int datatoreceive;
	for (int j = 0; j < 4; j++) {
		if (node.upstreamnodes[j] != -1) {
			MPI_Recv(&datatoreceive, 1, MPI_INT, node.upstreamnodes[j], 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}

	// Inform downstream node this node is ready
	int datatosend;
	if (world_rank != 0) {
		MPI_Send(&datatosend, 1, MPI_INT, node.downstreamnode, 0, MPI_COMM_WORLD);
	}

// Departure phase
	// Wait until fan-out parent is ready
	if (world_rank != 0) {
		MPI_Recv(&datatoreceive, 1, MPI_INT, node.fanoutparent, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	// Set fan-out children's parentsense
	if (2*world_rank + 1 < mpi_num_threads) {
		MPI_Send(&datatosend, 1, MPI_INT, node.fanoutchildren[0], 0, MPI_COMM_WORLD);
	}

	if (2*world_rank + 2 < mpi_num_threads) {
		MPI_Send(&datatosend, 1, MPI_INT, node.fanoutchildren[1], 0, MPI_COMM_WORLD);
	}
}

void mpi_mcs_finalize(){}
