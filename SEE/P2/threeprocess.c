#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2, pid3;

    printf("Parent Process P:\n");
    printf("PID: %d, PPID: %d\n", getpid(), getppid());

    // Create first child
    pid1 = fork();

    if (pid1 == 0) {
        // In child 1
        printf("Child Process C1:\n");
        printf("PID: %d, PPID: %d\n", getpid(), getppid());
        sleep(10); // Pause for checking with ps
        exit(0);
    }
    else {
        // Create second child
        pid2 = fork();

        if (pid2 == 0) {
            // In child 2
            printf("Child Process C2:\n");
            printf("PID: %d, PPID: %d\n", getpid(), getppid());
            sleep(10); // Pause for checking with ps
            exit(0);
        }
        else {
            // Create third child
            pid3 = fork();

            if (pid3 == 0) {
                // In child 3
                printf("Child Process C3:\n");
                printf("PID: %d, PPID: %d\n", getpid(), getppid());
                sleep(10); // Pause for checking with ps
                exit(0);
            }
            else {
                // In parent, wait for all children
                wait(NULL);
                wait(NULL);
                wait(NULL);
            }
        }
    }

    return 0;
}
