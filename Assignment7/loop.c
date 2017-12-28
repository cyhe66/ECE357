#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>

#define BILLION 1000000000L


void empty_function(){
	getpid();
}

int main(int argc, char* argv[]){
	int ii;
	uint64_t diff;
	struct timespec start, end;

	uint64_t start_time = clock_gettime(CLOCK_MONOTONIC, &start);

	//empty for loop
	for (ii = 0; ii < 100000000; ii++){
		empty_function();
	}
	uint64_t end_time = clock_gettime(CLOCK_MONOTONIC, &end);
	diff = BILLION *(end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	printf("Time elapsed: %llu nanoseconds.", (long long unsigned int) diff);
	return 0;
}

