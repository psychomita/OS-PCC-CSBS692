#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"
#define MAX_LEN 1024

int main() {
    char input[MAX_LEN];
    int start, length;
    char buffer[MAX_LEN];

    // Create the named pipes (FIFOs) if they do not already exist
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    // Take input string from the user
    printf("Enter a lowercase string: ");
    fgets(input, MAX_LEN, stdin);
    input[strcspn(input, "\n")] = '\0';  // Remove newline character from input

    // Take starting index for substring extraction
    printf("Enter starting index: ");
    scanf("%d", &start);

    // Take number of characters to extract
    printf("Enter number of characters: ");
    scanf("%d", &length);

    // Open FIFO1 for writing to send data to receiver
    int fd1 = open(FIFO1, O_WRONLY);
    if (fd1 < 0) {
        perror("Error opening fifo1");
        exit(EXIT_FAILURE);
    }

    // Format the data as: input string, starting index, and length (each separated by newline)
    // Limit string to 900 characters to avoid buffer overflow warning
    snprintf(buffer, sizeof(buffer), "%.900s\n%d\n%d", input, start, length);

    // Write the formatted message to FIFO1
    write(fd1, buffer, strlen(buffer) + 1);
    close(fd1);  // Close the FIFO after writing

    // Open FIFO2 for reading the processed result from receiver
    int fd2 = open(FIFO2, O_RDONLY);
    if (fd2 < 0) {
        perror("Error opening fifo2");
        exit(EXIT_FAILURE);
    }

    // Read the final uppercase result from the receiver
    read(fd2, buffer, sizeof(buffer));

    // Display the final result to the user
    printf("Answer from receiver: %s\n", buffer);
    close(fd2);  // Close the FIFO after reading

    return 0;
}

