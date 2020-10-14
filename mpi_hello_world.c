#include <mpi.h>
#include <stdio.h>

//int counter = 0;

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    int data = 0;
    // Print off a hello world message
    if (world_rank == 0){
        data++;
    	MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else {
    	if (world_rank == 1){
    		MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    	}

        printf("Hello world from processor %s with counter %d, rank %d out of %d processors\n",
               processor_name, data, world_rank, world_size);
    }



    // Finalize the MPI environment.
    MPI_Finalize();
}
