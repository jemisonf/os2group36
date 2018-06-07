#include <stdio.h>
#include <unistd.h>

#define claimedMemory syscall(354)
#define freeMemory syscall(353)

int main() {
	float fragmentation;

	printf("Running 3 tests:\n");

    int i;
	for (i = 0; i < 3; i++) {
		fragmentation = (float)freeMemory / (float)claimedMemory;
		printf("Claimed Memory: \t%lu\n", claimedMemory);
		printf("Free Memory: \t\t%lu\n", freeMemory);
		printf("Fragmentation: \t\t%f\n", fragmentation);
		printf("-----\n");
		sleep(1);
	}
}
