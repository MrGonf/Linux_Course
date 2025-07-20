#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 3        // Number of threads
#define INCREMENTS 10000000   // Number of increments per thread

int counter = 0;             // Shared counter variable
pthread_mutex_t lock;        // Mutex to protect access to counter

// Thread function to increment the counter
void* thr_handler(void* arg) {
    for (int i = 0; i < INCREMENTS; i++) {
        pthread_mutex_lock(&lock);   // Lock the mutex before modifying the counter
        counter++;                   // Increment the shared counter
        pthread_mutex_unlock(&lock); // Unlock the mutex after modification
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Initialize the mutex
    pthread_mutex_init(&lock, NULL);

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thr_handler, NULL);
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&lock);

    printf("Expected counter value: %d\n", NUM_THREADS * INCREMENTS);
    // Print the final value of counter
    printf("Final counter value: %d\n", counter);

    return 0;
}
