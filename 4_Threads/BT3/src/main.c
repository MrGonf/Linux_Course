#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define THRESHOLD 10

int data = 0;                         // Shared global variable
int data_ready = 0;                   // Flag to indicate data is ready
int total_data = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Producer thread function
void* producer(void* arg) {
    for (int i = 0; i < THRESHOLD; ++i) {
        sleep(1);  // Simulate some work

        // Generate random number
        int rand_num = rand() % 10 + 1;

        pthread_mutex_lock(&lock);
        data = rand_num;
        data_ready = 1;
        printf("=============== %d ==============\n", i+1);
        printf("[Producer] Generated: %d\n", data);
        pthread_cond_signal(&cond);   // Notify consumer
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

// Consumer thread function
void* consumer(void* arg) {
    for (int i = 0; i < THRESHOLD; ++i) {
        pthread_mutex_lock(&lock);
        while (data_ready == 0) {
            // Wait until producer signals that data is ready
            pthread_cond_wait(&cond, &lock);
        }

        printf("[Consumer] Consumed: %d\n", data);
        data_ready = 0;  // Reset flag after consumption
        total_data += data;
        pthread_mutex_unlock(&lock);
    }
    printf("=============== total ==============\n");
    printf("[Consumer] Consumed total: %d\n", total_data);
    return NULL;
}

int main() {
    int ret;
    srand(time(NULL));  // Seed random number generator

    pthread_t producer_thread, consumer_thread;

    // Create threads
    if ((ret = pthread_create(&producer_thread, NULL, producer, NULL)) != 0) {
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }
    
    if ((ret = pthread_create(&consumer_thread, NULL, consumer, NULL)) != 0) {
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }
    // Wait for both threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Clean up
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
