#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int* array;
    int start;
    int end;
    int ascending; // 1 for ascending, 0 for descending
} ThreadData;

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void sort(int* arr, int start, int end, int ascending) {
    for (int i = start; i < end - 1; i++) {
        for (int j = start; j < end - i + start - 1; j++) {
            if ((ascending && arr[j] > arr[j + 1]) || (!ascending && arr[j] < arr[j + 1])) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void* sort_thread(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    sort(data->array, data->start, data->end, data->ascending);
    return NULL;
}

int main() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int* arr = (int*)malloc(n * sizeof(int));
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    pthread_t t1, t2;
    ThreadData data1, data2;

    int mid = n / 2;

    data1.array = arr;
    data1.start = 0;
    data1.end = mid;
    data1.ascending = 1;

    data2.array = arr;
    data2.start = mid;
    data2.end = n;
    data2.ascending = 0;

    pthread_create(&t1, NULL, sort_thread, &data1);
    pthread_create(&t2, NULL, sort_thread, &data2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("\nFirst half sorted in ascending order:\n");
    for (int i = 0; i < mid; i++) {
        printf("%d ", arr[i]);
    }

    printf("\nSecond half sorted in descending order:\n");
    for (int i = mid; i < n; i++) {
        printf("%d ", arr[i]);
    }

    printf("\n");

    free(arr);
    return 0;
}

