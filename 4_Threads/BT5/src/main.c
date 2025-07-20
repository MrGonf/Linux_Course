#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2

int data = 0;  // Shared data
pthread_rwlock_t rwlock;  // Read-write lock

// Reader thread function
void* reader(void* arg) {
    int id = *((int*)arg);
    pthread_rwlock_rdlock(&rwlock);  // Acquire read lock
    printf("Reader %d reads data: %d\n", id, data);
    pthread_rwlock_unlock(&rwlock);  // Release lock
    return NULL;
}

// Writer thread function
void* writer(void* arg) {
    int id = *((int*)arg);
    pthread_rwlock_wrlock(&rwlock);  // Acquire write lock
    data += 1;
    printf("Writer %d updates data to: %d\n", id, data);
    pthread_rwlock_unlock(&rwlock);  // Release lock
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int reader_ids[NUM_READERS];
    int writer_ids[NUM_WRITERS];

    // Initialize the read-write lock
    pthread_rwlock_init(&rwlock, NULL);
/* ======================================================== */
    // Create reader threads
    for (int i = 0; i < NUM_READERS; ++i) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; ++i) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }
/* ======================================================== */

    // Create reader threads
    for (int i = 0; i < NUM_READERS; ++i) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }
/* ======================================================== */

    // Wait for all reader threads to finish
    for (int i = 0; i < NUM_READERS; ++i) {
        pthread_join(readers[i], NULL);
    }

    // Wait for all writer threads to finish
    for (int i = 0; i < NUM_WRITERS; ++i) {
        pthread_join(writers[i], NULL);
    }
/* ======================================================== */

    // Destroy the read-write lock
    pthread_rwlock_destroy(&rwlock);

    // Final data value
    printf("Final value of data: %d\n", data);

    return 0;
}
