#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/shm.h>
#include "psem.h"
#include "buffer.h"

const int SHMKEY = 1234;
const int SIZE = 100;

buffer *sbuf;

void handler(int sigNum);
char *generateName(int num);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Invalid syntax. Usage: ./philosopher <NUM> [init]\n");
        exit(1);
    }

    const int NUM = atoi(argv[1]); // Philosopher number
    signal(SIGINT, handler);

    int shmid;
    psem_t mutex1, mutex2;

    // Shared memory initialization
    if ((shmid = shmget(SHMKEY, sizeof(buffer), 0666 | IPC_CREAT)) == -1) {
        fprintf(stderr, "Failed to execute shmget\n");
        exit(1);
    }

    sbuf = (buffer *)shmat(shmid, NULL, 0);

    // Initialize shared buffer
    if (argc == 3 && getFinish(sbuf)) {
        fprintf(stderr, "All philosophers have already been seated\n");
        exit(1);
    } else if (argc == 3 && !getFinish(sbuf)) {
        buf_init(sbuf);  // Initialize shared buffer
        finished(sbuf);  // Mark initialization complete
    } else {
        buf_init(sbuf);
    }

    // Semaphore names for philosopher's forks
    int n1, n2;
    if (argc == 2) {
        n1 = NUM, n2 = NUM + 1;
    } else {
        n1 = NUM, n2 = 1; // Wrap around for the last philosopher
    }

    char *name1 = generateName(n1);
    char *name2 = generateName(n2);

    if (!psem_init(&mutex1, name1, 1)) {
        fprintf(stderr, "Failed to initialize semaphore\n");
        free(name1);
        free(name2);
        exit(1);
    }
    if (!psem_init(&mutex2, name2, 1)) {
        fprintf(stderr, "Failed to initialize semaphore\n");
        free(name1);
        free(name2);
        exit(1);
    }

    free(name1);
    free(name2);

    // Main loop: simulate thinking and eating
    while (getFlag(sbuf)) {
        printf("Philosopher %d is thinking\n", NUM);
        sleep(rand() % 3 + 1); // Simulate thinking time

        printf("Philosopher %d is hungry\n", NUM);

        // Deadlock prevention: odd-even strategy for picking up forks
        if (NUM % 2 == 0) {
            psem_wait(&mutex1);
            psem_wait(&mutex2);
        } else {
            psem_wait(&mutex2);
            psem_wait(&mutex1);
        }

        printf("Philosopher %d is eating\n", NUM);
        sleep(rand() % 3 + 1); // Simulate eating time

        // Release forks
        psem_post(&mutex1);
        psem_post(&mutex2);

        printf("Philosopher %d has finished eating\n", NUM);
        sleep(rand() % 3 + 1); // Simulate delay after eating
    }

    // Cleanup semaphores and shared memory
    printf("Philosopher %d exiting program\n", NUM);
    psem_destroy(&mutex1);
    psem_destroy(&mutex2);
    shmdt(sbuf);

    return 0;
}

// Signal handler for graceful termination
void handler(int sigNum) {
    if (sigNum == SIGINT) {
        changeFlag(sbuf); // Set the flag to stop all processes
        return;
    }
}

// Generate unique semaphore names
char *generateName(int num) {
    char *name = (char *)malloc(SIZE * sizeof(char));
    sprintf(name, "MUTEX_%d", num);
    return name;
}
