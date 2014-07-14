/**
	@how to compile
		mpicc -o mpi_prime mpi_prime.c
		
	@to execute (4 represents number of processors)
		mpirun -n 4 ./mpi_prime

**/
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int prime(int num) {

	int result = 0;
	
	if(num < 3) {
		if(num == 2)
			return 0;
		else
			return 1;
	}
	
	int i;
	int range = sqrt(num) + 1;
	
	if(num % 2 == 0)
		result = 1;
	else {
		for(i = 3; i < range && !result; i += 2) {
			if(num % i == 0)
				result = 1;
		}
	}
	
	return result;
}

int main(int argc, char** argv) {

	MPI_Init(NULL, NULL);

	// Range of numbers to find total number of primes from
	int TEST_NUM = 50000000;

	// Set local and total primes to 0. Count's will reduce to total_primes
	int count = 0;
	int total_primes = 0;

	// Sets number of processors
	int num_of_processors;
	MPI_Comm_size(MPI_COMM_WORLD, &num_of_processors);

	// Sets ranks of each process
	int process_id;
	MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

	// Start clock
	MPI_Barrier(MPI_COMM_WORLD);
	double elapsed_time = -MPI_Wtime();

	int     j, start, end, len;
    len     =  TEST_NUM / num_of_processors;
    start   =  process_id * len;
    end     =  start + len;

    // Check if number is prime. If so, increment the count.
    for(j = start; j <= end; j++) {
		if( prime(j) == 0 ) {
    		count++;
			//printf("PRIME HAS BEEN FOUND %d\n : ", j);
		}
	}

    // Sum up all local counts and store in total_primes
    MPI_Reduce(&count, &total_primes, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Get elapsed time
	MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time += MPI_Wtime();

    printf("Process %d had an elapsed time of %f\n", process_id, elapsed_time);

    MPI_Finalize();

    if(process_id == 0) // If this is root node
		printf("The amount of primes from 0 to %d is %d\n", TEST_NUM, total_primes);
 
 }
