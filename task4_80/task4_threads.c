#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 4096
#define NUM_THREADS 2
#define BYTE_RANGE 256

// Defining what each thread needs to share
typedef struct {
	const char *fname;
	unsigned char *buffer;
	sem_t *sem_full;
	sem_t *sem_empty;
	pthread_mutex_t *mutex;
	int *bytes_in_buffer;
	int *count;
} thread_args_t;

int count[BYTE_RANGE];
unsigned char buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
pthread_cond_t cond_full, cond_empty;
int bytes_in_buffer = 0;

// Producer thread that reads chunks from the file into the shared buffer
void* thread_A(void* arg) {
	thread_args_t *a = (thread_args_t*)arg;
	FILE *fp = fopen(a->fname, "rb");
	int read_bytes;

	if(!fp) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

// Continuously reading from the file until end-of-file (when fread returns 0)
	for(;;) {
		sem_wait(a->sem_empty);
		pthread_mutex_lock(a->mutex);

// Reads up to whatever BUFFER_SIZE is in bytes, into the buffer
	read_bytes = fread(a->buffer + *a->bytes_in_buffer, 1,
	BUFFER_SIZE - *a->bytes_in_buffer, fp);

	*a->bytes_in_buffer += read_bytes; // Publishing the count to consumer thread
	pthread_mutex_unlock(a->mutex);

	sem_post(a->sem_full); // The signal data is ready
	if(read_bytes == 0) break; // EOF is reached, loop is stopped
	}

	fclose(fp);
	pthread_exit(NULL);
}

// Consumer thread, waits for data from the producer thread, and then tallies each byte value
void* thread_B(void* arg) {
	thread_args_t *a = (thread_args_t*)arg;
	int i, bytes;

// Continuously consuming buffer chunks until the producer signals EOF
	for(;;) {
		sem_wait(a->sem_full);
		pthread_mutex_lock(a->mutex);

// Updates the histogram (bytes_in_buffer) with a tally for each byte read
		bytes = *a->bytes_in_buffer;
		for(i = 0; i < bytes; i++) {
			a->count[a->buffer[i]]++;
		}

// Marks the buffer as empty before allowing producer to write again
		*a->bytes_in_buffer = 0;
		pthread_mutex_unlock(a->mutex);
		sem_post(a->sem_empty);

		if(bytes == 0) break; // Producer signals EOF, loop is stopped
	}
// Prints out the final 256 lines of the histogram
	for(i = 0; i < BYTE_RANGE; i++) {
		printf("%3d: %d\n", i, a->count[i]);
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	int i, rc;
	pthread_t threadA, threadB;
	sem_t sem_full, sem_empty;
	pthread_mutex_t mutex;
	void *buffer;
	int bytes_in_buffer = 0;
	int count[BYTE_RANGE];
	thread_args_t args;

// Validates the arguments
	if(argc != 2) {
		printf("Usage: %s <input-file>\n", argv[0]);
		free(buffer);
		return 1;
	}

	buffer = malloc(BUFFER_SIZE);
	if(!buffer) {
		perror("malloc");
		free(buffer);
		return 1;
	}

// Empties the histogram
	for(i = 0; i < BYTE_RANGE; i++) count[i] = 0;

// Initialize the primitives used for synchronization,
// starting with 0 full slots and 1 empty slot
	pthread_mutex_init(&mutex, NULL);
	sem_init(&sem_full, 0, 0);
	sem_init(&sem_empty, 0, 01);

// Packing arguments for both threads and then launching them
	args.fname = argv[1];
	args.buffer = buffer;
	args.sem_full = &sem_full;
	args.sem_empty = &sem_empty;
	args.mutex = &mutex;
	args.bytes_in_buffer = &bytes_in_buffer;
	args.count = count;

	if((rc = pthread_create(&threadA, NULL, thread_A, (void*)&args)) != 0) {
		perror("Could not create thread A");
		exit(1);
	}

	if((rc = pthread_create(&threadB, NULL, thread_B, (void*)&args)) != 0) {
		perror("Could not create thread B");
		exit(1);
	}

   if (pthread_join(threadA, NULL) != 0) {
      perror("Could not join thread A");
      exit(1);
   }
   if (pthread_join(threadB, NULL) != 0) {
      perror("Could not join thread B");
      exit(1);
   }

// Cleaning up
	sem_destroy(&sem_full);
	sem_destroy(&sem_empty);
	pthread_mutex_destroy(&mutex);
	free(buffer);
	return 0;
}










