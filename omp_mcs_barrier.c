#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <omp.h>


// For future work with C++, use #define to define array sizes
// You can do it with consts in C (since it's more flexible), but just get in the habit


typedef struct {
	bool parentsense;
	bool *downstreampointer;
	bool *childpointers[2];
	bool haveupstream[4];
	bool upstreamnotready[4];
} TreeNode;

TreeNode *nodes;
int number_threads = 0;
bool global_sense = true;

void omp_mcs_init(int num_threads){
	number_threads = num_threads;
	nodes = malloc(sizeof(TreeNode) * num_threads);

	for (int i = 0; i < number_threads; i++) {

	// Set up arrival tree
		// For each node, define which nodes fan data into it
		for (int j = 0; j < 4; j++) {
			if (4 * i + j + 1 < number_threads) {
				nodes[i].haveupstream[j] = true;
			}
			else {
				nodes[i].haveupstream[j] = false;
			}
		}

		// For each node, define what its downstream node is, and which array slot on downstream node to update
		if (i == 0) {
			// NULLs are great for trees and testing you're at the end of tree etc
			nodes[i].downstreampointer = NULL;
		}
		else {
			nodes[i].downstreampointer = &nodes[(int)floor((i-1) / 4)].upstreamnotready[(i-1) % 4];
		}

		// Initially, set each immediate upstream node's "not-readiness" to true
		memcpy(nodes[i].upstreamnotready, nodes[i].haveupstream, sizeof(bool) * 4);

	// Set up departure tree
		// For each node, define what its first fan-out child is (or NULL)
		if (2*i + 1 >= num_threads) {
			nodes[i].childpointers[0] = NULL;
		}
		else {
			nodes[i].childpointers[0] = &nodes[2*i+1].parentsense;
		}

		// For each node, define what its second fan-out child is (or NULL)
		if (2*i + 2 >= num_threads) {
			nodes[i].childpointers[1] = NULL;
		}
		else {
			nodes[i].childpointers[1] = &nodes[2*i+2].parentsense;
		}

		// For each node, initially track parent's readiness as false
		nodes[i].parentsense = false;
	}
}

void omp_mcs_barrier(){
	int vpid = omp_get_thread_num();

	// Set value of sense each process has to have before it can continue
	// The value changes every round
	bool sense = global_sense;

// Arrival phase
	// Wait until child nodes have reached the barrier
	bool *children = nodes[vpid].upstreamnotready;
	while (children[0] || children[1] || children[2] || children[3]) {}

	// Reset children's readiness in preparation for next barrier
	memcpy(nodes[vpid].upstreamnotready, nodes[vpid].haveupstream, sizeof(bool) * 4);

	// Inform downstream node this node is ready
	if (vpid != 0) {
		*nodes[vpid].downstreampointer = false;
	}

// Departure phase
	// Wait until fan-out parent is ready
	if (vpid != 0) {
		while (!(sense == nodes[vpid].parentsense)) {}
	}

	// Set fan-out children's parentsense
	if (2*vpid + 1 < number_threads) {
		*nodes[vpid].childpointers[0] = sense;
	}

	if (2*vpid + 2 < number_threads) {
		*nodes[vpid].childpointers[1] = sense;
	}

	// Reset sense in preparation for next barrier
	global_sense = !sense;
}

void omp_mcs_finalize(){
	free(nodes);
}


// it's key to understand the algorithm first, and be able to explain it with words
// at that stage, implementing it becomes a pretty trivial task
// sometimes the lectures might not provide the most intuitive explanations, so it's worth looking for better ones
// My big mistake in this one (cost 2 hours) was not figuring out that we need a gloal AND a local sense variable
