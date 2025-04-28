#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"
#define MAX_LEN 1024

int main() {
    char buffer[MAX_LEN], str[MAX_LEN];
    int start, len;

    // Open FIFO1 for reading the input data sent by the sender
    int fd1 = open(FIFO1, O_RDONLY);
    if (fd1 < 0) {
        perror("Error opening fifo1");
        exit(EXIT_FAILURE);
    }

    // Read data from the sender
    read(fd1, buffer, sizeof(buffer));
    close(fd1);  // Close FIFO1 after reading

    // Parse the received data which is expected in format: string\nstart\nlength
    char *token = strtok(buffer, "\n");
    if (!token) exit(EXIT_FAILURE);
    strcpy(str, token);  // Extract the input string

    token = strtok(NULL, "\n");
    if (!token) exit(EXIT_FAILURE);
    start = atoi(token);  // Extract starting index

    token = strtok(NULL, "\n");
    if (!token) exit(EXIT_FAILURE);
    len = atoi(token);  // Extract number of characters to convert

    // Validate the extracted inputs
    int str_len = strlen(str);
    if (start < 0 || start >= str_len || len < 0) {
        // If index or length is invalid, send an error message
        strcpy(buffer, "Invalid index or length.");
    } else {
        // Adjust length if it goes beyond the end of the string
        if (start + len > str_len)
            len = str_len - start;

        char result[MAX_LEN];
        // Copy the requested substring
        strncpy(result, str + start, len);
        result[len] = '\0';  // Null-terminate the substring

        // Convert each character in the substring to uppercase
        for (int i = 0; i < len; i++)
            result[i] = toupper(result[i]);

        // Prepare the buffer to send back to the sender
        strcpy(buffer, result);
    }

    // Open FIFO2 for writing the result back to the sender
    int fd2 = open(FIFO2, O_WRONLY);
    if (fd2 < 0) {
        perror("Error opening fifo2");
        exit(EXIT_FAILURE);
    }

    // Send the result back to the sender
    write(fd2, buffer, strlen(buffer) + 1);
    close(fd2);  // Close FIFO2 after writing

    return 0;
}

