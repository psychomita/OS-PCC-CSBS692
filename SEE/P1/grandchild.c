#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid1, pid2;

    pid1 = fork(); // Create CP1 from parent P

    if (pid1 < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid1 == 0) {
        // Inside CP1
        printf("Child Process CP1:\n");
        printf("PID: %d, PPID: %d\n", getpid(), getppid());

        pid2 = fork(); // Create CP2 from CP1

        if (pid2 < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        else if (pid2 == 0) {
            // Inside CP2
            printf("Grandchild Process CP2:\n");
            printf("PID: %d, PPID: %d\n", getpid(), getppid());
        }
        else {
            // CP1 waits for CP2 to finish
            wait(NULL);
        }
    }
    else {
        // Inside Parent P
        printf("Parent Process P:\n");
        printf("PID: %d, PPID: %d\n", getpid(), getppid());

        // Parent waits for CP1
        wait(NULL);
    }

    return 0;
}
