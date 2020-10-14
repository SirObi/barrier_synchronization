# OpenMPI + OpenMP barrier synchronization

### How to build
`cd` to desired folder  

run `make test_<name of barrier>`  

for OpenMP barriers:  
run `./test_<name_of_barrier> <number of threads> <number of iterations inside the test>`,  
f.x. `./test_omp_mcs_barrier 2 10`  

for OpenMPI barriers:  
run `./test_<name_of_barrier> <number of processes> <number of iterations inside the test>`,  
f.x. `mpirun ./test_mpi_diss_barrier 2 10`  

for combined barriers:  
run `./test_<name_of_barrier> <number of processes> <number of threads> <number of iterations inside the test>`,  
f.x. `mpi_run ./test_combined_barrier 2 8 10`  

### Libraries used
openmp 
openmpi

### Main ideas
See the attached write-up for details on how to run the code.  

**Notes**:  
I never had the chance to use Georgia Tech's cluster.  
This set up was created to work on any cluster that has:  
- OpenMPI and OpenMP installed  
- SSH connectivity between nodes  
- all types of TCP traffic allowed between nodes  
 
---  
It is advisable to direct the outputs of the test commands to a text file.  