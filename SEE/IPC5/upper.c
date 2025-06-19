#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <string.h>

pid_t child_pid;

void upper_case(char *str) {
    for (int i = 0; str[i]; i++) {
        if (isalpha(str[i]))
            str[i] = toupper(str[i]);
    }
}

void sigusr1(int sig) {
    char input[1024];
    printf("\n[Parent] Received signal %d (SIGUSR1)\n", sig);
    printf("Enter a string: ");
    fflush(stdout);
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        upper_case(input);
        printf("UPPER CASE: %s\n", input);
    }
}

void sigint(int sig) {
    printf("\nKeyboard Interrupt received. Terminating...\n");
    if (child_pid > 0)
        kill(child_pid, SIGTERM);
    exit(0);
}

int main() {
    signal(SIGINT, sigint);

    child_pid = fork();

    if (child_pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // CHILD: Send SIGUSR1 to parent every 5 seconds
        printf("\nChild: PID = %d\n", getpid());
        printf("Sending signal to parent every 5 seconds.\n");
        while (1) {
            sleep(5);
            kill(getppid(), SIGUSR1);
        }
    } else {
        // PARENT: Handle SIGUSR1
        signal(SIGUSR1, sigusr1);
        printf("\nParent: PID = %d\n", getpid());
        while (1)
            pause();
    }

    return 0;
}
