#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 100

int numbers[SIZE];       // Array of 100 random integers
int even_count = 0;      // Counter for even numbers
int odd_count = 0;       // Counter for odd numbers

// Thread function to count and print even numbers
void* count_even(void* arg) {
    printf("\n================ Even numbers: ================\n");
    int count = 0;
    for (int i = 0; i < SIZE; i++) {
        if (numbers[i] % 2 == 0) {
            even_count++;
            printf("%3d ", numbers[i]);
            count++;
            if (count % 10 == 0) printf("\n");  // Newline every 10 numbers
        }
    }
    if (count % 10 != 0) printf("\n");  // Ensure final newline
    pthread_exit(NULL);
}

// Thread function to count and print odd numbers
void* count_odd(void* arg) {
    // Wait for the even thread to finish printing to avoid mixed output
    pthread_join(*(pthread_t*)arg, NULL);

    printf("\n================ Odd numbers: ================\n");
    int count = 0;
    for (int i = 0; i < SIZE; i++) {
        if (numbers[i] % 2 != 0) {
            odd_count++;
            printf("%3d ", numbers[i]);
            count++;
            if (count % 10 == 0) printf("\n");
        }
    }
    if (count % 10 != 0) printf("\n");
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    // Generate random numbers from 1 to 100
    for (int i = 0; i < SIZE; i++) {
        numbers[i] = (rand() % 100) + 1;
    }

    // Print the generated array
    printf("================ Generated array: ================\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%3d ", numbers[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }

    pthread_t even_thread, odd_thread;

    // Create the even-number thread
    pthread_create(&even_thread, NULL, count_even, NULL);
    // Pass thread1 as argument so the odd thread waits for even to finish printing
    pthread_create(&odd_thread, NULL, count_odd, &even_thread);

    // Wait for the odd-number thread (even already waited by odd thread)
    pthread_join(odd_thread, NULL);

    // Print final statistics
    printf("\nTotal even numbers: %d\n", even_count);
    printf("Total odd numbers: %d\n", odd_count);

    return 0;
}
