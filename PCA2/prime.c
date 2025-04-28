#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define THREAD_COUNT 10

typedef struct {
    int start;
    int end;
    int index;
    int* primes;
    int* count;
} ThreadData;

int is_prime(int num) {
    if (num <= 1) return 0;
    if (num == 2) return 1;
    if (num % 2 == 0) return 0;
    for (int i = 3; i * i <= num; i += 2)
        if (num % i == 0)
            return 0;
    return 1;
}

void* find_primes(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int idx = 0;
    for (int i = data->start; i <= data->end; i++) {
        if (is_prime(i)) {
            data->primes[idx++] = i;
        }
    }
    data->count[data->index] = idx; // store count of primes found
    pthread_exit(NULL);
}

int main() {
    int N;
    printf("Enter the value of N: ");
    scanf("%d", &N);

    pthread_t threads[THREAD_COUNT];
    ThreadData thread_data[THREAD_COUNT];
    int* results[THREAD_COUNT];
    int count[THREAD_COUNT] = {0};

    int range = N / THREAD_COUNT;
    int remainder = N % THREAD_COUNT;

    for (int i = 0; i < THREAD_COUNT; i++) {
        int start = i * range + 1;
        int end = (i + 1) * range;
        if (i == THREAD_COUNT - 1)
            end += remainder; // last thread takes the remainder

        results[i] = (int*)malloc((end - start + 1) * sizeof(int)); // max possible size

        thread_data[i].start = start;
        thread_data[i].end = end;
        thread_data[i].index = i;
        thread_data[i].primes = results[i];
        thread_data[i].count = count;

        pthread_create(&threads[i], NULL, find_primes, &thread_data[i]);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nPrime numbers between 1 and %d:\n", N);
    for (int i = 0; i < THREAD_COUNT; i++) {
        for (int j = 0; j < count[i]; j++) {
            printf("%d ", results[i][j]);
        }
    }
    printf("\n");

    for (int i = 0; i < THREAD_COUNT; i++) {
        free(results[i]);
    }

    return 0;
}

