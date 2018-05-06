#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#ifndef NUM_THREADS
#define NUM_THREADS 100
#endif

void* write_rand(void* args) 
{
	char str[1000];
	char filename[1000];
	int i;
	FILE * fp;
	for (i = 0; i < 1000; i++) {
		str[i] = (char) (rand() % 127) + 32;
	}
	sprintf(filename, "testfile%d", rand());
	fp = fopen(filename, "w+");
	fprintf(fp, "%s", str);
	pthread_exit(NULL);
}


int main(int argc, char* argv[]) 
{
	pthread_t threads[NUM_THREADS];	
	int i, terminated_threads = 0;
	char* ret;
	srand(time(NULL));
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_create(&threads[i], NULL, write_rand, NULL);
	}
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], (void**) &ret);
	}

}
