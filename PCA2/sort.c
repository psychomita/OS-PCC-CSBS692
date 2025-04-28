#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Structure to pass data to each sorting thread
typedef struct {
    int* array;       // Pointer to the array to be sorted
    int start;        // Start index of the sub-array
    int end;          // End index (exclusive) of the sub-array
    int ascending;    // Sorting order flag: 1 for ascending, 0 for descending
} ThreadData;

// Function to swap two integers
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Bubble sort function with customizable order (ascending/descending)
void sort(int* arr, int start, int end, int ascending) {
    for (int i = start; i < end - 1; i++) {
        for (int j = start; j < end - i + start - 1; j++) {
            // Compare and swap based on the sorting order
            if ((ascending && arr[j] > arr[j + 1]) || (!ascending && arr[j] < arr[j + 1])) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

// Thread function to sort a portion of the array
void* sort_thread(void* arg) {
    ThreadData* data = (ThreadData*)arg;      // Cast argument to ThreadData
    sort(data->array, data->start, data->end, data->ascending);  // Call sort
    return NULL;
}

int main() {
    int n;

    // Take input for number of elements
    printf("Enter number of elements: ");
    scanf("%d", &n);

    // Allocate memory for the array
    int* arr = (int*)malloc(n * sizeof(int));

    // Take array input from user
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    // Create two thread objects
    pthread_t t1, t2;

    // Thread data for each half
    ThreadData data1, data2;
    int mid = n / 2;  // Middle index

    // Setup thread data for the first half (ascending sort)
    data1.array = arr;
    data1.start = 0;
    data1.end = mid;
    data1.ascending = 1;

    // Setup thread data for the second half (descending sort)
    data2.array = arr;
    data2.start = mid;
    data2.end = n;
    data2.ascending = 0;

    // Create threads to sort each half of the array
    pthread_create(&t1, NULL, sort_thread, &data1);
    pthread_create(&t2, NULL, sort_thread, &data2);

    // Wait for both threads to complete
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Print results
    printf("\nFirst half sorted in ascending order:\n");
    for (int i = 0; i < mid; i++) {
        printf("%d ", arr[i]);
    }

    printf("\nSecond half sorted in descending order:\n");
    for (int i = mid; i < n; i++) {
        printf("%d ", arr[i]);
    }

    printf("\n");

    // Free dynamically allocated memory
    free(arr);
    return 0;
}
