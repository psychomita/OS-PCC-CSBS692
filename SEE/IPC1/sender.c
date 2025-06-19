#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"
#define MAX 100

void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = toupper(str[i]);
}

int main() {
    char inputStr[MAX], substring[MAX], finalStr[MAX];
    int start, count;

    // Create named pipes
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    pid_t pid = fork();

    if (pid > 0) {
        // Parent process - Sender
        printf("Sender Process (PID %d)\n", getpid());
        printf("Enter a lowercase string: ");
        fgets(inputStr, MAX, stdin);
        inputStr[strcspn(inputStr, "\n")] = '\0';  // remove newline

        printf("Enter starting index: ");
        scanf("%d", &start);

        printf("Enter number of characters: ");
        scanf("%d", &count);

        // Write to fifo1
        int fd1 = open(FIFO1, O_WRONLY);
        write(fd1, inputStr, sizeof(inputStr));
        write(fd1, &start, sizeof(start));
        write(fd1, &count, sizeof(count));
        close(fd1);

        // Read from fifo2
        int fd2 = open(FIFO2, O_RDONLY);
        read(fd2, finalStr, sizeof(finalStr));
        close(fd2);

        printf("Final string from receiver (uppercase substring): %s\n", finalStr);

        // Cleanup
        unlink(FIFO1);
        unlink(FIFO2);
    }
    else if (pid == 0) {
        // Child process - Receiver
        sleep(1); // Small delay to ensure sender writes first
        char buffer[MAX];
        int startIdx, charCount;

        int fd1 = open(FIFO1, O_RDONLY);
        read(fd1, buffer, sizeof(buffer));
        read(fd1, &startIdx, sizeof(startIdx));
        read(fd1, &charCount, sizeof(charCount));
        close(fd1);

        // Extract substring
        strncpy(substring, buffer + startIdx, charCount);
        substring[charCount] = '\0';

        // Convert to uppercase
        to_uppercase(substring);

        // Send back
        int fd2 = open(FIFO2, O_WRONLY);
        write(fd2, substring, sizeof(substring));
        close(fd2);
    }

    return 0;
}
