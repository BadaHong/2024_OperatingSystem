#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int shared_resource = 0;
volatile int lock_flag = 0;

#define NUM_ITERS 10
#define NUM_THREADS 10
#define MAX_ATTEMPTS 10 //set the maximum number of backoff attempts
#define INITIAL_BACKOFF_INTERVAL 10 //set the initial backoff interval in microseconds
#define MAX_BACKOFF_INTERVAL 10000 //set the maximum backoff interval in microseconds

int compare_and_swap(volatile int*, int, int);
void lock();
void unlock();

int compare_and_swap(volatile int *ptr, int expected, int new_value) {
	int old_value;
	__asm__ volatile(
		"lock cmpxchg %2, %1\n\t"
		: "=a"(old_value), "+m"(*ptr)
		: "q"(new_value), "0"(expected)
		: "memory"
	);
	return old_value;
}

void lock() {
	int expected = 0;
	int attempts = 0; //attempts of backoff
	int delay = INITIAL_BACKOFF_INTERVAL; //start the delay time with initial backoff interval

	while (compare_and_swap(&lock_flag, expected, 1) != expected) {
	//exponential backoff: increase wait time exponentially up to a maximum number of attempts
		if (attempts < MAX_ATTEMPTS) {
			usleep(delay); // delay without using CPU
			// double the backoff interval, but till MAX_BACKOFF_INTERVAL
			delay = delay * 2 < MAX_BACKOFF_INTERVAL ? delay * 2 : MAX_BACKOFF_INTERVAL;
			attempts++;
        } else {
            // if maximum backoff attempts reached, use a fixed wait time
            usleep(MAX_BACKOFF_INTERVAL);
        }
    }
}

void unlock() {
	lock_flag = 0;
}

void* thread_func(void* arg) {
	int tid = *(int*)arg;

	lock();

		for(int i = 0; i < NUM_ITERS; i++)	shared_resource++;

	unlock();

	pthread_exit(NULL);
}

int main() {
	pthread_t threads[NUM_THREADS];
	int tids[NUM_THREADS];
	
	for (int i = 0; i < NUM_THREADS; i++) {
		tids[i] = i;
		pthread_create(&threads[i], NULL, thread_func, &tids[i]);
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("shared: %d\n", shared_resource);

	return 0;
}
