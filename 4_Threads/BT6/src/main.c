#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define ARRAY_SIZE 1000000
#define NUM_THREADS 4

int array[ARRAY_SIZE];          // Global array of integers
long long total_sum = 0;        // Global total sum
pthread_mutex_t sum_mutex;      // Mutex to protect total_sum

// Structure to pass data to each thread
typedef struct {
    int start_index;
    int end_index;
    int thread_id;
} ThreadData;

// Thread function to compute partial sum
void* partial_sum(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    long long local_sum = 0;

    // Calculate the sum of the assigned portion
    for (int i = data->start_index; i < data->end_index; i++) {
        local_sum += array[i];
    }

    // Print thread's local sum
    printf("Thread %d: local sum = %lld\n", data->thread_id, local_sum);

    // Lock mutex before updating the global total sum
    pthread_mutex_lock(&sum_mutex);
    total_sum += local_sum;
    pthread_mutex_unlock(&sum_mutex);

    free(data); // Free memory allocated for thread data
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&sum_mutex, NULL);

    // Initialize random values in the array
    srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % 100;  // Random number between 0 and 99
    }

    int part_size = ARRAY_SIZE / NUM_THREADS;

    // Create threads and assign work
    for (int i = 0; i < NUM_THREADS; i++) {
        ThreadData* data = (ThreadData*)malloc(sizeof(ThreadData));
        data->start_index = i * part_size;
        data->end_index = (i == NUM_THREADS - 1) ? ARRAY_SIZE : (i + 1) * part_size;
        data->thread_id = i;

        pthread_create(&threads[i], NULL, partial_sum, data);
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the final total sum
    printf("Total sum of array = %lld\n", total_sum);

    pthread_mutex_destroy(&sum_mutex);
    return 0;
}
