#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define THREAD_COUNT 10  // Total number of threads to use

// Structure to store data for each thread
typedef struct {
    int start;          // Start of range
    int end;            // End of range
    int index;          // Index of the thread
    int* primes;        // Array to store found primes
    int* count;         // Pointer to the array storing count of primes found by each thread
} ThreadData;

// Function to check if a number is prime
int is_prime(int num) {
    if (num <= 1) return 0;
    if (num == 2) return 1;
    if (num % 2 == 0) return 0;
    for (int i = 3; i * i <= num; i += 2)
        if (num % i == 0)
            return 0;
    return 1;
}

// Function executed by each thread
void* find_primes(void* arg) {
    ThreadData* data = (ThreadData*)arg;  // Cast argument to ThreadData pointer
    int idx = 0;  // Index to insert primes in the array

    // Loop over the assigned range and check for prime numbers
    for (int i = data->start; i <= data->end; i++) {
        if (is_prime(i)) {
            data->primes[idx++] = i;  // Store prime in array
        }
    }

    // Store how many primes were found by this thread
    data->count[data->index] = idx;

    pthread_exit(NULL);  // Exit the thread
}

int main() {
    int N;
    printf("Enter the value of N: ");
    scanf("%d", &N);  // Input upper limit for prime search

    pthread_t threads[THREAD_COUNT];            // Array to hold thread IDs
    ThreadData thread_data[THREAD_COUNT];       // Array to hold data for each thread
    int* results[THREAD_COUNT];                 // Array of pointers to hold prime results for each thread
    int count[THREAD_COUNT] = {0};              // Array to hold count of primes found by each thread

    int range = N / THREAD_COUNT;               // Divide N into roughly equal ranges for threads
    int remainder = N % THREAD_COUNT;           // Remainder to be handled by the last thread

    // Create threads and assign their respective ranges
    for (int i = 0; i < THREAD_COUNT; i++) {
        int start = i * range + 1;
        int end = (i + 1) * range;
        if (i == THREAD_COUNT - 1)
            end += remainder;  // Last thread gets the extra values if N is not divisible by THREAD_COUNT

        results[i] = (int*)malloc((end - start + 1) * sizeof(int));  // Allocate space for max possible primes

        // Fill the thread data structure
        thread_data[i].start = start;
        thread_data[i].end = end;
        thread_data[i].index = i;
        thread_data[i].primes = results[i];
        thread_data[i].count = count;

        // Create the thread
        pthread_create(&threads[i], NULL, find_primes, &thread_data[i]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the collected prime numbers
    printf("\nPrime numbers between 1 and %d:\n", N);
    for (int i = 0; i < THREAD_COUNT; i++) {
        for (int j = 0; j < count[i]; j++) {
            printf("%d ", results[i][j]);
        }
    }
    printf("\n");

    // Free dynamically allocated memory
    for (int i = 0; i < THREAD_COUNT; i++) {
        free(results[i]);
    }

    return 0;
}
