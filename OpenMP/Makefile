CC=gcc
CFLAGS=-Wall -fopenmp -lm
	
test_omp_mcs_barrier: ./test_omp_mcs_barrier.c
	$(CC) -o test_omp_mcs_barrier ./test_omp_mcs_barrier.c ./omp_mcs_barrier.c $(CFLAGS)

test_omp_senserev_barrier: ./test_omp_senserev_barrier.c
	$(CC) -o test_omp_senserev_barrier ./test_omp_senserev_barrier.c ./omp_senserev_barrier.c $(CFLAGS)

test_omp_builtin_barrier: ./test_omp_builtin_barrier.c
	$(CC) -o test_omp_builtin_barrier ./test_omp_builtin_barrier.c $(CFLAGS)