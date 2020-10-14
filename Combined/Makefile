MPICC?=mpicc
CFLAGS = -g -Wall -fopenmp -lm

test_combined_barrier_diss: test_combined_barrier_diss.c
	${MPICC} -o test_combined_barrier_diss test_combined_barrier_diss.c ../MPI/mpi_diss_barrier.c ../OpenMP/omp_mcs_barrier.c $(CFLAGS)

test_combined_barrier_mcs: test_combined_barrier_mcs.c
	${MPICC} -o test_combined_barrier_mcs test_combined_barrier_mcs.c ../MPI/mpi_mcs_barrier.c ../OpenMP/omp_mcs_barrier.c $(CFLAGS)
