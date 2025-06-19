#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int pfd[2];
pid_t child_pid;

void sigusr1(int sig) {
    char input[1024];
    printf("\n[Parent] Received signal from child. Enter string: ");
    fflush(stdout);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove newline
    write(pfd[1], input, strlen(input) + 1); // Write only required
}

void sigabrt(int sig) {
    printf("\n[Parent] Received SIGABRT from child. Terminating...\n");
    exit(0);
}

int main() {
    if (pipe(pfd) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    signal(SIGUSR1, sigusr1);
    signal(SIGABRT, sigabrt);

    child_pid = fork();
    if (child_pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Child process
        close(pfd[1]); // Close write end
        char buffer[1024];

        while (1) {
            sleep(10);
            kill(getppid(), SIGUSR1); // Send signal to parent
            read(pfd[0], buffer, sizeof(buffer));

            if (strcmp(buffer, "MILK") == 0) {
                printf("\n[Child] Received MILK. PID = %d\n", getpid());
            } else {
                printf("\n[Child] Received '%s'. Sending SIGABRT.\n", buffer);
                kill(getppid(), SIGABRT);
                exit(0);
            }
        }
    } else {
        // Parent process
        close(pfd[0]); // Close read end
        printf("[Parent] PID = %d\n", getpid());
        while (1) {
            sleep(2);
            printf("[Parent] Alive. PID = %d\n", getpid());
        }
    }

    return 0;
}
