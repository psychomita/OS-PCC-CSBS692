#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>

pid_t child_pid;

// Function to convert string to aLtErNaTe case
void to_alternate_case(char *str) {
    for (int i = 0; str[i]; i++) {
        if (isalpha(str[i])) {
            if (i % 2 == 0)
                str[i] = tolower(str[i]);
            else
                str[i] = toupper(str[i]);
        }
    }
}

// Child signal handler
void handle_sigusr1(int sig) {
    char input[1024];

    printf("\nChild Process Received Signal %d\n", sig);
    printf("Enter a string: ");
    fflush(stdout);

    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0'; // remove newline
        to_alternate_case(input);
        printf("aLtErNaTe Case: %s\n", input);
    }
}

// Parent signal handler for graceful exit
void handle_sigint(int sig) {
    printf("\nKeyboard Interrupt received. Terminating processes...\n");
    if (child_pid > 0)
        kill(child_pid, SIGTERM);  // Terminate child
    exit(0);  // Exit parent
}

int main() {
    signal(SIGINT, handle_sigint);  // Catch Ctrl+C

    child_pid = fork();

    if (child_pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Child process
        signal(SIGUSR1, handle_sigusr1);  // Handle SIGUSR1
        printf("Child process started. PID: %d\n", getpid());

        // Infinite loop to wait for signals
        while (1) {
            pause();  // Wait for signal
        }
    } else {
        // Parent process
        printf("Parent process started. PID: %d\n", getpid());
        printf("Sending signal to child every 10 seconds. Press Ctrl+C to stop.\n");

        while (1) {
            sleep(10);
            kill(child_pid, SIGUSR1);  // Send signal to child
        }
    }

    return 0;
}

