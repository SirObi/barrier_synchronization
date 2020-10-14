MPICC?=mpicc
CFLAGS = -g -Wall -lm

test_mpi_diss_barrier: test_mpi_diss_barrier.c
	${MPICC} -o test_mpi_diss_barrier test_mpi_diss_barrier.c mpi_diss_barrier.c $(CFLAGS)

test_mpi_mcs_barrier: test_mpi_mcs_barrier.c
	${MPICC} -o test_mpi_mcs_barrier test_mpi_mcs_barrier.c mpi_mcs_barrier.c $(CFLAGS)

test_mpi_builtin_barrier: test_mpi_builtin_barrier.c
	${MPICC} -o test_mpi_builtin_barrier test_mpi_builtin_barrier.c $(CFLAGS)
